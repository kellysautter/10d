del class\zeidon\*.class
del kzoejava.obj
del ..\..\w\bin\kzoejava.dll
nmake -f kzoejava.mak %1 %2 %3
call distrib
