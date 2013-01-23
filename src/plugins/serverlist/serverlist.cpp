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

int threadlock_JoinGroup = 0;

void InitPlugin();

//////////////////////////////////////////////////////////////////////////
PLUGINLINK *pluginLink = 0;
PLUGININFO pluginInfo={
	sizeof(PLUGININFO),
	"NWNCX Serverlist",
	PLUGIN_MAKE_VERSION(1,0,0,0),
	"",
	"virusman & addicted2rpg",
	"virusman@virusman.ru & duckbreath@yahoo.com",
	"© 2013 virusman & addicted2rpg",
	"http://www.virusman.ru/",
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



/*
// This is if you are listing all servers and want to filter them by room yourself, you would use this.

int FilterGameType(int groupID, int skywingCode) {

	// skywingCode is named after Skywing, the guy who made it up.
	// Also, these next two lines may need the skywing codes switched (12 & 13).
	if(groupID == 274 && skywingCode == 12) return 0;   // either Action or Tech Support, going with Action
	if(groupID == 370 && skywingCode == 13) return 0;   // eitehr Action or Tech Support, going with Tech



	// I'm certain about these
	if(groupID == 275 && skywingCode == 3) return 0;  // Roleplay
	if(groupID == 276 && skywingCode == 4) return 0;  // Team
	if(groupID == 277 && skywingCode == 7) return 0;  // Social
	if(groupID == 278 && skywingCode == 9) return 0;  // PW Action
	if(groupID == 279 && skywingCode == 8) return 0;  // Alternative
	if(groupID == 363 && skywingCode == 1) return 0;  // Story
	if(groupID == 364 && skywingCode == 2) return 0;  // Story Lite
	if(groupID == 365 && skywingCode == 5) return 0;  // Melee
	if(groupID == 366 && skywingCode == 6) return 0;  // Arena
	if(groupID == 367 && skywingCode == 10) return 0; // PW Story
	if(groupID == 368 && skywingCode == 11) return 0; // Solo


	return 1;   // room mismatch, filter it.

}
*/


void AddServers(NWNMSClient *c) {
	int i, portnumber;
	ArrayOfNWGameServer *servers;	
	char *pvpfull = "FULL";
	char *pvpparty = "PARTY";
	char *pvpnone = "NONE";
	char *str_ptr;
	char buildstring[10];
	char *IP_ptr, *port_ptr;


	// The original function has a "Clear" routine that seems to be run after it.  I want this to run 
	// after that routine, so that is what the wait is about.  It works too.... after a fashion.
	Sleep(500);

	servers = c->GetServersInRoom(c->room);

	for(i=0; i < servers->__sizeNWGameServer;i++) {
		if(*(servers->NWGameServer[i]->Online) != 1) continue;
		// if(FilterGameType(c->room, *(servers->NWGameServer[i]->GameType))) continue;

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
			(void *)i, 
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
			c->room, // I think this is groupID
			*(servers->NWGameServer[i]->ELCEnforced),
			*(servers->NWGameServer[i]->ILREnforced),
			*(servers->NWGameServer[i]->LocalVault),
			*(servers->NWGameServer[i]->ExpansionsMask),
			false);

	}


	delete client;
	threadlock_JoinGroup = 0; // release the lock, the thread is done.
}



/*void (__fastcall *CConnectionLib__AddServer)(void *pConnectionLib, int edx,         
	     void * Server,
         const char * Hostname,
         const char * MapName,
         int NumPlayers,
         int MaxPlayers,
         int LevelRngLow,
         int LevelRngHigh,
         const char * PVP, // "NONE", "PARTY", "FULL"
         int PingTime,
         int HasPlayerPassword,
         const char *IPAddress,
         int PortNumber,
         int OnePartyOnly,
         int PlayerPause,
         const char *Build,
         const char *Details,
         int GroupId,
         int ELC,
         int ILR,
         int LocalVault,
         int Expansions,
         bool BuildNumberFilter
);*/

int (__fastcall *CGameSpyClient__JoinGroupRoom)(void *pGameSpy, int edx, int nRoom);
int __fastcall CGameSpyClient__JoinGroupRoom_Hook(void *pGameSpy, int edx, int nRoom)
{
	
		
	SIZE_T stack_size = 0;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR SD;
	LPTHREAD_START_ROUTINE  start_address = (LPTHREAD_START_ROUTINE) AddServers;
	LPDWORD threadID;	
	HANDLE thandle;

	while(threadlock_JoinGroup) {
		// do nothing.  Wait for the other thread to finish and then we can continue.
		// Race condition is low, unless someone goes clicktastic in the game UI
		continue;
	}
	

	client = new NWNMSClient(logFile);

	fprintf(logFile, "Requesting room %d\n", nRoom);
	fflush(logFile);

	client->room = nRoom;
	client->threadId = GetThreadId(GetCurrentThread());
		
	InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&SD, TRUE, NULL, FALSE);
    sa.nLength = sizeof (SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = &SD;
    sa.bInheritHandle = TRUE;

	thandle = CreateThread(&sa, 0, start_address, (void *)client, CREATE_SUSPENDED, NULL);

	threadlock_JoinGroup = 1;
	ResumeThread(thandle);

	//AddServers(client);  -- thread now handles this call


	return 	CGameSpyClient__JoinGroupRoom(pGameSpy, edx, nRoom);;
}


void HookFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	*(DWORD*)&CGameSpyClient__JoinGroupRoom = 0x00805080;
	//*(DWORD*)&CConnectionLib__AddServer = 0x008029C0;
	int success = DetourAttach(&(PVOID&)CGameSpyClient__JoinGroupRoom, CGameSpyClient__JoinGroupRoom_Hook)==0;

	fprintf(logFile, "Hooked: %d\n", success);
	DetourTransactionCommit();
}

void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWCX Serverlist plugin 1.0 (PRE-RELEASE)\n");
	fprintf(logFile, "(c) 2013 by virusman & addicted2rpg\n");
	fflush(logFile);
	if(pluginLink){
	}
	HookFunctions();

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		//delete plugin;
	}
	return TRUE;
}