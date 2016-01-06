/*----------------------------------------------------------------------
  vista.h: db_VISTA standard header file

   This file should be included in all programs which use db_VISTA.  It
   contains database status/error code definitions and standard type
   definitions.

   This file should only include user-oriented constructs.

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef VISTA_H
#define VISTA_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
      03-AUG-88 RTK Multi-tasking support - see "#ifdef MULTI_TASK"
      08-AUG-88 RTK Declared globals rn_dba, rn_type, and ft_offset to
                    support recfrst, recnext, etc in ONE_DB mode.
      16-Sep-88 RSC Fixed DBN_Dx to use variable # of arg macros.
      28-Sep-88 WLW Corrected the db_VERSION to 3.10 from 3.01.
      11-Oct-88 RSC Fixed DBN_Dx to use variable # of arg macros (again)
      11-Oct-88 RSC Fixed ONE_DB to compile correctly
      17-Oct-88 RSC Replaced MSC with ANSI, moved compiler check to dbtype
  439 17-Nov-88 RSC Placed some multi-tasking stuff within MULTI_TASK
  441 06-Dec-88 RSC Modifications to make gen lockmgr work with 3.10
      26-Jan-89 RSC Add defn for DB_UINT (for lockmgr)
      01-Sep-89 RTK Added tag name to LOCK_REQUEST typedef
      05-Jan-90 KGM Uses DB_VOID now
      26-Mar-90 KGM Dbata base number now required
      01-May-90 WLW Added option to NOT call SESSTAT.
      17-Dec-90 DPG Changed EXTERNAL_FIXED for OS2
 1430 30-Oct-91 DMB Full support of context switching using multiple caches
      20-Nov-91 TWP Added NETDELNAME and NOTRANS option switches
      02-Dec-91 BJB Added READONLY option switches
*/

#define db_VERSION "3.21A"
#define dbd_VERSION "V3.00\032"

/* -------------------status codes-------------------- */
/* user errors */
#define S_USER_FIRST -1                /* first user err */
#define S_DBOPEN    -1                 /* database not opened */
#define S_INVSET    -2                 /* invalid set */
#define S_INVREC    -3                 /* invalid record */
#define S_INVDB     -4                 /* invalid database */
#define S_INVFLD    -5                 /* invalid field name */
#define S_INVADDR   -6                 /* invalid db_address */
#define S_NOCR      -7                 /* no current record */
#define S_NOCO      -8                 /* set has no current owner */
#define S_NOCM      -9                 /* set has no current member */
#define S_KEYREQD   -10                /* key value required */
#define S_BADTYPE   -11                /* invalid lock type */
#define S_HASMEM    -12                /* record is owner of non-empty
                                        * set(s) */
#define S_ISMEM     -13                /* record is member of set(s) */
#define S_ISOWNED   -14                /* member already owned */
#define S_ISCOMKEY  -15                /* field is a compound key */
#define S_NOTCON    -16                /* record not connected to set */
#define S_NOTKEY    -17                /* field is not a valid key */
#define S_INVOWN    -18                /* record not legal owner of set */
#define S_INVMEM    -19                /* record not legal member of set */
#define S_SETPAGES  -20                /* error in d_setpages (database
                                        * open or bad param) */
#define S_INCOMPAT  -21                /* incompatible dictionary file */
#define S_DELSYS    -22                /* illegal attempt to delete system
                                        * record */
#define S_NOTFREE   -23                /* attempt to locked previously
                                        * locked rec or set */
#define S_NOTLOCKED -24                /* attempt to access unlocked
                                        * record or set */
#define S_TRANSID   -25                /* transaction id not be supplied */
#define S_TRACTIVE  -26                /* transaction already active */
#define S_TRNOTACT  -27                /* transaction not currently active */
#define S_TRFREE    -29                /* attempt to free a lock inside a
                                        * transaction */
#define S_NOTRANS   -31                /* attempted update outside of
                                        * transaction */
#define S_EXCLUSIVE -32                /* functions requires exclusive db
                                        * access */
#define S_STATIC    -33                /* Attempted to write lock a static
                                        * file */
#define S_USERID    -34                /* No user id exists */
#define S_NAMELEN   -35                /* database file/path name too long */
#define S_RENAME    -36                /* invalid file number was passed
                                        * to d_renfile */
#define S_NOTOPTKEY -37                /* field is not an optional key */
#define S_BADFIELD  -38                /* field not defined in current
                                        * record type */
#define S_COMKEY    -39                /* record/field has/in a compound
                                        * key */
#define S_INVNUM    -40                /* invalid record or set number */
#define S_TIMESTAMP -41                /* record or set not timestamped */
#define S_BADUSERID -42                /* invalid user id - not
                                        * alphanumeric */
#define S_NONETBIOS -43                /* NetBIOS is not installed on
                                        * machine */
#define S_NOTYPE    -46                /* No current record type */
#define S_INVSORT   -47                /* Invalid country table sort
                                        * string */
#define S_DBCLOSE   -48                /* database not closed */
#define S_INVPTR    -49                /* invalid pointer */
#define S_INVID     -50                /* invalid internal ID */
#define S_INVLOCK   -51                /* invalid lockmgr communication
                                        * type */
#define S_INVTASK   -52                /* invalid task id */
#define S_NOLOCKCOMM -53               /* d_lockcomm() must be called before
                                        * open in s or x modes */
#define S_NOTIMPLEMENTED -54
#define S_USER_LAST -54                /* last user error */

/* system errors */
#define S_SYSTEM_FIRST -900            /* first system error */
#define S_NOSPACE   -900               /* no more space on file */
#define S_SYSERR    -901               /* system error */
#define S_FAULT     -902               /* page fault -- changed during
                                        * usage */
#define S_NOWORK    -903               /* no working file set in dio */
#define S_NOMEMORY  -904               /* unable to allocate sufficient
                                        * memory */
#define S_NOFILE    -905               /* unable to locate a file */
#define S_DBLACCESS -906               /* unable to access db lock file */
#define S_DBLERR    -907               /* db lock file open/access error */
#define S_BADLOCKS  -908               /* inconsistent database locks */
#define S_RECLIMIT  -909               /* file record limit reached */
#define S_KEYERR    -910               /* key file inconsistency detected */
#define S_FSEEK     -912               /* Bad seek on database file */
#define S_READ      -914               /* Bad read on database/overflow
                                        * file */
#define S_NETSYNC   -915               /* Network synchronization error */
#define S_DEBUG     -916               /* Debugging check interrupt */
#define S_NETERR    -917               /* Network communications error */
#define S_RECOVERY  -918               /* Auto-recovery is about to occur */
#define S_WRITE     -919               /* Bad write on database/overflow
                                        * file */
#define S_NOLOCKMGR -920               /* Unable to open lockmgr session */
#define S_DUPUSERID -921               /* DBUSERID is already used by
                                        * another user */
#define S_LMBUSY    -922               /* The lock manager is busy */
#define S_DISCARDED -923               /* attempt to lock discarded memory */
#define S_SEM       -924               /* unable to open semaphore */

#define S_LMCERROR  -925               /* New Network layer error code    */

#define S_CLOSE     -937          /* Unable to close a file handle */
#define S_NODBQUERY -938               /* db_QUERY not linked with vista.dll*/
#define S_READONLY  -939               /* on_opt READONLY unable to update file */
#define S_EACCESS   -940               /* share violation, file in use */
#define S_NOSHARE   -941               /* share not loaded */
#define S_PROTECTED -942               /* must run in protected mode */
#define S_RECFAIL   -943               /* d_recover() failed */
#define S_TAFSYNC   -944               /* TAF file not consistant with LMC */
#define S_BLOCKIOPG -945               /* VMS block io problem */
#define S_SYSTEM_LAST -945             /* last system error */

/* function statuses */
#define S_FUNCTION_FIRST 0             /* first function status */
#define S_OKAY      0                  /* normal return, okay */
#define S_EOS       1                  /* end of set */
#define S_NOTFOUND  2                  /* record not found */
#define S_DUPLICATE 3                  /* duplicate key */
#define S_KEYSEQ    4                  /* field type used out of sequence
                                        * in d_keynext */
#define S_UNAVAIL   5                  /* database file currently
                                        * unavailable */
#define S_DELETED   6                  /* record/set deleted since last
                                        * accessed */
#define S_UPDATED   7                  /* record/set updated since last
                                        * accessed */
#define S_LOCKED    8                  /* current record's lock bit is set */
#define S_UNLOCKED  9                  /* current record's lock bit is
                                        * clear */
#define S_FUNCTION_LAST 9              /* last function status */

/* the following two definitions must be changed together */
typedef short DB_SHORT;

#define DB_SHORT_LEN 2

typedef unsigned short DB_UINT;        /* Some compilers won't handle
                                        * unsigned DB_SHORT */
typedef unsigned long DB_ULONG;
typedef int BOOLEAN;

#define MAXRECORDS 16777215L     /* This should match ADDRMASK in dbtype.h */
#define NULL_DBA 0L

/* db_VISTA database address */
#define DB_ADDR long

/* db_VISTA file number */
#define FILE_NO DB_SHORT

/* file address: page or record number */
#define F_ADDR long

/* record number indicator */
#define RECMARK 10000
/* field number indicator = rec * FLDMARK + fld_in_rec */
#define FLDMARK 1000L
/* set number indicator - must be greater than RECMARK */
#define SETMARK 20000

/* runtime option flags */
#define DCHAINUSE    0x00000001
#define TRLOGGING    0x00000002
#define ARCLOGGING   0x00000004
#define IGNORECASE   0x00000008
#define CLOSEFILES   0x00000020
#define GLOBALALLOCS 0x00000040
#define NOTRANS      0x00000400
#define READONLY     0x00000800
#define TXTEST       0x00001000
#define NORECOVER    0x00002000
#define LMC_OPT      0x80000000

/* LMC_OPT cannot be or'd together in one dt_on/off_opt() call with
   non LMC_OPTs
*/
#define LMC_OPT_TRUENAME  (LMC_OPT|0x00000001)
#define LMC_OPT_PORTABLE  (LMC_OPT|0x00000002)
#define LMC_OPT_DELNAME   (LMC_OPT|0x00000004)

/* runtime cache shareable options */
#ifdef MULTI_TASK
#define CA_SHAREABLE       1
#define CA_NONSHAREABLE    0
#endif




#ifndef MULTI_TASK
#define LIB_INIT
#endif

#ifndef VMS_SHARE                      /* ! VMS SHARABLE */

#ifndef MULTI_TASK
extern int db_status;
#endif

#else
noshare int db_status;

#endif

/* Use the 'const' keyword unless told otherwise */
#ifndef NO_CONST
#define CONST const
#else
#define CONST                          /**/
#endif

/* Use the 'void' keyword unless told otherwise */
#ifndef NO_VOID
#define DB_VOID void
#else
#define DB_VOID char
#endif

#ifdef ZOR
#define ANSI
#define LINT_ARGS
#endif

#ifdef TURBO
#define ANSI
#define LINT_ARGS
#endif

#ifdef MSC
#define ANSI
#define LINT_ARGS
#endif



/* Use PASCAL calling conventions unless told otherwise */
#ifndef NO_PASCAL
#ifndef PASCAL
#define PASCAL pascal
#endif
#ifndef CDECL
#define CDECL cdecl
#endif
#else
#ifndef PASCAL
#define PASCAL                         /**/
#endif
#ifndef CDECL
#define CDECL                          /**/
#endif
#endif

/* If mixed memory model use  far and near keywords */
#define DB_FAR                         /**/
#define DB_NEAR                        /**/
#define DB_FAR_PROC                    /**/

#ifdef __WATCOMC__
#define INTERNAL_FIXED                 /**/
#define EXTERNAL_FIXED  DB_FAR_PROC       /**/
#else
#ifdef MSC
#ifdef _MSC_VER
#define INTERNAL_FIXED  PASCAL
#define EXTERNAL_FIXED  DB_FAR_PROC PASCAL
#else
#define INTERNAL_FIXED  PASCAL
#define EXTERNAL_FIXED  DB_FAR_PROC PASCAL
#endif
#else
#define INTERNAL_FIXED  PASCAL
#define EXTERNAL_FIXED  DB_FAR_PROC PASCAL
#endif
#endif

#define EXTERNAL_DBN    EXTERNAL_FIXED



#define LOCK_DESC                      /**/
#define POINTER_INIT()          {  NULL  }
#define POINTER_ASSIGN(a)       {  a  }


typedef struct
{
   char DB_FAR *ptr;
   LOCK_DESC
} CHAR_P;
typedef struct
{
   DB_ADDR DB_FAR *ptr;
   LOCK_DESC
} DB_ADDR_P;

#ifdef MULTI_TASK
typedef struct
{
   struct TASK_S DB_FAR *ptr;
   LOCK_DESC
} TASK_P;
typedef struct
{
   DB_VOID DB_FAR *ptr;
   LOCK_DESC
} QTASK_P;


typedef struct DB_TASK_S
{
   TASK_P v;
   QTASK_P q;
}  DB_TASK;

#define DB_TASK_INIT() { POINTER_INIT(), POINTER_INIT() }

#else /* !MULTI_TASK */
/* need to allocate space for data structures used in non multi-tasking
   libraries
*/
typedef struct
{
   CHAR_P v;
   CHAR_P q;
}  DB_TASK;
#endif


/* Allow for function prototyping */
#ifdef LINT_ARGS                       /* LINT_ARGS */
#define P0      void                   /* parameterless function */
#define P1(t)   t                      /* first (or only) parameter in
                                        * function */
#define Pi(t)   ,t                     /* subsequent (2,3,...) parameter
                                        * in function */
#else                                  /* ! LINT_ARGS */
#define P0                             /**/
#define P1(t)                          /**/
#define Pi(t)                          /**/
#endif                                 /* */


#ifdef NO_DBN                          /* ! NO_DBN */
#define DBN_D1     P0
#define DBN_Dn                         /**/
#define DBN_FIX_D1 P0
#else                                  /* NO_DBN */
#define DBN_D1     P1(int)
#define DBN_Dn     Pi(int)
#define DBN_FIX_D1 P1(int)
#endif                                 /* */

#ifdef MULTI_TASK                      /* MULTI_TASK */
#define TASK_DBN_D1 P1(DB_TASK DB_FAR *) DBN_Dn
#ifndef NO_DBN                         /* MULTI_TASK & ! NO_DBN */
#define DBN_TASK_D1 DBN_D1 Pi(DB_TASK DB_FAR *)
#else                                  /* MULTI_TASK & NO_DBN */
#define DBN_TASK_D1 P1(DB_TASK DB_FAR *)
#endif                                 /* MULTI_TASK */
#define TASK_D1 P1(DB_TASK DB_FAR *)
#define TASK_Di Pi(DB_TASK DB_FAR *)
#define TASK_PTR_D1 P1(DB_TASK DB_FAR *)
#define TASK_PTR_Di Pi(DB_TASK DB_FAR *)
extern DB_TASK Currtask;

#define CURRTASK_ONLY &Currtask
#define ONLY_CURRTASK_DBN &Currtask DBN_PARM
#define CURRTASK_PARM , &Currtask
#else                                  /* ! MULTI_TASK */
#define TASK_DBN_D1 DBN_D1
#ifndef NO_DBN                         /* ! MULTI_TASK & ! NO_DBN */
#define DBN_TASK_D1 DBN_D1
#else                                  /* ! MULTI_TASK & NO_DBN */
#define DBN_TASK_D1                    /**/
#endif                                 /* ! MULTI_TASK */
#define TASK_D1 P0
#define TASK_Di                        /**/
#define TASK_PTR_D1 P0
#define TASK_PTR_Di                    /**/
#define CURRTASK_ONLY                  /**/
#define ONLY_CURRTASK_DBN DBN_ONLY
#define CURRTASK_PARM                  /**/
#endif                                 /* */

#define LOCK_D1                        /**/
#define LOCK_Di                        /**/


#ifdef MSC
#pragma pack(2)
#else
#ifdef TURBO
#pragma -a-
#else
#ifdef ZOR
#pragma ZTC align 2
#endif /* ZOR */
#endif /* TURBO */
#endif /* MSC */

typedef struct lock_request
{
   int item;                           /* record or set number */
   char type;                          /* lock type: 'r', 'w', 'x', 'k' */
}  LOCK_REQUEST;

#ifdef MSC
#pragma pack()
#else
#ifdef TURBO
#pragma -a.
#ifdef ZOR
#pragma ZTC align
#endif
#endif /* TURBO */
#endif /* MSC */


#define TRANS_ID_LEN 21

#ifndef NO_DBN                         /* ! ONE_DB & ! NO_DBN */
#define  CURR_DB                 -1
#define  ALL_DBS                 -2
#define  VOID_DB                 -3
#define  CURR_DB_PARM            , curr_db
#define  CURR_DB_ONLY            curr_db
#define  DBN_PARM                , dbn
#ifdef ZOR
#define  ADBN_PARM               , int dbn
#endif                                 /* ! ONE_DB */
#define  DBN_ONLY                dbn
#define  DBN_DECL                int dbn;
#else                                  /* ! ONE_DB & NO_DBN */
#define  CURR_DB                       /**/
#define  CURR_DB_PARM                  /**/
#define  CURR_DB_ONLY                  /**/
#define  DBN_PARM                      /**/
#ifdef ZOR
#define  ADBN_PARM                     /**/
#endif                                 /* ! ONE_DB */
#define  DBN_ONLY                      /**/
#define  DBN_DECL                      /**/
#endif                                 /* ! ONE_DB */

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (EXTERNAL_FIXED * ERRORPROC) (P1(int) Pi(char DB_FAR *));

#include "dproto.h"
#include "lockcomm.h"

#ifdef MULTI_TASK
#include "task.h"
#endif    /* MULTI_TASK */

#ifdef __cplusplus
}
#endif

#endif /* VISTA_H */
/* vpp @dos.vpp VISTA.h (Fri Jun 26 15:32:20 1992)
 *
 *     -dMSDOS -dDB_SHARE -nUNIX -nAIX -nRISC -nSUN -nSCO -nWIN3 -nWINDOWS
 *     -nVMS -nOS2 -nOS2_MT -nREOPEN_FILES -nFAR_ALLOC -nARCHIVING -nDB_DLL
 *     -nBSD -nMEMLOCK -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP
 *     -nSINGLE_USER -nNO_TRANS -nVer330 -n_WINDOWS -nQNX -nQNX2 -nQNX4
 *     -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY -nTRUENAME -nLAT
 */

