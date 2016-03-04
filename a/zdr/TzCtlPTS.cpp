/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlpts.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Painter Scrollbars.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.10.29    DKS    Z10
//    With right mouse rubber band select, no longer select children of
//    selected parents.
//
// 2001.06.28    DKS    Z10
//    Fixed marking of overlapped ctrls.
//
// 2001.01.31    DKS    Z10    QS999
//    Fix tabstop ordering.
//
// 2000.03.27    DKS    Z10    QS999
//    Normalize ctrl selection rectangle.
//
// 1999.09.22    DKS    Z10    QS999
//    Permitted groups to be resized to small height.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Added view to document.
//
// 1999.03.10    DKS
//    Altered the Profile functions to remove name conflict.
//
// 1999.01.27    DKS
//    Implemented alterations to function signatures for Undo/Redo.
//
// 1998.12.14    DKS
//    Better debugging info for AdjustGroupSets.
//
// 1998.08.13    DKS
//    Fix to Tab control acceptance by GroupBox (now warns)
//
// 1998.10.30    DKS
//    Improved selection algorithim.
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
// #define zDEBUG_CTRL

IMPLEMENT_DYNAMIC( TZPainterClient, CFormView )

BEGIN_MESSAGE_MAP( TZPainterClient, CFormView )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_MOUSEMOVE( )
   ON_WM_KEYDOWN( )
   ON_WM_HSCROLL( )
   ON_WM_VSCROLL( )
   ON_WM_SETFOCUS( )
   ON_WM_SIZE( )
   ON_WM_ERASEBKGND( )
   ON_WM_PAINT( )
   ON_WM_NCPAINT( )
   ON_WM_CTLCOLOR( )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

LPCSTR TZPainterClient::m_cpcZPtrClassName = 0;


static LPCSTR
RegisterSimilarClass( LPCSTR lpszNewClassName,
                      LPCSTR lpszOldClassName,
                      UINT   nIDResource )
{
#ifdef DEBUG_ALL
   TraceLineS( "RegisterSimilarClass new Class: ", lpszNewClassName );
#endif

   // Get class info for old class.
   HINSTANCE hInst = AfxGetInstanceHandle( );
   WNDCLASS  wc;

   if ( ::GetClassInfo( hInst, lpszOldClassName, &wc ) == 0 )
   {
       TraceLineS( "Can't find window class: ", lpszOldClassName );
       return( 0 );
   }

   // Register new class with same info, but different name and icon.
   wc.lpszClassName = lpszNewClassName;
// wc.hIcon = ::LoadIcon( hInst, MAKEINTRESOURCE( nIDResource ) );
   wc.hbrBackground = (HBRUSH) GetStockObject( LTGRAY_BRUSH );
   if ( AfxRegisterClass( &wc ) == 0 )
   {
      TraceLineS( "Unable to register window class: ", lpszNewClassName );
      return( 0 );
   }

   return( lpszNewClassName );
}

TZPainterClient::TZPainterClient( TZPainterWindow *pPainterWindow,
                                  CCreateContext  *pContext ) :
                 CFormView( IDD_INPUTFORM )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::ctor", "" );
#endif

   Attr.Style |= WS_VSCROLL | WS_HSCROLL;
   m_pPainterWindow = pPainterWindow;
   m_pRuler = 0;
   m_pPageHeader = 0;
   m_pPageFooter = 0;
   m_nRecurse = 0;
   m_nMultiMoveSize = 0;
   m_bRButtonUp = FALSE;
   m_bCapture = FALSE;
   m_bPaintWindow = FALSE;
   m_pHeadSelect = 0;

   m_pPainterWindow->m_lScrollUnitsX = 32;
   m_pPainterWindow->m_lScrollUnitsY = 16;

   // initialize members for ambient props
   m_pFontHolder = 0;
   m_descFont.lpstrName = 0;

   InitAmbientProperties();
   m_bTraceAmbientPropertyRequest = FALSE;

   CString csHelp;
   zPCHAR pszStorageFile = csHelp.GetBufferSetLength( zMAX_FILENAME_LTH );
   SysReadZeidonIni( -1, "[ActiveX]", "TraceAmbientPropertyRequest", pszStorageFile, zMAX_FILENAME_LTH );
   csHelp.ReleaseBuffer( );
   if ( csHelp.CompareNoCase( "y" ) == 0 )
   {
      m_bTraceAmbientPropertyRequest = TRUE;
   }

   m_pBitmap = new CBitmap( );
   m_pBrush = new CBrush( );

   VERIFY( m_pBitmap->LoadBitmap( IDB_GRID ) );
   VERIFY( m_pBrush->CreatePatternBrush( m_pBitmap ) );

   // views are always created with a border!
   if ( Create( 0, "SC", AFX_WS_DEFAULT_VIEW,
                CRect( 0, 0, 0, 0 ), m_pPainterWindow,
                AFX_IDW_PANE_FIRST, pContext ) == 0 )
   {
      TraceLineS( "TZPainterClient::CreateView failed", " creation" );
      return;   // can't continue without a view
   }

   m_pPainterWindow->m_pPainterClient = this;

   // View is created ... now add it to the document.
   m_pPainterWindow->m_pBar->m_pZSubtask->
                              m_pZView->GetDocument( )->AddView( this );

// typedef struct tagSCROLLINFO
// {  // si
//
//    UINT cbSize;
//    UINT fMask;
//    int  nMin;
//    int  nMax;
//    UINT nPage;
//    int  nPos;
//    int  nTrackPos;
//
// }   SCROLLINFO;
// typedef SCROLLINFO FAR *LPSCROLLINFO;
//
// Members:
//    cbSize - Specifies the size, in bytes, of this structure.
//    fMask - Specifies the scroll bar parameters to set or retrieve.
//            This member can be a combination of the following values:
//             SIF_ALL - Combination of SIF_PAGE, SIF_POS, and SIF_RANGE.
//             SIF_DISABLENOSCROLL - This value is used only when setting
//                a scroll bar's parameters.  If the scroll bar's new
//                parameters make the scroll bar unnecessary, disable the
//                scroll bar instead of removing it.
//             SIF_PAGE - The nPage member contains the page size for a
//                proportional scroll bar.
//             SIF_POS - The nPos parameter contains the scroll box
//                position.
//             SIF_RANGE - The nMin and nMax members contain the minimum
//                and maximum values for the scrolling range.
//    nMin - Specifies the minimum scrolling position.
//    nMax - Specifies the maximum scrolling position.
//    nPage - Specifies the page size.  A scroll bar uses this value to
//           determine the appropriate size of the proportional scroll box.
//    nPos - Specifies the position of the scroll box.
//    nTrackPos - Specifies the immediate position of a scroll box that the
//           user is dragging.  An application can retrieve this value while
//           processing the SB_THUMBTRACK notification message.  An
//           application cannot set the immediate scroll position ... the
//           SetScrollInfo function ignores this member.
//
// BOOL SetScrollInfo( int nBar, LPSCROLLINFO lpScrollInfo,
//                     BOOL bRedraw = TRUE );
// Return Value:
//    If successful, the return is TRUE. Otherwise, it is FALSE.
// Parameters:
//    nBar - Specifies whether the scroll bar is a control or part of a
//           window's nonclient area.  If it is part of the nonclient area,
//           nBar also indicates whether the scroll bar is positioned
//           horizontally, vertically, or both.  It must be one of the
//           following:
//       SB_BOTH - Specifies the horizontal and vertical scroll bars of
//           the window.
//       SB_HORZ - Specifies that the window is a horizontal scroll bar.
//       SB_VERT - Specifies that the window is a vertical scroll bar.
//    lpScrollInfo - A pointer to a SCROLLINFO structure.
//    bRedraw - Specifies whether the scroll bar should be redrawn to
//              reflect the new position.  If bRedraw is TRUE, the scroll
//              bar is redrawn.  If it is FALSE, it is not redrawn.  The
//              scroll bar is redrawn by default.
// Remarks:
//    Call this member function to set the information that the SCROLLINFO
//    structure maintains about a scroll bar.  The SCROLLINFO structure
//    contains information about a scroll bar, including the minimum and
//    maximum scrolling positions, the page size, and the position of the
//    scroll box (the thumb).  See the SCROLLINFO structure topic in the
//    Win32 SDK Programmer's Reference for more information about changing
//    the structure defaults.  The MFC Windows message handlers that indicate
//    scroll-bar position, CWnd::OnHScroll and CWnd::OnVScroll, provide only
//    16 bits of position data. GetScrollInfo and SetScrollInfo provide 32
//    bits of scroll-bar position data.  Thus, an application can call
//    GetScrollInfo while processing either CWnd::OnHScroll or
//    CWnd::OnVScroll to obtain 32-bit scroll-bar position data.
//

#if 0
   SCROLLINFO si;

   si.cbSize = sizeof( SCROLLINFO );
   si.fMask = SIF_ALL;
   si.nMin = 0;
   si.nMax = 2000;
   si.nPage = 400;
   si.nPos = 0;
   // si.nTrackPos = 0;  ignored anyhow

   SetScrollInfo( SB_VERT, &si, FALSE );
   SetScrollInfo( SB_HORZ, &si, FALSE );
#endif
// ScrollToPosition( CPoint( 0, 0 ) );
}

BOOL
TZPainterClient::PreCreateWindow( CREATESTRUCT& cs )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::PreCreateWindow", "" );
#endif

   // TODO: Modify the Window class or styles here by modifying
   // the CREATESTRUCT cs

   if ( CView::PreCreateWindow( cs ) == FALSE )
      return( FALSE );

   if ( m_cpcZPtrClassName == 0 )
   {
      // One-time initialization: register the class
      m_cpcZPtrClassName = RegisterSimilarClass( "ZeidonPainterView",
                                                 cs.lpszClass,
                                                 IDR_MAINFRAME );
      if ( m_cpcZPtrClassName == 0 )
         return( FALSE );
   }

   cs.lpszClass = m_cpcZPtrClassName;
   return( TRUE );
}

int
TZPainterClient::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::OnCreate", "" );
#endif

   if ( CView::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   ModifyStyle( 0, WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL );
   return( 0 );
}

TZPainterClient::~TZPainterClient( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient dtor", "" );
#endif

   // delete ambient props
   RemoveAmbientProperties( );

   mDeleteInit ( m_pFontHolder );
   if ( m_descFont.lpstrName )
   {
      delete[] m_descFont.lpstrName;
   }

   mDeleteInit( m_pBrush );
   mDeleteInit( m_pBitmap );

// mDeleteInit( m_hDC );
   mDeleteInit( m_pRuler );
}

// This routine determines if there is a child of the control that contains
// the banding rectangle (may be a point).  If a child of the specified
// control contains the banding rect, return FALSE ... otherwise return TRUE.
zBOOL
TZPainterClient::OnlyCtrlContainsPt( TZPainterCtrl *pCtrl,
                                     CRect& rectBand )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::OnlyCtrlContainsPt", "" );
#endif

   CRect rectCtrl;
   TZPainterCtrl *pCurrCtrl;
   zSHORT nPos;

   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCurrCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      pCurrCtrl->ParentToPainterRect( rectCtrl );
      if ( pCurrCtrl != pCtrl )
      {
         if ( (rectBand.left >= rectCtrl.left &&
               rectBand.top  >= rectCtrl.top &&
               rectBand.right <= rectCtrl.right &&
               rectBand.bottom <= rectCtrl.bottom) &&
               pCurrCtrl->m_pCtrlParent == pCtrl )
         {
            return( FALSE );
         }
      }
   }

   return( TRUE );
}

void
TZPainterClient::ShowContextMenu( CPoint& pt )
{
   CRect rect( 0, 0, 0, 0 );

   TZPainterCtrl *pCtrl = FindCtrlAtPt( rect, pt );
   if ( pCtrl )
   {
      TraceLineS( "TZPainterClient::ShowContextMenu for Ctrl: ",
                  pCtrl->m_csTag );
      pCtrl->ShowContextMenu( pt );
   }
   else
   {
      // Show a context menu for the client.
      TraceLineS( "TZPainterClient::ShowContextMenu for Client", "" );
   }
}

zLONG
GetSquaredDistance( CPoint& pt1, CPoint& pt2 )
{
   zLONG  lDistX = pt1.x - pt2.x;
   zLONG  lDistY = pt1.y - pt2.y;
   return( lDistX * lDistX + lDistY * lDistY );
}

// This routine locates the "childmost" control at a point.
TZPainterCtrl *
TZPainterClient::FindCtrlAtPt( CRect& rect, CPoint &pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::FindCtrlAtPt", "" );
#endif

   TZPainterCtrl *pCtrlFound = 0;
   TZPainterCtrl *pCtrl;
   CRect  rectCtrl;
   zSHORT nPos;

   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->GetVisibility( ) == 0 ) // painter can select and display
      {
         pCtrl->ParentToPainterRect( rectCtrl );
         if ( rectCtrl.PtInRect( pt ) )
         {
            if ( pCtrlFound )
            {
               if ( pCtrl->m_pCtrlParent == pCtrlFound || // child of prev ctrl
                    (rectCtrl.left >= rect.left &&        // or smaller than
                     rectCtrl.top  >= rect.top &&         // prev ctrl
                     rectCtrl.right <= rect.right &&
                     rectCtrl.bottom <= rect.bottom) )
               {
                  pCtrlFound = pCtrl;
                  rect = rectCtrl;
               }
               else
               {
                  // Determine if the midpoint of this ctrl is closer to the
                  // point than the midpoint of the previous ctrl.
                  CPoint pt1 = rect.CenterPoint( );
                  CPoint pt2 = rectCtrl.CenterPoint( );
                  zULONG ulDist1 = GetSquaredDistance( pt1, pt );
                  zULONG ulDist2 = GetSquaredDistance( pt2, pt );
                  if ( ulDist2 < ulDist1 )
                  {
                     pCtrlFound = pCtrl;
                     rect = rectCtrl;
                  }
               }
            }
            else
            {
               pCtrlFound = pCtrl;
               rect = rectCtrl;
            }
         }
      }
   }

   return( pCtrlFound );
}

TZPainterCtrl *
TZPainterClient::FindCtrlByCWnd( CWnd *pWnd )
{
   TZPainterCtrl *pCtrl;
   zSHORT nPos;

   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_pWndCtrl && pCtrl->m_pWndCtrl->m_hWnd == pWnd->m_hWnd )
      {
         return( pCtrl );
      }
   }

   return( 0 );
}

// Show tabbing, display tabbing order in controls
zBOOL
TZPainterClient::ShowTabbing( zSHORT nShow )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::ShowTabbing", "" );
#endif

   TZPainterCtrl *pCtrl;
   zSHORT  nPos;
   zBOOL   bShow;

   // Invert the current ShowTab setting.
   bShow = (m_pPainterWindow->m_pBar->m_fState & zTZCB_SHOWTABS) ?
                                                                FALSE : TRUE;
   m_pPainterWindow->Select( 0 );  // clear all selections
   if ( nShow < 1 )
   {
      if ( bShow )
         m_pPainterWindow->m_pBar->m_fState |= zTZCB_SHOWTABS;
      else
         m_pPainterWindow->m_pBar->m_fState &= ~zTZCB_SHOWTABS;
   }
   else
   if ( nShow  )
      m_pPainterWindow->m_pBar->m_fState |= zTZCB_SHOWTABS;
   else
      m_pPainterWindow->m_pBar->m_fState &= ~zTZCB_SHOWTABS;

   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
      {
      // TraceLine( "TZPainterClient::ShowTabbing for Window: %s   Ctrl: %s  %d",
      //            m_pPainterWindow->m_csWndTag,
      //            pCtrl->m_csTag, pCtrl->m_nTabStopNbr );
         if ( bShow )
         {
            if ( pCtrl->m_pCtrlCover )
               pCtrl->m_pCtrlCover->Invalidate( TRUE );
            else
               pCtrl->m_pWndCtrl->Invalidate( TRUE );
         }
         else
         {
            if ( pCtrl->m_pCtrlCover )
            {
               pCtrl->m_pCtrlCover->
                   SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                 SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
            }

            pCtrl->m_pWndCtrl->Invalidate( TRUE );
            pCtrl->m_pWndCtrl->UpdateWindow( );

            if ( pCtrl->m_pCtrlCover )
            {
               pCtrl->m_pCtrlCover->
                   SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                 SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
            }
         }
      }
   }

   Invalidate( );
   UpdateWindow( );
// TraceLineS( "TZPainterClient::ShowTabbing end", "" );
   return( bShow );
}

// Remove tabbing, remove tabbing order in controls
void
TZPainterClient::RemoveTabbing( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::RemoveTabbing", "" );
#endif

   TZPainterCtrl *pCtrl;
   zSHORT  nPos;

   if ( m_pPainterWindow->m_pBar->m_fState & zTZCB_SHOWTABS )
      ShowTabbing( 0 );

   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      pCtrl->m_nTabStopNbr = 9999;
      if ( pCtrl->m_pCtrlCover )
         pCtrl->m_pCtrlCover->Invalidate( TRUE );
   }

   m_pPainterWindow->m_nMaxTabIdNbr = 0;
}

BOOL
TZPainterClient::OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                                    VARIANT *pvar )
{
   TZAmbientProperty *pProp = 0;
   TZPainterCtrl     *pPainterCtrl = 0;
   BOOL bReturn = TRUE;

   pProp = FindAmbientProperty( dispid );

   if ( pProp == 0 )
   {
      if ( m_bTraceAmbientPropertyRequest )
      {
         TraceLineI( "*** Control requests unknown Ambient Property: ",
                     dispid );
      }

      bReturn = CWnd::OnAmbientProperty( pSite, dispid, pvar );
   }
   else
   {
      if ( m_bTraceAmbientPropertyRequest )
      {
         TraceLineS( "*** Control requests Ambient Property: ",
                     pProp->GetName() );
      }

      COleVariant v = pProp->GetValue();
      if ( dispid == DISPID_AMBIENT_DISPLAYNAME )
      {
         // Get a CWND from COleControlSite *pSite
         CWnd *pCtrlToFind = pSite->m_pWndCtrl;
         if ( pCtrlToFind )
         {
            if ( pCtrlToFind->IsKindOf( RUNTIME_CLASS( TZActiveX ) ) )
            {
               TZActiveX *pCtrl = (TZActiveX*)pCtrlToFind;
               v = pCtrl->m_csTag;
            }
         }
      }

      HRESULT hr = VariantCopy( pvar, &v );
      if ( SUCCEEDED (hr) )
      {
         bReturn = TRUE;
      }
      else
      {
         bReturn = FALSE;
      }
   }

   return( bReturn );
}

void
TZPainterClient::InitAmbientProperties( )
{
   CString csName;
   LPCOLESTR pszFaceName;
   TZAmbientProperty *pProp;

   csName.LoadString( IDS_APROPNAME_USERMODE );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_USERMODE, csName,
                                  COleVariant( VAR_FALSE, VT_BOOL ),
                                  VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_UIDEAD );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_UIDEAD, csName,
                                  COleVariant( VARIANT_FALSE, VT_BOOL ),
                                  VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_SHOWHATCHING );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_SHOWHATCHING, csName,
                                  COleVariant( VARIANT_BOOL( -1 ), VT_BOOL ),
                                  VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_SHOWGRABHANDLES );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_SHOWGRABHANDLES, csName,
                                  COleVariant( VARIANT_BOOL( -1 ), VT_BOOL ),
                                  VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_TEXTALIGN );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_TEXTALIGN, csName,
                                  COleVariant( short( 0 ), VT_I2 ),
                                  VTI_I2, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_BACKCOLOR );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_BACKCOLOR, csName,
                   COleVariant( long( GetSysColor( COLOR_WINDOW ) ), VT_I4 ),
                   VTI_COLOR, TRUE );
   m_apList.Add( pProp );


   mDeleteInit ( m_pFontHolder );
   m_pFontHolder = new CFontHolder( 0);
   if ( m_pFontHolder == 0 )
   {
     AfxThrowMemoryException();
   }

   m_descFont.cbSizeofstruct = sizeof( m_descFont );
   pszFaceName = L"MS Sans Serif";
   if ( m_descFont.lpstrName )
      delete[] m_descFont.lpstrName;

   m_descFont.lpstrName = new OLECHAR[ ocslen( pszFaceName ) + 1 ];
   if ( m_descFont.lpstrName == 0 )
   {
     AfxThrowMemoryException();
   }

   ocscpy( m_descFont.lpstrName, pszFaceName );
   m_descFont.cySize.Lo = 80000L;
   m_descFont.cySize.Hi = 0;
   m_descFont.sWeight = FW_NORMAL;
   m_descFont.sCharset = 0;
   m_descFont.fItalic = FALSE;
   m_descFont.fUnderline = FALSE;
   m_descFont.fStrikethrough = FALSE;
   m_pFontHolder->InitializeFont( &m_descFont );

   COleVariant varFont;
   varFont.vt = VT_DISPATCH;
   varFont.pdispVal = m_pFontHolder->GetFontDispatch();

   csName.LoadString( IDS_APROPNAME_FONT );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_FONT, csName, varFont,
                                  VTI_FONT, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_FORECOLOR );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_FORECOLOR, csName,
               COleVariant( long( GetSysColor( COLOR_WINDOWTEXT ) ), VT_I4 ),
               VTI_COLOR, TRUE );
   m_apList.Add( pProp );

// csName.LoadString( IDS_APROPNAME_SCALEUNITS );
// pProp = new TZAmbientProperty( DISPID_AMBIENT_SCALEUNITS, csName,
//   COleVariant( _T( "" ), VT_BSTR ), VTI_BSTR, TRUE );
// m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_DISPLAYNAME );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_DISPLAYNAME, csName,
                                  COleVariant( _T(""), VT_BSTR ),
                                  VTI_BSTR, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_LOCALEID );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_LOCALEID, csName,
                          COleVariant( long( GetUserDefaultLCID() ), VT_I4 ),
                          VTI_I4, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_MESSAGEREFLECT );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_MESSAGEREFLECT, csName,
                                  COleVariant( VARIANT_TRUE, VT_BOOL ),
                                  VTI_BOOL, TRUE );
   m_apList.Add( pProp );

   csName.LoadString( IDS_APROPNAME_SUPPORTSMNEMONICS );
   pProp = new TZAmbientProperty( DISPID_AMBIENT_SUPPORTSMNEMONICS, csName,
                                  COleVariant( VARIANT_BOOL( VARIANT_FALSE ), VT_BOOL ),
                                  VTI_BOOL, TRUE );
   m_apList.Add( pProp );

}

TZAmbientProperty *
TZPainterClient::FindAmbientProperty( DISPID dispid )
{
   TZAmbientProperty *pProp = 0;

   zLONG lMax = m_apList.GetSize();

   for ( zLONG lCurrent = 0; lCurrent < lMax; lCurrent++ )
   {
      if ( m_apList[ lCurrent ]->GetID() == dispid )
      {
         return( m_apList[ lCurrent ] );
      }
   }

   return( 0 );
}

TZAmbientProperty *
TZPainterClient::FindAmbientProperty( LPCOLESTR pszName )
{
   USES_CONVERSION;
   int iProp;
   LPCTSTR pszNameT;
   BOOL bFound = FALSE;

   pszNameT = OLE2CT( pszName );
   for ( iProp = 0; iProp < m_apList.GetSize(); iProp++ )
   {
      if ( m_apList[ iProp ]->GetName() == pszNameT )
      {
         return( m_apList[ iProp ] );
      }
   }

   // We didn't find the property.
   return( 0 );
}

void
TZPainterClient::RemoveAmbientProperties()
{
   TZAmbientProperty *pProp = 0;

   for (long k = m_apList.GetUpperBound(); k >= 0; k-- )
   {
      pProp = m_apList[ k ];
      m_apList.RemoveAt( k );
      delete pProp;
   }
}

void
TZPainterClient::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterClient::OnSetFocus", "" );
#endif

   m_pPainterWindow->SetFocus( );
}

void
TZPainterClient::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::OnKeyDown", "" );
#endif

   Default( );
}

void
TZPainterClient::OnMouseMove( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
// TracePoint( "TZPainterClient::OnMouseMove", pt );
#endif

#if 0
   TraceLineS( "TZPainterClient::OnMouseMove ================>",
               " shouldn't be here when mouse is captured" );

   // WM_MOUSEMOVE erase the previous rubberband rectangle and create
   // a new one based on the mouse's new position
   if ( m_bCapture )
   {
      CPoint ptStart;
      CPoint ptCurrent;
      CPoint ptWork;
      int    nOldROP2;

//??? if ( m_hDC == 0 )
//    {
//       TraceLineS( "<tzctlpts> MouseMove unexpected null hDC", "" );
//       m_hDC = new CClientDC( this );
//    }

//    nOldROP2 = m_hDC->SetROP2( R2_NOT );
      if ( m_ptLastMouseMove != m_ptMouseSelect )
      {
         // Erase the last rectangle drawn to represent where a control
         // would have been positioned on the last move
         ptStart = m_ptMouseSelect;
         ptCurrent = m_ptLastMouseMove;

         m_hDC->MoveTo( ptStart.x, ptStart.y );

         // Calculate and erase the previous rectangle
         ptWork.x = ptCurrent.x;
         ptWork.y = ptStart.y;
         m_hDC->LineTo( ptWork.x, ptWork.y );
         m_hDC->LineTo( ptCurrent.x, ptCurrent.y );
         ptWork.x = ptStart.x;
         ptWork.y = ptCurrent.y;
         m_hDC->LineTo( ptWork.x, ptWork.y );
         m_hDC->LineTo( ptStart.x, ptStart.y );
      }

      if ( pt != m_ptMouseSelect )
      {
         // Draw a new rubberband rectangle
         ptStart = m_ptMouseSelect;
         ptCurrent = pt;

         m_hDC->MoveTo( ptStart.x, ptStart.y );

         // Draw the new rubberband rectangle
         ptWork.x = ptCurrent.x;
         ptWork.y = ptStart.y;
         m_hDC->LineTo( ptWork.x, ptWork.y );
         m_hDC->LineTo( ptCurrent.x, ptCurrent.y );
         ptWork.x = ptStart.x;
         ptWork.y = ptCurrent.y;
         m_hDC->LineTo( ptWork.x, ptWork.y );
         m_hDC->LineTo( ptStart.x, ptStart.y );
      }

      m_ptLastMouseMove = pt;
//    m_hDC->SetROP2( nOldROP2 );
   }
#endif
// CWnd::OnMouseMove( uModKeys, pt );
}

void
TZPainterClient::OnLButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZPainterClient::OnLButtonDown", pt );
#endif

   CWnd *pActiveWnd = GetActiveWindow( );

// CFormView::OnLButtonDown( uModKeys, pt );
   m_ptMouseSelect = pt;

// if ( m_pCtrl->m_bCtrlKey == FALSE )
//    m_pPainterWindow->Select( 0 );  // clear all selections

   TZPainterCtrl *pCtrl;
   CRect rect;
   CRect rectClient;
   CPoint ptDiff;
   HWND  hWnd;

   GetWindowRect( rect );
   GetClientRect( rectClient );
   ptDiff.x = rect.Width( ) - rectClient.Width( );;
   ptDiff.x = rect.Height( ) - rectClient.Height( );

   if ( m_pPainterWindow->m_pPaintParent &&
        m_pPainterWindow->m_pPaintParent->IsModeValid( zCTRL_MODE_CLIENT ) )
   {
      hWnd = m_pPainterWindow->m_pPaintParent->m_pWndCtrl->m_hWnd;
   }
   else
      hWnd = m_hWnd;

   CWnd *pTrackWnd = new CWnd( );
   if ( pActiveWnd->m_hWnd != m_pPainterWindow->m_hWnd )
      SetRedraw( FALSE );

   pTrackWnd->CreateEx( WS_EX_TRANSPARENT | WS_EX_TOPMOST, 0,
                        "ClientCover", WS_CHILD | WS_VISIBLE,
                        0, 0, rect.Width( ), rect.Height( ),
                        hWnd, (HMENU) -1 );
   if ( pActiveWnd->m_hWnd != m_pPainterWindow->m_hWnd )
      SetRedraw( TRUE );

   rect.left = rect.right = pt.x - ptDiff.x + ::GetSystemMetrics( SM_CXBORDER );
   rect.top = rect.bottom = pt.y - ptDiff.y + ::GetSystemMetrics( SM_CYBORDER );

   CRectTracker rectTracker( rect, CRectTracker::hatchedBorder );
   rectTracker.m_sizeMin.cx = TZCtrlCover::m_nDragX;
   rectTracker.m_sizeMin.cy = TZCtrlCover::m_nDragY;

   do  // purist's goto
   {

   if ( (rectTracker.TrackRubberBand( pTrackWnd, pt, TRUE ) &&
         (rectTracker.m_rect.NormalizeRect( ),
          (rectTracker.m_rect.Width( ) > rectTracker.m_sizeMin.cx) ||
          (rectTracker.m_rect.Height( ) > rectTracker.m_sizeMin.cy))) ||
        m_bRButtonUp )
   {
      if ( m_bRButtonUp )
         SwapMouseButton( m_pPainterWindow->m_pBar->m_bMouseSwapped );

      SetRedraw( FALSE );
      mDeleteInit( pTrackWnd );
      SetRedraw( TRUE );

      if ( (GetKeyState( VK_ESCAPE ) < 0) )
         break;  // goto cleanup

      rectTracker.m_rect.NormalizeRect( );

      if ( (rectTracker.m_rect.Width( ) <= rectTracker.m_sizeMin.cx) ||
           (rectTracker.m_rect.Height( ) <= rectTracker.m_sizeMin.cy) )
      {
         rectTracker.m_rect.left = rectTracker.m_rect.right;
         rectTracker.m_rect.top = rectTracker.m_rect.bottom;
      }

      // Adjust for scroll position.
   // if ( (m_pPainterWindow->m_fWndState & zTZPW_SCROLLADJUSTED) == 0 )
      {
         CPoint ptScrollPos = GetDeviceScrollPosition( );
         rectTracker.m_rect.top += ptScrollPos.y;
         rectTracker.m_rect.left += ptScrollPos.x;
         rectTracker.m_rect.bottom += ptScrollPos.y;
         rectTracker.m_rect.right += ptScrollPos.x;
      }

   // TraceRect( "CRectTracker return rect", rectTracker.m_rect );

      CPoint ptStart;
      CPoint ptEnd;
      CRect  rectSelect;
      zSHORT nPos;
      zBOOL  bShift = (uModKeys & MK_SHIFT);
      zBOOL  bCtrl = (uModKeys & MK_CONTROL);

      rectSelect = rectTracker.m_rect;
      ptStart.x = rectTracker.m_rect.left;
      ptStart.y = rectTracker.m_rect.top;
      ptEnd.x = rectTracker.m_rect.right;
      ptEnd.y = rectTracker.m_rect.bottom;
      m_ptMouseSelect = ptStart;
      pt = ptEnd;

      // Paint a new control if one of the toolbar controls is selected.
      // Otherwise, select all controls within the rubber band.
      if ( m_bRButtonUp ||
           m_pPainterWindow->m_pBar->
                           PaintToolBoxCtrl( m_pPainterWindow,
                                             m_ptMouseSelect, pt ) == FALSE )
      {
         // If we were not in paint mode on a specific control (as
         // determined by the result of the PaintToolBoxCtrl method),
         // select controls which were enclosed by the rubber band action.
         CRect  rectCtrl;
         zLONG  lSelectOne = -1;
         zLONG  lSelectPos = -1;

         // Select the ModalCursor if the right button is used
         if ( m_bRButtonUp )
            m_pPainterWindow->m_pBar->SelectModalPointer( );

         if ( rectSelect.bottom - rectSelect.top <= 4 &&
              rectSelect.right - rectSelect.left <= 4 )
         {
            lSelectOne = 64000;  // we are going to select the one whose top
                                 // is closest to the selection point.
         }

         // Tabbing selection, if the right mouse button was used
         // with the shift key, set the tabbing for the controls
         // enclosed in the rubber band;
         if ( bShift && m_bRButtonUp )
         {
            // First, go through and see if any controls were selected,
            // if not, reset the tabbing for all objects
            for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
            {
               pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
               if ( pCtrl->GetVisibility( ) ) // not visible or not selectable
                  continue;

               pCtrl->ParentToPainterRect( rectCtrl );

               // At least the top-left halves of the control must
               // be in the selection rectangle ... or the selection
               // rectangle must be completely in the control rectangle
               // for the control to be selected.
               if ( (rectCtrl.left >= rectSelect.left &&
                     rectCtrl.top >= rectSelect.top  &&
                     rectCtrl.right - ((rectCtrl.right - rectCtrl.left) / 2) <=
                                                       rectSelect.right &&
                     rectCtrl.bottom - ((rectCtrl.bottom - rectCtrl.top) / 2) <=
                                                       rectSelect.bottom) ||
                    (rectSelect.left >= rectCtrl.left &&
                     rectSelect.top >= rectCtrl.top &&
                     rectSelect.right <= rectCtrl.right &&
                     rectSelect.bottom <= rectCtrl.bottom) )
               {
                  if ( lSelectOne >= 0 )
                  {
                     if ( rectSelect.left >= rectCtrl.left &&
                          rectSelect.top >= rectCtrl.top &&
                          rectSelect.right <= rectCtrl.right &&
                          rectSelect.bottom <= rectCtrl.bottom )
                     {
                        if ( rectSelect.top - rectCtrl.top <= lSelectOne )
                        {
                           lSelectOne = rectSelect.top - rectCtrl.top;
                           lSelectPos = nPos;
                        }
                     }
                  }
                  else
                  {
                     lSelectPos = nPos;
                     break;
                  }
               }
            }

            if ( lSelectOne >= 0 && lSelectPos >= 0 )
            {
               pCtrl = m_pPainterWindow->m_CtrlList[ lSelectPos ];

               // We will decrement the TabStop number for the control if
               // the Ctrl key is being held down.
               if ( bCtrl &&
                    pCtrl->m_nTabStopNbr != 9999 &&
                    pCtrl->m_nTabStopNbr > 1 )
               {
                  pCtrl->m_nTabStopNbr--;
               }
               else
               {
                  m_pPainterWindow->m_nMaxTabIdNbr++;
                  pCtrl->m_nTabStopNbr = m_pPainterWindow->m_nMaxTabIdNbr;
               }

               m_pPainterWindow->m_pPainterClient->ShowTabbing( 1 );
            }
            else
            if ( lSelectPos != -1 )
            {
               // Go through and assign tabbing ID's to the
               // controls within the rubber band group.
               zLONG lWkPosX, lLastPosX;
               zLONG lWkPosY, lLastPosY;
               TZPainterCtrl *pTabCtrl;

               lLastPosX = -1;
               lLastPosY = -1;

               for ( ; ; )
               {
                  pTabCtrl = 0;
                  lWkPosX = 9999;
                  lWkPosY = 9999;
                  for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
                  {
                     pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];

                     // If the WS_TABSTOP attribute is not set for this ctrl,
                     // do not permit the control to be assigned a tabstop
                     // number.  HOLD ON THERE!!! ... On second thought,
                     // this is disabled since we do want to be able to
                     // order text fields even though they cannot accept
                     // a tab stop.  If the text contains an '&', the
                     // dialog manager will use the text fields accelerator
                     // key to position to the next field that will accept
                     // a tab stop.
                  // if ( pCtrl->m_lSubtype & zCONTROL_NOTABSTOP )
                  //    continue;

                     pCtrl->ParentToPainterRect( rectCtrl );
                     if ( pCtrl->GetVisibility( ) == 0 && // visible and selectable
                          ((rectCtrl.left >= rectSelect.left &&
                            rectCtrl.top  >= rectSelect.top  &&
                            rectCtrl.right -
                              ((rectCtrl.right - rectCtrl.left) / 2) <=
                                                          rectSelect.right &&
                            rectCtrl.bottom -
                              ((rectCtrl.bottom - rectCtrl.top) / 2) <=
                                                        rectSelect.bottom) ||
                           (rectSelect.left >= rectCtrl.left &&
                            rectSelect.top  >= rectCtrl.top &&
                            rectSelect.right <= rectCtrl.right &&
                            rectSelect.bottom <= rectCtrl.bottom &&
                            m_pPainterWindow->m_pPainterClient->
                                OnlyCtrlContainsPt( pCtrl,
                                                    rectSelect ) == TRUE)) &&
                          (rectCtrl.top < lWkPosY ||
                           (rectCtrl.top  == lWkPosY &&
                            rectCtrl.left <  lWkPosX)) &&
                          (rectCtrl.top > lLastPosY ||
                           (rectCtrl.top  == lLastPosY &&
                            (rectCtrl.left > lLastPosX ||
                             pCtrl->m_nTabStopNbr == 9999))) )
                     {
                        pTabCtrl = pCtrl;
                        lWkPosX = rectCtrl.left;
                        lWkPosY = rectCtrl.top;
                     }
                  }

                  if ( pTabCtrl == 0 )
                     break;

                  pTabCtrl->ParentToPainterRect( rectCtrl );
                  lLastPosX = rectCtrl.left;
                  lLastPosY = rectCtrl.top;
                  m_pPainterWindow->m_nMaxTabIdNbr++;
                  pTabCtrl->m_nTabStopNbr = m_pPainterWindow->m_nMaxTabIdNbr;
               }

               m_pPainterWindow->m_pPainterClient->ShowTabbing( 1 );
            }
         }
         else
         {
            TZPainterCtrl *pCtrlParent;
            zBOOL  bParentSelected = FALSE;

            // Deselect any controls that were selected (if no Ctrl key).
            if ( bCtrl == FALSE )
               m_pPainterWindow->Select( 0 );  // clear all selections

            for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
            {
               pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
               if ( bCtrl == FALSE )
               {
                  pCtrlParent = pCtrl->m_pCtrlParent;
                  bParentSelected = FALSE;
                  while ( pCtrlParent )
                  {
                     if ( pCtrlParent->IsSelected( ) )
                     {
                        bParentSelected = TRUE;
                        break;
                     }

                     pCtrlParent = pCtrlParent->m_pCtrlParent;
                  }
               }

               pCtrl->ParentToPainterRect( rectCtrl );

               // Change right and bottom position of rectCtrl to
               // be the midpoint of the control for selection.
               rectCtrl.right  -= ((rectCtrl.right - rectCtrl.left) / 2);
               rectCtrl.bottom -= ((rectCtrl.bottom - rectCtrl.top) / 2);
               if ( bParentSelected == FALSE &&
                    rectCtrl.left >= rectSelect.left &&
                    rectCtrl.top  >= rectSelect.top  &&
                    rectCtrl.right <= rectSelect.right &&
                    rectCtrl.bottom <= rectSelect.bottom )
               {
                  pCtrl->Select( );
               }
            }

            // Deselect any ctrls that have a selected parent (Don's request).
            for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
            {
               pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
               if ( pCtrl->IsSelected( ) && pCtrl->m_pCtrlParent &&
                    pCtrl->m_pCtrlParent->IsSelected( ) )
               {
                  pCtrl->Deselect( );
               }
            }
         }

         m_pPainterWindow->SetDIL_Text( );   // Set the DilMessage
      }
   }
   else
   {
      SetRedraw( FALSE );
      mDeleteInit( pTrackWnd );
      SetRedraw( TRUE );

      // The mouse was not moved as a result of the rubber banding,
      // so deselect all ctrls and deselect any selected ctrl type
      // on the tool bar.
   // TraceRect( "CRectTracker escape rect", rectTracker.m_rect );
   // ::MessageBeep( MB_ICONQUESTION );

      // Deselect whichever toolbar control is highlighted.
      m_pPainterWindow->m_pBar->SelectModalPointer( );

      m_pPainterWindow->Select( 0 );  // clear all selections

      if ( m_pPainterWindow->m_pPaintParent )
      {
         // Select m_pPaintParent if they did not move the mouse.
//       TraceLineS( "SELECTING PAINT PARENT???", "" );
         m_pPainterWindow->m_pPaintParent->Select( );

         // Clear paint parent to avoid errors!
         m_pPainterWindow->m_pPaintParent = 0;

         // Set the DilMessage
         m_pPainterWindow->SetDIL_Text( );
      }
   }
   } while ( FALSE );   // end of:  purists goto

   m_bRButtonUp = FALSE;
}

void
TZPainterClient::OnLButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZPainterClient::OnLButtonUp??? ", pt );
// MessageBox( "TZPainterClient::OnLButtonUp", " shouldn't be here!!!", MB_OK );
#endif

#if 0
   if ( m_bCapture )
   {
      m_bCapture = FALSE;
      ReleaseCapture( );
   }

   m_bRButtonUp = FALSE;
#endif
}

void
TZPainterClient::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZPainterClient::OnLButtonDblClk", pt );
#endif

   // If the mouse button has been double clicked, go to the update window.
   if ( m_pPainterWindow->m_vDialog &&
        (m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
   {
      zVIEW vEvents;
      zVIEW vTZPESRCO;

      // Update the Zeidon window before anything else.
      m_pPainterWindow->UpdateZeidonWindow( );

      // Create an Events Object for window events
      if ( GetViewByName( &vEvents, szlTZPNEVWO,
                          m_pPainterWindow->m_pBar->m_vSubtask,
                          zLEVEL_TASK ) > 0 )
      {
         DropObjectInstance( vEvents );
      }

      ActivateEmptyObjectInstance( &vEvents, szlTZPNEVWO,
                                   m_pPainterWindow->m_pBar->m_vSubtask,
                                   zMULTIPLE );
      SetNameForView( vEvents, szlTZPNEVWO, 0, zLEVEL_TASK );
      GetViewByName( &vTZPESRCO, szlTZPESRCO,
                     m_pPainterWindow->m_pBar->m_vSubtask,
                     zLEVEL_TASK );
      SetCursorFirstEntity( vTZPESRCO, szlWindowStyle, 0 );

      zSHORT nRC = SetCursorFirstEntity( vTZPESRCO, szlEventDef, 0 );
      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vEvents, szlEvent, zPOS_AFTER );
         SetMatchingAttributesByName( vEvents, szlEvent,
                                      vTZPESRCO, szlEventDef, zSET_ALL );
         CreateEntity( vEvents, szlEventAct, zPOS_AFTER );
         nRC = SetCursorNextEntity( vTZPESRCO, szlEventDef, 0 );
      }

      // Now set the actions defined for the window
      zLONG lType;

      nRC = SetCursorFirstEntity( m_pPainterWindow->m_vDialog,
                                  szlWndEvent, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         GetIntegerFromAttribute( &lType, m_pPainterWindow->m_vDialog,
                                  szlWndEvent, szlType );
         if ( SetCursorFirstEntityByInteger( vEvents, szlEvent, szlType,
                                             lType, 0 ) > zCURSOR_UNCHANGED )
         {
            SetMatchingAttributesByName( vEvents, szlEventAct,
                                         m_pPainterWindow->m_vDialog,
                                         szlWndAct, zSET_ALL );
         }

         nRC = SetCursorNextEntity( m_pPainterWindow->m_vDialog,
                                    szlWndEvent, 0 );
      }

      // Name the view for the subdialog
      // if ( (m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
      //    CreateTemporalSubobjectVersion( m_pPainterWindow->m_vDialog,
      //                                    szlPage );
      // else
      //    CreateTemporalSubobjectVersion( m_pPainterWindow->m_vDialog,
      //                                    szlWindow );

      CPoint ptScrollPos = GetDeviceScrollPosition( );
      SetProfileNbr( m_pPainterWindow->m_vDialog, "Design",
                     m_pPainterWindow->m_csDlgTag,
                     m_pPainterWindow->m_csWndTag,
                     "ScrollX", ptScrollPos.x );
      SetProfileNbr( m_pPainterWindow->m_vDialog, "Design",
                     m_pPainterWindow->m_csDlgTag,
                     m_pPainterWindow->m_csWndTag,
                     "ScrollY", ptScrollPos.y );
      SetNameForView( m_pPainterWindow->m_vDialog,
                      szlTZWINDOW, 0, zLEVEL_TASK );
      m_pPainterWindow->m_pBar->ResetSelectedMouseReleaseSwap( );
      SetWindowActionBehavior( m_pPainterWindow->m_pBar->m_vSubtask,
                               zWAB_StartModalSubwindow,
                               szlTZPNTRAD, szlWND_UPD );
      m_pPainterWindow->m_pBar->EnablePainterWindows( 0 );
      ProcessActionLoop( m_pPainterWindow->m_pBar->m_pZSubtask,
                         0, 0, 0, 0, FALSE, "(pntr wnd)" );
   }
}

void
TZPainterClient::OnRButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZPainterClient::OnRButtonDown", pt );
#endif

// The SwapMouseButton function reverses or restores the meaning of the
// left and right mouse buttons.
//    Parameters:
//       fSwap - Specifies whether the mouse button meanings are reversed
//               or restored.  If this parameter is TRUE, the left button
//               generates right-button messages and the right button
//               generates left-button messages.  If this parameter is FALSE,
//               the buttons are restored to their original meanings.
//
//    Return Values:
//       If the meaning of the mouse buttons was reversed previously,
//       before the function was called, the return value is nonzero.
//       If the meaning of the mouse buttons was not reversed, the
//       return value is zero.

// m_bCapture = TRUE;
// SetCapture( );
   if ( (uModKeys & MK_CONTROL) == 0 && (uModKeys & MK_SHIFT) == 0 )
   {
   // ShowContextMenu( pt );
   }
   else
   {
      m_pPainterWindow->Select( 0 );  // clear all selections
      m_bRButtonUp = TRUE;
      SwapMouseButton( !m_pPainterWindow->m_pBar->m_bMouseSwapped );

      OnLButtonDown( uModKeys, pt );
   }

// LPARAM lParam = MAKELPARAM( pt.x, pt.y );
// SendMessage( WM_LBUTTONDOWN, uModKeys, lParam );
}

void
TZPainterClient::OnRButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZPainterClient::OnRButtonUp", pt );
#endif

#if 0
   if ( m_bCapture )
   {
      m_bCapture = FALSE;
      ReleaseCapture( );
   }

   m_bRButtonUp = FALSE;
#endif

// MessageBox( "TZPainterClient::OnRButtonUp", " shouldn't be here!!!", MB_OK );
}

void
TZPainterClient::OnPaint( )
{
#ifdef DEBUG_ALL // lots of messages
   TraceLineS( "TZPainterClient::OnPaint", m_pPainterWindow->m_csWndTag );
#endif

#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZPainterClient::OnPaint", "" );
#endif

// CPaintDC( this );
   CFormView::OnPaint( );
}

void
TZPainterClient::OnNcPaint( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::OnNcPaint", m_pPainterWindow->m_csWndTag );
#endif

   CFormView::OnNcPaint( );
}

void
TZPainterClient::OnDraw( CDC *pDC )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZPainterClient::OnDraw", "" );
#endif

#if 0
   if ( m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
   {
      CRect rect;

      GetUpdateRect( rect );
      if ( rect.left < m_pPainterWindow->m_lLeftMargin ||
           rect.right > m_pPainterWindow->m_lLeftMargin +
             mConvert256thInchToPixelX( m_pPainterWindow->m_lRptSizeX ) ||
           rect.top < m_pPainterWindow->m_lTopMargin )
      {
         CRect rectOrig = rect;
         CBrush brush( ::GetSysColor( COLOR_BTNFACE ) );

         if ( rect.left < m_pPainterWindow->m_lLeftMargin )
         {
            if ( rect.right > m_pPainterWindow->m_lLeftMargin )
               rect.right = m_pPainterWindow->m_lLeftMargin;

            pDC->FillRect( rect, &brush );
            if ( rect.right == m_pPainterWindow->m_lLeftMargin )
            {
               pDC->MoveTo( rect.right - 2, rect.top );
               pDC->LineTo( rect.right - 2, rect.bottom );
            }

            rect = rectOrig;
         }

         if ( rect.right > m_pPainterWindow->m_lLeftMargin +
                 mConvert256thInchToPixelX( m_pPainterWindow->m_lRptSizeX ) )
         {
            if ( rect.left < m_pPainterWindow->m_lLeftMargin +
                 mConvert256thInchToPixelX( m_pPainterWindow->m_lRptSizeX ) )
            {
               rect.left = m_pPainterWindow->m_lLeftMargin +
                 mConvert256thInchToPixelX( m_pPainterWindow->m_lRptSizeX );
            }

            pDC->FillRect( rect, &brush );
            if ( rect.left == m_pPainterWindow->m_lLeftMargin +
                 mConvert256thInchToPixelX( m_pPainterWindow->m_lRptSizeX ) )
            {
               pDC->MoveTo( rect.left + 2, rect.top );
               pDC->LineTo( rect.left + 2, rect.bottom );
            }

            rect = rectOrig;
         }

         if ( rect.top < m_pPainterWindow->m_lTopMargin )
         {
            CBrush brush( ::GetSysColor( COLOR_BTNFACE ) );
            if ( rect.bottom > m_pPainterWindow->m_lTopMargin )
               rect.bottom = m_pPainterWindow->m_lTopMargin;

            pDC->FillRect( rect, &brush );
         }
      }
   }
// else
// {
//    if ( m_pPainterWindow->m_fWndState & zTZPW_SHOWTABS )
//       ShowTabbing( TRUE );
// }
#else
// TraceLineS( "TZPainterClient::OnDraw", "" );
   ZDrDoc *pDoc = (ZDrDoc *)GetDocument( );
   ASSERT_VALID( pDoc );

   // TODO: add draw code for native data here
   // TODO: also draw all OLE items in the document

   // Draw the selection at an arbitrary position.  This code should be
   // removed once your real drawing code is implemented.  This position
   // corresponds exactly to the rectangle returned by ZDrOleClient,
   // to give the effect of in-place editing.

   // If possible, we paint to an off-screen DC and BitBlt the results to
   // the screen (reduces flicker).
   CDC      *pDCPaint;
   zBOOL    bMemDC = FALSE;
   CDC      memDC;
   CBitmap  bmp;
   CBitmap  *pOldBmp = 0;
   CRect    rectClient;

   GetClientRect( &rectClient );

   if ( 0 && memDC.CreateCompatibleDC( pDC ) &&
        bmp.CreateCompatibleBitmap( pDC, rectClient.Width( ),
                                    rectClient.Height( ) ) )
   {
      // Off-screen DC successfully created. Paint to it!
      bMemDC = TRUE;
      pOldBmp = memDC.SelectObject( &bmp );
      pDCPaint = &memDC;
   }
   else
      pDCPaint = pDC;

   // Redraw each client item as long as it is child of current view.
   POSITION pos = pDoc->GetStartPosition( );
   while ( pos )
   {
      ZDrOleClient *pItem = (ZDrOleClient *) pDoc->GetNextClientItem( pos );
      if ( pItem->GetParentView( ) == this )
      {
//         if ( pDCPaint->RectVisible( pItem->GetRect( ) ) )
         {
            pItem->Draw( pDCPaint, pItem->GetRect( ) ); // Draw Item itself

            CRectTracker rt;
            pItem->SetupTracker( rt );
            rt.Draw( pDCPaint ); // draw surrounding rectangle
         }
      }
   }

   if ( bMemDC )
   {
      // Copy the results to the on-screen DC.
   // CRect rect( rectClient );
      CRect rect;
   // pDC->GetClipBox( rect );  // need to do this if using memory DC,
                                // otw rect is uninitialized!!!
                                // Could be initialized by rectClient (DKS)
      pDC->BitBlt( rect.left, rect.top, rect.Width( ), rect.Height( ),
                   &memDC, rect.left, rect.top, SRCCOPY );

      memDC.SelectObject( pOldBmp );
   }
#endif
}

BOOL
TZPainterClient::OnEraseBkgnd( CDC *pDC )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::OnEraseBkgnd", "" );
#endif

   // Set brush to desired background color.
   CBrush backBrush( ::GetSysColor( COLOR_BTNFACE ) );

   // Save old brush.
   CBrush *pOldBrush;

   if ( m_pPainterWindow->m_pBar->m_fState & zTZCB_GRID )
   {
      m_pBrush->UnrealizeObject( );
      pDC->SetBrushOrg( 0, 0 );
      pOldBrush = pDC->SelectObject( m_pBrush );
   }
   else
      pOldBrush = pDC->SelectObject( &backBrush );

   CRect rect;
   pDC->GetClipBox( &rect );     // Erase the area needed.
   pDC->PatBlt( rect.left, rect.top, rect.Width( ),
                rect.Height( ), PATCOPY );
   pDC->SelectObject( pOldBrush );

   return( TRUE );
// return( CFormView::OnEraseBkgnd( pDC ) );
}

HBRUSH
TZPainterClient::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlType )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterClient::OnCtlColor", "" );
#endif

   HBRUSH hBrush = CFormView::OnCtlColor( pDC, pWnd, uCtlType );

   // INSERT>> Your code here.
   if ( uCtlType != CTLCOLOR_DLG )
   {
      zSHORT  nPos;
      TZPainterCtrl *pCtrl;

      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pWndCtrl->m_hWnd == pWnd->m_hWnd )
         {
            hBrush = pCtrl->CtrlColor( pDC, hBrush );
            break;
         }
      }

      return( hBrush );
   }

   if ( m_pPainterWindow->m_pBar->m_fState & zTZCB_GRID )
      return( *m_pBrush );
   else
      return( hBrush );
}

void
TZPainterClient::OnSize( UINT uType, int cx, int cy )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterClient::OnSize      cx ", cx );
   TraceLineI( "TZPainterClient::OnSize      cy ", cy );
   TraceLineI( "TZPainterClient::OnSize m_lScrollUnitsX ",
               m_pPainterWindow->m_lScrollUnitsX );
   TraceLineI( "TZPainterClient::OnSize m_lScrollUnitsY ",
               m_pPainterWindow->m_lScrollUnitsY );
#endif

   CWnd::OnSize( uType, cx, cy );

   if ( m_pPainterWindow->m_pBar )
   {
      if ( m_pPainterWindow->m_pBar->m_fState & zTZCB_SCROLLBARS )
      {
         SetScrollSizes( MM_TEXT,
                         CSize( m_pPainterWindow->m_lHScrollMax + 200,
                                m_pPainterWindow->m_lVScrollMax + 200 ),
                         CSize( cx, cy ),
                         CSize( m_pPainterWindow->m_lScrollUnitsX,
                                m_pPainterWindow->m_lScrollUnitsY ) );
      }
      else
      {
         SetScrollSizes( MM_TEXT, CSize( 0, 0 ) );
      }
   }
}

zSHORT OPERATION
fnCompareCtrlPosition( CArray<TZPainterCtrl *, TZPainterCtrl *> *pca,
                       zLONG  lLeft,
                       zLONG  lLargest,
                       zPVOID pvSortData )
{
   TZPainterCtrl *pCtrl1 = pca->GetAt( lLeft - 1 );
   TZPainterCtrl *pCtrl2 = pca->GetAt( lLargest - 1 );
   if ( pCtrl1 == 0 || pCtrl2 == 0 )
   {
      if ( pCtrl1 == 0 && pCtrl2 == 0 )
         return( 0 );
      else
      if ( pCtrl1 == 0 )
         return( -1 );
      else
   // if ( pCtrl2 == 0 )
         return( 1 );
   }

   TZPainterCtrl *pGroupSet1 = pCtrl1;
   TZPainterCtrl *pGroupSet2 = pCtrl2;
   TZPainterCtrl *pGroup1 = pCtrl1;
   TZPainterCtrl *pGroup2 = pCtrl2;
   zLONG lTop1 = pCtrl1->m_rectCtrl.top;
   zLONG lTop2 = pCtrl2->m_rectCtrl.top;
   zLONG lLeft1 = pCtrl1->m_rectCtrl.left;
   zLONG lLeft2 = pCtrl2->m_rectCtrl.left;
   zSHORT nLevel1 = 0;
   zSHORT nLevel2 = 0;
   zSHORT nRC = 0;

   while ( pGroupSet1->m_pCtrlParent )       // find Group and GroupSet
   {
      nLevel1++;
      pGroupSet1 = pGroupSet1->m_pCtrlParent;
      lTop1 += pGroupSet1->m_rectCtrl.top;
      lLeft1 += pGroupSet1->m_rectCtrl.left;
      if ( pGroupSet1->m_pCtrlParent )
         pGroup1 = pGroupSet1;
   }

   if ( nLevel1 == 0 )
      pGroup1 = 0;

   while ( pGroupSet2->m_pCtrlParent )       // find Group and GroupSet
   {
      nLevel2++;
      pGroupSet2 = pGroupSet2->m_pCtrlParent;
      lTop2 += pGroupSet2->m_rectCtrl.top;
      lLeft2 += pGroupSet2->m_rectCtrl.left;
      if ( pGroupSet2->m_pCtrlParent )
         pGroup2 = pGroupSet2;
   }

   if ( nLevel2 == 0 )
      pGroup2 = 0;

   if ( pGroupSet1 != pGroupSet2 )
   {
      if ( pGroupSet1->m_chType == 'H' )
         nRC = -5;  // Header must go first
      else
      if ( pGroupSet2->m_chType == 'H' )
         nRC = 5;   // Header must go first
      else
      if ( pGroupSet1->m_chType == 'F' )
         nRC = 5;   // Footer must go last
      else
      if ( pGroupSet2->m_chType == 'F' )
         nRC = -5;  // Footer must go last
      else
      {
         pCtrl1 = pGroupSet1;    // compare on basis of GroupSet
         pCtrl2 = pGroupSet2;    // using position
         lTop1 = pCtrl1->m_rectCtrl.top;
         lTop2 = pCtrl2->m_rectCtrl.top;
         lLeft1 = pCtrl1->m_rectCtrl.left;
         lLeft2 = pCtrl2->m_rectCtrl.left;
      }
  }
   else
   if ( pGroup1 != pGroup2 )
   {
      if ( pGroup1 == 0 )
         nRC = -4;
      else
      if ( pGroup2 == 0 )
         nRC = 4;
      else
      if ( pGroup1->m_chType == 'h' )
         nRC = -3;  // Header must go first
      else
      if ( pGroup2->m_chType == 'h' )
         nRC = 3;   // Header must go first
      else
      if ( pGroup1->m_chType == 'f' )
         nRC = 3;   // Footer must go last
      else
      if ( pGroup2->m_chType == 'f' )
         nRC = -3;  // Footer must go last
      else
      {
         pCtrl1 = pGroup1;       // compare on basis of Group
         pCtrl2 = pGroup2;       // using position
         lTop1 = pCtrl1->m_rectCtrl.top;
         lTop2 = pCtrl2->m_rectCtrl.top;
         lLeft1 = pCtrl1->m_rectCtrl.left;
         lLeft2 = pCtrl2->m_rectCtrl.left;
      }
   }
   else
   if ( nLevel1 == 0 && nLevel2 > 0 )  // Order GroupSet first
      nRC = -2;
   else
   if ( nLevel1 > 0 && nLevel2 == 0 )  // Order GroupSet first
      nRC = 2;
   else
   if ( nLevel1 == 1 && nLevel2 > 1 )  // Order group first
      nRC = -2;
   else
   if ( nLevel1 > 1 && nLevel2 == 1 )  // Order group first
      nRC = 2;

   // If we get to here, both ctrls are of the same type (GroupSet, Group,
   // or Ctrl).

   // If the top of the previous is lower than the top of the current
   // then we need to exchange (or if tops are equal and the bottom
   // of the previous is lower than the bottom of the current).
   if ( nRC )
   {
      // already determined return code
   }
   else
   if ( lTop1 < lTop2 )
      nRC = -1;
   else
   if ( lTop1 > lTop2 )
      nRC = 1;
   else
   if ( lLeft1 < lLeft2 )
      nRC = -1;
   else
   if ( lLeft1 > lLeft2 )
      nRC = 1;
   else
   if ( pCtrl1->m_rectCtrl.bottom < pCtrl2->m_rectCtrl.bottom )
      nRC = -1;
   else
   if ( pCtrl1->m_rectCtrl.bottom > pCtrl2->m_rectCtrl.bottom )
      nRC = 1;
   else
   if ( pCtrl1->m_rectCtrl.right < pCtrl2->m_rectCtrl.right )
      nRC = -1;
   else
   if ( pCtrl1->m_rectCtrl.right > pCtrl2->m_rectCtrl.right )
      nRC = 1;

#if 0
   if ( (zstrcmp( pCtrl1->m_csTag, "Rpt_PicGraphic.1" ) == 0 ||
         zstrcmp( pCtrl2->m_csTag, "Rpt_PicGraphic.1" ) == 0) &&
         (nRC == 1 || nRC == -1) )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, zsizeof( szMsg ), "Ctrl1: %s @Y:%ld Ctrl2: %s @Y:%ld RC = %ld",
                pCtrl1->m_csTag, lTop1, pCtrl2->m_csTag, lTop2, nRC );
      TraceLineS( szMsg, "" );
   }
#endif

   return( nRC );
}

zBOOL
TZPainterClient::AdjustGroupSets( )
{
   // There MUST always be at least two GroupSets ... PageHeader and PageFooter.
#ifdef DEBUG_ALL
   TraceLineI( "AdjustGroupSets m_bPaintWindow ", m_bPaintWindow );
   TraceLineI( "AdjustGroupSets m_pPageHeader ", (zLONG) m_pPageHeader );
   TraceLineI( "AdjustGroupSets m_pPageFooter ", (zLONG) m_pPageFooter );
#endif

   if ( m_bPaintWindow || m_pPageHeader == 0 || m_pPageFooter == 0 )
   {
      return( FALSE );
   }

   if ( m_nRecurse > 1 )   // we need to permit two recursions
      return( FALSE );

   m_nMultiMoveSize += 2;  // prevent status bar messages
   m_nRecurse++;

   TZPainterCtrl *pLastGroupSet = 0;
   zSHORT        nPosLastGroupSet = 0;

   TZPainterCtrl *pCtrl;
   TZPainterCtrl *pGroupSet;
   TZPainterCtrl *pLastGroup;
   zSHORT        nPosGroupSet;
   zSHORT        nPosGroup;
   zSHORT        nPos;
   zSHORT        nPosLastGroup;
   zLONG         lDiff;
   zBOOL         bAdjust = FALSE;

// SetRedraw( FALSE );

// #define zDEBUG_ADJUST
#ifdef zDEBUG_ADJUST
   // debug only!!!
   zCHAR szMsg[ 64 ];

   //               0123456789012345678
   strcpy_s( szMsg, zsizeof( szMsg ), "Prior to Adjust - x " );
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_chType )
         szMsg[ 18 ] = pCtrl->m_chType;
      else
      if ( pCtrl->m_pCtrlParent &&
           pCtrl->m_pCtrlParent->m_pCtrlParent )
         szMsg[ 18 ] = 'c';  // it's a control
      else
         szMsg[ 18 ] = 'x';

      strcpy_s( szMsg + 20, zsizeof( szMsg ) - 20, pCtrl->m_csTag );
      TraceRect( szMsg, pCtrl->m_rectCtrl );
   }
#endif

#if 0
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_chEntity == 'S' && zstrcmp( pCtrl->m_csTag, "FinancialAcctTransaction" ) == 0 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ), " PX %ld PY %ld SX %ld SY %ld",
                    pCtrl->m_rectCtrl.left, pCtrl->m_rectCtrl.top,
                    pCtrl->m_rectCtrl.right - pCtrl->m_rectCtrl.left,
                    pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top );
         TraceLineS( "TZPainterClient::AdjustGroupSets: FinancialAcctTransaction", szMsg );
      }
   }
#endif

   //////////////////////////////////////////////////////////////////////////
   // Guarantee that GroupSets and Groups are at least 20??? pixels high   //
   //////////////////////////////////////////////////////////////////////////
   for ( nPosGroupSet = 0;
         nPosGroupSet < m_pPainterWindow->m_nCtrlCnt;
         nPosGroupSet++ )
   {
      pGroupSet = m_pPainterWindow->m_CtrlList[ nPosGroupSet ];
      if ( pGroupSet->m_pCtrlParent )        // no parent for GroupSets
         continue;

      // ensure that each GroupSet is at least 20??? pixels high
      if ( pGroupSet->m_rectCtrl.bottom - pGroupSet->m_rectCtrl.top < 20 )
      {
         bAdjust = TRUE;
         pGroupSet->m_rectCtrl.bottom = pGroupSet->m_rectCtrl.top + 20;
      }

      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         // ensure that each Group is at least 3??? pixels high
      // zLONG lPixels =
      //    mConvert256thInchToPixelY( m_pPainterWindow->m_nGranularity );

         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent == pGroupSet &&
              pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top < 3 )
         {
            bAdjust = TRUE;
            pCtrl->m_rectCtrl.bottom = pCtrl->m_rectCtrl.top + 3;
         }
      }
   }

#if 1
   //////////////////////////////////////////////////////////////////////////
   // Guarantee that the GroupSets are in order ... there shouldn't be     //
   // many ... so use brute force.  This only puts the GroupSets in order  //
   // based on top and bottom positions (no size adjustments are made).    //
   //////////////////////////////////////////////////////////////////////////

   // The GroupSets are now in order ... ensure the Groups are in order

   //////////////////////////////////////////////////////////////////////////
   // Guarantee that the Groups are in order within their GroupSets ...    //
   // there shouldn't be many ... so use brute force.  This only puts      //
   // the Groups in order based on top and bottom positions (no size       //
   // adjustments are made).                                               //
   //////////////////////////////////////////////////////////////////////////

   HeapSortArray( (CArray<void *, void *> *) &(m_pPainterWindow->m_CtrlList),
                  (zFARPROC_COMPARE) fnCompareCtrlPosition,
                  m_pPainterWindow->m_nCtrlCnt, 0 );

#ifdef DEBUG_ALL
   TraceLineS( "AdjustGroupSets ", "List After HeapSort" );
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      zCHAR szMsg[ 256 ];

      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      sprintf_s( szMsg, zsizeof( szMsg ), " %s PX %ld PY %ld SX %ld SY %ld", pCtrl->m_csTag,
                pCtrl->m_rectCtrl.left, pCtrl->m_rectCtrl.top,
                pCtrl->m_rectCtrl.right - pCtrl->m_rectCtrl.left,
                pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top );
      if ( pCtrl->m_pCtrlParent == 0 )        // no parent for GroupSets
         TraceLineS( "GroupSet:", szMsg );
      else
      {
         if ( pCtrl->m_pCtrlParent->m_pCtrlParent )
            TraceLineS( "      Ctrl:", szMsg );
         else
            TraceLineS( "   Group:", szMsg );
      }
   }
#endif

#else

   //////////////////////////////////////////////////////////////////////////
   // Guarantee that the GroupSets are in order ... there shouldn't be     //
   // many ... so use brute force.  This only puts the GroupSets in order  //
   // based on top and bottom positions (no size adjustments are made).    //
   //////////////////////////////////////////////////////////////////////////
   pLastGroupSet = 0;
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_pCtrlParent == 0 )        // no parent for GroupSets
      {
         if ( pLastGroupSet == 0 )
         {
            pLastGroupSet = pCtrl;
            nPosLastGroupSet = nPos;
            continue;  // keep looking
         }

         // If the header is not first, push this one down.
         if ( pCtrl->m_chType == 'H' )
         {
            bAdjust = TRUE;

            // Exchange positions and restart.
            m_pPainterWindow->m_CtrlList[ nPos ] = pLastGroupSet;
            m_pPainterWindow->m_CtrlList[ nPosLastGroupSet ] = pCtrl;
            nPos = -1;
            pLastGroupSet = 0;
            nPosLastGroupSet = 0;
            continue;
         }

         // If the footer is not last, push this one up.
         if ( pLastGroupSet->m_chType == 'F' )
         {
            bAdjust = TRUE;

            // Exchange positions and restart.
            m_pPainterWindow->m_CtrlList[ nPos ] = pLastGroupSet;
            m_pPainterWindow->m_CtrlList[ nPosLastGroupSet ] = pCtrl;
            nPos = -1;
            pLastGroupSet = 0;
            nPosLastGroupSet = 0;
            continue;
         }

         if ( pLastGroupSet->m_chType == 'H' ||
              pCtrl->m_chType == 'F' )
         {
            pLastGroupSet = pCtrl;
            nPosLastGroupSet = nPos;
            continue;   // Page Header must be first ... Page Footer must be
                        // last (taken care of in above code)
         }

         // If the top of the previous is lower than the top of the current
         // then we need to exchange (or if tops are equal and the bottom
         // of the previous is lower than the bottom of the current).
         if ( pLastGroupSet->m_rectCtrl.top - pCtrl->m_rectCtrl.top > 0 ||
              (pLastGroupSet->m_rectCtrl.top - pCtrl->m_rectCtrl.top == 0 &&
               pLastGroupSet->m_rectCtrl.bottom - pCtrl->m_rectCtrl.bottom > 0) )
         {
            // exchange positions and restart
            m_pPainterWindow->m_CtrlList[ nPos ] = pLastGroupSet;
            m_pPainterWindow->m_CtrlList[ nPosLastGroupSet ] = pCtrl;
            nPos = -1;
            pLastGroupSet = 0;
            nPosLastGroupSet = 0;
         }
         else  // no reversal
         {
            pLastGroupSet = pCtrl;
            nPosLastGroupSet = nPos;
         }
      }
   }

   // The GroupSets are now in order ... ensure the Groups are in order

   //////////////////////////////////////////////////////////////////////////
   // Guarantee that the Groups are in order within their GroupSets ...    //
   // there shouldn't be many ... so use brute force.  This only puts      //
   // the Groups in order based on top and bottom positions (no size       //
   // adjustments are made).                                               //
   //////////////////////////////////////////////////////////////////////////
   for ( nPosGroupSet = 0;
         nPosGroupSet < m_pPainterWindow->m_nCtrlCnt;
         nPosGroupSet++ )
   {
      pGroupSet = m_pPainterWindow->m_CtrlList[ nPosGroupSet ];
      if ( pGroupSet->m_pCtrlParent )        // no parent for GroupSets
         continue;

      pLastGroup = 0;
      nPosLastGroup = 0;

      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent != pGroupSet )
            continue;

         if ( pLastGroup == 0 )
         {
            pLastGroup = pCtrl;
            nPosLastGroup = nPos;
            continue;  // keep looking
         }

         // If the header is not first, push this one down.
         if ( pCtrl->m_chType == 'h' )
         {
            bAdjust = TRUE;

            // Exchange positions and restart.
            m_pPainterWindow->m_CtrlList[ nPos ] = pLastGroup;
            m_pPainterWindow->m_CtrlList[ nPosLastGroup ] = pCtrl;
            nPos = -1;
            pLastGroup = 0;
            nPosLastGroup = 0;
            continue;
         }

         // If the footer is not last, push this one up.
         if ( pLastGroup->m_chType == 'f' )
         {
            bAdjust = TRUE;

            // Exchange positions and restart.
            m_pPainterWindow->m_CtrlList[ nPos ] = pLastGroup;
            m_pPainterWindow->m_CtrlList[ nPosLastGroup ] = pCtrl;
            nPos = -1;
            pLastGroup = 0;
            nPosLastGroup = 0;
            continue;
         }

         if ( pLastGroup->m_chType == 'h' || pCtrl->m_chType == 'f' )
         {
            pLastGroup = pCtrl;
            nPosLastGroup = nPos;
            continue;   // Group Header must be first ... Group Footer must
                        // be last (taken care of in above code)
         }

         // If the top of the previous is lower than the top of the current
         // then we need to exchange (or if tops are equal and the bottom
         // of the previous is lower than the bottom of the current).
         if ( pLastGroup->m_rectCtrl.top - pCtrl->m_rectCtrl.top > 0 ||
              (pLastGroup->m_rectCtrl.top - pCtrl->m_rectCtrl.top == 0 &&
               pLastGroup->m_rectCtrl.bottom -
                                         pCtrl->m_rectCtrl.bottom > 0) )
         {
            // Exchange positions and restart.
            m_pPainterWindow->m_CtrlList[ nPos ] = pLastGroup;
            m_pPainterWindow->m_CtrlList[ nPosLastGroup ] = pCtrl;
            nPos = -1;
            pLastGroup = 0;
            nPosLastGroup = 0;
         }
         else  // no reversal, simply adjust position of this one
         {
            pLastGroup = pCtrl;
            nPosLastGroup = nPos;
         }
      }
   }
#endif

   // GroupSets and the Groups are now in order ... next ensure that
   // there is no overlap between Groups.

   //////////////////////////////////////////////////////////////////////////
   // We have now guaranteed that the Groups are in order within their     //
   // GroupSets ... now we want to ensure that Groups within a GroupSet    //
   // do not overlap (there is no exchange of Groups).                     //
   //////////////////////////////////////////////////////////////////////////
   for ( nPosGroupSet = 0;
         nPosGroupSet < m_pPainterWindow->m_nCtrlCnt;
         nPosGroupSet++ )
   {
      pGroupSet = m_pPainterWindow->m_CtrlList[ nPosGroupSet ];
      if ( pGroupSet->m_pCtrlParent )        // no parent for GroupSets
         continue;

//    TZGroupSetRpt *pGroupSetRpt;
//    pGroupSetRpt = DYNAMIC_DOWNCAST( TZGroupSetRpt, pGroupSet );
      pLastGroup = 0;
      nPosLastGroup = 0;

      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent != pGroupSet )
            continue;

         if ( pLastGroup == 0 )
         {
            // The top Group must be at least 20??? pixels from the top
            // of the GroupSet.
            if ( pCtrl->m_rectCtrl.top < 20 )
            {
               bAdjust = TRUE;
               pCtrl->m_rectCtrl.top += 20;
               pCtrl->m_rectCtrl.bottom += 20;
            }

            pLastGroup = pCtrl;
            nPosLastGroup = nPos;
            continue;  // keep looking
         }

         // If the bottom of the previous one is lower than the top of this
         // one, these two are already in order, so let's go on.
         lDiff = pLastGroup->m_rectCtrl.bottom + 2 -
                                                  pCtrl->m_rectCtrl.top;
         if ( lDiff <= 0 )
         {
            pLastGroup = pCtrl;
            nPosLastGroup = nPos;
            continue;
         }

         bAdjust = TRUE;
         pCtrl->m_rectCtrl.top += lDiff;
         pCtrl->m_rectCtrl.bottom += lDiff;

         pLastGroup = pCtrl;
         nPosLastGroup = nPos;
      }

      // Ensure that the GroupSet is large enough to hold its Groups.
      if ( pGroupSet && pLastGroup )
      {
         if ( pLastGroup->m_rectCtrl.bottom + 2 >
                  pGroupSet->m_rectCtrl.bottom - pGroupSet->m_rectCtrl.top )
         {
            bAdjust = TRUE;
            pGroupSet->m_rectCtrl.bottom =
               pGroupSet->m_rectCtrl.top + pLastGroup->m_rectCtrl.bottom + 2;
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   // Now the GroupSets and the Groups are in order and there is no        //
   // overlap between Groups ... next ensure that there is no overlap      //
   // between GroupSets (there is no exchange of GroupSets).               //
   //////////////////////////////////////////////////////////////////////////
   pLastGroupSet = 0;
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_pCtrlParent == 0 )        // no parent for GroupSets
      {
         if ( pLastGroupSet == 0 )
         {
            pLastGroupSet = pCtrl;
            nPosLastGroupSet = nPos;
            continue;  // keep looking
         }

         // If the bottom of the previous one is lower than the top of this
         // one, these two are already in order, so let's go on (note that
         // this maintains a 8-pixel spacer between GroupSets).
         lDiff = pLastGroupSet->m_rectCtrl.bottom + 8 -
                                                  pCtrl->m_rectCtrl.top;
         if ( lDiff <= 0 )
         {
            pLastGroupSet = pCtrl;
            nPosLastGroupSet = nPos;
            continue;
         }

         bAdjust = TRUE;

#ifdef zDEBUG_ADJUST
         TraceLineI( "AdjustGroupSets found overlap between GroupSets: ",
                     lDiff );
         TraceRect( pLastGroupSet->m_csTag, pLastGroupSet->m_rectCtrl );
         TraceRect( pCtrl->m_csTag, pCtrl->m_rectCtrl );
#endif

         pCtrl->m_rectCtrl.top += lDiff;
         pCtrl->m_rectCtrl.bottom += lDiff;

         pLastGroupSet = pCtrl;
         nPosLastGroupSet = nPos;
      }
   }

#if 0
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_chEntity == 'S' &&
           zstrcmp( pCtrl->m_csTag, "FinancialAcctTransaction" ) == 0 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, zsizeof( szMsg ), " PX %ld PY %ld SX %ld SY %ld",
                   pCtrl->m_rectCtrl.left, pCtrl->m_rectCtrl.top,
                   pCtrl->m_rectCtrl.right - pCtrl->m_rectCtrl.left,
                   pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top );
         TraceLineS( "TZPainterClient::AdjustGroupSets2 FinancialAcctTransaction",
                     szMsg );
      }
   }
#endif

   if ( bAdjust )
   {
#ifdef zDEBUG_ADJUST
      //               0123456789012345678
      strcpy_s( szMsg, zsizeof( szMsg ), "==>After Adjust - x" );

      // Move all covering windows.
      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];

         if ( pCtrl->m_chType )
            szMsg[ 18 ] = pCtrl->m_chType;
         else
         if ( pCtrl->m_pCtrlParent &&
              pCtrl->m_pCtrlParent->m_pCtrlParent )
            szMsg[ 18 ] = 'c';  // it's a control
         else
            szMsg[ 18 ] = 'x';

         TraceRect( szMsg, pCtrl->m_rectCtrl );
      }
#endif

      // Move all covering windows.
      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlCover && mIs_hWnd( pCtrl->m_pCtrlCover->m_hWnd ) &&
              pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
         {
            pCtrl->MoveCoveringWindow( );
         }
      }

   // SetRedraw( TRUE );
      Invalidate( );
   }

   if ( pLastGroupSet )
   {
//    TraceLineI( "AdjustGroupSets LastGroupSet bottom",
//                pLastGroupSet->m_rectCtrl.bottom );

      // Ensure that there is enough room in the report to permit additional
      // GroupSets to be created.
      m_pPainterWindow->m_pLastGroupSet = pLastGroupSet;
      if ( m_pPainterWindow->m_lHScrollMax != pLastGroupSet->m_rectCtrl.right ||
           m_pPainterWindow->m_lVScrollMax != pLastGroupSet->m_rectCtrl.bottom )
      {
         m_pPainterWindow->m_lHScrollMax = pLastGroupSet->m_rectCtrl.right;
         m_pPainterWindow->m_lVScrollMax = pLastGroupSet->m_rectCtrl.bottom;

         if ( m_pPainterWindow->m_pBar->m_fState & zTZCB_SCROLLBARS )
         {
            CRect rect;
            GetClientRect( rect );

            SetScrollSizes( MM_TEXT,
                            CSize( m_pPainterWindow->m_lHScrollMax + 200,
                                   m_pPainterWindow->m_lVScrollMax + 200 ),
                            CSize( rect.right, rect.bottom ),
                            CSize( m_pPainterWindow->m_lScrollUnitsX,
                                   m_pPainterWindow->m_lScrollUnitsY ) );
         }
         else
         {
            SetScrollSizes( MM_TEXT, CSize( 0, 0 ) );
         }
      }
   }

   m_nRecurse--;
   if ( m_nRecurse == 0 )
   {
      ///////////////////////////////////////////////////////////////////////
      // Everything is looking good.  Now determine the ranges of          //
      // overlapping controls within each Group.                           //
      ///////////////////////////////////////////////////////////////////////
      TZPainterCtrl *pCtrlTemp;
      zLONG  lRangeTop;
      zLONG  lRangeBottom;
      zLONG  lPosY;
      zLONG  lSizeY;
      zBOOL  bChange;
      zSHORT nCnt = m_pPainterWindow->m_nCtrlCnt - 1;  // last pos in array

      // Set the range for ctrls within each group.
      pLastGroupSet = 0;
      pLastGroup = 0;
      nPosGroup = -1;
      for ( nPos = 0; nPos <= nCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent == 0 ||                  // new GroupSet
              pCtrl->m_pCtrlParent->m_pCtrlParent == 0 )    // new Group
         {
            // We are at a point that we need to update ctrl ranges in
            // previous group (if there was one).
            if ( pLastGroup )
            {
               while ( nPosGroup < nPos )
               {
                  pCtrlTemp = m_pPainterWindow->m_CtrlList[ nPosGroup ];
                  pCtrlTemp->m_lRangeTop = lRangeTop;
                  pCtrlTemp->m_lRangeBottom = lRangeBottom;
                  nPosGroup++;
               }
            }
         }

         if ( pCtrl->m_pCtrlParent == 0 )                 // GroupSet
         {
            pCtrl->m_lRangeTop = 0;
            pCtrl->m_lRangeBottom = 0;
            pLastGroupSet = pCtrl;
            pLastGroup = 0;
            nPosGroup = -1;
            continue;
         }

         if ( pCtrl->m_pCtrlParent->m_pCtrlParent == 0 )  // Group
         {
            pCtrl->m_lRangeTop = 0;
            pCtrl->m_lRangeBottom = 0;
            lRangeTop = 0;
            lRangeBottom = 0;
            pLastGroup = pCtrl;
            nPosGroup = nPos + 1;
            continue;
         }

      // if ( zstrcmp( pCtrl->m_csTag, "Type" ) == 0 )
      //    TraceLineS( "", "" );

         bChange = FALSE;
         lPosY = 0;
         lSizeY = pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top;
         pCtrlTemp = pCtrl;
         while ( pCtrlTemp->m_pCtrlParent->m_pCtrlParent )
         {
            lPosY += pCtrlTemp->m_rectCtrl.top;
            pCtrlTemp = pCtrlTemp->m_pCtrlParent;
         }

         lSizeY += lPosY - 1;
         if ( lRangeTop == 0 )   // new range
         {
            lRangeTop = lPosY;
            lRangeBottom = lSizeY;
            if ( lRangeTop == 0 )
               lRangeTop = -1;
         }
         else
         if ( (lPosY >= lRangeTop && lPosY <= lRangeBottom) ||
              (lSizeY >= lRangeTop && lSizeY <= lRangeBottom) ||
              (lRangeTop >= lPosY && lRangeTop <= lSizeY) ||
              (lRangeBottom >= lPosY && lRangeBottom <= lSizeY) )
         {
            // Adding this ctrl to the current range.
            if ( lPosY < lRangeTop )
            {
               lRangeTop = lPosY;
            }

            if ( lSizeY > lRangeBottom )
            {
               lRangeBottom = lSizeY;
            }
         }
         else
         {
            bChange = TRUE;
         }

         if ( bChange || nPos == nCnt )   // change or at end of list
         {
            // Set ctrls in range ... then start new range.
            if ( nPosGroup < 0 )
               nPosGroup = 0;

            while ( nPosGroup < nPos )
            {
               pCtrlTemp = m_pPainterWindow->m_CtrlList[ nPosGroup ];
               pCtrlTemp->m_lRangeTop = lRangeTop;
               pCtrlTemp->m_lRangeBottom = lRangeBottom;
               nPosGroup++;
            }

            if ( nPos == nCnt )   // at end of list
            {
               pCtrl->m_lRangeTop = lRangeTop;
               pCtrl->m_lRangeBottom = lRangeBottom;
            }

            lRangeTop = lPosY;
            lRangeBottom = lSizeY;
            pLastGroup = pCtrl;
            nPosGroup = nPos;
         }
      }

#ifdef DEBUG_ALL
      TraceLineS( "AdjustGroupSets ", "List After Range" );
      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         zCHAR szMsg[ 256 ];

         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         sprintf_s( szMsg, zsizeof( szMsg ), " %s PX %ld PY %ld SX %ld SY %ld RX %ld RY %ld",
                   pCtrl->m_csTag,
                   pCtrl->m_rectCtrl.left, pCtrl->m_rectCtrl.top,
                   pCtrl->m_rectCtrl.right - pCtrl->m_rectCtrl.left,
                   pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top,
                   pCtrl->m_lRangeTop, pCtrl->m_lRangeBottom );
         if ( pCtrl->m_pCtrlParent == 0 )        // no parent for GroupSets
            TraceLineS( "GroupSet:", szMsg );
         else
         {
            if ( pCtrl->m_pCtrlParent->m_pCtrlParent )
               TraceLineS( "      Ctrl:", szMsg );
            else
               TraceLineS( "   Group:", szMsg );
         }
      }
#endif

   }

   m_nMultiMoveSize -= 2;  // permit status bar messages
   return( bAdjust );
}

void
TZPainterClient::OnHScroll( UINT uSBCode, UINT uThumbPos,
                            CScrollBar *pScrollBar )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterClient::OnHScroll uSBCode ", uSBCode );
   TraceLineI( "TZPainterClient::OnHScroll uThumbPos ", uThumbPos );
#endif

   CScrollView::OnHScroll( uSBCode, uThumbPos, pScrollBar );
   if ( m_pRuler )
   {
      zLONG  lScrollX;
      zLONG  lScrollY;
      CPoint ptScrollPos = GetDeviceScrollPosition( );
      lScrollX = ptScrollPos.x;
      lScrollY = ptScrollPos.y;
      m_pRuler->SetWindowPos( 0,
   // /* keep at top */       0, 0,
      /* let it scroll off */ -lScrollX, -lScrollY,
                              0, 0,
                              SWP_NOSIZE | SWP_NOZORDER );
   }
}

void
TZPainterClient::OnVScroll( UINT uSBCode, UINT uThumbPos,
                            CScrollBar *pScrollBar )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterClient::OnVScroll uSBCode ", uSBCode );
   TraceLineI( "TZPainterClient::OnVScroll uThumbPos ", uThumbPos );
#endif

   CScrollView::OnVScroll( uSBCode, uThumbPos, pScrollBar );
   if ( m_pRuler )
   {
      zLONG  lScrollX;
      zLONG  lScrollY;

      CPoint ptScrollPos = GetDeviceScrollPosition( );
      lScrollX = ptScrollPos.x;
      lScrollY = ptScrollPos.y;
      m_pRuler->SetWindowPos( 0,
   // /* keep at top */       0, 0,
      /* let it scroll off */ -lScrollX, -lScrollY,
                              0, 0,
                              SWP_NOSIZE | SWP_NOZORDER );
   }
}

// Drag Other Selected, draw/erase trace boxes for multi-select drag
void
TZPainterClient::DragOtherSelected( TZPainterCtrl *pCtrl,
                                    CPoint        pt,
                                    zBOOL         bShift,
                                    zBOOL         bCtrl )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterClient::DragOtherSelected", "" );
#endif

   zSHORT   nPos;
   TZPainterCtrl *pCurrCtrl;
   CRect    rectPainter;
   CRect    rect;

//???if ( m_hDC == 0 )
//   {
//      TraceLineS( "<tzctlpts> DragOtherSelected unexpected null hDC", "" );
//      m_hDC = new CClientDC( this );
//   }

   pCtrl->ParentToPainterRect( rectPainter );
   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCurrCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCurrCtrl != pCtrl &&
           pCurrCtrl->IsSelected( ) &&
           pCurrCtrl->m_pCtrlParent == pCtrl->m_pCtrlParent )
      {
         CPoint ptOld;
         CPoint ptNew;
         CPoint ptOffset;
         CPoint ptWork;
//       int    nOldROP2;

         // Get the difference between the mouse select position and
         // the center of our control.
         pCurrCtrl->ParentToPainterRect( rect );
         ptOffset.x = rectPainter.left - rect.left;
         ptOffset.y = rectPainter.top  - rect.top;

#if 0 // this needs work //???
#if 1
         if ( pCurrCtrl->m_pCtrlCover )
         {
            CRect rectNew;

            ptOld = pCtrl->m_ptMouseSelect;
            ptNew = pt;

            // Calculate the new rectangle
            rectNew.left = ptNew.x - (ptOld.x - rectPainter.left);
            rectNew.left -= ptOffset.x;
            rectNew.top = ptNew.y - (ptOld.y - rectPainter.top);
            rectNew.top -= ptOffset.y;
            rectNew.right = rectNew.left +
                          (rect.right - rect.left);
            rectNew.bottom = rectNew.top +
                          (rect.bottom - rect.top);

//          rectNew.top -= TZCtrlCover::m_nFrameSizeY;
//          rectNew.left -= TZCtrlCover::m_nFrameSizeX;
//          rectNew.bottom += TZCtrlCover::m_nFrameSizeY;
//          rectNew.right += TZCtrlCover::m_nFrameSizeX;

            if ( pt == pCtrl->m_ptMouseSelect )
               SetWindowPos( &CWnd::wndTop,
                             rectNew.left, rectNew.top,
                             rectNew.right - rectNew.left,
                             rectNew.bottom - rectNew.top,
                             SWP_HIDEWINDOW );
            else
            if ( pt != pCtrl->m_ptLastMouseMove )
               SetWindowPos( &CWnd::wndTop,
                             rectNew.left, rectNew.top,
                             rectNew.right - rectNew.left,
                             rectNew.bottom - rectNew.top,
                             SWP_SHOWWINDOW );
         }
#else
//       nOldROP2 = m_hDC->SetROP2( R2_NOT );
         if ( pCtrl->m_ptLastMouseMove != pCtrl->m_ptMouseSelect )
         {
            // Erase the last rectangle drawn to represent where a
            // control would have been positioned on the last move.
            ptOld = pCtrl->m_ptMouseSelect;
            ptNew = pCtrl->m_ptLastMouseMove;

            // Calculate and erase the previous rectangle.
            ptWork.x = ptNew.x - (ptOld.x - rectPainter.left );
            ptWork.x -= ptOffset.x;
            ptWork.y = ptNew.y - (ptOld.y - rectPainter.top );
            ptWork.y -= ptOffset.y;

            // Go to upper left of rectangle.
            m_hDC->MoveTo( ptWork.x, ptWork.y );
            ptWork.x += (rect.right - rect.left);
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y += (rect.bottom - rect.top);
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.x -= (rect.right - rect.left);
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y -= (rect.bottom - rect.top);
            m_hDC->LineTo( ptWork.x, ptWork.y );
         }

         if ( pt != pCtrl->m_ptMouseSelect )
         {
            // Draw a rectangle to represent where the control
            // will be positioned if the user releases the mouse
            // button here.
            ptOld = pCtrl->m_ptMouseSelect;
            ptNew = pt;

            // Calculate and draw the rectangle.
            ptWork.x = ptNew.x - (ptOld.x - rectPainter.left);
            ptWork.x -= ptOffset.x;
            ptWork.y = ptNew.y - (ptOld.y - rectPainter.top);
            ptWork.y -= ptOffset.y;

            // Go to upper left of rectangle
            m_hDC->MoveTo( ptWork.x, ptWork.y );
            ptWork.x += (rect.right - rect.left);
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y += (rect.bottom - rect.top);
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.x -= (rect.right - rect.left);
            m_hDC->LineTo( ptWork.x, ptWork.y );
            ptWork.y -= (rect.bottom - rect.top);
            m_hDC->LineTo( ptWork.x, ptWork.y );
         }

//       m_hDC->SetROP2( nOldROP2 );
#endif
#endif
      }
   }
//
// mDeleteInit( m_hDC );
}

// Move Other Selected, move the other selected controls.
void
TZPainterClient::MoveOtherSelected( TZPainterCtrl *pCtrlSelect,
                                    CPoint        ptOffset,
                                    zSHORT        nIdNbr )
{
   // Skip if fewer than two are selected.
   if ( m_pPainterWindow->m_nSelectCnt < 2 )
      return;

#ifdef DEBUG_ALL
   TracePoint( "TZPainterClient::MoveOtherSelected ptOffset", ptOffset );
#endif

   TZPainterCtrl *pNewParent;
   TZPainterCtrl *pCtrl;
   zCHAR   szLabel[ 64 ];
   CRect   rectNew;
   zLONG   lType;
   zLONG   lNewType;
   zSHORT  nPos;
   zBOOL   bCtrlKey = (GetKeyState( VK_CONTROL ) < 0);
   zBOOL   bShiftKey = (GetKeyState( VK_SHIFT ) < 0);

   CPoint  ptScrollPos = GetDeviceScrollPosition( );
   ScrollToPosition( CPoint( 0, 0 ) );
   m_nRecurse += 2;  // prevent AdjustGroupSets until all have moved
   m_nMultiMoveSize += 2;  // prevent status bar messages

   if ( bCtrlKey && nIdNbr )
   {
      sprintf_s( szLabel, zsizeof( szLabel ), "&xxdo Clone %d item%s\tCtrl+x",
                m_pPainterWindow->m_nSelectCnt,
                m_pPainterWindow->m_nSelectCnt == 1 ? "" : "s" );
   }

   // The CtrlCnt needs to be preserved, otherwise when cloning, we tend to
   // go into an infinite loop creating lots of ctrls.
   int nItemCnt = m_pPainterWindow->m_nCtrlCnt;
   for ( nPos = 0; nPos < nItemCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl != pCtrlSelect &&
           pCtrl->IsSelected( ) &&
           pCtrl->m_pCtrlParent == pCtrlSelect->m_pCtrlParent )
      {
         // Get the difference between the mouse select position and
         // the center of our control.
         pCtrl->ParentToPainterRect( rectNew );

         rectNew.OffsetRect( ptOffset );

         lNewType = lType = pCtrl->m_lType;
         pNewParent = pCtrl->ValidateParentObject( rectNew, lNewType,
                           bCtrlKey ? zCTRL_VALID_PAINT : zCTRL_VALID_MOVE );

         if ( lNewType == 0 || lNewType != lType )
         {
            ::MessageBeep( MB_ICONQUESTION );
         }
         else
         {
            // If the Ctrl Key is down, Clone the ctrls rather than
            // moving them.
            if ( bCtrlKey )
            {
            // TraceRect( "Clone1", rectNew );

               // CLONE the ctrl!  If the clone is to be within a new
               // parent, calculate the rect within the parent.
               if ( pNewParent )
               {
                  TZPainterCtrl *pWork;
                  pWork = pNewParent;
                  while ( pWork )
                     pWork = pWork->AdjustCtrlRect( rectNew, FALSE );
               }

            // TraceRect( "Clone2", rectNew );
               pCtrl->Clone( pNewParent, rectNew, nIdNbr, szLabel );
            }
            else
            {
               pCtrl->Deselect( );
               pCtrl->m_pCtrlCover->MoveCtrl( rectNew, pNewParent, nIdNbr );
               pCtrl->Select( );
            }
         }
      }
   }

   ScrollToPosition( ptScrollPos  );
   m_nRecurse -= 2;  // permit AdjustGroupSets
   AdjustGroupSets( );
   m_nMultiMoveSize -= 2;  // permit status bar messages
}
