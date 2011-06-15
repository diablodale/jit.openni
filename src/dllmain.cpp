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

#ifdef _DEBUG
#include <stdlib.h>		// needed for _MAX_PATH reference below
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


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
			{
				TCHAR buff[_MAX_PATH];
				OutputDebugString(L"DLL_PROCESS_ATTACH: ");
				GetModuleFileName(hModule, buff, _MAX_PATH);
				OutputDebugString(buff);
				OutputDebugString(L"\n");
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
