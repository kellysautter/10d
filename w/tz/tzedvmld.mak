# Microsoft Developer Studio Generated NMAKE File, Based on tzedvmld.dsp
!IF "$(CFG)" == ""
CFG=tzedvmld - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tzedvmld - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tzedvmld - Win32 Release" && "$(CFG)" != "tzedvmld - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tzedvmld.mak" CFG="tzedvmld - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tzedvmld - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tzedvmld - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tzedvmld - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\bin\tzedvmld.dll"

!ELSE 

ALL : "tzvmledt - Win32 Release" "..\bin\tzedvmld.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tzvmledt - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\tzedvmld.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tzedvmld.exp"
	-@erase "$(OUTDIR)\tzedvmld.lib"
	-@erase "$(OUTDIR)\tzedvmld.pdb"
	-@erase "..\bin\tzedvmld.dll"
	-@erase "..\bin\tzedvmld.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /Zi /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /D "_RELEASE" /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WIN32__" /D "_USE_DATASET_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tzedvmld.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=tzvmledt.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib tzctl.lib tzlodopr.lib tzvmpopr.lib tzvmlopr.lib tzvmlip.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tzedvmld.pdb" /debug /machine:I386 /def:".\tzedvmld.def" /out:"..\..\w\bin\tzedvmld.dll" /implib:"$(OUTDIR)\tzedvmld.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release" 
DEF_FILE= \
	".\tzedvmld.def"
LINK32_OBJS= \
	"$(INTDIR)\tzedvmld.obj" \
	"..\libz\tzvmledt.lib"

"..\bin\tzedvmld.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tzedvmld - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\bin\tzedvmld.dll" "$(OUTDIR)\tzedvmld.bsc"

!ELSE 

ALL : "tzvmledt - Win32 Debug" "..\bin\tzedvmld.dll" "$(OUTDIR)\tzedvmld.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tzvmledt - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\tzedvmld.obj"
	-@erase "$(INTDIR)\tzedvmld.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tzedvmld.bsc"
	-@erase "$(OUTDIR)\tzedvmld.exp"
	-@erase "$(OUTDIR)\tzedvmld.lib"
	-@erase "$(OUTDIR)\tzedvmld.pdb"
	-@erase "..\bin\tzedvmld.dll"
	-@erase "..\bin\tzedvmld.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /D "_DEBUG" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "__WIN32__" /D "_USE_DATASET_" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tzedvmld.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\tzedvmld.sbr"

"$(OUTDIR)\tzedvmld.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=tzvmledt.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib tzctl.lib tzlodopr.lib tzvmpopr.lib tzvmlopr.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tzedvmld.pdb" /debug /machine:I386 /def:"..\..\w\tz\tzedvmld.def" /out:"..\..\w\bin\tzedvmld.dll" /implib:"$(OUTDIR)\tzedvmld.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release" 
LINK32_OBJS= \
	"$(INTDIR)\tzedvmld.obj" \
	"..\libz\tzvmledt.lib"

"..\bin\tzedvmld.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tzedvmld.dep")
!INCLUDE "tzedvmld.dep"
!ELSE 
!MESSAGE Warning: cannot find "tzedvmld.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tzedvmld - Win32 Release" || "$(CFG)" == "tzedvmld - Win32 Debug"
SOURCE=..\..\a\tz\tzedvmld.C

!IF  "$(CFG)" == "tzedvmld - Win32 Release"


"$(INTDIR)\tzedvmld.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "tzedvmld - Win32 Debug"


"$(INTDIR)\tzedvmld.obj"	"$(INTDIR)\tzedvmld.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "tzedvmld - Win32 Release"

"tzvmledt - Win32 Release" : 
   cd %KZD%:\%KZV%\a\TZ\ax\tzvmledt
   $(MAKE) /$(MAKEFLAGS) /F .\tzvmledt.mak CFG="tzvmledt - Win32 Release" 
   cd "..\..\..\..\W\TZ"

"tzvmledt - Win32 ReleaseCLEAN" : 
   cd %KZD%:\%KZV%\a\TZ\ax\tzvmledt
   $(MAKE) /$(MAKEFLAGS) /F .\tzvmledt.mak CFG="tzvmledt - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\..\..\W\TZ"

!ELSEIF  "$(CFG)" == "tzedvmld - Win32 Debug"

"tzvmledt - Win32 Debug" : 
   cd %KZD%:\%KZV%\a\TZ\ax\tzvmledt
   $(MAKE) /$(MAKEFLAGS) /F .\tzvmledt.mak CFG="tzvmledt - Win32 Debug" 
   cd "..\..\..\..\W\TZ"

"tzvmledt - Win32 DebugCLEAN" : 
   cd %KZD%:\%KZV%\a\TZ\ax\tzvmledt
   $(MAKE) /$(MAKEFLAGS) /F .\tzvmledt.mak CFG="tzvmledt - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\..\..\W\TZ"

!ENDIF 


!ENDIF 

