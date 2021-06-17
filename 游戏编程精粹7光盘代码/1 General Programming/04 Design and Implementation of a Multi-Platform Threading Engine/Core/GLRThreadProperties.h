//===============================================================================
//	GLRThreadProperties.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef _GLR_THREAD_PROPERTIES_H
#define _GLR_THREAD_PROPERTIES_H

#include "GLRCore.h"
#include "GLRPlatformSpecifier.h"

//===============================================================================
class GLRThreadProperties
{
public:
	GLRThreadProperties();
	//Function member to set the stack size of a particular thread
	void setStackSize( int stackSize );
	//Function member to populate the threads properties
	void createProperties( const GLRPlatformId& platformId );
public:
	//Stack size for a particular thread
	glrU32 mStackSize;
	//A particular threads handle
	glrHANDLE mThreadHandle;
	//A particular threads id
	glrU32 mThreadId;  
};
#endif //_GLR_THREAD_PROPERTIES_H
