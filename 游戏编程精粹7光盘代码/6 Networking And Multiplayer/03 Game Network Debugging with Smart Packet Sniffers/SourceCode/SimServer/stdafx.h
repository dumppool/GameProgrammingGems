// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <assert.h>
#include <stdio.h>
#include <tchar.h>

#include <WinSock2.h>
#endif

#ifdef __GNUC__
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <netinet/in.h> //needed for sockaddr_in
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#endif

#define TRACE printf

#include <list>
#include <algorithm>
#include "NetworkProtocol.h"