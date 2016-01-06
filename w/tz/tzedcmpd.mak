#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#
# Change Log
#
# 2001.07.06  RG
#    Added tzvmlip.lib for relative path operation
# 2001.07.01  BL
#    Added truegrid.lib for TrueGrid Operations
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzedcmpd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib tzvmpopr.lib tzvmlopr.lib truegrid.lib tzvmlip.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\tzedcm2d.obj
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
