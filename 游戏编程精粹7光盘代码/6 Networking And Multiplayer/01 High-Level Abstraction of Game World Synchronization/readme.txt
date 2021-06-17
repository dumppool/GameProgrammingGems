====================================================================
High Level Abstraction of Game World Synchronization
====================================================================

This sample code consists of these project files:

 antlrlib
 SWDComp
 MyHLA
 HLADemo

All of these files are tested and verified with Visual Studio or C++.net 2003.

------------------
antlrlib
------------------

antlrlib is a refined version of the original ANTLR C++ library 2.7.4, for convenient build
for Visual C++.net 2003. antlr.jar is ANTLR 2.7.4 runtime which takes grammar file and 
generates parser files in C++ or java.

ANTLR is copyrighted by Terence Parr. http://www.antlr.org/


------------------
SWDComp
------------------
SWDComp is a compiler which takes an SWD file and generates client and server side classes and runtimes
for our own HLA implementations.

SWDComp consists of two parts: front end and back end. The front end exists
in SWD.g. The back end exists Backend.cpp and Backend.h.

Java Runtime Environment 1.4(http://java.sun.com) and antlrlib are needed to compile 
this project, however, Java Runtime Environment is not needed to run SWDComp.exe.

------------------
MyHLA
------------------

MyHLA is a library for the supplement for SWD compilation output.
It is needed to add this library to your project when you adopt 'our own
HLA implementation.'

------------------
HLADemo
------------------

HLADemo is an interactive demo application of HLA. It shows synchronized entities
and network filtering based on user's viewport via pseudo networking layer. You can
create, move or destroy the knights(i.e. the synchronized game objects) as well as
the user's viewport.

Check out the generated code from MedivalWorld.SWD for examining how our own HLA can
be used.

You may want to check out HLADemo.CHM or out/Readme.rtf for more information.
