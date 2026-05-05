# Microsoft Developer Studio Generated NMAKE File, Based on buildlplr.dsp
!IF "$(CFG)" == ""
CFG=buildlplr - Win32 Release
!MESSAGE No configuration specified. Defaulting to buildlplr - Win32 Release.
!ENDIF

!IF "$(CFG)" != "buildlplr - Win32 Release" && "$(CFG)" != "buildlplr - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "buildlplr.mak" CFG="buildlplr - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "buildlplr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "buildlplr - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "buildlplr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\w\bin\buildlplr.exe"


CLEAN :
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\buildlplr.obj"
   -@erase "..\..\..\w\bin\buildlplr.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /EHsc /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /Fp"$(INTDIR)\buildlplr.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\buildlplr.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kzoengaa.lib tzlodopr.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\buildlplr.pdb" /machine:I386 /out:"..\..\..\w\bin\buildlplr.exe" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
LINK32_OBJS= \
   "$(INTDIR)\buildlplr.obj"

"..\..\..\w\bin\buildlplr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "buildlplr - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\w\bin\buildlplr.exe"


CLEAN :
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\vc60.pdb"
   -@erase "$(INTDIR)\buildlplr.obj"
   -@erase "$(OUTDIR)\buildlplr.pdb"
   -@erase "..\..\..\w\bin\buildlplr.exe"
   -@erase "..\..\..\w\bin\buildlplr.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /Gm /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\buildlplr.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\buildlplr.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kzoengaa.lib tzlodopr.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\buildlplr.pdb" /debug /machine:I386 /out:"..\..\..\w\bin\buildlplr.exe" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr"
LINK32_OBJS= \
   "$(INTDIR)\buildlplr.obj"

"..\..\..\w\bin\buildlplr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("buildlplr.dep")
!INCLUDE "buildlplr.dep"
!ELSE
!MESSAGE Warning: cannot find "buildlplr.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "buildlplr - Win32 Release" || "$(CFG)" == "buildlplr - Win32 Debug"
SOURCE=.\buildlplr.c

!IF  "$(CFG)" == "buildlplr - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /EHsc /O2 /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__WIN32__" /Fp"$(INTDIR)\buildlplr.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c

"$(INTDIR)\buildlplr.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "buildlplr - Win32 Debug"

CPP_SWITCHES=/nologo /ML /W3 /Gm /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\tz" /I ".\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__WIN32__" /Fp"$(INTDIR)\buildlplr.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c

"$(INTDIR)\buildlplr.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF


!ENDIF

