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

#define DATA_DOING_NOTHING 0
#define DATA_OLD 1
#define DATA_FETCHING 2
#define DATA_READY 3

#define LOG_THREADS 1

FILE *logFile;
char logFileName[] = "logs/nwncx_serverlist.txt";
NWNMSClient *client;

int joingroup_called = 0;

HANDLE threadlock_JoinGroup;
HANDLE threadlock_FetchServers;
HANDLE thandle;
int data_status = DATA_DOING_NOTHING;   // a righteous initialization.
int roomGlobal = -1;

void InitPlugin();
void CreateMutexes();
void InitThread();

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



void FetchServers(int nRoom) {

	if(LOG_THREADS) {
		fprintf(logFile, "Child thread acquiring FetchServers....");
		fflush(logFile);
	}
	WaitForSingleObject(threadlock_FetchServers, INFINITE);
	if(LOG_THREADS) {
		fprintf(logFile, "ACQUIRED (C)\n");
		fprintf(logFile, "Child thread acquiring JoinGroup....");
		fflush(logFile);
	}
	WaitForSingleObject(threadlock_JoinGroup, INFINITE);
	if(LOG_THREADS) {
		fprintf(logFile, "ACQUIRED (C)\n");
		fflush(logFile);
	}

	// This, thankfully, never happens.  If somehow it does, we really need to know about it.
	if(data_status == DATA_READY) { 
		fprintf(logFile, "* Serious mutex error in FetchServers().  Report in a post nwnx.org\n");
		fflush(logFile);
		ExitThread(0);
		return;
	}

	data_status = DATA_FETCHING;
	fprintf(logFile, "Class initialization & pulling rooms, but not adding them to the NWN client yet.\n");
	fflush(logFile);
	client = new NWNMSClient(logFile, g_pAppManager);


	client->GetServersInRoom(nRoom);  
	data_status = DATA_READY;

	fprintf(logFile, "Rooms pulled, data is ready.\n");
	fflush(logFile);

	if(LOG_THREADS) {
		fprintf(logFile, "Child thread releasing FetchServers....");
		fflush(logFile);
	}
	ReleaseMutex(threadlock_FetchServers);
	if(LOG_THREADS) {
		fprintf(logFile, "RELEASED (C)\n");
		fflush(logFile);
	}

	if(LOG_THREADS) {
		fprintf(logFile, "Child thread releasing JoinGroup....");
		fflush(logFile);
	}
	ReleaseMutex(threadlock_JoinGroup);
	if(LOG_THREADS) {
		fprintf(logFile, "RELEASED (C)\n");
		fflush(logFile);
	}


	// ExitThread(0);
}

void Poll() {

	if(data_status == DATA_READY) {
		fprintf(logFile, "Adding servers to Neverwinter Nights client\n");
		client->AddServers();
		delete client;
		data_status = DATA_OLD;
		fprintf(logFile, "Done!\n\n\n");
		fflush(logFile);
	}

	if(LOG_THREADS) {
		fprintf(logFile, "Main thread releasing FetchServers....");
		fflush(logFile);
	}
	ReleaseMutex(threadlock_FetchServers);
	if(LOG_THREADS) {
		fprintf(logFile, "RELEASED (M)\n");
		fflush(logFile);
	}


}


int (__fastcall *CGameSpyClient__JoinGroupRoom)(void *pGameSpy, int edx, int nRoom);
int __fastcall CGameSpyClient__JoinGroupRoom_Hook(void *pGameSpy, int edx, int nRoom)
{		


	if(LOG_THREADS) {
		fprintf(logFile, "Main thread acquiring JoinGroup....");
		fflush(logFile);
	}
	WaitForSingleObject(threadlock_JoinGroup, INFINITE);
	if(LOG_THREADS) {
		fprintf(logFile, "ACQUIRED (M)\n");
		fflush(logFile);
	}

	joingroup_called = 1;

	roomGlobal = nRoom;

	InitThread();
	ResumeThread(thandle);

	if(LOG_THREADS) {
		fprintf(logFile, "Main thread releasing JoinGroup....");
		fflush(logFile);
	}
	ReleaseMutex(threadlock_JoinGroup);
	if(LOG_THREADS) {
		fprintf(logFile, "RELEASED (M)\n");
		fflush(logFile);
	}


	//return 	CGameSpyClient__JoinGroupRoom(pGameSpy, edx, nRoom);
	return true;
}

void (__fastcall *CConnectionLib__UpdateGameSpyClient)();
void __fastcall CConnectionLib__UpdateGameSpyClient_Hook(CConnectionLib *pConnectionLib)
{
	// We are waiting because this thread also modifies data_status
	if(joingroup_called) {
		if(LOG_THREADS) {
			fprintf(logFile, "Main thread [POLL direction] acquiring FetchServers....");
			fflush(logFile);
		}
		WaitForSingleObject(threadlock_FetchServers, INFINITE);
		if(LOG_THREADS) {
			fprintf(logFile, "ACQUIRED (M)\n");
			fflush(logFile);
		}
		Poll();		
	}
	

	CConnectionLib__UpdateGameSpyClient();
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
	fprintf(logFile, "NWCX Serverlist plugin 1.0 (PRE-RELEASE)\n");
	fprintf(logFile, "(c) 2013 by virusman & addicted2rpg\n");
	fflush(logFile);
	if(pluginLink){
	}
	HookFunctions();
	CreateMutexes();
}

void CreateMutexes() {

	//threadlock_JoinGroup = CreateMutexA(&sa, FALSE, "JoinGroup");
	//threadlock_FetchServers = CreateMutexA(&sa, FALSE, "FetchServers");
	threadlock_JoinGroup = CreateMutexA(NULL, FALSE, "JoinGroup");
	threadlock_FetchServers = CreateMutexA(NULL, FALSE, "FetchServers");

	if(threadlock_JoinGroup == NULL) {
		fprintf(logFile, "* Fatal - JoinGroup mutex was null.\n");
		fflush(logFile);
		ExitProcess(0);
		return;
	}

	if(threadlock_FetchServers == NULL) {
		fprintf(logFile, "* Fatal - FetchServers mutex was null.\n");
		fflush(logFile);
		ExitProcess(0);
		return;
	}



}

void InitThread() {
	SIZE_T stack_size = 0;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR SD;
	LPTHREAD_START_ROUTINE  start_address = (LPTHREAD_START_ROUTINE) FetchServers;

	InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&SD, TRUE, NULL, FALSE);
    sa.nLength = sizeof (SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = &SD;
    sa.bInheritHandle = TRUE;

	thandle = CreateThread(&sa, 0, start_address, (void *)roomGlobal, CREATE_SUSPENDED, NULL);
	if(thandle == NULL) {
		fprintf(logFile, "* Fatal - Thread creation failed.\n");
		fflush(logFile);
		ExitProcess(0);
		return;
	}


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
		CloseHandle(threadlock_JoinGroup);
		CloseHandle(threadlock_FetchServers);
		CloseHandle(thandle);
		//delete plugin;
	}
	return TRUE;
}