/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzmsdilo.c - Zeidon application operations
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
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
ofnTZMSDILO_CopyDILToMSG( zVIEW vDIL_Msg,
                          zVIEW vMSG );

// returns 0 - not unique
//         1 - unique
zOPER_EXPORT zSHORT OPERATION
oTZMSDILO_UniqueMessageName( zVIEW vDIL_Msg,
                             zVIEW vCM_List )
{
   zCHAR    szMessageName[ 33 ];
   zSHORT   nRC;

   GetStringFromAttribute( szMessageName, zsizeof( szMessageName ), vDIL_Msg,
                           "DIL_Msg", "Name" );
   nRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                       szMessageName, "" );
   if ( nRC != zCURSOR_SET )
      return( 1 );

   if ( CompareAttributeToAttribute( vCM_List, "W_MetaDef", "CPLR_ZKey",
                                     vDIL_Msg, "DIL_Msg", "ZKey" ) != 0 )
   {
      return( 0 );
   }

   nRC = SetCursorNextEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szMessageName, "" );
   if ( nRC != zCURSOR_SET )
      return( 1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZMSDILO_RemoveMessageFromMSG( zVIEW vDIL_Msg )
{
   zVIEW    vMSG;
   zCHAR    szMessageName[ 33 ];
   zLONG    lZKey;
   zSHORT   nRC;

   nRC = oTZMSDGPO_GetViewForMSG( &vMSG, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetStringFromAttribute( szMessageName, zsizeof( szMessageName ), vDIL_Msg, "DIL_Msg", "Name" );
   if ( SetCursorFirstEntityByString( vMSG, "DIL_Msg", "Name",
                                      szMessageName, "" ) != zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vDIL_Msg, "DIL_Msg", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vMSG, "DIL_Msg", "ZKey",
                                          lZKey, "" ) != zCURSOR_SET )
      {
         return( -1 );
      }
   }

   nRC = DeleteEntity( vMSG, "DIL_Msg", zREPOS_NONE );

   nRC = oTZMSDGPO_CommitMSG( vMSG );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZMSDILO_ReplaceOrAddDILToMSG( zVIEW vDIL_Msg )
{
   zVIEW    vMSG;
   zCHAR    szMessageName[ 33 ];
   zLONG    lZKey;
   zSHORT   nRC;

   nRC = oTZMSDGPO_GetViewForMSG( &vMSG, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vDIL_Msg, "DIL_Msg", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vMSG, "DIL_Msg", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      DeleteEntity( vMSG, "DIL_Msg", zREPOS_PREV );
   }

   GetStringFromAttribute( szMessageName, zsizeof( szMessageName ), vDIL_Msg, "DIL_Msg", "Name" );
   if ( SetCursorFirstEntityByString( vMSG, "DIL_Msg", "Name",
                                      szMessageName, "" ) == zCURSOR_SET )
   {
      DeleteEntity( vMSG, "DIL_Msg", zREPOS_PREV );
   }

   ofnTZMSDILO_CopyDILToMSG( vDIL_Msg, vMSG );

   nRC = oTZMSDGPO_CommitMSG( vMSG );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZMSDILO_AddDILToMSG( zVIEW vDIL_Msg )
{
   zVIEW    vMSG;
   zCHAR    szMessageName[ 33 ];
   zLONG    lZKey;
   zSHORT   nRC;

   nRC = oTZMSDGPO_GetViewForMSG( &vMSG, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vDIL_Msg, "DIL_Msg", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vMSG, "DIL_Msg", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   GetStringFromAttribute( szMessageName, zsizeof( szMessageName ), vDIL_Msg, "DIL_Msg", "Name" );
   if ( SetCursorFirstEntityByString( vMSG, "DIL_Msg", "Name",
                                      szMessageName, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   ofnTZMSDILO_CopyDILToMSG( vDIL_Msg, vMSG );

   nRC = oTZMSDGPO_CommitMSG( vMSG );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZMSDILO_CopyDILToMSG( zVIEW vDIL_Msg,
                          zVIEW vMSG )
{
   zSHORT   nRC;

   CreateEntity( vMSG, "DIL_Msg", zPOS_FIRST );
   SetMatchingAttributesByName( vMSG, "DIL_Msg",
                                vDIL_Msg, "DIL_Msg", zSET_ALL );

   nRC = zgSortEntityWithinParent( zASCENDING, vMSG, "DIL_Msg", "Name", "" );
   return( nRC );
}

///////////////////////////// end of file  //////////////////////////////////
