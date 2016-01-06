/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlrte.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Rich Text Edit control.
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
// 2000.03.17    TMV    WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlRte.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZRichEdit, CRichEditCtrl )

BEGIN_MESSAGE_MAP( ZRichEdit, CRichEditCtrl )
   ON_WM_CREATE( )
   ON_WM_SYSKEYDOWN( )
   ON_WM_SYSCHAR( )
   ON_WM_KEYDOWN( )
   ON_WM_CHAR( )
// ON_WM_HSCROLL_REFLECT( )
// ON_WM_VSCROLL_REFLECT( )
   ON_WM_HSCROLL( )
   ON_WM_VSCROLL( )
   ON_WM_PAINT( )
   ON_WM_PARENTNOTIFY_REFLECT( )
   ON_NOTIFY_REFLECT( EN_CHANGE, RTENotifyChange )
   ON_NOTIFY_REFLECT( EN_UPDATE, RTENotifyUpdate )
   ON_CONTROL_REFLECT( EN_CHANGE, RTENChange )
   ON_CONTROL_REFLECT( EN_ERRSPACE, RTENErrspace )
   ON_CONTROL_REFLECT( EN_MAXTEXT, RTENMaxtext )
   ON_CONTROL_REFLECT( EN_SETFOCUS, RTENSetFocus )
   ON_CONTROL_REFLECT( EN_KILLFOCUS, RTENKillFocus )
   ON_CONTROL_REFLECT( EN_UPDATE, RTENUpdate )
   ON_CONTROL_REFLECT( EN_REQUESTRESIZE, RTENRequestResize )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )

#define zRTE_KEYDOWN          0x0001
#define zRTE_CHARACTER        0x0002
#define zRTE_SYSKEYDOWN       0x0004
#define zRTE_SYSCHARACTER     0x0008
#define zRTE_PAINT            0x0010
#define zRTE_SETFOCUS         0x0020
#define zRTE_KILLFOCUS        0x0040
#define zRTE_HSCROLL          0x0100
#define zRTE_VSCROLL          0x0200
#define zRTE_CHANGE           0x0400
#define zRTE_UPDATE           0x0800
#define zRTE_REQUEST_RESIZE   0x8000

// ZRichEdit - ctor
ZRichEdit::ZRichEdit( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
           CRichEditCtrl( ),
           ZMapAct( pZSubtask,        // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "RichEdit" )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::ctor ", *m_pzsTag );
   if ( m_pzsVName )
   {
      TraceLineS( "ZRichEdit ViewName", *m_pzsVName );
      TraceLineS( "ZRichEdit Entity", *m_pzsEName );
      TraceLineS( "ZRichEdit Attribute", *m_pzsAName );
   }
#endif

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style &= ~WS_BORDER;
   else
      Attr.Style |= WS_BORDER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

   Attr.Style &= ~(WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL);
   Attr.Style |= ES_AUTOVSCROLL | WS_VSCROLL;
// Attr.Style |= ES_MULTILINE;
   Attr.Style |= ES_MULTILINE | ES_WANTRETURN;
   Attr.X = pCtrlDef->PSDLG_X;
   Attr.Y = pCtrlDef->PSDLG_Y;
   Attr.W = pCtrlDef->SZDLG_X;
   Attr.H = pCtrlDef->SZDLG_Y;
#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZRichEdit::CreateZ( )
{
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

   if ( m_ulMapActFlags & zMAPACT_ENABLED )
      Attr.Style &= ~WS_DISABLED;
   else
      Attr.Style |= WS_DISABLED;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( Attr.Style,
                 CRect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + Attr.H ),
                 m_pWndParent, m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZRichEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CRichEditCtrl::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );
   RequestResize( );
   SetEventMask( ENM_SELCHANGE | ENM_CHANGE | ENM_SCROLL | ENM_UPDATE );

   zVIEW   vApp = 0;
   zULONG  ulLth = 0;

   if ( m_pzsVName && m_pzsEName && m_pzsAName ) // painter ensures all???
   {
      if ( GetViewByName( &vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         GetAttributeDisplayLength( &ulLth, vApp, *m_pzsEName,
                                    *m_pzsAName, *m_pzsContext );

         if ( ulLth )
         {
            // TextLen OnCreate to limit text. It
            // holds the size of the text buffer for static (and
            // derived) controls.  Because of the null terminator
            // on the string, the number of characters that can
            // be stored in the static control is one less than ulLth.
            ulLth++;
         }
      }
   }

//   SetLimitText( ulLth );
   return( nRC );
}

BOOL
ZRichEdit::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CRichEditCtrl::DestroyWindow( ) );
// Default( );
}

// get the data from the OI and map it to the entry control
zSHORT
ZRichEdit::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::MapFromOI ", *m_pzsTag );
#endif

   return( ZMapAct::MapFromOI( wFlag ) );
}

// get the text for the entry control and map it to the data OI
zSHORT
ZRichEdit::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::MapToOI ", *m_pzsTag );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      zSHORT  nRC;
      CString cs;

      GetWindowText( cs );
      nRC = SetAttributeFromVariable( vApp, *m_pzsEName,
                                      *m_pzsAName, cs,
                                      zTYPE_STRING, 0,
                                      *m_pzsContext,
                                      m_pzsContext->IsEmpty( ) ?
                                       (zSHORT) (lFlag | zUSE_DEFAULT_CONTEXT) :
                                       (zSHORT) lFlag );
      return( nRC );
   }

   return( 0 );
}

zLONG
ZRichEdit::SetZCtrlState( zLONG lStatusType,
                          zULONG ulValue )
{
   if ( mIs_hWnd( m_hWnd ) == FALSE )
      return( 0 );

   // we want to do something different for multiline edit controls.
   // As opposed to edit controls, it is OK to set the state of the RTE
   // control to ReadOnly when it is disabled so that is can receive
   // focus and scroll as necessary.
   if ( lStatusType != zCONTROL_STATUS_ENABLED &&
        lStatusType != zCONTROL_STATUS_AUTOGRAY )
   {
      return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
   }

   zBOOL bEnabledPrev = (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;

   if ( lStatusType == zCONTROL_STATUS_ENABLED )
   {
      // shut off auto enabled flag ... user is overriding enabled status
      m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
      if ( ulValue )
         m_ulMapActFlags |= zMAPACT_ENABLED;
      else
         m_ulMapActFlags &= ~zMAPACT_ENABLED;
   }
   else
   if ( lStatusType == zCONTROL_STATUS_AUTOGRAY )
   {
      if ( ulValue )
      {
         // we can't auto-enable a control that was manually disabled
         if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
         {
            m_ulMapActFlags |= zMAPACT_ENABLED;
            m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
         }
      }
      else
      {
         // we can't auto-disable a control that was manually disabled
         if ( m_ulMapActFlags & zMAPACT_ENABLED )
         {
            m_ulMapActFlags &= ~zMAPACT_ENABLED;
            m_ulMapActFlags |= zMAPACT_AUTO_DISABLED;
         }
      }
   }

   zBOOL bEnabled = (m_ulMapActFlags & zMAPACT_ENABLED) ? TRUE : FALSE;
   if ( bEnabled == TRUE && bEnabledPrev == FALSE ||
        bEnabled == FALSE && bEnabledPrev == TRUE )
   {
#if 0
      CClientDC dc( *this );

      if ( bEnabled )
         dc.SetTextColor( rgbColorFg );
      else
         rgbColorFg = dc.SetTextColor( ::GetSysColor( COLOR_ACTIVECAPTION ) );
#endif
      bEnabled = !bEnabled;     // for SetReadOnly and for return value
#ifdef zREMOTE_SERVER
      SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly",
                               bEnabled ? "Y" : "N" );
#else
      SetReadOnly( bEnabled );
      Invalidate( TRUE );
#endif
   }

   return( bEnabled );
}

void
ZRichEdit::RTENChange( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENChange ", *m_pzsTag );
#endif

   Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_CHANGE );
}

void
ZRichEdit::RTENErrspace( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENErrspace ", *m_pzsTag );
#endif

   Default( );             // pass it along to parent
}

void
//ZRichEdit::HScroll( UINT nSBCode, UINT nPos )
ZRichEdit::OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::HScroll ", *m_pzsTag );
#endif

   Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_HSCROLL );
}

void
ZRichEdit::RTENMaxtext( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENMaxtext ", *m_pzsTag );
#endif

   Default( );             // pass it along to parent
}

void
ZRichEdit::RTENSetFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENSetFocus ", *m_pzsTag );
#endif

   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
      Default( );             // pass it along to parent
      ProcessEvent( this, zRTE_SETFOCUS );
   }
}

void
ZRichEdit::RTENKillFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENKillFocus ", *m_pzsTag );
#endif

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_KILLFOCUS );
}

void
ZRichEdit::ParentNotify( UINT uMessage, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineX( "ZRichEdit::ParentNotify ", uMessage );
#endif

   Default( );
}

void
ZRichEdit::RTENUpdate( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENUpdate ", *m_pzsTag );
#endif

//x???   return( ProcessEvent( this, uNotificationCode ) );
   Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_UPDATE );
}

void
//ZRichEdit::VScroll( UINT nSBCode, UINT nPos )
ZRichEdit::OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::VScroll ", *m_pzsTag );
#endif

   Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_VSCROLL );
}

void
ZRichEdit::RTENRequestResize( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENRequestResize ", *m_pzsTag );
#endif

   Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_REQUEST_RESIZE );
}

void
ZRichEdit::OnSysKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::OnSysKeyDown ", *m_pzsTag );
   TraceLineI( "       SysKeyDown key - ", uKey );
#endif

   CRichEditCtrl::OnSysKeyDown( uKey, uRepeatCnt, uFlags );
// Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_SYSKEYDOWN );
}

void
ZRichEdit::OnSysChar( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::OnSysChar ", *m_pzsTag );
#endif

// TraceLineI( "WMSysChar - ", Msg.WParam );
   CRichEditCtrl::OnSysChar( uKey, uRepeatCnt, uFlags );
// Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_SYSCHARACTER );
}

void
ZRichEdit::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   zLONG lFirst;
   zLONG lLast;
   TraceLineS( "ZRichEdit::OnKeyDown ", *m_pzsTag );
   GetSel( lFirst, lLast );
   TraceLineI( "               First ", lFirst );
   TraceLineI( "                Last ", lLast );
#endif

   TraceLineI( "ZRichEdit::OnKeyDown - ", uKey );
   if ( uKey == VK_TAB )
   {
// do special method in driver for this???
//      m_pZSubtask->m_pZFWnd->SendMessage( WM_COMMAND,
//                                          zPROCESS_ZEIDON_TAB, 0 );
   }

   CRichEditCtrl::OnKeyDown( uKey, uRepeatCnt, uFlags );
// Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_KEYDOWN );
}

void
ZRichEdit::OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   zLONG lFirst;
   zLONG lLast;
   TraceLineS( "ZRichEdit::OnChar ", *m_pzsTag );
   GetSel( lFirst, lLast );
   TraceLineI( "            First ", lFirst );
   TraceLineI( "             Last ", lLast );
#endif

// TraceLineI( "WMChar - ", Msg.WParam );
   CRichEditCtrl::OnChar( uKey, uRepeatCnt, uFlags );
// Default( );             // pass it along to parent
   ProcessEvent( this, zRTE_CHARACTER );
}

void
ZRichEdit::RTENotifyChange( NMHDR *pNotifyStruct, LRESULT *result )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENotifyChange ", *m_pzsTag );
#endif

   Default( );
   ProcessEvent( this, zRTE_CHANGE );
}

void
ZRichEdit::RTENotifyUpdate( NMHDR *pNotifyStruct, LRESULT *result )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::RTENotifyUpdate ", *m_pzsTag );
#endif

   Default( );
   ProcessEvent( this, zRTE_UPDATE );
}

void
ZRichEdit::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZRichEdit::RTEPaint ", *m_pzsTag );
#endif

   Default( );
   ProcessEvent( this, zRTE_PAINT );
}

void
ZRichEdit::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZRichEdit - dtor
ZRichEdit::~ZRichEdit( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRichEdit::dtor ", *m_pzsTag );
#endif

// if ( m_hEditDS )
//    GlobalFree( m_hEditDS );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
RTE( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZRichEdit( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}
