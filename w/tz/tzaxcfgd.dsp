# Microsoft Developer Studio Project File - Name="tzaxcfgd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=tzaxcfgd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "tzaxcfdg.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "tzaxcfdg.mak" CFG="tzaxcfgd - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "tzaxcfgd - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tzaxcfgd - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "tzaxcfgd"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tzaxcfgd - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "tzaxcfgd_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /D "_RELEASE" /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "_USE_DATASET_" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ZdCtl.lib ZdrApp.lib kzoengaa.lib truegrid.lib tzctl.lib tzlodopr.lib /nologo /dll /debug /machine:I386 /out:"..\..\w\bin\tzaxcfgd.dll" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "tzaxcfgd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\w\tz\debug"
# PROP Intermediate_Dir "..\..\w\tz\debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "tzaxcfgd_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /D "_AFXEXT" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /FR /FD /TP /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 truegrid.lib tzctl.lib tzlodopr.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /dll /debug /machine:I386 /out:"..\..\w\bin/tzaxcfgd.dll" /implib:"$(KZD):\$(KZV)\w\libr/tzaxcfgd.lib" /pdbtype:sept /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF

# Begin Target

# Name "tzaxcfgd - Win32 Release"
# Name "tzaxcfgd - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\A\TZ\TZAXCFGD.C
# End Source File
# Begin Source File

SOURCE=.\tzaxcfgd.def
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
