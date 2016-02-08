/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzopsigo.c - Zeidon application operations
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
// LAST MESSAGE ID: OP01101
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#define  KZMETA_INCL
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
ofnTZOPSIGO_CopyOperToXGO( zVIEW vSubtask,
                           zVIEW vOperation,
                           zVIEW vXGO );
zOPER_EXPORT zSHORT OPERATION
ofnTZOPSIGO_CopyHdrToXGO( zVIEW vSubtask,
                          zVIEW vHdrFile,
                          zVIEW vXGO );

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_RemoveOperFromXGO( zVIEW  vSubtask,
                             zVIEW  vOperation )
{
   zVIEW    vXGO;
   zSHORT   nRC;
   zCHAR    szOperationName[ 33 ];
   zLONG    lZKey;

   nRC = oTZOPGRPO_GetViewForXGO( vSubtask, &vXGO, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vOperation, "Operation", "Name" );
   if ( SetCursorFirstEntityByString( vXGO, "Operation", "Name",
                                      szOperationName, "" ) != zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vOperation, "Operation", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vXGO, "Operation", "ZKey",
                                          lZKey, "" ) == zCURSOR_SET )
      {
         return( -1 );
      }
   }
   nRC = ExcludeEntity( vXGO, "Operation", zREPOS_NONE );

   nRC = oTZOPGRPO_CommitXGO( vXGO );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_ReplaceOrAddOperToXGO( zVIEW  vSubtask,
                                 zVIEW  vOperation )
{
   zVIEW    vXGO;
   zSHORT   nRC;
   zCHAR    szOperationName[ 33 ];
   zLONG    lZKey;

   nRC = oTZOPGRPO_GetViewForXGO( vSubtask, &vXGO, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vOperation, "Operation", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vXGO, "Operation", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      nRC = ExcludeEntity( vXGO, "Operation", zREPOS_PREV );
   }

   GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vOperation, "Operation", "Name" );
   if ( SetCursorFirstEntityByString( vXGO, "Operation", "Name",
                                      szOperationName, "" ) == zCURSOR_SET )
   {
      nRC = ExcludeEntity( vXGO, "Operation", zREPOS_PREV );
   }

   ofnTZOPSIGO_CopyOperToXGO( vSubtask, vOperation, vXGO );

   nRC = oTZOPGRPO_CommitXGO( vXGO );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_AddOperToXGO( zVIEW  vSubtask,
                        zVIEW  vOperation )
{
   zVIEW    vXGO;
   zSHORT   nRC;
   zCHAR    szOperationName[ 33 ];
   zLONG    lZKey;

   nRC = oTZOPGRPO_GetViewForXGO( vSubtask, &vXGO, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vOperation, "Operation", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vXGO, "Operation", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vOperation, "Operation", "Name" );
   if ( SetCursorFirstEntityByString( vXGO, "Operation", "Name",
                                      szOperationName, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   ofnTZOPSIGO_CopyOperToXGO( vSubtask, vOperation, vXGO );

   nRC = oTZOPGRPO_CommitXGO( vXGO );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZOPSIGO_CopyOperToXGO( zVIEW vSubtask,
                           zVIEW vOperation,
                           zVIEW vXGO )
{
   zVIEW    vTmpXGO;
   zVIEW    vTmpHdr;
   zSHORT   nRC;
   zLONG    lZKey;
   zCHAR    sz[ 128 ];

   CreateViewFromViewForTask( &vTmpXGO, vXGO, 0 );
   nRC = CreateEntity( vXGO, "Operation", zPOS_AFTER );
   nRC = SetMatchingAttributesByName( vXGO, "Operation",
                                      vOperation, "Operation", zSET_ALL );

// copy complete structure of the Operation Object to the XGO Group.
   nRC = SetCursorFirstEntity( vOperation, "Parameter", "" );
   while ( nRC == zCURSOR_SET )
   {
      nRC = CreateEntity( vXGO, "Parameter", zPOS_AFTER );
      nRC = SetMatchingAttributesByName( vXGO, "Parameter",
                                         vOperation, "Parameter", zSET_ALL );

      nRC = SetCursorNextEntity( vOperation, "Parameter", "" );
   }
   nRC = SetCursorFirstEntity( vOperation, "SourceFile", "" );
   while ( nRC == zCURSOR_SET )
   {
      nRC = CreateEntity( vXGO, "SourceFile", zPOS_AFTER );
      nRC = SetMatchingAttributesByName( vXGO, "SourceFile",
                                         vOperation, "SourceFile", zSET_ALL );

      nRC = SetCursorNextEntity( vOperation, "SourceFile", "" );
   }
   nRC = SetCursorFirstEntity( vOperation, "HeaderFile", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vOperation, "HeaderFile", "ZKey" );
      ActivateMetaOI_ByZKey( vSubtask, &vTmpHdr, 0, zSOURCE_HDR_META,
                             zSINGLE | zLEVEL_APPLICATION,
                             lZKey, zCURRENT_OI );
      if ( vTmpHdr != 0 )
      {
         if ( ObjectInstanceUpdatedFromFile( vTmpHdr ) == 1 )
         {
            strcpy_s( sz, sizeof( sz ), "Do you wish to save Header '" );
            GetStringFromAttribute( sz + zstrlen( sz ), sizeof( sz ) - zstrlen( sz ),
                                    vTmpHdr, "HeaderFile", "Name" );
            strcat_s( sz, sizeof( sz ), "'?" );
            nRC = MessagePrompt( vOperation, "OP01101", "Operations", sz,
                                 zBEEP, zBUTTONS_YESNOCANCEL, zRESPONSE_NO,  0 );
            if ( nRC == zRESPONSE_YES )
            {
               AssignZKeyToMetaOI( vSubtask, vTmpHdr, zSOURCE_HDR_META, 0 );
               CommitMetaOI( vSubtask, vTmpHdr, zSOURCE_HDR_META );
            }
         }
      }
      nRC = SetCursorFirstEntityByInteger( vTmpXGO, "HeaderFile",
                                           "ZKey", lZKey, "" );
      if ( nRC == zCURSOR_SET )
         nRC = IncludeSubobjectFromSubobject( vXGO, "HeaderFile",
                                              vTmpXGO, "HeaderFile",
                                              zPOS_AFTER );

      nRC = SetCursorNextEntity( vOperation, "HeaderFile", "" );
   }

   nRC = zgSortEntityWithinParent( zASCENDING, vXGO,
                                   "Operation", "Name", "" );

   DropView( vTmpXGO );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_UniqueHdrName( zVIEW vHdrFile, zVIEW vCM_List )
{
// returns 0 - not unique
//         1 - unique

   zSHORT   nRC;
   zCHAR    szHdrFileName[ 33 ];

   nRC = GetStringFromAttribute( szHdrFileName, sizeof( szHdrFileName ), vHdrFile,
                                 "HeaderFile", "Name" );
   nRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                       szHdrFileName, "" );
   if ( nRC != zCURSOR_SET )
      return( 1 );

   if ( CompareAttributeToAttribute( vCM_List, "W_MetaDef", "CPLR_ZKey",
                                     vHdrFile, "HeaderFile", "ZKey" ) != 0 )
   {
      return( 0 );
   }

   nRC = SetCursorNextEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szHdrFileName, "" );
   if ( nRC != zCURSOR_SET )
      return( 1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_RemoveHdrFromXGO( zVIEW vSubtask,
                            zVIEW vHdrFile )
{
   zVIEW    vXGO;
   zSHORT   nRC;
   zCHAR    szHdrFileName[ 33 ];
   zLONG    lZKey;

   nRC = oTZOPGRPO_GetViewForXGO( vSubtask, &vXGO, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetStringFromAttribute( szHdrFileName, sizeof( szHdrFileName ), vHdrFile, "HeaderFile", "Name" );
   if ( SetCursorFirstEntityByString( vXGO, "HeaderFile", "Name",
                                      szHdrFileName, "" ) != zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vHdrFile, "HeaderFile", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vXGO, "HeaderFile", "ZKey",
                                          lZKey, "" ) == zCURSOR_SET )
      {
         return( -1 );
      }
   }

   nRC = DeleteEntity( vXGO, "HeaderFile", zREPOS_NONE );
   nRC = oTZOPGRPO_CommitXGO( vXGO );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_ReplaceOrAddHdrToXGO( zVIEW vSubtask,
                                zVIEW vHdrFile )
{
   zVIEW    vXGO;
   zSHORT   nRC;
   zCHAR    szHdrFileName[ 33 ];
   zLONG    lZKey;

   nRC = oTZOPGRPO_GetViewForXGO( vSubtask, &vXGO, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vHdrFile, "HeaderFile", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vXGO, "HeaderFile", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      nRC = DeleteEntity( vXGO, "HeaderFile", zREPOS_PREV );
   }

   GetStringFromAttribute( szHdrFileName, sizeof( szHdrFileName ), vHdrFile, "HeaderFile", "Name" );
   if ( SetCursorFirstEntityByString( vXGO, "HeaderFile", "Name",
                                      szHdrFileName, "" ) == zCURSOR_SET )
   {
      nRC = DeleteEntity( vXGO, "HeaderFile", zREPOS_PREV );
   }

   ofnTZOPSIGO_CopyHdrToXGO( vSubtask, vHdrFile, vXGO );
   nRC = oTZOPGRPO_CommitXGO( vXGO );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_AddHdrToXGO( zVIEW vSubtask,
                       zVIEW vHdrFile )
{
   zVIEW    vXGO;
   zSHORT   nRC;
   zCHAR    szHdrFileName[ 33 ];
   zLONG    lZKey;

   nRC = oTZOPGRPO_GetViewForXGO( vSubtask, &vXGO, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vHdrFile, "HeaderFile", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vXGO, "HeaderFile", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   GetStringFromAttribute( szHdrFileName, sizeof( szHdrFileName ), vHdrFile, "HeaderFile", "Name" );
   if ( SetCursorFirstEntityByString( vXGO, "HeaderFile", "Name",
                                      szHdrFileName, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   ofnTZOPSIGO_CopyHdrToXGO( vSubtask, vHdrFile, vXGO );

   nRC = oTZOPGRPO_CommitXGO( vXGO );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZOPSIGO_CopyHdrToXGO( zVIEW vSubtask,
                          zVIEW vHdrFile,
                          zVIEW vXGO )
{
   zVIEW    vTmpXGO;
// zVIEW    vTm2XGO;
   zVIEW    vSysXGO;
   zSHORT   nRC;
   zLONG    lZKey;

   nRC = oTZOPGRPO_GetViewForXGO( vSubtask, &vSysXGO, zSYS_CURRENT_OI );
   CreateViewFromViewForTask( &vTmpXGO, vXGO, 0 );
   nRC = CreateEntity( vXGO, "HeaderFile", zPOS_AFTER );
   nRC = SetMatchingAttributesByName( vXGO, "HeaderFile",
                                      vHdrFile, "HeaderFile", zSET_ALL );

// copy complete structure of the HeaderFile Object to the XGO Group.
   nRC = SetCursorFirstEntity( vHdrFile, "DefinedItem", "" );
   while ( nRC == zCURSOR_SET )
   {
      nRC = CreateEntity( vXGO, "DefinedItem", zPOS_AFTER );
      nRC = SetMatchingAttributesByName( vXGO, "DefinedItem",
                                         vHdrFile, "DefinedItem", zSET_ALL );

      nRC = SetCursorNextEntity( vHdrFile, "DefinedItem", "" );
   }

   nRC = SetCursorFirstEntity( vHdrFile, "OperationPrototype", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vHdrFile,
                               "OperationPrototype", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vTmpXGO, "Operation",
                                           "ZKey", lZKey, "" );
      if ( nRC == zCURSOR_SET )
         nRC = IncludeSubobjectFromSubobject( vXGO, "OperationPrototype",
                                              vTmpXGO, "Operation",
                                              zPOS_AFTER );
      else
      {
         nRC = SetCursorFirstEntityByInteger( vSysXGO, "Operation",
                                             "ZKey", lZKey, "" );
         if ( nRC == zCURSOR_SET )
            nRC = IncludeSubobjectFromSubobject( vXGO, "OperationPrototype",
                                                 vSysXGO, "Operation",
                                                 zPOS_AFTER );
      }

      nRC = SetCursorNextEntity( vHdrFile, "OperationPrototype", "" );
   }

   nRC = zgSortEntityWithinParent( zASCENDING, vXGO,
                                   "OperationPrototype", "Name", "" );
   nRC = zgSortEntityWithinParent( zASCENDING, vXGO,
                                   "HeaderFile", "Name", "" );

   DropView( vTmpXGO );
   DropView( vSysXGO );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_DeriveSourceName( zVIEW        vMeta,
                            LPVIEWENTITY lpViewEntity,
                            LPVIEWATTRIB lpViewAttrib,
                            zSHORT       nMsg )
{
   zSHORT  nRC;
   zLONG   lSRC_ZKey;
   zCHAR   szSRC_Name[ 34 ];
   zVIEW   vTaskLPLR;

   szSRC_Name[ 0 ] = 0;
   nRC = GetViewByName( &vTaskLPLR, "TaskLPLR", vMeta, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      GetIntegerFromAttribute( &lSRC_ZKey, vMeta,
                               lpViewEntity->szName, "ZKey" );
      if ( SetCursorFirstEntityByInteger( vTaskLPLR, "W_MetaDef",
                                          "CPLR_ZKey", lSRC_ZKey,
                                          "LPLR" ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szSRC_Name, sizeof( szSRC_Name ), vTaskLPLR,
                                 "W_MetaDef", "Name" );
      }
   }

   StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szSRC_Name, 0 );
   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
