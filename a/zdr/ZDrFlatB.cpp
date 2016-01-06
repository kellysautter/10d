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
// ZFlatToolBar implements flat tool bars for MFC.
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
IMPLEMENT_DYNAMIC( ZFlatToolBar, CToolBar )

BEGIN_MESSAGE_MAP( ZFlatToolBar, CToolBar )
   ON_WM_WINDOWPOSCHANGING( )
   ON_WM_WINDOWPOSCHANGED( )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// Load override modifies the style after loading toolbar.
//
BOOL
ZFlatToolBar::LoadToolBar( zCPCHAR lpszResourceName )
{
   if ( CToolBar::LoadToolBar( lpszResourceName ) == 0 )
      return( FALSE );

   ModifyStyle( 0, TBSTYLE_FLAT ); // make it flat
   return( TRUE );
}

BOOL
ZFlatToolBar::LoadToolBar( UINT nIDResource )
{
   return( LoadToolBar( MAKEINTRESOURCE( nIDResource ) ) );
}

// #define ILLUSTRATE_DISPLAY_BUG        // remove comment to see the bug

/////////////////////////////////////////////////////////////////////////////
// MFC doesn't handle moving a TBSTYLE_FLAT toolbar correctly.
// The simplest way to fix it is to repaint the old rectangle and
// toolbar itself whenever the toolbar moves.
//
void
ZFlatToolBar::OnWindowPosChanging( LPWINDOWPOS lpwp )
{
   CToolBar::OnWindowPosChanging( lpwp );

#ifndef ILLUSTRATE_DISPLAY_BUG

   if ( (lpwp->flags & SWP_NOMOVE) == 0 ) // if moved:
   {
      CRect rc;                            // Fill rectangle with..
      GetWindowRect( &rc );                // ..my (toolbar) rectangle.
      CWnd *pParent = GetParent( );        // get parent (dock bar/frame) win..
      pParent->ScreenToClient( &rc );      // .. and convert to parent coords

      // Ask parent window to paint the area beneath my old location.
      // Typically, this is just solid grey. The area won't get painted until
      // I send WM_NCPAINT after the move, in OnWindowPosChanged below.
      //
      pParent->InvalidateRect( &rc );      // paint old rectangle
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Now toolbar has moved: repaint old area.
//
void
ZFlatToolBar::OnWindowPosChanged( LPWINDOWPOS lpwp )
{
   CToolBar::OnWindowPosChanged( lpwp );

#ifndef ILLUSTRATE_DISPLAY_BUG
   if ( (lpwp->flags & SWP_NOMOVE) == 0 ) // if moved:
   {
      // Now paint my non-client area at the new location.
      // This is the extra bit of border space surrounding the buttons.
      // Without this, you will still have a partial display bug (try it!).
      //
      SendMessage( WM_NCPAINT );
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////
// The following stuff is to make the command update UI mechanism
// work properly for flat tool bars. The main idea is to convert
// a "checked" button state into a "pressed" button state. Changed
// lines marked with "PD"

void
ZFlatOrCoolBarCmdUI::Enable( BOOL bOn )
{
   m_bEnableChanged = TRUE;
   CToolBar *pToolBar = (CToolBar *) m_pOther;
   ASSERT( pToolBar != NULL );
   ASSERT_KINDOF( CToolBar, pToolBar );
   ASSERT( m_nIndex < m_nIndexMax );

   UINT nNewStyle = pToolBar->GetButtonStyle( m_nIndex ) & ~TBBS_DISABLED;
   if ( bOn == FALSE )
   {
      nNewStyle |= TBBS_DISABLED;

      // WINBUG: If a button is currently pressed and then is disabled
      // COMCTL32.DLL does not unpress the button, even after the mouse
      // button goes up!  We work around this bug by forcing TBBS_PRESSED
      // off when a button is disabled.
      nNewStyle &= ~TBBS_PRESSED;
   }

   ASSERT( (nNewStyle & TBBS_SEPARATOR) == 0 );
   pToolBar->SetButtonStyle( m_nIndex, nNewStyle );
}

// Take your pick:
// #define MYTBBS_CHECKED TBBS_CHECKED       // use "checked" state
#define MYTBBS_CHECKED TBBS_PRESSED       // use pressed state

/////////////////////////////////////////////////////////////////////////////
// This is the only function that has changed: instead of TBBS_CHECKED,
// I use TBBS_PRESSED--PD
//
void
ZFlatOrCoolBarCmdUI::SetCheck( int nCheck )
{
   ASSERT( nCheck >= 0 && nCheck <= 2 ); // 0=>off, 1=>on, 2=>indeterminate
   CToolBar *pToolBar = (CToolBar *) m_pOther;
   ASSERT( pToolBar != NULL );
   ASSERT_KINDOF( CToolBar, pToolBar );
   ASSERT( m_nIndex < m_nIndexMax );

   UINT nOldStyle = pToolBar->GetButtonStyle( m_nIndex ); // PD
   UINT nNewStyle = nOldStyle &
            ~(MYTBBS_CHECKED | TBBS_INDETERMINATE); // PD
   if ( nCheck == 1 )
      nNewStyle |= MYTBBS_CHECKED; // PD
   else
   if ( nCheck == 2 )
      nNewStyle |= TBBS_INDETERMINATE;

   // Following is to fix display bug for TBBS_CHECKED:
   // If new state is unchecked, repaint - but only if style actually
   // changing (otherwise will end up with flicker).
   //
   if ( nNewStyle != nOldStyle )
   {
      ASSERT( (nNewStyle & TBBS_SEPARATOR) == 0 );
      pToolBar->SetButtonStyle( m_nIndex, nNewStyle );
      pToolBar->Invalidate( );
   }
}

void
ZFlatOrCoolBarCmdUI::SetText( LPCTSTR )
{
   // Ignore for now, but really should set the text.
}

/////////////////////////////////////////////////////////////////////////////
// This function is mostly copied from CToolBar/BARTOOL.CPP. The only thing
// that's different is the instantiation of a ZFlatOrCoolBarCmdUI instead of
// CToolCmdUI.
//
void
ZFlatToolBar::OnUpdateCmdUI( CFrameWnd *pTarget,
                             BOOL bDisableIfNoHndler )
{
   ZFlatOrCoolBarCmdUI state; // This is the only line that's different--PD
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
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
