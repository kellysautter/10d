#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Workstation Dialog
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzcmlpmd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib  kzrpsrco.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzpesrco.obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
