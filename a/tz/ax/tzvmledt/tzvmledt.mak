# Microsoft Developer Studio Generated NMAKE File, Based on tzvmledt.dsp
!IF "$(CFG)" == ""
CFG=tzvmledt - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tzvmledt - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tzvmledt - Win32 Release" && "$(CFG)" != "tzvmledt - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tzvmledt.mak" CFG="tzvmledt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tzvmledt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tzvmledt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tzvmledt - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\..\w\bin\tzvmledt.dll"


CLEAN :
	-@erase "$(INTDIR)\tzcswrpr.obj"
	-@erase "$(INTDIR)\tzvmledt.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\zcmddef.obj"
	-@erase "$(INTDIR)\zstring.obj"
	-@erase "$(INTDIR)\zviewdef.obj"
	-@erase "$(OUTDIR)\tzvmledt.pdb"
	-@erase "..\..\..\..\w\bin\tzvmledt.dll"
	-@erase "..\..\..\..\w\bin\tzvmledt.ilk"
	-@erase "..\..\..\..\w\libz\tzvmledt.exp"
	-@erase "..\..\..\..\w\libz\tzvmledt.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /ZI /Od /D "_RELEASE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "_USE_DATASET_" /D "zGRID_COMPILE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\a\tz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\zdr" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tzvmledt.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=tzlodopr.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib tzvmpopr.lib tzvmlopr.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tzvmledt.pdb" /debug /machine:I386 /def:".\tzvmledit.def" /out:"..\..\..\..\w\bin\tzvmledt.dll" /implib:"..\..\..\..\w\libz/tzvmledt.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release" 
DEF_FILE= \
	".\tzvmledit.def"
LINK32_OBJS= \
	"$(INTDIR)\tzcswrpr.obj" \
	"$(INTDIR)\tzvmledt.obj" \
	"$(INTDIR)\zcmddef.obj" \
	"$(INTDIR)\zstring.obj" \
	"$(INTDIR)\zviewdef.obj"

"..\..\..\..\w\bin\tzvmledt.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tzvmledt - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\w\bin\tzvmledt.dll" "$(OUTDIR)\tzvmledt.bsc"


CLEAN :
	-@erase "$(INTDIR)\tzcswrpr.obj"
	-@erase "$(INTDIR)\tzcswrpr.sbr"
	-@erase "$(INTDIR)\tzvmledt.obj"
	-@erase "$(INTDIR)\tzvmledt.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\zcmddef.obj"
	-@erase "$(INTDIR)\zcmddef.sbr"
	-@erase "$(INTDIR)\zstring.obj"
	-@erase "$(INTDIR)\zstring.sbr"
	-@erase "$(INTDIR)\zviewdef.obj"
	-@erase "$(INTDIR)\zviewdef.sbr"
	-@erase "$(OUTDIR)\tzvmledt.bsc"
	-@erase "$(OUTDIR)\tzvmledt.pdb"
	-@erase "..\..\..\..\w\bin\tzvmledt.dll"
	-@erase "..\..\..\..\w\bin\tzvmledt.ilk"
	-@erase "..\..\..\..\w\libz\tzvmledt.exp"
	-@erase "..\..\..\..\w\libz\tzvmledt.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GR /EHsc /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "zGRID_COMPILE" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\tzvmledt.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\a\tz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\zdr" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tzvmledt.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\tzcswrpr.sbr" \
	"$(INTDIR)\tzvmledt.sbr" \
	"$(INTDIR)\zcmddef.sbr" \
	"$(INTDIR)\zstring.sbr" \
	"$(INTDIR)\zviewdef.sbr"

"$(OUTDIR)\tzvmledt.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=tzlodopr.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib tzvmpopr.lib tzvmlopr.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tzvmledt.pdb" /debug /machine:I386 /def:".\tzvmledit.def" /out:"..\..\..\..\w\bin\tzvmledt.dll" /implib:"..\..\..\..\w\libz/tzvmledt.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\debug" 
DEF_FILE= \
	".\tzvmledit.def"
LINK32_OBJS= \
	"$(INTDIR)\tzcswrpr.obj" \
	"$(INTDIR)\tzvmledt.obj" \
	"$(INTDIR)\zcmddef.obj" \
	"$(INTDIR)\zstring.obj" \
	"$(INTDIR)\zviewdef.obj"

"..\..\..\..\w\bin\tzvmledt.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

ProjDir=.
TargetDir=\$(KZV)\w\bin
SOURCE="$(InputPath)"
PostBuild_Desc=copy ini settings
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "..\..\..\..\w\bin\tzvmledt.dll" "$(OUTDIR)\tzvmledt.bsc"
   copy ".\*.ini"  %KZD%:\%KZV%\w\bin\*.*
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tzvmledt.dep")
!INCLUDE "tzvmledt.dep"
!ELSE 
!MESSAGE Warning: cannot find "tzvmledt.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tzvmledt - Win32 Release" || "$(CFG)" == "tzvmledt - Win32 Debug"
SOURCE=.\tzcswrpr.cpp

!IF  "$(CFG)" == "tzvmledt - Win32 Release"


"$(INTDIR)\tzcswrpr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tzvmledt - Win32 Debug"


"$(INTDIR)\tzcswrpr.obj"	"$(INTDIR)\tzcswrpr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\tzvmledt.cpp

!IF  "$(CFG)" == "tzvmledt - Win32 Release"


"$(INTDIR)\tzvmledt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tzvmledt - Win32 Debug"


"$(INTDIR)\tzvmledt.obj"	"$(INTDIR)\tzvmledt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\zcmddef.cpp

!IF  "$(CFG)" == "tzvmledt - Win32 Release"


"$(INTDIR)\zcmddef.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tzvmledt - Win32 Debug"


"$(INTDIR)\zcmddef.obj"	"$(INTDIR)\zcmddef.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\zstring.cpp

!IF  "$(CFG)" == "tzvmledt - Win32 Release"


"$(INTDIR)\zstring.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tzvmledt - Win32 Debug"


"$(INTDIR)\zstring.obj"	"$(INTDIR)\zstring.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\zviewdef.cpp

!IF  "$(CFG)" == "tzvmledt - Win32 Release"


"$(INTDIR)\zviewdef.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tzvmledt - Win32 Debug"


"$(INTDIR)\zviewdef.obj"	"$(INTDIR)\zviewdef.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

