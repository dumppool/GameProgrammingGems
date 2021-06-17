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

#ifndef HMATRIX_H
#define HMATRIX_H

#include <vector>
#include <iostream>
#include <math.h>
#include "Camera.h"

#define X_AXIS_ROT 0
#define Y_AXIS_ROT 1
#define Z_AXIS_ROT 2

class Quaternion {
public:
    float w;
    float x;
    float y;
    float z;
    
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float angle, mVector axis);
    float magnitude();
    void normalizeMe();
    Quaternion operator * (const Quaternion &rhs)const;
};

class hMatrix {
    private:
    float** M;
    int size;
    
    public:
    hMatrix(int);
    hMatrix();
    ~hMatrix();
    
    int length()const { return size; }
    void printMatrix();
    void setIdentity();
    //transformations
    hMatrix invert();
    void rotate(float);
    void rotate(float,int);//3D version
    void rotateByQuaternion(float, mVector);
    void rotateBetweenVectors(mVector s, mVector t);
    void translate(float,float,float);
    void scale(float,float,float);
    void shear(float,float,float,float);
    //operators!
    void operator = (const hMatrix rhs);
	void multiplyTo(const hMatrix &lhs);
    hMatrix operator * (const hMatrix &rhs)const;
    std::vector<float> operator * (const std::vector<float> &rhs)const;
    mVector operator * (const mVector &rhs)const;
    hMatrix operator + (const hMatrix &rhs)const;

    float* operator [](int row)const;
};

#endif
