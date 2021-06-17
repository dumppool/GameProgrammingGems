/* Copyright (C) Scott Patterson, 2002. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Patterson, 2002"
 */

// VisualSysD3D8Dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "VisualSysD3D8Dll.h"
#include "../VisualSysD3D8Lib/VisualSysD3D8Impl_t.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

VisualSys_t *CreateDllVisualSys()
{
	return new VisualSysD3D8Impl_t;
}

void DeleteDllVisualSys( VisualSys_t *pVisualSys )
{
	delete pVisualSys;
}
