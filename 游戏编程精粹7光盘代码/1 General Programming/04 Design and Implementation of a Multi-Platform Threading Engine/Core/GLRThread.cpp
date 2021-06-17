//===============================================================================
//	GLRThread.cpp    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRThread.h"

//===============================================================================
GLRIThread::GLRIThread()
: mThreadAvail( TS_NotExecuting )
{   
}
//===============================================================================
void GLRIThread::setThreadAvail( GLRThreadStatus Available )
{   
	mThreadAvail = Available;
}
//===============================================================================
