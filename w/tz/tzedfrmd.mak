#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#-------------------------------------------------------------------
# CHANGE LOG
#
# 2000.09.29  TMV Z2000
#     there is no mfco42.lib and mfcd42.lib so remove it from BASELINKDLLS listing
#
# 2000.08.30  DGC Z2000
#     Changed includes to use standard .hnm files.

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzedfrmd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib tzvmpopr.lib tzvmlopr.lib
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
