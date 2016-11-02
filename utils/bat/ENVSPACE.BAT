@echo off
if {%1} == {free} goto free
if {%1} == {FREE} goto free
if {%1} == {alloc} goto alloc
if {%1} == {ALLOC} goto alloc
@echo. 
@echo  Purpose: %0 will free or alloc space taking environment variables
@echo    Usage: %0 mode big 
@echo           mode := free or alloc
@echo           big  := if parameter is non-blank then more space is reserved
@echo.
goto fini
:free
set envspac0=
set envspac1=
set envspac2=
set envspac3=
goto fini
:alloc
set envspac0=123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-
set envspac1=123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-
if not {%2}=={} set envspac2=123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-
if not {%2}=={} set envspac3=123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-
:fini
