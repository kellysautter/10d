/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzophdro.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: OP00501
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
/*
CHANGE LOG

2000.10.24  SR  Z2000 Size of path variables
   Modified size of szDir in function oTZOPHDRO_DeriveFileSpec and
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
*/

#define  KZMETA_INCL
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
oTZOPHDRO_DeriveFileSpec( zVIEW            vMeta,
                          LPVIEWENTITY     lpViewEntity,
                          LPVIEWATTRIB     lpViewAttrib,
                          zSHORT           nMsg )
{
   zSHORT  nRC;
   zVIEW   vTaskLPLR;
   zCHAR   szDir[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR  szTemp;
   zLONG   lLth;

   nRC = GetViewByName( &vTaskLPLR, "TaskLPLR", vMeta, zLEVEL_TASK );
   if ( nRC <= 0 )
   {
      MessageSend( vMeta, "OP005", "Operations",
                   "Cannot find TaskLPLR",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( zCALL_ERROR );
   }

   GetStringFromAttribute( szDir, sizeof( szDir ), vTaskLPLR, "LPLR", "PgmSrcDir" );
   if ( szDir[ 0 ] == 0 )
      GetStringFromAttribute( szDir, sizeof( szDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );

   lLth = zstrlen( szDir );
   if ( szDir[ lLth ] != '\\' )
      strcat_s( szDir, sizeof( szDir ), "\\" );

   lLth = zstrlen( szDir );
   if ( szDir[ lLth ] != '\\' )
      strcat_s( szDir, sizeof( szDir ), "\\" );

   GetAddrForAttribute( &szTemp, vMeta, "HeaderFile", "Name" );
   strcat_s(szDir, sizeof( szDir ), szTemp );
   strcat_s(szDir, sizeof( szDir ), "." );
   GetAddrForAttribute( &szTemp, vMeta, "HeaderFile", "Extension" );
   if ( szTemp[ 0 ] )
      strcat_s(szDir, sizeof( szDir ), szTemp );
   else
      strcat_s(szDir, sizeof( szDir ), "H" );

   StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szDir, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPHDRO_DeriveName( zVIEW        vMeta,
                      LPVIEWENTITY lpViewEntity,
                      LPVIEWATTRIB lpViewAttrib,
                      zSHORT       nMsg )
{
   zLONG   lHDR_ZKey;
   zVIEW   vHDR_Ref;
   zCHAR   szHDR_Name[ 33 ];
   zSHORT  nRC;

   GetIntegerFromAttribute( &lHDR_ZKey, vMeta,
                            lpViewEntity->szName, "ZKey" );
   nRC = ActivateMetaOI_ByZKey( vMeta, &vHDR_Ref, 0, zREFER_HDR_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                lHDR_ZKey, zCURRENT_OI );
   if ( nRC == 1 )
      GetStringFromAttribute( szHDR_Name, sizeof( szHDR_Name ), vHDR_Ref, "HeaderFile", "Name" );
   else
      strcpy_s( szHDR_Name, sizeof( szHDR_Name ), "" );

   DropMetaOI( vMeta, vHDR_Ref );
   StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szHDR_Name, 0 );
   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
