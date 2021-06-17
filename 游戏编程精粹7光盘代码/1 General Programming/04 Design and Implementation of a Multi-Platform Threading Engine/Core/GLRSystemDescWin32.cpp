//===============================================================================
//	GLRSystemDescWin32.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRSystemDescWin32.h"

//===============================================================================
GLRSystemDescriptionWin32::GLRSystemDescriptionWin32()
{
	startup();
}
//===============================================================================
void GLRSystemDescriptionWin32::startup( void )
{
	memInfo.dwLength=sizeof(MEMORYSTATUS);
	::GlobalMemoryStatus(&memInfo); 
	::GetSystemInfo(&SystemInfo);
}
//===============================================================================
int GLRSystemDescriptionWin32::getNumberProcessors( void )
{       
	return(SystemInfo.dwNumberOfProcessors);
}
//===============================================================================
int GLRSystemDescriptionWin32::getHyperThreadAvailability( void )
{
	int IsHTSupported=0;
	__asm
	{
		mov      eax,1
		cpuid 
		test     edx,0x10000000	 // Check if bit 28 in EDX is set
		jz       HT_Not_Support
		mov      IsHTSupported,1
		HT_Not_Support:
	}       
	return IsHTSupported;
}
//===============================================================================
glrDWORD GLRSystemDescriptionWin32::getMemoryLoad( void ) 
{
	return(glrDWORD) memInfo.dwMemoryLoad;
}
//===============================================================================
glrDWORD GLRSystemDescriptionWin32::getBytesPhysicalMemory( void )
{               
	return(glrDWORD) memInfo.dwAvailPhys;
}
//===============================================================================
glrBOOL GLRSystemDescriptionWin32::doesMemoryExceed( glrDWORD MemValue )
{
	if ( memInfo.dwTotalPhys/(eGLRSystemMB) > MemValue )
	{
		return true;
	}
	return false;
}
//===============================================================================

