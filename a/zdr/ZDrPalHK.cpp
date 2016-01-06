/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrpalhk.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of the Palette Message Handling class.
// The code is derived from code written by Paul DiLascia.

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
// To turn on tracing for palette messages, you need to define _TRACEPAL
// here or in your make file

// #define _TRACEPAL

#ifndef _TRACEPAL
#undef TRACEFN
#undef TRACE
#undef TRACE0
#define TRACEFN CString( ).Format
#define TRACE CString( ).Format
#define TRACE0 CString( ).Format
#endif

IMPLEMENT_DYNAMIC( ZPalMsgHandler, ZMsgHook );

ZPalMsgHandler::ZPalMsgHandler( )
{
   m_pPalette = 0;
}

ZPalMsgHandler::~ZPalMsgHandler( )
{
}

/////////////////////////////////////////////////////////////////////////////
// Message handler handles palette-related messages
//
LRESULT
ZPalMsgHandler::WindowProc( UINT msg, WPARAM wp, LPARAM lp )
{
   ASSERT_VALID( m_pWndHooked );

   switch ( msg )
   {
      case WM_PALETTECHANGED:
         OnPaletteChanged( CWnd::FromHandle( (HWND) wp ) );
         return( 0 );

      case WM_QUERYNEWPALETTE:
         return( OnQueryNewPalette( ) );

      case WM_SETFOCUS:
         OnSetFocus( CWnd::FromHandle( (HWND) wp ) );
         return( 0 );
   }

   return( ZMsgHook::WindowProc( msg, wp, lp ) );
}

/////////////////////////////////////////////////////////////////////////////
// Handle WM_PALETTECHANGED
//
void
ZPalMsgHandler::OnPaletteChanged( CWnd *pFocusWnd )
{
   ASSERT( m_pWndHooked );
   CWnd& wnd = *m_pWndHooked;
   TRACEFN( "ZPalMsgHandler::OnPaletteChanged for %s [from %s]\n",
            DbgName( &wnd ), DbgName( pFocusWnd ) );

   if ( pFocusWnd->GetSafeHwnd( ) != wnd.m_hWnd )
   {
      if ( DoRealizePalette( FALSE )==0 )
      {
         if ( wnd.GetParent( ) == 0 )
         {
            // I'm the top-level frame: Broadcast to children
            // (only MFC permanent CWnd's!)
            //
            const MSG& curMsg = AfxGetThreadState( )->m_lastSentMsg;
            wnd.SendMessageToDescendants( WM_PALETTECHANGED,
                                          curMsg.wParam, curMsg.lParam );
         }
      }
   }
   else
   {
      // I'm the window that triggered the WM_PALETTECHANGED
      // in the first place: ignore it
      //
      TraceLineS( "[It's me, don't realize palette.]", "" );
   }
}

/////////////////////////////////////////////////////////////////////////////
// Handle WM_QUERYNEWPALETTE
//
zBOOL
ZPalMsgHandler::OnQueryNewPalette( )
{
   ASSERT( m_pWndHooked );
   CWnd& wnd = *m_pWndHooked;
   TRACEFN( "ZPalMsgHandler::OnQueryNewPalette for %s\n", DbgName( &wnd ) );

   if ( DoRealizePalette( TRUE ) == 0 )
   {  // realize in foreground
      // No colors changed: if this is the top-level frame,
      // give active view a chance to realize itself
      //
      if ( wnd.GetParent( ) == 0 )
      {
         ASSERT_KINDOF( CFrameWnd, &wnd );
         CWnd *pView = ((CFrameWnd &) wnd).GetActiveFrame( )->GetActiveView( );
         if ( pView )
            pView->SendMessage( WM_QUERYNEWPALETTE );
      }
   }

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// Handle WM_SETFOCUS
//
void
ZPalMsgHandler::OnSetFocus( CWnd *pOldWnd )
{
   ASSERT( m_pWndHooked );
   CWnd& wnd = *m_pWndHooked;
   TRACEFN( "ZPalMsgHandler::OnSetFocus for %s\n", DbgName( &wnd ) );
   wnd.SetForegroundWindow( );      // Windows likes this
   DoRealizePalette( TRUE );        // realize in foreground
   Default( );                   // let app handle focus message too
}

/////////////////////////////////////////////////////////////////////////////
// Function to actually realize the palette.
// Override this to do different kind of palette realization; e.g.,
// DrawDib instead of setting the CPalette.
//
int
ZPalMsgHandler::DoRealizePalette( zBOOL bForeground )
{
   if ( !m_pPalette || !m_pPalette->m_hObject )
      return( 0 );

   ASSERT( m_pWndHooked );
   CWnd& wnd = *m_pWndHooked;
   TRACEFN( "ZPalMsgHandler::DoRealizePalette( %s ) for %s\n",
            bForeground ? "foreground" : "background", DbgName( &wnd ) );

   CClientDC dc( &wnd );
   CPalette *pOldPal = dc.SelectPalette( m_pPalette, !bForeground );
   int nColorsChanged = dc.RealizePalette( );
   if ( pOldPal )
      dc.SelectPalette( pOldPal, TRUE );

   if ( nColorsChanged > 0 )
      wnd.Invalidate( FALSE ); // repaint

// TRACE( "[%d colors changed]\n", nColorsChanged );
   return( nColorsChanged );
}

