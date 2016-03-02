#include "kzoephdr.h"
//
// MODULE NAME:  KZOEMQAA  -  Object services MessageQueue Operations
// DESCRIPTION:  This is the source file which contains Object services
//               Message Queue Operations and functions
// **********************************************************************
// * Copyright (c) 1993-2010 Quinsoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Doug and John
//  DATE:    4/1/93
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2000.11.29  HH
   Check to make sure we aren't calling message functions recursively.

2000.11.29  HH
   change GetViewByName with zLEVEL_TASK.

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

27.02.1997  HH
   MessagePresent corrected
*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#define  __DO_MSGOBJ__

//
// Header info used only in this module.
//

typedef zSHORT (POPERATION MQ_MessageSend) ( zVIEW   vMsgQ,
                                             zVIEW   vSubtask,
                                             zCPCHAR cpcMsgId,
                                             zCPCHAR cpcTitle,
                                             zCPCHAR cpcMsgText,
                                             zLONG   lMsgType,
                                             zSHORT  bBeep );

typedef zSHORT (POPERATION MQ_MessagePrompt) ( zVIEW   vMsgQ,
                                               zVIEW   vSubtask,
                                               zCPCHAR cpcMsgId,
                                               zCPCHAR cpcTitle,
                                               zCPCHAR cpcMsgText,
                                               zSHORT  bBeep,
                                               zSHORT  nButtons,
                                               zSHORT  nDefaultButton,
                                               zSHORT  nIcon );

typedef zSHORT (POPERATION MQ_MessagePromptForInput) ( zVIEW   vMsgQ,
                                                       zVIEW   vSubtask,
                                                       zCPCHAR cpcMsgId,
                                                       zCPCHAR cpcTitle,
                                                       zCPCHAR cpcMsgText,
                                                       zSHORT  bBeep,
                                                       zPCHAR  pchReturnBuffer,
                                                       zUSHORT usBufferLth );

typedef zSHORT (POPERATION MQ_MessagePresent) ( zVIEW  vMsgQ,
                                                zVIEW  vSubtask,
                                                zLONG  lEventType );

// This operation is called from MessageSend and also from kzoejava.c
// CallDialogOperation.  CallDialogOperation needs to know if there is
// a QMsg entity for the Task.  If so, CallDialogOperation does not call
// the operation (dependent upon flag passed to CallDialogOperation).
zVIEW OPERATION
fnGetMsgQ( zVIEW vSubtask, zPVOID lpTask, zSHORT lFlag )
{
   LPTASK lpCurrentTask;
   zVIEW  vMsgQ;

   if ( lpTask )
      lpCurrentTask = (LPTASK) lpTask;
   else
      lpCurrentTask = zGETPTR( vSubtask->hTask );

   GetViewByName( &vMsgQ, szl__MSGQ, vSubtask, zLEVEL_TASK );
   if ( vMsgQ == 0 )
   {
      if ( lpCurrentTask->vWebSubtask )
      {
         if ( ActivateEmptyObjectInstance( &vMsgQ, "KZMSGQOO",
                                           lpCurrentTask->vWebSubtask,
                                           zMULTIPLE ) >= 0 )
         {
            SetNameForView( vMsgQ, szl__MSGQ,
                            lpCurrentTask->vWebSubtask, zLEVEL_TASK );
         }
      }
   }

   if ( vMsgQ )
   {
      CreateViewFromViewForTask( &vMsgQ, vMsgQ, 0 );

      // We're gonna pop up a message box.  It's possible that the driver
      // will try to clean up any non-named, non-locked views.  Prevent
      // the temp view from being cleaned up by locking it.
      SfLockView( vMsgQ );
      if ( lFlag & 0x00000001 )  // determine if there are QMsg records
      {
         if ( lpCurrentTask->vWebSubtask )
         {
            if ( SetCursorFirstEntityByInteger( vMsgQ, szlTask, szlId,
                                                (zLONG) lpCurrentTask->vWebSubtask,
                                                0 ) != 0 ||
                 CheckExistenceOfEntity( vMsgQ, "QMsg" ) != 0 )
            {
               DropView( vMsgQ );
               vMsgQ = 0;  // there are no QMsg records for this task
            }
         }
      }
   }

   return( vMsgQ );
}

//./ ADD NAME=MessageSend
// Source Module=kzoemqaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MessageSend
//
//  PURPOSE: This is the routine that is used by most functions for
//           issuing an error message.  Depending on the lMsgType, the
//           message may be sent immediately to the operator or queued
//           for later processing.
//
//  PARAMETERS:
//           vSubtask   -  View associated with message.
//           cpcMsgId   -  Id of the particular message.
//           cpcTitle   -  Optional Title for the dialog box
//           cpcMsgText -  Message to be displayed in the dialog box
//           lMsgType   -  Identifies the type of message. (zMSGQ_...)
//           bBeep      -  Annoy user with a beep
//
//  RETURNS:
//           0 - Message was processed
//          -1 - Error in processing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
MessageSend( zVIEW   vSubtask,
             zCPCHAR cpcMsgId,
             zCPCHAR cpcTitle,
             zCPCHAR cpcMsgText,
             zLONG   lMsgType,
             zSHORT  bBeep )
{
   LPTASK         lpCurrentTask;
   LPMQINTERFACE  lpMQInterface;
   zVIEW          vMsgQ;
// zPLONG         lplTaskID;
   zSHORT         nRC = zCALL_ERROR;
   zCHAR          sz[ 10 ];

   lpCurrentTask = fnOperationCall( iMessageSend, vSubtask, 0 );
   if ( lpCurrentTask == 0 )
   {
      TraceLine( "MessageSend Task Error: %s  %s  %s  %d",
                 cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
   }

// lplTaskID = &lpCurrentTask->lTaskID;

#ifdef __DO_MSGOBJ__
// if ( fnValidView( lpCurrentTask, vSubtask ) == 0 )
// {
//    fnOperationReturn( iMessageSend, lpCurrentTask );
//    return( nRC );
// }

   lpMQInterface = fnFindMQInterface( vSubtask, lpCurrentTask );

   if ( lpMQInterface == 0 || lpMQInterface->pfnMQ[ 0 ] == 0 )
   {
      TraceLine( "MessageSend Interface Inactive: %s  %s  %s  %d",
                 cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
      if ( lpCurrentTask == 0 || lpCurrentTask->vWebSubtask == 0 )
      {
         zCHAR szMsg[ 512 ];

         sprintf_s( szMsg, zsizeof( szMsg ), "Message Interface Inactive: %s %s %s %d",
                   cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
         SysMessageBox( vSubtask, g_pchMQI[ 0 ], szMsg, 1 );
      }
   }
   else
   if ( lpMQInterface->bRecursiveCall )
   {
      TraceLine( "MessageSend Recursive Error: %s  %s  %s  %d",
                 cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
   // SysMessageBox( vSubtask, g_pchMQI[ 0 ], "Recursive error", 1 );
   // SysMessageBox( vSubtask, cpcTitle, cpcMsgText, 1 );
   }
   else
   {
      TraceLine( "MessageSend Interface Active: %s  %s  %s  %d",
                 cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
      lpMQInterface->bRecursiveCall = TRUE;
      vMsgQ = fnGetMsgQ( vSubtask, lpCurrentTask, 0 );

      // If the zeidon.ini requests that all messages be sent to the User, but
      // there is not a valid message object, we will simply exit, because we
      // cannot get a message back to the User.
      SysReadZeidonIni( -1, "[ObjectEngine]", "WebUserErrorMessages", sz, zsizeof( sz ) );
      if ( (sz[ 0 ] == 'Y' || sz[ 0 ] == 'y') && vMsgQ == 0 )
         nRC = -16;
      else
      {
         // When running on the Web, messages will be sent back to the End User if:
         // 1. The error message type is Domain, Constraint Error or Constraint Warning. (These
         //    messages should always be returned to the End User.)
         // 2. The zeidon.ini variable, WebUserErrorMessages, is set to Y. (This allows us to have
         //    messages pop up when debugging but to be sent back to the User in production).
         if ( lpCurrentTask->vWebSubtask )
         {
            if ( sz[ 0 ] == 'Y' || sz[ 0 ] == 'y' ||
                 lMsgType == zMSGQ_DOMAIN_ERROR ||
                 lMsgType == zMSGQ_OBJECT_CONSTRAINT_ERROR ||
                 lMsgType == zMSGQ_OBJECT_CONSTRAINT_WARNING ||
                 lMsgType == zMSGQ_REQUIRED_DATA_ITEM_ERROR )
            {
               if ( vMsgQ )
               {
                  TraceLine( "MessagePrompt Trying to pop up a Web message box: %s for WebSubtask: 0x%08x",
                             cpcMsgText, lpCurrentTask->vWebSubtask );
                  if ( SetCursorFirstEntityByInteger( vMsgQ, szlTask, szlId,
                                                      (zLONG) lpCurrentTask->vWebSubtask,
                                                      0 ) != 0 )
                  {
                  // TraceLine( "MessageSend Creating Task Entity: %s  %s  %s  %d",
                  //            cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
                     CreateEntity( vMsgQ, szlTask, zPOS_LAST );
                  // TraceLine( "MessageSend Created Task Entity: %s  %s  %s  %d",
                  //            cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
                     SetAttributeFromVariable( vMsgQ, szlTask, szlId,
                                               &lpCurrentTask->vWebSubtask,
                                               zTYPE_INTEGER,
                                               0, 0, 0 );
                     SetAttributeFromVariable( vMsgQ, szlTask, "Client",
                                               "ZeidonWeb", zTYPE_STRING, 0, 0, 0 );
                  }

               // TraceLine( "MessageSend Creating QMsg Entity: %s  %s  %s  %d",
               //            cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
                  CreateEntity( vMsgQ, "QMsg", zPOS_LAST );
               // TraceLine( "MessageSend Created QMsg Entity: %s  %s  %s  %d",
               //            cpcMsgId, cpcTitle, cpcMsgText, lMsgType );
                  SetAttributeFromString( vMsgQ, "QMsg", szlId, cpcMsgId );
                  SetAttributeFromString( vMsgQ, "QMsg", "Title", cpcTitle );
                  SetAttributeFromString( vMsgQ, "QMsg", "Text", cpcMsgText );
                  SetAttributeFromInteger( vMsgQ, "QMsg", "View", (zLONG) vSubtask );
               }
               else
               {
                  // We are in the Web without a valid view (problem "Invalid View" error), so simply return with error code.
                  nRC = -1;
               }
            }
            else
            {
               if ( vMsgQ )
               {
                  MQ_MessageSend pfn = (MQ_MessageSend) lpMQInterface->pfnMQ[ 0 ];
               // TraceLineS( "MessagePrompt Trying to pop up a message box: ",
               //             cpcMsgText );
                  nRC = (*pfn)( vMsgQ, vSubtask, cpcMsgId, cpcTitle,
                                cpcMsgText, lMsgType, bBeep );
                  DropView( vMsgQ );
               }
               else
                  IssueError( vSubtask, 0, 0, cpcMsgText );
            }
         }
         else
         {
            if ( vMsgQ )
            {
               MQ_MessageSend pfn = (MQ_MessageSend) lpMQInterface->pfnMQ[ 0 ];
            // TraceLineS( "MessagePrompt Trying to pop up a message box: ",
            //             cpcMsgText );
               nRC = (*pfn)( vMsgQ, vSubtask, cpcMsgId, cpcTitle,
                          cpcMsgText, lMsgType, bBeep );
               DropView( vMsgQ );
            }
            else
               IssueError( vSubtask, 0, 0, cpcMsgText );
         }
      }

      lpMQInterface->bRecursiveCall = FALSE;
   }

#endif

   if ( lpCurrentTask )
      fnOperationReturn( iMessageSend, lpCurrentTask );

   return( nRC );
}

//./ ADD NAME=MessagePrompt
// Source Module=kzoemqaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MessagePrompt
//
//  PURPOSE: To display a message box prompting the operator for a
//           response..
//
//  PARAMETERS:
//           vSubtask       - View associated with message.
//           cpcMsgId       - Id of the particular message.
//           cpcTitle       - Optional Title for the dialog box
//           cpcMsgText     - Message to be displayed in the dialog box
//           bBeep          - Annoy user with a beep
//           nButtons       - Buttons to place on the window, values are
//                            0 - OK button
//                            zBUTTONS_YESNO - Yes and No
//                            zBUTTONS_YESNOCANCEL - Yes, No, Cancel
//                            zBUTTONS_OK    - OK button only
//                            zBUTTONS_OKCANCEL - OK and CANCEL
//                            zBUTTONS_RETRYCANCEL - Retry and Cancel
//                            zBUTTONS_ABORTRETRYIGNORE - Abort,Retry,
//                                                        Ignore
//           nDefaultButton - 0 Default button is first button.
//                            zRESPONSE_YES     - Default is YES
//                            zRESPONSE_NO      - Default is NO
//                            zRESPONSE_CANCEL  - Default is CANCEL
//                            zRESPONSE_OK      - Default is OK
//                            zRESPONSE_ABORT   - Default is Abort
//                            zRESPONSE_RETRY   - Default is Retry
//                            zRESPONSE_IGNORE  - Default is Ignore
//           nIcon          - 0 - default based on buttons
//                                * Information icon for OK
//                                * Question mark for yes/no type
//                                * Exclamation mark for OK/CANCEL
//                                * Error (stop) icon for retry cancel
//
//                            zICON_INFORMATION - Information Icon
//                            zICON_QUESTION    - Question mark icon
//                            zICON_EXCLAMATION - Exclamation mark icon
//                            zICON_ERROR       - Error ( STOP ) Icon
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
//./ END + 9
zSHORT OPERATION
MessagePrompt( zVIEW   vSubtask,
               zCPCHAR cpcMsgId,
               zCPCHAR cpcTitle,
               zCPCHAR cpcMsgText,
               zSHORT  bBeep,
               zSHORT  nButtons,
               zSHORT  nDefaultButton,
               zSHORT  nIcon )
{
   LPTASK         lpCurrentTask;
   LPMQINTERFACE  lpMQInterface;
// zPLONG         lplTaskID;
   zVIEW          vMsgQ;
   zSHORT         nRC = zCALL_ERROR;

   lpCurrentTask = fnOperationCall( iMessagePrompt, vSubtask, 0 );
   if ( lpCurrentTask == 0 )
   {
      TraceLine( "MessagePrompt Task Error: %s  %s  %s",
                 cpcMsgId, cpcTitle, cpcMsgText );
   }

// lplTaskID = &lpCurrentTask->lTaskID;

#ifdef __DO_MSGOBJ__
// if ( fnValidView( lpCurrentTask, vSubtask ) == 0 )
// {
//    fnOperationReturn( iMessagePrompt, lpCurrentTask );
//    return( nRC );
// }

   lpMQInterface = fnFindMQInterface( vSubtask, lpCurrentTask );

   if ( lpMQInterface == 0 || lpMQInterface->pfnMQ[ 1 ] == 0 )
   {
      SysMessageBox( vSubtask, g_pchMQI[ 1 ], "Interface Inactive", 1 );
      SysMessageBox( vSubtask, cpcTitle, cpcMsgText, 1 );
   }
   else
   if ( lpMQInterface->bRecursiveCall )
   {
      TraceLine( "MessagePrompt Recursive Error: %s  %s  %s",
                 cpcMsgId, cpcTitle, cpcMsgText );
   // SysMessageBox( vSubtask, g_pchMQI[ 0 ], "Recursive error", 1 );
   // SysMessageBox( vSubtask, cpcTitle, cpcMsgText, 1 );
   }
   else
   {
      lpMQInterface->bRecursiveCall = TRUE;

      GetViewByName( &vMsgQ, szl__MSGQ, vSubtask, zLEVEL_TASK );
      if ( vMsgQ == 0 )
      {
         if ( lpCurrentTask->vWebSubtask )
         {
            if ( ActivateEmptyObjectInstance( &vMsgQ, "KZMSGQOO",
                                              lpCurrentTask->vWebSubtask,
                                              zMULTIPLE ) >= 0 )
            {
               SetNameForView( vMsgQ, szl__MSGQ,
                               lpCurrentTask->vWebSubtask, zLEVEL_TASK );
            }
         }
      }

      if ( vMsgQ )
      {
         CreateViewFromViewForTask( &vMsgQ, vMsgQ, 0 );

         // We're gonna pop up a message box.  It's possible that the driver
         // will try to clean up any non-named, non-locked views.  Prevent
         // the temp view from being cleaned up by locking it.
         SfLockView( vMsgQ );
         if ( lpCurrentTask->vWebSubtask )
         {
            TraceLineS( "MessagePrompt Trying to pop up a Web message box: ",
                        cpcMsgText );
            if ( SetCursorFirstEntityByInteger( vMsgQ, szlTask, szlId,
                                                (zLONG) lpCurrentTask->vWebSubtask,
                                                0 ) != 0 )
            {
               CreateEntity( vMsgQ, szlTask, zPOS_LAST );
               SetAttributeFromVariable( vMsgQ, szlTask, szlId,
                                         &lpCurrentTask->vWebSubtask,
                                         zTYPE_INTEGER,
                                         0, 0, 0 );
               SetAttributeFromVariable( vMsgQ, szlTask, "Client",
                                         "ZeidonWeb", zTYPE_STRING, 0, 0, 0 );
            }

            CreateEntity( vMsgQ, "QMsg", zPOS_LAST );
            SetAttributeFromString( vMsgQ, "QMsg", szlId, cpcMsgId );
            SetAttributeFromString( vMsgQ, "QMsg", "Title", cpcTitle );
            SetAttributeFromString( vMsgQ, "QMsg", "Text", cpcMsgText );
            SetAttributeFromInteger( vMsgQ, "QMsg", "View", (zLONG) vSubtask );
            if ( nButtons == 4 ) // zBUTTONS_OKCANCEL
               SetAttributeFromInteger( vMsgQ, "QMsg", "Type", -1 );  // prompt
         }
         else
         {
            MQ_MessagePrompt pfn = lpMQInterface->pfnMQ[ 1 ];
         // TraceLineS( "MessagePrompt Trying to pop up a message box: ", cpcMsgText );
            nRC = (*pfn)( vMsgQ, vSubtask, cpcMsgId, cpcTitle,
                          cpcMsgText, bBeep, nButtons,
                          nDefaultButton, nIcon );
         }

         DropView( vMsgQ );
      }

      lpMQInterface->bRecursiveCall = FALSE;
   }

#endif

   if ( lpCurrentTask )
      fnOperationReturn( iMessagePrompt, lpCurrentTask );

   return( nRC );
}

//./ ADD NAME=MessagePromptForInput
// Source Module=kzoemqaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MessagePromptForInput
//
//  PURPOSE: To display an input dialog box prompting the operator
//           for input.
//
//  PARAMETERS:
//           vSubtask         - Subtask View associated with message.
//           cpcMsgId         - Id of the particular message.
//           cpcTitle         - Optional Title for the dialog box
//           cpcMsgText       - Message to be displayed in the dialog box
//           bBeep            - Annoy user with a beep
//           lpchReturnBuffer - Pointer to the return buffer
//           usBufferLth      - Maximum length of the return buffer
//
//  RETURNS:
//           0 - Message was processed
//          -1 - Error in processing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
MessagePromptForInput( zVIEW   vSubtask,
                       zCPCHAR cpcMsgId,
                       zCPCHAR cpcTitle,
                       zCPCHAR cpcMsgText,
                       zSHORT  bBeep,
                       zPCHAR  pchReturnBuffer,
                       zUSHORT usBufferLth )
{
   LPTASK         lpCurrentTask;
   LPMQINTERFACE  lpMQInterface;
// zPLONG         lplTaskID;
   zVIEW          vMsgQ;
   zSHORT         nRC = zCALL_ERROR;

   lpCurrentTask = fnOperationCall( iMessagePromptForInput, vSubtask, 0 );
   if ( lpCurrentTask == 0 )
   {
      TraceLine( "MessagePromptForInput Task Error: %s  %s  %s",
                 cpcMsgId, cpcTitle, cpcMsgText );
   }

// lplTaskID = &lpCurrentTask->lTaskID;

#ifdef __DO_MSGOBJ__
// if ( fnValidView( lpCurrentTask, vSubtask ) == 0 )
// {
//    fnOperationReturn( iMessagePromptForInput, lpCurrentTask );
//    return( nRC );
// }

   lpMQInterface = fnFindMQInterface( vSubtask, lpCurrentTask );

   if ( lpMQInterface == 0 || lpMQInterface->pfnMQ[ 2 ] == 0 )
   {
      SysMessageBox( vSubtask, g_pchMQI[ 2 ], "Interface Inactive", 1 );
      SysMessageBox( vSubtask, cpcTitle, cpcMsgText, 1 );
   }
   else
   if ( lpMQInterface->bRecursiveCall )
   {
      TraceLine( "MessagePromptForInput Recursive Error: %s  %s  %s",
                 cpcMsgId, cpcTitle, cpcMsgText );
   // SysMessageBox( vSubtask, g_pchMQI[ 0 ], "Recursive error", 1 );
   // SysMessageBox( vSubtask, cpcTitle, cpcMsgText, 1 );
   }
   else
   {
      lpMQInterface->bRecursiveCall = TRUE;

      GetViewByName( &vMsgQ, szl__MSGQ, vSubtask, zLEVEL_TASK );
      if ( vMsgQ )
      {
         MQ_MessagePromptForInput pfn;

      // TraceLineS( "MessagePromptForInput Trying to pop up a message box: ",
      //             cpcMsgText );
         pfn = lpMQInterface->pfnMQ[ 2 ];
         CreateViewFromViewForTask( &vMsgQ, vMsgQ, 0 );
         nRC = (*pfn)( vMsgQ, vSubtask, cpcMsgId, cpcTitle,
                       cpcMsgText, bBeep, pchReturnBuffer, usBufferLth );
         DropView( vMsgQ );
      }

      lpMQInterface->bRecursiveCall = FALSE;
   }
#endif

   if ( lpCurrentTask )
      fnOperationReturn( iMessagePromptForInput, lpCurrentTask );

   return( nRC );
}

//./ ADD NAME=MessagePresent
// Source Module=kzoemqaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MessagePresent
//
//  PURPOSE: This is the normal operation used by a dialog to handle a
//           queued error message. It presents the error to the operator,
//           dequeues it from the error object and returns a code for the
//           action taken by the operator on the message.
//
//  PARAMETERS:
//           vSubtask   - View associated with message.
//           lEventType - Type processing when error occurred.
//                        (zMSGQ_PROCESS_...)
//
//  RETURNS:
//           0 - Message was processed
//          -1 - Error in processing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
MessagePresent( zVIEW    vSubtask,
                zLONG    lEventType,
                zVIEW    vMsgQ )
{
   LPTASK         lpCurrentTask;
   LPMQINTERFACE  lpMQInterface;
// zPLONG         lplTaskID;
   zSHORT         nRC = zCALL_ERROR;

   lpCurrentTask = fnOperationCall( iMessagePresent, vSubtask, 0 );
   if ( lpCurrentTask == 0 )
   {
      TraceLine( "MessagePresent Task Error: %d", lEventType );
   }

// lplTaskID = &lpCurrentTask->lTaskID;

#ifdef __DO_MSGOBJ__
// if ( fnValidView( lpCurrentTask, vSubtask ) == 0 )
// {
//    fnOperationReturn( iMessagePresent, lpCurrentTask );
//    return( nRC );
// }

   lpMQInterface = fnFindMQInterface( vSubtask, lpCurrentTask );

   if ( lpMQInterface == 0 || lpMQInterface->pfnMQ[ 3 ] == 0 )
   {
      SysMessageBox( vSubtask, g_pchMQI[ 3 ], "Interface Inactive", 1 );
   }
   else
   if ( lpMQInterface->bRecursiveCall )
   {
      TraceLine( "MessagePresent Recursive Error: %d", lEventType );
   // SysMessageBox( vSubtask, g_pchMQI[ 0 ],
   //                "Recursive error on MessagePresent", 1 );
   }
   else
   {
      lpMQInterface->bRecursiveCall = TRUE;

      if ( vMsgQ == 0 )
         GetViewByName( &vMsgQ, szl__MSGQ, vSubtask, zLEVEL_TASK );

      if ( vMsgQ ) // **HH**
      {
         MQ_MessagePresent pfn;

      // TraceLineS( "MessagePresent Trying to pop up a message box", "" );
         pfn = (MQ_MessagePresent) lpMQInterface->pfnMQ[ 3 ];
         CreateViewFromViewForTask( &vMsgQ, vMsgQ, 0 );
         nRC = (*pfn)( vMsgQ, vSubtask, lEventType );
         DropView( vMsgQ );
      }

      lpMQInterface->bRecursiveCall = FALSE;
   }
#endif

   if ( lpCurrentTask )
      fnOperationReturn( iMessagePresent, lpCurrentTask );

   return( nRC );
}

//./ ADD NAME=fnFindMQInterface
// Source Module=kzoemqaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnFindMQInterface
//
//  PURPOSE: Find MessageQueue Interface function pointers
//
//  RETURNS: List of function pointers, or 0 if none found or errors.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPMQINTERFACE
fnFindMQInterface( zVIEW vSubtask, LPTASK lpTask )
{
   LPAPP          lpApp;
   LPTASKAPP      lpTaskApp;
   LPMQINTERFACE  lpMQInterface;

   // DGC 12/2/96 Use Core function.
   fnGetApplicationForSubtask( &lpApp, vSubtask );

   // If a task is coming up, then it may not yet be tied to an
   // application, in this case get out fast.
   if ( lpApp == 0 )
      return( 0 );

   // Find the appropriate TaskAppRecord.
   lpTaskApp = fnFindTaskApp( vSubtask, lpTask, lpApp );

   if ( lpTaskApp == 0 )
      return( 0 );
   else
   {
      // TaskAppRecord for the application was found on the chain.
      lpMQInterface = zGETPTR( lpTaskApp->hMQInterface );
   }

   return( lpMQInterface );
}

//./ ADD NAME=fnFindTaskApp
// Source Module=kzoemqaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnFindTaskApp
//
//  PURPOSE: Find the TaskAppRecord for the combined TaskRecord and
//           AppRecord.  If the TaskAppRecord is not currently in the
//           chain, then one is created and added to the chain.
//
//  RETURNS: pointer to the TaskAppRecord
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPTASKAPP
fnFindTaskApp( zVIEW vSubtask, LPTASK lpTask, LPAPP lpApp )
{
   LPTASKAPP   lpTaskApp;
   zPVOID      hTaskApp;
   zBOOL       bCorrectProcess = TRUE;
   zLONG lProcessID = SysGetProcessID( 0 );

   // Scan the TaskApp chain for the matching application.
   if ( lpTask )
   {
      for ( lpTaskApp = zGETPTR( lpTask->hFirstTaskApp );
            lpTaskApp;
            lpTaskApp = zGETPTR( lpTaskApp->hNextTaskApp ) )
      {
         if ( lpTaskApp->lProcessID == lProcessID &&
              zGETPTR( lpTaskApp->hApp ) == lpApp )
         {
            // Ensure it is for the correct process.
            LPLIBRARY lpLibrary = zGETPTR( lpTaskApp->hLibrary );

            if ( lpLibrary && lpLibrary->lProcessID != lProcessID )
               bCorrectProcess = FALSE;

            break;
         }
      }
   }
   else
      lpTaskApp = 0;

   if ( lpTaskApp == 0 || bCorrectProcess == FALSE )
   {
      LPVIEWOD       lpViewOD;
      LPMQINTERFACE  lpMQInterface;
      zPVOID         hMQInterface;
      LPLIBRARY      hLibrary;

      // If the application was not found on the TaskApp chain, create
      // a new TaskAppRecord for this application and put it on the chain.
      if ( lpTaskApp == 0 ) // need to allocate Task/Interface object
      {
         hTaskApp = fnAllocDataspace( lpTask->hFirstDataHeader,
                                      sizeof( TaskAppRecord ),
                                      1, 0, iTaskApp );
         lpTaskApp = zGETPTR( hTaskApp );
         lpTaskApp->lProcessID = lProcessID;
      }

      if ( lpTaskApp == 0 )
      {
         LPTASK lpTask = zGETPTR( vSubtask->hTask );
         // "KZOEE012 - Error allocating memory"
         fnIssueCoreError( lpTask, vSubtask, 20, 12, 0,
                           "fnFindMQInterface", "TaskAppRecord" );
         return( 0 );
      }

      if ( bCorrectProcess ) // true if we allocated Task/Interface object
      {
         zBOOL bMutexLocked;

         lpTaskApp->hTask = zGETHNDL( lpTask );
         lpTaskApp->hApp  = zGETHNDL( lpApp );

         // Alloc space for the MessageQueue processing entry points.
         hMQInterface = fnAllocDataspace( lpTask->hFirstDataHeader,
                                          sizeof( MQInterfaceRecord ), 1, 0,
                                          iMQInterface );
         lpMQInterface = zGETPTR( hMQInterface );
         if ( lpMQInterface == 0 )
         {
            LPTASK lpTask = zGETPTR( vSubtask->hTask );
            // "KZOEE012 - Error allocating memory"
            fnIssueCoreError( lpTask, vSubtask, 20, 12, 0, "fnFindMQInterface",
                              "MQInterfaceRecord" );
            fnFreeDataspace( lpTaskApp );
            return( 0 );
         }

         // Save list of entry points.
         lpTaskApp->hMQInterface = hMQInterface;

         // Chain the TaskAppRecord off the Task record (single Link List).
         if ( lpTaskApp->hTask == AnchorBlock->hMainTask )
         {
            bMutexLocked = TRUE;
            zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
         }
         else
            bMutexLocked = FALSE;

         lpTaskApp->hNextTaskApp = lpTask->hFirstTaskApp;
         lpTask->hFirstTaskApp = hTaskApp;

         if ( bMutexLocked )
            zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      }
      else
      {
         SysFreeLibrary( vSubtask, lpTaskApp->hLibrary );
      }

      // Find the Message object so that we can get the dll name for
      // message queue object processing.
      for ( lpViewOD = zGETPTR( lpApp->hFirstViewOD );
            lpViewOD;
            lpViewOD = zGETPTR( lpViewOD->hNextViewOD ) )
      {
         if ( zstrcmp( lpViewOD->szName, szlMessageObject ) == 0 )
            break;
      }

      if ( lpViewOD == 0 )
      {
         zCHAR szMsg[ 512 ];

      // LPTASK lpTask = zGETPTR( vSubtask->hTask );

         // "KZOEE330 - Message Object Definition not loaded for Application"
      // fnIssueCoreError( lpTask, lpView, 20, 330, 0, lpApp->hName, 0 );

         // Don't use the fnIssueCoreError above because will just cause
         // a recursive call if the message object can't be found.
         strcpy_s( szMsg, zsizeof( szMsg ), "Cannot load Message Object: " );
         strcat_s( szMsg, zsizeof( szMsg ), szlMessageObject );
         SysMessageBox( vSubtask, "System Error", szMsg, 1 );
      }
      else
      {
         hLibrary = SysLoadLibraryWithErrFlag( vSubtask,
                                               lpViewOD->szOperLibname, 0 );

         // If we have a library, get all the Message Queueing Interfaces
         // that we know about.
         if ( hLibrary )
         {
            zSHORT  k;

            lpTaskApp->hLibrary = hLibrary;
            for ( k = 0; k < MAX_CORE_MQ_INT; k++ )
               lpMQInterface->pfnMQ[ k ] = SysGetProc( hLibrary, g_pchMQI[ k ] );
         }
         else
         {
            LPTASK lpTask = zGETPTR( vSubtask->hTask );
            // "KZOEE067 - Could not initialize the Core-MQ interface "
            fnIssueCoreError( lpTask, vSubtask, 20, 67, 3L,
                              lpViewOD->szOperLibname, 0 );
         }
      }
   }

   return( lpTaskApp );
}
