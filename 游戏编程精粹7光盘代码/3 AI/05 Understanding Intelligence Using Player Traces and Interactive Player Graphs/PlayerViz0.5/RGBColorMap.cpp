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

#include "RGBColorMap.h"
#include <iostream>
using namespace std;

RGBColorMap::RGBColorMap()
{
	colorList.clear();
}

void RGBColorMap::addColor(float pos, float R, float G, float B)
{
	//pos: position from 0 to 1 along the normalized spectrum
	bool found=false;
	
	for (unsigned int i=0; i < colorList.size(); i++)
	{
		if (colorList[i].value >= pos)
		{
			colorList.insert(colorList.begin()+i, RGBColor(pos,R,G,B));
			found=true;
			break;
		}
	}
	//if empty and nothing bigger found, just add it
	if (colorList.size() == 0 || found==false)
	{
		colorList.push_back(RGBColor(pos,R,G,B));
		return;
	}
}

RGBColor RGBColorMap::getColor(float position)
{
	for (unsigned int i=0; i < colorList.size(); i++)
	{
		//if colorList[i] is bigger than position, we should interpolate from i-1 to i
		if (colorList[i].value >= position)
		{
			//interpolate
			RGBColor diffColor, color;
			float ratio=0;
			if (i == 0)
			{
				diffColor = colorList[i];
				ratio = position / diffColor.value;
				color.R = diffColor.R * ratio;
				color.G = diffColor.G * ratio;
				color.B = diffColor.B * ratio;
				color.value = position;
			}
			else
			{
				diffColor = colorList[i] - colorList[i-1];
				ratio = (position - colorList[i-1].value) / diffColor.value;
				color.R = colorList[i-1].R + (diffColor.R * ratio);
				color.G = colorList[i-1].G + (diffColor.G * ratio);
				color.B = colorList[i-1].B + (diffColor.B * ratio);
				color.value = position;
			}
			return color;
		}
	}
	return RGBColor();
}
