# Microsoft Developer Studio Generated NMAKE File, Based on msdbgrid.dsp
# -----------------------------------------------------------------------
#
# Change Log
#   most recent first order
#
# YYYY.MM.DD  who  version
#
# 2000.07.13  RG,TMV   Z2000
#     remove /D _AFXDLL for releaseversion
# 2000.04.13  RG   Z2000
#     Build the DLL to directory ...\w\bin and the import lib to
#     directory ...\w\libr (same way like tzaxctl.dll and .lib)
# 2000.04.11  TMV  Z2000
#     created
#
#
#
#

!IF "$(DEBUG)" == "1"
CFG=msdbgrid - Win32 Debug
!ELSE
CFG=msdbgrid - Win32 Release
!ENDIF

!IF "$(CFG)" != "msdbgrid - Win32 Release" && "$(CFG)" != "msdbgrid - Win32 Debug"
CFG=msdbgrid - Win32 Debug
!ENDIF

!IF "$(CFG)" != "msdbgrid - Win32 Release" && "$(CFG)" != "msdbgrid - Win32 Debug"
!MESSAGE No configuration specified. Defaulting to msdbgrid - Win32 Debug.
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "msdbgrid.mak" CFG="msdbgrid - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "msdbgrid - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "msdbgrid - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!IF  "$(CFG)" == "msdbgrid - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\..\..\w\bin\msdbgrid.dll" "$(OUTDIR)\msdbgrid.bsc"


CLEAN :
   -@erase "$(INTDIR)\msdbgrid.obj"
   -@erase "$(INTDIR)\msdbgrid.sbr"
   -@erase "$(INTDIR)\MSDBMAP.obj"
   -@erase "$(INTDIR)\MSDBMAP.SBR"
   -@erase "$(INTDIR)\TZAXHLPR.obj"
   -@erase "$(INTDIR)\TZAXHLPR.SBR"
   -@erase "$(INTDIR)\vc50.idb"
   -@erase "..\..\..\..\w\bin\msdbgrid.bsc"
   -@erase "..\..\..\..\w\bin\msdbgrid.dll"
   -@erase "..\..\..\..\w\libr\msdbgrid.exp"
   -@erase "..\..\..\..\w\libr\msdbgrid.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /D "_AFXEXT" /D "_RELEASE" /D "WIN32" /D "_WINDOWS" /D "__WIN32__" /D "_WINDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msdbgrid.bsc"
BSC32_SBRS= \
   "$(INTDIR)\msdbgrid.sbr" \
   "$(INTDIR)\MSDBMAP.SBR" \
   "$(INTDIR)\TZAXHLPR.SBR"

"$(OUTDIR)\msdbgrid.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\msdbgrid.pdb" \
   /machine:I386 /def:".\msdbgrid.def" /out:"..\..\..\..\w\bin\msdbgrid.dll" /implib:"..\..\..\..\w\libr\msdbgrid.lib" \
   /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release"
DEF_FILE= \
   ".\msdbgrid.def"
LINK32_OBJS= \
   "$(INTDIR)\msdbgrid.obj" \
   "$(INTDIR)\MSDBMAP.obj" \
   "$(INTDIR)\TZAXHLPR.obj"

"..\..\..\..\w\bin\msdbgrid.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msdbgrid - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\w\bin\msdbgrid.dll" "$(OUTDIR)\msdbgrid.bsc"


CLEAN :
   -@erase "$(INTDIR)\msdbgrid.obj"
   -@erase "$(INTDIR)\msdbgrid.sbr"
   -@erase "$(INTDIR)\MSDBMAP.obj"
   -@erase "$(INTDIR)\MSDBMAP.SBR"
   -@erase "$(INTDIR)\TZAXHLPR.obj"
   -@erase "$(INTDIR)\TZAXHLPR.SBR"
   -@erase "$(INTDIR)\vc50.idb"
   -@erase "$(INTDIR)\vc50.pdb"
   -@erase "$(OUTDIR)\msdbgrid.bsc"
   -@erase "..\..\..\..\w\bin\msdbgrid.dll"
   -@erase "..\..\..\..\w\libr\msdbgrid.exp"
   -@erase "..\..\..\..\w\bin\msdbgrid.ilk"
   -@erase "..\..\..\..\w\libr\msdbgrid.lib"
   -@erase "$(OUTDIR)\msdbgrid.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /EHsc /ZI /Od /I "c:\zocx\src" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /D "_AFXEXT" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__WIN32__" /D "_WINDLL" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msdbgrid.bsc"
BSC32_SBRS= \
   "$(INTDIR)\msdbgrid.sbr" \
   "$(INTDIR)\MSDBMAP.SBR" \
   "$(INTDIR)\TZAXHLPR.SBR"

"$(OUTDIR)\msdbgrid.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /dll /incremental:yes \
  /pdb:"$(OUTDIR)\msdbgrid.pdb" /debug /machine:I386 /def:".\msdbgrid.def" \
  /out:"..\..\..\..\w\bin\msdbgrid.dll" /implib:"..\..\..\..\w\libr\msdbgrid.lib" \
  /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\debug"
DEF_FILE= \
   ".\msdbgrid.def"
LINK32_OBJS= \
   "$(INTDIR)\msdbgrid.obj" \
   "$(INTDIR)\MSDBMAP.obj" \
   "$(INTDIR)\TZAXHLPR.obj"

"..\..\..\..\w\bin\msdbgrid.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("msdbgrid.dep")
!INCLUDE "msdbgrid.dep"
!ELSE
!MESSAGE Warning: cannot find "msdbgrid.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "msdbgrid - Win32 Release" || "$(CFG)" == "msdbgrid - Win32 Debug"
SOURCE=.\msdbgrid.cpp

"$(INTDIR)\msdbgrid.obj"   "$(INTDIR)\msdbgrid.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MSDBMAP.cpp

"$(INTDIR)\MSDBMAP.obj"   "$(INTDIR)\MSDBMAP.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=..\tzaxctl\tzaxhlpr.cpp

"$(INTDIR)\TZAXHLPR.obj"   "$(INTDIR)\TZAXHLPR.SBR" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF

