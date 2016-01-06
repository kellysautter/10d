#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#
# Change Log
# 2001.03.07  SR  Z2000
#     Remove link flags: /debug /pdbtype:con because it cause errors
#
# 2000.02.06  SR  Z2000
#     Add link flags: /debug /pdbtype:con
#     to get debug information for release mode
#
# 2000.07.24  RG  Z2000
#     Removed tzcm_opr.lib
# 2000.05.12  RG  Z2000
#     Changed option -Oselg --> -Od
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzcmqcpo
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = zdrapp.lib zdctl.lib tzlodopr.lib tzeremdd.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzeremde.obj

# Extra options
USER_OPT       =

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
