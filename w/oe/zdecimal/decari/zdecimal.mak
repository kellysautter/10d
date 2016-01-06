#
# Change Log
# 2001.03.21  DGC
#     Change that added sironroot_c broke the compile for QuinSoft.  I made some
#     changes to fix it.
# 2001.03.07  RG
#     - Remove build pdb for releaseversion
# 2001.03.07  SR  Z2000
#     Remove link flags: /debug /pdbtype:con because it cause errors
#
# 2000.02.06  SR  Z2000
#     Add link flags: /debug /pdbtype:con
#     to get debug information for release mode
#     Change pdbtype from 'sept' to 'con'.
#
# 2001.01.12  RG   Z2000
#     Build PDB-File in release mode too
# 2000.02.14  RG  Z2000
#     Copy DLL to directory ../w/bin
# 2000.01.05  RG  Z2000  New directories for decari
#     decari.c and decari.h are moved to SironVobs on
#     directories: sironroot_c and sironroot_c/inc
# 1999.12.01  RG  Z2000  Bugfixing
#     We need option /Zp too
# 1999.11.29  SR  Z2000  Compile directives
#     Remove "/D __WIN32__" when defining CPP_PROJ (Debug and Release) because it
#     will also be defined by TB_NT
# 1999.11.09  RG  Z2000  MFC-DebugDLL's
#     Changed the option /MT,MTd to MD so that no MFC-Debug DLL's
#     are linked
#
# Microsoft Developer Studio Generated NMAKE File, Based on zdecimal.dsp
!IF "$(CFG)" == ""
CFG=zdecimal - Win32 Debug
!MESSAGE No configuration specified. Defaulting to zdecimal - Win32 Debug.
!ENDIF

!IF "$(CFG)" != "zdecimal - Win32 Release" && "$(CFG)" != "zdecimal - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "zdecimal.mak" CFG="zdecimal - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "zdecimal - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zdecimal - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

DecariC=..\..\..\..\..\sironroot_c
DecariInc=..\..\..\..\..\sironroot_c\inc

!IF !EXISTS($(DecariC)/$(NULL))
DecariC=..\..\..\..\a\oe\zdecimal\decari
DecariInc=..\..\..\..\a\oe\zdecimal\decari
!ENDIF

!IF  "$(CFG)" == "zdecimal - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0"

ALL : "..\..\..\bin\zdecimal.dll"

!ELSE

ALL : "..\..\..\bin\zdecimal.dll"

!ENDIF

CLEAN :
   -@erase "$(INTDIR)\decari.obj"
   -@erase "$(INTDIR)\vc50.idb"
   -@erase "$(INTDIR)\ZDECIMAL.obj"
   -@erase "..\..\..\bin\zdecimal.dll"
   -@erase "$(OUTDIR)\zdecimal.exp"
   -@erase "..\..\..\libz\zdecimal.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /O2 /Zp /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TB_NT" \
 /Fp"$(INTDIR)\zdecimal.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\zdecimal.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\zdecimal.pdb" /machine:I386 /def:".\ZDECIMAL.def"\
 /out:"..\..\..\bin\zdecimal.dll" /implib:"..\..\..\libz\zdecimal.lib"
DEF_FILE= \
   ".\ZDECIMAL.def"
LINK32_OBJS= \
   "$(INTDIR)\decari.obj" \
   "$(INTDIR)\ZDECIMAL.obj"

"..\..\..\bin\zdecimal.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "zdecimal - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0"

ALL : "..\..\..\bin\zdecimal.dll"

!ELSE

ALL : "..\..\..\bin\zdecimal.dll"

!ENDIF

CLEAN :
   -@erase "$(INTDIR)\decari.obj"
   -@erase "$(INTDIR)\vc60.idb"
   -@erase "$(INTDIR)\vc60.pdb"
   -@erase "$(INTDIR)\ZDECIMAL.obj"
   -@erase "..\..\..\bin\zdecimal.dll"
   -@erase "$(OUTDIR)\zdecimal.exp"
   -@erase "$(OUTDIR)\zdecimal.ilk"
   -@erase "..\..\..\libz\zdecimal.lib"
   -@erase "..\..\..\bin\zdecimal.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /EHsc /Zp /Zi /Od /I $(DecariInc) \
 /I "..\..\..\incr" /I "..\..\..\..\a\incr" /I "..\..\..\..\a\incz" /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TB_NT"\
 /Fp"$(INTDIR)\zdecimal.pch" /Fo"$(INTDIR)\\" /Fd"..\..\..\bin\zdecimal.pdb" /FD /c
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\zdecimal.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"..\..\..\bin\zdecimal.pdb" /debug /machine:I386 /def:".\ZDECIMAL.def"\
 /out:"..\..\..\bin\zdecimal.dll" /implib:"..\..\..\libz\zdecimal.lib"
DEF_FILE= \
   ".\ZDECIMAL.def"
LINK32_OBJS= \
   "$(INTDIR)\decari.obj" \
   "$(INTDIR)\ZDECIMAL.obj"

"..\..\..\bin\zdecimal.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF


!IF "$(CFG)" == "zdecimal - Win32 Release" || "$(CFG)" ==\
 "zdecimal - Win32 Debug"
SOURCE=$(DecariC)\decari.c
DEP_CPP_DECAR=$(DecariInc)


"$(INTDIR)\decari.obj" : $(SOURCE) $(DEP_CPP_DECAR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\a\oe\zdecimal\decari\zdecimal.c

!IF  "$(CFG)" == "zdecimal - Win32 Release"

NODEP_CPP_ZDECI=\
   "..\..\..\..\a\incr\zdecimal.h"\
   $(DecariInc)\decari.h\
   "..\..\..\incr\kzoeenaa.h"\


"$(INTDIR)\ZDECIMAL.obj" : $(SOURCE) $(NODEP_CPP_ZDECI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "zdecimal - Win32 Debug"

DEP_CPP_ZDECI=\
   "..\..\..\..\a\incr\zdecimal.h"\
   $(DecariInc)\decari.h\
   "..\..\..\incr\kzoeenaa.h"\


"$(INTDIR)\ZDECIMAL.obj" : $(SOURCE) $(DEP_CPP_ZDECI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF


!ENDIF

