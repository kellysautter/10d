if "%1" == "" goto :e2c
qad \10c \10d /s /rv /bc /x@qcx.exc
qrw ad.rpt /re=S
type ad.rpt |more
quit
:e2c
qad \10d \10c /s /rv /bc /x@qcx.exc
qrw ad.rpt /re=S
type ad.rpt | more

