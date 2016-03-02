#include "kzoephdr.h"
//
// MODULE NAME:  KZOEEIAA  -  Object services Entity Instance Management
// DESCRIPTION:  This is the source file which contains Object services
//               operations for creating and deleting entity instances
// OPERATIONS:   All OPERATIONS which create, move, copy, or delete
//               Entity instances
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    4/13/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2002.05.02  HH
   Improved SetIncrementalUpdateFlags, last change was buggy.

2002.04.29  DGC
   Changed the logic on the return code for CheckExistenceOfEntity( ).

   Changed SetIncrementalUpdateFlags( ) to check to entire OI to see if the
   OI updated flags need to be set.

2001.08.16  DGC
   Fixed bug reported by Jeff.  In fnValidateInsertPosition( ) we were not
   converting a handle to a pointer.

2001.08.04  HH
   fnInstanceVersioned check also versioned root.

2001.04.06  DGC  10b
   Added stuff for entity keys.

2001.02.28  DGC  10b
   Added functionality to SetIncrementUpdateFlags( ) to allow setting all
   attribute flags as well.

2000.12.14  DGC  10b
   Change requested by Jeff.  We now allow updates of derived entities even
   if the view is read-only.

2000.12.01  DGC  10b
   Fix for important bug reported by Frank H.  When an entity was being
   excluded it was being flagged as updated.

2000.10.12  DGC
   Changed logic to use fnSetUpdateIndicator( ) instead of updating the flag
   directly.

2000.06.01  DGC
   Changed DropEntity( ). It now removes entity instances that have been
   dropped from the linked chain.

2000.05.04  DGC
   Changed signature of LoadEntity( ).

2000.03.03  DGC
   Added code to fix bug that kept Duplicate Relationship entities from
   being spawned after an activate.

1999.12.21  DGC
   During DeleteEntity( ) processing, don't bother checking for the 'Restrict'
   flag on child entities that are hidden.

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

1999.08.17  DGC  10a
   Added zSET_INCRE_DELETED flag.

1999.05.07  DGC  10a
   During an Include/Relink we check the ER_DATE attribute in the ViewOD to
   make sure that the ODs are not out of sync.

1999.02.12  DGC
   Started adding (and removing) stuff for attribute flags.

*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#define __ACTIVATE_CONSTRAINTS__     // these are common to kzoesoaa.c

static zSHORT
fnCheckOIUpdated( LPVIEWOI lpViewOI );

zPCHAR LOCALOPER
fnEntityNameFromCsr( LPVIEWENTITYCSR lpViewEntityCsr );

// Entity was just created.  Initialize attribute values.
void LOCALOPER
fnInitAttributes( zVIEW        lpView,
                  LPVIEWENTITY lpViewEntity )
{
   LPVIEWATTRIB lpViewAttrib;

   if ( lpViewEntity->bAttrInit == FALSE )
      return;

   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      if ( lpViewAttrib->hInit )
      {
         StoreStringInRecord( lpView, lpViewEntity, lpViewAttrib,
                              zGETPTR( lpViewAttrib->hInit ) );
      }
   }

} // fnInitAttributes

//./ ADD NAME=CheckExistenceOfEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   CheckExistenceOfEntity
//
//  PURPOSE: To check whether or not a cursor position is
//           established on a specified entity
//
//  RETURNS: zCURSOR_NULL         Cursor position for Entity is NULL
//           zCURSOR_UNDEFINED    Cursor position for Entity is undefined
//           zCURSOR_SET          Cursor position for Entity is set
//           zCALL_ERROR          Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
CheckExistenceOfEntity( zVIEW   lpView,
                        zCPCHAR cpcEntityName )
{
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpScanEntityInstance;
   LPTASK            lpCurrentTask;
   zSHORT            nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCheckExistenceOfEntity, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Set lpViewCsr
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   // Validate that the entity exists.
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   nRC = zCURSOR_NULL;  // default RC to -3 (NULL Instance)

   // If object instance is not empty, check for entity.
   if ( lpViewOI->hRootEntityInstance )
   {
      if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
         fnEstablishCursorForView( lpViewEntityCsr );

      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

      // if the cursor needs to be established, do it now
      if ( lpEntityInstance )
      {
         if ( lpEntityInstance->u.nInd.bHidden )
         {
            // The EntityInstance is hidden, we start off assuming
            // that all twins are hidden, scan 'forward' for
            // any twin occurrence that is not hidden, if none are
            // found then scan 'backward' for any twin occurrence
            // that is not hidden.  If an occurrence is found that
            // is not hidden, the return code is set to indicate
            // that the cursor is undefined.

            // default indicates that there are no entity instances
            nRC = zCURSOR_NULL;
            lpScanEntityInstance = zGETPTR( lpEntityInstance->hNextTwin );
            while ( lpScanEntityInstance )
            {
               if ( lpScanEntityInstance->u.nInd.bHidden == FALSE )
               {
                  nRC = zCURSOR_UNDEFINED;
                  break;
               }

               lpScanEntityInstance = zGETPTR( lpScanEntityInstance->hNextTwin );
            }

            if ( nRC == zCURSOR_NULL )
            {
               lpScanEntityInstance = zGETPTR( lpEntityInstance->hPrevTwin );
               while ( lpScanEntityInstance )
               {
                  if ( lpScanEntityInstance->u.nInd.bHidden == FALSE )
                  {
                     nRC = zCURSOR_UNDEFINED;
                     break;
                  }

                  lpScanEntityInstance = zGETPTR( lpScanEntityInstance->hPrevTwin );
               }
            }
         }
         else
            nRC = zCURSOR_SET;
      }

// DGC 4-29-2002
// Made a change requested by Doug, Phil, and TB.  A cursor is returned NULL
// even if the parent cursors are NULL.  We used to return UNDEFINED if a
// parent was NULL but that made a certain situation ambiguous.
#if 0
      // The instance appears to be NULL, However it may be null under
      // a deleted parent somewhere. Find the supposed parent to the
      // entity and make sure the parent is not deleted!!!
      if ( lpEntityInstance == 0 )
      {
         for ( ; ; )
         {
            while ( lpViewEntityCsr && lpViewEntityCsr->hEntityInstance == 0 )
               lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );

            if ( lpViewEntityCsr == 0 ||
                 lpViewEntityCsr->hEntityInstance != UNSET_CSR )
            {
               break;
            }

            fnEstablishCursorForView( lpViewEntityCsr );
         }

         if ( lpViewEntityCsr )
            lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
         else
            lpEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );

         if ( lpEntityInstance && lpEntityInstance->u.nInd.bHidden )
         {
#if 0  // Gig, 10/26/92 ?? Should this be an error ???
            //  "KZOEE254 - Entity cursor is undefined"
            fnIssueCoreError( lpTask, lpView, 8, 254, 0,
                              lpViewEntity->szName, 0 );
            nRC = zCALL_ERROR;
#else
            nRC = zCURSOR_UNDEFINED;
#endif
         }
      }
#endif
   }

   fnOperationReturn( iCheckExistenceOfEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=CheckEntityInView
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   CheckEntityInView
//
//  PURPOSE: To check whether or not an entity is in view or not.  Note that
//           this does not check for the existence of an entity instance.
//
//  RETURNS:  0 - Entity is in view.
//           -1 - Entity not in view.
//           -2 - Entity not in object definition.
//        zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
CheckEntityInView( zVIEW   lpView,
                   zCPCHAR cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOD          lpViewOD;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   zSHORT            nStartLevel;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCheckEntityInView, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Get the view csr for the view.
   lpViewCsr = zGETPTR( lpView->hViewCsr );

   // Get the view object definition for the view
   lpViewOD = zGETPTR( lpView->hViewOD );

   // Validate that the entity exists.
   lpViewEntity    = zGETPTR( lpViewOD->hFirstOD_Entity );
   lpViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );

   // If the root cursor is not the first cursor, skip ViewEntities
   // until we get to the entity matching the root cursor.
   if ( lpViewEntityCsr != zGETPTR( lpViewCsr->hFirstOD_EntityCsr ) )
   {
      LPVIEWENTITYCSR lpSearchViewEntityCsr;

      lpSearchViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
      while ( lpSearchViewEntityCsr != lpViewEntityCsr )
      {
         lpViewEntity = zGETPTR( lpViewEntity->hNextHier );
         lpSearchViewEntityCsr++;
      }
   }

   nStartLevel = lpViewEntityCsr->nLevel;

   if ( cpcEntityName[ 0 ] == '\xff' )
   {
      zLONG lInputTok;

      lInputTok = *((zPLONG) (cpcEntityName + 1));
      while ( lpViewEntityCsr && lpViewEntity->lEREntTok != lInputTok )
      {
         if ( lpViewEntityCsr->nLevel < nStartLevel )  // dks 2006.05.08 10B diff
         {
            lpViewEntityCsr = 0;
            break;
         }

         lpViewEntity    = zGETPTR( lpViewEntity->hNextHier );
         lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
      }
   }
   else
   {
      while ( lpViewEntityCsr &&
              zstrcmp( lpViewEntity->szName, cpcEntityName ) != 0 )
      {
         if ( lpViewEntityCsr->nLevel < nStartLevel )  // dks 2006.05.08 10B diff
         {
            lpViewEntityCsr = 0;
            break;
         }

         lpViewEntity    = zGETPTR( lpViewEntity->hNextHier );
         lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
      }
   }

   // If entity found return OK.
   if ( lpViewEntityCsr )
   {
      fnOperationReturn( iCheckEntityInView, lpCurrentTask );
      return( 0 );
   }

   // Now make sure that the entity is valid for the view.
   for ( lpViewEntity = zGETPTR( lpViewOD->hFirstOD_Entity );
         lpViewEntity;
         lpViewEntity = zGETPTR( lpViewEntity->hNextHier ) )
   {
      if ( zstrcmp( lpViewEntity->szName, cpcEntityName ) == 0 )
      {
         fnOperationReturn( iCheckEntityInView, lpCurrentTask );
         return( -1 );
      }
   }

#if 0  // we don't want an error ... we're just asking the question

   // Note, the name may not be invalid, it may simply be out of view
   // because of a call to SetViewToSubobject
   //  "KZOEE103 - Invalid Entity name for View"
   TraceLine( "Invalid Entity name: %s   for View (0x%08x)   OD: %s",
              cpcEntityName, lpView, lpViewOD->szName );
   fnIssueCoreError( lpCurrentTask, lpView, 8, 103, 0,
                     lpViewOD->szName,
                     TrueName( cpcEntityName, zSHOW_ZKEY ) );
#endif

   fnOperationReturn( iCheckEntityInView, lpCurrentTask );
   return( -2 );
}

//./ ADD NAME=CreateTemporalEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CreateTemporalEntity
//
//  PURPOSE:    To create a Temporal Entity instance using the passed
//              view
//  PARAMETERS:
//              lpView        - View that contains the entity
//              cpcEntityName - Name of the entity to create
//              nPosition     - zPOS_BEFORE(4) or zPOS_AFTER(3) the current
//                              position, or zPOS_FIRST(1) or zPOS_LAST(2)
//
//  RETURNS:    0           - Entity Instance created
//              zCALL_ERROR - Error creating entity instance
//
//  NOTE: A WORK IN TEST...
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
CreateTemporalEntity( zVIEW   lpView,
                      zCPCHAR cpcEntityName,
                      zSHORT  nPosition )
{
   LPTASK          lpCurrentTask;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zSHORT          nPositionOK;  // 0=after, 1=before
   // int          nExtraRecordFlags;
   zSHORT          bNewRoot;
   zSHORT          nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCreateTemporalEntity, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists.
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iCreateTemporalEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If the view is read only, issue an error.
   if ( lpView->bReadOnly )
   {
      // "KZOEE119 - Invalid View, View is Read Only"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 119, 0, cpcEntityName, 0 );
      fnOperationReturn( iCreateTemporalEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure entity can be created.
   if ( lpViewEntity->bCreate == FALSE )
   {
      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 106, 0,
                        "Entity Creation not allowed",
                        lpViewEntity->szName );
      fnOperationReturn( iCreateTemporalEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Validate that an insert of this entity in this instance at
   // the specified position is valid.
   nPositionOK = fnValidateInsertPosition( &bNewRoot,
                                           iCreateTemporalEntity,
                                           lpView,
                                           lpViewEntityCsr,
                                           nPosition );
   if ( nPositionOK < 0 )
   {
      fnOperationReturn( iCreateTemporalEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

#ifdef __ACTIVATE_CONSTRAINTS__
   // If there is a create constraint active for this entity we need to
   // use a work view, just in case the constraint handler objects and
   // we have to set everything back to the way it was.
   if ( lpViewEntity->bCreateConstraint )
   {
      zVIEW             lpWorkView;
      LPVIEWENTITYCSR   lpWorkViewEntityCsr;
      zSHORT            n;

      CreateViewFromViewForTask( &lpWorkView, lpView, 0 );

      // Reset the View entity csr for work view.
      fnValidViewEntity( &lpWorkViewEntityCsr, lpWorkView, cpcEntityName, 0 );

      // Call fnCreateTemporalEntity to create the entity.
      nRC = fnCreateEntity( lpWorkView, lpViewEntity,
                            lpWorkViewEntityCsr,
                            nPosition, bNewRoot );
      if ( nRC )
      {
         fnDropView( lpWorkView );
         fnOperationReturn( iCreateTemporalEntity, lpCurrentTask );
         return( nRC );
      }

      // If the view entity has any initialization attributes, initialize
      // them now.
      fnInitAttributes( lpWorkView, lpViewEntity );

      n = fnInvokeECEOperation( lpWorkView, lpViewEntity,
                                lpCurrentTask, zECE_CREATE, 0 );
      if ( n )
      {
         fnDeleteEntity( lpWorkView, lpViewEntity, lpWorkViewEntityCsr,
                         zREPOS_NONE, lpCurrentTask );
         fnDropView( lpWorkView );
         fnOperationReturn( iCreateTemporalEntity, lpCurrentTask );
         return( n );
      }
      else
      {
         // Save cursors from the work view to the original.
         fnSetViewFromView( lpView, lpWorkView);

         // Drop work view.
         fnDropView( lpWorkView );
      }
   }
   else
#endif

   {
      // Call fnCreateEntity to create the entity.
      nRC = fnCreateEntity( lpView, lpViewEntity, lpViewEntityCsr,
                           nPosition, bNewRoot );
      // If the view entity has any initialization attributes, initialize
      // them now.
      if ( nRC == 0 )
         fnInitAttributes( lpView, lpViewEntity );
   }

   // If the entity instance has been successfully created, mark the
   // Entity instance as owning the entity record and check the
   // instances parent to see if any other linked instances require
   // spawning this entity type. If so, issue a warning message for
   // now since the overhead in establishing a view for spawning
   // entity creations may not be worth the effort and performance
   // lost when compared to the benefit gained. Unfortunately, this
   // check is done after the entity has been created (i.e. the horses
   // are already out of the barn).
   if ( nRC == 0 )
   {
      LPENTITYINSTANCE  lpEntityInstance;
      LPVIEWCSR         lpViewCsr;
      LPVIEWOI          lpViewOI;

      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      lpEntityInstance->u.nInd.bCreated  = TRUE;
      lpEntityInstance->u.nInd.bTemporal = TRUE;
      lpViewCsr = zGETPTR( lpView->hViewCsr );
      lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
      lpViewOI->nVersionedInstances++;
   }

   // return success, we've managed to create an entity
   fnOperationReturn( iCreateTemporalEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=CreateEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CreateEntity
//
//  PURPOSE:    To create an Entity instance using the passed
//              view
//  PARAMETERS:
//              lpView        - View that contains the entity
//              cpcEntityName - Name of the entity to create
//              nPosition     - zPOS_BEFORE(4) or zPOS_AFTER(3) the current
//                              position, or zPOS_FIRST(1) or zPOS_LAST(2)
//
//  RETURNS:    0           - Entity Instance created
//              zCALL_ERROR - Error creating entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
CreateEntity( zVIEW   lpView,
              zCPCHAR cpcEntityName,
              zSHORT  nPosition )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   zSHORT            nPositionOK;  // 0=after, 1=before
// int               nExtraRecordFlags;
   zSHORT            bNewRoot;
   zSHORT            nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCreateEntity, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists.
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iCreateEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If the view is read only, issue an error.
   if ( lpView->bReadOnly && lpViewEntity->bDerivedPath == FALSE )
   {
      // "KZOEE119 - Invalid View, View is Read Only"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 119, 0, cpcEntityName, 0 );
      fnOperationReturn( iCreateEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure entity can be created.
   if ( lpViewEntity->bCreate == FALSE )
   {
      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 106, 0,
                        "Entity Creation not allowed",
                        lpViewEntity->szName );
      fnOperationReturn( iCreateEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

#if 0
   // Debugging code ... dks 2007.05.24
   if ( lpViewEntityCsr->nLevel > 1 ) // parent exists at definition level
   {
      LPVIEWENTITYCSR lpParentViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );
      LPENTITYINSTANCE lpParentEntityInstance;

      // If there is no parent entity cursor, then this entity is to be the new root
      // of a subobject view, the parent in this case is the parent of the view.
      if ( lpParentViewEntityCsr )
      {
         lpParentEntityInstance = zGETPTR( lpParentViewEntityCsr->hEntityInstance );
      // if ( lpParentEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "CreateEntity", "UNSET_CSR", -1 );
      }
      else
      {
         LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
         lpParentEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );
      // if ( lpParentEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "CreateEntity", "UNSET_CSR", -1 );
      }
   }
#endif

   // Validate that an insert of this entity in this instance at the specified
   // position is valid.
   nPositionOK = fnValidateInsertPosition( &bNewRoot, iCreateEntity, lpView,
                                           lpViewEntityCsr, nPosition );
   if ( nPositionOK < 0 )
   {
      fnOperationReturn( iCreateEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

#ifdef __ACTIVATE_CONSTRAINTS__
   // If there is a create constraint active for this entity we need to
   // use a work view, just in case the constraint handler objects and
   // we have to set everything back to the way it was.
   if ( lpViewEntity->bCreateConstraint )
   {
      zVIEW             lpWorkView;
      LPVIEWENTITYCSR   lpWorkViewEntityCsr;
      zSHORT            n;
      LPENTITYINSTANCE  lpEntityInstance;

      CreateViewFromViewForTask( &lpWorkView, lpView, 0 );

      // Reset the View entity csr for work view.
      fnValidViewEntity( &lpWorkViewEntityCsr, lpWorkView, cpcEntityName, 0 );

      // Call fnCreateEntity to create the entity.
      nRC = fnCreateEntity( lpWorkView, lpViewEntity,
                            lpWorkViewEntityCsr,
                            nPosition, bNewRoot );
      if ( nRC )
      {
         fnDropView( lpWorkView );
         fnOperationReturn( iCreateEntity, lpCurrentTask );
         return( nRC );
      }

      // If the view entity has any initialization attributes, initialize
      // them now.
      fnInitAttributes( lpWorkView, lpViewEntity );

      // Set bCreated flag so constraint operation can modify keys if
      // wanted.  It will be turned on again later but that's OK.
      lpEntityInstance = zGETPTR( lpWorkViewEntityCsr->hEntityInstance );
      lpEntityInstance->u.nInd.bCreated = TRUE;

      n = fnInvokeECEOperation( lpWorkView, lpViewEntity,
                                lpCurrentTask, zECE_CREATE, 0 );
      if ( n )
      {
         fnDeleteEntity( lpWorkView, lpViewEntity, lpWorkViewEntityCsr,
                         zREPOS_NONE, lpCurrentTask );
         fnDropView( lpWorkView );
         fnOperationReturn( iCreateEntity, lpCurrentTask );
         return( n );
      }
      else
      {
         // Save cursors from the work view to the original.
         fnSetViewFromView( lpView, lpWorkView );

         // Drop work view.
         fnDropView( lpWorkView );
      }
   }
   else
#endif
   {
      // Call fnCreateEntity to create the entity.
      nRC = fnCreateEntity( lpView, lpViewEntity, lpViewEntityCsr,
                            nPosition, bNewRoot );

      // If the view entity has any initialization attributes, initialize them now.
      if ( nRC == 0 )
         fnInitAttributes( lpView, lpViewEntity );
   }

   // If the entity instance has been successfully created, mark the
   // Entity instance as owning the entity record and check the
   // instance's parent to see if any other linked instances require
   // spawning this entity type.  If so, issue a warning message for
   // now since the overhead in establishing a view for spawning
   // entity creations may not be worth the effort and performance
   // lost when compared to the benefit gained.  Unfortunately, this
   // check is done after the entity has been created (i.e. the horse
   // is already out of the barn).
   if ( nRC == 0 )
   {
      LPENTITYINSTANCE  lpEntityInstance;

      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      lpEntityInstance->u.nInd.bCreated = TRUE;

#if 0  // We have a case where the hParent is UNSET_CSR (1), but
       // lpViewEntityCsr->hParent is 0 and not sure what to do about it.  // dks 2007.05.16
      if ( lpEntityInstance->hParent == UNSET_CSR )
      {
         LPVIEWENTITYCSR   lpParentViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent )
         LPENTITYINSTANCE  lpEntityInstance;

         SysMessageBox( 0, "Establishing Cursor", "Prior to SpawnCreateEntity", -1 );
         fnEstablishCursorForView( lpViewEntityCsr );
      }
#endif

      // Spawn the entity create if necessary.
      fnSpawnCreateEntity( lpView, lpEntityInstance );

// >>> RAD54278-1 BEGIN
      fnSetUpdateIndicator( 0, lpEntityInstance, 0, TRUE, FALSE, FALSE );
// >>> RAD54278-1 END
   }

   // Return success, we've managed to create an entity.
   fnOperationReturn( iCreateEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=LoadEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      LoadEntity
//
//  PURPOSE:    To create an Entity instance using the passed
//              view, ignoring rules...
//  PARAMETERS:
//              lpView        - View that contains the entity
//              cpcEntityName - Name of the entity to create
//              nPosition     - zPOS_BEFORE(4) or zPOS_AFTER(3) the current
//                              position, or zPOS_FIRST(1) or zPOS_LAST(2)
//              plpRecord     - If non-zero this argument is set to the
//                              persist record of the entity.
//
//  RETURNS:    0           - Entity Instance loaded
//              zCALL_ERROR - Error creating entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
LoadEntity( zVIEW   lpView,
            zCPCHAR cpcEntityName,
            zSHORT  nPosition,
            zPPCHAR plpRecord )
{
   LPTASK          lpCurrentTask;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zSHORT          nPositionOK;  // 0=after, 1=before
   // int          nExtraRecordFlags;
   zSHORT          bNewRoot;
   zSHORT          nRC;

   if ( plpRecord )
      *plpRecord = 0;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iLoadEntity, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iLoadEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Validate that an insert of this entity in this instance at
   // the specified position is valid.
   nPositionOK = fnValidateInsertPosition( &bNewRoot,
                                           iLoadEntity,
                                           lpView,
                                           lpViewEntityCsr,
                                           nPosition );
   if ( nPositionOK < 0 )
   {
      fnOperationReturn( iLoadEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // call fnCreateEntity to create the entity
   nRC = fnCreateEntity( lpView, lpViewEntity, lpViewEntityCsr,
                         nPosition, bNewRoot );

   if ( plpRecord )
   {
      LPENTITYINSTANCE lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

      *plpRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
      if ( *plpRecord == 0 )
      {
         fnCreateAttributeRecord( lpCurrentTask,
                                  lpViewEntity, lpEntityInstance );
         *plpRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
      }
   }

   // Return success ... we've managed to create an entity.
   fnOperationReturn( iLoadEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=CountEntitiesForView
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CountEntitiesForView
//
//  PURPOSE:    To count occurrences of an Entity
//
//  PARAMETERS: zView          - View of the entity to be counted.
//              cpcEntityName  - Name of the entity to be counted.
//
//
//  RETURNS:    0 or greater  - Number of unhidden Entities.
//              zCALL_ERROR   - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG  OPERATION
CountEntitiesForView( zVIEW lpView, zCPCHAR cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zLONG             lCount = 0;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCountEntitiesForView, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iCountEntitiesForView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   if ( lpEntityInstance == UNSET_CSR )
      lpEntityInstance = fnEstablishCursorForView( lpViewEntityCsr );

   if ( lpEntityInstance )
   {
      // go to first instance
      while ( lpEntityInstance->hPrevTwin )
         lpEntityInstance = zGETPTR( lpEntityInstance->hPrevTwin );

      // scan the twin pointers and count all the unhidden instances
      while ( lpEntityInstance )
      {
         if ( lpEntityInstance->u.nInd.bHidden == FALSE )
            lCount++;

         lpEntityInstance = zGETPTR( lpEntityInstance->hNextTwin );
      }
   }

   fnOperationReturn( iCountEntitiesForView, lpCurrentTask );
   return( lCount );
}

//./ ADD NAME=DropEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DropEntity
//
//  PURPOSE:    To remove an entity and all of its descendents from the
//              current object instance.
//
//  PARAMETERS: lpView          - View of the entity to be deleted.
//              cpcEntityName   - Name of the entity to be deleted.
//              nReposition     - Indicates where to position the cursor
//                                after the delete.
//                                May be zREPOS_FIRST, zREPOS_LAST,
//                                zREPOS_NEXT, zREPOS_PREV or zREPOS_NONE.
//                                zREPOS_NONE or an invalid value will
//                                cause a zCURSOR_UNDEFINED return code.
//
//
//  RETURNS:    zCURSOR_... - Entity removed, and this is the status
//                            for setting the cursor.
//                            These return codes are shown under the
//                            operations for SetCursor...Entity.
//
//              zCALL_ERROR - Error removing entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
DropEntity( zVIEW lpView, zCPCHAR cpcEntityName, zSHORT nReposition )
{
   LPTASK            lpCurrentTask;
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpPrevVsn;
   zBOOL             bTemporal;
   zSHORT            nRC, nLvl;

   // Validate that the source view entity exists and has a cursor
   // value for copying, we call fnValidateInstanceParameters for the
   // source since it requires the same editing as an attribute retrieval
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntity,
                                      &lpViewEntityCsr,
                                      iDropEntity,
                                      lpView,
                                      cpcEntityName, 1 ) )
   {
      return( zCALL_ERROR );
   }

   // Get the entity instance to be deleted
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   fnRemoveAllChildTemporalVsns( lpView, lpEntityInstance, 0 );

   // If the entity instance is a temporal entity or the root of
   // a versioned Subobject, issue a cancel subobject prior to marking
   // the previous version as hidden
   nRC = 0;
   lpPrevVsn = zGETPTR( lpEntityInstance->hPrevVsn );
   if ( lpEntityInstance->u.nInd.bTemporal ||
        (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
   {
      // Set temporal entity indicator
      bTemporal = lpEntityInstance->u.nInd.bTemporal;
      nRC = CancelSubobject( lpView, cpcEntityName );

      // if the entity instance was temporal, then the cancel deleted the
      // temporal instance, issue the repositioning requested and return.
      if ( bTemporal )
      {
         // Now see if the user requested the cursor to be repositioned
         if ( nRC == 0 )
         switch ( nReposition )
         {
            case zREPOS_FIRST:
               nRC = SetCursorFirstEntity( lpView, cpcEntityName, 0 );
               break;

            case zREPOS_LAST:
               nRC = SetCursorLastEntity( lpView, cpcEntityName, 0 );
               break;

            case zREPOS_NEXT:
               nRC = SetCursorNextEntity( lpView, cpcEntityName, 0 );
               break;

            case zREPOS_PREV:
               nRC = SetCursorPrevEntity( lpView, cpcEntityName, 0 );
               break;

            default:
               nRC = zCURSOR_UNDEFINED;
         }

         // We think we're done, tell the caller everything is AOK!
         fnOperationReturn( iDropEntity, lpCurrentTask );
         return( nRC );
      }

      // Get the entity instance to be dropped after the cancel
      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   }

   nRC = 0; // Count the instance marked as hidden
   if ( lpEntityInstance )
   {
      nLvl = lpEntityInstance->nLevel;
      do
      {
         lpEntityInstance->u.nInd.bCreated =
            lpEntityInstance->u.nInd.bHidden = TRUE;

         // If the entity is linked to other entities, remove it from the link
         // chain.
         if ( lpEntityInstance->hNextLinked )
         {
            LPENTITYINSTANCE lpPrevLinked;

            // Find the EI that points to the current EI.
            for ( lpPrevLinked = zGETPTR( lpEntityInstance->hNextLinked );
                  zGETPTR( lpPrevLinked->hNextLinked ) != lpEntityInstance;
                  lpPrevLinked = zGETPTR( lpPrevLinked->hNextLinked ) )
            {
               // Nothing needs to be done here.
            }

            // Remove lpEntityInstance from the link chain.
            if ( zGETPTR( lpEntityInstance->hNextLinked ) == lpPrevLinked )
               lpPrevLinked->hNextLinked = 0;
            else
               lpPrevLinked->hNextLinked = lpEntityInstance->hNextLinked;

            // NULL out the hRecord pointer so that when the EI is eventually
            // freed we won't free the attribute data.
            // Note that we can do this because
            // a dropped entity instance doesn't change the database so we
            // don't need to keep seperate copies of the attributes.
            lpEntityInstance->hPersistRecord = 0;
            lpEntityInstance->hNextLinked = 0;
         }

         nRC++;
         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
      }  while ( lpEntityInstance && lpEntityInstance->nLevel > nLvl );

      lpViewOI->nHiddenCount += nRC;
      nRC = 0;
   }

   switch ( nReposition )
   {
      case zREPOS_FIRST:
         nRC = SetCursorFirstEntity( lpView, cpcEntityName, 0 );
         break;

      case zREPOS_LAST:
         nRC = SetCursorLastEntity( lpView, cpcEntityName, 0 );
         break;

      case zREPOS_NEXT:
         nRC = SetCursorNextEntity( lpView, cpcEntityName, 0 );
         break;

      case zREPOS_PREV:
         nRC = SetCursorPrevEntity( lpView, cpcEntityName, 0 );
         break;

      default:
         nRC = zCURSOR_UNDEFINED;
   }

#ifdef __RECLAIM__
   if ( nRC != zCURSOR_UNDEFINED && lpViewOI->nHiddenCount > __RECLAIM_LIMIT__ )
      fnReclaimHiddenInstances( lpViewOI );
#endif

   // We think we're done, tell the caller everything is AOK!
   fnOperationReturn( iDropEntity, lpCurrentTask );
   return( nRC );

} // DropEntity

//./ ADD NAME=CountEntitiesForViewUnderParent
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CountEntitiesForViewUnderParent
//
//  PURPOSE:    To count occurrences of an Entity under a parent.  If
//              cpcEntityName is specified then all entities under the
//              parent INCLUDING THE PARENT are counted.  If cpcEntityName
//              is NULL then all entities in the view are counted.
//
//  PARAMETERS: zView           - View of the entity to be counted.
//              cpcEntityName   - Name of the parent entity to be counted.
//
//
//  RETURNS:    0 or greater    - the number of unhidden Entities.
//              zCALL_ERROR     - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG  OPERATION
CountEntitiesForViewUnderParent( zVIEW   zView,
                                 zCPCHAR cpcEntityName,
                                 zCPCHAR cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zLONG             lCount = 0;
   zSHORT            nStopLevel;
   zSHORT            nLevel;

   if ( cpcScopingEntityName == 0 || cpcScopingEntityName[ 0 ] == 0 )
   {
      // No scoping entity is really just like...
      return( CountEntitiesForView( zView, cpcEntityName ) );
   }

   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpScopingViewEntity,
                                      &lpScopingViewEntityCsr,
                                      iCountEntitiesForViewUnderParent,
                                      zView,
                                      cpcScopingEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // If no current entity instance for scoping entity return zero.
   if ( lpScopingViewEntityCsr->hEntityInstance == 0 ||
        lpScopingViewEntityCsr->hEntityInstance == UNSET_CSR )
   {
      fnOperationReturn( iCountEntitiesForViewUnderParent, lpCurrentTask );
      return( 0 );
   }

   lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, zView, cpcEntityName, 0 );
   if ( lpViewEntity == 0 )
   {
      fnOperationReturn( iCountEntitiesForViewUnderParent, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpViewEntityCsr->hEntityInstance == 0 ||
        lpViewEntityCsr->hEntityInstance == UNSET_CSR )
   {
      // We need to find the first entity instance with a view entity
      // matching lpViewEntity.
      lpEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
      nStopLevel       = lpEntityInstance->nLevel;
      lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );

      while ( lpEntityInstance &&
              zGETPTR( lpEntityInstance->hViewEntity ) != lpViewEntity &&
              lpEntityInstance->nLevel > nStopLevel )
      {
         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
      }

      // If following condition is true then we never found a matching entity.
      if ( lpEntityInstance == 0 || lpEntityInstance->nLevel <= nStopLevel )
      {
         fnOperationReturn( iCountEntitiesForViewUnderParent, lpCurrentTask );
         return( zCALL_ERROR );
      }

      nLevel           = lpEntityInstance->nLevel;
      lpEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
   }
   else
   {

      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      nLevel           = lpEntityInstance->nLevel;
   }

   lpEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
   nStopLevel       = lpEntityInstance->nLevel;

   while ( lpEntityInstance )
   {
      if ( lpEntityInstance->u.nInd.bHidden == FALSE &&
           lpEntityInstance->nLevel == nLevel &&
           zGETPTR( lpEntityInstance->hViewEntity ) == lpViewEntity )
      {
         lCount++;
      }

      lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );

      if ( lpEntityInstance == 0 )
         break;

      if ( lpEntityInstance->nLevel <= nStopLevel )
         break;
   }

   fnOperationReturn( iCountEntitiesForViewUnderParent, lpCurrentTask );
   return( lCount );
}

//./ ADD NAME=DeleteEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeleteEntity
//
//  PURPOSE:    To delete an Entity and all of its descendents
//
//  PARAMETERS: lpView          - View of the entity to be deleted.
//              cpcEntityName   - Name of the entity to be deleted.
//              nReposition     - Indicates where to position the cursor
//                                after the delete.
//                                May be zREPOS_FIRST, zREPOS_LAST,
//                                zREPOS_NEXT, zREPOS_PREV or zREPOS_NONE.
//                                zREPOS_NONE or an invalid value will
//                                cause a zCURSOR_UNDEFINED return code.
//
//
//  RETURNS:    zCURSOR_... - Entity deleted, and this is the status
//                            for setting the cursor.
//                            These return codes are shown under the
//                            operations for SetCursor...Entity.
//
//              zCALL_ERROR - Error deleting entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
DeleteEntity( zVIEW lpView, zCPCHAR cpcEntityName, zSHORT nReposition )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpPrevVsn;
   zBOOL             bTemporal;
   zSHORT            nRC;

   // Validate that the source view entity exists and has a cursor
   // value for copying, we call fnValidateInstanceParameters for the
   // source since it requires the same editing as an attribute retrieval
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntity,
                                      &lpViewEntityCsr,
                                      iDeleteEntity,
                                      lpView,
                                      cpcEntityName, 1 ) )
   {
      return( zCALL_ERROR );
   }

   // Make sure entity can be deleted
   if ( lpViewEntity->bDelete == FALSE )
   {
      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 106, 0,
                        "Entity Deletion not allowed",
                        lpViewEntity->szName );
      fnOperationReturn( iDeleteEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Make sure that none of the child entities have restrictions on their
   // parents being deleted.  If bCheckRestrict is TRUE, then at least one
   // child entity has the bPRestrict flag set.
   if ( lpViewEntity->bCheckRestrict )
   {
      LPENTITYINSTANCE lpChild;

      // Loop through each of the child entity instances of lpEntityInstance.
      // When the level of the lpChild entity instance is no longer greater
      // than the level of the current entity instance it is not a child.
      for ( lpChild = zGETPTR( lpEntityInstance->hNextHier );
            lpChild;
            lpChild = zGETPTR( lpChild->hNextHier ) )
      {
         LPVIEWENTITY lpChildViewEntity = zGETPTR( lpChild->hViewEntity );

         if ( lpChildViewEntity->nLevel <= lpViewEntity->nLevel )
            break;

         // We check the entity levels because we only want child entities that
         // are direct children of lpViewEntity.
         if ( lpChildViewEntity->nLevel != lpViewEntity->nLevel + 1 )
            continue;

         // Don't bother checking a hidden child.
         if ( lpChild->u.nInd.bHidden )
            continue;

         // If the bPRestrict flag is set, then we can't delete the entity.
         if ( lpChildViewEntity->bPRestrict )
         {
            zCHAR szMsg[ 200 ];

            sprintf_s( szMsg, zsizeof( szMsg ), "Can't delete entity %s because of Restrict "
                       "constraint on child entity %s",
                       lpViewEntity->szName,
                       lpChildViewEntity->szName );
            TraceLineS( szMsg, "" );

            // "KZOEE106 - Rules violation"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 106, 0,
                              "Entity Deletion not allowed"
                              " - child is restricted",
                              lpViewEntity->szName );
            fnOperationReturn( iDeleteEntity, lpCurrentTask );
            return( zCALL_ERROR );
         }
      }

   } // if ( lpViewEntity->bCheckRestrict )...

#ifdef __ACTIVATE_CONSTRAINTS__
   // If there is an Entity Constraint for delete, see if it has
   // any objections about continuing.
   if ( lpViewEntity->bDeleteConstraint )
   {
      zSHORT   n;

      n = fnInvokeECEOperation( lpView, lpViewEntity, lpCurrentTask,
                                zECE_DELETE, 0 );
      // If constraint handler doesn't like something, get out.
      if ( n )
      {
         fnOperationReturn( iDeleteEntity, lpCurrentTask );
         return( n );
      }
   }
#endif

   // Get the entity instance to be deleted.
   fnRemoveAllChildTemporalVsns( lpView, lpEntityInstance, 0 );

   // If the entity instance is a temporal entity or the root of
   // a versioned Subobject, issue a cancel subobject prior to the
   // call to delete entity.
   nRC = 0;
   lpPrevVsn = zGETPTR( lpEntityInstance->hPrevVsn );
   if ( lpEntityInstance->u.nInd.bTemporal ||
        (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
   {
      // Set temporal entity indicator.
      bTemporal = lpEntityInstance->u.nInd.bTemporal;
      nRC = CancelSubobject( lpView, cpcEntityName );

      // If the entity instance was temporal, then the cancel deleted the
      // temporal instance, issue the repositioning requested and return.
      if ( bTemporal )
      {
         // Now see if the user requested the cursor to be repositioned.
         if ( nRC == 0 )
         switch ( nReposition )
         {
            case zREPOS_FIRST:
               nRC = SetCursorFirstEntity( lpView, cpcEntityName, 0 );
               break;

            case zREPOS_LAST:
               nRC = SetCursorLastEntity( lpView, cpcEntityName, 0 );
               break;

            case zREPOS_NEXT:
               nRC = SetCursorNextEntity( lpView, cpcEntityName, 0 );
               break;

            case zREPOS_PREV:
               nRC = SetCursorPrevEntity( lpView, cpcEntityName, 0 );
               break;

            default:
               nRC = zCURSOR_UNDEFINED;
         }

         // We think we're done, tell the caller everything is AOK!
         fnOperationReturn( iDeleteEntity, lpCurrentTask );
         return( nRC );
      }
   }

   // Call fnDeleteEntity if all is well so far.
   if ( nRC == 0 )
   {
      // Call fnDeleteEntity.
      nRC = fnDeleteEntity( lpView, lpViewEntity, lpViewEntityCsr,
                            nReposition, lpCurrentTask );
   }

#ifdef __RECLAIM__
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI = zGETPTR( lpViewCsr->hViewOI );
   if ( nRC != zCURSOR_UNDEFINED && lpViewOI->nHiddenCount > __RECLAIM_LIMIT__ )
      fnReclaimHiddenInstances( lpViewOI );
#endif

   // We think we're done, tell the caller everything is AOK!
   fnOperationReturn( iDeleteEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSameRelationshipInstance
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: fnSameRelationshipInstance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnSameRelationshipInstance( LPENTITYINSTANCE lpEntityInstance1,
                            LPENTITYINSTANCE lpEntityInstance2 )
{
   LPVIEWENTITY lpViewEntity1;
   LPVIEWENTITY lpViewEntity2;

   if ( lpEntityInstance1 == lpEntityInstance2 )
      return( 1 );  // Same relationship

   lpViewEntity1 = zGETPTR( lpEntityInstance1->hViewEntity );
   lpViewEntity2 = zGETPTR( lpEntityInstance2->hViewEntity );

   if ( lpViewEntity1->lERRelTok == lpViewEntity2->lERRelTok &&
        lpEntityInstance1->hParent && lpEntityInstance2->hParent )
   {
      if ( fnInstanceKeysMatch( lpEntityInstance1, lpEntityInstance2 ) &&
           fnInstanceKeysMatch( zGETPTR( lpEntityInstance1->hParent ),
                                zGETPTR( lpEntityInstance2->hParent ) ) )
      {
         return( 1 );
      }
   }

   return( 0 );  // Not the same relationship
}

//./ ADD NAME=fnDeleteEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: fnDeleteEntity
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnDeleteEntity( zVIEW            lpView,
                LPVIEWENTITY     lpViewEntity,
                LPVIEWENTITYCSR  lpViewEntityCsr,
                zSHORT           nReposition,
                LPTASK           lpCurrentTask )
{
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPENTITYINSTANCE  lpStartEntityInstance;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpLinkedInstance;
   LPENTITYINSTANCE  lpWorkInstance;
   zSHORT            nStartLevel;
   zSHORT            nLevel;
   zSHORT            nHiddenCount;
   zSHORT            nRC;

   // Get entity instance at which to start the delete.
   lpStartEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   lpEntityInstance = lpStartEntityInstance;
   nStartLevel = lpEntityInstance->nLevel;

   // First, exclude the entity instance, then go through and mark
   // all those as deleted whose delete rules support delete.
   lpLinkedInstance = lpEntityInstance;
   for ( ; ; )
   {
      LPVIEWENTITY lpTempViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

      // If the instance in question is already hidden, skip it
      // and all of its descendents since it may have been excluded or
      // moved BEFORE the Delete was issued.
      if ( lpEntityInstance->u.nInd.bHidden )
      {
         nLevel = lpEntityInstance->nLevel;
         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
         while ( lpEntityInstance &&
                 lpEntityInstance->nLevel > nLevel )
         {
            lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
         }
      }
      else
      if ( lpTempViewEntity->bPDelete == FALSE &&
           lpEntityInstance->nLevel > nStartLevel )
      {
         // If we get an entity instance which cannot be deleted,
         // skip it and all its descendents since they were
         // already excluded in the beginning of this operation.
         nLevel = lpEntityInstance->nLevel;
         if ( lpEntityInstance->u.nInd.bHidden == FALSE )
            fnExcludeEntityInstance( lpEntityInstance );

         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
         while ( lpEntityInstance &&
                 lpEntityInstance->nLevel > nLevel )
         {
            lpEntityInstance->u.nInd.bHidden = TRUE;
            lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
         }
      }
      else
      {
         // Mark all linked instances to this instance as deleted.
         lpWorkInstance = lpEntityInstance;
         for ( ; ; )
         {
            if ( lpWorkInstance->u.nInd.bHidden == FALSE )
               lpWorkInstance->u.nInd.bDeleted = TRUE;

            lpWorkInstance = zGETPTR( lpWorkInstance->hNextLinked );
            if ( lpWorkInstance == 0 || lpWorkInstance == lpEntityInstance )
               break;

#if 1
            // BEGIN - 9/30/93 - Bug in TZTENVRO Deletes
            // We are on a linked instance of a target delete, if the
            // instance in question is part of the same ViewOI and
            // represents a different relationship type or an instance
            // of the same relationship to a different parent, mark the
            // instance as Excluded.
            if ( lpWorkInstance->u.nInd.bHidden == FALSE )
            {
               if ( lpWorkInstance->hViewOI == lpEntityInstance->hViewOI &&
                    !fnSameRelationshipInstance( lpWorkInstance,
                                                 lpEntityInstance ) )
               {
                  lpWorkInstance->u.nInd.bExcluded = TRUE;
               }
            }
            // END - 9/30/93
#endif
         }

         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
      }

      // Issue the delete process for all linked instances to the initial
      // starting instance.
      if ( lpEntityInstance == 0 || lpEntityInstance->nLevel <= nStartLevel )
      {
         lpLinkedInstance = zGETPTR( lpLinkedInstance->hNextLinked );
         if ( lpLinkedInstance == 0 ||
              lpLinkedInstance == lpStartEntityInstance )
         {
            break;
         }
         else
         {
            lpEntityInstance = lpLinkedInstance;
            nStartLevel = lpEntityInstance->nLevel;
         }
      }
   }

   // After marking all necessary instances as deleted, call
   // fnHideExcludedDeletedInstances to hide ALL appropriate instances
   // from view.
   nHiddenCount = fnHideExcludedDeletedInstances( lpStartEntityInstance, 1 );

   // Update the hidden count in the View OI.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI = zGETPTR( lpViewCsr->hViewOI );
   lpViewOI->nHiddenCount += nHiddenCount;

// >>> RAD54278-1 BEGIN
   fnSetUpdateIndicator( lpViewOI, lpStartEntityInstance,
                         0, TRUE, FALSE, FALSE );
// >>> RAD54278-1 END

   // Now see if the user requested the cursor to be repositioned.
   switch ( nReposition )
   {
      case zREPOS_FIRST:
      case zREPOS_LAST:
      case zREPOS_NEXT:
      case zREPOS_PREV:
         nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                                  0, (zLONG) nReposition,
                                  0, 0, 0, 0, 0, 0,
                                  lpCurrentTask );
         break;

      default:
         nRC = zCURSOR_UNDEFINED;
   }

   return( nRC );
}

//./ ADD NAME=ExcludeEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ExcludeEntity
//
//  PURPOSE:    To exclude an Entity and all of its descendents
//
//  PARAMETERS: lpView          - View of the entity to be excluded.
//              cpcEntityName   - Name of the entity to be excluded.
//              nReposition     - Indicates where to position the cursor
//                                after the exclude.
//                                May be zREPOS_FIRST, zREPOS_LAST,
//                                zREPOS_NEXT, zREPOS_PREV or zREPOS_NONE.
//                                zREPOS_NONE or an invalid value will
//                                cause a zCURSOR_UNDEFINED return code.
//
//
//  RETURNS:    zCURSOR_... - Entity deleted, and this is the status
//                            for setting the cursor.
//                            These return codes are shown under the
//                            operations for SetCursor...Entity.
//
//              zCALL_ERROR - Error excluding entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
ExcludeEntity( zVIEW   lpView,
               zCPCHAR cpcEntityName,
               zSHORT  nReposition )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpPrevVsn;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nRC;

   // Validate that the source view entity exists and has a cursor value
   // for copying.  We call fnValidateInstanceParameters for the source
   // since it requires the same editing as an attribute retrieval
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntity,
                                      &lpViewEntityCsr,
                                      iExcludeEntity,
                                      lpView,
                                      cpcEntityName, 1 ) )
   {
      return( zCALL_ERROR );
   }

   // Make sure entity can be excluded
   if ( lpViewEntity->hParent && lpViewEntity->bExclude == FALSE )
   {
      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 106, 0,
                        "Entity Exclusion not allowed",
                        lpViewEntity->szName );
      fnOperationReturn( iExcludeEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

#ifdef __ACTIVATE_CONSTRAINTS__
   // If there is an Entity Constraint for exclude, see if it has
   // any objections about continuing.
   if ( lpViewEntity->bExcludeConstraint )
   {
      zSHORT   n;

      n = fnInvokeECEOperation( lpView, lpViewEntity, lpCurrentTask,
                                zECE_EXCLUDE, 0 );
      // If constraint handler doesn't like something, get out.
      if ( n )
      {
         fnOperationReturn( iExcludeEntity, lpCurrentTask );
         return( n );
      }
   }
#endif

   // Get the entity instance to be excluded
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   fnRemoveAllChildTemporalVsns( lpView, lpEntityInstance, 0 );

   // If the entity instance is a temporal entity or the root of
   // a versioned Subobject, issue a cancel subobject prior to the
   // call to exclude entity
   nRC = 0;
   lpPrevVsn = zGETPTR( lpEntityInstance->hPrevVsn );
   if ( lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot )
      nRC = CancelSubobject( lpView, cpcEntityName );

   // Call fnExcludeEntity
   nRC = fnExcludeEntity( lpView, lpViewEntity, lpViewEntityCsr,
                          nReposition, 1 );
#ifdef __RECLAIM__
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI = zGETPTR( lpViewCsr->hViewOI );
   if ( nRC != zCURSOR_UNDEFINED && lpViewOI->nHiddenCount > __RECLAIM_LIMIT__ )
      fnReclaimHiddenInstances( lpViewOI );
#endif

   // Whew, I think we've done it, return to caller
   fnOperationReturn( iExcludeEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnExcludeEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: fnExcludeEntity
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnExcludeEntity( zVIEW           lpView,
                 LPVIEWENTITY    lpViewEntity,
                 LPVIEWENTITYCSR lpViewEntityCsr,
                 zSHORT          nReposition,
                 zSHORT          bMarkExcluded )
{
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPENTITYINSTANCE  lpEntityInstance;
   zBOOL             bInvertedFound;
   zSHORT            nHiddenCount;
   zSHORT            nRC;


   // Exclude the entity instance by calling fnExcludeEntityInstance
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   bInvertedFound = fnExcludeEntityInstance( lpEntityInstance );

   // Now that the Exclude process is complete for the instance,
   // call fnHideDeletedExcludedInstances for the excluded instances Parent
   // This is done in the case of an include because linked instances of
   // this entity instances parents may have been as a result of structure
   // inversion.
   nHiddenCount = 0;
   if ( bInvertedFound )
      nHiddenCount = fnHideLinkedChildren( zGETPTR( lpEntityInstance->
                                                                  hParent ),
                                           bMarkExcluded );
   nHiddenCount += fnHideExcludedDeletedInstances( lpEntityInstance,
                                                   bMarkExcluded );

   // Update the hidden count in the View OI
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI = zGETPTR( lpViewCsr->hViewOI );
   lpViewOI->nHiddenCount += nHiddenCount;
   if ( bMarkExcluded )
// >>> RAD54278-1 BEGIN
      fnSetUpdateIndicator( lpViewOI, lpEntityInstance, 0, TRUE, FALSE, FALSE );
// >>> RAD54278-1 END

   // Now see if the user requested the cursor to be repositioned
   switch ( nReposition )
   {
      case zREPOS_FIRST:
         nRC = SetCursorFirstEntity( lpView, lpViewEntity->szName, 0 );
         break;

      case zREPOS_LAST:
         nRC = SetCursorLastEntity( lpView, lpViewEntity->szName, 0 );
         break;

      case zREPOS_NEXT:
         nRC = SetCursorNextEntity( lpView, lpViewEntity->szName, 0 );
         break;

      case zREPOS_PREV:
         nRC = SetCursorPrevEntity( lpView, lpViewEntity->szName, 0 );
         break;

      default:
         nRC = zCURSOR_UNDEFINED;
   }

   // Whew, I think we've done it, return to caller.
   return( nRC );
}

//./ ADD NAME=RelinkInstanceToInstance
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   RelinkInstanceToInstance
//
//  PURPOSE: This operation re-establishes links between two entity
//           instances either inter OR intra-object. It is an atomic
//           operation for single entity instances and DOES NOT
//           establish subobject links for matching children of the
//           entity instances passed!!!.  It is intended to be used
//           temporarily to re-establish linkes after loading
//           duplicate information from the repository ( until the
//           software can re-establish these links automatically at
//           som future point in time ).
//
//  PARAMETERS: zTgtView        - Target view for link
//              szTgtEntityName - Target entity for link
//              zSrcView        - Source view for link
//              szSrcEntityName - Source entity for link
//
//  RETURNS: 0           - Entity instances linked successfully
//           1           - Entity instances already linked
//           zCALL_ERROR - Error relinking entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
RelinkInstanceToInstance( zVIEW    zTgtView,
                          zCPCHAR  cpcTgtEntityName,
                          zVIEW    zSrcView,
                          zCPCHAR  cpcSrcEntityName )
{
   LPTASK           lpCurrentTask;
   LPTASK           lpSrcOwningTask;
   LPTASK           lpTgtOwningTask;
   LPVIEWOD         lpSrcViewOD;
   LPVIEWOD         lpTgtViewOD;
   LPVIEWCSR        lpViewCsr;
   LPVIEWOI         lpViewOI;
   LPVIEWENTITY     lpSrcViewEntity;
   LPVIEWENTITYCSR  lpSrcViewEntityCsr;
   LPVIEWENTITY     lpTgtViewEntity;
   LPVIEWENTITYCSR  lpTgtViewEntityCsr;
   LPENTITYINSTANCE lpSrcEntityInstance;
   LPENTITYINSTANCE lpTgtEntityInstance;
   zCHAR            szYN[ 2 ];
   zSHORT           nRC;

   // Validate parameters
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpSrcViewEntity,
                                      &lpSrcViewEntityCsr,
                                      iRelinkInstanceToInstance,
                                      zSrcView,
                                      cpcSrcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Validate parameters
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpTgtViewEntity,
                                      &lpTgtViewEntityCsr,
                                      iRelinkInstanceToInstance,
                                      zTgtView,
                                      cpcTgtEntityName,
                                      2 ) )  // we already called once
   {
      return( zCALL_ERROR );
   }

   // Make sure the target and source entity instances are the same
   // E/R Entity
   if ( lpSrcViewEntity->lEREntTok != lpTgtViewEntity->lEREntTok )
   {
      // "KZOEE125 - Target and Source Entities are not the same E/R Entity"
      fnIssueCoreError( lpCurrentTask, zSrcView, 8, 123, 0,
                        lpTgtViewEntity->szName,
                        lpSrcViewEntity->szName );

      fnOperationReturn( iRelinkInstanceToInstance, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure entity can be included
   if ( lpSrcViewEntity->bInclSrc == FALSE )
   {
      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, zSrcView, 8, 106, 0,
                        "Source Entity not includeble",
                        lpSrcViewEntity->szName );
      fnOperationReturn( iRelinkInstanceToInstance, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure that the LODs are in sync.  They are out of sync if the LODs
   // were built with different dates for the ER.  We won't bother performing
   // the check if either LOD doesn't have the ER date.  This allows old LODs
   // to work.
   lpSrcViewOD = zGETPTR( zSrcView->hViewOD );
   lpTgtViewOD = zGETPTR( zTgtView->hViewOD );
   if ( lpSrcViewOD->szER_Date[ 0 ] && lpTgtViewOD->szER_Date[ 0 ] &&
        zstrcmp( lpSrcViewOD->szER_Date, lpTgtViewOD->szER_Date ) != 0 )
   {
      zCHAR szMsg[ 200 ];

      TraceLineS( "ERROR: ", "The following LODs are out of sync and should be re-built" );

      sprintf_s( szMsg, zsizeof( szMsg ), "Out-of-sync error. Target LOD = %s, date = %s",
                 lpTgtViewOD->szName, lpTgtViewOD->szER_Date );
      TraceLineS( "   ==> ", szMsg );

      sprintf_s( szMsg, zsizeof( szMsg ), "Out-of-sync error. Source LOD = %s, date = %s",
                 lpSrcViewOD->szName, lpSrcViewOD->szER_Date );
      TraceLineS( "   ==> ", szMsg );

      SysReadZeidonIni( -1, "[Workstation]", "SyncER/LOD", szYN, zsizeof( szYN ) );
      if ( szYN[ 0 ] != 'N' )
      {
         // "KZOEE106 - Rules violation"
         fnIssueCoreError( lpCurrentTask, zSrcView, 8, 106, 0,
                           "LODs are potentially out of sync.  See trace for more.",
                           lpTgtViewOD->szName );

         fnOperationReturn( iRelinkInstanceToInstance, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   // Get the source and target task.
   lpViewCsr       = zGETPTR( zSrcView->hViewCsr );
   lpViewOI        = zGETPTR( lpViewCsr->hViewOI );
   lpSrcOwningTask = zGETPTR( lpViewOI->hTask );
   lpViewCsr       = zGETPTR( zTgtView->hViewCsr );
   lpViewOI        = zGETPTR( lpViewCsr->hViewOI );
   lpTgtOwningTask = zGETPTR( lpViewOI->hTask );

#if 0
   if ( lpSrcOwningTask != lpTgtOwningTask )
   {
      if ( lpSrcOwningTask != zGETPTR( AnchorBlock->hMainTask ) )
      {
         // "KZOEE131 - Attempt to include Subobject from another "
         // "           Application Task"
         fnIssueCoreError( zSrcView, lpView, 8, 131, 0, 0, 0 );
         fnOperationReturn( iRelinkInstanceToInstance, lpCurrentTask );
         return( zCALL_ERROR );
      }

      // lpViewOI is still set to the Tgt view OI.
      lpViewOI->hAllocTask = zGETHNDL( lpSrcOwningTask );
   }
#endif

   // Get the source and target instances
   lpSrcEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
   lpTgtEntityInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
   if ( !fnInstanceKeysMatch( lpTgtEntityInstance, lpSrcEntityInstance ) )
   {
      // "KZOEE117 - Entity Instance Keys do not match"
      fnIssueCoreError( lpCurrentTask, zSrcView, 8, 117, 0,
                        lpSrcViewEntity->szName,
                        lpTgtViewEntity->szName );
      fnOperationReturn( iRelinkInstanceToInstance, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = fnRelinkInstanceToInstance( lpTgtEntityInstance,
                                     lpSrcEntityInstance );


   // Return to caller
   fnOperationReturn( iRelinkInstanceToInstance, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetIncrementalUpdateFlags
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetIncrementalUpdateFlags
//
//  PURPOSE:    To reset the record ownership and relationship ownership
//              flags in an object instance and optionally mark the
//              instance as either created or persistent from a
//              database update point of view.
//
//  PARAMETERS: lpView        - View to instance to be reset
//
//              cpcEntityName - Optional Entity name, NULL to process all
//                              Entity types
//
//              nOption       - marking flags
//
//                zSET_INCR_CREATED         0x0002
//                     Mark all instances as created
//
//                zSET_INCR_PERSISTENT      0x0004
//                     Mark all instances as existing on database
//                     (i.e. not created)
//
//                zSET_INCR_UPDATED         0x0008
//                     Mark all instances as updated
//
//                zSET_INCR_NOT_UPDATED     0x0010
//                     Mark all instances as not updated
//
//                zSET_INCR_INCLUDED        0x0020
//                     Mark all instances as included
//
//                zSET_INCR_NOT_INCLUDED    0x0040
//                     Mark all instances as not included
//
//                zSET_INCR_CURSORPOS       0x0080
//                     Do setting of entity instance at the cursor
//                     position ONLY, requires EntityName
//
//                zSET_INCR_OWNERSHIP       0x0100
//                     Set record/relationship ownership
//
//                NOTE: The following 4 settings SHOULD NOT be used in
//                      Conjunction withe zSET_INCR_OWNERSHIP! If
//                      specified, they will be IGNORED!
//
//                zSET_INCR_RECORDOWNER     0x0200
//                     Force record ownership ON
//
//                zSET_INCR_NOT_RECORDOWNER 0x0400
//                     Force record ownership OFF
//
//                zSET_INCR_RELOWNER     0x1000
//                     Force relationship ownership ON
//
//                zSET_INCR_NOT_RELOWNER    0x2000
//                     Force relationship ownership OFF
//
//  RETURNS:    0           - Successful
//              zCALL_ERROR - Invalid view passed
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetIncrementalUpdateFlags( zVIEW   lpView,
                           zCPCHAR cpcEntityName,
                           zSHORT  nOption )
{
   zBOOL            bCreated;
   zBOOL            bPersist;
   zBOOL            bUpdated;
   zBOOL            bNotUpdated;
   zBOOL            bIncluded;
   zBOOL            bNotIncluded;
   zBOOL            bSingleEntity;
   zBOOL            bAttrUpdated;
   zBOOL            bAttrNotUpdated;
   LPTASK           lpCurrentTask;
   LPVIEWCSR        lpViewCsr;
   LPVIEWOI         lpViewOI;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpRootEntityInstance;
   LPENTITYINSTANCE lpEntityInstance;
   zBOOL            bMustCheckOIUpdated = FALSE;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetIncrementalUpdateFlags, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( nOption == 0 )
   {
      fnOperationReturn( iSetIncrementalUpdateFlags, lpCurrentTask );
      return( 0 );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   if ( cpcEntityName && cpcEntityName[ 0 ] )
   {
      if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                              lpView, cpcEntityName, 0 )) == 0 )
      {
         fnOperationReturn( iSetIncrementalUpdateFlags, lpCurrentTask );
         return( zCALL_ERROR );
      }

      bSingleEntity = ((nOption & zSET_INCR_CURSORPOS) != 0);
      if ( bSingleEntity )
      {
         lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
         if ( lpEntityInstance == UNSET_CSR )
         {
            fnEstablishCursorForView( lpViewEntityCsr );
            lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
         }

         if ( lpEntityInstance == 0 )
         {
            fnOperationReturn( iSetIncrementalUpdateFlags, lpCurrentTask );
            return( 0 );
         }
      }
   }
   else
   {
      bSingleEntity = FALSE;
      lpViewEntity = 0;
   }

   // Get the root of the object instance
   if ( bSingleEntity )
      lpRootEntityInstance = lpEntityInstance;
   else
      lpRootEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );

   if ( lpRootEntityInstance == 0 )
   {
      fnOperationReturn( iSetIncrementalUpdateFlags, lpCurrentTask );
      return( 0 );
   }

   bCreated  = ((nOption & zSET_INCR_CREATED ) != 0);
   if ( bCreated == FALSE )
   {
      bPersist = ((nOption & zSET_INCR_PERSISTENT) != 0);
      bUpdated = ((nOption & zSET_INCR_UPDATED) != 0);
      if ( bUpdated )
      {
         lpViewOI->bUpdated = lpViewOI->bUpdatedFile = TRUE;
         bNotUpdated = 0;
      }
      else
      {
         bNotUpdated = ((nOption & zSET_INCR_NOT_UPDATED) != 0);
      }
   }

   bIncluded = ((nOption & zSET_INCR_INCLUDED) != 0);
   if ( bIncluded == FALSE )
      bNotIncluded = ((nOption & zSET_INCR_NOT_INCLUDED) != 0);
   else
      bNotIncluded = 0;

   bAttrUpdated = ((nOption & zSET_INCR_ATTR_UPDATED) != 0);
   if ( bAttrUpdated == FALSE )
      bAttrNotUpdated = ((nOption & zSET_INCR_ATTR_NOT_UPDATED) != 0);

   // Loop through the entity instances.
   for ( lpEntityInstance = lpRootEntityInstance;
         lpEntityInstance;
         lpEntityInstance = bSingleEntity ? 0 :
                            zGETPTR( lpEntityInstance->hNextHier ) )
   {
      LPVIEWENTITY lpTempViewEntity;
      LPVIEWATTRIB lpViewAttrib;
      zBOOL        bWorkEntity; // indicate entity is work or derived

      lpTempViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

      // If lpViewEntity is not null, then we only want to change the flags
      // for EI's that match that view entity.
      if ( lpViewEntity && lpTempViewEntity != lpViewEntity )
         continue;  // They don't match so continue with next EI.

      bWorkEntity = FALSE;
      if ( lpTempViewEntity->bDerived || lpTempViewEntity->bDerivedPath ||
           lpTempViewEntity->hFirstDataRecord == 0 )
      {
         bWorkEntity = TRUE;
      }

      if ( bCreated )
      {
         lpEntityInstance->u.nInd.bCreated = TRUE;
         lpEntityInstance->u.nInd.bUpdated = FALSE;
         lpViewOI->bUpdatedFile = TRUE;
         if ( bWorkEntity == FALSE )
            lpViewOI->bUpdated = TRUE;
      }
      else
      {
         if ( bPersist )
         {
            lpEntityInstance->u.nInd.bCreated = FALSE;
            bMustCheckOIUpdated = TRUE;
         }

         if ( bUpdated )
         {
            lpEntityInstance->u.nInd.bUpdated = TRUE;
         }
         else
         if ( bNotUpdated )
         {
            lpEntityInstance->u.nInd.bUpdated = FALSE;
            bMustCheckOIUpdated = TRUE;

            /*
            HH, 2002.05.02
            This code was commented out for following reasons:
            - it is obviously wrong, the "created" flag is not turned on,
              but it is turned off.
            - to "correct" it would mean, that turn "updated" off for a
              deleted entity means to "undelete" it.

            If we would want that behaviour, then we should correct and
            re-activate the code. However, this is a runtime change which
            can influence existing applications.

            Removing the code is not a runtime change, because of the bug it
            did not have any significant effect.

            // We don't want the entities to be flagged as updated.  If
            // the current entity is flagged as deleted then we will turn
            // on the "created" flag so the EI is considered "dead".
            if ( lpEntityInstance->u.nInd.bDeleted )
            {
               lpEntityInstance->u.nInd.bCreated = FALSE; // wrong, = TRUE is correct
            }
            */
         }
      }

      if ( bIncluded && lpEntityInstance->hParent )
         lpEntityInstance->u.nInd.bIncluded = TRUE;
      else
      if ( bNotIncluded )
         lpEntityInstance->u.nInd.bIncluded = FALSE;

      // Now check to see if we need to update the attributes.
      if ( bAttrUpdated == FALSE && bAttrNotUpdated == FALSE )
         continue; // Nope--continue with the next EI.

      // Set attribute flags.
      for ( lpViewAttrib = zGETPTR( lpTempViewEntity->hFirstOD_Attrib );
            lpViewAttrib;
            lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         LPATTRIBFLAGS lpAttribFlags = fnGetAttribFlagsPtr( lpEntityInstance,
                                                            lpViewAttrib );

         // Set the update flag for the entity.  Since we KNOW that either
         // bAttrUpdated flag or bAttrNotUpdated flag is TRUE, and since they
         // can't both be TRUE, we can just set the update flag for the
         // attribute to bAttrUpdated.
         if ( lpAttribFlags )
            lpAttribFlags->u.bFlags.bUpdated = bAttrUpdated;
      }

      if ( bAttrUpdated && lpEntityInstance->u.nInd.bUpdated == FALSE )
      {
         lpEntityInstance->u.nInd.bUpdated = TRUE;
         lpViewOI->bUpdatedFile = TRUE;
         if ( bWorkEntity == FALSE )
            lpViewOI->bUpdated = TRUE;
      }
      else
      if ( bAttrNotUpdated && lpEntityInstance->u.nInd.bUpdated )
      {
         lpEntityInstance->u.nInd.bUpdated = FALSE;
         bMustCheckOIUpdated = TRUE;
      }
   }

   // if an update flag was removed, we have to re-check the
   //  OI update flags from scratch
   if ( bMustCheckOIUpdated )
      fnCheckOIUpdated( lpViewOI );

   // Everything has been set, return
   fnOperationReturn( iSetIncrementalUpdateFlags, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=GetIncrementalUpdateFlags
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetIncrementalUpdateFlags
//
//  PURPOSE:    To get the value of incremental flags for an entity.
//
//  PARAMETERS: lpView        - View to instance to be reset
//              cpcEntityName - Required Entity name.
//              nOption       - marking flags
//
//                zSET_INCR_CREATED         0x0002
//                     Check if entity instance is created
//
//                zSET_INCR_UPDATED         0x0008
//                     Check if entity instance is updated
//
//                zSET_INCR_INCLUDED        0x0020
//                     Check if entity instance is included
//
//                zSET_INCR_HANGING_ENTITY  0x0800
//                     Check if entity is a hanging entity.
//
//                zSET_INCR_TEMPORAL_ENTITY 0x4000
//                     Check if entity is a temporal entity.
//
//                zSET_INCR_DELETED         0x8000
//                     Check if entity is deleted.  Can only be used from
//                     a dbhandler.
//
//  RETURNS:    0           - Flag for entity not set.
//              1           - Flag is set.
//              zCALL_ERROR - Invalid view passed
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
GetIncrementalUpdateFlags( zVIEW   lpView,
                           zCPCHAR cpcEntityName,
                           zSHORT  nOption )
{
   LPTASK           lpCurrentTask;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpEntityInstance;
   zSHORT           nRC;

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iGetIncrementalUpdateFlags,
                                          lpView, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iGetIncrementalUpdateFlags, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   if ( lpEntityInstance == UNSET_CSR )
   {
      fnEstablishCursorForView( lpViewEntityCsr );
      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   }

   if ( lpEntityInstance == 0 )
   {
      fnOperationReturn( iGetIncrementalUpdateFlags, lpCurrentTask );
      return( 0 );
   }

   // Cast the flag to zUSHORT for IBM/AIX.
   switch ( (zUSHORT) nOption )
   {
      case zSET_INCR_CREATED:
         nRC = lpEntityInstance->u.nInd.bCreated;
         break;

      case zSET_INCR_UPDATED:
         nRC = lpEntityInstance->u.nInd.bUpdated;
         break;

      case zSET_INCR_INCLUDED:
         nRC = lpEntityInstance->u.nInd.bIncluded;
         break;

      case zSET_INCR_HANGING_ENTITY:
         nRC = lpEntityInstance->u.nInd.bHangingEntity;
         break;

      case zSET_INCR_TEMPORAL_ENTITY:
         nRC = lpEntityInstance->u.nInd.bTemporal;
         break;

      case zSET_INCR_DELETED:
         nRC = lpEntityInstance->u.nInd.bDeleted;
         break;

      default:
         //  "KZOEE120 - Invalid position parameter"
         fnIssueCoreError( lpCurrentTask, lpView, 16, 302, (zLONG) nOption, 0, 0 );
         nRC = zCALL_ERROR;
         break;
   }

   fnOperationReturn( iGetIncrementalUpdateFlags, lpCurrentTask );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTIONS:   Local functions used for entity creation/deletion, etc
//
/////////////////////////////////////////////////////////////////////////////

//./ ADD NAME=fnEstablishViewForInstance
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:    fnEstablishViewForInstance
//
//  RETURNS:     ViewEntityCsr established
//
//  PURPOSE:     To get to the appropriate subobject view for positioning
//               on an entity instance passed with the entity view cursor
//               passed. This function ASSUMES that the parameters passed
//               to it are VALID!.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
LPVIEWENTITYCSR
fnEstablishViewForInstance( zVIEW             lpView,
                            LPVIEWENTITYCSR   lpViewEntityCsr,
                            LPENTITYINSTANCE  lpEntityInstance )
{
   LPVIEWCSR        lpViewCsr;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpFirstViewEntityCsr;
   LPVIEWENTITYCSR  lpWorkCsr;
   LPVIEWENTITYCSR  lpRecursiveParent;
   LPVIEWENTITYCSR  lpRecursiveCsr;
   LPENTITYINSTANCE lpWorkInstance1;
   LPENTITYINSTANCE lpWorkInstance2;
   zBOOL            bReset = FALSE;  // initialize bReset

   lpViewEntity         = zGETPTR( lpEntityInstance->hViewEntity );
   lpViewCsr            = zGETPTR( lpView->hViewCsr );
   lpFirstViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );

   // If no entity cursor passed, establish one.
   lpRecursiveParent = 0;
   if ( lpViewEntityCsr == 0 )
   {
      fnResetView( lpView, 1 );
      bReset = TRUE;
      lpViewEntityCsr = lpFirstViewEntityCsr;
      lpViewEntityCsr += (lpViewEntity->nHierNbr - 1); // + (# * zsizeof( EntityCsr Structure ))
   }
   else
   if ( lpViewEntity != zGETPTR( lpViewEntityCsr->hViewEntity ) )
   {
      // The view entity of the instance does not match the view entity of
      // the csr, this means that the view entity of the cursor MUST be
      // a recursive parent cursor for the entity instance in question.
      lpRecursiveParent = lpViewEntityCsr;
      lpViewEntityCsr   = lpFirstViewEntityCsr;
      lpViewEntityCsr += (lpViewEntity->nHierNbr - 1);
   }

   // First, take care of the leveling problem.
   if ( lpViewEntityCsr->nLevel != lpEntityInstance->nLevel )
   {
      if ( bReset == FALSE )
      {
         fnResetView( lpView, 1 );
         bReset = TRUE;
      }

      while ( lpViewEntityCsr->nLevel < lpEntityInstance->nLevel )
      {
         // Until we get a cursor whose level matches the target instance
         // we want to establish, we continually find the highest level
         // recursive child which is greater than the current entity csr
         // level and set subobject views down to that child.
         lpWorkInstance1 = lpEntityInstance;
         for ( ; ; )
         {
            LPVIEWENTITY lpWorkViewEntity = zGETPTR( lpWorkInstance1->hViewEntity );

            if ( lpWorkViewEntity->bRecursive )
            {
               // Now find the cursor matching recursive child lpWorkInstance1
               lpRecursiveCsr = lpFirstViewEntityCsr;
               lpRecursiveCsr += (lpWorkViewEntity->nHierNbr - 1);

               // If we get a level match on the cursor, then accept the entity
               // at the cursor, otherwise keep looking.
               if ( lpRecursiveCsr->nLevel == lpWorkInstance1->nLevel )
                  break;
            }

            lpWorkInstance1 = zGETPTR( lpWorkInstance1->hParent );
         }

         lpWorkCsr = lpRecursiveCsr;
         lpWorkInstance2 = lpWorkInstance1;

         // Establish the cursors for the recursive child up to the
         // recursive parent.
         while ( lpWorkCsr &&
                 lpWorkCsr->nLevel == lpWorkInstance1->nLevel )
         {
            lpWorkCsr->hEntityInstance = zGETHNDL( lpWorkInstance1 );
         // if ( lpWorkCsr->hEntityInstance == UNSET_CSR )
         //    SysMessageBox( 0, "fnEstablishViewForInstance", "UNSET_CSR", -1 );

            lpWorkCsr = zGETPTR( lpWorkCsr->hParent );
            lpWorkInstance1 = zGETPTR( lpWorkInstance1->hParent );
         }

         // Now we set the view to the subobject in question and
         // continue until the target entity comes into view.
         fnSetViewToSubobject( lpView,
                               zGETPTR( lpWorkInstance2->hViewEntity ),
                               lpRecursiveCsr );
      }
   }

   // Now that we are done, establish any necessary cursors up the
   // parent chain.
   lpWorkCsr = lpViewEntityCsr;
   while ( lpWorkCsr &&
           lpWorkCsr->nLevel == lpEntityInstance->nLevel )
   {
      lpWorkCsr->hEntityInstance = zGETHNDL( lpEntityInstance );
   // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnEstablishViewForInstance2", "UNSET_CSR", -1 );

      lpEntityInstance = zGETPTR( lpEntityInstance->hParent );
      lpWorkCsr = zGETPTR( lpWorkCsr->hParent );
   }

   // If the ViewEntityCsr passed was the recursive parent cursor for the
   // instance being positioned on, do a SetViewToSubobject to position the
   // instance in the recursive parent cursor and reset the lpViewEntityCsr
   // back to the recursive parent.
   if ( lpRecursiveParent )
   {
      fnSetViewToSubobject( lpView, zGETPTR( lpViewEntityCsr->hViewEntity ),
                            lpViewEntityCsr );
      lpViewEntityCsr = lpRecursiveParent;
   }
   else
   {
      if ( bReset == FALSE )
         fnResetCursorForViewChildren( lpViewEntityCsr );
   }

   // Return success.
   return( lpViewEntityCsr );
}

//./ ADD NAME=fnValidateInsertPosition
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:    fnValidateInsertPosition
//
//  PURPOSE:     To validate that the entity type specified can be created
//               using the cursor position passed and that the position
//               parameter passed is valid.
//
//
//  ERRORS:      1. The position parameter is not
//                  zPOS_FIRST(1), zPOS_LAST(2), zPOS_BEFORE(4) (or zPOS_PREV),
//                  zPOS_AFTER(3) (or zPOS_NEXT).
//               2. The root of the instance has been deleted
//              *3. The entity is a new root and the previous
//                  root is not a child of the new entity
//              *4. The entity is not a new root and is either
//                  at the same hierarchical level as the
//                  old root or does not have a parent cursor
//                  and is therefore disjoint.
//               5. The entity's parent cursor points to a
//                  deleted entity instance and is therefore
//                  undefined
//               6. The entity's parent cursor points to a
//                  versioned entity instance
//               7. The entity's parent cursor is linked to another
//                  instance which would require spawning this entity
//                  create. This is currently not supported due to the
//                  complexity and overhead involved in creating a view
//                  and getting a cursor under which to spawn the create
//
//  RETURNS:    The nPosition input value - the insert is valid
//              zCALL_ERROR - Insert is invalid, exit immediately,
//                            error logged
//
//              ALSO: pnNewRoot is set for call to fnCreateEntity
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnValidateInsertPosition( zPSHORT         pnNewRoot,
                          zSHORT          iOperationID,
                          zVIEW           lpView,
                          LPVIEWENTITYCSR lpViewEntityCsr,
                          zSHORT          nPosition )
{
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpParentViewEntityCsr;
   LPVIEWENTITYCSR   lpRootViewEntityCsr;
   LPENTITYINSTANCE  lpViewRootEntityInstance;
   LPENTITYINSTANCE  lpTwinEntityInstance;
   LPENTITYINSTANCE  lpParentEntityInstance;

   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );
   lpViewCsr    = zGETPTR( lpViewEntityCsr->hViewCsr );
   lpViewOI     = zGETPTR( lpViewCsr->hViewOI );

   // Validate the position parameter
   switch ( nPosition )
   {
      case zPOS_BEFORE: // 4
      case zPOS_AFTER:  // 3
      case zPOS_FIRST:  // 1
      case zPOS_LAST:   // 2
         break;

      default:
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE120 - Invalid position parameter"
         fnIssueCoreError( lpTask, lpView, 8, 120, (zLONG) nPosition,
                           lpViewEntity->szName, 0 );
         return( zCALL_ERROR );
      }
   }

   //
   //  Insert errors:  See comment for function above
   //

   *pnNewRoot = 0;
   lpRootViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
   lpViewRootEntityInstance = zGETPTR( lpRootViewEntityCsr->hEntityInstance );

   if ( lpViewRootEntityInstance == UNSET_CSR )
      lpViewRootEntityInstance = fnEstablishCursorForView( lpRootViewEntityCsr );

   if ( lpViewRootEntityInstance == 0 )
      lpViewRootEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );

   // Gig, PATCH Attempt 1/8/93, go to parent of view root if it exists
   if ( lpViewRootEntityInstance && lpViewRootEntityInstance->hParent )
      lpViewRootEntityInstance = zGETPTR( lpViewRootEntityInstance->hParent );
   // Gig, PATCH Attempt 1/8/93, go to parent of view root if it exists

   if   ( lpViewRootEntityInstance &&
          lpViewRootEntityInstance->u.nInd.bHidden &&
          lpViewEntity->nLevel > 1 )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      //  "KZOEE252 - Root of view has been deleted"
      fnIssueCoreError( lpTask, lpView, 8, 252, 0,
                        lpViewEntity->szName, 0 );
      return( zCALL_ERROR );
   }

   // If the cursor for the current entity instance needs to be
   // established, establish it now.
   if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
   {
      fnEstablishCursorForView( lpViewEntityCsr );
   }

   lpParentEntityInstance = 0;
   lpTwinEntityInstance   = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // If the view entity has no parent OR the view cursor for the
   // entity is > 1 (recursive at the root level) look for a
   // parent entity instance.
   if ( lpViewEntity->hParent || lpViewEntityCsr->nLevel > 1 )
   {
      lpParentViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );

      // If null, we are at a subobject entity.
      if ( lpParentViewEntityCsr == 0 )
         lpParentEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );
      else
      {
         if ( lpParentViewEntityCsr->hEntityInstance == UNSET_CSR )
            fnEstablishCursorForView( lpParentViewEntityCsr );

         lpParentEntityInstance = zGETPTR( lpParentViewEntityCsr->hEntityInstance );

         // PATCH 7/1/93 Gig, We do not allow creation of entities which have
         // a parent unless the parent exists!!
         if ( lpParentEntityInstance == 0 )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            LPVIEWENTITY lpParentViewEntity = zGETPTR( lpParentViewEntityCsr->hViewEntity );

            //  "KZOEE122 - Attempt to create an Entity without a parent"
            fnIssueCoreError( lpTask, lpView, 8, 122, 0,
                              lpViewEntity->szName,
                              lpParentViewEntity->szName );
            return( zCALL_ERROR );
         }
      }

      // If the current parent cursor is on a previous version, issue
      // an error.
      if ( lpParentEntityInstance )
      {
         if ( lpParentEntityInstance->u.nInd.bHidden )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            //  "KZOEE254 - Entity cursor is undefined"
            fnIssueCoreError( lpTask, lpView, 8, 254, 0,
                              lpViewEntity->szName, 0 );
            return( zCALL_ERROR );   // Cursor undefined
         }
      }
   }
   else
   {
      lpParentViewEntityCsr  = 0;
      lpParentEntityInstance = 0;
   }

   // If a twin exists, make sure it is not the root of the complete
   // object instance and that the cursor is undefined because it is
   // pointing to a deleted entity or a previous version.
   if ( lpTwinEntityInstance ) // if a twin exists
   {
      if ( lpTwinEntityInstance == lpViewRootEntityInstance &&
           lpTwinEntityInstance->nLevel == 1 )
      {
         if ( lpViewOI->bMultipleRootLevels == FALSE )
         {
            if ( lpTwinEntityInstance->u.nInd.bHidden &&
                 lpTwinEntityInstance->u.nInd.bCreated )
            {
               LPENTITYINSTANCE  lpWrkInstance;

               lpWrkInstance = zGETPTR( lpTwinEntityInstance->hNextTwin );
               while ( lpWrkInstance && lpWrkInstance->u.nInd.bHidden )
                  lpWrkInstance = zGETPTR( lpWrkInstance->hNextTwin );

               if ( lpWrkInstance )
               {
                  LPTASK lpTask = zGETPTR( lpView->hTask );
                  //  "KZOEE121 - Attempt to insert twin to root of Instance"
                  fnIssueCoreError( lpTask, lpView, 8, 121, 0,
                                    lpViewEntity->szName, 0 );
                  return( zCALL_ERROR );
               }
            }
            else
            {
               LPTASK lpTask = zGETPTR( lpView->hTask );
               //  "KZOEE121 - Attempt to insert twin to root of Instance"
               fnIssueCoreError( lpTask, lpView, 8, 121, 0,
                                 lpViewEntity->szName, 0 );
               return( zCALL_ERROR );
            }
         }
      }
   }
   else   // No twin exists, see if entity has a parent
   if ( lpParentEntityInstance == 0 )  // Parent entity instance exists?
   {
      // No parent Entity Instance, make sure this can be a root.
      if ( lpViewRootEntityInstance )  // Another entity instance exists
      {
         LPVIEWENTITY lpRootViewEntity;

         lpRootViewEntity = zGETPTR( lpViewRootEntityInstance->hViewEntity );

         // If a root exists and entity has neither a twin nor a
         // parent cursor, then the new entity is disjoint unless the
         // entity is the parent of the current root cursor.
         if ( zGETPTR( lpRootViewEntity->hParent ) != lpViewEntity )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            //  "KZOEE122 - Attempt to create an Entity without a parent"
            fnIssueCoreError( lpTask, lpView, 8, 122, 0,
                              lpViewEntity->szName, 0 );
            return( zCALL_ERROR );  // Entity is disjoint
         }

         // Otherwise, the entity is a new root, set indicator
         *pnNewRoot = TRUE;
      }
   }

   // For every linked parent entity instance, make sure the
   // corresponding view object does not have this target entity
   // as either a child or direct parent to the linked parent found.
   if ( iOperationID != iIncludeSubobjectFromSubobject &&
        iOperationID != iCreateTemporalEntity &&
        iOperationID != iCreateEntity )
   {
      LPVIEWENTITY      lpSearchViewEntity;
      LPVIEWENTITY      lpLinkedParentViewEntity;
      LPENTITYINSTANCE  lpLinkedEntityInstance;

      if ( lpParentEntityInstance )
      {
         while ( lpParentEntityInstance->hPrevVsn )
            lpParentEntityInstance = zGETPTR( lpParentEntityInstance->hPrevVsn );
      }

      if ( lpParentEntityInstance && lpParentEntityInstance->hNextLinked )
      {
         lpLinkedEntityInstance = lpParentEntityInstance;
         while ( lpLinkedEntityInstance->hPrevVsn )
            lpLinkedEntityInstance = zGETPTR( lpLinkedEntityInstance->hPrevVsn );

         lpLinkedEntityInstance = zGETPTR( lpLinkedEntityInstance->hNextLinked );
         while ( lpLinkedEntityInstance != lpParentEntityInstance )
         {
            if ( lpLinkedEntityInstance->u.nInd.bHidden == FALSE )
            {
               LPVIEWENTITY lpParent;

               lpLinkedParentViewEntity = zGETPTR( lpLinkedEntityInstance->hViewEntity );
               lpParent = zGETPTR( lpLinkedParentViewEntity->hParent );

               // Check the linked parent view entity does not have
               // target entity as the parent
               if ( lpLinkedParentViewEntity->lERRelTok ==
                                             lpViewEntity->lERRelTok &&
                     lpParent &&
                     lpParent->lEREntTok == lpViewEntity->lERRelTok )
               {
                  LPTASK lpTask = zGETPTR( lpView->hTask );
                  // "KZOEE124 - Entity parent is included in another path "
                  // "           containing same entity type"
                  lpParent = zGETPTR( lpParentEntityInstance->hViewEntity );
                  fnIssueCoreError( lpTask, lpView, 8, 124, 0,
                                    lpViewEntity->szName,
                                    lpParent->szName );
                  return( zCALL_ERROR );
               }

               lpSearchViewEntity = zGETPTR( lpLinkedParentViewEntity->hNextHier );

               while ( lpSearchViewEntity &&
                       lpSearchViewEntity->nLevel >
                                          lpLinkedParentViewEntity->nLevel )
               {
                  if ( lpSearchViewEntity->lEREntTok ==
                                                   lpViewEntity->lEREntTok &&
                        lpSearchViewEntity->lERRelTok ==
                                                   lpViewEntity->lERRelTok )
                  {
                     LPTASK lpTask = zGETPTR( lpView->hTask );
                     // "KZOEE124 - Entity parent is included in another path "
                     // "           containing same entity type"
                     lpParent = zGETPTR( lpParentEntityInstance->hViewEntity );
                     fnIssueCoreError( lpTask, lpView, 8, 124, 0,
                                       lpViewEntity->szName,
                                       lpParent->szName );
                     return( zCALL_ERROR );
                  }

                  lpSearchViewEntity = zGETPTR( lpSearchViewEntity->hNextHier );
               }
            }

            lpLinkedEntityInstance = zGETPTR( lpLinkedEntityInstance->hNextLinked );
         }
      }
   }

   return( nPosition ); // return the nPosition value
}

//./ ADD NAME=fnSetEntityKey
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSetEntityKey
//
//  PURPOSE:    Sets the entity key to the next value.
//
//  RETURNS:    nothing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
void LOCALOPER
fnSetEntityKey( LPTASK           lpTask,
                LPTASK           lpOwningTask,
                LPENTITYINSTANCE lpEntityInstance )
{
   // lpOwningTask is not required, so set it.
   if ( lpOwningTask == 0 )
      lpOwningTask = lpTask;

// if ( lpOwningTask->lTaskID == AnchorBlock->lMainTaskID )
// if ( zGETHNDL( lpOwningTask ) == zGETHNDL( AnchorBlock) )
      zLOCK_MUTEX( zMUTEX_ENTITYKEY );

   lpEntityInstance->ulKey = ++(lpOwningTask->ulCurrentEntityKey);

// if ( lpOwningTask->lTaskID == AnchorBlock->lMainTaskID )
// if ( zGETHNDL( lpOwningTask ) == zGETHNDL( AnchorBlock) )
      zUNLOCK_MUTEX( zMUTEX_ENTITYKEY );
}

//./ ADD NAME=GetEntityKey
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   GetEntityKey
//
//  PURPOSE: To get the entity key of the entity at the current
//           cursor position.
//
//  RETURNS: Entity key
//           0 on error (cursor undefined, etc.).
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zULONG OPERATION
GetEntityKey( zVIEW lpView, zCPCHAR cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;

   // Validate parameters
   if ( fnValidateCursorParameters( &lpCurrentTask,
                                    &lpViewEntity,
                                    0,
                                    &lpViewEntityCsr,
                                    0,
                                    iGetEntityKey,
                                    lpView,
                                    cpcEntityName,
                                    0 ) != 0 )
   {
      return( 0 );
   }

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

#ifdef DEBUG
   if ( lpEntityInstance->ulKey == 0 )
      SysMessageBox( lpView, "Internal Zeidon Error",
                     "Entity Instance key is NULL", 1 );
#endif

   fnOperationReturn( iGetEntityKey, lpCurrentTask );
   return( lpEntityInstance->ulKey );
}

//./ ADD NAME=fnCreateEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCreateEntity
//
//  PURPOSE:    To create an Entity instance when all parameters required
//              for create have been validated (e.g. load situation).
//
//  RETURNS:    0 - Entity Instance created
//              zCALL_ERROR - Error creating entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
fnCreateEntity( zVIEW            lpView,
                LPVIEWENTITY     lpViewEntity,
                LPVIEWENTITYCSR  lpViewEntityCsr,
                zSHORT           nPosition,
                zSHORT           bNewRoot )
{
   LPTASK            lpAllocTask;
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWCSR         lpSearchViewCsr;
   LPVIEWSUBOBJECT   lpViewSubobject = 0;
   zPVOID            hEntityInstance = 0;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpRootEntityInstance;
   LPENTITYINSTANCE  lpParentEntityInstance;
   LPENTITYINSTANCE  lpTwinEntityInstance;

   lpViewCsr   = zGETPTR( lpViewEntityCsr->hViewCsr );
   lpViewOI    = zGETPTR( lpViewCsr->hViewOI );
   lpAllocTask = zGETPTR( lpViewOI->hTask );

   // Get twin to current entity.
   lpParentEntityInstance = 0;
   if ( lpViewEntityCsr->nLevel > 1 ) // parent exists at definition level
   {
      LPVIEWENTITYCSR lpParentViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );

      // If there is no parent entity cursor, then this entity is to be the new root
      // of a subobject view, the parent in this case is the parent of the view.
      if ( lpParentViewEntityCsr )
      {
         if ( lpParentViewEntityCsr->hEntityInstance == UNSET_CSR )
            SysMessageBox( 0, "fnCreateEntity", "UNSET_CSR", -1 );
         lpParentEntityInstance = zGETPTR( lpParentViewEntityCsr->hEntityInstance );
      }
      else
      {
         if ( lpViewCsr->hViewParentEntityInstance == UNSET_CSR )
            SysMessageBox( 0, "fnCreateEntity", "UNSET_CSR", -1 );
         lpParentEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );
      }
   }

   // Everything's OK for creating an entity instance, create it now.
   hEntityInstance = fnAllocDataspace( lpAllocTask->hFirstDataHeader,
                                       sizeof( EntityInstanceRecord ),
                                       1, 0, iEntityInstance );

   if ( hEntityInstance == 0 )
      return( zCALL_ERROR );

   lpEntityInstance = zGETPTR( hEntityInstance );
   fnSetEntityKey( lpAllocTask, 0, lpEntityInstance );

   // Set values in entity instance record.
   lpEntityInstance->hViewOI = lpViewCsr->hViewOI;

   // Set the view entity associated with the Entity, in the case of a
   // recursive subobject, the view entity pointed to by the cursor may not
   // in fact be the view entity we are creating, in this case we must get
   // the view entity from the subobject entry chained to this entry.
   lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
   if ( lpViewSubobject && lpViewSubobject->hRecursViewEntity &&
        lpViewEntityCsr == zGETPTR( lpViewSubobject->hRootCsr ) )
   {
      lpEntityInstance->hViewEntity = lpViewSubobject->hRecursViewEntity;
   }
   else
   {
      lpEntityInstance->hViewEntity = zGETHNDL( lpViewEntity );
      lpViewSubobject = 0;
   }

   lpEntityInstance->nLevel = lpViewEntityCsr->nLevel;

   // Get root of instance chain.
   lpRootEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );

   // The following check is made in CreateEntity, but is done again here so
   // calls from ActivateOI work after having to go down a subobject level.
   if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
      fnEstablishCursorForView( lpViewEntityCsr );

   // Get twin to current entity.
   lpTwinEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Set parent, twin, and hierarchical pointers.
   // If we are creating a new root, simply set hierarchical pointers.
   if ( bNewRoot || lpRootEntityInstance == 0 )
   {
      if ( lpRootEntityInstance )
      {
         lpEntityInstance->hNextHier = zGETHNDL( lpRootEntityInstance );
         lpRootEntityInstance->hPrevHier = hEntityInstance;
      }

      lpViewOI->hRootEntityInstance = hEntityInstance;
   }
   else
   {
      LPENTITYINSTANCE  lpNextTwin;
      LPENTITYINSTANCE  lpPrevTwin;

      lpEntityInstance->hParent = zGETHNDL( lpParentEntityInstance );
   // if ( lpEntityInstance->hParent == UNSET_CSR )
   //    SysMessageBox( 0, "fnCreateEntity", "UNSET_CSR", -1 );

      switch ( nPosition )
      {
         case zPOS_AFTER: // 3
            if ( lpTwinEntityInstance )
            {
               lpEntityInstance->hPrevTwin = zGETHNDL( lpTwinEntityInstance );
               lpEntityInstance->hNextTwin = lpTwinEntityInstance->hNextTwin;
               if ( lpTwinEntityInstance->hNextTwin )
               {
                  LPENTITYINSTANCE lpWorkInstance = zGETPTR( lpTwinEntityInstance->hNextTwin );

                  lpWorkInstance->hPrevTwin = hEntityInstance;
               }

               lpTwinEntityInstance->hNextTwin = hEntityInstance;
            }

            break;

         case zPOS_BEFORE: // 4
            if ( lpTwinEntityInstance )
            {
               lpEntityInstance->hNextTwin = zGETHNDL( lpTwinEntityInstance );
               lpEntityInstance->hPrevTwin = lpTwinEntityInstance->hPrevTwin;
               if ( lpTwinEntityInstance->hPrevTwin )
               {
                  LPENTITYINSTANCE lpWorkInstance = zGETPTR( lpTwinEntityInstance->hPrevTwin );

                  lpWorkInstance->hNextTwin = hEntityInstance;
               }

               lpTwinEntityInstance->hPrevTwin = hEntityInstance;
            }

            break;

         case zPOS_FIRST: // 1
            if ( lpTwinEntityInstance )
            {
               while ( lpTwinEntityInstance->hPrevTwin )
                  lpTwinEntityInstance = zGETPTR( lpTwinEntityInstance->hPrevTwin );

               lpEntityInstance->hNextTwin = zGETHNDL( lpTwinEntityInstance );
               lpTwinEntityInstance->hPrevTwin = hEntityInstance;

            }

            break;

         case zPOS_LAST: // 2
            if ( lpTwinEntityInstance )
            {
               while ( lpTwinEntityInstance->hNextTwin )
                  lpTwinEntityInstance = zGETPTR( lpTwinEntityInstance->hNextTwin );

               lpEntityInstance->hPrevTwin = zGETHNDL( lpTwinEntityInstance );
               lpTwinEntityInstance->hNextTwin = hEntityInstance;
            }

            break;
      }

      lpNextTwin = zGETPTR( lpEntityInstance->hNextTwin );

      // Set hierarchical pointers, if there is a next twin, then this
      // is ideal because setting the hierarchical cursor means simply
      // chaining this cursor to the next twin.
      if ( lpNextTwin )  // if there is a next twin
      {
         lpEntityInstance->hNextHier = zGETHNDL( lpNextTwin );
         lpEntityInstance->hPrevHier = lpNextTwin->hPrevHier;
         if ( lpEntityInstance->hPrevHier )
         {
            LPENTITYINSTANCE lpWorkInstance = zGETPTR( lpEntityInstance->hPrevHier );

            lpWorkInstance->hNextHier = hEntityInstance;
         }

         lpNextTwin->hPrevHier = hEntityInstance;
         if ( lpNextTwin == lpRootEntityInstance )
         {
            lpViewOI->hRootEntityInstance = hEntityInstance;
            lpRootEntityInstance = lpEntityInstance;
         }
      }
      else
      {
         // Otherwise, use the previous twin as the previous
         // hierarchical if the twin has no next hierarchical OR the
         // next hierarchical for the twin is not a child of the
         // twin.
         lpPrevTwin = zGETPTR( lpEntityInstance->hPrevTwin );
         if ( lpPrevTwin )
         {
            LPENTITYINSTANCE lpNextHier = zGETPTR( lpPrevTwin->hNextHier );

            if ( lpNextHier == 0 ||
                 lpNextHier->nLevel <= lpEntityInstance->nLevel )
            {
               lpEntityInstance->hPrevHier = zGETHNDL( lpPrevTwin );
               if ( lpPrevTwin->hNextHier )
               {
                  lpEntityInstance->hNextHier = lpPrevTwin->hNextHier;
                  lpNextHier->hPrevHier = hEntityInstance;
               }

               lpPrevTwin->hNextHier = hEntityInstance;
            }
            else
            {
               LPENTITYINSTANCE lpLastChild;
               LPENTITYINSTANCE lpNextHier;

               lpLastChild = zGETPTR( lpPrevTwin->hNextHier );
               lpNextHier  = zGETPTR( lpLastChild->hNextHier );

               while ( lpNextHier &&
                       lpNextHier->nLevel > lpEntityInstance->nLevel )
               {
                  lpLastChild = lpNextHier;
                  lpNextHier  = zGETPTR( lpLastChild->hNextHier );
               }

               lpEntityInstance->hPrevHier = zGETHNDL( lpLastChild );
               lpEntityInstance->hNextHier = lpLastChild->hNextHier;
               lpLastChild->hNextHier = hEntityInstance;

               if ( lpEntityInstance->hNextHier )
               {
                  lpNextHier = zGETPTR( lpEntityInstance->hNextHier );
                  lpNextHier->hPrevHier = hEntityInstance;
               }
            }
         }
         else
         {
            LPVIEWENTITY     lpWorkViewEnt;
            LPVIEWENTITYCSR  lpSearchCsr;
            LPENTITYINSTANCE lpSearchEntityInstance;
            LPENTITYINSTANCE lpNextHier;
            zSHORT           nParentHierNbr;
            zSHORT           nHierNbr;
            zSHORT           nLevel;

            // Here we take the first established cursor which is
            // hierarchically before the current entity instance
            // (knowing that it may in fact be the parent cursor itself)
            // and work forward from that entity instance in order to
            // find our correct hierarchical position

            lpSearchCsr = zGETPTR( lpViewEntityCsr->hPrevHier );
            while ( lpSearchCsr &&
                    (lpSearchCsr->hEntityInstance == 0 ||
                     lpSearchCsr->hEntityInstance == UNSET_CSR) )
            {
               lpSearchCsr = zGETPTR( lpSearchCsr->hPrevHier );
            }

            // If no cursor was found, then we must have hit the root of a
            // Subobject view, so use the parent root cursor for the view.
            if ( lpSearchCsr )
               lpSearchEntityInstance = zGETPTR( lpSearchCsr->hEntityInstance );
            else
               lpSearchEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );

            if ( lpSearchEntityInstance->nLevel >= lpEntityInstance->nLevel )
            {
               while ( lpSearchEntityInstance->hNextTwin )
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
            }

            // Now to set hierarchical cursor using a forward entity
            // instance search.
            lpNextHier     = zGETPTR( lpSearchEntityInstance->hNextHier );
            nLevel         = lpViewEntityCsr->nLevel;
            lpWorkViewEnt  = zGETPTR( lpEntityInstance->hViewEntity );
            nHierNbr       = lpWorkViewEnt->nHierNbr;

            // Found a condition (whacked account.vml) in which lpParentEntityInstance is null.  Don't
            // know why, but by protecting the following code, seems to be OK.  dks 2010.02.24
            if ( lpParentEntityInstance )
            {
               lpWorkViewEnt  = zGETPTR( lpParentEntityInstance->hViewEntity );
               nParentHierNbr = lpWorkViewEnt->nHierNbr;
            }
         // else
         //    nParentHierNbr = nHierNbr - 1;

            while ( lpNextHier )
            {
               lpWorkViewEnt = zGETPTR( lpNextHier->hViewEntity );

               if ( lpNextHier->nLevel > lpViewEntityCsr->nLevel ||
                    (lpNextHier->nLevel == lpViewEntityCsr->nLevel &&
                     lpWorkViewEnt->nHierNbr < nHierNbr) )
               {
                  lpSearchEntityInstance = lpNextHier;
                  lpNextHier             = zGETPTR( lpNextHier->hNextHier );
               }
               else
                  break;
            }

            lpEntityInstance->hPrevHier = zGETHNDL( lpSearchEntityInstance );
            lpEntityInstance->hNextHier = lpSearchEntityInstance->hNextHier;
            lpSearchEntityInstance->hNextHier = hEntityInstance;
            lpNextHier = zGETPTR( lpEntityInstance->hNextHier );
            if ( lpEntityInstance->hNextHier )
               lpNextHier->hPrevHier = hEntityInstance;

         }    // END else for previouse twin not end of inst chain
      }       // END else for new entity has no next twin
   }          // END else for this entity is not a new root

   // Finally, set the cursor for the entity and null the child cursors.
   lpViewEntityCsr->hEntityInstance = hEntityInstance;
// if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
//    SysMessageBox( 0, "fnCreateEntity", "UNSET_CSR", -1 );

   if ( lpTwinEntityInstance )
      fnNullCursorForViewChildren( lpViewEntityCsr );

   // Now that the entity had been created, update any other views
   // which have a cursor position on the new entity instance's
   // parent. If the entity is a new root, set the instance pointer
   // in all views, otherwise, if the entity cursor for the other
   // views is NULL, invalidate the pointer to force resetting in case
   // the newly created entity belongs in that cursor (we skip the
   // overhead of determining that fact now).
   lpSearchViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );
   if ( lpSearchViewCsr == lpViewCsr )
      lpSearchViewCsr = zGETPTR( lpSearchViewCsr->hNextViewCsr );

   if ( lpSearchViewCsr )
   {
      LPVIEWENTITYCSR  lpSearchViewEntityCsr;
      LPVIEWENTITY     lpSearchViewEntity;
      zSHORT           nRelativeCsr;
      zSHORT           nRelativeRecursCsr;

      nRelativeCsr = lpViewEntity->nHierNbr - 1;
      nRelativeRecursCsr = 0;

      // If there are other view cursors to this object instance, see if
      // the entity in question is a recursive entity type, this will
      // be true if either the ViewSubobject is set from above OR the
      // view entity for the current entity cursor is marked as recursive.
      // If so, then set nRelativeRecursCsr to the second entity cursor in
      // the subobject which may represent the entity in question.
      if ( lpViewSubobject )
      {
         if ( lpViewSubobject->hRecursViewEntity )
         {
            lpSearchViewEntity = zGETPTR( lpViewSubobject->hRecursViewEntity );
            nRelativeRecursCsr = lpSearchViewEntity->nHierNbr - 1;
         }
         else
         {
            TraceLine( "fnCreateEntity ERROR locating RecursViewEntity for lpViewSubobject: 0x%08x",
                       lpViewSubobject );
         }
      }
      else
      if ( lpViewEntity->bRecursive )
      {
         nRelativeRecursCsr = nRelativeCsr;
         lpSearchViewEntity = zGETPTR( lpViewEntity->hParent );
         while ( lpSearchViewEntity->lEREntTok != lpViewEntity->lEREntTok )
            lpSearchViewEntity = zGETPTR( lpSearchViewEntity->hParent );

         nRelativeCsr = lpSearchViewEntity->nHierNbr - 1;
      }

      while ( lpSearchViewCsr )
      {
         if ( lpSearchViewCsr != lpViewCsr )
         {
            lpSearchViewEntityCsr =
                           (LPVIEWENTITYCSR) (lpSearchViewCsr + 1);
            lpSearchViewEntityCsr += nRelativeCsr;
            if ( bNewRoot || lpRootEntityInstance == 0 )
            {
               lpSearchViewEntityCsr->hEntityInstance = hEntityInstance;
            // if ( lpSearchViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "fnCreateEntity2", "UNSET_CSR", -1 );
            }
            else
            {
               if ( lpSearchViewEntityCsr->nLevel == lpViewEntityCsr->nLevel &&
                    lpSearchViewEntityCsr->hEntityInstance == 0 )
               {
                  lpSearchViewEntityCsr->hEntityInstance = UNSET_CSR;
               // if ( lpSearchViewEntityCsr->hEntityInstance == UNSET_CSR )  // happens too much
               //    SysMessageBox( 0, "fnCreateEntity3", "UNSET_CSR", -1 );
               }
               else
               if ( nRelativeRecursCsr )
               {
                  lpSearchViewEntityCsr = (LPVIEWENTITYCSR) (lpSearchViewCsr + 1);
                  lpSearchViewEntityCsr += nRelativeRecursCsr;
                  if ( lpSearchViewEntityCsr->nLevel == lpViewEntityCsr->nLevel &&
                       lpSearchViewEntityCsr->hEntityInstance == 0 )
                  {
                     lpSearchViewEntityCsr->hEntityInstance = UNSET_CSR;
                  // if ( lpSearchViewEntityCsr->hEntityInstance == UNSET_CSR )  // happens too much
                  //    SysMessageBox( 0, "fnCreateEntity4", "UNSET_CSR", -1 );
                  }
               }
            }
         }

         lpSearchViewCsr = zGETPTR( lpSearchViewCsr->hNextViewCsr );
      }
   }

   fnAddEntityToDebugChange( lpEntityInstance );

   return( 0 );  // entity instance created successfully
}

//./ ADD NAME=fnValidateInstanceParameters
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnValidateInstanceParameters
//
//  PURPOSE:    To validate the standard parameters for an
//              attribute call
//              nFlags  0 - not for update
//                      1 - for update
//                      2 - second call from same operation
//                 0x8000 (32768) - do not report error
//
//  RETURNS:    0 - Attribute parameters valid
//              zCALL_ERROR - Error validating the attribute parameters
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT
fnValidateInstanceParameters( LPTASK           *plpReturnTask,
                              LPVIEWENTITY     *ppvReturnViewEntity,
                              LPVIEWENTITYCSR  *ppvReturnViewEntityCsr,
                              zSHORT           iOperationID,
                              zVIEW            lpView,
                              zCPCHAR          cpcEntityName,
                              zSHORT           nFlags )
{
   LPTASK            lpCurrentTask;
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpRootViewEntityCsr;
   LPENTITYINSTANCE  lpRootViewEntityInstance;
   LPENTITYINSTANCE  lpEntityInstance;

   if ( nFlags & 2 )
   {
      lpCurrentTask = *plpReturnTask;

      // Validate that the view passed is valid.
      if ( fnValidViewCsr( lpCurrentTask, lpView ) == 0 )
      {
         fnOperationReturn( iOperationID, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }
   else
   {
      // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
      if ( (lpCurrentTask = fnOperationCall( iOperationID, lpView,
                                             zVALID_VIEW_CSR )) == 0 )
      {
         return( zCALL_ERROR );
      }

      *plpReturnTask = lpCurrentTask;
   }

   // Get the view Object Instance for the view
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpView,
                                           cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Set View Entity and View Entity Cursor for caller.
   *ppvReturnViewEntity    = lpViewEntity;
   *ppvReturnViewEntityCsr = lpViewEntityCsr;

   // If intent is update, fail the call
   if ( lpView->bReadOnly && (nFlags & 1) &&
        lpViewEntity->bDerivedPath == FALSE )
   {
      // Special case, we will allow exclusion and inclusion of a root in a
      // read only view since the incremental update behavior of
      // excluding and including at the root level is null.
      if ( lpViewEntity->hParent ||
           (iOperationID != iIncludeSubobjectFromSubobject &&
            iOperationID != iExcludeEntity) )
      {
         if ( (nFlags & 0x8000) == 0 )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            // "KZOEE119 - Invalid View, View is Read Only"
            fnIssueCoreError( lpTask, lpView, 8, 119, 0, cpcEntityName, 0 );
         }
         else
         {
            TraceLineS( "KZOEE119 - Invalid View, View is Read Only for Entity: ",
                        cpcEntityName );
         }

         fnOperationReturn( iOperationID, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   lpRootViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
   // if Object instance is empty, issue an error
   if ( lpViewOI->hRootEntityInstance == 0 &&
        iOperationID != iCountEntitiesForView &&
        iOperationID != iRelinkAllSubobjectsForOI )
   {
      if ( (nFlags & 0x8000) == 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE253 - Entity cursor is NULL"
         fnIssueCoreError( lpTask, lpView, 8, 253, 0,
                           lpViewEntity->szName, 0 );
      }
      else
      {
         TraceLineS( "KZOEE253 - Entity cursor is NULL for Entity: ",
                     lpViewEntity->szName );
      }

      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If the root entity of the Object instance has been deleted,
   // issue an error.
   lpRootViewEntityInstance = zGETPTR( lpRootViewEntityCsr->hEntityInstance );
   if ( lpRootViewEntityInstance &&
        lpRootViewEntityInstance->u.nInd.bHidden )
   {
      if ( (nFlags & 0x8000) == 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE252 - Root of view has been deleted"
         fnIssueCoreError( lpTask, lpView, 8, 252, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ), 0 );
      }
      else
      {
         TraceLineS( "KZOEE252 - Root of view has been deleted for Entity: ",
                     fnEntityNameFromCsr( lpViewEntityCsr ) );
      }

      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If this is a relink all call, skip entity instance stuff.
   if ( iOperationID == iRelinkAllSubobjectsForOI )
      return( 0 );

   // Get Entity Instance pointer.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   if ( lpEntityInstance == UNSET_CSR )
      lpEntityInstance = fnEstablishCursorForView( lpViewEntityCsr );

   // If this is a count of the entities, exit now.
   if ( iOperationID == iCountEntitiesForView )
      return( 0 );

   // Now make sure a cursor exists for the view entity
   if ( lpEntityInstance == 0 )
   {
      if ( (nFlags & 0x8000) == 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE253 - Entity cursor is NULL"
         fnIssueCoreError( lpTask, lpView, 8, 253, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ), 0 );
      }
      else
      {
         TraceLineS( "KZOEE253 - Entity cursor is NULL for Entity: ",
                     fnEntityNameFromCsr( lpViewEntityCsr ) );
      }

      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpEntityInstance->nTableID != iEntityInstance )
   {
      TraceLineX( "(DGC) View   Cursor   = ", (zLONG) lpViewCsr );
      TraceLineX( "(DGC) Entity Cursor   = ", (zLONG) lpViewEntityCsr );
      TraceLineX( "(DGC) Entity Instance = ", (zLONG) lpEntityInstance );
      SysMessageBox( lpView, "DGC", "Invalid Table ID for entity instance", 1 );
      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpEntityInstance->u.nInd.bHidden )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      zSHORT nID;

      nID = 254;   // initialize error to: Entity cursor is undefined

      // The error in question is undefined UNLESS ALL of the
      // entity instances of the highest hidden parent are also hidden.
      while ( lpEntityInstance->hParent )
      {
         LPENTITYINSTANCE lpParent = zGETPTR( lpEntityInstance->hParent );

         if ( lpParent->u.nInd.bHidden == FALSE )
            break;

         lpEntityInstance = lpParent;
      }

      while ( lpEntityInstance->hPrevTwin )
         lpEntityInstance = zGETPTR( lpEntityInstance->hPrevTwin );

      while ( lpEntityInstance )
      {
         if ( lpEntityInstance->u.nInd.bHidden == FALSE )
            break;

         lpEntityInstance = zGETPTR( lpEntityInstance->hNextTwin );
      }

      // If no potentially visible entity instances were found, issue
      // the entity instance is NULL error.
      if ( (nFlags & 0x8000) == 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         if ( lpEntityInstance == 0 )
         {
            //  "KZOEE253 - Entity cursor is NULL"
            fnIssueCoreError( lpTask, lpView, 8, 253, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ), 0 );
         }
         else
         {
            //  "KZOEE254 - Entity cursor is undefined"
            fnIssueCoreError( lpTask, lpView, 8, 254, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ), 0 );
         }
      }
      else
      {
         if ( lpEntityInstance == 0 )
         {
            TraceLineS( "KZOEE253 - Entity cursor is NULL for Entity: ",
                        fnEntityNameFromCsr( lpViewEntityCsr ) );
         }
         else
         {
            TraceLineS( "KZOEE254 - Entity cursor is undefined for Entity: ",
                        fnEntityNameFromCsr( lpViewEntityCsr ) );
         }
      }

      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }
   else
   if ( lpEntityInstance->u.nInd.bPrevVersion && lpEntityInstance->hNextVsn )
   {
      if ( (nFlags & 0x8000) == 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE255 - Attempt to update a previous entity version"
         fnIssueCoreError( lpTask, lpView, 8, 255, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ), 0 );
      }
      else
      {
         TraceLineS( "KZOEE255 - Attempt to update a previous entity version for Entity: ",
                     fnEntityNameFromCsr( lpViewEntityCsr ) );
      }

      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Next make sure we are not processing hierarchically.
   if ( lpViewCsr->hHierRootEntityInstance )
   {
      if ( (nFlags & 0x8000) == 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         // "KZOEE264 - Invalid Operation for Hierarchical processing"
         fnIssueCoreError( lpTask, lpView, 8, 264, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ), 0 );
      }
      else
      {
         TraceLineS( "KZOEE264 - Invalid Operation for Hierarchical processing for Entity: ",
                     fnEntityNameFromCsr( lpViewEntityCsr ) );
      }

      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Everything is AOK, return.
   return( 0 );
}

//./ ADD NAME=fnHideLinkedChildren
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnHideLinkedChildren
//
//  PURPOSE:    To recursively process all linked instances and children
//              of linked instances for an entity instance which is being
//              marked as hidden due to a delete or an exclude
//
//  RETURNS:    zSHORT - Number of instances hidden
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnHideLinkedChildren( LPENTITYINSTANCE lpStartEntityInstance,
                      zSHORT bMarkExcluded )
{
   zSHORT            nHidden;
   zSHORT            nLevel;
   LPENTITYINSTANCE  lpLinked;
   LPENTITYINSTANCE  lpNextHier;

   nHidden  = 0;
   nLevel   = lpStartEntityInstance->nLevel;
   lpLinked = zGETPTR( lpStartEntityInstance->hNextLinked );

   // For every linked instance to the starting instance passed,
   // mark it and all its children as hidden IF they are not already hidden
   // and they are marked either for delete or exclude. Recursively call
   // ourself for every child of a linked instance marked for delete which
   // is itself also marked for delete.
   while ( lpLinked && lpLinked != lpStartEntityInstance )
   {
      if ( lpLinked->u.nInd.bHidden == FALSE &&
           (lpLinked->u.nInd.bExcluded || lpLinked->u.nInd.bDeleted) )
      {
         lpLinked->u.nInd.bHidden = TRUE;
         if ( bMarkExcluded == FALSE )
            lpLinked->u.nInd.bExcluded = FALSE;

         nHidden++;
         lpNextHier = zGETPTR( lpLinked->hNextHier );
         while ( lpNextHier && lpNextHier->nLevel > lpLinked->nLevel )
         {
            if ( lpNextHier->u.nInd.bHidden == FALSE )
            {
               if ( lpNextHier->hNextLinked &&
                    (lpNextHier->u.nInd.bExcluded ||
                     lpNextHier->u.nInd.bDeleted) )
               {
                  nHidden += fnHideLinkedChildren( lpNextHier,
                                                   bMarkExcluded );
               }

               lpNextHier->u.nInd.bHidden = TRUE;
               nHidden++;
            }

            lpNextHier = zGETPTR( lpNextHier->hNextHier );
         }
      }

      lpLinked = zGETPTR( lpLinked->hNextLinked );
   }

   // Return number of instances hidden
   return( nHidden );
}

//./ ADD NAME=fnHideExcludedDeletedInstances
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnHideExcludedDeletedInstances
//
//  PURPOSE:    To go through an entity instance and its children and
//              mark all the instances as hidden.
//
//  RETURNS:    zSHORT - Number of instances hidden
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnHideExcludedDeletedInstances( LPENTITYINSTANCE lpStartEntityInstance,
                                zSHORT           bMarkExcluded )
{
   zSHORT            nHidden;
   zSHORT            nLevel;

   nHidden = 0;
   nLevel = lpStartEntityInstance->nLevel;

   do
   {
      if ( lpStartEntityInstance->u.nInd.bHidden == FALSE )
      {
         lpStartEntityInstance->u.nInd.bHidden = TRUE;
         nHidden++;

         // Call the recursive function to mark any linked instances
         // as hidden which were deleted or excluded by the calling
         // operation
         if ( (lpStartEntityInstance->u.nInd.bDeleted ||
               lpStartEntityInstance->u.nInd.bExcluded) &&
              lpStartEntityInstance->hNextLinked )
         {
            nHidden += fnHideLinkedChildren( lpStartEntityInstance,
                                             bMarkExcluded );
         }

         if ( bMarkExcluded == FALSE )
            lpStartEntityInstance->u.nInd.bExcluded = FALSE;
      }

      lpStartEntityInstance = zGETPTR( lpStartEntityInstance->hNextHier );

   }  while ( lpStartEntityInstance &&
              lpStartEntityInstance->nLevel > nLevel );

   return( nHidden );
}

//./ ADD NAME=fnExcludeEntityInstance
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnExcludeEntityInstance
//
//  PURPOSE:    To mark an entity instance as included and all of the
//              subordinate instances as hidden and return the
//              next hierarchical entity instance after the last
//              descendent.
//
//  RETURNS:    0 - Success
//              1 - Success, Inverted parents found
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zBOOL
fnExcludeEntityInstance( LPENTITYINSTANCE lpEntityInstance )
{
   zBOOL             bInvertedFound;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpWrkViewEntity;
   LPENTITYINSTANCE  lpLinkedInstance;
   LPENTITYINSTANCE  lpLinkedParent;
   LPENTITYINSTANCE  lpWrkInstance;


   // Init inversion flag
   bInvertedFound = 0;

   // Get entity instance at which to start the exclude and mark the
   // Entity instance as excluded. Then go through and mark the entity
   // instance and all of its subordinates as hidden
   lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

   if ( lpEntityInstance->hParent == 0 )
      return( bInvertedFound );

   lpEntityInstance->u.nInd.bExcluded = TRUE;

   // Now that the simple exclusion is done, go through all linked
   // instances and mark all linked instances as excluded which represent
   // the same relationship to the same parent.
   lpLinkedInstance = zGETPTR( lpEntityInstance->hNextLinked );
   if ( lpLinkedInstance == 0 )
      return( bInvertedFound );

   while ( lpLinkedInstance != lpEntityInstance )
   {
      LPVIEWENTITY lpLinkedViewEntity = zGETPTR( lpLinkedInstance->hViewEntity );

      // We have a linked instance, for each instance see if the
      // corresponding view entity represents the same relationship
      // to the same parent instance OR has an inversion of the
      // same relationship to the same parent instance.
      // First, see if the instance is marked as deleted from
      // a delete entity call
      if ( lpLinkedViewEntity->lERRelTok == lpViewEntity->lERRelTok &&
           lpLinkedInstance->hParent )
      {
         LPENTITYINSTANCE hLinkedParent;

         // The linked instance represents the same relationship ...
         // see if its parent is a linked instance to the current parent.
         lpLinkedParent = zGETPTR( lpLinkedInstance->hParent );
         hLinkedParent  = lpLinkedParent->hNextLinked;

         if ( hLinkedParent )
         {
            while ( hLinkedParent != lpEntityInstance->hParent &&
                    hLinkedParent != lpLinkedInstance->hParent )
            {
               lpLinkedParent = zGETPTR( hLinkedParent );
               hLinkedParent  = lpLinkedParent->hNextLinked;
            }
         }

         if ( hLinkedParent == lpEntityInstance->hParent )
            lpLinkedInstance->u.nInd.bExcluded = TRUE;
      }

      // See if the structure is inverted for a linked instance
      lpWrkViewEntity = zGETPTR( lpLinkedViewEntity->hNextHier );
      while ( lpWrkViewEntity &&
              lpWrkViewEntity->nLevel > lpLinkedViewEntity->nLevel )
      {
         LPENTITYINSTANCE lpParent;

         if ( lpWrkViewEntity->hParent == lpLinkedInstance->hViewEntity &&
              lpWrkViewEntity->lERRelTok == lpViewEntity->lERRelTok )
         {
            // We've found an inverted structure in the linked entity
            // instance, now go and mark the child entity instance
            // which is linked to the source entity instances parent
            // as excluded.
            lpParent      = zGETPTR( lpEntityInstance->hParent );
            lpWrkInstance = zGETPTR( lpParent->hNextLinked );
            while (  lpWrkInstance && lpWrkInstance != lpParent )
            {
               if ( zGETPTR( lpWrkInstance->hParent ) == lpLinkedInstance )
               {
                  lpWrkInstance->u.nInd.bExcluded = TRUE;
                  bInvertedFound = TRUE;
               }

               lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
            }
         }

         lpWrkViewEntity = zGETPTR( lpWrkViewEntity->hNextHier );
      }

      lpLinkedInstance = zGETPTR( lpLinkedInstance->hNextLinked );

   } // while...

   // Whew, I think we've done it, return to caller.
   return( bInvertedFound );
}

//./ ADD NAME=fnSpawnCreateEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSpawnCreateEntity
//
//  PURPOSE:    This function checks to see if a created entity needs
//              spawning anywhere under linked parents.  If so, the
//              spawning is done.
//
//  RETURNS:    0   - Success
//             otw  - Error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnSpawnCreateEntity( zVIEW            lpView,
                     LPENTITYINSTANCE lpEntityInstance )
{
   LPENTITYINSTANCE lpParentInstance;
   LPENTITYINSTANCE lpLinkedInstance;
   LPVIEWENTITY     lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
   zSHORT           nRC = 0;

   // START OF CREATE ENTITY SPAWNING CODE
   // Now that the entity has been created, see if we need to call
   // fnCreateEntity to spawn the create of this same entity under a
   // different parent in the same view object. To do this, we check all
   // linked instances of the parent entity type from the same object
   // instance and see if that linked instance has a definition level
   // non-derived child entity type representing the current entity type.
   // If we find a spawn condition, we create a new view for spawning,
   // create the spawned instance in the appropriate position and
   // link it to the newly created instance.
   if ( lpEntityInstance->hParent == 0 || lpViewEntity->bDerived )
      return( nRC );

   lpParentInstance = zGETPTR( lpEntityInstance->hParent );
   for ( lpLinkedInstance = zGETPTR( lpParentInstance->hNextLinked );
         nRC == 0 && lpLinkedInstance && lpLinkedInstance != lpParentInstance;
         lpLinkedInstance = zGETPTR( lpLinkedInstance->hNextLinked ) )
   {
      LPVIEWENTITY lpParentViewEntity;
      LPVIEWENTITY lpRecursViewEntity;
      LPVIEWENTITY lpSearchViewEntity;

      if ( lpLinkedInstance->u.nInd.bHidden )
         continue;

      // If we find a linked parent in ANY object instance, go through the
      // child entity types of the linked parent and see if one exists which
      // matches the current entity type.
      lpParentViewEntity = zGETPTR( lpLinkedInstance->hViewEntity );
      lpRecursViewEntity = lpParentViewEntity;
      if ( lpParentViewEntity->bRecursive )
      {
         do
         {
            lpParentViewEntity = zGETPTR( lpParentViewEntity->hParent );
         }  while ( lpParentViewEntity->lEREntTok !=
                                              lpRecursViewEntity->lEREntTok );
      }

      for ( lpSearchViewEntity = zGETPTR( lpParentViewEntity->hNextHier );
            lpSearchViewEntity &&
               lpSearchViewEntity->nLevel > lpParentViewEntity->nLevel;
            lpSearchViewEntity = zGETPTR( lpSearchViewEntity->hNextHier ) )
      {
         zVIEW            lpSpawnView;
         LPVIEWOI         lpLinkedOI;
         LPVIEWCSR        lpLinkedFirstViewCsr;
         zSHORT           nSpawnPosition;
         LPVIEWENTITYCSR  lpSearchViewEntityCsr;
         LPVIEWENTITYCSR  lpWorkViewEntityCsr;
         LPENTITYINSTANCE lpLinkedTwin;
         LPENTITYINSTANCE lpSpawnedInstance;

         if ( zGETPTR( lpSearchViewEntity->hParent ) != lpParentViewEntity )
            continue;

         if ( lpSearchViewEntity->bDerived )
            continue;

         if ( lpSearchViewEntity->lERRelTok != lpViewEntity->lERRelTok )
            continue;

         if ( lpSearchViewEntity->bRelLink1 != lpViewEntity->bRelLink1 )
            continue;

         if ( !fnInstanceVersionRootMatch( lpParentInstance, lpLinkedInstance ) )
            continue;

         lpLinkedOI = zGETPTR( lpLinkedInstance->hViewOI );
         lpLinkedFirstViewCsr = zGETPTR( lpLinkedOI->hFirstViewCsr );

         // We've found a spawn condition, create a spawn view and do the spawn.
         nRC = CreateViewFromViewForTask( &lpSpawnView,
                                          zGETPTR( lpLinkedFirstViewCsr->hView ), 0 );
         if ( nRC )
            continue;

         // Establish target cursors up the parent chain.
         lpSearchViewEntityCsr =
               fnEstablishViewForInstance( lpSpawnView, 0, lpLinkedInstance );
         if ( lpParentViewEntity != lpRecursViewEntity )
         {
            fnSetViewToSubobject( lpSpawnView,
                                  lpRecursViewEntity,
                                  lpSearchViewEntityCsr );
            while ( zGETPTR( lpSearchViewEntityCsr->hViewEntity ) != lpParentViewEntity )
            {
               lpSearchViewEntityCsr = zGETPTR( lpSearchViewEntityCsr->hParent );
            }
         }

         // Now that the cursors are established up the parent chain, create a
         // cursor for the twin of the current instance under the linked parent.
         // First, find the view cursor for the target of the spawned create.
         lpWorkViewEntityCsr = zGETPTR( lpSearchViewEntityCsr->hNextHier );
         while ( zGETPTR( lpWorkViewEntityCsr->hViewEntity ) !=
                                                         lpSearchViewEntity )
         {
            lpWorkViewEntityCsr = zGETPTR( lpWorkViewEntityCsr->hNextHier );
         }

         nSpawnPosition = zPOS_AFTER;
         if ( lpEntityInstance->hNextTwin )
         {
            LPENTITYINSTANCE lpNextTwin;

            lpNextTwin   = zGETPTR( lpEntityInstance->hNextTwin );
            lpLinkedTwin = zGETPTR( lpNextTwin->hNextLinked );
            while ( lpLinkedTwin )
            {
               if ( zGETPTR( lpLinkedTwin->hParent ) == lpLinkedInstance )
                  break;

               if ( lpLinkedTwin->hNextLinked == lpEntityInstance->hNextTwin )
               {
                  lpLinkedTwin = 0;
                  break;
               }

               lpLinkedTwin = zGETPTR( lpLinkedTwin->hNextLinked );
            }

            if ( lpLinkedTwin )
            {
               lpWorkViewEntityCsr->hEntityInstance = zGETHNDL( lpLinkedTwin );
            // if ( lpWorkViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "fnSpawnCreateEntity", "UNSET_CSR", -1 );

               nSpawnPosition = zPOS_BEFORE;
            }
         }
         else
         if ( lpEntityInstance->hPrevTwin )
         {
            LPENTITYINSTANCE lpPrevTwin;

            lpPrevTwin   = zGETPTR( lpEntityInstance->hPrevTwin );
            lpLinkedTwin = zGETPTR( lpPrevTwin->hNextLinked );
            while ( lpLinkedTwin )
            {
               if ( zGETPTR( lpLinkedTwin->hParent ) == lpLinkedInstance )
                  break;

               if ( lpLinkedTwin->hNextLinked == lpEntityInstance->hPrevTwin )
               {
                  lpLinkedTwin = 0;
                  break;
               }

               lpLinkedTwin = zGETPTR( lpLinkedTwin->hNextLinked );
            }

            if ( lpLinkedTwin )
            {
               lpWorkViewEntityCsr->hEntityInstance = zGETHNDL( lpLinkedTwin );
            // if ( lpWorkViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "fnSpawnCreateEntity2", "UNSET_CSR", -1 );
            }
         }

         // For now, we do not support creation spawning in recursive levels of
         // the object instance.  So if the level does not match, issue an error
         // and get out.  NOTE, this should not happen if fnEstablishViewForInstance
         // is called above.  It removes the recursive spawning restriction!!
         if ( lpSearchViewEntityCsr->nLevel != lpLinkedInstance->nLevel )
         {
            LPTASK lpTask = zGETPTR( lpSpawnView->hTask );
            // "KZOEE139 - Error establishing cursor for "
            // "CreateEntity spawn"
            fnIssueCoreError( lpTask, lpView, 8, 139, 0,
                              lpSearchViewEntity->szName, 0 );
            nRC = zCALL_ERROR;
         }
         else
         {
            // We've established the cursors in the spawn view for spawning
            // the create, now call fnCreateEntity to spawn the create.
            nRC = fnCreateEntity( lpSpawnView,
                                  zGETPTR( lpWorkViewEntityCsr->hViewEntity ),
                                  lpWorkViewEntityCsr, nSpawnPosition, 0 );
            if ( nRC == 0 )
            {
               LPENTITYINSTANCE lpWorkInstance;

               lpSpawnedInstance = zGETPTR( lpWorkViewEntityCsr->hEntityInstance );
               lpSpawnedInstance->u.nInd.bCreated = TRUE;
               lpSpawnedInstance->hNextLinked = zGETHNDL( lpEntityInstance );
               if ( lpEntityInstance->hNextLinked )
               {
                  lpLinkedTwin = zGETPTR( lpEntityInstance->hNextLinked );
                  while ( zGETPTR( lpLinkedTwin->hNextLinked ) != lpEntityInstance )
                  {
                     lpLinkedTwin = zGETPTR( lpLinkedTwin->hNextLinked );
                  }

                  lpLinkedTwin->hNextLinked = zGETHNDL( lpSpawnedInstance );
               }
               else
                  lpEntityInstance->hNextLinked = zGETHNDL( lpSpawnedInstance );

               // Set the record pointer in the spawned instance.
               lpWorkInstance = zGETPTR( lpSpawnedInstance->hNextLinked );
               lpSpawnedInstance->hPersistRecord = lpWorkInstance->hPersistRecord;
            }

            DropView( lpSpawnView );
         }

      } // for ( lpSearchViewEntity )...

   }  // for ( lpLinkedInstance )...

   return( nRC );
}

// Checks to see if an entity is "dead".  A dead entity instance is an
// instance that's been deleted but doesn't need to be saved to the database.
zSHORT LOCALOPER
fnEntityInstanceIsDead( LPENTITYINSTANCE lpEntityInstance )
{
   // If the instance isn't hidden it can't be dead.
   if ( lpEntityInstance->u.nInd.bHidden == FALSE )
      return( FALSE );

   // Instances with previous versions aren't dead.
   if ( lpEntityInstance->hPrevVsn )
      return( FALSE );

   if ( lpEntityInstance->u.nInd.bCreated == FALSE &&
        lpEntityInstance->u.nInd.bIncluded == FALSE &&
        (lpEntityInstance->u.nInd.bExcluded ||
         lpEntityInstance->u.nInd.bDeleted) )
   {
      return( FALSE );
   }

   // If we get here then instance is dead.
   return( TRUE );
}

//./ ADD NAME=fnReclaimHiddenInstances
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnReclaimHiddenInstances
//
//  PURPOSE:    To process all hidden instances in an object instance
//              and return any instances which are hidden AND
//              do not have a cursor referencing them OR have persistent
//              indicators such that the would participate in a
//              DB Handler commit call.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zVOID LOCALOPER
fnReclaimHiddenInstances( LPVIEWOI  lpViewOI )
{
   LPENTITYINSTANCE lpEntityInstance;
   LPENTITYINSTANCE hEntityInstance;
   LPVIEWCSR        lpViewCsr;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITY     lpViewEntity, lpViewEntityRP;
   zSHORT           nHierLvl1, nHierLvl2;
   zBOOL            bGetNext;

   hEntityInstance = lpViewOI->hRootEntityInstance;
   if ( hEntityInstance == 0 )
      return;

   // Loop through each of the entity instances.  We use bGetNext to control
   // whether we get the next entity.  If bGetNext is FALSE then it's already
   // been done in the body of the loop.
   for ( ;
         hEntityInstance;
         hEntityInstance = bGetNext ? lpEntityInstance->hNextHier : hEntityInstance )
   {
      LPENTITYINSTANCE lpPrevHier;
      LPENTITYINSTANCE hNextHier;
      LPENTITYINSTANCE lpWork;
      zSHORT           nLevel;

      lpEntityInstance = zGETPTR( hEntityInstance );
      bGetNext = TRUE;

      // We only try to reclaim dead entity instances.
      if ( !fnEntityInstanceIsDead( lpEntityInstance ) )
         continue;

      // First see if instance is referenced by a cursor
      lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
      nHierLvl2 = 0;
      if ( lpViewEntity->bRecursive )
      {
         nHierLvl2 = lpViewEntity->nHierNbr - 1;
         lpViewEntityRP = zGETPTR( lpViewEntity->hParent );
         while ( lpViewEntityRP->lEREntTok != lpViewEntity->lEREntTok )
            lpViewEntityRP = zGETPTR( lpViewEntityRP->hParent );

         nHierLvl1 = lpViewEntityRP->nHierNbr - 1;
      }
      else
         nHierLvl1 = lpViewEntity->nHierNbr - 1;

      // Now go through the view cursors and see if any is referencing
      // the entity instance in question.
      for ( lpViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );
            lpViewCsr;
            lpViewCsr = zGETPTR( lpViewCsr->hNextViewCsr ) )
      {
         lpViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );  // blew on this line 2006.06.30 dks
         lpViewEntityCsr += nHierLvl1;
         if ( lpViewEntityCsr->hEntityInstance == hEntityInstance )
            break;

         if ( nHierLvl2 )
         {
            lpViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
            lpViewEntityCsr += nHierLvl2;
            if ( lpViewEntityCsr->hEntityInstance == hEntityInstance )
               break;
         }
      }

      // If no cursor was found, delete the entity instance and
      // all of its subordinates.
      if ( lpViewCsr )
         continue;     // Continue with next entity instance.

      nLevel = lpEntityInstance->nLevel;
      lpPrevHier = zGETPTR( lpEntityInstance->hPrevHier );
      if ( lpEntityInstance->hNextTwin )
      {
         lpWork = zGETPTR( lpEntityInstance->hNextTwin );
         lpWork->hPrevTwin = lpEntityInstance->hPrevTwin;
      }

      if ( lpEntityInstance->hPrevTwin )
      {
         lpWork = zGETPTR( lpEntityInstance->hPrevTwin );
         lpWork->hNextTwin = lpEntityInstance->hNextTwin;
      }

      // Indicate that we'll end up with the next entity instance.
      bGetNext = FALSE;

      // Check to see if we are removing the root entity instance.
      if ( lpViewOI->hRootEntityInstance == hEntityInstance )
         lpViewOI->hRootEntityInstance = lpEntityInstance->hNextTwin;

      // Clear this entity instance and all of its children.
      do
      {
         lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
         hNextHier    = lpEntityInstance->hNextHier;
         if ( lpEntityInstance->hNextLinked )
         {
            lpWork = zGETPTR( lpEntityInstance->hNextLinked );
            if ( lpWork->hNextLinked == hEntityInstance )
               lpWork->hNextLinked = 0;
            else
            {
               LPENTITYINSTANCE lpSearchLink;

               lpSearchLink = lpWork;
               while ( lpSearchLink->hNextLinked != hEntityInstance )
                  lpSearchLink = zGETPTR( lpSearchLink->hNextLinked );

               lpSearchLink->hNextLinked = lpEntityInstance->hNextLinked;
            }
         }
         else
            fnDeleteAttributeRecord( lpViewEntity, lpEntityInstance, TRUE );

         fnDeleteAttributeRecord( lpViewEntity, lpEntityInstance, FALSE );
         fnRemoveEntityFromDebugChange( lpEntityInstance );
         fnFreeDataspace( lpEntityInstance );

         // Get next instance.
         hEntityInstance  = hNextHier;
         lpEntityInstance = zGETPTR( hEntityInstance );

      }  while ( lpEntityInstance && lpEntityInstance->nLevel > nLevel );

      if ( lpPrevHier )
         lpPrevHier->hNextHier = hEntityInstance;

      if ( lpEntityInstance )
         lpEntityInstance->hPrevHier = zGETHNDL( lpPrevHier );

   } // for each lpEntityInstance...

   // We're all done, clear the hidden count.
   lpViewOI->nHiddenCount = 0;    // clear hidden count
}

//./ ADD NAME=fnInstanceLinkedToInstance
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnInstanceLinkedToInstance
//
//  PURPOSE:    To determine if two instances are linked
//
//  RETURNS:    0 - Instances not linked
//              1 - Instances linked
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zBOOL
fnInstanceLinkedToInstance( LPENTITYINSTANCE lpSrcInstance,
                            LPENTITYINSTANCE lpTgtInstance )
{
   LPENTITYINSTANCE lpWrkInstance;

   if ( lpSrcInstance == 0 || lpTgtInstance == 0 ||
        lpSrcInstance->hNextLinked == 0 || lpTgtInstance->hNextLinked == 0 )
   {
      return( 0 );
   }

   lpWrkInstance = zGETPTR( lpSrcInstance->hNextLinked );
   while ( lpWrkInstance != lpTgtInstance && lpWrkInstance != lpSrcInstance )
   {
      lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
   }

   if ( lpWrkInstance == lpTgtInstance )
      return( 1 );
   else
      return( 0 );
}

//./ ADD NAME=fnInstanceVersioned
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnInstanceVersioned
//
//  PURPOSE:    To determine if an instance is part of a versioned
//              sub-object
//
//  RETURNS:    0 - Instance not versioned
//              1 - Instance versioned
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zBOOL
fnInstanceVersioned( LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWOI lpViewOI = zGETPTR( lpEntityInstance->hViewOI );

   // If the ViewOI has no versioned instances, return 0.
   if ( lpViewOI->nVersionedInstances == 0 )
      return( 0 );

   // We look for either a temporal entity or a versioned instance up the
   // parent chain to see if the instance is versioned.
   while ( lpEntityInstance )
   {
      if ( lpEntityInstance->u.nInd.bTemporal || lpEntityInstance->hPrevVsn )
         return( 1 ); // the instance is versioned

      lpEntityInstance = zGETPTR( lpEntityInstance->hParent );
   }

   return( 0 ); // instance is not part of a newer version
}

//./ ADD NAME=fnInstanceVersionRootMatch
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnInstanceVersionRootMatch
//
//  PURPOSE:    To determine if the source instance is a newer version
//              of a subobject than the target version. This operation
//              is used to determine whether the source instance can
//              be used to spawn a create or include under the target
//              instance. If the source instance is a newer version, then
//              the spawn of the create or include must wait until the
//              parent of the source instance is Accepted.
//
//  RETURNS:    0 - Instances are part of separate subobject versions
//                  and the source instance is versioned.
//              1 - Instances are not part of the same subobject
//                  version or the source is unversioned
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zBOOL
fnInstanceVersionRootMatch( LPENTITYINSTANCE lpTgtEntityInstance,
                            LPENTITYINSTANCE lpSrcEntityInstance )
{
   LPVIEWOI lpViewOI = zGETPTR( lpSrcEntityInstance->hViewOI );

   // If no temporal entities or subobject versions exist for either
   // viewoi, then the answer is same version.
   if ( lpViewOI->nVersionedInstances == 0 )
      return( 1 );

   // Determine the source root version for the source entity instance.
   while ( lpSrcEntityInstance )
   {
      if ( lpSrcEntityInstance->hPrevVsn )
      {
         LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpSrcEntityInstance->hPrevVsn );

         if ( lpPrevVsn->u.nInd.bPrevVsnRoot )
            break;
      }

      lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hParent );
   }

   if ( lpSrcEntityInstance == 0 )
      return( 1 );

   // Determine the source root version for the target entity instance.
   while ( lpTgtEntityInstance )
   {
      if ( lpTgtEntityInstance->hPrevVsn )
      {
         LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpTgtEntityInstance->hPrevVsn );

         if ( lpPrevVsn->u.nInd.bPrevVsnRoot )
            break;
      }

      lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hParent );
   }

   // Return the comparison of the source and target version root.
   return( lpSrcEntityInstance == lpTgtEntityInstance );
}

//./ ADD NAME=fnInstanceKeysMatch
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnInstanceKeysMatch
//
//  PARAMETERS: lpTgtEntityInstance Target instance for link
//              lpSrcEntityInstance Source entity for link
//
//  RETURNS: 0           - Keys do not match (failure)
//           1           - Keys match (success)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnInstanceKeysMatch( LPENTITYINSTANCE lpTgtEntityInstance,
                     LPENTITYINSTANCE lpSrcEntityInstance )
{
   zBOOL        bKeyFound;
   zULONG       ulOffset;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   zPCHAR       lpSrcRecord;
   zPCHAR       lpTgtRecord;
   zPCHAR       lpSrcData;
   zPCHAR       lpTgtData;
   zCOREMEM     lpExternalP;

   bKeyFound = 0;
   lpTgtRecord = (zPCHAR) zGETPTR( lpTgtEntityInstance->hPersistRecord );
   lpSrcRecord = (zPCHAR) zGETPTR( lpSrcEntityInstance->hPersistRecord );
   if ( lpSrcRecord && lpTgtRecord )
   {
      lpViewEntity = zGETPTR( lpTgtEntityInstance->hViewEntity );
      lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
      while ( lpViewAttrib )
      {
         if ( lpViewAttrib->bKey && lpViewAttrib->bPersist )
         {
            bKeyFound = TRUE;
            ulOffset = lpViewAttrib->ulRecordOffset;
            lpSrcData = lpSrcRecord + ulOffset;
            lpTgtData = lpTgtRecord + ulOffset;
            switch ( lpViewAttrib->cType )
            {
               case 'S':
                  if ( lpSrcData[ 0 ] == (zCHAR) '\xff' &&
                       lpTgtData[ 0 ] == (zCHAR) '\xff' )
                  {
                     lpExternalP = (zCOREMEM) (lpSrcData + 1);
                     lpSrcData   = zGETPTR( *lpExternalP );
                     lpExternalP = (zCOREMEM) (lpTgtData + 1);
                     lpTgtData   = zGETPTR( *lpExternalP );
                  }

                  if ( lpSrcData[ 0 ] != lpTgtData[ 0 ] ||
                       zstrcmp( lpSrcData, lpTgtData ) != 0 )
                  {
                     return( 0 );
                  }

                  break;

               default:
                  if ( zmemcmp( lpSrcData, lpTgtData,
                                lpViewAttrib->ulLth ) != 0 )
                  {
                     return( 0 );
                  }
            }
         }

         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
      }
   }

   // If we get here and a key was found, then the records match ...
   // return success.
   if ( bKeyFound )
      return( 1 );

   // If a key was not found or a record does not exist, then they can't
   // match ... return failure.
   return( 0 );  // default is no match
}

//./ ADD NAME=fnRelinkInstanceToInstance
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnRelinkInstanceToInstance
//
//  PARAMETERS: lpTgtEntityInstance Target instance for link
//              lpSrcEntityInstance Source entity for link
//
//  RETURNS: 0           - Entity instances linked successfully
//           1           - Entity instances already linked
//           zCALL_ERROR - Error relinking entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnRelinkInstanceToInstance( LPENTITYINSTANCE lpTgtEntityInstance,
                            LPENTITYINSTANCE lpSrcEntityInstance )
{
   LPVIEWENTITY     lpTgtViewEntity;
   LPENTITYINSTANCE lpWrkEntityInstance;
   LPENTITYINSTANCE lpVsnEntityInstance;
   zPVOID           hOldRecord;
   zBOOL            bCreated, bUpdated, bPrevVersion;
   zSHORT           nRC;

#ifdef DEBUG
   LPVIEWENTITY lpSrcViewEntity = zGETPTR( lpSrcEntityInstance->hViewEntity );
#endif

   // Get the target view entity.
   lpTgtViewEntity = zGETPTR( lpTgtEntityInstance->hViewEntity );

   // Make sure that the source and target are not already linked.
   if ( lpSrcEntityInstance == lpTgtEntityInstance )
      nRC = 1;
   else
   {
      nRC = 0;   // default return code to 0
      bUpdated = lpSrcEntityInstance->u.nInd.bUpdated;
      bCreated = lpSrcEntityInstance->u.nInd.bCreated;
      bPrevVersion = FALSE;
      if ( lpSrcEntityInstance->u.nInd.bPrevVersion ||
           lpTgtEntityInstance->u.nInd.bPrevVersion )
      {
         bPrevVersion = TRUE;
         if ( lpSrcEntityInstance->u.nInd.bPrevVersion == FALSE )
         {
            lpSrcEntityInstance->u.nInd.bPrevVersion = TRUE;
            lpWrkEntityInstance = zGETPTR( lpSrcEntityInstance->hNextLinked );
            while ( lpWrkEntityInstance &&
                    lpWrkEntityInstance != lpSrcEntityInstance )
            {
               lpWrkEntityInstance->u.nInd.bPrevVersion = TRUE;
               lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hNextLinked );
            }
         }
      }

      lpWrkEntityInstance = zGETPTR( lpSrcEntityInstance->hNextLinked );
      while ( lpWrkEntityInstance &&
              lpWrkEntityInstance != lpSrcEntityInstance )
      {
         if ( lpWrkEntityInstance == lpTgtEntityInstance )
         {
            nRC = 1;
            break;
         }

         lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hNextLinked );
      }
   }

   // if the return code is 0, then all edits have passed and the
   // source and target entity instances are not currently linked,
   // link them now
   if ( nRC )
      return( nRC );

   // Link the two instances together
   if ( lpSrcEntityInstance->hNextLinked == 0 )
   {
      if ( lpTgtEntityInstance->hNextLinked == 0 )
         lpSrcEntityInstance->hNextLinked = zGETHNDL( lpTgtEntityInstance );
      else
      {
         // Chain forward past target entity instance on target chain
         lpWrkEntityInstance = zGETPTR( lpTgtEntityInstance->hNextLinked );
         for ( ; ; )
         {
            // Set the record pointer for other instances in target
            // link chain to point to the source record.
            lpWrkEntityInstance->hPersistRecord = lpSrcEntityInstance->hPersistRecord;
            lpWrkEntityInstance->u.nInd.bUpdated = bUpdated;
            lpWrkEntityInstance->u.nInd.bCreated = bCreated;
            lpWrkEntityInstance->u.nInd.bPrevVersion = bPrevVersion;

            if ( zGETPTR( lpWrkEntityInstance->hNextLinked ) ==
                                                   lpTgtEntityInstance )
            {
               break;
            }

            lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hNextLinked );
         }

         lpSrcEntityInstance->hNextLinked = lpTgtEntityInstance->hNextLinked;
      }

      lpTgtEntityInstance->hNextLinked = zGETHNDL( lpSrcEntityInstance );
   }
   else
   {
      if ( lpTgtEntityInstance->hNextLinked == 0 )
      {
         lpTgtEntityInstance->hNextLinked = lpSrcEntityInstance->hNextLinked;
         lpSrcEntityInstance->hNextLinked = zGETHNDL( lpTgtEntityInstance );
      }
      else
      {
         // Both entity instances are on two different link chains,
         // break the two chains and combine into one larger chain
         lpWrkEntityInstance = zGETPTR( lpSrcEntityInstance->hNextLinked );
         while ( TRUE )
         {
            LPENTITYINSTANCE lpWorkEI;

            lpWorkEI = zGETPTR( lpWrkEntityInstance->hNextLinked );

            if ( lpWorkEI == lpSrcEntityInstance )
               break;

            lpWrkEntityInstance = lpWorkEI;
         }

         lpWrkEntityInstance->hNextLinked = zGETHNDL( lpTgtEntityInstance );

         // Chain forward past target entity instance on target chain
         lpWrkEntityInstance = zGETPTR( lpTgtEntityInstance->hNextLinked );
         for ( ; ; )
         {
            LPENTITYINSTANCE lpWorkEI;

            // Set the record pointer for other instances in target
            // link chain to point to the source record
            lpWrkEntityInstance->hPersistRecord = lpSrcEntityInstance->hPersistRecord;
            lpWrkEntityInstance->u.nInd.bUpdated = bUpdated;
            lpWrkEntityInstance->u.nInd.bCreated = bCreated;
            lpWrkEntityInstance->u.nInd.bPrevVersion = bPrevVersion;

            lpWorkEI = zGETPTR( lpWrkEntityInstance->hNextLinked );

            if ( lpWorkEI == lpTgtEntityInstance )
               break;

            lpWrkEntityInstance = lpWorkEI;
         }

         // Now that we have found the last instance in the target
         // link chain, chain it forward to the source instance.
         lpWrkEntityInstance->hNextLinked = zGETHNDL( lpSrcEntityInstance );
      }
   }

#ifdef DEBUG
   lpSrcViewEntity = zGETPTR( lpSrcEntityInstance->hViewEntity );
#endif

   // The two instances are linked together, now free up the data
   // record for the target entity instance and set the record
   // pointer from the source entity instance.
   hOldRecord = lpTgtEntityInstance->hPersistRecord;
   fnDeleteAttributeRecord( lpTgtViewEntity, lpTgtEntityInstance, TRUE );
   lpTgtEntityInstance->hPersistRecord = lpSrcEntityInstance->hPersistRecord;
   lpTgtEntityInstance->u.nInd.bUpdated = bUpdated;
   lpTgtEntityInstance->u.nInd.bCreated = bCreated;
   lpTgtEntityInstance->u.nInd.bPrevVersion = bPrevVersion;

   // Now that the old record has been deleted, propogate the link
   // record across all versions of the target instance.
   if ( lpTgtEntityInstance->hNextVsn )
   {
      zPVOID hNewRecord;

      hNewRecord = lpTgtEntityInstance->hPersistRecord;

      lpVsnEntityInstance = zGETPTR( lpTgtEntityInstance->hNextVsn );
      while ( lpVsnEntityInstance )
      {
         if ( lpVsnEntityInstance->hPersistRecord == hOldRecord )
         {
            lpVsnEntityInstance->hPersistRecord = hNewRecord;
            lpWrkEntityInstance = zGETPTR( lpVsnEntityInstance->hNextLinked );

            while ( lpWrkEntityInstance &&
                    lpWrkEntityInstance != lpVsnEntityInstance )
            {
               lpVsnEntityInstance->hPersistRecord = hNewRecord;
               lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hNextLinked );
            }
         }

         lpVsnEntityInstance = zGETPTR( lpVsnEntityInstance->hNextVsn );
      }
   }

   return( nRC );  // return to caller
}

//./ ADD NAME=fnGetEntityCnstrntOper
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetEntityCnstrntOper
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zPECEOPER
fnGetEntityCnstrntOper( zVIEW        lpView,
                        LPVIEWENTITY lpViewEntity,
                        LPTASK       lpCurrentTask )
{
   LPTASK      hCurrentTask = zGETHNDL( lpCurrentTask );
   LPVIEWOD    lpViewOD;
   zPCHAR      pchOperLibName;
   LPLIBRARY   hLibrary;
   zPVOID      hTaskOperation;
   LPTASKOPER  lpTaskOperation;
   zLONG       lProcessID = SysGetProcessID( 0 );

   // No Object Constraint operation pointer, return 0 ==> not found.
   if ( lpViewEntity->szECEOpername[ 0 ] == 0 )
      return( 0 );

   lpViewOD = zGETPTR( lpView->hViewOD );

   for ( lpTaskOperation = zGETPTR( lpViewEntity->hFirstTaskOperation );
         lpTaskOperation;
         lpTaskOperation = zGETPTR( lpTaskOperation->hNextTaskOperationForOEA ) )
   {
      if ( lpTaskOperation->hTask == hCurrentTask &&
           lpTaskOperation->lProcessID == lProcessID )
      {
         return( (zPECEOPER) lpTaskOperation->pfnTaskOper );
      }
   }

   // No task object oper exists yet, create one if possible.
   pchOperLibName = lpViewOD->szOperLibname;
   if ( pchOperLibName == 0 || pchOperLibName[ 0 ] == 0 )
      return( 0 );

   hLibrary = SysLoadLibrary( lpView, pchOperLibName );

   if ( hLibrary )
   {
      zBOOL bMutexLocked;

      hTaskOperation = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                         sizeof( TaskOperationRecord ), 1, 0,
                                         iTaskOperation );
      if ( hTaskOperation == 0 )
         return( 0 );

      lpTaskOperation              = zGETPTR( hTaskOperation );
      lpTaskOperation->hViewEntity = zGETHNDL( lpViewEntity );
      lpTaskOperation->hTask       = hCurrentTask;
      lpTaskOperation->lProcessID  = lProcessID;

      // Set pointers for TaskOperation chain off of Task structure
      // Note: this is a single linked list
      if ( hCurrentTask == AnchorBlock->hMainTask )
      {
         bMutexLocked = TRUE;
         zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      }
      else
         bMutexLocked = FALSE;

      lpTaskOperation->hNextTaskOperationForTask =
                                          lpCurrentTask->hFirstTaskOperation;
      lpCurrentTask->hFirstTaskOperation = hTaskOperation;

      if ( bMutexLocked )
         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      lpTaskOperation->pfnTaskOper = SysGetProc( hLibrary,
                                                 lpViewEntity->szECEOpername );

      // Set pointers for TaskOperation chain off of ViewEntity structure
      // Note: this is a double linked list
      if ( lpViewEntity->hLastTaskOperation )
      {
         LPTASKOPER lpLastTaskOperation;

         lpLastTaskOperation = zGETPTR( lpViewEntity->hLastTaskOperation );
         lpLastTaskOperation->hNextTaskOperationForOEA = hTaskOperation;
         lpTaskOperation->hPrevTaskOperationForOEA =
                                            lpViewEntity->hLastTaskOperation;
         lpViewEntity->hLastTaskOperation = hTaskOperation;
      }
      else
      {
         lpViewEntity->hFirstTaskOperation = hTaskOperation;
         lpViewEntity->hLastTaskOperation  = hTaskOperation;
      }
   }
   else
      return( 0 );

   return( (zPECEOPER) lpTaskOperation->pfnTaskOper );
}

//./ ADD NAME=fnInvokeECEOperation
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnInvokeECEOperation
//
// PURPOSE: Invoke the Entity Constraint operation for a given entity
//
// RETURNS:
//                   0 - Constraint handler says okay.
//   zCONSTRAINT_ERROR - Constraint handler detected an error situation.
//         zCALL_ERROR - Operation failed.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnInvokeECEOperation( zVIEW        lpView,
                      LPVIEWENTITY lpViewEntity,
                      LPTASK       lpCurrentTask,
                      zSHORT       nEvent,
                      zSHORT       nState )
{
   zPECEOPER   pfn;

   pfn = fnGetEntityCnstrntOper( lpView, lpViewEntity, lpCurrentTask );
   if ( pfn )
   {
      return( (*pfn)( lpView, lpViewEntity->szName,
                      nEvent, nState ) );
   }

   return( 0 );
}

//./ ADD NAME=fnValidateInstanceAttributes
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnValidateInstanceAttributes
//
// PURPOSE: Check for required attributes that have a null value.
//
// RETURNS:
//                   0 - attributes okay.
//                  >0 - number of required attributes with a null value.
//         zCALL_ERROR - Operation failed.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnValidateInstanceAttributes( zVIEW             lpView,
                              LPVIEWENTITY      lpViewEntity,
                              LPENTITYINSTANCE  lpEntityInstance )
{
   LPVIEWATTRIB   lpViewAttrib;
   zPVOID         lpAttrAddr;
   zULONG         un;
   zSHORT         nErrorCount = 0;

   // User shouldn't pass us a null pointer...
   if ( lpEntityInstance == 0 )
      return( zCALL_ERROR );

   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      // If the attribute is not required don't worry about it.
      // Don't worry about hidden attributes.
      // If the attribute is a genkey then we'll set it later so skip it.
      if ( lpViewAttrib->bRequired == FALSE ||
           lpViewAttrib->bHidden || lpViewAttrib->bGenkey )
      {
         continue;
      }

      // If the entity is a hanging entity then we only care about keys.
      if ( lpEntityInstance->u.nInd.bHangingEntity )
      {
         // Not a key?  Skip it.
         if ( lpViewAttrib->bKey == FALSE )
            continue;
      }

      // If the attribute is NULL then we have a problem.
      if ( fnGetAttrAddrFromEntityInstance( (zCOREMEM) &lpAttrAddr, &un,
                                            lpEntityInstance,
                                            lpViewAttrib ) == -1 )
      {
         // "KZOEE248 - Required attribute is null"
         IssueOE_Error( lpView, 8, 248,
                        zMSGQ_REQUIRED_DATA_ITEM_ERROR, 0,
                        lpViewEntity->szName,
                        lpViewAttrib->szName );

         // attribute is null...
         if ( nErrorCount != 32767 )
            nErrorCount++;
      }

   } // for...

   return( nErrorCount );
}

//./ ADD NAME=fnRemoveAllChildTemporalVsns
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnRemoveAllChildTemporalVsns
//
// PURPOSE: This operation removes all temporal versions for an entity
//          instance by going to the last descendent and issuing a cancel
//          or an accept for all temporal children to the entity instance.
//
// RETURNS:  0 - Success
//           < zCURSOR_UNDEFINED - Accept constraint error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnRemoveAllChildTemporalVsns( zVIEW            lpView,
                              LPENTITYINSTANCE lpEntityInstance,
                              zBOOL            bAccept )
{
   LPVIEWENTITY     lpViewEntity;
   LPVIEWCSR        lpViewCsr;
   LPVIEWOI         lpViewOI;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpWorkInstance;
   zVIEW            vWork;
   zSHORT           nLvl;
   zSHORT           nRC;

   vWork = 0;
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   if ( lpEntityInstance == 0 || lpViewOI->nVersionedInstances == 0 )
      return( 0 );

   lpWorkInstance = lpEntityInstance;
   nLvl = lpWorkInstance->nLevel;
   while ( lpWorkInstance->hNextHier )
   {
      LPENTITYINSTANCE lpNextHier = zGETPTR( lpWorkInstance->hNextHier );

      if ( lpNextHier->nLevel <= nLvl )
         break;

      lpWorkInstance = lpNextHier;
   }

   // Now that we have found the last descendent of the entity instance,
   // go through and find all versions to accept or cancel.
   while ( lpWorkInstance != lpEntityInstance )
   {
      if ( lpWorkInstance->u.nInd.bTemporal || lpWorkInstance->hPrevVsn )
      {
         LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpWorkInstance->hPrevVsn );

         if ( lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot == FALSE )
         {
            LPTASK lpTask;

            // Remove was requested ... why not do it???
            TraceLineS( "RemoveAllTemporal skipping break:", " dks 2007.04.23" );  // dks  2007.04.23
            lpViewEntity = zGETPTR( lpWorkInstance->hViewEntity );
            nRC = fnValidateInstanceParameters( &lpTask,
                                                &lpViewEntity,
                                                &lpViewEntityCsr,
                                                iCancelSubobject,
                                                lpView,
                                                lpViewEntity->szName,
                                                (zSHORT) 0x8000 ); // test (no error message)
            if ( nRC != zCALL_ERROR )
               fnOperationReturn( iCancelSubobject, lpTask );

            if ( nRC )
               break;
         }

         if ( vWork == 0 )
            CreateViewFromViewForTask( &vWork, lpView, 0 );

         lpViewEntityCsr = fnEstablishViewForInstance( vWork, 0, lpWorkInstance );
         lpViewEntity = zGETPTR( lpWorkInstance->hViewEntity );
         if ( bAccept )
            nRC = AcceptSubobject( vWork, lpViewEntity->szName );
         else
            nRC = CancelSubobject( vWork, lpViewEntity->szName );

         if ( nRC < zCURSOR_UNDEFINED || lpViewOI->nVersionedInstances == 0 )
         {
            DropView( vWork );

            if ( lpViewOI->nVersionedInstances == 0 )
               return( 0 );
            else
               return( nRC );
         }

         // Get the instance pointer from the cursor after the accept or
         // cancel call.
         lpWorkInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      }

      lpWorkInstance = zGETPTR( lpWorkInstance->hPrevHier );
   }

   if ( vWork )
      DropView( vWork );

   return( 0 );
}

//./ ADD NAME=fnRemoveOtherOI_LinkedInstances
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnRemoveOtherOI_LinkedInstances
//
// PURPOSE: This operation removes all instances linked to the passed
//          Entity instance which are not in the same object instance.
//          This is done so the behavior in RelinkAllSubobjectsForOI is
//          consistent whether or not the target instance is a leaf node!.
//          Note that the record pointer for the entity instance and the
//          linked instances to the entity instance are zeroed if linked
//          instances are found outside the ViewOI scope.
//
// RETURNS:  0 - Success
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnRemoveOtherOI_LinkedInstances( LPENTITYINSTANCE lpTgtEntityInstance,
                                 LPENTITYINSTANCE lpSrcEntityInstance )
{
   zLONG            lTgtERRelTok;
   LPVIEWOI         hSrcViewOI;
   LPVIEWOI         hTgtViewOI;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITY     lpWkViewEntity;
   LPENTITYINSTANCE lpWk1;
   LPENTITYINSTANCE lpWk2;
   LPENTITYINSTANCE lpWk3;

   lpViewEntity = zGETPTR( lpTgtEntityInstance->hViewEntity );
   lTgtERRelTok = lpViewEntity->lERRelTok;
   hSrcViewOI   = lpSrcEntityInstance->hViewOI;
   hTgtViewOI   = lpTgtEntityInstance->hViewOI;

   // First, see if there are any entity instances linked to this
   // instance which are in a different ViewOI, if not so, then return.
   lpWk1 = zGETPTR( lpTgtEntityInstance->hNextLinked );
   if ( lpWk1 )
      lpWkViewEntity = zGETPTR( lpWk1->hViewEntity );
   while ( lpWk1 && lpWk1 != lpTgtEntityInstance &&
           ( lpWk1->hViewOI == hTgtViewOI ||
             lpWk1->hViewOI == hSrcViewOI ||
             lTgtERRelTok == lpWkViewEntity->lERRelTok) )
   {
      lpWk1 = zGETPTR( lpWk1->hNextLinked );
      lpWkViewEntity = zGETPTR( lpWk1->hViewEntity );
   }

   if ( lpWk1 == 0 || lpWk1 == lpTgtEntityInstance )
      return( 0 );

   // lpWk1 now points to the first Entity instance found which is
   // not part of the same object instance as the entity instance
   // passed, remove it from the link chain.
   lpWk2 = lpTgtEntityInstance;
   while ( zGETPTR( lpWk2->hNextLinked ) != lpWk1 )
   {
      lpWk2 = zGETPTR( lpWk2->hNextLinked );
   }

   if ( zGETPTR( lpWk1->hNextLinked ) == lpWk2 )
      lpWk2->hNextLinked = 0;
   else
      lpWk2->hNextLinked = lpWk1->hNextLinked;

   lpWk1->hNextLinked = 0;

   // Now see if we can find any more instances not in the link chain
   for ( ; ; )
   {
      if ( lpTgtEntityInstance->hNextLinked == 0 )
         break;

      lpWk2 = lpTgtEntityInstance;
      lpWk3 = zGETPTR( lpWk2->hNextLinked );
      lpViewEntity = zGETPTR( lpWk3->hViewEntity );
      while ( lpWk3 != lpTgtEntityInstance &&
              ( lpWk3->hViewOI == hTgtViewOI ||
                lpWk3->hViewOI == hSrcViewOI ||
                lTgtERRelTok == lpViewEntity->lERRelTok) )
      {
         lpWk2 = lpWk3;
         lpWk3 = zGETPTR( lpWk3->hNextLinked );
         lpViewEntity = zGETPTR( lpWk3->hViewEntity );
      }

      // If lpWk3 is in another object instance, remove it from the other
      // instance and link it up to lpWk1. lpWk2 is currently chained
      // to lpWk3 in order to make unchaining easier.
      if ( lpWk3 != lpTgtEntityInstance )
      {
         if ( lpWk3->hNextLinked == zGETHNDL( lpWk2 ) )
            lpWk2->hNextLinked = 0;
         else
            lpWk2->hNextLinked = lpWk3->hNextLinked;

         if ( lpWk1->hNextLinked == 0 )
            lpWk3->hNextLinked = zGETHNDL( lpWk1 );
         else
            lpWk3->hNextLinked = lpWk1->hNextLinked;

         lpWk1->hNextLinked = zGETHNDL( lpWk3 );
      }
      else
         break;
   }

   // Finally, zero all the record pointers from lpTgtEntityInstance and
   // its linked instances
   lpWk2 = lpTgtEntityInstance;
   do
   {
      lpWk3 = lpWk2;
      while ( lpWk3->hPrevVsn )
         lpWk3 = zGETPTR( lpWk3->hPrevVsn );

      while ( lpWk3 )
      {
         if ( lpWk3->hPersistRecord == lpWk1->hPersistRecord )
            lpWk3->hPersistRecord = 0;

         lpWk3 = zGETPTR( lpWk3->hNextVsn );
      }

      lpWk2 = zGETPTR( lpWk2->hNextLinked );

   }  while ( lpWk2 && lpWk2 != lpTgtEntityInstance );

   return( 0 );
}

//./ ADD NAME=fnEntityInRecursivePath
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnEntityInRecursivePath
//
// PURPOSE: This operation determines whether or not the view entity passed
//          is either a recursive behaving entity or in a path between
//          a recursive child entity and its recursive parent. This is
//          used to determine whether twin processing can be used in
//          searching for entity instances for cursor and relink processing.
//
// RETURNS:  1 - Entity is in a recursive path
//           0 - Entity is not in a recursive path
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zBOOL
fnEntityInRecursivePath( LPVIEWENTITY lpViewEntity )
{
   LPVIEWENTITY lpWkViewEntity;

   if ( lpViewEntity->bRecursive )
      return( 1 );

   while ( lpViewEntity )
   {
      while ( lpViewEntity && lpViewEntity->bRecursivePar == FALSE )
         lpViewEntity = zGETPTR( lpViewEntity->hParent );

      if ( lpViewEntity )
      {
         lpWkViewEntity = zGETPTR( lpViewEntity->hNextHier );

         while ( lpWkViewEntity &&
                 lpWkViewEntity->nLevel > lpViewEntity->nLevel )
         {
            if ( lpWkViewEntity->bRecursive &&
                 lpWkViewEntity->lEREntTok == lpViewEntity->lEREntTok )
            {
               return( 1 );
            }

            lpWkViewEntity = zGETPTR( lpWkViewEntity->hNextHier );
         }

         lpViewEntity = zGETPTR( lpViewEntity->hParent );
      }
   }

   return( 0 );
}


//./ ADD NAME=fnFirstLevel2_ForViewEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnFirstLevel2_ForViewEntity
//
// PURPOSE: This operation positions on the first level 2 instance which
//          is a candidate parent type for the view entity passed.
//
// RETURNS:  LPENTITYINSTANCE - Level 2 candidate parent
//           0 - There is no level 2 candidate
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
LPENTITYINSTANCE
fnFirstLevel2_ForViewEntity( LPENTITYINSTANCE lpStartEntityInstance,
                             LPVIEWENTITY     lpViewEntity )
{
   LPVIEWENTITY lpWkViewEntity;

   if ( lpViewEntity->nLevel < 2 )
      return( lpStartEntityInstance );

   while ( lpViewEntity->nLevel > 2 )
      lpViewEntity = zGETPTR( lpViewEntity->hParent );

   while ( lpStartEntityInstance && lpStartEntityInstance->nLevel == 1 )
      lpStartEntityInstance = zGETPTR( lpStartEntityInstance->hNextHier );

   for ( ; ; )
   {
      if ( lpStartEntityInstance == 0 )
         return( 0 );

      lpWkViewEntity = zGETPTR( lpStartEntityInstance->hViewEntity );
      if ( lpWkViewEntity->nHierNbr < lpViewEntity->nHierNbr )
      {
         if ( lpStartEntityInstance->hNextTwin )
            lpStartEntityInstance = zGETPTR( lpStartEntityInstance->hNextTwin );
         else
            lpStartEntityInstance = zGETPTR( lpStartEntityInstance->hNextHier );
      }
      else
      if ( lpWkViewEntity->nHierNbr > lpViewEntity->nHierNbr )
         return( 0 );
      else
         return( lpStartEntityInstance );
        }
}

//./ ADD NAME=fnStillPrevVersion
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       fnStillPrevVersion
//
//  PARAMETERS:  lpEntityInstance - instance to check if still prev vsn
//  RETURNS:     0 - Instance is no longer a previous version and its
//                   flag has been reset
//               1 - Instance is still a previous version
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zBOOL
fnStillPrevVersion( LPENTITYINSTANCE lpEntityInstance )
{
   LPENTITYINSTANCE lpLinked;

   if ( lpEntityInstance->hNextVsn )
      return( 1 );

   lpLinked = zGETPTR( lpEntityInstance->hNextLinked );
   while ( lpLinked && lpLinked != lpEntityInstance )
   {
      if ( lpLinked->hNextVsn )
         return( 1 );

      lpLinked = zGETPTR( lpLinked->hNextLinked );
   }

   lpEntityInstance->u.nInd.bPrevVersion = FALSE;
   return( 0 );
}

//./ ADD NAME=fnEntityContainsEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       fnEntityContainsEntity
//
//               Checks to see if all of entity2's non-hidden attributes
//               are non-hidden in entity1.  This is used to make sure
//               that entity2 can be included into entity1.
//
//  PARAMETERS:  lpVE1 - 'Container' entity.
//               lpVE2 - 'Source' entity.
//
//  RETURNS:     TRUE  - Entity1 contains Entity2.
//               FALSE - It doesn't.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zBOOL LOCALOPER
fnEntityContainsEntity( LPVIEWENTITY lpVE1, LPVIEWENTITY lpVE2 )
{
   LPVIEWATTRIB lpViewAttrib2;
   LPVIEWATTRIB lpViewAttrib1;

   // They had better be the same ER entity.
   if ( lpVE1->lEREntTok != lpVE2->lEREntTok )
      return( FALSE );

   if ( lpVE2 == lpVE1 )
      return( TRUE );

   if ( lpVE2->bFullPersist && lpVE1->bFullPersist )
      return( TRUE );

   // OK...the two enties are not the same AND one of the entities is
   // not fully persistent.  Look for hidden attributes.
   for ( lpViewAttrib2 = zGETPTR( lpVE2->hFirstOD_Attrib );
         lpViewAttrib2;
         lpViewAttrib2 = zGETPTR( lpViewAttrib2->hNextOD_Attrib ) )
   {
      // We don't care if the 'source' attribute is hidden.
      if ( lpViewAttrib2->bHidden &&
           lpViewAttrib2->bKey == FALSE &&
           lpViewAttrib2->bForeignKey == FALSE )
      {
         continue;
      }

      // See if there is a matching attribute in the cache entity.
      for ( lpViewAttrib1 = zGETPTR( lpVE1->hFirstOD_Attrib );
            lpViewAttrib1;
            lpViewAttrib1 = zGETPTR( lpViewAttrib1->hNextOD_Attrib ) )
      {
         if ( lpViewAttrib1->lERAttTok == lpViewAttrib2->lERAttTok )
            break;
      }

      // If we didn't find a matching attribute or if the attribute we
      // found is hidden then set compare to FALSE.
      if ( lpViewAttrib1 == 0 ||
           (lpViewAttrib1->bHidden && lpViewAttrib1->bKey == FALSE &&
            lpViewAttrib1->bForeignKey == FALSE) )
      {
         // We found a non-hidden attribute in lpVE2 that is not part of
         // lpVE1 so return FALSE.
         return( FALSE );
      }

   } // for ( lpViewAttrib2 )...

   return( TRUE );
}

//./ ADD NAME=fnCheckOIUpdated
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       fnCheckOIUpdated
//
//               Reset OI Update flags from scratch.
//
//  PARAMETERS:  lpViewOI - Onject Instance
//
//  RETURNS:     0 - OI is NOT updated
//               1 - OI is updated (Commit To File)
//               3 - OI is updated persistent (Database Commit)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
static zSHORT
fnCheckOIUpdated( LPVIEWOI lpViewOI )
{
   LPENTITYINSTANCE lpLoopInstance;
   LPVIEWENTITY lpViewEntity;

   lpViewOI->bUpdated = lpViewOI->bUpdatedFile = FALSE;

   // Loop through the entity instances.
   for ( lpLoopInstance = zGETPTR( lpViewOI->hRootEntityInstance );
         lpLoopInstance && lpViewOI->bUpdated == FALSE;
         lpLoopInstance = zGETPTR( lpLoopInstance->hNextHier ) )
   {
      // Check to see if the entity instance has been changed somehow.  If it has
      // then we need to flag the OI as being updated.  We ignore instances
      // that have a previous version because we can't tell yet if the OI will
      // be updated because of the versioned instance.
      if ( lpLoopInstance->hPrevVsn == 0 &&
           !fnEntityInstanceIsDead( lpLoopInstance ) &&
           (lpLoopInstance->u.nInd.bCreated  ||
            lpLoopInstance->u.nInd.bUpdated  ||
            lpLoopInstance->u.nInd.bDeleted  ||
            lpLoopInstance->u.nInd.bIncluded ||
            lpLoopInstance->u.nInd.bExcluded) )
      {
         lpViewEntity = zGETPTR( lpLoopInstance->hViewEntity );

         if ( lpViewEntity->bDerived == FALSE &&
              lpViewEntity->bDerivedPath == FALSE &&
              lpViewEntity->hFirstDataRecord != 0 )
         {
            // The entity is a "database" entity (not work).
            lpViewOI->bUpdated = TRUE;
         }

         lpViewOI->bUpdatedFile = TRUE;
      }
   }

   if ( lpViewOI->bUpdated )
      return( 3 );
   else
   if ( lpViewOI->bUpdatedFile )
      return( 1 );
   else
      return( 0 );
}
