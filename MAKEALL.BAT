%At98%echo off
if '%1' == 'DBG' GoTo Debug
if '%1' == 'dbg' GoTo Debug
GoTo StartBat

:Debug
echo on
shift
GoTo StartBat

MAKEALL.BAT

This makefile makes all the Zeidon files (at least for Win32).

Programming Notes:

   The env var callmake is set to call \%kzv%\CallMake.bat.  This var is used
   to actually call nmake.exe to run the makefiles.  If nmake.exe returns
   with an error CallMake.bat changes the var callmake to goto :End.  This
   will cause MAKEALL.BAT to stop making files the next time callmake is
   used.

   Before we change directories after calling CallMake.bat we execute %callmake%
   again so that we quit processing if there was an error.  This allows us to
   stay in the same directory that had the error.

2001.03.22  DGC
   I had to change the order of compilation.  DBH stuff now needs to go after
   the driver.

:PrintHelp
echo.
echo MAKEALL.BAT -- Compiles all Zeidon stuff for Win32.
echo.
echo  format:
echo "    MAKEALL [ oe ] [ dr ] [ tz ] [ ms ] [ all ] [ CLEAN | IGN-ERR ]"
echo             [ nmake flags... ]
echo.
echo  If MAKEALL is called with no arguments it will attempt to make all Zeidon
echo  stuff.  If any arguments are specified then stuff will be made only if
echo  it's group is specified (eg. oe, dr, etc.).
echo.
echo  For example, nothing will be made for the following:
echo     MAKEALL -a
echo.
echo  What's needed is a group qualifier:
echo     MAKEALL oe dr -a
echo  will cause all core and driver stuff to be made.
echo.
echo  Normally MAKEALL will stop making if there are any errors.  Specifying
echo  IGN-ERR will cause MAKEALL to attempt to make everything even if there
echo  are errors.  Makes that cause errors will be listed in %kzd%:\%kzv%\MAKEALL.ERR.
echo.
quit

:StartBat
if "%kztt%" == "" set kztt=a
if "%kzt%" == "" set kzt=w
if "%1" == "?"   goto :PrintHelp
if "%1" == "/?"  goto :PrintHelp
if "%1" == "-?"  goto :PrintHelp

set MakeOE=Y
set MakeTZ=Y
set MakeDR=Y
set MakeMS=Y
set IGNORE_ERROR=

if "%1" == "" goto :START

set MakeOE=
set MakeTZ=
set MakeDR=
set MakeMS=

:ReadArgs
if "%1" == "TZ"  goto :SetTZ
if "%1" == "tz"  goto :SetTZ
if "%1" == "OE"  goto :SetOE
if "%1" == "oe"  goto :SetOE
if "%1" == "dr"  goto :SetDR
if "%1" == "DR"  goto :SetDR
if "%1" == "ms"  goto :SetMS
if "%1" == "MS"  goto :SetMS
if "%1" == "all" goto :SetALL
if "%1" == "ALL" goto :SetALL
if "%1" == "IGN-ERR" goto :SetError
if "%1" == "ign-err" goto :SetError
if "%1" == "IGNERR"  goto :SetError
if "%1" == "ignerr"  goto :SetError
goto :START

:SetTZ
set MakeTZ=Y
shift
GOTO :ReadArgs

:SetMS
set MakeMS=Y
shift
GOTO :ReadArgs

:SetOE
set MakeOE=Y
shift
GOTO :ReadArgs

:SetDR
set MakeDR=Y
shift
GOTO :ReadArgs

:SetALL
set MakeOE=Y
set MakeDR=Y
set MakeMS=Y
set MakeTZ=Y
shift
GOTO :ReadArgs

:: Set stuff up so that if there is an error making something the make will
:: still continue.
:SetError
set IGNORE_ERROR=Y
shift

Echo Make errors... > %kzd%:\%kzv%\makeall.err

GOTO :ReadArgs

:START
if "%MakeOE% %MakeDR% %MakeMS% %MakeTZ%" == "   " goto :PrintHelp

set callmake=call \%kzv%\callmake

:MakeRebase
if not "%MakeOE% %MakeDR%" == "Y Y" GoTo :MakeEnd

:: Try rebasing all the DLLs
cd \%kzv%\w\bin
%callmake% rebase.mak %1 %2 %3 %4 %5 %6

:MakeEnd
cd \%kzv%

if "%IGNORE_ERROR%" == "Y" type makeall.err

:End
title Command Prompt
