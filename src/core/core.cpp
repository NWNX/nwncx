/***************************************************************************
	NWNCX Core - Loads the plugins
	Copyright (C) 2008 virusman (virusman@virusman.ru)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***************************************************************************/

#include "core.h"

CHashTable Libraries;
int debuglevel = 0;
FILE *logFile;
char logFileName[18] = "logs/nwncx.txt";


/***************************************************************************
Fake export function for detours
***************************************************************************/

extern "C" __declspec(dllexport) 
void dummy() 
{	
	return ;
}

void LoadLibraries()
{
	WIN32_FIND_DATA findData;
	memset(&findData, 0, sizeof(findData));
	char logDir[MAX_PATH];
	BOOL proceed = TRUE;
	char moduleName[MAX_PATH];

	strcpy (logDir, "logs");

	// create hash table with default size
	if (!Libraries.Create())
	{
		fprintf (logFile, "* Critical error: creation of hash table failed!");
		return;
	}

	// search for all plugin DLL's
	HANDLE hSearch = FindFirstFile("nwncx_*.dll", &findData);
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		fprintf(logFile, "* Error: No plugins found\n");
		return;
	}

	do
	{
		memset(moduleName, 0, MAX_PATH);
		char* pos = strchr(findData.cFileName, '_') + 1;
		char* end = strchr(pos, '.');
		strncpy (moduleName, pos, end - pos);

		fprintf(logFile, "* Loading %s...\n", findData.cFileName);
		// try to load the library
		HINSTANCE hDLL = LoadLibrary(findData.cFileName);
		if (hDLL == NULL)
		{
			LPVOID lpMsgBuf;
			DWORD dw = GetLastError(); 
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_MAX_WIDTH_MASK ,
				NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,	0, NULL);

			fprintf(logFile, "* An error occured while loading extension %s (%d: %s)\n", moduleName, dw, lpMsgBuf);
			LocalFree(lpMsgBuf);
		}
		else
		{
			// create an instance of module
			/*GetClassObject = (GETCLASSOBJECT)GetProcAddress(hDLL, "GetClassObject");
			if (GetClassObject != NULL)
			{
				CNWNXBase* pBase = GetClassObject();
				if (pBase)
				{
					if (!pBase->OnCreate(logDir))
						fprintf(logFile, "* An error occured during OnCreate of %s\n", moduleName);
					else
						fprintf(logFile, "* Plugin %s is loaded.\n", moduleName);
					Libraries.Insert(strupr(moduleName), pBase);
				}
				else
					fprintf(logFile, "* Could not create an instance of plugin %s", moduleName);
			}
			else
			{
				fprintf(logFile, "* Error: can not find creation function in %s\n", findData.cFileName);
			}*/
		}

		// find next module
		proceed = FindNextFile(hSearch, &findData);
	} while (proceed);

	FindClose(hSearch);
}

DWORD WINAPI NWNCXInit(LPVOID lpParam) 
{
	CIniFile iniFile ("nwncx.ini");
	debuglevel = iniFile.ReadInteger("NWNCX", "debuglevel", 0);
	logFile = fopen(logFileName, "w");
	fprintf(logFile, "NWN Client Extender 0.1\n");
	fprintf(logFile, "(c) 2008 by virusman\n");
	fprintf(logFile, "* Loading plugins...\n");
	LoadLibraries();
	fprintf(logFile, "* NWNCX activated.\n");
	fflush(logFile);
	return 0;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DWORD dwThreadId;
		HANDLE hThread; 
		hThread = CreateThread(NULL, 0, NWNCXInit, NULL, 0, &dwThreadId);                
		CloseHandle( hThread );
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//fprintf(logFile, "* NWNX2 shutting down...\n");
		Libraries.DeleteAll();
		////fprintf(logFile, "* NWNX2 shutdown successfull.\n");
		//fclose(logFile);
	}

	return TRUE;
}