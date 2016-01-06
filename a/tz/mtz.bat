echo off
:: New mtz to make tools that have been converted for 9J
set mtzn=1

del *.err
::
if not "%1" == "" set mtzn_last=%1
if "%mtzn_last%" == "" call mtznlast.bat

call cds %kzt% tz

call title Making %mtzn_last%

echo nmake -f %mtzn_last%.mak %2 %3 %4 %5 %6 %7 %8 %9
nmake -f %mtzn_last%.mak -nologo %2 %3 %4 %5 %6 %7 %8 %9
call cds a tz

call title Command Prompt

copy *.err x.err /y
type x.err |more
del x.err

:: Store value of mtzn_last to a file for later use.
echo set mtzn_last=%mtzn_last%> mtznlast.bat

set mtzn=
