@echo Off
IF "%1" == "/?" cdh
call envspace free
set KZVP=%KZV%

REM =========== ZPV ==================
@ECHO OFF
if "%3" == "" goto :venv
if "%3" == "." goto :venv
%qbin%\checkit %3 10E 10D 10C 10B Zeidon -X1
IF errorlevel 1 goto :get_v
set KZV=%3
%qbin%\checkit %3 10E 10D 10C 10B Zeidon -E1
IF errorlevel 1 goto :v_exit
if {%3}=={10E} set KZV=10E
if {%3}=={10D} set KZV=10D
if {%3}=={10C} set KZV=10C
if {%3}=={10B} set KZV=10B
if {%3}=={10A} set KZV=10A
goto :v_exit
:venv
%qbin%\checkit %KZV% 10E 10D 10C 10B 10A -E1
if errorlevel 1 goto :vq_exit
:get_v
@echo The Zeidon Product Version must be set before continuing
@echo.
@echo Enter 1 for 10E
@echo Enter 2 for 10D
@echo Enter 3 for 10C
@echo Enter 4 for 10B
@echo Enter 5 for 10A
@echo.
%qbin%\qask "Default is 10E: " /t0 12345 /d1
if errorlevel 5 goto :set10A
if errorlevel 4 goto :set10B
if errorlevel 3 goto :set10C
if errorlevel 2 goto :set10D
:set10E
set KZV=10E
goto :v_exit
:set10D
set KZV=10D
goto :v_exit
:set10C
set KZV=10C
goto :v_exit
:set10B
set KZV=10B
goto :v_exit
:set10A
set KZV=10A
:v_exit
@echo.
:vq_exit
if "%4" == "/v" goto :fini
if "%KZVP%" == "" set kzvp=%KZV%
if "%KZV%" == "%KZVP%" goto :nochg
@echo.
@echo  CAUTION: You are changing VERSION from %KZVP% to %KZV% for CD? commands,
@echo           but you may need to issue CENV . %KZV% for a 'complete'
@echo           environment change...
@echo.
pause 
:nochg

REM =========== ZPT ==================
@ECHO OFF
if "%1" == "" goto :ttenv
if "%1" == "." goto :ttenv
%qbin%\checkit %1 A 2 W N 5 X -X1
IF errorlevel 1 goto :get_tt
set kztt=%1
%qbin%\checkit %1 A 2 W N 5 X -E1
IF errorlevel 1 goto :tt_exit
if {%1}=={a} set kztt=A
if {%1}=={w} set kztt=W
if {%1}=={x} set kztt=X
goto :tt_exit
:ttenv
%qbin%\checkit %KZTT% A 2 W N 5 X -E1
if errorlevel 1 goto :ttq_exit
:get_tt
@echo The Zeidon Target must be set before continuing
@echo.
@echo Enter      for:
@echo  1 or a     Base    (A)
@echo  2          OS/2    (2)
@echo  3 or w     Windows (W)
@echo  4 or n     WinNT   (N)
@echo  5          Win95   (5)
@echo  6 or x     Unix    (X)
@echo.
%qbin%\qask "Default is Windows: " /t0 1a223w4n556x /d3
if errorlevel 11 goto :settx
if errorlevel 9 goto :sett5
if errorlevel 7 goto :settn
if errorlevel 5 goto :settw
if errorlevel 3 goto :sett2
set kztt=A
goto :tt_exit
:sett2
set kztt=2
goto :tt_exit
:settw
set kztt=w
goto :tt_exit
:settn
set kztt=n
goto :tt_exit
:sett5
set kztt=5
goto :tt_exit
:settx
set kztt=x
:tt_exit
@echo.
:ttq_exit

if "%KZTT%" == "a" goto :tx_exit
if "%KZTT%" == "A" goto :tx_exit
set kzt=%KZTT%
:tx_exit

%qbin%\checkit %KZT% 2 W N 5 X -E1
if errorlevel 1 goto :tq_exit
:get_t
goto :setw
@echo The Zeidon Product Target must be set before continuing
@echo.
@echo Enter      for:
@echo  2          OS/2    (2)
@echo  3 or w     Windows (W)
@echo  4 or n     WinNT   (N)
@echo  5          Win95   (5)
@echo  6 or x     Unix    (X)
@echo.
%qbin%\qask "Default is Windows: " /t0 223w4n556x /d3
if errorlevel 9 goto :setx
if errorlevel 7 goto :set5
if errorlevel 5 goto :setn
if errorlevel 3 goto :setw
if errorlevel 1 goto :set2
:set2
set kzt=2
goto :t_exit
:setw
set kzt=w
goto :t_exit
:setn
set kzt=n
goto :t_exit
:set5
set kzt=5
goto :t_exit
:setx
set kzt=x
:t_exit
@echo.
:tq_exit

REM =========== ZPS ==================
@ECHO OFF
if "%2" == "" goto :senv
if "%2" == "." goto :senv
%qbin%\checkit %2 OE DR UT AP TG VE TZ MS ED DB AX AXC ZDR ZDX SAK JAVA Z D O S J X T -X1
IF errorlevel 1 GOTO get_s
set kzs=%2
%qbin%\checkit %2 OE DR UT AP TZ MS ZDR ZDX JAVA -E1
IF errorlevel 1 goto :s_exit
if {%2}=={oe} set kzs=OE
if {%2}=={OE} set kzs=OE
if {%2}=={o}  set kzs=OE
if {%2}=={O}  set kzs=OE
if {%2}=={dr} set kzs=ZDR
if {%2}=={DR} set kzs=ZDR
if {%2}=={ut} set kzs=UT
if {%2}=={UT} set kzs=UT
if {%2}=={ap} set kzs=AP
if {%2}=={AP} set kzs=AP
if {%2}=={ss} set kzs=SS
if {%2}=={SS} set kzs=SS
if {%2}=={tg} set kzs=TZ\AX\TrueGrid
if {%2}=={TG} set kzs=TZ\AX\TrueGrid
if {%2}=={ve} set kzs=TZ\AX\tzvmledt
if {%2}=={VE} set kzs=TZ\AX\tzvmledt
if {%2}=={tz} set kzs=TZ
if {%2}=={TZ} set kzs=TZ
if {%2}=={t}  set kzs=TZ
if {%2}=={T}  set kzs=TZ
if {%2}=={ms} set kzs=MS
if {%2}=={MS} set kzs=MS
if {%2}=={DB} set kzs=TZ\AX\MsDbGrid
if {%2}=={db} set kzs=TZ\AX\MsDbGrid
if {%2}=={ed} set kzs=TZ\AX\_EDIT
if {%2}=={ED} set kzs=TZ\AX\_EDIT
if {%2}=={ax} set kzs=TZ\AX
if {%2}=={AX} set kzs=TZ\AX
if {%2}=={axc} set kzs=TZ\AX\TZAXCTL
if {%2}=={AXC} set kzs=TZ\AX\TZAXCTL
if {%2}=={z}  set kzs=ZDR
if {%2}=={Z}  set kzs=ZDR
if {%2}=={s}  set kzs=SAK
if {%2}=={S}  set kzs=SAK
if {%2}=={d}  set kzs=ZDR
if {%2}=={D}  set kzs=ZDR
if {%2}=={zdr} set kzs=ZDR
if {%2}=={ZDR} set kzs=ZDR
if {%2}=={sak} set kzs=SAK
if {%2}=={Sak} set kzs=SAK
if {%2}=={SAK} set kzs=SAK
if {%2}=={x}  set kzs=ZDX
if {%2}=={X}  set kzs=ZDX
if {%2}=={zdx} set kzs=ZDX
if {%2}=={ZDX} set kzs=ZDX
if {%2}=={j}  set kzs=JAVA
if {%2}=={J}  set kzs=JAVA
if {%2}=={java} set kzs=JAVA
if {%2}=={JAVA} set kzs=JAVA
goto :s_exit
:senv
%qbin%\checkit %KZS% OE DR UT SS AP TZ MS TZ\AX TZ\AX\_EDIT TZ\AX\TZAXCTL ZDR ZDX SAK JAVA -E1
if errorlevel 1 goto :sq_exit
:get_s
@echo The Zeidon Product SubSystem must be set before continuing
@echo.
@echo Enter   for                       Enter   for 
@echo   1     Object Engine (OE)          m     Message Handler  (MS)
@echo   2     Driver        (DR)          e     Operation Editor (ED)
@echo   3     Zeidon Tools  (TZ)          a     Zeidon ActiveX   (AX)
@echo   4     Utilities     (UT)          d     Zeidon NT Driver (ZDR)
@echo   5     Test Apps     (AP)          j     Java Lite Driver (JAVA)
@echo   c     ActiveX Ctrls (AXC)         x     Zeidon Remote Driver (ZDX)
@echo   s     SAK           (SAK)
@echo                                     
@echo.
%qbin%\qask "Default is Zeidon NT Driver: " /t0 12345cmeadxj /d10
if errorlevel 13 goto :setjava
if errorlevel 12 goto :setsak
if errorlevel 11 goto :setzdx
if errorlevel 10 goto :setzdr
if errorlevel 9 goto :setax
if errorlevel 8 goto :seted
if errorlevel 7 goto :setms
if errorlevel 6 goto :setaxc
if errorlevel 5 goto :setap
if errorlevel 4 goto :setut
if errorlevel 3 goto :settz
if errorlevel 2 goto :setdr
set kzs=OE
goto :s_exit
:setut
set kzs=UT
goto :s_exit
:setdr
set kzs=DR
goto :s_exit
:setap
set kzs=AP
goto :s_exit
:setss
set kzs=TZ\AX\TZAXCTL
goto :s_exit
:setms
set kzs=MS
goto :s_exit
:setax
set kzs=TZ\AX
goto :s_exit
:setzdr
set kzs=ZDR
goto :s_exit
:setzdx
set kzs=ZDX
goto :s_exit
:setsak
set kzs=SAK
goto :s_exit
:setjava
set kzs=JAVA
goto :s_exit
:seted
set kzs=TZ\AX\_EDIT
goto :s_exit
:settz
set kzs=TZ
goto :s_exit
:s_exit
@echo.
:sq_exit
:fini
