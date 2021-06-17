//===============================================================================
//	GLRThreadTest.cpp
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadTest.h"

//===============================================================================
TestSystem::TestSystem()
{
	mThreadedProps = new GLRThreadExecutionPropsWin32();
}
//===============================================================================
TestSystem::~TestSystem()
{
	if ( mThreadedProps )
	{
		delete mThreadedProps;
	}
}
//===============================================================================
void TestSystem::theIncredibleGameObject( void )
{
	int i=0;
	for ( ;i<10000; ++i )
	{
		char buffer[64]={0};
		wsprintf( buffer,"Ship the game! %i \n",i );
		OutputDebugString( buffer );
	}
}
//===============================================================================
void TestSystem::objectThreadPump( void )
{
	mThreadedTask.createThreadedTask( this,&TestSystem::theIncredibleGameObject,mThreadedProps );
	glrThreadingSystem.submitTask( &mThreadedTask );
}
//===============================================================================