/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzeremdo.c - Zeidon application operations
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

/*
CHANGE LOG
2000.10.18  BL
   Modified oTZEREMDO_DeleteEntity for delete View Copy
2000.06.19
   Modified oTZEREMDO_DeleteRelationship: if delete Relationship and this
   Relationship is a part of the Identifier, then delete the Relationship
   from the Identifier

from here old version

12.10.1997  DonC
   Corrected DeleteEntity and DeleteRelationship to properly handle the
   deletion of corresponding entries in an open Subject Area.

19.03.1998  DonC
   Added operation oTZEREMDO_FactTypeType.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZEREMDO_DeriveKey
//
// PURPOSE:   Derived function to set ParticipatesInKey for ER_Attribute
//            Entity in TZEREMDO
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_DeriveKey( zVIEW            vTZEREMDO,
                     LPVIEWENTITY     lpViewEntity,
                     LPVIEWATTRIB     lpViewAttrib,
                     zSHORT           nMsg )
{
   zSHORT nRC;

   // Derive Participates in Key
   nRC = SetCursorFirstEntity( vTZEREMDO, "ER_EntIdentifier", "" );
   nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_AttributeIdentifier",
                                          vTZEREMDO, lpViewEntity->szName,
                                          "ER_EntIdentifier" );
   if ( nRC >= zCURSOR_SET )
   {
      StoreValueInRecord( vTZEREMDO, lpViewEntity, lpViewAttrib, "Y", 0 );
   }
   else
   {
      StoreValueInRecord( vTZEREMDO, lpViewEntity, lpViewAttrib, "N", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZEREMDO_FactTypeType
//
// PURPOSE:   Derived function to set the type of FactType entry, whether
//            Attribute ("ATR") or Relationship ("REL").
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_FactTypeType( zVIEW            vTZEREMDO,
                        LPVIEWENTITY     lpViewEntity,
                        LPVIEWATTRIB     lpViewAttrib,
                        zSHORT           nMsg )
{
   if ( CheckExistenceOfEntity( vTZEREMDO, "ER_AttributeIdentifier" ) >= zCURSOR_SET )
   {
      StoreValueInRecord( vTZEREMDO, lpViewEntity, lpViewAttrib, "ATR", 0 );
   }
   else
   {
      StoreValueInRecord( vTZEREMDO, lpViewEntity, lpViewAttrib, "REL", 0 );
   }

   return( 0 );
}

typedef zVOID (POPERATION zFARPROC_RemoveFunc)( zVIEW, zPCHAR, zBOOL, zLONG );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZEREMDO_DeleteEntity
//
// PURPOSE:   Delete Relationship from ER Model
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_DeleteEntity( zVIEW  vTZEREMDO,
                        zSHORT nReposition )
{
   zVIEW    vTZEREMD1;
   zVIEW    vTZERSASO;
   zVIEW    vSubtask;   // to refresh ER Diagram
   zSHORT   nRC;
   zLONG    lEntityZKey;
   LPLIBRARY hLibrary = 0;  // used for dynamic call to ER control func's
   zFARPROC_RemoveFunc lpfn = 0;

//  Position and determine if there are any relationships involving
//  this Entity prior to deleting the Entity.
   nRC = CreateViewFromViewForTask( &vTZEREMD1, vTZEREMDO, 0 );
   nRC = GetIntegerFromAttribute( &lEntityZKey, vTZEREMD1, "ER_Entity", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity_2",
                                        "ZKey", lEntityZKey,
                                        "EntpER_Model" );

//  Delete the entity if it is involved in any relationships.
   if ( nRC >= zCURSOR_SET )
   {
      //  Loop and Delete all ER RelLinks
      for ( nRC = SetCursorFirstEntity( vTZEREMDO, "ER_RelLink", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" ) )
      {
         oTZEREMDO_DeleteRelationship( vTZEREMDO, zREPOS_NONE, 1 );
      }
      //  Check to if all relationships were deleted.
      if ( SetCursorFirstEntityByInteger( vTZEREMD1, "ER_Entity_2",
                                          "ZKey", lEntityZKey,
                                          "EntpER_Model" ) >= zCURSOR_SET )
      {
         DropView( vTZEREMD1 );
         return( -1 );
      }
   }

// Delete ER Entity
   if ( GetViewByName( &vSubtask, "TZEREMDD", vTZEREMDO, zLEVEL_TASK ) > 0 )
   {
      lpfn = (zFARPROC_RemoveFunc)
             GetOperationDynamicCallAddress( vSubtask,
                                             (LPLPLIBRARY) &hLibrary,
                                             "TZCTL", "RemoveEntity",
                                             "(tzlooprs)" );
      if ( lpfn )
        (*lpfn)( vSubtask, "ERD", 0, 0 );
   }

   nRC = DeleteEntity( vTZEREMDO, "ER_Entity", nReposition );

   // It is possible that the ER_Entity we just deleted is part of an
   // opened Subject Area.  If so, we must be sure that Subject Area doesn't
   // have hanging SA_Entity entities.
   if ( GetViewByName( &vTZERSASO, "TZERSASO", vTZEREMDO, zLEVEL_TASK ) > 0 )
   {
      nRC = SetCursorFirstEntity( vTZERSASO, "SA_Entity", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CheckExistenceOfEntity( vTZERSASO, "ER_Entity" ) < zCURSOR_SET )
         {
            if ( GetViewByName( &vSubtask, "SAVIEW", vTZEREMDO, zLEVEL_TASK ) > 0 )
            {
               if ( lpfn == 0 )   // only get func addr once
               {
                  lpfn = (zFARPROC_RemoveFunc)
                     GetOperationDynamicCallAddress( vSubtask,
                                                     (LPLPLIBRARY) &hLibrary,
                                                     "TZCTL", "RemoveEntity",
                                                     "(tzlooprs)" );
               }
               if ( lpfn )
               (*lpfn)( vSubtask, "ERD", 0, 0 );
            }

            DeleteEntity( vTZERSASO, "SA_Entity", zREPOS_NONE );
         }

         nRC = SetCursorNextEntity( vTZERSASO, "SA_Entity", 0 );
      }
   }

   DropView( vTZEREMD1 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZEREMDO_DeleteRelationship
//
// PURPOSE:   Delete Relationship from ER Model
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_DeleteRelationship( zVIEW  vTZEREMDO,
                              zSHORT nReposition,
                              zSHORT nQuietMode )
{
   zVIEW          vTZERSASO;
   zVIEW          vSubtask;
   zVIEW          vERD;
   zSHORT         nRC;
   zLONG          lZKey;
   LPLIBRARY      hLibrary = 0;  // used for dynamic call to ER control func's
   zFARPROC_RemoveFunc lpfn = 0;

   //  Positon on ER_RelType on Right Side( side representing relationships
   //  only ) thus allowing a single Delete to cascade and Delete all
   //  representations of the Relationship on both sides of the Object.
   nRC = SetCursorFirstEntityByEntityCsr( vTZEREMDO, "ER_RelType",
                                          vTZEREMDO, "ER_RelType_1",
                                          "EntpER_Model" );

   if ( GetViewByName( &vSubtask, "TZEREMDD", vTZEREMDO, zLEVEL_TASK ) > 0 )
   {
      if ( lpfn == 0 )   // only get func addr once
      {
         lpfn =(zFARPROC_RemoveFunc)
                  GetOperationDynamicCallAddress( vSubtask,(LPLPLIBRARY) &hLibrary,
                     "TZCTL", "RemoveRelationship", "(tzlooprs)" );
      }

      if ( lpfn )
        (*lpfn)( vSubtask, "ERD", 0, 0 );
   }

   // if delete Relationship and this Relationship is a part of the
   // Identifier, then delete the Relationship from the Identifier
   CreateViewFromViewForTask( &vERD, vTZEREMDO, 0 );

   for ( nRC = SetCursorFirstEntity( vERD, "ER_RelLink_2", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vERD, "ER_RelLink_2", "" ) )
   {
      GetIntegerFromAttribute( &lZKey, vERD, "ER_RelLink_2", "ZKey" );
      for ( nRC = SetCursorFirstEntityByInteger( vERD, "ER_RelLinkIdentifier", "ZKey", lZKey, "EntpER_Model" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntityByInteger( vERD, "ER_RelLinkIdentifier", "ZKey", lZKey, "EntpER_Model" ))
      {
         if ( CheckExistenceOfEntity( vERD, "ER_AttributeIdentifier" ) == zCURSOR_NULL )
         {

            DeleteEntity( vERD, "ER_FactType", zREPOS_NONE );
         }

      }
   }

   DropView( vERD );

   nRC = DeleteEntity( vTZEREMDO, "ER_RelType", nReposition );
   if ( nReposition != zREPOS_NONE )
   {
      if ( nReposition == zREPOS_NEXT )
         SetCursorNextEntity( vTZEREMDO, "ER_RelLink", "" );
      else
         SetCursorPrevEntity( vTZEREMDO, "ER_RelLink", "" );
   }

   // It is possible that the ER_RelType we just deleted is part of an
   // opened Subject Area.  If so, we must be sure that Subject Area doesn't
   // have hanging SA_RelType entities that result from the deleted
   // relationships.
   if ( GetViewByName( &vTZERSASO, "TZERSASO", vTZEREMDO, zLEVEL_TASK ) > 0 )
   {
      nRC = SetCursorFirstEntity( vTZERSASO, "SA_RelType", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CheckExistenceOfEntity( vTZERSASO, "ER_RelType" ) < zCURSOR_SET )
         {
            if ( GetViewByName( &vSubtask, "SAVIEW", vTZEREMDO, zLEVEL_TASK ) > 0 )
            {
               if ( lpfn == 0 )   // only get func addr once
               {
                  lpfn = (zFARPROC_RemoveFunc)
                     GetOperationDynamicCallAddress( vSubtask,
                                                     (LPLPLIBRARY) &hLibrary,
                                                     "TZCTL",
                                                     "RemoveRelationship",
                                                     "(tzlooprs)" );
               }
               if ( lpfn )
               (*lpfn)( vSubtask, "ERD", 0, 0 );
            }

            DeleteEntity( vTZERSASO, "SA_RelType", zREPOS_NONE );
         }

         nRC = SetCursorNextEntity( vTZERSASO, "SA_RelType", 0 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZEREMDO_GetUpdViewForER
//
// PURPOSE:   Get Updatable ER View.  Activate if not loaded
//            and get at application level if loaded.
//
//////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_GetUpdViewForER( zVIEW  vSubtask,
                           zPVIEW pvTZEREMDO,
                           zSHORT nCurrentOrReload )
{
   zVIEW  vTZERMFLO;
   zSHORT nRC;

// Get Access to ERD File List Object
   *pvTZEREMDO = 0;
   nRC = RetrieveViewForMetaList( vSubtask, &vTZERMFLO, zREFER_ERD_META );
   if ( nRC < 0 )
      return( -2 );

   if ( SetCursorFirstEntity( vTZERMFLO, "W_MetaDef", "" ) < zCURSOR_SET )
   {
      return( -1 );
   }
   else
   {
      nRC = ActivateMetaOI( vSubtask, pvTZEREMDO, vTZERMFLO, zSOURCE_ERD_META,
                            zSINGLE | zLEVEL_APPLICATION );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZEREMDO_GetRefViewForER
//
// PURPOSE:   Get access to ER View for Reference.  Activate if not loaded
//            and get at application level if loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_GetRefViewForER( zVIEW  vSubtask,
                           zPVIEW pvTZEREMDO,
                           zSHORT nCurrentOrReload )
{
   zVIEW  vTZERMFLO;
   zSHORT nRC;

// Get Access to ERD File List Object
   nRC = RetrieveViewForMetaList( vSubtask, &vTZERMFLO, zREFER_ERD_META );
   if ( nRC < 0 )
   {
      *pvTZEREMDO = 0;
      return( -2 );
   }

   if ( SetCursorFirstEntity( vTZERMFLO, "W_MetaDef", "" ) < zCURSOR_SET )
   {
      return( -2 );
   }

   nRC = ActivateMetaOI( vSubtask, pvTZEREMDO, vTZERMFLO,
                         zREFER_ERD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   return( nRC );
}

///////////////////////////// end of file  //////////////////////////////////
