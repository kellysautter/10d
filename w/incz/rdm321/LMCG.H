
/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  440 05-Dec-88 RSC Moved defn of MESSAGE to dbtype.h
*/

#ifndef LMCG_H
#define LMCG_H

#include "lmci.h"

#define DBL_TIMEOUT 20

typedef struct
{
   G_GLOBAL_P G_ptr;
   char Dblname[FILENMLEN];
   char Lokfile[FILENMLEN];
   long Lastaccess;
   int  Dbl;
   int  Lok;
   int  lockedByMe;
   int  Last_free_count;
   int  Dbl_timeout;
   int  Timer;
} LMC_GENERAL_DATA;

#define dblname    ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Dblname
#define dbl        ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Dbl
#define dbl_timeout ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Dbl_timeout
#define timer      ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Timer
#define lokfile    ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Lokfile
#define lok        ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Lok
#define lastaccess ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Lastaccess
#define LockedByMe ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->lockedByMe
#define last_free_count ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->Last_free_count

#define Gptr       ((LMC_GENERAL_DATA DB_FAR *)db_global.Lmc_data.ptr)->G_ptr


/* General error codes */
enum {
   G_OKAY = 0,
   G_CREATE_DBL,
   G_OPEN_DBL,
   G_UNLOCK_DBL,
   G_CREATE_GUARD,
   G_UNLINK_GUARD,
   G_SYNC_GUARD,
   G_NOLOCKMGR,
   G_DUPUSERID,
   G_LMBUSY,
   G_TIMEOUT,
   G_NOTOPEN,
   G_NETSYNC,
   G_RDMERROR,
   G_UNKNOWN
};

static char DB_FAR * g_errstr [] =
{
   "",
   "unable to create DBL file",
   "unable to open DBL file",
   "unable to unlock DBL file",
   "unable to create GUARD file",
   "unable to unlink GUARD file",
   "GUARD file is out of sync",
   "no lock manager found",
   "duplicate userid",
   "lock manager busy",
   "time out",
   "attempt to use lmc_trans befor call to lmc_open",
   "lock manager synchronization error",
   "error is handled by RDM",
   "unknown error, possible memory corruption"
};


int EXTERNAL_FIXED lmcg_avail   (P0);
int EXTERNAL_FIXED lmcg_checkid (P1(char DB_FAR *));
int EXTERNAL_FIXED lmcg_open    (P0);
int EXTERNAL_FIXED lmcg_close   (P0);
int EXTERNAL_FIXED lmcg_trans   (P1(MTYPE) Pi(char DB_FAR *) Pi(int) Pi(char DB_FAR *) Pi(int));
int EXTERNAL_FIXED lmcg_errstr  (P1(char DB_FAR *) Pi(int));
int EXTERNAL_FIXED lmcg_fileid  (P1(void DB_FAR *) Pi(char DB_FAR *) Pi(int));
void DB_FAR * EXTERNAL_FIXED lmcg_alloc (P1(unsigned int));
void  EXTERNAL_FIXED lmcg_free  (P1(void DB_FAR *));
int EXTERNAL_FIXED lmcg_locking (P1(int) Pi(int) Pi(long));

int EXTERNAL_FIXED lmcg_taf_open(P0);
int EXTERNAL_FIXED lmcg_taf_close(P0);
int EXTERNAL_FIXED lmcg_taf_access(P1(int));
int EXTERNAL_FIXED lmcg_taf_release(P0);
int EXTERNAL_FIXED lmcg_taf_add(P1(CONST char DB_FAR *));
int EXTERNAL_FIXED lmcg_taf_del(P1(CONST char DB_FAR *));

DB_STATIC int INTERNAL_FIXED lmcg_send(P1(HM_MESSAGE DB_FAR *) Pi(int));
DB_STATIC int INTERNAL_FIXED lmcg_rcvmsg(P1(HR_MESSAGE DB_FAR *) Pi(int));
DB_STATIC int INTERNAL_FIXED convert_lmcg_errno (P0);

int  INTERNAL_FIXED dbl_open(P0);
int  INTERNAL_FIXED dbl_access(P1(int) Pi(int) Pi(int));
int  INTERNAL_FIXED dbl_commit(P0);
void INTERNAL_FIXED dbl_close(P0);
int  INTERNAL_FIXED dbl_lock(P0);
void INTERNAL_FIXED dbl_unlock(P0);
void INTERNAL_FIXED dbl_name(P0);

#endif

/* vpp @win.vpp LMCG.H (Thu May 21 10:47:20 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

