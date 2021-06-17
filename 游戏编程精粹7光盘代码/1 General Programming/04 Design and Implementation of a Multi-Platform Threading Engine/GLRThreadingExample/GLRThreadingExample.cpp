//===============================================================================
//	GLRThreadingExample.cpp   
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThreadTest.h"
#include "..\core\GLRLog.h"

//===============================================================================
int _tmain(int argc, _TCHAR* argv[])
{
	//Create the GLR logging system
	GLRLog::initGLRLog();

	//Create a couple test objects to thread
	TestSystem myTestObject;
	myTestObject.objectThreadPump();

	TestSystem myTestObject2;
	myTestObject2.objectThreadPump();

	//This call should be placed in your main game loop.
	glrThreadingSystem.distribute();

	//Wait till all objects are done executing
	while ( !glrThreadingSystem.areAnyThreadsWorking() )
	{
	}

	return 0;
}
//===============================================================================

