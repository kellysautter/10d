/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctltim.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Timer control and TimeCtrl.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2000.12.14    DKS   Becker
//    If the application developer sets zCONTROL_STATUS_ENABLED to Enabled
//    with a value of something other than 0 or 1, that value is used as the
//    timer interval (in milli-seconds).
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlTim.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZTimer, CStatic )

BEGIN_MESSAGE_MAP( ZTimer, CStatic )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

#if 0  // Example

void CALLBACK EXPORT
fnTimerProc( HWND  hWnd,      // handle of CWnd that called SetTimer
             UINT  nMsg,      // WM_TIMER
             UINT  nIDEvent   // timer identification
             DWORD dwTime )   // system time
{
}

// This example shows how to use CWnd::SetTimer, CWnd::KillTimer,
// and how to handle WM_TIMER messages.  A timer is set up to send
// a WM_TIMER message to the main frame window every 2 seconds in
// OnStartTimer().  OnStopTimer will stop the timer by calling
// CWnd::KillTimer.  OnTimer was set up through class wizard to
// handle WM_TIMER messages for the main frame window.  In this
// example the PC speaker will beep every 2 seconds.

void
CMainFrame::OnStartTimer( )
{
   m_nTimer = SetTimer( 1, 2000, 0 );
}

void
CMainFrame::OnStopTimer( )
{
   KillTimer( m_nTimer );
}

void
CMainFrame::OnTimer( UINT nIDEvent )
{
   ::MessageBeep( MB_ICONQUESTION );   // Beep

   // Call base class handler.
   CMDIFrameWnd::OnTimer( nIDEvent );
}
#endif

UINT
fnTimerThreadProc( zPVOID p )
{
   ZTimer  *pZTimer = (ZTimer *) p;
   HWND    hWnd = pZTimer->m_hWnd;
   zUSHORT uInterval = (zUSHORT) pZTimer->m_lInterval;
// DWORD   dwStart = GetTickCount( );

   // Stop if this has taken too long
// if( GetTickCount() - dwStart >= TIMELIMIT )
//     Cancel();

#ifdef DEBUG_ALL
   TraceLineI( "Starting Thread: ", (zLONG) AfxGetThread( ) );
#endif

   while ( mIs_hWnd( hWnd ) && pZTimer->m_bEnabled )
   {
      SysWait( uInterval );
      if ( mIs_hWnd( hWnd ) )
         ProcessImmediateEvent( pZTimer, 1, 0 );
   }

#ifdef DEBUG_ALL
   TraceLineI( "All Done!!! Exiting Thread: ", (zLONG) AfxGetThread( ) );
#endif

   if ( mIs_hWnd( hWnd ) )
      pZTimer->m_pThread = 0;

   return( 0 );

} // fnTimerThreadProc

// ZTimer - ctor
ZTimer::ZTimer( ZSubtask *pZSubtask,
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
        CStatic( ),
        ZMapAct( pZSubtask,             // base class ctor
                 pzmaComposite,
                 pWndParent,
                 this,
                 vDialog,
                 nOffsetX,
                 nOffsetY,
                 pCtrlDef,
                 "Timer" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for ZTimer", (zLONG) this );
#endif

   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;

   // Make it a hidden window.
   Attr.Style &= ~(WS_TABSTOP | WS_VISIBLE );
   Attr.Style |= WS_DISABLED;
   Attr.X = 0;
   Attr.Y = 0;
   Attr.W = 0;
   Attr.H = 0;

   // Localize settings from XWD.
   m_lInterval = pCtrlDef->StyleX;
   m_bEnabled = TRUE;
   m_pThread = 0;

// TraceLineI( "Timer interval --> ", m_lInterval );
// TraceLineS( "string interval -> ", pCtrlDef->Text );
// TraceLineI( "Timer enabled ---> ", m_bEnabled );

#ifdef zREMOTE_SERVER
   zCHAR szInterval[ 32 ];

   _ltoa_s( m_lInterval, szInterval );
   SetRemoteZCtrlAttribute( this, "Timer", "Interval", szInterval );
#endif

   CreateZ( );
}

void
ZTimer::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTimer::CreateZ ", *m_pzsTag );
#endif

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( *m_pzsText, (Attr.Style & ~WS_BORDER),
                 CRect( 0, 0, 0, 0 ),
                 m_pWndParent, m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZTimer::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTimer::OnCreate ", *m_pzsTag );
#endif

   int nRC = CStatic::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   // Fire off a thread to handle timer.
   m_pThread = AfxBeginThread( fnTimerThreadProc,
                               (zPVOID) this,
                               THREAD_PRIORITY_NORMAL,
                               0, NULL );
   return( nRC );
}

BOOL
ZTimer::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTimer::DestroyWindow ", *m_pzsTag );
#endif

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CStatic::DestroyWindow( ) );
}

zSHORT
ZTimer::MapFromOI( WPARAM wFlag )
{
   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   return( 0 );
}

zLONG
ZTimer::SetZCtrlState( zLONG  lStatusType,
                       zULONG ulValue )
{
   zLONG lRC = ZMapAct::SetZCtrlState( lStatusType, ulValue );

   // Enabled is used to start/stop the timer thread
   if ( lStatusType == zCONTROL_STATUS_ENABLED )
   {
   // TraceLineI( "ZTimer::SetZCtrlState ENABLED: ", ulValue );

      if ( ulValue )
      {
         if ( ulValue > 1 )
         {
            m_lInterval = ulValue;

#ifdef zREMOTE_SERVER
            zCHAR szInterval[ 32 ];

            _ltoa_s( m_lInterval, szInterval );
            SetRemoteZCtrlAttribute( this, "Timer", "Interval", szInterval );
#endif
         }

         if ( m_bEnabled == FALSE )
         {
#ifdef zREMOTE_SERVER
            SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled", "Y" );
#else
            m_bEnabled = TRUE;
            m_pThread = AfxBeginThread( fnTimerThreadProc,
                                        (zPVOID) this,
                                        THREAD_PRIORITY_NORMAL,
                                        0, NULL );
#endif
         }
      }
      else
      {
         if ( m_bEnabled == TRUE )
         {
            m_bEnabled = FALSE;
#ifdef zREMOTE_SERVER
            SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled", "N" );
#endif
         }
      }
   }

   return( lRC );
}

// ZTimer - dtor
ZTimer::~ZTimer( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTimer::dtor ", *m_pzsTag );
#endif

   m_bEnabled = FALSE;
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZTime, CWnd )

BEGIN_MESSAGE_MAP( ZTime, CWnd )
   ON_WM_CREATE( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
END_MESSAGE_MAP( )

// ZTime - ctor
ZTime::ZTime( ZSubtask *pZSubtask,
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
                "Time" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for ZTime", (zLONG) this );
#endif

   // Localize settings from XWD.

   CreateZ( );
}

void
ZTime::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTime::CreateZ ", *m_pzsTag );
#endif

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else

   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( 0, "STATIC", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
                 CRect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + Attr.H ),
                 m_pWndParent, m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZTime::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTime::OnCreate ", *m_pzsTag );
#endif

   int nRC = CWnd::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   CRect rect;
   zPCHAR pch;

   GetWindowRect( rect );
   ScreenToClient( rect );
   m_DateTime.Create( Attr.Style | DTS_TIMEFORMAT | DTS_UPDOWN,
                      rect, this, 1 );

   // The user can specify a format for the time using the control's Tag.
   // If there is an '@' in the Tag, the characters following the '@' are
   // used to specify the format using the following specifications:
   //
   // Element  Description
   // "d"    - The one- or two-digit day.
   // "dd"   - The two-digit day. Single-digit day values are preceded by a zero.
   // "ddd"  - The three-character weekday abbreviation.
   // "dddd" - The full weekday name.
   // "h"    - The one- or two-digit hour in 12-hour format.
   // "hh"   - The two-digit hour in 12-hour format. Single-digit values are preceded by a zero.
   // "H"    - The one- or two-digit hour in 24-hour format.
   // "HH"   - The two-digit hour in 24-hour format. Single-digit values are preceded by a zero.
   // "m"    - The one- or two-digit minute.
   // "mm"   - The two-digit minute. Single-digit values are preceded by a zero.
   // "M"    - The one- or two-digit month number.
   // "MM"   - The two-digit month number. Single-digit values are preceded by a zero.
   // "MMM"  - The three-character month abbreviation.
   // "MMMM" - The full month name.
   // "t"    - The one-letter AM/PM abbreviation (that is, AM is displayed as "A").
   // "tt"   - The two-letter AM/PM abbreviation (that is, AM is displayed as "AM").
   // "yy"   - The last two digits of the year (that is, 1996 would be displayed as "96").
   // "yyyy" - The full year (that is, 1996 would be displayed as "1996").

   if ( (pch = (zPCHAR) zstrchr( *m_pzsTag, '@' )) != 0 )
      m_DateTime.SetFormat( pch + 1 );

   return( nRC );
}

BOOL
ZTime::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTime::DestroyWindow ", *m_pzsTag );
#endif

   m_DateTime.DestroyWindow( );
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

zSHORT
ZTime::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTime::MapFromOI: ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW  vApp;

      m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
      zUSHORT uMapState = GetMappingAutoGray( &vApp, *m_pzsVName,
                                              *m_pzsEName, *m_pzsAName );
   // TraceLineS( "ZMapAct::MapFromOI ", *m_pzsTag );
   // TraceLineX( "ZMapAct::MapFromOI vApp: ", (zLONG) vApp );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) )
      {
         COleDateTime time;
         GetOleDateTimeFromZeidonDateAttribute( time, vApp,
                                                *m_pzsEName, *m_pzsAName );
         m_DateTime.SetTime( time );
      }
   }

   return( 0 );
}

// Get the time from the time control and map it to the data OI.
zSHORT
ZTime::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZTime::MapToOI: ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      COleDateTime time;
      if ( m_DateTime.GetTime( time ) )
      {
         SetZeidonDateAttributeFromOleDateTime( time, vApp,
                                                *m_pzsEName, *m_pzsAName );
      }
   }

   return( 0 );
}

zLONG
ZTime::SetZCtrlState( zLONG  lStatusType,
                      zULONG ulValue )
{
   zLONG lRC = ZMapAct::SetZCtrlState( lStatusType, ulValue );
   return( lRC );
}

void
ZTime::OnSetFocus( CWnd *pWndLostFocus )
{
   m_DateTime.SetFocus( );
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
      Default( );             // pass it along to parent
}

void
ZTime::OnKillFocus( CWnd *pWndGetFocus )
{
   if ( pWndGetFocus == &m_DateTime )
   {
      // That's fine ... don't tell the driver cuz it should
      // keep on believing the Time has focus.
   }
   else
   {
      m_pZSubtask->InformDriverOfKillFocus( this );
   }

   Default( );                // pass it along to parent
}

// ZTime - dtor
ZTime::~ZTime( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZTime::dtor ", *m_pzsTag );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
Timer( ZSubtask *pZSubtask,
       CWnd     *pWndParent,
       ZMapAct  *pzmaComposite,
       zVIEW    vDialog,
       zSHORT   nOffsetX,
       zSHORT   nOffsetY,
       zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZTimer( pZSubtask, pWndParent,
                       pzmaComposite, vDialog,
                       nOffsetX, nOffsetY, pCtrlDef ) );
}
