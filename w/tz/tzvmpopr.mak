#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Tool VML Operations Composite
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzvmpopr
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzvmplaa.obj \
                 $(Z_OBJ)\tzvmpyaa.obj \
                 $(Z_OBJ)\tzvmp1aa.obj \
                 $(Z_OBJ)\tzvmp2aa.obj \
                 $(Z_OBJ)\tzvmp3aa.obj \
                 $(Z_OBJ)\tzvmp4aa.obj \
                 $(Z_OBJ)\tzvmp5aa.obj \
                 $(Z_OBJ)\tzvmp6aa.obj \
                 $(Z_OBJ)\tzvmp7aa.obj \
                 $(Z_OBJ)\tzvmp8aa.obj \
                 $(Z_OBJ)\tzvmp9aa.obj \
                 $(Z_OBJ)\tzvmpaaa.obj \
                 $(Z_OBJ)\tzvmpeaa.obj \
                 $(Z_OBJ)\tzvmpmoo.obj

# Extra options
USER_OPT       =

# Compile using above defs.
!include "..\..\mak\compile.hnm"
