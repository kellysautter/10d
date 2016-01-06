/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadcsdo.c - Zeidon application operations
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

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZADCSDO_DeriveKey
//
// PURPOSE:   Derived function to set ParticipatesInKey for LOD_Attribute
//            and LOD_AttributeRec Entities in TZZOLODO
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_DeriveKey( zVIEW        vTZADCSDO,
                     LPVIEWENTITY lpViewEntity,
                     LPVIEWATTRIB lpViewAttrib,
                     zSHORT       nMsg )
{
   zVIEW  vTZEREMDO_REF;
   zVIEW  vTemp;
   zVIEW  vLOD;
   zPCHAR szEntity;
   zPCHAR szAttribER;
   zSHORT nRC;

   // Set up the Entity and Attribute names for use in SetCurosor operations
   // based on which LOD Entity is using the derivation algorithm.
   if ( zstrcmp( lpViewEntity->szName, "M_LOD_Attribute" ) == 0 )
   {
      szEntity = "M_LOD_Entity";
      szAttribER = "M_ER_Attribute";
   }
   else
   if ( zstrcmp( lpViewEntity->szName, "MT_LOD_Attribute" ) == 0 )
   {
      szEntity = "MT_LOD_Entity";
      szAttribER = "MT_ER_Attribute";
   }
   else
   {
      szEntity = "L_LOD_Entity";
      szAttribER = "L_ER_Attribute";
   }

   GetViewByName( &vLOD, "LOD_Object", vTZADCSDO, zLEVEL_TASK );
   SetCursorFirstEntityByEntityCsr( vLOD, "LOD_Entity",
                                    vTZADCSDO, szEntity, "" );

   // Get access to ER Object.
   nRC = GetViewByName( &vTZEREMDO_REF, "TZEREMDO_REF", vTZADCSDO, zLEVEL_TASK );
   if ( nRC < 1 )
   {
      RetrieveViewForMetaList( vTZADCSDO, &vTemp, zREFER_ERD_META );
      ActivateMetaOI( vTZADCSDO, &vTZEREMDO_REF, vTemp,
                      zREFER_ERD_META, zSINGLE | zLEVEL_APPLICATION );
      SetNameForView( vTZEREMDO_REF, "TZEREMDO_REF", vTZADCSDO, zLEVEL_TASK );
   }

   // Derive Participates in Key.
   if ( CheckExistenceOfEntity( vLOD, "ER_Entity" ) >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO_REF, "ER_Entity",
                                             vLOD, "ER_Entity", "" );
      if ( nRC >= zCURSOR_SET )
      {
         // Now set the LOD_Attribute.ParticipatesInKey from
         //             ER_Attribute.ParticipatesInKey.
         // The following "correct" method is replaced for speed.
         SetCursorFirstEntity( vTZEREMDO_REF, "ER_EntIdentifier", "" );
         nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO_REF,
                                                "ER_AttributeIdentifier",
                                                vTZADCSDO, szAttribER, "" );
         if ( nRC >= zCURSOR_SET )
         {
            if ( CompareAttributeToString( vTZEREMDO_REF, "ER_EntIdentifier",
                                           "SystemMaintained", "Y" ) == 0 )
            {
               StoreValueInRecord( vTZADCSDO, lpViewEntity, lpViewAttrib, "G", 0 );
            }
            else
            {
               StoreValueInRecord( vTZADCSDO, lpViewEntity, lpViewAttrib, "Y", 0 );
            }
         }
         else
         {
            StoreValueInRecord( vTZADCSDO, lpViewEntity, lpViewAttrib, "N", 0 );
         }
      }
   }
   else
   {
      StoreValueInRecord( vTZADCSDO, lpViewEntity, lpViewAttrib, "", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZADCSDO_DeriveAttr
//
// PURPOSE:   Derived function to set ParticipatesInKey for LOD_Attribute
//            and LOD_AttributeRec Entities in TZZOLODO
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_DerivedAttr( zVIEW            vTZADCSDO,
                       LPVIEWENTITY     lpViewEntity,
                       LPVIEWATTRIB     lpViewAttrib,
                       zSHORT           nMsg )
{
   zVIEW   vLOD;
   zPCHAR  szLOD_AttributeName;

   // szLOD_AttributeName is either M_LOD_Attribute or MT_LOD_Attribute

   szLOD_AttributeName = lpViewEntity->szName;

   GetViewByName( &vLOD, "LOD_Object", vTZADCSDO, zLEVEL_TASK );
   SetCursorFirstEntityByEntityCsr( vLOD,      "LOD_Attribute",
                                    vTZADCSDO, szLOD_AttributeName, "LOD" );

   // Derive Participates in Key.
   if ( CheckExistenceOfEntity( vLOD,
                                "LOD_AttrDerivationOper" ) >= zCURSOR_SET )
   {
      StoreValueInRecord( vTZADCSDO, lpViewEntity, lpViewAttrib, "Y", 0 );
   }
   else
   {
      StoreValueInRecord( vTZADCSDO, lpViewEntity, lpViewAttrib, "N", 0 );
   }

   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
