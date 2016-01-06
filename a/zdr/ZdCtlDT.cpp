/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctldt.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon DayTimer control.
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

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlCal.h"
#include "ZdCtlDt.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

#define zHOLD_DATETIME_ALARM  zMAPACT_CTRL_SPECIFIC1

static const COLORREF g_clrTransparent = RGB( 255, 0, 255 );
static const int g_nImageWidth = 10;
static const int g_nHorzOffset = 10;
static const int g_nIdClickButtonEvt = 1;

static const int g_nSlowTimerDuration = 300;
static const int g_nFastTimerDuration = 100;
static const int g_nNumberOfSlowTimes = 10;

BOOL CALLBACK
IsSpecialDateCallback( ZDayTimer *pDayTimer, COleDateTime &date )
{
   zBOOL bRC = FALSE;

#if 1
   if ( pDayTimer && pDayTimer->m_pAgenda &&
        pDayTimer->m_pAgenda->GetAppointmentCountOnDate( date ) )
   {
      bRC = TRUE;
   }
#else
   COleDateTime dateNow = COleDateTime::GetCurrentTime( );
   COleDateTimeSpan span;

   if ( dateNow >= date )
      span = dateNow - date;
   else
      span = date - dateNow;

   if ( span.GetDays( ) <= zDAYS_IN_WEEK )
      bRC = TRUE;
#endif

   return( bRC );
}

CFont *
GetHeightFont( zCPCHAR cpcFaceName, int nPixelHeight, CDC *pDC )
{
   int nFontHeight = 0;
   int nPoint = 10;
   CFont *pFont = 0;

   while ( nFontHeight < nPixelHeight )
   {
      mDeleteInit( pFont );
      pFont = new CFont;
      VERIFY( pFont->CreatePointFont( nPoint, cpcFaceName ) );

      CFont *pOldFont = pDC->SelectObject( pFont );
      nFontHeight = pDC->GetTextExtent( "00" ).cy;
      pDC->SelectObject( pOldFont );

      nPoint += 10;
   }

   return( pFont );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDayTimer

IMPLEMENT_DYNAMIC( ZDayTimer, CWnd )
BEGIN_MESSAGE_MAP( ZDayTimer, CWnd )
   ON_WM_SIZE( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_TIMER( )
END_MESSAGE_MAP( )

CString ZDayTimer::m_csClassName = _T( "" );
#define DT_APPOINTMENT_TIMER_ID          1002
#define DT_APPOINTMENT_TIMER_INTERVAL   10000  // 10 seconds

// ZDayTimer - ctor
ZDayTimer::ZDayTimer( ZSubtask *pZSubtask,
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
   TraceLineS( "ZDayTimer::ctor ", *m_pzsTag );
#endif
   zSHORT nLth = m_pzsTag->GetLength( );

   if ( m_csClassName.IsEmpty( ) )
   {
      m_csClassName =
         ::AfxRegisterWndClass( CS_DBLCLKS,
                                ::LoadCursor( NULL, IDC_ARROW ),
                                ::GetSysColorBrush( COLOR_BTNFACE ) );
   }

// CtrlBOI has the format:
//   0 - Style - 4-byte long (3 unused) - MiniCal Appointment Both
//   4 - Rows - long
//   8 - Cols - long
//  12 - Size - long
//  16 - Max Appointment Days - long
//  20 - Type - (4-byte long) MultiSelect, PermitNull, Today, Position
//  24 - Selected Background Color - long
//  28 - Background Color - long
//  32 - 8 longs (reserved)
//  64 - Scoping Entity Name - char[ zTAG_LTH ]
//  97 - Scope - 3-byte char (2 unused) - Scope Entity OI
   zCHAR  szBlob[ 100 ];
   zULONG ulLth;

   GetAttributeLength( &ulLth, vDialog, szlCtrl, szlCtrlBOI );
   if ( ulLth = sizeof( szBlob ) )
   {
      GetVariableFromAttribute( szBlob, &ulLth, zTYPE_BLOB,
                                ulLth, vDialog, szlCtrl, szlCtrlBOI,
                                0, 0 );
   }
   else
   {
      zmemset( szBlob, 0, sizeof( szBlob ) );
   }

   m_uAppointmentTimerId = 0;
   m_pMiniCal = 0;
   m_pAgenda = 0;

   m_chStyle = szBlob[ 1 ]; // M - MiniCal; A - Appointment; B - Both

   strncat_s( pCtrlDef->Tag, zTAG_LTH, "_MiniCal", zTAG_LTH - (nLth + 1) );
   zLONG  lSubtype = pCtrlDef->Subtype;
   pCtrlDef->Subtype &= ~zCONTROL_FULL_CLIENT;
   pCtrlDef->Subtype &= ~zCONTROL_RELATIVE_POS_X;
   pCtrlDef->Subtype &= ~zCONTROL_RELATIVE_POS_Y;
   pCtrlDef->Subtype &= ~zCONTROL_RELATIVE_SIZE_X;
   pCtrlDef->Subtype &= ~zCONTROL_RELATIVE_SIZE_Y;

   if ( m_chStyle != 'A' )
   {
      m_pMiniCal = new ZMiniCalendar( this );
      m_pMiniCal->m_pDayTimer = this;
   }

   if ( m_chStyle != 'M' )
      m_pAgenda = new ZAgenda( this );

   strcpy_s( pCtrlDef->Tag, sizeof( pCtrlDef->Tag ), *m_pzsTag );
   pCtrlDef->Subtype = lSubtype;

   m_nRows = (zSHORT) *((zPLONG) (szBlob + 4));
   m_nCols = (zSHORT) *((zPLONG) (szBlob + 8));
   m_nMiniCalSize = (zSHORT) *((zPLONG) (szBlob + 12));

   m_nMaxShowAppointment = (zSHORT) *((zPLONG) (szBlob + 16));
   m_bMultipleSel = (szBlob[ 20 ] == 'M') ? TRUE : FALSE;
   m_bPermitNull = (szBlob[ 21 ] == 'N') ? TRUE : FALSE;
   m_bToday = (szBlob[ 22 ] == 'T') ? TRUE : FALSE;
   m_chAlignment = szBlob[ 23 ];
   m_clrSelBack = *((zPLONG) (szBlob + 24));
   if ( m_clrSelBack == 0 || m_clrSelBack == 0xFF000000 )
      m_clrSelBack = 0x00b3b562;  // what in the world is this???

   m_clrBackground = *((zPLONG) (szBlob + 28));
   if ( m_clrBackground == 0 || m_clrBackground == 0xFF000000 )
      m_clrBackground = RGB( 231, 231, 214 );

   strcpy_s( m_szScopeEntity, sizeof( m_szScopeEntity ), szBlob + 64 );
   m_chScope = szBlob[ 97 ];

   if ( m_nRows == 0 )
      m_nRows = 1;

   if ( m_nCols == 0 )
      m_nCols = 4;

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
ZDayTimer::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDayTimer::CreateZ ", *m_pzsTag );
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
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( lStyleEx, m_csClassName, *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;

      CSize sizeMax;
      CSize size( 0, 0 );
      int nSizeY = mConvertMapDlgToPixelY( m_nMiniCalSize );
      int nSizeX = mConvertPixelToMapDlgX( 652 );

      if ( m_nMiniCalSize <= 0 )
      {
         sizeMax = FMC_MAX_SIZE_PARENT;
      }
      else
      if ( m_nMiniCalSize == 1 )
      {
         sizeMax = FMC_MAX_SIZE_NONE;
         m_nMiniCalSize = 86;
         nSizeY = mConvertMapDlgToPixelY( m_nMiniCalSize );
      }
      else
      {
         sizeMax.cx = 9999;
         sizeMax.cy = nSizeY;
      }

      // Create the mini-calendar at the top of the day-timer.
      if ( m_pMiniCal )
      {
         ASSERT_VALID( m_pMiniCal );
         ASSERT_KINDOF( ZMiniCalendar, m_pMiniCal );

// not yet implemented!!!???
// zCHAR       m_chAlignment;
// zCHAR       m_chScope;
// zCHAR       m_szScopeEntity[ zTAG_LTH ];

         m_pMiniCal->EnableMultipleSelection( m_bMultipleSel );
         m_pMiniCal->EnableToday( m_bToday );
         m_pMiniCal->EnablePermitNull( m_bPermitNull );
         m_pMiniCal->SetBackColor( m_clrBackground );
         m_pMiniCal->SetMaxSelDays( m_nMaxShowAppointment );
         m_pMiniCal->m_clrSelBack = m_clrSelBack;
      // m_pMiniCal->SetUseAutoSettings( FALSE );


         m_pMiniCal->SetMaxSize( sizeMax );

         VERIFY( m_pMiniCal->Create( CRect( 0, 0, nSizeX, nSizeY ),
                                     this, 1 ) );

      // VERIFY( m_pMiniCal->Create( CRect( 0, 0, Attr.W, Attr.H ),
      //                             this, 1 ) );

         m_pMiniCal->SetCurrentDate( COleDateTime::GetCurrentTime( ) );
         m_pMiniCal->SetSpecialDaysCallback( IsSpecialDateCallback );

         m_pMiniCal->SetRowsAndColumns( m_nRows, m_nCols );
         m_pMiniCal->AutoConfigure( );
         m_pMiniCal->ReinitializeCells( );
         size = m_pMiniCal->ComputeTotalSize( );
      }

      // Create the 24 hour day-view below the mini-calendar.
      if ( m_pAgenda )
      {
         ASSERT_VALID( m_pAgenda );
         ASSERT_KINDOF( ZAgenda, m_pAgenda );

         RECT rect;

         rect.top = size.cy;
         rect.left = 0;
         rect.bottom = Attr.H;
         rect.right = Attr.W;

         m_pAgenda->Create( 0, "Agenda", WS_CHILD | WS_VISIBLE,
                             rect, this, 2 );
      }

      UpdatePositionOfControls( );
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
                   "ZDayTimer::CreateZ", pchMsg,
                   zMSGQ_MODAL_ERROR, FALSE );

      // Free the buffer.
      LocalFree( pchMsg );
#endif
   }
#endif
}

BOOL
ZDayTimer::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDayTimer::DestroyWindow ", *m_pzsTag );
#endif

   if ( mIs_hWnd( m_hWnd ) && m_uAppointmentTimerId )
   {
      KillTimer( m_uAppointmentTimerId );
      m_uAppointmentTimerId = 0;
   }

   if ( m_pAgenda && mIs_hWnd( m_pAgenda->m_hWnd ) )
      m_pAgenda->DestroyWindow( );

   if ( m_pMiniCal && mIs_hWnd( m_pMiniCal->m_hWnd ) )
      m_pMiniCal->DestroyWindow( );

   mDeleteInit( m_pMiniCal );
   mDeleteInit( m_pAgenda );
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

zSHORT
ZDayTimer::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDayTimer::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pMiniCal && m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW vApp;
      zUSHORT uMapState = GetMappingAutoGray( &vApp, *m_pzsVName,
                                              *m_pzsEName, *m_pzsAName );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) )
      {
         COleDateTime date;
         COleDateTime timeFrom;
         COleDateTime timeTo;
         ZAppointment *pA;
         zPCHAR       pchSubject;
         zPCHAR       pchMessage;
         zULONG       ulKey;
         zLONG        lAlarm;
         zLONG        lAppointmentType;
         zBOOL        bToday;
         zBOOL        bAfterToday;
         COleDateTime dateToday = COleDateTime::GetCurrentTime( );
         int          nYear = dateToday.GetYear( );
         int          nMonth = dateToday.GetMonth( );
         int          nDay = dateToday.GetDay( );
         int          nHour = dateToday.GetHour( );
         int          nMinute = dateToday.GetMinute( );
         int          nYearA;
         int          nMonthA;
         int          nDayA;

         zSHORT nRC = SetCursorFirstEntity( vApp, *m_pzsEName, 0 );
         while ( nRC >= zCURSOR_SET )
         {
            GetOleDateTimeFromZeidonDateAttribute( date, vApp,
                                                   *m_pzsEName,
                                                   *m_pzsAName );
            m_pMiniCal->AddSelectedDay( date, FALSE );
            nYearA = date.GetYear( );
            nMonthA = date.GetMonth( );
            nDayA = date.GetDay( );
            bToday = FALSE;
            bAfterToday = FALSE;
            if ( (wFlag & zMAP_FIRST_TIME) == 0 )
            {
               bAfterToday = TRUE;
            }
            else
            if ( nYear > nYearA ||
                 (nYear == nYearA && nMonth > nMonthA) ||
                 (nYear == nYearA && nMonth == nMonthA && nDay >= nDayA) )
            {
               if ( nYear == nYearA && nMonth == nMonthA && nDay > nDayA )
               {
                  bAfterToday = TRUE;
               }
               else
               {
                  bToday = TRUE;
               }
            }

            if ( m_pAgenda && *m_csTimeEntity && *m_csSubjectAttribute &&
                 *m_csMessageAttribute &&
                 *m_csTimeFromAttribute && *m_csTimeToAttribute &&
                 *m_csFullDayAttribute )
            {
               nRC = SetCursorFirstEntity( vApp, m_csTimeEntity, 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  ulKey = GetEntityKey( vApp, m_csTimeEntity );
                  GetAddrForAttribute( &pchSubject, vApp,
                                       m_csTimeEntity, m_csSubjectAttribute );
                  GetAddrForAttribute( &pchMessage, vApp,
                                       m_csTimeEntity, m_csMessageAttribute );
                  GetIntegerFromAttribute( &lAlarm, vApp,
                                           m_csTimeEntity, m_csAlarmAttribute );
                  GetIntegerFromAttribute( &lAppointmentType, vApp,
                                           m_csTimeEntity,
                                           m_csAppointmentTypeAttribute );
                  GetOleDateTimeFromZeidonDateAttribute( timeFrom, vApp,
                                                         m_csTimeEntity,
                                                         m_csTimeFromAttribute );
                  GetOleDateTimeFromZeidonDateAttribute( timeTo, vApp,
                                                         m_csTimeEntity,
                                                         m_csTimeToAttribute );

                  if ( CompareAttributeToString( vApp, m_csTimeEntity,
                                                 m_csFullDayAttribute, "Y" ) == 0 )
                  {
                     pA = AddFulldayAppointment( date, pchSubject, pchMessage,
                                                 lAlarm, lAppointmentType, ulKey );
                  }
                  else
                  {
                     pA = AddAppointment( date, timeFrom, timeTo,
                                          pchSubject, pchMessage,
                                          lAlarm, lAppointmentType, ulKey );
                  }

                  nRC = SetCursorNextEntity( vApp, m_csTimeEntity, 0 );
               }
            }

            nRC = SetCursorNextEntity( vApp, *m_pzsEName, 0 );
         }

         if ( m_pAgenda && (wFlag & zMAP_FIRST_TIME) )
         {
            SignalAlarms( );
            m_uAppointmentTimerId = SetTimer( DT_APPOINTMENT_TIMER_ID,
                                              DT_APPOINTMENT_TIMER_INTERVAL, 0 );
         }
      }
      else
      if ( vApp && (uMapState & zMAPACT_ENTITY_UPDATEABLE) )
      {
         SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, TRUE );
      }

      return( 0 );
   }

   return( -1 );
}

// Add a date and appointment entry for each appointment.
zSHORT
ZDayTimer::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZCalendar::MapToOI ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   m_ulMapActFlags |= zHOLD_DATETIME_ALARM;

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if (  m_pMiniCal && m_pzsVName && m_pzsEName && m_pzsAName &&
        (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      ZMiniCalendarSelSpot *pSel;
      ZAppointment *pA;
      COleDateTime date;
      POSITION pos;
      zCHAR    szDate[ 20 ];
      zULONG   ulKey;

      // First, delete all the Date entities (and child appointments) that
      // do not have corresponding Appointment instances.
      zSHORT nRC = SetCursorFirstEntity( vApp, *m_pzsEName, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetOleDateTimeFromZeidonDateAttribute( date, vApp,
                                                *m_pzsEName,
                                                *m_pzsAName );

         if ( m_pAgenda && *m_csTimeEntity &&
              *m_csSubjectAttribute && *m_csMessageAttribute &&
              *m_csTimeFromAttribute && *m_csTimeToAttribute &&
              *m_csAlarmAttribute && *m_csFullDayAttribute )
         {
            nRC = SetCursorFirstEntity( vApp, m_csTimeEntity, 0 );
            while ( nRC >= zCURSOR_SET )
            {
               ulKey = GetEntityKey( vApp, m_csTimeEntity );
               pA = m_pAgenda->m_pAppointmentHead;
               while ( pA )
               {
                  if ( pA->m_uKey == ulKey )
                     break;

                  pA = pA->m_pNext;
               }

               if ( pA  )
                  nRC = SetCursorNextEntity( vApp, m_csTimeEntity, 0 );
               else
                  nRC = DeleteEntity( vApp, m_csTimeEntity, zREPOS_NEXT );
            }
         }

         // If there are no child appointment entities or if the date
         // entity is not selected, delete the date entity.
         if ( m_pMiniCal->IsDaySelected( date ) ||
              (m_pAgenda && *m_csTimeEntity &&
               CheckExistenceOfEntity( vApp, m_csTimeEntity ) == 0) )
         {
            nRC = SetCursorNextEntity( vApp, *m_pzsEName, 0 );
         }
         else
         {
            nRC = DeleteEntity( vApp, *m_pzsEName, zREPOS_NEXT );
         }
      }

      if ( m_pAgenda )
      {
         // Now go through and ensure there is a corresponding date and
         // appointment entity for each appointment.
         pA = m_pAgenda->m_pAppointmentHead;
         while ( pA )
         {
            UpdateAppointment( pA );
            pA = pA->m_pNext;
         }
      }

      // Finally go through and ensure that all selected dates have
      // a corresponding date entity (and select it).
      SetAllSelectStatesForEntity( vApp, *m_pzsEName, FALSE, 0 );
      for ( pos = m_pMiniCal->m_SelectedDays.GetHeadPosition( );
            pos;
            m_pMiniCal->m_SelectedDays.GetNext( pos ) )
      {
         pSel = m_pMiniCal->m_SelectedDays.GetAt( pos );
         SetZeidonDateFromOleDateTime( szDate, sizeof( szDate ), pSel->m_date );
         if ( SetCursorFirstEntityByString( vApp, *m_pzsEName, *m_pzsAName, szDate, 0 ) != zCURSOR_SET )
         {
            CreateEntity( vApp, *m_pzsEName, zPOS_AFTER );
            SetZeidonDateAttributeFromOleDateTime( pSel->m_date, vApp, *m_pzsEName, *m_pzsAName );
         }

         SetSelectStateOfEntity( vApp, *m_pzsEName, TRUE );
      }
   }

   m_ulMapActFlags &= ~zHOLD_DATETIME_ALARM;
   return( 0 );
}

zLONG
ZDayTimer::SetZCtrlState( zLONG  lStatusType,
                         zULONG ulValue )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "FirstName", *m_pzsTag ) == 0 )
      TraceLineS( "ZDayTimer::SetZCtrlState: ", *m_pzsTag );
#endif

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

// ZCalendar - dtor
ZDayTimer::~ZDayTimer( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZDayTimer::dtor ", *m_pzsTag );
#endif
// if ( mIs_hWnd( m_hWnd ) )
//    DestroyWindow( );
}

int
ZDayTimer::OnCreate( LPCREATESTRUCT lpcs )
{
   int nRC = CWnd::OnCreate( lpcs );
   if ( nRC == -1 )
      return( -1 );

   return( nRC );
}

void
ZDayTimer::OnSize(  UINT uType, int cx, int cy )
{
   CWnd::OnSize( uType, cx, cy );
   m_rectCtrl.SetRect( 0, 0, cx, cy );

   if ( m_pMiniCal && mIs_hWnd( m_pMiniCal->m_hWnd ) )
   {
      CSize size = m_pMiniCal->ComputeTotalSize( );
      int nCols = m_pMiniCal->GetCols( );

      if ( cx < size.cx || m_nCols > nCols )
      {
#ifndef zREMOTE_SERVER  // only available under windows
         CWaitRedraw wd;
#endif
         int nRows = m_pMiniCal->GetRows( );
         if ( cx > size.cx && m_nCols > nCols )
         {
            nCols = m_nCols;
            m_pMiniCal->SetRowsAndColumns( nRows, nCols );
            m_pMiniCal->AutoConfigure( );
            m_pMiniCal->ReinitializeCells( );
            size = m_pMiniCal->ComputeTotalSize( );
         }

         if ( cx < size.cx )
         {
            while ( nCols > 1 )
            {
               nCols--;
               m_pMiniCal->SetRowsAndColumns( nRows, nCols );
               m_pMiniCal->AutoConfigure( );
               m_pMiniCal->ReinitializeCells( );
               size = m_pMiniCal->ComputeTotalSize( );
               if ( cx >= size.cx )
                  break;
            }
         }
      }
   }

   UpdatePositionOfControls( );
}

void
ZDayTimer::OnLButtonDown( UINT uFlags, CPoint pt )
{
   SetFocus( );
   CWnd::OnLButtonDown( uFlags, pt );
}

void
ZDayTimer::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pAgenda )
   {
      if ( pWndLostFocus == &(m_pAgenda->m_editAppointment) )
      {
         zCHAR szTag[ zTAG_LTH ];
         FindCtrlForFocus( m_pZSubtask->m_vDialog, szTag,
                           (::GetKeyState( VK_SHIFT) < 0), FALSE );
         if ( zstrcmp( szTag, (*m_pzsTag).GetString() ) )
         {
            SetFocusToCtrl( m_pZSubtask->m_vDialog, szTag );
            return;
         }
      }
      else
      {
         m_pAgenda->SetFocus( );
      }
   }

   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
      Default( );             // pass it along to parent
}

void
ZDayTimer::OnKillFocus( CWnd *pWndGetFocus )
{
   if ( pWndGetFocus == m_pMiniCal ||
        pWndGetFocus == m_pAgenda ||
        (m_pAgenda &&
         pWndGetFocus == &(m_pAgenda->m_editAppointment)) )
   {
      // That's fine ... don't tell the driver cuz it should
      // keep on believing the DayTimer has focus.
   }
   else
   {
      if ( m_pAgenda )
      {
         m_pAgenda->m_pAppointmentLastFocus = 0;
         m_pAgenda->m_bFocusRect = FALSE;
         if ( m_pAgenda->m_pDayMonthYear &&  // to get rid of focus rect
              mIs_hWnd( m_pAgenda->m_pDayMonthYear->m_hWnd ) )
         {
            m_pAgenda->m_pDayMonthYear->Invalidate( );
         }

      // if ( m_pAgenda->m_pFullDayWnd &&
      //      mIs_hWnd( m_pAgenda->m_pFullDayWnd->m_hWnd ) )
      // {
      //    m_pAgenda->m_pFullDayWnd->Invalidate( );
      // }
      }

      m_pZSubtask->InformDriverOfKillFocus( this );
   }

   Default( );                // pass it along to parent
}

int
ZDayTimer::GetHourHeight( int *pnVal )
{
   ASSERT_VALID( m_pAgenda );

   *pnVal = m_pAgenda->m_nHourHeight;
   return( 0 );
}

int
ZDayTimer::SetHourHeight( int nNewVal )
{
   ASSERT_VALID( m_pAgenda );

   if ( nNewVal < 8 )
   {
      MessageBox( "Invalid hour height. It should be at least 8 pixels" );
      return( -1 );
   }

   if ( nNewVal > 255 )
   {
      MessageBox( "Invalid hour height. It should be at most 255 pixels" );
      return( -1 );
   }

   m_pAgenda->m_nHourHeight = nNewVal;

#if 0  // UpdatePositionOfControls calls UpdateScrollSize
   if ( mIs_hWnd( *m_pAgenda ) )
   {
      m_pAgenda->UpdateScrollSize( );
      m_pAgenda->Invalidate( );
   }
#endif

   UpdatePositionOfControls( );
   Invalidate( );

   return( 0 );
}

BOOL
ZDayTimer::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN &&
        (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      if ( pMsg->wParam == VK_LEFT ||
           pMsg->wParam == VK_RIGHT ||
           pMsg->wParam == VK_UP ||
           pMsg->wParam == VK_DOWN )
      {
      // TraceLineI( "ZDayTimer::PreTranslateMessage: ", pMsg->wParam );
         switch ( pMsg->wParam )
         {
            case VK_LEFT:
            {
               COleDateTime date;
               if ( m_pMiniCal &&
                    m_pMiniCal->GetSelectedDateByOrder( date ) )
               {
                  if ( date.GetDay( ) == 1 &&
                       m_pMiniCal->GetCurrentMonth( ) == date.GetMonth( ) &&
                       m_pMiniCal->GetCurrentYear( ) == date.GetYear( ) )
                  {
                     m_pMiniCal->ScrollLeft( );
                  }

                  date -= 1;
                  m_pMiniCal->ClearAllSelectedDays( );
                  m_pMiniCal->AddSelectedDay( date );
                  m_pMiniCal->Invalidate( );
               }

               break;
            }

            case VK_RIGHT:
            {
               COleDateTime date;
               if ( m_pMiniCal &&
                    m_pMiniCal->GetSelectedDateByOrder( date ) )
               {
                  date += 1;
                  if ( date.GetDay( ) == 1 )
                  {
                     int nMonth = m_pMiniCal->GetCurrentMonth( );
                     int nYear = m_pMiniCal->GetCurrentYear( );
                     nMonth += m_pMiniCal->m_nRows * m_pMiniCal->m_nCols;
                     nYear += (nMonth - 1) / 12;
                     nMonth %= 12;
                     if ( nMonth == 0 )
                        nMonth = 12;

                     if ( date.GetYear( ) > nYear ||
                          (date.GetYear( ) == nYear && date.GetMonth( ) >= nMonth) )
                     {
                        m_pMiniCal->ScrollRight( );
                     }
                  }

                  m_pMiniCal->ClearAllSelectedDays( );
                  m_pMiniCal->AddSelectedDay( date );
                  m_pMiniCal->Invalidate( );
               }

               break;
            }

            case VK_UP:
               if ( m_pAgenda &&
                    m_pAgenda->m_timeFirstSelected.GetStatus( ) == COleDateTime::valid &&
                    m_pAgenda->m_timeLastSelected.GetStatus( ) == COleDateTime::valid &&
                    m_pAgenda->m_timeLastSelected > m_pAgenda->m_timeFirstSelected )
               {
                  COleDateTimeSpan ts( 0, 0, 30, 0 );
                  BOOL bChange = FALSE;
                  if ( m_pAgenda->m_timeFirstSelected.GetHour( ) > 0 ||
                       m_pAgenda->m_timeFirstSelected.GetMinute( ) >= 30 )
                  {
                     m_pAgenda->m_timeFirstSelected -= ts;
                     bChange = TRUE;
                  }

                  if ( ::GetKeyState( VK_SHIFT ) >= 0 )  // Shift key not pressed?
                  {
                     COleDateTime time = m_pAgenda->m_timeLastSelected;
                     m_pAgenda->m_timeLastSelected = m_pAgenda->m_timeFirstSelected;
                     m_pAgenda->m_timeLastSelected += ts;
                     if ( time != m_pAgenda->m_timeLastSelected )
                        bChange = TRUE;
                  }

                  if ( bChange )
                     m_pAgenda->GoToTime( m_pAgenda->m_timeFirstSelected, FALSE );
               }

               break;

            case VK_DOWN:
               if ( m_pAgenda &&
                    m_pAgenda->m_timeFirstSelected.GetStatus( ) == COleDateTime::valid &&
                    m_pAgenda->m_timeLastSelected.GetStatus( ) == COleDateTime::valid &&
                    m_pAgenda->m_timeLastSelected > m_pAgenda->m_timeFirstSelected )
               {
                  COleDateTimeSpan ts( 0, 0, 30, 0 );
                  BOOL bChange = FALSE;
                  if ( (m_pAgenda->m_timeFirstSelected.GetHour( ) < 24 &&
                        m_pAgenda->m_timeFirstSelected.GetMinute( ) < 30) ||
                       m_pAgenda->m_timeFirstSelected.GetHour( ) < 23 )
                  {
                     m_pAgenda->m_timeLastSelected += ts;
                     bChange = TRUE;
                  }

                  if ( ::GetKeyState( VK_SHIFT ) >= 0 )  // Shift key not pressed?
                  {
                     COleDateTime time = m_pAgenda->m_timeFirstSelected;
                     m_pAgenda->m_timeFirstSelected = m_pAgenda->m_timeLastSelected;
                     m_pAgenda->m_timeFirstSelected -= ts;
                     if ( time != m_pAgenda->m_timeFirstSelected )
                        bChange = TRUE;
                  }

                  if ( bChange )
                     m_pAgenda->GoToTime( m_pAgenda->m_timeLastSelected, FALSE );
               }

               break;
         }

         return( TRUE ); // do NOT process further
      }
      else
      if ( pMsg->wParam == VK_TAB )
      {
         if ( m_pAgenda )
         {
            ZAppointment *pA =
               m_pAgenda->GetNextShownAppointment( ::GetKeyState( VK_SHIFT ) < 0 );
            if ( pA )
            {
               m_pAgenda->EditAppointment( pA );
               TraceLineS( "ZDayTimer::PreTranslateMessage: VK_TAB ", "... TRUE" );
               return( TRUE ); // do NOT process further
            }

            TraceLineS( "ZDayTimer::PreTranslateMessage: VK_TAB ", "... FALSE" );
            SetFocus( );
         }
      }
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

ZAppointment *
ZDayTimer::AddAppointment( COleDateTime& date, COleDateTime& timeFrom,
                           COleDateTime& timeTo, zCPCHAR cpcSubject,
                           zCPCHAR cpcMessage, zLONG lAlarm,
                           zLONG lAppointmentType, zULONG ulKey )
{
   ASSERT_VALID( m_pAgenda );

   if ( timeFrom > 1 || timeTo > 1 )
   {
      MessageBox( "Invalid time specified. "
                  "Only hour/minute/second intervals are valid. "
                  "Like \"12.34.23\". You cannot specify a date." );
      return( 0 );
   }

   if ( (cpcSubject == 0 || cpcSubject[ 0 ] == 0) && lAlarm == 0 &&
        (cpcMessage == 0 || cpcMessage[ 0 ] == 0) )
   {
      MessageBox( "A subject has not been specified for the appointment" );
      return( 0 );
   }

   if ( timeFrom >= timeTo )
   {
      MessageBox( "The appointment start time is larger than the end time" );
      return( 0 );
   }

   ZAppointment *pA = new ZAppointment( m_pAgenda->m_nSeqNbr++, ulKey );

   pA->m_date = date;
   pA->m_bFullDay = FALSE;
   pA->m_csSubject = cpcSubject ? cpcSubject : "";
   pA->m_csMessage = cpcMessage ? cpcMessage : "";
   pA->m_timeFrom = timeFrom;
   pA->m_timeTo = timeTo;
   pA->m_nAlarm = lAlarm;
   pA->m_nAppointmentType = lAppointmentType;

   m_pAgenda->AddAppointment( pA );
   return( pA );
}

ZAppointment *
ZDayTimer::AddFulldayAppointment( COleDateTime& date, zCPCHAR cpcSubject,
                                  zCPCHAR cpcMessage, zLONG lAlarm,
                                  zLONG lAppointmentType, zULONG ulKey )
{
   ASSERT_VALID( m_pAgenda );
   ZAppointment *pA = new ZAppointment( m_pAgenda->m_nSeqNbr++, ulKey );

   pA->m_date = date;
   pA->m_bFullDay = TRUE;
   pA->m_csSubject = cpcSubject;
   pA->m_csMessage = cpcMessage;
   pA->m_timeFrom.SetDate( 100, 0, 0 );
   pA->m_timeTo.SetDate( 100, 0, 0 );
   pA->m_nAlarm = lAlarm;
   pA->m_nAppointmentType = lAppointmentType;

   m_pAgenda->AddAppointment( pA );
   return( pA );
}

int
ZDayTimer::RemoveAllAppointments( )
{
   ASSERT_VALID( m_pAgenda );

   m_pAgenda->RemoveAllAppointments( );
   return( 0 );
}

int
ZDayTimer::SignalAlarms( )
{
   if ( m_ulMapActFlags & zHOLD_DATETIME_ALARM )
      return( 0 );

   m_ulMapActFlags |= zHOLD_DATETIME_ALARM;
   int nAlarms = 0;

   if ( m_pAgenda && m_pAgenda->m_pAppointmentHead )
   {
      COleDateTime dateToday = COleDateTime::GetCurrentTime( );
      int          nYear = dateToday.GetYear( );
      int          nMonth = dateToday.GetMonth( );
      int          nDay = dateToday.GetDay( );
      int          nHour = dateToday.GetHour( );
      int          nMinute = dateToday.GetMinute( );
      int          nYearA;
      int          nMonthA;
      int          nDayA;
      BOOL         bToday;
      BOOL         bAfterToday;

      ZAppointment *pA = m_pAgenda->m_pAppointmentHead;
      while ( pA )
      {
         if ( pA->m_nAlarm > 0 )
         {
            bToday = FALSE;
            bAfterToday = FALSE;
            nYearA = pA->m_date.GetYear( );
            nMonthA = pA->m_date.GetMonth( );
            nDayA = pA->m_date.GetDay( );
            if ( nYearA > nYear ||
                 (nYearA == nYear && nMonthA > nMonth) ||
                 (nYearA == nYear && nMonthA == nMonth && nDayA >= nDay) )
            {
               if ( nYearA == nYear && nMonthA == nMonth && nDayA == nDay )
               {
                  bToday = TRUE;
               }
               else
               {
                  bAfterToday = TRUE;
               }
            }

            if ( bAfterToday == FALSE )
            {
               int nHourA = pA->m_timeFrom.GetHour( );
               if ( bToday == FALSE || nHourA < nHour ||
                    (nHourA == nHour && pA->m_timeFrom.GetMinute( ) <= nMinute) )
               {
                  nAlarms++;
                  UpdateAppointment( pA );  // set Zeidon data and position
                  ProcessImmediateEvent( this,
                                         zDAYTIMER_APPOINTMENT_ALARM,
                                         pA );
               }
            }
         }

         pA = pA->m_pNext;
      }
   }

   m_ulMapActFlags &= ~zHOLD_DATETIME_ALARM;
   return( nAlarms );
}

void
ZDayTimer::OnTimer( UINT uIDEvent )
{
   if ( uIDEvent == m_uAppointmentTimerId && mIs_hWnd( m_hWnd ) )
      SignalAlarms( );
}

ZAppointment *
ZDayTimer::NewAppointment( CPoint& pt,
                           LPCSTR  cpcSubject,
                           LPCSTR  cpcMessage,
                           zLONG   lAppointmentType,
                           BOOL    bFullDay )
{
   COleDateTime date;

   if ( m_pAgenda &&
        m_pAgenda->GetDateFromPoint( date, pt ) )
   {
      if ( bFullDay )
      {
         return( AddFulldayAppointment( date, cpcSubject, cpcMessage,
                                        lAppointmentType, FALSE, 0 ) );
      }

      if ( pt.x > m_pAgenda->m_nTimeGutterWidth )  // not in gutter
      {
         SCROLLINFO si;
         si.cbSize = sizeof( SCROLLINFO );
         si.fMask = SIF_POS;

         m_pAgenda->GetScrollInfo( SB_VERT, &si );
         int nPos = (pt.y + si.nPos * m_pAgenda->m_nHourHeight) /
                                              (m_pAgenda->m_nHourHeight / 2);

         COleDateTime timeFrom;
         COleDateTime timeTo;

         // Zeros out date by setting it to ==> 1899.12.30
         timeFrom.SetTime( nPos / 2, nPos % 2 ? 30 : 0, 0 );
         timeTo.SetTime( nPos / 2 + 1, nPos % 2 ? 30 : 0, 0 );

         ZAppointment *pA = new ZAppointment( m_pAgenda->m_nSeqNbr++, 0 );
         pA->m_date = date;
         pA->m_bFullDay = FALSE;

#if 0
         int nHour = timeFrom.GetHour( );
         zPCHAR pch;

         if ( nHour < 12 )
            pch = "am";
         else
         {
            pch = "pm";
            nHour -= 12;
         }

         if ( nHour == 0 )
            nHour = 12;
#endif

         pA->m_csSubject = cpcSubject;
         pA->m_csMessage = cpcMessage;
         pA->m_timeFrom = timeFrom;
         pA->m_timeTo = timeTo;
         pA->m_nAppointmentType = lAppointmentType;
         m_pAgenda->AddAppointment( pA );

      // GoToTime( timeFrom ); // Scroll the appointment into view
         SetFocus( );
         return( pA );
      }
   }

   return( 0 );
}

int
ZDayTimer::UpdateAppointment( ZAppointment *pA )
{
   zVIEW vApp;

   if ( m_pAgenda && m_pzsVName &&
        (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 &&
        m_pzsEName && m_pzsAName &&
        *m_csTimeEntity &&
        *m_csSubjectAttribute && *m_csMessageAttribute &&
        *m_csTimeFromAttribute && *m_csTimeToAttribute &&
        *m_csAlarmAttribute && *m_csFullDayAttribute )
   {
      zCHAR  szDate[ 20 ];

      SetZeidonDateFromOleDateTime( szDate, sizeof( szDate ), pA->m_date );
      if ( SetCursorFirstEntityByString( vApp, *m_pzsEName, *m_pzsAName, szDate, 0 ) != zCURSOR_SET )
      {
         CreateEntity( vApp, *m_pzsEName, zPOS_AFTER );
         SetAttributeFromString( vApp, *m_pzsEName, *m_pzsAName, szDate );
      }

      if ( pA->m_uKey == 0 ||
           SetEntityCursor( vApp, m_csTimeEntity, 0,
                            zQUAL_ENTITYKEY | zPOS_FIRST,
                            (zCPVOID) &(pA->m_uKey),
                            0, 0, 0, *m_pzsEName, 0 ) < zCURSOR_SET )
      {
         CreateEntity( vApp, m_csTimeEntity, zPOS_AFTER );
         pA->m_uKey = GetEntityKey( vApp, m_csTimeEntity );
      }

      SetAttributeFromString( vApp, m_csTimeEntity,
                              m_csSubjectAttribute, pA->m_csSubject );
      SetAttributeFromString( vApp, m_csTimeEntity,
                              m_csMessageAttribute, pA->m_csMessage );
      SetAttributeFromInteger( vApp, m_csTimeEntity,
                               m_csAlarmAttribute, pA->m_nAlarm );
      if ( pA->m_bFullDay )
      {
         SetAttributeFromString( vApp, m_csTimeEntity,
                                 m_csFullDayAttribute, "Y" );
      }
      else
      {
         SetAttributeFromString( vApp, m_csTimeEntity,
                                 m_csFullDayAttribute, "" );
         SetZeidonDateAttributeFromOleDateTime( pA->m_timeFrom, vApp,
                                                m_csTimeEntity,
                                                m_csTimeFromAttribute );
         SetZeidonDateAttributeFromOleDateTime( pA->m_timeTo, vApp,
                                                m_csTimeEntity,
                                                m_csTimeToAttribute );
      }

      return( 0 );
   }

   return( -1 );
}

int
ZDayTimer::DeleteAppointment( ZAppointment *pA )
{
   if ( m_pAgenda )
      return( m_pAgenda->DeleteAppointment( pA ) );

   return( -1 );
}

int
ZDayTimer::SetColorAssociation( COLORREF clrText,
                                COLORREF clrBack,
                                int      lFlag,
                                LPCSTR   cpcAttributeValue )
{
   return( 0 );
}

int
ZDayTimer::SetTimeEntityAttributes( LPCSTR cpcTimeEntity,
                                    LPCSTR cpcSubjectAttribute,
                                    LPCSTR cpcMessageAttribute,
                                    LPCSTR cpcTimeFromAttribute,
                                    LPCSTR cpcTimeToAttribute,
                                    LPCSTR cpcAlarmAttribute,
                                    LPCSTR cpcAppointmentTypeAttribute,
                                    LPCSTR cpcFullDayAttribute )
{
   m_csTimeEntity = cpcTimeEntity;
   m_csSubjectAttribute = cpcSubjectAttribute;
   m_csMessageAttribute = cpcMessageAttribute;
   m_csTimeFromAttribute = cpcTimeFromAttribute;
   m_csTimeToAttribute = cpcTimeToAttribute;
   m_csAlarmAttribute = cpcAlarmAttribute;
   m_csAppointmentTypeAttribute = cpcAppointmentTypeAttribute;
   m_csFullDayAttribute = cpcFullDayAttribute;
   return( 0 );
}

int
ZDayTimer::GoToTime( COleDateTime& Time, BOOL bMoveToTop )
{
   ASSERT_VALID( m_pAgenda );

   if ( Time > 1 )
   {
      MessageBox( "Invalid time specified. Only hour/minute/second intervals "
                    "are valid. Like \"12.34.23\". "
                    "You cannot specify a date" );
      return( 1 );
   }

   m_pAgenda->GoToTime( Time, bMoveToTop );
   return( 0 );
}

void
ZDayTimer::UpdatePositionOfControls( )
{
   CSize size = m_pMiniCal->ComputeTotalSize( );
   int   nOffsetX = m_rectCtrl.right - size.cx;

   if ( nOffsetX < 0 )
      nOffsetX = 0;

   // Set the position and size of the MiniCal and Agenda windows.
   if ( m_pMiniCal && mIs_hWnd( m_pMiniCal->m_hWnd ) )
   {
      m_pMiniCal->SetWindowPos( 0, nOffsetX, 0,
                                size.cx, size.cy,
                                SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE );
   }

   if ( m_pAgenda && mIs_hWnd( m_pAgenda->m_hWnd ) )
   {
      m_pAgenda->m_nClientX = m_rectCtrl.right;
      m_pAgenda->m_nClientY = m_rectCtrl.bottom - size.cy;
      m_pAgenda->UpdatePositionOfControls( );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZAgenda - Adapted from ActiveX control
// by: Christian S. Andersen
// DayView - An Outlook Like Appointment View
//

IMPLEMENT_DYNAMIC( ZAgenda, CWnd )
BEGIN_MESSAGE_MAP( ZAgenda, CWnd)
   //{{AFX_MSG_MAP( ZAgenda )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
   ON_WM_SIZE( )
   ON_WM_VSCROLL( )
   ON_WM_ERASEBKGND( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_MOUSEMOVE( )
   ON_WM_KEYDOWN( )
   ON_WM_CONTEXTMENU( )
   ON_WM_SETCURSOR( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZAgenda::ZAgenda( ZDayTimer *pDayTimer ) :
         m_editAppointment( this )
{
   m_pDayTimer = pDayTimer;
   m_pDayMonthYear = 0;
   m_pFullDayWnd = 0;
   m_nEditBorder = 0;
   m_nHourHeight = g_nFullDayAppointmentHeight * 2;
   m_nTimeGutterWidth = INTFRAC( m_nHourHeight, 1.5 );
   m_nHeightOfDayMonthYear = 0;
   m_pHourFont = 0;
   m_pSmallHourFont = 0;
   m_timeWorkdayStart.SetTime( 8, 0, 0 );
   m_timeWorkdayEnd.SetTime( 17, 0, 0 );
   m_lWorkdayFlag = 2 + 4 + 8 + 16 + 32;
   m_pAppointmentHead = 0;
   m_pAppointmentLastFocus = 0;
   m_bFocusRect = FALSE;
   m_nClientX = 0;
   m_nClientY = 0;

   COleDateTime date = COleDateTime::GetCurrentTime( );
   m_dateLastSelected.SetDate( date.GetYear( ), date.GetMonth( ),
                               date.GetDay( ) );
   m_timeFirstSelected.SetTime( 8, 0, 0 );
   m_timeLastSelected.SetTime( 8, 30, 0 );
   m_bTrackingSelected = FALSE;
   m_pTrackedAppointmentBand = 0;
   m_pTrackedAppointment = 0;
   m_bEditAppointment = FALSE;
}

ZAgenda::~ZAgenda( )
{
   if ( m_pDayTimer )
      m_pDayTimer->m_pAgenda = 0;

   mDeleteInit( m_pSmallHourFont );
   mDeleteInit( m_pHourFont );
   RemoveAllAppointments( );
}

BOOL
ZAgenda::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZAgenda::DestroyWindow ", "" );
#endif
   if ( mIs_hWnd( m_wndToolTip.m_hWnd ) )
      m_wndToolTip.DestroyWindow( );

   if ( m_pDayMonthYear && mIs_hWnd( m_pDayMonthYear->m_hWnd ) )
      m_pDayMonthYear->DestroyWindow( );

   if ( m_pFullDayWnd && mIs_hWnd( m_pFullDayWnd->m_hWnd ) )
      m_pFullDayWnd->DestroyWindow( );

   mDeleteInit( m_pDayMonthYear );
   mDeleteInit( m_pFullDayWnd );
   return( CWnd::DestroyWindow( ) );
}

BOOL
ZAgenda::PreCreateWindow( CREATESTRUCT& cs )
{
   cs.style |= WS_VSCROLL;
   if ( CWnd::PreCreateWindow( cs ) == FALSE )
      return( FALSE );

   return( TRUE );
}

int
ZAgenda::OnCreate( LPCREATESTRUCT lpcs )
{
   int nRC = CWnd::OnCreate( lpcs );
   if ( nRC == -1 )
      return( -1 );

   // Creation of objects should be done here rather than in CreateZ
   // since CreateZ is only called from the normal constructor.

   // Load images list:
   BOOL bRC = m_Images.Create( IDB_APPOINTMENT_ALARM,
                               16, 0, g_clrTransparent );
   if ( bRC == FALSE )
   {
      TraceLineS( "ZAgenda: Can't load images list!", "" );
      return( -1 );
   }

   // Define image size:
   IMAGEINFO imageInfo;
   m_Images.GetImageInfo( 0, &imageInfo );

   CRect rectImage = imageInfo.rcImage;
   m_ImageSize = rectImage.Size( );

   // Create pens.
   VERIFY( m_penDivider.CreatePen( PS_SOLID, 1,
                                   ::GetSysColor( COLOR_3DSHADOW ) ) );
   VERIFY( m_penNull.CreatePen( PS_NULL, 0, (COLORREF) 0 ) );
   VERIFY( m_penThinBlack.CreatePen( PS_SOLID, 1, (COLORREF) 0 ) );
// VERIFY( m_penThickBlack.CreatePen( PS_SOLID, 2, 0 ) );
   VERIFY( m_penShadow.CreatePen( PS_SOLID, 2, (COLORREF) 255 ) );

   // Create brushes.
   CBitmap bmpPattern;
   VERIFY( bmpPattern.LoadBitmap( IDS_CALENDAR_BKGND ) );
   CBitmap bmpPatternWorkday;
   VERIFY( bmpPatternWorkday.LoadBitmap( IDS_CALENDAR_WORKDAY ) );

   VERIFY( m_brushBack.CreatePatternBrush( &bmpPattern ) ); // background color
   VERIFY( m_brushWorkdayBack.CreatePatternBrush( &bmpPatternWorkday ) );
   VERIFY( m_brushAppointmentBack.CreateSolidBrush( ::GetSysColor( COLOR_WINDOW ) ) );

   // Create font.
   VERIFY( m_fontAppointmentSubject.CreateStockObject( DEFAULT_GUI_FONT ) );

   m_wndToolTip.Create( this, "", // *m_pzsText, this would be the constant text
                        _T( "MS Sans Serif" ), -12 );  // create tip window here

   m_pDayMonthYear = new ZDayMonthYear;
   m_pDayMonthYear->m_pAgenda = this;
   VERIFY( m_pDayMonthYear->Create( 0, "DayMonthYear",
                                    WS_CHILD | WS_VISIBLE | WS_BORDER,
                                    CRect( 0, 0, 0, 0 ), m_pDayTimer, 1 ) );

   // Create FullDayWnd.
   m_pFullDayWnd = new ZFullDayWnd;
   m_pFullDayWnd->m_pAgenda = this;
   VERIFY( m_pFullDayWnd->Create( 0, "FullDay",
                                  WS_CHILD | WS_VISIBLE,
                                  CRect( 0, 0, 0, 0 ), m_pDayTimer, 2 ) );

   // Create in place editor window.
   m_editAppointment.CreateEx( 0, "EDIT", "EditAppointment",
                               WS_CHILD | ES_LEFT |
                                 ES_MULTILINE | ES_AUTOVSCROLL | WS_THICKFRAME,
                               // ES_AUTOHSCROLL | WS_BORDER,
                               0, 0, 0, 0,
                               m_hWnd, (HMENU) 4 );

   // Adjust the vertical scrolling range and scroll box
   // position to reflect the new yMax value.
   UpdateScrollSize( TRUE );
   return( nRC );
}

void
ZAgenda::OnSize( UINT uType, int cx, int cy )
{
   CWnd::OnSize( uType, cx, cy );
//?UpdatePositionOfControls( );
}

void
ZAgenda::OnSetFocus( CWnd *pWndLostFocus )
{
// TraceLineS( "ZAgenda::OnSetFocus", "" );
   m_bFocusRect = TRUE;
   if ( m_pDayMonthYear &&
        mIs_hWnd( m_pDayMonthYear->m_hWnd ) )
   {
      m_pDayMonthYear->Invalidate( );  // to show focus rect
   }

// if ( m_pFullDayWnd &&
//      mIs_hWnd( m_pFullDayWnd->m_hWnd ) )
// {
//    m_pFullDayWnd->Invalidate( );
// }

   CWnd::OnSetFocus( pWndLostFocus );
}

void
ZAgenda::OnKillFocus( CWnd *pWndGetFocus )
{
// TraceLineS( "ZAgenda::OnKillFocus", "" );
   m_bFocusRect = FALSE;
   if ( m_pDayMonthYear )
      m_pDayMonthYear->Invalidate( );  // to get rid of focus rect

   if ( m_pDayTimer )
      m_pDayTimer->m_pZSubtask->InformDriverOfKillFocus( m_pDayTimer );

   CWnd::OnKillFocus( pWndGetFocus );
}

void
ZAgenda::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// ASSERT( m_pParent );

   if ( uChar == VK_LEFT ||
        uChar == VK_RIGHT ||
        uChar == VK_UP ||
        uChar == VK_DOWN )
   {
      TraceLineI( "ZAgenda::OnKeyDown: ", uChar );
   }
}

void
ZAgenda::UpdateScrollSize( BOOL bResetPos /* = FALSE */ )
{
   int nFullDayCnt = GetNumberOfFulldayAppointments( );
   int nHeightOfFullDay = nFullDayCnt * g_nFullDayAppointmentHeight +
                                               m_nHeightOfDayMonthYear;

   SCROLLINFO si;
   si.cbSize = sizeof( SCROLLINFO );
   si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;

   GetScrollInfo( SB_VERT, &si );

   si.nPage = max( 1, ((m_nClientY - nHeightOfFullDay) / m_nHourHeight) - 1 );
   if ( bResetPos )
      si.nPos = 7;  // initial top of agenda hour is 7AM

   // Set min and max vertical scrolling position
   si.nMin = 0;
   si.nMax = 22;  //max( 0, 24 - si.nPage );

   SetScrollInfo( SB_VERT, &si, TRUE );

   // Force the font to be reset.
   mDeleteInit( m_pHourFont );
   mDeleteInit( m_pSmallHourFont );

   Invalidate( );
}

void
ZAgenda::OnVScroll( UINT uSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
   int        nIncrement;
   SCROLLINFO si;

   si.cbSize = sizeof( SCROLLINFO );
   si.fMask  = SIF_POS | SIF_RANGE | SIF_PAGE;
   GetScrollInfo( SB_VERT, &si );

   switch ( uSBCode )
   {
      case SB_PAGEUP:      // user clicked on shaft above the scroll box
         nIncrement = m_nClientY / m_nHourHeight;
         nIncrement = min( -1, -nIncrement + 1 );
         break;

      case SB_PAGEDOWN:    // user clicked on shaft below the scroll box
         nIncrement = m_nClientY / m_nHourHeight;
         nIncrement = max( 1, nIncrement - 1 );
         break;

      case SB_LINEUP:      // user clicked the top arrow
         nIncrement = -1;
         break;

      case SB_LINEDOWN:    // user clicked the bottom arrow
         nIncrement = 1;
         break;

      case SB_THUMBTRACK:  // user dragged the scroll box
         nIncrement = uThumbPos - si.nPos;
         break;

      default:
         nIncrement = 0;
   }

   // If applying the vertical scrolling increment does not take the
   // scrolling position out of the scrolling range, increment the
   // scrolling position, adjust the position of the scroll box, and
   // update the window. UpdateWindow sends the WM_PAINT message.
   if ( nIncrement = max( -si.nPos,
                          min( nIncrement, (int) (24 - si.nPos - si.nPage) ) ) )
   {
      si.nPos += nIncrement;
      SetScrollInfo( SB_VERT, &si, TRUE );

      // A negative value will scroll the content of the window up.
      nIncrement *= -m_nHourHeight;
      ScrollWindow( 0, nIncrement );

      // I don't think I should have to do this, but ...
      // Take this out when things are done right???
#if 0
      CRect rectInvalid;
      GetClientRect( &rectInvalid );
      if ( nIncrement < 0 )
         rectInvalid.top = rectInvalid.bottom + nIncrement;
      else
         rectInvalid.bottom = rectInvalid.top + nIncrement;

      InvalidateRect( rectInvalid );
#else
      Invalidate( );
#endif

      UpdateWindow( );
   }
}

BOOL
ZAgenda::OnEraseBkgnd( CDC *pDC )
{
   return( FALSE );
}

void
ZAgenda::SetDrawRect( CRect& rectDraw, COleDateTime& timeFrom,
                      COleDateTime& timeTo,
                      const CRect& rectArea, int nScrollPos )
{
   int nFullHeight = m_nHourHeight * 24;
   int nStart = INTFRAC( nFullHeight, timeFrom );
   int nEnd = INTFRAC( nFullHeight, timeTo );

   rectDraw = rectArea;
   rectDraw.top = nStart - nScrollPos * m_nHourHeight;
   rectDraw.bottom = nEnd - nScrollPos * m_nHourHeight;
}

int
ZAgenda::GetDayCount( )
{
   if ( m_pDayTimer && m_pDayTimer->m_pMiniCal &&
        m_pDayTimer->m_pMiniCal->m_nDayCnt )
   {
      return( m_pDayTimer->m_pMiniCal->m_nDayCnt );
   }
   else
   {
      return( 1 );
   }
}

void
ZAgenda::OnPaint( )  // ZAgenda drawing
{
// TraceAppointmentList( "Before Agenda::OnPaint" );
   CRect rect;
   GetUpdateRect( rect, FALSE );
// TraceRect( "ZAgenda::OnPaint UpdateRect: ", rect );

   CPaintDC dcPaint( this ); // device context for painting

   CRect rectClient;
   GetClientRect( &rectClient );

   // If possible, we paint to an off-screen DC and BitBlt the results to
   // the screen (reduces flicker).
   CDC      *pDC;
   zBOOL    bMemDC;
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
   else
   {
      bMemDC = FALSE;
      pDC = &dcPaint;
   }

//?pDC->SetBkMode( TRANSPARENT );
// ZMemDC memDC( &dcPaint );

   memDC.FillSolidRect( &rectClient, ::GetSysColor( COLOR_BTNFACE ) );
// memDC.FillSolidRect( &rectClient, ::GetSysColor( COLOR_ACTIVECAPTION ) );

   static zCPCHAR cpcFaceName = "Tahoma";
   CRect rectTimeGutter( 0, 0, m_nTimeGutterWidth, rectClient.Height( ) );
   CFont *pFontOld = 0;
   CPen  *pPenOld = 0;
   int   nLargeFontHeight = INTFRAC( m_nHourHeight, 0.7 );
   int   nSmallFontHeight = INTFRAC( m_nHourHeight, 0.4 );

   // Use the current vertical scrolling position and coordinates of the
   // invalid rectangle to determine the range of new lines that should
   // be drawn in the client area.
   SCROLLINFO si;
   si.cbSize = sizeof( SCROLLINFO );
   si.fMask = SIF_ALL;
   GetScrollInfo( SB_VERT, &si );
// if ( si.nPos > 0 )
//    TraceLineI( "Paint pos: ", si.nPos );

   // Create fonts for drawing time divisions (in time gutter).
   if ( m_pHourFont == 0 )
      m_pHourFont = GetHeightFont( cpcFaceName, nLargeFontHeight, &memDC );

   if ( m_pSmallHourFont == 0 )
      m_pSmallHourFont = GetHeightFont( cpcFaceName, nSmallFontHeight, &memDC );

   ASSERT_VALID( m_pHourFont );
   ASSERT_VALID( m_pSmallHourFont );

   // Paint time gutter ... the whole client area was filled above.
// memDC.FillSolidRect( rectTimeGutter, ::GetSysColor( COLOR_BTNFACE ) );
   CRect rectApptArea( rectTimeGutter.right, rectTimeGutter.top,
                       rectClient.right + 1, rectTimeGutter.bottom + 1 );

   // Paint the background with a pattern brush.
   CBrush *pBrushOld = memDC.SelectObject( &m_brushBack );
   pPenOld = memDC.SelectObject( &m_penNull );
   // rectApptArea.DeflateRect( 1, 1, 1, 2 );
   VERIFY( memDC.Rectangle( &rectApptArea ) );

   // Draw the workday times for each selected day.
   rect = rectApptArea;
   int nDayCnt = GetDayCount( );
   int nSizeX = (m_nClientX - m_nTimeGutterWidth) / GetShownDayCount( );
   COleDateTime date;
   CRect rectWorkday;
   int k;

// TraceRect( "ZAgenda::OnPaint ApptArea: ", rectApptArea );
// TraceLineI( "ZAgenda =======> SubWidth: ", nSizeX );

   memDC.SelectObject( &m_brushWorkdayBack );
   rect.right = rect.left + nSizeX;
   for ( k = 0; k < nDayCnt; k++ )
   {
      if ( k == nDayCnt - 1 )
         rect.right = rectApptArea.right;

      if ( m_pDayTimer->m_nMaxShowAppointment == 0 ||
           k >= nDayCnt - m_pDayTimer->m_nMaxShowAppointment )
      {
         m_pDayTimer->m_pMiniCal->GetSelectedDateByOrder( date, k );
         int nDayOfWeek = date.GetDayOfWeek( );

         if ( (m_lWorkdayFlag & (0x00000001 << (nDayOfWeek - 1))) &&
              m_timeWorkdayStart < m_timeWorkdayEnd )
         {
            SetDrawRect( rectWorkday, m_timeWorkdayStart, m_timeWorkdayEnd,
                         rect, si.nPos );
            VERIFY( memDC.Rectangle( &rectWorkday ) );
         }

         rect.left += nSizeX;
         rect.right += nSizeX;
      }
   }

   // Draw selected hours.
   if ( m_timeFirstSelected.GetStatus( ) == COleDateTime::valid &&
        m_timeLastSelected.GetStatus( ) == COleDateTime::valid &&
        m_timeLastSelected > m_timeFirstSelected )
   {
      CRect rectSelected;
      SetDrawRect( rectSelected, m_timeFirstSelected, m_timeLastSelected,
                   rectApptArea, si.nPos );
      memDC.FillSolidRect( rectSelected,
                           ::GetSysColor( COLOR_ACTIVECAPTION ) );
   }

   // Restore DC.
   memDC.SelectObject( pBrushOld );
   memDC.SelectObject( pPenOld );

   // Draw hour and half-hour dividers.
   ASSERT_VALID( m_pHourFont );
   pFontOld = 0;
   memDC.SetBkMode( TRANSPARENT );

   zBOOL bFirstAM = FALSE;
   zBOOL bFirstPM = FALSE;
   int nTextTop;
   int nPos;
   int nIdx;
   int nHalfHour = m_nHourHeight / 2;
   int nHalfHourOffset =
       int( (double( m_nTimeGutterWidth ) * g_dHalfHourDivider) -
                                                       g_nTimeGutterBorder );
   pPenOld = memDC.SelectObject( &m_penDivider );
   for ( k = 0, nIdx = si.nPos * 2;
         k <= (int) (si.nPage + 1) * 2 + 1;
         k++, nIdx++ )
   {
      nPos = (k * m_nHourHeight) / 2;
      nTextTop = nPos + 2;

      if ( nIdx % 2 == 0 )
      {
         // Draw time.
         memDC.MoveTo( 0, nPos );
         memDC.LineTo( m_nTimeGutterWidth - g_nTimeGutterBorder, nPos );

         CString csTime;
         if ( nIdx < 48 )
         {
            if ( nIdx < 24 )
               csTime.Format( "%2d", nIdx / 2 );
            else
               csTime.Format( "%2d", (nIdx - 24) / 2 );

            if ( csTime == " 0" )
               csTime = "12";
         }
         else
            csTime = "";

         pFontOld = memDC.SelectObject( m_pHourFont );
         CRect rectTxt( 0, nTextTop, nHalfHourOffset - 3,
                        nPos + m_nHourHeight );
         memDC.DrawText( csTime, rectTxt,
                         DT_SINGLELINE | DT_RIGHT | DT_VCENTER );
         memDC.SelectObject( pFontOld );

         pFontOld = memDC.SelectObject( m_pSmallHourFont );
         rectTxt.left = rectTxt.right;
         rectTxt.right = m_nTimeGutterWidth - g_nTimeGutterBorder;
         csTime = "00";
         if ( nIdx < 24 )
         {
            if ( bFirstAM == FALSE )
            {
               csTime = "am";
               bFirstAM = TRUE;
            }
         }
         else
         if ( nIdx < 48 )
         {
            if ( bFirstPM == FALSE )
            {
               csTime = "pm";
               bFirstPM = TRUE;
            }
         }
         else
            csTime = "";

         memDC.DrawText( csTime, rectTxt,
                         DT_SINGLELINE | DT_RIGHT | DT_TOP );
         memDC.SelectObject( pFontOld );
      }
      else
      {
         // Draw the half-hour divider line (under the minutes).
         memDC.MoveTo( nHalfHourOffset, nPos );
         memDC.LineTo( m_nTimeGutterWidth - g_nTimeGutterBorder, nPos );
      }

      // Draw half-hour memo dividers.
      int nLeft = m_nTimeGutterWidth + g_nGutterBandWidth;
      memDC.MoveTo( nLeft, nPos );
      memDC.LineTo( rectApptArea.right, nPos );
   }

   // Restore DC.
   memDC.SelectObject( pPenOld );
// TraceAppointmentList( "Before Draw Appointments" );

   // Paint schedule for each selected day.
   rect = rectApptArea;
   rect.right = rect.left + nSizeX;
   for ( k = 0; k < nDayCnt; k++ )
   {
      if ( k == nDayCnt - 1 )
         rect.right = rectApptArea.right;

      if ( m_pDayTimer->m_nMaxShowAppointment == 0 ||
           k >= nDayCnt - m_pDayTimer->m_nMaxShowAppointment )
      {
         m_pDayTimer->m_pMiniCal->GetSelectedDateByOrder( date, k );

         // Paint the dividing margin (between gutter and appointment area).
         CRect rectGutterBand = rect;
         rectGutterBand.right = rectGutterBand.left + g_nGutterBandWidth;
         memDC.FillSolidRect( &rectGutterBand, RGB( 255, 255, 255 ) );

         // Paint margin border.
         pPenOld = memDC.SelectObject( &m_penThinBlack );
         memDC.MoveTo( rectGutterBand.right, 0 );
         memDC.LineTo( rectGutterBand.right, rectGutterBand.bottom );
         memDC.SelectObject( pPenOld );

         // Draw 3D edge on margin.
      // CRect rectEdge = rectApptArea;
      // rectEdge.top = si.nPos;
      // memDC.DrawEdge( &rectEdge, EDGE_ETCHED, BF_RECT );

         // Draw the appointments.
         rect.right -= 4;  // leave room to permit select on agenda area

         if ( m_pAppointmentHead )
         {
            ZAppointment *pA = m_pAppointmentHead;
            while ( pA && pA->m_date != date )
               pA = pA->m_pNext;

            if ( pA && pA->m_date == date )
               pA->DrawAppointment( date, &memDC, rect,
                                    this, si.nPos );
         }

         rect.right += 4;

         rect.left += nSizeX;
         rect.right += nSizeX;
      }
   }

   if ( bMemDC )
   {
      // Copy the results to the on-screen DC.
      CRect rectClip;
      dcPaint.GetClipBox( rectClip );
      dcPaint.BitBlt( rectClip.left, rectClip.top,
                      rectClip.Width( ), rectClip.Height( ),
                      &memDC, rectClip.left, rectClip.top, SRCCOPY );

      memDC.SelectObject( pOldBmp );
   }

   TraceAppointmentList( "After Draw Appointments" );
}

void
ZAgenda::ShowSelectedDays( )
{
   if ( m_pDayMonthYear && mIs_hWnd( m_pDayMonthYear->m_hWnd ) )
      m_pDayMonthYear->RedrawWindow( );

   if ( m_pFullDayWnd && mIs_hWnd( m_pFullDayWnd->m_hWnd ) )
      m_pFullDayWnd->RedrawWindow( );

   if ( mIs_hWnd( m_hWnd ) )
      RedrawWindow( );
}

int
ZAgenda::GetShownDayCount( )
{
   int nDayCnt = GetDayCount( );

   if ( m_pDayTimer->m_nMaxShowAppointment > 0 &&
        nDayCnt > m_pDayTimer->m_nMaxShowAppointment )
   {
      nDayCnt = m_pDayTimer->m_nMaxShowAppointment;
   }

   return( nDayCnt );
}

void
ZAgenda::SetWorkday( COleDateTime& timeWorkdayStart,
                     COleDateTime& timeWorkdayEnd,
                     zLONG         lWorkdayFlag )
{
   if ( timeWorkdayStart > timeWorkdayEnd )
   {
      TraceLineS( "ZAgendaView::SetWorkday",
                  "Work day start time should occur before end time" );
      return;
   }

   m_timeWorkdayStart = timeWorkdayStart;
   m_timeWorkdayEnd = timeWorkdayEnd;
   m_lWorkdayFlag = lWorkdayFlag;
   Invalidate( );
}

int
ZAgenda::GetDateFromPoint( COleDateTime& date, CPoint& pt )
{
   CRect rect;
   int   nLocation = 0;

   GetClientRect( rect );
   if ( rect.PtInRect( pt ) )
      nLocation = 1;  // in Agenda client area
   else
   {
      if ( m_pFullDayWnd )
      {
         m_pFullDayWnd->GetClientRect( rect );
         CPoint ptFullDay( pt );
         ClientToScreen( &ptFullDay );
         m_pFullDayWnd->ScreenToClient( &ptFullDay );
         if ( rect.PtInRect( ptFullDay ) )
            nLocation = 2;  // in FullDay client area
      }

      if ( nLocation == 0 && m_pDayMonthYear )
      {
         m_pDayMonthYear->GetWindowRect( rect );
         CPoint ptDayMonthYear( pt );
         ClientToScreen( &ptDayMonthYear );
      // m_pDayMonthYear->ScreenToClient( &ptDayMonthYear );
         if ( rect.PtInRect( ptDayMonthYear ) )
            nLocation = 3;  // in DayMonthYear client area
      }
   }

   if ( nLocation )
   {
      int nDayCnt = GetShownDayCount( );

      int nSizeX = (m_nClientX - m_nTimeGutterWidth) / nDayCnt;
      int nLeft = m_nTimeGutterWidth;

      for ( int k = 0; k < nDayCnt; k++ )
      {
         if ( k == nDayCnt - 1 ||
              (pt.x > nLeft && pt.x <= nLeft + nSizeX) )
         {
            // Get the date for the part of the agenda the mouse is over.
            if ( m_pDayTimer->m_pMiniCal->GetSelectedDateByOrder( date, k ) )
               return( nLocation );
         }

         nLeft += nSizeX;
      }
   }

   return( 0 );
}

void
ZAgenda::UpdateScrollPosition( )
{
   int nPos = GetScrollPos( SB_VERT );
   int nHalfHour = m_nHourHeight / 2;

   int nDiff = nPos % nHalfHour;
   if ( nDiff )
   {
      nPos -= nDiff;
   // ScrollToPosition( CPoint( 0, nPos ) );
      SetScrollPos( SB_VERT, nPos, TRUE );
      Invalidate( );
   }
}

BOOL
ZAgenda::IsDoubleClick( UINT uMessage, CPoint ptStart )
{
   MSG    msg;
   UINT   uInterval = 0;
   LONG   lMessage = GetMessageTime( );
   UINT   uDblclkTime = GetDoubleClickTime( );

   while ( uInterval < uDblclkTime )
   {
      uInterval = (zULONG) (GetCurrentTime( ) - lMessage);
      if ( ::PeekMessage( &msg, m_hWnd, uMessage, uMessage,
                          PM_NOREMOVE | PM_NOYIELD ) )
      {
         return( TRUE );
      }
      else
      if ( ::PeekMessage( &msg, m_hWnd, WM_MOUSEMOVE, WM_MOUSEMOVE,
                          PM_REMOVE | PM_NOYIELD ) )
      {
         CPoint pt( msg.lParam );
         if ( ptStart.x - pt.x > g_lDblClkX ||
              pt.x - ptStart.x > g_lDblClkX ||
              ptStart.y - pt.y > g_lDblClkY ||
              pt.y - ptStart.y > g_lDblClkY )
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            return( FALSE );
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

ZAppointment *
ZAgenda::GetNextShownAppointment( BOOL bBackwards )
{
   COleDateTime date;
   int nDayCnt = GetShownDayCount( );

   if ( m_pAppointmentLastFocus )
   {
      if ( bBackwards )
         m_pAppointmentLastFocus = m_pAppointmentLastFocus->m_pPrev;
      else
         m_pAppointmentLastFocus = m_pAppointmentLastFocus->m_pNext;
   }
   else
   {
      m_pAppointmentLastFocus = m_pAppointmentHead;
      if ( bBackwards )
      {
         while ( m_pAppointmentLastFocus &&
                 m_pAppointmentLastFocus->m_pNext )
         {
            m_pAppointmentLastFocus = m_pAppointmentLastFocus->m_pNext;
         }
      }
   }

   while ( m_pAppointmentLastFocus )
   {
      if ( m_pAppointmentLastFocus->m_bFullDay == FALSE )
      {
         for ( int k = 0; k < nDayCnt; k++ )
         {
            if ( m_pDayTimer->m_nMaxShowAppointment == 0 ||
                 k >= nDayCnt - m_pDayTimer->m_nMaxShowAppointment )
            {
               m_pDayTimer->m_pMiniCal->GetSelectedDateByOrder( date, k );
               if ( date == m_pAppointmentLastFocus->m_date )
               {
                  return( m_pAppointmentLastFocus );
               }
            }
         }
      }

      if ( bBackwards )
         m_pAppointmentLastFocus = m_pAppointmentLastFocus->m_pPrev;
      else
         m_pAppointmentLastFocus = m_pAppointmentLastFocus->m_pNext;
   }

   return( 0 );
}

void
ZAgenda::TrackEditAppointment( ZAppointment *pA, CPoint& pt )
{
// TraceLineS( "ZAgenda::EditAppointment Appointment: ",
//             pA->m_bFullDay ? "FullDay" : "Normal" );
   m_pTrackedAppointment = pA;
   m_ptTracked = pt;
   m_bEditAppointment = TRUE;
   SetFocus( );   // to hide m_editAppointment at current location
   SetCapture( );
}

void
ZAgenda::EditAppointment( ZAppointment *pA )
{
   ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_SELECT_APPOINTMENT, pA );
// TraceLine( "Hit Appointment: %s %s", pA->m_csSubject,
//            pA->m_csMessage );
   ASSERT( m_pDayTimer );
   ASSERT_VALID( pA );

   if ( m_editAppointment.m_pA )
   {
      CRect rect = m_editAppointment.m_pA->m_rectDraw;
      m_editAppointment.m_pA = 0;
      InvalidateRect( rect );
   }

   m_pAppointmentLastFocus = pA;
   m_editAppointment.m_pA = pA;
   m_editAppointment.SetWindowText( pA->m_csMessage );
   m_editAppointment.ShowWindow( SW_SHOW );
   m_editAppointment.SetFocus( );
   m_editAppointment.SetWindowPos( 0, // &CWnd::wndTop,
                                   pA->m_rectDraw.left + g_nGutterBandWidth,
                                   pA->m_rectDraw.top,
                                   pA->m_rectDraw.Width( ) - g_nGutterBandWidth,
                                   pA->m_rectDraw.Height( ),
                                   SWP_FRAMECHANGED
                                   /*SWP_SHOWWINDOW  | SWP_NOZORDER |
                                     SWP_NOOWNERZORDER */ );
   Invalidate( );
}

void
ZAgenda::OnLButtonDown( UINT uFlags, CPoint pt )
{
// TracePoint( "ZAgenda::OnLButtonDown", pt );
   ZAppointment *pA = AppointmentHitTest( pt );
   m_editAppointment.ShowWindow( SW_HIDE );
   if ( pA )
   {
      m_dateLastSelected = pA->m_date;
      if ( m_pDayMonthYear && mIs_hWnd( m_pDayMonthYear->m_hWnd ) )
         m_pDayMonthYear->Invalidate( );

      if ( pt.x > pA->m_rectDraw.left &&
           pt.x < pA->m_rectDraw.left + g_nGutterBandWidth )  // in gutter band
      {
         m_pTrackedAppointmentBand = pA;
         m_ptTracked = pt;
         SetCapture( );
         ::ShowCursor( FALSE );
      // m_wndToolTip.ShowWindow( SW_SHOW );
      }
      else
      if ( IsDoubleClick( WM_LBUTTONDBLCLK, pt ) == FALSE )
      {
         // Start tracking to edit the appointment in place.
         TrackEditAppointment( pA, pt );
      }
   }
   else
   {
   SetFocus( );
   if ( pt.x > m_nTimeGutterWidth )  // not in gutter
   {
      SCROLLINFO si;
      si.cbSize = sizeof( SCROLLINFO );
      si.fMask = SIF_POS;

      GetScrollInfo( SB_VERT, &si );
      int nPos = (pt.y + si.nPos * m_nHourHeight) / (m_nHourHeight / 2);

      m_bTrackingSelected = TRUE;
      SetFocus( );   // to hide m_editAppointment (if any)
      SetCapture( );

      // zero out date - 12/30/1899
      ASSERT( GetDateFromPoint( m_dateLastSelected, pt ) );
      if ( m_pDayMonthYear && mIs_hWnd( m_pDayMonthYear->m_hWnd ) )
         m_pDayMonthYear->Invalidate( );

      m_timeFirstSelected.SetTime( nPos / 2, nPos % 2 ? 30 : 0, 0 );
      if ( m_timeFirstSelected.GetMinute( ) == 0 )
         m_timeLastSelected.SetTime( m_timeFirstSelected.GetHour( ), 30, 0 );
      else
      {
         int nHour = m_timeFirstSelected.GetHour( );

         if ( nHour == 23 )
            m_timeLastSelected.SetTime( 23, 59, 59 );
         else
            m_timeLastSelected.SetTime( nHour + 1, 0, 0 );
      }

      CRect rectClient;
      CRect rectDraw;
      GetClientRect( rectClient );
      SetDrawRect( rectDraw, m_timeFirstSelected,
                   m_timeLastSelected, rectClient, si.nPos );
      Invalidate( ); // too much???

#if 0   // auto add of appointments for testing
      static int nCnt = 0;
      zPCHAR pch;
      nCnt++;

#if 1

      pA = new ZAppointment( m_nSeqNbr++, 0 );
      pA->m_date = m_dateLastSelected;
      pA->m_bFullDay = TRUE;
      pA->m_csSubject.Format( "FullDay Subject %d", nCnt );
      pA->m_csMessage.Format( "%s - FullDay Body", pA->m_csSubject );
      pA->m_timeFrom.SetDate( 100, 0, 0 );
      pA->m_timeTo.SetDate( 100, 0, 0 );
      AddAppointment( pA );
#endif

      pA = new ZAppointment( m_nSeqNbr++, 0 );
      pA->m_date = m_dateLastSelected;
      pA->m_bFullDay = FALSE;
      int nHour = m_timeFirstSelected.GetHour( );
      if ( nHour < 12 )
         pch = "am";
      else
      {
         pch = "pm";
         nHour -= 12;
      }

      if ( nHour == 0 )
         nHour = 12;

      pA->m_csSubject.Format( "%d:%02d%s - Lunch %d", nHour,
                              timeFrom.GetMinute( ), pch, nCnt );
      pA->m_csMessage.Format( "Remember %d", nCnt );
      pA->m_timeFrom = m_timeFirstSelected;
      pA->m_timeTo = m_timeLastSelected;
      AddAppointment( pA );

   // GoToTime( timeFrom ); // Scroll the appointment into view
#endif
   }
   }

   CWnd::OnLButtonDown( uFlags, pt );
// m_pDayTimer->SetFocus( ); not here!!! dks???
}

void
ZAgenda::OnLButtonUp( UINT uFlags, CPoint pt )
{
// TracePoint( "ZAgenda::OnLButtonUp: ", pt );
   if ( m_pTrackedAppointmentBand )
   {
      if ( GetCapture( ) == this )
         ReleaseCapture( );

      // Appointment position will change in "integral 5-minutes" for
      // each 2-pixel change.
      int nDiff = (pt.y - m_ptTracked.y) / 2;
      BOOL bNegative;

      if ( nDiff < 0 )
      {
         nDiff = -nDiff;
         bNegative = TRUE;
      }
      else
      {
         bNegative = FALSE;
      }

      COleDateTimeSpan ts( 0, 0, 5 * nDiff, 0 );
      COleDateTime timeFrom( m_pTrackedAppointmentBand->m_timeFrom );
      COleDateTime timeTo( m_pTrackedAppointmentBand->m_timeTo );

      if ( bNegative )
      {
         timeFrom -= ts;
         timeTo -= ts;
      }
      else
      {
         timeFrom += ts;
         timeTo += ts;
      }

      int nOffsetX = 0;
      int nOffsetY = 0;
      m_wndToolTip.ShowWindow( SW_HIDE );
      if ( timeFrom.GetDay( ) == m_pTrackedAppointmentBand->m_timeFrom.GetDay( ) &&
           timeTo.GetDay( ) == m_pTrackedAppointmentBand->m_timeTo.GetDay( ) )
      {
         nOffsetX = m_ptTracked.x - m_pTrackedAppointmentBand->m_rectDraw.left;
         nOffsetY = m_pTrackedAppointmentBand->m_rectDraw.top - m_ptTracked.y;
         m_pTrackedAppointmentBand->m_timeFrom = timeFrom;
         m_pTrackedAppointmentBand->m_timeTo = timeTo;

         // Now reposition all appointments.
         SortAppointments( );
         SetAppointmentOverlap( );

         GoToTime( m_pTrackedAppointmentBand->m_timeFrom, FALSE );
      }

      m_ptTracked.y = INTFRAC( m_nHourHeight * 24,
                               m_pTrackedAppointmentBand->m_timeFrom );
      SCROLLINFO si;
      si.cbSize = sizeof( SCROLLINFO );
      si.fMask  = SIF_POS | SIF_RANGE | SIF_PAGE;
      GetScrollInfo( SB_VERT, &si );

      m_ptTracked.x = m_pTrackedAppointmentBand->m_rectDraw.left + nOffsetX;
      m_ptTracked.y -= si.nPos * m_nHourHeight + nOffsetY;
      ClientToScreen( &m_ptTracked );
      ::SetCursorPos( m_ptTracked.x, m_ptTracked.y );

   // pt = m_ptTracked;
      m_pTrackedAppointmentBand = 0;
      ::ShowCursor( TRUE );
   }
   else
   if ( m_pTrackedAppointment )
   {
      if ( GetCapture( ) == this )
         ReleaseCapture( );

      ::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
      ZAppointment *pA = AppointmentHitTest( pt );
      if ( m_bEditAppointment == TRUE &&
           pA == m_pTrackedAppointment &&
           mIs_hWnd( m_editAppointment.m_hWnd ) )
      {
         EditAppointment( pA );

      // ZAppointment::OnLButtonDown( uFlags, pt );
      // Fire_OnClick( pA );
      }
      else
      if ( m_bEditAppointment == FALSE )
      {
         COleDateTime date;

         if ( GetDateFromPoint( date, pt ) )
         {
            DropAppointment( m_pTrackedAppointment, date );
         }
         else
         if ( m_pDayTimer && m_pDayTimer->m_pMiniCal )
         {
            ZMiniCalendarMonthCell *pMonthCell;
            ZMiniCalendarDaySpot   *pTrackedDaySpot;
            CPoint ptMiniCal( pt );
            ClientToScreen( &ptMiniCal );
            m_pDayTimer->m_pMiniCal->ScreenToClient( &ptMiniCal );

            if ( (pMonthCell = m_pDayTimer->m_pMiniCal->MonthCellHitTest( ptMiniCal )) != 0 &&
                 (pTrackedDaySpot = pMonthCell->DaySpotHitTest( ptMiniCal )) != 0 )
            {
            // ::MessageBox( 0, "Agenda Drop OK", "Agenda Drag and Drop", MB_OK );
               DropAppointment( m_pTrackedAppointment, pTrackedDaySpot->m_date );
            }
         }

         if ( m_pFullDayWnd->m_pHilightAppointment )
         {
            m_pFullDayWnd->m_pHilightAppointment = 0;
            m_pFullDayWnd->Invalidate( );
         }

         m_pDayTimer->m_ulMapActFlags &= ~zHOLD_DATETIME_ALARM;
      }
      else
      {
         if ( m_pTrackedAppointment->m_bFullDay )
         {
#if 0
            if ( pA )
               ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_SELECT_FULLDAY,
                                      (zPVOID) MAKELONG( pt.x, pt.y ) );
            else
               ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_SELECT_FULLDAYWND,
                                      (zPVOID) MAKELONG( pt.x, pt.y ) );
#endif
            if ( m_pFullDayWnd->m_pHilightAppointment )
            {
               m_pFullDayWnd->m_pHilightAppointment = 0;
               m_pFullDayWnd->Invalidate( );
            }
         }
         else
            ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_SELECT_AGENDA,
                                   (zPVOID) MAKELONG( pt.x, pt.y ) );
      }

      m_pTrackedAppointment = 0;
   }
   else
   if ( m_bTrackingSelected )
   {
      m_bTrackingSelected = FALSE;
      if ( GetCapture( ) == this )
         ReleaseCapture( );
   }

   CWnd::OnLButtonUp( uFlags, pt );
}

void
ZAgenda::OnLButtonDblClk( UINT uFlags, CPoint pt )
{
   ZAppointment *pA = AppointmentHitTest( pt );
   if ( pA )
   {
      ASSERT_VALID( pA );

      if ( ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_DBLCLK_APPOINTMENT,
                                  pA ) == zNO_APPLICATION_EVENT )
      {
         m_pTrackedAppointment = pA;
         m_ptTracked = pt;
         m_bEditAppointment = TRUE;
         SetFocus( );   // to hide m_editAppointment
         SetCapture( );
         OnLButtonUp( uFlags, pt );
      }
   }
   else
   {
      ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_DBLCLK_AGENDA,
                             (zPVOID) MAKELONG( pt.x, pt.y ) );
   }

   CWnd::OnLButtonDblClk( uFlags, pt );
}

void
ZAgenda::OnRButtonDown( UINT uFlags, CPoint pt )
{
   ZAppointment *pA = AppointmentHitTest( pt );
   if ( pA )
      ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_RCLICK_APPOINTMENT, pA );
   else
   {
      SetFocus( );
      ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_RCLICK_AGENDA,
                             (zPVOID) MAKELONG( pt.x, pt.y ) );
   }
}

void
ZAgenda::OnMouseMove( UINT uFlags, CPoint pt )
{
   if ( m_pTrackedAppointmentBand )
   {
      // Show tip in correct position.
      if ( m_pTrackedAppointmentBand )
      {
         // Appointment position will change in "integral 5-minutes" for
         // each 2-pixel change.
         int nDiff = (pt.y - m_ptTracked.y) / 2;
         BOOL bNegative;

         if ( nDiff < 0 )
         {
            nDiff = -nDiff;
            bNegative = TRUE;
         }
         else
         {
            bNegative = FALSE;
         }

         COleDateTimeSpan ts( 0, 0, 5 * nDiff, 0 );
         COleDateTime timeFrom( m_pTrackedAppointmentBand->m_timeFrom );
         COleDateTime timeTo( m_pTrackedAppointmentBand->m_timeTo );

         if ( bNegative )
         {
            timeFrom -= ts;
            timeTo -= ts;
         }
         else
         {
            timeFrom += ts;
            timeTo += ts;
         }

         if ( timeFrom.GetDay( ) == m_pTrackedAppointmentBand->m_timeFrom.GetDay( ) &&
              timeTo.GetDay( ) == m_pTrackedAppointmentBand->m_timeTo.GetDay( ) )
         {
            CString cs = FormatTimeAmPm( timeFrom );
            cs += " - ";
            cs += FormatTimeAmPm( timeTo );
            CPoint ptScreen = m_ptTracked;
            ClientToScreen( &ptScreen );
            m_wndToolTip.ShowTips( ptScreen, cs );
            pt.x = m_ptTracked.x;
         }
      }
   }
   else
   if ( m_pTrackedAppointment )
   {
      if ( m_bEditAppointment == TRUE &&
           PointWithinRange( pt, m_ptTracked, g_nDragX, g_nDragY ) == FALSE )
      {
         m_pDayTimer->m_ulMapActFlags |= zHOLD_DATETIME_ALARM;
         m_bEditAppointment = FALSE;
      }

      if ( m_bEditAppointment == FALSE )
      {
         COleDateTime date;
         BOOL bDropOK = FALSE;

         if ( GetDateFromPoint( date, pt ) )
         {
            if ( date == m_pTrackedAppointment->m_date )
               bDropOK = FALSE;
            else
               bDropOK = TRUE;
         }
         else
         if ( m_pDayTimer && m_pDayTimer->m_pMiniCal )
         {
            ZMiniCalendarMonthCell *pMonthCell;
            ZMiniCalendarDaySpot   *pTrackedDaySpot;
            CPoint ptMiniCal( pt );
            ClientToScreen( &ptMiniCal );
            m_pDayTimer->m_pMiniCal->ScreenToClient( &ptMiniCal );

            if ( (pMonthCell = m_pDayTimer->m_pMiniCal->MonthCellHitTest( ptMiniCal )) != 0 &&
                 (pTrackedDaySpot = pMonthCell->DaySpotHitTest( ptMiniCal )) != 0 )
            {
               bDropOK = TRUE;
            }
         }

         if ( bDropOK )
            ::SetCursor( AfxGetApp( )->LoadCursor( IDC_DROPOKM ) );
         else
            ::SetCursor( AfxGetApp( )->LoadCursor( IDC_DROPNOT ) );
      }
   }
   else
   if ( m_bTrackingSelected )
   {
      SCROLLINFO si;
      si.cbSize = sizeof( SCROLLINFO );
      si.fMask = SIF_POS;

      GetScrollInfo( SB_VERT, &si );
      int nPos = ((pt.y + si.nPos * m_nHourHeight) / (m_nHourHeight / 2)) + 1;

      // zero out date - 12/30/1899
      m_timeLastSelected.SetTime( nPos / 2, nPos % 2 ? 30 : 0, 0 );
      if ( m_timeFirstSelected.GetStatus( ) != COleDateTime::valid )
         m_timeFirstSelected.SetTime( nPos / 2, nPos % 2 ? 30 : 0, 0 );

      if ( m_timeLastSelected.GetStatus( ) != COleDateTime::valid ||
           m_timeLastSelected <= m_timeFirstSelected )
      {
         if ( m_timeFirstSelected.GetMinute( ) == 0 )
            m_timeLastSelected.SetTime( m_timeFirstSelected.GetHour( ), 30, 0 );
         else
            m_timeLastSelected.SetTime( m_timeFirstSelected.GetHour( ) + 1, 0, 0 );
      }

      CRect rectClient;
      CRect rectDraw;
      GetClientRect( rectClient );
      SetDrawRect( rectDraw, m_timeFirstSelected, m_timeLastSelected, rectClient, si.nPos );
      Invalidate( );  // too much???
   }

   CWnd::OnMouseMove( uFlags, pt );
}

BOOL
ZAgenda::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage )
{
   if ( uHitTest == HTCLIENT )
   {
      CPoint pt( GetMessagePos( ) );
      ScreenToClient( &pt );
      ZAppointment *pA = AppointmentHitTest( pt );
      if ( pA && pA->m_bFullDay == FALSE )
      {
         CRect rect = pA->m_rectDraw;

         rect.right = rect.left + g_nGutterBandWidth;
         if ( rect.PtInRect( pt ) )
         {
            ::SetCursor( AfxGetApp( )->LoadCursor( IDC_XSPLIT ) );
            return( TRUE );
         }
      }
   }

// ::SetCursor( AfxGetApp( )->LoadStandardCursor( IDC_SIZENS ) );
   return( CWnd::OnSetCursor( pWnd, uHitTest, uMessage ) );
}

void
ZAgenda::OnContextMenu( CWnd *pWnd, CPoint pt )
{
   ZAppointment *pA = AppointmentHitTest( pt );
   if ( pA )
   {
   // ASSERT( m_pAgenda );
      ASSERT_VALID( pA );
   // Invalidate( );

   // ZAgenda::OnContextMenu( pWnd, pt )
   // m_pAgenda->Fire_OnContextMenu( ipAppt );
   }
}

BOOL
ZAgenda::DropAppointment( ZAppointment  *pA,
                          COleDateTime& dateTracked )
{
   if ( pA->m_date == dateTracked )
      return( FALSE );

   m_pDayTimer->m_ulMapActFlags |= zHOLD_DATETIME_ALARM;
   unsigned int uKey = pA->m_uKey;
   COleDateTime date = pA->m_date;

   pA->m_date = dateTracked;
   pA->m_uKey = 0;

   if ( m_pDayTimer )
   {
      zVIEW vApp;

      if ( uKey && m_pDayTimer->m_pzsVName &&
           (vApp = m_pDayTimer->IsVisibleForMapToOI( *(m_pDayTimer->m_pzsVName) )) != 0 &&
           m_pDayTimer->m_pzsEName && m_pDayTimer->m_pzsAName &&
           *(m_pDayTimer->m_csTimeEntity) &&
           *(m_pDayTimer->m_csSubjectAttribute) &&
           *(m_pDayTimer->m_csMessageAttribute) &&
           *(m_pDayTimer->m_csTimeFromAttribute) &&
           *(m_pDayTimer->m_csTimeToAttribute) &&
           *(m_pDayTimer->m_csAlarmAttribute) &&
           *(m_pDayTimer->m_csFullDayAttribute) )
      {
         zCHAR  szDate[ 20 ];

         SetZeidonDateFromOleDateTime( szDate, sizeof( szDate ), date );
         if ( SetCursorFirstEntityByString( vApp,
                                            *(m_pDayTimer->m_pzsEName),
                                            *(m_pDayTimer->m_pzsAName),
                                            szDate, 0 ) == zCURSOR_SET &&
              SetEntityCursor( vApp, m_pDayTimer->m_csTimeEntity, 0,
                               zQUAL_ENTITYKEY | zPOS_FIRST,
                               (zCPVOID) &uKey, 0, 0, 0,
                               *(m_pDayTimer->m_pzsEName),
                               0 ) >= zCURSOR_SET )
         {
            zVIEW vTgt;

            CreateViewFromViewForTask( &vTgt, vApp, m_pDayTimer->m_pZSubtask->m_vDialog );
            SetZeidonDateFromOleDateTime( szDate, sizeof( szDate ), dateTracked );
            if ( SetCursorFirstEntityByString( vTgt,
                                               *(m_pDayTimer->m_pzsEName),
                                               *(m_pDayTimer->m_pzsAName),
                                               szDate, 0 ) != zCURSOR_SET )
            {
               CreateEntity( vTgt, *(m_pDayTimer->m_pzsEName), zPOS_AFTER );
               SetZeidonDateAttributeFromOleDateTime( dateTracked, vTgt,
                                                      *(m_pDayTimer->m_pzsEName),
                                                      *(m_pDayTimer->m_pzsAName) );
            }

            CreateEntity( vTgt, m_pDayTimer->m_csTimeEntity, zPOS_AFTER );
            SetMatchingAttributesByName( vTgt, m_pDayTimer->m_csTimeEntity,
                                         vApp, m_pDayTimer->m_csTimeEntity,
                                         zSET_ALL );
            pA->m_uKey = GetEntityKey( vTgt, m_pDayTimer->m_csTimeEntity );
            DeleteEntity( vApp, m_pDayTimer->m_csTimeEntity, zREPOS_PREV );
         }
      }

      // Now reposition all appointments.
      SortAppointments( );
      SetAppointmentOverlap( );
      UpdatePositionOfControls( );
      Invalidate( );
      if ( m_pDayTimer->m_pMiniCal )
         m_pDayTimer->m_pMiniCal->Invalidate( );

      if ( m_pFullDayWnd )
         m_pFullDayWnd->Invalidate( );
   }

   Invalidate( );
   m_pDayTimer->m_ulMapActFlags &= ~zHOLD_DATETIME_ALARM;
   return( TRUE );
}

void
ZAgenda::GoToTime( COleDateTime& Time, BOOL bMoveToTop )
{
   SCROLLINFO si;
   int nIncrement;
   int nPos = INTFRAC( 24, Time );

   si.cbSize = sizeof( SCROLLINFO );
   si.fMask  = SIF_POS | SIF_RANGE | SIF_PAGE;
   GetScrollInfo( SB_VERT, &si );

   if ( bMoveToTop || nPos < si.nPos ||
        nPos > (int) (si.nPos + si.nPage) )
   {
      nIncrement = si.nPos;
      si.nPos = max( 0, min( si.nMax, nPos ) );
      nIncrement -= si.nPos;
      SetScrollInfo( SB_VERT, &si, TRUE );

      // A negative value will scroll the content of the window up.
      ScrollWindow( 0, -nIncrement * m_nHourHeight );
   }

   Invalidate( );  // take this out when things are done right???
   UpdateWindow( );
}

void
ZAgenda::RemoveAllAppointments( )
{
   while ( m_pAppointmentHead )
      mDeleteInit( m_pAppointmentHead );
}

int
ZAgenda::DeleteAppointment( ZAppointment *pA )
{
#if 1

   pA->UnlinkAppointment( this );
   if ( m_pDayTimer && m_pDayTimer->m_pMiniCal )
      m_pDayTimer->m_pMiniCal->Invalidate( );

   delete( pA );
   SetAppointmentOverlap( );
   UpdatePositionOfControls( );
   Invalidate( );
   return( 0 );

#else

   ZAppointment *pTemp = m_pAppointmentHead;
   if ( pA == m_pAppointmentHead )
   {
      m_pAppointmentHead = m_pAppointmentHead->m_pNext;
      if ( m_pAppointmentHead )
         m_pAppointmentHead->m_pPrev = 0;
   }
   else
   {
      while ( pTemp )
      {
         if ( pA == pTemp->m_pNext )
         {
            pTemp->m_pNext = pA->m_pNext;
            if ( pA->m_pNext )
               pA->m_pNext->m_pPrev = pTemp;

            pTemp = pA;
            break;
         }

         pTemp = pTemp->m_pNext;
      }
   }

   if ( pTemp == pA )
   {
      pA->m_pNext = 0;
      delete( pA );
      Invalidate( );
      return( 0 );
   }

   return( -1 );

#endif
}

ZAppointment *
ZAgenda::AddAppointment( COleDateTime& date,
                         COleDateTime& timeFrom, COleDateTime& timeTo,
                         zCPCHAR cpcSubject, zCPCHAR cpcMessage,
                         zLONG lAlarm, zLONG lAppointmentType,
                         zBOOL bFullDay /* = FALSE */ )
{
   ZAppointment *pA = 0;
   if ( timeFrom > 1 || timeTo > 1 )
   {
      MessageBox( "Invalid time specified. "
                  "Only hour/minute/second intervals are valid. "
                  "Like \"12.34.23\". You cannot specify a date." );
   }
   else
   if ( cpcSubject == 0 || cpcSubject[ 0 ] == 0 )
   {
      MessageBox( "A subject has not been specified for the appointment" );
   }
   else
   if ( bFullDay == FALSE && timeFrom >= timeTo )
   {
      MessageBox( "The appointment start time is larger than the end time" );
   }
   else
   {
      pA = new ZAppointment( m_nSeqNbr++, 0 );

      pA->m_date = date;
      pA->m_bFullDay = bFullDay;
      pA->m_nOverlap = 0;
      pA->m_csSubject = cpcSubject ? cpcSubject : "";
      pA->m_csMessage = cpcMessage ? cpcMessage : "";
      pA->m_nAlarm = lAlarm;
      pA->m_nAppointmentType = lAppointmentType;
      if ( bFullDay )
      {
         pA->m_timeFrom.SetDate( 100, 0, 0 );
         pA->m_timeTo.SetDate( 100, 0, 0 );
      }
      else
      {
         pA->m_timeFrom = timeFrom;
         pA->m_timeTo = timeTo;
      }

      AddAppointment( pA );
   }

   return( pA );
}

void
ZAgenda::AddAppointment( ZAppointment *pA )
{
   ASSERT_VALID( pA );

// CString cs;
// cs.Format( "Before AddAppointment: %s", pA->m_csSubject );
// TraceAppointmentList( cs );

   // If the appointment already exists (based on key), just remove
   // the existing one from the list and delete it.
   if ( pA->m_uKey )
   {
      ZAppointment *pTemp;
      pTemp = m_pAppointmentHead;
      while ( pTemp )
      {
         if ( pTemp->m_uKey == pA->m_uKey )
         {
            DeleteAppointment( pTemp );
            break;
         }

         pTemp = pTemp->m_pNext;
      }
   }

   if ( m_pAppointmentHead == 0 )
   {
      m_pAppointmentHead = pA;
   }
   else
   {
#if 1
      // Simply add the new appointment to the head of the list since
      // we are going to sort anyway.
      m_pAppointmentHead->m_pPrev = pA;
      pA->m_pNext = m_pAppointmentHead;
      m_pAppointmentHead = pA;

#else

      // First, add the new appointment to the list.  Add it past any
      // full day appointments.  This has the side-effect of putting
      // full day appointments into the list in the order created.
      if ( m_pAppointmentHead->m_bFullDay == FALSE )
      {
         m_pAppointmentHead->m_pPrev = pA;
         pA->m_pNext = m_pAppointmentHead;
         m_pAppointmentHead = pA;
      }
      else
      {
         ZAppointment *pATmp = m_pAppointmentHead;
         while ( pATmp->m_pNext && pATmp->m_pNext->m_bFullDay )
            pATmp = pATmp->m_pNext;

         if ( pATmp->m_pNext )
         {
            pATmp->m_pNext->m_pPrev = pA;
            pA->m_pNext = pATmp->m_pNext;
         }

         pATmp->m_pNext = pA;
         pA->m_pPrev = pATmp;
      }
#endif

      // Now resort all appointments by position.
      TraceAppointmentList( "Before SortAppointments" );
      SortAppointments( );

      // Now set overlap attributes.
      TraceAppointmentList( "Before SetAppointmentOverlap" );
      SetAppointmentOverlap( );
   }

   UpdatePositionOfControls( );
   TraceAppointmentList( "After AddAppointment" );
   if ( pA->m_bFullDay )
      m_pFullDayWnd->Invalidate( );

   if ( m_pDayTimer && m_pDayTimer->m_pMiniCal )
      m_pDayTimer->m_pMiniCal->Invalidate( );

   Invalidate( );
}

// Set the position and size of the FullDayWnd and Agenda windows
// based on the number of "full day" appointments.
void
ZAgenda::UpdatePositionOfControls( )
{
   int nHeightOfMiniCal = 0;

   if ( m_pDayTimer && m_pDayTimer->m_pMiniCal )
   {
      CSize size = m_pDayTimer->m_pMiniCal->ComputeTotalSize( );
      nHeightOfMiniCal = size.cy;
   }

   int nHeightOfFullDay = GetNumberOfFulldayAppointments( ) *
                                                 g_nFullDayAppointmentHeight;

   m_nHeightOfDayMonthYear = 0;
   if ( m_pDayMonthYear && mIs_hWnd( m_pDayMonthYear->m_hWnd ) )
   {
      // Force size of Agenda to be "integral hour height"
#if 0
   // m_nHourHeight = g_nFullDayAppointmentHeight * 2;
   // int nHalfHour = m_nHourHeight / 2;

      int nFullHours = m_nClientY / m_nHourHeight;
      int nRemainder = m_nClientY % m_nHourHeight;
      if ( nRemainder > m_nHourHeight / 2 )
         nFullHours++;

      nFullHours *= m_nHourHeight;  // total height
      nFullHours++;  // add one for border
#endif

      int nRemaining = m_nClientY - (nHeightOfFullDay + 1);
      m_nHeightOfDayMonthYear = nRemaining % m_nHourHeight;
      if ( m_nHeightOfDayMonthYear < g_nFullDayAppointmentHeight )
         m_nHeightOfDayMonthYear += m_nHourHeight;

      m_pDayMonthYear->SetWindowPos( 0, m_nTimeGutterWidth, nHeightOfMiniCal,
                                     m_nClientX - m_nTimeGutterWidth, // -
                                         // GetSystemMetrics( SM_CXVSCROLL ),
                                     m_nHeightOfDayMonthYear,
                                     SWP_NOZORDER | SWP_NOACTIVATE );
   }

   if ( m_pFullDayWnd && mIs_hWnd( m_pFullDayWnd->m_hWnd ) )
   {
      m_pFullDayWnd->SetWindowPos( 0, m_nTimeGutterWidth,
                                   nHeightOfMiniCal + m_nHeightOfDayMonthYear,
                                   m_nClientX - m_nTimeGutterWidth,
                                   nHeightOfFullDay,
                                   SWP_NOZORDER | SWP_NOACTIVATE );
   }

   if ( mIs_hWnd( m_hWnd ) )
   {
      SetWindowPos( 0, 0,
                    nHeightOfMiniCal + m_nHeightOfDayMonthYear + nHeightOfFullDay,
                    m_nClientX,
                    m_nClientY - m_nHeightOfDayMonthYear - nHeightOfFullDay,
                    SWP_NOZORDER | SWP_NOACTIVATE );
      UpdateScrollSize( );
   }
}

void
ZAgenda::SetAppointmentOverlap( )
{
   ZAppointment *pStartOverlap;
   ZAppointment *pEndOverlap;
   ZAppointment *pNext;
   ZAppointment *pFirstForDate;
   ZAppointment *pA;

   if ( m_pAppointmentHead )
   {
      // First reset all overlap attributes.
      pA = m_pAppointmentHead;
      while ( pA )
      {
         pA->m_nOverlap = 0;
         pA->m_nOverlapMax = 0;
         pA->m_nOverlapPos = 0;
         pA->m_pStartOverlap = 0;
         pA->m_pEndOverlap = 0;
         pA = pA->m_pNext;
      }

      // Set the number of appointments that overlap each appointment.
      pA = m_pAppointmentHead;
      pFirstForDate = pA;
      while ( pA )
      {
         if ( pA->m_date != pFirstForDate->m_date )
            pFirstForDate = pA;

         if ( pA->m_bFullDay == FALSE )
         {
            pNext = pFirstForDate;
            while ( pNext && pNext->m_date == pFirstForDate->m_date )
            {
               if ( pA != pNext && pNext->m_bFullDay == FALSE &&
                    pNext->m_timeFrom < pA->m_timeTo &&
                    pNext->m_timeTo > pA->m_timeFrom )
               {
                  pA->m_nOverlap++;
               }

               pNext = pNext->m_pNext;
            }
         }

         pA->m_nOverlapMax = pA->m_nOverlap;
         pA = pA->m_pNext;
      }

      // Set the maximum number of appointments that overlap each appointment.
      // This is the number of appointments that overlap the appointments that
      // overlap this appointment.
      COleDateTime timeFrom;
      COleDateTime timeTo;
      int  nMax;

      pA = m_pAppointmentHead;
      pFirstForDate = pA;
      while ( pA )
      {
         if ( pA->m_date != pFirstForDate->m_date )
            pFirstForDate = pA;

         if ( pA->m_bFullDay == FALSE )
         {
            timeFrom = pA->m_timeFrom;
            timeTo = pA->m_timeTo;
            nMax = 0;

            pNext = pA->m_pNext;
            while ( pNext && pNext->m_date == pFirstForDate->m_date )
            {
               if ( pNext->m_timeFrom >= timeTo ||
                    pNext->m_timeTo <= timeFrom )
               {
                  break;  // no overlap ... get out of inner loop
               }
               else
               {
                  nMax++;
                  if ( pNext->m_timeFrom < timeFrom )
                     timeFrom = pNext->m_timeFrom;

                  if ( pNext->m_timeTo > timeTo )
                     timeTo = pNext->m_timeTo;

                  pNext = pNext->m_pNext;
               }
            }

            if ( nMax )
               nMax++;

            pStartOverlap = pA;
            pEndOverlap = pNext;
            while ( pA != pNext )
            {
               pA->m_pStartOverlap = pStartOverlap;
               pA->m_pEndOverlap = pEndOverlap;
               pA->m_nOverlapMax = nMax;
               pA = pA->m_pNext;
            }

            // should be done:  pA = pNext;
         }
         else
            pA = pA->m_pNext;
      }

      // Set the overlap position of overlapped appointments.
      int nTryPos;

      pA = m_pAppointmentHead;
      while ( pA )
      {
         if ( pA->m_nOverlapMax )
         {
            nTryPos = 1;
            pNext = pA->m_pStartOverlap;
            while ( pNext != pA->m_pEndOverlap )
            {
               // Must be within immediate overlap.
               if ( pA != pNext &&
                    pNext->m_timeFrom < pA->m_timeTo &&
                    pNext->m_timeTo > pA->m_timeFrom )
               {
                  if ( pNext->m_nOverlapPos == nTryPos )
                  {
                     nTryPos++;
                     pNext = pA->m_pStartOverlap;  // restart
                  }
                  else
                     pNext = pNext->m_pNext;
               }
               else
                  pNext = pNext->m_pNext;
            }

            ASSERT( nTryPos <= pA->m_nOverlapMax );
            pA->m_nOverlapPos = nTryPos;
         }

         pA = pA->m_pNext;
      }

      // Finally reset maximum overlap in each overlapped set.
      pA = m_pAppointmentHead;
      while ( pA )
      {
         if ( pA->m_nOverlapMax )
         {
            nMax = 0;
            pNext = pA->m_pStartOverlap;
            while ( pNext != pA->m_pEndOverlap )
            {
               if ( pNext->m_nOverlapPos > nMax )
                  nMax = pNext->m_nOverlapPos;

               pNext = pNext->m_pNext;
            }

            pNext = pA->m_pStartOverlap;
            while ( pNext != pA->m_pEndOverlap )
            {
               pNext->m_nOverlapMax = nMax;
               pNext = pNext->m_pNext;
            }

            pA = pA->m_pEndOverlap;
         }

         if ( pA )
            pA = pA->m_pNext;
      }
   }
}

// Put the appointments in sorted order by:
//   1. Date
//   2. Full day
//   3. earliest y start position
//   4. latest y end position
//   5. insert order
// This is kind of brute force since the number of appointments
// for a given day should be quite small.
void
ZAgenda::SortAppointments( )
{
   ZAppointment *pA = m_pAppointmentHead;
   ZAppointment *pNext;

   while ( pA && pA->m_pNext )
   {
      pNext = pA->m_pNext;
      if ( pNext->m_date < pA->m_date )
      {
         MoveBefore( pA, pNext );
         pA = m_pAppointmentHead;  // restart sort
      }
      else
      if ( pNext->m_date > pA->m_date )
      {
         pA = pNext;
      }
      else // pNext->m_date == pA->m_date
      if ( pA->m_bFullDay )
      {
         if ( pNext->m_nSeqNbr >= pA->m_nSeqNbr )
            pA = pNext;
         else
         if ( pNext->m_bFullDay )
         {
            MoveBefore( pA, pNext );
            pA = m_pAppointmentHead;  // restart sort
         }
         else
            pA = pNext;
      }
      else
      if ( pNext->m_bFullDay )
      {
         pA = pNext->m_pPrev;
         while ( pA && pA->m_bFullDay == FALSE )
         {
            MoveBefore( pA, pNext );
            pA = pNext->m_pPrev;
         }

         pA = m_pAppointmentHead;  // restart sort
      }
      else  // not a full-day appointment
      if ( pNext->m_timeFrom > pA->m_timeFrom ||
           (pNext->m_timeFrom == pA->m_timeFrom &&
            pNext->m_timeTo < pA->m_timeTo) ||
           (pNext->m_timeFrom == pA->m_timeFrom &&
            pNext->m_timeTo == pA->m_timeTo &&
            pNext->m_nSeqNbr >= pA->m_nSeqNbr) )
      {
         pA = pNext;
      }
      else
      {
         MoveBefore( pA, pNext );
         pA = m_pAppointmentHead;  // restart sort
      }
   }
}

int
ZAgenda::GetAppointmentCountOnDate( COleDateTime& date )
{
   ZAppointment *pA = m_pAppointmentHead;
   int nCnt = 0;

   while ( pA )
   {
      if ( pA->m_date == date )
         nCnt++;

      pA = pA->m_pNext;
   }

   return( nCnt );
}

// Trace appointment list.
void
ZAgenda::TraceAppointmentList( zCPCHAR cpcText )
{
#ifdef DEBUG_ALL
   ZAppointment *pA = m_pAppointmentHead;
   int nCnt = 0;

   TraceLineS( "ZAgenda::TraceAppointmentList: ", cpcText );
   while ( pA )
   {
      nCnt++;
      TraceLine( " %d%s==> %s *** %s - Overlap: %d.%d  ... Prev: %s",
                 nCnt, pA->m_bFullDay ? "*" : " ",
                 pA->m_csSubject, pA->m_csMessage,
                 pA->m_nOverlap, pA->m_nOverlapMax,
                 pA->m_pPrev ? pA->m_pPrev->m_csSubject : "None" );
      pA = pA->m_pNext;
   }
#endif
}

// Move the "next" appointment prior to the "current" appointment.
void
ZAgenda::MoveBefore( ZAppointment *pCurr,
                     ZAppointment *pNext )
{
   // Leave full-day appointments (at the same date) alone.
   if ( pCurr->m_bFullDay == FALSE || pCurr->m_date != pNext->m_date ||
        (pCurr->m_bFullDay && pNext->m_bFullDay &&
         pCurr->m_nSeqNbr > pNext->m_nSeqNbr) )
   {
      pCurr->m_pNext = pNext->m_pNext;
      pNext->m_pPrev = pCurr->m_pPrev;
      pCurr->m_pPrev = pNext;
      pNext->m_pNext = pCurr;

      if ( pNext->m_pPrev == 0 )
         m_pAppointmentHead = pNext;
      else
         pNext->m_pPrev->m_pNext = pNext;

      if ( pCurr->m_pNext )
         pCurr->m_pNext->m_pPrev = pCurr;
   }
}

#if 0  // not used at this point ... needs to check on
       // appointment date if it becomes useful.
void
ZAgenda::MoveTop( ZAppointment *pA )
{
   if ( pA == m_pAppointmentHead ||
        (pA->m_pPrev && pA->m_pPrev->m_bFullDay) )
   {
      return;
   }

   // Keep full-day appointments at the beginning.
   ZAppointment *pA_Head = m_pAppointmentHead;
   while ( pA_Head->m_bFullDay )
   {
      if ( pA_Head->m_pNext )
         pA_Head = pA_Head->m_pNext;
      else
      {
         pA_Head->m_pNext = pA;
         pA->m_pPrev = pA_Head;
         return;
      }
   }

   if ( pA != pA_Head )
   {
      // Bring the specified appointment prior to pA_Head.
      // First, remove pA from the linklist.
      if ( pA->m_pPrev )
         pA->m_pPrev->m_pNext = pA->m_pNext;

      if ( pA->m_pNext )
         pA->m_pNext->m_pPrev = pA->m_pPrev;

      pA->m_pPrev = pA_Head->m_pPrev;
      pA_Head->m_pPrev = pA;
      pA->m_pNext = pA_Head;
      if ( pA->m_pPrev == 0 )
         m_pAppointmentHead = pA;
   }
}
#endif

ZAppointment *
ZAgenda::AppointmentHitTest( CPoint& pt )
{
   COleDateTime date;

   if ( GetDateFromPoint( date, pt ) != 1 )
      date = m_pDayTimer->m_pMiniCal->m_dateToday;

   ZAppointment *pA;
   for ( pA = m_pAppointmentHead; pA; pA = pA->m_pNext )
   {
      if ( date == pA->m_date && pA->m_bFullDay == FALSE &&
           pA->m_rectDraw.PtInRect( pt ) )
      {
         return( pA );
      }
   }

   return( 0 );
}

int
ZAgenda::GetNumberOfFulldayAppointments( )
{
   ZAppointment *pFirstForDate;
   ZAppointment *pA;
   COleDateTime date;
   int nMaxCnt = 0;
   int nCnt = 0;
   int nDayCnt = GetShownDayCount( );

   pA = m_pAppointmentHead;
   pFirstForDate = pA;
   while ( pA )
   {
      if ( pA->m_date != pFirstForDate->m_date )
      {
         if ( nMaxCnt < nCnt )
         {
            for ( int k = 0; k < nDayCnt; k++ )
            {
               if ( m_pDayTimer->m_nMaxShowAppointment == 0 ||
                    k >= nDayCnt - m_pDayTimer->m_nMaxShowAppointment )
               {
                  m_pDayTimer->m_pMiniCal->GetSelectedDateByOrder( date, k );
                  if ( date == pFirstForDate->m_date )
                  {
                     nMaxCnt = nCnt;
                     break;
                  }
               }
            }
         }

         nCnt = 0;
         pFirstForDate = pA;
      }

      if ( pA->m_bFullDay )
         nCnt++;

      pA = pA->m_pNext;
   }

   if ( nMaxCnt < nCnt )
      nMaxCnt = nCnt;

   return( nMaxCnt );
}

BOOL
ZAgenda::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN &&
        (m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      if ( pMsg->wParam == VK_ESCAPE )
      {
      // TraceLineS( "ZAgenda::PreTranslateMessage: ", "VK_ESCAPE" );
         m_editAppointment.ShowWindow( SW_HIDE );
         m_editAppointment.m_pA = 0;
         m_bEditAppointment = FALSE;
         if ( m_pTrackedAppointmentBand ||
              m_pTrackedAppointment )
         {
            if ( m_pTrackedAppointmentBand )
            {
               m_wndToolTip.ShowWindow( SW_HIDE );
               GoToTime( m_pTrackedAppointmentBand->m_timeFrom,
                         FALSE );
               m_pTrackedAppointmentBand = 0;
               ::ShowCursor( TRUE );
            }
            else
            {
               m_pTrackedAppointment = 0;
               ::SetCursor( AfxGetApp( )->LoadCursor( IDC_ARROW ) );
            }

            ClientToScreen( &m_ptTracked );
            ::SetCursorPos( m_ptTracked.x, m_ptTracked.y );
            if ( GetCapture( ) == this )
               ::ReleaseCapture( );

            return( TRUE );
         }
      }
      else
      if ( pMsg->wParam == VK_RETURN )
      {
         if ( m_dateLastSelected.GetStatus( ) == COleDateTime::valid &&
              m_timeFirstSelected < m_timeLastSelected )
         {
            ZAppointment *pA = new ZAppointment( m_nSeqNbr++, 0 );
            pA->m_date = m_dateLastSelected;
            pA->m_timeFrom = m_timeFirstSelected;
            pA->m_timeTo = m_timeLastSelected;
            AddAppointment( pA );
            UpdateWindow( );
            CPoint pt( pA->m_rectDraw.left + 1, pA->m_rectDraw.top + 1 );
            TrackEditAppointment( pA, pt );
            if ( GetCapture( ) == this )
               ::ReleaseCapture( );

            m_pTrackedAppointment = 0;
            ::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
            EditAppointment( pA );
         }

         return( TRUE );
      }
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0
How does Windows know to display that little size arrow cursor when the
 user moves the mouse into the frame of a sizable window? By sending the
 window a special message, WM_NCHITTEST. The window returns a code
 indicating which area of the window the mouse is in. HTCAPTION says the
 mouse is in the caption; HTMINBUTTON says it's in the minimize button,
 and so on (see "Dave's Top Ten List of Tricks, Hints, and Techniques for
 Programming in Windows," MSJ October 1992, for more information on
 WM_NCHITTEST-Ed.). Of special relevance now are the hit test codes
 HTLEFT, HTRIGHT, HTTOP, HTTOPLEFT, HTTOPRIGHT, HTBOTTOM, HTBOTTOMLEFT,
 and HTBOTTOMRIGHT. As the names suggest, these codes indicate that the
 mouse is in some part of the window frame where sizing is allowed. For
 example, HTBOTTOMRIGHT tells Windows the mouse is in the bottom, right
 corner of the frame. When your window proc returns HTBOTTOMRIGHT, Windows
 displays the northwest/southeast arrow cursor to cue the user that he can
 size the window by dragging. Likewise, if you return HTLEFT or HTRIGHT,
 Windows displays the east-west arrow to indicate that the window can be
 sized horizontally. The Windows default window procedure, DefWindowProc,
 which is where MFC routes all unhandled messages, figures out where the
 mouse is and returns the appropriate HTxxx code. If the window doesn't
 allow sizing-that is, if it has WS_BORDER instead of WS_THICKFRAME as
 its border style-then DefWindowProc returns another code, HTBORDER, that
 tells Windows the mouse is in the border but sizing is not allowed. In
 that case, Windows displays its normal arrow cursor and sizing is not allowed.

Suppose you had a WS_THICKFRAME border but implemented your own handler
for WM_NCHITTEST that always returns HTBORDER when the mouse is in the
border? That's exactly what I did to fix MONITOR (see Figure 13). The
CMonitorWindow constructor creates the window with the WS_THICKFRAME
border style to give it the thick sizeable frame. This eliminates the
problem with the two-pixel high client area. But then, to prevent the
user from sizing the window, I implemented my own WM_ONNCHITTEST handler.
CMonitorWindow::OnNcHitTest first calls the base class CFrameWnd::OnNcHitTest
to get the hit test code DefWindowProc would normally return, then
CMonitorWindow mungs it. If the normal response would be HTTOP or HTBOTTOM,
CMonitorWindow returns HTBORDER instead. This has exactly the same effect
as a nonsizeable border. What's even cooler is that if CFrameWnd::OnNcHitTest
 returns HTLEFT, HTTOPLEFT or HTBOTTOMLEFT (any of the LEFT codes),
 CMonitorWindow returns HTLEFT, and if CFrameWnd returns HTRIGHT, HTTOPRIGHT,
 or HTBOTTOMRIGHT, CMonitorWindow returns HTRIGHT. The result is that the
 user can still size the window horizontally, which makes sense for a window
 that's only a title bar. If the user moves the mouse into one of the
 corners, Windows displays an east-west size arrow cursor, not a diagonal
 one. Pretty neat. If you don't want this feature, you can just return
 HTBORDER in all cases.
#endif

zLONG OPERATION
EditAppointmentWndProc( HWND     hWnd,
                        WORD     uMsg,
                        WPARAM   wParam,
                        LPARAM   lParam )
{
#if 0
   TRACE_DEBUG( "<EDITFLD.CPP>", "EditAppointmentWndProc" );
   static ZWndMsgMap msgMap;

   if ( uMsg != 32 && uMsg != 132 && uMsg != 512 )
      TraceLineS( "EditAppointmentWndProc Msg: ", msgMap.GetMsgName( uMsg ) );
#endif

   ZEditAppointment *pEA = (ZEditAppointment *) ::GetWindowLong( hWnd,
                                                                 GWL_USERDATA );
   if ( pEA )
   {
      if ( uMsg == WM_ENTERSIZEMOVE )
      {
         pEA->m_bSizeMove = TRUE;
         pEA->m_timeFrom.SetStatus( COleDateTime::invalid );
         pEA->m_timeTo.SetStatus( COleDateTime::invalid );
         pEA->m_rectSizing.SetRect( -1, -1, -1, -1 );
      }
      else
      if ( uMsg == WM_EXITSIZEMOVE )
      {
         pEA->m_bSizeMove = FALSE;
         if ( (pEA->m_rectSizing.left != -1 ||
               pEA->m_rectSizing.top != -1 ||
               pEA->m_rectSizing.left != -1 ||
               pEA->m_rectSizing.top != -1) &&
              (pEA->m_timeFrom.GetStatus( ) == COleDateTime::valid &&
               pEA->m_timeTo.GetStatus( ) == COleDateTime::valid) )
         {
            ZAppointment *pA = pEA->m_pA;
            pEA->m_pA = 0;
            pA->m_timeFrom = pEA->m_timeFrom;
            pA->m_timeTo = pEA->m_timeTo;
            pEA->Invalidate( );
            pEA->ShowWindow( SW_HIDE );
            pEA->m_pAgenda->m_wndToolTip.ShowWindow( SW_HIDE );
            pEA->m_rectSizing.SetRect( -1, -1, -1, -1 );

            // Now reposition all appointments.
            pEA->m_pAgenda->SortAppointments( );
            pEA->m_pAgenda->SetAppointmentOverlap( );
            pEA->m_pAgenda->GoToTime( pA->m_timeFrom, FALSE );
         }

         pEA->m_pAgenda->SetFocus( );
      }

      return( CallWindowProc( pEA->m_OldWndProc,
                              hWnd, uMsg, wParam, lParam ) );
   }

   return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}


BEGIN_MESSAGE_MAP( ZEditAppointment, CEdit )
   //{{AFX_MSG_MAP( ZEditAppointment )
   ON_WM_KILLFOCUS( )
   ON_WM_SIZE( )
   ON_WM_MOVE( )
   ON_WM_SIZING( )
   ON_WM_KEYDOWN( )
   ON_WM_CHAR( )
   ON_WM_CREATE( )
   ON_WM_NCPAINT( )
   ON_WM_NCHITTEST( )
   ON_WM_WINDOWPOSCHANGING( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZEditAppointment::ZEditAppointment( ZAgenda *pAgenda ) :
                  m_rectSizing( -1, -1, -1, -1 )
{
   ASSERT( pAgenda );

   m_pAgenda = pAgenda;
   m_pA = 0;
   m_bESC = FALSE;
   m_bSizeMove = FALSE;
   m_OldWndProc = 0;
   m_timeFrom.SetStatus( COleDateTime::invalid );
   m_timeTo.SetStatus( COleDateTime::invalid );
}

ZEditAppointment::~ZEditAppointment( )
{
   if ( m_OldWndProc )
   {
      ::SetWindowLong( m_hWnd, GWL_WNDPROC, (zLONG) m_OldWndProc );
      m_OldWndProc = 0;
   }
}

void
ZEditAppointment::OnWindowPosChanging( WINDOWPOS *pWndPos )
{
   if ( m_bSizeMove )
   {
      // Don't let this message cause a resize or move.  All will be
      // handled after EXITSIZEMOVE.
      pWndPos->flags |= SWP_NOMOVE | SWP_NOSIZE;
   }
   else
   {
      CEdit::OnWindowPosChanging( pWndPos );
   }
}

BOOL
ZEditAppointment::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN )
   {
      if ( pMsg->wParam == VK_TAB )
      {
         TraceLineS( "ZEditAppointment::PreTranslateMessage: ", "VK_TAB" );
      }
      else
      if ( pMsg->wParam == VK_RETURN ||
           pMsg->wParam == VK_DELETE ||
           pMsg->wParam == VK_ESCAPE ||
           ::GetKeyState( VK_CONTROL ) < 0 )
      {
         ::TranslateMessage( pMsg );

         // Strange but true:
         // If the edit control has ES_MULTILINE and ESC is pressed
         // the parent is destroyed if the message is dispatched.
         // In this case the parent is the Agenda control.
         if ( (GetStyle( ) & ES_MULTILINE) == 0 ||
              pMsg->wParam != VK_ESCAPE )
         {
            ::DispatchMessage( pMsg );
         }

         return( TRUE ); // do NOT process further
      }
   }

   return( CEdit::PreTranslateMessage( pMsg ) );
}

#if 0

Technique for Hilighting edit control with focus!!!

You don't have to use these odd styles (WS_EX_RIGHT and WS_EX_RTLREADING)
to reflect the frame changement.  Instead, use the third (and optional)
parameter of ModifyStyleEx which is a bit field for the SetWindowPos
method. Use the SWP_FRAMECHANGED flag and it works great:

void CBorderEdit::OnKillFocus(CWnd* pNewWnd)
{
   CEdit::OnKillFocus(pNewWnd);

   // TODO: Add your message handler code here
   ModifyStyleEx( WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0, SWP_FRAMECHANGED );
}

void CBorderEdit::OnSetFocus(CWnd* pOldWnd)
{
   CEdit::OnSetFocus( pOldWnd );

   // TODO: Add your message handler code here
   ModifyStyleEx( 0, WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, SWP_FRAMECHANGED );
}

#endif

void
ZEditAppointment::OnKillFocus( CWnd *pWndGetFocus )
{
   Invalidate( );
   ShowWindow( SW_HIDE );
   m_pAgenda->m_wndToolTip.ShowWindow( SW_HIDE );
   if ( m_pA )
   {
      if ( m_bESC == FALSE )
         GetWindowText( m_pA->m_csMessage );

      if ( m_pA->m_uKey == 0 && m_pA->m_nAlarm == 0 &&
           m_pA->m_csMessage.IsEmpty( ) && m_pA->m_csSubject.IsEmpty( ) )
      {
         if ( m_bESC )
            m_pAgenda->DeleteAppointment( m_pA );
         else
            m_pA->m_nAlarm = 1;
      }
      else
      {
         ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                                zDAYTIMER_EDIT_CHANGE_APPOINTMENT, m_pA );
      }

      m_pA = 0;
      m_pAgenda->Invalidate( );
   }

   m_timeFrom.SetStatus( COleDateTime::invalid );
   m_timeTo.SetStatus( COleDateTime::invalid );
   m_rectSizing.SetRect( -1, -1, -1, -1 );
   m_bESC = FALSE;
// SetWindowPos( this, // &CWnd::wndBottom,
//               0, 0, 0, 0,
//               SWP_HIDEWINDOW | /*SWP_NOZORDER |*/ SWP_NOOWNERZORDER );

   CEdit::OnKillFocus( pWndGetFocus );
}

void
ZEditAppointment::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
   TraceLineS( "ZEditAppointment::OnKeyDown", "========================" );
   if ( uKey == VK_ESCAPE )
   {
      m_bESC = TRUE;
      m_pAgenda->SetFocus( );
   }
   else
   {
      CEdit::OnKeyDown( uKey, uRepeatCnt, uFlags );

      // Resize edit control if needed.
      CalculateSize( );
   }
}

void
ZEditAppointment::OnChar( UINT uChar, UINT uRepeatCnt, UINT uFlags )
{
   TraceLineS( "ZEditAppointment::OnChar", "========================" );
   if ( uChar == VK_ESCAPE || uChar == VK_RETURN )
   {
      if ( uChar == VK_ESCAPE )
         m_bESC = TRUE;

      m_pAgenda->SetFocus( );
   }
   else
   {
      CEdit::OnChar( uChar, uRepeatCnt, uFlags );

      // Resize edit control if needed.
      CalculateSize( );
   }
}

int
ZEditAppointment::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CEdit::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   m_OldWndProc = (WNDPROC) ::SetWindowLong( m_hWnd, GWL_WNDPROC,
                                             (DWORD) EditAppointmentWndProc );
   ::SetWindowLong( m_hWnd, GWL_USERDATA, (DWORD) this );

   // Set the proper font.
   SetFont( &(m_pAgenda->m_fontAppointmentSubject) );

// SetFocus( );
   CalculateSize( );
   SetSel( 0, -1 );
   return( nRC );
}

void
ZEditAppointment::OnMove( int x, int y )
{
// TracePoint( "ZEditAppointment::OnMove: ", CPoint( x, y ) );
   if ( m_pA )
   {
      if ( m_timeFrom.GetStatus( ) == COleDateTime::valid &&
           m_timeTo.GetStatus( ) == COleDateTime::valid )
      {
         m_pA->m_timeFrom = m_timeFrom;
         m_pA->m_timeTo = m_timeTo;
      }
   }

   if ( m_bSizeMove == FALSE )
      CEdit::OnMove( x, y );
}

void
ZEditAppointment::OnSize( UINT uType, int cx, int cy )
{
// TraceSize( "ZEditAppointment::OnSize: ", CSize( cx, cy ) );
   if ( m_pA )
   {
      if ( m_timeFrom.GetStatus( ) == COleDateTime::valid &&
           m_timeTo.GetStatus( ) == COleDateTime::valid )
      {
         ZAppointment *pA = m_pA;
         m_pA = 0;
         pA->m_timeFrom = m_timeFrom;
         pA->m_timeTo = m_timeTo;

         Invalidate( );
         ShowWindow( SW_HIDE );
         m_pAgenda->m_wndToolTip.ShowWindow( SW_HIDE );
         m_rectSizing.SetRect( -1, -1, -1, -1 );
         m_pAgenda->SetFocus( );

         // Now reposition all appointments.
         m_pAgenda->SortAppointments( );
         m_pAgenda->SetAppointmentOverlap( );
         m_pAgenda->GoToTime( pA->m_timeFrom, FALSE );

         return;
      }
   }

   if ( m_bSizeMove == FALSE )
      CEdit::OnSize( uType, cx, cy );
}

void
ZEditAppointment::OnSizing( UINT uSide, LPRECT pRect )
{
   // 1-left; 2-right; 3-top; 6-bottom
   // 4-top/left; 5-top/right; 7-bottom/left; 8-bottom/right
   if ( m_pA )
   {
      if ( m_rectSizing.left == -1 && m_rectSizing.top == -1 &&
           m_rectSizing.left == -1 && m_rectSizing.top == -1 )
      {
         m_rectSizing = *pRect;
      }

      int nWidth = (pRect->right) - (pRect->left);
      int nWidthInit = m_rectSizing.Width( );
      if ( nWidthInit != nWidth )
      {
         // Because of the NcHitTest handler implementation, this
         // code should never take place.
         // Disallow sizing in the x direction.
         ASSERT( uSide != 3 && uSide != 6 );
         if ( uSide == 1 || uSide == 4 || uSide == 7 )
            pRect->left = pRect->right - nWidthInit;
         else
      // if ( uSide == 2 || uSide == 5 || uSide == 8 )
            pRect->right = pRect->left + nWidthInit;
      }

      int nHeight = (pRect->bottom) - (pRect->top);
      int nHeightInit = m_rectSizing.Height( );

      // Force appointment size to be "integral 5-minute height"
      int nFullHeight = m_pAgenda->m_nHourHeight;
      int nFiveMinuteHeight = nFullHeight / 12;
      int nDiff = nHeight - nHeightInit;
      int nNegative;

      if ( nDiff < 0 )
      {
         nDiff = -nDiff;
         nNegative = -1;
      }
      else
      {
         nNegative = 1;
      }

      if ( nDiff % nFiveMinuteHeight )
      {
         ASSERT( uSide != 1 && uSide != 5 );
         nDiff /= nFiveMinuteHeight;
         nDiff *= nFiveMinuteHeight;
         if ( uSide == 3 || uSide == 4 || uSide == 5 ) // sizing up
         {
            pRect->top = pRect->bottom - (nHeightInit + nDiff * nNegative);
         }
         else
      // if ( uSide == 6 || uSide == 7 || uSide == 8 )
         {
            pRect->bottom = pRect->top + (nHeightInit + nDiff * nNegative);
         }
      }
      else
      {
         nDiff /= nFiveMinuteHeight;
         nDiff *= nFiveMinuteHeight;
      }

      // Show tip in correct position.
      // Appointment position will change in "integral 5-minute"
      nDiff /= nFiveMinuteHeight;
      COleDateTimeSpan ts( 0, 0, 5 * nDiff, 0 );
      COleDateTime timeFrom( m_pA->m_timeFrom );
      COleDateTime timeTo( m_pA->m_timeTo );

      if ( uSide == 3 || uSide == 4 || uSide == 5 ) // sizing up
      {
         if ( nNegative == -1 )
            timeFrom += ts;
         else
            timeFrom -= ts;
      }
      else
   // if ( uSide == 6 || uSide == 7 || uSide == 8 )
      {
         if ( nNegative == -1 )
            timeTo -= ts;
         else
            timeTo += ts;
      }

      if ( (timeFrom != m_pA->m_timeFrom || timeTo != m_pA->m_timeTo) &&
           timeFrom.GetDay( ) == m_pA->m_timeFrom.GetDay( ) &&
           timeTo.GetDay( ) == m_pA->m_timeTo.GetDay( ) )
      {
         CString cs = FormatTimeAmPm( timeFrom );
         cs += " - ";
         cs += FormatTimeAmPm( timeTo );

         CPoint ptScreen;
         ptScreen.x = m_pA->m_rectDraw.left + g_nGutterBandWidth;
         if ( uSide == 3 || uSide == 4 || uSide == 5 ) // sizing up
            ptScreen.y = m_pA->m_rectDraw.bottom;
         else
            ptScreen.y = m_pA->m_rectDraw.top;

         m_pAgenda->ClientToScreen( &ptScreen );
         m_pAgenda->m_wndToolTip.ShowTips( ptScreen, cs );
         m_timeFrom = timeFrom;
         m_timeTo = timeTo;
      }
      else
      {
         m_pAgenda->m_wndToolTip.ShowWindow( SW_HIDE );
         if ( timeFrom == m_pA->m_timeFrom && timeTo == m_pA->m_timeTo)
         {
            m_timeFrom = timeFrom;
            m_timeTo = timeTo;
         }
      }
   }

   if ( m_bSizeMove == FALSE )
      CEdit::OnSizing( uSide, pRect );
}

void
ZEditAppointment::CalculateSize( )
{
   // Get text extent.
   CString cs;

   GetWindowText( cs );
   CWindowDC dc( this );
   CFont *pFont = GetParent( )->GetFont( );
   CFont *pFontDC = dc.SelectObject( pFont );
   CSize size;

   // Get client rect.
   CRect rectClient;
   CRect rectWindow;
   CRect rectParent;
   GetClientRect( &rectClient );
   GetWindowRect( &rectWindow );
   GetParent( )->GetClientRect( &rectParent );

   // Transform rect to parent coordinates.
   ClientToScreen( &rectClient );
// TraceRect( "ZEditAppointment::CalculateSize CLient: ", rectClient );
// TraceRect( "ZEditAppointment::CalculateSize Window: ", rectWindow );
   int nDiffX = rectClient.left - rectWindow.left;
   int nDiffY = rectClient.top - rectWindow.top;
   GetParent( )->ScreenToClient( &rectClient );

   if ( (GetStyle( ) & ES_MULTILINE) == 0 )
   {
      size = dc.GetTextExtent( cs );
      dc.SelectObject( pFontDC );
      size.cx += 5;            // add some extra buffer
   }
   else
   {
      CRect rectThin( rectClient );  // measure the skinniest text box
      CRect rectWide( rectClient );  // measure the widest text box
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

#if 0 // don't let it grow in the x-direction
   if ( size.cx > rectClient.Width( ) )
   {
      if ( size.cx + rectClient.left < rectParent.right - 2 )
         rectClient.right = rectClient.left + size.cx;
      else
         rectClient.right = rectParent.right - 2;

      bChanged = TRUE;
   }
#endif

   if ( size.cy > rectClient.Height( ) )
   {
      if ( size.cy + rectClient.top < rectParent.bottom - 2 )
         rectClient.bottom = rectClient.top + size.cy;
      else
      {
         rectClient.bottom = rectParent.bottom - 2;
         ShowScrollBar( SB_VERT );
      }

      bChanged = TRUE;
   }

   // If the size became larger reposition the window.
   if ( bChanged )
   {
      rectClient.InflateRect( nDiffX, nDiffY, nDiffX, nDiffY );
   // TraceRect( "ZEditAppointment::CalculateSize Changed: ", rectClient );
      MoveWindow( &rectClient );
   }
}

LRESULT
ZEditAppointment::OnNcHitTest( CPoint pt )
{
   UINT uRC = CEdit::OnNcHitTest( pt );
   if ( uRC == HTLEFT || uRC == HTRIGHT )
      uRC = HTBORDER;
   else
   if ( uRC == HTTOPLEFT || uRC == HTTOPRIGHT )
      uRC = HTTOP;
   else
   if ( uRC == HTBOTTOMLEFT || uRC == HTBOTTOMRIGHT )
      uRC = HTBOTTOM;

   return( uRC );
}

void
ZEditAppointment::OnNcPaint( )
{
   // Get window DC that is clipped to the non-client area
   CWindowDC dc( GetParent( ) ); // the HDC will be released by the destructor

   CRect rectClient;
   CRect rectBorder;
   GetClientRect( rectClient );
   ClientToScreen( rectClient );
   GetWindowRect( rectBorder );

   rectBorder.top -= rectClient.top - rectBorder.top;
   rectBorder.bottom -= rectClient.bottom - rectBorder.bottom;

   // Exclude client area.
   m_pAgenda->ScreenToClient( rectBorder );
   m_pAgenda->ScreenToClient( rectClient );
   dc.IntersectClipRect( rectBorder );
   dc.ExcludeClipRect( rectClient );
   rectBorder.left -= 2;  // g_nGutterBandWidth;
   rectBorder.right -= 1; // g_nGutterBandWidth;

   // Paint the border.
   CBrush brushBorder( RGB( 0, 0, 255 ) );
   CBrush *pBrushOld = dc.SelectObject( &brushBorder );
   dc.Rectangle( &rectBorder );
   dc.SelectObject( pBrushOld );
}

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZAppointment, CObject )

ZAppointment::ZAppointment( int nSeqNbr, unsigned int uKey )
{
   m_nSeqNbr = nSeqNbr;
   m_uKey = uKey;
   m_timeTo.SetDate( 100, 0, 0 );
   m_timeFrom.SetDate( 100, 0, 0 );
   m_bFullDay = FALSE;
   m_nOverlap = 0;
   m_nOverlapMax = 0;
   m_pPrev = m_pNext = 0;
   m_bSelected = FALSE;
   m_nAlarm = 0;
   m_nAppointmentType = 0;
}

ZAppointment::~ZAppointment( )
{
   mDeleteInit( m_pNext );
}

void
ZAppointment::DrawAppointment( COleDateTime& date,
                               CDC *pDC, const CRect& rectArea,
                               ZAgenda *pAgenda, int nScrollPos )
{
   ASSERT_VALID( pAgenda );
   ASSERT_VALID( pDC );

   if ( m_pNext && m_pNext->m_date == date )
      m_pNext->DrawAppointment( date, pDC, rectArea, pAgenda, nScrollPos );

   if ( m_bFullDay )
      return;

   CRect rect;
   pAgenda->SetDrawRect( rect, m_timeFrom, m_timeTo, rectArea, nScrollPos );

   if ( m_nOverlapMax > 0 )
   {
      int nWidth = rect.Width( ) / m_nOverlapMax;
      rect.left += nWidth * (m_nOverlapPos - 1);
      rect.right = rect.left + nWidth - 4; // 4-pixel space between appointments
   }

   // Outline the appointment.
   CRect rectBox( rect );
   CPen *pPenOld = pDC->SelectObject( &(pAgenda->m_penDivider) );
   pDC->MoveTo( rectBox.left + 1, rectBox.bottom );
   pDC->LineTo( rectBox.right, rectBox.bottom );
   pDC->LineTo( rectBox.right, rectBox.top + 1 );
   pDC->SelectObject( pPenOld );

   // Paint the background.
   CBrush *pBrOld = pDC->SelectObject( &(pAgenda->m_brushAppointmentBack) );
   CFont *pFontOld = pDC->SelectObject( &(pAgenda->m_fontAppointmentSubject) );
   pDC->Rectangle( &rectBox );

   // Set draw rect.
   m_rectDraw = rect;
   // Draw text.
   CString cs = FormatTimeAmPm( m_timeFrom );
   cs += "-";
   cs += FormatTimeAmPm( m_timeTo );
   cs += "  ";
   cs+= m_csSubject;

   CRect rectTxt( rect );
   rectTxt.left += g_nGutterBandWidth;
   rectTxt.DeflateRect( 2, 2, 2, 2 );

   if ( m_nAlarm )
   {
      CPoint ptImage( rectTxt.left + 1, rectTxt.top + 1 );
      pAgenda->m_Images.Draw( pDC, 0, ptImage, ILD_NORMAL );
      rectTxt.left += pAgenda->m_ImageSize.cx + 4;
   }

   pDC->DrawText( cs, &rectTxt,
                  DT_LEFT | DT_WORDBREAK | DT_EXPANDTABS |
                    DT_END_ELLIPSIS | DT_NOPREFIX );

   pDC->SelectObject( pBrOld );
   pDC->SelectObject( pFontOld );

   // Draw the appointment left margin.
   CRect rectHandle( rect );
   rectHandle.right = rectHandle.left + g_nGutterBandWidth + 1;

   CBrush brHandle( RGB( 0, 0, 255 ) );
   pBrOld = pDC->SelectObject( &brHandle );
   pPenOld = pDC->SelectObject( &(pAgenda->m_penThinBlack) );
   pDC->Rectangle( &rectHandle ); // rectangle
   pDC->SelectObject( pBrOld );
   pDC->SelectObject( pPenOld );
}

BOOL
ZAppointment::DrawFullDay( COleDateTime& date, CDC *pDC,
                           const CRect& rectArea,
                           ZAgenda *pAgenda, BOOL bSelected,
                           int nSelectedDay )
{
   ASSERT_VALID( pAgenda );
   ASSERT_VALID( pDC );

   if ( m_date != date )
      return( FALSE );

   CRect rectTxt( rectArea );
   m_rectDraw = rectTxt;

   if ( bSelected )
      pDC->FillSolidRect( &rectTxt, ::GetSysColor( COLOR_3DHIGHLIGHT ) );

   pDC->DrawEdge( &rectTxt, EDGE_ETCHED, BF_RECT );

   // Draw text.
   rectTxt.left += g_nGutterBandWidth;

   if ( m_nAlarm )
   {
      CPoint ptImage( rectTxt.left + 1, rectTxt.top + 2 );
      pAgenda->m_Images.Draw( pDC, 0, ptImage, ILD_NORMAL );
      rectTxt.left += pAgenda->m_ImageSize.cx + 4;
   }

   rectTxt.DeflateRect( 3, 3, 3, 3 );
   CFont *pFontOld = pDC->SelectObject( &(pAgenda->m_fontAppointmentSubject) );
   int nBkMode = pDC->SetBkMode( TRANSPARENT );
   pDC->DrawText( bSelected ? m_csMessage : m_csSubject, &rectTxt,
                  DT_LEFT | DT_EXPANDTABS | DT_END_ELLIPSIS | DT_NOPREFIX );
   pDC->SetBkMode( nBkMode );
   pDC->SelectObject( pFontOld );
   return( TRUE );
}

void
ZAppointment::UnlinkAppointment( ZAgenda *pAgenda )
{
   // Remove this appointment from the linklist.
   if ( m_pPrev )
      m_pPrev->m_pNext = m_pNext;

   if ( m_pNext )
      m_pNext->m_pPrev = m_pPrev;

   if ( this == pAgenda->m_pAppointmentHead )
   {
      pAgenda->m_pAppointmentHead = m_pNext;
      pAgenda->m_pAppointmentHead->m_pPrev = 0;
   }

   m_pNext = 0;
   m_pPrev = 0;
}

/////////////////////////////////////////////////////////////////////////////
// ZAutoFitLabel

CFont *
GetLargestFont( zCPCHAR cpcFaceName, zBOOL bBold, zBOOL bItalic,
                zCPCHAR cpcText, int nPixelWidth, int nPixelHeight, CDC *pDC )
{
   int nFontHeight = 0;
   int nTextWidth = 0;
   int nPoint = 10;
   CFont *pFont = 0;

   while ( 1 )
   {
      // Delete pFont;
      CFont *pTmp = new CFont;
      VERIFY( pTmp->CreateFont( nPoint, 0, 0, 0,
                                (bBold ? FW_BOLD : FW_NORMAL), bItalic,
                                0, 0, 0, 0, 0, 0, 0, cpcFaceName ) );

      CFont *pOldFont = pDC->SelectObject( pTmp );
      CSize size = pDC->GetTextExtent( cpcText );
      nFontHeight = size.cy;
      nTextWidth = size.cx;
      pDC->SelectObject( pOldFont );

      nPoint += 5;

      if ( nFontHeight >= nPixelHeight || nTextWidth >= nPixelWidth )
      {

         if ( pFont == 0 )
            pFont = pTmp;
         else
            delete( pTmp );

         break;
      }

      mDeleteInit( pFont );
      pFont = pTmp;
   }

   // Best fit font.
   return( pFont );
}

// dks???
BEGIN_MESSAGE_MAP( ZAutoFitLabel, CStatic )
   //{{AFX_MSG_MAP( ZAutoFitLabel )
   ON_WM_SIZE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZAutoFitLabel::ZAutoFitLabel( )
{
   m_pFont = 0;
}

ZAutoFitLabel::~ZAutoFitLabel( )
{
   mDeleteInit( m_pFont );
}

/////////////////////////////////////////////////////////////////////////////
// ZAutoFitLabel message handlers
void
ZAutoFitLabel::OnSize( UINT uType, int cx, int cy )
{
   CStatic::OnSize( uType, cx, cy );

   CString cs;
   GetWindowText( cs );
   if ( cs.GetLength( ) )
   {
      INTFRAC( cx, 0.9 );
      INTFRAC( cy, 0.9 );
      mDeleteInit( m_pFont );
      m_pFont = GetLargestFont( "Tahoma", FALSE, FALSE, cs, cx, cy, GetDC( ) );
      ASSERT_VALID( m_pFont );
      SetFont( m_pFont );
   }
}

void
ZAutoFitLabel::PreSubclassWindow( )
{
   Update( );
   CStatic::PreSubclassWindow( );
}

void
ZAutoFitLabel::Update( )
{
   CRect rect;
   GetClientRect( &rect );
   PostMessage( WM_SIZE, 0, MAKELONG( rect.Width( ), rect.Height( ) ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZDayMonthYear

BEGIN_MESSAGE_MAP( ZDayMonthYear, CWnd )
   //{{AFX_MSG_MAP(ZDayMonthYear)
   ON_WM_CREATE( )
   ON_WM_PAINT( )
   ON_WM_DESTROY( )
   ON_WM_ERASEBKGND( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_CONTEXTMENU( )
   ON_WM_SETFOCUS( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZDayMonthYear::ZDayMonthYear( )
{
   m_pAgenda = 0;
}

ZDayMonthYear::~ZDayMonthYear( )
{
   if ( m_pAgenda )
      m_pAgenda->m_pDayMonthYear = 0;
}

int
ZDayMonthYear::OnCreate( LPCREATESTRUCT lpcs )
{
   lpcs->style &= ~WS_BORDER;  // don't need a border
   int nRC = CWnd::OnCreate( lpcs );
   if ( nRC == -1 )
      return( -1 );

   // TODO: Add your specialized creation code here

   return( nRC );
}

BOOL
ZDayMonthYear::OnEraseBkgnd( CDC *pDC )
{
   return( FALSE );
}

void
ZDayMonthYear::OnPaint( )
{
   CPaintDC     dcPaint( this ); // device context for painting
   COleDateTime date;
   CString      csDate;
   CRect        rectClient;
   CRect        rectDate;
   int          k;

   GetClientRect( &rectClient );

   // If possible, we paint to an off-screen DC and BitBlt the results to
   // the screen (reduces flicker).
   CDC      *pDC;
   zBOOL    bMemDC;
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
   else
   {
      bMemDC = FALSE;
      pDC = &dcPaint;
   }

   rectDate = rectClient;
   rectDate.bottom = rectDate.top + g_nFullDayAppointmentHeight;
// pDC->FillSolidRect( &rectClient, ::GetSysColor( COLOR_3DSHADOW ) );
// pDC->FillSolidRect( &rectDate, ::GetSysColor( COLOR_BTNFACE ) );
   pDC->FillSolidRect( &rectClient, ::GetSysColor( COLOR_BTNFACE ) );

   CRect rect = rectDate;
// rect.right -= GetSystemMetrics( SM_CXVSCROLL );
   int nDayCnt = m_pAgenda->GetDayCount( );
   int nSizeX = (m_pAgenda->m_nClientX - m_pAgenda->m_nTimeGutterWidth) /
                                              m_pAgenda->GetShownDayCount( );
// TraceRect( "ZDayMonthYear::OnPaint Client: ", rect );
// TraceLineI( "ZDayMonthYear =====> SubWidth: ", nSizeX );

   rect.right = rect.left + nSizeX;
   for ( k = 0; k < nDayCnt; k++ )
   {
      if ( k == nDayCnt - 1 )
         rect.right = rectDate.right;

      if ( m_pAgenda->m_pDayTimer->m_nMaxShowAppointment == 0 ||
           k >= nDayCnt - m_pAgenda->m_pDayTimer->m_nMaxShowAppointment )
      {
         pDC->DrawEdge( &rect, EDGE_ETCHED, BF_RECT );

         if ( m_pAgenda && m_pAgenda->m_pDayTimer &&
              m_pAgenda->m_pDayTimer->m_pMiniCal )
         {
            m_pAgenda->m_pDayTimer->m_pMiniCal->GetSelectedDateByOrder( date, k );
         }
         else
         {
            date = COleDateTime::GetCurrentTime( );
         }

         csDate = date.Format( "%A, %B %d" );
         if ( pDC->GetTextExtent( csDate ).cx > rect.Width( ) )
            csDate = date.Format( "%a, %B %d" );

         if ( pDC->GetTextExtent( csDate ).cx > rect.Width( ) )
            csDate = date.Format( "%a %d" );

         if ( pDC->GetTextExtent( csDate ).cx > rect.Width( ) )
            csDate = date.Format( "%m/%d" );

         CFont *pFontOld = pDC->SelectObject( &(m_pAgenda->m_fontAppointmentSubject) );
         int nBkMode = pDC->SetBkMode( TRANSPARENT );
         pDC->DrawText( csDate, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
         pDC->SetBkMode( nBkMode );
         pDC->SelectObject( pFontOld );

         if ( m_pAgenda->m_dateLastSelected.GetStatus( ) == COleDateTime::valid &&
              date == m_pAgenda->m_dateLastSelected )
         {
         // CWnd *pWnd = GetFocus( );
         // if ( pWnd == this ||
         //      (m_pAgenda && pWnd == m_pAgenda) ||
         //      (m_pAgenda->m_pDayTimer && pWnd == m_pAgenda->m_pDayTimer) ||
         //      (m_pAgenda->m_pDayTimer->m_pMiniCal &&
         //       pWnd == m_pAgenda->m_pDayTimer->m_pMiniCal) )
            if ( m_pAgenda && m_pAgenda->m_bFocusRect )
            {
               CSize size = pDC->GetTextExtent( csDate );
               int nOffsetX;
               int nOffsetY;
               size.cx -= 2;
               size.cy -= 2;

               if ( rect.Width( ) > size.cx )
                  nOffsetX = (rect.Width( ) - size.cx) / 2;
               else
                  nOffsetX = 0;

               if ( rect.Height( ) > size.cy )
                  nOffsetY = (rect.Height( ) - size.cy) / 2;
               else
                  nOffsetY = 0;

               CRect rectExtent( rect.left + nOffsetX, rect.top + nOffsetY,
                                 rect.left + nOffsetX + size.cx,
                                 rect.top + nOffsetY + size.cy );
               pDC->DrawFocusRect( rectExtent );
            // TraceRect( "DrawFocusRect: ", rectExtent );
            }
         }

         rect.left += nSizeX;
         rect.right += nSizeX;
      }
   }

   if ( bMemDC )
   {
      // Copy the results to the on-screen DC.
      CRect rectClip;
      dcPaint.GetClipBox( rectClip );
      dcPaint.BitBlt( rectClip.left, rectClip.top,
                      rectClip.Width( ), rectClip.Height( ),
                      &memDC, rectClip.left, rectClip.top, SRCCOPY );

      memDC.SelectObject( pOldBmp );
   }
}

void
ZDayMonthYear::OnDestroy( )
{
   CWnd::OnDestroy( );
}

void
ZDayMonthYear::OnLButtonDown( UINT uFlags, CPoint pt )
{
   SetFocus( );
   CWnd::OnLButtonDown( uFlags, pt );
}

void
ZDayMonthYear::OnLButtonUp( UINT uFlags, CPoint pt )
{
   ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                          zDAYTIMER_SELECT_DAYMONTHYEAR,
                          (zPVOID) MAKELONG( pt.x, pt.y ) );
   CWnd::OnLButtonUp( uFlags, pt );
}

void
ZDayMonthYear::OnLButtonDblClk( UINT uFlags, CPoint pt )
{
   ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                          zDAYTIMER_DBLCLK_DAYMONTHYEAR,
                          (zPVOID) MAKELONG( pt.x, pt.y ) );
   CWnd::OnLButtonDblClk( uFlags, pt );
}

void
ZDayMonthYear::OnRButtonDown( UINT uFlags, CPoint pt )
{
   ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                          zDAYTIMER_RCLICK_DAYMONTHYEAR,
                          (zPVOID) MAKELONG( pt.x, pt.y ) );
   CWnd::OnRButtonDown( uFlags, pt );
}

void
ZDayMonthYear::OnRButtonUp( UINT uFlags, CPoint pt )
{
   CWnd::OnRButtonUp( uFlags, pt );
}

void
ZDayMonthYear::OnContextMenu( CWnd *pWnd, CPoint pt )
{
}

void
ZDayMonthYear::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pAgenda )
      m_pAgenda->SetFocus( );
}

/////////////////////////////////////////////////////////////////////////////
// ZFullDayWnd

BEGIN_MESSAGE_MAP( ZFullDayWnd, CWnd )
   //{{AFX_MSG_MAP(ZFullDayWnd)
   ON_WM_CREATE( )
   ON_WM_PAINT( )
   ON_WM_DESTROY( )
   ON_WM_ERASEBKGND( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
// ON_WM_MOUSEMOVE( )
   ON_WM_CONTEXTMENU( )
   ON_WM_SETFOCUS( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZFullDayWnd::ZFullDayWnd( )
{
   m_pAgenda = 0;
   m_pHilightAppointment = 0;
}

ZFullDayWnd::~ZFullDayWnd( )
{
   if ( m_pAgenda )
      m_pAgenda->m_pFullDayWnd = 0;
}

int
ZFullDayWnd::OnCreate( LPCREATESTRUCT lpcs )
{
   lpcs->style &= ~WS_BORDER;  // don't need a border
   int nRC = CWnd::OnCreate( lpcs );
   if ( nRC == -1 )
      return( -1 );

   // TODO: Add your specialized creation code here

   return( nRC );
}

BOOL
ZFullDayWnd::OnEraseBkgnd( CDC *pDC )
{
   return( FALSE );
}

void
ZFullDayWnd::OnPaint( )
{
   CPaintDC dcPaint( this ); // device context for painting
// ZMemDC   memDC( &dcPaint );
   CRect rectDay;
   CRect rectClient;
   int   nFullDayAppointments = m_pAgenda->GetNumberOfFulldayAppointments( );
   int   k;

   GetClientRect( &rectClient );

   // If possible, we paint to an off-screen DC and BitBlt the results to
   // the screen (reduces flicker).
   CDC      *pDC;
   zBOOL    bMemDC;
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
   else
   {
      bMemDC = FALSE;
      pDC = &dcPaint;
   }

   pDC->FillSolidRect( &rectClient, ::GetSysColor( COLOR_BTNFACE ) );

   COleDateTime date;
   ZAppointment *pA;
   CRect rect = rectClient;
// rect.right -= GetSystemMetrics( SM_CXVSCROLL );
   int nDayCnt = m_pAgenda->GetDayCount( );
   int nSizeX = (m_pAgenda->m_nClientX - m_pAgenda->m_nTimeGutterWidth) /
                                              m_pAgenda->GetShownDayCount( );
// TraceRect( "ZFullDayWnd::OnPaint Client: ", rect );
// TraceLineI( "ZFullDayWnd =====> SubWidth: ", nSizeX );

   rect.right = rect.left + nSizeX;
   for ( k = 0; k < nDayCnt; k++ )
   {
      if ( k == nDayCnt - 1 )
         rect.right = rectClient.right;

      if ( m_pAgenda->m_pDayTimer->m_nMaxShowAppointment == 0 ||
           k >= nDayCnt - m_pAgenda->m_pDayTimer->m_nMaxShowAppointment )
      {
         int nIdx = 0;
         int nPosY = 0;

         m_pAgenda->m_pDayTimer->m_pMiniCal->GetSelectedDateByOrder( date, k );
         for ( pA = m_pAgenda->m_pAppointmentHead; pA; pA = pA->m_pNext )
         {
            if ( pA->m_bFullDay )
            {
               rectDay.SetRect( rect.left, nPosY, rect.right,
                                nPosY + g_nFullDayAppointmentHeight );
               if ( pA->DrawFullDay( date, &memDC, &rectDay, m_pAgenda,
                                     pA == m_pHilightAppointment, k ) )
               {
                  nIdx++;
                  nPosY = (nIdx * g_nFullDayAppointmentHeight);
               }
            }
         }

         while ( nIdx < nFullDayAppointments )
         {
            rectDay.SetRect( rect.left, nPosY, rect.right,
                             nPosY + g_nFullDayAppointmentHeight );
            pDC->DrawEdge( &rectDay, EDGE_ETCHED, BF_RECT );
            nIdx++;
            nPosY = (nIdx * g_nFullDayAppointmentHeight);
         }

         rect.left += nSizeX;
         rect.right += nSizeX;
      }
   }

   if ( bMemDC )
   {
      // Copy the results to the on-screen DC.
      CRect rectClip;
      dcPaint.GetClipBox( rectClip );
      dcPaint.BitBlt( rectClip.left, rectClip.top,
                      rectClip.Width( ), rectClip.Height( ),
                      &memDC, rectClip.left, rectClip.top, SRCCOPY );

      memDC.SelectObject( pOldBmp );
   }
}

void
ZFullDayWnd::OnDestroy( )
{
   CWnd::OnDestroy( );
}

ZAppointment *
ZFullDayWnd::AppointmentHitTest( CPoint& pt )
{
   COleDateTime date;

   CPoint ptFullDay( pt );
   ClientToScreen( &ptFullDay );
   m_pAgenda->ScreenToClient( &ptFullDay );
   if ( m_pAgenda->GetDateFromPoint( date, ptFullDay ) != 2 )
      date = m_pAgenda->m_pDayTimer->m_pMiniCal->m_dateToday;

   ZAppointment *pA;
   for ( pA = m_pAgenda->m_pAppointmentHead; pA; pA = pA->m_pNext )
   {
      if ( date == pA->m_date && pA->m_bFullDay &&
           pA->m_rectDraw.PtInRect( pt ) )
      {
         return( pA );
      }
   }

   return( 0 );
}

BOOL
ZFullDayWnd::IsDoubleClick( UINT uMessage, CPoint ptStart )
{
   MSG    msg;
   UINT   uInterval = 0;
   LONG   lMessage = GetMessageTime( );
   UINT   uDblclkTime = GetDoubleClickTime( );

   while ( uInterval < uDblclkTime )
   {
      uInterval = (zULONG) (GetCurrentTime( ) - lMessage);
      if ( ::PeekMessage( &msg, m_hWnd, uMessage, uMessage,
                          PM_NOREMOVE | PM_NOYIELD ) )
      {
         return( TRUE );
      }
      else
      if ( ::PeekMessage( &msg, m_hWnd, WM_MOUSEMOVE, WM_MOUSEMOVE,
                          PM_REMOVE | PM_NOYIELD ) )
      {
         CPoint pt( msg.lParam );
         if ( ptStart.x - pt.x > g_lDblClkX ||
              pt.x - ptStart.x > g_lDblClkX ||
              ptStart.y - pt.y > g_lDblClkY ||
              pt.y - ptStart.y > g_lDblClkY )
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            return( FALSE );
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

void
ZFullDayWnd::OnLButtonDown( UINT uFlags, CPoint pt )
{
// TracePoint( "ZFullDayWnd::OnLButtonDown", pt );
   if ( (m_pAgenda->m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      ZAppointment *pA = AppointmentHitTest( pt );
      if ( pA )
      {
         m_pHilightAppointment = pA;
         Invalidate( );
         if ( IsDoubleClick( WM_LBUTTONDBLCLK, pt ) == FALSE )
         {
            m_pAgenda->m_ptTracked = pt;
            ClientToScreen( &(m_pAgenda->m_ptTracked) );
            m_pAgenda->ScreenToClient( &(m_pAgenda->m_ptTracked) );
            m_pAgenda->TrackEditAppointment( pA, pt );
         }
      }
      else
         SetFocus( );
   }

   CWnd::OnLButtonDown( uFlags, pt );
}

void
ZFullDayWnd::OnLButtonUp( UINT uFlags, CPoint pt )
{
// TracePoint( "ZFullDayWnd::OnLButtonUp", pt );
   if ( (m_pAgenda->m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      ZAppointment *pA = AppointmentHitTest( pt );
      if ( pA == m_pHilightAppointment )
      {
         ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                                zDAYTIMER_SELECT_FULLDAY,
                                (zPVOID) MAKELONG( pt.x, pt.y ) );
      }
      else
      {
         ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                                zDAYTIMER_SELECT_FULLDAY_WND,
                                (zPVOID) MAKELONG( pt.x, pt.y ) );
      }

      if ( m_pHilightAppointment )
      {
         m_pHilightAppointment = 0;
         Invalidate( );
      }
   }

   CWnd::OnLButtonUp( uFlags, pt );
}

void
ZFullDayWnd::OnLButtonDblClk( UINT uFlags, CPoint pt )
{
// TracePoint( "ZFullDayWnd::OnLButtonDblClk", pt );
   if ( (m_pAgenda->m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      ZAppointment *pA = AppointmentHitTest( pt );

      if ( pA && m_pHilightAppointment == pA )
      {
         ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                                zDAYTIMER_DBLCLK_FULLDAY, pA );
#if 0
         // dks??? won't do this when things are tested
         pA->UnlinkAppointment( m_pAgenda );
         delete( pA );
         m_pAgenda->UpdatePositionOfControls( );
      // m_pCalView->OnLButtonDblClk( uFlags, pt );
         Invalidate( );
#endif
      }
      else
         ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                                zDAYTIMER_DBLCLK_FULLDAY_WND,
                                (zPVOID) MAKELONG( pt.x, pt.y ) );
   }

   CWnd::OnLButtonDblClk( uFlags, pt );
}

void
ZFullDayWnd::OnRButtonDown( UINT uFlags, CPoint pt )
{
   if ( (m_pAgenda->m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      ZAppointment *pA = AppointmentHitTest( pt );
      if ( pA )
         ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                                zDAYTIMER_RCLICK_FULLDAY, pA );
      else
         ProcessImmediateEvent( m_pAgenda->m_pDayTimer,
                                zDAYTIMER_RCLICK_FULLDAY_WND,
                                (zPVOID) MAKELONG( pt.x, pt.y ) );
   }
}

void  // dks???
ZFullDayWnd::OnContextMenu( CWnd *pWnd, CPoint pt )
{
   ZAppointment *pA = AppointmentHitTest( pt );

   if ( pA )
   {
      m_pHilightAppointment = pA;
      Invalidate( );

   // m_pCalView->OnContextMenu( pWnd, pt );
   }
}

void
ZFullDayWnd::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pAgenda )
      m_pAgenda->SetFocus( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ZMiniCalendar
// Author:  Matt Gullett
//       gullettm@yahoo.com
// Copyright © 2001
//
// This is a user-interface componenet similar to the MS outlook mini-
// calendar control.  (The one used in date picker control and the
// appointment (day view)).
//
// You may freely use this source code in personal, freeware, shareware or
// commercial applications provided that 1) my name is recognized in the code
// and if this code represents a substantial portion of the application that
// my name be included in the credits for the application (about box, etc).
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Keith Rule for the CMemDC class
// http://www.codeproject.com/gdi/flickerfree.asp
//
/////////////////////////////////////////////////////////////////////////////

#define FMC_HEADER_TIMER_ID            1001
#define FMC_HEADER_TIMER_INTERVAL      30

#include <math.h>
#include <locale.h>

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarCtrlFontInfo

ZMiniCalendarCtrlFontInfo::ZMiniCalendarCtrlFontInfo( )
{
   m_pFont = 0;
}

ZMiniCalendarCtrlFontInfo::~ZMiniCalendarCtrlFontInfo( )
{
   mDeleteInit( m_pFont );
}

// Create the CFont GDI objects.
void
ZMiniCalendarCtrlFontInfo::CreateFont( CDC *pDC )
{
   ASSERT( pDC );

   if ( pDC )
   {
      LOGFONT FontInfo;
      memset( &FontInfo, 0, sizeof( LOGFONT ) );
      mDeleteInit( m_pFont );

      m_pFont = new CFont;
      ASSERT( m_pFont );
      if ( m_pFont == 0 )
         throw( new CMemoryException( ) );

      m_pFont->CreatePointFont( m_nFontSize * 10, m_csFontName );
      m_pFont->GetLogFont( &FontInfo );

      FontInfo.lfHeight = -MulDiv( m_nFontSize,
                                   GetDeviceCaps( pDC->GetSafeHdc( ),
                                                  LOGPIXELSY ),
                                   72 );
      FontInfo.lfWeight = 100;
      FontInfo.lfQuality = PROOF_QUALITY;

      if ( m_bBold )
         FontInfo.lfWeight = 700;

      if ( m_bItalic )
         FontInfo.lfItalic = TRUE;

      if ( m_bUnderline )
         FontInfo.lfUnderline = TRUE;

      delete( m_pFont );

      m_pFont = new CFont;
      ASSERT( m_pFont );
      if ( m_pFont == 0 )
         throw( new CMemoryException( ) );

      m_pFont->CreateFontIndirect( &FontInfo );
   }
}

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarMonthCell

ZMiniCalendarMonthCell::ZMiniCalendarMonthCell( )
{
   m_pDaySpots = new ZMiniCalendarDaySpot[ zDAY_SPOTS_IN_MONTH ];
   if ( m_pDaySpots == 0 )
      throw( new CMemoryException( ) );

   m_pMiniCal = 0;
   m_rectHeader.SetRect( 0, 0, 0, 0 );
   m_rectWeekdayHeader.SetRect( 0, 0, 0, 0 );
   m_rect.SetRect( 0, 0, 0, 0 );
   for ( int nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
      m_rectWeekdays[ nWeekday ].SetRect( 0, 0, 0, 0 );

   m_nDaysInMonth = 0;
   m_nRow = 0;
   m_nCol = 0;
   m_nYear = 0;
   m_nMonth = 0;
}

ZMiniCalendarMonthCell::~ZMiniCalendarMonthCell( )
{
   mDeleteInitA( m_pDaySpots );
}

BOOL
ZMiniCalendarMonthCell::IsDaySelected( int nDaySpot )
{
   return( m_pMiniCal->IsDaySelected( m_pDaySpots[ nDaySpot ].m_date ) );
}

BOOL
ZMiniCalendarMonthCell::IsLeftSelected( int nDaySpot )
{
   ASSERT ( nDaySpot >= 0 && nDaySpot < zDAY_SPOTS_IN_MONTH );

   if ( nDaySpot % 7 == 0 )  // left most
      return( FALSE );

   return( IsDaySelected( nDaySpot - 1 ) );
}

BOOL
ZMiniCalendarMonthCell::IsRightSelected( int nDaySpot )
{
   ASSERT ( nDaySpot >= 0 && nDaySpot < zDAY_SPOTS_IN_MONTH );

   if ( nDaySpot % 7 == 6 )  // right most
      return( FALSE );

   return( IsDaySelected( nDaySpot + 1 ) );
}

BOOL
ZMiniCalendarMonthCell::IsTopSelected( int nDaySpot )
{
   ASSERT ( nDaySpot >= 0 && nDaySpot < zDAY_SPOTS_IN_MONTH );

   if ( nDaySpot < 7 )
   {
      return( FALSE );
   }

   return( IsDaySelected( nDaySpot - 7 ) );
}

BOOL
ZMiniCalendarMonthCell::IsBottomSelected( int nDaySpot )
{
   ASSERT ( nDaySpot >= 0 && nDaySpot < zDAY_SPOTS_IN_MONTH );

   if ( nDaySpot >= zDAY_SPOTS_IN_MONTH - 7 )
   {
      return( FALSE );
   }

   return( IsDaySelected( nDaySpot + 7 ) );
}

ZMiniCalendarDaySpot *
ZMiniCalendarMonthCell::DaySpotHitTest( CPoint& pt ) const
{
   for ( int nDaySpot = 0; nDaySpot < zDAY_SPOTS_IN_MONTH; nDaySpot++ )
   {
      if ( m_pDaySpots[ nDaySpot ].m_rect.PtInRect( pt ) )
      {
         return( &(m_pDaySpots[ nDaySpot ]) );
      }
   }

   return( 0 );
}

int
ZMiniCalendarMonthCell::WeekdayHitTest( CPoint& pt ) const
{
   if ( m_pMiniCal && m_pMiniCal->m_bMultipleSel )
   {
      for ( int nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
      {
         if ( m_rectWeekdays[ nWeekday ].PtInRect( pt ) )
         {
         // TraceLineI( "WeekdayHitTest found Weekday: ", nWeekday );
            return( nWeekday );
         }
      }
   }

   return( zNO_HIT );
}

void
ZMiniCalendarMonthCell::InvalidateWeekday( int nWeekday )
{
   if ( nWeekday > 0 && nWeekday <= zDAYS_IN_WEEK )
   {
      CRect rect = m_rectWeekdays[ nWeekday - 1 ];
      rect.InflateRect( 2, 2 );

      m_pMiniCal->InvalidateRect( rect );
   }
}

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendar

IMPLEMENT_DYNAMIC( ZMiniCalendar, CWnd )
BEGIN_MESSAGE_MAP( ZMiniCalendar, CWnd )
   //{{AFX_MSG_MAP( ZMiniCalendar )

   ON_WM_KEYDOWN( )
   ON_WM_KILLFOCUS( )
   ON_WM_SIZE( )

   ON_WM_PAINT( )
   ON_WM_ERASEBKGND( )
   ON_WM_DEVMODECHANGE( )
   ON_WM_FONTCHANGE( )
   ON_WM_PALETTECHANGED( )
   ON_WM_SETTINGCHANGE( )
   ON_WM_TIMECHANGE( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_MOUSEMOVE( )
   ON_WM_SYSCOLORCHANGE( )
   ON_WM_CREATE( )
   ON_WM_TIMER( )
   ON_WM_SETFOCUS( )

   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

CString ZMiniCalendar::m_csClassName = _T( "" );
CString ZMiniCalendar::m_csHeaderClassName = _T( "" );

ZMiniCalendar::ZMiniCalendar( ZDayTimer *pDayTimer ) :
               m_clrLight( ::GetSysColor( COLOR_3DHILIGHT ) ),
               m_clrShadow( ::GetSysColor( COLOR_3DDKSHADOW ) )
{
   if ( m_csClassName.IsEmpty( ) )
      m_csClassName = ::AfxRegisterWndClass( 0 );

   m_pDayTimer = pDayTimer;
   m_nInPaint = 0;
   m_bFontsCreated = FALSE;
   m_bSizeComputed = FALSE;
   m_bNoneTracking = FALSE;
   m_bNoneDown = FALSE;
   m_bTodayTracking = FALSE;
   m_bTodayDown = FALSE;
// m_bScrollTracking = FALSE;
   m_bDayTracking = FALSE;
   m_bHeaderTracking = FALSE;
   m_nMaxSelDays = 0;
   m_pHeaderList = 0;
   m_nHeaderTimerID = 0;
   m_pHeaderCell = 0;
   m_nDayCnt = 0;

   m_nTrackedMonthMonthYear = zNO_HIT;
   m_nPressedMonthMonthYear = zNO_HIT;
   m_nTrackedMonthWeekday = zNO_HIT;
   m_nPressedMonthWeekday = zNO_HIT;
   m_nTrackedScrollButton = zNO_HIT;
   m_nPressedScrollButton = zNO_HIT;
   m_pTrackedDaySpot = 0;
   m_pPressedDaySpot = 0;

   m_nFirstDayOfWeek = 1;
   m_nMonthCells = 0;
   m_pMonthCells = 0;
   COleDateTime date = COleDateTime::GetCurrentTime( );
   m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );

   // Set default startup options.
   SetDefaultFont( FMC_DEFAULT_FONT ); // sets m_csDefaultFontName
   SetDefaultMinFontSize( FMC_DEFAULT_MIN_FONT_SIZE );
   SetHeaderFont( m_csDefaultFontName, FMC_DEFAULT_FONT_SIZE,
                  TRUE, FALSE, FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
   SetDaysOfWeekFont( m_csDefaultFontName, FMC_DEFAULT_FONT_SIZE,
                      FALSE, FALSE, FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
   SetDaysFont( m_csDefaultFontName, FMC_DEFAULT_FONT_SIZE,
                FALSE, FALSE, FALSE, ::GetSysColor( COLOR_BTNTEXT ) );

   // Create bold font (for marked days):
   SetSpecialDaysFont( m_csDefaultFontName, FMC_DEFAULT_FONT_SIZE,
                       TRUE, FALSE, FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
   SetBackColor( ::GetSysColor( COLOR_WINDOW ) );
   SetHighlightToday( TRUE );
   SetShowTodayButton( FALSE );
   SetShowNoneButton( FALSE );
   SetNonMonthDayColor( ::GetSysColor( COLOR_GRAYTEXT ) );
   EnableMultipleSelection( TRUE );
   m_clrSelBack = 0x00b3b562;  // what in the world is this???

   SetUseAutoSettings( FALSE );
   SetMaxSize( FMC_MAX_SIZE_PARENT );
   SetMaxSelDays( FMC_NO_MAX_SEL_DAYS );
   SetShowNonMonthDays( TRUE );
   SetSpecialDaysCallback( 0 );
   m_bShow3dBorder = TRUE;

   // Set month names.
   setlocale( LC_TIME, "" );   // should this be done here AND is it right???
   COleDateTime dateTemp;

   for ( int k = 1; k <= zMONTHS_IN_YEAR; k++ )
   {
      dateTemp.SetDate( 2003, k, 1 );
      SetMonthName( k, dateTemp.Format( "%B" ) );
   }

   // We need to determine the first-day-of-week to use in the
   // calendar.  We use the GetLocaleInfo function to do this.
   zCHAR szDayOfWeek[ 32 ];
   int  nFirstDayOfWeek = 0;
   zmemset( szDayOfWeek, 0, sizeof( szDayOfWeek ) );
   VERIFY( ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK,
                            szDayOfWeek, sizeof( szDayOfWeek ) ) != 0 );

   // The return value of GetLocaleInfo is 0 = Monday, 6 = Sunday.
   // Therefore we need to convert it to 1 = Sunday, 7 = Saturday,
   // which is the standard for COleDateTime.

   // Move from zero base to 1 base + 1 to accomodate Sunday being 6.
   nFirstDayOfWeek = atoi( szDayOfWeek ) + 2;
   if ( nFirstDayOfWeek > zDAYS_IN_WEEK )
   {
      ASSERT( nFirstDayOfWeek == 8 );    // we should never exceed 8
      nFirstDayOfWeek = 1;
   }

   ASSERT( nFirstDayOfWeek >= 1 );
   ASSERT( nFirstDayOfWeek <= zDAYS_IN_WEEK );

   // Set weekday names.
   if ( nFirstDayOfWeek >= 1 && nFirstDayOfWeek <= zDAYS_IN_WEEK )
      SetFirstDayOfWeek( nFirstDayOfWeek );
   else
      SetFirstDayOfWeek( 1 );

   SetRowsAndColumns( 1, 1 );
   SetCurrentMonthAndYear( m_dateToday.GetMonth( ),
                           m_dateToday.GetYear( ) );
}

ZMiniCalendar::~ZMiniCalendar( )
{
   ClearAllSelectedDays( );
   ClearAllDaySpots( );

   mDeleteInitA( m_pMonthCells );
   mDeleteInit( m_pHeaderList );

   if ( m_nHeaderTimerID )
      KillTimer( m_nHeaderTimerID );
}

BOOL
ZMiniCalendar::Create( const RECT& rect, CWnd *pParentWnd, UINT uID )
{
   return( CWnd::Create( m_csClassName, _T( "MiniCal" ),
                         WS_CHILD | WS_VISIBLE, rect, pParentWnd, uID ) );
}

int
ZMiniCalendar::OnCreate( LPCREATESTRUCT lpcs )
{
   int nRC = CWnd::OnCreate( lpcs );
   if ( nRC == -1 )
      return( -1 );

   // Create and adjust tooltip:
   CRect rectEmpty( 0, 0, 0, 0 ); // will be adjusted in "OnSize"

   if ( m_ToolTip.Create( this, TTS_ALWAYSTIP ) &&
        m_ToolTip.AddTool( this, IDS_PREV_YEAR, &rectEmpty, 1 ) &&
        m_ToolTip.AddTool( this, IDS_PREV_MONTH, &rectEmpty, 2 ) &&
        m_ToolTip.AddTool( this, IDS_NEXT_MONTH, &rectEmpty, 3 ) &&
        m_ToolTip.AddTool( this, IDS_NEXT_YEAR, &rectEmpty, 4 ) &&
        m_ToolTip.AddTool( this, IDS_TODAY, &rectEmpty, 5 ) )
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

   // Load images list.
   BOOL bRC = m_Images.Create( IDB_CALENDAR_BUTTONS,
                               g_nImageWidth, 0, g_clrTransparent );
   if ( bRC == FALSE )
   {
      TraceLineS( "ZMiniCalendar: Can't load images list!", "" );
      return( -1 );
   }

   // Define image size.
   IMAGEINFO imageInfo;
   m_Images.GetImageInfo( 0, &imageInfo );

   CRect rectImage = imageInfo.rcImage;
   m_ImageSize = rectImage.Size( );

   // Set mini-calendar styles.
   if ( lpcs->style & FMC_MULTISELECT )
      EnableMultipleSelection( TRUE );

   if ( lpcs->style & FMC_NOHIGHLIGHTTODAY )
      SetHighlightToday( FALSE );

   if ( lpcs->style & FMC_TODAYBUTTON )
      SetShowTodayButton( TRUE );

   if ( lpcs->style & FMC_NONEBUTTON )
      SetShowNoneButton( TRUE );

   if ( lpcs->style & FMC_NO3DBORDER )
      SetShow3DBorder( FALSE );

   if ( lpcs->style & FMC_AUTOSETTINGS )
      SetUseAutoSettings( TRUE );

   if ( lpcs->style & FMC_NOSHOWNONMONTHDAYS )
      SetShowNonMonthDays( FALSE );

   if ( m_bUseAutoSettings )
      AutoConfigure( );

   return( nRC );
}

int
ZMiniCalendar::ScrollButtonHitTest( CPoint& pt )
{
   int nButton;

   if ( m_rectScrollButtons[ eToday ].PtInRect( pt ) )
      nButton = eToday;
   else
   if ( m_rectScrollButtons[ ePrevYear ].PtInRect( pt ) )
      nButton = ePrevYear;
   else
   if ( m_rectScrollButtons[ ePrevMonth ].PtInRect( pt ) )
      nButton = ePrevMonth;
   else
   if ( m_rectScrollButtons[ eNextMonth ].PtInRect( pt ) )
      nButton = eNextMonth;
   else
   if ( m_rectScrollButtons[ eNextYear ].PtInRect( pt ) )
      nButton = eNextYear;
   else
      nButton = zNO_HIT;

   return( nButton );
}

void
ZMiniCalendar::SetTracking( ZMiniCalendarMonthCell **ppMonthHeader,
                            ZMiniCalendarMonthCell **ppMonthWeekdays,
                            ZMiniCalendarMonthCell **ppMonthCell,
                            CPoint& pt )
{
   int nMonthCell;

   *ppMonthHeader = 0;
   *ppMonthWeekdays = 0;
   *ppMonthCell = 0;

   m_nTrackedScrollButton = ScrollButtonHitTest( pt );
   m_nTrackedMonthMonthYear = zNO_HIT;
   m_nTrackedMonthWeekday = zNO_HIT;
   m_pTrackedDaySpot = 0;

   if ( m_nTrackedScrollButton == zNO_HIT )
   {
      *ppMonthHeader = MonthHeaderHitTest( pt );
      if ( *ppMonthHeader )
      {
         nMonthCell = MonthCellFromRowCol( (*ppMonthHeader)->m_nRow,
                                           (*ppMonthHeader)->m_nCol );
         m_nTrackedMonthMonthYear = nMonthCell;
      }
      else
      {
         *ppMonthWeekdays = MonthWeekdayHeaderHitTest( pt );
         if ( *ppMonthWeekdays )
         {
            int nTrackedWeekday = (*ppMonthWeekdays)->WeekdayHitTest( pt );
            if ( nTrackedWeekday != zNO_HIT )
            {
               nMonthCell = MonthCellFromRowCol( (*ppMonthWeekdays)->m_nRow,
                                                  (*ppMonthWeekdays)->m_nCol );
               m_nTrackedMonthWeekday = nMonthCell * 7 + nTrackedWeekday;
            }
         }
         else
         {
            *ppMonthCell = MonthCellHitTest( pt );
            if ( *ppMonthCell )
            {
               m_pTrackedDaySpot = (*ppMonthCell)->DaySpotHitTest( pt );
            }
         }
      }
   }
}

void
ZMiniCalendar::RelayEvent( UINT uMessage, WPARAM wParam, LPARAM lParam )
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

void
ZMiniCalendar::OnLButtonDown( UINT uFlags, CPoint pt )
{
   CRect rectClient;
   GetClientRect( rectClient );

   while ( m_nInPaint > 0 )
      SysWait( 10 );

   do  // purist's goto
   {
   if ( rectClient.PtInRect( pt ) == FALSE )
   {
      m_nTrackedScrollButton = zNO_HIT;
      m_nTrackedMonthMonthYear = zNO_HIT;
      m_nTrackedMonthWeekday = zNO_HIT;
      m_pTrackedDaySpot = 0;
      if ( GetCapture( ) == this )
         ReleaseCapture( );

      break;
   }

   ZMiniCalendarMonthCell *pMonthHeader;
   ZMiniCalendarMonthCell *pMonthWeekdays;
   ZMiniCalendarMonthCell *pMonthCell;

   SetTracking( &pMonthHeader, &pMonthWeekdays, &pMonthCell, pt );
   SetFocus( );
   RelayEvent( WM_LBUTTONDOWN, (WPARAM) uFlags,
               MAKELPARAM( LOWORD( pt.x ), LOWORD( pt.y ) ) );

   if ( (m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) &&
        m_nTrackedScrollButton == zNO_HIT )
   {
      m_nPressedScrollButton = zNO_HIT;
      m_nPressedMonthMonthYear = zNO_HIT;
      m_nPressedMonthWeekday = zNO_HIT;
      m_pPressedDaySpot = 0;
      if ( GetCapture( ) == this )
         ReleaseCapture( );

      break;
   }

   m_nPressedScrollButton = m_nTrackedScrollButton;
   m_nPressedMonthMonthYear = m_nTrackedMonthMonthYear;
   m_nPressedMonthWeekday = m_nTrackedMonthWeekday;
   m_pPressedDaySpot = m_pTrackedDaySpot;
// if ( m_nPressedMonthWeekday != zNO_HIT )
// {
//    TraceLineI( "LButtonDown PressedMonthWeekday: ",
//                m_nPressedMonthWeekday );
// }

// m_dateTrackingStart.SetStatus( COleDateTime::invalid );  // dks???
   if ( m_nPressedScrollButton != zNO_HIT )
   {
      InvalidateRect( m_rectScrollButtons[ m_nTrackedScrollButton ] );
      if ( m_nTrackedScrollButton == eToday )
      {
         m_bTodayTracking = TRUE;
         m_bTodayDown = TRUE;
         RedrawWindow( );
      }
      else
      if ( m_rectScrollButtons[ eNone ].PtInRect( pt ) )
      {
         m_bNoneTracking = TRUE;
         m_bNoneDown = TRUE;
         RedrawWindow( );
      }
      else
      {
         UpdateWindow( );
      // m_bScrollTracking = TRUE;
         m_bSlowTimerMode = TRUE;
         m_nSlowTimerCount = 0;
         SetTimer( g_nIdClickButtonEvt, g_nSlowTimerDuration, 0 );
         OnTimer( g_nIdClickButtonEvt );
      }
   }
   else
   if ( pMonthWeekdays && m_nTrackedMonthWeekday != zNO_HIT )
   {
      pMonthWeekdays->InvalidateWeekday( (m_nTrackedMonthWeekday % 7) );
      RedrawWindow( );
   }
   else
   if ( pMonthHeader )
   {
      m_pHeaderCell = pMonthHeader;
      int nMonthCell = MonthCellFromRowCol( pMonthHeader->m_nRow,
                                            pMonthHeader->m_nCol );
      if ( pt.x > m_rectScrollButtons[ ePrevMonth ].left &&
           pt.x < m_rectScrollButtons[ eNextMonth ].left )
      {
         // Pop up a scrollable list of months from which to select.
         // First, ensure list is not already created.
         ASSERT( m_pHeaderList == 0 );
         mDeleteInitA( m_pHeaderList );

         // Create list.
         m_pHeaderList = new ZMiniCalendarList;
         ASSERT( m_pHeaderList );

         if ( m_pHeaderList == 0 )
            throw( new CMemoryException( ) );

         // Create list control.
         if ( m_csHeaderClassName.IsEmpty( ) )
            m_csHeaderClassName = ::AfxRegisterWndClass( CS_BYTEALIGNCLIENT |
                                                           CS_VREDRAW |
                                                           CS_HREDRAW |
                                                           CS_DBLCLKS,
                                                         0, 0, 0 );

         DWORD dwStyle = WS_POPUP | WS_EX_TOPMOST | WS_EX_WINDOWEDGE | WS_BORDER;
         m_nHeaderTimerID = SetTimer( FMC_HEADER_TIMER_ID,
                                      FMC_HEADER_TIMER_INTERVAL, 0 );

         m_pHeaderList->SetMiddleMonthYear( pMonthHeader->m_nMonth,
                                            pMonthHeader->m_nYear );
         m_pHeaderList->SetCalendar( this );
         m_pHeaderList->CreateEx( 0, m_csHeaderClassName, "", dwStyle,
                                  0, 0, 0, 0, m_hWnd, 0, 0 );

         m_pHeaderList->AutoConfigure( );
         m_pHeaderList->ShowWindow( TRUE );

         SetCapture( );
         m_bHeaderTracking = TRUE;
      }
#ifdef DEBUG_ALL
      else
      {
      // TraceLine( "ZMiniCalendar::OnLButtonDown: Header hit = %s %d",
      //            GetMonthName( nMonth ), nYear );
      // AfxMessageBox( csMessage );
      }
#endif
   }
   else
   if ( m_pPressedDaySpot )
   {
      if ( m_bMultipleSel == FALSE ||
           ((uFlags & MK_CONTROL) == 0 &&   // Ctrl key is not pressed
            (uFlags & MK_SHIFT) == 0) )     // Shift key is not pressed
      {
         ClearAllSelectedDays( );
      }

      m_bDayTracking = TRUE;
      SetCapture( );
      RedrawWindow( );
   // FireNotifyClick( );
   }
   } while ( FALSE ); // end of purist's goto

   CWnd::OnLButtonDown( uFlags, pt );
}

void
ZMiniCalendar::OnLButtonUp( UINT uFlags, CPoint pt )
{
   ZMiniCalendarMonthCell *pMonthHeader;
   ZMiniCalendarMonthCell *pMonthWeekdays;
   ZMiniCalendarMonthCell *pMonthCell;

   RelayEvent( WM_LBUTTONUP, (WPARAM) uFlags,
               MAKELPARAM( LOWORD( pt.x ), LOWORD( pt.y ) ) );

   do  // purist's goto
   {
   if ( m_bHeaderTracking )
   {
      if ( m_pHeaderList && m_pHeaderCell )
      {
         int nSelMonth = m_pHeaderList->GetSelMonth( );
         int nSelYear = m_pHeaderList->GetSelYear( );

      // int nMonthCell = (((m_pHeaderCell->m_nRow - 1) * m_nCols) +
      //                                          m_pHeaderCell->m_nCol) - 1;
         int nMonthCell = MonthCellFromRowCol( m_pHeaderCell->m_nRow,
                                               m_pHeaderCell->m_nCol );
         for ( int nX = 0; nX < nMonthCell; nX++ )
         {
            nSelMonth--;
            if ( nSelMonth < 1 )
            {
               nSelMonth = zMONTHS_IN_YEAR;
               nSelYear--;
            }
         }

         SetCurrentMonthAndYear( nSelMonth, nSelYear );
      }

      mDeleteInit( m_pHeaderList );
   // mDeleteInit( m_pHeaderCell ); pHeaderCell points to the MonthCell array
      m_pHeaderCell = 0;

      if ( m_nHeaderTimerID )
      {
         KillTimer( m_nHeaderTimerID );
         m_nHeaderTimerID = 0;
      }

      m_bHeaderTracking = FALSE;
      ReleaseCapture( );
      RedrawWindow( );
      break;
   }

   SetTracking( &pMonthHeader, &pMonthWeekdays, &pMonthCell, pt );
   if ( m_pPressedDaySpot ||
        m_nPressedMonthWeekday != zNO_HIT ||
        m_nPressedScrollButton != zNO_HIT ||
        m_nPressedMonthMonthYear != zNO_HIT )
   {
      if ( (m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) &&
           m_nPressedScrollButton == zNO_HIT )
      {
         break;
      }

      if ( m_pPressedDaySpot )
      {
         ZMiniCalendarDaySpot *pPressedDaySpot = m_pPressedDaySpot;

         m_pPressedDaySpot = 0;

         if ( IsMultipleSelectionEnabled( ) == FALSE ||
              ((uFlags & MK_CONTROL) == 0 &&
               (uFlags & MK_SHIFT) == 0) )
         {
            ClearAllSelectedDays( );
         }

         if ( pPressedDaySpot == m_pTrackedDaySpot )
         {
            if ( (uFlags & MK_CONTROL) &&
                 IsDaySelected( pPressedDaySpot->m_date ) )
            {
               RemoveSelectedDay( pPressedDaySpot->m_date );
               pPressedDaySpot = 0;
            }
            else
            {
               COleDateTime date1;
               COleDateTime date2;

               if ( (uFlags & MK_SHIFT) && GetSelectedDateByOrder( date1 ) )
               {
                  if  ( date1 <= pPressedDaySpot->m_date )
                  {
                     // Put 'em on the stack in order.
                     date2 = pPressedDaySpot->m_date;
                  // date1 += 1;  // don't reselect
                     while ( date1 < date2 )
                     {
                        AddSelectedDay( date1, FALSE );
                        date1 += 1;
                     }
                  }
                  else
               // if  ( date1 > pPressedDaySpot->m_date )
                  {
                     // Put 'em on the stack in reverse order.
                     date2 = date1;
                  // date2 -= 1;  // don't reselect
                     date1 = pPressedDaySpot->m_date;
                     while ( date1 < date2 )
                     {
                        AddSelectedDay( date2, FALSE );
                        date2 -= 1;
                     }
                  }
               }

               // Make this the "current" selected.
               AddSelectedDay( pPressedDaySpot->m_date );  // force repaint
            }
         }

         if ( pPressedDaySpot )
            InvalidateRect( pPressedDaySpot->m_rect );
      }
      else
      if ( m_nPressedMonthWeekday != zNO_HIT )
      {
         int nPressedMonthWeekday = m_nPressedMonthWeekday;
         int nMonthCell = m_nPressedMonthWeekday / 7;
         int nWeekday = (m_nPressedMonthWeekday % 7);

         m_nPressedMonthWeekday = zNO_HIT;
         if ( nPressedMonthWeekday == m_nTrackedMonthWeekday )
         {
            ZMiniCalendarDaySpot *pDaySpot;

            if ( (uFlags & MK_CONTROL) == 0 )
               ClearAllSelectedDays( );

            // Select this weekday in each week of this month.
            for ( int k = 0; k < 6; k++ )
            {
               pDaySpot = &(m_pMonthCells[ nMonthCell ].m_pDaySpots[ (k * 7) + nWeekday ]);
               if ( pDaySpot->m_bNonMonth == FALSE )
                  AddSelectedDay( pDaySpot->m_date, FALSE );
            }

            OnSelectionChanged( );  // since it was turned off for each add
         }

         m_pMonthCells[ nMonthCell ].InvalidateWeekday( nWeekday + 1 );
         InvalidateRect( m_pMonthCells[ nPressedMonthWeekday ].m_rectHeader );
      }
      else
      if ( m_nPressedScrollButton != zNO_HIT )
      {
         int nPressedScrollButton = m_nPressedScrollButton;

         m_nPressedScrollButton = zNO_HIT;
         if ( nPressedScrollButton == eToday )
         {
            COleDateTime date = COleDateTime::GetCurrentTime( );
            if ( m_nCurrentYear != date.GetYear( ) ||
                 m_nCurrentMonth != date.GetMonth( ) )
            {
               m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );
               SetCurrentMonthAndYear( date.GetMonth( ), date.GetYear( ) );
               ReinitializeCells( );
               RedrawWindow( );
            }
         }
         else
         {
            KillTimer( g_nIdClickButtonEvt );
         }

         InvalidateRect( m_rectScrollButtons[ nPressedScrollButton ] );

         if ( m_nTrackedScrollButton != zNO_HIT )
            InvalidateRect( m_rectScrollButtons[ m_nTrackedScrollButton ] );
      }

      if ( m_nPressedMonthMonthYear != zNO_HIT )
      {
         int nPressedMonthMonthYear = m_nPressedMonthMonthYear;

         m_nPressedMonthMonthYear = zNO_HIT;
         InvalidateRect( m_pMonthCells[ nPressedMonthMonthYear ].m_rectHeader );
      }

      if ( m_pTrackedDaySpot == 0 &&
           m_nTrackedMonthWeekday == zNO_HIT &&
           m_nTrackedScrollButton == zNO_HIT &&
           m_nTrackedMonthMonthYear == zNO_HIT )
      {
         ReleaseCapture( );  // can't release if other buttons are pressed!!! ???
      }

   // UpdateWindow( );
      RedrawWindow( );
   }
   else
// if ( m_bDayTracking )     ???
// {
//    ReleaseCapture( );
//    m_bDayTracking = FALSE;
//    RedrawWindow( );
//    FireNotifyClick( );
// }
// else
   if ( m_bTodayDown )
   {
      FireTodayButton( );
      m_bTodayDown = FALSE;
      RedrawWindow( );
   }
   else
   if ( m_bNoneDown )
   {
      FireNoneButton( );
      m_bNoneDown = FALSE;
      RedrawWindow( );
   }
   else
   {
      ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_SELECT_CALENDAR,
                             (zPVOID) MAKELONG( pt.x, pt.y ) );
   }
   } while ( FALSE ); // end of purist's goto

   CWnd::OnLButtonUp( uFlags, pt );
}

void
ZMiniCalendar::OnLButtonDblClk( UINT uFlags, CPoint pt )
{
   ZMiniCalendarMonthCell *pMonthHeader;
   ZMiniCalendarMonthCell *pMonthWeekdays;
   ZMiniCalendarMonthCell *pMonthCell;

   SetTracking( &pMonthHeader, &pMonthWeekdays, &pMonthCell, pt );
   m_nPressedScrollButton = m_nTrackedScrollButton;
   m_nPressedMonthMonthYear = m_nTrackedMonthMonthYear;
   m_nPressedMonthWeekday = m_nTrackedMonthWeekday;

   if ( (m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      if ( pMonthCell && m_pTrackedDaySpot &&
           m_pTrackedDaySpot == m_pPressedDaySpot )
      {
         m_dateToday = m_pTrackedDaySpot->m_date;
         RedrawWindow( );
         ProcessImmediateEvent( m_pDayTimer,
                                zDAYTIMER_DBLCLK_DATE,
                                m_pTrackedDaySpot );
      // FireNotifyDblClick( );  dks???
      }
      else
      {
         ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_DBLCLK_CALENDAR,
                                (zPVOID) MAKELONG( pt.x, pt.y ) );
      }
   }

   CWnd::OnLButtonDblClk( uFlags, pt );
}

void
ZMiniCalendar::OnRButtonDown( UINT uFlags, CPoint pt )
{
   if ( (m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
      if ( m_pTrackedDaySpot )
         ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_RCLICK_DATE,
                                m_pTrackedDaySpot );
      else
         ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_RCLICK_CALENDAR,
                                (zPVOID) MAKELONG( pt.x, pt.y ) );
   }
}
void
ZMiniCalendar::OnMouseMove( UINT uFlags, CPoint pt )
{
   RelayEvent( WM_MOUSEMOVE, (WPARAM) uFlags,
               MAKELPARAM( LOWORD( pt.x ), LOWORD( pt.y ) ) );

   if ( (m_pDayTimer->m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
   {
   int nPrevTrackScrollButton = m_nTrackedScrollButton;
// TraceLineI( "OnMouseMove PrevTrackScrollButton: ", nPrevTrackScrollButton );
   int nPrevTrackMonthMonthYear = m_nTrackedMonthMonthYear;
   int nPrevTrackMonthWeekday = m_nTrackedMonthWeekday;
   ZMiniCalendarDaySpot *pPrevTrackedDaySpot = m_pTrackedDaySpot;

   ZMiniCalendarMonthCell *pMonthHeader = 0;
   ZMiniCalendarMonthCell *pMonthWeekdays = 0;
   ZMiniCalendarMonthCell *pMonthCell = 0;

   if ( m_bHeaderTracking == FALSE )
      SetTracking( &pMonthHeader, &pMonthWeekdays, &pMonthCell, pt );

// if ( m_nTrackedScrollButton != nPrevTrackScrollButton )
//    TraceLine( "TrackedScrollButton?: %d %d ", m_nTrackedScrollButton,
//                                               nPrevTrackScrollButton );

   // If the tracking state of a button is changing, we have work to do.
   if ( m_pTrackedDaySpot != pPrevTrackedDaySpot ||
        m_nTrackedMonthWeekday != nPrevTrackMonthWeekday ||
        m_nTrackedScrollButton != nPrevTrackScrollButton ||
        m_nTrackedMonthMonthYear != nPrevTrackMonthMonthYear )
   {
      zBOOL bNeedUpdate = FALSE;

      // Check for change to day spot.
      if ( pPrevTrackedDaySpot != 0 &&
           (m_pPressedDaySpot == 0 ||
            pPrevTrackedDaySpot == m_pPressedDaySpot) )
      {
         int nMonthCell = nPrevTrackMonthWeekday / 7;
         int nWeekday = (nPrevTrackMonthWeekday % 7) + 1;

         InvalidateRect( pPrevTrackedDaySpot->m_rect );
         bNeedUpdate = TRUE;
      }

      if ( m_pTrackedDaySpot != 0 &&
           (m_pPressedDaySpot == 0 ||
            m_pTrackedDaySpot == m_pPressedDaySpot) )
      {
         InvalidateRect( m_pTrackedDaySpot->m_rect );
         bNeedUpdate = TRUE;
      }

      // Check for change to month weekday.
      if ( nPrevTrackMonthWeekday != zNO_HIT &&
           (m_nPressedMonthWeekday == zNO_HIT ||
            nPrevTrackMonthWeekday == m_nPressedMonthWeekday) )
      {
         int nMonthCell = nPrevTrackMonthWeekday / 7;
         int nWeekday = (nPrevTrackMonthWeekday % 7) + 1;

         m_pMonthCells[ nMonthCell ].InvalidateWeekday( nWeekday );
         bNeedUpdate = TRUE;
      }

      if ( m_nTrackedMonthWeekday != zNO_HIT &&
           (m_nPressedMonthWeekday == zNO_HIT ||
            m_nTrackedMonthWeekday == m_nPressedMonthWeekday) )
      {
         if ( pMonthWeekdays )  // dks ???
            pMonthWeekdays->InvalidateWeekday( (m_nTrackedMonthWeekday % 7) );

         bNeedUpdate = TRUE;
      }

      // Check for change to scroll button.
      if ( nPrevTrackScrollButton != zNO_HIT &&
           (m_nPressedScrollButton == zNO_HIT ||
            nPrevTrackScrollButton == m_nPressedScrollButton) )
      {
         InvalidateRect( m_rectScrollButtons[ nPrevTrackScrollButton ] );
         bNeedUpdate = TRUE;
      }

      if ( m_nTrackedScrollButton != zNO_HIT &&
           (m_nPressedScrollButton == zNO_HIT ||
            m_nTrackedScrollButton == m_nPressedScrollButton) )
      {
         InvalidateRect( m_rectScrollButtons[ m_nTrackedScrollButton ] );
         bNeedUpdate = TRUE;
      }

      // Check for change to month/year.
      if ( nPrevTrackMonthMonthYear != zNO_HIT &&
           (m_nPressedMonthMonthYear == zNO_HIT ||
            nPrevTrackMonthMonthYear == m_nPressedMonthMonthYear) )
      {
         InvalidateRect( m_pMonthCells[ nPrevTrackMonthMonthYear ].m_rectHeader );
         bNeedUpdate = TRUE;
      }

      if ( m_nTrackedMonthMonthYear != zNO_HIT &&
           (m_nPressedMonthMonthYear == zNO_HIT ||
            m_nTrackedMonthMonthYear == m_nPressedMonthMonthYear) )
      {
         InvalidateRect( m_pMonthCells[ m_nTrackedMonthMonthYear ].m_rectHeader );
         bNeedUpdate = TRUE;
      }

      if ( m_pPressedDaySpot == 0 &&
           m_nPressedMonthWeekday == zNO_HIT &&
           m_nPressedScrollButton == zNO_HIT &&
           m_nPressedMonthMonthYear == zNO_HIT )
      {
         if ( (m_pTrackedDaySpot != 0 && pPrevTrackedDaySpot == 0) ||
              (m_nTrackedMonthWeekday != zNO_HIT && nPrevTrackMonthWeekday == zNO_HIT) ||
              (m_nTrackedScrollButton != zNO_HIT && nPrevTrackScrollButton == zNO_HIT) ||
              (m_nTrackedMonthMonthYear != zNO_HIT && nPrevTrackMonthMonthYear == zNO_HIT) )
         {
         // if ( m_nTrackedScrollButton != zNO_HIT && nPrevTrackScrollButton == zNO_HIT )
         // {
         //    TraceLineS( "TrackedScrollButton: ", "SetCapture" );
         // }

            if ( GetCapture( ) != this )
               SetCapture( );
         }
         else
         if ( (m_pTrackedDaySpot == 0 && m_nTrackedMonthWeekday == zNO_HIT &&
               m_nTrackedScrollButton == zNO_HIT && m_nTrackedMonthMonthYear == zNO_HIT) &&
              (pPrevTrackedDaySpot != 0 || nPrevTrackMonthWeekday != zNO_HIT ||
               nPrevTrackScrollButton != zNO_HIT || nPrevTrackMonthMonthYear != zNO_HIT) )
         {
         // if ( m_nTrackedScrollButton == zNO_HIT && nPrevTrackScrollButton != zNO_HIT )
         // {
         //    TraceLineS( "TrackedScrollButton: ", "ReleaseCapture" );
         // }

            if ( GetCapture( ) == this )
               ReleaseCapture( );
         }
      }

      if ( bNeedUpdate )
      {
      // UpdateWindow( );
         RedrawWindow( );
      }
   }

   if ( m_rectScrollButtons[ eToday ].PtInRect( pt ) )
   {
      if ( m_bTodayDown == FALSE )
      {
         m_bTodayDown = TRUE;
         RedrawWindow( );
      }
      else
      if ( m_bTodayDown )
      {
         m_bTodayDown = FALSE;
         RedrawWindow( );
      }
   }
   else
   if ( m_rectScrollButtons[ eNone ].PtInRect( pt ) )
   {
      if ( m_bNoneDown == FALSE )
      {
         m_bNoneDown = TRUE;
         RedrawWindow( );
      }
      else
      if ( m_bNoneDown )
      {
         m_bNoneDown = FALSE;
         RedrawWindow( );
      }
   }
   }

   CWnd::OnMouseMove( uFlags, pt );
}

void
ZMiniCalendar::OnTimer( UINT uIDEvent )
{
   if ( uIDEvent != g_nIdClickButtonEvt )
      return;

   if ( m_nPressedScrollButton != zNO_HIT )
   {
      if ( m_nPressedScrollButton == m_nTrackedScrollButton )
      {
         if ( m_nPressedScrollButton == ePrevYear )
            ScrollLeft( max( 1, 12 / (m_nRows * m_nCols) ) );
         else
         if ( m_nPressedScrollButton == ePrevMonth )
            ScrollLeft( );
         else
         if ( m_nPressedScrollButton == eNextMonth )
            ScrollRight( );
         else
         if ( m_nPressedScrollButton == eNextYear )
            ScrollRight( max( 1, 12 / (m_nRows * m_nCols) ) );

         if ( m_bSlowTimerMode && m_nSlowTimerCount++ >= g_nNumberOfSlowTimes )
         {
            m_bSlowTimerMode = FALSE;
            KillTimer( g_nIdClickButtonEvt );
            SetTimer( g_nIdClickButtonEvt, g_nFastTimerDuration, 0 );
         }
      }
      else
      {
         if ( m_bSlowTimerMode == FALSE )
         {
            m_bSlowTimerMode = TRUE;
            KillTimer( g_nIdClickButtonEvt );
            SetTimer( g_nIdClickButtonEvt, g_nSlowTimerDuration, 0 );
         }

         m_nSlowTimerCount = 0;
      }
   }
}

// Determine whether or not a given date is special.  This function
// calls a callback function (if specified) to make this determination,
// and then checks the list of marked dates.
BOOL
ZMiniCalendar::IsSpecialDay( COleDateTime &date )
{
   if ( m_pfnSpecialDaysCallback )
   {
      try
      {
         if ( m_pfnSpecialDaysCallback( m_pDayTimer, date ) )
            return( TRUE );
      }
      catch( ... )
      {
         ASSERT( FALSE );
      }
   }

   for ( POSITION pos = m_MarkedDays.GetHeadPosition( ); pos; )
   {
      ZMiniCalendarSelSpot *pSelSpot = m_MarkedDays.GetAt( pos );
      if ( pSelSpot->m_date == date )
      {
         return( TRUE );
      }

      m_MarkedDays.GetNext( pos );
   }

   return( FALSE );
}

// Set the special days callback function.
void
ZMiniCalendar::SetSpecialDaysCallback( fnSPECIALDATE pfnSpecialDaysCallback )
{
   m_pfnSpecialDaysCallback = pfnSpecialDaysCallback;
}

BOOL
ZMiniCalendar::MarkSpecialDay( COleDateTime& date, BOOL bMarked )
{
   ZMiniCalendarSelSpot *pMarkedSpot;
   ZMiniCalendarDaySpot *pDaySpot;
   zBOOL bInserted = FALSE;
   POSITION pos;

   m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );
   for ( pos = m_MarkedDays.GetHeadPosition( );
         pos;
         m_MarkedDays.GetNext( pos ) )
   {
      pMarkedSpot = m_MarkedDays.GetAt( pos );
      if ( pMarkedSpot->m_date == date )   // it is in the list
      {
         if ( bMarked )
         {
            return( TRUE );
         }
         else
         {
            m_MarkedDays.RemoveAt( pos );
            delete( pMarkedSpot );
            break;
         }
      }
   }

   if ( bMarked )
   {
      pMarkedSpot = new ZMiniCalendarSelSpot( date );
      m_SelectedDayOrder.AddHead( pMarkedSpot );
   }
   else
   if ( pos == 0 )  // trying to unmark, but not found
      return( FALSE );

   if ( mIs_hWnd( m_hWnd ) )
   {
      pDaySpot = FindDaySpotByDate( date );
      if ( pDaySpot )
      {
         CRect rect = pDaySpot->m_rect;
         rect.InflateRect( 3, 3 );
         InvalidateRect( rect );
      }
   }

   OnSelectionChanged( );
   return( TRUE );
}

void
ZMiniCalendar::UnMarkAllSpecialDays( )
{
   ZMiniCalendarDaySpot *pDaySpot;

   while ( m_MarkedDays.IsEmpty( ) == FALSE )
   {
      ZMiniCalendarSelSpot *pSelSpot = m_MarkedDays.RemoveHead( );
      pDaySpot = FindDaySpotByDate( pSelSpot->m_date );
      if ( pDaySpot )
      {
         pDaySpot->m_bSelected = FALSE;
         if ( mIs_hWnd( m_hWnd ) )
         {
            CRect rect = pDaySpot->m_rect;
            rect.InflateRect( 3, 3 );
            InvalidateRect( rect );
         }
      }

      delete( pSelSpot );
   }

   OnSelectionChanged( );
}

void
ZMiniCalendar::OnSelectionChanged( )
{
   if ( m_pDayTimer )
   {
      if ( m_pDayTimer->m_pAgenda )
         m_pDayTimer->m_pAgenda->UpdatePositionOfControls( );

      if ( m_pDayTimer->m_pAgenda )
         m_pDayTimer->m_pAgenda->ShowSelectedDays( );
   }

   ProcessImmediateEvent( m_pDayTimer, zMINICAL_SELECT_CHANGE );
}

int
ZMiniCalendar::GetSelectedDayCount( )
{
   int nSelectedCnt = 0;

   for ( POSITION pos = m_SelectedDays.GetHeadPosition( );
         pos;
         m_SelectedDays.GetNext( pos ) )
   {
      nSelectedCnt++;
   }

   return( nSelectedCnt );
}

BOOL
ZMiniCalendar::GetSelectedDateByOrder( COleDateTime& date,
                                       int nSelectedOrder )
{
   ZMiniCalendarSelSpot *pSelSpot;
   POSITION pos = m_SelectedDayOrder.GetHeadPosition( );

   while ( pos && nSelectedOrder >= 0 )
   {
      if ( nSelectedOrder == 0 )
      {
         pSelSpot = m_SelectedDayOrder.GetAt( pos );
         date = pSelSpot->m_date;
         return( TRUE );
      }

      nSelectedOrder--;
      m_SelectedDayOrder.GetNext( pos );
   }

   // Return true if there was a selected date, false otherwise.
   if ( m_dateToday.GetStatus( ) == COleDateTime::valid )
   {
      date = m_dateToday;
      return( TRUE );
   }
   else
   {
      ASSERT( FALSE );
      date = COleDateTime::GetCurrentTime( );
      return( FALSE );
   }
}

ZMiniCalendarDaySpot *
ZMiniCalendar::FindDaySpotByDate( COleDateTime& date )
{
   for ( int nMonthCell = 0; nMonthCell < m_nMonthCells; nMonthCell++ )
   {
      for ( int nDaySpot = 0; nDaySpot < zDAY_SPOTS_IN_MONTH; nDaySpot++ )
      {
         if ( m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_date == date )
            return( &(m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ]) );
      }
   }

   return( 0 );
}

void
ZMiniCalendar::ClearAllSelectedDays( )
{
   ZMiniCalendarDaySpot *pDaySpot;

   while ( m_SelectedDays.IsEmpty( ) == FALSE )
   {
      ZMiniCalendarSelSpot *pSelSpot = m_SelectedDays.RemoveHead( );
      pDaySpot = FindDaySpotByDate( pSelSpot->m_date );
      if ( pDaySpot )
      {
         pDaySpot->m_bSelected = FALSE;
         if ( mIs_hWnd( m_hWnd ) )
         {
            CRect rect = pDaySpot->m_rect;
            rect.InflateRect( 3, 3 );
            InvalidateRect( rect );
         }
      }

      delete( pSelSpot );
   }

   m_SelectedDayOrder.RemoveAll( );
   OnSelectionChanged( );
}

void
ZMiniCalendar::AddSelectedDay( COleDateTime& date,
                               BOOL bRedraw )
{
   ZMiniCalendarSelSpot *pSelSpot;
   ZMiniCalendarDaySpot *pDaySpot;
   zBOOL bInserted = FALSE;
   POSITION pos;

   m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );

   if ( m_pDayTimer && m_pDayTimer->m_pAgenda )
   {
      m_pDayTimer->m_pAgenda->m_pAppointmentLastFocus = 0;
      m_pDayTimer->m_pAgenda->m_dateLastSelected = m_dateToday;
   }

   for ( pos = m_SelectedDays.GetHeadPosition( );
         pos;
         m_SelectedDays.GetNext( pos ) )
   {
      pSelSpot = m_SelectedDays.GetAt( pos );
      if ( pSelSpot->m_date == date )   // already in list
      {
         // Force current date to tail of ordered selected list.
         for ( pos = m_SelectedDayOrder.GetHeadPosition( );
               pos;
               m_SelectedDayOrder.GetNext( pos ) )
         {
            if ( (m_SelectedDayOrder.GetAt( pos ))->m_date == pSelSpot->m_date )
            {
               m_SelectedDayOrder.RemoveAt( pos );
               break;
            }
         }

         m_SelectedDayOrder.AddTail( pSelSpot );

         if ( bRedraw )
            OnSelectionChanged( );

         return;
      }

      if ( pSelSpot->m_date > date )
      {
         pSelSpot = new ZMiniCalendarSelSpot( date );
         m_SelectedDays.InsertBefore( pos, pSelSpot );
         bInserted = TRUE;
         break;
      }
   }

   if ( bInserted == FALSE )
   {
      pSelSpot = new ZMiniCalendarSelSpot( date );
      m_SelectedDays.AddTail( pSelSpot );

      // Force current date to tail of ordered selected list.
      for ( pos = m_SelectedDayOrder.GetHeadPosition( );
            pos;
            m_SelectedDayOrder.GetNext( pos ) )
      {
         if ( (m_SelectedDayOrder.GetAt( pos ))->m_date == pSelSpot->m_date )
         {
            m_SelectedDayOrder.RemoveAt( pos );
            break;
         }
      }
   }

   m_SelectedDayOrder.AddTail( pSelSpot );
   m_nDayCnt = GetSelectedDayCount( );

   pDaySpot = FindDaySpotByDate( date );
   ProcessImmediateEvent( m_pDayTimer, zDAYTIMER_SELECT_DATE, pDaySpot );
   if ( pDaySpot )
   {
      CRect rect = pDaySpot->m_rect;
      rect.InflateRect( 3, 3 );
      InvalidateRect( rect );
   }

   if ( bRedraw )
      OnSelectionChanged( );
}

void
ZMiniCalendar::RemoveSelectedDay( COleDateTime& date )
{
   ZMiniCalendarSelSpot *pSelSpot;
   ZMiniCalendarSelSpot *pOrderedSelSpot;
   ZMiniCalendarDaySpot *pDaySpot;
   POSITION pos;

   for ( pos = m_SelectedDays.GetHeadPosition( );
         pos;
         m_SelectedDays.GetNext( pos ) )
   {
      pSelSpot = m_SelectedDays.GetAt( pos );
      if ( pSelSpot->m_date == date )
      {
         m_SelectedDays.RemoveAt( pos );

         // Remove date from ordered selected list.
         for ( pos = m_SelectedDayOrder.GetHeadPosition( );
               pos;
               m_SelectedDayOrder.GetNext( pos ) )
         {
            pOrderedSelSpot = m_SelectedDayOrder.GetAt( pos );
            if ( pOrderedSelSpot->m_date == pSelSpot->m_date )
            {
               m_SelectedDayOrder.RemoveAt( pos );
               break;
            }
         }

         if ( mIs_hWnd( m_hWnd ) )
         {
            pDaySpot = FindDaySpotByDate( date );
            if ( pDaySpot )
            {
               CRect rect = pDaySpot->m_rect;
               rect.InflateRect( 3, 3 );
               InvalidateRect( rect );
            }
         }

         delete( pSelSpot );
         break;
      }
   }

   if ( m_dateToday == date )
   {
      pos = m_SelectedDays.GetHeadPosition( );
      if ( pos && (pSelSpot = m_SelectedDays.GetAt( pos )) != 0 )
         m_dateToday = pSelSpot->m_date;
      else
      {
         COleDateTime dateT = COleDateTime::GetCurrentTime( );
         m_dateToday.SetDate( dateT.GetYear( ), dateT.GetMonth( ),
                              dateT.GetDay( ) );
      }
   }

   if ( m_pDayTimer && m_pDayTimer->m_pAgenda )
   {
      m_pDayTimer->m_pAgenda->m_pAppointmentLastFocus = 0;
      m_pDayTimer->m_pAgenda->m_dateLastSelected = m_dateToday;
   }

   m_nDayCnt = GetSelectedDayCount( );
   OnSelectionChanged( );
}

BOOL
ZMiniCalendar::IsDaySelected( COleDateTime& date ) const
{
   for ( POSITION pos = m_SelectedDays.GetHeadPosition( ); pos; )
   {
      ZMiniCalendarSelSpot *pSelSpot = m_SelectedDays.GetAt( pos );
      if ( pSelSpot->m_date == date )
      {
         return( TRUE );
      }

      m_SelectedDays.GetNext( pos );
   }

   return( FALSE );
}

void
ZMiniCalendar::OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags )
{
// ASSERT( m_pParent );

   if ( uChar == VK_ESCAPE )
   {
      ReleaseCapture( );
   // HideDropCalendar( );
   }
}

BOOL
ZMiniCalendar::OnEraseBkgnd( CDC *pDC )
{
   return( TRUE );
}

void
ZMiniCalendar::OnPaint( )
{
   m_nInPaint++;  // prevent button down while in paint

// The BeginPaint member function automatically validates the update region,
// so any call to GetUpdateRect made immediately after a call to BeginPaint
// retrieves an empty update region.
   CRect rect;
   GetUpdateRect( rect, FALSE );
// TraceRect( "ZMiniCalendar::OnPaint UpdateRect: ", rect );

   CRect rectClient;
   GetClientRect( rectClient );

   CPaintDC dcPaint( this ); // device context for painting

   // If possible, we paint to an off-screen DC and BitBlt the results to
   // the screen (reduces flicker).
   CDC      *pDC;
   zBOOL    bMemDC;
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
   else
   {
      bMemDC = FALSE;
      pDC = &dcPaint;
   }

//?pDC->SetBkMode( TRANSPARENT );

// ZMemDC memDC( &dcPaint );

   if ( m_bSizeComputed == FALSE )
      ComputeSize( );

   pDC->FillSolidRect( 0, 0, rectClient.Width( ),
                       rectClient.Height( ), m_clrBack );

   int nStartY = 4;
   int nStartX = 4;

   if ( m_bShow3dBorder )
   {
      pDC->Draw3dRect( 0, 0, rectClient.Width( ), rectClient.Height( ),
                       ::GetSysColor( COLOR_BTNFACE ),
                       ::GetSysColor( COLOR_BTNTEXT ) );
      pDC->Draw3dRect( 1, 1, rectClient.Width( ) - 2,
                       rectClient.Height( ) - 2,
                       ::GetSysColor( COLOR_WINDOW ),
                       ::GetSysColor( COLOR_3DSHADOW ) );
      pDC->Draw3dRect( 2, 2, rectClient.Width( ) - 4,
                       rectClient.Height( ) - 4,
                       ::GetSysColor( COLOR_BTNFACE ),
                       ::GetSysColor( COLOR_BTNFACE ) );
      pDC->Draw3dRect( 3, 3, rectClient.Width( ) - 6,
                       rectClient.Height( ) - 6,
                       ::GetSysColor( COLOR_3DSHADOW ),
                       ::GetSysColor( COLOR_WINDOW ) );
   }
   else
   {
      nStartX = 0;
      nStartY = 0;
   }

   int nY = nStartY;
   int nMonth = m_nCurrentMonth;
   int nYear = m_nCurrentYear;

   // Draw each row/column (month cell) individually.
   for ( int nRow = 1; nRow <= m_nRows; nRow++ )
   {
      int nCurrentX = nStartX;
      for ( int nCol = 1; nCol <= m_nCols; nCol++ )
      {
         int nCurrentY = nY;

         nCurrentY += DrawHeader( memDC, nCurrentY, nCurrentX,
                                  nRow, nCol, nYear, nMonth );
         nCurrentY += DrawDaysOfWeek( memDC, nCurrentY, nCurrentX,
                                      nRow, nCol, nYear, nMonth );
         nCurrentY += DrawDays( memDC, nCurrentY, nCurrentX, nRow, nCol,
                                nYear, nMonth );

         nCurrentX += m_sizeMonth.cx;

         nMonth++;
         if ( nMonth > zMONTHS_IN_YEAR )
         {
            nMonth = 1;
            nYear++;
         }
      }

      nY += m_sizeMonth.cy;
   }

   nY += DrawTodayButton( memDC, nY );

   if ( bMemDC )
   {
      // Copy the results to the on-screen DC.
      CRect rectClip;
      dcPaint.GetClipBox( rectClip );
      dcPaint.BitBlt( rectClip.left, rectClip.top,
                      rectClip.Width( ), rectClip.Height( ),
                      &memDC, rectClip.left, rectClip.top, SRCCOPY );

      memDC.SelectObject( pOldBmp );
   }

   m_nInPaint--;
}

// Draw the month/year header (e.g.  October 2004).
int
ZMiniCalendar::DrawHeader( CDC &dc, int nY, int nLeftX,
                           int nMonthRow, int nMonthCol,
                           int nYear, int nMonth )
{
   CString csText;
   csText.Format( "%s %d", GetMonthName( nMonth ), nYear );

   if ( m_bFontsCreated == FALSE )
      CreateFontObjects( );

   dc.FillSolidRect( nLeftX, nY, m_sizeMonth.cx - 1, m_nHeaderHeight,
                     ::GetSysColor( COLOR_BTNFACE ) );
   dc.Draw3dRect( nLeftX, nY, m_sizeMonth.cx - 1, m_nHeaderHeight,
                  ::GetSysColor( COLOR_WINDOW ),
                  ::GetSysColor( COLOR_3DSHADOW ) );

   CFont *pOldFont = dc.SelectObject( m_HeaderFontInfo.m_pFont );
   dc.SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
   dc.SetTextColor( m_HeaderFontInfo.m_cColor );
   dc.SetBkMode( TRANSPARENT );

   dc.DrawText( csText, CRect( nLeftX + 10, nY + 1,
                               nLeftX + m_sizeMonth.cx - 10,
                               nY + m_nHeaderHeight - 2 ),
                               DT_CENTER | DT_VCENTER | DT_SINGLELINE );

   CRect rectMonthYear( nLeftX + 30, nY + 1,
                        nLeftX + m_sizeMonth.cx - 30,
                        nY + m_nHeaderHeight - 2 );


   if ( m_nTrackedMonthMonthYear != zNO_HIT )
   {
      int nMonthCell = MonthCellFromRowCol( nMonthRow, nMonthCol );
      if ( nMonthCell == m_nTrackedMonthMonthYear )
         dc.Draw3dRect( &rectMonthYear, m_clrLight, m_clrShadow );
   }

   dc.SelectObject( pOldFont );

   SetCellPosition( nMonthRow, nMonthCol, rectMonthYear,
                    nYear, nMonth, 1 );

   CPen *pPen = new CPen( PS_SOLID, 1, ::GetSysColor( COLOR_CAPTIONTEXT ) );
   CPen *pOldPen = dc.SelectObject( pPen );

   // Deal with left scroll arrows and "Today" button.
   if ( nMonthRow == 1 && nMonthCol == 1 )
   {
      if ( m_bToday )
         m_rectScrollButtons[ eToday ].SetRect( nLeftX + 2, m_nHeaderHeight + 7,
                                                m_ImageSize.cx + nLeftX + 4,
                                                m_ImageSize.cy + m_nHeaderHeight + 9 );

      int nMiddle = nY + (m_nHeaderHeight / 2);
      int nTop = nMiddle - (m_nHeaderHeight / 4);
      int nBottom = nMiddle + (m_nHeaderHeight / 4);

      int nX1 = nLeftX + 4;
      int nX2 = nX1 + m_ImageSize.cx;

      m_rectScrollButtons[ ePrevYear ].SetRect( nX1, nTop - 1,
                                                nX2, nBottom + 1 );
      int k = nX2 - nX1 + 2;
      nX2 += k;
      nX1 += k;
      m_rectScrollButtons[ ePrevMonth ].SetRect( nX1, nTop - 1,
                                                 nX2, nBottom + 1 );
#if 0
      // Draw a left pointing arrow.
      for ( int nLineY = nTop; nLineY <= nBottom; nLineY++ )
      {
         dc.MoveTo( nX1, nMiddle );
         dc.LineTo( nX2, nLineY );
      }
#endif
   }

   // Deal with right scroll arrows.
   if ( nMonthRow == 1 && nMonthCol == m_nCols )
   {
      int nMiddle = nY + (m_nHeaderHeight / 2);
      int nTop = nMiddle - (m_nHeaderHeight / 4);
      int nBottom = nMiddle + (m_nHeaderHeight / 4);

      int nX1 = nLeftX + m_sizeMonth.cx - 6;
      int nX2 = nX1 - m_ImageSize.cx;

      m_rectScrollButtons[ eNextYear ].SetRect( nX2, nTop - 1,
                                                nX1, nBottom + 1 );
      int k = nX1 - nX2 + 2;
      nX2 -= k;
      nX1 -= k;
      m_rectScrollButtons[ eNextMonth ].SetRect( nX2, nTop - 1,
                                                 nX1, nBottom + 1 );
#if 0
      // Draw a right pointing arrow.
      for ( int nLineY = nTop; nLineY <= nBottom; nLineY++ )
      {
         dc.MoveTo( nX1, nMiddle );
         dc.LineTo( nX2, nLineY );
      }
#endif
   }

   dc.SelectObject( pOldPen );
   delete( pPen );

   for ( int k = 0; k < zMINICAL_BUTTON_IMAGE_CNT; k++ )
   {
      CRect rect = m_rectScrollButtons[ k ];
      if ( (k != eToday || m_bToday) && dc.RectVisible( rect ) )
      {
         zBOOL bPressed = (k == m_nPressedScrollButton);
         zBOOL bTracked = (k == m_nTrackedScrollButton);

         CPoint ptImage( rect.left + (rect.Width( ) - m_ImageSize.cx) / 2,
                         rect.top + (rect.Height( ) - m_ImageSize.cy) / 2 );
#if 1
         if ( k == eToday )
         {
            ptImage.x -= 1;
            ptImage.y -= 1;
         }
#endif
         if ( bPressed && bTracked )
            ptImage.Offset( 1, 1 );

         m_Images.Draw( &dc, bTracked ? k + zMINICAL_BUTTON_IMAGE_CNT : k,
                        ptImage, ILD_NORMAL );

         // Draw 3D border for the tracked or pressed buttons:
         if ( bTracked || bPressed )
         {
            dc.Draw3dRect( &rect,
                           bPressed && bTracked ? m_clrShadow : m_clrLight,
                           bPressed && bTracked ? m_clrLight : m_clrShadow );
         // dc.Draw3dRect( &rect, ::GetSysColor( COLOR_3DDKSHADOW ),
         //                ::GetSysColor( COLOR_3DHILIGHT ) );
         }
      }
   }

   // Adjust tooltip rectangles:
   m_ToolTip.SetToolRect( this, 1, m_rectScrollButtons[ ePrevYear ] );
   m_ToolTip.SetToolRect( this, 2, m_rectScrollButtons[ ePrevMonth ] );
   m_ToolTip.SetToolRect( this, 3, m_rectScrollButtons[ eNextMonth ] );
   m_ToolTip.SetToolRect( this, 4, m_rectScrollButtons[ eNextYear ] );
   if ( m_bToday )
      m_ToolTip.SetToolRect( this, 5, m_rectScrollButtons[ eToday ] );

   return( m_nHeaderHeight );
}

// Draw days of week header (i.e. S M T W T F S).
int
ZMiniCalendar::DrawDaysOfWeek( CDC &dc, int nY, int nLeftX,
                               int nMonthRow, int nMonthCol,
                               int nYear, int nMonth )
{
   int nMonthCell = MonthCellFromRowCol( nMonthRow, nMonthCol );
   ZMiniCalendarMonthCell *pMonthCell = &(m_pMonthCells[ nMonthCell ]);
   int nStartX = 0;
   int nEndX = 0;
   int nX = 0;

   if ( m_bFontsCreated == FALSE )
      CreateFontObjects( );

   // Calculate starting X position.
   nStartX = ((nLeftX + (m_sizeMonth.cx / 2))) -
                                    (((m_nIndividualDayWidth * 7) + 30) / 2);
   nEndX = ((nLeftX + (m_sizeMonth.cx / 2))) +
                                    (((m_nIndividualDayWidth * 7) + 30) / 2);

   nX = nStartX;

   CFont *pOldFont = dc.SelectObject( m_DaysOfWeekFontInfo.m_pFont );
   dc.SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
   dc.SetTextColor( m_DaysOfWeekFontInfo.m_cColor );
   dc.SetBkMode( TRANSPARENT );

   // Draw week days row.
   for ( int nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
   {
      CRect rectText( nX, nY + 1, nX + m_nIndividualDayWidth,
                      nY + 2 + m_nDaysOfWeekHeight );
      CRect rect( nX - 3, nY + 1, nX + m_nIndividualDayWidth + 2,
                  nY + 2 + m_nDaysOfWeekHeight + 2 );
      pMonthCell->m_rectWeekdays[ nWeekday ] = rect;

      zBOOL bPressed = (m_nPressedMonthWeekday != zNO_HIT &&
                        nMonthCell == (m_nPressedMonthWeekday / 7) &&
                        nWeekday == (m_nPressedMonthWeekday % 7));
      zBOOL bTracked = (m_nTrackedMonthWeekday != zNO_HIT &&
                        nMonthCell == (m_nTrackedMonthWeekday / 7) &&
                        nWeekday == (m_nTrackedMonthWeekday % 7));

      if ( bPressed && bTracked )
      {
         rect.OffsetRect( 1, 1 );
      }

      if ( dc.RectVisible( rect ) )
      {
         dc.DrawText( GetDayOfWeekName( nWeekday + 1 ), 1, rectText,
                      DT_SINGLELINE | DT_RIGHT | DT_BOTTOM );

         if ( m_bMultipleSel )
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

      nX += m_nIndividualDayWidth + 5;
   }

   dc.SelectObject( pOldFont );

   // Draw the divider line.
   CPen *pPen = new CPen( PS_SOLID, 1, ::GetSysColor( COLOR_3DSHADOW ) );
   CPen *pOldPen = dc.SelectObject( pPen );

   dc.MoveTo( nStartX, nY + 3 + m_nDaysOfWeekHeight );
   dc.LineTo( nEndX, nY + 3 + m_nDaysOfWeekHeight );

   dc.SelectObject( pOldPen );
   delete( pPen );

   SetCellPosition( nMonthRow, nMonthCol,
                    CRect( nStartX, nY, nEndX, nY + m_nDaysOfWeekHeight + 3 ),
                    nYear, nMonth, 2 );
   return( m_nDaysOfWeekHeight + 4 );
}

// Draw days of month.
int
ZMiniCalendar::DrawDays( CDC &dc, int nY, int nLeftX,
                         int nMonthRow, int nMonthCol,
                         int nYear, int nMonth )
{
// CRect rectClient;
   int nMonthCell = MonthCellFromRowCol( nMonthRow, nMonthCol );
   ZMiniCalendarMonthCell *pMonthCell = &(m_pMonthCells[ nMonthCell ]);
   COleDateTime dateStart;
   COleDateTime date;
   int nStartY = nY;
   int nStartX;
   int nEndX;
   int nReturnHeight = 0;

   if ( m_bFontsCreated == FALSE )
      CreateFontObjects( );

   dateStart.SetDate( nYear, nMonth, 1 );

   date = dateStart;
   while ( date.GetDayOfWeek( ) != m_nFirstDayOfWeek )
      date -= 1;

// GetClientRect( rectClient );

   // Calculate starting X position.
   nStartX = ((nLeftX + (m_sizeMonth.cx / 2))) -
                                    (((m_nIndividualDayWidth * 7) + 30) / 2);
   nEndX = ((nLeftX + (m_sizeMonth.cx / 2))) +
                                    (((m_nIndividualDayWidth * 7) + 30) / 2);

   CFont *pOldFont = dc.SelectObject( m_DaysFontInfo.m_pFont );
   COLORREF clrOldText = dc.SetTextColor( m_DaysFontInfo.m_cColor );
   int nOldBkMode = dc.SetBkMode( TRANSPARENT );

   CString  csText;
   COLORREF clrBack;
   COLORREF clrText;
   BOOL     bIsDaySelected;
   BOOL     bIsDayMarked;
   BOOL     bIsDayInvalid;
   BOOL     bIsToday;
   BOOL     bHighlight;
   BOOL     bNonMonthDay;
   int      nDayOfMonth;
   int      nDaySpot;
   int      nRow;

   // Allow up to 6 rows of days.  This is the actual maximum number of
   // calendar weeks that can occur in a month.
   for ( nDaySpot = 0, nRow = 1; nRow <= 6; nRow++ )
   {
      int nX = nStartX;

      for ( int nWeekday = 0; nWeekday < zDAYS_IN_WEEK; nWeekday++ )
      {
         // Special logic to permit showing of non-month days.
         if ( date.GetMonth( ) == dateStart.GetMonth( ) ||
              (date > dateStart && nMonthCol == m_nCols &&
               nMonthRow == m_nRows && m_bShowNonMonthDays) ||
              (date < dateStart && nMonthCol == 1 &&
               nMonthRow == 1 && m_bShowNonMonthDays) )
         {
            nDayOfMonth = date.GetDay( );
            csText.Format( "%d", nDayOfMonth );
            clrBack = m_clrBack;
            clrText = m_DaysFontInfo.m_cColor;

         // int nSelectedCnt = GetSelectedDayCount( );
            bIsDaySelected = pMonthCell->IsDaySelected( nDaySpot );

#ifdef DEBUG_ALL
         // if ( bIsDaySelected && m_pTrackedDaySpot && m_pTrackedDaySpot->m_date == date )
         //    TraceLineS( "", "" );

            if ( m_pTrackedDaySpot && m_pTrackedDaySpot->m_date == date &&
                 m_pPressedDaySpot == m_pTrackedDaySpot )
            {
               TraceLineI( "OnPaint PressedDay: ", m_pPressedDaySpot->m_nDaySpot );
            }

            if (  m_pPressedDaySpot )
               TraceLineI( "OnPaint PressedDay: ", m_pPressedDaySpot->m_nDaySpot );
#endif

            bIsDayMarked = IsSpecialDay( date );
            bIsToday = IsToday( date );
            bIsDayInvalid = !IsDateValid( date );
            bHighlight = FALSE;
            bNonMonthDay = (date.GetMonth( ) != nMonth);

         // CRect( nX - 3, nY, nX - 3 + m_nIndividualDayWidth + 5,
         //        nY + m_nDaysHeight + 2 ) );
            CRect rectDay( nX - 3, nY, nX - 3 + m_nIndividualDayWidth + 5,
                           nY + m_nDaysHeight + 3 );

            if ( dc.RectVisible( rectDay ) )
            {
            // TraceLine( "RectVisible for Year: %d  Month: %d  DaySpot: %d  Selected: %s",
            //            nYear, nMonth, nDaySpot, bIsDaySelected ? "Y" : "N" );
               CFont *pFontOld = 0;

               if ( bIsDayMarked )
                  pOldFont = dc.SelectObject( m_SpecialDaysFontInfo.m_pFont );
               else
                  pOldFont = dc.SelectObject( m_DaysFontInfo.m_pFont );

               if ( bIsDayInvalid )
                  clrText = ::GetSysColor( COLOR_GRAYTEXT );
               else
               if ( bNonMonthDay )
                 clrText = m_clrNonMonthDay;

               if ( bIsDaySelected )
               {
                  clrBack = m_clrSelBack;
                  if ( bNonMonthDay == FALSE )
                     clrText = m_clrSelText;

                  dc.FillSolidRect( rectDay, clrBack );

                  if ( m_bMultipleSel == FALSE )
                  {
                     bHighlight = TRUE;
                  }
                  else
                  {
                     CPen penDark( PS_SOLID, 1, m_clrShadow );
                     CPen *pOldPen = (CPen *) dc.SelectObject( &penDark );

                     if ( pMonthCell->IsLeftSelected( nDaySpot ) == FALSE )
                     {
                        dc.MoveTo( rectDay.left, rectDay.top );
                        dc.LineTo( rectDay.left, rectDay.bottom );
                     }

                     if ( pMonthCell->IsTopSelected( nDaySpot ) == FALSE )
                     {
                        dc.MoveTo( rectDay.left, rectDay.top );
                        dc.LineTo( rectDay.right, rectDay.top );
                     }

                     CPen penLight( PS_SOLID, 1, m_clrLight );
                     dc.SelectObject( &penLight );

                     if ( pMonthCell->IsRightSelected( nDaySpot ) == FALSE )
                     {
                        dc.MoveTo( rectDay.right, rectDay.top );
                        dc.LineTo( rectDay.right, rectDay.bottom );
                     }

                     if ( pMonthCell->IsBottomSelected( nDaySpot ) == FALSE )
                     {
                        dc.MoveTo( rectDay.left, rectDay.bottom - 1 );
                        dc.LineTo( rectDay.right, rectDay.bottom - 1 );
                     }

                     dc.SelectObject( pOldPen );
                  }
               }

               if ( bIsToday )
               {
                  // Circle Today (or should I say "square it").
                  CRect rect = rectDay;
                  rect.InflateRect( CSize( 1, 1 ) );
                  rect.bottom--;
                  dc.Draw3dRect( rect, RGB( 132, 0, 0 ), RGB( 132, 0, 0 ) );
               }

            // dc.DrawText( csDay, rectDay, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
               dc.SetTextColor( clrText );
               dc.DrawText( csText, CRect( nX, nY, nX + m_nIndividualDayWidth,
                                           nY + m_nDaysHeight ),
                            DT_BOTTOM | DT_RIGHT | DT_SINGLELINE );

               if ( pFontOld )
                  dc.SelectObject( pFontOld );

               // Define button state.
               eBUTTON_STATE nBtnState = eFLAT;

               if ( m_pPressedDaySpot && m_pPressedDaySpot->m_date == date )
               {
                  if ( m_pTrackedDaySpot && m_pTrackedDaySpot->m_date == date )
                  {
                     nBtnState = ePRESSED;
                  }
                  else
                  {
                     nBtnState = eHILIGHT;
                  }
               }
               else
               if ( m_pTrackedDaySpot && m_pTrackedDaySpot->m_date == date &&
                    m_pPressedDaySpot == 0 )
               {
                  nBtnState = eHILIGHT;
               }
               else
               if ( bHighlight )
               {
                  nBtnState = ePRESSED;
               }

               // Draw 3D border around the cell.
               switch ( nBtnState )
               {
                  case ePRESSED:
                     dc.Draw3dRect( rectDay, m_clrShadow, m_clrLight );
                  // rectDay.OffsetRect( 1, 1 ); ???
                     break;

                  case eHILIGHT:
                     dc.Draw3dRect( rectDay, m_clrLight, m_clrShadow );
                     break;
               }
            }
            else
            {
#if 0
               if ( nDaySpot == 14 )
               {
                  TraceLine( "Rect NOT Visible for Year: %d  Month: %d  DaySpot: %d  Selected: %s",
                             nYear, nMonth, nDaySpot, bIsDaySelected ? "Y" : "N" );
                  TraceRect( " ===> #14 Day rect: ", rectDay );
               }
#endif
            }

            SetDaySpot( nMonthRow, nMonthCol, nDaySpot, date,
                        CRect( nX - 3, nY, nX - 3 + m_nIndividualDayWidth + 5,
                               nY + m_nDaysHeight + 2 ) );
         }

         date += 1;
         nX += (m_nIndividualDayWidth + 5);
         nDaySpot++;
      }

      nY += (2 + m_nDaysHeight);
      nReturnHeight += (2 + m_nDaysHeight);
   }

   dc.SetBkMode( nOldBkMode );
   dc.SelectObject( pOldFont );
   dc.SetTextColor( clrOldText );   // restore default color

   // Draw the divider line.
   if ( nRow == m_nRows )
   {
      CPen *pPen = new CPen( PS_SOLID, 1, ::GetSysColor( COLOR_3DSHADOW ) );
      CPen *pOldPen = dc.SelectObject( pPen );

      dc.MoveTo( nStartX, nY );
      dc.LineTo( nEndX, nY );

      dc.SelectObject( pOldPen );
      delete( pPen );
   }

   nReturnHeight += 5;
   SetCellPosition( nMonthRow, nMonthCol,
                    CRect( nStartX, nStartY, nEndX, nY ),
                    nYear, nMonth, 3 );
   return( nReturnHeight );
}

void
ZMiniCalendar::SetValidInterval( const COleDateTime& fromDate,
                                 const COleDateTime& toDate )
{
   m_dateMin = fromDate;
   m_dateMax = toDate;

   // From MSDN: The COleDateTime class handles dates
   // from 1 January 100 to 31 December 9999.
   COleDateTime minAllowedDate( 100, 1, 1, 0, 0, 0 );
   COleDateTime maxAllowedDate( 9999, 12, 31, 23, 59, 59 );
   COleDateTime dateEmpty;
   dateEmpty.SetTime( 0, 0, 0 );

   if ( m_dateMin == dateEmpty )
      m_dateMin = minAllowedDate;
   else
      m_dateMin = max( m_dateMin, minAllowedDate );

   if ( m_dateMax == dateEmpty )
      m_dateMax = maxAllowedDate;
   else
      m_dateMax = min( m_dateMax, maxAllowedDate );

   if ( m_hWnd )
   {
      Invalidate( );
   }
}

BOOL
ZMiniCalendar::IsDateValid( const COleDateTime& date ) const
{
   if ( date.GetStatus( ) == COleDateTime::invalid )
   {
      ASSERT( FALSE );
      return( FALSE );
   }

   COleDateTime dateEmpty;

   dateEmpty.SetTime( 0, 0, 0 );
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

BOOL
ZMiniCalendar::IsToday( COleDateTime &date ) const
{
   COleDateTime dateToday = COleDateTime::GetCurrentTime( );

   if ( date.GetMonth( ) == dateToday.GetMonth( ) &&
        date.GetYear( ) == dateToday.GetYear( ) &&
        date.GetDay( ) == dateToday.GetDay( ) )
   {
      return( TRUE );
   }

   return( FALSE );
}

int
ZMiniCalendar::DrawTodayButton( CDC &dc, int nY )
{
   CRect   rectClient;
   int     nRC = 0;
   CString csText = "Today";

   if ( m_bFontsCreated == FALSE )
      CreateFontObjects( );

   GetClientRect( rectClient );

   CFont *pOldFont = dc.SelectObject( m_DaysOfWeekFontInfo.m_pFont );
   dc.SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
   dc.SetTextColor( m_DaysOfWeekFontInfo.m_cColor );
   dc.SetBkMode( TRANSPARENT );

   CSize sizeExtent = dc.GetTextExtent( csText );
   int nButtonWidth = sizeExtent.cx + 12;
   int nButtonHeight = sizeExtent.cy + 6;

   int nX = (rectClient.Width( ) / 2) - (nButtonWidth / 2);

   if ( m_bShowTodayButton && m_bShowNoneButton )
      nX = (rectClient.Width( ) - ((nButtonWidth * 2) + 10)) / 2;

   if ( m_bShowTodayButton )
   {
      csText = "Today";
      dc.FillSolidRect( nX, nY, nButtonWidth, nButtonHeight,
                        ::GetSysColor( COLOR_BTNFACE ) );

      if ( m_bTodayTracking && m_bTodayDown )
      {
         dc.Draw3dRect( nX, nY, nButtonWidth, nButtonHeight,
                        ::GetSysColor( COLOR_BTNTEXT ),
                        ::GetSysColor( COLOR_BTNFACE ) );
         dc.Draw3dRect( nX + 1, nY + 1,
                        nButtonWidth - 2, nButtonHeight - 2,
                        ::GetSysColor( COLOR_3DSHADOW ),
                        ::GetSysColor( COLOR_WINDOW ) );

         dc.DrawText( csText, CRect( nX + 2, nY + 2,
                                     nX + nButtonWidth, nY + nButtonHeight ),
                      DT_VCENTER | DT_CENTER | DT_SINGLELINE );
      }
      else
      {
         dc.Draw3dRect( nX, nY, nButtonWidth, nButtonHeight,
                        ::GetSysColor( COLOR_BTNFACE ),
                        ::GetSysColor( COLOR_BTNTEXT ) );
         dc.Draw3dRect( nX + 1, nY +1, nButtonWidth - 2, nButtonHeight - 2,
                        ::GetSysColor( COLOR_WINDOW ),
                        ::GetSysColor( COLOR_3DSHADOW ) );

         dc.DrawText( csText, CRect( nX, nY, nX + nButtonWidth,
                                     nY + nButtonHeight ),
                      DT_VCENTER | DT_CENTER | DT_SINGLELINE );
      }

      m_rectScrollButtons[ eToday ].SetRect( nX, nY, nX + nButtonWidth,
                                             nY + nButtonHeight );

      nX += nButtonWidth;
      nX += 10;
   }

   if ( m_bShowNoneButton )
   {
      csText = "None";
      dc.FillSolidRect( nX, nY, nButtonWidth, nButtonHeight,
                        ::GetSysColor( COLOR_BTNFACE ) );

      if ( m_bNoneTracking && m_bNoneDown )
      {
         dc.Draw3dRect( nX, nY, nButtonWidth, nButtonHeight,
                        ::GetSysColor( COLOR_BTNTEXT ),
                        ::GetSysColor( COLOR_BTNFACE ) );
         dc.Draw3dRect( nX + 1, nY + 1, nButtonWidth - 2,
                        nButtonHeight - 2,
                        ::GetSysColor( COLOR_3DSHADOW ),
                        ::GetSysColor( COLOR_WINDOW ) );

         dc.DrawText( csText, CRect( nX + 2, nY + 2,
                                     nX + nButtonWidth, nY + nButtonHeight ),
                      DT_VCENTER | DT_CENTER | DT_SINGLELINE );
      }
      else
      {
         dc.Draw3dRect( nX, nY, nButtonWidth, nButtonHeight,
                        ::GetSysColor( COLOR_BTNFACE ),
                        ::GetSysColor( COLOR_BTNTEXT ) );
         dc.Draw3dRect( nX + 1, nY + 1, nButtonWidth - 2,
                        nButtonHeight - 2, ::GetSysColor( COLOR_WINDOW ),
                        ::GetSysColor( COLOR_3DSHADOW ) );

         dc.DrawText( csText, CRect( nX, nY, nX + nButtonWidth,
                      nY + nButtonHeight ),
                      DT_VCENTER | DT_CENTER | DT_SINGLELINE );
      }

      m_rectScrollButtons[ eNone ].SetRect( nX, nY, nX + nButtonWidth,
                                            nY + nButtonHeight );

      nX += nButtonWidth;
      nX += 10;
   }

   dc.SelectObject( pOldFont );

   return( nRC );
}

void
ZMiniCalendar::SetCurrentMonthAndYear( int nMonth, int nYear )
{
   // From MSDN: The COleDateTime class handles dates
   // from 1 January 100 to 31 December 9999.
   ASSERT( nYear >= 100 && nYear <= 9999 );
   ASSERT( nMonth >= 1 && nMonth <= zMONTHS_IN_YEAR );

   if ( nMonth >= 1 && nMonth <= zMONTHS_IN_YEAR )
      m_nCurrentMonth = nMonth;

   if ( nYear >= 100 && nYear <= 9999 )
      m_nCurrentYear = nYear;

   ReinitializeCells( );
}

void
ZMiniCalendar::SetHeaderFont( LPCTSTR cpcFont, int nSize,
                              BOOL bBold, BOOL bItalic,
                              BOOL bUnderline, COLORREF cColor )
{
   ASSERT( nSize > 0 );
   ASSERT( nSize <= 72 );
   ASSERT( cpcFont );
   ASSERT( AfxIsValidString( cpcFont ) );

   if ( nSize > 0 && nSize <= 72 && cpcFont && AfxIsValidString( cpcFont ) )
   {
      m_HeaderFontInfo.m_bBold = bBold;
      m_HeaderFontInfo.m_bItalic = bItalic;
      m_HeaderFontInfo.m_bUnderline = bUnderline;
      m_HeaderFontInfo.m_nFontSize = nSize;
      m_HeaderFontInfo.m_csFontName = cpcFont;
      m_HeaderFontInfo.m_cColor = cColor;

      // Make sure font object gets recreated.
      mDeleteInit( m_HeaderFontInfo.m_pFont );
      m_bFontsCreated = FALSE;
   }
}

void
ZMiniCalendar::SetDaysOfWeekFont( LPCTSTR cpcFont, int nSize,
                                  BOOL bBold, BOOL bItalic,
                                  BOOL bUnderline, COLORREF cColor )
{
   ASSERT( nSize > 0 );
   ASSERT( nSize <= 72 );
   ASSERT( cpcFont );
   ASSERT( AfxIsValidString( cpcFont ) );

   if ( nSize > 0 && nSize <= 72 && cpcFont && AfxIsValidString( cpcFont ) )
   {
      m_DaysOfWeekFontInfo.m_bBold = bBold;
      m_DaysOfWeekFontInfo.m_bItalic = bItalic;
      m_DaysOfWeekFontInfo.m_bUnderline = bUnderline;
      m_DaysOfWeekFontInfo.m_nFontSize = nSize;
      m_DaysOfWeekFontInfo.m_csFontName = cpcFont;
      m_DaysOfWeekFontInfo.m_cColor = cColor;

      // Make sure font object gets recreated.
      mDeleteInit( m_DaysOfWeekFontInfo.m_pFont );
      m_bFontsCreated = FALSE;
   }
}

void
ZMiniCalendar::SetDaysFont( LPCTSTR cpcFont, int nSize, BOOL bBold,
                            BOOL bItalic, BOOL bUnderline,
                            COLORREF cColor )
{
   ASSERT( nSize > 0 );
   ASSERT( nSize <= 72 );
   ASSERT( cpcFont );
   ASSERT( AfxIsValidString( cpcFont ) );

   if ( nSize > 0 && nSize <= 72 && cpcFont && AfxIsValidString( cpcFont ) )
   {
      m_DaysFontInfo.m_bBold = bBold;
      m_DaysFontInfo.m_bItalic = bItalic;
      m_DaysFontInfo.m_bUnderline = bUnderline;
      m_DaysFontInfo.m_nFontSize = nSize;
      m_DaysFontInfo.m_csFontName = cpcFont;
      m_DaysFontInfo.m_cColor = cColor;

      // make sure font object gets recreated
      mDeleteInit( m_DaysFontInfo.m_pFont );
      m_bFontsCreated = FALSE;
   }
}

void
ZMiniCalendar::SetSpecialDaysFont( LPCTSTR cpcFont, int nSize,
                                   BOOL bBold, BOOL bItalic,
                                   BOOL bUnderline, COLORREF cColor )
{
   ASSERT( nSize > 0 );
   ASSERT( nSize <= 72 );
   ASSERT( cpcFont );
   ASSERT( AfxIsValidString( cpcFont ) );

   if ( nSize > 0 && nSize <= 72 && cpcFont && AfxIsValidString( cpcFont ) )
   {
      m_SpecialDaysFontInfo.m_bBold = bBold;
      m_SpecialDaysFontInfo.m_bItalic = bItalic;
      m_SpecialDaysFontInfo.m_bUnderline = bUnderline;
      m_SpecialDaysFontInfo.m_nFontSize = nSize;
      m_SpecialDaysFontInfo.m_csFontName = cpcFont;
      m_SpecialDaysFontInfo.m_cColor = cColor;

      // Make sure font object gets recreated.
      mDeleteInit( m_SpecialDaysFontInfo.m_pFont );
      m_bFontsCreated = FALSE;
   }
}

// Computes the size of the special 3d border this control
// draws (when option is set).
// I draw this myself because I could not find a standard
// border-style which closely mimicked the MS Outlook controls
// look and feel
CSize
ZMiniCalendar::Compute3DBorderSize( )
{
   // MENTAL NOTE: Why am I hard-coding this?
   CSize sizeReturn( 6, 6 );
   return( sizeReturn );
}

// determine height of today/none buttons
int
ZMiniCalendar::ComputeTodayNoneHeight( )
{
   int nHeight = 0;

   if ( m_bShowTodayButton || m_bShowNoneButton )
   {
      CFont     *pOldFont = 0;
      CClientDC *pDC = 0;

      if ( m_bFontsCreated == FALSE )
         CreateFontObjects( );

      // Allocate a DC to use when testing sizes, etc.
      if ( mIs_hWnd( GetSafeHwnd( ) ) )
         pDC = new CClientDC( this );
      else
         pDC = new CClientDC( AfxGetMainWnd( ) );

      ASSERT( pDC );

      if ( pDC == 0 )
         throw( new CMemoryException( ) );

      // Get current font and save for later restoration.
      pOldFont = pDC->GetCurrentFont( );

      // Now, compute height of today button.
      pDC->SelectObject( m_DaysOfWeekFontInfo.m_pFont );
      nHeight = pDC->GetTextExtent( "Today" ).cy + 8 + 7;

      // Cleanup DC.
      pDC->SelectObject( pOldFont );

      // Destroy DC.
      delete( pDC );
   }

   return( nHeight );
}

// Determine size of a given cell based on actual font settings.
CSize
ZMiniCalendar::ComputeSize( )
{
   CSize sizeReturn( 0, 0 );
   int nXDaysSpaceFromBorder = 15;
   int nHeaderSpaceForBorder = 15;
   int nHeaderYSpaceForBorder = 3;
   int nDaysXSpacing = 5;
   int nDaysYSpacing = 2;
   int nWidthByDaysIndividual = 0;
   int nWidthByDays = 0;
   int nWidthByDaysOfWeekIndividual = 0;
   int nWidthByDaysOfWeek = 0;
   int nWidthByHeader = 0;
   int nHeaderHeight = 0;
   int nDaysOfWeekHeight = 0;
   int nDaysHeight = 0;
   int nTodayHeight = 0;
   int nTotalWidth = 0;
   int nTotalHeight = 0;
   CFont *pOldFont = 0;
   CClientDC *pDC = 0;
   int nX;

   if ( m_bFontsCreated == FALSE )
      CreateFontObjects( );

   // Allocate a DC to use when testing sizes, etc.
   if ( mIs_hWnd( GetSafeHwnd( ) ) )
      pDC = new CClientDC( this );
   else
      pDC = new CClientDC( AfxGetMainWnd( ) );

   ASSERT( pDC );

   if ( pDC == 0 )
      throw( new CMemoryException( ) );

   // Get current font and save for later restoration.
   pOldFont = pDC->GetCurrentFont( );

   // First, use days to determine width.
   // NOTE: typically, most fonts use the same pixel sizes for all numbers,
   // but this is not mandatory and many "fancy" fonts change this.  To
   // compensate for this, calculate the width of all possible dates the
   // control will display.
   pDC->SelectObject( m_DaysFontInfo.m_pFont );
   for ( nX = 1; nX <= 31; nX++ )
   {
      CString csTemp;
      csTemp.Format( "00%d", nX );
      csTemp = csTemp.Right( 2 );

      CSize szTemp = pDC->GetTextExtent( csTemp );
      if ( szTemp.cx > nWidthByDays )
         nWidthByDays = szTemp.cx;

      if ( szTemp.cy > nDaysHeight )
         nDaysHeight = szTemp.cy;
   }

   // Make sure to try it with the special days font.
   pDC->SelectObject( m_SpecialDaysFontInfo.m_pFont );
   for ( nX = 1; nX <= 31; nX++ )
   {
      CString csTemp;
      csTemp.Format( "00%d", nX );
      csTemp = csTemp.Right( 2 );

      CSize sizeTemp = pDC->GetTextExtent( csTemp );
      if ( sizeTemp.cx > nWidthByDays )
         nWidthByDays = sizeTemp.cx;

      if ( sizeTemp.cy > nDaysHeight )
         nDaysHeight = sizeTemp.cy;
   }

   // Finish computation.
   nWidthByDaysIndividual = nWidthByDays;
   m_nDaysHeight = nDaysHeight;
   m_nIndividualDayWidth = nWidthByDaysIndividual;

   nWidthByDays = (nWidthByDays * 7) + (nDaysXSpacing * 6) +
                                                 (nXDaysSpaceFromBorder * 2);
   nDaysHeight = (nDaysHeight * 6) + (nDaysYSpacing * 6);

   // Next, use days of week to determine width and height.
   // Here again, test each variant.
   pDC->SelectObject( m_DaysOfWeekFontInfo.m_pFont );

   nWidthByDaysOfWeek = pDC->GetTextExtent( "S" ).cx;
   if ( pDC->GetTextExtent( "M" ).cx > nWidthByDaysOfWeek )
      nWidthByDaysOfWeek = pDC->GetTextExtent( "M" ).cx;

   if ( pDC->GetTextExtent( "T" ).cx > nWidthByDaysOfWeek )
      nWidthByDaysOfWeek = pDC->GetTextExtent( "T" ).cx;

   if ( pDC->GetTextExtent( "W" ).cx > nWidthByDaysOfWeek )
      nWidthByDaysOfWeek = pDC->GetTextExtent( "W" ).cx;

   if ( pDC->GetTextExtent( "F" ).cx > nWidthByDaysOfWeek )
      nWidthByDaysOfWeek = pDC->GetTextExtent( "F" ).cx;

   nDaysOfWeekHeight = pDC->GetTextExtent( "S" ).cy;
   if ( pDC->GetTextExtent( "M" ).cy > nDaysOfWeekHeight )
      nDaysOfWeekHeight = pDC->GetTextExtent( "M" ).cy;

   if ( pDC->GetTextExtent( "T" ).cy > nDaysOfWeekHeight )
      nDaysOfWeekHeight = pDC->GetTextExtent( "T" ).cy;

   if ( pDC->GetTextExtent( "W" ).cy > nDaysOfWeekHeight )
      nDaysOfWeekHeight = pDC->GetTextExtent( "W" ).cy;

   if ( pDC->GetTextExtent( "F" ).cy > nDaysOfWeekHeight )
      nDaysOfWeekHeight = pDC->GetTextExtent( "F" ).cy;

   // Finish computation.
   nWidthByDaysOfWeekIndividual = nWidthByDaysOfWeek;
   nWidthByDaysOfWeek = (nWidthByDaysOfWeek * 7) + (nDaysXSpacing * 6) +
                                                 (nXDaysSpaceFromBorder * 2);

   if ( nWidthByDaysOfWeekIndividual > m_nIndividualDayWidth )
      m_nIndividualDayWidth = nWidthByDaysOfWeekIndividual;

   m_nDaysOfWeekHeight = nDaysOfWeekHeight;

   // Next compute width and height of header (month name and year)
   // again, because of font variations we will use a 20 year window and
   // attempt the calculation with every month name.
   pDC->SelectObject( m_HeaderFontInfo.m_pFont );
   for ( int nYear = m_nCurrentYear - 10;
         nYear <= m_nCurrentYear + 10;
         nYear++ )
   {
      for ( int nMonth = 1; nMonth <= zMONTHS_IN_YEAR; nMonth++ )
      {
         CString csTest;

         csTest.Format( "%s %d", GetMonthName( nMonth ), nYear );

         if ( pDC->GetTextExtent( csTest ).cx > nWidthByHeader )
            nWidthByHeader = pDC->GetTextExtent( csTest ).cx;

         if ( pDC->GetTextExtent( csTest ).cy > nHeaderHeight )
            nHeaderHeight = pDC->GetTextExtent( csTest ).cy;
      }
   }

   // Finish computation.
   nWidthByHeader = nWidthByHeader + (nHeaderSpaceForBorder * 2);
   nHeaderHeight = nHeaderHeight + (nHeaderYSpaceForBorder * 2);
   m_nHeaderHeight = nHeaderHeight;

   // Cleanup DC.
   pDC->SelectObject( pOldFont );

   // Destroy DC.
   delete( pDC );

   // NOW, adjust return size if needed
   // start with widths (these are easy at this point)
   nTotalWidth = nWidthByHeader;
   if ( nWidthByDaysOfWeek > nTotalWidth )
      nTotalWidth = nWidthByDaysOfWeek;

   if ( nWidthByDays > nTotalWidth )
      nTotalWidth = nWidthByDays;

   if ( nTotalWidth > sizeReturn.cx )
      sizeReturn.cx = nTotalWidth;

   // Adjust heights.
   nTotalHeight = nHeaderHeight + (nDaysOfWeekHeight + 3) + 1 +
                                      nDaysHeight + 3 + 1 + 4 + nTodayHeight;
   if ( nTotalHeight > sizeReturn.cy )
      sizeReturn.cy = nTotalHeight;

   m_sizeMonth = sizeReturn;
   m_bSizeComputed = TRUE;

   return( sizeReturn );
}

// Determine total size of control (all rows/cols + border + none/today).
CSize
ZMiniCalendar::ComputeTotalSize( )
{
   CSize size = ComputeSize( );
   size.cx *= m_nCols;
   size.cy *= m_nRows;
   size.cy += ComputeTodayNoneHeight( );

   if ( m_bShow3dBorder )
      size += Compute3DBorderSize( );

   return( size );
}

// Create the CFont GDI objects.
void
ZMiniCalendar::CreateFontObjects( )
{
   CClientDC *pDC = 0;

   // Allocate a DC to use when testing sizes, etc.
   if ( mIs_hWnd( GetSafeHwnd( ) ) )
      pDC = new CClientDC( this );
   else
      pDC = new CClientDC( AfxGetMainWnd( ) );

   ASSERT( pDC );
   if ( pDC == 0 )
      throw( new CMemoryException( ) );

   m_HeaderFontInfo.CreateFont( pDC );
   m_DaysOfWeekFontInfo.CreateFont( pDC );
   m_DaysFontInfo.CreateFont( pDC );
   m_SpecialDaysFontInfo.CreateFont( pDC );

   delete( pDC );

   m_bFontsCreated = TRUE;
}

void
ZMiniCalendar::OnDevModeChange( LPTSTR lpDeviceName )
{
   CWnd::OnDevModeChange( lpDeviceName );

   RedrawWindow( );
}

void
ZMiniCalendar::OnFontChange( )
{
   CWnd::OnFontChange( );

   RedrawWindow( );
}

void
ZMiniCalendar::OnPaletteChanged( CWnd *pFocusWnd )
{
   CWnd::OnPaletteChanged( pFocusWnd );

   RedrawWindow( );
}

void
ZMiniCalendar::OnSettingChange( UINT uFlags,
                                LPCTSTR lpszSection )
{
   CWnd::OnSettingChange( uFlags, lpszSection );

   RedrawWindow( );
}

void
ZMiniCalendar::OnTimeChange( )
{
   CWnd::OnTimeChange( );
   RedrawWindow( );
}

void
ZMiniCalendar::OnSysColorChange( )
{
   CWnd::OnSysColorChange( );
   RedrawWindow( );
}

void
ZMiniCalendar::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pDayTimer && m_pDayTimer->m_pAgenda )
      m_pDayTimer->m_pAgenda->SetFocus( );
}

void
ZMiniCalendar::OnKillFocus( CWnd *pWndGetFocus )
{
// ASSERT( m_pParent );
   if ( m_nHeaderTimerID )
      KillTimer( m_nHeaderTimerID );

   if ( GetCapture( ) == this )
      ReleaseCapture( );

#if 0
   CRect rect;
   if ( GetButtonRect( m_nTrackedScrollButton, rect ) )
   {
      rect.InflateRect( 3, 3 );
      InvalidateRect( rect );
   }

   m_nStartMultipleSelection = zNO_HIT;
   m_nLastMultipleSelection = zNO_HIT;
   m_nPressedScrollButton = zNO_HIT;
   m_nTrackedScrollButton = zNO_HIT;

// HideDropCalendar( );
#else
   Default( );
#endif
}

// Clear all day hot-spots in all month windows.
void
ZMiniCalendar::ClearAllDaySpots( )
{
   for ( int nMonthCell = 0; nMonthCell < m_nMonthCells; nMonthCell++ )
   {
      for ( int nDaySpot = 0; nDaySpot < zDAY_SPOTS_IN_MONTH; nDaySpot++ )
      {
         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].
                                     m_date.SetStatus( COleDateTime::valid );
         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].
                                                m_rect.SetRect( 0, 0, 0, 0 );
         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_nDaySpot = 0;
      }
   }
}

void
ZMiniCalendar::ScrollLeft( int nCnt )
{
   ASSERT( nCnt > 0 && nCnt < 100 );

   for ( int nTry = 1; nTry <= nCnt; nTry++ )
   {
      CWaitCursor wait;
      ClearAllDaySpots( );

      for ( int nX = 1; nX <= (m_nRows * m_nCols); nX++ )
      {
         m_nCurrentMonth--;
         if ( m_nCurrentMonth < 1 )
         {
            m_nCurrentMonth = zMONTHS_IN_YEAR;
            m_nCurrentYear--;
         }
      }
   }

   ReinitializeCells( );
   RedrawWindow( );
}

void
ZMiniCalendar::ScrollRight( int nCnt )
{
   ASSERT( nCnt > 0 && nCnt < 100 );

   for ( int nTry = 1; nTry <= nCnt; nTry++ )
   {
      CWaitCursor wait;
      ClearAllDaySpots( );

      for ( int nX = 1; nX <= (m_nRows * m_nCols); nX++ )
      {
         m_nCurrentMonth++;
         if ( m_nCurrentMonth > zMONTHS_IN_YEAR )
         {
            m_nCurrentMonth = 1;
            m_nCurrentYear++;
         }
      }
   }

   ReinitializeCells( );
   RedrawWindow( );
}

#if 0
COleDateTime
ZMiniCalendar::GetDate( )
{
   if ( m_bMultipleSel )
   {
      COleDateTime dateReturn;
      dateReturn.SetStatus( COleDateTime::invalid );
      return( dateReturn );
   }
   else
   {
      COleDateTime dateReturn;
      dateReturn = m_dateSelBegin;
      return( dateReturn );
   }
}

void
ZMiniCalendar::GetDateSel( COleDateTime& dateBegin,
                           COleDateTime& dateEnd )
{
   dateBegin = m_dateSelBegin;
   dateEnd = m_dateSelEnd;
}
#endif

void
ZMiniCalendar::FireNoneButton( )
{
   while ( m_nInPaint > 0 )
      SysWait( 10 );

   COleDateTime dateToday = COleDateTime::GetCurrentTime( );
   m_nCurrentMonth = dateToday.GetMonth( );
   m_nCurrentYear = dateToday.GetYear( );

   ClearAllSelectedDays( );
   RedrawWindow( );
// FireNotifyClick( );
}

void
ZMiniCalendar::FireTodayButton( )
{
   while ( m_nInPaint > 0 )
      SysWait( 10 );

   COleDateTime dateToday = COleDateTime::GetCurrentTime( );
   int nMonth = dateToday.GetMonth( );
   int nDay = dateToday.GetDay( );
   int nYear = dateToday.GetYear( );

// dateToday.SetStatus( COleDateTime::invalid );
   dateToday.SetDate( nYear, nMonth, nDay );
   if ( dateToday.GetStatus( ) == COleDateTime::invalid )
      ASSERT( FALSE );

   m_nCurrentMonth = dateToday.GetMonth( );
   m_nCurrentYear = dateToday.GetYear( );

   ClearAllSelectedDays( );
   ReinitializeCells( );

#if 0 //dks
   m_dateSelBegin = dateNow;
   m_dateSelEnd = dateNow;
#endif

   RedrawWindow( );
// FireNotifyClick( );
}

void
ZMiniCalendar::FireNotifyClick( )
{
   NMHDR NotifyArea;

   NotifyArea.code = NM_CLICK;
   NotifyArea.hwndFrom = m_hWnd;
   NotifyArea.idFrom = ::GetDlgCtrlID( m_hWnd );

   GetParent( )->SendMessage( WM_NOTIFY, ::GetDlgCtrlID( m_hWnd ),
                              (WPARAM) &NotifyArea );
}

void
ZMiniCalendar::FireNotifyDblClick( )
{
   NMHDR NotifyArea;

   NotifyArea.code = NM_DBLCLK;
   NotifyArea.hwndFrom = m_hWnd;
   NotifyArea.idFrom = ::GetDlgCtrlID( m_hWnd );

   GetParent( )->SendMessage( WM_NOTIFY, ::GetDlgCtrlID( m_hWnd ),
                              (WPARAM) &NotifyArea );
}

void
ZMiniCalendar::SetRowsAndColumns( int nRows, int nCols )
{
   // MENTAL NOTE: I am still debating whether or not to set a max #
   // of rows/columns it seems like it would make sense, but I don't
   // know what to make it so I am making it rediculously? large.

   ASSERT( nRows > 0 && nRows < 100 );
   ASSERT( nCols > 0 && nCols < 100 );

   if ( nRows > 0 && nCols > 0 && nRows < 100 && nCols < 100 )
   {
      m_nRows = nRows;
      m_nCols = nCols;

      // Allocates and then sets m_pMiniCal, m_nRow, m_nCol for each cell.
      AllocateCells( );

      if ( m_bUseAutoSettings )
         AutoConfigure( );
   }
}

void
ZMiniCalendar::SetCellPosition( int nMonthRow, int nMonthCol, RECT rect,
                                int nYear, int nMonth, int nPart )
{
   ASSERT( nMonthRow > 0 && nMonthRow <= m_nRows );
   ASSERT( nMonthCol > 0 && nMonthCol <= m_nCols );

   int nMonthCell = MonthCellFromRowCol( nMonthRow, nMonthCol );
   ASSERT( nMonthCell >= 0 && nMonthCell < m_nMonthCells );

   if ( nMonthCell >= 0 && nMonthCell < m_nMonthCells )
   {
      if ( nPart == 1 )
         m_pMonthCells[ nMonthCell ].m_rectHeader = rect;
      else
      if ( nPart == 2 )
         m_pMonthCells[ nMonthCell ].m_rectWeekdayHeader = rect;
      else
    // if ( nPart == 3 )
         m_pMonthCells[ nMonthCell ].m_rect = rect;

      m_pMonthCells[ nMonthCell ].m_nYear = nYear;
      m_pMonthCells[ nMonthCell ].m_nMonth = nMonth;
   }
}

// MonthCell from Row/Col conversion.
int
ZMiniCalendar::MonthCellFromRowCol( int nMonthRow, int nMonthCol )
{
   int nMonthCell = (((nMonthRow - 1) * m_nCols) + nMonthCol) - 1;
   return( nMonthCell );
}

void
ZMiniCalendar::ReinitializeCells( )
{
   // From MSDN: The COleDateTime class handles dates
   // from 1 January 100 to 31 December 9999.
   ASSERT( m_nCurrentYear >= 100 && m_nCurrentYear <= 9999 );
   ASSERT( m_nCurrentMonth >= 1 && m_nCurrentMonth <= zMONTHS_IN_YEAR );

   COleDateTime date( m_nCurrentYear, m_nCurrentMonth, 1, 0, 0, 0 );
   date.SetStatus( COleDateTime::valid );

   for ( int nMonthCell = 0; nMonthCell < m_nMonthCells; nMonthCell++ )
   {
      date.SetDate( m_nCurrentYear, m_nCurrentMonth + nMonthCell, 1 );
      date.SetStatus( COleDateTime::valid );
      while ( date.GetDayOfWeek( ) != m_nFirstDayOfWeek )  // get to non-month day
      {
         date -= 1;
      }

      m_pMonthCells[ nMonthCell ].m_nDayOfWeekMonthBegins = zNO_HIT;
      for ( int nDaySpot = 0; nDaySpot < zDAY_SPOTS_IN_MONTH; nDaySpot++ )
      {
         if ( date.GetDay( ) == 1 && m_pMonthCells[ nMonthCell ].m_nDayOfWeekMonthBegins == zNO_HIT )
            m_pMonthCells[ nMonthCell ].m_nDayOfWeekMonthBegins = nDaySpot;

         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_bSelected = FALSE;
         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_rect.SetRect( 0, 0, 0, 0 );
         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_nDaySpot = nDaySpot;
         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_date = date;
         m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].
                                       m_date.SetStatus( COleDateTime::valid );

         if ( date.GetMonth( ) == m_nCurrentMonth + nMonthCell )
         {
            m_pMonthCells[ nMonthCell ].m_nDaysInMonth = date.GetDay( );
            m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_bNonMonth = FALSE;
         }
         else
            m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_bNonMonth = TRUE;

         date += 1;
      }

      while ( date.GetDay( ) > 1 )  // reset to beginning of month
         date -= 1;
   }
}

void
TraceDate( zCPCHAR cpcMessage, COleDateTime& date, zBOOL bDate )
{
   if ( bDate )
      TraceLine( "%s: Year: %d,  Month: %d,  Day: %d",
                 cpcMessage, date.GetYear( ), date.GetMonth( ), date.GetDay( ) );
   else
      TraceLine( "%s: Hour: %d,  Minute: %d,  Second: %d",
                 cpcMessage, date.GetHour( ), date.GetMinute( ), date.GetSecond( ) );
}

void
ZMiniCalendar::SetDaySpot( int nMonthRow, int nMonthCol,
                           int nDaySpot, COleDateTime& date,
                           RECT rect )
{
   ASSERT( nMonthRow >= 0 && nMonthRow <= m_nRows );
   ASSERT( nMonthCol >= 0 && nMonthCol <= m_nCols );

   int nMonthCell = MonthCellFromRowCol( nMonthRow, nMonthCol );

   ASSERT( nMonthCell >= 0 && nMonthCell < m_nMonthCells );
   ASSERT( nDaySpot >= 0 && nDaySpot < zDAY_SPOTS_IN_MONTH );
   ASSERT( m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_nDaySpot == nDaySpot );
   if ( m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_date != date )
   {
      TraceDate( "ZMiniCalendar::SetDaySpot dates not equal",
                 m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_date, TRUE );
      TraceDate( "========================================>", date, TRUE );
      ASSERT( m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_date == date );
   }

   if ( nMonthCell >= 0 && nMonthCell < m_nMonthCells &&
        nDaySpot >= 0 && nDaySpot < zDAY_SPOTS_IN_MONTH )
   {
      m_pMonthCells[ nMonthCell ].m_pDaySpots[ nDaySpot ].m_rect = rect;
   }
}

void
ZMiniCalendar::AllocateCells( )
{
   int nMonthCells = m_nRows * m_nCols;

   if ( nMonthCells > 0 )
   {
      mDeleteInitA( m_pMonthCells );
      m_pMonthCells = new ZMiniCalendarMonthCell[ nMonthCells ];

      ASSERT( m_pMonthCells );
      if ( m_pMonthCells == 0 )
         throw( new CMemoryException( ) );

      m_nMonthCells = nMonthCells;
      while ( nMonthCells-- )
      {
         m_pMonthCells[ nMonthCells ].m_pMiniCal = this;
         m_pMonthCells[ nMonthCells ].m_nRow = (nMonthCells / m_nCols) + 1;
         m_pMonthCells[ nMonthCells ].m_nCol = (nMonthCells % m_nCols) + 1;
      }
   }
}

// Resize this window within its parent.
void
ZMiniCalendar::AutoSize( )
{
   if ( mIs_hWnd( m_hWnd ) )
   {
      CSize size = ComputeTotalSize( );
      SetWindowPos( 0, 0, 0, size.cx, size.cy, SWP_NOMOVE );
   }
}

// Auto-configue attempts to optimize the font size for the control
// based on 1) maximum final size of the control, 2) number of rows
// and columns, 3) screen resolution, 4) minimum allowed font size.
void
ZMiniCalendar::AutoConfigure( )
{
   if ( mIs_hWnd( m_hWnd ) )
   {
      // First, determine the font sizes based on screen resolution.
      CString csFontName = m_csDefaultFontName;
      int nFontSize = FMC_DEFAULT_FONT_SIZE;

      if ( ::GetSystemMetrics( SM_CXSCREEN ) < 800 )
         nFontSize = FMC_DEFAULT_FONT_SIZE_640;
      else
      if ( ::GetSystemMetrics( SM_CXSCREEN ) < 1024 )
         nFontSize = FMC_DEFAULT_FONT_SIZE_800;
      else
      if ( ::GetSystemMetrics( SM_CXSCREEN ) < 1280 )
         nFontSize = FMC_DEFAULT_FONT_SIZE_1024;
      else
      if ( ::GetSystemMetrics( SM_CXSCREEN ) >= 1280 )
         nFontSize = FMC_DEFAULT_FONT_SIZE_1280;

      SetHeaderFont( csFontName, nFontSize, TRUE, FALSE,
                     FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
      SetDaysOfWeekFont( csFontName, nFontSize, FALSE, FALSE,
                         FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
      SetDaysFont( csFontName, nFontSize, FALSE, FALSE, FALSE,
                   ::GetSysColor( COLOR_BTNTEXT ) );
      SetSpecialDaysFont( csFontName, nFontSize, TRUE, FALSE,
                          FALSE, ::GetSysColor( COLOR_BTNTEXT ) );

      CSize sizeMax = m_sizeMax;
      CSize size = ComputeTotalSize( );

      if ( sizeMax != FMC_MAX_SIZE_NONE )
      {
         if ( sizeMax == FMC_MAX_SIZE_PARENT && GetParent( ) )
         {
            CRect rectParent;
            CRect rectMe;

            GetWindowRect( rectMe );
            GetParent( )->GetWindowRect( rectParent );
            GetParent( )->ScreenToClient( rectMe );

            sizeMax.cx = rectParent.Width( );
            sizeMax.cy = rectParent.Height( );

            sizeMax.cx -= rectMe.left;
            sizeMax.cy -= rectMe.top;
         }

         while ( (size.cx > sizeMax.cx || size.cy > sizeMax.cy) &&
                 nFontSize > m_nDefaultMinFontSize )
         {
            nFontSize--;

            SetHeaderFont( csFontName, nFontSize, TRUE, FALSE,
                           FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
            SetDaysOfWeekFont( csFontName, nFontSize, FALSE, FALSE,
                               FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
            SetDaysFont( csFontName, nFontSize, FALSE, FALSE, FALSE,
                         ::GetSysColor( COLOR_BTNTEXT ) );
            SetSpecialDaysFont( csFontName, nFontSize, TRUE, FALSE,
                                FALSE, ::GetSysColor( COLOR_BTNTEXT ) );
            size = ComputeTotalSize( );
         }
      }
   }

   AutoSize( );
}

void
ZMiniCalendar::SetMaxSize( SIZE sizeMax )
{
   m_sizeMax = sizeMax;
}

void
ZMiniCalendar::SetDefaultMinFontSize( int nValue )
{
   ASSERT( nValue >= 0 );
   ASSERT( nValue <= 72 );

   if ( nValue >= 0 && nValue <= 72 )
      m_nDefaultMinFontSize = nValue;
}

void
ZMiniCalendar::SetDefaultFont( LPCTSTR lpszValue )
{
   ASSERT( lpszValue );
   ASSERT( AfxIsValidString( lpszValue ) );

   if ( lpszValue && AfxIsValidString( lpszValue ) )
      m_csDefaultFontName = lpszValue;
}

void
ZMiniCalendar::SetMonthName( int nMonth, LPCTSTR cpcMonthName )
{
   ASSERT( nMonth > 0 && nMonth <= zMONTHS_IN_YEAR );
   ASSERT( cpcMonthName );
   ASSERT( AfxIsValidString( cpcMonthName ) );

   if ( nMonth > 0 && nMonth <= zMONTHS_IN_YEAR )
      m_csMonthNamesArray[ nMonth - 1 ] = cpcMonthName;
}

CString
ZMiniCalendar::GetMonthName( int nMonth )
{
   ASSERT( nMonth > 0 && nMonth <= zMONTHS_IN_YEAR );

   if ( nMonth > 0 && nMonth <= zMONTHS_IN_YEAR )
      return( m_csMonthNamesArray[ nMonth - 1 ] );
   else
      return( "" );
}

void
ZMiniCalendar::SetDayOfWeekName( int nDayOfWeek, LPCTSTR cpcDayName )
{
   ASSERT( nDayOfWeek > 0 && nDayOfWeek <= zDAYS_IN_WEEK );
   ASSERT( cpcDayName );
   ASSERT( AfxIsValidString( cpcDayName ) );

   if ( nDayOfWeek > 0 && nDayOfWeek <= zDAYS_IN_WEEK )
      m_csDaysOfWeekNamesArray[ nDayOfWeek - 1 ] = cpcDayName;
}

CString
ZMiniCalendar::GetDayOfWeekName( int nDayOfWeek )
{
   ASSERT( nDayOfWeek > 0 );
   ASSERT( nDayOfWeek <= zDAYS_IN_WEEK );

   if ( nDayOfWeek > 0 && nDayOfWeek <= zDAYS_IN_WEEK )
      return( m_csDaysOfWeekNamesArray[ nDayOfWeek - 1 ] );
   else
      return( CString( "" ) );
}

void
ZMiniCalendar::SetFirstDayOfWeek( int nDayOfWeek )
{
   ASSERT( nDayOfWeek > 0 );
   ASSERT( nDayOfWeek <= zDAYS_IN_WEEK );

   if ( nDayOfWeek > 0 && nDayOfWeek <= zDAYS_IN_WEEK )
   {
      m_nFirstDayOfWeek = nDayOfWeek;

      setlocale( LC_TIME, "" ); // should this be done here AND is it right???
      COleDateTime dateTemp = COleDateTime::GetCurrentTime( );

      // Find the specified day of the week.
      while ( dateTemp.GetDayOfWeek( ) != nDayOfWeek )
         dateTemp += 1;

      CString csName;
      for ( int nWeekday = 1; nWeekday <= zDAYS_IN_WEEK; nWeekday++ )
      {
         csName = dateTemp.Format( "%A" ).Left( 1 );
         SetDayOfWeekName( nWeekday, csName );
      // m_csDaysOfWeekNamesArray[ nWeekday ] = csName;
         dateTemp += 1;
      }
   }
}

int
ZMiniCalendar::GetFirstDayOfWeek( )
{
   return( m_nFirstDayOfWeek );
}

#if 0
void
ZMiniCalendar::SetDateSel( COleDateTime dateBegin,
                           COleDateTime dateEnd )
{
   if ( dateEnd.GetStatus( ) == COleDateTime::valid &&
        dateBegin.GetStatus( ) == COleDateTime::valid )
   {
      if ( dateEnd < dateBegin )
      {
         m_dateSelBegin.SetDate( dateEnd.GetYear( ), dateEnd.GetMonth( ),
                               dateEnd.GetDay( ) );
         m_dateSelEnd.SetDate( dateBegin.GetYear( ), dateBegin.GetMonth( ),
                               dateBegin.GetDay( ) );
      }
      else
      {
         m_dateSelBegin = dateBegin;
         m_dateSelEnd.SetDate( dateBegin.GetYear( ), dateBegin.GetMonth( ),
                               dateBegin.GetDay( ) );
      }
   }
   else
   {
      m_dateSelBegin.SetStatus( COleDateTime::invalid );
      m_dateSelEnd.SetStatus( COleDateTime::invalid );
   }
}

#endif

void
ZMiniCalendar::SetCurrentDate( COleDateTime date )
{
   if ( date.GetStatus( ) == COleDateTime::valid )
   {
      m_dateToday.SetDate( date.GetYear( ), date.GetMonth( ), date.GetDay( ) );

#if 0  // this should not need to happen???.
      if ( mIs_hWnd( m_hWnd ) )
      {
         Invalidate( );
         UpdateWindow( );
      }
#endif
   }
}

ZMiniCalendarMonthCell *
ZMiniCalendar::MonthHeaderHitTest( CPoint& pt )
{
   int nMonthCell;

   for ( nMonthCell = 0; nMonthCell < m_nMonthCells; nMonthCell++ )
   {
      if ( m_pMonthCells[ nMonthCell ].m_rectHeader.PtInRect( pt ) )
      {
      // TraceLineI( "MonthHeaderHitTest found Cell: ", nMonthCell );
         return( &(m_pMonthCells[ nMonthCell ]) );
      }
   }

   return( 0 );
}


ZMiniCalendarMonthCell *
ZMiniCalendar::MonthWeekdayHeaderHitTest( CPoint& pt )
{
   int nMonthCell;

   for ( nMonthCell = 0; nMonthCell < m_nMonthCells; nMonthCell++ )
   {
      if ( m_pMonthCells[ nMonthCell ].m_rectWeekdayHeader.PtInRect( pt ) )
      {
      // TraceLineI( "MonthWeekdayHeaderHitTest found Cell: ", nMonthCell );
         return( &(m_pMonthCells[ nMonthCell ]) );
      }
   }

   return( 0 );
}

ZMiniCalendarMonthCell *
ZMiniCalendar::MonthCellHitTest( CPoint& pt )
{
   int nMonthCell;

   for ( nMonthCell = 0; nMonthCell < m_nMonthCells; nMonthCell++ )
   {
      if ( m_pMonthCells[ nMonthCell ].m_rect.PtInRect( pt ) )
      {
      // TraceLineI( "MonthCellHitTest found Cell: ", nMonthCell );
         return( &(m_pMonthCells[ nMonthCell ]) );
      }
   }

   return( 0 );
}

BOOL
ZMiniCalendar::PreTranslateMessage( MSG *pMsg )
{
   if ( m_bHeaderTracking && m_pHeaderList )
      m_pHeaderList->ForwardMessage( pMsg );

   return( CWnd::PreTranslateMessage( pMsg ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ZMiniCalendarList
// Author:  Matt Gullett
//       gullettm@yahoo.com
// Copyright © 2001
//
// This is a companion control for use by the ZMiniCalendar.
// It is probably useless for any other purpose.
//
// You may freely use this source code in personal, freeware, shareware or
// commercial applications provided that 1) my name is recognized in the code
// and if this code represents a substantial portion of the application that
// my name be included in the credits for the application (about box, etc).
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Keith Rule for the CMemDC class.
// http://www.codeproject.com/gdi/flickerfree.asp
//

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarList

BEGIN_MESSAGE_MAP( ZMiniCalendarList, CWnd )
   //{{AFX_MSG_MAP( ZMiniCalendarList )
   ON_WM_ERASEBKGND( )
   ON_WM_PAINT( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZMiniCalendarList::ZMiniCalendarList( )
{
   COleDateTime dateToday = COleDateTime::GetCurrentTime( );

   SetCalendar( 0 );
   SetItemsPerPage( zDAYS_IN_WEEK );
   SetMiddleMonthYear( dateToday.GetMonth( ),
                       dateToday.GetYear( ) );
   SetBackColor( (COLORREF) ::GetSysColor( COLOR_WINDOW ) );
   SetTextColor( (COLORREF) ::GetSysColor( COLOR_BTNTEXT ) );

   SetBackColor( RGB( 255, 255, 255 ) );
   SetTextColor( RGB( 0, 0, 0 ) );

   m_nSelMonth = dateToday.GetMonth( );
   m_nSelYear = dateToday.GetYear( );

   m_nUpFactor = 0;
   m_nDownFactor = 0;
}

ZMiniCalendarList::~ZMiniCalendarList( )
{
}

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarList message handlers

BOOL
ZMiniCalendarList::OnEraseBkgnd( CDC *pDC )
{
   return( FALSE );
}

void
ZMiniCalendarList::OnPaint( )
{
   CPaintDC dc( this ); // device context for painting
   ZMemDC   memDC( &dc );
   CRect    rectClient;
   int nItem;

   // Calendar must be instantiated for this to work correctly.
   ASSERT( m_pWndCalendar );

   // If needed, create font.
   if ( m_FontInfo.m_pFont == 0 )
      m_FontInfo.CreateFont( &memDC );

   // Determine client area.
   GetClientRect( rectClient );

   // First, determine half of the items on the page.
   // Use this to reverse calculate the top item.
   int nHalfCount = m_nItemsPerPage / 2;
   if ( (nHalfCount * 2) > m_nItemsPerPage )
      nHalfCount--;

   int nMonth = m_nMiddleMonth;
   int nYear = m_nMiddleYear;

   for ( nItem = 0; nItem < nHalfCount; nItem++ )
   {
      nMonth--;
      if ( nMonth < 1 )
      {
         nMonth = zMONTHS_IN_YEAR;
         nYear--;
      }
   }

   // prepare DC
   // CFont *pOldFont = memDC.SelectObject( m_FontInfo.m_pFont );
   CFont *pOldFont =
                memDC.SelectObject( m_pWndCalendar->GetDaysFont( ).m_pFont );
   int nPosY = 0;
   int nPosX = 10;

   // Fill background.
   memDC.FillSolidRect( 0, 0, rectClient.Width( ),
                        rectClient.Height( ), m_clrBack );
   memDC.SetBkMode( TRANSPARENT );

   for ( nItem = 0; nItem < m_nItemsPerPage; nItem++ )
   {
      CString csDisplayText;
      CRect rectItem;

      if ( m_pWndCalendar )
         csDisplayText = m_pWndCalendar->GetMonthName( nMonth );

      CString cs;
      cs.Format( " %d", nYear );
      csDisplayText += cs;

      CSize szItem =
        memDC.GetTextExtent( "abcdefghijklmnopqrstuvwxyz"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" );

      rectItem.SetRect( nPosX, nPosY, nPosX + szItem.cx, nPosY + szItem.cy );

      if ( IsSelected( nItem, rectItem ) )
      {
         // This could be done better, but it is a very simple control so
         // just set the selected month/year right here in the draw code.
         m_nSelMonth = nMonth;
         m_nSelYear = nYear;

         memDC.SetTextColor( m_clrBack );
         memDC.SetBkColor( m_clrText );

         memDC.FillSolidRect( 0, nPosY, rectClient.Width( ),
                              szItem.cy, memDC.GetBkColor( ) );
      }
      else
      {
         memDC.SetTextColor( m_clrText );
         memDC.SetBkColor( m_clrBack );
      }

      memDC.DrawText( csDisplayText, rectItem,
                      DT_SINGLELINE | DT_LEFT | DT_TOP );

      nMonth++;
      if ( nMonth > zMONTHS_IN_YEAR )
      {
         nMonth = 1;
         nYear++;
      }

      nPosY += szItem.cy;
   }

   memDC.SelectObject( pOldFont );
}

void
ZMiniCalendarList::ForwardMessage( MSG *pMSG )
{
   if ( pMSG->message == WM_MOUSEMOVE || pMSG->message == WM_TIMER )
   {
      CRect rectClient;

      // Determine cursor position.
      CPoint pt( GetMessagePos( ) );

   // ScreenToClient( &pt );

      GetClientRect( rectClient );
      ClientToScreen( rectClient );

      if ( pt.y < rectClient.top )
      {
         int nDiff = rectClient.top - pt.y;

         if ( nDiff <= 15 )
            ScrollUp( 1 );
         else
            ScrollUp( 2 );
      }
      else
      if ( pt.y > rectClient.bottom )
      {
         int nDiff = pt.y - rectClient.bottom;

         if ( nDiff <= 15 )
            ScrollDown( 1 );
         else
            ScrollDown( 2 );
      }
      else
      {
         m_nUpFactor = 0;
         m_nDownFactor = 0;
      }

      RedrawWindow( );
   }
}

void
ZMiniCalendarList::SetMiddleMonthYear( int nMonth, int nYear )
{
   ASSERT( nYear >= 100 && nYear <= 9999 );
   ASSERT( nMonth >= 1 && nMonth <= zMONTHS_IN_YEAR );

   if ( nMonth >= 1 && nMonth <= zMONTHS_IN_YEAR )
      m_nMiddleMonth = nMonth;

   if ( nYear >= 100 && nYear <= 9999 )
      m_nMiddleYear = nYear;
}

void
ZMiniCalendarList::SetItemsPerPage( int nValue )
{
   // Require at least 3 items per page and the count must be odd (same
   // number of items on either side of middle).
   ASSERT( nValue >= 3 );
   ASSERT( fmod( (double) nValue, (double) 2 ) );

   if ( nValue >= 3 && fmod( (double) nValue, (double) 2 ) )
      m_nItemsPerPage = nValue;
}

void
ZMiniCalendarList::SetFont( LPCTSTR cpcFont, int nSize, BOOL bBold,
                                BOOL bItalic, BOOL bUnderline,
                                COLORREF cColor )
{
   ASSERT( nSize > 0 );
   ASSERT( nSize <= 72 );
   ASSERT( cpcFont );
   ASSERT( AfxIsValidString( cpcFont ) );

   if ( nSize > 0 && nSize <= 72 && cpcFont && AfxIsValidString( cpcFont ) )
   {
      m_FontInfo.m_bBold = bBold;
      m_FontInfo.m_bItalic = bItalic;
      m_FontInfo.m_bUnderline = bUnderline;
      m_FontInfo.m_nFontSize = nSize;
      m_FontInfo.m_csFontName = cpcFont;
      m_FontInfo.m_cColor = cColor;

      // Make sure font object gets recreated.
      mDeleteInit( m_FontInfo.m_pFont );
   }
}

void
ZMiniCalendarList::SetCalendar( ZMiniCalendar *pWnd )
{
   ASSERT( pWnd == 0 || mIs_hWnd( pWnd->GetSafeHwnd( ) ) );

   if ( pWnd == 0 || mIs_hWnd( pWnd->GetSafeHwnd( ) ) )
      m_pWndCalendar = pWnd;
}

BOOL
ZMiniCalendarList::IsSelected( int nX, CRect rectItem )
{
   BOOL bSelected = FALSE;
   CRect rectClient;

   // Determine cursor position.
   CPoint pt( GetMessagePos( ) );
   ScreenToClient( &pt );

   GetClientRect( rectClient );

   if ( pt.y < 0 && nX == 0 )
      bSelected = TRUE;
   else
   if ( pt.y > rectClient.Height( ) && nX == (m_nItemsPerPage - 1) )
      bSelected = TRUE;
   else
   if ( pt.y >= rectItem.top && pt.y <= rectItem.bottom - 1 )
      bSelected = TRUE;

   return( bSelected );
}

void
ZMiniCalendarList::AutoConfigure( )
{
   ASSERT( mIs_hWnd( GetSafeHwnd( ) ) );

   if ( mIs_hWnd( GetSafeHwnd( ) ) )
   {
      // Determine cursor position.
      CSize sizeMaxItem( 0,0 );

      CPoint pt( GetMessagePos( ) );
      CClientDC dc( this );

      // Calendar must be set for this to work correctly.
      ASSERT( m_pWndCalendar );

      // If needed, create font.
      if ( m_FontInfo.m_pFont == 0 )
         m_FontInfo.CreateFont( &dc );

      //CFont *pOldFont = dc.SelectObject( m_FontInfo.m_pFont );
      CFont *pOldFont =
              dc.SelectObject( m_pWndCalendar->GetDaysFont( ).m_pFont );

      sizeMaxItem.cy =
        dc.GetTextExtent( "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" ).cy;

      for ( int nMonth = 1; nMonth <= zMONTHS_IN_YEAR; nMonth++ )
      {
         CString csText;

         if ( m_pWndCalendar )
            csText = m_pWndCalendar->GetMonthName( nMonth );

         csText += " 0000";

         int nWidth = dc.GetTextExtent( csText ).cx;

         if ( nWidth > sizeMaxItem.cx )
            sizeMaxItem.cx = nWidth;
      }

      dc.SelectObject( pOldFont );

      // Calculate rectangle.
      CSize szCalendar( (sizeMaxItem.cx + 20),
                        ((sizeMaxItem.cy * m_nItemsPerPage) + 3) );

      int nLeftX = pt.x - (szCalendar.cx / 2);
      int nTopY = pt.y - (szCalendar.cy / 2);
      int nRightX = pt.x + (szCalendar.cx / 2);
      int nBottomY = pt.y + (szCalendar.cy / 2);

      SetWindowPos( 0, nLeftX, nTopY, (nRightX - nLeftX),
                    (nBottomY - nTopY), 0 );
      RedrawWindow( );
   }
}

void
ZMiniCalendarList::ScrollUp( int nCnt )
{
   m_nDownFactor = 0;
   m_nUpFactor += nCnt;

   if ( m_nUpFactor >= 10 )
   {
      for ( int nX = 0; nX < nCnt; nX++ )
      {
         if ( m_nMiddleYear == 100 )
            break;

         m_nMiddleMonth--;

         if ( m_nMiddleMonth < 1 )
         {
            m_nMiddleMonth = zMONTHS_IN_YEAR;
            m_nMiddleYear--;
         }
      }

      m_nUpFactor = 0;
   }
}

void
ZMiniCalendarList::ScrollDown( int nCnt )
{
   m_nUpFactor = 0;
   m_nDownFactor += nCnt;

   if ( m_nDownFactor >= 10 )
   {
      for ( int nX = 0; nX < nCnt; nX++ )
      {
         if ( m_nMiddleYear == 9999 )
            break;

         m_nMiddleMonth++;
         if ( m_nMiddleMonth > zMONTHS_IN_YEAR )
         {
            m_nMiddleMonth = 1;
            m_nMiddleYear++;
         }
      }

      m_nDownFactor = 0;
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//./ ADD NAME=DT_SetStartDateRowsCols
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_SetStartDateRowsCols
//                                          2003/04/29 Modified: 2003/04/29
//
//  PURPOSE:    This operation sets DayTimer Start Month and number of
//              Rows/Cols.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lFlag  - 0 or 3 ==> set Start Month and number of Rows/Cols
//                            1 ==> set Start Month
//                            2 ==> set number of Rows/Cols
//
//  RETURNS:    0 - Start Month and Rows/Cols successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zSHORT OPERATION
DT_SetStartDateRowsCols( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         zSHORT  nYear,
                         zSHORT  nMonth,
                         zSHORT  nRows,
                         zSHORT  nCols,
                         zLONG   lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer && pDayTimer->m_pMiniCal )
      {
         if ( lFlag == 0 || (lFlag & 0x00000001) )
            pDayTimer->m_pMiniCal->SetCurrentMonthAndYear( nMonth, nYear );

         if ( lFlag == 0 || (lFlag & 0x00000002) )
            pDayTimer->m_pMiniCal->SetRowsAndColumns( nRows, nCols );

         pDayTimer->m_pMiniCal->SetMaxSize( FMC_MAX_SIZE_PARENT );
         pDayTimer->m_pMiniCal->AutoConfigure( );
         pDayTimer->m_pMiniCal->ReinitializeCells( );

         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for DT_SetStartDateRowsCols ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=DT_SetTimeEntityAttributes
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_SetTimeEntityAttributes
//                                          2003/02/11 Modified: 2003/02/11
//
//  PURPOSE:    This operation sets DayTimer entities and attributes.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Entities/Attributes successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT zSHORT OPERATION
DT_SetTimeEntityAttributes( zVIEW   vSubtask,
                            zCPCHAR cpcCtrlTag,
                            zCPCHAR cpcTimeEntity,
                            zCPCHAR cpcSubjectAttribute,
                            zCPCHAR cpcMessageAttribute,
                            zCPCHAR cpcTimeFromAttribute,
                            zCPCHAR cpcTimeToAttribute,
                            zCPCHAR cpcAlarmAttribute,
                            zCPCHAR cpcAppointmentTypeAttribute,
                            zCPCHAR cpcFullDayAttribute )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer )
      {
         pDayTimer->SetTimeEntityAttributes( cpcTimeEntity,
                                             cpcSubjectAttribute,
                                             cpcMessageAttribute,
                                             cpcTimeFromAttribute,
                                             cpcTimeToAttribute,
                                             cpcAlarmAttribute,
                                             cpcAppointmentTypeAttribute,
                                             cpcFullDayAttribute );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for DT_SetTimeEntityAttributes ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=DT_NewAppointment
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_NewAppointment
//                                          2003/02/11 Modified: 2003/02/11
//
//  PURPOSE:    This operation creates a DayTimer appointment at the
//              point from the message (usually RButtonDown).
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:   -1 - Error locating control
//           non-zero - New appointment successfully created
//              0 - Appointment not created
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zLONG OPERATION
DT_NewAppointment( zVIEW   vSubtask,
                   zCPCHAR cpcCtrlTag,
                   zCPCHAR cpcSubject,
                   zCPCHAR cpcMessage,
                   zLONG   lAppointmentType,
                   zSHORT  bFullDay )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer )
      {
         zLONG lPoint = (zLONG) GetActionParameters( vSubtask );
         CPoint pt( LOWORD( lPoint ), HIWORD( lPoint ) );
         return( (zLONG) pDayTimer->NewAppointment( pt, cpcSubject,
                                                    cpcMessage,
                                                    lAppointmentType,
                                                    bFullDay ) );
      }

      TraceLineS( "drvr - Invalid control type for DT_NewAppointment ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=DT_UpdateCurrentAppointment
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_UpdateCurrentAppointment
//                                          2003/02/11 Modified: 2003/02/11
//
//  PURPOSE:    This updates the Zeidon information for an appointment
//              whose pointer was stored during the message.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Save successful
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
DT_UpdateCurrentAppointment( zVIEW   vSubtask,
                             zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer )
      {
         ZAppointment *pA = (ZAppointment *) GetActionParameters( vSubtask );
         if ( pA )
            pDayTimer->UpdateAppointment( pA );

         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for DT_UpdateAppointment ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=DT_DeleteAppointment
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_DeleteAppointment
//                                          2003/02/11 Modified: 2003/02/11
//
//  PURPOSE:    This deletes the an appointment whose pointer was
//              stored during the message.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Delete successful
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
DT_DeleteCurrentAppointment( zVIEW   vSubtask,
                             zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer )
      {
         ZAppointment *pA = (ZAppointment *) GetActionParameters( vSubtask );
         if ( pA )
            pDayTimer->DeleteAppointment( pA );

         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for DT_DeleteAppointment ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=DT_SetWorkday
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_SetWorkday
//                                          2003/02/11 Modified: 2003/02/11
//
//  PURPOSE:    This sets the workday hours.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Set successful
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
DT_SetWorkday( zVIEW   vSubtask,
               zCPCHAR cpcCtrlTag,
               zSHORT  nStartHour,
               zSHORT  nStartMinute,
               zSHORT  nEndHour,
               zSHORT  nEndMinute,
               zLONG   lWorkdayFlag ) // DayFlag 1+2+4+8+16+32+64 = SMTWTFS
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer && pDayTimer->m_pAgenda )
      {
         COleDateTime timeStart;
         COleDateTime timeEnd;

         timeStart.SetTime( nStartHour, nStartMinute, 0 );
         timeEnd.SetTime( nEndHour, nEndMinute, 0 );

         pDayTimer->m_pAgenda->SetWorkday( timeStart, timeEnd, lWorkdayFlag );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for DT_SetWorkday ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=DT_SetColorAssociation
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_SetColorAssociation
//                                          2003/02/11 Modified: 2003/02/11
//
//  PURPOSE:    This operation sets the associations between the value of
//              the (color control) attribute and the color to be displayed.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              lTextColor  - The RGB color in which text is displayed.
//              lBackColor  - The RGB color in which to display the background.
//              lFlag       - Flag - 1=Bold 2=Italic 4=Underline 8=Strikeout
//              cpcAttributeValue - The value of the attribute corresponding
//                                  to the specified color.
//
//  RETURNS:    0 - Attribute name set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
DT_SetColorAssociation( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag,
                        zLONG   lTextColor,
                        zLONG   lBackColor,
                        zLONG   lFlag,
                        zCPCHAR cpcAttributeValue )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer )
      {
         pDayTimer->SetColorAssociation( lTextColor, lBackColor,
                                         lFlag, cpcAttributeValue );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for DT_SetColorAssociation ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=DT_DisplayCurrentAppointment
// Source Module=zdctldt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: DT_DisplayCurrentAppointment
//                                          2003/02/11 Modified: 2003/02/11
//
//  PURPOSE:    This operation sets up a buffer with the appointment
//              values.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pchMsg      - Return message buffer.
//              lMsgMaxLth  - Maximum length of the return buffer.
//
//  RETURNS:    0 - Message set up successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
DT_DisplayCurrentAppointment( zVIEW   vSubtask,
                              zCPCHAR cpcCtrlTag,
                              zPCHAR  pchMsg,
                              zLONG   lMsgMaxLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   pchMsg[ 0 ] = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZDayTimer *pDayTimer = DYNAMIC_DOWNCAST( ZDayTimer, pzma->m_pCtrl );
      if ( pDayTimer )
      {
         ZAppointment *pA = (ZAppointment *) GetActionParameters( vSubtask );
         if ( pA )
         {
            CString csMsg;
            csMsg = pA->m_date.Format( "Appointment date: %A, %B %d, %Y " );
            if ( pA->m_bFullDay )
               csMsg += "Full Day";
            else
            {
               csMsg += FormatTimeAmPm( pA->m_timeFrom );
               csMsg += " - ";
               csMsg += FormatTimeAmPm( pA->m_timeTo );
            }

            csMsg += "  Subject: ";
            csMsg += pA->m_csSubject;
            csMsg += "  Message: ";
            csMsg += pA->m_csMessage;
            strncat_s( pchMsg, lMsgMaxLth, csMsg, lMsgMaxLth - 1 );
         }

         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for DT_DisplayCurrentAppointment ", cpcCtrlTag );
   }

   return( -1 );
}

AFX_EXT_API
CWnd * OPERATION                 // WINAPI
DayTimer( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZDayTimer( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}
