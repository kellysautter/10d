/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tztempmo.c - Zeidon application operations
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
// LAST MESSAGE ID: TE00301
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
oTZTEMPMO_Create( zPVIEW   vMPM,
                  zVIEW    vEMD_In,
                  zVIEW    vTE_In )
{
   zVIEW    vEMD;
   zVIEW    vTE;
   zLONG    lZKey;
   zSHORT   RESULT;

   CreateViewFromViewForTask( &vEMD, vEMD_In, 0 );
   CreateViewFromViewForTask( &vTE, vTE_In, 0 );
   RESULT = ActivateEmptyObjectInstance( vMPM, "TZTEMPMO", vEMD_In, zSINGLE );
   if ( RESULT < 0 )
   {
      MessageSend( vTE_In, "TE00301", "Technical Environment",
                   "The MMP object NOT created.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
   {
      SetNameForView( *vMPM, "TZTEMPMO", vEMD_In, zLEVEL_TASK );
   }

   CreateEntity( *vMPM, "ModelPhysicalMapping", zPOS_AFTER );
   RESULT = SetCursorFirstEntity( vEMD, "ER_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      IncludeSubobjectFromSubobject( *vMPM, "ER_Entity", vEMD, "ER_Entity", zPOS_AFTER );
      GetIntegerFromAttribute( &lZKey, vEMD, "ER_Entity", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vTE, "ER_Entity", "ZKey",
                                              lZKey, "TE_DB_Environ" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         IncludeSubobjectFromSubobject( *vMPM, "TE_TablRec_Entity", vTE, "TE_TablRec", zPOS_AFTER );
         RESULT = SetCursorNextEntityByInteger( vTE, "ER_Entity", "ZKey", lZKey, "TE_DB_Environ" );
      }

      RESULT = SetCursorFirstEntity( *vMPM, "ER_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         GetIntegerFromAttribute( &lZKey, *vMPM, "ER_Attribute", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vTE, "ER_Attribute", "ZKey", lZKey, "TE_DB_Environ" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
            IncludeSubobjectFromSubobject( *vMPM, "TE_FieldData", vTE, "TE_FieldDataRel", zPOS_AFTER );
            RESULT = SetCursorNextEntityByInteger( vTE, "ER_Attribute", "ZKey", lZKey, "TE_DB_Environ" );
         }

         RESULT = SetCursorNextEntity( *vMPM, "ER_Attribute", "" );
      }

      RESULT = SetCursorFirstEntity( *vMPM, "ER_RelLink", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         SetCursorFirstEntityByEntityCsr( vEMD, "ER_RelLink", *vMPM, "ER_RelLink", "EntpER_Model" );
         IncludeSubobjectFromSubobject( *vMPM, "ER_RelLink_O", vEMD, "ER_RelLink_Other", zPOS_AFTER );
         GetIntegerFromAttribute( &lZKey, *vMPM, "ER_RelLink_O", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vTE, "ER_RelLink", "ZKey", lZKey, "TE_DB_Environ" );
         while ( RESULT > zCURSOR_UNCHANGED )
         {
            IncludeSubobjectFromSubobject( *vMPM, "TE_FieldRel", vTE, "TE_FieldDataRel", zPOS_AFTER );
            RESULT = SetCursorNextEntityByInteger( vTE, "ER_RelLink", "ZKey", lZKey, "TE_DB_Environ" );
         }

         RESULT = SetCursorNextEntity( *vMPM, "ER_RelLink", "" );
      }

      RESULT = SetCursorNextEntity( vEMD, "ER_Entity", "" );
   }

   RESULT = SetCursorFirstEntity( vEMD, "ER_RelType", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      IncludeSubobjectFromSubobject( *vMPM, "ER_RelType", vEMD, "ER_RelType", zPOS_AFTER );
      GetIntegerFromAttribute( &lZKey, vEMD, "ER_RelType", "ZKey" );
      RESULT = SetCursorFirstEntityByInteger( vTE, "ER_RelType", "ZKey", lZKey, "TE_DB_Environ" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         IncludeSubobjectFromSubobject( *vMPM, "TE_TablRec_Rel", vTE, "TE_TablRec", zPOS_AFTER );
         RESULT = SetCursorNextEntityByInteger( vTE, "ER_RelType", "ZKey", lZKey, "TE_DB_Environ" );
      }

      RESULT = SetCursorNextEntity( vEMD, "ER_RelType", "" );
   }

   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
