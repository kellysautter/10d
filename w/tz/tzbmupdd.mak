#-------------------------------------------------------------------
# DESCRIPTION: TONBELLER Zeidon Bitmap Dialog
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzbmupdd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib tzvmlip.lib tzerrord.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
