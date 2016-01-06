/*----------------------------------------------------------------------
   lmncb.h -- Network Control Block definitions for NetBIOS

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef LMNCB_H
#define LMNCB_H

typedef char LM_FILEID;
#define HDR MTYPE mtype;

#include "lmc.h"


#define NETNAME_LEN  16
#define cbNETNAME    16

#ifndef NULL
#define NULL (char *)0
#endif

#define NETBIOS_INT 0x5c

#define  ADDNAME        (unsigned char)0x30
#define  DELNAME        (unsigned char)0x31
#define  RESET          (unsigned char)0x32
#define  STATUS         (unsigned char)0x33
#define  SESSTAT        (unsigned char)0x34
#define  nCANCEL        (unsigned char)0x35
#define  CALL           (unsigned char)0x10
#define  LISTEN         (unsigned char)0x11
#define  HANGUP         (unsigned char)0x12
#define  SEND           (unsigned char)0x14
#define  nbChainSend    (unsigned char)0x17
#define  RECEIVE        (unsigned char)0x15
#define  ADDNAIT        (unsigned char)0xB0
#define  DELNAIT        (unsigned char)0xB1
#define  CALNAIT        (unsigned char)0x90
#define  LISNAIT        (unsigned char)0x91
#define  HANNAIT        (unsigned char)0x92
#define  SENNAIT        (unsigned char)0x94
#define  RECNAIT        (unsigned char)0x95
#define  ARENAIT        (unsigned char)0x96
#define  ANY            (unsigned char)0xFF
#define  PENDING        0xFFFF

#define  NCBSIZE        64

#define  nbReceiveAny   (unsigned char)0x16
#define  nbNoWait       (unsigned char)0x80

#define  nbPending      (unsigned char)0xFF

#define  nbMsgIncomplete (unsigned char)0x06


/* -------------------- NetBIOS Status codes -------------------------------*/

#define N_BUSY          0xff
#define N_OKAY          0x00
#define N_NOTOPEN       0x01
#define N_NONETBIOS     0x03
#define N_TIMEOUT       0x05
#define N_SESCLOSED     0x0a
#define N_CANCEL        0x0b
#define N_DUPNAME       0x0d
#define N_TABFULL       0x11
#define N_OPENREJ       0x12
#define N_CALLNAME      0x14
#define N_DEREGISTERED  0x15
#define N_NAMEUSED      0x16
#define N_NAMEDEL       0x17
#define N_SESABORT      0x18
#define N_INTFBUSY      0x21
#define N_COMMANDS      0x22
#define N_HARDERROR     0x40

#define NETBIOS_INVALID_COMMAND 0x7F

/*==========================================================================*/

/***** NCB Commands parameter block *****/
typedef  struct   ncb
   {
   char           command;       /* NETBIOS Command */
   unsigned char  retcode;       /* Return value (some enumerated below) */
   unsigned char  lsn;           /* Local session number */
   unsigned char  num;           /* Name number */
   void far *     buffer;
   int length;                   /* Length of message buffer */
   char           callname[16];  /* Name of user being called */
   char           name[16];      /* My name */
   unsigned char  rto;           /* Receive timeout (500ms units) */
   unsigned char  sto;           /* Send timeout (500ms units) */
   void (far *    post)();
   unsigned char  lana_num;      /* Local area network adapter number (use 0) */
   unsigned char  cmd_cplt;      /* Final return value when posting           */
   char           reserve[14];   /* Reserved for NetBIOS */
   }
   NCB;


typedef struct    sChainBuffer
   {
   short cbExtra;
   void far *pvExtra;
   }
   EX, *pEX;

typedef struct
   {
   char rep_sess;
   char name_sess;
   char gram;
   char rcvany;
   struct
   {
      char lsn;
      char state_sess;
      char loc_name[16];
      char rem_name[16];
      char rcvs;
      char sends;
      }
      s[32];
   }
   SESSION;


typedef struct    AdapterStatus
   {
   unsigned char  UnitID[6];
   unsigned char  Jumpers;
   unsigned char  SelfTest;
   unsigned char  VerMajor;
   unsigned char  VerMinor;

   short          Duration;
   short          CRCErrs;
   short          AlignErrs;
   short          Collision;
   short          Aborts;
   long           Transmits;
   long           Receives;
   short          Retries;
   short          Resource;

   unsigned char  reserved1[8];
   short          NCBfree;
   short          NCBconfig;
   short          NCBMax;
   unsigned char  reserved2[4];
   short          PendSess;
   short          MaxPend;
   short          MaxSess;
   short          MaxSize;
   short          NameCount;
   unsigned char  Name[32][18];
   }
   AdStat, near * npAdStat, far * pAdStat;

#endif
/* vpp @win.vpp LMNCB.H (Mon Jun 01 07:53:23 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

