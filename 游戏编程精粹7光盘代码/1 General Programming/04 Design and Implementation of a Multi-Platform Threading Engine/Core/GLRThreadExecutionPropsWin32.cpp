//===============================================================================
//	GLRThreadExecutionPropsWin32.h
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadExecutionPropsWin32.h"

//===============================================================================
glrDWORD GLRThreadExecutionPropsWin32::getProcessorAffinity( int desiredCore)
{
	switch ( desiredCore )
	{
		case 1  : 
			return 0x1;
			break;
		case 2  : 
			return 0x2;
			break;
		case 3  :
			return 0x3;
			break;
		case 4  :
			return 0x4;
			break;
		case 8  :
			return 0x8;
			break;
		case 15 :
			return 0x15;
			break; 
		default : 
			return 0x1;
			break; 
	}
}
//===============================================================================
void GLRThreadExecutionPropsWin32::createTasksProcessorAffinity( void )
{            
	if ( mTasksProcessorAffinity==PA_Hard )
	{
		glrDWORD DesiredMask=getProcessorAffinity( mTasksIdealCore );
		glrDWORD ProcessMask,SystemMask;

		if ( ::GetProcessAffinityMask( ::GetCurrentProcess(),&ProcessMask,&SystemMask ) )
		{
			//2 cores - 4 process elements
			if ( SystemMask == 15 )
			{
				if ( DesiredMask<=ProcessMask )
				{
					mTasksProcessAffinityMask=DesiredMask;
				}
			} else
				//1 core - 2 process elements
				if ( SystemMask == 3 )
			{
				if ( mTasksIdealCore<=2 )
				{
					mTasksProcessAffinityMask=DesiredMask;
				}
			} else
			{
				//1 core : Regardless of what is set,this task/object will run on a
				//single process element.
				mTasksProcessAffinityMask=0x1;
			}
		}
	}
}
//===============================================================================
