/*----------------------------------------------------------------------
   dblock.h -- runtime/lockmgr common definitions and structures

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef DBLOCK_H
#define DBLOCK_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  368 28-Jul-88 RSC Integrate BSD changes into code
  115 16-Aug-88 RSC Integrate VAX/VMS changes into source code
  423 09-Sep-88 RSC Change variables to be compatible with MULTI_TASK
  420 07-Oct-88 RSC Unoptimized use of fl_list (full of bugs)
  441 09-Dec-88 RSC Modified defn of FCNPID for general lockmgr
      10-Sep-91 TWP Added NetBIOS error code for CANCEL'd NCB.
*/

/* Lock Manager/Runtime function interaction */

/* Function/Status codes */
#define L_LOCKHANDLE   -7
#define L_DUPUSER      -6
#define L_RECOVER      -5
#define L_QUEUEFULL    -4
#define L_TIMEOUT      -3
#define L_UNAVAIL      -2
#define L_SYSERR       -1

#define L_OKAY          0

/* mtype values for message to lockmgr */
#define L_DBOPEN        1
#define L_DBCLOSE       2
#define L_LOCK          3
#define L_FREE          4
#define L_TRCOMMIT      5
#define L_TREND         6
#define L_SETTIME       7
#define L_RECDONE       8
#define L_RECFAIL       9
#define L_LOGIN         10
#define L_DELETE        11
#define L_TERMINATE     12
#define L_LOGOUT        13


#ifndef LOGFILELEN
#define LOGFILELEN FILENMLEN
#endif

#define WORDPAD(a) ((sizeof(int)-(a)%sizeof(int))%sizeof(int))

/*==========================================================================*/

#pragma pack(1)


#define LM_HDR                      /* no header currently required */

#define LR_HDR DB_SHORT status;\
               char logfile[LOGFILELEN];/* header for each receive packet */


typedef struct DB_LOCKREQ_S
{
   DB_SHORT    fref;
   DB_SHORT    type;
}  DB_LOCKREQ;

typedef struct LM_MESSAGE_S
{
   LM_HDR
   char        mtext[1];
} LM_MESSAGE;

typedef struct LR_MESSAGE_S
{
   LR_HDR
   char        mtext[1];
} LR_MESSAGE;

typedef struct LM_LOGIN_S
{
   LM_HDR
   char        dbusrid[80];
}  LM_LOGIN;

typedef struct LM_DBOPEN_S
{
   LM_HDR
   DB_SHORT    nfiles;
   DB_SHORT    type;
   char        fnames[1];
}  LM_DBOPEN;

typedef struct LM_DBCLOSE_S
{
   LM_HDR
   DB_SHORT    nfiles;
   DB_SHORT    frefs[1];
}  LM_DBCLOSE;

typedef struct LM_LOCK_S
{
   LM_HDR
   DB_SHORT    nfiles;
   DB_LOCKREQ  locks[1];
}  LM_LOCK;

typedef struct LM_RECOVERED_S
{
   LM_HDR
   char        mtext[1];
}  LM_RECOVERED;

typedef struct LM_FREE_S
{
   LM_HDR
   DB_SHORT    nfiles;
   DB_SHORT    frefs[1];
}  LM_FREE;

typedef struct LM_TRCOMMIT_S
{
   LM_HDR
   char        logfile[LOGFILELEN];
}  LM_TRCOMMIT;

typedef struct LM_TREND_S
{
   LM_HDR
   char        mtext[1];
}  LM_TREND;

typedef struct LM_SETTIME_S
{
   LM_HDR
   DB_SHORT    secs;
}  LM_SETTIME;

typedef struct LM_LOGOUT_S
{
   LM_HDR
   char        mtext[1];
}  LM_LOGOUT;

typedef struct LM_TERMINATE_S
{
   LM_HDR
   char        mtext[1];
}  LM_TERMINATE;


/* Message packets, Lock Manager to Runtime */

typedef struct LR_LOGIN_S
{
   LR_HDR
   DB_SHORT    lsn;
}  LR_LOGIN;

typedef struct LR_DBOPEN_S
{
   LR_HDR
   DB_SHORT    nusers;
   DB_SHORT    nfiles;
   DB_SHORT    frefs[1];
}  LR_DBOPEN;

typedef struct LR_LOCK_S
{
   LR_HDR
}  LR_LOCK;

#pragma pack()

#endif /* DBLOCK_H */
/* vpp @win.vpp DBLOCK.h (Thu May 21 10:46:41 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

