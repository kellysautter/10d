#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Operation signature Dialog
#              NMAKE makefile
#-------------------------------------------------------------------
# CHANGE LOG
#
# 2001.07.20  TMV
#     compile as cpp module ( changed OBJS to CPP_OBJS )
#

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzopsigd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj

# Extra options
USER_OPT       =

CL_OPT = /D_AFXDLL $(CL_OPT)

# Add MFC libraries.
!ifdef DEBUG
BASELINKDLLS = $(BASELINKDLLS) mfcs140ud.lib
!else
BASELINKDLLS = $(BASELINKDLLS) mfcs140u.lib
!endif

# Compile using above defs.
!include "..\..\mak\compile.hnm"
