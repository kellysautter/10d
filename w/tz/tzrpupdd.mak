#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
# ChangeLog
#
# 2000.12.07  RG  Z2000
#     Set Variable BUILDSIRON to 'Y', if you want to build the
#     Siron DLL's
# 2000.07.26  RG  Z2000
#     inserted tzrpupsd again
# 2000.07.24  RG  Z2000
#     Removed tzcm_opr.lib and inserted kzsirrep.lib again
#-------------------------------------------------------------------

# Initialize Zeidon macros.
!include "..\..\mak\env.hnm"

MODULE         = tzrpupdd
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LIB_DEPENDENTS =
!if "$(BUILDSIRON)"=="Y"
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzrpup2d.obj  \
                 $(Z_OBJ)\tzrpupsd.obj
LINKDLLS       = tzlodopr.lib kzsirrep.lib
!else
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tzrpup2d.obj
LINKDLLS       = tzlodopr.lib
!endif
DRIVER_APP     = YES

# Extra options
!if "$(QUINSOFT)"=="Y"
USER_OPT       =  /DQUINSOFT
!else
USER_OPT       =
!endif

# Add MFC libraries.
!ifdef DEBUG
BASELINKDLLS = $(BASELINKDLLS) mfcs140ud.lib
!else
BASELINKDLLS = $(BASELINKDLLS) mfcs140u.lib
!endif

# Compile using above defs.
!include "..\..\mak\compile.hnm"
