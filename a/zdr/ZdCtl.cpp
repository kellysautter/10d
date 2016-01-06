/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctl.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon DR Controls.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.04.26    TMV    RemoteServer
//   implement creation of VirtualWebGrid
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2000.11.18    BL
//   set define  zUSE_MS_TREE for new Tree Control
//
// 1999.05.11    DKS   Z10    ActiveX
//    New editor as ActiveX control.
//
// 1998.11.20    DKS
//    Added sort flag to Button Header ListBox type.
//
// 1998.10.30    DKS
//    Activated Button Header Listbox choice.
//
// 1998.08.13 - DKS
//    Pending update disabled until completed.
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "ZdCtl.h"

#define zGLOBAL_DATA
#include "ZdCtlGbl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Initialization of MFC Extension DLL

#include "afxdllx.h"    // standard MFC Extension DLL routines

static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };
static const int g_nIdShowToolTipEvt = 1;
static const int g_nIdShowTitleTipEvt = 2;

extern "C" int APIENTRY
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
   // Remove this if you use lpReserved
   UNREFERENCED_PARAMETER( lpReserved );

   if ( dwReason == DLL_PROCESS_ATTACH )
   {
   // TRACE0( "ZDCTL.DLL Initializing!\n" );
      g_hInstanceDLL = hInstance;

      // Extension DLL one-time initialization - do not allocate memory here,
      // use the TRACE or ASSERT macros or call MessageBox
      if ( AfxInitExtensionModule( extensionDLL, hInstance ) == 0 )
         return( 0 );

      // Other initialization could be done here, as long as
      // it doesn't result in direct or indirect calls to AfxGetApp.
      // This extension DLL doesn't need to access the app object
      // but to be consistent with ZDrApp.dll, this DLL requires
      // explicit initialization as well (see below).

      // This allows for greater flexibility later in development.

      ///////////////////////////////////////////////////////////////////////
      //
      // We are calling this DLL from regular DLL's, so we have moved
      // the call to CDynLinkLibrary .
      //
      // Insert this DLL into the resource chain
      // NOTE: If this Extension DLL is being implicitly linked to by
      // an MFC Regular DLL (such as an ActiveX Control) instead of an
      // MFC application, then you will want to remove this line from
      // DllMain and put it in a separate function exported from this
      // Extension DLL.  The Regular DLL that uses this Extension DLL
      // should then explicitly call that function to initialize this
      // Extension DLL.  Otherwise, the CDynLinkLibrary object will not
      // be attached to the Regular DLL's resource chain, and serious
      // problems will result.
      //
      ///////////////////////////////////////////////////////////////////////

      // new CDynLinkLibrary( extensionDLL );
   }
   else
   if ( dwReason == DLL_PROCESS_DETACH )
   {
   // TRACE0( "ZDCTL.DLL Terminating!\n" );
      AfxTermExtensionModule( extensionDLL );
   }

   return( 1 );   // ok
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
ZeidonZDCTL_WndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
//   switch ( uMessage )
//   {
//      case WM_LBUTTONDBLCLK:
// #ifdef DEBUG_ALL
//         TraceLineS( "ZeidonHD LBUTTONDBLCLK", "" );
// #endif
//         break;
//   }

   return( ::DefWindowProc( hWnd, uMessage, wParam, lParam ) );
}

//zBOOL
//RegisterSS( HINSTANCE  hInstance );

/////////////////////////////////////////////////////////////////////////////
// Exported DLL initialization is run in context of running application
//
extern "C" void WINAPI
ZeidonInitZDCTL( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                 zLONG lMapDlgCharX, zLONG lMapDlgCharY )
{
   // Create a new CDynLinkLibrary for this app.
// new CDynLinkLibrary( extensionDLL );

   // Initialize globals.
   g_lDlgUnitCharX = lDlgUnitCharX;
   g_lDlgUnitCharY = lDlgUnitCharY;
   g_lMapDlgCharX = lMapDlgCharX;
   g_lMapDlgCharY = lMapDlgCharY;
   g_lDblClkX = GetSystemMetrics( SM_CXDOUBLECLK );
   g_lDblClkY = GetSystemMetrics( SM_CYDOUBLECLK );

// TraceLineI( "DlgUnitCharX = ", g_lDlgUnitCharX );     // e.g.  10
// TraceLineI( "DlgUnitCharY = ", g_lDlgUnitCharY );     // e.g.  20

#if 0
   CWindowDC dc( CWnd::GetDesktopWindow( ) );
   CPoint pt( 1000, 1000 );   // a HIENGLISH point is .001 inches

   // Change the mapping mode to high english.
   int nOldMapMode = dc.SetMapMode( MM_HIENGLISH );

   // Convert the client rect sizes to high english.
   zLONG lMod;

   dc.LPtoDP( &pt );
   g_lPixelsPerInchX = (pt.x < 0) ? -pt.x : pt.x;
   g_lPixelsPerInchY = (pt.y < 0) ? -pt.y : pt.y;
   lMod = g_lPixelsPerInchX % 16;
   if ( lMod >= 8 )
   {
      lMod = 16 - lMod;
      g_lPixelsPerInchX += lMod;
   }
   else
      g_lPixelsPerInchX -= lMod;

   lMod = g_lPixelsPerInchY % 16;
   if ( lMod >= 8 )
   {
      lMod = 16 - lMod;
      g_lPixelsPerInchY += lMod;
   }
   else
      g_lPixelsPerInchY -= lMod;

// TraceLineI( "PixelsPerInchX = ", g_lPixelsPerInchX );
// TraceLineI( "PixelsPerInchY = ", g_lPixelsPerInchY );

   // Reset the mapping mode.
   dc.SetMapMode( nOldMapMode );
#else

   g_lPixelsPerInchX = afxData.cxPixelsPerInch;
   g_lPixelsPerInchY = afxData.cyPixelsPerInch;

#endif

   WNDCLASS  wc;

   wc.style         = CS_OWNDC | CS_GLOBALCLASS | CS_DBLCLKS;
   wc.lpfnWndProc   = ZeidonZDCTL_WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = AfxGetInstanceHandle( );
   wc.hIcon         = 0;
   wc.hCursor       = ::LoadCursor( 0, IDC_ARROW );
   wc.hbrBackground = (HBRUSH) ::GetStockObject( LTGRAY_BRUSH );
   wc.lpszMenuName  = 0;
   wc.lpszClassName = "ZeidonCustomCtrl";

   if ( AfxRegisterClass( &wc ) == 0 )
      TraceLineS( "Unable to register window class: ", wc.lpszClassName );

// if ( RegisterSS( AfxGetInstanceHandle( ) ) == FALSE )
// {
//    DWORD dw = GetLastError( );
//    TraceLineX( "Registration Error ", dw );
// }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
fnGetRootEntityName( zVIEW vObject, zPCHAR pchEntity, zLONG lMaxLth )
{
   zCHAR szParentEntity[ zTAG_LTH ];
   strcpy_s( szParentEntity, sizeof( szParentEntity ), pchEntity );

   while ( szParentEntity[ 0 ] )
   {
      strcpy_s( pchEntity, lMaxLth, szParentEntity );
      MiGetParentEntityNameForView( szParentEntity, vObject, pchEntity );
   }
}

void
fnGetImageURL( ZSubtask *pZSubtask, zPCHAR pchImageURL, zSHORT nMaxLth )
{
#ifdef zREMOTE_SERVER

   if ( pZSubtask->m_pzsImageURL == 0 )
   {
      // Get Resource Image and URL names if specified.
      GetAppOrWorkstationValue( pZSubtask->m_vDialog,
                                "ExecutableRemoteImageURL",
                                pchImageURL, nMaxLth );
      pZSubtask->m_pzsImageURL = new CString( pchImageURL );
   }

   strcpy_s( pchImageURL, nMaxLth, *(pZSubtask->m_pzsImageURL) );

#else

   pchImageURL[ 0 ] = 0;

#endif
}

void
fnEnableChildCtrlRecursive( ZMapAct *pzma,
                            WPARAM  wParam,
                            LPARAM  lParam )
{
   ZMapAct *pzmaParent = (ZMapAct *) lParam;
   if ( pzma->m_pzmaComposite == pzmaParent )
   {
      fnEnableChildCtrls( pzma, (wParam & 1) ? TRUE : FALSE,
                          (wParam & 2) ? TRUE : FALSE );
   }
}

// Recursive routine to enable child ctrls.
void
fnEnableChildCtrls( ZMapAct *pzma, zBOOL bEnable, zBOOL bProcessChildren )
{
   WPARAM wParam = (bEnable ? 1 : 0) + 2;
   pzma->m_pZSubtask->
       m_pZMIXCtrl->VisitInorder( (fnVISIT_INORDER) fnEnableChildCtrlRecursive,
                                  0, wParam, (LPARAM) pzma );

   if ( bProcessChildren && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
   {
   // if ( zstrcmp( "CBAdmissionsTrack", *(pzma->m_pzsTag) ) == 0 )
   //    TraceLineS( "", "" );

      if ( bEnable )
      {
         if ( pzma->m_ulMapActFlags & zMAPACT_ENABLED )
            pzma->m_pCtrl->EnableWindow( TRUE );
      }
      else
      {
         pzma->m_pCtrl->EnableWindow( FALSE );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT CString OPERATION
FormatTimeAmPm( COleDateTime& date )
{
   CString cs;
   zPCHAR  pch;
   int     nHour = date.GetHour( );

   if ( nHour < 12 )
      pch = "am";
   else
   {
      pch = "pm";
      nHour -= 12;
   }

   if ( nHour == 0 )
      nHour = 12;

   cs.Format( "%d:%02d%s", nHour, date.GetMinute( ), pch );
   return( cs );
}

zOPER_EXPORT void OPERATION
SetZeidonDateFromOleDateTime( zPCHAR pchDate, zLONG lMaxLth, COleDateTime& DateTime )
{
   // pchDate looks like this:  YYYYMMDDhhmmssttt
   sprintf_s( pchDate, lMaxLth, "%04d%02d%02d%02d%02d%02d%03d",
              DateTime.GetYear( ), DateTime.GetMonth( ),
              DateTime.GetDay( ), DateTime.GetHour( ),
              DateTime.GetMinute( ), DateTime.GetSecond( ), 0 );
}

zOPER_EXPORT void OPERATION
SetZeidonDateAttributeFromOleDateTime( COleDateTime& DateTime,
                                       zVIEW vApp,
                                       zCPCHAR cpcEntity,
                                       zCPCHAR cpcAttribute )
{
   zCHAR  szDate[ 20 ];

   SetZeidonDateFromOleDateTime( szDate, sizeof( szDate ), DateTime );
   SetAttributeFromString( vApp, cpcEntity, cpcAttribute, szDate );
}

zOPER_EXPORT void OPERATION
SetOleDateTimeFromZeidonDate( COleDateTime& DateTime, zCPCHAR cpcDate )
{
   zCHAR  szDate[ 20 ];

   // cpcDate looks like this:  YYYYMMDDhhmmssttt
   strcpy_s( szDate, sizeof( szDate ), cpcDate );
// DateTime.wMilliseconds = atoi( cpcDate + 14 );
   szDate[ 14 ] = 0;
   int nSecond = atoi( szDate + 12 );
   szDate[ 12 ] = 0;
   int nMinute = atoi( szDate + 10 );
   szDate[ 10 ] = 0;
   int nHour = atoi( szDate + 8 );
   szDate[ 8 ] = 0;
   int nDay = atoi( szDate + 6 );
   szDate[ 6 ] = 0;
   int nMonth = atoi( szDate + 4 );
   szDate[ 4 ] = 0;
   int nYear = atoi( szDate );

   if ( nYear == 1900 && nMonth == 1 && nDay == 1 )  // Zeidon 0-date
      DateTime.SetTime( nHour, nMinute, nSecond );
   else
   if ( nHour == 0 && nMinute == 0 && nSecond == 0 )
      DateTime.SetDate( nYear, nMonth, nDay );
   else
      DateTime.SetDateTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );

   DateTime.SetStatus( COleDateTime::valid );
}

zOPER_EXPORT void OPERATION
GetOleDateTimeFromZeidonDateAttribute( COleDateTime& DateTime,
                                       zVIEW vApp,
                                       zCPCHAR cpcEntity,
                                       zCPCHAR cpcAttribute )
{
   zCHAR  szDate[ 20 ];

   GetStringFromAttribute( szDate, sizeof( szDate ), vApp, cpcEntity, cpcAttribute );
   SetOleDateTimeFromZeidonDate( DateTime, szDate );
}

zOPER_EXPORT void OPERATION
SetZeidonDateFromSysTime( zPCHAR pchDate, zLONG lMaxLth, SYSTEMTIME& SysTime )
{
   // szDate looks like this:  YYYYMMDDhhmmssttt
   sprintf_s( pchDate, lMaxLth, "%04d%02d%02d%02d%02d%02d%03d",
              SysTime.wYear, SysTime.wMonth,
              SysTime.wDay, SysTime.wHour,
              SysTime.wMinute, SysTime.wSecond,
              SysTime.wMilliseconds );
}

zOPER_EXPORT BOOL OPERATION
SetSysTimeFromZeidon( SYSTEMTIME& SysTime, zCPCHAR cpcDate )
{
   COleDateTime DateTime;

   SetOleDateTimeFromZeidonDate( DateTime, cpcDate );

   // cpcDate looks like this:  YYYYMMDDhhmmssttt
   SysTime.wMilliseconds = atoi( cpcDate + 14 );
   SysTime.wSecond = DateTime.GetSecond( );
   SysTime.wMinute = DateTime.GetMinute( );
   SysTime.wHour = DateTime.GetHour( );
   SysTime.wDay = DateTime.GetDay( );
   SysTime.wMonth = DateTime.GetMonth( );
   SysTime.wYear = DateTime.GetYear( );
   SysTime.wDayOfWeek = DateTime.GetDayOfWeek( );
   if ( DateTime.GetStatus( ) == COleDateTime::valid )
      return( TRUE );
   else
      return( FALSE );
}

zOPER_EXPORT BOOL OPERATION
PointWithinRange( CPoint& pt1, CPoint& pt2, int nRangeX, int nRangeY )
{
   int nRange = pt1.x - pt2.x;
   if ( nRange > nRangeX || nRange < -nRangeX )
      return( FALSE );

   nRange = pt1.y - pt2.y;
   if ( nRange > nRangeY || nRange < -nRangeY )
      return( FALSE );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// TitleTip.cpp : implementation file
//
// Based on code by Zafir Anjum
//
// Adapted by Chris Maunder <cmaunder@mail.com>
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
// For use with CGridCtrl v2.20+
//
// History
//         10 Apr 1999  Now accepts a LOGFONT pointer and
//                      a tracking rect in Show(...)  (Chris Maunder)
//         18 Apr 1999  Resource leak in Show fixed by Daniel Gehriger
//          8 Mar 2000  Added double-click fix found on codeguru
//                      web site but forgot / can't find who contributed it
//         28 Mar 2000  Aqiruse (marked with //FNA)
//                      Titletips now use cell color
//         18 Jun 2000  Delayed window creation added
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZTitleTip
//
ZTitleTip::ZTitleTip( zLONG lHoverDelay )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZTitleTip::ctor Delay: ", lHoverDelay );
#endif

   // Register the window class if it has not already been registered.
   WNDCLASS wndcls;
   HINSTANCE hInst = AfxGetInstanceHandle( );

   if ( ::GetClassInfo( hInst, ZTITLETIP_CLASSNAME, &wndcls ) == 0 )
   {
      // Otherwise we need to register a new class.
      wndcls.style         = CS_SAVEBITS;
      wndcls.lpfnWndProc   = ::DefWindowProc;
      wndcls.cbClsExtra    = wndcls.cbWndExtra = 0;
      wndcls.hInstance     = hInst;
      wndcls.hIcon         = 0;
      wndcls.hCursor       = LoadCursor( hInst, IDC_ARROW );
      wndcls.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1);
      wndcls.lpszMenuName  = 0;
      wndcls.lpszClassName = ZTITLETIP_CLASSNAME;
      if ( AfxRegisterClass( &wndcls ) == 0 )
         AfxThrowResourceException( );
   }

   m_dwLastLButtonDown  = ULONG_MAX;
   m_ulDblClickInterval = GetDoubleClickTime( );
   m_bCreated           = FALSE;
   m_pParentWnd         = 0;
   m_lHoverDelay        = lHoverDelay;
}

ZTitleTip::~ZTitleTip( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTitleTip::dtor", "" );
#endif
}

BEGIN_MESSAGE_MAP( ZTitleTip, CWnd )
   //{{AFX_MSG_MAP( ZTitleTip )
   ON_WM_MOUSEMOVE( )
   ON_WM_TIMER( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZTitleTip message handlers

BOOL
ZTitleTip::Create( CWnd *pParentWnd )
{
   ASSERT_VALID( pParentWnd );

   // Already created?
   if ( m_bCreated == FALSE )
   {
      DWORD dwStyle = WS_BORDER | WS_POPUP;
      DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
      m_pParentWnd = pParentWnd;
      m_uToolTipTimerId = 0;
      m_nOffsetX = 0;
      m_bCreated = CreateEx( dwExStyle, ZTITLETIP_CLASSNAME, 0, dwStyle,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             0, 0, 0 );
   }

   return( m_bCreated );
}

BOOL
ZTitleTip::DestroyWindow( )
{
   m_bCreated = FALSE;
   Hide( );
   return( CWnd::DestroyWindow( ) );
}

void
ZTitleTip::DrawTipText( int nId )
{
// TraceLineI( "ZTitleTip::DrawTipText Id: ", nId );
   CClientDC dc( this );
   CFont     *pFontDC;
   int       nOldMode = dc.SetBkMode( TRANSPARENT );

// if ( pLogFont )
// {
//    CFont font;
//    font.CreateFontIndirect( pLogFont );
//    pFontDC = dc.SelectObject( &font );
// }
// else
   {
      // Use same font as ctrl.
      pFontDC = dc.SelectObject( m_pParentWnd->GetFont( ) );
   }

   CSize size = dc.GetTextExtent( m_csTitle );

   TEXTMETRIC tm;
   dc.GetTextMetrics( &tm );
   size.cx += tm.tmOverhang;

   // Determine the width of the text.
   CRect rectTitle = m_rectTitle;
   m_pParentWnd->ClientToScreen( rectTitle );

   CRect rectDisplay = rectTitle;
   rectDisplay.left += m_nOffsetX;
   rectDisplay.right = rectDisplay.left + size.cx + m_nOffsetX;

   CRect rectExtent = rectTitle;
   int nTextHeight = dc.DrawText( m_csTitle, rectExtent, DT_CALCRECT | DT_TOP );

   // Do not display if the text fits within available space.
   if ( nTextHeight > size.cy ||
        rectDisplay.right > rectTitle.right - m_nOffsetX )
   {
      // Show the titletip.
      if ( nTextHeight > size.cy )
         SetWindowPos( &wndTop, rectDisplay.left, rectDisplay.top,
                       rectExtent.Width( ) + 4, nTextHeight + 2,
                       SWP_SHOWWINDOW | SWP_NOACTIVATE );
      else
         SetWindowPos( &wndTop, rectDisplay.left, rectDisplay.top,
                       rectDisplay.Width( ), rectDisplay.Height( ),
                       SWP_SHOWWINDOW | SWP_NOACTIVATE );

      // FNA - handle colors correctly.
      if ( m_clrBack != CLR_DEFAULT )
      {
         CBrush brushBk( m_clrBack );
         CBrush *pOldBrush = dc.SelectObject( &brushBk );
         CRect rect;
         dc.GetClipBox( &rect );     // Erase the area needed

         dc.PatBlt( rect.left, rect.top, rect.Width( ),
                    rect.Height( ), PATCOPY );
         dc.SelectObject( pOldBrush );
      }

      // Set color.
      if ( m_clrText != CLR_DEFAULT )  // FNA
         dc.SetTextColor( m_clrText ); // FA

   // dc.SetBkMode( TRANSPARENT );
      if ( nTextHeight > size.cy )
      {
         rectDisplay.top = 0;
         rectDisplay.right -= rectDisplay.left;
         rectDisplay.left = 0;
         rectDisplay.bottom = rectDisplay.top + nTextHeight + 2;
         dc.DrawText( m_csTitle, rectDisplay, DT_TOP );
      }
      else
      {
         dc.TextOut( 0, 0, m_csTitle );
      }
   }
   else
   {
      Hide( );
   }

   dc.SetBkMode( nOldMode );
   dc.SelectObject( pFontDC );
}


// Show      - Show the titletip if needed.
// rectTitle - The rectangle within which the original title
//             is constrained - in client coordinates.
// cpcTitleText - The text to be displayed.
// nOffsetX  - Number of pixels that the text is offset from
//             the left border of the cell.
void
ZTitleTip::Show( CRect rectTitle, zCPCHAR cpcTitleText,
                 int nOffsetX /* = 0 */,
                 LPRECT pHoverRect /* = 0 */,
                 const LOGFONT *pLogFont /* = 0 */,
                 COLORREF clrText /* = CLR_DEFAULT */,
                 COLORREF clrBack /* = CLR_DEFAULT */ )
{
#ifdef DEBUG_ALL
   TraceLine( "ZTitleTip::Show %s   Delay: %d", cpcTitleText, m_lHoverDelay );
#endif

   CString csTitle = _T( " " );
   csTitle += cpcTitleText;
   csTitle += _T( " " );
   if ( rectTitle == m_rectTitle && m_csTitle == csTitle )
      return;

// TraceLine( "ZTitleTip::Show csTitle: %s   m_csTitle: %s", csTitle, m_csTitle );
// TraceRect( "ZTitleTip::Show rectTitle: ", rectTitle );
// TraceRect( "ZTitleTip::Show m_rectTitle: ", m_rectTitle );

   m_csTitle = csTitle;

   if ( mIs_hWnd( m_hWnd ) == 0 )
      Create( m_pParentWnd );

   ASSERT( ::IsWindow( GetSafeHwnd( ) ) );
   if ( mIs_hWnd( m_hWnd ) == 0 )
      return;

   m_rectHover = pHoverRect ? pHoverRect : rectTitle;
   m_rectHover.right++;
   m_rectHover.bottom++;

// TraceRect( "ZTitleTip::Show Hover1", m_rectHover );
   m_pParentWnd->ClientToScreen( m_rectHover );
// TraceRect( "ZTitleTip::Show Hover2", m_rectHover );

   m_nOffsetX = nOffsetX;
   m_clrText = clrText;
   m_clrBack = clrBack;
#if 0
   // Define the rectangle outside which the titletip will be hidden.
   // We add a buffer of one pixel around the rectangle.
   m_rectTitle.top    = -1;
   m_rectTitle.left   = -m_nOffsetX - 1;
   m_rectTitle.right  = rectTitle.Width( ) - m_nOffsetX;
   m_rectTitle.bottom = rectTitle.Height( ) + 1;
#else
   m_rectTitle = rectTitle;
#endif
// TraceRect( "ZTitleTip::Show rectTitle: ", m_rectTitle );

   // If window is not valid, display rect is empty, text is empty,
   // title tip is already displayed, or app does not have focus ...
   // don't do anything.
   if ( mIs_hWnd( m_hWnd ) == 0 || rectTitle.IsRectEmpty( ) ||
        cpcTitleText == 0 || *cpcTitleText == 0 ||
        mIs_hWndVisible( m_hWnd ) || GetFocus( ) == 0 )
   {
      return;
   }

   if ( m_lHoverDelay > 0 )
   {
      if ( m_uToolTipTimerId )
         KillTimer( m_uToolTipTimerId );

      m_uToolTipTimerId = SetTimer( g_nIdShowTitleTipEvt, m_lHoverDelay, 0 );
   }
   else
   {
      DrawTipText( 1 );
   }

   SetCapture( );
}

#if 0
void
ZTitleTip::Hide( )
{
   if ( ::IsWindow( GetSafeHwnd( ) ) )
   {
      if ( GetCapture( )->GetSafeHwnd( ) == GetSafeHwnd( ) )
         ReleaseCapture( );

      ShowWindow( SW_HIDE );
   }
}
#else
void
ZTitleTip::Hide( )
{
// TraceLine( "ZTitleTip::Hide Timer Id: %d  Title: %s", m_uToolTipTimerId, m_csTitle );
// if ( m_uToolTipTimerId == 0 && zstrcmp( m_csTitle, " 9/10/1996 " ) == 0 )
//    ASSERT( 0 );

   if ( ::IsWindow( GetSafeHwnd( ) ) )
   {
   // if ( GetCapture( )->GetSafeHwnd( ) == GetSafeHwnd( ) )
      if ( GetCapture( ) == this )
         ReleaseCapture( );

      if ( m_uToolTipTimerId )
      {
         KillTimer( m_uToolTipTimerId );
         m_csTitle = "";
         m_rectTitle.SetRectEmpty( );
      }

   // ShowWindow( SW_HIDE );
      SetWindowPos( 0, 0, 0, 0, 0,
                    SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE |
                      SWP_NOZORDER | SWP_NOACTIVATE );
   }

   m_uToolTipTimerId = 0;
}
#endif

void
ZTitleTip::OnMouseMove( UINT uFlags, CPoint pt )
{
// const MSG *pMsg = CWnd::GetCurrentMessage( );
// CPoint ptm = pMsg->pt;
// TracePoint( "CurrentMessage pt1", ptm );
// ScreenToClient( &ptm );
// TracePoint( "CurrentMessage pt2", ptm );
// TraceRect( "ZTitleTip::OnMouseMove rectHover", m_rectHover );
// TracePoint( "ZTitleTip::OnMouseMove pt", pt );
   CRect rect = m_rectHover;
   ScreenToClient( rect );
// TraceRect( "ZTitleTip::Show Hover3", rect );

   if ( rect.PtInRect( pt ) == FALSE )
   {
      Hide( );

      // Forward the message
      ClientToScreen( &pt );
      CWnd *pWnd = WindowFromPoint( pt );
      if ( pWnd == this )
         pWnd = m_pParentWnd;

      int nHitTest = (int) pWnd->SendMessage( WM_NCHITTEST, 0,
                                              MAKELONG( pt.x, pt.y ) );

      if ( nHitTest == HTCLIENT )
      {
         pWnd->ScreenToClient( &pt );
         pWnd->PostMessage( WM_MOUSEMOVE, uFlags, MAKELONG( pt.x, pt.y ) );
      }
      else
      {
         pWnd->PostMessage( WM_NCMOUSEMOVE, nHitTest, MAKELONG( pt.x, pt.y ) );
      }
   }
}

BOOL
ZTitleTip::PreTranslateMessage( MSG *pMsg )
{
   // Used to qualify WM_LBUTTONDOWN messages as double-clicks.
   DWORD dwTick = 0;
   BOOL bDoubleClick = FALSE;

   CWnd *pWnd;
   int nHitTest;
   switch ( pMsg->message )
   {
      case WM_LBUTTONDOWN:
         // Get tick count since last LButtonDown.
         dwTick = GetTickCount( );
         bDoubleClick = ((dwTick - m_dwLastLButtonDown) <= m_ulDblClickInterval);
      // TraceLine( "ZTitleTip::PreTranslateMessage WM_LBUTTONDOWN DoubleClick: %s  Tick Count: %d",
      //            bDoubleClick ? "TRUE" : "FALSE", dwTick );
         m_dwLastLButtonDown = dwTick;
         // NOTE: No break; Intentionally fall through!

      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
      {
         POINTS pts = MAKEPOINTS( pMsg->lParam );
         CPoint pt;
         pt.x = pts.x;
         pt.y = pts.y;

         ClientToScreen( &pt );
         Hide( );

         pWnd = WindowFromPoint( pt );
         if ( pWnd == 0 )
            return( CWnd::PreTranslateMessage( pMsg ) );

      // if ( pWnd->GetSafeHwnd( ) == GetSafeHwnd( ) )
         if ( pWnd == this )
            pWnd = m_pParentWnd;

         nHitTest = (int) pWnd->SendMessage( WM_NCHITTEST, 0,
                                             MAKELONG( pt.x, pt.y ) );
         if ( nHitTest == HTCLIENT )
         {
            pWnd->ScreenToClient( &pt );
            pMsg->lParam = MAKELONG( pt.x, pt.y );
         }
         else
         {
            switch ( pMsg->message )
            {
               case WM_LBUTTONDOWN:
                  pMsg->message = WM_NCLBUTTONDOWN;
                  break;

               case WM_RBUTTONDOWN:
                  pMsg->message = WM_NCRBUTTONDOWN;
                  break;

               case WM_MBUTTONDOWN:
                  pMsg->message = WM_NCMBUTTONDOWN;
                  break;
            }

            pMsg->wParam = nHitTest;
            pMsg->lParam = MAKELONG( pt.x, pt.y );
         }

         // If this is the 2nd WM_LBUTTONDOWN in x milliseconds,
         // post a WM_LBUTTONDBLCLK message instead of a single click.
         pWnd->PostMessage( bDoubleClick ? WM_LBUTTONDBLCLK : pMsg->message,
                            pMsg->wParam, pMsg->lParam );
      // m_pParentWnd->PostMessage( bDoubleClick ? WM_LBUTTONDBLCLK : pMsg->message,
      //                            pMsg->wParam, pMsg->lParam );
         return( TRUE );
      }

      case WM_KEYDOWN:
      case WM_SYSKEYDOWN:
      {
         Hide( );
         m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
         return( TRUE );
      }
   }

   if ( GetFocus( ) == 0 )
   {
      Hide( );
      return( TRUE );
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

void
ZTitleTip::OnTimer( UINT uIdTimer )
{
// TraceLine( "ZTitleTip::OnTimer Id: %d", uIdTimer );

   if ( uIdTimer == m_uToolTipTimerId && mIs_hWnd( m_hWnd ) )
   {
      CPoint pt;

      if ( GetCursorPos( &pt ) )  // Where is the mouse right now?
      {
         // Over which window?
      // TracePoint( "ZTitleTip::OnTimer pt: ", pt );
      //xCWnd *pWnd = WindowFromPoint( pt );
      //xif ( GetSafeHwnd( ) == pWnd->GetSafeHwnd( ) )
      //x{
         // TraceLineS( "ZTitleTip::OnTimer ShowWindow ACTIVE", "" );
            ShowWindow( SW_SHOWNOACTIVATE );
      //x}

         DrawTipText( 2 );
      }

      KillTimer( m_uToolTipTimerId );
      m_uToolTipTimerId = 0;
   }

   CWnd::OnTimer( uIdTimer );
}

/////////////////////////////////////////////////////////////////////////////
// ZListTip
//
ZListTip::ZListTip( zLONG lHoverDelay )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListTip::ctor", "====================>>>>" );
#endif
   // Register the window class if it has not already been registered.
   WNDCLASS wndcls;
   HINSTANCE hInst = AfxGetInstanceHandle( );

   if ( ::GetClassInfo( hInst, "ZeidonListTip", &wndcls ) == 0 )
   {
      // Otherwise we need to register a new class.
      wndcls.style         = CS_SAVEBITS;
      wndcls.lpfnWndProc   = ::DefWindowProc;
      wndcls.cbClsExtra    = wndcls.cbWndExtra = 0;
      wndcls.hInstance     = hInst;
      wndcls.hIcon         = 0;
      wndcls.hCursor       = LoadCursor( hInst, IDC_ARROW );
      wndcls.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1);
      wndcls.lpszMenuName  = 0;
      wndcls.lpszClassName = "ZeidonListTip";
      if ( AfxRegisterClass( &wndcls ) == 0 )
         AfxThrowResourceException( );
   }

   m_ulDblClickInterval = 0;
   m_ptStart.x = m_ptStart.y = 0;
   m_lHoverDelay = lHoverDelay;
}

ZListTip::~ZListTip( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListTip::dtor", "" );
#endif
}

BEGIN_MESSAGE_MAP( ZListTip, CWnd )
   //{{AFX_MSG_MAP( ZListTip )
   ON_WM_MOUSEMOVE( )
   ON_WM_TIMER( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZListTip message handlers

BOOL
ZListTip::Create( CWnd *pParentWnd )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListTip::Create", "" );
#endif

   ASSERT_VALID( pParentWnd );
   DWORD dwStyle = WS_BORDER | WS_POPUP;
   DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
   m_pParentWnd = pParentWnd;
   m_uToolTipTimerId = 0;
   m_nOffsetX = 0;
   return( CreateEx( dwExStyle, "ZeidonListTip", 0, dwStyle,
                     0, 0, 0, 0, pParentWnd->m_hWnd, 0, 0 ) );
}

BOOL
ZListTip::DestroyWindow( )
{
   Hide( );
   return( CWnd::DestroyWindow( ) );
}

void
ZListTip::DrawTipText( )
{
   CClientDC dc( this );
   CFont     *pFont = m_pParentWnd->GetFont( );  // use same font as ctrl
   CFont     *pFontDC = dc.SelectObject( pFont );
   int       nOldMode = dc.SetBkMode( TRANSPARENT );

   dc.TextOut( m_nOffsetX - 1, 0, m_csTitle );
   dc.SetBkMode( nOldMode );
   dc.SelectObject( pFontDC );
}

// Show      - Show the titletip if needed.
// rectTitle - The rectangle within which the original title
//             is constrained - in client coordinates.
// cpcTitleText - The text to be displayed.
// nOffsetX  - Number of pixels that the text is offset from
//             the left border of the cell.
void
ZListTip::Show( CRect rectTitle, zCPCHAR cpcTitleText,
                int nOffsetX /* = 0 */,
                LPRECT pHoverRect /* = 0 */,
                const LOGFONT *pLogFont /* = 0 */,
                COLORREF clrText /* = CLR_DEFAULT */,
                COLORREF clrBack /* = CLR_DEFAULT */ )
{
#ifdef DEBUG_ALL
   TraceLine( "ZListTip::Show %s   Delay: %d", cpcTitleText, m_lHoverDelay );
#endif

   m_csTitle = cpcTitleText;

   // If window is not valid, display rect is empty, text is empty,
   // title tip is already displayed, or app does not have focus ...
   // don't do anything.
   if ( mIs_hWnd( m_hWnd ) == 0 || rectTitle.IsRectEmpty( ) ||
        cpcTitleText == 0 || *cpcTitleText == 0 ||
        mIs_hWndVisible( m_hWnd ) || GetFocus( ) == 0 )
   {
      return;
   }

   // Define the rectangle outside which the titletip will be hidden.
   // We add a buffer of one pixel around the rectangle.
   m_rectTitle.top = -1;
   m_rectTitle.left = -1;
   m_rectTitle.right = rectTitle.Width( );
   m_rectTitle.bottom = rectTitle.Height( ) + 1;

   // Determine the width of the text.
   m_pParentWnd->ClientToScreen( rectTitle );

   CClientDC dc( this );
   CFont *pFontDC;
// if ( pLogFont )
// {
//    CFont font;
//    font.CreateFontIndirect( pLogFont );
//    pFontDC = dc.SelectObject( &font );
// }
// else
   {
      // Use same font as ctrl.
      pFontDC = dc.SelectObject( m_pParentWnd->GetFont( ) );
   }

   m_nOffsetX = nOffsetX - 1;
   CRect rectDisplay = rectTitle;
   CSize size = dc.GetTextExtent( m_csTitle );
   rectDisplay.top--;
   rectDisplay.left--;
   rectDisplay.right = rectDisplay.left + size.cx + 2 * m_nOffsetX;
   dc.SelectObject( pFontDC );

   if ( rectDisplay.right > rectTitle.right + 1 )
   {
      UINT uFlags = SWP_NOACTIVATE;
      if ( m_lHoverDelay == 0 )
      {
         uFlags |= SWP_SHOWWINDOW;
      }
      else
      if ( m_lHoverDelay > 0 )
      {
         if ( m_uToolTipTimerId )
            KillTimer( m_uToolTipTimerId );

         m_uToolTipTimerId = SetTimer( g_nIdShowToolTipEvt, m_lHoverDelay, 0 );
      // TraceLine( "ZListTip::Show Timer Id: %d", m_uToolTipTimerId );
      }

      SetWindowPos( &wndTop, rectDisplay.left, rectDisplay.top,
                    rectDisplay.Width( ), rectDisplay.Height( ), uFlags );
      DrawTipText( );
      SetCapture( );
   }
}

void
ZListTip::Hide( )
{
   if ( ::IsWindow( GetSafeHwnd( ) ) )
   {
   // if ( GetCapture( )->GetSafeHwnd( ) == GetSafeHwnd( ) )
      if ( GetCapture( ) == this )
         ReleaseCapture( );

      if ( m_uToolTipTimerId )
         KillTimer( m_uToolTipTimerId );

   // ShowWindow( SW_HIDE );
      SetWindowPos( 0, 0, 0, 0, 0,
                    SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE |
                      SWP_NOZORDER | SWP_NOACTIVATE );
   }

   m_uToolTipTimerId = 0;
}

void
ZListTip::OnMouseMove( UINT uFlags, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceRect( "ZListTip::OnMouseMove rectTitle: ", m_rectTitle );
   TracePoint( "ZListTip::OnMouseMove pt: ", pt );
#endif
   if ( m_rectTitle.PtInRect( pt ) == FALSE )
   {
#ifdef DEBUG_ALL
   TracePoint( "ZListTip::OnMouseMove PtInRect: ", pt );
#endif

#if 0
      if ( GetCapture( ) == this )
         ReleaseCapture( );

      SetWindowPos( 0, 0, 0, 0, 0,
                    SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE |
                      SWP_NOZORDER | SWP_NOACTIVATE );
#else
      Hide( );
#endif

      // Forward the message
      ClientToScreen( &pt );
      CWnd *pWnd = WindowFromPoint( pt );
      if ( pWnd == this )
         pWnd = m_pParentWnd;

      CWnd *pWndTemp = GetFocus( );
      if ( pWndTemp != pWnd )
         pWnd->SetFocus( );

      zLONG lParam = MAKELONG( pt.x, pt.y );
      int nHitTest = (int) pWnd->SendMessage( WM_NCHITTEST, 0, lParam );
      if ( nHitTest == HTCLIENT )
      {
         pWnd->ScreenToClient( &pt );
         lParam = MAKELONG( pt.x, pt.y );
         pWnd->PostMessage( WM_MOUSEMOVE, uFlags, lParam );
      }
      else
      {
         pWnd->PostMessage( WM_NCMOUSEMOVE, nHitTest, lParam );
      }
   }
}

BOOL
ZListTip::PreTranslateMessage( MSG *pMsg )
{
   CWnd *pWnd;
   int nHitTest;
   switch ( pMsg->message )
   {
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
      {
         POINTS pts = MAKEPOINTS( pMsg->lParam );
         CPoint pt;
         pt.x = pts.x;
         pt.y = pts.y;

#ifdef DEBUG_ALL
   TracePoint( "ZListTip::OnButtonDown: ", pt );
#endif

         ClientToScreen( &pt );
         pWnd = WindowFromPoint( pt );
         if ( pWnd == 0 )
            return( CWnd::PreTranslateMessage( pMsg ) );

         if ( pWnd == this )
            pWnd = m_pParentWnd;

         nHitTest = (int) pWnd->SendMessage( WM_NCHITTEST, 0,
                                             MAKELONG( pt.x, pt.y ) );
         if ( nHitTest == HTCLIENT )
         {
            if ( pMsg->message == WM_LBUTTONDOWN )
            {
               zULONG ulTime = GetCurrentTime( );
               zULONG ulDblclkTime = GetDoubleClickTime( );
               if ( m_ulDblClickInterval == 0 )
               {
                  m_ulDblClickInterval = ulTime;
                  m_ptStart = pt;
               }
               else
               {
                  if ( ulTime - m_ulDblClickInterval > ulDblclkTime ||
                       m_ptStart.x - pt.x > g_lDblClkX ||
                       pt.x - m_ptStart.x > g_lDblClkX ||
                       m_ptStart.y - pt.y > g_lDblClkY ||
                       pt.y - m_ptStart.y > g_lDblClkY )
                  {
                     m_ptStart = pt;
                     m_ulDblClickInterval = ulTime;
                  }
                  else
                  {
                     pMsg->message = WM_LBUTTONDBLCLK;
                     m_ulDblClickInterval = 0;
                     m_ptStart.x = m_ptStart.y = 0;
                  }
               }
            }

            pWnd->ScreenToClient( &pt );
            pMsg->lParam = MAKELONG( pt.x, pt.y );
         }
         else
         {
            switch ( pMsg->message )
            {
               case WM_LBUTTONDOWN:
               {
                  pMsg->message = WM_NCLBUTTONDOWN;
                  break;
               }

               case WM_RBUTTONDOWN:
               {
                  pMsg->message = WM_NCRBUTTONDOWN;
                  break;
               }

               case WM_MBUTTONDOWN:
               {
                  pMsg->message = WM_NCMBUTTONDOWN;
                  break;
               }
            }

            pMsg->wParam = nHitTest;
            pMsg->lParam = MAKELONG( pt.x, pt.y );
         }

         Hide( );
         pWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
         return( TRUE );
      }

      case WM_KEYDOWN:
      case WM_SYSKEYDOWN:
      {
         Hide( );
         m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
         return( TRUE );
      }
   }

   if ( GetFocus( ) == 0 )
   {
      Hide( );
      return( TRUE );
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

void
ZListTip::OnTimer( UINT uIdTimer )
{
// TraceLine( "ZListTip::OnTimer Id: %d", uIdTimer );

   if ( uIdTimer == m_uToolTipTimerId && mIs_hWnd( m_hWnd ) )
   {
      CPoint pt;

      KillTimer( m_uToolTipTimerId );
      m_uToolTipTimerId = 0;

      if ( GetCursorPos( &pt ) )  // Where is the mouse right now?
      {
         // Over which window?
      // TracePoint( "ZListTip::OnTimer pt: ", pt );
      //xCWnd *pWnd = WindowFromPoint( pt );
      //xif ( GetSafeHwnd( ) == pWnd->GetSafeHwnd( ) )
      //x{
         // TraceLineS( "ZListTip::OnTimer ShowWindow ACTIVE", "" );
            ShowWindow( SW_SHOWNOACTIVATE );
      //x}

         DrawTipText( );
      }
   }

   CWnd::OnTimer( uIdTimer );
}

zSHORT OPERATION
CompareCell( ZColorItem *pColorItem, zLONG lColumn,
             zCPCHAR cpcValue, zBOOL bTestBoth )
{
   if ( (pColorItem->m_lClrFlags & zALWAYS_EQUAL) &&
        lColumn == pColorItem->m_lColumn )
   {
      return( 0 );
   }

   if ( bTestBoth &&   // check long value as well as string value
        lColumn &&     // match any column
        lColumn != pColorItem->m_lColumn )
   {
      return( -1 );    // not equal
   }

   zSHORT nRC;

   if ( (pColorItem->m_lClrFlags & zNZ) == zNZ ||   // null or zero
        (pColorItem->m_lClrFlags & zNN) == zNN )    // not null/zero
   {
      zPCHAR pch = (zPCHAR) cpcValue;
      while ( pch && *pch && (*pch == '0' || *pch == '.' || isspace( *pch )) )
         pch++;

      if ( pch && *pch )
      {
         if ( (pColorItem->m_lClrFlags & zNZ) == zNZ )  // null or zero
            return( -1 );  // it is N0T null or zero
         else
      // if ( (pColorItem->m_lClrFlags & zNN) == zNN )  // not null/zero
            return( 0 );   // it is not null/zero
      }
      else
      {
         if ( (pColorItem->m_lClrFlags & zNZ) == zNZ )  // null or zero
            return( 0 ); // it is null or zero
         else
      // if ( (pColorItem->m_lClrFlags & zNN) == zNN )  // not null/zero
            return( -1 );
      }
   }
   else
   if ( pColorItem->m_lClrFlags & zNUMERIC_COMPARE )    // vs. string compare
   {
      double d1 = atof( cpcValue );
      double d2 = atof( pColorItem->m_csValue );
      d1 -= d2;
      if ( d1 < 0 )
         nRC = -1;
      else
      if ( d1 == 0 )
         nRC = 0;
      else
      if ( d1 > 0 )
         nRC = 1;
   }
   else
   if ( pColorItem->m_lClrFlags & 0x000f0000 ) // contains, starts with (NOT)
   {
      CString cs1( pColorItem->m_csValue );
      CString cs2( cpcValue );
      cs1.MakeUpper( );
      cs2.MakeUpper( );
      int nFind = cs2.Find( cs1 );
      nRC = -1;
      if ( nFind >= 0 )
      {
         if ( (pColorItem->m_lClrFlags & zIN) == zIN )  // in (contains)
            nRC = 0;
         else
         if ( nFind == 0 )
         {
            if ( (pColorItem->m_lClrFlags & zSW) == zSW ) // starts with
               nRC = 0;
         }
         else
         {
            if ( (pColorItem->m_lClrFlags & zNS) == zNS ) // does not start with
               nRC = 0;
         }
      }
      else
      if ( (pColorItem->m_lClrFlags & zNI) == zNI || // not in (does not contain)
           (pColorItem->m_lClrFlags & zNS) == zNS )  // does not start with
      {
         nRC = 0;
      }

      return( nRC );
   }
   else
   {
      if ( (pColorItem->m_lClrFlags & zIGNORE_CASE) == zIGNORE_CASE )
         nRC = zstricmp( cpcValue, pColorItem->m_csValue.GetString() );
      else
         nRC = zstrcmp( cpcValue, pColorItem->m_csValue.GetString() );
   }

   switch ( pColorItem->m_lClrFlags & 0x0fff0000 )
   {
      case zLT:    // less than
         if ( nRC < 0 )
            nRC = 0;
         else
            nRC = -1;

         break;

      case zLE:    // less than or equal
         if ( nRC <= 0 )
            nRC = 0;
         else
            nRC = -1;

         break;

      case zGE:    // greater than or equal
         if ( nRC >= 0 )
            nRC = 0;
         else
            nRC = -1;

         break;

      case zGT:    // greater than
         if ( nRC > 0 )
            nRC = 0;
         else
            nRC = -1;

         break;

      case zNE:    // not equal
         if ( nRC )
            nRC = 0;
         else
            nRC = -1;

         break;

      default:
      case zEQ:    // equal (default)
         break;
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#if 0
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
void
CToolTipWnd::SetTimer( int iDelay )
{
   KillTimer( );
   if ( m_lHoverDelay > 0 )
      SetTimer( g_nIdShowToolTipEvt, m_lHoverDelay, 0 );
}


void
CToolTipWnd::SetShowTimer( int iTimeTillShow )
{
   KillShowTimer( );
   if ( iTimeTillShow > 0 ) // no timer if <= 0
      m_iTimer2 = CWnd::SetTimer( ID_TIMER_TOOLTIP_SHOW, iTimeTillShow, 0 );
}

// --------------------------------------------------------------------------
void
CToolTipWnd::KillTimer( )
{
   if ( m_iTimer > 0 && m_hWnd )
      CWnd::KillTimer( m_iTimer );

   m_iTimer = 0;
}

void
CToolTipWnd::KillShowTimer( )
{
   if ( m_iTimer2 > 0 && m_hWnd )
      CWnd::KillTimer( m_iTimer2 );

   m_iTimer2 = 0;
}

// --------------------------------------------------------------------------
// Make the tooltip appear just as if the mouse just entered 'hWnd'
// 'hWnd' must be the active window.

void
CToolTipWnd::Show( HWND hWnd, zCPCHAR cpcOverrideText, int timerDelayHide, int timerDelayShow )
{
#ifdef DEBUG_ALL
   TraceLine( "CToolTipWnd::Show %s", cpcOverrideText );
#endif

   // Make sure the window is active.
   CWnd *pFocusWnd = AfxGetApp( )->m_pMainWnd->GetFocus( );
   if ( pFocusWnd == 0 )
      return;

   // If another one was present, hide it.
   Hide( );

   // take the 'hWnd' coordinates and compute where the tooltip should appear
   RECT rect;
   ::GetWindowRect( hWnd, &rect );
   POINT pt;
   pt.x = rect.left + ( ( rect.right - rect.left ) / 2 );
   pt.y = rect.bottom;

   // Determine the text to dislay: 'cpcOverrideText' or original tooltip text
   if ( cpcOverrideText )
      m_csText = cpcOverrideText;
   else
   {
      // Reset 'm_csText' to original from tooltip
      BTOOLINFO *stToolInfo;
      if ( m_toolPtr.Lookup( hWnd, ( void *& )stToolInfo ) )
         m_csText = stToolInfo->strToolText;
   }

   // Make it appear at 'pt'
   if ( timerDelayHide <= 0 ) // don't permit a tooltip to stay forever
      timerDelayHide = m_iDelay;

   Show( pt.x, pt.y, timerDelayHide, timerDelayShow );

   m_bSkipNextMove = true; // it seems we receive a WM_MOUSEMOVE whenever a window is displayed...
   m_pCurrwnd = hWnd;
}


// --------------------------------------------------------------------------
void
CToolTipWnd::Show( int x, int y, int timerDelayToHide, int timerDelayToShow ) // force apparition of the tooltip
{
#ifdef DEBUG_ALL
   TraceLine( "CToolTipWnd::Show %s", cpcOverrideText );
#endif

   // Check to see if the tooltip has a portion outside the
   // screen, if so, adjust.
   if ( x < 0 )
      x = 0;

   if ( y < 0 )
      y = 0;

   RECT r;
   r.left = x;
   r.right = r.left + m_iWidth;
   r.top = y;
   r.bottom = r.top + m_iHeight;

   // Compare to screen coordinates ( don't take in account the desktop toolbar )
   int screenHeight = GetSystemMetrics( SM_CYSCREEN );
   if ( r.bottom > screenHeight )
      r.top = screenHeight - ( m_iHeight );

   int screenWidth = GetSystemMetrics( SM_CXSCREEN );
   if ( r.right > screenWidth )
      r.left = screenWidth - m_iWidth;

   // Move the window
   SetWindowPos( &wndTop, r.left, r.top, m_iWidth, m_iHeight, SWP_NOACTIVATE );

   // Only show after show delay has passed
   // ShowWindow( SW_SHOWNOACTIVATE );

    // Start the hide delay timer
   SetTimer( timerDelayToHide );

   // Start the show delay timer
   SetShowTimer( timerDelayToShow );
}


// --------------------------------------------------------------------------
void
CToolTipWnd::Hide( )
{
   if ( IsWindowVisible( ) )
   {
      ShowWindow( SW_HIDE );
      KillTimer( );
      KillShowTimer( );
   }
}
#endif
