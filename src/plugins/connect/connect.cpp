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
#include <nwnapi\custom\nwn_globals.h>
#include <nwnapi\custom\nwn_globals.cpp>

FILE *logFile;
char logFileName[] = "logs/nwncx_connect.txt";

void InitPlugin();

//////////////////////////////////////////////////////////////////////////
PLUGINLINK *pluginLink = 0;
PLUGININFO pluginInfo={
	sizeof(PLUGININFO),
	"NWNCX Connect",
	PLUGIN_MAKE_VERSION(1,0,0,0),
	"",
	"virusman",
	"virusman@virusman.ru",
	"© 2012 virusman",
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

//////////////////////////////////////////////////////////////////////////
int (__fastcall *CNWCMessage__HandleServerToPlayerMessage)(CNWMessage *pMessage, int edx, unsigned char *pData, int nLength);

void CNWCMessage__HandleServerToPlayerHakList(CNWMessage *pMessage)
{
	CExoString hak_prefix("HAK:");
	CExoString tlk_prefix("TLK:");

	int nTotal = pMessage->ReadINT(32);
	fprintf(logFile, "Total: %d\n", nTotal);

	//Preload HAKs
	for(int i = 0; i < nTotal; i++)
	{
		CExoString hak = pMessage->ReadCExoString(32);
		fprintf(logFile, "Hak: %s\n", hak.Text);
		fflush(logFile);
		if(g_pExoResMan->AddEncapsulatedResourceFile(hak_prefix+hak))
			fprintf(logFile, "Loaded hak: %s\n", hak.Text);
	}

	//Preload TLK
	CExoString tlk = pMessage->ReadCExoString(32);
	fprintf(logFile, "Tlk: %s\n", tlk.Text);
	fflush(logFile);
	if(g_pTlkTable->OpenFileAlternate(tlk_prefix+tlk))
		fprintf(logFile, "Loaded tlk: %s\n", tlk.Text);

	//Refresh 2da cache
	g_pRules->ReloadAll();
}

int __fastcall CNWCMessage__HandleServerToPlayerMessage_Hook(CNWMessage *pMessage, int edx, unsigned char *pData, int nLength)
{
	int nType = pData[1];
	int nSubtype = pData[2];
	pMessage->SetReadMessage(pData+3, nLength-3, -1, 1);
	fprintf(logFile, "Message: type %x, subtype %x\n", nType, nSubtype);
	fflush(logFile);
	if(nType == 100)
	{
		CNWCMessage__HandleServerToPlayerHakList(pMessage);
	}
	return CNWCMessage__HandleServerToPlayerMessage(pMessage, edx, pData, nLength);
}

void HookFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	*(DWORD*)&CNWCMessage__HandleServerToPlayerMessage = 0x00452420;
	int success = DetourAttach(&(PVOID&)CNWCMessage__HandleServerToPlayerMessage, CNWCMessage__HandleServerToPlayerMessage_Hook)==0;

	fprintf(logFile, "Hooked: %d\n", success);
	DetourTransactionCommit();
}

void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWCX Connect plugin 1.0 (PRE-RELEASE)\n");
	fprintf(logFile, "(c) 2012 by virusman\n");
	fflush(logFile);
	if(pluginLink){
		/*hAppendMsg = CreateHookableEvent("NWClient/GUI/AppendToMsgBuffer");
		hClientPostInit = CreateHookableEvent("NWClient/ExoApp/Initialized");
		CreateServiceFunction("NWClient/GUI/AppendToMsgBuffer", ShowLogMessage);*/
	}
	HookFunctions();

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		//MessageBox(NULL, GetCommandLine(), "NWNCX", MB_OK);
		//MessageBox(NULL, GetCurrentDirectory(), "NWNCX", MB_OK);
		//InitPlugin();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		//delete plugin;
	}
	return TRUE;
}