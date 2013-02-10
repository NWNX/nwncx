#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "detours.h"
#include "CExoString.h"
#include "C2DA.h"
#include "Gob.h"

char *thisBuf;

FILE *logFile;
char logFileName[] = "logs/nwncx_tweaks.txt";

void (*CNWCItem__LoadVisualEffect_origmid)();
void (*CNWCVisualEffectOnObject__LoadBeam_origmid)();
void (*CNWCVisualEffectOnObject__ApplyTextureReplace_origmid)();
void (*CNWSpellArray__Load_origmid)();
void (*CNWCCreatureAppearance__CreateHead_origmid)();

void EnableWrite (unsigned long location)
{
	unsigned char *page;
	DWORD oldAlloc;
	page = (unsigned char *) location;
	VirtualProtect(page, 0xFF, PAGE_EXECUTE_READWRITE, &oldAlloc);
}

void PatchImage()
{
	char *pPatch = (char *) 0x00589A15;
	EnableWrite((DWORD) pPatch);
	fprintf(logFile, "Patching: was %x\n", *(unsigned int *)pPatch);
	pPatch[0] = 0x07;
	fprintf(logFile, "Patching: now %x\n", *(unsigned int *)pPatch);
	fflush(logFile);
}

// ################################## WEAPON VFX ##################################

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

// ################################## BEAM VFX ##################################

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

// ################################## TEXTURE REPLACE ##################################

int __stdcall CNWCVisualEffectOnObject__ApplyTextureReplace_Hook(int nType, CExoString &sFxName)
{
	nType += 10;
	fprintf(logFile, "CNWCVisualEffectOnObject__ApplyTextureReplace_Hook: %d\n", nType);
	if(nType == 10 || nType == 20 || nType == 30 || nType == 40 || nType == 50 || nType == 60)
		return false;

	CExoString sModel;
	C2DA tVisualFX("vfx_texreplace", 0);
	tVisualFX.Load2DArray();
	tVisualFX.GetCExoStringEntry(nType, "Texture", &sModel);
	if(sModel.Length > 0){
		sFxName = sModel;
		
		fprintf(logFile, "sFxName: %s\n", sFxName.Text);
		fflush(logFile);
		return true;
	}
	else
		return false;
}

unsigned int CNWCVisualEffectOnObject__ApplyTextureReplace_eax;

void __declspec( naked )  CNWCVisualEffectOnObject__ApplyTextureReplace_hookmid()
{
	__asm{
		mov CNWCVisualEffectOnObject__ApplyTextureReplace_eax, eax
		lea ecx, [esp+4h]
		push ecx
		push eax
		call CNWCVisualEffectOnObject__ApplyTextureReplace_Hook
		test eax, eax
		jz orig
		mov ecx, 005701B6h
		jmp ecx

	orig:
		mov eax, CNWCVisualEffectOnObject__ApplyTextureReplace_eax
		jmp CNWCVisualEffectOnObject__ApplyTextureReplace_origmid
	}
}

// ################################## PROJECTILE TYPES ##################################

void __stdcall CNWSpellArray__Load_Hook(CExoString &sProjTypeName, int &nProjTypeValue)
{
	if(sProjTypeName == "burstup")
		nProjTypeValue = 4;
	else if(sProjTypeName == "linkedburstup")
		nProjTypeValue = 10;
	else if(sProjTypeName == "tripleballistic")
		nProjTypeValue = 11;
	else if(sProjTypeName == "tripleballistic2")
		nProjTypeValue = 12;
	else if(sProjTypeName == "doubleballistic")
		nProjTypeValue = 13;
	fprintf(logFile, "CNWSpellArray__Load_Hook: %s = %d\n", sProjTypeName.Text, nProjTypeValue);
	fflush(logFile);
}

unsigned int CNWSpellArray__Load_eax;

void __declspec( naked )  CNWSpellArray__Load_hookmid()
{
	__asm{
		mov CNWSpellArray__Load_eax, eax
		lea ecx, [esp+10h]   //projType int pointer
		push ecx
		lea ecx, [esp+18h]  //string
		push ecx
		call CNWSpellArray__Load_Hook
		mov eax, CNWSpellArray__Load_eax
		jmp CNWSpellArray__Load_origmid
	}
}

// ################################## HEADS ##################################

int __stdcall CNWCCreatureAppearance__CreateHead_Hook(Gob *pHelmModel)
{
	fprintf(logFile, "Head update\n");
	if(pHelmModel){
		fprintf(logFile, "Helm model name: %s, %s\n", pHelmModel->GetName(), pHelmModel->GetModelName());
		void *pPart = pHelmModel->GetPart("nwncx_openface");
		fprintf(logFile, "Part: %x\n", pPart);
		if(pPart)
			return true;
	}
	fflush(logFile);
	return false;
}

void __declspec( naked ) CNWCCreatureAppearance__CreateHead_hookmid()
{
	__asm{
		push ebp
		call CNWCCreatureAppearance__CreateHead_Hook
		test eax, eax
		jz orig
		mov ecx, 00540FF2h
		jmp ecx

		orig:
		jmp CNWCCreatureAppearance__CreateHead_origmid
	}
}

// ################################## HOOKS ##################################

void HookFunctions()
{
	LONG error;
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	*(DWORD*)&CNWCItem__LoadVisualEffect_origmid = 0x004EA0EA;
	*(DWORD*)&CNWCVisualEffectOnObject__LoadBeam_origmid = 0x00574112;
	*(DWORD*)&CNWCVisualEffectOnObject__ApplyTextureReplace_origmid = 0x00570126;
	*(DWORD*)&CNWSpellArray__Load_origmid = 0x004F9A0B;
	*(DWORD*)&CNWCCreatureAppearance__CreateHead_origmid = 0x00540FE5;
	
	error = DetourAttach(&(PVOID&)CNWCItem__LoadVisualEffect_origmid, CNWCItem__LoadVisualEffect_hookmid);
	fprintf(logFile, "CNWCItem::LoadVisualEffect hook: %d\n", error);

	error = DetourAttach(&(PVOID&)CNWCVisualEffectOnObject__LoadBeam_origmid, CNWCVisualEffectOnObject__LoadBeam_hookmid);
	fprintf(logFile, "CNWCVisualEffectOnObject::LoadBeam hook: %d\n", error);

	error = DetourAttach(&(PVOID&)CNWCVisualEffectOnObject__ApplyTextureReplace_origmid, CNWCVisualEffectOnObject__ApplyTextureReplace_hookmid);
	fprintf(logFile, "CNWCVisualEffectOnObject::ApplyTextureReplace hook: %d\n", error);

	error = DetourAttach(&(PVOID&)CNWSpellArray__Load_origmid, CNWSpellArray__Load_hookmid);
	fprintf(logFile, "CNWSpellArray::Load hook: %d\n", error);

	error = DetourAttach(&(PVOID&)CNWCCreatureAppearance__CreateHead_origmid, CNWCCreatureAppearance__CreateHead_hookmid);
	fprintf(logFile, "CNWCCreatureAppearance::CreateHeadAppearance hook: %d\n", error);
	
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
	fprintf(logFile, "NWN Client Extender - Tweaks plugin v.0.2.7\n");
	fprintf(logFile, "(c) 2011-2013 by virusman\n");
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