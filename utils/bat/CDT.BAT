@echo OFF
IF "%1" == "/?" cdh
if {%2}=={} goto :checkit
%2:
:checkit
checkit \%KZV% /d /e1
if errorlevel 1 goto :rel-on-drive
if not {%2}=={} echo Directory \%KZV% not present on drive %2: -- Switching to %KZD%:
if {%2}=={} echo Directory \%KZV% not present on current drive -- Switching to %KZD%:
%KZD%:
:rel-on-drive
if {%1}=={} goto :use-kztt
checkit \%KZV%\%1 /d /e1
if errorlevel 1 goto :dir-ok
goto :err
:use-kztt
checkit \%KZV%\%KZTT% /d /e1
if errorlevel 1 goto :dir-kztt
goto :err
:err
echo Directory \%KZV%\%1 not present for Release %KZV%.
goto :fini
:dir-ok
if {%1}=={a} set kztt=A
if {%1}=={A} set kztt=A
if {%1}=={w} set kztt=W
if {%1}=={W} set kztt=W
cd \%KZV%\%1
goto :fini
:dir-kztt
cd \%KZV%\%KZTT%
goto :fini
:fini
