/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctltab.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of C++ classes for Zeidon Painter Notebook.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.04.19    DKS   Z10
//    Change to use FontCtrl as default.
//
// 1999.08.02    DKS   Z10    QS999
//    Refresh Painter window after dropping a page (tab) from the notebook.
//    Maintain last selected tab when returning from a sub-dialog.
//
// 1999.01.25    DKS
//    Permit designer to use buttons to move to proper page (tab) in
//    the notebook.
//
// 1998.08.13    DKS
//    Fix to Tab control acceptance by GroupBox (now warns)
//

#include "zstdafx.h"

#define TZCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "TzCtl.h"
#include "TzCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( TZNoteBook, CTabCtrl )

BEGIN_MESSAGE_MAP( TZNoteBook, CTabCtrl )
   ON_WM_CREATE( )
   ON_NOTIFY_REFLECT( TCN_SELCHANGE, SelChange )
   ON_NOTIFY_REFLECT( TCN_SELCHANGING, SelChanging )
   ON_WM_SETFOCUS( )
// ON_WM_DRAWITEM( )
   ON_WM_DRAWITEM_REFLECT( )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
//
//  CONSTRUCTOR: Tab/Notebook control
//
/////////////////////////////////////////////////////////////////////////////
// ctor
TZNoteBook::TZNoteBook( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CTabCtrl( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNoteBook::ctor ", m_csTag );
#endif
   zLONG lTabsPerRow = 0;
   m_bDeletable = TRUE;    // mark it as deletable
   m_bInitialized = FALSE;
   m_pZNotePage = 0;
   m_nPageCnt = 0;
   m_nTabNbrMax = 1;

   Attr.Style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
   if ( vCtrl )
   {
      OrderEntityForView( vCtrl, szlCtrlCtrl, "Type A" );

      zCHAR szBlob[ 5 * sizeof( zLONG ) ];
      zULONG ulLth = sizeof( szBlob );

      GetBlobFromAttribute( szBlob, &ulLth, vCtrl, "Control", "CtrlBOI" );
//    TraceLineI( "TZNoteBook ctor Blob Lth = ", ulLth );
      if ( ulLth >= 3 * sizeof( zLONG ) )
      {
         lTabsPerRow = *((zPLONG) (szBlob + sizeof( zLONG ) * 2));
      }
   }

// if ( lTabsPerRow )
//    Attr.Style |= TCS_MULTILINE,

   CreateZ( );

   ShowWindow( SW_SHOW );

#if 0
   CRect r;
   r.top = GetClientTop( );
   r.left = GetClientLeft( );
   r.right = r.left + GetClientWidth( );
   r.bottom = r.top + GetClientHeight( );
   TraceRect( "TZNoteBook ctor Client Rect:", r );
#endif
}

void
TZNoteBook::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNoteBook::CreateZ ", m_csTag );
#endif

   Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP, // || TCS_MULTILINE,
           CRect( Attr.X - 2, Attr.Y - 2,
                  Attr.X + Attr.W + 4, Attr.Y + Attr.H + 4 ),
           m_pWndParent, -1 );
}

int
TZNoteBook::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CTabCtrl::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

BOOL
TZNoteBook::DestroyWindow( )
{
   return( CTabCtrl::DestroyWindow( ) );
}

TZNoteBook::~TZNoteBook( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNoteBook::dtor ", m_csTag );
#endif
}

void
TZNoteBook::SelectPoint( CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZNoteBook::SelectPoint", pt );
#endif
   if ( mIs_hWnd( m_hWnd ) == 0 )
      return;

   PostMoveCtrl( );
   m_bDeletable = TRUE;    // mark it as deletable

   CWnd *pWndDlgItem = GetDlgItem( 1 );
   if ( pWndDlgItem )
   {
   // MessageBox( "TZNoteBook::SelectPoint", "Found DlgItem" );
   // TraceLineX( "TZNoteBook::SelectPoint DlgItem: ",
   //             (zLONG) pWndDlgItem->m_hWnd );
      CWnd *pWndChild = ChildWindowFromPoint( pt, CWP_SKIPINVISIBLE );
      if ( pWndChild )
      {
      // TraceLineX( "TZNoteBook::SelectPoint ChildFromPt: ",
      //             (zLONG) pWndChild->m_hWnd );
         if ( pWndDlgItem->m_hWnd == pWndChild->m_hWnd )
         {
            ClientToScreen( &pt );
            pWndDlgItem->ScreenToClient( &pt );
            pWndDlgItem->SendMessage( WM_LBUTTONDOWN, 0, MAKELONG( pt.x, pt.y ) );
            pWndDlgItem->SendMessage( WM_LBUTTONUP, 0, MAKELONG( pt.x, pt.y ) );
            return;
         }
      }
   }

   SendMessage( WM_LBUTTONDOWN, 0, MAKELONG( pt.x, pt.y ) );
   SendMessage( WM_LBUTTONUP, 0, MAKELONG( pt.x, pt.y ) );
//x   zSHORT nActiveTabNbr = GetActivePage( );
// TraceLineI( "TZNoteBook::Select New ActiveTab = ", nActiveTabNbr );
//xif ( nActiveTab != m_nActiveTabNbr )
//x      ActivateTab( nActiveTabNbr );

// TraceLineI( "TZNoteBook::Select Activating tab IdNbr ", GetTabID( ) );
// TraceLineI( "                         tab number ", m_nActiveTabNbr );
// TraceLineI( "                    real tab number ", GetTab( ) );
}

TZPainterCtrl *
TZNoteBook::IsDeletable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNoteBook::IsDeletable","" );
#endif
   if ( m_bDeletable == FALSE )
      return( 0 );

   if ( IsSelected( ) && GetVisibility( ) == 0 )
   {
      zSHORT nTabCnt;
      m_bDeletable = FALSE;

      if ( (nTabCnt = GetItemCount( )) == 0 )
      {
         return( this );
      }
      else
      {
         TZNotePage *pNotePage = m_pZNotePage;
         while ( pNotePage )
         {
            if ( nTabCnt == 1 ||
                 pNotePage->GetVisibility( ) <= 1 )  // at least visible
            {
               return( pNotePage );
            }

            pNotePage = pNotePage->m_pNext;
         }

         // If we got out of the above loop, we didn't locate a "visible"
         // tab, so let's find out who the tab control thinks is active
         // and make that tab visible.
         zSHORT nTabIdx = GetCurSel( );
         pNotePage = m_pZNotePage;
         while ( pNotePage )
         {
            if ( pNotePage && pNotePage->m_nTabIdx == nTabIdx )
               return( pNotePage );

            pNotePage = pNotePage->m_pNext;
         }
      }
   }

   return( 0 );
}

// Size the ctrl covers of the child windows of this control and in turn
// size the child window controls.
void
TZNoteBook::PostMoveCtrl( )
{
#if 0
#ifdef DEBUG_ALL
   TraceLineS( "TZNoteBook::PostMoveCtrl", "" );
#endif
   CRect rect;

   GetClientRect( rect );

// TraceRect( "TZNoteBook::PostMoveCtrl ClientRect", rect );

   AdjustRect( FALSE, &rect );
// TraceRect( "TZNoteBook::PostMoveCtrl Adjusted ClientRect", rect );

   // Get the rect in client coords
   TZPainterCtrl *pCtrl;
   pCtrl = this;
   while ( pCtrl )
      pCtrl = pCtrl->AdjustCtrlRect( rect, TRUE );

// TraceRect( "TZNoteBook::PostMoveCtrl Client Coords", rect );

   for ( zSHORT nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_pCtrlParent == this )
      {
         if ( pCtrl->m_pCtrlCover )
            pCtrl->m_pCtrlCover->SetWindowPos( 0, rect.left, rect.top,
                                               rect.right - rect.left,
                                               rect.bottom - rect.top,
                                               SWP_NOZORDER | SWP_NOREDRAW );

//       pCtrl->m_pCtrlCover->MoveWindow( rect, TRUE );
//       pCtrl->m_pWndCtrl->MoveWindow( rect, TRUE );
      }
   }
#endif
}

void
TZNoteBook::EndPaint( )
{
   if ( mIs_hWnd( m_hWnd ) )
   {
      CString cs = "Sel." + m_csTag;
      zLONG lCurrSel = GetProfileNbr( m_pPainterWindow->m_vDialog, "Design",
                                      m_pPainterWindow->m_csDlgTag,
                                      m_pPainterWindow->m_csWndTag, cs );
#ifdef DEBUG_ALL
      TraceLineI( "TZNoteBook::EndPaint: ", lCurrSel );
#endif

      m_bInitialized = TRUE;
      SetCurSel( 0 );
      PostMoveCtrl( );

      SetVisibility( 0x0100, -1, 1 );  // set all children invisible
      SetVisibility( 0x0101, 0, -1 );  // set self (only) visible/selectable

      if ( m_pCtrlCover )
         m_pCtrlCover->SetWindowPos( 0, 0, 0, 0, 0,
                                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW |
                                       SWP_NOZORDER | SWP_HIDEWINDOW );

      Invalidate( );
      if ( lCurrSel > 0 )
         ActivateTab( (zSHORT) lCurrSel );
      else
         ActivateTab( 1 );

      UpdateWindow( );
   }

// if ( m_pCtrlCover )
//    m_pCtrlCover->SetWindowPos( 0, 0, 0, 0, 0,
//                                SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW |
//                                   SWP_NOZORDER | SWP_SHOWWINDOW );
}

TZNotePage *
TZNoteBook::GetActivePage( )
{
   zSHORT nCurrIdx = GetCurSel( );
#ifdef DEBUG_ALL
   TraceLineI( "TZNoteBook::GetActivePage: ", nCurrIdx );
#endif

   TZNotePage *pCurrNotePage = m_pZNotePage;
   while ( pCurrNotePage )
   {
      if ( pCurrNotePage->m_nTabIdx == nCurrIdx )
         break;

      pCurrNotePage = pCurrNotePage->m_pNext;
   }

   return( pCurrNotePage );
}

TZNotePage *
TZNoteBook::GetPage( zSHORT nPageNbr )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZNoteBook::GetPage: ", nPageNbr );
#endif
   TZNotePage *pCurrNotePage = 0;
   if ( nPageNbr > 0 && nPageNbr <= m_nPageCnt )
   {
      pCurrNotePage = m_pZNotePage;
      while ( pCurrNotePage )
      {
         if ( pCurrNotePage->m_nTabNbr == nPageNbr )
            break;

         pCurrNotePage = pCurrNotePage->m_pNext;
      }
   }

   return( pCurrNotePage );
}

void
TZNoteBook::ActivateTab( zSHORT nTabNbr )
{
   TZNotePage    *pNotePage = GetPage( nTabNbr );
   TZPainterCtrl *pCtrl;
   zSHORT        nPos;

#ifdef DEBUG_ALL

   zCHAR szMsg[ 256 ];

   if ( pNotePage )
   {
      TraceLineI( "TZNoteBook::ActivateTab nTabNbr = ", nTabNbr );
      TraceLineS( "TZNoteBook::ActivateTab Tag: ", pNotePage->m_csTag );
   // ::MessageBox( 0, "Going to ActivateTab", "TZNoteBook", MB_OK );

      TraceLineI( "TZNoteBook::ActivateTab (start) cnt: ",
                  m_pPainterWindow->m_nCtrlCnt );
      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      // while ( pCtrl )
         {
         // if ( pCtrl->m_pCtrlParent &&
         //      zstrcmp( pCtrl->m_pCtrlParent->m_csTag, "Candidates" ) == 0 )
         //      zstrcmp( pCtrl->m_csTag, "ClientActivity" ) == 0 )
            {
         //    pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
               sprintf_s( szMsg, sizeof( szMsg ), " %3d.%s%-32s %-32s%s0x%04x:", nPos + 1,
                         pCtrl->IsSelected( ) ? "*" : " ",
                         pCtrl->m_pCtrlParent ?
                                       pCtrl->m_pCtrlParent->m_csTag : "<null>",
                         pCtrl->m_csTag,
                         (pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd )) ? " " : "~",
                         pCtrl->m_uVisible );
               TraceRect( szMsg, pCtrl->m_rectCtrl );
            // break;
            }

         // pCtrl = pCtrl->m_pCtrlParent;
         }
      }
   }

#endif

   SetVisibility( 0x0100, -1, 1 );  // set all children invisible
   SetVisibility( 0x0101, 0, -1 );  // set self (only) visible/selectable

   // Only the top tab is ever visible ... all sub-tabs are controlled
   // from the top tab.
#ifdef DEBUG_ALL

   TraceLineI( "TZNoteBook::ActivateTab (after1) cnt: ",
               m_pPainterWindow->m_nCtrlCnt );
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
   // while ( pCtrl )
      {
      // if ( pCtrl->m_pCtrlParent &&
      //      zstrcmp( pCtrl->m_pCtrlParent->m_csTag, "Candidates" ) == 0 )
      //      zstrcmp( pCtrl->m_csTag, "ClientActivity" ) == 0 )
         {
      //    pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            sprintf_s( szMsg, sizeof( szMsg ), " %3d.%s%-32s %-32s%s0x%04x:", nPos + 1,
                      pCtrl->IsSelected( ) ? "*" : " ",
                      pCtrl->m_pCtrlParent ?
                                    pCtrl->m_pCtrlParent->m_csTag : "<null>",
                      pCtrl->m_csTag,
                      (pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd )) ? " " : "~",
                      pCtrl->m_uVisible );
            TraceRect( szMsg, pCtrl->m_rectCtrl );
      //    break;
         }

      // pCtrl = pCtrl->m_pCtrlParent;
      }
   }
#endif

   // Now locate the active child TZNoteBook and make it (and children)
   // visible.  If there are no children, only the parent (this) is visible.
   if ( pNotePage && pNotePage->m_nTabNbr == nTabNbr )
   {
      // There is one check we want to do first.  If we are on a NotePage
      // ctrl that is a congruent sibling (same parent, position, and size
      // of another ctrl), we want to move to the next congruent sibling.
      // We have to enable/disable appropriately.
      if ( pNotePage->m_uVisible & 0x0090 )
      {
      // TraceLineS( "ActivateTab Congruent siblings ", pNotePage->m_csTag );
         for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
         {
            pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            if ( pCtrl->m_pCtrlParent == pNotePage->m_pCtrlParent )
            {
            // pCtrl->SetVisibility( 0x0001, 0, -1 );  // set self (only) visible/selectable

               if ( (pCtrl->m_uVisible & 0x0090) &&
                     pCtrl->m_rectCtrl == pNotePage->m_rectCtrl )
               {
                  if ( pCtrl == pNotePage )
                  {
                     pNotePage->m_uVisible &= ~0x0010;
                  // pNotePage->m_uVisible |= 0x0080;
                     pNotePage->SetVisibility( 0x0080, 0, 1 );
                     m_pPainterWindow->SetChildOverlapVisibility( pNotePage,
                                                                  TRUE );
                  // pCtrl->SetVisibility( 0x0101, -1, -1 );  // set all children visible
                  }
                  else
                  {
                     pCtrl->m_uVisible &= ~0x0080;
                  // pCtrl->m_uVisible |= 0x0010;
                     pCtrl->SetVisibility( 0x0010, 0, 1 );
                     m_pPainterWindow->SetChildOverlapVisibility( pCtrl,
                                                                  FALSE );
                  // pCtrl->SetVisibility( 0x0101, -1, 1 );   // set all children invisible
                  }
               }
            }
         }
      }

#ifdef DEBUG_ALL

   TraceLineI( "TZNoteBook::ActivateTab (after2) cnt: ",
               m_pPainterWindow->m_nCtrlCnt );
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
   // while ( pCtrl )
      {
      // if ( pCtrl->m_pCtrlParent &&
      //      zstrcmp( pCtrl->m_pCtrlParent->m_csTag, "Candidates" ) == 0 )
      //      zstrcmp( pCtrl->m_csTag, "ClientActivity" ) == 0 )
         {
      //    pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            sprintf_s( szMsg, sizeof( szMsg ), " %3d.%s%-32s %-32s%s0x%04x:", nPos + 1,
                      pCtrl->IsSelected( ) ? "*" : " ",
                      pCtrl->m_pCtrlParent ?
                                    pCtrl->m_pCtrlParent->m_csTag : "<null>",
                      pCtrl->m_csTag,
                      (pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd )) ? " " : "~",
                      pCtrl->m_uVisible );
            TraceRect( szMsg, pCtrl->m_rectCtrl );
      //    break;
         }

      // pCtrl = pCtrl->m_pCtrlParent;
      }
   }
#endif

      SetCurSel( pNotePage->m_nTabIdx );

      // Set the zero-based index of the selected tab if successful or
      // -1 if no tab is selected.
      CString cs = "Sel." + m_csTag;
      SetProfileNbr( m_pPainterWindow->m_vDialog, "Design",
                     m_pPainterWindow->m_csDlgTag,
                     m_pPainterWindow->m_csWndTag,
                     cs, pNotePage->m_nTabIdx + 1 );

      // Set all children visible/selectable.
      pNotePage->SetVisibility( 0x0101, -1, -1 );

      // Set self's Ctrl (only) to Visible ... not Selectable.
      pNotePage->SetVisibility( 0x0101, 0, -1 );
      pNotePage->SetVisibility( 0x0001, 0, 1 );
   // pNotePage->SetVisibility( 0x0000, 0, 0 );

      if ( pNotePage->m_pCtrlCover )
         pNotePage->m_pCtrlCover->SetWindowPos( 0, 0, 0, 0, 0,
                                                SWP_NOSIZE | SWP_NOMOVE |
                                                SWP_NOREDRAW | SWP_NOZORDER |
                                                SWP_HIDEWINDOW );
//    TraceLineS( "TZNoteBook::ActivateTab ================>", "" );
#ifdef DEBUG_ALL

   TraceLineI( "TZNoteBook::ActivateTab (after3) cnt: ",
               m_pPainterWindow->m_nCtrlCnt );
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
   // while ( pCtrl )
      {
      // if ( pCtrl->m_pCtrlParent &&
      //      zstrcmp( pCtrl->m_pCtrlParent->m_csTag, "Candidates" ) == 0 )
      //      zstrcmp( pCtrl->m_csTag, "ClientActivity" ) == 0 )
         {
      //    pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            sprintf_s( szMsg, sizeof( szMsg ), " %3d.%s%-32s %-32s%s0x%04x:", nPos + 1,
                      pCtrl->IsSelected( ) ? "*" : " ",
                      pCtrl->m_pCtrlParent ?
                                    pCtrl->m_pCtrlParent->m_csTag : "<null>",
                      pCtrl->m_csTag,
                      (pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd )) ? " " : "~",
                      pCtrl->m_uVisible );
            TraceRect( szMsg, pCtrl->m_rectCtrl );
      //    break;
         }

      // pCtrl = pCtrl->m_pCtrlParent;
      }
   }

   TraceCtrl( "ActivateTab Ctrl", pNotePage );
#endif

      if ( m_pCtrlCover )
         m_pCtrlCover->SetWindowPos( 0, 0, 0, 0, 0,
                                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW |
                                        SWP_NOZORDER | SWP_HIDEWINDOW );
      SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
      Invalidate( );
      UpdateWindow( );

      if ( m_pCtrlCover )
         m_pCtrlCover->SetWindowPos( 0, 0, 0, 0, 0,
                                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW |
                                        SWP_NOZORDER | SWP_SHOWWINDOW );
   }

   m_pPainterWindow->MoveAllSelectedFramesToTop( );
// ::MessageBox( 0, "After MoveAllSelectedFrames", "ToTop", MB_OK );
}

TZPainterCtrl *
TZNoteBook::GetRealCtrl( )
{
   zSHORT nTabIdx = GetCurSel( );

#ifdef DEBUG_ALL
   TraceLineI( "TZNoteBook::GetRealParent nActiveTab = ", nTabIdx );
#endif

   TZNotePage *pNotePage = m_pZNotePage;
   while ( pNotePage )
   {
      if ( pNotePage && pNotePage->m_nTabIdx == nTabIdx )
      {
//       TraceLineS( "TZNoteBook::GetRealParent located Caption: ",
//                   ((TZNoteBook *) pCtrl->m_pWndCtrl)->m_pchCaption );

         // set the TZNotePage as the real parent
         return( pNotePage );
      }

//    TraceLineI( ((TZNoteBook *) pCtrl->m_pWndCtrl)->m_pchCaption,
//                   ((TZNoteBook *) pCtrl->m_pWndCtrl)->m_nTabIdx );

      pNotePage = pNotePage->m_pNext;
   }

   return( this );
}

void
TZNoteBook::RealizeCtrl( zBOOL bCreate )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZNoteBook::RealizeCtrl bCreate = ", bCreate );
#endif
   if ( bCreate )
   {
      TZPainterCtrl::RealizeCtrl( TRUE );
   // TZNotePage *pCurrNotePage = GetActivePage( );
   // if ( pCurrNotePage )
   // {
   //    ActivateTab( pCurrNotePage->m_nTabNbr );
   // }
   }

   if ( m_pWndCtrl && mIs_hWnd( m_pWndCtrl->m_hWnd ) )
      m_pWndCtrl->ShowWindow( bCreate ? SW_SHOW : SW_HIDE );
}

void
TZNoteBook::SelChange( NMHDR *pNMHDR, LRESULT *pResult )
{
// Default( );

   zSHORT nCurrIdx = GetCurSel( );
#ifdef DEBUG_ALL
   TraceLineI( "TZNoteBook::SelChange: ", nCurrIdx );
#endif

   *pResult = 0;
   TZNotePage *pCurrNotePage = GetActivePage( );
   if ( pCurrNotePage )
   {
      ActivateTab( pCurrNotePage->m_nTabNbr );
   }
}

void
TZNoteBook::SelChanging( NMHDR *pNMHDR, LRESULT *pResult )
{
// Default( );

   zSHORT nCurrIdx = GetCurSel( );
#ifdef DEBUG_ALL
   TraceLineI( "TZNoteBook::SelChanging: ", nCurrIdx );
#endif

   *pResult = 0;
   TZNotePage *pCurrNotePage = GetActivePage( );
   if ( pCurrNotePage )
   {
      pCurrNotePage->ShowWindow( SW_HIDE );
   }
}

void
TZNoteBook::OnSetFocus( CWnd *pWndLostFocus )
{
   m_pPainterWindow->SetFocus( );
}

BOOL
TZNoteBook::OnChildNotify( UINT message, WPARAM wParam,
                           LPARAM lParam, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNoteBook::OnChildNotify", "" );
#endif
   if ( message != WM_DRAWITEM )
      return( CWnd::OnChildNotify( message, wParam, lParam, pResult ) );

// TraceLineS( "TZNoteBook::OnChildNotify", " WM_DRAWITEM" );
   ASSERT( pResult == NULL );       // no return value expected
   UNUSED( pResult ); // unused in release builds

   DrawItem( (LPDRAWITEMSTRUCT) lParam );
   return( TRUE );
}

// Drawing: for all 4 control types
void
TZNoteBook::OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT pDrawInfo )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZNoteBook::OnDrawItem ================>", "" );
#endif
   if ( pDrawInfo->CtlType == ODT_MENU )
   {
      CMenu* pMenu =
         CMenu::FromHandlePermanent( (HMENU) pDrawInfo->hwndItem );

      if ( pMenu )
      {
         pMenu->DrawItem( pDrawInfo );
         return; // eat it
      }
   }
   else
   {
      // Reflect notification to child window control.
      if ( ReflectLastMsg( pDrawInfo->hwndItem ) )
         return;     // eat it
   }

   // not handled - do default
   Default( );
}

void
TZNoteBook::DrawItem( LPDRAWITEMSTRUCT pDrawInfo )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZNoteBook::DrawItem ================>", "" );
#endif
   if ( pDrawInfo->hwndItem != m_hWnd )
   {
      if ( ReflectLastMsg( pDrawInfo->hwndItem ) )
         return;     // eat it
   }

   CRect  rect;

   rect.left = pDrawInfo->rcItem.left;
   rect.right = pDrawInfo->rcItem.right;
   rect.top = pDrawInfo->rcItem.top;
   rect.bottom = pDrawInfo->rcItem.bottom;

   HBRUSH hLiteBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNFACE ) );
   ::FillRect( pDrawInfo->hDC, &rect, hLiteBrush );
   DeleteObject( hLiteBrush );

   CTabCtrl::DrawItem( pDrawInfo );
}

IMPLEMENT_DYNAMIC( TZNotePage, CWnd )

BEGIN_MESSAGE_MAP( TZNotePage, CWnd )
   ON_WM_CREATE( )
// ON_WM_DRAWITEM( )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
//
//  CONSTRUCTOR: Tab sub-control (to group controls at the tab level)
//
/////////////////////////////////////////////////////////////////////////////
// ctor
TZNotePage::TZNotePage( TZPainterWindow *pPainterWindow,
                        TZPainterCtrl   *pCtrlParent,
                        CWnd    *pWndParent,
                        CRect&  rect,
                        zVIEW   vCtrl,
                        zLONG   lType,
                        zBOOL   bPlaceHolder,
                        zLONG   lZKey,
                        zLONG   lTabStopNbr,
                        zLONG   lCtrlID,
                        zPCHAR  pchTag,
                        zPCHAR  pchText ) :
            CWnd( ),
            TZPainterCtrl( pPainterWindow, pCtrlParent, pWndParent,
                           rect, vCtrl, this, pchTag, pchText,
                           lType, bPlaceHolder, lZKey, lTabStopNbr, lCtrlID )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNotePage::ctor ", m_csTag );
#endif
   m_pNext = 0;
   m_uVisible = 0x0100;

   m_lSubtype = 0;

   m_pZNoteBook = DYNAMIC_DOWNCAST( TZNoteBook, pWndParent );
   if ( m_pZNoteBook == 0 )
      return;

   zPCHAR pch;
   zLONG  lTabNbr;
   zLONG  lLth;

   if ( vCtrl )
   {
      GetAddrForAttribute( &pch, vCtrl, szlControl, szlText );
      lLth = zstrlen( pch ) + 1;
      m_pchCaption = new char[ lLth ];
      strcpy_s( m_pchCaption, lLth, pch );
      GetIntegerFromAttribute( &lTabNbr, vCtrl, szlControl, szlType );
      if ( lTabNbr == 0 )
      {
         zVIEW vTemp;

         m_pZNoteBook->m_nTabNbrMax++;
         CreateViewFromViewForTask( &vTemp, vCtrl, pPainterWindow->m_vDialog );
         ResetViewFromSubobject( vTemp );
         while ( SetCursorFirstEntityByInteger( vTemp, szlCtrlCtrl, szlType,
                                                m_pZNoteBook->m_nTabNbrMax, 0 ) == zCURSOR_SET )
         {
            m_pZNoteBook->m_nTabNbrMax++;
         }

         SetAttributeFromInteger( vCtrl, szlControl, szlType, m_pZNoteBook->m_nTabNbrMax );
         m_lSubtype = m_pZNoteBook->m_nTabNbrMax;
         DropView( vTemp );
      }
      else
      {
         if ( lTabNbr > m_pZNoteBook->m_nTabNbrMax )
            m_pZNoteBook->m_nTabNbrMax = (zSHORT) lTabNbr;

         m_lSubtype = m_pZNoteBook->m_nTabNbrMax;
      }

      if ( MiGetUpdateForView( vCtrl ) > 0 )
      {
         SetAttributeFromAttribute( vCtrl, szlControl, szlSyncKey, vCtrl, szlControl, szlType );
      }
   }
   else
   {
      m_pZNoteBook->m_nTabNbrMax++;
      m_lSubtype = m_pZNoteBook->m_nTabNbrMax;
      lTabNbr = m_pZNoteBook->m_nTabNbrMax;
      lLth = zstrlen( pchText ) + 1;
      m_pchCaption = new char[ lLth ];
      strcpy_s( m_pchCaption, lLth, pchText );
   }

// TraceRect( "TabCtl rect ", rect );
// TraceLineI( "    Attr.X ", Attr.X );
// TraceLineI( "    Attr.Y ", Attr.Y );
// TraceLineI( "    Attr.W ", Attr.W );
// TraceLineI( "    Attr.H ", Attr.H );

   // Build sub-controls on demand (only when visible or selectable).
   m_bBuildOnDemand = TRUE;

   if ( m_pZNoteBook->m_pZNotePage )
   {
      m_nTabIdx = 1;
      TZNotePage *pCurrNotePage = m_pZNoteBook->m_pZNotePage;
      while ( pCurrNotePage->m_pNext )
      {
         m_nTabIdx++;
         pCurrNotePage = pCurrNotePage->m_pNext;
      }

      pCurrNotePage->m_pNext = this;
   }
   else
   {
      m_nTabIdx = 0;
      m_pZNoteBook->m_pZNotePage = this;
   }

// m_nTabNbr = (zSHORT) lTabNbr;
   m_nTabNbr = m_nTabIdx + 1;
   m_pZNoteBook->m_nPageCnt++;

   TC_ITEM TabCtrlItem;
   TabCtrlItem.mask = TCIF_TEXT;
   TabCtrlItem.pszText = m_pchCaption;
#ifdef DEBUG_ALL
   TraceLineS( "Adding TabCtrlItem: ", m_pchCaption );
#endif
   m_pZNoteBook->InsertItem( m_nTabIdx, &TabCtrlItem );

//?Attr.StyleEx = WS_EX_TRANSPARENT;
//?Attr.Style &= ~(WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

   CreateZ( );
}

void
TZNotePage::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNotePage::CreateZ ", m_csTag );
#endif

   CRect rect;

   m_pWndParent->GetClientRect( rect );
   m_pZNoteBook->AdjustRect( FALSE, &rect );
   CreateEx( 0, "ZeidonCustomCtrl", m_csText, WS_CHILD,
             rect.left, rect.top,
             rect.right - rect.left, rect.bottom - rect.top,
             m_pWndParent->m_hWnd, (HMENU) -1 );
}

int
TZNotePage::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( (int) Default( ) == -1 )
      return( -1 );

   if ( m_pPainterWindow->m_pBar->m_pFontWS )
      SendMessage( WM_SETFONT,
             (WPARAM)(HFONT) *(m_pPainterWindow->m_pBar->m_pFontWS) );

   ModifyStyleEx( 0, WS_EX_TRANSPARENT );
   return( 0 );
}

BOOL
TZNotePage::DestroyWindow( )
{
   if ( m_pZNoteBook->m_pZNotePage == this )
      m_pZNoteBook->m_pZNotePage = 0;

   return( CWnd::DestroyWindow( ) );
}

TZNotePage::~TZNotePage( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNotePage dtor ", m_csTag );
#endif

   mDeleteInitA( m_pchCaption );
   if ( mIs_hWnd( m_hWnd ) == 0 )
      return;

#ifdef DEBUG_ALL

   TC_ITEM TabCtrlItem;
   TZNotePage *pPage = m_pZNoteBook->m_pZNotePage;
   zSHORT k = m_pZNoteBook->m_nPageCnt;
   while ( k-- )
   {
      TabCtrlItem.mask = TCIF_TEXT;
      if ( m_pZNoteBook->GetItem( k, &TabCtrlItem ) )
      {
         TraceLineI( "NoteBook GetItem successful for ", k );
         TraceLineS( "                           text ", TabCtrlItem.pszText );
      }
      else
      {
         TraceLineI( "NoteBook GetItem unsuccessful for ", k );
      }
   }

   m_pZNoteBook->DeleteItem( m_nTabIdx );

   TraceLineS( "TZNotePage after DeleteItem ", m_csTag );

   pPage = m_pZNoteBook->m_pZNotePage;
   k = m_pZNoteBook->m_nPageCnt + 5;
   while ( k-- )
   {
      TabCtrlItem.mask = TCIF_TEXT;
      if ( m_pZNoteBook->GetItem( k, &TabCtrlItem ) )
      {
         TraceLineI( "NoteBook GetItem successful for ", k );
         TraceLineS( "                           text ", TabCtrlItem.pszText );
      }
      else
      {
         TraceLineI( "NoteBook GetItem unsuccessful for ", k );
      }
   }
#else

   m_pZNoteBook->DeleteItem( m_nTabIdx );

#endif

   if ( m_pZNoteBook->m_pZNotePage )
   {
      TZNotePage *pNotePage = m_pZNoteBook->m_pZNotePage;
      TZNotePage *pNotePagePrev = 0;

      while ( pNotePage )
      {
         if ( pNotePage == this )
         {
            if ( pNotePagePrev )
               pNotePagePrev->m_pNext = pNotePage->m_pNext;
            else
               m_pZNoteBook->m_pZNotePage = pNotePage->m_pNext;

            m_pZNoteBook->m_nPageCnt--;

            zSHORT k = m_nTabIdx;
            pNotePage = pNotePage->m_pNext;
            while ( pNotePage )
            {
               pNotePage->m_nTabIdx = k++;
               pNotePage = pNotePage->m_pNext;
            }

            break;
         }

         pNotePagePrev = pNotePage;
         pNotePage = pNotePage->m_pNext;
      }
   }
}

void
TZNotePage::SelectPoint( CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZNotePage::SelectPoint", pt );
#endif
   m_pZNoteBook->SelectPoint( pt );
}

void
TZNotePage::RealizeCtrl( zBOOL bCreate )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZNotePage::RealizeCtrl", bCreate );
#endif
   if ( bCreate )
      TZPainterCtrl::RealizeCtrl( TRUE );

   if ( m_pWndCtrl && mIs_hWnd( m_pWndCtrl->m_hWnd ) )
      m_pWndCtrl->ShowWindow( bCreate ? SW_SHOW : SW_HIDE );
}

void
TZNotePage::UndoRedo( zBOOL bToRedo )
{
   if ( bToRedo == FALSE )
   {
      // It is difficult to get the NoteBook ctrl to repaint
      // properly, so this is the punt ... repaint the whole
      // window (losing undo capability, but at least things
      // look right).
      m_pPainterWindow->UpdateZeidonWindow( );
      m_pPainterWindow->PaintZeidonWindow( );
   }
}

TZPainterCtrl *
TZNotePage::IsDeletable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNotePage::IsDeletable ", m_csTag );
#endif

   if ( m_pZNoteBook )
      m_pZNoteBook->m_bDeletable = TRUE;

   return( 0 );
}

void
TZNotePage::PreMoveCtrl( CRect& rect )
{
#ifdef DEBUG_ALL
   TraceRect( "TZNotePage::PreMoveCtrl1", rect );
#endif
   m_pZNoteBook->GetClientRect( rect );
   m_pZNoteBook->AdjustRect( FALSE, &rect );
// rect.right -= rect.left;
// rect.bottom -= rect.top;
#ifdef DEBUG_ALL
   TraceRect( "TZNotePage::PreMoveCtrl2", rect );
#endif
}

TZPainterCtrl *
TZNotePage::GetRealCtrl( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZNotePage::GetRealCtrl ", m_csTag );
#endif
   return( m_pCtrlParent->GetRealCtrl( ) );
}

TZPainterCtrl *
TZNotePage::AdjustCtrlRect( CRect& rect,
                            zBOOL  bToPainterRect )
{
#ifdef DEBUG_ALL
   TraceRect( "TZNotePage::AdjustCtrlRect1", rect );
#endif

   if ( mIs_hWnd( m_pZNoteBook->m_hWnd ) )
   {
      CRect rectClient;
      m_pZNoteBook->GetClientRect( rectClient );
      m_pZNoteBook->AdjustRect( FALSE, &rectClient );

      zLONG lLeft = rectClient.left;
      zLONG lTop = rectClient.top;

      if ( bToPainterRect )
      {
         rect.left += lLeft;
         rect.top += lTop;
         rect.right += lLeft;
         rect.bottom += lTop;
      }
      else
      {
         rect.left -= lLeft;
         rect.top -= lTop;
         rect.right -= lLeft;
         rect.bottom -= lTop;
      }

#ifdef DEBUG_ALL
      TraceRect( "TZNotePage::AdjustCtrlRect2", rect );
#endif
   }

   return( m_pCtrlParent );
}

#if 0
void
TZNotePage::OnPaint( );
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZNotePage::Paint", "" );
#endif

#if 0
   int nOldMode = tdc.SetBkMode( TRANSPARENT );
// TraceLineI( "TabDlg OldMode = ", nOldMode );
   CWnd::Paint( tdc, erase, rect );
   tdc.SetBkMode( nOldMode );
#endif
}
#endif

void
TZNotePage::OnDrawItem( int nCtrlId, LPDRAWITEMSTRUCT pDrawInfo )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZNotePage::OnDrawItem", "" );
#endif
   if ( pDrawInfo->hwndItem != m_hWnd )
   {
      if ( ReflectLastMsg( pDrawInfo->hwndItem ) )
         return;     // eat it
   }

   Default( );
}

BOOL
TZNotePage::OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                               VARIANT *pvar )
{
   if ( m_pPainterWindow && m_pPainterWindow->m_pPainterClient )
   {
      return( m_pPainterWindow->m_pPainterClient->
                                 OnAmbientProperty( pSite, dispid, pvar ) );
   }
   else
   {
      return( CWnd::OnAmbientProperty( pSite, dispid, pvar ) );
   }
}
