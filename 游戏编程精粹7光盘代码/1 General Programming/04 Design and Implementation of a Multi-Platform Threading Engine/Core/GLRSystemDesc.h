//===============================================================================
//	GLRSystemDesc.h    
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

#ifndef GLR_SYSTEM_DESC_H
#define GLR_SYSTEM_DESC_H

#include "GLRDataTypes.h"

//===============================================================================
class GLRISystemDescription
{
public: 
	GLRISystemDescription();
	virtual int getNumberProcessors( void )=0;
	virtual int getHyperThreadAvailability( void )=0;
	virtual glrDWORD getMemoryLoad( void )=0;
	virtual glrDWORD getBytesPhysicalMemory( void )=0;
	virtual glrBOOL	doesMemoryExceed( glrDWORD dwMemValue )=0;

protected:
	enum 
	{
		eGLRSystemMB = 1048576,        //1024*1024
	};

private:		
	glrDWORD mBytesPhysicalRam;
	glrDWORD mMemoryLoad;

private:	
	virtual void startup( void )=0;			    
};

#endif //GLR_SYSTEM_DESC_H






