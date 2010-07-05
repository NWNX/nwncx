#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "detours.h"
#include "CMyPanel.h"
#include <pluginapi.h>
#include "CExoString.h"
#include <gl/gl.h>

void (*OriginalTileRadialMenu)();
//int (*OriginalAddTimingEvent)(char *a1, char *a2, int a3);
void (*OriginalRenderScene)();

HWND *g_hWnd = (HWND *) 0x0092DC28;
HWND *g_hRenderWnd = (HWND *) 0x0092DC2C;
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

static LONG (WINAPI * TrueChangeDisplaySettings)(LPDEVMODEA lpDevMode, DWORD dwFlags) = ChangeDisplaySettings;
int (__fastcall *CGuiMan__HandleKeyPress)(void *pGuiMan, int edx, int key);
int (__fastcall *CGuiMan__HandleKeyEvent)(void *pGuiMan, int edx, unsigned long key1, unsigned long key2);
void (__fastcall *CGuiMan__HandleLMouseUp)(void *pGuiMan, int edx);
void (__fastcall *CGuiMan__GetMousePos)(void *pGuiMan, int edx, int *x, int *y);

char *buttonText = "NWNCX Custom Button";
char *thisBuf;

FILE *logFile;
char logFileName[] = "logs/nwncx_gui.txt";

const char *icon = "NWNX_ICON";

int __stdcall RadialButtonCallback(int a1)
{
	CMyPanel *gui = new CMyPanel();
	gui->Activate();
	//delete gui;
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

int __fastcall CGuiMan__HandleKeyPress_Hook(void *pGuiMan, int edx, int key)
{
	fprintf(logFile, "KeyPress: %x\n", key);
	fflush(logFile);
	CallService("NWClient/GUI/AppendToMsgBuffer", 0x80, (LPARAM) "KeyPress!");
	return CGuiMan__HandleKeyPress(pGuiMan, edx, key);
}

int __fastcall CGuiMan__HandleKeyEventHook(void *pGuiMan, int edx, unsigned long key1, unsigned long key2)
{
	fprintf(logFile, "KeyEvent: %x, %x\n", key1, key2);
	fflush(logFile);
	CallService("NWClient/GUI/AppendToMsgBuffer", 0x80, (LPARAM) "KeyEvent!");
	return CGuiMan__HandleKeyEvent(pGuiMan, edx, key1, key2);
}

void __fastcall CGuiMan__HandleLMouseUpHook(void *pGuiMan, int edx)
{
	int x, y;
	char *msg = new char[64];
	CGuiMan__GetMousePos(pGuiMan, 0, &x, &y);
	sprintf(msg, "LMouseUp: %d, %d", x, y);
	fprintf(logFile, "%s\n", msg);
	fflush(logFile);
	CallService("NWClient/GUI/AppendToMsgBuffer", 0x80, (LPARAM) msg);
	free(msg);
	CGuiMan__HandleLMouseUp(pGuiMan, edx);
}

void HookFunctions()
{
	LONG error;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DWORD pTileRadialMenu = 0x00529740;
	*(DWORD*)&OriginalTileRadialMenu = pTileRadialMenu;
	error = DetourAttach(&(PVOID&)OriginalTileRadialMenu, TileRadialMenuHook);
	fprintf(logFile, "Tile radial hook: %d\n", error);

	/*DWORD pRenderScene = 0x00799100;
	*(DWORD*)&OriginalRenderScene = pRenderScene;
	DetourAttach(&(PVOID&)OriginalRenderScene, OpenGLHook);*/
	
	//DetourAttach(&(PVOID&)TrueChangeDisplaySettings, ChangeDisplaySettingsHook);
	/*
	*(DWORD*)&CGuiMan__HandleKeyPress = 0x005DF3A0;
	error = DetourAttach(&(PVOID&)CGuiMan__HandleKeyPress, CGuiMan__HandleKeyPress_Hook);
	fprintf(logFile, "HandleKeyPress hook: %d\n", error);

	*(DWORD*)&CGuiMan__HandleKeyEvent = 0x005DF3C0;
	error = DetourAttach(&(PVOID&)CGuiMan__HandleKeyEvent, CGuiMan__HandleKeyEventHook);
	fprintf(logFile, "HandleKeyEvent hook: %d\n", error);

	*(DWORD*)&CGuiMan__HandleLMouseUp = 0x005DF820;
	error = DetourAttach(&(PVOID&)CGuiMan__HandleLMouseUp, CGuiMan__HandleLMouseUpHook);
	fprintf(logFile, "HandleLMouseUp hook: %d\n", error);

	*(DWORD*)&CGuiMan__GetMousePos = 0x005DFC40;
*/
	error = DetourTransactionCommit();
	if(error == NO_ERROR)
		fprintf(logFile, "Hooked successfully\n");
	else
		fprintf(logFile, "Hooking error: %d\n", error);
}

int OnClientLoaded(WPARAM wParam, LPARAM lParam)
{
	fprintf(logFile, "Game loaded\n");
	fflush(logFile);
	return 1;
}

int OnPluginsLoaded(WPARAM wParam,LPARAM lParam)
{
	fprintf(logFile, "Plugins loaded\n");
	HookEvent("NWClient/ExoApp/Initialized", OnClientLoaded);
	return 1;
}

void InitPlugin()
{
	//DebugBreak();
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWN Client Extender 0.1 - GUI plugin\n");
	fprintf(logFile, "(c) 2009 by virusman\n");
	//init();
	HookFunctions();
	HookEvent("NWNX/Core/PluginsLoaded", OnPluginsLoaded);
	fflush(logFile);

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		//InitPlugin();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		//delete plugin;
	}
	return TRUE;
}
