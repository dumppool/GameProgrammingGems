//===============================================================================
//	GLRLog.cpp   
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#include "stdafx.h"
#include "GLRLog.h"
 
//Create our object
GLRLog* GLRLog::sManager=NULL;

//===============================================================================
GLRLog::GLRLog()
{ 
}
//===============================================================================
GLRLog::~GLRLog()
{ 
}
//===============================================================================
void GLRLog::shutdown(void)
{		
	if (GLRLog::sManager)
	{	
		GLRLog::mLogFile.close();
		delete mpLogStream;
        GLRLog* TempManager = GLRLog::sManager;
		GLRLog::sManager = NULL;		
		delete TempManager;
	}
}
//===============================================================================
void GLRLog::initGLRLog(void)
{
	if (GLRLog::sManager == NULL)
	{
		GLRLog::sManager = new GLRLog();		
		mLogFile.open("GLRLog.dat", std::ios::out);
		mpLogStream = new std::ostream(&mLogFile);
	}
}
//===============================================================================
void GLRLog::shutdownGLRLog(void)
{
	if (GLRLog::sManager)
	{
		GLRLog::sManager->shutdown();
		delete GLRLog::sManager;
		GLRLog::sManager = 0;
	}
}
//===============================================================================
void GLRLog::write(char* pStr)
{    
	*(mpLogStream) << pStr;   
}
//===============================================================================

void GLRLog::write(const char* pStr)
{
    *(mpLogStream) << pStr;    
}
//===============================================================================
void GLRLog::write(int i)
{
    *(mpLogStream) << i;     
}
//===============================================================================
void GLRLog::write(long i)
{     
    *(mpLogStream) << i;       
}
//===============================================================================
void GLRLog::write(float x)
{
    *(mpLogStream) << x;       
}
//===============================================================================
void GLRLog::write(double x)
{       
	*(mpLogStream) << x;      
}
//===============================================================================
std::filebuf GLRLog::mLogFile;
std::ostream * GLRLog::mpLogStream;
//===============================================================================

