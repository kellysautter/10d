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

:: ==============
:: Make Core
:: ==============
if "%MakeOE%" == "" goto :MakeDR

cd \%kzv%\w\oe
cd zdecimal\decari
set CFG=zdecimal - Win32 Debug
%callmake% %kzd%:\%kzv%\w\oe\zdecimal\decari\zdecimal.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

cd \%kzv%\w\oe
%callmake% %kzd%:\%kzv%\mak\kzoengaa.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

:: ==============
:: Make driver
:: ==============
:MakeDR
if "%MakeDR%" == "" goto :MakeMS

cd \%kzv%\a\zdr

set CFG=ZDrApp - Win32 Debug
%callmake% zdrapp.mak %1 %2 %3 %4 %5 %6 %7 %8 %9

set CFG=ZdCtl - Win32 Debug
%callmake% zdctl.mak  %1 %2 %3 %4 %5 %6 %7 %8 %9

cd \%kzv%\w\tz

%callmake% tzapdmaa.mak %1 %2 %3 %4 %5 %6
%callmake% tzvmlip.mak  %1 %2 %3 %4 %5 %6
%callmake% tzweb.mak  %1 %2 %3 %4 %5 %6
%callmake% tzlodopr.mak %1 %2 %3 %4 %5 %6
%callmake% tzobrwad.mak %1 %2 %3 %4 %5 %6
%callmake%

cd \%kzv%\a\zdr

set CFG=TzCtl - Win32 Debug
%callmake% tzctl.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

set CFG=Z2MP - Win32 Debug
%callmake% z2mp.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

set CFG=ZSEMail - Win32 Debug
%callmake% zsemail.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

goto :SkipZDx

cd \%kzv%\a\zdx

set CFG=ZDrApp - Win32 Debug
%callmake% zdrapp.mak %1 %2 %3 %4 %5 %6 %7 %8 %9

set CFG=ZdCtl - Win32 Debug
%callmake% zdctl.mak  %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

cd \%kzv%\a\zdx
set CFG=ZDr - Win32 Debug
%callmake% zdr.mak    %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

:SkipZDx

cd \%kzv%\a\tz\ax\truegrid
set CFG=truegrid - Win32 Debug
%callmake% truegrid.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

cd \%kzv%\a\tz\ax\tzaxctl
set CFG=TzAxCtl - Win32 Debug
%callmake% tzaxctl.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

cd \%kzv%\a\tz\ax\_edit
set CFG=tbedit - Win32 Debug
%callmake% tbedit.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

cd \%kzv%\a\zdr
set CFG=ZDr - Win32 Debug
%callmake% zdr.mak    %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

set CFG=

:: ==============
:: Make Message
:: ==============
:MakeMS
if "%MakeMS%" == "" goto :MakeOE2

cd \%kzv%\w\ms
%callmake% kzmsgqaa.mak %1 %2 %3
%callmake%

:: ==============
:: Make DBH
:: ==============
:MakeOE2
if "%MakeOE%" == "" goto :MakeTZ1

:: make java classes
cd \%kzv%\a\java
call m

cd \%kzv%\w\oe
%callmake% %kzd%:\%kzv%\mak\kzoedbha.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

:: ==============
:: Make Some Tools (tzapdmaa.dll and tzlodopr.dll)
:: ==============
:MakeTZ1
if "%MakeTZ%" == "" goto :MakeOE3

:: ==============
:: Make Browser
:: ==============
:MakeOE3

:: Make the browser if making either OE or TZ
if "%MakeOE% %MakeTZ%" == " " goto :MakeTZ2

cd \%kzv%\w\tz
%callmake% tzapdmaa.mak %1 %2 %3 %4 %5 %6
%callmake% tzvmlip.mak  %1 %2 %3 %4 %5 %6
%callmake% tzweb.mak %1 %2 %3 %4 %5 %6
%callmake% tzlodopr.mak %1 %2 %3 %4 %5 %6
%callmake% tzobrwad.mak %1 %2 %3 %4 %5 %6
%callmake%

:: ==============
:: Make rest of Tools
:: ==============
:MakeTZ2
if "%MakeTZ%" == "" goto :MakeRebase

cd \%kzv%\w\tz
%callmake% tzapdmaa.mak %1 %2 %3 %4 %5 %6
%callmake% tzvmlip.mak  %1 %2 %3 %4 %5 %6
%callmake% tzweb.mak %1 %2 %3 %4 %5 %6
%callmake% tzlodopr.mak %1 %2 %3 %4 %5 %6
%callmake% tzobrwad.mak %1 %2 %3 %4 %5 %6

%callmake% tzvmlopr.mak %1 %2 %3 %4 %5 %6
%callmake% tzvmpopr.mak %1 %2 %3 %4 %5 %6
%callmake% tzadcdod.mak %1 %2 %3 %4 %5 %6
%callmake% tzadwdod.mak %1 %2 %3 %4 %5 %6
%callmake% tzbrentd.mak %1 %2 %3 %4 %5 %6
%callmake% tzbraudo.mak %1 %2 %3 %4 %5 %6
%callmake% tzbrau2o.mak %1 %2 %3 %4 %5 %6

%callmake% kzrpsrco.mak %1 %2 %3 %4 %5 %6

%callmake% tzcmrevd.mak %1 %2 %3 %4 %5 %6
%callmake% tzcmlpld.mak %1 %2 %3 %4 %5 %6
%callmake% tzcmcpld.mak %1 %2 %3 %4 %5 %6
%callmake% tzcmlpmd.mak %1 %2 %3 %4 %5 %6
%callmake% tzcmrptd.mak %1 %2 %3 %4 %5 %6
%callmake% tzcmslpd.mak %1 %2 %3 %4 %5 %6
%callmake% tzedcmpd.mak %1 %2 %3 %4 %5 %6
%callmake% tzedfrmd.mak %1 %2 %3 %4 %5 %6
%callmake% tzdmupdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzeremdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzhkeyad.mak %1 %2 %3 %4 %5 %6
%callmake% tzophdrd.mak %1 %2 %3 %4 %5 %6
%callmake% tzopsigd.mak %1 %2 %3 %4 %5 %6
%callmake% tzopupdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzpeupdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzoprusd.mak %1 %2 %3 %4 %5 %6
%callmake% tzpnctad.mak %1 %2 %3 %4 %5 %6
%callmake% tzpnctld.mak %1 %2 %3 %4 %5 %6
%callmake% tzpntbad.mak %1 %2 %3 %4 %5 %6
%callmake% tzpntrad.mak %1 %2 %3 %4 %5 %6
%callmake% tzxsltdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzpncwdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzrpupdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzpnupgd.mak %1 %2 %3 %4 %5 %6
%callmake% tzrepoad.mak %1 %2 %3 %4 %5 %6
%callmake% tztedbld.mak %1 %2 %3 %4 %5 %6
%callmake% tzteupdd.mak %1 %2 %3 %4 %5 %6
%callmake% tzvsdbad.mak %1 %2 %3 %4 %5 %6
%callmake% tzvsxcad.mak %1 %2 %3 %4 %5 %6
%callmake% tzwdgrwd.mak %1 %2 %3 %4 %5 %6
%callmake% tzwdvord.mak %1 %2 %3 %4 %5 %6
%callmake% tzzmaind.mak %1 %2 %3 %4 %5 %6
%callmake% tzzolodd.mak %1 %2 %3 %4 %5 %6
%callmake% tzzolodq.mak %1 %2 %3 %4 %5 %6
rem %callmake% tzsiimpd.mak %1 %2 %3 %4 %5 %6
%callmake% tzdbhtsd.mak %1 %2 %3 %4 %5 %6
%callmake% tzpnolad.mak %1 %2 %3 %4 %5 %6

set CFG=tzaxcfgd - Win32 Debug
%callmake% tzaxcfgd.mak %1 %2 %3 %4 %5 %6
%callmake%

cd \%kzv%\a\tz\ax\tzvmledt
set CFG=tzvmledt - Win32 Debug
%callmake% tzvmledt.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

cd \%kzv%\w\tz
set CFG=tzedvmld - Win32 Debug
%callmake% tzedvmld.mak %1 %2 %3 %4 %5 %6 %7 %8 %9
%callmake%

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
