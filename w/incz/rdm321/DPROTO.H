/*----------------------------------------------------------------------------
   dproto.h: db_VISTA (user) function declaration header file

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef DPROTO_H
#define DPROTO_H

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  571 31-Jan-89 RSC Placed ctbpath inside NO_COUNTRY, not NO_TIMESTAMP
 1430 30-Oct-91 DMB Full support of context switching using multiple caches
*/


int EXTERNAL_FIXED d_trbound(P0);       /* trlog.c */
int EXTERNAL_FIXED d_trlog(P1(int) Pi(int) Pi(CONST char DB_FAR *) Pi(int));

 /* trlog.c */
int EXTERNAL_FIXED d_trmark(P0);       /* trlog.c */
int EXTERNAL_FIXED d_tron(P0);         /* trlog.c */
int EXTERNAL_FIXED d_troff(P0);        /* trlog.c */
int EXTERNAL_FIXED d_decode_dba(P1(DB_ADDR) Pi(int DB_FAR *) Pi(long DB_FAR *));

 /* dbacode.c */
int EXTERNAL_FIXED d_encode_dba(P1(int) Pi(long) Pi(DB_ADDR DB_FAR *));

 /* dbacode.c */

#ifndef MULTI_TASK
#define dt_opentask(a)                 /**/
#define dt_closetask(a)                /**/
#define dt_newcache(a, b)
#define dt_sharecache(a, b)
#define dt_internals(a, b, c, d, e, f)       /**/

int EXTERNAL_FIXED d_setpages(P1(int) Pi(int)); /* dio.c */
int EXTERNAL_FIXED d_iclose(TASK_DBN_D1);       /* dblfcns.c */
int EXTERNAL_FIXED d_close(TASK_D1);       /* dblfcns.c */
int EXTERNAL_FIXED d_closeall(TASK_D1);/* closeall.c */

 /* dbarc.c */
int EXTERNAL_DBN d_cmstat(P1(int) DBN_Dn);      /* cmstat.c */
int EXTERNAL_DBN d_cmtype(P1(int) Pi(int DB_FAR *) DBN_Dn);     /* cmtype.c */
int EXTERNAL_DBN d_connect(P1(int) DBN_Dn);     /* connect.c */
int EXTERNAL_DBN d_costat(P1(int) DBN_Dn);      /* costat.c */
int EXTERNAL_DBN d_cotype(P1(int) Pi(int DB_FAR *) DBN_Dn);     /* cotype.c */
int EXTERNAL_DBN d_crget(P1(DB_ADDR DB_FAR *) DBN_Dn);  /* crget.c */
int EXTERNAL_DBN d_crread(P1(long) Pi(DB_VOID DB_FAR *) DBN_Dn);        /* crread.c */
int EXTERNAL_DBN d_crset(P1(DB_ADDR DB_FAR *) DBN_Dn);  /* crset.c */
int EXTERNAL_DBN d_crstat(DBN_D1);       /* crstat.c */
int EXTERNAL_DBN d_crtype(P1(int DB_FAR *) DBN_Dn);     /* crtype.c */
int EXTERNAL_DBN d_crwrite(P1(long) Pi(DB_VOID DB_FAR *) DBN_Dn);       /* crwrite.c */
int EXTERNAL_DBN d_csmget(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn); /* csmget.c */
int EXTERNAL_DBN d_csmread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) DBN_Dn);

 /* csmread.c */
int EXTERNAL_DBN d_csmset(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn); /* csmset.c */
int EXTERNAL_DBN d_csmwrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);

 /* csmwrite.c */
int EXTERNAL_DBN d_csoget(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn); /* csoget.c */
int EXTERNAL_DBN d_csoread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) DBN_Dn);

 /* csoread.c */
int EXTERNAL_DBN d_csoset(P1(int) Pi(DB_ADDR DB_FAR *) DBN_Dn); /* csoset.c */
int EXTERNAL_DBN d_csowrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);

 /* csowrite.c */
int EXTERNAL_DBN d_csstat(P1(int) DBN_Dn);      /* csstat.c */
int EXTERNAL_DBN d_ctscm(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* ctscm.c */
int EXTERNAL_DBN d_ctsco(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* ctsco.c */
int EXTERNAL_DBN d_ctscr(P1(DB_ULONG DB_FAR *) DBN_Dn); /* ctscr.c */

int EXTERNAL_FIXED d_ctbpath(P1(CONST char DB_FAR *));  /* pathfcns.c */

int EXTERNAL_DBN d_curkey(DBN_D1);       /* keyfcns.c */
int EXTERNAL_FIXED d_dbdpath(P1(CONST char DB_FAR *));  /* dbdpath.c */
int EXTERNAL_FIXED d_dbfpath(P1(CONST char DB_FAR *));  /* dbfpath.c */
int EXTERNAL_FIXED d_dblog(P1(CONST char DB_FAR *));    /* dblog.c */
int EXTERNAL_FIXED d_dbtaf(P1(CONST char DB_FAR *));    /* dbtaf.c */
int EXTERNAL_FIXED d_dbtmp(P1(CONST char DB_FAR *));    /* dbtaf.c */
int EXTERNAL_FIXED d_dbnum(P1(CONST char DB_FAR *) TASK_Di);    /* dbnum.c */
int EXTERNAL_FIXED d_dbuserid(P1(CONST char DB_FAR *)); /* dbuserid.c */
int EXTERNAL_FIXED d_checkid(P1(char DB_FAR *));        /* dbuserid.c */
int EXTERNAL_DBN d_delete(DBN_D1);       /* delete.c */
int EXTERNAL_DBN d_disdel(DBN_D1);       /* disdel.c */
int EXTERNAL_DBN d_recnext(DBN_D1);       /* recnext.c */
int EXTERNAL_DBN d_recprev(DBN_D1);       /* recprev.c */
int EXTERNAL_FIXED d_destroy(P1(CONST char DB_FAR *));  /* destroy.c */
int EXTERNAL_DBN d_discon(P1(int) DBN_Dn);      /* discon.c */
int EXTERNAL_DBN d_fillnew(P1(int) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);

 /* fillnew.c */
int EXTERNAL_DBN d_findco(P1(int) DBN_Dn);      /* findco.c */
int EXTERNAL_DBN d_findfm(P1(int) DBN_Dn);      /* findfm.c */
int EXTERNAL_DBN d_findlm(P1(int) DBN_Dn);      /* findlm.c */
int EXTERNAL_DBN d_findnm(P1(int) DBN_Dn);      /* findnm.c */
int EXTERNAL_DBN d_findpm(P1(int) DBN_Dn);      /* findpm.c */

int EXTERNAL_FIXED d_freeall(P0);       /* dblfcns.c */

int EXTERNAL_DBN d_gtscm(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* gtscm.c */
int EXTERNAL_DBN d_gtsco(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* gtsco.c */
int EXTERNAL_DBN d_gtscr(P1(DB_ULONG DB_FAR *) DBN_Dn); /* gtscr.c */
int EXTERNAL_DBN d_gtscs(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* gtscs.c */
int EXTERNAL_DBN d_initialize(DBN_D1);       /* initial.c */
int EXTERNAL_DBN d_initfile(P1(FILE_NO) DBN_Dn);        /* initial.c */
int EXTERNAL_DBN d_ismember(P1(int) DBN_Dn);    /* ismember.c */
int EXTERNAL_DBN d_isowner(P1(int) DBN_Dn);     /* isowner.c */

#ifndef NO_KEYBUILD
int EXTERNAL_DBN d_keybuild(DBN_D1);       /* keybuild.c */

#endif
int EXTERNAL_DBN d_keydel(P1(long) DBN_Dn);     /* keydel.c */
int EXTERNAL_DBN d_keyexist(P1(long) DBN_Dn);   /* keyexist.c */
int EXTERNAL_DBN d_keyfind(P1(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);

 /* keyfind.c */
int EXTERNAL_DBN d_keyfree(P1(long) DBN_Dn);    /* dblfcns.c */

int EXTERNAL_DBN d_keyfrst(P1(long) DBN_Dn);    /* keyfrst.c */
int EXTERNAL_DBN d_keylast(P1(long) DBN_Dn);    /* keylast.c */

int EXTERNAL_DBN d_keylock(P1(long) Pi(char DB_FAR *) DBN_Dn);  /* dblfcns.c */
int EXTERNAL_DBN d_keylstat(P1(long) Pi(char DB_FAR *) DBN_Dn); /* dblfcns.c */

int EXTERNAL_DBN d_keynext(P1(long) DBN_Dn);    /* keynext.c */
int EXTERNAL_DBN d_keyprev(P1(long) DBN_Dn);    /* keyprev.c */
int EXTERNAL_FIXED d_keyread(P1(DB_VOID DB_FAR *));     /* keyfcns.c */
int EXTERNAL_DBN d_keystore(P1(long) DBN_Dn);   /* keystore.c */

int EXTERNAL_DBN d_lock(P1(int) Pi(LOCK_REQUEST DB_FAR *) DBN_Dn);      /* dblfcns.c */
int EXTERNAL_FIXED d_lockmgr(P1(CONST char DB_FAR *));  /* lockmgr.c */

int EXTERNAL_DBN d_makenew(P1(int) DBN_Dn);     /* makenew.c */
int EXTERNAL_FIXED d_mapchar(P1(unsigned char) Pi(unsigned char)
                              Pi(CONST char DB_FAR *) Pi(unsigned char));

 /* mapchar.c */
int EXTERNAL_DBN d_members(P1(int) Pi(long DB_FAR *) DBN_Dn);   /* members.c */
int EXTERNAL_FIXED d_off_opt(P1(unsigned long));       /* options.c */
int EXTERNAL_FIXED d_on_opt(P1(unsigned long));       /* options.c */

int EXTERNAL_FIXED d_iopen(P1(CONST char DB_FAR *));

int EXTERNAL_FIXED d_open(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *));
int EXTERNAL_FIXED d_setfiles(P1(int));/* dio.c */

 /* dblfcns.c */
int EXTERNAL_FIXED d_rerdcurr(P1(DB_ADDR DB_FAR **) DBN_Dn);   /* rwcurr.c */
int EXTERNAL_FIXED d_rdcurr(P1(DB_ADDR DB_FAR **) Pi(int DB_FAR *) DBN_Dn);

 /* rwcurr.c */
int EXTERNAL_FIXED d_wrcurr(P1(DB_ADDR DB_FAR *) DBN_Dn);      /* rwcurr.c */

int EXTERNAL_DBN d_recfree(P1(int) DBN_Dn);     /* dblfcns.c */

int EXTERNAL_DBN d_recfrst(P1(int) DBN_Dn);     /* recfrst.c */
int EXTERNAL_DBN d_reclast(P1(int) DBN_Dn);     /* reclast.c */

int EXTERNAL_DBN d_reclock(P1(int) Pi(char DB_FAR *) DBN_Dn);   /* dblfcns.c */
int EXTERNAL_DBN d_reclstat(P1(int) Pi(char DB_FAR *) DBN_Dn);  /* dblfcns.c */

int EXTERNAL_FIXED d_recover(P1(CONST char DB_FAR *));  /* recover.c */

int EXTERNAL_DBN d_recread(P1(DB_VOID DB_FAR *) DBN_Dn);        /* recread.c */
int EXTERNAL_DBN d_recset(P1(int) DBN_Dn);      /* recset.c */
int EXTERNAL_FIXED d_recstat(P1(DB_ADDR) Pi(DB_ULONG) DBN_Dn);  /* recstat.c */
int EXTERNAL_DBN d_recwrite(P1(CONST DB_VOID DB_FAR *) DBN_Dn); /* recwrite.c */
int EXTERNAL_FIXED d_renfile(P1(CONST char DB_FAR *) Pi(FILE_NO)
                                Pi(CONST char DB_FAR *));       /* renfile.c */
int EXTERNAL_FIXED d_renclean(TASK_D1);/* renfile.c */

int EXTERNAL_FIXED d_rlbclr(DBN_D1);       /* dblfcns.c */
int EXTERNAL_FIXED d_rlbset(DBN_D1);       /* dblfcns.c */
int EXTERNAL_FIXED d_rlbtst(DBN_D1);       /* dblfcns.c */

int EXTERNAL_FIXED d_set_dberr(P1(ERRORPROC));  /* dberr.c */

int EXTERNAL_FIXED d_setdb(P1(int));       /* setdb.c */

int EXTERNAL_DBN d_setfree(P1(int) DBN_Dn);     /* dblfcns.c */

int EXTERNAL_DBN d_setkey(P1(long) Pi(CONST DB_VOID DB_FAR *) DBN_Dn);  /* makenew.c */

int EXTERNAL_DBN d_setlock(P1(int) Pi(char DB_FAR *) DBN_Dn);   /* dblfcns.c */
int EXTERNAL_DBN d_setlstat(P1(int) Pi(char DB_FAR *) DBN_Dn);  /* dblfcns.c */

int EXTERNAL_DBN d_setmm(P1(int) Pi(int) DBN_Dn);       /* setmm.c */
int EXTERNAL_DBN d_setmo(P1(int) Pi(int) DBN_Dn);       /* setmo.c */
int EXTERNAL_DBN d_setmr(P1(int) DBN_Dn);       /* setmr.c */
int EXTERNAL_DBN d_setom(P1(int) Pi(int) DBN_Dn);       /* setom.c */
int EXTERNAL_DBN d_setoo(P1(int) Pi(int) DBN_Dn);       /* setoo.c */
int EXTERNAL_DBN d_setor(P1(int) DBN_Dn);       /* setor.c */
int EXTERNAL_DBN d_setrm(P1(int) DBN_Dn);       /* setrm.c */
int EXTERNAL_DBN d_setro(P1(int) DBN_Dn);       /* setro.c */
int EXTERNAL_DBN d_stscm(P1(int) Pi(DB_ULONG) DBN_Dn);  /* stscm.c */
int EXTERNAL_DBN d_stsco(P1(int) Pi(DB_ULONG) DBN_Dn);  /* stsco.c */
int EXTERNAL_DBN d_stscr(P1(DB_ULONG) DBN_Dn);  /* stscr.c */
int EXTERNAL_DBN d_stscs(P1(int) Pi(DB_ULONG) DBN_Dn);  /* stscs.c */

int EXTERNAL_FIXED d_timeout(P1(int));       /* dblfcns.c */
int EXTERNAL_FIXED d_trabort(P0);       /* dblfcns.c */
int EXTERNAL_FIXED d_trbegin(P1(CONST char DB_FAR *));  /* dblfcns.c */
int EXTERNAL_FIXED d_trend(P0);        /* dblfcns.c */

int EXTERNAL_DBN d_utscm(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* utscm.c */
int EXTERNAL_DBN d_utsco(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* utsco.c */
int EXTERNAL_DBN d_utscr(P1(DB_ULONG DB_FAR *) DBN_Dn); /* utscr.c */
int EXTERNAL_DBN d_utscs(P1(int) Pi(DB_ULONG DB_FAR *) DBN_Dn); /* utscs.c */

#endif

#endif
/* vpp @dos.vpp DPROTO.h (Thu May 21 10:27:25 1992)
 *
 *     -dMSDOS -dDB_SHARE -nUNIX -nRISC -nSUN -nSCO -nWIN3 -nWINDOWS -nVMS
 *     -nOS2 -nOS2_MT -nREOPEN_FILES -nFAR_ALLOC -nARCHIVING -nDB_DLL -nBSD
 *     -nMEMLOCK -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
 *     -nNO_TRANS -nVer330 -n_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED
 *     -nOLD_WAY -nTRUENAME -nLAT
 */

