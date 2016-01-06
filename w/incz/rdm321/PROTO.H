/*----------------------------------------------------------------------------
   proto.h: db_VISTA function declaration header file

   (This file must be included after dbtype.h)

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef PROTO_H
#define PROTO_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
      03-Aug-88 RTK Added internal MULTI_TASKing function prototypes
  310 10-Aug-88 RSC Cleanup of function prototypes
      01-Sep-88 RTK task_switch changed from void to int
      01-Sep-88 RTK eliminate dio_discard
      17-Dec-90 DPG Added db_exit() for OS2_MT
      12-Sep-91 DJS Wrapped standard includes
      03-Oct-91 BJB Added DB_DEBUG support to dberr
 1430 30-Oct-91 DMB Full support of context switching using multiple caches
*/

 /* From dberr.c */
int EXTERNAL_FIXED dberr(P1(int));
int EXTERNAL_FIXED db_dberr(P1(int) Pi(char DB_FAR *) Pi(int));
#ifdef DB_DEBUG
#define dberr(n)  db_dberr(n, __FILE__, __LINE__)
#endif

/* From alloc.c: */
int INTERNAL_FIXED d_memlock(P1(CHAR_P DB_FAR *));      /* alloc.c */
int INTERNAL_FIXED d_memlock_group(P1(lock_group *));   /* alloc.c */
int INTERNAL_FIXED d_memunlock(P1(CHAR_P DB_FAR *));    /* alloc.c */
void INTERNAL_FIXED d_memunlock_group(P1(lock_group *));        /* alloc.c */


DB_VOID DB_FAR *INTERNAL_FIXED d_alloc(P1(CHAR_P DB_FAR *) Pi(unsigned));

 /* alloc.c */

void INTERNAL_FIXED d_free(P1(CHAR_P DB_FAR *));        /* alloc.c */

int INTERNAL_FIXED d_lockpush(P1(int));/* alloc.c */
int INTERNAL_FIXED d_lockpop(P1(int));       /* alloc.c */


/* From dberr.c: */
void INTERNAL_FIXED dbautorec(P0);

/* From dblfcns.c: */
int INTERNAL_FIXED taskinit(P1(TASK DB_FAR *));
int INTERNAL_FIXED cacheinit(P1(CACHE DB_FAR *));
unsigned int INTERNAL_FIXED get_process_id(P0);
int INTERNAL_FIXED initdbt(P1(CONST char DB_FAR *));
int INTERNAL_FIXED alloc_table(P1(CHAR_P DB_FAR *) Pi(unsigned) Pi(unsigned));
int INTERNAL_FIXED calloc_table(P1(CHAR_P DB_FAR *) Pi(unsigned) Pi(unsigned) Pi(unsigned));

#define FREE_TABLE(a, b, c, d, e) free_table((CHAR_P DB_FAR *)a, b, c, d, e)
int INTERNAL_FIXED free_table(P1(CHAR_P DB_FAR *) Pi(int) Pi(int) Pi(int)
                                 Pi(unsigned));


void INTERNAL_FIXED termfree(DBN_D1);

int INTERNAL_FIXED neterr(P0);


int INTERNAL_FIXED dupid_check(P0);


/* From dio.c: */
int INTERNAL_FIXED dio_open(P1(FILE_NO));
int INTERNAL_FIXED dio_closelru(P0);
int INTERNAL_FIXED dio_close(P1(FILE_NO));
int INTERNAL_FIXED dio_init(P0);
void INTERNAL_FIXED dio_free(P0);
int INTERNAL_FIXED dio_clrfile(P1(FILE_NO) Pi(BOOLEAN));
int INTERNAL_FIXED dio_clear(P1(BOOLEAN));

void INTERNAL_FIXED dio_ixclear(P0);

int INTERNAL_FIXED dio_flush(P0);
void INTERNAL_FIXED dio_setdef(P1(FILE_NO));
int INTERNAL_FIXED dio_get(P1(F_ADDR) Pi(char DB_FAR *DB_FAR *) Pi(int));
int INTERNAL_FIXED dio_touch(P1(F_ADDR));
int INTERNAL_FIXED dio_unget(P1(F_ADDR));
int INTERNAL_FIXED dio_read(P1(DB_ADDR) Pi(char DB_FAR *DB_FAR *) Pi(int));
int INTERNAL_FIXED dio_write(P1(DB_ADDR) Pi(CONST char DB_FAR *) Pi(int));
int INTERNAL_FIXED dio_release(P1(DB_ADDR) Pi(int));
int INTERNAL_FIXED dio_unlock(P0);
int INTERNAL_FIXED dio_lock(P1(LOOKUP_ENTRY DB_FAR *));
int INTERNAL_FIXED dio_rrlb(P1(DB_ADDR) Pi(DB_SHORT DB_FAR *));
int INTERNAL_FIXED dio_wrlb(P1(DB_ADDR) Pi(DB_SHORT));
int INTERNAL_FIXED dio_findpg(P1(FILE_NO) Pi(F_ADDR) Pi(PAGE_ENTRY DB_FAR *)
                                 Pi(PAGE_ENTRY DB_FAR *DB_FAR *)
                                 Pi(LOOKUP_ENTRY DB_FAR *DB_FAR *));

int INTERNAL_FIXED dio_out(P1(PAGE_ENTRY DB_FAR *) Pi(BOOLEAN));

DB_ULONG INTERNAL_FIXED dio_pzsetts(P1(FILE_NO));
DB_ULONG INTERNAL_FIXED dio_pzgetts(P1(FILE_NO));

int INTERNAL_FIXED dio_pzread(P1(FILE_NO));
int INTERNAL_FIXED dio_pzalloc(P1(FILE_NO) Pi(F_ADDR DB_FAR *));
int INTERNAL_FIXED dio_pzdel(P1(FILE_NO) Pi(F_ADDR));
F_ADDR INTERNAL_FIXED dio_pznext(P1(FILE_NO));
void INTERNAL_FIXED dio_pzclr(P0);

/* From initenv.c: */
int INTERNAL_FIXED initenv(P0);

/* From inittab.c: */
int INTERNAL_FIXED inittab(P0);

/* From keyfcns.c: */
int INTERNAL_FIXED key_open(P0);
void INTERNAL_FIXED key_close(DBN_D1);
int INTERNAL_FIXED key_init(P1(int));
int INTERNAL_FIXED key_reset(P1(FILE_NO));
int INTERNAL_FIXED key_locpos(P1(CONST char DB_FAR *) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED key_scan(P1(int) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED key_boundary(P1(int) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED key_insert(P1(int) Pi(CONST char DB_FAR *) Pi(DB_ADDR));
int INTERNAL_FIXED key_delete(P1(int) Pi(CONST char DB_FAR *) Pi(DB_ADDR));

/* Due to a problem with MSC 5.1, we couldn't declare the 2nd parm
   of key_bldcom as const (see keyfcns.c) */
int INTERNAL_FIXED key_bldcom(P1(int) Pi(char DB_FAR *) Pi(char DB_FAR *) Pi(int));

/* Because of a "problem" with the MSC 5.1 compiler, we couldn't declare
   the second argument to key_cmpcpy with a const attribute.  Hence we
   were not able to define the 2nd argument of key_bldcom as const.  */
void INTERNAL_FIXED key_cmpcpy(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(DB_SHORT));
void INTERNAL_FIXED key_acpy(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(DB_SHORT));

/* From libfcns.c: */
int INTERNAL_FIXED nset_check(P1(int) Pi(int DB_FAR *) Pi(SET_ENTRY DB_FAR *DB_FAR *));
int INTERNAL_FIXED nfld_check(P1(long) Pi(int DB_FAR *) Pi(int DB_FAR *)
                                 Pi(RECORD_ENTRY DB_FAR *DB_FAR *)
                                 Pi(FIELD_ENTRY DB_FAR *DB_FAR *));
int INTERNAL_FIXED nrec_check(P1(int) Pi(int DB_FAR *)
                                 Pi(RECORD_ENTRY DB_FAR *DB_FAR *));
int INTERNAL_FIXED fldcmp(P1(FIELD_ENTRY DB_FAR *) Pi(CONST char DB_FAR *)
                             Pi(CONST char DB_FAR *));
int INTERNAL_FIXED SHORTcmp(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *));
int INTERNAL_FIXED ADDRcmp(P1(CONST DB_ADDR DB_FAR *) Pi(CONST DB_ADDR DB_FAR *));
int INTERNAL_FIXED null_dba(P1(CONST char DB_FAR *));
int INTERNAL_FIXED check_dba(P1(DB_ADDR));

/* From makenew.c: */
int INTERNAL_FIXED sk_free(P0);


/* From opens.c: */
int INTERNAL_FIXED open_b(P1(CONST char DB_FAR *) Pi(unsigned int));
int INTERNAL_FIXED commit_file(P1(int) Pi(DB_SHORT));

#define BY_NAP_SUCCESS 1
#define BY_NAP_FAILURE 0

/* apply some randomness */
#define SUCCESS_FACTOR  0.100
#define FAILURE_FACTOR  1.300

#define MAX_FACTOR      2.000
#define DEF_FACTOR      0.010
#define MIN_FACTOR      0.005
#define ONE_FACTOR      0.001
/* if (2 * ONE_USER_NAP) >= MIN_FACTOR then change naptime() */

void INTERNAL_FIXED adjust_naptime (P1(int));
void INTERNAL_FIXED naptime (P0);


/* From ovfcns.c: */
int INTERNAL_FIXED o_setup(P0);
int INTERNAL_FIXED o_init(P0);
int INTERNAL_FIXED o_fileinit(P1(FILE_NO));
int INTERNAL_FIXED o_search(P1(FILE_NO) Pi(F_ADDR) Pi(F_ADDR DB_FAR *));
int INTERNAL_FIXED o_write(P1(PAGE_ENTRY DB_FAR *) Pi(LOOKUP_ENTRY DB_FAR *));
int INTERNAL_FIXED o_pzwrite(P1(FILE_NO));
int INTERNAL_FIXED o_flush(P0);
int INTERNAL_FIXED o_update(P0);
long INTERNAL_FIXED o_pages(P1(FILE_NO));
void INTERNAL_FIXED o_free(DBN_D1);
void INTERNAL_FIXED o_close(P0);

/* From recfcns.c: */
int INTERNAL_FIXED r_chkfld(P1(DB_SHORT) Pi(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *)
                               Pi(CONST char DB_FAR *));
int INTERNAL_FIXED r_delrec(P1(DB_SHORT) Pi(DB_ADDR));
int INTERNAL_FIXED r_gfld(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_gmem(P1(int) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_gset(P1(int) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_pfld(P1(DB_SHORT) Pi(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *)
                           Pi(CONST char DB_FAR *) Pi(DB_ADDR DB_FAR *));
int INTERNAL_FIXED r_pmem(P1(int) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_pset(P1(int) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_smem(P1(DB_ADDR DB_FAR *) Pi(int));
int INTERNAL_FIXED r_setopt(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_clropt(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED r_tstopt(P1(FIELD_ENTRY DB_FAR *) Pi(char DB_FAR *));

/* From pathfcns.c */
int INTERNAL_FIXED con_dbd(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(char DB_FAR *));
int INTERNAL_FIXED con_dbf(P1(char DB_FAR *) Pi(char DB_FAR *) Pi(char DB_FAR *)
                              Pi(char DB_FAR *));
char DB_FAR *INTERNAL_FIXED get_element(P1(char DB_FAR *) Pi(int));

/* From renfile.c: */
int INTERNAL_FIXED renfiles(P0);

/* From enter.c: */
int INTERNAL_FIXED db_enter(DBN_FIX_D1 TASK_PTR_Di LOCK_Di);



/* From taffcns.c: */
int INTERNAL_FIXED rdm_taf_open(P0);
int INTERNAL_FIXED rdm_taf_close(P0);
int INTERNAL_FIXED rdm_taf_access(P1(int));
int INTERNAL_FIXED rdm_taf_release(P0);
int INTERNAL_FIXED rdm_taf_add(P1(CONST char DB_FAR *));
int INTERNAL_FIXED rdm_taf_del(P1(CONST char DB_FAR *));
int INTERNAL_FIXED rdm_taf_login(P0);
int INTERNAL_FIXED rdm_taf_logout(P0);
int INTERNAL_FIXED rdm_taf_locking(P1(int) Pi(int) Pi(long) );

/* From task.c */
int INTERNAL_FIXED task_switch(P1(DB_TASK DB_FAR *));
int INTERNAL_FIXED remove_cache(P1(DB_TASK DB_FAR *));
int INTERNAL_FIXED ntask_check(P1(DB_TASK DB_FAR *));


/* From mapchar.c */
int INTERNAL_FIXED ctb_init(P0);
int INTERNAL_FIXED ctbl_alloc(P0);
void INTERNAL_FIXED ctbl_free(P0);

/* System functions */

#ifndef NO_STD_H
#include <stdlib.h>

#include <io.h>

#ifdef TURBO
#include <alloc.h>
#else
#ifndef ZOR
#include <malloc.h>
#endif
#endif /* TURBO */

#include <string.h>
#include <time.h>

#ifndef __WATCOMC__
#ifndef ZOR

#ifdef TURBO
#include <mem.h>
#else
#include <memory.h>
#endif

#endif
#endif /* __WATCOMC__ */

#ifndef ZOR
void sleep(P1(unsigned));
#endif

#endif  /* NO_STD_H */

#endif  /* PROTO_H */
/* vpp @win.vpp PROTO.h (Mon Jun 08 09:59:04 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

