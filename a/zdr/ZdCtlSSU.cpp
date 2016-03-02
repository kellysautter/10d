/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlssu.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Grid control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2003.03.03    DKS   Z10
//    New Grid implementation (without ActiveX).
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlSS.h"
#include "ZdCtlGbl.h"

// OLE stuff for clipboard operations
#include <afxadv.h>         // For CSharedFile
#include <afxconv.h>        // For LPTSTR -> LPSTR macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[ ] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// GridCtrl.cpp : implementation file
//
// MFC Grid Control v2.24
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// The code contained in this file was based on the original
// WorldCom Grid control written by Joe Willcoxson,
//      mailto:chinajoe@aol.com
//      http://users.aol.com/chinajoe
// (These addresses may be out of date) The code has gone through
// so many modifications that I'm not sure if there is even a single
// original line of code. In any case Joe's code was a great
// framework on which to build.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
//
// Please use and enjoy, and let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into
// this file.
//
//  History:
//  --------
//  This control is constantly evolving, sometimes due to new features that I
//  feel are necessary, and sometimes due to existing bugs. Where possible I
//  have credited the changes to those who contributed code corrections or
//  enhancements (names in brackets) or code suggestions (suggested by...)
//
//        1.0 - 1.13   20 Feb 1998 - 6 May 1999
//                    First release version. Progressed from being a basic
//                    grid based on the original WorldCom Grid control
//                    written by Joe Willcoxson (mailto:chinajoe@aol.com,
//                    http://users.aol.com/chinajoe) to something a little
//                    more feature rich. Rewritten so many times I doubt
//                    there is a single line of Joe's code left. Many, many,
//                    MANY people sent in bug reports and fixes. Thank you
//                    all.
//
//        2.0       1 Feb 2000
//                    Rewritten to make the grid more object oriented, in
//                    that the ZGridCell class now takes care of cell-specific
//                    tasks. This makes the code more robust, but more
//                    importantly it allows the simple insertion of other
//                    types of cells.
//
//        2.10      11 Mar 2000 - Ken Bertelson and Chris Maunder
//                    - Additions for virtual ZGridCell support of embedded tree
//                      & cell buttons implementation
//                    - Optional WYSIWYG printing
//                    - Awareness of hidden (0 width/height) rows and columns for
//                      key movements,  cut, copy, paste, and autosizing
//                    - ZGridCell can make title tips display any text rather than
//                      cell text only
//                    - Minor vis bug fixes
//                    - ZGrid now works with ZGridCellBase instead of ZGridCell
//                      This is a taste of things to come.
//
//        2.20      30 Jul 2000 - Chris Maunder
//                    - Font storage optimised (suggested by Martin Richter)
//                    - AutoSizeColumn works on either column header, data or both
//                    - EnsureVisible. The saga continues... (Ken)
//                    - Rewrote exception handling
//                    - Added TrackFocusCell and FrameFocusCell properties, as well as
//                      ExpandLastColumn (suggested by Bruce E. Stemplewski).
//                    - InsertColumn now allows you to insert columns at the end of the
//                      column range (David Weibel)
//                    - Shift-cell-selection more intuitive
//                    - API change: Set/GetGridColor now Set/GetGridLineColor
//                    - API change: Set/GetBkColor now Set/GetGridBkColor
//                    - API change: Set/GetTextColor, Set/GetTextBkColor depricated
//                    - API change: Set/GetFixedTextColor, Set/GetFixedBkColor depricated
//                    - Stupid DDX_GridControl workaround removed.
//                    - Added "virtual mode" via Set/GetVirtualMode
//                    - Added SetCallbackFunc to allow callback functions in virtual mode
//                    - Added Set/GetAutoSizeStyle
//                    - AutoSize() bug fixed
//                    - added GVIS_FIXEDROW, GVIS_FIXEDCOL states
//                    - added Get/SetFixed[Row|Column]Selection
//                    - cell "Get" methods now const'd. Sorry folks...
//                    - GetMouseScrollLines now uses win98/W2K friendly code
//                    - WS_EX_CLIENTEDGE style now implicit
//
//                    [Only the latest version and major version changes will be shown]
//
//        2.24      13 Jul 2002 - Chris Maunder
//                    - Optimised OnDraw for invisible cells.
//
// TODO: 1) Implement sparse grids (super easy now)
//       2) Fix it so that as you drag select, the speed of selection
//         increases with time.
//       3) Scrolling is still a little dodgy (too much grey area). I
//         know there is a simple fix but it's been a low priority.
//       4) Get some sleep
//
// ISSUES: 2) WindowFromPoint seems to do weird things in W2K. Causing
//         problems for the right-click-on-titletip code.
//
/////////////////////////////////////////////////////////////////////////////

#if defined( _WIN32_WCE ) && ( _WIN32_WCE <= 0x200 )
#define MF_DISABLED 0
#define DT_END_ELLIPSIS 0
#endif

// Spit out some messages as a sanity check for programmers
#ifdef GRIDCONTROL_NO_TITLETIPS
#pragma message( " -- ZGrid: No titletips for cells with large data" )
#endif
#ifdef GRIDCONTROL_NO_DRAGDROP
#pragma message( " -- ZGrid: No OLE drag and drop" )
#endif
#ifdef GRIDCONTROL_NO_CLIPBOARD
#pragma message( " -- ZGrid: No clipboard support" )
//#else
//#pragma message(" -- ZGrid: Clipboard support enabled for Grid")
#endif
#ifdef GRIDCONTROL_NO_PRINTING
#pragma message( " -- ZGrid: No printing support" )
#endif

// IMPLEMENT_DYNCREATE( ZGrid, CWnd )

// Get the number of lines to scroll with each mouse wheel notch
// Why doesn't windows give us this function???
UINT
GetMouseScrollLines( )
{
   int nScrollLines = 3;         // reasonable default

#ifndef _WIN32_WCE
   // Do things the hard way in win95
   OSVERSIONINFO VersionInfo;
   VersionInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
   if ( GetVersionEx( &VersionInfo ) == 0 ||
        (VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS &&
        VersionInfo.dwMinorVersion == 0) )
   {
      HKEY hKey;
      if ( RegOpenKeyEx( HKEY_CURRENT_USER,  _T( "Control Panel\\Desktop" ),
                         0, KEY_QUERY_VALUE, &hKey ) == ERROR_SUCCESS )
      {
         TCHAR szData[ 128 ];
         DWORD dwKeyDataType;
         DWORD dwDataBufSize = zsizeof( szData );

         if ( RegQueryValueEx( hKey, _T( "WheelScrollLines" ), 0, &dwKeyDataType,
                               (LPBYTE) &szData, &dwDataBufSize ) == ERROR_SUCCESS )
         {
            nScrollLines = _tcstoul( szData, 0, 10 );
         }

         RegCloseKey( hKey );
      }
   }
   else  // win98 or greater
     SystemParametersInfo ( SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0 );
#endif

   return( nScrollLines );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0
   ON_NOTIFY_REFLECT( GVN_BEGINDRAG, OnBeginDrag )
   ON_NOTIFY_REFLECT( GVN_BEGINEDIT, OnBeginEdit )
   ON_NOTIFY_REFLECT( GVN_ENDEDIT, OnEndEdit )
   ON_NOTIFY_REFLECT( GVN_COLUMNCLICK, OnColClick )
   ON_NOTIFY_REFLECT( GVN_DELETEITEM, OnDeleteItem )
   ON_NOTIFY_REFLECT( GVN_SELCHANGING, OnSelChanging )
   ON_NOTIFY_REFLECT( GVN_SELCHANGED, OnSelChanged )
   ON_NOTIFY_REFLECT( GVN_GETDISPINFO, OnGetDispInfo )
   ON_NOTIFY_REFLECT( GVN_ODCACHEHINT, OnOdCacheHint )
#endif

void
ZGrid::CorrelateZeidonSelectToListSelect( )
{
   zVIEW vAppList;

   if ( m_vApp == 0 )
      return;

   CreateViewFromViewForTask( &vAppList, m_vApp, m_pZSubtask->m_vDialog );
   SetViewFromView( vAppList, m_vApp );  // to get SelectSets
   zSHORT nPrevSelSet = SetSelectSetForView( vAppList, m_nSelectSelSet );
   SetAllSelectStatesForEntity( vAppList, *m_pzsEName, FALSE, *m_pzsScope );

   ZCellRange SelectedCellRange = GetSelectedCellRange( );
   zLONG lMinRow = SelectedCellRange.GetMinRow( );
   zLONG lMaxRow = SelectedCellRange.GetMaxRow( );

   for ( zLONG lRow = lMinRow; lRow <= lMaxRow; lRow++ )
   {
      zBOOL bCellSelected = FALSE;
      for ( int nCol = m_nFixedCols; nCol < m_nCols; nCol++ )
      {
         if ( IsCellSelected( lRow, nCol ) )
         {
            zLONG lKey = m_EntityKeyList.GetAt( lRow );
            SetEntityCursor( vAppList, *m_pzsEName, 0,
                             m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                             (zCPVOID) &lKey,
                             0, 0, 0, *m_pzsScope, 0 );
            SetSelectStateOfEntity( vAppList, *m_pzsEName, TRUE );
            break;  // out of inner loop
         }
      }
   }

   SetSelectSetForView( vAppList, nPrevSelSet );
// TraceLine( "CorrelateZeidonSelectToListSelect MinRow: %d  MaxRow: %d  Cell: %d,%d",
//            lMinRow, lMaxRow, m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );

   // Reset cursor to selected row.
   if ( m_cellIdCurrent.m_lRow >= lMinRow && m_cellIdCurrent.m_lRow <= lMaxRow )
   {
      zLONG lKey = m_EntityKeyList.GetAt( m_cellIdCurrent.m_lRow );
      SetEntityCursor( vAppList, *m_pzsEName, 0,
                       m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                       (zCPVOID) &lKey,
                       0, 0, 0, *m_pzsScope, 0 );
   }

   SetViewFromView( m_vApp, vAppList );  // to get SelectSets
   DropView( vAppList );
}

//void
//GridCursorListener( zVIEW v, zCPCHAR cpcEntityName, zPVOID pv )
//{
//   CString *pcs = (CString *) pv;
//
//   TraceLine( "CursorListener invoked for view: 0x%08x  entity: %s", v, cpcEntityName );
//   if ( pcs && zstrcmp( *pcs, cpcEntityName ) == 0 )
//      DisplayEntityInstance( v, cpcEntityName );
//}

LRESULT
ZGrid::RelayEvent( NM_GRIDVIEW& nmgv )
{
   zLONG lRC = 0;

   if ( nmgv.hdr.code == GVN_SELCHANGING )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent GVN_SELCHANGING  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }

      lRC = ProcessImmediateEvent( this, zGRDN_CELLSELECTCHANGING,
                                   &(nmgv.cell) );
      if ( (lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0) &&
           nmgv.cell.lRow != m_cellIdCurrent.m_lRow )
      {
         if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
            TraceLineS( "ZGrid::RelayEvent ", "ROWSELCHANGING" );

         lRC = ProcessImmediateEvent( this, zGRDN_ROWSELECTCHANGING,
                                      &(nmgv.cell) );
      }
   }
   else
   if ( nmgv.hdr.code == GVN_SELCHANGED )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent GVN_SELCHANGED Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }

      if ( m_cellIdCurrent.m_lRow >= 0 &&
           m_cellIdCurrent.m_lRow <= m_EntityKeyList.GetUpperBound( ) )
      {
         zULONG ulKey = m_EntityKeyList[ m_cellIdCurrent.m_lRow ];
         m_ulEntityKey = ulKey;
         if ( MiValidView( m_vApp ) && MiValidView( m_vAppList ) &&
              CheckEntityExistence( m_vAppList, *m_pzsEName ) == 0 &&
              (ulKey == GetEntityKey( m_vAppList, *m_pzsEName ) ||
               SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                                (zCPVOID) &ulKey,
                                0, 0, 0, *m_pzsScope, 0 ) >= zCURSOR_SET) )
         {
            if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
            {
               TraceLine( "ZGrid::RelayEvent CELLCHANGED vAppList Row: %d  Col: %d  Key: %d",
                          m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol, ulKey );
            // DisplayEntityInstance( m_vAppList, *m_pzsEName );
            }

            if ( ulKey == GetEntityKey( m_vApp, *m_pzsEName ) ||
                 (CheckEntityExistence( m_vApp, *m_pzsEName ) == 0 &&
                  SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                   m_ulFlag | zQUAL_ENTITYKEY | zPOS_FIRST,
                                   (zCPVOID) &ulKey,
                                   0, 0, 0, *m_pzsScope, 0 ) >= zCURSOR_SET) )
            {
               if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
               {
                  TraceLine( "ZGrid::RelayEvent zGRDN_CELLSELECTED vApp Row: %d  Col: %d  Key: %d",
                             m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol, ulKey );
               // DisplayEntityInstance( m_vApp, *m_pzsEName );
               }

            // AddCursorListener( m_vApp, GridCursorListener, m_pzsEName );
               CorrelateZeidonSelectToListSelect( );
               lRC = ProcessImmediateEvent( this, zGRDN_CELLSELECTED, &(nmgv.cell) );
            }

            if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
               EndEditing( );      // destroy any InPlaceEdit's
         }
      }
   }
   else
   if ( nmgv.hdr.code == NM_CLICK )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent CLICK  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }

      lRC = ProcessImmediateEvent( this, zGRDN_CLICK, &(nmgv.cell) );
   }
   else
   if ( nmgv.hdr.code == NM_DBLCLK )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent DBLCLK  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }

      lRC = ProcessImmediateEvent( this, zGRDN_DBLCLK, &(nmgv.cell) );
   }
   else
   if ( nmgv.hdr.code == GVN_COLUMNCLICK )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent COLUMNCLICK  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }

      lRC = ProcessImmediateEvent( this, zGRDN_COLUMNCLICK, &(nmgv.cell) );
   }
   else
   if ( nmgv.hdr.code == GVN_ODSTATECHANGED )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent CHECKBOXCHANGE  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }

      lRC = ProcessImmediateEvent( this, zGRDN_CHECKBOXCHANGE, &(nmgv.cell) );
   }
#if 0
   else
   if ( nmgv.hdr.code == GVN_DELETEITEM )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent DELETEITEM  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }
   }
   else
   if ( nmgv.hdr.code == GVN_BEGINDRAG )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent BEGINDRAG  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }
   }
   else
   if ( nmgv.hdr.code == GVN_GETDISPINFO )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent GETDISPINFO  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }
   }
   else
   if ( nmgv.hdr.code == GVN_ODCACHEHINT )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
      {
         TraceLine( "ZGrid::RelayEvent ODCACHEHINT  Row: %d  Col: %d",
                    m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      }
   }
#endif

   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
      lRC = 0;

   return( lRC );
}

#if 0 //???

void
ZGrid::OnUpdateEditCols( )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return;

   UpdateData( );

   zSHORT nOldNumCols = GetColumnCount( );

   TRY
   {
      SetColumnCount( m_nCols );
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return;
   }
   END_CATCH

   m_nCols = GetColumnCount( );
   m_nFixCols = GetFixedColumnCount( );
   UpdateData( FALSE );

   CString cs;
   for ( zSHORT k = nOldNumCols; k < m_nCols; k++ )
   {
       cs.Format( _T( "Column %d" ), k );
       SetItemText( 0, k, cs );
   }
}

void
ZGrid::OnUpdateEditFixcols( )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return;

   UpdateData( );

   TRY
   {
        SetFixedColumnCount( m_nFixCols );
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return;
   }
   END_CATCH

   m_nCols = GetColumnCount( );
   m_nFixCols = GetFixedColumnCount( );
   UpdateData( FALSE );
}

void
ZGrid::OnUpdateEditFixrows( )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return;

   UpdateData( );

   TRY
   {
      SetFixedRowCount( m_nFixRows );
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return;
   }
   END_CATCH

   m_lRows = GetRowCount( );
   m_nFixRows = GetFixedRowCount( );
   UpdateData( FALSE );
}

void
ZGrid::OnUpdateEditRows( )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return;

   UpdateData( );

   zLONG lOldNbrRows = GetRowCount( );

   TRY
   {
      SetRowCount( m_lRows );
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return;
   }
   END_CATCH

   m_lRows = GetRowCount( );
   m_nFixRows = GetFixedRowCount( );
   UpdateData( FALSE );

   CString cs;
   for ( zLONG k = lOldNbrRows; k < m_lRows; k++ )
   {
      cs.Format( _T( "Row %d" ), k );
      SetItemText( k, 0, cs );
   }

// EnsureVisible( m_lRows - 1, 0 );
}

void
ZGrid::OnPrintButton( )
{
#if !defined( WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )
   Print( );
#endif
}

void
ZGrid::OnFontButton( )
{
#ifndef _WIN32_WCE
   LOGFONT lf;
   GetFont( )->GetLogFont( &lf );

   CFontDialog dlg( &lf );
   if ( dlg.DoModal( ) == IDOK )
   {
      dlg.GetCurrentFont( &lf );

      CFont Font;
      Font.CreateFontIndirect( &lf );
      SetFont( &Font );
      OnItalics( );
      AutoSize( );
      Font.DeleteObject( );
   }
#endif
}

BOOL CALLBACK
EnumProc( HWND hwnd, LPARAM lParam )
{
   CWnd *pWnd = CWnd::FromHandle( hwnd );
   CSize *pTranslate = (CSize *) lParam;

   ZGrid *pDlg = (ZGrid *) pWnd->GetParent( );
   if ( pDlg == 0 )
      return( FALSE );

   CRect rect;
   pWnd->GetWindowRect( rect );
// if ( hwnd == pDlg->GetSafeHwnd( ) )
//    TRACE( _T( "Wnd rect: %d,%d - %d,%d\n" ),
//           rect.left, rect.top, rect.right, rect.bottom );

   pDlg->ScreenToClient( rect );
// if ( hwnd == pDlg->GetSafeHwnd( ) )
//    TRACE( _T( "Scr rect: %d,%d - %d,%d\n" ),
//           rect.left, rect.top, rect.right, rect.bottom );

   if ( hwnd == pDlg->GetSafeHwnd( ) )
   {
      if ( ((rect.top >= 7 && pTranslate->cy > 0) || rect.Height( ) > 20) &&
           ((rect.left >= 7 && pTranslate->cx > 0) || rect.Width( ) > 20) )
      {
         pWnd->MoveWindow( rect.left, rect.top,
                           rect.Width( ) + pTranslate->cx,
                           rect.Height( ) + pTranslate->cy, FALSE );
      }
      else
      {
         pWnd->MoveWindow( rect.left + pTranslate->cx,
                           rect.top + pTranslate->cy,
                           rect.Width( ), rect.Height( ), FALSE );
      }
   }
   else
   if ( hwnd == pDlg->m_TraceWnd.GetSafeHwnd( ) )
   {
      if ( ((rect.top >= 7 && pTranslate->cy > 0) || rect.Height( ) > 20) &&
           ((rect.left >= 7 && pTranslate->cx > 0) || rect.Width( ) > 20) )
      {
         pWnd->MoveWindow( rect.left, rect.top + pTranslate->cy,
                           rect.Width( ) + pTranslate->cx,
                           rect.Height( ), FALSE );
      }
      else
      {
         pWnd->MoveWindow( rect.left + pTranslate->cx,
                           rect.top + pTranslate->cy,
                           rect.Width( ), rect.Height( ), FALSE );
      }
   }
   else
   {
      if ( pWnd->GetDlgCtrlID( ) == IDC_SIZEBOX )
      {
         pWnd->MoveWindow( rect.left + pTranslate->cx,
                           rect.top + pTranslate->cy,
                           rect.Width( ), rect.Height( ), FALSE );
      }
      else
      {
         pWnd->MoveWindow( rect.left + pTranslate->cx, rect.top,
                           rect.Width( ), rect.Height( ), FALSE );
      }
   }

   pDlg->Invalidate( );

   return( TRUE );
}
#endif

#if 0

void
ZGrid::OnEditSelectall( )
{
   OnEditSelectAll( );
}

void
ZGrid::OnInsertRow( )
{
   zLONG lRow = GetFocusCell( ).m_lRow;
   if ( lRow >= 0 )
   {
      InsertRow( _T( "Newest Row" ), lRow );
      Invalidate( );
   }
}

void
ZGrid::OnDeleteRow( )
{
   zLONG lRow = GetFocusCell( ).m_lRow;
   if ( lRow >= 0 )
   {
      DeleteRow( lRow );
      Invalidate( );
   }
}

void
ZGrid::OnExpandToFit( )
{
   ExpandToFit( m_bExpandUseFixed );
}

void
ZGrid::OnFill( )
{
   DeleteAllItems( );
   AutoFill( );
}

void
ZGrid::OnCellNormal( )
{
   SetCellType( 1, 1, RUNTIME_CLASS( ZGridCell ) );
   SetItemText( 1, 1, _T( "1" ) );
   SetItemState( 1, 1, GetItemState( 1, 1 ) & ~GVIS_READONLY );
   Invalidate( );
}

void
ZGrid::OnCellReadonly( )
{
   OnCellNormal( );
   SetItemState( 1, 1, GetItemState( 1, 1 ) | GVIS_READONLY );
}

void
ZGrid::OnCellCheckbox( )
{
   OnCellNormal( );
   SetCellType( 1, 1, RUNTIME_CLASS( ZGridCellCheck ) );
}

void
ZGrid::OnCellDateTime( )
{
   OnCellNormal( );
   SetCellType( 1, 1, RUNTIME_CLASS( ZGridCellDateTime ) );

   ZGridCellDateTime *pCell = (ZGridCellDateTime *) GetCell( 1, 1 );
   pCell->SetTime( CTime::GetCurrentTime( ), 1, 1 );
   RedrawCell( 1, 1 );
   SetColumnWidth( 1, 100 );
}

void
ZGrid::OnCellUrl( )
{
   OnCellNormal( );

   if ( SetCellType( 1, 1, RUNTIME_CLASS( ZGridURLCell ) ) == 0 )
       return;

   SetItemText( 1, 1, _T( "www.codeproject.com" ) );
}

void
ZGrid::OnCellCombo( )
{
   OnCellNormal( );

   if ( SetCellType( 1, 1, RUNTIME_CLASS( ZGridCellCombo ) ) == 0 )
      return;

   SetItemText( 1, 1, _T( "Option 1" ) );

   CStringArray options;
   options.Add( _T( "Option 1" ) );
   options.Add( _T( "Option 2" ) );
   options.Add( _T( "Option 3" ) );

   ZGridCellCombo *pCell = (ZGridCellCombo *) GetCell( 1, 1 );
   pCell->SetOptions( options );
   pCell->SetStyle( CBS_DROPDOWN ); // CBS_DROPDOWNLIST, CBS_SIMPLE
}

void
ZGrid::OnCellNumeric( )
{
   OnCellNormal( );
   if ( SetCellType( 1, 1, RUNTIME_CLASS( ZGridCellNumeric ) ) == 0 )
      return;

   SetItemText( 1, 1, _T( "12345" ) );
}

void
ZGrid::OnTrackfocus( )
{
   m_bTrackFocus = !m_bTrackFocus;
   SetTrackFocusCell( m_bTrackFocus );
}

void
ZGrid::OnFramefocus( )
{
   m_bFrameFocus = !m_bFrameFocus;
   SetFrameFocusCell( m_bFrameFocus );
}

void
ZGrid::OnRejectEditAttempts( )
{
   m_bRejectEditAttempts = !m_bRejectEditAttempts;
}

void
ZGrid::OnRejectEditChanges( )
{
   m_bRejectEditChanges = !m_bRejectEditChanges;
}

void
ZGrid::OnExpandLast( )
{
   ExpandLastColumn( );
}

void
ZGrid::OnSetFocus11( )
{
   ZCellId cellId( 1, 1 );

   SetFocusCell( cell );

   if ( GetListMode( ) )
      SetSelectedRange( 1, GetFixedColumnCount( ), 1, GetColumnCount( ) - 1, TRUE );
   else
      SetSelectedRange( 1, 1, 1, 1, TRUE );
}

#endif  //???

// (Thanks to Koay Kah Hoe for this).
BOOL
ZGrid::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN )
   {
   // TraceLine( "ZGrid::PreTranslateMessage KEYDOWN Char: %d Flags: %d", pMsg->wParam, pMsg->lParam );
      // This permits copy/paste clipboard processing to occur.
      if ( mIsCTRL_Pressed( ) &&
           (pMsg->wParam == 'C' || pMsg->wParam == 'V' || pMsg->wParam == 'X' ||
            pMsg->wParam == 'c' || pMsg->wParam == 'v' || pMsg->wParam == 'x') )
      {
         ::TranslateMessage( pMsg );
         ::DispatchMessage( pMsg );
         return( TRUE );                    // DO NOT process further
      }
      else
      if ( pMsg->wParam == VK_RETURN )
      {
         ::TranslateMessage( pMsg );
         ::DispatchMessage( pMsg );
         return( TRUE );                    // DO NOT process further
      }
      else
      if ( pMsg->wParam == VK_ESCAPE )
      {
         if ( m_pZeidonWnd == 0 )
         {
            ::TranslateMessage( pMsg );
            ::DispatchMessage( pMsg );
            return( TRUE );                 // DO NOT process further
         }
         else
         {
         // TraceLineS( "ZGrid::PreTranslateMessage VK_ESCAPE", "" );
            EndEditing( );      // destroy any InPlaceEdit's
            SetFocus( );
            return( TRUE );     // DO NOT process further
         }
      }
      else
      if ( pMsg->wParam == VK_TAB && m_pZeidonWnd )
      {
      // TraceLineS( "ZGrid::PreTranslateMessage: ", "VK_TAB" );
         SetFocus( );
         SendMessage( WM_KEYDOWN, VK_TAB, pMsg->lParam );
         return( TRUE );                 // DO NOT process further
      }
      else
      if ( pMsg->wParam < 0x30 || pMsg->wParam > 0x5A )
      {
      // don't do nuthin;  // only handle alpha/numeric characters
#if 0
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

#if(WINVER >= 0x0400)
#define VK_PROCESSKEY     0xE5
#endif /* WINVER >= 0x0400 */

#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE

#endif

      }
      else
      {

      ZCellId cellId = GetFocusCell( );
      if ( IsValid( cellId ) &&
           IsCellEditable( cellId ) == FALSE &&
           mIsCTRL_Pressed( ) == FALSE &&
           pMsg->wParam >= 0x20 && pMsg->wParam < 0x7f )
      {
         CString  csText;
         GRID_ROW *pRow;
         long lRow = cellId.m_lRow;
         long lRowStart;
         ZGridCellBase *pCell;
         zULONG ulMessageTime = GetMessageTime( );
         if ( ulMessageTime - m_ulKeydownMessageTime > m_ulDblclkTime )
         {
            m_nKeydownCnt = 0;
            if ( m_szKeyFind[ m_nKeydownCnt ] == (char) pMsg->wParam )
               lRow++;
            else
               lRow = m_nFixedRows;
         }
         else
         if ( m_nKeydownCnt == 0 )
            lRow++;

         if ( lRow >= m_lRows )
            lRow = m_nFixedRows;

         m_ulKeydownMessageTime = ulMessageTime;
         m_szKeyFind[ m_nKeydownCnt ] = (char) pMsg->wParam;
         m_nKeydownCnt++;

         m_szKeyFind[ m_nKeydownCnt ] = 0;
         lRowStart = lRow - 1;
         if ( lRowStart < m_nFixedRows )
            lRowStart = m_lRows - 1;

         for ( ; lRow < m_lRows; lRow++ )
         {
            pRow = m_RowData[ lRow ];
            if ( pRow )
               pCell = pRow->GetAt( cellId.m_nCol );
            else
               pCell = 0;

            if ( pCell )
               pCell->GetText( csText, lRow, cellId.m_nCol );

            if ( lRow == lRowStart )
            {
               if ( _strnicmp( csText, m_szKeyFind, m_nKeydownCnt ) != 0 &&
                    m_nKeydownCnt > 1 )
               {
                  lRow++;
                  if ( lRow >= m_lRows )
                     lRow = m_nFixedRows;
               }

               break;
            }

            if ( pCell )
            {
               if ( _strnicmp( csText, m_szKeyFind, m_nKeydownCnt ) == 0 )
               {
               // TraceLine( "Keydown(%d) zCHAR found: ", m_nKeydownCnt, csText );
                  break;
               }
            }

            if ( lRow == m_lRows - 1 )
               lRow = m_nFixedRows - 1;
         }

         ZCellId next;
         next.m_nCol = cellId.m_nCol;
         next.m_lRow = lRow;

         int nOldMode = m_MouseMode;

         m_PrevSelectedCellMap.RemoveAll( );
      // m_MouseMode = m_bListMode ? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
         m_MouseMode = MOUSE_SELECT_ROW;
         m_cellSelectionStart = next;

         // Notify parent that selection is changing -
         // Arthur Westerman/Scot Brennecke
         if ( SendMessageToParent( next.m_lRow, next.m_nCol, GVN_SELCHANGING ) )
         {
            next = cellId;  // don't let cell change!
         }
         else
         {
            OnSelecting( next );
            SendMessageToParent( next.m_lRow, next.m_nCol, GVN_SELCHANGED );
            if ( next.m_lRow != cellId.m_lRow )
               SendMessageToParent( next.m_lRow, next.m_nCol, NM_CLICK );
         }

         m_MouseMode = MOUSE_NOTHING;

      // TraceLine( "Keydown search: %s   count: %d   row: %d",
      //            m_szKeyFind, m_nKeydownCnt, lRow );
         SetFocusCell( next );
         EnsureVisible( next.m_lRow, cellId.m_nCol );
         m_cellSelectionStart = next;
         OnSelecting( next );
         Invalidate( );
         return( TRUE );                 // DO NOT process further
      }
      }
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

// NM_RCLICK
void
ZGrid::OnGridRClick( NMHDR *pNotifyStruct, LRESULT *pResult )
{
    NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
    TraceLine( _T( "Right button click on row %d, col %d\n" ),
               pItem->cell.lRow, pItem->cell.nCol );
#endif
}

// GVN_BEGINEDIT
void
ZGrid::OnGridStartEdit( NMHDR *pNotifyStruct, LRESULT *pResult )
{
   NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
   TraceLine( _T( "Start Edit on row %d, col %d\n" ),
              pItem->cell.lRow, pItem->cell.nCol );
#endif
   *pResult = m_bRejectEditAttempts ? -1 : 0;
}

// GVN_ENDEDIT
void
ZGrid::OnGridEndEdit( NMHDR *pNotifyStruct, LRESULT *pResult )
{
   NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
   TraceLine( _T( "End Edit on row %d, col %d\n" ),
              pItem->cell.lRow, pItem->cell.nCol );
#endif
   *pResult = m_bRejectEditChanges ? -1 : 0;
}

// GVN_SELCHANGING
void
ZGrid::OnGridStartSelChange( NMHDR *pNotifyStruct, LRESULT *pResult )
{
   NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
   TraceLine( _T( "Start Selection Change on row %d, col %d (%d Selected)\n" ),
              pItem->cell.lRow, pItem->cell.nCol, GetSelectedCount( ) );
#endif
}

// GVN_SELCHANGED
void
ZGrid::OnGridEndSelChange( NMHDR *pNotifyStruct, LRESULT *pResult )
{
   NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
   TraceLine( _T( "End Selection Change on row %d, col %d (%d Selected)\n" ),
              pItem->cell.lRow, pItem->cell.nCol, GetSelectedCount( ) );
#endif
}

// GVN_BEGINDRAG
void
ZGrid::OnGridBeginDrag( NMHDR *pNotifyStruct, LRESULT *pResult )
{
   NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
   TraceLine( _T( "Begin Drag row %d, col %d\n" ),
              pItem->cell.lRow, pItem->cell.nCol );
#endif
}

void
ZGrid::OnGridDblClick( NMHDR *pNotifyStruct, LRESULT *pResult )
{
   NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
   TraceLine( _T( "Double Clicked on row %d, col %d\n" ),
              pItem->cell.lRow, pItem->cell.nCol );
#endif
}

void
ZGrid::OnGridClick( NMHDR *pNotifyStruct, LRESULT *pResult )
{
   NM_GRIDVIEW *pItem = (NM_GRIDVIEW *) pNotifyStruct;
#ifdef DEBUG_ALL
   TraceLine( _T( "Clicked on row %d, col %d\n" ),
              pItem->cell.lRow, pItem->cell.nCol );
#endif
}

#if 0  //???
void
ZGrid::OnCallbackFunction( )
{
   m_bCallback = !m_bCallback;
   SetCallbackFunc( m_bCallback ? GridCallback : 0, 0 );
}

void
ZGrid::OnVirtualMode( )
{
//?UpdateData( );
   m_bVirtualMode = !m_bVirtualMode;
   SetVirtualMode( m_bVirtualMode );
#if 0 //???
   m_bHeaderSort = GetHeaderSort( );
   m_bEditable = IsEditable( );

   if ( m_bVirtualMode )
   {
      SetCallbackFunc( m_bCallback ? GridCallback : 0, 0 );

      m_nFixCols = 1;
      m_nFixRows = 1;
      m_nCols = 100;
      m_lRows = 100000;

      TRY
      {
         SetRowCount( m_lRows );
         SetColumnCount( m_nCols );
         SetFixedRowCount( m_nFixRows );
         SetFixedColumnCount( m_nFixCols );
      }
      CATCH ( CMemoryException, e )
      {
         e->ReportError( );
         return;
      }
      END_CATCH
   }
   else
   {
      m_nFixCols = 1;
      m_nFixRows = 1;
      m_nCols = 6;
      m_lRows = 16;

      SetEditable( m_bEditable );
      SetAutoSizeStyle( );

      TRY
      {
         SetRowCount( m_lRows );
         SetColumnCount( m_nCols );
         SetFixedRowCount( m_nFixRows );
         SetFixedColumnCount( m_nFixCols );
      }
      CATCH ( CMemoryException, e )
      {
        e->ReportError( );
        return;
      }
      END_CATCH

      // Fill rows/cols with text.
      long lRowCnt = GetRowCount( );
      int  nColCnt = GetColumnCount( );
      for ( zLONG row = 0; row < lRowCnt; row++ )
      {
         for ( zSHORT nCol = 0; nCol < nColCnt; nCol++ )
         {
            CString cs;

            GV_ITEM Item;

            Item.mask = GVIF_TEXT;
            Item.m_lRow = row;
            Item.m_nCol = nCol;

            if ( row < m_nFixRows )
               cs.Format( _T( "Column %d" ), nCol );
            else
            if ( nCol < m_nFixCols )
                cs.Format( _T( "Row %d" ), row );
            else
                cs.Format( _T( "%d" ), row * nCol );

            Item.csText = cs;

            if ( rand( ) % 10 == 1 )
            {
               COLORREF clr = RGB( rand( ) % 128 + 128, rand( ) % 128 + 128,
                                   rand( ) % 128 + 128 );

               // SetItemBkColor( lRow, nCol, clr ); - or
               Item.crBkClr = clr;

               // SetItemFgColor( lRow, nCol RGB( 255, 0, 0 ) ); - or
               Item.crFgClr = RGB( 255, 0, 0 );

               Item.mask |= GVIF_BKCLR | GVIF_FGCLR;
            }

            if ( nCol < GetFixedColumnCount( ) )
            {
               Item.iImage = rand( ) % m_ImageList.GetImageCount( );
               Item.mask |= GVIF_IMAGE;
            }

            SetItem( &Item );
         }
      }
   }

// GetDefaultCell( FALSE, FALSE )->SetFormat( DT_RIGHT | DT_VCENTER |
//                                              DT_SINGLELINE | DT_NOPREFIX |
//                                              DT_END_ELLIPSIS );
// GetDefaultCell( TRUE, FALSE )->SetFormat( DT_RIGHT | DT_VCENTER |
//                                             DT_SINGLELINE | DT_NOPREFIX |
//                                             DT_END_ELLIPSIS );

   // Having weird Autosize problems with CE 2.11 - possibly just an
   // emulation problem.
#if !defined( _WIN32_WCE ) || ( _WIN32_WCE < 211 )
   AutoSize( );
#endif

   UpdateMenuUI( );
   UpdateData( FALSE );
#endif //???
}

BOOL
ZGrid::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT *pResult )
{
   if ( wParam == (WPARAM) GetDlgCtrlID( ) )
   {
      *pResult = 1;
      GV_DISPINFO *pDispInfo = (GV_DISPINFO *) lParam;
      if ( GVN_GETDISPINFO == pDispInfo->hdr.code )
      {
      // TRACE2( "Getting Display info for cell %d,%d\n",
      //         pDispInfo->item.m_lRow, pDispInfo->item.m_nCol );
         pDispInfo->item.csText.Format( _T( "Message %d,%d" ),
                                        pDispInfo->item.m_lRow,
                                        pDispInfo->item.m_nCol );
         return( TRUE );
      }
      else
      if ( GVN_ODCACHEHINT == pDispInfo->hdr.code )
      {
         GV_CACHEHINT *pCacheHint = (GV_CACHEHINT *) pDispInfo;
         Trace( _T( "Cache hint received for cell range %d,%d - %d,%d\n" ),
                pCacheHint->range.GetMinRow( ),
                pCacheHint->range.GetMinCol( ),
                pCacheHint->range.GetMaxRow( ),
                pCacheHint->range.GetMaxCol( ) );
      }
   }

   return( CDialog::OnNotify( wParam, lParam, pResult ) );
}

BOOL CALLBACK
ZGrid::GridCallback( GV_DISPINFO *pDispInfo, LPARAM lParam )
{
   pDispInfo->item.csText.Format( _T( "Callback %d,%d" ),
                                  pDispInfo->item.m_lRow,
                                  pDispInfo->item.m_nCol );
   return( TRUE );
}

void
ZGrid::UpdateMenuUI( )
{
#ifndef _WIN32_WCE
   zBOOL bHorzLines = GetGridLines( ) & GVL_HORZ;
   zBOOL bVertLines = GetGridLines( ) & GVL_VERT;
   GetMenu( )->CheckMenuItem( IDC_HORZ_LINES, MF_BYCOMMAND | (bHorzLines) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_VERT_LINES, MF_BYCOMMAND | (bVertLines) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->EnableMenuItem( IDC_SINGLESELMODE, MF_BYCOMMAND | (GetListMode( )) ? MF_ENABLED: MF_DISABLED|MF_GRAYED );
   GetMenu( )->CheckMenuItem( IDC_HEADERSORT, MF_BYCOMMAND | (GetHeaderSort( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_SINGLESELMODE, MF_BYCOMMAND | (GetSingleRowSelection( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_SINGLECOLSELMODE, MF_BYCOMMAND | (GetSingleColSelection( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_EDITABLE, MF_BYCOMMAND | (IsEditable( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_ALLOW_SELECTION, MF_BYCOMMAND | (IsSelectable( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_ROW_RESIZE, MF_BYCOMMAND | (GetRowResize( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_COL_RESIZE, MF_BYCOMMAND | (GetColumnResize( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_ITALICS, MF_BYCOMMAND | (m_bItalics) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_TITLETIPS, MF_BYCOMMAND | (GetTitleTips( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_TRACKFOCUS, MF_BYCOMMAND | (GetTrackFocusCell( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_FRAMEFOCUS, MF_BYCOMMAND | (GetFrameFocusCell( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_CALLBACK_FUNCTION, MF_BYCOMMAND | (GetCallbackFunc( )!=0) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_VIRTUAL_MODE, MF_BYCOMMAND | (GetVirtualMode( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->EnableMenuItem( IDC_CALLBACK_FUNCTION, MF_BYCOMMAND | (GetVirtualMode( )) ? MF_ENABLED: MF_DISABLED | MF_GRAYED );
   GetMenu( )->CheckMenuItem( IDC_FIXEDROW_SELECTION, MF_BYCOMMAND | (GetFixedRowSelection( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_FIXEDCOL_SELECTION, MF_BYCOMMAND | (GetFixedColumnSelection( )) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_VERTICAL_TEXT, MF_BYCOMMAND | (m_bVertical) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_EXPAND_USE_FIXED, MF_BYCOMMAND | (m_bExpandUseFixed) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_EDITING_REJECT_ATTEMPT, MF_BYCOMMAND | (m_bRejectEditAttempts) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( IDC_EDITING_REJECT_CHANGE, MF_BYCOMMAND | (m_bRejectEditChanges) ? MF_CHECKED: MF_UNCHECKED );
   GetMenu( )->CheckMenuItem( ID_HIDE2NDROWCOLUMN, MF_BYCOMMAND | (m_bRow2Col2Hidden) ? MF_CHECKED: MF_UNCHECKED );
#endif
}

void
ZGrid::OnFixedRowSelection( )
{
   SetFixedRowSelection( !GetFixedRowSelection( ) );
   UpdateMenuUI( );
}

void
ZGrid::OnFixedColSelection( )
{
   SetFixedColumnSelection( !GetFixedColumnSelection( ) );
   UpdateMenuUI( );
}

void
ZGrid::OnExpandUseFixed( )
{
   m_bExpandUseFixed = !m_bExpandUseFixed;
   UpdateMenuUI( );
}

void
ZGrid::OnVertical( )
{
   m_bVertical = !m_bVertical;

   // Set reasonable format
// GetDefaultCell( TRUE, FALSE )->SetFormat( DT_SINGLELINE | DT_NOPREFIX |
//                                             DT_END_ELLIPSIS | DT_LEFT |
//                                             DT_BOTTOM );
// GetDefaultCell( TRUE, TRUE )->SetFormat( DT_SINGLELINE | DT_NOPREFIX |
//                                            DT_END_ELLIPSIS | DT_LEFT |
//                                            DT_BOTTOM );

   // Get the font
   CFont *pFont = GetFont( );
   LOGFONT lf;
   pFont->GetLogFont( &lf );

   // Make it true-type
   memcpy( lf.lfFaceName, _T( "Arial" ), 6 );

   // Vertical or horizontal
   if ( m_bVertical )
   {
      lf.lfEscapement = 900;
      lf.lfOrientation = 900;
   }
   else
   {
      lf.lfEscapement = 0;
      lf.lfOrientation = 0;
   }

   // Set the fixed row defaults as this new font
   GetDefaultCell( TRUE, FALSE )->SetFont( &lf );

   // Set cell 1, 1 as the more normal font so URL cells look OK.
   lf.lfEscapement = 0;
   lf.lfOrientation = 0;
   SetItemFont( 0, 1, &lf );

   AutoSize( GVS_HEADER );
   if ( m_bTitleTips )
   {
      m_bTitleTips = !m_bVertical;
      EnableTitleTips( !m_bVertical );
   }

   UpdateMenuUI( );
}

void
ZGrid::OnContextMenu( CWnd *pWnd, CPoint pt )
{
   if ( pWnd->GetSafeHwnd( ) == GetSafeHwnd( ) )
   {
      ScreenToClient( &pt );
      ZCellId cellId = GetCellFromPt( pt );
#ifdef DEBUG_ALL
      TraceLine( _T( "Context menu called on row %d, col %d\n" ),
                 cell.m_lRow, cell.m_nCol );
#endif
   }
}

void
ZGrid::OnHide2ndrowcolumn( )
{
   m_bRow2Col2Hidden = !m_bRow2Col2Hidden;

   if ( m_bRow2Col2Hidden )
   {
      SetColumnWidth( 2, 0 );
      SetRowHeight( 2, 0 );
   }
   else
   {
      SetColumnWidth( 2, 1 );
      AutoSizeColumn( 2 );
      SetRowHeight( 2, 1 );
      AutoSizeRow( 2 );
   }

   Refresh( );
   UpdateMenuUI( );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
Grid( ZSubtask *pZSubtask,
      CWnd     *pWndParent,
      ZMapAct  *pzmaComposite,
      zVIEW    vDialog,
      zSHORT   nOffsetX,
      zSHORT   nOffsetY,
      zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZGrid( pZSubtask, pWndParent,
                      pzmaComposite, vDialog,
                      nOffsetX, nOffsetY, pCtrlDef ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid
#if 0
ZGrid::ZGrid( long lRows, int nCols, int nFixedRows, int nFixedCols )
{
   RegisterWindowClass( );
#if !defined( GRIDCONTROL_NO_DRAGDROP ) || !defined( GRIDCONTROL_NO_CLIPBOARD )
   _AFX_THREAD_STATE *pState = AfxGetThreadState( );
   if ( !pState->m_bNeedTerm && !AfxOleInit( ) )
     AfxMessageBox( _T( "OLE initialization failed. Make sure that the OLE "
                        "libraries are the correct version" ) );
#endif

   // Store the system colors in case they change. The gridctrl uses
   // these colors, and in OnSysColorChange we can check to see if
   // the gridctrl colors have been changed from the system colors.
   // If they have, then leave them, otherwise change them to reflect
   // the new system colors.
   m_clrWindowText     = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrWindowColor    = ::GetSysColor( COLOR_WINDOW );
   m_clr3DFace         = ::GetSysColor( COLOR_3DFACE );
   m_clrShadow         = ::GetSysColor( COLOR_3DSHADOW );
   m_clrGridLineColor  = RGB( 192, 192, 192 );

   m_lRows             = 0;
   m_nCols             = 0;
   m_nFixedRows        = 0;
   m_nFixedCols        = 0;

   m_bVirtualMode      = FALSE;
   m_pfnCallback       = 0;

   m_nVScrollMax       = 0;        // Scroll position
   m_nHScrollMax       = 0;
   m_nRowsPerWheelNotch = GetMouseScrollLines( ); // Get the number of lines
                                                  // per mouse wheel notch to scroll
   m_nBarState         = GVL_NONE;
   m_MouseMode         = MOUSE_NOTHING;
   m_nGridLines        = GVL_BOTH;
   m_bEditable         = TRUE;
   m_bListMode         = FALSE;
   m_bSingleRowSelection = FALSE;
   m_bSingleColSelection = FALSE;
   m_bLMouseButtonDown   = FALSE;
   m_bRMouseButtonDown   = FALSE;
   m_bAllowDraw          = TRUE;   // allow draw updates
   m_bEnableSelection    = TRUE;
   m_bFixedColumnSelection = TRUE;
   m_bFixedRowSelection  = TRUE;
   m_bAllowRowResize     = TRUE;
   m_bAllowColumnResize  = TRUE;
   m_bSortOnClick        = FALSE;  // Sort on header row click
   m_bHandleTabKey       = TRUE;
#ifdef _WIN32_WCE
   m_bDoubleBuffer     = FALSE;    // Use double buffering to avoid flicker?
#else
   m_bDoubleBuffer     = TRUE;     // Use double buffering to avoid flicker?
#endif
   m_bTitleTips        = TRUE;     // show cell title tips

   m_bWysiwygPrinting  = FALSE;    // use size-to-width printing

   m_bHiddenColUnhide  = TRUE;     // 0-width columns can be expanded via mouse
   m_bHiddenRowUnhide  = TRUE;     // 0-Height rows can be expanded via mouse

   m_bAllowColHide     = TRUE;     // Columns can be contracted to 0-width via mouse
   m_bAllowRowHide     = TRUE;     // Rows can be contracted to 0-height via mouse

   m_bAscending        = TRUE;     // sorting stuff
   m_nSortColumn       = -1;
   m_pfnCompare        = 0;

   m_nAutoSizeColumnStyle = GVS_BOTH; // Autosize grid using header and data info

   m_nTimerID          = 0;        // For drag-selection
   m_nTimerInterval    = 25;       // (in milliseconds)
   m_nResizeCaptureRange = 3;      // When resizing columns/row, the cursor has to be
                                   // within +/-3 pixels of the dividing line for
                                   // resizing to be possible
   m_pImageList        = 0;        // Images in the grid
   m_bAllowDragAndDrop = FALSE;    // for drag and drop - EFW - off by default
   m_bTrackFocusCell   = TRUE;     // Track Focus cell?
   m_bFrameFocus       = TRUE;     // Frame the selected cell?

   m_pRtcDefault = RUNTIME_CLASS( ZGridCell );

   SetupDefaultCells( );
// SetGridBkColor( m_clrShadow );
   SetGridBkColor( m_clr3DFace );

   // Set up the initial grid size.
   SetRowCount( lRows );
   SetColumnCount( nCols );
   SetFixedRowCount( nFixedRows );
   SetFixedColumnCount( nFixedCols );

   SetTitleTipTextClr( CLR_DEFAULT ); // FNA
   SetTitleTipBackClr( CLR_DEFAULT );

   // Set initial selection range (i.e. none).
   m_SelectedCellMap.RemoveAll( );
   m_PrevSelectedCellMap.RemoveAll( );

#if !defined( _WIN32_WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )
   // EFW - Added to support shaded/unshaded printout and
   // user-definable margins.
   m_bShadedPrintOut = TRUE;
   SetPrintMarginInfo( 2, 2, 4, 4, 1, 1, 1 );
#endif
}

ZGrid::~ZGrid( )
{
   DeleteAllItems( );

#ifndef GRIDCONTROL_NO_TITLETIPS
   if ( m_bTitleTips && mIs_hWnd( m_TitleTip.GetSafeHwnd( ) ) )
     m_TitleTip.DestroyWindow( );
#endif

   DestroyWindow( );

#if !defined( GRIDCONTROL_NO_DRAGDROP ) || !defined( GRIDCONTROL_NO_CLIPBOARD )
   // BUG FIX - EFW
   COleDataSource *pSource = COleDataSource::GetClipboardOwner( );
   if ( pSource )
      COleDataSource::FlushClipboard( );
#endif
}
#endif

// Register the window class if it has not already been registered.
BOOL
ZGrid::RegisterWindowClass( )
{
   WNDCLASS wndcls;
   HINSTANCE hInst = AfxGetInstanceHandle( );
   //HINSTANCE hInst = AfxGetResourceHandle( );

   if ( ::GetClassInfo( hInst, GRIDCTRL_CLASSNAME, &wndcls ) == 0 )
   {
      // otherwise we need to register a new class
      wndcls.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpfnWndProc   = ::DefWindowProc;
      wndcls.cbClsExtra    = wndcls.cbWndExtra = 0;
      wndcls.hInstance     = hInst;
      wndcls.hIcon         = 0;
#ifndef _WIN32_WCE_NO_CURSOR
      wndcls.hCursor       = AfxGetApp( )->LoadStandardCursor( IDC_ARROW );
#else
      wndcls.hCursor       = 0;
#endif
      wndcls.hbrBackground = (HBRUSH) (COLOR_3DFACE + 1);
      wndcls.lpszMenuName  = 0;
      wndcls.lpszClassName = GRIDCTRL_CLASSNAME;

      if ( AfxRegisterClass( &wndcls ) == 0 )
      {
         AfxThrowResourceException( );
         return( FALSE );
      }
   }

   return( TRUE );
}

zBOOL
ZGrid::Initialize( )
{
   // Stop re-entry problems.
   static zBOOL bRecursive = FALSE;
   if ( bRecursive )
      return( FALSE );

   bRecursive = TRUE;

#ifndef GRIDCONTROL_NO_TITLETIPS
   m_wndTitleTip.m_pParentWnd = this;
#endif

   // This would be a good place to register the droptarget but unfortunately
   // this causes problems if you are using the grid in a view.
   // Moved from OnSize.
//#ifndef GRIDCONTROL_NO_DRAGDROP
// m_DropTarget.Register( this );
//#endif

   if ( mIs_hWnd( m_hWnd ) )
      ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

   // Kludge: Make sure the client edge shows ... This is so horrible it
   // makes my eyes water.
   CRect rect;
   GetWindowRect( rect );
   CWnd *pParent = GetParent( );
   if ( pParent )
      pParent->ScreenToClient( rect );

   rect.InflateRect( 1, 1 );
   MoveWindow( rect );
   rect.DeflateRect( 1, 1 );
   MoveWindow( rect );

   bRecursive = FALSE;
   return( TRUE );
}

// Creates the control - use like any other window create control
BOOL
ZGrid::Create( const RECT& rect, CWnd *pParentWnd, UINT uId, DWORD dwStyle )
{
   ASSERT( pParentWnd->GetSafeHwnd( ) );

   TRY
   {
      m_arRowHeights.SetSize( m_lRows );   // initialize row heights
      m_arColWidths.SetSize( m_nCols );    // initialize column widths
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return( FALSE );
   }
   END_CATCH

   if ( CWnd::Create( GRIDCTRL_CLASSNAME, 0, dwStyle, rect, pParentWnd, uId ) == 0 )
      return( FALSE );

   return( TRUE );
}

void
ZGrid::SetupDefaultCells( )
{
   m_cellDefault.SetGrid( this );        // normal editable cell
   m_cellFixedColDef.SetGrid( this );    // cell for fixed columns
   m_cellFixedRowDef.SetGrid( this );    // cell for fixed rows
   m_cellFixedRowColDef.SetGrid( this ); // cell for area overlapped
                                         // by fixed columns/rows

   m_cellDefault.SetTextClr( m_clrWindowText );
   m_cellDefault.SetBackClr( m_clrWindow );
   m_cellFixedColDef.SetTextClr( m_clrWindowText );
   m_cellFixedColDef.SetBackClr( m_clr3DFace );
   m_cellFixedRowDef.SetTextClr( m_clrWindowText );
   m_cellFixedRowDef.SetBackClr( m_clr3DFace );
   m_cellFixedRowColDef.SetTextClr( m_clrWindowText );
   m_cellFixedRowColDef.SetBackClr( m_clr3DFace );
}

void
ZGrid::PreSubclassWindow( )
{
   CWnd::PreSubclassWindow( );

// HFONT hFont = ::CreateFontIndirect( m_cellDefault.GetFont( ) );
// OnSetFont( (LPARAM) hFont, 0 );
// DeleteObject( hFont );

   Initialize( );
}

// Sends a message to the parent in the form of a WM_NOTIFY message with
// a NM_GRIDVIEW structure attached.
LRESULT
ZGrid::SendMessageToParent( long lRow, int nCol, int nMessage )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return( 0 );

   NM_GRIDVIEW nmgv;
   nmgv.cell.lRow = lRow;
   nmgv.cell.nCol = nCol;
   nmgv.hdr.hwndFrom = m_hWnd;
   nmgv.hdr.idFrom = GetDlgCtrlID( );
   nmgv.hdr.code = nMessage;

   if ( m_pZSubtask->m_pZTask->m_nTraceAction >= 0x77 )
   {
      if ( nMessage == GVN_SELCHANGED )
      {
         TraceLine( "ZGrid::SendMessageToParent: GVN_SELCHANGED  Row: %d  Col: %d",
                    lRow, nCol );
      }
   }

   LRESULT lResult = RelayEvent( nmgv );

   if ( lResult == 0 )
   {
      CWnd *pOwner = GetOwner( );
      if ( pOwner && mIs_hWnd( pOwner->m_hWnd ) )
      {
         lResult = pOwner->SendMessage( WM_NOTIFY, nmgv.hdr.idFrom,
                                        (LPARAM) &nmgv );
      }
   }

   return( lResult );
}

// Send a request to the parent to return information on a given cell.
LRESULT
ZGrid::SendDisplayRequestToParent( GV_DISPINFO *pDisplayInfo ) const
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return( 0 );

   // Fix up the message headers
   pDisplayInfo->hdr.hwndFrom = m_hWnd;
   pDisplayInfo->hdr.idFrom = GetDlgCtrlID( );
   pDisplayInfo->hdr.code = GVN_GETDISPINFO;

   // Send the message
   CWnd *pOwner = GetOwner( );
   if ( pOwner && mIs_hWnd( pOwner->m_hWnd ) )
   {
      return( pOwner->SendMessage( WM_NOTIFY, pDisplayInfo->hdr.idFrom,
                                   (LPARAM) pDisplayInfo ) );
   }
   else
      return( 0 );
}

// Send a hint to the parent about caching information
LRESULT
ZGrid::SendCacheHintToParent( const ZCellRange& range ) const
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return( 0 );

   GV_CACHEHINT CacheHint;

   // Fix up the message headers
   CacheHint.hdr.hwndFrom = m_hWnd;
   CacheHint.hdr.idFrom = GetDlgCtrlID( );
   CacheHint.hdr.code = GVN_ODCACHEHINT;

   CacheHint.range = range;

   // Send the message
   CWnd *pOwner = GetOwner( );
   if ( pOwner && mIs_hWnd( pOwner->m_hWnd ) )
      return( pOwner->SendMessage( WM_NOTIFY, CacheHint.hdr.idFrom,
                                   (LPARAM) &CacheHint ) );
   else
      return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid message handlers

void
ZGrid::OnPaint( )
{
   CPaintDC dc( this );     // device context for painting

   if ( m_bDoubleBuffer )   // use a memory DC to remove flicker
   {
      CMemDC MemDC( &dc );
      OnDraw( &MemDC );
   }
   else
   {
      // Draw raw - helps in debugging visual problems.
      OnDraw( &dc );
   }
}

BOOL
ZGrid::OnEraseBkgnd( CDC *pDC )
{
   return( TRUE );   // don't erase the background
}

// Custom background erasure. This gets called from within the OnDraw
// function, since we will (most likely) be using a memory DC to stop
// flicker. If we just erase the background normally through OnEraseBkgnd,
// and didn't fill the memDC's selected bitmap with color, then all sorts
// of visual problems would occur.
void
ZGrid::EraseBkgnd( CDC *pDC )
{
   CRect  rectVis;
   CRect  rectClip;
   CRect  rect;
   CBrush brushFixedRowColBk( GetDefaultCell( TRUE, TRUE )->GetBackClr( ) );
   CBrush brushFixedRowBk( GetDefaultCell( TRUE, FALSE )->GetBackClr( ) );
   CBrush brushFixedColBk( GetDefaultCell( FALSE, TRUE )->GetBackClr( ) );
   CBrush brushTextBk( GetDefaultCell( FALSE, FALSE )->GetBackClr( ) );
   CBrush brushBk( GetGridBkColor( ) );
   // CBrush brushBk( GetTextBkColor( ) );

   if ( pDC->GetClipBox( rectClip ) == ERROR )
      return;

   GetVisibleNonFixedCellRange( rectVis );

   int nFixedColumnWidth = GetFixedColumnWidth( );
   int nFixedRowHeight = GetFixedRowHeight( );

   // Draw Fixed row/column background.
   if ( rectClip.left < nFixedColumnWidth && rectClip.top < nFixedRowHeight )
   {
      pDC->FillRect( CRect( rectClip.left, rectClip.top,
                            nFixedColumnWidth, nFixedRowHeight ),
                     &brushFixedRowColBk );
   }

   // Draw Fixed columns background.
   if ( rectClip.left < nFixedColumnWidth && rectClip.top < rectVis.bottom )
   {
      pDC->FillRect( CRect( rectClip.left, rectClip.top,
                            nFixedColumnWidth, rectVis.bottom ),
                     &brushFixedColBk );
   }

   // Draw Fixed rows background.
   if ( rectClip.top < nFixedRowHeight &&
        rectClip.right > nFixedColumnWidth && rectClip.left < rectVis.right )
   {
      pDC->FillRect( CRect( nFixedColumnWidth-1, rectClip.top,
                            rectVis.right, nFixedRowHeight ),
                     &brushFixedRowBk );
   }

   // Draw non-fixed cell background.
   if ( rect.IntersectRect( rectVis, rectClip ) )
   {
      CRect rectCell( max( nFixedColumnWidth, rect.left ),
                      max( nFixedRowHeight, rect.top ),
                      rect.right, rect.bottom );
      pDC->FillRect( rectCell, &brushTextBk );
   }

   // Draw right hand side of window outside grid.
   if ( rectVis.right < rectClip.right )
   {
      pDC->FillRect( CRect( rectVis.right, rectClip.top,
                            rectClip.right, rectClip.bottom ),
                     &brushBk );
   }

   // Draw bottom of window below grid.
   if ( rectVis.bottom < rectClip.bottom && rectClip.left < rectVis.right )
   {
      pDC->FillRect( CRect( rectClip.left, rectVis.bottom,
                            rectVis.right, rectClip.bottom ),
                     &brushBk );
   }
}

void
ZGrid::OnSize( UINT uType, int cx, int cy )
{
   static zBOOL bRecursive = FALSE;
   if ( bRecursive )
      return;

   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return;

   // This is not the ideal place to register the droptarget.
#ifndef GRIDCONTROL_NO_DRAGDROP
   m_DropTarget.Register( this );
#endif

   // Start re-entry blocking.
   bRecursive = TRUE;

   EndEditing( );      // destroy any InPlaceEdit's
   CWnd::OnSize( uType, cx, cy );
   ResetScrollBars( );

   // End re-entry blocking.
   bRecursive = FALSE;
}

UINT
ZGrid::OnGetDlgCode( )
{
   UINT uCode = DLGC_WANTARROWS | DLGC_WANTCHARS; // DLGC_WANTALLKEYS; //

   if ( m_bHandleTabKey && mIsCTRL_Pressed( ) == FALSE )
      uCode |= DLGC_WANTTAB;

   return( uCode );
}

#ifndef _WIN32_WCE
// If system colors change, then redo colors.
void
ZGrid::OnSysColorChange( )
{
   CWnd::OnSysColorChange( );

   if ( GetDefaultCell( FALSE, FALSE )->GetTextClr( ) == m_clrWindowText )             // Still using system colors
      GetDefaultCell( FALSE, FALSE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) ); // set to new system color

   if ( GetDefaultCell( FALSE, FALSE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( FALSE, FALSE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetDefaultCell( TRUE, FALSE )->GetTextClr( ) == m_clrWindowText )              // Still using system colors
      GetDefaultCell( TRUE, FALSE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) );  // set to new system color

   if ( GetDefaultCell( TRUE, FALSE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( TRUE, FALSE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetDefaultCell( FALSE, TRUE )->GetTextClr( ) == m_clrWindowText )              // Still using system colors
      GetDefaultCell( FALSE, TRUE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) );  // set to new system color

   if ( GetDefaultCell( FALSE, TRUE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( FALSE, TRUE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetDefaultCell( TRUE, TRUE )->GetTextClr( ) == m_clrWindowText )               // Still using system colors
      GetDefaultCell( TRUE, TRUE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) );   // set to new system color

   if ( GetDefaultCell( TRUE, TRUE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( TRUE, TRUE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetGridBkColor( ) == m_clrShadow )
      SetGridBkColor( ::GetSysColor( COLOR_3DSHADOW ) );

   m_clrWindowText = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrWindow = ::GetSysColor( COLOR_WINDOW );
   m_clr3DFace = ::GetSysColor( COLOR_3DFACE );
   m_clrShadow = ::GetSysColor( COLOR_3DSHADOW );
}
#endif

#ifndef _WIN32_WCE_NO_CURSOR
// If we are drag-selecting cells, or drag and dropping, stop now.
void
ZGrid::OnCaptureChanged( CWnd *pWnd )
{
   if ( pWnd->GetSafeHwnd( ) == GetSafeHwnd( ) )
      return;

   // kill timer if active
   if ( m_nTimerID )
   {
      KillTimer( m_nTimerID );
      m_nTimerID = 0;
   }

#ifndef GRIDCONTROL_NO_DRAGDROP
   // Kill drag and drop if active.
   if ( m_MouseMode == MOUSE_DRAGGING )
      m_MouseMode = MOUSE_NOTHING;
#endif
}
#endif

#if ( _MFC_VER >= 0x0421 ) || ( _WIN32_WCE >= 210 )
// If system settings change, then redo colors.
void
ZGrid::OnSettingChange( UINT uFlags, zCPCHAR lpszSection )
{
   CWnd::OnSettingChange( uFlags, lpszSection );

   if ( GetDefaultCell( FALSE, FALSE )->GetTextClr( ) == m_clrWindowText )             // Still using system colors
      GetDefaultCell( FALSE, FALSE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) ); // set to new system color

   if ( GetDefaultCell( FALSE, FALSE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( FALSE, FALSE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetDefaultCell( TRUE, FALSE )->GetTextClr( ) == m_clrWindowText )              // Still using system colors
      GetDefaultCell( TRUE, FALSE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) );  // set to new system color

   if ( GetDefaultCell( TRUE, FALSE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( TRUE, FALSE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetDefaultCell( FALSE, TRUE )->GetTextClr( ) == m_clrWindowText )              // Still using system colors
      GetDefaultCell( FALSE, TRUE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) );  // set to new system color

   if ( GetDefaultCell( FALSE, TRUE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( FALSE, TRUE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetDefaultCell( TRUE, TRUE )->GetTextClr( ) == m_clrWindowText )               // Still using system colors
      GetDefaultCell( TRUE, TRUE )->SetTextClr( ::GetSysColor( COLOR_WINDOWTEXT ) );   // set to new system color

   if ( GetDefaultCell( TRUE, TRUE )->GetBackClr( ) == m_clrWindow )
      GetDefaultCell( TRUE, TRUE )->SetBackClr( ::GetSysColor( COLOR_WINDOW ) );

   if ( GetGridBkColor( ) == m_clrShadow )
      SetGridBkColor( ::GetSysColor( COLOR_3DSHADOW ) );

   m_clrWindowText = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrWindow = ::GetSysColor( COLOR_WINDOW );
   m_clr3DFace = ::GetSysColor( COLOR_3DFACE );
   m_clrShadow = ::GetSysColor( COLOR_3DSHADOW );

   m_nRowsPerWheelNotch = GetMouseScrollLines( ); // Get the number of lines
}
#endif

// For drag-selection. Scrolls hidden cells into view.
// TODO: decrease timer interval over time to speed up selection over time.
void
ZGrid::OnTimer( UINT uIdEvent )
{
   ASSERT( uIdEvent == WM_LBUTTONDOWN );
   if ( uIdEvent != WM_LBUTTONDOWN )
      return;

   CPoint pt;
   CPoint ptOrig;

#ifdef _WIN32_WCE
   if ( m_MouseMode == MOUSE_NOTHING )
      return;

   ptOrig = GetMessagePos( );
#else
   if ( GetCursorPos( &ptOrig ) == 0 )
      return;
#endif

   ScreenToClient( &ptOrig );

   CRect rect;
   GetClientRect( rect );

   int nFixedRowHeight = GetFixedRowHeight( );
   int nFixedColWidth = GetFixedColumnWidth( );

   pt = ptOrig;
   if ( pt.y > rect.bottom )
   {
      //SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
      SendMessage( WM_KEYDOWN, VK_DOWN, 0 );

      if ( pt.x < rect.left )
         pt.x = rect.left;

      if ( pt.x > rect.right )
         pt.x = rect.right;

      pt.y = rect.bottom;
      OnSelecting( GetCellFromPt( pt ) );
   }
   else
   if ( pt.y < nFixedRowHeight )
   {
      //SendMessage( WM_VSCROLL, SB_LINEUP, 0 );
      SendMessage( WM_KEYDOWN, VK_UP, 0 );

      if ( pt.x < rect.left )
         pt.x = rect.left;

      if ( pt.x > rect.right )
         pt.x = rect.right;

      pt.y = nFixedRowHeight + 1;
      OnSelecting( GetCellFromPt( pt ) );
   }

   pt = ptOrig;
   if ( pt.x > rect.right )
   {
      // SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );
      SendMessage( WM_KEYDOWN, VK_RIGHT, 0 );

      if ( pt.y < rect.top )
         pt.y = rect.top;

      if ( pt.y > rect.bottom )
         pt.y = rect.bottom;

      pt.x = rect.right;
      OnSelecting( GetCellFromPt( pt ) );
   }
   else
   if ( pt.x < nFixedColWidth )
   {
      //SendMessage( WM_HSCROLL, SB_LINELEFT, 0 );
      SendMessage( WM_KEYDOWN, VK_LEFT, 0 );

      if ( pt.y < rect.top )
         pt.y = rect.top;

      if ( pt.y > rect.bottom )
         pt.y = rect.bottom;

      pt.x = nFixedColWidth + 1;
      OnSelecting( GetCellFromPt( pt ) );
   }
}

// Move around using the keyboard.
void
ZGrid::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLine( "ZGrid::OnKeyDown Char: %d Flags: %d", uChar, uFlags );
// if ( mIsCTRL_Pressed( ) && uChar != 17 )
// {
//    TraceLineS( "", "" );
// }

   if ( IsValid( m_cellIdCurrent ) == FALSE )
   {
      CWnd::OnKeyDown( uChar, uRepCnt, uFlags );
      return;
   }

   ZGridCellBase *pCell = GetCell( m_cellIdCurrent.m_lRow,
                                   m_cellIdCurrent.m_nCol );

   if ( pCell->WantKeyDown( uChar, uRepCnt, uFlags ) )
      return;

   ZCellId next = m_cellIdCurrent;
   zBOOL bChangeLine = FALSE;
   zBOOL bHorzScrollAction = FALSE;
   zBOOL bVertScrollAction = FALSE;

   if ( mIsCTRL_Pressed( ) && uChar != 17 )
   {
      switch ( uChar )
      {
         case 'A':
            OnEditSelectAll( );
            break;

#ifndef GRIDCONTROL_NO_CLIPBOARD
         case 'X':
            OnEditCut( );
            break;

         case VK_INSERT:
         case 'C':
            OnEditCopy( );
            break;

         case 'V':
            OnEditPaste( );
            break;
#endif
      }
   }

#ifndef GRIDCONTROL_NO_CLIPBOARD
   if ( mIsSHIFT_Pressed( ) && (uChar == VK_INSERT) )
      OnEditPaste( );
#endif

   zBOOL bFoundVisible;
   int  nOrig;
   long lRowCnt = GetRowCount( );
   int  nColCnt = GetColumnCount( );
   int  nFixedRowCnt = GetFixedRowCount( );
   int  nFixedColCnt = GetFixedColumnCount( );

   if ( uChar == VK_DELETE )
   {
      zLONG lRC = ProcessImmediateEvent( this, zGRDN_DELETEROWDATA, 0 );
      if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
         CutSelectedText( );
   }
   else
   if ( uChar == VK_DOWN )
   {
      // Don't let user go to a hidden row.
      bFoundVisible = FALSE;
      nOrig = next.m_lRow;
      next.m_lRow++;
      while ( next.m_lRow < lRowCnt )
      {
         if ( GetRowHeight( next.m_lRow ) > 0 )
         {
            bFoundVisible = TRUE;
            break;
         }

         next.m_lRow++;
      }

      if ( bFoundVisible == FALSE )
         next.m_lRow = nOrig;
   }
   else
   if ( uChar == VK_UP )
   {
      // Don't let user go to a hidden row.
      bFoundVisible = FALSE;
      nOrig = next.m_lRow;
      next.m_lRow--;
      while ( next.m_lRow >= m_nFixedRows )
      {
         if ( GetRowHeight( next.m_lRow ) > 0 )
         {
            bFoundVisible = TRUE;
            break;
         }

         next.m_lRow--;
      }

      if ( bFoundVisible == FALSE )
         next.m_lRow = nOrig;
   }
   else
   if ( uChar == VK_RIGHT || (uChar == VK_TAB && mIsSHIFT_Pressed( ) == FALSE) )
   {
      // Don't let user go to a hidden column.
      bFoundVisible = FALSE;
      nOrig = next.m_nCol;
      next.m_nCol++;

      if ( uChar == VK_TAB )
      {
         // If we're at the end of a row, go down a row until we find a
         // non-hidden row.
         if ( next.m_nCol == nColCnt &&
              next.m_lRow < (lRowCnt - 1) )
         {
            next.m_lRow++;
            while ( next.m_lRow < lRowCnt )
            {
               if ( GetRowHeight( next.m_lRow ) > 0 )
               {
                 bFoundVisible = TRUE;
                 break;
               }

               next.m_lRow++;
            }

            next.m_nCol = m_nFixedCols;  // place focus on first non-fixed column
            bChangeLine = TRUE;
         }
         else
            CWnd::OnKeyDown( uChar, uRepCnt, uFlags );
      }

      // We're on a non-hidden row, so look across for the next
      // non-hidden column.
      while ( next.m_nCol < nColCnt )
      {
         if ( GetColumnWidth( next.m_nCol ) > 0 )
         {
            bFoundVisible = TRUE;
            break;
         }

         next.m_nCol++;
      }

      // If nothing worked then don't bother.
      if ( bFoundVisible == FALSE )
         next.m_nCol = nOrig;
   }
   else
   if ( uChar == VK_LEFT || (uChar == VK_TAB && mIsSHIFT_Pressed( )) )
   {
      // Don't let user go to a hidden column.
      bFoundVisible = FALSE;
      nOrig = next.m_nCol;
      next.m_nCol--;

      if ( uChar == VK_TAB )
      {
         if ( next.m_nCol == (nFixedColCnt - 1) && next.m_lRow > nFixedRowCnt )
         {
            next.m_lRow--;
            while ( next.m_lRow > nFixedRowCnt )
            {
               if ( GetRowHeight( next.m_lRow ) > 0 )
               {
                 bFoundVisible = TRUE;
                 break;
               }

               next.m_lRow--;
            }

            next.m_nCol = nColCnt - 1;
            bChangeLine = TRUE;
         }
         else
            CWnd::OnKeyDown( uChar, uRepCnt, uFlags );
      }

      while ( next.m_nCol >= m_nFixedCols )
      {
         if ( GetColumnWidth( next.m_nCol ) > 0 )
         {
            bFoundVisible = TRUE;
            break;
         }

         next.m_nCol--;
      }

      if ( bFoundVisible == FALSE )
         next.m_nCol = nOrig;
   }
   else
   if ( uChar == VK_NEXT )
   {
      ZCellId idOldTopLeft = GetTopLeftNonFixedCell( );
      SendMessage( WM_VSCROLL, SB_PAGEDOWN, 0 );
      bVertScrollAction = TRUE;
      ZCellId idNewTopLeft = GetTopLeftNonFixedCell( );

      int nIncrement = idNewTopLeft.m_lRow - idOldTopLeft.m_lRow;
      if ( nIncrement )
      {
         next.m_lRow += nIncrement;
         if ( next.m_lRow >( GetRowCount( ) - 1 ) )
            next.m_lRow = GetRowCount( ) - 1;
      }
      else
         next.m_lRow = GetRowCount( ) - 1;
   }
   else
   if ( uChar == VK_PRIOR )
   {
      ZCellId idOldTopLeft = GetTopLeftNonFixedCell( );
      SendMessage( WM_VSCROLL, SB_PAGEUP, 0 );
      bVertScrollAction = TRUE;
      ZCellId idNewTopLeft = GetTopLeftNonFixedCell( );

      int nIncrement = idNewTopLeft.m_lRow - idOldTopLeft.m_lRow;
      if ( nIncrement )
      {
         next.m_lRow += nIncrement;
         if ( next.m_lRow < m_nFixedRows )
            next.m_lRow = m_nFixedRows;
      }
      else
         next.m_lRow = m_nFixedRows;
   }
   else
   if ( uChar == VK_HOME )
   {
      // Home and Ctrl-Home work more like Excel ...
      // and don't let user go to a hidden cell.
      if ( mIsCTRL_Pressed( ) )
      {
         SendMessage( WM_VSCROLL, SB_TOP, 0 );
         SendMessage( WM_HSCROLL, SB_LEFT, 0 );
         bVertScrollAction = TRUE;
         bHorzScrollAction = TRUE;
         next.m_lRow = m_nFixedRows;
         next.m_nCol = m_nFixedCols;
      }
      else
      {
         SendMessage( WM_HSCROLL, SB_LEFT, 0 );
         bHorzScrollAction = TRUE;
         next.m_nCol = m_nFixedCols;
      }

      // Adjust column to avoid hidden columns and rows.
      while ( next.m_nCol < nColCnt - 1 )
      {
         if ( GetColumnWidth( next.m_nCol ) > 0 )
            break;

         next.m_nCol++;
      }

      while ( next.m_lRow < lRowCnt - 1 )
      {
         if ( GetRowHeight( next.m_lRow ) > 0 )
            break;

         next.m_lRow++;
      }
   }
   else
   if ( uChar == VK_END )
   {
      // End and Ctrl-End work more like Excel
      // and don't let user go to a hidden cell.
      if ( mIsCTRL_Pressed( ) )
      {
         SendMessage( WM_VSCROLL, SB_BOTTOM, 0 );
         SendMessage( WM_HSCROLL, SB_RIGHT, 0 );
         bHorzScrollAction = TRUE;
         bVertScrollAction = TRUE;
         next.m_lRow = lRowCnt - 1;
         next.m_nCol = nColCnt - 1;
      }
      else
      {
         SendMessage( WM_HSCROLL, SB_RIGHT, 0 );
         bHorzScrollAction = TRUE;
         next.m_nCol = nColCnt - 1;
      }

      // Adjust column to avoid hidden columns and rows.
      while ( next.m_nCol > m_nFixedCols + 1 )
      {
         if ( GetColumnWidth( next.m_nCol ) > 0 )
            break;

         next.m_nCol--;
      }

      while ( next.m_lRow > m_nFixedRows + 1 )
      {
         if ( GetRowHeight( next.m_lRow ) > 0 )
            break;

         next.m_lRow--;
      }
   }
   else
   if ( uChar == VK_F2 )
   {
      OnEditCell( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol,
                  CPoint( -1, -1 ), VK_LBUTTON );
   }
   else
   {
      CWnd::OnKeyDown( uChar, uRepCnt, uFlags );
      return;
   }

   if ( next != m_cellIdCurrent )
   {
      zLONG lHoldRow = m_cellIdCurrent.m_lRow;

      // While moving with the Cursorkeys the current ROW/CELL will get
      // selected OR Selection will get expanded when SHIFT is pressed
      // Cut n paste from OnLButtonDown - Franco Bez
      // Added check for 0 mouse mode - Chris Maunder.
      if ( m_MouseMode == MOUSE_NOTHING )
      {
         m_PrevSelectedCellMap.RemoveAll( );
      // m_MouseMode = m_bListMode ? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
         m_MouseMode = MOUSE_SELECT_ROW;
         if ( mIsSHIFT_Pressed( ) == FALSE || uChar == VK_TAB )
            m_cellSelectionStart = next;

         // Notify parent that selection is changing -
         // Arthur Westerman/Scot Brennecke
         if ( SendMessageToParent( next.m_lRow, next.m_nCol, GVN_SELCHANGING ) )
         {
            next = m_cellIdCurrent;  // don't let cell change!
         }
         else
         {
            OnSelecting( next );
            SendMessageToParent( next.m_lRow, next.m_nCol, GVN_SELCHANGED );
            if ( next.m_lRow != lHoldRow )
               SendMessageToParent( next.m_lRow, next.m_nCol, NM_CLICK );
         }

         m_MouseMode = MOUSE_NOTHING;
      }

      SetFocusCell( next );

      if ( IsCellVisible( next ) == FALSE )
      {
         switch ( uChar )
         {
            case VK_RIGHT:
               SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );
               bHorzScrollAction = TRUE;
               break;

            case VK_LEFT:
               SendMessage( WM_HSCROLL, SB_LINELEFT, 0 );
               bHorzScrollAction = TRUE;
               break;

            case VK_DOWN:
               SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
               bVertScrollAction = TRUE;
               break;

            case VK_UP:
               SendMessage( WM_VSCROLL, SB_LINEUP, 0 );
               bVertScrollAction = TRUE;
               break;

            case VK_TAB:
               if ( mIsSHIFT_Pressed( ) )
               {
                  if ( bChangeLine )
                  {
                     SendMessage( WM_VSCROLL, SB_LINEUP, 0 );
                     bVertScrollAction = TRUE;
                     SetScrollPos32( SB_HORZ, m_nHScrollMax );
                     break;
                  }
                  else
                  {
                     SendMessage( WM_HSCROLL, SB_LINELEFT, 0 );
                     bHorzScrollAction = TRUE;
                  }
               }
               else
               {
                  if ( bChangeLine )
                  {
                     SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
                     bVertScrollAction = TRUE;
                     SetScrollPos32( SB_HORZ, 0 );
                     break;
                  }
                  else
                  {
                     SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );
                     bHorzScrollAction = TRUE;
                  }
               }

               break;
         }

         EnsureVisible( next ); // Make sure cell is visible
         m_cellSelectionStart = next;
         OnSelecting( next );
         Invalidate( );
      }

      EnsureVisible( next ); // Make sure cell is visible

      if ( bHorzScrollAction )
         SendMessage( WM_HSCROLL, SB_ENDSCROLL, 0 );

      if ( bVertScrollAction )
         SendMessage( WM_VSCROLL, SB_ENDSCROLL, 0 );
   }
}

void
ZGrid::OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   CWnd::OnKeyUp( uChar, uRepCnt, uFlags );
}

void
ZGrid::OnSysKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLine( "ZGrid::OnSysKeyDown Char: %d Flags: %d", uChar, uFlags );
#ifdef GRIDCONTROL_USE_TITLETIPS
   m_TitleTip.Hide( );  // hide any titletips
#endif

   CWnd::OnSysKeyDown( uChar, uRepCnt, uFlags );
}

// Instant editing of cells when keys are pressed.
void
ZGrid::OnChar( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   // EFW - BUG FIX
   if ( mIsCTRL_Pressed( ) == FALSE && m_MouseMode == MOUSE_NOTHING && uChar != VK_ESCAPE )
   {
      if ( m_bHandleTabKey == FALSE || (m_bHandleTabKey && uChar != VK_TAB) )
      {
         OnEditCell( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol,
                     CPoint( -1, -1 ), uChar );
         if ( m_pZeidonWnd )
         {
            MSG msg;

            ::PeekMessage( &msg, m_hWnd, WM_KEYUP, WM_KEYUP, PM_REMOVE );

            if ( m_pZeidonWnd )
            {
               if ( uFlags & 0x00000200 )
               {
                  ::MessageBox( 0, "System Error",
                                "ZGrid::OnChar Bit 10 set!!!\nPlease Inform System Support",
                                MB_OK );
               }

               // Turn on bit 10 (not used by Windows) as a flag to
               // InPlaceEdit to ignore the message.
               uFlags |= 0x00000200;
               m_pZeidonWnd->SendMessage( WM_KEYDOWN, uChar, MAKELONG( uRepCnt, uFlags ) );
               m_pZeidonWnd->SendMessage( WM_CHAR, uChar, MAKELONG( uRepCnt, uFlags ) );
               m_pZeidonWnd->SendMessage( WM_KEYUP, uChar, MAKELONG( uRepCnt, uFlags ) );
               uFlags &= ~0x00000200;  // turn off bit 10
               return;
            }
         }
      }
   }

   CWnd::OnChar( uChar, uRepCnt, uFlags );
}

// Added by KiteFly
LRESULT
ZGrid::OnImeChar( WPARAM wCharCode, LPARAM )
{
   // EFW - BUG FIX
   if ( mIsCTRL_Pressed( ) == FALSE && m_MouseMode == MOUSE_NOTHING && wCharCode != VK_ESCAPE )
   {
      OnEditCell( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol,
                  CPoint( -1, -1 ), wCharCode );
   }

   return( 0 );
}

// Callback from any CInPlaceEdits that ended. This just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void
ZGrid::OnEndInPlaceEdit( NMHDR *pNMHDR, LRESULT *pResult )
{
   GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
   GV_ITEM     *pgvItem = &pgvDispInfo->item;

   // In case OnEndInPlaceEdit called as window is being destroyed
   if ( mIs_hWnd( GetSafeHwnd( ) ) == FALSE )
      return;

   OnEndEditCell( pgvItem->lRow, pgvItem->nCol, pgvItem->csText );
   //InvalidateCellRect( ZCellId( pgvItem->lRow, pgvItem->nCol ) );

   switch ( pgvItem->lItemExtra )
   {
      case VK_TAB:
      case VK_DOWN:
      case VK_UP:
      case VK_RIGHT:
      case VK_LEFT:
      case VK_NEXT:
      case VK_PRIOR:
      case VK_HOME:
      case VK_END:
         OnKeyDown( pgvItem->lItemExtra, 0, 0 );
         OnEditCell( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol,
                     CPoint( -1, -1 ), pgvItem->lItemExtra );
   }

   *pResult = 0;
}

// Handle horz scrollbar notifications
void
ZGrid::OnHScroll( UINT uSBCode, UINT uPos, CScrollBar *pScrollBar )
{
   EndEditing( );      // destroy any InPlaceEdit's

#ifndef GRIDCONTROL_NO_TITLETIPS
   m_wndTitleTip.Hide( );  // hide any titletips
#endif

   int nScrollPos = GetScrollPos32( SB_HORZ );
   ZCellId idTopLeft = GetTopLeftNonFixedCell( TRUE );

   CRect rect;
   GetClientRect( rect );

   switch ( uSBCode )
   {
      case SB_LINERIGHT:
         if ( nScrollPos < m_nHScrollMax )
         {
            // May have contiguous hidden columns.  Blow by them.
            int nColCnt = GetColumnCount( );
            while ( idTopLeft.m_nCol < (nColCnt - 1) &&
                    GetColumnWidth( idTopLeft.m_nCol ) < 1 )
            {
               idTopLeft.m_nCol++;
            }

            int nScrollX = GetColumnWidth( idTopLeft.m_nCol );
            SetScrollPos32( SB_HORZ, nScrollPos + nScrollX );
            if ( GetScrollPos32( SB_HORZ ) == nScrollPos )
               break;        // didn't work

            rect.left = GetFixedColumnWidth( );
         // rect.left = GetFixedColumnWidth( ) + nScrollX;
         // ScrollWindow( -nScrollX, 0, rect );
         // rect.left = rect.right - nScrollX;
            InvalidateRect( rect );
         }

         break;

      case SB_LINELEFT:
         {
            int nFixedColCnt = GetFixedColumnCount( );
            if ( nScrollPos > 0 && idTopLeft.m_nCol > nFixedColCnt )
            {
               int nColToUse = idTopLeft.m_nCol - 1;

               // May have contiguous hidden columns.  Blow by them.
               while ( nColToUse > nFixedColCnt &&
                       GetColumnWidth( nColToUse ) < 1 )
               {
                  nColToUse--;
               }

               int nScrollX = GetColumnWidth( nColToUse );
               SetScrollPos32( SB_HORZ, max( 0, nScrollPos - nScrollX ) );
               rect.left = GetFixedColumnWidth( );
            // ScrollWindow( nScrollX, 0, rect );
            // rect.right = rect.left + nScrollX;
               InvalidateRect( rect );
            }
         }

         break;

      case SB_PAGERIGHT:
         if ( nScrollPos < m_nHScrollMax )
         {
            rect.left = GetFixedColumnWidth( );
            int nOffset = rect.Width( );
            int nPos = min( m_nHScrollMax, nScrollPos + nOffset );
            SetScrollPos32( SB_HORZ, nPos );
            rect.left = GetFixedColumnWidth( );
            InvalidateRect( rect );
         }

         break;

      case SB_PAGELEFT:
         if ( nScrollPos > 0 )
         {
            rect.left = GetFixedColumnWidth( );
            int nOffset = -rect.Width( );
            int nPos = max( 0, nScrollPos + nOffset );
            SetScrollPos32( SB_HORZ, nPos );
            rect.left = GetFixedColumnWidth( );
            InvalidateRect( rect );
         }

         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         {
            SetScrollPos32( SB_HORZ, GetScrollPos32( SB_HORZ, TRUE ) );
            m_cellIdTopLeft.m_lRow = -1;
            ZCellId idNewTopLeft = GetTopLeftNonFixedCell( );
            if ( idNewTopLeft != idTopLeft )
            {
               rect.left = GetFixedColumnWidth( );
               InvalidateRect( rect );
            }
         }

         break;

      case SB_LEFT:
         if ( nScrollPos > 0 )
         {
            SetScrollPos32( SB_HORZ, 0 );
            Invalidate( );
         }

         break;

      case SB_RIGHT:
         if ( nScrollPos < m_nHScrollMax )
         {
            SetScrollPos32( SB_HORZ, m_nHScrollMax );
            Invalidate( );
         }

         break;

      default:
         break;
   }
}

// Handle vert scrollbar notifications.
void
ZGrid::OnVScroll( UINT uSBCode, UINT uPos, CScrollBar *pScrollBar )
{
   EndEditing( );      // destroy any InPlaceEdit's

#ifndef GRIDCONTROL_NO_TITLETIPS
   m_wndTitleTip.Hide( );  // hide any titletips
#endif

   // Get the scroll position ourselves to ensure we get a 32 bit value
   int nScrollPos = GetScrollPos32( SB_VERT );
   ZCellId idTopLeft = GetTopLeftNonFixedCell( );

   CRect rect;
   GetClientRect( rect );

   switch ( uSBCode )
   {
      case SB_LINEDOWN:
         if ( nScrollPos < m_nVScrollMax )
         {
            // May have contiguous hidden rows.  Blow by them.
            long lRowCnt = GetRowCount( ) - 1;
            while ( idTopLeft.m_lRow < lRowCnt &&
                    GetRowHeight( idTopLeft.m_lRow ) < 1 )
            {
               idTopLeft.m_lRow++;
            }

            int nScrollY = GetRowHeight( idTopLeft.m_lRow ) * (m_nExtraRowsPerItem + 1);
            SetScrollPos32( SB_VERT, nScrollPos + nScrollY );
            if ( GetScrollPos32( SB_VERT ) == nScrollPos )
               break;        // didn't work

            rect.top = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
         // rect.top = GetFixedRowHeight( ) + nScrollY;
         // ScrollWindow( 0, -nScrollY, rect );
         // rect.top = rect.bottom - nScrollY;
            InvalidateRect( rect );
         }

         break;

      case SB_LINEUP:
         {
            int nFixedRowCnt = GetFixedRowCount( );
            if ( nScrollPos > 0 && idTopLeft.m_lRow > nFixedRowCnt )
            {
               int nRowToUse = idTopLeft.m_lRow - 1;

               // May have contiguous hidden rows.  Blow by them.
               while ( nRowToUse > nFixedRowCnt &&
                       GetRowHeight( nRowToUse ) < 1 )
               {
                  nRowToUse--;
               }

               int nScrollY = GetRowHeight( nRowToUse ) * (m_nExtraRowsPerItem + 1);
               SetScrollPos32( SB_VERT, max( 0, nScrollPos - nScrollY ) );
               rect.top = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
            // ScrollWindow( 0, nScrollY, rect );
            // rect.bottom = rect.top + nScrollY;
               InvalidateRect( rect );
            }
         }

         break;

      case SB_PAGEDOWN:
         if ( nScrollPos < m_nVScrollMax )
         {
            rect.top = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
            nScrollPos = min( m_nVScrollMax, nScrollPos + rect.Height( ) );
            SetScrollPos32( SB_VERT, nScrollPos );
            rect.top = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
            InvalidateRect( rect );
         }

         break;

      case SB_PAGEUP:
         if ( nScrollPos > 0 )
         {
            rect.top = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
            int nOffset = -rect.Height( );
            int nPos = max( 0, nScrollPos + nOffset );
            SetScrollPos32( SB_VERT, nPos );
            rect.top = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
            InvalidateRect( rect );
         }

         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         {
            SetScrollPos32( SB_VERT, GetScrollPos32( SB_VERT, TRUE ) );
            m_cellIdTopLeft.m_lRow = -1;
            ZCellId idNewTopLeft = GetTopLeftNonFixedCell( );
            if ( idNewTopLeft != idTopLeft )
            {
               rect.top = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
               InvalidateRect( rect );
            }
         }

         break;

      case SB_TOP:
         if ( nScrollPos > 0 )
         {
            SetScrollPos32( SB_VERT, 0 );
            Invalidate( );
         }

         break;

      case SB_BOTTOM:
         if ( nScrollPos < m_nVScrollMax )
         {
            SetScrollPos32( SB_VERT, m_nVScrollMax );
            Invalidate( );
         }

      default:
         break;
   }
}

// Returns the default cell implementation for the given grid region.
ZGridCellBase *
ZGrid::GetDefaultCell( zBOOL bFixedRow, zBOOL bFixedCol ) const
{
   if ( bFixedRow && bFixedCol )
      return( (ZGridCellBase *) &m_cellFixedRowColDef );

   if ( bFixedRow )
      return( (ZGridCellBase *) &m_cellFixedRowDef );

   if ( bFixedCol )
      return( (ZGridCellBase *) &m_cellFixedColDef );

   return( (ZGridCellBase *) &m_cellDefault );
}

ZGridCellBase *
ZGrid::GetCell( long lRow, int nCol ) const
{
   if ( lRow < 0 || lRow >= m_lRows || nCol < 0 || nCol >= m_nCols )
      return( 0 );

   if ( GetVirtualMode( ) )
   {
      ZGridCellBase *pCell = GetDefaultCell( lRow < m_nFixedRows,
                                             nCol < m_nFixedCols );
      static GV_DISPINFO gvdi;
      gvdi.item.lRow = lRow;
      gvdi.item.nCol = nCol;
      gvdi.item.mask = 0xFFFFFFFF;
      gvdi.item.dwState = 0;
      gvdi.item.dwFormat = pCell->GetFormat( );
      gvdi.item.nImage = pCell->GetImage( );
      gvdi.item.clrBk = pCell->GetBackClr( );
      gvdi.item.clrFg = pCell->GetTextClr( );
      gvdi.item.lItemExtra = pCell->GetData( lRow, nCol );
      memcpy( &gvdi.item.lfFont, pCell->GetFont( ), sizeof( LOGFONT ) );
      gvdi.item.nMargin = pCell->GetMargin( );
      gvdi.item.csText.Empty( );

      // Fix the state bits.
      if ( IsCellSelected( lRow, nCol ) )
         gvdi.item.dwState |= GVIS_SELECTED;

      if ( lRow < GetFixedRowCount( ) )
         gvdi.item.dwState |= (GVIS_FIXED | GVIS_FIXEDROW);

      if ( nCol < GetFixedColumnCount( ) )
         gvdi.item.dwState |= (GVIS_FIXED | GVIS_FIXEDCOL);

      if ( GetFocusCell( ) == ZCellId( lRow, nCol ) )
         gvdi.item.dwState |= GVIS_FOCUSED;

      if ( m_pfnCallback )
         m_pfnCallback( &gvdi, m_lParam );
      else
         SendDisplayRequestToParent( &gvdi );

      static ZGridCell cell;
      cell.SetState( gvdi.item.dwState );
      cell.SetFormat( gvdi.item.dwFormat );
      cell.SetImage( gvdi.item.nImage );
      cell.SetBackClr( gvdi.item.clrBk );
      cell.SetTextClr( gvdi.item.clrFg );
      cell.SetData( gvdi.item.lItemExtra );
      cell.SetFont( &(gvdi.item.lfFont) );
      cell.SetMargin( gvdi.item.nMargin );
      cell.SetText( gvdi.item.csText, lRow, nCol );
      cell.SetGrid( (ZGrid *) this );

      return( (ZGridCellBase *) &cell );
   }

   GRID_ROW *pRow = m_RowData[ lRow ];
   if ( pRow == 0 )
      return( 0 );

   if ( nCol < pRow->GetSize( ) )
      return( pRow->GetAt( nCol ) );
   else
   {
#ifdef DEBUG_ALL
      TraceLine( "ZGrid::GetCell requested col (%d) >= size (%d)",
                 nCol, pRow->GetSize( ) );
#endif
      return( 0 );
   }
}

zBOOL
ZGrid::SetCell( long lRow, int nCol, ZGridCellBase *pCell )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   if ( lRow < 0 || lRow >= m_lRows || nCol < 0 || nCol >= m_nCols )
      return( FALSE );

   GRID_ROW *pRow = m_RowData[ lRow ];
   if ( pRow == 0 )
      return( FALSE );

   pCell->SetCoords( lRow, nCol );
   pRow->SetAt( nCol, pCell );

   return( TRUE );
}

// nRemoveSetAdd: -1 - remove; 0 - set; 1 - add
zBOOL
ZGrid::SetAllCellStates( int nCol, DWORD dwState, int nRemoveSetAdd )
{
   if ( GetVirtualMode( ) || nCol < 0 || nCol >= m_nCols )
      return( FALSE );

   ZGridCellBase *pCell;
   GRID_ROW *pRow;
   int  lRow;

   for ( lRow = m_nFixedRows; lRow < m_lRows; lRow++ )
   {
      pRow = m_RowData[ lRow ];
      if ( pRow && (pCell = pRow->GetAt( nCol )) != 0 )
      {
         if ( nRemoveSetAdd > 0 )
            pCell->m_dwState |= dwState;
         else
         if ( nRemoveSetAdd == 0 )
            pCell->m_dwState = dwState;
         else
      // if ( nRemoveSetAdd < 0 )
            pCell->m_dwState &= ~dwState;
      }
   }

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid implementation functions

void
ZGrid::OnDraw( CDC *pDC )
{
   if ( m_bAllowDraw == FALSE )
      return;

   CRect rectClip;
   if ( pDC->GetClipBox( &rectClip ) == ERROR )
      return;

// TraceRect( "ZGrid::OnDraw ClipBox: ", rectClip );

   // OnEraseBkgnd does nothing, so erase bkgnd here.
   // This is necessary since we may be using a Memory DC.
   EraseBkgnd( pDC );

#ifdef _DEBUG
// LARGE_INTEGER nStartCount;
// QueryPerformanceCounter( &nStartCount );
#endif

   CRect rect;
   long  lRow;
   int   nCol;
   int   nRowDiv;
   int   nRowHeight;
   int   nColWidth;
   ZGridCellBase *pCell;

   int  nFixedRowHeight = GetFixedRowHeight( );
   int  nFixedColWidth  = GetFixedColumnWidth( );

   ZCellId idTopLeft = GetTopLeftNonFixedCell( );
   long minVisibleRow = idTopLeft.m_lRow;
   int  minVisibleCol = idTopLeft.m_nCol;

   CRect rectVis;
   ZCellRange VisCellRange = GetVisibleNonFixedCellRange( rectVis );
   int maxVisibleRow = VisCellRange.GetMaxRow( );
   int maxVisibleCol = VisCellRange.GetMaxCol( );

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( VisCellRange );

// if ( zstrcmp( *m_pzsTag, "GridListAccount" ) == 0 )
// {
//    TraceLineS( "", "" );
//    pCell = GetCell( 0, 0 );
// }

   // Draw top-left cells 0..m_nFixedRows - 1, 0..m_nFixedCols - 1
   rect.bottom = -1;
   for ( lRow = 0; lRow < m_nFixedRows; lRow++ )
   {
      if ( (nRowHeight = GetRowHeight( lRow )) <= 0 )
         continue;

      for ( nRowDiv = 0; nRowDiv <= m_nExtraRowsPerItem; nRowDiv++ )
      {
         rect.top = rect.bottom + 1;
         rect.bottom = rect.top + nRowHeight - 1;
         rect.right = -1;

         for ( nCol = 0; nCol < m_nFixedCols; nCol++ )
         {
            if ( (nColWidth = GetColumnWidth( nCol )) <= 0 )
               continue;

            rect.left = rect.right + 1;
            rect.right = rect.left + nColWidth - 1;

            pCell = GetCell( lRow, nCol );
            if ( pCell )
            {
               pCell->SetCoords( lRow, nCol );
               if ( m_nExtraRowsPerItem  )
               {
                  if ( nCol == 0 )
                  {
                     if ( nRowDiv == 0 )
                     {
                        rect.bottom += nRowHeight * m_nExtraRowsPerItem;
                        pCell->Draw( pDC, lRow, nCol, rect, FALSE );
                        rect.bottom -= nRowHeight * m_nExtraRowsPerItem;
                     }
                  }
                  else
                     pCell->Draw( pDC, lRow, nCol, rect, FALSE );
               }
               else
               {
                  pCell->Draw( pDC, lRow, nCol, rect, FALSE );
               }
            }
         }
      }
   }

   // Draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols - 1
   rect.bottom = nFixedRowHeight - 1 + (nRowHeight * m_nExtraRowsPerItem);
   for ( lRow = minVisibleRow; lRow <= maxVisibleRow; lRow++ )
   {
      if ( (nRowHeight = GetRowHeight( lRow )) <= 0 )
         continue;

      for ( nRowDiv = 0; nRowDiv <= m_nExtraRowsPerItem; nRowDiv++ )
      {
         rect.top = rect.bottom + 1;
         rect.bottom = rect.top + nRowHeight - 1;

         // rect.bottom = bottom pixel of previous row
      // if ( rect.top > rectClip.bottom )
      //    break;         // gone past cliprect!

      // if ( rect.bottom < rectClip.top )
      //    continue;      // reached cliprect yet?

         rect.right = -1;
         for ( nCol = 0; nCol < m_nFixedCols; nCol++ )
         {
            if ( (nColWidth = GetColumnWidth( nCol )) <= 0 )
               continue;

            rect.left = rect.right + 1;
            rect.right = rect.left + nColWidth - 1;

         // if ( rect.left > rectClip.right )
         //    break;      // gone past cliprect!

         // if ( rect.right < rectClip.left )
         //    continue;   // reached cliprect yet?

            pCell = GetCell( lRow, nCol );
            if ( pCell )
            {
               pCell->SetCoords( lRow, nCol );
               if ( m_nExtraRowsPerItem  )
               {
                  if ( nCol == 0 )
                  {
                     if ( nRowDiv == 0 )
                     {
                        rect.bottom += nRowHeight * m_nExtraRowsPerItem;
                        pCell->Draw( pDC, lRow, nCol, rect, FALSE );
                        rect.bottom -= nRowHeight * m_nExtraRowsPerItem;
                     }
                  }
                  else
                     pCell->Draw( pDC, lRow, nCol, rect, FALSE );
               }
               else
                  pCell->Draw( pDC, lRow, nCol, rect, FALSE );
            }
         }
      }
   }

   // Draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
   rect.bottom = -1;
   for ( lRow = 0; lRow < m_nFixedRows; lRow++ )
   {
      if ( (nRowHeight = GetRowHeight( lRow )) <= 0 )
         continue;

      rect.top = rect.bottom + 1;
      rect.bottom = rect.top + nRowHeight - 1;

      // rect.bottom = bottom pixel of previous row
   // if ( rect.top > rectClip.bottom )
   //    break;         // gone past cliprect!

   // if ( rect.bottom < rectClip.top )
   //    continue;      // reached cliprect yet?

      rect.right = nFixedColWidth - 1;
      for ( nCol = minVisibleCol; nCol <= maxVisibleCol; nCol++ )
      {
         if ( m_nExtraCol && nCol == m_nExtraCol )
         {
            rect.left = nFixedColWidth;
            rect.right = m_nExtraOffset + nFixedColWidth - 1;
            rect.top = rect.bottom + 1;
            rect.bottom = rect.top + nRowHeight - 1;
            CBrush brush( ::GetSysColor( COLOR_3DFACE ) );
            pDC->FillRect( rect, &brush );
         }

         if ( (nColWidth = GetColumnWidth( nCol )) <= 0 )
            continue;

         rect.left = rect.right + 1;
         rect.right = rect.left + nColWidth - 1;

      // if ( rect.left > rectClip.right )
      // {
      //    if ( nCol > m_nExtraCol )
      //       break;      // gone past cliprect!
      //    else
      //       continue;
      // }

      // if ( rect.right < rectClip.left )
      //    continue;   // reached cliprect yet?

         pCell = GetCell( lRow, nCol );
         if ( pCell )
         {
            pCell->SetCoords( lRow, nCol );
            if ( m_nExtraRowsPerItem  )
            {
               if ( nCol == 0 )
               {
                  if ( nRowDiv == 0 )
                  {
                     rect.bottom += nRowHeight * m_nExtraRowsPerItem;

                     CRect rectExtra( rect );
                     CRect rectInter;

                     rectExtra.left = nFixedColWidth;
                     rectExtra.right = rectVis.right;
                     if ( rectInter.IntersectRect( rectExtra, rectClip ) )
                     {
                        CBrush brush( ::GetSysColor( COLOR_3DFACE ) );
                        pDC->FillRect( rectInter, &brush );
                     }

                     pCell->Draw( pDC, lRow, nCol, rect, FALSE );
                     rect.bottom -= nRowHeight * m_nExtraRowsPerItem;
                  }
                  else
                     pCell->Draw( pDC, lRow, nCol, rect, TRUE );
               }
               else
                  pCell->Draw( pDC, lRow, nCol, rect, TRUE );
            }
            else
               pCell->Draw( pDC, lRow, nCol, rect, TRUE );
         }
      }
   }

   // Draw rest of non-fixed cells.
   rect.bottom = nFixedRowHeight - 1 + (nRowHeight * m_nExtraRowsPerItem);
   for ( lRow = minVisibleRow; lRow <= maxVisibleRow; lRow++ )
   {
      if ( (nRowHeight = GetRowHeight( lRow )) <= 0 )
         continue;

      rect.top = rect.bottom + 1;
      rect.bottom = rect.top + nRowHeight - 1;

      // rect.bottom = bottom pixel of previous row
   // if ( rect.top > rectClip.bottom )
   //    break;         // gone past cliprect!

   // if ( rect.bottom < rectClip.top )
   //    continue;      // reached cliprect yet?

      rect.right = nFixedColWidth - 1;
      for ( nCol = minVisibleCol; nCol <= maxVisibleCol; nCol++ )
      {
         if ( m_nExtraCol && nCol == m_nExtraCol )
         {
            rect.left = nFixedColWidth;
            rect.right = m_nExtraOffset + nFixedColWidth - 1;
            rect.top = rect.bottom + 1;
            rect.bottom = rect.top + nRowHeight - 1;
            CBrush brush( ::GetSysColor( COLOR_3DFACE ) );
            pDC->FillRect( rect, &brush );
         }

         if ( (nColWidth = GetColumnWidth( nCol )) <= 0 )
            continue;

         rect.left = rect.right + 1;
         rect.right = rect.left + nColWidth - 1;

      // if ( rect.left > rectClip.right )
      // {
      //    if ( nCol > m_nExtraCol )
      //       break;      // gone past cliprect!
      //    else
      //       continue;
      // }

      // if ( rect.right < rectClip.left )
      //    continue;   // reached cliprect yet?

         pCell = GetCell( lRow, nCol );
         // TRACE( _T( "Cell %d,%d type: %s\n" ), lRow, nCol,
         //        pCell->GetRuntimeClass( )->m_lpszClassName );
         if ( pCell )
         {
            pCell->SetCoords( lRow, nCol );
            if ( m_nExtraRowsPerItem  )
            {
               if ( nCol == 0 )
               {
                  if ( nRowDiv == 0 )
                  {
                     rect.bottom += nRowHeight * m_nExtraRowsPerItem;
                     pCell->Draw( pDC, lRow, nCol, rect, FALSE );
                     rect.bottom -= nRowHeight * m_nExtraRowsPerItem;
                  }
                  else
                     pCell->Draw( pDC, lRow, nCol, rect, FALSE );
               }
               else
                  pCell->Draw( pDC, lRow, nCol, rect, FALSE );
            }
            else
               pCell->Draw( pDC, lRow, nCol, rect, FALSE );
         }
      }
   }

   CPen pen;
   pen.CreatePen( PS_SOLID, 0, m_clrGridLine );
   pDC->SelectObject( &pen );

   // Draw vertical lines (drawn at ends of cells).
   if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT )
   {
      zBOOL bFirstLine = TRUE;
      int x = nFixedColWidth;
      int y;

      for ( nCol = minVisibleCol; nCol <= maxVisibleCol; nCol++ )
      {
         if ( m_nExtraCol && nCol == m_nExtraCol )
         {
            bFirstLine = FALSE;
            x = m_nExtraOffset + nFixedColWidth;
         }

         if ( (nColWidth = GetColumnWidth( nCol )) > 0 )
         {
            x += nColWidth;
            y = nFixedRowHeight * (m_nExtraRowsPerItem + 1);
            for ( lRow = minVisibleRow; lRow <= maxVisibleRow; lRow++ )
            {
               if ( (nRowHeight = GetRowHeight( lRow )) <= 0 )
                  continue;

               if ( bFirstLine == FALSE )
                  y += nRowHeight;

               pDC->MoveTo( x - 1, y );
               pDC->LineTo( x - 1, y + nRowHeight );

               if ( bFirstLine )
                  y += nRowHeight * (m_nExtraRowsPerItem + 1);
               else
                  y += nRowHeight;
            }
         }
      }
   }

   // Draw horizontal lines (drawn at bottom of each cell).
   if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ )
   {
      int y = nFixedRowHeight * (m_nExtraRowsPerItem + 1);

      pDC->MoveTo( nFixedColWidth, y - 1 );
      pDC->LineTo( rectVis.right,  y - 1 );
      for ( lRow = minVisibleRow; lRow <= maxVisibleRow; lRow++ )
      {
         if ( (nRowHeight = GetRowHeight( lRow )) <= 0 )
            continue;

         if ( m_nExtraRowsPerItem )
         {
            y += nRowHeight;
            pDC->MoveTo( nFixedColWidth, y - 1 );
            pDC->LineTo( rectVis.right,  y - 1 );
         }

         y += nRowHeight;
         pDC->MoveTo( nFixedColWidth, y - 1 );
         pDC->LineTo( rectVis.right,  y - 1 );
      }
   }

   pDC->SelectStockObject( NULL_PEN );

   // Let parent know it can discard its data if it needs to.
   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( -1, -1, -1, -1 ) );

#ifdef _DEBUG
// LARGE_INTEGER iEndCount;
// QueryPerformanceCounter( &iEndCount );
// TRACE1( "Draw counter ticks: %d\n", iEndCount.LowPart-iStartCount.LowPart );
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// ZGrid Cell selection stuff

// Is a given cell designation valid (i.e. within the bounds of our number
// of columns/rows)?
zBOOL
ZGrid::IsValid( long lRow, int nCol ) const
{
   return( lRow >= 0 && lRow < m_lRows && nCol >= 0 && nCol < m_nCols );
}

zBOOL
ZGrid::IsValid( const ZCellId& cell ) const
{
   return( IsValid( cell.m_lRow, cell.m_nCol ) );
}

// Is a given cell range valid (i.e. within the bounds of our number
// of columns/rows)?
zBOOL
ZGrid::IsValid( const ZCellRange& range ) const
{
   return( range.GetMinRow( ) >= 0 && range.GetMinCol( ) >= 0 &&
           range.GetMaxRow( ) >= 0 && range.GetMaxCol( ) >= 0 &&
           range.GetMaxRow( ) < m_lRows && range.GetMaxCol( ) < m_nCols &&
           range.GetMinRow( ) <= range.GetMaxRow( ) &&
           range.GetMinCol( ) <= range.GetMaxCol( ) );
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void
ZGrid::SetRedraw( BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */ )
{
//   TRACE( _T( "%s: Setting redraw to %s\n" ),
//          GetRuntimeClass( )->m_lpszClassName, bAllowDraw? _T( "TRUE" ) : _T( "FALSE" ) );

   if ( bAllowDraw && m_bAllowDraw == FALSE )
   {
      m_bAllowDraw = TRUE;
      Refresh( );
   }

   m_bAllowDraw = bAllowDraw;
   if ( bResetScrollBars )
      ResetScrollBars( );
}

// Forces a redraw of a cell immediately (using a direct DC construction,
// or the supplied dc).
zBOOL
ZGrid::RedrawCell( const ZCellId& cell, CDC *pDC /* = 0 */ )
{
   return( RedrawCell( cell.m_lRow, cell.m_nCol, pDC ) );
}

zBOOL
ZGrid::RedrawCell( long lRow, int nCol, CDC *pDC /* = 0 */ )
{
   zBOOL bResult = TRUE;
   zBOOL bMustReleaseDC = FALSE;

   if ( m_bAllowDraw == FALSE || IsCellVisible( lRow, nCol ) == FALSE )
      return( FALSE );

// TraceLine( "ZGrid::RedrawCell: Row: %d  Col: %d", lRow, nCol );
   CRect rect;
   if ( GetCellRect( lRow, nCol, rect ) == FALSE )
      return( FALSE );

   if ( m_nExtraRowsPerItem && nCol == m_nFixedCols - 1 )
      rect.bottom += GetRowHeight( lRow ) * m_nExtraRowsPerItem;

   if ( pDC == 0 )
   {
      pDC = GetDC( );
      if ( pDC )
         bMustReleaseDC = TRUE;
   }

   if ( pDC )
   {
      // Redraw cells directly.
      if ( lRow < m_nFixedRows || nCol < m_nFixedCols )
      {
         ZGridCellBase *pCell = GetCell( lRow, nCol );
         if ( pCell )
            bResult = pCell->Draw( pDC, lRow, nCol, rect, TRUE );
      }
      else
      {
         ZGridCellBase *pCell = GetCell( lRow, nCol );
         if ( pCell )
            bResult = pCell->Draw( pDC, lRow, nCol, rect, TRUE );

         // Since we have erased the background, we will need to redraw the gridlines
         CPen pen;
         pen.CreatePen( PS_SOLID, 0, m_clrGridLine );

         CPen *pOldPen = (CPen *) pDC->SelectObject( &pen );
         if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ )
         {
            pDC->MoveTo( rect.left, rect.bottom );
            pDC->LineTo( rect.right + 1, rect.bottom );
         }

         if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT )
         {
            pDC->MoveTo( rect.right, rect.top );
            pDC->LineTo( rect.right, rect.bottom + 1 );
         }

         pDC->SelectObject( pOldPen );
      }
   }
   else
      InvalidateRect( rect, TRUE ); // Could not get a DC - invalidate it anyway
                                    // and hope that OnPaint manages to get one

   if ( bMustReleaseDC )
      ReleaseDC( pDC );

   return( bResult );
}

// redraw a complete row
zBOOL
ZGrid::RedrawRow( long lRow )
{
   zBOOL bResult = TRUE;
   int  nColCnt = GetColumnCount( );

   CDC *pDC = GetDC( );
   for ( int nCol = 0; nCol < nColCnt; nCol++ )
      bResult = RedrawCell( lRow, nCol, pDC ) && bResult;

   if ( pDC )
      ReleaseDC( pDC );

   return( bResult );
}

// redraw a complete column
zBOOL
ZGrid::RedrawColumn( int nCol )
{
   zBOOL bResult = TRUE;
   long  lRowCnt = GetRowCount( );

   CDC *pDC = GetDC( );
   for ( long lRow = 0; lRow < lRowCnt; lRow++ )
      bResult = RedrawCell( lRow, nCol, pDC ) && bResult;

   if ( pDC )
      ReleaseDC( pDC );

   return( bResult );
}

// Sets the currently selected cell, returning the previous current cell.
ZCellId
ZGrid::SetFocusCell( long lRow, int nCol )
{
   return( SetFocusCell( ZCellId( lRow, nCol ) ) );
}

ZCellId
ZGrid::SetFocusCell( ZCellId cellId )
{
   if ( cellId == m_cellIdCurrent )
      return( m_cellIdCurrent );

   int  nFixedColCnt = GetFixedColumnCount( );
   int  nFixedRowCnt = GetFixedRowCount( );
   ZCellId idPrev = m_cellIdCurrent;

   // EFW - Bug Fix - Force focus to be in a non-fixed cell.
   if ( cellId.m_lRow != -1 && cellId.m_lRow < nFixedRowCnt )
      cellId.m_lRow = nFixedRowCnt;

   if ( cellId.m_nCol != -1 && cellId.m_nCol < nFixedColCnt )
      cellId.m_nCol = nFixedColCnt;

   m_cellIdCurrent = cellId;

   if ( IsValid( idPrev ) )
   {
      SetItemState( idPrev.m_lRow, idPrev.m_nCol,
                    GetItemState( idPrev.m_lRow,
                                  idPrev.m_nCol ) & ~GVIS_FOCUSED );
      RedrawCell( idPrev ); // comment to reduce flicker

      if ( GetTrackFocusCell( ) && idPrev.m_nCol != m_cellIdCurrent.m_nCol )
      {
         for ( long lRow = 0; lRow < m_nFixedRows; lRow++ )
            RedrawCell( lRow, idPrev.m_nCol );
      }

      if ( GetTrackFocusCell( ) && idPrev.m_lRow != m_cellIdCurrent.m_lRow )
      {
         for ( int nCol = 0; nCol < m_nFixedCols; nCol++ )
            RedrawCell( idPrev.m_lRow, nCol );
      }
   }

   if ( IsValid( m_cellIdCurrent ) )
   {
      SetItemState( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol,
                    GetItemState( m_cellIdCurrent.m_lRow,
                                  m_cellIdCurrent.m_nCol ) | GVIS_FOCUSED );

      RedrawCell( m_cellIdCurrent ); // comment to reduce flicker

      if ( GetTrackFocusCell( ) && idPrev.m_nCol != m_cellIdCurrent.m_nCol )
      {
         for ( long lRow = 0; lRow < m_nFixedRows; lRow++ )
            RedrawCell( lRow, m_cellIdCurrent.m_nCol );
      }

      if ( GetTrackFocusCell( ) && idPrev.m_lRow != m_cellIdCurrent.m_lRow )
      {
         for ( int nCol = 0; nCol < m_nFixedCols; nCol++ )
            RedrawCell( m_cellIdCurrent.m_lRow, nCol );
      }

      // EFW - New addition.  If in list mode, make sure the selected
      // row highlight follows the cursor.
      // Removed by C Maunder 27 May
      //if ( m_bListMode )
      //{
      //   m_PrevSelectedCellMap.RemoveAll( );
      //   m_MouseMode = MOUSE_SELECT_ROW;
      //   OnSelecting( m_cellIdCurrent );

          // Leave this off so that you can still drag the highlight around
          // without selecting rows.
          // m_MouseMode = MOUSE_NOTHING;
      //}
   }

   return( idPrev );
}

// Sets the range of currently selected cells
void
ZGrid::SetSelectedRange( const ZCellRange& Range,
                         zBOOL bForceRepaint /* = FALSE */,
                         zBOOL bSelectCells /*=TRUE*/ )
{
   SetSelectedRange( Range.GetMinRow( ), Range.GetMinCol( ),
                     Range.GetMaxRow( ), Range.GetMaxCol( ),
                     bForceRepaint, bSelectCells );
}

void
ZGrid::SetSelectedRange( long lMinRow, int nMinCol, long lMaxRow, int nMaxCol,
                         zBOOL bForceRepaint /* = FALSE */,
                         zBOOL bSelectCells /* = TRUE */ )
{
   if ( m_bEnableSelection == FALSE )
      return;

   CWaitCursor wait; // Thomas Haase
   int  nFixedRowCnt = GetFixedRowCount( );
   int  nFixedColCnt = GetFixedColumnCount( );

   CDC *pDC = 0;
   if ( bForceRepaint )
      pDC = GetDC( );

// TraceLine( "ZGrid::SetSelectedRange0 CurrentCell: (%d, %d)",
//            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );

   // Only redraw visible cells.
   ZCellRange VisCellRange;
   if ( mIs_hWnd( GetSafeHwnd( ) ) )
      VisCellRange = GetVisibleNonFixedCellRange( );

   // EFW - Bug fix - Don't allow selection of fixed rows
   if ( lMinRow >= 0 && lMinRow < nFixedRowCnt )
      lMinRow = nFixedRowCnt;

   if ( lMaxRow >= 0 && lMaxRow < nFixedRowCnt )
      lMaxRow = nFixedRowCnt;

   if ( nMinCol >= 0 && nMinCol < nFixedColCnt )
      nMinCol = nFixedColCnt;

   if ( nMaxCol >= 0 && nMaxCol < nFixedColCnt )
      nMaxCol = nFixedColCnt;

// TraceLine( "ZGrid::SetSelectedRange Min: (%d, %d)   Max: (%d, %d)  %s",
//            lMinRow, nMinCol, lMaxRow, nMaxCol, bSelectCells ? "SELECT" : "NOT SELECT" );

   // If we are selecting cells, first clear out the list of currently
   // selected cells, then ...
   if ( bSelectCells )
   {
      POSITION pos;

      // Unselect all previously selected cells.
      for ( pos = m_SelectedCellMap.GetStartPosition( ); pos; )
      {
         LARGE_INTEGER key;
         ZCellId cellId;
         m_SelectedCellMap.GetNextAssoc( pos, key, (ZCellId &) cellId );

         // Reset the selection flag on the cell.
         if ( IsValid( cellId ) )
         {
            // This will remove the cell from the m_SelectedCellMap map.
            SetItemState( cellId.m_lRow, cellId.m_nCol,
                          GetItemState( cellId.m_lRow,
                                        cellId.m_nCol ) & ~GVIS_SELECTED );

            // If this is to be reselected, continue on past the redraw
            if ( lMinRow <= cellId.m_lRow && cellId.m_lRow <= lMaxRow &&
                 nMinCol <= cellId.m_nCol && cellId.m_nCol <= nMaxCol )
            {
               continue;
            }

            if ( VisCellRange.IsValid( ) && VisCellRange.InRange( cellId ) )
            {
               if ( bForceRepaint && pDC )           // redraw NOW
                  RedrawCell( cellId.m_lRow, cellId.m_nCol, pDC );
               else
                  InvalidateCellRect( cellId );      // redraw at leisure
            }
         }
         else
         {
            m_SelectedCellMap.RemoveKey( key );  // if it's not valid,
                                                 // get rid of it!
         }
      }

      // If we are selecting cells, and there are previous selected cells
      // to be retained (e.g. Ctrl is being held down) then copy them to
      // the newly created list, and mark all these cells as selected.
      // Note that if we are list mode, single row selection, then we won't
      // be adding the previous cells. Only the current row of cells will be
      // added (see below).
      if ( GetSingleRowSelection( ) == 0 &&
           lMinRow >= 0 && nMinCol >= 0 && lMaxRow >= 0 && nMaxCol >= 0 )
      {
         for ( pos = m_PrevSelectedCellMap.GetStartPosition( ); pos; /* nothing */ )
         {
            LARGE_INTEGER key;
            ZCellId cellId;
            m_PrevSelectedCellMap.GetNextAssoc( pos, key, (ZCellId &) cellId );

            if ( IsValid( cellId ) == FALSE )
               continue;

            UINT uState = GetItemState( cellId.m_lRow, cellId.m_nCol );

            // Set state as Selected. This will add the cell to m_SelectedCellMap.
            SetItemState( cellId.m_lRow, cellId.m_nCol, uState | GVIS_SELECTED );

            if ( VisCellRange.IsValid( ) && VisCellRange.InRange( cellId ) )
            {
               // Redraw (immediately or at leisure).
               if ( bForceRepaint && pDC )
                  RedrawCell( cellId.m_lRow, cellId.m_nCol, pDC );
               else
                  InvalidateCellRect( cellId );
            }
         }
      }
   }

// TraceLine( "ZGrid::SetSelectedRange1 CurrentCell: (%d, %d)",
//            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );

   // Now select/deselect all cells in the cell range specified. If
   // selecting, and the cell has already been marked as selected
   // (above) then ignore it. If we are deselecting and the cell isn't
   // selected, then ignore.
   if ( lMinRow >= 0 && nMinCol >= 0 && lMaxRow >= 0 && nMaxCol >= 0 &&
        lMaxRow < m_lRows && nMaxCol < m_nCols &&
        lMinRow <= lMaxRow && nMinCol <= nMaxCol )
   {
      for ( long lRow = lMinRow; lRow <= lMaxRow; lRow++ )
      {
         for ( int nCol = nMinCol; nCol <= nMaxCol; nCol++ )
         {
            zBOOL bCellSelected = IsCellSelected( lRow, nCol );
            if ( bSelectCells == bCellSelected )
               continue;   // Already selected or deselected - ignore

            // Set the selected state. This will add/remove the cell to
            // m_SelectedCellMap.
            if ( bSelectCells )
               SetItemState( lRow, nCol,
                             GetItemState( lRow, nCol ) | GVIS_SELECTED );
            else
               SetItemState( lRow, nCol,
                             GetItemState( lRow, nCol ) & ~GVIS_SELECTED );

            if ( VisCellRange.IsValid( ) &&
                 VisCellRange.InRange( lRow, nCol ) )
            {
               // Redraw (immediately or at leisure).
               if ( bForceRepaint && pDC )
                  RedrawCell( lRow, nCol, pDC );
               else
                  InvalidateCellRect( lRow, nCol );
            }
         }
      }
   }

// TraceLine( "ZGrid::SetSelectedRange2 CurrentCell: (%d, %d)",
//            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
// TRACE( _T( "%d cells selected.\n" ), m_SelectedCellMap.GetCount( ) );

   if ( pDC )
      ReleaseDC( pDC );
}

// Selects all cells.
void
ZGrid::SelectAllCells( )
{
   if ( m_bEnableSelection == FALSE || m_chMultiSelect != 'Y' )
      return;

   SetSelectedRange( m_nFixedRows, m_nFixedCols,
                     GetRowCount( ) - 1, GetColumnCount( ) - 1 );
}

// Selects columns.
void
ZGrid::SelectColumns( ZCellId currentCell,
                      zBOOL bForceRedraw /*=FALSE*/,
                      zBOOL bSelectCells /*=TRUE*/ )
{
   if ( m_bEnableSelection == FALSE )
      return;

// if ( currentCell.m_nCol == m_cellIdCurrent.m_nCol ) return;
   if ( currentCell.m_nCol < m_nFixedCols )
      return;

   if ( IsValid( currentCell ) == FALSE )
      return;

   int  nFixedRowCnt = GetFixedRowCount( );
   int  nRowCnt = GetRowCount( );

   if ( GetSingleColSelection( ) )
      SetSelectedRange( nFixedRowCnt, currentCell.m_nCol,
                        nRowCnt - 1, currentCell.m_nCol,
                        bForceRedraw, bSelectCells );
   else
      SetSelectedRange( nFixedRowCnt,
                        min( m_cellSelectionStart.m_nCol, currentCell.m_nCol ),
                        nRowCnt - 1,
                        max( m_cellSelectionStart.m_nCol, currentCell.m_nCol ),
                        bForceRedraw, bSelectCells );
}

// Selects rows.
void
ZGrid::SelectRows( ZCellId currentCell,
                   zBOOL bForceRedraw /*=FALSE*/,
                   zBOOL bSelectCells /*=TRUE*/ )
{
   if ( m_bEnableSelection == FALSE )
      return;

// if ( currentCell.m_lRow; == m_cellIdCurrent.m_lRow )
//    return;

   if ( currentCell.m_lRow < m_nFixedRows )
      return;

   if ( IsValid( currentCell ) == FALSE )
      return;

   int  nFixedColCnt = GetFixedColumnCount( );
   int  nColCnt = GetColumnCount( );

   if ( GetSingleRowSelection( ) )
      SetSelectedRange( currentCell.m_lRow, nFixedColCnt,
                        currentCell.m_lRow, nColCnt - 1,
                        bForceRedraw, bSelectCells );
   else
      SetSelectedRange( min( m_cellSelectionStart.m_lRow, currentCell.m_lRow ),
                        nFixedColCnt,
                        max( m_cellSelectionStart.m_lRow, currentCell.m_lRow ),
                        nColCnt - 1,
                        bForceRedraw, bSelectCells );
}

// Selects cells.
void
ZGrid::SelectCells( ZCellId currentCell,
                    zBOOL bForceRedraw /*=FALSE*/, zBOOL bSelectCells /*=TRUE*/ )
{
   if ( m_bEnableSelection == FALSE )
      return;

#ifdef DEBUG_ALL
   TraceLine( "ZGrid::SelectCells CurrentCell: (%d, %d)",
              currentCell.m_lRow, currentCell.m_nCol );
#endif

   long lRow = currentCell.m_lRow;
   int nCol = currentCell.m_nCol;
   if ( lRow < m_nFixedRows || nCol < m_nFixedCols )
      return;

   if ( IsValid( currentCell ) == FALSE )
      return;

   // Prevent unnecessary redraws
// if ( currentCell == m_cellLeftClickDown )
//    return;
// else
// if ( currentCell == m_cellIdCurrent )
//    return;

   SetSelectedRange( min( m_cellSelectionStart.m_lRow, lRow ),
                     min( m_cellSelectionStart.m_nCol, nCol ),
                     max( m_cellSelectionStart.m_lRow, lRow ),
                     max( m_cellSelectionStart.m_nCol, nCol ),
                     bForceRedraw, bSelectCells );
}

// Called when mouse/keyboard selection is a-happening.
void
ZGrid::OnSelecting( const ZCellId& currentCell )
{
   if ( m_bEnableSelection == FALSE )
      return;

   switch ( m_MouseMode )
   {
      case MOUSE_SELECT_ALL:
         SelectAllCells( );
         break;

      case MOUSE_SELECT_COL:
         SelectColumns( currentCell, FALSE );
         break;

      case MOUSE_SELECT_ROW:
         SelectRows( currentCell, FALSE );
         break;

      case MOUSE_SELECT_CELLS:
         SelectCells( currentCell, FALSE );
         break;
   }

   // EFW - Bug fix [REMOVED CJM: this will cause infinite loop in list mode]
   // SetFocusCell( max( currentCell.m_lRow, m_nFixedRows ),
   //               max( currentCell.m_nCol, m_nFixedCols ) );
}

void
ZGrid::ValidateAndModifyCellContents( long lRow, int nCol, zCPCHAR cpcText )
{
   if ( IsCellEditable( lRow, nCol ) == FALSE )
      return;

   if ( SendMessageToParent( lRow, nCol, GVN_BEGINEDIT ) >= 0 )
   {
      CString csCurrentText;

      GetItemText( csCurrentText, lRow, nCol );
      if ( csCurrentText != cpcText )
      {
         SetItemText( lRow, nCol, cpcText );
         if ( ValidateEdit( lRow, nCol, cpcText ) &&
              SendMessageToParent( lRow, nCol, GVN_ENDEDIT ) >= 0 )
         {
            SetModified( TRUE, lRow, nCol );
            RedrawCell( lRow, nCol );
         }
         else
         {
            SetItemText( lRow, nCol, csCurrentText );
         }
      }
   }
}

void
ZGrid::ClearCells( ZCellRange Selection )
{
   for ( long lRow = Selection.GetMinRow( );
         lRow <= Selection.GetMaxRow( );
         lRow++ )
   {
      for ( int nCol = Selection.GetMinCol( );
            nCol <= Selection.GetMaxCol( );
            nCol++ )
      {
         // Don't clear hidden cells.
         if ( m_arRowHeights[ lRow ] > 0 && m_arColWidths[ nCol ] > 0 )
         {
            ValidateAndModifyCellContents( lRow, nCol, _T( "" ) );
         }
      }
   }

   Refresh( );
}

#ifndef GRIDCONTROL_NO_CLIPBOARD

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

// Deletes the contents from the selected cells.
void
ZGrid::CutSelectedText( )
{
   if ( IsEditable( ) == FALSE )
      return;

   for ( POSITION pos = m_SelectedCellMap.GetStartPosition( ); pos; )
   {
      LARGE_INTEGER key;
      ZCellId cellId;
      m_SelectedCellMap.GetNextAssoc( pos, key, (ZCellId &) cellId );
      ValidateAndModifyCellContents( cellId.m_lRow, cellId.m_nCol, _T( "" ) );
   }
}

void
ZGrid::WriteRow( CSharedFile& sf, ZCellRange& Selection,
                 zLONG lMinRow, zLONG lMaxRow, int nMinCol, int nMaxCol )
{
   // Get a tab delimited string to copy to cache.
   ZGridCellBase *pCell;
   CString cs;
   long lRow;
   int  nCol;

   for ( lRow = lMinRow; lRow <= lMaxRow; lRow++ )
   {
      // Don't copy hidden cells.
      if ( m_arRowHeights[ lRow ] <= 0 )
         continue;

      CString csText;
      cs.Empty( );
      for ( nCol = Selection.GetMinCol( );
            nCol <= Selection.GetMaxCol( );
            nCol++ )
      {
         // Don't copy hidden cells.
         if ( m_arColWidths[ nCol ] <= 0 )
            continue;

         pCell = GetCell( lRow, nCol );
         if ( pCell && ((pCell->GetState( ) & GVIS_SELECTED) || lRow < m_nFixedRows) )
         {
            pCell->GetText( csText, lRow, nCol );
         // if ( !pCell->GetText( lRow, nCol ) )
         //    cs += _T( " " );
         // else
               cs += csText;
         }

         if ( nCol != Selection.GetMaxCol( ) )
            cs += _T( "\t" );
      }

      if ( lRow != Selection.GetMaxRow( ) )
         cs += _T( "\n" );

      sf.Write( T2A( cs.GetBuffer( 1 ) ), cs.GetLength( ) );
      cs.ReleaseBuffer( );
   }
}

// Copies text from the selected cells to the clipboard.
COleDataSource *
ZGrid::CopyTextFromGrid( )
{
   USES_CONVERSION;

   ZCellRange Selection = GetSelectedCellRange( );
   if ( IsValid( Selection ) == FALSE )
      return( 0 );

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( Selection );

   // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE,
   // so we need to convert).
   CSharedFile sf( GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT );

   long lMinRow = Selection.GetMinRow( );
   long lMaxRow = Selection.GetMaxRow( );
   int  nMinCol = Selection.GetMinCol( );
   int  nMaxCol = Selection.GetMaxCol( );

   if ( m_nFixedRows && m_bSelectAll)
      WriteRow( sf, Selection, 0, 0, nMinCol, nMaxCol );

   WriteRow( sf, Selection, lMinRow, lMaxRow, nMinCol, nMaxCol );

   zCHAR c = 0;
   sf.Write( &c, 1 );

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( -1, -1, -1, -1 ) );

   DWORD dwLth = (DWORD) sf.GetLength( );
   HGLOBAL hMem = sf.Detach( );
   if ( hMem == 0 )
      return( 0 );

   hMem = ::GlobalReAlloc( hMem, dwLth,
                           GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT );
   if ( hMem == 0 )
      return( 0 );

   // Cache data.
   COleDataSource *pSource = new COleDataSource( );
   pSource->CacheGlobalData( CF_TEXT, hMem );

   return( pSource );
}

// Pastes text from the clipboard to the selected cells.
zBOOL
ZGrid::PasteTextToGrid( ZCellId cellId, COleDataObject *pDataObject,
                        zBOOL bSelectPastedCells /*=TRUE*/ )
{
   if ( IsValid( cellId ) == FALSE || IsCellEditable( cellId ) == FALSE ||
        pDataObject->IsDataAvailable( CF_TEXT ) == FALSE )
   {
      return( FALSE );
   }

   // Get the text from the COleDataObject.
   HGLOBAL hmem = pDataObject->GetGlobalData( CF_TEXT );
   CMemFile sf( (BYTE *) ::GlobalLock( hmem ), ::GlobalSize( hmem ) );

   // CF_TEXT is ANSI text, so we need to allocate a char buffer to hold this.
   LPSTR pchBuffer = new char[ ::GlobalSize( hmem ) ];
   if ( pchBuffer == 0 )
      return( FALSE );

   sf.Read( pchBuffer, ::GlobalSize( hmem ) );
   ::GlobalUnlock( hmem );

   // Now store in generic TCHAR form so we no longer have to deal with
   // ANSI/UNICODE problems.
   CString csText = pchBuffer;
   delete [] pchBuffer;

   // Parse text data and set in cells ...
   csText.LockBuffer( );
   CString csLine = csText;
   zLONG   lLine = 0;
   zLONG   lRowCnt = GetRowCount( );
   int     nColCnt = GetColumnCount( );
   zSHORT  nFlag;
// zBOOL   bSkipLine = TRUE;

   zGRID_CELL cell;
   zLONG lRC;

   // Find the end of the first line.
   ZCellRange PasteRange( cellId.m_lRow, cellId.m_nCol, -1, -1 );
   int nIndex;
   do
   {
      int nCol = 0;
      nIndex = csLine.Find( _T( "\n" ) );

      // Store the remaining chars after the newline.
      CString csNext = (nIndex < 0) ? _T( "" ) : csLine.Mid( nIndex + 1 );

#if 1 // We are not going to paste to the grid, rather we will create a new
      // entity for each "line" and set the data.  In the end we will refresh
      // the grid to show the new data.
      CreateEntity( m_vAppList, *m_pzsEName, zPOS_LAST );
//    if ( bSkipLine )
//    {
//       csLine = csNext;
//       bSkipLine = FALSE;
//    }
#endif

      // Remove all chars after the newline.
      if ( nIndex >= 0 )
         csLine = csLine.Left( nIndex );

      int nLineIndex = csLine.FindOneOf( _T( "\t," ) );
      CString csCellText = (nLineIndex >= 0) ? csLine.Left( nLineIndex ) : csLine;

      // Skip hidden rows.
      int nRowVis = cellId.m_lRow + lLine;
      while ( nRowVis < lRowCnt )
      {
         if ( GetRowHeight( nRowVis ) > 0 )
            break;

         lLine++;
         nRowVis++;
      }

      while ( csLine.IsEmpty( ) == FALSE && nCol < nColCnt )
      {
#if 1
         nCol++;  // increment prior to SetAttribute
         nFlag = (m_pCol[ nCol ].pchContext &&
                  m_pCol[ nCol ].pchContext[ 0 ]) ? 0 : zUSE_DEFAULT_CONTEXT;

         cell.lRow = lRowCnt + lLine + 1 - m_nFixedRows;
         cell.nCol = nCol + 1 - m_nFixedCols;

         TraceLine( "ZGrid::PasteTextToGrid BeforePasteCell( %d, %d ) Text: %s", cell.lRow, cell.nCol, csCellText );
         cell.pchText = csCellText.GetBuffer( csCellText.GetLength( ) );
         lRC = ProcessImmediateEvent( this, zGRDN_BEFOREPASTECELL, &cell );
         if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
         {
            if ( SetAttributeFromVariable( m_vAppList,
                                           m_pCol[ nCol ].pchEntity,
                                           m_pCol[ nCol ].pchAttrib,
                                           csCellText, zTYPE_STRING,
                                           zstrlen( csCellText ),
                                           m_pCol[ nCol ].pchContext,
                                           nFlag | zVALIDATE ) == 0 )
            {
               TraceLine( "Paste Entity: %s   Attribute: %s   Value: %s",
                                         m_pCol[ nCol ].pchEntity,
                                         m_pCol[ nCol ].pchAttrib,
                                         csCellText );
               SetAttributeFromVariable( m_vAppList,
                                         m_pCol[ nCol ].pchEntity,
                                         m_pCol[ nCol ].pchAttrib,
                                         csCellText, zTYPE_STRING,
                                         zstrlen( csCellText ),
                                         m_pCol[ nCol ].pchContext,
                                         nFlag );
               lRC = ProcessImmediateEvent( this, zGRDN_AFTERPASTECELL, &cell );
            }
         }

         csCellText.ReleaseBuffer( );
#else
         // Skip hidden columns.
         int nColVis = cellId.m_nCol + nCol;
         while ( nColVis < nColCnt )
         {
            if ( GetColumnWidth( nColVis ) > 0 )
               break;

            nCol++;
            nColVis++;
         }

         ZCellId TargetCell( nRowVis, nColVis );
         if ( IsValid( TargetCell ) )
         {
            csCellText.TrimLeft( );
            csCellText.TrimRight( );

            ValidateAndModifyCellContents( TargetCell.m_lRow, TargetCell.m_nCol,
                                           csCellText );

            // Make sure cell is not selected to avoid data loss.
            SetItemState( TargetCell.m_lRow, TargetCell.m_nCol,
                          GetItemState( TargetCell.m_lRow,
                                        TargetCell.m_nCol ) & ~GVIS_SELECTED );

            if ( nRowVis > PasteRange.GetMaxRow( ) )
               PasteRange.SetMaxRow( nRowVis );

            if ( nColVis > PasteRange.GetMaxCol( ) )
               PasteRange.SetMaxCol( nColVis );
         }

         nCol++;
#endif

         csLine = (nLineIndex >= 0) ? csLine.Mid( nLineIndex + 1 ) : _T( "" );
         nLineIndex = csLine.FindOneOf( _T( "\t," ) );
         csCellText = (nLineIndex >= 0) ? csLine.Left( nLineIndex ) : csLine;
      }

      csLine = csNext;
      lLine++;

   } while ( nIndex >= 0 );

   csText.UnlockBuffer( );

   if ( bSelectPastedCells )
     SetSelectedRange( PasteRange, TRUE );
   else
   {
      ResetSelectedRange( );
      Refresh( );
   }

   MapFromOI( );
   return( TRUE );
}

void
ZGrid::OnEditCut( )
{
   if ( IsEditable( ) == FALSE )
      return;

   COleDataSource *pSource = CopyTextFromGrid( );
   if ( pSource == 0 )
      return;

   pSource->SetClipboard( );
   CutSelectedText( );
}

void
ZGrid::OnEditCopy( )
{
   COleDataSource *pSource = CopyTextFromGrid( );
   if ( pSource == 0 )
      return;

   pSource->SetClipboard( );
}

void
ZGrid::OnEditPaste( )
{
   if ( IsEditable( ) == FALSE )
      return;

   ZCellRange cellRange = GetSelectedCellRange( );

   // Get the top-left selected cell, or the Focus cell, or the
   // topleft (non-fixed) cell.
   ZCellId cellId;
   if ( cellRange.IsValid( ) )
   {
      cellId.m_lRow = cellRange.GetMinRow( );
      cellId.m_nCol = cellRange.GetMinCol( );
   }
   else
   {
      cellId = GetFocusCell( );
      if ( IsValid( cellId ) == FALSE )
         cellId = GetTopLeftNonFixedCell( );

      if ( IsValid( cellId ) == FALSE )
         return;
   }

   // If a cell is being edited, then call its edit window paste function.
   if ( IsItemEditing( cellId.m_lRow, cellId.m_nCol ) )
   {
      ZGridCellBase *pCell = GetCell( cellId.m_lRow, cellId.m_nCol );
      ASSERT( pCell );
      if ( pCell == 0 )
         return;

      CWnd *pEditWnd = pCell->GetEditWnd( );
      if ( pEditWnd && pEditWnd->IsKindOf( RUNTIME_CLASS( CEdit ) ) )
      {
         ((CEdit *) pEditWnd)->Paste( );
         return;
      }
   }

   // Attach a COleDataObject to the clipboard and paste the data to the grid.
   COleDataObject obj;
   if ( obj.AttachClipboard( ) )
      PasteTextToGrid( cellId, &obj );
}

void
ZGrid::OnUpdateEditCopyOrCut( CCmdUI *pCmdUI )
{
   pCmdUI->Enable( GetSelectedCount( ) > 0 );
}

void
ZGrid::OnUpdateEditCopy( CCmdUI *pCmdUI )
{
   pCmdUI->Enable( GetSelectedCount( ) > 0 ); // - Thomas Haase
   //ZCellRange Selection = GetSelectedCellRange( );
   //pCmdUI->Enable( Selection.Count( ) && IsValid( Selection ) );
}

void
ZGrid::OnUpdateEditCut( CCmdUI *pCmdUI )
{
   pCmdUI->Enable( IsEditable( ) && GetSelectedCount( ) > 0 ); // - Thomas Haase
   //ZCellRange Selection = GetSelectedCellRange( );
   //pCmdUI->Enable( IsEditable( ) && Selection.Count( ) && IsValid( Selection ) );
}

void
ZGrid::OnUpdateEditPaste( CCmdUI *pCmdUI )
{
   ZCellId cellId = GetFocusCell( );

   zBOOL bCanPaste = IsValid( cellId ) && IsCellEditable( cellId ) &&
                    ::IsClipboardFormatAvailable( CF_TEXT );

   pCmdUI->Enable( bCanPaste );
}
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP

// Start drag n drop
void
ZGrid::OnBeginDrag( )
{
   if ( m_bAllowDragAndDrop == FALSE )
      return;

   COleDataSource *pSource = CopyTextFromGrid( );
   if ( pSource )
   {
      SendMessageToParent( GetSelectedCellRange( ).GetTopLeft( ).m_lRow,
                           GetSelectedCellRange( ).GetTopLeft( ).m_nCol,
                           GVN_BEGINDRAG );

      m_MouseMode = MOUSE_DRAGGING;
      m_bLMouseButtonDown = FALSE;

      DROPEFFECT dropEffect =
                   pSource->DoDragDrop( DROPEFFECT_COPY | DROPEFFECT_MOVE );

      if ( dropEffect & DROPEFFECT_MOVE )
         CutSelectedText( );

      if ( pSource )
         delete( pSource ); // did not pass source to clipboard, so must delete
   }
}

// Handle drag over grid.
DROPEFFECT
ZGrid::OnDragOver( COleDataObject *pDataObject,
                   DWORD dwKeyState, CPoint pt )
{
   // Any text data available for us?
   if ( m_bAllowDragAndDrop == FALSE || IsEditable( ) == FALSE ||
        pDataObject->IsDataAvailable( CF_TEXT ) == FALSE )
   {
      return( DROPEFFECT_NONE );
   }

   // Find which cell we are over and drop-highlight it.
   ZCellId cellId = GetCellFromPt( pt, FALSE );

   // If not valid, set the previously drop-highlighted cell as
   // no longer drop-highlighted.
   if ( IsValid( cellId ) == FALSE )
   {
      OnDragLeave( );
      m_cellLastDragOver = ZCellId( -1, -1 );
      return( DROPEFFECT_NONE );
   }

   if ( IsCellEditable( cellId ) == FALSE )
      return( DROPEFFECT_NONE );

   // Have we moved over a different cell than last time?
   if ( cellId != m_cellLastDragOver )
   {
      // Set the previously drop-highlighted cell as no longer drop-highlighted
      if ( IsValid( m_cellLastDragOver ) )
      {
         UINT uState = GetItemState( m_cellLastDragOver.m_lRow,
                                     m_cellLastDragOver.m_nCol );
         SetItemState( m_cellLastDragOver.m_lRow, m_cellLastDragOver.m_nCol,
                       uState & ~GVIS_DROPHILITED );
         RedrawCell( m_cellLastDragOver );
      }

      m_cellLastDragOver = cellId;

      // Set the new cell as drop-highlighted.
      if ( IsValid( m_cellLastDragOver ) )
      {
         UINT uState = GetItemState( m_cellLastDragOver.m_lRow,
                                     m_cellLastDragOver.m_nCol );
         SetItemState( m_cellLastDragOver.m_lRow, m_cellLastDragOver.m_nCol,
                       uState | GVIS_DROPHILITED );
         RedrawCell( m_cellLastDragOver );
      }
   }

   // Return an appropriate value of DROPEFFECT so mouse cursor is set properly.
   if ( dwKeyState & MK_CONTROL )
      return( DROPEFFECT_COPY );
   else
      return( DROPEFFECT_MOVE );
}

// Something has just been dragged onto the grid.
DROPEFFECT
ZGrid::OnDragEnter( COleDataObject *pDataObject, DWORD dwKeyState,
                    CPoint pt )
{
   // Any text data available for us?
   if ( m_bAllowDragAndDrop == FALSE ||
        pDataObject->IsDataAvailable( CF_TEXT ) == FALSE )
   {
      return( DROPEFFECT_NONE );
   }

   // Find which cell we are over and drop-highlight it.
   m_cellLastDragOver = GetCellFromPt( pt, FALSE );
   if ( IsValid( m_cellLastDragOver ) == FALSE )
      return( DROPEFFECT_NONE );

   if ( IsCellEditable( m_cellLastDragOver ) == FALSE )
      return( DROPEFFECT_NONE );

   if ( IsValid( m_cellLastDragOver ) )
   {
      UINT uState = GetItemState( m_cellLastDragOver.m_lRow,
                                  m_cellLastDragOver.m_nCol );
      SetItemState( m_cellLastDragOver.m_lRow, m_cellLastDragOver.m_nCol,
                    uState | GVIS_DROPHILITED );
      RedrawCell( m_cellLastDragOver );
   }

   // Return an appropriate value of DROPEFFECT so mouse cursor is set properly.
   if ( dwKeyState & MK_CONTROL )
      return( DROPEFFECT_COPY );
   else
      return( DROPEFFECT_MOVE );
}

// Something has just been dragged away from the grid.
void
ZGrid::OnDragLeave( )
{
   // Set the previously drop-highlighted cell as no longer drop-highlighted.
   if ( IsValid( m_cellLastDragOver ) )
   {
      UINT uState = GetItemState( m_cellLastDragOver.m_lRow,
                                  m_cellLastDragOver.m_nCol );
      SetItemState( m_cellLastDragOver.m_lRow, m_cellLastDragOver.m_nCol,
                    uState & ~GVIS_DROPHILITED );
      RedrawCell( m_cellLastDragOver );
   }
}

// Something has just been dropped onto the grid.
BOOL
ZGrid::OnDrop( COleDataObject *pDataObject, DROPEFFECT dropEffect,
               CPoint pt )
{
   m_MouseMode = MOUSE_NOTHING;

   if ( m_bAllowDragAndDrop == FALSE ||
        IsCellEditable( m_cellLastDragOver ) == FALSE )
   {
      return( FALSE );
   }

   OnDragLeave( );

   return( PasteTextToGrid( m_cellLastDragOver, pDataObject, FALSE ) );
}
#endif

void
ZGrid::OnEditSelectAll( )
{
   ZCellId cellHold = m_cellIdCurrent;
   if ( SendMessageToParent( m_cellLeftClickDown.m_lRow,
                             m_cellLeftClickDown.m_nCol,
                             GVN_SELCHANGING ) )
   {
      SetFocusCell( cellHold );  // don't let cell change!
   }
   else
   {
      SelectAllCells( );
      SendMessageToParent( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol,
                           GVN_SELCHANGED );
   }
}

void
ZGrid::OnUpdateEditSelectAll( CCmdUI *pCmdUI )
{
   pCmdUI->Enable( m_bEnableSelection );
}

/////////////////////////////////////////////////////////////////////////////
// hit test-like functions

// TRUE if the mouse is over a row resize area
zBOOL
ZGrid::MouseOverRowResizeArea( CPoint pt )
{
   if ( pt.x >= GetFixedColumnWidth( ) )
      return( FALSE );

   ZCellId cellId = GetCellFromPt( pt );
   CPoint  ptStart;
   if ( GetCellOrigin( cellId, ptStart ) == 0 )
      return( FALSE );

   int nEndY = ptStart.y +
                 (GetRowHeight( cellId.m_lRow ) * (m_nExtraRowsPerItem + 1));

   if ( (pt.y - ptStart.y < m_nResizeCaptureRange && cellId.m_lRow) ||
        nEndY - pt.y < m_nResizeCaptureRange )
   {
      return( TRUE );
   }
   else
      return( FALSE );
}

// TRUE if the mouse is over a column resize area. point is in Client coords
zBOOL
ZGrid::MouseOverColumnResizeArea( CPoint pt )
{
   if ( pt.y >= GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1) )
      return( FALSE );

   ZCellId cellId = GetCellFromPt( pt );
   CPoint  ptStart;
   if ( GetCellOrigin( cellId, ptStart ) == 0 )
      return( FALSE );

   int nEndX = ptStart.x + GetColumnWidth( cellId.m_nCol );

   if ( (pt.x - ptStart.x < m_nResizeCaptureRange &&
         ptStart.x - pt.x < m_nResizeCaptureRange &&
         cellId.m_nCol && cellId.m_nCol != m_nExtraCol) ||
        nEndX - pt.x < m_nResizeCaptureRange )
   {
      return( TRUE );
   }
   else
      return( FALSE );
}

// Get cell from point.
// point - client coordinates
// bAllowFixedCellCheck - if TRUE then fixed cells are checked
ZCellId
ZGrid::GetCellFromPt( CPoint pt, zBOOL bAllowFixedCellCheck /*=TRUE*/ )
{
   ZCellId cellId; // return value

   ZCellId idTopLeft = GetTopLeftNonFixedCell( );
   if ( bAllowFixedCellCheck == FALSE && IsValid( idTopLeft ) == FALSE )
      return( cellId );

   int nExtraRowsPerItem = m_nExtraRowsPerItem + 1;

   // Calculate row index (need this before column index).
   int  nFixedRowHeight = GetFixedRowHeight( );
   int  nPosY = 0;
   int  nRowHeight = 0;

   if ( pt.y < 0 ||
        (bAllowFixedCellCheck == FALSE &&
         pt.y < nFixedRowHeight * nExtraRowsPerItem) ) // not in window
   {
      cellId.m_lRow = -1;
   }
   else
   if ( pt.y < nFixedRowHeight * nExtraRowsPerItem ) // in fixed row
   {
      long lRow = 0;
      while ( lRow < m_nFixedRows )
      {
         nRowHeight = GetRowHeight( lRow ) * nExtraRowsPerItem;
         if ( nPosY + nRowHeight > pt.y )
            break;

         nPosY += nRowHeight;
         lRow++;
      }

      cellId.m_lRow = lRow;
   }
   else
   {
      long lRow = idTopLeft.m_lRow; // m_nFixedRows;
      long lRowCnt = GetRowCount( );

      nPosY = nFixedRowHeight * nExtraRowsPerItem;
      while ( lRow < lRowCnt )
      {
         nRowHeight = GetRowHeight( lRow ) * nExtraRowsPerItem;
         if ( nPosY + nRowHeight > pt.y )
            break;

         nPosY += nRowHeight;
         lRow++;
      }

      if ( lRow >= lRowCnt )
         cellId.m_lRow = -1;
      else
         cellId.m_lRow = lRow;
   }

   // Calculate column index.
   int nFixedColWidth = GetFixedColumnWidth( );

   if ( pt.x < 0 || cellId.m_lRow == -1 ||
        (bAllowFixedCellCheck == FALSE && pt.x < nFixedColWidth) ) // not in window
   {
      cellId.m_nCol = -1;
   }
   else
   if ( pt.x < nFixedColWidth ) // in fixed col
   {
      int nPosX = 0;
      int nCol = 0;
      while ( nCol < m_nFixedCols )
      {
         nPosX += GetColumnWidth( nCol );
         if ( nPosX > pt.x )
            break;

         nCol++;
      }

      cellId.m_nCol = nCol;
   }
   else   // in non-fixed col
   {
      int nColCnt = GetColumnCount( );
      int nPosX = nFixedColWidth;
      int nCol = idTopLeft.m_nCol; // m_nFixedCols;

      nPosY += GetRowHeight( cellId.m_lRow );
      while ( nCol < nColCnt )
      {
         if ( m_nExtraCol && nCol == m_nExtraCol )
         {
            if ( nPosY > pt.y )
            {
               nCol = nColCnt;  // past last column in first
               break;           // row of multi-row line
            }

            nPosX = m_nExtraOffset + nFixedColWidth;
            nPosY += nFixedRowHeight;
            if ( nPosX > pt.x && nPosY > pt.y )
            {
               nCol = nColCnt;    // in no man's land in front
               break;             // of first "extra col"
            }
         }

         nPosX += GetColumnWidth( nCol );
         if ( nPosX >= pt.x && nPosY > pt.y )
         {
            // Get the rightmost column that fits this position.
            while ( nCol < nColCnt - 1 )
            {
               if ( m_nExtraCol && (nCol + 1) == m_nExtraCol )
                  break;

               if ( (nPosX + GetColumnWidth( nCol + 1 )) - pt.x > m_nResizeCaptureRange )
                  break;

               nCol++;
            }

            break;
         }

         nCol++;
      }

      if ( nCol >= nColCnt )
         cellId.m_nCol = -1;
      else
         cellId.m_nCol = nCol;
   }

// TraceLine( "CellIdFromPoint Row: %d   Col: %d", cellId.m_lRow, cellId.m_nCol );
   return( cellId );
}

////////////////////////////////////////////////////////////////////////////////
// ZGrid cellrange functions

// Gets the first non-fixed cellId
ZCellId
ZGrid::GetTopLeftNonFixedCell( zBOOL bForceRecalculation /*=FALSE*/ )
{
   // Used cached value if possible
   if ( m_cellIdTopLeft.IsValid( ) && bForceRecalculation == FALSE )
      return( m_cellIdTopLeft );

   int  nVertScroll = GetScrollPos( SB_VERT );
   int  nHorzScroll = GetScrollPos( SB_HORZ );
   int  nColCnt = GetColumnCount( ) - 1;
   long lRowCnt = GetRowCount( ) - 1;

   m_cellIdTopLeft.m_nCol = m_nFixedCols;
   int nRight = 0;
   while ( nRight < nHorzScroll && m_cellIdTopLeft.m_nCol < nColCnt )
      nRight += GetColumnWidth( m_cellIdTopLeft.m_nCol++ );

   m_cellIdTopLeft.m_lRow = m_nFixedRows;
   int nTop = 0;
   while ( nTop < nVertScroll && m_cellIdTopLeft.m_lRow < lRowCnt )
      nTop += GetRowHeight( m_cellIdTopLeft.m_lRow++ ) * (m_nExtraRowsPerItem + 1);

   // TRACE2( "TopLeft cell is row %d, col %d\n",
   //         m_cellIdTopLeft.m_lRow, m_cellIdTopLeft.m_nCol );
   return( m_cellIdTopLeft );
}

// This gets even partially visible cells
ZCellRange
ZGrid::GetVisibleNonFixedCellRange( LPRECT pRect /*=0*/,
                                    zBOOL  bForceRecalculation /*=FALSE*/ )
{
   CRect rect;
   GetClientRect( rect );

   ZCellId idTopLeft = GetTopLeftNonFixedCell( bForceRecalculation );

   // calc bottom
   int  nBottom = GetFixedRowHeight( );
   long lRowCnt = GetRowCount( );
   int  k;
   for ( k = idTopLeft.m_lRow; k < lRowCnt; k++ )
   {
      nBottom += GetRowHeight( k ) * (m_nExtraRowsPerItem + 1);
      if ( nBottom >= rect.bottom )
      {
         nBottom = rect.bottom;
         break;
      }
   }

   int maxVisibleRow = min( k, lRowCnt - 1 );
   int nColCnt = GetColumnCount( );

   // calc right
   int nRight = GetFixedColumnWidth( );
   for ( k = idTopLeft.m_nCol; k < nColCnt; k++ )
   {
      nRight += GetColumnWidth( k );
      if ( nRight >= rect.right )
      {
         nRight = rect.right;
         if ( m_nExtraCol )  // all columns are always visible
            k = nColCnt;     // when m_nExtraCol is set

         break;
      }
   }

   int maxVisibleCol = min( k, nColCnt - 1 );
   if ( pRect )
   {
      pRect->left = pRect->top = 0;
      pRect->right = nRight;
      pRect->bottom = nBottom;
   }

   return( ZCellRange( idTopLeft.m_lRow, idTopLeft.m_nCol, maxVisibleRow, maxVisibleCol ) );
}

// used by ResetScrollBars( ) - This gets only fully visible cells
ZCellRange
ZGrid::GetUnobstructedNonFixedCellRange( zBOOL bForceRecalculation /*=FALSE*/ )
{
   CRect rect;
   GetClientRect( rect );

   ZCellId idTopLeft = GetTopLeftNonFixedCell( bForceRecalculation );

   // calc bottom
   int bottom = GetFixedRowHeight( );
   int k;

   for ( k = idTopLeft.m_lRow; k < GetRowCount( ); k++ )
   {
      bottom += GetRowHeight( k ) *(m_nExtraRowsPerItem + 1);
      if ( bottom >= rect.bottom )
         break;
   }

   int maxVisibleRow = min( k, GetRowCount( ) - 1 );
   if ( maxVisibleRow > 0 && bottom > rect.bottom )
      maxVisibleRow--;

   // calc right
   int right = GetFixedColumnWidth( );
   int nColCnt = GetColumnCount( );

   for ( k = idTopLeft.m_nCol; k < nColCnt; k++ )
   {
      right += GetColumnWidth( k );
      if ( right >= rect.right )
      {
         if ( m_nExtraCol )  // all columns are always visible
            k = nColCnt;     // when m_nExtraCol is set

         break;
      }
   }

   int maxVisibleCol = min( k, nColCnt - 1 );
   if ( maxVisibleCol > 0 && right > rect.right && m_nExtraCol == 0 )
      maxVisibleCol--;

   return( ZCellRange( idTopLeft.m_lRow, idTopLeft.m_nCol,
                       maxVisibleRow, maxVisibleCol ) );
}

// Returns the minimum bounding range of the current selection
// If no selection, then the returned ZCellRange will be invalid
ZCellRange
ZGrid::GetSelectedCellRange( ) const
{
   ZCellRange Selection( GetRowCount( ), GetColumnCount( ), -1, -1 );

   for ( POSITION pos = m_SelectedCellMap.GetStartPosition( ); pos; )
   {
      LARGE_INTEGER key;
      ZCellId cellId;
      m_SelectedCellMap.GetNextAssoc( pos, key, (ZCellId &) cellId );

      Selection.SetMinRow( min( Selection.GetMinRow( ), cellId.m_lRow ) );
      Selection.SetMinCol( min( Selection.GetMinCol( ), cellId.m_nCol ) );
      Selection.SetMaxRow( max( Selection.GetMaxRow( ), cellId.m_lRow ) );
      Selection.SetMaxCol( max( Selection.GetMaxCol( ), cellId.m_nCol ) );
   }

   return( Selection );
}

// Returns ALL the cells in the grid
ZCellRange
ZGrid::GetCellRange( ) const
{
   return( ZCellRange( 0, 0, GetRowCount( ) - 1, GetColumnCount( ) - 1 ) );
}

// Resets the selected cell range to the empty set.
void
ZGrid::ResetSelectedRange( )
{
   m_PrevSelectedCellMap.RemoveAll( );
   SetSelectedRange( -1, -1, -1, -1 );
   SetFocusCell( -1, -1 );
}

// Get/Set scroll position using 32 bit functions
int
ZGrid::GetScrollPos32( int nBar, BOOL bGetTrackPos /* = FALSE */ )
{
   SCROLLINFO si;
   si.cbSize = sizeof( SCROLLINFO );

   if ( bGetTrackPos )
   {
      if ( GetScrollInfo( nBar, &si, SIF_TRACKPOS ) )
         return( si.nTrackPos );
   }
   else
   {
      if ( GetScrollInfo( nBar, &si, SIF_POS ) )
         return( si.nPos );
   }

   return( 0 );
}

BOOL
ZGrid::SetScrollPos32( int nBar, int nPos, BOOL bRedraw /* = TRUE */ )
{
   m_cellIdTopLeft.m_lRow = -1;

   SCROLLINFO si;
   si.cbSize = sizeof( SCROLLINFO );
   si.fMask  = SIF_POS;
   si.nPos   = nPos;
   return( SetScrollInfo( nBar, &si, bRedraw ) );
}

void
ZGrid::EnableScrollBars( int nBar, zBOOL bEnable /*=TRUE*/ )
{
   if ( bEnable )
   {
      if ( IsVisibleHScroll( ) == FALSE && (nBar == SB_HORZ || nBar == SB_BOTH) )
      {
         m_nBarState |= GVL_HORZ;
         CWnd::EnableScrollBarCtrl( SB_HORZ, bEnable );
      }

      if ( IsVisibleVScroll( ) == FALSE && (nBar == SB_VERT || nBar == SB_BOTH) )
      {
         m_nBarState |= GVL_VERT;
         CWnd::EnableScrollBarCtrl( SB_VERT, bEnable );
      }
   }
   else
   {
      if ( IsVisibleHScroll( ) && (nBar == SB_HORZ || nBar == SB_BOTH) )
      {
         m_nBarState &= ~GVL_HORZ;
         CWnd::EnableScrollBarCtrl( SB_HORZ, bEnable );
      }

      if ( IsVisibleVScroll( ) && (nBar == SB_VERT || nBar == SB_BOTH) )
      {
         m_nBarState &= ~GVL_VERT;
         CWnd::EnableScrollBarCtrl( SB_VERT, bEnable );
      }
   }
}

// If resizing or cell counts/sizes change, call this - it'll fix up the scroll bars
void
ZGrid::ResetScrollBars( )
{
   // Force a refresh.
   m_cellIdTopLeft.m_lRow = -1;

   if ( m_bAllowDraw == FALSE || mIs_hWnd( GetSafeHwnd( ) ) == FALSE )
      return;

   CRect rect;
   int   nFixedRowHeight = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);

   // This would have caused OnSize event - Brian
   //EnableScrollBars( SB_BOTH, FALSE );

   GetClientRect( rect );

   if ( rect.left == rect.right || rect.top == rect.bottom )
      return;

   if ( IsVisibleVScroll( ) )
      rect.right += GetSystemMetrics( SM_CXVSCROLL ) + GetSystemMetrics( SM_CXBORDER );

   if ( IsVisibleHScroll( ) )
      rect.bottom += GetSystemMetrics( SM_CYHSCROLL ) + GetSystemMetrics( SM_CYBORDER );

   rect.left += GetFixedColumnWidth( );
   rect.top += nFixedRowHeight;

   if ( rect.left >= rect.right || rect.top >= rect.bottom )
   {
      EnableScrollBarCtrl( SB_BOTH, FALSE );
      return;
   }

   CRect VisibleRect( GetFixedColumnWidth( ), nFixedRowHeight,
                      rect.right, rect.bottom );
   CRect VirtualRect( GetFixedColumnWidth( ), nFixedRowHeight,
                      GetVirtualWidth( ), GetVirtualHeight( ) * (m_nExtraRowsPerItem + 1) );

   // Removed to fix single row scrollbar problem ( Pontus Goffe )
   // ZCellRange visibleCells = GetUnobstructedNonFixedCellRange( );
   // if ( IsValid( visibleCells ) == FALSE )
   //    return;

   //TRACE( _T( "Visible: %d x %d, Virtual %d x %d.  H %d, V %d\n" ),
   //     VisibleRect.Width( ), VisibleRect.Height( ),
   //     VirtualRect.Width( ), VirtualRect.Height( ),
   //     IsVisibleHScroll( ), IsVisibleVScroll( ) );

   // If vertical scroll bar, horizontal space is reduced
   if ( VisibleRect.Height( ) < VirtualRect.Height( ) )
      VisibleRect.right -= ::GetSystemMetrics( SM_CXVSCROLL );

   // If horz scroll bar, vert space is reduced
   if ( VisibleRect.Width( ) < VirtualRect.Width( ) )
      VisibleRect.bottom -= ::GetSystemMetrics( SM_CYHSCROLL );

   // Recheck vertical scroll bar
   //if ( VisibleRect.Height( ) < VirtualRect.Height( ) )
   // VisibleRect.right -= ::GetSystemMetrics( SM_CXVSCROLL );

   if ( VisibleRect.Height( ) < VirtualRect.Height( ) )
   {
      EnableScrollBars( SB_VERT, TRUE );
      m_nVScrollMax = VirtualRect.Height( ) - 1;
   }
   else
   {
      EnableScrollBars( SB_VERT, FALSE );
      m_nVScrollMax = 0;
   }

   if ( m_nExtraRowsPerItem == 0 &&
        VisibleRect.Width( ) < VirtualRect.Width( ) )
   {
      EnableScrollBars( SB_HORZ, TRUE );
      m_nHScrollMax = VirtualRect.Width( ) - 1;
   }
   else
   {
      EnableScrollBars( SB_HORZ, FALSE );
      m_nHScrollMax = 0;
   }

   ASSERT( m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX ); // This should be fine

   /* Old code - CJM
   SCROLLINFO si;
   si.cbSize = sizeof( SCROLLINFO );
   si.fMask = SIF_PAGE;
   si.nPage = (m_nHScrollMax > 0) ? VisibleRect.Width( ) : 0;
   SetScrollInfo( SB_HORZ, &si, FALSE );
   si.nPage = (m_nVScrollMax > 0) ? VisibleRect.Height( ) : 0;
   SetScrollInfo( SB_VERT, &si, FALSE );

   SetScrollRange( SB_VERT, 0, m_nVScrollMax, TRUE );
   SetScrollRange( SB_HORZ, 0, m_nHScrollMax, TRUE );
   */

   // New code - Paul Runstedler
   SCROLLINFO si;
   si.cbSize = sizeof( SCROLLINFO );
   si.fMask = SIF_PAGE | SIF_RANGE;
   si.nPage = (m_nHScrollMax > 0) ? VisibleRect.Width( ) : 0;
   si.nMin = 0;
   si.nMax = m_nHScrollMax;
   SetScrollInfo( SB_HORZ, &si, TRUE );

   si.fMask |= SIF_DISABLENOSCROLL;
   si.nPage = (m_nVScrollMax > 0) ? VisibleRect.Height( ) : 0;
   si.nMin = 0;
   si.nMax = m_nVScrollMax;
   SetScrollInfo( SB_VERT, &si, TRUE );
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

// Returns the top left point of the cell. Returns FALSE if cell not visible.
zBOOL
ZGrid::GetCellOrigin( long lRow, int nCol, CPoint& pt )
{
   int k;

   if ( IsValid( lRow, nCol ) == FALSE )
      return( FALSE );

   ZCellId idTopLeft;
   if ( nCol >= m_nFixedCols || lRow >= m_nFixedRows )
      idTopLeft = GetTopLeftNonFixedCell( );

   if ( (lRow >= m_nFixedRows && lRow < idTopLeft.m_lRow) ||
        (nCol >= m_nFixedCols && nCol < idTopLeft.m_nCol) )
   {
      return( FALSE );
   }

   int nExtraRowsPerItem = m_nExtraRowsPerItem + 1;
   pt.y = 0;
   if ( lRow < m_nFixedRows )                 // is a fixed row
   {
      for ( k = 0; k < lRow; k++ )
         pt.y += GetRowHeight( k ) * nExtraRowsPerItem;
   }
   else
   {                                          // is a scrollable data row
      for ( k = 0; k < m_nFixedRows; k++ )
         pt.y += GetRowHeight( k ) * nExtraRowsPerItem;

      for ( k = idTopLeft.m_lRow; k < lRow; k++ )
         pt.y += GetRowHeight( k ) * nExtraRowsPerItem;
   }

   pt.x = 0;
   if ( nCol < m_nFixedCols )                 // is a fixed column
   {
      for ( k = 0; k < nCol; k++ )
         pt.x += GetColumnWidth( k );
   }
   else
   {                                          // is a scrollable data column
      for ( k = 0; k < m_nFixedCols; k++ )
         pt.x += GetColumnWidth( k );

      if ( m_nExtraCol && nCol >= m_nExtraCol )
      {
         k = m_nExtraCol;
         pt.x += m_nExtraOffset;
         pt.y += GetRowHeight( lRow );
      }
      else
         k = idTopLeft.m_nCol;

      while ( k < nCol )
         pt.x += GetColumnWidth( k++ );
   }

   return( TRUE );
}

zBOOL
ZGrid::GetCellOrigin( const ZCellId& cellId, CPoint& pt )
{
   return( GetCellOrigin( cellId.m_lRow, cellId.m_nCol, pt ) );
}

// Returns the bounding box of the cell
zBOOL
ZGrid::GetCellRect( const ZCellId& cellId, CRect& rect )
{
   return( GetCellRect( cellId.m_lRow, cellId.m_nCol, rect ) );
}

zBOOL
ZGrid::GetCellRect( long lRow, int nCol, CRect& rect )
{
   CPoint ptCellOrigin;
   if ( GetCellOrigin( lRow, nCol, ptCellOrigin ) == 0 )
      return( FALSE );

   int nRowHeight = GetRowHeight( lRow );
   rect.left = ptCellOrigin.x;
   rect.top = ptCellOrigin.y;
   rect.right = ptCellOrigin.x + GetColumnWidth( nCol ) - 1;
// if ( nCol == 0 && m_nExtraCol )
//    rect.bottom = ptCellOrigin.y + (nRowHeight * (m_nExtraRowsPerItem + 1)) - 1;
// else
      rect.bottom = ptCellOrigin.y + nRowHeight - 1;

// TraceLine( "Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d",
//            lRow, nCol, ptCellOrigin.x, ptCellOrigin.y,
//            GetColumnWidth( nCol ), GetRowHeight( lRow ),
//            pRect->left, pRect->top, pRect->right, pRect->bottom );

   return( TRUE );
}

zBOOL
ZGrid::GetTextRect( const ZCellId& cellId, CRect& rect )
{
   return( GetTextRect( cellId.m_lRow, cellId.m_nCol, rect ) );
}

zBOOL
ZGrid::GetTextRect( long lRow, int nCol, CRect& rect )
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( FALSE );

   if ( GetCellRect( lRow, nCol, rect ) == FALSE )
      return( FALSE );

   return( pCell->GetTextRect( rect ) );
}

// Returns the bounding box of a range of cells.
zBOOL
ZGrid::GetCellRangeRect( const ZCellRange& cellRange, CRect& rect )
{
   CPoint ptMinOrigin, ptMaxOrigin;

   if ( GetCellOrigin( cellRange.GetMinRow( ), cellRange.GetMinCol( ),
                       ptMinOrigin ) == 0 )
   {
      return( FALSE );
   }

   if ( GetCellOrigin( cellRange.GetMaxRow( ), cellRange.GetMaxCol( ),
                       ptMaxOrigin ) == 0 )
   {
      return( FALSE );
   }

   rect.left   = ptMinOrigin.x;
   rect.top    = ptMinOrigin.y;
   rect.right  = ptMaxOrigin.x + GetColumnWidth( cellRange.GetMaxCol( ) ) - 1;
   rect.bottom = ptMaxOrigin.y + (GetRowHeight( cellRange.GetMaxRow( ) ) *
                                              (m_nExtraRowsPerItem + 1)) - 1;
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT
ZGrid::OnSetFont( WPARAM hFont, LPARAM lParam )
{
   LRESULT result = Default( );

   // Get the logical font.
   LOGFONT lf;
   if ( GetObject( (HFONT) hFont, sizeof( LOGFONT ), &lf ) == 0 )
      return( result );

   m_cellDefault.SetFont( &lf );
   m_cellFixedColDef.SetFont( &lf );
   m_cellFixedRowDef.SetFont( &lf );
   m_cellFixedRowColDef.SetFont( &lf );

   Refresh( );

   return( result );
}

LRESULT
ZGrid::OnGetFont( WPARAM /*wParam*/, LPARAM /*lParam*/ )
{
   //LOGFONT   lf;
   //m_cellDefault.GetFontObject( )->GetLogFont( &lf );

   return( (LRESULT) m_cellDefault.GetFontObject( )->GetSafeHandle( ) );
}

#ifndef _WIN32_WCE_NO_CURSOR
BOOL
ZGrid::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT message )
{
   if ( uHitTest == HTCLIENT )
   {
      switch ( m_MouseMode )
      {
      case MOUSE_OVER_COL_DIVIDE:
         SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZEWE ) );
         break;

      case MOUSE_OVER_ROW_DIVIDE:
         SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZENS ) );
         break;

#ifndef GRIDCONTROL_NO_DRAGDROP
      case MOUSE_DRAGGING:
         break;
#endif
      default:
         if ( GetVirtualMode( ) == FALSE )
         {
            CPoint pt( GetMessagePos( ) );
            ScreenToClient( &pt );
            ZCellId cellId = GetCellFromPt( pt );
            if ( IsValid( cellId ) )
            {
               ZGridCellBase *pCell = GetCell( cellId.m_lRow, cellId.m_nCol );
               if ( pCell )
                  return( pCell->OnSetCursor( ) );
            }
         }

         SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_ARROW ) );
      }

      return( TRUE );
   }

   return( CWnd::OnSetCursor( pWnd, uHitTest, message ) );
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

zBOOL
ZGrid::SetFixedRowCount( int nFixedRows )
{
   if ( m_nFixedRows == nFixedRows )
      return( TRUE );

   ASSERT( nFixedRows >= 0 );

   ResetSelectedRange( );

   // Force recalculation
   m_cellIdTopLeft.m_nCol = -1;

   if ( nFixedRows > GetRowCount( ) )
   {
      if ( SetRowCount( nFixedRows ) == 0 )
         return( FALSE );
   }

   if ( m_cellIdCurrent.m_lRow < nFixedRows )
      SetFocusCell( -1, -1 );

   if ( GetVirtualMode( ) == FALSE )
   {
      int nColCnt = GetColumnCount( );
      if ( nFixedRows > m_nFixedRows )
      {
         for ( int k = m_nFixedRows; k < nFixedRows; k++ )
         {
            for ( int j = 0; j < nColCnt; j++ )
            {
               SetItemState( k, j,
                             GetItemState( k, j ) | GVIS_FIXED | GVIS_FIXEDROW );
               SetItemBkColor( k, j, CLR_DEFAULT );
               SetItemFgColor( k, j, CLR_DEFAULT );
            }
         }
      }
      else
      {
         for ( int k = nFixedRows; k < m_nFixedRows; k++ )
         {
            int nFixedColCnt = GetFixedColumnCount( );
            int j;

            for ( j = 0; j < nFixedColCnt; j++ )
               SetItemState( k, j, GetItemState( k, j ) & ~GVIS_FIXEDROW );

            for ( j = nFixedColCnt; j < nColCnt; j++ )
            {
               SetItemState( k, j,
                             GetItemState( k, j ) & ~(GVIS_FIXED | GVIS_FIXEDROW) );
               SetItemBkColor( k, j, CLR_DEFAULT );
               SetItemFgColor( k, j, CLR_DEFAULT );
            }
         }
      }
   }

   m_nFixedRows = nFixedRows;
   Refresh( );

   return( TRUE );
}

zBOOL
ZGrid::SetFixedColumnCount( int nFixedCols )
{
   if ( m_nFixedCols == nFixedCols )
      return( TRUE );

   ASSERT( nFixedCols >= 0 );

   if ( nFixedCols > GetColumnCount( ) )
   {
      if ( SetColumnCount( nFixedCols ) == 0 )
         return( FALSE );
   }

   if ( m_cellIdCurrent.m_nCol < nFixedCols )
      SetFocusCell( -1, -1 );

   ResetSelectedRange( );

   // force recalculation.
   m_cellIdTopLeft.m_nCol = -1;

   if ( GetVirtualMode( ) == FALSE )
   {
      if ( nFixedCols > m_nFixedCols )
      {
         long lRowCnt = GetRowCount( );
         for ( int k = 0; k < lRowCnt; k++ )
         {
            for ( int j = m_nFixedCols; j < nFixedCols; j++ )
            {
               SetItemState( k, j,
                             GetItemState( k, j ) | GVIS_FIXED | GVIS_FIXEDCOL );
               SetItemBkColor( k, j, CLR_DEFAULT );
               SetItemFgColor( k, j, CLR_DEFAULT );
            }
         }
      }
      else
      {
         int nFixedRowCnt = GetFixedRowCount( );

         {  // Scope limit i,j
            for ( int k = 0; k < nFixedRowCnt; k++ )
            {
               for ( int j = nFixedCols; j < m_nFixedCols; j++ )
                  SetItemState( k, j, GetItemState( k, j ) & ~GVIS_FIXEDCOL );
            }
         }

         {  // Scope limit i,j
            long lRowCnt = GetRowCount( );

            for ( int k = nFixedRowCnt; k < lRowCnt; k++ )
            {
               for ( int j = nFixedCols; j < m_nFixedCols; j++ )
               {
                  SetItemState( k, j,
                                GetItemState( k, j ) & ~(GVIS_FIXED | GVIS_FIXEDCOL) );
                  SetItemBkColor( k, j, CLR_DEFAULT );
                  SetItemFgColor( k, j, CLR_DEFAULT );
               }
            }
         }
      }
   }

   m_nFixedCols = nFixedCols;
   Refresh( );

   return( TRUE );
}

zBOOL
ZGrid::SetRowCount( long lRows )
{
   ASSERT( lRows >= 0 );
   long lAddedRows = lRows - GetRowCount( );
   if ( lAddedRows == 0 )
      return( TRUE );

   zBOOL bResult = TRUE;
   long lRow;
   int  nCol;

   // Force recalculation.
   m_cellIdTopLeft.m_nCol = -1;

   if ( lRows < m_nFixedRows )
      m_nFixedRows = lRows;

   if ( m_cellIdCurrent.m_lRow >= lRows )
      SetFocusCell( -1, -1 );

   // If we are about to lose rows, then we need to delete the GridCell
   // objects in each column within each row.
   if ( lAddedRows < 0 )
   {
      if ( GetVirtualMode( ) == FALSE )
      {
         for ( lRow = lRows; lRow < m_lRows; lRow++ )
         {
            // Delete cells.
            for ( nCol = 0; nCol < m_nCols; nCol++ )
               DestroyCell( lRow, nCol );

            // Delete rows.
            GRID_ROW *pRow = m_RowData[ lRow ];
            if ( pRow )
               delete( pRow );
         }
      }

      m_lRows = lRows;
   }

   TRY
   {
      int nHeight = m_cellDefault.GetHeight( );
      m_arRowHeights.SetSize( lRows );

      if ( GetVirtualMode( ) )
      {
         m_lRows = lRows;
         if ( lAddedRows > 0 )
         {
            long lStartRow = lRows - lAddedRows;
            for ( lRow = lStartRow; lRow < lRows; lRow++ )
               m_arRowHeights[ lRow ] = nHeight;
         }
      }
      else
      {
         // Change the number of rows.
         m_RowData.SetSize( lRows );

         // If we have just added rows, we need to construct new elements
         // for each cell and set the default row height.
         if ( lAddedRows > 0 )
         {
            // Initialize row heights and data.
            ZGridCellBase *pCell;
            long lStartRow = lRows - lAddedRows;
            for ( lRow = lStartRow; lRow < lRows; lRow++ )
            {
               m_arRowHeights[ lRow ] = nHeight;

               m_RowData[ lRow ] = new GRID_ROW;
               m_RowData[ lRow ]->SetSize( m_nCols );
               for ( nCol = 0; nCol < m_nCols; nCol++ )
               {
                  GRID_ROW *pRow = m_RowData[ lRow ];
                  if ( pRow )
                  {
                  // DestroyCell( lRow, nCol );
                     pCell = GetCell( lRow, nCol );
                     if ( pCell )
                        SetItemState( lRow, nCol, 0 );
                     else
                        pRow->SetAt( nCol, CreateCell( lRow, nCol ) );
                  }
               }

               m_lRows++;
            }
         }
      }
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      bResult = FALSE;
   }
   END_CATCH

// SetModified( );
   ResetScrollBars( );
   Refresh( );

   return( bResult );
}

zBOOL
ZGrid::SetColumnCount( int nCols )
{
   ASSERT( nCols >= 0 );
   int nColCnt = GetColumnCount( );
   int nAddedCols = nCols - nColCnt;
   if ( nAddedCols == 0 )
      return( TRUE );

   zBOOL bResult = TRUE;
   long lRow;
   int  nCol;

   // Force recalculation.
   m_cellIdTopLeft.m_nCol = -1;

   if ( nCols < m_nFixedCols )
      m_nFixedCols = nCols;

   if ( m_cellIdCurrent.m_nCol >= nCols )
      SetFocusCell( -1, -1 );

   // If we are about to lose columns, then we need to delete the GridCell
   // objects within each column.
   if ( nAddedCols < 0 && GetVirtualMode( ) == FALSE )
   {
      for ( lRow = 0; lRow < m_lRows; lRow++ )
      {
         for ( nCol = nCols; nCol < nColCnt; nCol++ )
            DestroyCell( lRow, nCol );
      }
   }

   TRY
   {
      // Change the number of columns.
      m_arColWidths.SetSize( nCols );

      // Change the number of columns in each row.
      if ( GetVirtualMode( ) == FALSE )
      {
         for ( int k = 0; k < m_lRows; k++ )
         {
            if ( m_RowData[ k ] )
               m_RowData[ k ]->SetSize( nCols );
         }
      }

      // If we have just added columns, we need to construct new elements
      // for each cell and set the default column width.
      if ( nAddedCols > 0 )
      {
         // Initialized column widths.
         int nStartCol = nCols - nAddedCols;
         int nWidth = m_cellFixedColDef.GetWidth( );
         for ( nCol = nStartCol; nCol < nCols; nCol++ )
            m_arColWidths[ nCol ] = nWidth;

         // Initialize column data.
         ZGridCellBase *pCell;
         if ( GetVirtualMode( ) == FALSE )
         {
            for ( lRow = 0; lRow < m_lRows; lRow++ )
            {
               for ( nCol = nStartCol; nCol < nCols; nCol++ )
               {
                  GRID_ROW *pRow = m_RowData[ lRow ];
                  if ( pRow )
                  {
                  // DestroyCell( lRow, nCol );
                     pCell = GetCell( lRow, nCol );
                     if ( pCell )
                     {
                        SetItemState( lRow, nCol, 0 );
                        InitCell( pCell, lRow, nCol );
                     }
                     else
                        pRow->SetAt( nCol, CreateCell( lRow, nCol ) );
                  }
               }
            }
         }
      }
   // else   // check for selected cell ranges
   //    ResetSelectedRange( );
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      bResult = FALSE;
   }
   END_CATCH

   m_nCols = nCols;

   SetModified( );
   ResetScrollBars( );
   Refresh( );

   return( bResult );
}

// Insert a column at a given position, or add to end of columns (if nCol = -1)
int
ZGrid::InsertColumn( zCPCHAR cpcHeading,
                     UINT uFormat /* = DT_CENTER | DT_VCENTER | DT_SINGLELINE */,
                     int nCol /* = -1 */ )
{
   int nColCnt = GetColumnCount( );
   if ( nCol >= 0 && nCol < m_nFixedCols )
   {
      // TODO: Fix it so column insertion works for in the fixed column area.
      ASSERT( FALSE );
      return( -1 );
   }

   // If the insertion is for a specific column, check its within range.
   if ( nCol >= 0 && nCol > nColCnt )
      return( -1 );

   // Force recalculation
   m_cellIdTopLeft.m_nCol = -1;

   ResetSelectedRange( );

   // Gotta be able to at least see _some_ of the column.
   if ( m_lRows < 1 )
      SetRowCount( 1 );

   // Allow the user to insert after the last of the columns, but process it
   // as a "-1" column, meaning it gets flaged as being the last column, and
   // not a regular "insert" routine.
   if ( nCol == nColCnt )
      nCol = -1;

   TRY
   {
      if ( nCol < 0 )
      {
         nCol = m_nCols;
         m_arColWidths.Add( 0 );
         if ( GetVirtualMode( ) == FALSE )
         {
            for ( long lRow = 0; lRow < m_lRows; lRow++ )
            {
               GRID_ROW *pRow = m_RowData[ lRow ];
               if ( pRow == 0 )
                  return( -1 );

               pRow->Add( CreateCell( lRow, nCol ) );
            }
         }
      }
      else
      {
         m_arColWidths.InsertAt( nCol, (CUIntArray *) 0 );
         if ( GetVirtualMode( ) == FALSE )
         {
            for ( long lRow = 0; lRow < m_lRows; lRow++ )
            {
               GRID_ROW *pRow = m_RowData[ lRow ];
               if ( pRow == 0 )
                  return( -1 );

               pRow->InsertAt( nCol, CreateCell( lRow, nCol ) );
            }
         }
      }
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return( FALSE );
   }
   END_CATCH

   m_nCols++;
   if ( m_nColsMax < m_nCols )
      m_nColsMax = m_nCols;

   // Initialize column data.
   SetItemText( 0, nCol, cpcHeading );
   for ( long lRow = 0; lRow < m_lRows; lRow++ )
      SetItemFormat( lRow, nCol, uFormat );

   // Initialize column width.
   m_arColWidths[ nCol ] = GetTextExtent( 0, nCol, cpcHeading ).cx;

   if ( m_cellIdCurrent.m_nCol != -1 && nCol < m_cellIdCurrent.m_nCol )
      m_cellIdCurrent.m_nCol++;

   ResetScrollBars( );
   SetModified( );

   return( nCol );
}

// Insert a row at a given position, or add to end of rows (if lRow = -1).
int
ZGrid::InsertRow( zCPCHAR cpcHeading, long lRow /* = -1 */ )
{
   if ( lRow >= 0 && lRow < m_nFixedRows )
   {
      // TODO: Fix it so column insertion works for in the fixed row area.
      ASSERT( FALSE );
      return( -1 );
   }

   // If the insertion is for a specific row, check it's within range.
   if ( lRow >= 0 && lRow >= GetRowCount( ) )
      return( -1 );

   // Force recalculation
   m_cellIdTopLeft.m_nCol = -1;

   ResetSelectedRange( );

   // Gotta be able to at least _see_ some of the row.
   if ( m_nCols < 1 )
      SetColumnCount( 1 );

   TRY
   {
      // Adding a row to the bottom.
      if ( lRow < 0 )
      {
         lRow = m_lRows;
         m_arRowHeights.Add( 0 );
         if ( GetVirtualMode( ) == FALSE )
            m_RowData.Add( new GRID_ROW );
      }
      else
      {
         m_arRowHeights.InsertAt( lRow, (CUIntArray *) 0 );
         if ( GetVirtualMode( ) == FALSE )
            m_RowData.InsertAt( lRow, new GRID_ROW );
      }

      if ( GetVirtualMode( ) == FALSE )
         m_RowData[ lRow ]->SetSize( m_nCols );
   }
   CATCH ( CMemoryException, e )
   {
      e->ReportError( );
      return( FALSE );
   }
   END_CATCH

   m_lRows++;

   // Initialize cell data.
   if ( GetVirtualMode( ) == FALSE )
   {
      for ( int nCol = 0; nCol < m_nCols; nCol++ )
      {
         GRID_ROW *pRow = m_RowData[ lRow ];
         if ( pRow == 0 )
            return( -1 );

         pRow->SetAt( nCol, CreateCell( lRow, nCol ) );
      }
   }

   // Set row title.
   SetItemText( lRow, 0, cpcHeading );

   // Initialized row height.
// if ( cpcHeading && cpcHeading[ 0 ] )
//    m_arRowHeights[ lRow ] = GetTextExtent( lRow, 0, cpcHeading ).cy;
// else
//    m_arRowHeights[ lRow ] = m_cellFixedRowDef.GetHeight( );
      m_arRowHeights[ lRow ] = m_cellDefault.GetHeight( );

   if ( m_cellIdCurrent.m_lRow != -1 && lRow < m_cellIdCurrent.m_lRow )
      m_cellIdCurrent.m_lRow++;

   ResetScrollBars( );
   SetModified( );

   return( lRow );
}

/////////////////////////////////////////////////////////////////////////////
//
//  PURPOSE:    Set new Mapping for a Grid or a Grid column
//
//  NOTE:  If the "last" column's if text and attribute are empty, it is
//         deleted from the list of columns.
//
//  PARAMETERS: vSubtask         - The subtask view for the window containing
//                                 the control to be mapped.
//              cpcCtrlTag       - The unique name of the control.
//              cpcViewName      - new View Name to be mapped
//              cpcEntityName    - new Entity Name to be mapped
//              cpcAttributeName - new Attribute Name to be mapped
//              cpcContextName   - new Context Name to be mapped
//              lColumn          - Column for which the new mapping is to be
//                                 applied (0 ==> change grid mapping).
//
//  RETURNS:    0 - New Mapping set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZGrid::SetCtrlMapping( zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                       zCPCHAR cpcAttrib, zCPCHAR cpcContext,
                       zLONG lColumn )
{
   if ( lColumn == 0 )  // Remap the grid
   {
      ZMapAct::SetCtrlMapping( cpcViewName, cpcEntity,
                               cpcAttrib, cpcContext, lColumn );
      if ( GetViewByName( &m_vAppList, m_csViewNameList,
                          m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
      {
         DropView( m_vAppList );
      }
   }
   else
   {
      if ( cpcViewName )
         *m_pzsVName = cpcViewName;

      if ( lColumn > 0 && lColumn <= m_nColsMax )
      {
         mDeleteInitA( m_pCol[ lColumn ].pchEntity );
         mDeleteInitA( m_pCol[ lColumn ].pchAttrib );
         mDeleteInitA( m_pCol[ lColumn ].pchContext );
         zLONG lLth = zstrlen(cpcEntity) + 1;
         m_pCol[ lColumn ].pchEntity = new char[ lLth ];
         strcpy_s( m_pCol[ lColumn ].pchEntity, lLth, cpcEntity );
         lLth = zstrlen( cpcAttrib ) + 1;
         m_pCol[ lColumn ].pchAttrib = new char[ lLth ];
         strcpy_s( m_pCol[ lColumn ].pchAttrib, lLth, cpcAttrib );
         if ( cpcContext && *cpcContext )
         {
            lLth = zstrlen( cpcContext ) + 1;
            m_pCol[ lColumn ].pchContext = new char[ lLth ];
            strcpy_s( m_pCol[ lColumn ].pchContext, lLth, cpcContext );
         }

         // Remove "last" column if text and attribute are empty.
         if ( lColumn >= m_nCols )
         {
            InsertColumn( "" );
         }

      // if ( lColumn == m_nCols - 1 )
         if ( lColumn == m_nCols )  // - 1 ??? dks 2003.07.21
         {
            while ( m_nCols > 0 &&
                    (m_pCol[ lColumn ].pchText == 0 ||
                     m_pCol[ lColumn ].pchText[ 0 ] == 0) &&
                    (m_pCol[ lColumn ].pchAttrib == 0 ||
                     m_pCol[ lColumn ].pchAttrib[ 0 ] == 0) )
            {
               m_nCols--;
               if ( DeleteColumn( m_nCols ) == FALSE )
               {
                  TraceLineI( "ZGrid::SetCtrlMapping DeleteColumn failed: ",m_nCols );
               }

               lColumn--;
            }
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  PURPOSE:    Get Mapping for a Grid or a Grid column
//
//  PARAMETERS: pchViewName      - Mapping View Name
//              pchEntityName    - Mapping Entity Name
//              pchAttributeName - Mapping Attribute Name
//              pchContextName   - Mapping Context Name
//              lColumn          - Column for which the new mapping is to be
//                                 retrieved (ignored for single map ctrls).
//
//  RETURNS:    0 - Mapping returned successfully
//              1 - Invalid request (e.g.  outside range of valid columns)
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZGrid::GetCtrlMapping( zPCHAR pchViewName, zPCHAR pchEntity,
                       zPCHAR pchAttrib, zPCHAR pchContext,
                       zLONG lColumn )
{
   if ( lColumn == 0 )
   {
      ZMapAct::GetCtrlMapping( pchViewName, pchEntity,
                               pchAttrib, pchContext, lColumn );
   }
   else
   {
      if ( m_pzsVName )
         strcpy_s( pchViewName, zTAG_LTH, *m_pzsVName );
      else
         pchViewName[ 0 ] = 0;

      if ( lColumn > 0 && lColumn <= m_nColsMax )
      {
         strcpy_s( pchEntity, zTAG_LTH, m_pCol[ lColumn ].pchEntity );
         strcpy_s( pchAttrib, zTAG_LTH, m_pCol[ lColumn ].pchAttrib );
         strcpy_s( pchContext, zTAG_LTH, m_pCol[ lColumn ].pchContext );
      }
      else
      {
         pchEntity[ 0 ] = 0;
         pchAttrib[ 0 ] = 0;
         pchContext[ 0 ] = 0;
         return( 1 );
      }
   }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// Cell creation stuff

ZGridCellBase *
ZGrid::SetCellType( long lRow, int nCol, CRuntimeClass *pRuntimeClass )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ASSERT( IsValid( lRow, nCol ) );
   if ( IsValid( lRow, nCol ) == FALSE )
      return( 0 );

   if ( pRuntimeClass->IsDerivedFrom( RUNTIME_CLASS( ZGridCellBase ) ) == FALSE )
   {
      ASSERT( FALSE );
      return( 0 );
   }

   ZGridCellBase *pNewCell = (ZGridCellBase *) pRuntimeClass->CreateObject( );

   ZGridCellBase *pCurrCell = GetCell( lRow, nCol );
   if ( pCurrCell )
      *pNewCell = *pCurrCell;

   SetCell( lRow, nCol, pNewCell );
   delete( pCurrCell );

   return( pNewCell );
}

zBOOL
ZGrid::SetDefaultCellType( CRuntimeClass *pRuntimeClass )
{
   ASSERT( pRuntimeClass );
   if ( pRuntimeClass->IsDerivedFrom( RUNTIME_CLASS( ZGridCellBase ) ) == FALSE )
   {
      ASSERT( FALSE );
      return( FALSE );
   }

   m_pRtcDefault = pRuntimeClass;
   return( TRUE );
}

// Initializes a cell.
// virtual
void
ZGrid::InitCell( ZGridCellBase *pCell, long lRow, int nCol )
{
   ASSERT( pCell && GetVirtualMode( ) == FALSE );

   pCell->Reset( );

   pCell->SetGrid( this );
   pCell->SetCoords( lRow, nCol );

   if ( nCol < m_nFixedCols )
      pCell->SetState( pCell->GetState( ) | GVIS_FIXED | GVIS_FIXEDCOL );

   if ( lRow < m_nFixedRows )
      pCell->SetState( pCell->GetState( ) | GVIS_FIXED | GVIS_FIXEDROW );

   pCell->SetFormat( pCell->GetDefaultCell( )->GetFormat( ) );
}

// Creates a new grid cell and performs any necessary initialization.
// virtual
ZGridCellBase *
ZGrid::CreateCell( long lRow, int nCol )
{
   ASSERT( GetVirtualMode( ) == FALSE );

   if ( m_pRtcDefault == 0 ||
        m_pRtcDefault->IsDerivedFrom( RUNTIME_CLASS( ZGridCellBase ) ) == FALSE )
   {
      ASSERT( FALSE );
      return( 0 );
   }

   ZGridCellBase *pCell = (ZGridCellBase *) m_pRtcDefault->CreateObject( );
   if ( pCell == 0 )
      return( 0 );

   InitCell( pCell, lRow, nCol );
   return( pCell );
}

// Performs any cell cleanup necessary to maintain grid integrity.
// virtual
void
ZGrid::DestroyCell( long lRow, int nCol )
{
   // Should NEVER get here in virtual mode.
   ASSERT( GetVirtualMode( ) == FALSE );

   // Set the cells state to 0. If the cell is selected, this
   // will remove the cell from the selected list.
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell )
   {
      SetItemState( lRow, nCol, 0 );
      delete( pCell );
   }
}

zBOOL
ZGrid::DeleteColumn( int nCol )
{
   if ( nCol < 0 || nCol >= GetColumnCount( ) )
      return( FALSE );

   long lRowCnt = GetRowCount( );
   ResetSelectedRange( );

   if ( GetVirtualMode( ) == FALSE )
   {
      for ( long lRow = 0; lRow < lRowCnt; lRow++ )
      {
         GRID_ROW *pRow = m_RowData[ lRow ];
         if ( pRow == 0 )
            return( FALSE );

         DestroyCell( lRow, nCol );
         pRow->RemoveAt( nCol );
      }
   }

   m_arColWidths.RemoveAt( nCol );
   m_nCols--;
   if ( nCol < m_nFixedCols )
      m_nFixedCols--;

   if ( nCol == m_cellIdCurrent.m_nCol )
      m_cellIdCurrent.m_lRow = m_cellIdCurrent.m_nCol = -1;
   else
   if ( nCol < m_cellIdCurrent.m_nCol )
      m_cellIdCurrent.m_nCol--;

   ResetScrollBars( );
   SetModified( );

   return( TRUE );
}

zBOOL
ZGrid::DeleteRow( long lRow )
{
   if ( lRow < 0 || lRow >= GetRowCount( ) )
      return( FALSE );

   ResetSelectedRange( );

   if ( GetVirtualMode( ) == FALSE )
   {
      GRID_ROW *pRow = m_RowData[ lRow ];
      if ( pRow == 0 )
         return( FALSE );

      int nColCnt = GetColumnCount( );
      for ( int nCol = 0; nCol < nColCnt; nCol++ )
         DestroyCell( lRow, nCol );

      delete( pRow );
      m_RowData.RemoveAt( lRow );
   }

   m_arRowHeights.RemoveAt( lRow );

   m_lRows--;
   if ( lRow < m_nFixedRows )
      m_nFixedRows--;

   if ( lRow == m_cellIdCurrent.m_lRow )
      m_cellIdCurrent.m_lRow = m_cellIdCurrent.m_nCol = -1;
   else
   if ( lRow < m_cellIdCurrent.m_lRow )
      m_cellIdCurrent.m_lRow--;

   ResetScrollBars( );
   SetModified( );

   return( TRUE );
}

// Handy function that removes all non-fixed rows.
zBOOL
ZGrid::DeleteNonFixedRows( )
{
   ResetSelectedRange( );

   int  nFixed = GetFixedRowCount( );
   long lRowCnt = GetRowCount( );

   // Delete all data rows
   for ( long lRow = lRowCnt; lRow >= nFixed; lRow-- )
      DeleteRow( lRow );

   return( TRUE );
}

// Removes all rows, columns and data from the grid.
zBOOL
ZGrid::DeleteAllItems( )
{
   ResetSelectedRange( );

   m_arColWidths.RemoveAll( );
   m_arRowHeights.RemoveAll( );

   // Delete all cells in the grid
   if ( GetVirtualMode( ) == FALSE )
   {
      for ( long lRow = 0; lRow < m_lRows; lRow++ )
      {
         for ( int nCol = 0; nCol < m_nCols; nCol++ )
            DestroyCell( lRow, nCol );

         GRID_ROW *pRow = m_RowData[ lRow ];
         delete( pRow );
      }

      // Remove all rows
      m_RowData.RemoveAll( );
   }

   m_cellIdCurrent.m_lRow = m_cellIdCurrent.m_nCol = -1;
   m_lRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;

   ResetScrollBars( );
   SetModified( );

   return( TRUE );
}

void
ZGrid::AutoFill( )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return;

   CRect rect;
   GetClientRect( rect );

   SetColumnCount( rect.Width( ) / m_cellDefault.GetWidth( ) + 1 );
   SetRowCount( rect.Height( ) / m_cellDefault.GetHeight( ) + 1 );
   SetFixedRowCount( 1 );
   SetFixedColumnCount( 1 );
   ExpandToFit( );
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid data functions

// Set CListCtrl::GetNextItem for details
ZCellId
ZGrid::GetNextItem( ZCellId& cellId, int uFlags ) const
{
   long lRowCnt = GetRowCount( );
   int  nColCnt = GetColumnCount( );
   int  nFixedRowCnt = GetFixedRowCount( );
   int  nFixedColCnt = GetFixedColumnCount( );

   if ( (uFlags & GVNI_ALL) == GVNI_ALL )
   {   // GVNI_ALL Search whole Grid beginning from cell
      //        First row (cell.m_lRow) -- ONLY Columns to the right of cell
      //        following rows      -- ALL  Columns
      long lRow = cellId.m_lRow;
      int  nCol = cellId.m_nCol + 1;

      if ( lRow <= 0 )
         lRow = nFixedRowCnt;

      for ( ; lRow < lRowCnt; lRow++ )
      {
         if ( nCol <= 0 )
            nCol = nFixedColCnt;

         for ( ; nCol < nColCnt; nCol++ )
         {
            UINT uState = GetItemState( lRow, nCol );
            if ( (uFlags & GVNI_DROPHILITED && uState & GVIS_DROPHILITED) ||
                 (uFlags & GVNI_FOCUSED && uState & GVIS_FOCUSED)     ||
                 (uFlags & GVNI_SELECTED && uState & GVIS_SELECTED)   ||
                 (uFlags & GVNI_READONLY && uState & GVIS_READONLY)   ||
                 (uFlags & GVNI_FIXED && uState & GVIS_FIXED)         ||
                 (uFlags & GVNI_MODIFIED && uState & GVIS_MODIFIED) )
            {
               return( ZCellId( lRow, nCol ) );
            }
         }

         // go to First Column
         nCol = nFixedColCnt;
      }
   }
   else
   if ( (uFlags & GVNI_BELOW) == GVNI_BELOW &&
        (uFlags & GVNI_TORIGHT) == GVNI_TORIGHT )
   {
      // GVNI_AREA Search Grid beginning from cell to Lower-Right of Grid
      //         Only rows starting with cell.m_lRow and below
      //         All rows -- ONLY Columns to the right of cell
      long lRow = cellId.m_lRow;
      if ( lRow <= 0 )
         lRow = nFixedRowCnt;

      for ( ; lRow < lRowCnt; lRow++ )
      {
         int nCol = cellId.m_nCol + 1;
         if ( nCol <= 0 )
            nCol = nFixedColCnt;

         for ( ; nCol < nColCnt; nCol++ )
         {
            UINT uState = GetItemState( lRow, nCol );
            if ( (uFlags & GVNI_DROPHILITED && uState & GVIS_DROPHILITED) ||
                 (uFlags & GVNI_FOCUSED && uState & GVIS_FOCUSED)     ||
                 (uFlags & GVNI_SELECTED && uState & GVIS_SELECTED)   ||
                 (uFlags & GVNI_READONLY && uState & GVIS_READONLY)   ||
                 (uFlags & GVNI_FIXED && uState & GVIS_FIXED)         ||
                 (uFlags & GVNI_MODIFIED && uState & GVIS_MODIFIED) )
            {
               return( ZCellId( lRow, nCol ) );
            }
         }
      }
   }
   else
   if ( (uFlags & GVNI_ABOVE) == GVNI_ABOVE )
   {
      for ( long lRow = cellId.m_lRow - 1; lRow >= nFixedRowCnt; lRow-- )
      {
         UINT uState = GetItemState( lRow, cellId.m_nCol );
         if ( (uFlags & GVNI_DROPHILITED && uState & GVIS_DROPHILITED) ||
              (uFlags & GVNI_FOCUSED    && uState & GVIS_FOCUSED)    ||
              (uFlags & GVNI_SELECTED   && uState & GVIS_SELECTED)   ||
              (uFlags & GVNI_READONLY   && uState & GVIS_READONLY)   ||
              (uFlags & GVNI_FIXED      && uState & GVIS_FIXED)      ||
              (uFlags & GVNI_MODIFIED   && uState & GVIS_MODIFIED) )
         {
            return( ZCellId( lRow, cellId.m_nCol ) );
         }
      }
   }
   else
   if ( (uFlags & GVNI_BELOW) == GVNI_BELOW )
   {
      for ( long lRow = cellId.m_lRow + 1; lRow < lRowCnt; lRow++ )
      {
         UINT uState = GetItemState( lRow, cellId.m_nCol );
         if ( (uFlags & GVNI_DROPHILITED && uState & GVIS_DROPHILITED) ||
              (uFlags & GVNI_FOCUSED  && uState & GVIS_FOCUSED)    ||
              (uFlags & GVNI_SELECTED && uState & GVIS_SELECTED)   ||
              (uFlags & GVNI_READONLY && uState & GVIS_READONLY)   ||
              (uFlags & GVNI_FIXED    && uState & GVIS_FIXED)      ||
              (uFlags & GVNI_MODIFIED && uState & GVIS_MODIFIED) )
         {
            return( ZCellId( lRow, cellId.m_nCol ) );
         }
      }
   }
   else
   if ( (uFlags & GVNI_TOLEFT) == GVNI_TOLEFT )
   {
      for ( int nCol = cellId.m_nCol - 1; nCol >= nFixedColCnt; nCol-- )
      {
         UINT uState = GetItemState( cellId.m_lRow, nCol );
         if ( (uFlags & GVNI_DROPHILITED && uState & GVIS_DROPHILITED) ||
              (uFlags & GVNI_FOCUSED    && uState & GVIS_FOCUSED)    ||
              (uFlags & GVNI_SELECTED   && uState & GVIS_SELECTED)   ||
              (uFlags & GVNI_READONLY   && uState & GVIS_READONLY)   ||
              (uFlags & GVNI_FIXED      && uState & GVIS_FIXED)      ||
              (uFlags & GVNI_MODIFIED   && uState & GVIS_MODIFIED) )
         {
            return( ZCellId( cellId.m_lRow, nCol ) );
         }
      }
   }
   else
   if ( (uFlags & GVNI_TORIGHT) == GVNI_TORIGHT )
   {
      for ( int nCol = cellId.m_nCol + 1; nCol < nColCnt; nCol++ )
      {
         UINT uState = GetItemState( cellId.m_lRow, nCol );
         if ( (uFlags & GVNI_DROPHILITED && uState & GVIS_DROPHILITED) ||
              (uFlags & GVNI_FOCUSED    && uState & GVIS_FOCUSED)    ||
              (uFlags & GVNI_SELECTED   && uState & GVIS_SELECTED)   ||
              (uFlags & GVNI_READONLY   && uState & GVIS_READONLY)   ||
              (uFlags & GVNI_FIXED      && uState & GVIS_FIXED)      ||
              (uFlags & GVNI_MODIFIED   && uState & GVIS_MODIFIED) )
         {
            return( ZCellId( cellId.m_lRow, nCol ) );
         }
      }
   }

   return( ZCellId( -1, -1 ) );
}

// Sorts on a given column using the cell text
zBOOL
ZGrid::SortTextItems( int nCol, zBOOL bAscending, LPARAM data /* = 0 */ )
{
   SetSortColumn( nCol );
   SetSortAscending( bAscending );
   ResetSelectedRange( );
   SetFocusCell( -1, -1 );
   return( ZGrid::SortItems( CellTextCompare, nCol, bAscending, data ) );
}

void
ZGrid::SetCompareFunction( PFNLVCOMPARE pfnCompare )
{
   m_pfnCompare = pfnCompare;
}

// Sorts on a given column using the cell text and using the specified
// comparison function.
zBOOL
ZGrid::SortItems( int nCol, zBOOL bAscending, LPARAM data /* = 0 */ )
{
   SetSortColumn( nCol );
   SetSortAscending( bAscending );
   ResetSelectedRange( );
   SetFocusCell( -1, -1 );

   if ( m_pfnCompare == 0 )
   {
      return( ZGrid::SortItems( CellTextCompare, nCol,
                                bAscending, (zLONG) this ) );
   }
   else
   {
      return( ZGrid::SortItems( m_pfnCompare, nCol,
                                bAscending, (zLONG) this ) );
   }
}

// Sorts on a given column using the supplied compare function
// (see CListCtrl::SortItems).
zBOOL
ZGrid::SortItems( PFNLVCOMPARE pfnCompare, int nCol, zBOOL bAscending,
                  LPARAM data /* = 0 */ )
{
   SetSortColumn( nCol );
   SetSortAscending( bAscending );
   ResetSelectedRange( );
   SetFocusCell( -1, -1 );
   return( SortItems( pfnCompare, nCol, bAscending,
                      data, GetFixedRowCount( ), -1 ) );
}

int CALLBACK
CellTextCompare( LPARAM lParam1,
                           LPARAM lParam2,
                           LPARAM lGrid )
{
   ASSERT( lGrid );

   ZGrid *pGrid = (ZGrid *) lGrid;
   ZCellId *pCellId1 = (ZCellId *) lParam1;
   ZCellId *pCellId2 = (ZCellId *) lParam2;
   ZGridCellBase *pCell1 = pGrid->GetCell( pCellId1->m_lRow,
                                           pCellId1->m_nCol );
   ZGridCellBase *pCell2 = pGrid->GetCell( pCellId2->m_lRow,
                                           pCellId2->m_nCol );
   if ( pCell1 == 0 || pCell2 == 0 )
      return( 0 );

   CString cs1;
   CString cs2;

   pCell1->GetText( cs1, pCellId1->m_lRow, pCellId1->m_nCol );
   pCell2->GetText( cs2, pCellId2->m_lRow, pCellId2->m_nCol );

   return( _tcscmp( cs1, cs2 ) );
}

int CALLBACK
CellNumericCompare( LPARAM lParam1,
                    LPARAM lParam2,
                    LPARAM lGrid )
{
   ASSERT( lGrid );

   ZGrid *pGrid = (ZGrid *) lGrid;
   ZCellId *pCellId1 = (ZCellId *) lParam1;
   ZCellId *pCellId2 = (ZCellId *) lParam2;
   ZGridCellBase *pCell1 = pGrid->GetCell( pCellId1->m_lRow,
                                           pCellId1->m_nCol );
   ZGridCellBase *pCell2 = pGrid->GetCell( pCellId2->m_lRow,
                                           pCellId2->m_nCol );
   if ( pCell1 == 0 || pCell2 == 0 )
      return( 0 );

   CString cs1;
   CString cs2;

   pCell1->GetText( cs1, pCellId1->m_lRow, pCellId1->m_nCol );
   pCell2->GetText( cs2, pCellId2->m_lRow, pCellId2->m_nCol );

   int nValue1 = _ttol( cs1 );
   int nValue2 = _ttol( cs2 );

   if ( nValue1 < nValue2 )
     return( -1 );
   else
   if ( nValue1 == nValue2 )
     return( 0 );
   else
     return( 1 );
}

// Private recursive sort implementation.
zBOOL
ZGrid::SortItems( PFNLVCOMPARE pfnCompare, int nCol,
                  zBOOL bAscending, LPARAM data,
                  long lLow, long lHigh )
{
   int nColCnt = GetColumnCount( );
   if ( nCol >= nColCnt )
      return( FALSE );

   if ( lHigh == -1 )
      lHigh = GetRowCount( ) - 1;

   long lLo = lLow;
   long lHi = lHigh;

   if ( lHi <= lLo )
      return( FALSE );

   // LPARAM midItem = GetItemData( (lLo + lHi) / 2, nCol );
   // LPARAM pMidCell = (LPARAM) GetCell( (lLo + lHi) / 2, nCol );
   ZCellId cellIdMid;
   ZCellId cellId;

   cellIdMid.m_nCol = nCol;
   cellIdMid.m_lRow = (lLo + lHi) / 2;
   cellId.m_nCol = nCol;

   // Loop through the list until indices cross.
   while ( lLo <= lHi )
   {
      // Find the first element that is greater than or equal to the
      // partition element starting from the left Index.
      if ( bAscending )
      {
         cellId.m_lRow = lLo;
         while ( lLo < lHigh &&
                 pfnCompare( (LPARAM) &cellId,
                             (LPARAM) &cellIdMid, data ) < 0 )
         {
            lLo++;
            cellId.m_lRow = lLo;
         }
      }
      else
      {
         cellId.m_lRow = lLo;
         while ( lLo < lHigh &&
                 pfnCompare( (LPARAM) &cellId,
                             (LPARAM) &cellIdMid, data ) > 0 )
         {
            lLo++;
            cellId.m_lRow = lLo;
         }
      }

      // Find an element that is smaller than or equal to  the partition
      // element starting from the right Index.
      if ( bAscending )
      {
         cellId.m_lRow = lHi;
         while ( lHi > lLow &&
                 pfnCompare( (LPARAM) &cellId,
                             (LPARAM) &cellIdMid, data ) > 0 )
         {
            lHi--;
            cellId.m_lRow = lHi;
         }
      }
      else
      {
         while ( lHi > lLow &&
                 pfnCompare( (LPARAM) &cellId,
                             (LPARAM) &cellIdMid, data ) < 0 )
         {
            lHi--;
            cellId.m_lRow = lHi;
         }
      }

      // If the indexes have not crossed, swap if the items are not equal.
      if ( lLo <= lHi )
      {
         // Swap only if the items are not equal.
         cellId.m_lRow = lLo;
         cellIdMid.m_lRow = lHi;
         if ( pfnCompare( (LPARAM) &cellId,
                          (LPARAM) &cellIdMid, data ) != 0 )
         {
            for ( int nCol = 0; nCol < nColCnt; nCol++ )
            {
               ZGridCellBase *pCell = GetCell( lLo, nCol );
               SetCell( lLo, nCol, GetCell( lHi, nCol ) );
               SetCell( lHi, nCol, pCell );
            }

            UINT uRowHeight = m_arRowHeights[ lLo ];
            m_arRowHeights[ lLo ] = m_arRowHeights[ lHi ];
            m_arRowHeights[ lHi ] = uRowHeight;
         }

         lLo++;
         lHi--;
      }
   }

   // If the right index has not reached the left side of array
   // must now sort the left partition.
   if ( lLow < lHi )
      SortItems( pfnCompare, nCol, bAscending, data, lLow, lHi );

   // If the left index has not reached the right side of array
   // must now sort the right partition.
   if ( lLo < lHigh )
      SortItems( pfnCompare, nCol, bAscending, data, lLo, lHigh );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid data functions

zBOOL
ZGrid::SetItem( const GV_ITEM *pItem )
{
   if ( pItem == 0 || GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( pItem->lRow, pItem->nCol );
   if ( pCell == 0 )
      return( FALSE );

   SetModified( TRUE, pItem->lRow, pItem->nCol );

   if ( pItem->mask & GVIF_TEXT )
      pCell->SetText( pItem->csText, pItem->lRow, pItem->nCol );

   if ( pItem->mask & GVIF_PARAM )
      pCell->SetData( pItem->lItemExtra );

   if ( pItem->mask & GVIF_IMAGE )
      pCell->SetImage( pItem->nImage );

   if ( pItem->mask & GVIF_STATE )
      SetItemState( pItem->lRow, pItem->nCol, pItem->dwState );

   if ( pItem->mask & GVIF_FORMAT )
      pCell->SetFormat( pItem->dwFormat );

   if ( pItem->mask & GVIF_BKCLR )
      pCell->SetBackClr( pItem->clrBk );

   if ( pItem->mask & GVIF_FGCLR )
      pCell->SetTextClr( pItem->clrFg );

   if ( pItem->mask & GVIF_FONT )
      pCell->SetFont( &(pItem->lfFont) );

   if ( pItem->mask & GVIF_MARGIN )
      pCell->SetMargin( pItem->nMargin );

   return( TRUE );
}

zBOOL
ZGrid::GetItem( GV_ITEM *pItem )
{
   if ( pItem == 0 )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( pItem->lRow, pItem->nCol );
   if ( pCell == 0 )
      return( FALSE );

   if ( pItem->mask & GVIF_TEXT )
      GetItemText( pItem->csText, pItem->lRow, pItem->nCol );

   if ( pItem->mask & GVIF_PARAM )
      pItem->lItemExtra = pCell->GetData( pItem->lRow, pItem->nCol );

   if ( pItem->mask & GVIF_IMAGE )
      pItem->nImage = pCell->GetImage( );

   if ( pItem->mask & GVIF_STATE )
      pItem->dwState  = pCell->GetState( );

   if ( pItem->mask & GVIF_FORMAT )
      pItem->dwFormat = pCell->GetFormat( );

   if ( pItem->mask & GVIF_BKCLR )
      pItem->clrBk = pCell->GetBackClr( );

   if ( pItem->mask & GVIF_FGCLR )
      pItem->clrFg = pCell->GetTextClr( );

   if ( pItem->mask & GVIF_FONT )
      memcpy( &(pItem->lfFont), pCell->GetFont( ), sizeof( LOGFONT ) );

   if ( pItem->mask & GVIF_MARGIN )
      pItem->nMargin = pCell->GetMargin( );

   return( TRUE );
}

zBOOL
ZGrid::SetItemText( long lRow, int nCol, zCPCHAR cs )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( FALSE );

   pCell->SetText( cs, lRow, nCol );

   SetModified( TRUE, lRow, nCol );
   return( TRUE );
}

#if !defined( _WIN32_WCE ) || ( _WIN32_WCE >= 210 )
// EFW - 06/13/99 - Added to support printf-style formatting codes.
zBOOL
ZGrid::SetItemTextFmt( long lRow, int nCol, zCPCHAR szFmt, ... )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   CString csText;

   va_list argptr;

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( FALSE );

   // Format the message text.
   va_start( argptr, szFmt );
   csText.FormatV( szFmt, argptr );
   va_end( argptr );

   pCell->SetText( csText, lRow, nCol );

   SetModified( TRUE, lRow, nCol );
   return( TRUE );
}

// EFW - 06/13/99 - Added to support string resource ID.  Supports
// a variable argument list too.
zBOOL
ZGrid::SetItemTextFmtID( long lRow, int nCol, UINT uId, ... )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   CString csFmt;
   CString csText;
   va_list argptr;

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( FALSE );

   // Format the message text.
   va_start( argptr, uId );
   VERIFY( csFmt.LoadString( uId ) );
   csText.FormatV( csFmt, argptr );
   va_end( argptr );

   pCell->SetText( csText, lRow, nCol );

   SetModified( TRUE, lRow, nCol );
   return( TRUE );
}
#endif

zBOOL
ZGrid::SetItemData( long lRow, int nCol, LPARAM lParam )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( FALSE );

   pCell->SetData( lParam );
//?SetModified( TRUE, lRow, nCol );  dks???
   return( TRUE );
}

LPARAM
ZGrid::GetItemData( long lRow, int nCol ) const
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( (LPARAM) 0 );

   return( pCell->GetData( lRow, nCol ) );
}

zBOOL
ZGrid::SetItemImage( long lRow, int nCol, int iImage )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( FALSE );

   pCell->SetImage( iImage );
   SetModified( TRUE, lRow, nCol );
   return( TRUE );
}

int
ZGrid::GetItemImage( long lRow, int nCol ) const
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( -1 );

   return( pCell->GetImage( ) );
}

zBOOL
ZGrid::SetItemState( long lRow, int nCol, UINT uState )
{
   zBOOL bSelected = IsCellSelected( lRow, nCol );

   // If the cell is being unselected, remove it from the selected list.
   if ( bSelected && (uState & GVIS_SELECTED) == 0 )
   {
      ZCellId cellId;
      LARGE_INTEGER key;
      key.LowPart = lRow;
      key.HighPart = nCol;

      if ( m_SelectedCellMap.Lookup( key, (ZCellId &) cellId ) )
         m_SelectedCellMap.RemoveKey( key );
   }
   else // If cell is being selected, add it to the list of selected cells.
   if ( bSelected == FALSE && (uState & GVIS_SELECTED) )
   {
      LARGE_INTEGER key;
      key.LowPart = lRow;
      key.HighPart = nCol;
      ZCellId cellId( lRow, nCol );
      m_SelectedCellMap.SetAt( key, cellId );
   }

   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( FALSE );

   // Set the cell's state.
   pCell->SetState( uState );

   return( TRUE );
}

UINT
ZGrid::GetItemState( long lRow, int nCol ) const
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( 0 );

   return( pCell->GetState( ) );
}

zBOOL
ZGrid::SetItemFormat( long lRow, int nCol, UINT uFormat )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( FALSE );

   pCell->SetFormat( uFormat );
   return( TRUE );
}

UINT
ZGrid::GetItemFormat( long lRow, int nCol ) const
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( 0 );

   return( pCell->GetFormat( ) );
}

zBOOL
ZGrid::SetItemBkColor( long lRow, int nCol, COLORREF clr /* = CLR_DEFAULT */ )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( FALSE );

   pCell->SetBackClr( clr );
   return( TRUE );
}

COLORREF
ZGrid::GetItemBkColor( long lRow, int nCol ) const
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( 0 );

   return( pCell->GetBackClr( ) );
}

zBOOL
ZGrid::SetItemFgColor( long lRow, int nCol,
                       COLORREF clr /* = CLR_DEFAULT */ )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( FALSE );

   pCell->SetTextClr( clr );
   return( TRUE );
}

COLORREF
ZGrid::GetItemFgColor( long lRow, int nCol ) const
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( 0 );

   return( pCell->GetTextClr( ) );
}

zBOOL
ZGrid::SetItemFont( long lRow, int nCol, const LOGFONT *plf )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( FALSE );

   pCell->SetFont( plf );

   return( TRUE );
}

const LOGFONT *
ZGrid::GetItemFont( long lRow, int nCol )
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
   {
      return( GetDefaultCell( lRow < GetFixedRowCount( ),
                              nCol < GetFixedColumnCount( ) )->GetFont( ) );
   }

   return( pCell->GetFont( ) );
}

zBOOL
ZGrid::IsItemEditing( long lRow, int nCol )
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
// ASSERT( pCell );
   if ( pCell == 0 )
      return( FALSE );

   return( pCell->IsEditing( ) );
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long
ZGrid::GetVirtualWidth( ) const
{
   long lVirtualWidth = 0;
   int  nColCnt = GetColumnCount( );
   for ( int k = 0; k < nColCnt; k++ )
      lVirtualWidth += m_arColWidths[ k ];

   return( lVirtualWidth );
}

long
ZGrid::GetVirtualHeight( ) const
{
   long lVirtualHeight = 0;
   long lRowCnt = GetRowCount( );
   for ( int k = 0; k < lRowCnt; k++ )
      lVirtualHeight += m_arRowHeights[ k ];

   return( lVirtualHeight );
}

int
ZGrid::GetRowHeight( long lRow ) const
{
   ASSERT( lRow >= 0 && lRow < m_lRows );
   if ( lRow < 0 || lRow >= m_lRows )
      return( -1 );

   return( m_arRowHeights[ lRow ] );
}

int
ZGrid::GetColumnWidth( int nCol ) const
{
   ASSERT( nCol >= 0 && nCol < m_nCols );
   if ( nCol < 0 || nCol >= m_nCols )
      return( -1 );

   return( m_arColWidths[ nCol ] );
}

zBOOL
ZGrid::SetRowHeight( long lRow, int nHeight )
{
   ASSERT( lRow >= 0 && lRow < m_lRows && nHeight >= 0 );
   if ( lRow < 0 || lRow >= m_lRows || nHeight < 0 )
      return( FALSE );

   // If we reset the height of any row, we want to reset the
   // height of every row in the grid.
   for ( lRow = 0; lRow < m_lRows; lRow++ )
      m_arRowHeights[ lRow ] = nHeight;

   ResetScrollBars( );

   return( TRUE );
}

zBOOL
ZGrid::SetColumnWidth( int nCol, int nWidth )
{
   ASSERT( nCol >= 0 && nCol < m_nCols && nWidth >= 0 );
   if ( nCol < 0 || nCol >= m_nCols || nWidth < 0 )
      return( FALSE );

   m_arColWidths[ nCol ] = nWidth;
   ResetScrollBars( );

   return( TRUE );
}

int
ZGrid::GetFixedRowHeight( ) const
{
   int nHeight = 0;
   for ( int k = 0; k < m_nFixedRows; k++ )
      nHeight += GetRowHeight( k );

   return( nHeight );
}

int
ZGrid::GetFixedColumnWidth( ) const
{
   int nWidth = 0;
   for ( int k = 0; k < m_nFixedCols; k++ )
      nWidth += GetColumnWidth( k );

   return( nWidth );
}

zBOOL
ZGrid::AutoSizeColumn( int nCol, UINT uAutoSizeStyle /*=GVS_DEFAULT*/,
                       zBOOL bResetScroll /*=TRUE*/ )
{
   ASSERT( nCol >= 0 && nCol < m_nCols );
   if ( nCol < 0 || nCol >= m_nCols )
      return( FALSE );

   //  Skip hidden columns when autosizing
   if ( GetColumnWidth( nCol ) <= 0 )
      return( FALSE );

   CSize size;
   CDC *pDC = GetDC( );
   if ( pDC == 0 )
      return( FALSE );

   int nWidth = 0;

   ASSERT( GVS_DEFAULT <= uAutoSizeStyle && uAutoSizeStyle <= GVS_BOTH );
   if ( uAutoSizeStyle == GVS_DEFAULT )
      uAutoSizeStyle = GetAutoSizeStyle( );

   int nStartRow = (uAutoSizeStyle & GVS_HEADER) ? 0 : GetFixedRowCount( );
   int nEndRow = (uAutoSizeStyle & GVS_DATA) ? GetRowCount( ) - 1 : GetFixedRowCount( ) - 1;

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( nStartRow, nCol, nEndRow, nCol ) );

   for ( long lRow = nStartRow; lRow <= nEndRow; lRow++ )
   {
      ZGridCellBase *pCell = GetCell( lRow, nCol );
      if ( pCell )
         size = pCell->GetCellExtent( pDC, lRow, nCol );

      if ( size.cx > nWidth )
         nWidth = size.cx;
   }

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( -1, -1, -1, -1 ) );

   m_arColWidths[ nCol ] = nWidth;

   ReleaseDC( pDC );
   if ( bResetScroll )
      ResetScrollBars( );

   return( TRUE );
}

zBOOL
ZGrid::AutoSizeRow( long lRow, zBOOL bResetScroll /*=TRUE*/ )
{
   ASSERT( lRow >= 0 && lRow < m_lRows );
   if ( lRow < 0 || lRow >= m_lRows )
      return( FALSE );

   //  Skip hidden rows when autosizing.
   if ( GetRowHeight( lRow ) <= 0 )
      return( FALSE );

   CSize size;
   CDC *pDC = GetDC( );
   if ( pDC == 0 )
      return( FALSE );

   int nHeight = 0;
   int nColCnt = GetColumnCount( );

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( lRow, 0, lRow, nColCnt ) );

   for ( int nCol = 0; nCol < nColCnt; nCol++ )
   {
      ZGridCellBase *pCell = GetCell( lRow, nCol );
      if ( pCell )
         size = pCell->GetCellExtent( pDC, lRow, nCol );

      if ( size.cy > nHeight )
         nHeight = size.cy;
   }

   m_arRowHeights[ lRow ] = nHeight;

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( -1, -1, -1, -1 ) );

   ReleaseDC( pDC );
   if ( bResetScroll )
      ResetScrollBars( );

   return( TRUE );
}

void
ZGrid::AutoSizeColumns( UINT uAutoSizeStyle /*=GVS_DEFAULT*/ )
{
   int nColCnt = GetColumnCount( );
   for ( int nCol = 0; nCol < nColCnt; nCol++ )
   {
      //  Skip hidden columns when autosizing
      if ( GetColumnWidth( nCol ) > 0 )
         AutoSizeColumn( nCol, uAutoSizeStyle, FALSE );
   }

   ResetScrollBars( );
}

void
ZGrid::AutoSizeRows( )
{
   long lRowCnt = GetRowCount( );
   for ( long lRow = 0; lRow < lRowCnt; lRow++ )
   {
      //  Skip hidden rows when autosizing
      if ( GetRowHeight( lRow ) > 0 )
         AutoSizeRow( lRow, FALSE );
   }

   ResetScrollBars( );
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns( ) and AutoSizeRows( )
void
ZGrid::AutoSize( UINT uAutoSizeStyle /*=GVS_DEFAULT*/ )
{
   CDC *pDC = GetDC( );
   if ( pDC == 0 )
      return;

   int nColCnt = GetColumnCount( );
   int nFixedRowCnt = GetFixedRowCount( );
   int lRow;
   int nCol;

   ASSERT( GVS_DEFAULT <= uAutoSizeStyle && uAutoSizeStyle <= GVS_BOTH );
   if ( uAutoSizeStyle == GVS_DEFAULT )
      uAutoSizeStyle = GetAutoSizeStyle( );

   int nStartRow = (uAutoSizeStyle & GVS_HEADER) ? 0 : nFixedRowCnt;
   int nEndRow = (uAutoSizeStyle & GVS_DATA) ? GetRowCount( ) - 1 : nFixedRowCnt - 1;

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( nStartRow, 0, nEndRow, nColCnt ) );

   // Row initialization - only work on rows whose height is > 0
   for ( lRow = nStartRow; lRow <= nEndRow; lRow++ )
   {
      if ( GetRowHeight( lRow ) > 0 )
         m_arRowHeights[ lRow ] = 1;
   }

   CSize size;
   for ( nCol = 0; nCol < nColCnt; nCol++ )
   {
      //  Don't size hidden columns or rows
      if ( GetColumnWidth( nCol ) > 0 )
      {
         // Skip columns that are hidden, but now initialize
         m_arColWidths[ nCol ] = 0;
         for ( lRow = nStartRow; lRow <= nEndRow; lRow++ )
         {
            if ( GetRowHeight( lRow ) > 0 )
            {
               ZGridCellBase *pCell = GetCell( lRow, nCol );
               if ( pCell )
                  size = pCell->GetCellExtent( pDC, lRow, nCol );

               if ( size.cx > (int) m_arColWidths[ nCol ] )
                  m_arColWidths[ nCol ] = size.cx;

               if ( size.cy > (int) m_arRowHeights[ lRow ] )
                  m_arRowHeights[ lRow ] = size.cy;
            }
         }
      }
   }

   if ( GetVirtualMode( ) )
      SendCacheHintToParent( ZCellRange( -1, -1, -1, -1 ) );

   ReleaseDC( pDC );

   ResetScrollBars( );
   Refresh( );
}

// Expands the columns to fit the screen space. If bExpandFixed is FALSE then fixed
// columns will not be affected
void
ZGrid::ExpandColumnsToFit( zBOOL bExpandFixed /*=TRUE*/ )
{
   int nColCnt = GetColumnCount( );
   int nFixedColCnt = GetFixedColumnCount( );
   int nCol;
   if ( bExpandFixed )
   {
     if ( nColCnt <= 0 )
        return;
   }
   else
   {
     if ( nColCnt <= nFixedColCnt )
        return;
   }

   EnableScrollBars( SB_HORZ, FALSE );

   CRect rect;
   GetClientRect( rect );

   int nFirstColumn = (bExpandFixed) ? 0 : nFixedColCnt;

   int nNbrColumnsAffected = 0;
   for ( nCol = nFirstColumn; nCol < nColCnt; nCol++ )
   {
      if ( m_arColWidths[ nCol ] > 0 )
         nNbrColumnsAffected++;
   }

   if ( nNbrColumnsAffected <= 0 )
      return;

   long virtualWidth = GetVirtualWidth( );
   int nDifference = rect.Width( ) - (int) virtualWidth;
   int nColAdjustment = nDifference / nNbrColumnsAffected;

   for ( nCol = nFirstColumn; nCol < nColCnt; nCol++ )
   {
      if ( m_arColWidths[ nCol ] > 0 )
         m_arColWidths[ nCol ] += nColAdjustment;
   }

   if ( nDifference > 0 )
   {
      int nLeftOver = nDifference % nNbrColumnsAffected;
      for ( int nCount = 0, nCol = nFirstColumn;
            (nCol < nColCnt) && (nCount < nLeftOver);
            nCol++, nCount++ )
      {
         if ( m_arColWidths[ nCol ] > 0 )
            m_arColWidths[ nCol ] += 1;
      }
   }
   else
   {
      int nLeftOver = (-nDifference) % nNbrColumnsAffected;
      for ( int nCount = 0, nCol = nFirstColumn;
            (nCol < nColCnt) && (nCount < nLeftOver);
            nCol++, nCount++ )
      {
         if ( m_arColWidths[ nCol ] > 0 )
            m_arColWidths[ nCol ] -= 1;
      }
   }

   Refresh( );

   ResetScrollBars( );
}

void
ZGrid::ExpandLastColumn( )
{
   int nColCnt = GetColumnCount( );
   if ( nColCnt <= 0 )
      return;

   // Search for last non-hidden column
   int nLastColumn = nColCnt - 1;
   while ( m_arColWidths[ nLastColumn ] <= 0 )
      nLastColumn--;

   if ( nLastColumn <= 0 )
      return;

   EnableScrollBars( SB_HORZ, FALSE );

   CRect rect;
   GetClientRect( rect );

   long virtualWidth = GetVirtualWidth( );
   int nDifference = rect.Width( ) - (int) virtualWidth;

   if ( nDifference > 0 )
   {
      //if ( GetVirtualHeight( ) > rect.Height( ) )
      //   nDifference -= GetSystemMetrics( SM_CXVSCROLL );

      m_arColWidths[ nLastColumn ] += nDifference;
      Refresh( );
   }

   ResetScrollBars( );
}

// Expands the rows to fit the screen space. If bExpandFixed is FALSE then fixed
// rows will not be affected
void
ZGrid::ExpandRowsToFit( zBOOL bExpandFixed /*=TRUE*/ )
{
   long lRowCnt = GetRowCount( );
   int  nFixedRowCnt = GetFixedRowCount( );
   if ( bExpandFixed )
   {
     if ( lRowCnt <= 0 )
        return;
   }
   else
   {
     if ( lRowCnt <= nFixedRowCnt )
        return;
   }

   EnableScrollBars( SB_VERT, FALSE );

   CRect rect;
   GetClientRect( rect );

   long lRow;
   long lFirstRow = (bExpandFixed) ? 0 : nFixedRowCnt;

   long lNbrRowsAffected = 0;
   for ( lRow = lFirstRow; lRow < lRowCnt; lRow++ )
   {
      if ( m_arRowHeights[ lRow ] > 0 )
         lNbrRowsAffected++;
   }

   if ( lNbrRowsAffected <= 0 )
      return;

   long lVirtualHeight = GetVirtualHeight( );
   long lDifference = rect.Height( ) - lVirtualHeight;
   long lRowAdjustment = lDifference / lNbrRowsAffected;

   for ( lRow = lFirstRow; lRow < lRowCnt; lRow++ )
   {
      if ( m_arRowHeights[ lRow ] > 0 )
         m_arRowHeights[ lRow ] += lRowAdjustment;
   }

   if ( lDifference > 0 )
   {
      long lLeftOver = lDifference % lNbrRowsAffected;
      for ( long lCount = 0, lRow = lFirstRow;
            (lRow < lRowCnt) && (lCount < lLeftOver);
            lRow++, lCount++ )
      {
         if ( m_arRowHeights[ lRow ] > 0 )
            m_arRowHeights[ lRow ] += 1;
      }
   }
   else
   {
      long lLeftOver = (-lDifference) % lNbrRowsAffected;
      for ( long lCount = 0, lRow = lFirstRow;
            (lRow < lRowCnt) && (lCount < lLeftOver);
            lRow++, lCount++ )
      {
         if ( m_arRowHeights[ lRow ] > 0 )
            m_arRowHeights[ lRow ] -= 1;
      }
   }

   Refresh( );
   ResetScrollBars( );
}

// Expands the cells to fit the screen space. If bExpandFixed is FALSE then fixed
// cells will not be affected
void
ZGrid::ExpandToFit( zBOOL bExpandFixed /*=TRUE*/ )
{
   ExpandColumnsToFit( bExpandFixed );  // This will remove any existing horz scrollbar
   ExpandRowsToFit( bExpandFixed );     // This will remove any existing vert scrollbar
   ExpandColumnsToFit( bExpandFixed );  // Just in case the first adjustment was with a
                                        // vert scrollbar in place
   Refresh( );
}

/////////////////////////////////////////////////////////////////////////////////////
// Attributes

void
ZGrid::SetVirtualMode( zBOOL bVirtual )
{
   DeleteAllItems( );
   m_bVirtualMode = bVirtual;

   // Force some defaults here.
   if ( m_bVirtualMode )
   {
      SetEditable( FALSE );
      SetHeaderSort( FALSE );
      SetAutoSizeStyle( GVS_HEADER );
      SetFixedColumnSelection( FALSE );
      SetFixedRowSelection( FALSE );
   }
}

void
ZGrid::SetGridLines( int nWhichLines /*=GVL_BOTH*/ )
{
   m_nGridLines = nWhichLines;
   Refresh( );
}

void
ZGrid::SetListMode( zBOOL bEnableListMode /*=TRUE*/ )
{
   ResetSelectedRange( );
   SetSortColumn( -1 );
   m_bListMode = bEnableListMode;
   SetFixedRowSelection( FALSE );
   Refresh( );
}

void
ZGrid::SetSortColumn( int nCol )
{
   if ( m_nSortColumn >= 0 )
      InvalidateCellRect( 0, m_nSortColumn );

   m_nSortColumn = nCol;
   if ( nCol >= 0 )
      InvalidateCellRect( 0, nCol );
}

zBOOL
ZGrid::IsCellFixed( long lRow, int nCol )
{
   return( lRow < GetFixedRowCount( ) || nCol < GetFixedColumnCount( ) );
}

void
ZGrid::SetModified( zBOOL bModified /*=TRUE*/, long lRow /*=-1*/, int nCol /*=-1*/ )
{
   // Cannot guarantee sorting anymore...
   if ( nCol < 0 || nCol == GetSortColumn( ) )
      SetSortColumn( -1 );

   if ( lRow >= 0 && nCol >= 0 )
   {
      if ( bModified )
      {
         SetItemState( lRow, nCol, GetItemState( lRow, nCol ) | GVIS_MODIFIED );
         m_bModified = TRUE;
      }
      else
         SetItemState( lRow, nCol, GetItemState( lRow, nCol ) & ~GVIS_MODIFIED );
   }
   else
      m_bModified = bModified;

   if ( m_bModified == FALSE )
   {
      long lRowCnt = GetRowCount( );
      int  nColCnt = GetColumnCount( );
      for ( long lRow = 0; lRow < lRowCnt; lRow++ )
      {
         for ( int nCol = 0; nCol < nColCnt; nCol++ )
            SetItemState( lRow, nCol, GetItemState( lRow, nCol ) & ~GVIS_MODIFIED );
      }
   }
}

zBOOL
ZGrid::GetModified( long lRow /*=-1*/, int nCol /*=-1*/ )
{
   if ( lRow >= 0 && nCol >= 0 )
      return( (GetItemState( lRow, nCol ) & GVIS_MODIFIED) == GVIS_MODIFIED );
   else
      return( m_bModified );
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

void
ZGrid::Refresh( )
{
   if ( GetSafeHwnd( ) && m_bAllowDraw )
      Invalidate( );
}

// EnsureVisible supplied by Roelf Werkman
void
ZGrid::EnsureVisible( long lRow, int nCol )
{
   if ( m_bAllowDraw == FALSE )
      return;

   CRect rectWindow;
   /*
   // set the scroll to the approximate position of row (Nigel Page-Jones)
   int nPos = (int) ((float) lRow / GetRowCount( ) * 1000);
   float fPos = (float) nPos / 1000;
   SCROLLINFO scrollInfo;
   GetScrollInfo( SB_VERT, &scrollInfo );
   scrollInfo.nPos = (int) (scrollInfo.nMax * fPos);
   SetScrollInfo( SB_VERT, &scrollInfo, FALSE );

   GetClientRect( rectWindow );

   // redraw cells if necessary (Nigel Page-Jones)
   ZCellId idTopLeft = GetTopLeftNonFixedCell( FALSE );
   ZCellId idNewTopLeft = GetTopLeftNonFixedCell( TRUE );
   if ( idNewTopLeft != idTopLeft )
   {
      rectWindow.top = GetFixedRowHeight( );
      InvalidateRect( rectWindow );
   }
   */

   // We are gonna send some scroll messages, which will steal the focus
   // from its rightful owner. Squirrel it away ourselves so we can give
   // it back. (Damir)
   CWnd *pFocusWnd = GetFocus( );

   ZCellRange VisibleCells = GetVisibleNonFixedCellRange( );

   int right = nCol - VisibleCells.GetMaxCol( );
   int left  = VisibleCells.GetMinCol( ) - nCol;
   int down  = lRow - VisibleCells.GetMaxRow( );
   int up    = VisibleCells.GetMinRow( ) - lRow;

   int nColumnStart;
   int nRowStart;

   nColumnStart = VisibleCells.GetMaxCol( ) + 1;
   while ( right > 0 )
   {
      if ( GetColumnWidth( nColumnStart ) > 0 )
         SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );

      right--;
      nColumnStart++;
   }

   nColumnStart = VisibleCells.GetMinCol( ) - 1;
   while ( left > 0 )
   {
      if ( GetColumnWidth( nColumnStart ) > 0 )
         SendMessage( WM_HSCROLL, SB_LINELEFT, 0 );

      left--;
      nColumnStart--;
   }

   nRowStart = VisibleCells.GetMaxRow( ) + 1;
   while ( down > 0 )
   {
      if ( GetRowHeight( nRowStart ) > 0 )
         SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );

      down--;
      nRowStart++;
   }

   nRowStart = VisibleCells.GetMinRow( ) - 1;
   while ( up > 0 )
   {
      if ( GetRowHeight( nRowStart ) > 0 )
         SendMessage( WM_VSCROLL, SB_LINEUP, 0 );

      up--;
      nRowStart--;
   }

   // Move one more if we only see a snall bit of the cell.
   CRect rectCell;
   if ( GetCellRect( lRow, nCol, rectCell ) == FALSE )
   {
      if ( pFocusWnd && mIs_hWnd( pFocusWnd->GetSafeHwnd( ) ) )
         pFocusWnd->SetFocus( );

      return;
   }

   GetClientRect( rectWindow );

   // The previous fix was fixed properly by Martin Richter
   // <martin.richter@grutzeck.de>.
   if ( m_nExtraRowsPerItem == 0 )
   {
      while ( rectCell.right > rectWindow.right &&
              rectCell.left > GetFixedColumnWidth( ) )
      {
         SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );
         if ( GetCellRect( lRow, nCol, rectCell ) == FALSE )
         {
            if ( pFocusWnd && mIs_hWnd( pFocusWnd->GetSafeHwnd( ) ) )
              pFocusWnd->SetFocus( );

            return;
         }
      }
   }

   int nFixedRowHeight = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
   while ( rectCell.bottom > rectWindow.bottom &&
           rectCell.top > nFixedRowHeight )
   {
      SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
      if ( GetCellRect( lRow, nCol, rectCell ) == FALSE )
      {
         if ( pFocusWnd && mIs_hWnd( pFocusWnd->GetSafeHwnd( ) ) )
            pFocusWnd->SetFocus( );

         return;
      }
   }

   // Restore focus to whoever owned it.
   if ( pFocusWnd && mIs_hWnd( pFocusWnd->GetSafeHwnd( ) ) )
      pFocusWnd->SetFocus( );
}

zBOOL
ZGrid::IsCellEditable( ZCellId &cellId ) const
{
   return( IsCellEditable( cellId.m_lRow, cellId.m_nCol ) );
}

zBOOL
ZGrid::IsCellEditable( long lRow, int nCol ) const
{
   return( IsEditable( ) &&
           ((GetItemState( lRow, nCol ) & GVIS_READONLY) != GVIS_READONLY ) );
}

zBOOL
ZGrid::IsCellSelected( ZCellId &cellId ) const
{
   return( IsCellSelected( cellId.m_lRow, cellId.m_nCol ) );
}

zBOOL
ZGrid::IsCellSelected( long lRow, int nCol ) const
{
   if ( GetVirtualMode( ) )
   {
      if ( IsSelectable( ) == FALSE )
         return( FALSE );

      ZCellId cellId;
      LARGE_INTEGER key;
      key.LowPart = lRow;
      key.HighPart = nCol;

      return( m_SelectedCellMap.Lookup( key, (ZCellId &) cellId ) );
   }
   else
   {
      return( IsSelectable( ) &&
              ((GetItemState( lRow, nCol ) & GVIS_SELECTED) == GVIS_SELECTED) );
   }
}

zBOOL
ZGrid::IsCellVisible( ZCellId cellId )
{
   return( IsCellVisible( cellId.m_lRow, cellId.m_nCol ) );
}

zBOOL
ZGrid::IsCellVisible( long lRow, int nCol )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return( FALSE );

   int x, y;
   int nFixedRowCnt = GetFixedRowCount( );
   int nFixedColCnt = GetFixedColumnCount( );

   ZCellId TopLeft;
   if ( nCol >= nFixedColCnt || lRow >= nFixedRowCnt )
   {
      TopLeft = GetTopLeftNonFixedCell( );
      if ( nCol >= nFixedColCnt && nCol < TopLeft.m_nCol )
         return( FALSE );

      if ( lRow >= nFixedRowCnt && lRow < TopLeft.m_lRow )
         return( FALSE );
   }

   CRect rect;
   GetClientRect( rect );
   if ( nCol < nFixedColCnt )
   {
      x = 0;
      for ( int k = 0; k <= nCol; k++ )
      {
         if ( m_nExtraCol && k == m_nExtraCol )
            x = m_nExtraOffset;

         if ( x >= rect.right )
            return( FALSE );

         x += GetColumnWidth( k );
      }
   }
   else
   {
      x = GetFixedColumnWidth( );
      for ( int k = TopLeft.m_nCol; k <= nCol; k++ )
      {
         if ( m_nExtraCol && k == m_nExtraCol )
            x = m_nExtraOffset;

         if ( x >= rect.right )
            return( FALSE );

         x += GetColumnWidth( k );
      }
   }

   if ( lRow < nFixedRowCnt )
   {
      y = 0;
      for ( int k = 0; k <= lRow; k++ )
      {
         if ( y >= rect.bottom )
            return( FALSE );

         y += GetRowHeight( k ) * (m_nExtraRowsPerItem + 1);
      }
   }
   else
   {
      if ( lRow < TopLeft.m_lRow )
         return( FALSE );

      y = GetFixedRowHeight( ) * (m_nExtraRowsPerItem + 1);
      for ( int k = TopLeft.m_lRow; k <= lRow; k++ )
      {
         if ( y >= rect.bottom )
            return( FALSE );

         y += GetRowHeight( k ) * (m_nExtraRowsPerItem + 1);
      }
   }

   return( TRUE );
}

zBOOL
ZGrid::InvalidateCellRect( const ZCellId& cellId )
{
   return( InvalidateCellRect( cellId.m_lRow, cellId.m_nCol ) );
}

zBOOL
ZGrid::InvalidateCellRect( const long lRow, const int nCol )
{
// TraceLine( "ZGrid::InvalidateCellRect Row: %d   Col %d", lRow, nCol );
   if ( mIs_hWnd( GetSafeHwnd( ) ) == FALSE || m_bAllowDraw == FALSE )
      return( FALSE );

   if ( IsValid( lRow, nCol ) == FALSE )
      return( FALSE );

   if ( IsCellVisible( lRow, nCol ) == FALSE )
      return( FALSE );

   CRect rect;
   if ( GetCellRect( lRow, nCol, rect ) == FALSE )
      return( FALSE );

   rect.right++;
   rect.bottom++;
// TraceRect( "ZGrid::InvalidateCellRect", rect );
   InvalidateRect( rect, TRUE );

   return( TRUE );
}

zBOOL
ZGrid::InvalidateCellRect( const ZCellRange& cellRange )
{
   ASSERT( IsValid( cellRange ) );
   if ( mIs_hWnd( GetSafeHwnd( ) ) == FALSE || m_bAllowDraw == FALSE )
      return( FALSE );

   ZCellRange visibleCellRange = GetVisibleNonFixedCellRange( ).Intersect( cellRange );

   CRect rect;
   if ( GetCellRangeRect( visibleCellRange, rect ) == 0 )
      return( FALSE );

   rect.right++;
   rect.bottom++;
   InvalidateRect( rect, TRUE );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid Mouse stuff

// Handles mouse wheel notifications
// Note - if this doesn't work for win95 then use OnRegisteredMouseWheel
// instead.
#if !defined( _WIN32_WCE ) && ( _MFC_VER >= 0x0421 )
BOOL
ZGrid::OnMouseWheel( UINT uFlags, short nDelta, CPoint pt )
{
   // A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
   // wheel scrolls whole pages, not just lines.
   if ( m_nRowsPerWheelNotch == -1 )
   {
      int nPagesScrolled = nDelta / 120;

      if ( nPagesScrolled > 0 )
      {
         for ( int k = 0; k < nPagesScrolled; k++ )
         {
            PostMessage( WM_VSCROLL, SB_PAGEUP, 0 );
         }
      }
      else
      {
         for ( int k = 0; k > nPagesScrolled; k-- )
         {
            PostMessage( WM_VSCROLL, SB_PAGEDOWN, 0 );
         }
      }
   }
   else
   {
      int nRowsScrolled = m_nRowsPerWheelNotch * nDelta / 120;

      if ( nRowsScrolled > 0 )
      {
         for ( int k = 0; k < nRowsScrolled; k++ )
         {
            PostMessage( WM_VSCROLL, SB_LINEUP, 0 );
         }
      }
      else
      {
         for ( int k = 0; k > nRowsScrolled; k-- )
         {
            PostMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
         }
      }
   }

   return( CWnd::OnMouseWheel( uFlags, nDelta, pt ) );
}
#endif // !defined( _WIN32_WCE ) && ( _MFC_VER >= 0x0421 )

void
ZGrid::OnMouseMove( UINT uFlags, CPoint pt )
{
   CRect rect;
   GetClientRect( rect );

#ifndef GRIDCONTROL_NO_DRAGDROP
   // If outside client area, return (unless we are drag n dropping)
   if ( m_MouseMode != MOUSE_DRAGGING && rect.PtInRect( pt ) == FALSE )
      return;
#endif

   // Sometimes a MOUSEMOVE message can come after the left button
   // has been let go, but before the BUTTONUP message has been processed.
   // We'll keep track of mouse buttons manually to avoid this.  All
   // bMouseButtonDown's have been replaced with the member m_bLMouseButtonDown
   // zBOOL bMouseButtonDown = ((uFlags & MK_LBUTTON) == MK_LBUTTON);

   // If the left mouse button is up, then test to see if row/column
   // sizing is imminent.
   if ( m_bLMouseButtonDown == FALSE ||
        (m_bLMouseButtonDown && m_MouseMode == MOUSE_NOTHING) )
   {
      if ( m_bAllowColumnResize && MouseOverColumnResizeArea( pt ) )
      {
         if ( m_MouseMode != MOUSE_OVER_COL_DIVIDE )
         {
#ifndef _WIN32_WCE_NO_CURSOR
            SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZEWE ) );
#endif
            m_MouseMode = MOUSE_OVER_COL_DIVIDE;
         }
      }
      else
      if ( m_bAllowRowResize && MouseOverRowResizeArea( pt ) )
      {
         if ( m_MouseMode != MOUSE_OVER_ROW_DIVIDE )
         {
#ifndef _WIN32_WCE_NO_CURSOR
            SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZENS ) );
#endif
            m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
         }
      }
      else
      if ( m_MouseMode != MOUSE_NOTHING )
      {
#ifndef _WIN32_WCE_NO_CURSOR
         SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_ARROW ) );
#endif
         m_MouseMode = MOUSE_NOTHING;
      }

      if ( m_MouseMode == MOUSE_NOTHING )
      {
         ZGridCellBase *pCell = 0;
         ZCellId cellId;
         if ( GetVirtualMode( ) == FALSE || m_bTitleTips )
         {
            // Let the cell know that a big fat cursor is currently hovering
            // over it.
            cellId = GetCellFromPt( pt );
            pCell = GetCell( cellId.m_lRow, cellId.m_nCol );
            if ( pCell )
               pCell->OnMouseOver( );
         }

#ifndef GRIDCONTROL_NO_TITLETIPS
         // Titletips anyone? anyone?
         if ( m_bTitleTips )
         {
            CRect rectText;
            CRect rectCell;

            if ( pCell )
            {
               CString csTipText;

               pCell->GetTipText( csTipText, cellId.m_lRow, cellId.m_nCol );
               if ( m_bRMouseButtonDown == FALSE &&
                    csTipText.IsEmpty( ) == FALSE &&
                    pCell->IsEditing( ) == FALSE &&
                    GetCellRect( cellId.m_lRow, cellId.m_nCol, rectText ) &&
                    pCell->GetTipTextRect( cellId.m_lRow, cellId.m_nCol,
                                           rectText ) &&
                    GetCellRect( cellId.m_lRow, cellId.m_nCol, rectCell ) )
               {
                  if ( m_ptLastMouse != pt )
                  {
                  // TracePoint( "Showing TitleTip: ", pt );
                     m_wndTitleTip.Show( rectText, csTipText,
                                         0, rectCell, pCell->GetFont( ),
                                         GetTitleTipTextClr( ),
                                         GetTitleTipBackClr( ) );
                  }
               }
            }
         }
#endif
      }

      m_ptLastMouse = pt;
      return;
   }

   if ( IsValid( m_cellLeftClickDown ) == FALSE )
   {
      m_ptLastMouse = pt;
      return;
   }

// if ( zstrcmp( "SSQueryAttributes", *m_pzsTag ) )
// {
//    ZCellId cellId = GetCellFromPt( pt );
//    TracePoint( "ZGrid::OnMouseMove: ", pt );
//    TraceLine( "ZGrid::OnMouseMove Row %d   Col %d", cellId.m_lRow, cellId.m_nCol );
// }

   // If the left mouse button is down, then process appropriately.
   if ( m_bLMouseButtonDown )
   {
      switch ( m_MouseMode )
      {
         case MOUSE_SELECT_ALL:
            break;

         case MOUSE_SELECT_COL:
         case MOUSE_SELECT_ROW:
         case MOUSE_SELECT_CELLS:
         {
            ZCellId cellId = GetCellFromPt( pt );
            if ( IsValid( cellId ) == FALSE )
               return;

            if ( cellId != GetFocusCell( ) )
            {
               OnSelecting( cellId );

               // EFW - BUG FIX - Keep the appropriate cell row and/or column focused.  A
               // fix in SetFocusCell will place the cursor in a non-fixed cell as needed.
               if ( (cellId.m_lRow >= m_nFixedRows && cellId.m_nCol >= m_nFixedCols) ||
                    m_MouseMode==MOUSE_SELECT_COL ||
                    m_MouseMode==MOUSE_SELECT_ROW )
               {
                  SetFocusCell( cellId );
               }
            }

            break;
         }

         case MOUSE_SIZING_COL:
         {
            CDC *pDC = GetDC( );
            if ( pDC == 0 )
               break;

            CRect oldrectInverted( m_ptLastMouse.x, rect.top,
                                   m_ptLastMouse.x + 2, rect.bottom );
            pDC->InvertRect( &oldrectInverted );
            CRect newrectInverted( pt.x, rect.top,
                                   pt.x + 2, rect.bottom );
            pDC->InvertRect( &newrectInverted );
            ReleaseDC( pDC );
         }

         break;

         case MOUSE_SIZING_ROW:
         {
            CDC *pDC = GetDC( );
            if ( pDC == 0 )
               break;

            CRect oldrectInverted( rect.left, m_ptLastMouse.y,
                                   rect.right, m_ptLastMouse.y + 2 );
            pDC->InvertRect( &oldrectInverted );
            CRect newrectInverted( rect.left, pt.y,
                                   rect.right, pt.y + 2 );
            pDC->InvertRect( &newrectInverted );
            ReleaseDC( pDC );
         }

         break;

#ifndef GRIDCONTROL_NO_DRAGDROP
         case MOUSE_PREPARE_EDIT:
         case MOUSE_PREPARE_DRAG:
            m_MouseMode = MOUSE_PREPARE_DRAG;
            OnBeginDrag( );
            break;
#endif
      }
   }

   m_ptLastMouse = pt;
}

// Returns the point inside the cell that was clicked (coords relative to
// cell top left).
CPoint
ZGrid::GetPointClicked( long lRow, int nCol, const CPoint& pt )
{
   CPoint ptCellOrigin;
   if ( GetCellOrigin( lRow, nCol, ptCellOrigin ) == FALSE )
      return( CPoint( 0, 0 ) );

   CPoint ptClickedCellRelative( pt );
   ptClickedCellRelative -= ptCellOrigin;
   return( ptClickedCellRelative );
}

void
ZGrid::OnLButtonDblClk( UINT uFlags, CPoint pt )
{
// TracePoint( "ZGrid::OnLButtonDblClk", pt );

   ZCellId cellId = GetCellFromPt( pt );
   if ( IsValid( cellId ) == FALSE )
   {
      //ASSERT( FALSE );
      return;
   }

// TraceLine( "ZGrid::OnLButtonDblClk Row %d   Col %d", cellId.m_lRow, cellId.m_nCol );

   if ( cellId != m_cellIdCurrent &&
        ((cellId.m_nCol >= m_nFixedCols || m_MouseMode != MOUSE_OVER_ROW_DIVIDE) &&
         (cellId.m_lRow >= m_nFixedRows || m_MouseMode != MOUSE_OVER_COL_DIVIDE)) )
   {
      OnLButtonDown( uFlags, pt );
      return;
   }

#ifdef _WIN32_WCE
   if ( MouseOverColumnResizeArea( pt ) )
#else
   if ( m_MouseMode == MOUSE_OVER_COL_DIVIDE )
#endif
   {
      CPoint ptStart;
      if ( GetCellOrigin( 0, cellId.m_nCol, ptStart ) == FALSE )
         return;

      if ( pt.x - ptStart.x < m_nResizeCaptureRange ) // clicked right of border
         cellId.m_nCol--;

      // Ignore columns that are hidden and look left towards first
      // visible column.
      zBOOL bFoundVisible = FALSE;
      while ( cellId.m_nCol >= 0 )
      {
         if ( GetColumnWidth( cellId.m_nCol ) > 0 )
         {
            bFoundVisible = TRUE;
            break;
         }

         cellId.m_nCol--;
      }

      if ( bFoundVisible == FALSE )
         return;

      AutoSizeColumn( cellId.m_nCol, GetAutoSizeStyle( ) );
      Invalidate( );
   }
#ifdef _WIN32_WCE
   else
   if ( MouseOverRowResizeArea( pt ) )
#else
   else
   if ( m_MouseMode == MOUSE_OVER_ROW_DIVIDE )
#endif
   {
      CPoint ptStart;
      if ( GetCellOrigin( 0, cellId.m_nCol, ptStart ) == 0 )
         return;

      if ( pt.y - ptStart.y < m_nResizeCaptureRange ) // clicked below border
         cellId.m_lRow--;

      // Ignore rows that are hidden and look up towards first visible row.
      zBOOL bFoundVisible = FALSE;
      while ( cellId.m_lRow >= 0 )
      {
         if ( GetRowHeight( cellId.m_lRow ) > 0 )
         {
            bFoundVisible = TRUE;
            break;
         }

         cellId.m_lRow--;
      }

      if ( bFoundVisible == FALSE )
         return;

      SetRowHeight( 0, m_cellDefault.GetHeight( ) );
   // AutoSizeRow( cellId.m_lRow );
      Invalidate( );
   }
   else
   if ( m_MouseMode == MOUSE_NOTHING )
   {
      CPoint pointClickedRel;
      pointClickedRel = GetPointClicked( cellId.m_lRow, cellId.m_nCol, pt );

      ZGridCellBase *pCell = 0;
      if ( IsValid( cellId ) )
         pCell = GetCell( cellId.m_lRow, cellId.m_nCol );

      // Clicked in the text area? Only then will cell selection work.
      zBOOL bInTextArea = FALSE;
      if ( pCell )
      {
         CRect rectCell;
         if ( GetCellRect( cellId.m_lRow, cellId.m_nCol, rectCell ) &&
              pCell->GetTextRect( rectCell ) )
         {
            bInTextArea = rectCell.PtInRect( pt );
         }
      }

      if ( cellId.m_lRow >= m_nFixedRows && IsValid( m_cellLeftClickDown ) &&
           cellId.m_nCol >= m_nFixedCols && bInTextArea )
      {
         OnEditCell( cellId.m_lRow, cellId.m_nCol, pointClickedRel, VK_LBUTTON );
      }
      else
      if ( m_bListMode )
      {
         if ( IsValid( cellId ) == FALSE )
            return;

         if ( cellId.m_lRow >= m_nFixedRows && cellId.m_nCol >= m_nFixedCols && bInTextArea )
            OnEditCell( cellId.m_lRow, cellId.m_nCol, pointClickedRel, VK_LBUTTON );
      }

      if ( IsValid( cellId ) )
      {
         ZGridCellBase *pCell = GetCell( cellId.m_lRow, cellId.m_nCol );
         if ( pCell )
            pCell->OnDblClick( pointClickedRel );

         SendMessageToParent( cellId.m_lRow, cellId.m_nCol, NM_DBLCLK );
      }
   }

   CWnd::OnLButtonDblClk( uFlags, pt );
// TracePoint( "ZGrid::OnLButtonDblClk end: ", pt );
}

void
ZGrid::PostOnClickDown( ZGridCellBase *pCell, CPoint ptClicked )
{
   WPARAM wParam = MAKELONG( ptClicked.x, ptClicked.y );
   PostMessage( zZEIDON_CTRL_MSG + 1, wParam, (LPARAM) pCell );
}

LRESULT
ZGrid::OnPostedClickDown( WPARAM wParam, LPARAM lParam )
{
   ZGridCellBase *pCell = (ZGridCell *) lParam;
   CPoint ptClicked;
   ptClicked.x = LOWORD( wParam );
   ptClicked.y = HIWORD( wParam );
// TracePoint( "ZGrid::OnPostedClickDown: ", ptClicked );
   pCell->OnClickDown( ptClicked );
   return( 0 );
}

void
ZGrid::OnLButtonDown( UINT uFlags, CPoint pt )
{
#ifdef GRIDCONTROL_USE_TITLETIPS
   // EFW - Bug Fix
   m_TitleTip.Hide( );  // hide any titletips
#endif

// CWnd::OnLButtonDown( uFlags, pt );

   m_bSelect = FALSE;

   m_cellHold = m_cellIdCurrent;
   SetFocus( );

   m_bLMouseButtonDown = TRUE;
   m_ptLeftClickDown = pt;
   m_cellLeftClickDown = GetCellFromPt( pt );

// if ( zstrcmp( "SSQueryAttributes", *m_pzsTag ) )
// {
//    TracePoint( "ZGrid::OnLButtonDown: ", pt );
//    TraceLine( "ZGrid::OnLButtonDown Row %d   Col %d", m_cellLeftClickDown.m_lRow, m_cellLeftClickDown.m_nCol );
// }

   if ( m_cellLeftClickDown.m_lRow < GetFixedRowCount( ) &&
        m_cellLeftClickDown.m_nCol < GetFixedColumnCount( ) )
   {
      if ( m_chMultiSelect == 'Y' )
      {
         m_bSelectAll = TRUE;
         SelectAllCells( );
      }

      return;
   }

   m_bSelectAll = FALSE;
   if ( IsValid( m_cellLeftClickDown ) == FALSE )
      return;

#ifdef DEBUG_ALL
   TraceLine( "ZGrid::OnLButtonDown LeftClickDown CurrentCell: (%d, %d)",
              m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
#endif

   // If the SHIFT key is not down, then the start of the selection area
   // should be the cell just clicked. Otherwise, keep the previous
   // selection-start-cell so the user can add to their previous cell
   // selections in an intuitive way. If no selection-start-cell has been
   // specified, then set its value here and now.
   if ( (uFlags & MK_SHIFT) != MK_SHIFT )
      m_cellSelectionStart = m_cellLeftClickDown;
   else
   {
      if ( IsValid( m_cellSelectionStart ) == FALSE )
         m_cellSelectionStart = m_cellIdCurrent;
   }

   // Only end the edit if changing row or column with the click (dks 2003.05.09).
   if ( m_cellLeftClickDown.m_lRow != m_cellIdCurrent.m_lRow ||
        m_cellLeftClickDown.m_nCol != m_cellIdCurrent.m_nCol )
   {
      EndEditing( );      // destroy any InPlaceEdit's
   }

   // Tell the cell about the click.
   ZGridCellBase *pCell = GetCell( m_cellLeftClickDown.m_lRow,
                                   m_cellLeftClickDown.m_nCol );
   if ( pCell )
   {
      pCell->OnClickDown( GetPointClicked( m_cellLeftClickDown.m_lRow,
                                           m_cellLeftClickDown.m_nCol, pt ) );
      pCell = GetCell( m_cellLeftClickDown.m_lRow,
                       m_cellLeftClickDown.m_nCol );
   }

   // Clicked in the text area? ... only then will cell selection work.
   zBOOL bInTextArea = FALSE;
   if ( pCell )
   {
      CRect rectCell;
      if ( GetCellRect( m_cellLeftClickDown.m_lRow,
                        m_cellLeftClickDown.m_nCol, rectCell ) &&
           pCell->GetTextRect( rectCell, TRUE ) )
      {
         bInTextArea = rectCell.PtInRect( pt );
      }
   }

#if 1  // for Jeff 2006.10.16

// TraceLine( "ZGrid::OnLButtonDown LeftClickDown Cell: (%d, %d) Fixed rows: %d   Fixed cols: %d  CurrentCell: (%d, %d)",
//            m_cellLeftClickDown.m_lRow, m_cellLeftClickDown.m_nCol,
//            GetFixedRowCount( ), GetFixedColumnCount( ), m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );

   if ( m_cellSelectionStart == m_cellLeftClickDown &&
        GetFixedColumnCount( ) > 0 &&
        m_cellLeftClickDown.m_lRow >= GetFixedRowCount( ) &&
        m_cellLeftClickDown.m_nCol >= GetFixedColumnCount( ) &&
        (uFlags & MK_CONTROL) == 0 && bInTextArea &&
        IsCellEditable( m_cellLeftClickDown ) &&
        (uFlags & MK_CONTROL) == 0 && bInTextArea )
   {
      m_bSelect = TRUE;
   }
#endif

   // If the user clicks on the current cell, then prepare to edit it.
   // (If the user moves the mouse, then dragging occurs).
   if ( m_cellLeftClickDown == m_cellIdCurrent &&
        (uFlags & MK_CONTROL) == 0 && bInTextArea &&
        IsCellEditable( m_cellLeftClickDown ) )
   {
      m_MouseMode = MOUSE_PREPARE_EDIT;
      return;
   }

   // If the user clicks on a selected cell, then prepare to drag it.
   // (If the user moves the mouse, then dragging occurs).
   else
   if ( IsCellSelected( m_cellLeftClickDown ) )
   {
   // TraceLine( "ZGrid::OnLButtonDown IsCellSelected: (%d, %d)",
   //            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      if ( m_cellLeftClickDown.m_lRow >= GetFixedRowCount( ) &&
           m_cellLeftClickDown.m_nCol >= GetFixedColumnCount( ) )
      {
         SetFocusCell( m_cellLeftClickDown );
      }
      else
         SetFocusCell( -1, -1 );

      // If control is pressed then unselect the cell or row (depending on
      // the list mode).
      if ( uFlags & MK_CONTROL )
      {
      // SetFocusCell( m_cellLeftClickDown );
         if ( GetListMode( ) )
            SelectRows( m_cellLeftClickDown, TRUE, FALSE );
         else
            SelectCells( m_cellLeftClickDown, TRUE, FALSE );

         return;
      }
#ifndef GRIDCONTROL_NO_DRAGDROP
      else
      if ( m_bAllowDragAndDrop )
         m_MouseMode = MOUSE_PREPARE_DRAG;
#endif
   }
   else
   if ( m_MouseMode != MOUSE_OVER_COL_DIVIDE &&
        m_MouseMode != MOUSE_OVER_ROW_DIVIDE )
   {
   // TraceLine( "ZGrid::OnLButtonDown NOT MouseOverDivide: (%d, %d)",
   //            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      if ( m_cellLeftClickDown.m_lRow >= GetFixedRowCount( ) ) // &&
   //x     m_cellLeftClickDown.m_nCol >= GetFixedColumnCount( ) )
      {
         SetFocusCell( m_cellLeftClickDown );
      }
   //xelse
   //x   SetFocusCell( -1, -1 );

      // Don't set focus on any cell if the user clicked on a fixed
      // cell - David Pritchard.
      // if ( GetRowCount( ) > GetFixedRowCount( ) &&
      //      GetColumnCount( ) > GetFixedColumnCount( ) )
      // {
      //    SetFocusCell( max( m_cellLeftClickDown.m_lRow, m_nFixedRows ),
      //                  max( m_cellLeftClickDown.m_nCol, m_nFixedCols ) );
      // }
   }

   SetCapture( );

   if ( m_MouseMode == MOUSE_NOTHING )
   {
   // TraceLine( "ZGrid::OnLButtonDown MOUSE_NOTHING: (%d, %d)",
   //            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      if ( m_bAllowColumnResize && MouseOverColumnResizeArea( pt ) )
      {
         if ( m_MouseMode != MOUSE_OVER_COL_DIVIDE )
         {
#ifndef _WIN32_WCE_NO_CURSOR
            SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZEWE ) );
#endif
            m_MouseMode = MOUSE_OVER_COL_DIVIDE;
         }
      }
      else
      if ( m_bAllowRowResize && MouseOverRowResizeArea( pt ) )
      {
         if ( m_MouseMode != MOUSE_OVER_ROW_DIVIDE )
         {
#ifndef _WIN32_WCE_NO_CURSOR
            SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZENS ) );
#endif
            m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
         }
      }
   // else
   // if ( m_MouseMode != MOUSE_NOTHING )
   // {
   //    SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_ARROW ) );
   //    m_MouseMode = MOUSE_NOTHING;
   // }
   }

   if ( m_MouseMode == MOUSE_OVER_COL_DIVIDE ) // sizing column
   {
      m_MouseMode = MOUSE_SIZING_COL;

      // Kludge for if we are over the last column...
      int nColCnt = GetColumnCount( );
      if ( GetColumnWidth( nColCnt - 1 ) < m_nResizeCaptureRange )
      {
         CRect rectVis;
         GetVisibleNonFixedCellRange( rectVis );
         if ( abs( pt.x - rectVis.right ) < m_nResizeCaptureRange )
            m_cellLeftClickDown.m_nCol = nColCnt - 1;
      }

      CPoint ptStart;
      if ( GetCellOrigin( 0, m_cellLeftClickDown.m_nCol, ptStart ) == 0 )
         return;

      if ( m_bHiddenColUnhide == FALSE )
      {
         // Ignore columns that are hidden and look left towards first
         // visible column.
         zBOOL bLookForVisible = TRUE;
         zBOOL bIsCellRightBorder = pt.x - ptStart.x >= m_nResizeCaptureRange;

         if ( bIsCellRightBorder &&
              m_cellLeftClickDown.m_nCol + 1 >= nColCnt )
         {
            // Clicked on last column's right border.

            // If last column is visible, don't do anything.
            if ( m_cellLeftClickDown.m_nCol >= 0 )
               bLookForVisible = FALSE;
         }

         if ( bLookForVisible )
         {
            // Clicked on column divider other than last right border.
            zBOOL bFoundVisible = FALSE;
            int nOffset = 1;

            if ( bIsCellRightBorder )
               nOffset = 0;

            while ( m_cellLeftClickDown.m_nCol - nOffset >= 0 )
            {
               if ( GetColumnWidth( m_cellLeftClickDown.m_nCol - nOffset ) > 0 )
               {
                  bFoundVisible = TRUE;
                  break;
               }

               m_cellLeftClickDown.m_nCol--;
            }

            if ( bFoundVisible == FALSE )
               return;
         }
      }

      CRect rect;
      GetClientRect( rect );
      CRect rectInverted( pt.x, rect.top, pt.x + 2, rect.bottom );

      CDC *pDC = GetDC( );
      if ( pDC )
      {
         pDC->InvertRect( &rectInverted );
         ReleaseDC( pDC );
      }

      // If we clicked to the right of the column divide, then reset the
      // click-down cell as the cell to the left of the column divide -
      // UNLESS we clicked on the last column and the last column is
      // teensy (kludge fix).
      if ( pt.x - ptStart.x < m_nResizeCaptureRange )
      {
         if ( (m_nExtraCol &&
               (m_cellLeftClickDown.m_nCol < m_nExtraCol &&
                GetColumnWidth( m_nExtraCol - 1 ) >= m_nResizeCaptureRange) ||
                (m_cellLeftClickDown.m_nCol >= m_nExtraCol &&
                GetColumnWidth( nColCnt - 1 ) >= m_nResizeCaptureRange)) ||
              (m_nExtraCol == 0 &&
               (m_cellLeftClickDown.m_nCol < nColCnt - 1 ||
                GetColumnWidth( nColCnt - 1 ) >= m_nResizeCaptureRange)) )
         {
            if ( GetCellOrigin( 0, --m_cellLeftClickDown.m_nCol, ptStart ) == 0 )
               return;
         }
      }

      // Allow a cell resize width no greater than that which can be viewed
      // within the grid itself.
      int nMaxCellWidth = rect.Width( ) - GetFixedColumnWidth( );
      rect.left = ptStart.x + 1;
      rect.right = rect.left + nMaxCellWidth;

      ClientToScreen( rect );
#ifndef _WIN32_WCE_NO_CURSOR
      ClipCursor( rect );
#endif
   }
   else
   if ( m_MouseMode == MOUSE_OVER_ROW_DIVIDE ) // sizing row
   {
      m_MouseMode = MOUSE_SIZING_ROW;

      // Kludge for when we are over the last column ...
      if ( GetRowHeight( GetRowCount( ) - 1 ) < m_nResizeCaptureRange )  //???
      {
         CRect rectVis;
         GetVisibleNonFixedCellRange( rectVis );
         if ( abs( pt.y - rectVis.bottom ) < m_nResizeCaptureRange )
            m_cellLeftClickDown.m_lRow = GetRowCount( ) - 1;
      }

      CPoint ptStart;
      if ( GetCellOrigin( m_cellLeftClickDown, ptStart ) == FALSE )
         return;

      long  lRowCnt = GetRowCount( );
      if ( m_bHiddenRowUnhide == FALSE )
      {
         // Ignore rows that are hidden and look up towards first visible row.
         zBOOL bLookForVisible = TRUE;
         zBOOL bIsCellBottomBorder = pt.y - ptStart.y >= m_nResizeCaptureRange;

         if ( bIsCellBottomBorder &&
              m_cellLeftClickDown.m_lRow + 1 >= lRowCnt )
         {
            // Clicked on last row's bottom border.

            // If last row is visible, don't do anything.
            if ( m_cellLeftClickDown.m_lRow >= 0 )
               bLookForVisible = FALSE;
         }

         if ( bLookForVisible )
         {
            // Clicked on row divider other than last bottom border.
            zBOOL bFoundVisible = FALSE;
            int nOffset = 1;

            if ( bIsCellBottomBorder )
               nOffset = 0;

            while ( m_cellLeftClickDown.m_lRow - nOffset >= 0 )
            {
               if ( GetRowHeight( m_cellLeftClickDown.m_lRow - nOffset ) > 0 )
               {
                  bFoundVisible = TRUE;
                  break;
               }

               m_cellLeftClickDown.m_lRow--;
            }

            if ( bFoundVisible == FALSE )
               return;
         }
      }

      CRect rect;
      GetClientRect( rect );
      CRect rectInverted( rect.left, pt.y, rect.right, pt.y + 2 );

      CDC *pDC = GetDC( );
      if ( pDC )
      {
         pDC->InvertRect( &rectInverted );
         ReleaseDC( pDC );
      }

      // If we clicked below the row divide, then reset the click-down cell
      // as the cell above the row divide - UNLESS we clicked on the last row
      // and the last row is teensy (kludge fix).
      if ( pt.y - ptStart.y < m_nResizeCaptureRange )   // clicked below border
      {
         if ( m_cellLeftClickDown.m_lRow < lRowCnt - 1 ||
              GetRowHeight( lRowCnt - 1 ) >= m_nResizeCaptureRange )  //???
         {
            if ( GetCellOrigin( --m_cellLeftClickDown.m_lRow, 0, ptStart ) == 0 )
               return;
         }
      }

      int nMaxCellHeight = rect.Height( ) - GetFixedRowHeight( );
      rect.top = ptStart.y + 1;
      rect.bottom = rect.top + nMaxCellHeight;

      ClientToScreen( rect );

#ifndef _WIN32_WCE_NO_CURSOR
      ClipCursor( rect );
#endif
   }
   else
#ifndef GRIDCONTROL_NO_DRAGDROP
   if ( m_MouseMode != MOUSE_PREPARE_DRAG ) // not sizing or editing - selecting
#endif
   {
   // TraceLine( "ZGrid::OnLButtonDown NOT MOUSE_PREPARE_DRAG: (%d, %d)",
   //            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      // We need to reset the current cell for SELCHANGING process.  Below, we
      // will reset the current cell to the new cell.
      SetFocusCell( m_cellHold );
      if ( (m_cellLeftClickDown.m_lRow == m_cellHold.m_lRow &&
            m_cellLeftClickDown.m_nCol == m_cellHold.m_nCol) ||
           m_cellLeftClickDown.m_lRow < m_nFixedRows ||
           m_cellLeftClickDown.m_nCol < m_nFixedCols ||
           SendMessageToParent( m_cellLeftClickDown.m_lRow,
                                m_cellLeftClickDown.m_nCol,
                                GVN_SELCHANGING ) )
      {
         // Don't let cell change!
         if ( m_cellLeftClickDown.m_lRow >= m_nFixedRows &&
              m_cellLeftClickDown.m_nCol >= m_nFixedCols )
         {
            return;
         }
      }

      SetFocusCell( m_cellLeftClickDown );

      // If Ctrl pressed, save the current cell selection. This will get added
      // to the new cell selection at the end of the cell selection process.
      m_PrevSelectedCellMap.RemoveAll( );
      if ( (uFlags & MK_CONTROL) && m_chMultiSelect == 'Y' )
      {
         for ( POSITION pos = m_SelectedCellMap.GetStartPosition( ); pos; )
         {
            LARGE_INTEGER key;
            ZCellId cellId;
            m_SelectedCellMap.GetNextAssoc( pos, key, (ZCellId &) cellId );
            m_PrevSelectedCellMap.SetAt( key, cellId );
         }
      }

      if ( m_cellLeftClickDown.m_lRow < GetFixedRowCount( ) )
         OnFixedRowClick( m_cellLeftClickDown );
      else
      if ( m_cellLeftClickDown.m_nCol < GetFixedColumnCount( ) )
         OnFixedColumnClick( m_cellLeftClickDown );
      else
      {
      // m_MouseMode = m_bListMode ? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
         m_MouseMode = MOUSE_SELECT_ROW;  // dks???
      // TraceLine( "ZGrid::OnLButtonDown MouseMode: (%d, %d) %d",
      //            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol, m_MouseMode );
         OnSelecting( m_cellLeftClickDown );
      // TraceLine( "ZGrid::OnLButtonDown CurrentCell: (%d, %d)",
      //            m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );

         m_nTimerID = SetTimer( WM_LBUTTONDOWN, m_nTimerInterval, 0 );
      }
   }

   m_ptLastMouse = pt;
// TracePoint( "ZGrid::OnLButtonDown LastMouse: ", pt );
}

void
ZGrid::OnLButtonUp( UINT uFlags, CPoint pt )
{
// TracePoint( "ZGrid::OnLButtonUp: ", pt );

   CWnd::OnLButtonUp( uFlags, pt );

   m_bLMouseButtonDown = FALSE;

#ifndef _WIN32_WCE_NO_CURSOR
   ClipCursor( 0 );
#endif

   if ( GetCapture( )->GetSafeHwnd( ) == GetSafeHwnd( ) )
   {
      ReleaseCapture( );
      KillTimer( m_nTimerID );
      m_nTimerID = 0;
   }

   CPoint pointClickedRel;
   pointClickedRel = GetPointClicked( m_cellIdCurrent.m_lRow,
                                      m_cellIdCurrent.m_nCol, pt );

   // m_MouseMode == MOUSE_PREPARE_EDIT only if user clicked down on current cell
   // and then didn't move mouse before clicking up (releasing button).
   if ( m_MouseMode == MOUSE_PREPARE_EDIT || m_bSelect )
   {
      SendMessageToParent( m_cellIdCurrent.m_lRow,
                           m_cellIdCurrent.m_nCol, GVN_SELCHANGED );
      OnEditCell( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol,
                  pointClickedRel, VK_LBUTTON );
   }
#ifndef GRIDCONTROL_NO_DRAGDROP
   // m_MouseMode == MOUSE_PREPARE_DRAG only if user clicked down on a selected cell
   // and then didn't move mouse before clicking up (releasing button)
   else
   if ( m_MouseMode == MOUSE_PREPARE_DRAG )
   {
      ZGridCellBase *pCell = GetCell( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
      if ( pCell )
      {
         pCell->OnClick( GetPointClicked( m_cellIdCurrent.m_lRow,
                                          m_cellIdCurrent.m_nCol, pt ) );
      }

      if ( m_cellLeftClickDown.m_lRow != m_cellHold.m_lRow )
      {
         SendMessageToParent( m_cellLeftClickDown.m_lRow,
                              m_cellLeftClickDown.m_nCol, NM_CLICK );
      }

      if ( m_cellLeftClickDown.m_lRow != m_cellHold.m_lRow ||
           m_cellLeftClickDown.m_nCol != m_cellHold.m_nCol )
      {
         if ( SendMessageToParent( m_cellLeftClickDown.m_lRow,
                                   m_cellLeftClickDown.m_nCol,
                                   GVN_SELCHANGING ) )
         {
            SetFocusCell( m_cellHold );  // don't let cell change!
         }
         else
         {
            ResetSelectedRange( );
            SendMessageToParent( m_cellIdCurrent.m_lRow,
                                 m_cellIdCurrent.m_nCol, GVN_SELCHANGED );
         }
      }
   }
#endif
   else
   if ( m_MouseMode == MOUSE_SIZING_COL )
   {
      CRect rect;
      GetClientRect( rect );
      CRect rectInverted( m_ptLastMouse.x, rect.top,
                          m_ptLastMouse.x + 2, rect.bottom );

      CDC *pDC = GetDC( );
      if ( pDC )
      {
         pDC->InvertRect( &rectInverted );
         ReleaseDC( pDC );
      }

      if ( m_ptLeftClickDown != pt && (pt.x || pt.y) ) // 0 pt fix by email1@bierling.net
      {
         CPoint ptStart;
         if ( GetCellOrigin( m_cellLeftClickDown, ptStart ) == FALSE )
            return;

         int nColWidth = max( pt.x - ptStart.x, m_bAllowColHide ? 0 : 1 );

         SetColumnWidth( m_cellLeftClickDown.m_nCol, nColWidth );
         ResetScrollBars( );
         Invalidate( );
      }
   }
   else
   if ( m_MouseMode == MOUSE_SIZING_ROW )
   {
      CRect rect;
      GetClientRect( rect );
      CRect rectInverted( rect.left, m_ptLastMouse.y,
                          rect.right, m_ptLastMouse.y + 2 );

      CDC *pDC = GetDC( );
      if ( pDC )
      {
         pDC->InvertRect( &rectInverted );
         ReleaseDC( pDC );
      }

      if ( m_ptLeftClickDown != pt && (pt.x || pt.y) ) // 0 pt fix by email1@bierling.net
      {
         CPoint ptStart;
         if ( GetCellOrigin( m_cellLeftClickDown, ptStart ) == 0 )
            return;

         int nCurrRowHeight = GetRowHeight( m_cellLeftClickDown.m_lRow );
         int nRowHeight = pt.y - ptStart.y;

         if ( m_nExtraCol )
         {
            int nDiff = nRowHeight - (nCurrRowHeight * (m_nExtraRowsPerItem + 1)) + 1;
            nRowHeight += nDiff;
            if ( nDiff < 0 )
               nRowHeight += 2;

            nRowHeight /= (m_nExtraRowsPerItem + 1);
         }

         nRowHeight = max( nRowHeight, m_bAllowRowHide ? 0 : 1 );
         SetRowHeight( m_cellLeftClickDown.m_lRow, nRowHeight );
         ResetScrollBars( );
         Invalidate( );
      }
   }
   else
   {
   // TracePoint( "Grid got here at point:", pt );
      if ( m_cellHold.m_lRow != m_cellIdCurrent.m_lRow ||
           m_cellHold.m_nCol != m_cellIdCurrent.m_nCol )
      {
         SendMessageToParent( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol, GVN_SELCHANGED );
      }

      ZGridCellBase *pCell = GetCell( m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
   // TraceLineX( "Grid cell: ", (zLONG) pCell );
      if ( pCell )
      {
         pCell->OnClick( GetPointClicked( m_cellIdCurrent.m_lRow,
                                          m_cellIdCurrent.m_nCol, pt ) );
      }

      if ( m_cellLeftClickDown.m_lRow != m_cellHold.m_lRow &&
           m_cellLeftClickDown.m_lRow >= m_nFixedRows )
      {
         SendMessageToParent( m_cellLeftClickDown.m_lRow,
                              m_cellLeftClickDown.m_nCol, NM_CLICK );
      }
   }

   m_MouseMode = MOUSE_NOTHING;

#ifndef _WIN32_WCE_NO_CURSOR
   SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_ARROW ) );
#endif

   if ( IsValid( m_cellLeftClickDown ) == FALSE )
      return;

   CWnd *pOwner = GetOwner( );
   if ( pOwner && mIs_hWnd( pOwner->m_hWnd ) )
   {
      pOwner->PostMessage( WM_COMMAND, MAKELONG( GetDlgCtrlID( ), BN_CLICKED ),
                           (LPARAM)  GetSafeHwnd( ) );
   }
}

#ifndef _WIN32_WCE
void
ZGrid::OnRButtonDown( UINT uFlags, CPoint pt )
{
   m_bRMouseButtonDown = TRUE;
   CWnd::OnRButtonDown( uFlags, pt );

#if 1  // add SELCHANGED event on RButtonDown (dks 2007.03.30)

   ZCellId cellFocus;
   cellFocus = GetCellFromPt( pt );
   TraceLine( "ZGrid::OnRButtonDown Row %d   Col %d", cellFocus.m_lRow, cellFocus.m_nCol );

   m_bSelectAll = FALSE;
   if ( IsValid( cellFocus ) == FALSE )
      return;

#ifdef DEBUG_ALL
   TraceLine( "ZGrid::OnLButtonDown RightClickDown CurrentCell: (%d, %d)",
              m_cellIdCurrent.m_lRow, m_cellIdCurrent.m_nCol );
#endif

   if ( cellFocus.m_lRow != m_cellHold.m_lRow || cellFocus.m_nCol != m_cellHold.m_nCol )
   {
      if ( SendMessageToParent( cellFocus.m_lRow,
                                cellFocus.m_nCol,
                                GVN_SELCHANGING ) )
      {
         SetFocusCell( m_cellHold );  // don't let cell change!
      }
      else
      {
         // Only end the edit if changing row or column with the click (dks 2003.05.09).
         if ( cellFocus.m_lRow != m_cellIdCurrent.m_lRow ||
              cellFocus.m_nCol != m_cellIdCurrent.m_nCol )
         {
            EndEditing( );      // destroy any InPlaceEdit's
            ResetSelectedRange( );
            SendMessageToParent( cellFocus.m_lRow, cellFocus.m_nCol, GVN_SELCHANGED );
         }
      }
   }

   m_cellHold = cellFocus;
   m_cellSelectionStart = cellFocus;
   SetFocusCell( cellFocus );
// m_cellIdCurrent = cellFocus;

#endif



#ifdef GRIDCONTROL_USE_TITLETIPS
// TRACE0( "Hiding TitleTip\n" );
   m_TitleTip.Hide( );  // hide any titletips
#endif

   CPoint ptScreen( pt );
   ClientToScreen( &ptScreen );
   zULONG ulPoint = zMAKELONG( ptScreen.x, ptScreen.y );

   ProcessImmediateEvent( this, zGRDN_RBUTTONDOWN, (zPVOID) ulPoint );
}

// EFW - Added to forward right click to parent so that a context
// menu can be shown without deriving a new grid class.
void
ZGrid::OnRButtonUp( UINT uFlags, CPoint pt )
{
   CWnd::OnRButtonUp( uFlags, pt );

   m_bRMouseButtonDown = FALSE;

   ZCellId cellFocus;
   cellFocus = GetCellFromPt( pt );

   EndEditing( );      // destroy any InPlaceEdit's

   // If not a valid cell, pass -1 for row and column.
   if ( IsValid( cellFocus ) == FALSE )
      SendMessageToParent( -1, -1, NM_RCLICK );
   else
   {
      SetFocusCell( -1, -1 );
      SetFocusCell( max( cellFocus.m_lRow, m_nFixedRows ),
                    max( cellFocus.m_nCol, m_nFixedCols ) );

      // tell the cell about it
      ZGridCellBase *pCell = GetCell( cellFocus.m_lRow, cellFocus.m_nCol );
      if ( pCell )
      {
         pCell->OnRClick( GetPointClicked( cellFocus.m_lRow,
                                           cellFocus.m_nCol, pt ) );
      }

      SendMessageToParent( cellFocus.m_lRow, cellFocus.m_nCol, NM_RCLICK );
   }
}
#endif

#if !defined( _WIN32_WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )
/////////////////////////////////////////////////////////////////////////////
// ZGrid printing

// EFW - New print margin support functions
void
ZGrid::SetPrintMarginInfo( int nHeaderHeight, int nFooterHeight,
                           int nLeftMargin, int nRightMargin,
                           int nTopMargin, int nBottomMargin, int nGap )
{
   // If any parameter is -1, keep the existing setting
   if ( nHeaderHeight > -1 )
      m_nHeaderHeight = nHeaderHeight;

   if ( nFooterHeight > -1 )
      m_nFooterHeight = nFooterHeight;

   if ( nLeftMargin > -1 )
      m_nLeftMargin = nLeftMargin;

   if ( nRightMargin > -1 )
      m_nRightMargin = nRightMargin;

   if ( nTopMargin > -1 )
      m_nTopMargin = nTopMargin;

   if ( nBottomMargin > -1 )
      m_nBottomMargin = nBottomMargin;

   if ( nGap > -1 )
      m_nGap = nGap;
}

void
ZGrid::GetPrintMarginInfo( int &nHeaderHeight, int &nFooterHeight,
                           int &nLeftMargin, int &nRightMargin,
                           int &nTopMargin, int &nBottomMargin, int &nGap )
{
   nHeaderHeight = m_nHeaderHeight;
   nFooterHeight = m_nFooterHeight;
   nLeftMargin = m_nLeftMargin;
   nRightMargin = m_nRightMargin;
   nTopMargin = m_nTopMargin;
   nBottomMargin = m_nBottomMargin;
   nGap = m_nGap;
}

void
ZGrid::Print( CPrintDialog *pPrntDialog /*=0*/ )
{
   CDC dc;

   if ( pPrntDialog == 0 )
   {
      CPrintDialog printDlg( FALSE );
      if ( printDlg.DoModal( ) != IDOK )    // get printer settings from user
         return;

      dc.Attach( printDlg.GetPrinterDC( ) );      // attach a printer DC
   }
   else
      dc.Attach( pPrntDialog->GetPrinterDC( ) );  // attach a printer DC

   dc.m_bPrinting = TRUE;

   CString csTitle;
   csTitle.LoadString( AFX_IDS_APP_TITLE );

   if ( csTitle.IsEmpty( ) )
   {
      CWnd *pParentWnd = GetParent( );
      while ( pParentWnd )
      {
         pParentWnd->GetWindowText( csTitle );
         if ( csTitle.GetLength( ) )  // can happen if it is a CView,
            break;                    // CChildFrm has the title

         pParentWnd = pParentWnd->GetParent( );
      }
   }

   DOCINFO di;                             // initialize print doc details
   memset( &di, 0, sizeof ( DOCINFO ) );
   di.cbSize = sizeof ( DOCINFO );
   di.lpszDocName = csTitle;

   zBOOL bPrintingOK = dc.StartDoc( &di ); // begin a new print job

   CPrintInfo Info;
   Info.m_rectDraw.SetRect( 0, 0, dc.GetDeviceCaps( HORZRES ),
                            dc.GetDeviceCaps( VERTRES ) );

   OnBeginPrinting( &dc, &Info );          // initialize printing
   for ( UINT page = Info.GetMinPage( );
         page <= Info.GetMaxPage( ) && bPrintingOK;
         page++ )
   {
      dc.StartPage( );                     // begin new page
      Info.m_nCurPage = page;
      OnPrint( &dc, &Info );               // print page
      bPrintingOK = (dc.EndPage( ) > 0);   // end page
   }

   OnEndPrinting( &dc, &Info );            // clean up after printing

   if ( bPrintingOK )
      dc.EndDoc( );                        // end a print job
   else
      dc.AbortDoc( );                      // abort job.

   dc.Detach( );                           // detach the printer DC
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid printing overridables - for Doc/View print/print preview framework

// EFW - Various changes in the next few functions to support the
// new print margins and a few other adjustments.
void
ZGrid::OnBeginPrinting( CDC *pDC, CPrintInfo *pInfo )
{
   // OnBeginPrinting( ) is called after the user has committed to
   // printing by OK'ing the Print dialog, and after the framework
   // has created a CDC object for the printer or the preview view.

   // This is the right opportunity to set up the page range.
   // Given the CDC object, we can determine how many rows will
   // fit on a page, so we can in turn determine how many printed
   // pages represent the entire document.

   ASSERT( pDC && pInfo );
   if ( pDC == 0 || pInfo == 0 )
      return;

   // Get a DC for the current window (a screen DC for print previewing).
   CDC *pCurrentDC = GetDC( );   // will have dimensions of the client area
   if ( pCurrentDC == 0 )
      return;

   CSize PaperPixelsPerInch( pDC->GetDeviceCaps( LOGPIXELSX ),
                             pDC->GetDeviceCaps( LOGPIXELSY ) );
   CSize ScreenPixelsPerInch( pCurrentDC->GetDeviceCaps( LOGPIXELSX ),
                              pCurrentDC->GetDeviceCaps( LOGPIXELSY ) );

   // Create the printer font.
   int nFontSize = -10;
   CString csFontName = "Arial";
   m_PrinterFont.CreateFont( nFontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                             DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
                             CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, csFontName );

   CFont *pOldFont = pDC->SelectObject( &m_PrinterFont );

   // Get the average character width (in GridCtrl units) and hence the
   // margins.
   m_CharSize = pDC->GetTextExtent( _T( "abcdefghijklmnopqrstuvwxyz"
                                        "ABCDEFGHIJKLMNOPQRSATUVWXYZ" ), 52 );
   m_CharSize.cx /= 52;
   int nMargins = (m_nLeftMargin + m_nRightMargin) * m_CharSize.cx;

   // Get the page sizes (physical and logical).
   m_PaperSize = CSize( pDC->GetDeviceCaps( HORZRES ),
                        pDC->GetDeviceCaps( VERTRES ) );

   if ( m_bWysiwygPrinting )
   {
      m_LogicalPageSize.cx =
       ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx * 3 / 4;
      m_LogicalPageSize.cy =
       ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy * 3 / 4;
   }
   else
   {
      m_PaperSize = CSize( pDC->GetDeviceCaps( HORZRES ),
                           pDC->GetDeviceCaps( VERTRES ) );

      m_LogicalPageSize.cx = GetVirtualWidth( ) + nMargins;
#ifdef _WIN32_WCE
      m_LogicalPageSize.cy =
                    (m_LogicalPageSize.cx * m_PaperSize.cy) / m_PaperSize.cx;
#else
      m_LogicalPageSize.cy =
              MulDiv( m_LogicalPageSize.cx, m_PaperSize.cy, m_PaperSize.cx );
#endif
   }

   m_nPageHeight = m_LogicalPageSize.cy - GetFixedRowHeight( ) -
            (m_nHeaderHeight + m_nFooterHeight + 2 * m_nGap) * m_CharSize.cy;

   // Get the number of pages. Assumes no row is bigger than the page size.
   long lRowCnt = GetRowCount( );
   int  nFixedRowCnt = GetFixedRowCount( );
   int  nFixedColCnt = GetFixedColumnCount( );
   int  nTotalRowHeight = 0;
   m_nNbrPages = 1;
   for ( long lRow = nFixedRowCnt; lRow < lRowCnt; lRow++ )
   {
      nTotalRowHeight += GetRowHeight( lRow );  //???
      if ( nTotalRowHeight > m_nPageHeight )
      {
         m_nNbrPages++;
         nTotalRowHeight = GetRowHeight( lRow );  //???
      }
   }

   // Now, figure out how many additional pages must print out if rows ARE
   // bigger than page size.
   int nColumnOffset = 0;
   int k1;
   for ( k1 = 0; k1 < nFixedColCnt; k1++ )
   {
      nColumnOffset += GetColumnWidth( k1 );
   }

   m_nPageWidth = m_LogicalPageSize.cx - nColumnOffset - nMargins;
   m_nPageMultiplier = 1;

   if ( m_bWysiwygPrinting )
   {
      int nTotalRowWidth = 0;
      int nColCnt = GetColumnCount( );

      for ( k1 = nFixedColCnt; k1 < nColCnt; k1++ )
      {
         nTotalRowWidth += GetColumnWidth( k1 );
         if ( nTotalRowWidth > m_nPageWidth )
         {
            m_nPageMultiplier++;
            nTotalRowWidth = GetColumnWidth( k1 );
         }
      }

      m_nNbrPages *= m_nPageMultiplier;
   }

   // Set up the print info.
   pInfo->SetMaxPage( m_nNbrPages );
   pInfo->m_nCurPage = 1;                  // start printing at page# 1

   ReleaseDC( pCurrentDC );
   pDC->SelectObject( pOldFont );
}

void
ZGrid::OnPrint( CDC *pDC, CPrintInfo *pInfo )
{
   if ( pDC == 0 || pInfo == 0 )
      return;

   // CRect recPaget( pInfo->m_rectDraw );
   CFont *pOldFont = pDC->SelectObject( &m_PrinterFont );

   // Set the page map mode to use GridCtrl units, and setup margin.
   pDC->SetMapMode( MM_ANISOTROPIC );
   pDC->SetWindowExt( m_LogicalPageSize );
   pDC->SetViewportExt( m_PaperSize );
   pDC->SetWindowOrg( -m_nLeftMargin * m_CharSize.cx, 0 );

   // Header
   pInfo->m_rectDraw.top = 0;
   pInfo->m_rectDraw.left = 0;
   pInfo->m_rectDraw.right = m_LogicalPageSize.cx -
                            (m_nLeftMargin + m_nRightMargin) * m_CharSize.cx;
   pInfo->m_rectDraw.bottom = m_nHeaderHeight * m_CharSize.cy;
   PrintHeader( pDC, pInfo );
   pDC->OffsetWindowOrg( 0, -m_nHeaderHeight * m_CharSize.cy );

   // Gap between header and column headings
   pDC->OffsetWindowOrg( 0, -m_nGap * m_CharSize.cy );

   pDC->OffsetWindowOrg( 0, -GetFixedRowHeight( ) );

   // We need to find out which row to start printing for this page.
   long lRowCnt = GetRowCount( );
   int  nFixedRowCnt = GetFixedRowCount( );
   int  nTotalRowHeight = 0;
   UINT uNbrPages = 1;
   m_nCurrPrintRow = nFixedRowCnt;

   // Not only the row, but we need to figure out column, too

   // Can print 4 pages, where page 1 and 2 represent the same rows but
   // with different WIDE columns.
   //
   // .......
   // .1 .2 .  If representing page 3  -->   nPageIfIgnoredWideCols = 2
   // .......                         nWideColPageOffset = 0
   // .3 .4 .  If representing page 2  -->   nPageIfIgnoredWideCols = 1
   // .......                         nWideColPageOffset = 1

   int nPageIfIgnoredWideCols = pInfo->m_nCurPage / m_nPageMultiplier;
   int nWideColPageOffset = pInfo->m_nCurPage -
                            (nPageIfIgnoredWideCols * m_nPageMultiplier);
   if ( nWideColPageOffset > 0 )
      nPageIfIgnoredWideCols++;

   if ( nWideColPageOffset == 0 )
      nWideColPageOffset = m_nPageMultiplier;

   nWideColPageOffset--;

   // Calculate current print row based on nPageIfIgnoredWideCols.
   while ( m_nCurrPrintRow < lRowCnt &&
           (int) uNbrPages < nPageIfIgnoredWideCols )
   {
      nTotalRowHeight += GetRowHeight( m_nCurrPrintRow );  //???
      if ( nTotalRowHeight > m_nPageHeight )
      {
         uNbrPages++;
         if ( (int) uNbrPages == nPageIfIgnoredWideCols )
            break;

         nTotalRowHeight = GetRowHeight( m_nCurrPrintRow );  //???
      }

      m_nCurrPrintRow++;
   }

   int nFixedColCnt = GetFixedColumnCount( );
   int nColCnt = GetColumnCount( );
   int nTotalRowWidth = 0;
   int k1, k2;

   m_nPrintColumn = nFixedColCnt;

   // Now, calculate which print column to start displaying.
   for ( k1 = 0; k1 < nWideColPageOffset; k1++ )
   {
      for ( k2 = m_nPrintColumn; k2 < nColCnt; k2++ )
      {
         nTotalRowWidth += GetColumnWidth( k2 );
         if ( nTotalRowWidth > m_nPageWidth )
         {
            m_nPrintColumn = k2;
            nTotalRowWidth = 0;
            break;
         }
      }
   }

   PrintRowButtons( pDC, pInfo );   // print row buttons on each page
   int nColumnOffset = 0;
   for ( k1 = 0; k1 < nFixedColCnt; k1++ )
   {
      nColumnOffset += GetColumnWidth( k1 );
   }

   // Print the column headings.
   pInfo->m_rectDraw.bottom = GetFixedRowHeight( );

   if ( m_nPrintColumn == nFixedColCnt )
   {
      // Have the column headings fcn draw the upper left fixed cells
      // for the very first columns, only.
      pDC->OffsetWindowOrg( 0, +GetFixedRowHeight( ) );

      m_nPageWidth += nColumnOffset;
      m_nPrintColumn = 0;
      PrintColumnHeadings( pDC, pInfo );
      m_nPageWidth -= nColumnOffset;
      m_nPrintColumn = nFixedColCnt;

      pDC->OffsetWindowOrg( -nColumnOffset, -GetFixedRowHeight( ) );
   }
   else
   {
      // Changed all of this here to match above almost exactly same.
      pDC->OffsetWindowOrg( 0, +GetFixedRowHeight( ) );

      m_nPageWidth += nColumnOffset;

      // Print from column 0 ... last column that fits on the current page.
      PrintColumnHeadings( pDC, pInfo );

      m_nPageWidth -= nColumnOffset;

      pDC->OffsetWindowOrg( -nColumnOffset, -GetFixedRowHeight( ) );
   }

   if ( m_nCurrPrintRow >= lRowCnt )
      return;

   // Draw as many rows as will fit on the printed page.
   // Clip the printed page so that there is no partially shown
   // row at the bottom of the page (the same row which will be fully
   // shown at the top of the next page).

   zBOOL bFirstPrintedRow = TRUE;
   CRect rect;
   rect.bottom = -1;
   while ( m_nCurrPrintRow < lRowCnt )
   {
      rect.top = rect.bottom + 1;
      rect.bottom = rect.top + GetRowHeight( m_nCurrPrintRow ) - 1;  //???

      if ( rect.bottom > m_nPageHeight )
         break;         // gone past end of page

      rect.right = -1;

      // Modified to allow printing of wide grids on multiple pages.
      for ( int nCol = m_nPrintColumn; nCol < nColCnt; nCol++ )
      {
         rect.left = rect.right + 1;
         rect.right = rect.left + GetColumnWidth( nCol ) - 1;

         if ( rect.right > m_nPageWidth )
            break;

         ZGridCellBase *pCell = GetCell( m_nCurrPrintRow, nCol );
         if ( pCell )
            pCell->PrintCell( pDC, m_nCurrPrintRow, nCol, rect );

         if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ )
         {
            int Overlap = (nCol == 0) ? 0 : 1;
            pDC->MoveTo( rect.left-Overlap, rect.bottom );
            pDC->LineTo( rect.right, rect.bottom );
            if ( m_nCurrPrintRow == 0 )
            {
               pDC->MoveTo( rect.left-Overlap, rect.top );
               pDC->LineTo( rect.right, rect.top );
            }
         }

         if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT )
         {
            int Overlap = (bFirstPrintedRow) ? 0:1;
            pDC->MoveTo( rect.right, rect.top-Overlap );
            pDC->LineTo( rect.right, rect.bottom );
            if ( nCol == 0 )
            {
               pDC->MoveTo( rect.left, rect.top-Overlap );
               pDC->LineTo( rect.left, rect.bottom );
            }
         }
      }

      m_nCurrPrintRow++;
      bFirstPrintedRow = FALSE;
   }

   // Footer
   pInfo->m_rectDraw.bottom = m_nFooterHeight * m_CharSize.cy;
   pDC->SetWindowOrg( -m_nLeftMargin * m_CharSize.cx,
                      -m_LogicalPageSize.cy + m_nFooterHeight * m_CharSize.cy );
   PrintFooter( pDC, pInfo );

   // SetWindowOrg back for next page.
   pDC->SetWindowOrg( 0, 0 );

   pDC->SelectObject( pOldFont );
}

// added by M.Fletcher 12/17/00
void
ZGrid::PrintFixedRowCells( int nStartColumn, int nStopColumn,
                           long& lRow, CRect& rect,
                           CDC *pDC, zBOOL& bFirst )
{
   // Print all cells from nStartColumn to nStopColumn on row.
   for ( int nCol = nStartColumn; nCol < nStopColumn; nCol++ )
   {
      rect.left = rect.right + 1;
      rect.right = rect.left + GetColumnWidth( nCol ) - 1;

      if ( rect.right > m_nPageWidth )
         break;

      ZGridCellBase *pCell = GetCell( lRow, nCol );
      if ( pCell )
         pCell->PrintCell( pDC, lRow, nCol, rect );

      if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ )
      {
         int nOverlap = (nCol == 0) ? 0 : 1;

         pDC->MoveTo( rect.left - nOverlap, rect.bottom );
         pDC->LineTo( rect.right, rect.bottom );

         if ( lRow == 0 )
         {
            pDC->MoveTo( rect.left - nOverlap, rect.top );
            pDC->LineTo( rect.right, rect.top );
         }
      }

      if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT )
      {
         int Overlap = (lRow == 0) ? 0 : 1;

         pDC->MoveTo( rect.right, rect.top-Overlap );
         pDC->LineTo( rect.right, rect.bottom );

         if ( bFirst )
         {
            pDC->MoveTo( rect.left-1, rect.top-Overlap );
            pDC->LineTo( rect.left-1, rect.bottom );
            bFirst = FALSE;
         }
      }

   } // end of column cells loop

} // end of ZGrid::PrintFixedRowCells

void
ZGrid::PrintColumnHeadings( CDC *pDC, CPrintInfo *pInfo )
{
   CFont *pOldFont = pDC->SelectObject( &m_PrinterFont );

   CRect rect;
   rect.bottom = -1;

   zBOOL bFirst = TRUE;
   zBOOL bOriginal;
   int   nColCnt = GetColumnCount( );
   int   nFixedRowCnt = GetFixedRowCount( );
   int   nFixedColCnt = GetFixedColumnCount( );

   // Modified to allow column hdr printing of multi-page wide grids.
   for ( long lRow = 0; lRow < nFixedRowCnt; lRow++ )
   {
      rect.top = rect.bottom + 1;
      rect.bottom = rect.top + GetRowHeight( lRow ) - 1;  //???
      rect.right = -1;

      // If printColumn > fixedcolumncount we are on page 2 or more.
      // Let's print out those fixed cell headings again the 1 or more that
      // would be missed added by M.Fletcher 12/17/00.
      if ( m_nPrintColumn>= nFixedColCnt )
      {
         bOriginal=bFirst;

         // Let's print the missing fixed cells on left first out to last
         // fixed column.
         PrintFixedRowCells( 0, nFixedColCnt, lRow,
                             rect, pDC, bFirst );
         bFirst=bOriginal;
      }

      // Now back to normal business print cells in heading after all fixed
      // columns.
      PrintFixedRowCells( m_nPrintColumn, nColCnt, lRow, rect,
                          pDC, bFirst );
   } // end of Row Loop

   pDC->SelectObject( pOldFont );
} // end of ZGrid::PrintColumnHeadings

/////////////////////////////////////////////////////////////////////////////
//
// Prints line of row buttons on each page of the printout.  Assumes that
// the window origin is setup before calling.
//
/////////////////////////////////////////////////////////////////////////////
void
ZGrid::PrintRowButtons( CDC *pDC, CPrintInfo *pInfo )
{
   CFont *pOldFont = pDC->SelectObject( &m_PrinterFont );
   long  lRowCnt = GetRowCount( );
   int   nFixedColCnt = GetFixedColumnCount( );
   CRect rect;
   rect.right = -1;

   zBOOL bFirst = TRUE;
   for ( int nCol = 0; nCol < nFixedColCnt; nCol++ )
   {
      rect.left = rect.right + 1;
      rect.right = rect.left + GetColumnWidth( nCol ) - 1;

      rect.bottom = -1;
      for ( long lRow = m_nCurrPrintRow; lRow < lRowCnt; lRow++ )
      {
         rect.top = rect.bottom + 1;
         rect.bottom = rect.top + GetRowHeight( lRow ) - 1;  //???

         if ( rect.bottom > m_nPageHeight )
            break;

         ZGridCellBase *pCell = GetCell( lRow, nCol );
         if ( pCell )
            pCell->PrintCell( pDC, lRow, nCol, rect );

         if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ )
         {
            int nOverlap = (nCol == 0) ? 0 : 1;
            pDC->MoveTo( rect.left - nOverlap, rect.bottom );
            pDC->LineTo( rect.right, rect.bottom );
            if ( bFirst )
            {
               pDC->MoveTo( rect.left - nOverlap, rect.top - 1 );
               pDC->LineTo( rect.right, rect.top - 1 );
               bFirst = FALSE;
            }
         }

         if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT )
         {
            int nOverlap = (lRow == 0) ? 0 : 1;
            pDC->MoveTo( rect.right, rect.top - nOverlap );
            pDC->LineTo( rect.right, rect.bottom );
            if ( nCol == 0 )
            {
               pDC->MoveTo( rect.left, rect.top - nOverlap );
               pDC->LineTo( rect.left, rect.bottom );
            }
         }
      }
   }

   pDC->SelectObject( pOldFont );
}

void
ZGrid::PrintHeader( CDC *pDC, CPrintInfo *pInfo )
{
   // print App title on top right margin
   CString csRight;
   csRight.LoadString( AFX_IDS_APP_TITLE );

   // print parent window title in the centre ( Gert Rijs )
   CString csCenter;
   CWnd *pParentWnd = GetParent( );
   while ( pParentWnd )
   {
      pParentWnd->GetWindowText( csCenter );
      if ( csCenter.GetLength( ) )  // can happen if it is a CView,
         break;                     // CChildFrm has the title

      pParentWnd = pParentWnd->GetParent( );
   }

   CFont   fontBold;
   LOGFONT lf;

   // Create bold font for header and footer.
   VERIFY( m_PrinterFont.GetLogFont( &lf ) );
   lf.lfWeight = FW_BOLD;
   VERIFY( fontBold.CreateFontIndirect( &lf ) );

   CFont *pNormalFont = pDC->SelectObject( &fontBold );
   int nPrevBkMode = pDC->SetBkMode( TRANSPARENT );

   CRect rect( pInfo->m_rectDraw );
   if ( csCenter.IsEmpty( ) == FALSE )
   {
      pDC->DrawText( csCenter, &rect,
                     DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER );
   }

   if ( csRight.IsEmpty( ) == FALSE )
   {
      pDC->DrawText( csRight, &rect,
                     DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER );
   }

   pDC->SetBkMode( nPrevBkMode );
   pDC->SelectObject( pNormalFont );
   fontBold.DeleteObject( );

   // Draw ruled-line across top.
   pDC->SelectStockObject( BLACK_PEN );
   pDC->MoveTo( rect.left, rect.bottom );
   pDC->LineTo( rect.right, rect.bottom );
}

// Print footer with a line and date, and page number.
void
ZGrid::PrintFooter( CDC *pDC, CPrintInfo *pInfo )
{
   // Page numbering on left.
   CString csLeft;
   csLeft.Format( _T( "Page %d of %d" ),
                  pInfo->m_nCurPage, pInfo->GetMaxPage( ) );

   // Date and time on the right.
   CString csRight;
   COleDateTime t = COleDateTime::GetCurrentTime( );
   csRight = t.Format( _T( "%c" ) );

   CRect rect( pInfo->m_rectDraw );

   // Draw ruled line on bottom.
   pDC->SelectStockObject( BLACK_PEN );
   pDC->MoveTo( rect.left, rect.top );
   pDC->LineTo( rect.right, rect.top );

   CFont fontBold;
   LOGFONT lf;

   // Create bold font for header and footer.
   m_PrinterFont.GetLogFont( &lf );
   lf.lfWeight = FW_BOLD;
   fontBold.CreateFontIndirect( &lf );

   CFont *pNormalFont = pDC->SelectObject( &fontBold );
   int nPrevBkMode = pDC->SetBkMode( TRANSPARENT );

   // EFW - Bug fix - Force text color to black.  It doesn't always
   // get set to a printable color when it gets here.
   pDC->SetTextColor( RGB( 0, 0, 0 ) );

   if ( csLeft.IsEmpty( ) == FALSE )
   {
      pDC->DrawText( csLeft, &rect,
                     DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER );
   }

   if ( csRight.IsEmpty( ) == FALSE )
   {
      pDC->DrawText( csRight, &rect,
                     DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER );
   }

   pDC->SetBkMode( nPrevBkMode );
   pDC->SelectObject( pNormalFont );
   fontBold.DeleteObject( );
}

void
ZGrid::OnEndPrinting( CDC *pDC, CPrintInfo *pInfo )
{
   m_PrinterFont.DeleteObject( );
}

#endif // !defined( _WIN32_WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )


#ifndef _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// ZGrid persistance

zBOOL
ZGrid::Save( zCPCHAR filename, TCHAR chSeparator /*=_T( ',' )*/ )
{
   CStdioFile File;
   CFileException ex;
   CString csSeparator( chSeparator );
   CString csText;

   if ( File.Open( filename,
                   CFile::modeWrite | CFile::modeCreate | CFile::typeText,
                   &ex ) == 0 )
   {
      ex.ReportError( );
      return( FALSE );
   }

   TRY
   {
      long lRowCnt = GetRowCount( );
      int  nColCnt = GetColumnCount( );
      for ( int k = 0; k < lRowCnt; k++ )
      {
         for ( int j = 0; j < nColCnt; j++ )
         {
            GetItemText( csText, k, j );
            File.WriteString( csText );
            File.WriteString( (j == (nColCnt - 1)) ? _T( "\n" ) :
                                                         csSeparator );
         }
      }

      File.Close( );
   }

   CATCH ( CFileException, e )
   {
      AfxMessageBox( _T( "Unable to save grid list" ) );
      return( FALSE );
   }
   END_CATCH

   return( TRUE );
}

zBOOL
ZGrid::Load( zCPCHAR filename, TCHAR chSeparator /*=_T( ',' )*/ )
{
   if ( GetVirtualMode( ) )
      return( FALSE );

   TCHAR *token;
   TCHAR *end;
   TCHAR szBuffer[ 1024 ];
   CStdioFile File;
   CFileException e;

   if ( File.Open( filename, CFile::modeRead | CFile::typeText ) == 0 )
   {
      e.ReportError( );
      return( FALSE );
   }

   DeleteAllItems( );

   TRY
   {
      // Read Header off file.
      File.ReadString( szBuffer, zsizeof( szBuffer ) );

      // Get first token.
      for ( token = szBuffer, end = szBuffer;
            *end && (*end != chSeparator) && (*end != _T( '\n' ));
            end++ )
      {
         // do nothing
      }

      if ( (*end == _T( '\0' )) && (token == end) )
         token = 0;

      *end = _T( '\0' );

      while ( token )
      {
         InsertColumn( token );

         // Get next token
         for ( token = ++end;
               *end && (*end != chSeparator) && (*end != _T( '\n' ));
               end++ )
         {
            // do nothing
         }

         if ( (*end == _T( '\0' )) && (token == end) )
            token = 0;

         *end = _T( '\0' );
      }

      // Read in rest of data
      int nItem = 0;
      while ( File.ReadString( szBuffer, zsizeof( szBuffer ) ) )
      {
         // Get first token
         for ( token = szBuffer, end = szBuffer;
               *end && (*end != chSeparator) && (*end != _T( '\n' ) );
               end++ )
         {
            // do nothing
         }

         if ( (*end == _T( '\0' )) && (token == end) )
            token = 0;

         *end = _T( '\0' );

         int nSubItem = 0;
         while ( token )
         {
            if ( nSubItem == 0 )
               InsertRow( token );
            else
               SetItemText( nItem, nSubItem, token );

            // Get next token
            for ( token = ++end;
                  *end && (*end != chSeparator) && (*end != _T( '\n' ));
                 end++ )
            {
               // do nothing
            }

            if ( (*end == _T( '\0' )) && (token == end) )
               token = 0;

            *end = _T( '\0' );

            nSubItem++;
         }

         nItem++;
      }

      AutoSizeColumns( GetAutoSizeStyle( ) );
      File.Close( );
   }

   CATCH ( CFileException, e )
   {
      AfxMessageBox( _T( "Unable to load grid data" ) );
      return( FALSE );
   }
   END_CATCH

   return( TRUE );
}
#endif

/////////////////////////////////////////////////////////////////////////////
// ZGrid overrideables

#ifndef GRIDCONTROL_NO_DRAGDROP
// This is no longer needed since I've changed to OLE drag and drop - but
// it's still cool code. : )
CImageList *
ZGrid::CreateDragImage( CPoint *ptHotSpot )
{
   CDC *pDC = GetDC( );
   if ( pDC == 0 )
      return( 0 );

   CRect rect;
   ZCellId cellId = GetFocusCell( );
   if ( GetCellRect( cellId.m_lRow, cellId.m_nCol, rect ) == FALSE )
      return( 0 );

   // Translate coordinate system.
   rect.BottomRight( ) = CPoint( rect.Width( ), rect.Height( ) );
   rect.TopLeft( ) = CPoint( 0, 0 );
   *ptHotSpot = rect.BottomRight( );

   // Create a new imagelist (the caller of this function has responsibility
   // for deleting this list).
   CImageList *pList = new CImageList;
   if ( pList == 0 ||
        pList->Create( rect.Width( ), rect.Height( ), ILC_MASK, 1, 1 ) == 0 )
   {
      if ( pList )
         delete( pList );

      return( 0 );
   }

   // Create mem DC and bitmap.
   CDC MemDC;
   CBitmap bm;
   MemDC.CreateCompatibleDC( pDC );
   bm.CreateCompatibleBitmap( pDC, rect.Width( ), rect.Height( ) );
   CBitmap *pOldBitmap = MemDC.SelectObject( &bm );
   MemDC.SetWindowOrg( 0, 0 );

   // Draw cell onto bitmap in memDC.
   ZGridCellBase *pCell = GetCell( cellId.m_lRow, cellId.m_nCol );
   if ( pCell )
      pCell->Draw( &MemDC, cellId.m_lRow, cellId.m_nCol, rect, FALSE );

   // Clean up.
   MemDC.SelectObject( pOldBitmap );
   ReleaseDC( pDC );

   // Add the bitmap we just drew to the image list.
   pList->Add( &bm, GetDefaultCell( FALSE, FALSE )->GetBackClr( ) );
   bm.DeleteObject( );

   return( pList );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZGrid::SetDisplaySelSet( zSHORT nSelSet )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZGrid::SetDisplaySelSet ", nSelSet );
#endif
   if ( nSelSet == m_nSelectSelSet )
   {
      TraceLineI( "ZGrid::SetDisplaySelSet warning - "
                     "setting Display select set to Select select set",
                  nSelSet );
   }

   m_nDisplaySelSet = nSelSet;
   if ( m_nDisplaySelSet )
      m_ulFlag |= zQUAL_SELECTED;
   else
      m_ulFlag &= ~zQUAL_SELECTED;
}

void
ZGrid::SetSelectSelSet( zSHORT nSelSet )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZGrid::SetSelectSet", nSelSet );
#endif
   if ( nSelSet == m_nDisplaySelSet )
   {
      TraceLineI( "ZGrid::SetDisplaySelSet warning - "
                     "setting Display select set to Select select set",
                  nSelSet );
   }

   m_nSelectSelSet = nSelSet;
}

void
ZGrid::SetSelectedPosition( zLONG lDisplayPos )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "AttrList", *m_pzsTag ) == 0 )
      TraceLineI( "ZGrid::SetSelectedPosition", lDisplayPos );
#endif

#if 0 && defined( _MSC_VER ) && _MSC_VER > 1100  // must be implemented

   if ( lDisplayPos == -1 )
   {
      zLONG  lRowCnt = m_EntityKeyList.GetSize( );
      zLONG  lRow;
      zULONG ulKey = GetEntityKey( m_vApp, *m_pzsEName );

      for ( lRow = 0; lRow < lRowCnt; lRow++ )
      {
         if ( ulKey == m_EntityKeyList.GetAt( lRow ) )
         {
            lDisplayPos = lRow + 1;
            break;
         }
      }
   }

   POSITION pos = GetFirstSelectedItemPosition( );
   if ( pos )
   {
      zLONG lCurrSel = GetNextSelectedItem( pos ) + 1;
      if ( m_nCntPerPage > lDisplayPos )
      {
         if ( lCurrSel > lDisplayPos )
            EnsureVisible( lCurrSel - lDisplayPos, FALSE );
         else
            EnsureVisible( 0, FALSE );

         if ( lCurrSel + m_nCntPerPage < m_lEntityCnt )
            EnsureVisible( lCurrSel + m_nCntPerPage - lDisplayPos,
                           FALSE );
      }

      EnsureVisible( lCurrSel, FALSE );
      Update( lCurrSel );
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// nCol        - The number of the column on which to sort
//               (0 ==> resort using the current column)
// nAscending  - -1 ==> invert current sort order
//                0 ==> retain current sort order
//                1 ==> sort ascending
//                2 ==> sort descending
void
ZGrid::SortByColumn( zSHORT nCol, zSHORT nAscending )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListCtrl::SortByColumn ", *m_pzsTag );
#endif
   if ( nCol < m_nFixedCols || nCol > m_nCols )
   {
      nCol = m_nColLastSorted;
      if ( nCol < m_nFixedCols && m_nCols > 0 )
         nCol = m_nFixedCols;
   }
// else
// if ( m_nFixedCols == 0 )
//    nCol--;

   if ( nCol < 0 )
   {
      m_csSortBuffer = "";
      return;
   }

   if ( nAscending == 1 )
      m_chAscendDescend = 'A';
   else
   if ( nAscending == 2 )
      m_chAscendDescend = 'D';
   else
   if ( nAscending == -1 )
   {
      if ( m_nColLastSorted == nCol )
      {
         if ( m_chAscendDescend == 'A' )
            m_chAscendDescend = 'D';
         else
            m_chAscendDescend = 'A';
      }
      else
         m_chAscendDescend = 'A';
   }

   SetSortColumn( nCol );
   if ( m_vApp && m_pCol[ nCol ].pchAttrib[ 0 ] )
   {
      if ( m_chAscendDescend == 'D' )
         m_nColLastSorted = -1;
      else
         m_nColLastSorted = nCol;

      zLONG lBufferLth;
      zLONG lFind;
      zCHAR szBuffer[ 4096 ];

      if ( m_pCol[ nCol ].pchAttribX && m_pCol[ nCol ].pchAttribX[ 0 ] &&
           m_pCol[ nCol ].pchEntityX && m_pCol[ nCol ].pchEntityX[ 0 ] )
      {
         strcpy_s( szBuffer, zsizeof( szBuffer ), m_pCol[ nCol ].pchEntityX );
      }
      else
         strcpy_s( szBuffer, zsizeof( szBuffer ), m_pCol[ nCol ].pchEntity );

      lBufferLth = strlen( szBuffer );
      szBuffer[ lBufferLth++ ] = '.';

      if ( m_pCol[ nCol ].pchAttribX && m_pCol[ nCol ].pchAttribX[ 0 ] )
         strcpy_s( szBuffer + lBufferLth, zsizeof( szBuffer ) - lBufferLth, m_pCol[ nCol ].pchAttribX );
      else
         strcpy_s( szBuffer + lBufferLth, zsizeof( szBuffer ) - lBufferLth, m_pCol[ nCol ].pchAttrib );

      lBufferLth = zstrlen( szBuffer );
      szBuffer[ lBufferLth++ ] = ' ';
      szBuffer[ lBufferLth ] = 0;

   // "Abteilung.BezOrgText A"                           <== szBuffer
   // "Abteilung.InterneNummer A Abteilung.BezOrgText A" <== m_csSortBuffer
      lFind = 0;
      while ( (lFind = m_csSortBuffer.Find( szBuffer, lFind )) >= 0 )
      {
         if ( lFind == 0 || m_csSortBuffer.GetAt( lFind - 1 ) == ' ' )
            break;

         lFind += lBufferLth;
      }

      if ( lFind >= 0 )
      {
         zPCHAR pch;
         zLONG  lLth = m_csSortBuffer.GetLength( ) + lBufferLth;
         zPCHAR p = m_csSortBuffer.GetBuffer( lLth );
         if ( *(p + lFind + lBufferLth + 1) )
            strcpy_s( p + lFind, lLth - lFind, p + lFind + lBufferLth + 2 );
         else
            p[ lFind ] = 0;

         if ( p[ lFind ] == '[' )
         {
            pch = zstrchr( p + lFind + 1, ']' );
            if ( pch )
               strcpy_s( p + lFind, lLth - lFind, pch + 1 );
         }

         if ( p[ lFind ] == ' ' )
         {
            pch = p + lFind + 1;
            while ( *pch == ' ' )
               pch++;

            strcpy_s( p + lFind, lLth - lFind, pch );
         }

         m_csSortBuffer.ReleaseBuffer( );
      }

      szBuffer[ lBufferLth++ ] = m_chAscendDescend;

      if ( (m_pCol[ nCol ].pchContext &&
            (m_pCol[ nCol ].pchAttribX == 0 ||
             m_pCol[ nCol ].pchAttribX[ 0 ] == 0)) ||
           (m_pCol[ nCol ].pchContextX &&
            m_pCol[ nCol ].pchAttribX && m_pCol[ nCol ].pchAttribX[ 0 ]) )
      {
         szBuffer[ lBufferLth++ ] = ' ';
         szBuffer[ lBufferLth++ ] = '[';
         if ( m_pCol[ nCol ].pchContextX )
            strcpy_s( szBuffer + lBufferLth, zsizeof( szBuffer ) - lBufferLth, m_pCol[ nCol ].pchContextX );
         else
            strcpy_s( szBuffer + lBufferLth, zsizeof( szBuffer ) - lBufferLth, m_pCol[ nCol ].pchContext );

         lBufferLth = (zSHORT) zstrlen( szBuffer );
         szBuffer[ lBufferLth++ ] = ']';
      }

      szBuffer[ lBufferLth ] = 0;
      CString cs = m_csSortBuffer;
      m_csSortBuffer = szBuffer;
      if ( cs.IsEmpty( ) == FALSE )
      {
         m_csSortBuffer += " ";
         m_csSortBuffer += cs;
      }

#if 0
      m_SortedHeaderCtrl.SetSortImage( nCol,
                                       m_chAscendDescend == 'A' ?
                                                              TRUE : FALSE );
      if ( m_bSortDisplay )
      {
         zLONG   lItem;
         LV_ITEM lvi;
         zLONG   lKey = 0;

         for ( lItem = GetNextItem( -1, LVNI_SELECTED );
               lItem != -1;
               lItem = GetNextItem( lItem, LVNI_SELECTED ) )
         {
            if ( lKey == 0 )
               lKey = m_EntityKeyList.GetAt( lItem );

            lvi.mask = LVIF_STATE;
            lvi.iItem = lItem;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );

            lvi.state &= ~(LVIS_SELECTED| LVIS_FOCUSED);
            SetItem( &lvi );
            Update( lItem );
         }

         SortByDisplayData( );
         if ( lKey && (lvi.iItem = GetSortOrderForKey( lKey )) >= 0 )
         {
            lvi.mask = LVIF_STATE;
            lvi.iSubItem = 0;
            lvi.stateMask = 0xFFFF;      // get all state flags
            GetItem( &lvi );

            lvi.state |= LVIS_SELECTED | LVIS_FOCUSED;
            SetItem( &lvi );
         }

         lItem = HiliteAllSelectedEntities( );
         if ( lItem >= 0 )
         {
            EnsureVisible( lItem, FALSE );
            Update( lItem );
         }
      }
      else
#endif
      {
         if ( m_bRememberSort == FALSE )
         {
            if ( cs.Compare( m_csSortBuffer ) == 0 )
            {
               cs = ".bubblesort ";
               cs += m_csSortBuffer;
               OrderEntityForView( m_vApp, *m_pzsEName, cs );
               TraceLine( "Grid: %s   sorted entity: %s   using sort string: %s", *m_pzsTag, *m_pzsEName, cs );
            }
            else
            {
               OrderEntityForView( m_vApp, *m_pzsEName, m_csSortBuffer );
               TraceLine( "Grid: %s   sorted entity: %s   using sort string: %s", *m_pzsTag, *m_pzsEName, m_csSortBuffer );
            }

            MapFromOI( zMAP_NO_SORT | zMAP_BY_SORT );
         }
         else
            MapFromOI( zMAP_BY_SORT );
      }

      Invalidate( );
   }
}

void
ZGrid::OnFixedRowClick( ZCellId& cellId )
{
   if ( IsValid( cellId ) == FALSE )
      return;

   if ( GetHeaderSort( ) )
   {
      CWaitCursor wait;
      if ( m_bAutoSort )
         SortByColumn( cellId.m_nCol, -1 );

   // if ( cellId.m_nCol == GetSortColumn( ) )
   //    SortItems( cellId.m_nCol, !GetSortAscending( ) );
   // else
   //    SortItems( cellId.m_nCol, TRUE );

#if 0
      // Find all selected cells.
      UINT uState;

      long lRow = m_lEntityCnt + m_nFixedRows;
      while ( lRow > m_nFixedRows )
      {
         lRow--;
         int nCol = m_nCols;
         while ( nCol > m_nFixedCols )
         {
            nCol--;
            uState = GetItemState( lRow, nCol );
            if ( uState & GVIS_SELECTED )
            {
               TraceLineI( "After sort selected row: ", lRow );
               break;  // inner loop
            }
         }
      }
#endif

      Invalidate( );
   }

   // Did the user click on a fixed column cell (so the cell was within the
   // overlap of fixed row and column cells) - (fix by David Pritchard).
#if 0 // don't want selection when on a fixed column cell
   if ( GetFixedColumnSelection( ) )
   {
      if ( cellId.m_nCol < GetFixedColumnCount( ) )
      {
         cellId = GetFocusCell( );
      // m_MouseMode = MOUSE_SELECT_ALL;
         m_MouseMode = MOUSE_SELECT_ROW;  // dks???
         OnSelecting( cellId );
      }
      else
      {
      // m_MouseMode = MOUSE_SELECT_COL;
         m_MouseMode = MOUSE_SELECT_ROW;
         OnSelecting( cellId );
      }
   }
#endif
}

void
ZGrid::OnFixedColumnClick( ZCellId& cellId )
{
   if ( IsValid( cellId ) == FALSE )
      return;

// if ( m_bListMode &&
//      (GetItemState( cellId.m_lRow, m_nFixedCols ) & GVNI_SELECTED) )
// {
//    OnEditCell( cellId.m_lRow, cellId.m_nCol, VK_LBUTTON );
//    return;
// }

   // Did the user click on a fixed row cell (so the cell was within the
   // overlap of fixed row and column cells) - (fix by David Pritchard).
   if ( GetFixedRowSelection( ) )
   {
      if ( cellId.m_lRow < GetFixedRowCount( ) )
      {
         cellId = GetFocusCell( );
      // m_MouseMode = MOUSE_SELECT_ALL;
         m_MouseMode = MOUSE_SELECT_ROW;  // dks???
         OnSelecting( cellId );
      }
      else
      {
         m_MouseMode = MOUSE_SELECT_ROW;
         OnSelecting( cellId );
      }
   }
}

// Gets the extent of the text pointed to by cs (no CDC needed).
// By default this uses the selected font (which is a bigger font).
CSize
ZGrid::GetTextExtent( long lRow, int nCol, zCPCHAR cs )
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell == 0 )
      return( CSize( 0, 0 ) );
   else
      return( pCell->GetTextExtent( cs ) );
}

// virtual
void
ZGrid::OnEditCell( long lRow, int nCol, CPoint pt, UINT uChar )
{
#ifndef GRIDCONTROL_NO_TITLETIPS
   m_wndTitleTip.Hide( );  // hide any titletips
#endif

   // Can we do it?
   ZCellId cellId( lRow, nCol );
   if ( IsValid( cellId ) == FALSE || IsCellEditable( lRow, nCol ) == FALSE )
      return;

   // Can we see what we are doing?
   EnsureVisible( lRow, nCol );
   if ( IsCellVisible( lRow, nCol ) == FALSE )
      return;

   // Where, exactly, are we gonna do it??
   CRect rect;
   if ( GetCellRect( cellId, rect ) == FALSE )
      return;

   if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      return;

// TraceRect( "ZGrid::OnEditCell: ", rect );

   // Ensure we can edit ...
   if ( SendMessageToParent( lRow, nCol, GVN_BEGINEDIT ) >= 0 )
   {
      // Let's do it ...
      ZGridCellBase *pCell = GetCell( lRow, nCol );
      if ( pCell )
      {
         pCell->Edit( lRow, nCol, rect, pt, IDC_INPLACE_CONTROL, uChar );
         m_pZSubtask->m_ulSubtaskFlags |= zSUBTASK_INPLACEEDIT;
      }
   }
}

// virtual
void
ZGrid::EndEditing( BOOL bDestroy )
{
   if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      return;

   ZCellId cellId = GetFocusCell( );
   if ( IsValid( cellId ) == FALSE )
      return;

   ZGridCellBase *pCell = GetCell( cellId.m_lRow, cellId.m_nCol );
   if ( pCell && pCell->IsEditing( ) )
   {
      pCell->EndEdit( bDestroy );
      m_pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_INPLACEEDIT;
   }
}

// virtual
void
ZGrid::OnEndEditCell( long lRow, int nCol, CString cs )
{
   CString csText;
   GetItemText( csText, lRow, nCol );
   if ( csText != cs )
   {
      SetItemText( lRow, nCol, cs );
      if ( ValidateEdit( lRow, nCol, cs ) &&
           SendMessageToParent( lRow, nCol, GVN_ENDEDIT ) >= 0 )
      {
         SetModified( TRUE, lRow, nCol );
         RedrawCell( lRow, nCol );
      }
      else
      {
         SetItemText( lRow, nCol, csText );
      }
   }

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   if ( pCell )
      pCell->OnEndEdit( );
}

// If this returns FALSE then the editing isn't allowed.
// virtual
zBOOL
ZGrid::ValidateEdit( long lRow, int nCol, zCPCHAR cpcText )
{
   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return( TRUE );

   return( pCell->ValidateEdit( cpcText ) );
}

// virtual
void
ZGrid::GetItemText( CString& csText, long lRow, int nCol ) const
{
   csText = "";
   if ( lRow < 0 || lRow >= m_lRows || nCol < 0 || nCol >= m_nCols )
      return;

   ZGridCellBase *pCell = GetCell( lRow, nCol );
   ASSERT( pCell );
   if ( pCell == 0 )
      return;

   pCell->GetText( csText, lRow, nCol );
}

/////////////////////////////////////////////////////////////////////////////
//
// GridDropTarget.cpp : implementation file
//
// MFC Grid Control - Drag/Drop target implementation
//
// ZGridDropTarget is an OLE drop target for ZGrid. All it does
// is handle the drag and drop windows messages and pass them
// directly onto the grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.10+
//
/////////////////////////////////////////////////////////////////////////////

#ifndef GRIDCONTROL_NO_DRAGDROP

/////////////////////////////////////////////////////////////////////////////
// ZGridDropTarget

ZGridDropTarget::ZGridDropTarget( )
{
   m_pGridCtrl = 0;
   m_bRegistered = FALSE;
}

ZGridDropTarget::~ZGridDropTarget( )
{
}

// Overloaded Register( ) function performs the normal COleDropTarget::Register
// but also serves to connect this COleDropTarget with the parent grid control,
// where all drop messages will ultimately be forwarded.
zBOOL
ZGridDropTarget::Register( ZGrid *pGrid )
{
   if ( m_bRegistered )
      return( FALSE );

   // Stop re-entry problems
   static zBOOL bRecursive = FALSE;
   if ( bRecursive )
      return( FALSE );

   bRecursive = TRUE;

   ASSERT( pGrid->IsKindOf( RUNTIME_CLASS( ZGrid ) ) );
   ASSERT( pGrid );

   if ( pGrid == 0 || pGrid->IsKindOf( RUNTIME_CLASS( ZGrid ) ) == FALSE )
   {
      bRecursive = FALSE;
      return( FALSE );
   }

   m_pGridCtrl = pGrid;

   m_bRegistered = COleDropTarget::Register( pGrid );

   bRecursive = FALSE;
   return( m_bRegistered );
}

void
ZGridDropTarget::Revoke( )
{
   m_bRegistered = FALSE;
   COleDropTarget::Revoke( );
}

BEGIN_MESSAGE_MAP( ZGridDropTarget, COleDropTarget )
   //{{AFX_MSG_MAP( ZGridDropTarget )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZGridDropTarget message handlers
//
DROPEFFECT
ZGridDropTarget::OnDragScroll( CWnd *pWnd, DWORD dwKeyState, CPoint pt )
{
// TRACE( "In ZGridDropTarget::OnDragScroll\n" );
   if ( pWnd->GetSafeHwnd( ) == m_pGridCtrl->GetSafeHwnd( ) )
   {
      if ( dwKeyState & MK_CONTROL )
         return( DROPEFFECT_COPY );
      else
         return( DROPEFFECT_MOVE );
   }
   else
      return( DROPEFFECT_NONE );
}

DROPEFFECT
ZGridDropTarget::OnDragEnter( CWnd *pWnd, COleDataObject *pDataObject,
                              DWORD dwKeyState, CPoint pt )
{
   TRACE( _T( "In ZGridDropTarget::OnDragEnter\n" ) );
   ASSERT( m_pGridCtrl );

   if ( pWnd->GetSafeHwnd( ) == m_pGridCtrl->GetSafeHwnd( ) )
      return( m_pGridCtrl->OnDragEnter( pDataObject, dwKeyState, pt ) );
   else
      return( DROPEFFECT_NONE );
}

void
ZGridDropTarget::OnDragLeave( CWnd *pWnd )
{
   TRACE( _T( "In ZGridDropTarget::OnDragLeave\n" ) );
   ASSERT( m_pGridCtrl );

   if ( pWnd->GetSafeHwnd( ) == m_pGridCtrl->GetSafeHwnd( ) )
      m_pGridCtrl->OnDragLeave( );
}

DROPEFFECT
ZGridDropTarget::OnDragOver( CWnd *pWnd, COleDataObject *pDataObject,
                             DWORD dwKeyState, CPoint pt )
{
// TRACE( "In ZGridDropTarget::OnDragOver\n" );
   ASSERT( m_pGridCtrl );

   if ( pWnd->GetSafeHwnd( ) == m_pGridCtrl->GetSafeHwnd( ) )
      return( m_pGridCtrl->OnDragOver( pDataObject, dwKeyState, pt ) );
   else
      return( DROPEFFECT_NONE );
}

BOOL
ZGridDropTarget::OnDrop( CWnd *pWnd, COleDataObject *pDataObject,
                         DROPEFFECT dropEffect, CPoint pt )
{
   TRACE( _T( "In ZGridDropTarget::OnDrop\n" ) );
   ASSERT( m_pGridCtrl );

   if ( pWnd->GetSafeHwnd( ) == m_pGridCtrl->GetSafeHwnd( ) )
      return( m_pGridCtrl->OnDrop( pDataObject, dropEffect, pt ) );
   else
      return( FALSE );
}

#endif // GRIDCONTROL_NO_DRAGDROP
// InPlaceEdit.cpp : implementation file
//
// Adapted by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// The code contained in this file is based on the original
// CInPlaceEdit from http://www.codeguru.com/listview/edit_subitems.shtml
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.10+
//
// History:
//       10 May 1998  Uses GVN_ notifications instead of LVN_,
//                 Sends notification messages to the parent,
//                 instead of the parent's parent.
//       15 May 1998  There was a problem when editing with the in-place editor,
//                 there arises a general protection fault in user.exe, with a
//                 few qualifications:
//                   (1) This only happens with owner-drawn buttons;
//                   (2) This only happens in Win95
//                   (3) This only happens if the handler for the button does not
//                      create a new window (even an AfxMessageBox will avoid the
//                      crash)
//                   (4) This will not happen if Spy++ is running.
//                 PreTranslateMessage was added to route messages correctly.
//                 (Matt Weagle found and fixed this problem)
//       26 Jul 1998  Removed the ES_MULTILINE style - that fixed a few probs!
//        6 Aug 1998  Added uId to the constructor param list
//        6 Sep 1998  Space no longer clears selection when starting edit (Franco Bez)
//       10 Apr 1999  Enter, Tab and Esc key prob fixed (Koay Kah Hoe)
//                 Workaround for bizzare "shrinking window" problem in CE
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

BEGIN_MESSAGE_MAP( CInPlaceEdit, CEdit )
   //{{AFX_MSG_MAP( CInPlaceEdit )
   ON_WM_CREATE( )
   ON_WM_KILLFOCUS( )
   ON_WM_CHAR( )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   ON_WM_GETDLGCODE( )
   ON_WM_CREATE( )
   ON_WM_LBUTTONDOWN( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CInPlaceEdit::CInPlaceEdit( ZGrid *pGrid, CRect& rect, DWORD dwStyle,
                            UINT uId, long lRow, int nCol,
                            CString csInitText, UINT uFirstChar )
{
// TraceLineS( "CInPlaceEdit::ctor", "" );
   m_pGrid = pGrid;
   m_csInitText = csInitText;
   m_lRow = lRow;
   m_nCol = nCol;
   m_nLastChar = 0;
   m_bExitOnArrows = (uFirstChar != VK_LBUTTON); // if mouse click brought us
                                                 // here, then no exit on arrows
   m_rect = rect;  // for bizarre CE bug

   DWORD dwEditStyle = dwStyle | WS_BORDER | WS_CHILD |
                                 WS_VISIBLE | ES_AUTOHSCROLL; //| ES_MULTILINE
   if ( Create( dwEditStyle, rect, pGrid, uId ) == 0 )
      return;

   SetFont( pGrid->GetFont( ) );

   SetWindowText( csInitText );
   SetFocus( );

   switch ( uFirstChar )
   {
      case VK_LBUTTON:
      case VK_RETURN:
         SetSel( (int) _tcslen( m_csInitText ), -1 );
         return;

      case VK_BACK:
         SetSel( (int) _tcslen( m_csInitText ), -1 );
         break;

      case VK_TAB:
      case VK_DOWN:
      case VK_UP:
      case VK_RIGHT:
      case VK_LEFT:
      case VK_NEXT:
      case VK_PRIOR:
      case VK_HOME:
      case VK_SPACE:
      case VK_END:
         SetSel( 0, -1 );
         return;

      default:
         SetSel( 0, -1 );
   }

   // Added by KiteFly. When entering DBCS chars into cells the first char
   // was being lost SendMessage changed to PostMessage (John Lagerquist).
   if ( uFirstChar < 0x80 )
      PostMessage( WM_CHAR, uFirstChar );
   else
      PostMessage( WM_IME_CHAR, uFirstChar );
}

CInPlaceEdit::~CInPlaceEdit( )
{
// TraceLineS( "CInPlaceEdit::dtor", "" );
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers
int
CInPlaceEdit::OnCreate( LPCREATESTRUCT lpcs )
{
   int nRC = CEdit::OnCreate( lpcs );
   if ( nRC != -1 )
      m_pGrid->m_pZeidonWnd = this;

// TraceLineI( "CInPlaceEdit::OnCreate RC: ", nRC );
   return( nRC );
}

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void
CInPlaceEdit::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLineX( "CInPlaceEdit::OnKeyDown uFlags: ", uFlags );
   if ( uFlags & 0x00000200 )  // flag from ZGrid ... we don't want this
      return;

   if ( (uChar == VK_PRIOR || uChar == VK_NEXT ||
         uChar == VK_DOWN  || uChar == VK_UP   ||
         uChar == VK_RIGHT || uChar == VK_LEFT) &&
        (/*m_bExitOnArrows || always force Ctrl to move off cell ... dks*/
         GetKeyState( VK_CONTROL ) < 0) )
   {
      m_nLastChar = uChar;
      GetParent( )->SetFocus( );
      return;
   }

   CEdit::OnKeyDown( uChar, uRepCnt, uFlags );
}

void
CInPlaceEdit::OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLineX( "CInPlaceEdit::OnKeyUp uFlags: ", uFlags );
   if ( uFlags & 0x00000200 )  // flag from ZGrid ... we don't want this
      return;

   CEdit::OnKeyUp( uChar, uRepCnt, uFlags );
}

void
CInPlaceEdit::OnChar( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLineX( "CInPlaceEdit::OnChar uFlags: ", uFlags );
   if ( uFlags & 0x00000200 )  // flag from ZGrid ... we don't want this
      return;

   if ( uChar == VK_TAB || uChar == VK_RETURN )
   {
      m_nLastChar = uChar;
      GetParent( )->SetFocus( );   // This will destroy this window
      return;
   }

   if ( uChar == VK_ESCAPE )
   {
      SetWindowText( m_csInitText );   // restore previous text
      m_nLastChar = uChar;
      GetParent( )->SetFocus( );
      return;
   }

   CEdit::OnChar( uChar, uRepCnt, uFlags );

   // Resize edit control if needed.

   // Get text extent.
   CString cs;
   GetWindowText( cs );

   // Add some extra buffer.
   cs += _T( "  " );

   CWindowDC dc( this );
   CFont *pFontDC = dc.SelectObject( GetFont( ) );
   CSize size = dc.GetTextExtent( cs );
   dc.SelectObject( pFontDC );

   // Get client rect.
   CRect rectParent;
   GetParent( )->GetClientRect( &rectParent );

   // Check whether control needs to be resized and whether there is space
   // to grow.
   if ( size.cx > m_rect.Width( ) )
   {
      if ( size.cx + m_rect.left < rectParent.right )
         m_rect.right = m_rect.left + size.cx;
      else
         m_rect.right = rectParent.right;

      MoveWindow( &m_rect );
   }
}

void
CInPlaceEdit::OnLButtonDown( UINT uFlags, CPoint pt )
{
// TraceLineX( "CInPlaceEdit::OnLButtonDown hWnd: ", (zLONG) m_hWnd );
   if ( mIs_hWnd( m_hWnd ) )
      CEdit::OnLButtonDown( uFlags, pt );
}

// As soon as this edit control loses focus, kill it.
void
CInPlaceEdit::OnKillFocus( CWnd *pNewWnd )
{
// TraceLine( "CInPlaceEdit::OnKillFocus hWnd: 0x%08x  New Wnd: 0x%08x",
//            m_hWnd, pNewWnd->GetSafeHwnd( ) );

   CEdit::OnKillFocus( pNewWnd );
   EndEdit( );
}

UINT
CInPlaceEdit::OnGetDlgCode( )
{
// TraceLineX( "CInPlaceEdit::OnGetDlgCode hWnd: ", (zLONG) m_hWnd );
   return( DLGC_WANTALLKEYS );
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit overrides

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL
CInPlaceEdit::PreTranslateMessage( MSG *pMsg )
{
// TraceLine( "CInPlaceEdit::PreTranslateMessage hWnd: 0x%08x  Msg: 0x%x",
//            pMsg->hwnd, pMsg->message );
   // Catch the Alt key so we don't choke if focus is going to an owner
   // drawn button.
   if ( pMsg->message == WM_SYSCHAR )
      return( TRUE );

   return( CWnd::PreTranslateMessage( pMsg ) );
}

// Auto delete
void
CInPlaceEdit::PostNcDestroy( )
{
// TraceLineX( "CInPlaceEdit::PostNcDestroy hWnd: ", (zLONG) m_hWnd );
   if ( mIs_hWnd( m_hWnd ) )
      CEdit::PostNcDestroy( );

   delete( this );
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit implementation
void
CInPlaceEdit::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineX( "CInPlaceEdit::EndEdit hWnd: ", (zLONG) m_hWnd );
   ASSERT( ::IsWindow( m_hWnd ) );
#endif
   CString cs;

   // EFW - BUG FIX - Clicking on a grid scroll bar in a derived class
   // that validates input can cause this to get called multiple times
   // causing assertions because the edit control goes away the first time.
   static zBOOL bAlreadyEnding = FALSE;  // there can only be one inplace in the "universe"

   if ( bAlreadyEnding )
      return;

   bAlreadyEnding = TRUE;
   GetWindowText( cs );

   // Send Notification to parent.
   GV_DISPINFO dispinfo;

   dispinfo.hdr.hwndFrom = GetSafeHwnd( );
   dispinfo.hdr.idFrom = GetDlgCtrlID( );
   dispinfo.hdr.code = GVN_ENDEDIT;

   dispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;
   dispinfo.item.lRow = m_lRow;
   dispinfo.item.nCol = m_nCol;
   dispinfo.item.csText = cs;
   dispinfo.item.lItemExtra = (LPARAM) m_nLastChar;

   CWnd *pOwner = GetOwner( );
   if ( mIs_hWnd( pOwner->GetSafeHwnd( ) ) )
      pOwner->SendMessage( WM_NOTIFY, GetDlgCtrlID( ), (LPARAM) &dispinfo );

   // Close this window (PostNcDestroy will delete this).
   if ( bDestroy && mIs_hWnd( GetSafeHwnd( ) ) )
      SendMessage( WM_CLOSE, 0, 0 );

   bAlreadyEnding = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// GridCell.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for the "default" cell type of the
// grid control. Adds in cell editing.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.20+
//
// History:
// Eric Woodruff - 20 Feb 2000 - Added PrintCell( ) plus other minor changes
// Ken Bertelson - 12 Apr 2000 - Split ZGridCell into ZGridCell and ZGridCellBase
// <kenbertelson@hotmail.com>
// C Maunder    - 17 Jun 2000 - Font handling optimsed, Added ZGridDefaultCell
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( ZGridCell, ZGridCellBase )
IMPLEMENT_DYNCREATE( ZGridDefaultCell, ZGridCell )

/////////////////////////////////////////////////////////////////////////////
// GridCell

ZGridCell::ZGridCell( )
{
   m_plfFont = 0;
   ZGridCell::Reset( );
}

ZGridCell::~ZGridCell( )
{
   m_pGrid = 0;
   mDeleteInit( m_plfFont );
}

/////////////////////////////////////////////////////////////////////////////
// GridCell Attributes

void
ZGridCell::operator=( const ZGridCell& cellId )
{
   if ( this != &cellId )
      ZGridCellBase::operator=( cellId );
}

void
ZGridCell::Reset( )
{
   m_pGrid = 0;
   ZGridCellBase::Reset( );

   m_csText.Empty( );
   m_nImage   = -1;
   m_bEditing = FALSE;
   m_pEditWnd = 0;

   m_dwFormat = (DWORD) -1;   // use default from ZGridDefaultCell
   m_clrBkClr = CLR_DEFAULT;  // background color (or CLR_DEFAULT)
   m_clrFgClr = CLR_DEFAULT;  // foreground color (or CLR_DEFAULT)
   m_uMargin = (UINT) -1;     // use default from ZGridDefaultCell
   m_lCellExtra = 0;

   mDeleteInit( m_plfFont );             // cell font
}

zBOOL
ZGridCell::SetEditing( zBOOL bOn )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCell::SetEditing ", bOn ? "TRUE" : "FALSE" );
#endif

   if ( (m_pGrid->m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      return( FALSE );

   ZCellId cellId = m_pGrid->GetFocusCell( );
   zGRID_CELL cell;
   zBOOL bPrevSet = m_bEditing;
   zLONG lRC = 0;

   cell.lRow = cellId.m_lRow + 1 - m_pGrid->m_nFixedRows;
   cell.nCol = cellId.m_nCol + 1 - m_pGrid->m_nFixedCols;
   cell.pchText = 0;

   m_bEditing = bOn;
   if ( bOn && bPrevSet == FALSE )
   {
   // TraceLine( "ZGrid::RelayEvent BEGINEDIT  Row: %d  Col: %d  TRUE( %d, %d )",
   //            cellId.m_lRow, cellId.m_nCol, cell.lRow, cell.nCol );
   // DisplayEntityInstance( m_pGrid->m_vApp, *(m_pGrid->m_pzsEName) );
      lRC = ProcessImmediateEvent( m_pGrid, zGRDN_BEGINEDIT, &cell );
   }
   else
   if ( bOn == FALSE && bPrevSet )
   {
   // TraceLine( "ZGrid::RelayEvent ENDEDIT  Row: %d  Col: %d  TRUE( %d, %d )",
   //            cellId.m_lRow, cellId.m_nCol, cell.lRow, cell.nCol );
   // DisplayEntityInstance( m_pGrid->m_vApp, *(m_pGrid->m_pzsEName) );
      lRC = ProcessImmediateEvent( m_pGrid, zGRDN_ENDEDIT, &cell );
   // RemoveCursorListener( m_pGrid->m_vApp, GridCursorListener, m_pGrid->m_pzsEName );
   }

   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      return( TRUE );
   }
   else
   {
      m_bEditing = bPrevSet;
      return( FALSE );
   }
}

void
ZGridCell::SetFont( const LOGFONT *plf )
{
   if ( plf == 0 )
   {
      delete( m_plfFont );
      m_plfFont = 0;
   }
   else
   {
      if ( m_plfFont == 0 )
         m_plfFont = new LOGFONT;

      if ( m_plfFont )
         memcpy( m_plfFont, plf, sizeof( LOGFONT ) );
   }
}

LOGFONT *
ZGridCell::GetFont( ) const
{
   if ( m_plfFont == 0 )
   {
      ZGridDefaultCell *pDefaultCell = (ZGridDefaultCell *) GetDefaultCell( );
      if ( pDefaultCell == 0 )
         return( 0 );

      return( pDefaultCell->GetFont( ) );
   }

   return( m_plfFont );
}

CFont *
ZGridCell::GetFontObject( ) const
{
   // If the default font is specified, use the default cell implementation.
   if ( m_plfFont == 0 )
   {
      ZGridDefaultCell *pDefaultCell = (ZGridDefaultCell *) GetDefaultCell( );
      if ( pDefaultCell == 0 )
         return( 0 );

      return( pDefaultCell->GetFontObject( ) );
   }
   else
   {
      static CFont font;
      font.DeleteObject( );
      font.CreateFontIndirect( m_plfFont );
      return( &font );
   }
}

DWORD
ZGridCell::GetFormat( ) const
{
   if ( m_dwFormat == (DWORD) -1 )
   {
      ZGridDefaultCell *pDefaultCell = (ZGridDefaultCell *) GetDefaultCell( );
      if ( pDefaultCell == 0 )
         return( 0 );

      return( pDefaultCell->GetFormat( ) );
   }

   return( m_dwFormat );
}

UINT
ZGridCell::GetMargin( ) const
{
   if ( m_uMargin == (UINT) -1 )
   {
      ZGridDefaultCell *pDefaultCell = (ZGridDefaultCell *) GetDefaultCell( );
      if ( pDefaultCell == 0 )
         return( 0 );

      return( pDefaultCell->GetMargin( ) );
   }

   return( m_uMargin );
}

/////////////////////////////////////////////////////////////////////////////
// GridCell Operations

BOOL
ZGridCell::Edit( long lRow, int nCol, CRect rect,
                 CPoint pt, UINT uId, UINT uChar )
{
#ifdef DEBUG_ALL
   TraceLine( "ZGridCell::Edit Row: %d, Col: %d", lRow, nCol );
#endif
   if ( IsEditing( ) )
   {
      if ( m_pEditWnd )
        m_pEditWnd->SendMessage( WM_CHAR, uChar );
   }
   else
   {
      CString cs;
      DWORD dwStyle = GetFormat( );
      if ( dwStyle & DT_RIGHT )
         dwStyle = ES_RIGHT;
      else
      if ( dwStyle & DT_CENTER )
         dwStyle = ES_CENTER;
      else
         dwStyle = ES_LEFT;

      SetEditing( TRUE );

      // InPlaceEdit auto-deletes itself.
      ZGrid *pGrid = GetGrid( );
      GetText( cs, lRow, nCol );
      m_pEditWnd = new CInPlaceEdit( pGrid, rect, dwStyle, uId,
                                     lRow, nCol, cs, uChar );
   }

   return( TRUE );
}

void
ZGridCell::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCell::EndEdit", "" );
#endif
   if ( m_pEditWnd )
      ((CInPlaceEdit *) m_pEditWnd)->EndEdit( bDestroy );
   else
   if ( bDestroy )
      SetEditing( FALSE );
}

void
ZGridCell::OnEndEdit( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCell::OnEndEdit", "" );
#endif
   SetEditing( FALSE );
   m_pEditWnd = 0;
   ZGrid *pGrid = GetGrid( );
   if ( pGrid )
      pGrid->m_pZeidonWnd = 0;
}

// virtual
void
ZGridCell::SetText( zCPCHAR cpcText, long lRow, int nCol )
{
   if ( m_pGrid && mIs_hWnd( m_pGrid->m_hWnd ) )
   {
      if ( lRow >= m_pGrid->m_nFixedRows && lRow < m_pGrid->m_lRows &&
           nCol >= m_pGrid->m_nFixedCols && nCol < m_pGrid->m_nCols )
      {
         m_pGrid->TransferDataFromCellToOI( cpcText, lRow, nCol );
      }
      else
      if ( nCol < m_pGrid->m_nFixedCols && lRow >= m_pGrid->m_nFixedRows )
      {
         // Do nothing for column showing row numbers.
      }
      else
      {
         m_csText = cpcText;
      }
   }
   else
   {
      m_csText = cpcText;
   }
}

// virtual
void
ZGridCell::SetData( LPARAM lCellExtra )
{
   m_lCellExtra = lCellExtra;
}

// virtual
void
ZGridCell::GetText( CString& csText, long lRow, int nCol ) const
{
   csText = m_csText;
   if ( m_pGrid && mIs_hWnd( m_pGrid->m_hWnd ) )
   {
      if ( lRow >= m_pGrid->m_nFixedRows && lRow < m_pGrid->m_lRows &&
           nCol >= m_pGrid->m_nFixedCols && nCol < m_pGrid->m_nCols )
      {
         m_pGrid->FormatTextForCell( csText, lRow, nCol );
      }
      else
      if ( nCol < m_pGrid->m_nFixedCols && lRow >= m_pGrid->m_nFixedRows )
      {
         csText.Format( "%d", lRow - m_pGrid->m_nFixedRows + 1 );
      }
   }
}

// virtual
LPARAM
ZGridCell::GetData( long lRow, int nCol ) const
{
   return( m_lCellExtra );
}

/////////////////////////////////////////////////////////////////////////////
// ZGridDefaultCell

ZGridDefaultCell::ZGridDefaultCell( )
{
#ifdef _WIN32_WCE
   m_dwFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
#else
   m_dwFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE |
                                       DT_NOPREFIX | DT_END_ELLIPSIS;
#endif
   m_clrFgClr = CLR_DEFAULT;
   m_clrBkClr = CLR_DEFAULT;
// m_sizeDefault = CSize( 40, 16 );
   m_sizeDefault = CSize( 30, 10 );
   m_dwStyle = 0;

#ifdef _WIN32_WCE
   LOGFONT lf;
   GetObject( GetStockObject( SYSTEM_FONT ), sizeof( LOGFONT ), &lf );
   SetFont( &lf );
#else // not CE
   NONCLIENTMETRICS ncm;
   ncm.cbSize = sizeof( NONCLIENTMETRICS );
   VERIFY( SystemParametersInfo( SPI_GETNONCLIENTMETRICS,
                                 sizeof( NONCLIENTMETRICS ), &ncm, 0 ) );
   SetFont( &(ncm.lfMessageFont) );
#endif
}

ZGridDefaultCell::~ZGridDefaultCell( )
{
   m_fontCached.DeleteObject( );
}

////////////////////////////////////////////////////////////////////////////////////
// Setters/Getters

// virtual
DWORD
ZGridDefaultCell::GetStyle( ) const
{
   return( m_dwStyle );
}

// virtual
void
ZGridDefaultCell::SetStyle( DWORD dwStyle )
{
   m_dwStyle = dwStyle;
}

// virtual
int
ZGridDefaultCell::GetWidth( ) const
{
   return( m_sizeDefault.cx );
}

// virtual
int
ZGridDefaultCell::GetHeight( ) const
{
   return( m_sizeDefault.cy );
}

// virtual
void
ZGridDefaultCell::SetWidth( int nWidth )
{
   m_sizeDefault.cx = nWidth;
}

// virtual
void
ZGridDefaultCell::SetHeight( int nHeight )
{
   m_sizeDefault.cy = nHeight;
}

void
ZGridDefaultCell::SetFont( const LOGFONT *plf )
{
   ASSERT( plf );

   if ( plf == 0 )
      return;

   m_fontCached.DeleteObject( );
   m_fontCached.CreateFontIndirect( plf );

   ZGridCell::SetFont( plf );

   // Get the font size and hence the default cell size.
   CDC *pDC = CDC::FromHandle( ::GetDC( 0 ) );
   if ( pDC )
   {
      CFont *pOldFont = pDC->SelectObject( &m_fontCached );

      SetMargin( pDC->GetTextExtent( _T( " " ), 1 ).cx );
      m_sizeDefault = pDC->GetTextExtent( _T( " XXXXXXXXXXXX " ), 14 );
      m_sizeDefault.cy = (m_sizeDefault.cy * 3) / 2;

      pDC->SelectObject( pOldFont );
      ReleaseDC( 0, pDC->GetSafeHdc( ) );
   }
   else
   {
      SetMargin( 3 );
   // m_sizeDefault = CSize( 40, 16 );
      m_sizeDefault = CSize( 30, 10 );
   }
}

LOGFONT *
ZGridDefaultCell::GetFont( ) const
{
   ASSERT( m_plfFont );  // this is the default - it CAN'T be 0!
   return( m_plfFont );
}

CFont *
ZGridDefaultCell::GetFontObject( ) const
{
   ASSERT( m_fontCached.GetSafeHandle( ) );
   return( (CFont *) &m_fontCached );
}

DWORD
ZGridDefaultCell::GetState( ) const
{
   return( ZGridCell::GetState( ) | GVIS_READONLY );
}

// GridCellBase.cpp : implementation file
//
// MFC Grid Control - Main grid cell base class
//
// Provides the implementation for the base cell type of the
// grid control. No data is stored (except for state) but default
// implementations of drawing, printingetc provided. MUST be derived
// from to be used.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.22+
//
// History:
// Ken Bertelson - 12 Apr 2000 - Split ZGridCell into ZGridCell and
//                 ZGridCellBase
// C Maunder     - 19 May 2000 - Fixed sort arrow drawing (Ivan Ilinov)
// C Maunder     - 29 Aug 2000 - operator= checks for 0 font before
//                 setting (Martin Richter)
// C Maunder     - 15 Oct 2000 - GetTextExtent fixed (Martin Richter)
// C Maunder     -  1 Jan 2001 - Added ValidateEdit
//
// NOTES: Each grid cell should take care of its own drawing, though the
//      Draw( ) method takes an "erase background" parameter that is called
//      if the grid decides to draw the entire grid background in on hit.
//      Certain ambient properties such as the default font to use, and hints
//      on how to draw fixed cells should be fetched from the parent grid.
//      The grid trusts the cells will behave in a certain way, and the cells
//      trust the grid will supply accurate information.
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZGridCellBase, CObject )

/////////////////////////////////////////////////////////////////////////////
// GridCellBase

ZGridCellBase::ZGridCellBase( )
{
   Reset( );
}

ZGridCellBase::~ZGridCellBase( )
{
}

/////////////////////////////////////////////////////////////////////////////
// GridCellBase Operations

void
ZGridCellBase::Reset( )
{
   m_bEditing = FALSE;
   m_dwState  = 0;
}

void
ZGridCellBase::operator=( const ZGridCellBase& cell )
{
   if ( &cell == this )
      return;

   SetGrid( cell.GetGrid( ) );   // do first in case of dependencies

// SetText( cell.GetText( ) );
   SetImage( cell.GetImage( ) );
// SetData( cell.GetData( ) );
   SetState( cell.GetState( ) );
   SetFormat( cell.GetFormat( ) );
   SetTextClr( cell.GetTextClr( ) );
   SetBackClr( cell.GetBackClr( ) );
   SetFont( cell.IsDefaultFont( ) ? 0 : cell.GetFont( ) );
   SetMargin( cell.GetMargin( ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZGridCellBase Attributes

// Returns a pointer to a cell that holds default values for this particular
// type of cell.
ZGridCellBase *
ZGridCellBase::GetDefaultCell( ) const
{
   ZGrid *pGrid = GetGrid( );
   if ( pGrid )
      return( pGrid->GetDefaultCell( IsFixedRow( ), IsFixedCol( ) ) );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// ZGridCellBase Operations

// EFW - Various changes to make it draw cells better when using alternate
// color schemes.  Also removed printing references as that's now done
// by PrintCell( ) and fixed the sort marker so that it doesn't draw out
// of bounds.
BOOL
ZGridCellBase::Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                     BOOL bEraseBkgnd /* = TRUE */,
                     BOOL bDrawText /* = TRUE */ )
{
   // Note - all through this function we totally brutalize 'rect'.  Do not
   // depend on its value being that which was passed in.

   ZGrid *pGrid = GetGrid( );
   ASSERT( pGrid );

// TraceLine( "GridCellBase::Draw Row: %d   Col: %d", lRow, nCol );

   if ( pGrid == 0 || pGrid->m_pCol == 0 || pDC == 0 )
      return( FALSE );

   if ( rect.Width( ) <= 0 || rect.Height( ) <= 0 ) // prevents imagelist item
      return( FALSE );                              // from drawing even though
                                                    // cell is hidden

   // TRACE3( "Drawing %scell %d, %d\n", IsFixed() ?
   //                                _T( "Fixed " ) : _T( "" ), lRow, nCol );

   // Get the default cell implementation for this kind of cell. We use it if
   // this cell has anything marked as "default".
   ZGridDefaultCell *pDefaultCell = (ZGridDefaultCell *) GetDefaultCell( );
   if ( pDefaultCell == 0 )
      return( FALSE );

   int nSavedDC = pDC->SaveDC( );
   pDC->SetBkMode( TRANSPARENT );

   // Set up text and background colors.
   COLORREF clrTextSave;
   COLORREF clrText;
   COLORREF clrTextBk;
   int   nRowHeight = pGrid->GetRowHeight( lRow ) *
                                     (pGrid->m_nExtraRowsPerItem + 1);  //???
   CRect rectIcon( rect );

   clrText = GetTextClr( );
   if ( clrText == CLR_DEFAULT )
      clrText = pDefaultCell->GetTextClr( );

   clrTextBk = GetBackClr( );
   if ( clrTextBk == CLR_DEFAULT )
   {
      if ( pGrid->IsCellEditable( lRow, nCol ) )
         clrTextBk = pDefaultCell->GetBackClr( );
      else
      {
         clrTextBk = ::GetSysColor( COLOR_3DFACE );
         bEraseBkgnd = TRUE;
      }
   }
   else
      bEraseBkgnd = TRUE;

   CFont   *pFont = 0;
   CFont   *pFontOld = 0;
   ZColorItem *pColorItem;
   CString csText;

// if ( lRow == 0 && nCol == 0 && zstrcmp( *(pGrid->m_pzsTag), "GridListAccount" ) == 0 )
// {
//    GetText( csText, lRow, nCol );
//    if ( csText != "" )
//       TraceLineS( "Drawing text: ", csText );
// }

   if ( bDrawText )
   {
      GetText( csText, lRow, nCol );
      int nLF = csText.Find( '\n' );
      int nCR = csText.Find( '\r' );
      if ( nCR >= 0 || nLF >= 0 )
      {
         if ( nLF < 0 )
            nLF = nCR;

         if ( nCR < 0 )
            nCR = nLF;

         if ( nCR < nLF )
            nLF = nCR;

         CString cs = csText.Left( nLF );
         cs += "...";
         csText = cs;
      }

   // if ( lRow == 9 && nCol == 2 )
   // {
   //    TraceLine( "DrawItem Text: %s  Row: %d  Col: %d",
   //               csText, lRow, nCol );
   // }

      if ( lRow >= pGrid->m_nFixedRows && nCol >= pGrid->m_nFixedCols )
         pColorItem = (ZColorItem *) pGrid->GetItemData( lRow, nCol );
      else
         pColorItem = 0;

      int nColIdx = nCol;
      if ( pGrid->GetFixedColumnCount( ) == 0 )
         nColIdx++;

      if ( pColorItem )
      {
         if ( pColorItem->m_lColumn == -1 )  // color full line
         {
            // Flag - 1=Bold 2=Italic 4=Underline 8=Strikeout
            if ( pColorItem->m_lClrFlags & 0x0000000F )
            {
               LOGFONT lf;

               pFont = new CFont;
            // (pDC->GetCurrentFont( ))->GetLogFont( &lf );
               (GetFontObject( ))->GetLogFont( &lf );
               if ( pColorItem->m_lClrFlags & 0x00000001 )
                  lf.lfWeight = FW_BOLD;
               else
                  lf.lfWeight = FW_NORMAL;

               if ( pColorItem->m_lClrFlags & 0x00000002 )
                  lf.lfItalic = TRUE;
               else
                  lf.lfItalic = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000004 )
                  lf.lfUnderline = TRUE;
               else
                  lf.lfUnderline = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000008 )
                  lf.lfStrikeOut = TRUE;
               else
                  lf.lfStrikeOut = FALSE;

               if ( pFont->CreateFontIndirect( &lf ) )
                  pFontOld = pDC->SelectObject( pFont );
               else
               {
                  delete( pFont );
                  pFont = 0;
               }
            }

            if ( pColorItem->m_lClrText != -1 )  // default color
               clrText = pColorItem->m_lClrText;

            if ( pColorItem->m_lClrBack != -1 )  // default color
               clrTextBk = pColorItem->m_lClrBack;
         }

         // Try to find a column specific value.
         if ( pGrid->m_pColorList )
            pColorItem = pGrid->m_pColorList->Find( nCol, csText, TRUE );
         else
            pColorItem = 0;

         if ( pColorItem )
         {
            // Flag - 1=Bold 2=Italic 4=Underline 8=Strikeout
            if ( pColorItem->m_lClrFlags & 0x0000000F )
            {
               LOGFONT lf;

               mDeleteInit( pFont );
               pFont = new CFont;

            // (pDC->GetCurrentFont( ))->GetLogFont( &lf );
               (GetFontObject( ))->GetLogFont( &lf );
               if ( pColorItem->m_lClrFlags & 0x00000001 )
                  lf.lfWeight = FW_BOLD;
               else
                  lf.lfWeight = FW_NORMAL;

               if ( pColorItem->m_lClrFlags & 0x00000002 )
                  lf.lfItalic = TRUE;
               else
                  lf.lfItalic = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000004 )
                  lf.lfUnderline = TRUE;
               else
                  lf.lfUnderline = FALSE;

               if ( pColorItem->m_lClrFlags & 0x00000008 )
                  lf.lfStrikeOut = TRUE;
               else
                  lf.lfStrikeOut = FALSE;

               if ( pFont->CreateFontIndirect( &lf ) )
               {
                  CFont *pFontTemp = pDC->SelectObject( pFont );
                  if ( pFontOld == 0 )
                     pFontOld = pFontTemp;
               }
               else
               {
                  delete( pFont );
                  pFont = 0;
               }
            }

            if ( pColorItem->m_lClrText != -1 )  // default color
               clrText = pColorItem->m_lClrText;

            if ( pColorItem->m_lClrBack != -1 )  // default color
               clrTextBk = pColorItem->m_lClrBack;
         }
      }

      if ( (pGrid->m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      {
         if ( pGrid->m_ulMapActFlags & zMAPACT_DISABLE_READONLY )
         {
            if ( lRow < pGrid->m_nFixedRows || nCol < pGrid->m_nFixedCols )
               clrText = ::GetSysColor( COLOR_3DSHADOW );
            else
               clrTextBk = pDefaultCell->GetBackClr( );
         }
      }
   }

   // Draw cell background and highlighting (if necessary).
   ZCellId cellId = pGrid->GetFocusCell( );
   zBOOL bFocusCell =
          (pGrid->GetFocus( ) == pGrid &&
           pGrid->IsItemEditing( cellId.m_lRow, cellId.m_nCol ) == FALSE &&
           (IsFocused( ) ||
            (cellId.m_lRow == lRow && cellId.m_nCol == nCol))) ? TRUE : FALSE;

   if ( bFocusCell || IsDropHighlighted( ) )
   {
   // TraceLine( "GridCellBase (focus) ::Draw Row: %d   Col: %d   Text: %s",
   //            lRow, nCol, csText.Left( 128 ) );

      // Always draw even in list mode so that we can tell where the
      // cursor is at.  Use the highlight colors though.
      if ( GetState( ) & GVIS_SELECTED )
      {
         clrTextBk = ::GetSysColor( COLOR_HIGHLIGHT );
         clrText = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
         bEraseBkgnd = TRUE;
      }

      rect.right++;   // FillRect doesn't draw RHS or bottom
      rect.bottom++;
      if ( bEraseBkgnd )
      {
         TRY
         {
            CBrush brush( clrTextBk );
            pDC->FillRect( rect, &brush );
         }
         CATCH( CResourceException, e )
         {
            //e->ReportError( );
         }
         END_CATCH
      }

      // Don't adjust frame rect if no grid lines so that the
      // whole cell is enclosed.
      if ( pGrid->GetGridLines( ) != GVL_NONE )
      {
         rect.right--;
         rect.bottom--;
      }

      if ( pGrid->GetFrameFocusCell( ) )
      {
         // Use same color as text to outline the cell so that it shows
         // up if the background is black.
         TRY
         {
            CBrush brush( clrText );
            pDC->FrameRect( rect, &brush );
         }
         CATCH( CResourceException, e )
         {
            // e->ReportError( );
         }
         END_CATCH
      }

      clrTextSave = pDC->SetTextColor( clrText );

      // Adjust rect after frame draw if no grid lines.
      if ( pGrid->GetGridLines( ) == GVL_NONE )
      {
         rect.right--;
         rect.bottom--;
      }

      rect.DeflateRect( 0, 1, 1, 1 );
      if ( bFocusCell )
      {
         CRect rectFocus( rect );
         rectFocus.DeflateRect( 1, 1, 1, 1 );
         pDC->DrawFocusRect( rectFocus );
      }
   }
   else
   if ( (GetState( ) & GVIS_SELECTED) )
   {
   // TraceLine( "GridCellBase (selected) ::Draw Row: %d   Col: %d   Text: %s",
   //            lRow, nCol, csText.Left( 128 ) );

      rect.right++;   // FillRect doesn't draw RHS or bottom
      rect.bottom++;
      pDC->FillSolidRect( rect, ::GetSysColor( COLOR_HIGHLIGHT ) );
      rect.right--;
      rect.bottom--;
      clrTextSave = pDC->SetTextColor( ::GetSysColor( COLOR_HIGHLIGHTTEXT ) );
   }
   else
   {
   // TraceLine( "GridCellBase (erase?) ::Draw Row: %d   Col: %d   Text: %s",
   //            lRow, nCol, csText.Left( 128 ) );

      if ( bEraseBkgnd )
      {
         rect.right++;    // FillRect doesn't draw RHS or bottom
         rect.bottom++;
         CBrush brush( clrTextBk );
         pDC->FillRect( rect, &brush );
         rect.right--;
         rect.bottom--;
      }

      clrTextSave = pDC->SetTextColor( clrText );
   }

   // Draw lines only when wanted.
   if ( IsFixed( ) && pGrid->GetGridLines( ) != GVL_NONE )
   {
      ZCellId cellFocus = pGrid->GetFocusCell( );

      // As above, always show current location even in list mode so
      // that we know where the cursor is at.
      zBOOL bHiliteFixed = pGrid->GetTrackFocusCell( ) &&
                          pGrid->IsValid( cellFocus ) &&
                          (cellFocus.m_lRow == lRow || cellFocus.m_nCol == nCol);

      // If this fixed cell is on the same row/col as the focus cell,
      // highlight it.
      CPen *pOldPen = pDC->GetCurrentPen( );
      CPen penDark( PS_SOLID, 2, ::GetSysColor( COLOR_3DDKSHADOW ) );
      if ( bHiliteFixed )
      {
         rect.right++;
         rect.bottom++;
         pDC->DrawEdge( rect, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT );
         rect.DeflateRect( 1, 1 );

         pDC->SelectObject( &penDark );
         pDC->MoveTo( rect.right, rect.top );
         pDC->LineTo( rect.right, rect.bottom );
         pDC->LineTo( rect.left, rect.bottom );
      }
      else
      {
         CPen penLight( PS_SOLID, 2, ::GetSysColor( COLOR_3DHIGHLIGHT ) );

         pDC->SelectObject( &penLight );
         pDC->MoveTo( rect.right, rect.top );
         pDC->LineTo( rect.left, rect.top );
         pDC->LineTo( rect.left, rect.bottom );

         pDC->SelectObject( &penDark );
         pDC->MoveTo( rect.right, rect.top );
         pDC->LineTo( rect.right, rect.bottom );
         pDC->LineTo( rect.left, rect.bottom );

         rect.DeflateRect( 1, 1 );
      }

      pDC->SelectObject( pOldPen );
   }

   // Draw Text and image.
#if !defined( _WIN32_WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )
   if ( pDC->m_bPrinting == FALSE )
#endif
   {
      if ( pFont == 0 )
      {
         pFont = GetFontObject( );
         ASSERT( pFont );
         if ( pFont )
            pDC->SelectObject( pFont );
      }
   }

   rect.DeflateRect( GetMargin( ), 0 );

   if ( pGrid->GetImageList( ) && GetImage( ) >= 0 )
   {
      IMAGEINFO Info;
      if ( pGrid->GetImageList( )->GetImageInfo( GetImage( ), &Info ) )
      {
         // Would like to use a clipping region but seems to have issue
         // working with CMemDC directly.  Instead, don't display image
         // if any part of it is cut-off.
         //
         // CRgn rgn;
         // rgn.CreateRectRgnIndirect( rect );
         // pDC->SelectClipRgn( &rgn );
         // rgn.DeleteObject( );

         int nImageWidth = Info.rcImage.right-Info.rcImage.left + 1;
         int nImageHeight = Info.rcImage.bottom-Info.rcImage.top + 1;

         if ( nImageWidth + rect.left <= rect.right + (int) (2 * GetMargin( )) &&
              nImageHeight + rect.top <= rect.bottom + (int) (2 * GetMargin( )) )
         {
            pGrid->GetImageList( )->Draw( pDC, GetImage( ),
                                          rect.TopLeft( ), ILD_NORMAL );
         }

         // rect.left += nImageWidth + GetMargin( );
      }
   }

   int  nOffset = pDC->GetTextExtent( _T( " " ), 1 ).cx * 2;
   if ( bDrawText )
   {
      // We want to see '&' characters so use DT_NOPREFIX.
      GetTextRect( rect );
      CRect rectLabel( rect );

      // Adjust the rect further if Sort arrow is to be displayed.
      if ( pGrid->GetSortColumn( ) == nCol && lRow == 0 )
      {
         rectLabel.right -= 3 * nOffset;
      }

      DWORD dwFormat = GetFormat( );
      if ( lRow >= pGrid->m_nFixedRows && pGrid->m_pCol )
         dwFormat |= pGrid->m_pCol[ nCol ].nJustify;

   // TraceLine( "GridCellBase (DrawText) ::Draw Row: %d   Col: %d   ClrBk: 0x%08x",
   //            lRow, nCol, clrTextBk );
   // TraceRect( "GridCellBase DrawText:", rectLabel );
      DrawText( pDC->m_hDC, csText, csText.GetLength( ),
                rectLabel, dwFormat | DT_NOPREFIX );
   }

   // Draw sort arrow.
   if ( pGrid->GetSortColumn( ) == nCol && lRow == 0 )
   {
      CSize size = pDC->GetTextExtent( _T( "M" ) );
      int nOffsetY = 2;

      // Base the size of the triangle on the smaller of the column
      // height or text height with a slight offset top and bottom.
      // Otherwise, it can get drawn outside the bounds of the cell.
      size.cy -= (nOffsetY * 2);

      if ( size.cy >= rectIcon.Height( ) )
         size.cy = rectIcon.Height( ) - (nOffsetY * 2);

      size.cx = size.cy;     // make the dimensions square

      // Kludge for vertical text.
      zBOOL bVertical = (GetFont( )->lfEscapement == 900);

      // Only draw if it'll fit!
      if ( size.cx + rectIcon.left < rectIcon.right + (int) (2 * GetMargin( )) )
      {
         // Set up pens to use for drawing the triangle.
         CPen penLight( PS_SOLID, 1, ::GetSysColor( COLOR_3DHILIGHT ) );
         CPen penShadow( PS_SOLID, 1, ::GetSysColor( COLOR_3DSHADOW ) );
         CPen *pOldPen = pDC->SelectObject( &penLight );

         if ( pGrid->GetSortAscending( ) )
         {
            // Draw triangle pointing upwards.
            pDC->MoveTo( rectIcon.right - 2 * nOffset,
                         rectIcon.top + nOffset - 1 );
            pDC->LineTo( rectIcon.right - 3 * nOffset / 2,
                         rectIcon.bottom - nOffset );
            pDC->LineTo( rectIcon.right - 5 * nOffset / 2 - 2,
                         rectIcon.bottom - nOffset );
            pDC->MoveTo( rectIcon.right - 5 * nOffset / 2 - 1,
                         rectIcon.bottom - nOffset - 1 );

            pDC->SelectObject( &penShadow );
            pDC->LineTo( rectIcon.right - 2 * nOffset,
                         rectIcon.top + nOffset - 2 );
         }
         else
         {
            // Draw triangle pointing downwards.
            pDC->MoveTo( rectIcon.right - 3 * nOffset / 2,
                         rectIcon.top + nOffset - 1 );
            pDC->LineTo( rectIcon.right - 2 * nOffset - 1,
                         rectIcon.bottom - nOffset + 1 );
            pDC->MoveTo( rectIcon.right - 2 * nOffset - 1,
                         rectIcon.bottom - nOffset );

            pDC->SelectObject( &penShadow );
            pDC->LineTo( rectIcon.right - 5 * nOffset / 2 - 1,
                         rectIcon.top + nOffset - 1 );
            pDC->LineTo( rectIcon.right - 3 * nOffset / 2,
                         rectIcon.top + nOffset - 1 );
         }

         // Restore the pen.
         pDC->SelectObject( pOldPen );

         if ( bVertical == FALSE )
            rect.right -= size.cy;
      }
   }

   // Set original font and colors.
   if ( pFontOld )
   {
      pDC->SelectObject( pFontOld );
      pFont->DeleteObject( );
      delete( pFont );
   }

   pDC->SetTextColor( clrTextSave );
   pDC->RestoreDC( nSavedDC );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZGridCellBase Mouse and Cursor events

// Not yet implemented
void
ZGridCellBase::OnMouseEnter( )
{
// TRACE0( "Mouse entered cell\n" );
}

void
ZGridCellBase::OnMouseOver( )
{
// TRACE0( "Mouse over cell\n" );
}

// Not Yet Implemented
void
ZGridCellBase::OnMouseLeave( )
{
// TRACE0( "Mouse left cell\n" );
}

void
ZGridCellBase::OnClick( CPoint ptCellRelative )
{
   UNUSED_ALWAYS( ptCellRelative );
// TRACE2( "Mouse Left btn up in cell at x=%i y=%i\n",
//         ptCellRelative.x, ptCellRelative.y );
}

void
ZGridCellBase::OnClickDown( CPoint ptCellRelative )
{
   UNUSED_ALWAYS( ptCellRelative );
// TRACE2( "Mouse Left btn down in cell at x=%i y=%i\n",
//         ptCellRelative.x, ptCellRelative.y );
}

void
ZGridCellBase::OnRClick( CPoint ptCellRelative )
{
   UNUSED_ALWAYS( ptCellRelative );
// TRACE2( "Mouse right-clicked in cell at x=%i y=%i\n",
//         ptCellRelative.x, ptCellRelative.y );
}

void
ZGridCellBase::OnDblClick( CPoint ptCellRelative )
{
   UNUSED_ALWAYS( ptCellRelative );
// TRACE2( "Mouse double-clicked in cell at x=%i y=%i\n",
//         ptCellRelative.x, ptCellRelative.y );
}

// Return TRUE if you set the cursor.
BOOL
ZGridCellBase::OnSetCursor( )
{
#ifndef _WIN32_WCE_NO_CURSOR
   SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_ARROW ) );
#endif
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZGridCellBase editing

void
ZGridCellBase::OnEndEdit( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCellBase::OnEndEdit", "" );
#endif
   ASSERT( FALSE );
}

zBOOL
ZGridCellBase::ValidateEdit( zCPCHAR cpc )
{
   UNUSED_ALWAYS( cpc );
   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// ZGridCellBase Sizing

// i/o:  i=dims of cell rect; o=dims of text rect
zBOOL
ZGridCellBase::GetTextRect( CRect& rect, zBOOL bFull )
{
   ZGrid *pGrid = GetGrid( );
   if ( pGrid && GetImage( ) >= 0 )
   {
      IMAGEINFO Info;

      CImageList *pImageList = pGrid->GetImageList( );
      if ( pImageList && pImageList->GetImageInfo( GetImage( ), &Info ) )
      {
         int nImageWidth = Info.rcImage.right-Info.rcImage.left + 1;
         rect.left += nImageWidth + GetMargin( );
      }
   }

   return( TRUE );
}

// By default this uses the selected font (which is a bigger font).
CSize
ZGridCellBase::GetTextExtent( zCPCHAR szText, CDC *pDC /*= 0*/ )
{
   ZGrid *pGrid = GetGrid( );
   ASSERT( pGrid );

   zBOOL bReleaseDC = FALSE;
   if ( pDC == 0 || szText == 0 )
   {
      if ( szText )
         pDC = pGrid->GetDC( );

      if ( pDC == 0 || szText == 0 )
      {
         ZGridDefaultCell *pDefCell = (ZGridDefaultCell *) GetDefaultCell( );
         ASSERT( pDefCell );
         return( CSize( pDefCell->GetWidth( ), pDefCell->GetHeight( ) ) );
      }

      bReleaseDC = TRUE;
   }

   CFont *pOldFont = 0;
   CFont *pFont = GetFontObject( );

   if ( pFont )
      pOldFont = pDC->SelectObject( pFont );

   CSize size;
   int nFormat = GetFormat( );

   // If the cell is a multiline cell, then use the width of the cell
   // to get the height.
   if ( (nFormat & DT_WORDBREAK) && (nFormat & DT_SINGLELINE) == 0 )
   {
      CString cs = szText;
      int nMaxWidth = 0;
      while ( TRUE )
      {
         int nPos = cs.Find( _T( '\n' ) );
         CString csTemp = (nPos < 0) ? cs : cs.Left( nPos );
         int nTempWidth = pDC->GetTextExtent( csTemp ).cx;
         if ( nTempWidth > nMaxWidth )
            nMaxWidth = nTempWidth;

         if ( nPos < 0 )
            break;

         cs = cs.Mid( nPos + 1 );   // Bug fix by Thomas Steinborn
      }

      CRect rect;
      rect.SetRect( 0, 0, nMaxWidth + 1, 0 );
      pDC->DrawText( szText, -1, rect, nFormat | DT_CALCRECT );
      size = rect.Size( );
   }
   else
      size = pDC->GetTextExtent( szText, _tcslen( szText ) );

   TEXTMETRIC tm;
   pDC->GetTextMetrics( &tm );
   size.cx += (tm.tmOverhang);

   if ( pOldFont )
      pDC->SelectObject( pOldFont );

   size += CSize( 4 * GetMargin( ), 2 * GetMargin( ) );

   // Kludge for vertical text.
   LOGFONT *pLF = GetFont( );
   if ( pLF->lfEscapement == 900 || pLF->lfEscapement == -900 )
   {
      int nTemp = size.cx;
      size.cx = size.cy;
      size.cy = nTemp;
      size += CSize( 0, 4 * GetMargin( ) );
   }

   if ( bReleaseDC )
      pGrid->ReleaseDC( pDC );

   return( size );
}

CSize
ZGridCellBase::GetCellExtent( CDC *pDC, long lRow, int nCol )
{
   CString cs;

   GetText( cs, lRow, nCol );
   CSize size = GetTextExtent( cs, pDC );
   CSize ImageSize( 0, 0 );

   int nImage = GetImage( );
   if ( nImage >= 0 )
   {
      ZGrid *pGrid = GetGrid( );
      ASSERT( pGrid );

      if ( pGrid->GetImageList( ) )
      {
         IMAGEINFO Info;
         if ( pGrid->GetImageList( )->GetImageInfo( nImage, &Info ) )
         {
            ImageSize = CSize( Info.rcImage.right - Info.rcImage.left + 1,
                               Info.rcImage.bottom - Info.rcImage.top + 1 );
         }
      }
   }

   return( CSize( size.cx + ImageSize.cx, max( size.cy, ImageSize.cy ) ) );
}

// EFW - Added to print cells so that grids that use different colors are
// printed correctly.
zBOOL
ZGridCellBase::PrintCell( CDC *pDC, long lRow, int nCol, CRect rect )
{
#if defined( _WIN32_WCE_NO_PRINTING ) || defined( GRIDCONTROL_NO_PRINTING )
   return( FALSE );
#else
   COLORREF clrFg;
   COLORREF clrBk;
   GV_ITEM  Item;

   ZGrid *pGrid = GetGrid( );
   if ( pGrid == 0 || pDC == 0 )
      return( FALSE );

   if ( rect.Width( ) <= 0 || rect.Height( ) <= 0 ) // prevents imagelist item
      return( FALSE );                              // from drawing even though
                                                    // cell is hidden

   int nSavedDC = pDC->SaveDC( );

   pDC->SetBkMode( TRANSPARENT );

   if ( pGrid->GetShadedPrintOut( ) )
   {
      // Get the default cell implementation for this kind of cell. We use it
      // if this cell has anything marked as "default".
      ZGridDefaultCell *pDefaultCell = (ZGridDefaultCell *) GetDefaultCell( );
      if ( pDefaultCell == 0 )
         return( FALSE );

      // Use custom color if it doesn't match the default color and the
      // default grid background color.  If not, leave it alone.
      if ( IsFixed( ) )
      {
         clrBk = (GetBackClr( ) != CLR_DEFAULT) ?
                                GetBackClr( ) : pDefaultCell->GetBackClr( );
      }
      else
      {
         clrBk = (GetBackClr( ) != CLR_DEFAULT &&
                  GetBackClr( ) != pDefaultCell->GetBackClr( )) ?
                                                GetBackClr( ) : CLR_DEFAULT;
      }

      // Use custom color if the background is different or if it doesn't
      // match the default color and the default grid text color.
      if ( IsFixed( ) )
      {
         clrFg = (GetBackClr( ) != CLR_DEFAULT) ?
                                GetTextClr( ) : pDefaultCell->GetTextClr( );
      }
      else
      {
         clrFg = (GetBackClr( ) != CLR_DEFAULT) ?
                                GetTextClr( ) : pDefaultCell->GetTextClr( );
      }

      // If not printing on a color printer, adjust the foreground color
      // to a gray scale if the background color isn't used so that all
      // colors will be visible.  If not, some colors turn to solid black
      // or white when printed and may not show up.  This may be caused by
      // coarse dithering by the printer driver too (see image note below).
      if ( pDC->GetDeviceCaps( NUMCOLORS ) == 2 && clrBk == CLR_DEFAULT )
      {
         clrFg = RGB( GetRValue( clrFg ) * 0.30, GetGValue( clrFg ) * 0.59,
                      GetBValue( clrFg ) * 0.11 );
      }

      // Only erase the background if the color is not the default
      // grid background color.
      if ( clrBk != CLR_DEFAULT )
      {
         CBrush brush( clrBk );
         rect.right++;
         rect.bottom++;
         pDC->FillRect( rect, &brush );
         rect.right--;
         rect.bottom--;
      }
   }
   else
   {
      clrBk = CLR_DEFAULT;
      clrFg = RGB( 0, 0, 0 );
   }

   pDC->SetTextColor( clrFg );

   CFont *pFont = GetFontObject( );
   if ( pFont )
      pDC->SelectObject( pFont );

   /*
   //////////////////////////////////////////////////////////////////////////
   // Disabled - if you need this functionality then you'll need to rewrite.
   // Create the appropriate font and select into DC.
   CFont Font;
   // Bold the fixed cells if not shading the print out.  Use italic
   // font it it is enabled.
   const LOGFONT *plfFont = GetFont( );
   if ( IsFixed( ) && !pGrid->GetShadedPrintOut( ) )
   {
      Font.CreateFont( plfFont->lfHeight, 0, 0, 0, FW_BOLD,
                       plfFont->lfItalic, 0, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
#ifndef _WIN32_WCE
                       PROOF_QUALITY,
#else
                       DEFAULT_QUALITY,
#endif
                       VARIABLE_PITCH | FF_SWISS, plfFont->lfFaceName );
   }
   else
      Font.CreateFontIndirect( plfFont );

   pDC->SelectObject( &Font );
   //////////////////////////////////////////////////////////////////////////
   */

   // Draw lines only when wanted on fixed cells.  Normal cell grid lines
   // are handled in OnPrint.
   if ( pGrid->GetGridLines( ) != GVL_NONE && IsFixed( ) )
   {
      CPen penLight( PS_SOLID, 1,  ::GetSysColor( COLOR_3DHIGHLIGHT ) );
      CPen penDark( PS_SOLID,  1, ::GetSysColor( COLOR_3DDKSHADOW ) );
      CPen *pOldPen = pDC->GetCurrentPen( );

      pDC->SelectObject( &penLight );
      pDC->MoveTo( rect.right, rect.top );
      pDC->LineTo( rect.left, rect.top );
      pDC->LineTo( rect.left, rect.bottom );

      pDC->SelectObject( &penDark );
      pDC->MoveTo( rect.right, rect.top );
      pDC->LineTo( rect.right, rect.bottom );
      pDC->LineTo( rect.left, rect.bottom );

      rect.DeflateRect( 1, 1 );
      pDC->SelectObject( pOldPen );
   }

   rect.DeflateRect( GetMargin( ), 0 );

   if ( pGrid->GetImageList( ) && GetImage( ) >= 0 )
   {
      // NOTE: If your printed images look like fuzzy garbage, check the
      //       settings on your printer driver.  If it's using coarse
      //       dithering and/or vector graphics, they may print wrong.
      //       Changing to fine dithering and raster graphics makes them
      //       print properly.  My HP 4L had that problem.

      IMAGEINFO Info;
      if ( pGrid->GetImageList( )->GetImageInfo( GetImage( ), &Info ) )
      {
         int nImageWidth = Info.rcImage.right-Info.rcImage.left;
         pGrid->GetImageList( )->Draw( pDC, GetImage( ),
                                       rect.TopLeft( ), ILD_NORMAL );
         rect.left += nImageWidth+GetMargin( );
      }
   }

   // Draw without clipping so as not to lose text when printed for real
   // DT_NOCLIP removed 01.01.01. Slower, but who cares - we are printing!
   CString cs;

   GetText( cs, lRow, nCol );
   DrawText( pDC->m_hDC, cs, cs.GetLength( ), rect,
             GetFormat( ) | /*DT_NOCLIP | */ DT_NOPREFIX );

   pDC->RestoreDC( nSavedDC );

   return( TRUE );
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Callable by derived classes, only
/////////////////////////////////////////////////////////////////////////////
LRESULT
ZGridCellBase::SendMessageToParent( long lRow, int nCol, int nMessage )
{
   ZGrid *pGrid = GetGrid( );
   if ( pGrid )
      return( pGrid->SendMessageToParent( lRow, nCol, nMessage ) );

   return( 0 );
}

// GridCellCheck.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// ZInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.22+
//
// History:
// 23 Jul 2001 - Complete rewrite
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( ZGridCellCheck, ZGridCell )

ZGridCellCheck::ZGridCellCheck( ) : ZGridCell( )
{
   m_bChecked = FALSE;
// m_rect.IsRectNull( );
}

CSize
ZGridCellCheck::GetCellExtent( CDC *pDC, long lRow, int nCol )
{
   // Using SM_CXHSCROLL as a guide to the size of the checkbox.
   int nWidth = GetSystemMetrics( SM_CXHSCROLL ) + 2 * GetMargin( );
   return( ZGridCell::GetCellExtent( pDC, lRow, nCol ) +
                                                   CSize( nWidth, nWidth ) );
}

// i/o:  i=dims of cell rect; o=dims of text rect
zBOOL
ZGridCellCheck::GetTextRect( CRect& rect, zBOOL bFull )
{
   zBOOL bResult = ZGridCell::GetTextRect( rect );
   if ( bResult && bFull == FALSE )
   {
      int nWidth = GetSystemMetrics( SM_CXHSCROLL ) + 2 * GetMargin( );
      rect.left += nWidth;
      if ( rect.left > rect.right )
         rect.left = rect.right;
   }

   return( bResult );
}

// Override draw to show a checkbox in the cell.
BOOL
ZGridCellCheck::Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                      BOOL bEraseBkgnd /* = TRUE */,
                      BOOL bDrawText /* = TRUE */ )
{
// TraceLine( "GridCellCheck::Draw Row: %d   Col: %d", lRow, nCol );
   zBOOL bResult;
   ZGrid *pGrid = GetGrid( );
   if ( pGrid == 0 || pGrid->m_pCol == 0 )
      return( FALSE );

   bDrawText = pGrid->m_pCol[ nCol ].pchAttribX &&
               pGrid->m_pCol[ nCol ].pchAttribX[ 0 ] ? TRUE : FALSE;

   bResult = ZGridCell::Draw( pDC, lRow, nCol, rect,
                              bEraseBkgnd, bDrawText );

#ifndef _WIN32_WCE
   // Store the cell's dimensions for later.
   m_rect = rect;

   CRect rectCheck = GetCheckPlacement( nCol );
   rect.left = rectCheck.right;

   // Enough room to draw?
   // if ( rectCheck.Width( ) < rect.Width( ) &&
   //      rectCheck.Height( ) < rect.Height( ) )
   // {

   // Do the draw.
   UINT uState = DFCS_BUTTONCHECK /* | DFCS_FLAT */;
   if ( GetCheck( lRow, nCol ) )
      uState |= DFCS_CHECKED;

   ZCellId cellId( lRow, nCol );

   if ( pGrid->IsCellEditable( cellId ) == FALSE )
      uState |= DFCS_INACTIVE;

   pDC->DrawFrameControl( rectCheck, DFC_BUTTON, uState );

   // }
#endif
   return( bResult );
}

void
ZGridCellCheck::OnClickDown( CPoint ptCellRelative )
{
   if ( (m_pGrid->m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      return;

   if ( m_pGrid && m_pGrid->m_cellLeftClickDown == m_pGrid->m_cellIdCurrent )
      OnClick( ptCellRelative );
}

void
ZGridCellCheck::OnClick( CPoint ptCellRelative )
{
   if ( (m_pGrid->m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      return;

   // ptCellRelative is relative to the top/left of the cell. Convert to
   // client coords.
   ptCellRelative += m_rect.TopLeft( );

   // GetCheckPlacement returns the checkbox dimensions in client coords.
   // Only check/uncheck if the user clicked in the box.
   ZCellId cellId = m_pGrid->GetFocusCell( );
   if ( GetCheckPlacement( cellId.m_nCol ).PtInRect( ptCellRelative ) )
   {
      ZGrid *pGrid = m_pGrid;

      SetEditing( TRUE );
      SetCheck( !m_bChecked, cellId.m_lRow, cellId.m_nCol );
      if ( m_pGrid && mIs_hWnd( pGrid->m_hWnd ) )
         pGrid->InvalidateRect( m_rect );

#ifdef DEBUG_ALL
      TraceLineS( "ZGridCellCheck::OnClick Checked: ", m_bChecked ? "Y" : "N" );
#endif
   }
}

/////////////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////////////

zBOOL
ZGridCellCheck::SetCheck( zSHORT nChecked, long lRow, int nCol )
{
   zBOOL bCheckedPrev = m_bChecked;
   ZCellId cellId( lRow, nCol );

   if ( m_pGrid && m_pGrid->m_pCol && m_pGrid->IsCellEditable( cellId ) )
   {
      if ( nChecked < 0 )
      {
         GetCheck( lRow, nCol );
      }
      else
      {
         m_bChecked = (zBOOL) nChecked;
      }

      // GetCheck could permit messages to be processed that would delete
      // m_pGrid->m_pCol ... so we test again.
      if ( m_pGrid && m_pGrid->m_pCol && m_bChecked != bCheckedPrev )
      {
         zPCHAR pch;
         zPCHAR pchOn = m_pGrid->m_pCol[ nCol ].pchData;
         zLONG  lLth = sizeof( zLONG ) + *((zPLONG) pchOn); // skip past text
         zPCHAR pchOff = pchOn + lLth;
         pchOn += lLth + sizeof( zLONG );                   // ==> "on" text
         pchOff += 2 * sizeof( zLONG ) + *((zPLONG) pchOff);// ==> "off" text

         if ( m_rect.IsRectEmpty( ) == FALSE )
            m_pGrid->InvalidateRect( m_rect );

#ifdef DEBUG_ALL
         TraceLine( "ZGridCellCheck::SetCheck Row: %d, Col: %d  Checked: %s",
                    cellId.m_lRow, cellId.m_nCol, m_bChecked ? "Y" : "N" );
#endif

         pch = m_bChecked ? pchOn : pchOff;
         SetText( pch, lRow, nCol );
         if ( nChecked >= 0 )
         {
            m_pGrid->SendMessageToParent( lRow, nCol, GVN_ODSTATECHANGED );
         }
      }
   }

   return( bCheckedPrev );
}

zBOOL
ZGridCellCheck::GetCheck( long lRow, int nCol )
{
   if ( m_pGrid && m_pGrid->m_pCol && mIs_hWnd( m_pGrid->m_hWnd ) )
   {
      CString csText;
      zPCHAR pchOn = m_pGrid->m_pCol[ nCol ].pchData;
      zLONG  lLth = sizeof( zLONG ) + *((zPLONG) pchOn); // skip past text
      pchOn += lLth + sizeof( zLONG );                   // ==> "on" text

      GetText( csText, lRow, nCol );

      // GetText could permit messages to be processed that would delete
      // m_pGrid->m_pCol ... so we test again.
      if ( m_pGrid && m_pGrid->m_pCol &&
           m_pGrid->m_pCol[ nCol ].pchAttribX &&
           m_pGrid->m_pCol[ nCol ].pchAttribX[ 0 ] )
      {
         zCHAR szMap[ 65534 ];

         // Position was established by GetText call above.
         GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                   zsizeof( szMap ) - 1,
                                   m_pGrid->m_vAppList,
                                   m_pGrid->m_pCol[ nCol ].pchEntity,
                                   m_pGrid->m_pCol[ nCol ].pchAttrib,
                                   m_pGrid->m_pCol[ nCol ].pchContext,
                                   (m_pGrid->m_pCol[ nCol ].pchContext &&
                                    m_pGrid->m_pCol[ nCol ].pchContext[ 0 ]) ?
                                zACCEPT_NULL_ENTITY :
                 zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );
         csText = szMap;
      }

      m_bChecked = (zstrcmp( pchOn, csText.GetString() ) == 0) ? TRUE : FALSE;
   }
   else
      m_bChecked = FALSE;

   return( m_bChecked );
}

BOOL
ZGridCellCheck::Edit( long lRow, int nCol, CRect rect,
                      CPoint pt, UINT uId, UINT uChar )
{
#ifdef DEBUG_ALL
   TraceLine( "ZGridCellCheck::Edit Row: %d, Col: %d", lRow, nCol );
#endif
   SetEditing( TRUE );
   m_pGrid->SetFocusCell( lRow, nCol );
   return( TRUE );
}

void
ZGridCellCheck::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCellCheck::EndEdit", "" );
#endif
#if 0
   if ( m_pGrid && m_pGrid->m_pCol && mIs_hWnd( m_pGrid->m_hWnd ) )
   {
      ZCellId cellId = m_pGrid->GetFocusCell( );

      TraceLine( "ZGridCellCheck::EndEdit Row: %d, Col: %d  Checked: %s",
                 cellId.m_lRow, cellId.m_nCol, m_bChecked ? "Y" : "N" );

      zPCHAR pch = m_pGrid->m_pCol[ cellId.m_nCol ].pchData;
      zLONG lLth = sizeof( zLONG ) + *((zPLONG) pch); // skip past Text

      if ( GetCheck( cellId.m_lRow, cellId.m_nCol ) == FALSE )
         lLth += sizeof( zLONG ) + *((zPLONG) (pch + lLth)); // skip ON

      pch += lLth + sizeof( zLONG );
      SetText( pch, cellId.m_lRow, cellId.m_nCol );
   }
#endif

   ZGridCell::EndEdit( bDestroy );
}

// virtual
BOOL
ZGridCellCheck::WantKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   if ( uChar == VK_SPACE )
   {
      ZCellId cellId = m_pGrid->GetFocusCell( );
      CRect rect = GetCheckPlacement( cellId.m_nCol );
      CPoint pt( (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 );
      pt -= m_rect.TopLeft( );

      OnClick( pt );
      return( TRUE );
   }

   return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// Protected implementation
/////////////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the checkbox in client coords.
CRect
ZGridCellCheck::GetCheckPlacement( int nCol )
{
   int   nWidth = GetSystemMetrics( SM_CXHSCROLL ) - 2;
   CRect rectPlace = m_rect + CSize( GetMargin( ), GetMargin( ) - 1 );
   int   nDiff;

   // Check for centering.
   ZGrid *pGrid = GetGrid( );
   zBOOL bLeftJustify = pGrid->m_pCol[ nCol ].pchAttribX &&
                        pGrid->m_pCol[ nCol ].pchAttribX[ 0 ] ? TRUE : FALSE;

   rectPlace.bottom = rectPlace.top + nWidth;
   if ( bLeftJustify )
   {
      rectPlace.right = rectPlace.left + nWidth;
   }
   else
   {
      // Center horizontally.
      nDiff = (rectPlace.Width( ) - nWidth) / 2;
      if ( nDiff > 0 )
      {
        rectPlace.left += nDiff;
        rectPlace.right = rectPlace.left + nWidth;
      }

   }

   // Center vertically.
   nDiff = (rectPlace.Height( ) - nWidth) / 2;
   if ( nDiff > 0 )
   {
     rectPlace.top += nDiff;
     rectPlace.bottom = rectPlace.top + nWidth;
   }

   return( rectPlace );
}

// GridCellCombo.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2001. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// ZInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.22+
//
// History:
// 6 Aug 1998 - Added ZSS_ComboEdit to subclass the edit control - code
//           provided by Roelf Werkman <rdw@inn.nl>. Added uId to
//           the constructor param list.
// 29 Nov 1998 - bug fix in onkeydown (Markus Irtenkauf)
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZSS_ComboEdit

ZSS_ComboEdit::ZSS_ComboEdit( )
{
}

ZSS_ComboEdit::~ZSS_ComboEdit( )
{
}

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL
ZSS_ComboEdit::PreTranslateMessage( MSG *pMsg )
{
   // Make sure that the keystrokes continue to the appropriate handlers
   if ( pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP )
   {
      ::TranslateMessage( pMsg );
      ::DispatchMessage( pMsg );
      return( TRUE );
   }

   // Catch the Alt key so we don't choke if focus is going to an owner
   // drawn button.
   if ( pMsg->message == WM_SYSCHAR )
      return( TRUE );

   return( CEdit::PreTranslateMessage( pMsg ) );
}

BEGIN_MESSAGE_MAP( ZSS_ComboEdit, CEdit )
   //{{AFX_MSG_MAP( ZSS_ComboEdit )
   ON_WM_KILLFOCUS( )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZSS_ComboEdit message handlers

void
ZSS_ComboEdit::OnKillFocus( CWnd *pNewWnd )
{
   CEdit::OnKillFocus( pNewWnd );

   ZInPlaceList *pOwner = (ZInPlaceList *) GetOwner( );  // MUST be a ZInPlaceList
   if ( pOwner )
      pOwner->EndEdit( );
}

void
ZSS_ComboEdit::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLineX( "ZSS_ComboEdit::OnKeyDown uFlags: ", uFlags );
   if ( (uChar == VK_PRIOR || uChar == VK_NEXT ||
         uChar == VK_DOWN  || uChar == VK_UP   ||
         uChar == VK_RIGHT || uChar == VK_LEFT) &&
        (GetKeyState( VK_CONTROL ) < 0 && GetDlgCtrlID( ) == IDC_COMBOEDIT) )
   {
      CWnd *pOwner = GetOwner( );
      if ( pOwner )
      {
         pOwner->SendMessage( WM_KEYDOWN, uChar,
                              uRepCnt + (((DWORD) uFlags) << 16));
      }

      return;
   }

   CEdit::OnKeyDown( uChar, uRepCnt, uFlags );
}

void
ZSS_ComboEdit::OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   if ( uChar == VK_ESCAPE )
   {
      CWnd *pOwner = GetOwner( );
      if ( pOwner )
      {
         pOwner->SendMessage( WM_KEYUP, uChar,
                              uRepCnt + (((DWORD) uFlags) << 16));
      }

      return;
   }

   if ( uChar == VK_TAB || uChar == VK_RETURN || uChar == VK_ESCAPE )
   {
      CWnd *pOwner = GetOwner( );
      if ( pOwner )
      {
         pOwner->SendMessage( WM_KEYUP, uChar,
                              uRepCnt + (((DWORD) uFlags) << 16));
      }

      return;
   }

   CEdit::OnKeyUp( uChar, uRepCnt, uFlags );
}

/////////////////////////////////////////////////////////////////////////////
// ZInPlaceList

ZInPlaceList::ZInPlaceList( CWnd *pParent, CRect& rect, DWORD dwStyle,
                            UINT uId, long lRow, int nCol,
                            COLORREF clrFg, COLORREF clrBk,
                            CStringArray& Items, CString csInitText,
                            UINT uFirstChar )
{
   m_clrFg = clrFg;
   m_clrBk = clrBk;

   m_nNbrLines = 4;
   m_csInitText = csInitText;
   m_lRow     = lRow;
   m_nCol     = nCol;
   m_nLastChar = 0;
   m_bExitOnArrows = FALSE; // (uFirstChar != VK_LBUTTON); // If mouse click brought us here,

   // Create the combobox.
   DWORD dwComboStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL |
                        CBS_AUTOHSCROLL | dwStyle;
   int nHeight = rect.Height( );
   rect.bottom = rect.bottom + m_nNbrLines * nHeight +
                                          ::GetSystemMetrics( SM_CYHSCROLL );
   if ( Create( dwComboStyle, rect, pParent, uId ) == 0 )
      return;

   // Add the strings
   for ( int k = 0; k < Items.GetSize( ); k++ )
     AddString( Items[ k ] );

   SetFont( pParent->GetFont( ) );
   SetItemHeight( -1, nHeight );

   int nMaxLength = GetCorrectDropWidth( );
   /*
   if ( nMaxLength > rect.Width( ) )
      rect.right = rect.left + nMaxLength;

   // Resize the edit window and the drop down window
   MoveWindow( rect );
   */

   SetDroppedWidth( nMaxLength );

   SetHorizontalExtent( 0 ); // no horz scrolling

   // Set the initial text to m_csInitText
   if ( mIs_hWnd( m_hWnd ) && SelectString( -1, m_csInitText ) == CB_ERR )
     SetWindowText( m_csInitText ); // no text selected, so restore what
                                    // was there before

   ShowDropDown( );

   // Subclass the combobox edit control if style includes CBS_DROPDOWN.
   if ( (dwStyle & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST )
   {
      m_edit.SubclassDlgItem( IDC_COMBOEDIT, this );
      SetFocus( );
      switch ( uFirstChar )
      {
         case VK_LBUTTON:
         case VK_RETURN:
            m_edit.SetSel( (int) _tcslen( m_csInitText ), -1 );
            return;

         case VK_BACK:
            m_edit.SetSel( (int) _tcslen( m_csInitText ), -1 );
            break;

         case VK_DOWN:
         case VK_UP:
         case VK_RIGHT:
         case VK_LEFT:
         case VK_NEXT:
         case VK_PRIOR:
         case VK_HOME:
         case VK_END:
            m_edit.SetSel( 0, -1 );
            return;

         default:
            m_edit.SetSel( 0, -1 );
      }

      SendMessage( WM_CHAR, uFirstChar );
   }
   else
      SetFocus( );
}

ZInPlaceList::~ZInPlaceList( )
{
}

void
ZInPlaceList::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZInPlaceList::EndEdit", "" );
#endif
   CString cs;
   if ( mIs_hWnd( m_hWnd ) )
   {
      GetWindowText( cs );
      SetWindowText( "" );
   }

   // Send Notification to parent
   GV_DISPINFO dispinfo;

   dispinfo.hdr.hwndFrom = GetSafeHwnd( );
   dispinfo.hdr.idFrom = GetDlgCtrlID( );
   dispinfo.hdr.code = GVN_ENDEDIT;

   dispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;
   dispinfo.item.lRow = m_lRow;
   dispinfo.item.nCol = m_nCol;
   dispinfo.item.csText = cs;
   dispinfo.item.lItemExtra = (LPARAM) m_nLastChar;

   CWnd *pOwner = GetOwner( );
   if ( mIs_hWnd( pOwner->GetSafeHwnd( ) ) )
      pOwner->SendMessage( WM_NOTIFY, GetDlgCtrlID( ), (LPARAM) &dispinfo );

   // Close this window (PostNcDestroy will delete this)
   if ( bDestroy && mIs_hWnd( GetSafeHwnd( ) ) )
      PostMessage( WM_CLOSE, 0, 0 );
}

int
ZInPlaceList::GetCorrectDropWidth( )
{
   const int nMaxWidth = 200;  // don't let the box be bigger than this

   // Reset the dropped width.
   int nNbrEntries = GetCount( );
   int nWidth = 0;
   CString cs;

   CClientDC dc( this );
   int nSave = dc.SaveDC( );
   dc.SelectObject( GetFont( ) );

   int nScrollWidth = ::GetSystemMetrics( SM_CXVSCROLL );
   for ( int k = 0; k < nNbrEntries; k++ )
   {
      GetLBText( k, cs );
      int nLength = dc.GetTextExtent( cs ).cx + nScrollWidth;
      nWidth = max( nWidth, nLength );
   }

   // Add margin space to the calculations.
   nWidth += dc.GetTextExtent( "0" ).cx;

   dc.RestoreDC( nSave );

   nWidth = min( nWidth, nMaxWidth );

   return( nWidth );
   //SetDroppedWidth( nWidth );
}

/*
// Fix by Ray (raybie@Exabyte.COM)
void
ZInPlaceList::OnSelendOK( )
{
   int nIndex = GetCurSel( );
   if ( nIndex != CB_ERR )
   {
      CString csLbText;
      GetLBText( nIndex, csLbText );

      if ( !((GetStyle( ) & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST) )
         m_edit.SetWindowText( csLbText );
   }

   GetParent( )->SetFocus( );
}
*/

void
ZInPlaceList::PostNcDestroy( )
{
   CComboBox::PostNcDestroy( );

   delete( this );
}

BEGIN_MESSAGE_MAP( ZInPlaceList, CComboBox )
   //{{AFX_MSG_MAP( ZInPlaceList )
   ON_WM_KILLFOCUS( )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   ON_CONTROL_REFLECT( CBN_DROPDOWN, OnDropdown )
   ON_WM_GETDLGCODE( )
   ON_WM_CTLCOLOR_REFLECT( )
   //}}AFX_MSG_MAP
   //ON_CONTROL_REFLECT( CBN_SELENDOK, OnSelendOK )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZInPlaceList message handlers

UINT
ZInPlaceList::OnGetDlgCode( )
{
   return( DLGC_WANTALLKEYS );
}

void
ZInPlaceList::OnDropdown( )
{
   SetDroppedWidth( GetCorrectDropWidth( ) );
}

void
ZInPlaceList::OnKillFocus( CWnd *pNewWnd )
{
   CComboBox::OnKillFocus( pNewWnd );

   if ( GetSafeHwnd( ) == pNewWnd->GetSafeHwnd( ) )
      return;

   // Only end editing on change of focus if we're using the
   // CBS_DROPDOWNLIST style.
   if ( (GetStyle( ) & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST )
      EndEdit( );
}

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void
ZInPlaceList::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLineX( "ZInPlaceList::OnKeyDown uFlags: ", uFlags );
   if ( (uChar == VK_PRIOR || uChar == VK_NEXT ||
         uChar == VK_DOWN  || uChar == VK_UP   ||
         uChar == VK_RIGHT || uChar == VK_LEFT ) &&
        (m_bExitOnArrows || GetKeyState( VK_CONTROL ) < 0) )
   {
      m_nLastChar = uChar;
      GetParent( )->SetFocus( );
      return;
   }

   CComboBox::OnKeyDown( uChar, uRepCnt, uFlags );
}

// Need to keep a lookout for Tabs, Esc and Returns.
void
ZInPlaceList::OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   if ( uChar == VK_ESCAPE )
     SetWindowText( m_csInitText );   // restore previous text

   if ( uChar == VK_TAB || uChar == VK_RETURN || uChar == VK_ESCAPE )
   {
     m_nLastChar = uChar;
     GetParent( )->SetFocus( );   // This will destroy this window
     return;
   }

   CComboBox::OnKeyUp( uChar, uRepCnt, uFlags );
}

HBRUSH
ZInPlaceList::CtlColor( CDC *pDC, UINT uCtlColor )
{
   /*
   static CBrush brush( m_clrBackClr );
   pDC->SetTextColor( m_clrForeClr );
   pDC->SetBkMode( TRANSPARENT );
   return( (HBRUSH) brush.GetSafeHandle( ) );
   */

   // TODO: Return a non-zero brush if the parent's handler should not be called
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// ZGridCellCombo
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( ZGridCellCombo, ZGridCell )

ZGridCellCombo::ZGridCellCombo( ) : ZGridCell( )
{
   SetStyle( CBS_DROPDOWN );  // CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE, CBS_SORT
}

// Create a control to do the editing.
BOOL
ZGridCellCombo::Edit( long lRow, int nCol, CRect rect,
                      CPoint pt, UINT uId, UINT uChar )
{
#ifdef DEBUG_ALL
   TraceLine( "ZGridCellCombo::Edit Row: %d, Col: %d", lRow, nCol );
#endif
   CString cs;
   SetEditing( TRUE );

   // ZInPlaceList auto-deletes itself.
   GetText( cs, lRow, nCol );
   ZGrid *pGrid = GetGrid( );
   m_pEditWnd = new ZInPlaceList( pGrid, rect, GetStyle( ),
                                  uId, lRow, nCol,
                                  GetTextClr( ), GetBackClr( ),
                                  m_Strings, cs, uChar );
   pGrid->m_pZeidonWnd = m_pEditWnd;
   return( TRUE );
}

CWnd *
ZGridCellCombo::GetEditWnd( ) const
{
   if ( m_pEditWnd &&
        (m_pEditWnd->GetStyle( ) & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST )
   {
     return( &(((ZInPlaceList *) m_pEditWnd)->m_edit) );
   }

   return( 0 );
}

CSize
ZGridCellCombo::GetCellExtent( CDC *pDC, long lRow, int nCol )
{
   CSize sizeScroll( GetSystemMetrics( SM_CXVSCROLL ),
                     GetSystemMetrics( SM_CYHSCROLL ) );

   return( ZGridCell::GetCellExtent( pDC, lRow, nCol ) + sizeScroll );
}

// Cancel the editing.
void
ZGridCellCombo::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCellCombo::EndEdit", "" );
#endif
   if ( m_pEditWnd )
      ((ZInPlaceList *) m_pEditWnd)->EndEdit( bDestroy );
}

// Override draw so that when the cell is selected, a drop arrow is shown in
// the RHS.
BOOL
ZGridCellCombo::Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                      BOOL bEraseBkgnd /*=TRUE*/, BOOL bDrawText /*=TRUE*/ )
{
// TraceLine( "GridCellCombo::Draw Row: %d   Col: %d", lRow, nCol );
#ifdef _WIN32_WCE
   return( ZGridCell::Draw( pDC, lRow, nCol, rect, bEraseBkgnd, bDrawText ) );
#else
   // Cell selected?
   //if ( !IsFixed( ) && IsFocused( ) )
   if ( GetGrid( )->IsCellEditable( lRow, nCol ) && IsEditing( ) == FALSE )
   {
      // Get the size of the scroll box.
      CSize sizeScroll( GetSystemMetrics( SM_CXVSCROLL ),
                        GetSystemMetrics( SM_CYHSCROLL ) );

      // Enough room to draw?
      if ( sizeScroll.cy < rect.Width( ) && sizeScroll.cy < rect.Height( ) )
      {
         // Draw control at RHS of cell
         CRect ScrollRect = rect;
         ScrollRect.left = rect.right - sizeScroll.cx;
         ScrollRect.bottom = rect.top + sizeScroll.cy;

         // Do the draw
         pDC->DrawFrameControl( ScrollRect, DFC_SCROLL, DFCS_SCROLLDOWN );

         // Adjust the remaining space in the cell
         rect.right = ScrollRect.left;
      }
   }

   CString cs;

   GetText( cs, lRow, nCol );
   if ( IsEditing( ) )
      SetText( _T( "" ), lRow, nCol );

   // Drop through and complete the cell drawing using the base class method.
   zBOOL bResult = ZGridCell::Draw( pDC, lRow, nCol, rect, bEraseBkgnd );

   if ( IsEditing( ) )
      SetText( cs, lRow, nCol );

   return( bResult );
#endif
}

// For setting the strings that will be displayed in the drop list.
void
ZGridCellCombo::SetOptions( CStringArray& ar )
{
   m_Strings.RemoveAll( );
   for ( int k = 0; k < ar.GetSize( ); k++ )
      m_Strings.Add( ar[ k ] );
}

///////////////////////////////////////////////////////////////////////////
//
// GridCellDateTime.cpp: implementation of the ZGridCellDateTime class.
//
// Provides the implementation for a datetime picker cell type of the
// grid control.
//
// Written by Podsypalnikov Eugen 15 Mar 2001
// Modified:
//   31 May 2001  Fixed m_cTime bug (Chris Maunder)
//
// For use with ZGrid v2.22+
//
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// ZGridCellDateTime

IMPLEMENT_DYNCREATE( ZGridCellDateTime, ZGridCell )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZGridCellDateTime::ZGridCellDateTime( ) : ZGridCell( )
{
   m_dwStyle = 0;
   m_time = CTime::GetCurrentTime( );
}

ZGridCellDateTime::ZGridCellDateTime( DWORD dwStyle ) : ZGridCell( )
{
   Init( dwStyle );
}

ZGridCellDateTime::~ZGridCellDateTime( )
{
}

BOOL
ZGridCellDateTime::Edit( long lRow, int nCol, CRect rect, CPoint pt,
                         UINT uId, UINT uChar )
{
#ifdef DEBUG_ALL
   TraceLine( "ZGridCellDateTime::Edit Row: %d, Col: %d", lRow, nCol );
#endif
   SetEditing( TRUE );

   // ZInPlaceDateTime auto-deletes itself
   ZGrid *pGrid = GetGrid( );
   m_pEditWnd = new ZInPlaceDateTime( pGrid, rect,
                                      m_dwStyle | DTS_UPDOWN,
                                      uId, lRow, nCol,
                                      GetTextClr( ), GetBackClr( ),
                                      GetTime( ), uChar );
   pGrid->m_pZeidonWnd = m_pEditWnd;
   return( TRUE );
}

CWnd *
ZGridCellDateTime::GetEditWnd( ) const
{
   return( m_pEditWnd );
}

void
ZGridCellDateTime::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCellDateTime::EndEdit", "" );
#endif
   if ( m_pEditWnd )
      ((ZInPlaceDateTime *) m_pEditWnd)->EndEdit( bDestroy );
}

void
ZGridCellDateTime::Init( DWORD dwStyle )
{
   m_dwStyle = dwStyle;

   long lRow = 0;
   int  nCol = 0;

   ASSERT( FALSE );  // need to get the correct row and column if we use this

   SetTime( CTime::GetCurrentTime( ), lRow, nCol );

#ifdef _WIN32_WCE
   SetFormat( DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX );
#else
   SetFormat( DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX |
                DT_END_ELLIPSIS );
#endif

}

// Should be changed to use locale settings.
void
ZGridCellDateTime::SetTime( CTime time, long lRow, int nCol )
{
   m_time = time;
   lRow = 0;
   nCol = 0;

   ASSERT( FALSE );  // need to get the correct row and column if we use this

   if ( DTS_TIMEFORMAT == m_dwStyle )
   {
#ifdef _WIN32_WCE
     CString csTemp;
     csTemp.Format( _T( "%02d:%02d:%02d" ),
                    m_time.GetHour( ),
                    m_time.GetMinute( ),
                    m_time.GetSecond( ) );
     SetText( csTemp, lRow, nCol );
#else
     SetText( m_time.Format( _T( "%H:%M:%S" ) ), lRow, nCol );
#endif
   }
   else
   if ( DTS_SHORTDATEFORMAT == m_dwStyle )
   {
#ifdef _WIN32_WCE
     CString csTemp;
     csTemp.Format( _T( "%02d/%02d/%02d" ),
                    m_time.GetMonth( ),
                    m_time.GetDay( ),
                    m_time.GetYear( ) );
     SetText( csTemp, lRow, nCol );
#else
     SetText( m_time.Format( _T( "%d/%m/%Y" ) ), lRow, nCol );
#endif
   }
}

/////////////////////////////////////////////////////////////////////////////
// ZInPlaceDateTime
//
ZInPlaceDateTime::ZInPlaceDateTime( CWnd *pParent, CRect& rect,
                                    DWORD dwStyle, UINT uId,
                                    long lRow, int nCol,
                                    COLORREF clrFg, COLORREF clrBk,
                                    CTime *pTime,
                                    UINT uFirstChar )
{
   m_clrFg = clrFg;
   m_clrBk = clrBk;
   m_lRow = lRow;
   m_nCol = nCol;
   m_nLastChar = 0;
   m_bExitOnArrows = FALSE;
   m_pTime = pTime;

   DWORD dwStl = WS_BORDER | WS_VISIBLE | WS_CHILD | dwStyle;

   if ( Create( dwStl, rect, pParent, uId ) == 0 )
   {
      return;
   }

   SetTime( m_pTime );

   SetFont( pParent->GetFont( ) );
   SetFocus( );

   switch ( uFirstChar )
   {
      case VK_LBUTTON:
      case VK_RETURN:
         return;

      case VK_BACK:
         break;

      case VK_DOWN:
      case VK_UP:
      case VK_RIGHT:
      case VK_LEFT:
      case VK_NEXT:
      case VK_PRIOR:
      case VK_HOME:
      case VK_END:
         return;

      default:
         break;
   }

   SendMessage( WM_CHAR, uFirstChar );
}

ZInPlaceDateTime::~ZInPlaceDateTime( )
{
}

void
ZInPlaceDateTime::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZInPlaceDateTime::EndEdit", "" );
#endif
   CString cs;
   if ( mIs_hWnd( m_hWnd ) )
   {
      GetWindowText( cs );
      GetTime( *m_pTime );
   }

   // Send Notification to parent.
   GV_DISPINFO dispinfo;

   dispinfo.hdr.hwndFrom = GetSafeHwnd( );
   dispinfo.hdr.idFrom   = GetDlgCtrlID( );
   dispinfo.hdr.code     = GVN_ENDEDIT;

   dispinfo.item.mask    = LVIF_TEXT | LVIF_PARAM;
   dispinfo.item.lRow    = m_lRow;
   dispinfo.item.nCol    = m_nCol;
   dispinfo.item.csText  = cs;
   dispinfo.item.lItemExtra  = (LPARAM) m_nLastChar;

   CWnd *pOwner = GetOwner( );
   if ( mIs_hWnd( pOwner->GetSafeHwnd( ) ) )
      pOwner->SendMessage( WM_NOTIFY, GetDlgCtrlID( ), (LPARAM) &dispinfo );

   // Close this window (PostNcDestroy will delete( this )).
   if ( bDestroy && mIs_hWnd( m_hWnd ) )
      PostMessage( WM_CLOSE, 0, 0 );
}

void
ZInPlaceDateTime::PostNcDestroy( )
{
   CDateTimeCtrl::PostNcDestroy( );
   delete( this );
}

BEGIN_MESSAGE_MAP( ZInPlaceDateTime, CDateTimeCtrl )
   //{{AFX_MSG_MAP( ZInPlaceDateTime )
   ON_WM_KILLFOCUS( )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   ON_WM_GETDLGCODE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )


/////////////////////////////////////////////////////////////////////////////
// ZInPlaceDateTime message handlers

void
ZInPlaceDateTime::OnKillFocus( CWnd *pNewWnd )
{
   CDateTimeCtrl::OnKillFocus( pNewWnd );

   if ( GetSafeHwnd( ) == pNewWnd->GetSafeHwnd( ) )
   {
     return;
   }

   EndEdit( );
}

UINT
ZInPlaceDateTime::OnGetDlgCode( )
{
   return( DLGC_WANTALLKEYS );
}

void
ZInPlaceDateTime::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// TraceLineX( "ZInPlaceDateTime::OnKeyDown uFlags: ", uFlags );
   if ( (uChar == VK_PRIOR || uChar == VK_NEXT ||
         uChar == VK_DOWN  || uChar == VK_UP   ||
         uChar == VK_RIGHT || uChar == VK_LEFT ) &&
        (m_bExitOnArrows || GetKeyState( VK_CONTROL ) < 0) )
   {
      m_nLastChar = uChar;
      GetParent( )->SetFocus( );
      return;
   }

   CDateTimeCtrl::OnKeyDown( uChar, uRepCnt, uFlags );
}

void
ZInPlaceDateTime::OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   if ( uChar == VK_TAB || uChar == VK_RETURN || uChar == VK_ESCAPE )
   {
      m_nLastChar = uChar;
      GetParent( )->SetFocus( );   // This will destroy this window
      return;
   }

   CDateTimeCtrl::OnKeyUp( uChar, uRepCnt, uFlags );
}

// GridCellNumeric.cpp: implementation of the ZGridCellNumeric class.
//
// Written by Andrew Truckle [ajtruckle@wsatkins.co.uk]
//
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( ZGridCellNumeric, ZGridCell )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Create a control to do the editing.
BOOL
ZGridCellNumeric::Edit( long lRow, int nCol, CRect rect,
                        CPoint pt, UINT uId, UINT uChar )
{
#ifdef DEBUG_ALL
   TraceLine( "ZGridCellNumeric::Edit Row: %d, Col: %d", lRow, nCol );
#endif
   CString cs;
   SetEditing( TRUE );

   // CInPlaceEdit auto-deletes itself.
   GetText( cs, lRow, nCol );
   ZGrid *pGrid = GetGrid( );
   m_pEditWnd = new CInPlaceEdit( pGrid, rect,
                                  /*GetStyle( ) |*/ ES_NUMBER,
                                  uId, lRow, nCol, cs, uChar );
   pGrid->m_pZeidonWnd = m_pEditWnd;
   return( TRUE );
}

// Cancel the editing.
void
ZGridCellNumeric::EndEdit( BOOL bDestroy )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGridCellNumeric::EndEdit", "" );
#endif
   if ( m_pEditWnd )
      ((CInPlaceEdit *) m_pEditWnd)->EndEdit( bDestroy );
}

//////////////////////////////////////////////////////////////////////
// GridURLCell.cpp: implementation of the ZGridURLCell class.
//

IMPLEMENT_DYNCREATE( ZGridURLCell, ZGridCell )

#ifndef _WIN32_WCE
HCURSOR ZGridURLCell::g_hLinkCursor = 0;
#endif

// Possible prefixes that indicate a hyperlink.
URLStruct ZGridURLCell::g_szURIprefixes[ ] =
{
   { _T( "www." ), (int) _tcslen( _T( "www." ) ) },
   { _T( "http:" ), (int) _tcslen( _T( "http:" ) ) },
   { _T( "mailto:" ), (int) _tcslen( _T( "mailto:" ) ) },
   { _T( "ftp:" ), (int) _tcslen( _T( "ftp:" ) ) },
   { _T( "https:" ), (int) _tcslen( _T( "https:" ) ) },
   { _T( "news:" ), (int) _tcslen( _T( "news:" ) ) },
   { _T( "gopher:" ), (int) _tcslen( _T( "gopher:" ) ) },
   { _T( "telnet:" ), (int) _tcslen( _T( "telnet:" ) ) },
   { _T( "url:" ), (int) _tcslen( _T( "url:" ) ) },
   { _T( "file:" ), (int) _tcslen( _T( "file:" ) ) },
   { _T( "ftp." ), (int) _tcslen( _T( "ftp." ) ) }
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZGridURLCell::ZGridURLCell( )
{
#ifndef _WIN32_WCE
   g_hLinkCursor = GetHandCursor( );
#endif
   m_bLaunchUrl = TRUE;
   m_clrUrl = GetSysColor( COLOR_HIGHLIGHT );
}

ZGridURLCell::~ZGridURLCell( )
{
}

BOOL
ZGridURLCell::Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                    BOOL bEraseBkgnd, BOOL bDrawText /*=TRUE*/ )
{
// TraceLine( "GridURLCell::Draw Row: %d   Col: %d", lRow, nCol );
   CString cs;

   // If url is present then change text color.
   GetText( cs, lRow, nCol );
   if ( HasUrl( cs ) )
     SetTextClr( m_clrUrl );

   // Good a place as any to store the bounds of the rect.
   m_rect = rect;

   return( ZGridCell::Draw( pDC, lRow, nCol, rect, bEraseBkgnd, bDrawText ) );
}

#pragma warning( disable:4100 )
BOOL
ZGridURLCell::Edit( long lRow, int nCol, CRect rect,
                    CPoint pt, UINT uId, UINT uChar )
{
#ifdef DEBUG_ALL
   TraceLine( "ZGridURLCell::Edit Row: %d, Col: %d", lRow, nCol );
#endif
   return( FALSE );
}
#pragma warning( default:4100 )

void
ZGridURLCell::OnClick( CPoint ptCellRelative )
{
#ifndef _WIN32_WCE
   CString csURL;
   if ( GetAutoLaunchUrl( ) && OverURL( ptCellRelative, csURL ) )
      ShellExecute( 0, _T( "open" ), csURL, 0, 0, SW_SHOW );
#endif
}

// Return TRUE if you set the cursor.
BOOL
ZGridURLCell::OnSetCursor( )
{
#ifndef _WIN32_WCE
   CString csURL;
   CPoint pt( GetMessagePos( ) );
   GetGrid( )->ScreenToClient( &pt );
   pt = pt - m_rect.TopLeft( );

   if ( OverURL( pt, csURL ) )
   {
      SetCursor( g_hLinkCursor );
      return( TRUE );
   }
   else
#endif
      return( ZGridCell::OnSetCursor( ) );
}

#ifndef _WIN32_WCE
HCURSOR
ZGridURLCell::GetHandCursor( )
{
   if ( g_hLinkCursor == 0 )   // No cursor handle - load our own
   {
      // Get the windows directory
      CString csWndDir;
      GetWindowsDirectory( csWndDir.GetBuffer( MAX_PATH ), MAX_PATH );
      csWndDir.ReleaseBuffer( );

      csWndDir += _T( "\\winhlp32.exe" );
      // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
      HMODULE hModule = LoadLibrary( csWndDir );
      if ( hModule )
      {
         HCURSOR hHandCursor = ::LoadCursor( hModule, MAKEINTRESOURCE( 106 ) );
         if ( hHandCursor )
         {
            g_hLinkCursor = CopyCursor( hHandCursor );
         }
      }

      FreeLibrary( hModule );
   }

   return( g_hLinkCursor );
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Helper functions

zBOOL
ZGridURLCell::HasUrl( CString cs )
{
   int nNbrPrefixes = zsizeof( g_szURIprefixes ) / sizeof( g_szURIprefixes[ 0 ] );
   for ( int k = 0; k < nNbrPrefixes; k++ )
   {
      // if ( cs.Left( g_szURIprefixes[ k ].nLength ) ==
      //                                   g_szURIprefixes[ k ].szURLPrefix )
      if ( cs.Find( g_szURIprefixes[ k ].szURLPrefix ) >= 0 )
         return( TRUE );
   }

   return( FALSE );
}

// Here we figure out if we are over a URL or not.
zBOOL
ZGridURLCell::OverURL( CPoint& pt, CString& csURL )
{
   // TRACE2( "Checking point %d,%d\n",pt.x,pt.y );
   CString cs;
   zBOOL bOverURL = FALSE;
   ZCellId cellId = m_pGrid->GetCellFromPt( pt );

   GetText( cs, cellId.m_lRow, cellId.m_nCol );
   CSize size = GetTextExtent( cs );

   // Add left of cell so we know if we clicked on text or not.
   pt.x += m_rect.left;
   CPoint center = m_rect.CenterPoint( );

   if ( (m_dwFormat & DT_RIGHT) && pt.x >= (m_rect.right - size.cx) )
   {
      bOverURL = TRUE;
   }
   else
   if ( (m_dwFormat & DT_CENTER) &&
        ((center.x - (size.cx / 2)) <= pt.x) &&
        (pt.x <= (center.x + (size.cx / 2))))
   {
      bOverURL = TRUE;
   }
   else
   if ( pt.x <= (size.cx + m_rect.left) )
   {
      bOverURL = TRUE;
   }

   if ( bOverURL == FALSE )
      return( FALSE );

   // We are over text - but are we over a URL?
   bOverURL = FALSE;
   GetText( csURL, cellId.m_lRow, cellId.m_nCol );

   // Use float, otherwise we get an incorrect letter from the point.
   float width = (float) size.cx / (float) csURL.GetLength( );

   // Remove left of cell so we have original point again.
   pt.x -= m_rect.left;
   if ( m_dwFormat & DT_RIGHT )
   {
      int wide = m_rect.Width( ) - size.cx;
      pt.x -= wide;
      if ( pt.x <= 0 )
         return( FALSE );
   }

   if ( m_dwFormat & DT_CENTER )
   {
     int wide = m_rect.Width( ) - size.cx;
     pt.x -= (wide / 2);
     if ( pt.x <= 0 || pt.x > (size.cx + (wide / 2)) )
        return( FALSE );
   }

   // Turn point into a letter.
   int nLetters = (int) ((float) pt.x / width);

#if  !defined( _WIN32_WCE ) || ( _WIN32_WCE > 210 )
   // Find spaces before and after letter, process text between
   int nEndSpace = csURL.Find( _T( ' ' ), nLetters );
   if ( nEndSpace != -1 )
      csURL.Delete( nEndSpace, csURL.GetLength( ) - nEndSpace );

   int nBeginSpace = csURL.ReverseFind( _T( ' ' ) );
   if ( nBeginSpace != -1 )
      csURL.Delete( 0, ++nBeginSpace );
#endif

   // Does text have url?
   return( HasUrl( csURL ) );
}
