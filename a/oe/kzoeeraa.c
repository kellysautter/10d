#include "kzoephdr.h"
//
// MODULE NAME:  KZOEERAA  -  Object services Error Handling Operations
// DESCRIPTION:  This is the source file which contains Object services
//               error setting and retrieval operations.
// OPERATIONS:   All OPERATIONS which do error object handling
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    4/12/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

1999.05.28  DGC  10a
   Removed 'temp' code to ignore error #358.

1998.01.27  DGC
   Corrected the handling of recursive errors so they would not occur when
   the user forces a Zeidon shut down.

*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#define  __DO_MSGOBJ__

//./ ADD NAME=SendDomainError
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       SendDomainError
//
//  PURPOSE:     To log an error situation
//
//  DESCRIPTION: Domain processors can call this operation to raise an
//               error situation at any time during processing.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
SendDomainError( zVIEW     zView,
                 LPDOMAIN  lpDomain,
                 zSHORT    nSeverity,
                 zLONG     lMessageID,
                 zLONG     lInfo,
                 zCPCHAR   cpcInfo1,
                 zCPCHAR   cpcInfo2 )
{
   zPCHAR      pchTitle;
   zLONG       lMsgType;
   zCHAR       szMsg[ 512 ];
   zCHAR       szMsgID[ 16 ];
   zSHORT      nEntityAttrFlag = 1;

   pchTitle = szlApplicationLogicError;
   if ( nSeverity >= 0 && nSeverity < 16 )
      lMsgType = zMSGQ_DOMAIN_ERROR;
   else
      lMsgType = zMSGQ_SYSTEM_ERROR;

   switch ( lMessageID )
   {
      case 1:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME001" );
         strcpy_s( szMsg, sizeof( szMsg ), "Invalid Input Data Type." );
         nEntityAttrFlag = 0;
         break;

      case 2:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME002" );
         strcpy_s( szMsg, sizeof( szMsg ), "Text String exceeds attribute length." );
         break;

      case 3:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME003" );
         strcpy_s( szMsg, sizeof( szMsg ), "Attribute Type invalid for this Domain." );
         break;

      case 4:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME004" );
         strcpy_s( szMsg, sizeof( szMsg ), "Invalid Domain Entry Type." );
         break;

      case 5:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME005" );
         strcpy_s( szMsg, sizeof( szMsg ), "Table_Handler invalid for this Domain." );
         break;

      case 6:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME006" );
         strcpy_s( szMsg, sizeof( szMsg ), "Integer overflow." );
         break;

      case 7:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME007" );
         strcpy_s( szMsg, sizeof( szMsg ), "Integer underflow." );
         break;

      case 8:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME008" );
         strcpy_s( szMsg, sizeof( szMsg ), "Could not find context for Domain." );
         break;

      case 9:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME009" );
         strcpy_s( szMsg, sizeof( szMsg ), "Context edit string is invalid for Domain." );
         break;

      case 10:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME010" );
         strcpy_s( szMsg, sizeof( szMsg ), "DateTime input string invalid." );
         nEntityAttrFlag = 0;
         break;

      case 11:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME011" );
         strcpy_s( szMsg, sizeof( szMsg ), "Error storing value in record." );
         break;

      case 12:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME012" );
         strcpy_s( szMsg, sizeof( szMsg ), "Context Required when Type is INTEGER. " );
         break;

      case 13:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME013" );
         strcpy_s( szMsg, sizeof( szMsg ), "Context/cType Combination is invalid." );
         break;

      case 14:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME014" );
         strcpy_s( szMsg, sizeof( szMsg ), "Context is for retrieval only." );
         break;

      case 15:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME015" );
         strcpy_s( szMsg, sizeof( szMsg ), "Context only used for arithmetic operations." );
         break;

      case 16:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME016" );
         strcpy_s( szMsg, sizeof( szMsg ), "Input invalid for context." );
         break;

      case 17:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME017" );
         strcpy_s( szMsg, sizeof( szMsg ), "Context Required when Type is DECIMAL." );
         break;

      case 18:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME018" );
         strcpy_s( szMsg, sizeof( szMsg ), "Context edit string is null." );
         break;

      case 19:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME019" );
         strcpy_s( szMsg, sizeof( szMsg ), "International number formatting is not available." );
         break;

      case 20:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME020" );
         strcpy_s( szMsg, sizeof( szMsg ), "Invalid decimal string." );
         nEntityAttrFlag = 0;
         break;

      case 21:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME021" );
         strcpy_s( szMsg, sizeof( szMsg ), "Return area not large enough for formatted string." );
         break;

      case 22:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME022" );
         strcpy_s( szMsg, sizeof( szMsg ), "Only AlphaNumeric chars are allowed." );
         nEntityAttrFlag = 0;
         break;

      case 23:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME023" );
         strcpy_s( szMsg, sizeof( szMsg ), "Value is out of valid range." );
         nEntityAttrFlag = 0;
         break;

      case 24:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME024" );
         strcpy_s( szMsg, sizeof( szMsg ), "Invalid integer string." );
         nEntityAttrFlag = 0;
         break;

      case 25:
         strcpy_s( szMsgID, sizeof( szMsgID ), "TZDME025" );
         strcpy_s( szMsg, sizeof( szMsg ), "Invalid table value." );
         nEntityAttrFlag = 0;
         break;

      default:
         strcpy_s( szMsg, sizeof( szMsg ), "Unknown Error Message " );
   }

   // If flag is off then check Zeidon INI file to see if we should turn it
   // on anyway.
   if ( nEntityAttrFlag == 0 )
   {
      zCHAR sz[ 10 ];

      SysReadZeidonIni( -1, "[Debug]", "DomainMessage", sz, sizeof( sz ) );
      if ( sz[ 0 ] == 'Y' || sz[ 0 ] == 'y' )
         nEntityAttrFlag = 1;
   }

   if ( nEntityAttrFlag == 1 )
   {
      // If the message created was for a programming error (not an expected
      // end-user message), then add the Entity and Attribute.  This information
      // only appears when there is no LAND.MSG ... otherwise the german text
      // overwrites everything!
      if ( cpcInfo1 )
      {
         strcat_s( szMsg, sizeof( szMsg ), "\n\n Debug Info 1:\t" );
         strcat_s( szMsg, sizeof( szMsg ), cpcInfo1 );
      }

      if ( cpcInfo2 )
      {
         strcat_s( szMsg, sizeof( szMsg ), "\n Debug Info 2:\t" );
         strcat_s( szMsg, sizeof( szMsg ), cpcInfo2 );
      }
   }

   TraceLine( "DomainError: %s - %s   Type: %c   for Domain: %s  Info: %s.%s",
              szMsgID, szMsg, lpDomain ? lpDomain->cDomainType : '?',
              lpDomain ? lpDomain->szName : "Domain Name Unknown",
              cpcInfo1 ? cpcInfo1 : "", cpcInfo2 ? cpcInfo2 : "" );
   if ( nSeverity == -1 ) // domain validation only ... no error message!
      return( 0 );
   else
      return( MessageSend( zView, szMsgID, pchTitle, szMsg, lMsgType, 0 ) );
}

//./ ADD NAME=IssueError
// Source Module=kzoeeraa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      IssueError
//
//  PURPOSE:    To log an error situation
//
//  DESCRIPTION: Object Services, clients, and system software can
//               call this operation to raise an error situation at
//               any time during processing.
//
//               Severity: 16 - catastrophic error      (Object services)
//                         12 - Application logic error (Object services)
//                          8 - Application logic error (Application)
//                          4 - Domain errors           (Application)
//                          0 - Information logging msg
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
IssueError( zVIEW        lpView,
            zSHORT       nSeverity,
            zSHORT       nID,
            zCPCHAR      cpcMessage )
{
   LPTASK    lpTask;
   zPCHAR    szTitle;

   if ( nSeverity < 16 )
      szTitle = szlApplicationLogicError;
   else
      szTitle = szlOE_SystemError;

   // NOTE, there is no guarantee that lpView is set OR valid.
   lpTask = zGETPTR( lpView->hTask );

   // Log the error with the subtask
   // TEMP For now, we just spit all errors out to the window!!! TEMP
   TraceLineS( "IssueError: ", cpcMessage );
   SysMessageBox( lpView, szTitle, cpcMessage, 1 );         // TEMP

   return( 0 );   // Error creating error object
}

//./ ADD NAME=fnIssueCoreError
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:    fnIssueCoreError
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT
fnIssueCoreError( LPTASK       lpTask,
                  zVIEW        lpView,
                  zSHORT       nSeverity,
                  zSHORT       nID,
                  zLONG        lLong,         // Additional info
                  zCPCHAR      cpcMessage1,   // Additional info
                  zCPCHAR      cpcMessage2 )  // Additional info
{
   if ( lpTask->nRecurse )
   {
      if ( cpcMessage2 == 0 || *cpcMessage2 == 0 )
      {
         if ( cpcMessage1 == 0 || *cpcMessage1 == 0 )
            cpcMessage2 = "No message";
         else
            cpcMessage2 = cpcMessage1;
      }

      TraceLineS( "(er) - recursive error ", cpcMessage2 );
   // SysMessageBox( "(er) - recursive error ", cpcMessage2, 1 );
      return( 0 );
   }
   else
   {
      LPSUBTASK lpFirstSubtask = zGETPTR( lpTask->hFirstSubtask );
      zSHORT    nRC;

      if ( MiValidView( lpView ) == 0 )
         lpView = zGETPTR( lpFirstSubtask->hSubtaskView );

      lpTask->nRecurse++;

      nRC = IssueOE_Error( lpView, nSeverity, nID,
                           zMSGQ_SYSTEM_ERROR,
                           lLong, cpcMessage1, cpcMessage2 );
      lpTask->nRecurse--;
      return( nRC );
   }
}

//./ ADD NAME=IssueOE_Error
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:    IssueOE_Error
//
//  nSeverity    4 - Data Validation Error
//               8 - Application Logic Error
//              16 - OE System Error
//              20 - Catastrophic Error (straight to SysMessageBox)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
IssueOE_Error( zVIEW        lpView,
               zSHORT       nSeverity,
               zSHORT       nID,
               zLONG        lMsgType,
               zLONG        lLong,          // additional info
               zCPCHAR      cpcMessage1,    // additional info
               zCPCHAR      cpcMessage2 )   // additional info
{
   LPTASK      lpTask;
   zCHAR       szCoreMessage[ 512 ];
   zCHAR       szMsgId[ 16 ];
   zVIEW       zView;
   zPCHAR      pchTitle;
   LPVIEWOD    lpViewOD;

// SysMessageBox( 0, "IssueOE_Error", cpcMessage1, -1 ); call MessageBox directly

   if ( lpView )
   {
      lpTask = zGETPTR( lpView->hTask );

      // If the view is valid we'll pass it along, otherwise we will
      // pass the first SubtaskView for the task.
      if ( lpView )
         zView = lpView;
      else
      {
         LPSUBTASK lpFirstSubtask = zGETPTR( lpTask->hFirstSubtask );
         zView = zGETPTR( lpFirstSubtask->hSubtaskView );
      }

      lpViewOD = zGETPTR( zView->hViewOD );
   }
   else
   {
      lpTask = 0;
      zView = 0;
      lpViewOD = 0;
   }

   szCoreMessage[ 0 ] = 0;
   SysGetBaseMessage( szCoreMessage, nID, 256 );
   if ( szCoreMessage[ 0 ] == 0 )
   {
      strcpy_s( szCoreMessage, sizeof( szCoreMessage ), "Unknown Error Message (" );
      zltoa( nID, szCoreMessage + 490, sizeof( szCoreMessage ) - 490 );
      strcat_s( szCoreMessage, sizeof( szCoreMessage ), szCoreMessage + 490 );
      strcat_s( szCoreMessage, sizeof( szCoreMessage ), ") " );
   }

   if ( lLong )
   {
      zltoa( lLong, szCoreMessage + 490, sizeof( szCoreMessage ) - 490 );
      strcat_s( szCoreMessage, sizeof( szCoreMessage ), &szCoreMessage[ 490 ] );
   }

   if ( cpcMessage1 )  // if any additional information
   {
      strcat_s( szCoreMessage, sizeof( szCoreMessage ), ",\n\n " );
      strcat_s( szCoreMessage, sizeof( szCoreMessage ), cpcMessage1 );
      if ( cpcMessage2 )
      {
         strcat_s( szCoreMessage, sizeof( szCoreMessage ), ",\n" );
         strcat_s( szCoreMessage, sizeof( szCoreMessage ), cpcMessage2 );
      }
   }

   TraceLineS( "IssueOE_Error : ", szCoreMessage );

   // See if we can determine the view object resulting in the error.
   // But don't do it for EE071 or EE101 as it implies erroneous information.
   if ( lpView && lpViewOD && nID != 71 && nID != 101 )  // dks 2006.04.27
   {
       strcat_s( szCoreMessage, sizeof( szCoreMessage ), ",\n\nView Object: " );
       strcat_s( szCoreMessage, sizeof( szCoreMessage ), lpViewOD->szName );
   }

   // See if we can determine the operation.
   if ( lpTask && lpTask->bAudit && lpTask->nOperIdx > -1 )
   {
      zCHAR szOperationName[ 32 ];

      szOperationName[ 0 ] = 0;
      SysGetOperationMessage( szOperationName,
                           // lpTask->nOper[ 0 ], 31 );
                              lpTask->nOper[ lpTask->nOperIdx ], 31 );
      if ( szOperationName[ 0 ] == 0 )
      {
         szOperationName[ 0 ] = '(';
      // zltoa( lpTask->nOper[ 0 ], szOperationName + 1, sizeof( szOperationName ) - 1 );
         zltoa( lpTask->nOper[ lpTask->nOperIdx ], szOperationName + 1, sizeof( szOperationName ) - 1 );
         strcat_s( szOperationName, sizeof( szOperationName ), ")" );
      }

      strcat_s( szCoreMessage, sizeof( szCoreMessage ), ",\nOperation: " );
      strcat_s( szCoreMessage, sizeof( szCoreMessage ), szOperationName );
      if ( lpTask->nOperIdx > 0 )
      {
         zSHORT k;
         for ( k = lpTask->nOperIdx - 1; k > -1 ; k-- )
         {
            strcat_s( szCoreMessage, sizeof( szCoreMessage ), ", " );
            szOperationName[ 0 ] = 0;
            SysGetOperationMessage( szOperationName, lpTask->nOper[ k ], 31 );
            if ( szOperationName[ 0 ] == 0 )
            {
               szOperationName[ 0 ] = '(';
               zltoa( lpTask->nOper[ k ], szOperationName + 1, sizeof( szOperationName ) - 1 );
               strcat_s( szOperationName, sizeof( szOperationName ), ") " );
            }

            strcat_s( szCoreMessage, sizeof( szCoreMessage ), szOperationName );
         }
      }
   }

   TraceLine( "IssueOE_Error:  %s  Severity: %d", szCoreMessage, nSeverity );

   if ( nSeverity > 16 )
      return( IssueError( zView, nSeverity, nID, szCoreMessage ) );

#ifndef __DO_MSGOBJ__
   if ( lpTask == 0 )
      return( IssueError( zView, nSeverity, nID, szCoreMessage ) );
#endif

   if ( nSeverity == 16 )
      pchTitle = szlOE_SystemError;
   else
      pchTitle = szlApplicationLogicError;

   zltoa( nID, szMsgId, sizeof( szMsgId ) ); // convert message id to a string
   return( MessageSend( zView, szMsgId,
                        szlApplicationLogicError,
                        szCoreMessage,
                        lMsgType + nSeverity, 0 ) );
}