#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "detours.h"
#include "CExoString.h"
#include "C2DA.h"

char *thisBuf;

FILE *logFile;
char logFileName[] = "logs/nwncx_tweaks.txt";

void (*CNWCItem__LoadVisualEffect_origmid)();
void (*CNWCVisualEffectOnObject__LoadBeam_origmid)();

void EnableWrite (unsigned long location)
{
	unsigned char *page;
	DWORD oldAlloc;
	page = (unsigned char *) location;
	VirtualProtect(page, 0xFF, PAGE_EXECUTE_READWRITE, &oldAlloc);
}

void PatchImage()
{
	/*char *pPatch = (char *) 0x004D4AF7;
	EnableWrite((DWORD) pPatch);
	fprintf(logFile, "Patching: was %x\n", *(unsigned int *)pPatch);
	pPatch[0] = 0x84;
	pPatch[1] = 0xF2;
	pPatch[2] = 0x06;
	fprintf(logFile, "Patching: now %x\n", *(unsigned int *)pPatch);
	fflush(logFile);*/
}

int __stdcall CNWCItem__LoadVisualEffect_Hook(int nType, CExoString &sFxName)
{
	fprintf(logFile, "CNWCItem__LoadVisualEffect_Hook: %d\n", nType);

	CExoString sFxprefix("_fx");
	CExoString sFxsuffix;
	C2DA tVisualFX("iprp_visualfx", 0);
	tVisualFX.Load2DArray();
	tVisualFX.GetCExoStringEntry(nType, "ModelSuffix", &sFxsuffix);
	if(sFxsuffix.Length > 0){
		sFxName = sFxName + sFxprefix + sFxsuffix;
		
		fprintf(logFile, "sFxName: %s\n", sFxName.Text);
		fflush(logFile);
		return true;
	}
	else
		return false;
}

unsigned int CNWCItem__LoadVisualEffect_eax;

void __declspec( naked ) CNWCItem__LoadVisualEffect_hookmid()
{
	__asm{
		mov CNWCItem__LoadVisualEffect_eax, eax
		lea ecx, [esp+10h]
		push ecx
		push eax
		call CNWCItem__LoadVisualEffect_Hook
		test eax, eax
		jz orig
		mov ecx, 004EA381h
		jmp ecx

	orig:
		mov eax, CNWCItem__LoadVisualEffect_eax
		jmp CNWCItem__LoadVisualEffect_origmid
	}
}

int __stdcall CNWCVisualEffectOnObject__LoadBeam_Hook(int nType, CExoString &sFxName)
{
	fprintf(logFile, "CNWCVisualEffectOnObject__LoadBeam_Hook: %d\n", nType);

	CExoString sModel;
	C2DA tVisualFX("vfx_beams", 0);
	tVisualFX.Load2DArray();
	tVisualFX.GetCExoStringEntry(nType, "Model", &sModel);
	if(sModel.Length > 0){
		sFxName = sModel;
		
		fprintf(logFile, "sFxName: %s\n", sFxName.Text);
		fflush(logFile);
		return true;
	}
	else
		return false;
}

unsigned int CNWCVisualEffectOnObject__LoadBeam_eax;

void __declspec( naked )  CNWCVisualEffectOnObject__LoadBeam_hookmid()
{
	__asm{
		mov CNWCVisualEffectOnObject__LoadBeam_eax, eax
		lea ecx, [esp+4h]
		push ecx
		push eax
		call CNWCVisualEffectOnObject__LoadBeam_Hook
		test eax, eax
		jz orig
		mov ecx, 00574188h
		jmp ecx

	orig:
		mov eax, CNWCVisualEffectOnObject__LoadBeam_eax
		jmp CNWCVisualEffectOnObject__LoadBeam_origmid
	}
}

void HookFunctions()
{
	LONG error;
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	*(DWORD*)&CNWCItem__LoadVisualEffect_origmid = 0x004EA0EA;
	*(DWORD*)&CNWCVisualEffectOnObject__LoadBeam_origmid = 0x00574112;
	//*(DWORD*)&CNWCItem__LoadVisualEffect_origmid = 0x004EA40B;
	
	error = DetourAttach(&(PVOID&)CNWCItem__LoadVisualEffect_origmid, CNWCItem__LoadVisualEffect_hookmid);
	fprintf(logFile, "CNWCItem::LoadVisualEffect hook: %d\n", error);

	error = DetourAttach(&(PVOID&)CNWCVisualEffectOnObject__LoadBeam_origmid, CNWCVisualEffectOnObject__LoadBeam_hookmid);
	fprintf(logFile, "CNWCVisualEffectOnObject::LoadBeam hook: %d\n", error);

	error = DetourTransactionCommit();
	if(error == NO_ERROR)
		fprintf(logFile, "Hooked successfully\n");
	else
		fprintf(logFile, "Hooking error: %d\n", error);
	fflush(logFile);
}


void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWN Client Extender - Tweaks plugin v.0.2.2\n");
	fprintf(logFile, "(c) 2011-2012 by virusman\n");
	fflush(logFile);
	PatchImage();
	HookFunctions();
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