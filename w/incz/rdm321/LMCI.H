
/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  440 05-Dec-88 RSC Moved defn of MESSAGE to dbtype.h
*/

#ifndef LMCI_H
#define LMCI_H

typedef char LM_FILEID;
#define HDR MTYPE mtype;

#include "lmc.h"


#ifdef TURBO
#ifdef __SMALL__
#define SMALL_DATA
#endif
#ifdef __MEDIUM__
#define SMALL_DATA
#endif
#endif

#ifdef ZOR
#ifdef __SMALL__
#define SMALL_DATA
#endif
#ifdef __MEDIUM__
#define SMALL_DATA
#endif
#endif



#define L_PENDING 10
#define MAXTRIES 50
#define EMPTY (-1)
#define DEFAULT_TIMEOUT 10

#define DEFUSERS 32
#define DEFFILES 128
#define DEFQUEUE 256
#define USERBYTES  ((DEFUSERS-1)/8+1)
#define FILEBYTES  ((DEFFILES-1)/8+1)

/* User table status */
#define U_OKAY          0
#define U_EMPTY         0

#define U_LIVE          1
#define U_DEAD          2
#define U_BEING_REC     3
#define U_RECOVERING    4
#define U_REC_MYSELF    5
#define U_HOLDING_X     6

/* Insure byte alignment regardless of compiler */
#pragma pack(1)


typedef struct
{
   char DB_FAR *ptr;
} GENERAL_P;


typedef struct USERTAB_S
{
   char u_name[16];                    /* user name, from np_listen() */
   GENERAL_P U_open;                      /* bit map of open files */
#ifdef SMALL_DATA
   int u_filler_1;                     /* for mixed memory models */
#endif
   GENERAL_P U_lock;                      /* bit map of locked files */
#ifdef SMALL_DATA
   int u_filler_2;                     /* for mixed memory models */
#endif
   GENERAL_P U_req;                       /* bit map of incomplete lock */
#ifdef SMALL_DATA
   int u_filler_3;                     /* for mixed memory models */
#endif
   int u_pending;                      /* count of pending lock requests */
   int u_status;                       /* indicates "holding exclusive */
   char u_logfile[LOGFILELEN];
}  USERTAB;


typedef struct FILETAB_S
{
   char f_name[FILENMLEN];             /* full name of this file */
   int f_lockstat;                     /* lock status */
   GENERAL_P F_open;                      /* bit map of users having this
                                        * file open */
#ifdef SMALL_DATA
   int f_filler_1;                     /* for mixed memory models */
#endif
   GENERAL_P F_lock;                      /* bit map of users having this
                                        * file locked */
#ifdef SMALL_DATA
   int f_filler_2;                     /* for mixed memory models */
#endif
   int f_queue;                        /* pointer to head of lock request
                                        * queue */
}  FILETAB;

typedef struct REQQUEUE_S
{
   char q_locktype;                    /* requested lock type */
   int q_user;                         /* user waiting for this lock */
   int q_next;                         /* next request for this file */
}  REQQUEUE;

#pragma pack()


typedef struct INTERNAL_USERTAB_S
{
   MESSAGE DB_FAR *u_last_msg;         /* Last message from user */
   MTYPE u_last_mtype;                 /* Last Message type from user */
   int u_waiting;                      /* flag indicating user is ready */
   int u_timeout;
   int u_timer;
   int u_recuser;
} INTERNAL_USERTAB;

typedef struct
{
   struct FILETAB_S DB_FAR *ptr;
      LOCK_DESC
}  FILETAB_P;

typedef struct
{
   struct REQQUEUE_S DB_FAR *ptr;
      LOCK_DESC
}  REQQUEUE_P;

typedef struct
{
   struct USERTAB_S DB_FAR *ptr;
      LOCK_DESC
}  USERTAB_P;

typedef struct
{
   struct INTERNAL_USERTAB_S DB_FAR *ptr;
      LOCK_DESC
}  INTERNAL_USERTAB_P;

typedef struct G_GLOBAL_S
{
   USERTAB_P Usertab;
   USERTAB_P Userend;
   INTERNAL_USERTAB_P I_usertab;
   INTERNAL_USERTAB_P I_userend;
   FILETAB_P Filetab;
   FILETAB_P Fileend;
   REQQUEUE_P Req_queue;
   REQQUEUE_P Reqend;
   short Maxusers;
   short Numusers;
   int   Usertabsize;
   short Maxfiles;
   short Numfiles;
   int   Filetabsize;
   short Maxqueue;
   short Numqueue;
   int   Queuesize;
   short Userbytes;
   short Filebytes;
   short Maxuserbytes;
   short Maxfilebytes;
   GENERAL_P Bitmap;
   int Bitmapsize;

   int Current_free_count;
   MESSAGE DB_FAR *Last_msg;

} G_GLOBAL;

typedef struct
{
   struct G_GLOBAL_S DB_FAR *ptr;
      LOCK_DESC
}  G_GLOBAL_P;

#define current_free_count   Gptr.ptr->Current_free_count

#define filetab   Gptr.ptr->Filetab.ptr
#define fileend   Gptr.ptr->Fileend.ptr
#define i_usertab Gptr.ptr->I_usertab.ptr
#define i_userend Gptr.ptr->I_userend.ptr
#define usertab   Gptr.ptr->Usertab.ptr
#define userend   Gptr.ptr->Userend.ptr
#define req_queue Gptr.ptr->Req_queue.ptr
#define reqend    Gptr.ptr->Reqend.ptr

#define f_open    F_open.ptr
#define f_lock    F_lock.ptr
#define u_req     U_req.ptr
#define u_open    U_open.ptr
#define u_lock    U_lock.ptr
#define rptr      Rptr.ptr
#define uptr      Uptr.ptr
#define fptr      Fptr.ptr
#define qptr      Qptr.ptr
#define i_uptr    I_uptr.ptr


#define maxusers  Gptr.ptr->Maxusers
#define numusers  Gptr.ptr->Numusers
#define maxfiles  Gptr.ptr->Maxfiles
#define numfiles  Gptr.ptr->Numfiles
#define maxqueue  Gptr.ptr->Maxqueue
#define numqueue  Gptr.ptr->Numqueue
#define userbytes Gptr.ptr->Userbytes
#define filebytes Gptr.ptr->Filebytes
#define maxuserbytes Gptr.ptr->Maxuserbytes
#define maxfilebytes Gptr.ptr->Maxfilebytes
#define last_msg     Gptr.ptr->Last_msg

#undef  bitmap
#define bitmap     Gptr.ptr->Bitmap.ptr

#define bitmapsize  Gptr.ptr->Bitmapsize
#define usertabsize Gptr.ptr->Usertabsize
#define filetabsize Gptr.ptr->Filetabsize
#define queuesize   Gptr.ptr->Queuesize

/*
 * Functions in LMC_FCNS.C
 */

extern int bit_set    (P1(char DB_FAR *) Pi(int));
extern int is_bit_set (P1(char DB_FAR *) Pi(int));
extern int is_map_zero(P1(char DB_FAR *) Pi(int));
extern int streq      (P1(char DB_FAR *) Pi(char DB_FAR *));
extern int bit_clr    (P1(char DB_FAR *) Pi(int));
extern int map_zero   (P1(char DB_FAR *) Pi(int));
extern int zeromem    (P1(char DB_FAR *) Pi(int));
extern int bit_or     (P1(char DB_FAR *) Pi(char DB_FAR *) Pi(int));

extern int  INTERNAL_FIXED l_dbopen(P1(int) Pi(USERTAB_P) Pi(struct HM_DBOPEN_S DB_FAR *) Pi(struct HR_DBOPEN_S DB_FAR *) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED l_dbclose(P1(int) Pi(struct HM_DBCLOSE_S DB_FAR *) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED l_lock(P1(int) Pi(struct HM_LOCK_S DB_FAR *) Pi(struct HR_LOCK_S DB_FAR *) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED l_free(P1(int) Pi(struct HM_FREE_S DB_FAR *) Pi(G_GLOBAL_P));
extern void INTERNAL_FIXED l_trcommit(P1(int) Pi(struct HM_TRCOMMIT_S DB_FAR *) Pi(G_GLOBAL_P));
extern void INTERNAL_FIXED l_trend(P1(int) Pi(G_GLOBAL_P));

extern int  INTERNAL_FIXED ins_head(P1(int) Pi(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED ins_tail(P1(int) Pi(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED ins_Record(P1(int) Pi(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED enter_user(P1(char DB_FAR *) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED get_q(P1(G_GLOBAL_P));
extern void INTERNAL_FIXED conv(P1(char DB_FAR *) Pi(char DB_FAR *));
extern void INTERNAL_FIXED lock_all(P1(G_GLOBAL_P));
extern int  INTERNAL_FIXED grant_lock(P1(int) Pi(int) Pi(int) Pi(G_GLOBAL_P));
extern void INTERNAL_FIXED lmc_reply(P1(int) Pi(struct HR_LOCK_S DB_FAR *) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED free_partial(P1(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED close_user(P1(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED free_pending(P1(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED close_file(P1(int) Pi(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED freelock(P1(int) Pi(int) Pi(G_GLOBAL_P));
extern int  INTERNAL_FIXED adj_users(P1(G_GLOBAL_P));
extern void INTERNAL_FIXED clean_mem(P1(G_GLOBAL_P));
extern int  INTERNAL_FIXED adj_files(P1(G_GLOBAL_P));
extern int  INTERNAL_FIXED free_q(P1(int) Pi(G_GLOBAL_P));
extern void INTERNAL_FIXED rep(P1(G_GLOBAL_P));
extern void INTERNAL_FIXED cvt_bin(P1(char DB_FAR *) Pi(char));
extern int  INTERNAL_FIXED init_gptr (P1(G_GLOBAL_P DB_FAR *));

extern void dbl_start_clock(P0);
extern int dbl_is_time_up(P0);

extern int IsShareLoaded;

#define open    _lopen
#define close   _lclose
#define write   _lwrite
#define read    _lread
#define lseek   _llseek
#define strrchr _fstrrchr
#define calloc  _fcalloc
#define malloc  _fmalloc
#define free    _ffree
extern far pascal Yield(void);
extern int QueryLoaded;
extern int _pascal qtask_switch(DB_TASK DB_FAR * );

#endif
/* vpp @win.vpp LMCI.H (Thu May 21 10:47:30 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

