/*----------------------------------------------------------------------------
   dtcover.h: converts d_* calls to dt_* for MULTI_TASKing

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------------*/

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
      01-Sep-89 RTK Added function d_checkid
 1328 23-Oct-91 AOY Added dt_dbtaf
*/

#ifndef DTCOVER_H
#define DTCOVER_H


/* Unconditionally defined functions */
#define d_setpages              dt_setpages
#define d_iclose                dt_iclose
#define d_close                 dt_close
#define d_closeall              dt_closeall
#define d_checkid               dt_checkid
#define d_cmstat                dt_cmstat
#define d_cmtype                dt_cmtype
#define d_connect               dt_connect
#define d_cotype                dt_cotype
#define d_crget                 dt_crget
#define d_crread                dt_crread
#define d_crset                 dt_crset
#define d_crtype                dt_crtype
#define d_crwrite               dt_crwrite
#define d_csmget                dt_csmget
#define d_csmread               dt_csmread
#define d_csmset                dt_csmset
#define d_csmwrite              dt_csmwrite
#define d_csoget                dt_csoget
#define d_csoread               dt_csoread
#define d_csoset                dt_csoset
#define d_csowrite              dt_csowrite
#define d_ctbpath               dt_ctbpath
#define d_curkey                dt_curkey
#define d_dbnum                 dt_dbnum
#define d_dbdpath               dt_dbdpath
#define d_dbfpath               dt_dbfpath
#define d_dblog                 dt_dblog
#define d_dbtaf                 dt_dbtaf
#define d_dbtmp                 dt_dbtmp
#define d_dbuserid              dt_dbuserid
#define d_delete                dt_delete
#define d_disdel                dt_disdel
#define d_recnext               dt_recnext
#define d_recprev               dt_recprev
#define d_destroy               dt_destroy
#define d_discon                dt_discon
#define d_fillnew               dt_fillnew
#define d_findco                dt_findco
#define d_findfm                dt_findfm
#define d_findlm                dt_findlm
#define d_findnm                dt_findnm
#define d_findpm                dt_findpm
#define d_initialize            dt_initialize
#define d_initfile              dt_initfile
#define d_ismember              dt_ismember
#define d_isowner               dt_isowner
#define d_keybuild              dt_keybuild
#define d_keydel                dt_keydel
#define d_keyexist              dt_keyexist
#define d_keyfind               dt_keyfind
#define d_keyfrst               dt_keyfrst
#define d_keylast               dt_keylast
#define d_keynext               dt_keynext
#define d_keyprev               dt_keyprev
#define d_keyread               dt_keyread
#define d_keystore              dt_keystore
#define d_makenew               dt_makenew
#define d_mapchar               dt_mapchar
#define d_members               dt_members
#define d_off_opt               dt_off_opt
#define d_on_opt                dt_on_opt
#define d_iopen                 dt_iopen
#define d_open                  dt_open
#define d_rdcurr                dt_rdcurr
#define d_rerdcurr              dt_rerdcurr
#define d_recfrst               dt_recfrst
#define d_reclast               dt_reclast
#define d_recread               dt_recread
#define d_recset                dt_recset
#define d_recwrite              dt_recwrite
#define d_renfile               dt_renfile
#define d_renclean              dt_renclean
#define d_set_dberr             dt_set_dberr
#define d_setdb                 dt_setdb
#define d_setfiles              dt_setfiles
#define d_setkey                dt_setkey
#define d_setmm                 dt_setmm
#define d_setmo                 dt_setmo
#define d_setmr                 dt_setmr
#define d_setom                 dt_setom
#define d_setoo                 dt_setoo
#define d_setor                 dt_setor
#define d_setrm                 dt_setrm
#define d_setro                 dt_setro
#define d_wrcurr                dt_wrcurr

/* Timestamping functions */
#define d_costat                dt_costat
#define d_crstat                dt_crstat
#define d_csstat                dt_csstat
#define d_ctscm                 dt_ctscm
#define d_ctsco                 dt_ctsco
#define d_ctscr                 dt_ctscr
#define d_gtscm                 dt_gtscm
#define d_gtsco                 dt_gtsco
#define d_gtscr                 dt_gtscr
#define d_gtscs                 dt_gtscs
#define d_recstat               dt_recstat
#define d_stscm                 dt_stscm
#define d_stsco                 dt_stsco
#define d_stscr                 dt_stscr
#define d_stscs                 dt_stscs
#define d_utscm                 dt_utscm
#define d_utsco                 dt_utsco
#define d_utscr                 dt_utscr
#define d_utscs                 dt_utscs

/* Multi-user/Transaction functions */
#define d_freeall               dt_freeall
#define d_keyfree               dt_keyfree
#define d_keylock               dt_keylock
#define d_keylstat              dt_keylstat
#define d_lock                  dt_lock
#define d_lockmgr               dt_lockmgr
#define d_recfree               dt_recfree
#define d_reclock               dt_reclock
#define d_reclstat              dt_reclstat
#define d_recover               dt_recover
#define d_rlbclr                dt_rlbclr
#define d_rlbset                dt_rlbset
#define d_rlbtst                dt_rlbtst
#define d_setfree               dt_setfree
#define d_setlock               dt_setlock
#define d_setlstat              dt_setlstat
#define d_timeout               dt_timeout
#define d_trabort               dt_trabort
#define d_trbegin               dt_trbegin
#define d_trend                 dt_trend

#endif
/* vpp @win.vpp DTCOVER.h (Thu May 21 10:46:59 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

