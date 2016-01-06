#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Standard
#              NMAKE makefile
#
# Change Log
#
# 08.06.1998  RG
#    Wenn zdrapp.lib fehlt, wird die DLL nicht gebildet !!!
#
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = kzmsgqaa
TARGETS        = $(Z_BIN)\$(MODULE).dll
OBJS           = $(Z_OBJ)\$(MODULE).obj
C_DEPENDENTS   =
LINKDLLS       = zdrapp.lib
DRIVER_APP     = YES


# Extra options
USER_OPT       =

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
