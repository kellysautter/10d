#include "kzoephdr.h"
//
// MODULE NAME:  KZOEVMAA  -  Object services View management Operations
// DESCRIPTION:  This is the source file which contains Object services
//               operations for creating and dropping views.
// OPERATIONS:   All OPERATIONS which manage views
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    4/20/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2001.10.11  DGC  10b
   Added code for GetViewFlags( ).

2001.07.24  DGC  10b
   Added check for view name lth.

2001.04.18  DGC  10b
   Added code for DropViewObject( ) logic.

2001.02.09  DGC  All
   Free cursor listener struct when dropping a view.

2000.10.27  DGC  All
   Made change initiated by Jeff.  On GetViewByName( ) at the task level, we
   were ignoring the subtask view being passed in.  Now we use it if it has
   been supplied.

2000.01.31  DGC  All
   Finally fixed problem with XWD activation.  Turns out we were incorrectly
   setting EntityCsr pointers in ResetViewFromSubobject( ) if we were more
   than two levels deep in a recursive subobject.

2000.01.18  DGC
   Small change...pop up a message sooner.

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

1999.06.11  DGC
   Removed references to fnValidRootInstance( ).

1999.02.26  DGC
   Made change to get rid of MSVC 6.0 compiler warning.

*/

#define __ACTIVATE_CONSTRAINTS__

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

zSHORT LOCALOPER
fnCheckIfViewOD_IsUsed( zVIEW lpView, LPVIEWOD lpViewOD );

zPCHAR LOCALOPER
fnEntityNameFromCsr( LPVIEWENTITYCSR lpViewEntityCsr );
zPCHAR LOCALOPER
fnEntityNameFromInstance( LPENTITYINSTANCE lpEntityInstance );

/*

   ViewEntityCsr's are stored in a table instead of it's own memory chunk.
   Because of this we have to manually create handles for ViewEntityCsr's
   using the ViewCsr pointer/handle.

   nIndex allows us to return a handle for a prev/next ViewEntityCsr.

   This code should be OS-independent because the logic should work just as
   well for handle's and pointers.

*/
LPVIEWENTITYCSR LOCALOPER
fnCreateEntityCsrHandle( LPVIEWENTITYCSR lpViewEntityCsr,
                         LPVIEWCSR       lpViewCsr,
                         zSHORT          nIndex )
{
   zLONG hndl;

   // If lpViewEntityCsr is 0 then the resulting handle is 0 too.
   if ( lpViewEntityCsr == 0 )
      return( 0 );

   // Get the handle for the ViewCsr pointer.
   hndl = (zLONG) zGETHNDL( lpViewCsr );

   // Set lpViewEntityCsr to a prev/next Csr (if nIndex is 0 nothing happens).
   lpViewEntityCsr = lpViewEntityCsr + nIndex;

   // The new handle is created by adding the difference between the 2
   // pointers (the offset) to the ViewCsr handle.
   hndl = hndl + ((zLONG) lpViewEntityCsr - (zLONG) lpViewCsr);

   return( (LPVIEWENTITYCSR) hndl );
}

//./ ADD NAME=SetViewReadonly
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetViewReadonly
//
//  PURPOSE: To mark a view as readonly. Any update requests issued for
//           data with the view will be denied.
//
//  RETURNS: 0           - View is now readonly
//           zCALL_ERROR - View passed is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SetViewReadOnly( zVIEW  lpView )
{
   LPTASK   lpCurrentTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetViewReadOnly, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // If we're here, success.
   lpView->bReadOnly = TRUE;

   fnOperationReturn( iSetViewReadOnly, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=SetViewUpdate
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetViewUpdate
//
//  PURPOSE: To mark a view as not readonly.
//
//  RETURNS: 0           - View is now updateable.
//           zCALL_ERROR - View passed is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SetViewUpdate( zVIEW  lpView )
{
   LPTASK   lpCurrentTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetViewReadOnly, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // If we're here, success
   lpView->bReadOnly = 0;

   fnOperationReturn( iSetViewReadOnly, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=SetViewFlags
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetViewFlags
//
//  PURPOSE: To set view flags.
//
//       Current flags:
//          zVF_MESSAGEONDROP - Pops up a message when the view is about to
//                              be dropped.
//          zVF_TRACEONDROP   - Traces a message when the view is about to
//                              be dropped.
//
//  RETURNS: 0           - OK
//           zCALL_ERROR - View passed is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SetViewFlags( zVIEW lpView, zLONG lControl )
{
   LPTASK   lpCurrentTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetViewFlags, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( lControl & zVF_MESSAGEONDROP )
      lpView->bFlagDropView = TRUE;
   else
      lpView->bFlagDropView = FALSE;

   if ( lControl & zVF_TRACEONDROP )
      lpView->bTraceDropView = TRUE;
   else
      lpView->bTraceDropView = FALSE;

   fnOperationReturn( iSetViewFlags, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=GetViewFlags
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   GetViewFlags
//
//  PURPOSE: To get view flags.
//       Current flags:
//          zVF_MESSAGEONDROP - Pops up a message when the view is about to
//                              be dropped.
//
//  RETURNS: 0           - OK
//           zCALL_ERROR - View passed is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG OPERATION
GetViewFlags( zVIEW  lpView )
{
   LPTASK      lpCurrentTask;
   zLONG       lReturn;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iGetViewFlags, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( lpView->bFlagDropView )
      lReturn |= zVF_MESSAGEONDROP;

   fnOperationReturn( iGetViewFlags, lpCurrentTask );
   return( 0 );
}

//#define DEBUG_VIEW
#ifdef DEBUG_VIEW
#define zMAX_VIEW_TEST  64000
zPVOID *g_pvView[ zMAX_VIEW_TEST + 1 ];
zLONG  g_lHighWaterView = -1;
#endif

zSHORT OPERATION
CreateViewTrace( zCPCHAR cpcMsg,
                 zPVIEW  pvReturnView,
                 zVIEW   vSrc,
                 zVIEW   vTask )
{
   LPVIEWOD lpViewOD = zGETPTR( vSrc->hViewOD );
   zSHORT   nRC;

   nRC = CreateViewFromViewForTask( pvReturnView, vSrc, vTask );
   TraceLine( "CreateView %s: 0x%08x   LOD: %s", cpcMsg, *pvReturnView, lpViewOD->szName );
   SetViewFlags( *pvReturnView, zVF_TRACEONDROP );
   return( nRC );
}

zSHORT OPERATION
CreateViewFromView( zPVIEW pvReturnView,
                    zVIEW  vSrc )
{
   return( CreateViewFromViewForTask( pvReturnView, vSrc, 0 ) );
}

//./ ADD NAME=CreateViewFromViewForTask
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   CreateViewFromViewForTask
//
//  PURPOSE: Returns the new ViewID in lpReturn View, the new view's
//           cursor positions are set on the exact same entity
//           instances as the SrcView's.
//
//  RETURNS: 0           - View successfully declared and returned
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
CreateViewFromViewForTask( zPVIEW pvReturnView,
                           zVIEW  vSrc,
                           zVIEW  vTask )
{
   LPTASK      lpCurrentTask;
   LPTASK      lpTask;
   zVIEW       vTemp;
   zSHORT      nRC;

#ifdef DEBUG_VIEW
   zLONG  k;

   if ( pvReturnView == 0 || g_lHighWaterView == -1 )
   {
      TraceLineS( "CreateViewFromView Initializing View Usage ===============================> ", "" );
      DropView( 0 );
      for ( k = 0; k < zMAX_VIEW_TEST; k++ )
         g_pvView[ k ] = 0;

      g_lHighWaterView = 0;
      if ( pvReturnView == 0 )
         return( 0 );
   }

   for ( k = 0; k < zMAX_VIEW_TEST; k++ )
   {
      if ( g_pvView[ k ] == 0 )
         break;
   }

#endif

   *pvReturnView = 0;  // init in case of error

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCreateViewFromView, vSrc, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpTask = lpCurrentTask;

   if ( vSrc->bApplicationView )  // dks 2005.11.29
      ;  // nothing to do
   else
   if ( vTask )
      lpTask = zGETPTR( vTask->hTask );

   if ( (nRC = fnDeclareView( &vTemp, lpTask, vSrc, 0, 0 )) != 0 )
   {
      fnOperationReturn( iCreateViewFromView, lpCurrentTask );
      return( nRC );
   }

   if ( vSrc->bApplicationView && (nRC = SfTransferView( vTemp, vSrc, zLEVEL_APPLICATION )) != 0 )
   {
      fnOperationReturn( iCreateViewFromView, lpCurrentTask );
      return( nRC );
   }

   if ( nRC == 0 )
   {
#ifdef DEBUG_VIEW

      LPVIEWOD lpViewOD = zGETPTR( vTemp->hViewOD );

      if ( zstrcmpi( lpViewOD->szName, "TZ__PRFO" ) == 0 )
      {
         TraceLine( "CreateViewFromView: 0x%08x   LOD: %s", vTemp, lpViewOD->szName );
         if ( k >= zMAX_VIEW_TEST )
            TraceLineS( "CreateView Error: ", "Memory Test Error" );
         else
         {
            if ( g_lHighWaterView < k )
               g_lHighWaterView = k;

            g_pvView[ k ] = (zPVOID) *pvReturnView;
         }
      }
#endif

      nRC = fnSetViewFromView( vTemp, vSrc );
      *pvReturnView = vTemp;
   }

// {
//    LPVIEWOD lpViewOD = zGETPTR( (*pvReturnView)->hViewOD );
//    if ( zstrcmp( lpViewOD->szName, "TZRPSRCO" ) == 0 )
//    {
//       TraceLine( "CreateView: 0x%08x   LOD: %s", *pvReturnView, lpViewOD->szName );
//    }
// }

   fnOperationReturn( iCreateViewFromView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfCreateSysViewFromView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SfCreateSysViewFromView
//
//  PURPOSE: Returns the new ViewID in lpReturn View, the new view's
//           cursor positions are set on the exact same entity
//           instances as the SrcView's.
//
//           The new view is a system view.
//
//  RETURNS: 0           - View successfully declared and returned
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfCreateSysViewFromView( zPVIEW pvReturnView, zVIEW vSrc )
{
   LPTASK      lpCurrentTask;
   zSHORT      nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfCreateSysViewFromView, vSrc,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Declare the new view as a system view.
   if ( (nRC = fnDeclareView( pvReturnView, lpCurrentTask,
                              vSrc, 0, 1 )) != 0 )
   {
      fnOperationReturn( iSfCreateSysViewFromView, lpCurrentTask );
      return( nRC );
   }

   if ( vSrc->bApplicationView &&
        (nRC = SfTransferView( *pvReturnView, vSrc, zLEVEL_APPLICATION )) != 0 )
   {
      fnOperationReturn( iSfCreateSysViewFromView, lpCurrentTask );
      return( nRC );
   }

   if ( nRC == 0 )
   {
      nRC = fnSetViewFromView( *pvReturnView, vSrc );
      TraceLine( "SfCreateSysViewFromView: 0x%08x", *pvReturnView );
   }

   fnOperationReturn( iSfCreateSysViewFromView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnDeclareView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnDeclareView
//
//  PURPOSE: To create a view for a specified object type. The View
//           passed as the second parameter is used to qualify the
//           application in which to look for the object. If the
//           view passed is a subtask view, then the application used
//           is the application tied to the subtask. Otherwise, the
//           application for the view object associated with the
//           passed view is used.
//
//  We don't need to set a mutex here to protect the App from being dropped
//  before we can use it.  The App won't get dropped by another task because:
//    (1) lpView is a subtask view that references the App;   OR
//    (2) lpView represents an OI with an OD that is part of the App.
//  fnDeleteApp won't drop an App if one of the above is true.
//
//  RETURNS: 0           - View successfully declared and returned
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
fnDeclareView( zPVIEW pvReturnView, LPTASK lpCurrentTask, zVIEW lpView,
               zCPCHAR cpcViewOD_Name, zBOOL bSystemObject )
{
   LPTASK   hCurrentTask = zGETHNDL( lpCurrentTask );
   LPVIEWOD lpViewOD;
   zVIEW    lpNewView;
   zPVOID   hNewView;
   zSHORT   nRC;

   lpNewView = 0;
   if ( cpcViewOD_Name )
      lpViewOD = ActivateViewObject( lpView, cpcViewOD_Name, bSystemObject );
   else
   if ( lpView == (zVIEW) -1 )  // creating a dummy view
      lpViewOD = (LPVIEWOD) -1;
   else
      lpViewOD = zGETPTR( lpView->hViewOD );

   nRC = 0;
   if ( lpViewOD )
   {
   // if ( lpCurrentTask == zGETPTR( AnchorBlock->hMainTask ) )
   // if ( lpViewOD != (LPVIEWOD) -1 && zstrcmp( lpViewOD->szName, "DOMAINT" ) == 0 )
   //    nRC /= nRC;

      // Allocate a View Record.
      hNewView = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                   sizeof( ViewRecord ), 1, 0, iView );
   // if ( hNewView == (zVIEW) 0x8310e4ca )
   //    nRC /= nRC;

      lpNewView = zGETPTR( hNewView );
      if ( lpNewView )
      {
      // LPAPP lpApp;

         lpNewView->hTask = hCurrentTask;
         if ( lpViewOD == (LPVIEWOD) -1 )
            lpNewView->hViewOD = 0;
         else
            lpNewView->hViewOD = zGETHNDL( lpViewOD );

// can't|if ( lpView && lpView != (zVIEW) -1 && lpView->bApplicationView &&
// do   |     fnGetApplicationForSubtask( &lpApp, lpView ) == 0 )
// this |{
// here!|   lpNewView->bApplicationView = TRUE; // indicate that scope is app
// must |   lpNewView->hNextView = lpApp->hFirstView; // add view to app
// do   |   lpApp->hFirstView = hNewView;
// at   |}
// caller|else
         {
            if ( hCurrentTask == AnchorBlock->hMainTask )
               zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

            lpNewView->hNextView = lpCurrentTask->hFirstView;
            lpCurrentTask->hFirstView = hNewView;

            if ( hCurrentTask == AnchorBlock->hMainTask )
               zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
         }
      }
      else
         nRC = zCALL_ERROR;
   }
   else
      nRC = zCALL_ERROR;

   // Set return view.
   *pvReturnView = lpNewView;
   return( nRC );
}

//./ ADD NAME=SetViewFromView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetViewFromView
//
//  PURPOSE: To update an existing view with the cursor positions
//           in another view. If the target view has cursors, then
//           the cursors are dropped and a new set of cursors are
//           created based on the subobject view level and entity
//           instance positions of the source view. Note, the new
//           cursor positions in the target view are independent
//           of the positions in the source view after initialization
//           from the source view.
//
//  RETURNS: 0           - View successfully updated
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SetViewFromView( zVIEW  vTgt, zVIEW  vSrc )
{
   LPTASK      lpCurrentTask;
   zSHORT      nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetViewFromView, vSrc,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( fnValidView( lpCurrentTask, vTgt ) == 0 )
   {
      fnOperationReturn( iSetViewFromView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Call fnSetViewFromView.
   nRC = fnSetViewFromView( vTgt, vSrc );

   // Success, return the good news.
   fnOperationReturn( iSetViewFromView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetViewFromView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnSetViewFromView
//
//
//  RETURNS: 0           - View successfully updated
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnSetViewFromView( zVIEW  vTgt, zVIEW  vSrc )
{
   LPTASK            lpOwningTask;
   LPVIEWOD          lpSrcViewOD;
   LPVIEWOD          lpTgtViewOD;
   LPVIEWOI          lpSrcViewOI;
   LPVIEWOI          lpTgtViewOI;
   LPVIEWCSR         lpTgtViewCsr;
   LPVIEWCSR         lpSrcViewCsr;
   LPVIEWCSR         lpNewViewCsr;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpNewViewEntityCsr;
   LPVIEWSUBOBJECT   lpSrcViewSubobject;
   LPVIEWSUBOBJECT   lpNewViewSubobject;
   LPVIEWSUBOBJECT   lpPrevNewViewSubobject;
   zPVOID            hNewViewCsr;
   zULONG            ulViewSize;
   zLONG             lEntities;

   // We use LPVIEWENTITYCSR instead of zPVOID to declare the following
   // handles so that we can perform pointer arithmetic on it.
   LPVIEWENTITYCSR   hSrcViewEntityCsr;
   LPVIEWENTITYCSR   hNewViewEntityCsr;

   // If the ViewCsr for the source and target are the same, then
   // there is nothing to do, return a 0.
   if ( vSrc->hViewCsr == vTgt->hViewCsr )
      return( 0 );

   lpSrcViewOD = zGETPTR( vSrc->hViewOD );
   lpTgtViewOD = zGETPTR( vTgt->hViewOD );

   // Now make sure the source view object definition and the target
   // view object definition are the same object definition. If not,
   // we're in deep trouble so issue an error and get out pronto.
   if ( lpSrcViewOD == 0 || lpSrcViewOD != lpSrcViewOD )
   {
      LPTASK lpTask = zGETPTR( vTgt->hTask );
      zPCHAR  szName1;
      zPCHAR  szName2;

      szName1 = 0;
      if ( lpTgtViewOD )
         szName1 = lpTgtViewOD->szName;

      szName2 = 0;
      if ( lpSrcViewOD )
         szName2 = lpSrcViewOD->szName;

      // "KZOEE274 - Target and Source views are different View Object types"
      fnIssueCoreError( lpTask, vSrc, 8, 274, 0, szName1, szName2 );
      return( zCALL_ERROR );
   }

   lpSrcViewCsr   = zGETPTR( vSrc->hViewCsr );
   lpSrcViewOI    = zGETPTR( lpSrcViewCsr->hViewOI );
   lpOwningTask   = zGETPTR( lpSrcViewOI->hTask );
   lpTgtViewCsr   = zGETPTR( vTgt->hViewCsr );

   if ( lpTgtViewCsr )
      lpTgtViewOI = zGETPTR( lpTgtViewCsr->hViewOI );
   else
      lpTgtViewOI = 0;

   if ( lpSrcViewOI == lpTgtViewOI )
   {
      lpNewViewCsr = lpTgtViewCsr;
      hNewViewCsr  = zGETHNDL( lpNewViewCsr );
      fnResetView( vTgt, 0 );   // Reset the target View
      lpTgtViewCsr->hHierRootEntityInstance = lpSrcViewCsr->hHierRootEntityInstance;
      lpTgtViewCsr->hHierEntityInstance = lpSrcViewCsr->hHierEntityInstance;
      lpTgtViewCsr->hViewParentEntityInstance = lpSrcViewCsr->hViewParentEntityInstance;
      lEntities = lpTgtViewCsr->uEntities;
      hSrcViewEntityCsr  = lpSrcViewCsr->hFirstOD_EntityCsr;
      lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hFirstOD_EntityCsr );
      hNewViewEntityCsr  = lpNewViewCsr->hFirstOD_EntityCsr;
      lpNewViewEntityCsr = zGETPTR( lpNewViewCsr->hFirstOD_EntityCsr );
      if ( lpSrcViewCsr->hFirstViewSubobject == 0 )
      {
         while ( lEntities )
         {
            lpNewViewEntityCsr->hEntityInstance = lpSrcViewEntityCsr->hEntityInstance;
         // if ( lpNewViewEntityCsr->hEntityInstance == UNSET_CSR )
         //    SysMessageBox( 0, "fnSetViewFromView", "UNSET_CSR", -1 );  // happens too much

            hSrcViewEntityCsr++;
            lpSrcViewEntityCsr++;
            hNewViewEntityCsr++;
            lpNewViewEntityCsr++;

            lEntities--;
         }
      }
      else
      {
         // Set root view entity csr.
         while ( hSrcViewEntityCsr != lpSrcViewCsr->hRootViewEntityCsr )
         {
            hSrcViewEntityCsr++;
            lpSrcViewEntityCsr++;
            hNewViewEntityCsr++;
            lpNewViewEntityCsr++;
         }

         lpNewViewCsr->hRootViewEntityCsr = hNewViewEntityCsr;

         // Reset first cursor in case root view csr is not the first csr.
         hSrcViewEntityCsr  = lpSrcViewCsr->hFirstOD_EntityCsr;
         lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hFirstOD_EntityCsr );
         hNewViewEntityCsr  = lpNewViewCsr->hFirstOD_EntityCsr;
         lpNewViewEntityCsr = zGETPTR( lpNewViewCsr->hFirstOD_EntityCsr );

         while ( lEntities )
         {
            lpNewViewEntityCsr->hEntityInstance = lpSrcViewEntityCsr->hEntityInstance;
         // if ( lpNewViewEntityCsr->hEntityInstance == UNSET_CSR )
         //    SysMessageBox( 0, "fnSetViewFromView2", "UNSET_CSR", -1 );  // happens too much

            lpNewViewEntityCsr->hViewEntity = lpSrcViewEntityCsr->hViewEntity;
            lpNewViewEntityCsr->nLevel = lpSrcViewEntityCsr->nLevel;
            if ( lpSrcViewEntityCsr->hNextHier == 0 )
               lpNewViewEntityCsr->hNextHier = 0;

            if ( lpSrcViewEntityCsr->hPrevHier == 0 )
               lpNewViewEntityCsr->hPrevHier = 0;

            if ( lpSrcViewEntityCsr->hParent == 0 )
               lpNewViewEntityCsr->hParent = 0;

            hSrcViewEntityCsr++;
            lpSrcViewEntityCsr++;
            hNewViewEntityCsr++;
            lpNewViewEntityCsr++;

            lEntities--;
         }
      }
   }
   else
   {
      // If the source and target ViewOI are not equal, we need to allocate
      // a new ViewCsr and ViewEntityCsrs so we can drop the previous
      // ViewCsr and appropriate cleanup will take place on the ViewOI
      // previously referenced by the SrcView.
      // Allocate space for View.
      lEntities = lpSrcViewCsr->uEntities;
      ulViewSize = sizeof( ViewCsrRecord ) + (lEntities * sizeof( ViewEntityCsrRecord ));

      // Create a ViewOI Record.
      hNewViewCsr = fnAllocDataspace( lpOwningTask->hFirstDataHeader,
                                      ulViewSize, 0, 0, iViewCsr );
      lpNewViewCsr = zGETPTR( hNewViewCsr );
      if ( lpNewViewCsr == 0 )
         return( zCALL_ERROR );

      // Copy the information from the old view to the new view.
      zmemcpy( lpNewViewCsr, lpSrcViewCsr, ulViewSize );

      // Initialize ViewEntityCsr records.
      // Now go through and set the entity pointers.
      hSrcViewEntityCsr  = lpSrcViewCsr->hFirstOD_EntityCsr;
      lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hFirstOD_EntityCsr );
      hNewViewEntityCsr  = (zPVOID) ((zLONG) hNewViewCsr + sizeof( ViewCsrRecord ));
      lpNewViewEntityCsr = (LPVIEWENTITYCSR) zGETPTR( hNewViewEntityCsr );
      lpNewViewCsr->hFirstOD_EntityCsr = hNewViewEntityCsr;

      // Set root view entity csr.
      while ( hSrcViewEntityCsr != lpSrcViewCsr->hRootViewEntityCsr )
      {
         hSrcViewEntityCsr++;
         lpSrcViewEntityCsr++;
         hNewViewEntityCsr++;
         lpNewViewEntityCsr++;
      }

      lpNewViewCsr->hRootViewEntityCsr  = hNewViewEntityCsr;
      lpNewViewCsr->hHierRootEntityInstance = 0;
      lpNewViewCsr->hHierEntityInstance = 0;
      lpNewViewCsr->hFirstSelectedInstance = 0;

      // Reset first cursor in case root view csr is not the first csr.
      hSrcViewEntityCsr  = lpSrcViewCsr->hFirstOD_EntityCsr;
      lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hFirstOD_EntityCsr );
      hNewViewEntityCsr  = lpNewViewCsr->hFirstOD_EntityCsr;
      lpNewViewEntityCsr = zGETPTR( lpNewViewCsr->hFirstOD_EntityCsr );

      // Note: we use lEntities when initializing the ViewEntityCsr records
      // because hierarchical pointers may have been zeroed by a call.
      // to SetViewToSubobject.
      while ( lEntities )
      {
         lpNewViewEntityCsr->hViewCsr = hNewViewCsr;

         if ( lpSrcViewEntityCsr->hNextHier )
            lpNewViewEntityCsr->hNextHier = hNewViewEntityCsr + 1;

         // If it's not the first ViewEntityCsr in the chain.
         if ( hSrcViewEntityCsr != lpSrcViewCsr->hFirstOD_EntityCsr )
         {
            LPVIEWENTITYCSR lpParentViewEntityCsr;
            LPVIEWENTITYCSR hParentViewEntityCsr;

            // When establishing a parent cursor in the new view, we use the
            // hierarchical number from the previous view instead of levels
            // because levels may have been mucked with by calls to
            // SetViewToSubobject which can cause erroneous parent setting.
            if ( lpSrcViewEntityCsr->hPrevHier )
               lpNewViewEntityCsr->hPrevHier = hNewViewEntityCsr - 1;

            if ( lpSrcViewEntityCsr->hParent )
            {
               zSHORT          nParentHierNbr;
               LPVIEWENTITYCSR lpTempParent;

               hParentViewEntityCsr  = hNewViewEntityCsr - 1;
               lpParentViewEntityCsr = lpNewViewEntityCsr - 1;

               lpTempParent = zGETPTR( lpSrcViewEntityCsr->hParent );
               nParentHierNbr = lpTempParent->nHierNbr;
               while ( lpParentViewEntityCsr->nHierNbr != nParentHierNbr )
               {
                  hParentViewEntityCsr--;
                  lpParentViewEntityCsr--;
               }

               lpNewViewEntityCsr->hParent = hParentViewEntityCsr;
            // if ( lpNewViewEntityCsr->hParent == (LPVIEWENTITYCSR) 1 )
            //    SysMessageBox( 0, "fnSetViewFromView", "UNSET_CSR", -1 );
            }
         }

         hSrcViewEntityCsr++;
         lpSrcViewEntityCsr++;
         hNewViewEntityCsr++;
         lpNewViewEntityCsr++;

         lEntities--;
      }
   }

   // Now that the new view has been created, copy all of the view
   // subobject records.
   lpSrcViewSubobject = zGETPTR( lpSrcViewCsr->hFirstViewSubobject );
   lpNewViewCsr->hFirstViewSubobject = 0;
   lpPrevNewViewSubobject = 0;
   while ( lpSrcViewSubobject )
   {
      zPVOID hNewViewSubobject;

      // Create a viewsubobject record and chain it to the ViewCsr.
      hNewViewSubobject = fnAllocDataspace( lpOwningTask->hFirstDataHeader,
                                            sizeof( ViewSubobjectRecord ), 1, 0,
                                            iViewSubobject );
      lpNewViewSubobject = zGETPTR( hNewViewSubobject );
      if ( lpNewViewSubobject == 0 )
      {
         if ( lpNewViewCsr == lpTgtViewCsr )
         {
            fnResetView( vTgt, 0 );   // reset the target view
         }
         else
         {
            while ( lpNewViewCsr->hFirstViewSubobject )
            {
               lpNewViewSubobject = zGETPTR( lpNewViewCsr->hFirstViewSubobject );
               lpNewViewCsr->hFirstViewSubobject = lpNewViewSubobject->hNextViewSubobject;
               fnFreeDataspace( lpNewViewSubobject );
            }

            fnFreeDataspace( lpNewViewCsr );
            return( zCALL_ERROR );
         }
      }

      zmemcpy( (zPCHAR) lpNewViewSubobject, (zPCHAR) lpSrcViewSubobject,
               sizeof( ViewSubobjectRecord ) );

      // Set appropriate view entity cursors in new subobject
      // First, set lpViewParent csr. NOTE: the view parent
      // Cursor may be NULL in the case of recursion when the
      // recursive parent entity type is to root of the
      // view object.
      if ( lpSrcViewSubobject->hViewRootParentCsr )
      {
         hSrcViewEntityCsr  = lpSrcViewCsr->hFirstOD_EntityCsr;
         lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hFirstOD_EntityCsr );
         hNewViewEntityCsr  = lpNewViewCsr->hFirstOD_EntityCsr;
         lpNewViewEntityCsr = zGETPTR( lpNewViewCsr->hFirstOD_EntityCsr );
         while ( hSrcViewEntityCsr != lpSrcViewSubobject->hViewRootParentCsr )
         {
            hSrcViewEntityCsr++;
            lpSrcViewEntityCsr++;
            hNewViewEntityCsr++;
            lpNewViewEntityCsr++;
         }

         lpNewViewSubobject->hViewRootParentCsr = hNewViewEntityCsr;
      }

      // Next set lpRootCsr
      hSrcViewEntityCsr  = lpSrcViewCsr->hFirstOD_EntityCsr;
      lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hFirstOD_EntityCsr );
      hNewViewEntityCsr  = lpNewViewCsr->hFirstOD_EntityCsr;
      lpNewViewEntityCsr = zGETPTR( lpNewViewCsr->hFirstOD_EntityCsr );
      while ( hSrcViewEntityCsr != lpSrcViewSubobject->hRootCsr )
      {
         hSrcViewEntityCsr++;
         lpSrcViewEntityCsr++;
         hNewViewEntityCsr++;
         lpNewViewEntityCsr++;
      }

      lpNewViewSubobject->hRootCsr = hNewViewEntityCsr;

      // Next, set lpLastDescCsr, note we do not reset lpSrcViewEntityCsr
      // and lpNewViewEntityCsr because the last descendent must occur
      // on the same or a subsequent cursor to the root cursor
      while ( hSrcViewEntityCsr != lpSrcViewSubobject->hLastDescCsr )
      {
         hSrcViewEntityCsr++;
         lpSrcViewEntityCsr++;
         hNewViewEntityCsr++;
         lpNewViewEntityCsr++;
      }

      lpNewViewSubobject->hLastDescCsr = hNewViewEntityCsr;

      // Next, set RecursViewEntityCsr if set in old View Subobject.
      if ( lpSrcViewSubobject->hRecursViewEntityCsr )
      {
         hSrcViewEntityCsr  = lpSrcViewCsr->hFirstOD_EntityCsr;
         lpSrcViewEntityCsr = zGETPTR( lpSrcViewCsr->hFirstOD_EntityCsr );
         hNewViewEntityCsr  = lpNewViewCsr->hFirstOD_EntityCsr;
         lpNewViewEntityCsr = zGETPTR( lpNewViewCsr->hFirstOD_EntityCsr );
         while ( hSrcViewEntityCsr != lpSrcViewSubobject->hRecursViewEntityCsr )
         {
            hSrcViewEntityCsr++;
            lpSrcViewEntityCsr++;
            hNewViewEntityCsr++;
            lpNewViewEntityCsr++;
         }

         lpNewViewSubobject->hRecursViewEntityCsr = hNewViewEntityCsr;
      }

      if ( lpPrevNewViewSubobject )
      {
         lpPrevNewViewSubobject->hNextViewSubobject = hNewViewSubobject;
         lpNewViewSubobject->hPrevViewSubobject =
                                          zGETHNDL( lpPrevNewViewSubobject );
      }
      else
         lpNewViewCsr->hFirstViewSubobject = hNewViewSubobject;

      lpNewViewSubobject->hNextViewSubobject = 0;
      lpPrevNewViewSubobject = lpNewViewSubobject;
      lpSrcViewSubobject = zGETPTR( lpSrcViewSubobject->hNextViewSubobject );
   }

   // 'copy' the selected instance chain.
   if ( lpSrcViewCsr->hFirstSelectedInstance )
   {
      LPSELECTEDINSTANCE   lpSrcSelectedInstance;

      lpSrcSelectedInstance = zGETPTR( lpSrcViewCsr->hFirstSelectedInstance );
      while ( lpSrcSelectedInstance )
      {
         zLONG    lEntities;

         lpNewViewEntityCsr = zGETPTR( lpNewViewCsr->hFirstOD_EntityCsr );
         hNewViewEntityCsr  = lpNewViewCsr->hFirstOD_EntityCsr;
         lEntities = lpNewViewCsr->uEntities;
         do
         {
            if ( lpNewViewEntityCsr->hViewEntity == lpSrcSelectedInstance->hViewEntity )
            {
               break;
            }

            lpNewViewEntityCsr++;
            hNewViewEntityCsr++;
            lEntities--;

         } while ( lEntities > 0 );

         // Set the current select set to the summ of the selections for the
         // select instance.
         lpNewViewCsr->nCurrentSelectSet = lpSrcSelectedInstance->nSelectSet;
         fnSelectedInstanceFarbler( zGETPTR( lpNewViewEntityCsr->hViewCsr ),
                                    zGETPTR( lpSrcSelectedInstance->hViewEntity ),
                                    lpSrcSelectedInstance->xEntityInstance, 2 );
         lpSrcSelectedInstance = zGETPTR( lpSrcSelectedInstance->hNextSelectedInstance );
      }

      // After the selections in this view, reset the current select set
      // for the new view to match the source view.
      lpNewViewCsr->nCurrentSelectSet = lpSrcViewCsr->nCurrentSelectSet;
   }

   // If we created a brand new ViewCsr, clear up the old view and
   // chain the new ViewCsr to the ViewOI.
   if ( lpNewViewCsr != lpTgtViewCsr )
   {
      // Chain ViewCsr to ViewOI.
      lpNewViewCsr->hNextViewCsr = lpSrcViewOI->hFirstViewCsr;
      lpSrcViewOI->hFirstViewCsr = hNewViewCsr;
      if ( lpTgtViewCsr )
         fnDropViewCsr( zGETPTR( vTgt->hViewCsr ) );

      vTgt->hViewCsr = hNewViewCsr;
      lpNewViewCsr->hView = zGETHNDL( vTgt );
   }

   // Finally, copy read only bit from source to target.
   vTgt->bReadOnly = vSrc->bReadOnly;

   // We should be calling listeners to inform them that the cursor has been
   // set, but we don't know the (if any) entity name ... dks 2007.05.24
// fnCallListeners( lpView, ??????Entity->szName );

   return( 0 ); // success, return the good news
}

//./ ADD NAME=DropNameForView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   DropNameForView
//
//  PURPOSE: To drop a name given a view at the level requested
//           (see SetNameForView for level values). Note, the
//           view parameter is optional and will be verified with the
//           name if passed. Otherwise, the name will be dropped
//           regardless of the view it points to.
//
//  RETURNS: 0           - Name for view successfully dropped
//           1           - Name for view not found
//           zCALL_ERROR - error naming view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
DropNameForView( zVIEW        lpView,
                 zCPCHAR      cpcName,
                 zVIEW        lpSubtaskView,
                 zSHORT       nLevel )
{
   LPTASK      lpCurrentTask;
   LPSUBTASK   lpSubtask;
   LPVIEWNAME  lpViewName;
   LPVIEWNAME  lpPrevViewName;
   LPAPP       lpApp;
   zPVOID      hView = zGETHNDL( lpView );
   zSHORT      nRC = zCALL_ERROR;

#if 1
   if ( lpSubtaskView == 0 )
   {
      if ( g_vDefaultTaskView )
         lpSubtaskView = g_vDefaultTaskView;
   }
   else
   if ( g_vDefaultTaskView && nLevel == zLEVEL_TASK )
   {
      if ( lpSubtaskView->hTask != g_vDefaultTaskView->hTask )
      {
         lpCurrentTask = zGETPTR( lpSubtaskView->hTask );
         if ( lpCurrentTask && lpCurrentTask->bTransientTask )
         {
            TraceLine( "DropNameForView sending in view for TRANSIENT task: %d (0x%08x)  "
                         "should be for task: %d (0x%08x) ... WILL NOT RESET TASK VIEW",
                       lpSubtaskView->hTask, lpSubtaskView->hTask,
                       g_vDefaultTaskView->hTask, g_vDefaultTaskView->hTask );
         }
         else
         {
         // SysMessageBox( 0, "Zeidon OE", "DropNameForView now RESETTING TASK VIEW", 0 );
            if ( g_nOE_TraceWarning )
            {
               TraceLine( "DropNameForView sending in view for task: %d (0x%08x)  "
                            "should be for task: %d (0x%08x) ... RESETTING TASK VIEW",
                          lpSubtaskView->hTask, lpSubtaskView->hTask,
                          g_vDefaultTaskView->hTask, g_vDefaultTaskView->hTask );
            }

            lpSubtaskView = g_vDefaultTaskView;
         }
      }
   }
#endif

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iDropNameForView, lpView, zVALID_VIEW )) == 0 )
      return( zCALL_ERROR );

   do  // begin purist's goto
   {
      // Check for valid scope.
      if ( nLevel != zLEVEL_SUBTASK     && nLevel != zLEVEL_TASK   &&
           nLevel != zLEVEL_APPLICATION && nLevel != zLEVEL_SYSTEM &&
           nLevel != zLEVEL_ANY )
      {
         TraceLineS( "DropNameForView Invalid scope for View name: ", cpcName );
         //  "KZOEE110 - Invalid scope for View name"
         fnIssueCoreError( lpCurrentTask, lpSubtaskView, 8, 110, (zLONG) nLevel, 0, 0 );
      // fnOperationReturn( iDropNameForView, lpCurrentTask );
         break;
      }

      if ( cpcName == 0 || cpcName[ 0 ] == 0 ||
           zstrlen( cpcName ) > zZEIDON_NAME_LTH * 2 )
      {
         TraceLineS( "DropNameForView Length Error: ", cpcName );
         fnIssueCoreError( lpCurrentTask, lpSubtaskView, 8, 450, 0, cpcName, 0 );
      // fnOperationReturn( iDropNameForView, lpCurrentTask );
         break;
      }

   // if ( fnValidView( lpCurrentTask, lpView ) == 0 )
   // {
   //    fnOperationReturn( iDropNameForView, lpCurrentTask );
   //    return( zCALL_ERROR );
   // }

      // If user wants a subtask view make sure subtask is OK.
      if ( nLevel == zLEVEL_SUBTASK &&
           fnValidSubtaskView( lpCurrentTask, lpSubtaskView ) == 0 )
      {
         TraceLineS( "DropNameForView Invalid Subtask view for Name: ", cpcName );
         break;  // view invalid
      }

      // Depending on the scope parameter, look in the appropriate
      // name chain for the name being dropped.
      nRC = 0;
      if ( nLevel == zLEVEL_SUBTASK )
      {
         // Look for the name in the subtask.
         lpSubtask = zGETPTR( lpSubtaskView->hSubtask );
         lpPrevViewName = 0;
         lpViewName = zGETPTR( lpSubtask->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpPrevViewName = lpViewName;
            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, remove the view from the subtask
         // chain and free up its storage.
         if ( lpViewName && (lpView == 0 || lpViewName->hView == hView) )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            if ( lpPrevViewName )
               lpPrevViewName->hNextViewName = lpViewName->hNextViewName;
            else
               lpSubtask->hFirstViewName = lpViewName->hNextViewName;

            fnFreeDataspace( lpViewName );
         }
         else
         {
            nRC = 1;
         }
      }
      else
      if ( nLevel == zLEVEL_TASK )
      {
         LPTASK     lpTask;

         // Look for the requested name in the task.  If a subtask view was
         // supplied use the task represented by the view.
         if ( lpSubtaskView )
            lpTask = zGETPTR( lpSubtaskView->hTask );
         else
            lpTask = lpCurrentTask;

         // Look for the name in the task.
         lpPrevViewName = 0;
         lpViewName = zGETPTR( lpTask->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpPrevViewName = lpViewName;
            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, remove the view from the task
         // chain and free up its storage.
         if ( lpViewName && (lpView == 0 || lpViewName->hView == hView) )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            if ( lpPrevViewName )
               lpPrevViewName->hNextViewName = lpViewName->hNextViewName;
            else
               lpTask->hFirstViewName = lpViewName->hNextViewName;

            fnFreeDataspace( lpViewName );
         }
         else
            nRC = 1;
      }
      else
      if ( nLevel == zLEVEL_APPLICATION )
      {
         // Look for the name at the application level.
         // The application level is determined by the view being named.
         // If the view being named is a subtask view, then the application
         // is the application for the subtask. Otherwise, if the view has
         // a ViewOD, the application is the app for the ViewOD. If all
         // else fails, we will use the application associated with the
         // task.
         fnGetApplicationForSubtask( &lpApp, lpView );
         lpPrevViewName = 0;
         lpViewName = zGETPTR( lpApp->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpPrevViewName = lpViewName;
            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, remove the view from the application
         // chain and free up its storage
         if ( lpViewName && (lpView == 0 || lpViewName->hView == hView) )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            if ( lpPrevViewName )
               lpPrevViewName->hNextViewName = lpViewName->hNextViewName;
            else
               lpApp->hFirstViewName = lpViewName->hNextViewName;

            fnFreeDataspace( lpViewName );
         }
         else
            nRC = 1;
      }
      else
      if ( nLevel == zLEVEL_SYSTEM )
      {
         zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

         // First, see if the name exists at the system level.
         lpPrevViewName = 0;
         lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpPrevViewName = lpViewName;
            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, remove the view from the system
         // chain and free up its storage.
         if ( lpViewName && (lpView == 0 || lpViewName->hView == hView) )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            if ( lpPrevViewName )
               lpPrevViewName->hNextViewName = lpViewName->hNextViewName;
            else
               AnchorBlock->hFirstViewName = lpViewName->hNextViewName;

            fnFreeDataspace( lpViewName );
         }
         else
            nRC = 1;

         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      }

   } while ( FALSE );  // end of purist's goto

   fnOperationReturn( iDropNameForView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetNameForView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetNameForView
//
//  PURPOSE:    To retrieve a name from a view at the level requested
//              (see SetNameForView for level values).  This operation
//              will only retrieve the first name found for a view at the
//              level requested even though there may be multiple names
//              for the same view at that level.
//
//  RETURNS:   -1 - View not found.
//              1 - Level of the view found is zLEVEL_SUBTASK
//              2 - Level of the view found is zLEVEL_TASK
//              3 - Level of the view found is zLEVEL_APPLICATION
//              4 - Level of the view found is zLEVEL_SYSTEM
//    zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetNameForView( zPCHAR   pchReturnName,
                zVIEW    lpView,
                zVIEW    lpSubtaskView,
                zSHORT   nLevel )
{
   LPTASK      lpCurrentTask;
   LPSUBTASK   lpSubtask;
   LPVIEWNAME  lpViewName;
   zPVOID      hView = zGETHNDL( lpView );
   LPAPP       lpApp;
   zSHORT      nRC;

   // Clear out pchReturnName
   pchReturnName[ 0 ] = 0;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iGetNameForView, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( (nLevel == zLEVEL_SUBTASK || lpSubtaskView) &&
        (fnValidSubtaskView( lpCurrentTask, lpSubtaskView )) == 0 )
   {
      fnOperationReturn( iGetNameForView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Check for valid scope.
   if ( nLevel != zLEVEL_SUBTASK     && nLevel != zLEVEL_TASK   &&
        nLevel != zLEVEL_APPLICATION && nLevel != zLEVEL_SYSTEM &&
        nLevel != zLEVEL_ANY )
   {
      // "KZOEE110 - Invalid scope for View name"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 110, (zLONG) nLevel, 0, 0 );
      nRC = zCALL_ERROR;
   }

   // Depending on the scope parameter, check the appropriate
   // name chain for the view.  If nRC is set to a non-zero value, then the
   // view has been found and no further checking needs to be done.
   nRC = -1;
   if ( nLevel == zLEVEL_SUBTASK || (nLevel == zLEVEL_ANY && lpSubtaskView) )
   {
      // Look for a named view in the subtask.
      lpSubtask = zGETPTR( lpSubtaskView->hSubtask );
      lpViewName = zGETPTR( lpSubtask->hFirstViewName );
      while ( lpViewName )
      {
         if ( lpViewName->hView == hView )
         {
            nRC = zLEVEL_SUBTASK;
            break;
         }

         lpViewName = zGETPTR( lpViewName->hNextViewName );
      }
   }

   if ( (nLevel == zLEVEL_TASK || nLevel == zLEVEL_ANY) && nRC < 0 )
   {
      // Look for the view in the task names
      lpViewName = zGETPTR( lpCurrentTask->hFirstViewName );
      while ( lpViewName )
      {
         if ( lpViewName->hView == hView )
         {
            nRC = zLEVEL_TASK;
            break;
         }

         lpViewName = zGETPTR( lpViewName->hNextViewName );
      }
   }

   if ( (nLevel == zLEVEL_APPLICATION || nLevel == zLEVEL_ANY) && nRC < 0 )
   {
      // Look for the view at the application level.
      fnGetApplicationForSubtask( &lpApp, lpView );
      lpViewName = zGETPTR( lpApp->hFirstViewName );
      while ( lpViewName )
      {
         if ( lpViewName->hView == hView )
         {
            nRC = zLEVEL_APPLICATION;
            break;
         }

         lpViewName = zGETPTR( lpViewName->hNextViewName );
      }
   }

   if ( (nLevel == zLEVEL_SYSTEM || nLevel == zLEVEL_ANY) && nRC < 0 )
   {
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      // First, see if the name exists at the system level
      lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
      while ( lpViewName )
      {
         if ( lpViewName->hView == hView )
         {
            nRC = zLEVEL_SYSTEM;
            break;
         }

         lpViewName = zGETPTR( lpViewName->hNextViewName );
      }

      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   }

   // If a name search was done, then see if the name was found.
   // If so, return the name in pchReturnName
   if ( nRC > 0 )
      strcpy_s( pchReturnName, sizeof( lpViewName->szName ), lpViewName->szName );

   fnOperationReturn( iGetNameForView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfSetSubtaskView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SfSetSubtaskView
//
//  PURPOSE: To establish a view as the view which defines
//           the subtask.
//
//  RETURNS: 0           - View successfully set as subtask view
//           zCALL_ERROR - error in setting subtask view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
SfSetSubtaskView( zVIEW lpView,
                  zVIEW lpCurrentSubtaskView )
{
   LPTASK      lpCurrentTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetSubtaskView, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( (fnValidSubtaskView( lpCurrentTask, lpCurrentSubtaskView )) == 0 )
   {
      fnOperationReturn( iSetSubtaskView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpView->hTask != lpCurrentSubtaskView->hTask )
   {
      //  "KZOEE111 - Attempt to set subtask view with a view from "
      //  "           another task"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 111, 0, 0, 0 );
      fnOperationReturn( iSetSubtaskView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpView->bApplicationView )
   {
      LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

      //  "KZOEE112 - Attempt to set subtask view attached to an application"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 112, 0, lpViewOD->szName, 0 );
      fnOperationReturn( iSetSubtaskView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpView != lpCurrentSubtaskView )
   {
      LPSUBTASK lpSubtask;

      if ( lpView->hSubtask )
      {
         //  "KZOEE100 - Invalid View, view is already a Subtask View"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 100, 0, 0, 0 );
         fnOperationReturn( iSetSubtaskView, lpCurrentTask );
         return( zCALL_ERROR );
      }

      lpView->hSubtask = lpCurrentSubtaskView->hSubtask;
      lpView->bViewLocked = TRUE;  // Subtask Views get locked automatically
      lpCurrentSubtaskView->hSubtask = 0;
      lpCurrentSubtaskView->bViewLocked = 0; // Unlock view, no longer subtask
      lpSubtask = zGETPTR( lpView->hSubtask );
      lpSubtask->hSubtaskView = zGETHNDL( lpView );

      // If the view has no ViewOD associated with it, then drop the view.
      if ( lpCurrentSubtaskView->hViewOD == 0 )
         fnDropView( lpCurrentSubtaskView );
   }

   fnOperationReturn( iSetSubtaskView, lpCurrentTask );
   return( 0 );
}

#define IDM_GETOEWARNINGTRACE 215

/////////////////////////////////////////////////////////////////////////////
//
//  lFlag    1 - Set the default task view
//           2 - Set WEB mode for the task
//           4 - Set the real task view
//
//  RETURNS: previous default/real task view
//
/////////////////////////////////////////////////////////////////////////////
zVIEW OPERATION
SetDefaultViewForActiveTask( zVIEW vDefault, zLONG lFlag )
{
   zVIEW vPrev = g_vDefaultTaskView;
   zCHAR szTrace[ 2 ];

   if ( lFlag & 0x00000004 )
   {
      vPrev = g_vRealTaskView;
      g_vRealTaskView = vDefault;
   }

   if ( lFlag & 0x00000001 )
   {
      g_vDefaultTaskView = vDefault;
      g_lProcessID = SysGetProcessID( 0 );

   }

   if ( lFlag & 0x00000002 )
   {
      LPTASK lpTask = 0;

      if ( vDefault )
         lpTask  = zGETPTR( vDefault->hTask );
      else
      if ( g_vDefaultTaskView )
         lpTask = zGETPTR( g_vDefaultTaskView->hTask );

      if ( lpTask )
         lpTask->vWebSubtask = vDefault;
   }

   szTrace[ 0 ] = (zCHAR) SysSendOE_Message( IDM_GETOEWARNINGTRACE, 0 );
   szTrace[ 1 ] = 0;
   if ( szTrace[ 0 ] == 'n' || szTrace[ 0 ] == 'N' )
      g_nOE_TraceWarning = 0;
   else
   if ( szTrace[ 0 ] == 'y' || szTrace[ 0 ] == 'Y' )
      g_nOE_TraceWarning = 99;
   else
      g_nOE_TraceWarning = (zSHORT) zatol( szTrace );

   return( vPrev );
}

zVIEW OPERATION
GetDefaultViewForActiveTask( )
{
   return( g_vDefaultTaskView );
}

//./ ADD NAME=SetNameForView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetNameForView
//
//  PURPOSE: To name a view at the level requested.
//           Note: If the name already exists at the level requested,
//           then it is changed to point to the new view passed.
//
//
//  PARMS:   lpView        - View to be named.
//           cpcName       - View name.
//           lpSubtaskView - Defines the subtask, task, or application the
//                           name is to be tied to.
//           nLevel        - Defines what level the view name is to be tied
//                           to.  Also can indicate whether view is to be
//                           automatically cleaned up.
//
//  RETURNS: 0           - View successfully named
//           zCALL_ERROR - error naming view
//
//  DEFINED LEVELS:
//
//         zLEVEL_SUBTASK      1
//         zLEVEL_TASK         2
//         zLEVEL_APPLICATION  4
//         zLEVEL_SYSTEM       8
//         zLEVEL_ANY          F = 1 + 2 + 4 + 8
//         zNAME_AUTODROP      64
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetNameForView( zVIEW    lpView,
                zCPCHAR  cpcName,
                zVIEW    lpSubtaskView,
                zSHORT   nLevel )
{
   LPTASK      lpCurrentTask;
   LPTASK      lpTask;
   LPSUBTASK   lpSubtask;
   LPVIEWNAME  lpViewName;
   zPVOID      hViewName;
   LPAPP       lpApp;
   zBOOL       bAutoDrop;
   zSHORT      nRC;

#if 1
   if ( lpSubtaskView == 0 )
   {
      if ( g_vDefaultTaskView )
         lpSubtaskView = g_vDefaultTaskView;  // must be called from Driver
      else
         lpSubtaskView = lpView;  // must be called from JSP
   }
   else
   if ( g_vDefaultTaskView && nLevel == zLEVEL_TASK )
   {
      if ( lpSubtaskView->hTask != g_vDefaultTaskView->hTask )
      {
         lpCurrentTask = zGETPTR( lpSubtaskView->hTask );
         if ( lpCurrentTask && lpCurrentTask->bTransientTask )
         {
            TraceLine( "SetNameForView sending in view for TRANSIENT task: %d (0x%08x)  "
                         "should be for task: %d (0x%08x) ... WILL NOT RESET TASK VIEW",
                       lpSubtaskView->hTask, lpSubtaskView->hTask,
                       g_vDefaultTaskView->hTask, g_vDefaultTaskView->hTask );
         }
         else
         {
         // SysMessageBox( 0, "Zeidon OE", "SetNameForView now RESETTING TASK VIEW", 0 );
            if ( g_nOE_TraceWarning )
            {
               TraceLine( "SetNameForView sending in view for task: %d (0x%08x)  "
                            "should be for task: %d (0x%08x) ... RESETTING TASK VIEW",
                          lpSubtaskView->hTask, lpSubtaskView->hTask,
                          g_vDefaultTaskView->hTask, g_vDefaultTaskView->hTask );
            }

            lpSubtaskView = g_vDefaultTaskView;
         }
      }
   }
#endif

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetNameForView, lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( zstrlen( cpcName ) > zZEIDON_NAME_LTH * 2 )
   {
      fnIssueCoreError( lpCurrentTask, lpView, 8, 450, 0, cpcName, 0 );
      fnOperationReturn( iSetNameForView, lpCurrentTask );
      return( zCALL_ERROR );
   }

#if 0
// if ( zstrcmp( cpcName, "vERD_LPLR" ) == 0 )
   if ( cpcName[ 0 ] != '_' )
   TraceLine( "SetNameForView (0x%08x) %s   Task: 0x%08x   Level: %s",
              lpView, cpcName,
              lpSubtaskView ? lpSubtaskView->hTask : lpView->hTask,
              nLevel & 1 ? "SUBTASK" : nLevel & 2 ? "TASK" : nLevel & 4 ?
                "APPLICATION" : nLevel & 8 ? "SYSTEM" : "UNKNOWN" );
#endif

   if ( (nLevel & zLEVEL_SUBTASK) && (fnValidSubtaskView( lpCurrentTask, lpSubtaskView )) == 0 )
   {
      TraceLineS( "(vm) Invalid subtask -- name = ", cpcName );
      fnOperationReturn( iSetNameForView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( (nLevel & zLEVEL_APPLICATION) || lpSubtaskView )
   {
      // If level is application but lpSubtaskView is not specified, then use the lpView.
      if ( lpSubtaskView == 0 )
         lpSubtaskView = lpView;

      // Since we are using the passed view in SetNameForView, we should probably use
      // the task of the passed view as the task on which to hang the name.
      lpTask = zGETPTR( lpSubtaskView->hTask );
      if ( (nLevel & zLEVEL_APPLICATION) &&
           lpView->bApplicationView == FALSE &&
           lpTask->bApplicationTask == FALSE &&
           lpView->hTask != AnchorBlock->hMainTask )
      {
      // static zSHORT k = 0;
      // zCHAR szDebug[ 2 ];

         TraceLineS( "(vm) Naming non-application level view at the application level: ", cpcName );

      // SysReadZeidonIni( -1, "[Debug]", "BombZDr", szDebug, sizeof( szDebug ) );
      // if ( szDebug[ 0 ] == 'Y' )
      // {
      //    k /= k;
      //    k++;
      // }
      }

      if ( fnValidView( lpCurrentTask, lpSubtaskView ) == 0 )
      {
         TraceLineS( "(vm) Invalid view -- name = ", cpcName );
         fnOperationReturn( iSetNameForView, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }
   else
      lpTask = lpCurrentTask;

   if ( nLevel & zNAME_AUTODROP )
   {
      bAutoDrop = TRUE;
      nLevel -= zNAME_AUTODROP;
   }
   else
      bAutoDrop = 0;

   // Check for valid scope.
   if ( (nLevel & zLEVEL_SUBTASK) == 0     && (nLevel & zLEVEL_TASK) == 0   &&
        (nLevel & zLEVEL_APPLICATION) == 0 && (nLevel & zLEVEL_SYSTEM) == 0 &&
        (nLevel & zLEVEL_ANY) == 0 )
   {
      //  "KZOEE110 - Invalid scope for View name"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 110, (zLONG) nLevel, 0, 0 );
      nRC = zCALL_ERROR;
      TraceLineS( "(vm) Invalid scoping level -- name = ", cpcName );
   }

   // Depending on the scope parameter, check the appropriate name chain
   // for the name being created.
   nRC = 0;
   if ( cpcName && cpcName[ 0 ] )
   {
      if ( nLevel == zLEVEL_SUBTASK )
      {
         // First, see if the name exists in the subtask.
         lpSubtask = zGETPTR( lpSubtaskView->hSubtask );
         lpViewName = zGETPTR( lpSubtask->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, change it to point to the view
         // just passed in.
         if ( lpViewName )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            lpViewName->hView = zGETHNDL( lpView );
            lpView->nViewNames++;
         }
         else
         {
            hViewName = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                          sizeof( ViewNameRecord ), 0, 0,
                                          iViewName );
            lpViewName = zGETPTR( hViewName );
            if ( lpViewName )
            {
               strcpy_s( lpViewName->szName, sizeof( lpViewName->szName ), cpcName );
               lpViewName->hView = zGETHNDL( lpView );
               lpView->nViewNames++;
               lpViewName->hNextViewName = lpSubtask->hFirstViewName;
               lpSubtask->hFirstViewName = hViewName;
            }
            else
               nRC = zCALL_ERROR;
         }
      }
      else
      if ( nLevel == zLEVEL_TASK )
      {
         // First, see if the name exists in the task.
         lpViewName = zGETPTR( lpTask->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, change it to point to the view
         // just passed in.
         if ( lpViewName )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            lpViewName->hView = zGETHNDL( lpView );
            lpView->nViewNames++;
            lpView->bTaskNamed = TRUE;
         }
         else
         {
            hViewName = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                          sizeof( ViewNameRecord ), 0, 0,
                                          iViewName );
            lpViewName = zGETPTR( hViewName );
            if ( lpViewName )
            {
               strcpy_s( lpViewName->szName, sizeof( lpViewName->szName ), cpcName );
               lpViewName->hView = zGETHNDL( lpView );
               lpView->nViewNames++;
               lpView->bTaskNamed = TRUE;
               lpViewName->hNextViewName = lpTask->hFirstViewName;
               lpTask->hFirstViewName = hViewName;
            }
            else
               nRC = zCALL_ERROR;
         }
      }
      else
      if ( nLevel == zLEVEL_APPLICATION )
      {
         // First, see if the name exists at the application level.
         // The application level is determined by the view being named.
         // If the view being named is a subtask view, then the application
         // is the application for the subtask. Otherwise, if the view has a
         // ViewOD, the application is the app for the ViewOD. If all else
         // fails, we will use the application associated with the task.

         fnGetApplicationForSubtask( &lpApp, lpSubtaskView );
         lpViewName = zGETPTR( lpApp->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, change it to point to the view
         // just passed in
         if ( lpViewName )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            lpViewName->hView = zGETHNDL( lpView );
            lpView->nViewNames++;
            lpView->bApplicationNamed = TRUE;
         }
         else
         {
            hViewName = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                          sizeof( ViewNameRecord ), 0, 0,
                                          iViewName );
            lpViewName = zGETPTR( hViewName );
            if ( lpViewName )
            {
               strcpy_s( lpViewName->szName, sizeof( lpViewName->szName ), cpcName );
               lpViewName->hView = zGETHNDL( lpView );
               lpView->nViewNames++;
               lpView->bApplicationNamed = TRUE;
               lpViewName->hNextViewName = lpApp->hFirstViewName;
               lpApp->hFirstViewName = hViewName;
            }
            else
               nRC = zCALL_ERROR;
         }
      }
      else
      if ( nLevel == zLEVEL_SYSTEM )
      {
         zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

         // First, see if the name exists at the system level
         lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
               break;

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         // If the view name was found, change it to point to the view
         // just passed in
         if ( lpViewName )
         {
            zVIEW lpTempView = zGETPTR( lpViewName->hView );

            lpTempView->nViewNames--;
            lpViewName->hView = zGETHNDL( lpView );
            lpView->nViewNames++;
            lpView->bSystemNamed = TRUE;
         }
         else
         {
            hViewName = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                          sizeof( ViewNameRecord ), 0, 0,
                                          iViewName );
            lpViewName = zGETPTR( hViewName );
            if ( lpViewName )
            {
               strcpy_s( lpViewName->szName, sizeof( lpViewName->szName ), cpcName );
               lpViewName->hView = zGETHNDL( lpView );
               lpView->nViewNames++;
               lpView->bSystemNamed = TRUE;
               lpViewName->hNextViewName = AnchorBlock->hFirstViewName;
               AnchorBlock->hFirstViewName = hViewName;
            }
            else
               nRC = zCALL_ERROR;
         }

         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      }
      else
      {
         //  "KZOEE110 - Invalid scope for View name"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 110, (zLONG) nLevel, 0, 0 );
         lpViewName = 0;
         nRC = zCALL_ERROR;
      }

      // Now that the name has been created, increment the autodrop
      // counter if autodrop was specified
      if ( bAutoDrop && lpViewName )
      {
         lpView->nAutoDropNames++;
      }
   }

   fnOperationReturn( iSetNameForView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetViewByName
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetViewByName
//
//  PURPOSE:    To retrieve a view by name at the level requested
//              (see SetNameForView for level values). The view passed
//              is used to qualify the subtask for subtask level and
//              the application for application level. If the level is
//              subtask, the view MUST BE a subtask view. Otherwise, any
//              valid view will do. For Task and System level, the
//              view passed may be NULL.  When qualifying an application,
//              the application for a subtask is used if the view is a
//              subtask view, otherwise, the application for the view
//              object referenced by the view is used.
//
//              zLEVEL_ANY has been changed: if lpSubtaskView is 0 then
//              this function won't check for a view at the subtask or
//              application level.
//
//  RETURNS:   -1 - View not found.
//              1 - Level of the view found is zLEVEL_SUBTASK
//              2 - Level of the view found is zLEVEL_TASK
//              4 - Level of the view found is zLEVEL_APPLICATION
//              8 - Level of the view found is zLEVEL_SYSTEM
//    zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetViewByName( zPVIEW   pvReturnView,
               zCPCHAR  cpcName,
               zVIEW    lpSubtaskView,
               zSHORT   nLevel )
{
   LPTASK      lpCurrentTask;
   LPTASK      hCurrentTask;
   zVIEW       vSubtask;
   LPSUBTASK   lpSubtask;
   LPVIEWNAME  lpViewName;
   LPAPP       lpApp;
   zSHORT      nRC = zCALL_ERROR;

   *pvReturnView = 0;  // initialize return view to NULL

   vSubtask = lpSubtaskView;

   if ( lpSubtaskView )
      hCurrentTask = lpSubtaskView->hTask;
   else
      hCurrentTask = 0;

#if 1
   if ( lpSubtaskView == 0 )
   {
      if ( g_vDefaultTaskView )
         lpSubtaskView = g_vDefaultTaskView;
      else
         SysMessageBox( 0, "OE Error", "GetViewByName now requires a valid view", -1 );
   }
   else
   if ( g_vDefaultTaskView && nLevel == zLEVEL_TASK )
   {
      if ( lpSubtaskView->hTask != g_vDefaultTaskView->hTask )
      {
         lpCurrentTask = zGETPTR( lpSubtaskView->hTask );
         if ( lpCurrentTask && lpCurrentTask->bTransientTask )
         {
            TraceLine( "GetViewByName sending in view for TRANSIENT task: %d (0x%08x)  "
                         "should be for task: %d (0x%08x) ... WILL NOT RESET TASK VIEW",
                       lpSubtaskView->hTask, lpSubtaskView->hTask,
                       g_vDefaultTaskView->hTask, g_vDefaultTaskView->hTask );
         }
         else
         {
         // SysMessageBox( 0, "Zeidon OE", "GetViewByName now RESETTING TASK VIEW", 0 );
            if ( g_nOE_TraceWarning )
            {
               TraceLine( "GetViewByName sending in view for task: %d (0x%08x)  "
                            "should be for task: %d (0x%08x) ... RESETTING TASK VIEW",
                          lpSubtaskView->hTask, lpSubtaskView->hTask,
                          g_vDefaultTaskView->hTask, g_vDefaultTaskView->hTask );
            }

            lpSubtaskView = g_vDefaultTaskView;
         }
      }
   }
#endif

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iGetViewByName, lpSubtaskView, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

// if ( zstrcmp( cpcName, "vERD_LPLR" ) == 0 )
// TraceLine( "GetViewByName for: %s   Task: %d (0x%08x)   CurrentTask %d (0x%08x)",
//            cpcName, lpSubtaskView->hTask, lpSubtaskView->hTask,
//            zGETHNDL( lpCurrentTask ), zGETHNDL( lpCurrentTask ) );

   do  // begin purist's goto
   {
      // Check for valid scope.
      if ( nLevel != zLEVEL_SUBTASK     && nLevel != zLEVEL_TASK   &&
           nLevel != zLEVEL_APPLICATION && nLevel != zLEVEL_SYSTEM &&
           nLevel != zLEVEL_ANY )
      {
         TraceLineS( "GetViewByName Invalid scope for View name: ", cpcName );
         //  "KZOEE110 - Invalid scope for View name"
         fnIssueCoreError( lpCurrentTask, lpSubtaskView, 8, 110, (zLONG) nLevel, 0, 0 );
      // fnOperationReturn( iGetViewByName, lpCurrentTask );
         break;
      }

      if ( cpcName == 0 || cpcName[ 0 ] == 0 ||
           zstrlen( cpcName ) > zZEIDON_NAME_LTH * 2 )
      {
         if ( cpcName && cpcName[ 0 ] )
         {
            TraceLineS( "GetViewByName Length Error: ", cpcName );
            fnIssueCoreError( lpCurrentTask, lpSubtaskView, 8, 450, 0, cpcName, 0 );
         }
         else
         {
            TraceLineS( "GetViewByName - Empty name", "" );
         }

      // fnOperationReturn( iGetViewByName, lpCurrentTask );
         break;
      }

      // If user wants a subtask view make sure subtask is OK.
      if ( nLevel == zLEVEL_SUBTASK &&
           fnValidSubtaskView( lpCurrentTask, lpSubtaskView ) == 0 )
      {
         TraceLineS( "GetViewByName Invalid Subtask view for Name: ", cpcName );
         break;  // view invalid
      }

      // If user specified ANY and specified a view make sure it's OK.
      if ( nLevel == zLEVEL_ANY &&
           fnValidSubtaskView( lpCurrentTask, lpSubtaskView ) == 0 )
      {
         TraceLineS( "GetViewByName Invalid view for Name: ", cpcName );
         break;  // view invalid
      }

      if ( nLevel == zLEVEL_APPLICATION || lpSubtaskView )
      {
         // If lpSubtaskView is not a subtask view, check to see if it's a
         // valid view.  (If lpSubtaskView is null, we know it's an invalid
         // view but we'll still call fnValidView to show the error message).
         if ( lpSubtaskView && lpSubtaskView->hSubtask == 0 )
         {
            if ( fnValidView( lpCurrentTask, lpSubtaskView ) == 0 )
            {
               TraceLineS( "GetViewByName Invalid Subtask (null) for Name: ", cpcName );
               break;  // view invalid
            }
         }
         else
         // lpSubtaskView is a subtask view--check for validity.
         if ( fnValidSubtaskView( lpCurrentTask, lpSubtaskView ) == 0 )
         {
            TraceLineS( "GetViewByName Invalid Subtask view for Name: ", cpcName );
            break;  // view invalid
         }
      }

      // Depending on the scope parameter, look in the appropriate name chain
      // for the name requested.
      nRC = -1;
      lpViewName = 0;
      if ( nLevel == zLEVEL_SUBTASK || (nLevel == zLEVEL_ANY && lpSubtaskView) )
      {
         // Look for the requested name in the subtask
         lpSubtask = zGETPTR( lpSubtaskView->hSubtask );
         lpViewName = zGETPTR( lpSubtask->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
            {
               nRC = zLEVEL_SUBTASK;
            // TraceLineS( "GetViewByName SUBTASK: ", cpcName );
               break;
            }

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }
      }

      if ( nLevel == zLEVEL_TASK || (nLevel == zLEVEL_ANY && nRC < 0) )
      {
         LPTASK     lpTask;
         LPVIEWNAME lpVName;  // debugging only

         // Look for the requested name in the task.  If a subtask view was
         // supplied use the task represented by the view.
         if ( lpSubtaskView )
            lpTask = zGETPTR( lpSubtaskView->hTask );
         else
            lpTask = lpCurrentTask;

      // TraceLineS( "GetViewByName looking for Task Level view by name: ",
      //             cpcName );
         for ( lpVName = zGETPTR( lpTask->hFirstViewName );
               lpVName;
               lpVName = zGETPTR( lpVName->hNextViewName ) )
         {
         // if ( zstrcmp( cpcName, "vERD_LPLR" ) == 0 )
         // TraceLine( "GetViewByName at Task Level 0x%08x ViewName: %s",
         //            zGETHNDL( lpTask ), lpVName->szName );

            if ( zstrcmp( lpVName->szName, cpcName ) == 0 )
            {
               nRC = zLEVEL_TASK;
            // TraceLineS( "GetViewByName TASK found: ", cpcName );
               lpViewName = lpVName;  // debugging only
               break; // omitted for debugging only
            }
         }
      }

      if ( nLevel == zLEVEL_APPLICATION ||
           (nLevel == zLEVEL_ANY && lpSubtaskView && nRC < 0) )
      {
         // Get the application from the subtask if the view is a subtask
         // view. Otherwise, get the application from the view object if
         // one is present. If all else fails, use the application from
         // the current task.
         fnGetApplicationForSubtask( &lpApp, lpSubtaskView );
         lpViewName = zGETPTR( lpApp->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
            {
               nRC = zLEVEL_APPLICATION;
            // TraceLineS( "GetViewByName APPLICATION: ", cpcName );
               break;
            }

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }
      }

      if ( nLevel == zLEVEL_SYSTEM || (nLevel == zLEVEL_ANY && nRC < 0) )
      {
         zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

         // Look for the requested name at the system level
         lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, cpcName ) == 0 )
            {
               nRC = zLEVEL_SYSTEM;
            // TraceLineS( "GetViewByName SYSTEM: ", cpcName );
               break;
            }

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }

         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      }

   } while ( FALSE );  // end of purist's goto

   // If the return code from the search is > 0, then set the view to be
   // returned.
   if ( nRC > 0 )
   {
      // Set the view to be returned.
      *pvReturnView = zGETPTR( lpViewName->hView );
   }

#if 0
   if ( zstrcmp( "OpenCM_Metas", cpcName ) == 0 )
   {
      SysMessageBox( lpSubtaskView, "GetViewByName", "OpenCM_Metas", 0 );
      TraceLineS( "GetViewByName: ", "OpenCM_Metas" );
   }
#endif

#if 0
   if ( cpcName[ 0 ] != '_' )
   TraceLine( "GetViewByName (0x%08x) %s   View/Task: 0x%08x/0x%08x   Level: %s   RC ===> %s",
              *pvReturnView, cpcName, lpSubtaskView, lpSubtaskView->hTask,
              (nLevel & 0x000F) == 0x000F ? "ANY" :
                nLevel & 1 ? "SUBTASK" : nLevel & 2 ? "TASK" : nLevel & 4 ?
                "APPLICATION" : nLevel & 8 ? "SYSTEM" : "UNKNOWN",
              nRC <= 0 ? "NOT FOUND" :
                nRC & 1 ? "SUBTASK" : nRC & 2 ? "TASK" : nRC & 4 ?
                "APPLICATION" : nRC & 8 ? "SYSTEM" : "UNKNOWN" );
#endif

#if 1
   if ( *pvReturnView && hCurrentTask &&             // dks debug
        (*pvReturnView)->hTask != hCurrentTask )
   {
      LPTASK lpTask = zGETPTR( hCurrentTask );
      if ( lpTask && lpTask->bTransientTask &&
           zstrcmp( cpcName, "Zeidon DBHandler Work Object" ) != 0 )
      {
         TraceLine( "GetViewByName TRANSIENT ERROR (0x%08x/0x%08x)  %s   View/Task: 0x%08x/0x%08x   Level: %s   RC ===> %s",
                    *pvReturnView, (*pvReturnView)->hTask, cpcName, vSubtask, hCurrentTask,
                    (nLevel & 0x000F) == 0x000F ? "ANY" :
                      nLevel & 1 ? "SUBTASK" : nLevel & 2 ? "TASK" : nLevel & 4 ?
                      "APPLICATION" : nLevel & 8 ? "SYSTEM" : "UNKNOWN",
                    nRC <= 0 ? "NOT FOUND" :
                      nRC & 1 ? "SUBTASK" : nRC & 2 ? "TASK" : nRC & 4 ?
                      "APPLICATION" : nRC & 8 ? "SYSTEM" : "UNKNOWN" );
      // if ( *pvReturnView )
      //    DisplayObjectInstance( *pvReturnView, 0, 0 );
      }
   }
#endif

   fnOperationReturn( iGetViewByName, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfGetFirstNamedView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetFirstNamedView
//
//  RETURNS:   -1 - View not found.
//              1 - Level of the view found is zLEVEL_SUBTASK
//              2 - Level of the view found is zLEVEL_TASK
//              4 - Level of the view found is zLEVEL_APPLICATION
//              8 - Level of the view found is zLEVEL_SYSTEM
//    zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SfGetFirstNamedView( zPVIEW  pvReturnView,
                     zPCHAR  pchReturnName,
                     zVIEW   lpView,
                     zSHORT  nLevel )
{
   LPTASK       lpCurrentTask;
   LPSUBTASK    lpSubtask;
   LPVIEWNAME   lpViewName;
   LPAPP        lpApp;
   zSHORT       nRC;

   *pvReturnView = 0;      // initialize return view to NULL
   pchReturnName[ 0 ] = 0; // initialize return name

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfGetFirstNamedView, lpView, 0 )) == 0 )
      return( zCALL_ERROR );

   if ( (nLevel == zLEVEL_SUBTASK) &&
        (fnValidSubtaskView( lpCurrentTask, lpView )) == 0 )
   {
      fnOperationReturn( iSfGetFirstNamedView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( (nLevel == zLEVEL_APPLICATION) &&
        (fnValidView( lpCurrentTask, lpView )) == 0 )
   {
      fnOperationReturn( iSfGetFirstNamedView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Check for valid scope.
   if ( nLevel != zLEVEL_SUBTASK     && nLevel != zLEVEL_TASK   &&
        nLevel != zLEVEL_APPLICATION && nLevel != zLEVEL_SYSTEM )
   {
      //  "KZOEE110 - Invalid scope for View name"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 110, (zLONG) nLevel, 0, 0 );
      fnOperationReturn( iSfGetFirstNamedView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Depending on the scope parameter, look in the appropriate
   // name chain for the name requested.
   lpViewName = 0;
   if ( nLevel == zLEVEL_SUBTASK )
   {
      // Look for the requested name in the subtask.
      lpSubtask = zGETPTR( lpView->hSubtask );
      lpViewName = zGETPTR( lpSubtask->hFirstViewName );
      nRC = zLEVEL_SUBTASK;
   }
   else
   if ( nLevel == zLEVEL_TASK )
   {
      // Look for the requested name in the task.
      lpViewName = zGETPTR( lpCurrentTask->hFirstViewName );
      nRC = zLEVEL_TASK;
   }
   else
   if ( nLevel == zLEVEL_APPLICATION )
   {
      // Get the application from the subtask if the view is a subtask
      // view. Otherwise, get the application from the view object if
      // one is present. If all else fails, use the application from
      // the current task.

      fnGetApplicationForSubtask( &lpApp, lpView );
      lpViewName = zGETPTR( lpApp->hFirstViewName );
      nRC = zLEVEL_APPLICATION;
   }
   else
   if ( nLevel == zLEVEL_SYSTEM )
   {
      // Look for the requested name at the system level.
      lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
      nRC = zLEVEL_SYSTEM;
   }

   // If an lpViewName was found, return the view and name.
   if ( lpViewName )
   {
      // Set the view to be returned
      *pvReturnView = zGETPTR( lpViewName->hView );
      strcpy_s( pchReturnName, sizeof( lpViewName->szName ), lpViewName->szName );
   }
   else
      nRC = -1;

   fnOperationReturn( iSfGetFirstNamedView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfGetNextNamedView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetNextNamedView
//
//  RETURNS:   -1 - View not found.
//              1 - Level of the view found is zLEVEL_SUBTASK
//              2 - Level of the view found is zLEVEL_TASK
//              4 - Level of the view found is zLEVEL_APPLICATION
//              8 - Level of the view found is zLEVEL_SYSTEM
//    zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SfGetNextNamedView( zPVIEW   pvReturnView,
                    zPCHAR   pchReturnName,
                    zVIEW    lpView,
                    zSHORT   nLevel )
{
   LPTASK       lpCurrentTask;
   LPSUBTASK    lpSubtask;
   LPVIEWNAME   lpViewName;
   LPAPP        lpApp;
   zSHORT       nRC;

   *pvReturnView = 0;      // initialize return view to null

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfGetNextNamedView, lpView, 0 )) == 0 )
      return( zCALL_ERROR );

   if ( (nLevel == zLEVEL_SUBTASK) &&
        (fnValidSubtaskView( lpCurrentTask, lpView )) == 0 )
   {
      fnOperationReturn( iSfGetNextNamedView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( (nLevel == zLEVEL_APPLICATION) &&
        (fnValidView( lpCurrentTask, lpView )) == 0 )
   {
      fnOperationReturn( iSfGetNextNamedView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Check for valid scope.
   if ( nLevel != zLEVEL_SUBTASK     && nLevel != zLEVEL_TASK   &&
        nLevel != zLEVEL_APPLICATION && nLevel != zLEVEL_SYSTEM )
   {
      //  "KZOEE110 - Invalid scope for View name"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 110, (zLONG) nLevel, 0, 0 );
      fnOperationReturn( iSfGetNextNamedView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Depending on the scope parameter, look in the appropriate
   // name chain for the name requested.
   lpViewName = 0;
   if ( pchReturnName && pchReturnName[ 0 ] )
   {
      if ( nLevel == zLEVEL_SUBTASK )
      {
         // Look for the requested name in the subtask.
         lpSubtask  = zGETPTR( lpView->hSubtask );
         lpViewName = zGETPTR( lpSubtask->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, pchReturnName ) == 0 )
            {
               nRC = zLEVEL_SUBTASK;
               lpViewName = zGETPTR( lpViewName->hNextViewName );
               break;
            }

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }
      }
      else
      if ( nLevel == zLEVEL_TASK )
      {
         // Look for the requested name in the task.
         lpViewName = zGETPTR( lpCurrentTask->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, pchReturnName ) == 0 )
            {
               nRC = zLEVEL_TASK;
               lpViewName = zGETPTR( lpViewName->hNextViewName );
               break;
            }

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }
      }
      else
      if ( nLevel == zLEVEL_APPLICATION )
      {
         // Get the application from the subtask if the view is a subtask
         // view. Otherwise, get the application from the view object if
         // one is present. If all else fails, use the application from
         // the current task.
         fnGetApplicationForSubtask( &lpApp, lpView );
         lpViewName = zGETPTR( lpApp->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, pchReturnName ) == 0 )
            {
               nRC = zLEVEL_APPLICATION;
               lpViewName = zGETPTR( lpViewName->hNextViewName );
               break;
            }

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }
      }
      else
      if ( nLevel == zLEVEL_SYSTEM )
      {
         // Look for the requested name at the system level.
         lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
         while ( lpViewName )
         {
            if ( zstrcmp( lpViewName->szName, pchReturnName ) == 0 )
            {
               nRC = zLEVEL_SYSTEM;
               lpViewName = zGETPTR( lpViewName->hNextViewName );
               break;
            }

            lpViewName = zGETPTR( lpViewName->hNextViewName );
         }
      }
   }

   // If an lpViewName was found, return the view and name.
   if ( lpViewName )
   {
      // Set the view to be returned.
      *pvReturnView = zGETPTR( lpViewName->hView );
      strcpy_s( pchReturnName, sizeof( lpViewName->szName ), lpViewName->szName );
   }
   else
   {
      pchReturnName[ 0 ] = 0;
      nRC = -1;
   }

   fnOperationReturn( iSfGetNextNamedView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=ResetView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   ResetView
//
//  PURPOSE: To reset a view to its initial state. All subobject
//           views are reset and the cursors are reset to the
//           first entity instance for each entity type.  Select
//           sets are reset as well.
//
//  RETURNS: 0           - View successfully reset
//           zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
ResetView( zVIEW  lpView )
{
   LPTASK  lpCurrentTask;
   zSHORT  nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iResetView, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Call fnResetView to reset the view.
   nRC = fnResetView( lpView, 0 );

   // We're done, return success
   fnOperationReturn( iResetView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=ResetViewPositions
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   ResetViewPositions
//
//  PURPOSE: To reset a view to its initial state. All subobject
//           views are reset and the cursors are reset to the
//           first entity instance for each entity type.  Select
//           sets are NOT reset.
//
//  RETURNS: 0           - View successfully reset
//           zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
ResetViewPositions( zVIEW  lpView )
{
   LPTASK  lpCurrentTask;
   zSHORT  nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iResetViewPositions, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Call fnResetView to reset the view without resetting select sets
   nRC = fnResetView( lpView, 1 );

   // We're done, return success
   fnOperationReturn( iResetViewPositions, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetViewToSubobject
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetViewToSubobject
//
//  PURPOSE: To set a view to a subobject entity. If the subobject
//           entity is defined to behave recursively, then the
//           instance leveling is adjusted and the entity instance
//           at the target entity is moved up to the recursive
//           parent and will then be referenced by the recursive
//           parent's name. Otherwise, the entity types visible in
//           the view are refined to only include the target
//           entity and its subordinates.
//
//  RETURNS: 0           - View successfully set for subobject
//           1           - View is currently established on the
//                         subobject
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SetViewToSubobject( zVIEW  lpView, zCPCHAR cpcSubobjectEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewRootParentCsr;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetViewToSubobject, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Get the ViewCsr for the view
   lpViewCsr = zGETPTR( lpView->hViewCsr );

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpView,
                                           cpcSubobjectEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iSetViewToSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Check to make sure that the subobject entity is not the root
   // root of the view object definition
   if ( zGETPTR( lpViewCsr->hFirstOD_EntityCsr ) == lpViewEntityCsr )
   {
      LPVIEWOI lpViewOI = zGETPTR( lpViewCsr->hViewOI );
      LPVIEWOD lpViewOD = zGETPTR( lpViewOI->hViewOD );

      //  "KZOEE271 - Subobject Entity is root of View Object Definition"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 271, 0,
                        fnEntityNameFromCsr( lpViewEntityCsr ),
                        lpViewOD->szName );
      fnOperationReturn( iSetViewToSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Check to make sure that the subobject entity is not already the
   // root of the view, if so return a 1
   if ( zGETPTR( lpViewCsr->hRootViewEntityCsr ) == lpViewEntityCsr )
   {
      fnOperationReturn( iSetViewToSubobject, lpCurrentTask );
      return( 1 );
   }

   // Check to make sure that there is a parent entity instance for
   // the new root of the view
   if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
      fnEstablishCursorForView( lpViewEntityCsr );

   lpViewRootParentCsr = zGETPTR( lpViewEntityCsr->hParent );

   // We double check that the parent cursor does not need establishing
   // because create entity ( through another view ) might mark the
   // parent only as needing to be established.
   if ( lpViewRootParentCsr->hEntityInstance == UNSET_CSR )
      fnEstablishCursorForView( lpViewRootParentCsr );

   if ( lpViewRootParentCsr->hEntityInstance == 0 )
   {
      //  "KZOEE272 - Subobject Parent Cursor is NULL"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 272, 0,
                        fnEntityNameFromCsr( lpViewEntityCsr ),
                        fnEntityNameFromCsr( lpViewRootParentCsr ) );
      fnOperationReturn( iSetViewToSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Make sure the parent cursor is defined
   lpEntityInstance = zGETPTR( lpViewRootParentCsr->hEntityInstance );
   if ( lpEntityInstance->u.nInd.bHidden )
   {
      //  "KZOEE273 - Parent Entity instance for View deleted"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 273, 0,
                        fnEntityNameFromCsr( lpViewRootParentCsr ), 0 );
      fnOperationReturn( iSetViewToSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If we're in hierarchical mode, then reset is not allowed
   if ( lpViewCsr->hHierRootEntityInstance &&
        (lpViewCsr->hHierEntityInstance != lpViewEntityCsr->hEntityInstance ||
         lpViewEntity->bRecursive == FALSE) )

   {
      // "KZOEE266 - Invalid Operation for Hierarchical cursor position"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 266, 0, 0, 0 );
      fnOperationReturn( iSetViewToSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Call fnSetViewToSubobject to do the rest
   nRC = fnSetViewToSubobject( lpView, lpViewEntity, lpViewEntityCsr );

   fnOperationReturn( iSetViewToSubobject, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=ResetViewFromSubobjectWithoutError
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   ResetViewFromSubobjectWithoutError
//
//  PURPOSE: To reset a view from a subobject entity back to the
//           previous subobject level specified. SetViewToSubobject
//           calls act like a push pop stack so if three
//           SetViewToSubobject calls are made to refine a view, then
//           three ResetViewFromSubobject calls must be made to
//           get back to the initial state of the view.
//
//  RETURNS: 0 - View successfully reset
//           1 - View is not currently on a subobject
//          -1 - Parent Entity instance for View deleted
//          -2 - Invalid Operation for Hierarchical processing
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
ResetViewFromSubobjectWithoutError( zVIEW  lpView )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWSUBOBJECT   lpViewSubobject;
   LPENTITYINSTANCE  lpViewParentEntityInstance;
   zSHORT            nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iResetViewFromSubobject, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Get the ViewCsr for the view
   lpViewCsr       = zGETPTR( lpView->hViewCsr );
   lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );

   // Check to make sure that the subobject entity is not already the
   // root of the view
   if ( lpViewSubobject == 0 )
   {
      fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
      return( 1 );
   }

   lpViewParentEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );
   if ( lpViewParentEntityInstance->u.nInd.bHidden )
   {
      //  "KZOEE273 - Parent Entity instance for View deleted"
   // fnIssueCoreError( lpCurrentTask, lpView, 8, 273, 0,
   //                   fnEntityNameFromInstance( lpViewParentEntityInstance ),
   //                   0 );
      fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
      return( -1 );
   }

   // If we're in hierarchical mode, then reset is not allowed
   if ( lpViewCsr->hHierRootEntityInstance )
   {
      //  "KZOEE264 - Invalid Operation for Hierarchical processing"
   // fnIssueCoreError( lpCurrentTask, lpView, 8, 264, 0, 0, 0 );
      fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
      return( -2 );
   }

   // OK subobject is in-hand, reset from the subobject to the
   // previous subobject level.
   nRC = fnResetViewFromSubobject( lpView );

   fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=ResetViewFromSubobject
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   ResetViewFromSubobject
//
//  PURPOSE: To reset a view from a subobject entity back to the
//           previous subobject level specified. SetViewToSubobject
//           calls act like a push pop stack so if three
//           SetViewToSubobject calls are made to refine a view, then
//           three ResetViewFromSubobject calls must be made to
//           get back to the initial state of the view.
//
//  RETURNS: 0           - View successfully reset
//           1           - View is not currently on a subobject
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
ResetViewFromSubobject( zVIEW  lpView )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWSUBOBJECT   lpViewSubobject;
   LPENTITYINSTANCE  lpViewParentEntityInstance;
   zSHORT            nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iResetViewFromSubobject, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Get the ViewCsr for the view
   lpViewCsr       = zGETPTR( lpView->hViewCsr );
   lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );

   // Check to make sure that the subobject entity is not already the
   // root of the view
   if ( lpViewSubobject == 0 )
   {
      fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
      return( 1 );
   }

   lpViewParentEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );
   if ( lpViewParentEntityInstance->u.nInd.bHidden )
   {
      //  "KZOEE273 - Parent Entity instance for View deleted"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 273, 0,
                        fnEntityNameFromInstance( lpViewParentEntityInstance ),
                        0 );
      fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If we're in hierarchical mode, then reset is not allowed
   if ( lpViewCsr->hHierRootEntityInstance )
   {
      //  "KZOEE264 - Invalid Operation for Hierarchical processing"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 264, 0, 0, 0 );
      fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // OK subobject is in-hand, reset from the subobject to the
   // previous subobject level.
   nRC = fnResetViewFromSubobject( lpView );

   fnOperationReturn( iResetViewFromSubobject, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=ResetViewFromSubobjectTop
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   ResetViewFromSubobjectTop
//
//  PURPOSE: To reset a view from a subobject entity back to the top level.
//
//  RETURNS: 0           - View successfully reset
//           1           - View is not currently on a subobject
//           zCALL_ERROR - error creating new view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
ResetViewFromSubobjectTop( zVIEW  lpView )
{
   while ( ResetViewFromSubobjectWithoutError( lpView ) == 0 )
   {
   }
   return( 0 );
}

//./ ADD NAME=DropView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   DropView
//
//  PURPOSE: To Drop a view and the cursor positions on an object
//           instance. If the view is the only view for an object
//           instance, then the object instance itself is dropped.
//           This operation CAN NOT be used to drop a subtask view.
//           SfDropSubtask must be used to drop a subtask view.
//           However, if the object instance associated with a
//           subtask view needs to be dropped, it may be done
//           with the operation DropObjectInstance.
//
//  RETURNS: 0           - View successfully dropped
//           zCALL_ERROR - Invalid view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
DropViewTrace( zCPCHAR cpcMsg, zVIEW  lpView )
{
   LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );
//xif ( zstrcmp( lpViewOD->szName, "TZRPSRCO" ) == 0 )
//x{
      TraceLine( "DropView %s: 0x%08x   LOD: %s", cpcMsg, lpView, lpViewOD->szName );
//x}

   return( DropView( lpView ) );
}

zSHORT OPERATION
DropView( zVIEW  lpView )
{
   LPTASK      lpCurrentTask;
   zSHORT      nRC;

#ifdef DEBUG_VIEW

   zLONG  k;

   if ( lpView == 0 )
   {
      LPVIEWOD lpViewOD;

      TraceLine( "DropView Reporting View Usage ===============================> High Water Mark: %d", g_lHighWaterView );
      for ( k = 0; k < g_lHighWaterView; k++ )
      {
         if ( g_pvView[ k ] )
         {
            lpViewOD = zGETPTR( g_pvView[ k ] );
            TraceLine( "== %06d ============> View: 0x%08x   LOD: %s",
                       k, g_pvView[ k ], lpViewOD->szName );
         }
      }

   // g_lHighWaterView = -1;
      return( 0 );
   }

#endif

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iDropView, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( lpView->hSubtask )
   {
      //  "KZOEE108 - Attempt to drop Subtask View"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 108, 0, 0, 0 );
      nRC = zCALL_ERROR;
   }
   else
   {
#ifdef __ACTIVATE_CONSTRAINTS__
      LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

      // If we have a constraint for Dropping the OI, we need to
      // determine if we are the last view for this OI.  If so,
      // call the constraint operation.
      // Note: We drop the view no matter what the constraint operation
      // returns, so we don't check the return code.
      if ( lpViewOD->bDropOIConstraint )
      {
         LPVIEWCSR lpViewCsr      = zGETPTR( lpView->hViewCsr );
         LPVIEWOI  lpViewOI       = zGETPTR( lpViewCsr->hViewOI );
         LPVIEWCSR lpFirstViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );

         if ( lpFirstViewCsr->hNextViewCsr == 0 )
         {
            zSHORT nState = 0;

            if ( lpCurrentTask->bShutdown )
               nState += zOCE_STATE_SHUTDOWN;

            fnInvokeOCEOperation( lpView, lpCurrentTask,
                                  zOCE_DROPOI, nState );
            lpViewOI->bDropConstraintInvoked = TRUE;
         }
      }
#endif
      nRC = fnDropView( lpView ); // blew on this line 2006.07.03 dks1
   }

   fnOperationReturn( iDropView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfGetFirstSubtaskView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetFirstSubtaskView
//
//  PURPOSE:    To return the subtask view to the first subtask in the task
//
//  RETURNS:    0           - Subtask view returned
//              1           - Subtask not found
//              zCALL_ERROR - Task disabled
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfGetFirstSubtaskView( zPVIEW pvReturnSubtask, zVIEW vTaskView )
{
   LPTASK      lpCurrentTask;
   LPSUBTASK   lpSubtask;
   zSHORT      nRC;

   *pvReturnSubtask = 0;

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfGetFirstSubtaskView, vTaskView, 0 )) == 0 )
      return( zCALL_ERROR );

   // Get first subtask.
   lpSubtask = zGETPTR( lpCurrentTask->hFirstSubtask );
   if ( lpSubtask )
   {
      nRC = 0;
      *pvReturnSubtask = zGETPTR( lpSubtask->hSubtaskView );
   }
   else
   {
      nRC = 1;
   }

   fnOperationReturn( iSfGetFirstSubtaskView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfGetNextSubtaskView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetNextSubtaskView
//
//  PURPOSE:    To return the view to the next subtask in the task
//              given a view to a current subtask
//
//  RETURNS:    0           - Subtask view returned
//              1           - Subtask not found
//              zCALL_ERROR - Task disabled or view passed is not a
//                            subtask view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfGetNextSubtaskView( zPVIEW  pvReturnSubtask, zVIEW  lpSubtaskView )
{
   LPTASK      lpCurrentTask;
   LPSUBTASK   lpNextSubtask;
   LPSUBTASK   lpSubtask;
   zSHORT      nRC;

   *pvReturnSubtask = 0;

   // If task not active or disabled, or subtask view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfGetNextSubtaskView, lpSubtaskView,
                                          zVALID_SUBTASK_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Get next subtask view based on the current subtask.
   lpSubtask     = zGETPTR( lpSubtaskView->hSubtask );
   lpNextSubtask = zGETPTR( lpSubtask->hNextSubtask );
   if ( lpNextSubtask )
   {
      nRC = 0;
      *pvReturnSubtask = zGETPTR( lpNextSubtask->hSubtaskView );
   }
   else
   {
      nRC = 1;
   }

   fnOperationReturn( iSfGetNextSubtaskView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfSetApplicationTask
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfSetApplicationTask
//
//  PURPOSE:    To set a task as the application task.
//
//  PARAMETERS:
//              lpView    - View from which to get task/app.
//              lFlags    - Reserved ... should be zero.
//
//  RETURNS:    0           - Application task successfully set.
//              zCALL_ERROR - error setting application task.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfSetApplicationTask( zVIEW lpTaskView, zLONG lFlags )
{
   LPAPP     lpApp;
   LPTASK    lpTask;
   zSHORT    nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( lFlags != 0 ||
        (lpTask = fnOperationCall( iSfSetApplicationTask, lpTaskView,
                                   zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpApp = zGETPTR( lpTask->hApp );
   if ( lpApp->hApplicationTask == 0 )
   {
      TraceLine( "SfSetApplicationTask for App: %s   Task: 0x%08x",
                 lpApp->szName, lpTaskView->hTask );
      lpApp->hApplicationTask = lpTaskView->hTask;
      lpTask->bApplicationTask = TRUE;
      nRC = 0;
   }
   else
   {
      nRC = zCALL_ERROR;
   }

   fnOperationReturn( iSfSetApplicationTask, lpTask );
   return( nRC );
}

//./ ADD NAME=SfSetUserIdForTask
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfSetUserIdForTask
//
//  PURPOSE:    To associate a user id to the task.
//
//  PARAMETERS:
//              lpView    - View from which to get task.
//              cpcUserID - User Id to associate with task.
//
//  RETURNS:    0           - User Id successfully set for task.
//              zCALL_ERROR - error setting User Id.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfSetUserIdForTask( zVIEW lpTaskView, zCPCHAR cpcUserId )
{
   LPTASK    lpTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpTask = fnOperationCall( iSfSetUserIdForTask, lpTaskView,
                                   zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpTask->szUserId[ 0 ] = 0;
   strncat_s( lpTask->szUserId, sizeof( lpTask->szUserId ), cpcUserId, sizeof( lpTask->szUserId ) - 1 );

   fnOperationReturn( iSfSetUserIdForTask, lpTask );
   return( 0 );
}

//./ ADD NAME=SfGetUserIdForTask
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetUserIdForTask
//
//  PURPOSE:    To get the User Id associated with the task.
//
//  PARAMETERS:
//              lpView    - View from which to get task.
//              cpcUserID - User Id associated with task.
//
//  RETURNS:    0           - User Id for task successfully returned.
//              zCALL_ERROR - error getting User Id.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfGetUserIdForTask( zVIEW lpTaskView, zPCHAR pchUserId )
{
   LPTASK    lpTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpTask = fnOperationCall( iSfGetUserIdForTask, lpTaskView,
                                   zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   strcpy_s( pchUserId, sizeof( lpTask->szUserId ), lpTask->szUserId );

   fnOperationReturn( iSfGetUserIdForTask, lpTask );
   return( 0 );
}

//./ ADD NAME=SfTransferView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfTransferView
//
//  PURPOSE:    To transfer the scope of a view from the task to either the
//              Application scope or System task scope.
//              View parameter is optional and will be verified with the
//              name if passed.  Otherwise, the name will be dropped
//              regardless of the view it points to.
//
//  PARAMETERS:
//              lpView        - View that will be transferred.
//              lpAppQualView - View that qualifies the task.
//              nScope        - Specifies whether to move view to
//                              Application object or System task.
//
//  RETURNS:    0           - View successfully transferred.
//              zCALL_ERROR - error transferring views.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SfTransferView( zVIEW  lpView,
                zVIEW  lpAppQualView,
                zSHORT nScope )
{
   LPTASK      lpCurrentTask;
   zPVIEW      plpFirstView;
   int         i = 0;
   i = 1 / i;  // this should never be called ... dks 2006.07.10

#if 1
   if ( lpView && lpAppQualView && lpAppQualView->hTask != lpView->hTask )
   {
      LPVIEWOD    lpViewOD = zGETPTR( lpView->hViewOD );
      TraceLine( "SfTransferView trying to transfer view (0x%08x %s) from Task: "
                    "0x%08x to Task: 0x%08x at Scope: %s",
                 lpView, lpViewOD->szName, lpView->hTask,
                 lpAppQualView ? lpAppQualView->hTask : 0,
                 nScope == zLEVEL_APPLICATION ? "APPLICATION" :
                 nScope == zLEVEL_SYSTEM ? "SYSTEM" : "SCOPE UNKNOWN" );
   }
#endif

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfTransferView, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Verify validity of lpAppQualView if it was supplied.
   if ( (lpAppQualView || nScope == zLEVEL_APPLICATION) &&
        fnValidView( lpCurrentTask, lpAppQualView ) == 0 )
   {
      fnOperationReturn( iSfTransferView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Check validity of nScope.
   if ( nScope != zLEVEL_APPLICATION && nScope != zLEVEL_SYSTEM )
   {
      // Invalid scope parm passed.
      // "KZOEE110 - Invalid level for View name"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 110, (zLONG) nScope, 0, 0 );
      fnOperationReturn( iSfTransferView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // If view is a subtask (hSubtask has been set), then transfer is
   // invalid--return zCALL_ERROR.
   if ( lpView->hSubtask )
   {
      // "KZOEE100 - Invalid View, view is a Subtask View"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 100, 0, 0, 0 );
      fnOperationReturn( iSfTransferView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // View cannot be connected to a valid object instance.  Check to see if
   // the cursor pointer points to anything--if it does, then the view has an
   // object instance and cannot be transferred.
   if ( lpView->hViewCsr )
   {
      LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

      // "KZOEE113 - Invalid View, view contains an instance"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 113, 0, lpViewOD->szName, 0 );
      fnOperationReturn( iSfTransferView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // We have validated everything ... for sure the level is APPLICATION or
   // SYSTEM when we get here.

   // Remove view from task or application.  First set the pointer to point
   // to the lpFirstView pointer in either the application structure or the
   // task structure depending on lpView->bApplicationView.
   if ( lpView->bApplicationView )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      LPAPP  lpApp  = zGETPTR( lpTask->hApp );

      plpFirstView = &(lpApp->hFirstView);
   }
   else
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );

      plpFirstView = &(lpTask->hFirstView);
   }

   // Remove lpView from the view chain.
   if ( *plpFirstView == zGETHNDL( lpView ) )
      *plpFirstView = lpView->hNextView;
   else
   {
      zVIEW  lpSearchView;
      zVIEW  hView = zGETHNDL( lpView );

      lpSearchView = zGETPTR( *plpFirstView );
      while ( lpSearchView && lpSearchView->hNextView != hView )   // DKS 2003.12.17
         lpSearchView = zGETPTR( lpSearchView->hNextView );

      if ( lpSearchView )
         lpSearchView->hNextView = lpView->hNextView;
   }

   // Add view to application object or system task.  If scope is an
   // application, then find the target application from the AppQualView via
   // the View object definition.
   if ( nScope == zLEVEL_APPLICATION )
   {
      // If the app qual view has a subtask, then move the source view to
      // application pointed to by the subtask.  If the app qual view doesn't
      // have a subtask, then find the application through the app qual view
      // object definition.
      LPAPP lpApp;

      fnGetApplicationForSubtask( &lpApp, lpAppQualView );

      lpView->hNextView = lpApp->hFirstView;
      lpApp->hFirstView = zGETHNDL( lpView );
      lpView->hTask = AnchorBlock->hMainTask;  // view is now part of main  // don't do this ... dks 2004.10.14
      lpView->bApplicationView = TRUE;         // indicate that scope is app
   }
   else
// if ( nScope == zLEVEL_SYSTEM )  must be true
   {
      LPTASK lpMainTask = zGETPTR( AnchorBlock->hMainTask );

      lpView->hNextView = lpMainTask->hFirstView;
      lpMainTask->hFirstView = zGETHNDL( lpView );
      lpView->hTask = AnchorBlock->hMainTask;  // view is now part of main
      lpView->bApplicationView = FALSE;        // indicate that scope is sys
   }

   if ( lpView->hNextView == zGETHNDL( lpView ) )
   {
      zCHAR szDebug[ 2 ];

      SysReadZeidonIni( -1, "[Debug]", "BombZDr", szDebug, sizeof( szDebug ) );
      if ( szDebug[ 0 ] == 'Y' )
         SysMessageBox( 0, "Houston ... we have a problem", "Recursive view", -1 );
   }

   // If execution has fallen this far, then everything must have worked.
   fnOperationReturn( iSfTransferView, lpCurrentTask );
   return( 0 );
}  // SfTransferView

//./ ADD NAME=SfLockView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfLockView
//
//  PURPOSE:    To mark a view as 'LOCKED', so that during the deletion
//              of a subtask, the view will not be dropped, even if the
//              nViewNames == 0.
//
//  PARAMETERS:
//              lpView - View that will be lock.
//
//  RETURNS:    0           - View successfully locked.
//              zCALL_ERROR - error on call.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SfLockView( zVIEW lpView )
{
   LPTASK      lpCurrentTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfLockView, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Mark the view as being locked.
   lpView->bViewLocked = TRUE;

   // If execution has fallen this far then everything must have worked.
   fnOperationReturn( iSfLockView, lpCurrentTask );
   return( 0 );

}  // SfLockView

/////////////////////////////////////////////////////////////////////////////
//
//   Local Functions for view processing
//
/////////////////////////////////////////////////////////////////////////////

zVOID fnCallListeners( zVIEW lpView, zCPCHAR cpcEntityName );

//./ ADD NAME=fnDropView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDropView
//
//  PURPOSE:    To drop a View Object. If the view object is connected to a
//              view cursor object, then the view csr object is deleted. If
//              the view cursor object is dropped and the viewOI has no more
//              view cursors, then the view oi is dropped.  Note:  if the
//              view is a subtask view, then the view is left on the task
//              view chain and can only be removed by a call to SfDropSubtask.
//
//  RETURNS:    0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
fnDropView( zVIEW  lpView )
{
   LPTASK      hOwningTask;
   LPTASK      lpOwningTask;
   LPAPP       lpOwningApp;
   LPSUBTASK   lpSubtask;
   LPVIEWCSR   lpViewCsr;
   LPVIEWNAME  lpViewName;
   LPVIEWNAME  lpPrevViewName;
   LPVIEWNAME  lpNextViewName;
   LPVIEWOD    lpViewOD;
   zPVOID      hView = zGETHNDL( lpView );

#ifdef DEBUG_VIEW

   lpViewOD = zGETPTR( lpView->hViewOD );
   if ( lpViewOD && zstrcmpi( lpViewOD->szName, "TZ__PRFO" ) == 0 )
   {
      zLONG  k;

      TraceLine( "fnDropView: 0x%08x   LOD: %s", lpView, lpViewOD ? lpViewOD->szName : "???" );

      for ( k = 0; k < g_lHighWaterView; k++ )
      {
         if ( g_pvView[ k ] == (zPVOID) lpView )
         {
            g_pvView[ k ] = 0;
            break;
         }
      }

      if ( k >= zMAX_VIEW_TEST )
      {
         TraceLine( "DropView Error ... View Not Found: 0x%08x", lpView );
      }
   }

#endif

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   hOwningTask = lpView->hTask;
   lpOwningTask = zGETPTR( hOwningTask );

   // Remove any cursor listeners.
   fnCallListeners( lpView, 0 ); // first call to inform of DropView
   while ( lpView->hFirstCursorListener )
   {
      LPCURSORLISTENER lpCsrListener = zGETPTR( lpView->hFirstCursorListener );

      lpView->hFirstCursorListener = lpCsrListener->hNextCursorListener;
      fnFreeDataspace( lpCsrListener );
   }

   // Check to see if the user wants to pop up a warning when a view is dropped.
   if ( lpView->hViewOD &&
        (lpView->bFlagDropView || lpView->bTraceDropView) )
   {
      zCHAR    szMsg[ 200 ];
      zPCHAR   pch = ".Subtask.";

      if ( lpView->hViewOD )
      {
         LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );
         pch = lpViewOD->szName;
      }

      sprintf_s( szMsg, sizeof( szMsg ), "View about to be dropped (0x%08x). Object Name = %s",
                (zULONG) lpView, pch );
      TraceLineS( "(vm) ", szMsg );
      if ( lpView->bFlagDropView )
         SysMessageBox( lpView, "Core Warning", szMsg, 0 );
   }

   if ( lpViewCsr )
      fnDropViewCsr( lpViewCsr ); //   // dks 2006.09.18 error while cleaning up client OE

   // If the view is the "original" view as part of optimistic locking, then
   // we need to flag the "main" OI that the "original" view has been dropped.
   // Same thing with qualification view.
   if ( lpView->bOrigView )
   {
      if ( lpView->bInvalidMainOI == FALSE )
      {
         LPVIEWOI lpMainOI = zGETPTR( lpView->hMainOI );

         lpMainOI->vOriginalOI = 0;
         lpView->bOrigView = FALSE;
      }
      else
         SysMessageBox( lpView, "fnDropView Error", "MainOI Invalid", 0 );
   }
   else
   if ( lpView->bQualView )
   {
      if ( lpView->bInvalidMainOI == FALSE )
      {
         LPVIEWOI lpMainOI = zGETPTR( lpView->hMainOI );

         lpMainOI->vQualOI = 0;
         lpView->bQualView = FALSE;
      }
      else
         SysMessageBox( lpView, "fnDropView Error", "Unexpected Invalid MainOI", 0 );
   }

   // If the view is a subtask view, do not unchain it NOR remove it
   // from the task chain, simply NULL the ViewCsr pointer.
   if ( lpView->hSubtask )
   {
      lpView->hViewCsr = 0;
      return( 0 );
   }

   // Drop all names for the view 1: first go through all the subtasks and
   // find all names for the view at any subtask level within the task.
   if ( lpView->nViewNames )
   {
      lpSubtask = zGETPTR( lpOwningTask->hFirstSubtask );
      while ( lpSubtask )
      {
         lpPrevViewName = 0;
         lpViewName = zGETPTR( lpSubtask->hFirstViewName );
         while ( lpViewName )
         {
            lpNextViewName = zGETPTR( lpViewName->hNextViewName );
            if ( lpViewName->hView == hView )
            {
               if ( lpPrevViewName )
                  lpPrevViewName->hNextViewName = zGETHNDL( lpNextViewName );
               else
                  lpSubtask->hFirstViewName = zGETHNDL( lpNextViewName );

               lpView->nViewNames--;
               fnFreeDataspace( lpViewName );
               if ( lpView->nViewNames == 0 )
                  break;
            }
            else
               lpPrevViewName = lpViewName;

            lpViewName = lpNextViewName;
         }

         if ( lpView->nViewNames == 0 )
            break;

         lpSubtask = zGETPTR( lpSubtask->hNextSubtask );
      }
   }

   // Drop all names for the view 2: if a name was created at the task
   // level, go through all names for the owning task and delete any
   // names found for the current view.
   if ( lpView->nViewNames && lpView->bTaskNamed )
   {
      lpPrevViewName = 0;
      lpViewName = zGETPTR( lpOwningTask->hFirstViewName );
      while ( lpViewName )
      {
         lpNextViewName = zGETPTR( lpViewName->hNextViewName );
         if ( lpViewName->hView == hView )
         {
            if ( lpPrevViewName )
               lpPrevViewName->hNextViewName = zGETHNDL( lpNextViewName );
            else
               lpOwningTask->hFirstViewName = zGETHNDL( lpNextViewName );

            lpView->nViewNames--;
            fnFreeDataspace( lpViewName );
            if ( lpView->nViewNames == 0 )
               break;
         }
         else
            lpPrevViewName = lpViewName;

         lpViewName = lpNextViewName;
      }
   }

   // Drop all names for the view 3: if a name was created at the
   // application level, go through all names for the application
   // and delete any names found for the current view.
   if ( lpView->nViewNames && lpView->bApplicationNamed )
   {
      if ( lpView->hSubtask )
      {
         LPSUBTASK lpSubtask = zGETPTR( lpView->hSubtask );

         lpOwningApp = zGETPTR( lpSubtask->hApp );
      }
      else
      if ( lpView->hViewOD )
      {
         LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

         lpOwningApp = zGETPTR( lpViewOD->hApp );
      }
      else
         lpOwningApp = zGETPTR( lpOwningTask->hApp );

      lpPrevViewName = 0;
      lpViewName = zGETPTR( lpOwningApp->hFirstViewName );
      while ( lpViewName )
      {
         lpNextViewName = zGETPTR( lpViewName->hNextViewName );
         if ( lpViewName->hView == hView )
         {
            if ( lpPrevViewName )
               lpPrevViewName->hNextViewName = zGETHNDL( lpNextViewName );
            else
               lpOwningApp->hFirstViewName = zGETHNDL( lpNextViewName );

            lpView->nViewNames--;
            fnFreeDataspace( lpViewName );
            if ( lpView->nViewNames == 0 )
               break;
         }
         else
            lpPrevViewName = lpViewName;

         lpViewName = lpNextViewName;
      }
   }

   // Drop all names for the view 4: if a name was created at
   // the system level, go through all names for the system
   // and delete any names found for the current view.
   if ( lpView->nViewNames && lpView->bSystemNamed )
   {
      zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      lpPrevViewName = 0;
      lpViewName = zGETPTR( AnchorBlock->hFirstViewName );
      while ( lpViewName )
      {
         lpNextViewName = zGETPTR( lpViewName->hNextViewName );
         if ( lpViewName->hView == hView )
         {
            if ( lpPrevViewName )
               lpPrevViewName->hNextViewName = zGETHNDL( lpNextViewName );
            else
               AnchorBlock->hFirstViewName = zGETHNDL( lpNextViewName );

            lpView->nViewNames--;
            fnFreeDataspace( lpViewName );
            if ( lpView->nViewNames == 0 )
               break;
         }
         else
            lpPrevViewName = lpViewName;

         lpViewName = lpNextViewName;
      }

      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   }

   // Drop all names for the view 5: if a name was created at the task
   // level, go through all tasks other than the owning task and see
   // if the view can be found in one of them.
   if ( lpView->nViewNames && lpView->bTaskNamed )
   {
      LPTASK lpSearchTask;

      // Make sure nobody else is deleting a task.
      fnStartBrowseOfTaskList( lpView->hTask, FALSE );

      lpSearchTask = zGETPTR( AnchorBlock->hFirstTask );
      while ( lpSearchTask )
      {
         if ( lpSearchTask != lpOwningTask )
         {
            lpPrevViewName = 0;
            lpViewName = zGETPTR( lpSearchTask->hFirstViewName );
            while ( lpViewName )
            {
               lpNextViewName = zGETPTR( lpViewName->hNextViewName );
               if ( lpViewName->hView == hView )
               {
                  if ( lpPrevViewName )
                     lpPrevViewName->hNextViewName = zGETHNDL( lpNextViewName );
                  else
                     lpSearchTask->hFirstViewName = zGETHNDL( lpNextViewName );

                  lpView->nViewNames--;
                  fnFreeDataspace( lpViewName );
                  if ( lpView->nViewNames == 0 )
                     break;
               }
               else
                  lpPrevViewName = lpViewName;

               lpViewName = lpNextViewName;
            }
         }
         if ( lpView->nViewNames == 0 )
            break;

         lpSearchTask = zGETPTR( lpSearchTask->hNextTask );
      }

      fnEndBrowseOfTaskList( FALSE );
   }

   // Drop all names for the view 6: if a name still exists at this
   // point, then it must exist in a subtask for another task, search
   // all subtasks in the other tasks for a name for this view.
   if ( lpView->nViewNames )
   {
      LPTASK lpSearchTask;

      // Make sure nobody else is deleting a task.
      fnStartBrowseOfTaskList( lpView->hTask, FALSE );

      lpSearchTask = zGETPTR( AnchorBlock->hFirstTask );
      while ( lpSearchTask )
      {
         if ( lpSearchTask != lpOwningTask )
         {
            lpSubtask = zGETPTR( lpSearchTask->hFirstSubtask );
            while ( lpSubtask )
            {
               lpPrevViewName = 0;
               lpViewName = zGETPTR( lpSubtask->hFirstViewName );
               while ( lpViewName )
               {
                  lpNextViewName = zGETPTR( lpViewName->hNextViewName );
                  if ( lpViewName->hView == hView )
                  {
                     if ( lpPrevViewName )
                        lpPrevViewName->hNextViewName = zGETHNDL( lpNextViewName );
                     else
                        lpSubtask->hFirstViewName = zGETHNDL( lpNextViewName );

                     lpView->nViewNames--;
                     fnFreeDataspace( lpViewName );
                     if ( lpView->nViewNames == 0 )
                        break;
                  }
                  else
                     lpPrevViewName = lpViewName;

                  lpViewName = lpNextViewName;
               }

               if ( lpView->nViewNames == 0 )
                  break;

               lpSubtask = zGETPTR( lpSubtask->hNextSubtask );
            }
         }

         if ( lpView->nViewNames == 0 )
            break;

         lpSearchTask = zGETPTR( lpSearchTask->hNextTask );
      }

      fnEndBrowseOfTaskList( FALSE );
   }

   // Drop all names for the view 7: if a name was created at the
   // application level, go through all apps other than the owning
   // app and see if the view can be found in one of them.
   if ( lpView->nViewNames && lpView->bApplicationNamed )
   {
      LPAPP  lpSearchApp;

      zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      lpSearchApp = zGETPTR( AnchorBlock->hFirstApp );
      while ( lpSearchApp )
      {
         if ( lpSearchApp != lpOwningApp )
         {
            lpPrevViewName = 0;
            lpViewName = zGETPTR( lpSearchApp->hFirstViewName );
            while ( lpViewName )
            {
               lpNextViewName = zGETPTR( lpViewName->hNextViewName );
               if ( lpViewName->hView == hView )
               {
                  if ( lpPrevViewName )
                     lpPrevViewName->hNextViewName = zGETHNDL( lpNextViewName );
                  else
                     lpSearchApp->hFirstViewName = zGETHNDL( lpNextViewName );

                  lpView->nViewNames--;
                  fnFreeDataspace( lpViewName );
                  if ( lpView->nViewNames == 0 )
                     break;
               }
               else
                  lpPrevViewName = lpViewName;

               lpViewName = lpNextViewName;
            }
         }

         if ( lpView->nViewNames == 0 )
            break;

         lpSearchApp = zGETPTR( lpSearchApp->hNextApp );
      }

      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   }

   // Whew, end of view name cleanup code.  If there is still a name here,
   // issue an internal name cleanup error.
   if ( lpView->nViewNames )
      SysMessageBox( lpView, "Error", "(vm) Name cleanup error", 1 );

   // Unchain the view from its view chain, either in the task or app.
   //
   // Check to see if view is part of application chain and unchain it
   // if it is.
   if ( lpView->bApplicationView )
   {
      LPAPP  lpSearchApp;
      zBOOL  bDone = 0;

      zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      lpSearchApp = zGETPTR( AnchorBlock->hFirstApp );
      while ( lpSearchApp && bDone == FALSE )
      {
         zVIEW  lpSearchView;

         if ( lpSearchApp->hFirstView == hView )
         {
            // Unchain view from view chain.
            lpSearchApp->hFirstView = lpView->hNextView;
            bDone = TRUE;
         }
         else
         {
            lpSearchView = zGETPTR( lpSearchApp->hFirstView );
            if ( lpSearchView )
            {
               while ( lpSearchView->hNextView && bDone == FALSE )
               {
                  if ( lpSearchView->hNextView == hView )
                  {
                     // Unchain view from view chain.
                     lpSearchView->hNextView = lpView->hNextView;
                     bDone = TRUE;
                  }
                  else
                     lpSearchView = zGETPTR( lpSearchView->hNextView );
               }
            }

            lpSearchApp = zGETPTR( lpSearchApp->hNextApp );
         }
      }  // while...

      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   } // if ( lpView->bApplicationView )...
   else
   {
      if ( hOwningTask == AnchorBlock->hMainTask )
         zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      // View is not part of application chain ... unchain from owning task.
      if ( lpOwningTask->hFirstView == hView )
         lpOwningTask->hFirstView = lpView->hNextView;
      else
      {
         zVIEW  lpSearchView;

         // Repair the view chain.
         for ( lpSearchView = zGETPTR( lpOwningTask->hFirstView );
               lpSearchView && lpSearchView->hNextView != hView;
               lpSearchView = zGETPTR( lpSearchView->hNextView ) )
         {
            // Nothing needs to be done here.
         }

         if ( lpSearchView )
            lpSearchView->hNextView = lpView->hNextView;
      }

      if ( hOwningTask == AnchorBlock->hMainTask )
         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   }  // if ( lpView->bApplicationView )...else...

   lpViewOD = zGETPTR( lpView->hViewOD );

   // If the ViewOD is flagged as deprecated, let's see if we can remove it.
   if ( lpViewOD && lpViewOD->bDeprecated &&
        fnCheckIfViewOD_IsUsed( lpView, lpViewOD ) == 0 )
   {
      // View is not used by any tasks so it's safe to remove it.
      zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      fnDeleteViewObject ( lpViewOD );
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   }

   // Finally, free the dataspace for the view.
   fnFreeDataspace( lpView );
   return( 0 );
}

//./ ADD NAME=fnSetViewToSubobject
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnSetViewToSubobject
//
//  PURPOSE: To set a view to a subobject entity. If the subobject
//           entity is defined to behave recursively, then the
//           instance leveling is adjusted and the entity instance
//           at the target entity is moved up to the recursive
//           parent and will then be referenced by the recursive
//           parent's name. Otherwise, the entity types visible in
//           the view are refined to only include the target
//           entity and its subordinates.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnSetViewToSubobject( zVIEW           lpView,
                      LPVIEWENTITY    lpViewEntity,
                      LPVIEWENTITYCSR lpViewEntityCsr )
{
   zSHORT            nLevelChg;
   LPTASK            lpOwningTask;
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITYCSR   lpViewRootParentCsr;
   LPVIEWENTITYCSR   lpRecursParentCsr;
   LPVIEWENTITYCSR   lpLastDescCsr;
   LPVIEWSUBOBJECT   lpViewSubobject;
   LPVIEWSUBOBJECT   lpPrevViewSubobject;
   zPVOID            hViewSubobject;
   LPVIEWENTITY      lpWorkViewEntity;

   // Following handles are declared as pointers instead of zPVOID so that
   // we can use pointer arithmetic on them.
   LPVIEWENTITYCSR   hViewEntityCsr;

   // Get the ViewCsr associated with the view
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   // Get the instance owning task so we can do allocates
   lpOwningTask = zGETPTR( lpViewOI->hTask );

   if ( lpViewEntityCsr == 0 )
   {
      lpViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
      lpViewEntityCsr += ( lpViewEntity->nHierNbr - 1 );
   }

   // Create a handle for the view entity csr.
   hViewEntityCsr = fnCreateEntityCsrHandle( lpViewEntityCsr, lpViewCsr, 0 );

   if ( lpViewCsr->hRootViewEntityCsr == hViewEntityCsr )
      return( 1 );

   if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
      fnEstablishCursorForView( lpViewEntityCsr );

   lpViewRootParentCsr = zGETPTR( lpViewEntityCsr->hParent );

   // We double check that the parent cursor does not need establishing
   // because create entity ( through another view ) might mark the
   // parent only as needing to be established.
   if ( lpViewRootParentCsr->hEntityInstance == UNSET_CSR )
      fnEstablishCursorForView( lpViewRootParentCsr );

   // Create a viewsubobject record for saving view switching info.
   hViewSubobject = fnAllocDataspace( lpOwningTask->hFirstDataHeader,
                                      sizeof( ViewSubobjectRecord ), 1, 0,
                                      iViewSubobject );
   lpViewSubobject = (LPVIEWSUBOBJECT) zGETPTR( hViewSubobject );
   if ( lpViewSubobject == 0 )
      return( zCALL_ERROR );

   // Next, look for a recursive parent to the entity to see if the
   // structure needs to be folded up recursively.
   lpRecursParentCsr = 0;
   if ( lpViewEntity->bRecursive )
   {
      lpPrevViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
      lpRecursParentCsr = lpViewRootParentCsr;
      while ( lpRecursParentCsr )
      {
         LPVIEWENTITY lpRecursParentEntity = zGETPTR( lpRecursParentCsr->hViewEntity );

         if ( lpRecursParentEntity->lEREntTok == lpViewEntity->lEREntTok &&
              lpRecursParentCsr->nLevel < lpViewEntityCsr->nLevel )
         {
            break;
         }
         else
         if ( lpRecursParentCsr == zGETPTR( lpViewCsr->hFirstOD_EntityCsr ) )
         {
            lpRecursParentCsr = 0;
            break;
         }
         else
         if ( lpRecursParentCsr->hParent )
         {
            lpRecursParentCsr = zGETPTR( lpRecursParentCsr->hParent );
         }
         else
         {
            lpRecursParentCsr = zGETPTR( lpPrevViewSubobject->hViewRootParentCsr );
            lpPrevViewSubobject = zGETPTR( lpPrevViewSubobject->hNextViewSubobject );
         }
      }
   }

   // Set view parent entity instance.
   lpViewCsr->hViewParentEntityInstance = lpViewRootParentCsr->hEntityInstance;
   lpViewSubobject->hViewParentEntityInstance = lpViewRootParentCsr->hEntityInstance;

   // Determine the level change for the recursive subobject.
   nLevelChg = 0;
   if ( lpRecursParentCsr ) // is it recursive
   {
      LPVIEWSUBOBJECT lpWorkViewSubobject;

      lpViewSubobject->hRecursParentEntityInstance = lpRecursParentCsr->hEntityInstance;
      lpViewSubobject->hRecursViewEntityCsr = hViewEntityCsr;
      lpViewSubobject->hRecursViewEntity = lpViewEntityCsr->hViewEntity;
      nLevelChg = lpViewEntityCsr->nLevel - lpRecursParentCsr->nLevel;
      lpRecursParentCsr->hEntityInstance = lpViewEntityCsr->hEntityInstance;
   // if ( lpRecursParentCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnSetViewToSubobject", "UNSET_CSR", -1 );

      lpViewEntityCsr = lpRecursParentCsr;
      lpViewRootParentCsr = zGETPTR( lpViewEntityCsr->hParent );
      hViewEntityCsr = fnCreateEntityCsrHandle( lpViewEntityCsr, lpViewCsr, 0 );

      // Before resetting cursor for children, capture the current
      // root cursor positions for outstanding subobject views which
      // are subordinate to the recursive parent cursor and re-do
      // any recursive level changes which are subordinate to the
      // parent cursor.
      lpWorkViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
      while ( lpWorkViewSubobject )
      {
         zSHORT          nLevelChg;
         LPVIEWENTITYCSR lpRootCsr;
         LPVIEWENTITYCSR lpWorkCsr;

         if ( lpWorkViewSubobject->nRootHierNbr <= lpRecursParentCsr->nHierNbr )
            break;

         // Get current entity instance for reset.
         lpRootCsr = zGETPTR( lpWorkViewSubobject->hRootCsr );
         lpWorkViewSubobject->hRootEntityInstance = lpRootCsr->hEntityInstance;
         nLevelChg = lpWorkViewSubobject->nLevelChg;
         if ( lpWorkViewSubobject->nLastDescHierNbr < lpViewCsr->uEntities )
         {
            lpWorkCsr = zGETPTR( lpWorkViewSubobject->hLastDescCsr );
            lpWorkCsr->hNextHier = fnCreateEntityCsrHandle( lpWorkCsr, lpViewCsr, 1 );
         }

         // Reset hierarchical and parent pointers.
         lpWorkCsr = zGETPTR( lpWorkViewSubobject->hRootCsr );
         lpWorkCsr->hPrevHier = fnCreateEntityCsrHandle( lpWorkCsr, lpViewCsr, -1 );
         lpWorkCsr->hParent = lpWorkViewSubobject->hViewRootParentCsr;
      // if ( lpWorkCsr->hParent == (LPVIEWENTITYCSR) 1 )
      //    SysMessageBox( 0, "fnSetViewToSubobject", "1", -1 );

         // Redo the leveling of the recursive subobject which is
         // itself a subobject of a parent recursive subobject.
         if ( nLevelChg )
         {
            LPVIEWENTITYCSR hWorkCsr;

            hWorkCsr = fnCreateEntityCsrHandle( lpWorkCsr, lpViewCsr, 0 );
            while ( hWorkCsr != lpWorkViewSubobject->hLastDescCsr )
            {
               lpWorkCsr->nLevel -= nLevelChg;
               lpWorkCsr++;
               hWorkCsr++;
            }

            lpWorkCsr->nLevel -= nLevelChg; // change level of last desc
         }

         lpWorkViewSubobject = zGETPTR( lpWorkViewSubobject->hNextViewSubobject );
      }

      if ( lpViewEntityCsr->hEntityInstance )
         fnResetCursorForViewChildren( lpViewEntityCsr );
      else
         fnNullCursorForViewChildren( lpViewEntityCsr );
   }

   // Continue setting subobject information for reset.
   lpWorkViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );
   lpViewSubobject->hRootCsr           = hViewEntityCsr;
   lpViewSubobject->nRootHierNbr       = lpWorkViewEntity->nHierNbr;
   lpViewSubobject->hViewRootParentCsr =
            fnCreateEntityCsrHandle( lpViewRootParentCsr, lpViewCsr, 0 );
   lpViewSubobject->nLevelChg = nLevelChg;

   // Set the new root view entity cursor.
   lpViewCsr->hRootViewEntityCsr = hViewEntityCsr;

   // Chain view subobject on to view csr.
   if ( lpViewCsr->hFirstViewSubobject )
   {
      LPVIEWSUBOBJECT lpFirstViewSubobject;

      lpViewSubobject->hNextViewSubobject = lpViewCsr->hFirstViewSubobject;
      lpFirstViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
      lpFirstViewSubobject->hPrevViewSubobject = hViewSubobject;
   }

   lpViewCsr->hFirstViewSubobject = hViewSubobject;

   // Now, go from the current view position to the last descendent
   // of the entity view and adjust the levels if necessary.
   // Note: we do this loop whether or not an adjustment is necessary
   //       in order to get a pointer to the last descendent.
   lpLastDescCsr = lpViewEntityCsr;
   for ( ; ; )
   {
      LPVIEWENTITYCSR lpNextHierCsr;

      lpLastDescCsr->nLevel += nLevelChg;
      lpNextHierCsr = zGETPTR( lpLastDescCsr->hNextHier );
      if ( lpNextHierCsr == 0 ||
           (lpNextHierCsr->hParent &&
            lpNextHierCsr->hParent == lpViewEntityCsr->hParent) )
      {
         break;
      }

      lpLastDescCsr = zGETPTR( lpLastDescCsr->hNextHier );
   }

   // Set last descendent information in lpViewSubobject.
   lpViewSubobject->hLastDescCsr =
                     fnCreateEntityCsrHandle( lpLastDescCsr, lpViewCsr, 0 );
   lpWorkViewEntity = zGETPTR( lpLastDescCsr->hViewEntity );
   lpViewSubobject->nLastDescHierNbr = lpWorkViewEntity->nHierNbr;

   // Null the previous hierarchical pointer and parent pointer from
   // the target entity.
   lpViewEntityCsr->hParent   = 0;
   lpViewEntityCsr->hPrevHier = 0;
   lpLastDescCsr->hNextHier   = 0;
   return( 0 );
}

//./ ADD NAME=fnResetView
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnResetView
//
//  PURPOSE: To reset a view to its initial state. All subobject
//           views are reset and the cursors are reset to the
//           first entity instance for each entity type.
//
//  PARAMETERS:
//           lpView   - das view
//           nControl - 0, fnResetView functions same as before this
//                         control field was added.
//                      1, indicates to not clear any SelectedInstance
//                         indicators associated with this view
//
//  RETURNS: 0           - View successfully reset
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnResetView( zVIEW  lpView, zSHORT  nControl )
{
   LPVIEWCSR           lpViewCsr;
   LPVIEWOI            lpViewOI;
   LPVIEWOD            lpViewOD;
   LPVIEWENTITY        lpViewEntity;
   LPVIEWENTITYCSR     lpViewEntityCsr;
   LPENTITYINSTANCE    lpRootEntityInstance;
   LPSELECTEDINSTANCE  lpSelectedInstance;

   // Get View Cursors for instance.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   lpViewOD  = zGETPTR( lpViewOI->hViewOD );
   lpViewCsr->hHierRootEntityInstance = 0;
   lpViewCsr->hHierEntityInstance = 0;

   // If the view has any outstanding subobjects, delete the subobject
   // entries and reset the hierarchical and parent pointers in the
   // view cursors as well as the levels.
   if ( lpViewCsr->hFirstViewSubobject )
   {
      while ( lpViewCsr->hFirstViewSubobject )
      {
         LPVIEWSUBOBJECT lpViewSubobject;

         lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
         lpViewCsr->hFirstViewSubobject =
                                    lpViewSubobject->hNextViewSubobject;
         fnFreeDataspace( lpViewSubobject );
      }

      // Now that we've blasted the subobject information, reset the
      // hierarchical pointers and parent pointers in the view cursors.
      lpViewEntity    = zGETPTR( lpViewOD->hFirstOD_Entity );
      lpViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
      while ( lpViewEntity )
      {
         lpViewEntityCsr->nLevel = lpViewEntity->nLevel;
         if ( lpViewEntity->hParent )
            lpViewEntityCsr->hPrevHier =
               fnCreateEntityCsrHandle( lpViewEntityCsr, lpViewCsr, -1 );

         if ( lpViewEntity->hNextHier )
            lpViewEntityCsr->hNextHier =
               fnCreateEntityCsrHandle( lpViewEntityCsr, lpViewCsr, 1 );

         lpViewEntity = zGETPTR( lpViewEntity->hNextHier );
         lpViewEntityCsr++;
      }

      // Now reset the parent cursor pointers.
      lpViewEntityCsr = zGETPTR( lpViewCsr->hFirstOD_EntityCsr );
      lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
      while ( lpViewEntityCsr )
      {
         if ( lpViewEntityCsr->hParent == 0 )
         {
            LPVIEWENTITYCSR  lpParentCsr;

            lpParentCsr = lpViewEntityCsr - 1;
            while ( lpParentCsr->nLevel != lpViewEntityCsr->nLevel - 1 )
               lpParentCsr--;

            lpViewEntityCsr->hParent = fnCreateEntityCsrHandle( lpParentCsr, lpViewCsr, 0 );
         // if ( lpViewEntityCsr->hParent == (LPVIEWENTITYCSR) 1 )
         //    SysMessageBox( 0, "fnResetView", "1", -1 );
         }

         lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
      }

      lpViewCsr->hRootViewEntityCsr = lpViewCsr->hFirstOD_EntityCsr;
      lpViewCsr->hViewParentEntityInstance = 0;
   }

   // Clear any and all selected instances, unless asked not to.
   if ( (nControl & 1) == 0 )
   {
      while ( lpViewCsr->hFirstSelectedInstance )
      {
         lpSelectedInstance = zGETPTR( lpViewCsr->hFirstSelectedInstance );
         lpViewCsr->hFirstSelectedInstance =
            lpSelectedInstance->hNextSelectedInstance;

         fnFreeDataspace( lpSelectedInstance );
      }
   }

   // Reset the cursor for all the view children from the root.
   lpRootEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );
   if ( lpRootEntityInstance )
   {
      lpViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
      while ( lpViewEntityCsr->hViewEntity !=
                                          lpRootEntityInstance->hViewEntity )
      {
         lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
      }

      lpViewEntityCsr->hEntityInstance = zGETHNDL( lpRootEntityInstance );
   // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnResetView", "UNSET_CSR", -1 );

      fnResetCursorForViewChildren( lpViewEntityCsr );
   }

   return( 0 );  // we're done, return success
}

//./ ADD NAME=fnResetViewFromSubobject
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnResetViewFromSubobject
//
//  PURPOSE:    To reset a subobject view
//
//  RETURNS:    0 - View successfully reset
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnResetViewFromSubobject( zVIEW  lpView )
{
   zSHORT            nLevelChg;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITYCSR   lpRootCsr;
   LPVIEWENTITYCSR   lpWorkCsr;
   LPVIEWENTITYCSR   lpLastDescCsr;
   LPVIEWSUBOBJECT   lpViewSubobject;

   // Get the ViewCsr for the view.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpRootCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
   lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
   if ( lpViewSubobject == 0 )
      return( 1 );

   lpLastDescCsr = zGETPTR( lpViewSubobject->hLastDescCsr );

   // Check to see if there is another subobject structure.  If there is then
   // the current cursor structure won't change, just the entity instances.
   if ( lpViewSubobject->hNextViewSubobject == 0 )
   {
      // No other subobject structures so change the cursor structure back to
      // its original form.
      lpRootCsr->hParent = lpViewSubobject->hViewRootParentCsr;
   // if ( lpRootCsr->hParent == (LPVIEWENTITYCSR) 1 )
   //    SysMessageBox( 0, "fnResetViewFromSubobject", "1", -1 );

      lpRootCsr->hPrevHier = fnCreateEntityCsrHandle( lpRootCsr, lpViewCsr, -1 );
      if ( lpViewSubobject->nLastDescHierNbr < lpViewCsr->uEntities )
         lpLastDescCsr->hNextHier = fnCreateEntityCsrHandle( lpLastDescCsr, lpViewCsr, 1 );

      // Reset RootViewCsr parent.
      lpWorkCsr = lpRootCsr;
      while ( lpWorkCsr->hParent )
         lpWorkCsr = zGETPTR( lpWorkCsr->hParent );

      lpViewCsr->hRootViewEntityCsr = fnCreateEntityCsrHandle( lpWorkCsr, lpViewCsr, 0 );
   }
   // DGC 2000.1.31
   // Following code just for testing a theory ... can be removed at any time.
   else
   {
      LPVIEWSUBOBJECT lpNextViewSubobject = zGETPTR( lpViewSubobject->hNextViewSubobject );

      if ( lpNextViewSubobject->hRootCsr != lpViewSubobject->hRootCsr )
      {
         // We should never see this message.  The root cursors should always
         // be the same.
         SysDiagnosticMessage( lpView, "QuinSoft Internal Message",
                               "Root cursors don't match.  Contact QuinSoft", 0 );
      }
   }

   // Get level change.
   nLevelChg = lpViewSubobject->nLevelChg;
   if ( nLevelChg )
   {
      LPVIEWSUBOBJECT  lpWorkViewSubobject;
      LPVIEWENTITYCSR  lpRecursChildEntityCsr;
      LPENTITYINSTANCE lpRecursChildEntityInstance;
      zPVOID           hRecursChildEntityInstance;

      lpWorkCsr = lpRootCsr;
      lpWorkCsr->nLevel -= nLevelChg;
      while ( lpWorkCsr != lpLastDescCsr )
      {
         lpWorkCsr++;
         lpWorkCsr->nLevel -= nLevelChg;
      }

      // Reset the recursive parent entity cursor to the recursive
      // parent entity instance when SetViewToSubobject was called.
      lpRecursChildEntityInstance = zGETPTR( lpRootCsr->hEntityInstance );
      lpRootCsr->hEntityInstance = lpViewSubobject->hRecursParentEntityInstance;
   // if ( lpRootCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnResetViewFromSubobject", "UNSET_CSR", -1 );

      fnResetCursorForViewChildren( lpRootCsr );

      // Now reset the recursive childe entity cursor to the recursive
      // parents cursor value when the reset was called. Note: this
      // entity instance may be NULL.
      lpRecursChildEntityCsr = zGETPTR( lpViewSubobject->hRecursViewEntityCsr );
      lpRecursChildEntityCsr->hEntityInstance = zGETHNDL( lpRecursChildEntityInstance );
   // if ( lpRecursChildEntityCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnResetViewFromSubobject2", "UNSET_CSR", -1 );

      // Now re-establish all cursors up the path from the recursive
      // child to the recursive parent entity types in the event that
      // the recursive child's parent is NOT the recursive parent.
      hRecursChildEntityInstance = lpViewSubobject->hViewParentEntityInstance;
      lpRecursChildEntityCsr = zGETPTR( lpRecursChildEntityCsr->hParent );
      while ( lpRecursChildEntityCsr != lpRootCsr )
      {
         lpRecursChildEntityCsr->hEntityInstance = hRecursChildEntityInstance;
      // if ( lpRecursChildEntityCsr->hEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "fnResetViewFromSubobject3", "UNSET_CSR", -1 );

         lpRecursChildEntityInstance = zGETPTR( hRecursChildEntityInstance );
         hRecursChildEntityInstance = lpRecursChildEntityInstance->hParent;
         lpRecursChildEntityCsr = zGETPTR( lpRecursChildEntityCsr->hParent );
      }

      // Now that we have reset the recursive view, see if a previous view
      // had been established which was subordinate to the recursive view.
      lpWorkViewSubobject = zGETPTR( lpViewSubobject->hNextViewSubobject );
      if ( lpWorkViewSubobject &&
           lpWorkViewSubobject->nRootHierNbr > lpViewSubobject->nRootHierNbr )
      {
         LPVIEWSUBOBJECT lpNextViewSubobject =
                           zGETPTR( lpWorkViewSubobject->hNextViewSubobject );

         while ( lpNextViewSubobject &&
                 lpNextViewSubobject->nRootHierNbr >
                                             lpViewSubobject->nRootHierNbr )
         {
            lpWorkViewSubobject = lpNextViewSubobject;
            lpNextViewSubobject =
                           zGETPTR( lpWorkViewSubobject->hNextViewSubobject );
         }

         while ( lpWorkViewSubobject != lpViewSubobject )
         {
            LPVIEWENTITYCSR   lpSubRootCsr;
            LPENTITYINSTANCE  lpWorkEntityInstance;
            int               nLevelChg;

            // Re-establish previous views from recursive view.
            // Here we restablish previous subordinate views to a recursive
            // view in the order in which they were initially established.
            lpSubRootCsr = zGETPTR( lpWorkViewSubobject->hRootCsr );
            lpSubRootCsr->hEntityInstance = lpWorkViewSubobject->hRootEntityInstance;
         // if ( lpSubRootCsr->hEntityInstance == UNSET_CSR )
         //    SysMessageBox( 0, "fnResetViewFromSubobject4", "UNSET_CSR", -1 );

            if ( lpSubRootCsr->hEntityInstance )
            {
               fnResetCursorForViewChildren( lpSubRootCsr );

               // Set parent cursors from subobject csr to lpRootCsr.
               lpWorkCsr = zGETPTR( lpSubRootCsr->hParent );
               lpWorkEntityInstance = zGETPTR( lpSubRootCsr->hEntityInstance );
               while ( lpWorkCsr && lpWorkCsr != lpRootCsr )
               {
                  lpWorkEntityInstance = zGETPTR( lpWorkEntityInstance->hParent );
                  lpWorkCsr = zGETPTR( lpWorkCsr->hParent );
               }
            }
            else
               fnNullCursorForViewChildren( lpSubRootCsr );

            // Now that we've reset a cursor, redo the leveling if a level
            // change is necessary.
            nLevelChg = lpWorkViewSubobject->nLevelChg;
            lpWorkCsr = zGETPTR( lpWorkViewSubobject->hLastDescCsr );
            lpWorkCsr->hNextHier = 0;
            lpWorkCsr = zGETPTR( lpWorkViewSubobject->hRootCsr );
            lpWorkCsr->hPrevHier = 0;
            lpWorkCsr->hParent = 0;
            lpViewCsr->hRootViewEntityCsr =
                        fnCreateEntityCsrHandle( lpWorkCsr, lpViewCsr, 0 );
            lpViewCsr->hViewParentEntityInstance =
                        lpWorkViewSubobject->hViewParentEntityInstance;
            if ( nLevelChg )
            {
               while( lpWorkCsr )
               {
                  lpWorkCsr->nLevel += nLevelChg;
                  lpWorkCsr = zGETPTR( lpWorkCsr->hNextHier );
               }
            }

            // Whew, one done, re-establish the next sub view.
            lpWorkViewSubobject =
                        zGETPTR( lpWorkViewSubobject->hPrevViewSubobject );
         }
      }
   }

   // Unchain lpViewSubobject and return it to the system.
   lpViewCsr->hFirstViewSubobject = lpViewSubobject->hNextViewSubobject;
   if ( lpViewCsr->hFirstViewSubobject )
   {
      LPVIEWSUBOBJECT lpFirstViewSubobject =
                                 zGETPTR( lpViewCsr->hFirstViewSubobject );

      lpFirstViewSubobject->hPrevViewSubobject = 0;

      // Gig, 11/4/92 - Reset View Parent Entity Instance.
      lpViewCsr->hViewParentEntityInstance =
                     lpFirstViewSubobject->hViewParentEntityInstance;
   }
   else
      lpViewCsr->hViewParentEntityInstance = 0;

   fnFreeDataspace( lpViewSubobject );
   return( 0 );
}

//./ ADD NAME=fnDropAllViewSubobjects
// Source Module=kzoevmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDropAllViewSubobjects
//
//  PURPOSE:    Drop all ViewSubobjects for a view
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
void
fnDropAllViewSubobjects( zVIEW zView )
{
   LPVIEWCSR   lpViewCsr;

   lpViewCsr = zGETPTR( zView->hViewCsr );

   // Discard all subobject views.
   while ( lpViewCsr->hFirstViewSubobject )
   {
      LPVIEWSUBOBJECT   lpViewSubobject;

      lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
      lpViewCsr->hFirstViewSubobject = lpViewSubobject->hNextViewSubobject;
      fnFreeDataspace( lpViewSubobject );
   }
}
