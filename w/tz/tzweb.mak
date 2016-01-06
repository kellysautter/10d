#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzweb
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzvmlip.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\tzwebgl1.obj \
                 $(Z_OBJ)\tzwebgl2.obj \
                 $(Z_OBJ)\tzwebgl3.obj \
                 $(Z_OBJ)\tzwebgl4.obj 
OBJS           =

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
