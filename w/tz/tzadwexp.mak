#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon ADW Export
#              NMAKE makefile
#
# Change Log
#
# 2000.06.10  BL
#    create this make file for ADW Export
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzadwexp
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       =
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
