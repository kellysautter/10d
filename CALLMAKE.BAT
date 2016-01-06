:: This bat file is called from MAKEALL.BAT.  Look there for info.

:: If there are no arguments just exit.  This allows MakeAll.BAT call this
:: file to check if all previous makes were OK.
if "%1" == "" goto :End

echo.
echo ---------- Making %1 ----------------------------
echo.
call title Making %1...

nmake -nologo -f %1 %2 %3 %4 %5 %6 %7 %8 %9
if not ERRORLEVEL 1 goto :End

echo.
echo ERROR trying to make %1 %2 %3 %4 %5 %6 %7 %8 %9
echo.

if "%IGNORE_ERROR%" == "" goto :StopMake

echo.                                                 >> %kzd%:\%kzv%\makeall.err
echo ERROR trying to make %1 %2 %3 %4 %5 %6 %7 %8 %9  >> %kzd%:\%kzv%\makeall.err

goto :End

:: Set variables that will cause MakeAll.BAT to stop.
:StopMake
set makeerror=%1 %2 %3 %4 %5 %6 %7 %8 %9
set callmake=goto :End

:End
echo Done making %1
