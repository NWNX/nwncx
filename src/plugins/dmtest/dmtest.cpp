#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "detours.h"
#include "CExoString.h"

char *thisBuf;

FILE *logFile;
char logFileName[] = "logs/nwncx_dmtest.txt";

void (__fastcall *SetModuleToQuickLoad)(void *pClientExoApp, int edx, CExoString sModuleName);
void (__fastcall *SetDungeonMasterEXERunning)(void *pClientExoApp, int edx, int nValue);

void __fastcall SetModuleToQuickLoad_hook(void *pClientExoApp, int edx, CExoString sModuleName)
{
	if(sModuleName.Text){
		fprintf(logFile, "SetModuleToQuickLoad_hook: %s\n", sModuleName.Text);
		fflush(logFile);
		SetDungeonMasterEXERunning(pClientExoApp, 0, 1);
	}
	SetModuleToQuickLoad(pClientExoApp, edx, sModuleName);
}

void EnableWrite (unsigned long location)
{
	unsigned char *page;
	DWORD oldAlloc;
	page = (unsigned char *) location;
	VirtualProtect(page, 0xFF, PAGE_EXECUTE_READWRITE, &oldAlloc);
}

void HookFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	/*DWORD pTileRadialMenu = 0x00529740;
	*(DWORD*)&OriginalTileRadialMenu = pTileRadialMenu;
	DetourAttach(&(PVOID&)OriginalTileRadialMenu, TileRadialMenuHook);

	DWORD pRenderScene = 0x00799100;
	*(DWORD*)&OriginalRenderScene = pRenderScene;
	DetourAttach(&(PVOID&)OriginalRenderScene, OpenGLHook);

	0x00405110;*/
	*(DWORD*)&SetModuleToQuickLoad = 0x00405110;
	*(DWORD*)&SetDungeonMasterEXERunning = 0x00408070;
	int success = DetourAttach(&(PVOID&)SetModuleToQuickLoad, SetModuleToQuickLoad_hook);
	fprintf(logFile, "Attach attempt: %d\n", success);

	DetourTransactionCommit();

}

void PatchImage()
{
	char *pLoadPatch1 = (char *) 0x00402FA8;
	EnableWrite((DWORD) pLoadPatch1);
	fprintf(logFile, "Patching: was %x\n", pLoadPatch1[0]);
	pLoadPatch1[0] = 0xEB;
	fprintf(logFile, "Patching: now %x\n", pLoadPatch1[0]);
}


void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWN Client Extender 0.1 - DM Test plugin\n");
	fprintf(logFile, "(c) 2009 by virusman\n");
	fflush(logFile);
	HookFunctions();
	PatchImage();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		InitPlugin();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		//delete plugin;
	}
	return TRUE;
}