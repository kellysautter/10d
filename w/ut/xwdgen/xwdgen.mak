# Microsoft Developer Studio Generated NMAKE File, Based on xwdgen.dsp
!IF "$(CFG)" == ""
CFG=xwdgen - Win32 Release
!MESSAGE No configuration specified. Defaulting to xwdgen - Win32 Release.
!ENDIF

!IF "$(CFG)" != "xwdgen - Win32 Release" && "$(CFG)" != "xwdgen - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "xwdgen.mak" CFG="xwdgen - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "xwdgen - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "xwdgen - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xwdgen - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\w\bin\xwdgen.exe"


CLEAN :
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\xwdgen.obj"
   -@erase "..\..\w\bin\xwdgen.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /EHsc /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /Fp"$(INTDIR)\xwdgen.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\xwdgen.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=ZdrApp.lib kzoengaa.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\xwdgen.pdb" /machine:I386 /out:"..\..\w\bin\xwdgen.exe" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
LINK32_OBJS= \
   "$(INTDIR)\xwdgen.obj"

"..\..\w\bin\xwdgen.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "xwdgen - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\w\bin\xwdgen.exe"


CLEAN :
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\vc60.pdb"
   -@erase "$(INTDIR)\xwdgen.obj"
   -@erase "$(OUTDIR)\xwdgen.pdb"
   -@erase "..\..\w\bin\xwdgen.exe"
   -@erase "..\..\w\bin\xwdgen.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /Gm /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\xwdgen.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\xwdgen.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=ZdrApp.lib kzoengaa.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\xwdgen.pdb" /debug /machine:I386 /out:"..\..\w\bin\xwdgen.exe" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
LINK32_OBJS= \
   "$(INTDIR)\xwdgen.obj"

"..\..\w\bin\xwdgen.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("xwdgen.dep")
!INCLUDE "xwdgen.dep"
!ELSE
!MESSAGE Warning: cannot find "xwdgen.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "xwdgen - Win32 Release" || "$(CFG)" == "xwdgen - Win32 Debug"
SOURCE=.\xwdgen.c

!IF  "$(CFG)" == "xwdgen - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /EHsc /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__WIN32__" /Fp"$(INTDIR)\xwdgen.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c

"$(INTDIR)\xwdgen.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "xwdgen - Win32 Debug"

CPP_SWITCHES=/nologo /ML /W3 /Gm /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__WIN32__" /Fp"$(INTDIR)\xwdgen.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c

"$(INTDIR)\xwdgen.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF


!ENDIF

