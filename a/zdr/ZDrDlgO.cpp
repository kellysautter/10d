/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdlgo.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of "C interface" routines into the
// C++ functionality available through the Zeidon driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.03.01    DKS    Z10
//    OperatorPromptForInput corrected after Ctrl-P enhancement.
//
// 2001.05.10    DKS   WEB
//    OperatorSend now calls OperatorPrompt to do the work.
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2000.12.08    BL     RemoteServer
//    Modified OperatorPrompt: set correct return view
//
// 2000.12.04    BL     RemoteServer
//    Modified OperatorPrompt: do not overwrite Message Box Return Value
//
// 2000.05.24    DKS    RemoteServer
//    Create unique MsgBox tag.
//
// 1999.07.09    DKS    Z2000   QS999
//    Reordered code in ResumeFromDebugger to prevent exception.
//
// 1998.11.17    DKS
//    Permit password type input for OperatorPromptForInput.
//
// 1998.10.05    DKS
//    Corrected algorithm for locating a Subtask across tasks.
//
// 1998.09.30    DKS
//    Ctrl+E and ResumeFromPainter fix (TB 281).
//
// 1998.09.09    DKS
//    OperatorPromptForInput now maintains the "wait" cursor (QS 205).
//
// 1998.08.13    DKS
//    Changes to unprintable characters for ClearCase.
//
// 1998.06.02    DKS (HH)
//    Changes in OperatorPromptForInput.
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


//./ ADD NAME=@_DriverFlow
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: @_DriverFlow
//                                              07/08/93 Modified: 07/08/93
//This is a quick attempt to describe the "flow of control" implemented by
//the current Zeidon run-time Driver.  Any suggestions for improving its
//understandability, while keeping it brief enough for a Quick Reference
//Guide will be appreciated.
//
//The Driver does 2 "major" tasks.
//   A. Processing a Window per the specification in a Dialog Definition.
//      This includes "minor" tasks:
//      1. Building the window;
//      2. Refreshing the window;
//      3. Leaving the window;
//      4. Processing Actions;
//   B. Handling the MS Window Events on behalf of the Window and the
//      Controls that it contains.
//
//The flow of each of these tasks are outlined in some detail below.  The
//Driver can be thoght of as a simple loop:
//
//         START Dialog's default Window (using A1_Build)
//         while "there is a window"
//            HANDLE the GUI Events for the window as they occur
//                                 (using B1_Handle_GUI)
//         end
//
//
//NOTES: 1. All zWND_EVENT_...s can have an associated Action that in
//          turn can execute your code.
//       2. All WM...s are MS Windows messages and can happen asyncronously.
//          Therefore, if the processing outline below "issues WM..." and
//          then continues, any actions associated with that WM... may
//          occur before OR after the remaining procedure.
//
//
//A. Processing a Window per the specification in a Dialog Definition.
//
//   A1_Build the window
//
//      DO zWND_EVENT_PRE_BUILD assoc. Action
//      Construct Window
//         CreateWindow
//         SetCaptionTitle
//         CreateActionBar
//         CreateControls
//      DO zWND_EVENT_POST_BUILD assoc. Action
//      DO A2_Refresh the window
//      issue WMActivate
//
//
//   A2_Refresh the window
//
//      AutoGrayActions
//      DO zWND_EVENT_ON_REFRESH assoc. Action
//         if ( RC > 0 )
//            return
//         otherwise
//            invoke Refresh (c++)method for each Window Control
//
//
//   A3_Leave the window
//
//      Destroy window(s)
//      if there is a Window remaining
//         issue WMActivate (see Note 2 above)
//         DO zWND_EVENT_ON_RETURN assoc. Action
//         if ( ReturnWithRefresh )
//            DO A2_Refresh the window
//
//
//   A4_ProcessAction
//
//      Map to the object instance (if Mapping has not been prevented)
//         if ( MappingError )
//            PresentMessage and TerminateAction
//
//      Process subobject behaviors
//      |  Determine if current entity is temporal
//      |
//      |  if ( zSUBOBJECT_VERIFY ) MessagePrompt
//      |     if ( No )
//      |        TerminateAction
//      |
//      |  if ( zSUBOBJECT_ACCEPT  or
//      |       zSUBOBJECT_CANCEL  or
//      |       zSUBOBJECT_DELETE  or
//      |       zSUBOBJECT_EXCLUDE ) issue Accept, Cancel, Delete, or Exclude
//      |     if ( Error )
//      |        PresentMessage and TerminateAction
//      |
//      |  Note: the following SUBOBJECT_CREATE_xxx should not be specified
//      |        with NEXT/PREV as the create is automatically done later
//      |  if ( zSUBOBJECT_CREATE_VERSION or
//      |       zSUBOBJECT_CREATE_TEMPORAL or
//      |       zSUBOBJECT_CREATE_ENTITY ) issue appropiate Create
//      |     if ( Error )
//      |        PresentMessage and TerminateAction
//
//      Invoke user specified Dialog Operation for the Action
//
//      Process Next/Prev subobject behaviors
//      |  if ( zSUBOBJECT_NEXT or
//      |       zSUBOBJECT_PREV ) SetCursor Next or Prev
//      |     if ( the current entity was temporal )
//      |        CreateTemporalSubobjectVersion
//      |           if ( Error )
//      |              PresentMessage and TerminateAction
//      |
//      |  if ( zSUBOBJECT_DELETE or zSUBOBJECT_EXCLUDE )
//      |     zSUBOBJECT_STAYWHENNULL  or
//      |     zSUBOBJECT_RETURNWHENNULL  (with refresh)
//
//      Process Window Action Behavior (zWAB...)
//
//
//B. Handling the MS Window Events on behalf of the Window and its Controls.
//
//   SysCommand
//      DO zWND_EVENT_CLOSE assoc. Action
//            or DO zWND_EVENT_MINIMIZE assoc. Action
//            or DO zWND_EVENT_MAXIMIZE assoc. Action
//            or DO zWND_EVENT_RESTORE assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      if ( there is No zWND_EVENT_CLOSE and event == SC_CLOSE )
//         if ( there is an Alt-F4 "hotkey" defined for this window )
//            Invoke the "hotkey"
//      otherwise
//         Prompt for Close
//
//
//   WMActivate
//      DO zWND_EVENT_NOTIFYCHILD_ACTIVATE assoc. Action
//      DO zWND_EVENT_ON_ACTIVATE assoc. Action
//      Show Window
//
//
//   WMSize
//      DO zWND_EVENT_ON_SIZE assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//         otherwise
//            inform all controls that a WM_SIZE has occurred
//
//
//   WMInitMenu
//      if ( Popup )
//         DO zWND_EVENT_POPUP_PRE_DROPDOWN assoc. Action
//      otherwise
//         DO zWND_EVENT_MENU_PRE_DROPDOWN assoc. Action
//
//
//   WMPaletteChanged
//      DO zWND_EVENT_NOTIFYCHILD_PALETTE assoc. Action
//      DO zWND_EVENT_ON_PALETTE_CHANGED assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//   WMLButtonDown
//      DO zWND_EVENT_LBUTTON_DOWN assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//   WMLButtonUp
//      zMOUSE_DRAG_LEFT
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_EVENT_LBUTTON_DRAG_DROP assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_EVENT_LBUTTON_CLICK assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_EVENT_LBUTTON_UP assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//   WMLButtonDblclk
//      DO zWND_EVENT_LBUTTON_DBLCLK assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//   WMRButtonDown
//      DO zWND_EVENT_RBUTTON_DOWN assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//   WMRButtonUp
//      DO zWND_MOUSE_DRAG_RIGHT assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_EVENT_RBUTTON_DRAG_DROP assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_EVENT_RBUTTON_CLICK assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_EVENT_RBUTTON_UP assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//   WMRButtonDblclk
//      DO zWND_EVENT_RBUTTON_DBLCLK assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//   WMMouseMove
//      DO zWND_EVENT_LBUTTON_DRAG assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_MOUSE_DRAG_RIGHT assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
//      DO zWND_EVENT_MOUSE_MOVE assoc. Action
//         if ( RC > 0 )
//            return  (no default processing)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 1

//./ ADD NAME=@__DocumentDefines
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: @DocumentDefines
//                                              04/01/93 Modified: 07/05/93
//
// #define zWAB_SwitchToAction                   -1
// #define zWAB_DeterminedInDialogOperation       0
//
// #define zWAB_StayOnWindow                      1       // PREVENT ACTION
// #define zWAB_StayOnWindowWithRefresh           2
//
// #define zWAB_StartOrFocusModelessDialog       30
// #define zWAB_StartDialogTask                  31
// #define zWAB_StartModelessWindow              32
// #define zWAB_StartOrFocusModelessWindow       38
// #define zWAB_StartModalWindow                 36
// #define zWAB_StartModelessSubwindow           zTAG_LTH
// #define zWAB_StartOrFocusModelessSubwindow    39
// #define zWAB_StartModalSubwindow              34
// #define zWAB_StartOrFocusModelessApp          37
// #define zWAB_StartNewApp                      35
//
// #define zWAB_StartPopupMenu                   40
//
// #define zWAB_ReplaceDialogWithDialog          51
// #define zWAB_ReplaceWindowWithModelessWindow  52
// #define zWAB_ReplaceWindowWithModalWindow     53
// #define zWAB_ReplaceCurrentAppWithApp         54
//
// #define zWAB_ReturnToParent                   71
// #define zWAB_ReturnToParentWithRefresh        72
// #define zWAB_ReturnToTopWindow                73
// #define zWAB_ReturnToTopWindowWithRefresh     74
// #define zWAB_StartTopWindow                   75
// #define zWAB_ResetTopWindow                   76
//
// #define zWAB_ExitDialogTask                   99
//
// #define zWAB_SuspendToDebugger               201
// #define zWAB_ResumeFromDebugger              202
//
// #define zWAB_TerminateActionForError        9999
// #define zWAB_ProcessImmediateAction  0x10000000L
// #define zWAB_NoDriverMessageOnError  0x20000000L
// #define zWAB_ProcessImmediateReturn  0x40000000L
//
//
// // common attributes for all controls
// #define zCONTROL_NOTABSTOP            0x00080000L // -2147483648
// #define zCONTROL_RESERVED             0x80000000L // 268435456
// #define zCONTROL_SIZEABLEBORDER       0x40000000L // 1073741824
// #define zCONTROL_BORDERON             0x00000000L // default
// #define zCONTROL_BORDEROFF            0x20000000L // 536870912
// #define zCONTROL_RESERVED_1           0x10000000L // 268435456
// #define zCONTROL_INVISIBLE            0x08000000L // 134217728
// #define zCONTROL_DISABLED             0x04000000L // 67108864
// #define zCONTROL_RESERVED_2           0x02000000L // zTAG_LTH554432
// #define zCONTROL_RIGHTJUSTIFY         0x01000000L // 16777216
// #define zCONTROL_RELATIVE             0x00F00000L // position and size
// #define zCONTROL_RELATIVE_POSITION    0x00C00000L // both X and Y position
// #define zCONTROL_RELATIVE_POS_X       0x00800000L // 8388608
// #define zCONTROL_RELATIVE_POS_Y       0x00400000L // 4194304
// #define zCONTROL_RELATIVE_SIZE        0x00300000L // both X and Y size
// #define zCONTROL_RELATIVE_SIZE_X      0x00200000L // 2097152
// #define zCONTROL_RELATIVE_SIZE_Y      0x00100000L // 1048576
// #define zCONTROL_SPECIFIC_1           0x00040000L
// #define zCONTROL_SPECIFIC_2           0x00020000L
// #define zCONTROL_SPECIFIC_3           0x00010000L
//
//
// #define zRGB_DEFAULT                  -1L
// #define zRGB_BLACK                    RGB( 0, 0, 0 )
// #define zRGB_BLUE                     RGB( 0, 0, 255 )
// #define zRGB_DARKBLUE                 RGB( 0, 0, 128 )
// #define zRGB_BROWN                    RGB( 128, 128, 64 )
// #define zRGB_CYAN                     RGB( 0, 255, 255 )
// #define zRGB_DARKCYAN                 RGB( 0, 128, 128 )
// #define zRGB_GRAY                     RGB( 192, 192, 192 )
// #define zRGB_DARKGRAY                 RGB( 128, 128, 128 )
// #define zRGB_GREEN                    RGB( 0, 255, 0 )
// #define zRGB_DARKGREEN                RGB( 0, 128, 0 )
// #define zRGB_PINK                     RGB( 255, 0, 255 )
// #define zRGB_DARKPINK                 RGB( 255, 0, 128 )
// #define zRGB_YELLOW                   RGB( 255, 255, 0 )
// #define zRGB_DARKYELLOW               RGB( 128, 128, 0 )
// #define zRGB_RED                      RGB( 255, 0, 0 )
// #define zRGB_DARKRED                  RGB( 128, 0, 0 )
// #define zRGB_WHITE                    RGB( 255, 255, 255 )
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 1

//./ ADD NAME=GetOperationDynamicCallAddress
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetOperationDynamicCallAddress
//                                              04/16/93 Modified: 04/16/93
//
//  PURPOSE: To load a library (DLL) and/or get an address for the
//           specified operation.  If the hLibrary is zero, attempt
//           to load the DLL, otherwise assume the hLibrary is valid.
//
//  PARAMETERS: vSubtask  - The subtask view for the window containing
//                          the control.
//              lpLibrary - Pointer to a handle to a library.
//              cpcDLL_Name - DLL_Name to load.
//              cpcOperationName - Name of the operation (function) to
//                                 get the address of.
//              lpErrorMessagePrefix - an identifier with which to
//                                     prefix the error message if
//                                     an error occurs.
//
//  RETURNS:    0 - the library or the operation could not be located.
//              otw - the address of the operation and the handle to
//                    the library (hLibrary).
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zPVOID OPERATION
GetOperationDynamicCallAddress( zVIEW       vSubtask,
                                LPLPLIBRARY lpLibrary,
                                zCPCHAR     cpcDLL_Name,
                                zCPCHAR     cpcOperationName,
                                zCPCHAR     cpcErrorMessagePrefix )
{
   zPVOID lpfnDynRoutine = 0;

   if ( *lpLibrary == 0 )
      *lpLibrary = SysLoadLibraryWithErrFlag( vSubtask, cpcDLL_Name, 0 );

   if ( *lpLibrary )
   {
      lpfnDynRoutine = SysGetProc( *lpLibrary, cpcOperationName );
      if ( lpfnDynRoutine )
         return( lpfnDynRoutine );
   }

   if ( cpcErrorMessagePrefix && *cpcErrorMessagePrefix )
   {
      zSHORT  nLth = (zSHORT) zstrlen( cpcErrorMessagePrefix );
      zPCHAR  pch = new char[ nLth + 256 ];

      strcpy_s( pch, nLth + 256, cpcErrorMessagePrefix );
      if ( *lpLibrary == 0 )
      {
         strcat_s( pch, nLth + 256, " Unable to load\n application DLL - " );
         strcat_s( pch, nLth + 256, cpcDLL_Name );
         strcat_s( pch, nLth + 256, "\n for operation - " );
         strcat_s( pch, nLth + 256, cpcOperationName );
      }
      else
      {
         strcat_s( pch, nLth + 256, " " );
         strcat_s( pch, nLth + 256, cpcOperationName );
         strcat_s( pch, nLth + 256, " Operation Code not found\n in DLL - " );
         strcat_s( pch, nLth + 256, cpcDLL_Name );
      }

      TraceLineS( pch, "" );
      MessageSend( vSubtask, 0, "Zeidon Driver", pch, 0, TRUE );
      delete [] pch;
   }

   return( 0 );
}

#if 0
//   Tab Stop (boolean)
//   Tabbing Order Index
//   Drag Mode (manual (code) or automatic (GUILIB handles))
//   Drag Icon
#endif

//./ ADD NAME=InitInterpretor
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: InitInterpretor
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To register the interpretor subtask to the driver
//
//  PARAMETERS: vSubtask     - subtask view for the application
//              vInterpIn    - subtask view for the interpretor
//              lpfnInterpIn - address of the interpretor operation
//
//
//  DESCRIPTION: The flow for interpretor/driver interaction is as
//               follows:
//
//              - AppDirector registers Zeidon application for the
//                interpretor application and receives a subtask view id?????
//              - AppDirector gets the interpretor's dynamic call
//                routine address
//              - AppDirector calls driver's InitInterpretor with
//                these two parameters ... the driver now knows that
//                the interpretor is active
//              - Any application that the driver starts will be started
//                with the interpretor flag set ON, except those
//                applications started by a driver HotKey and the
//                debugger.  All application code for applications with
//                the interpretor flag set will be filtered to the
//                interpretor.
//              - The driver will interrogate the return code from code
//                executed by the interpretor.  If the return code is
//                non-zero, the driver will start the debugger application
//                ... suspending (disable all windows within) the current
//                application
//              - When the debugger wants to quit and permit the application
//                to resume, the debugger will invoke ResumeFromDebugger,
//                which will re-activate (enable all windows within) the
//                original application ACTION at the point beyond operation
//                code
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
InitInterpretor( zVIEW vSubtask,
                 zVIEW vInterpIn,
                 zFARPROC_INTERP lpfnInterpIn )
{
   LPAPP    pApp;
   ZSubtask *pZSubtask;

   SfGetApplicationForSubtask( &pApp, vInterpIn );
   if ( pApp )
      TraceLineS( "Interpretor application =====> ", pApp->szName );

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      pZSubtask->m_pZTask->m_lpfnInterp = lpfnInterpIn;
      pZSubtask->m_pZTask->m_vInterpSubtask = vInterpIn;
      return( 0 );
   }

   return( -1 );
}

//./ ADD NAME=ResumeFromDebugger
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ResumeFromDebugger
//                                              04/01/93 Modified: 04/14/93
//
//  PURPOSE:   To resume processing of the application from the debugger
//
//  PARAMETERS: vDebuggerSubtask  - the debugger subtask view
//              cpcDebuggerAction - currently always 0
//              vAppSubtask       - subtask of application being debugged
//
//  RETURNS:    0 - Resume OK
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
ResumeFromDebugger( zVIEW   vDebuggerSubtask,
                    zCPCHAR cpcDebuggerAction,
                    zVIEW   vAppSubtask )
{
   ZSubtask *pZSubtask;

// TraceLineI( "ResumeFromDebugger =============> ", (zLONG) vAppSubtask );

   if ( GetWindowAndCtrl( &pZSubtask, 0, vDebuggerSubtask, 0 ) == 0 )
   {
      ZTask *pZTask = pZSubtask->m_pZTask;

      // First process the debugger action (if requested).
      if ( cpcDebuggerAction )
         ProcessActionLoop( pZSubtask, cpcDebuggerAction, 0, 0, 0, FALSE, "Debugger" );

      if ( GetWindowAndCtrl( &pZSubtask, 0, vAppSubtask, 0 ) == 0 )
      {
         CWaitCursor wait;
         zCHAR szActionName[ zTAG_LTH ];
         zCHAR szMessage[ 80 ];

         strcpy_s( szMessage, sizeof( szMessage ), "(drvr[" );
         ltoa( (zLONG) pZSubtask, szMessage + 6, 10 );
         if ( pZSubtask )
         {
            strcat_s( szMessage, sizeof( szMessage ), "." );
            strcat_s( szMessage, sizeof( szMessage ), *(pZSubtask->m_pzsDlgTag) );
            strcat_s( szMessage, sizeof( szMessage ), "." );
            strcat_s( szMessage, sizeof( szMessage ), *(pZSubtask->m_pzsWndTag) );
         }

         // Re-enable all windows for the application being debugged.
#ifdef zREMOTE_SERVER
      // EnableZWindowsForApplication( pZSubtask->m_pZSocketTask->m_pZSubtaskTop, TRUE );
#else
      // EnableZWindowsForApplication( pZSubtask->m_pZTask->m_pZSubtaskTop, TRUE );
#endif

         pZSubtask->m_vInterpSubtask = pZSubtask->m_pZTask->m_vInterpSubtask;
         if ( pZSubtask->m_pzsResumeAction == 0 )
         {
            pZTask = pZSubtask->m_pZTask;
            pZSubtask->RestartBuildWindow( vAppSubtask );
            zULONG  ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
            if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) != 0 &&
                 (ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED )
            {
               pZTask->DeleteSubtask( pZSubtask );
            }
         }
         else
         {
            // Localize Action name so it can be modified by ProcessAction.
            strcpy_s( szActionName, sizeof( szActionName ), *(pZSubtask->m_pzsResumeAction) );
            mDeleteInit( pZSubtask->m_pzsResumeAction );

            // The first time thru ... resume flag is ON.
            while ( ProcessAction( pZSubtask, szActionName, vAppSubtask, 0, "Resume" ) > 0 )
            {
               vAppSubtask = 0;
            }
         }

         if ( pZTask->m_nTraceAction > 2 )
            TraceLineS( szMessage, "]) ResumeFromDebugger" );

         return( 0 );
      }
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  GLOBAL OPERATION: HotkeyToPainter
//                                              05/04/93 Modified: 05/04/93
//
//  PURPOSE: To hotkey to the painter
//
//  PARAMETERS: vSubtask  - The subtask view for the window.
//
//  RETURNS:    -1  - Error locating window
//              -2  - The window is a parent window
//              -3  - Cannot start painter for deployed application
//              otw - Return value as defined for the WinExec documentation
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
HotkeyToPainter( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;
   zCHAR  szDeployed[ 2 ];

   TraceLineX( "HotkeyToPainter for Subtask: ", (zLONG) vSubtask );
   GetAppOrWorkstationValue( vSubtask, "Deployed",
                             szDeployed, sizeof( szDeployed ) );
   if ( szDeployed[ 0 ] == 'Y' || szDeployed[ 0 ] == 'y' )
      return( -3 );  // cannot start painter for deployed application

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zVIEW  vIdlePainter;
      zLONG  lItemCnt;

      // Run through the list of windows to find any that have this window
      // as its parent ... if we find any prohibit transfer to painter.
      lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      while ( lItemCnt )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
              g_pSubtaskList[ lItemCnt ].lTID == pZSubtask->m_lTID &&
              g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == pZSubtask )
         {
            OperatorPrompt( pZSubtask->m_vDialog, "Transfer to Dialog Editor",
                            "Transfer prevented ... close sub-windows", 1,
                            zBUTTONS_OK, zRESPONSE_OK, zICON_QUESTION );
            return( -2 );
         }

         lItemCnt--;
      }

      // Pass subtask of application being painted to painter.
      SetNameForView( vSubtask, "__SysPainterHotkey", vSubtask, zLEVEL_SYSTEM );

      // Do processing for the hotkey here ... e.g. disable all windows in
      // the current application and start painter.
      EnableAllSubwindows( vSubtask, FALSE );
      if ( GetViewByName( &vIdlePainter, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) != zLEVEL_SYSTEM )
      {
         zCHAR  szFileSpec[ zMAX_FILENAME_LTH ];
         zSHORT nLth;

         SysGetLocalDirectory( szFileSpec );
//       SysGetEnvVar( szFileSpec, "ZEIDON", 200 );
         nLth = (zSHORT) zstrlen( szFileSpec );
//       if ( szFileSpec[ nLth - 1 ] != '\\' )
//          szFileSpec[ nLth++ ] = '\\';

         strcpy_s( szFileSpec + nLth, sizeof( szFileSpec ) - nLth, "zdr ~~Zeidon_Tools~tzpntrad" );
         UINT uRC = ::WinExec( szFileSpec, SW_SHOWNORMAL );
         if ( uRC < 32 )
         {
            ::MessageBox( 0, szFileSpec, "Execution Error", MB_OK );
            EnableAllSubwindows( vSubtask, TRUE );
         }

         return( uRC );
      }
      else
      {
         // The painter is in another task, so we do not have addressability
         // to its data ... but we can send a message.
         TraceLineX( "Looking for vIdlePainter: ", (zLONG) vIdlePainter );

         // Protect this with a semaphore!!!
         SysMutexLock( vSubtask, "ZDr", 0, 0 );
      // TraceLineS( "HotkeyToPainter Lock Mutex ZDr", "" );

         zPVOID pvIdlePainter = SysGetHandleFromPointer( vIdlePainter );
         zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
         while ( lItemCnt > 0 )
         {
            if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
                 g_pSubtaskList[ lItemCnt ].pvDialog == pvIdlePainter )
            {
            // TraceLineS( "HotkeyToPainter Unlock Mutex ZDr", "" );
               SysMutexUnlock( vSubtask, "ZDr", 0 );

               // We found the painter info.
               ::ShowWindow( g_pSubtaskList[ lItemCnt ].hWnd, SW_RESTORE );
               ::SetForegroundWindow( g_pSubtaskList[ lItemCnt ].hWnd );
               return( 0 );
            }

            lItemCnt--;
         }

      // TraceLineS( "HotkeyToPainter Unlock Mutex ZDr", "" );
         SysMutexUnlock( vSubtask, "ZDr", 0 );

         DropNameForView( vIdlePainter, "__SysIdlePainter",
                          vSubtask, zLEVEL_SYSTEM );
         ::MessageBox( 0, "Could not activate Painter",
                       "Execution Error", MB_OK );
         EnableAllSubwindows( vSubtask, TRUE );
      }
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  GLOBAL OPERATION: StartPainterForApp
//                                              05/04/93 Modified: 05/04/93
//
//  PURPOSE: To transfer to the report or dialog painter
//
//  PARAMETERS: vSubtask  - The subtask view for the window.
//
//  RETURNS:    -1  - Error locating window
//              -2  - Error loading painter
//              -3  - Painter already active
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
StartPainterForApp( zVIEW vSubtask, // Subtask from application
                    zVIEW vDialogReport,
                    zCPCHAR cpcDialogReport,
                    zLONG lFlags ) // 0x0000001 - start report painter
                                   // 0x0000008 - use dialog/report view ...
                                   //             otw use dialog/report name
{
   zVIEW    vTemp;

   TraceLineX( "StartPainterFromApp for Subtask: ", (zLONG) vSubtask );
   if ( GetViewByName( &vTemp, "__SysApplicationReport",
                       vSubtask, zLEVEL_SYSTEM ) == zLEVEL_SYSTEM )
   {
      OperatorPrompt( vSubtask, "Report", "Cannot edit more than one report",
                      0, 0, 0, 0 );
      return( -3 );
   }

   if ( lFlags & 0x00000008 )
   {
      if ( cpcDialogReport && *cpcDialogReport )
         vDialogReport = 0;
   }
   else
   {
      // Load up the report here!!!
      vDialogReport = 0;
   }

   if ( vDialogReport )
   {
      // Pass application report being painted to painter.
      SetNameForView( vDialogReport, "__SysApplicationReport", vSubtask, zLEVEL_SYSTEM );

#if 1
      ZSubtask *pZSubtask;
      if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
      {
         zVIEW  vDialog;
      // LPAPP  lpSystemApp = (LPAPP) SysGetAnchorBlockData( 0x80000001 );

         CreateWindowSubtask( &vDialog, vSubtask, 0,
                              (lFlags & 0x00000001) ? "tzrpupzd" : "tzpntrad",
                              "Zeidon_Tools" ); // lpSystemApp->szName );
         if ( vDialog )
         {
            zPCHAR pchDlg;
            zPCHAR pchWnd;

            GetAddrForAttribute( (zPCHAR *) &pchDlg, vDialog, "Dlg", "Tag" );
            GetAddrForAttribute( (zPCHAR *) &pchWnd, vDialog, "Dlg", "DfltWnd" );
            if ( pchWnd[ 0 ] )
               SetCursorFirstEntityByString( vDialog, "Wnd", "Tag", pchWnd, 0 );

            BuildWindowImmediate( vSubtask, vDialog, zWAB_StartModalSubwindow );
            return( 0 );
         }
         else
         {
            zCPCHAR cpcTitle;

            if ( lFlags & 0x00000001 )
               cpcTitle = "Transfer to Report Editor";
            else
               cpcTitle = "Transfer to Dialog Editor";

            OperatorPrompt( pZSubtask->m_vDialog, cpcTitle,
                            "Transfer prevented ... could not open editor", 1,
                            zBUTTONS_OK, zRESPONSE_OK, zICON_QUESTION );
            return( -2 );
         }
      }
      else
         return( -1 );
#else
      CWaitCursor wc;

      // Disable all windows in the current application and start painter.
      EnableAllSubwindows( vSubtask, FALSE );

      StartTask( vSubtask, "ZDr.exe ~~Zeidon_Tools~tzrpupzd", SW_SHOW );
      while ( GetViewByName( &vDialogReport, "__SysApplicationReport",
                             vSubtask, zLEVEL_SYSTEM ) == zLEVEL_SYSTEM )
      {
         SysWait( 400 );
         PumpMessages( vSubtask );
      }

      // Re-enable all windows in the current application.
      EnableAllSubwindows( vSubtask, TRUE );
      PumpMessages( vSubtask );
      ActivateWindow( vSubtask );
      SetFocusToCtrl( vSubtask, 0 );
      return( 0 );
#endif
   }

   zCPCHAR cpcTitle;
   zCPCHAR cpcMsg;

   if ( lFlags & 0x00000001 )
   {
      cpcTitle = "Transfer to Report Editor";
      cpcMsg = "Could not load Report";
   }
   else
   {
      cpcTitle = "Transfer to Dialog Editor";
      cpcMsg = "Could not load Dialog";
   }

   OperatorPrompt( vSubtask, cpcTitle, cpcMsg, 1,
                   zBUTTONS_OK, zRESPONSE_OK, zICON_QUESTION );
   return( -2 );

}

// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: ResumeFromPainter
//                                              04/01/93 Modified: 04/20/93
//
//  PURPOSE:   To resume processing of the application from the painter
//
//  PARAMETERS: vPainterSubtask  - the painter subtask view
//              cpcPainterAction - currently always 0
//              vAppSubtask      - subtask of application being painted
//
//  RETURNS:    0 - Resume OK
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
ResumeFromPainter( zVIEW   vPainterSubtask,
                   zCPCHAR cpcPainterAction,
                   zVIEW   vAppSubtask )
{
   ZSubtask *pZSubtask;

   if ( cpcPainterAction &&
        GetWindowAndCtrl( &pZSubtask, 0, vPainterSubtask, 0 ) == 0 )
   {
      // first process the painter action (if requested)
      ProcessActionLoop( pZSubtask, cpcPainterAction,
                         0, 0, 0, FALSE, "Painter" );
   }

   ZFindDialog fd;

   if ( vAppSubtask )
      fd.pvDialog = SysGetHandleFromPointer( vAppSubtask );
   else
      fd.pvDialog = 0;

   fd.DlgN[ 0 ] = 0;
   fd.WndN[ 0 ] = 0;
   if ( FindGlobalDialogWindow( &fd ) )
   {
//    TraceLineS( "FindGlobalDialogWindow found: ", fd.WndN );
      ::PostMessage( fd.hWnd, zZEIDON_RESUME_FROM_PAINTER,
                     0, (LPARAM) fd.pZSubtask );
      return( 0 );
   }

   TraceLineX( "FindGlobalDialogWindow could NOT find: ", (zLONG) vAppSubtask );
   return( -1 );
}

//./ ADD NAME=fnResumeFromPainter
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: fnResumeFromPainter
//                                              04/01/93 Modified: 04/20/93
//
//  PURPOSE:   To resume processing of the application from the painter.
//             Note that the painter destroys itself on exit.
//
//  PARAMETERS: vSubtask  - the subtask to which the painter is resuming
//
//  RETURNS:    0 - Resume OK
//
/////////////////////////////////////////////////////////////////////////////
void
fnResumeFromPainter( ZSubtask *pZSubtask )
{
   zCHAR  szViewName[ 68 ];
   zVIEW  vDialogNew;
   CWaitCursor wait;

   szViewName[ 0 ] = '_';  szViewName[ 1 ] = '_';
   strcpy_s( szViewName + 2, sizeof( szViewName ) - 2, *(pZSubtask->m_pzsDlgTag) );
   if ( GetViewByName( &vDialogNew, szViewName,
                       pZSubtask->m_vDialog, zLEVEL_TASK ) == zLEVEL_TASK )
   {
      DropView( vDialogNew );
   }

#ifdef zREMOTE_SERVER
   EnableZWindowsForApplication( pZSubtask->m_pZSocketTask->m_pZSubtaskTop,
                                 TRUE );
#else
   EnableZWindowsForApplication( pZSubtask->m_pZTask->m_pZSubtaskTop, TRUE );
#endif

   pZSubtask = pZSubtask->m_pZTask->
                  NewZWnd( pZSubtask->m_pZParent,     // Zeidon parent
                           pZSubtask->m_pZParent,     // Zeidon owner
                           pZSubtask->m_vDialog,
                           0,                         // new subtask
                           (pZSubtask->m_ulSubtaskFlags & 0x00000001) ?
                                                             TRUE : FALSE,
                           *(pZSubtask->m_pzsDlgTag), // Dialog to process
                           *(pZSubtask->m_pzsWndTag), // Window to create
                           pZSubtask );               // delete current
   if ( pZSubtask && pZSubtask->m_pZFWnd &&
        mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
   {
      pZSubtask->m_pZFWnd->SetForegroundWindow( );
   }
}

//./ ADD NAME=OperatorSend
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OperatorSend
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:    To display a message box prompting the operator for a
//              response.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window.
//              pchTitle    - An optional title for the dialog box
//              cpcMessage  - The message to be displayed
//              bBeep       - 0 - No beep
//                            1 - Annoy the operator with a beep
//
//  RETURNS: -  0 - everything ok
//             -1 - the subtask was invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
OperatorSend( zVIEW    vSubtask,
              zCPCHAR  cpcTitle,
              zCPCHAR  cpcMessage,
              zSHORT   bBeep )
{
#if 1
   return( OperatorPrompt( vSubtask, cpcTitle, cpcMessage, bBeep, 0, 0, 0 ) );
#else
   zBOOL    bMsgFound = FALSE;
   MSG      msg;

   if ( PeekMessage( &msg, 0, g_uMsgPostedClose, g_uMsgPostedClose, PM_REMOVE | PM_NOYIELD ) )
   {
      bMsgFound = TRUE;
   }

   if ( bBeep )
      ::MessageBeep( MB_ICONQUESTION );

// TraceLineS( cpcTitle, cpcMsgText );
   ::MessageBox( GetActiveWindow( ), cpcMessage, cpcTitle, MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL );

   if ( bMsgFound )
      ::PostMessage( msg.hwnd, g_uMsgPostedClose, msg.wParam, msg.lParam );

   return( 0 );
#endif
}

//./ ADD NAME=OperatorPromptForInput
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OperatorPromptForInput
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:    To display an input dialog box prompting the operator
//              for input.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window (this
//                            may be zero if there is no valid subtask).
//              cpcTitle    - An optional title for the dialog box
//              cpcMsgText  - The prompt for the requested input
//              bBeep       - 0 - No beep
//                            1 - Annoy the operator with a beep
//                           otw - the "password character" ... the return
//                                information will be displayed as a password.
//
//              pchReturnBuffer - Pointer to the return buffer.
//              uBufferLth  - Maximum length of the return buffer
//
//  RETURNS: -  0 - everything ok
//             -1 - the subtask was invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
OperatorPromptForInput( zVIEW    vSubtask,
                        zCPCHAR  cpcTitle,
                        zCPCHAR  cpcMsgText,
                        zSHORT   bBeep,
                        zPCHAR   pchReturnBuffer,
                        zUSHORT  uBufferLth )
{
   ZSubtask *pZSubtask = 0;
   zBOOL    bMsgFound = FALSE;
   MSG      msg;
   zSHORT   nRC = -1;

   if ( PeekMessage( &msg, 0, g_uMsgPostedClose, g_uMsgPostedClose, PM_REMOVE | PM_NOYIELD ) )
   {
      bMsgFound = TRUE;
   }

#ifdef zREMOTE_SERVER

   SysMutexLock( vSubtask, "ZDm", 0, 0 );
   g_lMsgTag++;
   zLONG lMsgTag = g_lMsgTag;
   SysMutexUnlock( vSubtask, "ZDm", 0 );

   if ( vSubtask == 0 || GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) )
   {
      GetViewByName( &vSubtask, "__ZDrSubtask", 0, zLEVEL_TASK );
      if ( vSubtask )
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
   }

   if ( pZSubtask && pZSubtask->m_pZSocketTask )
   {
   // TraceLineS( "OperatorPrompt ZSubtask: ", *(pZSubtask->m_pzsWndTag) );
   // pZSubtask->m_ulSubtaskFlags |= zSUBTASK_APPLICATION_OPERATION;

      ZXRA_Item *pXRA = pZSubtask->m_pZSocketTask-> m_pCacheWndList->Find( *(pZSubtask->m_pzsRemoteWndTag),
                                                                           pZSubtask, "OperatorPromptForInput" );
      if ( pXRA )
      {
         if ( pXRA->m_vXRA_Delta == 0 )
         {
            pXRA->ActivateEmptyDelta( pZSubtask->m_pZSocketTask->m_vRegisteredSubtask,
                                      pZSubtask->m_pZSocketTask->m_szAppTag );
         }

         while ( ResetViewFromSubobject( pXRA->m_vXRA_Delta ) == 0 )
         {
         }

         while ( CheckExistenceOfEntity( pXRA->m_vXRA_Delta, "MsgBox" ) == zCURSOR_SET )
         {
            DeleteEntity( pXRA->m_vXRA_Delta, "MsgBox", zREPOS_FIRST );
         }

         CreateEntity( pXRA->m_vXRA_Delta, "MsgBox", zPOS_AFTER );
         CString csTag = cpcTitle;
         csTag += ':';
         zCHAR szTemp[ 16 ];
         _ltoa_s( lMsgTag, szTemp );
         csTag += szTemp;
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "Tag", csTag );
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "Text", cpcMsgText );
         if ( bBeep )
            SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "Beep", "Y" );

         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "Value", pchReturnBuffer );
         SetAttributeFromInteger( pXRA->m_vXRA_Delta, "MsgBox", "MaxLength", uBufferLth );
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "OK", "Y" );
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "Cancel", "Y" );
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "Default", "OK" );
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox", "Question", "Y" );

         // Disabling the Wnd getting the MessageBox.
         nRC = SetCursorFirstEntity( pXRA->m_vXRA_Delta, "Wnd", 0 );
         if ( nRC >= zCURSOR_SET )
         {
            SetAttributeFromString( pXRA->m_vXRA_Delta, "Wnd", "Enabled", "N" );
         }

         zCHAR szBuffer[ zWEB_COMMUNICATION_BUFFER_SIZE + 2 ];
         zCHAR szYN[ 2 ];

         // Hold on to the current Delta and restore it after SendReceive
         // (which will delete it).
         zVIEW vDelta;
         ActivateOI_FromOI_ForTask( &vDelta, pXRA->m_vXRA_Delta, 0, zSINGLE );

         // Show the message box.
         fnSendReceiveXML( pZSubtask->m_pZSocketTask, szBuffer );

         pXRA->m_vXRA_Delta = vDelta;

#ifdef DEBUG_ALL
         SetNameForView( pXRA->m_vXRA_Delta, "__XRA_Delta", vSubtask, zLEVEL_SUBTASK );
#endif

         GetStringFromAttribute( szYN, sizeof( szYN ), pXRA->m_vXRA_Delta, "MsgBox", "OK" );
         if ( szYN[ 0 ] )
         {
            nRC = zRESPONSE_OK;
            GetStringFromAttribute( pchReturnBuffer, pXRA->m_vXRA_Delta, "MsgBox", "Value" );
         }
         else
            nRC = zRESPONSE_CANCEL;

         while ( DeleteEntity( pXRA->m_vXRA_Delta, "MsgBox", zREPOS_PREV ) == zCURSOR_SET )
         {
         }

         // Enabling the Wnd when the MessageBox has been answered.
         nRC = SetCursorFirstEntity( pXRA->m_vXRA_Delta, "Wnd", 0 );
         if ( nRC >= zCURSOR_SET )
         {
            SetAttributeFromString( pXRA->m_vXRA_Delta, "Wnd", "Enabled", "Y" );
         }
      }
   }

#else

   if ( bBeep )
   {
      ::MessageBeep( MB_ICONQUESTION );
      if ( bBeep == 1 )
         bBeep = 0;
   }

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      HCURSOR hSaveCursor = ::SetCursor( ::LoadCursor( 0, IDC_WAIT ) );
      zLONG lFlags = 0x01000000;
      nRC = ConstructZeidonPrompt( vSubtask, cpcTitle, cpcMsgText,
                                   pchReturnBuffer, uBufferLth,
                                   &lFlags, " " /*"Input:"*/, (char) bBeep, 0 );
      if ( nRC == IDYES )
         nRC = zRESPONSE_YES;
      else
      if ( nRC == IDNO )
         nRC = zRESPONSE_NO;
      else
      if ( nRC == IDOK )
         nRC = zRESPONSE_OK;
      else
      if ( nRC == IDCANCEL )
         nRC = zRESPONSE_CANCEL;
      else
      if ( nRC == IDABORT )
         nRC = zRESPONSE_ABORT;
      else
      if ( nRC == IDRETRY )
         nRC = zRESPONSE_RETRY;
      else
      if ( nRC == IDIGNORE )
         nRC = zRESPONSE_IGNORE;
      else
         nRC = -1;

      ::SetCursor( hSaveCursor );
   }

   if ( bMsgFound )
      ::PostMessage( msg.hwnd, g_uMsgPostedClose, msg.wParam, msg.lParam );

#endif

   return( nRC );
}

#if 0
<html>
<body>

<script type="text/javascript">
alert("Hello again! This is how we" + '\n' + "add line breaks into an alert box")
</script>

</body>
</html>
#endif

//./ ADD NAME=OperatorPrompt
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OperatorPrompt
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:    To display a message box prompting the operator for a
//              response.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window.
//              cpcTitle    - An optional title for the message box
//              cpcMessage  - The message to be displayed
//              bBeep       - 0 - No beep
//                            1 - Annoy the operator with a beep
//
//              nButtons    - Buttons to place on the window, values are
//                            0 - OK button
//                            zBUTTONS_YESNO - Yes and No
//                            zBUTTONS_YESNOCANCEL - Yes, No, Cancel
//                            zBUTTONS_OK    - OK button only
//                            zBUTTONS_OKCANCEL - OK and CANCEL
//                            zBUTTONS_RETRYCANCEL - Retry and Cancel
//                            zBUTTONS_ABORTRETRYIGNORE - Abort,Retry,Ignore
//
//              nDefaultButton 0 - Default button is first button
//                            zRESPONSE_YES     - Default is YES
//                            zRESPONSE_NO      - Default is NO
//                            zRESPONSE_CANCEL  - Default is CANCEL
//                            zRESPONSE_OK      - Default is OK
//                            zRESPONSE_ABORT   - Default is Abort
//                            zRESPONSE_RETRY   - Default is Retry
//                            zRESPONSE_IGNORE  - Default is Ignore
//
//              nIcon       - 0 - default based on buttons
//                                * Information icon for OK
//                                * Question mark for yes/no type
//                                * Exclamation mark for OK/CANCEL
//                                * Error (stop) icon for retry cancel
//
//                            zICON_INFORMATION - Information Icon
//                            zICON_QUESTION    - Question mark icon
//                            zICON_EXCLAMATION - Exclamation mark icon
//                            zICON_ERROR       - Error (STOP) Icon (SystemModal)
//                            zICON_STOP        - Error (STOP) Icon (TaskModal)
//
//
//  RETURNS: -  zRESPONSE_YES     - Operator responded YES
//              zRESPONSE_NO      - Operator responded NO
//              zRESPONSE_CANCEL  - Operator responded CANCEL
//              zRESPONSE_OK      - Operator responded OK
//              zRESPONSE_ABORT   - Operator responded Abort
//              zRESPONSE_RETRY   - Operator responded Retry
//              zRESPONSE_IGNORE  - Operator responded Ignore
//              -1 - Error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
OperatorPrompt( zVIEW    vSubtask,
                zCPCHAR  cpcTitle,
                zCPCHAR  cpcMsgText,
                zSHORT   bBeep,
                zSHORT   nButtons,
                zSHORT   nDefaultButton,
                zSHORT   nIcon )
{
   static zCHAR cShowPopup = ' ';
   ZSubtask *pZSubtask = 0;
   zVIEW    vTemp;

   if ( vSubtask == 0 || vSubtask->hViewOD == 0 ||
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) )
   {
      zVIEW vSubtaskHold = vSubtask;

      if ( vSubtask == 0 || vSubtask->hViewOD == 0 )
      {
         vTemp = GetDefaultViewForActiveTask( );
         if ( vTemp )
            vSubtask = vTemp;
      }

      GetViewByName( &vSubtask, "__ZDrSubtask", vSubtask, zLEVEL_TASK );
      if ( vSubtask )
         GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
      else
         vSubtask = vSubtaskHold;
   }

   if ( pZSubtask && (pZSubtask->m_ulSubtaskFlags & zSUBTASK_NO_ERROR_PROMPT) )
   {
      TraceLineS( cpcTitle, cpcMsgText );
      return( -1 );
   }

   if ( vSubtask && cShowPopup == ' ' )
   {
      LPTASK lpTask = (LPTASK) zGETPTR( vSubtask->hTask );

   // if ( lpTask && lpTask->bServer )  don't care if server or not
      {
         zCHAR sz[ zMAX_FILENAME_LTH ];

         // Check a flag in the Zeidon INI to see if we should pop up the msg.
         SysReadZeidonIni( -1, "[Debug]", "ServerDebug", sz, sizeof( sz ) );
         if ( *sz == 0 || ((*sz && *(sz + 1) == 0) && (*sz == 'Y' || *sz == 'y' || *sz == '1')) )
         {
            cShowPopup = 'Y';
         }
         else
         {
            cShowPopup = 'N';
            if ( *sz && *(sz + 1) )  // assume log file name
            {
               zCHAR szLogFileName[ zMAX_FILENAME_LTH ];
               zLONG hLogFile;

               SysConvertEnvironmentString( szLogFileName, sizeof( szLogFileName ), sz );
               if ( (hLogFile = fnSysOpenFile( lpTask, szLogFileName, COREFILE_APPEND )) != -1 )
               {
                  fnSysWriteLine( lpTask, hLogFile, 0, cpcMsgText );
                  fnSysCloseFile( lpTask, hLogFile, 0 );
               }
               else
               {
                  TraceLine( "Message Handler Error opening log file (%s) for message: %s",
                             szLogFileName, cpcMsgText );
               }
            }
         }
      }
   // else
   //    cShowPopup = 'Y';
   }

   if ( cShowPopup == 'N' )
      return( 0 );

   zBOOL  bMsgFound = FALSE;
   MSG    msg;
   zSHORT nRC;

   if ( PeekMessage( &msg, 0, g_uMsgPostedClose, g_uMsgPostedClose, PM_REMOVE | PM_NOYIELD ) )
   {
      bMsgFound = TRUE;
   }

#ifdef zREMOTE_SERVER

   SysMutexLock( vSubtask, "ZDm", 0, 0 );
   g_lMsgTag++;
   zLONG lMsgTag = g_lMsgTag;
   SysMutexUnlock( vSubtask, "ZDm", 0 );

   if ( pZSubtask && pZSubtask->m_pZSocketTask )
   {
   // TraceLineS( "OperatorPrompt ZSubtask: ", *(pZSubtask->m_pzsWndTag) );
   // pZSubtask->m_ulSubtaskFlags |= zSUBTASK_APPLICATION_OPERATION;

      ZXRA_Item *pXRA = pZSubtask->m_pZSocketTask->
                    m_pCacheWndList->Find( *(pZSubtask->m_pzsRemoteWndTag),
                                           pZSubtask, "OperatorPrompt" );
      if ( pXRA )
      {
         if ( pXRA->m_vXRA_Delta == 0 )
         {
            pXRA->ActivateEmptyDelta( pZSubtask->m_pZSocketTask->
                                                        m_vRegisteredSubtask,
                                      pZSubtask->m_pZSocketTask->
                                                        m_szAppTag );
         }

         while ( ResetViewFromSubobject( pXRA->m_vXRA_Delta ) == 0 )
         {
         }

         while ( CheckExistenceOfEntity( pXRA->m_vXRA_Delta,
                                         "MsgBox" ) == zCURSOR_SET )
         {
            DeleteEntity( pXRA->m_vXRA_Delta, "MsgBox", zREPOS_FIRST );
         }

         CreateEntity( pXRA->m_vXRA_Delta, "MsgBox", zPOS_AFTER );
         CString csTag = cpcTitle;
         csTag += ':';
         zCHAR szTemp[ 16 ];
         _ltoa_s( lMsgTag, szTemp );
         csTag += szTemp;
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                 "Tag", csTag );
         SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                 "Text", cpcMsgText );
         if ( bBeep )
            SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                    "Beep", "Y" );

         switch ( nButtons )
         {
            case zBUTTONS_YESNO:
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Yes", "Y" );
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "No", "Y" );
               if ( nIcon == 0 )
                  nIcon = zICON_QUESTION;

               if ( nDefaultButton == zRESPONSE_NO )
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "No" );
               else
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Yes" );
               break;

            case zBUTTONS_YESNOCANCEL:
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Yes", "Y" );
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "No", "Y" );
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Cancel", "Y" );
               if ( nIcon == 0 )
                  nIcon = zICON_QUESTION;

               if ( nDefaultButton == zRESPONSE_NO )
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "No" );
               else
               if ( nDefaultButton == zRESPONSE_CANCEL )
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Cancel" );
               else
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Yes" );
               break;

            case zBUTTONS_OKCANCEL:
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "OK", "Y" );
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Cancel", "Y" );
               if ( nIcon == 0 )
                  nIcon = zICON_EXCLAMATION;

               if ( nDefaultButton == zRESPONSE_CANCEL )
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Cancel" );
               else
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "OK" );
               break;

            case zBUTTONS_RETRYCANCEL:
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Retry", "Y" );
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Cancel", "Y" );
               if ( nIcon == 0 )
                  nIcon = zICON_STOP;

               if ( nDefaultButton == zRESPONSE_CANCEL )
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Cancel" );
               else
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Retry" );
               break;

            case zBUTTONS_ABORTRETRYIGNORE:
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Abort", "Y" );
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Retry", "Y" );
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Ignore", "Y" );
               if ( nIcon == 0 )
                  nIcon = zICON_STOP;

               if ( nDefaultButton == zRESPONSE_RETRY )
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Retry" );
               else
               if ( nDefaultButton == zRESPONSE_IGNORE )
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Ignore" );
               else
                  SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                          "Default", "Abort" );
               break;

   //       case zBUTTONS_OK:
            default:
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "OK", "Y" );
               if ( nIcon == 0 )
                  nIcon = zICON_INFORMATION;

               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Default", "OK" );
         }

         if ( nIcon )
         {
            if ( nIcon == zICON_INFORMATION )
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Information", "Y" );
            else
            if ( nIcon == zICON_QUESTION )
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Question", "Y" );
            else
            if ( nIcon == zICON_EXCLAMATION )
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Exclamation", "Y" );
            else
            if ( nIcon == zICON_ERROR )
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Error", "Y" );
            else
               SetAttributeFromString( pXRA->m_vXRA_Delta, "MsgBox",
                                       "Stop", "Y" );
         }

         // Disabling the Wnd getting the MessageBox.
         nRC = SetCursorFirstEntity( pXRA->m_vXRA_Delta, "Wnd", 0 );
         if ( nRC >= zCURSOR_SET )
         {
            SetAttributeFromString( pXRA->m_vXRA_Delta, "Wnd",
                                    "Enabled", "N" );
         }

         zCHAR szBuffer[ zWEB_COMMUNICATION_BUFFER_SIZE + 2 ];
         zCHAR szYN[ 2 ];

         // Hold on to the current Delta and restore it after SendReceive
         // (which will delete it).
         zVIEW vDelta;
         ActivateOI_FromOI_ForTask( &vDelta, pXRA->m_vXRA_Delta, 0, zSINGLE );

         // Show the message box.
         fnSendReceiveXML( pZSubtask->m_pZSocketTask, szBuffer );

         pXRA->m_vXRA_Delta = vDelta;

#ifdef DEBUG_ALL
         SetNameForView( pXRA->m_vXRA_Delta, "__XRA_Delta",
                         vSubtask, zLEVEL_SUBTASK );
#endif

         zVIEW  vXRA_State = pZSubtask->m_pZSocketTask->m_vXRA_State;

         GetStringFromAttribute( szYN, sizeof( szYN ), vXRA_State, "MsgBox", "OK" );
         if ( szYN[ 0 ] )
            nRC = zRESPONSE_OK;
         else
         {
            GetStringFromAttribute( szYN, sizeof( szYN ), vXRA_State, "MsgBox", "Cancel" );
            if ( szYN[ 0 ] )
               nRC = zRESPONSE_CANCEL;
            else
            {
               GetStringFromAttribute( szYN, sizeof( szYN ), vXRA_State, "MsgBox", "Yes" );
               if ( szYN[ 0 ] )
                  nRC = zRESPONSE_YES;
               else
               {
                  GetStringFromAttribute( szYN, sizeof( szYN ), vXRA_State, "MsgBox", "No" );
                  if ( szYN[ 0 ] )
                     nRC = zRESPONSE_NO;
                  else
                  {
                     GetStringFromAttribute( szYN, sizeof( szYN ), vXRA_State,
                                             "MsgBox", "Abort" );
                     if ( szYN[ 0 ] )
                        nRC = zRESPONSE_ABORT;
                     else
                     {
                        GetStringFromAttribute( szYN, sizeof( szYN ), vXRA_State,
                                                "MsgBox", "Retry" );
                        if ( szYN[ 0 ] )
                           nRC = zRESPONSE_RETRY;
                        else
                        {
                           GetStringFromAttribute( szYN, sizeof( szYN ), vXRA_State,
                                                   "MsgBox", "Ignore" );
                           if ( szYN[ 0 ] )
                              nRC = zRESPONSE_IGNORE;
                           else
                              nRC = -1;
                        }
                     }
                  }
               }
            }
         }

         while ( DeleteEntity( pXRA->m_vXRA_Delta,
                               "MsgBox", zREPOS_PREV ) == zCURSOR_SET )
         {
         }

         // Enabling the Wnd when the MessageBox has been answered.
         if ( SetCursorFirstEntity( pXRA->m_vXRA_Delta, "Wnd", 0 ) >= zCURSOR_SET )
         {
            SetAttributeFromString( pXRA->m_vXRA_Delta, "Wnd",
                                    "Enabled", "Y" );
         }
      }
   }

#else

   zUSHORT uStyle;

   if ( nIcon == zICON_ERROR )
      uStyle = MB_SYSTEMMODAL;
   else
      uStyle = MB_TASKMODAL;

   if ( nDefaultButton == 0 )
      uStyle |= MB_DEFBUTTON1;

   switch ( nButtons )
   {
      case zBUTTONS_YESNO:
         uStyle |= MB_YESNO;
         if ( nIcon == 0 )
            uStyle |= MB_ICONQUESTION;
         if ( nDefaultButton == zRESPONSE_NO )
            uStyle |= MB_DEFBUTTON2;
         else
            uStyle |= MB_DEFBUTTON1;

         break;

      case zBUTTONS_YESNOCANCEL:
         uStyle |= MB_YESNOCANCEL;
         if ( nIcon == 0 )
            uStyle |= MB_ICONQUESTION;
         if ( nDefaultButton == zRESPONSE_NO )
            uStyle |= MB_DEFBUTTON2;
         else
         if ( nDefaultButton == zRESPONSE_CANCEL )
            uStyle |= MB_DEFBUTTON3;
         else
            uStyle |= MB_DEFBUTTON1;

         break;

      case zBUTTONS_OKCANCEL:
         uStyle |= MB_OKCANCEL;
         if ( nIcon == 0 )
            uStyle |= MB_ICONEXCLAMATION;
         if ( nDefaultButton == zRESPONSE_CANCEL )
            uStyle |= MB_DEFBUTTON2;
         else
            uStyle |= MB_DEFBUTTON1;

         break;

      case zBUTTONS_RETRYCANCEL:
         uStyle |= MB_RETRYCANCEL;
         if ( nIcon == 0 )
            uStyle |= MB_ICONSTOP;
         if ( nDefaultButton == zRESPONSE_CANCEL )
            uStyle |= MB_DEFBUTTON2;
         else
            uStyle |= MB_DEFBUTTON1;

         break;

      case zBUTTONS_ABORTRETRYIGNORE:
         uStyle |= MB_ABORTRETRYIGNORE;
         if ( nIcon == 0 )
            uStyle |= MB_ICONSTOP;
         if ( nDefaultButton == zRESPONSE_RETRY )
            uStyle |= MB_DEFBUTTON2;
         else
         if ( nDefaultButton == zRESPONSE_IGNORE )
            uStyle |= MB_DEFBUTTON3;
         else
            uStyle |= MB_DEFBUTTON1;

         break;

//    case zBUTTONS_OK:
      default:
         uStyle |= MB_OK;
         if ( nIcon == 0 )
            uStyle |= MB_ICONINFORMATION;
   }

   if ( nIcon )
   {
      if ( nIcon == zICON_INFORMATION )
         uStyle |= MB_ICONINFORMATION;
      else
      if ( nIcon == zICON_QUESTION )
         uStyle |= MB_ICONQUESTION;
      else
      if ( nIcon == zICON_EXCLAMATION )
         uStyle |= MB_ICONEXCLAMATION;
      else
         uStyle |= MB_ICONSTOP;     // zICON_ERROR or zICON_STOP
   }

   // beep at the user if requested
   if ( bBeep )
   {
      if ( uStyle & MB_ICONINFORMATION )
         ::MessageBeep( MB_ICONASTERISK );
      else
      if ( uStyle & MB_ICONEXCLAMATION )
         ::MessageBeep( MB_ICONEXCLAMATION );
      else
      if ( uStyle & MB_ICONSTOP )
         ::MessageBeep( MB_ICONHAND );
      else
         ::MessageBeep( MB_ICONQUESTION );
   }

   // Show the message box.
   nRC = ::MessageBox( GetActiveWindow( ), cpcMsgText, cpcTitle, uStyle );

   if ( nRC == IDYES )
      nRC = zRESPONSE_YES;
   else
   if ( nRC == IDNO )
      nRC = zRESPONSE_NO;
   else
   if ( nRC == IDOK )
      nRC = zRESPONSE_OK;
   else
   if ( nRC == IDCANCEL )
      nRC = zRESPONSE_CANCEL;
   else
   if ( nRC == IDABORT )
      nRC = zRESPONSE_ABORT;
   else
   if ( nRC == IDRETRY )
      nRC = zRESPONSE_RETRY;
   else
   if ( nRC == IDIGNORE )
      nRC = zRESPONSE_IGNORE;
   else
      nRC = -1;

#endif

#if 0
   if ( pZSubtask )
   {
   // pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_APPLICATION_OPERATION;
   }
#endif

   if ( bMsgFound )
      ::PostMessage( msg.hwnd, g_uMsgPostedClose, msg.wParam, msg.lParam );

   return( nRC );
}

//./ ADD NAME=OperatorConversationalDialog
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OperatorConversationalDialog
//                                              04/01/93 Modified: 10/18/00
//
//  PURPOSE:   To display a message box prompting the operator for input.
//
//  PARAMETERS: vSubtask      - The subtask view for the current window.
//              pchReturnCtrlTag - The return Tag of the control causing
//                              the dialog to close (this should point to
//                              a memory area at least zTAG_LTH bytes long). If
//                              pchReturnCtrlTag is zero, the Tag will not
//                              be returned.
//              cpcDlgTag - Dialog name
//              cpcWndTag - Window name on which to base the dialog
//
//    RETURNS: -   0 - Success
//                -1 - vSubtask is not valid
//                -2 - The dialog name requested could not be found
//                -3 - The window name requested could not be found
//
//    NOTES:   -  There are only two action behaviors that are honored by
//                this dialog:
//                              zWAB_ReturnToParent
//                              zWAB_ReturnToParentWithRefresh
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
OperatorConversationalDialog( zVIEW   vSubtask,
                              zPCHAR  pchReturnCtrlTag,
                              zCPCHAR cpcDlgTag,
                              zCPCHAR cpcWndTag )
{
   ZSubtask *pZSubtask;
   zPCHAR   pch;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      zVIEW  vDialog;
      zCHAR  szTagBuffer[ zTAG_LTH ];

      if ( pchReturnCtrlTag )
         pch = pchReturnCtrlTag;
      else
         pch = szTagBuffer;

      *pch = 0;

      if ( CreateWindowSubtask( &vDialog, vSubtask, 0, cpcDlgTag, 0 ) )
      {
         MessageSend( vSubtask, 0, "Zeidon Application",
                      "Cannot create Zeidon subtask",
                      zMSGQ_MODAL_ERROR, FALSE );
         return( -2 );  // error
      }

      if ( SetCursorFirstEntityByString( vDialog, szlWnd, szlTag,
                                         cpcWndTag, 0 ) <= zCURSOR_UNCHANGED )
      {
         zCHAR szMessage[ 256 ];

         sprintf_s( szMessage, sizeof( szMessage ),
                   "(drvr) Window name: %s not found in Dialog: %s",
                   cpcDlgTag, cpcWndTag );
         MessageSend( vSubtask, 0, "Zeidon Application",
                      szMessage, zMSGQ_MODAL_ERROR, FALSE );
         return( -3 );  // error
      }

      // If we are requesting a conversational dialog from a splitter
      // window, make the primary subtask's frame the parent.
      ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
      if ( pZSubtaskMain )
         pZSubtask = pZSubtaskMain;

      ZModalDlg dlg( pch, pZSubtask, cpcDlgTag, cpcWndTag, vDialog );
      dlg.DoModal( );
      pZSubtask->m_pZTask->m_pZSubtaskConversational = 0;
      return( 0 );
   }

   return( -1 );
}

#if 0
//./ ADD NAME=SetMessageQueueTaskStatus
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetMessageQueueTaskStatus
//                                              04/30/93 Modified: 04/30/93
//             NOTE:  this operation is currently deactivated!!!!
//
//  PURPOSE:   To set the Message Queue object's Task entity Status attribute.
//
//  PARAMETERS: vSubtask     - The subtask view.
//              lStatus      - The new status to be set.
//
//  RETURNS:    -1  - Error locating window
//              -2  - No message queue object
//              -3  - No Status attribute
//               0  - Status set as specified
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
SetMessageQueueTaskStatus( zVIEW vSubtask,
                           zLONG lStatus )
{
   ZSubtask *pZSubtask;
   zPCHAR   pchStatus;

   // If this operation is ever activated ... it should probably move
   // to CORE ... at the least is should verify the Task Id.
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_vMsgQ )
      {
         GetAddrForAttribute( (zPCHAR *) &pchStatus, pZSubtask->m_vMsgQ,
                              szlTask, szlStatus );
         if ( pchStatus )
         {
            *((zPLONG) pchStatus) = lStatus;
            return( 0 );
         }

         return( -3 );
      }

      return( -2 );
   }

   return( -1 );
}

//./ ADD NAME=SetMessageStatusFromTask
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetMessageStatusFromTask
//                                              04/30/93 Modified: 04/30/93
//             NOTE:  this operation is currently deactivated!!!!
//
//  PURPOSE:   To set the Message Queue object's MsgQ entity (at the current
//             cursor position) Status attribute from the current setting
//             on the Task entity Status attribute.
//
//  PARAMETERS: vSubtask     - The subtask view.
//              lStatus      - The new status to be set.
//
//  RETURNS:    -1  - Error locating window
//              -2  - No message queue object
//              -3  - No Status attribute
//               0  - Status set as specified
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
SetMessageStatusFromTask( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;
   zPCHAR   pchStatus;

   // If this operation is ever activated ... it should probably move
   // to CORE ... at the least it should verify the Task Id.
   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_vMsgQ )
      {
         GetAddrForAttribute( (zPCHAR *) &pchStatus, pZSubtask->m_vMsgQ,
                              szlTask, szlStatus );
         if ( pchStatus )
         {
            SetAttributeFromInteger( pZSubtask->m_vMsgQ, szlQMsg, szlStatus,
                                     *((zPLONG) pchStatus) );
            return( 0 );
         }

         return( -3 );
      }

      return( -2 );
   }

   return( -1 );
}
#endif

//./ ADD NAME=GetPressedState
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetPressedState
//                                              08/05/99 Modified: 08/05/99
//
//  PURPOSE:   To determine the state of the specified virtual key.  The
//             virtual key may be VK_LBUTTON, VK_CONTROL, etc.
//
//  PARAMETERS: lVirtualKey - The key for which the query is directed.
//              nAsync      - 0 ==> get current state of the key
//                            1 ==> get state of key associated with event
//  // not implemented        2 ==> get state of key associated with action
//
//  RETURNS:    TRUE  - key is pressed.
//              FALSE - key is not pressed.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
GetPressedState( zLONG  lVirtualKey,
                 zSHORT nAsync )
{
#if 0
   if ( nAsync == 2 )
   {
      ZSubtask *pZSubtask;
      zVIEW     vSubtask;

      GetViewByName( &vSubtask, "__ZDrSubtask", 0, zLEVEL_TASK );
      if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
      {
         zLONG lModKeys = pZSubtask->m_uPressedFlags;
         lModKeys &= 0x03FF;     // scan code + extended flag + ZDr flag
         lVirtualKey &= 0x01FF;  // scan code + extended flag

         if ( lModKeys & 0x0200 )   // ZDr mouse flag set
            return( (lModKeys & lVirtualKey) ? TRUE : FALSE );
         else
            return( (lModKeys & lVirtualKey) == lVirtualKey ? TRUE : FALSE );
      }
   }
#endif

   if ( nAsync )
      return( (GetAsyncKeyState( lVirtualKey ) & 0x8000) );
   else
      return( (GetKeyState( lVirtualKey ) < 0) );
}

//./ ADD NAME=IsDoubleClick
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: IsDoubleClick
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To determine if the current click is part of a double click.
//
//  PARAMETERS: vSubtask     - The subtask view for the window containing
//                             the control.
//              cpcOptionTag - The unique name of the option
//
//  RETURNS:    True  - current click is part of a double-click
//              FALSE - current click is not part of a double-click ...
//                      OR the specified window was not located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
IsDoubleClick( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag )
{
   HWND     hWnd;
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      MSG     msg;
      zULONG  ulInterval = 0;
      zLONG   lMessage = GetMessageTime( );
      zULONG  ulDblclkTime = GetDoubleClickTime( );
      zSHORT  nMoves = 0;

      if ( cpcCtrlTag )
         hWnd = pzma->m_pCtrl->m_hWnd;
      else
         hWnd = *(pZSubtask->m_pZFWnd);

      while ( ulInterval < ulDblclkTime )
      {
         ulInterval = (zULONG) (GetCurrentTime( ) - lMessage);
         if ( ::PeekMessage( &msg, hWnd, WM_LBUTTONDBLCLK, WM_LBUTTONDBLCLK,
                             PM_NOREMOVE | PM_NOYIELD ) )
         {
            if ( ulInterval < ulDblclkTime )
               return( TRUE );
         }
         else
         if ( ::PeekMessage( &msg, hWnd, WM_MOUSEMOVE, WM_MOUSEMOVE,
                             PM_REMOVE | PM_NOYIELD ) )
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            nMoves++;
            if ( nMoves > 2 )
            {
               return( FALSE );
            }
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

//./ ADD NAME=IsKeyQueued
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: IsKeyQueued
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE: To determine if the specified key is in the message queue.
//           A mini message loop is set up.  Each time PeekMessage returns,
//           the scheduler is giving the app some processing time.  If there
//           is a pending keydown message, it is pulled from the queue and
//           discarded.  If the keydown message is for the specified key,
//           return TRUE, otherwise return FALSE to permit the calling app
//           to do a chunk of work.
//
//  PARAMETERS: vSubtask  - The subtask view for the window containing
//                          the control.
//              uVirtualKey - The virtual key code of the requested key.
//              bAsync - TRUE - messages are passed on to the app
//                     - FALSE - messages are removed from the queue
//
//  RETURNS:    TRUE  - a key-stroke for the requested key has occurred.
//              FALSE - a key-stroke for the requested key has not yet
//                      occurred.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
IsKeyQueued( zVIEW   vSubtask,
             zUSHORT uVirtualKey,
             zLONG   bAsync )
{
   ZSubtask    *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      HWND hWnd = *(pZSubtask->m_pZFWnd);

      // if any key has been pressed, pull key down (and up) from the queue
      MSG  msg;

      if ( bAsync )
      {
         while ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
         {
            // if the key is the requested key, return TRUE to the user.  Note
            // that all other strokes for this application are being discarded.
            if ( msg.message == WM_KEYDOWN )
            {
               if ( msg.wParam == uVirtualKey )
                  return( TRUE );
            }
            else
            {
               ::TranslateMessage( &msg );
               ::DispatchMessage( &msg );
            }
         }
      }
      else
      {
//       while ( ::PeekMessage( &msg, hWnd, WM_KEYDOWN, WM_KEYDOWN,
//                              PM_REMOVE | PM_NOYIELD ) )
         while ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
         {
            // if the key is the requested key, return TRUE to the user.  Note
            // that all key strokes for this application are being discarded.
            if ( msg.message == WM_KEYDOWN && msg.wParam == uVirtualKey )
               return( TRUE );
         }
      }
   }

   return( FALSE );
}

//./ ADD NAME=InvokeHelpWithContext
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: InvokeHelpWithContext
//                                              04/14/93 Modified: 04/14/93
//
//  PURPOSE:   To present standard Windows Help using a context sensitive
//             string of the users choice or automatically generated from
//             the dialog.window.control names.
//
//  PARAMETERS: vSubtask  - The subtask view for the parent window
//              cpcCtrlTag - Control tag (if zero, defaults to the
//                           last control with focus in this window).
//              cpcHelpFile - Help file name (if zero, defaults to
//                            the dialog_name.hlp)
//              cpcHelpContext - Context string (if zero, defaults to
//                               dlg.wnd.ctrl).  Used with help type of
//                               HELP_KEY or HELP_MULTIKEY.
//              ulHelpContextNbr - Help context number for topic used with
//                                 help type of HELP_CONTEXT or HELP_SETINDEX.
//              uHelpType - Help type ... one of the following
//
//                           HELP_INDEX           (see Windows Help API
//                           HELP_HELPONHELP       for details)
//                           HELP_SETINDEX
//                           HELP_CONTEXT
//                           HELP_KEY
//                           HELP_MULTIKEY
//
//  RETURNS:    -2  - Memory allocation error
//              -1  - Error locating window
//               0  - help successfully presented
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
InvokeHelpWithContext( zVIEW    vSubtask,
                       zCPCHAR  cpcCtrlTag,
                       zCPCHAR  cpcHelpFile,
                       zCPCHAR  cpcHelpContext,
                       zULONG   ulHelpContextNbr,
                       zUSHORT  uHelpType )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      zCHAR  szHelpFile[ 256 ];
      zCHAR  szHelpContext[ 256 ];
      HANDLE hmkh;
      MULTIKEYHELP FAR *mkh;
      zUSHORT usSize;

      if ( cpcHelpFile && cpcHelpFile[ 0 ] )
         strcpy_s( szHelpFile, sizeof( szHelpFile ), cpcHelpFile );
      else
      {
         GetStringFromAttribute( szHelpFile, sizeof( szHelpFile ), pZSubtask->m_vDialog, szlDlg, szlDLL );
         strcat_s( szHelpFile, sizeof( szHelpFile ), ".hlp" );
      }

      // this is here so that the driver knows that help has been invoked
      // and will at the appropriate time call HELP_QUIT
      mDeleteInit( pZSubtask->m_pZTask->m_pzsHelpFile );
      pZSubtask->m_pZTask->m_pzsHelpFile = new CString( szHelpFile );

      if ( cpcHelpContext && cpcHelpContext[ 0 ] )
         strcpy_s( szHelpContext, sizeof( szHelpContext ), cpcHelpContext );
      else
      {
         strcpy_s( szHelpContext, sizeof( szHelpContext ), *(pZSubtask->m_pzsDlgTag) );
         usSize = (zSHORT) zstrlen( szHelpContext );
         if ( usSize )
            szHelpContext[ usSize++ ] = '.';

         strcpy_s( szHelpContext + usSize, sizeof( szHelpContext - usSize ), *(pZSubtask->m_pzsWndTag) );
         usSize += (zSHORT) zstrlen( szHelpContext + usSize );
         szHelpContext[ usSize++ ] = '.';

         if ( cpcCtrlTag && cpcCtrlTag[ 0 ] )
            strcpy_s( szHelpContext + usSize, sizeof( szHelpContext - usSize ), cpcCtrlTag );
         else
         if ( pZSubtask->m_pzmaWithFocus )
            strcpy_s( szHelpContext + usSize, sizeof( szHelpContext - usSize ), *(pZSubtask->m_pzmaWithFocus->m_pzsTag) );
      }

      TraceLineS( "dlgo - Calling WinHelp with help file - ", szHelpFile );
      TraceLineS( "                            context   - ", szHelpContext );
      TraceLineI( "                            context # - ", ulHelpContextNbr );
      TraceLineI( "                            type      - ", uHelpType );

      switch ( uHelpType )
      {
         case HELP_INDEX:
         case HELP_HELPONHELP:
            WinHelp( *(pZSubtask->m_pZFWnd), szHelpFile, uHelpType, (DWORD) 0 );
            break;

         case HELP_CONTEXT:
         case HELP_SETINDEX:
            WinHelp( *(pZSubtask->m_pZFWnd), szHelpFile, uHelpType, (DWORD) ulHelpContextNbr );
            break;

         case HELP_KEY:
            WinHelp( *(pZSubtask->m_pZFWnd), szHelpFile, uHelpType, (DWORD) szHelpContext );
            break;

         case HELP_MULTIKEY:
            usSize = sizeof( MULTIKEYHELP ) + (zSHORT) zstrlen( szHelpContext );
            hmkh = GlobalAlloc( GHND, (DWORD) usSize );
            if ( !hmkh )
            {
               TraceLineS( "******************", "Help Allocation Error " );
               return( -2 );
            }

            mkh = (MULTIKEYHELP FAR *) GlobalLock( hmkh );
            mkh->mkSize = usSize;
            mkh->mkKeylist = 'Z';
            strcpy_s( mkh->szKeyphrase, sizeof( mkh->szKeyphrase ), szHelpContext );
            WinHelp( *(pZSubtask->m_pZFWnd), szHelpFile, HELP_MULTIKEY, (DWORD) mkh );

            GlobalUnlock( hmkh );
            GlobalFree( hmkh );
            break;
      }

      return( 0 );
   }

   return( -1 );
}

typedef zSHORT (POPERATION zFARPROC_DYNOBJ1)( zVIEW );

//./ ADD NAME=CallObjectOperation1Param
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//
//  DIALOG OPERATION: CallObjectOperation1Param
//                                              07/15/04 Modified: 07/15/04
//
//  PURPOSE: To call an object opertion that has only one parameter, the
//           view to the object.
//
//  PARAMETERS: vObject          - The subtask view for the window containing
//                                 the control.
//              pchOperationName - Name of the operation (function) to get
//                                 the address of.
//              pchErrorPrefix   - an identifier with which to prefix the
//                                 error message if an error occurs.  Note
//                                 that if this string is empty, the error
//                                 message is suppressed.
//
//  RETURNS:    -16 - the library or the operation could not be located.
//              otw - the return code from the called operation.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zLONG OPERATION
CallObjectOperation1Param( zVIEW  vObject,
                           zPCHAR pchOperationName,
                           zPCHAR pchErrorPrefix )
{
   zFARPROC_DYNOBJ1 lpfnDynRoutine;
   LPLIBRARY hLibrary = 0;
   LPVIEWOD  lpViewOD;
   zPCHAR    pchNameDLL;
   zPCHAR    pchObjectName;
   zCHAR     szOperation[ 256 ];
   zSHORT    nRC = -1;

   lpViewOD = (LPVIEWOD) zGETPTR( vObject->hViewOD );
   pchObjectName = lpViewOD->szName;
   pchNameDLL = lpViewOD->szOperLibname;
   if ( pchNameDLL == 0 || pchNameDLL[ 0 ] == 0 )
      return( -16 );

   strcpy_s( szOperation, sizeof( szOperation ), "o" );
   strcat_s( szOperation, sizeof( szOperation ), pchObjectName );
   strcat_s( szOperation, sizeof( szOperation ), "_" );
   strcat_s( szOperation, sizeof( szOperation ), pchOperationName );

   lpfnDynRoutine = (zFARPROC_DYNOBJ1)
          GetOperationDynamicCallAddress( vObject, &hLibrary, pchNameDLL,
                                          szOperation, pchErrorPrefix );
   if ( lpfnDynRoutine )
   {
      nRC = (*lpfnDynRoutine)( vObject );
   }

   return( nRC );

} // CallObjectOperation1Param

//./ ADD NAME=CallDialogOperation
// Source Module=zdrdlgo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//
//  DIALOG OPERATION: CallDialogOperation
//                                              07/15/04 Modified: 07/15/04
//
//  PURPOSE: To call a dialog operation.
//
//  PARAMETERS: vSubtask    - The subtask view for the window.
//              pchDLL_Name - Name of the DLL containing the operation.
//              pchOperName - Name of the operation (function) to call.
//
//  RETURNS:    return code from the called operation.
//              otw - 0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
CallDialogOperation( zVIEW  vSubtask,
                     zPCHAR pchDLL_Name,
                     zPCHAR pchOperName )
{
   zSHORT    nRC = 0;

   if ( pchDLL_Name && pchDLL_Name[ 0 ] )
   {
      LPLIBRARY hLibrary = 0;
      zFARPROC_DRIVER lpfnDynRoutine = (zFARPROC_DRIVER)
         GetOperationDynamicCallAddress( vSubtask, &hLibrary, pchDLL_Name,
                                         pchOperName, "CallDialogOperation" );
      if ( lpfnDynRoutine )
      {
         nRC = (*lpfnDynRoutine)( vSubtask );
      }
   }

   TraceLine( "CallDialogOperation: %s.%s  RC: %d", pchDLL_Name, pchOperName, nRC );
   return( nRC );

} // CallObjectOperation1Param
