/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tz__oprs.h -  Zeidon application operation prototypes
// DESCRIPTION:   This C file contains global and LOD operation prototypes
//                for the Zeidon application.
//
// **************************************************************************
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// **************************************************************************
//
// AUTHOR:        Doug Sorensen
// DATE:          1995/08/05
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.8E   1995/08/05
//
// HISTORY:
//
//    1995/08/31: Created by DGC.
//
//    2001.04.08  BL
//       Added defines for CPLR Enqueue Flags
//
//    2000.08.16  BL
//       Added defines for save as with Operation code
//
//    2000.08.14  RG
//      Added defines for changing path, reportname and LPLR-Name
//      in Siron Reports in functions migrate, external LPRL and SAVE AS
//    2000.07.28  BL
//      Added define for move and Copy Operation with Operation Code
//
//    2000.07.20  BL
//      Added define for Editor
//
//    2000.06.29  BL
//      Added defines for Repository LPLR's
//
//    1999.04.19  RS
//      Commentlines required for Rose.
//
/////////////////////////////////////////////////////////////////////////////


// This file is currently included from tzlodopr.h and generated .C files.

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose

#ifndef TZ__OPRS_H
#define TZ__OPRS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define zCURRENT_OI         0
#define zFORCE_RELOAD       1
#define zSYS_CURRENT_OI     2
#define zSYS_FORCE_RELOAD   3

#define zASCENDING 0
#define zDESCENDING 1

#define  zREFER_HDR_META       2000
#define  zREFER_SRC_META       2001
#define  zREFER_GO_META        2002
#define  zREFER_DOMAIN_META    2003
#define  zREFER_ERD_META       2004
#define  zREFER_SA_META        2005
#define  zREFER_DTE_META       2006
#define  zREFER_LOD_META       2007
#define  zREFER_POD_META       2008
#define  zREFER_VOR_META       2009
#define  zREFER_PENV_META      2010
#define  zREFER_DIALOG_META    2011
#define  zREFER_UIS_META       2012
#define  zREFER_DOMAINGRP_META 2013
#define  zREFER_GOPGRP_META    2014
#define  zREFER_REPORT_META    2015
#define  zREFER_XSLT_META      2016
#define  zREFER_MAX_META       2016

/* defines for LPLR Types */
#define  zSOURCE_HDR_META        0
#define  zSOURCE_SRC_META        1
#define  zSOURCE_GO_META         2
#define  zSOURCE_DOMAIN_META     3
#define  zSOURCE_ERD_META        4
#define  zSOURCE_SA_META         5
#define  zSOURCE_DTE_META        6
#define  zSOURCE_LOD_META        7
#define  zSOURCE_POD_META        8
#define  zSOURCE_VOR_META        9
#define  zSOURCE_PENV_META      10
#define  zSOURCE_DIALOG_META    11
#define  zSOURCE_UIS_META       12
#define  zSOURCE_DOMAINGRP_META 13
#define  zSOURCE_GOPGRP_META    14
#define  zSOURCE_REPORT_META    15
#define  zSOURCE_XSLT_META      16
#define  zSOURCE_MAX_META       16

/* defines for CPLR Enqueue Flags */
#define  zCPLR_DEQUEUE                0
#define  zCPLR_ENQUEUE_FOR_REFRESH    1
#define  zCPLR_ENQUEUE_FOR_CHECK_IN   2
#define  zCPLR_ENQUEUE_FOR_CHECK_OUT  3
#define  zCPLR_ENQUEUE_BY_SYSADM      5

/* defines for Repository LPLR's */
#define zNoActionAfterSaveAs        0
#define zNewComponentAfterSaveAs    1
#define zOpenComponentAfterSaveAs   2
#define zSwitchLPLRAfterSaveAs      3
#define zExitToolAfterSaveAs        4

/* defines for Editor */
#define zSHOW_OPERATION      0
#define zDELETE_OPERATION    1
#define zMOVE_OPERATION      2
#define zCOPY_OPERATION      3
#define zRENAME_OPERATION    4
#define zNORENAME_OPERATION  5

/* defines for ChangeRepFileContents */
#define zCHANGE_REP_MIGRATE_EXTERNAL 0
#define zCHANGE_REP_SAVEAS           1

struct CMOD_Struct
{
   zPCHAR szOD;
   zPCHAR szOD_ROOT;
   zPCHAR szOD_NAME;
   zPCHAR szOD_EXT;
   zPCHAR szOD_PREFIX;
};

#ifdef __cplusplus
}
#endif

#endif   // #ifndef TZ__OPRS_H
