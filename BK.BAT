@echo off
if {%2}=={} goto :help
%KZD%:
cd\temp
del .
QBB %KZD%:\10E\*.* -S -D%1 -T%2 -xstate.*;*.bak;*.lib;*.err;*.map;*.sym -NE -F%%s
if errorlevel 1 goto :dups
QBB %KZD%:\10E\*.* -S -D%1 -T%2 -xstate.*;*.bak;*.lib;*.err;*.map;*.sym "-Fcopy %%s"
qsortfil x.bat xx.bat
qelimdup xx.bat x.bat
call x.bat
z bk *.* -m
uz bk -t
dir *.zip
quit
:dups
ECHO Duplicate file names encountered ... 
quit
:help
ECHO Two paramters are require%KZD%: Date and Time
