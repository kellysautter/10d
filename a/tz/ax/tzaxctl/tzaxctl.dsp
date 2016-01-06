# Microsoft Developer Studio Project File - Name="TzAxCtl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TzAxCtl - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "TzAxCtl.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "TzAxCtl.mak" CFG="TzAxCtl - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "TzAxCtl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TzAxCtl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TzAxCtl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\TzAxCtl\Release"
# PROP BASE Intermediate_Dir ".\TzAxCtl\Release"
# PROP BASE Target_Dir ".\TzAxCtl"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\TzAxCtl"
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\\" /D "NDEBUG" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /Yu"zstdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /out:"..\..\..\..\w\bin\TzAxCtl.dll" /implib:"..\..\..\..\w\libr\TzAxCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TzAxCtl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\TzAxCtl\Debug"
# PROP BASE Intermediate_Dir ".\TzAxCtl\Debug"
# PROP BASE Target_Dir ".\TzAxCtl"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\TzAxCtl"
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\\" /D "_DEBUG" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR /Yu"zstdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\w\bin\TzAxCtl.dll" /implib:"..\..\..\..\w\libr\TzAxCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF

# Begin Target

# Name "TzAxCtl - Win32 Release"
# Name "TzAxCtl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\TzAxCal.cpp

!IF  "$(CFG)" == "TzAxCtl - Win32 Release"

# ADD CPP /Yc"zstdafx.h"

!ELSEIF  "$(CFG)" == "TzAxCtl - Win32 Debug"

# ADD CPP /I ".\." /Yc"zstdafx.h"
# SUBTRACT CPP /I "..\\"

!ENDIF

# End Source File
# Begin Source File

SOURCE=.\TzAxCtl.cpp

!IF  "$(CFG)" == "TzAxCtl - Win32 Release"

!ELSEIF  "$(CFG)" == "TzAxCtl - Win32 Debug"

# ADD CPP /I ".\."
# SUBTRACT CPP /I "..\\"

!ENDIF

# End Source File
# Begin Source File

SOURCE=.\TzAxCtl.def
# End Source File
# Begin Source File

SOURCE=.\TzAxDate.cpp

!IF  "$(CFG)" == "TzAxCtl - Win32 Release"

!ELSEIF  "$(CFG)" == "TzAxCtl - Win32 Debug"

# ADD CPP /I ".\."
# SUBTRACT CPP /I "..\\"

!ENDIF

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
