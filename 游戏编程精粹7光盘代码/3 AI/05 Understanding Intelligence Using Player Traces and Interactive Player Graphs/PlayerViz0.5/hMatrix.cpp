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

#include "hMatrix.h"

Quaternion::Quaternion()
{
    w=1.0f;
    x=y=z=0.0f;
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

Quaternion::Quaternion(float angle, mVector axis)
{
    w = cos(angle/2.0f);
    x = axis.x * sin(angle/2.0f);
    y = axis.y * sin(angle/2.0f);
    z = axis.z * sin(angle/2.0f);
}

float Quaternion::magnitude()
{
    return sqrt(w*w + x*x + y*y + z*z);
}

void Quaternion::normalizeMe()
{
    float m = magnitude();
    w /= m;
    x /= m;
    y /= m;
    z /= m;
}

Quaternion Quaternion::operator * (const Quaternion &rhs)const
{
    Quaternion temp;
    temp.w = w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z;
    temp.x = w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y;
    temp.y = w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x;
    temp.z = w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w;
    return temp;
}

////////////////////////////////////////////////////////

hMatrix::hMatrix(int s) {
    size=s;
    M=new float *[size];
    for (int i=0; i < size; i++)
        M[i]=new float[size];
    setIdentity();
}

hMatrix::hMatrix() {
    size=3;
    M=new float *[size];
    for (int i=0; i < size; i++)
        M[i]=new float[size];
    setIdentity();
}

hMatrix::~hMatrix() {
    for (int i=0; i < size; i++)
        delete [] M[i];
    delete [] M;
}

void hMatrix::setIdentity() {
    for (int i=0; i < size; i++) {
        for(int j=0; j < size; j++) {
            if (i==j)
               M[i][j]=1.0;
            else
               M[i][j]=0.0;
        }
    }
}
//// Transformations
//   all transforms are premultiplied by default

// invert (shortcut, does NOT deal with rotations and shears)
// Just negates any translations and reciprocates scaling.
//   useful for coordinate system transforms that only have
//   translate and scaling.
hMatrix hMatrix::invert() {
    hMatrix N(size);
    //invert translations
    N[0][size-1]=-M[0][size-1]*(1.0/M[0][0]);
    N[1][size-1]=-M[1][size-1]*(1.0/M[1][1]);
    if (size==4) N[2][size-1]=-M[2][size-1]*(1.0/M[2][2]);
    //reciprocate scaling
    N[0][0]=1.0/M[0][0];
    N[1][1]=1.0/M[1][1];
    if (size==4) N[2][2]=1.0/M[2][2];
    return N;
}

void hMatrix::translate(float Tx, float Ty, float Tz) {
    hMatrix N(size);
    N[0][size-1]=Tx;
    N[1][size-1]=Ty;
    if (size==4) N[2][size-1]=Tz;
    *this=N * (*this);
}

void hMatrix::scale(float Sx, float Sy, float Sz) {
    hMatrix N(size);
    N[0][0]=Sx;
    N[1][1]=Sy;
    if (size==4) N[2][2]=Sz;
    *this=N * (*this);
}

void hMatrix::rotate(float phi) {
    hMatrix N(size);
    N[0][0]=cos(phi); N[0][1]=-sin(phi);
    N[1][0]=sin(phi); N[1][1]=cos(phi);
    *this=N * (*this);
}

void hMatrix::rotate(float phi, int axis) {
    hMatrix N(size);
    switch (axis) {
        case 0:// X-axis
            N[1][1]=cos(phi); N[1][2]=-sin(phi);
            N[2][1]=sin(phi); N[2][2]=cos(phi);
            break;
        case 1:// Y-axis
            N[0][0]=cos(phi); N[0][2]=-sin(phi);
            N[2][0]=sin(phi); N[2][2]=cos(phi);
            break;
        case 2:// Z-axis
            N[0][0]=cos(phi); N[0][1]=-sin(phi);
            N[1][0]=sin(phi); N[1][1]=cos(phi);
            break;
    }
    *this=N * (*this);
}

void hMatrix::rotateByQuaternion(float angle, mVector axis)
{
    hMatrix N(4);
    Quaternion origin;
    Quaternion rotation(angle,axis);
    rotation = rotation*origin;
    float w = rotation.w;
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    
    //first row
    N[0][0] = w*w + x*x - y*y - z*z;
    N[0][1] = 2*x*y - 2*w*x;
    N[0][2] = 2*x*z + 2*w*y;
    N[0][3] = 0;
    //second row
    N[1][0] = 2*x*y + 2*w*z;
    N[1][1] = w*w - x*x + y*y - z*z;
    N[1][2] = 2*y*z - 2*w*z;
    N[1][3] = 0;
    //third row
    N[2][0] = 2*x*z - 2*w*y;
    N[2][1] = 2*y*z - 2*w*z;
    N[2][2] = w*w - x*x - y*y + z*z;
    N[2][3] = 0;
    //fourth row
    N[3][0] = 0;
    N[3][1] = 0;
    N[3][2] = 0;
    N[3][3] = 1;
    *this = N * (*this);
}

void hMatrix::rotateBetweenVectors(mVector s, mVector t)
{
    s = s.normalize();
    t = t.normalize();
    mVector v = s.cross(t);
    float e = s*t;
    float h = 1.0 / (1+e);
    hMatrix N(4);
    
    //first row
    N[0][0] = e + h*v.x*v.x;
    N[0][1] = h*v.x*v.y - v.z;
    N[0][2] = h*v.x*v.z + v.y;
    N[0][3] = 0;
    //second row
    N[1][0] = h*v.x*v.y + v.z;
    N[1][1] = e + h*v.y*v.y;
    N[1][2] = h*v.y*v.z - v.x; 
    N[1][3] = 0;
    //third row
    N[2][0] = h*v.x*v.z - v.y;
    N[2][1] = h*v.y*v.z + v.x;
    N[2][2] = e + h*v.z*v.z;
    N[2][3] = 0;
    //fourth row
    N[3][0] = 0;
    N[3][1] = 0;
    N[3][2] = 0;
    N[3][3] = 1;
    
    multiplyTo(N);
}


void hMatrix::shear(float Sx, float Sy, float xr, float yr) {
    hMatrix N(size);
    N[0][1]=Sx;//assuming ref point is 0,0
    N[1][0]=Sy;
    N[0][size-1]=-Sx*yr;
    N[1][size-1]=-Sy*xr;
    *this=N * (*this);
}
//// Operator overloads

//Indexing operator
float* hMatrix::operator [](int row)const {
    return M[row];
}

//Equal operator
void hMatrix::operator =(const hMatrix rhs) {
    if (rhs.length() != size) {
        this->~hMatrix();
        size=rhs.length();
        M=new float *[size];
        for (int i=0; i < size; i++)
            M[i]=new float[size];
    }
    for (int r=0; r < size; r++) {
        for (int c=0; c < size; c++)
		{
            M[r][c]=rhs[r][c];
		}
    }
}


//Addition operator
hMatrix hMatrix::operator +(const hMatrix &rhs)const {
    hMatrix temp(size);
    for (int r=0; r < size; r++) {
        for (int c=0; c < size; c++)
            temp[r][c]=M[r][c]+rhs[r][c];
    }
    return temp;
}

void hMatrix::multiplyTo(const hMatrix &lhs) {
	hMatrix temp(size);
    float sum;
    //main loop is which row we are on in M
    for (int r=0; r < size; r++) {
        for (int i=0; i < size; i++) {
            sum=0;
            for (int c=0; c < size; c++)
                sum+=lhs[r][c]*M[c][i];
            temp[r][i]=sum;
        }
    }
    //Set it equal to the temp
	for (int r=0; r < size; r++) {
        for (int c=0; c < size; c++)
		{
            M[r][c]=temp[r][c];
		}
	}
}

//Multiplication operator for: Matrix * Matrix
hMatrix hMatrix::operator *(const hMatrix &rhs)const {
    hMatrix temp(size);
    float sum;
    //main loop is which row we are on in M
    for (int r=0; r < size; r++) {
        for (int i=0; i < size; i++) {
            sum=0;
            for (int c=0; c < size; c++)
                sum+=M[r][c]*rhs[c][i];
            temp[r][i]=sum;
        }
    }
    return temp;
}

//Multiplication operator for: Matrix * Vector
std::vector<float> hMatrix::operator *(const std::vector<float> &rhs)const {
    std::vector<float> v;
    v.resize(size);
    float sum=0;
    for (int r=0; r < size; r++) {
        sum=0;
        for (int i=0; i < size; i++) {
            sum+=M[r][i]*rhs[i];
        }
        v[r]=sum;
    }
    return v;
}

//Multiplication operator for: Matrix * mVector
mVector hMatrix::operator *(const mVector &rhs)const {
    mVector v;
    float sum=0;
    for (int r=0; r < size; r++) {
        sum=0;
        sum+=M[r][0] * rhs.x;
        sum+=M[r][1] * rhs.y;
        if (size == 3)
           sum+=M[r][2];
        else if (size==4) {
           sum+=M[r][2] * rhs.z;
           sum+=M[r][3];
        }
        switch (r) {
          case 0:v.x=sum;break;
          case 1:v.y=sum;break;
          case 2:v.z=sum;break;
        }
    }
    return v;
}

//Print out contents of Matrix
void hMatrix::printMatrix() {
    for (int r=0; r < size; r++) {
        std::cout << "[";
        for (int c=0; c < size; c++) {
            std::cout << M[r][c];
            if (c==size-1)
               std::cout << "]";
            else
               std::cout << ",";
        }
        std::cout << "\n";
    }
}

