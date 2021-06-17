-------------------------------------------------------------------------------
 PlayerViz (Player Trace Visualizer) 
 Version 0.5 Released 08.10.2007
 Part of the CGUL (Common Games Understanding and Learning) Toolkit
 URL: http://playground.uncc.edu/GameIntelligenceGroup/Projects/CGUL
-------------------------------------------------------------------------------
###############
#    Usage    #
###############

Optional parameters:
[player trace filename] {SSPS map filename} [image name]

If PlayerViz is started with the above parameters, then it will be in automatic image capture mode.  This means it will open the player trace and map (if provided) and take 4 snapshots from different angles and save them as .pnm image files.  Keep in mind that the image name is not a filename, just the part before the extention since a number (0-3) and ".pnm" will be added to it automatically.

If PlayerViz is started without parameters then it will be in interactive mode.  The user can open a player trace and/or map file and examine them.  Multiple traces can be opened, and the ones that are highlighted in the list box will be highlighted in the 3D view.  
The mouse controls are as follows:
Left button drag: Rotate view
Middle button drag: Pan view
Right button drag: Zoom view
Scroll wheel: Zoom view

We have included some sample data in the SampleData folder.  This includes three player traces and one SSPS region map.  

###############
#  Compiling  #
###############

We have provided several ways to compile PlayerViz.  If a command line interface is used to compile, the Makefile provided should automatically detect the platform and use the right makefile.  Here we will explain the options for each platform:

First on any platform, you must download the Fast and Light ToolKit (FLTK)
Available here: http://fltk.org

You will also need OpenGL and GLU libraries

Windows:
- If you want to use Visual Studio, you can use the VS solution and project files in the FLTK source package to compile FLTK.  Then you can use the Visual Studio project file we provided in the Windows folder, however, this is for Version 7.0 (2002).  
- You can also use cygwin to make PlayerViz, this will require you to also make the fltk libraries using cygwin

Mac OS X:

FLTK must be compiled using the Terminal using one of two methods:
The standard way:
- open Terminal and run "./configure --enable-shared" in the fltk directory.  
- Once it configures successfully, you can run "make" and then "make install"

If you have problems getting it to compile in this way, you can download "CMake" (http://cmake.org) and use that as follows:
- open Terminal and run "cmake ." in the fltk directory
- then run "ccmake ." and make sure you turn ON the shared libraries option
- press 'c' and then 'g'
- now you can run "make" and "make install"

Once FLTK is installed, the provided Xcode project files should work to compile and run PlayerViz.  If they do not, you can also compile it using the provided Makefile

Linux:
FLTK must be compiled as follows:
- open Terminal and run "./configure --enable-shared" in the fltk directory.  
- Once it configures successfully, you can run "make" and then "make install"

Once FLTk is installed, use the provided Makefile to compile and run PlayerViz

###############
#   Contact   #
###############

If you have any issues or questions regarding PlayerViz, feel free to contact Dr. Michael Youngblood at youngbld@uncc.edu.

Also visit our website at:
http://playground.uncc.edu/GameIntelligenceGroup/Projects/CGUL

