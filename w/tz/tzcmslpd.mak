#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Workstation Dialog
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzcmslpd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
