/****************************************************************************
	jit.openni
	Copyright (C) 2011 Dale Phurrough

	This file is part of jit.openni.

    jit.openni is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    jit.openni is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with jit.openni.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
	 jit.openni - a Max Jitter external for OpenNI middleware
	 Shell of it was imspired by the jit.simple example from the MaxSDK and
	 the MaxSDK documentation
*/

#ifdef WIN_VERSION
// dllmain.cpp : Defines the entry point for the DLL application.

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifdef _DEBUG

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

void SetStdOutToNewConsole()
{
	HANDLE consoleHandle;
	FILE *fp;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int fileDescriptor;
	SECURITY_ATTRIBUTES secAttr;

	// allocate a console for this app
    if (AllocConsole())
	{
		// redirect unbuffered STDOUT to the console
		  secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		  secAttr.bInheritHandle = true;
		  consoleHandle = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &secAttr, OPEN_EXISTING, 0, 0);
		  SetStdHandle(STD_OUTPUT_HANDLE, consoleHandle);
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
		fp = _fdopen( fileDescriptor, "w" );
		*stdout = *fp;
		setvbuf( stdout, NULL, _IONBF, 0 );

		// redirect unbuffered STDERR to the console
		consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
		fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
		fp = _fdopen( fileDescriptor, "w" );
		*stderr = *fp;
		setvbuf( stderr, NULL, _IONBF, 0 );

		// then we need to open the output stream associated with the console
		std::ofstream con_out("CONOUT$");
		std::ofstream con_err("CONOUT$");
		std::ofstream con_log("CONOUT$");
 
		// once we have the console buffer, we can set it as the std::cout stream buffer
		std::cout.rdbuf(con_out.rdbuf());
		std::cerr.rdbuf(con_err.rdbuf());
		std::clog.rdbuf(con_log.rdbuf());

		std::ios::sync_with_stdio();

		// give the console window a nicer title
		SetConsoleTitle(L"jit.openni stdout, stderr console output");

		// give the console window a bigger buffer size
		if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
		{
			COORD bufferSize;
			bufferSize.X = csbi.dwSize.X;
			bufferSize.Y = 9999;
			SetConsoleScreenBufferSize(consoleHandle, bufferSize);
		}
		printf("Simple printf() routed here\n");
		fprintf(stdout,"stdout is now routed here\n");
		fprintf(stderr,"stderr is now routed here\n");
		std::cout << "cout is now routed here\n";
		std::cerr << "cerr is now routed here\n";
		std::clog << "clog is now routed here\n";
		std::wcout << "wcout is now routed here\n";
		std::wcerr << "wcerr is now routed here\n";
		std::wclog << "wclog is now routed here\n";
	}
}
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
			// Initialize once for each new process.  Return FALSE to fail DLL load.
			// Since we do nothing in our DLL_THREAD_ATTACH and DLL_THREAD_DETACH calls below, 
			// we don't need to actually receive those calls. The below call tells the OS to 
			// optimize those out.  
			DisableThreadLibraryCalls(hModule);
#ifdef _DEBUG
			SetStdOutToNewConsole();
			{
				TCHAR mname[1024];
				TCHAR dtemp[1024 + 80];
				GetModuleFileName(hModule, mname, 1024);
				swprintf_s(dtemp, 1024 + 80, L"DllMain() DLL_PROCESS_ATTACH: %s", mname);
				OutputDebugString(dtemp);
			}
#endif
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif // #ifdef WIN_VERSION
