/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdlga.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of "C interface" routines into the
// C++ functionality for actions available through the Zeidon driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.06.08    DKS    Debug
//    Added debugging code to solve Action bug for TMV.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2000.03.20    DKS    Z2000   QS999
//    EnableAction has changed to prevent override of code specified
//    enable/disable.  This may impact current applications, but it
//    is not expected to do so.
//
// 1999.07.29    DKS    Z2000   QS999
//    New function GetActionName.
//
// 1998.09.17    DKS
//    Fix for ASSERT for TerminateActionForError (TB 165)
//
// 1998.08.13    DKS
//    Trace the tag of the menu item that could not be located.
//
// 1998.06.30    RG
//    New function GetActionParameters from DKS
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ACTION 12

//./ ADD NAME=TerminateActionForError
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TerminateActionForError
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To signal that an error has occurred and to prevent further
//             action processing.  The user must present the error.
//
//  PARAMETERS: vSubtask    - The subtask view.
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
TerminateActionForError( zVIEW vSubtask )
{
#ifdef DEBUG_ALL
   TraceLineX( "TerminateActionForError vSubtask: ", (zLONG) vSubtask );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZAction *pAction = &(pZSubtask->m_Action);
//    while ( pAction->m_pNextAction )
//       pAction = pAction->m_pNextAction;

      pAction->m_lActType = zWAB_TerminateActionForError;
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=SwitchToAction
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SwitchToAction
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To signal that a different action is to be processed.  The
//             current action process is terminated.
//
//  PARAMETERS: vSubtask     - The subtask view.
//              cpcNewAction - The action to process.
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
SwitchToAction( zVIEW   vSubtask,
                zCPCHAR cpcNewAction )
{
#ifdef DEBUG_ALL
   TraceLineS( "SwitchToAction new action: ", cpcNewAction );
#endif

   ZSubtask *pZSubtask;

   if ( cpcNewAction &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZAction *pAction = &(pZSubtask->m_Action);
//    while ( pAction->m_pNextAction )
//       pAction = pAction->m_pNextAction;

      pAction->m_lActType = zWAB_SwitchToAction;
      strcpy_s( pAction->m_szAction, sizeof( pAction->m_szAction ), cpcNewAction );
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=InvokeAction
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: InvokeAction
//                                              06/22/93 Modified: 06/22/93
//
//  PURPOSE:   To process the specified action immediately.
//
//  PARAMETERS: vSubtask     - The subtask view.
//              cpcActionTag - The action to process.
//
//  RETURNS:     0  - Action performed
//              -1  - Error locating window
//              -2  - Action not located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
InvokeAction( zVIEW   vSubtask,
              zCPCHAR cpcActionTag )
{
#ifdef DEBUG_ALL
   TraceLineS( "InvokeAction action: ", cpcActionTag );
#endif

   ZSubtask *pZSubtask;

   if ( cpcActionTag && cpcActionTag[ 0 ] && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
//    ZAction *pAction = &(pZSubtask->m_Action);
//    while ( pAction->m_pNextAction )
//       pAction = pAction->m_pNextAction;
//
//    pAction->m_pNextAction = new ZAction( 0, cpcActionTag );
//    pAction = pAction->m_pNextAction;

      zCHAR szAction[ zTAG_LTH ];
      strcpy_s( szAction, sizeof( szAction ), cpcActionTag ); // localize action
      return( (zSHORT) ProcessAction( pZSubtask, szAction, 0, 0, "InvokeAction" ) );
   }

   return( -1 );
}

//./ ADD NAME=GetWebRedirection
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWebRedirection
//                                              06/22/93 Modified: 06/22/93
//
//  PURPOSE:   To get the current Web KZXMLPGO "next window".
//
//  PARAMETERS: vSubtask    - The subtask view for the current window
//              nWindowActionBehavior - set the action behavior
//              cpcDlgTag - Dialog name containing new window
//              cpcWndTag - Window name to which to transfer
//
//  RETURNS:     0  - Action performed
//              -1  - Error locating window
//              -2  - Action not located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetWebRedirection( zVIEW   vSubtask,
                   zLONG   lWindowBehavior,
                   zPCHAR  pchDlgTag,
                   zPCHAR  pchWndTag )
{
   zVIEW vKZXMLPGO;
   zCHAR szFunctionCall[ 64 ];
   zCHAR szDlgWnd[ 256 ];
   zSHORT nRC = 0;

   GetViewByName( &vKZXMLPGO, "KZXMLPGO", vSubtask, zLEVEL_TASK );
// TraceLineS( "", "" );
   TraceLine( "GetWebRedirection KZXMLPGO %s   Dlg: %s   Wnd: %s",
              GetActionString( lWindowBehavior ), pchDlgTag ? pchDlgTag : "", pchWndTag ? pchWndTag : "" );
// DisplayObjectInstance( vKZXMLPGO, 0, 0 );
   if ( vKZXMLPGO )
   {
      GetStringFromAttribute( pchDlgTag, zTAG_LTH, vKZXMLPGO, "NextDialogWindow", "DialogName" );
      GetStringFromAttribute( pchWndTag, zTAG_LTH, vKZXMLPGO, "NextDialogWindow", "WindowName" );
      GetStringFromAttribute( szFunctionCall, sizeof( szFunctionCall ), vKZXMLPGO, "NextDialogWindow", "FunctionCall" );

      DeleteEntity( vKZXMLPGO, "NextDialogWindow", zREPOS_LAST );

      if ( zstrcmp( szFunctionCall, "StartSubwindow" ) == 0 )
      {
         strcpy_s( szDlgWnd, sizeof( szDlgWnd ), pchDlgTag );
         strcat_s( szDlgWnd, sizeof( szDlgWnd ), pchWndTag );
         CreateEntity( vKZXMLPGO, "PagePath", zPOS_AFTER );
         SetAttributeFromString( vKZXMLPGO, "PagePath", "LastPageName", szDlgWnd );
         nRC = 1;
      }
   }

// TraceLineS( "", "" );
// TraceLineS( "GetWebRedirection KZXMLPGO (post)", "" );
   TraceLine( "GetWebRedirection KZXMLPGO (post) %s   Dlg: %s   Wnd: %s",
              GetActionString( lWindowBehavior ), pchDlgTag ? pchDlgTag : "", pchWndTag ? pchWndTag : "" );
// DisplayObjectInstance( vKZXMLPGO, 0, 0 );

   return( nRC );
}

//./ ADD NAME=SetWebRedirection
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetWebRedirection
//                                              06/22/93 Modified: 06/22/93
//
//  PURPOSE:   To set up the Web KZXMLPGO entities.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window
//              nWindowActionBehavior - set the action behavior
//              cpcDlgTag - Dialog name containing new window
//              cpcWndTag - Window name to which to transfer
//
//  RETURNS:     0  - Action performed
//              -1  - Error locating window
//              -2  - Action not located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
SetWebRedirection( zVIEW   vSubtask,
                   zLONG   lWindowBehavior,
                   zCPCHAR cpcDlgTag,
                   zCPCHAR cpcWndTag )
{
   zVIEW  vKZXMLPGO;
   zSHORT nRC;

// TraceLineS( "", "" );
   TraceLine( "SetWebRedirection KZXMLPGO %s   Dlg: %s   Wnd: %s",
              GetActionString( lWindowBehavior ), cpcDlgTag ? cpcDlgTag : "", cpcWndTag ? cpcWndTag : "" );
// DisplayObjectInstance( vKZXMLPGO, 0, 0 );
   if ( lWindowBehavior == zWAB_StayOnWindow )
   {
      // nothing to do
   }
   else
   {
      GetViewByName( &vKZXMLPGO, "KZXMLPGO", vSubtask, zLEVEL_TASK );
      if ( vKZXMLPGO )
      {
         while ( SetCursorFirstEntity( vKZXMLPGO, "NextDialogWindow", 0 ) == 0 )
            DeleteEntity( vKZXMLPGO, "NextDialogWindow", zREPOS_NONE );

         CreateEntity( vKZXMLPGO, "NextDialogWindow", zPOS_FIRST );
         if ( cpcDlgTag && cpcDlgTag[ 0 ] && cpcWndTag && cpcWndTag[ 0 ] )
         {
            SetAttributeFromString( vKZXMLPGO, "NextDialogWindow",
                                    "DialogName", cpcDlgTag );
            SetAttributeFromString( vKZXMLPGO, "NextDialogWindow",
                                    "WindowName", cpcWndTag );
         }

         if ( lWindowBehavior == zWAB_StayOnWindowWithRefresh )
         {
            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "DialogName",
                                       vKZXMLPGO, "DialogWindowList", "DialogName" );
            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "WindowName",
                                       vKZXMLPGO, "DialogWindowList", "WindowName" );
            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "FunctionCall",
                                       vKZXMLPGO, "DialogWindowList", "FunctionCall" );
         }
         else
         if ( lWindowBehavior == zWAB_StartTopWindow ||
              lWindowBehavior == zWAB_ResetTopWindow )
         {
            zBOOL bCreated = FALSE;

            while ( SetCursorFirstEntity( vKZXMLPGO, "DialogWindowList", 0 ) == 0 )
               DeleteEntity( vKZXMLPGO, "DialogWindowList", zREPOS_NONE );

            CreateEntity( vKZXMLPGO, "DialogWindowList", zPOS_FIRST );
            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "DialogName", cpcDlgTag );
            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "WindowName", cpcWndTag );

            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "FunctionCall", "StartTopSubwindow" );
            SetAttributeFromString( vKZXMLPGO, "NextDialogWindow",
                                    "FunctionCall", "StartTopSubwindow" );

            if ( CheckExistenceOfEntity( vKZXMLPGO, "TopDialogWindow" ) != 0 )
            {
               bCreated = TRUE;
               CreateEntity( vKZXMLPGO, "TopDialogWindow", zPOS_FIRST );
            }

            if ( bCreated || lWindowBehavior == zWAB_ResetTopWindow )
            {
               SetAttributeFromString( vKZXMLPGO, "TopDialogWindow",
                                       "DialogName", cpcDlgTag );
               SetAttributeFromString( vKZXMLPGO, "TopDialogWindow",
                                       "WindowName", cpcWndTag );
               SetAttributeFromString( vKZXMLPGO, "TopDialogWindow",
                                       "FunctionCall", "StartTopSubwindow" );
            }
         }
         else
         if ( lWindowBehavior == zWAB_StartModalSubwindow ||
              lWindowBehavior == zWAB_StartModelessSubwindow )
         {
            CreateEntity( vKZXMLPGO, "DialogWindowList", zPOS_LAST );
            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "DialogName", cpcDlgTag );
            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "WindowName", cpcWndTag );

            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "FunctionCall", "StartSubwindow" );
            SetAttributeFromString( vKZXMLPGO, "NextDialogWindow",
                                    "FunctionCall", "StartSubwindow" );
         }
         else
         if ( lWindowBehavior == zWAB_ReplaceWindowWithModalWindow ||
              lWindowBehavior == zWAB_ReplaceWindowWithModelessWindow )
         {
            if ( SetCursorLastEntity( vKZXMLPGO, "DialogWindowList", 0 ) == 0 )
               DeleteEntity( vKZXMLPGO, "DialogWindowList", zREPOS_LAST );

            CreateEntity( vKZXMLPGO, "DialogWindowList", zPOS_LAST );
            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "DialogName", cpcDlgTag );
            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "WindowName", cpcWndTag );

            SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                    "FunctionCall", "ReplaceWindow" );
            SetAttributeFromString( vKZXMLPGO, "NextDialogWindow",
                                    "FunctionCall", "ReplaceWindow" );
         }
         else
         if ( lWindowBehavior == zWAB_ReturnToParentWithRefresh ||
              lWindowBehavior == zWAB_ReturnToParent )
         {
            // If a "return to" window is specified, look for it in the list.  If it is found,
            // delete all following entities.  If the "return to" window is not located, just
            // do a normal return to the previous window.
            if ( cpcDlgTag[ 0 ] && cpcWndTag[ 0 ] )
            {
               nRC = SetCursorFirstEntityByString( vKZXMLPGO, "DialogWindowList", "DialogName",
                                                   cpcDlgTag, 0  );
               while ( nRC == zCURSOR_SET )
               {
                  if ( CompareAttributeToString( vKZXMLPGO, "DialogWindowList",
                                                 "WindowName", cpcWndTag ) == 0 )
                  {
                     while ( SetCursorNextEntity( vKZXMLPGO, "DialogWindowList", 0 ) == zCURSOR_SET )
                        DeleteEntity( vKZXMLPGO, "DialogWindowList", zREPOS_NONE );

                     break;  // we found the right one
                  }

                  nRC = SetCursorNextEntityByString( vKZXMLPGO, "DialogWindowList", "DialogName",
                                                     cpcDlgTag, 0  );
               }

               // We won't delete all remaining entities ... just do a normal return.
            // nRC = SetCursorFirstEntity( vKZXMLPGO, "DialogWindowList", 0 );
            // while ( nRC == zCURSOR_SET )
            // {
            //    DeleteEntity( vKZXMLPGO, "DialogWindowList", zREPOS_NONE );
            //    nRC = SetCursorNextEntity( vKZXMLPGO, "DialogWindowList", 0 );
            // }
            }
            else
               nRC = zCURSOR_UNDEFINED;

            if ( nRC != zCURSOR_SET &&
                 SetCursorLastEntity( vKZXMLPGO, "DialogWindowList", 0 ) == 0 )
            {
               DeleteEntity( vKZXMLPGO, "DialogWindowList", zREPOS_LAST );
            }

            if ( CheckExistenceOfEntity( vKZXMLPGO, "DialogWindowList" ) != 0 )
            {
               CreateEntity( vKZXMLPGO, "DialogWindowList", zPOS_LAST );
               SetAttributeFromAttribute( vKZXMLPGO, "DialogWindowList", "DialogName",
                                          vKZXMLPGO, "TopDialogWindow", "DialogName" );
               SetAttributeFromAttribute( vKZXMLPGO, "DialogWindowList", "WindowName",
                                          vKZXMLPGO, "TopDialogWindow", "WindowName" );
               SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                       "FunctionCall", "ReturnToParent" );
            }

            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "DialogName",
                                       vKZXMLPGO, "DialogWindowList", "DialogName" );
            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "WindowName",
                                       vKZXMLPGO, "DialogWindowList", "WindowName" );
            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "FunctionCall",
                                       vKZXMLPGO, "DialogWindowList", "FunctionCall" );
         }
         else
         if ( lWindowBehavior == zWAB_ReturnToTopWindow )
         {
            if ( SetCursorFirstEntity( vKZXMLPGO, "DialogWindowList", 0 ) == 0 )
            {
               while ( SetCursorNextEntity( vKZXMLPGO, "DialogWindowList", 0 ) == 0 )
                  DeleteEntity( vKZXMLPGO, "DialogWindowList", zREPOS_NONE );

               SetCursorFirstEntity( vKZXMLPGO, "DialogWindowList", 0 );
            }

            if ( CheckExistenceOfEntity( vKZXMLPGO, "DialogWindowList" ) != 0 )
            {
               CreateEntity( vKZXMLPGO, "DialogWindowList", zPOS_LAST );
               SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                       "DialogName", cpcDlgTag );
               SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                       "WindowName", cpcWndTag );
               SetAttributeFromString( vKZXMLPGO, "DialogWindowList",
                                       "FunctionCall", "ReturnToTopWindow" );
            }

            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "DialogName",
                                       vKZXMLPGO, "DialogWindowList", "DialogName" );
            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "WindowName",
                                       vKZXMLPGO, "DialogWindowList", "WindowName" );
            SetAttributeFromAttribute( vKZXMLPGO, "NextDialogWindow", "FunctionCall",
                                       vKZXMLPGO, "DialogWindowList", "FunctionCall" );
         }
         else
         {
            SetAttributeFromString( vKZXMLPGO, "NextDialogWindow",
                                    "FunctionCall", "???????" );
         }

      }
   }

// TraceLineS( "", "" );
// TraceLineS( "SetWebRedirection KZXMLPGO (post)", "" );
// DisplayObjectInstance( vKZXMLPGO, 0, 0 );
   return( 0 );
}

//./ ADD NAME=SetWindowActionBehavior
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetWindowActionBehavior
//                                              04/01/93 Modified: 04/14/93
//
//  PURPOSE:    To override the default control flow action defined for
//              a window to a new WINDOW. This operation can be used to
//              change the default flow of control.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window
//              nWindowActionBehavior - this overrides the automatic
//                                      (default) action behavior specified
//                                      on the currently executing action..
//                 valid values are:
//
//                 zWAB_SwitchToAction                   -1
//                 zWAB_DeterminedInDialogOperation       0
//
//                 zWAB_StayOnWindow                      1(PREVENT ACTION)
//                 zWAB_StayOnWindowWithRefresh           2
//
//                 zWAB_StartOrFocusModelessDialog       30
//                 zWAB_StartDialogTask                  31
//                 zWAB_StartModelessWindow              32
//                 zWAB_StartOrFocusModelessWindow       38
//                 zWAB_StartModalWindow                 36
//                 zWAB_StartModelessSubwindow           zTAG_LTH
//                 zWAB_StartOrFocusModelessSubwindow    39
//                 zWAB_StartModalSubwindow              34
//                 zWAB_StartOrFocusModelessApp          37
//                 zWAB_StartNewApp                      35
//
//                 zWAB_ReplaceDialogWithDialog          51
//                 zWAB_ReplaceWindowWithModelessWindow  52
//                 zWAB_ReplaceWindowWithModalWindow     53
//                 zWAB_ReplaceCurrentAppWithApp         54
//
//                 zWAB_ReturnToParent                   71
//                 zWAB_ReturnToParentWithRefresh        72
//                 zWAB_ReturnToTopWindow                73
//                 zWAB_ReturnToTopWindowWithRefresh     74
//                 zWAB_StartTopWindow                   75
//                 zWAB_ResetTopWindow                 76
//
//                 zWAB_ExitDialogTask                   99
//
//                 zWAB_SuspendToDebugger               201
//                 zWAB_ResumeFromDebugger              202
//
//                 zWAB_TerminateActionForError        9999
//                 zWAB_ProcessImmediateAction   0x10000000L
//                 zWAB_NoDriverMessageOnError   0x20000000L
//                 zWAB_ProcessImmediateReturn   0x40000000L
//
//
//              cpcDlgTag - Dialog name containing new window
//              cpcWndTag - Window name to which to transfer
//
//  RETURNS:    0 - Call successful
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
SetWindowActionBehavior( zVIEW   vSubtask,
                         zLONG   lWindowBehavior,
                         zCPCHAR cpcDlgTag,
                         zCPCHAR cpcWndTag )
{
#ifdef DEBUG_ALL
   TraceLineX( "SetWindowActionBehavior: ", lWindowBehavior );
   if ( cpcDlgTag )
      TraceLineS( "                DlgName: ", cpcDlgTag );
   if ( cpcWndTag )
      TraceLineS( "                WndName: ", cpcWndTag );
#endif

   if ( vSubtask )
   {
      ZSubtask *pZSubtask;
      LPTASK lpTask = (LPTASK) zGETPTR( vSubtask->hTask );

      if ( lpTask && lpTask->vWebSubtask )
      {
#if 0
         zCHAR szDlgTag[ zTAG_LTH ];
         zCHAR szWndTag[ zTAG_LTH ];

         if ( cpcDlgTag && cpcDlgTag[ 0 ] )
            strcpy_s( szDlgTag, sizeof( szDlgTag ), cpcDlgTag );
         else
            szDlgTag[ 0 ] = 0;

         if ( cpcWndTag && cpcWndTag[ 0 ] )
            strcpy_s( szWndTag, sizeof( szWndTag ), cpcWndTag );
         else
            szWndTag[ 0 ] = 0;
#endif
         SetWebRedirection( vSubtask, lWindowBehavior, cpcDlgTag, cpcWndTag );
         return( 0 );
      }
      else
      if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
      {
         ZAction *pAction = &(pZSubtask->m_Action);
         pAction->m_lActType = lWindowBehavior & ~zWAB_ProcessImmediateAction;
         ASSERT( ((pAction->m_lActType & ~zWAB_ProcessImmediateReturn) >= 0 &&
                  (pAction->m_lActType & ~zWAB_ProcessImmediateReturn) < 100) ||
                 (pAction->m_lActType & ~zWAB_ProcessImmediateReturn) == zWAB_TerminateActionForError );
         if ( cpcDlgTag )
            strcpy_s( pAction->m_szActDlg, sizeof( pAction->m_szActDlg ), cpcDlgTag );
         else
            pAction->m_szActDlg[ 0 ] = 0;

         if ( cpcWndTag )
            strcpy_s( pAction->m_szActWnd, sizeof( pAction->m_szActWnd ), cpcWndTag );
         else
            pAction->m_szActWnd[ 0 ] = 0;

         if ( (lWindowBehavior & zWAB_ProcessImmediateAction) == zWAB_ProcessImmediateAction )
         {
            ProcessAction( pZSubtask, 0, 0, 0, "Immediate" );
            PumpMessages( vSubtask );
         }

         return( 0 );
      }
      else
      {
         ZFindDialog fd;

         fd.pvDialog = SysGetHandleFromPointer( vSubtask );
         fd.DlgN[ 0 ] = 0;
         fd.WndN[ 0 ] = 0;
         if ( FindGlobalDialogWindow( &fd ) && mIs_hWnd( fd.hWnd ) )
         {
            zPVOID pv;

            if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &pv, sizeof( ZFindDialog ) ) == 0 )
            {
               ZFindDialog *pfd = (ZFindDialog *) SfLockTaskMemory( pv );

               if ( cpcDlgTag && *cpcDlgTag )
                  strcpy_s( pfd->DlgN, sizeof( pfd->DlgN ), cpcDlgTag );
               else
                  pfd->DlgN[ 0 ] = 0;

               if ( cpcWndTag && *cpcWndTag )
                  strcpy_s( pfd->WndN, sizeof( pfd->WndN ), cpcWndTag );
               else
                  pfd->WndN[ 0 ] = 0;

               // Borrow lTaskId to carry behavior.
               pfd->lTID = lWindowBehavior;

               zSHORT nRC = (zSHORT) ::SendMessage( fd.hWnd, zZEIDON_SET_WAB, 0, (LPARAM) pv );

               pfd = (ZFindDialog *) SfUnlockTaskMemory( pfd );
               SfFreeTaskMemory( pv );
               return( nRC );
            }
         }

         TraceLineX( "SetWindowActionBehavior secondary search could not "
                     "locate Subtask: ", (zLONG) vSubtask );
      }
   }

   return( -1 );
}

//./ ADD NAME=InvokeWindowActionBehavior
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: InvokeWindowActionBehavior
//                                              04/01/93 Modified: 04/14/93
//
//  PURPOSE:    To invoke a window action behavior immediately.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window
//              nWindowActionBehavior - window action behavior to invoke ...
//                 valid values are:
//
//                 zWAB_SwitchToAction                   -1
//                 zWAB_DeterminedInDialogOperation       0
//
//                 zWAB_StayOnWindow                      1(PREVENT ACTION)
//                 zWAB_StayOnWindowWithRefresh           2
//
//                 zWAB_StartOrFocusModelessDialog       30
//                 zWAB_StartDialogTask                  31
//                 zWAB_StartModelessWindow              32
//                 zWAB_StartOrFocusModelessWindow       38
//                 zWAB_StartModalWindow                 36
//                 zWAB_StartModelessSubwindow           zTAG_LTH
//                 zWAB_StartOrFocusModelessSubwindow    39
//                 zWAB_StartModalSubwindow              34
//                 zWAB_StartOrFocusModelessApp          37
//                 zWAB_StartNewApp                      35
//
//                 zWAB_StartPopupMenu                   40
//
//                 zWAB_ReplaceDialogWithDialog          51
//                 zWAB_ReplaceWindowWithModelessWindow  52
//                 zWAB_ReplaceWindowWithModalWindow     53
//                 zWAB_ReplaceCurrentAppWithApp         54
//
//                 zWAB_ReturnToParent                   71
//                 zWAB_ReturnToParentWithRefresh        72
//                 zWAB_ReturnToTopWindow                73
//                 zWAB_ReturnToTopWindowWithRefresh     74
//                 zWAB_StartTopWindow                   75
//                 zWAB_ResetTopWindow                 76
//
//                 zWAB_ExitDialogTask                   99
//
//                 zWAB_SuspendToDebugger               201
//                 zWAB_ResumeFromDebugger              202
//
//                 zWAB_TerminateActionForError        9999
//                 zWAB_ProcessImmediateAction   0x10000000L
//                 zWAB_NoDriverMessageOnError   0x20000000L
//                 zWAB_ProcessImmediateReturn   0x40000000L
//
//
//              cpcDlgTag - Dialog name containing new window
//              cpcWndTag - Window name to which to transfer
//
//  RETURNS:    0 - Call successful
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
InvokeWindowActionBehavior( zVIEW   vSubtask,
                            zLONG   lWindowBehavior,
                            zCPCHAR cpcDlgTag,
                            zCPCHAR cpcWndTag )
{
#ifdef DEBUG_ALL
   TraceLineX( "InvokeWindowActionBehavior: ", lWindowBehavior );
   if ( cpcDlgTag )
      TraceLineS( "                DlgName: ", cpcDlgTag );
   if ( cpcWndTag )
      TraceLineS( "                WndName: ", cpcWndTag );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZAction *pAction = &(pZSubtask->m_Action);
      pAction->m_lActType = lWindowBehavior & ~zWAB_ProcessImmediateAction;
      ASSERT( ((pAction->m_lActType & ~zWAB_ProcessImmediateReturn) >= 0 &&
               (pAction->m_lActType & ~zWAB_ProcessImmediateReturn) < 100) ||
              (pAction->m_lActType & ~zWAB_ProcessImmediateReturn) == zWAB_TerminateActionForError );
      if ( cpcDlgTag )
         strcpy_s( pAction->m_szActDlg, sizeof( pAction->m_szActDlg ), cpcDlgTag );
      else
         pAction->m_szActDlg[ 0 ] = 0;

      if ( cpcWndTag )
         strcpy_s( pAction->m_szActWnd, sizeof( pAction->m_szActWnd ), cpcWndTag );
      else
         pAction->m_szActWnd[ 0 ] = 0;

      ProcessAction( pZSubtask, 0, 0, 0, "Immediate" );

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=EnableAction
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: EnableAction
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:    To enable/disable an action (and its referenced controls)
//
//  PARAMETERS: vSubtask      - the subtask view for the window containing
//                              the specified action
//              cpcActionName - action to be enabled/disabled
//              nEnable       - TRUE ==> enable ... FALSE ==> disable
//
//  RETURNS:    0 - Call successful
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
EnableAction( zVIEW   vSubtask,
              zCPCHAR cpcActionName,
              zSHORT  nEnable )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( cpcActionName, "GOTO_UpdateTermOfResidency" ) == 0 )
   {
      zCHAR szMsg[ 256 ];
   // int k = 0;

   // k /= k;
      sprintf_s( szMsg, sizeof( szMsg ), "EnableAction: %s Enable=%s", cpcActionName,
                (nEnable == FALSE) ? "FALSE" : (nEnable == TRUE) ? "TRUE" :
                (nEnable == zAUTO_DISABLE) ? "AUTO_DISABLE" :
                (nEnable == zAUTO_ENABLE) ? "AUTO_ENABLE" : "Enable ???" );
      TraceLineS( szMsg, "" );
   }
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pchEnabledAction &&
        SetEntityCursor( vSubtask, szlAct, szlTag,
                         zPOS_FIRST | zQUAL_STRING,
                         cpcActionName, 0, 0, 0, 0, 0 ) >= 0 )
   {
      zULONG  ulIdx;
      ZMapAct *pzma;
      ZOption *pZOption;
      zPCHAR  pch;
      zSHORT  nPrevEnable;
      zSHORT  nRC;

      if ( pZSubtask->m_pZBinPopup )
      {
         mDeleteInit( pZSubtask->m_pZBinPopup );  // note: all ZOptions are
         mDeleteInit( pZSubtask->m_pTrackMenu );  // deleted as well
         mDeleteInit( pZSubtask->m_pzsPopupTag );
      }

      // Get the sync index for this action.
      GetIntegerFromAttribute( (zPLONG) &ulIdx, vSubtask, szlAct, "SyncIdx" );
#ifdef DEBUG_ACTION
      if ( ulIdx == DEBUG_ACTION )
      {
         TraceBlob( "EnableAction pre-set:",
                    pZSubtask->m_pchEnabledAction + sizeof( zSHORT ),
                    *((zSHORT *) pZSubtask->m_pchEnabledAction) );
      }
#endif
      nPrevEnable =
           pZSubtask->m_pchEnabledAction[ (zSHORT) (ulIdx + sizeof( zSHORT )) ];

      // Honor all USER enable/disable requests ... or if the the Action
      // has previously been AUTO enabled/disabled ... permit the request.
      if ( nEnable == FALSE || nEnable == TRUE ||
           nPrevEnable == zAUTO_DISABLE || nPrevEnable == zAUTO_ENABLE )
      {
         pZSubtask->m_pchEnabledAction[ (zSHORT) (ulIdx +
                                        sizeof( zSHORT )) ] = (zCHAR) nEnable;

         if ( nEnable == zAUTO_DISABLE )
            nEnable = FALSE;
         else
         if ( nEnable == zAUTO_ENABLE )
            nEnable = TRUE;

         if ( pZSubtask->m_pZMIXCtrl )
         {
            nRC = SetEntityCursor( vSubtask, szlActCtrl, 0, zPOS_FIRST,
                                   0, 0, 0, 0, szlAct, 0 );
            while ( nRC > zCURSOR_UNCHANGED )
            {
               GetIntegerFromAttribute( (zPLONG) &ulIdx, vSubtask, szlActCtrl, szlIdNbr );
               pzma = GetSafeCtrlNode( pZSubtask->m_pZMIXCtrl, (zUSHORT) ulIdx );
               if ( pzma )
               {
                  pzma->SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, nEnable );
                  if ( (nEnable && (nPrevEnable == zAUTO_DISABLE || nPrevEnable == FALSE)) ||
                       (nEnable == FALSE && (nPrevEnable == zAUTO_ENABLE || nPrevEnable)) )
                  {
                     if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
                        pzma->m_pCtrl->Invalidate( TRUE );
                  }
               }

               nRC = SetEntityCursor( vSubtask, szlActCtrl, 0, zPOS_NEXT,
                                      0, 0, 0, 0, szlAct, 0 );
            }
         }

         if ( pZSubtask->m_pZBinMenu )
         {
            CString csSeek;
            zLONG lEnable = MF_BYCOMMAND | (nEnable ? MF_ENABLED : MF_GRAYED);

            nRC = SetEntityCursor( vSubtask, szlActOpt, 0,
                                   zPOS_FIRST, 0, 0, 0, 0, szlAct, 0 );
            while ( nRC > zCURSOR_UNCHANGED )
            {
               GetAddrForAttribute( (zPCHAR *) &pch, vSubtask,
                                    szlActOpt, szlTag );
               // assume existence
               csSeek = pch;
               pZOption = (ZOption *)
                                 pZSubtask->m_pZBinMenu->SeekNode( &csSeek );

               // pZOption may have come back zero if the option is in a popup
               // and not on the action bar.  If so, do not try and disable it
               // because it does not exist!
               if ( pZOption )
               {
                  pZOption->m_bEnabled = (zBOOL) nEnable;
//                TraceLineS( "EnableAction EnableMenuItem: ",
//                            *(pZOption->m_pzsTag) );
                  nRC = pZSubtask->m_pMenu->EnableMenuItem( pZOption->m_uOptionID,
                                                            lEnable );

                  if ( nRC == -1 )
                  {
                     TraceLineS( "(drvr) Enable Menu Item failed ",
                                 *(pZOption->m_pzsTag) );
                     TraceLineS( "     for Action: ", cpcActionName );
                  }
               }

               nRC = SetEntityCursor( vSubtask, szlActOpt, 0,
                                      zPOS_NEXT, 0, 0, 0, 0, szlAct, 0 );
            }
         }
      }

#ifdef DEBUG_ACTION
      if ( ulIdx == DEBUG_ACTION )
      {
         TraceBlob( "EnableAction post-set:",
                    pZSubtask->m_pchEnabledAction + sizeof( zSHORT ),
                    *((zSHORT *) pZSubtask->m_pchEnabledAction) );
      }
#endif
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=StartTask
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: StartTask
//                                              04/01/93 Modified: 11/29/96
//
//  PURPOSE:   To start a new task (independent of the current task).
//             See WinExec for details of parameters and return code.
//
//  PARAMETERS: vSubtask    - Current view to window
//              cpcCmdLine  - Command line
//              uCmdShow    - specifies how the application window is
//                            to be shown.
//
//  RETURNS:    See doc for WinExec in SDK or DosStartSession for OS/2
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zUSHORT OPERATION
StartTask( zVIEW   vSubtask,
           zCPCHAR cpcCmdLine,
           zUSHORT uCmdShow )
{

// We should have system independent values for uCmdShow. Something like
//   zSTART_SHOWNORMAL, zSTART_SHOWMAXIMIZED, ...

#ifdef zREMOTE_SERVER

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zCHAR  szTempString[ 128 ];
      zPCHAR pch;

      strcpy_s( szTempString, sizeof( szTempString ), cpcCmdLine );
      zstrupper( szTempString );
      pch = zstrstr( szTempString, ".EXE" );
      if ( pch )
      {
         pch += 4;
      }
      else
      {
         pch = szTempString;
         while ( *pch == ' ' )
            pch++;

         while ( *pch && zisspace( *pch ) == FALSE )
            pch++;
      }

      while ( *pch == ' ' )
         pch++;

      // pch should now be pointing at the delimiter.
      if ( *pch && *pch == *(pch + 1) )
      {
         zPCHAR pchApp;
         zPCHAR pchDlg;

         pchApp = pch + 2;
         pchDlg = zstrchr( pchApp, *pch );
         if ( pchDlg )
         {
            zVIEW  vNewSubtask;

            strcpy_s( szTempString, sizeof( szTempString ), cpcCmdLine );
            *pchDlg = 0;
            pchDlg++;

            SfCreateSubtask( &vNewSubtask, 0, pchApp );

            // Build a new window.
            pZSubtask->m_pZTask->NewZWnd( 0,                // Zeidon parent
                                          pZSubtask,        // Zeidon owner
                                          pZSubtask->m_vDialog,
                                          vNewSubtask,      // new subtask
                                          FALSE,            // modeless
                                          pchDlg,   // Dialog to process
                                          0,        // Window to create
                                          0,        // Window to replace
                                          pZSubtask->m_pZSocketTask );

#if 0
            zCHAR  szStartTask[ 512 ];
            zCHAR  szNameExt[ 256 ];
            zVIEW  vXRA;
            zSHORT nLth;

            SysReadZeidonIni( -1, "[Workstation]", "StartTask", szStartTask );
            nLth = zstrlen( szStartTask );
            szStartTask[ nLth++ ] = ' ';
            szStartTask[ nLth++ ] = '-';
            szStartTask[ nLth++ ] = 'a';
            strcpy_s( szStartTask + nLth, sizeof( szStartTask ) - nLth, pchApp );
            nLth += zstrlen( szStartTask + nLth );
            szStartTask[ nLth++ ] = ' ';
            szStartTask[ nLth++ ] = '-';
            szStartTask[ nLth++ ] = 'd';

            GetApplDirectoryFromView( szNameExt, vNewSubtask,
                                      zAPPL_DIR_OBJECT, sizeof( szNameExt ) );
            strcat_s( szNameExt, sizeof( szNameExt ), pchDlg );
            strcat_s( szNameExt, sizeof( szNameExt ), ".xra" );

            // Activate using SfActivateSysOI_FromFile so that the kzxra.xod
            // does not have to be in the application LPLR.
            if ( SfActivateSysOI_FromFile( &vXRA, "KZXRA", vNewSubtask,
                                           szNameExt, zSINGLE ) == 0 )
            {
               if ( SetCursorFirstEntityByString( vXRA, "Wnd", "DfltWnd",
                                                  "Y", 0  ) == zCURSOR_SET )
               {
                  GetStringFromAttribute( szStartTask + nLth, vXRA, "Wnd", "Tag" );
               }

               return( WinExec( szStartTask, SW_SHOWMINIMIZED ) );
            }
#endif
            return( 0 );
         }
      }
   }

   return( -1 );

#else

   return( WinExec( cpcCmdLine, uCmdShow ) );

#endif
}

//./ ADD NAME=SetNewApplicationSubtask
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetNewApplicationSubtask
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set the new subtask (may be a different application).
//
//  PARAMETERS: vSubtask    - Current window subtask
//              vAppSubtask - Subtask for the new application
//
//  RETURNS:    0 - subtask view set
//             -1 - unable to locate current subtask window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
SetNewApplicationSubtask( zVIEW vSubtask,
                          zVIEW vAppSubtask )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      pZSubtask->m_pZTask->m_vNewAppSubtask = vAppSubtask;
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=CreateWindowSubtask
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CreateWindowSubtask
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:  This routine creates a subtask that can be used in a
//            subsequent SetNewApplicationSubtask or BuildWindowImmediate
//
//  PARAMETERS: pvSubtask   - Return subtask
//              vSubtask    - Current window subtask
//              vNewSubtask - Usually the same as vSubtaskIn, but can
//                            be a view from another application
//              cpcDlgTag  - Dialog to be associated with the subtask
//              cpcAppName  - Application (if specified) to be associated
//                            with the subtask
//
//  RETURNS:    0 - subtask created successfully
//             -1 - unable to locate the specified dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
CreateWindowSubtask( zVIEW   *pvSubtask,
                     zVIEW   vSubtask,
                     zVIEW   vNewSubtask,
                     zCPCHAR cpcDlgTag,
                     zCPCHAR cpcAppName )
{
   zCHAR  szViewName[ 68 ];
   zVIEW  vDialogNew;
   zSHORT nRC;

   if ( vNewSubtask == 0 )
      SfCreateSubtask( &vNewSubtask, vSubtask, cpcAppName );

   szViewName[ 0 ] = '_';  szViewName[ 1 ] = '_';
   strcpy_s( szViewName + 2, sizeof( szViewName ) - 2, cpcDlgTag );
   if ( GetViewByName( &vDialogNew, szViewName, vSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
   {
      zCHAR szNameExt[ 256 ];

      // This original Dialog view will never be used ... it is just here by
      // name so that new (usable) views can be declared/set from this one.
      GetApplDirectoryFromView( szNameExt, vNewSubtask, zAPPL_DIR_OBJECT, sizeof( szNameExt ) );
      strcat_s( szNameExt, sizeof( szNameExt ), cpcDlgTag );
      strcat_s( szNameExt, sizeof( szNameExt ), ".xwd" );

      // Activate using SfActivateSysOI_FromFile so that the kzwdlgxo.xod does not have to be in the application LPLR.
      if ( (nRC = SfActivateSysOI_FromFile( &vDialogNew, szlKZWDLGXO, vSubtask, szNameExt, zSINGLE )) == 0 )
      {
//       TraceLineS( "Setting name for view ... ", szViewName );
//       TraceLineI( "Named view is ", (zLONG) vDialogNew );
         SetNameForView( vDialogNew, szViewName, vSubtask, zLEVEL_TASK );   //???
      }

//    DisplayObjectInstance( vDialogNew, 0, 0 );
      if ( nRC )
      {
         // ERROR out of here ...
         pvSubtask = 0;
         TraceLineS( "(drvr) Cannot load Dialog - ", cpcDlgTag );
         IssueError( vSubtask, 0, 0, "Cannot load Dialog OI" );
         return( -1 );
      }
   }

   // Create a view to a Dialog and instantiate it.
   CreateViewFromViewForTask( pvSubtask, vDialogNew, vSubtask );

   // Now set the subtask view to be the new view to the dialog/window.
   SfSetSubtaskView( *pvSubtask, vNewSubtask );  // this drops vNewSubtask
   return( 0 );
}

#define zCOMBOBOX_CONTROL                   1050L
#define zLISTBOX_CONTROL                    1080L

void
fnResetCtrls( zVIEW vDialogNew, zLONG lX_Factor, zLONG lY_Factor )
{
   zLONG  lType;
   zLONG  lValue;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vDialogNew, "Ctrl", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lType, vDialogNew, "Ctrl", "Type" );
      if ( lType == zCOMBOBOX_CONTROL )
      {
         SetAttributeFromInteger( vDialogNew, "Ctrl", "SZDLG_Y", 12 );
      }
      else
      if ( lType == zLISTBOX_CONTROL )
      {
         nRC = SetCursorFirstEntity( vDialogNew, "ListBoxColumn", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lValue, vDialogNew, "ListBoxColumn", "ColumnWidth" );
            lValue = lValue * g_lDlgUnitCharX / 2;
            SetAttributeFromInteger( vDialogNew, "ListBoxColumn", "ColumnWidth", lValue );
            nRC = SetCursorNextEntity( vDialogNew, "ListBoxColumn", 0 );
         }
      }

      GetIntegerFromAttribute( &lValue, vDialogNew, "Ctrl", "PSDLG_X" );
      lValue = lValue * g_lDlgUnitCharX / 2;
      SetAttributeFromInteger( vDialogNew, "Ctrl", "PSDLG_X", lValue );
      GetIntegerFromAttribute( &lValue, vDialogNew, "Ctrl", "PSDLG_Y" );
      lValue = lValue * g_lDlgUnitCharY / 2;
      SetAttributeFromInteger( vDialogNew, "Ctrl", "PSDLG_Y", lValue );
      GetIntegerFromAttribute( &lValue, vDialogNew, "Ctrl", "SZDLG_X" );
      lValue = lValue * g_lDlgUnitCharX / 2;
      SetAttributeFromInteger( vDialogNew, "Ctrl", "SZDLG_X", lValue );
      GetIntegerFromAttribute( &lValue, vDialogNew, "Ctrl", "SZDLG_Y" );
      lValue = lValue * g_lDlgUnitCharY / 2;
      SetAttributeFromInteger( vDialogNew, "Ctrl", "SZDLG_Y", lValue );

      SetViewToSubobject( vDialogNew, "CtrlCtrl" );
      fnResetCtrls( vDialogNew, lX_Factor, lY_Factor );
      ResetViewFromSubobject( vDialogNew );

      nRC = SetCursorNextEntity( vDialogNew, "Ctrl", 0 );
   }
}

//./ ADD NAME=CreateRemoteWindowSubtask
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CreateRemoteWindowSubtask
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:  This routine creates a subtask that can be used in a
//            subsequent SetNewApplicationSubtask or BuildWindowImmediate
//
//  PARAMETERS: pvSubtask  - Return subtask
//              vSubtask   - Current window subtask
//              cpcDlgTag - Dialog to be associated with the subtask
//
//  RETURNS:    0 - subtask created successfully
//             -1 - unable to locate the specified dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
CreateRemoteWindowSubtask( zVIEW   *pvSubtask,
                           zVIEW   vSubtask,
                           zCPCHAR cpcDlgTag,
                           zCPCHAR cpcWndTag )
{
#ifdef zREMOTE_SERVER
   zCHAR  szViewName[ 68 ];
   zVIEW  vDialogNew;
   zPCHAR pch;
   zSHORT nRC;

   szViewName[ 0 ] = '_';  szViewName[ 1 ] = '@';
   strcpy_s( szViewName + 2, sizeof( szViewName ) - 2, cpcDlgTag );
   if ( GetViewByName( &vDialogNew, szViewName, vSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
   {
      zCHAR szNameExt[ 256 ];

      // This original Dialog view will never be used ... it is just here by
      // name so that new (usable) views can be declared/set from this one.
      GetApplDirectoryFromView( szNameExt, vSubtask, zAPPL_DIR_OBJECT, sizeof( szNameExt ) );
      strcat_s( szNameExt, sizeof( szNameExt ), cpcDlgTag );
      strcat_s( szNameExt, sizeof( szNameExt ), ".xra" );

      // Activate using SfActivateSysOI_FromFile so that the kzxra.xod
      // does not have to be in the application LPLR.
      if ( (nRC = SfActivateSysOI_FromFile( &vDialogNew, "KZXRA", vSubtask,
                                            szNameExt, zSINGLE )) == 0 )
      {
//       DisplayObjectInstance( vDialogNew, 0, 0 );
//       TraceLineS( "Setting name for view ... ", szViewName );
//       TraceLineI( "Named view is ", (zLONG) vDialogNew );
         SetNameForView( vDialogNew, szViewName, vSubtask, zLEVEL_TASK );

         zLONG lValue;

         // Reset the positions and sizes by a specified factor.
         nRC = SetCursorFirstEntity( vDialogNew, "Wnd", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lValue, vDialogNew, "Wnd", "PSDLG_X" );
            lValue = lValue * g_lDlgUnitCharX / 2;
            SetAttributeFromInteger( vDialogNew, "Wnd", "PSDLG_X", lValue );
            GetIntegerFromAttribute( &lValue, vDialogNew, "Wnd", "PSDLG_Y" );
            lValue = lValue * g_lDlgUnitCharY / 2;
            SetAttributeFromInteger( vDialogNew, "Wnd", "PSDLG_Y", lValue );

            GetIntegerFromAttribute( &lValue, vDialogNew, "Wnd", "SZDLG_X" );
            lValue = lValue * g_lDlgUnitCharX / 2;
            SetAttributeFromInteger( vDialogNew, "Wnd", "SZDLG_X", lValue );
            GetIntegerFromAttribute( &lValue, vDialogNew, "Wnd", "SZDLG_Y" );
            lValue = lValue * g_lDlgUnitCharY / 2;
            SetAttributeFromInteger( vDialogNew, "Wnd", "SZDLG_Y", lValue );

            GetIntegerFromAttribute( &lValue, vDialogNew, "Wnd", "ScrollX" );
            lValue = lValue * g_lDlgUnitCharY / 2;
            SetAttributeFromInteger( vDialogNew, "Wnd", "ScrollX", lValue );
            GetIntegerFromAttribute( &lValue, vDialogNew, "Wnd", "ScrollY" );
            lValue = lValue * g_lDlgUnitCharY / 2;
            SetAttributeFromInteger( vDialogNew, "Wnd", "ScrollY", lValue );

            fnResetCtrls( vDialogNew, g_lDlgUnitCharX, g_lDlgUnitCharY  );

            nRC = SetCursorNextEntity( vDialogNew, "Wnd", 0 );
         }

         nRC = SetCursorFirstEntity( vDialogNew, "Wnd", 0 );
      }
      else
      {
         // ERROR out of here ...
         pvSubtask = 0;
         TraceLineS( "(drvr) Cannot load Dialog - ", cpcDlgTag );
         MessageSend( vSubtask, 0, cpcDlgTag,
                      "Cannot load Dialog OI",
                      zMSGQ_MODAL_ERROR, FALSE );
         return( -1 );
      }
   }

   // Create a view to a Dialog and instantiate it with only the window
   // requested.
   ActivateOI_FromOI_ForTask( pvSubtask, vDialogNew, 0, zSINGLE );
   nRC = SetCursorFirstEntity( *pvSubtask, "Wnd", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetAddrForAttribute( (zPCHAR *) &pch, *pvSubtask, "Wnd", "Tag" );
      if ( zstrcmp( pch, cpcWndTag ) )
         DeleteEntity( *pvSubtask, "Wnd", zREPOS_NONE );

      nRC = SetCursorNextEntity( *pvSubtask, "Wnd", 0 );
   }

   // Now set the subtask view to be the new view to the dialog/window.
   SfSetSubtaskView( *pvSubtask, vSubtask );
#endif

   return( 0 );
}

//./ ADD NAME=GetActionParameters
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetActionParameters
//                                              06/30/98 Modified: 06/30/98
//
//  PURPOSE:   This operation gets the parameters passed to the action from
//             the event ... the returned pointer is valid only for the
//             duration of the action and should not be stored.  Also, the
//             meaning of the parameter depends on the event and must be
//             properly interpreted by the application.
//
//  PARAMETERS: vSubtask     - The subtask view.
//
//  RETURNS:    0 - No parameters for event/action
//             otw - Parameters passed to action from event
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zPVOID OPERATION
GetActionParameters( zVIEW  vSubtask )
{
#ifdef DEBUG_ALL
   TraceLineS( "GetActionParameters", "" );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      return( pZSubtask->m_pEventParms );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetActionName
//                                              06/30/98 Modified: 06/30/98
//
//  PURPOSE:   This operation gets the action name calling the current
//             application operation code.
//
//  PARAMETERS: vSubtask      - The subtask view.
//              pchActionName - (Return) current action name.
//
//  RETURNS:    0 - subtask and action successfully located
//             -1 - unable to locate the specified dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
GetActionName( zVIEW  vSubtask,
               zPCHAR pchActionTag )
{
#ifdef DEBUG_ALL
   TraceLineS( "GetActionName", "" );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      strcpy_s( pchActionTag, zTAG_LTH, pZSubtask->m_szActionName );
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=SetActionTrace
// Source Module=zdrdlga.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetActionTrace
//                                              06/30/98 Modified: 06/30/98
//
//  PURPOSE:   This operation set the trace level for the following actions.
//             This may be done to limit the amount of tracing for an
//             action that is very frequent (such as keystroke actions).
//             If the OE setting is '~', it cannot be reset by this
//             operation.
//
//  PARAMETERS: vSubtask    - The subtask view.
//              nTraceLevel - Level at which to trace actions
//                              0 - no trace
//                              1 - errors
//                              2 - errors + warnings
//                              3 - errors + warnings + informational
//                              4 - all
//                           0x70 - Report ('R')
//                           0x71 - Task ('T')
//                           0x72 - Subtask ('S')
//                           0x74 - View ('V')
//                           0x77 - Task/Subtask/View ('Z')
//                           0x99 - Cannot be reset by application ('~')
//
//  RETURNS:    0 - No parameters for event/action
//             otw - Parameters passed to action from event
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SetActionTrace( zVIEW vSubtask, zSHORT nTraceLevel )
{
   ZSubtask *pZSubtask;
   zSHORT   nOldTraceLevel = nTraceLevel;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      nOldTraceLevel = pZSubtask->m_pZTask->m_nTraceAction;

      // Don't let action trace be turned off if it is set above 0x70!
      if ( nOldTraceLevel < 0x70 )
         pZSubtask->m_pZTask->m_nTraceAction = nTraceLevel;
   }

#ifdef DEBUG_ALL
   TraceLineI( "SetActionTrace Prev: ", nOldTraceLevel );
   TraceLineI( "SetActionTrace: ", nTraceLevel );
#endif

   return( nOldTraceLevel );
}
