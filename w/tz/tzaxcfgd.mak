# Microsoft Developer Studio Generated NMAKE File, Based on tzaxcfgd.dsp
!IF "$(CFG)" == ""
CFG=tzaxcfgd - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tzaxcfgd - Win32 Debug.
!ENDIF

!IF "$(CFG)" != "tzaxcfgd - Win32 Release" && "$(CFG)" != "tzaxcfgd - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "tzaxcfgd.mak" CFG="tzaxcfgd - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "tzaxcfgd - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tzaxcfgd - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "tzaxcfgd - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\bin\tzaxcfgd.dll"


CLEAN :
   -@erase "$(INTDIR)\TZAXCFGD.obj"
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\vc60.pdb"
   -@erase "$(OUTDIR)\tzaxcfgd.exp"
   -@erase "$(OUTDIR)\tzaxcfgd.lib"
   -@erase "$(OUTDIR)\tzaxcfgd.pdb"
   -@erase "..\bin\tzaxcfgd.dll"
   -@erase "..\bin\tzaxcfgd.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /EHsc /Zi /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /D "_RELEASE" /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "_USE_DATASET_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP /c
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tzaxcfgd.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib truegrid.lib tzctl.lib tzlodopr.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tzaxcfgd.pdb" /debug /machine:I386 /def:"..\..\w\tz\tzaxcfgd.def" /out:"..\..\w\bin\tzaxcfgd.dll" /implib:"$(OUTDIR)\tzaxcfgd.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release"
DEF_FILE= \
   "..\..\w\tz\tzaxcfgd.def"
LINK32_OBJS= \
   "$(INTDIR)\TZAXCFGD.obj"

"..\bin\tzaxcfgd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tzaxcfgd - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\bin\tzaxcfgd.dll"


CLEAN :
   -@erase "$(INTDIR)\TZAXCFGD.obj"
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\vc60.pdb"
   -@erase "$(OUTDIR)\tzaxcfgd.exp"
   -@erase "$(OUTDIR)\tzaxcfgd.lib"
   -@erase "$(OUTDIR)\tzaxcfgd.pdb"
   -@erase "..\bin\tzaxcfgd.dll"
   -@erase "..\bin\tzaxcfgd.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /D "_DEBUG" /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "_USE_DATASET_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP /c
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tzaxcfgd.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib truegrid.lib tzctl.lib tzlodopr.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tzaxcfgd.pdb" /debug /machine:I386 /def:"..\..\w\tz\tzaxcfgd.def" /out:"..\..\w\bin\tzaxcfgd.dll" /implib:"$(OUTDIR)\tzaxcfgd.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release"
DEF_FILE= \
   "..\..\w\tz\tzaxcfgd.def"
LINK32_OBJS= \
   "$(INTDIR)\TZAXCFGD.obj"

"..\bin\tzaxcfgd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tzaxcfgd.dep")
!INCLUDE "tzaxcfgd.dep"
!ELSE
!MESSAGE Warning: cannot find "tzaxcfgd.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "tzaxcfgd - Win32 Release" || "$(CFG)" == "tzaxcfgd - Win32 Debug"
SOURCE=..\..\a\tz\TZAXCFGD.C

"$(INTDIR)\TZAXCFGD.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF

