/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdlgw.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of "C interface" routines into the
// C++ functionality for windows available through the Zeidon driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.12.04    DKS   Jeff
//    Implemented CloseFrame to close a pane in a splitter window.
//
// 2002.08.24    DKS   Phil
//    Enforce documentation for FindDialogWindow with respect to empty
//    Window name.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.02    DKS   Z10
//    Added Twain acquire operation to get a scanned image.
//
// 2001.05.22    DKS   Z10
//    Permit add/remove of menu items in a popup (tracking) menu.
//
// 2001.05.11    DKS   Z10
//    BuildWindowImmediate now restricts the Window Action Behaviors.
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   Z10
//    Added new operation GetAppOrWorkstationValue.
//
// 2001.04.17    RS     WEB
//    Changes from DKS for Printout-Problem (Doc).
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 2000.12.15    HH
//    Modified GetApplicationInstance for Zeidon Web
//
// 2000.05.22    DKS   Z10    51363
//    Fixed GetWorkstationApplicationValues call to get Zeidon.ini.
//
// 2000.04.04    DKS   Z10    Enhance
//    Added ResetWindowPlacement operation.
//
// 1999.05.11    DKS   Z10    Enhance
//    Added RefreshWindowExceptForCtrl operation.
//
// 1999.02.04    DKS
//    Corrected GetWorkstationApplicationValues doc and implementation.
//
// 1999.01.20    DKS
//    Added new operation GetWorkstationApplicationValues.
//
// 1999.01.20    DKS
//    Added new operation GetParentWindowName.
//
// 1998.10.30    DKS
//    Added new operation RefreshParentSubwindows to correct
//    RefreshAllSubwindows.
//
// 1998.10.05    DKS
//    Corrected algorithm for locating a Subtask across tasks.
//
// 1998.09.30    DKS
//    Ctrl+E and ResumeFromPainter fix (TB 281).
//
// 1998.09.10    DKS
//    Second attempt to fix SetWindowState DESTROY case.
//
// 1998.08.31    DKS
//    Altered GetSubtaskForWindowName to get the active "subtask view"
//    if the view to the subtask passed in is null.  Added flag to
//    permit the user to Destroy a window using SetWindowState.
//
// 1998.08.13    DKS
//    Fixed return code for application.
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

// #define DEBUG_ALL

//./ ADD NAME=GetWindowAndCtrl
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowAndCtrl
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the C++ "handles" for the Window (via the specified
//             subtask) and the Control (via the control tag).  This is
//             used by C interface routines.
//
//  PARAMETERS: pZSubtaskReturn  - return pointer to C++ ZSubtask handle
//              pzmaReturn  - return pointer to C++ ZMapAct control handle
//              vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.  If this is
//                            0, only the Window pointer is returned
//
//  RETURNS:    -2  - Controls have not yet been created
//              -1  - Error locating window or control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetWindowAndCtrl( ZSubtask **pZSubtaskReturn,
                  ZMapAct  **pzmaReturn,
                  zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag )
{
#if 0 //def DEBUG_ALL  lots of messages
   TraceLineI( "In GetWindowAndCtrl - vSubtask = ", (zLONG) vSubtask );
#endif

   *pZSubtaskReturn = 0;
   if ( pzmaReturn )
      *pzmaReturn = 0;

   do    // purist's goto
   {
      if ( vSubtask && MiValidView( vSubtask ) )
      {
         if ( vSubtask->hViewOD )
         {
            zCHAR szViewObjectName[ 34 ];

            MiGetObjectNameForView( szViewObjectName, vSubtask );
            if ( szViewObjectName[ 0 ] == 'K' &&  // just for speed
                 szViewObjectName[ 1 ] == 'Z' &&
                 szViewObjectName[ 2 ] == 'W' &&
                 szViewObjectName[ 3 ] == 'D' &&
                 szViewObjectName[ 4 ] == 'L' &&
                 szViewObjectName[ 5 ] == 'G' &&
                 szViewObjectName[ 6 ] == 'X' &&
                 szViewObjectName[ 7 ] == 'O' &&
                 szViewObjectName[ 8 ] == 0 )
            {
               zLONG lItemCnt;
               if ( GetIntegerFromAttribute( (zPLONG) pZSubtaskReturn, vSubtask,
                                             szlSubtask, szlCPP ) == 0 )
               {
                  // Just an additional check to be sure we know we exist.
                  // Protect this with a semaphore!!!
                  SysMutexLock( vSubtask, "ZDr", 0, 0 );
               // TraceLineS( "GetWindowAndCtrl Lock Mutex ZDr", "" );

                  zPVOID pvDialog = SysGetHandleFromPointer( vSubtask );
                  lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
                  while ( lItemCnt > 0 )
                  {
                     if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
                          pvDialog == g_pSubtaskList[ lItemCnt ].pvDialog &&
                          *pZSubtaskReturn == g_pSubtaskList[ lItemCnt ].pZSubtask )
                     {
#ifdef zREMOTE_SERVER
                        if ( AfxGetThread( )->m_nThreadID !=
                                       (zULONG) g_pSubtaskList[ lItemCnt ].lTID )
                        {
                           lItemCnt = 0;     // in the wrong task!!!
                        }
#else
                     // if ( SysGetTaskFromView( vSubtask ) != g_pSubtaskList[ lItemCnt ].lTID )
                        if ( SysGetTaskFromView( g_vRegister ) != g_pSubtaskList[ lItemCnt ].lTID )
                        {
                           lItemCnt = 0;     // in the wrong task!!!
                        }
#endif
                        break;
                     }

                     lItemCnt--;
                  }

               // TraceLineS( "GetWindowAndCtrl Unlock Mutex ZDr", "" );
                  SysMutexUnlock( vSubtask, "ZDr", 0 );

                  if ( lItemCnt > 0 )
                  {
                     // we got what we wanted ... now see if we need ctrl
                     break;
                  }
               }

#ifdef DEBUG_ALL
               // Protect this with a semaphore!!!
               SysMutexLock( vSubtask, "ZDr", 0, 0 );
            // TraceLineS( "GetWindowAndCtrl2 Lock Mutex ZDr", "" );

               lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
               TraceLineX( "GetWindowAndCtrl cannot locate requested window: ",
                           (zLONG) vSubtask );
               TraceLineS( "               for object name: ", szViewObjectName );
               TraceLineI( "    within global subtask list: ", lItemCnt );
               while ( lItemCnt > 0 )
               {
                  if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
                  {
                     TraceLineX( "      vSubtask: ",
                                 (zLONG) g_pSubtaskList[ lItemCnt ].pvDialog );
                     TraceLineX( "   ==> Subtask: ",
                                 (zLONG) g_pSubtaskList[ lItemCnt ].pZSubtask );
                  }

                  lItemCnt--;
               }

               if ( *pZSubtaskReturn )
                  TraceLineX( "            in located subtask: ",
                              (zLONG) *pZSubtaskReturn );

            // TraceLineS( "GetWindowAndCtrl2 Unlock Mutex ZDr", "" );
               SysMutexUnlock( vSubtask, "ZDr", 0 );
#endif
            }
         }
         else
         if ( vSubtask->bInvalidMainOI && vSubtask->hMainOI )
         {
            *pZSubtaskReturn = (ZSubtask *) vSubtask->hMainOI;
            break;
         }
      }

      *pZSubtaskReturn = 0;
      return( -2 );

   } while ( FALSE );   // end of: purist's goto

   if ( cpcCtrlTag )
   {
      zULONG ulSubtaskState = ZSubtask::GetSubtaskState( *pZSubtaskReturn );
      if ( (ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED )
      {
         if ( (*pZSubtaskReturn)->m_pZMIXCtrl == 0 )
         {
            if ( (*pZSubtaskReturn)->m_pZTask->m_nTraceAction > 1 )
            {
               zCHAR szMsg[ 256 ];

               sprintf_s( szMsg, zsizeof( szMsg ),
                         "(drvr) Controls have not yet been created for: %s.%s.%s ",
                         (*(*pZSubtaskReturn)->m_pzsDlgTag).GetString(),
                         (*(*pZSubtaskReturn)->m_pzsWndTag).GetString(), cpcCtrlTag );
               TraceLineI( szMsg, (zLONG) *pZSubtaskReturn );
            }

            return( -2 );
         }

         // Run through the list of Child windows to find the one with the specified name.
         CString csSeek( cpcCtrlTag );  // localize Tag for the seek
         *pzmaReturn = (ZMapAct *)(*pZSubtaskReturn)->m_pZMIXCtrl->SeekNode( &csSeek );
         if ( *pzmaReturn )
         {
         // TraceLineS( "Located control by name = ", (*pzmaReturn)->pzsTag );
         // TraceLineI( "Located control by ID   = ", (*pzmaReturn)->nIdNbr );
            return( 0 );
         }
         else
         {
            if ( (*pZSubtaskReturn)->m_pZTask->m_nTraceAction > 1 )
            {
               TraceLine( "Cannot locate control: %s.%s.%s",
                          (*(*pZSubtaskReturn)->m_pzsDlgTag).GetString(),
                          (*(*pZSubtaskReturn)->m_pzsWndTag).GetString(), cpcCtrlTag );
            }

            return( -1 );
         }
      }
      else
         return( -1 );
   }
   else
      return( 0 ); // leave pzmaReturn alone since it is not requested
}

//./ ADD NAME=GetWindowName
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowName
//                                              01/20/99 Modified: 01/20/99
//
//  PURPOSE:   To get the window's Dialog and Window name.
//
//  PARAMETERS: vSubtask    - The subtask view for the window for which
//                            the name is desired...
//              pchDlgTag   - return tag of dialog
//              pchWndTag   - return tag of window
//
//  RETURNS:   0  - successfully found parent view
//             otherwise - window's subtask view could not be found
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GetWindowName( zVIEW  vSubtask,
               zPCHAR pchDlgTag,
               zPCHAR pchWndTag )
{
#ifdef DEBUG_ALL
   TraceLineI( "In GetWindowName - vSubtask = ", (zLONG) vSubtask );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      strcpy_s( pchDlgTag, zTAG_LTH, *(pZSubtask->m_pzsDlgTag) );
      strcpy_s( pchWndTag, zTAG_LTH, *(pZSubtask->m_pzsWndTag) );
      return( 0 );
   }

   pchDlgTag[ 0 ] = 0;
   pchWndTag[ 0 ] = 0;
   return( -1 );
}

//./ ADD NAME=GetParentWindow
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetParentWindow
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the parent window's subtask view
//
//  PARAMETERS: vSubtask    - The subtask view for the window for which
//                            the parent is desired...
//
//  RETURNS:   0  - successfully found parent view
//             otherwise - parent window's subtask view could not be found
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
GetParentWindow( zPVIEW pParentSubtask,
                 zVIEW  vSubtask )
{
#ifdef DEBUG_ALL
   TraceLineI( "In GetParentWindow - vSubtask = ", (zLONG) vSubtask );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZParent )
   {
      *pParentSubtask = pZSubtask->m_pZParent->m_vDialog;
      return( 0 );
   }

   *pParentSubtask = 0;
   return( -1 );
}

//./ ADD NAME=GetParentWindowName
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetParentWindowName
//                                              01/20/99 Modified: 01/20/99
//
//  PURPOSE:   To get the parent window's Dialog and Window name.
//
//  PARAMETERS: vSubtask    - The subtask view for the window for which
//                            the parent is desired...
//              pchDlgTag   - return tag of parent dialog
//              pchWndTag   - return tag of parent window
//
//  RETURNS:   0  - successfully found parent view
//             otherwise - parent window's subtask view could not be found
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GetParentWindowName( zVIEW  vSubtask,
                     zPCHAR pchDlgTag,
                     zPCHAR pchWndTag )
{
#ifdef DEBUG_ALL
   TraceLineI( "In GetParentWindowName - vSubtask = ", (zLONG) vSubtask );
#endif

   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZParent )
   {
      pZSubtask = pZSubtask->m_pZParent;
      strcpy_s( pchDlgTag, zTAG_LTH, *(pZSubtask->m_pzsDlgTag) );
      strcpy_s( pchWndTag, zTAG_LTH, *(pZSubtask->m_pzsWndTag) );
      return( 0 );
   }

   pchDlgTag[ 0 ] = 0;
   pchWndTag[ 0 ] = 0;
   return( -1 );
}

//./ ADD NAME=GetWindowHandle
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowHandle
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the Windows "handles" for the specified subtask and
//             the Control (via the control tag).
//
//  PARAMETERS: pWindowReturn - return pointer to Windows handle
//              pCtrlReturn - return pointer to ctrl Windows handle
//              vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.  If this is
//                            0, only the Window pointer is returned
//
//  RETURNS:    -2  - Controls have not yet been created
//              -1  - Error locating window or control
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetWindowHandle( zPULONG pWindowReturn,
                 zPULONG pCtrlReturn,
                 zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;
   zBOOL    bCtrlTag = TRUE;

#ifdef DEBUG_ALL
   TraceLineI( "In GetWindowHandle - vSubtask = ", (zLONG) vSubtask );
#endif

   if ( (zLONG) cpcCtrlTag == -1 || cpcCtrlTag == 0 || cpcCtrlTag[ 0 ] == 0 )
   {
      bCtrlTag = FALSE;
      cpcCtrlTag = 0;
   }

   // Get the structure of attributes for this control.
// if ( GetIntegerFromAttribute( (zPLONG) &pZSubtask, vSubtask,
//                               szlSubtask, szlCPP ) != 0 ||
//      pZSubtask->m_pZFWnd == 0 )
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) != 0 ||
        pZSubtask->m_pZFWnd == 0 )
   {
      *pWindowReturn = 0;
      TraceLineS( "GetWindowHandle Cannot locate requested window", "" );
   // IssueError( vSubtask, 0, 0, "Cannot locate requested window" );
      return( -2 );
   }

   if ( mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) == FALSE )
   {
      TraceLineI( "Invalid GetWindowHandle ",
                  (zLONG) pZSubtask->m_pZFWnd->m_hWnd );
   }
   else
   {
   // TraceLineI( "Valid GetWindowHandle ",
   //             (zLONG) pZSubtask->m_pZFWnd->m_hWnd );
   }

   *pWindowReturn = (zULONG) pZSubtask->m_pZFWnd->m_hWnd;

   if ( cpcCtrlTag )
   {
      if ( pZSubtask->m_pZMIXCtrl == 0 )
      {
         *pCtrlReturn = 0;
         TraceLineX( "(drvr) Controls have not yet been created ",
                     (zLONG) pZSubtask );
         return( -2 );
      }

      // run through the list of Child windows to find the one with the
      // specified name
      CString csSeek( cpcCtrlTag );  // localize Tag for the seek
      pzma = (ZMapAct *) pZSubtask->m_pZMIXCtrl->SeekNode( &csSeek );
      if ( pzma )
      {
//       TraceLineS( "Located control by name = ",
//                   pzma->pzsTag );
//       TraceLineI( "Located control by ID   = ", pzma->nIdNbr );
         *pCtrlReturn = (zULONG) pzma->m_pCtrl->m_hWnd;
         return( 0 );
      }
      else
      {
         TraceLineS( "Cannot locate control - ", cpcCtrlTag );
         *pCtrlReturn = 0;
         return( -1 );
      }
   }
   else
   if ( bCtrlTag == FALSE  && pCtrlReturn )
      *pCtrlReturn = (zLONG) (pZSubtask->m_pZTask->m_pZDrApp->m_hWndCommOE);

   return( 0 ); // leave pCtrlReturn alone since it is not requested
}

//./ ADD NAME=GetApplicationInstance
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetApplicationInstance
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the Windows instance handle for the specified subtask
//
//  PARAMETERS: vSubtask    - The subtask view for the application
//
//  RETURNS:     0  - Error locating window
//               otherwise - HINSTANCE for the application
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
GetApplicationInstance( zVIEW  vSubtask )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      HINSTANCE hInst = pZSubtask->m_pZTask->m_pZDrApp->m_hInstance;

      if ( pZSubtask && pZSubtask->m_pZFWnd && pZSubtask->m_pZFWnd->m_hWnd )
      {
#ifdef __WIN32__
         HINSTANCE hInst2 = (HINSTANCE)
            ::GetWindowLong( pZSubtask->m_pZFWnd->m_hWnd, GWL_HINSTANCE );
#else
         HINSTANCE hInst2 = (HINSTANCE)
            ::GetWindowWord( pZSubtask->m_pZFWnd->m_hWnd, GWW_HINSTANCE );
#endif
         if ( hInst != hInst2 )
         {
            MessageSend( vSubtask, 0,
                         "GetApplicationInstance",
                         "HINSTANCE difference",
                         zMSGQ_MODAL_ERROR, FALSE );
         }

         hInst = hInst2;
      }


      if ( hInst == 0 )
      {
         MessageSend( vSubtask, 0,
                      "GetApplicationInstance",
                      "HINSTANCE ZERO",
                      zMSGQ_MODAL_ERROR, FALSE );
      }

      TraceLineI( "GetApplicationInstance = ", (zLONG) hInst );
      return( (zLONG) hInst );
   }

   return( 0 );
}

//./ ADD NAME=GetWindowAndOption
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowAndOption
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the C++ "handles" for the Window (via the specified
//             subtask) and the Option (via the option tag).  This is
//             used by C interface routines.
//
//  PARAMETERS: pZSubtaskReturn  - return pointer to C++ ZSubtask handle
//              pzmaReturn  - return pointer to C++ ZOption option handle
//              vSubtask    - The subtask view for the window containing
//                            the menu.
//              cpcOptTag   - The option tag (unique name).  If this is
//                            0, only the Window pointer is returned
//
//  RETURNS:    -1  - Error locating window or option
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetWindowAndOption( ZSubtask **pZSubtaskReturn,
                    ZOption  **pZOptionReturn,
                    zVIEW    vSubtask,
                    zCPCHAR  cpcOptTag )
{
#ifdef DEBUG_ALL
   TraceLineI( "In GetWindowAndOption - vSubtask = ", (zLONG) vSubtask );
#endif

   // Locate the subtask.
   if ( GetWindowAndCtrl( pZSubtaskReturn, 0, vSubtask, 0 ) )
   {
      TraceLineX( "GetWindowAndOption cannot locate requested window: ",
                  (zLONG) vSubtask );
   // int k = 0;
   // k /= k;
      return( -2 );
   }

   if ( cpcOptTag )
   {
      // Protect this with a semaphore!!!
      SysMutexLock( vSubtask, "ZDr", 0, 0 );
   // TraceLineS( "GetWindowAndOption Lock Mutex ZDr", "" );

      zULONG ulSubtaskState = ZSubtask::GetSubtaskState( *pZSubtaskReturn );

   // TraceLineS( "GetWindowAndCtrl Unlock Mutex ZDr", "" );
      SysMutexUnlock( vSubtask, "ZDr", 0 );

      if ( (ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED )
      {
         if ( (*pZSubtaskReturn)->m_pZBinMenu == 0 )
         {
            *pZOptionReturn = 0;
            TraceLineS( "Menu has not yet been created: ", cpcOptTag );
            return( -2 );
         }

         // Run through the list of options to find the one with the
         // specified name.  Localize the cpcOptTag for the seek.

         CString csSeek( cpcOptTag );
         *pZOptionReturn =
              (ZOption *) (*pZSubtaskReturn)->m_pZBinMenu->SeekNode( &csSeek );
         if ( *pZOptionReturn )
            return( 0 );
         else
         {
            TraceLineS( "Cannot locate option - ", cpcOptTag );
            return( -1 );
         }
      }
      else
         return( -1 );
   }
   else
      return( 0 ); // leave pZOptionReturn alone since it is not requested
}

//./ ADD NAME=GetWindowAndPopupOption
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowAndPopupOption
//                                              05/22/01 Modified: 05/22/01
//
//  PURPOSE:   To get the C++ "handles" for the Window (via the specified
//             subtask) and the Option (via the option tag).  This is
//             used by C interface routines.
//
//  PARAMETERS: pZSubtaskReturn  - return pointer to C++ ZSubtask handle
//              pzmaReturn  - return pointer to C++ ZOption option handle
//              vSubtask    - The subtask view for the window containing
//                            the menu.
//              cpcOptTag   - The option tag (unique name).  If this is
//                            0, only the Window pointer is returned
//
//  RETURNS:    -1  - Error locating window or option
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetWindowAndPopupOption( ZSubtask **pZSubtaskReturn,
                         ZOption  **pZOptionReturn,
                         zVIEW    vSubtask,
                         zCPCHAR  cpcOptTag )
{
#ifdef DEBUG_ALL
   TraceLineI( "In GetWindowAndPopupOption - vSubtask = ", (zLONG) vSubtask );
#endif

   // Locate the subtask.
   if ( GetWindowAndCtrl( pZSubtaskReturn, 0, vSubtask, 0 ) )
   {
      TraceLineX( "GetWindowAndPopupOption cannot locate requested window: ",
                  (zLONG) vSubtask );
      return( -2 );
   }

   if ( cpcOptTag )
   {
      if ( (*pZSubtaskReturn)->m_pZBinPopup == 0 )
      {
         *pZOptionReturn = 0;
         TraceLineS( "Popup Menu has not yet been created: ", cpcOptTag );
         return( -2 );
      }

      // Run through the list of options to find the one with the
      // specified name.  Localize the cpcOptTag for the seek.
      CString csSeek( cpcOptTag );
      *pZOptionReturn =
           (ZOption *) (*pZSubtaskReturn)->m_pZBinPopup->SeekNode( &csSeek );
      if ( *pZOptionReturn )
         return( 0 );
      else
      {
         TraceLineS( "Cannot locate option - ", cpcOptTag );
         return( -1 );
      }
   }
   else
      return( 0 ); // leave pZOptionReturn alone since it is not requested
}

//./ ADD NAME=MapWindow
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: MapWindow
//                                              04/19/93 Modified: 04/19/93
//
//  PURPOSE:   To map (MapToOI) all controls in the specified window
//
//  PARAMETERS: vSubtask    - The subtask view for the window which is to
//                            have its data mapped to the OI.
//
//  RETURNS:    0 - Mapped the specified window
//              1 - Mapping error occurred
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
MapWindow( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zSHORT nRC = pZSubtask->MapToOI( );
      if ( nRC == 0 )
         return( 0 );
      else
         return( 1 );
   }

   return( -1 );
}

//./ ADD NAME=RefreshWindow
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RefreshWindow
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To refresh (MapFromOI) all controls in the specified window
//
//  PARAMETERS: vSubtask    - The subtask view for the window which is to
//                            receive focus.
//
//  RETURNS:    0 - Refresh the specified window
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RefreshWindow( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
   // TraceLineS( "RefreshWindow tag: ", *(pZSubtask->m_pzsWndTag) );
      pZSubtask->MapFromOI( );
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=RefreshWindowExceptForCtrl
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RefreshWindowExceptForCtrl
//                                              04/26/99 Modified: 04/26/99
//
//  PURPOSE:   To refresh (MapFromOI) all controls in the specified window
//
//  PARAMETERS: vSubtask    - The subtask view for the window which is to
//                            receive focus.
//              cpcCtrlTag  - The unique name of the control which is NOT
//                            to be refreshed.  This may be a semi-colon
//                            separated list of controls if the first
//                            character is a TAB character.
//
//  RETURNS:    0 - Refresh the specified window
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RefreshWindowExceptForCtrl( zVIEW vSubtask,
                            zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zLONG lLth = zstrlen( cpcCtrlTag ) + 1;
      zPCHAR pch = new char[ lLth ];
      strcpy_s( pch, lLth, cpcCtrlTag );
//    TraceLineS( "RefreshWindowExceptForCtrl tag", *(pZSubtask->m_pzsWndTag) );
      pZSubtask->MapFromOI( 0, pch );
      delete [] pch;
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=GetLastDlgWnd
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetLastDlgWnd
//                                          06/12/2002 Modified: 10/22/2002
//
//  PURPOSE:   To get the last dialog/window tags on a return from a window.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window.
//              pchDlgTag   - Last dialog tag (return).
//              pchWndTag   - Last window tag (return).
//
//  RETURNS:    0 - OK
//             -1 - Error locating window.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
GetLastDlgWnd( zVIEW vSubtask, zPCHAR pchDlgTag, zPCHAR pchWndTag )
{
   ZSubtask *pZSubtask;

   *pchDlgTag = *pchWndTag = 0;
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pZTask->m_pzsLastDlgTag &&
           pZSubtask->m_pZTask->m_pzsLastWndTag )
      {
         strcpy_s( pchDlgTag, zTAG_LTH, *(pZSubtask->m_pZTask->m_pzsLastDlgTag) );
         strcpy_s( pchWndTag, zTAG_LTH, *(pZSubtask->m_pZTask->m_pzsLastWndTag) );
         return( 0 );
      }
   }

   return( -1 );
}

//./ ADD NAME=SplitFrame
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SplitFrame
//                                          06/12/2002 Modified: 10/22/2002
//
//  PURPOSE:   To split a frame into panes.
//
//  PARAMETERS: vSubtask    - The subtask view for the window which is to
//                            be split.
//              cpcSplitTag - The unique name of the splitter window.
//              nPercentX   - Percentage of client area to be taken up by
//                            the width of the "0 frame".
//              nPercentY   - Percentage of client area to be taken up by
//                            the height of the "0 frame".
//              lFlag       - 1 ==> right; 2 ==> down
//
//  RETURNS:    0 - OK
//             -1 - Error locating window or there is no view to split.
//             -2 - Invalid flag specification.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zLONG  OPERATION
SplitFrame( zVIEW   vSubtask,
            zCPCHAR cpcSplitTag,
            zSHORT  nPercentX,
            zSHORT  nPercentY,
            zLONG   lFlag )
{
   ZDrApp   *pApp = (ZDrApp *) AfxGetApp( );
   ZSubtask *pZSubtask;

   if ( pApp == 0 || pApp->m_pDoc == 0 ||
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) ||
        pZSubtask->m_pZView == 0 )
   {
      return( -1 );
   }

   ZSubtask::SetSubtaskState( pZSubtask, 0, zSUBTASK_STATE_SPLIT_IN_PROCESS );
   zSHORT nRowCnt;
   zSHORT nColCnt;

   if ( lFlag == 1 )
   {
      nRowCnt = 0;
      nColCnt = 1;
   }
   else
   if ( lFlag == 2 )
   {
      nRowCnt = 1;
      nColCnt = 0;
   }
   else
   {
      TraceLineI( "SplitFrame invalid parameter lFlag: ", lFlag );
      ASSERT( FALSE );
      return( -2 );
   }

#ifdef DEBUG_ALL
   TraceLine( "SplitFrame Subtask: %d  ZView: %d Q:%d  ZFWnd: %d",
              pZSubtask->m_ulIdNbr, pZSubtask->m_pZView->m_ulIdNbr,
              pZSubtask->m_pZView->m_eQuad,
              pZSubtask->m_pZFWnd ?
                ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_ulIdNbr : 0 );
#endif

   // Set up a context.
   CCreateContext context;
   context.m_pNewViewClass = RUNTIME_CLASS( ZDrView );
   context.m_pCurrentDoc = pApp->m_pDoc;

   pZSubtask->m_ulSubtaskFlags |= zSUBTASK_SUBWND_SPLITTER_PANE;

   ZSplitterWnd *pSplitterWnd = new ZSplitterWnd( pZSubtask, cpcSplitTag,
                                                  nPercentX, nPercentY,
                                                  lFlag );
   if ( pZSubtask->GetSplitterMainSubtask( ) == 0 )
   {
   do  // purist's goto
   {
      // Create the new splitter for the frame's view.
      ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_lPaneId =
                                        pZSubtask->m_pZView->GetDlgCtrlID( );
      pZSubtask->m_ulSubtaskFlags |= zSUBTASK_SPLITTER_MAIN_SUBTASK;

      ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_pSplitterWnd = pSplitterWnd;

   // pSplitterWnd->CreateStatic( pSplitterView, nRowCnt + 1, nColCnt + 1 );
      if ( pSplitterWnd->CreateStatic( pZSubtask->m_pZFWnd,
                                       nRowCnt + 1, nColCnt + 1 ) == 0 )
      {
         TraceLineS( "Failed to create splitter window", "" );
         break;
      }

      // Get the client rect first so it can be used to calculate pane sizes.
      CRect rect;
      pZSubtask->m_pZView->GetClientRect( &rect );

      // Create the left and right pane views.
      if ( pSplitterWnd->CreateView( nRowCnt, nColCnt,
                                     RUNTIME_CLASS( ZDrView ),
                                     CSize( 0, 0 ),
                                     &context ) == 0 )
      {
         TRACE0( "Failed to create pane view\n" );
         break;
      }

      CWnd *pParent = pZSubtask->m_pZView->SetParent( pSplitterWnd );
      ASSERT( pParent->m_hWnd == pZSubtask->m_pZFWnd->m_hWnd );

      // Make pane( 0, 0 ) in the new splitter to be the pane that was
      // originally pane( nRow, nCol ) in the parent splitter.
      pZSubtask->m_pZView->m_pParentSplitterView = 0;
      pZSubtask->m_pZView->m_pParentSplitterWnd = pSplitterWnd;
      pZSubtask->m_pZView->SetDlgCtrlID( pSplitterWnd->IdFromRowCol( 0, 0 ) );

      ZDrView *pPaneView1 = (ZDrView *) pSplitterWnd->GetPane( 0, 0 );
      ZDrView *pPaneView2 = (ZDrView *) pSplitterWnd->GetPane( nRowCnt,
                                                               nColCnt );
      pPaneView1->m_pParentSplitterWnd = pSplitterWnd;
      pPaneView2->m_pParentSplitterWnd = pSplitterWnd;

#ifdef DEBUG_ALL
      TraceLine( "SplitFrame  MainFrame: %d  SplitterWnd: %s  "
                    "Pane1( 0, 0 ): %d  Pane2( %d, %d ): %d   x,y%%: %d,%d",
                 ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_ulIdNbr,
                 *(pSplitterWnd->m_pcsSplitTag),
                 pPaneView1->m_ulIdNbr, nRowCnt, nColCnt,
                 pPaneView2->m_ulIdNbr, nPercentX, nPercentY );
#endif

      pZSubtask->m_pZView->GetClientRect( rect );
   // pPaneView1->MoveWindow( -2, -2, rect.Width( ) / 2, rect.Height( ) );
      if ( nRowCnt )
      {
      // pSplitterWnd->SetRowInfo( 0, ((rect.Height( ) * nPercentY) / 100) - 4, 0 );
      // pSplitterWnd->SetRowInfo( 1, ((rect.Height( ) * (100 - nPercentY)) / 100) - 4, 0 );
         pSplitterWnd->SetRowInfo( 0, ((rect.Height( ) * (100 - nPercentY)) / 100) - 4, 0 );
         pSplitterWnd->SetRowInfo( 1, ((rect.Height( ) * nPercentY) / 100) - 4, 0 );
         pSplitterWnd->SetColumnInfo( 0, rect.Width( ), 0 );
         pPaneView1->m_eQuad = ZDrView::eTop;
         pPaneView2->m_eQuad = ZDrView::eBottom;
      }
      else
      {
         pSplitterWnd->SetRowInfo( 0, rect.Height( ), 0 );
         pSplitterWnd->SetColumnInfo( 0, ((rect.Width( ) * nPercentX) / 100) - 4, 0 );
         pSplitterWnd->SetColumnInfo( 1, ((rect.Width( ) * (100 - nPercentX)) / 100) - 4, 0 );
      // pSplitterWnd->SetColumnInfo( 0, ((rect.Width( ) * (100 - nPercentX)) / 100) - 4, 0 );
      // pSplitterWnd->SetColumnInfo( 1, ((rect.Width( ) * nPercentX) / 100) - 4, 0 );
         pPaneView1->m_eQuad = ZDrView::eLeft;
         pPaneView2->m_eQuad = ZDrView::eRight;
      }

      pPaneView2->m_pPrevView = pZSubtask->m_pZView;
      if ( pZSubtask->m_pZView->m_pNextView == 0 )
      {
         pPaneView2->m_pNextView = pZSubtask->m_pZView;
         pZSubtask->m_pZView->m_pPrevView = pPaneView2;
      }
      else
      {
         ASSERT( FALSE );  // shouldn't get here
         pZSubtask->m_pZView->m_pNextView->m_pPrevView = pPaneView2;
         pPaneView2->m_pNextView = pZSubtask->m_pZView->m_pNextView;
      }

      pZSubtask->m_pZView->m_pNextView = pPaneView2;

      // Set the left pane as the active view.
      pZSubtask->m_pZFWnd->SetActiveView( pPaneView1 );
      pZSubtask->m_pZFWnd->DelayRecalcLayout( );
      pSplitterWnd->RecalcLayout( );

   } while ( FALSE );  // end of: purist's goto
   }
   else
   {
      // We are splitting a splitter.
   do  // purist's goto
   {
      ASSERT( pZSubtask->m_pZView->m_eQuad != ZDrView::eUndef );
      context.m_pNewViewClass = RUNTIME_CLASS( ZSplitterView );
      ZSplitterWnd *pParentSplitterWnd =
                          (ZSplitterWnd *) pZSubtask->m_pZView->GetParent( );
      ZSplitterView *pParentSplitterView =
         DYNAMIC_DOWNCAST( ZSplitterView, pParentSplitterWnd->GetParent( ) );

      // Determine row/col of current subtask view in ParentSplitterWnd.
      zSHORT nRow = (pZSubtask->m_pZView->m_eQuad == ZDrView::eBottom) ? 1 : 0;
      zSHORT nCol = (pZSubtask->m_pZView->m_eQuad == ZDrView::eRight) ? 1 : 0;
      ZDrView *pPaneView1 = (ZDrView *) pParentSplitterWnd->GetPane( 0, 0 );
      ZDrView *pPaneView2 = (ZDrView *) pParentSplitterWnd->GetPane( nRow, nCol );

#ifdef DEBUG_ALL
      TraceLine( "SplitFrame  MainFrame: %d  ParentSplitterWnd: %s  "
                    "Pane1( 0, 0 ): %d  Pane2( %d, %d ): %d",
                    pZSubtask->m_pZFWnd ? ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_ulIdNbr : 0,
                 *(pParentSplitterWnd->m_pcsSplitTag),
                 pPaneView1->m_ulIdNbr, nRow, nCol, pPaneView2->m_ulIdNbr );
#endif

      // Remove the subtask view from its parent splitter so that it can be
      // set as pane( 0, 0 ) in the new splitter (to be created).
      pZSubtask->m_pZView->SetDlgCtrlID( 0 );
      pZSubtask->m_pZView->ShowWindow( SW_HIDE );

   // TraceLine( "Shifting subtask view out: 0x%08x of Splitter: 0x%08x\n",
   //             pZSubtask->m_pZView, pParentSplitterWnd );
   //
   // zCHAR szMsg[ 256 ];
   //
   // sprintf_s( szMsg, zsizeof( szMsg ), "SplitFrame: %s ZSubtask: 0x%08x ZView: 0x%08x",
   //           cpcSplitTag, pZSubtask, pZSubtask->m_pZView );
   // TraceLineS( szMsg, "" );

      CRect rect;
      CSize size;

      pZSubtask->m_pZView->GetClientRect( rect );
      size.cx = rect.Width( );
      if ( nColCnt )
         size.cx = size.cx * nPercentX / 100;

      size.cy = rect.Height( );
      if ( nRowCnt )
         size.cy = size.cy * nPercentY / 100;

      // Create a splitter view (which can contain multiple views) to
      // replace the pane view that was removed from the parent splitter
      // above.
   // size.cx = size.cy = 200;
      if ( !pParentSplitterWnd->CreateView( nRow, nCol,
                                            RUNTIME_CLASS( ZSplitterView ),
                                            size, &context ) )
      {
         TRACE0( "Failed to create splitter view\n" );
         break;
      }

      pParentSplitterWnd->m_bSplitterView[ nRow ][ nCol ] = TRUE;

      // Now retrieve and show the newly created splitter view.
      ZSplitterView *pSplitterView =
              (ZSplitterView *) pParentSplitterWnd->GetPane( nRow, nCol );
      pSplitterView->ShowWindow( SW_SHOW );

#ifdef DEBUG_ALL
      TraceLine( "SplitFrame  New SplitterView (0x%08x) %d: ( %d, %d )  "
                    "for ParentSplitterWnd: %s",
                 pSplitterView, pSplitterView->m_ulIdNbr, nRow, nCol,
                 *(pParentSplitterWnd->m_pcsSplitTag) );
#endif

      // Create the new splitter for the pane view.
      ASSERT( pSplitterView->m_pSplitterWnd == 0 );
      pSplitterView->m_pSplitterWnd = pSplitterWnd;
      pSplitterWnd->CreateStatic( pSplitterView, nRowCnt + 1, nColCnt + 1 );

      // Keep parent history to assist in deletion of a splitter pane.
      ZParentData *pParentData = new ZParentData( pParentSplitterView,
                                                  pParentSplitterWnd,
                                                  pSplitterView,
                                                  nRow, nCol );

      // Make pane( 0, 0 ) in the new splitter to be the pane that was
      // originally pane( nRow, nCol ) in the parent splitter.
      pParentData->m_pOrigViewParent =
            (ZSplitterWnd *) pZSubtask->m_pZView->SetParent( pSplitterWnd );
      pZSubtask->m_pZView->m_pParentSplitterView = pSplitterView;
      pZSubtask->m_pZView->m_pParentSplitterWnd = pSplitterWnd;
      pSplitterView->m_pParentData = pParentData;

#ifdef DEBUG_ALL
      TraceLine( "SplitFrame Reset parent of ZView: %d ==> "
                    "ParentSplitterWnd: %s  ParentSplitterView: %d ->"
                    "ParentData: %d",
                 pZSubtask->m_pZView->m_ulIdNbr, *(pSplitterWnd->m_pcsSplitTag),
                 pSplitterView->m_ulIdNbr, pParentData->m_ulIdNbr );
#endif

      pZSubtask->m_pZView->SetDlgCtrlID( pSplitterWnd->IdFromRowCol( 0, 0 ) );
      pZSubtask->m_pZView->EnableWindow( TRUE );
      pZSubtask->m_pZView->ShowWindow( SW_SHOW );

      // Now create a pane view for the "other" pane in the new splitter.
      if ( pSplitterWnd->CreateView( nRowCnt, nColCnt,
                                     RUNTIME_CLASS( ZDrView ),
                                     CSize( 0, 0 ), &context ) == 0 )
      {
         TRACE0( "Failed to create pane view\n" );
      }

      ZDrView *pZView = (ZDrView *) pSplitterWnd->GetPane( nRowCnt, nColCnt );
      pZView->m_pParentSplitterView = pSplitterView;
      pZView->m_pParentSplitterWnd = pSplitterWnd;

#ifdef DEBUG_ALL
      TraceLine( "SplitFrame new SplitterWnd: %s with Pane( 0, 0 ): "
                     "%d SplitView: %d  Pane( %d, %d ): %d SplitView: %d   x,y%%: %d,%d",
                 *(pSplitterWnd->m_pcsSplitTag), pZSubtask->m_pZView->m_ulIdNbr,
                 pZSubtask->m_pZView->m_pParentSplitterView->m_ulIdNbr,
                 nRowCnt, nColCnt, pZView->m_ulIdNbr,
                 pZView->m_pParentSplitterView->m_ulIdNbr, nPercentX, nPercentY );
#endif

      // OnInitialUpdate( ) is not called as a result of calling
      // CreateView( ) above.  It is not always called by the framework,
      // so it is called here.
      pSplitterView->OnInitialUpdate( );

   // CWnd *pOrigParent = pFrame->GetParent( );
   // if ( pOrigParent != pParentSplitterWnd )
   //    pOrigParent = pFrame->SetParent( pParentSplitterWnd );

   // pFrame->SetDlgCtrlID( pSplitterWnd->IdFromRowCol( nRow, nCol ) );

      // Set new pane size.
      if ( nRowCnt )
      {
         pZSubtask->m_pZView->m_eQuad = ZDrView::eTop;
         pZView->m_eQuad = ZDrView::eBottom;
         pSplitterWnd->SetRowInfo( 0, rect.Height( ) / 2, 0 );
         pSplitterWnd->SetRowInfo( 1, rect.Height( ) / 2, 0 );
         pSplitterWnd->SetColumnInfo( 0, rect.Width( ), 0 );
      }
      else
      {
         pZSubtask->m_pZView->m_eQuad = ZDrView::eLeft;
         pZView->m_eQuad = ZDrView::eRight;
         pSplitterWnd->SetRowInfo( 0, rect.Height( ), 0 );
         pSplitterWnd->SetColumnInfo( 0, rect.Width( ) / 2, 0 );
         pSplitterWnd->SetColumnInfo( 1, rect.Width( ) / 2, 0 );
      }

      pZView->m_pPrevView = pZSubtask->m_pZView;
      if ( pZSubtask->m_pZView->m_pNextView == 0 )
      {
         ASSERT( FALSE );  // shouldn't get here
         pZView->m_pNextView = pZSubtask->m_pZView;
         pZSubtask->m_pZView->m_pPrevView = pZView;
      }
      else
      {
         pZSubtask->m_pZView->m_pNextView->m_pPrevView = pZView;
         pZView->m_pNextView = pZSubtask->m_pZView->m_pNextView;
      }

      pZSubtask->m_pZView->m_pNextView = pZView;

      ZDrFrame *pMainFrame;
      ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
      if ( pZSubtaskMain && pZSubtaskMain->m_pZFWnd )
         pMainFrame = (ZDrFrame *) pZSubtaskMain->m_pZFWnd;
      else
         pMainFrame = (ZDrFrame *) AfxGetMainWnd( );

      CWnd *pWndParent = pSplitterWnd->GetParent( );
      while ( pWndParent && pWndParent != pMainFrame )
         pWndParent = pWndParent->GetParent( );

      // Set the new pane as the active view.
      if ( pWndParent )
         pMainFrame->SetActiveView( pZView );

   // pMainFrame->Invalidate( );

   // These are commented out to force Recalc on AttachSubtaskToPane
   // pSplitterWnd->RecalcLayout( );
   // pParentSplitterWnd->RecalcLayout( );
   // pMainFrame->RecalcLayout( );

   } while ( FALSE );  // end of: purist's goto
   }

   ZSubtask::SetSubtaskState( pZSubtask, zSUBTASK_STATE_SPLIT_IN_PROCESS, 0 );
   return( 0 );
}

//./ ADD NAME=AttachSubtaskToPane
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: AttachSubtaskToPane
//                                          07/18/2002 Modified: 12/04/2002
//
//  PURPOSE:   To a subtask to a frame pane.  If the pane already has an
//             associated subtask, delete that subtask.
//
//  PARAMETERS: vSubtask    - The subtask view which contains the splitter.
//              cpcSplitTag - The unique name of the splitter window.
//              cpcDlgTag   - Dialog tag of new subtask.
//              cpcWndTag   - Window tag of new subtask.
//              lPane       - 1 ==> right; 2 ==> down
//
//  RETURNS:    0 - Error locating window or creating new splitter
//            otw - View to window of new frame
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zVIEW  OPERATION
AttachSubtaskToPane( zVIEW   vSubtask,
                     zCPCHAR cpcSplitTag,
                     zCPCHAR cpcDlgTag,
                     zCPCHAR cpcWndTag,
                     zLONG   lPane )
{
   ASSERT( (lPane & ~0x00000003) == 0 );
   ZDrApp   *pApp = (ZDrApp *) AfxGetApp( );
   ZSubtask *pZSubtask;
   ZTask    *pZTask;

   if ( pApp == 0 || pApp->m_pDoc == 0 ||
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) )
   {
      AfxMessageBox( "AttachSubtaskToPane requires a valid Subtask view!" );
      return( 0 );
   }

   while ( ZSubtask::GetSubtaskState( pZSubtask ) &
                                         zSUBTASK_STATE_SPLIT_IN_PROCESS )
   {
      SysWait( 100 );
   }

   if ( pZSubtask->m_pZView == 0 )
   {
      AfxMessageBox( "SplitFrame cannot find any views!" );
      return( 0 );
   }

   if ( pZSubtask->m_pZView->m_pParentSplitterWnd == 0 )
   {
      AfxMessageBox( "SplitFrame cannot find a splitter!" );
      return( 0 );
   }

// TraceLineS( "AttachSubtaskToPane Initialization MutexLock ZDw: ", cpcSplitTag );
   SysMutexLock( vSubtask, "ZDw", 0, 0 );

   pZTask = pZSubtask->m_pZTask;
   ASSERT( mIs_hWnd( pZSubtask->m_pZView->m_hWnd ) );
   ASSERT( mIs_hWnd( pZSubtask->m_pZView->m_pParentSplitterWnd->m_hWnd ) );
   ASSERT( pZSubtask->m_pZView->m_eQuad != ZDrView::eUndef );

   ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
   if ( pZSubtaskMain && pZSubtaskMain->m_pZFWnd &&
        mIs_hWnd( pZSubtaskMain->m_pZFWnd->m_hWnd ) &&
        pZSubtaskMain->m_pZView &&
        pZSubtaskMain->m_pZView->m_pParentSplitterWnd &&
        mIs_hWnd( pZSubtaskMain->m_pZView->m_pParentSplitterWnd->m_hWnd ) )
   {
   // pZSubtaskMain->m_pZFWnd->ShowWindow( SW_HIDE );  a bit much
      pZSubtaskMain->m_pZView->m_pParentSplitterWnd->ShowWindow( SW_HIDE );
   }

#if 0
   ASSERT( lPane == 0 ||
           (lPane == 1 && (pZSubtask->m_pZView->m_eQuad == ZDrView::eLeft ||
                           pZSubtask->m_pZView->m_eQuad == ZDrView::eRight)) ||
           (lPane == 2 && (pZSubtask->m_pZView->m_eQuad == ZDrView::eTop ||
                           pZSubtask->m_pZView->m_eQuad == ZDrView::eBottom)) );
#endif

   int nRow = (pZSubtask->m_pZView->m_eQuad == ZDrView::eBottom) ? 1 : 0;
   int nCol = (pZSubtask->m_pZView->m_eQuad == ZDrView::eRight) ? 1 : 0;

   // Find the "other" pane.
   int nRow2 = (pZSubtask->m_pZView->m_eQuad == ZDrView::eTop) ? 1 : 0;
   int nCol2 = (pZSubtask->m_pZView->m_eQuad == ZDrView::eLeft) ? 1 : 0;
   ZDrView *pZViewOtherPane = (ZDrView *) pZSubtask->m_pZView->
                               m_pParentSplitterWnd->GetPane( nRow2, nCol2 );

   // Create the new subtask.
   zBOOL bFoundInDiscardPile = TRUE;
   ZSubtask *pZSubtaskNew =
            pZTask->FindSubtaskOnSplitterDiscardPile( cpcSplitTag,
                                                      cpcDlgTag, cpcWndTag );
   if ( pZSubtaskNew == 0 )
   {
      pZSubtaskNew = pZSubtask->m_pZTask->NewZWnd( 0, 0, vSubtask, 0, 2,
                                                   cpcDlgTag, cpcWndTag, 0 );
      if ( pZSubtaskNew )
      {
         pZSubtaskNew->m_csSplitTag = cpcSplitTag;
         bFoundInDiscardPile = FALSE;
      }
   }

   while ( pZSubtaskNew )  // purist's goto
   {

   pZSubtaskNew->m_ulSubtaskFlags |= zSUBTASK_SUBWND_SPLITTER_PANE;

   // Determine which pane is getting the new subtask.
   ZSubtask *pZSubtaskTemp;
   ZDrView *pZView;

   if ( lPane && pZSubtask->m_pZView->m_pParentSplitterWnd->m_pZSubtask2 &&
        (ZSubtask::GetSubtaskState( pZSubtask->m_pZView->m_pParentSplitterWnd->m_pZSubtask2 ) & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
   {
      pZSubtaskTemp = pZSubtask->m_pZView->m_pParentSplitterWnd->m_pZSubtask2;
      pZView = pZSubtaskTemp->m_pZView;
      pZSubtaskTemp->m_pZView = 0;
      pZSubtaskNew->m_pZView = pZView;
      pZSubtaskNew->m_pZView->m_pZSubtask = pZSubtaskNew;
      pZSubtaskNew->m_pZFWnd = pZSubtaskTemp->m_pZFWnd;
      ((ZDrFrame *) pZSubtaskNew->m_pZFWnd)->m_pZSubtask = pZSubtaskNew;
      pZSubtaskTemp->m_pZFWnd = 0;
      if ( pZSubtask->m_pZTask->m_pZSubtaskTop == pZSubtaskTemp )
         pZSubtask->m_pZTask->m_pZSubtaskTop = pZSubtaskNew;

      pZSubtaskNew->m_pZView->m_pParentSplitterWnd->m_pZSubtask2 = pZSubtaskNew;
      pZSubtaskTemp->m_ulSubtaskFlags &= ~zSUBTASK_SUBWND_SPLITTER_PANE;
      pZTask->PutSubtaskOnSplitterDiscardPile( pZSubtaskTemp );
   }
   else
   if ( lPane == 0 && pZSubtask->m_pZView->m_pParentSplitterWnd->m_pZSubtask1 &&
        (ZSubtask::GetSubtaskState( pZSubtask->m_pZView->m_pParentSplitterWnd->m_pZSubtask1 ) & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
   {
      pZSubtaskTemp = pZSubtask->m_pZView->m_pParentSplitterWnd->m_pZSubtask1;
      pZView = pZSubtaskTemp->m_pZView;
      pZSubtaskTemp->m_pZView = 0;
      pZSubtaskNew->m_pZView = pZView;
      pZSubtaskNew->m_pZView->m_pZSubtask = pZSubtaskNew;
      pZSubtaskNew->m_pZFWnd = pZSubtaskTemp->m_pZFWnd;
      ((ZDrFrame *) pZSubtaskNew->m_pZFWnd)->m_pZSubtask = pZSubtaskNew;
      pZSubtaskTemp->m_pZFWnd = 0;
      if ( pZSubtask->m_pZTask->m_pZSubtaskTop == pZSubtaskTemp )
         pZSubtask->m_pZTask->m_pZSubtaskTop = pZSubtaskNew;

      pZSubtaskNew->m_pZView->m_pParentSplitterWnd->m_pZSubtask1 = pZSubtaskNew;
      pZSubtaskTemp->m_ulSubtaskFlags &= ~zSUBTASK_SUBWND_SPLITTER_PANE;
      pZTask->PutSubtaskOnSplitterDiscardPile( pZSubtaskTemp );
   }
   else
   if ( (lPane == 0 && (pZSubtask->m_pZView->m_eQuad == ZDrView::eTop || pZSubtask->m_pZView->m_eQuad == ZDrView::eLeft)) ||
        (lPane != 0 && (pZSubtask->m_pZView->m_eQuad == ZDrView::eBottom || pZSubtask->m_pZView->m_eQuad == ZDrView::eRight)) )
   {
      ::MessageBox( 0, "Replacing primary subtask", "AttachSubtaskToPane", MB_OK );

      // Replace the current subtask.
      pZView = pZSubtask->m_pZView;
      pZSubtask->m_pZView = 0;
      pZView->m_pParentSplitterWnd->m_pZSubtask1 = pZSubtaskNew;
      CFrameWnd *pZFWnd = pZSubtaskNew->m_pZFWnd;
      pZSubtaskNew->m_pZFWnd = pZSubtask->m_pZFWnd;
      pZSubtask->m_pZFWnd = pZFWnd;
      pZTask->PutSubtaskOnSplitterDiscardPile( pZSubtask );
      nRow2 = nRow;
      nCol2 = nCol;
   }
   else
   if ( (lPane == 0 && (pZViewOtherPane->m_eQuad == ZDrView::eTop || pZViewOtherPane->m_eQuad == ZDrView::eLeft)) ||
        (lPane != 0 && (pZViewOtherPane->m_eQuad == ZDrView::eBottom || pZViewOtherPane->m_eQuad == ZDrView::eRight)) )
   {
      pZView = pZViewOtherPane;
      pZView->m_pParentSplitterWnd->m_pZSubtask2 = pZSubtaskNew;
      if ( pZViewOtherPane->m_pZSubtask )
      {
         // Replace the other subtask.
         pZSubtask = pZViewOtherPane->m_pZSubtask;
         pZSubtask->m_pZView = 0;
         CFrameWnd *pZFWnd = pZSubtaskNew->m_pZFWnd;
         pZSubtaskNew->m_pZFWnd = pZSubtask->m_pZFWnd;
         pZSubtask->m_pZFWnd = pZFWnd;
         pZTask->PutSubtaskOnSplitterDiscardPile( pZSubtask );
      }
   }
   else
   {
      ::MessageBox( 0, "Invalid pane position for Subtask", "AttachSubtaskToPane", MB_OK );
      break;
   }

   pZView->m_pZSubtask = pZSubtaskNew;
   pZSubtaskNew->m_pZView = pZView;
   pZSubtaskNew->RestartBuildWindow( 0 );
   if ( pZSubtaskNew->m_pZFWnd == 0 )
   {
      pZSubtaskNew->m_pZFWnd = new ZDrFrame( pZSubtaskNew );
      pZSubtaskNew->m_pZFWnd->LoadFrame( IDR_MAINFRAME );
   }

   if ( pZView != pZSubtaskNew->m_pZView )
   {
      pZSubtaskNew->m_pZView->m_pZSubtask = 0;
      pZSubtaskNew->m_pZView->DestroyWindow( );
      pZSubtaskNew->m_pZView = pZView;
   // pZSubtaskNew->m_pZView->SetParent( pZSubtaskNew->m_pZView->m_pParentSplitterWnd );
   // pZSubtaskNew->m_pZView->SetDlgCtrlID( pZSubtaskNew->m_pZView->m_pParentSplitterWnd->
   //                                       IdFromRowCol( nRow2, nCol2 ) );
      pZSubtaskNew->m_pZView->ShowWindow( SW_SHOW );
   }

#if 0
   if ( pZSubtaskNew )
   {
      if ( pZSubtaskMain == 0 ||
           pZSubtaskMain->m_pZView->m_pParentSplitterWnd !=
                               pZSubtaskNew->m_pZView->m_pParentSplitterWnd )
      pZSubtaskNew->m_pZView->m_pParentSplitterWnd->RecalcLayout( );
   }
#endif

// pZSubtaskMain->m_pZFWnd->UpdateWindow( );
// pZSubtaskNew->m_pZView->UpdateWindow( );
// pZSubtask->m_pZView->UpdateWindow( );
//?pZSubtaskNew->m_pZView->Repaint( );
// pZSubtask->m_pZView->Repaint( );

   break;
   } // end of: purist's goto

// GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
   if ( pZSubtask->m_pZTask->m_pZFWndHidden == 0 )
      pZSubtask->RecalcLayout( 0 );

   SysMutexUnlock( vSubtask, "ZDw", 0 );
// TraceLineS( "AttachSubtaskToPane Termination MutexUnlock: ", cpcSplitTag );
   return( pZSubtaskNew ? pZSubtaskNew->m_vDialog : 0 );
}

void
FlipFrameSubtaskView( ZSubtask *pZSubtask, ZSubtask *pZSubtaskNew )
{
#ifdef DEBUG_ALL
   TraceLine( "FlipFrameSubtaskView (before) ZSubtask1: %d  ZFWnd: %d  ZView: %d   "
              "Subtask2: %d  ZFWnd: %d  ZView: %d",
              pZSubtask->m_ulIdNbr, ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_ulIdNbr,
              pZSubtask->m_pZView->m_ulIdNbr,
              pZSubtaskNew->m_ulIdNbr, ((ZDrFrame *) pZSubtaskNew->m_pZFWnd)->m_ulIdNbr,
              pZSubtaskNew->m_pZView->m_ulIdNbr );
#endif

   CFrameWnd *pZFWnd = pZSubtaskNew->m_pZFWnd;
   pZSubtaskNew->m_pZFWnd = pZSubtask->m_pZFWnd;
   pZSubtask->m_pZFWnd = pZFWnd;
   ((ZDrFrame *) pZSubtaskNew->m_pZFWnd)->m_pZSubtask = pZSubtaskNew;
   ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_pZSubtask = pZSubtask;
   pZSubtaskNew->m_pZView->SetParent( pZSubtaskNew->m_pZFWnd );
   pZSubtask->m_pZView->SetParent( pZSubtask->m_pZFWnd );

#ifdef DEBUG_ALL
   TraceLine( "FlipFrameSubtaskView (after) ZSubtask1: %d  ZFWnd: %d  ZView: %d   "
              "Subtask2: %d  ZFWnd: %d  ZView: %d",
              pZSubtask->m_ulIdNbr, ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_ulIdNbr,
              pZSubtask->m_pZView->m_ulIdNbr,
              pZSubtaskNew->m_ulIdNbr, ((ZDrFrame *) pZSubtaskNew->m_pZFWnd)->m_ulIdNbr,
              pZSubtaskNew->m_pZView->m_ulIdNbr );
#endif

   pZSubtask->m_pZView->ShowWindow( SW_HIDE );
   pZSubtaskNew->m_pZView->ShowWindow( SW_SHOW );
   pZSubtaskNew->m_pZFWnd->SetActiveView( pZSubtaskNew->m_pZView );

   pZSubtaskNew->RecalcLayout( 0 );
   pZSubtaskNew->m_pZFWnd->RecalcLayout( );
}

//./ ADD NAME=HideGlobalView
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: HideGlobalView
//                                          05/28/2003 Modified: 05/28/2003
//
//  PURPOSE:   To make the current subtask global ... hide it and replace
//             it with the specified subtask.  This permits
//
//  PARAMETERS: vSubtask    - The subtask view for the window which is to
//                            be made global.
//              cpcDlgTag   - Dialog tag of new subtask.
//              cpcWndTag   - Window tag of new subtask.
//
//  RETURNS:   -1 - Error locating window
//            otw - View to new subtask
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zVIEW  OPERATION
HideGlobalView( zVIEW   vSubtask,
                zCPCHAR cpcDlgTag,
                zCPCHAR cpcWndTag )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZSubtask *pZSubtaskNew;

   // TraceLineS( "HideGlobalView Initialization MutexLock ZDw: ", cpcWndTag );
      SysMutexLock( vSubtask, "ZDw", 0, 0 );

      if ( pZSubtask->m_pZSubtaskGlobal &&
           (cpcDlgTag == 0 || *cpcDlgTag == 0) )
      {
         pZSubtaskNew = pZSubtask->m_pZSubtaskGlobal;
         pZSubtask->m_pZSubtaskGlobal = 0;
         pZSubtask->m_pZTask->m_pZSubtaskTop = pZSubtaskNew;
         pZSubtaskNew->m_pZSubtaskProxy = 0;

         // Replace the current subtask.
         FlipFrameSubtaskView( pZSubtask, pZSubtaskNew );

         pZSubtask->PostClose( 0 );
         SysMutexUnlock( vSubtask, "ZDw", 0 );
      // TraceLineS( "HideGlobalView Quick Term MutexLock ZDw: ", cpcWndTag );
         return( pZSubtaskNew->m_vDialog );
      }

      ASSERT( pZSubtask->m_pZSubtaskGlobal == 0 );
      ASSERT( pZSubtask->m_pZTask->m_pZSubtaskTop == pZSubtask );

      // Create the new subtask.
      pZSubtaskNew = pZSubtask->m_pZTask->NewZWnd( 0, 0, vSubtask, 0, 2,
                                                   cpcDlgTag, cpcWndTag, 0 );
      if ( pZSubtaskNew->m_pZFWnd == 0 )
      {
         pZSubtaskNew->m_pZFWnd = new ZDrFrame( pZSubtaskNew );
         pZSubtaskNew->m_pZFWnd->LoadFrame( IDR_MAINFRAME );
      }

      do  // purist's goto
      {
         pZSubtask->m_pZTask->m_pZSubtaskTop = pZSubtaskNew;
         pZSubtaskNew->m_pZSubtaskGlobal = pZSubtask;
         pZSubtask->m_pZSubtaskProxy = pZSubtaskNew;

         pZSubtaskNew->m_ulSubtaskFlags |= zSUBTASK_SUBWND_SPLITTER_PANE;
         pZSubtaskNew->RestartBuildWindow( 0 );
         pZSubtaskNew->m_ulSubtaskFlags &= ~zSUBTASK_SUBWND_SPLITTER_PANE;
         pZSubtaskNew->m_pZView->m_pZSubtask = pZSubtaskNew;
         pZSubtask->m_pZView->m_pZSubtask = pZSubtask;

         // Replace the current subtask.
         FlipFrameSubtaskView( pZSubtask, pZSubtaskNew );
         pZSubtaskNew->m_pZFWnd->ActivateFrame( );

      } while ( FALSE );  // end of: purist's goto

      SysMutexUnlock( vSubtask, "ZDw", 0 );
   // TraceLineS( "HideGlobalView Termination MutexLock ZDw: ", cpcWndTag );

      return( pZSubtaskNew->m_vDialog );
   }

   return( 0 );
}

//./ ADD NAME=SwitchView
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SwitchView
//                                          06/12/2002 Modified: 06/12/2002
//
//  PURPOSE:   To split a window into frames.
//
//  PARAMETERS: vSubtask    - The subtask view for the window which is to
//                            be split.
//
//  RETURNS:   -1 - Error locating window
//            otw - View to new subtask
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG  OPERATION
SwitchView( zVIEW   vSubtask,
            zLONG   lViewId )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
#if 0
      if ( pZSubtask->m_pZSubtaskSplitter )
         TraceLineS( "SwitchView tag: ", *(pZSubtask->m_pzsWndTag) );

      switch ( nViewNbr )
      {
         case 1:
            pZSubtask->m_pSplitterWnd->ShowView( pZSubtask->m_pSplitterWnd->m_nView1 );
            break;

         case 2:
            pZSubtask->m_pSplitterWnd->ShowView( pZSubtask->m_pSplitterWnd->m_nView2 );
            break;

         case 3:
            pZSubtask->m_pSplitterWnd->ShowView( pZSubtask->m_pSplitterWnd->m_nView3 );
            break;
      }
#endif
   }

   return( -1 );
}

//./ ADD NAME=CloseFrame
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CloseFrame
//                                          12/04/2002 Modified: 12/04/2002
//
//  PURPOSE:   To close the frame associated with the specified subtask.
//
//  PARAMETERS: vSubtask  - The subtask view for the window
//              vSubtaskDelete  - The subtask view for the window to close.
//
//  RETURNS:    -1  - Error locating window
//               0  - Close was successful
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CloseFrame( zVIEW  vSubtask )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
//#ifdef DEBUG_ALL
      TraceLine( "CloseFrame: %s.%s",
                 *(pZSubtask->m_pzsDlgTag), *(pZSubtask->m_pzsWndTag) );
//#endif

      ZTask *pZTask = pZSubtask->m_pZTask;
   //?MSG   msg;

      if ( pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE )
      {
      //?pZSubtask->PostClose( 0 );
         pZSubtask->m_pZFWnd->SendMessage( g_uMsgPostedClose,
                                           0, (zLONG) pZSubtask );

      //?while ( ::PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE ) )
      //?   pZTask->m_pZDrApp->PumpMessage( );
      }
      else
      {
         zKZWDLGXO_Act_DEF ActDef;
         ActDef.Type = zWAB_ExitDialogTask;
         fnProcessReturn( pZSubtask, ActDef );
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=RefreshParentSubwindows
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RefreshParentSubwindows
//                                              10/30/98 Modified: 10/30/98
//
//  PURPOSE:   To refresh (MapFromOI) all subwindows of a parent window,
//             (optionally including the parent window).
//
//  PARAMETERS: vSubtask  - The subtask view for the parent window
//              bRefreshParent - TRUE - refresh the parent window
//                               FALSE - do NOT refresh the parent window
//
//  RETURNS:    -1  - Error locating window
//               0  - the refresh was successful
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
RefreshParentSubwindows( zVIEW  vSubtask,
                         zBOOL  bRefreshParent )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In RefreshParentSubwindows ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( bRefreshParent )
         pZSubtask->MapFromOI( );

      zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      while ( lItemCnt )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
              g_pSubtaskList[ lItemCnt ].lTID == pZSubtask->m_lTID &&
              g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == pZSubtask )
         {
            RefreshAllSubwindows( g_pSubtaskList[ lItemCnt ].pZSubtask->m_vDialog );
         }

         lItemCnt--;
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=RefreshAllSubwindows
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: RefreshAllSubwindows
//                                              04/14/93 Modified: 10/30/98
//
//  PURPOSE:   To refresh (MapFromOI) all subwindows of a parent window,
//             (including the parent window).
//
//  PARAMETERS: vSubtask  - The subtask view for the parent window
//
//  RETURNS:    -1  - Error locating window
//               0  - the refresh was successful
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RefreshAllSubwindows( zVIEW  vSubtask )
{
#ifdef DEBUG_ALL
   TraceLineI( "In RefreshAllSubwindows ", (zLONG) vSubtask );
#endif

   return( RefreshParentSubwindows( vSubtask, TRUE ) );
}

//./ ADD NAME=SetWindowCaptionTitle
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetWindowCaptionTitle
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set the caption and title for a window
//
//  PARAMETERS: vSubtask    - The subtask view for the window whose caption
//                            and title are to be set. This is usually the
//                            subtask passed into the dialog operation.
//              cpcCaption  - Caption for window (may be NULL if only
//                            the title is being set).
//              cpcTitle    - Title for window (may be NULL if only
//                            the caption is being set).
//
//  RETURNS:    0 - Caption and title set
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
SetWindowCaptionTitle( zVIEW   vSubtask,
                       zCPCHAR cpcCaption,
                       zCPCHAR cpcTitle )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      pZSubtask->SetCaptionTitle( cpcCaption, cpcTitle, TRUE );
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=ActivateWindow
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ActivateWindow
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set focus to the appropriate window
//
//  PARAMETERS: vSubtask - The subtask view for the window which is to
//                         receive focus.
//
//  RETURNS:    0 - Focus set to the specified window
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ActivateWindow( zVIEW vSubtask )
{
// ZSubtask    *pZSubtask;
   ZFindDialog fd;

   fd.pvDialog = SysGetHandleFromPointer( vSubtask );
   fd.DlgN[ 0 ] = 0;
   fd.WndN[ 0 ] = 0;
   if ( FindGlobalDialogWindow( &fd ) && mIs_hWnd( fd.hWnd ) )
   {
   // MSG   msg;

      PumpMessages( vSubtask );

      if ( mIs_hWndEnabled( fd.hWnd ) == FALSE )
         EnableWindow( fd.hWnd, TRUE );

      if ( ::IsIconic( fd.hWnd ) ||
         mIs_hWndVisible( fd.hWnd ) == FALSE )
      {
         ::ShowWindow( fd.hWnd, SW_SHOWNORMAL );
         ::SetWindowPos( fd.hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREPOSITION );
      }

      PumpMessages( vSubtask );
   // while ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
   // {
   //    ::TranslateMessage( &msg );
   //    ::DispatchMessage( &msg );
   // }

   // ::SetFocus( fd.hWnd );
      ::PostMessage( fd.hWnd, zZEIDON_POSTED_ACTIVATE, 0, 0 );

   // TraceLineX( "ActivateWindow: ", (zLONG) fd.hWnd );
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=PutWindowOnTop
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PutWindowOnTop
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To bring a window to the top of the z-order
//
//  PARAMETERS: vSubtask    - The subtask view for the window which is to
//                            get to the top of the z-order
//
//  RETURNS:    0 - Window brought to the top of the z-order
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PutWindowOnTop( zVIEW vSubtask )
{
   ZFindDialog fd;

   fd.pvDialog = SysGetHandleFromPointer( vSubtask );
   fd.DlgN[ 0 ] = 0;
   fd.WndN[ 0 ] = 0;
   if ( FindGlobalDialogWindow( &fd ) && mIs_hWnd( fd.hWnd ) )
   {
      ::SetWindowPos( fd.hWnd, HWND_TOPMOST, 0, 0, 0, 0,
                      SWP_NOSIZE | SWP_NOMOVE | SWP_ASYNCWINDOWPOS );
      ::SetWindowPos( fd.hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                      SWP_NOSIZE | SWP_NOMOVE | SWP_ASYNCWINDOWPOS );
      ::FlashWindow( fd.hWnd, FALSE );
//    TraceLineX( "PutWindowOnTop: ", (zLONG) fd.hWnd );
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=AcquireTwainImage
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: AcquireTwainImage
//                                              08/20/01 Modified: 08/20/01
//
//  PURPOSE:   This routine acquires an image from a twain compliant device
//             and saves it to a file.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window
//              cpcFileName - Specified name of the file in which to store
//                            the image.  If null or empty, a temporary file
//                            name is generated.
//              pchReturnFileName - Name of the file in which to store
//                            the image.
//              nMaxFileNameLth -  Maximum length of (return) image file name.
//
//  RETURNS:    0 - Image acquired
//             -4 - Cannot acquire image
//             -3 - Cannot select source
//             -2 - Error loading Twain driver
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
AcquireTwainImage( zVIEW   vSubtask,
                   zCPCHAR cpcFileName,
                   zPCHAR  pchReturnFileName,
                   zSHORT  nMaxFileNameLth )
{
   ZSubtask *pZSubtask;
   zSHORT nRC = -1;

#ifdef DEBUG_ALL
   TraceLineI( "In AcquireTwainImage ", (zLONG) vSubtask );
#endif

   SysMutexLock( vSubtask, "ZDrTwain", 0, 0 );
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      pZSubtask->m_pZTask->ReleaseTwain( 0 );
      if ( pZSubtask->m_pZTask && pZSubtask->m_pZFWnd )
      {
         mDeleteInit( pZSubtask->m_pZTask->m_pzsTwainFileName );
         if ( cpcFileName && *cpcFileName )
         {
            strncpy_s( pchReturnFileName, nMaxFileNameLth, cpcFileName, nMaxFileNameLth - 1 );
         }
         else
         {
            zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
            zCHAR  szWorkString[ zMAX_FILENAME_LTH ];
            LPAPP pApp;

            szZeidonPath[ 0 ] = 0;
            if ( SfGetApplicationForSubtask( &pApp,
                                             pZSubtask->m_vDialog ) == 0 &&
                 pApp )
            {
               strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szLocalDir );
            }

            if ( szZeidonPath[ 0 ] == 0  )
               strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), "c:\\temp\\" );

            SysAppendcDirSep( szZeidonPath );
            SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
            GenerateUniqueFileName( pchReturnFileName, nMaxFileNameLth, szWorkString, "Twain", "bmp" );
         }

         pZSubtask->m_pZTask->m_pzsTwainFileName = new CString( pchReturnFileName );
         if ( pZSubtask->m_pZTask->InitTwain( pZSubtask->m_pZFWnd ) )
         {
            if ( pZSubtask->m_pZTask->SelectSource( ) )
            {
               if ( pZSubtask->m_pZTask->Acquire( TWCPP_ANYCOUNT ) )
               {
                  nRC = 0;
               }
               else
               {
                  pZSubtask->m_pZTask->ReleaseTwain( pZSubtask->m_pZFWnd );
                  nRC = -4;
               }
            }
            else
            {
               pZSubtask->m_pZTask->ReleaseTwain( pZSubtask->m_pZFWnd );
               nRC = -3;
            }
         }
         else
         {
            pZSubtask->m_pZTask->ReleaseTwain( pZSubtask->m_pZFWnd );
            nRC = -2;  // error loading Twain driver
         }
      }
      else
         nRC = -5;  // no window to associate with Twain
   }

   SysMutexUnlock( vSubtask, "ZDrTwain", 0 );
   return( -1 );
}

//./ ADD NAME=BuildWindowImmediate
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: BuildWindowImmediate
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   This routine builds a window using a subtask view
//             previously obtained from a CreateWindowSubtask operation
//
//  PARAMETERS: vSubtask  - The subtask view for the current window
//              vDialog   - The subtask view for the new dialog
//              nWindowBehavior - The Window Action Behavior requested
//
//  RETURNS:    0 - Focus set to the specified window
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
BuildWindowImmediate( zVIEW  vSubtask,
                      zVIEW  vDialog,
                      zSHORT nWindowBehavior )
{
   zPCHAR pchDialogName;
   zPCHAR pchWindowName;

   if ( GetAddrForAttribute( (zPCHAR *) &pchDialogName,
                             vDialog, szlDlg, szlTag ) == 0 &&
        GetAddrForAttribute( (zPCHAR *) &pchWindowName,
                             vDialog, szlWnd, szlTag ) == 0 )
   {
      ZSubtask *pZSubtask;
      ZSubtask *pZSubtaskP;
      zBOOL bModal = FALSE;

      if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) != 0 )
         return( -2 );

      if ( nWindowBehavior == zWAB_ReturnToTopWindowWithRefresh )
         pZSubtaskP = 0;
      else
      {
         if ( nWindowBehavior == zWAB_ReplaceWindowWithModalWindow )
         {
            pZSubtaskP = pZSubtask->m_pZParent;
            bModal = TRUE;
         }
         else
         if ( nWindowBehavior == zWAB_StartModalSubwindow )
         {
            pZSubtaskP = pZSubtask;
            bModal = TRUE;
         }
         else
         {
            TraceLineI( "(drvr) BuildWindowImmediate invalid "
                        "Window Action Behavior: ", nWindowBehavior );
            return( -1 );
         }
      }

      pZSubtask->m_pZTask->NewZWnd( pZSubtaskP, pZSubtaskP, vSubtask, vDialog,
                                    bModal, pchDialogName, pchWindowName );
      return( 0 );
   }

   return( -1 );
}

// This operation expects the pfdReturn DlgN and WndN values or the
// vSubtask value to be set.
zBOOL
FindGlobalDialogWindow( ZFindDialog *pfd )
{
   // Protect this with a semaphore!!!
   SysMutexLock( g_vRegister, "ZDr", 0, 0 );
// TraceLineS( "FindGlobalDialogWindow Lock Mutex ZDr", "" );

   zPVOID pvDialog = pfd->pvDialog;
   zLONG  lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   while ( lItemCnt > 0 )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
      {
         if ( (pvDialog && pvDialog == g_pSubtaskList[ lItemCnt ].pvDialog) ||
              (pvDialog == 0 &&
               zstrcmp( pfd->DlgN, g_pSubtaskList[ lItemCnt ].DlgN ) == 0 &&
               (pfd->WndN[ 0 ] == 0 ||
                zstrcmp( pfd->WndN, g_pSubtaskList[ lItemCnt ].WndN ) == 0)) )
         {
            *pfd = g_pSubtaskList[ lItemCnt ];

         // TraceLineS( "FindGlobalDialogWindow Unlock Mutex ZDr", "" );
            SysMutexUnlock( g_vRegister, "ZDr", 0 );

            return( TRUE );
         }
      }

      lItemCnt--;
   }

#ifdef DEBUG_ALL
   lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   if ( pfd->pvDialog )
   {
      TraceLineX( "FindGlobalDialogWindow could NOT find: ",
                  (zLONG) pfd->pvDialog );
      TraceLineI( "FindGlobalDialogWindow ItemCnt: ", lItemCnt );
      while ( lItemCnt > 0 )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
         {
            TraceLineX( "  vSubtask: ",
                        (zLONG) g_pSubtaskList[ lItemCnt ].pvDialog );
         }

         lItemCnt--;
      }
   }
   else
   {
      TraceLineS( "FindGlobalDialogWindow could NOT find Dlg: ", pfd->DlgN );
      TraceLineS( "                                      Wnd: ", pfd->WndN );
      TraceLineI( "FindGlobalDialogWindow ItemCnt: ", lItemCnt );
      while ( lItemCnt > 0 )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
         {
            TraceLineS( "   Dlg: ", g_pSubtaskList[ lItemCnt ].DlgN );
            TraceLineS( "   Wnd: ", g_pSubtaskList[ lItemCnt ].WndN );
         }

         lItemCnt--;
      }
   }
#endif

// TraceLineS( "FindGlobalDialogWindow Unlock Mutex ZDr", "" );
   SysMutexUnlock( g_vRegister, "ZDr", 0 );

// ::MessageBox( 0, "Could not find app", "FindGlobalDialogWindow", MB_OK );
   return( FALSE );
}

//./ ADD NAME=FindDialogWindow
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: FindDialogWindow
//                                              11/21/97 Modified: 11/21/97
//
//  PURPOSE:   To determine if a dialog/window exists ... if it exists,
//             the view to the subtask is returned.
//
//  PARAMETERS: vSubtask  - A subtask view within the current application.
//                          If vSubtask is null, FindDialogWindow will
//                          attempt to find the dialog/window using the
//                          global list of subtasks.
//              cpcDlgTag - Name of the dialog whose subtask is sought
//              cpcWndTag - Name of the window whose subtask is sought
//                          (may be null in which case the most recently
//                          constructed window within the dialog is returned)
//
//  RETURNS:   0  - the dialog/window was not located
//           vSubtask  - view to the dialog/window that was located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zVIEW OPERATION
FindDialogWindow( zVIEW   vSubtask,
                  zCPCHAR cpcDlgTag,
                  zCPCHAR cpcWndTag )
{
#ifdef DEBUG_ALL
   TraceLineI( "In FindDialogWindow ", (zLONG) vSubtask );
#endif

   ZSubtask *pZSubtask;
   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      // Run through the list of windows to find the one with the
      // specified tags.
      zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      for ( zLONG lPos = 0; lPos < lItemCnt; lPos++ )
      {
         if ( g_pSubtaskList[ lPos ].pZSubtask &&
              g_pSubtaskList[ lPos ].lTID == pZSubtask->m_lTID &&
              (g_pSubtaskList[ lPos ].ulSubtaskState &
                  zSUBTASK_STATE_WELL_FORMED) == zSUBTASK_STATE_WELL_FORMED &&
              (zstricmp( g_pSubtaskList[ lPos ].DlgN, cpcDlgTag ) == 0 ||
               zstricmp( g_pSubtaskList[ lPos ].pZSubtask->m_pZTask->m_szInitialDlgName, cpcDlgTag ) == 0) )
         {
            return( (zVIEW) SysGetPointerFromHandle( g_pSubtaskList[ lPos ].pvDialog ) );
         }
      }
   }

   if ( vSubtask == 0 )
   {
      ZFindDialog fd;

      fd.pvDialog = 0;
      strcpy_s( fd.DlgN, zsizeof( fd.DlgN ), cpcDlgTag );
      if ( cpcWndTag )
         strcpy_s( fd.WndN, zsizeof( fd.WndN ), cpcWndTag );
      else
         fd.WndN[ 0 ] = 0;

      if ( FindGlobalDialogWindow( &fd ) )
      {
         vSubtask = (zVIEW) SysGetPointerFromHandle( fd.pvDialog );
      }

      return( vSubtask );
   }

   return( 0 );
}

//./ ADD NAME=GetSubtaskForWindowName
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetSubtaskForWindowName
//                                              04/14/93 Modified: 04/14/93
//
//  PURPOSE:   To get the vSubtask for a given window name
//
//  PARAMETERS: vSubtask    - The subtask view for a window in the same
//                            task as desired window ... if this is null,
//                            the subtask for the current window will be
//                            returned.
//              pvSubtaskReturn - view to the dialog/window that was located
//              cpcWndTag - Name of the window whose subtask is sought ...
//                          ignored if vSubtask is null.
//
//  RETURNS:    -1  - Error locating window
//               0  - the window was located
//              otw - the number of additional windows in the task with the
//                    same name (the first window subtask was returned).
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GetSubtaskForWindowName( zVIEW   vSubtask,  // current subtask within task
                         zVIEW   *pvSubtaskReturn,
                         zCPCHAR cpcWndTag )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In GetSubtaskForWindowName ", (zLONG) vSubtask );
#endif

   *pvSubtaskReturn = 0;
   if ( vSubtask == 0 )
   {
      HWND  hWnd = GetActiveWindow( );

      // Protect this with a semaphore!!!
      SysMutexLock( vSubtask, "ZDr", 0, 0 );
   // TraceLineS( "GetSubtaskForWindowName Lock Mutex ZDr", "" );

      zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      while ( lItemCnt > 0 )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
              g_pSubtaskList[ lItemCnt ].hWnd == hWnd )
         {
            *pvSubtaskReturn = (zVIEW)
              SysGetPointerFromHandle( g_pSubtaskList[ lItemCnt ].pvDialog );

         // TraceLineS( "GetSubtaskForWindowName Unlock Mutex ZDr", "" );
            SysMutexUnlock( vSubtask, "ZDr", 0 );

            return( 0 );
         }

         lItemCnt--;
      }

   // TraceLineS( "GetSubtaskForWindowNameB Unlock Mutex ZDr", "" );
      SysMutexUnlock( vSubtask, "ZDr", 0 );
   }
   else
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zSHORT nCnt = -1;

      // Protect this with a semaphore!!!
      SysMutexLock( vSubtask, "ZDr", 0, 0 );
   // TraceLineS( "GetSubtaskForWindowName2 Lock Mutex ZDr", "" );

      // Run through the list of windows to find the one with the
      // specified tags.
      zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      while ( lItemCnt )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
              g_pSubtaskList[ lItemCnt ].lTID == pZSubtask->m_lTID &&
              zstricmp( g_pSubtaskList[ lItemCnt ].WndN, cpcWndTag ) == 0 )
         {
            if ( ((g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0) &&
                 (((g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_WELL_FORMED) == zSUBTASK_STATE_WELL_FORMED) ||
                  ((g_pSubtaskList[ lItemCnt ].ulSubtaskState & 0x000000ff) >= zSUBTASK_STATE_PREBUILD_PRECODE)) )
            {
               nCnt++;
               if ( *pvSubtaskReturn == 0 )
               {
                  *pvSubtaskReturn = (zVIEW)
                  SysGetPointerFromHandle( g_pSubtaskList[ lItemCnt ].pvDialog );
               }
            }
         }

         lItemCnt--;
      }

   // TraceLineS( "GetSubtaskForWindowName2 Unlock Mutex ZDr", "" );
      SysMutexUnlock( vSubtask, "ZDr", 0 );
      return( nCnt );
   }

   return( -1 );
}

//./ ADD NAME=GetDlgWndCtrlNames
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetDlgWndCtrlNames
//                                              06/02/93 Modified: 06/02/93
//
//  PURPOSE:   To get the tag names of the Dialog, Window, and Control
//             that currently have the focus.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window.
//              pchDlgName  - Return name of the dialog
//              pchWndName  - Return name of the window
//              pchCtrlName - Return name of the control currently with focus.
//                            This is null if no control has focus.
//
//  RETURNS:    -1  - Error locating window
//               0  - success
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetDlgWndCtrlNames( zVIEW  vSubtask,
                    zPCHAR pchDlgTag,
                    zPCHAR pchWndTag,
                    zPCHAR pchCtrlTag )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In GetDlgWndCtrlNames ", (zLONG) vSubtask );
#endif

   *pchDlgTag = *pchWndTag = *pchCtrlTag = 0;
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      strcpy_s( pchDlgTag, zTAG_LTH, *(pZSubtask->m_pzsDlgTag) );
      strcpy_s( pchWndTag, zTAG_LTH, *(pZSubtask->m_pzsWndTag) );
      if ( pZSubtask->m_pzmaWithFocus )
         strcpy_s( pchCtrlTag, zTAG_LTH, *(pZSubtask->m_pzmaWithFocus->m_pzsTag) );

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=GetWindowState
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowState
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get enable/visible ... for a window
//
//  PARAMETERS: vSubtask    - The subtask view for the window.
//              lStatusType - Status type to get:
//                                  zWINDOW_STATUS                    0
//                                  zWINDOW_STATUS_ENABLED            1
//                                  zWINDOW_STATUS_VISIBLE            2
//
//  RETURNS:    -1  - Error locating window
//              otw - original state of the window
//                    except for zWINDOW_STATUS which returns the current
//                    state of the window as one of the following (note:
//                    these states are guaranteed to be in ascending order):
//
//                                  zSUBTASK_INIT
//                                  zSUBTASK_IN_CONSTRUCTION
//                                  zSUBTASK_PREBUILD_PRECODE
//                                  zSUBTASK_PREBUILD_POSTCODE
//                                  zSUBTASK_POSTBUILD_PRECODE
//                                  zSUBTASK_POSTBUILD_POSTCODE
//                                  zSUBTASK_WELL_FORMED (complete)
//                                  zSUBTASK_DELETE (pending)
//                                  zSUBTASK_DELETE_WELL_FORMED (pending)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
GetWindowState( zVIEW  vSubtask,
                zLONG  lStatusType )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In GetWindowState ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( lStatusType == zWINDOW_STATUS_ENABLED )
      {
         if ( pZSubtask->m_nDisabled == 0 )
            return( TRUE );
         else
            return( FALSE );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_VISIBLE )
      {
         if ( pZSubtask->m_nInvisible >= 0 )
            return( TRUE );
         else
            return( FALSE );
      }
      else
      if ( lStatusType == zWINDOW_STATUS )
         return( ZSubtask::GetSubtaskState( pZSubtask ) );
   }

   return( -1 );
}

//./ ADD NAME=SetWindowState
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetWindowState
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set enable/visible/check... for a window.  Note that to
//             get/set a window's size or position, use GetCtrlSize, etc.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the window.
//              lStatusType - Status type to be set:
//                               zWINDOW_STATUS_ENABLED                 1
//                               zWINDOW_STATUS_VISIBLE                 2
//                               zWINDOW_STATUS_MINIMIZE                3
//                               zWINDOW_STATUS_MAXIMIZE                4
//                               zWINDOW_STATUS_RESTORE                 5
//                               zWINDOW_STATUS_ASYNC                   6
//                               zWINDOW_STATUS_UPDATE                  7
//                               zWINDOW_STATUS_SETFOCUS                8
//                               zWINDOW_STATUS_DESTROY                 9
//                               zWINDOW_STATUS_SHOW_PARENT            10
//                               zWINDOW_STATUS_STOP_EVENTS            11
//                               zWINDOW_STATUS_SET_NO_ERROR_PROMPT    12
//                               zWINDOW_STATUS_ENABLED_VIEW           13
//                               zWINDOW_STATUS_INVALIDATE             14
//                               zWINDOW_STATUS_NO_REFRESH             15
//              lValue      - TRUE  - enable, make visible, ...
//                          - FALSE - disable, make invisible, ...
//                          - RGB Color
//
//  RETURNS:    -1  - Error locating window
//              otw - original state of the window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zLONG OPERATION
SetWindowState( zVIEW   vSubtask,
                zLONG   lStatusType,
                zULONG  ulValue )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In SetWindowState ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZFWnd && mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
   {
      if ( lStatusType == zWINDOW_STATUS_ENABLED_VIEW )
      {
         if ( pZSubtask->m_pZView && mIs_hWnd( pZSubtask->m_pZView->m_hWnd ) )
         {
            pZSubtask->m_pZView->EnableWindow( ulValue ? TRUE : FALSE );
            return( 0 );
         }

         return( -1 );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_ENABLED )
         return( pZSubtask->EnableWindow( (zBOOL) ulValue ) );
      else
      if ( lStatusType == zWINDOW_STATUS_SHOW_PARENT )
      {
         if ( ulValue )
         {
            pZSubtask->RecalcLayout( (HWND) ulValue );
            ::UpdateWindow( (HWND) ulValue );
         }
         else
         {
            ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
            HWND hWndParent;
            HWND hWnd = pZSubtask->m_pZView->m_hWnd;

            if ( pZSubtaskMain )
               pZSubtask->m_pZTask->m_pZFWndHidden = pZSubtaskMain->m_pZFWnd;
            else
               pZSubtask->m_pZTask->m_pZFWndHidden = pZSubtask->m_pZFWnd;

            pZSubtask->m_pZTask->m_bLocked =
                      pZSubtask->m_pZTask->m_pZFWndHidden->LockWindowUpdate( );
            while ( mIs_hWnd( hWnd ) )
            {
               hWndParent = ::GetParent( hWnd );
               if ( hWndParent == pZSubtask->m_pZTask->m_pZFWndHidden->m_hWnd )
               {
                  ::ShowWindow( hWnd, SW_HIDE );
                  return( (zLONG) hWnd );
               }

               hWnd = hWndParent;
            }
         }

         return( 0 );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_VISIBLE )
      {
         zSHORT nVisible = pZSubtask->m_nInvisible;
         if ( nVisible < 0 )
            nVisible = 0;
         else
            nVisible = 1;

         if ( ulValue )
         {
            if ( pZSubtask->m_nInvisible < 0 )
               pZSubtask->m_nInvisible++;
         }
         else
         {
            if ( pZSubtask->m_nInvisible <= 0 )
               pZSubtask->m_nInvisible--;
            else
               pZSubtask->m_nInvisible = -1;
         }

         if ( pZSubtask->m_nInvisible < 0 )
            pZSubtask->m_pZFWnd->ShowWindow( SW_HIDE );
         else
            pZSubtask->m_pZFWnd->ShowWindow( SW_SHOW );

         return( nVisible );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_MINIMIZE )
      {
         pZSubtask->m_pZFWnd->ShowWindow( SW_MINIMIZE );
         return( 0 );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_MAXIMIZE )
      {
         pZSubtask->m_pZFWnd->ShowWindow( SW_MAXIMIZE );
         return( 0 );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_RESTORE )
      {
         pZSubtask->m_pZFWnd->ShowWindow( SW_RESTORE );
         pZSubtask->m_pZFWnd->SetForegroundWindow( );
         return( 0 );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_SETFOCUS )
      {
         ZSubtask::SetSubtaskState( pZSubtask, 0, zSUBTASK_STATE_ON_RETURN );
         pZSubtask->m_pZFWnd->SetForegroundWindow( );
         return( 0 );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_INVALIDATE )
      {
         if ( pZSubtask->m_pZFWnd &&
              mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         {
            pZSubtask->m_pZFWnd->Invalidate( );
         }
      }
      else
      if ( lStatusType == zWINDOW_STATUS_UPDATE )
      {
         if ( pZSubtask->m_pZFWnd &&
              mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         {
#if 0
            if ( ulValue )
            {
               if ( ulValue == -1 )
               {
                  if ( pZSubtask->m_bLocked )
                     pZSubtask->m_pZFWnd->UnlockWindowUpdate( );

                  return( 0 );
               }
               else
               if ( ulValue == 1 )
               {
                  if ( pZSubtask->m_bLocked == FALSE )
                   pZSubtask->m_bLocked =
                                    pZSubtask->m_pZFWnd->LockWindowUpdate( );

                  return( 0 );
               }
            }
#endif
            // If there is a WM_PAINT in the queue, process it.
            HWND hWnd = *(pZSubtask->m_pZFWnd);
            MSG  msg;

            while ( ::PeekMessage( &msg, hWnd, WM_PAINT,
                                   WM_PAINT, PM_NOREMOVE ) )
            {
               if ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
               {
                  ::TranslateMessage( &msg );
                  ::DispatchMessage( &msg );
               }
            }

            pZSubtask->m_pZFWnd->UpdateWindow( );
         }

         return( 0 );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_DESTROY )
      {
         ZSubtask *pZParent = pZSubtask->m_pZParent;
         if ( pZParent == 0 )
            pZParent = pZSubtask->m_pZOwner;

         zBOOL bRC = pZSubtask->PostClose( 0 );
         if ( pZParent )
            ActivateWindow( pZParent->m_vDialog );

         return( bRC );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_STOP_EVENTS )
      {
         zBOOL bStopEvents = pZSubtask->m_bStopEvents;
         if ( bStopEvents )
            PumpMessages( pZSubtask->m_vDialog );

         pZSubtask->m_bStopEvents = (zBOOL) ulValue;
         return( bStopEvents );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_SET_NO_ERROR_PROMPT )
      {
         zBOOL bNoErrorPrompt =
            (pZSubtask->m_ulSubtaskFlags & zSUBTASK_NO_ERROR_PROMPT) ? TRUE :
                                                                       FALSE;
         if ( ulValue )
            pZSubtask->m_ulSubtaskFlags |= zSUBTASK_NO_ERROR_PROMPT;
         else
            pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_NO_ERROR_PROMPT;

         return( bNoErrorPrompt );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_ASYNC )
      {
         if ( ulValue )
         {
            pZSubtask->m_uMouseStatus = zMOUSE_ASYNC_DISABLE;

//          if ( pZSubtask->m_pZMIXCtrl )
//          {
//             pZSubtask->m_pZMIXCtrl->
//                VisitInorder( (fnVISIT_INORDER) fnEnableDisableCtrls, 0 );
//          }

            pZSubtask->m_pZView->EnableWindow( FALSE );
         }
         else
         {
//          if ( pZSubtask->m_pZMIXCtrl )
//          {
//             pZSubtask->m_pZMIXCtrl->
//                VisitInorder( (fnVISIT_INORDER) fnEnableDisableCtrls );
//          }

            pZSubtask->m_pZView->EnableWindow( TRUE );

            pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
         }

//       return( pZSubtask->EnableWindow( (zBOOL) ulValue ? FALSE : TRUE ) );
      }
      else
      if ( lStatusType == zWINDOW_STATUS_NO_REFRESH )
      {
         if ( ulValue )
            pZSubtask->m_ulSubtaskFlags |= zSUBTASK_STATE_PREVENT_REFRESH;
         else
            pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_STATE_PREVENT_REFRESH;
      }
   }
   else
   {
      ZFindDialog fd;

      fd.pvDialog = SysGetHandleFromPointer( vSubtask );
      fd.DlgN[ 0 ] = 0;
      fd.WndN[ 0 ] = 0;
      if ( FindGlobalDialogWindow( &fd ) && mIs_hWnd( fd.hWnd ) )
      {
         if ( lStatusType == zWINDOW_STATUS_DESTROY )
         {
         // TraceLineX( "SetWindowState successfully posted DESTROY for hWnd: ",
         //             (zLONG) fd.hWnd );

            ZSubtask *pZParent = 0;
            if ( fd.pZSubtask )
            {
               pZParent = fd.pZSubtask->m_pZParent;
               if ( pZParent == 0 )
                  pZParent = fd.pZSubtask->m_pZOwner;
            }

            if ( pZParent )
               ActivateWindow( pZParent->m_vDialog );

            ::PostMessage( fd.hWnd, g_uMsgPostedClose,
                           0, (LPARAM) fd.pZSubtask );
            return( 0 );
         }
         else
         {
            return( ::SendMessage( fd.hWnd, zZEIDON_SET_WINDOW_STATE,
                                   (WPARAM) lStatusType, (LPARAM) ulValue ) );
         }
      }

      TraceLineX( "SetWindowState secondary search could not locate Subtask: ",
                  (zLONG) vSubtask );
   }

   return( -1 );
}

//./ ADD NAME=EnableAllSubwindows
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: EnableAllSubwindows
//                                              04/14/93 Modified: 04/14/93
//
//  PURPOSE:   To enable/disable all subwindows of a parent window,
//             (including the parent window).
//
//  PARAMETERS: vSubtask  - The subtask view for the parent window
//              bEnable   - TRUE ==> enable   FALSE ==> disable
//
//  RETURNS:    -1  - Error locating window
//               0  - enable was successful
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
EnableAllSubwindows( zVIEW  vSubtask,
                     zSHORT bEnable )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In EnableAllSubwindows ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      pZSubtask->MapFromOI( );
#ifdef zREMOTE_SERVER
      EnableZWindowsForApplication( pZSubtask->m_pZSocketTask->m_pZSubtaskTop,
                                    (zBOOL) bEnable );
#else
      EnableZWindowsForApplication( pZSubtask->m_pZTask->m_pZSubtaskTop,
                                    (zBOOL) bEnable );
#endif
      zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      while ( lItemCnt )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
              g_pSubtaskList[ lItemCnt ].lTID == pZSubtask->m_lTID &&
              g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == pZSubtask )
         {
            EnableAllSubwindows( g_pSubtaskList[ lItemCnt ].pZSubtask->m_vDialog,
                                 bEnable );
         }

         lItemCnt--;
      }

      return( 0 );
   }

   return( -1 );
}

zOPER_EXPORT HWND OPERATION
CreateMDI_Child( zVIEW   vSubtask,
                 zCPCHAR cpcTitle )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        (pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_MDI_FRAME) )
   {
//??? return( pZSubtask->CreateMDI_Child( cpcTitle ) );
   }

   return( 0 );
}

zOPER_EXPORT HWND OPERATION
GetMDI_Client( zVIEW vSubtask )
{
#if 0
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        (pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_MDI_FRAME) &&
        pZSubtask->m_pMDI_Client )
   {
      if ( mIs_hWnd( pZSubtask->m_pMDI_Client->m_hWnd ) == FALSE )
      {
         TraceLineI( "Invalid GetMDI_Client ", (zLONG) pZSubtask->m_pMDI_Client->m_hWnd );
      }
      else
      {
         TraceLineI( "Returning valid GetMDI_Client ", (zLONG) pZSubtask->m_pMDI_Client->m_hWnd );
      }
      return( pZSubtask->m_pMDI_Client->m_hWnd );
   }
#endif

   return( 0 );
}

zOPER_EXPORT HWND OPERATION
GetLastMDI_Child( zVIEW vSubtask )
{
#if 0
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pMDI_LastChild )
   {
      return( pZSubtask->m_pMDI_LastChild->m_hWnd );
   }
#endif

   return( 0 );
}

//./ ADD NAME=GetAppOrWorkstationValue
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetAppOrWorkstationValue
//                                              04/11/01 Modified: 04/11/01
//
//  PURPOSE:    To get application values from the Zeidon.ini.
//              The order in which this function looks for the values is:
//               1. Zeidon.ini [App.LPLR_Name] section.
//               2. Zeidon.ini [Workstation] section.
//
//  PARAMETERS: vSubtask         - The subtask view
//              cpcKey           - Key name in the .ini file
//              pchReturnString  - return string
//              nReturnStringLth - return string maximum length
//
//  RETURNS:    TRUE - key located in .INI file.
//              FALSE - key not located in .INI file.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zBOOL OPERATION
GetAppOrWorkstationValue( zVIEW    vSubtask,
                          zCPCHAR  cpcKey,
                          zPCHAR   pchReturnString,
                          zSHORT   nReturnStringLth )
{
   zCHAR  szValue[ 16000 ];
   zCHAR  szAppName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szAppSection[ zZEIDON_NAME_LTH + 7 ];

   // Get application value.
   pchReturnString[ 0 ] = 0;
   GetApplDirectoryFromView( szAppName, vSubtask, zAPPL_NAME, zsizeof( szAppName ) );
   sprintf_s( szAppSection, "[App.%s]", szAppName );

   SysReadZeidonIni( -1, szAppSection, (zPCHAR) cpcKey, szValue, zsizeof( szValue ) );
   if ( szValue[ 0 ] == 0 )
   {
      SysReadZeidonIni( -1, "[Workstation]", (zPCHAR) cpcKey, szValue, zsizeof( szValue ) );
   }

   if ( szValue[ 0 ] )
   {
      strncat_s( pchReturnString, nReturnStringLth, szValue, nReturnStringLth - 1 );
      return( TRUE );
   }

   return( FALSE );
}

//./ ADD NAME=GetWorkstationAppValue
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWorkstationAppValue
//                                              04/10/00 Modified: 04/10/00
//
//  PURPOSE:    To get application values from the local application ini file.
//              The order in which this function looks for the values is:
//               1. ZeidonRT.ini in the local directory specified in the
//                  Zeidon.app for the current application.
//               2. ZeidonRT.ini in the local directory specified by the
//                  LocalDir item in the Zeidon.ini [Workstation] group.
//               3. Zeidon.ini itself in the [Workstation] group
//
//              The values are assumed to be a string value.
//
//  PARAMETERS: vSubtask         - The subtask view
//              cpcSection       - Section name in the .ini file
//              cpcKey           - Key name in the .ini file
//              pchReturnString  - return string
//              nReturnStringLth - return string maximum length
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT void OPERATION
GetWorkstationAppValue( zVIEW    vSubtask,
                        zPCHAR   cpcSection,
                        zCPCHAR  cpcKey,
                        zPCHAR   pchReturnString,
                        zSHORT   nReturnStringLth )
{
#ifdef DEBUG_ALL
   TraceLineI( "In GetWorkstationAppValue ", (zLONG) vSubtask );
#endif

   zCHAR  szZeidonRT_Path[ zMAX_FILENAME_LTH + 1 ];
   LPAPP  pApp;

   pchReturnString[ 0 ] = 0;
   szZeidonRT_Path[ 0 ] = 0;
   if ( SfGetApplicationForSubtask( &pApp, vSubtask ) == 0 && pApp )
      strcpy_s( szZeidonRT_Path, zsizeof( szZeidonRT_Path ), pApp->szLocalDir );

   if ( szZeidonRT_Path[ 0 ] )
   {
      SysAppendcDirSep( szZeidonRT_Path );
      strcat_s( szZeidonRT_Path, "ZeidonRT.ini" );
      GetPrivateProfileString( cpcSection, cpcKey, "", pchReturnString, nReturnStringLth, szZeidonRT_Path );
   }

   if ( pchReturnString[ 0 ] == 0 )
   {
      SysReadZeidonIni( -1, "[Workstation]", "LocalDir", szZeidonRT_Path, zsizeof( szZeidonRT_Path ) );
      if ( szZeidonRT_Path[ 0 ] )
      {
         SysAppendcDirSep( szZeidonRT_Path );
         strcat_s( szZeidonRT_Path, zsizeof( szZeidonRT_Path ), "ZeidonRT.ini" );
         GetPrivateProfileString( cpcSection, cpcKey, "", pchReturnString, nReturnStringLth, szZeidonRT_Path );
      }

      if ( pchReturnString[ 0 ] == 0 )
      {
         zCHAR  szValue[ 16000 ];
         zSHORT nLth = (zSHORT) zstrlen( cpcSection ) + 1;
         zPCHAR pch = new char[ nLth + 2 ];

         strcpy_s( pch + 1, nLth + 1, cpcSection );
         pch[ 0 ] = '['; pch[ nLth ] = ']'; pch[ nLth + 1 ] = 0;
         SysReadZeidonIni( -1, pch, (zPCHAR) cpcKey, szValue, zsizeof( szValue ) );
         strncat_s( pchReturnString, nReturnStringLth, szValue, nReturnStringLth - 1 );
         delete [] pch;
      }
   }
}

//./ ADD NAME=GetWorkstationApplicationValues
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWorkstationApplicationValues
//                                              02/02/99 Modified: 02/02/99
//
//  PURPOSE:    To get application values from the local application ini file.
//              The order in which this function looks for the values is:
//               1. ZeidonRT.ini in the local directory specified in the
//                  Zeidon.app for the current application.
//               2. ZeidonRT.ini in the local directory specified by the
//                  LocalDir item in the Zeidon.ini [Workstation] group.
//               3. Zeidon.ini itself in the [Workstation] group
//
//              The values are assumed to be a string value, followed by
//              up to a maximum of 10 numeric values.
//
//  PARAMETERS: vSubtask         - The subtask view
//              pchReturnString  - return string
//              nReturnStringLth - return string maximum length
//              plVal1 - plVal10 - return numeric values (may be null
//                                 pointers)
//
//  RETURNS:    -1  - Error locating window
//             >= 0 - number of values retrieved (0-11)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 15
zOPER_EXPORT zSHORT OPERATION
GetWorkstationApplicationValues( zVIEW    vSubtask,
                                 zCPCHAR  cpcValueName,
                                 zPCHAR   pchReturnString,
                                 zSHORT   nReturnStringLth,
                                 zPLONG   plVal1,
                                 zPLONG   plVal2,
                                 zPLONG   plVal3,
                                 zPLONG   plVal4,
                                 zPLONG   plVal5,
                                 zPLONG   plVal6,
                                 zPLONG   plVal7,
                                 zPLONG   plVal8,
                                 zPLONG   plVal9,
                                 zPLONG   plVal10 )
{
#ifdef DEBUG_ALL
   TraceLineI( "In GetWorkstationApplicationValues ", (zLONG) vSubtask );
#endif

   zPCHAR  pchString = new char[ nReturnStringLth + 100 ];
   zPCHAR  pch;
   zPCHAR  pchComma;
   zPLONG  pl;
   zSHORT  k;
   zSHORT  nValues = 0;

   GetWorkstationAppValue( vSubtask, "Workstation", cpcValueName,
                           pchString, nReturnStringLth + 100 );

   pch = pchString;
   if ( plVal1 )  // if looking for a first value, restrict the string
   {
      pchComma = zstrchr( pch, ',' );
      if ( pchComma )
      {
         *pchComma = 0;
         pchComma++;
      }
   }
   else
   {
      pchComma = 0;  // pass back the entire string
   }

   pchReturnString[ 0 ] = 0;
   strncat_s( pchReturnString, nReturnStringLth, pch, nReturnStringLth - 1 );
   pch = pchComma;
   if ( pchReturnString[ 0 ] )
      nValues++;

   for ( k = 0; k < 10; k++ )
   {
      switch ( k )
      {
         case 0: pl = plVal1; break;
         case 1: pl = plVal2; break;
         case 2: pl = plVal3; break;
         case 3: pl = plVal4; break;
         case 4: pl = plVal5; break;
         case 5: pl = plVal6; break;
         case 6: pl = plVal7; break;
         case 7: pl = plVal8; break;
         case 8: pl = plVal9; break;
         case 9: pl = plVal10; break;
         default: pl = 0; break;
      }

      if ( pl == 0 )
         break;

      if ( pch )
      {
         pchComma = zstrchr( pch, ',' );
         if ( pchComma )
         {
            *pchComma = 0;
            pchComma++;
         }

         *pl = zatol( pch );
         nValues++;
         pch = pchComma;
      }
      else
         *pl = 0;
   }

   mDeleteInitA( pchString );
   return( nValues );
}

//./ ADD NAME=PlaceWindow
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PlaceWindow
//                                          10/17/2002 Modified: 10/17/2002
//
//  PURPOSE:   To set the window placement within the physical screen.
//
//  PARAMETERS: vSubtask  - The subtask view for the current window.
//              lPosition - 0 - Centered (vertically and horizontally)
//                          1 - Top Centered
//                          2 - Left Centered
//                          3 - Top Left Corner
//                          4 - Bottom Centered
//                          6 - Bottom Left Corner
//                          8 - Right Centered
//                          9 - Top Right Corner
//                         12 - Bottom Right Corner
//                         32 - Tile based on parent
//
//  RETURNS:    0 - Window placement set
//             -1 - Error setting window placement
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
PlaceWindow( zVIEW   vSubtask,
             zLONG   lPosition )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pZFWnd && mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) &&
           (pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) == 0 )
      {
         RECT  r;
         CRect rectClient;
         CRect rect( pZSubtask->Attr.X, pZSubtask->Attr.Y,
                     pZSubtask->Attr.X + pZSubtask->Attr.W,
                     pZSubtask->Attr.Y + pZSubtask->Attr.H );

         pZSubtask->m_pZFWnd->GetWindowRect( rectClient );
         ::SystemParametersInfo( SPI_GETWORKAREA, 0, &r, 0 );

         zLONG lTopOffset = ((r.bottom - r.top) - rectClient.Height( ));
         zLONG lLeftOffset = ((r.right - r.left) - rectClient.Width( ));

         if ( lPosition == 32 )  // tile
         {
            if ( pZSubtask->m_pZParent && pZSubtask->m_pZParent->m_pZFWnd &&
                 mIs_hWnd( pZSubtask->m_pZParent->m_pZFWnd->m_hWnd ) )
            {
               CRect rectParent;
               pZSubtask->m_pZParent->m_pZFWnd->GetWindowRect( rectParent );
               zLONG lCaptionHeight = ::GetSystemMetrics( SM_CYCAPTION );
               lTopOffset = rectParent.top + lCaptionHeight;
               lLeftOffset = rectParent.left + lCaptionHeight;
            }
            else
            {
               lTopOffset = r.top;
               lLeftOffset = r.left;
            }
         }
         else
         if ( lPosition == 0 )
         {
            lTopOffset /= 2;
            lLeftOffset /= 2;
         }
         else
         {
            // Centering is requested in some direction ... determine
            // left and top offsets for centering.
            //  0 - Centered (vertically and horizontally)
            //  1 - Top Centered
            //  2 - Left Centered
            //  4 - Bottom Centered
            //  8 - Right Centered

            // Position to a corner.
            //  3 - Top Left Corner
            //  6 - Bottom Left Corner
            //  9 - Top Right Corner
            // 12 - Bottom Right Corner

            if ( lPosition & 1 )
               lTopOffset = 0;

            if ( lPosition & 2 )
               lLeftOffset = 0;

            if ( (lPosition & 4) == 0 )
               lTopOffset /= 2;

            if ( (lPosition & 8) == 0 )
               lLeftOffset /= 2;
         }

         pZSubtask->m_pZFWnd->SetWindowPos( 0, lLeftOffset + r.left,
                                            lTopOffset + r.top,
                                            rectClient.Width( ),
                                            rectClient.Height( ),
                                            SWP_NOSIZE | SWP_NOZORDER );
      }

      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=ResetWindowPlacement
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ResetWindowPlacement
//                                              04/04/00 Modified: 04/04/00
//
//  PURPOSE:   To set the window placement from one window to the placement
//             of another window.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window.
//              cpcToDlgTag - Dialog whose placement is to be reset
//              cpcToWndTag - Window whose placement is to be reset
//              cpcFromDlgTag - Dialog from which to reset placement
//              cpcFromWndTag - Window from which to reset placement
//
//  RETURNS:    0 - Window placement reset
//             -1 - Error resetting window placement
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
ResetWindowPlacement( zVIEW   vSubtask,
                      zCPCHAR cpcToDlgTag,
                      zCPCHAR cpcToWndTag,
                      zCPCHAR cpcFromDlgTag,
                      zCPCHAR cpcFromWndTag )
{
   if ( vSubtask )
   {
      WINDOWPLACEMENT wp;
      wp.length = sizeof( wp );

      // Get the original (from) window placement ...
      if ( ReadWindowPlacement( vSubtask, "ZDR",
                                cpcFromDlgTag, cpcFromWndTag, &wp ) )
      {
         // and write it to the .INI file for the current (to) window.
         WriteWindowPlacement( vSubtask, "ZDR",
                               cpcToDlgTag, cpcToWndTag, &wp );
         return( 0 );
      }
   }

   return( -1 );
}

//./ ADD NAME=SetRemoteWndAttribute
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetRemoteWndAttribute
//                                              01/07/00 Modified: 01/07/00
//
//  PURPOSE:   To set an attribute on a Wnd entity in the XRA.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the target control. This is the
//                            subtask passed into the dialog operation if
//                            the control is on the current window.
//              cpcAttribute - Name of the Attribute to set.
//              cpcValue    - pointer to the string from which to set
//                            the Wnd's attribute.
//
//  RETURNS:    0 - Wnd attribute set
//             -1 - Error locating Window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
SetRemoteWndAttribute( zVIEW   vSubtask,
                       zCPCHAR cpcAttribute,
                       zCPCHAR cpcValue,
                       zBOOL   bLock )
{
#ifdef zREMOTE_SERVER
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   TraceLineI( "In SetRemoteWndAttribute ", (zLONG) vSubtask );
#endif

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZSocketTask *pZSocketTask = pZSubtask->m_pZSocketTask;
      ZXRA_Item *pXRA;

      if ( pZSocketTask &&
           (pXRA = pZSocketTask->
             m_pCacheWndList->Find( *(pZSubtask->m_pzsRemoteWndTag),
                                    pZSubtask,
                                    "SetRemoteWndAttribute" )) != 0 )
      {
         zVIEW vXRA;

#ifdef DEBUG_ALL
         zCHAR  szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ),
                   "SetRemoteWndAttribute Tag: %s  Attribute: %s  Value: %s",
                   *(pZSubtask->m_pzsRemoteWndTag), cpcAttribute, cpcValue );
         TraceLineS( szMsg, "" );
#endif

         if ( pXRA->m_vXRA_Delta == 0 )
            pXRA->ActivateEmptyDelta( pZSocketTask->m_vRegisteredSubtask,
                                      pZSocketTask->m_szAppTag );

         CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA_Delta, m_pZSubtask->m_vDialog );
         while ( ResetViewFromSubobject( vXRA ) == 0 )
         {
         }

         if ( CheckExistenceOfEntity( vXRA, "Wnd" ) < zCURSOR_SET ||
              SetCursorFirstEntityByString( vXRA, "Wnd", "Tag",
                                            *(pZSubtask->m_pzsRemoteWndTag),
                                            0 ) < zCURSOR_SET )
         {
            CreateEntity( vXRA, "Wnd", zPOS_AFTER );
            SetAttributeFromString( vXRA, "Wnd",
                                    "Tag", *(pZSubtask->m_pzsRemoteWndTag) );
         }

         SetAttributeFromString( vXRA, "Wnd", cpcAttribute, cpcValue );

         DropView( vXRA );
      }

      return( 0 );
   }
   else
      return( -1 );
#else
   return( -1 );
#endif
}

//./ ADD NAME=WindowsShellExecute
/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: WindowsShellExecute
//
//    Open/Edit/Print a file based on the registered association.  Or can
//    be used invoke an EXE with parameters.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
WindowsShellExecute( zVIEW  vSubtask,
                     zPCHAR szFileOrExeName,
                     zPCHAR szFileOpenCommand,
                     zPCHAR szExeParams )
{
   HWND hWnd;
   zULONG lControlReturn;
   HINSTANCE hInstance;

   GetWindowHandle( (zPULONG) &hWnd, &lControlReturn, vSubtask, "" );

   if ( szFileOpenCommand && szFileOpenCommand[ 0 ] )
      TraceLineS( "WindowsShellExecute FileOpenCommand: ", szFileOpenCommand );

   if ( szFileOrExeName && szFileOrExeName[ 0 ] )
      TraceLineS( "WindowsShellExecute FileOrExeName: ", szFileOrExeName );

   if ( szExeParams && szExeParams[ 0 ] )
      TraceLineS( "WindowsShellExecute ExeParams: ", szExeParams );

   hInstance = ShellExecute( hWnd, szFileOpenCommand, szFileOrExeName,
                             szExeParams, NULL, SW_SHOWNORMAL );
   if ( hInstance > (HINSTANCE) 32)
   {
      TraceLineX( "WindowsShellExecute Success - Result: ", (zLONG) hInstance );
      return( 0 );
   }
   else
   {
      TraceLineX( "WindowsShellExecute Failed Result: ", (zLONG) hInstance );
      return( (zLONG) hInstance );
   }
} //WindowsShellExecute
