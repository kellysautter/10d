#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Make for Java support
#-------------------------------------------------------------------

!IFNDEF Z_ENV
!IFDEF  KZT
Z_ENV = $(KZT)
!ELSE
Z_ENV = w
!ENDIF
!ENDIF

Z_BASE      = ..\..
Z_BASE_JAVA = $(Z_BASE)\a\java

Z_LIB     = $(Z_BASE)\$(Z_ENV)\lib
Z_BIN     = $(Z_BASE)\$(Z_ENV)\bin
Z_SRC     = $(Z_BASE_JAVA)
Z_INC     = $(Z_BASE)\a\inc
Z_INC_ENV = $(Z_BASE)\$(Z_ENV)\inc

Z_CLASSPATH = $(CLASSPATH);.\class
Z_CLASSDIR  = .\class
Z_CLASS     = $(Z_CLASSDIR)\zeidon\zView.class \
              $(Z_CLASSDIR)\zeidon\zQual.class

ALL: $(Z_CLASS) kzoejava.h $(Z_BIN)\kzoejava.dll

$(Z_CLASS): $(@B).java
   qspawn !$(@B).err javac $(@B).java -version -d $(Z_CLASSDIR)

kzoejava.h: $(Z_CLASSDIR)\zeidon\zView.class
   del kzoejava.h
   javah -o kzoejava.h -jni zeidon.zView

$(Z_BIN)\kzoejava.dll: kzoejava.c
   qspawn kzoejava.err cl @<<
-c -MDd -LDd -D__WIN32__ -W3 -Zp -Z7 -DDEBUG -D_DEBUG -Od kzoejava.c
-I "$(JAVA_HOME)\include"
-I "$(JAVA_HOME)\include\win32"
<<
   @qspawn +kzoejava.err link @<<
kzoejava.obj kzoengaa.lib kzrpsrco.lib zdrapp.lib "$(JAVA_HOME)\lib\jvm.lib"
/DEBUG /DLL /SUBSYSTEM:WINDOWS
/OUT:$(Z_BIN)\kzoejava.dll
/IMPLIB:$(Z_LIB)r\kzoejava.lib
<<
#kzoejava.obj kzoengaa.lib javai.lib javai_g.lib
