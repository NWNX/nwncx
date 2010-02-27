#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		//MessageBox(NULL, GetCommandLine(), "NWNCX", MB_OK);
		//MessageBox(NULL, GetCurrentDirectory(), "NWNCX", MB_OK);
		LoadLibrary("nwnx-module.dll");
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		//delete plugin;
	}
	return TRUE;
}
