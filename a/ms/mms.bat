CALL CDS %kzt% MS
set mtzn=1
nmake -f kzmsgqaa.mak %1 %2 %3
set mtzn=
CALL CDS A MS
