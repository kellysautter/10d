MODULE     = qspawn
ZBASE      = $(KZD):\$(KZV)
TARGET_DIR = $(ZBASE)\w\bin

ALL: setenv $(TARGET_DIR)\$(MODULE).exe

!ifdef DEBUG
CC   = cl -Zi -W3 -Od
LINK = /CO /MAP
!else
CC   = cl -W3
LINK =
!endif

setenv:
   SET INCLUDE=%C-DIR%\MSC\INC;%C-DIR%\MSC\INC\SYS
   SET LIB=%C-DIR%\MSC\LIB
   SET PATH=%C-DIR%\MSC\BIN

.c.obj:
    $(CC) -c $*.c > $*.err

$(TARGET_DIR)\$(MODULE).exe : $(MODULE).obj
   link $(LINK) /MAP /ST:16000 $(MODULE), $(TARGET_DIR)\$(MODULE), $(MODULE).map, , ,
