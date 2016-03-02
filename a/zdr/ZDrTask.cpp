/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrtask.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of class ZTask.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//

// 2002.05.15    HH     WEB
//    Fix bug in fnTransferMessages (incompatible Domains Attribute "Beep").
//
// 2001.12.18    HH     WEB
//    WriteOI_ToXML without zXML_INCREFLAGS.
//
// 2001.10.16    DKS    WEB
//    Handle System Close and ESC events that have no associated action.
//
// 2001.09.24    DKS    Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.12    DKS    Helmut
//    Fix for terminating application when replacing window with window.
//
// 2001.08.08    DKS    R55618
//    Fix for Replace window with same window.
//
// 2001.08.03    DKS    WEB
//    Set "Active" window attribute on NewZWnd.
//
// 2001.06.15    DKS    WEB
//    Fix for Replace window with same window.
//
// 2001.05.16    DKS    WEB
//    Handle system close event with no action.
//
// 2001.05.15    BL     WEB
//    Bugfix for Multi Selection in ListBox
//
// 2001.04.15    TMV    Z10/WEB
//    Add some code to trace xml into a file if the system is built in debug
//    mode in the zeidon.ini.  The entry XMLTraceFile=SomeFileName.SomeExt
//    must be defined.
//    Added zXML_INCREFLAGS to call of WriteMultipleOIs_ToXML to force XML
//    writing to write empty tags if attribute is updated and NULL.
//
// 2001.05.03    DKS    WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.26    DKS    WEB
//    Fixed fnSend memory overwrite occurring for large buffers (pchText).
//
// 2001.04.19    DKS    Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2001.03.09    DKS    Z10/WEB
//    Replace window code did not account for multiple sequential replace
//    requests.  Also shut down when the global task list is empty.
//
// 2001.02.07    DKS    WEB
//    Set LastCommandTag for WEB.
//
// 2000.05.24    DKS    RemoteServer
//    Create unique MsgBox tag.
//
// 2000.04.11    US     all    Bootstrap 'parser' enhanced
//    replaced a part of the bootstrap parser with a little bit more
//    sophisticated one that can handle different tag orders.
//    Note: This is definitely no real parser, but I didn't know how to
//    name it different. It's still a very plain handling of the first
//    XML string.
//
// 2000.03.17    US     all    communication protocol bug removed
//    Whoever wrote this should decide whether to use a 0 (zero) or an O
//    (upper 'O') for signaling the last message block. Anyway, changed
//    the "== '0'" to "!= 'C'" to get better robustness.
//    By the way, if you have a layer for implementing a kind of protocol
//    (here fnRead() and fnSend()) you should use this layer rather than
//    reading directly from the socket (in fnZDrConnectionThreadProc()).
//    We should have noted that we broke the protocol in order to bootstrap
//    in fnProcessConnectionThread ... DKS.
//
// 2000.03.01    DKS    Z10    RemoteServer
//    Multi-select ListBox implementation for RemoteServer
//
// 1999.09.22    DKS    Z10    QS999
//    Started memory leak analyis.
//
// 1999.09.15    DKS    Z10    Enhance
//    Added mutex protection of global subtask list.
//
// 1999.07.08    DKS    Z10    Enhance
//    Keep track of the list of subtasks in a view available to an
//    application (requested by FH).
//
// 1999.07.06    DKS    Z10    Hartmut?
//    Fixed Interp/Debugger problem when attempting to replace the
//    main window with a frame that had not yet been created.
//
// 1999.07.02    DKS
//    Keep track of whether we are in NT or not.
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef zREMOTE_SERVER

static zLONG lXMLTraceCount = 0;

void
fnGetXML_Value( zPCHAR pchTgt, zPCHAR pchSrc, zPCHAR pchEndTag );

ZSocketTask::ZSocketTask( SOCKET sock, ZTask *pZTask )
{
   m_pCacheWndList = new ZXRA_List( this );
   m_sock = sock;
   m_pZTask = pZTask;
   m_pZSubtaskTop = 0;
   m_vRegisteredSubtask = 0;
   m_vCreatedSubtask = 0;
   m_vSubtask = 0;
   m_vXRA = 0;
   m_vXRA_State = 0;
   m_lSyncIdx = 0;
   m_szAppTag[ 0 ] = 0;
   m_szRemoteWndTag[ 0 ] = 0;
   m_szActTag[ 0 ] = 0;
   m_lSocketState = 0;
   m_chActType = 0;            // WndEvent, Ctrl, Opt, Hotkey (W,C,O,H)
   m_pWinThread = 0;
   m_lSysMutexLockCnt = 0;
   m_pZDxFrame = (ZDxFrame *) AfxGetMainWnd( );
   m_pzsAppName = 0;

#ifdef DEBUG_ALL
   zCHAR szMsg[ 128 ];

   sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] ZSocketTask::ctor: pCacheWndList: ",
              this, AfxGetThread( ), m_pWinThread, m_sock );
   TraceLineI( szMsg, (zLONG) m_pCacheWndList );
#endif

// m_pZDxFrame->SendMessage( zZEIDON_REMOTE_SOCKET, 0, (LPARAM) this );
   SendZDxFrameMessage( 0 );
}

ZSocketTask::~ZSocketTask( )
{
#ifdef DEBUG_ALL
   zCHAR szMsg[ 128 ];

   sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] ZSocketTask::dtor: pCacheWndList: %ld SysMutexLockCnt: ",
              this, AfxGetThread( ), m_pWinThread, m_sock, (zLONG) m_pCacheWndList );
   TraceLineI( szMsg, m_lSysMutexLockCnt );
#endif

   if ( m_lSysMutexLockCnt )
   {
      MessageSend( m_vSubtask, 0, "ZSocketTask::dtor", "Mutex leak detected", zMSGQ_MODAL_ERROR, FALSE );
   // while ( m_lSysMutexLockCnt )
   //    SysLockMutex( ?, FALSE, "MutexLeak" );
   }

   // Remove pZSocketTask from the global list here!!!
   SysMutexLock( vSubtask, "ZDs", 0, 0 );
   zLONG lPos = (zLONG) g_pSocketList[ 0 ].pZSocketTask;
   while ( lPos > 0 )
   {
      if ( g_pSocketList[ lPos ].pZSocketTask == this )
      {
         g_pSocketList[ lPos ].pZSocketTask = 0;
         g_pSocketList[ lPos ].ulSocketState = -1;
         break;
      }

      lPos--;
   }

   SysMutexUnlock( vSubtask, "ZDs", 0 );

// ZDxFrame *pFrame = (ZDxFrame *) AfxGetMainWnd( );
// pFrame->SendMessage( zZEIDON_REMOTE_SOCKET, -1, (LPARAM) this );
   SendZDxFrameMessage( -1 );

   mDeleteInit( m_pzsAppName );
   mDeleteInit( m_pCacheWndList );
   closesocket( m_sock );
}

LRESULT
ZSocketTask::SendZDxFrameMessage( WPARAM wParam )
{
   LRESULT lResult = 0;

   if ( g_pSocketList )
   {
      zLONG lPos = (zLONG) g_pSocketList[ 0 ].pZSocketTask;
      while ( lPos > 0 )
      {
         if ( g_pSocketList[ lPos ].pZSocketTask == this )
         {
            g_pSocketList[ lPos ].ulSocketState = wParam;
            break;
         }

         lPos--;
      }

      if ( m_pZDxFrame == 0 || mIs_hWnd( m_pZDxFrame->m_hWnd ) == FALSE ||
           ::SendMessageTimeout( m_pZDxFrame->m_hWnd, zZEIDON_REMOTE_SOCKET, lPos, (LPARAM) g_pSocketList,
                                 SMTO_ABORTIFHUNG | SMTO_BLOCK, 10000, (zPULONG) &lResult ) == 0 )
      {
         TraceLineS( "ZSocketTask::SendZDxFrameMessage: ", "SendMessageTimeout" );
      }
   }

   return( lResult );
}

void
ZSocketTask::LockMutex( zBOOL bLock, zCPCHAR cpcText )
{
#ifdef DEBUG_ALL
   zCHAR szMsg[ 128 ];

   sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] %sMutex: %s LockCnt = ",
              this, AfxGetThread( ), m_pWinThread, m_sock, bLock ? "Lock" : "Unlock", cpcText );
#endif

   if ( bLock )
      m_lSysMutexLockCnt = SysMutexLock( vSubtask, "ZDx", 0, 0 );
   else
      m_lSysMutexLockCnt = SysMutexUnlock( vSubtask, "ZDx", 0 );

#ifdef DEBUG_ALL
   TraceLineI( szMsg, m_lSysMutexLockCnt );
#endif
}

zSHORT
ZSocketTask::ApplyState( zBOOL bLocateActionOnly )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWENTITY lpParent;
   LPVIEWATTRIB lpViewAttrib;
   zCHAR     szValue[ 65534 ];
   zCHAR     szEntityName[ zTAG_LTH ];
   zCHAR     szRealRemoteWndTag[ 66 ];
   zCHAR     szHoldRemoteWndTag[ 66 ];
   zCHAR     szTag[ 66 ];       // the Remote Wnd Tag is a concatenated tag
   zUSHORT   uHierLevel = 1;
   ZXRA_Item *pXRA = 0;
   zBOOL     bXRA_Delta = FALSE;
   zBOOL     bResetSelected = FALSE;
   zBOOL     bWnd = FALSE;
   zVIEW     vXRA;
   zSHORT    nRC;

   szRealRemoteWndTag[ 0 ] = 0;
   strcpy_s( szHoldRemoteWndTag, zsizeof( szHoldRemoteWndTag ), m_szRemoteWndTag );
   m_lSyncIdx = 0;
   m_szAppTag[ 0 ] = 0;
   m_szRemoteWndTag[ 0 ] = 0;
   m_szActTag[ 0 ] = 0;
   m_chActType = 0;            // WndEvent, Ctrl, Opt, Hotkey (W,C,O,H)

   if ( m_vXRA_State == 0 )
      return( -1 );

   GetStringFromAttribute( m_szAppTag, m_vXRA_State, "App", "Tag" );

   nRC = CreateViewFromViewForTask( &vXRA, m_vXRA_State, m_vSubtask );
   SetNameForView( m_vXRA_State, "__XRA_State", 0, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vXRA ) == 0 )
   {
   }

#ifdef DEBUG_ALL
// ::MessageBox( 0, "DKS", "ApplyState", MB_OK );
   TraceLineS( "Entering ApplyState for Application: ", m_szAppTag );
   DisplayObjectInstance( vXRA, 0, 0 );
   zCHAR szMsg[ 256 ];
#endif

#if 0
   if ( SetCursorFirstEntityByString( vXRA, "Wnd", "Active", "Y", 0 ) == zCURSOR_SET )
   {
      GetStringFromAttribute( szRealRemoteWndTag, zsizeof( szRealRemoteWndTag ), vXRA, "Wnd", "Tag" );
   }
   else
   {
      MessageBox( 0, "Error ... No Active Window specified", "RemoteServer ApplyState", MB_OK );
      return;
   }
#endif

   ZCorrelatedList cl;  // correlated list of Entity Tag/HierLevel
   ZCorrelatedItem *pItem;
   zLONG k, j;

   SetCursorFirstEntity( vXRA, "Wnd", 0 );
   strcpy_s( szEntityName, zsizeof( szEntityName ), "App" );
   DefineHierarchicalCursor( vXRA, "App" );
   while ( nRC != zCURSOR_UNCHANGED && nRC != zCALL_ERROR )
   {
   // DisplayEntityInstance( vXRA, szEntityName );

      // Get the ViewEntity for the current entity (for attribute processing).
      lpViewEntity = MiGetViewEntityForView( vXRA, szEntityName );

      // Delete items at and beyond the current level.
      k = uHierLevel;
      while ( (pItem = cl.Find( k )) != 0 )
      {
         j = k;
         if ( pItem->m_pNextItem )
            k = pItem->m_pNextItem->m_lNumber;
         else
            k++;

         pItem = cl.Remove( j );
         pItem->m_pNextItem = 0; // prevent dtor from cascading
         delete( pItem );
      }

      if ( zstrcmp( szEntityName, "Wnd" ) == 0 )
      {
         // There is no previous tag (up a level).
         szTag[ 0 ] = 0;
         bWnd = TRUE;
      }
      else
      {
         // Locate and set the previous tag (up a level).
         for ( k = uHierLevel - 1; k > 0; k-- )
         {
            pItem = cl.Find( k );
            if ( pItem )
            {
               strcpy_s( szTag, zsizeof( szTag ), pItem->m_csValue );
               break;
            }
         }
      }

      lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstOD_Attrib );

      if ( m_lSyncIdx == 0 && zstrcmp( szEntityName, "Wnd" ) == 0 )
      {
         if ( pXRA && bXRA_Delta )
         {
            pXRA->m_vXRA_Delta = 0;
            bXRA_Delta = FALSE;
         }

         GetStringFromAttribute( m_szRemoteWndTag, vXRA, szEntityName, "Tag" );
         pXRA = m_pCacheWndList->Find( m_szRemoteWndTag, 0, "ZSocketTask::ApplyState" );
         if ( pXRA )
         {
            if ( pXRA->m_vXRA_Delta == 0 )
            {
               pXRA->m_vXRA_Delta = pXRA->m_vXRA;
               bXRA_Delta = TRUE;
            }
         }
      }

      // Run through the list of attributes.
      while ( lpViewAttrib )
      {
         if ( zstrcmp( lpViewAttrib->szName, "Tag" ) == 0 )
         {
            GetStringFromAttribute( szTag, zsizeof( szTag ), vXRA, szEntityName, "Tag" );
            if ( zstrcmp( szEntityName, "Ctrl" ) == 0 )
               bResetSelected = FALSE;

            cl.Add( uHierLevel, szTag );  // add to Tag/HierLevel list
         }
         else
         if ( szTag[ 0 ] && lpViewAttrib->bHidden == FALSE &&
              AttributeUpdated( vXRA, szEntityName, lpViewAttrib->szName ) )
         {
#ifdef DEBUG_ALL
            sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "Entity: %s Attribute: %s updated", szEntityName, lpViewAttrib->szName );
            TraceLineS( szMsg, "" );
#endif
            if ( zstrcmp( lpViewAttrib->szName, "SyncIdx" ) == 0 )
            {
               strcpy_s( szRealRemoteWndTag, zsizeof( szRealRemoteWndTag ), m_szRemoteWndTag );
               GetIntegerFromAttribute( &m_lSyncIdx, vXRA, szEntityName, "SyncIdx" );
               m_chActType = szEntityName[ 0 ];

               switch ( m_chActType )
               {
                  ZSubtask *pZSubtask;

                  case 'C':
                  {
                     ZMapAct *pzma;

                     if ( GetWindowAndCtrl( &pZSubtask, &pzma, pXRA->m_pZSubtask->m_vDialog, szTag ) == 0 )
                     {
                        pZSubtask->m_pzsLastTag = pzma->m_pzsTag;
                        pZSubtask->m_pzsLastCtrlTag = pzma->m_pzsTag;
                     }

                     break;
                  }

                  case 'O':
                  {
                     ZOption *pOpt;

                     if ( GetWindowAndOption( &pZSubtask, &pOpt, pXRA->m_pZSubtask->m_vDialog, szTag ) == 0 )
                     {
                        pZSubtask->m_pzsLastTag = pOpt->m_pzsTag;
                     }

                     break;
                  }

                  case 'H':
                  {
                     break;
                  }

                  case 'W':
                  {
                     if ( m_lSyncIdx == -1 )
                        m_chActType = ' ';
                  }

                  default:
                     break;
               }
            }
            else
            if ( zstrcmp( lpViewAttrib->szName, "SelectedAction" ) == 0 )
            {
               GetStringFromAttribute( m_szActTag, vXRA, szEntityName, "SelectedAction" );
            }
            else
            if ( zstrcmp( lpViewAttrib->szName, "SelectedIndex" ) == 0 )
            {
               // Selecting an item within a list (ListBox, ComboBox, ...)
               if ( pXRA && pXRA->m_pZSubtask )
               {
                  zLONG lSelectedIndex;

                  GetIntegerFromAttribute( &lSelectedIndex, vXRA, szEntityName, "SelectedIndex" );
                  ZSubtask *pZSubtask;
                  ZMapAct  *pzma;

                  if ( GetWindowAndCtrl( &pZSubtask, &pzma, pXRA->m_pZSubtask->m_vDialog, szTag ) == 0 )
                  {
                     if ( bResetSelected == FALSE )
                     {
                        pzma->SetZCtrlState( zCONTROL_STATUS_SELECT_INDEX, -1 );
                        bResetSelected = TRUE;
                     }

                  // TraceLineI( "ZSocketTask::ApplyState SelectedIndex: ", lSelectedIndex );
                  // TraceLineS( "                             for Ctrl: ", *(pzma->m_pzsTag) );
                     SetNameForView( vXRA, "__XRA_Select", 0, zLEVEL_TASK );
                     pzma->SetZCtrlState( zCONTROL_STATUS_SELECT_INDEX, lSelectedIndex );
                  }
               }
            }
            else
            if ( pXRA && pXRA->m_pZSubtask && zstrcmp( szEntityName, "Wnd" ) == 0 && zstrcmp( lpViewAttrib->szName, "CtrlLastFocusTag" ) == 0 )
            {
               ZSubtask *pZSubtask;
               ZMapAct  *pzma;

               GetStringFromAttribute( szValue, zsizeof( szValue ), vXRA, "Wnd", "CtrlLastFocusTag" );
               if ( szValue[ 0 ] && GetWindowAndCtrl( &pZSubtask, &pzma, pXRA->m_pZSubtask->m_vDialog, szValue ) == 0 )
               {
                  pXRA->m_pZSubtask->m_pzmaWithFocus = pzma;
               }
            }
            else
            if ( pXRA && pXRA->m_pZSubtask && bLocateActionOnly == FALSE )
            {
               GetStringFromAttribute( szValue, zsizeof( szValue ), vXRA, szEntityName, lpViewAttrib->szName );
               if ( zstrcmp( szEntityName, "App" ) == 0 )
               {
               }
               else
               if ( zstrcmp( szEntityName, "Wnd" ) == 0 )
               {
                  SetRemoteWndAttribute( pXRA->m_pZSubtask->m_vDialog, lpViewAttrib->szName, szValue );
               }
               else
               {
                  SetRemoteCtrlAttribute( pXRA->m_pZSubtask->m_vDialog, szTag, szEntityName, lpViewAttrib->szName, szValue );
               }
            }
         }
      // else
      // {
      //    sprintf( szMsg, "Entity: %s  Attribute: %s  NOT updated", szEntityName, lpViewAttrib->szName );
      //    TraceLineS( szMsg, "" );
      // }

         lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextOD_Attrib );
      }

      nRC = SetCursorNextEntityHierarchical( &uHierLevel, szEntityName, vXRA );
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      {
         // Find the "real" entity name (e.g.  Ctrl instead of CtrlCtrl).
#if 0
         nRC = SetViewToSubobject( vXRA, szEntityName );
         GetRealEntityName( vXRA, szEntityName );
#else
         nRC = SetViewToSubobject( vXRA, szEntityName );
         lpViewEntity = (LPVIEWENTITY) MiGetViewEntityForView( vXRA, szEntityName );
         for ( lpParent = (LPVIEWENTITY) zGETPTR( lpViewEntity->hParent );
               lpParent->bRecursivePar == 0;
               lpParent = (LPVIEWENTITY) zGETPTR( lpParent->hParent ) )
         {
            // nothing to do
         }

         strcpy_s( szEntityName, zsizeof( szEntityName ), lpParent->szName );
#endif
      }
   }

   if ( szRealRemoteWndTag[ 0 ] )
      strcpy_s( m_szRemoteWndTag, zsizeof( m_szRemoteWndTag ), szRealRemoteWndTag );
   else
   if ( bWnd == FALSE )
      strcpy_s( m_szRemoteWndTag, zsizeof( m_szRemoteWndTag ), szHoldRemoteWndTag );

#ifdef DEBUG_ALL
   sprintf_s( szMsg, zsizeof( szMsg ), "ApplyState App: %s Wnd: %s Action: %s ActType: %c SyncIdx: %d",
              m_szAppTag, m_szRemoteWndTag, m_szActTag, m_chActType, m_lSyncIdx );
   TraceLineS( szMsg, "" );

// if ( m_lSyncIdx < 0 )
//    MessageBox( 0, "Negative SyncIdx", "???", MB_OK );
#endif

   if ( pXRA )
   {
   // TraceLineS( "After ApplyState ... ", "DisplayObjectInstance m_vXRA_Delta" );
   // DisplayObjectInstance( pXRA->m_vXRA_Delta, 0, 0 );

      if ( bXRA_Delta )
      {
         pXRA->m_vXRA_Delta = 0;
         bXRA_Delta = FALSE;
      }

      nRC = 0;
   }
   else
   {
      TraceLineS( "ApplyState unable to locate appropriate Delta: ", m_szRemoteWndTag );
      DisplayObjectInstance( m_vXRA_State, 0, 0 );
      m_pCacheWndList->ListViews( );
      pXRA = m_pCacheWndList->Find( m_szRemoteWndTag, 0, "ZSocketTask::ApplyState" );
      if ( pXRA == 0 )
      {
         TraceLineS( "ApplyState remains unable to locate appropriate Delta: ", m_szRemoteWndTag );
      }

      nRC = -1;
   }

   DropHierarchicalCursor( vXRA );
   DropView( vXRA );
// TraceLineS( "Exiting ApplyState for Application: ", m_szAppTag );
   return( nRC );
}

void
fnTransferMessages( zVIEW vXRA, zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// We need to keep track of whether or not a local thread is its own
// Zeidon task or if it's part of a parent process.  To do this we use
// global variables that are different for each thread.

UINT
fnZDrConnectionThreadProc( zPVOID p )
{
   ZSocketTask *pZSocketTask = (ZSocketTask *) p;
   LPTASK      lpTask;
   zCHAR       szMsg[ zWEB_COMMUNICATION_BUFFER_SIZE + 256 ];
   zSHORT      nRC = zCALL_ERROR;
   zLONG       lBlockOff = FALSE;
   zCHAR       szBuffer[ zWEB_COMMUNICATION_BUFFER_SIZE + 2 ];
   zCHAR       szAppName[ zZEIDON_NAME_LTH + 5 ];  // room for "\\"
   zCHAR       szDlgName[ zZEIDON_NAME_LTH * 2 + 1 ];
   zPCHAR      pch;
// zLONG       lLth;
   zLONG       lBytes;
// ZDxFrame    *pFrame = (ZDxFrame *) AfxGetMainWnd( );
   ZSubtask    *pZSubtask = 0;
   zPCHAR      pchTag;
   zPCHAR      pchWnd;
   zPCHAR      pchApp;
   zPCHAR      pchEnd;
   zLONG       lItemCnt;
   zULONG      ulSubtaskState;
   zBOOL       bRegistered = FALSE;

   pZSocketTask->m_pWinThread = AfxGetThread( );

   zVIEW vZDrSystem;

   if ( GetViewByName( &vZDrSystem, "__ZDrSystem", 0, zLEVEL_SYSTEM ) > 0 )
   {
      CreateViewFromViewForTask( &vZDrSystem, vZDrSystem, m_vSubtask );
      CreateEntity( vZDrSystem, "Task", zPOS_FIRST );
      SetAttributeFromInteger( vZDrSystem, "Task", "Handle", pZSocketTask->m_pWinThread->m_nThreadID );
      DropView( vZDrSystem );
   }

#ifdef DEBUG_ALL
   TraceLineI( "Starting Thread: ", (zLONG) pZSocketTask->m_pWinThread );

   // Protect this with a semaphore!!!
   SysMutexLock( vSubtask, "ZDr", 0, 0 );

   lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   TraceLineI( "fnZDrConnectionThreadProc startup: ", lItemCnt );
   while ( lItemCnt > 0 )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
      {
         sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ),
                    " [S%ld.T%ld.t%ld.s%ld] Dlg: %s Wnd: %s vSubtask: %#010hx   Subtask: ",
                    g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZSocketTask, AfxGetThread( ),
                    g_pSubtaskList[ lItemCnt ].lTID,
                    g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZSocketTask->m_sock,
                    g_pSubtaskList[ lItemCnt ].DlgN,
                    g_pSubtaskList[ lItemCnt ].WndN,
                    g_pSubtaskList[ lItemCnt ].pvDialog );
         TraceLineI( szMsg, (zLONG) g_pSubtaskList[ lItemCnt ].pZSubtask );
      }

      lItemCnt--;
   }

#endif

   SysMutexUnlock( vSubtask, "ZDr", 0 );

   // We broke the protocol in order to bootstrap ... DKS.
   //
   // Get length of application name.
   lBytes = recv( pZSocketTask->m_sock, szBuffer, zsizeof( szBuffer ), 0 );
#ifdef DEBUG_ALL
   TraceLineI( "fnZDrConnectionThreadProc Initialization buffer size: ", lBytes );
#endif
   if ( lBytes == SOCKET_ERROR || lBytes == 0 || (pch = zstrstr( szBuffer, "<SelectedAction>" )) == 0 || zstrstr( pch, ">StartApplication<" ) == 0 )
   {
      TraceLineS( "fnZDrConnectionThreadProc ", "No more bytes" );
      goto InitializationError_label;
   }
   else
   if  (lBytes == zsizeof( szBuffer ) )
   {
      TraceLineS( "fnZDrConnectionThreadProc ", "Initialization buffer overflow" );
   }

   szAppName[ 0 ] = 0;
   szDlgName[ 0 ] = 0;
   g_lDlgUnitCharX = 0;
   g_lDlgUnitCharY = 0;
   g_lDblClkX = 0;
   g_lDblClkY = 0;
   pchTag = zstrstr( szBuffer, "<Factor>" );
   if ( pchTag )
   {
      pchEnd = pchTag + 8;
      pch = zstrchr( pchEnd, '<' );
      pchTag = zstrchr( pchEnd, 'x' );
      if ( pchTag == 0 || pchTag > pch )
         pchTag = pchEnd;
      else
         pchTag++;

      g_lDlgUnitCharX = zatol( pchTag );
      pchTag = zstrchr( pchTag, 'y' );
      if ( pchTag == 0 || pchTag > pch )
         g_lDlgUnitCharY = g_lDlgUnitCharX;
      else
         g_lDlgUnitCharY = zatol( pchTag + 1 );
   }

   if ( g_lDlgUnitCharX == 0 )
      g_lDlgUnitCharX = 5;

   if ( g_lDlgUnitCharY == 0 )
      g_lDlgUnitCharY = 5;

   while ( TRUE )   // purist's goto ... break when an error occurs
   {
      pchApp = zstrstr( szBuffer, "<App>" );
      if ( pchApp == NULL )
          break;

      pchWnd = zstrstr( pchApp, "<Wnd>" );
      pchTag = zstrstr( pchApp, "<Tag>" );
      if ( (pchTag == NULL) || (pchWnd == NULL) )
          break;

      if ( pchTag < pchWnd )
      {
         // The <Wnd> tag comes after the <Tag> tag of <App>.
         szAppName[ 0 ] = '~';
         szAppName[ 1 ] = '~';
         fnGetXML_Value( &szAppName[ 2 ], pchTag, "</Tag>" );

         // Now get the <Tag> value from the <Wnd> tag.
         pchTag = zstrstr( pchWnd, "<Tag>" );
         if ( pchTag == NULL )
            break;

         fnGetXML_Value( szDlgName, pchTag, "</Tag>" );
      }
      else
      {
         fnGetXML_Value( szDlgName, pchTag, "</Tag>" );
         pchEnd = zstrstr( pchWnd, "</Wnd>" );
         if ( pchEnd == NULL )
            break;

         pchTag = zstrstr( pchEnd, "<Tag>" );
         if ( pchTag == NULL )
            break;

         szAppName[ 0 ] = '~';
         szAppName[ 1 ] = '~';
         fnGetXML_Value( &szAppName[ 2 ], pchTag, "</Tag>" );
      }

      break; // end the loop - it's just a dummy for easy error handling
   }

   if ( szAppName[ 0 ] == 0 || szDlgName[ 0 ] == 0 )
   {
      TraceLineS( "Invalid initialization XML for Remote Server", "" );
      goto InitializationError_label;
   }

   pchEnd = zstrchr( szDlgName, '.' );
   if ( pchEnd )
      *pchEnd = 0;   // A '.' is the end of the DialogName

   if ( pZSocketTask->m_pZTask->m_nTraceAction )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "Received a connection ==> AppName '%s' DialogName '%s'", szAppName + 2, szDlgName );
      TraceLineS( szMsg, "" );
   }

   RegisterZeidonAppForThread( &(pZSocketTask->m_vRegisteredSubtask), "~~Zeidon System", 0, 0, 0 );
   bRegistered = TRUE;
   lpTask = SfGetCurrentTask();
   lpTask->bServer = TRUE;

   SfCreateSubtask( &(pZSocketTask->m_vSubtask), 0, szAppName + 2 );
   strcpy_s( pZSocketTask->m_szAppTag, zsizeof( pZSocketTask->m_szAppTag ), szAppName + 2 );
   pZSocketTask->m_pzsAppName = new CString( szAppName + 2 );
   strcpy_s( pZSocketTask->m_pZTask->m_szInitialDlgName, zsizeof( pZSocketTask->m_pZTask->m_szInitialDlgName ), szDlgName );

#ifdef DEBUG_ALL
   TraceLineS( "fnZDrConnectionThreadProc Calling init ", "Prev" );
#endif
   pZSubtask = pZSocketTask->m_pZTask->NewZWnd( 0, 0, pZSocketTask->m_vSubtask, 0, FALSE, szDlgName, "", 0, pZSocketTask );
#ifdef DEBUG_ALL
   TraceLineS( "fnZDrConnectionThreadProc Calling init ", "Post" );
#endif
   if ( pZSubtask == 0 )
   {
      ConnectionRecord  cr;
      zVIEW vXRA;

      // Activate using SfActivateSysEmptyOI so that the kzxra.lod
      // does not have to be in the application LPLR.
#ifdef DEBUG_ALL
      TraceLineS( "fnZDrConnectionThreadProc Calling init ", "NULL Subtask" );
#endif
      SfActivateSysEmptyOI( &vXRA, "KZXRA", pZSocketTask->m_vSubtask, zSINGLE );
      CreateEntity( vXRA, "App", zPOS_AFTER );
      SetAttributeFromString( vXRA, "App", "Tag", szAppName + 2 );
      CreateEntity( vXRA, "Wnd", zPOS_AFTER );
      SetAttributeFromString( vXRA, "Wnd", "Tag", szDlgName );
      fnTransferMessages( vXRA, pZSocketTask->m_vSubtask );

      zmemset( &cr, 0, zsizeof( ConnectionRecord ) );
      cr.pszBuffer = szBuffer;
      cr.sock = pZSocketTask->m_sock;
      cr.lFileHandle = -1;
      cr.cMore = 0;

#ifdef _DEBUG
      CFile *f = NULL;
      zCHAR szTempDir[ MAX_PATH ];
      zCHAR szXRAFile[ MAX_PATH ];
      zCHAR szExtension[ MAX_PATH ];
      int nExtensionPos;

      CString strFileName;

      zmemset( szTempDir, 0, MAX_PATH );
      zmemset( szXRAFile, 0, MAX_PATH );
      zmemset( szExtension, 0, MAX_PATH );

      SysReadZeidonIni( -1, "[Workstation]", "XMLTraceFile", szXRAFile );

      if ( szXRAFile[ 0 ] )
      {
         strFileName = szXRAFile;
         nExtensionPos = strFileName.ReverseFind( '.' );
         if ( nExtensionPos > -1 )
         {
            strcpy_s( szExtension, zsizeof( szExtension ), ((LPCSTR( strFileName )) + nExtensionPos ) );
            szXRAFile[ nExtensionPos ] = 0;
         }
         else
         {
            strcpy_s( szExtension, zsizeof( szExtension ), ".XML" );
         }
         if ( ::GetEnvironmentVariable( "TEMP", szTempDir, MAX_PATH ) == 0)
         {
            strFileName.Format( "%s%s%d%s","c:\\", szXRAFile, lXMLTraceCount, szExtension );
         }
         else
         {
            strFileName.Format( "%s%s%s%d%s",szTempDir, "\\", szXRAFile, lXMLTraceCount, szExtension );
         }

         f = new CFile( strFileName, CFile::modeCreate | CFile::modeWrite );
         lXMLTraceCount++;
         cr.lFileHandle = f->m_hFile;
      }
#endif

      nRC = WriteOI_ToXML( vXRA, zXML_ZEIDONINFO, // | zXML_INCREFLAGS,
                           fnSend, (zPVOID) &cr );
      DropView( vXRA );
      if ( nRC >= 0 )
         fnSend( 0, 0, 0, 0, (zPVOID) &cr );

#ifdef _DEBUG
      mDeleteInit( f )
      cr.lFileHandle = -1;
#endif
   }
   else
      nRC = 0;

#ifdef DEBUG_ALL
   TraceLineS( "fnZDrConnectionThreadProc Calling init ", "Msg" );
#endif
// pFrame->SendMessage( zZEIDON_REMOTE_SOCKET, 1, (LPARAM) pZSocketTask );
   pZSocketTask->SendZDxFrameMessage( 1 );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG_ALL
   TraceLineS( "fnZDrConnectionThreadProc Calling init ", "try" );
#endif

   try
   {  // begin of try block

   ZXRA_Item *pXRA = 0;
   while ( pZSubtask )
   {
#ifdef DEBUG_ALL
      TraceLineX( "fnZDrConnectionThreadProc SendReceiveXML: ", (zLONG) pZSubtask );
#endif
      if ( (nRC = fnSendReceiveXML( pZSocketTask, szBuffer )) < 0 )
         break;

      pZSocketTask->m_pCacheWndList->DeleteEmpty( );

      // Protect this with a semaphore!!!
      pZSocketTask->LockMutex( pZSocketTask->m_pZTask->m_vSubtask, TRUE, "SendReceive" );

      SetAttributeFromString( pZSocketTask->m_vXRA_State, "App", "Tag", pZSocketTask->m_szAppTag );

      if ( pZSocketTask->m_vXRA_State )
      {
      // TraceLineS( "DisplayObjectInstance m_vXRA_State ", "..." );
      // DisplayObjectInstance( pZSocketTask->m_vXRA_State, 0, 0 );
         GetStringFromAttribute( szMsg, zsizeof( szMsg ), pZSocketTask->m_vXRA_State, "App", "SelectedAction" );
      }
      else
         TraceLineS( "Did NOT receive m_vXRA_State ", "..." );

      if ( pZSocketTask->m_vXRA_State == 0 || zstrcmp( szMsg, "StopApplication" ) == 0 )
      {
         pZSocketTask->LockMutex( pZSocketTask->m_pZTask->m_vSubtask, FALSE, "SendReceive" );
         break;
      }

   // pZSocketTask->m_pCacheWndList->ResetSubtasks( );
      pZSocketTask->ApplyState( FALSE );
      mDropOI_Init( pZSocketTask->m_vXRA_State );

      pXRA = pZSocketTask->m_pCacheWndList->Find( pZSocketTask->m_szRemoteWndTag, 0, "fnZDrConnectionThreadProc" );
      if ( pXRA == 0 )
      {
         TraceLineS( "RemoteServer error condition ... cannot find: ", pZSocketTask->m_szRemoteWndTag );
         pZSocketTask->LockMutex( pZSocketTask->m_pZTask->m_vSubtask, FALSE, "SendReceive" );
         break;
      }

      pXRA->ActivateEmptyDelta( pZSocketTask->m_vRegisteredSubtask, pZSocketTask->m_szAppTag );

      switch ( pZSocketTask->m_chActType )
      {
         case 'C':
            strcpy_s( szMsg, zsizeof( szMsg ), "CtrlEvent" );
            break;

         case 'W':
            strcpy_s( szMsg, zsizeof( szMsg ), "WndEvent" );
            break;

         case 'O':
            strcpy_s( szMsg, zsizeof( szMsg ), "Option" );
            break;

         case 'H':
            strcpy_s( szMsg, zsizeof( szMsg ), "Hotkey" );
            break;

         default:
            strcpy_s( szMsg, zsizeof( szMsg ), "Unknown" );
            break;
      }

      if ( pZSocketTask->m_lSyncIdx > 0 )
      {
         pZSocketTask->m_lSyncIdx--;
         pZSocketTask->LockMutex( pZSocketTask->m_pZTask->m_vSubtask, FALSE, "SendReceive" );

         if ( pXRA && pXRA->m_pZSubtask )
         {
            ulSubtaskState = ZSubtask::GetSubtaskState( pXRA->m_pZSubtask );
            if ( (ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
            {
               SetEntityCursor( pXRA->m_pZSubtask->m_vDialog, szlAct, szlSyncIdx,
                                zPOS_FIRST | zQUAL_INTEGER, &(pZSocketTask->m_lSyncIdx), 0, 0, 0, 0, 0 );
               GetAddrForAttribute( &pch, pXRA->m_pZSubtask->m_vDialog, szlAct, szlTag );
               ProcessActionLoop( pXRA->m_pZSubtask, pch, 0, 0, 0, FALSE, szMsg );
            }

         // TraceLineS( "Remove1??? ===> ", pZSocketTask->m_szRemoteWndTag );
         // pZSocketTask->m_pCacheWndList->Remove( pXRA );
         }
      }
      else
      {
         if ( pXRA == 0 || pXRA->m_pZSubtask == 0 )
         {
            pXRA = pZSocketTask->m_pCacheWndList->Find( pZSocketTask->m_szRemoteWndTag, 0, "fnZDrConnectionThreadProc2" );
         }

         if ( pXRA && pXRA->m_pZSubtask )
         {
            ZMapAct  *pzma;

            switch ( pZSocketTask->m_chActType )
            {
               case 'W':
               {
                  zLONG lEvent = 0;
                  pch = zstrrchr( pZSocketTask->m_szActTag, ';' );
                  if ( pch )
                  {
                     *pch = 0;
                     pch++;
                     lEvent = zatol( pch );
                  }

                  pXRA->m_pZSubtask->HandleEvent( lEvent );
                  break;
               }

               case ' ':
               {
                  if ( zstrcmp( pZSocketTask->m_szActTag, "Alt+F4" ) == 0 )
                  {
                     pXRA->m_pZSubtask->HandleEvent( zWND_EVENT_CLOSE );
                  }

                  break;
               }

               case 'H':
               {
                  if ( zstrcmp( pZSocketTask->m_szActTag, "ESC" ) == 0 )
                  {
                     pXRA->m_pZSubtask->HandleEvent( zWND_EVENT_CLOSE );
                  }

                  break;
               }

               case 'C':
               default:
               {
                  if ( pXRA && pXRA->m_pZSubtask && pXRA->m_pZSubtask->m_pZMIXCtrl )
                  {
                     zLONG lEvent = 0;
                     pch = zstrrchr( pZSocketTask->m_szActTag, ';' );
                     if ( pch )
                     {
                        *pch = 0;
                        pch++;
                        lEvent = zatol( pch );
                     }

                     // Localize Tag for the seek.
                     CString csSeek( pZSocketTask->m_szActTag );
                     if ( pXRA->m_pZSubtask->m_pZMIXCtrl )
                     {
                        pzma = (ZMapAct *) pXRA->m_pZSubtask->m_pZMIXCtrl->SeekNode( &csSeek );
                        if ( pzma )
                           pzma->HandleEvent( lEvent );
                     }
                  }

                  break;
               }
            }
         }

         pZSocketTask->LockMutex( pZSocketTask->m_pZTask->m_vSubtask, FALSE, "SendReceive" );
      }

      pXRA = pZSocketTask->m_pCacheWndList->Find( pZSocketTask->m_szRemoteWndTag, 0, "fnZDrConnectionThreadProc2" );
#if 0
      if ( pXRA )
      {
         sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "pXRA: %ld m_pZSubtask: %ld found for %s",
                    pXRA, pXRA->m_pZSubtask, pZSocketTask->m_szRemoteWndTag );
         TraceLineS( szMsg, "" );
      }
      else
         TraceLineS( "pXRA not found for: ", pZSocketTask->m_szRemoteWndTag );
#endif

      if ( pXRA && (pXRA->m_pZSubtask == 0 || ((ulSubtaskState = ZSubtask::GetSubtaskState( pXRA->m_pZSubtask )) & zSUBTASK_STATE_MARK_FOR_DELETE)) )
      {
         while ( ResetViewFromSubobject( pXRA->m_vXRA_Delta ) == 0 )
         {
         }

         if ( CheckExistenceOfEntity( pXRA->m_vXRA_Delta, "Wnd" ) < zCURSOR_SET ||
              SetCursorFirstEntityByString( pXRA->m_vXRA_Delta, "Wnd", "Tag", pZSocketTask->m_szRemoteWndTag, 0 ) < zCURSOR_SET )
         {
            CreateEntity( pXRA->m_vXRA_Delta, "Wnd", zPOS_AFTER );
            SetAttributeFromString( pXRA->m_vXRA_Delta, "Wnd", "Tag", pZSocketTask->m_szRemoteWndTag );
         }

         SetAttributeFromString( pXRA->m_vXRA_Delta, "Wnd", "Closed", "Y" );
      // TraceLineS( "Remove2??? ===> ", pZSocketTask->m_szRemoteWndTag );
      // pZSocketTask->m_pCacheWndList->Remove( pXRA );
      }
   }

   }  // end of try block

   catch( CException *e )
   {
      zKZWDLGXO_Act_DEF ActDef;

      e->GetErrorMessage( szMsg, zsizeof( szMsg ) );
      TraceLineS( "(drvr) Remote Thread Exception ====> ", szMsg );
   // ::MessageBox( 0, szMsg, "Action Exception", MB_OK );
      ActDef.Type = zWAB_ExitDialogTask | zWAB_ProcessImmediateReturn;
      fnProcessReturn( pZSubtask, ActDef );
   }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

InitializationError_label:

// TraceLineS( "Done!!!", "" );

// while ( pZSocketTask->m_lSysMutexLockCnt )
// {
//    TraceLineS( "fnZDrConnectionThreadProc Termination waiting ...", "" );
//    SysWait( 500 );
// }

   if ( pZSocketTask->m_lSysMutexLockCnt )
      TraceLineS( "fnZDrConnectionThreadProc Termination waiting ...", "" );

   if ( pZSocketTask->m_vSubtask )
      SfDropSubtask( pZSocketTask->m_vSubtask, 0 );

// TraceLineI( "Done!!! Sending Termination String: ", (zLONG) pZSocketTask->m_sock );

   // Protect this with a semaphore!!!
   SysMutexLock( vSubtask, "ZDr", 0, 0 );

#ifdef DEBUG_ALL
   lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   TraceLineI( "fnZDrConnectionThreadProc shutdown: ", lItemCnt );
   while ( lItemCnt > 0 )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask )
      {
         sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ),
                    " [S%ld.T%ld.t%ld.s%ld] Dlg: %s Wnd: %s vSubtask: %#010hx   Subtask: ",
                    g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZSocketTask, AfxGetThread( ),
                    g_pSubtaskList[ lItemCnt ].lTID,
                    pZSocketTask->m_sock,
                    g_pSubtaskList[ lItemCnt ].DlgN,
                    g_pSubtaskList[ lItemCnt ].WndN,
                    g_pSubtaskList[ lItemCnt ].pvDialog );
         TraceLineI( szMsg, (zLONG) g_pSubtaskList[ lItemCnt ].pZSubtask );
      }

      lItemCnt--;
   }
#endif

   lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   CWinThread *pWinThread = AfxGetThread( );
   while ( lItemCnt > 0 )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
           pWinThread->m_nThreadID == (DWORD) g_pSubtaskList[ lItemCnt ].lTID &&
           (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
      {
         sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ),
                   "ZDRemote detected abnormal termination ... cleaning up: [S%ld.T%ld.t%ld.s%ld] Dlg: %s  Wnd: %s  vSubtask: %#010hx   Subtask: %ld  @",
                   g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZSocketTask, AfxGetThread( ),
                   g_pSubtaskList[ lItemCnt ].lTID,
                   pZSocketTask->m_sock,
                   g_pSubtaskList[ lItemCnt ].DlgN,
                   g_pSubtaskList[ lItemCnt ].WndN,
                   g_pSubtaskList[ lItemCnt ].pvDialog,
                   g_pSubtaskList[ lItemCnt ].pZSubtask );
         TraceLineI( szMsg, lItemCnt );
         pZSubtask = g_pSubtaskList[ lItemCnt ].pZSubtask;
         if ( pZSubtask )
         {
            SysMutexUnlock( vSubtask, "ZDr", 0 );
            pZSocketTask->m_pZTask->DeleteSubtask( pZSubtask );
            SysMutexLock( vSubtask, "ZDr", 0, 0 );
            lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
         }
         else
            lItemCnt--;
      }
      else
         lItemCnt--;
   }

   SysMutexUnlock( vSubtask, "ZDr", 0 );

   if ( nRC >= 0 )
   {
      ConnectionRecord  cr;

      // Send a cleanup string.
      zmemset( &cr, 0, zsizeof( ConnectionRecord ) );
      cr.pszBuffer = szBuffer;
      cr.sock = pZSocketTask->m_sock;
      cr.cMore = 0;
      fnSend( 0, "<zOI><SystemClose><Text>ApplicationTermination</Text></SystemClose></zOI>", 73, 0, (zPVOID) &cr );
//    fnSend( 0, "X", 1, 0, (zPVOID) &cr );
      fnSend( 0, 0, 0, 0, (zPVOID) &cr );
   }

   if ( GetViewByName( &vZDrSystem, "__ZDrSystem", 0, zLEVEL_SYSTEM ) > 0 )
   {
      CreateViewFromViewForTask( &vZDrSystem, vZDrSystem, m_vSubtask );
      if ( SetCursorFirstEntityByInteger( vZDrSystem, "Task", "Handle", pZSocketTask->m_pWinThread->m_nThreadID, 0 ) == zCURSOR_SET )
      {
         DeleteEntity( vZDrSystem, "Task", zREPOS_FIRST );
      }

      DropView( vZDrSystem );
   }

   // Remove pZSocketTask from the global list here!!!
// TraceLineI( "Done!!! Closing Socket: ", (zLONG) pZSocketTask->m_sock );
   delete( pZSocketTask );
   if ( bRegistered )
      UnregisterZeidonApplication( );

// TraceLineI( "All Done!!! Exiting Thread: ", (zLONG) AfxGetThread( ) );
   return( 0 );

} // fnZDrConnectionThreadProc

UINT
TrackConnectionThreadProc( zPVOID p )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDrApp::TrackConnectionThreadProc", "" );
#endif
   ZTask       *pTask = (ZTask *) p;
   WSADATA     WSAData;
   int         nStatus;
   SOCKET      listenSocket;
   SOCKADDR_IN saServer;
   int         nRC;

   if ( (nStatus = WSAStartup( 0x0101, &WSAData ) ) != 0 )
   {
      TraceLineI( "Error loading starting WinSock: ", nStatus );
      return( 0 );
   }

   listenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
   if ( listenSocket == INVALID_SOCKET )
   {
      TraceLineS( "Error creating socket", "" );
      return( 0 );
   }

   // Get the port number for the Remote Server (if specified - otherwise
   // default to 3027).
   zCHAR szPortNbr[ 32 ];

   SysReadZeidonIni( -1, "[zWinSock]", "RemoteServerPort", szPortNbr, zsizeof( szPortNbr ) );
   zUSHORT uPort = (zUSHORT) zatol( szPortNbr );
   if ( uPort == 0 )
      uPort = 3027;

   // Fill in the address structure.
   saServer.sin_family = AF_INET;
   saServer.sin_addr.s_addr = INADDR_ANY;  // Let WinSock supply address
   saServer.sin_port = htons( uPort );

   // Bind the name to the socket.
   nRC = bind( listenSocket, (LPSOCKADDR) &saServer, zsizeof( struct sockaddr ) );
   if ( nRC == SOCKET_ERROR )
   {
      fnShowError( "Bind" );
      TraceLineS( "Error binding socket", "" );
      closesocket( listenSocket );
      return( 0 );
   }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

   TraceLineS( "Waiting for connections...", "" );

   nRC = listen( listenSocket, 2 );
   if ( nRC == SOCKET_ERROR )
   {
      TraceLineS( "Error starting listen", "" );
      closesocket( listenSocket );
      return( 0 );
   }

   SOCKET newSocket;
// zLONG  lBytes;
// HANDLE hConnThread;
// UINT   dwThreadID;

   // Loop until the listen socket is closed.
   while ( TRUE )
   {
      newSocket = accept( listenSocket, 0, 0 );
      if ( newSocket == INVALID_SOCKET )
      {
         TraceLineS( "Error starting listen", "" );
         closesocket( listenSocket );
         return( 0 );
      }

      // Add pZSocketTask to the global list here!!!
      ZSocketTask *pZSocketTask = new ZSocketTask( newSocket, pTask );

      SysMutexLock( vSubtask, "ZDs", 0, 0 );
      zLONG lPos = (zLONG) g_pSocketList[ 0 ].pZSocketTask;
      while ( lPos > 0 )
      {
         if ( g_pSocketList[ lPos ].pZSocketTask == 0 )
         {
            g_pSocketList[ lPos ].pZSocketTask = pZSocketTask;
            g_pSocketList[ lPos ].ulSocketState = 0;
            break;
         }

         lPos--;
      }

      if ( lPos <= 0 )
      {
         lPos = (zLONG) g_pSocketList[ 0 ].pZSocketTask;
         lPos++;
         g_pSocketList[ 0 ].pZSocketTask = (ZSocketTask *) lPos;
         g_pSocketList[ lPos ].pZSocketTask = pZSocketTask;
         g_pSocketList[ lPos ].ulSocketState = 0;
      }

      SysMutexUnlock( vSubtask, "ZDs", 0 );

      // Fire off another thread to handle this connection.
      CWinThread *pThread = AfxBeginThread( fnZDrConnectionThreadProc, (zPVOID) pZSocketTask, THREAD_PRIORITY_NORMAL, 0, NULL );
   // TraceLineI( "Got a connection!  ID: ", (zLONG) pThread->m_nThreadID );
   // TraceLineI( "Got a connection!  Handle: ", (zLONG) pThread->m_hThread );
      if ( pThread )
      {
         ; // pThread->CloseHandle( (HANDLE) hThread );
      }
      else
      {
         closesocket( listenSocket );
         return( 0 );
      }
   }

   closesocket( listenSocket );
   return( 1 );
}

#endif

/////////////////////////////////////////////////////////////////////////////
//
// Random Student Id Generator
//
// use IO::Socket::INET;
//
// use constant RANDY_HOST => '10.100.0.24';
// use constant RANDY_PORT => '6288';
//
// my $peeraddr = RANDY_HOST . ':' . RANDY_PORT;
// my $client = IO::Socket::INET->new(PeerAddr => $peeraddr)
//   or die "Cannot connect to ". $peeraddr .": $!\n";
// my $id;
// $client->recv($id, 9);
// $client->close;
//
// # value of interest is now in $id
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
GenerateRandomStudentId( zPCHAR  pchReturnStudentId,
                         zLONG   lMaxLth,
                         zCPCHAR cpcIpAddress,
                         zLONG   lPort )
{
   WSADATA     WSAData;
   int         nStatus;
   SOCKET      listenSocket;
   SOCKADDR_IN saServer;
   int         nRC;

   *pchReturnStudentId = 0;

   if ( (nStatus = WSAStartup( 0x0101, &WSAData ) ) != 0 )
   {
      TraceLineI( "Error loading starting WinSock: ", nStatus );
      return( 0 );
   }

   listenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
   if ( listenSocket == INVALID_SOCKET )
   {
      TraceLineS( "Error creating socket", "" );
      return( 0 );
   }

   // Fill in the address structure.
   saServer.sin_family = AF_INET;

// saServer.sin_addr.s_addr = INADDR_ANY;  // Let WinSock supply address
   if ( cpcIpAddress == 0 || *cpcIpAddress == 0 )
      saServer.sin_addr.s_addr = inet_addr( "10.100.0.24" );
   else
      saServer.sin_addr.s_addr = inet_addr( cpcIpAddress );

   // Default the port number for the Remote Server to 6288.
   if ( lPort == 0 )
      lPort = 6288;

   saServer.sin_port = htons( (zUSHORT) lPort );

   // Bind the name to the socket.
   nRC = bind( listenSocket, (LPSOCKADDR) &saServer, sizeof( struct sockaddr ) );
   if ( nRC == SOCKET_ERROR )
   {
   // fnShowError( "GenerateRandomStudentId bind" );
      TraceLineS( "Error binding socket", "" );
      closesocket( listenSocket );
      return( 0 );
   }

   nRC = recv( listenSocket, pchReturnStudentId, lMaxLth, 0 );
   closesocket( listenSocket );

   return( nRC );
}

extern zPCHAR g_MemHead;
extern zPCHAR g_MemTail;

// ZTask - ctor
zLONG ZTask::g_lIdNbr = 1;
ZTask::ZTask( ZDrApp *pDrApp,
              zVIEW  vSubtask,
              ZCommandLineInfo *pCmdInfo )
{
   m_ulTaskIdNbr = g_lIdNbr++;

   m_nOS_Version = DetermineSystemVersion( 0, 0, 0, 0 );
   m_pZDrApp = pDrApp;

   // Used to keep track of the subtasks (windows) within this application
   m_pZSubtaskTop = 0;
   m_vNewAppSubtask = 0;
   m_pZSubtaskFocus = 0;   // Window to which to set focus (current if 0)
   m_lpfnInterp = 0;       // Interpretor
   m_vInterpSubtask = 0;
   m_pzsHelpFile = 0;
   m_pCmdInfo = pCmdInfo;
   m_pZSubtaskConversational = 0;
   m_pzsTwainFileName = 0;
   m_ulNextSplitterPane = AFX_IDW_PANE_FIRST + 1;
   m_pSplitterDiscardHead = 0;
   if ( IsNT( ) )
      m_uAppState = zAPP_STATE_NT;
   else
      m_uAppState = 0;

   m_vHK = 0;  // Hotkey
   m_vPE = 0;  // PPE
   InitTask( vSubtask );

   m_bLocked = FALSE;
   m_pZFWndHidden = 0;
   m_pzsLastDlgTag = 0;
   m_pzsLastWndTag = 0;
   m_pZSubtaskPrint = 0;

   // The cursors used for Drag & Drop are at the task level for now.  Also,
   // the default cursors need to be picked up from a .INI for each application???
   m_hcurSave = 0;
   m_hcurDropNot  = pDrApp->LoadCursor( IDC_DROPNOT );
   m_hcurDropOKC1 = pDrApp->LoadCursor( IDC_DROPOKC1 );
   m_hcurDropOKM1 = pDrApp->LoadCursor( IDC_DROPOKM1 );
   m_hcurDropOKCM = pDrApp->LoadCursor( IDC_DROPOKCM );
   m_hcurDropOKMM = pDrApp->LoadCursor( IDC_DROPOKMM );

#ifdef zREMOTE_SERVER
   m_pThread = 0;
#endif

   m_pWndList = 0;

   // Retain the Dialog name by which this task is being started
   SfGetTaskInfo( m_szInitialDlgName, zsizeof( m_szInitialDlgName ), zTASK_DIALOG, SfGetCurrentTask( vSubtask ) );

   // Ask OE if Driver action messages should be traced.
#define IDM_GETACTIONTRACE 204
   m_nTraceAction = (zSHORT) SysSendOE_Message( IDM_GETACTIONTRACE, 0 );
   // 0 - no trace
   // 1 - errors
   // 2 - errors + warnings
   // 3 - errors + warnings + informational
   // 4 - all
   // 0x70 - Report ('R')
   // 0x71 - Task ('T')
   // 0x72 - Subtask ('S')
   // 0x74 - View ('V')
   // 0x77 - Task/Subtask/View ('Z')
   // 0x99 - Cannot be reset by application ('~') SetActionTrace
   if ( (char) m_nTraceAction == '0' || (char) m_nTraceAction == 'N' || (char) m_nTraceAction == 'n' )
   {
      m_nTraceAction = 0;
   }
   else
   if ( (char) m_nTraceAction == '4' || (char) m_nTraceAction == 'Y' || (char) m_nTraceAction == 'y' )
   {
      m_nTraceAction = 4;
   }
   else
   if ( (char) m_nTraceAction == '~' )
      m_nTraceAction = 0x99;
   else
   if ( (char) m_nTraceAction == 'R' )  // Report
      m_nTraceAction = 0x70;
   else
   if ( (char) m_nTraceAction == 'T' )  // Task
      m_nTraceAction = 0x71;
   else
   if ( (char) m_nTraceAction == 'S' )  // Subtask
      m_nTraceAction = 0x72;
   else
   if ( (char) m_nTraceAction == 'V' )  // View
      m_nTraceAction = 0x74;
   else
   if ( (char) m_nTraceAction == 'Z' )  // Task/Subtask/View
      m_nTraceAction = 0x77;
   else
   if ( m_nTraceAction >= '1' && m_nTraceAction <= '4' )
      m_nTraceAction -= '0';

   if ( (m_nTraceAction & 0x71) == 0x71 )
      TraceLine( "ZTask::ctor (0x%08x): %d", this, m_ulTaskIdNbr );

   if ( m_nTraceAction )
      TraceLineX( "ZTask::TraceAction setting: ", m_nTraceAction );

   zCHAR szHoverDelay[ 256 ] = { 'x' };

   SysReadZeidonIni( -1, "[Workstation]", "HoverDelay", szHoverDelay, zsizeof( szHoverDelay ) );
   m_lHoverDelay = zatol( szHoverDelay );

#ifdef zREMOTE_SERVER

   // Start up the thread to track connections.
   m_pThread = AfxBeginThread( TrackConnectionThreadProc,
                               (zPVOID) this,
                               THREAD_PRIORITY_NORMAL,
                               0, NULL );
   if ( m_pThread )
   {
   // m_pThread->CloseHandle( (HANDLE) hThread );
   // TraceLineI( "Track Connection thread: ", (zLONG) m_pThread->m_nThreadID );
   }
   else
      ::PostMessage( m_pZDrApp->m_hWndCommOE, WM_CLOSE, 0, 0L );

#else

   zVIEW vZDrSystem;

   if ( GetViewByName( &vZDrSystem, "__ZDrSystem", vSubtask, zLEVEL_SYSTEM ) > 0 )
   {
      CreateViewFromViewForTask( &vZDrSystem, vZDrSystem, vSubtask );
      CreateEntity( vZDrSystem, "Task", zPOS_FIRST );
      SetAttributeFromInteger( vZDrSystem, "Task", "Handle", (zLONG) this );
      DropView( vZDrSystem );
   }

   ZSubtask *pZSubtask = NewZWnd( 0, 0, vSubtask, 0, FALSE, m_szInitialDlgName, "" );
   if ( m_pZSubtaskTop && m_pZSubtaskTop->m_pZFWnd )
   {
      zULONG ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtaskTop );
      if ( ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE )
      {
         if ( m_pZSubtaskTop && (m_uAppState & zAPP_STATE_TERMINATED) == 0 )
            mDeleteInit( m_pZSubtaskTop );

         ::PostMessage( m_pZDrApp->m_hWndCommOE, WM_CLOSE, 0, 0L );
      }
   }

#endif
}

// ZTask - alternate ctor
ZTask::ZTask( zVIEW  vSubtask )
{
   m_ulTaskIdNbr = g_lIdNbr++;
   if ( (m_nTraceAction & 0x71) == 0x71 )
      TraceLine( "ZTask::ZTask ALTERNATE ctor: (0x%08x) %d", this, m_ulTaskIdNbr );

   m_nOS_Version = DetermineSystemVersion( 0, 0, 0, 0 );
   m_pZDrApp = 0;

   // Used to keep track of the subtasks (windows) within this application
   m_pZSubtaskTop = 0;
   m_vNewAppSubtask = 0;
   m_pZSubtaskFocus = 0;   // Window to which to set focus (current if 0)
   m_lpfnInterp = 0;       // Interpretor
   m_vInterpSubtask = 0;
   m_pzsHelpFile = 0;
   m_pCmdInfo = 0;
   m_pZSubtaskConversational = 0;
   m_pzsTwainFileName = 0;
   m_ulNextSplitterPane = AFX_IDW_PANE_FIRST + 1;
   m_pSplitterDiscardHead = 0;
   if ( IsNT( ) )
      m_uAppState = zAPP_STATE_NT;
   else
      m_uAppState = 0;

   m_vHK = 0;  // Hotkey
   m_vPE = 0;  // PPE
   InitTask( vSubtask );

   m_bLocked = FALSE;
   m_pZFWndHidden = 0;
   m_pzsLastDlgTag = 0;
   m_pzsLastWndTag = 0;

   // The cursors used for Drag & Drop are at the task level for now.
   // Also, the default cursors need to be picked up from a .INI for
   // each application???
   m_hcurSave = 0;

   m_hcurDropNot  = 0;
   m_hcurDropOKC1 = 0;
   m_hcurDropOKM1 = 0;
   m_hcurDropOKCM = 0;
   m_hcurDropOKMM = 0;

#ifdef zREMOTE_SERVER
   m_pThread = 0;
#endif

   m_pWndList = 0;

   m_szInitialDlgName[ 0 ] = 0;

   zCHAR szHoverDelay[ 256 ];

   SysReadZeidonIni( -1, "[Workstation]", "HoverDelay", szHoverDelay, zsizeof( szHoverDelay ) );
   m_lHoverDelay = zatol( szHoverDelay );

   // Ask OE if Driver action messages should be traced.
#define IDM_GETACTIONTRACE 204
   m_nTraceAction = (zSHORT) SysSendOE_Message( IDM_GETACTIONTRACE, 0 );
   // 0 - no trace
   // 1 - errors
   // 2 - errors + warnings
   // 3 - errors + warnings + informational
   // 4 - all
   // 99 - cannot be reset by SetActionTrace
   if ( (char) m_nTraceAction == '0' || (char) m_nTraceAction == 'N' || (char) m_nTraceAction == 'n' )
   {
      m_nTraceAction = 0;
   }
   else
   if ( (char) m_nTraceAction == '~' )
      m_nTraceAction = 99;
   else
   if ( m_nTraceAction >= '1' && m_nTraceAction <= '4' )
      m_nTraceAction -= '0';
}

// ZTask - dtor
ZTask::~ZTask( )
{
   if ( (m_nTraceAction & 0x71) == 0x71 )
      TraceLine( "ZTask::dtor: (0x%08x) %d", this, m_ulTaskIdNbr );

   mDeleteInit( m_pWndList );

   ZSubtask *pZSubtask;
   ZNumberItem *pNext;

   while ( m_pSplitterDiscardHead )
   {
      pZSubtask = (ZSubtask *) m_pSplitterDiscardHead;
      pNext = m_pSplitterDiscardHead->m_pNextNumber;
      m_pSplitterDiscardHead->m_pNextNumber = 0;
      delete( m_pSplitterDiscardHead );
      m_pSplitterDiscardHead = pNext;

      delete( pZSubtask );
   }

   if ( m_vPE )
      DropObjectInstance( m_vPE );

   if ( m_vHK )
      DropObjectInstance( m_vHK );

#ifdef zREMOTE_SERVER
   TraceLineI( "End Track Connection thread: ", (zLONG) m_pThread->m_nThreadID );
   mDeleteInit( m_pThread );

#else

   zVIEW vZDrSystem;

   if ( m_pZDrApp && GetViewByName( &vZDrSystem, "__ZDrSystem", m_pZDrApp->m_vSubtask, zLEVEL_SYSTEM ) > 0 )
   {
      CreateViewFromViewForTask( &vZDrSystem, vZDrSystem, m_pZDrApp->m_vSubtask );
      if ( SetCursorFirstEntityByInteger( vZDrSystem, "Task", "Handle", (zLONG) this, 0 ) == zCURSOR_SET )
      {
         DeleteEntity( vZDrSystem, "Task", zREPOS_FIRST );
      }

      DropView( vZDrSystem );
   }

#endif

// mDeleteInit( m_pCmdInfo ); m_pCmdInfo is a copy of a pointer
   mDeleteInit( m_pzsTwainFileName );
   mDeleteInit( m_pzsHelpFile );
   mDeleteInit( m_pzsLastDlgTag );
   mDeleteInit( m_pzsLastWndTag );

   if ( m_hcurDropNot )
      ::DestroyCursor( m_hcurDropNot );

   if ( m_hcurDropOKC1 )
      ::DestroyCursor( m_hcurDropOKC1 );

   if ( m_hcurDropOKM1 )
      ::DestroyCursor( m_hcurDropOKM1 );

   if ( m_hcurDropOKCM )
      ::DestroyCursor( m_hcurDropOKCM );

   if ( m_hcurDropOKMM )
      ::DestroyCursor( m_hcurDropOKMM );

   if ( m_pZDrApp )
   {
      if ( g_MemHead )
      {
         TraceLineX( "ZTask::dtor: Memory leak detected: ", (zLONG) g_MemHead );
      }

      while ( g_MemHead )
      {
         DrFreeTaskMemory( g_MemHead );
      }
   }
}

void
ZTask::InitTask( zVIEW vSubtask )
{
   // This is an application level object ... if the object already exists,
   // use it, otherwise activate it.
   GetViewByName( &m_vPE, szl__PE, vSubtask, zLEVEL_TASK );
   if ( m_vPE == 0 )
   {
      zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
      zVIEW  vTaskLPLR;
      zBOOL  bLPLR_XPE = FALSE;

      //BL, 2000.01.30 search in LPLR directory
      // Activate using SfActivateSysOI_FromFile so that the kzpexvoo.xod
      // does not have to be in the application LPLR.
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
      {
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "LPLR", "ExecDir" );
         SysAppendcDirSep( szFileName );
         strcat_s( szFileName, zsizeof( szFileName ), szlZEIDON_XPE );
         if ( SfActivateSysOI_FromFile( &m_vPE, szlKZPEXVOO, vSubtask, szFileName, zSINGLE | zLEVEL_APPLICATION | zNOI_OKAY ) >= 0 )
         {
            bLPLR_XPE = TRUE;
         }
      }

      if ( bLPLR_XPE == FALSE )
      {
         SysGetLocalSysDirectory( szFileName );
         SysAppendcDirSep( szFileName );
         strcat_s( szFileName, zsizeof( szFileName ), szlZEIDON_XPE );
         if ( SfActivateSysOI_FromFile( &m_vPE, szlKZPEXVOO, vSubtask, szFileName, zSINGLE | zLEVEL_APPLICATION | zNOI_OKAY ) >= 0 )
         {
            bLPLR_XPE = TRUE;
         }
      }

      if ( bLPLR_XPE )
      {
         SetNameForView( m_vPE, szl__PE, vSubtask, zLEVEL_TASK );
         SfLockView( m_vPE );  // we lock this view because it is unnamed ... to prevent the OE from
                               // automatically cleaning it up during the deletion of an OE subtask.
      }
      else
      {
         // ERROR out of here ... will not go far without this object!!!
         // Is this the correct way to handle this error condition???
         TraceLineS( "(drvr) Cannot load Presentation Environment from file - ", szlZEIDON_XPE );
         MessageSend( vSubtask, 0, szlDrvr, "Cannot load Presentation Environment",  zMSGQ_MODAL_ERROR, FALSE );
         return; // error ... throw an exception here???
      }
   }

   GetViewByName( &m_vHK, szl__HKEY, vSubtask, zLEVEL_TASK );
   if ( m_vHK == 0 )
   {
      zCHAR szFileSpec[ 256 ];

      // Activate using SfActivateSysOI_FromFile so that the kzhkeyxo.xod does not have to be in the application LPLR.
      GetApplDirectoryFromView( szFileSpec, vSubtask, zAPPL_DIR_OBJECT, zsizeof( szFileSpec ) );
      strcat_s( szFileSpec, zsizeof( szFileSpec ), szlKZHKEYAA_POR );
      if ( SfActivateSysOI_FromFile( &m_vHK, szlKZHKEYXO, vSubtask, szFileSpec, zSINGLE | zLEVEL_APPLICATION | zNOI_OKAY ) < 0 )
      {
         zULONG  ulHotkeyLth;
         zULONG  ulGlobalHotkeyLth;

         CreateEntity( m_vHK, szlApp, zPOS_AFTER );
         ulGlobalHotkeyLth = sizeof( zSHORT );
         ulHotkeyLth = -1;
         SetAttributeFromBlob( m_vHK, szlApp, szlHotkey, &ulHotkeyLth, ulGlobalHotkeyLth );
      }

      SetNameForView( m_vHK, szl__HKEY, vSubtask, zLEVEL_TASK );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Create a new window within this task.
ZSubtask *
ZTask::NewZWnd( ZSubtask *pZParent,    // Zeidon parent window (subtask)
                ZSubtask *pZOwner,     // Zeidon owner window (subtask)
                zVIEW    vSubtask,     // Zeidon subtask for this window
                zVIEW    vNewSubtask,  // if 0, create new subtask, else use
                zSHORT   fCreateModal, // create as a modal window
                zCPCHAR  cpcDlgTag,    // Dialog containing the ...
                zCPCHAR  cpcWndTag,    // Window to create (0 ==> dflt)
#ifdef zREMOTE_SERVER
                ZSubtask *pZSubtaskReplace,
                ZSocketTask *pZSocketTask )
#else
                ZSubtask *pZSubtaskReplace )
#endif
{
   ZSubtask *pZSubtask;
   zULONG   ulSubtaskState;
   zULONG   ulSubtaskStateHold = zSUBTASK_STATE_INIT;
   zBOOL    bReplaced = FALSE;
   zBOOL    bModal = (fCreateModal == TRUE) ? TRUE : FALSE;

   // Protect this with a semaphore!!!
// SysMutexLock( vSubtask, "ZTask", 0, zMUTEX_SAMETASK );

   if ( pZSubtaskReplace )
      ulSubtaskStateHold = ZSubtask::SetSubtaskState( pZSubtaskReplace, 0, zSUBTASK_STATE_PROVISION_REPLACE );

#ifdef zREMOTE_SERVER
   if ( pZSubtaskReplace &&
        zstrcmp( cpcDlgTag, *(pZSubtaskReplace->m_pzsDlgTag) ) == 0 &&
        zstrcmp( cpcWndTag, *(pZSubtaskReplace->m_pzsWndTag) ) == 0 )
   {
      pZSubtask = pZSubtaskReplace;
      pZSubtaskReplace = 0;
      ZSubtask::SetSubtaskState( pZSubtask, ~zSUBTASK_STATE_IN_CONSTRUCTION,
                                 zSUBTASK_STATE_IN_CONSTRUCTION | zSUBTASK_STATE_REPLACED_BY_SELF );
   }
   else
#endif
   {
      if ( pZParent && (pZParent->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) )
      {
         pZSubtask = pZParent->GetSplitterMainSubtask( );
         if ( pZSubtask )
         {
            pZParent = pZSubtask;
            if ( pZOwner && (pZOwner->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) )
            {
               pZSubtask = pZOwner->GetSplitterMainSubtask( );
               if ( pZSubtask )
                  pZOwner = pZSubtask;
            }
         }
      }

      // First create the subtask object which will localize all the information garnered from the Zeidon Dialog object (.XWD).
#ifdef zREMOTE_SERVER
      if ( pZSocketTask == 0 )
         pZSocketTask = pZParent->m_pZSocketTask;

   // TraceLineS( "ZTask::NewZWnd: ", "new ZSubtask" );
      pZSubtask = new ZSubtask( this, pZParent, pZOwner, vSubtask, vNewSubtask, bModal, cpcDlgTag, cpcWndTag, pZSocketTask );

   // TraceLineS( "ZTask::NewZWnd: ", "ZSubtask created" );
      if ( pZSocketTask->m_pZSubtaskTop == 0 )
         pZSocketTask->m_pZSubtaskTop = pZSubtask;

      strcpy_s( pZSocketTask->m_szRemoteWndTag, zsizeof( pZSocketTask->m_szRemoteWndTag ), *(pZSubtask->m_pzsRemoteWndTag) );
      ZXRA_Item *pXRA = pZSocketTask->m_pCacheWndList->Find( *(pZSubtask->m_pzsRemoteWndTag), 0, "ZTask::NewZWnd" );

      if ( pXRA == 0 )
      {
         zVIEW  vSocketSubtask;

         SfCreateSubtask( &vSocketSubtask, pZSubtask->m_vDialog, 0 );
         if ( CreateRemoteWindowSubtask( &(pZSocketTask->m_vXRA), vSocketSubtask, *(pZSubtask->m_pzsDlgTag), *(pZSubtask->m_pzsRemoteWndTag) ) )
         {
            MessageSend( pZSocketTask->m_vSubtask, 0, "Zeidon Application", "Cannot create Zeidon subtask", zMSGQ_MODAL_ERROR, FALSE );
            ZSubtask::SetSubtaskState( pZSubtask, 0, zSUBTASK_STATE_DELETE );
         }
         else
         {
            if ( (m_nTraceAction & 0x71) == 0x71 )
            {
               CString cs = *(pZSubtask->m_pzsDlgTag);
               cs += *(pZSubtask->m_pzsRemoteWndTag);
               cs += "__SocketSubtask";
               SetNameForView( pZSocketTask->m_vXRA, cs, pZSocketTask->m_vSubtask, zLEVEL_TASK );
            }

            SetAttributeFromString( pZSocketTask->m_vXRA, "App", "Tag", pZSocketTask->m_szAppTag );

            pXRA = pZSocketTask->m_pCacheWndList->Add( *(pZSubtask->m_pzsRemoteWndTag), pZSubtask, pZSocketTask->m_vXRA );
            pZSocketTask->m_vXRA = 0;
         }
      }
      else
      {
         mDropOI_Init( pXRA->m_vXRA );
         ActivateOI_FromOI_ForTask( &(pXRA->m_vXRA), pXRA->m_vXRA_Def, 0, zSINGLE );
         if ( (m_nTraceAction & 0x71) == 0x71 )
         {
            CString cs = *(pZSubtask->m_pzsDlgTag);
            cs += *(pZSubtask->m_pzsRemoteWndTag);
            cs += "__XRA_FromDef";
            SetNameForView( pXRA->m_vXRA, cs, pZSocketTask->m_vSubtask, zLEVEL_TASK );
         }

         mDropOI_Init( pXRA->m_vXRA_Delta );
      }

      if ( pXRA )
      {
         pXRA->ActivateEmptyDelta( pZSocketTask->m_vRegisteredSubtask, pZSocketTask->m_szAppTag );

         pZSubtask->m_pZSocketTask = pZSocketTask;
         pXRA->m_pZSubtask = pZSubtask;

      // zCHAR szMsg[ 256 ];
      //
      // sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "pXRA: %ld m_pZSubtask: %ld set for %s",
      //           pXRA, pXRA->m_pZSubtask, pZSocketTask->m_szRemoteWndTag );
      // TraceLineS( szMsg, "" );
      }
   // else
      // TraceLineS( "pXRA NULL for: ", pZSocketTask->m_szRemoteWndTag );

#else

      pZSubtask = new ZSubtask( this, pZParent, pZOwner, vSubtask, vNewSubtask, bModal, cpcDlgTag, cpcWndTag );
#endif
   }

   if ( pZSubtaskReplace )
   {
      ZSubtask::SetSubtaskState( pZSubtaskReplace, 0xFFFFFFFF, ulSubtaskStateHold );
   }

   // If things did not go well in the Subtask ctor, we want to
   // leave as gracefully as possible.
   ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
   if ( ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE )
   {
      TraceLine( "ZTask::NewZWnd unable to create Subtask: %s.%s", *(pZSubtask->m_pzsDlgTag), *(pZSubtask->m_pzsWndTag) );
      m_uAppState |= zAPP_STATE_TERMINATED;
      delete( pZSubtask );
   // SysMutexUnlock( vSubtask, "ZTask", zMUTEX_SAMETASK );
      return( 0 );
   }

// if ( pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SDI_FRAME )
// {
// }

   if ( pZSubtaskReplace )
   {
#ifdef zREMOTE_SERVER
      if ( pZSocketTask->m_pZSubtaskTop == pZSubtaskReplace )
      {
         pZSocketTask->m_pZSubtaskTop = pZSubtask;
         bReplaced = TRUE;
      }
#else
      if ( m_pZSubtaskTop == pZSubtaskReplace )
      {
         m_pZSubtaskTop = pZSubtask;
         bReplaced = TRUE;
      }
#endif

      ZSubtask::SetSubtaskState( pZSubtaskReplace, 0, zSUBTASK_STATE_REPLACED | zSUBTASK_STATE_DELETE );
   }

   if ( fCreateModal == 2 ) // hack to get me out of here if Splitter
   {
   // SysMutexUnlock( vSubtask, "ZTask", zMUTEX_SAMETASK );
      return( pZSubtask );
   }

   pZSubtask->RestartBuildWindow( 0 );

   if ( pZSubtaskReplace )
   {
      ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtaskReplace );
      if ( ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE )
      {
         DeleteSubtask( pZSubtaskReplace );
         pZSubtaskReplace = 0;
      }
   }

#ifdef zREMOTE_SERVER
   ulSubtaskState = ZSubtask::GetSubtaskState( pZSocketTask->m_pZSubtaskTop );
#else
   ulSubtaskState = ZSubtask::GetSubtaskState( m_pZSubtaskTop );
#endif

   if ( ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE )
   {
      ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
      if ( ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE )
      {
#ifdef zREMOTE_SERVER
         if ( pZSocketTask->m_pZSubtaskTop == pZSubtask )
#else
         if ( m_pZSubtaskTop == pZSubtask )
#endif
         {
            // If this was the main window and there are other windows left, we must find another main window to take this one's place.
            zLONG lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
            while ( lItemCnt )
            {
               if ( g_pSubtaskList[ lItemCnt ].pZSubtask && (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
               {
                  if ( pZSubtask->m_lTID == g_pSubtaskList[ lItemCnt ].lTID && pZSubtask == g_pSubtaskList[ lItemCnt ].pZSubtask )
                  {
                     MessageSend( pZSubtask->m_vDialog, 0, "ZTask::NewZWnd", "Unexpected subtask pointer", zMSGQ_MODAL_ERROR, FALSE );
                  }
                  else
                  if ( pZSubtask->m_lTID == g_pSubtaskList[ lItemCnt ].lTID && g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == 0 )
                  {
#ifdef zREMOTE_SERVER
                     pZSocketTask->m_pZSubtaskTop = g_pSubtaskList[ lItemCnt ].pZSubtask;
#else
                     m_pZSubtaskTop = g_pSubtaskList[ lItemCnt ].pZSubtask;
                     m_pZDrApp->m_pMainWnd = m_pZSubtaskTop->m_pZFWnd;
                     ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, (zLONG) m_pZDrApp->m_pMainWnd->m_hWnd );
#endif
                     break;
                  }
               }

               lItemCnt--;
            }

            if ( lItemCnt == 0 )
            {
               DeleteSubtask( pZSubtask );
               pZSubtask = 0;
               m_uAppState |= zAPP_STATE_TERMINATED;

               // There are no "viable" windows left in this task.
               ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, 0L );
               ::PostMessage( m_pZDrApp->m_hWndCommOE, WM_CLOSE, 0, 0L );
               AfxPostQuitMessage( 0 );

               // m_pZDrApp->m_pMainWnd = 0;
            }
         }
      }
      else
      {
#ifdef zREMOTE_SERVER
         pZSocketTask->m_pZSubtaskTop = pZSubtask;
#else
         m_pZDrApp->m_pMainWnd = pZSubtask->m_pZFWnd;
         if ( m_pZDrApp->m_pMainWnd )
            ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, (zLONG) m_pZDrApp->m_pMainWnd->m_hWnd );
         else
            ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, 0 );
#endif
      }
   }
   else
   {
      ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask );
      if ( ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE )
      {
         if ( (ulSubtaskState & zSUBTASK_STATE_ERROR) != zSUBTASK_STATE_ERROR )
            DeleteSubtask( pZSubtask );

         pZSubtask = 0;
      }
      else
      if ( bReplaced )
      {
         m_pZDrApp->m_pMainWnd = pZSubtask->m_pZFWnd;
         if ( m_pZDrApp->m_pMainWnd )
            ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, (zLONG) m_pZDrApp->m_pMainWnd->m_hWnd );
         else
            ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, 0 );
      }
   }

#ifdef zREMOTE_SERVER
   if ( pZSubtask )
      SetRemoteWndAttribute( pZSubtask->m_vDialog, "Active", "Y" );
#endif

// SysMutexUnlock( vSubtask, "ZTask", zMUTEX_SAMETASK );
   return( pZSubtask );
}

void
ZTask::DeleteZWnd( ZSubtask *pZSubtask )
{
#ifdef DEBUG_ALL
   int k = 0;
// k /= k;
   TraceLine( "ZTask::DeleteZWnd Task: 0x%08x  IdNbr: %d  Subtask: 0x%08x %s",
              this, pZSubtask->m_ulSubtaskIdNbr, pZSubtask, (pZSubtask && pZSubtask->m_pzsWndTag) ? *(pZSubtask->m_pzsWndTag) : "?" );
#endif

   pZSubtask->m_nInvisible = -1;    // prevent painting or whatever

#if 0  // this causes an exception down in Windows
   MSG   msg;
   while ( ::PeekMessage( &msg, 0, WM_NCMOUSEMOVE, WM_NCMOUSEMOVE, PM_REMOVE ) )
   {
      TraceLine( "DeleteZWnd removing NC MouseMove messages from queue for hWnd: 0x%08x", msg.hwnd );
   // ::TranslateMessage( &msg );    do nuttin
   // ::DispatchMessage( &msg );
   }
#endif

#ifdef zREMOTE_SERVER
   ZSocketTask *pZSocketTask = pZSubtask->m_pZSocketTask;
#endif

   // Run through the list of Child windows to find any that have this window as its parent ... destroy them if they exist.
   zULONG ulSubtaskState;
   zLONG  lPos = 1;
   zLONG  lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   while ( lPos <= lItemCnt )
   {
      if ( g_pSubtaskList[ lPos ].pZSubtask && g_pSubtaskList[ lPos ].lTID == pZSubtask->m_lTID &&
           g_pSubtaskList[ lPos ].pZSubtask->m_pZParent == pZSubtask )
      {
         pZSubtask->m_pZTask->DeleteSubtask( g_pSubtaskList[ lPos ].pZSubtask );

         lPos = 1;
         lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      }
      else
         lPos++;
   }

#ifdef DEBUG_ALL
   TraceLine( "ZTask::DeleteZWnd CREATED_MODAL?: [0x%08x]  Subtask: 0x%08x  IdNbr: %d   %s.%s   MODAL==> %s",
              this, pZSubtask, pZSubtask ? pZSubtask->m_ulSubtaskIdNbr : 0,
              (pZSubtask && pZSubtask->m_pzsDlgTag) ? *(pZSubtask->m_pzsDlgTag) : "?",
              (pZSubtask && pZSubtask->m_pzsWndTag) ? *(pZSubtask->m_pzsWndTag) : "?",
              (pZSubtask->m_ulSubtaskFlags & zSUBTASK_CREATED_MODAL) ? "YES" : "NO" );
#endif

   if ( pZSubtask->m_ulSubtaskFlags & zSUBTASK_CREATED_MODAL )
      pZSubtask->EnableParentWindows( TRUE );

   lItemCnt = ZSubtask::GetSubtaskCount( pZSubtask->m_lTID );
   if ( lItemCnt == 0 || (lItemCnt == 1 && pZSubtask->m_pZParent &&
        ((ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask->m_pZParent )) & zSUBTASK_STATE_EXTERNAL_APP)) )
   {
      if ( m_pzsHelpFile )
      {
         ::WinHelp( pZSubtask->m_pZFWnd->m_hWnd, *m_pzsHelpFile, HELP_QUIT, 0 );
         mDeleteInit( m_pzsHelpFile );
      }

      if ( lItemCnt == 1 )
         mDeleteInit( pZSubtask->m_pZParent );

      m_pZSubtaskTop = 0;
//    TraceLineS( "ZDrTask::DeleteZWnd ", "1" );
      ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, 0L );
      ::SendMessage( m_pZDrApp->m_hWndCommOE, zZEIDON_POSTED_CLOSE, 0, 0L );
      AfxPostQuitMessage( 0 );
   }
   else
#ifdef zREMOTE_SERVER
   if ( pZSocketTask->m_pZSubtaskTop == pZSubtask )
#else
   if ( m_pZSubtaskTop == pZSubtask )
#endif
   {
      // If this was the main window and there are other windows left, we must find another main window to take this one's place.
      if ( lItemCnt )
      {
         lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
         while ( lItemCnt )
         {
            if ( g_pSubtaskList[ lItemCnt ].pZSubtask && (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
            {
               if ( pZSubtask->m_lTID == g_pSubtaskList[ lItemCnt ].lTID && pZSubtask == g_pSubtaskList[ lItemCnt ].pZSubtask )
               {
                  MessageSend( pZSubtask->m_vDialog, 0, "ZTask::DeleteZWnd", "Unexpected subtask pointer", zMSGQ_MODAL_ERROR, FALSE );
               }
               else
               if ( pZSubtask->m_lTID == g_pSubtaskList[ lItemCnt ].lTID && g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == 0 )
               {
#ifdef zREMOTE_SERVER
                  pZSocketTask->m_pZSubtaskTop = g_pSubtaskList[ lItemCnt ].pZSubtask;
#else
                  m_pZSubtaskTop = g_pSubtaskList[ lItemCnt ].pZSubtask;
                  m_pZDrApp->m_pMainWnd = m_pZSubtaskTop->m_pZFWnd;
                  ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, (zLONG) m_pZDrApp->m_pMainWnd->m_hWnd );
#endif
                  break;
               }
            }

            lItemCnt--;
         }
      }

      if ( lItemCnt == 0 )
      {
         // There are no "viable" windows left in this task.
         ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, 0L );
         ::PostMessage( m_pZDrApp->m_hWndCommOE, WM_CLOSE, 0, 0L );
         AfxPostQuitMessage( 0 );

         // m_pZDrApp->m_pMainWnd = 0;
      }
   }
}

void
FlipFrameSubtaskView( ZSubtask *pZSubtask, ZSubtask *pZSubtaskNew );

void
ZTask::DeleteSubtask( ZSubtask *pZSubtask )
{
#ifdef DEBUG_ALL
   TraceLine( "ZTask::DeleteSubtask Task: [0x%08x]  Subtask: [0x%08x] IdNbr: %d  %s.%s",
              this, pZSubtask, pZSubtask ? pZSubtask->m_ulSubtaskIdNbr : 0,
              (pZSubtask && pZSubtask->m_pzsDlgTag) ? *(pZSubtask->m_pzsDlgTag) : "?",
              (pZSubtask && pZSubtask->m_pzsWndTag) ? *(pZSubtask->m_pzsWndTag) : "?" );
#endif

   zLONG  lTID = pZSubtask->m_lTID;
   zLONG  lItemCnt;
   zULONG ulSubtaskState = ZSubtask::GetSubtaskState( pZSubtask->m_pZSubtaskGlobal );
   zULONG ulSubtaskState1 = ZSubtask::GetSubtaskState( pZSubtask->m_pZSubtaskProxy );

   if ( ((ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && lTID == pZSubtask->m_pZSubtaskGlobal->m_lTID) ||
        ((ulSubtaskState1 & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 && lTID == pZSubtask->m_pZSubtaskProxy->m_lTID) )
   {
      lItemCnt = ZSubtask::GetSubtaskCount( lTID );
      if ( lItemCnt == 2 )
      {
         ZSubtask *pZSubtaskGlobal;
         ZSubtask *pZSubtaskProxy;

         if ( pZSubtask->m_pZSubtaskProxy )
         {
            pZSubtaskProxy = pZSubtask->m_pZSubtaskProxy;
            pZSubtask->m_pZSubtaskProxy = 0;
            pZSubtask = pZSubtaskProxy;
         }

         pZSubtaskGlobal = pZSubtask->m_pZSubtaskGlobal;
         pZSubtask->m_pZSubtaskGlobal = 0;
         pZSubtaskGlobal->m_pZSubtaskProxy = 0;
         FlipFrameSubtaskView( pZSubtask, pZSubtaskGlobal );
         DeleteSubtask( pZSubtaskGlobal );
      }
   }

   ZSubtask *pZSubtaskOwner;
   if ( pZSubtask->m_pZParent == 0 )
      pZSubtaskOwner = pZSubtask->m_pZOwner;
   else
      pZSubtaskOwner = pZSubtask->m_pZParent;

   // Force all child subtasks to be closed before parent subtasks.

   // Protect this with a semaphore!!!
   SysMutexLock( pZSubtask->m_vDialog, "ZDr", 0, 0 );
// TraceLineS( "ZSubtask dtor Lock Mutex ZDr", "" );

   lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
// TraceLineI( "ZTask::DeleteSubtask Item Count: ", lItemCnt );

   // Delete sub-windows of this window.
   while ( lItemCnt > 0 )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
           (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED &&
           g_pSubtaskList[ lItemCnt ].lTID == lTID && g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == pZSubtask &&
           g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZTask == this )
      {
         g_pSubtaskList[ lItemCnt ].ulSubtaskState |= zSUBTASK_STATE_MARK_FOR_DELETE;
         SysMutexUnlock( pZSubtask->m_vDialog, "ZDr", 0 );
         DeleteSubtask( g_pSubtaskList[ lItemCnt ].pZSubtask );
         SysMutexLock( pZSubtask->m_vDialog, "ZDr", 0, 0 );
         lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      }
      else
      {
         lItemCnt--;
      }
   }

   lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
// TraceLineI( "ZTask::DeleteSubtask Item Count: ", lItemCnt );

   // Check to see if the current window was already deleted.
   while ( lItemCnt > 0 )
   {
      if ( g_pSubtaskList[ lItemCnt ].pZSubtask == pZSubtask )
      {
         if ( (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_DELETED) == zSUBTASK_STATE_DELETED )
         {
            lItemCnt = 0;
         }

         break;
      }

      lItemCnt--;
   }

// TraceLineS( "ZSubtask dtor Unlock Mutex ZDr", "" );
   SysMutexUnlock( pZSubtask->m_vDialog, "ZDr", 0 );

   if ( lItemCnt == 0 )  // it was removed from the list
      return;

   if ( (m_nTraceAction & 0x71) == 0x71 )
   {
#ifdef zREMOTE_SERVER
      zCHAR szMsg[ 128 ];

      sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] ZTask::DeleteSubtask: ",
                pZSubtask->m_pZSocketTask, AfxGetThread( ), pZSubtask->m_pZSocketTask->m_pWinThread, pZSubtask->m_pZSocketTask->m_sock );
#else
      TraceLine( "ZTask::DeleteSubtask: [0x%08x]  IdNbr: %d   %s.%s",
                 pZSubtask, pZSubtask->m_ulSubtaskIdNbr, *(pZSubtask->m_pzsDlgTag), *(pZSubtask->m_pzsWndTag) );
#endif
   }

   ulSubtaskState = ZSubtask::SetSubtaskState( pZSubtask, 0, zSUBTASK_STATE_DELETE );
   if ( (ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED )
   {
      if ( pZSubtask->m_pZFWnd )
      {
         if ( mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
         {
#ifdef zREMOTE_SERVER
            if ( pZSubtask->m_pZSocketTask->m_pZSubtaskTop == pZSubtask )
#else
            if ( m_pZSubtaskTop == pZSubtask )
#endif
            {
               if ( (pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_SPLITTER_PANE) )
               {
                  ASSERT( pZSubtask->m_pZView->m_pParentSplitterWnd );
                  ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
                  ZSplitterWnd *pSplitterWnd = pZSubtask->m_pZView->m_pParentSplitterWnd;
                  if ( pZSubtaskMain == pZSubtask && pSplitterWnd->m_pZSubtask2 &&
                       (ZSubtask::GetSubtaskState( pSplitterWnd->m_pZSubtask2 ) & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 &&
                       pSplitterWnd->m_pZSubtask2->m_pZFWnd )
                  {
                     // We need to swap the Frame windows for the subtasks so that
                     // the primary frame is not destroyed.
                     ZDrFrame *pZFWnd1 = (ZDrFrame *) pSplitterWnd->m_pZSubtask1->m_pZFWnd;
                     ZDrFrame *pZFWnd2 = (ZDrFrame *) pSplitterWnd->m_pZSubtask2->m_pZFWnd;
                     pSplitterWnd->m_pZSubtask1->m_pZFWnd = pZFWnd2;
                     pSplitterWnd->m_pZSubtask2->m_pZFWnd = pZFWnd1;
                     pZFWnd1->m_pZSubtask = pSplitterWnd->m_pZSubtask2;
                     pZFWnd2->m_pZSubtask = pSplitterWnd->m_pZSubtask1;
                     pZFWnd2->m_pSplitterWnd = pZFWnd1->m_pSplitterWnd;
                     pZFWnd1->m_pSplitterWnd = 0;

                     if ( pZSubtask == pZFWnd1->m_pZSubtask )
                        m_pZSubtaskTop = pZFWnd2->m_pZSubtask;
                     else
                        m_pZSubtaskTop = pZFWnd1->m_pZSubtask;

                     if ( (m_nTraceAction & 0x71) == 0x71 )
                     {
                        TraceLine( "ZTask::DeleteTopSubtask IdNbr: %d SWAP Frame1: %d Subtask: %d SplitWnd: %s <=> Frame2: %d Subtask: %d SplitWnd: %s",
                                   pZSubtask->m_ulSubtaskIdNbr, pZFWnd1->m_ulFrameIdNbr,
                                   pZFWnd1->m_pZSubtask->m_ulSubtaskIdNbr,
                                   pZFWnd1->m_pSplitterWnd ? *(pZFWnd1->m_pSplitterWnd->m_pcsSplitTag) : "null",
                                   pZFWnd2->m_ulFrameIdNbr,
                                   pZFWnd2->m_pZSubtask->m_ulSubtaskIdNbr,
                                   pZFWnd2->m_pSplitterWnd ? *(pZFWnd2->m_pSplitterWnd->m_pcsSplitTag) : "null" );
                     }
                  }
               }

               // If this was the main window and there are other windows left, we must find another main window to take this one's place.
               lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
               if ( m_pZSubtaskTop == pZSubtask )
               while ( lItemCnt )
               {
                  if ( g_pSubtaskList[ lItemCnt ].pZSubtask && (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_MARK_FOR_DELETE) == 0 )
                  {
                     if ( lTID == g_pSubtaskList[ lItemCnt ].lTID && pZSubtask == g_pSubtaskList[ lItemCnt ].pZSubtask )
                     {
                        MessageSend( pZSubtask->m_vDialog, 0, "ZTask::DeleteSubtask", "Unexpected subtask pointer", zMSGQ_MODAL_ERROR, FALSE );
                     }
                     else
                     if ( lTID == g_pSubtaskList[ lItemCnt ].lTID && g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == 0 )
                     {
#ifdef zREMOTE_SERVER
                        pZSubtask->m_pZSocketTask->m_pZSubtaskTop = g_pSubtaskList[ lItemCnt ].pZSubtask;
#else
                        if ( g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZFWnd == 0 )
                        {
                           MessageSend( pZSubtask->m_vDialog, 0, "ZTask::DeleteSubtask", "Unexpected null frame window", zMSGQ_MODAL_ERROR, FALSE );
                        }
                        else
                        {
                           m_pZSubtaskTop = g_pSubtaskList[ lItemCnt ].pZSubtask;
                           m_pZDrApp->m_pMainWnd = m_pZSubtaskTop->m_pZFWnd;
                           ::SetWindowLong( m_pZDrApp->m_hWndCommOE, GWL_USERDATA, (zLONG) m_pZDrApp->m_pMainWnd->m_hWnd );
#endif
                           break;
                        }
                     }
                  }

                  lItemCnt--;
               }
            }

            // Never explicitly call 'delete' on a CFrameWnd ... use DestroyWindow instead!
//          CFrameWnd *pWnd = pZSubtask->m_pZFWnd;
//          pZSubtask->m_pZFWnd->SendMessage( WM_CLOSE );
            pZSubtask->m_pZFWnd->DestroyWindow( );
//          mDeleteInit( pZSubtask->m_pZFWnd );  // Cleanup will have been called
//          delete( pWnd );
         }
         else
         {
            mDeleteInit( pZSubtask->m_pZFWnd );
            delete( pZSubtask );             // Cleanup will NOT have been called
         }
      }
      else
      {
         delete( pZSubtask );
      }
   }

   if ( pZSubtaskOwner )
   {
      // Protect this with a semaphore!!!
   // SysMutexLock( vSubtask, "ZDr", 0, 0 );
   // TraceLineS( "ZSubtask dtor Lock Mutex ZDr", "" );

      lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   // TraceLineI( "ZTask::DeleteSubtask Item Count: ", lItemCnt );

      while ( lItemCnt > 0 )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask == pZSubtaskOwner &&
              (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED &&
              g_pSubtaskList[ lItemCnt ].lTID == lTID &&
              g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZParent == pZSubtask &&
              g_pSubtaskList[ lItemCnt ].pZSubtask->m_pZTask == this )
         {
            if ( pZSubtaskOwner->m_pZFWnd && mIs_hWnd( pZSubtaskOwner->m_pZFWnd->m_hWnd ) )
            {
               ::ActivateWindow( pZSubtaskOwner->m_vDialog );
            }

            break;
         }
         else
         {
            lItemCnt--;
         }
      }

   // TraceLineS( "ZSubtask dtor Unlock Mutex ZDr", "" );
   // SysMutexUnlock( vSubtask, "ZDr", 0 );
   }

   if ( (m_nTraceAction & 0x71) == 0x71 )
   {
      lItemCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
      while ( lItemCnt > 0 )
      {
         if ( g_pSubtaskList[ lItemCnt ].pZSubtask &&
              (g_pSubtaskList[ lItemCnt ].ulSubtaskState & zSUBTASK_STATE_DELETED) != zSUBTASK_STATE_DELETED &&
              g_pSubtaskList[ lItemCnt ].lTID == lTID )
         {
            pZSubtask = g_pSubtaskList[ lItemCnt ].pZSubtask;
            if ( pZSubtask->m_pZView )
            {
               TraceLine( "ZTask::DeleteSubtask POST: Subtask: [0x%08x]  IdNbr: %d   FWnd %d  View: %d  Quad: %s  ParentSplitterView (0x%08x): %d",
                          pZSubtask, pZSubtask->m_ulSubtaskIdNbr,
                          ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_ulFrameIdNbr,
                          pZSubtask->m_pZView->m_ulViewIdNbr,
                          pZSubtask->m_pZView->m_eQuad == ZDrView::eTop ? "Top" :
                            pZSubtask->m_pZView->m_eQuad == ZDrView::eLeft ? "Left" :
                            pZSubtask->m_pZView->m_eQuad == ZDrView::eBottom ? "Bottom" :
                            pZSubtask->m_pZView->m_eQuad == ZDrView::eRight ? "Right" : "Undefined",
                          pZSubtask->m_pZView->m_pParentSplitterView ? pZSubtask->m_pZView->m_pParentSplitterView : 0,
                          pZSubtask->m_pZView->m_pParentSplitterView ? pZSubtask->m_pZView->m_pParentSplitterView->m_ulIdNbr : 0 );
            }
            else
            {
               TraceLine( "ZTask::DeleteSubtask POST: Subtask: [0x%08x]  IdNbr: %d  FrameIdNbr %d  (Null View)",
                          pZSubtask, pZSubtask->m_ulSubtaskIdNbr, ((ZDrFrame *) pZSubtask->m_pZFWnd)->m_ulFrameIdNbr );
            }
         }

         lItemCnt--;
      }
   }
}

ZSubtask *
ZTask::FindSubtaskOnSplitterDiscardPile( zCPCHAR cpcSplitTag,
                                         zCPCHAR cpcDlgTag,
                                         zCPCHAR cpcWndTag )
{
   ZSubtask *pZSubtask;
   ZNumberItem *pNext = m_pSplitterDiscardHead;
   ZNumberItem *pPrev = 0;

   while ( pNext )
   {
      pZSubtask = (ZSubtask *) pNext->m_lNumber;
      if ( zstrcmp( pZSubtask->m_csSplitTag.GetString(), cpcSplitTag ) == 0 &&
           zstrcmp( (*pZSubtask->m_pzsDlgTag).GetString(), cpcDlgTag ) == 0 &&
           zstrcmp( (*pZSubtask->m_pzsWndTag).GetString(), cpcWndTag ) == 0 )
      {
         if ( pPrev )  // not at the head of list
            pPrev->m_pNextNumber = pNext->m_pNextNumber;

         pNext->m_pNextNumber = 0;
         delete( pNext );
         return( pZSubtask );
      }

      pPrev = pNext;
      pNext = pNext->m_pNextNumber;
   }

   return( 0 );
}

void
ZTask::PutSubtaskOnSplitterDiscardPile( ZSubtask *pZSubtask )
{
   delete( pZSubtask );
   return;
   ZNumberItem *pItem = new ZNumberItem( (zLONG) pZSubtask );

   if ( m_pSplitterDiscardHead )
      pItem->m_pNextNumber = m_pSplitterDiscardHead;

   m_pSplitterDiscardHead = pItem;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static unsigned char masktable[ ] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

void
Create24Bit( ZDib2& source, ZDib2& dest )
{
   int pal;
   int k;

   dest.Create( source.Width( ), source.Height( ), 24 );
   pal = source.GetPaletteSize( );

   BYTE palet[ 768 ];

   for ( k = 0; k < pal; k++ )
   {
      COLORREF col = source.PaletteColor( k );
      palet[ k * 3 + 2 ] = GetRValue( col );
      palet[ k * 3 + 1 ] = GetGValue( col );
      palet[ k * 3 + 0 ] = GetBValue( col );
   }

   int j;
   BYTE *src, *dst, *ptr;

   for ( k = 0; k < source.Height( ); k++ )
   {
      src = source.GetLinePtr( k );
      dst = dest.GetLinePtr( k );
      ptr = dst;
      int index;
      for ( j = 0; j < source.Width( ); j++, ptr += 3 )
      {
         switch ( pal )
         {
            case 2:
               if ( src[ j >> 3 ] & masktable[ j & 7 ] )
               {
                  index = 1;
               }
               else
               {
                  index = 0;
               }

               break;

            case 16:
               if ( j & 1 )
               {
                  index = src[ j >> 1 ] & 0x0f;
               }
               else
               {
                  index = (src[ j >> 1 ] >> 4) & 0x0f;
               }

               break;

            case 256:
               index = src[ j ];
               break;
         }

         ASSERT( index < pal );
         memcpy( ptr, palet + index * 3, 3 );
      }

      index = (ptr - dst) / 3;
      ASSERT( index <= source.Width( ) );
   }
}

void
ZTask::SetImage( HANDLE hBitmap, zUSHORT uBits )
{
   ZDib2 dib;
// if ( uBits == 24 )
   {
      dib.CreateFromHandle( hBitmap, uBits );
   }
// else
// {
//    ZDib2 dib2;
//    dib2.CreateFromHandle( hBitmap, uBits );
//    Create24Bit( dib2, dib );
// }

   dib.SaveBMP( *m_pzsTwainFileName );

   if ( m_pZFWnd && ((ZDrFrame *) m_pZFWnd)->m_pZSubtask &&
        (((ZDrFrame *) m_pZFWnd)->m_pZSubtask->m_ulWndOper & zWND_EVENT_ON_ACQUIRE) == zWND_EVENT_ON_ACQUIRE &&
        SetCursorFirstEntityByInteger( ((ZDrFrame *) m_pZFWnd)->m_pZSubtask->m_vDialog,
                                       szlWndEvent, szlType, zWND_EVENT_ON_ACQUIRE, 0 ) > zCURSOR_UNCHANGED )
   {
      if ( ProcessActionLoop( ((ZDrFrame *) m_pZFWnd)->m_pZSubtask, szlWndAct, 0, 0, zACTION_DIALOG_CODE_ONLY, TRUE, "OnAcquire" ) > 0 )
      {
         return;
      }
   }
}

void
ZTask::CopyImage( HANDLE hBitmap, TW_IMAGEINFO& info )
{
   SetImage( hBitmap, info.BitsPerPixel );
}

void
ZTask::AddApplicationDLL( ZSubtask *pZSubtask, zCPCHAR pchDLL_Name )
{
   CString zsDLL( pchDLL_Name );
   zsDLL.MakeUpper( );
   if ( m_DLL_InitList.AddTail( zsDLL ) )
   {
      LPLIBRARY hCCtlLibrary = 0;  // set to 0 for GetOperationDynamicCallAddress

      CString zs( "ZeidonInit" );
      zs += zsDLL;

      zFARPROC_INIT lpfnInitProc = (zFARPROC_INIT) GetOperationDynamicCallAddress( pZSubtask->m_vDialog,
                                                                                   (LPLPLIBRARY) &hCCtlLibrary,
                                                                                   zsDLL, zs, "(drvr) Init" );
      if ( lpfnInitProc )
      {
         (*lpfnInitProc)( pZSubtask->m_vDialog, g_lDlgUnitCharX, g_lDlgUnitCharY, g_lMapDlgCharX, g_lMapDlgCharY );
         if ( m_nTraceAction > 2 )
            TraceLineS( "Application DLL Initialization completed: ", zs );
      }
      else
   // if ( m_nTraceAction )
         TraceLineS( "Could NOT initialize Application DLL: ", zs );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef zREMOTE_SERVER

zLONG
ZTask::GetSocketTaskCount( )
{
   SysMutexLock( vSubtask, "ZDr", 0, 0 );
   zLONG lCnt = (zLONG) g_pSubtaskList[ 0 ].pZSubtask;
   SysMutexUnlock( vSubtask, "ZDr", 0 );
   return( lCnt );
}

struct ErrorMessageStruct g_ErrMsgs[] =
{
   WSAEINTR,              "WSAEINTR",
   WSAEBADF,              "WSAEBADF",
   WSAEACCES,             "WSAEACCES",
   WSAEFAULT,             "WSAEFAULT",
   WSAEINVAL,             "WSAEINVAL",
   WSAEMFILE,             "WSAEMFILE",
   WSAEWOULDBLOCK,        "WSAEWOULDBLOCK",
   WSAEINPROGRESS,        "WSAEINPROGRESS",
   WSAEALREADY,           "WSAEALREADY",
   WSAENOTSOCK,           "WSAENOTSOCK",
   WSAEDESTADDRREQ,       "WSAEDESTADDRREQ",
   WSAEMSGSIZE,           "WSAEMSGSIZE",
   WSAEPROTOTYPE,         "WSAEPROTOTYPE",
   WSAENOPROTOOPT,        "WSAENOPROTOOPT",
   WSAEPROTONOSUPPORT,    "WSAEPROTONOSUPPORT",
   WSAESOCKTNOSUPPORT,    "WSAESOCKTNOSUPPORT",
   WSAEOPNOTSUPP,         "WSAEOPNOTSUPP",
   WSAEPFNOSUPPORT,       "WSAEPFNOSUPPORT",
   WSAEAFNOSUPPORT,       "WSAEAFNOSUPPORT",
   WSAEADDRINUSE,         "WSAEADDRINUSE",
   WSAEADDRNOTAVAIL,      "WSAEADDRNOTAVAIL",
   WSAENETDOWN,           "WSAENETDOWN",
   WSAENETUNREACH,        "WSAENETUNREACH",
   WSAENETRESET,          "WSAENETRESET",
   WSAECONNABORTED,       "WSAECONNABORTED",
   WSAECONNRESET,         "WSAECONNRESET",
   WSAENOBUFS,            "WSAENOBUFS",
   WSAEISCONN,            "WSAEISCONN",
   WSAENOTCONN,           "WSAENOTCONN",
   WSAESHUTDOWN,          "WSAESHUTDOWN",
   WSAETOOMANYREFS,       "WSAETOOMANYREFS",
   WSAETIMEDOUT,          "WSAETIMEDOUT",
   WSAECONNREFUSED,       "WSAECONNREFUSED",
   WSAELOOP,              "WSAELOOP",
   WSAENAMETOOLONG,       "WSAENAMETOOLONG",
   WSAEHOSTDOWN,          "WSAEHOSTDOWN",
   WSAEHOSTUNREACH,       "WSAEHOSTUNREACH",
   WSAENOTEMPTY,          "WSAENOTEMPTY",
   WSAEPROCLIM,           "WSAEPROCLIM",
   WSAEUSERS,             "WSAEUSERS",
   WSAEDQUOT,             "WSAEDQUOT",
   WSAESTALE,             "WSAESTALE",
   WSAEREMOTE,            "WSAEREMOTE",
   WSASYSNOTREADY,        "WSASYSNOTREADY",
   WSAVERNOTSUPPORTED,    "WSAVERNOTSUPPORTED",
   WSANOTINITIALISED,     "WSANOTINITIALISED",
   0,                     0
};

void
PrintHelp( )
{
   TraceLineS( "echosrvr: echo server", "" );
}

void
fnShowError( zPCHAR pszFuncName )
{
   int    iErr = WSAGetLastError();
   zSHORT k;
   zCHAR  szMsg[ 400 ];

   for ( k = 0; g_ErrMsgs[ k ].pszErrorMessage; k++ )
   {
      if ( g_ErrMsgs[ k ].iErrorNum == iErr )
         break;
   }

   if ( g_ErrMsgs[ k ].pszErrorMessage )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "(zwinsock) WinSock error Func = %s %s %d", pszFuncName, g_ErrMsgs[ k ].pszErrorMessage, iErr );
   }
   else
   {
      sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "(zwinsock) Unknown WinSock error Func = %s %d", pszFuncName, iErr );
   }

   TraceLineS( szMsg, "" );

} // fnShowError

void
fnTransferMessages( zVIEW vXRA, zVIEW vSubtask )
{
   zVIEW   vMsg;
   zSHORT  nRC;

   GetViewByName( &vMsg, "__MSGQ", vSubtask, zLEVEL_TASK );
   if ( vMsg == 0 )
      return;

   if ( (nRC = SetCursorFirstEntity( vMsg, "QMsg", 0 )) >= zCURSOR_SET )
   {
      zLONG  lMsgType;
      zBOOL  bAbort = FALSE;
      zSHORT nMsgCnt = 0;

      while ( nRC >= zCURSOR_SET )
      {
         nMsgCnt++;
         GetIntegerFromAttribute( &lMsgType, vMsg, "QMsg", "Type" );
         if ( lMsgType > 500 )  // Messages over 500 should only be system errors.
         {
            bAbort = TRUE;
            break;
         }

         nRC = SetCursorNextEntity( vMsg, "QMsg", 0 );
      }

      if ( nMsgCnt > 1 )  // || bAbort
      {
         // Put out only a single "something's terribly wrong" message.
         while ( SetCursorFirstEntity( vMsg, "QMsg", 0 ) >= zCURSOR_SET )
            DeleteEntity( vMsg, "QMsg", zREPOS_NONE );  // clean up each message

         OperatorPrompt( vSubtask, "Application Unavailable", "Please contact support", FALSE, zBUTTONS_OK, zRESPONSE_OK, zICON_EXCLAMATION );
      // AbortTask( "Task terminated" );
      }
      else
      {
         zLONG   lMsgTag;
         zLONG   lBeep;
         CString csTag;
         zPCHAR  pch;

         CreateEntity( vXRA, "MsgBox", zPOS_LAST );
         GetAddrForAttribute( &pch, vMsg, "QMsg", "Title" );
         if ( *pch == 0 )
         {
            GetAddrForAttribute( &pch, vMsg, "QMsg", "Id" );
            if ( *pch == 0 )
               GetAddrForAttribute( &pch, vMsg, "QMsg", "Text" );
         }

         SysMutexLock( vSubtask, "ZDm", 0, 0 );
         g_lMsgTag++;
         lMsgTag = g_lMsgTag;
         SysMutexUnlock( vSubtask, "ZDm", 0 );

         csTag = pch;
         csTag += ':';
         zCHAR szTemp[ 16 ];
         _ltoa_s( lMsgTag, szTemp, zsizeof( szTemp ), 10 );
         csTag += szTemp;
         SetAttributeFromString( vXRA, "MsgBox", "Tag", csTag );

         SetAttributeFromAttribute( vXRA, "MsgBox", "Text", vMsg, "QMsg", "Text" );
         GetIntegerFromAttribute( &lBeep, vMsg, "QMsg", "Beep" );
         if ( lBeep )
            SetAttributeFromString( vXRA, "MsgBox", "Beep", "Y" );
         else
         {
            // Null attribute if Beep is 0.
            SetAttributeFromString( vXRA, "MsgBox", "Beep", "" );
         }

         SetAttributeFromString( vXRA, "MsgBox", "OK", "Y" );

         // Clean up this message.
         DropEntity( vMsg, "QMsg", zREPOS_NONE );
      }
   }
}

zSHORT OPERATION
fnSend( zULONG lControl,
        zPCHAR pchText,
        zULONG uLth,
        zSHORT nLevel,
        zPVOID lpData )
{
   zLONG  lBytesToSend;
   zLONG  lBytesSent;
   int    nRC;
   LPCONN lpConn = (LPCONN) lpData;

   if ( pchText && uLth == 0 )
      uLth = zstrlen( pchText );

   // Check to see if we need to flush the current buffer.  If pchText is 0 then user wants a flush.
   if ( pchText == 0 || lpConn->ulBufferLth + uLth > zWEB_COMMUNICATION_BUFFER_SIZE )
   {
      zCHAR szMsg[ 32 ];

      do
      {
         if ( (m_nTraceAction & 0x71) == 0x71 )
         {
         // TraceLineS( "Perfoming send()", "" );
            TraceBlob( "fnSend Sending buffer lth: ", lpConn->pszBuffer, lpConn->ulBufferLth );
         }

         sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "%05d/%c", (int) lpConn->ulBufferLth, pchText ? 'C' : '0' );
         lBytesToSend = zstrlen( szMsg );
      // TraceLineS( "==> ", szMsg );
         lBytesSent = 0;
         do
         {
            if ( lpConn->lFileHandle != -1 )
               SysWriteFile( lpConn->lFileHandle, szMsg + lBytesSent, lBytesToSend - lBytesSent );

            nRC = send( lpConn->sock, szMsg + lBytesSent, lBytesToSend - lBytesSent, 0 );
            if ( lBytesSent == SOCKET_ERROR )
            {
               fnShowError( "Sockets Error in send( )" );
               TraceLineS( "Sockets Error: ", "Error in send()" );
               return( zCALL_ERROR );
            }

            lBytesSent += nRC;

         } while ( lBytesToSend > lBytesSent );

         lBytesToSend = (zLONG) lpConn->ulBufferLth;
         lBytesSent = 0;
         do
         {
            if ( lpConn->lFileHandle != -1 )
               SysWriteFile( lpConn->lFileHandle, lpConn->pszBuffer + lBytesSent, lBytesToSend - lBytesSent );

            lBytesSent += send( lpConn->sock, lpConn->pszBuffer + lBytesSent, lBytesToSend - lBytesSent, 0 );
            if ( lBytesSent == SOCKET_ERROR )
            {
               TraceLineS( "Sockets Error: ", "Error in send()" );
               return( zCALL_ERROR );
            }

         } while ( lBytesToSend > lBytesSent );

         lpConn->ulBufferLth = 0;

         if ( uLth <= zWEB_COMMUNICATION_BUFFER_SIZE )
            break;

         zmemcpy( lpConn->pszBuffer, pchText, zWEB_COMMUNICATION_BUFFER_SIZE );
         uLth -= zWEB_COMMUNICATION_BUFFER_SIZE;
         pchText += zWEB_COMMUNICATION_BUFFER_SIZE;
         lpConn->ulBufferLth = zWEB_COMMUNICATION_BUFFER_SIZE;

      } while ( TRUE );

      // If pchText is 0, then the user only wanted a flush so get out.
      if ( pchText == 0 )
         return( 0 );
   }

   zmemcpy( lpConn->pszBuffer + lpConn->ulBufferLth, pchText, uLth );
   lpConn->ulBufferLth += uLth;

   return( 0 );

} // fnSend

zSHORT OPERATION
fnRead( zPPCHAR ppchReturnBuffer,
        zPULONG pulReturnLth,
        zPVOID  lpData )
{
   LPCONN lpConn = (LPCONN) lpData;
   zLONG  lBytesToRead;
   zLONG  lBytesRead;
   zBOOL  bDone;

   if ( (m_nTraceAction & 0x71) == 0x71 )
      TraceLineS( "Calling fnRead()", "" );

   // No matter what happens we are always returning the buffer address.
   *ppchReturnBuffer = lpConn->pszBuffer;

   // Check to see if we need to read the size of the block coming in.
   if ( lpConn->ulBlockLth == 0 && lpConn->cMore != '0' )
   {
      zCHAR szBlockLth[ 10 ];

      // Get the block length.
      lBytesRead = recv( lpConn->sock, szBlockLth, 7, 0 );

      szBlockLth[ 7 ] = 0;
      lpConn->ulBlockLth = (zULONG) zatol( szBlockLth );
      lpConn->cMore = szBlockLth[ 6 ];

      if ( (m_nTraceAction & 0x71) == 0x71 )
      {
         zCHAR szMsg[ 256 ];
         sprintf_s( szMsg, zsizeof( szMsg ), zsizeof( szMsg ), "fnRead Received block lth [%s] = %ld More: '%c' BytesRead: ",
                    szBlockLth, lpConn->ulBlockLth, lpConn->cMore );
         TraceBlob( szMsg, lpConn->pszBuffer, lpConn->ulBlockLth );
      }
   }

   // If the block lth is *still* 0 then there's no more data for the XML object.  Set appropriate flags and get out.
   if ( lpConn->ulBlockLth == 0 )
   {
      *pulReturnLth = 0;
      return( TRUE );      // This means we're done.
   }

   if ( lpConn->ulBlockLth > zWEB_COMMUNICATION_BUFFER_SIZE )
   {
      // The block size is bigger than the buffer so fill up the buffer and set the bDone flag off because there is more data in the block.
      lBytesToRead = zWEB_COMMUNICATION_BUFFER_SIZE;
      bDone = FALSE;
   }
   else
   {
      // The block is smaller than the buffer.  Read the rest of the block and then set bDone according to the value of cMore.
      lBytesToRead = lpConn->ulBlockLth;
      bDone = (lpConn->cMore != 'C');
   }

   lBytesRead = recv( lpConn->sock, lpConn->pszBuffer, lBytesToRead, 0 );
// TraceLineI( "fnRead BytesRead: ", lBytesRead );
   if ( lBytesRead < 0 )
   {
      fnShowError( "fnRead()" );

      // No more bytes.
      *pulReturnLth = 0;
      return( TRUE );
   }

   // If we didn't read all the bytes we thought we would read then we aren't done so set flag.
   if ( lBytesRead < lBytesToRead )
      bDone = FALSE;

   lpConn->ulBlockLth -= lBytesRead;

   // TraceBuffer( "Buffer read: ", lpConn->pszBuffer, lBytesRead );

   *pulReturnLth = lBytesRead;
   return( bDone );
}

zOPER_EXPORT zSHORT
fnSendReceiveXML( ZSocketTask *pZSocketTask,
                  zPCHAR pchBuffer )
{
   CString csText;
   zSHORT  nRC = 0;

   if ( (m_nTraceAction & 0x71) == 0x71 )
   {
      csText.Format( TEXT( "[S%ld.T%ld.t%ld.s%ld]: " ), (zLONG) pZSocketTask,  AfxGetThread( ),
                     (zLONG) pZSocketTask->m_pWinThread, (zLONG) pZSocketTask->m_sock );
   }

// ZDxFrame *pFrame = (ZDxFrame *) AfxGetMainWnd( );
   ConnectionRecord  cr;

   zmemset( &cr, 0, zsizeof( ConnectionRecord ) );
   cr.pszBuffer = pchBuffer;
   cr.sock = pZSocketTask->m_sock;
   cr.lFileHandle = -1;
   cr.cMore = 0;

#ifdef _DEBUG
   CFile *f = NULL;
   zCHAR szTempDir[MAX_PATH];
   zCHAR szXRAFile[MAX_PATH];
   zCHAR szExtension[MAX_PATH];
   int nExtensionPos;

   CString strFileName;

   zmemset( szTempDir, 0, MAX_PATH );
   zmemset( szXRAFile, 0, MAX_PATH );
   zmemset( szExtension, 0, MAX_PATH );

   SysReadZeidonIni( -1, "[Workstation]", "XMLTraceFile", szXRAFile );

   if ( szXRAFile[ 0 ] )
   {
      strFileName = szXRAFile;
      nExtensionPos = strFileName.ReverseFind( '.' );
      if ( nExtensionPos > -1 )
      {
         strcpy_s( szExtension, zsizeof( szExtension ), ((LPCSTR( strFileName )) + nExtensionPos ) );
         szXRAFile[ nExtensionPos ] = 0;
      }
      else
      {
         strcpy_s( szExtension, zsizeof( szExtension ), ".XML" );
      }

      if ( ::GetEnvironmentVariable( "TEMP", szTempDir, MAX_PATH ) == 0 )
      {
         strFileName.Format( "%s%s%d%s","c:\\", szXRAFile, lXMLTraceCount, szExtension );
      }
      else
      {
         strFileName.Format( "%s%s%s%d%s",szTempDir, "\\", szXRAFile, lXMLTraceCount, szExtension );
      }

      f = new CFile( strFileName, CFile::modeCreate | CFile::modeWrite );
      lXMLTraceCount++;
      cr.lFileHandle = f->m_hFile;
   }
#endif

// pFrame->SendMessage( zZEIDON_REMOTE_SOCKET, 2, (LPARAM) pZSocketTask );
   pZSocketTask->SendZDxFrameMessage( 2 );

   if ( (m_nTraceAction & 0x71) == 0x71 )
      TraceLineS( csText, "Sending XRA" );

   ZXRA_Item *pXRA = pZSocketTask->m_pCacheWndList->Find( pZSocketTask->m_szRemoteWndTag, 0, "fnSendReceiveXML" );

   if ( pXRA && pXRA->m_pZSubtask )
      fnTransferMessages( pXRA->m_vXRA_Delta, pXRA->m_pZSubtask->m_vDialog );

   zSHORT nViewCnt = pZSocketTask->m_pCacheWndList->CountViews( 0 );
   if ( nViewCnt )
   {
      zVIEW *pvOI = (zVIEW *) new char[ nViewCnt * sizeof( zVIEW ) ];
      pZSocketTask->m_pCacheWndList->CountViews( pvOI );
      nRC = WriteMultipleOIs_ToXML( pvOI, nViewCnt, zXML_ZEIDONINFO, // | zXML_INCREFLAGS,
                                    fnSend, (zPVOID) &cr );
      delete [] pvOI;
   }

   if ( nRC < 0 )
      return( nRC );

   // Flush the buffer.
   nRC = fnSend( 0, 0, 0, 0, (zPVOID) &cr );

#ifdef _DEBUG
   mDeleteInit( f );
   cr.lFileHandle = -1;
#endif

   if ( nRC < 0 )
      return( nRC );

// SysCloseFile( cr.lFileHandle, 0 );
   ASSERT( cr.lFileHandle == -1 );

   pZSocketTask->m_pCacheWndList->DropObjectInstances( );

   // Now try getting the returned object.
// pFrame->SendMessage( zZEIDON_REMOTE_SOCKET, 4, (LPARAM) pZSocketTask );
   pZSocketTask->SendZDxFrameMessage( 4 );

   if ( (m_nTraceAction & 0x71) == 0x71 )
      TraceLineS( csText, "Waiting for XRA ..." );

   // Reset connection record.
   zmemset( &cr, 0, zsizeof( ConnectionRecord ) );
   cr.pszBuffer = pchBuffer;
   cr.sock      = pZSocketTask->m_sock;
   cr.cMore = 0;
   pchBuffer[ 0 ] = 0;
   nRC = ActivateOI_FromXML( &(pZSocketTask->m_vXRA_State), "KZXRA", pZSocketTask->m_vRegisteredSubtask, 0, fnRead, &cr );
   if ( (m_nTraceAction & 0x71) == 0x71 &&
        nRC != zCALL_ERROR )
   {
      CString cs = *(pZSocketTask->m_szAppTag);
      cs += *(pZSocketTask->m_szRemoteWndTag);
      cs += "__XRA_State";
      SetNameForView( pZSocketTask->m_vXRA_State, cs, pZSocketTask->m_vSubtask, zLEVEL_TASK );
   }

   pZSocketTask->SendZDxFrameMessage( 8 );

   if ( (m_nTraceAction & 0x71) == 0x71 )
   {
      TraceLineS( csText, "Received XRA ..." );
      TraceLineS( "Received XRA ", "..." );
   }

// pFrame->SendMessage( zZEIDON_REMOTE_SOCKET, 8, (LPARAM) pZSocketTask );
   pZSocketTask->SendZDxFrameMessage( 16 );

   if ( (m_nTraceAction & 0x71) == 0x71 )
      TraceLineS( csText, "Received XRA Idle ..." );

   return( nRC );
}

void
fnGetXML_Value( zPCHAR pchTgt, zPCHAR pchSrc, zPCHAR pchEndTag )
{
   zPCHAR pchStart;
   zPCHAR pchEnd;

   pchStart = zstrchr( pchSrc, '>' ) + 1;
   pchEnd = zstrstr( pchSrc, pchEndTag );
   if ( pchStart && pchEnd )
   {
      pchEnd[ 0 ] = 0;   // set end end of string character for easy copy
      strcpy_s( pchTgt, ?, pchStart );
      pchEnd[ 0 ] = pchEndTag[ 0 ]; // restore original value
   }
   else
   {
      pchTgt[ 0 ] = 0;
   }
}

#endif   // #ifdef zREMOTE_SERVER
