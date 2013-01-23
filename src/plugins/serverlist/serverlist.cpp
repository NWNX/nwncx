#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <pluginapi.h>
#include "detours.h"
#include <m_common.h>
#include <nwnapi\nwnapi.h>
#include <nwnapi\CNWMessage.h>
#include <nwnapi\CTlkTable.h>
#include <nwnapi\CExoResMan.h>
#include <nwnapi\CNWRules.h>
#include <nwnapi\CAppManager.h>
#include <nwnapi\custom\nwn_globals.h>
#include <nwnapi\custom\nwn_globals.cpp>
#include "NWNMSClient.h"

FILE *logFile;
char logFileName[] = "logs/nwncx_serverlist.txt";

void InitPlugin();

//////////////////////////////////////////////////////////////////////////
PLUGINLINK *pluginLink = 0;
PLUGININFO pluginInfo={
	sizeof(PLUGININFO),
	"NWNCX Serverlist",
	PLUGIN_MAKE_VERSION(1,0,0,0),
	"",
	"virusman",
	"virusman@virusman.ru",
	"© 2013 virusman",
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
	fprintf(logFile, "Requesting room %d\n", nRoom);
	fflush(logFile);
	NWNMSClient client(logFile);
	client.GetServersInRoom(nRoom);
	g_pAppManager->ClientExoApp->Internal->m_pConnectionLib->AddServer((void *)0x00805080, "ServerName", "ModuleName", 5, 15, 1, 20, "FULL", 0, 0, "88.198.48.254", 5121, 0, 0, "8109", "Group", nRoom, 1, 1, 0, 0, true);
	//return CGameSpyClient__JoinGroupRoom(pGameSpy, edx, nRoom);
	return true;
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
	fprintf(logFile, "(c) 2013 by virusman\n");
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