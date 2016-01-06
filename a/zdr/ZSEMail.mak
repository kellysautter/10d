# Microsoft Developer Studio Generated NMAKE File, Based on ZSEMail.dsp
!IF "$(CFG)" == ""
CFG=ZSEMail - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ZSEMail - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ZSEMail - Win32 Release" && "$(CFG)" != "ZSEMail - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZSEMail.mak" CFG="ZSEMail - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZSEMail - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ZSEMail - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ZSEMail - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ZSEMail.dll"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ZSEMail.obj"
	-@erase "$(INTDIR)\ZSEMail.res"
	-@erase "$(OUTDIR)\ZSEMail.dll"
	-@erase "$(OUTDIR)\ZSEMail.exp"
	-@erase "$(OUTDIR)\ZSEMail.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /EHsc /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\ZSEMail.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x407 /fo"$(INTDIR)\ZSEMail.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZSEMail.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ZSEMail.pdb" /machine:I386 /def:".\ZSEMail.def" /out:"$(OUTDIR)\ZSEMail.dll" /implib:"$(OUTDIR)\ZSEMail.lib" 
DEF_FILE= \
	".\ZSEMail.def"
LINK32_OBJS= \
	"$(INTDIR)\ZSEMail.obj" \
	"$(INTDIR)\ZSEMail.res"

"$(OUTDIR)\ZSEMail.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ZSEMail - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\w\bin\ZSEMail.dll" "$(OUTDIR)\ZSEMail.bsc"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ZSEMail.obj"
	-@erase "$(INTDIR)\ZSEMail.res"
	-@erase "$(INTDIR)\ZSEMail.sbr"
	-@erase "$(OUTDIR)\ZSEMail.bsc"
	-@erase "$(OUTDIR)\ZSEMail.pdb"
	-@erase "..\..\w\bin\ZSEMail.dll"
	-@erase "..\..\w\bin\ZSEMail.ilk"
	-@erase "..\..\w\libr\ZSEMail.exp"
	-@erase "..\..\w\libr\ZSEMail.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /WX /Gm /EHsc /ZI /Od /I "$(KZD):\$(KZV)\w\incz" /I "$(KZD):\$(KZV)\w\incr" /I "$(KZD):\$(KZV)\a\incz" /I "$(KZD):\$(KZV)\a\incr" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "__WIN32__" /D "QUINSOFT" /D "_NOANCHOR" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ZSEMail.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZSEMail.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ZSEMail.sbr"

"$(OUTDIR)\ZSEMail.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kzoengaa.lib zdrapp.lib Ws2_32.lib see32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ZSEMail.pdb" /debug /machine:I386 /def:".\ZSEMail.def" /out:"..\..\w\bin\ZSEMail.dll" /implib:"..\..\w\libr\ZSEMail.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" 
DEF_FILE= \
	".\ZSEMail.def"
LINK32_OBJS= \
	"$(INTDIR)\ZSEMail.obj" \
	"$(INTDIR)\ZSEMail.res"

"..\..\w\bin\ZSEMail.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ZSEMail.dep")
!INCLUDE "ZSEMail.dep"
!ELSE 
!MESSAGE Warning: cannot find "ZSEMail.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ZSEMail - Win32 Release" || "$(CFG)" == "ZSEMail - Win32 Debug"
SOURCE=.\ZSEMail.cpp

!IF  "$(CFG)" == "ZSEMail - Win32 Release"


"$(INTDIR)\ZSEMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZSEMail.pch"


!ELSEIF  "$(CFG)" == "ZSEMail - Win32 Debug"


"$(INTDIR)\ZSEMail.obj"	"$(INTDIR)\ZSEMail.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ZSEMail.rc

"$(INTDIR)\ZSEMail.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

