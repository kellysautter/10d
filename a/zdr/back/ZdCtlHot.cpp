// HyperLink.cpp : implementation file
//
// HyperLink static control.
//
// Copyright (C) 1997, 1998 Giancarlo Iovino (giancarlo@saria.com)
// All rights reserved. May not be sold for profit.
//
// This code is based on CHyperlink by Chris Maunder.
// "GotoURL" function by Stuart Patterson appeared in the Aug, 1997
// Windows Developer's Journal.
// "Default hand cursor" from Paul DiLascia's Jan 1998 MSJ article.

/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctllbh.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon ToolBar control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlHot.h"
#include "ZdCtlGbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

#define TOOLTIP_ID 1

#define SETBITS( dw, bits )   (dw |= bits)
#define CLEARBITS( dw, bits ) (dw &= ~(bits))
#define BITSET( dw, bit )     (((dw) & (bit)) != 0L)

/////////////////////////////////////////////////////////////////////////////
// CHyperLink
//
const DWORD CHyperLink::StyleUnderline     = 0x00000001;    // Underline bit
const DWORD CHyperLink::StyleUseHover      = 0x00000002;    // Hand over coloring bit
const DWORD CHyperLink::StyleAutoSize      = 0x00000004;    // Auto size bit
const DWORD CHyperLink::StyleDownClick     = 0x00000008;    // Down click mode bit
const DWORD CHyperLink::StyleGetFocusOnClick = 0x00000010;  // Get focus on click bit
const DWORD CHyperLink::StyleNoHandCursor  = 0x00000020;    // No hand cursor bit
const DWORD CHyperLink::StyleNoActiveColor = 0x00000040;    // No active color bit

COLORREF CHyperLink::g_crLinkColor     = RGB( 0, 0, 255 );   // Blue
COLORREF CHyperLink::g_crActiveColor   = RGB( 0, 128, 128 ); // Dark cyan
COLORREF CHyperLink::g_crVisitedColor  = RGB( 128, 0, 128 ); // Purple
COLORREF CHyperLink::g_crHoverColor    = RGB( 255, 0, 0 );   // Red
HCURSOR  CHyperLink::g_hLinkCursor     = 0;                  // No cursor

IMPLEMENT_DYNAMIC( CHyperLink, CStatic )

BEGIN_MESSAGE_MAP( CHyperLink, CStatic )
   //{{AFX_MSG_MAP( CHyperLink )
   ON_WM_CTLCOLOR_REFLECT( )
   ON_WM_SETCURSOR( )
   ON_WM_MOUSEMOVE( )
   ON_WM_LBUTTONUP( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_KEYDOWN( )
   ON_WM_NCHITTEST( )
   ON_WM_LBUTTONDOWN( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CHyperLink::CHyperLink( )
{
   m_bOverControl = FALSE; // Cursor not yet over control
   m_bVisited     = FALSE; // Link has not been visited yet
   m_bLinkActive  = FALSE; // Control doesn't own the focus yet
   m_csURL.Empty( );       // Set URL to an empty string

   // Set default styles.
   m_dwStyle = StyleUnderline | StyleAutoSize | StyleGetFocusOnClick;
}

CHyperLink::~CHyperLink( )
{
   m_Font.DeleteObject( );
}

BOOL
CHyperLink::SubclassDlgItem( UINT uID, CWnd *pParent, zCPCHAR cpcURL )
{
   m_csURL = cpcURL;
   return( CStatic::SubclassDlgItem( uID, pParent ) );
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink message handlers

BOOL
CHyperLink::PreTranslateMessage( MSG *pMsg )
{
   m_ToolTip.RelayEvent( pMsg );
   return( CStatic::PreTranslateMessage( pMsg ) );
}

void
CHyperLink::PreSubclassWindow( )
{
   // If the URL string is empty try to set it to the window text.
   if ( m_csURL.IsEmpty( ) )
      GetWindowText( m_csURL );

   // Check that the window text isn't empty.  If so, set it as URL string.
   CString csWndText;
   GetWindowText( csWndText );
   if ( csWndText.IsEmpty( ) )
   {
      // Set the URL string as the window text
      ASSERT( m_csURL.IsEmpty( ) == FALSE ); // window text and URL both empty!
      CStatic::SetWindowText( m_csURL );
   }

   // Get the current window font
   CFont *pFont = GetFont( );

   if ( pFont )
   {
      LOGFONT lf;
      pFont->GetLogFont( &lf );
      lf.lfUnderline = BITSET( m_dwStyle, StyleUnderline );
      if ( m_Font.CreateFontIndirect( &lf ) )
         CStatic::SetFont( &m_Font );

      // Adjust window size to fit URL if necessary.
      AdjustWindow( );
   }
   else
   {
      // If GetFont returns 0 then the static control is probably not of a
      // text type: it's better to set auto-resizing off.
      CLEARBITS( m_dwStyle, StyleAutoSize );
   }

   if ( !BITSET( m_dwStyle, StyleNoHandCursor ) )
      SetDefaultCursor( );      // try to load a "hand" cursor

    // Create the tooltip.
    CRect rect;
    GetClientRect( rect );
    m_ToolTip.Create( this );
    m_ToolTip.AddTool( this, m_csURL, rect, TOOLTIP_ID );

    CStatic::PreSubclassWindow( );
}

// Handler for WM_CTLCOLOR reflected message (see message map).
HBRUSH
CHyperLink::CtlColor( CDC *pDC, UINT uCtlColor )
{
   ASSERT( uCtlColor == CTLCOLOR_STATIC );

   if ( m_bOverControl && BITSET( m_dwStyle, StyleUseHover ) )
      pDC->SetTextColor( g_crHoverColor );
   else
   if ( !BITSET( m_dwStyle, StyleNoActiveColor ) && m_bLinkActive )
      pDC->SetTextColor( g_crActiveColor );
   else
   if ( m_bVisited )
      pDC->SetTextColor( g_crVisitedColor );
   else
      pDC->SetTextColor( g_crLinkColor );

   // Set transparent drawing mode.
   pDC->SetBkMode( TRANSPARENT );
   return( (HBRUSH) GetStockObject( NULL_BRUSH ) );
}

void
CHyperLink::OnMouseMove( UINT uFlags, CPoint pt )
{
   if ( m_bOverControl )        // cursor currently over control
   {
      CRect rect;
      GetClientRect( rect );

      if ( rect.PtInRect( pt ) == FALSE )
      {
         m_bOverControl = FALSE;
         ReleaseCapture( );
         Invalidate( );
         return;
      }
   }
   else                      // cursor has left control area
   {
      m_bOverControl = TRUE;
      Invalidate( );
      SetCapture( );
   }
}

/////////////////////////////////////////////////////////////////////////////
// "Normally, a static control does not get mouse events unless it has
// SS_NOTIFY. This achieves the same effect as SS_NOTIFY, but it's fewer
// lines of code and more reliable than turning on SS_NOTIFY in OnCtlColor
// because Windows doesn't send WM_CTLCOLOR to bitmap static controls."
// (Paul DiLascia)
LRESULT
CHyperLink::OnNcHitTest( CPoint /*pt*/ )
{
   return( HTCLIENT );
}

void
CHyperLink::OnLButtonDown( UINT /*uFlags*/, CPoint /*pt*/ )
{
   if ( BITSET( m_dwStyle, StyleGetFocusOnClick ) )
      SetFocus( );          // set the focus and make the link active

   if ( BITSET( m_dwStyle, StyleDownClick ) )
      FollowLink( );

   m_bLinkActive = TRUE;
}

void
CHyperLink::OnLButtonUp( UINT uFlags, CPoint pt )
{
   if ( m_bLinkActive && !BITSET( m_dwStyle, StyleDownClick ) )
      FollowLink( );
}

BOOL
CHyperLink::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage )
{
   if ( g_hLinkCursor )
   {
      ::SetCursor( g_hLinkCursor );
      return( TRUE );
   }

   return( FALSE );
}

void
CHyperLink::OnSetFocus( CWnd *pOldWnd )
{
   m_bLinkActive = TRUE;
   Invalidate( );                    // repaint to set the focus
}

void
CHyperLink::OnKillFocus( CWnd *pNewWnd )
{
   // Assume that control lost focus = mouse out
   // this avoid troubles with the Hover color.
   m_bOverControl = FALSE;
   m_bLinkActive = FALSE;
   Invalidate( );                    // repaint to unset the focus
}

void
CHyperLink::OnKeyDown( UINT uChar, UINT uRepeatCnt, UINT uFlags )
{
   if ( uChar == VK_SPACE )
      FollowLink( );
   else
      CStatic::OnKeyDown( uChar, uRepeatCnt, uFlags );
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink operations

void
CHyperLink::SetColors( COLORREF crLinkColor,
                       COLORREF crActiveColor,
                       COLORREF crVisitedColor,
                       COLORREF crHoverColor /* = -1 */ )
{
   g_crLinkColor    = crLinkColor;
   g_crActiveColor  = crActiveColor;
   g_crVisitedColor = crVisitedColor;

   if ( crHoverColor == -1 )
      g_crHoverColor = ::GetSysColor( COLOR_HIGHLIGHT );
   else
      g_crHoverColor = crHoverColor;
}

void
CHyperLink::SetColors( HYPERLINKCOLORS& linkColors )
{
   g_crLinkColor    = linkColors.crLink;
   g_crActiveColor  = linkColors.crActive;
   g_crVisitedColor = linkColors.crVisited;
   g_crHoverColor   = linkColors.crHover;
}

void
CHyperLink::GetColors( HYPERLINKCOLORS& linkColors )
{
   linkColors.crLink = g_crLinkColor;
   linkColors.crActive = g_crActiveColor;
   linkColors.crVisited = g_crVisitedColor;
   linkColors.crHover = g_crHoverColor;
}

void
CHyperLink::SetLinkCursor( HCURSOR hCursor )
{
   ASSERT( hCursor );

   g_hLinkCursor = hCursor;
   if ( g_hLinkCursor == 0 )
      SetDefaultCursor( );
}

HCURSOR
CHyperLink::GetLinkCursor( )
{
   return( g_hLinkCursor );
}

BOOL
CHyperLink:: ModifyLinkStyle( DWORD dwRemove, DWORD dwAdd,
                             BOOL bApply /* =TRUE */ )
{
   // Check if we are adding and removing the same style.
   if ( (dwRemove & dwAdd) != 0 )
      return( FALSE );

   // Remove old styles and set the new ones
   CLEARBITS( m_dwStyle, dwRemove );
   SETBITS( m_dwStyle, dwAdd );

   if ( bApply && mIs_hWnd( GetSafeHwnd( ) ) )
   {
      // If possible, APPLY the new styles on the fly.
      if ( BITSET( dwAdd, StyleUnderline ) ||
           BITSET( dwRemove, StyleUnderline ) )
      {
         SwitchUnderline( );
      }

      if ( BITSET( dwAdd, StyleAutoSize ) )
         AdjustWindow( );

      if ( BITSET( dwRemove, StyleUseHover ) )
         Invalidate( );
   }

   return( TRUE );
}

DWORD
CHyperLink::GetLinkStyle( ) const
{
   return( m_dwStyle );
}

void
CHyperLink::SetURL( CString csURL )
{
   m_csURL = csURL;

   if ( mIs_hWnd( GetSafeHwnd( ) ) )
   {
      ShowWindow( SW_HIDE );
      AdjustWindow( );
      m_ToolTip.UpdateTipText( csURL, this, TOOLTIP_ID );
      ShowWindow( SW_SHOW );
   }
}

CString
CHyperLink::GetURL( ) const
{
   return( m_csURL );
}

void
CHyperLink::SetWindowText( zCPCHAR cpcText )
{
   ASSERT( cpcText );

   if ( mIs_hWnd( GetSafeHwnd( ) ) )
   {
      // Set the window text and adjust its size while the window
      // is kept hidden in order to allow dynamic modification.
      ShowWindow( SW_HIDE );          // hide window

      // Call the base class SetWindowText( ).
      CStatic::SetWindowText( cpcText );

      // Resize the control if necessary.
      AdjustWindow( );
      ShowWindow( SW_SHOW );          // show window
   }
}

void
CHyperLink::SetFont( CFont *pFont )
{
   ASSERT( mIs_hWnd( GetSafeHwnd( ) ) );
   ASSERT( pFont );

   // Set the window font and adjust its size while the window
   // is kept hidden in order to allow dynamic modification.
   ShowWindow( SW_HIDE );          // hide window
   LOGFONT lf;

   // Create the new font
   pFont->GetLogFont( &lf );
   m_Font.DeleteObject( );
   m_Font.CreateFontIndirect( &lf );

   // Call the base class SetFont( )
   CStatic::SetFont( &m_Font );

   // Resize the control if necessary
   AdjustWindow( );
   ShowWindow( SW_SHOW );          // show window
}

// Function to set underline on/off.
void
CHyperLink::SwitchUnderline( )
{
   LOGFONT lf;
   CFont* pFont = GetFont( );
   if ( pFont )
   {
      pFont->GetLogFont( &lf );
      lf.lfUnderline = BITSET( m_dwStyle, StyleUnderline );
      m_Font.DeleteObject( );
      m_Font.CreateFontIndirect( &lf );
      SetFont( &m_Font );
   }
}

// Move and resize the window so that its client area has the same size
// as the hyperlink text. This prevents the hyperlink cursor being active
// when it is not over the text.
void
CHyperLink::AdjustWindow( )
{
   ASSERT( mIs_hWnd( GetSafeHwnd( ) ) );

   if ( !BITSET( m_dwStyle, StyleAutoSize ) )
        return;

    // Get the current window rect
    CRect rcWnd;
    GetWindowRect( rcWnd );

   // For a child CWnd object, window rect is relative to the
   // upper-left corner of the parent window’s client area.
    CWnd *pParent = GetParent( );
    if ( pParent )
        pParent->ScreenToClient( rcWnd );

   // Get the current client rect
   CRect rcClient;
   GetClientRect( rcClient );

   // Calc border size based on window and client rects.
   int borderWidth = rcWnd.Width( ) - rcClient.Width( );
   int borderHeight = rcWnd.Height( ) - rcClient.Height( );

   // Get the extent of window text
   CString csWndText;
   GetWindowText( csWndText );

   CDC *pDC = GetDC( );
   CFont *pOldFont = pDC->SelectObject( &m_Font );
   CSize Extent = pDC->GetTextExtent( csWndText );
   pDC->SelectObject( pOldFont );
   ReleaseDC( pDC );

   // Get the text justification style.
   DWORD dwStyle = GetStyle( );

   // Recalc window size and position based on text justification.
   if ( BITSET( dwStyle, SS_CENTERIMAGE ) )
      rcWnd.DeflateRect( 0, (rcWnd.Height( ) - Extent.cy) / 2 );
   else
      rcWnd.bottom = rcWnd.top + Extent.cy;

   if ( BITSET( dwStyle, SS_CENTER ) )
      rcWnd.DeflateRect( (rcWnd.Width( ) - Extent.cx) / 2, 0 );
   else
   if ( BITSET( dwStyle, SS_RIGHT ) )
      rcWnd.left  = rcWnd.right - Extent.cx;
   else // SS_LEFT
      rcWnd.right = rcWnd.left + Extent.cx;

   // Move and resize the window.
   MoveWindow( rcWnd.left, rcWnd.top, rcWnd.Width( ) + borderWidth,
               rcWnd.Height( ) + borderHeight );
}

void
CHyperLink::SetVisited( BOOL bVisited /* = TRUE */ )
{
   m_bVisited = bVisited;
}

BOOL
CHyperLink::IsVisited( ) const
{
   return( m_bVisited );
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink implementation

// The following function appeared in Paul DiLascia's Jan 1998
// MSJ articles. It loads a "hand" cursor from "winhlp32.exe"
// resources.
void
CHyperLink::SetDefaultCursor( )
{
   if ( g_hLinkCursor == 0 )    // No cursor handle - load our own
   {
        // Get the windows directory
      CString csWndDir;
      GetWindowsDirectory( csWndDir.GetBuffer( MAX_PATH ), MAX_PATH );
      csWndDir.ReleaseBuffer( );
      csWndDir += _T( "\\winhlp32.exe" );

      // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer.
      HMODULE hModule = LoadLibrary( csWndDir );
      if ( hModule )
      {
         HCURSOR hHandCursor = ::LoadCursor( hModule, MAKEINTRESOURCE( 106 ) );
         if ( hHandCursor )
            g_hLinkCursor = CopyCursor( hHandCursor );
      }

      FreeLibrary( hModule );
   }
}

LONG
CHyperLink::GetRegKey( HKEY hKey, zCPCHAR cpcSubkey, zPCHAR pchReturnData )
{
   zLONG lRC = RegOpenKeyEx( hKey, cpcSubkey, 0, KEY_QUERY_VALUE, &hKey );

   if ( lRC == ERROR_SUCCESS )
   {
      zLONG lDataSize = MAX_PATH;
      zCHAR szData[ MAX_PATH ];
      RegQueryValue( hKey, 0, szData, &lDataSize );
      strcpy_s( pchReturnData, MAX_PATH, szData );
      RegCloseKey( hKey );
   }

   return( lRC );
}

// Error report function.
void
CHyperLink::ReportError( int nError )
{
   CString cs;

   switch ( nError )
   {
        case 0:
           cs = _T( "The operating system is out\nof memory or resources." );
           break;

        case ERROR_FILE_NOT_FOUND:
           cs = _T( "The specified file was not found." );
           break;

        case ERROR_PATH_NOT_FOUND:
           cs = _T( "The specified path was not found." );
           break;

        case ERROR_BAD_FORMAT:
           cs = _T( "The .EXE file is invalid\n(non-Win32 .EXE or error in .EXE image)." );
           break;

        case SE_ERR_ACCESSDENIED:
           cs = _T( "The operating system denied\naccess to the specified file." );
           break;

        case SE_ERR_ASSOCINCOMPLETE:
           cs = _T( "The filename association is\nincomplete or invalid." );
           break;

        case SE_ERR_DDEBUSY:
           cs = _T( "The DDE transaction could not\nbe completed because other DDE transactions\nwere being processed." );
           break;

        case SE_ERR_DDEFAIL:
           cs = _T( "The DDE transaction failed." );
           break;

        case SE_ERR_DDETIMEOUT:
           cs = _T( "The DDE transaction could not\nbe completed because the request timed out." );
           break;

        case SE_ERR_DLLNOTFOUND:
           cs = _T( "The specified dynamic-link library was not found." );
           break;

     // case SE_ERR_FNF:
     //    cs = _T( "Windows 95 only: The specified file was not found." );
     //    break;

        case SE_ERR_NOASSOC:
           cs = _T( "There is no application associated\nwith the given filename extension." );
           break;

        case SE_ERR_OOM:
           cs = _T( "There was not enough memory to complete the operation." );
           break;

     // case SE_ERR_PNF:
     //    cs = _T( "The specified path was not found." );
     //    break;

        case SE_ERR_SHARE:
           cs = _T( "A sharing violation occurred. " );
           break;
        default:
           cs.Format( _T( "Unknown Error (%d) occurred." ), nError );
           break;
   }

   cs = "Can't open link:\n\n" + cs;
   AfxMessageBox( cs, MB_ICONEXCLAMATION | MB_OK );
}

// "GotoURL" function by Stuart Patterson
// As seen in the August, 1997 Windows Developer's Journal.
HINSTANCE
CHyperLink::GotoURL( zCPCHAR cpcURL, int nShowCmd )
{
   TCHAR pchKey[ MAX_PATH + MAX_PATH ];

   // First try ShellExecute( ).
   HINSTANCE hInst = ShellExecute( 0, _T( "open" ), cpcURL, 0, 0, nShowCmd );

   // If it failed, get the .htm regkey and lookup the program.
   if ( (UINT) hInst <= HINSTANCE_ERROR )
   {
      if ( GetRegKey( HKEY_CLASSES_ROOT, _T( ".htm" ), pchKey ) == ERROR_SUCCESS )
      {
         strcat_s( pchKey, _T( "\\shell\\open\\command" ) );

         if ( GetRegKey( HKEY_CLASSES_ROOT, pchKey, pchKey ) == ERROR_SUCCESS )
         {
            TCHAR *pos;
            pos = _tcsstr( pchKey, _T( "\"%1\"" ) );
            if ( pos == 0 )
            {
               // No quotes found
               pos = zstrstr( pchKey, _T( "%1" ) ); // check for %1 without quotes
               if ( pos == 0 )                      // no parameter at all ...
                  pos = pchKey + zstrlen( pchKey ) - 1;
               else
                  *pos = 0;                  // remove the parameter
            }
            else
               *pos = 0;                     // remove the parameter

            strcat_s( pos, MAX_PATH + MAX_PATH - (pos - pchKey), _T( " " ) );
            strcat_s( pos, MAX_PATH + MAX_PATH - (pos - pchKey), cpcURL );
            hInst = (HINSTANCE) WinExec( pchKey, nShowCmd );
         }
      }
   }

   return( hInst );
}

// Activate the link.
void
CHyperLink::FollowLink( )
{
   int nResult = (int) GotoURL( m_csURL, SW_SHOW );
   if ( nResult <= HINSTANCE_ERROR )
   {
      MessageBeep( MB_ICONEXCLAMATION );  // unable to follow link
      ReportError( nResult );
   }
   else
   {
      // Mark link as visited and repaint window.
      m_bVisited = TRUE;
      Invalidate( );
   }
}
