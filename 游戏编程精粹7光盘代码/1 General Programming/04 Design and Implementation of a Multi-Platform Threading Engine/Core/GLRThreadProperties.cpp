//===============================================================================
//	GLRThreadProperties.cpp 
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadProperties.h"

//===============================================================================
GLRThreadProperties::GLRThreadProperties()
: mThreadHandle(0)
, mThreadId(0)
{
}
//===============================================================================
void GLRThreadProperties::createProperties( const GLRPlatformId& platformId )
{
	//Generate the threads properties
	switch ( platformId )
	{
		case GLRWindows:
			setStackSize(65536);
			break;
		default:        
			setStackSize(65536);
			break;
	}
}
//===============================================================================
void GLRThreadProperties::setStackSize( int stackSize )
{
	mStackSize=stackSize;
}
//===============================================================================