/*----------------------------------------------------------------------------
   dbxtrn.h: db_VISTA external data header file

   (This file is included by dbtype.h)

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef DBXTRN_H
#define DBXTRN_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  423 22-Sep-88 RSC Cleanup file to compile under Lattice
  368 23-Sep-88 RSC Changed defn of Last_fcn to match dbtype.h
  439 17-Nov-88 RSC Integrate 3.10 branch (w/o windows) into main
  440 13-Dec-88 RSC LR_LOCK was removed from db_global for general LM
  532 06-Jan-89 RSC Fixed defns of rn_dba/type,db_name for ONE_DB
      23-Aug-91 TWP Added lock manager support fields
 1430 30-Oct-91 DMB Full support of context switching using multiple caches
 1328 03-Nov-91 AOY Moved dbtaf into the task structure
      11-Nov-91 BJB Added extern for def_ db and ix _pgtab_sz
      12-Nov-91 BJB Moved the dblfg file name to TASK Structure (PORTABLE)
*/

#include "dblock.h"

/* Define Cache table */
typedef struct CACHE_S {
   unsigned int Shareable;          /* TRUE or FALSE */
   DB_SHORT Task_count;             /* number of tasks owning this cache */
   int Ix_pgtab_sz;
   LOOKUP_ENTRY_P Ix_lookup_p;      /* index page lookup table */
   PAGE_ENTRY_P Ixpg_table_p;       /* index page table */
   int Ixpg_lru_slot;               /* least recently accessed ix page */
   CHAR_P Dbpgbuff_p;               /* allocated by dio_init used by o_update */
   BOOLEAN Trcommit;
   DB_SHORT Largest_page;
   int Db_pgtab_sz;
   LOOKUP_ENTRY_P Db_lookup_p;      /* database page lookup */
   PAGE_ENTRY_P Dbpg_table_p;       /* database page table */
   struct {
      DB_TASK task;
      FILE_NO file;
      F_ADDR pageno;
      int slot;
   } Last_dblu;                     /* last found lookup entry in cache */
   int Dbpg_lru_slot;               /* least recently accessed db page */
   int No_modheld;                  /* number of modified or held db pages */
} CACHE;

/* Dynamically allocated global variables */
#define ix_lookup db_cache.Ix_lookup_p.ptr
#define ixpg_table db_cache.Ixpg_table_p.ptr
#define dbpgbuff db_cache.Dbpgbuff_p.ptr
#define shareable db_cache.Shareable
#define task_count db_cache.Task_count

#define db_lookup db_cache.Db_lookup_p.ptr
#define dbpg_table db_cache.Dbpg_table_p.ptr

/* Non-dynamically allocated global variables */
#define ix_pgtab_sz db_cache.Ix_pgtab_sz
#define Ix_lookup db_cache.Ix_lookup_p
#define Ixpg_table db_cache.Ixpg_table_p
#define ixpg_lru_slot db_cache.Ixpg_lru_slot
#define trcommit db_cache.Trcommit
#define largest_page db_cache.Largest_page
#define db_pgtab_sz db_cache.Db_pgtab_sz
#define Dbpgbuff db_cache.Dbpgbuff_p
#define Db_lookup db_cache.Db_lookup_p
#define Dbpg_table db_cache.Dbpg_table_p
#define last_dblu db_cache.Last_dblu
#define dbpg_lru_slot db_cache.Dbpg_lru_slot
#define no_modheld db_cache.No_modheld

/* Database Dictionary Tables */


typedef int   (EXTERNAL_FIXED * LMC_AVAIL_PROC)   (P0);
typedef int   (EXTERNAL_FIXED * LMC_OPEN_PROC)    (P0);
typedef int   (EXTERNAL_FIXED * LMC_CLOSE_PROC)   (P0);
typedef int   (EXTERNAL_FIXED * LMC_CHECKID_PROC) (P1(char DB_FAR *));
typedef int   (EXTERNAL_FIXED * LMC_TRANS_PROC)   (P1(MTYPE) Pi(void DB_FAR *) Pi(int) Pi(void DB_FAR *) Pi(int));
typedef int   (EXTERNAL_FIXED * LMC_LOCKING_PROC) (P1(int) Pi(int) Pi(long));
typedef int   (EXTERNAL_FIXED * LMC_ERRSTR_PROC)  (P1(char DB_FAR *) Pi(int));
typedef int   (EXTERNAL_FIXED * LMC_FILEID_PROC)  (P1(void DB_FAR *) Pi(char DB_FAR *) Pi(int));
typedef void DB_FAR * (EXTERNAL_FIXED * LMC_ALLOC_PROC) (P1(unsigned int));
typedef void  (EXTERNAL_FIXED * LMC_FREE_PROC)    (P1(void DB_FAR *));

typedef int   (EXTERNAL_FIXED * TAF_LOGIN_PROC)   (P0);
typedef int   (EXTERNAL_FIXED * TAF_LOGOUT_PROC)  (P0);
typedef int   (EXTERNAL_FIXED * TAF_OPEN_PROC)    (P0);
typedef int   (EXTERNAL_FIXED * TAF_CLOSE_PROC)   (P0);
typedef int   (EXTERNAL_FIXED * TAF_ACCESS_PROC)  (P1(int));
typedef int   (EXTERNAL_FIXED * TAF_RELEASE_PROC) (P0);
typedef int   (EXTERNAL_FIXED * TAF_ADD_PROC)     (P1(CONST char DB_FAR *));
typedef int   (EXTERNAL_FIXED * TAF_DEL_PROC)     (P1(CONST char DB_FAR *));

typedef struct
{
   void DB_FAR *ptr;
      LOCK_DESC
}  LMC_DATA_P;


typedef struct TASK_S
{
   char Dbuserid[FILENMLEN];
   char Dbdpath[FILENMLEN * 2];
   char Dbfpath[FILENMLEN * 2];
   char Lockmgrn[FILENMLEN];
   char Dbtaf[FILENMLEN];
   char ctbpath[FILENMLEN];
   char Trans_id[TRANS_ID_LEN];
   char Dblog[FILENMLEN];
   CNTRY_TBL_P country_tbl;
   int ctbl_activ;
   int Db_status;
   int Dbopen;
   int Dbshflag;
   int Lfn;
   int No_of_keys;
   unsigned long Dboptions;
   int Ov_header_written;
   int Ov_setup_done;
   int Lock_lvl;
   char DB_FAR *Crloc;                 /* location in page buffer of
                                        * current record */
#define LOCK_STACK_SIZE         10
   int Lock_stack[LOCK_STACK_SIZE];
   BOOLEAN Cache_ovfl;
   PGZERO_P Pgzero;
   KEY_INFO_P Key_info;
   KEY_TYPE Key_type;
   long Ov_initaddr;
   long Ov_rootaddr;
   long Ov_nextaddr;
   RI_ENTRY_P Root_ix;
   llist Ren_list;
   DB_SHORT Page_size;
   DB_ADDR Curr_rec;
   DB_ADDR_P Curr_own;
   DB_ADDR_P Curr_mem;
   FILE_NO Ov_file;
   FILE_ENTRY_P File_table;
   DB_SHORT Size_ft;
   RECORD_ENTRY_P Record_table;
   DB_SHORT Size_rt;
   SET_ENTRY_P Set_table;
   DB_SHORT Size_st;
   MEMBER_ENTRY_P Member_table;
   DB_SHORT Size_mt;
   SORT_ENTRY_P Sort_table;
   DB_SHORT Size_srt;
   FIELD_ENTRY_P Field_table;
   DB_SHORT Size_fd;
   KEY_ENTRY_P Key_table;
   DB_SHORT Size_kt;
   llist Sk_list;
   ERRORPROC error_func;
   DB_SHORT Old_size_ft;
   DB_SHORT Old_size_fd;
   DB_SHORT Old_size_st;
   DB_SHORT Old_size_mt;
   DB_SHORT Old_size_srt;
   DB_SHORT Old_size_kt;
   DB_SHORT Old_size_rt;
   DB_SHORT Old_no_of_dbs;
   int Curr_db;
   int Set_db;
   DB_SHORT No_of_dbs;
   DB_ENTRY DB_FAR *Curr_db_table;
   RN_ENTRY_P Rn_table;
   RN_ENTRY DB_FAR *Curr_rn_table;
   DB_ENTRY_P Db_table;
   MTYPE Last_mtype;
   REOPEN_ENTRY_P Reopen_table;
   DB_ULONG Cr_time;
   ULONG_P Co_time;
   ULONG_P Cm_time;
   ULONG_P Cs_time;
   BOOLEAN Db_tsrecs;
   BOOLEAN Db_tssets;
   int Dbwait_time;
   int Db_timeout;
   int Db_lockmgr;
   int Keyl_cnt;
   int Lp_size;
   int Fp_size;
   INT_P App_locks;
   INT_P Excl_locks;
   INT_P Kept_locks;
   LOCK_DESCR_P Rec_locks;
   LOCK_DESCR_P Set_locks;
   LOCK_DESCR_P Key_locks;
   LM_LOCK DB_FAR *Lock_pkt;
   LM_FREE DB_FAR *Free_pkt;
   FILE_NO_P File_refs;
   BOOLEAN Session_active;
   USED_FILE_ENTRY_P Used_files;       /* LRU file table */
   int Max_open_files;                 /* maximum number of open files
                                        * allowed */
   int Cnt_open_files;                 /* count of currently open files */
   int Last_file;                      /* least recently used file */
   int Max_handle;                     /* largest file handle */
   int Rlb_status;
   char Type[2];                 /* open type (s or x) - used by dopen() */
   FILE_NO Working_file;         /* current key file being processed */
   unsigned int Process_id;      /* process id */

   WORD Gmem_options;


   double Nap_factor;               /* lmcg sleep time */

   int lockMgrComm;
   unsigned long Lmc_options;    /* lmc specific options defined in vista.h */
   int Lmc_errno;                /* lmc error code, msg thru lmc_errstr  */
   LMC_DATA_P Lmc_data;          /* Generic pointer for lmc internal use */

   LMC_AVAIL_PROC    Lmc_avail;
   LMC_CHECKID_PROC  Lmc_checkid;
   LMC_OPEN_PROC     Lmc_open;
   LMC_CLOSE_PROC    Lmc_close;
   LMC_TRANS_PROC    Lmc_trans;
   LMC_LOCKING_PROC  Lmc_locking;
   LMC_ERRSTR_PROC   Lmc_errstr;
   LMC_FILEID_PROC   Lmc_fileid;
   LMC_ALLOC_PROC    Lmc_alloc;
   LMC_FREE_PROC     Lmc_free;

   TAF_LOGIN_PROC    Taf_login;
   TAF_LOGOUT_PROC   Taf_logout;
   TAF_OPEN_PROC     Taf_open;
   TAF_CLOSE_PROC    Taf_close;
   TAF_ACCESS_PROC   Taf_access;
   TAF_RELEASE_PROC  Taf_release;
   TAF_ADD_PROC      Taf_add;
   TAF_DEL_PROC      Taf_del;

   CACHE_P Cache;

} TASK;

/* Dynamically allocated global variables */

#define gmem_options db_global.Gmem_options

#define LockMgrComm   db_global.lockMgrComm
#define lmc_options   db_global.Lmc_options
#define lmc_errno     db_global.Lmc_errno
#define lmc_data      db_global.Lmc_data.ptr
#define nap_factor    db_global.Nap_factor

#define lmc_avail     (*db_global.Lmc_avail)
#define lmc_checkid   (*db_global.Lmc_checkid)
#define lmc_open      (*db_global.Lmc_open)
#define lmc_close     (*db_global.Lmc_close)
#define lmc_trans     (*db_global.Lmc_trans)
#define lmc_locking   (*db_global.Lmc_locking)
#define lmc_errstr    (*db_global.Lmc_errstr)
#define lmc_fileid    (*db_global.Lmc_fileid)
#define lmc_alloc     (*db_global.Lmc_alloc)
#define lmc_free      (*db_global.Lmc_free)

#define taf_login     (*db_global.Taf_login)
#define taf_logout    (*db_global.Taf_logout)
#define taf_open      (*db_global.Taf_open)
#define taf_close     (*db_global.Taf_close)
#define taf_access    (*db_global.Taf_access)
#define taf_release   (*db_global.Taf_release)
#define taf_add       (*db_global.Taf_add)
#define taf_del       (*db_global.Taf_del)

#define app_locks db_global.App_locks.ptr
#define excl_locks db_global.Excl_locks.ptr
#define kept_locks db_global.Kept_locks.ptr
#define rec_locks db_global.Rec_locks.ptr
#define set_locks db_global.Set_locks.ptr
#define key_locks db_global.Key_locks.ptr
#define lock_pkt db_global.Lock_pkt
#define free_pkt db_global.Free_pkt
#define file_refs db_global.File_refs.ptr
#define key_info db_global.Key_info.ptr
#define curr_own db_global.Curr_own.ptr
#define curr_mem db_global.Curr_mem.ptr
#define rn_table db_global.Rn_table.ptr
#define db_table db_global.Db_table.ptr
#define file_table db_global.File_table.ptr
#define record_table db_global.Record_table.ptr
#define set_table db_global.Set_table.ptr
#define member_table db_global.Member_table.ptr
#define sort_table db_global.Sort_table.ptr
#define field_table db_global.Field_table.ptr
#define key_table db_global.Key_table.ptr
#define pgzero db_global.Pgzero.ptr
#define root_ix db_global.Root_ix.ptr
#define co_time db_global.Co_time.ptr
#define cm_time db_global.Cm_time.ptr
#define cs_time db_global.Cs_time.ptr
#define reopen_table db_global.Reopen_table.ptr
#define used_files db_global.Used_files.ptr

/* Used in DIO.C and OVFCNS.C */

/* Non-dynamically allocated global variables */
#define dbwait_time db_global.Dbwait_time
#define db_timeout db_global.Db_timeout
#define dbopen db_global.Dbopen
#define dbshflag db_global.Dbshflag
#define db_lockmgr db_global.Db_lockmgr
#define dbuserid db_global.Dbuserid
#define lockmgrn db_global.Lockmgrn
#define dbdpath db_global.Dbdpath
#define dbfpath db_global.Dbfpath
#define trans_id db_global.Trans_id
#define dblog db_global.Dblog
#define keyl_cnt db_global.Keyl_cnt
#define lp_size db_global.Lp_size
#define fp_size db_global.Fp_size
#define session_active db_global.Session_active
#define cache_ovfl db_global.Cache_ovfl
#define no_of_keys db_global.No_of_keys
#define key_type db_global.Key_type
#define net_status db_global.Net_status
#define last_mtype db_global.Last_mtype
#define dboptions db_global.Dboptions
#define ov_initaddr db_global.Ov_initaddr
#define ov_rootaddr db_global.Ov_rootaddr
#define ov_nextaddr db_global.Ov_nextaddr
#define page_size db_global.Page_size
#define curr_db db_global.Curr_db
#define curr_db_table db_global.Curr_db_table
#define curr_rec db_global.Curr_rec
#define cr_time db_global.Cr_time
#define db_tsrecs db_global.Db_tsrecs
#define db_tssets db_global.Db_tssets
#define ov_file db_global.Ov_file
#define ov_header_written db_global.Ov_header_written
#define ov_setup_done db_global.Ov_setup_done
#define set_db db_global.Set_db
#define no_of_dbs db_global.No_of_dbs
#define curr_rn_table db_global.Curr_rn_table
#define size_ft db_global.Size_ft
#define size_rt db_global.Size_rt
#define size_st db_global.Size_st
#define size_mt db_global.Size_mt
#define size_srt db_global.Size_srt
#define size_fd db_global.Size_fd
#define size_kt db_global.Size_kt
#define lock_lvl db_global.Lock_lvl
#define crloc db_global.Crloc
#define lock_stack db_global.Lock_stack
#define ren_list db_global.Ren_list
#define sk_list db_global.Sk_list
#define max_open_files db_global.Max_open_files
#define cnt_open_files db_global.Cnt_open_files
#define max_handle db_global.Max_handle
#define last_file db_global.Last_file
#define hUserLockMan   db_global.HUserLockMan
#define idLastMessage  db_global.IdLastMessage

#define old_size_ft db_global.Old_size_ft
#define old_size_fd db_global.Old_size_fd
#define old_size_st db_global.Old_size_st
#define old_size_mt db_global.Old_size_mt
#define old_size_srt db_global.Old_size_srt
#define old_size_kt db_global.Old_size_kt
#define old_size_rt db_global.Old_size_rt
#define old_no_of_dbs db_global.Old_no_of_dbs

#define lfn db_global.Lfn
#define dbtaf db_global.Dbtaf


#define rlb_status db_global.Rlb_status
#define working_file db_global.Working_file
#define process_id db_global.Process_id

extern TASK db_global;
extern CACHE db_cache;


#define db_status  db_global.Db_status

extern DB_TASK Orgtask;
extern DB_TASK Vistatask;


extern lock_group lock_set[];
extern int no_discard;


extern int ProtectedMode;


#endif      /* DBXTRN_H */
/* vpp @win.vpp DBXTRN.h (Fri Jun 05 17:12:24 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

