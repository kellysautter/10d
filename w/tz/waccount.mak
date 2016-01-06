#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Account Dialog
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = wAccount
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = kzrpsrco.lib truegrid.lib z2mp.lib ZSEmail.lib
DRIVER_APP     = YES
#OBJS          = $(Z_OBJ)\$(MODULE).obj
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
