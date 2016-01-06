#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon ??? Dialog
#              NMAKE makefile
#

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzaxcfgd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = ZdCtl.lib ZdrApp.lib kzoengaa.lib truegrid.lib tzctl.lib tzlodopr.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
