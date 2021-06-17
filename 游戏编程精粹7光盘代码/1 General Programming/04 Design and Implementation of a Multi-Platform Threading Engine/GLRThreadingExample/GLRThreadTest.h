//===============================================================================
//	GLRThreadTest.h
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_THREAD_TEST_H
#define GLR_THREAD_TEST_H

#include "..\core\GLRThread.h"
#include "..\core\GLRThreadExecutionPropsWin32.h"

//===============================================================================

class TestSystem
{
public:
	TestSystem();   
	~TestSystem();
	void theIncredibleGameObject( void );
	void objectThreadPump( void );

private:
	GLRThreadedTask<TestSystem> mThreadedTask;
	GLRThreadExecutionProps *mThreadedProps;
};
//===============================================================================
#endif //GLR_THREAD_TEST_H