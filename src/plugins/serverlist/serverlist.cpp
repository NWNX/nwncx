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
#define UI_LOAD_TIME 2000


FILE *logFile;
char logFileName[] = "logs/nwncx_serverlist.txt";
NWNMSClient *client;
int join_attempted = 0;

int threadlock_JoinGroup = 0;
int data_status = DATA_DOING_NOTHING;   // a righteous initialization.

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



void FetchServers(int nRoom) {

	fprintf(logFile, "Child thread: %d\n", GetThreadId(GetCurrentThread()));
	fflush(logFile);

	// You can actually make this true, at this place, if you click like a rabid monkey on every room in the game
	// It is supposed to be locked....
	if(data_status == DATA_READY) { 
		fprintf(logFile, "Yup, I'm actually bailing out of this thread.\n");
		ExitThread(0);
		return;
	}

	data_status = DATA_FETCHING;

	client = new NWNMSClient(logFile, g_pAppManager);

//	client->threadId = GetThreadId(GetCurrentThread()); 

	client->GetServersInRoom(nRoom);

	data_status = DATA_READY;

	fprintf(logFile, "::DATA READY\n");
	fflush(logFile);

	
	threadlock_JoinGroup = 0; // release the lock, the thread is done.
}

void Poll() {

	if(data_status == DATA_READY) {
		client->AddServers();
		delete client;
		data_status = DATA_OLD;
		fprintf(logFile, "Done.\n");
		fflush(logFile);
	}
	/* 
	If you uncomment this, there will be thousands of lines in your log :)  
	if(data_status == DATA_FETCHING) {
		fprintf(logFile, "Main loop() visited the Poll and data is still fetching.  Nothing returned.\n");
		fflush(logFile);
	}

	*/

}


int (__fastcall *CGameSpyClient__JoinGroupRoom)(void *pGameSpy, int edx, int nRoom);
int __fastcall CGameSpyClient__JoinGroupRoom_Hook(void *pGameSpy, int edx, int nRoom)
{		
	SIZE_T stack_size = 0;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR SD;
	LPTHREAD_START_ROUTINE  start_address = (LPTHREAD_START_ROUTINE) FetchServers;
	LPDWORD threadID;	
	HANDLE thandle;

	join_attempted = 1;


	// don't make another thread until the last one is done.
	while(threadlock_JoinGroup) {   // blocking main thread
		continue;
	}
	threadlock_JoinGroup = 1;	

	InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&SD, TRUE, NULL, FALSE);
    sa.nLength = sizeof (SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = &SD;
    sa.bInheritHandle = TRUE;


	fprintf(logFile, "Main Thread ID=%d\n", GetThreadId(GetCurrentThread()));
	fflush(logFile);

	thandle = CreateThread(&sa, 0, start_address, (void *)nRoom, CREATE_SUSPENDED, NULL);
	ResumeThread(thandle);



	//return 	CGameSpyClient__JoinGroupRoom(pGameSpy, edx, nRoom);
	return true;
}

void (__fastcall *CConnectionLib__UpdateGameSpyClient)();
void __fastcall CConnectionLib__UpdateGameSpyClient_Hook(CConnectionLib *pConnectionLib)
{

	if(join_attempted && !threadlock_JoinGroup) {
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