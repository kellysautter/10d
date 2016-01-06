/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrmsgq.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of message queue routines used by
// the driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.07.08    DKS    Z10    Enhance
//    Permit OE to determine ZDr state (Dlg Tag, Wnd Tag, Current Operation)
//    given the current vSubtask.
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


/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: fnPushMsgQ_State
//
// PURPOSE: This routine sets the state of the message queue object
//          and pushes the state on the stack.  The stack is organized
//          as follows:
//
//             zLONG  lCurrentState;
//             zSHORT nEntryCnt;
//             zSHORT nEntryMax;
//             zLONG  lState[ 1 ];
//             zLONG  lState[ 2 ];
//                 .
//                 .
//                 .
//
//             zLONG  lState[ n ];  // same as lCurrentState
//
/////////////////////////////////////////////////////////////////////////////
void
fnPushMsgQ_State( zVIEW vMsgQ,
                  zLONG lState )
{
   zPCHAR  pStack;    // blob containing Status Stack
   zUSHORT uCnt;
   zUSHORT uMax;

#ifdef zREMOTE_SERVER
   // Protect this with a semaphore!!!
   SysMutexLock( vSubtask, "ZDm", 0, 0 );
// TraceLineS( "PushMsgQ_State Lock Mutex ZDm", "" );
// DisplayObjectInstance( vMsgQ, 0, 0 );
// ::MessageBox( 0, "PushMsgQ", "ZDr", MB_OK );
#endif

   GetAddrForAttribute( (zPCHAR *) &pStack, vMsgQ, szlTask, szlStatus );
   if ( pStack )
   {
      uCnt = *((zPSHORT) (pStack + sizeof( zLONG )));
      uMax = *((zPSHORT) (pStack + sizeof( zLONG ) + sizeof( zSHORT )));

      // If there is no more room on the stack ... make more room.
      if ( uCnt >= uMax )
      {
         zULONG ulBlobLth;
         zULONG ulLth;

         uMax += 10;
         ulLth = sizeof( zLONG ) + 2 * sizeof( zSHORT ) + uMax * sizeof( zLONG );
         ulBlobLth = ulLth;
         zPCHAR pch = new char[ ulLth ];
         GetBlobFromAttribute( pch, &ulBlobLth, vMsgQ, szlTask, szlStatus );
         if ( ulLth != ulBlobLth + 10 * sizeof( zLONG ) )
            OperatorSend( 0, szlDrvr, "PushMsgQ_State", TRUE );
         else
            TraceLineI( "Message queue stack expanded to ", ulLth );

         SetAttributeFromBlob( vMsgQ, szlTask, szlStatus, pch, ulLth );
         GetAddrForAttribute( (zPCHAR *) &pStack, vMsgQ, szlTask, szlStatus );
         *((zPSHORT) (pStack + sizeof( zLONG ) + sizeof( zSHORT ))) = uMax;
         delete [] pch;
      }

      (*((zPSHORT) (pStack + sizeof( zLONG ))))++;    // increment EntryCnt
      *((zPLONG) pStack) = lState;                    // set CurrentState
      *((zPLONG) (pStack + sizeof( zLONG ) +          // set State[ n ]
                   2 * sizeof( zSHORT ) +
                   uCnt * sizeof( zLONG ))) = lState; // (not incrementing
                                                      //  uCnt since not
                                                      //  used hereafter)
   }
   else
   {
      TraceLineS( "PushMsgQ_State Lock Mutex ZDm: ", "NULL Status?" );
      DisplayObjectInstance( vMsgQ, 0, 0 );
      ::MessageBox( 0, "PushMsgQ_State NULL Status?", "ZDr Error", MB_OK );
   }

#ifdef zREMOTE_SERVER
// TraceLineS( "PushMsgQ_State Unlock Mutex ZDm", "" );
   SysMutexUnlock( vSubtask, "ZDm", 0 );
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: fnPopMsgQ_State
//
// PURPOSE: This routine pops the state of the message queue object
//
/////////////////////////////////////////////////////////////////////////////
zLONG
fnPopMsgQ_State( zVIEW vMsgQ )
{
   zPCHAR  pStack;    // blob containing Status Stack
   zLONG   lPrevState = 0;
   zUSHORT uCnt;

#ifdef zREMOTE_SERVER
   // Protect this with a semaphore!!!
   SysMutexLock( vSubtask, "ZDm", 0, 0 );
// TraceLineS( "PopMsgQ_State Lock Mutex ZDm", "" );
#endif

   GetAddrForAttribute( (zPCHAR *) &pStack, vMsgQ, szlTask, szlStatus );
   if ( pStack )
   {
      uCnt = *((zPSHORT) (pStack + sizeof( zLONG )));

      if ( uCnt > 0 )
      {
         uCnt--;
         *((zPSHORT) (pStack + sizeof( zLONG ))) = uCnt;
      }
      else
      {
         OperatorSend( 0, szlDrvr, "Cannot pop message queue", TRUE );
      }

      if ( uCnt )
      {
         lPrevState = *((zPLONG) (pStack + // sizeof( zLONG ) + the extra long is in uCnt
                         2 * sizeof( zSHORT ) + uCnt * sizeof( zLONG )));
      }
      else
         lPrevState = 0;

      *((zPLONG) pStack) = lPrevState;
   }
   else
   {
      TraceLineS( "PopMsgQ_State Lock Mutex ZDm: ", "NULL Status?" );
      DisplayObjectInstance( vMsgQ, 0, 0 );
      ::MessageBox( 0, "PushMsgQ_State NULL Status?", "ZDr Error", MB_OK );
   }

#ifdef zREMOTE_SERVER
// TraceLineS( "PopMsgQ_State Unlock Mutex ZDm", "" );
   SysMutexUnlock( vSubtask, "ZDm", 0 );
#endif

   return( lPrevState );
}

zOPER_EXPORT void OPERATION
AbortTask( zCPCHAR cpcMessage )
{
   ZDriverException de( cpcMessage );
   THROW( &de );
// AfxThrowFileException( CFileException::generic, -1,
//                        cpcMessage ); // throw an exception here
}

/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCurrentState
//                                              07/08/99 Modified: 07/08/99
//
//  PURPOSE:   To get the Dialog, Window, and Current Operation names for
//             the specified subtask.
//
//  PARAMETERS: vSubtask     - subtask view for the desired window
//              pchDlgTag    - return dialog tag
//              pchWndTag    - return window tag
//              pchOperation - name of the current operation (if any)
//
//  RETURNS:    -1  - Error locating window
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetCurrentState( zVIEW  vSubtask,
                 zPCHAR pchDlgTag,
                 zPCHAR pchWndTag,
                 zPCHAR pchOperation )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      strcpy_s( pchDlgTag, zTAG_LTH, *(pZSubtask->m_pzsDlgTag) );
      strcpy_s( pchWndTag, zTAG_LTH, *(pZSubtask->m_pzsWndTag) );
      if ( pZSubtask->m_cpcCurrOper )
         strcpy_s( pchOperation, zTAG_LTH, pZSubtask->m_cpcCurrOper );
      else
         pchOperation[ 0 ] = 0;

      return( 0 );
   }

   pchDlgTag[ 0 ] = 0;
   pchWndTag[ 0 ] = 0;
   pchOperation[ 0 ] = 0;
   return( -1 );
}
