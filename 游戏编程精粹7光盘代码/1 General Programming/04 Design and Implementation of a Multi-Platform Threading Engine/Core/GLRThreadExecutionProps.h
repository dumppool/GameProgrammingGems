//===============================================================================
//	GLRThreadExecutionProps.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef _GLR_THREAD_EXECUTION_PROPS_H
#define _GLR_THREAD_EXECUTION_PROPS_H

#include "GLRCore.h"

//===============================================================================
enum GLRThreadStatus
{
	TS_NotExecuting = 0,
	TS_Executing    = 1,
	TS_Purging      = 2,
	TS_None         = 3,
};

//===============================================================================
enum GLRThreadPriority
{
	//GLR platform agnostic and the correspond Win32 label
	TP_Low          =0,	//Low
	TP_Normal       =1,	//Normal
	TP_High         =2,	//Above Normal
	TP_Critical     =3,	//High
	TP_TimeCritical =4,	//Time Critical
};

//===============================================================================
enum GLRProcessorAffinity
{
	PA_Soft=0,
	PA_Hard,  
	PA_Undefined,
};
//===============================================================================
class GLRThreadExecutionProps
{
public:
	GLRThreadExecutionProps();
	//Method to get the process affinity
	inline GLRProcessorAffinity getTasksDefinedProcessAffinity( void ) { return mTasksProcessorAffinity; }
	//Method to get the tasks execution priority
	inline GLRThreadPriority getTasksDefinedPriority( void ) { return mTasksPriority; }
	//Method to get the tasks process element
	inline int getTasksDefinedCore( void ) { return mTasksIdealCore; }
	//Method to get the tasks process affinity mask
	inline glrDWORD getTasksProcessAffinityMask( void ) { return mTasksProcessAffinityMask; }   
	//Method to create an affinity mask
	virtual void createTasksProcessorAffinity( void )=0;
	//Set tasks processor affinity
	void setTasksProcessAffinity( const GLRProcessorAffinity& procAffinity ){ mTasksProcessorAffinity=procAffinity; }
	//Set tasks priority
	void            setTasksPriority( const GLRThreadPriority& procPriority ){ mTasksPriority=procPriority; }
	//Set a tasks ideal core
	void            setTasksCore( const int& procCore) { mTasksIdealCore=procCore; }
	//Fill execution properties
	void            createExecutionProp( const GLRProcessorAffinity& procAffinity,
										 const GLRThreadPriority& procPriority,
										 const int& procCore );

protected:
	//Defines what core the task/job will execute on
	glrDWORD mTasksProcessAffinityMask; 
	//The tasks processor affinity		
	GLRProcessorAffinity mTasksProcessorAffinity;               
	//The tasks priority
	GLRThreadPriority mTasksPriority;   
	//The tasks ideal core
	int mTasksIdealCore;         
};

#endif //_GLR_THREAD_EXECUTION_PROPS_H
