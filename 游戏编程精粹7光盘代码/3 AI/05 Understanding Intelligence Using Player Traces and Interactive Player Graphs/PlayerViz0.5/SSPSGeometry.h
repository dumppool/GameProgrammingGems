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

#ifndef SSPS_GEOMETRY
#define SSPS_GEOMETRY

#include <vector>
#include <string>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Camera.h"
#include "hMatrix.h"

#ifndef isnan
#define isnan(x) ((x) != (x))
#endif

#define ABS(x) (((x)<0)?(-(x)):(x))
#define FLOATSEQUAL(a,b) ((ABS((a)-(b)))<=(0.0000001)?(1):(0))

const float BASE_INCREMENT = 0.5f;

struct ViewFrustum
{
    Camera eyePoint;
    float distance;
    float angle;
    bool intersected;
};

struct SSPSTexCoord
{
	int u;
	int v;
	SSPSTexCoord()
	{
		u=0;
		v=0;
	}
	SSPSTexCoord(int U, int V)
	{
		u=U;
		v=V;
	}
};

struct SSPSVertex
{
	float x;
	float y;
	float z;
    
	SSPSVertex()
	{
	}
	
	SSPSVertex(float X, float Y, float Z)
	{
		x=X;
		y=Y;
		z=Z;
	}
    
    void setByVector(mVector v)
    {
        x=v.x;
        y=v.y;
        z=v.z;
    }
};

struct BoundingQuad
{
    SSPSVertex minPoint;
    SSPSVertex maxPoint;
    
    void initialize()
    {
        minPoint = SSPSVertex(9999,9999,9999);
        maxPoint = SSPSVertex(-9999,-9999,-9999);
    }
};


struct Plane {
	mVector center;
	mVector N;
    
	Plane(){}
	Plane(mVector c, mVector n)
	{
		N=n;
		center=c;
	}

    float distanceToPoint(mVector point)
    {
        return (point * N)/N.length();
    }
    
    float getD()
    {
        return (N*center);
    }
};

class SSPSFace
{
public:
	int ID;
	SSPSVertex normal;
	std::vector<SSPSVertex> vertices;
    std::vector<SSPSVertex> texCoords;
    BoundingQuad boundQuad;
    Plane plane;
    
    GLubyte* texture;
    bool* textureUpdated;
    GLuint texID;
    int textureWidth;
    int textureHeight;
    float totalValue;
    bool selected;
    
    SSPSFace();
    
    ~SSPSFace();
    
    void initialize();
    void deleteTexture();
    void initTexture();
    void clearTexture();
    void clearUpdateStatus();
    
	SSPSTexCoord getTextureCoord(float x, float y, float z);
	bool isValidTexturePosition(float x, float y, float z);
    void changeTextureValue(int diff, float x, float y, float z);
    void draw();
};

class SSPSGateway
{
public:
	std::vector<SSPSVertex> vertices;
	int area1;
	int area2;
	std::string howto;	
};


struct Ray {
    mVector start;
    mVector dir;
    Ray(){}
    Ray(mVector s,mVector d){
        start=s; dir=d;
		dir = dir.normalize();
    }
    
    mVector operator () (float t) 
    {
        return getPoint(t);
    }
    
    mVector getPoint(float t)
    {
        mVector v;
        v.x=start.x + (dir.x*t);
        v.y=start.y + (dir.y*t);
        v.z=start.z + (dir.z*t);
        return v;
    }
    
    float intersect(Plane p) {
        float v= p.N * dir;
        if (v == 0) return -1;
        float v0=-(start - p.center) * p.N;
        return v0/v;
    }
};


struct SSPSObject
{
	int ID;
	std::string name;
	std::vector<SSPSFace> faces;
};

struct SSPSNegativeSpace
{
	int ID;
	std::string name;
	std::vector<SSPSFace> faces;
	std::vector<int> contains;
};

struct DebugRayInfo
{
    mVector intersection;
    mVector eyePoint;
    float t;
    int faceID;
};

class SSPSGeometry
{
private:
	std::vector<SSPSObject> objects;
	std::vector<SSPSNegativeSpace> freespace;
	std::vector<SSPSGateway> gateways;
	std::string filename;
	
	int goalID;
	bool drawNegativeSpace;
	bool drawGateways;
	bool trimNegativeSpace;
	
	float totalAverageNegativeY;
	float totalAverageNegativeZ;
	
    //debug info
    std::vector<DebugRayInfo> intersectionList;
    float maxRayDistance;
    bool faceTextures;
	
public:
	bool flipX;
	bool swapYZ;
	
	SSPSGeometry();
	
	void setFaceTextures(bool f);
	void clear();
	void loadFile(std::string fname);
	void writeTextures(std::string fname);
	void loadTextures(std::string fname);
    void initializeFaceTextures();
    void deleteFaceTextures();
	void render();
	int numObjects();
	SSPSObject getObject(int i);
	std::string getFilename();
	
	void toggleDrawNegativeSpace();
	void toggleTrimNegative();
	void toggleDrawGateways();
    bool pointInObject(mVector p, SSPSObject obj);
    std::vector<SSPSFace*> calculateSurfaceIntersections(std::vector<ViewFrustum*> viewPoints);
	void setGoalID(int ID);
	std::vector<SSPSFace*> getAllValidFaces();
};

#endif
