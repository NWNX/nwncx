#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "detours.h"
#include "NWNGUI.h"
#include "CExoString.h"
#include <gl/gl.h>

void (*OriginalTileRadialMenu)();
//int (*OriginalAddTimingEvent)(char *a1, char *a2, int a3);
void (*OriginalRenderScene)();

HWND *g_hWnd = (HWND *) 0x0092DC28;
HWND *g_hRenderWnd = (HWND *) 0x0092DC2C;

static LONG (WINAPI * TrueChangeDisplaySettings)(LPDEVMODEA lpDevMode, DWORD dwFlags) = ChangeDisplaySettings;

char *buttonText = "NWNCX Custom Button";
char *thisBuf;

FILE *logFile;
char logFileName[] = "logs/nwncx_gui.txt";

const char *icon = "NWNX_ICON";

int __stdcall RadialButtonCallback(int a1)
{
	return 1;
}

void __declspec(naked) TileRadialMenuHook()
{
	__asm{
		push 0
		push RadialButtonCallback
		push edi
		push buttonText //0x571
		push icon
		push 0
		mov  ecx, esi
		mov eax, 0x005F1430 // 0x005F1380 //
		call eax
		jmp OriginalTileRadialMenu
	}
}

void ViewOrtho(int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, x , y , 0, -1, 1 );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void ViewPerspective(void)
{
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void DrawOverlay(void)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glColor3f(1.0f,1.0f,1.0f);
	ViewOrtho(500, 500);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, 100);
	glVertex2f(100, 100);
	glVertex2f(100, 0);
	glEnd();
	ViewPerspective();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void OpenGLHook()
{
	__asm{
		mov thisBuf, ecx
	}
	fprintf(logFile, "* OpenGL hook call\n");
	glDisable(GL_CULL_FACE); 
	DrawOverlay();


	/*__asm{
		push 0
			push RadialButtonCallback
			push edi
			push buttonText //0x571
			push icon
			push 0
			mov  ecx, esi
			mov eax, 0x005F1430 // 0x005F1380 //
			call eax
			jmp OriginalAddTimingEvent
	}*/
	__asm{
		mov ecx, thisBuf
	}
	OriginalRenderScene();
}

LONG WINAPI ChangeDisplaySettingsHook(LPDEVMODEA lpDevMode, DWORD dwFlags)
{
	DWORD pCaller;
	__asm {mov eax, [ebp+0x4]}
	__asm {mov pCaller, eax}
	fprintf(logFile, "* ChangeDisplaySettings: %x, caller: %x\n", dwFlags, pCaller);
	fprintf(logFile, "hWnd: %x\n", *g_hWnd);
	fprintf(logFile, "hRenderWnd: %x\n", *g_hRenderWnd);
	fprintf(logFile, "GetForegroundWindow: %x\n", GetForegroundWindow());

	HWND hWnd = *g_hRenderWnd;
	char sWindowCaption[64];
	sWindowCaption[0] = 0;
	GetWindowText(hWnd, sWindowCaption, 64);

	fprintf(logFile, "Window caption: %s\n", sWindowCaption);
	
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	
	fprintf(logFile, "hMonitor: %x\n", hMonitor);
	MONITORINFOEX stMonitorInfo;
	memset(&stMonitorInfo, 0, sizeof(MONITORINFOEX));
	stMonitorInfo.cbSize = sizeof(MONITORINFOEX);
	if(GetMonitorInfo(hMonitor, &stMonitorInfo))
	{
		fprintf(logFile, "Got monitor info\n");
		fprintf(logFile, "Device: %s\n", stMonitorInfo.szDevice);
	}
	else
	{
		fprintf(logFile, "MONITORINFO is NULL\n");
	}
	fflush(logFile);
	return ChangeDisplaySettingsEx(NULL, lpDevMode, NULL, dwFlags, NULL);
}

void __fastcall RestoreAuroraDisplayMode(char *pAurInternal)
{


}

void HookFunctions()
{
	LONG error;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DWORD pTileRadialMenu = 0x00529740;
	*(DWORD*)&OriginalTileRadialMenu = pTileRadialMenu;
	DetourAttach(&(PVOID&)OriginalTileRadialMenu, TileRadialMenuHook);

	/*DWORD pRenderScene = 0x00799100;
	*(DWORD*)&OriginalRenderScene = pRenderScene;
	DetourAttach(&(PVOID&)OriginalRenderScene, OpenGLHook);*/
	
	DetourAttach(&(PVOID&)TrueChangeDisplaySettings, ChangeDisplaySettingsHook);

	error = DetourTransactionCommit();
	if(error == NO_ERROR)
		fprintf(logFile, "Hooked successfully\n");
	else
		fprintf(logFile, "Hooking error: %d\n", error);
}



void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWN Client Extender 0.1 - GUI plugin\n");
	fprintf(logFile, "(c) 2009 by virusman\n");
	init();
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
