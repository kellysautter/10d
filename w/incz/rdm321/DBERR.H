/*----------------------------------------------------------------------
   dberr.h -- text of error messages used by dberr function

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef DBERR_H
#define DBERR_H

/* dberr error/status messages */
static char *user_error[] = {
   "database not opened",
   "invalid set",
   "invalid record",
   "invalid database",
   "invalid field name",
   "invalid db_address",
   "no current record",
   "set has no current owner",
   "set has no current member",
   "key value required",
   "invalid lock value",
   "record is owner of non-empty set(s)",
   "record is member of set(s)",
   "member already owned",
   "field is a compound key",
   "record not connected to set",
   "field is not a valid key",
   "record not legal owner of set",
   "record not legal member of set",
   "error in d_setpages (database open or bad param)",
   "incompatible dictionary file",
   "illegal attempt to delete system record",
   "illegal attempt to lock locked set/record",
   "attempt to access unlocked set/record",
   "transaction id not supplied",
   "transaction already active",
   "transaction not active",
   "transaction cannot begin due to locked files",
   "cannot free locks within a transaction",
   "too many changed database pages",
   "cannot update database outside a transaction",
   "exclusive access required",
   "write locks not allowed on static files",
   "unspecified or duplicate user id",
   "database, path or file name too long",
   "invalid file number was passed to d_renfile",
   "field is not an optional key field",
   "field is not defined in current record type",
   "record/field has/in a compound key",
   "invalid record or set number",
   "record/set not timestamped",
   "bad DBUSERID (contains non-alphanumeric)",
   "NetBIOS has not been installed on machine",
   "wrong # of elements in DBDPATH",
   "wrong # of elements in DBFPATH",
   "no current record type",
   "invalid country table sort string",
   "database not closed",
   "invalid pointer",
   "invalid internal ID",
   "invalid lockmgr communication type",
   "invalid task",
   "Lock Manager Communication not initilized",
   "option is not implemented in this version"
};

/* dberr system error messages */
static char *system_error[] = {
   "no more space on file",
   "system error",
   "page fault",
   "no working file in dio",
   "unable to allocate sufficient memory",
   "error opening file",
   "unable to get access to database taf file",
   "database taf/log file error",
   "inconsistent database locks",
   "file record limit exceeded",
   "key file inconsistency",
   "maximum concurrent user limit exceeded",
   "error seeking in database file",
   "invalid file specified",
   "error reading from a database/overflow file",
   "lock manager synchronization error",
   "debug check interrupt",
   "network communications error",
   "auto-recovery about to occur",
   "error writing to a database/overflow file",
   "no lock manager is installed",
   "DBUSERID is already being used",
   "the lock manager is busy",
   "attempt to lock discarded memory",
   "Semaphore error",
   "network SEND or RECEIVE timed out",
   "network SESSION closed",
   "",
   "no network resources available",
   "network session open rejected",
   "network name not found",
   "network name already in use",
   "network name is deregistered, but not available",
   "network session ended abnormally",
   "too many outstanding network commands",
   "",
   "network hardware error",
   "error closing file",
   "db_QUERY not linked with vista.dll",
   "unable to update file, opened with READONLY option",
   "file in use",
   "share not loaded",
   "must run in protected mode",
   "recovery failed",
   "TAF file / lockmgr synchronization error",
   "Block IO files must be multiple of 512",
   ""
};

#endif
/* vpp @win.vpp DBERR.h (Fri Jun 05 10:23:37 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

