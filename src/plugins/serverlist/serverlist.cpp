#include "NWNMSClient.h"
#include "CGameSpyStub.h"
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
NWNMSClient *msClient;
CGameSpyStub *gameSpyClient;

void InitPlugin();

//////////////////////////////////////////////////////////////////////////
PLUGINLINK *pluginLink = 0;
PLUGININFO pluginInfo={
	sizeof(PLUGININFO),
	"NWNCX Serverlist",
	PLUGIN_MAKE_VERSION(2,0,0,0),
	"",
	"virusman & addicted2rpg",
	"virusman@virusman.ru & duckbreath@yahoo.com",
	"© 2013-2014 virusman & addicted2rpg",
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
	msClient->RequestServerList(nRoom);

	//CGameSpyClient__JoinGroupRoom(pGameSpy, edx, nRoom);
	return true;
}

void (__fastcall *CConnectionLib__UpdateGameSpyClient)();
void __fastcall CConnectionLib__UpdateGameSpyClient_Hook(CConnectionLib *pConnectionLib)
{
	CConnectionLib__UpdateGameSpyClient();
	gameSpyClient->peerThink();
	msClient->Update();
}

void (__cdecl *__peerConnect)(void *gsClient, void *unk1, void *unk2,
							void *pNickErrorCallback, CGameSpyStub::ConnectCallbackProc pConnectCallback,
							CConnectionLib *pConnectionLib, void *pCallbackContext);
void __cdecl peerConnect_Hook(void *gsClient, void *unk1, void *unk2,
	void *pNickErrorCallback, CGameSpyStub::ConnectCallbackProc pConnectCallback,
	CConnectionLib *pConnectionLib, void *pCallbackContext)
{
	fprintf(logFile, "peerConnect\n");	fflush(logFile);
	gameSpyClient->peerConnect(pConnectCallback, pConnectionLib);
}

void (__cdecl *__peerListGroupRooms)(void *gsClient, void *pCallbackContext, CGameSpyStub::ListGroupRoomsCallbackProc pListGroupRoomsCallback,
									void *pConnectionLib, void *unk1);
void __cdecl peerListGroupRooms_Hook(void *gsClient, void *pCallbackContext, CGameSpyStub::ListGroupRoomsCallbackProc pListGroupRoomsCallback,
									void *pConnectionLib, void *unk)
{
	fprintf(logFile, "peerListGroupRooms\n");	fflush(logFile);
	gameSpyClient->peerListGroupRooms(pListGroupRoomsCallback);
}

void (__cdecl *__peerJoinGroupRoom)(void *gsClient, int nGroupID, CGameSpyStub::JoinGroupRoomCallbackProc pJoinGroupRoomCallback,
									void *unk1, void *unk2);
void __cdecl peerJoinGroupRoom_Hook(void *gsClient, int nGroupID, CGameSpyStub::JoinGroupRoomCallbackProc pJoinGroupRoomCallback,
									void *unk1, void *unk2)
{
	fprintf(logFile, "peerJoinGroupRoom\n");	fflush(logFile);
	gameSpyClient->peerJoinGroupRoom(pJoinGroupRoomCallback);
}

void (__cdecl *__peerStartListingGames)(void *gsClient, int nGroupID, CGameSpyStub::ListingGamesCallbackProc pListingGamesCallback,
	void *unk1, void *unk2);
void __cdecl peerStartListingGames_Hook(void *gsClient, int nGroupID, CGameSpyStub::ListingGamesCallbackProc pListingGamesCallback,
	void *unk1, void *unk2)
{
	fprintf(logFile, "peerStartListingGames\n");	fflush(logFile);
	gameSpyClient->peerStartListingGames(pListingGamesCallback);
}

void (__fastcall *CConnectionLib__UpdateConnectionPhase)(CConnectionLib *pConnectionLib, int edx, unsigned char phase, CExoString const &context);
void __fastcall CConnectionLib__UpdateConnectionPhase_Hook(CConnectionLib *pConnectionLib, int edx, unsigned char phase, CExoString const &context)
{
	fprintf(logFile, "UpdateConnectionPhase: %d\n", phase);	fflush(logFile);
	CConnectionLib__UpdateConnectionPhase(pConnectionLib, edx, phase, context);
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
}

void HookFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	*(DWORD*)&CGameSpyClient__JoinGroupRoom = 0x00805080;
	*(DWORD*)&CConnectionLib__UpdateGameSpyClient = 0x008027A0;
	*(DWORD*)&__peerConnect = 0x00806480;
	*(DWORD*)&__peerListGroupRooms = 0x00807570;
	*(DWORD*)&__peerJoinGroupRoom = 0x008071F0;
	*(DWORD*)&__peerStartListingGames = 0x00807800;
	*(DWORD*)&CConnectionLib__UpdateConnectionPhase = 0x008027E0;

	int success = DetourAttach(&(PVOID&)CGameSpyClient__JoinGroupRoom, CGameSpyClient__JoinGroupRoom_Hook)==0  &&
				  DetourAttach(&(PVOID&)CConnectionLib__UpdateGameSpyClient, CConnectionLib__UpdateGameSpyClient_Hook)==0 &&
				  DetourAttach(&(PVOID&)__peerConnect, peerConnect_Hook)==0 &&
				  DetourAttach(&(PVOID&)__peerListGroupRooms, peerListGroupRooms_Hook)==0 &&
				  DetourAttach(&(PVOID&)__peerJoinGroupRoom, peerJoinGroupRoom_Hook)==0 &&
				  DetourAttach(&(PVOID&)__peerStartListingGames, peerStartListingGames_Hook)==0 &&
				  DetourAttach(&(PVOID&)CConnectionLib__UpdateConnectionPhase, CConnectionLib__UpdateConnectionPhase_Hook)==0;

	fprintf(logFile, "Hooked: %d\n", success);
	DetourTransactionCommit();
}

void InitPlugin()
{
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWCX Serverlist plugin 2.0.0\n");
	fprintf(logFile, "(c) 2013-2014 by virusman & addicted2rpg\n");
	fflush(logFile);
	if(pluginLink){
	}
	HookFunctions();
	//PatchImage();
	msClient = new NWNMSClient(logFile, ServerListCallback);
	gameSpyClient = new CGameSpyStub(logFile);
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