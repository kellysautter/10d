/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdract.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Action processing functions.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2003.03.24    DKS    Z10
//    Refresh and Map all subtasks within a splitter on automatic mapping.
//
// 2002.06.12    DKS    Z10
//    Pick up the correct window action termination on a switch action.
//
// 2002.06.11    DKS/FH
//    Fix Landesbank problem of "psychedelic windows" (problem of not
//    redrawing the topmost window).
//
// 2002.01.19    DKS    Z10
//    Prevent events from other dialog windows when a conversational
//    dialog is "in process".
//
// 2001.12.03    DKS    WEB
//    Permit ProcessEvent to be called from RemoteServer.
//
// 2001.10.12    HH
//    Handle Operation return code on return from interpreter.
//
// 2001.08.03    DKS    WEB
//    Set "Active" window attribute on ProcessReturn.
//
// 2001.06.08    DKS    Debug
//    Added debugging code to solve Action bug for TMV.
//
// 2001.05.30    BL     RemoteServer
//    Bugfix for Close Window with ReturnToTop
//
// 2001.05.22    DKS
//    Eliminated hanging hourglass on zWAB_StartPopupMenu.
//
// 2001.03.19    PAS
//    Removed TraceLineS that was too frequent.
//
// 2001.02.01    DKS    Z10    QS999
//    Corrected zWAB_StartOrFocusModelessSubwindow.
//
// 1999.07.28    DKS    Z10    FH999
//    Improved zWAB_StartOrFocusModelessApp to handle focus to another LPLR.
//
// 1999.07.08    DKS    Z10    Enhance
//    Permit OE to determine ZDr state (Dlg Tag, Wnd Tag, Current Operation)
//    given the current vSubtask.
//
// 1999.07.07    DKS    Z10    QS999
//    Fixed ASSERT and continue when SwitchToAction WAB was set.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Return zNO_APPLICATION_ACTION when there is no application action
//    defined for an event.
//
// 1999.02.19  DKS
//    EventAct search now terminated by count rather than termination flag.
//    An event with id zero is now permitted (for ActiveX events).
//
// 1999.01.21    DKS
//    Protected against null pointer after call to SfGetApplicationForSubtask.
//
// 1998.11.09  DKS
//    If we are doing an automatic zTEMPORAL_SUBOBJECT_DELETE, we want to
//    skip the CreateTemporalSubobjectVersion if not staying on the same
//    window.
//
// 1998.10.08  DKS
//    Added check for Tracing driver action messages.
//
// 1998.10.05  DKS
//    Corrected algorithm for locating a Subtask across tasks.
//
// 1998.09.23  DKS
//    Return the correct return code from fnProcessDriverOperationCode.
//
// 1998.09.17    DKS
//    Fix for ASSERT for TerminateActionForError (TB 165)
//
// 1998.08.13    DKS
//    Comment the reason for returning -1 from ProcessImmediateEvent.
//
// 1998.08.13    DKS
//    Trace WAB with text value.
//
// 1997.10.30  DKS
//    The application is now put into a state in which user input is ignored
//    during the processing of an action.  MapFromOI will no-op and return if
//    called recursively.
//
// 1997.10.21  DonC
//    Modified ProcessAction to eliminate "if test" after setting cursor prev
//    on undefined cursor position.  The cursor prev correctly set position if
//    the first entity under a parent was deleted and the "if" incorrectly set
//    it forward one (extra) position.
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

// Process the action for the control.
zBOOL OPERATION
ProcessEvent( const ZMapAct *pzma,
              zLONG lEvent )
{
   if ( pzma->m_pZSubtask->m_bStopEvents == FALSE && pzma->m_pchEventAction &&
        (pzma->m_pZSubtask->m_pZTask->m_pZSubtaskConversational == 0 ||
         pzma->m_pZSubtask->m_pZTask->m_pZSubtaskConversational == pzma->m_pZSubtask) )
   {
      zSHORT k;
      zLONG  l;

      for ( k = 0; k < pzma->m_nEventActionCnt; k++ )
      {
         // N.B.  this is NO GOOD ==>
         //    (zLONG) *(pzma->m_pchEventAction + (k * 2 * sizeof( zLONG ))
         // because of the little endian stuff.
         l = *((zPLONG) (pzma->m_pchEventAction + (k * 2 * sizeof( zLONG ))));
         if ( lEvent == l )
         {
            zPCHAR pch;

            l = *((zPLONG) (pzma->m_pchEventAction + ((k * 2 * sizeof( zLONG )) + sizeof( zLONG ))));
            SetEntityCursor( pzma->m_pZSubtask->m_vDialog, szlAct, szlSyncIdx, zPOS_FIRST | zQUAL_INTEGER, &l, 0, 0, 0, 0, 0 );
            GetAddrForAttribute( (zPCHAR *) &pch, pzma->m_pZSubtask->m_vDialog, szlAct, szlTag );
//          TraceLineS( "Processing EventAct - ", pch );
//          TraceLineI( "           SyncIdx = ", *((zPLONG) (pzma->m_pchEventAction + k)) );
         // pzma->m_pZSubtask->m_pEventParms = 0; can't do this!
            pzma->m_pZSubtask->m_pzsLastTag = pzma->m_pzsTag;
            pzma->m_pZSubtask->m_pzsLastCtrlTag = pzma->m_pzsTag;

#ifdef zREMOTE_SERVER
            ProcessActionLoop( pzma->m_pZSubtask, pch, GetMessagePos( ), 0, 0, FALSE, "CtrlEvent" );
#else
            if ( pzma->m_pZSubtask->m_pZFWnd && mIs_hWnd( pzma->m_pZSubtask->m_pZFWnd->m_hWnd ) )
            {
               pzma->m_pZSubtask->m_pZFWnd->PostMessage( zZEIDON_PROCESS_EVENT, GetMessagePos( ), (zLONG) pch );
            }
            else
            {
               ProcessActionLoop( pzma->m_pZSubtask, pch, GetMessagePos( ), 0, 0, FALSE, "CtrlEvent" );
            }
#endif

            return( TRUE );  // the action for the event has been processed
         }
      }
   }

   return( FALSE );
}

zLONG OPERATION
ProcessImmediateEvent( const  ZMapAct *pzma,
                       zLONG  lEvent,
                       zPVOID pEventParms )
{
   if ( pzma->m_pZSubtask->m_bStopEvents == FALSE && pzma->m_pchEventAction &&
        (pzma->m_pZSubtask->m_pZTask->m_pZSubtaskConversational == 0 || pzma->m_pZSubtask->m_pZTask->m_pZSubtaskConversational == pzma->m_pZSubtask) )
   {
      zSHORT k;
      zLONG  l;

      for ( k = 0; k < pzma->m_nEventActionCnt; k++ )
      {
         // N.B.  this is NO GOOD ==>
         //    (zLONG) *(pzma->m_pchEventAction + (k * 2 * sizeof( zLONG ))
         // because of the little endian stuff.
         l = *((zPLONG) (pzma->m_pchEventAction + (k * 2 * sizeof( zLONG ))));
         if ( lEvent == l )
         {
            zPCHAR pch;

            l = *((zPLONG) (pzma->m_pchEventAction + ((k * 2 * sizeof( zLONG )) + sizeof( zLONG ))));
            SetEntityCursor( pzma->m_pZSubtask->m_vDialog, szlAct, szlSyncIdx, zPOS_FIRST | zQUAL_INTEGER, &l, 0, 0, 0, 0, 0 );
            GetAddrForAttribute( (zPCHAR *) &pch, pzma->m_pZSubtask->m_vDialog, szlAct, szlTag );
//          TraceLineS( "Processing EventAct - ", pch );

            pzma->m_pZSubtask->m_pEventParms = pEventParms;
            pzma->m_pZSubtask->m_pzsLastTag = pzma->m_pzsTag;
            pzma->m_pZSubtask->m_pzsLastCtrlTag = pzma->m_pzsTag;
#ifdef zREMOTE_SERVER
            return( ProcessActionLoop( pzma->m_pZSubtask, pch, GetMessagePos( ), 0, 0, FALSE, "CtrlEvent" ) );
#else
            if ( pzma->m_pZSubtask->m_pZFWnd && mIs_hWnd( pzma->m_pZSubtask->m_pZFWnd->m_hWnd ) )
            {
               return( pzma->m_pZSubtask->m_pZFWnd->SendMessage( zZEIDON_PROCESS_EVENT, GetMessagePos( ), (zLONG) pch ) );
            }
            else
            {
               return( ProcessActionLoop( pzma->m_pZSubtask, pch, GetMessagePos( ), 0, 0, FALSE, "CtrlEvent" ) );
            }
#endif
         }
      }
   }

   // Return a value that cannot be returned from the application action
   // associated with the event.
   return( zNO_APPLICATION_EVENT );
}

zPCHAR g_Actions[ 101 ] =
{
   "DeterminedInDialogOperation",            //   0
   "(1)StayOnWindow(NoRefresh)",             //   1
   "(2)StayOnWindowWithRefresh",             //   2

   "", "", "", "", "", "", "", "", "", "",   //  3-12
   "", "", "", "", "", "", "", "", "", "",   //  13-22
   "", "", "", "", "", "", "",               //  23-29

   "(30)StartOrFocusModelessDialog",         //  30
   "(31)StartDialogTask",                    //  31
   "(32)StartModelessWindow",                //  32
   "(zTAG_LTH)StartModelessSubwindow",       //  zTAG_LTH
   "(34)StartModalSubwindow",                //  34
   "(35)StartNewApp",                        //  35
   "(36)StartModalWindow",                   //  36
   "(37)StartOrFocusModelessApp",            //  37
   "(38)StartOrFocusModelessWindow",         //  38
   "(39)StartOrFocusModelessSubwindow",      //  39

   "(40)StartPopupMenu",                     //  40
   "(41)StartBrowserHTML_Page",              //  41
   "(42)StartEmailClient",                   //  42
   "(43)StartEmailClientBCC",                //  43
   "(44)StartEmailClientCC",                 //  44
   "(45)LinkToHTML_Address",                 //  45
   "(46)ProcessDownloadFile",                //  46
   "(47)StartBrowserPDF_Page",               //  47
   "", "", "",                               //  48-50

   "(51)ReplaceDialogWithDialog",            //  51
   "(52)ReplaceWindowWithModelessWindow",    //  52
   "(53)ReplaceWindowWithModalWindow",       //  53
   "(54)ReplaceCurrentAppWithApp",           //  54
   "(55)ReCAPTCHA Replace Window",           //  55
   "", "", "", "",                           //  56-59

   "(60)PrintWnd",                           //  60
   "(61)StartSplitWindow",                   //  61
   "(62)ReplaceSplitWindow",                 //  62
   "(63)DeleteSplitWindow",                  //  63
   "", "", "", "", "", "", "",               //  64-70

   "(71)ReturnToParent",                     //  71
   "(72)ReturnToParentWithRefresh",          //  72
   "(73)ReturnToTopWindow",                  //  73
   "(74)ReturnToTopWindowWithRefresh",       //  74
   "(75)StartTopWindow",                     //  75
   "(76)ReplaceTopWindow",                   //  76
   "",                                       //  77
   "(78)StartModalWebPopup",                 //  78
   "(79)StartModelessWebPopup",              //  79

   "", "", "", "",                           //  80-83
   "(84)ReturnFromWebPopupNoRefresh",        //  84
   "(85)ReturnFromWebPopupWithRefresh",      //  85

   "", "", "", "", "",                       //  86-90
   "", "", "", "", "", "", "", "",           //  91-98

   "(99)ExitDialogTask",                     //  99
   "???",                                    //  100
};

zOPER_EXPORT zPCHAR OPERATION
GetActionString( zLONG lAction )
{
   if ( lAction >= 0 && lAction < 100 )
      return g_Actions[ lAction ];
   else
      return g_Actions[ 100 ];
}

zLONG OPERATION
ProcessActionLoop( ZSubtask *pZSubtask,
                   zCPCHAR  cpcEntityName,
                   zULONG   ulMsgPos,
                   zVIEW    vResumeSubtask,
                   zLONG    lOperCtrl,
                   zBOOL    bEntity,
                   zCPCHAR  cpcEvent )
{
// CWaitRedraw wd;
   CWaitCursor wc;
   zLONG  lRC;

   if ( bEntity )
   {
      zPCHAR pch;

      GetAddrForAttribute( (zCOREMEM) &pch, pZSubtask->m_vDialog, cpcEntityName, szlTag );
      lRC = ProcessAction( pZSubtask, pch, vResumeSubtask, lOperCtrl, cpcEvent );
   }
   else
      lRC = ProcessAction( pZSubtask, cpcEntityName, vResumeSubtask, lOperCtrl, cpcEvent );

// TraceLineI( "RC from ActionLoop = ", lRC );
   return( lRC );
}

#define IDM_RESETTIMEOUT 218

//
// Process the requested action ... note that if vResumeSubtask is non-zero, pZSubtask has been set to
// the ZSubtask corresponding to the resume subtask.  If pchActionName is null ... if the action has been
// set for this ZSubtask, process the "dynamic" action, otherwise return.  I am aware that this function
// violates all rules of structured programming.  If you would like to analyze the function and re-write it
// to conform to all these rules, be my guest.  My guess is it may conform by using purist's goto's and
// other tricks ... at the expense of readability and performance.
//
// Note:  ZTask->m_vNewAppSubtask is set by the operation:
//    SetNewApplicationSubtask ... which uses the operation:
//    CreateWindowSubtask to create a subtask for a given LPLR.
zLONG OPERATION
ProcessAction( ZSubtask *pZSubtask,
               zCPCHAR  cpcActionName,
               zVIEW    vResumeSubtask,
               zLONG    lOperCtrl,
               zCPCHAR  cpcEvent )
{
   static zSHORT nRecurseCnt = 0;

   if ( pZSubtask == 0 )  // pZSubtask must be set to do the initializations below
      return( nRecurseCnt );

   zCHAR  szMsg[ 256 ];
   LPTASK hTask = (LPTASK) SysGetTaskFromView( pZSubtask->m_vDialog );
   LPTASK lpTask = (LPTASK) SysGetPointerFromHandle( hTask );
   zSHORT nOperIdx = lpTask->nOperIdx;
   static zBOOL bOperIdxErrorWritten = FALSE;
   ZAction *pAction = &(pZSubtask->m_Action);
   zPVOID  pvHoldEventParms = pZSubtask->m_pEventParms;
   zVIEW v = GetDefaultViewForActiveTask( );
   ZTask *pZTask = 0;

#ifdef DEBUG_ALL
   if ( pZSubtask == 0 || pZSubtask->m_pZTask->m_nTraceAction )
   {
      sprintf_s( szMsg, sizeof( szMsg ), "ProcessAction Name: %s Subtask: %ld Type: %ld",
                 cpcActionName, pZSubtask->m_ulIdNbr, pAction->m_lActType );
      TraceLineS( szMsg, "" );
#ifdef DEBUG_LISTWND
      ListWindowsAndControls( pZSubtask, "ProcessAction Init", 0 );
#endif

   }
#endif

   if ( cpcActionName == 0 && pAction->m_lActType == 0 )
      return( 0 );
/*
   if ( (pZSubtask->m_ulSubtaskFlags & zSUBTASK_PROCESSING_ACTIVATE) != 0 ||
        (cpcEvent != 0 && (zstrcmp( cpcEvent, "Immediate" ) == 0 || zstrcmp( cpcEvent, "cpcEvent" ) == 0 || zstrcmp( cpcEvent, "OnActivate" ) == 0)) ||
        (cpcActionName != 0 && (zstrstr( cpcActionName, "InitMenu" ) != 0 || zstrstr( cpcActionName, "ReceiveFocus" ) != 0)) )
   {
   }
   else
   {
      TraceLineS( "=================================>>>> Processing Dialog Operation ", cpcActionName ? cpcActionName : cpcEvent );
   }
*/
   pZTask = pZSubtask->m_pZTask;
   pZSubtask->m_szActionName[ 0 ] = 0;  // dks conversational fix

#if 0
   if ( pZSubtask->m_ulSubtaskFlags & zSUBTASK_MAPPING_FROM_OI )
   {
      return( 0 );

      // Just wait until mapping has completed.  Dispatch all messages.
      TraceLineS( "(drvr) Waiting for MapFromOI to complete for Action: ", cpcActionName ? cpcActionName : cpcEvent ? cpcEvent : "" );

      MSG   msg;

      while ( pZSubtask->m_ulSubtaskFlags & zSUBTASK_MAPPING_FROM_OI )
      {
         if ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
         }
      }
   }
#endif

   // Implement a stack of Actions to prevent one action from overlaying the processing of another. The
   // stack should probably never go deeper than two or three, so the definition of 16 should be way
   // more than sufficient ... we will check the depth, but will essentially punt if the limit is reached.

   zKZWDLGXO_Act_DEF ActDef;
   zULONG ulTime;
   zULONG ulSubtaskState;
   zSHORT nLth = 0;
   zLONG  lRC = 0;

   nRecurseCnt++;

   if ( pZTask == 0 || pZTask->m_nTraceAction )
      ulTime = SysGetTickCount( );

   zmemset( &ActDef, 0, sizeof( zKZWDLGXO_Act_DEF ) );
   fnPushMsgQ_State( pZSubtask->m_vMsgQ, zMSGQ_Action );

   try   // we always have to re-enable the window
   {  // begin of try block

   zCHAR  szMessage[ 256 ];
   zLONG  lErr = 0;

   do    // to implement purist's goto
   {
      zCHAR   szActionName[ zTAG_LTH ];
      zVIEW   vApp;
      zSHORT  nTemporalState = 0;

      if ( pZTask->m_nTraceAction )
      {
      // sprintf_s( szMessage, sizeof( szMessage ), "(drvr[%ld", pZSubtask->m_ulIdNbr );
         sprintf_s( szMessage, sizeof( szMessage ), "(drvr[0x%08x.%ld", (zULONG) (v ? v->hTask : 0), pZSubtask->m_ulSubtaskIdNbr );
      // strcpy_s( szMessage, sizeof( szMessage ), "(drvr[" );
      // _ltoa_s( (zLONG) pZSubtask, szMessage + 6 );

         if ( pAction && pAction->m_szActDlg[ 0 ] )
         {
            strcat_s( szMessage, sizeof( szMessage ), "." );
            strcat_s( szMessage, sizeof( szMessage ), pAction->m_szActDlg );
            strcat_s( szMessage, sizeof( szMessage ), "." );
            strcat_s( szMessage, sizeof( szMessage ), pAction->m_szActWnd );
         }
         else
         if ( pZSubtask )
         {
            strcat_s( szMessage, sizeof( szMessage ), "." );
            strcat_s( szMessage, sizeof( szMessage ), *(pZSubtask->m_pzsDlgTag) );
            strcat_s( szMessage, sizeof( szMessage ), "." );
            strcat_s( szMessage, sizeof( szMessage ), *(pZSubtask->m_pzsWndTag) );
         }

         strcat_s( szMessage, sizeof( szMessage ), "]) Action=>" );

         // Localize Action name so it can be modified.
         if ( cpcActionName )
         {
            strcpy_s( szActionName, sizeof( szActionName ), cpcActionName );
            strcat_s( szMessage, sizeof( szMessage ), szActionName );
         }
         else
         {
            szActionName[ 0 ] = 0;
            strcat_s( szMessage, sizeof( szMessage ), "(code)" );
         }
      }
      else
      {
         szMessage[ 0 ] = 0;
         nLth = 0;

         // Localize Action name so it can be modified.
         if ( cpcActionName )
            strcpy_s( szActionName, sizeof( szActionName ), cpcActionName );
         else
            szActionName[ 0 ] = 0;
      }

      // Earlier test guaranteed lActType is set.
      if ( szActionName[ 0 ] == 0 )
      {
         // These are not set by overrides.
         ActDef.DO[ 0 ] = 0;
         ActDef.OperCtrl = ActDef.SB = 0;

         ActDef.Type = pAction->m_lActType;
         ASSERT( ((ActDef.Type & ~zWAB_ProcessImmediateReturn) >= 0 && (ActDef.Type & ~zWAB_ProcessImmediateReturn) < 100) ||
                  (ActDef.Type & ~zWAB_ProcessImmediateReturn) == zWAB_TerminateActionForError || ActDef.Type == -1 );
         pAction->m_lActType = 0;
         strcpy_s( ActDef.DlgN, sizeof( ActDef.DlgN ), pAction->m_szActDlg );
         strcpy_s( ActDef.WndN, sizeof( ActDef.WndN ), pAction->m_szActWnd );
      }
      else
      if ( SetCursorFirstEntityByString( pZSubtask->m_vDialog, szlAct, szlTag, szActionName, 0 ) > zCURSOR_UNCHANGED )
      {
         // Get the structure of attributes for this action.
         ActDef.CheckByte = 0xFE;
         ActDef.lStructSize = sizeof( zKZWDLGXO_Act_DEF );
         GetStructFromEntityAttrs( (zPCHAR) &ActDef, pZSubtask->m_vDialog, szlAct );
         if ( ActDef.CheckByte != 0xFE )
            MessageBox( 0, "ActDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

         // EnabledAction flags have been relocated into the C++ object since
         // they are dynamic (i.e.  vary from .XWD instance to instance).

#ifdef DEBUG_ACTION
         TraceBlob( "ProcessAction:", pZSubtask->m_pchEnabledAction + sizeof( zSHORT ), *((zSHORT *) pZSubtask->m_pchEnabledAction) );
#endif
         zSHORT nEnabled = pZSubtask->m_pchEnabledAction[ (zSHORT) (ActDef.SyncIdx + sizeof( zSHORT )) ];
         if ( nEnabled <= 0 )
         {
            if ( pZTask->m_nTraceAction > 1 )
            {
            // strcat_s( szMessage, sizeof( szMessage ), ActDef.Tag );
               if ( nEnabled < 0 )
                  strcat_s( szMessage, sizeof( szMessage ), " (auto)" );

               strcat_s( szMessage, sizeof( szMessage ), " disabled at index: " );
               TraceLineI( szMessage, ActDef.SyncIdx );
            }

            pAction->m_lActType = 0;
            lRC = 0;
            goto CLEANUP_ACT_label;
         }
      }
      else   // the action was not located
      {
         if ( pZTask->m_nTraceAction > 1 )
            TraceLineS( szMessage, " - Action not defined (ignored)" );

         pAction->m_lActType = 0;
         lRC = -2;
         goto CLEANUP_ACT_label;
      }

      strcpy_s( pZSubtask->m_szActionName, sizeof( pZSubtask->m_szActionName ), szActionName ); // dks conversational fix
      if ( pZTask->m_nTraceAction )
      {
         nLth = (zSHORT) zstrlen( szMessage );
         strcpy_s( szMessage + nLth, sizeof( szMessage ) - nLth, " Event=>" );
         strcpy_s( szMessage + nLth + 9, sizeof( szMessage ) - nLth - 9, cpcEvent );
         if ( ActDef.DO[ 0 ] )
         {
            strcat_s( szMessage, sizeof( szMessage ), " Operation=>" );
            strcat_s( szMessage, sizeof( szMessage ), ActDef.DO );
         }
         else
         {
            if ( ActDef.Type == zWAB_SwitchToAction )
            {
               strcat_s( szMessage, sizeof( szMessage ), " (no code) WAB=>" );
               TraceLineS( szMessage, "SwitchToAction" );
            }
            else
            {
               nLth = (zSHORT) zstrlen( szMessage );
               sprintf( szMessage + nLth, "  (no code)  WAB=>%s %s.%s", g_Actions[ ActDef.Type ], ActDef.DlgN, ActDef.WndN );
               TraceLineS( szMessage, "" );
            }

            nLth = 0;   // flag that message has already been emitted
         }
      }

      if ( vResumeSubtask )
      {
         // Enable all windows that were disabled from debug processing.
#ifdef zREMOTE_SERVER
         EnableZWindowsForApplication( pZSubtask->m_pZSocketTask->m_pZSubtaskTop, TRUE );
#else
         EnableZWindowsForApplication( pZTask->m_pZSubtaskTop, TRUE );
#endif
      }
      else
      {
         if ( ActDef.OperCtrl & zACTION_DIALOG_CODE_ONLY || lOperCtrl & zACTION_DIALOG_CODE_ONLY )
         {
            // If Dialog Operation code exists for this action, execute it.
            if ( pZTask->m_nTraceAction )
            {
               strcat_s( szMessage, sizeof( szMessage ), " (code only)" );
               nLth = -1;   // flag that message is fully loaded
            }

            if ( szActionName[ 0 ] && ActDef.DO[ 0 ] )
            {
            // strcpy_s( pZSubtask->m_szActionName, sizeof( pZSubtask->m_szActionName ), szActionName ); // dks conversational fix
               lRC = fnProcessDialogOperationCode( pZSubtask, ActDef.DLL, ActDef.DO );
               ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
               if ( lRC == zNO_APPLICATION_EVENT || (ulSubtaskState & zSUBTASK_STATE_DELETED) == zSUBTASK_STATE_DELETED )
               {
                  pZSubtask = 0;    // The subtask no longer exists!!!
                  goto CLEANUP_ACT_label;
               }

            // pZSubtask->m_szActionName[ 0 ] = 0;  // dks conversational fix
               if ( zHIUSHORT( lRC ) == 2 )
               {
                  fnStartDebugger( pZSubtask, szActionName );
                  goto CLEANUP_ACT_label;
               }
            }
            else
            if ( ActDef.Type == zWAB_StartPopupMenu )
               pAction->m_lActType = zWAB_StartPopupMenu;

            if ( pAction->m_lActType == zWAB_SwitchToAction )
            {
               strcpy_s( szActionName, sizeof( szActionName ), pAction->m_szAction );
               cpcActionName = szActionName;

               if ( nLth && pZTask->m_nTraceAction )
               {
                  double dTime = ((double) (SysGetTickCount( ) - ulTime)) / zTICKS_PER_SECOND;

                  nLth = (zSHORT) zstrlen( szMessage );
                  sprintf( szMessage + nLth, " [%1.3f sec]", dTime );
                  TraceLineS( szMessage, "" );
               }

               if ( SetCursorFirstEntityByString( pZSubtask->m_vDialog, szlAct, szlTag, szActionName, 0 ) > zCURSOR_UNCHANGED )
               {
                  // Get the structure of attributes for this action.
                  ActDef.lStructSize = sizeof( zKZWDLGXO_Act_DEF );
                  GetStructFromEntityAttrs( (zPCHAR) &ActDef, pZSubtask->m_vDialog, szlAct );
                  pAction->m_lActType = ActDef.Type;
               }

               // This is against all structured programming rules and as such is clearly
               // indefensible, but here GOes anyway ...
               continue;  // go back up (in code) and restart processing this new action
            }

            ActDef.Type = pAction->m_lActType;
            ASSERT( ((ActDef.Type & ~zWAB_ProcessImmediateReturn) >= 0 && (ActDef.Type & ~zWAB_ProcessImmediateReturn) < 100) ||
                     (ActDef.Type & ~zWAB_ProcessImmediateReturn) == zWAB_TerminateActionForError || ActDef.Type == -1 );
            pAction->m_lActType = 0;
            if ( ActDef.Type == zWAB_TerminateActionForError )
            {
               lErr = zMSGQ_AfterDialogOperation;
               goto PRESENT_MESSAGE_label;
            }

            // Let popup menus take place as a "code only" option.
            if ( ActDef.Type != zWAB_StartPopupMenu )
            {
               // The only other "code only" option is to stay on the window (no refresh).
               // If refresh must occur, call it from code.
               ActDef.Type = zWAB_StayOnWindow;
            }

            break;   // goto SKIP_TO_ACTION_label;
         }

         if ( nLth && pZTask->m_nTraceAction )
            TraceLineS( szMessage, "" );

         // Map to the object instance (if Mapping has not been prevented).
         // Mapping MUST be done PRIOR to execution of dialog operation code.
         if ( szActionName[ 0 ] && ActDef.NoMap[ 0 ] == 0 )
         {
            lRC = pZSubtask->MapToOI( );

            // Map all subtasks within a splitter.
            if ( pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE )
            {
               ZDrView *pZView = pZSubtask->m_pZView->m_pNextView;
               while ( lRC >= 0 && pZView && pZView != pZSubtask->m_pZView )
               {
                  if ( pZView->m_pZSubtask )
                  {
                     lRC = pZView->m_pZSubtask->MapToOI( );
                  }

                  pZView = pZView->m_pNextView;
               }
            }

            if ( lRC < 0 )
            {
               ActDef.Type = zWAB_TerminateActionForError;
               lErr = zMSGQ_AfterMapToOI;
               goto PRESENT_MESSAGE_label;
            }
         }

         if ( szActionName[ 0 ] && ActDef.SB && ActDef.VN[ 0 ] )  // too many checks???
         {
            // We are depending on vApp being set to 0 if invalid view.
            GetViewByName( &vApp, ActDef.VN, pZSubtask->m_vDialog, zLEVEL_ANY );
         }
         else
            vApp = 0;

         // Process temporal subobjects as specified:
         //
         //    #define zTEMPORAL_SUBOBJECT_ACCEPT           0x00000001L
         //    #define zTEMPORAL_SUBOBJECT_CANCEL           0x00000002L
         //    #define zTEMPORAL_SUBOBJECT_DELETE           0x00000004L
         //    #define zTEMPORAL_SUBOBJECT_EXCLUDE          0x00000008L
         //    #define zTEMPORAL_SUBOBJECT_CREATE_ENTITY    0x00000010L
         //    #define zTEMPORAL_SUBOBJECT_CREATE_TEMPORAL  0x00000020L
         //    #define zTEMPORAL_SUBOBJECT_CREATE_VERSION   0x00000040L
         //    #define zTEMPORAL_SUBOBJECT_NEXT             0x00000080L
         //    #define zTEMPORAL_SUBOBJECT_PREV             0x00000100L
         //    #define zTEMPORAL_SUBOBJECT_STAYWHENNULL     0x00000200L
         //    #define zTEMPORAL_SUBOBJECT_RETURNWHENNULL   0x00000400L
         //
         //    #define zTEMPORAL_SUBOBJECT_NEXT             0x00000080L
         //    #define zTEMPORAL_SUBOBJECT_PREV             0x00000100L
         //    #define zTEMPORAL_SUBOBJECT_STAYWHENNULL     0x00000200L
         //    #define zTEMPORAL_SUBOBJECT_RETURNWHENNULL   0x00000400L
         //
         // Note that NEXT and PREV are not performed until after any
         // "interrupt" processing (e.g.  TerminateActionForError) could
         // occur.
         //
         if ( vApp && ActDef.SB )
         {
            if ( ActDef.SB & zTEMPORAL_SUBOBJECT_NEXT   ||
                 ActDef.SB & zTEMPORAL_SUBOBJECT_PREV   ||
                 ActDef.SB & zTEMPORAL_SUBOBJECT_DELETE ||
                 ActDef.SB & zTEMPORAL_SUBOBJECT_EXCLUDE )
            {
               nTemporalState = (MiGetTemporalStateOfEntity( vApp, ActDef.EN ) == zTES_SUBOBJECT_ROOT ) ? 1 : -1;
            }

            if ( ActDef.SB & zTEMPORAL_SUBOBJECT_VERIFY )
            {
               zCHAR szBehavior[ 16 ];
               zCHAR szText[ 255 ];
               zCHAR szTitle[ 64 ];

               if ( ActDef.Prompt[ 0 ] == 0 )
               {
                  if ( ActDef.SB & zTEMPORAL_SUBOBJECT_DELETE )
                     strcpy_s( szBehavior, sizeof( szBehavior ), "Delete " );
                  else
                  if ( ActDef.SB & zTEMPORAL_SUBOBJECT_EXCLUDE )
                     strcpy_s( szBehavior, sizeof( szBehavior ), "Exclude " );
                  else
                  if ( ActDef.SB & zTEMPORAL_SUBOBJECT_ACCEPT )
                     strcpy_s( szBehavior, sizeof( szBehavior ), "Accept " );
                  else
                  if ( ActDef.SB & zTEMPORAL_SUBOBJECT_CANCEL )
                     strcpy_s( szBehavior, sizeof( szBehavior ), "Cancel " );

                  strcpy_s( szTitle, sizeof( szTitle ), szBehavior );
                  strcat_s( szTitle, sizeof( szTitle ), "Confirmation " );

                  strcpy_s( szText, sizeof( szText ), "OK to continue with " );
                  strcat_s( szText, sizeof( szText ), szBehavior );

                  strcat_s( szText, sizeof( szText ), "of " );
                  strcat_s( szText, sizeof( szText ), ActDef.EN );
                  strcat_s( szText, sizeof( szText ), "?" );
               }
               else
               {
                  strcpy_s( szTitle, sizeof( szTitle ), "Confirmation " );
                  strcpy_s( szText, sizeof( szText ), ActDef.Prompt );
               }

               if ( MessagePrompt( vApp, 0, szTitle, szText, FALSE, zBUTTONS_YESNO,
                                   ActDef.SB == zTEMPORAL_SUBOBJECT_ACCEPT ? zRESPONSE_YES : zRESPONSE_NO, zICON_QUESTION ) != zRESPONSE_YES )
               {
                  ActDef.Type = zWAB_TerminateActionForError;
                  break;   // goto SKIP_TO_ACTION_label;
               }
            }

            if ( ActDef.SB & zTEMPORAL_SUBOBJECT_ACCEPT )
            {
               if ( AcceptSubobject( vApp, ActDef.EN ) < 0 )
               {
                  lErr = zMSGQ_AfterAcceptSubobject;
               }
            }
            else
            {
               if ( ActDef.SB & zTEMPORAL_SUBOBJECT_CANCEL )
               {
                  if ( (lRC = CancelSubobject( vApp, ActDef.EN )) < zCURSOR_UNDEFINED )
                  {
                     lErr = zMSGQ_AfterCancelSubobject;
                  }
               // else
               //    TraceLineS( "Auto CancelSubobject", "" );
               }
               else
               if ( ActDef.SB & zTEMPORAL_SUBOBJECT_DELETE )
               {
                  if ( (lRC = DeleteEntity( vApp, ActDef.EN, zREPOS_NONE )) < zCURSOR_UNDEFINED )
                  {
                     lErr = zMSGQ_AfterDeleteEntity;
                  }
                  else
                  {
                  // TraceLineS( "Auto DeleteEntity", "" );

                     // Skip this CreateTemporalSubobjectVersion because if you don't stay on the same window,
                     // you don't want the entity to be versioned.
                     if ( (ActDef.Type & ~zWAB_ProcessImmediateReturn) != zWAB_StayOnWindowWithRefresh )
                     {
                        nTemporalState = 0;
                     // TraceLineS( "Skipping CreateTemporalSubobjectVersion", "" );
                     }
                  }
               }
               else
               if ( ActDef.SB & zTEMPORAL_SUBOBJECT_EXCLUDE )
               {
                  if ( (lRC = ExcludeEntity( vApp, ActDef.EN, zREPOS_NONE )) < zCURSOR_UNDEFINED )
                  {
                     lErr = zMSGQ_AfterExcludeEntity;
                  }
               // else
               //    TraceLineS( "Auto ExcludeEntity", "" );
               }

               if ( lErr == 0 && lRC == zCURSOR_UNDEFINED )
               {
//                TraceLineS( "(drvr) Resetting undefined cursor after automatic subobject execution for entity ", ActDef[ ulAD ].EN );
                  lRC = SetCursorPrevEntity( vApp, ActDef.EN, 0 );

                  // Even if SetCursorPrevEntity fails according to the return code, it will still set cursor to a defined
                  // position so that the code below would set cursor to the second entity if permitted to execute.
               // if ( lRC < zCURSOR_SET )
               //    SetCursorNextEntity( vApp, ActDef.EN, 0 );
               }
            }

            if ( lErr )
               goto PRESENT_MESSAGE_WITH_CHECK_label;

            if ( ActDef.SB & zTEMPORAL_SUBOBJECT_CREATE_VERSION ||
                 ActDef.SB & zTEMPORAL_SUBOBJECT_CREATE_TEMPORAL ||
                 ActDef.SB & zTEMPORAL_SUBOBJECT_CREATE_ENTITY )
            {
               if ( ActDef.SB & zTEMPORAL_SUBOBJECT_CREATE_VERSION )
               {
                  if ( CreateTemporalSubobjectVersion( vApp, ActDef.EN ) < 0 )
                  {
                     lErr = zMSGQ_AfterCreateTemporalSubobject;
                  }
               }
               else
               if ( ActDef.SB & zTEMPORAL_SUBOBJECT_CREATE_TEMPORAL )
               {
                  if ( CreateTemporalEntity( vApp, ActDef.EN, zPOS_AFTER ) < 0 )
                  {
                     lErr = zMSGQ_AfterCreateTemporalEntity;
                  }
               }
               else
               if ( ActDef.SB & zTEMPORAL_SUBOBJECT_CREATE_ENTITY )
               {
                  if ( CreateEntity( vApp, ActDef.EN, zPOS_AFTER ) < 0 )
                  {
                     lErr = zMSGQ_AfterCreateEntity;
                  }
               }

               if ( lErr )
                  goto PRESENT_MESSAGE_WITH_CHECK_label;
            }
         }

         // If DialogOperation Code exists for this action, execute it.
         if ( szActionName[ 0 ] && ActDef.DO[ 0 ] )
         {
            // This next if should be removed at an appropriate point in the future ... when we are sure
            // that operation code and subobject behaviors are no longer present in legacy code. // DKS 12/9/96
         // if ( vApp && ActDef[ ulAD ].SB )
         // {
         //    ::MessageBox( 0, "Dialog operation code and Automatic\nSubobject Behavior occur simultaneously\n"
         //                     "Please correct", "Zeidon runtime error", MB_OK );
         // }

            // If the debugger gets turned on ...
            //    - disable the current application
            //    - start the debugger
         // strcpy_s( pZSubtask->m_szActionName, sizeof( pZSubtask->m_szActionName ), szActionName ); // dks conversational fix
            lRC = fnProcessDialogOperationCode( pZSubtask, ActDef.DLL, ActDef.DO );
            ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
            if ( lRC == zNO_APPLICATION_EVENT || (ulSubtaskState & zSUBTASK_STATE_DELETED) == zSUBTASK_STATE_DELETED )
            {
               pZSubtask = 0;    // The subtask no longer exists!!!
               goto CLEANUP_ACT_label;
            }

         // pZSubtask->m_szActionName[ 0 ] = 0;  // dks conversational fix
            if ( zHIUSHORT( lRC ) == 2 )
            {
               fnStartDebugger( pZSubtask, szActionName );
               goto CLEANUP_ACT_label;
            }

            if ( pAction->m_lActType == zWAB_TerminateActionForError )
            {
               ActDef.Type = zWAB_TerminateActionForError;
               pAction->m_lActType = 0;
               lErr = zMSGQ_AfterDialogOperation;
               goto PRESENT_MESSAGE_label;
            }
         }

      }  // end of:  else of:  if ( vResumeSubtask )

      //
      // Process temporal subobjects as specified.  The three remaining
      // temporal flags to be processed include:
      //
      //    Next
      //    Prev
      //    Delete (or Exclude)
      //

      // nTemporalState must be next or prev or delete or exclude to be
      // non-zero.
      if ( nTemporalState == 0 || GetViewByName( &vApp, ActDef.VN, pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         // skip ... not one of the SB's in which we are currently interested
      }
      else
      if ( SetEntityCursor( vApp, ActDef.EN, 0, zPOS_FIRST | zTEST_CSR_RESULT, 0, 0, 0, 0, ActDef.Scope, 0 ) >= 0 )
      {
         if ( ActDef.SB & zTEMPORAL_SUBOBJECT_NEXT )
            SetEntityCursor( vApp, ActDef.EN, 0, zPOS_NEXT, 0, 0, 0, 0, ActDef.Scope, 0 );
         else
         if ( ActDef.SB & zTEMPORAL_SUBOBJECT_PREV )
            SetEntityCursor( vApp, ActDef.EN, 0, zPOS_PREV, 0, 0, 0, 0, ActDef.Scope, 0 );

         // If state is temporal and there is cursor position on the
         // entity (On a delete/return there would not be position).
         if ( nTemporalState > 0 && CheckExistenceOfEntity( vApp, ActDef.EN ) >= zCURSOR_SET )
         {
            if ( CreateTemporalSubobjectVersion( vApp, ActDef.EN ) < 0 )
            {
               lErr = zMSGQ_AfterCreateTemporalSubobject;
               goto PRESENT_MESSAGE_WITH_CHECK_label;
            }
         }
      }
      else
      {
         if ( ActDef.SB & zTEMPORAL_SUBOBJECT_DELETE || ActDef.SB & zTEMPORAL_SUBOBJECT_EXCLUDE )
         {
            if ( ActDef.SB & zTEMPORAL_SUBOBJECT_STAYWHENNULL )
            {
               ActDef.Type = zWAB_StayOnWindowWithRefresh;
            }
            else
            if ( ActDef.SB & zTEMPORAL_SUBOBJECT_RETURNWHENNULL )
            {
               ActDef.Type = zWAB_ReturnToParentWithRefresh;
            }
         }
      }

      // Determine if the dialog operation code specified the processing to
      // be performed.
      if ( pAction->m_lActType )
      {
         ActDef.Type = pAction->m_lActType;
         ASSERT( ((ActDef.Type & ~zWAB_ProcessImmediateReturn) >= 0 && (ActDef.Type & ~zWAB_ProcessImmediateReturn) < 100) ||
                 (ActDef.Type & ~zWAB_ProcessImmediateReturn) == zWAB_TerminateActionForError || ActDef.Type == -1 );
         pAction->m_lActType = 0;
         strcpy_s( ActDef.DlgN, sizeof( ActDef.DlgN ), pAction->m_szActDlg );
         strcpy_s( ActDef.WndN, sizeof( ActDef.WndN ), pAction->m_szActWnd );

         if ( ActDef.Type == zWAB_SwitchToAction )
         {
            strcpy_s( szActionName, sizeof( szActionName ), pAction->m_szAction );
            cpcActionName = szActionName;

            // This is against all structured programming rules and as such is clearly indefensible, but here goes anyway ...
            continue;  // go back up (in code) and restart processing this new action
         }
      }

      break; // always exit do while except when continuing

   }  while ( TRUE );  // end of:  do while (to implement purist's goto)

   goto SKIP_TO_ACTION_label;

CLEANUP_ACT_label:

   // We just want to skip the action switch below.  There appears to be a bug in MSVC that thinks CLEANUP_ACT_label is in the exception
   // handing code if it is placed after the switch.  So the label is placed here and we do nothing in the switch statement.
   ActDef.Type = zWAB_TerminateActionForError;  // this causes no-op
   nLth = 0;
   goto SKIP_TO_ACTION_label;

PRESENT_MESSAGE_WITH_CHECK_label:

   // Exclude the case where the subtask state is DELETE in this if.
   ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
   if ( (ulSubtaskState & 0x000000FF) >= zSUBTASK_STATE_POSTBUILD_PRECODE )
      ActDef.Type = zWAB_TerminateActionForError;
   else
      ActDef.Type = zWAB_ReturnToParent;

   lRC = ActDef.Type;
// MQ_MessagePresent( pZSubtask->m_vMsgQ, pZSubtask->m_vDialog, lErr );
   MessagePresent( pZSubtask->m_vDialog, lErr, 0 );
   goto SKIP_TO_ACTION_label;

PRESENT_MESSAGE_label:

   // Note that ActDef.Type must have been set appropriately.

// MQ_MessagePresent( pZSubtask->m_vMsgQ, pZSubtask->m_vDialog, lErr );
   MessagePresent( pZSubtask->m_vDialog, lErr, 0 );
   lRC = zWAB_TerminateActionForError;

SKIP_TO_ACTION_label:

   if ( nLth && (pZTask == 0 || pZTask->m_nTraceAction) )
   {
      if ( nLth > 0 )
      {
         if ( ActDef.DO[ 0 ] )
         {
            strcpy_s( szMessage + nLth, sizeof( szMessage ) - nLth, " Operation=>" );
            nLth += 13;
            strcpy_s( szMessage + nLth, sizeof( szMessage ) - nLth, ActDef.DO );
            nLth += (zSHORT) zstrlen( szMessage + nLth );
         }

         strcpy_s( szMessage + nLth, sizeof( szMessage ) - nLth, " WAB=>" );
         nLth += 7;
         if ( ActDef.Type < 100 )
            strcpy_s( szMessage + nLth, sizeof( szMessage ) - nLth, g_Actions[ ActDef.Type ] );
         else
            _ltoa_s( ActDef.Type, szMessage + nLth, sizeof( szMessage ) - nLth, 10 );
      }

      double dTime = ((double) (SysGetTickCount( ) - ulTime)) / zTICKS_PER_SECOND;

      nLth = (zSHORT) zstrlen( szMessage );
      sprintf_s( szMessage + nLth, sizeof( szMessage ) - nLth, " [%1.3f sec] %s.%s", dTime, ActDef.DlgN, ActDef.WndN );
      TraceLineS( szMessage, "" );
   }

   if ( pZSubtask )
   {
      pZSubtask->m_pEventParms = 0;
      fnPopMsgQ_State( pZSubtask->m_vMsgQ );
   }

   // Let's do the action.
   switch ( ActDef.Type & ~zWAB_ProcessImmediateReturn )
   {
      zVIEW  vNewSubtask;

      case zWAB_StayOnWindow:
      case zWAB_TerminateActionForError:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StayOnWindow ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         break;
      }

      case zWAB_StayOnWindowWithRefresh:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StayOnWindowWithRefresh ", ActDef.DlgN );
         TraceLineS( "for Window ", pZSubtask->m_ActDef.WndN );
//       TraceLineI( "nMultiRefresh = ", nMultiRefresh );
#endif
         // Refresh this and all other subtasks within a splitter.
         pZSubtask->RefreshSplitters( );

         break;
      }

      case zWAB_StartModalSubwindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartModalSubwindow ", ActDef.WndN );
         TraceLineS( "for Dialog ", ActDef.DlgN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
            vNewSubtask = 0;

         pZTask->NewZWnd( pZSubtask,            // Zeidon parent
                          pZSubtask,            // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,          // new subtask
                          TRUE,                 // modal
                          ActDef.DlgN,          // Dialog to process
                          ActDef.WndN,          // Window to create
#ifdef zREMOTE_SERVER
                          0,
                          pZSubtask->m_pZSocketTask );
#else
                          0 );
#endif
         break;
      }

      case zWAB_StartOrFocusModelessSubwindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartOrFocusModelessSubwindow ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         zVIEW vSubtaskReturn;

         if ( fnGetSubtask( pZSubtask, &vSubtaskReturn, ActDef.DlgN, ActDef.WndN, TRUE ) )
         {
            pZTask->m_vNewAppSubtask = 0;
            ActivateWindow( vSubtaskReturn );
            break;
         }
      }     // otw intentionally fall through

      case zWAB_StartModelessSubwindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartModelessSubwindow ", ActDef.WndN );
         TraceLineS( "for Dialog ", ActDef.DlgN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
            vNewSubtask = 0;

         pZTask->NewZWnd( pZSubtask,            // Zeidon parent
                          pZSubtask,            // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,          // new subtask
                          FALSE,                // modeless
                          ActDef.DlgN,          // Dialog to process
                          ActDef.WndN,          // Window to create
#ifdef zREMOTE_SERVER
                          0,
                          pZSubtask->m_pZSocketTask );
#else
                          0 );
#endif
         break;
      }

      case zWAB_StartModalWindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartModalWindow ", ActDef.WndN );
         TraceLineS( "for Dialog ", ActDef.DlgN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
            vNewSubtask = 0;

         pZTask->NewZWnd( pZSubtask->m_pZParent, // Zeidon parent
                          pZSubtask,             // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,           // new subtask
                          TRUE,                  // modal
                          ActDef.DlgN,           // Dialog to process
                          ActDef.WndN,           // Window to create
#ifdef zREMOTE_SERVER
                          0,
                          pZSubtask->m_pZSocketTask );
#else
                          0 );
#endif
         break;
      }

      case zWAB_StartOrFocusModelessWindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartOrFocusModelessWindow ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         zVIEW vSubtaskReturn;

         if ( fnGetSubtask( pZSubtask, &vSubtaskReturn, ActDef.DlgN, ActDef.WndN, TRUE ) )
         {
            pZTask->m_vNewAppSubtask = 0;
            ActivateWindow( vSubtaskReturn );
            break;
         }
      }     // otw intentionally fall through

      case zWAB_StartModelessWindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartModelessWindow ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
            vNewSubtask = 0;

         pZTask->NewZWnd( pZSubtask->m_pZParent, // Zeidon parent
                          pZSubtask,             // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,           // new subtask
                          FALSE,                 // modeless
                          ActDef.DlgN,           // Dialog to process
                          ActDef.WndN,           // Window to create
#ifdef zREMOTE_SERVER
                          0,
                          pZSubtask->m_pZSocketTask );
#else
                          0 );
#endif
         break;
      }

      case zWAB_ReplaceWindowWithModelessWindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "ReplaceWindowWithModelessWindow ", ActDef.WndN );
         TraceLineS( "for Dialog ", pZSubtask->m_ActDef.DlgN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
         {
            vNewSubtask = 0;
         }

         // Build a new window and then delete this window from task list.
         pZTask->NewZWnd( pZSubtask->m_pZParent, // Zeidon parent
                          0, // pZSubtask, no owner here!  // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,           // new subtask
                          FALSE,                 // modeless
                          ActDef.DlgN,           // Dialog to process
                          ActDef.WndN,           // Window to create
#ifdef zREMOTE_SERVER
                          pZSubtask,
                          pZSubtask->m_pZSocketTask );
#else
                          pZSubtask );
#endif

         pZSubtask = 0;  // pZSubtask should have been destroyed ...
         break;          // we cannot manipulate its data anymore!
      }

      case zWAB_ReplaceWindowWithModalWindow:
      case zWAB_StartTopWindow:
      case zWAB_ResetTopWindow:
      {
#ifdef DEBUG_WAB
         TraceLineS( "ReplaceWindowWithModalWindow ", ActDef.WndN );
         TraceLineS( "for Dialog ", ActDef.DlgN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
         {
            vNewSubtask = 0;
         }

         // Build a new window and then delete this window from task list.
         pZTask->NewZWnd( pZSubtask->m_pZParent, // Zeidon parent
                          0, // pZSubtask, no owner here!  // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,           // new subtask
                          TRUE,                  // modal
                          ActDef.DlgN,           // Dialog to process
                          ActDef.WndN,           // Window to create
#ifdef zREMOTE_SERVER
                          pZSubtask,
                          pZSubtask->m_pZSocketTask );
#else
                          pZSubtask );
#endif

         pZSubtask = 0;  // pZSubtask should have been destroyed ...
         break;          // we cannot manipulate its data anymore!
      }

      case zWAB_StartOrFocusModelessDialog:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartOrFocusModelessDialog ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         zVIEW vSubtaskReturn;

         if ( fnGetSubtask( pZSubtask, &vSubtaskReturn, ActDef.DlgN, ActDef.WndN, TRUE ) )
         {
            pZTask->m_vNewAppSubtask = 0;
            ActivateWindow( vSubtaskReturn );
            break;
         }
      }     // otw intentionally fall through

      case zWAB_StartDialogTask:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartDialogtask ", ActDef.WndN );
         TraceLineS( "for Dialog ", ActDef.DlgN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
            vNewSubtask = 0;

         pZTask->NewZWnd( 0,                    // Zeidon parent
                          pZSubtask,            // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,          // new subtask
                          FALSE,                // modeless
                          ActDef.DlgN,          // Dialog to process
#ifdef zREMOTE_SERVER
                          ActDef.WndN,          // Window to create
                          0,
                          pZSubtask->m_pZSocketTask );
#else
                          ActDef.WndN );        // Window to create
#endif
         break;
      }

      case zWAB_StartOrFocusModelessApp:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartOrFocusModelessApp ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         zVIEW vSubtaskReturn;

         if ( fnGetSubtask( pZSubtask, &vSubtaskReturn, ActDef.DlgN, ActDef.WndN, FALSE ) )
         {
            ActivateWindow( vSubtaskReturn );
            break;
         }

         // Protect this with a semaphore!!!
         SysMutexLock( pZSubtask->m_vDialog, "ZDr", 0, 0 );
      // TraceLineS( "ProcessAction ModelessApp Lock Mutex ZDr", "" );

         zLONG lPos = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
         while ( lPos > 1 )
         {
            if ( g_pSubtaskList[ lPos ].pZSubtask && (g_pSubtaskList[ lPos ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 &&
                 zstricmp( g_pSubtaskList[ lPos ].DlgN, ActDef.DlgN ) == 0 && (ActDef.WndN[ 0 ] == 0 || zstricmp( g_pSubtaskList[ lPos ].WndN, ActDef.WndN ) == 0) )
            {
               vSubtaskReturn = (zVIEW) SysGetPointerFromHandle( g_pSubtaskList[ lPos ].pvDialog );

               if ( vSubtaskReturn && MiValidView( vSubtaskReturn ) && mIs_hWnd( g_pSubtaskList[ lPos ].hWnd ) && mIs_hWndEnabled( g_pSubtaskList[ lPos ].hWnd ) )
               {
                  if ( ::IsIconic( g_pSubtaskList[ lPos ].hWnd ) )
                     ::ShowWindow( g_pSubtaskList[ lPos ].hWnd, SW_SHOWNORMAL );
                  else
                  {
                     ::ShowWindow( g_pSubtaskList[ lPos ].hWnd, SW_SHOWMINIMIZED );
                     ::ShowWindow( g_pSubtaskList[ lPos ].hWnd, SW_RESTORE );
                  }

                  ::SetWindowPos( g_pSubtaskList[ lPos ].hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );
                  ::FlashWindow( g_pSubtaskList[ lPos ].hWnd, FALSE );
               // ::SetFocus( g_pSubtaskList[ lPos ].hWnd );

                  lPos = -1;  // break (out of for) and set flag to cause break (out of case)
               }
            }

            lPos--;
         }

      // TraceLineS( "ProcessAction ModelessApp Unlock Mutex ZDr", "" );
         SysMutexUnlock( pZSubtask->m_vDialog, "ZDr", 0 );

         if ( lPos < 0 )
            break;

      }     // otw intentionally fall through

      case zWAB_StartNewApp:
      {
#ifdef DEBUG_WAB
         TraceLineS( "StartNewApp ", ActDef.WndN );
         TraceLineS( "for Dialog ", ActDef.DlgN );
#endif
         zVIEW vSubtask;

         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
            vSubtask = vNewSubtask;
         }
         else
         {
            vNewSubtask = 0;
            vSubtask = pZSubtask->m_vDialog;
         }

         LPAPP pApp;
         if ( SfGetApplicationForSubtask( &pApp, vSubtask ) == 0 && pApp )
         {
            zCHAR szCmdLine[ 128 ];
            ZSubtask *pZSubtaskParent;

            strcpy_s( szCmdLine, sizeof( szCmdLine ), "zdr.exe ~~" );
            strcat_s( szCmdLine, sizeof( szCmdLine ), pApp->szName );
            strcat_s( szCmdLine, sizeof( szCmdLine ), "~" );
            strcat_s( szCmdLine, sizeof( szCmdLine ), ActDef.DlgN );
            StartTask( pZSubtask->m_vDialog, szCmdLine, SW_SHOW );

            // Try to make this subtask the top subtask of the newly
            // created subtask.
            if ( (ActDef.Type & ~zWAB_ProcessImmediateReturn) == zWAB_StartOrFocusModelessApp )
            {
               for ( zSHORT k = 0; k < 4; k++ )
               {
                  if ( (pZSubtaskParent = fnGetSubtask( pZSubtask, &vSubtask, ActDef.DlgN, ActDef.WndN, TRUE )) != 0 )
                  {
#ifdef zREMOTE_SERVER
                     pZSubtaskParent->m_pZSocketTask->m_pZSubtaskTop = pZSubtask;
#else
                     pZSubtaskParent->m_pZTask->m_pZSubtaskTop = pZSubtask;
#endif
                     break;
                  }

                  MSG  msg;
                  if ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
                  {
                     ::TranslateMessage( &msg );
                     ::DispatchMessage( &msg );
                  }
               }
            }
         }

         break;
      }

      case zWAB_ReplaceDialogWithDialog:
      {
#ifdef DEBUG_WAB
         TraceLineS( "ReplaceDialogWithDialog ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
            vNewSubtask = 0;

         // Build a new window and then delete this dialog from task list.
         pZTask->NewZWnd( 0,                   // Zeidon parent
                          pZSubtask,           // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,         // new subtask
                          FALSE,               // modeless
                          ActDef.DlgN,         // Dialog to process
                          ActDef.WndN,         // Window to create
#ifdef zREMOTE_SERVER
                          pZSubtask->m_pZSocketTask->m_pZSubtaskTop,
                          pZSubtask->m_pZSocketTask );
#else
                          pZTask->m_pZSubtaskTop );
#endif
         pZSubtask = 0;  // pZSubtask should have been destroyed ...
         break;          // we cannot manipulate its data anymore!
      }

      case zWAB_ReplaceCurrentAppWithApp:
      {
#ifdef DEBUG_WAB
         TraceLineS( "ReplaceCurrentAppWithApp ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
#endif
         if ( pZTask->m_vNewAppSubtask )
         {
            vNewSubtask = pZTask->m_vNewAppSubtask;
            pZTask->m_vNewAppSubtask = 0;
         }
         else
            vNewSubtask = 0;

         // Build a new window and then delete this window from task list.
         pZTask->NewZWnd( 0,                   // Zeidon parent
                          pZSubtask,           // Zeidon owner
                          pZSubtask->m_vDialog,
                          vNewSubtask,         // new subtask
                          FALSE,               // modeless
                          ActDef.DlgN,         // Dialog to process
                          ActDef.WndN,         // Window to create
#ifdef zREMOTE_SERVER
                          pZSubtask,
                          pZSubtask->m_pZSocketTask );
#else
                          pZSubtask );
#endif

         pZSubtask = 0;  // pZSubtask should have been destroyed ...
         break;          // we cannot manipulate its data anymore!
      }

      case zWAB_StartPopupMenu:
      {
         DWORD dwPoint;
         if ( pvHoldEventParms == 0 )
            dwPoint = GetMessagePos( );
         else
            dwPoint = (DWORD) pvHoldEventParms;

         if ( ActDef.Popup[ 0 ] )
         {
            mDeleteInit( pZSubtask->m_pzsPopupTag );
            pZSubtask->m_pzsPopupTag = new CString( ActDef.Popup );
            pZSubtask->m_pZFWnd->PostMessage( zZEIDON_POSTED_POPUP_MENU, 0, dwPoint );
         }

         pZSubtask->m_pEventParms = pvHoldEventParms;  // reset for choice event
         break;
      }

      case zWAB_ReturnToParent:
      case zWAB_ReturnToParentWithRefresh:
      case zWAB_ReturnToTopWindow:
      case zWAB_ReturnToTopWindowWithRefresh:
      case zWAB_ExitDialogTask:
#ifdef DEBUG_WAB
         TraceLineS( "ReturnToParent ", ActDef.DlgN );
         TraceLineS( "for Window ", ActDef.WndN );
         TraceLineS( "zWAB =  ", g_Actions[ ActDef.Type ] );
#endif
         fnProcessReturn( pZSubtask, ActDef );
         pZSubtask = 0;  // pZSubtask should have been destroyed ...
         break;          // we cannot manipulate its data anymore!

      default:
         TraceLineI( "(drvr) Invalid WAB ====> ", ActDef.Type );
         OperatorSend( 0, szlDrvr, "Invalid window action behavior", TRUE );
   }

   }  // end of try block

   catch( CException *e )
   {
      e->GetErrorMessage( szMsg, sizeof( szMsg ) );
      TraceLineS( "(drvr) Action Exception ====> ", szMsg );
   // ::MessageBox( 0, szMsg, "Action New Exception", MB_OK );
      ActDef.Type = zWAB_ExitDialogTask | zWAB_ProcessImmediateReturn;
      fnProcessReturn( pZSubtask, ActDef );
      nRecurseCnt--;
      return( 0 );
   }

   nRecurseCnt--;
   if ( nRecurseCnt == 0 &&
        pZTask && pZTask->m_nTraceAction )
   {
      if ( lpTask->nOperIdx != nOperIdx )
      {
         TraceLine( "DGC: ProcessAction termination error: OperIdx Original: %d   Current: %d", nOperIdx, lpTask->nOperIdx );
         if ( bOperIdxErrorWritten == FALSE )
         {
            SysMessageBox( v, "DGC", "OperIdx error - see trace", 0 );
            bOperIdxErrorWritten = TRUE;
         }
      }
   }


   SysSendOE_Message( IDM_RESETTIMEOUT, 0 );

#ifdef DEBUG_LISTWND
   TraceLineS( "End of ProcessAction: ", cpcActionName );
   ListWindowsAndControls( pZSubtask, "ProcessAction Term", 0 );
#endif
   return( lRC );

} // end of:  ProcessAction

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: fnProcessReturn
//
// PURPOSE: This routine processes the return request
//
/////////////////////////////////////////////////////////////////////////////
void
fnProcessReturn( ZSubtask *pZSubtask,
                 zKZWDLGXO_Act_DEF ActDef )
{
   ZSubtask *pZSubtaskParent;
   zLONG    lBehavior = ActDef.Type & ~zWAB_ProcessImmediateReturn;
   zULONG   ulSubtaskState;
   zSHORT   nReturn = 1;
#ifdef zREMOTE_SERVER
#ifdef DEBUG_ALL
   zCHAR    szMsg[ 128 ];
   TraceLineX( "Processing return =========================== for Subtask: ",
               (zLONG) pZSubtask );
#endif
#endif

   if ( pZSubtask == 0 ||
        ((ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask )) & zSUBTASK_STATE_DELETED) == zSUBTASK_STATE_DELETED )
   {
      return;
   }

   ZTask *pZTask = pZSubtask->m_pZTask;
   zBOOL bConversational = (pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_CONVERSATIONAL) ? TRUE : FALSE;
   if ( lBehavior == zWAB_ExitDialogTask ) // terminate application
   {

#ifdef zREMOTE_SERVER

      pZSubtask->m_pZSocketTask->m_lSocketState = -1;

#ifdef DEBUG_ALL
      sprintf_s( szMsg, sizeof( szMsg ),
                "[S%ld.T%ld.t%ld.s%ld] fnProcessReturn (ExitDialog) pZSubtask: ",
                pZSubtask->m_pZSocketTask,
                AfxGetThread( ),
                pZSubtask->m_pZSocketTask->m_pWinThread,
                pZSubtask->m_pZSocketTask->m_sock );
      TraceLineI( szMsg, (zLONG) pZSubtask );
#endif

#else
      if ( (ulSubtaskState & 0x000000FF) < zSUBTASK_STATE_PREBUILD_POSTCODE )
         pZSubtask->m_pZTask->m_uAppState |= zAPP_STATE_TERMINATED;
#endif

      // Delete all windows from subtask list.
      while ( pZSubtask->m_pZParent && ((ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask->m_pZParent )) & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED )
      {
         pZSubtask = pZSubtask->m_pZParent;
      }

      mDeleteInit( pZTask->m_pzsLastDlgTag );
      mDeleteInit( pZTask->m_pzsLastWndTag );
      pZTask->m_pzsLastDlgTag = new CString( *(pZSubtask->m_pzsDlgTag) );
      pZTask->m_pzsLastWndTag = new CString( *(pZSubtask->m_pzsWndTag) );

      if ( ActDef.Type & zWAB_ProcessImmediateReturn )
         pZSubtask->m_pZTask->DeleteSubtask( pZSubtask );
      else
         pZSubtask->PostClose( 1 );

      pZSubtask = 0;
   }
   else
   if ( lBehavior == zWAB_ReturnToTopWindow ||
        lBehavior == zWAB_ReturnToTopWindowWithRefresh )
   {
      // Delete all windows except the top-level window.
#ifdef zREMOTE_SERVER
      zBOOL bFoundSubtask = FALSE;

      while ( pZSubtask != pZSubtask->m_pZSocketTask->m_pZSubtaskTop )
      {
         bFoundSubtask = TRUE;

         if ( pZSubtask->m_pZParent )
            pZSubtaskParent = pZSubtask->m_pZParent;
         else
            pZSubtaskParent = pZSubtask->m_pZSocketTask->m_pZSubtaskTop;

         mDeleteInit( pZTask->m_pzsLastDlgTag );
         mDeleteInit( pZTask->m_pzsLastWndTag );
         pZTask->m_pzsLastDlgTag = new CString( *(pZSubtask->m_pzsDlgTag) );
         pZTask->m_pzsLastWndTag = new CString( *(pZSubtask->m_pzsWndTag) );

         if ( ActDef.Type & zWAB_ProcessImmediateReturn )
            pZSubtask->m_pZTask->DeleteSubtask( pZSubtask );
         else
            pZSubtask->PostClose( 1 );

         pZSubtask = pZSubtaskParent;
      }

      if ( bFoundSubtask == FALSE )
      {
         mDeleteInit( pZTask->m_pzsLastDlgTag );
         mDeleteInit( pZTask->m_pzsLastWndTag );
         pZTask->m_pzsLastDlgTag = new CString( *(pZSubtask->m_pzsDlgTag) );
         pZTask->m_pzsLastWndTag = new CString( *(pZSubtask->m_pzsWndTag) );

         if ( ActDef.Type & zWAB_ProcessImmediateReturn )
            pZSubtask->m_pZTask->DeleteSubtask( pZSubtask );
         else
            pZSubtask->PostClose( 1 );
      }

#ifdef DEBUG_ALL
      sprintf_s( szMsg, sizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] fnProcessReturn (to Top) pZSubtask: ",
                pZSubtask->m_pZSocketTask,
                AfxGetThread( ),
                pZSubtask->m_pZSocketTask->m_pWinThread,
                pZSubtask->m_pZSocketTask->m_sock );
      TraceLineI( szMsg, (zLONG) pZSubtask );
#endif
#else
      while ( pZSubtask != pZSubtask->m_pZTask->m_pZSubtaskTop )
      {
         if ( pZSubtask->m_pZParent )
            pZSubtaskParent = pZSubtask->m_pZParent;
         else
            pZSubtaskParent = pZSubtask->m_pZTask->m_pZSubtaskTop;

         mDeleteInit( pZTask->m_pzsLastDlgTag );
         mDeleteInit( pZTask->m_pzsLastWndTag );
         pZTask->m_pzsLastDlgTag = new CString( *(pZSubtask->m_pzsDlgTag) );
         pZTask->m_pzsLastWndTag = new CString( *(pZSubtask->m_pzsWndTag) );

         if ( ActDef.Type & zWAB_ProcessImmediateReturn )
            pZSubtask->m_pZTask->DeleteSubtask( pZSubtask );
         else
            pZSubtask->PostClose( 1 );

         pZSubtask = pZSubtaskParent;
      }
#endif
   }
   else
   {
      if ( ActDef.DlgN[ 0 ] && ActDef.WndN[ 0 ] && zstricmp( *(pZSubtask->m_pzsDlgTag), ActDef.DlgN ) == 0 && zstricmp( *(pZSubtask->m_pzsWndTag), ActDef.WndN ) == 0 )
      {
         ActDef.DlgN[ 0 ] = 0;
      }

      // Delete this window from subtask list.  If the Dialog Name and the Window Name are specified for return to parent, keep deleting
      // subtasks until reaching the specified parent.
      while ( pZSubtask )
      {
         if ( ActDef.DlgN[ 0 ] && ActDef.WndN[ 0 ] && zstricmp( *(pZSubtask->m_pzsDlgTag), ActDef.DlgN ) == 0 && zstricmp( *(pZSubtask->m_pzsWndTag), ActDef.WndN ) == 0 )
         {
            break;
         }

#ifdef zREMOTE_SERVER
#ifdef DEBUG_ALL
         sprintf_s( szMsg, sizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] fnProcessReturn pZSubtask: ",
                   pZSubtask->m_pZSocketTask,
                   AfxGetThread( ),
                   pZSubtask->m_pZSocketTask->m_pWinThread,
                   pZSubtask->m_pZSocketTask->m_sock );
         TraceLineI( szMsg, (zLONG) pZSubtask );
#endif
#endif

         pZSubtaskParent = pZSubtask->m_pZParent;
         if ( pZSubtaskParent == 0 )
            pZSubtaskParent = pZSubtask->m_pZOwner;

         mDeleteInit( pZTask->m_pzsLastDlgTag );
         mDeleteInit( pZTask->m_pzsLastWndTag );
         pZTask->m_pzsLastDlgTag = new CString( *(pZSubtask->m_pzsDlgTag) );
         pZTask->m_pzsLastWndTag = new CString( *(pZSubtask->m_pzsWndTag) );

         if ( ActDef.Type & zWAB_ProcessImmediateReturn )
         {
//          TraceLineS( "ProcessAction ImmediateReturn: ", *(pZSubtask->m_pzsWndTag) );
            pZSubtask->m_pZTask->DeleteSubtask( pZSubtask );
            pZSubtask = 0;
         }
         else
         {
//          TraceLineS( "ProcessAction PostClose: ", *(pZSubtask->m_pzsWndTag) );
            pZSubtask->PostClose( 1 );
            pZSubtask = 0;
         }

         pZSubtask = pZSubtaskParent;
         if ( ActDef.DlgN[ 0 ] == 0 || ActDef.WndN[ 0 ] == 0 )
         {
            break;
         }

         nReturn++;
      }
   }

   if ( pZSubtask &&
        ((ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask )) & zSUBTASK_STATE_DELETED) == zSUBTASK_STATE_DELETED )
   {
      pZSubtask = 0;
   }

   if ( pZSubtask )
   {
   // TraceLineS( "ProcessAction Returning to: ", *(pZSubtask->m_pzsWndTag) );
#ifdef zREMOTE_SERVER
      SetRemoteWndAttribute( pZSubtask->m_vDialog, "Active", "Y" );
      if ( pZSubtask->m_pZSocketTask )
         strcpy_s( pZSubtask->m_pZSocketTask->m_szRemoteWndTag, sizeof( pZSubtask->m_pZSocketTask->m_szRemoteWndTag ), *(pZSubtask->m_pzsRemoteWndTag) );
#endif
      if ( (pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) )
      {
         pZSubtaskParent = pZSubtask->GetSplitterMainSubtask( );
         if ( pZSubtaskParent )
            pZSubtask = pZSubtaskParent;
      }

      // Even if pZSubtask is 0, it is possible that we should still set the active window ... but this is very difficult to determine!!!
   // TraceLine( "ProcessAction Returning to: %s.%s   IdNbr: %d   Return Count: %d",
   //            *(pZSubtask->m_pzsDlgTag), *(pZSubtask->m_pzsWndTag), pZSubtask->m_ulSubtaskIdNbr, nReturn );
      while ( nReturn > 0 )
      {
         nReturn--;
         pZSubtask->EnableWindow( TRUE );
      }

      if ( pZSubtask->m_pZFWnd && pZSubtask->m_pSplitterView == 0 && mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
      {
         pZSubtask->m_pZFWnd->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,  SWP_NOSIZE | SWP_NOMOVE |
         /* must redraw topmost window...SWP_NOREDRAW | */ SWP_SHOWWINDOW );
      // pZSubtask->m_pZFWnd->SetForegroundWindow( );
      }

      if ( pZSubtask->m_pzmaFirstFocus )
      {
         pZSubtask->m_pzmaWithFocus = pZSubtask->m_pzmaFirstFocus;
         if ( mIs_hWnd( pZSubtask->m_pzmaFirstFocus->m_pCtrl->m_hWnd ) )
            ::SetFocus( pZSubtask->m_pzmaFirstFocus->m_pCtrl->m_hWnd );
      }
      else
      if ( pZSubtask->m_pZView )
      {
         if ( pZSubtask->m_pZFWnd && mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         {
            pZSubtask->m_pZFWnd->SetActiveView( pZSubtask->m_pZView, FALSE );
         }

         ::SetActiveWindow( *(pZSubtask->m_pZView) );
      }

      if ( bConversational == FALSE && (pZSubtask->m_ulWndOper & zWND_EVENT_ON_RETURN) &&
           SetCursorFirstEntityByInteger( pZSubtask->m_vDialog, szlWndEvent, szlType, zWND_EVENT_ON_RETURN, 0 ) > zCURSOR_UNCHANGED )
      {
//       MSG    msg;
//       if ( PeekMessage( &msg, 0, g_uMsgPostedClose, g_uMsgPostedClose, PM_NOREMOVE | PM_NOYIELD ) )
//       {
//          while ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
//          {
//             TranslateMessage( &msg );
//             DispatchMessage( &msg );
//          }
//       }
//
         ZSubtask::SetSubtaskState( pZSubtask, 0, zSUBTASK_STATE_ON_RETURN );
         if ( ProcessActionLoop( pZSubtask, szlWndAct, 0, 0, 0, TRUE, "OnReturn" ) > 0 )
         {
            TraceLineS( "ProcessAction OnReturn disabling Refresh", "" );
            lBehavior = 0;
         }
      }


      if ( lBehavior == zWAB_ReturnToParentWithRefresh || lBehavior == zWAB_ReturnToTopWindowWithRefresh )
      {
         ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
         if ( (ulSubtaskState & zSUBTASK_STATE_DELETED) == 0 )
            pZSubtask->RefreshSplitters( );
      }
   }
}

ZSubtask *
fnGetSubtask( ZSubtask *pZSubtask,
              zVIEW    *pvSubtaskReturn,
              zCPCHAR  cpcDlgTag,
              zCPCHAR  cpcWndTag,
              zBOOL    bMatchTask )
{
   // Run through the list of windows to find one matching the
   // specified tags.  If bMatchTask is false, run through two
   // times ... the first time trying to locate a match including
   // a match on Task, the second time ignoring Task.
   zBOOL bMatch = TRUE;
   zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   do
   {
      for ( zLONG lPos = 1; lPos <= lItemCnt; lPos++ )
      {
         if ( g_pSubtaskList[ lPos ].pZSubtask && (bMatch == FALSE || (bMatch && g_pSubtaskList[ lPos ].lTID == pZSubtask->m_lTID)) &&
              (g_pSubtaskList[ lPos ].ulSubtaskState & zSUBTASK_STATE_WELL_FORMED) == zSUBTASK_STATE_WELL_FORMED &&
              zstricmp( g_pSubtaskList[ lPos ].DlgN, cpcDlgTag ) == 0 )
         {
            if ( (*cpcWndTag && zstricmp( g_pSubtaskList[ lPos ].WndN, cpcWndTag ) == 0) || *cpcWndTag == 0 )
            {
            // TraceLineS( "Subtask FOUND Dialog ", *(pZSubtaskCurr->m_pzsDlgTag) );
            // TraceLineS( " !!!!!!!!!!!! Window ", *(pZSubtaskCurr->m_pzsWndTag) );
               *pvSubtaskReturn = (zVIEW) SysGetPointerFromHandle( g_pSubtaskList[ lPos ].pvDialog );
               return( g_pSubtaskList[ lPos ].pZSubtask );
            }
         }
      }

      if ( bMatchTask == bMatch )
         break;

      bMatch = FALSE;

   } while ( TRUE );

   *pvSubtaskReturn = 0;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: fnStartDebugger
//
// PURPOSE: This routine starts the debugger (an error occurred
//          while interpreting code)
//
/////////////////////////////////////////////////////////////////////////////
void
fnStartDebugger( ZSubtask *pZSubtask,
                 zCPCHAR  pchActionName )
{
   zVIEW   vNewSubtask;

   // Disable all windows for the application being debugged.
#ifdef zREMOTE_SERVER
   EnableZWindowsForApplication( pZSubtask->m_pZSocketTask->m_pZSubtaskTop, FALSE );
#else
   EnableZWindowsForApplication( pZSubtask->m_pZTask->m_pZSubtaskTop, FALSE );
#endif
   mDeleteInit( pZSubtask->m_pzsResumeAction );
   pZSubtask->m_pzsResumeAction = new CString( pchActionName );

   // Start the Debugger ... and then come back at resume.
   SfCreateSubtask( &vNewSubtask, pZSubtask->m_pZTask->m_vInterpSubtask, 0 );
   ZSubtask *pZSubtaskNew =
      pZSubtask->m_pZTask->NewZWnd( 0,             // Zeidon parent
                                    pZSubtask,     // Zeidon owner
                                    pZSubtask->m_pZTask->m_vInterpSubtask,
                                    vNewSubtask,
                                    FALSE,         // modeless
                                    "TZVSDBAD",    // Dialog to process
                                    "TZVSDBAD" );  // Window to create
   ZSubtask::SetSubtaskState( pZSubtaskNew, 0, zSUBTASK_STATE_INTERP_DEBUG );
   pZSubtask->m_vInterpSubtask = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: fnProcessDialogOperationCode
//
// PURPOSE: This routine processes the requested dialog operation code
//          If the interpretor is active ... the debugger
//
/////////////////////////////////////////////////////////////////////////////
zLONG
fnProcessDialogOperationCode( ZSubtask *pZSubtask,
                              zCPCHAR  cpcDLL_Name,
                              zCPCHAR  cpcCodeName )
{
   ZDrApp *pZApp = pZSubtask->m_pZTask->m_pZDrApp;
   zPCHAR pchDLL_Name;
   zFARPROC_DRIVER lpfnDynRoutine;
   zLONG  lRC;

   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_DELETED) == zSUBTASK_STATE_DELETED )
      return( 0 );

// TraceLineS( "fnProcessDialogOperationCode ZSubtask: ", *(pZSubtask->m_pzsWndTag) );
// pZSubtask->m_ulSubtaskFlags |= zSUBTASK_APPLICATION_OPERATION;

   SetNameForView( pZSubtask->m_vDialog, "__ZDrSubtask", 0, zLEVEL_TASK );
   fnPushMsgQ_State( pZSubtask->m_vMsgQ, zMSGQ_DialogOperation );

// SetWindowState( pZSubtask->m_vDialog, zWINDOW_STATUS_ASYNC, TRUE );

#ifdef DEBUG_ALL
   TraceLineS( "(drvr) Calling Dialog Operation ==> ", cpcCodeName );
#endif

#ifdef DEBUG_LISTWND
   ListWindowsAndControls( pZSubtask, "Pre DialogOperationCode", 0 );
#endif

   // Get the DLL name.
   if ( cpcDLL_Name[ 0 ] )
      pchDLL_Name = (zPCHAR) cpcDLL_Name;
   else
      GetAddrForAttribute( (zPCHAR *) &pchDLL_Name, pZSubtask->m_vDialog, szlDlg, szlDLL );

   if ( pZSubtask->m_vInterpSubtask )
   {
      zLONG lRetVal = 0;

      // The possible "hiword" return codes from the interpretor are:
      //   0 - no problems ... the code was executed by the interpretor
      //   1 - inform driver to execute the code from the DLL
      //   2 - please start the debugger
      lRC = (*(pZSubtask->m_pZTask->m_lpfnInterp))( pZSubtask->m_vInterpSubtask, pZSubtask->m_vDialog, pchDLL_Name, cpcCodeName, &lRetVal );
      if ( zHIUSHORT( lRC ) == 2 )
         goto OPER_EXIT_label;    // invoke debugger

      if ( zHIUSHORT( lRC ) == 0 )    // the code has been executed ... return
      {
         lRC = zMAKELONG( zLOUSHORT( lRetVal ), 0 );
         goto OPER_EXIT_label;
      }
   }

   lpfnDynRoutine = (zFARPROC_DRIVER) GetOperationDynamicCallAddress( pZSubtask->m_vDialog, (LPLPLIBRARY) &(pZSubtask->m_hLibrary),
                                                                      pchDLL_Name, cpcCodeName, "(drvr5)" );
   if ( lpfnDynRoutine )
   {
   /*
      if ( (pZSubtask->m_ulSubtaskFlags & zSUBTASK_PROCESSING_ACTIVATE) == 0 &&
            zstrstr( cpcCodeName, "InitMenu" ) != 0 && zstrstr( cpcCodeName, "ReceiveFocus" ) != 0 )
      {
         TraceLineS( "=================================>>>> Processing Dialog Operation ", cpcCodeName );
      }
   */
      pZSubtask->m_cpcCurrOper = cpcCodeName;
      try   // to catch any application generated exceptions
      {  // begin of try block

         lRC = zMAKELONG( zLOUSHORT( (*lpfnDynRoutine)( pZSubtask->m_vDialog ) ), 0 );

      }  // end of try block

      catch( CException *e )
      {
         zCHAR szMsg[ 256 ];
         zCHAR szOperMsg[ 512 ];

         e->GetErrorMessage( szMsg, sizeof( szMsg ) );
         sprintf_s( szOperMsg, "(drvr) Operation [%s] Exception ====> ", cpcCodeName );
         TraceLineS( szOperMsg, szMsg );
      // pZApp->ExitInstance( );
         return( zNO_APPLICATION_EVENT );
      }

      if ( (ZSubtask::GetSubtaskState( pZSubtask ) & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED )
      {
         pZSubtask->m_cpcCurrOper = 0;
      }
   // TraceLineS( "Terminating Dialog Operation ", cpcCodeName );
   }
   else
   {
      lRC = 0;
   }

OPER_EXIT_label:

#ifdef DEBUG_ALL
   TraceLineS( "(drvr) Return from Dialog Operation ==> ", cpcCodeName );
#endif

// SetWindowState( pZSubtask->m_vDialog, zWINDOW_STATUS_ASYNC, FALSE );

   ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
   if ( (ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED )
      fnPopMsgQ_State( pZSubtask->m_vMsgQ );

// TraceLineS( "ProcessDialogOperation Unlock Mutex ZDr", "" );
// SysMutexUnlock( pZSubtask->m_vDialog, "ZDr", 0 );

// pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_APPLICATION_OPERATION;
// DropNameForView( pZSubtask->m_vDialog, "__ZDrSubtask", 0, zLEVEL_TASK );
   return( lRC );
}
