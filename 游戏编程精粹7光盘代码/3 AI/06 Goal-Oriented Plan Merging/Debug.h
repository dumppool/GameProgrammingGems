//Copyright (c) 2006 Digipen (USA) Corp.

#ifndef DEBUG_H
#define DEBUG_H

#ifdef _MSC_VER
#pragma once
#endif

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>

#include <windows.h>

//Prints out a debug statement. Taken from http://www.unixwiz.net/techtips/outputdebugstring.html
// OutputDebugString needs a debugger (Visual Studio, or a debug-print catcher like DebugView)
// in order to see the output log. It has no effect for release builds.
void inline __cdecl dprintf(const char *format, ...)
{
#ifdef _DEBUG
	char	buf[4096], *p = buf;
	va_list	args;

	va_start(args, format);

#if _MSC_VER >= 1400
	p += _vsnprintf_s(p, 4096, sizeof buf - 1, format, args);
#else
	p += _vsnprintf(p, sizeof buf - 1, format, args);
#endif

	va_end(args);

	while ( p > buf  &&  isspace(p[-1]) )
			*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugString(buf);
#endif
}

#endif //DEBUG_H
