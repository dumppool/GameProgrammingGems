============================================================================
Read me for "Foot navigation technique for first person shooting games" 
with Flock of Birds version. Additional code for data glove and head mounted 
display devices is also provided
============================================================================


About this archive.
-------------------

These are the files necessary to run and compile our prototype FootNavigation using flock of brids (magnetic tracker equipament) and data glove.

This prototype is designed to build on Windows platform.

This archive was assembled by:
    Marcus A. C. Farias, Daniela G. Trevisan and Luciana P.Nedel
    Colaborators: Fabio Dapper and Alexandre L. Azevedo
    Universidade Federal do Rio Grande do Sul - RS - Brasil
    Instituto de Informática
    http://www.inf.ufrgs.br/
    contact: nedel@inf.ufrgs.br
    2007-07-07

The FootNavigation license model.
--------------------------------
FootNavigation is released under the GNU General Public License (GPL).
FootNavigation is freely available for non-commercial use under the terms of the GNU General Public License.


Files needed for execution.
---------------------------
Textures files: chao.bmp, crate.bmp, init.bmp, parede.bmp end.bmp
Configuration file: rvirt.cfg
Log file: labrv_log.txt
Map (standard): rvirt.map
DLLs: SDL.dll, Bird.dll (Flock of Birds), fglove.dll (data glove), OpenGL dlls (system dlls), VisualStudio Dlls.
These files should be in the same directory of hte executable file.


Colors indication in the game.

Red: target reached
Green: target activated
Yellow: exit (the end)
Blue: entrance (starting capture of measures)


Controls without virtual reality devices:

Look up/down: arrow up/down or mouse movement
Turn left/right: arrow left/right or mouse movement
Walk foraward/back: W/S keys or buttons mouse left/right
Start: press space
Show/hied arrow mouse:  pause key (Mouse movements work with hide arrow mode)
Setting roll position for data glove: press L

Naturally you won't need most of these commands, since you can control the game, using the body movements according to the virtual reality device used. For more information see the interactions described in the paper: "Foot navigation technique for first person shooting games" found in GEMS 7 book. 


===================
EOF

