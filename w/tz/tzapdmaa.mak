#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Standard Domains
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzapdmaa
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       =
DRIVER_APP     =
OBJS           = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzapdmab.obj
CPP_OBJS       = $(Z_OBJ)\tzapdmac.obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
