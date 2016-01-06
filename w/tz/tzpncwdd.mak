#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
# ChangeLog
#
# 2000.09.16  BL Z2000
#     inserted tzpncw2d.obj, is a new Source File for clone Dialog
#     window
#     inserted tzedfrmd.lib for clone Window with copy Operation Code
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzpncwdd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib  tzedfrmd.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzpncw2d.obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
