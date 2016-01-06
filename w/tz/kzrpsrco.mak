#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = kzrpsrco
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\kzrpquer.obj
OBJS           = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\kzrpsrc2.obj  \
                 $(Z_OBJ)\kzrpwebo.obj  \
                 $(Z_OBJ)\kzxmlpgo.obj  \
                 $(Z_OBJ)\kzxmlpg2.obj

# Put the lib in LIBR instead of LIBZ
Z_LIB_SUFFIX = r

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
