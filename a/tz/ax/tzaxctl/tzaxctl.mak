# Microsoft Developer Studio Generated NMAKE File, Based on TzAxCtl.dsp
!IF "$(CFG)" == ""
CFG=TzAxCtl - Win32 Release
!MESSAGE No configuration specified. Defaulting to TzAxCtl - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "TzAxCtl - Win32 Release" && "$(CFG)" != "TzAxCtl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TzAxCtl - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\w\bin\TzAxCtl.dll"

!ELSE 

ALL : "..\..\..\..\w\bin\TzAxCtl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\TzAxCal.obj"
	-@erase "$(INTDIR)\TzAxCtl.obj"
	-@erase "$(INTDIR)\TzAxCtl.pch"
	-@erase "$(INTDIR)\TzAxDate.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\..\..\w\bin\TzAxCtl.dll"
	-@erase "..\..\..\..\w\bin\TzAxCtl.ilk"
	-@erase "..\..\..\..\w\libr\TzAxCtl.exp"
	-@erase "..\..\..\..\w\libr\TzAxCtl.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /ZI /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /Fp"$(INTDIR)\TzAxCtl.pch" /Yu"zstdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TzAxCtl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\TzAxCtl.pdb" /machine:I386 /def:".\TzAxCtl.def" /out:"..\..\..\..\w\bin\TzAxCtl.dll" /implib:"..\..\..\..\w\libr\TzAxCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" 
DEF_FILE= \
	".\TzAxCtl.def"
LINK32_OBJS= \
	"$(INTDIR)\TzAxCal.obj" \
	"$(INTDIR)\TzAxCtl.obj" \
	"$(INTDIR)\TzAxDate.obj" \
	"..\..\..\..\w\libr\TzCtl.lib" \
	"..\..\..\..\w\libr\ZdCtl.lib" \
	"..\..\..\..\w\libr\ZDrApp.lib"

"..\..\..\..\w\bin\TzAxCtl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TzAxCtl - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\w\bin\TzAxCtl.dll" "$(OUTDIR)\TzAxCtl.bsc"

!ELSE 

ALL : "ZDrApp - Win32 Debug" "ZdCtl - Win32 Debug" "TzCtl - Win32 Debug" "..\..\..\..\w\bin\TzAxCtl.dll" "$(OUTDIR)\TzAxCtl.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TzCtl - Win32 DebugCLEAN" "ZdCtl - Win32 DebugCLEAN" "ZDrApp - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\TzAxCal.obj"
	-@erase "$(INTDIR)\TzAxCal.sbr"
	-@erase "$(INTDIR)\TzAxCtl.obj"
	-@erase "$(INTDIR)\TzAxCtl.pch"
	-@erase "$(INTDIR)\TzAxCtl.sbr"
	-@erase "$(INTDIR)\TzAxDate.obj"
	-@erase "$(INTDIR)\TzAxDate.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TzAxCtl.bsc"
	-@erase "$(OUTDIR)\TzAxCtl.pdb"
	-@erase "..\..\..\..\w\bin\TzAxCtl.dll"
	-@erase "..\..\..\..\w\bin\TzAxCtl.ilk"
	-@erase "..\..\..\..\w\libr\TzAxCtl.exp"
	-@erase "..\..\..\..\w\libr\TzAxCtl.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /EHsc /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TzAxCtl.pch" /Yu"zstdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TzAxCtl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\TzAxCal.sbr" \
	"$(INTDIR)\TzAxCtl.sbr" \
	"$(INTDIR)\TzAxDate.sbr"

"$(OUTDIR)\TzAxCtl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\TzAxCtl.pdb" /debug /machine:I386 /def:".\TzAxCtl.def" /out:"..\..\..\..\w\bin\TzAxCtl.dll" /implib:"..\..\..\..\w\libr\TzAxCtl.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" 
DEF_FILE= \
	".\TzAxCtl.def"
LINK32_OBJS= \
	"$(INTDIR)\TzAxCal.obj" \
	"$(INTDIR)\TzAxCtl.obj" \
	"$(INTDIR)\TzAxDate.obj" \
	"..\..\..\..\w\libr\TzCtl.lib" \
	"..\..\..\..\w\libr\ZdCtl.lib" \
	"..\..\..\..\w\libr\ZDrApp.lib"

"..\..\..\..\w\bin\TzAxCtl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TzAxCtl.dep")
!INCLUDE "TzAxCtl.dep"
!ELSE 
!MESSAGE Warning: cannot find "TzAxCtl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TzAxCtl - Win32 Release" || "$(CFG)" == "TzAxCtl - Win32 Debug"
SOURCE=.\TzAxCal.cpp

!IF  "$(CFG)" == "TzAxCtl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /EHsc /ZI /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /Fp"$(INTDIR)\TzAxCtl.pch" /Yc"zstdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\\" /FD /c 

"$(INTDIR)\TzAxCal.obj"	"$(INTDIR)\TzAxCtl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TzAxCtl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /EHsc /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TzAxCtl.pch" /Yc"zstdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I ".\." /FD /c 

"$(INTDIR)\TzAxCal.obj"	"$(INTDIR)\TzAxCal.sbr"	"$(INTDIR)\TzAxCtl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TzAxCtl.cpp

!IF  "$(CFG)" == "TzAxCtl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /EHsc /ZI /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /Fp"$(INTDIR)\TzAxCtl.pch" /Yu"zstdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\\"/FD /c 

"$(INTDIR)\TzAxCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TzAxCtl.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TzAxCtl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /EHsc /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TzAxCtl.pch" /Yu"zstdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I ".\." /FD /c 

"$(INTDIR)\TzAxCtl.obj"	"$(INTDIR)\TzAxCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TzAxCtl.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

