MODULE     = qspawn
ZBASE      = $(KZD):\$(KZV)
TARGET_DIR = $(ZBASE)\w\bin

ALL: $(TARGET_DIR)\$(MODULE).exe

!ifdef DEBUG
CC   = cl -Zi -W3 -Od /D_CRT_SECURE_NO_WARNINGS
LINK = /CO /MAP
!else
CC   = cl -W3 /D_CRT_SECURE_NO_WARNINGS
LINK =
!endif

!if "$(BUILDOS)" == "win32"
CC = $(CC) -D__WIN32__
!endif

.c.obj:
    $(CC) -c $*.c > $*.err

$(TARGET_DIR)\$(MODULE).exe : $(MODULE).obj
!if "$(BUILDOS)" == "win32"
   link @<<
qspawn.obj /out:$(KZD):\$(KZV)\w\bin\qspawn.exe
<<
!else
   link $(LINK) /MAP /ST:16000 $(MODULE), $(TARGET_DIR)\$(MODULE), $(MODULE).map, , ,
!endif
