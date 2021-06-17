//===============================================================================
//	GLRThreadPoolProperties.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef _GLR_THREAD_POOL_PROPERTIES_H
#define _GLR_THREAD_POOL_PROPERTIES_H

#include "GLRCore.h"
#include "GLRPlatformSpecifier.h"

//===============================================================================
class GLRThreadPoolProperties
{
public:
	GLRThreadPoolProperties();
	//Function member to set the maximum allowable threads
	void setMaxThreads( int MaxThreads ); 
	//Function member to set the maximum allowable number of tasks
	void setMaxTasks( int MaxTasks ); 
	//Function member to set the task pool lock
	void setTaskPoolLock( glrBOOL status );   
	//Get the task pool lock
	glrBOOL getTaskPoolLock( void );
	//Get the maximum allowed threads
	int getMaxThreadCount( void ) { return mMaxThreads; }
public:
	//Maximum number of threads that the pool can allocate
	int mMaxThreads;
	//Maximum number of tasks that the pool can allocate
	int mMaxTasks;
	//Block any new tasks from being submitted
	glrBOOL mBlockTask;
};
#endif //_GLR_THREAD_POOL_PROPERTIES_H
