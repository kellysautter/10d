#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Tool VML Operations Composite
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzvmlopr
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib TZVMPOPR.lib tzobrwad.lib DelayImp.lib /DelayLoad:tzlodopr.dll
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzvmi3aa.obj \
                 $(Z_OBJ)\tzwrksto.obj \
                 $(Z_OBJ)\tzvmedaw.obj \
                 $(Z_OBJ)\tzvmg1aa.obj \
                 $(Z_OBJ)\tzvmg2aa.obj \
                 $(Z_OBJ)\tzvmgnaa.obj
CPP_OBJS       = $(Z_OBJ)\tzvmi1aa.obj \
                 $(Z_OBJ)\tzvmi2aa.obj \
                 $(Z_OBJ)\tzvminaa.obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
