#-------------------------------------------------------------------
# DESCRIPTION: Standard C Library Functions for VML Interpreter
#              NMAKE makefile
#-------------------------------------------------------------------
# CHANGE LOG
#
# 2001.12.20  DGC Z2000
#      Removed MORE_CL_OPTS and used CPP_OBJS instead.
# 2000.09.06  RG  Z2000
#      Use mergedef like in the other makefiles
# 2000.08.30  DGC Z2000
#      Changed to use standard .hnm file.  Added SKIP_MERGEDEF.

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzvmlip
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       =
DRIVER_APP     = NO
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj
DEFFILE        = $(Z_BASE_ENV)\$(MODULE).def

Z_LIB_SUFFIX   = r
SKIP_MERGEDEF  = Y

# Compile using above defs.
!include "..\..\mak\compile.hnm"
