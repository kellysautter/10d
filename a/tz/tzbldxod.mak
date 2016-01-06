ALL: OclnCsv.exe
!include "d:\lplr\zencas\zenv.hnm"
!ifdef DEBUG
CC   = cl /W3 -Zp -DWIN32 -DDEBUG -Od -Z7
LINK = /MAP /SUBSYSTEM:console /DEBUG:FULL
!else
CC   = cl /W3 -Zp -DWIN32 -Oselg
LINK = /MAP /SUBSYSTEM:native
!endif

OBJS = tzbldxod.obj

.c.obj:
    qspawn $*.err $(CC) -c $*.c -D__WIN32__ -DZEIDON

tzbldxod.exe : $(OBJS)
   link $(LINK) @<<
/OUT:OclnCsv.exe
/MAP:OclnCsv.map
$(OBJS) kzoengaa.lib $(LINKDLLS) objlib\ZGLOBAL1.lib wininet.lib tzvmlip.lib
<<
