#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#-------------------------------------------------------------------

#CHANGE LOG
#
# 2000.04.29   HH
#    compile C++
# 1999.10.11   BL  Z10  new menu structure
#    insert tzoprusd.lib (menu item <Add Source File> calls operations
#    from tzoprusd.c)

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzzolodq
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzlodopr.lib kzrpsrco.lib tzoprusd.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzzoloq2.obj

# Extra options
USER_OPT       =

CL_OPT = /D_AFXDLL $(CL_OPT)

# Add MFC libraries.
!ifdef DEBUG
BASELINKDLLS = $(BASELINKDLLS) mfc42d.lib
!else
BASELINKDLLS = $(BASELINKDLLS) mfc42.lib
!endif

# Compile using above defs.
!include "..\..\mak\compile.hnm"
