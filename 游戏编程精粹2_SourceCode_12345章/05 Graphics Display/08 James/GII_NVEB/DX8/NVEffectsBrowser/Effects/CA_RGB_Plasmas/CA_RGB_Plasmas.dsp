# Microsoft Developer Studio Project File - Name="CA_RGB_Plasmas" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CA_RGB_Plasmas - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CA_RGB_Plasmas.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CA_RGB_Plasmas.mak" CFG="CA_RGB_Plasmas - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CA_RGB_Plasmas - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CA_RGB_Plasmas - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CA_RGB_Plasmas"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CA_RGB_Plasmas_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\include" /I "..\..\shared" /I "..\..\..\CommonSrc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CA_RGB_Plasmas_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 d3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib winmm.lib /nologo /dll /machine:I386 /out:"CA_RGB_Plasmas.dll"

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CA_RGB_Plasmas_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /I "..\..\shared" /I "..\..\..\CommonSrc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CA_RGB_Plasmas_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3dx8d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /dll /debug /machine:I386 /out:"CA_RGB_Plasmas.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CA_RGB_Plasmas - Win32 Release"
# Name "CA_RGB_Plasmas - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CA_Water.cpp
# End Source File
# Begin Source File

SOURCE=..\..\shared\eb_property.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect_Plasmas.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\CommonSrc\NV_Error.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\CommonSrc\nvtex.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CA_Water.h
# End Source File
# Begin Source File

SOURCE=.\Constants.h
# End Source File
# Begin Source File

SOURCE=.\Effect_Plasmas.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\MouseUI.h
# End Source File
# Begin Source File

SOURCE=.\PixelConstants.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Pixel Shaders"

# PROP Default_Filter ".nvp"
# Begin Source File

SOURCE=.\ApplyForceShader.nvp

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# Begin Custom Build
InputPath=.\ApplyForceShader.nvp

"ApplyForceShader.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# Begin Custom Build
InputPath=.\ApplyForceShader.nvp

"ApplyForceShader.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ApplyVelocityShader.nvp

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# Begin Custom Build
InputPath=.\ApplyVelocityShader.nvp

"ApplyVelocityShader.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# Begin Custom Build
InputPath=.\ApplyVelocityShader.nvp

"ApplyVelocityShader.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EqualWeightCombine_PostMult.nvp

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# Begin Custom Build
InputPath=.\EqualWeightCombine_PostMult.nvp

"EqualWeightCombine_PostMult.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# Begin Custom Build
InputPath=.\EqualWeightCombine_PostMult.nvp

"EqualWeightCombine_PostMult.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NeighborForceCalc.nvp

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# Begin Custom Build
InputPath=.\NeighborForceCalc.nvp

"NeighborForceCalc.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# Begin Custom Build
InputPath=.\NeighborForceCalc.nvp

"NeighborForceCalc.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NeighborForceCalc2.nvp

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# Begin Custom Build
InputPath=.\NeighborForceCalc2.nvp

"NeighborForceCalc2.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# Begin Custom Build
InputPath=.\NeighborForceCalc2.nvp

"NeighborForceCalc2.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NormalMapCreate2_Scale.nvp

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# Begin Custom Build
InputPath=.\NormalMapCreate2_Scale.nvp

"NormalMapCreate2_Scale.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# Begin Custom Build
InputPath=.\NormalMapCreate2_Scale.nvp

"NormalMapCreate2_Scale.pso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Vertex Shaders"

# PROP Default_Filter ".nvv"
# Begin Source File

SOURCE=.\TexCoord_4_Offset.nvv

!IF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Release"

# Begin Custom Build
InputPath=.\TexCoord_4_Offset.nvv

"TexCoord_4_Offset.vso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "CA_RGB_Plasmas - Win32 Debug"

# Begin Custom Build
InputPath=.\TexCoord_4_Offset.nvv

"TexCoord_4_Offset.vso" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\nvasm.exe $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
