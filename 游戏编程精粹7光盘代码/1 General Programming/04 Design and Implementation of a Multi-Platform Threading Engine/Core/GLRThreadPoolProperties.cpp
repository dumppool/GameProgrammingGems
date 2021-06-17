//===============================================================================
//	GLRThreadProperties.cpp 
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadPoolProperties.h"

//===============================================================================
GLRThreadPoolProperties::GLRThreadPoolProperties()
: mMaxThreads( 10 )
, mMaxTasks( 25 )
{
}
//===============================================================================
void GLRThreadPoolProperties::setMaxThreads( int MaxThreads )
{
	mMaxThreads=MaxThreads;
}
//===============================================================================
void GLRThreadPoolProperties::setMaxTasks( int MaxTasks )
{
	mMaxTasks=MaxTasks;
}
//===============================================================================
void GLRThreadPoolProperties::setTaskPoolLock( glrBOOL status )
{
	mBlockTask=status;
}
//===============================================================================
glrBOOL GLRThreadPoolProperties::getTaskPoolLock( void )
{
	return mBlockTask;
}
//===============================================================================
