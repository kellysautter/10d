del *.pdb /s
del *.sbr /s
del *.idb /s
del *.ilk /s
del *.map /s
del *.err /s
del *.ler /s
del *.res /s
del *.tmp /s
del *.rpt /s
del *.bak /s
del x.bat /s

ren w\bin\mergedef.exe mergedef.exe.hld
ren w\bin\qpreproc.exe qpreproc.exe.hld
ren w\libr\JpegLib.lib JpegLib.lib.hld
ren w\libr\ZdCtl.lib ZdCtl.lib.hld
ren w\libr\ZDrApp.lib ZDrApp.lib.hld
ren w\libr\kzrpsrco.lib kzrpsrco.lib.hld
ren w\libz\tzlodopr.lib tzlodopr.lib.hld
ren w\libz\tzvmledt.lib tzvmledt.lib.hld
ren w\libz\truegrid.lib truegrid.lib.hld
ren w\libz\truegrid.lib truegrid.lib.hld
ren w\libz\tzedfrmd.lib tzedfrmd.lib.hld
ren w\libz\tzpnctad.lib tzpnctad.lib.hld
ren w\libz\tzoprusd.lib tzoprusd.lib.hld

del w\bin\*.exe
del w\bin\*.dll
del col*.obj /s
del hz*.obj /s
del account.obj /s
del waccount.obj /s
del j*.obj /s
del z*.obj /s
del z*.lib /s
del t*.obj /s
del t*.lib /s
del kz*.obj /s
del kz*.lib /s
del account.lib /s
del waccount.lib /s
del xarraydb.obj /s

del regex.obj /s
del tbedit.obj /s
del metersec.obj /s
del decari.obj /s
del mergedef.obj /s
del tbedit.lib /s
del zwinsock.lib /s
del tbedit.lib /s
del truegrid.lib /s
del zdecimal.lib /s
del ZSIMPDIR.LIB /s

del *.ipch /s
del *.log /s
del *.exp /s
del *.tlog /s

ren w\bin\mergedef.exe.hld mergedef.exe
ren w\bin\qpreproc.exe.hld qpreproc.exe
ren w\libr\JpegLib.lib.hld JpegLib.lib
ren w\libr\ZdCtl.lib.hld ZdCtl.lib
ren w\libr\ZDrApp.lib.hld ZDrApp.lib
ren w\libr\kzrpsrco.lib.hld kzrpsrco.lib
ren w\libz\tzlodopr.lib.hld tzlodopr.lib
ren w\libz\tzvmledt.lib.hld tzvmledt.lib
ren w\libz\truegrid.lib.hld truegrid.lib
ren w\libz\tzedfrmd.lib.hld tzedfrmd.lib
ren w\libz\tzpnctad.lib.hld tzpnctad.lib
ren w\libz\tzoprusd.lib.hld tzoprusd.lib

dir *.obj /s
dir *.lib /s
dir *.exe /s
dir *.dll /s