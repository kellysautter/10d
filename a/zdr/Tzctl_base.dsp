# Microsoft Developer Studio Project File - Name="TzCtl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TzCtl - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "TzCtl.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "TzCtl.mak" CFG="TzCtl - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "TzCtl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TzCtl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TzCtl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\TzCtl\Release"
# PROP BASE Intermediate_Dir ".\TzCtl\Release"
# PROP BASE Target_Dir ".\TzCtl"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\TzCtl"
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "_AFXEXT" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ZdCtl.lib ZDrApp.lib kzoengaa.lib ..\..\w\libz\tzcm_opr.lib /nologo /subsystem:windows /dll /map /machine:I386 /out:"..\..\w\bin\TzCtl.dll" /implib:"..\..\w\libr\TzCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TzCtl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\TzCtl\Debug"
# PROP BASE Intermediate_Dir ".\TzCtl\Debug"
# PROP BASE Target_Dir ".\TzCtl"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\TzCtl"
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "_AFXEXT" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\Debug/ZDr.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ZdCtl.lib ZDrApp.lib kzoengaa.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\w\bin\TzCtl.dll" /implib:"..\..\w\libr\TzCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
# SUBTRACT LINK32 /pdb:none

!ENDIF

# Begin Target

# Name "TzCtl - Win32 Release"
# Name "TzCtl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\TzCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtl.def
# End Source File
# Begin Source File

SOURCE=.\TzCtl.rc
# End Source File
# Begin Source File

SOURCE=.\TzCtlATX.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlCVT.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlERD.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlERE.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlERL.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlERV.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlOPR.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTB.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTC.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTE.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTM.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTR.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTS.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTU.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlPTX.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlTAB.cpp
# End Source File
# Begin Source File

SOURCE=.\TzCtlTBE.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;inl;fi;fd"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\TzCtl.h
# End Source File
# Begin Source File

SOURCE=.\TzCtlRes.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap.bmp
# End Source File
# Begin Source File

SOURCE=.\bmpctl.bmp
# End Source File
# Begin Source File

SOURCE=.\check.bmp
# End Source File
# Begin Source File

SOURCE=.\combo.bmp
# End Source File
# Begin Source File

SOURCE=.\dil.bmp
# End Source File
# Begin Source File

SOURCE=.\edit.bmp
# End Source File
# Begin Source File

SOURCE=.\graph.bmp
# End Source File
# Begin Source File

SOURCE=.\group.bmp
# End Source File
# Begin Source File

SOURCE=.\grp.bmp
# End Source File
# Begin Source File

SOURCE=.\grpset.bmp
# End Source File
# Begin Source File

SOURCE=.\hierdgrm.bmp
# End Source File
# Begin Source File

SOURCE=.\icon.bmp
# End Source File
# Begin Source File

SOURCE=.\list.bmp
# End Source File
# Begin Source File

SOURCE=.\mle.bmp
# End Source File
# Begin Source File

SOURCE=.\outliner.bmp
# End Source File
# Begin Source File

SOURCE=.\palette.bmp
# End Source File
# Begin Source File

SOURCE=.\push.bmp
# End Source File
# Begin Source File

SOURCE=.\radio.bmp
# End Source File
# Begin Source File

SOURCE=.\shape.bmp
# End Source File
# Begin Source File

SOURCE=.\ss.bmp
# End Source File
# Begin Source File

SOURCE=.\tab.bmp
# End Source File
# Begin Source File

SOURCE=.\text.bmp
# End Source File
# End Group
# End Target
# End Project
