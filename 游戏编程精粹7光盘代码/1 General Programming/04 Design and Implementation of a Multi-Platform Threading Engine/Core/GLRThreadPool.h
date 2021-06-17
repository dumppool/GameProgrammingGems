//===============================================================================
//	GLRThreadPool.cpp    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_THREAD_POOL_H
#define GLR_THREAD_POOL_H

#include "GLRStl.h"
#include "GLRThread.h"
#include "GLRThreadPoolProperties.h"

//===============================================================================
class GLRThreadPool
{

public:
	//Ctor
	GLRThreadPool();    
	//Dtor
	~GLRThreadPool();
	//Function member to allocates the applications threads at startup
	bool createThreadPool( const GLRPlatformId& threadType ); 
	//Function member to adds a task function to the tasklist
	bool submitTask( const GLRThreadTask& task ); 
	//Function member to adds a task function to the object tasklist
	bool submitTask( GLRIThreadableObject* pInvokingObject );	
	//Function member to associates tasks within objects to threads and then allows them to execute
	void processObjectTaskList( void );
	//Function member that destroy's thread handles, memallocs etc...
	bool shutdown( void );
	//Determines if any threads are working on tasks.
	bool areAnyThreadsWorking( void );
	//Terminate all threads that are executing.
	void terminateAllWorkingThreads( void );
	//Suspend all executing threads.
	void suspendAllWorkingThreads( void );
	//Function member to get the number of tasks in queue
	int  getTaskCount( void ) { return(int)mTTaskList.size(); }    
	//Function member to get the number of tasks in missed queue
	int  getTasksMissedCount( void ) { return(int)mTTaskMissList.size(); }
	//Function member to get the number of threads that are available
	int  getThreadAvailableCount( void );
	//Function member to retrieve the count of a specific status 
	int getSpecificThreadStatusCount( GLRThreadStatus ThreadType );
	//Is the pool free of tasks?
	bool isPoolEmpty( void );

private:    
	//Threads list	
	typedef GLRVector<GLRIThread*>                        ThreadList;
	typedef GLRVector<GLRIThread*> :: iterator            ThreadListIT;

	//The thread pools list of threads
	ThreadList      mThreadList;
	//Iterator to access the threading pool
	ThreadListIT    mThreadListIt;      

	//Queued tasks
	typedef GLRDeque<GLRThreadTask>                       TaskList;
	typedef GLRDeque<GLRThreadTask> :: iterator           TaskListIT;

	//Methods within an object
	typedef GLRDeque<GLRIThreadableObject*>                       TTaskList;
	typedef GLRDeque<GLRIThreadableObject*> :: iterator           TTaskListIT;
	TTaskList   mTTaskList, mTTaskMissList; 
	//Iterator for the task list
	TTaskListIT mTTaskListIt;

	//List of tasks to execute
	TaskList    mTaskList,mTaskMissList;    
	//Iterator for the task list
	TaskListIT  mTaskListIt; 
	//Thread pool properties (max tasks, max thread etc...)
	GLRThreadPoolProperties mTPProps;

private:
	//Method to get the next available thread
	GLRIThread* getAvailThread( void );
	//Method to deletes a tasks from the task list
	bool removeTask( TaskListIT tlIT );
	//Reserved cleanup for missed task list
	bool cleanTaskList( void );
	//Method to lock or unlock the task pool
	void setTaskPoolLock( const bool& status );
	//Accessor to get the status of the task pool
	bool getTaskPoolLock( void );
	//Method to promote threads from Cleanup status to Available
	void promoteCleanupToAvailability( void );
};    

#endif // GLR_THREAD_POOL_H
