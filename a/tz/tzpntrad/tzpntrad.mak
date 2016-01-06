#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
# Change Log
# 1999.11.23  BL  Z10  new menu structure
#     insert tzoprusd.lib for reuse Operation
#
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzpntrad
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib tzedfrmd.lib tzpnctad.lib tzoprusd.lib tzvmlip.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj  \
                 $(Z_OBJ)\TZPNTRVD.obj
# Extra options
USER_OPT       =


# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
