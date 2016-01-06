cls
del \10d\w\bin\tzvmlip.dll
del \10d\w\bin\tzvmlip.ilk
del \10d\w\bin\tzvmlip.pdb
del \10d\w\tz\debug\tzvmlip.pdb
del \10d\w\tz\debug\tzvmlip.obj
cl /MDd /c -DDEBUG -D_DEBUG -Od /Gz /W3 /Gm /EHsc /Zi /Od /Zp /DZEIDON /D__WIN32__ /D_WINDOWS  -TP  ..\..\a\tz\tzvmlip.c  -Fo..\..\w\tz\debug\tzvmlip.obj
link              /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
pause
del \10d\w\tz\debug\tzvmlip.obj
cl /MDd /c -DDEBUG -D_DEBUG -Od /RTC /W3    /EHsc /Zi /Od /Zp /DZEIDON /D__WIN32__ -DWIN32   -TP  ..\..\a\tz\tzvmlip.c  -Fo..\..\w\tz\debug\tzvmlip.obj
link              /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
pause
del \10d\w\tz\debug\tzvmlip.obj
cl /MDd /c -DDEBUG -D_DEBUG -Od /Gz /W3 /Gm /EHsc /Zi /Od /Zp /DZEIDON /D__WIN32__             -TP  ..\..\a\tz\tzvmlip.c  -Fo..\..\w\tz\debug\tzvmlip.obj
link              /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
pause
del \10d\w\tz\debug\tzvmlip.obj
cl /MDd /c -DDEBUG -D_DEBUG -Od /Gz /W3 /Gm /EHsc /Zi /Od /Zp /DZEIDON /D__WIN32__             -TP  ..\..\a\tz\tzvmlip.c  -Fo..\..\w\tz\debug\tzvmlip.obj
link              /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
pause
del \10d\w\tz\debug\tzvmlip.obj
cl /MDd -c -DDEBUG -D_DEBUG -Od     /W3           /Zi     -Zp -DZEIDON -D__WIN32__ -DWIN32 -nologo -FAs -Foc:\10d\w\tz\debug\tzvmlip -Ic:\10d\w\tz -Ic:\10d\A\tz  /Fdc:\10d\w\tz\debug\tzvmlip.PDB -TP c:\10d\a\tz\tzvmlip.c
link              /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
pause

qpreproc __WIN32__=1 c:\10d\w\tz\tzvmlip.def > C:\TEMP\tzvmlip.def
link /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
rem link /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   htmlhelp.lib  c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
dir \10d\tzvmlip.* /s

link /VERBOSE:lib /debug:full /subsystem:windows /nologo /dll /OUT:..\..\w\bin\tzvmlip.dll  /MAP:tzvmlip.MAP /PDB:..\..\w\bin\tzvmlip.PDB       /DEF:..\..\w\tz\tzvmlip.def  /IMPLIB:..\..\w\libz\tzvmlip.lib  mfcs140.lib /NOD /machine:I386 /debug                                 ..\..\w\tz\debug\tzvmlip.obj kzoengaa.lib
link              /debug:full /subsystem:windows /nologo /dll /OUT:..\..\w\bin\tzvmlip.dll  /MAP:tzvmlip.MAP /PDB:..\..\w\bin\tzvmlip.PDB       /DEF:..\..\w\tz\tzvmlip.def  /IMPLIB:..\..\w\libz\tzvmlip.lib  user32.lib                                                                      ..\..\w\tz\debug\tzvmlip.obj kzoengaa.lib
link              /DEBUG:FULL /SUBSYSTEM:windows /nologo /DLL /OUT:c:\10d\w\bin\tzvmlip.DLL /MAP:tzvmlip.MAP /PDB:c:\10d\w\tz\debug\tzvmlip.pdb /DEF:C:\10d\w\tz\tzvmlip.DEF /IMPLIB:c:\10d\w\libz\tzvmlip.LIB user32.lib advapi32.lib kernel32.lib shell32.lib uuid.lib gdi32.lib   c:\10d\w\tz\debug\tzvmlip.OBJ  kzoengaa.lib
