call cds w tz
del ..\..\a\tz\*.err
del ..\..\a\tz\*.ler

set mtzn=1

nmake -f tzapdmaa.mak %1 %2 %3 %4 %5 %6
nmake -f tzvmlip.mak  %1 %2 %3 %4 %5 %6

nmake -f tzlodopr.mak %1 %2 %3 %4 %5 %6
nmake -f tzvmlopr.mak %1 %2 %3 %4 %5 %6
nmake -f tzvmpopr.mak %1 %2 %3 %4 %5 %6
nmake -f tzpntrad.mak %1 %2 %3 %4 %5 %6

nmake -f tzadcdod.mak %1 %2 %3 %4 %5 %6
nmake -f tzadwdod.mak %1 %2 %3 %4 %5 %6
nmake -f tzbrentd.mak %1 %2 %3 %4 %5 %6
nmake -f tzcmlpld.mak %1 %2 %3 %4 %5 %6
nmake -f tzcmcpld.mak %1 %2 %3 %4 %5 %6
nmake -f tzcmlpmd.mak %1 %2 %3 %4 %5 %6
nmake -f tzcmrptd.mak %1 %2 %3 %4 %5 %6
nmake -f tzcmslpd.mak %1 %2 %3 %4 %5 %6
nmake -f tzedcmpd.mak %1 %2 %3 %4 %5 %6
nmake -f tzedfrmd.mak %1 %2 %3 %4 %5 %6
nmake -f tzdmupdd.mak %1 %2 %3 %4 %5 %6
nmake -f tzeremdd.mak %1 %2 %3 %4 %5 %6
nmake -f tzhkeyad.mak %1 %2 %3 %4 %5 %6
nmake -f tzophdrd.mak %1 %2 %3 %4 %5 %6
nmake -f tzoprusd.mak %1 %2 %3 %4 %5 %6
nmake -f tzopsigd.mak %1 %2 %3 %4 %5 %6
nmake -f tzopupdd.mak %1 %2 %3 %4 %5 %6
nmake -f tzpeupdd.mak %1 %2 %3 %4 %5 %6
nmake -f tzpnctad.mak %1 %2 %3 %4 %5 %6
nmake -f tzpntrad.mak %1 %2 %3 %4 %5 %6
nmake -f tzpnctld.mak %1 %2 %3 %4 %5 %6
nmake -f tzpncwdd.mak %1 %2 %3 %4 %5 %6
nmake -f tzpnupgd.mak %1 %2 %3 %4 %5 %6
nmake -f tzrpupdd.mak %1 %2 %3 %4 %5 %6
nmake -f tzrepoad.mak %1 %2 %3 %4 %5 %6
nmake -f tztedbld.mak %1 %2 %3 %4 %5 %6
nmake -f tzteupdd.mak %1 %2 %3 %4 %5 %6
nmake -f tzvsdbad.mak %1 %2 %3 %4 %5 %6
nmake -f tzvsxcad.mak %1 %2 %3 %4 %5 %6
nmake -f tzwdgrwd.mak %1 %2 %3 %4 %5 %6
nmake -f tzwdvord.mak %1 %2 %3 %4 %5 %6
nmake -f tzzmaind.mak %1 %2 %3 %4 %5 %6
nmake -f tzzolodd.mak %1 %2 %3 %4 %5 %6
rem nmake -f tzsiimpd.mak %1 %2 %3 %4 %5 %6
nmake -f tzdbhtsd.mak %1 %2 %3 %4 %5 %6
nmake -f tzpnolad.mak %1 %2 %3 %4 %5 %6
nmake -f kzrpsrco.mak %1 %2 %3 %4 %5 %6
nmake -f tzobrwad.mak %1 %2 %3 %4 %5 %6
set mtzn=

