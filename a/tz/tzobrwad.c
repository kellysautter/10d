//
// OPERATIONS:   Build Task List - Build list of tasks and what's
//               going on
//
// ///////////////////////////////////////////////////////////////////
// * Copyright (c) 1993 QuinSoft Corporation. All rights reserved.   *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// ///////////////////////////////////////////////////////////////////
//  AUTHOR:  Gig Kirk
//  DATE:    3/31/92
//  API:     None
//  ENVIRONMENT: Any
//
/*
CHANGE LOG

2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES

2002.02.21  FH
   enable display of description on shop of OI from vml debugger.

2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.10.24  SR  Z2000  Size of path variables
   Modified size ofsize of all filename and path variables in whole module
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
   Adjust the size of messagetext variables.

2000.08.31  BL   Z10
   Modified zwTZOBRWAD_ChangeAttribute: bugfix for change Attribute value

2000.02.15  DGC  Z10  TB1005
   Fixed bug reported by Ruth S.  When user is browsing a view that is
   dropped the browser crashed.  Put in code to check to see if the view is
   still valid and exit child window if it is not.

1999.11.07  BL   Z10  new menu structure
   Insert operation zwTZOBRWAD_HelpAbout, zwTZOBRWAD_HelpBasics,
   zwTZOBRWAD_HelpOnHelp and zwTZOBRWAD_HelpContents for menu help

1999.10.14  DGC  Z10
   Fixed bug that allowed the browser to access tasks even if they were in the
   process of shutting down.

1999.10.04  DKS  Z10
   Adding preliminary code for implementation of Compare object instances.

1999.09.14  DKS  Z10
   Removed references to bRelOwner in the structure EntityInstanceInd.

1999.07.13  DGC  Z10
   Changed (fixed?) the browser to show hidden entities/attributes if user
   set's 'Show hidden' flag.

1998.12.10  DKS
   Deactivated PrintPreview functionality in the ObjectBrowser and moved
   it to the Object Engine.

1998.10.13  DKS
   Implemented new button header listbox with auto sort.

1998.10.13  DGC
   Made changes made necessary by the change in how GETPTR works.

1998.08.10  DKS
   Added processing to maintain Zoom across iterations.

1998.02.19  DGC
   Small change because of application mutexes.

1997.11.06  DonC
   Modified BuildTextForGraph to check for null pointer (lpWkCsr) that was not
   converted correctly from 9G.

1997.10.07  DonC
   Corrected SetCursor routine to refresh diagram properly.

1997.09.15  DG
   Streamlined code for better readability.

1997.09.06  DKS
   Show a reasonable task title for all tasks.

1997.07.13  DKS
   Does not free memory that it gets.  There is still a bug somewhere
   when moving around the browser too much, but quite a bit more stable
   than it was.

1997.07.10  GT
   Show the task title for background tasks

1997.05.02  GT
   For OS/2: Correct behavior when double clicking on a view
   The Browser is not able to show the LOD tree in OS/2, cause
   there is no Tree control. The Browser opens now the OldViewOI
   window for a double click in OS/2.

*/

//#define zNO_KZOEINCO

// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
//typedef long MutexInfo;

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#define lShowDerived 0

//#define _NOANCHOR

#include <ctype.h>

#define zGLOBAL_DATA
#define KZOESYS_INCL
#include <kzoengaa.h>     // Object Services public prototypes

#include <zdrvropr.h>
#include <tzlodopr.h>
#include <tz__oprs.h>
#include <tzvmlip.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"

zOPER_EXPORT zSHORT OPERATION
CompareObjects( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildTextForGraph( zVIEW   vSubtask,
                              zCPCHAR cpcSpecifyColor = 0 );
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorChange( zVIEW vSubtask, zSHORT nPos,
                         zCPCHAR cpcTag = "OI_Hier" );
zSHORT
SelectNode( zVIEW vSubtask, zCPCHAR cpcTag = "OI_Hier" );
zSHORT
CenterNode( zVIEW vSubtask, zCPCHAR cpcTag = "OI_Hier" );
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CenterNode( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ActivateOIDisplay( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
oTZ__PRFO_GetViewToProfile( zPVIEW, zPCHAR, zVIEW, zSHORT );
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildAttrsForGraph( zVIEW vSubtask );
zSHORT
BuildAttrsForGraph( zVIEW vSubtask, zCPCHAR cpcTag );

// Literals
char szlCreatedFlag[]         = "CreatedFlag";
char szlUpdatedFlag[]         = "UpdatedFlag";
char szlDeletedFlag[]         = "DeletedFlag";
char szlExcludedFlag[]        = "ExcludedFlag";
char szlIncludedFlag[]        = "IncludedFlag";
char szlRecordOwnerFlag[]     = "RecordOwnerFlag";
char szlRelOwnerFlag[]        = "RelOwnerFlag";
char szlCollapsed[]           = "Collapsed";
char szlHidden[]              = "Hidden";
char szlHiddenFlag[]          = "HiddenFlag";
char szlApplication[]         = "Application";
char szlAttrib[]              = "Attrib";
char szlChildEntity[]         = "ChildEntity";
char szlColor[]               = "Color";
char szlCommitBinary[]        = "CommitBinary";
char szlCommitIncremental[]   = "CommitIncremental";
char szlDIL[]                 = "DIL";
char szlEntity[]              = "Entity";
char szlFileName[]            = "FileName";
char szlFirstAttr[]           = "FirstAttr";
char szlUpdateAttr[]          = "UpdateAttr";
char szlHD_ChildNode[]        = "HD_ChildNode";
char szlHD_Diagram[]          = "HD_Diagram";
char szlHD_Node[]             = "HD_Node";
char szlOI_Hier[]             = "OI_Hier";
char szlID[]                  = "ID";
char szlInd[]                 = "Ind";
char szlInstanceOwner[]       = "InstanceOwner";
char szlInstancePointer[]     = "InstancePointer";
char szlKZAPPLOO[]            = "KZAPPLOO";
char szlLevel[]               = "Level";
char szlLocked[]              = "Locked";
char szlMode[]                = "Mode";
char szlName[]                = "Name";
char szlNumberOfViews[]       = "NumberOfViews";
char szlObject[]              = "Object";
char szlObjectInstanceID[]    = "ObjectInstanceID";
char szlObjectType[]          = "ObjectType";
char szlOIDisplay[]           = "OIDisplay";
char szlPointer[]             = "Pointer";
char szlRoot[]                = "Root";
char szlShowHidden[]          = "ShowHidden";
char szlSubtaskInd[]          = "SubtaskInd";
char szlSystem[]              = "System";
char szlTask[]                = "Task";
char szlText[]                = "Text";
char szlTitle[]               = "Title";
char szlTZOBOIOO[]            = "TZOBOIOO";
char szlTZOBRWAD[]            = "TZOBRWAD";
char szlTZOBRWOO[]            = "TZOBRWOO";
char szlTZZOHDGO[]            = "TZZOHDGO";
char szlTZZOHDGV[]            = "TZZOHDGV";
char szlTZZOHDGV_S[]          = "TZZOHDGV_S";
char szlTZOLOBWO[]            = "TZOLOBWO";
char szlValue[]               = "Value";
char szlView[]                = "View";
char szlVIEWOI[]              = "VIEWOI";
char szlWkEntity[]            = "WkEntity";
char szlZEIDON[]              = "ZEIDON";
char szlZKey[]                = "ZKey";


#define zENTITY_CREATE        10
#define zENTITY_CREATETEMP    11
#define zENTITY_DELETE        12
#define zENTITY_DELETEALL     13
#define zENTITY_DROP          14
#define zENTITY_EXCLUDE       15
#define zENTITY_EXCLUDEALL    16
#define zENTITY_TEMPORAL      17
#define zENTITY_ACCEPT        18
#define zENTITY_CANCEL        19
#define zENTITY_SETVIEW       20

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildGraphForView( zVIEW vSubtask );

zLONG
fnGetAttributesForEntityInstance( zVIEW vSubtask,
                                  zVIEW vInstance,
                                  zBOOL bShare );

zSHORT
fnSetInstanceOwnerForName( LPVIEWNAME lpViewName, zVIEW vBrowser );

LPVIEWENTITYCSR
fnEstablishViewForInstance( zVIEW             lpView,
                            LPVIEWENTITYCSR   lpViewEntityCsr,
                            LPENTITYINSTANCE  lpEntityInstance );

zVOID
fnSwapTaskContext( zVIEW lpView )
{
#if 1
   return;
#else
   LPANCHOR    AnchorBlock;
   zVIEW       vTZOBRWOO;
   zLONG       lMyTask;
   zLONG       lOtherTask;

   AnchorBlock = SysGetAnchorBlock( );

   if ( AnchorBlock->lSwappedTaskID == SysGetTaskFromView( lpView ) )
   {
      AnchorBlock->lSwappingTaskID = 0;
      AnchorBlock->lSwappedTaskID  = 0;
   }
   else
   if ( GetViewByName( &vTZOBRWOO, "TZOBRWOO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      lMyTask = SysGetTaskFromView( lpView );
      GetIntegerFromAttribute( &lOtherTask, vTZOBRWOO, szlTask, szlID );
      AnchorBlock->lSwappingTaskID = lMyTask;
      AnchorBlock->lSwappedTaskID  = lOtherTask;
   }
#endif
}

#define  zPOS_CURR          (zSHORT) 0

//./ ADD NAME=AddView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: AddView
//
//   PURPOSE:   Add the lpView to the browser entity Name.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
AddView( zVIEW vBrowser, zVIEW lpView, zPCHAR pchName, zPCHAR pchLevel )
{
   LPVIEWOD  lpViewOD;
   LPVIEWCSR lpViewCsr;
   LPAPP     lpApp;

   lpViewOD  = (LPVIEWOD) SysGetPointerFromHandle( lpView->hViewOD );
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( lpView->hViewCsr );

   CreateEntity( vBrowser, szlName, zPOS_LAST );
   if ( pchName )
      SetAttributeFromString( vBrowser, szlName, szlName, pchName );
   else
      SetAttributeFromString( vBrowser, szlName, szlName, "** un-named **" );

   SetAttributeFromInteger( vBrowser, szlName, szlID,
                            (zLONG) SysGetHandleFromPointer( lpView ) );
   if ( lpView->bViewLocked )
      SetAttributeFromString( vBrowser, szlName, szlLocked, "Y" );
   else
      SetAttributeFromString( vBrowser, szlName, szlLocked, "N" );

   if ( lpViewCsr )
      SetAttributeFromInteger( vBrowser, szlName, szlObjectInstanceID,
                               (zLONG) SysGetPointerFromHandle( lpViewCsr->hViewOI ) );
   if ( lpViewOD )
      SetAttributeFromString( vBrowser, szlName, szlObjectType,
                              lpViewOD->szName );
   if ( lpView->hSubtask )
      SetAttributeFromString( vBrowser, szlName, szlSubtaskInd, "Y" );

   if ( lpView->bFlagDropView )
      SetAttributeFromString( vBrowser, szlName,
                              "ShowMessageWhenViewIsDropped", "Y" );
   else
      SetAttributeFromString( vBrowser, szlName,
                              "ShowMessageWhenViewIsDropped", "N" );

   SfGetApplicationForSubtask( &lpApp, lpView );
   if ( lpApp )
      SetAttributeFromString( vBrowser, szlName, "ApplicationName",
                              lpApp->szName );

   SetAttributeFromString( vBrowser, szlName, szlLevel, pchLevel );
   return( 0 );

} // AddView

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: CheckShowDesc
//
//   PURPOSE:   correct the MenuFlag for the ShowDesc
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CheckShowDesc( zVIEW vSubtask )
{
   zVIEW vProfile;
   zCHAR szFlag[ 2 ];

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   // GetViewByName( &vProfile, "ProfileXFER", vSubtask, zLEVEL_SYSTEM )
   if ( CheckExistenceOfEntity( vProfile, "OBR" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szFlag, sizeof( szFlag ), vProfile, "OBR", "ShowDesc" );
      if ( szFlag[ 0 ] == 'Y' )
         SetOptionState( vSubtask, "ShowDesc", zCONTROL_STATUS_CHECKED, 1 );
      else
         SetOptionState( vSubtask, "ShowDesc", zCONTROL_STATUS_CHECKED, 0 );
   }

   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=BuildTaskList
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: BuildTaskList
//
//   PURPOSE:   Build an object containing all Applications, tasks,
//              task view ids and view id names.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
BuildTaskList( zVIEW vSubtask )
{
   LPANCHOR    AnchorBlock;
   LPTASK      lpTask;
   zLONG       lCurrTask;
   LPAPP       lpApp;
   LPTASK      lpMyTask;
   zVIEW       vBrowser;
   zVIEW       vProfile;
   zVIEW       vKZAPPLOO;
   zPCHAR      pchShowUnnamed;

   AnchorBlock = SysGetAnchorBlock( );

#if 1
// SetWindowState( vSubtask, zWINDOW_STATUS_UPDATE, 0 );
   for ( lpTask = SfGetNextTask( 0 );
         lpTask;
         lpTask = SfGetNextTask( lpTask ) )
   {
      if ( lpTask->bShutdown )
      {
         lpTask = SfGetNextTask( 0 );
         SysWait( 100 );
         continue;
      }
   }
#endif

#if 0  // Taken out DGC 1998.10.09
   // If the task is currently swapped, then we must be in another section
   // of this code and a message box was issued resulting in our invocation
   // by an ActivateEvent, get out quickly to prevent major crisis.
   if ( AnchorBlock->lSwappedTaskID )
   {
   // TraceLineI( "BuildTaskList SwappedTaskID",
   //             AnchorBlock->lSwappedTaskID );
      return( 0 );
   }
#endif

// SysMutexLock( vSubtask, "ZEIDON MUTEX -- ANCHOR BLOCK", 0, 0 );
   SysStartBrowseOfTaskList( vSubtask );

   if ( GetViewByName( &vKZAPPLOO, szlKZAPPLOO, vSubtask, zLEVEL_TASK ) < 1 )
   {
      zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
      zSHORT nLth;

      SysGetEnvVar( szFileName, szlZEIDON, zMAX_FILENAME_LTH + 1 );

      // Edit the zeidon environment and make sure it ends with
      // a directory separator.
      nLth = (zSHORT) zstrlen( szFileName );
      if ( szFileName[ nLth - 1 ] != '\\' )
      {
         szFileName[ nLth++ ] = '\\';
         szFileName[ nLth ] = 0;
      }

      strcat_s( szFileName, sizeof( szFileName ), "zeidon.app" );
      SfActivateSysOI_FromFile( &vKZAPPLOO, szlKZAPPLOO, vSubtask,
                                szFileName, zSINGLE );
      SetNameForView( vKZAPPLOO, szlKZAPPLOO, vSubtask, zLEVEL_TASK );
   }

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   GetAddrForAttribute( &pchShowUnnamed, vProfile, "OBR", "ShowUnnamed" );

   // Get our task since we will not report on ourselves.
   lpMyTask = SfGetCurrentTask( vSubtask );

   // Activate the browser object.
   lCurrTask = 0;
   if ( GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetIntegerFromAttribute( &lCurrTask, vBrowser, szlTask, szlID );
      lCurrTask = (zLONG) SysGetPointerFromHandle( (zPVOID) lCurrTask );
      DropObjectInstance( vBrowser );
   }

   ActivateEmptyObjectInstance( &vBrowser, szlTZOBRWOO, vSubtask, zSINGLE );
   SetNameForView( vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );

   // Create the root entity.
   CreateEntity( vBrowser, szlRoot, zPOS_AFTER );
   SetAttributeFromInteger( vBrowser, szlRoot, szlCommitBinary, 0 );
   SetAttributeFromInteger( vBrowser, szlRoot, szlCommitIncremental, 0 );

   // Now go through and create the Task, View, and Name entities.
   for ( lpTask = SfGetNextTask( 0 );
         lpTask;
         lpTask = SfGetNextTask( lpTask ) )
   {
      zBOOL  bTitle;
      zVIEW  lpView;
      LPVIEWNAME lpViewName;
      zLONG  lViewCount;

      // Don't get info for current (browser) task.
      if ( lpTask == lpMyTask )
         continue;

      lViewCount = 0;
      CreateEntity( vBrowser, szlTask, zPOS_AFTER );
      SetAttributeFromInteger( vBrowser, szlTask, szlID,
                               (zLONG) SysGetHandleFromPointer( lpTask ) );
      SetAttributeFromInteger( vBrowser, szlTask, szlPointer,
                               (zLONG) lpTask );

      bTitle = FALSE;
      if ( lpTask->hMainWnd )
      {
         zCHAR  szTaskTitle[ 128 ];
         HWND   hWndMain = (HWND) lpTask->hMainWnd;

         // Get Title of task.
         GetWindowText( hWndMain, szTaskTitle, sizeof( szTaskTitle ) - 1 );
         SetAttributeFromString( vBrowser, szlTask, szlName, szTaskTitle );
         if ( szTaskTitle[ 0 ] )
            bTitle = TRUE;
      }
      else
      if ( lpTask->szTaskTitle[ 0 ] )
      {
         SetAttributeFromString( vBrowser, szlTask, szlName,
                                 lpTask->szTaskTitle );
         bTitle = TRUE;
      }

      // Set a default task title, for background processes, which have no window.
      if ( bTitle == FALSE )
      {
         zCHAR  szTaskTitle[ 128 ];

         if ( lpTask->szTaskTitle[ 0 ] )
            strcpy_s( szTaskTitle, sizeof( szTaskTitle ), lpTask->szTaskTitle );
         else
         {
            sprintf_s( szTaskTitle, sizeof( szTaskTitle ), "<Task 0x%08x, %s>",
                       (zULONG) SysGetHandleFromPointer( lpTask ),
                       ((LPAPP) SysGetPointerFromHandle( lpTask->hApp ))->szName );
         }

         SetAttributeFromString( vBrowser, szlTask, szlName, szTaskTitle );
      }

      // Go through and set views for the task.
      lpView = (zVIEW) SysGetPointerFromHandle( lpTask->hFirstView );

      // Validate that each view is valid.
      while ( lpView && lpView->nTableID == iView &&
              SysValidAddress( lpView, sizeof( ViewRecord ), 1 ) )
      {
         // If a view has no names and unnamed views should be shown,
         // then add the view to the view list.
         if ( lpView->nViewNames == 0 && pchShowUnnamed[ 0 ] == 'Y' )
            AddView( vBrowser, lpView, 0, "Task" );

         if ( lpView->hSubtask )
         {
            LPSUBTASK lpSubtask = (LPSUBTASK) SysGetPointerFromHandle( lpView->hSubtask );

            // Add named subtask views.
            lpViewName = (LPVIEWNAME) SysGetPointerFromHandle( lpSubtask->hFirstViewName );
            while ( lpViewName )
            {
               AddView( vBrowser, (zVIEW) SysGetPointerFromHandle( lpViewName->hView ),
                        lpViewName->szName, "Subtask" );
               fnSetInstanceOwnerForName( lpViewName, vBrowser );
               lpViewName = (LPVIEWNAME)
                                    SysGetPointerFromHandle( lpViewName->hNextViewName );
            }
         }

         lpView = (zVIEW) SysGetPointerFromHandle( lpView->hNextView );
      }

      if ( lpView )
         SysMessageBox( vSubtask, "Zeidon Browser", "Encountered Invalid View", 1 );

      // Go through names at the task, application and system level and
      // add them to the view list.
      lpViewName = (LPVIEWNAME) SysGetPointerFromHandle( lpTask->hFirstViewName );
      while ( lpViewName )
      {
         AddView( vBrowser, (zVIEW) SysGetPointerFromHandle( lpViewName->hView ),
                  lpViewName->szName, "Task" );
         fnSetInstanceOwnerForName( lpViewName, vBrowser );
         lpViewName = (LPVIEWNAME) SysGetPointerFromHandle( lpViewName->hNextViewName );
      }

      lpApp      = (LPAPP) SysGetPointerFromHandle( lpTask->hApp );
      lpViewName = (LPVIEWNAME) SysGetPointerFromHandle( lpApp->hFirstViewName );
      while ( lpViewName )
      {
         AddView( vBrowser, (zVIEW) SysGetPointerFromHandle( lpViewName->hView ),
                  lpViewName->szName, "Application" );
         fnSetInstanceOwnerForName( lpViewName, vBrowser );
         lpViewName = (LPVIEWNAME) SysGetPointerFromHandle( lpViewName->hNextViewName );
      }

      lpViewName = (LPVIEWNAME) SysGetPointerFromHandle( AnchorBlock->hFirstViewName );
      while ( lpViewName )
      {
         AddView( vBrowser, (zVIEW) SysGetPointerFromHandle( lpViewName->hView ),
                  lpViewName->szName, "System" );
         fnSetInstanceOwnerForName( lpViewName, vBrowser );
         lpViewName = (LPVIEWNAME) SysGetPointerFromHandle( lpViewName->hNextViewName );
      }

      SetAttributeFromInteger( vBrowser, szlTask, szlNumberOfViews, lViewCount );
   }

   // Now that we have done all of the task stuff, go through all
   // applications and display their information.
   lpApp = (LPAPP) SysGetPointerFromHandle( AnchorBlock->hFirstApp );
   while ( lpApp )
   {
      LPVIEWOD  lpViewOD;
      CreateEntity( vBrowser, szlApplication, zPOS_AFTER );
      SetAttributeFromString( vBrowser, szlApplication, szlName,
                              lpApp->szName );
      lpViewOD = (LPVIEWOD) SysGetPointerFromHandle( lpApp->hFirstViewOD );
      while ( lpViewOD )
      {
         CreateEntity( vBrowser, szlObject, zPOS_AFTER );
         SetAttributeFromString( vBrowser, szlObject, szlObjectType,
                                 lpViewOD->szName );

         lpViewOD = (LPVIEWOD) SysGetPointerFromHandle( lpViewOD->hNextViewOD );
      }

      lpApp = (LPAPP) SysGetPointerFromHandle( lpApp->hNextApp );
   }

   // Reset the browser view.
   ResetView( vBrowser );
   if ( lCurrTask )
   {
      lCurrTask = (zLONG) SysGetHandleFromPointer( (zPVOID) lCurrTask );
      if ( SetCursorFirstEntityByInteger( vBrowser, szlTask, szlID,
                                          lCurrTask, 0 ) >= zCURSOR_SET )
      {
         SetSelectStateOfEntity( vBrowser, szlTask, 1 );
      }
   }

// SysMutexUnlock( vSubtask, "ZEIDON MUTEX -- ANCHOR BLOCK", 0 );
   SysEndBrowseOfTaskList( );

   // Correct the MenuFlag for the ShowDesc.
   zwTZOBRWAD_CheckShowDesc( vSubtask );

// TraceLineS( "BuildTaskList", "Calling RefreshWindow" );
   RefreshWindow( vSubtask );
// SetWindowState( vSubtask, zWINDOW_STATUS_INVALIDATE, 0 );
// SetWindowState( vSubtask, zWINDOW_STATUS_UPDATE, 0 );

   // We're done, leave gracefully.
   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=fnBuildObjectInstanceForView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: fnBuildObjectInstanceForView
//
//   PURPOSE:   Build an object instance containing the information
//              for a hilighted named view.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnBuildObjectInstanceForView( zVIEW vSubtask, zBOOL bShare )
{
   zVIEW       vBrowser;
   zVIEW       vProfile;
   zVIEW       vObject;
   zVIEW       vWorkObject;
   zVIEW       vInstance;
   zPCHAR      pchShowHidden;
   LPVIEWCSR   lpViewCsr = 0;
   LPVIEWOI    lpViewOI;
   LPVIEWENTITYCSR      lpViewEntityCsr;
   LPVIEWENTITYCSR      lpRootViewEntityCsr;
   LPVIEWENTITYCSR      lpWorkViewEntityCsr;
   LPVIEWATTRIB         lpViewAttrib;
   LPENTITYINSTANCE     lpEntityInstance;
   LPENTITYINSTANCE     lpLinkedEntityInstance;
   LPSELECTEDINSTANCE   lpFirstSelectedInstance;
   LPSELECTEDINSTANCE   lpSelectedInstance;
   zCHAR       szAttrib[ 260 ];
   zCHAR       szInd[ 5 ];
   zLONG       lEntities, lAttributes;
   zSHORT      nRC;

   // Display operation counts
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   OperationCount( vSubtask, 11 );
   if ( GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropObjectInstance( vInstance );
   }

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
// GetViewByName( &vProfile, "ProfileXFER", vSubtask, zLEVEL_TASK );

   GetAddrForAttribute( &pchShowHidden, vProfile, "OBR", "ShowHidden" );

   if ( CheckExistenceOfEntity( vBrowser, szlName ) < zCURSOR_SET )
      return( 0 );

   ActivateEmptyObjectInstance( &vInstance, szlTZOBOIOO, vSubtask, zSINGLE );
   SetNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
   CreateEntity( vInstance, szlName, zPOS_AFTER );
   SetMatchingAttributesByName( vInstance, szlName, vBrowser, szlName, zSET_NULL );

   GetIntegerFromAttribute( (zPLONG) &vObject, vBrowser, szlName, szlID );
   vObject = (zVIEW) SysGetPointerFromHandle( vObject );

   // See if there is any instance data to display.
   if ( vObject->hViewCsr )
   {
      lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vObject->hViewCsr );
      lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
   }
   else
      lpViewOI = 0;

   if ( lpViewOI && lpViewOI->hRootEntityInstance )
   {
      // Go through the view and call CheckExistenceOfEntity for each
      // visible cursor which is currently in an unset state to force
      // the cursor to be positioned.
      lpWorkViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
      while ( lpWorkViewEntityCsr )
      {
         if ( lpWorkViewEntityCsr->hEntityInstance == UNSET_CSR )
         {
            LPVIEWENTITY lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpWorkViewEntityCsr->hViewEntity );

            CheckExistenceOfEntity( vObject, lpViewEntity->szName );
         }

         lpWorkViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWorkViewEntityCsr->hNextHier );
      }

      // Create a work view to the object
      CreateViewFromViewForTask( &vWorkObject, vObject, vSubtask );
      ResetView( vWorkObject );

      // Get first selected instance for selection marking
      lpRootViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
      lpFirstSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpViewCsr->hFirstSelectedInstance );

      lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vWorkObject->hViewCsr );
      lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );

      // Now go through each instance in the target object, plug the cursor
      // for the instance and retrieve the attributes for each entity
      lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewOI->hRootEntityInstance );
      lEntities   = 0;
      lAttributes = 0;
      szInd[ 4 ]  = 0;
      nRC = 0;
      while ( nRC == 0 && lpEntityInstance )
      {
         zSHORT       nAttrSet;
         LPVIEWENTITY lpViewEntity;

         if ( lpEntityInstance->u.nInd.bHidden == FALSE ||
              pchShowHidden[ 0 ] == 'Y' )
         {
            zSHORT nIdx;
            zBOOL  bHidden;

            if ( lpEntityInstance->u.nInd.bHidden )
            {
               bHidden = TRUE;
               lpEntityInstance->u.nInd.bHidden = FALSE;
            }
            else
               bHidden = FALSE;

            nRC = CreateEntity( vInstance, szlEntity, zPOS_AFTER );
            SetAttributeFromInteger( vInstance, szlEntity, szlPointer,
                                     (zLONG) lpEntityInstance );
            lEntities++;

            szInd[ 0 ] = '.';
            szInd[ 1 ] = '.';
            szInd[ 2 ] = '.';
            szInd[ 3 ] = '.';

            if ( lpEntityInstance->hNextLinked )
            {
               lpLinkedEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextLinked );

               while ( lpLinkedEntityInstance != lpEntityInstance &&
                       lpLinkedEntityInstance->u.nInd.bHidden )
               {
                  lpLinkedEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkedEntityInstance->hNextLinked );
               }

               if ( lpLinkedEntityInstance != lpEntityInstance )
                  szInd[ 0 ] = 'l';
            }

            if ( lpFirstSelectedInstance )
            {
               for ( lpSelectedInstance = lpFirstSelectedInstance;
                     lpSelectedInstance;
                     lpSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpSelectedInstance->hNextSelectedInstance ) )
               {
                  if ( SysGetPointerFromHandle( lpSelectedInstance->xEntityInstance ) == lpEntityInstance )
                  {
                     szInd[ 1 ] = 's';
                     break;
                  }
               }
            }

            if ( lpEntityInstance->hPrevVsn )
               szInd[ 2 ] = 'v';
            else
            if ( lpEntityInstance->u.nInd.bTemporal )
               szInd[ 2 ] = 't';
            else
            if ( lpEntityInstance->u.nInd.bPrevVersion )
               szInd[ 2 ] = 'p';

            lpWorkViewEntityCsr = lpRootViewEntityCsr;
            while ( lpWorkViewEntityCsr )
            {
               if ( SysGetPointerFromHandle( lpWorkViewEntityCsr->hEntityInstance ) == lpEntityInstance )
               {
                  szInd[ 3 ] = '>';
                  break;
               }

               lpWorkViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWorkViewEntityCsr->hNextHier );
            }

            SetAttributeFromString( vInstance, szlEntity, szlInd, szInd );

            szAttrib[ 0 ] = 0;
            for ( nIdx = 0; nIdx < (lpEntityInstance->nLevel - 1); nIdx++ )
            {
               szAttrib[ nIdx ] = ' ';
            }

            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );

            szAttrib[ nIdx ] = 0;
            strcat_s( szAttrib, sizeof( szAttrib ), lpViewEntity->szName );

            SetAttributeFromString( vInstance, szlEntity, szlName, szAttrib );
            SetAttributeFromInteger( vInstance, szlEntity, szlLevel,
                                     (zLONG) lpEntityInstance->nLevel );

            nAttrSet = 0;
            for ( lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
                  lpViewAttrib;
                  lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib ) )
            {
               if ( lpViewAttrib->bHidden && pchShowHidden[ 0 ] != 'Y' )
                  continue;

               if ( lpViewAttrib->szDerivedOper[ 0 ] && lShowDerived == 0 )
                  continue;

               if ( lpViewAttrib->cType != 'B' )
                  continue;

               if ( nAttrSet == 0 ||
                    (nAttrSet == 1 && lpViewAttrib->cType == 'S') )
               {
                  lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hFirstOD_EntityCsr );
                  while ( lpViewEntityCsr->hViewEntity !=
                                              lpEntityInstance->hViewEntity )
                  {
                     lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
                  }

                  lpViewEntityCsr->hEntityInstance = (LPENTITYINSTANCE) SysGetHandleFromPointer( lpEntityInstance );

                  if ( lpViewAttrib->bHidden )
                     GetStringFromRecord( vWorkObject,
                                          lpViewEntity,
                                          lpViewAttrib, szAttrib, 256 );
                  else
                     GetVariableFromAttribute( szAttrib, 0, zTYPE_STRING,
                                               256, vWorkObject,
                                               lpViewEntity->szName,
                                               lpViewAttrib->szName, 0, 0 );

                  SetAttributeFromString( vInstance, szlEntity,
                                          szlFirstAttr, szAttrib );
#if 0  // DGC 10/1/96
                  if ( lpViewAttrib->cType == 'S' )
                     nAttrSet = 2;
                  else
                     nAttrSet = 1;
#endif
               }

            } // for ( lpViewAttrib... )...

            if ( bHidden )
               lpEntityInstance->u.nInd.bHidden = TRUE;

         } // if ( lpEntityInstance->u.nInd.bHidden == FALSE ||
           //      pchShowHidden[ 0 ] == 'Y' )...

         lpEntityInstance = (LPENTITYINSTANCE)
                                      SysGetPointerFromHandle( lpEntityInstance->hNextHier );
      }

      ResetView( vInstance );
      fnGetAttributesForEntityInstance( vSubtask, vInstance, 0 );
      if ( CheckExistenceOfEntity( vInstance, szlAttrib ) == zCURSOR_SET )
      {
         SetAttributeFromAttribute( vInstance, szlEntity, szlUpdateAttr,
                                    vInstance, szlAttrib, szlValue );
      }

      DropView( vWorkObject );
   }
   else
   {
      OperatorPrompt( vSubtask, "Object Browser",
                      "Object instance is empty!", 0, 0, 0, zICON_STOP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   // Display operation counts
   OperationCount( vSubtask, 0 );

   // Get our task so we can activate the trace

// Display results of object instance creation
TraceLineI( "(tzobrwad) Entity Entities created = ", lEntities );
TraceLineI( "        Attribute Entities created = ", lAttributes );
TraceLineI( "            Total Entities created = ",
            lEntities + lAttributes );

   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=BuildObjectInstanceForView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: BuildObjectInstanceForView
//
//   PURPOSE:   Build an object instance containing the information
//              for a hilighted named view.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
BuildObjectInstanceForView( zVIEW vSubtask )
{
   zVIEW  vBrowser;
   zVIEW  vInstance;
   zCHAR  szID[ 15 ];

   // First, get the handle to the browser object
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szID, sizeof( szID ), vBrowser,
                           szlName, szlObjectInstanceID );
   if ( GetViewByName( &vInstance, szID, vSubtask, zLEVEL_TASK ) > 0 )
   {
      ActivateWindow( vInstance );
      return( 0 );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow,
                               szlTZOBRWAD, szlVIEWOI );
      //SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow,
      //                         szlTZOBRWAD, szlOIDisplay );
   }

   fnBuildObjectInstanceForView( vSubtask, 1 );
   return( 0 );
}

//./ ADD NAME=CommitOI
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: CommitOI
//
//   PURPOSE:   Build an object instance containing the information
//              for a hilighted named view.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CommitOI( zVIEW vSubtask )
{
   zVIEW  vBrowser;
   zVIEW  vView;
   zPCHAR pchFileName;
   zLONG  lCommitBinary, lCommitIncremental, lControl;
   zCHAR  szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zCHAR  szFileType[ 2 ];
   zSHORT nRC;

   // First, get the handle to the browser object
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( &pchFileName, vBrowser, szlRoot, szlFileName );

   if ( pchFileName[ 0 ] )
   {
      GetIntegerFromAttribute( (zPLONG) &vView, vBrowser, szlName, szlID );
      vView = (zVIEW) SysGetPointerFromHandle( vView );
      GetIntegerFromAttribute( &lCommitBinary, vBrowser, szlRoot,
                               szlCommitBinary );
      GetIntegerFromAttribute( &lCommitIncremental, vBrowser, szlRoot,
                               szlCommitIncremental );
      GetStringFromAttribute( szFileType, sizeof( szFileType ), vBrowser, szlRoot, "FileType" );
      TraceLineS( "*** FileType: ", szFileType );
      lControl = 0;
      if ( szFileType[ 0 ] == 'Z' )
      {
         if ( lCommitIncremental )
            lControl = lControl | zINCREMENTAL;

         nRC = CommitOI_ToFile( vView, pchFileName, lControl );
      }
      else
      {
         nRC = CommitOI_ToXML_File( vView, pchFileName, lControl );
      }

      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "CM00107", "Object Browser", "Error on Writing File.",
                      zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      }
      else
      {
         strcpy_s( szMsg, sizeof( szMsg ), "ObjectInstance successfully written to File: " );
         strcat_s( szMsg, sizeof( szMsg ), pchFileName );

         MessageSend( vSubtask, "CM00107", "Object Browser", szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      }
   }
   else
   {
      MessageSend( vSubtask, "CM00107", "Object Browser", "No File Specified",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

//./ ADD NAME=ExitDialog
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ExitDialog
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ExitDialog( zVIEW vSubtask )
{
   zVIEW  vProfile;

   SetDefaultViewForActiveTask( 0, 4 );  // set real task view
   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   if ( vProfile == 0 )
      return( 0 );

   oTZ__PRFO_CommitProfile( vProfile );
   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=ToggleShowHidden
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ToggleShowHidden
//
//   PURPOSE:   Toggle the ShowHidden attribute in the profile object.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ToggleShowHidden( zVIEW vSubtask )
{
   BuildTaskList( vSubtask );
   return( 0 );
}

//./ ADD NAME=ToggleShowUnnamed
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ToggleShowUnnamed
//
//   PURPOSE:   Toggle the ShowUnnamed attribute in the profile object.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ToggleShowUnnamed( zVIEW vSubtask )
{
   BuildTaskList( vSubtask );
   return( 0 );
}

//./ ADD NAME=DisplayObjDef
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: DisplayObjDef
//
//   PURPOSE:   Display the object definition for the selected view.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DisplayObjDef( zVIEW vSubtask )
{
   zVIEW  vBrowser;
   zVIEW  vView;

   // First, get the handle to the browser object
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vView, vBrowser, szlName, szlID );
   vView = (zVIEW) SysGetPointerFromHandle( vView );
   DisplayObjectDef( vView, 0 );

   return( 0 );
}

//./ ADD NAME=DisplayTaskInstance
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: DisplayTaskInstance
//
//   PURPOSE:   To call DisplayObjectInstance with all collected task
//              information.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DisplayTaskInstance( zVIEW vSubtask )
{
   zVIEW       vBrowser;
   zLONG       lMyTask;

   // Get our task since we will not report on ourselves
   lMyTask = SysGetTaskFromView( vSubtask );

   if ( lMyTask )
   {
      if ( GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK ) > 0 )
      {
         DisplayObjectInstance( vBrowser, 0, 0 );
      }
   }

   return( 0 );
}

//./ ADD NAME=RefreshListBox3
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: RefreshListBox3
//
//   PURPOSE:   To refresh the attributes on the object browser when
//              an entity is selected.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RefreshListBox3( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "ListBox3" );
   return( 0 );
}


//./ ADD NAME=RefreshListBox2
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: RefreshListBox2
//
//   PURPOSE:   To refresh the attributes on the object browser when
//              an entity is selected.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RefreshListBox2( zVIEW vSubtask )
{
   zVIEW vInstance;

   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   fnGetAttributesForEntityInstance( vSubtask, vInstance, 1 );
   if ( CheckExistenceOfEntity( vInstance, szlAttrib ) == zCURSOR_SET )
   {
      SetAttributeFromAttribute( vInstance, szlEntity, szlUpdateAttr,
                                 vInstance, szlAttrib, szlValue );
   }

   RefreshCtrl( vSubtask, "OI_ListBox2" );
   RefreshCtrl( vSubtask, "OI_Edit1" );
   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   return( 0 );
}

//./ ADD NAME=fnGetAttributesForEntityInstance
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: fnGetAttributesForEntityInstance
//
//   PURPOSE:   To retrieve the attributes for a selected entity
//              instance in the object instance browser
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG
fnGetAttributesForEntityInstance( zVIEW vSubtask,
                                  zVIEW vInstance,
                                  zBOOL bShare )
{
   zVIEW             vBrowser;
   zVIEW             vObject;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zVIEW             vWorkObject;
   zVIEW             vProfile;
   zCHAR             szAttrib[ 260 ];
   zPCHAR            pchShowHidden;
   zULONG            uLth;
   zLONG             lAttributes = 0;
   zBOOL             bCreate;
   zSHORT            nRC;

   // Get the view to the object instance instance
   GetViewByName( &vBrowser, szlTZOBRWOO, vInstance, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vObject, vBrowser, szlName, szlID );
   vObject = (zVIEW) SysGetPointerFromHandle( vObject );

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
// GetViewByName( &vProfile, "ProfileXFER", vInstance, zLEVEL_TASK );
   GetAddrForAttribute( &pchShowHidden, vProfile, "OBR", "ShowHidden" );

   // Now that we have the id, get the task for the id and share that
   // tasks memory with our own
   if ( vObject->hViewCsr )
   {
      // Get the entity instance
      GetIntegerFromAttribute( (zPLONG) &lpEntityInstance, vInstance,
                               szlEntity, szlPointer );
      CreateViewFromViewForTask( &vWorkObject, vObject, vSubtask );
      ResetView( vWorkObject );
      lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vWorkObject->hViewCsr );
      lpViewEntityCsr = (LPVIEWENTITYCSR)
                    SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
      while ( lpViewEntityCsr &&
              lpViewEntityCsr->hViewEntity != lpEntityInstance->hViewEntity )
      {
         lpViewEntityCsr = (LPVIEWENTITYCSR)
                                       SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
      }

      if ( lpViewEntityCsr )
      {
         zSHORT       nAttrSet = 0;
         zBOOL        bHidden;
         LPVIEWENTITY lpViewEntity = (LPVIEWENTITY)
                                    SysGetPointerFromHandle( lpEntityInstance->hViewEntity );

         if ( lpEntityInstance->u.nInd.bHidden )
         {
            bHidden = TRUE;
            lpEntityInstance->u.nInd.bHidden = FALSE;
         }
         else
            bHidden = FALSE;

         lpViewEntityCsr->hEntityInstance = (LPENTITYINSTANCE)
                  SysGetHandleFromPointer( lpEntityInstance );

         lpViewAttrib = (LPVIEWATTRIB)
                                   SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
         bCreate = 1;
         if ( SetCursorFirstEntity( vInstance, szlAttrib, 0 ) == zCURSOR_SET )
         {
            bCreate = 0;
         }

         nRC = 0;
         while ( nRC == 0 && lpViewAttrib )
         {
            lAttributes++;
            if ( (lpViewAttrib->szDerivedOper[ 0 ] == 0 || lShowDerived) &&
                 (lpViewAttrib->bHidden == FALSE || pchShowHidden[ 0 ] == 'Y') )
            {
               if ( bCreate )
               {
                  nRC = CreateEntity( vInstance, szlAttrib, zPOS_AFTER );
                  SetAttributeFromString( vInstance, szlAttrib, szlName,
                                          lpViewAttrib->szName );
               }

               lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hFirstOD_EntityCsr );
               while ( lpViewEntityCsr->hViewEntity != lpEntityInstance->hViewEntity )
               {
                  lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
               }

               lpViewEntityCsr->hEntityInstance = (LPENTITYINSTANCE) SysGetHandleFromPointer( lpEntityInstance );

               if ( lpViewAttrib->cType != 'B' )
               {
                  if ( lpViewAttrib->bHidden )
                     GetStringFromRecord( vWorkObject, lpViewEntity,
                                          lpViewAttrib, szAttrib, 256 );
                  else
                     GetVariableFromAttribute( szAttrib, 0, zTYPE_STRING, 256,
                                               vWorkObject,
                                               lpViewEntity->szName,
                                               lpViewAttrib->szName, 0, 0 );
               }
               else
               {
                  GetAttributeLength( &uLth, vWorkObject,
                                      lpViewEntity->szName,
                                      lpViewAttrib->szName );
                  strcpy_s( szAttrib, sizeof( szAttrib ), "(Blob, lth=" );
                  zltoa( uLth, szAttrib + 50, sizeof( szAttrib ) - 50 );
                  strcat_s( szAttrib, sizeof( szAttrib ), szAttrib + 50 );
                  strcat_s( szAttrib, sizeof( szAttrib ), ")" );
               }
               SetAttributeFromString( vInstance, szlAttrib, szlValue, szAttrib );
               if ( (lpViewAttrib->szDerivedOper[ 0 ] == 0 || lShowDerived) &&
                    (nAttrSet == 0 ||
                    (nAttrSet == 1 && lpViewAttrib->cType == 'S')) )
               {
                  SetAttributeFromString( vInstance, szlEntity, szlFirstAttr, szAttrib );
                  if ( lpViewAttrib->cType == 'S' )
                     nAttrSet = 2;
                  else
                     nAttrSet = 1;
               }

               if ( bCreate == FALSE )
                  SetCursorNextEntity( vInstance, szlAttrib, 0 );
            }

            lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );

         } // while ( nRC == 0 && lpViewAttrib )...

         sprintf_s( szAttrib, sizeof( szAttrib ), "0x%8lx", (zLONG) lpEntityInstance );
         SetAttributeFromString( vInstance, szlEntity, szlInstancePointer, szAttrib );

         if ( lpEntityInstance->u.nInd.bCreated )
            SetAttributeFromInteger( vInstance, szlEntity, szlCreatedFlag, 1 );
         else
            SetAttributeFromInteger( vInstance, szlEntity, szlCreatedFlag, 0 );
         if ( lpEntityInstance->u.nInd.bUpdated )
            SetAttributeFromInteger( vInstance, szlEntity, szlUpdatedFlag, 1 );
         else
            SetAttributeFromInteger( vInstance, szlEntity, szlUpdatedFlag, 0 );
         if ( lpEntityInstance->u.nInd.bIncluded )
            SetAttributeFromInteger( vInstance, szlEntity, szlIncludedFlag, 1 );
         else
            SetAttributeFromInteger( vInstance, szlEntity, szlIncludedFlag, 0 );
         if ( lpEntityInstance->u.nInd.bExcluded )
            SetAttributeFromInteger( vInstance, szlEntity, szlExcludedFlag, 1 );
         else
            SetAttributeFromInteger( vInstance, szlEntity, szlExcludedFlag, 0 );
         if ( lpEntityInstance->u.nInd.bDeleted )
            SetAttributeFromInteger( vInstance, szlEntity, szlDeletedFlag, 1 );
         else
            SetAttributeFromInteger( vInstance, szlEntity, szlDeletedFlag, 0 );
         if ( bHidden )
            SetAttributeFromInteger( vInstance, szlEntity, szlHiddenFlag, 1 );
         else
            SetAttributeFromInteger( vInstance, szlEntity, szlHiddenFlag, 0 );

         if ( bHidden )
            lpEntityInstance->u.nInd.bHidden = TRUE;

      } // if ( lpViewEntityCsr )...

      SetCursorFirstEntity( vInstance, szlAttrib, 0 );
      DropView( vWorkObject );
   }

   DropView( vProfile );
   return( lAttributes );
}

//./ ADD NAME=SelectAttribute
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: SelectAttribute
//
//   PURPOSE:   To select an attribute by mouse single click
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SelectAttribute( zVIEW vSubtask )
{
   zVIEW vInstance;

   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   SetAttributeFromAttribute( vInstance, szlEntity, szlUpdateAttr,
                              vInstance, szlAttrib, szlValue );
   RefreshCtrl( vSubtask, "OI_Edit1" );
   return( 0 );
}

//./ ADD NAME=UpdateAttribute
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: UpdateAttribute
//
//   PURPOSE:   To update and attributes value
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UpdateAttribute( zVIEW vSubtask )
{
   zVIEW            vInstance;
   zVIEW            vBrowser;
   zVIEW            vProfile;
   zVIEW            vID;
   zVIEW            vWork;
   zPCHAR           pchShowHidden;
   zPCHAR           pchAttribName;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWATTRIB     lpViewAttrib;
   LPVIEWATTRIB     lpDispAttrib;
   LPENTITYINSTANCE lpEntityInstance;

   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );

   // Get the view for the real instance
   GetIntegerFromAttribute( (zPLONG) &vID, vInstance, szlName, szlID );
   vID = (zVIEW) SysGetPointerFromHandle( vID );

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
// GetViewByName( &vProfile, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( &pchShowHidden, vProfile, "OBR", "ShowHidden" );

   GetAddrForAttribute( &pchAttribName, vInstance, szlAttrib, szlName );

   // Find the entity and the attrib in the view object
   GetIntegerFromAttribute( (zPLONG) &lpEntityInstance,
                              vInstance, szlEntity, szlPointer );
   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );
   lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
   while ( zstrcmp( lpViewAttrib->szName, pchAttribName ) != 0 )
      lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );

   // if the attribute is not a blob, attempt to update it
   if ( lpViewAttrib->cType != 'B' )
   {
      CreateViewFromViewForTask( &vWork, vID, vSubtask );
      fnEstablishViewForInstance( vWork, 0, lpEntityInstance );
      if ( SetAttributeFromAttribute( vWork, lpViewEntity->szName,
                                      pchAttribName, vInstance,
                                      szlEntity, szlUpdateAttr ) == 0 )
      {
         SetAttributeFromAttribute( vInstance, szlAttrib, szlValue,
                                    vWork, lpViewEntity->szName,
                                    pchAttribName );
      }

      SetAttributeFromAttribute( vInstance, szlEntity, szlUpdateAttr,
                                 vInstance, szlAttrib, szlValue );
      // Now determine if the attribute is the attribute displayed in the
      // Entity Instance list, the display attribute is the first string OR
      // the first non-blob.
      lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
      while ( lpDispAttrib )
      {
         if ( lpDispAttrib->cType == 'S' &&
              (lpDispAttrib->bHidden == FALSE || pchShowHidden[ 0 ] == 'Y') )
         {
            break;
         }

         lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpDispAttrib->hNextOD_Attrib );
      }

      if ( lpDispAttrib == 0 )
      {
         lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
         while ( lpDispAttrib )
         {
            if ( lpDispAttrib->cType != 'B' &&
                 (lpDispAttrib->bHidden == FALSE || pchShowHidden[ 0 ] == 'Y') )
            {
               break;
            }

            lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpDispAttrib->hNextOD_Attrib );
         }
      }

      if ( lpDispAttrib == lpViewAttrib )
      {
         SetAttributeFromAttribute( vInstance, szlEntity, szlFirstAttr,
                                    vInstance, szlAttrib, szlValue );
         RefreshCtrl( vSubtask, "OI_ListBox1" );
      }

      RefreshCtrl( vSubtask, "OI_Edit1" );
      RefreshCtrl( vSubtask, "OI_ListBox2" );
      DropView( vWork );
   }

   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=ViewOI_Prebuild
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ViewOI_Prebuild
//
//   PURPOSE:   This operation takes the built object instance and
//              moves the mapping name to the subtask level so
//              multiple object instance may be viewed at once, it
//              also creates a task name of the object instance ID
//              so multiple requests to view the same instance can
//              simply activate the open window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ViewOI_Prebuild( zVIEW vSubtask )
{
   zVIEW   vBrowser;
   zVIEW   vInstance;
   zCHAR   szInstanceID[ 20 ];

   // Get the view to the object instance instance
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
   DropNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
   SetNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   GetStringFromAttribute( szInstanceID, sizeof( szInstanceID ), vBrowser, szlName, szlObjectInstanceID );
   SetNameForView( vSubtask, szInstanceID, vSubtask, zLEVEL_TASK );
   return( 0 );

}

//./ ADD NAME=ViewOI_Exit
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ViewOI_Exit
//
//   PURPOSE:   This operation drops the object instance being viewed
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ViewOI_Exit( zVIEW vSubtask )
{
   zVIEW  vInstance;

   // Get the view to the object instance instance
   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   DropObjectInstance( vInstance );
   return( 0 );
}

//./ ADD NAME=SetTraceLevel
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: SetTraceLevel
//
//   PURPOSE:   This operation changes the dbhandler trace level for a task.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SetTraceLevel( zVIEW vSubtask )
{
   LPTASK lpTask;
   zVIEW  vTZOBRWOO;
   zLONG  lTraceLevel;

   // Get the view to the object instance instance
   GetViewByName( &vTZOBRWOO, szlTZOBRWOO, vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &lpTask, vTZOBRWOO, szlTask, szlPointer );
   GetIntegerFromAttribute( &lTraceLevel, vTZOBRWOO, "Root", "TraceLevel" );
   lpTask->nDBHandlerTraceLevel = (zSHORT) lTraceLevel;

   return( 0 );

}

//./ ADD NAME=GetTraceLevel
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: GetTraceLevel
//
//   PURPOSE:   This operation retrieves the dbhandler trace level for a task.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
GetTraceLevel( zVIEW vSubtask )
{
   LPTASK lpTask;
   zVIEW  vTZOBRWOO;

   // Get the view to the object instance instance
   GetViewByName( &vTZOBRWOO, szlTZOBRWOO, vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &lpTask, vTZOBRWOO, szlTask, szlPointer );
   SetAttributeFromInteger( vTZOBRWOO, "Root", "TraceLevel", (zLONG) lpTask->nDBHandlerTraceLevel );
   return( 0 );
}

//./ ADD NAME=fnDeleteExcludeEntityInstance
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: fnDeleteExcludeEntityInstance
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
fnDeleteExcludeEntityInstance( zVIEW vSubtask, zBOOL bDelete )
{
   zVIEW vInstance;
   zVIEW vID;
   zVIEW vWork;
   LPVIEWENTITY     lpViewEntity;
   LPENTITYINSTANCE lpEntityInstance;
   LPENTITYINSTANCE lpTwinInstance;
   LPENTITYINSTANCE lpParentInstance;
   zCHAR szMessage[ zSHORT_MESSAGE_LTH + 1 ];
   zSHORT nRC;

   // Get the view to the object instance instance
   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );

   // Find the entity and the attrib in the view object
   GetIntegerFromAttribute( (zPLONG) &lpEntityInstance, vInstance, szlEntity, szlPointer );
   GetIntegerFromAttribute( (zPLONG) &vID, vInstance, szlName, szlID );
   vID = (zVIEW) SysGetPointerFromHandle( vID );
   lpParentInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hParent );
   lpTwinInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextTwin );
   while ( lpTwinInstance && lpTwinInstance->u.nInd.bHidden )
      lpTwinInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpTwinInstance->hNextTwin );

   if ( lpTwinInstance == 0 )
   {
      lpTwinInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hPrevTwin );
      while ( lpTwinInstance && lpTwinInstance->u.nInd.bHidden )
         lpTwinInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpTwinInstance->hPrevTwin );
   }

   strcpy_s( szMessage, "OK to continue with " );
   if ( bDelete )
      strcat_s( szMessage, "Delete " );
   else
      strcat_s( szMessage, "Exclude " );

   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );
   strcat_s( szMessage, "Entity '" );
   strcat_s( szMessage, lpViewEntity->szName );
   strcat_s( szMessage, "'" );
   if ( OperatorPrompt( vSubtask, "Confirmation",
                        szMessage, 1, zBUTTONS_YESNO,
                        zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_YES )
   {
      CreateViewFromViewForTask( &vWork, vID, vSubtask );
      fnEstablishViewForInstance( vWork, 0, lpEntityInstance );
      if ( bDelete )
         nRC = DeleteEntity( vWork, lpViewEntity->szName, zREPOS_NONE );
      else
         nRC = ExcludeEntity( vWork, lpViewEntity->szName, zREPOS_NONE );

      if ( nRC != zCALL_ERROR )
      {
         DropObjectInstance( vInstance );
         fnBuildObjectInstanceForView( vSubtask, 0 );

         // Now that we have built the new object instance, we must
         // move its name down to the subtask level
         GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
         DropNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
         SetNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
         SetSelectStateOfEntity( vInstance, szlEntity, 1 );
      }

      DropView( vWork );
   }

   return( 0 );
}

//./ ADD NAME=DeleteEntityInstance
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: DeleteEntityInstance
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DeleteEntityInstance( zVIEW vSubtask )
{
   return( fnDeleteExcludeEntityInstance( vSubtask, 1 ) );
}

//./ ADD NAME=ExcludeEntityInstance
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ExcludeEntityInstance
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ExcludeEntityInstance( zVIEW vSubtask )
{
   return( fnDeleteExcludeEntityInstance( vSubtask, 0 ) );
}

//./ ADD NAME=SelectCreateEntity
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: SelectCreateEntity
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SelectCreateEntity( zVIEW vSubtask )
{
   zVIEW vInstance;

   // Get the view to the object instance instance
   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
   SetNameForView( vSubtask, szlVIEWOI, vSubtask, zLEVEL_TASK );

   return( 0 );
}

//./ ADD NAME=PrebuildCreateEntity
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: PrebuildCreateEntity
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PrebuildCreateEntity( zVIEW vSubtask )
{
   zVIEW vInstance;
   LPVIEWENTITY     lpViewEntity;
   LPENTITYINSTANCE lpEntityInstance;

   // Get the view to the object instance instance
   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
   DropNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
   SetNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );

   // Find the entity and the attrib in the view object
   GetIntegerFromAttribute( (zPLONG) &lpEntityInstance, vInstance, szlEntity, szlPointer );
   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );
   if ( SetCursorFirstEntity( vInstance, szlWkEntity, 0 ) == zCURSOR_SET )
   {
      while ( DeleteEntity( vInstance, szlWkEntity, zREPOS_NEXT ) == zCURSOR_SET );
   }

   CreateEntity( vInstance, szlWkEntity, zPOS_AFTER );
   SetAttributeFromString( vInstance, szlWkEntity, szlName, lpViewEntity->szName );
   SetSelectStateOfEntity( vInstance, szlWkEntity, 1 );
   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
   while ( lpViewEntity )
   {
      if ( lpViewEntity->hParent == lpEntityInstance->hViewEntity )
      {
         CreateEntity( vInstance, szlWkEntity, zPOS_AFTER );
         SetAttributeFromString( vInstance, szlWkEntity, szlName, lpViewEntity->szName );
      }

      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
   }

   return( 0 );
}

//./ ADD NAME=CreateEntityInstance
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: CreateEntityInstance
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CreateEntityInstance( zVIEW vSubtask )
{
   zVIEW vInstance;
   zVIEW vID;
   zVIEW vWork;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpEntityInstance;
   LPVIEWENTITY     lpViewEntity;
   zCHAR  szEntityName[ 34 ];
   zSHORT nRC;

   // Get the view to the object instance instance
   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   GetIntegerFromAttribute( (zPLONG) &vID, vInstance, szlName, szlID );
   vID = (zVIEW) SysGetPointerFromHandle( vID );
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vInstance, szlWkEntity, szlName );
   CreateViewFromViewForTask( &vWork, vID, vSubtask );
   // Find the entity and the attrib in the view object
   GetIntegerFromAttribute( (zPLONG) &lpEntityInstance, vInstance, szlEntity, szlPointer );
   lpViewEntityCsr = fnEstablishViewForInstance( vWork, 0, lpEntityInstance );
   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );

   if ( zstrcmp( szEntityName, lpViewEntity->szName ) == 0 )
      nRC = CreateEntity( vWork, szEntityName, zPOS_AFTER );
   else
      nRC = CreateEntity( vWork, szEntityName, zPOS_FIRST );

   if ( nRC != zCALL_ERROR )
   {
      DropObjectInstance( vInstance );
      fnBuildObjectInstanceForView( vSubtask, 0 );
      // Now that we have built the new object instance, we must
      // move its name down to the subtask level
      GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
      DropNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
      GetViewByName( &vSubtask, szlVIEWOI, vSubtask, zLEVEL_TASK );
      DropNameForView( vSubtask, szlVIEWOI, vSubtask, zLEVEL_TASK );
      SetNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
      SetSelectStateOfEntity( vInstance, szlEntity, 1 );
   }

   DropView( vWork );

   return( 0 );
}

//./ ADD NAME=UpdateIndicators
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: UpdateIndicators
//
//   PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UpdateIndicators( zVIEW vSubtask )
{
   zVIEW vInstance;
   LPENTITYINSTANCE lpEntityInstance;
   zLONG lFlag;
   zCHAR szPtr[ 20 ];

   // Get the view to the object instance instance
   GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );

   GetIntegerFromAttribute( (zPLONG) &lpEntityInstance, vInstance,
                            szlEntity, szlPointer );

   sprintf_s( szPtr, sizeof( szPtr ), "0x%8lx", (zLONG) lpEntityInstance );
   SetAttributeFromString( vInstance, szlEntity, szlInstancePointer, szPtr );

   GetIntegerFromAttribute( &lFlag, vInstance, szlEntity, szlCreatedFlag );
   if ( lFlag )
      lpEntityInstance->u.nInd.bCreated = 1;
   else
      lpEntityInstance->u.nInd.bCreated = 0;

   GetIntegerFromAttribute( &lFlag, vInstance, szlEntity, szlUpdatedFlag );
   if ( lFlag )
      lpEntityInstance->u.nInd.bUpdated = 1;
   else
      lpEntityInstance->u.nInd.bUpdated = 0;

   GetIntegerFromAttribute( &lFlag, vInstance, szlEntity, szlDeletedFlag );
   if ( lFlag )
      lpEntityInstance->u.nInd.bDeleted = 1;
   else
      lpEntityInstance->u.nInd.bDeleted = 0;

   GetIntegerFromAttribute( &lFlag, vInstance, szlEntity, szlExcludedFlag );
   if ( lFlag )
      lpEntityInstance->u.nInd.bExcluded = 1;
   else
      lpEntityInstance->u.nInd.bExcluded = 0;

   GetIntegerFromAttribute( &lFlag, vInstance, szlEntity, szlIncludedFlag );
   if ( lFlag )
      lpEntityInstance->u.nInd.bIncluded = 1;
   else
      lpEntityInstance->u.nInd.bIncluded = 0;

   GetIntegerFromAttribute( &lFlag, vInstance, szlEntity, szlRecordOwnerFlag );
   if ( lFlag )
      lpEntityInstance->u.nInd.bRecordOwner = 1;
   else
      lpEntityInstance->u.nInd.bRecordOwner = 0;

   GetIntegerFromAttribute( &lFlag, vInstance, szlEntity, szlHiddenFlag );
   if ( lFlag )
      lpEntityInstance->u.nInd.bHidden = TRUE;
   else
      lpEntityInstance->u.nInd.bHidden = FALSE;

   return( 0 );
}

//./ ADD NAME=fnEstablishViewForInstance
// Source Module=tzobrwad.c
////////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:    fnEstablishViewForInstance
//
//  RETURNS:     ViewEntityCsr established
//
//  PURPOSE:     THIS CODE HAS BEEN COPIED AND MODIFIED FROM THE
//               CODE IN KZOEEIAA.C IT MAY REQUIRE MODIFICATION IN THE
//               FUTURE IF UNDERLYING CHANGES ARE MADE IN CORE ITSELF.
//
////////////////////////////////////////////////////////////////////////////////
//./ END + 4
LPVIEWENTITYCSR
fnEstablishViewForInstance( zVIEW            lpView,
                            LPVIEWENTITYCSR  lpViewEntityCsr,
                            LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWENTITYCSR  lpWorkCsr;
   LPVIEWENTITYCSR  lpRecursiveCsr;
   LPENTITYINSTANCE lpWorkInstance1;
   LPENTITYINSTANCE lpWorkInstance2;
   LPVIEWENTITY     lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );
   ResetView( lpView );

   // If no entity cursor passed, establish one.
   if ( lpViewEntityCsr == 0 )
   {
      LPVIEWCSR lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( lpView->hViewCsr );

      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hFirstOD_EntityCsr );
      lpViewEntityCsr += (lpViewEntity->nHierNbr - 1);
   }

   // First, take care of the leveling problem
   if ( lpViewEntityCsr->nLevel != lpEntityInstance->nLevel )
   {
      while ( lpViewEntityCsr->nLevel < lpEntityInstance->nLevel )
      {
         LPVIEWCSR    lpViewCsr;
         LPVIEWENTITY lpWorkViewEntity;

         // Until we get a cursor whose level matches the target instance
         // we want to establish, we continually find the highest level
         // recursive child which is greater than the current entity csr
         // level and set subobject views down to that child.
         lpWorkInstance1 = lpEntityInstance;
         lpWorkInstance2 = 0;
         while ( lpWorkInstance1->nLevel != lpViewEntityCsr->nLevel )
            lpWorkInstance1 = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpWorkInstance1->hParent );

         while ( lpWorkInstance2 == 0 )
         {
            lpWorkViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpWorkInstance2->hViewEntity );

            if ( lpViewEntity->bRecursive )
               lpWorkInstance2 = lpWorkInstance1;

            lpWorkInstance1 = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpWorkInstance1->hParent );
         }

         // Now find the cursor matching recursive child lpWorkInstance2
         lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( lpView->hViewCsr );
         lpRecursiveCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hFirstOD_EntityCsr );
         lpWorkViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpWorkInstance2->hViewEntity );
         lpRecursiveCsr += (lpWorkViewEntity->nHierNbr - 1);
         lpWorkCsr = lpRecursiveCsr;

         // Establish the cursors for the recursive child up to the
         // recursive parent.
         lpWorkInstance1 = lpWorkInstance2;
         while ( lpWorkCsr && lpWorkCsr->nLevel == lpWorkInstance1->nLevel )
         {
            lpWorkCsr->hEntityInstance = (LPENTITYINSTANCE) SysGetHandleFromPointer( lpWorkInstance1 );
            lpWorkCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWorkCsr->hParent );
            lpWorkInstance1 = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpWorkInstance1->hParent );
         }

         // Now we set the view to the subobject in question and
         // continue until the target entity comes into view
         lpWorkViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpWorkInstance2->hViewEntity );
         SetViewToSubobject( lpView, lpWorkViewEntity->szName );
      }
   }

   // Now that we are done, establish any necessary cursors up
   // the parent chain.
   lpWorkCsr = lpViewEntityCsr;
   while ( lpWorkCsr && lpWorkCsr->nLevel == lpEntityInstance->nLevel )
   {
      lpWorkCsr->hEntityInstance = (LPENTITYINSTANCE) SysGetHandleFromPointer( lpEntityInstance );
      lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hParent );
      lpWorkCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWorkCsr->hParent );
   }

   // Return success
   return( lpViewEntityCsr );
}

//./ ADD NAME=fnSetInstanceOwnerForName
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: fnSetInstanceOwnerForName
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnSetInstanceOwnerForName( LPVIEWNAME lpViewName, zVIEW vBrowser )
{
   LPANCHOR    AnchorBlock;
   LPVIEWOI    lpViewOI;
   LPVIEWCSR   lpViewCsr;
   zVIEW       lpView = (zVIEW) SysGetPointerFromHandle( lpViewName->hView );

   AnchorBlock = SysGetAnchorBlock( );
   if ( lpView->hViewCsr )
   {
      lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( lpView->hViewCsr );
      lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
      if ( lpViewOI->hTask != AnchorBlock->hMainTask )
      {
         SetAttributeFromString( vBrowser, szlName, szlInstanceOwner, szlTask );
      }
      else
      {
         lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( lpViewOI->hFirstViewCsr );
         while ( lpViewCsr )
         {
            lpView = (zVIEW) SysGetPointerFromHandle( lpViewCsr->hView );

            if ( lpView->bApplicationView )
               break;

            lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( lpViewCsr->hNextViewCsr );
         }

         if ( lpViewCsr )
            SetAttributeFromString( vBrowser, szlName, szlInstanceOwner, szlApplication );
         else
            SetAttributeFromString( vBrowser, szlName, szlInstanceOwner, szlTask );
      }
   }
   else
   {
      if ( lpView->hTask == AnchorBlock->hMainTask )
      {
         SetAttributeFromString( vBrowser, szlName, szlInstanceOwner, szlSystem );
      }
      else
      {
         SetAttributeFromString( vBrowser, szlName, szlInstanceOwner, szlTask );
      }
   }

   return( 0 );
}

//
// Following operation doesn't seem to be used anywhere as of 3/9/93 -- dgc.
// the following is used in the debugger... please see jeff before altering
//
zOPER_EXPORT zSHORT OPERATION
StageSingleTask( zVIEW vSubtask, zVIEW vViewToStage )
{
   LPANCHOR AnchorBlock;
   LPTASK   lpTask;
   LPTASK   lpMyTask;
   zVIEW    vBrowser = 0;
   zVIEW    lpView;
   zLONG    lViewCount;

   AnchorBlock = SysGetAnchorBlock( );
   lpMyTask = (LPTASK) SysGetTaskFromView( vSubtask );

   // Go through the task block we want and make sure we can share its memory
   for ( lpTask = SfGetNextTask( 0 );
         lpTask && lpTask != lpMyTask;
         lpTask = SfGetNextTask( lpTask ) )
   {
      ; // Nothing needs to be done here.
   }

   if ( lpTask && lpTask == lpMyTask )
   {
      // Validate that the view passed is valid.
      if ( SysValidAddress( vViewToStage, sizeof( ViewRecord ), 1 ) )
      {
         if ( vViewToStage->nTableID != iView )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            return( -1 );
         }
      }
      else
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      // Activate the browser object
      if ( GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK ) > 0 )
      {
         DropObjectInstance( vBrowser );
      }
      ActivateEmptyObjectInstance( &vBrowser, szlTZOBRWOO, vSubtask, zSINGLE );
      SetNameForView( vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );

      // Create the root entity
      CreateEntity( vBrowser, szlRoot, zPOS_AFTER );
      lViewCount = 0;
      CreateEntity( vBrowser, szlTask, zPOS_AFTER );
      SetAttributeFromInteger( vBrowser, szlTask, szlPointer, (zLONG) lpTask );
      SetAttributeFromInteger( vBrowser, szlTask, szlID, (zLONG) SysGetHandleFromPointer( lpTask ) );
      if ( lpTask->szTaskTitle )
      {
         SetAttributeFromString( vBrowser, szlTask, szlName, lpTask->szTaskTitle );
      }

      // Go through and get the specific view requested from the task
      lpView = (zVIEW) SysGetPointerFromHandle( lpTask->hFirstView );
      while( lpView != vViewToStage )
         lpView = (zVIEW) SysGetPointerFromHandle( lpView->hNextView );

      CreateEntity( vBrowser, szlName, zPOS_AFTER );
      SetAttributeFromInteger( vBrowser, szlName, szlID,
                               (zLONG) SysGetHandleFromPointer( lpView ) );
      if ( lpView->hViewOD )
      {
         LPVIEWOD lpViewOD = (LPVIEWOD) SysGetPointerFromHandle( lpView->hViewOD );

         SetAttributeFromString( vBrowser, szlName, szlObjectType, lpViewOD->szName );
      }

      if ( lpView->hViewCsr )
      {
         LPVIEWCSR lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( lpView->hViewCsr );

         SetAttributeFromInteger( vBrowser, szlName, szlObjectInstanceID,
                                  (zLONG) SysGetPointerFromHandle( lpViewCsr->hViewOI ) );
      }

      SetAttributeFromString( vBrowser, szlName, szlLevel, "Subtask" );

      // DGC 11/7/96
      // I'm taking out the following line because it appears to be wrong.
      // The argument lpView should be lpViewName but there is no lpViewName.
      // Since this error should cause a crash, I'm assuming that none of
      // this code is executed; therefore I'm adding a message box.
      //fnSetInstanceOwnerForName( lpView, vBrowser );
      //SysMessageBox( "Zeidon Browser", "Executing dead code", 1 );
   }

   // Reset the browser view
   ResetView( vBrowser );

   zwTZOBRWAD_ActivateOIDisplay( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SorryNotDone( zVIEW vSubtask )
{
   OperatorPrompt( vSubtask, "Object Browser",
                   "Sorry, the option you selected has\nnot been implemented yet!",
                   0, 0, 0, zICON_INFORMATION );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildTextForGraph( zVIEW   vSubtask,
                              zCPCHAR cpcSpecifyColor )
{
   zVIEW            vTZZOHDGO;
   zVIEW            vTZZOHDGV;
   zVIEW            vProfile;
   zVIEW            vFlatCompare = 0;
   zCHAR            szMsg[ 40 ];
   zPCHAR           pchShowHidden;
   LPVIEWCSR        lpViewCsr;
   LPVIEWOD         lpViewOD;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPENTITYINSTANCE lpEntityInstance;
   LPVIEWATTRIB     lpDispAttrib;
   zLONG            lPrevTwins, lNextTwins;
   zSHORT           nLastLevel;

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   GetAddrForAttribute( &pchShowHidden, vProfile, "OBR", "ShowHidden" );

   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   if ( cpcSpecifyColor )
   {
      if ( zstrchr( cpcSpecifyColor, '1' ) )
         GetViewByName( &vFlatCompare, "__FlatCompare1", vSubtask, zLEVEL_TASK );
      else
         GetViewByName( &vFlatCompare, "__FlatCompare2", vSubtask, zLEVEL_TASK );

      CreateViewFromViewForTask( &vFlatCompare, vFlatCompare, vSubtask );
   }

   CreateViewFromViewForTask( &vTZZOHDGV, vTZZOHDGV, vSubtask );
   CreateViewFromViewForTask( &vTZZOHDGO, vTZZOHDGO, vSubtask );
   while ( ResetViewFromSubobject( vTZZOHDGO ) == 0 )
   {
   }

   SetCursorFirstEntity( vTZZOHDGO, szlHD_ChildNode, 0 );

   fnSwapTaskContext( vSubtask );

   lpViewOD        = (LPVIEWOD) SysGetPointerFromHandle( vTZZOHDGV->hViewOD );
   lpViewEntity    = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
   lpViewCsr       = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );

   nLastLevel = lpViewEntity->nLevel;
   while ( lpViewEntity )
   {
      if ( lpViewEntity->nLevel > nLastLevel )
      {
         SetViewToSubobject( vTZZOHDGO, szlHD_ChildNode );
         nLastLevel++;
      }
      else
      if ( lpViewEntity->nLevel < nLastLevel )
      {
         while ( nLastLevel > lpViewEntity->nLevel )
         {
            ResetViewFromSubobject( vTZZOHDGO );
            nLastLevel--;
         }

         SetCursorNextEntity( vTZZOHDGO, szlHD_Node, 0 );
      }
      else
         SetCursorNextEntity( vTZZOHDGO, szlHD_Node, 0 );

      if ( lpViewEntityCsr == 0 || (LPVIEWENTITY)
           SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity ) != lpViewEntity )
      {
         SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlTitle, "" );
         SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlText, "(out of view)" );
         SetAttributeFromInteger( vTZZOHDGO, szlHD_Node, szlColor, 2 );
      }
      else
      {
         // Call CheckExistenceOfEntity for each visible cursor which is
         // currently in an unset state to force the cursor to be positioned.
         if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
            CheckExistenceOfEntity( vTZZOHDGV, lpViewEntity->szName );

         // If there is no instance for this entity, show it as null.
         if ( lpViewEntityCsr->hEntityInstance == NULL_CSR )
         {
            LPVIEWENTITYCSR  lpWkCsr;
            LPENTITYINSTANCE lpWkEntityInstance = 0;

            SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlTitle, "" );
            lpWkCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hParent );
            while ( lpWkCsr && lpWkCsr->hEntityInstance == NULL_CSR )
               lpWkCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWkCsr->hParent );

            if ( lpWkCsr )
               lpWkEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpWkCsr->hEntityInstance );

            if ( lpWkCsr && lpWkEntityInstance->u.nInd.bHidden )
            {
               SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlText, "(undefined)" );
            }
            else
            {
               SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlText, "(null)" );
               SetAttributeFromInteger( vTZZOHDGO, szlHD_Node, szlColor, 1 );
            }
         }
         else
         {
            zBOOL bSetHidden;

            lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewEntityCsr->hEntityInstance );
            lPrevTwins = 0;
            if ( lpEntityInstance )
               lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hPrevTwin );

            while ( lpEntityInstance )
            {
               if ( lpEntityInstance->u.nInd.bHidden == FALSE || pchShowHidden[ 0 ] == 'Y' )
               {
                  lPrevTwins++;
               }

               lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hPrevTwin );
            }

            lNextTwins = 0;
            lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewEntityCsr->hEntityInstance );
            lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextTwin );
            while ( lpEntityInstance )
            {
               if ( lpEntityInstance->u.nInd.bHidden == FALSE || pchShowHidden[ 0 ] == 'Y' )
               {
                  lNextTwins++;
               }

               lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextTwin );
            }

            lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewEntityCsr->hEntityInstance );

            // If we are showing hidden entities then turn off hidden flag.
            if ( lpEntityInstance->u.nInd.bHidden && pchShowHidden[ 0 ] == 'Y' )
            {
               lpEntityInstance->u.nInd.bHidden = FALSE;
               bSetHidden = TRUE;
            }
            else
               bSetHidden = FALSE;

            if ( lpEntityInstance->u.nInd.bHidden )
            {
               SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlTitle, "" );
               if ( lPrevTwins == 0 && lNextTwins == 0 )
               {
                  SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlText, "(null)" );
                  SetAttributeFromInteger( vTZZOHDGO, szlHD_Node, szlColor, 1 );
               }
               else
                  SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlText, "(undefined)" );
            }
            else
            {
               zltoa( lPrevTwins + 1, szMsg, sizeof( szMsg ) );
               strcat_s( szMsg, sizeof( szMsg ), " of " );
               zltoa( lPrevTwins + 1 + lNextTwins, szMsg + 20, sizeof( szMsg ) - 20 );
               strcat_s( szMsg, sizeof( szMsg ), szMsg + 20 );
               SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlTitle, szMsg );

               zLONG lAbsolutePos = 0;
               if ( cpcSpecifyColor && vFlatCompare )
               {
                  LPENTITYINSTANCE lpEI = lpEntityInstance;

                  while ( lpEI->hPrevHier )
                  {
                     lAbsolutePos++;
                     lpEI = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEI->hPrevHier );
                  }
               }

               if ( cpcSpecifyColor && vFlatCompare &&
                    SetCursorFirstEntityByInteger( vFlatCompare, szlHD_Node, szlZKey, lAbsolutePos, 0 ) >= zCURSOR_SET )
               {
                  zLONG lColor;

                  GetIntegerFromAttribute( &lColor, vFlatCompare, szlHD_Node, szlColor );
                  SetAttributeFromInteger( vTZZOHDGO, szlHD_Node, szlColor, lColor );
               // SetAttributeFromAttribute( vTZZOHDGO, szlHD_Node, szlColor, vFlatCompare, szlHD_Node, szlColor );
               }
               else
               if ( lpEntityInstance->hPrevVsn || lpEntityInstance->u.nInd.bTemporal )
               {
                  SetAttributeFromInteger( vTZZOHDGO, szlHD_Node, szlColor, 3 );
               }
               else
               {
                  SetAttributeFromInteger( vTZZOHDGO, szlHD_Node, szlColor, 0 );
               }

               lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
               while ( lpDispAttrib )
               {
                  if ( lpDispAttrib->cType == 'S' && lpDispAttrib->bHidden == FALSE )
                  {
                     break;
                  }

                  lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpDispAttrib->hNextOD_Attrib );
               }

               if ( lpDispAttrib == 0 )
               {
                  lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
                  while ( lpDispAttrib )
                  {
                     if ( lpDispAttrib->cType != 'B' && lpDispAttrib->bHidden == FALSE )
                     {
                        break;
                     }

                     lpDispAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpDispAttrib->hNextOD_Attrib );
                  }
               }

               if ( lpDispAttrib )
               {
                  // if the display attribute is too long, we can get a message that we don't want (text too long for the
                  // HD work object).  So instead of using SetAttributeFromAttribute, use GetVariable and SetVariable.

//                SetAttributeFromAttribute( vTZZOHDGO, szlHD_Node, szlText,
//                                           vTZZOHDGV, lpViewEntity->szName, lpDispAttrib->szName );

                  zCHAR szText[ 512 ]; // room for overflow

                  GetVariableFromAttribute( szText, 0, zTYPE_STRING, 254,
                                            vTZZOHDGV, lpViewEntity->szName,
                                            lpDispAttrib->szName, 0, zUSE_DEFAULT_CONTEXT );
                  SetAttributeFromVariable( vTZZOHDGO, szlHD_Node, szlText,
                                            szText, zTYPE_STRING, 0, 0, zUSE_DEFAULT_CONTEXT );
               }
            }

            // If we need to reset the hidden flag then do it.
            if ( bSetHidden )
               lpEntityInstance->u.nInd.bHidden = TRUE;
         }

         lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
      }

      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
   }

   fnSwapTaskContext( vSubtask );

   if ( vFlatCompare )
      DropView( vFlatCompare );

   DropView( vTZZOHDGO );
   DropView( vTZZOHDGV );
   DropView( vProfile );
   return( 0 );
}

void OPERATION
AppendSlash( zPCHAR pchDirectory )
{
   zSHORT k;

   k = (zSHORT) zstrlen( pchDirectory );
   if ( pchDirectory[ k - 1 ] != '\\' )
   {
      pchDirectory[ k ] = '\\';
      pchDirectory[ k + 1 ] = 0;
   }
}

//./ ADD NAME=GetWKS_FileName
// Source Module=tzcmwkso.c
////////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  GetWKS_FileName
//
//  PURPOSE:    To retrieve the Zeidon WorkStation( RepositoryClient )
//              file name
//
//  PARAMETERS: pchZeidonWKS - a pointer to a string which is returned
//                with the fully qualified RepositoryClient File Name.
//
//  RETURNS:    0 - FileName successfully retrieved
//             -1 - Error encountered
//
////////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
GetWKS_FileName( zPCHAR pchZeidonWKS )
{
   if ( SysGetEnvVar( pchZeidonWKS, "ZEIDON",  zMAX_FILENAME_LTH + 1 ) == 0 )
   {
      AppendSlash( pchZeidonWKS );
      strcat_s( pchZeidonWKS, zMAX_FILENAME_LTH + 1, "TZCMWKS8.POR" );
      return( 0 );
   }
   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
ActivateWorkStation( zPVIEW vTZCMWKSO, zVIEW vQual, zLONG lControl )
{
   zCHAR  szZeidonWKS[ zMAX_FILENAME_LTH + 1 ];

   if ( GetWKS_FileName( szZeidonWKS ) == 0 )
      return( ActivateOI_FromFile( vTZCMWKSO, "TZCMWKSO", vQual, szZeidonWKS, lControl ) );
   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_OpenDescDisp( zVIEW vBrowser, zVIEW lpView )
{
   zVIEW vTZZOLODO_Desc;
   zVIEW vTZCMLPLO;
   zCHAR szFileName[ zMAX_FILESPEC_LTH + 1 ];

   zCHAR szObject[ 40 ];
   zCHAR szAppName[ 40 ];
   zCHAR szAppName2[ 40 ];
   zCHAR szMsg[ zLONG_MESSAGE_LTH + 1 ];

   zVIEW vTZCMWKSO;
   zSHORT nRC;
   zSHORT nRC_cmp;

   //2002.02.21  FH
   //enable display of description on shop of OI from vml debugger:
   // on start from vml debugger the view TZOBRWOO does not have a valid application
   // --> take the entity "LPLR" from OI "TaskLPLR" in this case.
   if (CheckExistenceOfEntity( lpView, "Application" ) == 0)
   {
      SetCursorFirstEntityByAttr( lpView, "Application", "Name", lpView, "Name", "ApplicationName", "" );
      GetStringFromAttribute( szAppName, sizeof( szAppName ), lpView, "Application", "Name" );

   }
   else
   {
      nRC = GetViewByName( &vTZCMLPLO, "TaskLPLR", vBrowser, zLEVEL_TASK );
      if ( nRC < 0 )
         return( -1 );

      GetStringFromAttribute( szAppName, sizeof( szAppName ), vTZCMLPLO, "LPLR", "Name" );
   }

   GetStringFromAttribute( szObject, sizeof( szObject ), lpView, "Name", "ObjectType" );

   if ( ActivateWorkStation( &vTZCMWKSO, vBrowser, zLEVEL_TASK ) < 0 )
   {
      MessageSend( vBrowser, "CM00107", "Object Browser",
                   "Descriptions of OI cannot be shown because local workstation does not have application source.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( -1 );
   }

   if ( zstrcmp( szAppName, "Zeidon_Tools" ) == 0 || zstrcmp( szAppName, "Zeidon System" ) == 0 )
   {
      nRC = SetCursorFirstEntity( vTZCMWKSO, "LPLR", "");
      nRC_cmp = 0;
      while ( nRC >= zCURSOR_SET && nRC_cmp == 0 )
      {
         GetStringFromAttribute( szAppName2, sizeof( szAppName2 ), vTZCMWKSO, "LPLR", "Name" );
         nRC_cmp = ((zstrncmp( szAppName2, "Zeid", 4 ) == 0) ||
                    (zstrncmp( szAppName2, "zeid", 4 ) == 0) ||
                    (zstrncmp( szAppName2, "ZEID", 4 ) == 0));
         // nRC_cmp = 0 --> no match.
         if ( nRC_cmp == 0 )
           nRC = SetCursorNextEntity( vTZCMWKSO, "LPLR", "");
      }

      nRC_cmp = nRC_cmp - 1;

   }
   else
      nRC_cmp = SetCursorFirstEntityByString( vTZCMWKSO, "LPLR", "Name", szAppName, 0 );

   // nRC_cmp >= 0 --> found a match.
   if ( nRC_cmp < zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Descriptions of OI cannot be shown because " );
      strcat_s( szMsg, sizeof( szMsg ), "Local workstation does not have application source (Appl: " );
      strcat_s( szMsg, sizeof( szMsg ), szAppName );
      strcat_s( szMsg, sizeof( szMsg ), ")." );
      MessageSend( vBrowser, "CM00107", "Object Browser", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( -1 );
   }

   GetStringFromAttribute( szFileName, sizeof( szFileName ), vTZCMWKSO, "LPLR", "MetaSrcDir");

   DropView( vTZCMWKSO );
   AppendSlash( szFileName );
   strcat_s( szFileName, sizeof( szFileName ), szObject );
   strcat_s( szFileName, sizeof( szFileName ), ".LOD" );

   if ( SysOpenFile( vBrowser, szFileName, COREFILE_EXIST ) == -1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "The LOD-File with Descriptions does not exist: " );
      strcat_s( szMsg, sizeof( szMsg ), szFileName );
      MessageSend( vBrowser, "CM00107", "Object Browser", szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( -1 );
   }

   nRC = ActivateOI_FromFile( &vTZZOLODO_Desc, "TZZOLODO", vBrowser, szFileName, zLEVEL_TASK | zIGNORE_ERRORS );

   if ( nRC == 0 )
   {
      SetNameForView( vTZZOLODO_Desc, "TZZOLODO_Desc", vBrowser, zLEVEL_TASK );

      SetWindowActionBehavior( vBrowser, zWAB_StartModelessSubwindow, "TZOBRWAD", "DescDisp" );
   }

   return( 0 );
}

//./ ADD NAME=ToggleShowDesc
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ToggleShowDesc
//
//   PURPOSE:   Toggle the ShowDesc attribute in the profile object.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ToggleShowDesc( zVIEW vSubtask )
{
   zVIEW  vProfile;
   zPCHAR pchShowDesc;

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   if ( vProfile == 0 )
      return( 0 );

   GetAddrForAttribute( &pchShowDesc, vProfile, "OBR", "ShowDesc" );

   if ( pchShowDesc[ 0 ] == 'Y' )
   {
      SetAttributeFromString( vProfile, "OBR", "ShowDesc", "N" );
      SetOptionState( vSubtask, "ShowDesc", zOPTION_STATUS_CHECKED, 0 );
   }
   else
   {
      SetAttributeFromString( vProfile, "OBR", "ShowDesc", "Y" );
      SetOptionState( vSubtask, "ShowDesc", zOPTION_STATUS_CHECKED, 1 );
   }

   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=ToggleShowDesc
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: ToggleShowDescFromOI
//
//   PURPOSE:   Toggle the ShowDesc attribute in the profile object.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ToggleShowDescFromOI( zVIEW vSubtask )
{
   zVIEW  vProfile;
   zPCHAR pchShowDesc;
   zVIEW vTZOBRWOO;
   zVIEW vTZZOLODO_Desc;
   zVIEW vTZOBRWAD_Desc;

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   if ( vProfile == 0 )
      return( 0 );

   GetAddrForAttribute( &pchShowDesc, vProfile, "OBR", "ShowDesc" );

   if ( pchShowDesc[ 0 ] != 'Y' )
   {
      SetAttributeFromString( vProfile, "OBR", "ShowDesc", "Y" );
      GetViewByName( &vTZOBRWOO, szlTZOBRWOO, vSubtask, zLEVEL_TASK );

      if ( zwTZOBRWAD_OpenDescDisp( vSubtask, vTZOBRWOO ) < 0 )
        SetAttributeFromString( vProfile, "OBR", "ShowDesc", "N" );

   }
   else
   {
      if ( GetViewByName( &vTZZOLODO_Desc, "TZZOLODO_Desc", vSubtask, zLEVEL_TASK ) >= 0 )
         DropView( vTZZOLODO_Desc );

      if ( GetViewByName( &vTZOBRWAD_Desc, "TZOBRWAD_Desc", vSubtask, zLEVEL_TASK ) >= 0 )
         SetWindowActionBehavior( vTZOBRWAD_Desc, zWAB_ReturnToParent | zWAB_ProcessImmediateAction, "", "" );

      SetAttributeFromString( vProfile, "OBR", "ShowDesc", "N" );
   }

   DropView( vProfile );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_PostBuildDescDisp( zVIEW vSubtask )
{
   SetNameForView( vSubtask, "TZOBRWAD_Desc", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_PreBuildOIDisplay( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;
   zVIEW vTZZOHDGV;
   zVIEW vTZZOHDGV_S;
   zVIEW vTZOBRWOO;
   zCHAR szInstanceID[ 17 ];

   zVIEW  vProfile;
   zPCHAR pchShowDesc;

// CreateViewFromViewForTask( 0, 0, 0 );  // start view debug
   ActivateWindowPlacement( vSubtask, "ProfileXFER" );

   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_TASK );
   DropNameForView( vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_TASK );
   SetNameForView( vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
   DropNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
   SetNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   CreateViewFromViewForTask( &vTZZOHDGV_S, vTZZOHDGV, vSubtask );
   SetNameForView( vTZZOHDGV_S, szlTZZOHDGV_S, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZOBRWOO, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szInstanceID, sizeof( szInstanceID ), vTZOBRWOO, szlName, szlObjectInstanceID );
   SetNameForView( vSubtask, szInstanceID, vSubtask, zLEVEL_TASK );
   zwTZOBRWAD_BuildTextForGraph( vSubtask );

   // if the User wants to see the Descriptions, then try to
   // read the .LOD-File, OpenDescDisp...
   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   if ( vProfile )
   {
      GetAddrForAttribute( &pchShowDesc, vProfile, "OBR", "ShowDesc" );
      if ( pchShowDesc[ 0 ] == 'Y' )
      {
         if ( zwTZOBRWAD_OpenDescDisp( vSubtask, vTZOBRWOO ) < 0 )
            SetAttributeFromString( vProfile, "OBR", "ShowDesc", "N" );
      }
   }

   ResetView( vTZZOHDGO );
   DropView( vProfile );
   return( 0 );
}

zSHORT
fnSetModeEntities( zVIEW vSubtask )
{
   zVIEW vTZOBOIOO;
   zCHAR szMode[ 2 ];

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      if ( szMode[ 0 ] == 'E' )
         return( 0 );
   }
   else
   {
      ActivateEmptyObjectInstance( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zSINGLE );
      SetNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
      CreateEntity( vTZOBOIOO, szlName, zPOS_AFTER );
   }

   SetAttributeFromString( vTZOBOIOO, szlName, szlMode, "A" );  // make Attributes default (dks 2010.02.04)
   zwTZOBRWAD_BuildAttrsForGraph( vSubtask );

   // I don't know why i shouldn't be able to refresh the Radio Group??
   // (instead of refreshing each button in it)
// RefreshCtrl( vSubtask, "OI_Detail" );
   RefreshCtrl( vSubtask, "OI_Attributes" );
   RefreshCtrl( vSubtask, "OI_Twins" );
   RefreshCtrl( vSubtask, "OI_Entities" );
   RefreshCtrl( vSubtask, "OI_Markings" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_PostBuildOIDisplay( zVIEW vSubtask )
{
   zULONG  dwColors[ 5 ];
   zSHORT  nZoom;

   // Color 0 is Green     (Creatable)
   // Color 1 is Yellow    (Includable)
   // Color 2 is Lt. Gray  (Includable Path)
   // Color 3 is Blue      (Versioned or Temporal Entity)
   // Color 4 is terminator
   dwColors[ 0 ] = (zULONG) zRGB( 0, 255, 0 );
   dwColors[ 1 ] = (zULONG) zRGB( 255, 255, 0 );
   dwColors[ 2 ] = (zULONG) zRGB( 192, 192, 192 );
   dwColors[ 3 ] = (zULONG) zRGB( 0, 192, 192 ); // to see where this is set, look for szlColor, 3 );
   dwColors[ 4 ] = 0;
   HD_SetColors( vSubtask, szlOI_Hier, dwColors, 0, 0, 0, 0, 0, 1 );
   fnSetModeEntities( vSubtask );

   // Set the select set for selection in the locking entity combo
   // box to 2 so it does not conflict with the listbox displaying the
   // entity names!
   CB_SetSelectSelectSet( vSubtask, "OI_LockEntity", 2 );

   nZoom = (zSHORT) GetWindowPreferenceInteger( vSubtask, "Zoom" );
   if ( nZoom )
      HD_ZoomDiagram( vSubtask, szlOI_Hier, nZoom, 1 );

// DropView( 0 );  // end view debug

   return( 0 );
}

zCHAR HexTable[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_RefreshDescDisp( zVIEW vSubtask )
{
   zVIEW vTZZOLODO_Desc;
   zVIEW vTZOBRWAD_Desc;
   zVIEW vTZOBOIOO;
   zVIEW vTZZOHDGO;

   zCHAR  szMode[ 2 ];
   zPCHAR pchGraphEntity;

   if ( GetViewByName( &vTZZOLODO_Desc, "TZZOLODO_Desc", vSubtask, zLEVEL_TASK ) < 0 ||
        GetViewByName( &vTZOBRWAD_Desc, "TZOBRWAD_Desc", vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) < 0 )
      return( -1 );

   GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetAddrForAttribute( &pchGraphEntity, vTZZOHDGO, szlHD_Node, "DIL" );

   // position on the correct Entity:
   SetCursorFirstEntityByString( vTZZOLODO_Desc, "LOD_Entity", "Name", pchGraphEntity, "LOD" );

   if ( szMode[ 0 ] == 'A' &&
        GetSelectStateOfEntity( vTZOBOIOO, "Attrib" ) == 1 )
   {
      // position on the correct Attribute if the is selected
      SetCursorFirstEntityByAttr( vTZZOLODO_Desc, "ER_Attribute", "Name",
                                  vTZOBOIOO, "Attrib", "Name", "LOD_Entity" );
   }

   RefreshWindow( vTZOBRWAD_Desc );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildAttrsForGraph( zVIEW vSubtask )
{
   return( BuildAttrsForGraph( vSubtask, szlOI_Hier ) );
}

zSHORT
BuildAttrsForGraph( zVIEW vSubtask, zCPCHAR cpcTag )
{
   zVIEW  vTZZOHDGO;
   zVIEW  vTZZOHDGV;
   zVIEW  vTZOBOIOO;
   zVIEW  vWork;
   zVIEW  vProfile;
   zSHORT nWk1, nWk2, nWk3;
   zULONG uLth;
   zUCHAR ucWk;
   zLONG  lLinks;
   LPVIEWENTITY       lpViewEntity;
   LPVIEWENTITYCSR    lpViewEntityCsr;
   LPVIEWENTITYCSR    lpWkEntityCsr;
   LPVIEWATTRIB       lpViewAttrib;
   LPENTITYINSTANCE   lpEntityInstance;
   LPENTITYINSTANCE   lpLinkInstance;
   LPSELECTEDINSTANCE lpSelectedInstance;
   LPVIEWCSR          lpViewCsr;
   zPCHAR pchGraphEntity;
   zPCHAR pchEntityName;
   zPCHAR pchShowHidden;
   zPCHAR pchIgnoreDerivedAttrs;
   zCHAR  szLockEntity[ 34 ];
   zPCHAR pchBlob;
   zCHAR  szMode[ 2 ];
   zCHAR  szHiddenAttribute[ 34 ];
   zCHAR  szAttribute[ 512 ];
   zCHAR  szTag[ 33 ];
   zBOOL  bOI_Hier = FALSE;
   zBOOL  bSetHidden;

   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   GetAddrForAttribute( &pchGraphEntity, vTZZOHDGO, szlHD_Node, szlDIL );

   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   if ( zstrchr( cpcTag, '1' ) )
      strcpy_s( szTag, "CompareHD1" );
   else
   if ( zstrchr( cpcTag, '2' ) )
      strcpy_s( szTag, "CompareHD2" );
   else
   {
      strcpy_s( szTag, cpcTag );
      bOI_Hier = TRUE;
   }

   if ( HD_IsNodeVisible( vSubtask, szTag ) == 0 )
      CenterNode( vSubtask, szTag );

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   GetAddrForAttribute( &pchShowHidden, vProfile, "OBR", "ShowHidden" );
   GetAddrForAttribute( &pchIgnoreDerivedAttrs, vProfile, "OBR",
                        "IgnoreDerivedAttributes" );

   if ( bOI_Hier )
   {
      GetCtrlText( vSubtask, "OI_LockEntity", szLockEntity, 33 );
      if ( szLockEntity[ 0 ] )
         pchEntityName = szLockEntity;
      else
         pchEntityName = pchGraphEntity;

      if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_ANY ) > 0 )
      {
         GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
         DropObjectInstance( vTZOBOIOO );
      }
      else
      {
         szMode[ 0 ] = 'A';
         szMode[ 1 ] = 0;
      }

      if ( szMode[ 0 ] == 'A' )
      {
         SetCtrlState( vSubtask, "OI_LockEntity",
                       zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "OI_UpdateButton",
                       zCONTROL_STATUS_VISIBLE, 1 );
         SetCtrlState( vSubtask, "OI_Edit1", zCONTROL_STATUS_VISIBLE, 1 );
      }
      else
      {
         SetCtrlState( vSubtask, "OI_LockEntity",
                       zCONTROL_STATUS_VISIBLE, 1 );
         SetCtrlState( vSubtask, "OI_UpdateButton",
                       zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "OI_Edit1", zCONTROL_STATUS_VISIBLE, 0 );
         if ( szMode[ 0 ] == 'E' )
            SetCtrlState( vSubtask, "OI_LockEntity",
                          zCONTROL_STATUS_ENABLED, 1 );
         else
            SetCtrlState( vSubtask, "OI_LockEntity",
                          zCONTROL_STATUS_ENABLED, 0 );
      }

      if ( szMode[ 0 ] == 'E' || szMode[ 0 ] == 'M' )
      {
         SetCtrlState( vSubtask, "OI_ListBox1", zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "OI_ListBox2", zCONTROL_STATUS_VISIBLE, 1 );
      }
      else
      {
         SetCtrlState( vSubtask, "OI_ListBox1", zCONTROL_STATUS_VISIBLE, 1 );
         SetCtrlState( vSubtask, "OI_ListBox2", zCONTROL_STATUS_VISIBLE, 0 );
      }
   }
   else
   {
      if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_ANY ) > 0 )
         DropObjectInstance( vTZOBOIOO );

      pchEntityName = pchGraphEntity;
      szMode[ 0 ] = 'A';
      szMode[ 1 ] = 0;
   }

   ActivateEmptyObjectInstance( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zSINGLE );
   SetNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   CreateEntity( vTZOBOIOO, szlName, zPOS_AFTER );
   SetAttributeFromString( vTZOBOIOO, szlName, szlMode, szMode );
   CreateEntity( vTZOBOIOO, szlEntity, zPOS_AFTER );
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR)
                                    SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
   while ( lpViewEntityCsr )
   {
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );

      if ( szMode[ 0 ] != 'E' &&
           zstrcmp( lpViewEntity->szName, pchEntityName ) == 0 )
      {
         if ( bOI_Hier == FALSE )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, lpViewEntity->szName );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlValue, "=============================" );
         }

         break;
      }

      if ( szMode[ 0 ] == 'E' )
      {
         CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
         strcpy_s( szAttribute, sizeof( szAttribute ), "                              " );
         strcpy_s( szAttribute + (2 * (lpViewEntity->nLevel - 1)), sizeof( szAttribute ) - (2 * (lpViewEntity->nLevel - 1)), lpViewEntity->szName );
         SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
         SetAttributeFromString( vTZOBOIOO, szlAttrib, szlValue, lpViewEntity->szName );
         if ( GetSelectStateOfEntity( vTZZOHDGO, szlHD_Node ) &&
              zstrcmp( pchGraphEntity, lpViewEntity->szName ) == 0 )
         {
            SetSelectStateOfEntity( vTZOBOIOO, szlAttrib, 1 );
         }
      }

      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
   }

   if ( lpViewEntityCsr == 0 )
   {
      if ( zstrcmp( cpcTag, szlOI_Hier ) == 0 )
      {
         if ( szMode[ 0 ] == 'E' )
         {
            RefreshCtrl( vSubtask, "OI_Edit1" );
            RefreshCtrl( vSubtask, "OI_ListBox1" );
            RefreshCtrl( vSubtask, "OI_ListBox2" );
         }
      }

      zwTZOBRWAD_RefreshDescDisp( vSubtask );
      DropView( vProfile );
      return( 0 );
   }

   bSetHidden = FALSE;
   if ( pchShowHidden[ 0 ] == 'Y' )
   {
      lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewEntityCsr->hEntityInstance );

      if ( lpEntityInstance != UNSET_CSR &&
           lpEntityInstance != NULL_CSR  &&
           lpEntityInstance->u.nInd.bHidden )
      {
         lpEntityInstance->u.nInd.bHidden = FALSE;
         bSetHidden = TRUE;
      }
   }

   if ( CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL )
   {
      fnSwapTaskContext( vSubtask );

      if ( szMode[ 0 ] == 'M' )
      {
         CreateViewFromViewForTask( &vWork, vTZOBOIOO, vSubtask );
         lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewEntityCsr->hEntityInstance );
         lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
         lpSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpViewCsr->hFirstSelectedInstance );
         while ( lpSelectedInstance &&
                 ((LPENTITYINSTANCE) SysGetPointerFromHandle( lpSelectedInstance->xEntityInstance ) != lpEntityInstance ||
                   lpSelectedInstance->hViewEntity != lpEntityInstance->hViewEntity ) )
         {
            lpSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpSelectedInstance->hNextSelectedInstance );
         }

         CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
         sprintf_s( szAttribute, sizeof( szAttribute ), "Instance Ptr 0x%8lx  Key %d", (zULONG) lpEntityInstance, lpEntityInstance->ulKey );
         SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );

         if ( lpSelectedInstance && lpSelectedInstance->nSelectSet )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            strcpy_s( szAttribute, sizeof( szAttribute ), "Selected(" );
            nWk1 = 0;
            nWk2 = 0x0001;
            for ( nWk3 = 1; nWk3 < 17; nWk3++ )
            {
               if ( lpSelectedInstance->nSelectSet & nWk2 )
               {
                  if ( nWk1 )
                     strcat_s( szAttribute, sizeof( szAttribute ), "," );

                  zltoa( (zLONG) nWk3, szAttribute + 250, sizeof( szAttribute ) - 250 );
                  strcat_s( szAttribute, sizeof( szAttribute ), &szAttribute[ 250 ] );
                  nWk1++;
               }

               nWk2 <<= 1;
            }

            strcat_s( szAttribute, sizeof( szAttribute ), ")" );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
         }

         if ( lpEntityInstance->u.nInd.bCreated )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Created" );
         }

         if ( lpEntityInstance->u.nInd.bDeleted )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Deleted" );
         }

         if ( lpEntityInstance->u.nInd.bUpdated )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Updated" );
         }

         if ( lpEntityInstance->u.nInd.bIncluded )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Included" );
         }

         if ( lpEntityInstance->u.nInd.bExcluded )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Excluded" );
         }

         if ( lpEntityInstance->u.nInd.bTemporal )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Temporal Entity" );
         }
         else
         if ( lpEntityInstance->hPrevVsn )
         {
            LPENTITYINSTANCE lpTempEI;

            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            lpLinkInstance = lpEntityInstance;
            lpTempEI = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hPrevVsn );
            if ( lpTempEI->u.nInd.bPrevVsnRoot == FALSE )
            {
               LPVIEWENTITY lpViewEntity;

               lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpLinkInstance->hViewEntity );

               while ( lpTempEI->u.nInd.bPrevVsnRoot == FALSE )
               {
                  lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hParent );
                  lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpLinkInstance->hViewEntity );
                  lpTempEI = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hPrevVsn );
               }

               lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpLinkInstance->hViewEntity );
               strcpy_s( szAttribute, sizeof( szAttribute ), "Versioned by " );
               strcat_s( szAttribute, sizeof( szAttribute ), lpViewEntity->szName );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
            }
            else
            {
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Versioned" );
            }
         }
         else
         if ( lpEntityInstance->u.nInd.bPrevVersion )
         {
            lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextLinked );
            while ( lpLinkInstance && lpLinkInstance != lpEntityInstance )
            {
               if ( lpLinkInstance->hNextVsn )
                  break;

               lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hNextLinked );
            }

            if ( lpLinkInstance && lpLinkInstance->hNextVsn )
            {
               CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName,
                                       "Previous Version from another path" );
            }
            else
            {
               CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName,
                                       "??? Marked but not a Previous Version from another path" );
            }
         }

         lLinks = 0;
         lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextLinked );
         while ( lpLinkInstance && lpLinkInstance != lpEntityInstance )
         {
            zBOOL bDup;
            if ( lpLinkInstance->u.nInd.bHidden == FALSE ||
                 pchShowHidden[ 0 ] == 'Y' )
            {
               LPVIEWENTITY lpLinkViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpLinkInstance->hViewEntity );

               if ( lpLinkInstance->hViewOI == lpEntityInstance->hViewOI )
               {
                  strcpy_s( szAttribute, sizeof( szAttribute ), "Internal Link to " );
                  strcat_s( szAttribute, sizeof( szAttribute ), lpLinkViewEntity->szName );
               }
               else
               {
                  LPVIEWOI lpViewOI = (LPVIEWOI) SysGetPointerFromHandle( lpLinkInstance->hViewOI );
                  LPVIEWOD lpViewOD = (LPVIEWOD) SysGetPointerFromHandle( lpViewOI->hViewOD );

                  strcpy_s( szAttribute, sizeof( szAttribute ), "External Link to " );
                  strcat_s( szAttribute, sizeof( szAttribute ), lpViewOD->szName );
                  strcat_s( szAttribute, sizeof( szAttribute ), "." );
                  strcat_s( szAttribute, sizeof( szAttribute ), lpLinkViewEntity->szName );
               }

               bDup = 0;
               if ( SetCursorFirstEntityByString( vWork, szlAttrib, szlName, szAttribute, 0 ) == 0 )
               {
                  bDup = TRUE;
               }

               if ( bDup == FALSE )
               {
                  CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
                  SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
               }
            }

            lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hNextLinked );
            lLinks++;
         }

         if ( lLinks )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            strcpy_s( szAttribute, sizeof( szAttribute ), "Number of links is " );
            zltoa( lLinks, szAttribute + 250, sizeof( szAttribute ) - 250 );
            strcat_s( szAttribute, sizeof( szAttribute ), &szAttribute[ 250 ] );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
         }

         DropView( vWork );
      }
      else
      {
         lpViewEntity= (LPVIEWENTITY)  SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
         lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
         if ( szMode[ 0 ] == 'A' )
         {
            for ( ;
                  lpViewAttrib;
                  lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib ) )
            {
               if ( lpViewAttrib->bHidden && pchShowHidden[ 0 ] != 'Y' )
                  continue;

               // Ignore attributes with a derived operation if that's what
               // the user wants.
               if ( pchIgnoreDerivedAttrs[ 0 ] == 'Y' &&
                    lpViewAttrib->szDerivedOper[ 0 ] != 0 )
               {
                  continue;
               }

               if ( lpViewAttrib->cType == 'B' )
               {
                  if ( lpViewAttrib->bHidden )
                     uLth = 0;
                  else
                     GetAttributeLength( &uLth, vTZZOHDGV, pchEntityName, lpViewAttrib->szName );

                  if ( uLth > 60 || uLth == 0 )
                  {
                     strcpy_s( szAttribute, sizeof( szAttribute ), "(Blob, lth=" );
                     zltoa( uLth, szAttribute + 50, sizeof( szAttribute ) - 50 );
                     strcat_s( szAttribute, sizeof( szAttribute ), szAttribute + 50 );
                     strcat_s( szAttribute, sizeof( szAttribute ), ")" );
                  }
                  else
                  {
                     GetAddrForAttribute( &pchBlob, vTZZOHDGV,
                                          pchEntityName, lpViewAttrib->szName );
                     nWk1 = 2;
                     strcpy_s( szAttribute, sizeof( szAttribute ), "x'" );
                     for ( nWk2 = 0; nWk2 < (zSHORT) uLth; nWk2++ )
                     {
                        ucWk = pchBlob[ nWk2 ];
                        ucWk >>= 4;
                        ucWk = HexTable[ ucWk ];
                        szAttribute[ nWk1++ ] = ucWk;
                        ucWk = pchBlob[ nWk2 ];
                        ucWk <<= 4;
                        ucWk >>= 4;
                        ucWk = HexTable[ ucWk ];
                        szAttribute[ nWk1++ ] = ucWk;
                     }

                     szAttribute[ nWk1++ ] = '\'';
                     szAttribute[ nWk1++ ] = ',';
                     szAttribute[ nWk1++ ] = '(';
                     for ( nWk2 = 0; nWk2 < (zSHORT) uLth; nWk2++ )
                     {
                        ucWk = pchBlob[ nWk2 ];
                        if ( isprint( ucWk ) )
                           szAttribute[ nWk1++ ] = ucWk;
                        else
                           szAttribute[ nWk1++ ] = '.';
                     }

                     szAttribute[ nWk1++ ] = ')';
                     szAttribute[ nWk1   ] = 0;
                  }
               }
               else
               {
                  GetStringFromRecord( vTZZOHDGV, lpViewEntity, lpViewAttrib, szAttribute, 298 );
               // GetVariableFromAttribute( szAttribute, 0, zTYPE_STRING,
               //                           298, vTZZOHDGV, pchEntityName,
               //                           lpViewAttrib->szName, 0, 0 );
               }

               CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
               if ( lpViewAttrib->bHidden )
               {
                  szHiddenAttribute[ 0 ] = '*';
                  strcpy_s( szHiddenAttribute + 1, sizeof( szHiddenAttribute ) - 1, lpViewAttrib->szName );
               }
               else
                  strcpy_s( szHiddenAttribute, sizeof( szHiddenAttribute ), lpViewAttrib->szName );

               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szHiddenAttribute );

               // force the value to fit within the work object's length
               szAttribute[ 252 ] = 0;
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlValue, szAttribute );
            }
         }
         else
         {
            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
            while ( lpViewAttrib )
            {
               if ( lpViewAttrib->cType == 'S' && lpViewAttrib->bHidden == FALSE )
               {
                  break;
               }

               lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
            }

            if ( lpViewAttrib == 0 )
            {
               lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
               while ( lpViewAttrib )
               {
                  if ( lpViewAttrib->cType != 'B' && lpViewAttrib->bHidden == FALSE )
                  {
                     break;
                  }

                  lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
               }
            }

            CreateViewFromViewForTask( &vWork, vTZZOHDGV, vSubtask );
            lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vWork->hViewCsr );
            lpWkEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
            while ( (LPVIEWENTITY) SysGetPointerFromHandle( lpWkEntityCsr->hViewEntity ) != lpViewEntity )
            {
               lpWkEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWkEntityCsr->hNextHier );
            }

            SetCursorFirstEntity( vWork, lpViewEntity->szName, 0 );
            do
            {
               CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, lpViewEntity->szName );
               if ( lpViewAttrib )
               {
                  GetVariableFromAttribute( szAttribute, 0, zTYPE_STRING, 298,
                                            vWork, pchEntityName,
                                            lpViewAttrib->szName, 0, 0 );
               }
               else
                  szAttribute[ 0 ] = 0;

               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlValue, szAttribute );
               if ( lpWkEntityCsr->hEntityInstance == lpViewEntityCsr->hEntityInstance )
               {
                  SetSelectStateOfEntity( vTZOBOIOO, szlAttrib, 1 );
               }
            }  while ( SetCursorNextEntity( vWork, lpViewEntity->szName, 0 ) >= zCURSOR_SET );
            DropView( vWork );
         }
      }

      fnSwapTaskContext( vSubtask );
   }

   if ( bSetHidden )
   {
      lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewEntityCsr->hEntityInstance );
      lpEntityInstance->u.nInd.bHidden = TRUE;
   }

   if ( zstrcmp( cpcTag, szlOI_Hier ) == 0 )
   {
      zwTZOBRWAD_RefreshDescDisp( vSubtask );

      RefreshCtrl( vSubtask, "OI_Edit1" );
      RefreshCtrl( vSubtask, "OI_ListBox1" );
      RefreshCtrl( vSubtask, "OI_ListBox2" );
   }

   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=zwTZOBRWAD_ActivateOIDisplay
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwTZOBRWAD_ActivateOIDisplay
//
//   PURPOSE:   Build an object instance containing the information
//              for a hilighted named view.
//              In windows, the object instance is displayed with a
//              tree control, for OS/2 the old list box is used.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ActivateOIDisplay( zVIEW vSubtask )
{
   zVIEW  vBrowser;
   zVIEW  vInstance;
   zCHAR  szID[ 17 ];

   // First, get the handle to the browser object
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szID, sizeof( szID ), vBrowser, szlName, szlObjectInstanceID );
// TraceLineS( "ActivateOIDisplay ID: ", szID );
// DisplayObjectInstance( vBrowser, 0, 0 );
   if ( szID[ 0 ] && GetViewByName( &vInstance, szID, vSubtask, zLEVEL_TASK ) > 0 )
   {
      ActivateWindow( vInstance );
      return( 0 );
   }
   else
   {
   // SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow, szlTZOBRWAD, szlVIEWOI );
      SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow, szlTZOBRWAD, szlOIDisplay );
   }

   zwTZOBRWAD_BuildGraphForView( vSubtask );
   return( 0 );
}

//./ ADD NAME=zwTZOBRWAD_BuildGraphForView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwTZOBRWAD_BuildGraphForView
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildGraphForView( zVIEW vSubtask )
{
   zVIEW        vBrowser;
   zVIEW        vTZZOHDGV;
   zVIEW        vTZZOHDGO;
   LPVIEWENTITY lpViewEntity;
   zSHORT       nLastLevel;
   LPVIEWOD     lpViewOD;

   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZZOHDGO );

   if ( CheckExistenceOfEntity( vBrowser, szlName ) < zCURSOR_SET )
      return( 0 );

   GetIntegerFromAttribute( (zPLONG) &vTZZOHDGV, vBrowser, szlName, szlID );
   vTZZOHDGV = (zVIEW) SysGetPointerFromHandle( vTZZOHDGV );
   if ( vTZZOHDGV->hViewCsr == 0 )
   {
      OperatorPrompt( vSubtask, "Object Browser Build Graph",
                      "Object instance has no data!", 0, 0, 0, zICON_STOP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   ActivateEmptyObjectInstance( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zSINGLE );
   SetNameForView( vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_TASK );
   CreateEntity( vTZZOHDGO, szlHD_Diagram, zPOS_AFTER );
   SetAttributeFromInteger( vTZZOHDGO, szlHD_Diagram, szlZKey, (zLONG) vTZZOHDGV );

   // Ensure task sharing is done by a setname and getname call
// SetNameForView( vTZZOHDGV, "__TZOBRWAD_SHARE", 0, zLEVEL_SYSTEM );
// GetViewByName( &vTZZOHDGV, "__TZOBRWAD_SHARE", vSubtask, zLEVEL_SYSTEM );
// DropNameForView( vTZZOHDGV, "__TZOBRWAD_SHARE", 0, zLEVEL_SYSTEM );

   SetNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
// GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );  // debugging only
   lpViewOD     = (LPVIEWOD) SysGetPointerFromHandle( vTZZOHDGV->hViewOD );
   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
   nLastLevel = lpViewEntity->nLevel;
   while ( lpViewEntity )
   {
      if ( lpViewEntity->nLevel > nLastLevel )
      {
         SetViewToSubobject( vTZZOHDGO, szlHD_ChildNode );
         nLastLevel++;
      }
      else
      {
         while ( nLastLevel > lpViewEntity->nLevel )
         {
            ResetViewFromSubobject( vTZZOHDGO );
            nLastLevel--;
         }
      }

      CreateEntity( vTZZOHDGO, szlHD_Node, zPOS_AFTER );
      SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlDIL, lpViewEntity->szName );
      SetAttributeFromInteger( vTZZOHDGO, szlHD_Node, szlZKey, (zLONG) lpViewEntity->nHierNbr );
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
   }

// TraceLineS( "DisplayObjectInstance TZZOHDGO ", "========================" );
// DisplayObjectInstance( vTZZOHDGO, 0, 0 );
// GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );  // debugging only
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_ZoomIn
//
// PURPOSE:   Zoom In on the hierarchical diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ZoomIn( zVIEW vSubtask )
{
   HD_ZoomDiagram( vSubtask, szlOI_Hier, 25, 2 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_ZoomOut
//
// PURPOSE:   Zoom Out on the hierarchical diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ZoomOut( zVIEW vSubtask )
{
   HD_ZoomDiagram( vSubtask, szlOI_Hier, -25, 2 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_ZoomFit
//
// PURPOSE:   Zoom to fit on the hierarchical diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ZoomFit( zVIEW vSubtask )
{
   HD_ZoomDiagram( vSubtask, szlOI_Hier, 0, 3 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_Zoom100
//
// PURPOSE:   Zoom to fit on the hierarchical diagram
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_Zoom100( zVIEW vSubtask )
{
   HD_ZoomDiagram( vSubtask, szlOI_Hier, 100, 1 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityMenu( zVIEW vSubtask )
{
   zPOINT  pt;

   HD_GetPointerCoordinates( vSubtask, szlOI_Hier, &pt.x, &pt.y );
   CreateTrackingPopupMenu( vSubtask, "EntityMenu", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ClientMenu( zVIEW vSubtask )
{
   zPOINT  pt;

   HD_GetPointerCoordinates( vSubtask, szlOI_Hier, &pt.x, &pt.y );
   CreateTrackingPopupMenu( vSubtask, "ClientMenu", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
}

#define  zPOS_NEXTOI  ((zSHORT) 100)
#define  zPOS_PREVOI  ((zSHORT) 101)

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorChange( zVIEW vSubtask, zSHORT nPos, zCPCHAR cpcTag )
{
   LPVIEWENTITY    lpViewEntity;
   LPVIEWENTITYCSR lpViewEntityCsr;
   LPVIEWCSR       lpViewCsr;
   zVIEW           vTZZOHDGO;
   zVIEW           vTZZOHDGV;
   zPCHAR          pchEntityName;
   zSHORT          nRC;

   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   GetAddrForAttribute( &pchEntityName, vTZZOHDGO, szlHD_Node, szlDIL );

   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   lpViewCsr       = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
   while ( lpViewEntityCsr )
   {
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
      if ( zstrcmp( lpViewEntity->szName, pchEntityName ) == 0 )
         break;

      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
   }

   if ( lpViewEntityCsr == 0 )
      return( 0 );

   switch ( nPos )
   {
      case zPOS_CURR:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL;
         break;

      case zPOS_FIRST:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL &&
               SetCursorFirstEntity( vTZZOHDGV, pchEntityName, 0 ) >= zCURSOR_SET;
         break;

      case zPOS_LAST:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL &&
               SetCursorLastEntity( vTZZOHDGV, pchEntityName, 0 ) >= zCURSOR_SET;
         break;

      case zPOS_PREV:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL &&
               SetCursorPrevEntity( vTZZOHDGV, pchEntityName, 0 ) >= zCURSOR_SET;
         break;

      case zPOS_NEXT:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL &&
               SetCursorNextEntity( vTZZOHDGV, pchEntityName, 0 ) >= zCURSOR_SET;
         break;

      case zPOS_PREVOI:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL &&
               SetEntityCursor( vTZZOHDGV, pchEntityName, 0, zPOS_PREV | zQUAL_SCOPE_OI,
                                0, 0, 0, 0, 0, 0 ) >= zCURSOR_SET;
         break;

      case zPOS_NEXTOI:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) > zCURSOR_NULL &&
               SetEntityCursor( vTZZOHDGV, pchEntityName, 0, zPOS_NEXT | zQUAL_SCOPE_OI,
                                0, 0, 0, 0, 0, 0 ) >= zCURSOR_SET;
         break;

      case zENTITY_CREATE:
         nRC = CreateEntity( vTZZOHDGV, pchEntityName, zPOS_AFTER ) >= 0;
         break;

      case zENTITY_CREATETEMP:
         nRC = CreateTemporalEntity( vTZZOHDGV, pchEntityName,
                                     zPOS_AFTER ) >= 0;
         break;

      case zENTITY_DELETE:
         nRC = CheckExistenceOfEntity( vTZZOHDGV,
                                       pchEntityName ) == zCURSOR_SET &&
               DeleteEntity( vTZZOHDGV, pchEntityName,
                             zREPOS_AFTER ) >= zCURSOR_NULL;
         break;

      case zENTITY_DELETEALL:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
               DeleteEntity( vTZZOHDGV, pchEntityName, zREPOS_AFTER ) >= zCURSOR_NULL;

         if ( nRC )
         {
            while ( SetCursorFirstEntity( vTZZOHDGV, pchEntityName, 0 ) >= zCURSOR_SET &&
                    DeleteEntity( vTZZOHDGV, pchEntityName, zREPOS_NONE ) >= zCURSOR_UNDEFINED )
            {
               ; // Nothing needs to be done here.
            }
         }

         break;

      case zENTITY_DROP:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
               DropEntity( vTZZOHDGV, pchEntityName, zREPOS_AFTER ) >= zCURSOR_NULL;
         break;

      case zENTITY_EXCLUDE:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
               ExcludeEntity( vTZZOHDGV, pchEntityName, zREPOS_AFTER ) >= zCURSOR_NULL;
         break;

      case zENTITY_EXCLUDEALL:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
               ExcludeEntity( vTZZOHDGV, pchEntityName, zREPOS_AFTER ) >= zCURSOR_NULL;

         if ( nRC )
         {
            while ( SetCursorFirstEntity( vTZZOHDGV, pchEntityName, 0 ) >= zCURSOR_SET &&
                    ExcludeEntity( vTZZOHDGV, pchEntityName,  zREPOS_NONE ) >= zCURSOR_UNDEFINED )
            {
               ; // Nothing needs to be done here.
            }
         }

         break;

      case zENTITY_TEMPORAL:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
               CreateTemporalSubobjectVersion( vTZZOHDGV, pchEntityName ) > zCALL_ERROR;
         break;

      case zENTITY_ACCEPT:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
               AcceptSubobject( vTZZOHDGV, pchEntityName ) > zCALL_ERROR;
         break;

      case zENTITY_CANCEL:
         nRC = CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
               CancelSubobject( vTZZOHDGV, pchEntityName ) > zCALL_ERROR;
         break;

      case zENTITY_SETVIEW:
         nRC = SetViewToSubobject( vTZZOHDGV, pchEntityName ) > zCALL_ERROR;
         break;
   }

   if ( nRC >= 0 )
   {
      if ( zstrcmp( cpcTag, szlOI_Hier ) == 0 )
      {
         zwTZOBRWAD_BuildTextForGraph( vSubtask );
         RefreshCtrl( vSubtask, cpcTag );
         zwTZOBRWAD_BuildAttrsForGraph( vSubtask );
      }
      else
      {
         zwTZOBRWAD_BuildTextForGraph( vSubtask, cpcTag );
         RefreshCtrl( vSubtask, cpcTag );
      }
   }

   SelectNode( vSubtask, cpcTag );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorNext( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zPOS_NEXT );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorPrev( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zPOS_PREV );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorNextOI( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zPOS_NEXTOI );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorPrevOI( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zPOS_PREVOI );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorFirst( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zPOS_FIRST );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CursorLast( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zPOS_LAST );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_AcceptAllSubobjects( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;

   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   AcceptAllTemporalSubobjects( vTZZOHDGV );
   zwTZOBRWAD_BuildTextForGraph( vSubtask );
   zwTZOBRWAD_BuildAttrsForGraph( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CancelAllSubobjects( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;

   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   CancelAllTemporalSubobjects( vTZZOHDGV );
   zwTZOBRWAD_BuildTextForGraph( vSubtask );
   zwTZOBRWAD_BuildAttrsForGraph( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityCreate( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_CREATE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityCreateTemporal( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_CREATETEMP );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityDelete( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_DELETE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityDeleteAll( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_DELETEALL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityDrop( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_DROP );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityExclude( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_EXCLUDE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityExcludeAll( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_EXCLUDEALL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SubobjectTemporal( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_TEMPORAL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SubobjectAccept( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_ACCEPT );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SubobjectCancel( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_CANCEL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SubobjectSetView( zVIEW vSubtask )
{
   zwTZOBRWAD_CursorChange( vSubtask, zENTITY_SETVIEW );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SubobjectResetView( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;
   zVIEW vTZZOHDGV;

   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   if ( ResetViewFromSubobject( vTZZOHDGV ) > zCALL_ERROR )
   {
      zwTZOBRWAD_BuildTextForGraph( vSubtask );
      RefreshCtrl( vSubtask, szlOI_Hier );
      zwTZOBRWAD_BuildAttrsForGraph( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ExpandNode( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;

   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      if ( CompareAttributeToString( vTZZOHDGO, szlHD_Node, szlHidden, "Y" ) == 0 )
      {
         SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlHidden, "N" );
         SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlCollapsed, "Y" );
      }
      else
      {
         SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlCollapsed, "N" );
         SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlHidden, "N" );
         if ( SetCursorFirstEntity( vTZZOHDGO, szlHD_ChildNode, 0 ) >= zCURSOR_SET )
         {
            do
            {
               SetAttributeFromString( vTZZOHDGO, szlHD_ChildNode, szlCollapsed, "Y" );
               SetAttributeFromString( vTZZOHDGO, szlHD_ChildNode, szlHidden, "N" );
            } while ( SetCursorNextEntity( vTZZOHDGO, szlHD_ChildNode, 0 ) >= zCURSOR_SET );
         }
      }
   }

   SelectNode( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_ExpandAllNodes
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
void
zfnTZOBRWAD_RecursExpandNodes( zVIEW vTZZOHDGO )
{
   SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlCollapsed, "N" );
   SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlHidden, "N" );
   if ( SetCursorFirstEntity( vTZZOHDGO, szlHD_ChildNode, 0 ) >= zCURSOR_SET )
   {
      do
      {
         SetViewToSubobject( vTZZOHDGO, szlHD_ChildNode );
         zfnTZOBRWAD_RecursExpandNodes( vTZZOHDGO );
         ResetViewFromSubobject( vTZZOHDGO );
      } while ( SetCursorNextEntity( vTZZOHDGO, szlHD_ChildNode, 0 ) >= zCURSOR_SET );
   }
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ExpandAllNodes( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;
   zVIEW vWork;

   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      CreateViewFromViewForTask( &vWork, vTZZOHDGO, vSubtask );
      ResetView( vWork );
      zfnTZOBRWAD_RecursExpandNodes( vWork );
      DropView( vWork );
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_CollapseNode
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CollapseNode( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;

   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlCollapsed, "Y" );
      SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlHidden, "N" );
   }

   SelectNode( vSubtask );
   RefreshCtrl( vSubtask, szlOI_Hier );
   CenterNode( vSubtask, szlOI_Hier );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_HideNode
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_HideNode( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;

   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlHidden, "Y" );
      SetAttributeFromString( vTZZOHDGO, szlHD_Node, szlCollapsed, "N" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: SelectNode
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
SelectNode( zVIEW vSubtask, zCPCHAR cpcTag )
{
   HD_SelectNode( vSubtask, cpcTag );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: CenterNode
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
CenterNode( zVIEW vSubtask, zCPCHAR cpcTag )
{
   HD_CenterNode( vSubtask, cpcTag );
   SelectNode( vSubtask, cpcTag );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_CenterNode
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CenterNode( zVIEW vSubtask )
{
   CenterNode( vSubtask, szlOI_Hier );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntitySelect( zVIEW vSubtask )
{
   zVIEW           vTZZOHDGO;
   zVIEW           vTZZOHDGV;
   zVIEW           vTZOBOIOO;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWCSR       lpViewCsr;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zPCHAR          pchEntityName;

   SelectNode( vSubtask );
   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   GetAddrForAttribute( &pchEntityName, vTZZOHDGO, szlHD_Node, szlDIL );

   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   lpViewCsr       = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
   while ( lpViewEntityCsr )
   {
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
      if ( zstrcmp( lpViewEntity->szName, pchEntityName ) == 0 )
         break;

      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
   }

   if ( lpViewEntityCsr == 0 )
      return( 0 );

   if ( CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
        SetSelectStateOfEntity( vTZZOHDGV, pchEntityName, 1 ) >= 0 )
   {
      if ( CompareAttributeToString( vTZOBOIOO, szlName, szlMode, "M" ) == 0 )
         zwTZOBRWAD_BuildAttrsForGraph( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityDeselect( zVIEW vSubtask )
{
   zVIEW           vTZZOHDGO;
   zVIEW           vTZZOHDGV;
   zVIEW           vTZOBOIOO;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWCSR       lpViewCsr;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zPCHAR          pchEntityName;

   SelectNode( vSubtask );
   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   GetAddrForAttribute( &pchEntityName, vTZZOHDGO, szlHD_Node, szlDIL );

   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   lpViewCsr       = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
   while ( lpViewEntityCsr )
   {
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
      if ( zstrcmp( lpViewEntity->szName, pchEntityName ) == 0 )
         break;

      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
   }

   if ( lpViewEntityCsr == 0 )
      return( 0 );

   if ( CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET &&
        SetSelectStateOfEntity( vTZZOHDGV, pchEntityName, 0 ) >= 0 )
   {
      if ( CompareAttributeToString( vTZOBOIOO, szlName, szlMode, "M" ) == 0 )
         zwTZOBRWAD_BuildAttrsForGraph( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntitySelectAll( zVIEW vSubtask )
{
   zVIEW           vTZZOHDGO;
   zVIEW           vTZZOHDGV;
   zVIEW           vTZOBOIOO;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWCSR       lpViewCsr;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zPCHAR          pchEntityName;

   SelectNode( vSubtask );
   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   GetAddrForAttribute( &pchEntityName, vTZZOHDGO, szlHD_Node, szlDIL );

   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   lpViewCsr       = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
   while ( lpViewEntityCsr )
   {
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
      if ( zstrcmp( lpViewEntity->szName, pchEntityName ) == 0 )
         break;

      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
   }

   if ( lpViewEntityCsr == 0 )
      return( 0 );

   SetAllSelectStatesForEntity( vTZZOHDGV, pchEntityName, 1, 0 );
   if ( CompareAttributeToString( vTZOBOIOO, szlName, szlMode, "M" ) == 0 )
      zwTZOBRWAD_BuildAttrsForGraph( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_EntityDeselectAll( zVIEW vSubtask )
{
   zVIEW           vTZZOHDGO;
   zVIEW           vTZZOHDGV;
   zVIEW           vTZOBOIOO;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWCSR       lpViewCsr;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zPCHAR          pchEntityName;

   SelectNode( vSubtask );
   GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   GetAddrForAttribute( &pchEntityName, vTZZOHDGO, szlHD_Node, szlDIL );

   if ( vTZZOHDGV == 0 )
   {
      MessageSend( vSubtask, "CM00108", "Object Browser",
                   "The view you are browsing has been dropped.",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

      return( 0 );
   }

   lpViewCsr       = (LPVIEWCSR) SysGetPointerFromHandle( vTZZOHDGV->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
   while ( lpViewEntityCsr )
   {
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
      if ( zstrcmp( lpViewEntity->szName, pchEntityName ) == 0 )
         break;

      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
   }

   if ( lpViewEntityCsr == 0 )
      return( 0 );

   SetAllSelectStatesForEntity( vTZZOHDGV, pchEntityName, 0, 0 );
   if ( CompareAttributeToString( vTZOBOIOO, szlName, szlMode, "M" ) == 0 )
      zwTZOBRWAD_BuildAttrsForGraph( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_DisplayLB_Click
//
// PURPOSE:   This operation get control whenever the user single
//            clicks on OI_ListBox1 in the display OI window, based on the
//            mode of display 1 of 3 things happens.
//
//             1 - Attribute display - Set the attribute in the update
//                            field so the attribute can be updated.
//
//             2 - Twin display - position the object cursor on the
//                            selected twin
//
//             3 - Entity view display - move the entity selected into
//                              the center of the hierarchical
//                              diagram.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_DisplayLB_Click( zVIEW vSubtask )
{
   zVIEW  vTZZOHDGO;
   zVIEW  vTZZOHDGV;
   zVIEW  vTZOBOIOO;
   zPCHAR pchEntityName;
   zCHAR  szAttribute[ 300 ];
   zCHAR  szMode[ 2 ];
   zULONG nRelNbr;

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
      if ( vTZZOHDGV == 0 )
      {
         MessageSend( vSubtask, "CM00108", "Object Browser",
                      "The view you are browsing has been dropped.",
                      zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

         return( 0 );
      }

      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      if ( szMode[ 0 ] == 'A' )
      {
         SetAttributeFromAttribute( vTZOBOIOO, szlEntity, szlUpdateAttr,
                                    vTZOBOIOO, szlAttrib, szlValue );
         RefreshCtrl( vSubtask, "OI_Edit1" );
      }
      else
      if ( szMode[ 0 ] == 'E' )
      {
         GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
         ResetView( vTZZOHDGO );
         SetCursorFirstSelectedEntity( vTZOBOIOO, szlAttrib, 0 );
         GetAddrForAttribute( &pchEntityName, vTZOBOIOO, szlAttrib, szlValue );
         if ( CompareAttributeToString( vTZZOHDGO, szlHD_Node, szlDIL, pchEntityName ) != 0 )
         {
            SetEntityCursor( vTZZOHDGO, szlHD_ChildNode, szlDIL,
                             zPOS_FIRST | zQUAL_STRING |
                             zQUAL_EQUAL | zRECURS, pchEntityName,
                             0, 0, 0, zSCOPE_OI, 0 );
            SetViewToSubobject( vTZZOHDGO, szlHD_ChildNode );
         }

         zwTZOBRWAD_ExpandAllNodes( vSubtask );
         RefreshCtrl( vSubtask, szlOI_Hier );
         if ( HD_IsNodeVisible( vSubtask, szlOI_Hier ) == 0 )
            CenterNode( vSubtask, szlOI_Hier );

         SelectNode( vSubtask );
      }
      else
      if ( szMode[ 0 ] == 'T' )
      {
         GetAddrForAttribute( &pchEntityName, vTZOBOIOO,
                              szlAttrib, szlName );
         nRelNbr = GetRelativeEntityNumber( vTZOBOIOO, szlAttrib, 0, 0L );
         SetCursorFirstEntity( vTZZOHDGV, pchEntityName, 0 );
         if ( nRelNbr )
            SetCursorRelativeEntity( vTZZOHDGV, pchEntityName, nRelNbr, 0 );

         zwTZOBRWAD_BuildTextForGraph( vSubtask );
         RefreshCtrl( vSubtask, szlOI_Hier );
         SelectNode( vSubtask );
      }
   }

   GetStringFromAttribute( szAttribute, sizeof( szAttribute ), vTZOBOIOO, "Attrib", "Name" );
   zwTZOBRWAD_RefreshDescDisp( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ChangeAttribute( zVIEW vSubtask )
{
   zVIEW    vTZZOHDGO;
   zVIEW    vTZZOHDGV;
   zVIEW    vTZOBOIOO;
   zPCHAR   pchEntityName;
   zCHAR    szAttribName[ 34 ];
   zCHAR    szMode[ 2 ];
   zCHAR    szLockEntity[ 34 ];
   LPVIEWENTITY  lpViewEntity;
   LPVIEWATTRIB  lpViewAttrib;
   LPVIEWOD      lpViewOD;

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 &&
        GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK ) > 0 &&
        GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      if ( szMode[ 0 ] == 'A' &&
           SetCursorFirstSelectedEntity( vTZOBOIOO, szlAttrib, 0 ) >= zCURSOR_SET )
      {
         GetAddrForAttribute( &pchEntityName, vTZZOHDGO, szlHD_Node, szlDIL );
         GetCtrlText( vSubtask, "OI_LockEntity", szLockEntity, 33 );
         if ( szLockEntity[ 0 ] )
            pchEntityName = szLockEntity;

         GetStringFromAttribute( szAttribName, sizeof( szAttribName ), vTZOBOIOO, szlAttrib, szlName );
         // Now that we have the entity and the attribute, make sure the attribute is not a blob
         lpViewOD     = (LPVIEWOD) SysGetPointerFromHandle( vTZZOHDGV->hViewOD );
         lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
         while ( zstrcmp( lpViewEntity->szName, pchEntityName ) )
            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );

         lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
         while ( zstrcmp( lpViewAttrib->szName, szAttribName ) )
            lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );

         if ( lpViewAttrib->cType != 'B' && CheckExistenceOfEntity( vTZZOHDGV, pchEntityName ) == zCURSOR_SET )
         {
            if ( SetAttributeFromAttribute( vTZZOHDGV, pchEntityName, szAttribName, vTZOBOIOO, szlEntity, szlUpdateAttr ) >= 0 )
            {
               if ( CompareAttributeToAttribute( vTZZOHDGO, szlHD_Node, szlText, vTZOBOIOO, szlAttrib, szlValue ) == 0 )
               {
                  zwTZOBRWAD_BuildTextForGraph( vSubtask );
                  RefreshCtrl( vSubtask, szlOI_Hier );
               }

               zwTZOBRWAD_BuildAttrsForGraph( vSubtask );
               GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
               if ( SetCursorFirstEntityByString( vTZOBOIOO, szlAttrib, szlName, szAttribName, 0 ) >= zCURSOR_SET )
               {
                  SetSelectStateOfEntity( vTZOBOIOO, szlAttrib, 1 );
                  RefreshCtrl( vSubtask, "OI_ListBox1" );
                  RefreshCtrl( vSubtask, "OI_ListBox2" );
                  zwTZOBRWAD_DisplayLB_Click( vSubtask );
               }
            }
         }
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_NameOI_AtTask( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;

   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CommitFilePostBuild( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;

   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
   DropNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
   SetNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );

   SetCtrlState( vSubtask, "Incremental", zCONTROL_STATUS_CHECKED, 1 );
// SetCtrlState( vSubtask, "ASCII", zCONTROL_STATUS_CHECKED, 1 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_NameViewPostBuild( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;

   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
   DropNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK );
   SetNameForView( vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CommitToFile( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;
   zCHAR pchFileName[ zMAX_FILENAME_LTH + 1 ];
   zLONG lControl;
   zCHAR szMsg[ zMAX_MESSAGE_LTH + 1 ];

   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   lControl = 0;

   if ( GetCtrlState( vSubtask, "Incremental", zCONTROL_STATUS_CHECKED ) )
      lControl |= zINCREMENTAL;

   GetCtrlText( vSubtask, "FileName", pchFileName,  zMAX_FILENAME_LTH );
   if ( pchFileName[ 0 ] &&
        CommitOI_ToFile( vTZZOHDGV, pchFileName, lControl ) >= 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "ObjectInstance successfully written to File: " );
      strcat_s( szMsg, sizeof( szMsg ), pchFileName );

      MessageSend( vSubtask, "CM00107", "Object Browser", szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      return( 0 );
   }

   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_NameView( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;
   zCHAR szViewName[ 130 ];
   zSHORT nLevel;

   GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   nLevel = zLEVEL_TASK;
   if ( GetCtrlState( vSubtask, "Application", zCONTROL_STATUS_CHECKED ) )
      nLevel = zLEVEL_APPLICATION;
   else
   if ( GetCtrlState( vSubtask, "System", zCONTROL_STATUS_CHECKED ) )
      nLevel = zLEVEL_SYSTEM;

   GetCtrlText( vSubtask, "ViewName", szViewName, 128 );

   fnSwapTaskContext( vSubtask );

   if ( szViewName[ 0 ] &&
        SetNameForView( vTZZOHDGV, szViewName, vTZZOHDGV, nLevel ) == 0 )
   {
      fnSwapTaskContext( vSubtask );
      return( 0 );
   }

   fnSwapTaskContext( vSubtask );
   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_DropView( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV,
                       vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      if ( OperatorPrompt( vSubtask, "Confirm Drop View",
                           "OK to drop View?",
                           0, zBUTTONS_YESNO,
                           zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_YES )
      {
         DropView( vTZZOHDGV );
         return( 0 );
      }
   }

   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_DropInstance( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV,
                       vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      if ( OperatorPrompt( vSubtask, "Confirm Drop Instance",
                           "OK to drop the Object Instance?",
                           0, zBUTTONS_YESNO,
                           zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_YES )
      {
         DropObjectInstance( vTZZOHDGV );
         return( 0 );
      }
   }

   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_OIDisplayExit( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;
   zVIEW vTZZOHDGV;
   zVIEW vTZZOHDGV_S;
   zVIEW vTZZOLODO_Desc;
   zSHORT nZoom;

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK ) > 0 &&
        GetViewByName( &vTZZOHDGV_S, szlTZZOHDGV_S, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      if ( OperatorPrompt( vSubtask, "View Update",
                           "Do you wish to retain any cursor modifications\n"
                           "or entity selections made to the view?",
                           0, zBUTTONS_YESNO,
                           zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_NO )
      {
         // Reset any changes made to the view back to the
         // original state.
         SetViewFromView( vTZZOHDGV, vTZZOHDGV_S );
      }

      DropView( vTZZOHDGV_S );
   }

   if ( GetViewByName( &vTZZOLODO_Desc, "TZZOLODO_Desc", vSubtask, zLEVEL_TASK ) > 0 )
     DropView( vTZZOLODO_Desc );

   nZoom = HD_ZoomDiagram( vSubtask, szlOI_Hier, 0, 0 );
   SetWindowPreferenceInteger( vSubtask, "Zoom", nZoom );

   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      DropObjectInstance( vTZZOHDGO );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_OIDisplayExitCancel( zVIEW vSubtask )
{
   zVIEW vTZZOHDGO;
   zVIEW vTZZOHDGV;
   zVIEW vTZZOHDGV_S;
   zVIEW vTZZOLODO_Desc;
   zSHORT nZoom;

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK ) > 0 &&
        GetViewByName( &vTZZOHDGV_S, szlTZZOHDGV_S, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      // Reset any changes made to the view back to the
      // original state.
      SetViewFromView( vTZZOHDGV, vTZZOHDGV_S );
      DropView( vTZZOHDGV_S );
   }

   if ( GetViewByName( &vTZZOLODO_Desc, "TZZOLODO_Desc", vSubtask, zLEVEL_TASK ) > 0 )
     DropView( vTZZOLODO_Desc );

   nZoom = HD_ZoomDiagram( vSubtask, szlOI_Hier, 0, 0 );
   SetWindowPreferenceInteger( vSubtask, "Zoom", nZoom );

   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      DropObjectInstance( vTZZOHDGO );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CreateNewView( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;
   zVIEW vTZZOHDGV_S;
   zVIEW vTZZOHDGV_NEW;

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      fnSwapTaskContext( vSubtask );
      CreateViewFromViewForTask( &vTZZOHDGV_NEW, vTZZOHDGV, vSubtask );
      fnSwapTaskContext( vSubtask );
      SetNameForView( vTZZOHDGV_NEW, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
      if ( GetViewByName( &vTZZOHDGV_S, szlTZZOHDGV_S, vSubtask, zLEVEL_SUBTASK ) > 0 )
      {
         if ( OperatorPrompt( vSubtask, "View Update",
                              "Do you wish to retain any cursor modifications\n"
                              "or entity selections made to the view?",
                              0, zBUTTONS_YESNO,
                              zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_NO )
         {
            // Reset any changes made to the view back to the
            // original state.
            SetViewFromView( vTZZOHDGV, vTZZOHDGV_S );
         }
      }

      DropView( vTZZOHDGV_S );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ActivateObject( zVIEW vSubtask )
{
   zVIEW  vBrowser;
// LPTASK hTask;
   LPTASK lpTask;
   zVIEW  lpView;
   zVIEW  vActivate;
   zVIEW  vAppSubtask;
   zVIEW  vKZAPPLOO;
   zVIEW  vOldView;
   zLONG  lOptions;
   zCHAR  szObject[ 10 ];
   zCHAR  szName[ 22 ];
   zCHAR  pchFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szAppName[ 40 ];
   zSHORT nLevel;
   zSHORT nRC = -2;

   GetViewByName( &vKZAPPLOO, szlKZAPPLOO, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szAppName, sizeof( szAppName ), vKZAPPLOO,
                           "APPLICATION", "APP_NAME" );
   GetCtrlText( vSubtask, "Object", szObject, 9 );
   GetCtrlText( vSubtask, "Name", szName, 20 );
   GetCtrlText( vSubtask, "FileName", pchFileName, zMAX_FILENAME_LTH );
   lOptions = zIGNORE_ERRORS;

   if ( GetCtrlState( vSubtask, "Multiple", zCONTROL_STATUS_CHECKED ) )
      lOptions |= zMULTIPLE;
   else
      lOptions |= zSINGLE;

   if ( szObject[ 0 ] )
   {
      if ( szName[ 0 ] == 0 )
         strcpy_s( szName, szObject );

      GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   // DisplayObjectInstance( vBrowser, 0, 0 );

      GetIntegerFromAttribute( (zPLONG) &lpTask, vBrowser, szlTask, szlPointer );
   // lpTask = (LPTASK) hTask; // SysGetPointerFromHandle( hTask );
      lpView = (zVIEW) SysGetPointerFromHandle( lpTask->hFirstView );

      fnSwapTaskContext( vSubtask );
      SfCreateSubtask( &vAppSubtask, vSubtask, szAppName );
      if ( pchFileName[ 0 ] )
      {
         nRC = ActivateOI_FromFile( &vActivate, szObject, vAppSubtask, pchFileName, lOptions );
      }
      else
      {
         nRC = ActivateEmptyObjectInstance( &vActivate, szObject, vAppSubtask, lOptions );
      }

      TraceLine( "ActivateObject for Task: 0x%08x   Application: %s   RC: %d", vAppSubtask->hTask, szAppName, nRC );
      if ( nRC >= 0 )
      {
         // If the Activate was OK and the user set "DropViewCheckBox", then Drop old View with the given Name.
         if ( GetCtrlState( vSubtask, "DropViewCheckBox", zCONTROL_STATUS_CHECKED ) )
         {
            if ( GetViewByName( &vOldView, szName, lpView, zLEVEL_ANY ) >= zCURSOR_SET )
               DropView( vOldView );
            else
               TraceLineS("(tzobrwad) Old View to be dropped did not exist, Name: ", szName );
         }

         if ( GetCtrlState( vSubtask, "RadioBtnSystem", zCONTROL_STATUS_CHECKED ) )
            nLevel = zLEVEL_SYSTEM;

         if ( GetCtrlState( vSubtask, "RadioBtnApplication", zCONTROL_STATUS_CHECKED ) )
            nLevel = zLEVEL_APPLICATION;

         if ( GetCtrlState( vSubtask, "RadioBtnTask", zCONTROL_STATUS_CHECKED ) )
            nLevel = zLEVEL_TASK;

         if ( GetCtrlState( vSubtask, "RadioBtnSubtask", zCONTROL_STATUS_CHECKED ) )
            nLevel = zLEVEL_SUBTASK;

         nRC = SetNameForView( vActivate, szName, lpView, nLevel );

         if ( GetCtrlState( vSubtask, "CheckBoxDisplay", zCONTROL_STATUS_CHECKED ) )
            DisplayObjectInstance( vActivate, 0, 0 );
      }

      SfDropSubtask( vAppSubtask, 0 );
      fnSwapTaskContext( vSubtask );
   }
   else
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Object Name required",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );

   if ( nRC <= -1 )
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_GetNameForFile( zVIEW vSubtask )
{
   zCHAR szFileName[ 512 ];

   GetCtrlText( vSubtask, "FileName", szFileName, 512 );
   if ( szFileName[ 0 ] == 0 )
      strcpy_s( szFileName, "*.por" );

   OperatorPromptForFile( vSubtask, szFileName, 512,
                          "portable file (*.por) ||*.por||all files (*.*)||*.*||||", 0, TRUE );
   SetCtrlText(  vSubtask, "FileName", szFileName);

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ShowSelectSet( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;
   zSHORT nSet;
   zCHAR szMsg[ 100 ];

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV,
                       vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      nSet = SetSelectSetForView( vTZZOHDGV, 1 );
      SetSelectSetForView( vTZZOHDGV, nSet );
      strcpy_s( szMsg, "Select set is currently " );
      zltoa( nSet, szMsg + 90, sizeof( szMsg ) - 90 );
      strcat_s( szMsg, sizeof( szMsg ), szMsg + 90 );
      OperatorPrompt( vSubtask, "Select Set Display", szMsg, 0, 0, 0, zICON_INFORMATION );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_NextSelectSet( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;
   zSHORT nSet;
   zCHAR szMsg[ 100 ];

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      nSet = SetSelectSetForView( vTZZOHDGV, 1 );
      if ( nSet < 16 )
         nSet++;

      SetSelectSetForView( vTZZOHDGV, nSet );
      strcpy_s( szMsg, "Select set is now " );
      zltoa( nSet, szMsg + 90, sizeof( szMsg ) - 90 );
      strcat_s( szMsg, sizeof ( szMsg ), szMsg + 90 );
      OperatorPrompt( vSubtask, "Select Set Display", szMsg, 0, 0, 0, zICON_INFORMATION );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_PrevSelectSet( zVIEW vSubtask )
{
   zVIEW vTZZOHDGV;
   zSHORT nSet;
   zCHAR szMsg[ 100 ];

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      nSet = SetSelectSetForView( vTZZOHDGV, 1 );
      if ( nSet > 1 )
         nSet--;

      SetSelectSetForView( vTZZOHDGV, nSet );
      strcpy_s( szMsg, "Select set is now " );
      zltoa( nSet, szMsg + 90, sizeof( szMsg ) - 90 );
      strcat_s( szMsg, sizeof( szMsg ), szMsg + 90 );
      OperatorPrompt( vSubtask, "Select Set Display", szMsg, 0, 0, 0, zICON_INFORMATION );
   }

   return( 0 );
}

//./ ADD NAME=fnBuildOutlinerOI_ForView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: BuildOutlinerOI_ForView
//
//   PURPOSE:   Build an object instance containing the information
//              for a hilighted named view.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnBuildOutlinerOI_ForView( zVIEW vSubtask,
                           zBOOL bShare )
{
   zVIEW                vBrowser;
   zVIEW                vProfile;
   zVIEW                vObject;
   zVIEW                vWorkObject;
   zVIEW                vInstance;
   zVIEW                vTZOLOBWO;
   zPCHAR               pchShowHidden;
   zCHAR                szAttrib[ 260 ];
   zCHAR                szInd[ 5 ];
   zLONG                lEntities;
   zLONG                lAttributes;
   LPVIEWCSR            lpViewCsr;
   LPVIEWOI             lpViewOI = 0;
   LPVIEWENTITY         lpViewEntity;
   LPVIEWENTITYCSR      lpViewEntityCsr;
   LPVIEWENTITYCSR      lpRootViewEntityCsr;
   LPVIEWENTITYCSR      lpWorkViewEntityCsr;
   LPVIEWATTRIB         lpViewAttrib;
   LPENTITYINSTANCE     lpEntityInstance;
   LPENTITYINSTANCE     lpLinkedEntityInstance;
   LPSELECTEDINSTANCE   lpFirstSelectedInstance;
   LPSELECTEDINSTANCE   lpSelectedInstance;
   zSHORT               nRC;

   /* TZOBOIOO - Object browser listbox display object */
   if ( GetViewByName( &vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vInstance );

   /* TZOLOBWO - Object browser outliner display object */
   if ( GetViewByName( &vInstance, szlTZOLOBWO, vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vInstance );

   /* TZOBRWOO - Object browser work view */
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vBrowser, szlName ) < zCURSOR_SET )
      return( 0 );

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   GetAddrForAttribute( &pchShowHidden, vProfile, "OBR", "ShowHidden" );

   ActivateEmptyObjectInstance( &vInstance, szlTZOBOIOO, vSubtask, zSINGLE );
   SetNameForView( vInstance, szlTZOBOIOO, vSubtask, zLEVEL_TASK );
   CreateEntity( vInstance, szlName, zPOS_AFTER );
   SetMatchingAttributesByName( vInstance, szlName,
                                vBrowser, szlName, zSET_NULL );
   SetAttributeFromString( vInstance, szlName, szlMode, "A" );

   ActivateEmptyObjectInstance( &vTZOLOBWO, szlTZOLOBWO, vSubtask, zSINGLE );
   SetNameForView( vTZOLOBWO, szlTZOLOBWO, vSubtask, zLEVEL_TASK );
   CreateEntity( vTZOLOBWO, szlObject, zPOS_AFTER );
   SetMatchingAttributesByName( vTZOLOBWO, szlObject,
                                vBrowser, szlName, zSET_NULL );

   /* get the view to the object for which to present information */
   GetIntegerFromAttribute( (zPLONG) &vObject, vBrowser, szlName, szlID );
   vObject = (zVIEW) SysGetPointerFromHandle( vObject );
   SetNameForView( vObject, "__OL_OBRW__", 0, zLEVEL_SYSTEM );

   /* see if there is any instance data to display */
   if ( vObject->hViewCsr )
   {
      lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vObject->hViewCsr );
      lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
   }
   else
      lpViewCsr = 0;

   if ( lpViewCsr && lpViewOI->hRootEntityInstance )
   {
      zSHORT    nPrevLevel = 1;
      LPVIEWCSR lpWorkViewCsr;
      LPVIEWOI  lpWorkViewOI;

      // Go through the view hierarchically and call CheckExistenceOfEntity
      // for each visible cursor which is currently in an unset state to
      // force the cursor to be positioned.
      lpWorkViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
      while ( lpWorkViewEntityCsr )
      {
         if ( lpWorkViewEntityCsr->hEntityInstance == UNSET_CSR )
         {
            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpWorkViewEntityCsr->hViewEntity );
            CheckExistenceOfEntity( vObject, lpViewEntity->szName );
         }

         lpWorkViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWorkViewEntityCsr->hNextHier );
      }

      // Create a work view to the object which is to be presented.
      CreateViewFromViewForTask( &vWorkObject, vObject, vSubtask );

      // Get first selected instance for selection marking.
      lpRootViewEntityCsr     = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hRootViewEntityCsr );
      lpFirstSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpViewCsr->hFirstSelectedInstance );

      // Now go through each instance in the target object, plug the cursor
      // for the instance and retrieve the attributes for each entity.
      lpWorkViewCsr  = (LPVIEWCSR) SysGetPointerFromHandle( vWorkObject->hViewCsr );
      lpWorkViewOI   = (LPVIEWOI) SysGetPointerFromHandle( lpWorkViewCsr->hViewOI );
      lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpWorkViewOI->hRootEntityInstance );
      lEntities   = 0;
      lAttributes = 0;
      szInd[ 4 ]  = 0;  // terminate indicator flags string
      nRC = 0;
      while ( nRC == 0 && lpEntityInstance )
      {
         zSHORT nAttrSet;

         if ( lpEntityInstance->u.nInd.bHidden == FALSE ||
              pchShowHidden[ 0 ] == 'Y' )
         {
            zSHORT nIdx;
            zBOOL  bHidden;

            if ( lpEntityInstance->u.nInd.bHidden )
            {
               bHidden = TRUE;
               lpEntityInstance->u.nInd.bHidden = FALSE;
            }
            else
               bHidden = FALSE;

            if ( nPrevLevel > lpEntityInstance->nLevel )
            {
               while ( nPrevLevel > lpEntityInstance->nLevel )
               {
                  ResetViewFromSubobject( vTZOLOBWO );
                  nPrevLevel--;
               }
            }
            else
            if ( nPrevLevel < lpEntityInstance->nLevel )
            {
               while ( nPrevLevel < lpEntityInstance->nLevel )
               {
                  SetViewToSubobject( vTZOLOBWO, szlChildEntity );
                  nPrevLevel++;
               }
            }

            CreateEntity( vTZOLOBWO, szlEntity, zPOS_AFTER );

            // Retain the pointer to the corresponding entity from the
            // outliner display work object so we can get back to the
            // real object when an item in the outliner is selected.
            SetAttributeFromInteger( vTZOLOBWO, szlEntity, szlPointer,
                                     (zLONG) lpEntityInstance );
            lEntities++;
            szInd[ 0 ] = szInd[ 1 ] = szInd[ 2 ] = szInd[ 3 ] = '.';

            if ( lpEntityInstance->hNextLinked )
            {
               lpLinkedEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextLinked );
               while ( lpLinkedEntityInstance != lpEntityInstance &&
                       lpLinkedEntityInstance->u.nInd.bHidden )
               {
                  lpLinkedEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkedEntityInstance->hNextLinked );
               }

               if ( lpLinkedEntityInstance != lpEntityInstance )
                  szInd[ 0 ] = 'l';
            }

            if ( lpFirstSelectedInstance )
            {
               for ( lpSelectedInstance = lpFirstSelectedInstance;
                     lpSelectedInstance;
                     lpSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpSelectedInstance->hNextSelectedInstance ) )
               {
                  if ( (LPENTITYINSTANCE)
                       SysGetPointerFromHandle( lpSelectedInstance->xEntityInstance ) == lpEntityInstance )
                  {
                     szInd[ 1 ] = 's';
                     break;
                  }
               }
            }

            if ( lpEntityInstance->hPrevVsn )
               szInd[ 2 ] = 'v';
            else
            if ( lpEntityInstance->u.nInd.bTemporal )
               szInd[ 2 ] = 't';
            else
            if ( lpEntityInstance->u.nInd.bPrevVersion )
               szInd[ 2 ] = 'p';

            lpWorkViewEntityCsr = lpRootViewEntityCsr;
            while ( lpWorkViewEntityCsr )
            {
               if ( (LPENTITYINSTANCE)
                    SysGetPointerFromHandle( lpWorkViewEntityCsr->hEntityInstance ) == lpEntityInstance )
               {
                  szInd[ 3 ] = '>';
                  break;
               }

               lpWorkViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWorkViewEntityCsr->hNextHier );
            }

            SetAttributeFromString( vTZOLOBWO, szlEntity, szlInd, szInd );
            szAttrib[ 0 ] = 0;
            for ( nIdx = 0; nIdx < (lpEntityInstance->nLevel - 1); nIdx++ )
               szAttrib[ nIdx ] = ' ';

            szAttrib[ nIdx ] = 0;
            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );
            strcat_s( szAttrib, lpViewEntity->szName );
            SetAttributeFromString( vTZOLOBWO, szlEntity, szlName, lpViewEntity->szName );
            SetAttributeFromInteger( vTZOLOBWO, szlEntity, szlLevel, (zLONG) lpEntityInstance->nLevel );

            nAttrSet = 0;

            lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
            while ( nRC == 0 && lpViewAttrib )
            {
               if ( lpViewAttrib->bHidden == FALSE ||
                    pchShowHidden[ 0 ] == 'Y' )
               {
                  if ( (lpViewAttrib->szDerivedOper[ 0 ] == 0 || lShowDerived) &&
                       (nAttrSet == 0 ||
                        (nAttrSet == 1 && lpViewAttrib->cType == 'S')) &&
                       lpViewAttrib->cType != 'B' )
                  {
                     lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWorkViewCsr->hFirstOD_EntityCsr );
                     while ( lpViewEntityCsr->hViewEntity != lpEntityInstance->hViewEntity )
                     {
                        lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );
                     }

                     lpViewEntityCsr->hEntityInstance = (LPENTITYINSTANCE) SysGetHandleFromPointer( lpEntityInstance );

                     if ( lpViewAttrib->bHidden )
                        GetStringFromRecord( vWorkObject, lpViewEntity, lpViewAttrib, szAttrib, 256 );
                     else
                        GetVariableFromAttribute( szAttrib, 0, zTYPE_STRING,
                                                  256, vWorkObject, lpViewEntity->szName, lpViewAttrib->szName, 0, 0 );

                     SetAttributeFromString( vTZOLOBWO, szlEntity, szlFirstAttr, szAttrib );
                     if ( lpViewAttrib->cType == 'S' )
                        nAttrSet = 2;
                     else
                        nAttrSet = 1;
                  }
               }

               lpViewAttrib = (LPVIEWATTRIB)
                    SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
            }

            if ( bHidden )
               lpEntityInstance->u.nInd.bHidden = TRUE;

         } // if ( lpEntityInstance->u.nInd.bHidden == FALSE ||
           //      pchShowHidden[ 0 ] == 'Y' ) ...

         lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextHier );
      }

      ResetView( vTZOLOBWO );
      lAttributes = fnGetAttributesForEntityInstance( vSubtask, vTZOLOBWO, 0 );
      if ( CheckExistenceOfEntity( vTZOLOBWO, szlAttrib ) == zCURSOR_SET )
      {
         SetAttributeFromAttribute( vTZOLOBWO, szlEntity, szlUpdateAttr,
                                    vTZOLOBWO, szlAttrib, szlValue );
      }

      DropView( vWorkObject );
   }
   else
   {
      OperatorPrompt( vSubtask, "Object Browser",
                      "Object instance is empty!", 0, 0, 0, zICON_STOP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   DropView( vProfile );
   return( 0 );
}

//./ ADD NAME=BuildOutlinerOI_ForView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: BuildOutlinerOI_ForView
//
//   PURPOSE:   Build an object instance containing the information
//              for a hilighted named view.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
BuildOutlinerOI_ForView( zVIEW vSubtask )
{
   zVIEW  vBrowser;
   zVIEW  vInstance;
   zCHAR  szID[ 15 ];

   /* first, get the handle to the browser object */
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szID, sizeof( szID ), vBrowser, szlName, szlObjectInstanceID );
   if ( GetViewByName( &vInstance, szID, vSubtask, zLEVEL_TASK ) > 0 )
   {
      ActivateWindow( vInstance );
      return( 0 );
   }

   fnBuildOutlinerOI_ForView( vSubtask, 1 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildOutlinerAttrs( zVIEW vSubtask )
{
   zVIEW                vTZOLOBWO;
   zVIEW                vTZOBOIOO;
   zVIEW                vObject;
   zVIEW                vWork;
   zSHORT               nWk1, nWk2, nWk3;
   zUCHAR               ucWk;
   zLONG                lLinks;
   zPCHAR               pchEntityName;
   zCHAR                szLockEntity[ 34 ];
   zPCHAR               pchBlob;
   zCHAR                szMode[ 2 ];
   zCHAR                szAttribute[ 300 ];
   LPVIEWCSR            lpViewCsr;
   LPVIEWENTITY         lpViewEntity;
   LPVIEWENTITYCSR      lpViewEntityCsr;
   LPVIEWENTITYCSR      lpWkEntityCsr;
   LPVIEWATTRIB         lpViewAttrib;
   LPENTITYINSTANCE     lpEntityInstance;
   LPENTITYINSTANCE     lpLinkInstance;
   LPSELECTEDINSTANCE   lpSelectedInstance;

   /* reset the attribute list object */
   szMode[ 0 ] = 'A';
   szMode[ 1 ] = 0;
   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      DropObjectInstance( vTZOBOIOO );
   }

   ActivateEmptyObjectInstance( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zSINGLE );
   SetNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   CreateEntity( vTZOBOIOO, szlName, zPOS_AFTER );
   SetAttributeFromString( vTZOBOIOO, szlName, szlMode, szMode );
   CreateEntity( vTZOBOIOO, szlEntity, zPOS_AFTER );

   /* TZOLOBWO - Object browser outliner display object */
   GetViewByName( &vTZOLOBWO, szlTZOLOBWO, vSubtask, zLEVEL_TASK );

   /* this sets position within the view to the currently hilited entity
      and returns the view to the object being displayed by the outliner */
   OL_GetCurrentEntityName( vSubtask, "Outliner", &vTZOLOBWO, szLockEntity );
   TraceLineS( "Current outliner entity name = ", szLockEntity );
   pchEntityName = szLockEntity;

   /* retrieve the pointer to the real entity from the outliner display work object */
   GetIntegerFromAttribute( (zPLONG) &lpEntityInstance,
                            vTZOLOBWO, szlEntity, szlPointer );

   /* get the object being represented by the outliner */
   GetViewByName( &vObject, "__OL_OBRW__", vSubtask, zLEVEL_SYSTEM );

   if ( vObject == 0 || lpEntityInstance == 0 || pchEntityName[ 0 ] == 0 )
   {
      /* no entity associated with outliner item */
      RefreshCtrl( vSubtask, "Edit" );
      RefreshCtrl( vSubtask, "ListAttributes" );
      return( 0 );
   }

   if ( szMode[ 0 ] == 'A' )
   {
//    SetCtrlState( vSubtask, "OI_LockEntity", zCONTROL_STATUS_VISIBLE, 0 );
      SetCtrlState( vSubtask, "OI_UpdateButton", zCONTROL_STATUS_VISIBLE, 1 );
      SetCtrlState( vSubtask, "OI_Edit", zCONTROL_STATUS_VISIBLE, 1 );
      CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
      SetAttributeFromString( vTZOBOIOO, szlAttrib, "Name", "Name" );
      SetAttributeFromString( vTZOBOIOO, szlAttrib, "Value", "Value" );
      CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
      SetAttributeFromString( vTZOBOIOO, szlAttrib, "Name",
                              "------------------------------" );
      SetAttributeFromString( vTZOBOIOO, szlAttrib, "Value",
                              "------------------------------------------" );


      /* plug cursor to position on current entity */
      lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vObject->hViewCsr );
      lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hFirstOD_EntityCsr );
      while ( lpViewEntityCsr->hViewEntity != lpEntityInstance->hViewEntity )
         lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewEntityCsr->hNextHier );

      lpViewEntityCsr->hEntityInstance = (LPENTITYINSTANCE) SysGetHandleFromPointer( lpEntityInstance );

      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpEntityInstance->hViewEntity );
      lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
      while ( lpViewAttrib )
      {
         if ( lpViewAttrib->bHidden == FALSE )
         {
            if ( lpViewAttrib->cType == 'B' )
            {
               zULONG uLth;

               GetAttributeLength( &uLth, vObject, lpViewEntity->szName, lpViewAttrib->szName );
               if ( uLth > 60 || uLth == 0 )
               {
                  strcpy_s( szAttribute, sizeof( szAttribute ), "(Blob, lth=" );
                  zltoa( uLth, szAttribute + 50, sizeof( szAttribute ) - 50 );
                  strcat_s( szAttribute, sizeof( szAttribute ), szAttribute + 50 );
                  strcat_s( szAttribute, sizeof( szAttribute ), ")" );
               }
               else
               {
                  GetAddrForAttribute( &pchBlob, vObject, lpViewEntity->szName, lpViewAttrib->szName );
                  nWk1 = 2;
                  strcpy_s( szAttribute, sizeof( szAttribute ), "x'" );
                  for ( nWk2 = 0; nWk2 < (zSHORT) uLth; nWk2++ )
                  {
                     ucWk = pchBlob[ nWk2 ];
                     ucWk >>= 4;
                     ucWk = HexTable[ ucWk ];
                     szAttribute[ nWk1++ ] = ucWk;
                     ucWk = pchBlob[ nWk2 ];
                     ucWk <<= 4;
                     ucWk >>= 4;
                     ucWk = HexTable[ ucWk ];
                     szAttribute[ nWk1++ ] = ucWk;
                  }
                  szAttribute[ nWk1++ ] = '\'';
                  szAttribute[ nWk1++ ] = ',';
                  szAttribute[ nWk1++ ] = '(';
                  for ( nWk2 = 0; nWk2 < (zSHORT) uLth; nWk2++ )
                  {
                     ucWk = pchBlob[ nWk2 ];
                     if ( isprint( ucWk ) )
                        szAttribute[ nWk1++ ] = ucWk;
                     else
                        szAttribute[ nWk1++ ] = '.';
                  }
                  szAttribute[ nWk1++ ] = ')';
                  szAttribute[ nWk1 ] = 0;
               }
            }
            else
            {
               GetVariableFromAttribute( szAttribute, 0, zTYPE_STRING,
                                         298, vObject, lpViewEntity->szName,
                                         lpViewAttrib->szName, 0, 0 );
            }

            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, lpViewAttrib->szName );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlValue, szAttribute );
         }

         lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
      }

      RefreshCtrl( vSubtask, "Edit" );
      RefreshCtrl( vSubtask, "ListAttributes" );

      return( 0 );
   }
   else
   {
      SetCtrlState( vSubtask, "OI_UpdateButton", zCONTROL_STATUS_VISIBLE, 0 );
      SetCtrlState( vSubtask, "OI_Edit", zCONTROL_STATUS_VISIBLE, 0 );
   }

   if ( szMode[ 0 ] == 'E' || szMode[ 0 ] == 'M' )
   {
      SetCtrlState( vSubtask, "OI_ListAttributes", zCONTROL_STATUS_VISIBLE, 0 );
   }
   else
   {
      SetCtrlState( vSubtask, "OI_ListAttributes", zCONTROL_STATUS_VISIBLE, 1 );
   }

   lpViewCsr       = (LPVIEWCSR) SysGetPointerFromHandle( vTZOLOBWO->hViewCsr );
   lpViewEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpViewCsr->hFirstOD_EntityCsr );
   lpViewEntity    = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
// DisplayEntityInstance( vTZOLOBWO, lpViewEntity->szName );

   if ( lpViewEntityCsr == 0 )
   {
      if ( szMode[ 0 ] == 'E' )
      {
         RefreshCtrl( vSubtask, "Edit" );
         RefreshCtrl( vSubtask, "ListAttributes" );
      }

      return( 0 );
   }

   if ( CheckExistenceOfEntity( vTZOLOBWO, pchEntityName ) > zCURSOR_NULL )
   {
      fnSwapTaskContext( vSubtask );

      if ( szMode[ 0 ] == 'M' )
      {
         CreateViewFromViewForTask( &vWork, vTZOBOIOO, vSubtask );
         lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewEntityCsr->hEntityInstance );
         lpSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpViewCsr->hFirstSelectedInstance );
         while ( lpSelectedInstance &&
                 ((LPENTITYINSTANCE) SysGetPointerFromHandle( lpSelectedInstance->xEntityInstance ) != lpEntityInstance ||
                   lpSelectedInstance->hViewEntity != lpEntityInstance->hViewEntity) )
         {
            lpSelectedInstance = (LPSELECTEDINSTANCE) SysGetPointerFromHandle( lpSelectedInstance->hNextSelectedInstance );
         }


         CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
         sprintf_s( szAttribute, sizeof( szAttribute ), "Instance Ptr 0x%8lx  Key %d", (zULONG) lpEntityInstance, lpEntityInstance->ulKey );
         SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );

         if ( lpSelectedInstance && lpSelectedInstance->nSelectSet )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            strcpy_s( szAttribute, sizeof( szAttribute ), "Selected(" );
            nWk1 = 0;
            nWk2 = 0x0001;
            for ( nWk3 = 1; nWk3 < 17; nWk3++ )
            {
               if ( lpSelectedInstance->nSelectSet & nWk2 )
               {
                  if ( nWk1 )
                     strcat_s( szAttribute, sizeof( szAttribute ), "," );

                  zltoa( nWk3, szAttribute + 250, sizeof( szAttribute ) - 250 );
                  strcat_s( szAttribute, sizeof( szAttribute ), szAttribute + 250 );
                  nWk1++;
               }
               nWk2 <<= 1;
            }

            strcat_s( szAttribute, sizeof( szAttribute ), ")" );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
         }

         if ( lpEntityInstance->u.nInd.bCreated )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Created" );
         }

         if ( lpEntityInstance->u.nInd.bUpdated )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Updated" );
         }
         if ( lpEntityInstance->u.nInd.bIncluded )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Included" );
         }

         if ( lpEntityInstance->u.nInd.bTemporal )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Temporal Entity" );
         }
         else
         if ( lpEntityInstance->hPrevVsn )
         {
            LPENTITYINSTANCE lpPrevVsn;
            LPVIEWENTITY     lpLinkViewEntity;

            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            lpLinkInstance = lpEntityInstance;
            lpPrevVsn = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hPrevVsn );
            if ( lpPrevVsn->u.nInd.bPrevVsnRoot == FALSE )
            {
               while ( lpPrevVsn->u.nInd.bPrevVsnRoot == FALSE )
               {
                  lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hParent );
                  lpPrevVsn = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hPrevVsn );
               }

               lpLinkViewEntity = (LPVIEWENTITY)  SysGetPointerFromHandle( lpLinkInstance->hViewEntity );
               strcpy_s( szAttribute, sizeof( szAttribute ), "Versioned by " );
               strcat_s( szAttribute, sizeof( szAttribute ), lpLinkViewEntity->szName );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
            }
            else
            {
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, "Versioned" );
            }
         }
         else
         if ( lpEntityInstance->u.nInd.bPrevVersion )
         {
            lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextLinked );
            while ( lpLinkInstance && lpLinkInstance != lpEntityInstance )
            {
               if ( lpLinkInstance->hNextVsn )
                  break;

               lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hNextLinked );
            }

            if ( lpLinkInstance && lpLinkInstance->hNextVsn )
            {
               CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName,
                                       "Previous Version from another path" );
            }
            else
            {
               CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName,
                                       "??? Marked but not a Previous Version from another path" );
            }
         }

         lLinks = 0;
         lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextLinked );
         while ( lpLinkInstance && lpLinkInstance != lpEntityInstance )
         {
            zBOOL bDup;
            if ( lpLinkInstance->u.nInd.bHidden == FALSE )
            {
               LPVIEWENTITY lpLinkViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpLinkInstance->hViewEntity );

               if ( lpLinkInstance->hViewOI == lpEntityInstance->hViewOI )
               {
                  strcpy_s( szAttribute, sizeof( szAttribute ), "Internal Link to " );
                  strcat_s( szAttribute, sizeof( szAttribute ), lpLinkViewEntity->szName );
               }
               else
               {
                  LPVIEWOI lpLinkOI = (LPVIEWOI) SysGetPointerFromHandle( lpLinkInstance->hViewOI );
                  LPVIEWOD lpLinkOD = (LPVIEWOD) SysGetPointerFromHandle( lpLinkOI->hViewOD );
                  LPVIEWENTITY lpLinkEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpLinkInstance->hViewEntity );

                  strcpy_s( szAttribute, sizeof( szAttribute ), "External Link to " );
                  strcat_s( szAttribute, sizeof( szAttribute ), lpLinkOD->szName );
                  strcat_s( szAttribute, sizeof( szAttribute ), "." );
                  strcat_s( szAttribute, sizeof( szAttribute ), lpLinkViewEntity->szName );
               }

               bDup = 0;
               if ( SetCursorFirstEntityByString( vWork,
                           szlAttrib, szlName, szAttribute, 0 ) == 0 )
               {
                  bDup = 1;
               }

               if ( bDup == FALSE )
               {
                  CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
                  SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
               }
            }

            lpLinkInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpLinkInstance->hNextLinked );
            lLinks++;
         }

         if ( lLinks )
         {
            CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
            strcpy_s( szAttribute, sizeof( szAttribute ), "Number of links is " );
            zltoa( lLinks, szAttribute + 250, sizeof( szAttribute ) - 250 );
            strcat_s( szAttribute, sizeof( szAttribute ), szAttribute + 250 );
            SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, szAttribute );
         }

         DropView( vWork );
      }
      else
      {
         lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
         lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
         if ( szMode[ 0 ] == 'A' )
         {
            while ( lpViewAttrib )
            {
               if ( lpViewAttrib->bHidden == FALSE )
               {
                  if ( lpViewAttrib->cType == 'B' )
                  {
                     zULONG uLth;

                     GetAttributeLength( &uLth, vTZOLOBWO, pchEntityName, lpViewAttrib->szName );
                     if ( uLth > 60 || uLth == 0 )
                     {
                        strcpy_s( szAttribute, sizeof( szAttribute ), "(Blob, lth=" );
                        zltoa( uLth, szAttribute + 50, sizeof( szAttribute ) - 50 );
                        strcat_s( szAttribute, sizeof( szAttribute ), szAttribute + 50 );
                        strcat_s( szAttribute, sizeof( szAttribute ), ")" );
                     }
                     else
                     {
                        GetAddrForAttribute( &pchBlob, vTZOLOBWO, pchEntityName, lpViewAttrib->szName );
                        nWk1 = 2;
                        strcpy_s( szAttribute, sizeof( szAttribute ), "x'" );
                        for ( nWk2 = 0; nWk2 < (zSHORT) uLth; nWk2++ )
                        {
                           ucWk = pchBlob[ nWk2 ];
                           ucWk >>= 4;
                           ucWk = HexTable[ ucWk ];
                           szAttribute[ nWk1++ ] = ucWk;
                           ucWk = pchBlob[ nWk2 ];
                           ucWk <<= 4;
                           ucWk >>= 4;
                           ucWk = HexTable[ ucWk ];
                           szAttribute[ nWk1++ ] = ucWk;
                        }
                        szAttribute[ nWk1++ ] = '\'';
                        szAttribute[ nWk1++ ] = ',';
                        szAttribute[ nWk1++ ] = '(';
                        for ( nWk2 = 0; nWk2 < (zSHORT) uLth; nWk2++ )
                        {
                           ucWk = pchBlob[ nWk2 ];
                           if ( isprint( ucWk ) )
                              szAttribute[ nWk1++ ] = ucWk;
                           else
                              szAttribute[ nWk1++ ] = '.';
                        }
                        szAttribute[ nWk1++ ] = ')';
                        szAttribute[ nWk1 ] = 0;
                     }
                  }
                  else
                  {
                     GetVariableFromAttribute( szAttribute, 0, zTYPE_STRING,
                                               298, vTZOLOBWO, pchEntityName,
                                               lpViewAttrib->szName, 0, 0 );
                  }

                  CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
                  SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, lpViewAttrib->szName );
                  SetAttributeFromString( vTZOBOIOO, szlAttrib, szlValue, szAttribute );
               }

               lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
            }
         }
         else
         {
            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntityCsr->hViewEntity );
            while ( lpViewAttrib )
            {
               if ( lpViewAttrib->cType == 'S' &&
                    lpViewAttrib->bHidden == FALSE )
               {
                  break;
               }

               lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
            }

            if ( lpViewAttrib == 0 )
            {
               lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
               while ( lpViewAttrib )
               {
                  if ( lpViewAttrib->cType != 'B' &&
                       lpViewAttrib->bHidden == FALSE )
                  {
                     break;
                  }

                  lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
               }
            }

            CreateViewFromViewForTask( &vWork, vTZOLOBWO, vSubtask );
            lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vWork->hViewCsr );
            lpWkEntityCsr = lpViewCsr->hRootViewEntityCsr;
            while ( (LPVIEWENTITY) SysGetPointerFromHandle( lpWkEntityCsr->hViewEntity ) != lpViewEntity )
            {
               lpWkEntityCsr = (LPVIEWENTITYCSR) SysGetPointerFromHandle( lpWkEntityCsr->hNextHier );
            }

            SetCursorFirstEntity( vWork, lpViewEntity->szName, 0 );
            do
            {
               CreateEntity( vTZOBOIOO, szlAttrib, zPOS_AFTER );
               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlName, lpViewEntity->szName );
               if ( lpViewAttrib )
               {
                  GetVariableFromAttribute( szAttribute, 0, zTYPE_STRING,
                                            298, vWork, pchEntityName,
                                            lpViewAttrib->szName, 0, 0 );
               }
               else
                  szAttribute[ 0 ] = 0;

               SetAttributeFromString( vTZOBOIOO, szlAttrib, szlValue, szAttribute );
               if ( lpWkEntityCsr->hEntityInstance == lpViewEntityCsr->hEntityInstance )
               {
                  SetSelectStateOfEntity( vTZOBOIOO, szlAttrib, 1 );
               }

            }  while ( SetCursorNextEntity( vWork, lpViewEntity->szName, 0 ) >= zCURSOR_SET );
            DropView( vWork );
         }
      }

      fnSwapTaskContext( vSubtask );
   }

// DisplayObjectInstance( vTZOBOIOO, "", 0 );
   RefreshCtrl( vSubtask, "Edit" );
   RefreshCtrl( vSubtask, "ListAttributes" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_SetModeEntities
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SetModeEntities( zVIEW vSubtask )
{
   zVIEW vTZOBOIOO;
   zCHAR szMode[ 2 ];

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      if ( szMode[ 0 ] == 'E' )
         return( 0 );
   }
   else
   {
      ActivateEmptyObjectInstance( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zSINGLE );
      SetNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
      CreateEntity( vTZOBOIOO, szlName, zPOS_AFTER );
   }

   SetAttributeFromString( vTZOBOIOO, szlName, szlMode, "E" );
   zwTZOBRWAD_BuildOutlinerAttrs( vSubtask );

   // Why can't we refresh the Radio Group (instead of each button in it)???
// RefreshCtrl( vSubtask, "OI_Detail" );
// RefreshCtrl( vSubtask, "OI_Attributes" );
// RefreshCtrl( vSubtask, "OI_Twins" );
// RefreshCtrl( vSubtask, "OI_Entities" );
// RefreshCtrl( vSubtask, "OI_Markings" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_SetModeMarkings
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SetModeMarkings( zVIEW vSubtask )
{
   zVIEW vTZOBOIOO;
   zCHAR szMode[ 2 ];

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      if ( szMode[ 0 ] == 'M' )
         return( 0 );
   }
   else
   {
      ActivateEmptyObjectInstance( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zSINGLE );
      SetNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
      CreateEntity( vTZOBOIOO, szlName, zPOS_AFTER );
   }

   SetAttributeFromString( vTZOBOIOO, szlName, szlMode, "M" );
   zwTZOBRWAD_BuildOutlinerAttrs( vSubtask );

   // Why can't we refresh the Radio Group (instead of each button in it)???
// RefreshCtrl( vSubtask, "OI_Detail" );
// RefreshCtrl( vSubtask, "OI_Attributes" );
// RefreshCtrl( vSubtask, "OI_Twins" );
// RefreshCtrl( vSubtask, "OI_Entities" );
// RefreshCtrl( vSubtask, "OI_Markings" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_SetModeAttributes
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SetModeAttributes( zVIEW vSubtask )
{
   zVIEW vTZOBOIOO;
   zCHAR szMode[ 2 ];

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      if ( szMode[ 0 ] == 'A' )
         return( 0 );
   }
   else
   {
      ActivateEmptyObjectInstance( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zSINGLE );
      SetNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
      CreateEntity( vTZOBOIOO, szlName, zPOS_AFTER );
   }

   SetAttributeFromString( vTZOBOIOO, szlName, szlMode, "A" );
   zwTZOBRWAD_BuildOutlinerAttrs( vSubtask );

   // Why can't we refresh the Radio Group (instead of each button in it)???
// RefreshCtrl( vSubtask, "OI_Detail" );
// RefreshCtrl( vSubtask, "OI_Attributes" );
// RefreshCtrl( vSubtask, "OI_Twins" );
// RefreshCtrl( vSubtask, "OI_Entities" );
// RefreshCtrl( vSubtask, "OI_Markings" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: zwTZOBRWAD_SetModeTwins
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SetModeTwins( zVIEW vSubtask )
{
   zVIEW vTZOBOIOO;
   zCHAR szMode[ 2 ];

   if ( GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK ) > 0 )
   {
      GetStringFromAttribute( szMode, sizeof( szMode ), vTZOBOIOO, szlName, szlMode );
      if ( szMode[ 0 ] == 'T' )
         return( 0 );
   }
   else
   {
      ActivateEmptyObjectInstance( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zSINGLE );
      SetNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
      CreateEntity( vTZOBOIOO, szlName, zPOS_AFTER );
   }

   SetAttributeFromString( vTZOBOIOO, szlName, szlMode, "T" );
   zwTZOBRWAD_BuildOutlinerAttrs( vSubtask );

   // Why can't we refresh the Radio Group (instead of each button in it)???
// RefreshCtrl( vSubtask, "OI_Detail" );
// RefreshCtrl( vSubtask, "OI_Attributes" );
// RefreshCtrl( vSubtask, "OI_Twins" );
// RefreshCtrl( vSubtask, "OI_Entities" );
// RefreshCtrl( vSubtask, "OI_Markings" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_PostBuildMainW( zVIEW vSubtask )
{
   SetDefaultViewForActiveTask( vSubtask, 4 );
// zwTZOBRWAD_CheckPrintPreview( vSubtask );
// zwTZOBRWAD_CheckShowDesc( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SortViewList( zVIEW vSubtask )
{
#if 0
   zVIEW  vMetaList;
   zPCHAR pchListButton;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "TZOBRWOO", vSubtask, zLEVEL_TASK );
   pchListButton = GetActionParameters( vSubtask );
   if ( vMetaList && pchListButton && pchListButton[ 0 ] )
   {
      if ( zstrcmp( pchListButton, "ViewName" ) == 0 )
         nRC = OrderEntityForView( vMetaList, "Name", "Name A" );
      else
      if ( zstrcmp( pchListButton, "Type" ) == 0 )
         nRC = OrderEntityForView( vMetaList, "Name", "ObjectType A" );
      else
      if ( zstrcmp( pchListButton, "ViewID" ) == 0 )
         nRC = OrderEntityForView( vMetaList, "Name", "ID A" );
      else
      if ( zstrcmp( pchListButton, "Level" ) == 0 )
         nRC = OrderEntityForView( vMetaList, "Name", "Level A" );
      else
      if ( zstrcmp( pchListButton, "OI_ID" ) == 0 )
         nRC = OrderEntityForView( vMetaList, "Name", "ObjectInstanceID A" );
      else
      if ( zstrcmp( pchListButton, "AppName" ) == 0 )
         nRC = OrderEntityForView( vMetaList, "Name", "ApplicationName A" );
      else
         return( 0 );

      RefreshCtrl( vSubtask, "ListViews" );
   }
   else
      nRC = 0;

   return( nRC );
#else
   return( 0 );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//./ ADD NAME=BuildRelatedOI_ForView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: BuildRelatedOI_ForView
//
//   PURPOSE:   Build an object instance containing the information for
//              a view.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zVIEW
BuildRelatedOI_ForView( zVIEW vSubtask, zVIEW vCompare )
{
   zVIEW            vBrowser;
   zVIEW            vProfile;
   zVIEW            vHD_Compare;
   zPCHAR           pchShowHidden;
   LPVIEWCSR        lpViewCsr;
   LPVIEWOI         lpViewOI;

   if ( vCompare == 0 || vCompare->hViewCsr == 0 )
      return( 0 );

   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vBrowser, szlName ) < zCURSOR_SET )
      return( 0 );

   oTZ__PRFO_GetViewToProfile( &vProfile, "OB", vSubtask, zCURRENT_OI );
   GetAddrForAttribute( &pchShowHidden, vProfile, "OBR", "ShowHidden" );

   ActivateEmptyObjectInstance( &vHD_Compare, szlTZZOHDGO, vSubtask, zSINGLE );
   CreateEntity( vHD_Compare, szlHD_Diagram, zPOS_AFTER );
   SetAttributeFromInteger( vHD_Compare, szlHD_Diagram, szlZKey, (zLONG) vCompare );

   // See if there is any instance data to display.
   if ( vCompare->hViewCsr )
   {
      lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vCompare->hViewCsr );
      lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
   }
   else
      lpViewOI = 0;

   if ( lpViewOI && lpViewOI->hRootEntityInstance )
   {
      // Now go through each instance in the target object, plug the cursor
      // for the instance and retrieve the attributes for each entity
      zLONG  lEntityCnt = 0;
      zSHORT nRC = 0;
      LPENTITYINSTANCE lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpViewOI->hRootEntityInstance );

      while ( nRC == 0 && lpEntityInstance )
      {
         if ( lpEntityInstance->u.nInd.bHidden == FALSE ||
              pchShowHidden[ 0 ] == 'Y' )
         {
            CreateEntity( vHD_Compare, szlHD_Node, zPOS_AFTER );
            SetAttributeFromInteger( vHD_Compare, szlHD_Node, szlZKey, lEntityCnt );
         }

         lEntityCnt++;
         lpEntityInstance = (LPENTITYINSTANCE) SysGetPointerFromHandle( lpEntityInstance->hNextHier );
      }
   }
   else
   {
      OperatorPrompt( vSubtask, "Object Browser", "Object instance is empty!", 0, 0, 0, zICON_STOP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   DropView( vProfile );
   return( vHD_Compare );
}

//./ ADD NAME=zwTZOBRWAD_FlagView
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwTZOBRWAD_FlagView
//
//   Toggles the flag for a view that tells Core to pop up a message when the
//   view is dropped.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_FlagView( zVIEW vSubtask )
{
   zVIEW  vBrowser;
   zVIEW  lpView;
   zCHAR  sz[ 100 ];

   // First, get the handle to the browser object
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( sz, sizeof( sz ), vBrowser, szlName, szlID );
   lpView = (zVIEW) zatol( sz );

   lpView->bFlagDropView = !lpView->bFlagDropView;

   if ( lpView->bFlagDropView )
      SetAttributeFromString( vBrowser, szlName, "ShowMessageWhenViewIsDropped", "Y" );
   else
      SetAttributeFromString( vBrowser, szlName, "ShowMessageWhenViewIsDropped", "N" );

   return( 0 );
}

//./ ADD NAME=zwTZOBRWAD_SelectViewForCompare
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwTZOBRWAD_SelectViewForCompare
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_SelectViewForCompare( zVIEW vSubtask )
{
   zVIEW  vBrowser;
   zVIEW  vTZZOHDGV;
   zVIEW  vTZZOHDGO;
   zCHAR  szPopupTag[ 33 ];

   // First, get the handle to the browser object
   GetViewByName( &vBrowser, szlTZOBRWOO, vSubtask, zLEVEL_TASK );
   GetLastCommandTag( vSubtask, szPopupTag );
   if ( szPopupTag[ 0 ] != '_' ||
        vBrowser == 0 ||
        CheckExistenceOfEntity( vBrowser, szlName ) < zCURSOR_SET )
   {
      return( 0 );
   }

   GetIntegerFromAttribute( (zPLONG) &vTZZOHDGV, vBrowser, szlName, szlID );
   vTZZOHDGV = (zVIEW) SysGetPointerFromHandle( vTZZOHDGV );
   if ( vTZZOHDGV->hViewCsr == 0 )
   {
      OperatorPrompt( vSubtask, "Object Browser Select View",
                      "Object instance has no data!", 0, 0, 0, zICON_STOP );
      return( 0 );
   }

   if ( GetViewByName( &vTZZOHDGO, szPopupTag, vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZZOHDGO );

   ActivateEmptyObjectInstance( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zSINGLE );
   SetNameForView( vTZZOHDGO, szPopupTag, vSubtask, zLEVEL_TASK );
   CreateEntity( vTZZOHDGO, szlHD_Diagram, zPOS_AFTER );
   SetAttributeFromInteger( vTZZOHDGO, szlHD_Diagram, szlZKey, (zLONG) vTZZOHDGV );
   return( 0 );
}

//./ ADD NAME=CompareBuildAttrValueList
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: CompareBuildAttrValueList
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
CompareBuildAttrValueList( zVIEW vSubtask, zCPCHAR cpcTag )
{
   zCHAR szTag[ 33 ];
   zVIEW vHD_Compare;
   zVIEW vCompare;
   zVIEW vTZZOHDGO;
   zVIEW vTZZOHDGV;
   zVIEW vTZOBOIOO;
   zBOOL bFirst;

   strcpy_s( szTag, cpcTag );

   if ( GetViewByName( &vTZZOHDGO, szlTZZOHDGO, vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vTZZOHDGO );

   if ( GetViewByName( &vTZZOHDGV, szlTZZOHDGV, vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vTZZOHDGV );

   GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_ANY );
   if ( vTZOBOIOO )
      DropObjectInstance( vTZOBOIOO );

   if ( zstrchr( szTag, '1' ) )
   {
      GetViewByName( &vTZOBOIOO, "TZOBOIOO1", vSubtask, zLEVEL_ANY );
      if ( vTZOBOIOO )
         DropObjectInstance( vTZOBOIOO );

      GetViewByName( &vHD_Compare, "__HD_Compare1", vSubtask, zLEVEL_TASK );
      GetViewByName( &vCompare, "__Compare1", vSubtask, zLEVEL_TASK );
      strcpy_s( szTag, "CompareAttr1" );
      bFirst = TRUE;
   }
   else
   {
      GetViewByName( &vTZOBOIOO, "TZOBOIOO2", vSubtask, zLEVEL_ANY );
      if ( vTZOBOIOO )
         DropObjectInstance( vTZOBOIOO );

      GetViewByName( &vHD_Compare, "__HD_Compare2", vSubtask, zLEVEL_TASK );
      GetViewByName( &vCompare, "__Compare2", vSubtask, zLEVEL_TASK );
      strcpy_s( szTag, "CompareAttr2" );
      bFirst = FALSE;
   }

   SetNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );

   zSHORT nRC = BuildAttrsForGraph( vSubtask, szTag );

   GetViewByName( &vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   if ( bFirst )
      SetNameForView( vTZOBOIOO, "TZOBOIOO1", vSubtask, zLEVEL_SUBTASK );
   else
      SetNameForView( vTZOBOIOO, "TZOBOIOO2", vSubtask, zLEVEL_SUBTASK );

   DropNameForView( vTZOBOIOO, szlTZOBOIOO, vSubtask, zLEVEL_SUBTASK );
   DropNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   DropNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );

   RefreshCtrl( vSubtask, szTag );
   return( nRC );
}

char g_szIgnoreAttrList[ 256 ];

//./ ADD NAME=zwCompareBuildAttrValueList
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwCompareBuildAttrValueList
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwCompareBuildAttrValueList( zVIEW vSubtask )
{
   zCHAR szTag[ 33 ];

   GetLastCommandTag( vSubtask, szTag );
   return( CompareBuildAttrValueList( vSubtask, szTag ) );
}

//./ ADD NAME=BuildCompareViews
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: BuildCompareViews
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
BuildCompareViews( zVIEW vSubtask )
{
   zCHAR        szObjectName1[ 34 ];
   zCHAR        szObjectName2[ 34 ];
   zVIEW        vCompare1;
   zVIEW        vCompare2;
   zVIEW        vHD_Compare1;
   zVIEW        vHD_Compare2;
   LPVIEWENTITY lpViewEntity;
   zSHORT       nLastLevel;
   LPVIEWOD     lpViewOD;

   GetViewByName( &vCompare1, "__SystemCompare1", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare2, "__SystemCompare2", vSubtask, zLEVEL_TASK );
   if ( vCompare1 == 0 || vCompare2 == 0 )
   {
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Views have not been selected for comparison\n"
                   "Please select views to be compared!",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetIntegerFromAttribute( (zPLONG) &vCompare1, vCompare1, szlHD_Diagram, szlZKey );
   GetIntegerFromAttribute( (zPLONG) &vCompare2, vCompare2, szlHD_Diagram, szlZKey );
   if ( vCompare1->hViewCsr == 0 || vCompare2->hViewCsr == 0 )
   {
      OperatorPrompt( vSubtask, "Object Browser Build Compare",
                      "Object instance has no data!", 0, 0, 0, zICON_STOP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   MiGetObjectNameForView( szObjectName1, vCompare1 );
   MiGetObjectNameForView( szObjectName2, vCompare2 );
   if ( zstrcmp( szObjectName1, szObjectName2 ) != 0 )
   {
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Views selected for comparison are not of the same type\n"
                   "Please select compatible views to be compared!",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   if ( GetViewByName( &vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropObjectInstance( vHD_Compare1 );
   }

   if ( GetViewByName( &vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropObjectInstance( vHD_Compare2 );
   }

   ActivateEmptyObjectInstance( &vHD_Compare1, szlTZZOHDGO, vSubtask, zSINGLE );
   ActivateEmptyObjectInstance( &vHD_Compare2, szlTZZOHDGO, vSubtask, zSINGLE );
   SetNameForView( vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK );
   SetNameForView( vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK );
   CreateEntity( vHD_Compare1, szlHD_Diagram, zPOS_AFTER );
   CreateEntity( vHD_Compare2, szlHD_Diagram, zPOS_AFTER );

   SetAttributeFromInteger( vHD_Compare1, szlHD_Diagram, szlZKey, (zLONG) vCompare1 );
   SetAttributeFromInteger( vHD_Compare2, szlHD_Diagram, szlZKey, (zLONG) vCompare2 );

   SetNameForView( vCompare1, "__Compare1", vSubtask, zLEVEL_TASK );
   SetNameForView( vCompare2, "__Compare2", vSubtask, zLEVEL_TASK );
   lpViewOD     = (LPVIEWOD) SysGetPointerFromHandle( vCompare1->hViewOD );
   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
   nLastLevel = lpViewEntity->nLevel;
   while ( lpViewEntity )
   {
      if ( lpViewEntity->nLevel > nLastLevel )
      {
         SetViewToSubobject( vHD_Compare1, szlHD_ChildNode );
         nLastLevel++;
      }
      else
      {
         while ( nLastLevel > lpViewEntity->nLevel )
         {
            ResetViewFromSubobject( vHD_Compare1 );
            nLastLevel--;
         }
      }

      CreateEntity( vHD_Compare1, szlHD_Node, zPOS_AFTER );
      SetAttributeFromString( vHD_Compare1, szlHD_Node, szlDIL, lpViewEntity->szName );
      SetAttributeFromInteger( vHD_Compare1, szlHD_Node, szlZKey, (zLONG) lpViewEntity->nHierNbr );
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
   }

   SetNameForView( vHD_Compare1, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vCompare1, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   zwTZOBRWAD_BuildTextForGraph( vSubtask );

   lpViewOD     = (LPVIEWOD) SysGetPointerFromHandle( vCompare2->hViewOD );
   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
   nLastLevel = lpViewEntity->nLevel;
   while ( lpViewEntity )
   {
      if ( lpViewEntity->nLevel > nLastLevel )
      {
         SetViewToSubobject( vHD_Compare2, szlHD_ChildNode );
         nLastLevel++;
      }
      else
      {
         while ( nLastLevel > lpViewEntity->nLevel )
         {
            ResetViewFromSubobject( vHD_Compare2 );
            nLastLevel--;
         }
      }

      CreateEntity( vHD_Compare2, szlHD_Node, zPOS_AFTER );
      SetAttributeFromString( vHD_Compare2, szlHD_Node, szlDIL, lpViewEntity->szName );
      SetAttributeFromInteger( vHD_Compare2, szlHD_Node, szlZKey, (zLONG) lpViewEntity->nHierNbr );
      lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
   }

   SetNameForView( vHD_Compare2, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vCompare2, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   zwTZOBRWAD_BuildTextForGraph( vSubtask );

   DropNameForView( vHD_Compare2, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   DropNameForView( vCompare2, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );

   zVIEW v;

   GetViewByName( &v, "__FlatCompare1", vSubtask, zLEVEL_TASK );
   if ( v )
      DropView( v );

   GetViewByName( &v, "__FlatCompare2", vSubtask, zLEVEL_TASK );
   if ( v )
      DropView( v );

   v = BuildRelatedOI_ForView( vSubtask, vCompare1 );
   SetNameForView( v, "__FlatCompare1", vSubtask, zLEVEL_TASK );

   v = BuildRelatedOI_ForView( vSubtask, vCompare2 );
   SetNameForView( v, "__FlatCompare2", vSubtask, zLEVEL_TASK );

   CompareObjects( vSubtask );

   SetNameForView( vHD_Compare1, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vCompare1, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   zwTZOBRWAD_BuildTextForGraph( vSubtask, "1" );
   CompareBuildAttrValueList( vSubtask, "1" );
   SetNameForView( vHD_Compare2, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vCompare2, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   zwTZOBRWAD_BuildTextForGraph( vSubtask, "2" );
   CompareBuildAttrValueList( vSubtask, "2" );
   DropNameForView( vHD_Compare2, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   DropNameForView( vCompare2, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );

   return( 0 );
}

//./ ADD NAME=zwTZOBRWAD_BuildCompareViews
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: zwTZOBRWAD_BuildCompareViews
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_BuildCompareViews( zVIEW vSubtask )
{
   g_szIgnoreAttrList[ 0 ] = 0;
   return( BuildCompareViews( vSubtask ) );
}

//./ ADD NAME=CompareIgnoringAttributes
// Source Module=tzobrwad.c
/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: CompareIgnoringAttributes
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CompareExcludingAttributes( zVIEW vSubtask )
{
   OperatorPromptForInput( vSubtask, "Object Compare",
                           "Enter list of attributes to exclude for compare",
                           FALSE, g_szIgnoreAttrList,
                           sizeof( g_szIgnoreAttrList ) );
   return( BuildCompareViews( vSubtask ) );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ComparePostBuild( zVIEW vSubtask )
{
   HD_ZoomDiagram( vSubtask, "CompareHD1", 0, 3 );
   HD_ZoomDiagram( vSubtask, "CompareHD2", 0, 3 );
   HD_ZoomDiagram( vSubtask, "CompareHD1", 25, 2 );
   HD_ZoomDiagram( vSubtask, "CompareHD2", 25, 2 );
   HD_ZoomDiagram( vSubtask, "CompareHD1", 25, 2 );
   HD_ZoomDiagram( vSubtask, "CompareHD2", 25, 2 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CompareZoomIn( zVIEW vSubtask )
{
   HD_ZoomDiagram( vSubtask, "CompareHD1", 25, 2 );
   HD_ZoomDiagram( vSubtask, "CompareHD2", 25, 2 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CompareZoomOut( zVIEW vSubtask )
{
   HD_ZoomDiagram( vSubtask, "CompareHD1", -25, 2 );
   HD_ZoomDiagram( vSubtask, "CompareHD2", -25, 2 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_CompareNext( zVIEW vSubtask )
{
   zCHAR  szTag[ 33 ];
   zVIEW  vCompare;
   zVIEW  vHD_Compare;

   GetLastCommandTag( vSubtask, szTag );
   if ( zstrcmp( szTag, "Next" ) == 0 || zstrcmp( szTag, "Next1" ) == 0 )
   {
      GetViewByName( &vHD_Compare, "__HD_Compare1", vSubtask, zLEVEL_TASK );
      SetNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );

      GetViewByName( &vCompare, "__SystemCompare1", vSubtask, zLEVEL_TASK );
      GetIntegerFromAttribute( (zPLONG) &vCompare, vCompare, szlHD_Diagram, szlZKey );
      SetNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
      zwTZOBRWAD_CursorChange( vSubtask, zPOS_NEXT, "CompareHD1" );
   }

   if ( zstrcmp( szTag, "Next" ) == 0 || zstrcmp( szTag, "Next2" ) == 0 )
   {
      GetViewByName( &vHD_Compare, "__HD_Compare2", vSubtask, zLEVEL_TASK );
      SetNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );

      GetViewByName( &vCompare, "__SystemCompare2", vSubtask, zLEVEL_TASK );
      GetIntegerFromAttribute( (zPLONG) &vCompare, vCompare, szlHD_Diagram, szlZKey );
      SetNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
      zwTZOBRWAD_CursorChange( vSubtask, zPOS_NEXT, "CompareHD2" );
   }

   zwCompareBuildAttrValueList( vSubtask );
   DropNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   DropNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_ComparePrev( zVIEW vSubtask )
{
   zCHAR  szTag[ 33 ];
   zVIEW  vCompare;
   zVIEW  vHD_Compare;

   GetLastCommandTag( vSubtask, szTag );
   if ( zstrcmp( szTag, "Prev" ) == 0 || zstrcmp( szTag, "Prev1" ) == 0 )
   {
      GetViewByName( &vHD_Compare, "__HD_Compare1", vSubtask, zLEVEL_TASK );
      SetNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );

      GetViewByName( &vCompare, "__SystemCompare1", vSubtask, zLEVEL_TASK );
      GetIntegerFromAttribute( (zPLONG) &vCompare, vCompare, szlHD_Diagram, szlZKey );
      SetNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
      zwTZOBRWAD_CursorChange( vSubtask, zPOS_PREV, "CompareHD1" );
   }

   if ( zstrcmp( szTag, "Prev" ) == 0 || zstrcmp( szTag, "Prev2" ) == 0 )
   {
      GetViewByName( &vHD_Compare, "__HD_Compare2", vSubtask, zLEVEL_TASK );
      SetNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );

      GetViewByName( &vCompare, "__SystemCompare2", vSubtask, zLEVEL_TASK );
      GetIntegerFromAttribute( (zPLONG) &vCompare, vCompare, szlHD_Diagram, szlZKey );
      SetNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
      zwTZOBRWAD_CursorChange( vSubtask, zPOS_PREV, "CompareHD2" );
   }

   zwCompareBuildAttrValueList( vSubtask );
   DropNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   DropNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   return( 0 );
}

void
MiGetRootEntityForView( zVIEW v, zPCHAR pchRootEntity )
{
   zCHAR szParentEntity[ 33 ];
   LPVIEWOD lpViewOD = (LPVIEWOD) MiGetViewEntityForView( v, 0 );
   LPVIEWENTITY lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
   strcpy_s( szParentEntity, lpViewEntity->szName );
   while ( szParentEntity[ 0 ] )
   {
      strcpy_s( pchRootEntity, 33, szParentEntity );
      MiGetParentEntityNameForView( szParentEntity, v, pchRootEntity );
   }
}

void
CompareSelectNode( zVIEW vSubtask, zVIEW vCompare, zVIEW vFlatCompare,
                   zVIEW vHD_Compare, zCPCHAR cpcHD_CtrlTag )
{
   zCHAR   szEntityName[ 33 ];
   zCHAR   szHD_EntityName[ 33 ];
   zCHAR   szTempEntityName[ 33 ];
   zLONG   lAbsolutePos;
   zUSHORT uReturnLevel;
   zSHORT  nRC;

   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vFlatCompare, szlHD_Node, szlDIL );
   GetIntegerFromAttribute( &lAbsolutePos, vFlatCompare, szlHD_Node, szlZKey );
   SetCursorAbsolutePosition( szHD_EntityName, vCompare, lAbsolutePos );

   lAbsolutePos = -1;
   ResetView( vHD_Compare );
   MiGetRootEntityForView( vHD_Compare, szHD_EntityName );
   DefineHierarchicalCursor( vHD_Compare, szHD_EntityName );
   while ( (nRC = SetCursorNextEntityHierarchical( &uReturnLevel, szHD_EntityName, vHD_Compare )) >= zCURSOR_SET )
   {
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
         SetViewToSubobject( vHD_Compare, szlHD_ChildNode );

      GetStringFromAttribute( szTempEntityName, sizeof( szTempEntityName ), vHD_Compare, szlHD_Node, szlDIL );
      if ( zstrcmp( szTempEntityName, szEntityName ) == 0 )
      {
         GetAbsolutePositionForEntity( &lAbsolutePos, vHD_Compare, szlHD_Node );
         break;
      }
   }

   DropHierarchicalCursor( vHD_Compare );
   if ( lAbsolutePos >= 0 )
      SetCursorAbsolutePosition( szHD_EntityName, vHD_Compare, lAbsolutePos );

   SetNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   SetNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
// zwTZOBRWAD_BuildTextForGraph( vSubtask, cpcHD_CtrlTag );
   zwTZOBRWAD_CursorChange( vSubtask, zPOS_CURR, cpcHD_CtrlTag );
   DropNameForView( vHD_Compare, szlTZZOHDGO, vSubtask, zLEVEL_SUBTASK );
   DropNameForView( vCompare, szlTZZOHDGV, vSubtask, zLEVEL_SUBTASK );
   CompareBuildAttrValueList( vSubtask, cpcHD_CtrlTag );
   if ( HD_IsNodeVisible( vSubtask, cpcHD_CtrlTag ) == 0 )
      CenterNode( vSubtask, cpcHD_CtrlTag );
   else
      HD_SelectNode( vSubtask, cpcHD_CtrlTag );
}

zOPER_EXPORT zSHORT OPERATION
CompareFirstDifference( zVIEW vSubtask )
{
   zVIEW  vCompare1;
   zVIEW  vCompare2;
   zVIEW  vFlatCompare1;
   zVIEW  vFlatCompare2;
   zVIEW  vHD_Compare1;
   zVIEW  vHD_Compare2;
   zLONG  rgbColor = RGB( 0, 255, 0 );
   zLONG  rgbCurrColor;
   zLONG  lAbsolutePos1 = MAXLONG;
   zLONG  lMatch1 = MAXLONG;
   zLONG  lAbsolutePos2 = MAXLONG;
   zLONG  lMatch2 = MAXLONG;
   zSHORT nRC;

   GetViewByName( &vFlatCompare1, "__FlatCompare1", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare1, "__SystemCompare1", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare1, vCompare1,
                            szlHD_Diagram, szlZKey );

   GetViewByName( &vFlatCompare2, "__FlatCompare2", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare2, "__SystemCompare2", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare2, vCompare2, szlHD_Diagram, szlZKey );

   nRC = SetCursorFirstEntity( vFlatCompare1, szlHD_Node, 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare1, szlHD_Node, szlColor );
      if ( rgbCurrColor && rgbCurrColor != rgbColor )
      {
         GetIntegerFromAttribute( &lAbsolutePos1, vFlatCompare1, szlHD_Node, szlZKey );
         GetIntegerFromAttribute( &lMatch1, vFlatCompare1, szlHD_Node, "Match" );
         break;
      }

      nRC = SetCursorNextEntity( vFlatCompare1, szlHD_Node, 0 );
   }

   nRC = SetCursorFirstEntity( vFlatCompare2, szlHD_Node, 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare2, szlHD_Node, szlColor );
      if ( rgbCurrColor && rgbCurrColor != rgbColor )
      {
         GetIntegerFromAttribute( &lAbsolutePos2, vFlatCompare2, szlHD_Node, szlZKey );
         GetIntegerFromAttribute( &lMatch2, vFlatCompare2, szlHD_Node, "Match" );
         break;
      }

      nRC = SetCursorNextEntity( vFlatCompare2, szlHD_Node, 0 );
   }

   if ( lAbsolutePos1 == MAXLONG && lAbsolutePos2 == MAXLONG )
   {
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Views do not differ",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }
   else
   {
      if ( lAbsolutePos1 > lMatch2 )
      {
         lAbsolutePos1 = lMatch2;
         lMatch1 = lAbsolutePos2;
      }
      else
      if ( lAbsolutePos2 > lMatch1 )
      {
         lAbsolutePos2 = lMatch1;
         lMatch2 = lAbsolutePos1;
      }

      if ( lAbsolutePos1 == MAXLONG )
         SetCursorFirstEntity( vFlatCompare1, szlHD_Node, 0 );

      GetViewByName( &vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK );
      CompareSelectNode( vSubtask, vCompare1, vFlatCompare1, vHD_Compare1, "CompareHD1" );

      if ( lAbsolutePos2 == MAXLONG )
         SetCursorFirstEntity( vFlatCompare2, szlHD_Node, 0 );

      GetViewByName( &vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK );
      CompareSelectNode( vSubtask, vCompare2, vFlatCompare2, vHD_Compare2, "CompareHD2" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CompareNextDifference( zVIEW vSubtask )
{
   zCHAR  szTag[ 33 ];
   zVIEW  vCompare1;
   zVIEW  vCompare2;
   zVIEW  vFlatCompare1;
   zVIEW  vFlatCompare2;
   zVIEW  vHD_Compare1;
   zVIEW  vHD_Compare2;
   zLONG  rgbColor = RGB( 0, 255, 0 );
   zLONG  rgbCurrColor;
   zLONG  lAbsolutePos = MAXLONG;
   zLONG  lMatch = MAXLONG;
   zSHORT nRC;

   GetViewByName( &vFlatCompare1, "__FlatCompare1", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare1, "__SystemCompare1", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare1, vCompare1, szlHD_Diagram, szlZKey );

   GetViewByName( &vFlatCompare2, "__FlatCompare2", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare2, "__SystemCompare2", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare2, vCompare2, szlHD_Diagram, szlZKey );

   GetLastCtrlTag( vSubtask, szTag );
   if ( zstrchr( szTag, '1' ) )
   {
      nRC = SetCursorNextEntity( vFlatCompare1, szlHD_Node, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare1, szlHD_Node, szlColor );
         if ( rgbCurrColor && rgbCurrColor != rgbColor )
         {
            GetIntegerFromAttribute( &lAbsolutePos, vFlatCompare1, szlHD_Node, szlZKey );
            GetIntegerFromAttribute( &lMatch, vFlatCompare1, szlHD_Node, "Match" );
            GetViewByName( &vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK );
            CompareSelectNode( vSubtask, vCompare1, vFlatCompare1, vHD_Compare1, "CompareHD1" );
            if ( lMatch < MAXLONG )
            {
            // SetCursorAbsolutePosition( szEntity, vFlatCompare2, lMatch );
               SetCursorFirstEntityByInteger( vFlatCompare2, szlHD_Node, szlZKey, lMatch, 0 );
               GetViewByName( &vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK );
               CompareSelectNode( vSubtask, vCompare2, vFlatCompare2, vHD_Compare2, "CompareHD2" );
            }

            break;
         }

         nRC = SetCursorNextEntity( vFlatCompare1, szlHD_Node, 0 );
      }
   }
   else
   {
      nRC = SetCursorNextEntity( vFlatCompare2, szlHD_Node, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare2, szlHD_Node, szlColor );
         if ( rgbCurrColor && rgbCurrColor != rgbColor )
         {
            GetIntegerFromAttribute( &lAbsolutePos, vFlatCompare2, szlHD_Node, szlZKey );
            GetIntegerFromAttribute( &lMatch, vFlatCompare2, szlHD_Node, "Match" );
            GetViewByName( &vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK );
            CompareSelectNode( vSubtask, vCompare2, vFlatCompare2, vHD_Compare2, "CompareHD2" );
            if ( lMatch < MAXLONG )
            {
            // SetCursorAbsolutePosition( szEntity, vFlatCompare1, lMatch );
               SetCursorFirstEntityByInteger( vFlatCompare1, szlHD_Node, szlZKey, lMatch, 0 );
               GetViewByName( &vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK );
               CompareSelectNode( vSubtask, vCompare1, vFlatCompare1, vHD_Compare1, "CompareHD1" );
            }

            break;
         }

         nRC = SetCursorNextEntity( vFlatCompare2, szlHD_Node, 0 );
      }
   }

   if ( lAbsolutePos == MAXLONG )
   {
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Views do not differ beyond selected entity",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ComparePreviousDifference( zVIEW vSubtask )
{
   zCHAR  szTag[ 33 ];
   zVIEW  vCompare1;
   zVIEW  vCompare2;
   zVIEW  vFlatCompare1;
   zVIEW  vFlatCompare2;
   zVIEW  vHD_Compare1;
   zVIEW  vHD_Compare2;
   zLONG  rgbColor = RGB( 0, 255, 0 );
   zLONG  rgbCurrColor;
   zLONG  lAbsolutePos = -1;
   zLONG  lMatch = -1;
   zSHORT nRC;

   GetViewByName( &vFlatCompare1, "__FlatCompare1", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare1, "__SystemCompare1", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare1, vCompare1,
                            szlHD_Diagram, szlZKey );

   GetViewByName( &vFlatCompare2, "__FlatCompare2", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare2, "__SystemCompare2", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare2, vCompare2, szlHD_Diagram, szlZKey );

   GetLastCtrlTag( vSubtask, szTag );
   if ( zstrchr( szTag, '1' ) )
   {
      nRC = SetCursorPrevEntity( vFlatCompare1, szlHD_Node, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare1, szlHD_Node, szlColor );
         if ( rgbCurrColor && rgbCurrColor != rgbColor )
         {
            GetIntegerFromAttribute( &lAbsolutePos, vFlatCompare1, szlHD_Node, szlZKey );
            GetIntegerFromAttribute( &lMatch, vFlatCompare1, szlHD_Node, "Match" );
            GetViewByName( &vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK );
            CompareSelectNode( vSubtask, vCompare1, vFlatCompare1, vHD_Compare1, "CompareHD1" );
            if ( lMatch < MAXLONG )
            {
            // SetCursorAbsolutePosition( szEntity, vFlatCompare2, lMatch );
               SetCursorFirstEntityByInteger( vFlatCompare2, szlHD_Node, szlZKey, lMatch, 0 );
               GetViewByName( &vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK );
               CompareSelectNode( vSubtask, vCompare2, vFlatCompare2, vHD_Compare2, "CompareHD2" );
            }

            break;
         }

         nRC = SetCursorPrevEntity( vFlatCompare1, szlHD_Node, 0 );
      }
   }
   else
   {
      nRC = SetCursorPrevEntity( vFlatCompare2, szlHD_Node, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare2, szlHD_Node, szlColor );
         if ( rgbCurrColor && rgbCurrColor != rgbColor )
         {
            GetIntegerFromAttribute( &lAbsolutePos, vFlatCompare2, szlHD_Node, szlZKey );
            GetIntegerFromAttribute( &lMatch, vFlatCompare2, szlHD_Node, "Match" );
            GetViewByName( &vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK );
            CompareSelectNode( vSubtask, vCompare2, vFlatCompare2, vHD_Compare2, "CompareHD2" );
            if ( lMatch < MAXLONG )
            {
            // SetCursorAbsolutePosition( szEntity, vFlatCompare1, lMatch );
               SetCursorFirstEntityByInteger( vFlatCompare1, szlHD_Node, szlZKey, lMatch, 0 );
               GetViewByName( &vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK );
               CompareSelectNode( vSubtask, vCompare1, vFlatCompare1, vHD_Compare1, "CompareHD1" );
            }

            break;
         }

         nRC = SetCursorPrevEntity( vFlatCompare2, szlHD_Node, 0 );
      }
   }

   if ( lAbsolutePos == -1 )
   {
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Views do not differ prior to selected entity",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CompareLastDifference( zVIEW vSubtask )
{
   zVIEW  vCompare1;
   zVIEW  vCompare2;
   zVIEW  vFlatCompare1;
   zVIEW  vFlatCompare2;
   zVIEW  vHD_Compare1;
   zVIEW  vHD_Compare2;
   zLONG  rgbColor = RGB( 0, 255, 0 );
   zLONG  rgbCurrColor;
   zLONG  lAbsolutePos1 = -1;
   zLONG  lMatch1 = -1;
   zLONG  lAbsolutePos2 = -1;
   zLONG  lMatch2 = -1;
   zSHORT nRC;

   GetViewByName( &vFlatCompare1, "__FlatCompare1", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare1, "__SystemCompare1", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare1, vCompare1, szlHD_Diagram, szlZKey );

   GetViewByName( &vFlatCompare2, "__FlatCompare2", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompare2, "__SystemCompare2", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( (zPLONG) &vCompare2, vCompare2, szlHD_Diagram, szlZKey );

   nRC = SetCursorLastEntity( vFlatCompare1, szlHD_Node, 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare1, szlHD_Node, szlColor );
      if ( rgbCurrColor && rgbCurrColor != rgbColor )
      {
         GetIntegerFromAttribute( &lAbsolutePos1, vFlatCompare1, szlHD_Node, szlZKey );
         GetIntegerFromAttribute( &lMatch1, vFlatCompare1, szlHD_Node, "Match" );
         break;
      }

      nRC = SetCursorPrevEntity( vFlatCompare1, szlHD_Node, 0 );
   }

   nRC = SetCursorLastEntity( vFlatCompare2, szlHD_Node, 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &rgbCurrColor, vFlatCompare2, szlHD_Node, szlColor );
      if ( rgbCurrColor && rgbCurrColor != rgbColor )
      {
         GetIntegerFromAttribute( &lAbsolutePos2, vFlatCompare2, szlHD_Node, szlZKey );
         GetIntegerFromAttribute( &lMatch2, vFlatCompare2, szlHD_Node, "Match" );
         break;
      }

      nRC = SetCursorPrevEntity( vFlatCompare2, szlHD_Node, 0 );
   }

   if ( lAbsolutePos1 == -1 && lAbsolutePos2 == -1 )
   {
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Views do not differ",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
   }
   else
   {
      if ( lAbsolutePos1 < lMatch2 )
      {
         lAbsolutePos1 = lMatch2;
         lMatch1 = lAbsolutePos2;
      }
      else
      if ( lAbsolutePos2 < lMatch1 )
      {
         lAbsolutePos2 = lMatch1;
         lMatch2 = lAbsolutePos1;
      }

      if ( lAbsolutePos1 == -1 )
         SetCursorLastEntity( vFlatCompare1, szlHD_Node, 0 );

      GetViewByName( &vHD_Compare1, "__HD_Compare1", vSubtask, zLEVEL_TASK );
      CompareSelectNode( vSubtask, vCompare1, vFlatCompare1, vHD_Compare1, "CompareHD1" );

      if ( lAbsolutePos2 == -1 )
         SetCursorLastEntity( vFlatCompare2, szlHD_Node, 0 );

      GetViewByName( &vHD_Compare2, "__HD_Compare2", vSubtask, zLEVEL_TASK );
      CompareSelectNode( vSubtask, vCompare2, vFlatCompare2, vHD_Compare2, "CompareHD2" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CompareSetCursorCorresponding( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CompareSubobject( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CompareResetSubobject( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SwitchOrientation( zVIEW vSubtask )
{
   HD_SetProperties( vSubtask, "OI_Hier", 2 );  // switch orientation
   RefreshCtrl( vSubtask, "OI_Hier" );
   HD_SelectNode( vSubtask, "OI_Hier" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION   zwTZOBRWAD_HelpAbout
// PURPOSE:    This Entry does the About
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_HelpAbout( zVIEW vSubtask )
{

   IssueToolAbout( vSubtask, zTOOL_BROWSER, __FILE__, __DATE__, __TIME__ );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOBRWAD_HelpBasics
// PURPOSE:    This Entry transfers to basic help
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOBRWAD_HelpOnHelp
// PURPOSE:    This Entry transfers to HelpOnHelp
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZOBRWAD_HelpContents
// PURPOSE:    This Entry transfers to help Contents
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZOBRWAD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <limits.h>

#define mInitZero( s, n )  memset( (s), 0, (n) )

typedef struct zCHANGE_tag
{
  struct zCHANGE_tag *link;   // Previous or next edit command
  zLONG inserted;             // # lines of file 1 changed here.
  zLONG deleted;              // # lines of file 0 changed here.
  zLONG line0;                // Line number of 1st deleted line.
  zLONG line1;                // Line number of 1st inserted line.

} zCHANGE;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef struct zENTITY_tag
{
   zLONG  lAbsolutePos;
   zPCHAR pchEntity;

} zENTITY;

typedef struct zVIEW_tag
{
   zVIEW   vOI;         // view to object instance
   zCPCHAR cpcViewName; // view name
   zLONG   lEntityCnt;  // Number of entities

   // Buffer in which entity information is stored.
   zENTITY *pEntity;

   // Vector, indexed by absolute entity number, containing 1 for an entity
   // that is an insertion or a deletion.  Comparison results are stored here.
   zCHAR *changed_flag;

} zVIEW_DATA;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// a simpleton single-link list
class CSLink
{
public:
   CSLink( zCPCHAR cpcName );
   ~CSLink( );

   zPCHAR m_pchName;
   CSLink *m_pNext;
};

CSLink::CSLink( zCPCHAR cpcName )
{
   zSHORT nLth = (zSHORT) strlen( cpcName );
   m_pchName = new char[ nLth + 1 ];
   strcpy_s( m_pchName, nLth + 1, cpcName );
   m_pNext = 0;
}

CSLink::~CSLink( void )
{
   mDeleteInitA( m_pchName );
   mDeleteInit( m_pNext );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// The basic algorithm is described in:
// "An O(ND) Difference Algorithm and its Variations", Eugene Myers,
// Algorithmica Vol. 1 No. 2, 1986, pp. 251-266;
// see especially section 4.2, which describes the variation used below.
// This code uses the TOO_EXPENSIVE heuristic, by Paul Eggert, to limit
// the cost to O(N**1.5 log N) at the price of producing suboptimal output
// for large inputs with many differences.
//
// The basic algorithm was independently discovered as described in:
// "Algorithms for Approximate String Matching", E. Ukkonen,
// Information and Control Vol. 64, 1985, pp. 100-118.

// Describe the two views currently being compared.

CSLink *g_pEntityList;
zVIEW_DATA g_data[ 2 ];

zENTITY *g_plXV;        // Vectors being compared.
zENTITY *g_plYV;        // Vectors being compared.
zPLONG g_fdiag;         // Vector, indexed by diagonal, containing
                        // 1 + the X coordinate of the point furthest
                        // along the given diagonal in the forward
                        // search of the edit matrix.
zPLONG g_bdiag;         // Vector, indexed by diagonal, containing
                        // the X coordinate of the point furthest
                        // along the given diagonal in the backward
                        // search of the edit matrix.
zLONG g_lTooExpensive;  // Edit scripts longer than this are too
                        // expensive to compute.
zCPCHAR g_cpcViewName0;
zCPCHAR g_cpcViewName1;

// Next line number to be printed in the two input views.
zLONG g_lNext0, g_lNext1;


#define SNAKE_LIMIT 40  // Snakes bigger than this are considered `big'.

typedef struct zPARTITION_tag
{
  zLONG lMidX, lMidY;   // Midpoints of this partition.
  zLONG lMinimalLo;     // Nonzero if low half will be analyzed minimally.
  zLONG lMinimalHi;     // Likewise for high half.

} zPARTITION;

zLONG Diagonal( zVIEW_DATA *, zLONG, zLONG, zLONG, zLONG, zLONG,
                zPARTITION *, short );
zCHANGE *AddChange( zVIEW, zLONG, zLONG, zLONG, zLONG, zCHANGE * );
zCHANGE *BuildScript( zVIEW vSubtask, zVIEW_DATA const[ ] );
void CompareSeq( zVIEW_DATA *, zLONG, zLONG, zLONG, zLONG, zLONG );
void ShiftBoundaries( zVIEW_DATA[ ] );
zLONG ReadViews( zVIEW vSubtask, zVIEW_DATA viewvec[ ] );
void PrintSdiffScript( zVIEW vSubtask, zCHANGE *script );
zSHORT DiffViews( zVIEW vSubtask, zVIEW_DATA viewvec[ ] );
void Print1SdiffLine( zVIEW, zVIEW_DATA *,
                      zLONG, zCHAR *, zVIEW_DATA *, zLONG );
void PrintSdiffCommonLines( zVIEW, zLONG, zLONG );
void PrintSdiffHunk( zVIEW, zCHANGE * );

#ifdef __cplusplus
extern "C"
{
#endif

zSHORT
fnCompareEntities( zVIEW vOI1, zENTITY const *pEntity1,
                   zVIEW vOI2, zENTITY const *pEntity2 )
{
   if ( pEntity1->pchEntity == pEntity2->pchEntity )
   {
      SetCursorAbsolutePosition( pEntity1->pchEntity, vOI1,
                                 pEntity1->lAbsolutePos );
      SetCursorAbsolutePosition( pEntity2->pchEntity, vOI2,
                                 pEntity2->lAbsolutePos );
      if ( CompareEntitiesIgnoringAttrs( vOI1, pEntity1->pchEntity,
                                         vOI2, pEntity2->pchEntity,
                                         g_szIgnoreAttrList ) == 0 )
      {
         return( 0 );
      }

      return( 1 );
   }

   return( -1 );
}

// Value is:
//    0 - if views are the same
//    1 - if different
//    2 - if there is a problem opening them
//
zOPER_EXPORT zSHORT OPERATION
CompareObjects( zVIEW vSubtask )
{
   zVIEW_DATA inf[ 2 ];
   zSHORT nRC;

   mInitZero( inf, sizeof( inf ) );

   SetNameForView( vSubtask, "__SystemCompare", vSubtask, zLEVEL_TASK );
   inf[ 0 ].cpcViewName = "__Compare1";
   inf[ 1 ].cpcViewName = "__Compare2";

   if ( GetViewByName( &inf[ 0 ].vOI, inf[ 0 ].cpcViewName, vSubtask, zLEVEL_TASK ) <= 0 ||
        GetViewByName( &inf[ 1 ].vOI, inf[ 1 ].cpcViewName, vSubtask, zLEVEL_TASK ) <= 0 )
   {
      MessageSend( vSubtask, "CM00xxx", "Object Browser",
                   "Views have not been selected for comparison\n"
                   "Please select views to be compared!",
                   zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
      return( -1 );
   }

   CreateViewFromViewForTask( &inf[ 0 ].vOI, inf[ 0 ].vOI, vSubtask );
   CreateViewFromViewForTask( &inf[ 1 ].vOI, inf[ 1 ].vOI, vSubtask );

   // Compare the objects.
   nRC = DiffViews( vSubtask, inf );

   // Now the comparison has been done, if no error prevented it,
   // and nRC is the value this function will return.
   if ( nRC == 0 )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ), "Object Instances: %s and %s are identical",
                 inf[ 0 ].cpcViewName, inf[ 1 ].cpcViewName );
      TraceLineS( szMsg, "" );
   }
   else
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ), "Object Instances: %s and %s are different",
                 inf[ 0 ].cpcViewName, inf[ 1 ].cpcViewName );
      TraceLineS( szMsg, "" );
   }

   DropView( inf[ 0 ].vOI );
   DropView( inf[ 1 ].vOI );

   return( nRC );
}

#ifdef __cplusplus
}
#endif

// Find the midpoint of the shortest edit script for a specified
// portion of the two views.
//
// Scan from the beginnings of the views, and simultaneously from the ends,
// doing a breadth-first search through the space of edit-sequence.
// When the two searches meet, we have found the midpoint of the shortest
// edit sequence.
//
// If MINIMAL is nonzero, find the minimal edit script regardless
// of expense.  Otherwise, if the search is too expensive, use
// heuristics to stop the search and report a suboptimal answer.
//
// Set PART->(XMID, YMID) to the midpoint (XMID, YMID).  The diagonal number
// XMID - YMID equals the number of inserted lines minus the number
// of deleted lines (counting only lines before the midpoint).
// Return the approximate edit cost; this is the total number of
// lines inserted or deleted (counting only lines before the midpoint),
// unless a heuristic is used to terminate the search prematurely.
//
// Set PART->LEFT_MINIMAL to nonzero iff the minimal edit script for the
// left half of the partition is known; similarly for PART->RIGHT_MINIMAL.
//
// This function assumes that the first lines of the specified portions
// of the two views do not match, and likewise that the last lines do not
// match.  The caller must trim matching lines from the beginning and end
// of the portions it is going to specify.
//
// If we return the "wrong" partitions, the worst that can happen is
// suboptimal diff output.  It cannot cause incorrect diff output.

zLONG
Diagonal( zVIEW_DATA viewvec[ ],
          zLONG xoff, zLONG xlim, zLONG yoff, zLONG ylim,
          zLONG lMinimal, zPARTITION *part, short bHeuristic )
{
   zPLONG const fd = g_fdiag;        // Give the compiler a chance.
   zPLONG const bd = g_bdiag;        // Additional help for the compiler.
   zENTITY const * const xv = g_plXV;   // Still more help for the compiler.
   zENTITY const * const yv = g_plYV;   // And more and more . . .
   zLONG const dmin = xoff - ylim; // Minimum valid diagonal.
   zLONG const dmax = xlim - yoff; // Maximum valid diagonal.
   zLONG const fmid = xoff - yoff; // Center diagonal of top-down search.
   zLONG const bmid = xlim - ylim; // Center diagonal of bottom-up search.
   zLONG fmin = fmid, fmax = fmid; // Limits of top-down search.
   zLONG bmin = bmid, bmax = bmid; // Limits of bottom-up search.
   zLONG lCost;                    // Cost.
   zLONG odd = (fmid - bmid) & 1;  // True if southeast corner is on an odd
                                  // diagonal with respect to the northwest.

   fd[ fmid ] = xoff;
   bd[ bmid ] = xlim;

   for ( lCost = 1;; lCost++ )
   {
      zLONG d;                    // Active diagonal.
      zLONG big_snake = 0;

      // Extend the top-down search by an edit step in each diagonal.
      fmin > dmin ? fd[ --fmin - 1 ] = -1 : ++fmin;
      fmax < dmax ? fd[ ++fmax + 1 ] = -1 : --fmax;
      for ( d = fmax; d >= fmin; d -= 2 )
      {
         zLONG x, y, oldx, lLo = fd[ d - 1 ], lHi = fd[ d + 1 ];

         if ( lLo >= lHi )
            x = lLo + 1;
         else
            x = lHi;

         oldx = x;
         y = x - d;
         while ( x < xlim && y < ylim &&
                 fnCompareEntities( viewvec[ 0 ].vOI, &xv[ x ], viewvec[ 1 ].vOI, &yv[ y ] ) == 0 )
         {
            x++;
            y++;
         }

         if ( x - oldx > SNAKE_LIMIT )
            big_snake = 1;

         fd[ d ] = x;
         if ( odd && bmin <= d && d <= bmax && bd[ d ] <= x )
         {
            part->lMidX = x;
            part->lMidY = y;
            part->lMinimalLo = part->lMinimalHi = 1;
            return( 2 * lCost - 1 );
         }
      }

      // Similarly extend the bottom-up search.
      bmin > dmin ? bd[ --bmin - 1 ] = INT_MAX : ++bmin;
      bmax < dmax ? bd[ ++bmax + 1 ] = INT_MAX : --bmax;
      for ( d = bmax; d >= bmin; d -= 2 )
      {
         zLONG x, y, oldx, lLo = bd[ d - 1 ], lHi = bd[ d + 1 ];

         if ( lLo < lHi )
            x = lLo;
         else
            x = lHi - 1;

         oldx = x;
         y = x - d;
         while ( x > xoff && y > yoff &&
                 fnCompareEntities( viewvec[ 0 ].vOI, &xv[ x - 1 ], viewvec[ 1 ].vOI, &yv[ y - 1 ] ) == 0 )
         {
            x--;
            y--;
         }

         if ( oldx - x > SNAKE_LIMIT )
            big_snake = 1;

         bd[ d ] = x;
         if ( odd == 0 && fmin <= d && d <= fmax && x <= fd[ d ] )
         {
            part->lMidX = x;
            part->lMidY = y;
            part->lMinimalLo = part->lMinimalHi = 1;
            return( 2 * lCost );
         }
      }

      if ( lMinimal )
         continue;

      // Heuristic: check occasionally for a diagonal that has made
      // lots of progress compared with the edit distance.
      // If we have any such, find the one that has made the most
      // progress and return it as if it had succeeded.
      //
      // With this heuristic, for views with a constant small density
      // of changes, the algorithm is linear in the file size.

      if ( lCost > 200 && big_snake && bHeuristic )
      {
         zLONG best = 0;
         for ( d = fmax; d >= fmin; d -= 2 )
         {
            zLONG dd = d - fmid;
            zLONG x = fd[ d ];
            zLONG y = x - d;
            zLONG v = (x - xoff) * 2 - dd;
            if ( v > 12 * (lCost + (dd < 0 ? -dd : dd )))
            {
               if ( v > best &&
                    xoff + SNAKE_LIMIT <= x && x < xlim &&
                    yoff + SNAKE_LIMIT <= y && y < ylim )
               {
                  // We have a good enough best diagonal;
                  // now insist that it end with a significant snake.
                  zLONG k;

                  for ( k = 1;
                        fnCompareEntities( viewvec[ 0 ].vOI, &xv[ x - k ],
                                           viewvec[ 1 ].vOI, &yv[ y - k ] ) == 0;
                        k++ )
                  {
                     if ( k == SNAKE_LIMIT )
                     {
                         best = v;
                         part->lMidX = x;
                         part->lMidY = y;
                         break;
                     }
                  }
               }
            }
         }

         if ( best > 0 )
         {
            part->lMinimalLo = 1;
            part->lMinimalHi = 0;
            return( 2 * lCost - 1 );
         }

         best = 0;
         for ( d = bmax; d >= bmin; d -= 2 )
         {
            zLONG dd = d - bmid;
            zLONG x = bd[ d ];
            zLONG y = x - d;
            zLONG v = (xlim - x) * 2 + dd;
            if ( v > 12 * (lCost + (dd < 0 ? -dd : dd )))
            {
               if ( v > best &&
                    xoff < x && x <= xlim - SNAKE_LIMIT &&
                    yoff < y && y <= ylim - SNAKE_LIMIT )
               {
                  // We have a good enough best diagonal;
                  // now insist that it end with a significant snake.
                  zLONG k;

                  for ( k = 0;
                        fnCompareEntities( viewvec[ 0 ].vOI, &xv[ x + k ],
                                           viewvec[ 1 ].vOI, &yv[ y + k ] ) == 0;
                        k++ )
                  {
                     if ( k == SNAKE_LIMIT - 1 )
                     {
                         best = v;
                         part->lMidX = x;
                         part->lMidY = y;
                         break;
                     }
                  }
               }
            }
         }

         if ( best > 0 )
         {
            part->lMinimalLo = 0;
            part->lMinimalHi = 1;
            return( 2 * lCost - 1 );
         }
      }

      // Heuristic: if we've gone well beyond the call of duty,
      // give up and report halfway between our best results so far.
      if ( lCost >= g_lTooExpensive )
      {
         zLONG fxybest, fxbest;
         zLONG bxybest, bxbest;

         fxbest = bxbest = 0;  // initialize

         // Find forward diagonal that maximizes X + Y.
         fxybest = -1;
         for ( d = fmax; d >= fmin; d -= 2 )
         {
            zLONG x = min( fd[ d ], xlim );
            zLONG y = x - d;
            if ( ylim < y )
               x = ylim + d, y = ylim;

            if ( fxybest < x + y )
            {
               fxybest = x + y;
               fxbest = x;
            }
         }

         // Find backward diagonal that minimizes X + Y.
         bxybest = INT_MAX;
         for ( d = bmax; d >= bmin; d -= 2 )
         {
            zLONG x = max( xoff, bd[ d ] );
            zLONG y = x - d;
            if ( y < yoff )
               x = yoff + d, y = yoff;

            if ( x + y < bxybest )
            {
                bxybest = x + y;
                bxbest = x;
            }
         }

         // Use the better of the two diagonals.
         if ( (xlim + ylim) - bxybest < fxybest - (xoff + yoff) )
         {
            part->lMidX = fxbest;
            part->lMidY = fxybest - fxbest;
            part->lMinimalLo = 1;
            part->lMinimalHi = 0;
         }
         else
         {
            part->lMidX = bxbest;
            part->lMidY = bxybest - bxbest;
            part->lMinimalLo = 0;
            part->lMinimalHi = 1;
         }

         return( 2 * lCost - 1 );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// Compare in detail contiguous subsequences of the two views
// which are known, as a whole, to match each other.
//
// The results are recorded in the vectors views[ N ].changed_flag, by
// storing a 1 in the element for each line that is an insertion or deletion.
//
// The subsequence of file 0 is [ XOFF, XLIM ) and likewise for file 1.
//
// Note that XLIM, YLIM are exclusive bounds.
// All line numbers are origin-0 and discarded lines are not counted.
//
// If MINIMAL is nonzero, find a minimal difference no matter how
// expensive it is.
/////////////////////////////////////////////////////////////////////////////
void
CompareSeq( zVIEW_DATA viewvec[ ],
            zLONG xoff, zLONG xlim, zLONG yoff, zLONG ylim, zLONG lMinimal )
{
   zENTITY * const xv = g_plXV; // Help the compiler.
   zENTITY * const yv = g_plYV;

   // Slide down the bottom initial diagonal.
   while ( xoff < xlim && yoff < ylim &&
           fnCompareEntities( viewvec[ 0 ].vOI, &xv[ xoff ],
                              viewvec[ 1 ].vOI, &yv[ yoff ] ) == 0 )
   {
      xoff++, yoff++;
   }

   // Slide up the top initial diagonal.
   while ( xlim > xoff && ylim > yoff &&
           fnCompareEntities( viewvec[ 0 ].vOI, &xv[ xlim - 1 ],
                              viewvec[ 1 ].vOI, &yv[ ylim ] - 1 ) == 0 )
   {
      xlim--, ylim--;
   }

   // Handle simple cases.
   if ( xoff == xlim )
   {
      while ( yoff < ylim )
         g_data[ 1 ].changed_flag[ yoff++ ] = 1;
   }
   else
   if ( yoff == ylim )
   {
      while ( xoff < xlim )
         g_data[ 0 ].changed_flag[ xoff++ ] = 1;
   }
   else
   {
      zLONG lCost;
      zPARTITION part;

      // Find a point of correspondence in the middle of the views.

      lCost = Diagonal( viewvec, xoff, xlim, yoff, ylim, lMinimal, &part, 1 );

      if ( lCost == 1 )
      {
         // This should be impossible, because it implies that
         // one of the two subsequences is empty,
         // and that case was handled above without calling `diag'.
         // Let's verify that this is true.
         abort( );
      }
      else
      {
         // Use the partitions to split this problem into subproblems.
         CompareSeq( viewvec, xoff, part.lMidX, yoff, part.lMidY, part.lMinimalLo );
         CompareSeq( viewvec, part.lMidX, xlim, part.lMidY, ylim, part.lMinimalHi );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// Cons an additional entry onto the front of an edit script OLD.
// LINE0 and LINE1 are the first affected lines in the two views (origin 0).
// DELETED is the number of lines deleted here from file 0.
// INSERTED is the number of lines inserted here in file 1.
//
// If DELETED is 0 then LINE0 is the number of the line before
// which the insertion was done; vice versa for INSERTED and LINE1.
/////////////////////////////////////////////////////////////////////////////
zCHANGE *
AddChange( zVIEW vSubtask, zLONG line0, zLONG line1,
           zLONG deleted, zLONG inserted,
           zCHANGE *old )
{
   zCHANGE *pNew;

   DrAllocTaskMemory( (zCOREMEM) &pNew, sizeof( zCHANGE ), 1 );
   pNew->line0 = line0;
   pNew->line1 = line1;
   pNew->inserted = inserted;
   pNew->deleted = deleted;
   pNew->link = old;
   return( pNew );
}

/////////////////////////////////////////////////////////////////////////////
// Scan the tables of which lines are inserted and deleted,
// producing an edit script in forward order.
/////////////////////////////////////////////////////////////////////////////
zCHANGE *
BuildScript( zVIEW vSubtask, zVIEW_DATA const viewvec[ ] )
{
   zCHANGE *script = 0;
   zCHAR *changed0 = viewvec[ 0 ].changed_flag;
   zCHAR *changed1 = viewvec[ 1 ].changed_flag;
   zLONG i0 = viewvec[ 0 ].lEntityCnt;
   zLONG i1 = viewvec[ 1 ].lEntityCnt;

   // Note that changedN[ -1 ] does exist, and contains 0.

   while ( i0 >= 0 || i1 >= 0 )
   {
       if ( changed0[ i0 - 1 ] || changed1[ i1 - 1 ] )
       {
           zLONG line0 = i0, line1 = i1;

           // Find # lines changed here in each file.
           while ( changed0[ i0 - 1 ] )
              i0--;

           while ( changed1[ i1 - 1 ] )
              i1--;

           // Record this change.
           script = AddChange( vSubtask, i0, i1, line0 - i0, line1 - i1, script );
       }

       // We have reached lines in the two views that match each other.
       i0--, i1--;
   }

   return( script );
}

zCHANGE *
FindChange( zVIEW vSubtask, zCHANGE *start )
{
   return( start );
}

void
PrintScript( zVIEW vSubtask, zCHANGE *script,
             zCHANGE * (*hunkfn)( zVIEW, zCHANGE * ),
             void (*printfn)( zVIEW, zCHANGE * ) )
{
   zCHANGE *next = script;

   while ( next )
   {
      zCHANGE *curr, *end;

      // Find a set of changes that belong together.
      curr = next;
      end = (*hunkfn)( vSubtask, next );

      // Disconnect them from the rest of the changes,
      // making them a hunk, and remember the rest for next iteration.
      next = end->link;
      end->link = 0;
#if 0 // def DEBUG
      debug_script( curr );
#endif

      // Print current hunk.
      (*printfn)( vSubtask, curr );

      // Reconnect the script so it will all be freed properly.
      end->link = next;
   }
}

// Look at a hunk of edit script and report the range of lines in each file
// that it applies to.  HUNK is the start of the hunk, which is a chain
// of `zCHANGE'.  The first and last line numbers of file 0 are stored
// in *FIRST0 and *LAST0, and likewise for file 1 in *FIRST1 and *LAST1.
// Note that these are internal line numbers that count from 0.
//
// If no lines from file 0 are deleted, then FIRST0 is LAST0 + 1.
//
// Also set *DELETES nonzero if any lines of file 0 are deleted
// and set *INSERTS nonzero if any lines of file 1 are inserted.
// If only ignorable lines are inserted or deleted, both are
// set to 0.

void
AnalyzeHunk( zCHANGE *hunk,
             zPLONG first0,
             zPLONG last0,
             zPLONG first1,
             zPLONG last1,
             zPLONG plDeletes,
             zPLONG plInserts )
{
   zLONG l0, l1, show_from, show_to;
   zCHANGE *next;

   show_from = show_to = 0;

   *first0 = hunk->line0;
   *first1 = hunk->line1;

   next = hunk;
   do
   {
      l0 = next->line0 + next->deleted - 1;
      l1 = next->line1 + next->inserted - 1;
      show_from += next->deleted;
      show_to += next->inserted;

   } while ( (next = next->link) != 0 );

   *last0 = l0;
   *last1 = l1;

   // If all inserted or deleted lines are ignorable,
   // tell the caller to ignore this hunk.

   *plDeletes = show_from;
   *plInserts = show_to;
}

void
PrintSdiffScript( zVIEW vSubtask, zCHANGE *script )
{
   g_lNext0 = g_lNext1 = 0;
   PrintScript( vSubtask, script, FindChange, PrintSdiffHunk );
   PrintSdiffCommonLines( vSubtask, 0, 0 );
}

#if 0
zSHORT
FindHD_Entity( zVIEW vHD_Compare, zLONG lAbsolutePos )
{
   if ( CheckExistenceOfEntity( vHD_Compare, szlHD_Node ) == 0 )
   {
      zSHORT nRC = SetCursorFirstEntityByInteger( vHD_Compare, szlHD_Node, szlZKey, lAbsolutePos, 0 );
      if ( nRC >= zCURSOR_SET )
         return( nRC );

      nRC = SetCursorFirstEntity( vHD_Compare, szlHD_Node, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         SetViewToSubobject( vHD_Compare, szlHD_ChildNode );
         if ( (nRC = FindHD_Entity( vHD_Compare, lAbsolutePos )) >= zCURSOR_SET )
         {
            return( nRC );
         }

         ResetViewFromSubobject( vHD_Compare );
         nRC = SetCursorNextEntity( vHD_Compare, szlHD_Node, 0 );
      }
   }

   return( zCALL_ERROR );
}
#endif

zSHORT
SetHD_EntityColor( zVIEW   vSubtask,
                   zCPCHAR cpcViewName,
                   zCPCHAR cpcEntityName,
                   zLONG   lAbsolutePos,
                   zLONG   rgbColor,
                   zLONG   lMatch )
{
   zVIEW  vFlatCompare;
   zSHORT nRC = zCALL_ERROR;

   if ( GetViewByName( &vSubtask, "__SystemCompare", vSubtask, zLEVEL_TASK ) > 0 &&
        GetViewByName( &vFlatCompare, cpcViewName, vSubtask, zLEVEL_TASK ) > 0 )
   {
      nRC = SetCursorFirstEntityByInteger( vFlatCompare, szlHD_Node, szlZKey, lAbsolutePos, 0 );
      if ( nRC >= zCURSOR_SET )
      {
         SetAttributeFromString( vFlatCompare, szlHD_Node, szlDIL, cpcEntityName );
         SetAttributeFromInteger( vFlatCompare, szlHD_Node, szlColor, rgbColor );
         SetAttributeFromInteger( vFlatCompare, szlHD_Node, "Match", lMatch );
      }
      else
         TraceLineI( "SetHD_EntityColor did not find AbsolutePos: ", lAbsolutePos );
   }

   return( nRC );
}

//
// Print side by side lines with a separator in the middle.
// Null parameters are taken to indicate white space text.
// Blank lines that can easily be caught are reduced to a single newline.
//
void
Print1SdiffLine( zVIEW vSubtask, zVIEW_DATA * f1, zLONG offset1,
                 zCHAR *pchSep, zVIEW_DATA * f2, zLONG offset2 )
{
   zENTITY *pEntity1 = 0;
   zENTITY *pEntity2 = 0;
   zLONG   rgbColor = RGB( 255, 0, 0 );
   zBOOL   bMatch = FALSE;

// if ( pchSep[ 0 ] == '<' )
//    TraceLineS( "Separator ", "<" );

   if ( f1 && f2 )
   {
      pEntity1 = &(f1->pEntity[ offset1 ]);
      pEntity2 = &(f2->pEntity[ offset2 ]);
      if ( zstrcmp( pEntity1->pchEntity, pEntity2->pchEntity ) == 0 )
      {
         bMatch = TRUE;
         if ( pchSep[ 0 ] == ' ' )
            rgbColor = RGB( 0, 255, 0 );
         else
            rgbColor = RGB( 0, 255, 255 );
      }
      else
         rgbColor = RGB( 255, 255, 255 );
   }
   else
   {
      rgbColor = RGB( 255, 255, 255 );
#if 0
      if ( f1 )
      {
         pEntity1 = &(f1->pEntity[ offset1 ]);
         TraceLineI( "Entity added at AbsolutePos: ", pEntity1->lAbsolutePos );
         SetCursorAbsolutePosition( pEntity1->pchEntity, f1->vOI, pEntity1->lAbsolutePos );
         DisplayEntityInstance( f1->vOI, pEntity1->pchEntity );
      }
      else
      if ( f2 )
      {
         pEntity2 = &(f2->pEntity[ offset2 ]);
         TraceLineI( "Entity removed at AbsolutePos: ", pEntity2->lAbsolutePos );
         SetCursorAbsolutePosition( pEntity2->pchEntity, f2->vOI, pEntity2->lAbsolutePos );
         DisplayEntityInstance( f2->vOI, pEntity2->pchEntity );
      }
#endif
   }

   if ( f1 )
   {
      pEntity1 = &(f1->pEntity[ offset1 ]);
      SetHD_EntityColor( vSubtask, "__FlatCompare1", pEntity1->pchEntity,
                         pEntity1->lAbsolutePos, rgbColor,
                         (bMatch && pEntity2) ? pEntity2->lAbsolutePos : MAXLONG );
   // SetCursorAbsolutePosition( pEntity1->pchEntity, f1->vOI, pEntity1->lAbsolutePos );
   // DisplayEntityInstance( f1->vOI, pEntity1->pchEntity );
   }

///if ( pchSep[ 0 ] != ' ' )
// {
//    TraceLineS( "", "" );
//    TraceLineS( "Separator ====> ", pchSep );
// }

   if ( f2 )
   {
      pEntity2 = &(f2->pEntity[ offset2 ]);
      SetHD_EntityColor( vSubtask, "__FlatCompare2", pEntity2->pchEntity,
                         pEntity2->lAbsolutePos, rgbColor,
                         (bMatch && pEntity1) ? pEntity1->lAbsolutePos : MAXLONG );
   // SetCursorAbsolutePosition( pEntity2->pchEntity, f2->vOI, pEntity2->lAbsolutePos );
   // DisplayEntityInstance( f2->vOI, pEntity2->pchEntity );
   }

 //TraceLineS( "End Print1SdiffLine ******", "" );
}

// Print lines common to both views in side-by-side format.
void
PrintSdiffCommonLines( zVIEW vSubtask, zLONG limit0, zLONG limit1 )
{
   int i0 = g_lNext0, i1 = g_lNext1;

   if ( i0 != limit0 || i1 != limit1 )
   {
      while ( i0 != limit0 && i0 < g_data[ 0 ].lEntityCnt &&
              i1 != limit1 && i1 < g_data[ 1 ].lEntityCnt )
      {
         Print1SdiffLine( vSubtask, &g_data[ 0 ], i0, " ", &g_data[ 1 ], i1 );
         i0++; i1++;
      }

      while ( i1 != limit1 && i1 < g_data[ 1 ].lEntityCnt )
      {
         Print1SdiffLine( vSubtask, 0, 0, " )", &g_data[ 1 ], i1 );
         i1++;
      }

      while ( i0 != limit0 && i0 < g_data[ 0 ].lEntityCnt )
      {
          Print1SdiffLine( vSubtask, &g_data[ 0 ], i0, "( ", 0, 0 );
          i0++;
      }
   }

   g_lNext0 = limit0;
   g_lNext1 = limit1;
}

// Print a hunk of an sdiff diff.
// This is a contiguous portion of a complete edit script,
// describing changes in consecutive lines.
void
PrintSdiffHunk( zVIEW vSubtask, zCHANGE *hunk )
{
   zLONG first0, last0, first1, last1, lDeletes, lInserts;
   zLONG k, j;

   // Determine range of line numbers involved in each file.
   AnalyzeHunk( hunk, &first0, &last0, &first1, &last1, &lDeletes, &lInserts );
   if ( lDeletes == 0 && lInserts == 0 )
      return;

   // Print out lines up to this change.
   PrintSdiffCommonLines( vSubtask, first0, first1 );

   // Print ``xxx  |  xxx '' lines
   if ( lInserts && lDeletes )
   {
      for ( k = first0, j = first1; k <= last0 && j <= last1; ++k, ++j )
         Print1SdiffLine( vSubtask, &g_data[ 0 ], k, "|", &g_data[ 1 ], j );

      lDeletes = k <= last0;
      lInserts = j <= last1;
      g_lNext0 = first0 = k;
      g_lNext1 = first1 = j;
   }

   // Print ``     >  xxx '' lines
   if ( lInserts )
   {
      for ( j = first1; j <= last1; ++j )
         Print1SdiffLine( vSubtask, 0, 0, ">", &g_data[ 1 ], j );

      g_lNext1 = j;
   }

   // Print ``xxx  <     '' lines */
   if ( lDeletes )
   {
      for ( k = first0; k <= last0; ++k )
         Print1SdiffLine( vSubtask, &g_data[ 0 ], k, "<", 0, 0 );

      g_lNext0 = k;
   }
}

/////////////////////////////////////////////////////////////////////////////
// Report the differences of two views.
/////////////////////////////////////////////////////////////////////////////
zSHORT
DiffViews( zVIEW vSubtask, zVIEW_DATA viewvec[ ] )
{
   zCHAR szChange[ 2 ];
   zLONG lDiagCnt;
   zLONG k;
   zCHANGE *e, *p;
   zCHANGE *script;

   ReadViews( vSubtask, viewvec );

   // Allocate vectors for the results of comparison:  a flag for each line
   // of each file, saying whether that line is an insertion or deletion.
   // Allocate an extra element, always zero, at each end of each vector.

   zLONG lLth = viewvec[ 0 ].lEntityCnt + viewvec[ 1 ].lEntityCnt + 4;
   DrAllocTaskMemory( &(viewvec[ 0 ].changed_flag), lLth, 2 );
   mInitZero( viewvec[ 0 ].changed_flag, lLth );
   viewvec[ 0 ].changed_flag++;
   viewvec[ 1 ].changed_flag = viewvec[ 0 ].changed_flag + viewvec[ 0 ].lEntityCnt + 2;

   // Now do the main comparison algorithm.

   g_plXV = viewvec[ 0 ].pEntity;
   g_plYV = viewvec[ 1 ].pEntity;
   lDiagCnt = viewvec[ 0 ].lEntityCnt +
              viewvec[ 1 ].lEntityCnt + 3;
   DrAllocTaskMemory( (zCOREMEM) &g_fdiag, lDiagCnt * (2 * sizeof( zLONG )), 3 );
   g_bdiag = g_fdiag + lDiagCnt;
   g_fdiag += viewvec[ 1 ].lEntityCnt + 1;
   g_bdiag += viewvec[ 1 ].lEntityCnt + 1;

   // Set TOO_EXPENSIVE to be approximate square root of input size,
   // bounded below by 256.
   g_lTooExpensive = 1;
   for ( k = viewvec[ 0 ].lEntityCnt + viewvec[ 1 ].lEntityCnt;
         k != 0;
         k >>= 2 )
   {
      g_lTooExpensive <<= 1;
   }

   g_lTooExpensive = max( 256, g_lTooExpensive );

   g_data[ 0 ] = viewvec[ 0 ];
   g_data[ 1 ] = viewvec[ 1 ];

   CompareSeq( viewvec,
               0, viewvec[ 0 ].lEntityCnt,
               0, viewvec[ 1 ].lEntityCnt, 0 );

   // Get the results of comparison in the form of a chain
   // of `zCHANGE's -- an edit script.

   zSHORT nChanged;

   script = BuildScript( vSubtask, viewvec );
   if ( script )
   {
      nChanged = 1;

      // Record info for starting up output,
      // to be used if and when we have some output to print.
      // setup_output( g_data[ 0 ].cpcViewName, g_data[ 1 ].cpcViewName );
      g_cpcViewName0 = g_data[ 0 ].cpcViewName;
      g_cpcViewName1 = g_data[ 1 ].cpcViewName;

      PrintSdiffScript( vSubtask, script );
   }
   else
   {
      // Everything matches, so just run through and set everything as a
      // match.
      g_lNext0 = g_lNext1 = 0;
      PrintSdiffCommonLines( vSubtask, g_data[ 0 ].lEntityCnt, g_data[ 1 ].lEntityCnt );
   }

   szChange[ 1 ] = 0;

#if 0
   zCHAR szMsg[ 256 ];
   TraceLineS( "Changed flags for vector 1", "" );
   for ( k = 0; k < viewvec[ 0 ].lEntityCnt; k++ )
   {
      szChange[ 0 ] = viewvec[ 0 ].changed_flag[ k ];
      sprintf( szMsg, "%d - %s", k + 1, szChange );
      TraceLineS( szMsg, "" );
   }

   TraceLineS( "Changed flags for vector 2", "" );
   for ( k = 0; k < viewvec[ 1 ].lEntityCnt; k++ )
   {
      szChange[ 0 ] = viewvec[ 1 ].changed_flag[ k + viewvec[ 0 ].lEntityCnt + 2 ];
      sprintf( szMsg, "%d - %s", k + 1, szChange );
      TraceLineS( szMsg, "" );
   }
#endif

   if ( g_fdiag )
   {
      g_fdiag -= viewvec[ 1 ].lEntityCnt + 1;
      DrFreeTaskMemory( g_fdiag );
      g_fdiag = 0;
   }

   viewvec[ 0 ].changed_flag--;  // point to start of original allocation
   DrFreeTaskMemory( viewvec[ 0 ].changed_flag );

   for ( e = script; e; e = p )
   {
      p = e->link;
      DrFreeTaskMemory( e );
   }

   if ( viewvec[ 0 ].pEntity != viewvec[ 1 ].pEntity )
      DrFreeTaskMemory( viewvec[ 0 ].pEntity );

   DrFreeTaskMemory( viewvec[ 1 ].pEntity );
   return( nChanged );
}

// Given a vector of two zVIEW_DATA objects, build the table views.
zLONG
ReadViews( zVIEW vSubtask, zVIEW_DATA viewvec[ ] )
{
   CSLink  *pPrev;
   CSLink  *pCurr;
   zVIEW   vHierOI;
   zUSHORT uReturnLevel;
   zCHAR   szRootEntityName[ 33 ];
   zCHAR   szEntityName[ 33 ];
   zLONG   lEntityCnt;
   zLONG   lAbsolutePos;
   zLONG   k;

   for ( k = 0; k < 2; k++ )
   {
      // Get the current view completely into memory.
      vHierOI = viewvec[ k ].vOI;
      ResetView( vHierOI );
      MiGetRootEntityForView( vHierOI, szRootEntityName );
      ResetView( vHierOI );
      DefineHierarchicalCursor( vHierOI, szRootEntityName );
      lEntityCnt = 0;
      strcpy_s( szEntityName, szRootEntityName );

      do
      {
         lEntityCnt++;
         pCurr = g_pEntityList;
         pPrev = 0;
         while ( pCurr )
         {
            if ( zstrcmp( pCurr->m_pchName, szEntityName ) == 0 )
               break;

            pPrev = pCurr;
            pCurr = pCurr->m_pNext;
         }

         if ( pCurr == 0 )
         {
            pCurr = new CSLink( szEntityName );
            if ( pPrev )
               pPrev->m_pNext = pCurr;
            else
               g_pEntityList = pCurr;
         }

      } while ( SetCursorNextEntityHierarchical( &uReturnLevel, szEntityName, vHierOI ) >= zCURSOR_SET );

      viewvec[ k ].lEntityCnt = lEntityCnt;
      DropHierarchicalCursor( vHierOI );

      // Allocate enough room for appended newline and sentinel.
      zLONG lBufSize = (viewvec[ k ].lEntityCnt + 2) * sizeof( zENTITY );
      DrAllocTaskMemory( (zCOREMEM) &(viewvec[ k ].pEntity), lBufSize, 4 );

      ResetView( vHierOI );
      DefineHierarchicalCursor( vHierOI, szRootEntityName );
      lEntityCnt = 0;
      strcpy_s( szEntityName, szRootEntityName );

      do
      {
         pCurr = g_pEntityList;
         while ( pCurr )
         {
            if ( zstrcmp( pCurr->m_pchName, szEntityName ) == 0 )
               break;

            pCurr = pCurr->m_pNext;
         }

         if ( pCurr == 0 )
         {
            zVIEW vSystemCompare;

            GetViewByName( &vSystemCompare, "__SystemCompare", vSubtask, zLEVEL_TASK );
            MessageSend( vSubtask, "CM00xxx", szEntityName,
                         "Unexpected error locating entity",
                         zMSGQ_OBJECT_CONSTRAINT_INFORMATION, zBEEP );
            return( -1 );
         }

         GetAbsolutePositionForEntity( &lAbsolutePos, vHierOI, szEntityName );
      // TraceLineI( "DisplayEntityInstance for AbsolutePos: ", lAbsolutePos );
      // DisplayEntityInstance( vHierOI, szEntityName );

         viewvec[ k ].pEntity[ lEntityCnt ].lAbsolutePos = lAbsolutePos;
         viewvec[ k ].pEntity[ lEntityCnt ].pchEntity = pCurr->m_pchName;
         lEntityCnt++;

      } while ( SetCursorNextEntityHierarchical( &uReturnLevel, szEntityName, vHierOI ) >= zCURSOR_SET );

      DropHierarchicalCursor( vHierOI );
      ResetView( vHierOI );
   }

   return( 0 );
}

#ifdef __cplusplus
}
#endif

///////////////////////////// end of file  //////////////////////////////////
