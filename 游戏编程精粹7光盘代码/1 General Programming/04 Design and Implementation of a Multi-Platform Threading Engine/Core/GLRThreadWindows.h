//===============================================================================
//	GLRThreadWindows.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================


#ifndef _GLR_THREAD_WINDOWS_H
#define _GLR_THREAD_WINDOWS_H

#include "GLRThread.h"
#include "GLRWindows.h"

//===============================================================================

class GLRThreadWindows : public GLRIThread
{
public:
	GLRThreadWindows();
	//Thread creation method
	virtual bool createThread( const GLRThreadProperties& creationProps ); 
	//Alter a threads properties, such as its stack size, id etc..
	virtual void changeThreadProperties( const GLRThreadTask& attributes ); 
	//Alter a threads properties, such as its stack size, id etc.. and then execute it
	virtual void changeThreadPropertiesandResume( const GLRThreadTask& attributes );   
	//Alter the threads properties, such as its stack size, id etc.. and then execute it
	virtual void changeThreadPropertiesandResume( GLRIThreadableObject* pAttributes );     
	//Alter the threads execution properties
	virtual void changeThreadsExecutionProperties( GLRThreadExecutionProps& executionProps );
	//Controls how a thread gets execution
	virtual glrDWORD executeThread( void );        
	//Allows the thread to execute
	virtual void resumeThread( void );    

	//Destroy a thread
	virtual void terminateThread( void );
	//Suspend a thread from executing
	virtual void suspendThread( void );

private:
	//Forwarding mechanism for threads "execution" point
	static  glrDWORD __stdcall threadFunction(void* pThis);
};

#endif //_GLR_THREAD_WINDOWS_H
