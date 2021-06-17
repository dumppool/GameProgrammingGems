//===============================================================================
//	GLRCacheUtilities.h
//	(c) 2006, 2007 Michael Ramsey. All Rights Reserved.
//  The information and source code contained herein is the
//  exclusive property of Michael Ramsey. No part of this software
//  may be used, reproduced, stored or distributed in any form,
//  without explicit written authorization from Michael Ramsey.
//===============================================================================

// Methods to organize data structures for better cache coherency.    

#ifndef GLR_CACHE_UTILITIES_H
#define GLR_CACHE_UTILITIES_H

#include "GLRDataTypes.h"

//Determine alignment for cache padding macro
#ifdef _WINDOWS_
	#define CACHE_ALIGNMENT 32
#endif

#ifdef _XENON_
	#define CACHE_ALIGNMENT 32
#endif

//Macro 1: Allows you to group your data together in
//cache line (size) chunks and on cache-line boundaries.

#define GLRCachePad(Name, BytesSoFar) \
	BFBYTE Name[CACHE_ALIGNMENT - ((BytesSoFar) % CACHE_ALIGNMENT)]

#endif //GLR_CACHE_UTILITIES_H
