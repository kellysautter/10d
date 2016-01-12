#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon E/R Dialog
#              NMAKE makefile
#
# Change Log
# 2000.10.14   BL  Z10
#     Added tzersapd.obj for SAP Import
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzeremdd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib  kzrpsrco.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzeremde.obj

# Extra options
USER_OPT       =

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
