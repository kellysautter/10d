# rebase.mak "rebases" all the Zeidon runtime DLLs.

# static_files contain all files that should not be rebased but should be
# used in determining the new base for the other DLLs.
static_files = xmlparse.dll \
               xmltok.dll

# rebase_files is a list of all files that can potentially be rebased.
rebase_files = kzhgenky.dll \
               kzhsqloa.dll \
               kzhsqlxx.dll \
               kzmsgqaa.dll \
               kzoengaa.dll \
               tzapdmaa.dll \
               zdctl.dll    \
               tzctl.dll    \
               zdecimal.dll \
               zdrapp.dll   \
               z2mp.dll     \
               zsemail.dll  \
               zwinsock.dll

!ifdef DB2SDK
rebase_files = $(rebase_files) kzhsqlda.dll
!endif

!ifdef SQLBASESDK
rebase_files = $(rebase_files) kzhsqlba.dll
!endif

# rebase.log is a temp file that is used to determine if any of the DLLs have
# been recompiled since the last rebase.
All: rebase.log

rebase.log: $(static_files) $(rebase_files)
   echo Temp file used to rebase Zeidon DLLs -- do not erase > rebase.log
   rebase -b 0x68000000 -l rebase.log -d -R . -v $(static_files) $(rebase_files)

CLEAN:
   qdelete $(rebase_files)

