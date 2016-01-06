# Microsoft Developer Studio Project File - Name="tzvmledt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=tzvmledt - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "tzvmledt.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "tzvmledt.mak" CFG="tzvmledt - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "tzvmledt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tzvmledt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "tzvmledt"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tzvmledt - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "tzvmledt_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /I "$(KZD):\$(KZV)\a\tz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\zdr" /D "_RELEASE" /D "_WINDLL" /D "_USE_DATASET_" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "zGRID_COMPILE" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 tzlodopr.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib tzvmpopr.lib tzvmlopr.lib /nologo /dll /incremental:yes /debug /machine:I386 /out:"..\..\..\..\w\bin\tzvmledt.dll" /implib:"..\..\..\..\w\libz/tzvmledt.lib" /pdbtype:con /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "tzvmledt - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "tzvmledt_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "$(KZD):\$(KZV)\a\tz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\zdr" /D "_DEBUG" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "zGRID_COMPILE" /D "_NOANCHOR" /FR /YX"zstdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 tzlodopr.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib tzvmpopr.lib tzvmlopr.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\w\bin\tzvmledt.dll" /implib:"..\..\..\..\w\libz/tzvmledt.lib" /pdbtype:sept /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\debug"
# SUBTRACT LINK32 /pdb:none /incremental:no
# Begin Special Build Tool
ProjDir=.
TargetDir=\10d\w\bin
SOURCE="$(InputPath)"
PostBuild_Desc=copy ini settings
PostBuild_Cmds=copy "$(ProjDir)\*.ini" "$(TargetDir)\*.*"
# End Special Build Tool

!ENDIF

# Begin Target

# Name "tzvmledt - Win32 Release"
# Name "tzvmledt - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\tzcswrpr.cpp
# End Source File
# Begin Source File

SOURCE=.\tzvmledit.def
# End Source File
# Begin Source File

SOURCE=.\tzvmledt.cpp
# End Source File
# Begin Source File

SOURCE=.\zcmddef.cpp
# End Source File
# Begin Source File

SOURCE=.\zstring.cpp
# End Source File
# Begin Source File

SOURCE=.\zstring.h
# End Source File
# Begin Source File

SOURCE=.\zviewdef.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\tzcswrpr.h
# End Source File
# Begin Source File

SOURCE=.\tzvmlcst.h
# End Source File
# Begin Source File

SOURCE=.\tzvmledt.h
# End Source File
# Begin Source File

SOURCE=.\zcmddef.h
# End Source File
# Begin Source File

SOURCE=.\zviewdef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "others"

# PROP Default_Filter "mak; ini; pwd"
# Begin Source File

SOURCE=.\VMLLANG.INI
# End Source File
# End Group
# End Target
# End Project
