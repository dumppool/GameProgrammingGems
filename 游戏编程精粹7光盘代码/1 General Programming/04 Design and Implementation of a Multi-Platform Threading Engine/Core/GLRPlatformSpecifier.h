//===============================================================================
//	GLRPlatformSpecifier.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef _GLR_PLATFORM_SPECIFIER_H
#define _GLR_PLATFORM_SPECIFIER_H


enum GLRPlatformId
{
	GLRWindows=0,
	GLRXenon,
	GLRPS3,
	//===new cfgs
	GLRX86,
	GLRAMD,
	GLRMotorola,
	GLRNone
};
//===============================================================================
class GLRPlatformSpecifier
{
public:
	GLRPlatformSpecifier()
	: mPlatformId(GLRWindows)
	{
	}
	inline GLRPlatformId getPlatformId(void)
	{
		return mPlatformId;
	}
	inline void setPlatformId(const GLRPlatformId& id)
	{
		mPlatformId=id;
	}
private:
	GLRPlatformId mPlatformId;
};

#endif //_GLR_PLATFORM_SPECIFIER_H