//===============================================================================
//	GLRSystemDescWin32.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_SYSTEM_DESC_WIN32_H
#define GLR_SYSTEM_DESC_WIN32_H

#include "GLRSystemDesc.h"
#include "GLRWindows.h"

//===============================================================================
class GLRSystemDescriptionWin32 : public GLRISystemDescription
{

public: 
	GLRSystemDescriptionWin32();
	int getNumberProcessors( void );
	int getHyperThreadAvailability( void );
	glrDWORD getMemoryLoad( void );
	glrDWORD getBytesPhysicalMemory( void );
	glrBOOL doesMemoryExceed( glrDWORD MemValue );

private:	
	virtual void    startup( void );

private:
	MEMORYSTATUS memInfo;
	SYSTEM_INFO SystemInfo;
};

#endif //GLR_SYSTEM_DESC_WIN32_H
