#if !defined( GLINC_H_ )
#define GLINC_H_

#ifdef _MSC_VER
#pragma once
#endif

#ifdef _WIN32
	#define NOMINMAX // Needed to avoid name collision with numeric_limits
	#include <windows.h>
	#include <gl/gl.h>
	#include <gl/glu.h>
	#include "glut32/glut.h"
#endif
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else //linux, presumably?
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include "glut32/glut.h"
#endif

#endif // #if !defined( GLINC_H_ )
