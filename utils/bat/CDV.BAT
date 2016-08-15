@echo OFF
cd\
if "%1" == "/?" cdh
if {%2}=={} goto :chk1
%2:
:chk1
if {%1}=={} goto :use-kzv
checkit \%1 /d /e1
if errorlevel 1 goto :rel-on-drive
if not {%2}=={} echo Directory \%1 not present on drive %2: -- Switching to %KZD%:
if {%2}=={} echo Directory \%1 not present on current drive -- Switching to %KZD%:
goto :use-kzd
:use-kzv
checkit \%KZV% /d /e1
if errorlevel 1 goto :rel-on-drive
if not {%2}=={} echo Directory \%KZV% not present on drive %2: -- Switching to %KZD%:
if {%2}=={} echo Directory \%KZV% not present on current drive -- Switching to %KZD%:
goto :use-kzd
:use-kzd
%KZD%:
if {%1}=={} goto :kzv
checkit \%1 /d /e1
if errorlevel 1 goto :rel-on-drive
echo  Directory \%1 not present
goto :fini
:kzv
checkit \%KZV% /d /e1
if errorlevel 1 goto :rel-on-drive
echo  Directory \%KZV% not present
goto :fini
:rel-on-drive
if {%1}=={} goto :dir-kzv
cd \%1
goto :fini
:dir-kzv
cd \%KZV%
goto :fini
:fini

