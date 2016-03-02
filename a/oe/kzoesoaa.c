#include "kzoephdr.h"
//
// MODULE NAME:  KZOESOAA  -  Object services Subobject Entity Instance
//               Management
// DESCRIPTION:  This is the source file which contains Object services
//               operations for creating and deleting subobject entity
//               instances
// OPERATIONS:   All OPERATIONS which create, move, copy, or delete
//               subobject Entity instances
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

2002.05.16  DGC
   Put in diagnostic information requested by Phil.

2002.03.12  HH
   R056357, fix bug where update flag was not set.
   (fnIncludeSubobjectFromSubobject)

2002.01.23  RG
   Make AcceptAllTemporalSubobjects and CancelAllTemporalSubobjects
   available for VML

2001.11.01  HH
   improved resetting subobject version update flags.

2001.09.10  DGC
   Added some diagnostic info for a Include error.

2001.08.07  HH
   added SubobjectVersionUpdated.
   Improved Setting Update Flags for versioned entities.
   Use fnSetUpdateIndicator to set update flags on include.

2001.02.05  DGC  10b
   Change requested by Don--I removed a consistency check made when a spawn
   is caused by an IncludeSubobject( ).  With changes made to the dbhandlers
   a while ago it is no longer necessary to force entities to be included in
   a LOD for consistency reasons.

2000.11.22  DGC  10b
   Added information to an error message.

2000.10.24  DGC 10b
   Fixed MoveSubobject( ) bugs reported by Doug. MoveSubobject( ) was having
   problems if root entities were being moved.

2000.09.21  DGC 10b
   Made fix to CreateTemporalSubobjectVersion( ). We weren't checking for a
   null linked list.

2000.07.27  DGC 10b
   Added IncludeSubobjectFromSubobjectEx.

2000.07.10  DGC 10b
   Fixed bug reported by Jeff--AcceptSubobject( ) was freeing hRecord too
   many times and not removing linked EI's from the link chain.

2000.04.14  DGC 10b
   Fixed a view leak in AcceptSubobject( ) reported by Don.

2000.03.29  DGC 10b
   Fixed bug reported by Doug and Hartmut.  Link chain was not correctly
   being changed during an AcceptSubobject( ).

2000.03.07  DGC 10b
   Bug reported by Doug:  core wasn't properly handling an AcceptSubobject( )
   when two different things happened:
      1) The old subobject version had two EIs linked to each other.
      2) After a new subobject version is created user
         calls SetViewToSubobject( ).
   In either of these cases we weren't correctly resetting view cursors.
   Now we do.

2000.03.03  DGC 10b
   Added code to fix bug that kept Duplicate Relationship entities from
   being spawned after an activate.

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

1999.08.25  TMV TB742 ALL
   comment out obsolete code to avoid crash
         related function: CreateTemporalSubobjectVersion

1999.05.07  DGC  10a
   During an Include/Relink we check the ER_DATE attribute in the ViewOD to
   make sure that the ODs are not out of sync.

1999.04.02  DGC
   Another fix for fnValidStructureEntity( ).
   Made a change for EMD app--we now allow entities to be included into a
   read-only entity if the target entity is part of a derived relationship.

1999.03.08  DGC
   Fixed typo in error message.
   Changed IncludeSubobject...( ) so that the source view can be read-only.

1999.02.12  DGC
   Started adding (and removing) stuff for attribute flags.

1999.02.11  DGC
   Added check to fnIncludeEntityAndChildren( ) to look for bDerivedPath as
   well as bDerived (we were incorrectly setting bUpdateFile).

1999.01.29  DGC
   Totally rewrote the include code in an attempt to fix include bugs where
   entities weren't properly copied from source to the target OI.

1999.01.04  DGC
   Fixed (yet another) bug dealing with including parent entities.

*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#define __RELINK_PERFORMANCE__
#define __ACTIVATE_CONSTRAINTS__     // these are common to kzoeeiaa.c

// Following structure used by fnInclude...( ).
typedef struct IncludeInfoStruct IncludeInfoRecord;
typedef IncludeInfoRecord * LPINCLUDEINFO;
struct IncludeInfoStruct
{
   zVIEW     lpTgtView;
   LPVIEWOI  lpTgtViewOI;
   LPTASK    lpTgtAllocTask;
   zVIEW     lpSrcView;
   LPVIEWOI  lpSrcViewOI;
   LPTASK    lpSrcAllocTask;
   LPVIEWCSR lpSrcViewCsr;
   zSHORT    nVsnLevel;
   zBOOL     bFirstEntity;
   zSHORT    bNewRoot;
   zSHORT    bMarkIncluded;
   zSHORT    nPosition;       // Include position (zPOS_FIRST, etc.)
};

static zSHORT
fnGetResetUpdFlags( LPENTITYINSTANCE lpStartInstance,
                    zSHORT *pUpdated, zSHORT *pUpdatedFile,
                    zSHORT nDoReset );
zSHORT
fnValidateInstanceAttributes( zVIEW             lpView,
                              LPVIEWENTITY      lpViewEntity,
                              LPENTITYINSTANCE  lpEntityInstance );

//./ ADD NAME=fnVersionedForUpdate
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnVersionedForUpdate
//
// PURPOSE: Call only by CreateTemporalSubobjectVersion.  It goes up,
//          down, and around the chains, to see if there is a difference
//          in the record pointer.  If any found this EntityInstance is
//          versioned for update!
//
// RETURNS:  0 - EI not versioned for update
//           1 - EI versioned for update
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnVersionedForUpdate( LPENTITYINSTANCE lpEI,
                      zPVOID lpRecord,
                      zSHORT nDirection )
{
   LPENTITYINSTANCE  lpEI1;

   if ( lpRecord != zGETPTR( lpEI->hPersistRecord ) )
      return( 1 );

   if ( nDirection )
   {
      if ( lpEI->u.nInd.bPrevVersion && lpEI->hNextVsn )
      {
         if ( fnVersionedForUpdate( zGETPTR( lpEI->hNextVsn ),
                                    lpRecord, nDirection ))
         {
            return( 1 );
         }

         lpEI1 = zGETPTR( lpEI->hNextLinked );
         while ( lpEI1 && lpEI1 != lpEI )
         {
            if ( lpEI1->hNextVsn &&
                 fnVersionedForUpdate( zGETPTR( lpEI1->hNextVsn ),
                                       lpRecord, nDirection ) )
            {
               return( 1 );
            }

            lpEI1 = zGETPTR( lpEI1->hNextLinked );
         }
      }

      return( 0 );
   }

   if ( lpEI->u.nInd.bPrevVersion && lpEI->hPrevVsn )
   {
      if ( fnVersionedForUpdate( zGETPTR( lpEI->hPrevVsn ),
                                 lpRecord, nDirection ) )
      {
         return( 1 );
      }

      lpEI1 = zGETPTR( lpEI->hNextLinked );
      while ( lpEI1 && lpEI1 != lpEI )
      {
         if ( lpEI1->hPrevVsn &&
              fnVersionedForUpdate( zGETPTR( lpEI1->hPrevVsn ),
                                    lpRecord, nDirection ) )
         {
            return( 1 );
         }

         lpEI1 = zGETPTR( lpEI1->hNextLinked );
      }
   }

   return( 0 );
}

//./ ADD NAME=CreateTemporalSubobjectVersion
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CreateTemporalSubobjectVersion
//
//  PURPOSE:    To create a new version of a subobject which may either
//              be kept or dropped later on using the operations
//              AcceptSubobject or CancelSubobject.
//
//              RULES:
//
//                1- No instance may be versioned which is already
//                   Versioned down a different path (the instance will
//                   be marked as a previous version without any
//                   lpPrevVersion link for the instance NOR any of
//                   its parents)
//                2- No instance may be versioned if it is already
//                   versioned or any one of its descendents is versioned
//
//
//  DESCRIPTION:  This operation takes a subobject instance and duplicates
//                the entity instances and their records and places the
//                new structure in the Object instance chain. All linked
//                instances are removed from the new chain and marked
//                as a previous version so no updates may be done
//                to them until the new version is either accepted or
//                dropped.
//
//  RETURNS:    0           - Subobject SuccessfullyVersioned
//              zCALL_ERROR - Error versioning Subobject
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
CreateTemporalSubobjectVersion( zVIEW lpView, zCPCHAR cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPTASK           lpAllocTask;
   LPTASK           lpInstAllocTask;
   LPVIEWOI         lpViewOI;
   LPVIEWOD         lpViewOD;
   LPVIEWCSR        lpViewCsr;
   LPVIEWCSR        lpWrkViewCsr;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITYCSR  lpWrkViewEntityCsr;
   LPENTITYINSTANCE lpStartOldInstance;
   LPENTITYINSTANCE lpOldInstance;
   LPENTITYINSTANCE lpStartNewInstance;
   zPVOID           hStartNewInstance;
   LPENTITYINSTANCE lpLastOldInstance;
   LPENTITYINSTANCE lpLastNewInstance;
   LPENTITYINSTANCE lpNewInstance;
   zPVOID           hInstance;
   LPENTITYINSTANCE lpWrkInstance;
   LPENTITYINSTANCE lpDupInstance;
   zSHORT           nStartLevel;

   // Validate parameters.
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntity,
                                      &lpViewEntityCsr,
                                      iCreateTemporalSubobjectVersion,
                                      lpView,
                                      cpcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // If the view is ReadOnly, return.
   if ( lpView->bReadOnly )
   {
      fnOperationReturn( iCreateTemporalSubobjectVersion, lpCurrentTask );
      return( 0 );  // We think everything was OK
   }

   // Get starting level for create.
   nStartLevel = lpViewEntityCsr->nLevel;

   // Get view csr and view oi.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   lpViewOD  = zGETPTR( lpView->hViewOD );

   // Get task which owns the object instance.
   lpAllocTask     = zGETPTR( lpViewOI->hAllocTask );
   lpInstAllocTask = zGETPTR( lpViewOI->hTask );

   // Get instance on which to start.
   lpStartOldInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Check to ensure neither this instance nor any of its descendents is already
   // versioned. This is true when lpPrevVsn points to a versioned root instance.
   lpOldInstance = lpStartOldInstance;
   do
   {
      LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpOldInstance->hPrevVsn );
      LPVIEWENTITY     lpOldViewEntity = zGETPTR( lpOldInstance->hViewEntity );

      if ( lpOldInstance->u.nInd.bTemporal ||
           (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
      {
         if ( lpOldInstance == lpStartOldInstance )
         {
            // "KZOEE136 - Entity Instance already versioned"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 136, 0,
                              lpViewEntity->szName, 0 );
         }
         else
         {
            // "KZOEE137 - Entity Instance descendent versioned"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 137, 0,
                              lpViewEntity->szName,
                              lpOldViewEntity->szName );
         }

         fnOperationReturn( iCreateTemporalSubobjectVersion, lpCurrentTask );
         return( zCALL_ERROR );
      }

      // If the instance to be versioned is already versioned, make sure
      // it is not versioned down a different path or through a different
      // object than the current object and path we are on. If this is
      // true, the the instance will be marked as a previous version yet
      // it will not have a previous version entity instance pointer.
      //
      // We only care about this if the instance being versioned may be
      // updated and there is already a versioned instance down a
      // different path that may be updated.

      if ( lpOldInstance->u.nInd.bPrevVersion &&
           lpOldViewEntity->bUpdate &&
           lpOldInstance->hNextLinked )
      {
         // We have found an instance marked as a previous version
         // which does not have a previous version itself, this means
         // that the instance is versioned down a linked path, go
         // through the linked versions to see if the record itself
         // was versioned for updating. If NOT, then we can still version
         // this instance.
         lpWrkInstance = zGETPTR( lpOldInstance->hNextLinked );
         while ( lpWrkInstance && lpWrkInstance != lpOldInstance )
         {
            LPENTITYINSTANCE lpNextVsn = zGETPTR( lpWrkInstance->hNextVsn );

            if ( lpNextVsn &&
                 lpNextVsn->hPersistRecord != lpOldInstance->hPersistRecord )
            {
               break;
            }

            lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
         }

         if ( lpWrkInstance != lpOldInstance )
         {
            // TB742
            // to avoid a crash within zGETPTR we can either check
            // lpWrkInstance against NULL or remove this obsolete code.
//          LPVIEWENTITY lpWrkViewEntity =
//                                 zGETPTR( lpWrkInstance->hViewEntity );

            // "KZOEE134 - Attempt to version an Entity Instance "
            // "           versioned via a different path"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 134, 0,
                              lpViewEntity->szName,
                              lpViewEntity->szName );
            fnOperationReturn( iCreateTemporalSubobjectVersion, lpCurrentTask );
            return( zCALL_ERROR );
         }
      }

      lpOldInstance = zGETPTR( lpOldInstance->hNextHier );

   } while ( lpOldInstance && lpOldInstance->nLevel > nStartLevel );

   // All error checking is complete, Version the source instances and
   // all of its dependents.
   lpOldInstance      = lpStartOldInstance;
   lpStartNewInstance = 0;
   hStartNewInstance  = 0;
   do
   {
      LPVIEWENTITY lpOldViewEntity = zGETPTR( lpOldInstance->hViewEntity );
      LPVIEWENTITY lpNewViewEntity;

      hInstance = fnAllocDataspace( lpInstAllocTask->hFirstDataHeader,
                                    sizeof( EntityInstanceRecord ), 1, 0,
                                    iEntityInstance );
      lpNewInstance = zGETPTR( hInstance );
      if ( lpNewInstance == 0 )
      {
         // "KZOEE012 - Error allocating memory"
         fnIssueCoreError( lpCurrentTask, lpView, 12, 20, 0, 0, 0 );
         break;
      }

      // Copy old instance information to new instance.
      zmemcpy( lpNewInstance, lpOldInstance,
               sizeof( EntityInstanceRecord ) );

      // If this is not the first instance versioned.
      if ( lpStartNewInstance )
      {
         LPENTITYINSTANCE lpOldParent = zGETPTR( lpOldInstance->hParent );

         lpLastNewInstance->hNextHier = hInstance;
         lpNewInstance->hPrevHier = zGETHNDL( lpLastNewInstance );
         lpNewInstance->hParent   = lpOldParent->hNextVsn;
      // if ( lpNewInstance->hParent == UNSET_CSR )
      //    SysMessageBox( 0, "CreateTemporalSubobjectVersion", "UNSET_CSR", -1 );

         if ( lpOldInstance->hPrevTwin )
         {
            LPENTITYINSTANCE lpPrevTwin;

            lpPrevTwin = zGETPTR( lpOldInstance->hPrevTwin );
            lpNewInstance->hPrevTwin = lpPrevTwin->hNextVsn;

            lpPrevTwin = zGETPTR( lpNewInstance->hPrevTwin );
            lpPrevTwin->hNextTwin = hInstance;
         }
      }
      else
      {
         LPENTITYINSTANCE lpWork;

         hStartNewInstance  = hInstance;
         lpStartNewInstance = lpNewInstance;

         if ( lpOldInstance->hPrevHier )
         {
            lpWork = zGETPTR( lpOldInstance->hPrevHier );
            lpWork->hNextHier = hInstance;
         }

         if ( lpOldInstance->hPrevTwin )
         {
            lpWork = zGETPTR( lpOldInstance->hPrevTwin );
            lpWork->hNextTwin = hInstance;
         }

         if ( lpOldInstance->hNextTwin )
         {
            lpWork = zGETPTR( lpOldInstance->hNextTwin );
            lpWork->hPrevTwin = hInstance;
         }
      }

      lpNewInstance->hPrevVsn = zGETHNDL( lpOldInstance );
      lpOldInstance->hNextVsn = hInstance;
      lpNewInstance->hNextLinked  = 0;

      // Set the previous version indicators for both the old and new versions.
      // If the old version is linked, mark all of its linked instances as a
      // previous version.
      lpNewInstance->u.nInd.bPrevVersion = FALSE;
      lpNewInstance->u.nInd.bPrevVsnRoot = FALSE;
      lpOldInstance->u.nInd.bPrevVersion = TRUE;
      lpWrkInstance = zGETPTR( lpOldInstance->hNextLinked );
      lpDupInstance = 0;
      while ( lpWrkInstance && lpWrkInstance != lpOldInstance )
      {
         // If the linked instance has a next version, see if that instances next
         // version is a child of the starting parent of this version being created.
         if ( lpWrkInstance->hNextVsn )
         {
            lpDupInstance = zGETPTR( lpWrkInstance->hNextVsn );
            while ( lpDupInstance && lpDupInstance != lpStartNewInstance )
               lpDupInstance = zGETPTR( lpDupInstance->hParent );

            if ( lpDupInstance )
               lpDupInstance = zGETPTR( lpWrkInstance->hNextVsn );
         }

         lpWrkInstance->u.nInd.bPrevVersion = TRUE;
         lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
      }

      // Copy the attribute record UNLESS this instance is a duplicate of another
      // instance in the same Subobject OR the view entity is not updateable.
      lpNewViewEntity = zGETPTR( lpNewInstance->hViewEntity );
      if ( lpDupInstance || lpNewViewEntity->bUpdate == FALSE )
      {
         if ( lpDupInstance )
         {
            if ( lpDupInstance->hPersistRecord == lpOldInstance->hPersistRecord &&
                 lpNewViewEntity->bUpdate )
            {
               lpDupInstance->hPersistRecord =
                 fnCopyAttributeRecord( lpAllocTask, lpOldViewEntity,
                                        (zPCHAR) zGETPTR( lpOldInstance->hPersistRecord ), 1 );

               lpWrkInstance = zGETPTR( lpDupInstance->hNextLinked );
               while ( lpWrkInstance && lpWrkInstance != lpDupInstance )
               {
                  lpWrkInstance->hPersistRecord = lpDupInstance->hPersistRecord;
                  lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
               }
            }

            lpNewInstance->hPersistRecord = lpDupInstance->hPersistRecord;
            if ( lpDupInstance->hNextLinked )
               lpNewInstance->hNextLinked = lpDupInstance->hNextLinked;
            else
               lpNewInstance->hNextLinked = zGETHNDL ( lpDupInstance );

            lpDupInstance->hNextLinked = hInstance;
         }
      }
      else
      {
         lpNewInstance->hPersistRecord =
           fnCopyAttributeRecord( lpAllocTask, lpOldViewEntity,
                                  (zPCHAR) zGETPTR( lpOldInstance->hPersistRecord ), 1 );
      }

      lpNewInstance->hNonPersistRecord =
        fnCopyAttributeRecord( lpAllocTask, lpOldViewEntity,
                               (zPCHAR) zGETPTR( lpOldInstance->
                                                    hNonPersistRecord ), 0 );
      lpLastOldInstance = lpOldInstance;
      lpLastNewInstance = lpNewInstance;
      lpOldInstance = zGETPTR( lpOldInstance->hNextHier );

      fnAddEntityToDebugChange( lpNewInstance );

   } while ( lpOldInstance && lpOldInstance->nLevel > nStartLevel );

   if ( lpNewInstance == 0 ) // if any alloc failed, we're in trouble
   {
      // We will clean up here and undo the damage we've done to
      // the old instances.
      if ( lpStartNewInstance )
      for ( ; ; )
      {
         lpWrkInstance = zGETPTR( lpStartNewInstance->hNextVsn );
         lpWrkInstance->hPrevVsn = lpStartNewInstance->hPrevVsn;
         lpWrkInstance->u.nInd.bPrevVersion = FALSE;
         lpWrkInstance = lpStartNewInstance;
         hStartNewInstance  = lpStartNewInstance->hNextHier;
         lpStartNewInstance = zGETPTR( lpStartNewInstance->hNextHier );
         fnDeleteAttributeRecord( zGETPTR( lpWrkInstance->hViewEntity ),
                                  lpWrkInstance, TRUE );

         lpDupInstance = zGETPTR( lpWrkInstance->hNextLinked );
         if ( lpDupInstance )
         {
            while ( lpDupInstance != lpWrkInstance )
            {
               lpDupInstance->hPersistRecord = 0;
               lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
            }
         }

         fnFreeDataspace( lpWrkInstance );
         if ( lpWrkInstance == lpLastNewInstance )
            break;
      }

      fnOperationReturn( iCreateTemporalSubobjectVersion, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Mark the starting initial instance as a previous version root.
   lpStartOldInstance->u.nInd.bPrevVsnRoot = TRUE;
   lpStartOldInstance->hPrevHier = 0;
   lpStartOldInstance->hNextTwin = 0;
   lpStartOldInstance->hPrevTwin = 0;

   lpLastOldInstance->hNextHier = 0;

   // If lpOldInstance exists set its previous hierarchical pointer.
   if ( lpOldInstance )
      lpOldInstance->hPrevHier = hInstance;

   // If the root of the instance is being versioned, set the
   // root entity instance pointer in the ViewOI.
   if ( zGETPTR( lpViewOI->hRootEntityInstance ) == lpStartOldInstance )
      lpViewOI->hRootEntityInstance = hStartNewInstance;

   // Set the cursor to the new instance
   lpViewEntityCsr->hEntityInstance = hStartNewInstance;
// if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
//    SysMessageBox( 0, "CreateTemporalSubobjectVersion", "UNSET_CSR", -1 );

   fnResetCursorForViewChildren( lpViewEntityCsr );

   // Now go through all views for the old object and update any cursors
   // positioned on the previous instance to point to the new version.
   for ( lpWrkViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );
         lpWrkViewCsr;
         lpWrkViewCsr = zGETPTR( lpWrkViewCsr->hNextViewCsr ) )
   {
      if ( lpWrkViewCsr == lpViewCsr )
         continue;

      lpWrkViewEntityCsr = zGETPTR( lpWrkViewCsr->hFirstOD_EntityCsr );
      lpWrkViewEntityCsr += (lpViewEntity->nHierNbr - 1);

      if ( zGETPTR( lpWrkViewEntityCsr->hEntityInstance ) == lpStartOldInstance )
      {
         lpWrkViewEntityCsr->hEntityInstance = hStartNewInstance;
      // if ( lpWrkViewEntityCsr->hEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "CreateTemporalSubobjectVersion2", "UNSET_CSR", -1 );

         fnResetCursorForViewChildren( lpWrkViewEntityCsr );
      }
   }

   // Increment versioned instance count in viewoi.
   lpViewOI->nVersionedInstances++;

#if 0   // Gig, debugging, for all new instances, see if a record
        // match can be found which is not linked!!!!
   lpNewInstance      = lpStartNewInstance;
   do
   {
      lpWrkInstance = zGETPTR( lpNewInstance->hNextHier );
      while ( lpWrkInstance && lpWrkInstance->nLevel > nStartLevel )
      {
         if ( lpWrkInstance->hPersistRecord == lpNewInstance->hPersistRecord )
         {
            lpDupInstance = zGETPTR( lpNewInstance->hNextLinked );
            while ( lpDupInstance && lpDupInstance != lpNewInstance )
            {
               if ( lpDupInstance == lpWrkInstance )
                  break;

               lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
            }

            if ( lpDupInstance != lpWrkInstance )
            {
               SysMessageBox( "ERROR AFTER CREATE",
                              "Record match found in non-linked instance", 1 );
            }
         }

         lpWrkInstance = zGETPTR( lpWrkInstance->hNextHier );
      }

      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );

   } while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel );
#endif

   // return to sender
   fnOperationReturn( iCreateTemporalSubobjectVersion, lpCurrentTask );
   return( 0 );  // We think everything was OK
}

//./ ADD NAME=AcceptAllTemporalSubobjects
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      AcceptAllTemporalSubobjects
//
//  PURPOSE:    To Accept all outstanding temporal subobjects in the
//              Object Instance
//
//  RETURNS:    0           - All Temporal Subobject versions successfully
//                            accepted
//              zCALL_ERROR - Error accepting a temporal version of
//                            Subobject
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
AcceptAllTemporalSubobjects( zVIEW lpView )
{
   LPTASK           lpCurrentTask;
   LPVIEWOI         lpViewOI;
   LPVIEWCSR        lpViewCsr;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpRoot;
   LPVIEWENTITY     lpRootViewEntity;
   zVIEW            vWork;
   zSHORT           nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iAcceptAllTemporalSubobjects,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   if ( lpViewOI->nVersionedInstances )
   {
      lpRoot = zGETPTR( lpViewOI->hRootEntityInstance );
      lpRootViewEntity = zGETPTR( lpRoot->hViewEntity );
      vWork = 0;
      while ( lpRoot )
      {
         LPENTITYINSTANCE lpPrevVsn;

         nRC = fnRemoveAllChildTemporalVsns( lpView, lpRoot, 1 );
         if ( nRC < zCURSOR_UNDEFINED )
         {
            if ( vWork )
               DropView( vWork );

            fnOperationReturn( iAcceptAllTemporalSubobjects, lpCurrentTask );
            return( nRC );
         }

         lpPrevVsn = zGETPTR( lpRoot->hPrevVsn );
         if ( lpRoot->u.nInd.bTemporal ||
              (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
         {
            if ( vWork == 0 )
               CreateViewFromViewForTask( &vWork, lpView, 0 );

            lpViewEntityCsr = fnEstablishViewForInstance( vWork, 0, lpRoot );
            nRC = AcceptSubobject( vWork, lpRootViewEntity->szName );
            if ( nRC < zCURSOR_UNDEFINED )
            {
               if ( vWork )
                  DropView( vWork );

               fnOperationReturn( iAcceptAllTemporalSubobjects, lpCurrentTask );
               return( nRC );
            }

            lpRoot = zGETPTR( lpViewEntityCsr->hEntityInstance );
         }

         if ( lpRoot )
            lpRoot = zGETPTR( lpRoot->hNextTwin );
      }

      if ( vWork )
         DropView( vWork );
   }

   fnOperationReturn( iAcceptAllTemporalSubobjects, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=fnUpdateNextVsnRecordPointers
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnUpdateNextVsnRecordPointers
//
//  PURPOSE:    To update versioned/linked instances.
//              Called by AcceptSubobject.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
void
fnUpdateNextVsnRecordPointers( LPENTITYINSTANCE  lpNextVsnInstance,
                               LPENTITYINSTANCE  lpNewInstance,
                               zPVOID            lpOldRecord,
                               zBOOL             bStillPrevVsn )
{
   LPENTITYINSTANCE  lpDupInstance;
   LPENTITYINSTANCE  lpNextVsn;
   LPVIEWENTITY      lpViewEntity = zGETPTR( lpNewInstance->hViewEntity );

   lpDupInstance = lpNextVsnInstance;
   do
   {
      lpDupInstance->hPersistRecord = lpNewInstance->hPersistRecord;
      if ( lpNewInstance->u.nInd.bUpdated )
         lpDupInstance->u.nInd.bUpdated = TRUE;

      lpDupInstance->u.nInd.bPrevVersion = bStillPrevVsn;

      // If the duplicate instance has newer version(s) with the old record,
      // update the newer version(s) and it linked instances.
      lpNextVsn = zGETPTR( lpDupInstance->hNextVsn );
      if ( lpNextVsn &&
           zGETPTR( lpNextVsn->hPersistRecord ) == lpOldRecord )
      {
         fnUpdateNextVsnRecordPointers( lpNextVsn,
                                        lpNewInstance, lpOldRecord,
                                        bStillPrevVsn );
      }

      lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );

   } while ( lpDupInstance && lpDupInstance != lpNextVsnInstance );
}

//./ ADD NAME=fnGetRecurseParentEntity
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnGetRecurseParentEntity
//
// PURPOSE: If input view entity is a recursive entity, return the parent
//          that is the object of the recursion, else just return the
//          input veiw entity.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPVIEWENTITY
fnGetRecurseParentEntity( LPVIEWENTITY lpViewEntity )
{
   LPVIEWENTITY   lpScanViewEntity;

   if ( lpViewEntity->bRecursive == FALSE )
      return( lpViewEntity );

   lpScanViewEntity = zGETPTR( lpViewEntity->hParent );
   while ( lpScanViewEntity->lEREntTok != lpViewEntity->lEREntTok )
      lpScanViewEntity = zGETPTR( lpScanViewEntity->hParent );

   return( lpScanViewEntity );
}

//./ ADD NAME=fnCountEntities
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnCountEntities
//
// PURPOSE: Count the number of entities in the twin chain that are not
//          hidden.
//
// RETURNS:
//          The number of unhidden entities in the twin chain for a
//          specific entity instance.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zUSHORT
fnCountEntities( LPENTITYINSTANCE lpEntityInstance )
{
   LPENTITYINSTANCE  lpScanEI;
   zUSHORT           uEICount = 0;

   if ( lpEntityInstance == 0 )
      return( 0 );

   if ( lpEntityInstance->u.nInd.bHidden == FALSE )
      uEICount++; // count the current instance

   // Scan to the head of the EI chain.
   lpScanEI = zGETPTR( lpEntityInstance->hPrevTwin );
   while ( lpScanEI )
   {
      if ( lpScanEI->u.nInd.bHidden == FALSE )
         uEICount++;

      lpScanEI = zGETPTR( lpScanEI->hPrevTwin );
   }

   // Scan to the tail of the chain.
   lpScanEI = zGETPTR( lpEntityInstance->hNextTwin );
   while ( lpScanEI )
   {
      if ( lpScanEI->u.nInd.bHidden == FALSE )
         uEICount++;

      lpScanEI = zGETPTR( lpScanEI->hNextTwin );
   }

   return( uEICount );
}

//./ ADD NAME=fnValidateCardinality
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnValidateCardinality
//
// PURPOSE:
//          If Created or Temporal Entity Instance, ensure that it and
//          its twins do not exceed max cardinality
//
//          If Deleted or Excluded, make sure its twins >= Minimum
//          cardinality.
//
//
// RETURNS:
//         -2 - No child EI, when view enity indicates required child
//         -1 - cardinality min violation.
//          0 - cardinality okay or not required.
//          1 - cardinality max violation.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnValidateCardinality( zVIEW              lpView,
                       LPVIEWENTITY       lpViewEntity,
                       LPENTITYINSTANCE   lpEntityInstance,
                       zBOOL              bCount )
{
   LPENTITYINSTANCE lpNextHier;
   // If Created or Temporal Entity Instance, ensure that it and its twins
   // do not exceed max cardinality.
   //
   // If Deleted or Excluded, make sure its twins >= Min cardinality.
   //
   zUSHORT        uMax, uMin;
   zCHAR          szRel[ 255 ];

   uMax = lpViewEntity->uCardMax;
   uMin = lpViewEntity->uCardMin;

   if ( bCount && (uMax || uMin) )
   {
      zUSHORT  un = fnCountEntities( lpEntityInstance );

      if ( uMin && un < uMin )
         return( -1 );
      else
      if ( uMax && un > uMax )
         return( 1 );
   }

#if 1
   // If there are no children for this entity instance, but the
   // the view entity indicates there is at least one required
   // child at the next level, scan the view entities for the
   // immediate children of this entity and display error for
   // the first missing required entity.
   lpNextHier = zGETPTR( lpEntityInstance->hNextHier );
   if ( lpNextHier == 0 ||
        lpNextHier->nLevel <= lpEntityInstance->nLevel )
   {
      LPVIEWENTITY   lpRPViewEntity;
      zVIEW          vErrView;

      lpRPViewEntity = fnGetRecurseParentEntity( lpViewEntity );
      if ( lpRPViewEntity->bRequiredChild )
      {
         LPVIEWENTITY   lpScanViewEntity;
         zSHORT         nVEStopLevel = lpRPViewEntity->nLevel;
         zSHORT         nVELevel = nVEStopLevel + 1;

         lpScanViewEntity = zGETPTR( lpRPViewEntity->hNextHier );
         while ( lpScanViewEntity &&
                 lpScanViewEntity->nLevel > nVEStopLevel )
         {
            if ( lpScanViewEntity->nLevel == nVELevel &&
                 lpScanViewEntity->uCardMin )
            {
               CreateViewFromViewForTask( &vErrView, lpView, 0 );
               fnEstablishViewForInstance( vErrView, 0, lpEntityInstance );
               SetNameForView( vErrView, "KZOEE106", lpView, zLEVEL_TASK );
               TraceLineS( "Browse view named 'KZOEE106' to see what entity "
                           "instance has the error.  Entity name = ",
                           lpViewEntity->szName );

               strcpy_s( szRel, zsizeof( szRel ), "\nEntity: \t" );
               strcat_s( szRel, zsizeof( szRel ), lpScanViewEntity->szName );

               // "KZOEE106 - Rules violation"
               IssueOE_Error( lpView, 8, 106,
                              zMSGQ_OBJECT_CONSTRAINT_ERROR,
                              0,
                              "number of entities less than minimum cardinality",
                              szRel );

               fnDropView( vErrView );

               return( -2 );
            }

            lpScanViewEntity = zGETPTR( lpScanViewEntity->hNextHier );
         }

         strcpy_s( szRel, zsizeof( szRel ), "\nEntity: \t" );
         strcat_s( szRel, zsizeof( szRel ), lpViewEntity->szName );

         CreateViewFromViewForTask( &vErrView, lpView, 0 );
         fnEstablishViewForInstance( vErrView, 0, lpEntityInstance );
         SetNameForView( vErrView, "KZOEE106", lpView, zLEVEL_TASK );
         TraceLineS( "Browse view named 'KZOEE106' to see what entity "
                     "instance has the error.  Entity name = ",
                     lpViewEntity->szName );

         // "KZOEE106 - Rules violation"
         IssueOE_Error( lpView, 8, 106, 0,
                        zMSGQ_OBJECT_CONSTRAINT_ERROR,
                        "Marked as having a required child, but none found...",
                        szRel );
         return( -2 );
      }

      return( 0 ); // no required children, all done...
   }
#endif

   return( 0 );
}

//./ ADD NAME=fnCheckChildEntities
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnCheckChildEntities
//
// PURPOSE: Recursive procedure to check all the child entities and
//          instances for required attributes and cardinality
//          violations.
//          Only works on the children of the entity instance that is
//          input to the function.  If child entity instances are present
//          the function calls itself.
//
// RETURNS:
//       Updates the EntityInstance pointer...
//
//                       0 - no constraint violations.
//   zCONSTRAINT_VIOLATION - constraint violations.
//             zCALL_ERROR - Operation failed.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnCheckChildEntities( zVIEW            lpView,
                      LPENTITYINSTANCE *lpEntityInstance,
                      zBOOL            bChkAllEntitiesAtChildLvl )
{
   zSHORT            nStopLevel;
   zSHORT            nCurrLevel;
   zSHORT            nEntityStopLevel;
   zSHORT            nEntityLevel;
   zSHORT            nCardinalityViolation;
   LPVIEWENTITY      lpWrkViewEntity1;
   LPVIEWENTITY      lpWrkViewEntity2;
   LPENTITYINSTANCE  lpWrkEI1;
// LPENTITYINSTANCE  lpWrkEI2;
   zBOOL             bCount = TRUE;
   zCHAR             szRel[ 255 ];


   // Scan through all the child instances, looking for attributes,
   // marked required and not marked hidden, that have a 'NULL' value.
   lpWrkEI1         = *lpEntityInstance;
   lpWrkViewEntity1 = zGETPTR( lpWrkEI1->hViewEntity );
   nEntityStopLevel = lpWrkViewEntity1->nLevel;
   nStopLevel       = lpWrkEI1->nLevel;
   nCurrLevel       = nStopLevel + 1;
   lpWrkEI1         = zGETPTR( lpWrkEI1->hNextHier );

   // First child ViewEntity
   lpWrkViewEntity2 = zGETPTR( lpWrkViewEntity1->hNextHier );
   nEntityLevel = lpWrkViewEntity2->nLevel;

   while ( lpWrkEI1 && lpWrkEI1->nLevel > nStopLevel )
   {
      LPVIEWENTITY lpRPViewEntity;
      LPVIEWENTITY lpViewEntity;

      if ( lpWrkEI1->u.nInd.bHidden == FALSE )
      {
         LPENTITYINSTANCE lpNextHier;

         lpViewEntity = zGETPTR( lpWrkEI1->hViewEntity );

         // Look for possible missing entities that maybe required.
         // Only check one level down ...
         if ( bChkAllEntitiesAtChildLvl &&
              lpViewEntity != lpWrkViewEntity2 )
         {
            while ( lpWrkViewEntity2 &&
                    lpWrkViewEntity2->nHierNbr < lpViewEntity->nHierNbr )
            {
               if ( lpWrkViewEntity2->nLevel == nEntityLevel &&
                    lpWrkViewEntity2->uCardMin )
               {
                  // create a string which describes the relationship:
                  strcpy_s( szRel, zsizeof( szRel ), "\nRelationship: \t" );
                  strcat_s( szRel, zsizeof( szRel ), lpWrkViewEntity1->szName );
                  strcat_s( szRel, zsizeof( szRel ), " <--> " );
                  strcat_s( szRel, zsizeof( szRel ), lpWrkViewEntity2->szName );

                  // "KZOEE106 - Rules violation"

                  IssueOE_Error( lpView, 8, 106, 0,
                                 zMSGQ_OBJECT_CONSTRAINT_ERROR,
                                 "number of entities less than minimum cardinality",
                                 szRel );
                  return( zCONSTRAINT_VIOLATION );
               }

               lpWrkViewEntity2 = zGETPTR( lpWrkViewEntity2->hNextHier );
            }
         }

         if ( lpWrkViewEntity2 &&
              lpWrkViewEntity2->nHierNbr == lpViewEntity->nHierNbr )
         {
            lpWrkViewEntity2 = zGETPTR( lpWrkViewEntity2->hNextHier );
         }

         if ( fnValidateInstanceAttributes( lpView, lpViewEntity, lpWrkEI1 ) )
            return( zCONSTRAINT_VIOLATION );

         // Even though we have passed the cardinality check at the
         // 'twin' level, we need to call this again for possible
         // missing children of the current instance ...
         nCardinalityViolation = fnValidateCardinality( lpView,
                                                        lpViewEntity,
                                                        lpWrkEI1, bCount );
         bCount = 0;
         if ( nCardinalityViolation )
         {
            // "KZOEE106 - Rules violation"
            if ( nCardinalityViolation != -2 )
            {

               strcpy_s( szRel, zsizeof( szRel ), "\nEntity: \t" );
               strcat_s( szRel, zsizeof( szRel ), lpViewEntity->szName );

               IssueOE_Error( lpView, 8, 106, 0,
                              zMSGQ_OBJECT_CONSTRAINT_ERROR,
                              (nCardinalityViolation < 0 ?
                                "number of entities less than minimum cardinality" :
                                "maximum cardinality exceeded"),
                              szRel );
            }

            return( zCONSTRAINT_VIOLATION );
         }

         // Recursive Parent entity only used to determine if there are any
         // Required children ...
         lpRPViewEntity = fnGetRecurseParentEntity( lpViewEntity );
         lpNextHier = zGETPTR( lpWrkEI1->hNextHier );
         if ( lpNextHier && lpNextHier->nLevel > nCurrLevel )
         {
            zSHORT nRC;

            nRC = fnCheckChildEntities( lpView, &lpWrkEI1,
                                        (zBOOL) lpRPViewEntity->bRequiredChild );
            bCount = TRUE;
            if ( nRC )
               return( nRC );

            continue;
         }
         else
         {
            // If different entity type, turn on the count flag ...
            if ( lpNextHier && lpWrkEI1->hViewEntity != lpNextHier->hViewEntity )
               bCount = TRUE;

            lpWrkEI1 = lpNextHier;
         }
      }
      else
         lpWrkEI1 = zGETPTR( lpWrkEI1->hNextHier );
   }

   // Look for possible missing entities that maybe required.
   if ( bChkAllEntitiesAtChildLvl && lpWrkViewEntity2 &&
        lpWrkViewEntity2->nLevel > nEntityStopLevel  )
   {
      while ( lpWrkViewEntity2 &&
              lpWrkViewEntity2->nLevel > nEntityStopLevel )
      {
         if ( lpWrkViewEntity2->nLevel == nEntityLevel &&
              lpWrkViewEntity2->uCardMin )
         {
            // Create a string which describes the relationship.
            strcpy_s( szRel, zsizeof( szRel ), "\nRelationship: \t" );
            strcat_s( szRel, zsizeof( szRel ), lpWrkViewEntity1->szName);
            strcat_s( szRel, zsizeof( szRel ), " <--> " );
            strcat_s( szRel, zsizeof( szRel ), lpWrkViewEntity2->szName);

            // "KZOEE106 - Rules violation"
            IssueOE_Error( lpView, 8, 106, 0,
                           zMSGQ_OBJECT_CONSTRAINT_ERROR,
                           "number of entities less than minimum cardinality",
                           szRel );
            return( zCONSTRAINT_VIOLATION );
         }

         lpWrkViewEntity2 = zGETPTR( lpWrkViewEntity2->hNextHier );
      }
   }

   *lpEntityInstance = lpWrkEI1;
   return( 0 );
}

//./ ADD NAME=fnOEAcceptConstraints
// Source Module=kzoeeiaa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnOEAcceptConstraints
//
// PURPOSE: Two types of core constraints are verified in this code.
//
//          1. Required Attributes.  For the entity instance being
//          accepted and all of its child entity instances, verify that
//          any required attribute, that is not hidden, does not have a
//          'NULL' value.
//
//          2. Entity Cardinality. Verify that minimum and maximum card-
//          inality values have not been exceeded for the Entity (or any
//          child entities).
//
// RETURNS:
//                       0 - no constraint violations.
//   zCONSTRAINT_VIOLATION - constraint violations.
//             zCALL_ERROR - Operation failed.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnOEAcceptConstraints( zVIEW             lpView,
                       LPVIEWENTITYCSR   lpViewEntityCsr,
                       zBOOL             bCheckChildEntities )
{
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpNextHier;
   LPVIEWSUBOBJECT   lpViewSubobject;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpRecursViewEntity = 0;
   LPVIEWCSR         lpViewCsr;
   zSHORT            nCardinalityViolation;
   zSHORT            nRC = 0;

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   lpViewEntity     = zGETPTR( lpViewEntityCsr->hViewEntity );
   lpViewCsr        = zGETPTR( lpViewEntityCsr->hViewCsr );
   lpViewSubobject  = zGETPTR( lpViewCsr->hFirstViewSubobject );

   if ( fnValidateInstanceAttributes( lpView, lpViewEntity,
                                      lpEntityInstance ) )
   {
      return( zCONSTRAINT_VIOLATION );
   }

   // If the lpViewEntityCsr is the Root cursor of a subobject view, then
   // use the RecursiveViewEntity when validating min/max cardinality.
   if ( lpViewSubobject )
   {
      while ( lpViewSubobject )
      {
         if ( zGETPTR( lpViewSubobject->hRootCsr ) == lpViewEntityCsr &&
              lpViewSubobject->hRecursViewEntity )
         {
            lpRecursViewEntity = zGETPTR( lpViewSubobject->hRecursViewEntity );
            break;
         }

         lpViewSubobject = zGETPTR( lpViewSubobject->hNextViewSubobject );
      }
   }

   if ( (nCardinalityViolation =
               fnValidateCardinality( lpView,
                                      ( lpRecursViewEntity ?
                                          lpRecursViewEntity : lpViewEntity ),
                                       lpEntityInstance, 1 ) ) != 0 )
   {
#if 0
      zCHAR szRel[ 255 ];

      // "KZOEE106 - Rules violation"
      if ( nCardinalityViolation != -2 )

         strcpy( szRel, "\nEntity: \t" );
         strcat( szRel, lpViewEntity->szName );

         IssueOE_Error( lpView, 8, 106, 0,
                        zMSGQ_OBJECT_CONSTRAINT_ERROR,
                        (nCardinalityViolation < 0 ?
                           "number of entities less than minimum cardinality" :
                           "maximum cardinality exceeded"),
                        szRel );
#endif

      return( zCONSTRAINT_VIOLATION );
   }

#if 1
   lpNextHier = zGETPTR( lpEntityInstance->hNextHier );
   if ( bCheckChildEntities &&
        lpNextHier &&
        lpNextHier->nLevel == ( lpEntityInstance->nLevel + 1 ) )
   {
      nRC = fnCheckChildEntities( lpView, &lpEntityInstance,
                                  (zBOOL) lpViewEntity->bRequiredChild );
   }
#endif

   return( nRC );
}

//./ ADD NAME=AcceptSubobject
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      AcceptSubobject
//
//  PURPOSE:    To Accept a current version of a Subobject created with
//              CreateTemporalSubobjectVersion
//
//  RETURNS:    0           - Temporal Subobject version successfully
//                            accepted
//              zCALL_ERROR - Error accepting temporal version of Subobject
//
//  NOTE: A WORK IN TEST...
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
AcceptSubobject( zVIEW lpView, zCPCHAR cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPVIEWOI         lpViewOI;
   LPVIEWOD         lpViewOD;
   LPVIEWCSR        lpViewCsr;
   LPVIEWCSR        lpWrkViewCsr;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITYCSR  lpWrkViewEntityCsr;
   LPENTITYINSTANCE lpPrevVsn;
   LPENTITYINSTANCE lpOldInstance;
   LPENTITYINSTANCE lpStartOldInstance;
   LPENTITYINSTANCE lpStartNewInstance;
   LPENTITYINSTANCE lpLastNewInstance;
   LPENTITYINSTANCE lpFirstIncluded;
   LPENTITYINSTANCE lpLastIncluded;
   LPENTITYINSTANCE lpNewInstance;
   LPENTITYINSTANCE lpWrkInstance;
   LPENTITYINSTANCE lpDupInstance;
   zPVOID           lpOldRecord;
   zVIEW            vWorkView;
   zBOOL            bStillPrevVsn;
   zSHORT           nStartLevel;
   zSHORT           nRC = 0;

   // Validate parameters.
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntity,
                                      &lpViewEntityCsr,
                                      iAcceptSubobject,
                                      lpView,
                                      cpcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   lpViewOD  = zGETPTR( lpView->hViewOD );

   // If the view is ReadOnly, return with an error if any versioned
   // instances exist, else return success.
   if ( lpView->bReadOnly )
   {
      nRC = 0;
      if ( lpViewOI->nVersionedInstances )
      {
         nRC = zCALL_ERROR;
         // "KZOEE119 - Invalid View, View is Read Only"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 119, 0, cpcEntityName, 0 );
      }

      fnOperationReturn( iAcceptSubobject, lpCurrentTask );
      return( nRC );
   }

#ifdef __ACTIVATE_CONSTRAINTS__
   // If there is an Entity Constraint for accept, see if it has
   // any objections about continuing.
   if ( lpViewEntity->bAcceptConstraint )
   {
      zSHORT n;

      n = fnInvokeECEOperation( lpView, lpViewEntity, lpCurrentTask,
                                zECE_ACCEPT, 0 );
      // If constraint handler doesn't like something, get out.
      if ( n )
      {
         fnOperationReturn( iAcceptSubobject, lpCurrentTask );
         return( n );
      }
   }

   if ( fnOEAcceptConstraints( lpView, lpViewEntityCsr, 1 ) )
   {
      fnOperationReturn( iAcceptSubobject, lpCurrentTask );
      return( zCONSTRAINT_VIOLATION );
   }
#endif

   // Get instance on which to start.
   lpStartNewInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Get starting level for Accept.
   nStartLevel = lpViewEntityCsr->nLevel;

#if 0   // Gig, debugging, for all new instances, see if a record
        // match can be found which is not linked!!!!
   lpNewInstance = lpStartNewInstance;
   do
   {
      lpWrkInstance = zGETPTR( lpNewInstance->hNextHier );
      while ( lpWrkInstance && lpWrkInstance->nLevel > nStartLevel )
      {
         if ( lpWrkInstance->hPersistRecord == lpNewInstance->hPersistRecord )
         {
            LPENTITYINSTANCE lpPrevVsn;

            for ( lpPrevVsn = lpNewInstance;
                  lpPrevVsn;
                  lpPrevVsn = zGETPTR( lpPrevVsn->hPrevVsn ) )
            {
               lpDupInstance = zGETPTR( lpPrevVsn->hNextLinked );
               while ( lpDupInstance && lpDupInstance != lpPrevVsn )
               {
                  if ( lpDupInstance == lpWrkInstance )
                     break;

                  lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
               }

               if ( lpDupInstance != lpWrkInstance )
               {
                  SysMessageBox( "ERROR BEFORE ACCEPT",
                                 "Record match found in non-linked instance", 1 );
               }
            }
         }

         lpWrkInstance = zGETPTR( lpWrkInstance->hNextHier );
      }

      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );

   } while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel );

#endif

   while ( lpStartNewInstance->hNextVsn )
      lpStartNewInstance = zGETPTR( lpStartNewInstance->hNextVsn );

   // If the entity instance is not versioned, return a logic error.
   lpPrevVsn = zGETPTR( lpStartNewInstance->hPrevVsn );
   if ( lpStartNewInstance->u.nInd.bTemporal == FALSE &&
        (lpPrevVsn == 0 || lpPrevVsn->u.nInd.bPrevVsnRoot == FALSE) )
   {
      fnOperationReturn( iAcceptSubobject, lpCurrentTask );
      return( 0 );
   }

   // Check to make sure none of this instance's descendents are
   // already versioned. This is true when lpPrevVsn points to a
   // versioned root instance.
   lpNewInstance = zGETPTR( lpStartNewInstance->hNextHier );
   if ( lpViewOI->nVersionedInstances > 1 )
      while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel )
      {
         lpPrevVsn = zGETPTR( lpNewInstance->hPrevVsn );

         if ( lpNewInstance->u.nInd.bTemporal ||
              (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
         {
            LPVIEWENTITY lpWork = zGETPTR( lpNewInstance->hViewEntity );

            // "KZOEE137 - Entity Instance descendent versioned"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 137, 0,
                              lpViewEntity->szName,
                              lpWork->szName );
            fnOperationReturn( iAcceptSubobject, lpCurrentTask );
            return( zCALL_ERROR );
         }

         lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
      }


   // If we arrive here, then we must have a previous version of this
   // entity. Implement the current version amongst the linked
   // instances of the previous version and clean up the previous
   // version instances.

   // Get instance on which to start
   lpStartOldInstance = zGETPTR( lpStartNewInstance->hPrevVsn );
   lpNewInstance = lpStartNewInstance;

   // Initialize first instance found which was included OR created
   // in the versioned subobject.
   lpFirstIncluded = 0;

   // Create a work view.
   CreateViewFromViewForTask( &vWorkView, lpView, 0 );

   // For all new versions of the instance in question, put the new
   // version in the linked instance chain and copy the incremental
   // indicators across all linked versions of the indicators.
   if ( lpNewInstance->u.nInd.bTemporal )
   {
      // Turn off the temporal indicator
      lpNewInstance->u.nInd.bTemporal = 0;
      lpDupInstance = zGETPTR( lpNewInstance->hNextLinked );
      while ( lpDupInstance && lpDupInstance != lpNewInstance )
      {
         lpDupInstance->u.nInd.bTemporal = 0;
         lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
      }

      // Spawn the entity create if necessary
      fnSpawnCreateEntity( lpView, lpNewInstance );

      // Now that we have spawned the create, spawn all creates and
      // includes which have taken place beneath the created temporal
      // entity.
      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
      while ( lpNewInstance &&
              lpNewInstance->nLevel > lpStartNewInstance->nLevel )
      {
         if ( lpNewInstance->u.nInd.bIncluded )
         {
            // This next if blocks spawning for the case where an entity
            // has been created and included and then excluded.
            if ( lpNewInstance->u.nInd.bExcluded == FALSE ) // DKS 7/22/96
            {
               lpViewEntityCsr = fnEstablishViewForInstance( vWorkView, 0,
                                                             lpNewInstance );
               fnSpawnIncludeSubobject( vWorkView, lpViewOI,
                                        zGETPTR( lpNewInstance->hViewEntity ),
                                        lpViewEntityCsr, 1 );
            }

            nRC = lpNewInstance->nLevel;
            lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
            while ( lpNewInstance && lpNewInstance->nLevel > nRC )
               lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
         }
         else
         {
            fnSpawnCreateEntity( lpView, lpNewInstance );
            lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
         }
      }

      // Decrement versioned instance count in viewoi.
      lpViewOI->nVersionedInstances--;

      if ( !fnInstanceVersioned( lpStartNewInstance ) )
      {
         zSHORT nUpdated = 0;
         zSHORT nUpdatedFile = 0;

         // Reset the version update flags.
         fnGetResetUpdFlags( lpStartNewInstance, &nUpdated, &nUpdatedFile, 1 );

         // Not versioned after accept.
         lpViewOI->bUpdated = lpViewOI->bUpdatedFile = TRUE;
      }
      else
      {
         // Still versioned after accept.
         // We set the version updated flag to the entity which was
         // just accepted.
         fnSetUpdateIndicator( lpViewOI, lpStartNewInstance,
                               0, TRUE, FALSE, FALSE );
      }

      DropView( vWorkView );
      fnOperationReturn( iAcceptSubobject, lpCurrentTask );
      return( 0 );  // assume everythings AOK if we get here
   }

   // If we're not accepting a temporal entity, then the accept process
   // is more involved - PASS Number 1.
   lpNewInstance = lpStartNewInstance;
   do
   {
      // In the first pass, we issue deletes and excludes on the old
      // version so hiding of instances takes place across all
      // appropriate linked instances of the old version, this is
      // done in a separate loop because the propagation did not
      // work across inverted relationships when done in the loop
      // below - Gig 5/8/93.
      lpOldInstance = zGETPTR( lpNewInstance->hPrevVsn );

      // if the new instance was deleted OR Excluded and the
      // old instance is not currently marked as hidden, issue
      // a Delete OR Exclude on the old instance so all
      // linked versions of the old instance are correctly marked.
      if ( lpOldInstance &&
           lpOldInstance->u.nInd.bHidden == FALSE &&
           (lpNewInstance->u.nInd.bDeleted ||
            lpNewInstance->u.nInd.bExcluded) )
      {
         lpWrkViewEntityCsr = fnEstablishViewForInstance( vWorkView, 0,
                                                          lpOldInstance );
         if ( lpNewInstance->u.nInd.bDeleted )
         {
            fnDeleteEntity( vWorkView,
                            zGETPTR( lpOldInstance->hViewEntity ),
                            lpWrkViewEntityCsr, zREPOS_NONE,
                            lpCurrentTask );
         }
         else
         {
            fnExcludeEntity( vWorkView, zGETPTR( lpOldInstance->hViewEntity ),
                             lpWrkViewEntityCsr, zREPOS_NONE, 1 );
         }
      }

      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );

   }  while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel );

   // If we're not accepting a temporal entity, then the accept process
   // is more involved - PASS Number 2.
   lpNewInstance = lpStartNewInstance;
   do
   {
      // Since we support includes and creates within a new version,
      // see if the current instance represents this situation, if
      // so, we need to re-do the include or create after this
      // accepting process is complete.
      if ( lpNewInstance->hPrevVsn == 0 )
      {
         // If the new instance has appeared on the chain without a previous
         // version, mark it and all of its descendents as temporal entities
         // for re-creating or re-including later on!
         if ( lpFirstIncluded == 0 )
            lpFirstIncluded = lpNewInstance;

         while ( lpNewInstance && lpNewInstance->hPrevVsn == 0 &&
                 lpNewInstance->nLevel > nStartLevel )
         {
           lpNewInstance->u.nInd.bTemporal = TRUE;
           lpLastIncluded = lpNewInstance;
           lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
         }

         // End out of outer do loop if we've passed all the new
         // version instances.
         if ( lpNewInstance == 0 || lpNewInstance->nLevel <= nStartLevel )
            break;
      }

      // Position on the previous version of this instance.
      lpOldInstance = zGETPTR( lpNewInstance->hPrevVsn );
      lpNewInstance->hPrevVsn = lpOldInstance->hPrevVsn;
      if ( lpNewInstance->hPrevVsn )
      {
         LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpNewInstance->hPrevVsn );

         lpPrevVsn->hNextVsn = zGETHNDL( lpNewInstance );
      }

      // If the old instance was linked, replace the old instance
      // in the link chain with the new instance.
      lpDupInstance = 0;
      bStillPrevVsn = 0;
      if ( lpOldInstance && lpOldInstance->hNextLinked )
      {
         zPVOID hOldInstance = zGETHNDL( lpOldInstance );
         zPVOID hNewInstance = zGETHNDL( lpNewInstance );

         // Find the instance in the link chain that points to lpOldInstance.
         // Also, find out if any of the instances in the link chain still
         // have a previous version.
         lpWrkInstance = zGETPTR( lpOldInstance->hNextLinked );
         for ( ; ; )
         {
            // Check to see if the linked instance is also versioned.  If it
            // is then we will flag the accepted instance as still versioned.
            if ( lpWrkInstance->hNextVsn )
            {
               LPENTITYINSTANCE lpParent;

               // OK...a linked instance to the accepted instance is still
               // versioned.  But wait!  This linked instance could be part
               // of the current version.  An example could be:
               //
               //    (A)
               //    / \
               //  (B) (B')
               //
               // Entity 'A' is the parent of the new version and B and B'
               // are linked.  lpOldInstance is the original version of B.
               // Make sure lpWrkInstance isn't a case of B'.  To so this
               // check to see if a parent of B' is the versioned parent.

               for ( lpParent = zGETPTR( lpWrkInstance->hParent );
                     lpParent;
                     lpParent = zGETPTR( lpParent->hParent ) )
               {
                  if ( lpParent == lpStartOldInstance )
                     break;
               }

               if ( lpParent == 0 )
                  bStillPrevVsn = TRUE;
            }

            // Check to see if we have found the instance that points to
            // lpOldInstance.
            if ( lpWrkInstance->hNextLinked == hOldInstance )
               break;

            lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
         }

         // Set the previous version indicator of the new instance.
         lpNewInstance->u.nInd.bPrevVersion = bStillPrevVsn;
         if ( lpNewInstance->hNextLinked )
         {
            lpDupInstance = lpNewInstance;
            while ( lpDupInstance->hNextLinked != hNewInstance &&
                    lpDupInstance->hNextLinked != hOldInstance )
            {
               lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
               lpDupInstance->u.nInd.bPrevVersion = bStillPrevVsn;
            }
         }

         // if the new instance was linked, update the linked instance
         // pointing to the new instance to point to the
         // first linked instance off the old instance. Otherwise ...
         if ( lpDupInstance )
         {
            // Add the new instance into the link chain ONLY if it is
            // not already there, Otherwise, simply remove the
            // old instance from the link chain.
            if ( lpDupInstance->hNextLinked != hNewInstance )
            {
               // lpDupInstance is pointing to the old instance, update it
               // to point to ine instance past the old instance to
               // remove the old instance from the link chain.
               lpDupInstance->hNextLinked = lpOldInstance->hNextLinked;
               while ( lpDupInstance->hNextLinked != hNewInstance )
                  lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
            }
            else
            {
               // lpDupInstance is pointing to the new instance in the new
               // chain and lpWrkInstance is pointing to the old instance
               // in the old chain, point lpDupInstance to the first instance
               // past the old instance in the old chain and lpWrkInstance to
               // the new instance to combine the 2 link chains and remove
               // the old instance from the combined chains at the same time.
               lpDupInstance->hNextLinked = lpOldInstance->hNextLinked;
               lpWrkInstance->hNextLinked = hNewInstance;
            }
         }
         else
         {
            // The new instance is not on any link chain, simply replace
            // the old instance in the old chain with the new instance.
            lpNewInstance->hNextLinked = lpOldInstance->hNextLinked;
            lpWrkInstance->hNextLinked = hNewInstance;
         }

         lpOldInstance->hNextLinked = 0;

         // If in removing old instances from the link chain we circle
         // back on the new instance itself, zero out the next linked
         // pointer.
         if ( lpNewInstance->hNextLinked == hNewInstance )
            lpNewInstance->hNextLinked = 0;
      }

      // If the old instances record has already been updated to point
      // to the new record as a result or the copy operation below on
      // a previously linked old record, OR the old instances record
      // and the new instances record were the same record because
      // the entity is not updateable, simply zero out the
      // record pointer so we don't kill it in clean up later on.
      if ( lpOldInstance->hPersistRecord == lpNewInstance->hPersistRecord )
         lpOldInstance->hPersistRecord = 0;

      // Copy the data record and incremental update information from
      // the new version to all occurrences of the old version.
      if ( lpNewInstance->hNextLinked )
      {
         if ( lpDupInstance )
            lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
         else
            lpDupInstance = zGETPTR( lpNewInstance->hNextLinked );

         // If the records and indicators have not been copied to all
         // occurrences of the old link chain, copy them now.
         if ( lpDupInstance->hPersistRecord != lpNewInstance->hPersistRecord )
         {
            lpOldRecord = zGETPTR( lpDupInstance->hPersistRecord );
            while ( lpDupInstance != lpNewInstance )
            {
               LPENTITYINSTANCE lpNextVsn;
               LPVIEWENTITY     lpNewViewEntity =
                                    zGETPTR( lpNewInstance->hViewEntity );

               lpDupInstance->hPersistRecord = lpNewInstance->hPersistRecord;
               if ( lpNewInstance->u.nInd.bUpdated )
                  lpDupInstance->u.nInd.bUpdated = TRUE;

               lpDupInstance->u.nInd.bPrevVersion = bStillPrevVsn;

               // If the duplicate instance has newer version(s)
               // with the old record, update the newer version(s) and
               // it linked instances.
               lpNextVsn = zGETPTR( lpDupInstance->hNextVsn );
               if ( lpNextVsn &&
                    zGETPTR( lpNextVsn->hPersistRecord ) == lpOldRecord )
               {
                  fnUpdateNextVsnRecordPointers( zGETPTR( lpDupInstance->hNextVsn ),
                                                 lpNewInstance,
                                                 lpOldRecord,
                                                 bStillPrevVsn );
               }

               lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
            }
         }
      }

      lpLastNewInstance = lpNewInstance;
      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );

   }  while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel );

   // Go through the EIs we are about to remove and set a flag so we can
   // determine if a cursor is pointing to one of them.
   for ( lpOldInstance = lpStartOldInstance;
         lpOldInstance;
         lpOldInstance = zGETPTR( lpOldInstance->hNextHier ) )
   {
      if ( lpOldInstance->nLevel <= lpStartOldInstance->nLevel &&
           lpOldInstance != lpStartOldInstance )
      {
         break;
      }

      lpOldInstance->u.nInd.bDelByAccept = TRUE;
   }

   // Now go through all views for the old object and update any cursors
   // positioned on the previous version to point to the new version.
   for ( lpWrkViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );
         lpWrkViewCsr;
         lpWrkViewCsr = zGETPTR( lpWrkViewCsr->hNextViewCsr ) )
   {
      lpWrkViewEntityCsr = zGETPTR( lpWrkViewCsr->hFirstOD_EntityCsr );
      lpWrkViewEntityCsr += (lpViewEntity->nHierNbr - 1);

      if ( lpWrkViewEntityCsr->hEntityInstance == 0 ||
           lpWrkViewEntityCsr->hEntityInstance == UNSET_CSR )
      {
         // Don't have to worry about a null or unset cursor.
         continue;
      }

      lpWrkInstance = zGETPTR( lpWrkViewEntityCsr->hEntityInstance );
      if ( lpWrkInstance->u.nInd.bDelByAccept )
      {
         lpWrkViewEntityCsr->hEntityInstance = lpWrkInstance->hNextVsn;
      // if ( lpWrkViewEntityCsr->hEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "AcceptSubobject", "UNSET_CSR", -1 );

         fnResetCursorForViewChildren( lpWrkViewEntityCsr );
      }
   }

   // Finally, clean up the previous version instance chain.
   lpOldInstance = lpStartOldInstance;
   while ( lpOldInstance )
   {
      fnDeleteAttributeRecord( zGETPTR( lpOldInstance->hViewEntity ),
                               lpOldInstance, TRUE );
      fnDeleteAttributeRecord( zGETPTR( lpOldInstance->hViewEntity ),
                               lpOldInstance, FALSE );

      // Null out the hRecord for all linked EI's.  This will keep us from
      // trying to free the hRecord twice.
      if ( lpOldInstance->hNextLinked )
      {
         LPENTITYINSTANCE lpLinked;
         LPENTITYINSTANCE lpPrevLinked;

         for ( lpLinked = zGETPTR( lpOldInstance->hNextLinked );
               lpLinked != lpOldInstance;
               lpLinked = zGETPTR( lpLinked->hNextLinked ) )
         {
            lpLinked->hPersistRecord = 0;
            lpPrevLinked = lpLinked;
         }

         // Now remove lpOldInstance from the link chain.
         if ( zGETPTR( lpOldInstance->hNextLinked ) == lpPrevLinked )
         {
            // Only two entities were in the link chain so now there is no
            // longer a chain.
            lpPrevLinked->hNextLinked = 0;
         }
         else
            lpPrevLinked->hNextLinked = lpOldInstance->hNextLinked;
      }

      lpWrkInstance = lpOldInstance;
      lpOldInstance = zGETPTR( lpOldInstance->hNextHier );
      fnFreeDataspace( lpWrkInstance );
   }

   // Decrement versioned instance count in viewoi.
   lpViewOI->nVersionedInstances--;
   if ( !fnInstanceVersioned( lpStartNewInstance ) )
   {
      zSHORT nUpdated = 0;
      zSHORT nUpdatedFile = 0;

      fnGetResetUpdFlags( lpStartNewInstance, &nUpdated, &nUpdatedFile, 1 );

      if ( nUpdated )
         lpViewOI->bUpdated = TRUE;

      if ( nUpdatedFile )
         lpViewOI->bUpdatedFile = TRUE;
   }

   // Now that we've cleaned up the old versioned information, we need
   // to re-create the created entity instances and re-include the included
   // instances so any appropriate spawning takes place in the new object
   // instance.
   if ( lpFirstIncluded )
   {
      LPVIEWENTITY      lpViewEntity;
      LPVIEWENTITYCSR   lpViewEntityCsr;
      LPENTITYINSTANCE  lpLastIncluded_NextHier;

      lpNewInstance = lpFirstIncluded;

      lpLastIncluded_NextHier = zGETPTR( lpLastIncluded->hNextHier );
      while ( lpNewInstance != lpLastIncluded_NextHier )
      {
         if ( lpNewInstance->u.nInd.bTemporal )
         {
            if ( lpNewInstance->u.nInd.bHidden )
            {
               lpNewInstance->u.nInd.bTemporal = 0;
               lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
            }
            else
            {
               LPVIEWCSR lpWorkViewCsr = zGETPTR( vWorkView->hViewCsr );

               // if the instance in question was included, UNDO the include
               // and re-do it so spawning will take place
               lpViewEntity = zGETPTR( lpNewInstance->hViewEntity );
               lpViewEntityCsr = zGETPTR( lpWorkViewCsr->hFirstOD_EntityCsr );
               lpViewEntityCsr += ( lpViewEntity->nHierNbr - 1 );
               fnEstablishViewForInstance( vWorkView, lpViewEntityCsr,
                                          lpNewInstance );

               if ( lpNewInstance->u.nInd.bIncluded )
               {
                  lpWrkInstance = lpNewInstance;
                  do
                  {
                     lpWrkInstance->u.nInd.bTemporal = 0;
                     lpWrkInstance = zGETPTR( lpWrkInstance->hNextHier );

                  }  while ( lpWrkInstance != lpLastIncluded_NextHier &&
                             lpWrkInstance->nLevel > lpNewInstance->nLevel );

                  // Now, spawn the include done on the new version
                  nRC = fnSpawnIncludeSubobject( vWorkView, lpViewOI,
                                                lpViewEntity, lpViewEntityCsr,
                                                1 );
                  lpNewInstance = lpWrkInstance;
               }
               else
               {
                  lpNewInstance->u.nInd.bTemporal = 0;
                  nRC = fnSpawnCreateEntity( vWorkView, lpNewInstance );
                  lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
               }

               // If something terrible happened, go through and at least
               // clean up the temporal markings in the included/created
               // entities
               if ( nRC )
               {
                  while ( lpNewInstance != lpLastIncluded_NextHier )
                  {
                     lpNewInstance->u.nInd.bTemporal = 0;
                     lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
                  }

                  break;   // Get out with bad return code
               }
            }
         }
         else
            lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
      }
   }

   // Drop the work view
   DropView( vWorkView );

#if 0   // Gig, debugging, for all new instances, see if a record
        // match can be found which is not linked!!!!
   lpNewInstance      = lpStartNewInstance;
   do
   {
      lpWrkInstance = zGETPTR( lpNewInstance->hNextHier );
      while ( lpWrkInstance && lpWrkInstance->nLevel > nStartLevel )
      {
         if ( lpWrkInstance->hPersistRecord == lpNewInstance->hPersistRecord )
         {
            lpDupInstance = zGETPTR( lpNewInstance->hNextLinked );
            while ( lpDupInstance && lpDupInstance != lpNewInstance )
            {
               if ( lpDupInstance == lpWrkInstance )
                  break;

               lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );
            }

            if ( lpDupInstance != lpWrkInstance )
            {
               SysMessageBox( "ERROR AFTER ACCEPT",
                              "Record match found in non-linked instance", 1 );
            }
         }

         lpWrkInstance = zGETPTR( lpWrkInstance->hNextHier );
      }

      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );

   } while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel );
#endif

#ifdef __RECLAIM__
   if ( lpViewOI->nHiddenCount > __RECLAIM_LIMIT__ )
      fnReclaimHiddenInstances( lpViewOI );
#endif

   fnOperationReturn( iAcceptSubobject, lpCurrentTask );
   return( nRC );  // Return results
}

LPENTITYINSTANCE LOCALOPER
fnFindLastEntityUnderParent( LPENTITYINSTANCE lpParent,
                             zBOOL            bUseTwin )
{
   LPENTITYINSTANCE lpReturnEI;
   LPENTITYINSTANCE lpEI;

   // Find the last entity under lpParent hierarchically.
   if ( bUseTwin && lpParent->hNextTwin )
   {
      // lpParent has a twin.  That makes it easy.
      lpEI = zGETPTR( lpParent->hNextTwin );
      return( zGETPTR( lpEI->hPrevHier ) );
   }

   // lpParent doesn't have a twin.  That means we have to loop through the
   // children of lpParent looking for the last entity hierarchically.
   lpReturnEI = lpParent;
   for ( lpEI = zGETPTR( lpParent->hNextHier );
         lpEI && lpEI->nLevel > lpParent->nLevel;
         lpEI = zGETPTR( lpEI->hNextHier ) )
   {
      // To try to make things faster we'll skip all twins.
      while ( lpEI->hNextTwin )
         lpEI = zGETPTR( lpEI->hNextTwin );

      lpReturnEI = lpEI;
   }

   return( lpReturnEI );
}

//./ ADD NAME=CancelSubobject
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CancelSubobject
//
//  PURPOSE:    To Drop a current version of a Subobject created with
//              CreateTemporalSubobjectVersion
//
//  RETURNS:    zCURSOR_SET - The version was cancelled and the cursor has
//                            been re-established on the previous version
//              zCURSOR_UNDEFINED - The version was cancelled and the
//                            temporal entity has been deleted!
//              zCALL_ERROR - Error dropping current version of Subobject
//
//  NOTE: A WORK IN TEST...
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
CancelSubobject( zVIEW lpView, zCPCHAR cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPVIEWOI         lpViewOI;
   LPVIEWCSR        lpViewCsr;
   LPVIEWCSR        lpWrkViewCsr;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITYCSR  lpWrkViewEntityCsr;
   LPENTITYINSTANCE lpOldInstance;
   LPENTITYINSTANCE lpStartOldInstance;
   LPENTITYINSTANCE lpStartNewInstance;
   LPENTITYINSTANCE lpLastNewInstance;
   LPENTITYINSTANCE lpLastOldInstance;
   LPENTITYINSTANCE lpNewInstance;
   LPENTITYINSTANCE lpWrkInstance;
   LPENTITYINSTANCE lpDupInstance;
   LPENTITYINSTANCE lpPrevVsn;
   zPVOID           hStartOldInstance;
   zPVOID           hStartNewInstance;
   zSHORT           nStartLevel;
   zSHORT           nHiddenCount;
   zBOOL            bStillPrevVsn;

   // Validate parameters
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntity,
                                      &lpViewEntityCsr,
                                      iCancelSubobject,
                                      lpView,
                                      cpcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Get view csr and view OI.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   // If the view is ReadOnly, return with an error if any versioned
   // instances exist, else return success.
   if ( lpView->bReadOnly )
   {
      zSHORT  nRC = 0;
      if ( lpViewOI->nVersionedInstances )
      {
         nRC = zCALL_ERROR;
         // "KZOEE119 - Invalid View, View is Read Only"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 119, 0, cpcEntityName, 0 );
      }

      fnOperationReturn( iCancelSubobject, lpCurrentTask );
      return( nRC );
   }

#ifdef __ACTIVATE_CONSTRAINTS__
   // If there is an Entity Constraint for cancel, see if it has
   // any objections about continuing.
   if ( lpViewEntity->bCancelConstraint )
   {
      zSHORT   n;

      n = fnInvokeECEOperation( lpView, lpViewEntity, lpCurrentTask,
                                zECE_CANCEL, 0 );
      // If constraint handler doesn't like something, get out.
      if ( n )
      {
         fnOperationReturn( iCancelSubobject, lpCurrentTask );
         return( n );
      }
   }
#endif

   // Get instance on which to start.
   lpStartNewInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   while ( lpStartNewInstance->hNextVsn )
      lpStartNewInstance = zGETPTR( lpStartNewInstance->hNextVsn );

   lpStartOldInstance = zGETPTR( lpStartNewInstance->hPrevVsn );

   // If the entity instance is not versioned, return a logic error.
   if ( lpStartNewInstance->u.nInd.bTemporal == FALSE &&
        (lpStartOldInstance == 0 ||
         lpStartOldInstance->u.nInd.bPrevVsnRoot == FALSE) )
   {
      fnOperationReturn( iCancelSubobject, lpCurrentTask );
      return( 0 );
   }

   // Get starting level for Cancel
   nStartLevel = lpViewEntityCsr->nLevel;

   // Check to make sure none of this instances descendents are
   // already versioned. This is true when lpPrevVsn points to a
   // versioned root instance.
   lpNewInstance   = zGETPTR( lpStartNewInstance->hNextHier );
   while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel )
   {
      lpPrevVsn = zGETPTR( lpNewInstance->hPrevVsn );

      if ( lpNewInstance->u.nInd.bTemporal ||
           (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
      {
         LPVIEWENTITY lpNewViewEntity = zGETPTR( lpNewInstance->hViewEntity );

         // "KZOEE137 - Entity Instance descendent versioned"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 137, 0,
                           lpViewEntity->szName,
                           lpNewViewEntity->szName );
         fnOperationReturn( iCancelSubobject, lpCurrentTask );
         return( zCALL_ERROR );
      }

      lpNewInstance   = zGETPTR( lpNewInstance->hNextHier );
   }

   // If we arrive here, then we must have a previous version of this
   // entity. Re-implement the previous version amongst the linked
   // instances of the previous version and clean up the current
   // version instances.

   // Get instance on which to start
   lpNewInstance = lpStartNewInstance;

   // If the starting instance is a temporal entity, mark the instance
   // as hidden as well as any children created by either CreateEntity
   // OR IncludeSubobjectFromSubobject.
   if ( lpNewInstance->u.nInd.bTemporal )
   {
      LPENTITYINSTANCE  lpLinked;

      nHiddenCount = 0;

      lpLinked = zGETPTR( lpNewInstance->hNextLinked );

      // if any linked instances for this temporal entity, mark them
      // and their children as hidden.
      if ( lpLinked )
      {
         while ( lpLinked != lpNewInstance )
         {
            zSHORT            nLevel;
            LPENTITYINSTANCE  lpLinkedChild;

            nLevel = lpLinked->nLevel;
            lpLinkedChild = lpLinked;
            do
            {
               if ( lpLinkedChild->u.nInd.bHidden == FALSE )
               {
                  lpLinkedChild->u.nIndicators = 0;
                  lpLinkedChild->u.nInd.bCreated = TRUE;
                  lpLinkedChild->u.nInd.bHidden = TRUE;
                  nHiddenCount++;
               }

               lpLinkedChild = zGETPTR( lpLinkedChild->hNextHier );

            }  while ( lpLinkedChild && lpLinkedChild->nLevel > nLevel );

            lpLinked = zGETPTR( lpLinked->hNextLinked );
         }
      }

      do
      {
         // Mark all subordinate entities for removal from memory.
         lpNewInstance->u.nIndicators = 0;
         lpNewInstance->u.nInd.bCreated = TRUE;
         lpNewInstance->u.nInd.bHidden = TRUE;
         nHiddenCount++;
         lpNewInstance = zGETPTR( lpNewInstance->hNextHier );

      }  while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel );

      lpViewOI->nHiddenCount += nHiddenCount;

      // Decrement versioned instance count in viewOI.
      lpViewOI->nVersionedInstances--;

      // We're all done here, return success.
      fnOperationReturn( iCancelSubobject, lpCurrentTask );
      return( zCURSOR_UNDEFINED );  // Assume everythings AOK
   }

   // For all old versions of the instance in question, unset the
   // previous version indicator if the instance is no longer a previous
   // version.
   lpStartOldInstance->u.nInd.bPrevVsnRoot = 0;
   do
   {
      LPENTITYINSTANCE lpNextLinked = zGETPTR( lpNewInstance->hNextLinked );

      // First, if the new instance in linked, remove the new instance
      // from the linked chain and zero out its record pointer
      lpDupInstance = 0;
      if ( lpNextLinked )
      {
         lpDupInstance = lpNextLinked;
         while ( zGETPTR( lpDupInstance->hNextLinked ) != lpNewInstance )
            lpDupInstance = zGETPTR( lpDupInstance->hNextLinked );

         if ( lpDupInstance == lpNextLinked )
            lpDupInstance->hNextLinked = 0;
         else
            lpDupInstance->hNextLinked = lpNewInstance->hNextLinked;

         lpNewInstance->hPersistRecord = 0;
         lpNewInstance->hNextLinked = 0;
      }

      // Get the previous version pointer from the new instance
      lpOldInstance = zGETPTR( lpNewInstance->hPrevVsn );

      // If the new instance had a previous version, reset the
      // previous version indicators if necessary.
      if ( lpOldInstance )
      {
         bStillPrevVsn = 0;
         if ( lpOldInstance->hNextLinked )
         {
            lpWrkInstance = zGETPTR( lpOldInstance->hNextLinked );
            while ( lpWrkInstance != lpOldInstance )
            {
               if ( lpWrkInstance->hNextVsn )
               {
                  bStillPrevVsn = TRUE;
                  break;
               }

               lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
            }
         }

         if ( lpOldInstance->hPersistRecord == lpNewInstance->hPersistRecord )
            lpNewInstance->hPersistRecord = 0;

         lpOldInstance->hNextVsn = lpNewInstance->hNextVsn;

         // Reset the prev version flags in all linked instances to
         // the original instance.
         if ( bStillPrevVsn == FALSE )
         {
            lpOldInstance->u.nInd.bPrevVersion = FALSE;
            lpWrkInstance = zGETPTR( lpOldInstance->hNextLinked );
            while ( lpWrkInstance && lpWrkInstance != lpOldInstance )
            {
               lpWrkInstance->u.nInd.bPrevVersion = FALSE;
               lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );
            }
         }
      }

      lpLastNewInstance = lpNewInstance;
      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );

   }  while ( lpNewInstance && lpNewInstance->nLevel > nStartLevel );

   // Reset the hierarchical, parent and twin pointers for the root
   // of the old version and the last instance in the new version
   hStartOldInstance = zGETHNDL( lpStartOldInstance );
   hStartNewInstance = zGETHNDL( lpStartNewInstance );
   lpStartOldInstance->hPrevHier = lpStartNewInstance->hPrevHier;
   if ( lpStartOldInstance->hPrevHier )
   {
      lpWrkInstance = zGETPTR( lpStartOldInstance->hPrevHier );
      lpWrkInstance->hNextHier = hStartOldInstance;
   }

   lpStartOldInstance->hParent = lpStartNewInstance->hParent;
// if ( lpStartOldInstance->hParent == UNSET_CSR )
//    SysMessageBox( 0, "CancelSubobject", "UNSET_CSR", -1 );

   lpStartOldInstance->hPrevTwin = lpStartNewInstance->hPrevTwin;
   if ( lpStartOldInstance->hPrevTwin )
   {
      lpWrkInstance = zGETPTR( lpStartOldInstance->hPrevTwin );
      lpWrkInstance->hNextTwin = hStartOldInstance;
   }

   lpStartOldInstance->hNextTwin = lpStartNewInstance->hNextTwin;
   if ( lpStartOldInstance->hNextTwin )
   {
      lpWrkInstance = zGETPTR( lpStartOldInstance->hNextTwin );
      lpWrkInstance->hPrevTwin = hStartOldInstance;
   }

   lpLastOldInstance = fnFindLastEntityUnderParent( lpStartOldInstance, FALSE );
   lpPrevVsn = zGETPTR( lpLastNewInstance->hPrevVsn );
   lpLastOldInstance->hNextHier = lpLastNewInstance->hNextHier;
   if ( lpLastNewInstance->hNextHier )
   {
      LPENTITYINSTANCE lpWrkInstance2;

      lpWrkInstance2 = zGETPTR( lpLastNewInstance->hNextHier );
      lpWrkInstance2->hPrevHier = zGETHNDL( lpLastOldInstance );
      lpLastNewInstance->hNextHier = 0;
   }

   // If the root of the instance was versioned, set the
   // root entity instance pointer in the ViewOI
   if ( lpViewOI->hRootEntityInstance == hStartNewInstance )
      lpViewOI->hRootEntityInstance = hStartOldInstance;

   // Now go through all views for the old object and update any cursors
   // positioned on the current version to point to the old version
   lpWrkViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );
   while ( lpWrkViewCsr )
   {
      lpWrkViewEntityCsr = zGETPTR( lpWrkViewCsr->hFirstOD_EntityCsr );
      lpWrkViewEntityCsr += ( lpViewEntity->nHierNbr - 1 );
      if ( lpWrkViewEntityCsr->hEntityInstance == hStartNewInstance )
      {
         lpWrkViewEntityCsr->hEntityInstance = hStartOldInstance;
      // if ( lpWrkViewEntityCsr->hEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "CancelSubobject", "UNSET_CSR", -1 );

         fnResetCursorForViewChildren( lpWrkViewEntityCsr );
      }

      lpWrkViewCsr = zGETPTR( lpWrkViewCsr->hNextViewCsr );
   }

   // Finally, clean up the new version instance chain
   lpNewInstance = lpStartNewInstance;
   while ( lpNewInstance )
   {
      if ( lpNewInstance->hNextLinked )
      {
         lpWrkInstance = zGETPTR( lpNewInstance->hNextLinked );
         while ( lpWrkInstance->hNextLinked != zGETHNDL( lpNewInstance ) )
            lpWrkInstance = zGETPTR( lpWrkInstance->hNextLinked );

         if ( zGETPTR( lpNewInstance->hNextLinked ) == lpWrkInstance )
            lpWrkInstance->hNextLinked = 0;
         else
            lpWrkInstance->hNextLinked = lpNewInstance->hNextLinked;
      }
      else
      {
         fnDeleteAttributeRecord( zGETPTR( lpNewInstance->hViewEntity ),
                                  lpNewInstance, TRUE );
      }

      fnDeleteAttributeRecord( zGETPTR( lpNewInstance->hViewEntity ),
                               lpNewInstance, FALSE );
      lpWrkInstance = lpNewInstance;
      lpNewInstance = zGETPTR( lpNewInstance->hNextHier );
      fnFreeDataspace( lpWrkInstance );
   }

   // Decrement versioned instance count in viewOI
   lpViewOI->nVersionedInstances--;

#ifdef __RECLAIM__
   if ( lpViewOI->nHiddenCount > __RECLAIM_LIMIT__ )
      fnReclaimHiddenInstances( lpViewOI );
#endif

   fnOperationReturn( iCancelSubobject, lpCurrentTask );
   return( zCURSOR_SET );  // Assume everythings AOK if we get here
}

//./ ADD NAME=CancelAllTemporalSubobjects
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CancelAllTemporalSubobjects
//
//  PURPOSE:    To Cancel all outstanding temporal subobjects in the
//              Object Instance
//
//  RETURNS:    0           - All Temporal Subobject versions successfully
//                            accepted
//              zCALL_ERROR - Error accepting a temporal version of
//                            Subobject
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CancelAllTemporalSubobjects( zVIEW lpView )
{
   LPTASK           lpCurrentTask;
   LPVIEWOI         lpViewOI;
   LPVIEWCSR        lpViewCsr;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpRoot;
   LPENTITYINSTANCE lpPrevVsn;
   zVIEW            vWork;
   zSHORT           nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCancelAllTemporalSubobjects,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   if ( lpViewOI->nVersionedInstances == 0 )
   {
      fnOperationReturn( iCancelAllTemporalSubobjects, lpCurrentTask );
      return( 0 );
   }

   lpRoot       = zGETPTR( lpViewOI->hRootEntityInstance );
   lpViewEntity = zGETPTR( lpRoot->hViewEntity );
   vWork = 0;
   while ( lpRoot )
   {
      nRC = fnRemoveAllChildTemporalVsns( lpView, lpRoot, 0 );
      if ( nRC < zCURSOR_UNDEFINED )
      {
         if ( vWork )
            DropView( vWork );

         fnOperationReturn( iCancelAllTemporalSubobjects, lpCurrentTask );
         return( nRC );
      }

      lpPrevVsn = zGETPTR( lpRoot->hPrevVsn );
      if ( lpRoot->u.nInd.bTemporal ||
           (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
      {
         if ( vWork == 0 )
            CreateViewFromViewForTask( &vWork, lpView, 0 );

         lpViewEntityCsr = fnEstablishViewForInstance( vWork, 0, lpRoot );
         nRC = CancelSubobject( vWork, lpViewEntity->szName );
         if ( nRC < zCURSOR_UNDEFINED )
         {
            if ( vWork )
               DropView( vWork );

            fnOperationReturn( iCancelAllTemporalSubobjects, lpCurrentTask );
            return( nRC );
         }

         lpRoot = zGETPTR( lpViewEntityCsr->hEntityInstance );
      }

      if ( lpRoot )
         lpRoot = zGETPTR( lpRoot->hNextTwin );
   }

   if ( vWork )
      DropView( vWork );

   fnOperationReturn( iCancelAllTemporalSubobjects, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=IncludeSubobjectFromSubobject
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      IncludeSubobjectFromSubobject
//
//  PURPOSE:    To include a subobject in another subobject and
//              share the attribute record.  This operation creates a
//              new relationship to the target subobject position under
//              the source entity's parent.  The persistent attribute
//              information from the source entity is shared with the
//              target entity, so any updates to the attributes from either
//              the source or target view are immediately reflected in
//              both views.
//
//  RULES FOR VALID
//  SUBOBJECT INCLUSION:  1. The source and target entity MUST represent
//                           the same E/R Entity by token number.
//                        2. The source and target entitys must both have a
//                           complete set of persistent attributes so the
//                           data record can be shared.
//                        3. The source and target positions cannot
//                           have the same parent entity instance (i.e. you
//                           cannot establish the same parent/child
//                           relationship twice).
//                        4. The source structure must contain all the
//                           subordinate entities (with the correct
//                           relationship) defined in the target
//                           Subobject.  Note however, one of the
//                           subordinates may in fact be a parent entity in
//                           the source subobject as long as the
//                           cardinality in the target subobject is defined
//                           as 0,1 or 1,1.
//                        5. Neither the source nor the target position may
//                           be a previous version NOR have a previous
//                           version up their respective parent chains.
//
//          NOTE: Both Intra-Object and Inter-Object linking are allowed.
//
//  RETURNS:    0           - Target subobject inclusion successful
//              zCALL_ERROR - Error including subobject instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
IncludeSubobjectFromSubobject( zVIEW  lpTgtView,
                               zCPCHAR cpcTgtEntityName,
                               zVIEW  lpSrcView,
                               zCPCHAR cpcSrcEntityName,
                               zSHORT nPosition )
{
   LPTASK            lpCurrentTask;
   LPTASK            lpSrcOwningTask;
   LPTASK            lpTgtOwningTask;
   LPVIEWOD          lpSrcViewOD;
   LPVIEWOD          lpTgtViewOD;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWOI          lpSrcViewOI;
   LPVIEWOI          lpTgtViewOI;
   LPVIEWCSR         lpViewCsr;
   zCHAR             szEntityString[ 255 ];
   zSHORT            bNewRoot;
   zSHORT            nPositionOK;
   zSHORT            nRC;

   // Validate that the source view entity exists and has a cursor
   // value for copying, we call fnValidateInstanceParameters for the
   // source since it requires the same editing as an attribute retrieval.
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpSrcViewEntity,
                                      &lpSrcViewEntityCsr,
                                      iIncludeSubobjectFromSubobject,
                                      lpSrcView,
                                      cpcSrcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Validate that the view passed is valid.
   if ( fnValidViewCsr( lpCurrentTask, lpTgtView ) == 0 )
   {
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( (lpTgtViewEntity = fnValidViewEntity( &lpTgtViewEntityCsr,
                                              lpTgtView,
                                              cpcTgtEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If intent is update and the target view is read-only, fail the call.
   if ( lpTgtView->bReadOnly )
   {
      // We'll only fail the call if the entity isn't down a derived path.
      if ( lpTgtViewEntity->bDerivedPath == FALSE )
      {
         // "KZOEE119 - Invalid View, View is Read Only"
         fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 119, 0, cpcTgtEntityName, 0 );
         fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   // Make sure entity can be included.
   if ( lpSrcViewEntity->bInclSrc == FALSE )
   {
      strcpy_s( szEntityString, zsizeof( szEntityString ), "\nEntity: \t" );
      strcat_s( szEntityString, zsizeof( szEntityString ), lpSrcViewEntity->szName );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "Source Entity not includeble",
                        szEntityString );
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure entity can be included.
   if ( lpTgtViewEntity->hParent && lpTgtViewEntity->bInclude == FALSE )
   {
      strcpy_s( szEntityString, zsizeof( szEntityString ), "\nEntity: \t" );
      strcat_s( szEntityString, zsizeof( szEntityString ), lpTgtViewEntity->szName );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "Entity inclusion not allowed",
                        szEntityString );
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure that the LODs are in sync.  They are out of sync if the LODs
   // were built with different dates for the ER.  We won't bother performing
   // the check if either LOD doesn't have the ER date.  This allows old LODs
   // to work.
   lpSrcViewOD = zGETPTR( lpSrcView->hViewOD );
   lpTgtViewOD = zGETPTR( lpTgtView->hViewOD );
   if ( lpSrcViewOD->szER_Date[ 0 ] && lpTgtViewOD->szER_Date[ 0 ] &&
        zstrcmp( lpSrcViewOD->szER_Date, lpTgtViewOD->szER_Date ) != 0 )
   {
      zCHAR szMsg[ 200 ];

      TraceLineS( "ERROR: ", "The following LODs are out of sync and should be re-built" );

      sprintf_s( szMsg, zsizeof( szMsg ), "Out-of-sync error. Target LOD = %s, date = %s",
                 lpTgtViewOD->szName, lpTgtViewOD->szER_Date );
      TraceLineS( "ERROR: ", szMsg );

      sprintf_s( szMsg, zsizeof( szMsg ), "Out-of-sync error. Source LOD = %s, date = %s",
                 lpSrcViewOD->szName, lpSrcViewOD->szER_Date );
      TraceLineS( "ERROR: ", szMsg );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "LODs are potentially out of sync.  See trace for more.",
                        lpTgtViewOD->szName );

      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // DGC 97/03/19 Added following checks.

   // If the target entity has physical information then the source view had
   // better have physical information.
   if ( lpTgtViewEntity->hFirstDataRecord &&
        lpSrcViewEntity->hFirstDataRecord == 0 )
   {
      zCHAR    szMsg[ 200 ];
      LPVIEWOD lpViewOD;

      TraceLineS( "Rules violation: physical mapping mismatch", "" );

      lpViewOD = zGETPTR( lpTgtView->hViewOD );
      sprintf_s( szMsg, zsizeof( szMsg ), "%s.%s", lpViewOD->szName, lpTgtViewEntity->szName );
      TraceLineS( "   Target entity = ", szMsg );

      lpViewOD = zGETPTR( lpSrcView->hViewOD );
      sprintf_s( szMsg, zsizeof( szMsg ), "%s.%s", lpViewOD->szName, lpSrcViewEntity->szName );
      TraceLineS( "   Source entity = ", szMsg );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "Target entity has physical mapping but source "
                        "entity does not.  Rebuild source LOD with TE.\n"
                        "See trace for more.", 0 );
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Get source and target tasks ... steal the owning variables for a second
   // so we can do the test immediately following.
   lpSrcOwningTask = zGETPTR( lpSrcView->hTask );
   lpTgtOwningTask = zGETPTR( lpTgtView->hTask );

   // If the task of the source view is not equal to the task of the target
   // view, we will have a problem if the source task is destroyed before the
   // target task.  So we will error out of here unless perhaps the source
   // task is the application or main (anchor) task.
#if 0
   if ( lpTgtView->hTask != lpSrcView->hTask )
      SysMessageBox( 0, "OE Error",
                     "IncludeSubobjectFromSubobject source and target views task conflict",
                     -1 );
#else
   if ( lpTgtView->hTask != lpSrcView->hTask &&
        lpSrcView->bApplicationView == FALSE &&
        lpSrcOwningTask->bApplicationTask == FALSE &&
        lpTgtOwningTask->bApplicationTask == FALSE &&
        lpSrcView->hTask != AnchorBlock->hMainTask )
   {
      static zSHORT k = 0;
      zCHAR szDebug[ 2 ];

      if ( k == 0 )
         SysReadZeidonIni( -1, "[Debug]", "BombZDr", szDebug, zsizeof( szDebug ) );

      if ( k == 0 && szDebug[ 0 ] == 'Y' )
      {
         lpTgtOwningTask = zGETPTR( lpTgtView->hTask );
         if ( lpTgtOwningTask->bApplicationTask )
         {
            TraceLine( "IncludeSubobjectFromSubobject Include source task (0x%08x) "
                         "does not match target task (0x%08x), but target task is the "
                         "application task - Source Entity: %s  Target Entity: %s",
                       lpSrcView->hTask, lpTgtView->hTask,
                       cpcSrcEntityName, cpcTgtEntityName );
         }
         else
         {
            k /= k;
            k++;

            // "KZOEE451 - Include source task does not match target task"
            fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 451, 0, cpcTgtEntityName, 0 );
         // fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );

         // return( zCALL_ERROR );
         }
      }
      else
      {
         lpTgtOwningTask = zGETPTR( lpTgtView->hTask );
         if ( lpTgtOwningTask->bApplicationTask )
         {
            TraceLine( "IncludeSubobjectFromSubobject Include source task (0x%08x) does not match target task (0x%08x),"
                         " but target task is the application task"
                         "Source Entity: %s   Target Entity: %s",
                       lpSrcView->hTask, lpTgtView->hTask,
                       cpcSrcEntityName, cpcTgtEntityName );
         }
         else
         {
            TraceLine( "IncludeSubobjectFromSubobject Include source task (0x%08x) does not match target task (0x%08x) "
                         "Source Entity: %s   Target Entity: %s",
                       lpSrcView->hTask, lpTgtView->hTask,
                       cpcSrcEntityName, cpcTgtEntityName );
         }
      }
   }
#endif

   // If the source entity has physical mapping but the target entity does
   // not, then it's possible that the attributes in each entity don't match.
   // Make sure that each attribute in the target has a matching attribute in
   // the source.
   if ( lpTgtViewEntity->hFirstDataRecord == 0 &&
        lpSrcViewEntity->hFirstDataRecord )
   {
      LPVIEWATTRIB lpTgtAttrib;
      LPVIEWATTRIB lpSrcAttrib;

      // Loop through each of the target attributes.
      for ( lpTgtAttrib = zGETPTR( lpTgtViewEntity->hFirstOD_Attrib );
            lpTgtAttrib;
            lpTgtAttrib = zGETPTR( lpTgtAttrib->hNextOD_Attrib ) )
      {
         // We only care about persistent attributes.
         if ( lpTgtAttrib->bPersist == FALSE )
            continue;

         // Look for a matching source attribute.
         for ( lpSrcAttrib = zGETPTR( lpSrcViewEntity->hFirstOD_Attrib );
               lpSrcAttrib && lpSrcAttrib->lERAttTok != lpTgtAttrib->lERAttTok;
               lpSrcAttrib = zGETPTR( lpSrcAttrib->hNextOD_Attrib ) )
         {
            // Nothing needs to be done here.
         }

         // If lpSrcAttrib is 0 then we didn't find a matching source attrib.
         // If the offsets don't match then we also have an error.
         if ( lpSrcAttrib == 0 ||
              lpSrcAttrib->ulRecordOffset != lpTgtAttrib->ulRecordOffset )
         {
            zCHAR szMsg[ 250 ];
            LPVIEWOD lpViewOD;

            if ( lpSrcAttrib )
            {
               lpViewOD = (LPVIEWOD) zGETPTR( lpSrcView->hViewOD );

               sprintf_s( szMsg, zsizeof( szMsg ), "%s.%s.%s Offset = %d",
                          lpViewOD->szName,
                          lpSrcViewEntity->szName,
                          lpSrcAttrib->szName,
                          lpSrcAttrib->ulRecordOffset );
               TraceLineS( "(so) IncludeSubobject Src ", szMsg );
            }
            else
               TraceLineS( "(so) No matching Src Attrib", "" );

            lpViewOD = (LPVIEWOD) zGETPTR( lpTgtView->hViewOD );
            sprintf_s( szMsg, zsizeof( szMsg ), "%s.%s.%s Offset = %d",
                       lpViewOD->szName,
                       lpTgtViewEntity->szName,
                       lpTgtAttrib->szName,
                       lpTgtAttrib->ulRecordOffset );
            TraceLineS( "(so) IncludeSubobject Tgt ", szMsg );

            strcpy_s( szEntityString, zsizeof( szEntityString ), "\nEntity: \t" );
            strcat_s( szEntityString, zsizeof( szEntityString ), lpTgtViewEntity->szName );

            // "KZOEE106 - Rules violation"
            fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                              "Source entity was built with TE but the target "
                              "entity was not and attributes do not match. "
                              "Rebuild target LOD with TE.\n"
                              "Target entity name", szEntityString );
            fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
            return( zCALL_ERROR );
         }

      } // For each lpTgtAttrib...

   } // if ( lpTgtViewEntity->hFirstDataRecord == 0 &&
     //      lpSrcViewEntity->hFirstDataRecord )...

   // DGC 97/03/19 End.

   lpViewCsr   = zGETPTR( lpSrcView->hViewCsr );
   lpSrcViewOI = zGETPTR( lpViewCsr->hViewOI );
   lpViewCsr   = zGETPTR( lpTgtView->hViewCsr );
   lpTgtViewOI = zGETPTR( lpViewCsr->hViewOI );

   // Versioning rule, the source instance can be versioned, but the
   // include will actually take place from the OLDEST version of the
   // source. If the source instances OLDEST version is under a version
   // root with a previous version, then we must fail the include
   // request since the cancel of the version root will cause the
   // included instance to totally disappear.
   if ( lpSrcViewOI->nVersionedInstances )
   {
      LPENTITYINSTANCE lpSrcInstance;
      LPENTITYINSTANCE lpTgtParent;

      lpSrcInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );

      // If the source of the include is a temporal entity, issue an
      // error because a cancel could UNDO the include and this
      // would be confusing to the application developer!
      if ( lpSrcInstance->u.nInd.bTemporal &&
           lpSrcViewOI != lpTgtViewOI )
      {
         //  "KZOEE141 - Attempt to include a subobject whose root is a "
         //  "           Temporal Entity"
         fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 141, 0,
                           lpSrcViewEntity->szName, 0 );
         fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
         return( zCALL_ERROR );
      }

      while ( lpSrcInstance->hPrevVsn )
         lpSrcInstance = zGETPTR( lpSrcInstance->hPrevVsn );

      while ( lpSrcInstance )
      {
         if ( lpSrcInstance->hPrevVsn )
            break;

         lpSrcInstance = zGETPTR( lpSrcInstance->hParent );
      }

      // The source instance is versioned, now it is pointing to
      // the root of sources previous version, see if the parent
      // target entity instance is part of the same version, if so
      // then the include can happen, otherwise the include must fail
      // because it may disappear as the result of a cancel call.
      if ( lpSrcInstance && lpTgtViewEntityCsr->hParent )
      {
         LPENTITYINSTANCE lpPrevVsn;
         LPVIEWENTITYCSR  lpParentCsr;

         lpSrcInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
         while ( lpSrcInstance )
         {
            lpPrevVsn = zGETPTR( lpSrcInstance->hPrevVsn );
            if ( (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) ||
                 lpSrcInstance->u.nInd.bTemporal )
            {
               break;
            }

            lpSrcInstance = zGETPTR( lpSrcInstance->hParent );
         }

         lpParentCsr = zGETPTR( lpTgtViewEntityCsr->hParent );
         if ( lpParentCsr->hEntityInstance == UNSET_CSR )
            fnEstablishCursorForView( lpParentCsr );

         lpTgtParent = zGETPTR( lpParentCsr->hEntityInstance );
         while ( lpTgtParent )
         {
            lpPrevVsn = zGETPTR( lpTgtParent->hPrevVsn );
            if ( (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) ||
                 lpTgtParent->u.nInd.bTemporal )
            {
               break;
            }

            lpTgtParent = zGETPTR( lpTgtParent->hParent );
         }

         if ( lpTgtParent == 0 || lpTgtParent != lpSrcInstance )
            lpSrcInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
         else
            lpSrcInstance = 0;
      }

      if ( lpSrcInstance )
      {
         LPVIEWENTITY lpTemp = zGETPTR( lpSrcInstance->hViewEntity );

         //  "KZOEE116 - Attempt to include an instance created under
         //  "           a versioned parent"
         fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 116, 0,
                           lpTemp->szName,
                           lpSrcViewEntity->szName );
         fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   // Validate that an insert of this entity in this instance at
   // the specified position is valid.
   nPositionOK = fnValidateInsertPosition( &bNewRoot,
                                           iIncludeSubobjectFromSubobject,
                                           lpTgtView,
                                           lpTgtViewEntityCsr,
                                           nPosition );
   if ( nPositionOK < 0 )
   {
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Get source and target owning tasks
   lpSrcOwningTask = zGETPTR( lpSrcViewOI->hTask );
   lpTgtOwningTask = zGETPTR( lpTgtViewOI->hTask );

#if 0
   // Validate that the Source and Target ViewOI belong to the
   // Same task!!!
   if ( lpSrcOwningTask != lpTgtOwningTask )
   {
      // If we have a task mismatch, make sure the source task is the
      // system task AND that all the target entities of the include
      // are READ ONLY.
      if ( lpSrcOwningTask != zGETPTR( AnchorBlock->hMainTask ) )
      {
         // "KZOEE131 - Attempt to include Subobject from another "
         // "           Application Task"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 131, 0, 0, 0 );
         fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
         return( zCALL_ERROR );
      }

      lpTgtViewOI->hAllocTask = zGETHNDL( lpSrcOwningTask );
   }
#endif

#ifdef __ACTIVATE_CONSTRAINTS__
   // If there is an Entity Constraint for include, see if it has
   // any objections about continuing.
   if ( lpTgtViewEntity->bIncludeConstraint )
   {
      zVIEW             lpWorkView;
      LPVIEWENTITY      lpWorkViewEntity;
      LPVIEWENTITYCSR   lpWorkViewEntityCsr;
      zSHORT            n;

      // Create a work view to save all the cursors just in case the
      // constraint handler says it's a no go situation.
      CreateViewFromViewForTask( &lpWorkView, lpTgtView, 0 );
      lpWorkViewEntity = fnValidViewEntity( &lpWorkViewEntityCsr,
                                            lpWorkView, cpcTgtEntityName, 0 );

      // Call fnIncludeSubobjectFromSubobject to do the rest.
      nRC = fnIncludeSubobjectFromSubobject( lpTgtView, lpTgtViewEntity,
                                             lpSrcView, lpSrcViewEntity,
                                             nPosition, bNewRoot, 1 );

      if ( nRC == 0 )
      {
         n = fnInvokeECEOperation( lpTgtView, lpTgtViewEntity, lpCurrentTask,
                                   zECE_INCLUDE, 0 );
         // If constraint handler doesn't like something, get out.
         if ( n )
         {
            // We must back out the include ... but how????
            fnExcludeEntity( lpTgtView, lpTgtViewEntity, lpTgtViewEntityCsr,
                             zREPOS_NONE, 1 );

            // and reset the cursors before exiting...
            fnSetViewFromView( lpTgtView, lpWorkView );
            fnDropView( lpWorkView );
            fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
            return( n );
         }
         else
         {
            // Everything looks okey dokey...
            fnDropView( lpWorkView );
         }
      }
      else
      {
         // Clean up work view if error from fnIncludeSubobjectFromSubobject.
         fnDropView( lpWorkView );
      }
   }
   else
#endif

   // Call fnIncludeSubobjectFromSubobject to do the rest.
   nRC = fnIncludeSubobjectFromSubobject( lpTgtView, lpTgtViewEntity,
                                          lpSrcView, lpSrcViewEntity,
                                          nPosition, bNewRoot, 1 );

   // Simple enough, let's return to caller.
   fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
   return( nRC );
}

zSHORT OPERATION
IncludeSubobjectFromSubobjectEx( zVIEW   lpTgtView,
                                 zCPCHAR cpcTgtEntityName,
                                 zVIEW   lpSrcView,
                                 zCPCHAR cpcSrcEntityName,
                                 zSHORT  nPosition,
                                 zLONG   lControl )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   zSHORT            nRC;

   // Validate that the source view entity exists and has a cursor
   // value for copying, we call fnValidateInstanceParameters for the
   // source since it requires the same editing as an attribute retrieval.
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpSrcViewEntity,
                                      &lpSrcViewEntityCsr,
                                      iIncludeSubobjectFromSubobject,
                                      lpSrcView,
                                      cpcSrcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Validate that the view passed is valid.
   if ( fnValidViewCsr( lpCurrentTask, lpTgtView ) == 0 )
   {
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( (lpTgtViewEntity = fnValidViewEntity( &lpTgtViewEntityCsr,
                                              lpTgtView,
                                              cpcTgtEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = IncludeSubobjectFromSubobject( lpTgtView, cpcTgtEntityName,
                                        lpSrcView, cpcSrcEntityName,
                                        nPosition );

   if ( nRC == zCALL_ERROR )
      goto EndOfFunction;

   if ( lControl | zINCLUDE_WITHCOPY )
   {
      LPENTITYINSTANCE lpSrcEI = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
      LPENTITYINSTANCE lpTgtEI = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
      LPVIEWATTRIB     lpViewAttrib;

      // Copy non-persistent attributes.
      for ( lpViewAttrib = zGETPTR( lpSrcViewEntity->hFirstOD_Attrib );
            lpViewAttrib;
            lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         LPVIEWATTRIB lpTgtViewAttrib;

         if ( lpViewAttrib->bPersist )
            continue;

         // Look for an attribute with the same name.
         for ( lpTgtViewAttrib = zGETPTR( lpTgtViewEntity->hFirstOD_Attrib );
               lpTgtViewAttrib;
               lpTgtViewAttrib = zGETPTR( lpTgtViewAttrib->hNextOD_Attrib ) )
         {
            if ( zstrcmp( lpTgtViewAttrib->szName,
                          lpViewAttrib->szName ) == 0 )
            {
               // Found a matching attribute by name so break.
               break;
            }
         }

         // If lpTgtViewAttrib is 0 then we didn't find a matching attr by
         // name so try again with the next attrib.
         if ( lpTgtViewAttrib == 0 )
            continue;

         if ( SetAttributeFromAttribute( lpTgtView,
                                         lpTgtViewEntity->szName,
                                         lpTgtViewAttrib->szName,
                                         lpSrcView,
                                         lpSrcViewEntity->szName,
                                         lpViewAttrib->szName ) == zCALL_ERROR )
         {
            nRC = zCALL_ERROR;
            goto EndOfFunction;
         }

      }

   } // if ( lControl | zINCLUDE_WITHCOPY )...

EndOfFunction:
   // Simple enough, let's return to caller.
   fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
   return( nRC );
}

zSHORT LOCALOPER
fnMoveSubobjectUnderNewParent( LPENTITYINSTANCE lpWkEntityInstance,
                               zVIEW            lpTgtView,
                               LPVIEWENTITYCSR  lpTgtViewEntityCsr,
                               zVIEW            lpSrcView,
                               LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                               zSHORT           nPosition,
                               zSHORT           nSrcPosition,
                               LPTASK           lpCurrentTask )
{
   LPVIEWOI          lpSrcViewOI;
   LPVIEWOI          lpTgtViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWCSR         lpTgtViewCsr = zGETPTR( lpTgtView->hViewCsr );
   LPVIEWENTITYCSR   lpWkTgtViewEntityCsr;
   LPENTITYINSTANCE  lpNewEntityInstance = 0;
   LPENTITYINSTANCE  lpSrcEntityInstance = 0;
   LPVIEWENTITY      lpSrcViewEntity = zGETPTR( lpSrcViewEntityCsr->hViewEntity );
   LPVIEWENTITY      lpTgtViewEntity = zGETPTR( lpTgtViewEntityCsr->hViewEntity );
   zSHORT            bNewRoot;
   zSHORT            bSameInstance;
   zSHORT            nStartLevel;
   zSHORT            nLevelDifference;
   zSHORT            nHiddenCount;
   zSHORT            nPositionOK;
   zSHORT            nResetCount;
   zSHORT            nRC;

   // Validate that an insert of this entity in this instance at
   // the specified position is valid.
   nPositionOK = fnValidateInsertPosition( &bNewRoot,
                                           iMoveSubobject,
                                           lpTgtView,
                                           lpTgtViewEntityCsr,
                                           nPosition );
   if ( nPositionOK < 0 )
   {
   // fnOperationReturn( iMoveSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Since we are not moving within the same parent, do rules checking
   // now to ensure that the move is OK.

   // Make sure entity can be included.
   if ( lpSrcViewEntity->bExclude == FALSE )
   {
      zCHAR szEntityString[ 100 ];

      strcpy_s( szEntityString, zsizeof( szEntityString ), "\nEntity: \t" );
      strcat_s( szEntityString, zsizeof( szEntityString ), lpSrcViewEntity->szName );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "Source Entity not excludable",
                        szEntityString );
   // fnOperationReturn( iMoveSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure entity can be included.
   if ( lpTgtViewEntity->bCreate == FALSE )
   {
      zCHAR szEntityString[ 100 ];

      strcpy_s( szEntityString, zsizeof( szEntityString ), "\nEntity: \t" );
      strcat_s( szEntityString, zsizeof( szEntityString ), lpTgtViewEntity->szName );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "Entity creation not allowed",
                        szEntityString );
   // fnOperationReturn( iMoveSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpSrcEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );

   nHiddenCount = 0;
   while ( lpWkEntityInstance )
   {
      if ( lpWkEntityInstance == lpSrcEntityInstance )
      {
         // "KZOEE115 - Attempt to move an instance under one of its children"
         fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 115, 0,
                           lpSrcViewEntity->szName,
                           lpTgtViewEntity->szName );
      // fnOperationReturn( iMoveSubobject, lpCurrentTask );
         return( zCALL_ERROR );
      }

      lpWkEntityInstance = zGETPTR( lpWkEntityInstance->hParent );
   }

   lpViewCsr   = zGETPTR( lpSrcView->hViewCsr );
   lpSrcViewOI = zGETPTR( lpViewCsr->hViewOI );
   lpViewCsr   = zGETPTR( lpTgtView->hViewCsr );
   lpTgtViewOI = zGETPTR( lpViewCsr->hViewOI );

   bSameInstance = (lpSrcViewOI == lpTgtViewOI);

   nStartLevel = lpSrcViewEntityCsr->nLevel;
   nLevelDifference = lpTgtViewEntityCsr->nLevel - nStartLevel;
   nRC = fnCreateEntity( lpTgtView, lpTgtViewEntity, lpTgtViewEntityCsr,
                         nPosition, bNewRoot );
   if ( nRC )
   {
   // fnOperationReturn( iMoveSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpWkTgtViewEntityCsr = lpTgtViewEntityCsr;
   lpNewEntityInstance = zGETPTR( lpWkTgtViewEntityCsr->hEntityInstance );
   lpNewEntityInstance->u.nInd = lpSrcEntityInstance->u.nInd;
   if ( lpSrcEntityInstance->hParent )
      lpSrcEntityInstance->u.nInd.bExcluded = TRUE;

   if ( lpNewEntityInstance->hParent )
      lpNewEntityInstance->u.nInd.bIncluded = TRUE;

   // Set target instance as updated.
   lpTgtViewOI->bUpdatedFile = lpTgtViewOI->bUpdated = TRUE;

   // If source instance is different from target instance then flag source
   // instance as updated.
   if ( bSameInstance == FALSE )
      lpSrcViewOI->bUpdatedFile = lpSrcViewOI->bUpdated = TRUE;

   if ( lpTgtViewEntity->bRecursive )
   {
      SetViewToSubobject( lpTgtView, lpTgtViewEntity->szName );
      nResetCount = 1;
   }
   else
      nResetCount = 0;

   for ( ; ; )
   {
      if ( nRC == 0 )
      {
         // link the new instance to the old instance
         if ( lpSrcEntityInstance->hNextLinked )
         {
            lpWkEntityInstance = zGETPTR( lpSrcEntityInstance->hNextLinked );
            while ( zGETPTR( lpWkEntityInstance->hNextLinked ) !=
                                                        lpSrcEntityInstance )
            {
               lpWkEntityInstance = zGETPTR( lpWkEntityInstance->hNextLinked );
            }

            lpWkEntityInstance->hNextLinked = zGETHNDL( lpNewEntityInstance );
         }
         else
            lpSrcEntityInstance->hNextLinked = zGETHNDL( lpNewEntityInstance );

         lpNewEntityInstance->hNextLinked = zGETHNDL( lpSrcEntityInstance );

         // Set Record and Rel Record.
         lpNewEntityInstance->hPersistRecord = lpSrcEntityInstance->hPersistRecord;

         // Transfer Rel record UNLESS this is the first time and the
         // source entity and target entities are not the same.
         if ( lpSrcViewEntity == lpTgtViewEntity ||
              lpSrcEntityInstance->nLevel > nStartLevel )
         {
            lpNewEntityInstance->hNonPersistRecord =
                                 lpSrcEntityInstance->hNonPersistRecord;
            lpSrcEntityInstance->hNonPersistRecord = 0;
         }
      }

      // Mark Source entity instance is hidden.
      lpSrcEntityInstance->u.nInd.bHidden = TRUE;
      nHiddenCount++;

      do
      {
         lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );

      } while ( lpSrcEntityInstance && lpSrcEntityInstance->u.nInd.bHidden );

      if ( lpSrcEntityInstance == 0 ||
           lpSrcEntityInstance->nLevel <= nStartLevel )
      {
         break;
      }

      if ( nRC == 0 )
      {
         // We have a new entity instance, find the appropriate target
         // cursor for creating the new entity and do any subobject
         // processing to get the appropriate level.
         for ( ; ; )
         {
            lpWkTgtViewEntityCsr =
                           zGETPTR( lpTgtViewCsr->hRootViewEntityCsr );
            while ( lpWkTgtViewEntityCsr &&
                    lpWkTgtViewEntityCsr->hViewEntity !=
                                      lpSrcEntityInstance->hViewEntity )
            {
               lpWkTgtViewEntityCsr =
                           zGETPTR( lpWkTgtViewEntityCsr->hNextHier );
            }

            // If a target cursor not found, the entity is out of view
            // and 1 or more resets are necessary.
            if ( lpWkTgtViewEntityCsr == 0 ||
                 lpWkTgtViewEntityCsr->nLevel >
                        lpSrcEntityInstance->nLevel + nLevelDifference )
            {
               fnResetViewFromSubobject( lpTgtView );
               nResetCount--;
               continue;
            }

            if ( lpWkTgtViewEntityCsr->nLevel <
                     lpSrcEntityInstance->nLevel + nLevelDifference )
            {
               LPVIEWENTITY lpSrchParentViewEntity;
               LPVIEWENTITY lpSrchChildViewEntity;
               LPVIEWENTITY lpWkTgtViewEntity =
                           zGETPTR( lpWkTgtViewEntityCsr->hViewEntity );

               if ( lpWkTgtViewEntity->bRecursive )
               {
                  SetViewToSubobject( lpTgtView, lpWkTgtViewEntity->szName );
                  nResetCount++;
                  break;
               }

               // If the entity is at a lower level and the entity does not
               // have recursive behavior, then another child of one of the
               // entity's parents must have recursive behavior, FIND IT and
               // set view to subobject for that entity type.
               lpSrchParentViewEntity = zGETPTR( lpWkTgtViewEntity->hParent );
               while ( lpSrchParentViewEntity )
               {
                  lpSrchChildViewEntity = zGETPTR( lpSrchParentViewEntity->hNextHier );
                  while ( lpSrchChildViewEntity &&
                          lpSrchChildViewEntity->nLevel >
                                       lpSrchParentViewEntity->nLevel &&
                          (lpSrchChildViewEntity->bRecursive == FALSE ||
                           lpSrchChildViewEntity->lEREntTok !=
                                       lpSrchParentViewEntity->lEREntTok ) )
                  {
                     lpSrchChildViewEntity = zGETPTR( lpSrchChildViewEntity->hNextHier );
                  }

                  if ( lpSrchChildViewEntity &&
                       lpSrchChildViewEntity->nLevel > lpSrchParentViewEntity->nLevel )
                  {
                     break;
                  }

                  lpSrchParentViewEntity = zGETPTR( lpSrchParentViewEntity->hParent );
               }

               if ( lpSrchParentViewEntity )
               {
                  SetViewToSubobject( lpTgtView, lpSrchChildViewEntity->szName );
                  nResetCount++;
               }
            }

            break;
         }

         // We have a new target entity cursor, create an entity in
         // the target.
         nRC = fnCreateEntity( lpTgtView,
                               zGETPTR( lpSrcEntityInstance->hViewEntity ),
                               lpWkTgtViewEntityCsr, zPOS_AFTER, 0 );
         if ( nRC == 0 )
         {
            lpNewEntityInstance = zGETPTR( lpWkTgtViewEntityCsr->hEntityInstance );
            lpNewEntityInstance->u.nInd = lpSrcEntityInstance->u.nInd;
         }
      }  // END if ( RC == 0 ) for subsequent create entity call
   }     // END for ( ; ; ) looping through child entities

   // We've created all the entities from the source, now do any subobject
   // resets necessary and reset the target entity's subordinate cursors.
   if ( nRC == 0 )
   {
      while ( nResetCount > 0 )
      {
         fnResetViewFromSubobject( lpTgtView );
         nResetCount--;
      }

      fnResetCursorForViewChildren( lpTgtViewEntityCsr );
   }

   // Update the hidden count in the View OI and call fnReclaimHiddenInstance
   // if it is greater than the preset limit.
   lpSrcViewOI->nHiddenCount += nHiddenCount;

   // Now see if the user requested the cursor to be repositioned for the
   // src entity.
   switch ( nSrcPosition )
   {
      case zREPOS_FIRST:
         nRC = SetCursorFirstEntity( lpSrcView, lpSrcViewEntity->szName, 0 );
         break;

      case zREPOS_LAST:
         nRC = SetCursorLastEntity( lpSrcView, lpSrcViewEntity->szName, 0 );
         break;

      case zREPOS_NEXT:
         nRC = SetCursorNextEntity( lpSrcView, lpSrcViewEntity->szName, 0 );
         break;

      case zREPOS_PREV:
         nRC = SetCursorPrevEntity( lpSrcView, lpSrcViewEntity->szName, 0 );
         break;

      default:
         nRC = zCURSOR_UNDEFINED;
   }

#ifdef __RECLAIM__
   if ( nRC != zCURSOR_UNDEFINED && lpSrcViewOI->nHiddenCount > __RECLAIM_LIMIT__ )
      fnReclaimHiddenInstances( lpSrcViewOI );
#endif

// fnOperationReturn( iMoveSubobject, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnMoveSubobjectAmongTwins
/*
   We're moving lpSrcEI next to lpTargetEI depending on nPosition which can
   be zPOS_AFTER, zPOS_BEFORE.
*/
zSHORT LOCALOPER
fnMoveSubobjectAmongTwins( zVIEW            lpView,
                           LPENTITYINSTANCE lpTgtEI,
                           LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                           LPENTITYINSTANCE lpSrcEI,
                           zSHORT           nPosition,
                           zSHORT           nSrcPosition )
{
   zSHORT               nRC;
   LPENTITYINSTANCE     lpEI;
   LPENTITYINSTANCE     lpTgtLastHier;
   LPENTITYINSTANCE     lpSrcLastHier;
   LPVIEWENTITY         lpViewEntity;
   LPVIEWOI             lpViewOI = zGETPTR( lpTgtEI->hViewOI );

   switch ( nPosition )
   {
      case zPOS_FIRST:
         // If we're moving the src entity to the beginning this is the same
         // as moving src before the first entity so change lpTgtEI to be
         // the first twin.
         while ( lpTgtEI->hPrevTwin )
            lpTgtEI = zGETPTR( lpTgtEI->hPrevTwin );

         // Check to make sure tgt and src aren't the same.
         if ( lpSrcEI == lpTgtEI )
            break;

         // FALL THROUGH to perform the move.

      case zPOS_BEFORE:
         // Make sure we actually need to move something.
         if ( zGETPTR( lpTgtEI->hPrevTwin ) == lpSrcEI )
            break;  // lpSrcEI is already before lpTgtEI.

         // We don't move something before/after itself.
         if ( lpTgtEI == lpSrcEI )
            break;

         // Change hier pointers.
         lpSrcLastHier = fnFindLastEntityUnderParent( lpSrcEI, TRUE );
         if ( lpSrcEI->hPrevHier )
         {
            lpEI = zGETPTR( lpSrcEI->hPrevHier );
            lpEI->hNextHier = lpSrcLastHier->hNextHier;
         }

         if ( lpSrcLastHier->hNextHier )
         {
            lpEI = zGETPTR( lpSrcLastHier->hNextHier );
            lpEI->hPrevHier = lpSrcEI->hPrevHier;
         }

         if ( lpTgtEI->hPrevHier )
         {
            lpEI = zGETPTR( lpTgtEI->hPrevHier );
            lpEI->hNextHier = zGETHNDL( lpSrcEI );
         }

         lpSrcEI->hPrevHier       = lpTgtEI->hPrevHier;
         lpTgtEI->hPrevHier       = zGETHNDL( lpSrcLastHier );
         lpSrcLastHier->hNextHier = zGETHNDL( lpTgtEI );

         //
         // Change twin pointers.
         //
         if ( lpSrcEI->hPrevTwin )
         {
            lpEI = zGETPTR( lpSrcEI->hPrevTwin );
            lpEI->hNextTwin = lpSrcEI->hNextTwin;
         }

         if ( lpSrcEI->hNextTwin )
         {
            lpEI = zGETPTR( lpSrcEI->hNextTwin );
            lpEI->hPrevTwin = lpSrcEI->hPrevTwin;
         }

         if ( lpTgtEI->hPrevTwin )
         {
            lpEI = zGETPTR( lpTgtEI->hPrevTwin );
            lpEI->hNextTwin = zGETHNDL( lpSrcEI );
         }

         lpSrcEI->hPrevTwin = lpTgtEI->hPrevTwin;
         lpTgtEI->hPrevTwin = zGETHNDL( lpSrcEI );
         lpSrcEI->hNextTwin = zGETHNDL( lpTgtEI );

         break;

      case zPOS_LAST:
         // If we're moving the src entity to the end this is the same
         // as moving src after the last entity so change lpTgtEI to be
         // the last twin.
         while ( lpTgtEI->hNextTwin )
            lpTgtEI = zGETPTR( lpTgtEI->hNextTwin );

         // Check to make sure tgt and src aren't the same.
         if ( lpSrcEI == lpTgtEI )
            break;

         // FALL THROUGH to perform the move.

      case zPOS_AFTER:
         // Make sure we actually need to move something.
         if ( zGETPTR( lpTgtEI->hNextTwin ) == lpSrcEI )
            break;  // lpSrcEI is already next after lpTgtEI.

         // We don't move something before/after itself.
         if ( lpTgtEI == lpSrcEI )
            break;

         //
         // Change hier pointers.
         //

         lpSrcLastHier = fnFindLastEntityUnderParent( lpSrcEI, TRUE );
         lpTgtLastHier = fnFindLastEntityUnderParent( lpTgtEI, TRUE );

         if ( lpSrcEI->hPrevHier )
         {
            lpEI = zGETPTR( lpSrcEI->hPrevHier );
            lpEI->hNextHier = lpSrcLastHier->hNextHier;
         }

         if ( lpSrcLastHier->hNextHier )
         {
            lpEI = zGETPTR( lpSrcLastHier->hNextHier );
            lpEI->hPrevHier = lpSrcEI->hPrevHier;
         }

         if ( lpTgtLastHier->hNextHier )
         {
            lpEI = zGETPTR( lpTgtLastHier->hNextHier );
            lpEI->hPrevHier = zGETHNDL( lpSrcLastHier );
         }

         lpSrcEI->hPrevHier       = zGETHNDL( lpTgtLastHier );
         lpSrcLastHier->hNextHier = lpTgtLastHier->hNextHier;
         lpTgtLastHier->hNextHier = zGETHNDL( lpSrcEI );

         //
         // Change twin pointers.
         //
         if ( lpSrcEI->hPrevTwin )
         {
            lpEI = zGETPTR( lpSrcEI->hPrevTwin );
            lpEI->hNextTwin = lpSrcEI->hNextTwin;
         }

         if ( lpSrcEI->hNextTwin )
         {
            lpEI = zGETPTR( lpSrcEI->hNextTwin );
            lpEI->hPrevTwin = lpSrcEI->hPrevTwin;
         }

         if ( lpTgtEI->hNextTwin )
         {
            lpEI = zGETPTR( lpTgtEI->hNextTwin );
            lpEI->hPrevTwin = zGETHNDL( lpSrcEI );
         }

         lpSrcEI->hPrevTwin = zGETHNDL( lpTgtEI );
         lpSrcEI->hNextTwin = lpTgtEI->hNextTwin;
         lpTgtEI->hNextTwin = zGETHNDL( lpSrcEI );

         break;
   }

   // Change cursor position of the source view.
   switch ( nSrcPosition )
   {
      case zREPOS_FIRST:
         lpEI = lpTgtEI;
         while ( lpEI->hPrevTwin )
            lpEI = zGETPTR( lpEI->hPrevTwin );

         lpSrcViewEntityCsr->hEntityInstance = zGETHNDL( lpEI );
         break;

      case zREPOS_LAST:
         lpEI = lpTgtEI;
         while ( lpEI->hNextTwin )
            lpEI = zGETPTR( lpEI->hNextTwin );

         lpSrcViewEntityCsr->hEntityInstance = zGETHNDL( lpEI );
         break;

      case zREPOS_BEFORE:
         lpSrcViewEntityCsr->hEntityInstance = lpSrcEI->hPrevTwin;
         break;

      case zREPOS_AFTER:
         lpSrcViewEntityCsr->hEntityInstance = lpSrcEI->hNextTwin;
         break;

      case zREPOS_NONE:
         // Leave lpSrcViewEntityCsr as it is.
         break;
   }

   if ( lpSrcViewEntityCsr->hEntityInstance == NULL_CSR )
      lpSrcViewEntityCsr->hEntityInstance = UNSET_CSR;

   if ( lpSrcViewEntityCsr->hEntityInstance == UNSET_CSR )
   {
   // if ( lpSrcViewEntityCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnMoveSubobjectAmongTwins", "UNSET_CSR", -1 );

       // We can't allow the root entity to be null so reset it.
      if ( lpSrcViewEntityCsr->hParent == 0 )
      {
         LPENTITYINSTANCE lpFirstRoot;

         switch ( nSrcPosition )
         {
            case zREPOS_BEFORE:
            case zREPOS_FIRST:
               lpEI = lpTgtEI;
               while ( lpEI->hPrevTwin )
                  lpEI = zGETPTR( lpEI->hPrevTwin );

               lpSrcViewEntityCsr->hEntityInstance = zGETHNDL( lpEI );
            // if ( lpSrcViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "fnMoveSubobjectAmongTwins1", "UNSET_CSR", -1 );

               break;

            case zREPOS_AFTER:
            case zREPOS_LAST:
               lpEI = lpTgtEI;
               while ( lpEI->hNextTwin )
                  lpEI = zGETPTR( lpEI->hNextTwin );

               lpSrcViewEntityCsr->hEntityInstance = zGETHNDL( lpEI );
            // if ( lpSrcViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "fnMoveSubobjectAmongTwins2", "UNSET_CSR", -1 );

               break;
         }

         // Make sure ViewOI is pointing to the first EI.
         for ( lpFirstRoot = zGETPTR( lpViewOI->hRootEntityInstance );
               lpFirstRoot->hPrevTwin;
               lpFirstRoot = zGETPTR( lpFirstRoot->hPrevTwin ) )
         {
            // Nothing needs to be done here.
         }

         lpViewOI->hRootEntityInstance = zGETHNDL( lpFirstRoot );

         nRC = 0;
      }
      else
         nRC = zCURSOR_UNDEFINED;
   }
   else
      nRC = 0;

   // Make sure ViewOI is pointing to the first EI.
   if ( lpSrcViewEntityCsr->hParent == 0 )
   {
      LPENTITYINSTANCE lpFirstRoot;

      for ( lpFirstRoot = zGETPTR( lpViewOI->hRootEntityInstance );
            lpFirstRoot->hPrevTwin;
            lpFirstRoot = zGETPTR( lpFirstRoot->hPrevTwin ) )
      {
         // Nothing needs to be done here.
      }

      lpViewOI->hRootEntityInstance = zGETHNDL( lpFirstRoot );

      nRC = 0;
   }

   // If the entity has autosequencing then we need to set the update flags.
   lpViewEntity = zGETPTR( lpTgtEI->hViewEntity );
   if ( lpViewEntity->bAutoSeq )
   {
      lpSrcEI->u.nInd.bUpdated = TRUE;
      lpTgtEI->u.nInd.bUpdated = TRUE;

      // Set target instance as updated.
      lpViewOI->bUpdatedFile = lpViewOI->bUpdated = TRUE;
   }

   return( nRC );
}

//./ ADD NAME=MoveSubobject
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      MoveSubobject
//
//  PURPOSE:    To move a Subobject to a new parent either within
//              a current object instance or to another object instance.
//
//  DESCRIPTION: This operation moves a subobject by essentially
//               including the subobject under the new parent and
//               excluding the subobject from the old parent. For a
//               subobject to be moved, the object type MUST BE the
//               SAME OBJECT for both the source and target views, and the
//               ENTITY TYPEs MUST either be the same or a recursive
//               parent child of each other.
//
//               Since the subobject is being moved, the normal include
//               processing rules are not used. The source entity MUST
//               be excludable and the Target entity MUST be creatable.
//
//  PARAMETERS: lpTgtView        - View of where the subobject is to be
//                                 moved.
//              cpcTgtEntityName - Entity name of where the subobject is
//                                 to be moved.
//              lpSrcView        - View of where the subobject is coming
//                                 from.
//              cpcSrcEntityName - Entity name of where the subobject is
//                                 coming from.
//              nPosition        - Indicate where to move the subobject,
//                                 relative to the target's cursor position.
//              nSrcPosition     - Indicates where to position the cursor
//                                 of the source entity after the move.
//                                 May be zREPOS_FIRST, zREPOS_LAST,
//                                 zREPOS_NEXT, zREPOS_PREV or zREPOS_NONE.
//                                 zREPOS_NONE or an invalid value will
//                                 cause a zCURSOR_UNDEFINED return code.
//
//  RETURNS:    zCURSOR_... - Subobject moved, and this is the status
//                            for setting the cursor of the source entity.
//                            These return codes are shown under the
//                            operations for SetCursor...Entity.
//
//              zCALL_ERROR - Error moving Subobject
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
MoveSubobject( zVIEW   lpTgtView,
               zCPCHAR cpcTgtEntityName,
               zVIEW   lpSrcView,
               zCPCHAR cpcSrcEntityName,
               zSHORT  nPosition,
               zSHORT  nSrcPosition )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWCSR         lpTgtViewCsr;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPENTITYINSTANCE  lpSrcEntityInstance;
   LPENTITYINSTANCE  lpWkEntityInstance;
   zSHORT            nRC;

   // Validate that the source view entity exists and has a cursor
   // value for copying, we call fnValidateInstanceParameters for the
   // source since it requires the same editing as an attribute retrieval.
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpSrcViewEntity,
                                      &lpSrcViewEntityCsr,
                                      iMoveSubobject,
                                      lpSrcView,
                                      cpcSrcEntityName, 1 ) )
   {
      return( zCALL_ERROR );
   }

   // Validate that the view passed is valid.
   if ( fnValidViewCsr( lpCurrentTask, lpTgtView ) == 0 )
   {
      fnOperationReturn( iMoveSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpTgtViewCsr = zGETPTR( lpTgtView->hViewCsr );

   if ( (lpTgtViewEntity = fnValidViewEntity( &lpTgtViewEntityCsr,
                                              lpTgtView,
                                              cpcTgtEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iMoveSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure the source entity type and target entity type are either the
   // same view entity type or a recursive parent child entity pair.
   if ( lpSrcViewEntity != lpTgtViewEntity )
   {
      LPVIEWENTITY lpSSrcViewEntity;
      LPVIEWENTITY lpSTgtViewEntity;

      lpSSrcViewEntity = lpSrcViewEntity;
      lpSTgtViewEntity = lpTgtViewEntity;
      if ( lpSSrcViewEntity->bRecursive )
      {
         lpSSrcViewEntity = zGETPTR( lpSrcViewEntity->hParent );
         while ( lpSSrcViewEntity->lEREntTok != lpSrcViewEntity->lEREntTok )
            lpSSrcViewEntity = zGETPTR( lpSSrcViewEntity->hParent );
      }
      else
      if ( lpSTgtViewEntity->bRecursive )
      {
         lpSTgtViewEntity = zGETPTR( lpTgtViewEntity->hParent );
         while ( lpSTgtViewEntity->lEREntTok != lpTgtViewEntity->lEREntTok )
            lpSSrcViewEntity = zGETPTR( lpSSrcViewEntity->hParent );
      }

      if ( lpSSrcViewEntity != lpSTgtViewEntity )
      {
         // "KZOEE114 - Source and Target entities do not match"
         fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 114, 0,
                           lpSrcViewEntity->szName,
                           lpTgtViewEntity->szName );
         fnOperationReturn( iMoveSubobject, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   // Next get the source entity instance and ensure that we are
   // not attempting to move it under one of its children.
   lpSrcEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
   if ( lpTgtViewEntityCsr->hParent )
   {
      LPVIEWENTITYCSR lpTempCsr;

      lpTempCsr = zGETPTR( lpTgtViewEntityCsr->hParent );
      lpWkEntityInstance = zGETPTR( lpTempCsr->hEntityInstance );
   }
   else
      lpWkEntityInstance = zGETPTR( lpTgtViewCsr->hViewParentEntityInstance );

   // Set parent match indicator
   if ( zGETPTR( lpSrcEntityInstance->hParent ) == lpWkEntityInstance )
   {
      nRC = fnMoveSubobjectAmongTwins( lpTgtView,
                                       zGETPTR( lpTgtViewEntityCsr->hEntityInstance ),
                                       lpSrcViewEntityCsr, lpSrcEntityInstance,
                                       nPosition, nSrcPosition );
   }
   else
   {
      nRC = fnMoveSubobjectUnderNewParent( lpWkEntityInstance,
                                           lpTgtView,
                                           lpTgtViewEntityCsr,
                                           lpSrcView,
                                           lpSrcViewEntityCsr,
                                           nPosition, nSrcPosition,
                                           lpCurrentTask );
   }

   // Whew, I think we've done it, return to caller.
   fnOperationReturn( iMoveSubobject, lpCurrentTask );
   return( nRC );

}  // END of MoveSubobject

//./ ADD NAME=fnUpdateRelinkCursors
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnUpdateRelinkCursors
//
//  PURPOSE: This function is called by RelinkAllSubobjectsForOI after
//           an Exclude/Include is done to reset all entity cursors which
//           used to point to the passed instance or one of its linked
//           members to point to the next twin of the passed instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnUpdateRelinkCursors( LPENTITYINSTANCE lpEntityInstance )
{
   zSHORT            nOffset1, nOffset2;
   zLONG             lEREntTok;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpWkEntityInstance;

   lpWkEntityInstance = lpEntityInstance;
   do
   {
      if ( lpWkEntityInstance->u.nInd.bHidden &&
                                       lpWkEntityInstance->hNextTwin )
      {
         lpViewOI     = zGETPTR( lpWkEntityInstance->hViewOI );
         lpViewCsr    = zGETPTR( lpViewOI->hFirstViewCsr );
         lpViewEntity = zGETPTR( lpWkEntityInstance->hViewEntity );
         nOffset1 = lpViewEntity->nHierNbr - 1;
         if ( lpViewEntity->bRecursive )
         {
            nOffset2 = nOffset1;
            lEREntTok = lpViewEntity->lEREntTok;

            lpViewEntity = zGETPTR( lpViewEntity->hParent );
            while ( lpViewEntity->lEREntTok != lEREntTok )
               lpViewEntity = zGETPTR( lpViewEntity->hParent );

            nOffset1 = lpViewEntity->nHierNbr - 1;
         }
         else
            nOffset2 = 0;

         while ( lpViewCsr )
         {
            lpViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
            lpViewEntityCsr += nOffset1;
            if ( zGETPTR( lpViewEntityCsr->hEntityInstance ) == lpWkEntityInstance )
            {
               lpViewEntityCsr->hEntityInstance = lpWkEntityInstance->hNextTwin;
            // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "fnUpdateRelinkCursors1", "UNSET_CSR", -1 );

               fnResetCursorForViewChildren( lpViewEntityCsr );
            }
            else
            if ( nOffset2 )
            {
               lpViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
               lpViewEntityCsr += nOffset2;
               if ( zGETPTR( lpViewEntityCsr->hEntityInstance ) == lpWkEntityInstance )
               {
                  lpViewEntityCsr->hEntityInstance = lpWkEntityInstance->hNextTwin;
               // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
               //    SysMessageBox( 0, "fnUpdateRelinkCursors2", "UNSET_CSR", -1 );

                  fnResetCursorForViewChildren( lpViewEntityCsr );
               }
            }

            lpViewCsr = zGETPTR( lpViewCsr->hNextViewCsr );
         }
      }

      lpWkEntityInstance = zGETPTR( lpWkEntityInstance->hNextLinked );

   }  while ( lpWkEntityInstance &&
              lpWkEntityInstance != lpEntityInstance );

   return( 0 );
}

//./ ADD NAME=RelinkAllSubobjectsForOI
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   RelinkAllSubobjectsForOI
//
//  PURPOSE: This operation re-establishes links between two inter-object
//           subobjects by type.  Each subobject instance in the target
//           object instance is relinked to the corresponding subobject
//           instance in the source object ( if any ).
//
//  PARAMETERS: lpTgtView        -  Target view for link
//              cpcTgtEntityName -  Target subobject entity for link
//              lpSrcView        -  Source view for link
//              cpcSrcEntityName -  Source subobject entity for link
//
//  RETURNS: 0           - Success, All Target instances succesfully
//                         linked to source instance
//           1           - Success, Some target instances not found in
//                         instance
//           zCALL_ERROR - Error relinking entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
RelinkAllSubobjectsForOI( zVIEW    lpTgtView,
                          zCPCHAR  cpcTgtEntityName,
                          zVIEW    lpSrcView,
                          zCPCHAR  cpcSrcEntityName )
{
   LPTASK            lpCurrentTask;
   LPTASK            lpSrcOwningTask;
   LPTASK            lpTgtOwningTask;
   zVIEW             vTask;
   LPVIEWCSR         lpSrcViewCsr;
   LPVIEWCSR         lpTgtViewCsr;
   LPVIEWOI          lpSrcViewOI;
   LPVIEWOI          lpTgtViewOI;
   LPVIEWOD          lpSrcViewOD;
   LPVIEWOD          lpTgtViewOD;
   LPVIEWENTITY      hSrcViewEntity;
   LPVIEWENTITY      hTgtViewEntity;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITY      lpWkViewEntity;
   LPVIEWENTITY      lpInvertedSrcEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWENTITYCSR   lpInvertedSrcCsr;
   LPENTITYINSTANCE  lpSrcEntityInstance;
   LPENTITYINSTANCE  lpSrcRootEntityInstance;
   LPENTITYINSTANCE  lpTgtEntityInstance;
   LPENTITYINSTANCE  lpWkEntityInstance;
   zCHAR             szEntityString[ 255 ];
   zSHORT            bLeafNode;
   zSHORT            bIncluded;
   zBOOL             bUpdated;
   zBOOL             bUpdatedFile;
   zSHORT            nInvertedParents;
   zSHORT            nNotFound;
   zSHORT            nRC;

#ifdef            __RELINK_PERFORMANCE__
   zBOOL             bDoSrcTwin;
   zBOOL             bDoTgtTwin;
   zSHORT            nStopSrcHier;
   zSHORT            nStopTgtHier;
   LPENTITYINSTANCE  lpFirstSource;
   LPENTITYINSTANCE  lpLastSource;
#endif

   // Validate that the source view entity exists and has a cursor
   // value for copying, we call fnValidateInstanceParameters for the
   // source since it requires the same editing as an attribute retrieval.
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpSrcViewEntity,
                                      &lpSrcViewEntityCsr,
                                      iRelinkAllSubobjectsForOI,
                                      lpSrcView,
                                      cpcSrcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Validate that the view passed is valid
   if ( fnValidViewCsr( lpCurrentTask, lpTgtView ) == 0 )
   {
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( (lpTgtViewEntity = fnValidViewEntity( &lpTgtViewEntityCsr,
                                              lpTgtView,
                                              cpcTgtEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
      return( zCALL_ERROR );
   }

   hSrcViewEntity = zGETHNDL( lpSrcViewEntity );
   hTgtViewEntity = zGETHNDL( lpTgtViewEntity );

   // Make sure entity can be included.
   if ( lpSrcViewEntity->bInclSrc == FALSE )
   {
      strcpy_s( szEntityString, zsizeof( szEntityString ), "\nEntity: \t" );
      strcat_s( szEntityString, zsizeof( szEntityString ), lpSrcViewEntity->szName );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "Source Entity not includeble",
                        szEntityString );
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Get the viewOI for the source and the target.
   lpSrcViewCsr = zGETPTR( lpSrcView->hViewCsr );
   lpSrcViewOI  = zGETPTR( lpSrcViewCsr->hViewOI );
   lpTgtViewCsr = zGETPTR( lpTgtView->hViewCsr );
   lpTgtViewOI =  zGETPTR( lpTgtViewCsr->hViewOI );

   // If the source view oi is the target view oi, then it is an error.
   if ( lpSrcViewOI == lpTgtViewOI )
   {
      // "KZOEE140 - Target and source Object instance are the same instance"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 140, 0,
                        lpSrcViewEntity->szName,
                        lpTgtViewEntity->szName );
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure that the LODs are in sync.  They are out of sync if the LODs
   // were built with different dates for the ER.  We won't bother performing
   // the check if either LOD doesn't have the ER date.  This allows old LODs
   // to work.
   lpSrcViewOD = zGETPTR( lpSrcView->hViewOD );
   lpTgtViewOD = zGETPTR( lpTgtView->hViewOD );
   if ( lpSrcViewOD->szER_Date[ 0 ] && lpTgtViewOD->szER_Date[ 0 ] &&
        zstrcmp( lpSrcViewOD->szER_Date, lpTgtViewOD->szER_Date ) != 0 )
   {
      zCHAR szMsg[ 200 ];

      TraceLineS( "ERROR: The following LODs are out of sync",
                  " and should be re-built" );

      sprintf_s( szMsg, zsizeof( szMsg ), "Out-of-sync error. Target LOD = %s, date = %s",
                lpTgtViewOD->szName, lpTgtViewOD->szER_Date );
      TraceLineS( "ERROR: ", szMsg );

      sprintf_s( szMsg, zsizeof( szMsg ), "Out-of-sync error. Source LOD = %s, date = %s",
                lpSrcViewOD->szName, lpSrcViewOD->szER_Date );
      TraceLineS( "ERROR: ", szMsg );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "LODs are potentially out of sync.  See trace for more.",
                        lpTgtViewOD->szName );

      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
   // fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // We will attempt relinking with either the source OR
   // the target versioned, If the source is versioned, the relinking
   // will take place with the OLDEST version of the source chain.
   // If the target is versioned, we will relink to the OLDEST version
   // instances of the target chain and then propogate the records to
   // the newer versions in the chain.. If the keys have changed in ANY
   // of the target instances of the link of an older version, then error
   // will be issued and the relinking will STOP midstream!!!. The instance
   // itself will be in a clean-upable mode, but its data will be
   // not be 100% in sync!!
#if 0
   // Make sure that none of the source instances to be included are
   // versioned.
   if ( lpSrcViewOI->nVersionedInstances )
   {
      //  "KZOEE116 - Attempt to include subobject with "
      //  " versioned instances"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 116, 0,
                        lpSrcViewEntity->hName, 0 );
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
   // fnOperationReturn( iIncludeSubobjectFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpTgtViewOI->nVersionedInstances )
   {
      //  "KZOEE116 - Attempt to include subobject with "
      //  " versioned instances"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 116, 0,
                        lpSrcViewEntity->hName, 0 );
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
      return( zCALL_ERROR );
   }
#endif

   // Get source and target owning tasks
   lpSrcOwningTask = zGETPTR( lpSrcViewOI->hTask );
   lpTgtOwningTask = zGETPTR( lpTgtViewOI->hTask );

   // Validate that the Source and Target ViewOI belong to the
   // Same task
#if 0
   if ( lpSrcOwningTask != lpTgtOwningTask )
   {
      zSHORT       nError;

      // if we have a task mismatch, make sure the source task is the
      // system task AND that all the target entities of the include
      // are READ ONLY.
      nError = 1;
      if ( lpSrcOwningTask == zGETPTR( AnchorBlock->hMainTask ) )
      {
         nError = 0;
         lpTgtViewOI->hAllocTask = lpSrcViewOI->hTask;
      }

      if ( nError )
      {
         // "KZOEE131 - Attempt to include Subobject from another "
         // "           Application Task"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 131, 0, 0, 0 );
         fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }
#endif

   vTask = zGETPTR( lpCurrentTask->hFirstView );
   SysMutexLock( vTask, "Zeidon RLK", 0, 0 );

   // If the source and target structures do not match, error out here!
   if ( (nInvertedParents =
                fnValidSubobjectStructureMatch( lpTgtView, lpTgtViewEntity,
                                                lpSrcView,
                                                lpSrcViewEntity, 1 )) < 0 )
   {
      SysMutexUnlock( vTask, "Zeidon RLK", 0 );
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
      return( zCALL_ERROR );
   }

   bLeafNode = TRUE;
   lpWkViewEntity = zGETPTR( lpTgtViewEntity->hNextHier );
   while ( lpWkViewEntity &&
           lpWkViewEntity->nLevel > lpTgtViewEntity->nLevel )
   {
      if ( lpWkViewEntity->hParent == hTgtViewEntity &&
           lpWkViewEntity->bDerived == FALSE )
      {
         bLeafNode = 0;
         break;
      }

      lpWkViewEntity = zGETPTR( lpWkViewEntity->hNextHier );
   }

   // Next, see if the target parent is inverted in the source structure,
   // if so we need to do the re-include processing and drop the target
   // parent from the source structure before each re-include.
   lpInvertedSrcEntity = 0;
   lpInvertedSrcCsr    = 0;
   if ( lpTgtViewEntity->bDerived == FALSE && lpTgtViewEntity->hParent )
   {
      lpInvertedSrcEntity = zGETPTR( lpSrcViewEntity->hNextHier );
      while ( lpInvertedSrcEntity )
      {
         if ( lpInvertedSrcEntity->nLevel <= lpSrcViewEntity->nLevel )
         {
            lpInvertedSrcEntity = 0;
            break;
         }

         if ( lpInvertedSrcEntity->lERRelTok == lpTgtViewEntity->lERRelTok &&
              lpInvertedSrcEntity->hParent  == hSrcViewEntity  &&
              lpInvertedSrcEntity->bRelLink1 != lpTgtViewEntity->bRelLink1 &&
              lpInvertedSrcEntity->bDerived == FALSE )
         {
            break;
         }

         lpInvertedSrcEntity = zGETPTR( lpInvertedSrcEntity->hNextHier );
      }
   }

   // Make sure entity can be included UNLESS it is a leaf node.
   if ( lpTgtViewEntity->hParent && lpTgtViewEntity->bInclude == FALSE &&
        bLeafNode == FALSE )
   {
      strcpy_s( szEntityString, zsizeof( szEntityString ), "\nEntity: \t" );
      strcat_s( szEntityString, zsizeof( szEntityString ), lpTgtViewEntity->szName );

      // "KZOEE106 - Rules violation"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 8, 106, 0,
                        "Entity inclusion not allowed",
                        szEntityString );
      SysMutexUnlock( vTask, "Zeidon RLK", 0 );
      fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // OK, Finally we can get down to business, get a work view to the source
   // and target instance for doing the relinks
   lpSrcRootEntityInstance = zGETPTR( lpSrcViewOI->hRootEntityInstance );
   while ( lpSrcRootEntityInstance->hPrevVsn )
      lpSrcRootEntityInstance = zGETPTR( lpSrcRootEntityInstance->hPrevVsn );

   lpTgtEntityInstance = zGETPTR( lpTgtViewOI->hRootEntityInstance );
   while ( lpTgtEntityInstance->hPrevVsn )
      lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hPrevVsn );

#ifdef            __RELINK_PERFORMANCE__
   // PERFORMANCE CODE???
   bDoSrcTwin = !fnEntityInRecursivePath( lpSrcViewEntity );
   bDoTgtTwin = !fnEntityInRecursivePath( lpTgtViewEntity );
   nStopSrcHier = 0;
   if ( lpSrcViewOI->bMultipleRootLevels == FALSE )
   {
      lpWkViewEntity = zGETPTR( lpSrcViewEntity->hNextHier );
      while ( lpWkViewEntity && lpWkViewEntity->nLevel != 2 )
         lpWkViewEntity = zGETPTR( lpWkViewEntity->hNextHier );

      if ( lpWkViewEntity )
         nStopSrcHier = lpWkViewEntity->nHierNbr;
   }

   nStopTgtHier = 0;
   if ( lpTgtViewOI->bMultipleRootLevels == FALSE )
   {
      lpWkViewEntity = zGETPTR( lpTgtViewEntity->hNextHier );
      while ( lpWkViewEntity && lpWkViewEntity->nLevel != 2 )
         lpWkViewEntity = zGETPTR( lpWkViewEntity->hNextHier );

      if ( lpWkViewEntity )
         nStopTgtHier = lpWkViewEntity->nHierNbr;
   }

   if ( lpSrcRootEntityInstance )
   {
      lpFirstSource = fnFirstLevel2_ForViewEntity( lpSrcRootEntityInstance,
                                                   lpSrcViewEntity );
      if ( lpFirstSource )
      {
         LPVIEWENTITY lpFirstSourceViewEntity;

         lpFirstSourceViewEntity = zGETPTR( lpFirstSource->hViewEntity );
         while ( lpFirstSourceViewEntity != lpSrcViewEntity ||
                 lpFirstSource->u.nInd.bHidden )
         {
            lpFirstSource = zGETPTR( lpFirstSource->hNextHier );
            if ( lpFirstSource == 0 )
               break;

            lpFirstSourceViewEntity = zGETPTR( lpFirstSource->hViewEntity );
            if ( nStopSrcHier &&
                 lpFirstSourceViewEntity->nHierNbr >= nStopSrcHier )
            {
               lpFirstSource = 0;
               break;
            }
         }

         if ( lpFirstSource )
         {
            lpSrcEntityInstance = lpFirstSource;
            while ( lpSrcEntityInstance )
            {
               if ( lpSrcEntityInstance->hViewEntity == hSrcViewEntity &&
                    lpSrcEntityInstance->u.nInd.bHidden == FALSE )
               {
                  lpLastSource = lpSrcEntityInstance;
               }

               if ( bDoSrcTwin &&
                    lpSrcEntityInstance->hViewEntity == hSrcViewEntity )
               {
                  while ( lpSrcEntityInstance &&
                          lpSrcEntityInstance->hNextTwin == 0 )
                  {
                     lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hParent );
                  }

                  if ( lpSrcEntityInstance )
                     lpSrcEntityInstance =
                                    zGETPTR( lpSrcEntityInstance->hNextTwin );
               }
               else
                  lpSrcEntityInstance =
                                    zGETPTR( lpSrcEntityInstance->hNextHier );
            }
         }
      }
   }

   if ( lpSrcRootEntityInstance && lpTgtEntityInstance )
   {
      lpTgtEntityInstance = fnFirstLevel2_ForViewEntity( lpTgtEntityInstance,
                                                         lpTgtViewEntity );
   }
   // PERFORMANCE CODE???
#endif

   nNotFound = 0;

#ifdef __RELINK_PERFORMANCE__
   if ( lpFirstSource && lpTgtEntityInstance )
#else
   if ( lpSrcRootEntityInstance && lpTgtEntityInstance )
#endif
   {
      // Change lpTgtView and lpSrcView to work views
      nRC = CreateViewFromViewForTask( &lpTgtView, lpTgtView, 0 );
      if ( nRC == 0 )
         nRC = CreateViewFromViewForTask( &lpSrcView, lpSrcView, 0 );

      lpSrcViewEntityCsr = 0;
      lpTgtViewEntityCsr = 0;
      while ( nRC == 0 && lpTgtEntityInstance )
      {
         while ( lpTgtEntityInstance->hPrevVsn )
            lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hPrevVsn );

         if ( lpTgtEntityInstance->hViewEntity == hTgtViewEntity &&
              lpTgtEntityInstance->u.nInd.bHidden == FALSE )
         {
#ifdef            __RELINK_PERFORMANCE__
            lpSrcEntityInstance = lpFirstSource;
#else
            lpSrcEntityInstance = lpSrcRootEntityInstance;
#endif
            while ( lpSrcEntityInstance )
            {
               while ( lpSrcEntityInstance->hPrevVsn )
                  lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hPrevVsn );

               if ( lpSrcEntityInstance->hViewEntity == hSrcViewEntity &&
                    lpSrcEntityInstance->u.nInd.bHidden == FALSE &&
                    fnInstanceKeysMatch( lpSrcEntityInstance,
                                         lpTgtEntityInstance ) )
               {
                  if ( !fnInstanceLinkedToInstance( lpSrcEntityInstance,
                                                    lpTgtEntityInstance ) )
                  {
                     if ( bLeafNode && lpInvertedSrcEntity == 0 &&
                          lpTgtEntityInstance->u.nInd.bPrevVersion == FALSE )
                     {
                        fnRemoveOtherOI_LinkedInstances( lpTgtEntityInstance,
                                                         lpSrcEntityInstance );
                        fnRelinkInstanceToInstance( lpTgtEntityInstance,
                                                    lpSrcEntityInstance );
                     }
                     else
                     {
                        // Here we have a re-include situation, if the
                        // source target entity instance has a next
                        // version, then propagate the source subobject
                        // structure across the target instance and all
                        // its versions ensuring that neither structural
                        // changes NOR key changes have occurred between the
                        // source subobject and the target subobject

                        if ( lpTgtEntityInstance->u.nInd.bPrevVersion &&
                             fnStillPrevVersion( lpTgtEntityInstance ) )
                        {
                           nRC = fnRelinkVsnSubobjectToSubobject( lpTgtView,
                                                      lpTgtEntityInstance,
                                                      lpSrcView,
                                                      lpSrcEntityInstance,
                                                      nInvertedParents,
                                                      lpInvertedSrcEntity );
                        }
                        else
                        {
                           // if the source structure inverts the target
                           // then we need to find the instance in the
                           // source and drop it
                           if ( lpInvertedSrcEntity )
                           {
                              lpWkEntityInstance =
                                   zGETPTR( lpSrcEntityInstance->hNextHier );
                              while ( lpWkEntityInstance &&
                                      lpWkEntityInstance->nLevel > lpSrcEntityInstance->nLevel )
                              {
                                 if ( zGETPTR( lpWkEntityInstance->hViewEntity ) ==
                                                       lpInvertedSrcEntity &&
                                      lpWkEntityInstance->u.nInd.bHidden == FALSE &&
                                      fnInstanceKeysMatch(
                                             zGETPTR( lpTgtEntityInstance->hParent ),
                                                      lpWkEntityInstance ) )
                                 {
                                    lpInvertedSrcCsr =
                                       fnEstablishViewForInstance( lpSrcView, lpInvertedSrcCsr,
                                                         lpWkEntityInstance );

                                    nRC = fnExcludeEntity( lpSrcView,
                                                           lpInvertedSrcEntity,
                                                           lpInvertedSrcCsr,
                                                           zREPOS_NONE, 0 );
                                    break;
                                 }

                                 lpWkEntityInstance = zGETPTR( lpWkEntityInstance->hNextHier );
                              }
                           }

                           lpSrcViewEntityCsr =
                                 fnEstablishViewForInstance( lpSrcView,
                                                       lpSrcViewEntityCsr,
                                                       lpSrcEntityInstance );
                           lpTgtViewEntityCsr =
                                 fnEstablishViewForInstance( lpTgtView,
                                                       lpTgtViewEntityCsr,
                                                       lpTgtEntityInstance );

                           // First, let's save the included incremental flag
                           // for the target instance, so we can do the
                           // appropriate re-include marking.
                           bUpdated  = lpTgtViewOI->bUpdated;
                           bUpdatedFile = lpTgtViewOI->bUpdatedFile;
                           bIncluded = lpTgtEntityInstance->u.nInd.bIncluded;
                           nRC = fnExcludeEntity( lpTgtView, lpTgtViewEntity,
                                                  lpTgtViewEntityCsr,
                                                  zREPOS_NONE, 0 );
                           if ( nRC != zCALL_ERROR )
                           {
                              nRC = fnIncludeSubobjectFromSubobject(
                                                lpTgtView, lpTgtViewEntity,
                                                lpSrcView, lpSrcViewEntity,
                                                zPOS_AFTER, 0, bIncluded );

                              // If the re-include worked, re-establish any
                              // cursors pointing to the excluded instance.
                              if ( nRC == 0 )
                                 fnUpdateRelinkCursors( lpTgtEntityInstance );
                           }

                           lpTgtViewOI->bUpdated = bUpdated;
                           lpTgtViewOI->bUpdatedFile = bUpdatedFile;
                        }
                     }
                  }

                  break;
               }
#ifdef            __RELINK_PERFORMANCE__
               if ( lpSrcEntityInstance == lpLastSource )
                  lpSrcEntityInstance = 0;
               else
               if ( bDoSrcTwin &&
                    lpSrcEntityInstance->hViewEntity == hSrcViewEntity )
               {
                  while ( lpSrcEntityInstance &&
                          lpSrcEntityInstance->hNextTwin == 0 )
                  {
                     lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hParent );
                  }

                  if ( lpSrcEntityInstance )
                     lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextTwin );
               }
               else
                  lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );
#else
               lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );
#endif
            }

            if ( lpSrcEntityInstance == 0 )
               nNotFound = 1;  // Flag that one was not found
         }

         while ( lpTgtEntityInstance->hNextVsn )
            lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hNextVsn );

#ifdef            __RELINK_PERFORMANCE__
         if ( bDoTgtTwin &&
              lpTgtEntityInstance->hViewEntity == hTgtViewEntity )
         {
            while ( lpTgtEntityInstance && lpTgtEntityInstance->hNextTwin == 0 )
               lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hParent );

            if ( lpTgtEntityInstance )
               lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hNextTwin );
         }
         else
            lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hNextHier );

         if ( nStopTgtHier && lpTgtEntityInstance )
         {
            LPVIEWENTITY lpWrkViewEntity =
                              zGETPTR( lpTgtEntityInstance->hViewEntity );

            if ( lpWrkViewEntity->nHierNbr >= nStopTgtHier )
               lpTgtEntityInstance = 0;
         }
#else
         lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hNextHier );
#endif
      }

      DropView( lpSrcView );
      DropView( lpTgtView );
   }

   SysMutexUnlock( vTask, "Zeidon RLK", 0 );

   // Return the results.
   if ( nRC == 0 )
      nRC = nNotFound;  // Change 0 return code to not found state

   fnOperationReturn( iRelinkAllSubobjectsForOI, lpCurrentTask );
   return( nRC );
}

//=fnValidStructureEntity
// This function makes sure that all the children of lpTgtViewEntity exist in
// the lpSrcViewEntity structure.
zSHORT LOCALOPER
fnValidStructureEntity( zVIEW        lpTgtView,
                        LPVIEWENTITY lpTgtViewEntity,
                        zVIEW        lpSrcView,
                        LPVIEWENTITY lpSrcViewEntity,
                        zBOOL        bInclude )
{
   LPVIEWENTITY lpWrkSrcViewEntity;
   LPVIEWENTITY lpWrkTgtViewEntity;
   zSHORT       nRC;
   zSHORT       nInvertedParents = 0;

   // Forget it if the entity is derived.
   if ( lpTgtViewEntity->bDerived )
      return( 0 );

   if ( bInclude )
   {
      if ( lpTgtViewEntity->bRecursive  )
      {
         LPTASK lpTask = zGETPTR( lpSrcView->hTask );
         // "KZOEE138 - Attempt to include target entity with "
         // "           recursive behavior"
         fnIssueCoreError( lpTask, lpSrcView, 8, 138, 0,
                           lpTgtViewEntity->szName, 0 );
         return( zCALL_ERROR ); // Target Entity is recursive
      }
   }

   for ( lpWrkTgtViewEntity = zGETPTR( lpTgtViewEntity->hNextHier );
         lpWrkTgtViewEntity &&
            lpWrkTgtViewEntity->nLevel > lpTgtViewEntity->nLevel;
         lpWrkTgtViewEntity = zGETPTR( lpWrkTgtViewEntity->hNextHier ) )
   {
      // We only care about the direct children of lpTgtViewEntity.
      if ( lpWrkTgtViewEntity->nLevel != lpTgtViewEntity->nLevel + 1 )
         continue;

      // Forget it if the entity is derived.
      if ( lpWrkTgtViewEntity->bDerived )
         continue;

      // Check to see if one of the children of lpSrcViewEntity matches
      // lpWrkTgtViewEntity.
      for ( lpWrkSrcViewEntity = zGETPTR( lpSrcViewEntity->hNextHier );
            lpWrkSrcViewEntity &&
              lpWrkSrcViewEntity->nLevel > lpSrcViewEntity->nLevel;
            lpWrkSrcViewEntity = zGETPTR( lpWrkSrcViewEntity->hNextHier ) )
      {
         // We only care about the direct children of lpSrcViewEntity.
         if ( lpWrkSrcViewEntity->nLevel != lpSrcViewEntity->nLevel + 1 )
            continue;

         // Src and Tgt must have the same relationship.
         if ( lpWrkSrcViewEntity->lERRelTok != lpWrkTgtViewEntity->lERRelTok )
            continue;

         // Make sure relationship is the same direction.
         if ( lpWrkSrcViewEntity->bRelLink1 != lpWrkTgtViewEntity->bRelLink1 )
            continue;

         // If we get here then the previous two checks are OK and this
         // implies that the EREntTok values are the same.  This means that
         // the current lpWrkTgtViewEntity checks out OK so let's make sure that
         // it's children check out.
         nRC = fnValidStructureEntity( lpTgtView, lpWrkTgtViewEntity,
                                       lpSrcView,
                                       lpWrkSrcViewEntity, bInclude );
         if ( nRC == zCALL_ERROR )
            return( zCALL_ERROR );

         nInvertedParents += nRC;

         // We found what we wanted so break loop.
         break;
      }

      // If lpWrkSrcViewEntity is 0 then we didn't find a child of
      // lpSrcViewEntity that matched so let's check the parent.
      if ( lpWrkSrcViewEntity == 0 ||
           lpWrkSrcViewEntity->nLevel <= lpSrcViewEntity->nLevel )
      {
         if ( lpSrcViewEntity->hParent )
         {
            LPVIEWENTITY lpParent;

            // Src and Tgt must have the same relationship but it must be
            // reversed.
            if ( lpSrcViewEntity->lERRelTok == lpWrkTgtViewEntity->lERRelTok &&
                 lpSrcViewEntity->bRelLink1 != lpWrkTgtViewEntity->bRelLink1 )
            {
               // We have an inverted parent.
               nInvertedParents++;

               lpParent = zGETPTR( lpSrcViewEntity->hParent );
               nRC = fnValidStructureEntity( lpTgtView, lpWrkTgtViewEntity,
                                             lpSrcView, lpParent, bInclude );
               if ( nRC == zCALL_ERROR )
                  return( zCALL_ERROR );

               nInvertedParents += nRC;

               // To keep from getting an error lets set lpWrkSrcViewEntity.
               lpWrkSrcViewEntity = lpSrcViewEntity;
            }
         }
      }

   } // for each lpWrkTgtViewEntity...

   return( nInvertedParents );
}

//./ ADD NAME=fnValidSubobjectStructureMatch
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:    fnValidSubobjectStructureMatch
//
//  PURPOSE:     To validate that Target Subobject structure passed matches
//               (i.e. is a subset of) the source Subobject Structure so
//               an IncludeSubobjectFromSubobject can be done. This function
//               goes through each entity in the TargetSubobject and makes
//               sure that the corresponding Relationship/Entity exists
//               in the source subobject.  NOTE: A subobject match includes
//               inverting part of the structure on the source Subobject
//               providing the maximum cardinality is 1 on the target
//               Subobject.
//
//
//  ERRORS:      1. An include is to be done and either the source or
//                  the target Subobject does not have a full set
//                  of persistent attributes.
//               2. The Target Subobject is not a subset of the source
//                  Subobject.
//               3. The Source Subobject has inverted an Entity in the
//                  target subobject and the max cardinality on the
//                  target subobject entity is not 1.
//
//  RETURNS:    nInvertedParents - Number of inverted parents
//              zCALL_ERROR - Error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnValidSubobjectStructureMatch( zVIEW        lpTgtView,
                                LPVIEWENTITY lpTgtViewEntity,
                                zVIEW        lpSrcView,
                                LPVIEWENTITY lpSrcViewEntity,
                                zBOOL        bInclude )
{
   zSHORT nInvertedParents;

   nInvertedParents = 0;
   if ( lpSrcViewEntity->lEREntTok != lpTgtViewEntity->lEREntTok )
   {
      LPTASK lpTask = zGETPTR( lpSrcView->hTask );

      //  "KZOEE125 - Target and Source Entities are not the same E/R Entity"
      fnIssueCoreError( lpTask, lpSrcView, 8, 125, 0,
                        lpTgtViewEntity->szName,
                        lpSrcViewEntity->szName );
      return( zCALL_ERROR );  // entities do not match
   }

   nInvertedParents = fnValidStructureEntity( lpTgtView, lpTgtViewEntity,
                                              lpSrcView,
                                              lpSrcViewEntity, bInclude );

   // Return the number of inverted parents found.
   return( nInvertedParents );
}

//./ ADD NAME=fnInstanceChildOfInstance
// Source Module=kzoeeiaa.c
// ENTRY:   fnInstanceChildOfInstanceForRel
// RETURNS:   TRUE - A relationship between lpEntityInstance and lpParent
//                   already exists.
//            FALSE - No relationship exists yet.
//./ END + 4
zSHORT
fnInstanceChildOfInstanceForRel( LPENTITYINSTANCE lpEntityInstance,
                                 LPENTITYINSTANCE lpParentEntityInstance,
                                 zLONG            lERRelTok )
{
   LPVIEWENTITY     lpViewEntity;
   LPENTITYINSTANCE lpWkInstance1;
   LPENTITYINSTANCE lpWkInstance2;
   LPVIEWENTITY     hViewEntity;
   LPVIEWENTITY     lpParentViewEntity;
   zPVOID           hParentEntityInstance = zGETHNDL( lpParentEntityInstance );

   while ( lpEntityInstance->hPrevVsn )
      lpEntityInstance = zGETPTR( lpEntityInstance->hPrevVsn );

   lpWkInstance1 = lpEntityInstance;
   while ( lpWkInstance1 )
   {
      lpViewEntity= zGETPTR( lpWkInstance1->hViewEntity );
      if ( lpWkInstance1->u.nInd.bHidden == FALSE && // Gig, 9/13/92
           lpWkInstance1->hParent == hParentEntityInstance &&
           lpViewEntity->lERRelTok == lERRelTok )
      {
         lpParentViewEntity = zGETPTR( lpParentEntityInstance->hViewEntity );

         TraceLineS( "--- Error ---", "" );
         TraceLineS( "The relationship already exists between two entities:", "" );
         TraceLineX( "Object Instance ID = ", (zLONG) lpWkInstance1->hViewOI );
         TraceLineS( "Parent Name = ", lpParentViewEntity->szName );
         TraceLineX( "       EI   = ", (zLONG) lpParentEntityInstance );
         TraceLineI( "       Key  = ", (zLONG) lpParentEntityInstance->ulKey );
         TraceLineS( "", "" );
         TraceLineS( "Child Name = ", lpViewEntity->szName );
         TraceLineX( "      EI   = ", (zLONG) lpWkInstance1 );
         TraceLineI( "      Key  = ", (zLONG) lpWkInstance1->ulKey );
         return( TRUE );
      }

      lpWkInstance2 = zGETPTR( lpWkInstance1->hNextVsn );
      while ( lpWkInstance2 )
      {
         hViewEntity = zGETPTR( lpWkInstance2->hViewEntity );
         if ( lpWkInstance2->u.nInd.bHidden == FALSE && // Gig, 9/13/92
              lpWkInstance2->hParent == hParentEntityInstance &&
              hViewEntity->lERRelTok == lERRelTok )
         {
            lpParentViewEntity = zGETPTR( lpParentEntityInstance->hViewEntity );

            TraceLineS( "--- Error ---", "" );
            TraceLineS( "The relationship already exists between two entities:", "" );
            TraceLineX( "Object Instance ID = ", (zLONG) lpWkInstance2->hViewOI );
            TraceLineS( "Parent Name = ", lpParentViewEntity->szName );
            TraceLineX( "       EI   = ", (zLONG) lpParentEntityInstance );
            TraceLineI( "       Key  = ", (zLONG) lpParentEntityInstance->ulKey );
            TraceLineS( "", "" );
            TraceLineS( "Child Name = ", lpViewEntity->szName );
            TraceLineX( "      EI   = ", (zLONG) lpWkInstance2 );
            TraceLineI( "      Key  = ", (zLONG) lpWkInstance2->ulKey );
            return( TRUE );
         }

         lpWkInstance2 = zGETPTR( lpWkInstance2->hNextVsn );
      }

      lpWkInstance1 = zGETPTR( lpWkInstance1->hNextLinked );
      if ( lpWkInstance1 == 0 || lpWkInstance1 == lpEntityInstance )
      {
         lpWkInstance1 = zGETPTR( lpEntityInstance->hNextVsn );
         if ( lpWkInstance1 )
            lpEntityInstance = lpWkInstance1;
      }
   }

   return( FALSE );
}

//./ ADD NAME=fnIncludeEntityAndChildren
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnIncludeEntityAndChildren.
//
//  PURPOSE: This function includes a child from source to target and
//           then checks to see if any of the related source entities
//           need to be copied to the target.
//
//  NOTES:   The logic is handled recursively.  After copying the EI
//           indicated by lpOrigSrcEntityInstance to the target OI
//           we then go through each of the immediate children of the
//           orig EI and see if the relationship between source and
//           child exists as a child relationship between the target
//           EI just created and its children.  If the relationship
//           exists then we call this function again to copy the child
//           to the target OI and to then copy *its* children.
//
//           When this is done for an EI we then check to see if
//           the parent of lpOrigSrcEntityInstance should be copied to
//           the target.  If it is, we call this function again.  We
//           keep calling this function for parents until we find an
//           EI that should not have its parent copied.
//
//  RETURNS: 0 - OK.
//           zCALL_ERROR - Oops.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT LOCALOPER
fnIncludeEntityAndChildren( LPVIEWENTITY     lpTgtViewEntity,
                            LPVIEWENTITYCSR  lpTgtViewEntityCsr,
                            LPENTITYINSTANCE lpOrigSrcEntityInstance,
                            zBOOL            bIncludeParent,
                            LPINCLUDEINFO    lpIncludeInfo )
{
   LPVIEWENTITYCSR   lpChildTgtViewEntityCsr;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPENTITYINSTANCE  lpSrcEntityInstance;
   LPENTITYINSTANCE  lpTgtEntityInstance;
   LPENTITYINSTANCE  lpChildEntityInstance;
   LPVIEWENTITY      lpChildTgtViewEntity;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpSrcChildViewEntity;
   LPTASK            lpAllocTask = 0;
   LPTASK            hAllocTask = 0;
   zSHORT            nSourceLevel;
   zSHORT            nTargetLevel;
   zSHORT            nSkipType;
   zSHORT            nRC;

#ifdef DEBUG
   lpSrcViewEntity = zGETPTR( lpOrigSrcEntityInstance->hViewEntity );
#endif

   // Get the OLDEST source instance for updating the target instance
   // information.
   lpSrcEntityInstance = lpOrigSrcEntityInstance;
   while ( lpSrcEntityInstance->hPrevVsn )
      lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hPrevVsn );

   // Call fnCreateEntity to create the entity for the target object.
   nRC = fnCreateEntity( lpIncludeInfo->lpTgtView,
                         lpTgtViewEntity, lpTgtViewEntityCsr,
                         lpIncludeInfo->nPosition, lpIncludeInfo->bNewRoot );
   if ( nRC )
      return( zCALL_ERROR );

   // Get the target instance just created and point its record to the source
   // instance record.
   lpTgtEntityInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
   lpTgtEntityInstance->hPersistRecord = lpSrcEntityInstance->hPersistRecord;
   lpTgtEntityInstance->u.nInd  = lpSrcEntityInstance->u.nInd;

   // If the new entity is updated then set flag indicated that target OI
   // has been changed.  We check to see if updated is TRUE so that we can
   // avoid calling fnInstanceVersioned( ) whenever possible.
   if ( lpTgtViewEntity->bDerived == FALSE                &&
        lpTgtViewEntity->bDerivedPath == FALSE            &&
        (lpTgtEntityInstance->u.nInd.bIncluded ||
         lpTgtEntityInstance->u.nInd.bCreated  ||
         lpTgtEntityInstance->u.nInd.bUpdated)            &&
        lpIncludeInfo->lpTgtViewOI->bUpdatedFile == FALSE &&
        fnInstanceVersioned( lpTgtEntityInstance ) == FALSE )
   {
      lpIncludeInfo->lpTgtViewOI->bUpdated =
         lpIncludeInfo->lpTgtViewOI->bUpdatedFile = TRUE;
   }

   // Set the include flag only if:
   // o  This is the first EI we've created.  We only flag the first EI as
   //    being included.
   //    All the other EIs added by "spawning" the include have their
   //    flags left alone.
   // o  The call wants the entity flagged as included; i.e. bMarkedIncluded
   //    is TRUE.
   // o  The target instance has a parent.  If it has no parent then it is
   //    a root EI and therefore won't be included into anything.
   if ( lpIncludeInfo->bFirstEntity )
   {
      if ( lpIncludeInfo->bMarkIncluded && lpTgtEntityInstance->hParent )
         lpTgtEntityInstance->u.nInd.bIncluded = TRUE;

      // Set flag to indicate we've created the first entity.
      lpIncludeInfo->bFirstEntity = FALSE;

      // Turn off flags.
      lpIncludeInfo->bNewRoot  = 0;
      lpIncludeInfo->nPosition = zPOS_NEXT;
   }

   // Add the created entity into the link chain.
   if ( lpSrcEntityInstance->hNextLinked )
      lpTgtEntityInstance->hNextLinked = lpSrcEntityInstance->hNextLinked;
   else
      lpTgtEntityInstance->hNextLinked = zGETHNDL( lpSrcEntityInstance );

   lpSrcEntityInstance->hNextLinked = zGETHNDL( lpTgtEntityInstance );

#if 0
   // DGC 6-23-2006
   // We thought we would allow cross-task includes by moving the OIs to the
   // system task.  Unfortunately this opens tons of race-condition holes.
   // The views really need to be locked but we don't know all the OIs that
   // are linked together until we get here.  Until we figure something out,
   // we will not allow cross-task linking.
   // DGC 6-23-2006

   // Check to see if either the views or the OIs belong to different tasks.
   // If they do, we need to move the attribute record to the system or
   // application task.
   if ( lpIncludeInfo->lpTgtView->hTask != lpIncludeInfo->lpSrcView->hTask ||
        lpIncludeInfo->lpTgtAllocTask != lpIncludeInfo->lpSrcAllocTask )
   {
      // We know that either the source or target task is an application or
      // system task.  Find out which.
      if ( lpIncludeInfo->lpSrcAllocTask == zGETPTR( AnchorBlock->hMainTask ) )
         lpAllocTask = lpIncludeInfo->lpSrcAllocTask;
      else
      if ( lpIncludeInfo->lpTgtAllocTask == zGETPTR( AnchorBlock->hMainTask ) )
         lpAllocTask = lpIncludeInfo->lpTgtAllocTask;
      else
      if ( lpIncludeInfo->lpSrcAllocTask->bApplicationTask )
         lpAllocTask = lpIncludeInfo->lpSrcAllocTask;
      else
      if ( lpIncludeInfo->lpTgtAllocTask->bApplicationTask )
         lpAllocTask = lpIncludeInfo->lpTgtAllocTask;
      else
         // We should never get here.  If we do, then we didn't correctly
         // verify the views at the beginning.  Cause a crash.
         lpAllocTask = lpAllocTask->hNextTask;

      // Set the alloc task for both OIs.  One of these assignments will be
      // redundant.
      hAllocTask = zGETHNDL( lpAllocTask );
      lpIncludeInfo->lpTgtViewOI->hAllocTask = hAllocTask;
      lpIncludeInfo->lpSrcViewOI->hAllocTask = hAllocTask;

      // Now that we know the alloc task, move the record pointer to the
      // new task.
      lpTgtEntityInstance->hPersistRecord =
         fnCopyAttributeRecord( lpAllocTask,
                                zGETPTR( lpSrcEntityInstance->lpViewEntity ),
                                lpSrcEntityInstance->hPersistRecord), TRUE );
      fnDeleteAttributeRecord( zGETPTR( lpSrcEntityInstance->hViewEntity ),
                               lpSrcEntityInstance, TRUE );

      // Go through each of the linked instances and point them to the new
      // attribute record.
      for ( lpEI = zGETPTR( lpTgtEntityInstance->hNextLinked );
            lpEI != lpTgtEntityInstance;
            lpEI = zGETPTR( lpEI->hNextLinked ) )
      {
         // All ViewOIs that are contained in this linked chain need to have
         // to have the same hAllocTask.
         LPVIEWOI lpViewOI = zGETPTR( lpEI->hViewOI );
         lpViewOI->hAllocTask = hAllocTask;

         lpEI->hPersistRecord = lpTgtEntityInstance->hPersistRecord;
      }

   }
#endif

   // Go back to the newest src EI.
   lpSrcEntityInstance = lpOrigSrcEntityInstance;

   // Now loop through all the direct children of lpSrcEntityInstance and see
   // if they need to be copied over to the Tgt OI.  At the end of this loop,
   // we will call this function recursively to copy more entities.
   nSourceLevel = lpSrcEntityInstance->nLevel;
   nSkipType = 1;  // this means to start with the next hier from the src EI
   while ( TRUE )
   {
      LPVIEWENTITYCSR lpChildTgtByToken;

      switch ( nSkipType )
      {
         case 0:  // Normal case.  Just go to the next twin.
            if ( lpSrcEntityInstance->hNextTwin )
               lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextTwin );
            else
               lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );

            break;

         case 1:  // Skip to the next hier.
            lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );
            break;

         case 2:  // Skip current EI and all it's twins.
            while ( lpSrcEntityInstance->hNextTwin )
               lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextTwin );

            // Now go to the next hier.
            lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );
            break;
      }

      if ( lpSrcEntityInstance == 0 )
         break;

      // Unless told otherwise we will always skip to the next twin.
      nSkipType = 0;

      // Skip all hidden entities.
      if ( lpSrcEntityInstance->u.nInd.bHidden )
         continue;

      // We are only interested in EI's that are direct children of the
      // original source EI.
      if ( lpSrcEntityInstance->nLevel > nSourceLevel + 1 )
      {
         nSkipType = 2;    // Skip this EI and all it's twins.
         continue;
      }

      // If the level is <= to the original source EI then we are no longer
      // underneath (i.e. a child/descendent) of the original EI so we're done
      // looking for children.
      if ( lpSrcEntityInstance->nLevel <= nSourceLevel )
         break;

      // We have a new source instance.  Make sure the new source instance is
      // actually in the source view (i.e. not out of view).  It will not be
      // in the source view ONLY IF it is a recursive instance or a child of
      // a recursive instance.  NOTE: Versioning DOES NOT support including
      // into a target with a recursive structure!!!!
      lpSrcViewEntityCsr =
                  zGETPTR( lpIncludeInfo->lpSrcViewCsr->hRootViewEntityCsr );
      while ( lpSrcViewEntityCsr &&
              lpSrcViewEntityCsr->hViewEntity !=
                                          lpSrcEntityInstance->hViewEntity )
      {
         lpSrcViewEntityCsr = zGETPTR( lpSrcViewEntityCsr->hNextHier );
      }

      if ( lpSrcViewEntityCsr == 0 ||
           lpSrcViewEntityCsr->nLevel != lpSrcEntityInstance->nLevel )
      {
         // lpSrcEntityInstance does not belong to the current source
         // view (it is out of view).  Go on to the next one.
         nSkipType = 2;    // skip this EI and all its twins
         continue;
      }

      lpSrcViewEntity = zGETPTR( lpSrcEntityInstance->hViewEntity );

      // Finally, see if the current entity instance belongs in the target
      // Object Instance.  To do this we loop through all the view entities
      // starting with the root of the include (not the root of the OI).
      // When we find an entity that has the same:
      //    o entity name as the source entity
      //    o ER token as the source entity
      //    o ER relationship as the source entity
      //    o RelLink1 flag
      // then we know that the source entity should be copied (included) into
      // the target entity.  If we don't find an entity with the same name
      // then we'll use the entity that matches the other criteria (if any).
      lpChildTgtByToken = 0;
      nTargetLevel = lpTgtViewEntityCsr->nLevel;
      for ( lpChildTgtViewEntityCsr = zGETPTR( lpTgtViewEntityCsr->hNextHier );
            lpChildTgtViewEntityCsr;
            lpChildTgtViewEntityCsr = zGETPTR( lpChildTgtViewEntityCsr->hNextHier ) )
      {
         // We're only interested in direct children of lpTgtViewEntityCsr.
         if ( lpChildTgtViewEntityCsr->nLevel > nTargetLevel + 1 )
            continue;

         if ( lpChildTgtViewEntityCsr->nLevel <= nTargetLevel )
         {
            lpChildTgtViewEntityCsr = 0;
            break;  // No more child csrs to check.
         }

         lpChildTgtViewEntity = zGETPTR( lpChildTgtViewEntityCsr->hViewEntity );

         if ( lpSrcViewEntity->lEREntTok == lpChildTgtViewEntity->lEREntTok &&
              lpSrcViewEntity->lERRelTok == lpChildTgtViewEntity->lERRelTok &&
              lpSrcViewEntity->bRelLink1 == lpChildTgtViewEntity->bRelLink1 )
         {
            // Check to see if the entities match by name also. If they do
            // then we've found our man.  If not we'll save the current view
            // cursor in case we never find a match by name.
            if ( zstrcmp( lpSrcViewEntity->szName,
                          lpChildTgtViewEntity->szName ) == 0 )
            {
               break; // they match, so break
            }
            else
            {
               // If we haven't already saved the cursor then do it.
               if ( lpChildTgtByToken == 0 )
                  lpChildTgtByToken = lpChildTgtViewEntityCsr;
            }
         }
      }

      // If we didn't find a csr by name maybe we found one by token.
      if ( lpChildTgtViewEntityCsr == 0 )
         lpChildTgtViewEntityCsr = lpChildTgtByToken;

      if ( lpChildTgtViewEntityCsr != 0 )
         lpChildTgtViewEntity = zGETPTR( lpChildTgtViewEntityCsr->hViewEntity );

      // If we didn't find a csr then go on to the next EI.  Also ...
      // Gig, patch 9/4/92, if we find a cursor which is
      // derived, skip this entity instance and all children of
      // the current entity instance.
      if ( lpChildTgtViewEntityCsr == 0 || lpChildTgtViewEntity->bDerived )
      {
         // We can skip the current source EI and all it's twins because we know
         // that there is no match in the target view for the current source
         // entity type.
         nSkipType = 2;
         continue;
      }

      // Finally check to see if the source instance has a previous version
      // which cannot disappear in an CancelSubobject call.
      if ( lpIncludeInfo->nVsnLevel != 0 ||
           lpSrcEntityInstance->hPrevVsn != 0 )
      {
         // Establish the new nPrevVsn if necessary
         if ( lpIncludeInfo->nVsnLevel &&
              lpSrcEntityInstance->nLevel <= lpIncludeInfo->nVsnLevel )
         {
            lpIncludeInfo->nVsnLevel = 0;
         }

         if ( lpIncludeInfo->nVsnLevel == 0 && lpSrcEntityInstance->hPrevVsn )
            lpIncludeInfo->nVsnLevel = lpSrcEntityInstance->nLevel;

         // If nVsnLevel is set, then the current source instance is part of
         // a source version chain, make sure this instance cannot disappear
         // by a CancelSubobject call at ANY LEVEL.
         if ( lpIncludeInfo->nVsnLevel != 0 )
         {
            LPENTITYINSTANCE lpWrkEntityInstance = lpSrcEntityInstance;

            while ( lpWrkEntityInstance->hPrevVsn )
               lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hPrevVsn );

            while ( lpWrkEntityInstance )
            {
               if ( lpWrkEntityInstance->hPrevVsn )
                  break;

               lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hParent );
            }

            // If we found an instance then we need to skip this guy and his
            // twins because a parent has a previous version.
            if ( lpWrkEntityInstance )
            {
               nSkipType = 2;  // Skip all twins.
               continue;
            }
         }

      } // Checking version level...

      // Whew...if we get here then it's OK to copy the src EI to the target
      // OI.  We call ourselves recursively so that we do this all over again.
      if ( fnIncludeEntityAndChildren( lpChildTgtViewEntity,
                                       lpChildTgtViewEntityCsr,
                                       lpSrcEntityInstance,
                                       FALSE,  // don't try to include parent
                                       lpIncludeInfo ) == zCALL_ERROR )
      {
         return( zCALL_ERROR );
      }

   } // while ( TRUE )...

   // If we don't need to try to include the parent then return OK.
   if ( bIncludeParent == FALSE )
      return( 0 );

   // If the current target view entity doesn't have a next hier then it has
   // no child so there's nothing to include.
   if ( lpTgtViewEntityCsr->hNextHier == 0 )
      return( 0 );

   // If the source entity instance doesn't have a parent then we're done.
   lpSrcEntityInstance = lpOrigSrcEntityInstance;
   if ( lpSrcEntityInstance->hParent == 0 )
      return( 0 );

   lpChildTgtViewEntityCsr = zGETPTR( lpTgtViewEntityCsr->hNextHier );
   lpChildTgtViewEntity    = zGETPTR( lpChildTgtViewEntityCsr->hViewEntity );

   // If the level of the child is <= the level of the TgtViewEntity then
   // it really isn't a child.
   if ( lpChildTgtViewEntity->nLevel <= lpTgtViewEntity->nLevel )
      return( 0 );

   lpChildEntityInstance = lpSrcEntityInstance;
   lpSrcEntityInstance   = zGETPTR( lpSrcEntityInstance->hParent );
   lpSrcViewEntity       = zGETPTR( lpSrcEntityInstance->hViewEntity );
   lpSrcChildViewEntity  = zGETPTR( lpChildEntityInstance->hViewEntity );

   while ( lpChildTgtViewEntity->lEREntTok != lpSrcViewEntity->lEREntTok      ||
           lpChildTgtViewEntity->lERRelTok != lpSrcChildViewEntity->lERRelTok ||
           lpChildTgtViewEntity->bRelLink1 == lpSrcChildViewEntity->bRelLink1 ||
           lpChildTgtViewEntityCsr->nLevel != lpTgtViewEntityCsr->nLevel + 1 )
   {
      lpChildTgtViewEntityCsr = zGETPTR( lpChildTgtViewEntityCsr->hNextHier );

      // If csr is 0 then there is no match for the parent so just return.
      if ( lpChildTgtViewEntityCsr == 0 )
         return( 0 );

      lpChildTgtViewEntity = zGETPTR( lpChildTgtViewEntityCsr->hViewEntity );

      // If the level of the child is <= the level of the TgtViewEntity then
      // we have reached the end of the children and there's no match.
      if ( lpChildTgtViewEntity->nLevel <= lpTgtViewEntity->nLevel )
         return( 0 );
   }

   // Copy the parent src EI to the target OI.
   if ( fnIncludeEntityAndChildren( lpChildTgtViewEntity,
                                    lpChildTgtViewEntityCsr,
                                    lpSrcEntityInstance,
                                    TRUE,  // try including the parent as well
                                    lpIncludeInfo ) == zCALL_ERROR )
   {
      return( zCALL_ERROR );
   }

   return( 0 );
}

//./ ADD NAME=fnIncludeSubobjectFromSubobject
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnIncludeSubobjectFromSubobject
//
//  PURPOSE:    This is a recursive function for doing the bulk of the
//              IncludeSubobjectFromSubobject OPERATION code. This
//              function is written to skip the initial editing done
//              by the IncludeSubobjectFromSubobject OPERATION and
//              be used recursively to spawn subobject includes when
//              Entity/Relationship structures appear multiple times in a
//              view object (either inverted or not). The key edit we
//              need to circumvent is the include rule on spawned includes
//              (cannot be included from outside object services but
//              must be abled to be spawned internally).
//
//
//  RETURNS:    0 - Attribute parameters valid
//              zCALL_ERROR - Error validating the attribute parameters
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT
fnIncludeSubobjectFromSubobject( zVIEW          lpTgtView,
                                 LPVIEWENTITY   lpTgtViewEntity,
                                 zVIEW          lpSrcView,
                                 LPVIEWENTITY   lpSrcViewEntity,
                                 zSHORT         nPosition,
                                 zSHORT         bNewRoot,
                                 zSHORT         bMarkIncluded )
{
   LPVIEWOI          lpSrcViewOI;
   LPVIEWOI          lpTgtViewOI;
   LPVIEWCSR         lpSrcViewCsr;
   LPVIEWCSR         lpTgtViewCsr;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPENTITYINSTANCE  lpParentEntityInstance;
   LPENTITYINSTANCE  lpWrkEntityInstance;
   IncludeInfoRecord IncludeInfo;
   zSHORT            nRC = zCALL_ERROR;

   // Get the view cursors for the source and target views.
   lpSrcViewCsr = zGETPTR( lpSrcView->hViewCsr );
   lpTgtViewCsr = zGETPTR( lpTgtView->hViewCsr );

   // Get the view Object Instances for the source and target views.
   lpSrcViewOI = zGETPTR( lpSrcViewCsr->hViewOI );
   lpTgtViewOI = zGETPTR( lpTgtViewCsr->hViewOI );

   lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hRootViewEntityCsr );
   while ( lpSrcViewEntityCsr->nHierNbr != lpSrcViewEntity->nHierNbr )
      lpSrcViewEntityCsr++;

   lpTgtViewEntityCsr = zGETPTR( lpTgtViewCsr->hRootViewEntityCsr );
   while ( lpTgtViewEntityCsr->nHierNbr != lpTgtViewEntity->nHierNbr )
      lpTgtViewEntityCsr++;

   // Now that we know we can safely insert the target entity in the
   // target object, check that the source and target objects are
   // compatible for inclusion from one subobject to the other.
   if ( fnValidSubobjectStructureMatch( lpTgtView, lpTgtViewEntity,
                                        lpSrcView,
                                        lpSrcViewEntity, 1 ) < 0 )
   {
      return( zCALL_ERROR );
   }

   // Next, check to make sure that the source entity or any of its
   // linked instances is not already a child of the target's parent.
   // To do this, compare the parents of the source instance and all
   // its linked instances to the parent of the target instance.  If
   // a match is found, issue an error.
   if ( lpTgtViewEntityCsr->hEntityInstance &&
        lpTgtViewEntityCsr->hEntityInstance != UNSET_CSR )
   {
      LPENTITYINSTANCE lpEntityInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );

      lpParentEntityInstance = zGETPTR( lpEntityInstance->hParent );
      if ( lpParentEntityInstance )
      {
         if ( fnInstanceChildOfInstanceForRel( zGETPTR( lpSrcViewEntityCsr->hEntityInstance ),
                                               lpParentEntityInstance,
                                               lpTgtViewEntity->lERRelTok ) )
         {
            LPTASK lpTask = zGETPTR( lpSrcView->hTask );
            zCHAR szMsg[ 400 ];

            // "KZOEE129 - "KZOEE129 - Trying to create a relationship (via "
            //             Include, possibly as part of a spawn) that already "
            //             exists.  See trace for more."
            sprintf_s( szMsg, zsizeof( szMsg ), "Source view entity = %s, Target view entity = %s",
                      lpSrcViewEntity->szName, lpTgtViewEntity->szName );
            fnIssueCoreError( lpTask, lpSrcView, 8, 129, 0, szMsg, 0 );
            return( zCALL_ERROR );
         }
      }
   }

   // Ensure that neither the source instance nor the target
   // position are a previous version or have a parent which is a
   // previous version.  A previous versioned parent may exist in the
   // case where a structure is inverted.
   lpWrkEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );

   // 3/4/93 - Error changed so it is issued only when the instance
   // has been created under a versioned parent.
   while ( lpWrkEntityInstance->hPrevVsn )
      lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hPrevVsn );

   while ( lpWrkEntityInstance )
   {
      if ( lpWrkEntityInstance->hPrevVsn )
         break;

      lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hParent );
   }

   // If a versioned entity instance was found, issue a logic error.
   if ( lpWrkEntityInstance )
   {
      LPENTITYINSTANCE lpSrcInstance;
      LPENTITYINSTANCE lpTgtParent;

      // We have a source instance which may go away as the result of
      // a cancel, make sure the target parent is in the same version.
      lpSrcInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
      while ( lpSrcInstance->hPrevVsn )
         lpSrcInstance = zGETPTR( lpSrcInstance->hPrevVsn );

      while ( lpSrcInstance )
      {
         if ( lpSrcInstance->hPrevVsn )
            break;

         lpSrcInstance = zGETPTR( lpSrcInstance->hParent );
      }

      // The source instance is versioned, now it is pointing to
      // the root of sources previous version, see if the parent
      // target entity instance is part of the same version, if so
      // then the include can happen, otherwise the include must fail
      // because it may disappear as the result of a cancel call.
      if ( lpSrcInstance && lpTgtViewEntityCsr->hParent )
      {
         LPVIEWENTITYCSR lpWorkParentCsr;

         lpSrcInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
         while ( lpSrcInstance )
         {
            LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpSrcInstance->hPrevVsn );

            if ( lpSrcInstance->u.nInd.bTemporal ||
                 (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
            {
               break;
            }

            lpSrcInstance = zGETPTR( lpSrcInstance->hParent );
         }

         lpWorkParentCsr = zGETPTR( lpTgtViewEntityCsr->hParent );
         if ( lpWorkParentCsr->hEntityInstance == UNSET_CSR )
            fnEstablishCursorForView( lpWorkParentCsr );

         lpTgtParent = zGETPTR( lpWorkParentCsr->hEntityInstance );
         while ( lpTgtParent )
         {
            LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpTgtParent->hPrevVsn );

            if ( lpTgtParent->u.nInd.bTemporal ||
                 (lpPrevVsn && lpPrevVsn->u.nInd.bPrevVsnRoot) )
            {
               break;
            }

            lpTgtParent = zGETPTR( lpTgtParent->hParent );
         }

         if ( lpTgtParent == 0 || lpTgtParent != lpSrcInstance )
            lpSrcInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
         else
            lpSrcInstance = 0;
      }

      if ( lpSrcInstance )
      {
         LPTASK lpTask = zGETPTR( lpSrcView->hTask );
         // "KZOEE133 - Attempt to include an Entity Instance with "
         //             versioned parent"
         //
         //  NOTE!! Message has been changed to
         //
         //  "KZOEE116 - Attempt to include an instance created under
         //  "           a versioned parent"
         //
         fnIssueCoreError( lpTask, lpSrcView, 8, 116, 0,
                           lpSrcViewEntity->szName,
                           lpTgtViewEntity->szName );
         return( zCALL_ERROR );
      }
   }

   // Set up include info.
   IncludeInfo.lpTgtView      = lpTgtView;
   IncludeInfo.lpTgtViewOI    = lpTgtViewOI;
   IncludeInfo.lpTgtAllocTask = zGETPTR( lpTgtViewOI->hAllocTask );
   IncludeInfo.lpSrcView      = lpSrcView;
   IncludeInfo.lpSrcViewOI    = lpSrcViewOI;
   IncludeInfo.lpSrcViewCsr   = lpSrcViewCsr;
   IncludeInfo.lpSrcAllocTask = zGETPTR( lpSrcViewOI->hAllocTask );
   IncludeInfo.nVsnLevel      = 0;
   IncludeInfo.bFirstEntity   = TRUE;
   IncludeInfo.bNewRoot       = bNewRoot;
   IncludeInfo.nPosition      = nPosition;
   IncludeInfo.bMarkIncluded  = bMarkIncluded;

   if ( fnIncludeEntityAndChildren( lpTgtViewEntity,
                                    lpTgtViewEntityCsr,
                                    zGETPTR( lpSrcViewEntityCsr->hEntityInstance ),
                                    TRUE, &IncludeInfo ) == zCALL_ERROR )
   {
      goto EndOfFunction;
   }

   // R056357
   // bDerived is checked inside fnSetUpdateIndicator, so don't check it here.
   if ( bMarkIncluded &&
        lpTgtViewEntityCsr->hParent && lpTgtViewEntityCsr->hEntityInstance )
   {
      // HH 2001.08.03 use fnSetUpdateIndicator.
      LPENTITYINSTANCE lpEI = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
      fnSetUpdateIndicator( lpTgtViewOI, lpEI, 0, TRUE, FALSE, FALSE );
   }

   // Now that we have included the source subobject in the target
   // instance, see if we need to spawn a new include because the
   // target instance contains the same structure a second time at
   // the definition level either inverted or not.
   nRC = fnSpawnIncludeSubobject( lpTgtView, lpTgtViewOI,
                                  lpTgtViewEntity, lpTgtViewEntityCsr,
                                  bMarkIncluded );

EndOfFunction:

   // Reset the cursor for the target entity view children.
   fnResetCursorForViewChildren( lpTgtViewEntityCsr );

   // Simple enough, let's return to caller.
   return( nRC );
}

//./ ADD NAME=fnSpawnIncludeSubobject
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSpawnIncludeSubobject
//
//  PURPOSE:    This function checks to see if a created Subobject needs
//              spawning anywhere else in the structure or linked
//              structures, if so, spawning is done.
//
//  RETURNS:    0   - Success
//              !0  - Error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnSpawnIncludeSubobject( zVIEW            lpTgtView,
                         LPVIEWOI         lpTgtViewOI,
                         LPVIEWENTITY     lpTgtViewEntity,
                         LPVIEWENTITYCSR  lpTgtViewEntityCsr,
                         zSHORT           bMarkIncluded )
{
   LPVIEWENTITY      lpSrchViewEntity;
   LPVIEWENTITY      lpRecursViewEntity;
   LPVIEWENTITY      lpWkViewEntity;
   LPVIEWENTITYCSR   lpSpawnCsr;
   LPENTITYINSTANCE  lpTgtInstance;
   LPENTITYINSTANCE  lpSrchInstance;
   LPENTITYINSTANCE  lpSrchChild;
   LPENTITYINSTANCE  lpTgtParent;
   zVIEW             lpSpawnView;
   zSHORT            nRC;

   lpTgtInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
   lpTgtParent   = zGETPTR( lpTgtInstance->hParent );

   // Can't spawn a ROOT or a derived relationship.
   if ( lpTgtParent == 0 || lpTgtViewEntity->bDerived )
      return( 0 );

   // Spawning pass 1, for every visible linked instance of the entity
   // instance, see if the structure is inverted anywhere and needs
   // to be spawned.
   nRC = 0;
   lpSrchInstance = zGETPTR( lpTgtInstance->hNextLinked );

   // Do pass 1 looking for inverted relationships of the target.
   while ( nRC == 0 && lpSrchInstance && lpSrchInstance != lpTgtInstance )
   {
      if ( lpSrchInstance->u.nInd.bHidden == FALSE )
      {
         lpSrchViewEntity = zGETPTR( lpSrchInstance->hViewEntity );
         lpRecursViewEntity = lpSrchViewEntity;
         if ( lpSrchViewEntity->bRecursive )
         {
            do
            {
               lpSrchViewEntity = zGETPTR( lpSrchViewEntity->hParent );

            } while ( lpSrchViewEntity->lEREntTok != lpRecursViewEntity->lEREntTok );
         }

         // See if the linked entity instance has a child entity type
         // which is a non-derived inversion of the target entity type.
         lpWkViewEntity = zGETPTR( lpSrchViewEntity->hNextHier );
         while ( lpWkViewEntity )
         {
            if ( lpWkViewEntity->nLevel <= lpSrchViewEntity->nLevel )
            {
               lpWkViewEntity = 0;
               break;
            }

            if ( zGETPTR( lpWkViewEntity->hParent ) == lpSrchViewEntity &&
                 lpWkViewEntity->lERRelTok == lpTgtViewEntity->lERRelTok &&
                 lpWkViewEntity->bRelLink1 != lpTgtViewEntity->bRelLink1 &&
                 fnInstanceVersionRootMatch( lpSrchInstance,
                                             lpTgtInstance ) &&
                 lpWkViewEntity->bDerived == FALSE )
            {
               // Here we have found an instance which has a child
               // entity type for spawning. Make sure this include
               // has not already been spawned.
               lpSrchChild = zGETPTR( lpTgtParent->hNextLinked );
               while ( lpSrchChild && lpSrchChild != lpTgtParent )
               {
                  if ( zGETPTR( lpSrchChild->hParent ) == lpSrchInstance &&
                       zGETPTR( lpSrchChild->hViewEntity ) == lpWkViewEntity &&
                       lpSrchChild->u.nInd.bHidden == FALSE )
                  {
                     break;
                  }

                  lpSrchChild = zGETPTR( lpSrchChild->hNextLinked );
               }

               // If the spawn has not been done for this inversion, do it now.
               if ( lpSrchChild == 0 || lpSrchChild == lpTgtParent )
               {
                  LPVIEWOI  lpViewOI;
                  LPVIEWCSR lpViewCsr;

                  // Get a view to the entity instance which is the
                  // parent for the new spawn
                  lpViewOI    = zGETPTR( lpSrchInstance->hViewOI );
                  lpViewCsr   = zGETPTR( lpViewOI->hFirstViewCsr );
                  lpSpawnView = zGETPTR( lpViewCsr->hView );
                  CreateViewFromViewForTask( &lpSpawnView, lpSpawnView, 0 );
                  lpSpawnCsr = fnEstablishViewForInstance( lpSpawnView, 0,
                                                           lpSrchInstance );
                  if ( lpRecursViewEntity != lpSrchViewEntity )
                  {
                     fnSetViewToSubobject( lpSpawnView, lpRecursViewEntity,
                                           lpSpawnCsr );
                     while ( zGETPTR( lpSpawnCsr->hViewEntity ) !=
                                                            lpSrchViewEntity )
                     {
                        lpSpawnCsr = zGETPTR( lpSpawnCsr->hParent );
                     }
                  }

                  while ( zGETPTR( lpSpawnCsr->hViewEntity ) !=
                                                            lpWkViewEntity )
                  {
                     lpSpawnCsr = zGETPTR( lpSpawnCsr->hNextHier );
                  }

                  fnEstablishCursorForView( lpSpawnCsr );
                  nRC = fnIncludeSubobjectFromSubobject( lpSpawnView,
                                       lpWkViewEntity, lpTgtView,
                                       zGETPTR( lpTgtViewEntity->hParent ),
                                       zPOS_LAST, 0, bMarkIncluded );
                  DropView( lpSpawnView );
               }
            }

            lpWkViewEntity = zGETPTR( lpWkViewEntity->hNextHier );
         }
      }

      // Now travel to next linked instance and try again.
      lpSrchInstance = zGETPTR( lpSrchInstance->hNextLinked );
   }

   // Spawning pass 2, for every visible linked instance of the entity parent,
   // see if the same structure exists anywhere and needs to be spawned.
   nRC = 0;
   lpSrchInstance = zGETPTR( lpTgtParent->hNextLinked );

   while ( nRC == 0 && lpSrchInstance && lpSrchInstance != lpTgtParent )
   {
      if ( lpSrchInstance->u.nInd.bHidden == FALSE )
      {
         lpSrchViewEntity = zGETPTR( lpSrchInstance->hViewEntity );
         lpRecursViewEntity = lpSrchViewEntity;
         if ( lpSrchViewEntity->bRecursive )
         {
            do
            {
               lpSrchViewEntity = zGETPTR( lpSrchViewEntity->hParent );

            } while ( lpSrchViewEntity->lEREntTok != lpRecursViewEntity->lEREntTok );
         }

         // See if the linked entity instance has a child entity type
         // which is a non derived inversion of the target entity type.
         lpWkViewEntity = zGETPTR( lpSrchViewEntity->hNextHier );
         while ( lpWkViewEntity )
         {
            if ( lpWkViewEntity->nLevel <= lpSrchViewEntity->nLevel )
            {
               lpWkViewEntity = 0;
               break;
            }

            if ( lpWkViewEntity->hParent == zGETHNDL( lpSrchViewEntity ) &&
                 lpWkViewEntity->lERRelTok == lpTgtViewEntity->lERRelTok &&
                 lpWkViewEntity->bRelLink1 == lpTgtViewEntity->bRelLink1 &&
                 fnInstanceVersionRootMatch( lpSrchInstance,
                                             lpTgtParent ) &&
                 lpWkViewEntity->bDerived == FALSE )
            {
               // Here we have found an instance which has a child entity
               // type for spawning. Make sure this include has not already
               // been spawned.
               lpSrchChild = zGETPTR( lpTgtInstance->hNextLinked );
               while ( lpSrchChild && lpSrchChild != lpTgtInstance )
               {
                  if ( lpSrchChild->hParent == zGETHNDL( lpSrchInstance ) &&
                       lpSrchChild->hViewEntity == zGETHNDL( lpWkViewEntity ) &&
                       lpSrchChild->u.nInd.bHidden == FALSE )
                  {
                     break;
                  }

                  lpSrchChild = zGETPTR( lpSrchChild->hNextLinked );
               }

               // If the spawn has not been done for this structure match,
               // do it now.
               if ( lpSrchChild == 0 || lpSrchChild == lpTgtInstance )
               {
                  LPVIEWOI  lpViewOI;
                  LPVIEWCSR lpViewCsr;

                  lpViewOI    = zGETPTR( lpSrchInstance->hViewOI );
                  lpViewCsr   = zGETPTR( lpViewOI->hFirstViewCsr );

                  // Get a view to the entity instance which is the parent
                  // for the new spawn.
                  lpSpawnView = zGETPTR( lpViewCsr->hView );
                  CreateViewFromViewForTask( &lpSpawnView, lpSpawnView, 0 );
                  lpSpawnCsr = fnEstablishViewForInstance( lpSpawnView, 0,
                                                           lpSrchInstance );
                  if ( lpRecursViewEntity != lpSrchViewEntity )
                  {
                     fnSetViewToSubobject( lpSpawnView, lpRecursViewEntity, lpSpawnCsr );
                     while ( lpSpawnCsr->hViewEntity != zGETHNDL( lpSrchViewEntity ) )
                        lpSpawnCsr = zGETPTR( lpSpawnCsr->hParent );
                  }

                  while ( lpSpawnCsr->hViewEntity != zGETHNDL( lpWkViewEntity ) )
                     lpSpawnCsr = zGETPTR( lpSpawnCsr->hNextHier );

                  fnEstablishCursorForView( lpSpawnCsr );
                  nRC = fnIncludeSubobjectFromSubobject( lpSpawnView,
                                                         lpWkViewEntity,
                                                         lpTgtView,
                                                         lpTgtViewEntity,
                                                         zPOS_LAST, 0,
                                                         bMarkIncluded );
                  DropView( lpSpawnView );
               }
            }

            lpWkViewEntity = zGETPTR( lpWkViewEntity->hNextHier );
         }
      }

      // Now travel to next linked instance and try again
      lpSrchInstance = zGETPTR( lpSrchInstance->hNextLinked );
   }

   // We think we're done, return the results
   return( nRC );
}

//./ ADD NAME=fnRelinkVsnSubobjectToSubobject
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnRelinkVsnSubobjectToSubobject
//
//  PARAMETERS: lpTgtView       -  Target view for link
//              lpTgtEntityInstance Target instance for link
//              lpSrcView       -  Source view for link
//              lpSrcEntityInstance Source entity for link
//
//  RETURNS: 0           - Subobject linked successfully
//           zCALL_ERROR - Error relinking Subobject
//
//  DESCRIPTION: This operation attempts to relink a source subobject
//               to a target subobject and all its new versions. This
//               function requires that the source subobject and the target
//               subobject match exactly in instance structure and key
//               value, if a structural or key mismatch takes place, then
//               an error is issued and the relinking process is stopped.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnRelinkVsnSubobjectToSubobject( zVIEW            lpTgtView,
                                 LPENTITYINSTANCE lpTgtEntityInstance,
                                 zVIEW            lpSrcView,
                                 LPENTITYINSTANCE lpSrcEntityInstance,
                                 zSHORT           nInvertedParents,
                                 LPVIEWENTITY     lpInvertedSrcEntity )
{
   LPTASK           lpAllocTask;
   zSHORT           nStartLevel;
   zSHORT           nWkLevel;
   zSHORT           nWkInvertedParents;
   zBOOL            bProcessParentsNow;
   LPVIEWENTITY     lpTgtViewEntity;
   LPVIEWENTITY     lpSrcViewEntity;
   LPENTITYINSTANCE lpOrigSrcEntityInstance;
   LPENTITYINSTANCE lpOrigTgtEntityInstance;
   LPENTITYINSTANCE lpVsnEntityInstance;
   LPENTITYINSTANCE lpWrkEntityInstance;
   LPENTITYINSTANCE lpLnkEntityInstance;
   zPVOID           hOldRecord;
   zPVOID           hNewRecord;
   LPVIEWOI         lpTgtViewOI;

   SysMessageBox( lpSrcView, "QuinSoft Info", "Call DG (#102)", 0 );

   nStartLevel = lpTgtEntityInstance->nLevel;
   nWkInvertedParents = nInvertedParents;
   lpOrigSrcEntityInstance = lpSrcEntityInstance;
   lpOrigTgtEntityInstance = lpTgtEntityInstance;
   lpTgtViewEntity = zGETPTR( lpTgtEntityInstance->hViewEntity );
   lpSrcViewEntity = zGETPTR( lpSrcEntityInstance->hViewEntity );
   bProcessParentsNow = 0;
   lpTgtViewOI = zGETPTR( lpTgtEntityInstance->hViewOI );
   lpAllocTask = zGETPTR( lpTgtViewOI->hAllocTask );
   for ( ; ; )
   {
      while ( lpSrcEntityInstance->hPrevVsn )
         lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hPrevVsn );

      if ( lpSrcEntityInstance == 0 ||
           fnInstanceKeysMatch( lpSrcEntityInstance, lpTgtEntityInstance ) == 0 )
      {
         LPTASK lpTask = zGETPTR( lpTgtView->hTask );
         //  "KZOEE118 - Internal Key error relinking versioned Subobject"
         fnIssueCoreError( lpTask, lpSrcView, 16, 118, 0,
                           lpTgtViewEntity->szName,
                           lpSrcViewEntity->szName );
         return( -1 );
      }

      if ( fnInstanceLinkedToInstance( lpSrcEntityInstance,
                                       lpTgtEntityInstance ) == 0 )
      {
         hOldRecord = lpTgtEntityInstance->hPersistRecord;
         fnRemoveOtherOI_LinkedInstances( lpTgtEntityInstance,
                                          lpSrcEntityInstance );
         fnRelinkInstanceToInstance( lpTgtEntityInstance,
                                     lpSrcEntityInstance );
         hNewRecord = lpTgtEntityInstance->hPersistRecord;
         lpSrcEntityInstance->u.nInd.bPrevVersion = TRUE;
         lpWrkEntityInstance = lpTgtEntityInstance;
         do
         {
            lpVsnEntityInstance = zGETPTR( lpWrkEntityInstance->hNextVsn );

            // Create a new copy of the record for each later version
            // of the target instance.
            while ( lpVsnEntityInstance )
            {
               if ( lpVsnEntityInstance->hPersistRecord == hOldRecord )
                  lpVsnEntityInstance->hPersistRecord = hNewRecord;
               else
               if ( lpVsnEntityInstance->hPersistRecord != hNewRecord )
               {
                  fnDeleteAttributeRecord(
                        zGETPTR( lpVsnEntityInstance->hViewEntity ),
                        lpVsnEntityInstance, TRUE );
                  lpVsnEntityInstance->hPersistRecord =
                      fnCopyAttributeRecord( lpAllocTask,
                        zGETPTR( lpWrkEntityInstance->hViewEntity ),
                        (zPCHAR) zGETPTR( lpWrkEntityInstance->hPersistRecord ), 1 );
               }

               lpLnkEntityInstance =
                                 zGETPTR( lpVsnEntityInstance->hNextLinked );
               while ( lpLnkEntityInstance &&
                       lpLnkEntityInstance != lpVsnEntityInstance )
               {
                  lpLnkEntityInstance->hPersistRecord = lpVsnEntityInstance->hPersistRecord;
                  lpLnkEntityInstance =
                                 zGETPTR( lpLnkEntityInstance->hNextLinked );
               }

               lpVsnEntityInstance = zGETPTR( lpVsnEntityInstance->hNextVsn );
            }

            lpWrkEntityInstance = zGETPTR( lpWrkEntityInstance->hNextLinked );
         }  while ( lpWrkEntityInstance &&
                    lpWrkEntityInstance != lpTgtEntityInstance );
      }

      lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hNextHier );
      if ( lpTgtEntityInstance == 0 ||
           lpTgtEntityInstance->nLevel <= nStartLevel )
      {
         break;
      }
      else
      {
         while ( lpTgtEntityInstance->hPrevVsn )
            lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hPrevVsn );

         if ( lpTgtEntityInstance->u.nInd.bHidden )
         {
            nWkLevel = lpTgtEntityInstance->nLevel;
            do
            {
               lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hNextHier );
            }  while ( lpTgtEntityInstance &&
                       lpTgtEntityInstance->nLevel > nWkLevel );

            if ( lpTgtEntityInstance == 0 ||
                 lpTgtEntityInstance->nLevel <= nStartLevel )
            {
               break;
            }

            while ( lpTgtEntityInstance->hPrevVsn )
               lpTgtEntityInstance = zGETPTR( lpTgtEntityInstance->hPrevVsn );
         }
      }

      if ( nWkInvertedParents && bProcessParentsNow == FALSE )
      {
         LPVIEWENTITY     lpWk1, lpWk2;
         LPENTITYINSTANCE lpParent;

         // Determine if it is time to process the parents by checking the
         // Target Entity instance to see if the target instance is
         // an instance of the original sources parent
         lpWk1    = zGETPTR( lpTgtEntityInstance->hViewEntity );
         lpParent = zGETPTR( lpOrigSrcEntityInstance->hParent );
         lpWk2    = zGETPTR( lpParent->hViewEntity );
         if ( lpTgtEntityInstance->nLevel == ( nStartLevel + 1 ) &&
              lpWk1->lEREntTok == lpWk2->lEREntTok &&
              lpWk1->lERRelTok == lpSrcViewEntity->lERRelTok &&
              lpWk1->bRelLink1 != lpSrcViewEntity->bRelLink1 )
         {
            bProcessParentsNow = TRUE;
         }
      }

      if ( nWkInvertedParents && bProcessParentsNow )
      {
         lpSrcEntityInstance = zGETPTR( lpOrigSrcEntityInstance->hParent );
         nWkInvertedParents--;
      }
      else
      {
         for ( ; ; )
         {
            lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );
            if ( lpSrcEntityInstance == 0 )
               break;

            if ( lpSrcEntityInstance->u.nInd.bHidden )
            {
               nWkLevel = lpSrcEntityInstance->nLevel;
               do
               {
                  lpSrcEntityInstance =
                                 zGETPTR( lpSrcEntityInstance->hNextHier );
               }  while ( lpSrcEntityInstance &&
                          lpSrcEntityInstance->nLevel > nWkLevel );
            }
            else
            {
               LPVIEWENTITY lpWk1 =
                              zGETPTR( lpSrcEntityInstance->hViewEntity );
               LPVIEWENTITY lpWk2 =
                              zGETPTR( lpTgtEntityInstance->hViewEntity );

               if ( lpWk1->lEREntTok == lpWk2->lEREntTok )
               {
                  break;
               }
            }
         }
      }
   }

   if ( lpInvertedSrcEntity )
   {
      // If the source structure inverts the target then we need to find the
      // instance in the source and drop it.
      lpSrcEntityInstance = zGETPTR( lpOrigSrcEntityInstance->hNextHier );
      lpTgtEntityInstance = zGETPTR( lpOrigTgtEntityInstance->hParent );
      if ( lpTgtEntityInstance )
      {
         LPVIEWENTITY hInvertedSrcEntity = zGETHNDL( lpInvertedSrcEntity );

         while ( lpSrcEntityInstance &&
                 lpSrcEntityInstance->nLevel > lpOrigSrcEntityInstance->nLevel )
         {
            if ( lpSrcEntityInstance->hViewEntity == hInvertedSrcEntity &&
                 lpSrcEntityInstance->u.nInd.bHidden == FALSE &&
                 fnInstanceKeysMatch( lpTgtEntityInstance,
                                      lpSrcEntityInstance ) )
            {
               nInvertedParents =
                   fnValidSubobjectStructureMatch( lpSrcView,
                            lpInvertedSrcEntity, lpTgtView,
                            zGETPTR( lpTgtEntityInstance->hViewEntity ), 0 );
               fnRelinkVsnSubobjectToSubobject( lpSrcView,
                                                lpSrcEntityInstance,
                                                lpTgtView,
                                                lpTgtEntityInstance,
                                                nInvertedParents, 0 );
               break;
            }

            lpSrcEntityInstance = zGETPTR( lpSrcEntityInstance->hNextHier );
         }
      }
   }

   return( 0 );  // SUCCESS
}

//./ ADD NAME=SubobjectVersionUpdated
// Source Module=kzoesoaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SubobjectVersionUpdated
//
//  PURPOSE:    To determine if an Subobject Version has been updated.
//
//  PARAMETERS: lpView - View of object instance to check.
//              cpcEntityName - root entity of the subobject
//              bRootOnly -  if TRUE, check only root entity of subobject
//
//  RETURNS:    0           - Subobject has not been updated
//              1           - Subobject has been updated NON persistent
//              2           - Subobject has been updated persistent
//              zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SubobjectVersionUpdated( zVIEW   lpView,
                         zCPCHAR cpcEntityName,
                         zSHORT  bRootOnly )
{
   LPTASK           lpCurrentTask;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpEntityInstance;
   LPENTITYINSTANCE lpPrevVsn;
   zSHORT           nUpdated = 0;
   zSHORT           nUpdatedFile = 0;
   zSHORT           nRC = 0;

   // Validate parameters
   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntity,
                                      &lpViewEntityCsr,
                                      iSubobjectVersionUpdated,
                                      lpView,
                                      cpcEntityName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Get instance on which to start.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Go to the latest version.
   while ( lpEntityInstance->hNextVsn )
      lpEntityInstance = zGETPTR( lpEntityInstance->hNextVsn );

   // If the entity instance is not versioned, return unless it's not
   // created under a parent which IS versioned.
   lpPrevVsn = zGETPTR( lpEntityInstance->hPrevVsn );
   if ( lpEntityInstance->u.nInd.bTemporal == FALSE && lpPrevVsn == 0 )
   {
      // Entity instance is NOT versioned.
      LPENTITYINSTANCE lpParent = zGETPTR( lpEntityInstance->hParent );
      if ( lpEntityInstance->u.nInd.bCreated && lpParent &&
           (lpParent->hPrevVsn || lpParent->u.nInd.bTemporal) )
      {
         // This is the exception!  Do NOT return because the EI is
         // created under a versioned parent.
      }
      else
      {
         // Not versioned, and the exception does not apply ... return.
         fnOperationReturn( iSubobjectVersionUpdated, lpCurrentTask );
         return( 0 );
      }
   }

   if ( bRootOnly )
   {
      if ( lpEntityInstance->u.nInd.bVersionUpdFile )
         nUpdatedFile = 1;

      if ( lpEntityInstance->u.nInd.bVersionUpdated )
         nUpdated = 1;
   }
   else
   {
      fnGetResetUpdFlags( lpEntityInstance, &nUpdated, &nUpdatedFile, 0 );
   }


   if ( nUpdated )
      nRC = 2;
   else
   if ( nUpdatedFile )
      nRC = 1;

   fnOperationReturn( iSubobjectVersionUpdated, lpCurrentTask );
   return( nRC );
}

//  >>>fnGetResetUpdFlags
// Determine, whether the version update flags are set.
// Clear the version update flags if neccessary.
static zSHORT
fnGetResetUpdFlags( LPENTITYINSTANCE lpStartInstance,
                    zSHORT *pUpdated, zSHORT *pUpdatedFile,
                    zSHORT nDoReset )
{

   LPENTITYINSTANCE lpInstance = lpStartInstance;
   zSHORT nStartLevel = lpStartInstance->nLevel;

   do
   {
      if ( lpInstance->u.nInd.bVersionUpdFile )
         *pUpdatedFile = 1;

      if ( lpInstance->u.nInd.bVersionUpdated )
         *pUpdated = 1;

      if ( nDoReset )
      {
         lpInstance->u.nInd.bVersionUpdated = 0;
         lpInstance->u.nInd.bVersionUpdFile = 0;
      }

      lpInstance = zGETPTR( lpInstance->hNextHier );

   } while ( lpInstance && lpInstance->nLevel > nStartLevel );

   return( 0 );
}
