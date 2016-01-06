#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon PPE Dialog
#              NMAKE makefile
#
# CHANGE LOG
# 2000.06.21   BL
# Added tzvmlip.lib for zGetStringWithoutBlank
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzpeupdd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib tzvmlip.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
