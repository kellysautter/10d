/*------------------------------------------------------------------------
   lmc.h -- Raima Data Manager LMC header file
            used by lmc_ modules and lockmgr.

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
--------------------------------------------------------------------------*/

#pragma pack(1)


/* ========================================================================
 * communication headers for use with lockmgr
 * HDR must be defined before this file is included
 * ========================================================================
 */

typedef struct MESSAGE_HDR_S
{
  HDR
} MESSAGE_HDR;

typedef struct MESSAGE_S
{
   MESSAGE_HDR hdr;
   char     mtext[1];
} MESSAGE;

typedef struct HM_MESSAGE_S
{
   MESSAGE_HDR hdr;
   LM_MESSAGE  lm_message;
} HM_MESSAGE;

typedef struct HR_MESSAGE_S
{
   MESSAGE_HDR hdr;
   LR_MESSAGE  lr_message;
} HR_MESSAGE;


#define HDR_SIZE  sizeof(MESSAGE_HDR)

typedef struct HM_LOGIN_S
{
   MESSAGE_HDR hdr;
   LM_LOGIN    lm_login;
   LM_FILEID   lm_fileid;
}  HM_LOGIN;

typedef struct HM_DBOPEN_S
{
   MESSAGE_HDR hdr;
   LM_DBOPEN   lm_dbopen;
}  HM_DBOPEN;

typedef struct HM_DBCLOSE_S
{
   MESSAGE_HDR hdr;
   LM_DBCLOSE  lm_dbclose;
}  HM_DBCLOSE;

typedef struct HM_LOCK_S
{
   MESSAGE_HDR hdr;
   LM_LOCK     lm_lock;
}  HM_LOCK;

typedef struct HM_RECOVERED_S
{
   MESSAGE_HDR hdr;
   LM_RECOVERED lm_recovered;
}  HM_RECOVERED;

typedef struct HM_FREE_S
{
   MESSAGE_HDR hdr;
   LM_FREE     lm_free;
}  HM_FREE;

typedef struct HM_TRCOMMIT_S
{
   MESSAGE_HDR hdr;
   LM_TRCOMMIT lm_trcommit;
}  HM_TRCOMMIT;

typedef struct HM_TREND_S
{
   MESSAGE_HDR hdr;
   LM_TREND    lm_trend;
}  HM_TREND;

typedef struct HM_SETTIME_S
{
   MESSAGE_HDR hdr;
   LM_SETTIME  lm_settime;
}  HM_SETTIME;

typedef struct HM_LOGOUT_S
{
   MESSAGE_HDR hdr;
   LM_LOGOUT lm_logout;
}  HM_LOGOUT;

typedef struct HM_TERMINATE_S
{
   MESSAGE_HDR hdr;
   LM_TERMINATE lm_terminate;
}  HM_TERMINATE;


/* Message packets, Lock Manager to Runtime */

typedef struct HR_LOGIN_S
{
   MESSAGE_HDR hdr;
   LR_LOGIN    lr_login;
}  HR_LOGIN;

typedef struct HR_DBOPEN_S
{
   MESSAGE_HDR hdr;
   LR_DBOPEN   lr_dbopen;
}  HR_DBOPEN;

typedef struct HR_LOCK_S
{
   MESSAGE_HDR hdr;
   LR_LOCK     lr_lock;
}  HR_LOCK;

typedef union
{
   HM_LOGIN       hm_login;
   HM_DBOPEN      hm_dbopen;
   HM_DBCLOSE     hm_dbclose;
   HM_LOCK        hm_lock;
   HM_RECOVERED   hm_recovered;
   HM_FREE        hm_free;
   HM_TRCOMMIT    hm_trcommit;
   HM_TREND       hm_trend;
   HM_SETTIME     hm_settime;
   HM_LOGOUT      hm_logout;
   HM_TERMINATE   hm_terminate;
   HR_LOGIN       hr_login;
   HR_DBOPEN      hr_dbopen;
   HR_LOCK        hr_lock;
} MESSAGE_PKT;

#define MAX_PCKT_LEN (sizeof (MESSAGE_PKT) + (size_ft+1)*sizeof(LM_FILEID))

#pragma pack()

#ifdef ZOR
/* Zortech's locking() call can get into an infinate loop */
int _pascal Locking(int, int, long);
#define locking Locking
#endif

/* vpp @win.vpp LMC.H (Wed Jun 24 13:58:54 1992)
 *
 *     -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS
 *     -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC
 *     -nARCHIVING -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES
 *     -dMULTI_TASK -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP
 *     -nSINGLE_USER -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4
 *     -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY -nTRUENAME -nLAT
 */

