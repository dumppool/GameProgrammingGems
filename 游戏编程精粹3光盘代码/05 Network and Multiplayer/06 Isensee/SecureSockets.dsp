# Microsoft Developer Studio Project File - Name="SecureSockets" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SecureSockets - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SecureSockets.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SecureSockets.mak" CFG="SecureSockets - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SecureSockets - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SecureSockets - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "SecureSockets - Win32 Profile" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SecureSockets"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SecureSockets - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /Gz /W4 /GX /Zi /O2 /Ob2 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x0500 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib ws2_32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "SecureSockets - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D _MBCS_WIN32_WINNT=0x0500 /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x0500 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib advapi32.lib ws2_32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "SecureSockets - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SecureSockets___Win32_Profile"
# PROP BASE Intermediate_Dir "SecureSockets___Win32_Profile"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Profile"
# PROP Intermediate_Dir "Profile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W4 /GX /O2 /Ob2 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x0500 /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gz /W4 /GX /O2 /Ob2 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x0500 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib advapi32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 kernel32.lib user32.lib advapi32.lib ws2_32.lib /nologo /subsystem:console /profile /machine:I386
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "SecureSockets - Win32 Release"
# Name "SecureSockets - Win32 Debug"
# Name "SecureSockets - Win32 Profile"
# Begin Source File

SOURCE=.\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\Crypto.cpp
# End Source File
# Begin Source File

SOURCE=.\Crypto.h
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\SADatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\SADatabase.h
# End Source File
# Begin Source File

SOURCE=.\SecureBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\SecureBuffer.h
# End Source File
# Begin Source File

SOURCE=.\SecurityAssociation.cpp
# End Source File
# Begin Source File

SOURCE=.\SecurityAssociation.h
# End Source File
# Begin Source File

SOURCE=.\TestCrypto.cpp
# End Source File
# End Target
# End Project
