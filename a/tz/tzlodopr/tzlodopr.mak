#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Tool Object Operations Composite
#              NMAKE makefile
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzlodopr
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   = $(Z_SRC)\tzcm_opr.hg
#LINKDLLS       = tzvmlip.lib tzvmpopr.lib tzvmlopr.lib
LINKDLLS       = tzvmlip.lib  tzweb.lib
DRIVER_APP     = YES
OBJS           = $(Z_OBJ)\$(MODULE).obj \
                 $(Z_OBJ)\tz__prfo.obj \
                 $(Z_OBJ)\tzadcs2o.obj \
                 $(Z_OBJ)\tzadcsdo.obj \
                 $(Z_OBJ)\tzadw1aw.obj \
                 $(Z_OBJ)\tzadw2aw.obj \
                 $(Z_OBJ)\tzadw3aw.obj \
                 $(Z_OBJ)\tzadw4aw.obj \
                 $(Z_OBJ)\tzadw5aw.obj \
                 $(Z_OBJ)\tzadw6aw.obj \
                 $(Z_OBJ)\tzadw7aw.obj \
                 $(Z_OBJ)\tzadw8aw.obj \
                 $(Z_OBJ)\tzadwexp.obj \
                 $(Z_OBJ)\tzadwoaw.obj \
                 $(Z_OBJ)\tzdgsrco.obj \
                 $(Z_OBJ)\tzdmsrco.obj \
                 $(Z_OBJ)\tzdmxgpo.obj \
                 $(Z_OBJ)\tzeremdo.obj \
                 $(Z_OBJ)\tzeremxo.obj \
                 $(Z_OBJ)\tzersa2o.obj \
                 $(Z_OBJ)\tzersaso.obj \
                 $(Z_OBJ)\tzlooprs.obj \
                 $(Z_OBJ)\tzogsrco.obj \
                 $(Z_OBJ)\tzopgrpo.obj \
                 $(Z_OBJ)\tzophdro.obj \
                 $(Z_OBJ)\tzopsigo.obj \
                 $(Z_OBJ)\tzpnoldc.obj \
#                $(Z_OBJ)\tzsioprs.obj \
                 $(Z_OBJ)\tzsixsko.obj \
                 $(Z_OBJ)\tztempmo.obj \
                 $(Z_OBJ)\tztenv2o.obj \
                 $(Z_OBJ)\tztenvro.obj \
                 $(Z_OBJ)\tzwdgrwo.obj \
                 $(Z_OBJ)\tzwdlg2o.obj \
                 $(Z_OBJ)\tzwdlg3o.obj \
                 $(Z_OBJ)\tzwdlgso.obj \
                 $(Z_OBJ)\tzzolo2o.obj \
                 $(Z_OBJ)\tzzolodo.obj \
                 $(Z_OBJ)\tzzolodr.obj \
                 $(Z_OBJ)\tzzorpts.obj \
                 $(Z_OBJ)\tzzoxodo.obj \
#         Previously in tzcm_opr...     \
                 $(Z_OBJ)\tzcm_opr.obj \
                 $(Z_OBJ)\tzcmcpro.obj \
                 $(Z_OBJ)\tzcmcvto.obj \
                 $(Z_OBJ)\tzcmqsyo.obj \
                 $(Z_OBJ)\tzcmlops.obj \
                 $(Z_OBJ)\tzcmlp2o.obj \
                 $(Z_OBJ)\tzcmlplo.obj \
                 $(Z_OBJ)\tzcmlpmo.obj \
                 $(Z_OBJ)\tzcmrepo.obj \
                 $(Z_OBJ)\tzcmutil.obj \
                 $(Z_OBJ)\tzcmwkso.obj

CPP_OBJS       = $(Z_OBJ)\tzrpsrco.obj \
                 $(Z_OBJ)\tzrpsxm2.obj \
                 $(Z_OBJ)\tzcmoprs.obj \
                 $(Z_OBJ)\tzwdlg4o.obj \
                 $(Z_OBJ)\tzwdlg6o.obj \
                 $(Z_OBJ)\tzerem2o.obj \
                 $(Z_OBJ)\TZVSRTOO.obj

# Extra options
USER_OPT       =  /D _AFXDLL

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm
