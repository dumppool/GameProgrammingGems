//===============================================================================
//	GLRLog.h
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_LOG_H
#define GLR_LOG_H

#include <iostream>
#include <fstream>
//===============================================================================
class GLRLog
{
public:
    GLRLog();
    ~GLRLog();  

    static void		initGLRLog(void);
    static void		shutdownGLRLog(void);
    static GLRLog*	getGLRLog(void) { return GLRLog::sManager; }
	static void		write(char* pStr);
    static void		write(const char* pStr);
    static void		write(int i);
    static void		write(long i);
    static void		write(float x);
    static void		write(double x);

private:
    static std::filebuf		mLogFile;
	static std::ostream*	mpLogStream;

private:
	void shutdown(void);

protected:
	static GLRLog* sManager;
	
};

//===============================================================================

#endif //GLR_LOG_H

