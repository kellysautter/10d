#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Workstation Dialog
#              NMAKE makefile
#
# Change Log
#
# 2001.07.15  BL
#    Added truegrid.lib for TrueGrid Operations
#
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzcmlpld
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib truegrid.lib tzvmlip.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzcmlpldv.obj  \

# Extra options
USER_OPT       =

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
