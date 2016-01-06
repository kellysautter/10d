#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Object Browser Dialog
#              NMAKE makefile
# Change Log
#
# 1999.10.14  US  10a remove change from RG 1999.10.07
#     Change no longer necessary because DKS did all work so that
#     the modules can compile under C++ without warning
# 1999.10.07  RG  Z10 Fixed compile error
#     Use OBJS instead of CPP_OBJS, because we get the compiler
#     error C2440 if we use CPP_OBJS
#
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzobrwad
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   = $(Z_INC)z\kzoemiaa.h \
                 $(Z_INC)z\kzoep0aa.h \
                 $(Z_INC)r\kzoemeaa.h \
                 $(Z_INC_ENV)r\kzoeenaa.h
LINKDLLS       = tzlodopr.lib
DRIVER_APP     = YES
#OBJS          = $(Z_OBJ)\$(MODULE).obj
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj

# Extra options
#CL_OPT = $(CL_OPT) /D_CRT_SECURE_NO_WARNINGS
#LINK_OPT = $(LINK_OPT) /NODEFAULTLIB:MSVCRTD.lib
USER_OPT       =

# Add MFC libraries.
!ifdef DEBUG
#BASELINKDLLS = $(BASELINKDLLS) mfcs140ud.lib
!else
#BASELINKDLLS = $(BASELINKDLLS) mfcs140u.lib
!endif

# Compile using above defs.
!include "..\..\mak\compile.hnm"