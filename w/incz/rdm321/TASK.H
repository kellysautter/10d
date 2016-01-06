/*----------------------------------------------------------------------------
   task.h: db_VISTA task cover function declaration header file

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

#ifndef TASK_H
#define TASK_H

/* ********************** EDIT HISTORY *******************************
 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
  571 31-Jan-89 RSC Placed ctbpath inside NO_COUNTRY, not NO_TIMESTAMP
      01-Sep-89 RTK Removed functions which do not take a task parameter
      01-Sep-89 RTK Added function d_checkid
      05-Jan-90 KGM Uses DB_VOID now
 1421 25-Aug-91 DJS Changed DB_TASK parm to dt_internals to pointer
 1430 30-Oct-91 DMB Full support of context switching using multiple caches
 1328 03-Nov-91 AOY Added dt_dbtaf()
*/


int EXTERNAL_FIXED dt_setpages(P1(int) Pi(int) Pi(DB_TASK DB_FAR *));
int EXTERNAL_FIXED dt_opentask(P1(DB_TASK DB_FAR *));
int EXTERNAL_FIXED dt_closetask(P1(DB_TASK DB_FAR *));
int EXTERNAL_FIXED dt_newcache(P1(DB_TASK DB_FAR *) Pi(int));
int EXTERNAL_FIXED dt_sharecache(P1(DB_TASK DB_FAR *) Pi(DB_TASK DB_FAR *));
int EXTERNAL_FIXED dt_internals(P1(DB_TASK DB_FAR *) Pi(int) Pi(int) Pi(int)
                                   Pi(DB_VOID DB_FAR *) Pi(unsigned));

int EXTERNAL_FIXED dt_iclose(TASK_D1 DBN_Dn);   /* dblfcns.c */
int EXTERNAL_FIXED dt_close(TASK_D1);       /* dblfcns.c */
int EXTERNAL_FIXED dt_closeall(TASK_D1);        /* closeall.c */

 /* dbarc.c */
int EXTERNAL_DBN dt_cmstat(P1(int) TASK_Di DBN_Dn);     /* cmstat.c */
int EXTERNAL_DBN dt_cmtype(P1(int) Pi(int DB_FAR *) TASK_Di DBN_Dn);

 /* cmtype.c */
int EXTERNAL_DBN dt_costat(P1(int) TASK_Di DBN_Dn);     /* costat.c */
int EXTERNAL_DBN dt_connect(P1(int) TASK_Di DBN_Dn);    /* connect.c */
int EXTERNAL_DBN dt_cotype(P1(int) Pi(int DB_FAR *) TASK_Di DBN_Dn);

 /* cotype.c */
int EXTERNAL_DBN dt_crget(P1(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);

 /* crget.c */
int EXTERNAL_DBN dt_crread(P1(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* crread.c */
int EXTERNAL_DBN dt_crset(P1(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);

 /* crset.c */
int EXTERNAL_DBN dt_crstat(TASK_D1 DBN_Dn);     /* crstat.c */
int EXTERNAL_DBN dt_crtype(P1(int DB_FAR *) TASK_Di DBN_Dn);    /* crtype.c */
int EXTERNAL_DBN dt_crwrite(P1(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* crwrite.c */
int EXTERNAL_DBN dt_csmget(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);

 /* csmget.c */
int EXTERNAL_DBN dt_csmread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* csmread.c */
int EXTERNAL_DBN dt_csmset(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);

 /* csmset.c */
int EXTERNAL_DBN dt_csmwrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di
                                DBN_Dn);        /* csmwrite.c */
int EXTERNAL_DBN dt_csoget(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);

 /* csoget.c */
int EXTERNAL_DBN dt_csoread(P1(int) Pi(long) Pi(DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* csoread.c */
int EXTERNAL_DBN dt_csoset(P1(int) Pi(DB_ADDR DB_FAR *) TASK_Di DBN_Dn);

 /* csoset.c */
int EXTERNAL_DBN dt_csowrite(P1(int) Pi(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di
                                DBN_Dn);        /* csowrite.c */
int EXTERNAL_DBN dt_csstat(P1(int) TASK_Di DBN_Dn);     /* csstat.c */
int EXTERNAL_DBN dt_ctscm(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* ctscm.c */
int EXTERNAL_DBN dt_ctsco(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* ctsco.c */
int EXTERNAL_DBN dt_ctscr(P1(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);        /* ctscr.c */

int EXTERNAL_FIXED dt_ctbpath(P1(CONST char DB_FAR *) TASK_Di); /* pathfcns.c */

int EXTERNAL_DBN dt_curkey(TASK_D1 DBN_Dn);     /* curkey.c */
int EXTERNAL_FIXED dt_dbdpath(P1(CONST char DB_FAR *) TASK_Di); /* dbdpath.c */
int EXTERNAL_FIXED dt_dbfpath(P1(CONST char DB_FAR *) TASK_Di); /* dbfpath.c */
int EXTERNAL_FIXED dt_dblog(P1(CONST char DB_FAR *) TASK_Di);   /* dblog.c */
int EXTERNAL_FIXED dt_dbtaf(P1(CONST char DB_FAR *) TASK_Di);   /* dbtaf.c */
int EXTERNAL_FIXED dt_dbtmp(P1(CONST char DB_FAR *) TASK_Di);   /* dbtaf.c */
int EXTERNAL_FIXED dt_dbnum(P1(CONST char DB_FAR *) TASK_Di);   /* dbnum.c */
int EXTERNAL_FIXED dt_dbuserid(P1(CONST char DB_FAR *) TASK_Di);        /* dbuserid.c */
int EXTERNAL_FIXED dt_checkid(P1(char DB_FAR *) TASK_Di);       /* dbuserid.c */
int EXTERNAL_DBN dt_delete(TASK_D1 DBN_Dn);     /* delete.c */
int EXTERNAL_DBN dt_disdel(TASK_D1 DBN_Dn);     /* disdel.c */
int EXTERNAL_DBN dt_recnext(TASK_D1 DBN_Dn);    /* recnext.c */
int EXTERNAL_DBN dt_recprev(TASK_D1 DBN_Dn);    /* recprev.c */
int EXTERNAL_FIXED dt_setfiles(P1(int) TASK_Di);        /* dio.c */
int EXTERNAL_FIXED dt_destroy(P1(CONST char DB_FAR *) TASK_Di); /* destroy.c */
int EXTERNAL_DBN dt_discon(P1(int) TASK_Di DBN_Dn);     /* discon.c */
int EXTERNAL_DBN dt_fillnew(P1(int) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* fillnew.c */
int EXTERNAL_DBN dt_findco(P1(int) TASK_Di DBN_Dn);     /* findco.c */
int EXTERNAL_DBN dt_findfm(P1(int) TASK_Di DBN_Dn);     /* findfm.c */
int EXTERNAL_DBN dt_findlm(P1(int) TASK_Di DBN_Dn);     /* findlm.c */
int EXTERNAL_DBN dt_findnm(P1(int) TASK_Di DBN_Dn);     /* findnm.c */
int EXTERNAL_DBN dt_findpm(P1(int) TASK_Di DBN_Dn);     /* findpm.c */
int EXTERNAL_DBN dt_gtscm(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* gtscm.c */
int EXTERNAL_DBN dt_gtsco(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* gtsco.c */
int EXTERNAL_DBN dt_gtscr(P1(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);        /* gtscr.c */
int EXTERNAL_DBN dt_gtscs(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* gtscs.c */
int EXTERNAL_DBN dt_initialize(TASK_D1 DBN_Dn); /* initial.c */
int EXTERNAL_DBN dt_initfile(P1(FILE_NO) TASK_Di DBN_Dn);       /* initial.c */
int EXTERNAL_DBN dt_ismember(P1(int) TASK_Di DBN_Dn);   /* ismember.c */
int EXTERNAL_DBN dt_isowner(P1(int) TASK_Di DBN_Dn);    /* isowner.c */

#ifndef NO_KEYBUILD
int EXTERNAL_DBN dt_keybuild(TASK_D1 DBN_Dn);   /* keybuild.c */

#endif
int EXTERNAL_DBN dt_keydel(P1(long) TASK_Di DBN_Dn);    /* keydel.c */
int EXTERNAL_DBN dt_keyexist(P1(long) TASK_Di DBN_Dn);  /* keyexist.c */
int EXTERNAL_DBN dt_keyfind(P1(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* keyfind.c */
int EXTERNAL_DBN dt_keyfrst(P1(long) TASK_Di DBN_Dn);   /* keyfrst.c */
int EXTERNAL_DBN dt_keylast(P1(long) TASK_Di DBN_Dn);   /* keylast.c */
int EXTERNAL_DBN dt_keynext(P1(long) TASK_Di DBN_Dn);   /* keynext.c */
int EXTERNAL_DBN dt_keyprev(P1(long) TASK_Di DBN_Dn);   /* keyprev.c */
int EXTERNAL_FIXED dt_keyread(P1(DB_VOID DB_FAR *) TASK_Di);    /* keyfcns.c */
int EXTERNAL_DBN dt_keystore(P1(long) TASK_Di DBN_Dn);  /* keystore.c */
int EXTERNAL_FIXED dt_lockmgr(P1(CONST char DB_FAR *) TASK_Di); /* lockmgr.c */

int EXTERNAL_DBN dt_makenew(P1(int) TASK_Di DBN_Dn);    /* makenew.c */
int EXTERNAL_FIXED dt_mapchar(P1(unsigned char) Pi(unsigned char)
                      Pi(CONST char DB_FAR *) Pi(unsigned char) TASK_Di);

 /* mapchar.c */
int EXTERNAL_DBN dt_members(P1(int) Pi(long DB_FAR *) TASK_Di DBN_Dn);

 /* members.c */
int EXTERNAL_FIXED dt_off_opt(P1(unsigned long) TASK_Di); /* options.c */
int EXTERNAL_FIXED dt_on_opt(P1(unsigned long) TASK_Di);  /* options.c */

int EXTERNAL_FIXED dt_iopen(P1(CONST char DB_FAR *) TASK_Di);

int EXTERNAL_FIXED dt_open(P1(CONST char DB_FAR *) Pi(CONST char DB_FAR *) TASK_Di);

 /* dblfcns.c */
int EXTERNAL_FIXED dt_rdcurr(P1(DB_ADDR_P DB_FAR *) Pi(int DB_FAR *) TASK_Di DBN_Dn);

 /* rwcurr.c */
int EXTERNAL_FIXED dt_rerdcurr(P1(DB_ADDR_P DB_FAR *) TASK_Di DBN_Dn); /* rwcurr.c */
int EXTERNAL_FIXED dt_wrcurr(P1(DB_ADDR_P) TASK_Di DBN_Dn);    /* rwcurr.c */

int EXTERNAL_DBN dt_recfrst(P1(int) TASK_Di DBN_Dn);    /* recfrst.c */
int EXTERNAL_DBN dt_reclast(P1(int) TASK_Di DBN_Dn);    /* reclast.c */
int EXTERNAL_DBN dt_recread(P1(DB_VOID DB_FAR *) TASK_Di DBN_Dn);       /* recread.c */
int EXTERNAL_DBN dt_recset(P1(int) TASK_Di DBN_Dn);     /* recset.c */
int EXTERNAL_FIXED dt_recstat(P1(DB_ADDR) Pi(DB_ULONG) TASK_Di DBN_Dn);

 /* recstat.c */
int EXTERNAL_DBN dt_recwrite(P1(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* recwrite.c */
int EXTERNAL_FIXED dt_renfile(P1(CONST char DB_FAR *) Pi(FILE_NO)
                                 Pi(CONST char DB_FAR *) TASK_Di);

 /* renfile.c */
int EXTERNAL_FIXED dt_renclean(TASK_D1);        /* renfile.c */
int EXTERNAL_FIXED dt_set_dberr(P1(ERRORPROC) TASK_Di); /* dberr.c */

int EXTERNAL_FIXED dt_setdb(P1(int) TASK_Di);   /* setdb.c */

int EXTERNAL_DBN dt_setkey(P1(long) Pi(CONST DB_VOID DB_FAR *) TASK_Di DBN_Dn);

 /* makenew.c */
int EXTERNAL_DBN dt_setmm(P1(int) Pi(int) TASK_Di DBN_Dn);      /* setmm.c */
int EXTERNAL_DBN dt_setmo(P1(int) Pi(int) TASK_Di DBN_Dn);      /* setmo.c */
int EXTERNAL_DBN dt_setmr(P1(int) TASK_Di DBN_Dn);      /* setmr.c */
int EXTERNAL_DBN dt_setom(P1(int) Pi(int) TASK_Di DBN_Dn);      /* setom.c */
int EXTERNAL_DBN dt_setoo(P1(int) Pi(int) TASK_Di DBN_Dn);      /* setoo.c */
int EXTERNAL_DBN dt_setor(P1(int) TASK_Di DBN_Dn);      /* setor.c */
int EXTERNAL_DBN dt_setrm(P1(int) TASK_Di DBN_Dn);      /* setrm.c */
int EXTERNAL_DBN dt_setro(P1(int) TASK_Di DBN_Dn);      /* setro.c */
int EXTERNAL_DBN dt_stscm(P1(int) Pi(DB_ULONG) TASK_Di DBN_Dn);

 /* stscm.c */
int EXTERNAL_DBN dt_stsco(P1(int) Pi(DB_ULONG) TASK_Di DBN_Dn);

 /* stsco.c */
int EXTERNAL_DBN dt_stscr(P1(DB_ULONG) TASK_Di DBN_Dn); /* stscr.c */
int EXTERNAL_DBN dt_stscs(P1(int) Pi(DB_ULONG) TASK_Di DBN_Dn);

 /* stscs.c */
int EXTERNAL_DBN dt_utscm(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* utscm.c */
int EXTERNAL_DBN dt_utsco(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* utsco.c */
int EXTERNAL_DBN dt_utscr(P1(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);        /* utscr.c */
int EXTERNAL_DBN dt_utscs(P1(int) Pi(DB_ULONG DB_FAR *) TASK_Di DBN_Dn);

 /* utscs.c */
int EXTERNAL_FIXED dt_freeall(TASK_D1);/* dblfcns.c */
int EXTERNAL_DBN dt_keyfree(P1(long) TASK_Di DBN_Dn);   /* dblfcns.c */
int EXTERNAL_DBN dt_keylock(P1(long) Pi(char DB_FAR *) TASK_Di DBN_Dn);

 /* dblfcns.c */
int EXTERNAL_DBN dt_keylstat(P1(long) Pi(char DB_FAR *) TASK_Di DBN_Dn);

 /* dblfcns.c */
int EXTERNAL_DBN dt_lock(P1(int) Pi(LOCK_REQUEST DB_FAR *) TASK_Di DBN_Dn);

 /* dblfcns.c */
int EXTERNAL_DBN dt_recfree(P1(int) TASK_Di DBN_Dn);    /* dblfcns.c */
int EXTERNAL_DBN dt_reclock(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);

 /* dblfcns.c */
int EXTERNAL_DBN dt_reclstat(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);

 /* dblfcns.c */
int EXTERNAL_FIXED dt_recover(P1(CONST char DB_FAR *) TASK_Di); /* recover.c */
int EXTERNAL_FIXED dt_rlbclr(TASK_D1 DBN_Dn);   /* dblfcns.c */
int EXTERNAL_FIXED dt_rlbset(TASK_D1 DBN_Dn);   /* dblfcns.c */
int EXTERNAL_FIXED dt_rlbtst(TASK_D1 DBN_Dn);   /* dblfcns.c */
int EXTERNAL_DBN dt_setfree(P1(int) TASK_Di DBN_Dn);    /* dblfcns.c */
int EXTERNAL_DBN dt_setlock(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);

 /* dblfcns.c */
int EXTERNAL_DBN dt_setlstat(P1(int) Pi(char DB_FAR *) TASK_Di DBN_Dn);
int EXTERNAL_FIXED dt_timeout(P1(int) TASK_Di); /* dblfcns.c */
int EXTERNAL_FIXED dt_trabort(TASK_D1);/* dblfcns.c */
int EXTERNAL_FIXED dt_trbegin(P1(CONST char DB_FAR *) TASK_Di); /* dblfcns.c */
int EXTERNAL_FIXED dt_trend(TASK_D1);       /* dblfcns.c */

 /* dblfcns.c */

#ifndef NO_DT_COVER                    /* ! NO_DT_COVER */

#define d_setpages(a,b)              dt_setpages(a, b CURRTASK_PARM)
#define d_iclose(a)                  dt_iclose(CURRTASK_ONLY, a)
#define d_close()                    dt_close(CURRTASK_ONLY)
#define d_closeall()                 dt_closeall(CURRTASK_ONLY)
#define d_ctbpath(a)                 dt_ctbpath(a CURRTASK_PARM)
#define d_dbdpath(a)                 dt_dbdpath(a CURRTASK_PARM)
#define d_dbfpath(a)                 dt_dbfpath(a CURRTASK_PARM)
#define d_dblog(a)                   dt_dblog(a CURRTASK_PARM)
#define d_dbtaf(a)                   dt_dbtaf(a CURRTASK_PARM)
#define d_dbtmp(a)                   dt_dbtmp(a CURRTASK_PARM)
#define d_dbnum(a)                   dt_dbnum(a CURRTASK_PARM)
#define d_dbuserid(a)                dt_dbuserid(a CURRTASK_PARM)
#define d_checkid(a)                 dt_checkid(a CURRTASK_PARM)
#define d_destroy(a)                 dt_destroy(a CURRTASK_PARM)
#define d_freeall()                  dt_freeall(CURRTASK_ONLY)
#define d_off_opt(a)                 dt_off_opt(a CURRTASK_PARM)
#define d_on_opt(a)                  dt_on_opt(a CURRTASK_PARM)
#define d_iopen(a)                   dt_iopen(a CURRTASK_PARM)
#define d_lockmgr(a)                 dt_lockmgr(a CURRTASK_PARM)
#define d_mapchar(a, b, c, d)        dt_mapchar(a, b, c, d CURRTASK_PARM)
#define d_open(a, b)                 dt_open(a, b CURRTASK_PARM)
#define d_recover(a)                 dt_recover(a CURRTASK_PARM)
#define d_renfile(a, b, c)           dt_renfile(a, b, c CURRTASK_PARM)
#define d_renclean()                 dt_renclean(CURRTASK_ONLY)
#define d_set_dberr(a)               dt_set_dberr(a CURRTASK_PARM)
#define d_setdb(a)                   dt_setdb(a CURRTASK_PARM)
#define d_timeout(a)                 dt_timeout(a CURRTASK_PARM)
#define d_trabort()                  dt_trabort(CURRTASK_ONLY)
#define d_trbegin(a)                 dt_trbegin(a CURRTASK_PARM)
#define d_trend()                    dt_trend(CURRTASK_ONLY)

#define d_rdcurr(a, b, dbn)       dt_rdcurr(a, b CURRTASK_PARM, dbn)
#define d_rerdcurr(a, dbn)        dt_rerdcurr(a CURRTASK_PARM, dbn)
#define d_wrcurr(a, dbn)          dt_wrcurr(a CURRTASK_PARM, dbn)

#define d_recstat(a, b, dbn)      dt_recstat(a, b CURRTASK_PARM, dbn)
#define d_cmstat(a , dbn)         dt_cmstat(a CURRTASK_PARM , dbn)
#define d_cmtype(a, b , dbn)      dt_cmtype(a, b CURRTASK_PARM , dbn)
#define d_connect(a , dbn)        dt_connect(a CURRTASK_PARM , dbn)
#define d_costat(a , dbn)         dt_costat(a CURRTASK_PARM , dbn)
#define d_cotype(a, b , dbn)      dt_cotype(a, b CURRTASK_PARM , dbn)
#define d_crget(a , dbn)          dt_crget(a CURRTASK_PARM , dbn)
#define d_crread(a, b , dbn)      dt_crread(a, b CURRTASK_PARM , dbn)
#define d_crset(a , dbn)          dt_crset(a CURRTASK_PARM , dbn)
#define d_crstat(dbn)             dt_crstat(CURRTASK_ONLY , dbn)
#define d_crtype(a , dbn)         dt_crtype(a CURRTASK_PARM , dbn)
#define d_crwrite(a, b , dbn)     dt_crwrite(a, b CURRTASK_PARM , dbn)
#define d_csmget(a, b , dbn)      dt_csmget(a, b CURRTASK_PARM , dbn)
#define d_csmread(a, b, c , dbn)  dt_csmread(a, b, c CURRTASK_PARM , dbn)
#define d_csmset(a, b , dbn)      dt_csmset(a, b CURRTASK_PARM , dbn)
#define d_csmwrite(a, b, c , dbn) dt_csmwrite(a, b, c CURRTASK_PARM , dbn)
#define d_csoget(a, b , dbn)      dt_csoget(a, b CURRTASK_PARM , dbn)
#define d_csoread(a, b, c , dbn)  dt_csoread(a, b, c CURRTASK_PARM , dbn)
#define d_csoset(a, b , dbn)      dt_csoset(a, b CURRTASK_PARM , dbn)
#define d_csowrite(a, b, c , dbn) dt_csowrite(a, b, c CURRTASK_PARM , dbn)
#define d_csstat(a , dbn)         dt_csstat(a CURRTASK_PARM , dbn)
#define d_ctscm(a, b , dbn)       dt_ctscm(a, b CURRTASK_PARM , dbn)
#define d_ctsco(a, b , dbn)       dt_ctsco(a, b CURRTASK_PARM , dbn)
#define d_ctscr(a, dbn)           dt_ctscr(a    CURRTASK_PARM , dbn)
#define d_curkey(dbn)             dt_curkey(CURRTASK_ONLY , dbn)
#define d_delete(dbn)             dt_delete(CURRTASK_ONLY , dbn)
#define d_disdel(dbn)             dt_disdel(CURRTASK_ONLY , dbn)
#define d_recnext( dbn)           dt_recnext(CURRTASK_ONLY , dbn)
#define d_recprev( dbn)           dt_recprev(CURRTASK_ONLY , dbn)
#define d_discon(a , dbn)         dt_discon(a CURRTASK_PARM , dbn)
#define d_fillnew(a, b , dbn)     dt_fillnew(a, b CURRTASK_PARM , dbn)
#define d_findco(a , dbn)         dt_findco(a CURRTASK_PARM , dbn)
#define d_findfm(a , dbn)         dt_findfm(a CURRTASK_PARM , dbn)
#define d_findlm(a , dbn)         dt_findlm(a CURRTASK_PARM , dbn)
#define d_findnm(a , dbn)         dt_findnm(a CURRTASK_PARM , dbn)
#define d_findpm(a , dbn)         dt_findpm(a CURRTASK_PARM , dbn)
#define d_gtscm(a, b , dbn)       dt_gtscm(a, b CURRTASK_PARM , dbn)
#define d_gtsco(a, b , dbn)       dt_gtsco(a, b CURRTASK_PARM , dbn)
#define d_gtscr(a,     dbn)       dt_gtscr(a    CURRTASK_PARM , dbn)
#define d_gtscs(a, b , dbn)       dt_gtscs(a, b CURRTASK_PARM , dbn)
#define d_initialize( dbn)        dt_initialize(CURRTASK_ONLY , dbn)
#define d_initfile(a , dbn)       dt_initfile(a CURRTASK_PARM , dbn)
#define d_ismember(a , dbn)       dt_ismember(a CURRTASK_PARM , dbn)
#define d_isowner(a , dbn)        dt_isowner(a CURRTASK_PARM , dbn)
#ifndef NO_KEYBUILD
#define d_keybuild(dbn)           dt_keybuild(CURRTASK_ONLY , dbn)
#endif
#define d_keydel(a , dbn)         dt_keydel(a CURRTASK_PARM , dbn)
#define d_keyexist(a , dbn)       dt_keyexist(a CURRTASK_PARM , dbn)
#define d_keyfind(a, b , dbn)     dt_keyfind(a, b CURRTASK_PARM , dbn)
#define d_keyfree(a , dbn)        dt_keyfree(a CURRTASK_PARM , dbn)
#define d_keyfrst(a , dbn)        dt_keyfrst(a CURRTASK_PARM , dbn)
#define d_keylast(a , dbn)        dt_keylast(a CURRTASK_PARM , dbn)
#define d_keylock(a, b , dbn)     dt_keylock(a, b CURRTASK_PARM , dbn)
#define d_keylstat(a, b , dbn)    dt_keylstat(a, b CURRTASK_PARM , dbn)
#define d_keynext(a , dbn)        dt_keynext(a CURRTASK_PARM , dbn)
#define d_keyprev(a , dbn)        dt_keyprev(a CURRTASK_PARM , dbn)
#define d_keyread(a )             dt_keyread(a CURRTASK_PARM)
#define d_keystore(a , dbn)       dt_keystore(a CURRTASK_PARM , dbn)
#define d_lock(a, b , dbn)        dt_lock(a, b CURRTASK_PARM , dbn)
#define d_makenew(a , dbn)        dt_makenew(a CURRTASK_PARM , dbn)
#define d_members(a, b , dbn)     dt_members(a, b CURRTASK_PARM , dbn)
#define d_recfree(a , dbn)        dt_recfree(a CURRTASK_PARM , dbn)
#define d_recfrst(a , dbn)        dt_recfrst(a CURRTASK_PARM , dbn)
#define d_reclast(a , dbn)        dt_reclast(a CURRTASK_PARM , dbn)
#define d_reclock(a, b , dbn)     dt_reclock(a, b CURRTASK_PARM , dbn)
#define d_reclstat(a, b , dbn)    dt_reclstat(a, b CURRTASK_PARM , dbn)
#define d_recread(a , dbn)        dt_recread(a CURRTASK_PARM , dbn)
#define d_recset(a , dbn)         dt_recset(a CURRTASK_PARM , dbn)
#define d_recwrite(a , dbn)       dt_recwrite(a CURRTASK_PARM , dbn)
#define d_rlbclr(dbn)                   dt_rlbclr(CURRTASK_ONLY ,dbn)
#define d_rlbset(dbn)                   dt_rlbset(CURRTASK_ONLY ,dbn)
#define d_rlbtst(dbn)                   dt_rlbtst(CURRTASK_ONLY ,dbn)
#define d_setfree(a , dbn)        dt_setfree(a CURRTASK_PARM , dbn)
#define d_setfiles(a )            dt_setfiles(a CURRTASK_PARM )
#define d_setkey(a, b , dbn)      dt_setkey(a, b CURRTASK_PARM , dbn)
#define d_setlock(a, b , dbn)     dt_setlock(a, b CURRTASK_PARM , dbn)
#define d_setlstat(a, b , dbn)    dt_setlstat(a, b CURRTASK_PARM , dbn)
#define d_setmm(a, b , dbn)       dt_setmm(a, b CURRTASK_PARM , dbn)
#define d_setmo(a, b , dbn)       dt_setmo(a, b CURRTASK_PARM , dbn)
#define d_setmr(a , dbn)          dt_setmr(a CURRTASK_PARM , dbn)
#define d_setom(a, b , dbn)       dt_setom(a, b CURRTASK_PARM , dbn)
#define d_setoo(a, b , dbn)       dt_setoo(a, b CURRTASK_PARM , dbn)
#define d_setor(a , dbn)          dt_setor(a CURRTASK_PARM , dbn)
#define d_setrm(a , dbn)          dt_setrm(a CURRTASK_PARM , dbn)
#define d_setro(a , dbn)          dt_setro(a CURRTASK_PARM , dbn)

#define d_stscm(a, b , dbn)       dt_stscm(a, b CURRTASK_PARM , dbn)
#define d_stsco(a, b , dbn)       dt_stsco(a, b CURRTASK_PARM , dbn)
#define d_stscr(a,     dbn)       dt_stscr(a    CURRTASK_PARM , dbn)
#define d_stscs(a, b , dbn)       dt_stscs(a, b CURRTASK_PARM , dbn)
#define d_utscm(a, b , dbn)       dt_utscm(a, b CURRTASK_PARM , dbn)
#define d_utsco(a, b , dbn)       dt_utsco(a, b CURRTASK_PARM , dbn)
#define d_utscr(a,     dbn)       dt_utscr(a    CURRTASK_PARM , dbn)
#define d_utscs(a, b , dbn)       dt_utscs(a, b CURRTASK_PARM , dbn)


#endif                                 /* NO_DT_COVER */

#endif                                 /* TASK_H */
/* vpp @win.vpp TASK.h (Thu May 21 10:46:33 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

