# Microsoft Developer Studio Project File - Name="ZDr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ZDr - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Zdr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Zdr.mak" CFG="ZDr - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZDr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ZDr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZDr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ZDr_"
# PROP BASE Intermediate_Dir ".\ZDr_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "NDEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FD /c
# SUBTRACT CPP /WX /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ZDrApp.lib kzoengaa.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\w\bin\ZDr.exe" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"

!ELSEIF  "$(CFG)" == "ZDr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "." /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ZDrApp.lib kzoengaa.lib kzrpsrco.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\w\bin\ZDr.exe" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
# SUBTRACT LINK32 /pdb:none /incremental:no /map

!ENDIF 

# Begin Target

# Name "ZDr - Win32 Release"
# Name "ZDr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ZDr.cpp

!IF  "$(CFG)" == "ZDr - Win32 Release"

!ELSEIF  "$(CFG)" == "ZDr - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ZDr.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;inl;fi;fd"
# Begin Source File

SOURCE=.\ZdCtl.h
# End Source File
# Begin Source File

SOURCE=.\ZDCTLSS.H
# End Source File
# Begin Source File

SOURCE=.\ZDlgUnits.h
# End Source File
# Begin Source File

SOURCE=.\ZDr.h
# End Source File
# Begin Source File

SOURCE=.\ZDrApp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\abhorzup.bmp
# End Source File
# Begin Source File

SOURCE=.\abvertup.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ACCTBAR.BMP
# End Source File
# Begin Source File

SOURCE=.\ACTBARUP.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ActiveX.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\ALGNBTUP.bmp
# End Source File
# Begin Source File

SOURCE=.\ALGNLFUP.bmp
# End Source File
# Begin Source File

SOURCE=.\ALGNRTUP.bmp
# End Source File
# Begin Source File

SOURCE=.\ALGNTPUP.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Appoint.bmp
# End Source File
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

SOURCE=.\RES\CalBack.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\CalBkGnd.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\CalBtns.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Calendar.bmp
# End Source File
# Begin Source File

SOURCE=.\cbc.bmp
# End Source File
# Begin Source File

SOURCE=.\CBLED.bmp
# End Source File
# Begin Source File

SOURCE=.\cbx.bmp
# End Source File
# Begin Source File

SOURCE=.\CHANGEUP.bmp
# End Source File
# Begin Source File

SOURCE=.\CMAIN.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\color_ma.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\RES\DateBtns.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\DateTime.bmp
# End Source File
# Begin Source File

SOURCE=.\DLETUP.bmp
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

SOURCE=.\DROPOKM.CUR
# End Source File
# Begin Source File

SOURCE=.\DROPOKM1.cur
# End Source File
# Begin Source File

SOURCE=.\DROPOKMM.cur
# End Source File
# Begin Source File

SOURCE=.\EQHORZUP.bmp
# End Source File
# Begin Source File

SOURCE=.\EQVERTUP.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\GRID.bmp
# End Source File
# Begin Source File

SOURCE=.\Hand1.cur
# End Source File
# Begin Source File

SOURCE=.\hand2.cur
# End Source File
# Begin Source File

SOURCE=.\HandX.cur
# End Source File
# Begin Source File

SOURCE=.\HSPLIT.cur
# End Source File
# Begin Source File

SOURCE=.\RES\images.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\painter.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\palette.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Pinned.ico
# End Source File
# Begin Source File

SOURCE=.\prevend.bmp
# End Source File
# Begin Source File

SOURCE=.\prevhome.bmp
# End Source File
# Begin Source File

SOURCE=.\prevnext.bmp
# End Source File
# Begin Source File

SOURCE=.\prevone.bmp
# End Source File
# Begin Source File

SOURCE=.\prevprev.bmp
# End Source File
# Begin Source File

SOURCE=.\prevprin.bmp
# End Source File
# Begin Source File

SOURCE=.\prevtwo.bmp
# End Source File
# Begin Source File

SOURCE=.\prevzin.bmp
# End Source File
# Begin Source File

SOURCE=.\prevzout.bmp
# End Source File
# Begin Source File

SOURCE=.\printer.bmp
# End Source File
# Begin Source File

SOURCE=.\SIZEHTDN.bmp
# End Source File
# Begin Source File

SOURCE=.\SIZEHTUP.bmp
# End Source File
# Begin Source File

SOURCE=.\SIZEHWUP.bmp
# End Source File
# Begin Source File

SOURCE=.\SIZEWDUP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ToolbarX.bmp
# End Source File
# Begin Source File

SOURCE=.\TreeClosed.bmp
# End Source File
# Begin Source File

SOURCE=.\TreeOpened.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Unpinned.ico
# End Source File
# Begin Source File

SOURCE=.\VSPLIT.cur
# End Source File
# Begin Source File

SOURCE=.\RES\Workday.bmp
# End Source File
# Begin Source File

SOURCE=.\XSPLIT.cur
# End Source File
# Begin Source File

SOURCE=.\res\ZDr.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ZDrDoc.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ZDX.ICO
# End Source File
# End Group
# End Target
# End Project
