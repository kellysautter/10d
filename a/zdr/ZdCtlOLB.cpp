/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlolb.cpp
// AUTHOR:
// Copyright (c) Iuri Apollonio 1998
// Use & modify as you want & need, and leave those 3 lines.
// http://www.codeguru.com
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Outlook Bar control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlOLB.h"
#include "ZdCtlGbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _ID_GFX_SMALLICON  "Small Icon"
#define _ID_GFX_LARGEICON  "Large Icon"
#define _ID_GFX_REMOVEITEM "Remove Item"
#define _ID_GFX_RENAMEITEM "Rename Item"

/////////////////////////////////////////////////////////////////////////////
// ZOutlookBar

IMPLEMENT_DYNAMIC( ZOutlookBar, CWnd )
BEGIN_MESSAGE_MAP( ZOutlookBar, CWnd )
   //{{AFX_MSG_MAP( ZOutlookBar )
   ON_WM_CREATE( )
   ON_WM_TIMER( )
   ON_WM_PAINT( )
   ON_WM_ERASEBKGND( )
   ON_WM_MOUSEMOVE( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_SETCURSOR( )
   ON_WM_SIZE( )
   ON_WM_RBUTTONDOWN( )
   ON_COMMAND( ID_GFX_LARGEICON, OnGfxLargeicon )
   ON_UPDATE_COMMAND_UI( ID_GFX_LARGEICON, OnUpdateGfxLargeicon )
   ON_COMMAND( ID_GFX_SMALLICON, OnGfxSmallicon )
   ON_UPDATE_COMMAND_UI( ID_GFX_SMALLICON, OnUpdateGfxSmallicon )
   ON_COMMAND( ID_GFX_REMOVEITEM, OnGfxRemoveitem )
   ON_UPDATE_COMMAND_UI( ID_GFX_REMOVEITEM, OnUpdateGfxRemoveitem )
   ON_COMMAND( ID_GFX_RENAMEITEM, OnGfxRenameitem )
   ON_UPDATE_COMMAND_UI( ID_GFX_RENAMEITEM, OnUpdateGfxRenameitem )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   //}}AFX_MSG_MAP
   ON_MESSAGE( WM_OUTBAR_NOTIFY, OnEndLabelEdit )
END_MESSAGE_MAP( )


#define zCALENDAR_DROPDOWN          zMAPACT_CTRL_SPECIFIC1

// ZOutlookBar - ctor
ZOutlookBar::ZOutlookBar( ZSubtask *pZSubtask,
                          CWnd     *pWndParent,
                          ZMapAct  *pzmaComposite,
                          zVIEW    vDialog,
                          zSHORT   nOffsetX,
                          zSHORT   nOffsetY,
                          zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
             CWnd( ),
             ZMapAct( pZSubtask,             // base class ctor
                      pzmaComposite,
                      pWndParent,
                      this,
                      vDialog,
                      nOffsetX,
                      nOffsetY,
                      pCtrlDef,
                      "OutlookBar" )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZOutlookBar::ctor ", *m_pzsTag );
#endif

   m_lpfnOLB_Event = 0;

   m_clrBackGroundColor = GetSysColor( COLOR_BACKGROUND ); //RGB( 0, 128, 128 );
   m_clrBackGroundColor1 = GetSysColor( COLOR_BACKGROUND );
   m_clrTextColor = GetSysColor( COLOR_WINDOW );
   m_clr3dFace = GetSysColor( COLOR_3DFACE );
   m_clrLightBorder = GetSysColor( COLOR_3DLIGHT );
   m_clrHilightBorder = GetSysColor( COLOR_3DHILIGHT );
   m_clrShadowBorder = GetSysColor( COLOR_3DSHADOW );
   m_clrDkShadowBorder = GetSysColor( COLOR_3DDKSHADOW );

   m_pBlackPen = new CPen( PS_SOLID, 1, RGB( 0, 0, 0 ) );

   m_nFolderHeight = 22;

   m_dwFlags = fDragItems | fEditGroups | fEditItems | fRemoveGroups |
               fRemoveItems | fAddGroups | fAnimation | fSelHighlight;

   m_nSelFolder = 1;

   m_nLastFolderHighlighted = -1;
   m_nLastSelectedFolder = -1;

   m_pLargeImageList = 0;
   m_pSmallImageList = 0;

   m_nFirstItem = 0;

   m_nSmallIconLabelOffsetX = 4;
   m_nLargeIconLabelOffsetY = 3;

   m_nSmallIconSpacingY = 10;
   m_nLargeIconSpacingY = 8;

   m_nLeftMarginX = 5;
   m_nTopMarginY = 5;

   m_hHandCursor = AfxGetApp( )->LoadCursor( IDC_HAND1 );
   m_rectUpArrow = m_rectDownArrow = CRect( 0, 0, 0, 0 );

   m_bUpArrow = m_bDownArrow = FALSE;
   m_bUpPressed = m_bDownPressed = FALSE;
   m_bLooping = FALSE;

   m_nLastItemHighlighted = -1;
   m_bPressedHighlight = FALSE;
   m_nLastDragItemDraw = -1;

   m_lAnimationTickCount = 10;

   m_hDragCursor = AfxGetApp( )->LoadCursor( IDC_DRAGGING );
   m_hNoDragCursor = AfxGetApp( )->LoadCursor( IDC_NODRAGGING );

   m_nLastSel = -1;
   m_nSelAnimCount = 0;
   m_nSelAnimTiming = 0;

   CreateZ( );
}

ZOutlookBar::~ZOutlookBar( )
{
   for ( int t = 0; t < m_arFolder.GetSize( ); t++ )
   {
      if ( m_arFolder.GetAt( t ) )
         delete( (ZBarFolder *) m_arFolder.GetAt( t ) );
   }

   m_arFolder.RemoveAll( );

   delete( m_pBlackPen );
}

// Sends a message to the parent in the form of a WM_OUTBAR_NOTIFY message
// with a OUTBAR_INFO structure attached.
LRESULT
ZOutlookBar::SendMessageToParent( WPARAM wMsg, LPARAM lParam )
{
   LRESULT lResult = 1;

   if ( mIs_hWnd( m_hWnd ) )
   {
   // TraceLineI( "ZOutlookBar::SendMessageToParent: ", wMsg );
      if ( m_lpfnOLB_Event )
      {
         if ( wMsg == NM_OB_ITEMCLICK || wMsg == NM_FOLDERCHANGE )
            lParam++;

         lResult = (*m_lpfnOLB_Event)( m_pZSubtask->m_vDialog,
                                       *(m_pzsTag), wMsg, lParam );

         if ( wMsg == NM_OB_ITEMCLICK || wMsg == NM_FOLDERCHANGE )
            lParam--;
      }

      if ( lResult )
      {
         CWnd *pOwner = GetOwner( );
         if ( pOwner && mIs_hWnd( pOwner->m_hWnd ) )
         {
            lResult = pOwner->SendMessage( WM_OUTBAR_NOTIFY, wMsg, lParam );
         }
      }
   }

   return( lResult );
}

/////////////////////////////////////////////////////////////////////////////
// ZOutlookBar message handlers

long
ZOutlookBar::OnEndLabelEdit( WPARAM wParam, LPARAM lParam )
{
   if ( wParam == NM_OB_ONGROUPENDEDIT || wParam == NM_OB_ONLABELENDEDIT )
   {
      ZOLB_Edit *pEdit = (ZOLB_Edit *) lParam;
      OUTBAR_INFO ob;
      ob.nIdx = pEdit->m_nIdx;
      ob.cpcText = pEdit->m_csText;

      long lResult = SendMessageToParent( wParam, (LPARAM) &ob );

      if ( lResult )
      {
         CRect rect;
         if ( wParam == NM_OB_ONGROUPENDEDIT )
         {
            SetFolderText( pEdit->m_nIdx, pEdit->m_csText );
            GetFolderRect( pEdit->m_nIdx, rect );
            InvalidateRect( rect, FALSE );
         }
         else
      // if ( wParam == NM_OB_ONLABELENDEDIT )
         {
            SetItemText( pEdit->m_nIdx, pEdit->m_csText );
            GetInsideRect( rect );
            InvalidateRect( rect, TRUE );
         }
      }
   }

   return( 0 );
}

void
ZOutlookBar::SetFolderText( const int nIdx, zCPCHAR cpcText )
{
   ASSERT( nIdx >= 0 && nIdx < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nIdx );
   if ( pbf->m_pchName )
   {
      delete [] pbf->m_pchName;
      pbf->m_pchName = 0;
   }

   zLONG lLth = zstrlen( cpcText ) + 1;
   pbf->m_pchName = new char[ lLth ];
   strcpy_s( pbf->m_pchName, lLth, cpcText );
}

void
ZOutlookBar::SetItemText( const int nIdx, zCPCHAR cpcText )
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
   if ( pi->m_pchItem )
   {
      delete [] pi->m_pchItem;
      pi->m_pchItem = 0;
   }

   zLONG lLth = zstrlen( cpcText ) + 1;
   pi->m_pchItem = new char[ lLth ];
   strcpy_s( pi->m_pchItem, lLth, cpcText );
}

BOOL
ZOutlookBar::Create( DWORD dwStyle, const RECT& rect,
                        CWnd *pParentWnd, UINT nID,
                        const DWORD dwFlags )
{
   m_dwFlags = dwFlags;
   return( CWnd::CreateEx( WS_EX_STATICEDGE, 0, 0, dwStyle | WS_CHILD,
                           rect, pParentWnd, nID ) );
   //GFXOUTBARCTRL_CLASSNAME
}

bool
ZOutlookBar::IsSmallIconView( ) const
{
   return( m_dwFlags & fSmallIcon );
}

void
ZOutlookBar::SetSmallIconView( const bool bSet )
{
   m_nFirstItem = 0;

   if ( bSet )
   {
      if ( !IsSmallIconView( ) )
      {
         m_dwFlags |= fSmallIcon;
         CRect rect;
         GetInsideRect( rect );
         InvalidateRect( rect, FALSE );
      }
   }
   else
   {
      if ( IsSmallIconView( ) )
      {
         m_dwFlags &= ~fSmallIcon;
         CRect rect;
         GetInsideRect( rect );
         InvalidateRect( rect, FALSE );
      }
   }
}

DWORD
ZOutlookBar::GetFlag( ) const
{
   return( m_dwFlags );
}

void
ZOutlookBar::ModifyFlag( const DWORD& dwRemove,
                            const DWORD& dwAdd,
                            const UINT redraw )
{
   if ( dwRemove )
      m_dwFlags &= ~(dwRemove);

   if ( dwAdd )
      m_dwFlags |= dwAdd;

   if ( GetSafeHwnd( ) )
   {
      if ( redraw != 0 )
         SetWindowPos( 0, 0, 0, 0, 0,
                       SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | redraw );
      else
         Invalidate( );
   }
}

int ZOutlookBar::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   // TODO: Add your specialized creation code here

   return( 0 );
}

void
ZOutlookBar::OnTimer( UINT nIDEvent )
{
   if ( nIDEvent == 1 )
   {
      CPoint pt( GetMessagePos( ) );

      ScreenToClient( &pt );

      CRect rect;
      GetClientRect( &rect );
      if ( rect.PtInRect( pt ) == FALSE )
      {
         HighlightFolder( -1 );
         HighlightItem( -1 );
         KillTimer( 1 );
      }
   }
   else
   if ( nIDEvent == 3 && m_nLastSel >= 0 )
   {
      m_nSelAnimCount++;
      if ( m_nSelAnimCount > 10 )
         m_nSelAnimCount = -1;

      if ( m_nSelAnimCount == 0 )
         DrawAnimItem( -1, 1, m_nLastSel );

      if ( m_nSelAnimCount == 1 )
         DrawAnimItem( 0, 0, m_nLastSel );

      if ( m_nSelAnimCount == 2 )
         DrawAnimItem( 1, 1, m_nLastSel );

      if ( m_nSelAnimCount == 3 )
         DrawAnimItem( 0, 0, m_nLastSel );
   }

   CWnd::OnTimer( nIDEvent );
}

void
ZOutlookBar::DrawAnimItem( const int nOffsetX,
                              const int nOffsetY, const int nIdx )
{
   CImageList *ima = GetFolderImageList( m_nSelFolder, IsSmallIconView( ) );

   CRect rect;
   CRect rectInside;
   GetInsideRect( rectInside );
   GetItemRect( m_nSelFolder, m_nLastSel, rect );

   if ( m_nLastSel >= m_nFirstItem &&
        rectInside.bottom > rect.bottom &&
        rectInside.top < rect.top )
   {
      ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
      ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );

      ASSERT( nIdx >= 0 && nIdx < pbf->GetItemCount( ) );
      ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );

      ASSERT( pi && ima );

      CClientDC dc( this );

      if ( IsSmallIconView( ) )
      {
         if ( ima )
         {
            IMAGEINFO ii;
            ima->GetImageInfo( pi->m_nImageIdx, &ii );
            CSize sizeImage = CRect( ii.rcImage ).Size( );
            CPoint pt;
            pt.x = rect.left + 2;
            pt.y = rect.top + (rect.Height( ) - sizeImage.cy) / 2;

            CRect rectBack( pt.x - 1, pt.y - 1, pt.x + sizeImage.cx + 2,
                            pt.y + sizeImage.cy + 2 );
            dc.FillSolidRect( rectBack, m_clrBackGroundColor );

            pt.x += nOffsetX;
            pt.y += nOffsetY;

            ima->Draw( &dc, pi->m_nImageIdx, pt, ILD_NORMAL );
         }
      }
      else
      {
         if ( ima )
         {
            IMAGEINFO ii;
            ima->GetImageInfo( pi->m_nImageIdx, &ii );
            CSize sizeImage = CRect( ii.rcImage ).Size( );
            CPoint pt;
            pt.x = rect.left + (rect.Width( ) - sizeImage.cx) / 2;
            pt.y = rect.top;

            CRect rectBack( pt.x - 1, pt.y - 1, pt.x + sizeImage.cx + 2,
                            pt.y + sizeImage.cy + 2 );
            dc.FillSolidRect( rectBack, m_clrBackGroundColor );

            pt.x += nOffsetX;
            pt.y += nOffsetY;

            ima->Draw( &dc, pi->m_nImageIdx, pt, ILD_NORMAL );
         }
      }
   }
}

void
ZOutlookBar::OnPaint( )
{
   int nMax = m_arFolder.GetSize( );
   if ( nMax < m_nSelFolder )
      return;

   CRect rectClient;
   GetClientRect( &rectClient );
   CPaintDC dc( this );
   CDC memDC;
   memDC.CreateCompatibleDC( &dc );
   CBitmap bmp;
   bmp.CreateCompatibleBitmap( &dc, rectClient.Width( ), rectClient.Height( ) );
   CBitmap *obmp = memDC.SelectObject( &bmp );
   CDC *pDC = &memDC;

   CRect rect;
   GetInsideRect( rect );
   if ( !GetFolderChild( ) )
      pDC->FillSolidRect( rect, m_clrBackGroundColor );

   CRect frc;
   for ( int t = 0; t < nMax; t++ )
   {
      GetFolderRect( t, frc );
      DrawFolder( pDC, t, frc, FALSE );
   }

   if ( !GetFolderChild( ) )
   {
      int f, l;
      GetVisibleRange( m_nSelFolder, f, l );
      m_rectUpArrow.SetRect( 0, 0, GetSystemMetrics( SM_CXVSCROLL ),
                             GetSystemMetrics( SM_CXVSCROLL ) );
      m_rectDownArrow = m_rectUpArrow;
      m_rectUpArrow.OffsetRect( rect.right - 5 - GetSystemMetrics( SM_CXVSCROLL ),
                                rect.top + 5 );
      m_rectDownArrow.OffsetRect( rect.right - 5 - GetSystemMetrics( SM_CXVSCROLL ),
                                  rect.bottom - 5 - GetSystemMetrics( SM_CXVSCROLL ) );

      if ( f > 0 )
      {
         pDC->DrawFrameControl( m_rectUpArrow, DFC_SCROLL,
                                DFCS_SCROLLUP | (m_bUpPressed ? DFCS_PUSHED : 0) );
         m_bUpArrow = TRUE;
         pDC->ExcludeClipRect( m_rectUpArrow );
      }
      else
         m_bUpArrow = FALSE;

      if ( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) &&
           l < GetItemCount( ) - 1 )
      {
         pDC->DrawFrameControl( m_rectDownArrow, DFC_SCROLL,
                                DFCS_SCROLLDOWN | (m_bDownPressed ? DFCS_PUSHED : 0) );

         m_bDownArrow = TRUE;
         pDC->ExcludeClipRect( m_rectDownArrow );
      }
      else
         m_bDownArrow = FALSE;

      PaintItems( pDC, m_nSelFolder, rect );
   }

   dc.BitBlt( rectClient.left, rectClient.top, rectClient.Width( ),
              rectClient.Height( ), &memDC, 0, 0, SRCCOPY );

   memDC.SelectObject( obmp );

   if ( m_nLastFolderHighlighted >= 0 )
   {
      int k = m_nLastFolderHighlighted;
      m_nLastFolderHighlighted = -1;
      HighlightFolder( k );
   }

   if ( m_nLastItemHighlighted >= 0 )
   {
      int k = m_nLastItemHighlighted;
      m_nLastItemHighlighted = -1;
      HighlightItem( k, m_bPressedHighlight );
   }
}

BOOL
ZOutlookBar::OnEraseBkgnd( CDC *pDC )
{
   return( TRUE );
}

bool
ZOutlookBar::GetFolderRect( const int nIdx, CRect & rect ) const
{
   int nMax = m_arFolder.GetSize( );
   ASSERT( nIdx >= 0 && nIdx < nMax );

   if ( nIdx >= 0 && nIdx < nMax )
   {
      CRect rectClient;
      GetClientRect( rectClient );
      if ( nIdx > m_nSelFolder )
         rect.SetRect( rectClient.left,
                       rectClient.bottom - ((nMax - nIdx)) * m_nFolderHeight,
                       rectClient.right,
                       rectClient.bottom - (nMax - nIdx - 1) * m_nFolderHeight);
      else
         rect.SetRect( rectClient.left,
                       rectClient.top + nIdx * m_nFolderHeight - 1,
                       rectClient.right,
                       rectClient.top + (1 + nIdx) * m_nFolderHeight - 1 );

      return( TRUE );
   }

   return( FALSE );
}

void
ZOutlookBar::GetItemRect( const int nFolder, const int nIdx, CRect& rect )
{
   CRect rectInside;
   GetInsideRect( rectInside );
   int top = rectInside.top;
   CSize size( 0, 0 );
   int y = 0;
   for ( int t = 0; t < nIdx; t++ )
   {
      size = GetItemSize( nFolder, t, ircAll );
      top += size.cy;

      if ( IsSmallIconView( ) )
      {
         top += m_nSmallIconSpacingY;
      }
      else
      {
         top += m_nLargeIconSpacingY;
      }

      if ( t == m_nFirstItem - 1 )
         y = top - rectInside.top;
   }

   size = GetItemSize( nFolder, nIdx, ircAll );
   rect.SetRect( rectInside.left, top, rectInside.left + size.cx, top + size.cy );

   rect.top -= y;
   rect.bottom -= y;

   rect.left += m_nLeftMarginX;
   rect.top  += m_nTopMarginY;
   rect.bottom += m_nTopMarginY;

   if ( !IsSmallIconView( ) )
   {
      rect.left = rectInside.left;
      rect.right = rectInside.right;
   }
}

void
ZOutlookBar::DrawFolder( CDC *pDC, const int iIdx,
                         CRect rect, const bool bSelected )
{
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( iIdx );

   if ( !bSelected )
   {
      CPen pn1( PS_SOLID, 1, m_clrBackGroundColor1 );
      CPen *op = pDC->SelectObject( &pn1 );
      pDC->MoveTo( rect.left, rect.top );
      pDC->LineTo( rect.right, rect.top );
      pDC->SelectObject( op );

      rect.top++;

      pDC->Draw3dRect( rect, m_clrHilightBorder, m_clrBackGroundColor1 );
      rect.InflateRect( -1, -1 );
      pDC->FillSolidRect( rect, m_clr3dFace );
      int obk = pDC->SetBkMode( TRANSPARENT );
      CFont *of = pDC->SelectObject( CFont::FromHandle( (HFONT) GetStockObject( DEFAULT_GUI_FONT ) ) );
      pDC->DrawText( CString( pbf->m_pchName ),
                     rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
      pDC->SetBkMode( obk );
      pDC->SelectObject( of );
   }
   else
   {
      CPen pn1( PS_SOLID, 1, m_clrBackGroundColor1 );
      CPen *op = pDC->SelectObject( &pn1 );
      pDC->MoveTo( rect.left + 1, rect.top );
      pDC->LineTo( rect.right, rect.top );
      pDC->SelectObject( op );

      rect.top++;

      pDC->Draw3dRect( rect, m_clrDkShadowBorder, m_clrHilightBorder );
      rect.InflateRect( -1, -1 );
      pDC->Draw3dRect( rect, m_clrBackGroundColor1, m_clr3dFace );
      rect.InflateRect( -1, -1 );

      pDC->FillSolidRect( rect, m_clr3dFace );

      int obk = pDC->SetBkMode( TRANSPARENT );
      CFont *of = pDC->SelectObject( CFont::FromHandle( (HFONT) GetStockObject( DEFAULT_GUI_FONT ) ) );
      pDC->DrawText( CString( pbf->m_pchName ), rect,
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE );
      pDC->SetBkMode( obk );
      pDC->SelectObject( of );
   }
}

int
ZOutlookBar::AddFolder( zCPCHAR cpcFolderName, const DWORD dwData )
{
   ZBarFolder *pbf = new ZBarFolder( cpcFolderName, dwData );
   ASSERT( pbf );
   m_arFolder.Add( (void *) pbf );

   return( m_arFolder.GetSize( ) - 1 );
}

ZOutlookBar::ZBarFolder::ZBarFolder( zCPCHAR cpcName, DWORD exData )
{
   m_pchName = 0;
   m_dwData = exData;

   if ( cpcName )
   {
      zLONG lLth = zstrlen( cpcName ) + 1;
      m_pchName = new char[ lLth ];
      ASSERT( m_pchName );
      strcpy_s( m_pchName, lLth, cpcName );
   }

   m_pLargeImageList = 0;
   m_pSmallImageList = 0;
   m_pChild = 0;
}

ZOutlookBar::ZBarFolder::~ZBarFolder( )
{
   if ( m_pchName )
      delete [] m_pchName;

   for ( int t = 0; t < m_arItems.GetSize( ); t++ )
   {
      if ( m_arItems.GetAt( t ) )
         delete (ZBarFolder *) m_arItems.GetAt( t );
   }

   m_arItems.RemoveAll( );
}

void
ZOutlookBar::GetInsideRect( CRect& rect ) const
{
   GetClientRect( rect );
   if ( m_arFolder.GetSize( ) > 0 )
   {
      int max = m_arFolder.GetSize( );
      rect.top += m_nFolderHeight * (m_nSelFolder + 1) - 1; //+ 2;
      rect.bottom -= (max - m_nSelFolder - 1) * m_nFolderHeight;
      return;
   }
}

void
ZOutlookBar::OnMouseMove( UINT uFlags, CPoint pt )
{
   int nIdx;
   int ht = HitTestEx( pt, nIdx );

   if ( ht != htFolder && m_nLastFolderHighlighted >= 0 )
      HighlightFolder( -1 );

   if ( ht != htItem && m_nLastItemHighlighted   >= 0 )
      HighlightItem( -1 );

   if ( ht == htFolder )
   {
      HighlightFolder( nIdx );
      SetTimer( 1, 100, 0 );
   }
   else
   if ( ht == htItem )
   {
      HighlightItem( nIdx );
      SetTimer( 1, 100, 0 );
   }

   CWnd::OnMouseMove( uFlags, pt );
}

int
ZOutlookBar::HitTestEx( const CPoint& pt, int& nIdx )
{
   if ( m_bUpArrow && m_rectUpArrow.PtInRect( pt ) )
      return( htUpScroll );

   if ( m_bDownArrow && m_rectDownArrow.PtInRect( pt ) )
      return( htDownScroll );

   int max = m_arFolder.GetSize( );
   int t;
   if ( max == 0 )
      return( htNothing );

   CRect rect;
   for ( t = 0; t < max; t++ )
   {
      GetFolderRect( t, rect );
      if ( rect.PtInRect( pt ) )
      {
         nIdx = t;
         return( htFolder );
      }
   }

   GetInsideRect( rect );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   max = pbf->GetItemCount( );
   for ( t = m_nFirstItem; t < max; t++ )
   {
      ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( t );
      CRect itemRc;
      if ( !IsSmallIconView( ) )
      {
         GetIconRect( m_nSelFolder, t, itemRc );
         if ( itemRc.PtInRect( pt ) )
         {
            nIdx = t;
            return( htItem );
         }
         else
         if ( itemRc.top > rect.bottom )
            break;

         GetLabelRect( m_nSelFolder, t, itemRc );
         itemRc.top -= m_nLargeIconLabelOffsetY;

         if ( itemRc.PtInRect( pt ) )
         {
            nIdx = t;
            return( htItem );
         }
         else
         if ( itemRc.top > rect.bottom )
            break;
      }
      else
      {
         GetItemRect( m_nSelFolder, t, itemRc );
         if ( itemRc.PtInRect( pt ) )
         {
            nIdx = t;
            return( htItem );
         }
         else
         if ( itemRc.top > rect.bottom )
            break;
      }
   }

   return( htNothing );
}

void
ZOutlookBar::HighlightFolder( const int nIdx )
{
   CWnd *pf = GetFocus( );
   if ( pf != 0 && pf != this && IsChild( pf ) ) return;

   if ( m_nLastFolderHighlighted == nIdx ) return;

   if ( m_nLastFolderHighlighted >= 0 )
   {
      CRect rect;
      if ( GetFolderRect( m_nLastFolderHighlighted, rect ) )
      {
         CClientDC dc( this );
         CPen pn( PS_SOLID, 1, m_clrBackGroundColor1 );//m_clrShadowBorder );
         CPen *op = dc.SelectObject( &pn );
         dc.MoveTo( rect.left, rect.bottom-1 );
         dc.LineTo( rect.right-1, rect.bottom-1 );
         dc.LineTo( rect.right-1, rect.top );
         CPen pn1( PS_SOLID, 1, m_clr3dFace );
         dc.SelectObject( &pn1 );
         dc.MoveTo( rect.left+1, rect.bottom-2 );
         dc.LineTo( rect.right-2, rect.bottom-2 );
         dc.LineTo( rect.right-2, rect.top+1 );
         dc.SelectObject( op );
      }
   }
   m_nLastFolderHighlighted = nIdx;
   if ( m_nLastFolderHighlighted >= 0 )
   {
      CRect rect;
      if ( GetFolderRect( m_nLastFolderHighlighted, rect ) )
      {
         CClientDC dc( this );
         CPen pn( PS_SOLID, 1, m_clrDkShadowBorder );
         CPen *op = dc.SelectObject( &pn );
         dc.MoveTo( rect.left, rect.bottom-1 );
         dc.LineTo( rect.right-1, rect.bottom-1 );
         dc.LineTo( rect.right-1, rect.top );
         CPen pn1( PS_SOLID, 1, m_clrBackGroundColor1 );
         dc.SelectObject( &pn1 );
         dc.MoveTo( rect.left+1, rect.bottom-2 );
         dc.LineTo( rect.right-2, rect.bottom-2 );
         dc.LineTo( rect.right-2, rect.top+1 );
         dc.SelectObject( op );
      }
   }
}

void
ZOutlookBar::OnLButtonDown( UINT uFlags, CPoint pt )
{
   if ( GetFocus( ) != this )
      SetFocus( );

   CWnd::OnLButtonDown( uFlags, pt );

   int nIdx;
   int ht = HitTestEx( pt, nIdx );
   m_nLastDragItemDrawType = -1;

   CRect inRc;
   GetInsideRect( inRc );

   if ( ht == htFolder )
   {
      bool bHigh = TRUE;
      CRect rect;
      GetFolderRect( nIdx, rect );

      if ( ::GetCapture( ) == 0 )
      {
         SetCapture( );
         ASSERT( this == GetCapture( ) );
         CClientDC dc( this );
         DrawFolder( &dc, nIdx, rect, TRUE );
         AfxLockTempMaps( );
         for ( ; ; )
         {
            MSG msg;
            VERIFY( ::GetMessage( &msg, 0, 0, 0 ) );

            if ( CWnd::GetCapture( ) != this )
               break;

            switch ( msg.message )
            {
            case WM_MOUSEMOVE:
               {
                  CPoint pt( msg.lParam );
                  int idx, ht1 = HitTestEx( pt, idx );
                  if ( ht1 == htFolder && idx == nIdx )
                  {
                     if ( !bHigh )
                     {
                        DrawFolder( &dc, nIdx, rect, TRUE );
                        bHigh = TRUE;
                     }
                  }
                  else
                  {
                     if ( bHigh )
                     {
                        DrawFolder( &dc, nIdx, rect, FALSE );
                        bHigh = FALSE;
                     }
                  }
               }

               break;

            case WM_LBUTTONUP:
               {
                  if ( bHigh )
                  {
                     DrawFolder( &dc, nIdx, rect, FALSE );
                     bHigh = FALSE;
                  }

                  CPoint pt( msg.lParam );
                  int idx, ht1 = HitTestEx( pt, idx );
                  if ( ht1 == htFolder && idx != m_nSelFolder )
                     SetSelFolder( idx );
               }

               goto ExitLoop2;

            case WM_KEYDOWN:
               if ( msg.wParam != VK_ESCAPE )
                  break;

            default:
               DispatchMessage( &msg );
               break;
            }
         }

      ExitLoop2:
         ReleaseCapture( );
         AfxUnlockTempMaps( FALSE );
      }

      if ( bHigh )
         InvalidateRect( rect, FALSE );
   }
   else
      m_nLastSelectedFolder = -1;

   if ( ht == htItem )
   {
      m_nLastDragItemDraw = -1;
      bool bHigh = TRUE, bDragging = FALSE;
      CRect rect;
      GetItemRect( m_nSelFolder, nIdx, rect );

      HCURSOR hCur = GetCursor( );

      if ( ::GetCapture( ) == 0 )
      {
         SetCapture( );
         ASSERT( this == GetCapture( ) );
         CClientDC dc( this );
         HighlightItem( nIdx, TRUE );
         AfxLockTempMaps( );
         for ( ; ; )
         {
            MSG msg;
            VERIFY( ::GetMessage( &msg, 0, 0, 0 ) );

            if ( CWnd::GetCapture( ) != this )
               break;

            switch ( msg.message )
            {
            case WM_MOUSEMOVE:
               {
                  CPoint pt( msg.lParam );
                  int idx, ht1 = HitTestEx( pt, idx );
                  if ( bDragging )
                  {
                     if ( ht1 == htItem )
                     {
                        DrawDragArrow( &dc, nIdx, idx );
                        SetCursor( m_hDragCursor );
                        hCur = m_hDragCursor;
                     }
                     else
                     {
                        CRect rcItem;
                        GetItemRect( m_nSelFolder, GetItemCount( ) - 1, rcItem );
                        if ( pt.y > rcItem.bottom && pt.y < inRc.bottom )
                        {
                           DrawDragArrow( &dc, nIdx, GetItemCount( ) );
                           SetCursor( m_hDragCursor );
                           hCur = m_hDragCursor;
                        }
                        else
                        {
                           DrawDragArrow( &dc, nIdx, -1 );
                           SetCursor( m_hNoDragCursor );
                           hCur = m_hNoDragCursor;
                        }
                     }
                  }
                  else
                  {
                     if ( ht1 == htItem && idx == nIdx )
                     {
                        if ( !bHigh )
                        {
                           HighlightItem( nIdx, TRUE );
                           bHigh = TRUE;
                           m_bPressedHighlight = TRUE;
                        }
                     }
                     else
                     {
                        if ( ht1 == htItem )
                        {
                           if ( bHigh )
                           {
                              HighlightItem( nIdx, FALSE );
                              bHigh = FALSE;
                              m_bPressedHighlight = FALSE;
                           }
                        }
                        else
                        {
                           if ( m_dwFlags & fDragItems )
                           {
                              HighlightItem( nIdx, TRUE );
                              bHigh = TRUE;
                              bDragging = TRUE;

                              SetCursor( m_hDragCursor );
                              hCur = m_hDragCursor;

                              m_bPressedHighlight = TRUE;
                           }
                        }
                     }
                  }
               }
               break;

            case WM_SETCURSOR:
               SetCursor( hCur );
               break;

            case WM_LBUTTONUP:
               {
                  if ( bHigh )
                  {
                     HighlightItem( -1 );
                     bHigh = FALSE;
                  }

                  CPoint pt( msg.lParam );
                  int idx, ht1 = HitTestEx( pt, idx );
                  if ( !bDragging )
                  {
                     if ( ht1 == htItem && idx == nIdx )
                     {
                        if ( m_nSelAnimTiming > 0 && nIdx != m_nLastSel && m_nLastSel >= 0 )
                        {
                           DrawAnimItem( 0, 0, m_nLastSel );
                        }

                        if ( m_dwFlags & fSelHighlight && m_nLastSel >= 0 )
                        {
                           CRect rect;
                           GetIconRect( m_nSelFolder, m_nLastSel, rect );
                           rect.InflateRect( 1, 1 );
                           InvalidateRect( rect );
                        }

                        m_nLastSel = nIdx;

                        if ( m_dwFlags & fSelHighlight && m_nLastSel >= 0 )
                        {
                           CRect rect;
                           GetIconRect( m_nSelFolder, m_nLastSel, rect );
                           rect.InflateRect( 1, 1 );
                           InvalidateRect( rect );
                        }

                        SendMessageToParent( NM_OB_ITEMCLICK, idx );
                     }
                  }
                  else
                  {
                     if ( ht1 == htItem )
                     {
                        if ( idx != nIdx )
                        {
                           OUTBAR_INFO ob;
                           ob.nDragFrom = m_nSelFolder;
                           ob.nDragTo = nIdx;
                           if ( SendMessageToParent( NM_OB_DRAGITEM, (LPARAM) &ob ) )
                           {
                              ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
                              ZBarItem *piFrom = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
                              pbf->m_arItems.RemoveAt( nIdx );
                              if ( idx > nIdx )
                                 idx--;

                              pbf->m_arItems.InsertAt( idx, piFrom );
                           }
                        }
                     }
                     else
                     {
                        CRect rcItem;
                        GetItemRect( m_nSelFolder, GetItemCount( ) - 1, rcItem );
                        if ( pt.y > rcItem.bottom && pt.y < inRc.bottom )
                        {
                           OUTBAR_INFO ob;
                           ob.nDragFrom = m_nSelFolder;
                           ob.nDragTo = nIdx;
                           if ( SendMessageToParent( NM_OB_DRAGITEM, (LPARAM) &ob ) )
                           {
                              ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
                              ZBarItem *piFrom = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
                              pbf->m_arItems.RemoveAt( nIdx );
                              pbf->m_arItems.Add( (void *) piFrom );
                           }
                        }
                     }
                  }
               }

               goto ExitLoop4;

            default:
               DispatchMessage( &msg );
               break;
            }
         }

      ExitLoop4:
         ReleaseCapture( );
         AfxUnlockTempMaps( FALSE );
         if ( bDragging )
         {
            Invalidate( );
         }
      }

      m_bPressedHighlight = FALSE;
      if ( bHigh )
         InvalidateRect( rect, FALSE );
   }
   else
      m_nLastItemHighlighted = -1;

   if ( ht == htDownScroll )
   {
      m_bLooping = TRUE;
      bool bHigh = TRUE;

      if ( ::GetCapture( ) == 0 )
      {
         SetCapture( );
         ASSERT( this == GetCapture( ) );
         CClientDC dc( this );
         dc.DrawFrameControl( m_rectDownArrow, DFC_SCROLL,
                              DFCS_SCROLLDOWN | DFCS_PUSHED );
         SetTimer( 2, 300, 0 );
         AfxLockTempMaps( );
         for ( ;; )
         {
            MSG msg;
            VERIFY( ::GetMessage( &msg, 0, 0, 0 ) );

            if ( CWnd::GetCapture( ) != this )
               break;

            switch ( msg.message )
            {
            case WM_MOUSEMOVE:
               {
                  CPoint pt( msg.lParam );
                  if ( m_rectDownArrow.PtInRect( pt ) )
                  {
                     if ( bHigh == FALSE )
                     {
                        dc.DrawFrameControl( m_rectDownArrow, DFC_SCROLL,
                                             DFCS_SCROLLDOWN | DFCS_PUSHED );
                        bHigh = TRUE;
                        m_bDownArrow = TRUE;
                        m_bDownPressed = TRUE;
                     }
                  }
                  else
                  {
                     if ( bHigh == TRUE )
                     {
                        dc.DrawFrameControl( m_rectDownArrow, DFC_SCROLL,
                                             DFCS_SCROLLDOWN );
                        bHigh = FALSE;
                        m_bDownArrow = FALSE;
                        m_bDownPressed = FALSE;
                     }
                  }
               }

               break;

            case WM_LBUTTONUP:
               {
                  if ( bHigh )
                  {
                     dc.DrawFrameControl( m_rectDownArrow, DFC_SCROLL,
                                          DFCS_SCROLLDOWN );
                     bHigh = FALSE;
                  }
                  m_bDownArrow = FALSE;
                  CPoint pt( msg.lParam );
                  if ( m_rectDownArrow.PtInRect( pt ) )
                  {
                     CRect itrc;
                     GetItemRect( m_nSelFolder, GetItemCount( ) - 1, itrc );
                     CRect crc;
                     GetInsideRect( crc );
                     if ( itrc.bottom > crc.bottom )
                     {
                        m_nFirstItem++;
                        InvalidateRect( crc, TRUE );
                     }
                  }
               }
               goto ExitLoop3;


            case WM_TIMER:
               {
                  if ( msg.wParam == 2 )
                  {
                     if ( bHigh )
                     {
                        CPoint pt( msg.pt );
                        ScreenToClient( &pt );
                        if ( m_rectDownArrow.PtInRect( pt ) )
                        {
                           m_bDownPressed = TRUE;
                           CRect itrc;
                           GetItemRect( m_nSelFolder,
                                        GetItemCount( ) - 1, itrc );
                           CRect crc;
                           GetInsideRect( crc );
                           if ( itrc.bottom > crc.bottom )
                           {
                              m_nFirstItem++;
                              InvalidateRect( crc, TRUE );
                           }
                           else
                              goto ExitLoop3;
                        }
                        else
                           m_bDownPressed = FALSE;
                     }
                  }
                  break;
               }

            case WM_KEYDOWN:
               if ( msg.wParam != VK_ESCAPE )
                  break;

            default:
               DispatchMessage( &msg );
               break;
            }
         }

      ExitLoop3:
         KillTimer( 2 );
         ReleaseCapture( );
         AfxUnlockTempMaps( FALSE );
         m_bLooping = FALSE;
         m_bDownPressed = FALSE;
         m_bDownArrow = FALSE;
         CRect crc;
         GetInsideRect( crc );
         InvalidateRect( crc, TRUE );
      }
   }

   if ( ht == htUpScroll )
   {
      m_bLooping = TRUE;
      bool bHigh = TRUE;

      if ( ::GetCapture( ) == 0 )
      {
         SetCapture( );
         ASSERT( this == GetCapture( ) );
         CClientDC dc( this );
         dc.DrawFrameControl( m_rectUpArrow, DFC_SCROLL,
                              DFCS_SCROLLUP | DFCS_PUSHED );
         SetTimer( 2, 300, 0 );
         AfxLockTempMaps( );
         for ( ;; )
         {
            MSG msg;
            VERIFY( ::GetMessage( &msg, 0, 0, 0 ) );

            if ( CWnd::GetCapture( ) != this )
               break;

            switch ( msg.message )
            {
            case WM_MOUSEMOVE:
               {
                  CPoint pt( msg.lParam );
                  if ( m_rectUpArrow.PtInRect( pt ) )
                  {
                     if ( bHigh == FALSE )
                     {
                        dc.DrawFrameControl( m_rectUpArrow, DFC_SCROLL,
                                             DFCS_SCROLLUP | DFCS_PUSHED );
                        bHigh = TRUE;
                        m_bUpArrow = TRUE;
                        m_bUpPressed = TRUE;
                     }
                  }
                  else
                  {
                     if ( bHigh == TRUE )
                     {
                        dc.DrawFrameControl( m_rectUpArrow,
                                             DFC_SCROLL, DFCS_SCROLLUP );
                        bHigh = FALSE;
                        m_bUpArrow = FALSE;
                        m_bUpPressed = FALSE;
                     }
                  }
               }

               break;

            case WM_LBUTTONUP:
               {
                  if ( bHigh )
                  {
                     dc.DrawFrameControl( m_rectUpArrow,
                                          DFC_SCROLL, DFCS_SCROLLUP );
                     bHigh = FALSE;
                  }

                  m_bUpArrow = FALSE;
                  CPoint pt( msg.lParam );
                  if ( m_rectUpArrow.PtInRect( pt ) )
                  {
                     if ( m_nFirstItem > 0 )
                     {
                        m_nFirstItem--;
                        CRect crc;
                        GetInsideRect( crc );
                        InvalidateRect( crc, TRUE );
                     }
                  }
               }

               goto ExitLoop;

            case WM_TIMER:
               {
                  if ( msg.wParam == 2 )
                  {
                     if ( bHigh )
                     {
                        CPoint pt( msg.pt );
                        ScreenToClient( &pt );
                        if ( m_rectUpArrow.PtInRect( pt ) )
                        {
                           m_bUpPressed = TRUE;
                           if ( m_nFirstItem > 0 )
                           {
                              m_nFirstItem--;
                              CRect crc;
                              GetInsideRect( crc );
                              InvalidateRect( crc, TRUE );
                           }
                           else goto
                              ExitLoop;
                        }
                        else
                           m_bUpPressed = FALSE;
                     }
                  }

                  break;
               }

            case WM_KEYDOWN:
               if ( msg.wParam != VK_ESCAPE )
                  break;

            default:
               DispatchMessage( &msg );
               break;
            }
         }

      ExitLoop:
         KillTimer( 2 );
         ReleaseCapture( );
         AfxUnlockTempMaps( FALSE );
         m_bLooping = FALSE;
         m_bUpPressed = FALSE;
         m_bUpArrow = FALSE;
         CRect crc;
         GetInsideRect( crc );
         InvalidateRect( crc, TRUE );
      }
   }
}

int
ZOutlookBar::InsertItem( const int nFolder, const int nIdx,
                         zCPCHAR cpcText, const int nImage,
                         const DWORD dwData )
{
   ASSERT( nFolder >= 0 && nFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nFolder );
   return( pbf->InsertItem( nIdx, cpcText, nImage, dwData ) );
}

int
ZOutlookBar::GetItemCount( ) const
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   return( pbf->GetItemCount( ) );
}

void
ZOutlookBar::SetSelFolder( const int nIdx )
{
   ASSERT( nIdx >= 0 && nIdx < GetFolderCount( ) );
// if ( nIdx == m_nSelFolder ) return;

   CWnd *pc = GetFolderChild( );
   if ( pc )
      pc->ShowWindow( SW_HIDE );

   if ( nIdx != m_nSelFolder )
   {
      if ( m_dwFlags & fAnimation && m_lAnimationTickCount >= 0 )
         AnimateFolderScroll( m_nSelFolder, nIdx );
   }

   m_nSelFolder = nIdx;
   m_nFirstItem = 0;
   m_nLastSel = -1;

   pc = GetFolderChild( );
   if ( pc )
   {
      CRect rect;
      GetInsideRect( rect );
      pc->MoveWindow( rect );
      pc->ShowWindow( SW_SHOW );
   }

   SendMessageToParent( NM_FOLDERCHANGE, (LPARAM) m_nSelFolder );

   Invalidate( );
}

int
ZOutlookBar::GetFolderCount( ) const
{
   return( m_arFolder.GetSize( ) );
}

int
ZOutlookBar::GetSelFolder( ) const
{
   return( m_nSelFolder );
}

void
ZOutlookBar::RemoveFolder( const int nIdx )
{
   ASSERT( nIdx >= 0 && nIdx < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nIdx );
   delete pbf;
   m_arFolder.RemoveAt( nIdx );
   if ( m_nSelFolder >= nIdx ) m_nSelFolder = nIdx - 1;
   if ( m_nSelFolder < 0 && GetFolderCount( ) > 0 ) m_nSelFolder = 0;
   Invalidate( );
}

int
ZOutlookBar::ZBarFolder::InsertItem( int nIdx, zCPCHAR text,
                                     const int image, const DWORD exData )
{
   if ( nIdx < 0 || nIdx > GetItemCount( ) )
      nIdx = GetItemCount( );

   int c = GetItemCount( );

   ZBarItem *pbf = new ZBarItem( text, image, exData );
   ASSERT( pbf );
   if ( nIdx < GetItemCount( ) )
      m_arItems.InsertAt( nIdx, (void *) pbf );
   else
      m_arItems.Add( (void *) pbf );

   c = GetItemCount( );

   return( nIdx );
}

int
ZOutlookBar::GetCountPerPage( ) const
{
   return( 0 );
}

CImageList *
ZOutlookBar::SetImageList( CImageList *pImageList, int nImageList )
{
   CImageList *o = 0;
   if ( nImageList == fSmallIcon )
   {
      o = m_pSmallImageList;
      m_pSmallImageList = pImageList;
   }
   else
   if ( nImageList == fLargeIcon )
   {
      o = m_pLargeImageList;
      m_pLargeImageList = pImageList;
   }

   return( o );
}

CImageList *
ZOutlookBar::GetImageList( CImageList *pImageList, int nImageList )
{
   if ( nImageList == fSmallIcon )
      return( m_pSmallImageList );
   else
   if ( nImageList == fLargeIcon )
      return( m_pLargeImageList );

   return( 0 );
}

CImageList *
ZOutlookBar::SetFolderImageList( const int folder, CImageList *pImageList,
                                 int nImageList )
{
   ASSERT( folder >= 0 && folder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( folder );
   CImageList *o = 0;
   if ( nImageList == fSmallIcon )
   {
      o = pbf->m_pSmallImageList;
      pbf->m_pSmallImageList = pImageList;
   }
   else
   if ( nImageList == fLargeIcon )
   {
      o = pbf->m_pLargeImageList;
      pbf->m_pLargeImageList = pImageList;
   }

   return( o );
}

ZOutlookBar::ZBarItem::ZBarItem( zCPCHAR cpcName, const int image,
                                 DWORD dwData )
{
   m_pchItem = 0;
   m_nImageIdx = image;
   m_dwData = dwData;
   if ( cpcName )
   {
      zLONG lLth = zstrlen( cpcName ) + 1;
      m_pchItem = new char[ lLth ];
      ASSERT( m_pchItem );
      strcpy_s( m_pchItem, lLth, cpcName );
   }
}

ZOutlookBar::ZBarItem::~ZBarItem( )
{
   if ( m_pchItem )
      delete [] m_pchItem;
}

int
ZOutlookBar::ZBarFolder::GetItemCount( )
{
   return( m_arItems.GetSize( ) );
}

void
ZOutlookBar::PaintItems( CDC *pDC, const int nFolder, CRect rect )
{
   ASSERT( nFolder >= 0 && nFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nFolder );
   int max = pbf->GetItemCount( ), t;
   CRect rectInside;
   GetInsideRect( rectInside );
   int isdc = pDC->SaveDC( );
   CRgn itemRegion;
   itemRegion.CreateRectRgnIndirect( &rectInside );
   pDC->SelectClipRgn( &itemRegion );
   itemRegion.DeleteObject( );
   for ( t = m_nFirstItem; t < max; t++ )
   {
      ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( t );
      CRect itemRc;
      GetItemRect( nFolder, t, itemRc );
      if ( itemRc.top > rect.bottom )
         break;
      else
         DrawItem( pDC, nFolder, itemRc, t );
   }

   pDC->RestoreDC( isdc );
}


CSize
ZOutlookBar::GetItemSize( const int nFolder, const int nIdx, const int type )
{
   ASSERT( nFolder >= 0 && nFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nFolder );

   ASSERT( nIdx >= 0 && nIdx < pbf->GetItemCount( ) );
   ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );

   CSize m_sizeImage( 0, 0 );
   CSize szLabel( 0, 0 );
   CSize szAll( 0, 0 );

   if ( pi->m_nImageIdx >= 0 )
   {
      if ( type != ircLabel )
      {
         CImageList *il = GetFolderImageList( nFolder, IsSmallIconView( ) );
         ASSERT( il );
         if ( il )
         {
            IMAGEINFO ii;
            il->GetImageInfo( pi->m_nImageIdx, &ii );
            m_sizeImage = CRect( ii.rcImage ).Size( );
         }
      }
   }

   if ( pi->m_pchItem )
   {
      if ( type != ircIcon )
      {
         CClientDC dc( this );
         CFont *oft = (CFont *) dc.SelectObject( CFont::
                  FromHandle( (HFONT) GetStockObject( DEFAULT_GUI_FONT ) ) );
         if ( IsSmallIconView( ) )
         {
            szLabel = dc.GetTextExtent( pi->m_pchItem,
                                        zstrlen( pi->m_pchItem ) );
         }
         else
         {
            CRect rect;
            GetInsideRect( rect );
            rect.bottom = rect.top;
            dc.DrawText( pi->m_pchItem, zstrlen( pi->m_pchItem ), rect,
                         DT_CALCRECT | DT_CENTER | DT_WORDBREAK );
            szLabel = rect.Size( );
         }

         dc.SelectObject( oft );
      }
   }

   if ( IsSmallIconView( ) )
   {
      if ( type == ircIcon )
         szAll = m_sizeImage;
      else
      if ( type == ircLabel )
         szAll = szLabel;
      else
      if ( type == ircAll )
         szAll = CSize( m_sizeImage.cx + szLabel.cx +
                          m_nSmallIconLabelOffsetX,
                        m_sizeImage.cy > szLabel.cy ?
                          m_sizeImage.cy : szLabel.cy );
   }
   else
   {
      if ( type == ircIcon )
         szAll = m_sizeImage;
      else
      if ( type == ircLabel )
         szAll = szLabel;
      else
      if ( type == ircAll )
      {
         szAll = CSize( m_sizeImage.cx > szLabel.cx ?
                          m_sizeImage.cx : szLabel.cx,
                        szLabel.cy + m_sizeImage.cy +
                          m_nLargeIconLabelOffsetY + m_nLargeIconSpacingY );
      }
   }

   return( szAll );
}

CImageList *
ZOutlookBar::GetFolderImageList( const int nIdx, const bool bSmall ) const
{
   ASSERT( nIdx >= 0 && nIdx < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nIdx );
   if ( bSmall )
   {
      if ( pbf->m_pSmallImageList )
         return( pbf->m_pSmallImageList );
      else
         return( m_pSmallImageList );
   }

   if ( pbf->m_pLargeImageList )
      return( pbf->m_pLargeImageList );
   else
      return( m_pLargeImageList );
}

void
ZOutlookBar::DrawItem( CDC *pDC, const int nFolder, CRect rect,
                          const int nIdx, const bool bOnlyImage )
{
   CImageList *ima = GetFolderImageList( nFolder, IsSmallIconView( ) );

   ASSERT( nFolder >= 0 && nFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nFolder );

   ASSERT( nIdx >= 0 && nIdx < pbf->GetItemCount( ) );
   ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );

   ASSERT( pi && ima );

   CFont *oft = (CFont *) pDC->SelectObject( CFont::
                  FromHandle( (HFONT) GetStockObject( DEFAULT_GUI_FONT ) ) );
   COLORREF ocr = pDC->SetTextColor( m_clrTextColor );
   int obk = pDC->SetBkMode( TRANSPARENT );

   if ( IsSmallIconView( ) )
   {
      if ( ima )
      {
         IMAGEINFO ii;
         ima->GetImageInfo( pi->m_nImageIdx, &ii );
         CSize m_sizeImage = CRect( ii.rcImage ).Size( );
         CPoint pt;
         pt.x = rect.left + 2;
         pt.y = rect.top + ( rect.Height( ) - m_sizeImage.cy ) / 2;
         ima->Draw( pDC, pi->m_nImageIdx, pt, ILD_NORMAL );

         if ( !bOnlyImage )
         {
            rect.left += m_sizeImage.cx + m_nSmallIconLabelOffsetX;
            pDC->TextOut( rect.left, rect.top, CString( pi->m_pchItem ) );
         }
      }
   }
   else
   {
      if ( ima )
      {
         IMAGEINFO ii;
         ima->GetImageInfo( pi->m_nImageIdx, &ii );
         CSize m_sizeImage = CRect( ii.rcImage ).Size( );
         CPoint pt;
         pt.x = rect.left + ( rect.Width( ) - m_sizeImage.cx ) / 2;
         pt.y = rect.top;// + ( rect.Height( ) - m_sizeImage.cy ) / 2;
         ima->Draw( pDC, pi->m_nImageIdx, pt, ILD_NORMAL );
         if ( !bOnlyImage )
         {
            rect.top += m_sizeImage.cy + m_nLargeIconLabelOffsetY;
            pDC->DrawText( pi->m_pchItem, zstrlen( pi->m_pchItem ), rect,
                           DT_CENTER | DT_WORDBREAK );
         }
      }
   }

   if ( m_dwFlags & fSelHighlight && m_nLastSel == nIdx && m_nLastSel >= 0 )
   {
      CRect rect;
      GetIconRect( m_nSelFolder, m_nLastSel, rect );
      rect.InflateRect( 1, 1 );
      pDC->Draw3dRect( rect, m_clrDkShadowBorder, m_clr3dFace );
   }

   pDC->SetTextColor( ocr );
   pDC->SelectObject( oft );
   pDC->SetBkMode( obk );

}


BOOL
ZOutlookBar::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
   CPoint pt( GetMessagePos( ) );
   ScreenToClient( &pt );
   int nIdx = 0;
   int ht = HitTestEx( pt, nIdx );
   if ( ht == htFolder )
   {
      SetCursor( m_hHandCursor );
      return( TRUE );
   }

   return( CWnd::OnSetCursor( pWnd, nHitTest, message ) );
}

void
ZOutlookBar::GetVisibleRange( const int nFolder, int& first, int& last )
{
   last = first = m_nFirstItem;
   CRect rect;
   GetInsideRect( rect );

   if ( nFolder < m_arFolder.GetSize( ) )
   {
      ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nFolder );
      int max = pbf->GetItemCount( ), t;
      for ( t = m_nFirstItem; t < max; t++ )
      {
         ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( t );
         CRect itemRc;
         GetItemRect( nFolder, t, itemRc );
         if ( itemRc.bottom > rect.bottom )
         {
            last = t - 1;
            break;
         }
         else
            last = t;
      }
   }
}

void
ZOutlookBar::OnSize( UINT nType, int cx, int cy )
{
   m_bUpArrow = m_bDownArrow = FALSE;

   CWnd::OnSize( nType, cx, cy );

   int t, max = GetFolderCount( );
   CRect rect;
   GetInsideRect( rect );

   for ( t = 0; t < max; t++ )
   {
      ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( t );
      CWnd *pc = GetFolderChild( t );
      if ( pc )
      {
         pc->SetWindowPos( 0, rect.left, rect.top,
                           rect.Width( ), rect.Height( ), SWP_NOZORDER );
      }
   }
}

void
ZOutlookBar::HighlightItem( const int nIdx, const bool bPressed )
{
   CWnd *pf = GetFocus( );
   if ( pf != 0 && pf != this && IsChild( pf ) ) return;

   static bool bOldPressed = FALSE;
   if ( m_nLastItemHighlighted == nIdx && bOldPressed == bPressed ) return;

   bOldPressed = bPressed;

   CClientDC dc( this );

   CRect rectInside;
   GetInsideRect( rectInside );
   int isdc = dc.SaveDC( );
   CRgn itemRegion;
   itemRegion.CreateRectRgnIndirect( &rectInside );
   dc.SelectClipRgn( &itemRegion );
   itemRegion.DeleteObject( );


   if ( m_nLastItemHighlighted >= 0 && IsValidItem( m_nLastItemHighlighted ) )
   {
      CRect rect;
      GetIconRect( m_nSelFolder, m_nLastItemHighlighted, rect );
      rect.InflateRect( 1, 1 );
      dc.Draw3dRect( rect, m_clrBackGroundColor, m_clrBackGroundColor );
   }

   if ( m_nSelAnimTiming > 0 && nIdx == m_nLastSel )
   {
      m_nLastItemHighlighted = -1;
      return;
   }
   if ( m_dwFlags & fSelHighlight && m_nLastSel == nIdx )
   {
      m_nLastItemHighlighted = -1;
      return;
   }

   m_nLastItemHighlighted = nIdx;
   if ( m_nLastItemHighlighted >= 0 && IsValidItem( m_nLastItemHighlighted ) )
   {
      CRect rect;
      GetIconRect( m_nSelFolder, m_nLastItemHighlighted, rect );
      rect.InflateRect( 1, 1 );
      if ( bPressed )
         dc.Draw3dRect( rect, m_clrDkShadowBorder, m_clr3dFace );
      else
         dc.Draw3dRect( rect, m_clr3dFace, m_clrDkShadowBorder );
   }

   dc.RestoreDC( isdc );
}

void
ZOutlookBar::GetIconRect( const int nFolder, const int nIdx, CRect& rect )
{
   CRect rectInside;
   GetInsideRect( rectInside );
   int top = rectInside.top;
   CSize size( 0, 0 );
   int y = 0;
   int nSpacing = IsSmallIconView( ) ?
                                 m_nSmallIconSpacingY : m_nLargeIconSpacingY;

   for ( int t = 0; t < nIdx; t++ )
   {
      size = GetItemSize( nFolder, t, ircAll );
      top += size.cy;
      top += nSpacing;
      if ( t == m_nFirstItem - 1 )
         y = top - rectInside.top;
   }

   top += nSpacing;

   size = GetItemSize( nFolder, nIdx, ircIcon );
   if ( IsSmallIconView( ) )
   {
      rect.SetRect( rectInside.left, top,
                    rectInside.left + size.cx, top + size.cy );
      rect.left += m_nLeftMarginX + 2;
      rect.right += m_nLeftMarginX + 2;
      rect.top -= m_nTopMarginY;
      rect.bottom -= m_nTopMarginY;

      rect.top -= y;
      rect.bottom -= y;
   }
   else
   {
      rect.SetRect( rectInside.left + (rectInside.Width( ) - size.cx) / 2, top,
                    rectInside.left + (rectInside.Width( ) - size.cx) / 2 + size.cx,
                    top + size.cy );
      rect.top -= y + 3;
      rect.bottom -= y + 2;
   }
}

void
ZOutlookBar::GetLabelRect( const int nFolder, const int nIdx, CRect& rect )
{
   CRect rectInside;
   GetInsideRect( rectInside );
   int top = rectInside.top;
   CSize size( 0, 0 );
   int y = 0;
   int nSpacing = IsSmallIconView( ) ? m_nSmallIconSpacingY : m_nLargeIconSpacingY;

   for ( int t = 0; t < nIdx; t++ )
   {
      size = GetItemSize( nFolder, t, ircAll );
      top += size.cy;
      top += nSpacing;
      if ( t == m_nFirstItem - 1 )
         y = top - rectInside.top;
   }

   top += nSpacing;

   size = GetItemSize( nFolder, nIdx, ircAll );

   CSize szLabel = GetItemSize( nFolder, nIdx, ircLabel );

   if ( IsSmallIconView( ) )
   {
      rect.SetRect( rectInside.left, top, rectInside.left + size.cx, top + size.cy );
      rect.left += m_nLeftMarginX + 2;
      rect.right += m_nLeftMarginX + 2;
      rect.top  += m_nTopMarginY - 5;
      rect.bottom += m_nTopMarginY - 5;
   }
   else
   {
      rect.SetRect( rectInside.left + (rectInside.Width( ) - size.cx) / 2, top,
                    rectInside.left + (rectInside.Width( ) - size.cx) / 2 + size.cx,
                    top + size.cy );
      rect.top -= y + 3;
      rect.bottom -= y + 2;

      rect.bottom -=  m_nLargeIconSpacingY;
      rect.top = rect.bottom - szLabel.cy;
   }
}

void
ZOutlookBar::StartGroupEdit( const int nIdx )
{
   ZOLB_Edit *pEdit = new ZOLB_Edit;
   pEdit->m_nIdx = nIdx;
   CRect rect;
   GetFolderRect( nIdx, rect );
   rect.InflateRect( -2, -2 );
   pEdit->Create( WS_CHILD | WS_VISIBLE | ES_CENTER | ES_MULTILINE,
                  rect, this, 1 );
   pEdit->m_msgSend = NM_OB_ONGROUPENDEDIT;
   pEdit->ModifyStyleEx( 0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED );
   ASSERT( nIdx >= 0 && nIdx < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nIdx );
   pEdit->SetWindowText( pbf->m_pchName );
   pEdit->SetFocus( );
}

void
ZOutlookBar::StartItemEdit( const int nIdx )
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   ASSERT( nIdx >= 0 && nIdx < pbf->GetItemCount( ) );
   ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );

   ZOLB_Edit *pEdit = new ZOLB_Edit;
   pEdit->m_nIdx = nIdx;
   CRect rect;
   CRect crc;
   GetLabelRect( m_nSelFolder, nIdx, rect );
   GetInsideRect( crc );

   DWORD dwFlag = WS_CHILD | WS_VISIBLE | WS_BORDER;

   if ( !IsSmallIconView( ) )
   {
      rect.left = crc.left + 5;
      rect.right = crc.right - 5;
      rect.bottom += 5;
      dwFlag |= ES_CENTER | ES_MULTILINE;
   }
   else
   {
      CRect rectInside;
      GetIconRect( m_nSelFolder, nIdx, rectInside );
      dwFlag |= ES_AUTOHSCROLL;
      rect.OffsetRect( rectInside.Width( ) + 1, -6 );
      pEdit->m_bNoDown = TRUE;
   }

   pEdit->Create( dwFlag, rect, this, 1 );
   pEdit->m_msgSend = NM_OB_ONLABELENDEDIT;

   pEdit->SetWindowText( pi->m_pchItem );
   pEdit->SetFocus( );
}

void
ZOutlookBar::OnRButtonDown( UINT uFlags, CPoint pt )
{
   m_nHitInternal1 = HitTestEx( pt, m_nHitInternal2 );

   ZOLB_PopupMenu cMenu;
   cMenu.CreatePopupMenu( );

   cMenu.AppendMenu( MF_STRING, ID_GFX_SMALLICON, _ID_GFX_SMALLICON );
   cMenu.AppendMenu( MF_STRING, ID_GFX_LARGEICON, _ID_GFX_LARGEICON );

   if ( m_nHitInternal1 == htItem )
   {
      if ( m_dwFlags & fRemoveItems || m_dwFlags & fEditItems )
      {
         cMenu.AppendMenu( MF_SEPARATOR );
         if ( m_dwFlags & fRemoveItems )
            cMenu.AppendMenu( MF_STRING, ID_GFX_REMOVEITEM, _ID_GFX_REMOVEITEM );

         if ( m_dwFlags & fEditItems )
            cMenu.AppendMenu( MF_STRING, ID_GFX_RENAMEITEM, _ID_GFX_RENAMEITEM );
      }
   }
   else
   if ( m_nHitInternal1 == htFolder )
   {
      if ( m_dwFlags & fRemoveGroups || m_dwFlags & fEditGroups )
      {
         cMenu.AppendMenu( MF_SEPARATOR );

         if ( m_dwFlags & fRemoveGroups )
            cMenu.AppendMenu( MF_STRING, ID_GFX_REMOVEITEM, _ID_GFX_REMOVEITEM );

         if ( m_dwFlags & fEditGroups )
            cMenu.AppendMenu( MF_STRING, ID_GFX_RENAMEITEM, _ID_GFX_RENAMEITEM );
      }
   }

   cMenu.LoadToolBarResource( IDR_MAINFRAME );
   cMenu.RemapMenu( &cMenu );
   cMenu.EnableMenuItems( &cMenu, this );

   CPoint pt1( pt );
   ClientToScreen( &pt1 );
   cMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt1.x, pt1.y, this );

   cMenu.DestroyMenu( );

   CWnd::OnRButtonDown( uFlags, pt );
}

void
ZOutlookBar::OnGfxLargeicon( )
{
   SetSmallIconView( FALSE );
   Invalidate( );
}

void
ZOutlookBar::OnUpdateGfxLargeicon( CCmdUI* pCmdUI )
{
   pCmdUI->Enable( IsSmallIconView( ) );
}

void
ZOutlookBar::OnGfxSmallicon( )
{
   SetSmallIconView( TRUE );
   Invalidate( );
}

void
ZOutlookBar::OnUpdateGfxSmallicon( CCmdUI* pCmdUI )
{
   pCmdUI->Enable( !IsSmallIconView( ) );
}

void
ZOutlookBar::OnGfxRemoveitem( )
{
   if ( m_nHitInternal1 == htFolder )
   {
      RemoveFolder( m_nHitInternal2 );
   }
   else if ( m_nHitInternal1 == htItem )
   {
      RemoveItem( m_nHitInternal2 );
   }
}

void
ZOutlookBar::OnUpdateGfxRemoveitem( CCmdUI *pCmdUI )
{

}

void
ZOutlookBar::OnGfxRenameitem( )
{
   if ( m_nHitInternal1 == htFolder )
   {
      StartGroupEdit( m_nHitInternal2 );
   }
   else
   if ( m_nHitInternal1 == htItem )
   {
      StartItemEdit( m_nHitInternal2 );
   }
}

void
ZOutlookBar::OnUpdateGfxRenameitem( CCmdUI *pCmdUI )
{
}

void
ZOutlookBar::RemoveItem( const int nIdx )
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   ASSERT( nIdx >= 0 && nIdx < pbf->GetItemCount( ) );
   if ( IsValidItem( nIdx ) )
   {
      ZBarItem *k = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
      delete k;

      pbf->m_arItems.RemoveAt( nIdx );
      CRect rect;
      GetInsideRect( rect );
      InvalidateRect( rect );
   }
}

bool
ZOutlookBar::IsValidItem( const int nIdx ) const
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   return( nIdx >= 0 && nIdx < pbf->GetItemCount( ) );
}

DWORD
ZOutlookBar::GetItemData( const int nIdx ) const
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   if ( IsValidItem( nIdx ) )
   {
      ZBarItem *pbi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
      return( pbi->m_dwData );
   }

   return( 0 );
}

int
ZOutlookBar::GetItemImage( const int nIdx ) const
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   if ( IsValidItem( nIdx ) )
   {
      ZBarItem *pbi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
      return( pbi->m_nImageIdx );
   }

   return( 0 );
}

void
ZOutlookBar::SetItemData( const int nIdx, const DWORD dwData )
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   if ( IsValidItem( nIdx ) )
   {
      ZBarItem *pbi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
      pbi->m_dwData = dwData;
   }
}

void
ZOutlookBar::SetItemImage( const int nIdx, const int nImage )
{
   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   if ( IsValidItem( nIdx ) )
   {
      ZBarItem *pbi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
      pbi->m_nImageIdx = nImage;
   }
}

void
ZOutlookBar::DrawDragArrow( CDC *pDC, const int iFrom, const int iTo )
{
   if ( iTo == m_nLastDragItemDraw )
      return;

   CRect rc1;
   if ( m_nLastDragItemDraw >= 0 )
   {
      GetDragItemRect( m_nLastDragItemDraw, rc1 );
      if ( m_nLastDragItemDrawType == 0 )
         rc1.bottom = rc1.top + 5;
      else
      if ( m_nLastDragItemDrawType == 1 )
      {
         rc1.top -= 4;
         rc1.bottom = rc1.top + 9;
      }
      else
      if ( m_nLastDragItemDrawType == 2 )
      {
         rc1.top -= 4;
         rc1.bottom = rc1.top + 5;
      }

      InvalidateRect( rc1, TRUE );
      UpdateWindow( );
   }

   m_nLastDragItemDraw = iTo;

   if ( m_nLastDragItemDraw >= 0 )
   {
      m_nLastDragItemDrawType = GetDragItemRect( m_nLastDragItemDraw, rc1 );

      CPen *op = pDC->SelectObject( m_pBlackPen );

      pDC->MoveTo( rc1.left, rc1.top );
      pDC->LineTo( rc1.right, rc1.top );

      if ( m_nLastDragItemDrawType != 2 )
      {
         pDC->MoveTo( rc1.left, rc1.top + 1 );
         pDC->LineTo( rc1.left + 7, rc1.top + 1 );
         pDC->MoveTo( rc1.left, rc1.top + 2 );
         pDC->LineTo( rc1.left + 5, rc1.top + 2 );
         pDC->MoveTo( rc1.left, rc1.top + 3 );
         pDC->LineTo( rc1.left + 3, rc1.top + 3 );
         pDC->MoveTo( rc1.left, rc1.top + 4 );
         pDC->LineTo( rc1.left + 1, rc1.top + 4 );

         pDC->MoveTo( rc1.right - 1, rc1.top + 1 );
         pDC->LineTo( rc1.right - 7, rc1.top + 1 );
         pDC->MoveTo( rc1.right - 1, rc1.top + 2 );
         pDC->LineTo( rc1.right - 5, rc1.top + 2 );
         pDC->MoveTo( rc1.right - 1, rc1.top + 3 );
         pDC->LineTo( rc1.right - 3, rc1.top + 3 );
      }

      if ( m_nLastDragItemDrawType != 0 )
      {
         pDC->MoveTo( rc1.left, rc1.top - 1 );
         pDC->LineTo( rc1.left + 7, rc1.top - 1 );
         pDC->MoveTo( rc1.left, rc1.top-2 );
         pDC->LineTo( rc1.left + 5, rc1.top - 2 );
         pDC->MoveTo( rc1.left, rc1.top-3 );
         pDC->LineTo( rc1.left + 3, rc1.top - 3 );
         pDC->MoveTo( rc1.left, rc1.top-4 );
         pDC->LineTo( rc1.left + 1, rc1.top - 4 );

         pDC->MoveTo( rc1.right - 1, rc1.top - 1 );
         pDC->LineTo( rc1.right - 7, rc1.top - 1 );
         pDC->MoveTo( rc1.right - 1, rc1.top - 2 );
         pDC->LineTo( rc1.right - 5, rc1.top - 2 );
         pDC->MoveTo( rc1.right - 1, rc1.top - 3 );
         pDC->LineTo( rc1.right - 3, rc1.top - 3 );
         pDC->MoveTo( rc1.right - 1, rc1.top - 4 );
         pDC->LineTo( rc1.right - 1, rc1.top - 4 );
      }

      pDC->SelectObject( op );
   }
}

int
ZOutlookBar::GetDragItemRect( const int nIdx, CRect& rect )
{
   CRect rectItem;
   CRect crc;
   GetInsideRect( crc );
   crc.InflateRect( -2, 0 );

   GetItemRect( m_nSelFolder,
                nIdx < GetItemCount( ) ? nIdx : nIdx - 1, rectItem );
   int line = 0;
   if ( nIdx < GetItemCount( ) )
   {
      line = rectItem.top - 4;
   }
   else
   {
      line = rectItem.bottom + 6;
   }
   int tpe;
   if ( nIdx == 0 )
   {
      rect.SetRect( crc.left, line+2, crc.right, line + 7 );
      tpe = 0;
   }
   else
   if ( nIdx < GetItemCount( ) )
   {
      rect.SetRect( crc.left, line - 9, crc.right, line );
      tpe = 1;
      if ( IsSmallIconView( ) )
      {
         rect.top += 8;
         rect.bottom += 8;
      }
   }
   else
   {
      rect.SetRect( crc.left, line, crc.right, line + 5 );
      tpe = 2;
   }

   return( tpe );
}

void
ZOutlookBar::AnimateFolderScroll( const int iFrom, const int iTo )
{
   ASSERT( iFrom >= 0 && iFrom < GetFolderCount( ) );
   ASSERT( iTo >= 0 && iTo < GetFolderCount( ) );

   CRect rect, rc1, frc, frc1;
   GetInsideRect( rect );
   rc1.SetRect( 0, 0, rect.Width( ), rect.Height( ) );
   GetFolderRect( iTo, frc );
   frc1 = frc;

   CClientDC dc( this );
   CDC memDC;
   memDC.CreateCompatibleDC( &dc );
   CBitmap bmpFrom, bmpTo;
   bmpFrom.CreateCompatibleBitmap( &dc, rc1.Width( ), rc1.Height( ) + frc.Height( ) * 2 );
   bmpTo.CreateCompatibleBitmap( &dc, rc1.Width( ), rc1.Height( ) + frc.Height( ) * 2 );
   CDC *pDC = &memDC;

   CWnd *pWnd1 = GetFolderChild( iFrom );
   CWnd *pWnd2 = GetFolderChild( iTo );

   CBitmap *obmp = pDC->SelectObject( &bmpFrom );
   CBitmap *pBmpDC;
   if ( iTo > iFrom )
   {
      rc1.bottom += frc.Height( ) * 2;
      pDC->FillSolidRect( rc1, m_clrBackGroundColor );
      if ( pWnd1 )
      {
         BOOL bPrev = pWnd1->ShowWindow( SW_SHOW );
         pWnd1->SendMessage( WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         pWnd1->SendMessage( WM_PAINT, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         if ( bPrev == FALSE )
            pWnd1->ShowWindow( SW_HIDE );
      }
      else
         PaintItems( pDC, iFrom, rc1 );

      pBmpDC = pDC->SelectObject( &bmpTo );
      frc.SetRect( 0, 0, frc.Width( ), frc.Height( ) );
      rc1.SetRect( 0, frc.Height( ), rect.Width( ), rect.Height( ) + frc.Height( ) * 2 );
      pDC->FillSolidRect( rc1, m_clrBackGroundColor );

      if ( pWnd2 )
      {
         CPoint ovpt = pDC->SetViewportOrg( 0, frc.Height( ) );
         BOOL bPrev = pWnd2->ShowWindow( SW_SHOW );
         pWnd2->SendMessage( WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         pWnd2->SendMessage( WM_PAINT, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         if ( bPrev == FALSE )
            pWnd2->ShowWindow( SW_HIDE );

         pDC->SetViewportOrg( ovpt );
      }
      else
         PaintItems( pDC, iTo, rc1 );

      DrawFolder( pDC, iTo, frc, FALSE );
      pDC->SelectObject( pBmpDC );
   }
   else
   {
      frc.SetRect( 0, 0,frc.Width( ),frc.Height( ) );
      rc1.top += frc.Height( );
      rc1.bottom += frc.Height( ) * 2;
      pDC->FillSolidRect( rc1, m_clrBackGroundColor );
      if ( pWnd1 )
      {
         CPoint ovpt = pDC->SetViewportOrg( 0, frc.Height( ) );
         BOOL bPrev = pWnd1->ShowWindow( SW_SHOW );
         pWnd1->SendMessage( WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         pWnd1->SendMessage( WM_PAINT, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         if ( bPrev == FALSE )
            pWnd1->ShowWindow( SW_HIDE );

         pDC->SetViewportOrg( ovpt );
      }
      else
         PaintItems( pDC, iFrom, rc1 );

      DrawFolder( pDC, iFrom, frc, FALSE );

      pBmpDC = pDC->SelectObject( &bmpTo );
      rc1.SetRect( 0, 0,rect.Width( ), rect.Height( ) + frc.Height( ) * 2 );
      pDC->FillSolidRect( rc1, m_clrBackGroundColor );

      if ( pWnd2 )
      {
         BOOL bPrev = pWnd2->ShowWindow( SW_SHOW );
         pWnd2->SendMessage( WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         pWnd2->SendMessage( WM_PAINT, (WPARAM) pDC->GetSafeHdc( ), (LPARAM) 0 );
         if ( bPrev == FALSE )
            pWnd2->ShowWindow( SW_HIDE );
      }
      else
         PaintItems( pDC, iTo, rc1 );

      pDC->SelectObject( pBmpDC );
   }

   pDC->SelectObject( obmp );

   if ( iTo > iFrom )
   {
      CRect rcFrom, rcTo;
      GetFolderRect( iFrom, rcFrom );
      GetFolderRect( iTo, rcTo );
      int fh = rcFrom.Height( );
      for ( int y = rcTo.top - fh; y > rcFrom.bottom; y -= fh )
      {
         pBmpDC = pDC->SelectObject( &bmpFrom );
         dc.BitBlt( rect.left, rcFrom.bottom + 1, rect.Width( ),
                    y - rcFrom.bottom - 1, pDC, 0, fh, SRCCOPY );
         pDC->SelectObject( &bmpTo );
         dc.BitBlt( rect.left, y, rect.Width( ), rect.bottom - y + fh,
                    pDC, 0, 0, SRCCOPY );
         pDC->SelectObject( pBmpDC );
         Sleep( m_lAnimationTickCount );
      }
   }
   else
   {
      CRect rcFrom, rcTo;
      GetFolderRect( iFrom, rcFrom );
      int fh = rcFrom.Height( );
      rcTo.SetRect( rect.left, rect.bottom, rect.right, rect.bottom - fh );
      for ( int y = rcFrom.top + 1; y < rcTo.top - fh; y += fh )
      {
         pBmpDC = pDC->SelectObject( &bmpTo );
         dc.BitBlt( rect.left, rcFrom.top, rect.Width( ), y - rcFrom.top - 1,
                    pDC, 0, fh * 2, SRCCOPY );
         pDC->SelectObject( &bmpFrom );
         dc.BitBlt( rect.left, y, rect.Width( ), rect.bottom - y,
                    pDC, 0, 0, SRCCOPY );
         pDC->SelectObject( pBmpDC );
         Sleep( m_lAnimationTickCount );
      }
   }
}

CString
ZOutlookBar::GetItemText( const int nIdx )
{
   CString csText;

   ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( m_nSelFolder );
   if ( IsValidItem( nIdx ) )
   {
      ZBarItem *pi = (ZBarItem *) pbf->m_arItems.GetAt( nIdx );
      if ( pi->m_pchItem )
         csText = pi->m_pchItem;
   }

   return( csText );
}

int
ZOutlookBar::AddFolderBar( zCPCHAR cpcFolder, CWnd *pSon,
                           const DWORD dwData )
{
   ZBarFolder *pbf = new ZBarFolder( cpcFolder, dwData );
   ASSERT( pbf );
   pSon->ShowWindow( SW_HIDE );
   pSon->SetParent( this );
   pbf->m_pChild = pSon;

   m_arFolder.Add( (void *) pbf );

   return( m_arFolder.GetSize( ) - 1 );
}

CWnd *
ZOutlookBar::GetFolderChild( int nFolder )
{
   if ( GetFolderCount( ) )
   {
      if ( nFolder < 0 )
         nFolder = m_nSelFolder;

      ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nFolder );
      return( pbf->m_pChild );
   }

   return( 0 );
}

DWORD
ZOutlookBar::GetFolderData( int nFolder )
{
   if ( nFolder < 0 )
      nFolder = m_nSelFolder;

   ZBarFolder *pbf = (ZBarFolder *) m_arFolder.GetAt( nFolder );
   return( pbf->m_dwData );
}

void
ZOutlookBar::SetAnimSelHighlight( const int nTime )
{
   if ( nTime <= 0 )
      KillTimer( 3 );
   else
      SetTimer( 3, nTime, 0 );

   m_nSelAnimTiming = nTime;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////

ZOLB_PopupMenu::ZOLB_PopupMenu( )
{
   m_clrMenuText = GetSysColor( COLOR_MENUTEXT );
   m_clrMenuTextSel = GetSysColor( COLOR_HIGHLIGHTTEXT );

   m_clr3dFace = GetSysColor( COLOR_3DFACE );
   m_clrMenu = GetSysColor( COLOR_MENU );
   m_clrHighlight = GetSysColor( COLOR_HIGHLIGHT );
   m_clr3dHilight = GetSysColor( COLOR_3DHILIGHT );
   m_clr3dShadow = GetSysColor( COLOR_3DSHADOW );
   m_clrGrayText = GetSysColor( COLOR_GRAYTEXT );

   m_clrBtnFace = GetSysColor( COLOR_BTNFACE );
   m_clrBtnHilight = GetSysColor( COLOR_BTNHILIGHT );
   m_clrBtnShadow = GetSysColor( COLOR_BTNSHADOW );

   m_nSpawnItem = 0;
   m_pSpawnItem = 0;

   m_nImageItem = 0;
   m_pImageItem = 0;

   m_sizeImage = CSize( 20, 20 );

   m_hMenuFont = 0;
/* COLORMAP cMap[ 3 ] = {
      { RGB( 128,128,128 ), m_clr3dShadow },
      { RGB( 192,192,192 ), m_clr3dFace },
      { RGB( 255,255,255 ), m_clr3dHilight }
   };
   CBitmap bmp;
   bmp.LoadMappedBitmap( IDB_MENUCHK, 0, cMap, 3 );
   m_ilOther.Create( 19, 19, ILC_COLOR4 | ILC_MASK, 1, 0 );
   m_ilOther.Add( &bmp, m_clr3dFace );
   bmp.DeleteObject( );
*/
   NONCLIENTMETRICS ncm;
   zmemset( &ncm, 0, sizeof( ncm ) );
   ncm.cbSize = sizeof( ncm );

   ::SystemParametersInfo( SPI_GETNONCLIENTMETRICS, 0, (PVOID) &ncm, 0 );

   m_hGuiFont = ::CreateFontIndirect( &ncm.lfMenuFont );

   // David 08/04/98 - start - bold font handling.
   m_hMenuBoldFont = 0;
   CreateBoldFont( );
   // David 08/04/98 - end - bold font handling.
}

ZOLB_PopupMenu::~ZOLB_PopupMenu( )
{
   if ( m_nSpawnItem > 0 )
   {
      for ( int t = 0; t < m_nSpawnItem; t++ )
      {
         if ( m_pSpawnItem[ t ] )
            delete( m_pSpawnItem[ t ] );
      }

      GlobalFree( (HGLOBAL) m_pSpawnItem );
   }

   if ( m_nImageItem > 0 )
   {
      GlobalFree( (HGLOBAL) m_pImageItem );
   }

   if ( m_hMenuFont )
      ::DeleteObject( (HGDIOBJ) m_hMenuFont );

   if ( m_hMenuBoldFont )
      ::DeleteObject( (HGDIOBJ) m_hMenuBoldFont );
}

void
ZOLB_PopupMenu::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
// CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
// CRect rcItem( lpDrawItemStruct->rcItem );
// pDC->FillSolidRect( rcItem, RGB( 255, 0, 0 ) );
   if ( lpDrawItemStruct->CtlType == ODT_MENU )
   {
      UINT id = lpDrawItemStruct->itemID;
      UINT state = lpDrawItemStruct->itemState;
      bool bEnabled = !(state & ODS_DISABLED);
      bool bSelected = (state & ODS_SELECTED) ? TRUE : FALSE;
      bool bChecked = (state & ODS_CHECKED) ? TRUE : FALSE;

      // David 08/04/98 - start - bold font handling
      bool bBold = ( state & ODS_DEFAULT ) ? TRUE : FALSE;
      // David 08/04/98 - end - bold font handling

      SpawnItem *pItem = (SpawnItem *) lpDrawItemStruct->itemData;
      if ( pItem )
      {
         CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
         CFont *pft;

         // David 08/04/98 - start - bold font handling.
         if ( !bBold )
            pft = CFont::FromHandle( (HFONT) m_hMenuFont ? m_hMenuFont : m_hGuiFont );
         else
            pft = CFont::FromHandle( (HFONT) m_hMenuBoldFont ? m_hMenuBoldFont : m_hGuiFont );

         // David 08/04/98 - end - bold font handling
         CFont *of = pDC->SelectObject( pft );

         CRect rect( lpDrawItemStruct->rcItem );
         CRect rectImage( rect ), rectText( rect );
         rectImage.right = rectImage.left + rect.Height( );
         rectImage.bottom = rect.bottom;

         if ( pItem->nCmd == -3 ) // is a separator
         {
            CPen pnDk( PS_SOLID, 1, m_clr3dShadow );
            CPen pnLt( PS_SOLID, 1, m_clr3dHilight );
            CPen *opn = pDC->SelectObject( &pnDk );
            pDC->MoveTo( rect.left + 2, rect.top + 2 );
            pDC->LineTo( rect.right - 2, rect.top + 2 );
            pDC->SelectObject( &pnLt );
            pDC->MoveTo( rect.left + 2, rect.top + 3 );
            pDC->LineTo( rect.right - 2, rect.top + 3 );
            pDC->SelectObject( opn );
         }
         else
         if ( pItem->nCmd == -4 ) // is a title csText
         {
            CString cs( pItem->szText );
            CString cs1;
            CRect rectBorder( rectText );

            if ( bSelected && bEnabled )
            {
               rectText.top++;
               rectText.left += 2;
            }

            pDC->FillSolidRect( rectText, m_clrMenu );
            pDC->DrawText( cs, rectText,
                           DT_VCENTER | DT_CENTER | DT_SINGLELINE );
            if ( bSelected && bEnabled )
               pDC->Draw3dRect( rectBorder, m_clr3dShadow, m_clr3dHilight );
         }
         else
         {
            rectText.left += rectImage.right + 1;

            int obk = pDC->SetBkMode( TRANSPARENT );

            COLORREF ocr;
            if ( bSelected )
            {
               if ( pItem->nImageIdx >= 0 || (state & ODS_CHECKED) )
                  pDC->FillSolidRect( rectText, m_clrHighlight );
               else
                  pDC->FillSolidRect( rect, m_clrHighlight );

               ocr = pDC->SetTextColor( m_clrMenuTextSel );
            }
            else
            {
               if ( pItem->nImageIdx >= 0 || (state & ODS_CHECKED) )
                  pDC->FillSolidRect( rectText, m_clrMenu );
               else
                  pDC->FillSolidRect( rect/*rectText*/, m_clrMenu );

               ocr = pDC->SetTextColor( m_clrMenuText );
            }

            if ( pItem->nImageIdx >= 0 )
            {
               int ay = (rectImage.Height( ) - m_sizeImage.cy) / 2;
               int ax = (rectImage.Width( ) - m_sizeImage.cx) / 2;

               if ( bSelected && bEnabled )
                  pDC->Draw3dRect( rectImage, m_clr3dHilight, m_clr3dShadow );
               else
               {
                  pDC->Draw3dRect( rectImage, m_clrMenu, m_clrMenu );
               }


               if ( bEnabled )
               {
                  m_ilList.Draw( pDC, pItem->nImageIdx,
                               CPoint( rectImage.left + ax, rectImage.top + ay ),
                               ILD_NORMAL );
               }
               else
               {
                  HICON hIcon = m_ilList.ExtractIcon( pItem->nImageIdx );
                  pDC->DrawState( CPoint( rectImage.left + ax, rectImage.top + ay ),
                                  m_sizeImage, (HICON) hIcon, DST_ICON | DSS_DISABLED,
                                  (CBrush *) 0 );
               }
            }
            else
            {
               if ( bChecked )
               {
                  int ay = (rectImage.Height( ) - m_sizeImage.cy) / 2;
                  int ax = (rectImage.Width( ) - m_sizeImage.cx) / 2;

                  m_ilOther.Draw( pDC, 0,
                                CPoint( rectImage.left + ax, rectImage.top + ay - 2 ),
                                ILD_NORMAL );
               }
            }

            CString cs( pItem->szText ), cs1;
            CSize size;
            size = pDC->GetTextExtent( cs );
            int ay1 = (rectText.Height( ) - size.cy) / 2;
            rectText.top += ay1;
            rectText.left += 2;
            rectText.right -= 15;

            int tf = cs.Find( '\t' );
            if ( tf >= 0 )
            {
               cs1 = cs.Right( cs.GetLength( ) - tf - 1 );
               cs = cs.Left( tf );
               if ( !bEnabled )
               {
                  if ( !bSelected )
                  {
                     CRect rcText1( rectText );
                     rcText1.InflateRect( -1, -1 );
                     pDC->SetTextColor( m_clr3dHilight );
                     pDC->DrawText( cs, rcText1, DT_VCENTER | DT_LEFT );
                     pDC->DrawText( cs1, rcText1, DT_VCENTER | DT_RIGHT );
                     pDC->SetTextColor( m_clrGrayText );
                     pDC->DrawText( cs, rectText, DT_VCENTER | DT_LEFT );
                     pDC->DrawText( cs1, rectText, DT_VCENTER | DT_RIGHT );
                  }
                  else
                  {
                     pDC->SetTextColor( m_clrMenu );
                     pDC->DrawText( cs, rectText, DT_VCENTER | DT_LEFT );
                     pDC->DrawText( cs1, rectText, DT_VCENTER | DT_RIGHT );
                  }
               }
               else
               {
                  pDC->DrawText( cs, rectText, DT_VCENTER | DT_LEFT );
                  pDC->DrawText( cs1, rectText, DT_VCENTER | DT_RIGHT );
               }
            }
            else
            {
               if ( !bEnabled )
               {
                  if ( !bSelected )
                  {
                     CRect rcText1( rectText );
                     rcText1.InflateRect( -1, -1 );
                     pDC->SetTextColor( m_clr3dHilight );
                     pDC->DrawText( cs, rcText1,
                                    DT_VCENTER | DT_LEFT | DT_EXPANDTABS );
                     pDC->SetTextColor( m_clrGrayText );
                     pDC->DrawText( cs, rectText,
                                    DT_VCENTER | DT_LEFT | DT_EXPANDTABS );
                  }
                  else
                  {
                     pDC->SetTextColor( m_clrMenu );
                     pDC->DrawText( cs, rectText,
                                    DT_VCENTER | DT_LEFT | DT_EXPANDTABS );
                  }
               }
               else
                  pDC->DrawText( cs, rectText,
                                 DT_VCENTER | DT_LEFT | DT_EXPANDTABS );
            }
            pDC->SetTextColor( ocr );
            pDC->SetBkMode( obk );
         }

         pDC->SelectObject( of );
      }
   }
}

void
ZOLB_PopupMenu::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
// lpMeasureItemStruct->itemWidth = 200;
// lpMeasureItemStruct->itemHeight = 25;
   bool res = FALSE;
   if ( lpMeasureItemStruct->CtlType == ODT_MENU )
   {
      UINT id = lpMeasureItemStruct->itemID;
      SpawnItem *pItem = (SpawnItem *) lpMeasureItemStruct->itemData;
      if ( pItem )
      {
         if ( pItem->nCmd == -3 ) // is a separator
         {
            lpMeasureItemStruct->itemWidth  = 10;
            lpMeasureItemStruct->itemHeight = 6;
         }
         else
         {
            CString cs( pItem->szText );
            if ( !cs.IsEmpty( ) )
            {
               CClientDC dc( AfxGetMainWnd( ) );
               CFont *pft = CFont::FromHandle( m_hMenuFont ? m_hMenuFont : m_hGuiFont );
               CFont *of = dc.SelectObject( pft );
               CSize size = dc.GetOutputTabbedTextExtent( cs, 0, 0 );
               if ( pItem->nCmd == -4 )
               {
                  CRect rect( 0, 0, 0, 0 );
                  dc.DrawText( cs, rect,
                               DT_CALCRECT | DT_TOP | DT_VCENTER | DT_SINGLELINE );
                  lpMeasureItemStruct->itemHeight = rect.Height( );
                  lpMeasureItemStruct->itemWidth = rect.Width( );
               }
               else
               {
                  lpMeasureItemStruct->itemHeight = m_sizeImage.cy + 5;
                  if ( size.cy > (int) lpMeasureItemStruct->itemHeight )
                     lpMeasureItemStruct->itemHeight = (int) size.cy;

                  lpMeasureItemStruct->itemWidth = size.cx + 2 + 15;
                  lpMeasureItemStruct->itemWidth +=
                     lpMeasureItemStruct->itemHeight > (UINT) m_sizeImage.cx ?
                                (UINT) lpMeasureItemStruct->itemHeight : (UINT) m_sizeImage.cx;
               }

               dc.SelectObject( of );
            }
            else
            {
               lpMeasureItemStruct->itemHeight = m_sizeImage.cy + 5;
               lpMeasureItemStruct->itemWidth = 100;
            }
         }
      }
   }
}

bool
ZOLB_PopupMenu::CreateBoldFont( )
{
   if ( m_hMenuBoldFont )
      ::DeleteObject( (HGDIOBJ) m_hMenuBoldFont );

   LOGFONT lf;
   ::GetObject( m_hMenuFont ? m_hMenuFont : m_hGuiFont, sizeof( lf ), &lf );
   lf.lfWeight = FW_BOLD;

   m_hMenuBoldFont = CreateFontIndirect( &lf );
   return( !!m_hMenuBoldFont );
}

bool
ZOLB_PopupMenu::AddToolBarResource( unsigned int resId )
{
   // David 08/04/98 - start - put CMenuSpawn in DLL
   HINSTANCE hInst = AfxFindResourceHandle ( MAKEINTRESOURCE( resId ), RT_TOOLBAR );
   if ( hInst == 0 )
      return( FALSE );

   // David 08/04/98 - end - put CMenuSpawn in DLL

   HRSRC hRsrc = ::FindResource( /*AfxGetResourceHandle( )*/ hInst,
                                 MAKEINTRESOURCE( resId ), RT_TOOLBAR );
   if ( hRsrc == 0 )
      return( FALSE );

   HGLOBAL hGlb = ::LoadResource( /*AfxGetResourceHandle( )*/hInst, hRsrc );
   if ( hGlb == 0 )
      return( FALSE );


   ToolBarData *pTBData = (ToolBarData *) ::LockResource( hGlb );
   if ( pTBData == 0 )
      return( FALSE );

   ASSERT( pTBData->wVersion == 1 );

   CBitmap bmp;
   bmp.LoadBitmap( resId );
   int nBmpItems = m_ilList.Add( &bmp, RGB( 192, 192, 192 ) );
   bmp.DeleteObject( );

   WORD *pItem = (WORD *) (pTBData + 1);

   for ( int k = 0; k < pTBData->wItemCount; k++, pItem++ )
   {
      if ( *pItem != ID_SEPARATOR )
         AddImageItem( nBmpItems++, (WORD) *pItem );
   }
// ** it seem that Windows doesn't free these resource (from Heitor Tome)
    ::UnlockResource( hGlb );
    ::FreeResource( hGlb );
// **
   return( TRUE );
}

bool
ZOLB_PopupMenu::LoadToolBarResource( unsigned int resId )
{
   // David 08/04/98 - start - put CMenuSpawn in DLL
   HINSTANCE hInst = AfxFindResourceHandle ( MAKEINTRESOURCE( resId ), RT_TOOLBAR );
   if ( !hInst )
      return( FALSE );

   // David 08/04/98 - end - put CMenuSpawn in DLL

   HRSRC hRsrc = ::FindResource( /*AfxGetResourceHandle( )*/ hInst,
                                 MAKEINTRESOURCE( resId ), RT_TOOLBAR );
   if ( hRsrc == 0 )
      return( FALSE );

   HGLOBAL hGlb = ::LoadResource( /*AfxGetResourceHandle( )*/ hInst, hRsrc );
   if ( hGlb == 0 )
      return( FALSE );

   ToolBarData* pTBData = (ToolBarData *) ::LockResource( hGlb );
   if ( pTBData == 0 )
      return( FALSE );

   ASSERT( pTBData->wVersion == 1 );

   m_sizeImage.cx = (int) pTBData->wWidth;
   m_sizeImage.cy = (int) pTBData->wHeight;

   if ( m_ilList.Create( m_sizeImage.cx, m_sizeImage.cy, ILC_COLOR4 | ILC_MASK,
                         pTBData->wItemCount, 0 ) == FALSE )
   {
      return( FALSE );
   }

   m_ilList.SetBkColor( m_clr3dFace );

   CBitmap bmp;
   bmp.LoadBitmap( resId );
   m_ilList.Add( &bmp, RGB( 192, 192, 192 ) );
   bmp.DeleteObject( );

   WORD *pItem = (WORD *)(pTBData + 1);
   int nBmpItems = 0;
   for ( int k = 0; k < pTBData->wItemCount; k++, pItem++ )
   {
      if ( *pItem != ID_SEPARATOR )
         AddImageItem( nBmpItems++, (WORD) *pItem );
   }

   // It seems that Windows doesn't free these resource (from Heitor Tome).
   ::UnlockResource( hGlb );
   ::FreeResource( hGlb );

    return( TRUE );
}

void
ZOLB_PopupMenu::AddImageItem( const int nIdx, WORD wCmd )
{
   if ( m_nImageItem == 0 )
      m_pImageItem = (ImageItem *) GlobalAlloc( GPTR, sizeof( ImageItem ) );
   else
      m_pImageItem = (ImageItem *) GlobalReAlloc( (HGLOBAL) m_pImageItem,
                                                  sizeof( ImageItem ) * (m_nImageItem + 1),
                                                  GMEM_MOVEABLE | GMEM_ZEROINIT );

   ASSERT( m_pImageItem );
   m_pImageItem[ m_nImageItem ].nCmd = (int) wCmd;
   m_pImageItem[ m_nImageItem ].nImageIdx = nIdx;
   m_nImageItem++;
}

void
ZOLB_PopupMenu::RemapMenu( CMenu *pMenu )
{
   static int nRecurse = 0;
   nRecurse++;

   ASSERT( pMenu );
   int nItem = pMenu->GetMenuItemCount( );
   while ( ( --nItem )>=0 )
   {
      UINT itemId = pMenu->GetMenuItemID( nItem );
      if ( itemId == (UINT) -1 )
      {
         CMenu *pops = pMenu->GetSubMenu( nItem );
         if ( pops ) RemapMenu( pops );
         if ( nRecurse > 0 )
         {
            CString cs;
            pMenu->GetMenuString( nItem, cs, MF_BYPOSITION );
            if ( cs != "" )
            {
               SpawnItem *sp = AddSpawnItem( cs, ( nRecurse == 1 ) ? -4 : -2 );
               pMenu->ModifyMenu( nItem, MF_BYPOSITION  |  MF_OWNERDRAW, (UINT) -1, (LPCTSTR) sp );
            }
         }
      }
      else
      {
         if ( itemId != 0 )
         {
            UINT oldState = pMenu->GetMenuState( nItem, MF_BYPOSITION );
            if ( !( oldState&MF_OWNERDRAW ) && !( oldState&MF_BITMAP ) )
            {
               ASSERT( oldState != (UINT)-1 );
               CString cs;
               pMenu->GetMenuString( nItem, cs, MF_BYPOSITION );
               SpawnItem *sp = AddSpawnItem( cs, itemId );
               pMenu->ModifyMenu( nItem, MF_BYPOSITION | MF_OWNERDRAW | oldState, (LPARAM)itemId, (LPCTSTR)sp );
            }
         }
         else
         {
            UINT oldState = pMenu->GetMenuState( nItem, MF_BYPOSITION );
            if ( !( oldState&MF_OWNERDRAW ) && !( oldState&MF_BITMAP ) )
            {
               ASSERT( oldState != (UINT)-1 );
               SpawnItem *sp = AddSpawnItem( "--", -3 );
               pMenu->ModifyMenu( nItem, MF_BYPOSITION | MF_OWNERDRAW | oldState, (LPARAM)itemId, (LPCTSTR)sp );
            }
         }
      }
   }
   nRecurse--;
}

ZOLB_PopupMenu::SpawnItem *
ZOLB_PopupMenu::AddSpawnItem( zCPCHAR cpcText, const int nCmd )
{
   if ( m_nSpawnItem == 0 )
      m_pSpawnItem = (SpawnItem **) GlobalAlloc( GPTR, sizeof( SpawnItem ) );
   else
      m_pSpawnItem = (SpawnItem **) GlobalReAlloc( (HGLOBAL) m_pSpawnItem,
                                                   sizeof( SpawnItem ) * (m_nSpawnItem + 1),
                                                   GMEM_MOVEABLE | GMEM_ZEROINIT );

   ASSERT( m_pSpawnItem );

   SpawnItem *p = new SpawnItem;
   ASSERT( p );
   m_pSpawnItem[ m_nSpawnItem ] = p;
   strcpy_s( p->szText, zsizeof( p->szText ), cpcText );
   p->nCmd = nCmd;

   if ( nCmd >= 0 )
      p->nImageIdx = FindImageItem( nCmd );
   else
      p->nImageIdx = nCmd;

   m_nSpawnItem++;
   return( p );
}

int
ZOLB_PopupMenu::FindImageItem( const int cmd )
{
   for ( int t = 0; t < m_nImageItem; t++ )
   {
      if ( m_pImageItem[ t ].nCmd == cmd )
         return( m_pImageItem[ t ].nImageIdx );
   }

   return( -1 );
}

void
ZOLB_PopupMenu::EnableMenuItems( CMenu *pMenu, CWnd *pParent )
{
   ASSERT( pMenu );
   ASSERT( pParent );

   int nItem = pMenu->GetMenuItemCount( );
   CCmdUI state;
   state.m_pMenu = pMenu;
   state.m_nIndex = nItem-1;
   state.m_nIndexMax = nItem;

   while ( --nItem >= 0 )
   {
      UINT itemId = pMenu->GetMenuItemID( nItem );
      if ( itemId == (UINT) -1 )
      {
         CMenu *pops = pMenu->GetSubMenu( nItem );
         if ( pops )
            EnableMenuItems( pops, pParent );
      }
      else
      {
         if ( itemId != 0 )
         {
            state.m_nID = itemId;
            pParent->OnCmdMsg( itemId, CN_UPDATE_COMMAND_UI, &state, 0 );
            state.DoUpdate( pParent, TRUE );
         }
      }

      state.m_nIndex = nItem-1;
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZOLB_Edit

ZOLB_Edit::ZOLB_Edit( )
{
   m_bEscapeKey = FALSE;
   m_nIdx = -1;
   m_msgSend = NM_OB_ONGROUPENDEDIT;
   m_bNoDown = FALSE;
}

ZOLB_Edit::~ZOLB_Edit( )
{
}

BEGIN_MESSAGE_MAP( ZOLB_Edit, CEdit )
   //{{AFX_MSG_MAP( ZOLB_Edit )
   ON_WM_KILLFOCUS( )
   ON_WM_CREATE( )
   ON_WM_CHAR( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZOLB_Edit message handlers

void
ZOLB_Edit::OnKillFocus( CWnd *pNewWnd )
{
   PostMessage( WM_CLOSE, 0, 0 );
   if ( !m_bEscapeKey )
   {
      GetWindowText( m_csText );
      if ( m_csText != "" )
         GetOwner( )->SendMessage( WM_OUTBAR_NOTIFY, m_msgSend, (LPARAM) this );
   }
}

BOOL
ZOLB_Edit::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->wParam == VK_RETURN )
   {
      PostMessage( WM_CLOSE, 0, 0 );
      return( TRUE );
   }
   else
   if ( pMsg->wParam == VK_ESCAPE )
   {
      PostMessage( WM_CLOSE, 0, 0 );
      return( m_bEscapeKey = TRUE );
   }

   return( CEdit::PreTranslateMessage( pMsg ) );
}

void
ZOLB_Edit::PostNcDestroy( )
{
   CEdit::PostNcDestroy( );
   delete this;
}

int
ZOLB_Edit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CEdit::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SendMessage( WM_SETFONT, (WPARAM) GetStockObject( DEFAULT_GUI_FONT ),
                MAKELPARAM( TRUE, 0 ) );
   return( 0 );
}

void
ZOLB_Edit::OnChar( UINT nChar, UINT nRepCnt, UINT uFlags )
{
   if ( m_msgSend == NM_OB_ONGROUPENDEDIT )
   {
      CEdit::OnChar( nChar, nRepCnt, uFlags );
      return;
   }

   if ( nChar == VK_ESCAPE || nChar == VK_RETURN )
   {
      if ( nChar == VK_ESCAPE )
         m_bEscapeKey = TRUE;

      GetParent( )->SetFocus( );
      return;
   }

   CEdit::OnChar( nChar, nRepCnt, uFlags );
   CString cs;
   CRect rect;
   CRect rectParent;
   GetClientRect( &rect );
   GetParent( )->GetClientRect( &rectParent );
   ClientToScreen( &rect );
   GetParent( )->ScreenToClient( &rect );
   GetWindowText( cs );
   CWindowDC dc( this );
   CFont *pFont = GetParent( )->GetFont( );
   CFont *pFontDC = dc.SelectObject( pFont );
   CRect rectSize( rect );
   rectSize.bottom = rectSize.top;

   if ( m_bNoDown == TRUE )
   {
      dc.DrawText( cs, rectSize, DT_CALCRECT );
      if ( rectSize.right >= rectParent.right - 1 )
         rect.right = rectParent.right - 1;
      else
         rect.right = rectSize.right;

      MoveWindow( &rect );
      return;
   }

   dc.DrawText( cs, rectSize, DT_WORDBREAK | DT_CENTER | DT_CALCRECT );
   dc.SelectObject( pFontDC );
   CSize size = rectSize.Size( );

   if ( size.cx > rect.Width( ) )
   {
      if ( size.cx + rect.left < rectParent.right )
         rect.right = rect.left + size.cx;
      else
         rect.right = rectParent.right;

      MoveWindow( &rect );
   }
   else
   if ( size.cy > rect.Height( ) )
   {
      if ( size.cy + rect.bottom < rectParent.bottom )
         rect.bottom = rect.top + size.cy;
      else
         rect.bottom = rectParent.bottom;

      MoveWindow( &rect );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0

   CGfxSplitterWnd   wndSplitter;
   ZOutlookBar    wndBar;

   CImageList        imaLarge;
   CImageList        imaSmall;

   CTreeCtrl         wndTree;


BOOL
CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
   if (!wndSplitter.CreateStatic(this, 1, 2))
      return false;

   if (!wndSplitter.CreateView(0, 1, pContext->m_pNewViewClass, CSize(0,0), pContext))
      return false;

   DWORD dwf = ZOutlookBar::fDragItems|ZOutlookBar::fEditGroups|ZOutlookBar::fEditItems|ZOutlookBar::fRemoveGroups|
            ZOutlookBar::fRemoveItems|ZOutlookBar::fAddGroups|ZOutlookBar::fAnimation;
//          |ZOutlookBar::fSelHighlight;


   wndBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), &wndSplitter, wndSplitter.IdFromRowCol(0, 0), dwf);
   wndBar.SetOwner(this);

   imaLarge.Create(IDB_IMAGELIST, 32, 0, RGB(128,128,128));
   imaSmall.Create(IDB_SMALL_IMAGELIST, 16, 0, RGB(0,128,128));

   wndBar.SetImageList(&imaLarge, ZOutlookBar::fLargeIcon);
   wndBar.SetImageList(&imaSmall, ZOutlookBar::fSmallIcon);

   wndTree.Create(WS_CHILD|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &wndBar, 1010);
   wndTree.SetImageList(&imaSmall, TVSIL_NORMAL);

   wndBar.SetAnimationTickCount(20);
   wndBar.SetAnimSelHighlight(200);

   HTREEITEM htm = wndTree.InsertItem("tree 1", 0,0);
   HTREEITEM htm1 = wndTree.InsertItem("tree 2", 1, 1, htm);
   wndTree.InsertItem("tree 3", 1, 1, htm);

   for (int xx = 0; xx < 30; xx ++)
   {
      htm = wndTree.InsertItem("tree 1", 0,0);
      htm1 = wndTree.InsertItem("tree 2", 1, 1, htm);
      wndTree.InsertItem("tree 3", 1, 1, htm);

      htm = wndTree.InsertItem("tree 1", 0,0);
      htm1 = wndTree.InsertItem("tree asdasdasdadasdasdasdasdas2", 1, 1, htm);
      wndTree.InsertItem("tree 3", 1, 1, htm);
   }

   wndBar.AddFolder("Folder 1", 0);
   wndBar.AddFolderBar("TreeBar", &wndTree);
   wndBar.AddFolder("Folder 2", 1);
   wndBar.AddFolder("Folder 3", 2);
   wndBar.AddFolder("Folder 4", 3);

   wndBar.InsertItem(0, 0, "Item 1", 0, 0);
   wndBar.InsertItem(0, 1, "Item 2", 1, 0);
   wndBar.InsertItem(0, 2, "Item 3", 2, 0);
   wndBar.InsertItem(0, 3, "Item 4", 3, 0);
   wndBar.InsertItem(0, 4, "Item 5", 0, 0);
   wndBar.InsertItem(0, 5, "Item 6", 1, 0);
   wndBar.InsertItem(0, 6, "Item 7", 2, 0);
   wndBar.InsertItem(0, 7, "Item 8", 3, 0);
   wndBar.InsertItem(0, 8, "Item 9", 0, 0);
   wndBar.InsertItem(0, 9, "Item 10", 1, 0);
   wndBar.InsertItem(0, 10, "Item 11", 2, 0);
   wndBar.InsertItem(0, 11, "Item 12", 3, 0);
   wndBar.InsertItem(0, 12, "Item 13", 0, 0);
   wndBar.InsertItem(0, 13, "Item 14", 1, 0);
   wndBar.InsertItem(0, 14, "Item 15", 2, 0);
   wndBar.InsertItem(0, 15, "Item 16", 3, 0);

   wndBar.InsertItem(0, 1, "New Item 1", 3, 0);
   wndBar.InsertItem(0, 17, "Item 17", 3, 0);

   wndBar.InsertItem(2, 0, "Item 1", 0, 0);
   wndBar.InsertItem(2, 1, "Let's see what this Item will become actually 2", 1, 0);
   wndBar.InsertItem(2, 2, "Item 3", 2, 0);
   wndBar.InsertItem(2, 3, "Item 4", 3, 0);

   wndBar.SetSelFolder(0);

   CRect r;
   GetClientRect(&r);

   int w1 = r.Width()/5;
   int w2 = r.Width()/4;

   wndSplitter.SetColumnInfo( 0, w1, 0 );
   wndSplitter.SetColumnInfo( 1, w2, 0 );

   wndSplitter.RecalcLayout();

   return true;
}

long
CMainFrame::OnOutbarNotify(WPARAM wParam, LPARAM lParam)
{
   switch (wParam)
   {
      case NM_OB_ITEMCLICK:
      // cast the lParam to an integer to get the clicked item
         {
            int index = (int) lParam;
            CString cs, cs1;
            cs1 = wndBar.GetItemText(index);
            cs.Format("Clicked on %d - <%s>", (int)lParam, cs1);
            AfxMessageBox(cs);
         }
      return( 0 );

      case NM_OB_ONLABELENDEDIT:
      // cast the lParam to an OUTBAR_INFO * struct; it will contain info about the edited item
      // return 1 to do the change and 0 to cancel it
         {
            OUTBAR_INFO * pOI = (OUTBAR_INFO *) lParam;
            TRACE2("Editing item %d, new text:%s\n", pOI->index, pOI->cText);
         }
      return( 1 );

      case NM_OB_ONGROUPENDEDIT:
      // cast the lParam to an OUTBAR_INFO * struct; it will contain info about the edited folder
      // return 1 to do the change and 0 to cancel it
         {
            OUTBAR_INFO * pOI = (OUTBAR_INFO *) lParam;
            TRACE2("Editing folder %d, new text:%s\n", pOI->index, pOI->cText);
         }
      return( 1 );

      case NM_OB_DRAGITEM:
      // cast the lParam to an OUTBAR_INFO * struct; it will contain info about the dragged items
      // return 1 to do the change and 0 to cancel it
         {
            OUTBAR_INFO * pOI = (OUTBAR_INFO *) lParam;
            TRACE2("Drag item %d at position %d\n", pOI->iDragFrom, pOI->iDragTo);
         }
      return( 1 );
   }
   return( 0 );
}

void CMainFrame::OnGfxLargeicon()
{
   wndBar.SetSmallIconView(false);
}

void CMainFrame::OnUpdateGfxLargeicon(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(!wndBar.IsSmallIconView());
}

void CMainFrame::OnGfxSmallicon()
{
   wndBar.SetSmallIconView(true);
}

void CMainFrame::OnUpdateGfxSmallicon(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(wndBar.IsSmallIconView());
}
#endif

void
ZOutlookBar::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZOutlookBar::CreateZ ", *m_pzsTag );
#endif

   DWORD dwf = ZOutlookBar::fDragItems | ZOutlookBar::fEditGroups |
               ZOutlookBar::fEditItems | ZOutlookBar::fRemoveGroups |
               ZOutlookBar::fRemoveItems | ZOutlookBar::fAddGroups | ZOutlookBar::fAnimation;

   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( WS_CHILD | WS_VISIBLE, CRect( 0, 0, 0, 0 ),
                 m_pWndParent, m_nIdNbr, dwf )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;

      SetOwner( m_pWndParent );
      if ( m_pzmaComposite && m_pzmaComposite->m_pCtrl &&
           mIs_hWnd( m_pzmaComposite->m_pCtrl->m_hWnd ) )
      {
         m_pzmaComposite->m_hWndFullClient = m_hWnd;
      }
   }
}

BOOL
ZOutlookBar::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZOutlookBar::DestroyWindow ", *m_pzsTag );
#endif
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION                 // WINAPI
OutlookBar( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZOutlookBar( pZSubtask, pWndParent,
                            pzmaComposite, vDialog,
                            nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=OLB_AddImages
// Source Module=zdctlolb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OLB_AddImages
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation adds images to an outlook bar.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the outlook bar control.
//              cpcCtrlTag  - The unique name of the control.
//              nLargeImageListId  - Resource number of large image list.
//              nSmallImageListId  - Resource number of small image list.
//
//  RETURNS:    0 - Images added
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
OLB_AddImages( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zSHORT  nLargeImageListId,
               zSHORT  nSmallImageListId )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZOutlookBar *pOutlookBar = DYNAMIC_DOWNCAST( ZOutlookBar,
                                                   pzma->m_pCtrl );
      if ( pOutlookBar && mIs_hWnd( pOutlookBar->m_hWnd ) )
      {
         HINSTANCE hInst = AfxGetResourceHandle( );
         AfxSetResourceHandle( pZSubtask->m_hInstDLL );
         pOutlookBar->m_imgLarge.Create( nLargeImageListId, 32, 0,
                                         RGB( 128, 128, 128 ) );
         pOutlookBar->m_imgSmall.Create( nSmallImageListId, 16, 0,
                                         RGB( 0, 128, 128 ) );

         pOutlookBar->SetImageList( &(pOutlookBar->m_imgLarge),
                                    ZOutlookBar::fLargeIcon );
         pOutlookBar->SetImageList( &(pOutlookBar->m_imgSmall),
                                    ZOutlookBar::fSmallIcon );
         AfxSetResourceHandle( hInst );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for OLB_AddImages ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=OLB_AddFolder
// Source Module=zdctlolb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OLB_AddFolder
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation adds folders to an outlook bar.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the outlook bar control.
//              cpcCtrlTag  - The unique name of the control.
//              cpcFolderText - Label of folder.
//              nFolderNbr  - Number of folder to add (> 0).
//
//  RETURNS:    0 - Folder added
//             -1 - Error locating control or invalid folder number
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
OLB_AddFolder( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zCPCHAR cpcFolderText,
               zSHORT  nFolderNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( nFolderNbr > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZOutlookBar *pOutlookBar = DYNAMIC_DOWNCAST( ZOutlookBar,
                                                   pzma->m_pCtrl );
      if ( pOutlookBar && mIs_hWnd( pOutlookBar->m_hWnd ) )
      {
         pOutlookBar->AddFolder( cpcFolderText, nFolderNbr - 1 );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for OLB_AddFolder ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=OLB_AddFolderItem
// Source Module=zdctlolb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OLB_AddFolderItem
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation adds folders to an outlook bar.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the outlook bar control.
//              cpcCtrlTag  - The unique name of the control.
//              nFolderNbr  - Number of folder (> 0).
//              nItemNbr    - Number of item (> 0).
//              cpcItemText - Label of item.
//              nImageNbr   - Number of image from image list (> 0 ).
//              ulExtraData - Extra data kept with item.
//
//  RETURNS:    0 - Folder added
//             -1 - Error locating control or invalid folder number
//             -2 - Invalid folder number
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
OLB_AddFolderItem( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zSHORT  nFolderNbr,
                   zSHORT  nItemNbr,
                   zCPCHAR cpcItemText,
                   zSHORT  nImageNbr,
                   zULONG  ulExtraData )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( nFolderNbr > 0 && nItemNbr > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZOutlookBar *pOutlookBar = DYNAMIC_DOWNCAST( ZOutlookBar,
                                                   pzma->m_pCtrl );
      if ( pOutlookBar && mIs_hWnd( pOutlookBar->m_hWnd ) &&
           nFolderNbr <= pOutlookBar->GetFolderCount( ) )
      {
         pOutlookBar->InsertItem( nFolderNbr - 1, nItemNbr - 1,
                                  cpcItemText, nImageNbr - 1,
                                  ulExtraData );
         return( 0 );
      }
      else
         return( -2 );

      TraceLineS( "drvr - Invalid control type for OLB_AddFolderItem ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=OLB_AddFolderBar
// Source Module=zdctlolb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OLB_AddFolderBar
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation adds folders to an outlook bar.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the outlook bar control.
//              cpcCtrlTag  - The unique name of the control.
//              cpcFolderText  - Label of folder.
//              cpcSubCtrlTag  - The unique name of the sub-control.
//              ulExtraData - Extra data kept with folder.
//
//  RETURNS:    0 - Folder added
//             -1 - Error locating control or invalid folder number
//             -2 - Error locating sub-control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
OLB_AddFolderBar( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zCPCHAR cpcFolderText,
                  zCPCHAR cpcSubCtrlTag,
                  zULONG  ulExtraData )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZOutlookBar *pOutlookBar = DYNAMIC_DOWNCAST( ZOutlookBar,
                                                   pzma->m_pCtrl );
      if ( pOutlookBar && mIs_hWnd( pOutlookBar->m_hWnd ) )
      {
         if ( GetWindowAndCtrl( &pZSubtask, &pzma,
                                vSubtask, cpcSubCtrlTag ) == 0 &&
              pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
         {
            pOutlookBar->AddFolderBar( cpcFolderText, pzma->m_pCtrl,
                                       ulExtraData );
         }
         else
            return( -2 );

         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for OLB_AddFolderBar ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=OLB_SetSelFolder
// Source Module=zdctlolb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OLB_SetSelFolder
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation adds folders to an outlook bar.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the outlook bar control.
//              cpcCtrlTag  - The unique name of the control.
//              nFolderNbr  - Number of folder (> 0).
//
//  RETURNS:    0 - Folder added
//             -1 - Error locating control or invalid folder number
//             -2 - Invalid folder number
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
OLB_SetSelFolder( zVIEW   vSubtask,
                  zCPCHAR cpcCtrlTag,
                  zSHORT  nFolderNbr )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( nFolderNbr > 0 &&
        GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZOutlookBar *pOutlookBar = DYNAMIC_DOWNCAST( ZOutlookBar,
                                                   pzma->m_pCtrl );
      if ( pOutlookBar && mIs_hWnd( pOutlookBar->m_hWnd ) &&
           nFolderNbr <= pOutlookBar->GetFolderCount( ) )
      {
         pOutlookBar->SetSelFolder( nFolderNbr - 1 );
         return( 0 );
      }
      else
         return( -2 );

      TraceLineS( "drvr - Invalid control type for OLB_SetSelFolder ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=OLB_SetEventCallback
// Source Module=zdctlolb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: OLB_SetEventCallback
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation sets the callback function for OLB events.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the outlook bar control.
//              cpcCtrlTag  - The unique name of the control.
//              lpfnOLB_Event - Pointer to the event callback function.
//
//  RETURNS:    0 - Callback function set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
OLB_SetEventCallback( zVIEW   vSubtask,
                      zCPCHAR cpcCtrlTag,
                      LPARAM  lpfnOLB_Event )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZOutlookBar *pOutlookBar = DYNAMIC_DOWNCAST( ZOutlookBar,
                                                   pzma->m_pCtrl );
      if ( pOutlookBar && mIs_hWnd( pOutlookBar->m_hWnd ) )
      {
         pOutlookBar->m_lpfnOLB_Event = (zFARPROC_OLB_EVENT) lpfnOLB_Event;
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for OLB_SetEventCallback ",
                  cpcCtrlTag );
   }

   return( -1 );
}
