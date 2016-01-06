#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon E/R Dialog
#              NMAKE makefile
#
#CHANGE LOG
#2000.10.14   BL  Z10
#     Added tzersapd.obj for SAP Import
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzeremdd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib  kzrpsrco.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzeremde.obj  \
                 $(Z_OBJ)\tzeremdo.obj  \
                 $(Z_OBJ)\tzersapd.obj

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
