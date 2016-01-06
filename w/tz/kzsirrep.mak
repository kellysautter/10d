#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon access to Siron reports
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = kzsirrep
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\kzsirrep.obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"

