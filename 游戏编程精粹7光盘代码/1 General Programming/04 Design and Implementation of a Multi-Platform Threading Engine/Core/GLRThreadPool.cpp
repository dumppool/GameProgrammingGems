//===============================================================================
//	GLRThreadPool.cpp    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadPool.h"
#include "GLRThreadWindows.h"
#include "GLRThreadProperties.h"
#include "GLRLog.h"

//===============================================================================
GLRThreadPool::GLRThreadPool()
{
}
//===============================================================================
GLRThreadPool::~GLRThreadPool()
{
	shutdown();
}
//===============================================================================
bool GLRThreadPool::shutdown(void)
{
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		delete (*mThreadListIt);
	}
	mThreadList.clear();    
	return true;
}
//===============================================================================
bool GLRThreadPool::createThreadPool( const GLRPlatformId& ThreadType )
{
	//1.) Generate properties for the thread pool
	switch ( ThreadType )
	{
		case GLRWindows:
			mTPProps.setMaxTasks(25);
			mTPProps.setMaxThreads(15);
			mTPProps.setTaskPoolLock(false);
			break;

		default:        
			mTPProps.setMaxTasks(25);
			mTPProps.setMaxThreads(10);
			mTPProps.setTaskPoolLock(false);
			break;
	}

	//2.) Create the thread pools system specific threads
	for ( int i=0;i<mTPProps.mMaxThreads;i++ )
	{
		GLRIThread *pThread = new GLRThreadWindows();
		//Create a property object for every particular thread
		GLRThreadProperties threadProps;
		threadProps.createProperties(ThreadType);
		pThread->createThread(threadProps);
		mThreadList.push_back(pThread);
	}
	return true;
}
//===============================================================================
bool GLRThreadPool::submitTask( const GLRThreadTask &Task )
{
	if ( !mTPProps.getTaskPoolLock() )
	{
		if ( (int)mTaskList.size() > mTPProps.mMaxTasks )
		{
			return false;
		}
		mTaskList.push_front(Task);
		return true;
	}
	return false;
}
//===============================================================================
bool GLRThreadPool::submitTask( GLRIThreadableObject* InvokingObject )
{   
	if ( !mTPProps.getTaskPoolLock() )
	{
		if ( (int)mTaskList.size() > mTPProps.mMaxTasks )
		{
			return false;
		}
		mTTaskList.push_front(InvokingObject);
		return true;
	}
	return false;
}
//===============================================================================
void GLRThreadPool::processObjectTaskList( void )
{   
	int MissCount=0;

	//1.) Previous misses
	if ( mTTaskMissList.size()>0 )
	{
		TTaskListIT TaskListEnd=mTTaskMissList.end();

		for ( mTTaskListIt=mTTaskMissList.begin(); mTTaskListIt!=mTTaskMissList.end();++mTTaskListIt )
		{
			GLRIThread *pWorker=getAvailThread();
			if ( pWorker )
			{
				pWorker->changeThreadPropertiesandResume((*mTTaskListIt));
			} else
			{
				MissCount++;
			}
		}   
	}

	if ( MissCount>0 )
	{
		OutputDebugString(_T("Potentially serious problem with task execution!"));
	}
	else
	{
		mTTaskMissList.clear();
	}

	//2.) Newly added tasks
	TTaskListIT TaskListEnd=mTTaskList.end();

	for ( mTTaskListIt=mTTaskList.begin(); mTTaskListIt!=mTTaskList.end();++mTTaskListIt )
	{
		//general algorithm is as follows:
		//
		//1.) assign this function to a thread: (*mTTaskListIt).function()
		// 1.1) execute that thread via resume
		//2.) remove the task from the task list - which will free up a task
		//    slot for another potential work task
		GLRIThread *pWorker=getAvailThread();
		if ( pWorker )
		{
			pWorker->changeThreadPropertiesandResume((*mTTaskListIt));
		} else
		{
			//save task in miss list
			mTTaskMissList.push_front(*mTTaskListIt);
		}
	}   

	mTTaskList.clear();

	promoteCleanupToAvailability();
}
//===============================================================================
GLRIThread* GLRThreadPool::getAvailThread( void )
{
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		if ( (*mThreadListIt)->getThreadAvail() == TS_NotExecuting )
		{
			return *mThreadListIt;
		}
	}
	return NULL;
}
//===============================================================================
bool GLRThreadPool::areAnyThreadsWorking(void)
{
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		//Is this thread working on anything?
		if ( (*mThreadListIt)->getThreadAvail() == TS_NotExecuting )
		{
			return true;
		}
	}
	return false;
}
//===============================================================================
//todo: should not terminate a thread if it is within a synchronization 
// mechanism (GLRSync)
void GLRThreadPool::terminateAllWorkingThreads( void )
{
	//1.) Disallow any new tasks to be submitted
	mTPProps.setTaskPoolLock(true);
	//2.) Loop through and terminate all executing threads
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		if ( (*mThreadListIt)->getThreadAvail() == TS_NotExecuting )
		{
			(*mThreadListIt)->terminateThread();
		}
	}
}
//===============================================================================
void GLRThreadPool::suspendAllWorkingThreads( void )
{
	//1.) Disallow any new tasks to be submitted
	mTPProps.setTaskPoolLock(true);
	//2.) Loop through and terminate all executing threads
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		if ( (*mThreadListIt)->getThreadAvail() == TS_NotExecuting )
		{
			(*mThreadListIt)->suspendThread();
		}
	}
}
//===============================================================================
int GLRThreadPool::getThreadAvailableCount( void )
{
	int ThreadAvailCnt=0;
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		if ( (*mThreadListIt)->getThreadAvail() == TS_NotExecuting ||
			 (*mThreadListIt)->getThreadAvail() == TS_Purging
		   )
		{
			ThreadAvailCnt++;               
		}
	}   
	return ThreadAvailCnt;
}
//===============================================================================
void GLRThreadPool::promoteCleanupToAvailability(void)
{
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		if ( (*mThreadListIt)->getThreadAvail() == TS_Purging )
		{
			(*mThreadListIt)->setThreadAvail(TS_NotExecuting);
		}
	}   
}
//===============================================================================
int GLRThreadPool::getSpecificThreadStatusCount(GLRThreadStatus ThreadType)
{
	int ThreadAvailCnt=0;
	ThreadListIT ThreadListEnd=mThreadList.end();
	for ( mThreadListIt=mThreadList.begin();mThreadListIt!=ThreadListEnd;++mThreadListIt )
	{
		if ( (*mThreadListIt)->getThreadAvail() == ThreadType )
		{
			ThreadAvailCnt++;               
		}
	}   
	return ThreadAvailCnt;
}
//===============================================================================
bool GLRThreadPool::isPoolEmpty( void )
{
	int threadAvailabilityCnt=getThreadAvailableCount();
	if ( threadAvailabilityCnt == mTPProps.getMaxThreadCount() )
	{
		return true;
	}
	return false;
}
//===============================================================================