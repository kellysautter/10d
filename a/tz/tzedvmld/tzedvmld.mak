#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Editor Dialog
#              NMAKE makefile
#
# Change Log
# 1999.11.23  BL  Z10  new menu structure
#     insert tzoprusd.lib for reuse Operation
#-------------------------------------------------------------------

# Initialize Zeidon macros.
#!include "..\..\mak\env.hnm"
!include env.hnm

MODULE         = tzedvmld
TARGETS        = $(Z_BIN)\$(MODULE).dll
C_DEPENDENTS   =
LINKDLLS       = tzvmledt.lib tzlodopr.lib tzvmpopr.lib tzvmlopr.lib
DRIVER_APP     = YES
CPP_OBJS       = $(Z_OBJ)\$(MODULE).obj


# Extra options
USER_OPT       = /I "$(KZD):\$(KZV)\a\zdr"

# Compile using above defs.
#!include "..\..\mak\compile.hnm"
!include compile.hnm

#LINK32_FLAGS=tzvmledt.lib ZdCtl.lib ZdrApp.lib kzoengaa.lib tzctl.lib tzlodopr.lib tzvmpopr.lib tzvmlopr.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tzedvmld.pdb" /debug /machine:I386 /def:"..\..\w\tz\tzedvmld.def" /out:"..\..\w\bin\tzedvmld.dll" /implib:"$(OUTDIR)\tzedvmld.lib" /libpath:"$(KZD):\$(KZV)\w\libz" /libpath:"$(KZD):\$(KZV)\w\libr" /libpath:".\release" 
