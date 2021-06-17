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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Value_Slider.H>

#include "GLWidget.h"

class MainWindow
{
  private:

    Fl_Window *win;
    GLWidget *glwidget;
	Fl_File_Chooser* chooser;
	Fl_File_Chooser* sspsChooser;
    
	Fl_Output *lblCurrentMap;
	Fl_Multi_Browser *playerTraceBrowser;
	Fl_Light_Button *btnLight, *btnAxes, *btnNegative, *btnTrimNegative, *btnGateways, *btnSwapYZ, *btnFlipX;
	Fl_Button *btnRemove, *btnResetCamera;
	Fl_Value_Slider *sldWorldScale;
	
    std::string lastTracePathChosen;
    std::string lastSSPSPathChosen;
    std::string currentPath;

  public:

    MainWindow(int argc, char** argv);
    ~MainWindow();
    
	void savePathToFile();
    void show( int argc, char **argv );
	void setTitle(const char* title);
	
	void setLastPath(std::string path);
    
	Fl_File_Chooser* getChooser();
    Fl_File_Chooser* getSSPSChooser();
    
    std::string getCurrentPath();
    
	GLWidget* getGLWidget();
	void myGuiCallback(Fl_Widget* w);
};

#endif // MAINWINDOW_H
