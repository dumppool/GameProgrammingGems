//===============================================================================
//	GLRThreadWindows.cpp 
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadWindows.h"
#include "GLRLog.h"

//===============================================================================
GLRThreadWindows::GLRThreadWindows()
{
}
//===============================================================================
bool GLRThreadWindows::createThread( const GLRThreadProperties &creationProps )
{   
	glrDWORD ThreadId;
	glrHANDLE ret=::CreateThread( NULL,0,threadFunction,this,CREATE_SUSPENDED,&ThreadId );   
	if ( ret != NULL )
	{
		mThreadProp.mThreadHandle=ret;
		mThreadProp.mThreadId=ThreadId;
		return true;
	}
	return false;
}
//===============================================================================
DWORD __stdcall GLRThreadWindows::threadFunction( void* pThis )
{
	return((GLRIThread*)pThis)->executeThread();   
}
//===============================================================================
glrDWORD GLRThreadWindows::executeThread( void )
{
	//GLRLog::write("Executing Thread... \n");	
	//Alter the threads execution properties
	changeThreadsExecutionProperties(mpThreadsObject->getExecutionProps());
	//Execute the associated task
	mpThreadsObject->taskToExecute();
	//GLRLog::write("Execution of thread has completed! \n");				
	setThreadAvail( TS_Purging ); 
	suspendThread();

	return 0;
}
//===============================================================================
void GLRThreadWindows::changeThreadProperties( const GLRThreadTask &attributes )
{   
}
//=================================================================================
void GLRThreadWindows::changeThreadPropertiesandResume( const GLRThreadTask &attributes )
{   
	mThreadsTask.taskToExecute = attributes.taskToExecute;
	setThreadAvail(TS_Executing);
	resumeThread();
}
//===============================================================================
void GLRThreadWindows::changeThreadPropertiesandResume( GLRIThreadableObject* attributes )
{   
	//This contains execution data for the thread. Its callback and potential arguments
	mpThreadsObject=attributes;
	setThreadAvail( TS_Executing );
	resumeThread();
}   
//=================================================================================
void GLRThreadWindows::resumeThread( void )
{
	::ResumeThread(mThreadProp.mThreadHandle);
}
//=================================================================================
void GLRThreadWindows::terminateThread( void )
{
	::TerminateThread(mThreadProp.mThreadHandle,(DWORD)-1);
}
//=================================================================================
void GLRThreadWindows::suspendThread( void )
{
	::SuspendThread(mThreadProp.mThreadHandle);
}
//=================================================================================
void GLRThreadWindows::changeThreadsExecutionProperties( GLRThreadExecutionProps &attributes )
{   
	//1.) Establish the threads affinity	
	if ( attributes.getTasksDefinedProcessAffinity()==PA_Hard )
	{
		//Establishes a preferred core for a particular thread, as well as 
		//restricts it - to that processor. Thus the "hard" description.
		::SetThreadAffinityMask(mThreadProp.mThreadHandle,attributes.getTasksProcessAffinityMask());    
	} else
	{
		//Establishes a preferred core for a particular thread but it does NOT restrict 
		//the thread to that core. 
		//This is just a "hint" to the OS - thus the soft description.
		::SetThreadIdealProcessor(mThreadProp.mThreadHandle,attributes.getTasksDefinedCore()); 
	}

	//2.) Establish a priority for a particular thread
	int threadPriority;
	switch ( attributes.getTasksDefinedPriority() )
	{
		case TP_Low: 
			threadPriority=THREAD_PRIORITY_LOWEST;
			break;
		case TP_Normal: 
			threadPriority=THREAD_PRIORITY_NORMAL;
			break;
		case TP_High: 
			threadPriority=THREAD_PRIORITY_ABOVE_NORMAL;
			break;
		case TP_Critical: 
			threadPriority=THREAD_PRIORITY_HIGHEST;
			break;
		case TP_TimeCritical: 
			threadPriority=THREAD_PRIORITY_TIME_CRITICAL;
			break;
		default: 
			threadPriority=THREAD_PRIORITY_NORMAL;
			break;
	}

	::SetThreadPriority(mThreadProp.mThreadHandle,threadPriority);
}
//=================================================================================
