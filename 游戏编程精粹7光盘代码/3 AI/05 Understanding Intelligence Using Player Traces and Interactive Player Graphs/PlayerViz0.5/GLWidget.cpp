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

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GLWidget.h"

#include "FL/fl_draw.H"

using namespace std;

GLWidget::GLWidget( int x, int y, int w, int h, const char *l, Fl_Multi_Browser* browser, Fl_Output* label )
		: Fl_Gl_Window( x, y, w, h, l )
{
	width=w;
	height=h;
	playerTraceBrowser = browser;
	lblCurrentMap = label;
	
	yawStickSize = 50;
	playerCircleRadius = 2;
	
	//define the colormap
	colorMap.addColor(0, 0,0,1);
	colorMap.addColor(0.25, 0,1,1);
	colorMap.addColor(0.5, 0,1,0);
	colorMap.addColor(0.75, 1,1,0);
	colorMap.addColor(1, 1,0,0);
	
	cameraMode=CAM_NONE;
	cameraAngle=-1;
	cam.init(FIRST_PERSON,-100,100,200);
	
	showAxes=true;
	lightEnabled=true;
	
	//turn on negative
	cgulData.ssps.toggleDrawNegativeSpace();
	
	cgulData.ssps.setFaceTextures(false);
}

void GLWidget::toggleAxes()
{
	showAxes= 1 - showAxes;
	redraw();
}

void GLWidget::toggleLighting()
{
    lightEnabled = 1 - lightEnabled;
    redraw();
}

void GLWidget::toggleNegativeSpace()
{
	cgulData.ssps.toggleDrawNegativeSpace();
	redraw();
}

void GLWidget::toggleTrimNegative()
{
	cgulData.ssps.toggleTrimNegative();
	redraw();
}

void GLWidget::toggleGateways()
{
	cgulData.ssps.toggleDrawGateways();
	redraw();
}

void GLWidget::toggleSwapYZ()
{
	cgulData.ssps.swapYZ = 1 - cgulData.ssps.swapYZ;
	redraw();
}

void GLWidget::toggleFlipX()
{
	cgulData.ssps.flipX = 1 - cgulData.ssps.flipX;
	redraw();
}

void GLWidget::setWorldScale(float scale)
{
	cgulData.worldScale = scale;
	redraw();
}

void GLWidget::resetCamera()
{
	cam.init(FIRST_PERSON,-100,100,200);
	redraw();
}


int GLWidget::handle( int event )
{
	// The handle() function, reimplemented from
	// Fl_Gl_Window, notifies us of any user input
	// in our OpenGL drawing area. There are various
	// Fl::event_ functions to get event parameters like
	// which button or key pressed, mouse x and y
	// position, etc.
	float movement=10;
		
	switch( event )
	{
		// Mouse button pressed
		case FL_PUSH:
			if (Fl::event_button3() || ((Fl::get_key(FL_Shift_L) || Fl::get_key(FL_Shift_R)) && Fl::event_button1()))
			{
				//right button or shift+click(zoom mode)
				cameraMode=CAM_ZOOM;
				lastX= Fl::event_x();
				lastY= Fl::event_y();
			}
			else if ( Fl::event_button2() || ((Fl::get_key(FL_Control_L) || Fl::get_key(FL_Control_R)) && Fl::event_button1()))
			{
				//middle button or ctrl+click (pan mode)
				cameraMode=CAM_PAN;
				lastX= Fl::event_x();
				lastY= Fl::event_y();
			}
			else if( Fl::event_button1() )
			{
				//left button (rotate mode)
				cameraMode=CAM_ROTATE;
				lastX= Fl::event_x();
				lastY= Fl::event_y();
			}
			// Must return 1 to let FLTK know we handled the
			// event. Otherwise, it may get sent to another
			// widget.
			return 1;
			//Mouse button is released
		case FL_RELEASE:
			cameraMode=CAM_NONE;
			return 1;
			// Called repeatedly while there is mouse movement
			// with a button down
		case FL_DRAG:
			switch (cameraMode)
			{
				case CAM_PAN:
					cam.move(-(Fl::event_x() - lastX)*2, (Fl::event_y() - lastY)*2);
					lastX = Fl::event_x();
					lastY = Fl::event_y();
					break;
				case CAM_ZOOM:
					cam.move((Fl::event_y() - lastY));
					lastX = Fl::event_x();
					lastY = Fl::event_y();
					break;
				case CAM_ROTATE:
					cam.rotate(-(Fl::event_y() - lastY)*0.05, -(Fl::event_x() - lastX)*0.05);
					lastX = Fl::event_x();
					lastY = Fl::event_y();
					break;
				case CAM_NONE:
					break;
			}
			redraw();
			return 1;
		case FL_MOUSEWHEEL:
			//the mouse wheel was scrolled
			cam.move(Fl::event_dy()*0.5);
			redraw();
			return 1;
			
		case FL_KEYUP:
			string key = Fl::event_text();
			if (key.length() > 0)
			{
				if (key[0] == 'c')
				{
					cout << "Camera position: " << cam.cx << "," << cam.cy << "," << cam.cz << endl;
					cout << "Reference point: " << cam.rx << "," << cam.ry << "," << cam.rz << endl;
				}
				switch(key[0])
				{
					case '0':
						cam.position(-271.627,169.64,-120.834);
						cam.setReferencePoint(-156.772,82.6463,17.8758);
						break;
					case '1':
						cam.position(107.317,166.791,-138.897);
						cam.setReferencePoint(-5.39524,88.9065,6.80833);
						break;
					case '2':
						cam.position(139.42,224.441,303.338);
						cam.setReferencePoint(21.2121,95.5968,206.25);
						break;
					case '3':
						cam.position(-260.725,205.932,307.388);
						cam.setReferencePoint(-160.227,77.0886,192.065);
						break;

					case 'w':
						zoomAmount = 0;
						break;
					case 's':
						zoomAmount = 0;
						break;
					case 'a':
						horizPanAmount = 0;
						break;
					case 'd':
						horizPanAmount = 0;
						break;
					case 'A':
						vertPanAmount = 0;
						break;
					case 'D':
						vertPanAmount = 0;
						break;
				}
				redraw();
				return 1;
			}
  }

  // Send any event we didn't use to the default handler
  return Fl_Gl_Window::handle( event );
}

void GLWidget::setGLView()
{
	// Fl::event_x() and Fl::event_y() return mouse position
	// in pixels from top-left of drawing area. So we'll set
	// GL's window extents to match:
	// x = 0 (left) to 700 (right), y = 500 (bottom) to 0 (top)
	
	//For 3D
    //glDrawBuffer(GL_AUX0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,width,height);
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f, 100000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//cam.U.print();
	gluLookAt(cam.cx,cam.cy,cam.cz,
			  cam.cx + cam.N.x,cam.cy + cam.N.y,cam.cz + cam.N.z,
			  cam.U.x,cam.U.y,cam.U.z);
	
	if (lightEnabled)
    {
        //setup all lighting information
        //float midpoint = (max/2.0) * 0.1;
        float lightpos[]= {cam.cx + cam.N.x,cam.cy + cam.N.y,cam.cz + cam.N.z, 1.0f};
		//cam.N.x, cam.N.y, cam.N.z, 0.0f};//midpoint,10,midpoint,1.0f};
        float MatAmb[] = { 0.2,0.2,0.2,1};
        float MatSpec[] = {0,0,0,1};
		float MatDiff[] = {0.2,0.2,0.2,1};
        float MatShininess[] = {1000.0f};
		
        float LightAmb[] = { 0.5,0.5,0.5, 1 };
        float LightSpec[] = {0.1,0.1,0.1,1};
		float LightDiff[] = {0.3,0.3,0.3,1};
        
        glShadeModel(GL_SMOOTH);
        
        glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpec);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiff);
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);	
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
}

void GLWidget::drawSphere(float x1, float y1, float z1, float radius, bool filled)
{
	GLUquadric *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	if (filled)
	{
		gluQuadricDrawStyle( quadric, GLU_FILL );
		glPushMatrix();
		glTranslatef(x1,y1,z1);
		gluSphere(quadric,radius,6,6);
		glPopMatrix();
	}
	else
	{
		gluQuadricDrawStyle( quadric, GLU_LINE );
		glPushMatrix();
		glTranslatef(x1,y1,z1);
		gluSphere(quadric,radius,6,6);
		glPopMatrix();
	}
	gluDeleteQuadric(quadric);
}

mVector GLWidget::getVectorByAngles(float azimuth, float zenith, float length)
{
    //If flipped over 180 degrees
    //... turning right
    if (zenith > 2.0f * M_PI)
    {
        zenith -= (2.0f * M_PI);
    }
    
    //If turned around 360 degrees 
    //... turning right
    if (azimuth > 2 * M_PI)
    {
        azimuth -= (2.0f * M_PI);
    }
    
    //If flipped over 180 degrees
    //... turning left
    if (zenith < 0.0f)
    {
        zenith += (2.0f * M_PI);
    }
    
    //If turned around 360 degrees 
    //... turning left
    if (azimuth < 0.0f)
    {
        azimuth += (2.0f * M_PI);
    }
	mVector result;
	result.x = sin(zenith) * cos(azimuth);
	result.y = cos(zenith);
	result.z = sin(zenith) * sin(azimuth);
    
    result.x = length * result.x;
    result.y = length * result.y;
    result.z = length * result.z;
	
	return result;
}

void GLWidget::draw()
{
    // All drawing code goes in the draw() function, which is
    // reimplemented from Fl_Gl_Window.
    //
    // NOTE: draw() should only be called by the system. To
    // explicitly redraw the screen in your code, call
    // redraw(). This doesn't swap buffers.

    if( !valid() )
    {
      // If our window has been invalidated (for example
      // minimized then restored), we'll have to reset
      // the window parameters.
      setGLView();
    }
	switch(cameraAngle)
	{
		case -1:
			break;
		case 0:
			cam.position(-271.627,169.64,-120.834);
			cam.setReferencePoint(-156.772,82.6463,17.8758);
			break;
		case 1:
			cam.position(107.317,166.791,-138.897);
			cam.setReferencePoint(-5.39524,88.9065,6.80833);
			break;
		case 2:
			cam.position(139.42,224.441,303.338);
			cam.setReferencePoint(21.2121,95.5968,206.25);
			break;
		case 3:
			cam.position(-260.725,205.932,307.388);
			cam.setReferencePoint(-160.227,77.0886,192.065);
			break;
	}
	setGLView();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	
	
	if (showAxes)
	{
		//draw the axes for point of reference
		glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(-500,0,0);
		glColor3f(1,1,1);
		glVertex3f(500,0,0);
		
		glColor3f(0,1,0);
		glVertex3f(0,-500,0);
		glColor3f(1,1,1);
		glVertex3f(0,500,0);
		
		glColor3f(0,0,1);
		glVertex3f(0,0,-500);
		glColor3f(1,1,1);
		glVertex3f(0,0,500);
		glEnd();
	}
	
	//
	// REMEMBER: i=row(y) and j=col(x)

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	glPushMatrix();
	//scale all
	glScalef(cgulData.worldScale,cgulData.worldScale,cgulData.worldScale);
	float signX = ((cgulData.ssps.flipX)?-1:1);
	
	float alpha = 1;
	for (unsigned int p=0; p < cgulData.playerTraces.size(); p++) 
	{

		for (unsigned int i=1; i < cgulData.playerTraces[p].size(); i++) 
		{
			float playerTraceY;
			float playerTraceZ;
			if (cgulData.ssps.swapYZ)
			{
				playerTraceY = cgulData.playerTraces[p][i].z;
				playerTraceZ = cgulData.playerTraces[p][i].y;
			}
			else
			{
				playerTraceY = cgulData.playerTraces[p][i].y;
				playerTraceZ = cgulData.playerTraces[p][i].z;
			}
			float ratio = (1.0 * i) / cgulData.playerTraces[p].size();
			RGBColor color = colorMap.getColor(ratio);
			glLineWidth(2);
			
			if (playerTraceBrowser->selected(p+1) || cameraAngle != -1)
				alpha = 1.0;
			else
				alpha=0.1;
			glColor4f(color.R,color.G,color.B,alpha);
			glBegin(GL_LINES);
			if (cgulData.ssps.swapYZ)
			{
				glVertex3f(signX * cgulData.playerTraces[p][i-1].x, cgulData.playerTraces[p][i-1].z, cgulData.playerTraces[p][i-1].y);
			}
			else
			{
				glVertex3f(signX * cgulData.playerTraces[p][i-1].x, cgulData.playerTraces[p][i-1].y, cgulData.playerTraces[p][i-1].z);
			}
			glVertex3f(signX * cgulData.playerTraces[p][i].x, playerTraceY, playerTraceZ);
			glEnd();
			
			glColor4f(color.R,color.G,color.B,alpha);
    		drawSphere(signX * cgulData.playerTraces[p][i].x,playerTraceY,playerTraceZ, playerCircleRadius, true);
			//draw line showing rotation
			//pointing shows yaw
			//color shows pitch (up:white, down:black)
			//size of stick changes with pitch
			//assumptions: 
			//	negative pitch means looking down, 0 means looking straight
			float azimuth = -((cgulData.playerTraces[p][i].h+180) * M_PI) / 180.0f;
			float zenith = ((90 + cgulData.playerTraces[p][i].p) * M_PI) / 180.0f;
			mVector stick = getVectorByAngles(azimuth, zenith, 1.0f);
			
			Camera view(FIRST_PERSON, signX * cgulData.playerTraces[p][i].x, playerTraceY, playerTraceZ);
			view.calculateVectors(mVector(signX * cgulData.playerTraces[p][i].x, playerTraceY, playerTraceZ),
								  mVector(signX * cgulData.playerTraces[p][i].x + stick.x,
										  playerTraceY + stick.y,
										  playerTraceZ + stick.z));

			glLineWidth(1);
			glBegin(GL_LINES);
			
			glColor4f(0,0,0,alpha);
			glVertex3f(view.cx, view.cy, view.cz);
			glColor4f(1,1,1,alpha);
			glVertex3f(view.cx + view.N.x * yawStickSize, 
					   view.cy + view.N.y * yawStickSize, 
					   view.cz + view.N.z * yawStickSize);
			glEnd();
			glLineWidth(1);	
			//draw circle at end of stick if shot was fired
			if (i > 0)
			{
				if (cgulData.playerTraces[p][i].shots != cgulData.playerTraces[p][i-1].shots)
				{
					glColor4f(1,0,0,alpha);
					drawSphere(view.cx + view.N.x * yawStickSize * 0.5f, 
							   view.cy + view.N.y * yawStickSize * 0.5f, 
							   view.cz + view.N.z * yawStickSize * 0.5f,
							   playerCircleRadius * 0.8f,false);
				}
			}
			
			//draw a circle if health was lost
			if (i > 0)
			{
				if (cgulData.playerTraces[p][i].health != cgulData.playerTraces[p][i-1].health)
				{
					glColor4f(1,0,0,alpha);
					drawSphere(signX * cgulData.playerTraces[p][i].x,playerTraceY,playerTraceZ, playerCircleRadius+1, false);
				}
			}
			
			//draw a white circle if captured
			if (i == cgulData.playerTraces[p].size()-1 && cgulData.capturedFlag[p]==true)
			{
				glColor4f(1,1,1,alpha);
				drawSphere(signX * cgulData.playerTraces[p][i].x,playerTraceY,playerTraceZ, playerCircleRadius+2, false);
			}
		}
	}
		
	
	if (cgulData.currentMap != "")
	{
		//Draw the flag position
		glColor4f(1,0,0,0.5);
		if (cgulData.ssps.swapYZ)
		{
			drawSphere(signX * cgulData.flagPosition.x,cgulData.flagPosition.z,cgulData.flagPosition.y,playerCircleRadius*2,true);
		}
		else
		{
			drawSphere(signX * cgulData.flagPosition.x,cgulData.flagPosition.y,cgulData.flagPosition.z,playerCircleRadius*2,true);
		}
	}

	//draw the geometry
	cgulData.ssps.render();
	
	glDisable(GL_BLEND);
	glPopMatrix();
	
	if (cameraAngle > -1)
	{
		if (playerTraceBrowser->size() > 0)
		{
			cout << "Before dump#" << cameraAngle << endl;
			if (!WindowDump())
				cout << "... finished but FAILED" << endl;
			else
				cout << "Image dumped." << endl;
			cameraAngle++;
			if (cameraAngle > 3)
				exit(0);
			draw();
		}
	}
}

void GLWidget::setCameraAngle(int c)
{
	cameraAngle=c;
}

void GLWidget::setImageName(string name)
{
	cgulData.imagePath = name;
}

//****** Calls CGUL data functions
void GLWidget::readDataFile(const char* filename, const char* sspspath, const char* diaspath)
{
	if (filename != NULL)
    {
        string fname=string(filename);
        if (fname.length() < 5 || fname.substr(fname.length() - 5, 5) != ".data")
        {
            //Its a list of files
            ifstream ifs(filename);
            string str;
            while (!ifs.eof())
            {
                getline(ifs,str);
                readPlayerTraceFile(str.c_str());
            }
            ifs.close();
        }
        else
        {
            readPlayerTraceFile(filename);
        }
    }
    if (sspspath != NULL)
    {
        readSSPSFile(sspspath);
    }
	if (diaspath != NULL)
	{
		readDiasData(diaspath);
	}
	redraw();
}

void GLWidget::readPlayerTraceFile(const char* filename)
{
	
	string fname = string(filename);
	if (fname.length() == 0)
	{
	    return;
    }
	string levelNum = fname.substr(fname.length()-9,1);
	int slashPos = fname.rfind("/");
	if (slashPos == string::npos)
	{
		slashPos = fname.rfind("\\");
		slashPos = fname.rfind("\\",slashPos-1);
	}
	else
	{
		slashPos = fname.rfind("/",slashPos-1);
	}
	string pathString = fname.substr(slashPos+1,fname.length()-slashPos-1);
	
	//read the file
	cgulData.readPlayerTraceFile(filename);

	
	playerTraceBrowser->add(pathString.c_str());
	playerTraceBrowser->select(playerTraceBrowser->size(),1);
	playerTraceBrowser->redraw();
}

void GLWidget::readSSPSFile(const char* sspspath)
{
	cgulData.readSSPSFile(sspspath);
	lblCurrentMap->value(cgulData.currentMap.c_str());
	lblCurrentMap->redraw();
}

void GLWidget::readDiasData(string filename)
{
	cgulData.readDiasData(filename);
}


void GLWidget::clearData()
{
	cgulData.clearData();
	playerTraceBrowser->clear();
	redraw();
}

void GLWidget::removeSelectedTraces()
{	
	for (int p=0; p < cgulData.playerTraces.size(); p++) 
	{
		if (playerTraceBrowser->selected(p+1))
		{
			if (cgulData.removeTrace(p))
			{
				playerTraceBrowser->remove(p+1);
				p--;
			}
		}
	}
	redraw();
}

void GLWidget::setDataDirectory(string dir)
{
	cgulData.setDataDirectory(dir);
}


/*
 The following function borrowed from: 
 http://local.wasp.uwa.edu.au/~pbourke/modelling_rendering/windowdump/
 I had to change the C code to C++ code but the logic is all theirs
 -Priyesh
 
 Original comment:
 Write the current view to a file
 The multiple fputc()s can be replaced with
 fwrite(image,width*height*3,1,fptr);
 If the memory pixel order is the same as the destination file format.
 */
int GLWidget::WindowDump()
{
	int i,j;
	ofstream ofs;
	string fname;
	ostringstream oss;
	unsigned char *image = new unsigned char[3*WIDTH*HEIGHT];
	
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	
	/* Open the file */
	oss << cgulData.imagePath << "-" << cameraAngle << ".pnm";
	fname= oss.str();
	
	//sprintf(fname,"%s-%d.pnm",imagePath.c_str(),cameraAngle);//
	cout << "opening " << fname << endl;
	ofs.open(fname.c_str());
	cout << "after opening...." << endl;
	if (!ofs) {
		cerr << "Failed to open file for window dump\n";
		return(false);
	}
	
	/* Copy the image into our buffer */
	glReadPixels(0,0,WIDTH,HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,image);
	
	/* Write the raw file */
	ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
	for (j=HEIGHT-1;j>=0;j--) {
		for (i=0;i<WIDTH;i++) {
			ofs << (image[3*j*WIDTH+3*i+0]);
			ofs << (image[3*j*WIDTH+3*i+1]);
			ofs << (image[3*j*WIDTH+3*i+2]);
		}
	}
	ofs.close();
	
	/* Clean up */
	delete image;
	return(true);
}



