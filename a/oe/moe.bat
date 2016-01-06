echo off
set PATH=%PATH%;"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin"
@if "%kzt%" == "a" set kzt=w
@if "%kzt%" == "A" set kzt=w
%kzd%:
del *.err /s
del *.ler /s
call cds %kzt% oe
del *.err /s
del *.ler /s
cd zdecimal\decari
set CFG=zdecimal - Win32 Debug
nmake -f zdecimal.mak %1 %2 %3 %4 %5
cd ..\..
call moe %1 %2 %3 %4 %5
call mdbh %1 %2 %3 %4 %5
call cds a
if not '%buildos%' == 'win32' call mob
qff *.err /q /12
qff *.err /q /14
cd ..\..\w\oe
qff *.err /q /12
qff *.err /q /14
cd ..\..\a\oe
dir *.err /os
dir ..\..\w\oe\*.err