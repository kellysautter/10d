/*----------------------------------------------------------------------
  lockcomm.h: LockManger Communication header file, included by vista.h

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef LOCKCOMM_H
#define LOCKCOMM_H


/* The following block of #ifdef allow the developer to #define one of
 * the LMC_ constants when compiling their source to prevent the linker
 * from adding other unused lmc modules.  For example defining LMC_GENERAL
 * redefines d_lockcomm() to lmcg_lockcomm(), the GENERAL specific version
 * of d_lockcomm() located in lmcg.c.  As a result lockcomm.c is not linked
 * in and neither are the rest of the LMC modules.  When no LMC constant is
 * defined, lockcomm.c is linked in which references all the LMC modules,
 * thus allowing the program to dynamicaly change the LMC layer.
 */


/* multi-user version */
/* if these enums change, correct the RDM_SU and AIX blocks above!! */

enum {
   LMC_NONE_NUM = 0,
   LMC_USER_NUM,
   LMC_NETBIOS_NUM,
   LMC_INTERNAL_NUM,
   LMC_GENERAL_NUM,
   LMC_SPX_NUM,
   LMC_SEMAPHORE_NUM,
   LMC_IP_NUM,
   LMC_TCP_NUM,
   LMC_VMS_NUM,
   LMC_QNX_NUM
};

/* Lock Manager Communication Interface */
#ifdef MULTI_TASK

#define d_lockcomm(a)       dt_lockcomm(a CURRTASK_PARM)
#define lmcusr_lockcomm(a)  lmcusrt_lockcomm(a CURRTASK_PARM)
#define lmcb_lockcomm(a)    lmcbt_lockcomm(a CURRTASK_PARM)
#define lmci_lockcomm(a)    lmcit_lockcomm(a CURRTASK_PARM)
#define lmcg_lockcomm(a)    lmcgt_lockcomm(a CURRTASK_PARM)
#define lmcs_lockcomm(a)    lmcst_lockcomm(a CURRTASK_PARM)
#define sem_lockcomm(a)     semt_lockcomm(a CURRTASK_PARM)
#define ip_lockcomm(a)      ipt_lockcomm(a CURRTASK_PARM)
#define tcp_lockcomm(a)     tcpt_lockcomm(a CURRTASK_PARM)
#define lmcv_lockcomm(a)    lmcvt_lockcomm(a CURRTASK_PARM)
#define lmcq_lockcomm(a)    lmcqt_lockcomm(a CURRTASK_PARM)


int EXTERNAL_FIXED dt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED lmcusrt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED lmcbt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED lmcit_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED lmcgt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED lmcst_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED semt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED ipt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED tcpt_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED lmcvt_i_lockcomm(P1(int) TASK_Di);
int EXTERNAL_FIXED lmcqt_lockcomm(P1(int) TASK_Di);

#else /* MULTI_TASK */

int EXTERNAL_FIXED d_lockcomm(P1(int));
int EXTERNAL_FIXED lmcusr_lockcomm(P1(int));
int EXTERNAL_FIXED lmcb_lockcomm(P1(int));
int EXTERNAL_FIXED lmci_lockcomm(P1(int));
int EXTERNAL_FIXED lmcg_lockcomm(P1(int));
int EXTERNAL_FIXED lmcs_lockcomm(P1(int));
int EXTERNAL_FIXED sem_lockcomm(P1(int));
int EXTERNAL_FIXED ip_lockcomm(P1(int));
int EXTERNAL_FIXED tcp_lockcomm(P1(int));
int EXTERNAL_FIXED lmcv_lockcomm(P1(int));
int EXTERNAL_FIXED lmcq_lockcomm(P1(int));

#endif /* MULTI_TASK */

#define LMC_NONE     LMC_NONE_NUM

#ifdef  LMC_USER
#undef  LMC_USER
#define LMC_USER     LMC_USER_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   lmcusr_lockcomm(a)
#define dt_lockcomm     lmcusrt_lockcomm

#else

#ifdef  LMC_INTERNAL
#undef  LMC_INTERNAL
#define LMC_INTERNAL LMC_INTERNAL_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   lmci_lockcomm(a)
#define dt_lockcomm     lmcit_lockcomm

#else

#ifdef  LMC_GENERAL
#undef  LMC_GENERAL
#define LMC_GENERAL  LMC_GENERAL_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   lmcg_lockcomm(a)
#define dt_lockcomm     lmcgt_lockcomm

#else

#ifdef  LMC_NETBIOS
#undef  LMC_NETBIOS
#define LMC_NETBIOS  LMC_NETBIOS_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   lmcb_lockcomm(a)
#define dt_lockcomm     lmcbt_lockcomm

#else

#ifdef  LMC_SPX
#undef  LMC_SPX
#define LMC_SPX      LMC_SPX_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   lmcs_lockcomm(a)
#define dt_lockcomm     lmcst_lockcomm

#else

#ifdef  LMC_TCP
#undef  LMC_TCP
#define LMC_TCP      LMC_TCP_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   tcp_lockcomm(a)
#define dt_lockcomm     tcpt_lockcomm

#else

#ifdef  LMC_IP
#undef  LMC_IP
#define LMC_IP       LMC_IP_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   ip_lockcomm(a)
#define dt_lockcomm     ipt_lockcomm

#else

#ifdef  LMC_SEMAPHORE
#undef  LMC_SEMAPHORE
#define LMC_SEMAPHORE LMC_SEMAPHORE_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   sem_lockcomm(a)
#define dt_lockcomm     semt_lockcomm

#else

#ifdef  LMC_VMS
#undef  LMC_VMS
#define LMC_VMS      LMC_VMS_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   lmcv_lockcomm(a)
#define dt_lockcomm     lmcvt_lockcomm


#else

#ifdef  LMC_QNX
#undef  LMC_QNX
#define LMC_QNX      LMC_QNX_NUM
#undef  d_lockcomm
#define d_lockcomm(a)   lmcq_lockcomm(a)
#define dt_lockcomm     lmcqt_lockcomm

#else  /* LMC_???? not defined */

#define LMC_USER              LMC_USER_NUM
#define LMC_INTERNAL          LMC_INTERNAL_NUM
#define LMC_GENERAL           LMC_GENERAL_NUM
#define LMC_NETBIOS           LMC_NETBIOS_NUM
#define LMC_SPX               LMC_SPX_NUM
#define LMC_TCP               LMC_TCP_NUM
#define LMC_IP                LMC_IP_NUM
#define LMC_SEMAPHORE         LMC_SEMAPHORE_NUM
#define LMC_VMS               LMC_VMS_NUM
#define LMC_QNX               LMC_QNX_NUM

#endif /* LMC_QNX          */
#endif /* LMC_VMS          */
#endif /* LMC_SEMAPHORE    */
#endif /* LMC_IP           */
#endif /* LMC_TCP          */
#endif /* LMC_SPX          */
#endif /* LMC_GENERAL      */
#endif /* LMC_INTERNAL     */
#endif /* LMC_NETBIOS      */
#endif /* LMC_USER         */

#endif /* LOCKCOMM_H */

/* vpp @dos.vpp lockcomm.h (Mon Jul 13 12:55:52 1992)
 *
 *     -nRDM_SU -dMSDOS -dDB_SHARE -nUNIX -nAIX -nRISC -nSUN -nSCO -nWIN3
 *     -nWINDOWS -nVMS -nOS2 -nOS2_MT -nREOPEN_FILES -nFAR_ALLOC -nARCHIVING
 *     -nDB_DLL -nBSD -nMEMLOCK -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP
 *     -nSINGLE_USER -nNO_TRANS -nVer330 -n_WINDOWS -nQNX -nQNX2 -nQNX4
 *     -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY -nTRUENAME -nLAT
 */

