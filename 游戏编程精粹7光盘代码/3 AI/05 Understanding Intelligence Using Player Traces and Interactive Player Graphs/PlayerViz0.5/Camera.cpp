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

#include "Camera.h"
#include <iostream>
using namespace std;

/////////////mVector///////////////////////////////////
mVector::mVector() {
    x=0; y=0; z=0;
    azim=0; colat=0;
}

float mVector::length() {
    return sqrt(x*x+y*y+z*z);
}

void mVector::print() {
    std::cout << "[" << x << "," << y << "," << z << "]\n";
}

void mVector::set(float xx,float yy,float zz) {
    x=xx; y=yy; z=zz;
    mVector temp=*this;
    temp.normalize();
    colat=acos(temp.z);
    if (sin(colat) == 0 && x != 0)
        azim=atan(y/x);
    else if (sin(colat) != 0 && x == 0)
        azim=asin(temp.y/sin(colat));
    else
        azim=0;
}

void mVector::setA(float a, float c, float d) {
    //a=azimuth
    //c=co-latitude
    //if (length() > d)
    //    d=length();
    if (c > 2*M_PI) c-=2*M_PI;
    if (a > 2*M_PI) a-=2*M_PI;
    if (c < 0) c+=2*M_PI;
    if (a < 0) a+=2*M_PI;
    x=d*sin(c)*cos(a);
    y=d*sin(a);
    z=d*cos(a)*cos(c);
    azim=a;
    colat=c;
}

void mVector::rotate(float da,float dc) {
    setA(azim+da,colat+dc,length());
}

mVector mVector::normalize() {
    float s=length();
    mVector temp;
    temp.x=x/s;
    temp.y=y/s;
    temp.z=z/s;
    return temp;
}

void mVector::setRange01() {
	mVector temp=normalize();
	temp= temp*0.5 + mVector(0.5,0.5,0.5);
	set(temp.x,temp.y,temp.z);
}

mVector mVector::cross(const mVector &rhs)const {//cross-product
    mVector temp;
    float cx=y*rhs.z - z*rhs.y;
    float cy=z*rhs.x - x*rhs.z;
    float cz=x*rhs.y - y*rhs.x;
    temp.set(cx,cy,cz);
    return temp;
}

float mVector::operator * (const mVector &rhs)const {//dot-product
    return (x*rhs.x + y*rhs.y + z*rhs.z);
}

mVector mVector::operator + (const mVector &rhs)const {
	mVector temp(x+rhs.x,y+rhs.y,z+rhs.z);
	return temp;
}

mVector mVector::operator - (const mVector &rhs)const {
	mVector temp(x-rhs.x,y-rhs.y,z-rhs.z);
	return temp;
}

mVector mVector::operator - ()const {
	mVector temp(-x,-y,-z);
	return temp;
}

mVector mVector::operator * (float num)const {
	return mVector(x*num, y*num, z*num);
}

mVector mVector::operator / (float num)const {
	return mVector(x/num,y/num,z/num);
}

float mVector::distanceTo(mVector point)
{
	return sqrt((point.x-x)*(point.x-x) + (point.y-y)*(point.y-y) + (point.z-z)*(point.z-z));
}


////////////////////////////////////////////////////////

Camera::Camera() {
	mode=NONE;
	yMove=true;
}

Camera::Camera(CAM_MODE m, float x, float y, float z) {
	init(m,x,y,z);
}

void Camera::init(CAM_MODE m, float x, float y, float z) {
	mode=m;
	yMove=true;
	U.setA(M_PI_2,M_PI,1.0);//010
	V.setA(0,M_PI_2,1.0);//100
	N.setA(0,M_PI,1.0);//00-1

	if (mode== THIRD_PERSON) {
		position(x,y,z);
		setThird(x-1,y-1,z-1);
	}
	else 
    {
		position(x,y,z);
		setReferencePoint(x,y,z-1);//look down the Z by default
	}
}

Camera::~Camera() {
}

void Camera::position(float x,float y,float z) {
	cx=x;
	cy=y;
	cz=z;
}

void Camera::setReferencePoint(float x,float y,float z) {
	rx=x; ry=y; rz=z;
    N.x = rx-cx;
    N.y = ry-cy;
    N.z = rz-cz;
}

void Camera::calculateVectors(mVector pos, mVector ref)
{
    cx=pos.x;
    cy=pos.y;
    cz=pos.z;
    //ref point
    rx=ref.x;
    ry=ref.y;
    rz=ref.z;
    
    N = ref - pos;
    N=N.normalize();
    U.set(0,1,0);
    V = N.cross(U);
    U = V.cross(N);
}

//rotate the camera
void Camera::rotate(float phiX, float phiY) {
	U.rotate(phiX,phiY);
	V.rotate(0,phiY);
	N.rotate(phiX,phiY);
	//move camera
	//if in first person, turn camera
	if (mode==FIRST_PERSON) {
		rx=cx+N.x;
		ry=cy+N.y;
		rz=cz+N.z;
	} else {//else move camera
		cx=rx-N.x;
		cy=ry-N.y;
		cz=rz-N.z;
	}
}

//move forward/backward
void Camera::move(float dir) {
	mVector Nn=N.normalize();
	cx+=dir*Nn.x;
	cy+=dir*Nn.y;
	cz+=dir*Nn.z;
	if (mode != FIRST_PERSON)
		setReferencePoint(rx,ry,rz);
	else
		setReferencePoint(rx+(dir*Nn.x),ry+(dir*Nn.y),rz+(dir*Nn.z));
}

//panning the view
void Camera::move(float xd, float yd) {
	cx+=U.x*yd + V.x*xd;
	cy+=U.y*yd + V.y*xd;
	cz+=U.z*yd + V.z*xd;
//move reference point
	rx+=U.x*yd + V.x*xd;
	ry+=U.y*yd + V.y*xd;
	rz+=U.z*yd + V.z*xd;
}


void Camera::setThird(float px,float py,float pz) {
	float dx,dy,dz,d;
	mVector Nn=N.normalize();
	setReferencePoint(px,py,pz);
	dx=px - cx;
	dy=py - cy;
	dz=pz - cz;
	d=sqrt(dx*dx+dy*dy+dz*dz);
	if (d > MAX_DIST) {
		cx+=Nn.x;
		cz+=Nn.z;
	}
	else if (d < MIN_DIST) {
		cx-=Nn.x;
		cz-=Nn.z;
	}
}
