# Microsoft Developer Studio Generated NMAKE File, Based on Z2MP.dsp
!IF "$(CFG)" == ""
CFG=Z2MP - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Z2MP - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Z2MP - Win32 Release" && "$(CFG)" != "Z2MP - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Z2MP.mak" CFG="Z2MP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Z2MP - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Z2MP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Z2MP - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Z2MP.dll"


CLEAN :
	-@erase "$(INTDIR)\mpna81.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Z2MP.obj"
	-@erase "$(INTDIR)\Z2MP.res"
	-@erase "$(INTDIR)\Z2MPAPI.obj"
	-@erase "$(OUTDIR)\Z2MP.dll"
	-@erase "$(OUTDIR)\Z2MP.exp"
	-@erase "$(OUTDIR)\Z2MP.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Z2MP.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x407 /fo"$(INTDIR)\Z2MP.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Z2MP.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Z2MP.pdb" /machine:I386 /def:".\Z2MP.def" /out:"$(OUTDIR)\Z2MP.dll" /implib:"$(OUTDIR)\Z2MP.lib" 
DEF_FILE= \
	".\Z2MP.def"
LINK32_OBJS= \
	"$(INTDIR)\mpna81.obj" \
	"$(INTDIR)\Z2MP.obj" \
	"$(INTDIR)\Z2MPAPI.obj" \
	"$(INTDIR)\Z2MP.res"

"$(OUTDIR)\Z2MP.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Z2MP - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\w\bin\Z2MP.dll" "$(OUTDIR)\Z2MP.bsc"


CLEAN :
	-@erase "$(INTDIR)\mpna81.obj"
	-@erase "$(INTDIR)\mpna81.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Z2MP.obj"
	-@erase "$(INTDIR)\Z2MP.pch"
	-@erase "$(INTDIR)\Z2MP.res"
	-@erase "$(INTDIR)\Z2MP.sbr"
	-@erase "$(INTDIR)\Z2MPAPI.obj"
	-@erase "$(INTDIR)\Z2MPAPI.SBR"
	-@erase "$(OUTDIR)\Z2MP.bsc"
	-@erase "$(OUTDIR)\Z2MP.pdb"
	-@erase "..\..\w\bin\Z2MP.dll"
	-@erase "..\..\w\bin\Z2MP.ilk"
	-@erase "..\..\w\libr\Z2MP.exp"
	-@erase "..\..\w\libr\Z2MP.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /WX /Gm /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Z2MP.pch" /Yu"mpna81.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Z2MP.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Z2MP.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\mpna81.sbr" \
	"$(INTDIR)\Z2MP.sbr" \
	"$(INTDIR)\Z2MPAPI.SBR"

"$(OUTDIR)\Z2MP.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kzoengaa.lib zdrapp.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Z2MP.pdb" /debug /machine:I386 /def:".\Z2MP.def" /out:"..\..\w\bin\Z2MP.dll" /implib:"..\..\w\libr\Z2MP.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" 
DEF_FILE= \
	".\Z2MP.def"
LINK32_OBJS= \
	"$(INTDIR)\mpna81.obj" \
	"$(INTDIR)\Z2MP.obj" \
	"$(INTDIR)\Z2MPAPI.obj" \
	"$(INTDIR)\Z2MP.res"

"..\..\w\bin\Z2MP.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Z2MP.dep")
!INCLUDE "Z2MP.dep"
!ELSE 
!MESSAGE Warning: cannot find "Z2MP.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Z2MP - Win32 Release" || "$(CFG)" == "Z2MP - Win32 Debug"
SOURCE=.\mpna81.cpp

!IF  "$(CFG)" == "Z2MP - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /EHsc /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\Z2MP.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\mpna81.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Z2MP.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Z2MP - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /WX /Gm /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Z2MP.pch" /Yc"mpna81.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\mpna81.obj"	"$(INTDIR)\mpna81.sbr"	"$(INTDIR)\Z2MP.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Z2MP.cpp

!IF  "$(CFG)" == "Z2MP - Win32 Release"


"$(INTDIR)\Z2MP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Z2MP.pch"


!ELSEIF  "$(CFG)" == "Z2MP - Win32 Debug"


"$(INTDIR)\Z2MP.obj"	"$(INTDIR)\Z2MP.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Z2MP.pch"


!ENDIF 

SOURCE=.\Z2MP.rc

"$(INTDIR)\Z2MP.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Z2MPAPI.cpp

!IF  "$(CFG)" == "Z2MP - Win32 Release"


"$(INTDIR)\Z2MPAPI.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Z2MP.pch"


!ELSEIF  "$(CFG)" == "Z2MP - Win32 Debug"


"$(INTDIR)\Z2MPAPI.obj"	"$(INTDIR)\Z2MPAPI.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Z2MP.pch"


!ENDIF 


!ENDIF 

