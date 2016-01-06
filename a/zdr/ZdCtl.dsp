# Microsoft Developer Studio Project File - Name="ZdCtl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ZDCTL - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZdCtl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZdCtl.mak" CFG="ZDCTL - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZdCtl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ZdCtl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ZdCtl"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZdCtl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ZdCtl\Release"
# PROP BASE Intermediate_Dir ".\ZdCtl\Release"
# PROP BASE Target_Dir ".\ZdCtl"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\ZdCtl"
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "NDEBUG" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /Yu"ZdCtl.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ZDrApp.lib kzoengaa.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\w\bin\ZdCtl.dll" /implib:"..\..\w\libr\ZdCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ZdCtl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\ZdCtl\Debug"
# PROP BASE Intermediate_Dir ".\ZdCtl\Debug"
# PROP BASE Target_Dir ".\ZdCtl"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\ZdCtl"
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "_DEBUG" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR /Yu"ZdCtl.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\Debug/ZDr.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ZDrApp.lib kzoengaa.lib jpeglib.lib msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\w\bin\ZdCtl.dll" /implib:"..\..\w\libr\ZdCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ZdCtl - Win32 Release"
# Name "ZdCtl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ZdCtl.cpp
# ADD CPP /Yc"ZdCtl.h"
# End Source File
# Begin Source File

SOURCE=.\ZdCtl.def
# End Source File
# Begin Source File

SOURCE=.\ZdCtl.rc
# End Source File
# Begin Source File

SOURCE=.\ZdCtlATX.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlBar.CPP
# End Source File
# Begin Source File

SOURCE=.\ZdCtlBMB.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlBTN.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlCal.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlCHK.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlCMB.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlDIL.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlDT.CPP
# End Source File
# Begin Source File

SOURCE=.\ZdCtlEDT.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlGBX.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlHD.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlHDN.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlHot.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlICO.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlLBH.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlLBX.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlMLE.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlMST.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlO.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlOLB.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlRB.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlRTE.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlRTO.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlSS.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlSSU.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlTAB.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlTim.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlTXT.cpp
# End Source File
# Begin Source File

SOURCE=.\ZdCtlVWG.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;inl;fi;fd"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ZCtlRes.h
# End Source File
# Begin Source File

SOURCE=.\ZdCtl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\BCOPY1.cur
# End Source File
# Begin Source File

SOURCE=.\BCOPYM.cur
# End Source File
# Begin Source File

SOURCE=.\BDROPNOT.cur
# End Source File
# Begin Source File

SOURCE=.\BMOVE.cur
# End Source File
# Begin Source File

SOURCE=.\BMOVE1.cur
# End Source File
# Begin Source File

SOURCE=.\BMOVEM.cur
# End Source File
# Begin Source File

SOURCE=.\BPROMOTE.cur
# End Source File
# Begin Source File

SOURCE=.\CBRSV1.bmp
# End Source File
# Begin Source File

SOURCE=.\CBRSV2.bmp
# End Source File
# Begin Source File

SOURCE=.\CBRSV3.bmp
# End Source File
# Begin Source File

SOURCE=.\CBRSV4.bmp
# End Source File
# Begin Source File

SOURCE=.\CBRSV5.bmp
# End Source File
# Begin Source File

SOURCE=.\CBRSV6.bmp
# End Source File
# Begin Source File

SOURCE=.\DROPNOT.cur
# End Source File
# Begin Source File

SOURCE=.\DROPOKC1.cur
# End Source File
# Begin Source File

SOURCE=.\DROPOKCM.cur
# End Source File
# Begin Source File

SOURCE=.\DROPOKM1.cur
# End Source File
# Begin Source File

SOURCE=.\DROPOKMM.cur
# End Source File
# Begin Source File

SOURCE=.\HAND1.cur
# End Source File
# Begin Source File

SOURCE=.\HSPLIT.cur
# End Source File
# Begin Source File

SOURCE=.\STATEICO.bmp
# End Source File
# Begin Source File

SOURCE=.\treeclse.bmp
# End Source File
# Begin Source File

SOURCE=.\treedflt.ico
# End Source File
# Begin Source File

SOURCE=.\treeopen.bmp
# End Source File
# Begin Source File

SOURCE=.\treeopen.ico
# End Source File
# Begin Source File

SOURCE=.\treesel.bmp
# End Source File
# Begin Source File

SOURCE=.\treesel.ico
# End Source File
# Begin Source File

SOURCE=.\VSPLIT.cur
# End Source File
# Begin Source File

SOURCE=.\XSPLIT.cur
# End Source File
# End Group
# End Target
# End Project
