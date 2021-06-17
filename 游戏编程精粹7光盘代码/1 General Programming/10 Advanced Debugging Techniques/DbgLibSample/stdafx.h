// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <cstdio>
#include <cstdlib>

#if defined(_WIN32)

#include <tchar.h>
#include <Windows.h>

#else	// _WIN32

// for simplicity we just define the Windows functions we use for ANSI/Unicode support to their Unix ANSI equivalents
#define _tprintf	printf
#define _gettchar	getchar
#define _T(x)		x
#define _tmain		main
typedef	char		TCHAR;

#endif	// _WIN32

#include "../DbgLib/DbgLib.h"
