#include "NWNMSClient.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <pluginapi.h>
#include "detours.h"
//#include <m_common.h>
#include <nwnapi\nwnapi.h>
#include <nwnapi\CNWMessage.h>
#include <nwnapi\CTlkTable.h>
#include <nwnapi\CExoResMan.h>
#include <nwnapi\CNWRules.h>
#include <nwnapi\CAppManager.h>
#include <nwnapi\custom\nwn_globals.h>
#include <nwnapi\custom\nwn_globals.cpp>

FILE *logFile;
char logFileName[] = "logs/nwncx_serverlist.txt";
NWNMSClient *client;

void InitPlugin();

//////////////////////////////////////////////////////////////////////////
PLUGINLINK *pluginLink = 0;
PLUGININFO pluginInfo={
	sizeof(PLUGININFO),
	"NWNCX Serverlist",
	PLUGIN_MAKE_VERSION(1,0,1,0),
	"",
	"virusman & addicted2rpg",
	"virusman@virusman.ru & duckbreath@yahoo.com",
	"© 2013 virusman & addicted2rpg",
	"http://www.nwnx.org/",
	0		//not transient
};

extern "C" __declspec(dllexport) PLUGININFO* GetPluginInfo(DWORD nwnxVersion)
{
	return &pluginInfo;
}

extern "C" __declspec(dllexport) int InitPlugin(PLUGINLINK *link)
{
	pluginLink=link;
	InitPlugin();
	return 0;
}

void ServerListCallback(ServerListResult result)
{
	ArrayOfNWGameServer *servers = result.servers;
	int roomId = result.roomId;

	fprintf(logFile, "Callback called!\n");
	fflush(logFile);

	int i, portnumber;
	char *pvpfull = "FULL";
	char *pvpparty = "PARTY";
	char *pvpnone = "NONE";
	char *str_ptr;
	char buildstring[10];
	char *IP_ptr, *port_ptr;

	// The original function has a "Clear" routine that seems to be run after it.  I want this to run 
	// after that routine, so that is what the wait is about.  It works too.... after a fashion.
	// Sleep(500);

	g_pAppManager->ClientExoApp->Internal->m_pConnectionLib->ClearServers();

	for(i=0; i < servers->__sizeNWGameServer;i++) {
		if(*(servers->NWGameServer[i]->Online) != 1) continue;

		IP_ptr = strtok(servers->NWGameServer[i]->ServerAddress, ":");
		port_ptr = IP_ptr + strlen(IP_ptr) + 1;   // I never really trusted strtok(NULL) anyway :)

		if(*(servers->NWGameServer[i]->PVPLevel) == 0) {
			str_ptr = pvpnone;
		}
		else if(*(servers->NWGameServer[i]->PVPLevel) == 1) {
			str_ptr = pvpparty;
		}
		else {
			str_ptr = pvpfull;
		}
		itoa(*(servers->NWGameServer[i]->BuildNumber), buildstring, 10);
		portnumber = atoi(port_ptr);
//		fprintf(logFile, "Online: %d Server Address: %s  Port Number: %s Room: %d\n", *(servers->NWGameServer[i]->Online), IP_ptr, port_ptr, *(servers->NWGameServer[i]->GameType));		

		g_pAppManager->ClientExoApp->Internal->m_pConnectionLib->AddServer(
			(void *)(i+1), 
			servers->NWGameServer[i]->ServerName, 
			servers->NWGameServer[i]->ModuleName,
			*(servers->NWGameServer[i]->ActivePlayerCount),
			*(servers->NWGameServer[i]->MaximumPlayerCount),
			*(servers->NWGameServer[i]->MinimumLevel),
			*(servers->NWGameServer[i]->MaximumLevel),
			str_ptr,
			75, // making up ping initially...
			*(servers->NWGameServer[i]->PrivateServer), 
			IP_ptr,
			portnumber, 
			*(servers->NWGameServer[i]->OnePartyOnly),
			*(servers->NWGameServer[i]->PlayerPause),
			buildstring, 
			servers->NWGameServer[i]->ServerDescription, 
			roomId, // I think this is groupID
			*(servers->NWGameServer[i]->ELCEnforced),
			*(servers->NWGameServer[i]->ILREnforced),
			*(servers->NWGameServer[i]->LocalVault),
			*(servers->NWGameServer[i]->ExpansionsMask),
			false);

	}


	g_pAppManager->ClientExoApp->Internal->m_pConnectionLib->UpdateConnectionPhase(9, "");

	fprintf(logFile, "%d Servers added\n", i);
	fflush(logFile);
}

int (__fastcall *CGameSpyClient__JoinGroupRoom)(void *pGameSpy, int edx, int nRoom);
int __fastcall CGameSpyClient__JoinGroupRoom_Hook(void *pGameSpy, int edx, int nRoom)
{		
	client->RequestServerList(nRoom);

	CGameSpyClient__JoinGroupRoom(pGameSpy, edx, nRoom);
	return true;
}

void (__fastcall *CConnectionLib__UpdateGameSpyClient)();
void __fastcall CConnectionLib__UpdateGameSpyClient_Hook(CConnectionLib *pConnectionLib)
{
	CConnectionLib__UpdateGameSpyClient();
	client->Update();
}

void EnableWrite (unsigned long location)
{
	unsigned char *page;
	DWORD oldAlloc;
	page = (unsigned char *) location;
	VirtualProtect(page, 0xFF, PAGE_EXECUTE_READWRITE, &oldAlloc);
}

void PatchImage()
{
	char *patch = (char *) 0x008050C1;
	EnableWrite((DWORD) patch);
	memset(patch, 0x90, 5);                 //remove call to _peerStopListingGames

	patch = (char *) 0x008050CC;
	EnableWrite((DWORD) patch);
	memset(patch, 0x90, 5);                 //remove call to CConnectionLib::ClearServers()

	patch = (char *) 0x00805210;
	EnableWrite((DWORD) patch);
	memset(patch, 0x90, 5);                 //remove call to _peerStartListingGames
}

void HookFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	*(DWORD*)&CGameSpyClient__JoinGroupRoom = 0x00805080;
	*(DWORD*)&CConnectionLib__UpdateGameSpyClient = 0x008027A0;
	int success = DetourAttach(&(PVOID&)CGameSpyClient__JoinGroupRoom, CGameSpyClient__JoinGroupRoom_Hook)==0  &&
				  DetourAttach(&(PVOID&)CConnectionLib__UpdateGameSpyClient, CConnectionLib__UpdateGameSpyClient_Hook)==0;

	fprintf(logFile, "Hooked: %d\n", success);
	DetourTransactionCommit();
}

void InitPlugin()
{
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWCX Serverlist plugin 1.0.2\n");
	fprintf(logFile, "(c) 2013 by virusman & addicted2rpg\n");
	fflush(logFile);
	if(pluginLink){
	}
	HookFunctions();
	PatchImage();
	client = new NWNMSClient(logFile, ServerListCallback);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		fprintf(logFile, "* Plugin exit.  Cleaning up....\n");
		fflush(logFile);
	}
	return TRUE;
}