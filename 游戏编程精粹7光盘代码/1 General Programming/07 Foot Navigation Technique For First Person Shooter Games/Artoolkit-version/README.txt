Read me for "Foot navigation technique for first person shooting games" - with ARToolkit version
================================================================================================

Contents.
---------
About this archive.
The FootNavigation license model.
Building on Windows.
Running on windows.


About this archive.
-------------------

These are the files necessary to run and compile our prototype FootNavigation.

FootNavigation is designed to build on Windows platform.

This archive was assembled by:
    Marcus A. C. Farias, Daniela G. Trevisan and Luciana P.Nedel
    Universidade Federal do Rio Grande do Sul - RS - Brasil
    Instituto de Informática
    http://www.inf.ufrgs.br/
    contact: nedel@inf.ufrgs.br
    2007-07-07


The FootNavigation license model.
--------------------------------
FootNavigation is released under the GNU General Public License (GPL).
FootNavigation is freely available for non-commercial use under the terms of the GNU General Public License.


Building on windows.
-------------------
These are the files necessary to compile our prototype under MS Visual C++ 2005.
Most (if not all) of the code is standard ISO C++, so it should compile anywhere, provided you have the libraries properly set up.

You need to have installed in your computer:

Boost library - http://www.boost.org

The following libraries (includes, libs and DLLs) are provided with the project:

SDL - Simple Directmedia Layer - http://www.libsdl.org/

ARToolkit - (tested with ARToolKit 1.72.1 version) - http://sourceforge.net/projects/artoolkit/


Includes directories needed to compile the project:

Boost-directory;include

Libs needed to compile the project: 

libAR.lib libARgsub_lite.lib libARvideo.lib glu32.lib opengl32.lib SDLmain.lib SDL.lib

Compile the project as Release Solution COnfiguration.


Running on windows.
-------------------
Place the executable file (lab.exe) into the bin directory. The  bin directory contains all files needed to run the program.

We will need a webcam to run the program.

If you have some problem while running  the program take a look in the labrv_log.txt file.


The controls are:
W: walk forward
S: walk backward
A: sidestep left
D: sidestep right
Up Arrow: look up
Down Arrow: look down
Left Arrow: turn left
Right Arrow: turn right

Pause: "grab" the mouse pointer, so that you can look around using mouse movements (but you will be unable to switch to other windows using the mouse).

Press Pause again to "ungrab" it.

Mouse: look around
Mouse left button: walk forward
Mouse right button: walk backward

Esc: exit
Space: Start/Restart

Naturally you won't need most of these commands, since you can control the game, using the "Hiro" marker movements. You can find the "Hiro" marker (pattHiro.pdf) in the Pattern directory. Place it facing the camera, then move and rotate it around to see the results. For more information see the interactions described in the paper: "Foot navigation technique for first person shooting games" found in GEMS 7 book.

===================
EOF