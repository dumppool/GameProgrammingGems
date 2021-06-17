//-------------------------------------------------------------------------------
// PlayerViz (Player Trace Visualizer) 
// Version 0.5 Released 08.10.2007
// Part of the CGUL (Common Games Understanding and Learning) Toolkit
//-------------------------------------------------------------------------------
//  Copyright (C) 2007 Priyesh N. Dixit and G. Michael Youngblood
//  Game Intelligence Group, Games + Learning Lab
//  The University of North Carolina at Charlotte
//  College of Computing and Informatics, Department of Computer Science
//  9201 University City Blvd, Charlotte, NC 28223-0001
//  {pndixit, youngbld}@uncc.edu
//-------------------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
//-------------------------------------------------------------------------------

#ifndef GLWIDGET_H
#define GLWIDGET_H

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <vector>

#include "Camera.h"
#include "RGBColorMap.h"
#include "CGULData.h"
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Output.H>

//Width and height of the GLWidget
const int WIDTH=700;
const int HEIGHT=600;

class GLWidget : public Fl_Gl_Window
{
  private:
	//private vars go here
	int width,height;
	float scaleFactor;
	
	float yawStickSize;
	float playerCircleRadius;
	
	Fl_Multi_Browser* playerTraceBrowser;
	Fl_Output* lblCurrentMap;
		
	RGBColorMap colorMap;
	Camera cam;
	
	//Camera enum
	enum {CAM_ZOOM, CAM_PAN, CAM_ROTATE, CAM_NONE} cameraMode;
	int lastX, lastY;
	
	//allows for movement while rotating the view
	float zoomAmount;
	float vertPanAmount;
	float horizPanAmount;
		
	bool showAxes, lightEnabled;
	int cameraAngle;
	
	//CGUL data
	CGULData cgulData;
	
	
  public:
	
    GLWidget( int x, int y, int w, int h, const char *title, Fl_Multi_Browser* browser, Fl_Output* lblCurrentMap );

    void setGLView();
    int handle( int event );
	
	void drawSphere(float x1, float y1, float z1, float radius, bool filled);
    mVector getVectorByAngles(float azimuth, float zenith, float length);
	void draw();
		
	void toggleAxes();
	void toggleLighting();
	void toggleNegativeSpace();
	void toggleTrimNegative();
	void toggleGateways();
	void toggleSwapYZ();
	void toggleFlipX();
	void setWorldScale(float scale);
	void resetCamera();
	
	void setCameraAngle(int c);
	void setImageName(std::string name);
	
	void readDataFile(const char* filename, const char* sspspath, const char* diaspath);
    void readPlayerTraceFile(const char* filename);
	void readSSPSFile(const char* sspspath);
	void readDiasData(std::string filename);
	void clearData();
	void removeSelectedTraces();
	void setDataDirectory(std::string dir);
	
	
	int WindowDump();
};

#endif
