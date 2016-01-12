# Microsoft Developer Studio Generated NMAKE File, Based on tbedit.dsp
!IF "$(CFG)" == ""
CFG=tbedit - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tbedit - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tbedit - Win32 Release" && "$(CFG)" != "tbedit - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tbedit.mak" CFG="tbedit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tbedit - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tbedit - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tbedit - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\w\bin\tbedit.ocx" "$(OUTDIR)\tbedit.tlb" ".\Release\regsvr32.trg"

!ELSE 

ALL : "..\..\..\..\w\bin\tbedit.ocx" "$(OUTDIR)\tbedit.tlb" ".\Release\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\colpick.obj"
	-@erase "$(INTDIR)\colpopup.obj"
	-@erase "$(INTDIR)\hzocregs.obj"
	-@erase "$(INTDIR)\hzoectl.obj"
	-@erase "$(INTDIR)\hzoectle.res"
	-@erase "$(INTDIR)\hzoedlg.obj"
	-@erase "$(INTDIR)\hzoefunc.obj"
	-@erase "$(INTDIR)\hzoemeth.obj"
	-@erase "$(INTDIR)\hzoeprop.obj"
	-@erase "$(INTDIR)\hzoeundo.obj"
	-@erase "$(INTDIR)\hzoezsyn.obj"
	-@erase "$(INTDIR)\hzoregex.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\tbedit.obj"
	-@erase "$(INTDIR)\tbedit.tlb"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tbedit.exp"
	-@erase "$(OUTDIR)\tbedit.lib"
	-@erase "..\..\..\..\w\bin\tbedit.ocx"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /ZI /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\_common\_class" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x407 /fo"$(INTDIR)\hzoectle.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tbedit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"..\..\..\..\w\bin\tbedit.pdb" /machine:I386 /def:".\hzoectl.def" /out:"..\..\..\..\w\bin\tbedit.ocx" /implib:"$(OUTDIR)\tbedit.lib" 
DEF_FILE= \
	".\hzoectl.def"
LINK32_OBJS= \
	"$(INTDIR)\colpick.obj" \
	"$(INTDIR)\colpopup.obj" \
	"$(INTDIR)\hzocregs.obj" \
	"$(INTDIR)\hzoectl.obj" \
	"$(INTDIR)\hzoedlg.obj" \
	"$(INTDIR)\hzoefunc.obj" \
	"$(INTDIR)\hzoemeth.obj" \
	"$(INTDIR)\hzoeprop.obj" \
	"$(INTDIR)\hzoeundo.obj" \
	"$(INTDIR)\hzoezsyn.obj" \
	"$(INTDIR)\hzoregex.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\tbedit.obj" \
	"$(INTDIR)\hzoectle.res" \
	"..\..\..\..\w\libr\TzCtl.lib" \
	"..\..\..\..\w\libr\ZdCtl.lib" \
	"..\..\..\..\w\libr\ZDrApp.lib"

"..\..\..\..\w\bin\tbedit.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\$(KZV)\w\bin\tbedit.ocx
InputPath=\$(KZV)\w\bin\tbedit.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OUTDIR)\regsvr32.trg"
<< 
	

!ELSEIF  "$(CFG)" == "tbedit - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\w\bin\tbedit.ocx" ".\Debug\regsvr32.trg"

!ELSE 

ALL : "..\..\..\..\w\bin\tbedit.ocx" ".\Debug\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\colpick.obj"
	-@erase "$(INTDIR)\colpopup.obj"
	-@erase "$(INTDIR)\hzocregs.obj"
	-@erase "$(INTDIR)\hzoectl.obj"
	-@erase "$(INTDIR)\hzoectle.res"
	-@erase "$(INTDIR)\hzoedlg.obj"
	-@erase "$(INTDIR)\hzoefunc.obj"
	-@erase "$(INTDIR)\hzoemeth.obj"
	-@erase "$(INTDIR)\hzoeprop.obj"
	-@erase "$(INTDIR)\hzoeundo.obj"
	-@erase "$(INTDIR)\hzoezsyn.obj"
	-@erase "$(INTDIR)\hzoregex.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\tbedit.obj"
	-@erase "$(INTDIR)\tbedit.pch"
	-@erase "$(INTDIR)\tbedit.tlb"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tbedit.exp"
	-@erase "$(OUTDIR)\tbedit.lib"
	-@erase "..\..\..\..\w\bin\tbedit.pdb"
	-@erase "..\..\..\..\w\bin\tbedit.ilk"
	-@erase "..\..\..\..\w\bin\tbedit.ocx"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /EHsc /ZI /Od /Gm /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_NOANCHOR" /Fp"$(INTDIR)\tbedit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /I "$(KZD):\$(KZV)\_common\_class" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x407 /fo"$(INTDIR)\hzoectle.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tbedit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"..\..\..\..\w\bin\tbedit.pdb" /debug /machine:I386 /def:".\hzoectl.def" /out:"..\..\..\..\w\bin\tbedit.ocx" /implib:"$(OUTDIR)\tbedit.lib"
DEF_FILE= \
	".\hzoectl.def"
LINK32_OBJS= \
	"$(INTDIR)\colpick.obj" \
	"$(INTDIR)\colpopup.obj" \
	"$(INTDIR)\hzocregs.obj" \
	"$(INTDIR)\hzoectl.obj" \
	"$(INTDIR)\hzoedlg.obj" \
	"$(INTDIR)\hzoefunc.obj" \
	"$(INTDIR)\hzoemeth.obj" \
	"$(INTDIR)\hzoeprop.obj" \
	"$(INTDIR)\hzoeundo.obj" \
	"$(INTDIR)\hzoezsyn.obj" \
	"$(INTDIR)\hzoregex.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\tbedit.obj" \
	"$(INTDIR)\hzoectle.res" \
	"..\..\..\..\w\libr\TzCtl.lib" \
	"..\..\..\..\w\libr\ZdCtl.lib" \
	"..\..\..\..\w\libr\ZDrApp.lib"

"..\..\..\..\w\bin\tbedit.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\$(KZV)\w\bin\tbedit.ocx
InputPath=\$(KZV)\w\bin\tbedit.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OUTDIR)\regsvr32.trg"
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tbedit.dep")
!INCLUDE "tbedit.dep"
!ELSE 
!MESSAGE Warning: cannot find "tbedit.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tbedit - Win32 Release" || "$(CFG)" == "tbedit - Win32 Debug"
SOURCE=..\..\..\..\_common\_class\colpick.cpp

!IF  "$(CFG)" == "tbedit - Win32 Release"

CPP_SWITCHES=/nologo /MD  /W3 /EHsc /ZI /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\_common\_class" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /FD /c 

"$(INTDIR)\colpick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "tbedit - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /EHsc /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_NOANCHOR" /Fp"$(INTDIR)\tbedit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /I "$(KZD):\$(KZV)\_common\_class" /FD /c 

"$(INTDIR)\colpick.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\tbedit.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<

!ENDIF 

SOURCE=..\..\..\..\_common\_class\colpopup.cpp

"$(INTDIR)\colpopup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\_common\_class\hzocregs.cpp

"$(INTDIR)\hzocregs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\hzoectl.cpp

"$(INTDIR)\hzoectl.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hzoectle.rc

!IF  "$(CFG)" == "tbedit - Win32 Release"


"$(INTDIR)\hzoectle.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x407 /fo"$(INTDIR)\hzoectle.res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "tbedit - Win32 Debug"


"$(INTDIR)\hzoectle.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\tbedit.tlb"
	$(RSC) /l 0x407 /fo"$(INTDIR)\hzoectle.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\hzoedlg.cpp

"$(INTDIR)\hzoedlg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hzoefunc.cpp

"$(INTDIR)\hzoefunc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hzoemeth.cpp

"$(INTDIR)\hzoemeth.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hzoeprop.cpp

"$(INTDIR)\hzoeprop.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hzoeundo.cpp

"$(INTDIR)\hzoeundo.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hzoezsyn.cpp

"$(INTDIR)\hzoezsyn.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\..\..\_common\_class\hzoregex.cpp

"$(INTDIR)\hzoregex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\_common\_class\regex.cpp

!IF  "$(CFG)" == "tbedit - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /EHsc /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\_common\_class" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /FD /c 

"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "tbedit - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /EHsc /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_NOANCHOR" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /I "$(KZD):\$(KZV)\_common\_class" /FD /c 

"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\tbedit.cpp

!IF  "$(CFG)" == "tbedit - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /EHsc /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\_common\_class" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /FD /c 

"$(INTDIR)\tbedit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "tbedit - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /EHsc /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_NOANCHOR" /Fp"$(INTDIR)\tbedit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\a\tz\ax\_edit" /I "$(KZD):\$(KZV)\_common\_class" /FD /c

"$(INTDIR)\tbedit.obj"	"$(INTDIR)\tbedit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\tbedit.odl

!IF  "$(CFG)" == "tbedit - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\tbedit.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\tbedit.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "tbedit - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\tbedit.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\tbedit.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<

!ENDIF 


!ENDIF 

