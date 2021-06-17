//===============================================================================
//	GLRThread.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_THREAD_H
#define GLR_THREAD_H

#include "GLRCore.h"
#include "GLRThreadProperties.h"
#include "GLRThreadExecutionProps.h"

//===============================================================================
struct GLRThreadTask
{
	//Threads execution callback
	void (*taskToExecute)();
	//Threads execution arguments
	void *ArgList;
	//Reserved deletion flag
	bool Delete;
};
//===============================================================================
class GLRIThreadableObject
{
public:
	//Ctor
	GLRIThreadableObject()
	: mTaskExecutionProps(NULL)
	, mObjectId(0)
	{
	}
	//Method that is overrideable and maps an outside function which is normally a callback
	virtual void taskToExecute(void)=0;
	//Function member to retrieve the id for an object for access through object registry
	unsigned int getObjectId(void) {return mObjectId;}
	//Method to retrieve the execution properties
	virtual GLRThreadExecutionProps& getExecutionProps(void)=0;
protected:
	//Tasks execution properties
	GLRThreadExecutionProps* mTaskExecutionProps;
private:    
	//Used to register an object with the object registry
	glrU32 mObjectId;   
};
//===============================================================================
template <class T> 
class GLRThreadedTask : public GLRIThreadableObject
{
public:
	//Ctor - no setup occuring
	GLRThreadedTask()
	: mInvokingObject(NULL)
	, mFunctionToExecute(NULL)  
	{
	}   
	//Ctor - registering the invoking object and the function that will be executed,
	// requiring task execution properties
	GLRThreadedTask( T *pInvokingObjectName, void (T::*pFunctionToExecute)(void),
					GLRThreadExecutionProps* pTaskExecutionProps )
	: mInvokingObject( pInvokingObjectName )
	, mFunctionToExecute( pFunctionToExecute )
	, mTaskExecutionProps( pTaskExecutionProps )
	{
	}
	//Function member to fill in the invoking object and the function that it executes
	void createThreadedTask( T *pInvokingObjectName, 
							void (T::*pFunctionToExecute)(void),
							GLRThreadExecutionProps* pTaskExecutionProps )
	{
		mInvokingObject=pInvokingObjectName;
		mFunctionToExecute=pFunctionToExecute;
		mTaskExecutionProps=pTaskExecutionProps;
	}
	//Method calling the registered objects (mInvokingObject) function
	virtual void taskToExecute( void )
	{
		(mInvokingObject->*mFunctionToExecute)();
	}   
	GLRThreadExecutionProps& getExecutionProps( void ) {return *mTaskExecutionProps;}
private:
	//Name of the object that object thread task will use
	T* mInvokingObject;         
private:
	//Method inside the invoking object that will be executed
	void (T::*mFunctionToExecute)( void );
};

//===============================================================================
class GLRIThread  
{
public:
	GLRIThread();
	//Thread creation method
	virtual bool createThread( const GLRThreadProperties& CreationProps )=0;
	//Alter the threads properties, such as its stack size, id etc..
	virtual void changeThreadProperties( const GLRThreadTask& Attributes )=0;  
	//Alter the threads properties, such as its stack size, id etc.. and then execute it
	virtual void changeThreadPropertiesandResume( const GLRThreadTask& Attributes )=0;
	//Alter the threads properties, such as its stack size, id etc.. and then execute it
	virtual void changeThreadPropertiesandResume( GLRIThreadableObject* pAttributes )=0;   
	//Alter the threads execution properties
	virtual void changeThreadsExecutionProperties( GLRThreadExecutionProps& ExecutionProps )=0;
	//Controls how a thread gets executed
	virtual glrDWORD executeThread( void )=0;
	//Allows the thread to execute (run)
	virtual void resumeThread( void )=0;
	//Destroy a thread, permanently
	virtual void terminateThread( void )=0;
	//Suspend a thread from executing
	virtual void suspendThread( void )=0;
	//Accessor to determine if a thread is available for work?
	GLRThreadStatus getThreadAvail( void ) { return mThreadAvail; }
	//Accessor to determine if a thread is available for work?
	void setThreadAvail( GLRThreadStatus status ); 

protected:
	//Threads static properties - from task to task
	GLRThreadProperties  mThreadProp;  
	//This contains execution data for the thread. Its callback and potential arguments
	GLRThreadTask mThreadsTask; 
	//This contains execution data for the thread. Its callback and potential arguments
	GLRIThreadableObject *mpThreadsObject;  
	//Is the thread available?
	GLRThreadStatus mThreadAvail;
};    
#endif // GLR_THREAD_H
