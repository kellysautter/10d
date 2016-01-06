#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
# 2000.07.05   BL
# Added tzvmlip.lib for zGetStringWithoutBlank
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzdmupdd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib  tzedfrmd.lib  tzvmlip.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzdmup2d.obj

# Extra options
USER_OPT       =

# Add MFC libraries.
!ifdef DEBUG
BASELINKDLLS = $(BASELINKDLLS) mfcs140ud.lib
!else
BASELINKDLLS = $(BASELINKDLLS) mfcs140u.lib
!endif

# Compile using above defs.
!include "..\..\mak\compile.hnm"
