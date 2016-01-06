#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Workstation Dialog
#              NMAKE makefile
#
# Change LOG
# 2000.02.24  BL  Z10
#    added tzcmlpld.lib for refresh user defined
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzcmrptd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib  tzcmlpld.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
