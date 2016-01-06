#-------------------------------------------------------------------
# DESCRIPTION: QuinSoft Zeidon Make for Java support
#  Linux version.
#-------------------------------------------------------------------

SHELL = /bin/sh
kz_bin   = $(KZV)/x/bin
kz_obj   = $(KZV)/x/oe/obj
kz_src   = $(KZV)/a/java
kz_incza = $(KZV)/a/incz
kz_inczx = $(KZV)/x/incz
kz_incra = $(KZV)/a/incr
kz_incrx = $(KZV)/x/incr

kz_inc   = -I $(kz_inczx) -I $(kz_incza) -I $(kz_incrx) -I $(kz_incra) -I $(kz_src) -I /usr/jdk/include -I /usr/jdk/include/linux
cc_opt   = $(kz_inc) -D __UNIX__ -D _REENTRANT -D DEBUG -fpic -ggdb -o $@

Z_BASE      = ..\..
Z_BASE_JAVA = $(Z_BASE)\a\java

Z_LIB     = $(Z_BASE)\$(Z_ENV)\lib
Z_BIN     = $(Z_BASE)\$(Z_ENV)\bin
Z_SRC     = $(Z_BASE_JAVA)
Z_INC     = $(Z_BASE)\a\inc
Z_INC_ENV = $(Z_BASE)\$(Z_ENV)\inc

Z_CLASSPATH = $(CLASSPATH):./class
Z_CLASSDIR  = ./class
Z_CLASS     = $(Z_CLASSDIR)/zeidon/zView.class \
              $(Z_CLASSDIR)/zeidon/zQual.class \
              $(Z_CLASSDIR)/zeidon/ZeidonViewHttpSessionListener.class \
              $(Z_CLASSDIR)/Tester.class

.SUFFIXES: .class .java

# For some reason Linux needs BOTH kzoejava.so and libkzoejava.so,
# even though they are the same.
ALL: $(Z_CLASS) kzoejava.h $(kz_bin)/kzoejava.so $(kz_bin)/libkzoejava.so

$(Z_CLASSDIR)/Tester.class : Tester.java
	javac -classpath $(Z_CLASSPATH) Tester.java -d ./class

%.class:
	../../x/bin/qspawn $(*F).err javac $(*F).java -d $(Z_CLASSDIR) -classpath $(Z_CLASSPATH)

kzoejava.h: $(Z_CLASSDIR)/zeidon/zView.class
	rm -f kzoejava.h
	javah -o kzoejava.h  -classpath $(Z_CLASSPATH) -jni zeidon.zView

$(kz_bin)/kzoejava.so: $(kz_src)/kzoejava.c
	../../x/bin/qspawn kzoejava.err gcc $(cc_opt) $(kz_src)/kzoejava.c $(kz_bin)/kzoengaa.so -shared -Wl,-soname,$(kz_bin)/kzoejava.so -g -ldl -lm
	echo kzoejava.so compiled OK!

$(kz_bin)/libkzoejava.so: $(kz_src)/kzoejava.c
	../../x/bin/qspawn kzoejava.err gcc $(cc_opt) $(kz_src)/kzoejava.c $(kz_bin)/kzoengaa.so -shared -Wl,-soname,$(kz_bin)/libkzoejava.so -g -ldl -lm
	echo libkzoejava.so compiled OK!
