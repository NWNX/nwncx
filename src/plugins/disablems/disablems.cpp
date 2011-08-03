#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "detours.h"

char *thisBuf;

FILE *logFile;
char logFileName[] = "logs/nwncx_dmisablems.txt";

void EnableWrite (unsigned long location)
{
	unsigned char *page;
	DWORD oldAlloc;
	page = (unsigned char *) location;
	VirtualProtect(page, 0xFF, PAGE_EXECUTE_READWRITE, &oldAlloc);
}

void PatchImage()
{
	char *pPatch = (char *) 0x004D4AF7;
	EnableWrite((DWORD) pPatch);
	fprintf(logFile, "Patching: was %x\n", *(unsigned int *)pPatch);
	pPatch[0] = 0x84;
	pPatch[1] = 0xF2;
	pPatch[2] = 0x06;
	fprintf(logFile, "Patching: now %x\n", *(unsigned int *)pPatch);
	fflush(logFile);
}


void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWN Client Extender 1.0 - Disable Master Server\n");
	fprintf(logFile, "(c) 2011 by virusman\n");
	fflush(logFile);
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