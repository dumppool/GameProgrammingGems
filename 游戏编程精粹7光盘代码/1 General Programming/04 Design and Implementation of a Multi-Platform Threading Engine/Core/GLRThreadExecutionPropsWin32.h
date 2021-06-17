//===============================================================================
//	GLRThreadExecutionPropsWin32.h
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_THREAD_EXECUTION_PROPS_WIN32_H
#define GLR_THREAD_EXECUTION_PROPS_WIN32_H

#include "GLRThreadExecutionProps.h"
#include "GLRWindows.h"

//===============================================================================
class GLRThreadExecutionPropsWin32 : public GLRThreadExecutionProps
{
public: 
	//Overloaded method to create/verify a thread's affinity
	virtual void createTasksProcessorAffinity( void );    
private:
	//Local method to determine processor affinity mask based on desired core
	glrDWORD getProcessorAffinity(int desiredCore);
};
#endif //GLR_THREAD_EXECUTION_PROPS_WIN32_H

