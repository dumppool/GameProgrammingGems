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

#include "CGULData.h"
#include <fstream>
using namespace std;

CGULData::CGULData()
{
	capturedFlag.clear();
	playerTraces.clear();
	currentMap="";
	worldScale = 0.05f;
}

void CGULData::readPlayerTraceFile(const char* filename)
{
	ifstream ifs;
	
	ifs.open(filename);
	if (!ifs) {
		cerr << "NO FILE\n";
		return;
	}
	playerTraces.push_back(vector<PlayerMove>());
	int index=playerTraces.size()-1;
	int count=0;
	bool flagCapture=false;
	do {
		PlayerMove m;
		ifs >> m.x >> m.y >> m.z >> m.h >> m.p >> m.r >> m.speed >> m.etime >> m.time_score >> m.health >> m.shots >> m.time_score >> m.capture;
		
		if (m.capture == 0)
			playerTraces[index].push_back(m);
		else
		{
			cout << "Yes flag was captured at " << playerTraces[index][count-1].x << "," << playerTraces[index][count-1].y << "," << playerTraces[index][count-1].z << endl;
			flagCapture=true;
			break;
		}
		count++;
		
	}while(!ifs.eof());
	cout << "Done! This player trace has " << count << " points.\n";
	ifs.close();
	capturedFlag.push_back(flagCapture);
	
	//if flag NOT captured, then get rid of the last entry
	if (capturedFlag[index] == false)
	{
		playerTraces[index].erase(playerTraces[index].end()-1);
	}
}

void CGULData::readSSPSFile(const char* sspspath)
{
	//loaded successfully, now load the geometry
	//find out which map to load by taking a substring "...../[l?s??]-ssps.xml"
	//the map name is at len-10 to len-5
	string mapFilename = sspspath;
    string mapName;
	int lastSlash = mapFilename.rfind("/");
	if (lastSlash != string::npos)
	{
		mapName = mapFilename.substr(lastSlash+1, mapFilename.length() - (lastSlash+1));
	}
	else
	{
		//we must be on Windows
		lastSlash = mapFilename.rfind("\\");
		mapName = mapFilename.substr(lastSlash+1, mapFilename.length() - (lastSlash+1));
	}
	if (mapFilename != ssps.getFilename())
	{
		cout << "loading map: " << mapFilename << endl;
		ssps.loadFile(mapFilename);
	}
	currentMap = mapName;
}

void CGULData::readDiasData(string filename)
{
	ifstream ifs(filename.c_str());
	string line;
	
	while(!ifs.eof())
	{
		getline(ifs,line);
		int pos = line.find("neutralflag");
		if (pos != string::npos)
		{
			//find the start/stop for X pos
			int numStart = line.find("_",pos)+1;
			int numStop = line.find("_",numStart)-1;
			flagPosition.x = -atoi(line.substr(numStart, numStop-numStart+1).c_str());
			
			numStart = line.find("_",numStop)+1;
			numStop = line.find("_",numStart)-1;
			flagPosition.z = atoi(line.substr(numStart, numStop-numStart+1).c_str());

			numStart = line.find("_",numStop)+1;
			numStop = line.find("\"",numStart)-1;
			flagPosition.y = atoi(line.substr(numStart, numStop-numStart+1).c_str());
			
			//get the ="
			numStart = line.find("=\"",numStop)+2;
			numStop = line.find("\"",numStart)-1;
			flagPositionID = atoi(line.substr(numStart,numStop-numStart+1).c_str());
			cout << "FLAG FOUND: " << flagPosition.x << "," << flagPosition.y << "," << flagPosition.z << ", ID=" << flagPositionID << endl;
			break;
		}
	}
	ifs.close();
}

void CGULData::clearData()
{
	//clears out the paths
	playerTraces.clear();
	capturedFlag.clear();
	ssps.clear();
	
	currentMap="";
}

bool CGULData::removeTrace(int i)
{
	if (i < 0 || i >= playerTraces.size())
	{
		return false;
	}
	playerTraces.erase(playerTraces.begin() + i);
	return true;
}

void CGULData::setDataDirectory(string dir)
{
	dataDirectory=dir;
	if (dataDirectory[dataDirectory.length()-1] != '/' && dataDirectory[2] != ':')
	{
		//not on Windows and needs a trailing slash
		dataDirectory += "/";
	}
	else if (dataDirectory[dataDirectory.length()-1] != '\\' && dataDirectory[2] == ':')
	{
		//on Windows
		dataDirectory += "\\";
	}
}



