/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrdib.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// ZCoolBar implements coolbars for MFC.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
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
IMPLEMENT_DYNAMIC( ZCoolBar, CControlBar )

BEGIN_MESSAGE_MAP( ZCoolBar, CControlBar )
   //{{AFX_MSG_MAP( ZCoolBar )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
   ON_WM_ERASEBKGND( )
   ON_NOTIFY_REFLECT( RBN_HEIGHTCHANGE, OnHeightChange )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZCoolBar::ZCoolBar( ZSubtask *pZSubtask )
{
   if ( pZSubtask->m_hInstDLL )
   {
   // HINSTANCE hInst = AfxGetResourceHandle( );
   // AfxSetResourceHandle( pZSubtask->m_hInstDLL );

   // if ( LoadToolBar( 4104 ) == 0 )
      if ( LoadBitmap( pZSubtask->m_hInstDLL, MAKEINTRESOURCE( 4104 ) ) == 0 )
      {
         DestroyWindow( );
         TraceLineS( "Failed to load CoolBar bitmap:", "" );
      }

   // AfxSetResourceHandle( hInst );
   }
}

ZCoolBar::~ZCoolBar( )
{
}

/////////////////////////////////////////////////////////////////////////////
// Create coolbar
//
BOOL
ZCoolBar::Create( CWnd *pParentWnd, DWORD dwStyle,
                  DWORD dwAfxBarStyle, UINT nID )
{
   ASSERT_VALID( pParentWnd );   // must have a parent

   // Dynamic coolbar not supported.
   dwStyle &= ~CBRS_SIZE_DYNAMIC;

   // Save the style (this code copied from MFC--probably unecessary).
   m_dwStyle = dwAfxBarStyle;
   if ( nID == AFX_IDW_TOOLBAR )
      m_dwStyle |= CBRS_HIDE_INPLACE;

   // MFC requires these:
   dwStyle |= CCS_NODIVIDER|CCS_NOPARENTALIGN;

   // Initialize cool common controls.
   static BOOL bInit = FALSE;
   if ( bInit == FALSE )
   {
      INITCOMMONCONTROLSEX iccx;
      iccx.dwSize = sizeof( INITCOMMONCONTROLSEX );
      iccx.dwICC = ICC_COOL_CLASSES;
      InitCommonControlsEx( &iccx );
      bInit = TRUE;
   }

   // Create the cool bar using style and parent.
   CRect rc;
   rc.SetRectEmpty( );
   return( CWnd::CreateEx( WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL,
                           dwStyle, rc, pParentWnd, nID ) );
}

/////////////////////////////////////////////////////////////////////////////
// Handle WM_CREATE. Call virtual fn so derived class can create bands.
//
int
ZCoolBar::OnCreate( LPCREATESTRUCT lpcs )
{
   return( (CControlBar::OnCreate( lpcs ) == -1) ? -1 :
               OnCreateBands( ) );   // call pure virtual fn to create bands
}

/////////////////////////////////////////////////////////////////////////////
// Standard UI handler updates any controls in the coolbar.
//
void
ZCoolBar::OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler )
{
   UpdateDialogControls( pTarget, bDisableIfNoHndler );
}

/////////////////////////////////////////////////////////////////////////////
// These two functions are called by MFC to calculate the layout of the
// main frame.  Since ZCoolBar is not designed to be dynamic, the size
// is always fixed, and the same as the window size.
//
CSize
ZCoolBar::CalcDynamicLayout( int nLength, DWORD dwMode )
{
   return( CalcFixedLayout( dwMode & LM_STRETCH, dwMode & LM_HORZ ) );
}

CSize
ZCoolBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
{
   CRect rc;
   GetWindowRect( &rc );
   CSize sz( bHorz && bStretch ? 0x7FFF : rc.Width( ),
             !bHorz && bStretch ? 0x7FFF : rc.Height( ) );
   return( sz );
}

/////////////////////////////////////////////////////////////////////////////
// Low-level height-changed handler just passes to virtual fn w/nicer args.
//
void
ZCoolBar::OnHeightChange( NMHDR *pNMHDR, LRESULT *pRes )
{
   CRect rc;
   GetWindowRect( &rc );
   OnHeightChange( rc );
   *pRes = 0; // why not?
}

/////////////////////////////////////////////////////////////////////////////
// Height changed:
// Notify the parent frame by posting a WM_SIZE message. This will cause the
// frame to do RecalcLayout.  The message must be posted, not sent, because
// the coolbar could send RBN_HEIGHTCHANGE while the user is sizing, which
// would be in the middle of a CFrame::RecalcLayout, and RecalcLayout doesn't
// let you re-enter it.  Posting gurantees that CFrameWnd can finish any
// recalc it may be in the middle of before handling my posted WM_SIZE.
// Very confusing.
//
void
ZCoolBar::OnHeightChange( const CRect& rcNew )
{
   CWnd *pParent = GetParent( );
   CRect rc;
   pParent->GetWindowRect( &rc );
   pParent->PostMessage( WM_SIZE, 0, MAKELONG( rc.Width( ),rc.Height( ) ) );
}

void
ZCoolBar::OnPaint( )
{
   Default( ); // bypass CControlBar
}

BOOL
ZCoolBar::OnEraseBkgnd( CDC *pDC )
{
   return( (BOOL) Default( ) );  // bypass CControlBar
}

BOOL
ZCoolBar::GetBarInfo( LPREBARINFO lp )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (BOOL) SendMessage( RB_GETBARINFO, 0, (LPARAM) lp ) );
}

BOOL
ZCoolBar::SetBarInfo( LPREBARINFO lp )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (BOOL) SendMessage( RB_SETBARINFO, 0, (LPARAM) lp ) );
}

BOOL
ZCoolBar::GetBandInfo( int iBand, LPREBARBANDINFO lp )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (BOOL) SendMessage( RB_GETBANDINFO, iBand, (LPARAM) lp ) );
}

BOOL
ZCoolBar::SetBandInfo( int iBand, LPREBARBANDINFO lp )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (BOOL) SendMessage( RB_SETBANDINFO, iBand, (LPARAM) lp ) );
}

BOOL
ZCoolBar::InsertBand( int iWhere, LPREBARBANDINFO lp )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (BOOL) SendMessage( RB_INSERTBAND, (WPARAM) iWhere,
                               (LPARAM) lp ) );
}

BOOL
ZCoolBar::DeleteBand( int nWhich )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (BOOL) SendMessage( RB_INSERTBAND, (WPARAM) nWhich ) );
}

int
ZCoolBar::GetBandCount( )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (int) SendMessage( RB_GETBANDCOUNT ) );
}

int
ZCoolBar::GetRowCount( )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (int) SendMessage( RB_GETROWCOUNT ) );
}

int
ZCoolBar::GetRowHeight( int nWhich )
{
   ASSERT( mIs_hWnd( m_hWnd ) );
   return( (int) SendMessage( RB_GETROWHEIGHT, (WPARAM) nWhich ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Special tool bar to use in cool bars.
// Mainly, it overides yukky stuff in CToolBar.
//
IMPLEMENT_DYNAMIC( ZCoolToolBar, CToolBar )

BEGIN_MESSAGE_MAP( ZCoolToolBar, CToolBar )
   ON_WM_NCCREATE( )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
   ON_WM_NCCALCSIZE( )
END_MESSAGE_MAP( )

ZCoolToolBar::ZCoolToolBar( )
{
}

ZCoolToolBar::~ZCoolToolBar( )
{
}

/////////////////////////////////////////////////////////////////////////////
// Make the parent frame my owner. This is important for status bar
// prompts to work. Note that when you create the ZCoolToolBar in
// CYourCoolBar::OnCreateBands, you must also set CBRS_FLYBY in the
// the ZCoolToolBar style!
//
BOOL
ZCoolToolBar::OnNcCreate( LPCREATESTRUCT lpcs )
{
   CFrameWnd *pFrame = GetParentFrame( );
   ASSERT_VALID( pFrame );
   SetOwner( pFrame );
   return( CToolBar::OnNcCreate( lpcs ) );
}

void
ZCoolToolBar::OnNcPaint( )
{
   Default( ); // bypass CToolBar/CControlBar
}

void
ZCoolToolBar::OnPaint( )
{
   Default( ); // bypass CToolBar/CControlBar
}

void
ZCoolToolBar::OnNcCalcSize( BOOL, NCCALCSIZE_PARAMS * )
{
   Default( ); // bypass CToolBar/CControlBar
}

/////////////////////////////////////////////////////////////////////////////
// This function is mostly copied from CToolBar/BARTOOL.CPP. The only thing
// that's different is the instantiation of a ZFlatOrCoolBarCmdUI instead of
// CToolCmdUI.
//
void
ZCoolToolBar::OnUpdateCmdUI( CFrameWnd *pTarget,
                             BOOL bDisableIfNoHndler )
{
   ZFlatOrCoolBarCmdUI state; // this is the only line that's different--PD
   state.m_pOther = this;

   state.m_nIndexMax = (UINT) DefWindowProc( TB_BUTTONCOUNT, 0, 0 );
   for ( state.m_nIndex = 0;
         state.m_nIndex < state.m_nIndexMax;
         state.m_nIndex++ )
   {
      // Get button state.
      TBBUTTON button;
      VERIFY( DefWindowProc( TB_GETBUTTON, state.m_nIndex, (LPARAM) &button ) );
      // TBSTATE_ENABLED == TBBS_DISABLED so invert it
      button.fsState ^= TBSTATE_ENABLED;

      state.m_nID = button.idCommand;

      // Ignore separators.
      if ( (button.fsStyle & TBSTYLE_SEP) == 0 )
      {
         // Allow the toolbar itself to have update handlers.
         if ( CWnd::OnCmdMsg( state.m_nID, CN_UPDATE_COMMAND_UI,
                              &state, NULL ) )
         {
            continue;
         }

         // Allow the owner to process the update.
         state.DoUpdate( pTarget, bDisableIfNoHndler );
      }
   }

   // Update the dialog controls added to the toolbar.
   UpdateDialogControls( pTarget, bDisableIfNoHndler );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZRebarInfo::ZRebarInfo( )
{
   zmemset( this, 0, sizeof( REBARINFO ) );
   cbSize = sizeof( REBARINFO );
}

ZRebarBandInfo::ZRebarBandInfo( )
{
   zmemset( this, 0, sizeof( REBARBANDINFO ) );
   cbSize = sizeof( REBARBANDINFO );
}
