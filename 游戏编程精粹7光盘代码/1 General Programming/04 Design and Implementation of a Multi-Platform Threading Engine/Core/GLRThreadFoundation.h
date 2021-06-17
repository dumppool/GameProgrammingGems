//===============================================================================
//	GLRThreadFoundation.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_THREAD_FOUNDATION_H
#define GLR_THREAD_FOUNDATION_H

#include "GLRThread.h"
#include "GLRThreadExecutionProps.h"
#include "GLRThreadPool.h"
#include "GLRCacheUtilities.h"

//===============================================================================
class GLRThreadFoundation
{
public:
	//ctor that also sets the platform specifier for contained object instances
	GLRThreadFoundation();
	//dtor
	~GLRThreadFoundation(); 
	//Function member if any threads are executing tasks.
	bool areAnyThreadsWorking( void );
	//Function member to terminate any threaded activity prematurely.
	void terminateAllWorkingThreads( void );
	//Function member to suspend all executing threads.
	void suspendAllWorkingThreads( void );
	//Function member to allocate an execution property
	void getTaskExecutionProps( GLRThreadExecutionProps& tep );
	//Function to submit a task to the pool
	bool submitTask( GLRIThreadableObject* pInvokingObject );
	//Process the tasks that have been submitted
	void distribute( void );
	//Explicitly used to create the thread pool
	void createThreadPool( void );
#ifdef _DEBUG
	//Function member to get the GLRThreadPool instance.
	GLRThreadPool& getTPInstance( void ); 
#endif

private:
	//static singleton object for the thread pool
	static GLRThreadPool*  mpThreadPool;	
	//The platform specifier 
	GLRPlatformSpecifier mPlatformSpecifier;
private:
	//Function member to clean up all allocations.
	void shutdown(void);
};

#endif //GLR_THREAD_FOUNDATION_H
