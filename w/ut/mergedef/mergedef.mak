# Microsoft Developer Studio Generated NMAKE File, Based on mergedef.dsp
!IF "$(CFG)" == ""
CFG=mergedef - Win32 Debug
!MESSAGE No configuration specified. Defaulting to mergedef - Win32 Debug.
!ENDIF

!IF "$(CFG)" != "mergedef - Win32 Release" && "$(CFG)" != "mergedef - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "mergedef.mak" CFG="mergedef - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "mergedef - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mergedef - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!IF  "$(CFG)" == "mergedef - Win32 Release"

OUTDIR=d:\b\bin
INTDIR=.\Release
# Begin Custom Macros
OutDir=d:\b\bin
# End Custom Macros

ALL : "$(OUTDIR)\mergedef.exe"


CLEAN :
   -@erase "$(INTDIR)\MERGEDEF.obj"
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(OUTDIR)\mergedef.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /EHsc /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\mergedef.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /D _CRT_SECURE_NO_WARNINGS /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mergedef.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\mergedef.pdb" /machine:I386 /out:"$(OUTDIR)\mergedef.exe"
LINK32_OBJS= \
   "$(INTDIR)\MERGEDEF.obj"

"$(OUTDIR)\mergedef.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mergedef - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\mergedef.exe"


CLEAN :
   -@erase "$(INTDIR)\MERGEDEF.obj"
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\vc60.pdb"
   -@erase "$(OUTDIR)\mergedef.exe"
   -@erase "$(OUTDIR)\mergedef.ilk"
   -@erase "$(OUTDIR)\mergedef.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /EHsc /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\mergedef.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /D _CRT_SECURE_NO_WARNINGS /c

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mergedef.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\mergedef.pdb" /debug /machine:I386 /out:"$(OUTDIR)\mergedef.exe"
LINK32_OBJS= \
   "$(INTDIR)\MERGEDEF.obj"

"$(OUTDIR)\mergedef.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("mergedef.dep")
!INCLUDE "mergedef.dep"
!ELSE
!MESSAGE Warning: cannot find "mergedef.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "mergedef - Win32 Release" || "$(CFG)" == "mergedef - Win32 Debug"
SOURCE=.\MERGEDEF.cpp

"$(INTDIR)\MERGEDEF.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF

