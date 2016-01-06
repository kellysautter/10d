/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlcal.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Calendar control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2003.02.14    DKS
//    Implemented MiniCalendar and DayTimer controls.
//
// 2000.03.22    DKS    Z2000
//    Disable control when the entity does not exist or update is disabled.
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlCal.h"
#include "ZdCtlGbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

static const COLORREF g_clrTransparent = RGB( 255, 0, 255 );
static const int g_nImageWidth = 10;
static const int g_nHorzOffset = 10;
static const int g_nIdClickButtonEvt = 1;

static const int g_nSlowTimerDuration = 300;
static const int g_nFastTimerDuration = 100;
static const int g_nNumberOfSlowTimes = 10;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZCalendar, CWnd )
BEGIN_MESSAGE_MAP( ZCalendar, CWnd )
   ON_WM_CREATE( )
   ON_WM_ENABLE( )
   ON_WM_PAINT( )
   ON_WM_CTLCOLOR( )
// ON_WM_CTLCOLOR_REFLECT( )
   ON_NOTIFY_REFLECT( DTN_DATETIMECHANGE, OnDateTimeChange )
   ON_NOTIFY_REFLECT( DTN_USERSTRING, OnUserString )
   ON_NOTIFY_REFLECT( DTN_WMKEYDOWN, OnKeyDown )
   ON_NOTIFY_REFLECT( DTN_FORMAT, OnFormat )
   ON_NOTIFY_REFLECT( DTN_FORMATQUERY, OnFormatQuery )
   ON_NOTIFY_REFLECT( DTN_DROPDOWN, OnDropDown )
   ON_NOTIFY_REFLECT( DTN_CLOSEUP, OnCloseUp )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_MOUSEMOVE( )

END_MESSAGE_MAP( )

#define zCALENDAR_DROPDOWN          zMAPACT_CTRL_SPECIFIC1

// ZCalendar - ctor
ZCalendar::ZCalendar( ZSubtask *pZSubtask,
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
                    "Calendar" )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::ctor ", *m_pzsTag );
#endif

   // Initialize date common controls.
   static zBOOL bInitCommonControls = FALSE;
   if ( bInitCommonControls == FALSE )
   {
      INITCOMMONCONTROLSEX iccx;
      iccx.dwSize = sizeof( INITCOMMONCONTROLSEX );
      iccx.dwICC = ICC_DATE_CLASSES;
      ::InitCommonControlsEx( &iccx );
      bInitCommonControls = TRUE;
   }

   m_bPermitNull = (pCtrlDef->Subtype & zCALENDAR_PERMIT_NULL) ? TRUE : FALSE;
   m_bShowCheckBox = (pCtrlDef->Subtype & zCALENDAR_CHECKBOX) ? TRUE : FALSE;
   m_bReportDateChangeWhileDropped = FALSE;
   m_ulMapActFlags &= ~zCALENDAR_DROPDOWN;
   m_CurrentTime = COleDateTime::GetCurrentTime( );

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style &= ~WS_BORDER;
   else
      Attr.Style |= WS_BORDER;

   Attr.Style |= WS_TABSTOP;
   if ( m_bShowCheckBox )
      Attr.Style |= DTS_SHOWNONE;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZCalendar::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::CreateZ ", *m_pzsTag );
#endif

   zLONG lStyleEx;

   if ( Attr.Style & WS_BORDER )
      lStyleEx = WS_EX_CLIENTEDGE;
   else
      lStyleEx = 0;

   Attr.Style |= WS_VISIBLE;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( lStyleEx, DATETIMEPICK_CLASS, *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
      if ( (m_ulMapActFlags & zMAPACT_VISIBLE) == 0 ) // don't know why this
         ShowWindow( FALSE );                         // is necessary, but it
      else                                            // is to initially show/
         ShowWindow( TRUE );                          // hide correctly
   }
   else
   {
#ifdef DEBUG_ALL
      zPCHAR pchMsg;
      ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
                                              FORMAT_MESSAGE_ALLOCATE_BUFFER,
                       0, 0x0000057F,  // GetLastError( ),
                       MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                       (zPCHAR) &pchMsg, 0, 0 );
      MessageSend( m_pZSubtask->m_vDialog, 0,
                   "ZCalendar::CreateZ", pchMsg,
                   zMSGQ_MODAL_ERROR, FALSE );

      // Free the buffer.
      LocalFree( pchMsg );
#endif
   }
#endif
}

int
ZCalendar::OnCreate( LPCREATESTRUCT lpcs )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnCreate ", *m_pzsTag );
#endif

   int nRC = CWnd::OnCreate( lpcs );
// if ( nRC == -1 )
//    return( -1 );

   return( nRC );
}

BOOL
ZCalendar::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::DestroyWindow ", *m_pzsTag );
#endif

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

void
ZCalendar::OnDateTimeChange( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnDateTimeChange", "" );
#endif
   if ( m_ulMapActFlags & zCALENDAR_RECURSIVECLICK )
      return;

   m_ulMapActFlags |= zCALENDAR_RECURSIVECLICK;
   ProcessImmediateEvent( this, zDTN_DATETIMECHANGE );
   m_ulMapActFlags &= ~zCALENDAR_RECURSIVECLICK;
}

#if 0
/////////////////////////////////////////////////////////////////////////////
// ZCalendar notification handlers
// This traps the DTN_DATETIMECHANGE notification and checks that
// the date/time has actually changed. If it has, then the message
// is passed on to the parent as normal, otherwise it is blocked.
void
ZCalendar::DoDateTimeChange( )
{
   TRACE0( "In OnDateTimeChange\n" );
}

BOOL
ZCalendar::OnDateTimeChange( NMDATETIMECHANGE *dtmh, LRESULT *pResult )
{
   *pResult = 0;   // docs say we must return( 0

   if ( m_bReportDateChangeWhileDropped == FALSE &&
        (m_ulMapActFlags & zCALENDAR_DROPDOWN) )
   {
      return( TRUE );
   }

   // Check box notification sent here.
// if ( dtmh->dwFlags == GDT_NONE )
//    TRACE0( "control Disabled\n" );
// else
//    TRACE0( "control Active\n" );

   // Has the date changed?
   BOOL bDateChanged = FALSE;
   if ( dtmh->dwFlags == GDT_VALID )
   {
      COleDateTime time;

      time = GetSystemTime( );
      if ( time != m_CurrentTime )
      {
         m_CurrentTime = time;
         bDateChanged = TRUE;
         DoDateTimeChange( );
      }
   }

   BOOL bUpDownStyle = ((GetStyle( ) & DTS_UPDOWN) == DTS_UPDOWN);
   if ( bUpDownStyle )
      return( FALSE );   // Let parent handle message

   // If the control is NOT a DTS_UPDOWN style (i.e. it has a drop down)
   // then we need to catch multiple notifications of date changes (this
   // can cause infinite loops).
   if ( m_ulMapActFlags & zCALENDAR_DROPDOWN )
   {
      if ( bDateChanged )
         return( TRUE );   // Don't let the parent see the message if not dropped down
      else
         return( FALSE );  // Let parent see message, since date hasn't changed
   }
   else
      return( FALSE );     // Let parent handle message
}
#endif

void
ZCalendar::OnUserString( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnUserString", "" );
#endif
   ProcessImmediateEvent( this, zDTN_USERSTRING );
   *pResult = 0;   // docs say we must return 0
}

void
ZCalendar::OnKeyDown( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnKeyDown", "" );
#endif
   ProcessImmediateEvent( this, zDTN_WMKEYDOWN );
   *pResult = 0;   // docs say we must return 0
}

void
ZCalendar::OnFormat( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnFormat", "" );
#endif
   ProcessImmediateEvent( this, zDTN_FORMAT );
   *pResult = 0;           // docs say we must return 0
}

void
ZCalendar::OnFormatQuery( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnFormatQuery", "" );
#endif
// lpDTFormatQuery->szMax = CSize( 366, 16 );   // must calculate Max time string size
   ProcessImmediateEvent( this, zDTN_FORMATQUERY );
   *pResult = 0;   // docs say we must return 0
}

void
ZCalendar::OnDropDown( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnDropDown", "" );
#endif
   m_ulMapActFlags |= zCALENDAR_DROPDOWN;
   ProcessImmediateEvent( this, zDTN_DROPDOWN );
}

void
ZCalendar::OnCloseUp( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::OnCloseUp", "" );
#endif
   m_ulMapActFlags &= ~zCALENDAR_DROPDOWN;

#if 0
   if ( m_bReportDateChangeWhileDropped == FALSE )
   {
      NMDATETIMECHANGE dtmh;
      dtmh.nmhdr.hwndFrom = GetSafeHwnd( );
      dtmh.nmhdr.idFrom = GetDlgCtrlID( );
      dtmh.nmhdr.code = DTN_DATETIMECHANGE;
      dtmh.dwFlags = DateTime_GetSystemtime( GetSafeHwnd( ), &(dtmh.st) );

      CWnd *pOwner = GetOwner( );
      if ( pOwner && IsWindow( pOwner->m_hWnd ) && GetSystemTime( ) != m_CurrentTime )
         pOwner->SendMessage( WM_NOTIFY, (WPARAM) GetDlgCtrlID( ), (LPARAM) &dtmh );
   }
#endif

   ProcessImmediateEvent( this, zDTN_CLOSEUP );
}

void
ZCalendar::SetDefaultDate( DWORD& dwGDT )
{
#ifdef zREMOTE_SERVER
   SetZCtrlText( this, "" );
#else
   COleDateTime DateTime = COleDateTime::GetCurrentTime( );

   if ( m_bPermitNull )
      ModifyStyle( 0, DTS_SHOWNONE );

   m_SysTime.wMilliseconds = 0;
   m_SysTime.wSecond = 0;
   m_SysTime.wMinute = 0;
   m_SysTime.wHour = 0;
   m_SysTime.wDay = 0;
   m_SysTime.wMonth = 0;
   m_SysTime.wYear = 0;

   m_SysTime.wDay = DateTime.GetDay( );
   m_SysTime.wMonth = DateTime.GetMonth( );
   m_SysTime.wYear = DateTime.GetYear( );
   m_SysTime.wDayOfWeek = DateTime.GetDayOfWeek( );

   // To keep from seeing last entered date, set the date fields twice.
   dwGDT = GDT_VALID;
   DateTime_SetSystemtime( m_hWnd, dwGDT, &m_SysTime );
   dwGDT = GDT_NONE;
   DateTime_SetSystemtime( m_hWnd, dwGDT, &m_SysTime );
#endif
}

zSHORT
ZCalendar::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW vApp;
      zCHAR  szDate[ 256 ];
      zUSHORT uMapState = GetMappingAutoGray( &vApp, *m_pzsVName,
                                              *m_pzsEName, *m_pzsAName );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) &&
           GetStringFromAttribute( szDate, sizeof( szDate ), vApp,
                                   *m_pzsEName, *m_pzsAName ) >= 0 )
      {
#ifdef zREMOTE_SERVER
         SetZCtrlText( this, szDate );
#else
         DWORD dwGDT;

         if ( SetSysTimeFromZeidon( m_SysTime, szDate ) )
            dwGDT = GDT_VALID;

         DateTime_SetSystemtime( m_hWnd, dwGDT, &m_SysTime );
#endif
         return( 0 );
      }
      else
      {
         DWORD dwGDT;

         ZCalendar::SetDefaultDate( dwGDT );
      }
   }

   return( -1 );
}

zSHORT
ZCalendar::SetRowColText( zCPCHAR cpcDate, zLONG lRow, zLONG lColumn )
{
   if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
   {
      DWORD dwGDT = GDT_NONE;

      if ( cpcDate == 0 || cpcDate[ 0 ] == 0 )
         SetDefaultDate( dwGDT );
      else
      {
         zCHAR szDate[ 20 ];
         COleDateTime date;
         zBOOL bRC;

         // We've got to get szDate to look like this:  YYYYMMDDhhmmssttt
         bRC = date.ParseDateTime( cpcDate );
         if ( bRC )
         {
            SetZeidonDateFromOleDateTime( szDate, sizeof( szDate ), date );
            if ( SetSysTimeFromZeidon( m_SysTime, szDate ) )
               dwGDT = GDT_VALID;
         }
         else
            SetDefaultDate( dwGDT );
      }

      DateTime_SetSystemtime( m_hWnd, dwGDT, &m_SysTime );
      m_pCtrl->SetWindowText( cpcDate );
      m_pCtrl->Invalidate( );  // to force repaint
   }

   return( 0 );
}

zSHORT
ZCalendar::GetRowColText( zPCHAR pchText, zUSHORT uMaxLth,
                          zLONG lRow, zLONG lColumn )
{
   DWORD dwGDT = DateTime_GetSystemtime( m_hWnd, &m_SysTime );

   if ( dwGDT == GDT_VALID )
   {
      return( ZMapAct::GetRowColText( pchText, uMaxLth, lRow, lColumn ) );
   }
   else
   {
      pchText[ 0 ] = 0;
      return( 0 );
   }
}

// Get the text for the entry control and map it to the data OI.
zSHORT
ZCalendar::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::MapToOI ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      zSHORT  nRC;
      zCHAR   szDate[ 256 ];

#ifdef zREMOTE_SERVER
      GetZCtrlText( this, szDate, sizeof( szDate ) );
#else
      DWORD dwGDT = DateTime_GetSystemtime( m_hWnd, &m_SysTime );

      if ( dwGDT == GDT_VALID )
      {
         if ( (lFlag & zVALIDATE) == zVALIDATE )
            return( 0 );

         SetZeidonDateFromSysTime( szDate, sizeof( szDate ), m_SysTime );
      }
      else
         szDate[ 0 ] = 0;
#endif

      nRC = SetAttributeFromString( vApp, *m_pzsEName, *m_pzsAName, szDate );
      return( nRC );
   }

   return( 0 );
}

void
ZCalendar::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
      Default( );             // pass it along to parent
}

void
ZCalendar::OnKillFocus( CWnd *pWndGetFocus )
{
   if ( (m_ulMapActFlags & zCALENDAR_DROPDOWN) == 0 )
      m_pZSubtask->InformDriverOfKillFocus( this );

   Default( );                // pass it along to parent
}

void
ZCalendar::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

void
ZCalendar::OnPaint( )
{
   CRect rect;
   GetClientRect( rect );
   InvalidateRect( rect );
   DWORD dwGDT = DateTime_GetSystemtime( m_hWnd, &m_SysTime );
   CWnd::OnPaint( );   // call default paint for dropdown, etc!

   if ( GetFocus( ) != this || dwGDT != GDT_VALID )
   {
   // GetUpdateRect( rect, FALSE );
   // CPaintDC dc( this ); // device context for painting
      CClientDC dc( this );
      int nSave = dc.SaveDC( );

      CString cs;
      GetWindowText( cs );
      dc.SelectObject( ::GetStockObject( DEFAULT_GUI_FONT ) );

      // If the date is set to null, display empty text!
      if ( dwGDT != GDT_VALID )
         cs.Empty( );
      else
      {
         // If the month or day are single digits, we need to add spaces to
         // the text to fill out the format (width of 2 spaces == 1 digit).
         int nPos = cs.Find( '/' );
         if ( nPos == 1 )
         {
            nPos += 2;
            cs.Insert( 0, "  " );
         }

         if ( cs.GetAt( nPos + 3 ) != '/' )
            cs.Insert( nPos + 1, "  " );
      }

      if ( mIs_hWndEnabled( m_hWnd ) == FALSE )
      {
      // TraceLineS( "ZCalendar disabled", "" );
         dc.SetBkColor( GetSysColor( COLOR_3DFACE ) );
         if ( m_ulMapActFlags & zMAPACT_DISABLE_READONLY )
            dc.SetTextColor( GetSysColor( COLOR_WINDOWTEXT ) );
         else
            dc.SetTextColor( GetSysColor( COLOR_GRAYTEXT ) );
      }
      else
      {
         dc.SetTextColor( GetSysColor( COLOR_WINDOWTEXT ) );
         dc.SetBkColor( GetSysColor( COLOR_WINDOW ) );
      }

      int k = ::GetSystemMetrics( SM_CXVSCROLL );
      int x = ::GetSystemMetrics( SM_CXBORDER );
      int y = ::GetSystemMetrics( SM_CYBORDER ) * 2;
      if ( m_bShowCheckBox )
      {
         rect.left += k; // - x;  // make the calendar ctrl 15 units high
                                  // to make it so that the repaint of the
                                  // digits does not shift
      }
      else
      {
      // CSize size = dc.GetTextExtent( "0123456789" ); // in logical units
      // size.cx /= 10;
         x *= 2;
         rect.left += x;
      // if ( m_SysTime.wMonth < 10 )
      //    rect.left += size.cx + x;
      // else
      //    rect.left += x;
      }

      rect.right -= k;
      if ( rect.right < rect.left )
         rect.right = rect.left;

      rect.top += y;
      dc.FillRect( rect, &CBrush( dc.GetBkColor( ) ) );
   // TraceLineS( "Calendar paint text: ", cs );
      dc.DrawText( cs, rect,
                   DT_SINGLELINE | DT_NOPREFIX | DT_TOP ); // DT_VCENTER );

      dc.RestoreDC( nSave );
   }
}

HBRUSH
//ZCalendar::CtlColor( CDC *pDC, UINT uCtlColor )
ZCalendar::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlColor )
{
   HBRUSH hBrush = 0; // I hope this is OK. It doesn't seem to cause problems
#if 0
   if ( m_clrText )
      pDC->SetTextColor( m_clrText );

   if ( m_pBrush )
   {
      hBrush = (HBRUSH) *m_pBrush;
      pDC->SetBkColor( m_clrBk );
   }
#endif
   pDC->SetTextColor( zRGB( 255, 0, 0 ) );
   return( hBrush );
}

void
ZCalendar::OnEnable( BOOL bEnable )
{
// if ( zstrcmp( "CBAdmissionsTrack", *m_pzsTag ) == 0 )
//    TraceLineS( "", "" );

   CWnd::OnEnable( bEnable );

   // Possible values for nColIndex:
   // MCSC_BACKGROUND   0   // the background color (between months)
   // MCSC_TEXT         1   // the dates
   // MCSC_TITLEBK      2   // background of the title
   // MCSC_TITLETEXT    3
   // MCSC_MONTHBK      4   // background within the month cal
   // MCSC_TRAILINGTEXT 5   // the text color of header & trailing days

   COLORREF clr = SetMonthCalColor( 3, zRGB( 255, 0, 0 ) );
}

zLONG
ZCalendar::SetZCtrlState( zLONG  lStatusType,
                          zULONG ulValue )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "FirstName", *m_pzsTag ) == 0 )
      TraceLineS( "ZCalendar::SetZCtrlState: ", *m_pzsTag );
#endif

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

// ZCalendar - dtor
ZCalendar::~ZCalendar( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::dtor ", *m_pzsTag );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//
// MFC Wrapper class for the IE3 DateTimePicker control
// Check out http://www.microsoft.com/msdn/sdk/inetsdk/help/itt/CommCtls/DateTime/DateTime.htm#ch_dtp_make
// for details.
//
// To use in a dialog:
//
//   Create a custom control with classname "SysDateTimePick32"
//
// You will then need to subclass the custom control ( in the InitDialog fn ).
// e.g.
//
//   m_DateTime.SubclassDlgItem( IDC_CUSTOM1, this );
//
// Instead of subclassing, you can implement a DDX_Control( pDX, IDC_DTPICK1, m_DTPick1 )
// in you DoDataExchange( CDataExchange *pDX ) function. In either case, if you do not use the
// Create( ) function (see below), set the Style in the Properties of your control to
// the combination of the DTS_.. values that you require. Typically, for a Calender display,
// dwStyle = 0x50010000, for a time control, dwStyle = 0x50010009.
//
//  To use if creating with "Create":
//      call "ZCalendar::Create( dwStyle, rect, pParentWnd, nID )"
//
//  DateTime styles:
//
//        DTS_UPDOWN          - use UPDOWN instead of MONTHCAL
//        DTS_SHOWNONE        - allow a NONE selection
//        DTS_SHORTDATEFORMAT - use the short date format (app must forward WM_WININICHANGE messages)
//        DTS_LONGDATEFORMAT  - use the long date format (app must forward WM_WININICHANGE messages)
//        DTS_TIMEFORMAT      - use the time format (app must forward WM_WININICHANGE messages)
//        DTS_APPCANPARSE     - allow user entered strings (app MUST respond to DTN_USERSTRING)
//        DTS_RIGHTALIGN      - right-align popup instead of left-align it
//
//  For the ZCalendar::SetFormat( LPCTSTR szFmt ) specs, see the functions GetDateFormat
//  and GetTimeFormat for the format of szFmt.
//
//  For the Set- and Get-Color functions, use the following for the nColIndex:
//
//        MCSC_BACKGROUND   0   // the background color (between months)
//        MCSC_TEXT         1   // the dates
//        MCSC_TITLEBK      2   // background of the title
//        MCSC_TITLETEXT    3   // title text
//        MCSC_MONTHBK      4   // background within the month cal
//        MCSC_TRAILINGTEXT 5   // the text color of header & trailing days
//
// Written by Chris Maunder (Chris.Maunder@cbr.clw.csiro.au)
// Copyright (c) 1998.
//
// Modified 7 Feb 1998 - bug fix in OnChildNotify (Jim Richardson)
//                     - Added COleDateTime support
//          4 Sep 1998 - Made Windows CE Compatible. By Gerrit Hoekstra
//
// This code is free. If the source code in this file is used in any
// commercial application then an email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to anything
//
//
// Expect bugs.
//
// Please use and enjoy. Please let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into this
// file.
//
/////////////////////////////////////////////////////////////////////////////

#if 0
// Thanks to Tim Gilman (tdgilman@best.com) for this.
void AFXAPI DDX_DateTime( CDataExchange *pDX, int nIDC, SYSTEMTIME &SysTime )
{
   HWND hWndCtrl = pDX->PrepareCtrl( nIDC );
   ASSERT ( hWndCtrl );

   if ( pDX->m_bSaveAndValidate )
   {
      ::SendMessage( hWndCtrl, DTM_GETSYSTEMTIME, 0, (LPARAM) &SysTime );
   }
   else // initializing
   {
      ::SendMessage( hWndCtrl, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM) &SysTime );
   }
}
#endif

/////////////////////////////////////////////////////////////////////////////
// ZCalendar

//BOOL ZCalendar::bCommonControlsInitialized = FALSE;

/////////////////////////////////////////////////////////////////////////////
// ZCalendar (public) operations

CTime
ZCalendar::GetDateTime( )
{
   SYSTEMTIME SysTime;

   SysTimeFromCOleDateTime( m_CurrentTime, &SysTime );
   return( CTime( SysTime ) );
}

BOOL
ZCalendar::SetDateTime( const CTime& time )
{
   SYSTEMTIME SysTime;
   SysTimeFromCTime( time, &SysTime );

   m_CurrentTime = SysTime;
   return( SetSystemTime( SysTime ) );
}

COleDateTime
ZCalendar::GetOleDateTime( )
{
   return( m_CurrentTime );
}

BOOL
ZCalendar::SetDateTime( const COleDateTime& time )
{
   m_CurrentTime = time;
   return( SetSystemTime( time ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZCalendar (protected) implementation

void
ZCalendar::SysTimeFromCTime( const CTime& time, SYSTEMTIME *pSysTime )
{
   pSysTime->wYear   = (WORD) time.GetYear( );
   pSysTime->wMonth  = (WORD) time.GetMonth( );
   pSysTime->wDay    = (WORD) time.GetDay( );
   pSysTime->wHour   = (WORD) time.GetHour( );
   pSysTime->wMinute = (WORD) time.GetMinute( );
   pSysTime->wSecond = (WORD) time.GetSecond( );
   pSysTime->wMilliseconds = 0;
}

BOOL
ZCalendar::SetSystemTime( const SYSTEMTIME& time )
{
   return( DateTime_SetSystemtime( m_hWnd, GDT_VALID, &time ) );
}

BOOL
ZCalendar::SetSystemTime( const CTime& time )
{
   SYSTEMTIME SysTime;
   SysTimeFromCTime( time, &SysTime );
   return( DateTime_SetSystemtime( m_hWnd, GDT_VALID, &SysTime ) );
}

void
ZCalendar::SysTimeFromCOleDateTime( const COleDateTime& time, SYSTEMTIME *pSysTime )
{
   pSysTime->wYear   = (WORD) time.GetYear( );
   pSysTime->wMonth  = (WORD) time.GetMonth( );
   pSysTime->wDay    = (WORD) time.GetDay( );
   pSysTime->wHour   = (WORD) time.GetHour( );
   pSysTime->wMinute = (WORD) time.GetMinute( );
   pSysTime->wSecond = (WORD) time.GetSecond( );
   pSysTime->wMilliseconds = 0;
}

BOOL
ZCalendar::SetSystemTime( const COleDateTime& time )
{
   SYSTEMTIME SysTime;
   SysTimeFromCOleDateTime( time, &SysTime );
   return( DateTime_SetSystemtime( m_hWnd, GDT_VALID, &SysTime ) );
}

COleDateTime
ZCalendar::GetSystemTime( )
{
   SYSTEMTIME SysTime;
   if ( DateTime_GetSystemtime( m_hWnd, &SysTime ) == GDT_VALID )
      return( COleDateTime( SysTime ) );
   else
      return( COleDateTime::GetCurrentTime( ) );
}

// See GetDateFormat and GetTimeFormat for the format of szFmt.
BOOL
ZCalendar::SetFormat( LPCTSTR szFmt )
{
   return( DateTime_SetFormat( m_hWnd, szFmt ) );
}

// Possible values for nColIndex:
// MCSC_BACKGROUND   0   // the background color (between months)
// MCSC_TEXT         1   // the dates
// MCSC_TITLEBK      2   // background of the title
// MCSC_TITLETEXT    3
// MCSC_MONTHBK      4   // background within the month cal
// MCSC_TRAILINGTEXT 5   // the text color of header & trailing days

COLORREF
ZCalendar::SetMonthCalColor( int nColIndex, COLORREF clr )
{
   return( DateTime_SetMonthCalColor( m_hWnd, nColIndex, clr ) );
}

COLORREF
ZCalendar::GetMonthCalColor( int nColIndex )
{
   return( DateTime_GetMonthCalColor( m_hWnd, nColIndex ) );
}

void
ZCalendar::SetMonthCalFont( CFont& font, BOOL bRedraw /*=TRUE*/ )
{
   DateTime_SetMonthCalFont( m_hWnd, (HFONT) font, bRedraw );
}

CFont *
ZCalendar::GetMonthCalFont( )
{
   return( CFont::FromHandle( (HFONT) DateTime_GetMonthCalFont( m_hWnd ) ) );
}

// Returns TRUE if success for both times, FALSE otherwise.
BOOL
ZCalendar::SetRange( CTime *pMinTime, CTime *pMaxTime )
{
   SYSTEMTIME SysTimeRange[ 2 ];
   int nWhich = 0;

   if ( pMinTime )
   {
      nWhich |= GDTR_MIN;
      SysTimeFromCTime( *pMinTime, &(SysTimeRange[ 0 ]) );
   }

   if ( pMaxTime )
   {
      nWhich |= GDTR_MAX;
      SysTimeFromCTime( *pMaxTime, &(SysTimeRange[ 1 ]) );
   }

   return( DateTime_SetRange( m_hWnd, nWhich, SysTimeRange ) );
}

// returns GDTR_MIN|GDTR_MAX if there is a minimum|maximum limit
DWORD
ZCalendar::GetRange( CTime *pMinTime, CTime *pMaxTime )
{
   SYSTEMTIME SysTimeRange[ 2 ];
   DWORD result = DateTime_GetRange( m_hWnd, SysTimeRange );

   *pMinTime = CTime( SysTimeRange[ 0 ] );   // systime is 0 if no limit
   *pMaxTime = CTime( SysTimeRange[ 1 ] );

   return( result );
}

// Returns TRUE if success for both times, FALSE otherwise
BOOL
ZCalendar::SetRange( COleDateTime *pMinTime, COleDateTime *pMaxTime )
{
   SYSTEMTIME SysTimeRange[ 2 ];
   int nWhich = 0;

   if ( pMinTime )
   {
      nWhich |= GDTR_MIN;
      SysTimeFromCOleDateTime( *pMinTime, &(SysTimeRange[ 0 ]) );
   }

   if ( pMaxTime )
   {
      nWhich |= GDTR_MAX;
      SysTimeFromCOleDateTime( *pMaxTime, &(SysTimeRange[ 1 ]) );
   }

   return( DateTime_SetRange( m_hWnd, nWhich, SysTimeRange ) );
}

// returns GDTR_MIN|GDTR_MAX if there is a minimum | maximum limit
DWORD
ZCalendar::GetRange( COleDateTime *pMinTime, COleDateTime *pMaxTime )
{
   SYSTEMTIME SysTimeRange[2];
   DWORD result = DateTime_GetRange( m_hWnd, SysTimeRange );

   *pMinTime = COleDateTime( SysTimeRange[ 0 ] );   // systime is 0 if no limit
   *pMaxTime = COleDateTime( SysTimeRange[ 1 ] );

   return( result );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCalendarWeekdayWnd

BEGIN_MESSAGE_MAP( CCalendarWeekdayWnd, CWnd )
   //{{AFX_MSG_MAP( CCalendarWeekdayWnd )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_MOUSEMOVE( )
   ON_WM_PAINT( )
   ON_WM_ERASEBKGND( )
   ON_WM_SIZE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

int
GetDaysInMonth( int nMonth, int nYear )
{
   static int nMonthLen[ ] =
   {
      31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
   };

   int nDays = nMonthLen[ nMonth - 1 ];
   if ( nMonth == 2 && nYear % 4 == 0 &&
        (nYear % 100 != 0 || nYear % 400 == 0) )
   {
      nDays = 29;
   }

   return( nDays );
}

CString CCalendarWeekdayWnd::m_csClassName = _T( "" );

CCalendarWeekdayWnd::CCalendarWeekdayWnd( ) :
                     m_pParentCalendar( 0 ),
                     m_nTrackedWeekday( zNO_HIT ),
                     m_nPressedWeekday( zNO_HIT ),
                     m_bMultiEnabled( FALSE ),
                     m_nFirstDayOfWeek( 0 )
{
   if ( m_csClassName.IsEmpty( ) )
      m_csClassName = ::AfxRegisterWndClass( 0 );

   for ( zSHORT nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
      m_WeekdayLabels[ nWeekday ] = 0;

   COleDateTime date = COleDateTime::GetCurrentTime( );
   m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );
}

CCalendarWeekdayWnd::~CCalendarWeekdayWnd( )
{
   for ( zSHORT nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
      mDeleteInitA( m_WeekdayLabels[ nWeekday ] );
}

BOOL
CCalendarWeekdayWnd::Create( const RECT& rect, CWnd *pParentWnd, UINT uID )
{
   ASSERT_KINDOF( CMultiSelCal, pParentWnd );
   m_pParentCalendar = (CMultiSelCal *) pParentWnd;

   return( CWnd::Create( m_csClassName, _T( "CalCaption" ),
                         WS_CHILD | WS_VISIBLE, rect, pParentWnd, uID ) );
}

void
CCalendarWeekdayWnd::OnLButtonDown( UINT uFlags, CPoint pt )
{
   ASSERT( m_pParentCalendar );
   m_pParentCalendar->SetFocus( );

   m_nTrackedWeekday = m_nPressedWeekday = WeekdayHitTest( pt );
   if ( m_nTrackedWeekday != zNO_HIT )
   {
      InvalidateDay( m_nTrackedWeekday );
      UpdateWindow( );
   }
}

void
CCalendarWeekdayWnd::OnLButtonUp( UINT uFlags, CPoint pt )
{
   ASSERT( m_pParentCalendar );

   ReleaseCapture( );

   m_nTrackedWeekday = WeekdayHitTest( pt );
   if ( m_nTrackedWeekday != zNO_HIT &&
        m_nTrackedWeekday == m_nPressedWeekday )
   {
      m_pParentCalendar->SelectDayInWeek( m_nTrackedWeekday + 1,
                                          (uFlags & MK_CONTROL) == 0 );
   }

   int nRedraw1 = m_nPressedWeekday;
   int nRedraw2 = m_nTrackedWeekday;

   m_nPressedWeekday = zNO_HIT;
   m_nTrackedWeekday = zNO_HIT;

   if ( nRedraw1 != zNO_HIT )
   {
      InvalidateDay( nRedraw1 );
   }

   if ( nRedraw2 != zNO_HIT )
   {
      InvalidateDay( nRedraw2 );
   }

   UpdateWindow( );

   CWnd::OnLButtonUp( uFlags, pt );
}

void
CCalendarWeekdayWnd::OnMouseMove( UINT uFlags, CPoint pt )
{
   int nPrevTrackWeekday = m_nTrackedWeekday;
   m_nTrackedWeekday = WeekdayHitTest( pt );

   if ( m_nTrackedWeekday != nPrevTrackWeekday )
   {
      zBOOL bNeedUpdate = FALSE;

      if ( nPrevTrackWeekday != zNO_HIT &&
           (m_nPressedWeekday == zNO_HIT ||
            nPrevTrackWeekday == m_nPressedWeekday) )
      {
         InvalidateDay( nPrevTrackWeekday );
         bNeedUpdate = TRUE;
      }

      if ( m_nTrackedWeekday != zNO_HIT &&
           (m_nPressedWeekday == zNO_HIT ||
            m_nTrackedWeekday == m_nPressedWeekday) )
      {
         InvalidateDay( m_nTrackedWeekday );
         bNeedUpdate = TRUE;
      }

      if ( bNeedUpdate )
      {
         UpdateWindow( );
      }

      if ( m_nTrackedWeekday != zNO_HIT )
      {
         if ( nPrevTrackWeekday == zNO_HIT )
         {
            SetCapture( );
         }
      }
      else
      {
         if ( m_nPressedWeekday == zNO_HIT )
         {
            ReleaseCapture( );
         }
      }
   }

   CWnd::OnMouseMove( uFlags, pt );
}

BOOL
CCalendarWeekdayWnd::OnEraseBkgnd( CDC *pDC )
{
   return( TRUE );
}

void
CCalendarWeekdayWnd::OnPaint( )
{
   CPaintDC dc( this ); // device context for painting

   CRect rectClient;
   GetClientRect( &rectClient );

   dc.SetBkMode( TRANSPARENT );
   dc.SelectStockObject( DEFAULT_GUI_FONT );

   // Draw title (calendar's highlighted month):
   CRect rectTitle = rectClient;
   rectTitle.bottom = rectTitle.top + rectClient.Height( ) / 2;

   dc.FillSolidRect( &rectTitle, ::GetSysColor( COLOR_ACTIVECAPTION ) );
   dc.Draw3dRect( &rectTitle,
                  ::GetSysColor( COLOR_3DHILIGHT ),
                  ::GetSysColor( COLOR_3DDKSHADOW ) );

   CString csTitle = m_dateToday.Format( _T( "%B %Y" ) );
   dc.SetTextColor( ::GetSysColor( COLOR_CAPTIONTEXT ) );
   dc.DrawText( csTitle, rectTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER );

   // Draw week days row:
   int nDayCaptionsHeight = rectClient.Height( ) / 2;

   CRect rectDays = rectClient;
   rectDays.top = rectTitle.bottom + 1;

   if ( m_bMultiEnabled )
   {
      dc.FillSolidRect( &rectDays, ::GetSysColor( COLOR_3DFACE ) );
      dc.SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );
   }
   else
   {
      dc.FillSolidRect( &rectDays, ::GetSysColor( COLOR_INACTIVECAPTION ) );
      dc.SetTextColor( ::GetSysColor( COLOR_INACTIVECAPTIONTEXT ) );
   }

   for ( int nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
   {
      CRect rect = m_rectWeekdays[ nWeekday ];
      zBOOL bPressed = (nWeekday == m_nPressedWeekday);
      zBOOL bTracked = (nWeekday == m_nTrackedWeekday);

      if ( bPressed && bTracked )
      {
         rect.OffsetRect( 1, 1 );
      }

      if ( dc.RectVisible( rect ) )
      {
         dc.DrawText( m_WeekdayLabels[ nWeekday ], rect,
                      DT_SINGLELINE | DT_VCENTER | DT_CENTER );

         if ( m_bMultiEnabled )
         {
            // Draw 3D border for the tracked or pressed buttons:
            if ( bTracked || bPressed )
            {
               rect.InflateRect( 0, -1 );
               dc.Draw3dRect( &rect,
                              bPressed && bTracked ?
                                ::GetSysColor( COLOR_3DDKSHADOW ) :
                                ::GetSysColor( COLOR_3DHILIGHT ),
                              bPressed && bTracked ?
                                ::GetSysColor( COLOR_3DHILIGHT ) :
                                ::GetSysColor( COLOR_3DDKSHADOW ) );
            }
         }
      }
   }
}

void
CCalendarWeekdayWnd::OnSize( UINT uType, int cx, int cy )
{
   CWnd::OnSize( uType, cx, cy );
   int nWeekday;

   CRect rectClient;
   GetClientRect( &rectClient );

   int nDayWidth = (cx - 20) / zDAYS_IN_WEEK;

   // Define button locations:
   for ( nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
   {
      m_rectWeekdays[ nWeekday ] = rectClient;

      m_rectWeekdays[ nWeekday ].top += cy / 2 + 1;
      m_rectWeekdays[ nWeekday ].bottom--;
      m_rectWeekdays[ nWeekday ].left =
                                rectClient.left + nDayWidth * nWeekday + 10;
      m_rectWeekdays[ nWeekday ].right =
                                m_rectWeekdays[ nWeekday ].left + nDayWidth;
   }

   m_rectWeekdays[ 0 ].left = rectClient.left;
   m_rectWeekdays[ zDAYS_IN_WEEK - 1 ].right = rectClient.right;

   // Define labels:
   CClientDC dc( this );
   dc.SelectStockObject( DEFAULT_GUI_FONT );

   COleDateTimeSpan day( 1, 0, 0, 0 );
   int    nMaxTextLength = nDayWidth - 4;
   zPCHAR pch;
   zLONG  lLth;

   for ( nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
   {
   // COleDateTime dateTmp = COleDateTime::GetCurrentTime( );
      COleDateTime dateTmp = m_dateToday;
      int nDay = (nWeekday + m_nFirstDayOfWeek) % zDAYS_IN_WEEK;

      while ( dateTmp.GetDayOfWeek( ) != nDay + 1 )
      {
         dateTmp += day;
      }

      CString csWeekdayName = dateTmp.Format( _T( "%a" ) );

      while ( csWeekdayName.GetLength( ) > 0 &&
              dc.GetTextExtent( csWeekdayName ).cx > nMaxTextLength )
      {
         csWeekdayName = csWeekdayName.Left( csWeekdayName.GetLength( ) - 1 );
      }

      mDeleteInitA( m_WeekdayLabels[ nWeekday ] );
      lLth = csWeekdayName.GetLength( ) + 1;
      pch = new char[ lLth ];
      strcpy_s( pch, lLth, csWeekdayName );
      m_WeekdayLabels[ nWeekday ] = pch;
   }
}

int
CCalendarWeekdayWnd::WeekdayHitTest( CPoint& pt ) const
{
   if ( m_bMultiEnabled )
   {
      for ( int nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
      {
         if ( m_rectWeekdays[ nWeekday ].PtInRect( pt ) )
         {
            return( nWeekday );
         }
      }
   }

   return( zNO_HIT );
}

void
CCalendarWeekdayWnd::InvalidateDay( int nDay )
{
   if ( nDay < 1 || nDay > zDAYS_IN_WEEK )
   {
      return;
   }

   CRect rect = m_rectWeekdays[ nDay - 1 ];
   rect.InflateRect( 2, 2 );

   InvalidateRect( rect );
}

void
CCalendarWeekdayWnd::SetDate( const COleDateTime& date )
{
   m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );
   if ( mIs_hWnd( m_hWnd ) )
   {
      Invalidate( );
      UpdateWindow( );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCalendarScrollbarWnd

BEGIN_MESSAGE_MAP( CCalendarScrollbarWnd, CWnd )
   //{{AFX_MSG_MAP( CCalendarScrollbarWnd )
   ON_WM_CREATE( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_MOUSEMOVE( )
   ON_WM_PAINT( )
   ON_WM_ERASEBKGND( )
   ON_WM_SIZE( )
   ON_WM_TIMER( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CString CCalendarScrollbarWnd::m_csClassName = _T( "" );

CCalendarScrollbarWnd::CCalendarScrollbarWnd( ) :
                     m_clrBack( ::GetSysColor( COLOR_3DFACE ) ),
                     m_clrLight( ::GetSysColor( COLOR_3DHILIGHT ) ),
                     m_clrShadow( ::GetSysColor( COLOR_3DDKSHADOW ) ),
                     m_pParentCalendar( 0 ),
                     m_nTrackedScrollButton( zNO_HIT ),
                     m_nPressedScrollButton( zNO_HIT ),
                     m_nWindowHeight( 0 ),
                     m_nButtonHeight( 0 )
{
   if ( m_csClassName.IsEmpty( ) )
      m_csClassName = ::AfxRegisterWndClass( 0 );
}

CCalendarScrollbarWnd::~CCalendarScrollbarWnd( )
{
}

BOOL
CCalendarScrollbarWnd::Create( const RECT& rect, CWnd *pParentWnd, UINT uID )
{
   ASSERT_KINDOF( CMultiSelCal, pParentWnd );
   m_pParentCalendar = (CMultiSelCal *) pParentWnd;

   return( CWnd::Create( m_csClassName, _T( "CalBar" ),
                         WS_CHILD | WS_VISIBLE, rect, pParentWnd, uID ) );
}

int
CCalendarScrollbarWnd::OnCreate( LPCREATESTRUCT lpcs )
{
   int nRC = CWnd::OnCreate( lpcs );
   if ( nRC == -1 )
      return( -1 );

   // Load images list:
   BOOL bRC = m_Images.Create( IDB_CALENDAR_BUTTONS,
                               g_nImageWidth, 0, g_clrTransparent );
   if ( bRC == FALSE )
   {
      TraceLineS( "CCalendarScrollbarWnd: Can't load images list!", "" );
      return( -1 );
   }

   // Define image size:
   IMAGEINFO imageInfo;
   m_Images.GetImageInfo( 0, &imageInfo );

   CRect rectImage = imageInfo.rcImage;
   m_ImageSize = rectImage.Size( );

   // Define text height:
   CClientDC dc( this );
   CFont *pOldFont = (CFont *) dc.SelectStockObject( DEFAULT_GUI_FONT );

   TEXTMETRIC tm;
   dc.GetTextMetrics( &tm );

   dc.SelectObject( pOldFont );

   // Calculate buttons height:
   m_nButtonHeight = max( m_ImageSize.cy * 2, tm.tmHeight );
   m_nWindowHeight = 3 * m_nButtonHeight / 2 + 2;

   // Load "Today" text:
   m_csTodayLabel.LoadString( IDS_TODAY );

   // Create and adjust tooltip:
   CRect rectEmpty( 0, 0, 0, 0 ); // will be adjusted in "OnSize"

   if ( m_ToolTip.Create( this, TTS_ALWAYSTIP ) &&
        m_ToolTip.AddTool( this, IDS_PREV_YEAR, &rectEmpty, 1 ) &&
        m_ToolTip.AddTool( this, IDS_PREV_MONTH, &rectEmpty, 2 ) &&
        m_ToolTip.AddTool( this, IDS_NEXT_MONTH, &rectEmpty, 3 ) &&
        m_ToolTip.AddTool( this, IDS_NEXT_YEAR, &rectEmpty, 4 ) )
   {
      m_ToolTip.SendMessage( TTM_SETMAXTIPWIDTH, 0, SHRT_MAX );
      m_ToolTip.SendMessage( TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX );
      m_ToolTip.SendMessage( TTM_SETDELAYTIME, TTDT_INITIAL, 500 );
      m_ToolTip.SendMessage( TTM_SETDELAYTIME, TTDT_RESHOW, 500 );
   }
   else
   {
      TRACE( _T( "CCalendarScrollbarWnd: Error in creating ToolTip" ) );
   }

   return( nRC );
}

void
CCalendarScrollbarWnd::OnLButtonDown( UINT uFlags, CPoint pt )
{
   ASSERT( m_pParentCalendar );
   m_pParentCalendar->SetFocus( );

   RelayEvent( WM_LBUTTONDOWN, (WPARAM) uFlags,
               MAKELPARAM( LOWORD( pt.x ), LOWORD( pt.y ) ) );

   m_nTrackedScrollButton = m_nPressedScrollButton = ScrollButtonHitTest( pt );
   if ( m_nTrackedScrollButton != zNO_HIT )
   {
      InvalidateRect( m_rectScrollButtons[ m_nTrackedScrollButton ] );
      UpdateWindow( );

      if ( m_nTrackedScrollButton != eToday )
      {
         m_bSlowTimerMode = TRUE;
         m_nSlowTimerCount = 0;
         SetTimer( g_nIdClickButtonEvt, g_nSlowTimerDuration, 0 );
      }
   }

   CWnd::OnLButtonDown( uFlags, pt );
}

void
CCalendarScrollbarWnd::OnLButtonUp( UINT uFlags, CPoint pt )
{
   KillTimer( g_nIdClickButtonEvt );
   ReleaseCapture( );

   RelayEvent( WM_LBUTTONUP, (WPARAM) uFlags,
               MAKELPARAM( LOWORD( pt.x ), LOWORD( pt.y ) ) );

   m_nTrackedScrollButton = ScrollButtonHitTest( pt );
   if ( m_nTrackedScrollButton == m_nPressedScrollButton &&
        m_nTrackedScrollButton != zNO_HIT )
   {
      OnClickScrollButton( m_nTrackedScrollButton );
   }

   int nRedraw1 = m_nPressedScrollButton;
   int nRedraw2 = m_nTrackedScrollButton;

   m_nPressedScrollButton = zNO_HIT;
   m_nTrackedScrollButton = zNO_HIT;

   if ( nRedraw1 != zNO_HIT )
   {
      InvalidateRect( m_rectScrollButtons[ nRedraw1 ] );
   }

   if ( nRedraw2 != zNO_HIT )
   {
      InvalidateRect( m_rectScrollButtons[ nRedraw2 ] );
   }

   UpdateWindow( );

   CWnd::OnLButtonUp( uFlags, pt );
}

void
CCalendarScrollbarWnd::OnMouseMove( UINT uFlags, CPoint pt )
{
   RelayEvent( WM_MOUSEMOVE, (WPARAM) uFlags,
               MAKELPARAM( LOWORD( pt.x ), LOWORD( pt.y ) ) );

   int nPrevTrackScrollButton = m_nTrackedScrollButton;
   m_nTrackedScrollButton = ScrollButtonHitTest( pt );

   if ( m_nTrackedScrollButton != nPrevTrackScrollButton )
   {
      zBOOL bNeedUpdate = FALSE;

      if ( (m_nPressedScrollButton == zNO_HIT ||
            nPrevTrackScrollButton == m_nPressedScrollButton) &&
           nPrevTrackScrollButton != zNO_HIT )
      {
         InvalidateRect( m_rectScrollButtons[ nPrevTrackScrollButton ] );
         bNeedUpdate = TRUE;
      }

      if ( (m_nPressedScrollButton == zNO_HIT ||
            m_nTrackedScrollButton == m_nPressedScrollButton) &&
           m_nTrackedScrollButton != zNO_HIT )
      {
         InvalidateRect( m_rectScrollButtons[ m_nTrackedScrollButton ] );
         bNeedUpdate = TRUE;
      }

      if ( bNeedUpdate )
      {
         UpdateWindow( );
      }

      if ( m_nTrackedScrollButton != zNO_HIT )
      {
         if ( nPrevTrackScrollButton == zNO_HIT )
         {
            SetCapture( );
         }
      }
      else
      {
         if ( m_nPressedScrollButton == zNO_HIT )
         {
            ReleaseCapture( );
         }
      }
   }

   CWnd::OnMouseMove( uFlags, pt );
}

BOOL
CCalendarScrollbarWnd::OnEraseBkgnd( CDC *pDC )
{
   CRect rectClient;
   GetClientRect( &rectClient );

   pDC->FillSolidRect( &rectClient, m_clrBack );
   return( TRUE );
}

void
CCalendarScrollbarWnd::OnPaint( )
{
   CPaintDC dc( this ); // device context for painting

   CRect rectClient;
   GetClientRect( &rectClient );

   for ( int k = 0; k < zMINICAL_BUTTON_IMAGE_CNT; k++ )
   {
      CRect rect = m_rectScrollButtons[ k ];

      if ( dc.RectVisible( rect ) )
      {
         zBOOL bPressed = (k == m_nPressedScrollButton);
         zBOOL bTracked = (k == m_nTrackedScrollButton);

         if ( k == eToday )
         {
            // Draw button text ("Today" button only):
            dc.SetBkMode( TRANSPARENT );
            dc.SelectStockObject( DEFAULT_GUI_FONT );
            dc.SetTextColor( bTracked ?
                               ::GetSysColor( COLOR_HIGHLIGHT ) : m_clrText );

            CRect rectText = rect;
            if ( bPressed && bTracked )
            {
               rectText.OffsetRect( 1, 1 );
            }

            dc.DrawText( m_csTodayLabel, rectText,
                         DT_SINGLELINE | DT_VCENTER | DT_CENTER );
         }
         else
         {
            // Draw button image (except "Today" button):
            CPoint ptImage( rect.left + (rect.Width( ) - m_ImageSize.cx) / 2,
                            rect.top + (rect.Height( ) - m_ImageSize.cy) / 2 );

            if ( bPressed && bTracked )
               ptImage.Offset( 1, 1 );

            m_Images.Draw( &dc, bTracked ? k + zMINICAL_BUTTON_IMAGE_CNT : k,
                           ptImage, ILD_NORMAL );
         }

         // Draw 3D border for the tracked or pressed buttons:
         if ( bTracked || bPressed )
         {
            dc.Draw3dRect( &rect,
                           bPressed && bTracked ? m_clrShadow : m_clrLight,
                           bPressed && bTracked ? m_clrLight : m_clrShadow );
         }
      }
   }

   // Draw horizontal line on the top of window:
   CPen pen1( PS_SOLID, 1, m_clrLight );
   CPen *pPenPrev = (CPen *) dc.SelectObject( &pen1 );

   int y = rectClient.top + 1;

   int x1 = rectClient.left + g_nHorzOffset;
   int x2 = rectClient.right - g_nHorzOffset;

   dc.MoveTo( x1, y );
   dc.LineTo( x2, y );

   CPen pen2( PS_SOLID, 1, m_clrShadow );
   dc.SelectObject( &pen2 );

   dc.MoveTo( x1, y - 1 );
   dc.LineTo( x2, y - 1 );

   dc.SelectObject( pPenPrev );
}

void
CCalendarScrollbarWnd::OnSize( UINT uType, int cx, int cy )
{
   CWnd::OnSize( uType, cx, cy );

   CRect rectClient;
   GetClientRect( &rectClient );
   rectClient.top += 2; // Leave space for horiz.line

   int nBtnWidth = m_ImageSize.cy * 2; // Expect TODAY button
   int yOffset = (rectClient.Height( ) - m_nButtonHeight) / 2;

   // Calculate button rectangles:
   for ( int k = 0; k < zMINICAL_BUTTON_IMAGE_CNT; k++ )
   {
      m_rectScrollButtons[ k ].top = rectClient.top + yOffset;
      m_rectScrollButtons[ k ].bottom =
                            m_rectScrollButtons[ k ].top + m_nButtonHeight;
   }

   m_rectScrollButtons[ ePrevYear ].left = rectClient.left + g_nHorzOffset;
   m_rectScrollButtons[ ePrevYear ].right =
                         m_rectScrollButtons[ ePrevYear ].left + nBtnWidth;

   m_rectScrollButtons[ ePrevMonth ].left =
                         m_rectScrollButtons[ ePrevYear ].right + 1;
   m_rectScrollButtons[ ePrevMonth ].right =
                         m_rectScrollButtons[ ePrevMonth ].left + nBtnWidth;

   m_rectScrollButtons[ eNextYear ].right = rectClient.right - g_nHorzOffset;
   m_rectScrollButtons[ eNextYear ].left =
                         m_rectScrollButtons[ eNextYear ].right - nBtnWidth;

   m_rectScrollButtons[ eNextMonth ].right =
                         m_rectScrollButtons[ eNextYear ].left - 1;
   m_rectScrollButtons[ eNextMonth ].left =
                         m_rectScrollButtons[ eNextMonth ].right - nBtnWidth;

   m_rectScrollButtons[ eToday ].left =
                         m_rectScrollButtons[ ePrevMonth ].right + 1;
   m_rectScrollButtons[ eToday ].right =
                         m_rectScrollButtons[ eNextMonth ].left - 1;

   // Adjust tooltip rectangles:
   m_ToolTip.SetToolRect( this, 1, m_rectScrollButtons[ ePrevYear ] );
   m_ToolTip.SetToolRect( this, 2, m_rectScrollButtons[ ePrevMonth ] );
   m_ToolTip.SetToolRect( this, 3, m_rectScrollButtons[ eNextMonth ] );
   m_ToolTip.SetToolRect( this, 4, m_rectScrollButtons[ eNextYear ] );
}

int
CCalendarScrollbarWnd::ScrollButtonHitTest( CPoint& pt ) const
{
   for ( int k = 0; k < zMINICAL_BUTTON_IMAGE_CNT; k++ )
   {
      if ( m_rectScrollButtons[ k ].PtInRect( pt ) )
      {
         return( k );
      }
   }

   return( zNO_HIT );
}

void
CCalendarScrollbarWnd::OnTimer( UINT uIDEvent )
{
   if ( uIDEvent != g_nIdClickButtonEvt )
      return;

   if ( m_nTrackedScrollButton == m_nPressedScrollButton )
   {
      OnClickScrollButton( m_nTrackedScrollButton );

      if ( m_bSlowTimerMode && m_nSlowTimerCount++ >= g_nNumberOfSlowTimes )
      {
         m_bSlowTimerMode = FALSE;
         KillTimer( g_nIdClickButtonEvt );
         SetTimer( g_nIdClickButtonEvt, g_nFastTimerDuration, 0 );
      }
   }
}

void
CCalendarScrollbarWnd::OnClickScrollButton( int nScrollButton )
{
   ASSERT( m_pParentCalendar );
   m_pParentCalendar->OnToolbarNotifyClickButton( (eBUTTON_ID) nScrollButton );
}

void
CCalendarScrollbarWnd::AlignInParentClientArea( BOOL bBottom )
{
   ASSERT( m_pParentCalendar );

   if ( m_pParentCalendar->m_hWnd == 0 )
   {
      return;
   }

   CRect rectParentClient;
   m_pParentCalendar->GetClientRect( &rectParentClient );

   int y = (bBottom) ?
      rectParentClient.bottom - m_nWindowHeight - 2:
      rectParentClient.top + 2;

   SetWindowPos( 0, rectParentClient.left + 2, y,
                 rectParentClient.Width( ) - 4, m_nWindowHeight,
                 SWP_NOZORDER );
}

void
CCalendarScrollbarWnd::RelayEvent( UINT uMessage, WPARAM wParam, LPARAM lParam )
{
   if ( m_ToolTip.m_hWnd )
   {
      MSG msg;

      msg.hwnd = m_hWnd;
      msg.message = uMessage;
      msg.wParam = wParam;
      msg.lParam = lParam;
      msg.time = 0;
      msg.pt.x = LOWORD( lParam );
      msg.pt.y = HIWORD( lParam );

      m_ToolTip.RelayEvent( &msg );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMultiSelCal

IMPLEMENT_DYNAMIC( CMultiSelCal, CWnd )
BEGIN_MESSAGE_MAP( CMultiSelCal, CWnd )
   //{{AFX_MSG_MAP( CMultiSelCal )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_MOUSEMOVE( )
   ON_WM_PAINT( )
   ON_WM_ERASEBKGND( )
   ON_WM_KEYDOWN( )
   ON_WM_CREATE( )
   ON_WM_KILLFOCUS( )
   ON_WM_SIZE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CString CMultiSelCal::m_csClassName = _T( "" );
static const int nRows = 6;
static const int nColumns = 7;

CMultiSelCal::CMultiSelCal( )
{
   m_nTrackedDaySpot = zNO_HIT;
   m_nPressedDaySpot = zNO_HIT;
   m_bIsDropCalendar = FALSE;
   m_bMultipleSel = FALSE;
   m_bAbsoluteSel = FALSE;
   m_nStartMultipleSelection = zNO_HIT;
   m_nLastMultipleSelection = zNO_HIT;

   SetValidInterval( COleDateTime( 1899, 12, 30, 0, 0, 0 ),
                     COleDateTime( 1899, 12, 30, 0, 0, 0 ) );
   COleDateTime date = COleDateTime::GetCurrentTime( );
   m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );

   m_clrBack = ::GetSysColor( COLOR_WINDOW );
   m_clrText = ::GetSysColor( COLOR_WINDOWTEXT );
   m_clrSelBack = ::GetSysColor( COLOR_3DFACE );
   m_clrSelText = ::GetSysColor( COLOR_BTNTEXT );
   m_clrLight = ::GetSysColor( COLOR_3DHILIGHT );
   m_clrShadow = ::GetSysColor( COLOR_3DDKSHADOW );

   TCHAR szLocaleData[ 32 ];
   ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK,
                    szLocaleData, sizeof( szLocaleData ) );

   // m_nFirstDayOfWeek should be 0 = Sunday, 1 = Monday, ....:
   m_nFirstDayOfWeek = (szLocaleData[ 0 ] - _T( '0' ) + 1) % 7;

   if ( m_nFirstDayOfWeek < 0 || m_nFirstDayOfWeek >= 7 )
   {
      ASSERT( FALSE );
      m_nFirstDayOfWeek = 1;
   }
}

CMultiSelCal::~CMultiSelCal( )
{
}

/////////////////////////////////////////////////////////////////////////////
// CMultiSelCal message handlers

void
CMultiSelCal::OnLButtonDown( UINT uFlags, CPoint pt )
{
   ASSERT( m_pParent );

   SetFocus( );

   CRect rectClient;
   GetClientRect( rectClient );

   if ( rectClient.PtInRect( pt ) == FALSE )
   {
      ReleaseCapture( );
      HideDropCalendar( );
      return;
   }

   m_nTrackedDaySpot = m_nPressedDaySpot = DaySpotHitTest( pt );

   int nStartMultipleSelection = m_nTrackedDaySpot;
   int nLastMultipleSelection = m_nTrackedDaySpot;

   if ( uFlags & MK_SHIFT ) // Shift is held down
   {
      nStartMultipleSelection = 0;
      for ( int nDay = m_nTrackedDaySpot - 1; nDay > 1; nDay-- )
      {
         if ( IsDaySelected( nDay ) )
         {
            nStartMultipleSelection = nDay;
            break;
         }
      }

      if ( nStartMultipleSelection == 0 )
      {
         nStartMultipleSelection = m_nTrackedDaySpot;

         nLastMultipleSelection = 0;
         for ( int nDay = m_nTrackedDaySpot + 1;
               nDay <= m_nDaysInMonth;
               nDay++ )
         {
            if ( IsDaySelected( nDay ) )
            {
               nLastMultipleSelection = nDay;
               break;
            }
         }

         if ( nLastMultipleSelection == 0 )
         {
            nLastMultipleSelection = m_nTrackedDaySpot;
         }
         else
         {
            while ( IsDaySelected( ++nLastMultipleSelection ) )
               ;

            nLastMultipleSelection--;
         }
      }
      else
      {
         while ( IsDaySelected( --nStartMultipleSelection ) )
            ;

         nStartMultipleSelection++;
      }

      ClearSelectedDays( );
   }
   else
   if ( (uFlags & MK_CONTROL) == 0 )   // Ctrl is released
   {
      ClearSelectedDays( );
   }

   CRect rect;
   if ( GetButtonRect( m_nTrackedDaySpot, rect ) )
   {
      rect.InflateRect( 3, 3 );
      InvalidateRect( rect );
   }

   if ( m_bMultipleSel )
   {
      if ( uFlags & MK_SHIFT )
      {
         m_nStartMultipleSelection = nStartMultipleSelection;
         m_nLastMultipleSelection = nLastMultipleSelection;
         AddSelectedDay( m_nStartMultipleSelection );
      }
      else
      {
         m_nStartMultipleSelection = zNO_HIT;
         m_nLastMultipleSelection = zNO_HIT;
      }

      OnMouseMove( uFlags, pt );
   }

   UpdateWindow( );
   CWnd::OnLButtonDown( uFlags, pt );
}

/////////////////////////////////////////////////////////////////////////////
void
CMultiSelCal::OnLButtonUp( UINT uFlags, CPoint pt )
{
   ReleaseCapture( );

   m_nStartMultipleSelection = zNO_HIT;
   m_nLastMultipleSelection = zNO_HIT;

   m_nTrackedDaySpot = DaySpotHitTest( pt );

   if ( m_nTrackedDaySpot == m_nPressedDaySpot &&
      m_nTrackedDaySpot > 0 )
   {
      COleDateTime date( m_dateToday.GetYear( ), m_dateToday.GetMonth( ),
                         m_nTrackedDaySpot, m_dateToday.GetHour( ),
                         m_dateToday.GetMinute( ), 0 );

      if ( HideDropCalendar( &date ) )
      {
         return;
      }

      if ( m_bMultipleSel )
      {
         if ( uFlags & MK_CONTROL )
         {
            if ( IsDaySelected( m_nTrackedDaySpot ) )
            {
               RemoveSelectedDay( m_nTrackedDaySpot );
            }
            else
            {
               AddSelectedDay( m_nTrackedDaySpot );
            }
         }
         else
         {
            AddSelectedDay( m_nTrackedDaySpot );
         }

         if ( m_dateToday != date )
         {
            m_dateToday = date;
            OnDateChanged( );
         }
      }
      else
      {
         if ( m_dateToday != date )
         {
            for ( int k = 0; k < 2; k++ )
            {
               CRect rect = m_rectDayLocations[ m_dateToday.GetDay( ) - 1 ];
               rect.InflateRect( 3, 3 );
               InvalidateRect( rect );

               m_dateToday = date;
            }

            OnDateChanged( );
         }
      }
   }

   int nRedraw1 = m_nPressedDaySpot;
   int nRedraw2 = m_nTrackedDaySpot;

   m_nPressedDaySpot = 0;
   m_nTrackedDaySpot = 0;

   if ( nRedraw1 > 0 )
   {
      InvalidateRect( m_rectDayLocations[ nRedraw1 - 1 ] );
   }

   if ( nRedraw2 > 0 )
   {
      InvalidateRect( m_rectDayLocations[ nRedraw2 - 1 ] );
   }

   UpdateWindow( );
   CWnd::OnLButtonUp( uFlags, pt );
}

/////////////////////////////////////////////////////////////////////////////
void
CMultiSelCal::OnMouseMove( UINT uFlags, CPoint pt )
{
   int nPrevTrackButton = m_nTrackedDaySpot;
   m_nTrackedDaySpot = DaySpotHitTest( pt );

   if ( m_bMultipleSel &&
        (uFlags & MK_SHIFT) && (uFlags & MK_LBUTTON) &&
        m_nStartMultipleSelection != zNO_HIT && m_nTrackedDaySpot > 0 &&
        m_nStartMultipleSelection != m_nTrackedDaySpot )
   {
      int nDay;
      int nDay1 = (m_nTrackedDaySpot > m_nStartMultipleSelection) ?
                                m_nStartMultipleSelection : m_nTrackedDaySpot;
      int nDay2 = (m_nTrackedDaySpot > m_nStartMultipleSelection) ?
                                m_nTrackedDaySpot : m_nStartMultipleSelection;

      if ( m_nLastMultipleSelection > m_nTrackedDaySpot )
      {
         for ( nDay = m_nTrackedDaySpot + 1;
               nDay <= m_nLastMultipleSelection;
               nDay++ )
         {
            RemoveSelectedDay( nDay );
         }
      }

      for ( nDay = nDay1; nDay <= nDay2; nDay++ )
      {
         AddSelectedDay( nDay );
      }

      m_nLastMultipleSelection = m_nTrackedDaySpot;
      m_nPressedDaySpot = 0;
   }

   if ( m_nTrackedDaySpot != nPrevTrackButton )
   {
      CRect rect;

      if ( m_nPressedDaySpot == 0 ||
           nPrevTrackButton == m_nPressedDaySpot )
      {
         if ( GetButtonRect( nPrevTrackButton, rect ) )
         {
            rect.InflateRect( 3, 3 );
            InvalidateRect( rect );
         }
      }

      if ( m_nPressedDaySpot == 0 ||
           m_nTrackedDaySpot == m_nPressedDaySpot )
      {
         if ( GetButtonRect( m_nTrackedDaySpot, rect ) )
         {
            rect.InflateRect( 3, 3 );
            InvalidateRect( rect );
         }
      }

      if ( m_nTrackedDaySpot )
      {
         if ( nPrevTrackButton == 0 )
         {
            SetCapture( );
         }
      }
      else
      {
         if ( m_nPressedDaySpot == 0 )
         {
            ReleaseCapture( );
         }
      }
   }

   UpdateWindow( );
   CWnd::OnMouseMove( uFlags, pt );
}

BOOL
CMultiSelCal::OnEraseBkgnd( CDC *pDC )
{
   return( TRUE );
}

void
CMultiSelCal::OnPaint( )
{
   ASSERT( m_pParent );

   CPaintDC dcPaint( this ); // device context for painting

   CRect rectClient;
   GetClientRect( &rectClient );

   // If possible, we paint to an off-screen DC and BitBlt the results to
   // the screen (reduces flicker).
   CDC      *pDC = &dcPaint;
   zBOOL    bMemDC = FALSE;
   CDC      memDC;
   CBitmap  bmp;
   CBitmap  *pOldBmp = 0;

   if ( memDC.CreateCompatibleDC( &dcPaint ) &&
        bmp.CreateCompatibleBitmap( &dcPaint, rectClient.Width( ),
                                    rectClient.Height( ) ) )
   {
      // Off-screen DC successfully created. Paint to it!
      bMemDC = TRUE;
      pOldBmp = memDC.SelectObject( &bmp );
      pDC = &memDC;
   }

   pDC->SetBkMode( TRANSPARENT );
   CFont *pOldFont = (CFont *) pDC->SelectStockObject( DEFAULT_GUI_FONT );

   // Fill background:
   pDC->FillSolidRect( &rectClient, m_clrBack );

   // Draw 3D border around the control:
   if ( m_bIsDropCalendar )
   {
      pDC->Draw3dRect( &rectClient,
                       ::GetSysColor( COLOR_3DHILIGHT ),
                       ::GetSysColor( COLOR_3DDKSHADOW ) );

      rectClient.InflateRect( -1, -1 );
      pDC->Draw3dRect( &rectClient,
                       ::GetSysColor( COLOR_3DHILIGHT ),
                       ::GetSysColor( COLOR_3DSHADOW ) );
   }
   else
   {
      pDC->Draw3dRect( &rectClient,
                       ::GetSysColor( COLOR_3DDKSHADOW ),
                       ::GetSysColor( COLOR_3DHILIGHT ) );

      rectClient.InflateRect( -1, -1 );
      pDC->Draw3dRect( &rectClient,
                       ::GetSysColor( COLOR_3DSHADOW ),
                       ::GetSysColor( COLOR_3DLIGHT ) );
   }

   // Get first day of the month:
   COleDateTimeSpan day( 1, 0, 0, 0 );
   COleDateTime dateCurr = m_dateToday;
   while ( dateCurr.GetDay( ) > 1 )
   {
      dateCurr -= day;
   }

   // Draw days:
   pDC->SetTextColor( m_clrText );

   COleDateTime dateToday = COleDateTime::GetCurrentTime( );

   for ( int nDay = 1; nDay <= m_nDaysInMonth; nDay++ )
   {
      CString csDay;
      csDay.Format( _T( "%d" ), nDay );

      zBOOL bIsSelectedDay = IsDaySelected( nDay );
      zBOOL bIsDayMarked = (m_MarkedDays.Find( nDay ) != 0);

      zBOOL bIsToday = (nDay == dateToday.GetDay( ) &&
                       m_dateToday.GetMonth( ) == dateToday.GetMonth( ) &&
                       m_dateToday.GetYear( ) == dateToday.GetYear( ));

      COleDateTime date( m_dateToday.GetYear( ),
                         m_dateToday.GetMonth( ), nDay, 0, 0, 0 );
      zBOOL bIsInvalidDay = !IsDateValid( date );
      zBOOL bHighlight = FALSE;

      CRect rect;
      if ( GetButtonRect( nDay, rect ) && dcPaint.RectVisible( rect ) )
      {
         if ( bIsSelectedDay )
         {
            CRect rectSelect = rect;

            pDC->FillSolidRect( rectSelect, m_clrSelBack );
            pDC->SetTextColor( m_clrSelText );

            if ( m_bMultipleSel == FALSE )
            {
               bHighlight = TRUE;
            }
            else
            {
               CPen penDark( PS_SOLID, 1, m_clrShadow );
               CPen *pOldPen = (CPen *) pDC->SelectObject( &penDark );

               if ( IsLeftSelected( nDay ) == FALSE )
               {
                  pDC->MoveTo( rect.left, rect.top );
                  pDC->LineTo( rect.left, rect.bottom );
               }

               if ( IsTopSelected( nDay ) == FALSE )
               {
                  pDC->MoveTo( rect.left, rect.top );
                  pDC->LineTo( rect.right, rect.top );
               }

               CPen penLight( PS_SOLID, 1, m_clrLight );
               pDC->SelectObject( &penLight );

               if ( IsRightSelected( nDay ) == FALSE )
               {
                  pDC->MoveTo( rect.right, rect.top );
                  pDC->LineTo( rect.right, rect.bottom );
               }

               if ( IsBottomSelected( nDay ) == FALSE )
               {
                  pDC->MoveTo( rect.left, rect.bottom - 1 );
                  pDC->LineTo( rect.right, rect.bottom - 1 );
               }

               pDC->SelectObject( pOldPen );
            }
         }
         else
         if ( bIsToday )
         {
            pDC->FillSolidRect( rect, ::GetSysColor( COLOR_INFOBK ) );
            pDC->SetTextColor( ::GetSysColor( COLOR_INFOTEXT ) );

            bHighlight = TRUE;
         }

         if ( bIsInvalidDay )
         {
            pDC->SetTextColor( ::GetSysColor( COLOR_GRAYTEXT ) );
         }

         CFont *pFontCurr = 0;
         if ( bIsDayMarked )
         {
            pFontCurr = pDC->SelectObject( &m_fontBold );
         }

         pDC->DrawText( csDay, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER );

         if ( pFontCurr )
         {
            pDC->SelectObject( pFontCurr );
         }

         if ( bIsSelectedDay || bIsInvalidDay || bIsToday )
         {
            pDC->SetTextColor( m_clrText );   // restore default color
         }

         // Define button state:
         eBUTTON_STATE nBtnState = eFLAT;

         if ( nDay == m_nPressedDaySpot )
         {
            if ( nDay == m_nTrackedDaySpot )
            {
               nBtnState = ePRESSED;
            }
            else
            {
               nBtnState = eHILIGHT;
            }
         }
         else
         if ( nDay == m_nTrackedDaySpot && m_nPressedDaySpot == 0 )
         {
            nBtnState = eHILIGHT;
         }
         else
         if ( bIsToday || bHighlight )
         {
            nBtnState = ePRESSED;
         }

         // Draw 3D border around the cell.
         switch ( nBtnState )
         {
            case ePRESSED:
               pDC->Draw3dRect( rect, m_clrShadow, m_clrLight );
               rect.OffsetRect( 1, 1 );
               break;

            case eHILIGHT:
               pDC->Draw3dRect( rect, m_clrLight, m_clrShadow );
               break;
         }
      }
   }

   pDC->SelectObject( pOldFont );

   if ( bMemDC )
   {
      // Copy the results to the on-screen DC.
      CRect rect;
      dcPaint.GetClipBox( rect );
      dcPaint.BitBlt( rect.left, rect.top, rect.Width( ), rect.Height( ),
                      &memDC, rect.left, rect.top, SRCCOPY );

      memDC.SelectObject( pOldBmp );
   }
}

void
CMultiSelCal::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   ASSERT( m_pParent );

   if ( uChar == VK_ESCAPE )
   {
      ReleaseCapture( );
      HideDropCalendar( );
   }
}

int
CMultiSelCal::OnCreate( LPCREATESTRUCT lpcs )
{
   int nRC = CWnd::OnCreate( lpcs );
   if ( nRC == -1 )
      return( -1 );

   // Create toolbar:
   CRect rectToolbar( 0, 0, 0, 0 );
   if ( m_wndToolbar.Create( rectToolbar, this, 1 ) == 0 )
   {
      TRACE( _T( "CMultiSelCal: Can't create toolbar!\n" ) );
      return( -1 );
   }

   UpdateColors( );

   // Create caption:
   CRect rectCaption( 0, 0, 0, 0 );
   if ( m_wndCaption.Create( rectCaption, this, 2 ) == 0 )
   {
      TRACE( _T( "CMultiSelCal: Can't create caption window!\n" ) );
      return( -1 );
   }

   EnableMultipleSelection( TRUE );
   m_clrSelBack = 0x00b3b562;  // what in the world is this???

   m_wndCaption.m_bMultiEnabled = m_bMultipleSel;
   m_wndCaption.m_nFirstDayOfWeek = m_nFirstDayOfWeek;

   // Create bold font (for marked days):
   CFont *pFontDefault =
         CFont::FromHandle( (HFONT) ::GetStockObject( DEFAULT_GUI_FONT ) );
   ASSERT( pFontDefault );

   LOGFONT lf;
   pFontDefault->GetLogFont( &lf );
   lf.lfWeight = FW_EXTRABOLD;
   lf.lfUnderline = TRUE;

   m_fontBold.CreateFontIndirect( &lf );

   // Calculate calendar size:
   CRect rectClient;
   GetClientRect( &rectClient );

   CClientDC dc( this );
   CFont *pOldFont = (CFont *) dc.SelectStockObject( DEFAULT_GUI_FONT );

   TEXTMETRIC tm;
   dc.GetTextMetrics( &tm );

   int nTextHeight = tm.tmHeight * 3 / 2;
   int nColumnWidth = tm.tmMaxCharWidth * 2;

   m_nTitleHeight = nTextHeight * 2;

   int nWndHeight = m_nTitleHeight + m_wndToolbar.GetHeight( ) +
                                              nTextHeight * (nRows + 1);

   int nWndWidth = nColumnWidth * (nColumns + 1);

   SetWindowPos( 0, -1, -1,
                 nWndWidth, nWndHeight,
                 SWP_NOMOVE | SWP_NOZORDER );

   m_nDaysInMonth = GetDaysInMonth( m_dateToday.GetMonth( ),
                                    m_dateToday.GetYear( ) );
   RebuildLocations( );
   dc.SelectObject( pOldFont );

   return( nRC );
}

BOOL
CMultiSelCal::Create( DWORD dwStyle, int x, int y, CWnd *pParentWnd )
{
   if ( m_csClassName.IsEmpty( ) )
   {
      m_csClassName =
       AfxRegisterWndClass( 0, AfxGetApp( )->LoadStandardCursor( IDC_ARROW ) );
   }

   m_pParent = pParentWnd;
   ASSERT( m_pParent );
   ASSERT( m_pParent->GetSafeHwnd( ) );

   m_bIsDropCalendar = (dwStyle & WS_POPUP) ? TRUE : FALSE;
   return( CWnd::CreateEx( m_bIsDropCalendar ? WS_EX_PALETTEWINDOW : 0,
                           m_csClassName, _T( "" ), dwStyle, x, y, 100, 100,
                           m_bIsDropCalendar ? 0 : pParentWnd->GetSafeHwnd( ),
                           0 ) );
}

int
CMultiSelCal::DaySpotHitTest( CPoint& pt ) const
{
   for ( int nDay = 1; nDay <= m_nDaysInMonth; nDay++ )
   {
      COleDateTime date( m_dateToday.GetYear( ),
                         m_dateToday.GetMonth( ), nDay, 0, 0, 0 );

      CRect rect;
      if ( IsDateValid( date ) && GetButtonRect( nDay, rect ) &&
           rect.PtInRect( pt ) )
      {
         return( nDay );
      }
   }

   return( 0 );
}

BOOL
CMultiSelCal::GetButtonRect( int nID, CRect& rect ) const
{
   if ( nID < 1 || nID > m_nDaysInMonth )
      return( FALSE );

   rect = m_rectDayLocations[ nID - 1 ];
   return( TRUE );
}

void
CMultiSelCal::RebuildLocations( )
{
   // Get first day of the month:
   COleDateTime dateCurr( m_dateToday.GetYear( ),
                          m_dateToday.GetMonth( ), 1, 0, 0, 0 );
   COleDateTimeSpan day( 1, 0, 0, 0 );

   int y = m_rectDays.top;

   int nRowHeight = m_rectDays.Height( ) / nRows;
   int nColumnWidth = m_rectDays.Width( ) / nColumns;

   while ( dateCurr.GetMonth( ) == m_dateToday.GetMonth( ) )
   {
      int nWeekday = dateCurr.GetDayOfWeek( ) - m_nFirstDayOfWeek;
      if ( nWeekday < 1 )
      {
         nWeekday += 7;
      }

      int nDay = dateCurr.GetDay( );

      int x = m_rectDays.left + nColumnWidth * (nWeekday - 1);

      m_rectDayLocations[ nDay - 1 ] =
               CRect( x, y, x + nColumnWidth, y + nRowHeight );

      if ( nWeekday == 7 )
      {
         y += nRowHeight;
         x = m_rectDays.left;
      }
      else
      {
         x += nColumnWidth;
      }

      dateCurr += day;
   }
}

void
CMultiSelCal::OnKillFocus( CWnd *pWndGetFocus )
{
   ASSERT( m_pParent );
   if ( GetCapture( ) == this )
      ReleaseCapture( );

   CRect rect;
   if ( GetButtonRect( m_nTrackedDaySpot, rect ) )
   {
      rect.InflateRect( 3, 3 );
      InvalidateRect( rect );
   }

   m_nStartMultipleSelection = zNO_HIT;
   m_nLastMultipleSelection = zNO_HIT;
   m_nPressedDaySpot = 0;
   m_nTrackedDaySpot = 0;

   HideDropCalendar( );
}

BOOL
CMultiSelCal::HideDropCalendar( const COleDateTime *pDateNew )
{
   if ( m_bIsDropCalendar == FALSE )
   {
      return( FALSE );
   }
#if 0
   ASSERT( m_pParent );
   if ( m_pParent->IsKindOf( RUNTIME_CLASS( CBCGDateTimeCtrl ) ) )
   {
      CBCGDateTimeCtrl *pDTWnd = (CBCGDateTimeCtrl *) m_pParent;
      pDTWnd->HideDropCalendar( pDateNew );

      return( TRUE );
   }
#endif
   return( FALSE );
}

BOOL
CMultiSelCal::IsDateValid( const COleDateTime& date ) const
{
   if ( date.GetStatus( ) == COleDateTime::invalid )
   {
      return( FALSE );
   }

   COleDateTime dateEmpty;

   if ( m_dateMin != dateEmpty && date < m_dateMin )
   {
      return( FALSE );
   }

   if ( m_dateMax != dateEmpty && date > m_dateMax )
   {
      return( FALSE );
   }

   return( TRUE );
}

void
CMultiSelCal::SetValidInterval( const COleDateTime& fromDate,
                                const COleDateTime& toDate )
{
   m_dateMin = fromDate;
   m_dateMax = toDate;

   // From MSDN: The COleDateTime class handles dates
   // from 1 January 100 to 31 December 9999.
   COleDateTime minAllowedDate( 100, 1, 1, 0, 0, 0 );
   COleDateTime maxAllowedDate( 9999, 12, 31, 23, 59, 59 );
   COleDateTime emptyDate( 1899, 12, 30, 0, 0, 0 );

   if ( m_dateMin == emptyDate )
      m_dateMin = minAllowedDate;
   else
      m_dateMin = max( m_dateMin, minAllowedDate );

   if ( m_dateMax == emptyDate )
      m_dateMax = maxAllowedDate;
   else
      m_dateMax = min( m_dateMax, maxAllowedDate );

   if ( m_hWnd )
   {
      Invalidate( );
   }
}

BOOL
CMultiSelCal::SetDate( const COleDateTime& date )
{
   if ( IsDateValid( date ) == FALSE )
   {
      return( FALSE );
   }

   COleDateTime oldDate = m_dateToday;
   zBOOL bIsMonthChanged = (m_dateToday.GetMonth( ) != date.GetMonth( ) ||
                           m_dateToday.GetYear( ) != date.GetYear( ));

   m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );
   m_nDaysInMonth = GetDaysInMonth( m_dateToday.GetMonth( ),
                                    m_dateToday.GetYear( ) );
   AlignSelectedDays( oldDate );
   RebuildLocations( );

   // Redraw days + title:
   m_wndCaption.SetDate( m_dateToday );

   if ( m_hWnd )
   {
      InvalidateRect( m_rectDays );
      UpdateWindow( );
   }
#if 0
   if ( bIsMonthChanged && m_hWnd )
   {
      CBCGCalendarCtrl *pParent = (CBCGCalendarCtrl *) GetParent( );
      if ( pParent && pParent->IsKindOf( RUNTIME_CLASS( CBCGCalendarCtrl ) ) )
      {
         pParent->FireOnMonthChanged( );
      }
   }
#endif

   OnDateChanged( );
   return( TRUE );
}

const COleDateTime&
CMultiSelCal::GetDate( ) const
{
   return( m_dateToday );
}

void
CMultiSelCal::OnSize( UINT uType, int cx, int cy )
{
   CWnd::OnSize( uType, cx, cy );

   // Adjust toolbar and caption:
   m_wndToolbar.AlignInParentClientArea( );
   m_wndCaption.SetWindowPos( 0, 2, 2, cx - 4,
                              m_nTitleHeight, SWP_NOZORDER );

   CRect rectToolbar;
   m_wndToolbar.GetClientRect( &rectToolbar );

   // Define days area rectangle:
   m_rectDays = CRect( 10, m_nTitleHeight + 5,
                  cx - 10, cy - rectToolbar.Height( ) - 5 );

   RebuildLocations( );
}

void
CMultiSelCal::ClearSelectedDays( )
{
   while ( m_SelectedDays.IsEmpty( ) == FALSE )
   {
      int nDay = m_SelectedDays.RemoveHead( );
      ASSERT( nDay >= 1 && nDay <= 31 );

      CRect rect = m_rectDayLocations[ nDay - 1 ];
      rect.InflateRect( 3, 3 );
      InvalidateRect( rect );
   }

   UpdateWindow( );
   OnSelectionChanged( );
}

void
CMultiSelCal::AddSelectedDay( int nDay )
{
   ASSERT( nDay >= 1 && nDay <= 31 );
   zBOOL bInserted = FALSE;

   for ( POSITION pos = m_SelectedDays.GetHeadPosition( ); pos; )
   {
      POSITION posSave = pos;
      int nSelDay = m_SelectedDays.GetNext( pos );

      if ( nSelDay == nDay )     // already in list, do nothing!
         return;

      if ( nSelDay > nDay )
      {
         m_SelectedDays.InsertBefore( posSave, nDay );
         bInserted = TRUE;
         break;
      }
   }

   if ( bInserted == FALSE )
   {
      m_SelectedDays.AddTail( nDay );
   }

   CRect rect = m_rectDayLocations[ nDay - 1 ];
   rect.InflateRect( 3, 3 );
   InvalidateRect( rect );

   OnSelectionChanged( );
}

void
CMultiSelCal::RemoveSelectedDay( int nDay )
{
   ASSERT( nDay >= 1 && nDay <= 31 );

   for ( POSITION pos = m_SelectedDays.GetHeadPosition( ); pos; )
   {
      POSITION posSave = pos;
      if ( m_SelectedDays.GetNext( pos ) == nDay )
      {
         m_SelectedDays.RemoveAt( posSave );

         CRect rect = m_rectDayLocations[ nDay - 1 ];
         rect.InflateRect( 3, 3 );
         InvalidateRect( rect );
         return;
      }
   }

   OnSelectionChanged( );
}

BOOL
CMultiSelCal::IsDaySelected( int nDay ) const
{
   if ( nDay < 1 || nDay > m_nDaysInMonth )
   {
      return( FALSE );
   }

   if ( m_bMultipleSel == FALSE )
   {
      return( nDay == m_dateToday.GetDay( ) );
   }

   for ( POSITION pos = m_SelectedDays.GetHeadPosition( ); pos; )
   {
      if ( m_SelectedDays.GetNext( pos ) == nDay )
      {
         return( TRUE );
      }
   }

   return( FALSE );
}

void
CMultiSelCal::OnToolbarNotifyClickButton( CCalendarScrollbarWnd::eBUTTON_ID nID )
{
   int nDay = m_dateToday.GetDay( );
   int nMonth = m_dateToday.GetMonth( );
   int nYear = m_dateToday.GetYear( );
   int nHour = m_dateToday.GetHour( );
   int nMin = m_dateToday.GetMinute( );

   int nDir = 0;

   switch ( nID )
   {
      case CCalendarScrollbarWnd::ePrevMonth:
         nMonth--;
         nDir = -1;
         if ( nMonth == 0 )
         {
            nMonth = zMONTHS_IN_YEAR;
            nYear--;
         }

         break;

      case CCalendarScrollbarWnd::eNextMonth:
         nDir = 1;
         nMonth++;
         if ( nMonth > zMONTHS_IN_YEAR )
         {
            nMonth = 1;
            nYear++;
         }

         break;

      case CCalendarScrollbarWnd::ePrevYear:
         nYear--;
         nDir = -1;
         break;

      case CCalendarScrollbarWnd::eNextYear:
         nDir = 1;
         nYear++;
         break;

      case CCalendarScrollbarWnd::eToday:
      {
         COleDateTime dateToday = COleDateTime::GetCurrentTime( );
         nDay = dateToday.GetDay( );
         nMonth = dateToday.GetMonth( );
         nYear = dateToday.GetYear( );
         break;
      }

      default:
         return;
   }

   int nDaysInMonth = GetDaysInMonth( nMonth, nYear );
   if ( nDay > nDaysInMonth )
   {
      nDay = nDaysInMonth;
   }

   COleDateTime date( nYear, nMonth, nDay, nHour, nMin, 0 );
   if ( nDir > 0 && date > m_dateMax )
   {
      date = m_dateMax;
   }

   if ( nDir < 0 && date < m_dateMin )
   {
      date = m_dateMin;
   }

   if ( m_dateToday == date ||
        SetDate( date ) == FALSE )
   {
      MessageBeep( -1 );
   }
   else
   {
      m_nDaysInMonth = nDaysInMonth;
   }
}

BOOL
CMultiSelCal::IsLeftSelected( int nDay ) const
{
   ASSERT( nDay >= 1 && nDay <= 31 );

   if ( nDay == 1 )
   {
      return( FALSE );
   }

   COleDateTime date( m_dateToday.GetYear( ),
                      m_dateToday.GetMonth( ), nDay, 0, 0, 0 );

   if ( date.GetDayOfWeek( ) == 1 )   // almost left
   {
      return( FALSE );
   }

   return( IsDaySelected( nDay - 1 ) );
}

BOOL
CMultiSelCal::IsRightSelected( int nDay ) const
{
   ASSERT( nDay >= 1 && nDay <= 31 );

   if ( nDay == m_nDaysInMonth )
   {
      return( FALSE );
   }

   COleDateTime date( m_dateToday.GetYear( ),
                      m_dateToday.GetMonth( ), nDay, 0, 0, 0 );

   if ( date.GetDayOfWeek( ) == 7 )   // almost right
   {
      return( FALSE );
   }

   return( IsDaySelected( nDay + 1 ) );
}

BOOL
CMultiSelCal::IsTopSelected( int nDay ) const
{
   ASSERT( nDay >= 1 && nDay <= 31 );

   int nTopDay = nDay - 7;
   if ( nTopDay < 1 || nTopDay > m_nDaysInMonth )
   {
      return( FALSE );
   }

   return( IsDaySelected( nTopDay ) );
}

BOOL
CMultiSelCal::IsBottomSelected( int nDay ) const
{
   ASSERT( nDay >= 1 && nDay <= 31 );

   int nBottomDay = nDay + 7;
   if ( nBottomDay < 1 || nBottomDay > m_nDaysInMonth )
   {
      return( FALSE );
   }

   return( IsDaySelected( nBottomDay ) );
}

void
CMultiSelCal::AlignSelectedDays( const COleDateTime& oldDate )
{
   if ( m_bAbsoluteSel )
   {
      return;
   }

   int nMonth = m_dateToday.GetMonth( );
   int nYear = m_dateToday.GetYear( );

   int nOldMonth = oldDate.GetMonth( );
   int nOldYear = oldDate.GetYear( );

   // Save current selections:
   POSITION pos;
   CList<int,int> oldSelectedDays;
   for ( pos = m_SelectedDays.GetHeadPosition( ); pos; )
   {
      oldSelectedDays.AddTail( m_SelectedDays.GetNext( pos ) );
   }

   m_SelectedDays.RemoveAll( );

   COleDateTime oldFirstDayInMonth( nOldYear, nOldMonth, 1, 0, 0, 0 );
   int nOldFirstDayInMonthWeek = oldFirstDayInMonth.GetDayOfWeek( );

   COleDateTime newFirstDayInMonth( nYear, nMonth, 1, 0, 0, 0 );
   int nNewFirstDayInMonthWeek = newFirstDayInMonth.GetDayOfWeek( );

   int nDelta = nNewFirstDayInMonthWeek - nOldFirstDayInMonthWeek;

   for ( pos = oldSelectedDays.GetHeadPosition( ); pos; )
   {
      int nOldSeldDay = oldSelectedDays.GetNext( pos );
      int nNewSeldDay = nOldSeldDay - nDelta;

      if ( nNewSeldDay > 0 && nNewSeldDay <= m_nDaysInMonth )
      {
         m_SelectedDays.AddTail( nNewSeldDay );
      }
   }
}

void
CMultiSelCal::EnableMultipleSelection( BOOL bMultipleSel )
{
   if ( m_bMultipleSel == bMultipleSel )
   {
      return;
   }

   m_bMultipleSel = bMultipleSel;
   m_SelectedDays.RemoveAll( );

   if ( GetSafeHwnd( ) )
   {
      m_wndCaption.m_bMultiEnabled = m_bMultipleSel;
      m_wndCaption.Invalidate( );
      m_wndCaption.UpdateWindow( );

      Invalidate( );
      UpdateWindow( );
   }
}

void
CMultiSelCal::SelectDayInWeek( int nWeekday, BOOL bClearOther )
{
   if ( bClearOther )
   {
      m_SelectedDays.RemoveAll( );
   }

   int nMonth = m_dateToday.GetMonth( );
   int nYear = m_dateToday.GetYear( );

   for ( int nDay = 1; nDay <= m_nDaysInMonth; nDay++ )
   {
      COleDateTime date( nYear, nMonth, nDay, 0, 0, 0 );
      if ( date.GetDayOfWeek( ) == nWeekday )
      {
         if ( IsDaySelected( nDay ) )
         {
            RemoveSelectedDay( nDay );
         }
         else
         {
            AddSelectedDay( nDay );
         }
      }
   }

   Invalidate( );
   UpdateWindow( );

   OnSelectionChanged( );
}

void
CMultiSelCal::UpdateColors( )
{
   m_wndToolbar.m_clrBack = m_clrBack;
   m_wndToolbar.m_clrText = m_clrText;
   m_wndToolbar.m_clrLight = m_clrLight;
   m_wndToolbar.m_clrShadow = m_clrShadow;

   if ( GetSafeHwnd( ) )
   {
      Invalidate( );
      UpdateWindow( );
   }
}

void
CMultiSelCal::OnSelectionChanged( )
{
#if 0
   if ( m_hWnd )
   {
      CBCGCalendarCtrl *pParent = (CBCGCalendarCtrl *) GetParent( );
      if ( pParent && pParent->IsKindOf( RUNTIME_CLASS( CBCGCalendarCtrl ) ) )
      {
         pParent->FireOnSelectionChanged( );
      }
   }
#endif
}

void
CMultiSelCal::OnDateChanged( )
{
#if 0
   if ( m_hWnd )
   {
      CBCGCalendarCtrl *pParent = (CBCGCalendarCtrl *) GetParent( );
      if ( pParent && pParent->IsKindOf( RUNTIME_CLASS( CBCGCalendarCtrl ) ) )
      {
         pParent->FireOnDateChanged( );
      }
   }
#endif
}

BOOL
CMultiSelCal::MarkDay( int nDay, BOOL bMarked )
{
   if ( bMarked == FALSE )
   {
      POSITION pos = m_MarkedDays.Find( nDay );
      if ( pos )
      {
         return( FALSE );
      }

      m_MarkedDays.RemoveAt( pos );
   }
   else
   {
      if ( m_MarkedDays.Find( nDay ) == 0 )
      {
         m_MarkedDays.AddTail( nDay );
      }
   }

   if ( GetSafeHwnd( ) )
   {
      ASSERT( nDay >= 1 && nDay <= 31 );

      CRect rect = m_rectDayLocations[ nDay - 1 ];
      rect.InflateRect( 3, 3 );
      InvalidateRect( rect );
      UpdateWindow( );
   }

   return( TRUE );
}

void
CMultiSelCal::UnMarkAllDays( )
{
   while ( m_MarkedDays.IsEmpty( ) == FALSE )
   {
      int nDay = m_MarkedDays.RemoveHead( );
      ASSERT( nDay >= 1 && nDay <= 31 );

      if ( GetSafeHwnd( ) )
      {
         CRect rect = m_rectDayLocations[ nDay - 1 ];
         rect.InflateRect( 3, 3 );
         InvalidateRect( rect );
      }
   }

   if ( GetSafeHwnd( ) )
   {
      UpdateWindow( );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZMultiCal, CMultiSelCal )
BEGIN_MESSAGE_MAP( ZMultiCal, CMultiSelCal )
END_MESSAGE_MAP( )

#define zDTN_DATETIMECHANGE  1
#define zDTN_USERSTRING      2
#define zDTN_WMKEYDOWN       4
#define zDTN_FORMAT          8
#define zDTN_FORMATQUERY    16
#define zDTN_DROPDOWN       32
#define zDTN_CLOSEUP        64

#define zCALENDAR_PERMIT_NULL           zCONTROL_SPECIFIC_1
#define zCALENDAR_CHECKBOX              zCONTROL_SPECIFIC_2
#define zCALENDAR_RECURSIVECLICK        zMAPACT_CTRL_SPECIFIC1

// ZMultiCal - ctor
ZMultiCal::ZMultiCal( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
           CMultiSelCal( ),
           ZMapAct( pZSubtask,             // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "Calendar" )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMultiCal::ctor ", *m_pzsTag );
#endif

   m_bPermitNull = (pCtrlDef->Subtype & zCALENDAR_PERMIT_NULL) ? TRUE : FALSE;

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style &= ~WS_BORDER;
   else
      Attr.Style |= WS_BORDER;

   Attr.Style |= WS_TABSTOP;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZMultiCal::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMultiCal::CreateZ ", *m_pzsTag );
#endif

   zLONG lStyleEx;

   if ( Attr.Style & WS_BORDER )
      lStyleEx = WS_EX_CLIENTEDGE;
   else
      lStyleEx = 0;

   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        Create( Attr.Style, Attr.X, Attr.Y, m_pWndParent ) )
   {
   // SetBackgroundColor( RGB( 231, 231, 214 ) );
      m_clrBack = RGB( 231, 231, 214 );
      UpdateColors( );

      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
   else
   {
#ifdef DEBUG_ALL
      zPCHAR pchMsg;
      ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
                                              FORMAT_MESSAGE_ALLOCATE_BUFFER,
                       0, 0x0000057F,  // GetLastError( ),
                       MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                       (zPCHAR) &pchMsg, 0, 0 );
      MessageSend( m_pZSubtask->m_vDialog, 0,
                   "ZMultiCal::CreateZ", pchMsg,
                   zMSGQ_MODAL_ERROR, FALSE );

      // Free the buffer.
      LocalFree( pchMsg );
#endif
   }
#endif
}

BOOL
ZMultiCal::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMultiCal::DestroyWindow ", *m_pzsTag );
#endif

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

#if 0
void
ZMultiCal::OnDateTimeChange( LPNMHDR pNMHDR, LRESULT *pResult )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMultiCal::OnDateTimeChange", "" );
#endif
   if ( m_ulMapActFlags & zCALENDAR_RECURSIVECLICK )
      return;

   m_ulMapActFlags |= zCALENDAR_RECURSIVECLICK;
   ProcessImmediateEvent( this, zDTN_DATETIMECHANGE );
   m_ulMapActFlags &= ~zCALENDAR_RECURSIVECLICK;
}
#endif

#if 0
typedef struct _SYSTEMTIME
{
   WORD wYear;
   WORD wMonth;
   WORD wDayOfWeek;
   WORD wDay;
   WORD wHour;
   WORD wMinute;
   WORD wSecond;
   WORD wMilliseconds;

} SYSTEMTIME, *PSYSTEMTIME, FAR *LPSYSTEMTIME;

// DWORD DateTimePick_GetSystemtime( HWND hdp, LPSYSTEMTIME pst )
//  - returns GDT_NONE if "none" is selected( DTS_SHOWNONE only )
//  - returns GDT_VALID and modifies *pst to be the currently selected value
#define DTM_GETSYSTEMTIME  (DTM_FIRST + 1)
#define DateTime_GetSystemtime( hdp, pst ) \
        (DWORD) SNDMSG( hdp, DTM_GETSYSTEMTIME, 0, (LPARAM)(pst) )

// BOOL DateTime_SetSystemtime( HWND hdp, DWORD gd, LPSYSTEMTIME pst )
//   if gd == GDT_NONE, sets datetimepick to None( DTS_SHOWNONE only )
//   if gd == GDT_VALID, sets datetimepick to *pst
//   returns TRUE on success, FALSE on error (such as bad params)
#define DTM_SETSYSTEMTIME  (DTM_FIRST + 2)
#define DateTime_SetSystemtime( hdp, gd, pst ) \
        (BOOL) SNDMSG( hdp, DTM_SETSYSTEMTIME, (LPARAM)(gd), (LPARAM)(pst) )
#endif

zSHORT
ZMultiCal::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMultiCal::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW vApp;
      zCHAR  szDate[ 256 ];
      zUSHORT uMapState = GetMappingAutoGray( &vApp, *m_pzsVName,
                                              *m_pzsEName, *m_pzsAName );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) &&
           GetStringFromAttribute( szDate, sizeof( szDate ), vApp,
                                   *m_pzsEName, *m_pzsAName ) >= 0 )
      {
#ifdef zREMOTE_SERVER
         SetZCtrlText( this, szDate );
#else
         DWORD dwGDT = GDT_NONE;

         if ( SetSysTimeFromZeidon( m_SysTime, szDate ) )
            dwGDT = GDT_VALID;

         DateTime_SetSystemtime( m_hWnd, dwGDT, &m_SysTime );
#endif
         return( 0 );
      }
      else
      {
#ifdef zREMOTE_SERVER
         SetZCtrlText( this, "" );
#else
         DWORD dwGDT;

         if ( m_bPermitNull )
            ModifyStyle( 0, DTS_SHOWNONE );

         m_SysTime.wMilliseconds = 0;
         m_SysTime.wSecond = 0;
         m_SysTime.wMinute = 0;
         m_SysTime.wHour = 0;
         m_SysTime.wDay = 0;
         m_SysTime.wMonth = 0;
         m_SysTime.wYear = 0;
         dwGDT = GDT_NONE;
         DateTime_SetSystemtime( m_hWnd, dwGDT, &m_SysTime );
#endif
      }
   }

   return( -1 );
}

// Get the text for the entry control and map it to the data OI.
zSHORT
ZMultiCal::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::MapToOI ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      zSHORT  nRC;
      zCHAR   szDate[ 256 ];

#ifdef zREMOTE_SERVER
      GetZCtrlText( this, szDate, sizeof( szDate ) );
#else
      DWORD dwGDT = DateTime_GetSystemtime( m_hWnd, &m_SysTime );

      if ( dwGDT == GDT_VALID )
      {
         if ( (lFlag & zVALIDATE) == zVALIDATE )
            return( 0 );

         SetZeidonDateFromSysTime( szDate, sizeof( szDate ), m_SysTime );
      }
      else
         szDate[ 0 ] = 0;
#endif

      nRC = SetAttributeFromString( vApp, *m_pzsEName, *m_pzsAName, szDate );
      return( nRC );
   }

   return( 0 );
}

zLONG
ZMultiCal::SetZCtrlState( zLONG  lStatusType,
                          zULONG ulValue )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "FirstName", *m_pzsTag ) == 0 )
      TraceLineS( "ZMultiCal::SetZCtrlState: ", *m_pzsTag );
#endif

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

// ZCalendar - dtor
ZMultiCal::~ZMultiCal( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMultiCal::dtor ", *m_pzsTag );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0
void
myDialog::OnDblclkStringInListBox( )
{
   const INT nIdx = m_Ctl.GetCurSel( );
   if ( nIdx == LB_ERR )
      return;

   CString cs;
   m_Ctl.GetText( nIdx, cs );
   RECT rect;
   INT result = m_Ctl.GetItemRect( nIdx, &rect );
   if ( result == LB_ERR )
      return;

   SmartEdit *pEdit = new SmartEdit;
   rect.bottom += 4;
   pEdit->Create( WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                  rect, &m_Ctl, (UINT) -1 );
   pEdit->SetWindowText( cs );
   pEdit->SetFocus( );
   pEdit->LimitText( MYMAX_LEN ); // user defined maximum length of string
}

Figure 1 shows the ON_LBN_DBLCLK handler from the myDialog class.
This is the handler called when the user double-clicks a string
in the listbox. We first determine which, if any, of the listbox
strings is currently selected. Note GetCurSel() can only be used
with a single-selection listbox. The actual string is then
retrieved from the listbox. The dimension of the item's rectangle
is determined since this will be the size and position of our
SmartEdit. The SmartEdit is created on the heap, and its Create()
function called which creates the Windows edit control and attaches
it to the CEdit object.

Figure 2 "SmartEdit.cpp implementation"


SmartEdit::SmartEdit( ) :
           m_bEscapeKey( FALSE )
{
}

BEGIN_MESSAGE_MAP( SmartEdit, CEdit )
//{{AFX_MSG_MAP(SmartEdit)
ON_WM_KILLFOCUS()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void
SmartEdit::OnKillFocus( CWnd *pWndGetFocus )
{
   PostMessage( WM_CLOSE, 0, 0 );
   if ( m_bEscapeKey == FALSE )
   {
      CString cs;
      GetWindowText( cs );
      COleDataSource *pds = new COleDataSource;
      PTCHAR cp = (PTCHAR) GlobalAlloc( GMEM_FIXED,
                     (cs.GetLength( ) * sizeof( TCHAR )) + sizeof( TCHAR ) );
      _tcscpy( cp, cs );
      pds->CacheGlobalData( CF_TEXT, cp );
      pds->SetClipboard( );
      GetOwner( )->PostMessage( EDITCLASSMSG );
      TRACE1( "SmartEdit::OnKillFocus  Posting message to my owner."
                "I have put [%s] on the clipboard for him\n", cp );
   }
}

// "override" base class member function
void
SmartEdit::PostNcDestroy( )
{
   delete( this );
}

// "augment" base class member function
BOOL
SmartEdit::PreTranslateMessage( MSG *pMsg )
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
      return( bEscapeKey = TRUE );
   }

   return( CEdit::PreTranslateMessage( pMsg ) );
}

Figure 2 shows SmartEdit's implementation. It is straightforward, but
perhaps the PostNcDestroy function needs further explanation. Remember
this object was created on the heap with "new" in myDialog. That means
someone must "delete" it. Since PostNcDestroy() is called after the
window has been destroyed, it is a perfect time to delete the object.
Modeless dialogs use this method of self-destruction.

The most important action occurs in its OnKillFocus( ) handler. When the
SmartEdit loses focus by a means other than the key, it retrieves the
new string with GetWindowText( ). The string is then placed on the clipboard
using a COleDataSource. COleDataSource is a source actor in OLE data
transfer. Notice I used PTCHAR, and _tcscpy( ) to allow the code to be
transparantly compiled in both ANSI and UNICODE applications.  After the
string is placed on the clipboard, it sends EDITCLASSMSG (simply #defined
as WM_APP + 100) to its parent.

Figure 3 "myDialog.cpp EDITCLASSMSG handling code"


BOOL
myDialog::PreTranslateMessage( MSG *pMsg )
{
   if ( EDITCLASSMSG == pMsg->message )
   {
      COleDataObject data;

      hClipboard( ) )
      {
         if ( data.IsDataAvailable( CF_TEXT ) )
         {
            HGLOBAL hg;
            if ( hg = data.GetGlobalData( CF_TEXT ) )
            {
               CString cs = (LPCTSTR) GlobalLock( hg );
               TRACE1( "myDialog::PreTrans(EDITCLASSMSG)  This is on the clipboard [%s]\n", cs );
               GlobalUnlock( hg );
               NewString( cs ); // Appropriate action
            }
         }

         data.Release( );
      }

      return( TRUE );
   }

   return( CDialog::PreTranslateMessage( pMsg ) );
}

Figure 3 shows what myDialog does when handling a EDITCLASSMSG message. It
knows there's a string on the clipboard, so uses COleDataObject to retrieve
it. COleDataObject is the destination actor in OLE data transfer. The string
from the clipboard is simply passed to a helper function, New String() in
this case, which will decide a course of action. To replace the string that
was 'underneath' the string double-clicked in the listbox, you would keep
the item index obtained in OnDblclkStringInListBox(). For example,
"const INT nIdx = m_Ctl.GetCurSel();" could be replaced by
"m_nIdx = m_Ctl.GetCurSel();" after which NewString() would use
myDialog's member variable 'm_nIdex' as the index for the new string.

We now have a fully functional SmartEdit capable of notifying its parent
(using OLE) that it has accepted a string. One problem still remains.
Can you spot it? Since the SmartEdit object 'kills' itself using 'delete'
in PostNcDestroy(), it *must* be created on the heap using 'new'. If we
don't enforce this, someone will inevitably use our SmartEdit in a
DDX_Control(). This will cause serious havoc when the "delete this;" line
is executed. So what can we do?

The answer is simple. make the ~SmartEdit destructor protected. This still
allows the class to be used in inheritance, but prevents its usage on the
stack.

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#else

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZInPlaceEdit

BEGIN_MESSAGE_MAP( ZInPlaceEdit, CEdit )
   //{{AFX_MSG_MAP( ZInPlaceEdit )
   ON_WM_KILLFOCUS()
   ON_WM_CHAR()
   ON_WM_CREATE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

ZInPlaceEdit::ZInPlaceEdit( int nItem, int nSubItem, CString csInitText ) :
              m_csInitText( csInitText )
{
   m_nItem = nItem;
   m_nSubItem = nSubItem;
   m_bESC = FALSE;
}

ZInPlaceEdit::~ZInPlaceEdit( )
{
}

/////////////////////////////////////////////////////////////////////////////
// ZInPlaceEdit message handlers

BOOL
ZInPlaceEdit::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN )
   {
      if ( pMsg->wParam == VK_RETURN ||
           pMsg->wParam == VK_DELETE ||
           pMsg->wParam == VK_ESCAPE ||
           GetKeyState( VK_CONTROL) )
      {
         ::TranslateMessage( pMsg );

         // Strange but true:
         // If the edit control has ES_MULTILINE and ESC
         // is pressed the parent is destroyed if the
         // message is dispatched.  In this
         // case the parent is the list control.

         if ( (GetStyle( ) & ES_MULTILINE) == 0 ||
              pMsg->wParam != VK_ESCAPE )
         {
            ::DispatchMessage( pMsg );
         }

         return( TRUE );            // DO NOT process further
      }
   }

   return( CEdit::PreTranslateMessage( pMsg ) );
}

void
ZInPlaceEdit::OnKillFocus( CWnd *pNewWnd )
{
   CEdit::OnKillFocus( pNewWnd );

   CString cs;
   GetWindowText( cs );

   // Send Notification to parent of ListView ctrl
   LV_DISPINFO dispinfo;
   dispinfo.hdr.hwndFrom = GetParent( )->m_hWnd;
   dispinfo.hdr.idFrom = GetDlgCtrlID( );
   dispinfo.hdr.code = LVN_ENDLABELEDIT;

   dispinfo.item.mask = LVIF_TEXT;
   dispinfo.item.iItem = m_nItem;
   dispinfo.item.iSubItem = m_nSubItem;
   dispinfo.item.pszText = m_bESC ? 0 : LPTSTR( (LPCTSTR) cs );
   dispinfo.item.cchTextMax = m_bESC ? 0 : cs.GetLength( );

   GetParent( )->GetParent( )->SendMessage( WM_NOTIFY,
                                            GetParent( )->GetDlgCtrlID( ),
                                            (LPARAM) &dispinfo );
}

void
ZInPlaceEdit::OnChar( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   if ( uChar == VK_ESCAPE || uChar == VK_RETURN )
   {
      if ( uChar == VK_ESCAPE )
         m_bESC = TRUE;

      GetParent( )->SetFocus( );
      return;
   }

   CEdit::OnChar( uChar, uRepCnt, uFlags );

   // Resize edit control if needed.
   CalculateSize( );
}

int
ZInPlaceEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CEdit::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   // Set the proper font.
   CFont *pFont = GetParent( )->GetFont( );
   SetFont( pFont );

   SetWindowText( m_csInitText );
   SetFocus( );
   CalculateSize( );
   SetSel( 0, -1 );
   return( nRC );
}

void
ZInPlaceEdit::CalculateSize( )
{
   // Get text extent.
   CString cs;

   GetWindowText( cs );
   CWindowDC dc( this );
   CFont *pFont = GetParent( )->GetFont( );
   CFont *pFontDC = dc.SelectObject( pFont );
   CSize size;

   // Get client rect.
   CRect rect;
   CRect rectParent;
   GetClientRect( &rect );
   GetParent( )->GetClientRect( &rectParent );

   // Transform rect to parent coordinates.
   ClientToScreen( &rect );
   GetParent( )->ScreenToClient( &rect );

   if ( (GetStyle( ) & ES_MULTILINE) == 0 )
   {
      size = dc.GetTextExtent( cs );
      dc.SelectObject( pFontDC );
      size.cx += 5;            // add some extra buffer
   }
   else
   {
      CRect rectThin( rect );  // measure the skinniest text box
      CRect rectWide( rect );  // measure the widest text box
      rectWide.right = rectParent.right;

      // Use the shortest of the two box sizes.
      int nThinHeight = dc.DrawText( cs, &rectThin,
                                     DT_CALCRECT | DT_NOPREFIX | DT_LEFT |
                                       DT_EXPANDTABS | DT_WORDBREAK );
      int nWideHeight = dc.DrawText( cs, &rectWide,
                                     DT_CALCRECT | DT_NOPREFIX | DT_LEFT |
                                       DT_EXPANDTABS | DT_WORDBREAK );
      if ( nThinHeight >= nWideHeight )
      {
         size.cy = nWideHeight + 5;
         size.cx = rectWide.right - rectWide.left + 5;
      }
      else
      {
         size.cy = nThinHeight + 5;
         size.cx = rectThin.right - rectThin.left + 5;
      }
   }

   // Check whether control needs to be resized and whether there is space
   // to grow.
   zBOOL bChanged = FALSE;
   if ( size.cx > rect.Width( ) )
   {
      if ( size.cx + rect.left < rectParent.right - 2 )
         rect.right = rect.left + size.cx;
      else
         rect.right = rectParent.right - 2;

      bChanged = TRUE;
   }

   if ( size.cy > rect.Height( ) )
   {
      if ( size.cy + rect.top < rectParent.bottom - 2 )
         rect.bottom = rect.top + size.cy;
      else
      {
         rect.bottom = rectParent.bottom - 2;
         ShowScrollBar( SB_VERT );
      }

      bChanged = TRUE;
   }

   // If the size became larger reposition the window.
   if ( bChanged )
      MoveWindow( &rect );
   }

#endif

/////////////////////////////////////////////////////////////////////////////
// QInPlaceEdit declares four private variables.  These are used when the
// control sends the LVN_ENDLABELEDIT notification.

/////////////////////////////////////////////////////////////////////////////
// QInPlaceEdit window

/////////////////////////////////////////////////////////////////////////////
//
// The listing of the implementation file now follows.  The QInPlaceEdit
// constructor simply saves the values passed through its arguments and
// initializes m_bESC to false.  The overridden PreTranslateMessage( ) is
// to ascertain that certain key strokes do make it to the edit control.
// The escape key and the enter key are normally pre-translated by the
// CDialog or the CFormView object, we therefore specifically check for
// these and pass it on to the edit control.  The check for
// GetKeyState( VK_CONTROL) makes sure that key combinations such as
// Ctrl-C, Ctrl-V and Ctrl-X get forwarded to the edit control.
// OnKillFocus( ) sends of the LVN_ENDLABELEDIT notification and destroys
// the edit control.  The notification is sent to the parent of the list
// view control and not to the list view control itself. When sending the
// notification, the m_bESC member variable is used to determine whether
// to send a NULL string.
//
// The OnNcDestroy( ) function is the appropriate place to destroy the C++
// object.
//
// The OnChar( ) function ends the edit if the Escape or the Enter key is
// pressed.  It does this by setting focus to the list view control which
// force the OnKillFocus( ) of the edit control to be called.  For any other
// character, the OnChar( ) function lets the base class function take care
// of it before it tries to determine if the control needs to be resized.
// The function first gets the extent of the new string using the proper
// font and then compares it to the current dimension of the edit control.
// If the string is too long to fit within the edit control, it resizes the
// edit control after checking the parent window (the list view control) to
// determine if there is space for the edit control to grow.
//
// The OnCreate( ) function creates the edit control and initializes it with
// the proper values.
//

/////////////////////////////////////////////////////////////////////////////
// QInPlaceEdit

BEGIN_MESSAGE_MAP( QInPlaceEdit, CEdit )
   //{{AFX_MSG_MAP( QInPlaceEdit )
   ON_WM_KILLFOCUS()
   ON_WM_NCDESTROY()
   ON_WM_CHAR()
   ON_WM_CREATE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

QInPlaceEdit::QInPlaceEdit( int nItem, int nSubItem, CString csInitText ) :
              m_csInitText( csInitText )
{
   m_nItem = nItem;
   m_nSubItem = nSubItem;
   m_bESC = FALSE;
}

QInPlaceEdit::~QInPlaceEdit( )
{
}

/////////////////////////////////////////////////////////////////////////////
// QInPlaceEdit message handlers

BOOL
QInPlaceEdit::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN )
   {
      if ( pMsg->wParam == VK_RETURN ||
           pMsg->wParam == VK_DELETE ||
           pMsg->wParam == VK_ESCAPE ||
           GetKeyState( VK_CONTROL) )
      {
         ::TranslateMessage( pMsg );
         ::DispatchMessage( pMsg );
         return( TRUE );            // DO NOT process further
      }
   }

   return( CEdit::PreTranslateMessage( pMsg ) );
}

void
QInPlaceEdit::OnKillFocus( CWnd *pNewWnd )
{
   CEdit::OnKillFocus( pNewWnd );

   CString cs;
   GetWindowText( cs );

   // Send Notification to parent of ListView ctrl.
   LV_DISPINFO dispinfo;
   dispinfo.hdr.hwndFrom = GetParent( )->m_hWnd;
   dispinfo.hdr.idFrom = GetDlgCtrlID( );
   dispinfo.hdr.code = LVN_ENDLABELEDIT;

   dispinfo.item.mask = LVIF_TEXT;
   dispinfo.item.iItem = m_nItem;
   dispinfo.item.iSubItem = m_nSubItem;
   dispinfo.item.pszText = m_bESC ? 0 : LPTSTR( (LPCTSTR) cs );
   dispinfo.item.cchTextMax = cs.GetLength( );

   GetParent( )->GetParent( )->SendMessage( WM_NOTIFY,
                                            GetParent( )->GetDlgCtrlID( ),
                                            (LPARAM) &dispinfo );
   DestroyWindow( );
}

void
QInPlaceEdit::OnNcDestroy( )
{
   CEdit::OnNcDestroy( );
   delete( this );
}

void
QInPlaceEdit::OnChar( UINT uChar, UINT uRepCnt, UINT uFlags )
{
   if ( uChar == VK_ESCAPE || uChar == VK_RETURN )
   {
      if ( uChar == VK_ESCAPE )
         m_bESC = TRUE;

      GetParent( )->SetFocus( );
      return;
   }

   CEdit::OnChar( uChar, uRepCnt, uFlags );

   // Resize edit control if needed.
   // Get text extent.
   CString cs;

   GetWindowText( cs );
   CWindowDC dc( this );
   CFont *pFont = GetParent( )->GetFont( );
   CFont *pFontDC = dc.SelectObject( pFont );
   CSize size = dc.GetTextExtent( cs );
   dc.SelectObject( pFontDC );
   size.cx += 5;              // add some extra buffer

   // Get client rect.
   CRect rect, rectParent;
   GetClientRect( &rect );
   GetParent( )->GetClientRect( &rectParent );

   // Transform rect to parent coordinates.
   ClientToScreen( &rect );
   GetParent()->ScreenToClient( &rect );

   // Check whether control needs to be resized
   // and whether there is space to grow.
   if ( size.cx > rect.Width( ) )
   {
      if ( size.cx + rect.left < rectParent.right )
         rect.right = rect.left + size.cx;
      else
         rect.right = rectParent.right;

      MoveWindow( &rect );
   }
}

int
QInPlaceEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CEdit::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   // Set the proper font.
   CFont *pFont = GetParent( )->GetFont( );
   SetFont( pFont );

   SetWindowText( m_csInitText );
   SetFocus( );
   SetSel( 0, -1 );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CxSplitterWnd

BEGIN_MESSAGE_MAP( CxSplitterWnd, CWnd )
   //{{AFX_MSG_MAP( CxSplitterWnd )
   ON_WM_SETCURSOR( )
   ON_WM_LBUTTONDOWN( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CxSplitterWnd::CxSplitterWnd( )
{
}

CxSplitterWnd::~CxSplitterWnd( )
{
}

BOOL
CxSplitterWnd::BindWithControl( CWnd *pParent, DWORD ctrlId )
{
   m_pParent = pParent;
   m_minLeft = m_minRight = 10;

   SubclassWindow( m_pParent->GetDlgItem( ctrlId )->GetSafeHwnd( ) );

   // Make sure to get mouse message from the dialog window.
   DWORD style = GetStyle( );
   ::SetWindowLong( GetSafeHwnd( ), GWL_STYLE, style | SS_NOTIFY );

   return( TRUE );
}

void
CxSplitterWnd::Unbind( )
{
   DetachAllPanes( );
   UnsubclassWindow( );
}

void
CxSplitterWnd::SetMinWidth( int left, int right )
{
   m_minLeft = left;
   m_minRight = right;
}

BOOL
CxSplitterWnd::AttachAsLeftPane( DWORD ctrlId )
{
   m_leftIds.Add( ctrlId );
   return( TRUE );
}

BOOL
CxSplitterWnd::AttachAsRightPane( DWORD ctrlId )
{
   m_rightIds.Add( ctrlId );
   return( TRUE );
}

BOOL
CxSplitterWnd::DetachAllPanes( )
{
   m_leftIds.RemoveAll( );
   m_rightIds.RemoveAll( );
   return( TRUE );
}

void
CxSplitterWnd::RecalcLayout( )
{
   CWnd *pane;
   RECT rectBar, rectPane;

   GetWindowRect( &rectBar );
   m_pParent->ScreenToClient( &rectBar );

   int   k;
   DWORD dwId;

   for ( k = 0; k < m_leftIds.GetSize( ); k++ )
   {
      dwId = m_leftIds.GetAt( k );
      pane = m_pParent->GetDlgItem( dwId );
      pane->GetWindowRect( &rectPane );
      m_pParent->ScreenToClient( &rectPane );
      rectPane.right = rectBar.left - 1;
      pane->MoveWindow( &rectPane, FALSE );
   }

   for ( k = 0; k < m_rightIds.GetSize( ); k++ )
   {
      dwId = m_rightIds.GetAt( k );
      pane = m_pParent->GetDlgItem( dwId );
      pane->GetWindowRect( &rectPane );
      m_pParent->ScreenToClient( &rectPane );
      rectPane.left = rectBar.right + 1;
      pane->MoveWindow( &rectPane, FALSE );
   }

   m_pParent->Invalidate( );
}

BOOL
CxSplitterWnd::GetMouseClipRect( LPRECT rectClip, CPoint& pt )
{
   RECT rectOrg, rectTarget, rectParent, rectPane;
   int   k;
   DWORD dwId;

   GetWindowRect( &rectOrg );
   m_pParent->GetClientRect( &rectParent );
   m_pParent->ClientToScreen( &rectParent );

   rectTarget = rectOrg;
   rectTarget.left = rectParent.left + m_minLeft;
   for ( k = 0; k < m_leftIds.GetSize( ); k++ )
   {
      dwId = m_leftIds.GetAt( k );
      m_pParent->GetDlgItem( dwId )->GetWindowRect( &rectPane );
      if ( rectTarget.left < rectPane.left + m_minLeft )
         rectTarget.left = rectPane.left + m_minLeft;
   }

   rectTarget.right = rectParent.right - m_minRight;
   for ( k = 0; k < m_rightIds.GetSize( ); k++ )
   {
      dwId = m_rightIds.GetAt( k );
      m_pParent->GetDlgItem( dwId )->GetWindowRect( &rectPane );
      if ( rectTarget.right > rectPane.right - m_minRight )
         rectTarget.right = rectPane.right - m_minRight;
   }

   if ( rectTarget.left >= rectTarget.right )
   {
      TRACE( "No room to drag the x-splitter bar" );
      return( FALSE );
   }

   rectClip->left = rectTarget.left + pt.x;
   rectClip->right = rectTarget.right -
                                (rectOrg.right - rectOrg.left - pt.x) + 1;
   rectClip->top = rectOrg.top;
   rectClip->bottom = rectOrg.bottom;

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// CxSplitterWnd message handlers

BOOL
CxSplitterWnd::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage )
{
   // TODO: Add your message handler code here and/or call default
// ::SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZENS ) );
   ::SetCursor( AfxGetApp( )->LoadCursor( IDC_HSPLIT ) );
   return( TRUE );

// return( CWnd::OnSetCursor( pWnd, nHitTest, uMessage ) );
}

void
CxSplitterWnd::OnLButtonDown( UINT uFlags, CPoint pt )
{
   // Don't handle if capture already set.
   if ( ::GetCapture( ) )
      return;

   // Don't handle if no room to drag.
   RECT rectMouseClip;
   if ( GetMouseClipRect( &rectMouseClip, pt ) == 0 )
      return;

   ::ClipCursor( &rectMouseClip );

   // Set capture to the window which received this message.
   SetCapture( );
   ASSERT( CWnd::GetCapture( ) == this );

   // Get DC for drawing.
   CDC *pDrawDC;
   pDrawDC = m_pParent->GetDC( );
   ASSERT_VALID( pDrawDC );

   int    nCurrX, nCurrY;
   int    nDiffX, nDiffY;
   CRect  rectOrg, rectCur, rectOld;
   CSize  sizeBar;

   GetWindowRect( rectOrg );
   sizeBar = CSize( rectOrg.Width( ), rectOrg.Height( ) );

   m_pParent->ScreenToClient( rectOrg );
   pDrawDC->DrawDragRect( &rectOrg, sizeBar, 0, sizeBar );
   rectOld = rectCur = rectOrg;
   nDiffX = nDiffY = 0;

   // Get messages until capture lost or cancelled/accepted.
   for ( ; ; )
   {
      MSG msg;
      VERIFY( ::GetMessage( &msg, 0, 0, 0 ) );

      if ( CWnd::GetCapture( ) != this )
        break;

      switch ( msg.message )
      {
         // Handle movement/accept messages.
         case WM_MOUSEMOVE:
            // Handle resize cases (and part of move).
            nCurrX = (int)(short) LOWORD( msg.lParam );
            nCurrY = (int)(short) HIWORD( msg.lParam );

            nDiffX = nCurrX - pt.x;
            nDiffY = nCurrY - pt.y;

            rectCur = rectOrg;
            rectCur.left += nDiffX;
            rectCur.right += nDiffX;
            pDrawDC->DrawDragRect( &rectCur, sizeBar, &rectOld, sizeBar );
            rectOld = rectCur;

            break;

         // Handle cancel messages.
         case WM_KEYDOWN:
            if ( msg.wParam != VK_ESCAPE )
               break;

         case WM_LBUTTONUP:
         case WM_RBUTTONDOWN:
            goto ExitLoop;

         // Just dispatch rest of the messages.
         default:
            DispatchMessage( &msg );
            break;
      }
   }

ExitLoop:
   pDrawDC->DrawDragRect( &rectCur, sizeBar, 0, sizeBar );

   m_pParent->ReleaseDC( pDrawDC );
   ReleaseCapture( );
   ::ClipCursor( 0 );

   if ( nDiffX == 0 )
      return;

   // Move the splitter bar & re-position the attached panes if necessary.
   MoveWindow( rectCur, FALSE );
   RecalcLayout( );

   m_pParent->SendMessage( WM_SPLITTER_MOVED, nDiffX, GetDlgCtrlID( ) );

   //CWnd::OnLButtonDown( uFlags, pt );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CySplitterWnd

BEGIN_MESSAGE_MAP( CySplitterWnd, CWnd )
   //{{AFX_MSG_MAP( CySplitterWnd )
   ON_WM_LBUTTONDOWN( )
   ON_WM_SETCURSOR( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CySplitterWnd::CySplitterWnd( )
{
}

CySplitterWnd::~CySplitterWnd( )
{
}

BOOL
CySplitterWnd::BindWithControl( CWnd *pParent, DWORD ctrlId )
{
   m_pParent = pParent;

   SubclassWindow( m_pParent->GetDlgItem( ctrlId )->GetSafeHwnd( ) );

   // Make sure to get mouse message from the dialog window
   DWORD style = GetStyle( );
   ::SetWindowLong( GetSafeHwnd( ), GWL_STYLE, style | SS_NOTIFY );

   return( TRUE );
}

void
CySplitterWnd::Unbind( )
{
   DetachAllPanes( );
   UnsubclassWindow( );
}

void
CySplitterWnd::SetMinHeight( int above, int below )
{
   m_minAbove = above;
   m_minBelow = below;
}

BOOL
CySplitterWnd::AttachAsAbovePane( DWORD ctrlId )
{
   m_aboveIds.Add( ctrlId );
   return( TRUE );
}

BOOL
CySplitterWnd::AttachAsBelowPane( DWORD ctrlId )
{
   m_belowIds.Add( ctrlId );
   return( TRUE );
}

BOOL
CySplitterWnd::DetachAllPanes( )
{
   m_aboveIds.RemoveAll( );
   m_belowIds.RemoveAll( );
   return( TRUE );
}

void
CySplitterWnd::RecalcLayout( )
{
   CWnd *pane;
   RECT rectBar, rectPane;

   GetWindowRect( &rectBar );
   m_pParent->ScreenToClient( &rectBar );

   int   k;
   DWORD dwId;

   for ( k = 0; k < m_aboveIds.GetSize( ); k++ )
   {
      dwId = m_aboveIds.GetAt( k );
      pane = m_pParent->GetDlgItem( dwId );
      pane->GetWindowRect( &rectPane );
      m_pParent->ScreenToClient( &rectPane );
      rectPane.bottom = rectBar.top - 1;
      pane->MoveWindow( &rectPane, FALSE );
   }

   for ( k = 0; k < m_belowIds.GetSize( ); k++ )
   {
      dwId = m_belowIds.GetAt( k );
      pane = m_pParent->GetDlgItem( dwId );
      pane->GetWindowRect( &rectPane );
      m_pParent->ScreenToClient( &rectPane );
      rectPane.top = rectBar.bottom + 1;
      pane->MoveWindow( &rectPane, FALSE );
   }

   m_pParent->Invalidate( );
}

BOOL
CySplitterWnd::GetMouseClipRect( LPRECT rectClip, CPoint& pt )
{
   RECT  rectOrg, rectTarget, rectParent, rectPane;
   int   k;
   DWORD dwId;

   GetWindowRect( &rectOrg );
   m_pParent->GetClientRect( &rectParent );
   m_pParent->ClientToScreen( &rectParent );

   rectTarget = rectOrg;
   rectTarget.top = rectParent.top + m_minAbove;
   for ( k = 0; k < m_aboveIds.GetSize( ); k++ )
   {
      dwId = m_aboveIds.GetAt( k );
      m_pParent->GetDlgItem( dwId )->GetWindowRect( &rectPane );
      if ( rectTarget.top < rectPane.top + m_minAbove )
         rectTarget.top = rectPane.top + m_minAbove;
   }

   rectTarget.bottom = rectParent.bottom - m_minBelow;
   for ( k = 0; k < m_belowIds.GetSize( ); k++ )
   {
      dwId = m_belowIds.GetAt( k );
      m_pParent->GetDlgItem( dwId )->GetWindowRect( &rectPane );
      if ( rectTarget.bottom > rectPane.bottom - m_minBelow )
         rectTarget.bottom = rectPane.bottom - m_minBelow;
   }

   if ( rectTarget.top >= rectTarget.bottom )
   {
      TRACE( "No room to drag the y-splitter bar" );
      return( FALSE );
   }

   rectClip->left = rectOrg.left;
   rectClip->right = rectOrg.right;
   rectClip->top = rectTarget.top + pt.y;
   rectClip->bottom = rectTarget.bottom -
                                (rectOrg.bottom - rectOrg.top - pt.y) + 1;

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// CySplitterWnd message handlers

BOOL
CySplitterWnd::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT message )
{
   // TODO: Add your message handler code here and/or call default
// ::SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZEWE ) );
   ::SetCursor( AfxGetApp( )->LoadCursor( IDC_VSPLIT ) );
   return( TRUE );

   //return( CWnd::OnSetCursor( pWnd, nHitTest, message ) );
}

void
CySplitterWnd::OnLButtonDown( UINT /*uFlags*/, CPoint pt )
{
   // TODO: Add your message handler code here and/or call default

   // Don't handle if capture already set.
   if ( ::GetCapture( ) )
      return;

   // Don't handle if no room to drag.
   RECT rectMouseClip;
   if ( GetMouseClipRect( &rectMouseClip, pt ) == FALSE )
      return;

   ::ClipCursor( &rectMouseClip );

   // Set capture to the window which received this message.
   SetCapture( );
   ASSERT( CWnd::GetCapture( ) == this );

   // Get DC for drawing.
   CDC *pDrawDC;
   pDrawDC = m_pParent->GetDC( );
   ASSERT_VALID( pDrawDC );

   int    nCurrX, nCurrY;
   int    nDiffX, nDiffY;
   CRect  rectOrg, rectCur, rectOld;
   CSize  sizeBar;

   GetWindowRect( rectOrg );
   sizeBar = CSize( rectOrg.Width( ), rectOrg.Height( ) );

   m_pParent->ScreenToClient( rectOrg );
   pDrawDC->DrawDragRect( &rectOrg, sizeBar, 0, sizeBar );
   rectOld = rectCur = rectOrg;
   nDiffX = nDiffY = 0;

   // Get messages until capture lost or cancelled/accepted.
   for ( ; ; )
   {
      MSG msg;
      VERIFY( ::GetMessage( &msg, 0, 0, 0 ) );

      if ( CWnd::GetCapture( ) != this )
        break;

      switch ( msg.message )
      {
         // Handle movement/accept messages.
         case WM_MOUSEMOVE:
            // Handle resize cases (and part of move).
            nCurrX = (int)(short) LOWORD( msg.lParam );
            nCurrY = (int)(short) HIWORD( msg.lParam );

            nDiffX = nCurrX - pt.x;
            nDiffY = nCurrY - pt.y;

            rectCur = rectOrg;
            rectCur.top += nDiffY;
            rectCur.bottom += nDiffY;
            pDrawDC->DrawDragRect( &rectCur, sizeBar, &rectOld, sizeBar );
            rectOld = rectCur;

            break;

         // Handle cancel messages.
         case WM_KEYDOWN:
            if ( msg.wParam != VK_ESCAPE )
               break;

         case WM_LBUTTONUP:
         case WM_RBUTTONDOWN:
            goto ExitLoop;

         // just dispatch rest of the messages
         default:
            DispatchMessage( &msg );
            break;
      }
   }

ExitLoop:
   pDrawDC->DrawDragRect( &rectCur, sizeBar, 0, sizeBar );

   m_pParent->ReleaseDC( pDrawDC );
   ReleaseCapture( );
   ::ClipCursor( 0 );

   if ( nDiffY == 0 )
      return;

   // Move the splitter bar & re-position the attached panes if necessary
   MoveWindow( rectCur, FALSE );
   RecalcLayout( );

   m_pParent->SendMessage( WM_SPLITTER_MOVED, nDiffY, GetDlgCtrlID( ) );

// CWnd::OnLButtonDown( uFlags, pt );
}

AFX_EXT_API
CWnd * OPERATION                 // WINAPI
Calendar( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   if ( zstrcmp( pCtrlDef->Tag, "MultiCal" ) == 0 )
   return( new ZMultiCal( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
   else
   return( new ZCalendar( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}
