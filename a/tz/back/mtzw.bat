if "%1" == "" goto :makeall
qrw *.c /t3
qrw *.vml /t3
cd ..\zdr
qrw *.cpp /t3
qrw *.h /t3
cd ..\oe
qrw *.c /t3
qrw *.cpp /t3
qrw *.h /t3
cd ..\incr
qrw *.h /t3
cd ..\tz

:makeall
shift
del *.ler
del *.err
call cdv
call makeall ms %1 %2 %3 %4 %5 %6 %7 %8 %9
call cdv
call makeall tz %1 %2 %3 %4 %5 %6 %7 %8 %9
call cds a tz
dir *.ler
dir *.err /os
