/*----------------------------------------------------------------------
   vistase.g.h -- segmentation pragmas for windows dll's

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef VISTASEG_H
#define VISTASEG_H


#ifdef ALLOC_C
#pragma alloc_text (SG00002, d_memlock, d_memunlock, Memunlock)
#pragma alloc_text (SG00003, d_alloc, ll_deaccess, DBMalloc)
#pragma alloc_text (SG00006, d_lockpop)
#pragma alloc_text (SG00012, d_free)
#pragma alloc_text (SG00018, Memlock)
#pragma alloc_text (SG00019, d_lockpush, d_memlock_group, ll_free)
#pragma alloc_text (SG00023, CurrLock, CurrUnlock, ll_access, ll_append, ll_first)
#pragma alloc_text (SG00023, ll_next, ll_prepend, NewInit)
#pragma alloc_text (SG00025, d_memunlock_group)
#endif

#ifdef CLOSEALL_C
#pragma alloc_text (SG00026, dt_closeall)
#endif

#ifdef CMSTAT_C
#pragma alloc_text (SG00025, dt_cmstat)
#endif

#ifdef CMTYPE_C
#pragma alloc_text (SG00020, dt_cmtype)
#endif

#ifdef CONNECT_C
#pragma alloc_text (SG00003, dt_connect)
#pragma alloc_text (SG00018, sortcmp)
#endif

#ifdef COSTAT_C
#pragma alloc_text (SG00026, dt_costat)
#endif

#ifdef COTYPE_C
#pragma alloc_text (SG00008, dt_cotype)
#endif

#ifdef CRGET_C
#pragma alloc_text (SG00007, dt_crget)
#endif

#ifdef CRREAD_C
#pragma alloc_text (SG00010, dt_crread)
#endif

#ifdef CRSET_C
#pragma alloc_text (SG00007, dt_crset)
#endif

#ifdef CRSTAT_C
#pragma alloc_text (SG00006, dt_crstat)
#endif

#ifdef CRTYPE_C
#pragma alloc_text (SG00008, dt_crtype)
#endif

#ifdef CRWRITE_C
#pragma alloc_text (SG00028, dt_crwrite)
#endif

#ifdef CSMGET_C
#pragma alloc_text (SG00007, dt_csmget)
#endif

#ifdef CSMREAD_C
#pragma alloc_text (SG00025, dt_csmread)
#endif

#ifdef CSMSET_C
#pragma alloc_text (SG00007, dt_csmset)
#endif

#ifdef CSMWRITE_C
#pragma alloc_text (SG00028, dt_csmwrite)
#endif

#ifdef CSOGET_C
#pragma alloc_text (SG00002, dt_csoget)
#endif

#ifdef CSOREAD_C
#pragma alloc_text (SG00008, dt_csoread)
#endif

#ifdef CSOSET_C
#pragma alloc_text (SG00004, dt_csoset)
#endif

#ifdef CSOWRITE_C
#pragma alloc_text (SG00033, dt_csowrite)
#endif

#ifdef CSSTAT_C
#pragma alloc_text (SG00006, dt_csstat)
#endif

#ifdef CTSCM_C
#pragma alloc_text (SG00025, dt_ctscm)
#endif

#ifdef CTSCO_C
#pragma alloc_text (SG00008, dt_ctsco)
#endif

#ifdef CTSCR_C
#pragma alloc_text (SG00006, dt_ctscr)
#endif

#ifdef DBACODE_C
#pragma alloc_text (SG00006, d_decode_dba, d_encode_dba)
#endif

#ifdef DBDPATH_C
#pragma alloc_text (SG00006, dt_dbdpath)
#endif

#ifdef DBERR_C
#ifdef DB_DEBUG
#pragma alloc_text (SG00004, db_dberr)
#endif
#pragma alloc_text (SG00004, dberr)
#pragma alloc_text (SG00007, dt_set_dberr)
#pragma alloc_text (SG00013, dbautorec)
#endif

#ifdef DBFPATH_C
#pragma alloc_text (SG00006, dt_dbfpath)
#endif

#ifdef DBLFCNS_C
#pragma alloc_text (SG00004, dt_open)
#pragma alloc_text (SG00005, dt_rlbclr, dt_timeout, dt_trend, initses, neterr)
#pragma alloc_text (SG00005, send_free_pkt, send_lock_pkt, termses)
#pragma alloc_text (SG00006, dt_close, dt_reclstat, dt_rlbtst, dt_setlstat)
#pragma alloc_text (SG00007, dt_iopen)
#pragma alloc_text (SG00009, dt_keylock, dt_lock, dt_reclock, dt_setlock, keep_locks)
#pragma alloc_text (SG00009, lock_files, process_lock)
#pragma alloc_text (SG00010, dt_keyfree, dt_keylstat)
#pragma alloc_text (SG00012, dt_iclose, dt_trabort, free_table)
#pragma alloc_text (SG00013, dt_trbegin, recovery_check)
#pragma alloc_text (SG00014, dt_freeall, dt_recfree, free_dblocks, free_files)
#pragma alloc_text (SG00014, reset_locks)
#pragma alloc_text (SG00017, alloc_table, calloc_table, bld_lock_tables)
#pragma alloc_text (SG00019, dt_setfree, termfree)
#pragma alloc_text (SG00021, dopen, dupid_check, initdbt)
#pragma alloc_text (SG00030, dt_rlbset)
#endif

#ifdef DBLOG_C
#pragma alloc_text (SG00006, dt_dblog)
#endif

#ifdef DBTAF_C
#pragma alloc_text (SG00007, dt_dbtaf)
#endif

#ifdef DBUSERID_C
#pragma alloc_text (SG00026, dt_checkid)
#pragma alloc_text (SG00030, dt_dbuserid)
#endif

#ifdef DELETE_C
#pragma alloc_text (SG00018, dt_delete)
#endif

#ifdef DESTROY_C
#pragma alloc_text (SG00019, dt_destroy)
#endif

#ifdef DIO_C
#pragma alloc_text (SG00002, dio_findpg, dio_lock)
#pragma alloc_text (SG00004, dio_in, dio_pzalloc)
#pragma alloc_text (SG00006, dt_setpages)
#pragma alloc_text (SG00008, dio_read, dio_release)
#pragma alloc_text (SG00009, dio_clrfile)
#pragma alloc_text (SG00012, dio_clear, dio_free, dio_ixclear, dio_pzclr)
#pragma alloc_text (SG00014, dio_touch)
#pragma alloc_text (SG00015, clear_cache, dio_close, dio_closelru, dio_flush, dio_open)
#pragma alloc_text (SG00015, dio_out, dio_pzflush, dio_rrlb, dio_unlock)
#pragma alloc_text (SG00016, dio_get, dio_pzdel, dio_pzread, dio_unget)
#pragma alloc_text (SG00021, cache_init, dio_init, dio_pzinit)
#pragma alloc_text (SG00026, dt_setfiles)
#pragma alloc_text (SG00027, dio_pznext)
#pragma alloc_text (SG00028, dio_write)
#pragma alloc_text (SG00030, dio_wrlb)
#pragma alloc_text (SG00031, dio_setdef)
#pragma alloc_text (SG00028, dio_pzgetts, dio_pzsetts)
#endif

#ifdef DISCON_C
#pragma alloc_text (SG00028, dt_discon)
#endif

#ifdef DISDEL_C
#pragma alloc_text (SG00007, dt_disdel)
#endif

#ifdef ENTER_C
#pragma alloc_text (SG00006, db_enter)
#endif

#ifdef FILLNEW_C
#pragma alloc_text (SG00003, dt_fillnew)
#endif

#ifdef FINDCO_C
#pragma alloc_text (SG00003, dt_findco)
#endif

#ifdef FINDFM_C
#pragma alloc_text (SG00004, dt_findfm)
#endif

#ifdef FINDLM_C
#pragma alloc_text (SG00008, dt_findlm)
#endif

#ifdef FINDNM_C
#pragma alloc_text (SG00003, dt_findnm)
#endif

#ifdef FINDPM_C
#pragma alloc_text (SG00008, dt_findpm)
#endif

#ifdef GTSCM_C
#pragma alloc_text (SG00006, dt_gtscm)
#endif

#ifdef GTSCO_C
#pragma alloc_text (SG00026, dt_gtsco)
#endif

#ifdef GTSCR_C
#pragma alloc_text (SG00032, dt_gtscr)
#endif

#ifdef GTSCS_C
#pragma alloc_text (SG00006, dt_gtscs)
#endif

#ifdef INITENV_C
#pragma alloc_text (SG00021, initenv)
#endif

#ifdef INITIAL_C
#pragma alloc_text (SG00007, dt_initfile, dt_initialize)
#endif

#ifdef INITTAB_C
#pragma alloc_text (SG00017, initcurr)
#pragma alloc_text (SG00024, alloc_dict, inittab)
#endif

#ifdef INTERNAL_C
#pragma alloc_text (SG00029, dt_internals)
#endif

#ifdef ISMEMBER_C
#pragma alloc_text (SG00020, dt_ismember)
#endif

#ifdef ISOWNER_C
#pragma alloc_text (SG00008, dt_isowner)
#endif

#ifdef KEYBUILD_C
#pragma alloc_text (SG00014, dt_keybuild)
#endif

#ifdef KEYDEL_C
#pragma alloc_text (SG00033, dt_keydel)
#endif

#ifdef KEYEXIST_C
#pragma alloc_text (SG00008, dt_keyexist)
#endif

#ifdef KEYFCNS_C
#pragma alloc_text (SG00010, key_boundary, key_found, key_init, key_scan)
#pragma alloc_text (SG00012, key_close)
#pragma alloc_text (SG00014, expand, key_insert, key_reset, open_slots, split_node)
#pragma alloc_text (SG00014, split_root)
#pragma alloc_text (SG00016, close_slots, delete, key_locpos, node_search)
#pragma alloc_text (SG00018, keycmp)
#pragma alloc_text (SG00021, key_open)
#pragma alloc_text (SG00022, dt_curkey, dt_keyread, key_bldcom, key_cmpcpy, key_acpy)
#pragma alloc_text (SG00033, key_delete)
#endif

#ifdef KEYFIND_C
#pragma alloc_text (SG00002, dt_keyfind)
#pragma alloc_text (SG00022, chk_desc_key)
#endif

#ifdef KEYFRST_C
#pragma alloc_text (SG00010, dt_keyfrst)
#endif

#ifdef KEYLAST_C
#pragma alloc_text (SG00010, dt_keylast)
#endif

#ifdef KEYNEXT_C
#pragma alloc_text (SG00010, dt_keynext)
#endif

#ifdef KEYPREV_C
#pragma alloc_text (SG00010, dt_keyprev)
#endif

#ifdef KEYSTORE_C
#pragma alloc_text (SG00022, dt_keystore)
#endif

#ifdef LIBFCNS_C
#pragma alloc_text (SG00004, nrec_check)
#pragma alloc_text (SG00007, check_dba)
#pragma alloc_text (SG00010, nfld_check, rec_okay, SHORTcmp)
#pragma alloc_text (SG00018, ADDRcmp, ctblcmp, fldcmp)
#pragma alloc_text (SG00020, nset_check, null_dba)
#endif

#ifdef LIBMAIN_C
#pragma alloc_text (SG00026, LibMain, taskinit, WEP)
#endif

#ifdef LOCKCOMM_C
#pragma alloc_text (SG00030, dt_lockcomm)
#endif

#ifdef LOCKMGR_C
#pragma alloc_text (SG00026, dt_lockmgr)
#endif

#ifdef MAKENEW_C
#pragma alloc_text (SG00019, sk_free)
#pragma alloc_text (SG00023, dt_makenew, dt_setkey)
#endif

#ifdef MAPCHAR_C
#pragma alloc_text (SG00012, ctbl_free)
#pragma alloc_text (SG00013, _vbgets, ctbl_alloc, dt_mapchar, _vnextc)
#pragma alloc_text (SG00021, ctb_init)
#endif

#ifdef MEMBERS_C
#pragma alloc_text (SG00008, dt_members)
#endif

#ifdef NETWIN_C
#pragma alloc_text (SG00003, get_q)
#pragma alloc_text (SG00004, grant_lock, ins_tail, nw_send)
#pragma alloc_text (SG00005, nw_addnm, nw_delnm, nw_hangup, nw_sestat)
#pragma alloc_text (SG00005, nw_trans, xnet)
#pragma alloc_text (SG00007, l_trcommit)
#pragma alloc_text (SG00009, nw_rcvmsg)
#pragma alloc_text (SG00011, bit_set, conv, free_partial, ins_head, ins_Record)
#pragma alloc_text (SG00011, l_dbclose, l_dbopen, l_lock, lock_all, lmc_reply)
#pragma alloc_text (SG00016, l_trend)
#pragma alloc_text (SG00017, enter_user, LockDLLLM, nw_call, UnlockDLLLM)
#pragma alloc_text (SG00018, zeromem)
#pragma alloc_text (SG00022, bit_or)
#pragma alloc_text (SG00026, netbios_chk)
#pragma alloc_text (SG00028, l_free)
#endif

#ifdef OPENS_C
#pragma alloc_text (SG00007, open_b)
#pragma alloc_text (SG00015, commit_file)
#endif

#ifdef OPTIONS_C
#pragma alloc_text (SG00013, dt_off_opt, dt_on_opt)
#endif

#ifdef OVFCNS_C
#pragma alloc_text (SG00003, o_close)
#pragma alloc_text (SG00004, o_pzwrite)
#pragma alloc_text (SG00012, o_free, o_init)
#pragma alloc_text (SG00015, o_flush, o_pages)
#pragma alloc_text (SG00021, o_setup)
#pragma alloc_text (SG00028, o_fileinit)
#pragma alloc_text (SG00031, ixp_used, o_search, o_update, o_write, oaddr_of_ixp)
#pragma alloc_text (SG00031, set_ixpbit)
#endif

#ifdef PATHFCNS_C
#pragma alloc_text (SG00024, con_dbd, con_dbf, get_element, isabs, remdrv, remfile)
#pragma alloc_text (SG00026, dt_ctbpath)
#endif

#ifdef RECFCNS_C
#pragma alloc_text (SG00002, r_pmem, r_pset)
#pragma alloc_text (SG00008, r_gset)
#pragma alloc_text (SG00019, r_clropt)
#pragma alloc_text (SG00020, r_gmem, r_smem)
#pragma alloc_text (SG00022, r_setopt)
#pragma alloc_text (SG00025, r_gfld)
#pragma alloc_text (SG00028, r_delrec)
#pragma alloc_text (SG00033, r_chkfld, r_pfld, r_tstopt)
#endif

#ifdef RECFRST_C
#pragma alloc_text (SG00027, dt_recfrst)
#endif

#ifdef RECLAST_C
#pragma alloc_text (SG00027, dt_reclast)
#endif

#ifdef RECNEXT_C
#pragma alloc_text (SG00027, dt_recnext)
#endif

#ifdef RECOVER_C
#pragma alloc_text (SG00013, dt_recover)
#endif

#ifdef RECPREV_C
#pragma alloc_text (SG00027, dt_recprev)
#endif

#ifdef RECREAD_C
#pragma alloc_text (SG00002, dt_recread)
#endif

#ifdef RECSET_C
#pragma alloc_text (SG00006, dt_recset)
#endif

#ifdef RECSTAT_C
#pragma alloc_text (SG00007, dt_recstat)
#endif

#ifdef RECWRITE_C
#pragma alloc_text (SG00033, dt_recwrite)
#endif

#ifdef RENFILE_C
#pragma alloc_text (SG00021, renfiles)
#pragma alloc_text (SG00023, dt_renclean, dt_renfile)
#endif

#ifdef RWCURR_C
#pragma alloc_text (SG00012, dt_wrcurr)
#pragma alloc_text (SG00025, dt_rdcurr, dt_rerdcurr)
#endif

#ifdef SETDB_C
#pragma alloc_text (SG00025, dt_setdb)
#endif

#ifdef SETMM_C
#pragma alloc_text (SG00020, dt_setmm)
#endif

#ifdef SETMO_C
#pragma alloc_text (SG00020, dt_setmo)
#endif

#ifdef SETMR_C
#pragma alloc_text (SG00020, dt_setmr)
#endif

#ifdef SETOM_C
#pragma alloc_text (SG00020, dt_setom)
#endif

#ifdef SETOO_C
#pragma alloc_text (SG00020, dt_setoo)
#endif

#ifdef SETOR_C
#pragma alloc_text (SG00020, dt_setor)
#endif

#ifdef SETRM_C
#pragma alloc_text (SG00006, dt_setrm)
#endif

#ifdef SETRO_C
#pragma alloc_text (SG00007, dt_setro)
#endif

#ifdef SLEEP_C
#pragma alloc_text (SG00017, sleep)
#endif

#ifdef STSCM_C
#pragma alloc_text (SG00006, dt_stscm)
#endif

#ifdef STSCO_C
#pragma alloc_text (SG00006, dt_stsco)
#endif

#ifdef STSCR_C
#pragma alloc_text (SG00006, dt_stscr)
#endif

#ifdef STSCS_C
#pragma alloc_text (SG00006, dt_stscs)
#endif

#ifdef TAFFCNS_C
#pragma alloc_text (SG00013, rdm_taf_access, rdm_taf_add, rdm_taf_del, rdm_taf_open, rdm_taf_release)
#pragma alloc_text (SG00026, rdm_taf_close)
#endif

#ifdef TASK_C
#pragma alloc_text (SG00002, task_switch)
#pragma alloc_text (SG00026, dt_closetask, dt_opentask)
#endif

#ifdef TRLOG_C
#pragma alloc_text (SG00005, d_trmark)
#pragma alloc_text (SG00013, d_troff, d_tron)
#pragma alloc_text (SG00015, d_trlog)
#pragma alloc_text (SG00025, d_trbound)
#endif

#ifdef UTSCM_C
#pragma alloc_text (SG00004, dt_utscm)
#endif

#ifdef UTSCO_C
#pragma alloc_text (SG00020, dt_utsco)
#endif

#ifdef UTSCR_C
#pragma alloc_text (SG00027, dt_utscr)
#endif

#ifdef UTSCS_C
#pragma alloc_text (SG00020, dt_utscs)
#endif

#ifdef WINLFCNS_C
#pragma alloc_text (SG00004, freelock)
#pragma alloc_text (SG00011, adj_files, adj_users, bit_clr, close_file, close_user)
#pragma alloc_text (SG00011, free_pending, free_q, is_bit_set, is_map_zero, map_zero)
#pragma alloc_text (SG00017, streq)
#endif

#endif                                 /* */
/* vpp @win.vpp VISTASEG.h (Thu May 21 10:47:04 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

