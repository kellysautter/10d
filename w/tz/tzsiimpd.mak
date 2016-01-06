#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#
# Change Log:
#
# 2000.02.04  RG  Z2000
#     We need new lib: tzvmlip.lib
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzsiimpd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib tzsioprs.lib tzvmlip.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       = /TP

# Compile using above defs.
!include "..\..\mak\compile.hnm"
