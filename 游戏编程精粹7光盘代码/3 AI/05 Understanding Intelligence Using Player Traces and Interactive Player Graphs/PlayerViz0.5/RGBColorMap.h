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

#ifndef RGB_COLOR_MAP
#define RGB_COLOR_MAP

#include <vector>

class RGBColor
{
public:
	float R;
	float G;
	float B;
	float value;
	RGBColor(){
		R=0; G=0; B=0; value=0;
	}
	RGBColor(float v, float r, float g, float b)
	{
		R=r; G=g; B=b; value=v;
	}
	
	RGBColor operator - (const RGBColor &rhs)const
	{
		RGBColor color;
		color.R = R - rhs.R;
		color.G = G - rhs.G;
		color.B = B - rhs.B;
		color.value = value - rhs.value;
		return color;
	}
};

class RGBColorMap
{
private:
	std::vector<RGBColor> colorList;
	
public:
	RGBColorMap();
	
	void addColor(float pos, float R, float G, float B);
	RGBColor getColor(float position);	
};

#endif


