#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon LOD Dialog
#              NMAKE makefile
#
# Change Log
# 2001.03.07  SR  Z2000
#     Remove link flags: /debug /pdbtype:con because it cause errors
#
# 2000.02.06  SR  Z2000
#     Add link flags: /debug /pdbtype:con
#     to get debug information for release mode
#
# 2000.07.24  RG  Z2000
#     Removed tzcm_opr.lib
# 2000.05.12  RG  Z2000
#     Changed option -Oselg --> -Od
#-------------------------------------------------------------------

MODULE     = TZCMQCPO
MODULE_EXT = dll
MODULE_DIR = tz
MODULE_SRC = c

!IFNDEF Z_ENV
Z_ENV    = w
!ENDIF

Z_BASE   = ..\..
Z_BASE_A = $(Z_BASE)\a\$(MODULE_DIR)
Z_BASE_ENV = $(Z_BASE)\$(Z_ENV)\$(MODULE_DIR)
Z_LIB = $(Z_BASE)\$(Z_ENV)\lib
Z_BIN = $(Z_BASE)\$(Z_ENV)\bin

ALL: $(Z_BIN)\$(MODULE).$(MODULE_EXT)

!IFNDEF MODEL
MODEL = l
!ENDIF

!IF "$(MODEL)"=="s"
MODEL    = -Asnw -D_WINDOWS -D_WINDLL -DZEIDON
LINKDLLS = sdllcew libw kzoengaa OLDNAMES
!ELSE
!IF "$(MODEL)"=="m"
MODEL    = -Alnw -D_WINDOWS -D_WINDLL -DZEIDON
LINKDLLS = mdllcew libw kzoengaa OLDNAMES
!ELSE
!IF "$(MODEL)"=="l"
MODEL    = -Alfw -D_WINDOWS -D_WINDLL -DZEIDON
LINKDLLS = ldllcew libw kzoengaa OLDNAMES
!ELSE
!ERROR Invalid model specified.  Must be s, m or l
!ENDIF
!ENDIF
!ENDIF

!if "$(DEBUG)" == "1"
Z_OBJ = $(Z_BASE_ENV)\debug
!else
Z_OBJ = $(Z_BASE_ENV)\nodbg
!ENDIF

Z_SRC = $(Z_BASE_ENV)
Z_SRC_CONCAT = $(Z_BASE_ENV);$(Z_BASE_A)
Z_INC_CONCAT = $(Z_BASE_ENV);$(Z_BASE_A);$(Z_BASE)\$(Z_ENV)\incz;$(Z_BASE)\$(Z_ENV)\incr;$(Z_BASE)\a\incz;$(Z_BASE)\a\incr;$(INCLUDE)

Z_OBJ_GBL = $(Z_BASE_ENV)\$(Z_OBJ)
LIBENTRY = $(Z_OBJ_GBL)\LIBENTRY.obj


CL_OPT =  -DWINVER=0x030A -W3 -Zp -G2 -Gw -I$(Z_BASE_ENV) -I$(Z_BASE_A)
LINK_OPT = /NOD /NOE /ONERROR:NOEXE /M /A:16

!IF "$(DEBUG)" == "1"
CL_OPT = -Zi -DDEBUG -D_DEBUG -Od $(CL_OPT)
LINK_OPT = /CO $(LINK_OPT)
!ELSE
CL_OPT = -Od $(CL_OPT)
!ENDIF

DLLFLAG = -c $(MODEL) $(CL_OPT) -NT$(@B)

!IFDEF USER_OPT
DLLFLAG = $(DLLFLAG) $(USER_OPT)
!ENDIF

OBJS = $(Z_OBJ)\$(MODULE).obj

{$(Z_BASE_A)}.c.cod :
   $(CC) @<<
$(DLLFLAG) /Fc /Fo$(Z_OBJ)\$(@B).obj $(Z_BASE_A)\$(@B).$(MODULE_SRC)
<<

{$(Z_BASE_A)}.c.i :
   $(CC) @<<
$(DLLFLAG) /P /Fo$(Z_OBJ)\$(@B).obj $(Z_BASE_A)\$(@B).$(MODULE_SRC)
<<

{$(Z_BASE_A)}.c{$(Z_OBJ)}.obj :
   $(CC) @<<
$(DLLFLAG) /Fo$(Z_OBJ)\$(@B).obj $(Z_BASE_A)\$(@B).$(MODULE_SRC)
<<

clean :
    -del $(Z_OBJ)\$(MODULE).obj

$(Z_SRC)\$(MODULE).res:  $(Z_SRC)\$(MODULE).rc
    rc -r $(Z_SRC)\$(MODULE).rc $(Z_SRC)\$(MODULE).res

$(Z_BIN)\$(MODULE).$(MODULE_EXT) : $(OBJS) \
                                   $(Z_SRC)\$(MODULE).res
   link @<<$(MODULE).lnk
$(LINK_OPT) +
$(OBJS:  = +^
)
$(Z_BIN)\$(MODULE).$(MODULE_EXT)
$(Z_BASE_ENV)\$(MODULE).map
$(LINKDLLS) zdrapp zdctl tzlodopr
$(Z_BASE_ENV)\$(MODULE).dfb
<<KEEP
   qmap2def $(Z_BASE_ENV)\$(MODULE).dfb $(Z_BASE_ENV)\$(MODULE).map > $(Z_BASE_ENV)\$(MODULE).def
   link @<<
$(LINK_OPT) +
$(OBJS:  = +^
)
$(Z_BIN)\$(MODULE).$(MODULE_EXT)
NUL
$(LINKDLLS) zdrapp zdctl tzlodopr
$(Z_BASE_ENV)\$(MODULE).def
<<
   implib $(Z_LIB)r\$(MODULE).lib $(Z_BASE_ENV)\$(MODULE).def
   rc  $(Z_SRC)\$(MODULE).res $(Z_BIN)\$(MODULE).dll

