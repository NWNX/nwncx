#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <pluginapi.h>
#include "detours.h"

typedef struct  
{
	char		*Text;
	unsigned int Length;
} CExoString;

typedef struct  
{
	char	Text[16];
} CResRef;

FILE *logFile;
char logFileName[] = "logs/nwncx_test.txt";

void InitPlugin();

//////////////////////////////////////////////////////////////////////////
PLUGINLINK *pluginLink = 0;
PLUGININFO pluginInfo={
	sizeof(PLUGININFO),
	"NWNCX Test",
	PLUGIN_MAKE_VERSION(0,0,0,2),
	"The long description of your plugin, to go in the plugin options dialog",
	"virusman",
	"virusman@virusman.ru",
	"© 2010 virusman",
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
HANDLE hAppendMsg;
HANDLE hClientPostInit;
void *g_pGUIInGame = 0;

void (__fastcall *CGuiInGame__AppendToMsgBuffer)(void *pGUIInGame, int edx, CExoString str, unsigned long type, unsigned long a3, int a4, CResRef sResRef);
int (__fastcall *CClientExoAppInternal__Initialize)(void *pExoAppInternal, int edx);
void (__fastcall *CClientExoApp__SetCaptureEvents)(void *pExoApp, int edx, CExoString str);
void (__fastcall *CClientExoApp__SetCaptureEventPlayBack)(void *pExoApp, int edx, CExoString str);

void __fastcall CGuiInGame__AppendToMsgBuffer_Hook(void *pGUIInGame, int edx, CExoString str, unsigned long type, unsigned long a3, int a4, CResRef sResRef)
{
	if(!g_pGUIInGame) g_pGUIInGame = pGUIInGame;
	fprintf(logFile, "CGuiInGame__AppendToMsgBuffer\n");
	fprintf(logFile, "  CExoString: %s\n", str.Text);
	fprintf(logFile, "  a2: %x\n", type);
	fprintf(logFile, "  a3: %x\n", a3);
	fprintf(logFile, "  a4: %x\n", a4);
	fprintf(logFile, "  CResRef: %s\n", sResRef.Text);
	fflush(logFile);
	if(type == 0x80){
		char *txt = new char[str.Length];
		strncpy(txt, str.Text+6, strlen(str.Text)-10);
		fprintf(logFile, "--Text: %s\n", txt);
		if(strcmp(txt, "Axistown version 1.9.81d") == 0){
			strcpy(str.Text, "NWNCX is here!");
		}
		delete txt;
	}
	NotifyEventHooks(hAppendMsg, type, (LPARAM) &str);
	CGuiInGame__AppendToMsgBuffer(pGUIInGame, edx, str, type, a3, a4, sResRef);
}

//						  mode			text
int ShowLogMessage(WPARAM wParam,LPARAM lParam)
{
	CExoString str;
	CResRef sResRef;
	if(!g_pGUIInGame) return 0;

	str.Text = (char *) lParam;
	str.Length = strlen(str.Text)+1;
	sResRef.Text[0] = 0;

	CGuiInGame__AppendToMsgBuffer(g_pGUIInGame, 0, str, wParam, 0, 0, sResRef);
	return 1;
}

void *GetClientExoApp()
{
	return *(*(void ***)0x0092DC50);
}

void CaptureEvents()
{
	CExoString str;
	str.Text = "capture.dat";
	str.Length = strlen(str.Text)+1;
	CClientExoApp__SetCaptureEvents(GetClientExoApp(), 0, str);
}

void PlaybackEvents()
{
	CExoString str;
	str.Text = "capture.dat";
	str.Length = strlen(str.Text)+1;
	CClientExoApp__SetCaptureEventPlayBack(GetClientExoApp(), 0, str);
}

int __fastcall CClientExoAppInternal__InitializeHook(void *pExoAppInternal, int edx)
{
	int res = CClientExoAppInternal__Initialize(pExoAppInternal, edx);
	NotifyEventHooks(hClientPostInit, 0, 0);
	CaptureEvents();
	return res;
}

void HookFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	*(DWORD*)&CGuiInGame__AppendToMsgBuffer = 0x00493BD0;
	int success = DetourAttach(&(PVOID&)CGuiInGame__AppendToMsgBuffer, CGuiInGame__AppendToMsgBuffer_Hook)==0;
	*(DWORD*)&CClientExoAppInternal__Initialize = 0x00409D50;
	DetourAttach(&(PVOID&)CClientExoAppInternal__Initialize, CClientExoAppInternal__InitializeHook);

	*(DWORD*)&CClientExoApp__SetCaptureEvents = 0x00407FA0;
	*(DWORD*)&CClientExoApp__SetCaptureEventPlayBack = 0x00408000;

	fprintf(logFile, "Hooked: %d\n", success);
	DetourTransactionCommit();
}

void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWN Client Extender 0.1 - Test plugin\n");
	fprintf(logFile, "(c) 2010 by virusman\n");
	fflush(logFile);
	if(pluginLink){
		hAppendMsg = CreateHookableEvent("NWClient/GUI/AppendToMsgBuffer");
		hClientPostInit = CreateHookableEvent("NWClient/ExoApp/Initialized");
		CreateServiceFunction("NWClient/GUI/AppendToMsgBuffer", ShowLogMessage);
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