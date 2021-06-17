# Microsoft Developer Studio Project File - Name="ase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ase.mak" CFG="ase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ase - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ase - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ase", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ase - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386

!ELSEIF  "$(CFG)" == "ase - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ase - Win32 Release"
# Name "ase - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ase.cpp
# End Source File
# Begin Source File

SOURCE=.\ase.rc
# End Source File
# Begin Source File

SOURCE=.\ase_board.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_coordpair.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_costs.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_losapproxboard.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_losscan.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_piescan.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_ray.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_search.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_threat.cpp
# End Source File
# Begin Source File

SOURCE=.\ase_threatboard.cpp
# End Source File
# Begin Source File

SOURCE=.\aseDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\aseView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeView.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ase.h
# End Source File
# Begin Source File

SOURCE=.\ase_board.h
# End Source File
# Begin Source File

SOURCE=.\ase_coordpair.h
# End Source File
# Begin Source File

SOURCE=.\ase_costs.h
# End Source File
# Begin Source File

SOURCE=.\ase_losapproxboard.h
# End Source File
# Begin Source File

SOURCE=.\ase_losscan.h
# End Source File
# Begin Source File

SOURCE=.\ase_piescan.h
# End Source File
# Begin Source File

SOURCE=.\ase_ray.h
# End Source File
# Begin Source File

SOURCE=.\ase_search.h
# End Source File
# Begin Source File

SOURCE=.\ase_terrain.h
# End Source File
# Begin Source File

SOURCE=.\ase_threat.h
# End Source File
# Begin Source File

SOURCE=.\ase_threatboard.h
# End Source File
# Begin Source File

SOURCE=.\aseDoc.h
# End Source File
# Begin Source File

SOURCE=.\aseView.h
# End Source File
# Begin Source File

SOURCE=.\asincludes.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NodeView.h
# End Source File
# Begin Source File

SOURCE=.\PathFinder.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ase.ico
# End Source File
# Begin Source File

SOURCE=.\res\ase.rc2
# End Source File
# Begin Source File

SOURCE=.\res\aseDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
