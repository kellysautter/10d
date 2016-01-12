# Microsoft Developer Studio Generated NMAKE File, Based on truegrid.dsp
!IF "$(CFG)" == ""
CFG=truegrid - Win32 Debug
!MESSAGE No configuration specified. Defaulting to truegrid - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "truegrid - Win32 Release" && "$(CFG)" != "truegrid - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "truegrid.mak" CFG="truegrid - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "truegrid - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "truegrid - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "truegrid - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\w\bin\truegrid.dll"

!ELSE 

ALL : "..\..\..\..\w\bin\truegrid.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\tdbg6.obj"
	-@erase "$(INTDIR)\tgcolser.obj"
	-@erase "$(INTDIR)\trddmap.obj"
	-@erase "$(INTDIR)\trdgmap.obj"
	-@erase "$(INTDIR)\trgrxprt.obj"
	-@erase "$(INTDIR)\trhlpr.obj"
	-@erase "$(INTDIR)\truegrid.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\xarraydb.obj"
	-@erase "$(OUTDIR)\truegrid.pdb"
	-@erase "..\..\..\..\w\bin\truegrid.dll"
	-@erase "..\..\..\..\w\bin\truegrid.ilk"
	-@erase "..\..\..\..\w\libz\truegrid.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /ZI /Od /D "_RELEASE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "_USE_DATASET_" /D "zGRID_COMPILE" /D "TB_DROPDOWN_IMPLEMENT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\truegrid.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\truegrid.pdb" /debug /machine:I386 /def:".\truegrid.def" /out:"..\..\..\..\w\bin\truegrid.dll" /implib:"..\..\..\..\w\libz/truegrid.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release" 
DEF_FILE= \
	".\truegrid.def"
LINK32_OBJS= \
	"$(INTDIR)\tdbg6.obj" \
	"$(INTDIR)\tgcolser.obj" \
	"$(INTDIR)\trddmap.obj" \
	"$(INTDIR)\trdgmap.obj" \
	"$(INTDIR)\trgrxprt.obj" \
	"$(INTDIR)\trhlpr.obj" \
	"$(INTDIR)\truegrid.obj" \
	"$(INTDIR)\xarraydb.obj" \
	"..\..\..\..\w\libr\ZdCtl.lib" \
	"..\..\..\..\w\libr\ZDrApp.lib" \
	"..\..\..\..\w\libr\TzCtl.lib"

"..\..\..\..\w\bin\truegrid.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF                                               "$(CFG)" == "truegrid - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\w\bin\truegrid.dll" "$(OUTDIR)\truegrid.bsc"

!ELSE 

ALL : "..\..\..\..\w\bin\truegrid.dll" "$(OUTDIR)\truegrid.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\tdbg6.obj"
	-@erase "$(INTDIR)\tdbg6.sbr"
	-@erase "$(INTDIR)\tgcolser.obj"
	-@erase "$(INTDIR)\tgcolser.sbr"
	-@erase "$(INTDIR)\trddmap.obj"
	-@erase "$(INTDIR)\trddmap.sbr"
	-@erase "$(INTDIR)\trdgmap.obj"
	-@erase "$(INTDIR)\trdgmap.sbr"
	-@erase "$(INTDIR)\trgrxprt.obj"
	-@erase "$(INTDIR)\trgrxprt.sbr"
	-@erase "$(INTDIR)\trhlpr.obj"
	-@erase "$(INTDIR)\trhlpr.sbr"
	-@erase "$(INTDIR)\truegrid.obj"
	-@erase "$(INTDIR)\truegrid.pch"
	-@erase "$(INTDIR)\truegrid.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\xarraydb.obj"
	-@erase "$(INTDIR)\xarraydb.sbr"
	-@erase "$(OUTDIR)\truegrid.bsc"
	-@erase "$(OUTDIR)\truegrid.pdb"
	-@erase "..\..\..\..\w\bin\truegrid.dll"
	-@erase "..\..\..\..\w\bin\truegrid.ilk"
	-@erase "..\..\..\..\w\libz\truegrid.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /EHsc /ZI /Od /GR /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "zGRID_COMPILE" /D "TB_DROPDOWN_IMPLEMENT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\truegrid.pch" /Yu"tdbg6.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<< KEEP

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
<< KEEP

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\truegrid.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\tdbg6.sbr" \
	"$(INTDIR)\tgcolser.sbr" \
	"$(INTDIR)\trddmap.sbr" \
	"$(INTDIR)\trdgmap.sbr" \
	"$(INTDIR)\trgrxprt.sbr" \
	"$(INTDIR)\trhlpr.sbr" \
	"$(INTDIR)\truegrid.sbr" \
	"$(INTDIR)\xarraydb.sbr"

"$(OUTDIR)\truegrid.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ZdCtl.lib ZdrApp.lib kzoengaa.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\truegrid.pdb" /debug /machine:I386 /def:".\truegrid.def" /out:"..\..\..\..\w\bin\truegrid.dll" /implib:"..\..\..\..\w\libz/truegrid.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\debug" 
DEF_FILE= \
	".\truegrid.def"
LINK32_OBJS= \
	"$(INTDIR)\tdbg6.obj" \
	"$(INTDIR)\tgcolser.obj" \
	"$(INTDIR)\trddmap.obj" \
	"$(INTDIR)\trdgmap.obj" \
	"$(INTDIR)\trgrxprt.obj" \
	"$(INTDIR)\trhlpr.obj" \
	"$(INTDIR)\truegrid.obj" \
	"$(INTDIR)\xarraydb.obj" \
	"..\..\..\..\w\libr\ZdCtl.lib" \
	"..\..\..\..\w\libr\ZDrApp.lib" \
	"..\..\..\..\w\libr\TzCtl.lib"

"..\..\..\..\w\bin\truegrid.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("truegrid.dep")
!INCLUDE "truegrid.dep"
!ELSE 
!MESSAGE Warning: cannot find "truegrid.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "truegrid - Win32 Release" || "$(CFG)" == "truegrid - Win32 Debug"
SOURCE=.\tdbg6.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /EHsc /ZI /Od /D "_RELEASE" /D "_WINDLL" /D "_USE_DATASET_" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "__WIN32__" /D "zGRID_COMPILE" /D "TB_DROPDOWN_IMPLEMENT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /FD /c 

"$(INTDIR)\tdbg6.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /EHsc /ZI /Od /GR /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_AFXEXT" /D "__WIN32__" /D "zGRID_COMPILE" /D "TB_DROPDOWN_IMPLEMENT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\truegrid.pch" /Yc"tdbg6.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /I "$(KZD):\$(KZV)\a\zdr" /I "..\tzaxctl" /FD /c 

"$(INTDIR)\tdbg6.obj"	"$(INTDIR)\tdbg6.sbr"	"$(INTDIR)\truegrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<< KEEP

!ENDIF 

SOURCE=.\tgcolser.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"


"$(INTDIR)\tgcolser.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"


"$(INTDIR)\tgcolser.obj"	"$(INTDIR)\tgcolser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\truegrid.pch"


!ENDIF 

SOURCE=.\trddmap.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"


"$(INTDIR)\trddmap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"


"$(INTDIR)\trddmap.obj"	"$(INTDIR)\trddmap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\truegrid.pch"


!ENDIF 

SOURCE=.\trdgmap.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"


"$(INTDIR)\trdgmap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"


"$(INTDIR)\trdgmap.obj"	"$(INTDIR)\trdgmap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\truegrid.pch"


!ENDIF 

SOURCE=.\trgrxprt.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"


"$(INTDIR)\trgrxprt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"


"$(INTDIR)\trgrxprt.obj"	"$(INTDIR)\trgrxprt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\truegrid.pch"


!ENDIF 

SOURCE=.\trhlpr.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"


"$(INTDIR)\trhlpr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"


"$(INTDIR)\trhlpr.obj"	"$(INTDIR)\trhlpr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\truegrid.pch"


!ENDIF 

SOURCE=.\truegrid.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"


"$(INTDIR)\truegrid.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"


"$(INTDIR)\truegrid.obj"	"$(INTDIR)\truegrid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\truegrid.pch"


!ENDIF 

SOURCE=.\xarraydb.cpp

!IF  "$(CFG)" == "truegrid - Win32 Release"


"$(INTDIR)\xarraydb.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "truegrid - Win32 Debug"


"$(INTDIR)\xarraydb.obj"	"$(INTDIR)\xarraydb.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\truegrid.pch"


!ENDIF 


!ENDIF 

