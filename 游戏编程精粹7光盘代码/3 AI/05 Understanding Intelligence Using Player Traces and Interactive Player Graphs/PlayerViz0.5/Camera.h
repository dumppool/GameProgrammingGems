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

#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679
#endif


#define MAX_DIST 20.0
#define MIN_DIST 10.0

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

enum CAM_MODE { FIRST_PERSON, THIRD_PERSON, MODEL_VIEW, NONE };

class mVector {
public:
    float x,y,z;
    float azim,colat;
    //functions
    mVector();
    mVector(float xx,float yy,float zz) { set(xx,yy,zz); }
    float length();
    void print();
	
	float GetX() { return x; }
	float GetY() { return y; }
	float GetZ() { return z; }
    void set(float xx,float yy,float zz);
    void setA(float a,float c,float d);
    //operators
    void rotate(float da,float dc);
    mVector normalize();
	void setRange01();
    mVector cross(const mVector &rhs)const;//cross-product
	float operator * (const mVector &rhs)const;//dot-product
		
	mVector operator * (float num)const;
	mVector operator + (const mVector &rhs)const;
	mVector operator - (const mVector &rhs)const;
	mVector operator - ()const;
	mVector operator / (float num)const;
	float distanceTo(mVector point);
};

class Camera {
private:
	bool yMove;
	CAM_MODE mode;
 	
    public:
    mVector U,V,N;
   
    float cx,cy,cz;//camera position
	float rx,ry,rz;//reference point
	
	Camera();
	Camera(CAM_MODE,float,float,float);
	~Camera();
	
	void init(CAM_MODE,float,float,float);
	
    void position(float x,float y,float z);

    void setReferencePoint(float x,float y,float z);

    void calculateVectors(mVector pos, mVector ref);
	//rotate
    void rotate(float phiX, float phiY);
	
    void move(float dir);
  
    void move(float xd, float yd);
    
    float distance() {
        return N.length();
    }
	
	void setThird(float px, float py, float pz);
	
};

#endif
