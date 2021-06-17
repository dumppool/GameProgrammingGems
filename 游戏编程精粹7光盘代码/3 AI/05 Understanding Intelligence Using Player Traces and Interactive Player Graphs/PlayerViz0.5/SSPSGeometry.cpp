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

#include "SSPSGeometry.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
#include <ctime>
#include "hMatrix.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define timeSince(x) ((clock()-x)/CLOCKS_PER_SEC)

using namespace std;

///////////////
// Find the closest power of 2 to n
int closestPOW2(int n)
{
    for (int p=1; p < 20; p++)
    {
        int power2 = (int)pow(2.0f,p);
        if (power2 >= n)
        {
            return power2;
        }
    }
    return 0;
}

SSPSFace::SSPSFace()
{
    texture=NULL;
    selected=false;
}

SSPSFace::~SSPSFace()
{
}

void SSPSGeometry::setFaceTextures(bool f)
{
	faceTextures=f;
}

void SSPSFace::initialize()
{
    //Calculate the plane of the face
    //Plane point
    plane.center= mVector(vertices[0].x,vertices[0].y,vertices[0].z);
    
    //Calculate normal vector of plane
    mVector vec1 = mVector(vertices[0].x,vertices[0].y,vertices[0].z) - 
        mVector(vertices[2].x,vertices[2].y,vertices[2].z);
    
    mVector vec2 = mVector(vertices[0].x,vertices[0].y,vertices[0].z) - 
        mVector(vertices[1].x,vertices[1].y,vertices[1].z);
    
    plane.N = vec2.cross(vec1);
    plane.N = plane.N.normalize();
        
    //First we rotate the N vector of this face's plane
    //  so its aligned with the +Z axis.  This will put
    //  all the vertices on the XY plane
    mVector zaxis(0,0,1);
    hMatrix normalToZaxis(4);
    hMatrix zAxisToNormal(4);
    
    //first make sure that the plane is not already aligned with the Z axis
    // If the dot product is 1 or -1, we don't need to rotate at all
    float dot = zaxis * plane.N;
    
    if (dot != 1 && dot != -1)
    {
        normalToZaxis.rotateBetweenVectors(plane.N, zaxis);
        zAxisToNormal.rotateBetweenVectors(zaxis, plane.N);
    }
    
    boundQuad.initialize();
    vector<mVector> rotatedVerts;
    
    for (int i=0; i < vertices.size(); i++)
    {
        //Rotate the vertex to align it with the Z axis
        mVector v0 = mVector(vertices[i].x,vertices[i].y,vertices[i].z);
        mVector v = normalToZaxis * v0;
        
        rotatedVerts.push_back(v);
        
        if (v.x < boundQuad.minPoint.x)
        {
            boundQuad.minPoint.x=v.x;
        }
        else if (v.x > boundQuad.maxPoint.x)
        {
            boundQuad.maxPoint.x=v.x;
        }
        if (v.y < boundQuad.minPoint.y)
        {
            boundQuad.minPoint.y=v.y;
        }
        else if (v.y > boundQuad.maxPoint.y)
        {
            boundQuad.maxPoint.y=v.y;
        }
        //Z is constant for the transformed vertices
        boundQuad.minPoint.z = v.z;
        boundQuad.maxPoint.z = v.z;
    }
    
    //calculate the texture for the quad based on the bounding quad
    float xSize = boundQuad.maxPoint.x - boundQuad.minPoint.x;
    float ySize = boundQuad.maxPoint.y - boundQuad.minPoint.y;
    
    //create the texture (size has to be a power of 2)
    textureWidth = closestPOW2((int)xSize);
    textureHeight = closestPOW2((int)ySize);

    initTexture();
    
    //Now that we have the min/max points.. we can store the texture coordinates
    texCoords.clear();
    for (int i=0; i < rotatedVerts.size(); i++)
    {
        SSPSVertex tex;
        tex.x = (rotatedVerts[i].x - boundQuad.minPoint.x) / textureWidth;
        tex.y = (rotatedVerts[i].y - boundQuad.minPoint.y) / textureHeight;
        if (tex.x > 1)
        {
            tex.x = 1.0f;
        }
        if (tex.x < 0)
        {
            tex.x = 0.0f;
        }
        if (tex.y > 1)
        {
            tex.y = 1.0f;
        }
        if (tex.y < 0)
        {
            tex.y = 0.0f;
        }
        
        texCoords.push_back(tex);
    }
}

void SSPSFace::deleteTexture()
{
    if (texture != NULL)
    {
        glDeleteTextures(1,&(texID));
        delete [] textureUpdated;
        delete [] texture;
        texture=NULL;
    }
}

void SSPSFace::initTexture()
{
    glEnable(GL_TEXTURE_2D);
    if (texture == NULL)
	{
        texture = new GLubyte[textureWidth*textureHeight*4];
        textureUpdated = new bool[textureWidth*textureHeight];
        clearTexture();
        
        glGenTextures(1,&(texID));
    }
    
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	glDisable(GL_TEXTURE_2D);
}

void SSPSFace::clearTexture()
{
    if (texture == NULL)
    {
        return;
    }
    for (int i=0; i < textureHeight; i++)
    {
        for (int j=0; j < textureWidth; j++)
        {
            textureUpdated[(i*textureWidth) + j]=false;

            int index=((i*textureWidth) + j) * 4;
            
            texture[index]=255;
			
            texture[index+1]=255;
			
            texture[index+2]=255;
			
            texture[index+3]=255;//alpha
        }
    }
    
    //Reset the texture
    totalValue = 0;
    
    //Set selected to false.. 
    selected=false;
}

void SSPSFace::clearUpdateStatus()
{
    for (int i=0; i < textureHeight; i++)
    {
        for (int j=0; j < textureWidth; j++)
        {
            int index=((i*textureWidth) + j);
            textureUpdated[index]=false;
        }
    }
}

SSPSTexCoord SSPSFace::getTextureCoord(float x, float y, float z)
{
	mVector point(x,y,z);
    mVector zaxis(0,0,1);
    hMatrix normalToZaxis(4);
    
    float dot = zaxis * plane.N;
    if (dot != 1 && dot != -1)
    {
        normalToZaxis.rotateBetweenVectors(plane.N, zaxis);
    }
    
    //rotate the point onto the bounding quad's plane
    point = normalToZaxis * point;
	
	if (point.x < boundQuad.minPoint.x ||
		point.y < boundQuad.minPoint.y ||
		point.x > boundQuad.maxPoint.x ||
		point.y > boundQuad.maxPoint.y)
	{
		return SSPSTexCoord(-1,-1);
	}
	
	int u = int(point.x - boundQuad.minPoint.x);
	int v = int(point.y - boundQuad.minPoint.y);
    
	return SSPSTexCoord(u,v);
}

bool SSPSFace::isValidTexturePosition(float x, float y, float z)
{
    //Find the u,v coordinates based on x,y,z and bounding quad
    SSPSTexCoord point = getTextureCoord(x,y,z);
	
    //now simple bounds checking will do
    if (point.u < 0 || point.u >= textureWidth ||
		point.v < 0 || point.v >= textureHeight)
    {
        //the point is out of bounds
        return false;
    }
	
	return true;
}

void SSPSFace::changeTextureValue(int diff, float x, float y, float z)
{
	if (isValidTexturePosition(x,y,z))
	{
		SSPSTexCoord point = getTextureCoord(x,y,z);
		if (textureUpdated[int((point.v*textureWidth) + point.u)] == false)
		{
			int index = ((point.v*textureWidth) + point.u) * 4;
			//By default decrement all colors
			texture[index]   -=diff;
            texture[index+1] -=diff;
            texture[index+2] -=diff;
			textureUpdated[int((point.v*textureWidth) + point.u)] = true;
            
            //increment total value
            totalValue += diff;
		}
	}
}

void SSPSFace::draw()
{
    if (texture != NULL)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID);
	}
    
    glBegin(GL_POLYGON);
    for (int i=0; i < vertices.size(); i++)
    {
        glNormal3f(normal.x, normal.z, normal.y);
        glTexCoord2f(texCoords[i].x, texCoords[i].y);
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd(); 
	
	glDisable(GL_TEXTURE_2D);
}

SSPSGeometry::SSPSGeometry()
{
	drawNegativeSpace=false;
	srand((unsigned)time(0));
	flipX = true;
	swapYZ = true;
	trimNegativeSpace = true;
	drawGateways=false;
}

void SSPSGeometry::clear()
{
    deleteFaceTextures();
	objects.clear();
	freespace.clear();
	gateways.clear();
	filename="";
}

void SSPSGeometry::loadFile(string fname)
{
	bool positiveSpace=false;
	bool negativeSpace=false;
	bool gatewaysFound=false;
	string line;
	int objectIndex=-1;
	int freespaceIndex=-1;
	int gatewayIndex=-1;

	totalAverageNegativeY = 0;
	totalAverageNegativeZ = 0;
	
	ifstream ifs(fname.c_str());
	if (!ifs)
	{
		cerr << "Invalid file!" << endl;
		return;
	}
	clear();
    
	this->filename = fname;
	
	/**************************
		Freespace objects
	**************************/
	freespace.clear();
	while(!ifs.eof())
	{
		getline(ifs,line);
		if (line.size() > 10)
		{
			if (line.substr(1,10) == "<negative>")
			{
				//starting the negative freespace objects
				negativeSpace=true;
				break;
			}
		}
	}
	int totalNegativeFaces = 0;
	//now if we are within positive space records read the data
	if (negativeSpace == true)
	{
		while(1)
		{
			getline(ifs,line);
			if (line.size() > 9)
			{
				if (line.substr(1,11) == "</negative>")
				{
					//end of negative space
					negativeSpace=false;
					break;
				}
				if (line.substr(3,4) == "area")
				{
					//start new freespace area
					string line2;
					freespace.push_back(SSPSNegativeSpace());
					freespaceIndex++;
					
					//get the area ID
					//search for "
					int idStart = line.find("\"")+1;
					int idStop = line.rfind("\"")-1;
					freespace[freespaceIndex].ID = atoi(line.substr(idStart,idStop-idStart+1).c_str());
					
					//now get the name of the object
					getline(ifs,line2);
					//search for > <
					int nameStart= line2.find(">")+1;
					int nameStop = line2.find("<",nameStart+1)-1;
					freespace[freespaceIndex].name = line2.substr(nameStart,nameStop-nameStart+1);
					
					//read in all the faces now
					SSPSFace currentFace;
					//vector<SSPSFace> allFaces;
					while (1)
					{
						getline(ifs,line2);
						if (line2.substr(2,7) == "</area>")
						{
							//end of area
							break;
						}
						else if (line2.substr(4,8) == "contains")
						{
							//ID of the object it contains
							idStart = line2.find("\"")+1;
							idStop = line2.rfind("\"")-1;
							freespace[freespaceIndex].contains.push_back(atoi(line2.substr(idStart,idStop-idStart+1).c_str()));
						}
						//if not end of object, it should be a <face...>
						else if (line2.substr(4,4) == "face")
						{
							//get the face ID
							int faceIDStart = line2.find("\"")+1;
							int faceIDStop  = line2.find("\"",faceIDStart+1)-1;
							currentFace.ID = atoi(line2.substr(faceIDStart,faceIDStop-faceIDStart+1).c_str());
							
							//get numVertices
							int numVertStart = line2.find("\"",faceIDStop+2)+1;
							int numVertStop  = line2.find("\"",numVertStart+1)-1;
							int numVertices = atoi(line2.substr(numVertStart,numVertStop-numVertStart+1).c_str());
							
							currentFace.vertices.clear();
							float avgY = 0;
							float avgZ = 0;
							for (int i=0; i < numVertices; i++)
							{
								string vertexLine;
								getline(ifs,vertexLine);
								currentFace.vertices.push_back(SSPSVertex());
								
								//get vertex info
								int xStart = vertexLine.find("\"")+1;
								int xStop  = vertexLine.find("\"",xStart+1)-1;
								currentFace.vertices[i].x = atof(vertexLine.substr(xStart,xStop-xStart+1).c_str());
								
								int yStart = vertexLine.find("\"",xStop+2)+1;
								int yStop  = vertexLine.find("\"",yStart+1)-1;
								currentFace.vertices[i].y = atof(vertexLine.substr(yStart,yStop-yStart+1).c_str());
								
								int zStart = vertexLine.find("\"",yStop+2)+1;
								int zStop  = vertexLine.find("\"",zStart+1)-1;
								currentFace.vertices[i].z = atof(vertexLine.substr(zStart,zStop-zStart+1).c_str());
								
								avgY += currentFace.vertices[i].y;
								avgZ += currentFace.vertices[i].z;
							}
							avgY /= numVertices;
							avgZ /= numVertices;
							
							//now read the normal of the face
							getline(ifs,line2);
							int xStart = line2.find("\"")+1;
							int xStop  = line2.find("\"",xStart+1)-1;
							currentFace.normal.x = atof(line2.substr(xStart,xStop-xStart+1).c_str());
							
							int yStart = line2.find("\"",xStop+2)+1;
							int yStop  = line2.find("\"",yStart+1)-1;
							currentFace.normal.y = atof(line2.substr(yStart,yStop-yStart+1).c_str());
							
							int zStart = line2.find("\"",yStop+2)+1;
							int zStop  = line2.find("\"",zStart+1)-1;
							currentFace.normal.z = atof(line2.substr(zStart,zStop-zStart+1).c_str());
							
							
							//add to the list
							freespace[freespaceIndex].faces.push_back(currentFace);
							totalNegativeFaces++;
							
							//need to store total average
							totalAverageNegativeY += avgY;
							totalAverageNegativeZ += avgZ;
						
							//read the </face> line
							getline(ifs,line2);
						}
					}
				}
			}
		}
	}
	
	/**************************
		Positive objects
	**************************/
	objects.clear();
	while(!ifs.eof())
	{
		getline(ifs,line);
		if (line.size() > 10)
		{
			if (line.substr(1,10) == "<positive>")
			{
				//starting the positive space objects
				positiveSpace=true;
				break;
			}
		}
	}
	int totalPositiveFaces = 0;
	//now if we are within positive space records read the data
	if (positiveSpace == true)
	{
		while(1)
		{
			getline(ifs,line);
			if (line.size() > 9)
			{
				if (line.substr(1,11) == "</positive>")
				{
					//end of positive space
					positiveSpace=false;
					break;
				}
				if (line.substr(3,6) == "object")
				{
					//start new object
					string line2;
					int faceIndex=-1;
					objects.push_back(SSPSObject());
					objectIndex++;
					
					//get the object ID
					//search for "
					int idStart = line.find("\"")+1;
					int idStop = line.rfind("\"")-1;
					objects[objectIndex].ID = atoi(line.substr(idStart,idStop-idStart+1).c_str());
					
					//now get the name of the object
					getline(ifs,line2);
					//search for > <
					int nameStart= line2.find(">")+1;
					int nameStop = line2.find("<",nameStart+1)-1;
					objects[objectIndex].name = line2.substr(nameStart,nameStop-nameStart+1);
					
					//read in all the faces now
					while (1)
					{
						getline(ifs,line2);
						if (line2.substr(2,9) == "</object>")
						{
							//end of object
							break;
						}
						//if not end of object, it must be a <face...>
						objects[objectIndex].faces.push_back(SSPSFace());
						totalPositiveFaces++;
						faceIndex++;
						
						//get the face ID
						int faceIDStart = line2.find("\"")+1;
						int faceIDStop  = line2.find("\"",faceIDStart+1)-1;
						objects[objectIndex].faces[faceIndex].ID = atoi(line2.substr(faceIDStart,faceIDStop-faceIDStart+1).c_str());

						//get numVertices
						int numVertStart = line2.find("\"",faceIDStop+2)+1;
						int numVertStop  = line2.find("\"",numVertStart+1)-1;
						int numVertices = atoi(line2.substr(numVertStart,numVertStop-numVertStart+1).c_str());
						for (int i=0; i < numVertices; i++)
						{
							string vertexLine;
							getline(ifs,vertexLine);
							objects[objectIndex].faces[faceIndex].vertices.push_back(SSPSVertex());
							
							//get vertex info
                            //X is flipped in the world so negate it
							int xStart = vertexLine.find("\"")+1;
							int xStop  = vertexLine.find("\"",xStart+1)-1;
							objects[objectIndex].faces[faceIndex].vertices[i].x = atof(vertexLine.substr(xStart,xStop-xStart+1).c_str());
							
                            //We are reading it XZY because Z is up in SSPS 
							int yStart = vertexLine.find("\"",xStop+2)+1;
							int yStop  = vertexLine.find("\"",yStart+1)-1;
							objects[objectIndex].faces[faceIndex].vertices[i].y = atof(vertexLine.substr(yStart,yStop-yStart+1).c_str());
							
							int zStart = vertexLine.find("\"",yStop+2)+1;
							int zStop  = vertexLine.find("\"",zStart+1)-1;
							objects[objectIndex].faces[faceIndex].vertices[i].z = atof(vertexLine.substr(zStart,zStop-zStart+1).c_str());
						}
						
						//now read the normal of the face
						getline(ifs,line2);
						int xStart = line2.find("\"")+1;
						int xStop  = line2.find("\"",xStart+1)-1;
						objects[objectIndex].faces[faceIndex].normal.x = atof(line2.substr(xStart,xStop-xStart+1).c_str());
						
						int yStart = line2.find("\"",xStop+2)+1;
						int yStop  = line2.find("\"",yStart+1)-1;
						objects[objectIndex].faces[faceIndex].normal.y = atof(line2.substr(yStart,yStop-yStart+1).c_str());
						
						int zStart = line2.find("\"",yStop+2)+1;
						int zStop  = line2.find("\"",zStart+1)-1;
						objects[objectIndex].faces[faceIndex].normal.z = atof(line2.substr(zStart,zStop-zStart+1).c_str());
                        
						//read the </face> line
						getline(ifs,line2);
					}
				}
			}
		}
	}
	/**************************
		Gateways
	**************************/
	gateways.clear();
	while(!ifs.eof())
	{
		getline(ifs,line);
		if (line.size() > 10)
		{
			if (line.substr(1,10) == "<gateways>")
			{
				//starting the positive space objects
				gatewaysFound=true;
				break;
			}
		}
	}
	//now if we are within positive space records read the data
	if (gatewaysFound == true)
	{
		while(1)
		{
			getline(ifs,line);
			if (line.size() > 9)
			{
				if (line.substr(1,11) == "</gateways>")
				{
					//end of positive space
					gatewaysFound=false;
					break;
				}
				if (line.substr(3,7) == "gateway")
				{
					//start new object
					string line2;
					int vertexIndex=-1;
					gateways.push_back(SSPSGateway());
					gatewayIndex++;
					
					//get the area1 and area2 ID
					//search for "
					int area1Start = line.find("\"")+1;
					int area1Stop  = line.find("\"",area1Start+1)-1;
					gateways[gatewayIndex].area1 = atoi(line.substr(area1Start,area1Stop-area1Start+1).c_str());
					
					int area2Start = line.find("\"",area1Stop+2)+1;
					int area2Stop  = line.find("\"",area2Start+1)-1;
					gateways[gatewayIndex].area2 = atoi(line.substr(area2Start,area2Stop-area2Start+1).c_str());
					
					//get operator (how to traverse)
					int howtoStart = line.find("operator",area2Stop+2)+10;
					int howtoStop  = line.find("\"",howtoStart+1)-1;
					gateways[gatewayIndex].howto = line.substr(howtoStart,howtoStop-howtoStart+1);
					
					//read in all the vertices now
					while (1)
					{
						getline(ifs,line2);
						if (line2.substr(2,10) == "</gateway>")
						{
							//end of gateway
							break;
						}
						//if not end of gateway, it must be a <vertex...>
						gateways[gatewayIndex].vertices.push_back(SSPSVertex());
						vertexIndex++;
						
						//get vertex info
						//X is flipped in the world so negate it
						int xStart = line2.find("\"")+1;
						int xStop  = line2.find("\"",xStart+1)-1;
						gateways[gatewayIndex].vertices[vertexIndex].x = atof(line2.substr(xStart,xStop-xStart+1).c_str());
						
						//We are reading it XZY because Z is up in SSPS 
						int yStart = line2.find("\"",xStop+2)+1;
						int yStop  = line2.find("\"",yStart+1)-1;
						gateways[gatewayIndex].vertices[vertexIndex].y = atof(line2.substr(yStart,yStop-yStart+1).c_str());
						
						int zStart = line2.find("\"",yStop+2)+1;
						int zStop  = line2.find("\"",zStart+1)-1;
						gateways[gatewayIndex].vertices[vertexIndex].z = atof(line2.substr(zStart,zStop-zStart+1).c_str());
					}
				}
			}
		}
	}
	ifs.close();
	//calculate the total average
	totalAverageNegativeY /= totalNegativeFaces;
	totalAverageNegativeZ /= totalNegativeFaces;
	
	cout << "Negative objects=" << freespace.size() << ", faces=" << totalNegativeFaces << ", AvgY=" << totalAverageNegativeY << ", avgZ=" << totalAverageNegativeZ << endl;
	cout << "Positive objects=" << objects.size() << ", faces=" << totalPositiveFaces << endl;
	cout << "Gateways=" << gateways.size() << endl;
    cout << "DONE!\n";
	if (faceTextures)
	{
		initializeFaceTextures();
	}
}

void SSPSGeometry::writeTextures(string fname)
{
    ofstream ofs(fname.c_str());
    //ssps filename
    cout << filename << endl;
    ofs << filename.substr(filename.length() - 14, 5) << endl;
    for (int o=0; o < objects.size(); o++)
    {
        for (int f=0; f < objects[o].faces.size(); f++)
        {
            //Surface value
            ofs << objects[o].faces[f].totalValue << endl;
            if (objects[o].faces[f].totalValue > 0)
            {
                //Write out the entire texture
                for (int i=0; i < objects[o].faces[f].textureHeight; i++)
                {
                    for (int j=0; j < objects[o].faces[f].textureWidth; j++)
                    {
                        int index=((i*objects[o].faces[f].textureWidth) + j) * 4;
                        //saving the decrement for RGBA
                        ofs <<  (255-objects[o].faces[f].texture[index]) << " " << 
                                    (255-objects[o].faces[f].texture[index+1]) << " " << 
                                    (255-objects[o].faces[f].texture[index+2]) << " " << 
                                    (255-objects[o].faces[f].texture[index+3]) << endl;
                    }
                }
            }
        }
    }
    ofs.close();
}

void SSPSGeometry::loadTextures(string fname)
{
    ifstream ifs(fname.c_str());
    if (!ifs)
    {
        cerr << "Invalid filename\n";
        return;
    }
    //ssps filename
    string sspsname;
    getline(ifs,sspsname);
    if (sspsname != filename.substr(filename.length() - 14, 5))
    {
        cerr << "From a different map!\n";
        return;
    }
    for (int o=0; o < objects.size(); o++)
    {
        for (int f=0; f < objects[o].faces.size(); f++)
        {
            int num;
            //Surface value
            ifs >> num;
            objects[o].faces[f].totalValue += num;
            if (num > 0)
            {
                //Read the entire texture
                int dr;
                int dg;
                int db;
                int da;
                
                for (int i=0; i < objects[o].faces[f].textureHeight; i++)
                {
                    for (int j=0; j < objects[o].faces[f].textureWidth; j++)
                    {
                        int index=((i*objects[o].faces[f].textureWidth) + j) * 4;
                        ifs >> dr >> dg >> db >> da;
                        objects[o].faces[f].texture[index] -= dr;
                        objects[o].faces[f].texture[index+1] -= dg;
                        objects[o].faces[f].texture[index+2] -= db;
                        objects[o].faces[f].texture[index+3] -= da;
                    }
                }
                objects[o].faces[f].initTexture();
            }
        }
    }
    ifs.close();
}


void SSPSGeometry::initializeFaceTextures()
{
    for (int o=0; o < objects.size(); o++)
    {
        for (int f=0; f < objects[o].faces.size(); f++)
        {
            objects[o].faces[f].initialize();
        }
    }
}

void SSPSGeometry::deleteFaceTextures()
{
    for (int o=0; o < objects.size(); o++)
    {
        for (int f=0; f < objects[o].faces.size(); f++)
        {
            objects[o].faces[f].deleteTexture();
        }
    }
}

void SSPSGeometry::render()
{
	GLboolean lightingEnabled;
    glGetBooleanv(GL_LIGHTING,&lightingEnabled);
    vector<SSPSFace> drawFaces;
	
	float signX = ((flipX)?-1:1);
	
	glDisable(GL_DEPTH_TEST);
	if (drawNegativeSpace == true)
	{
		//draw the negative freespace areas
		for (unsigned int i=0; i < freespace.size(); i++)
		{
			float currAvgY=0;
			
			for (unsigned int j=0; j < freespace[i].faces.size(); j++)
			{
				//if we want to trim negative spaces
				if (trimNegativeSpace)
				{
					//is the normal facing up or down?
					if ((swapYZ==false && (fabs(freespace[i].faces[j].normal.y) > fabs(freespace[i].faces[j].normal.x) && fabs(freespace[i].faces[j].normal.y) > fabs(freespace[i].faces[j].normal.z))) ||
						(swapYZ==true && (fabs(freespace[i].faces[j].normal.z) > fabs(freespace[i].faces[j].normal.x) && fabs(freespace[i].faces[j].normal.z) > fabs(freespace[i].faces[j].normal.y))))
					{
						currAvgY = 0;
						for (int k=0; k < freespace[i].faces[j].vertices.size(); k++)
						{
							if (swapYZ)
							{
								currAvgY+= fabs(freespace[i].faces[j].vertices[k].z);
							}
							else
							{
								currAvgY+= fabs(freespace[i].faces[j].vertices[k].y);
							}
						}
						currAvgY /= freespace[i].faces[j].vertices.size();
						
						if ((swapYZ==false && (currAvgY <= totalAverageNegativeY)) ||
							(swapYZ==true  && (currAvgY <= totalAverageNegativeZ)))
						{
							//add to the list if its not higher than average height (to get rid of the "ceiling")
							drawFaces.push_back(freespace[i].faces[j]);
						}
					}
				}
				else
				{
					//just draw them all
					glColor4f(0.8,0.5,0.5,0.1);
					glNormal3f(signX * freespace[i].faces[j].normal.x,
					   ((swapYZ)?freespace[i].faces[j].normal.z:freespace[i].faces[j].normal.y),
					   ((swapYZ)?freespace[i].faces[j].normal.y:freespace[i].faces[j].normal.z));
			
					glBegin(GL_POLYGON);
					for (unsigned int k=0; k < freespace[i].faces[j].vertices.size(); k++)
					{
						glVertex3f(signX * freespace[i].faces[j].vertices[k].x,
						   ((swapYZ)?freespace[i].faces[j].vertices[k].z:freespace[i].faces[j].vertices[k].y),
						   ((swapYZ)?freespace[i].faces[j].vertices[k].y:freespace[i].faces[j].vertices[k].z));
					}
					glEnd();
				}
			}
		}
		if (trimNegativeSpace)
		{
			//only draw chosen faces
			for (int f=0; f < drawFaces.size(); f++)
			{
				glColor4f(0.8,0.5,0.5,0.1);
				glNormal3f(signX * drawFaces[f].normal.x,
						   ((swapYZ)?drawFaces[f].normal.z:drawFaces[f].normal.y),
						   ((swapYZ)?drawFaces[f].normal.y:drawFaces[f].normal.z));
				
				glBegin(GL_POLYGON);
				for (unsigned int k=0; k < drawFaces[f].vertices.size(); k++)
				{
					glVertex3f(signX * drawFaces[f].vertices[k].x,
							   ((swapYZ)?drawFaces[f].vertices[k].z:drawFaces[f].vertices[k].y),
							   ((swapYZ)?drawFaces[f].vertices[k].y:drawFaces[f].vertices[k].z));
				}
				glEnd();
			}
		}
	}

	//draw all objects
	for (unsigned int i=0; i < objects.size(); i++)
	{
		//each face of the object
		for (unsigned int j=0; j < objects[i].faces.size(); j++)
		{
            if (objects[i].faces[j].selected==true)
            {
                glColor4f(1,1,1,1);
                glDisable(GL_LIGHTING);
                glDisable(GL_BLEND);
            }
            else
            {
                glColor4f(1,1,1,0.2);
            }
            
            if (objects[i].faces[j].texture != NULL)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, objects[i].faces[j].texID);
            }
            
			glNormal3f(signX * objects[i].faces[j].plane.N.x,
					   ((swapYZ)?objects[i].faces[j].plane.N.z:objects[i].faces[j].plane.N.y),
					   ((swapYZ)?objects[i].faces[j].plane.N.y:objects[i].faces[j].plane.N.z));
			glBegin(GL_POLYGON);
			for (unsigned int k=0; k < objects[i].faces[j].vertices.size(); k++)
			{
				if (faceTextures)
				{
					glTexCoord2f(objects[i].faces[j].texCoords[k].x, objects[i].faces[j].texCoords[k].y);
				}
				glVertex3f(signX * objects[i].faces[j].vertices[k].x, 
						   ((swapYZ)?objects[i].faces[j].vertices[k].z:objects[i].faces[j].vertices[k].y), 
						   ((swapYZ)?objects[i].faces[j].vertices[k].y:objects[i].faces[j].vertices[k].z));
			}
			glEnd();
            
            glDisable(GL_TEXTURE_2D);
            if (lightingEnabled==true)
            {
                glEnable(GL_LIGHTING);
            }
            glEnable(GL_BLEND);
		}
	}
	
	//draw all gateways
	if (drawGateways)
	{
		for (unsigned int i=0; i < gateways.size(); i++)
		{
			glColor4f(0,1,0,0.1f);
				
			glBegin(GL_POLYGON);
			//each vertex of the gateway
			for (unsigned int j=0; j < gateways[i].vertices.size(); j++)
			{
				glVertex3f(signX * gateways[i].vertices[j].x, 
							((swapYZ)?gateways[i].vertices[j].z:gateways[i].vertices[j].y), 
							((swapYZ)?gateways[i].vertices[j].y:gateways[i].vertices[j].z));
			}
			glEnd();
		}
	}
	glEnable(GL_DEPTH_TEST);
}

bool insideFace(SSPSFace face, mVector point)
{
    double d = face.plane.getD();
    // dot product of test point and normal
    double pdotN = face.plane.N * point;

    if(FLOATSEQUAL(pdotN, d))
        return true;
    else if( pdotN < d)
        return true;
    else
        return false;
}


bool SSPSGeometry::pointInObject(mVector p, SSPSObject obj)
{
    for (int i=0; i < obj.faces.size(); i++)
    {
        if (!insideFace(obj.faces[i], p))
        {
            return false;
        }
    }
    return true;
}

vector<SSPSFace*> SSPSGeometry::calculateSurfaceIntersections(vector<ViewFrustum*> viewPoints)
{
    time_t tcount = clock();
	time_t tstart = clock();
	int numSkipped=0;
	
	cout << "**********************************************" << endl;
	cout << "# viewpoints = " << viewPoints.size() << endl;
    for (int o=0; o < objects.size(); o++)
    {
        for (int f=0; f < objects[o].faces.size(); f++)
        {
            objects[o].faces[f].clearTexture();
        }
    }
    
    intersectionList.clear();
    cout << "Distance=" << viewPoints[0]->distance << endl;
    cout << "Half angle=" << viewPoints[0]->angle << endl;
    
    for (int v=0; v < viewPoints.size(); v++)
    {
        maxRayDistance = viewPoints[v]->distance;
        //Location of player's eye point
        mVector eye(viewPoints[v]->eyePoint.cx,viewPoints[v]->eyePoint.cy,viewPoints[v]->eyePoint.cz);
        
        //Ray from eye
        Ray ray(eye, viewPoints[v]->eyePoint.N);
        
        //Middle point of the base
        mVector baseMidPoint = ray.start + (ray.dir*viewPoints[v]->distance);
        
        //Plane of the base of the frustum
        Plane base(baseMidPoint, -ray.dir);
        
        //Angle of the view frustum
        float baseAngle = viewPoints[v]->angle;
        
        //Half the size of one side of the square base
        float halfBaseSize = viewPoints[v]->distance * atan(baseAngle);
        
        //The full base size
        float baseSize = 2 * halfBaseSize;
        
        //Calculate corner point of base
        mVector topLeft = baseMidPoint + viewPoints[v]->eyePoint.U*halfBaseSize - viewPoints[v]->eyePoint.V*halfBaseSize;
        mVector bottomRight = baseMidPoint - viewPoints[v]->eyePoint.U*halfBaseSize + viewPoints[v]->eyePoint.V*halfBaseSize;
        
        if (timeSince(tcount) >= 10)
        {
            time_t timesince = timeSince(tstart);
            if (timesince < 0)
            {
                timesince = -timesince;
            }
            cout << "Viewpoint#" << v << " [skipped " << numSkipped << "] TIME=" << timesince << ".....(" << (timesince/(1.0f*v)) << " sec for one) ETA ~";
            //get estimated time of finish in seconds
            double eta = (viewPoints.size() * (timesince/(1.0f*v))) - timesince;
            if (eta > 60)
            {
                //Convert to minutes
                eta /= 60;
                if (eta > 60)
                {
                    //Convert to hours
                    eta /= 60;
                    cout << eta << " hours" << endl;
                }
                else
                {
                    cout << eta << " minutes" << endl;
                }
            }
            else
            {
                cout << eta << " seconds" << endl;
            }
           tcount=clock();
        }
        
        int inside= -1;
        //Check to make sure we are not inside an object
        for (int o=0; o < objects.size(); o++)
        {
            if (pointInObject(eye,objects[o]) == true)
            {
                inside=o;
                break;
            }
        }
        if (inside >= 0)
        {
            //skip this view frustum
            //cout << "Viewport#" << v << " skipped... inside object#" << inside << endl;
            viewPoints[v]->intersected=false;
            numSkipped++;
            continue;
        }
        bool foundIntersect=false;
        //instead of one ray.. shoot one to each point on the base
        for(float i=0; i < baseSize; i+=BASE_INCREMENT)
        {
            for(float j=0; j < baseSize; j+=BASE_INCREMENT)
            {
                 //Temp variable for holding the base point
                 mVector basePoint;
                 //point on base we are on
                 basePoint = topLeft + viewPoints[v]->eyePoint.V*i - viewPoints[v]->eyePoint.U*j;
                 //Ray from eye to base
                 Ray baseRay(eye, (basePoint - eye));
                 //The t value of the base point
                 float tbase = baseRay.intersect(base);
                 //Stores info on the best intersected surface so far
                 //initialize the tbest variable to the base t
                 float tbest=tbase;

                SSPSFace* bestFace=NULL;
                mVector bestPoint;
                mVector bestBasePoint;
                //cout << "   Checking ray intersections.. TIME=" << ((clock()-tstart)/CLOCKS_PER_SEC) << endl;
                for (int o = 0; o < objects.size(); o++)
                {
                    for (int f = 0; f < objects[o].faces.size(); f++)
                    {
                        //Make a plane for face
                        Plane plane = objects[o].faces[f].plane;
                        
                        float t = baseRay.intersect(plane);

                        if (t < 0 || t > tbase)
                        {
                            //not within range
                            continue;
                        }
                        
                        mVector newPos = baseRay(t);
                        
                        //if its between 0 and tbase
                        //check to see if this is the best one
                        if (t < tbest)
                        {
                            if (objects[o].faces[f].isValidTexturePosition(newPos.x,newPos.y,newPos.z))
                            {
                                tbest=t;
                                bestFace = &objects[o].faces[f];
                                //cout << "new best is " << tbest << ", from face#" << objects[o].faces[f].ID << endl;
                                bestPoint=newPos;
                                bestBasePoint = basePoint;
                            }
                        } 
                    }
                }
                //do it on the best
                if (bestFace != NULL)
                {
                    //store in intersection list
                    DebugRayInfo info;
                    info.intersection=bestPoint;
                    info.eyePoint = eye;
                    info.t = tbest;
                    info.faceID = bestFace->ID;
                   
                    intersectionList.push_back(info);
                    /*
                    cout << "Best face#" << bestFace->texID << " verts=" << endl;
                    for (int vertIndex=0; vertIndex < bestFace->vertices.size(); vertIndex++)
                    {
                        cout << bestFace->vertices[vertIndex].x << "," << bestFace->vertices[vertIndex].y << "," << bestFace->vertices[vertIndex].z << endl;
                    }
                    cout << "Intersection at: "; bestPoint.print();
                    */
                    //distance from user to the intersection point
                    float d = eye.distanceTo(bestPoint);
                    if (d == 0 || isnan(d))
                    {
                        cout << "d = " << d << endl;
                        d = 1;
                    }
                    //Max value for update 
                    float maxValue = 100000/(d*d);
                    
                    float baseRadius = baseMidPoint.distanceTo(bestBasePoint);
                    
                    if (baseRadius == 0 || isnan(baseRadius))
                    {
                        cout << "baseRadius = " << baseRadius << endl;
                        baseRadius = 1;
                    }
                    //cout << "value=" << (maxValue/baseRadius) << endl;
                    bestFace->changeTextureValue(int(maxValue/baseRadius),bestPoint.x,bestPoint.y,bestPoint.z);  
                    foundIntersect=true;
                    //Set the intersected to true
                    //viewPoints[v]->intersected=true;
                }
            }
            if (foundIntersect)
            {
                //cout << "Found intersection for Viewpoint#" << v <<"\n";
            }
        }
        for (int o=0; o < objects.size(); o++)
        {
            for (int f=0; f < objects[o].faces.size(); f++)
            {
                objects[o].faces[f].clearUpdateStatus();
            }    
        }
    }
    cout << "Part 1 time=" << ((clock() - tstart)/CLOCKS_PER_SEC) << endl;
    cout << "Total skipped viewpoints=" << numSkipped << endl;
    return getAllValidFaces();
}


int SSPSGeometry::numObjects()
{
	return objects.size();
}

SSPSObject SSPSGeometry::getObject(int i)
{
	return objects[i];
}

string SSPSGeometry::getFilename()
{
	return filename;
}

void SSPSGeometry::setGoalID(int ID)
{
	goalID=ID;
}

void SSPSGeometry::toggleDrawNegativeSpace()
{
	drawNegativeSpace=1 - drawNegativeSpace;
}

void SSPSGeometry::toggleTrimNegative()
{
	trimNegativeSpace = 1 - trimNegativeSpace;
}

void SSPSGeometry::toggleDrawGateways()
{
	drawGateways = 1 - drawGateways;
}

vector<SSPSFace*> SSPSGeometry::getAllValidFaces()
{
    vector<SSPSFace*> intersectedFaces;
    for (int o=0; o < objects.size(); o++)
    {
        for (int f=0; f < objects[o].faces.size(); f++)
        {
           //List them all for now
           //if (objects[o].faces[f].totalValue > 0)
            {
                cout << "Object#" << o << ", face#" << f << ", value=" << objects[o].faces[f].totalValue << endl;
                bool inserted=false;
                //insert it at the right position
                for (int i=0; i < intersectedFaces.size(); i++)
                {
                    if (intersectedFaces[i]->totalValue > objects[o].faces[f].totalValue)
                    {
                        intersectedFaces.insert(intersectedFaces.begin()+i, &objects[o].faces[f]);
                        inserted=true;
                        break;
                    }
                }
                if (inserted==false)
                {
                    intersectedFaces.push_back(&objects[o].faces[f]);
                }
                objects[o].faces[f].initTexture();
            }
        }
    }
    return intersectedFaces;
}


