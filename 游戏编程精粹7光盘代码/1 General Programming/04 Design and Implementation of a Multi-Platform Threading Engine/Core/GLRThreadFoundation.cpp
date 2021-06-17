//===============================================================================
//	GLRThreadFoundation.cpp    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadFoundation.h"

//statically declared objects for the GLRThread foundation
GLRThreadPool*  GLRThreadFoundation::mpThreadPool=NULL;

//===============================================================================
GLRThreadFoundation::GLRThreadFoundation()
{
	//Set the platform
	GLRPlatformId tempId;
	//Determine and store the platform specifier for later use.
	tempId = GLRWindows;
	mPlatformSpecifier.setPlatformId(tempId);
	createThreadPool();
}
//===============================================================================
GLRThreadFoundation::~GLRThreadFoundation()
{
	shutdown();
}
//===============================================================================
void GLRThreadFoundation::shutdown( void )
{
	if ( mpThreadPool )
	{
		delete mpThreadPool;
		mpThreadPool=NULL;
	}
}
//===============================================================================
#ifdef _DEBUG
//mpr: enabled only for debugging!
GLRThreadPool& GLRThreadFoundation::getTPInstance( void )
{
	if ( mpThreadPool )
	{
		return *mpThreadPool;
	}

	createThreadPool();

	return *mpThreadPool;
}
#endif
//===============================================================================
bool GLRThreadFoundation::areAnyThreadsWorking( void )
{
	return( mpThreadPool->isPoolEmpty() );
}
//===============================================================================
void GLRThreadFoundation::terminateAllWorkingThreads( void )
{
	mpThreadPool->terminateAllWorkingThreads();
}
//===============================================================================
void GLRThreadFoundation::suspendAllWorkingThreads( void )
{
	mpThreadPool->suspendAllWorkingThreads();   
}
//===============================================================================
void GLRThreadFoundation::distribute( void )
{
	mpThreadPool->processObjectTaskList();
}
//===============================================================================
bool GLRThreadFoundation::submitTask( GLRIThreadableObject* pInvokingObject )
{
	//mpr: development note - to date there has NOT been the need
	//to surround the submitTask with a synchronization construct.
	//mpr: todo acquire lock
	return(mpThreadPool->submitTask(pInvokingObject));
	//mpr: todo release lock
}
//===============================================================================
void GLRThreadFoundation::createThreadPool( void )
{
	if ( !mpThreadPool )
	{
		mpThreadPool= new GLRThreadPool();
		//Create the platform specific thread pool, using the platform specifier.
		mpThreadPool->createThreadPool(mPlatformSpecifier.getPlatformId());
	}
}
//===============================================================================