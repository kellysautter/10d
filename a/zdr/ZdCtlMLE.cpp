/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlmle.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Multi-line EditBox control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.01.15    BL
//    Modified HandleQuickKey for handle insert TAB in a MLE with key
//    CTRL+TAB
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.07.20    TMV
//    Implement setting Background color
//
// 2001.06.10    BL     WEB/Windows
//    Corrected max. input length of MLE.
//    On mouse click, do not select the content of a MLE.
//
// 2001.05.22    DKS    Z10
//    Activated the "change" and "update" events.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2001.01.03    DKS    Z10
//    Permit select of text in EditBox/MLE.
//
// 2000.03.17    TMV    WEB
//    Validate parent window's handle prior to call Create/CreateEx.
//
// 1999.07.22    DKS    Z10    QS999
//    Permit multi-line edit ctrl to be disabled in a "ReadOnly" mode.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1998.12.04    DKS
//    MLE now honors the UPPERCASE and LOWERCASE style flags.
//
// 1998.10.30    DKS
//    Set focus from an MLE now works within a Notebook control.
//
// 1998.10.19    DKS
//    MLE on a tab control stay disabled appropriately (TB 297) plus a
//    MLE that is disabled must be set to be "Read Only", not completely
//    disabled so that it can still be scrolled.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlMle.h"
#include "ZdCtlRte.h"
#include "ZdCtlGbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( ZMLE, CEdit )

BEGIN_MESSAGE_MAP( ZMLE, CEdit )
   ON_WM_CREATE( )
   ON_WM_CTLCOLOR_REFLECT( )
   ON_CONTROL_REFLECT( EN_CHANGE, MLENChange )
   ON_CONTROL_REFLECT( EN_ERRSPACE, MLENErrspace )
   ON_CONTROL_REFLECT( EN_HSCROLL, MLENHscroll )
   ON_CONTROL_REFLECT( EN_MAXTEXT, MLENMaxtext )
   ON_CONTROL_REFLECT( EN_SETFOCUS, MLENSetFocus )
   ON_CONTROL_REFLECT( EN_KILLFOCUS, MLENKillFocus )
   ON_CONTROL_REFLECT( EN_UPDATE, MLENUpdate )
   ON_CONTROL_REFLECT( EN_VSCROLL, MLENVscroll )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )


// ZMLE - ctor
ZMLE::ZMLE( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
      CEdit( ),
      ZMapAct( pZSubtask,        // base class ctor
               pzmaComposite,
               pWndParent,
               this,
               vDialog,
               nOffsetX,
               nOffsetY,
               pCtrlDef,
               "MLE" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for ZMLE", (zLONG) this );
   if ( m_pzsVName )
   {
      TraceLineS( "ZMLE ViewName", *m_pzsVName );
      TraceLineS( "ZMLE Entity", *m_pzsEName );
      TraceLineS( "ZMLE Attribute", *m_pzsAName );
   }
#endif

   m_ulLth = 0;
   if ( m_ulMapActFlags & zMAPACT_USE_SUBTASK_BRUSH )
   {
      mDeleteInit( m_pBrush );
      m_clrBk = 0;
   }

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
   Attr.Style |= ES_MULTILINE | ES_WANTRETURN;

   if ( pCtrlDef->Subtype & zEDITBOX_LOWERCASE )
      Attr.Style |= ES_LOWERCASE;

   if ( pCtrlDef->Subtype & zEDITBOX_UPPERCASE )
      Attr.Style |= ES_UPPERCASE;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZMLE::CreateZ( )
{
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   // The "disabled" property is set during OnCreate using the ReadOnly flag.
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( WS_EX_CLIENTEDGE, "EDIT", *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZMLE::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CEdit::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );

#ifdef zREMOTE_SERVER
   SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly",
                            (m_ulMapActFlags & zMAPACT_ENABLED) ? "N" : "Y" );
#else
   SetReadOnly( (m_ulMapActFlags & zMAPACT_ENABLED) ? FALSE : TRUE );
#endif

   zVIEW  vApp = 0;

   if ( m_pzsVName && m_pzsEName && m_pzsAName ) // painter ensures all???
   {
      if ( GetViewByName( &vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         if ( m_ulLth == 0 )
            GetAttributeDisplayLength( &m_ulLth, vApp, *m_pzsEName,
                                       *m_pzsAName, *m_pzsContext );
      }
   }

   LimitTextLength( m_ulLth );

   if ( m_ulMapActFlags & zMAPACT_DISABLE_READONLY )
      SetZCtrlState( zCONTROL_STATUS_DISABLE_READONLY, TRUE );

   return( nRC );
}

BOOL
ZMLE::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CEdit::DestroyWindow( ) );
}

// Get the data from the OI and map it to the entry control.
zSHORT
ZMLE::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MapFromOI: ", *m_pzsTag );
#endif

   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW  vApp = 0;

      if ( GetViewByName( &vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         if ( m_ulLth == 0 )
         {
            GetAttributeDisplayLength( &m_ulLth, vApp, *m_pzsEName,
                                       *m_pzsAName, *m_pzsContext );

            LimitTextLength( (zSHORT) m_ulLth );
         }
      }

   // if ( zstrcmp( *m_pzsTag, "MLEdit7" ) == 0 )
   //    DisplayEntityInstance( vApp, *m_pzsEName );
   }

   return( ZMapAct::MapFromOI( ) );
}

// Get the text for the entry control and map it to the data OI.
zSHORT
ZMLE::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MapToOI: ", *m_pzsTag );
   if ( m_pzsAName )
      TraceLineS( "In MapToOI for ANAME ==> ", *m_pzsAName );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

#ifdef zREMOTE_SERVER
      zCHAR   szMap[ 65534 ];

      GetZCtrlText( this, szMap, sizeof( szMap ) );
#else
      CString szMap;  // named szMap rather than csMap for SetAttribute below

      GetWindowText( szMap );
#endif
      zSHORT nRC =
         SetAttributeFromVariable( vApp, *m_pzsEName,
                                   *m_pzsAName, szMap,
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
ZMLE::SetZCtrlState( zLONG  lStatusType,
                     zULONG ulValue )
{
   // We want to do something different for multiline edit controls.
   // As opposed to edit controls, it is OK to set the state of the MLE
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
      // Shut off auto enabled flag ... user is overriding enabled status.
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
         // We can't auto-enable a control that was manually disabled.
         if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
         {
            m_ulMapActFlags |= zMAPACT_ENABLED;
            m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
         }
      }
      else
      {
         // We can't auto-disable a control that was manually disabled.
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
      bEnabled = !bEnabled;     // for SetReadOnly and for return value
#ifdef zREMOTE_SERVER
      SetRemoteZCtrlAttribute( this, "Ctrl", "Enabled",
                               bEnabled ? "N" : "Y" );
      SetRemoteZCtrlAttribute( this, "Ctrl", "ReadOnly",
                               bEnabled ? "Y" : "N" );
#else
      if ( mIs_hWnd( m_hWnd ) )
      {
         SetReadOnly( bEnabled );
         Invalidate( TRUE );
      }
#endif
   }

   return( bEnabled );
}

void
ZMLE::MLENChange( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MLENChange: ", *m_pzsTag );
#endif
   ProcessEvent( this, 1024 );
   Default( );             // pass it along to parent
   if ( (m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI) &&
        (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SELECT_FIRST_EDIT) )
   {
      SetSel( 0, -1 );
   }
}

void
ZMLE::MLENErrspace( )
{
   Default( );             // pass it along to parent
}

void
ZMLE::MLENHscroll( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MLENHscroll: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

void
ZMLE::MLENMaxtext( )
{
   Default( );             // pass it along to parent
}

void
ZMLE::MLENSetFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MLENSetFocus: ", *m_pzsTag );
#endif
   ProcessEvent( this, 32 );
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
      Default( );             // pass it along to parent
   }
}

void
ZMLE::MLENKillFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MLENKillFocus: ", *m_pzsTag );
#endif

   ZMapAct *pzmaGetFocus = 0;
   CWnd *pWnd = GetFocus( );
   if ( pWnd && pWnd->m_hWnd != m_hWnd )
   {
      // Attempt to locate the child window using the list of Zeidon MapActs
      zLONG k = m_pZSubtask->m_pZMIXCtrl->GetCount( );
      while ( k )
      {
         pzmaGetFocus = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( k );
         if ( pzmaGetFocus && pzmaGetFocus->m_pCtrl &&
              pzmaGetFocus->m_pCtrl->m_hWnd == pWnd->m_hWnd )
         {
            break;
         }

         k--;
      }

      if ( k == 0 )
         pzmaGetFocus = 0;
   }

   if ( pzmaGetFocus )
      ProcessEvent( this, 64 );

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
}

void
ZMLE::MLENUpdate( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MLENUpdate: ", *m_pzsTag );
#endif
   ProcessEvent( this, 2048 );
   Default( );             // pass it along to parent
}

void
ZMLE::MLENVscroll( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::MLENVscroll: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

HBRUSH
ZMLE::CtlColor( CDC *pDC, UINT nCtlColor )
{
#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( "EdtNcPaint", *m_pzsTag ) == 0 )
   {
      CString cs;
      GetWindowText( cs );
      TraceLineS( "ZMLE::CtlColor Tag ", *m_pzsTag );
   // TraceLineS( "ZMLE::CtlColor Text ", cs );
   // TraceLineI( "ZMLE::CtlColor nCtlColor ", nCtlColor );
   // TraceLineX( "ZMLE::CtlColor colorText ", (zLONG) m_clrText );
   }
#endif

   HBRUSH hBrush;

   if ( m_pBrush )
   {
      hBrush = (HBRUSH) *m_pBrush;
      pDC->SetBkColor( m_clrBk );
   }
   else
      hBrush = (HBRUSH) Default( );

   if ( m_ulCtrlErr )
   {
      pDC->SetTextColor( zRGB( 255, 0, 0 ) );
//    pDC->SetTextColor( m_pZSubtask->m_rgbColorErrText );
   }
   else
   {
      if ( m_clrText )
         pDC->SetTextColor( m_clrText );
   }

   return( hBrush );
}

#if 0
BOOL
ZMLE::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB )
   {
      if ( fnIsAccelerator( m_pZSubtask, pMsg ) )
         return( TRUE );
   }

   return( CEdit::PreTranslateMessage( pMsg ) );
}
#endif

zBOOL
ZMLE::HandleQuickKey( zACCEL_DEF ad )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::HandleQuickKey: ", *m_pzsTag );
#endif

   if ( ad.uVK == VK_TAB && ad.fAlt == 0 && ad.fCtrl == 1 )
   {
      ReplaceSel( "\t", TRUE );
      return( TRUE );
   }
   else
   if ( ad.uVK == VK_TAB && ad.fAlt == 0 && ad.fCtrl == 0 )
   {
      ZMapAct *pzma = m_pZSubtask->
         GetFocusCtrlWithinComposite( 0, this, !ad.fShift );
      if ( pzma && pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
      {
         pzma->m_pCtrl->SetFocus( );
      }

      return( TRUE );
   }
   else
   if ( ad.uVK == VK_RETURN )
   {
      if ( ad.fAlt == 0 )
      {
         ReplaceSel( "\r\n", TRUE );
      }
      else
      if ( ad.fAlt &&
           m_pZSubtask->m_pCurrDfltBtn &&
           m_pZSubtask->m_pCurrDfltBtn->m_pCtrl &&
           mIs_hWnd( m_pZSubtask->m_pCurrDfltBtn->m_pCtrl->m_hWnd ) )
      {
         m_pZSubtask->m_pCurrDfltBtn->m_pCtrl->SendMessage( WM_LBUTTONDOWN );
         m_pZSubtask->m_pCurrDfltBtn->m_pCtrl->SendMessage( WM_LBUTTONUP );
      }

      return( TRUE );
   }

   return( FALSE );
}

void
ZMLE::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZMLE::OnKeyDown - ", uKey );
#endif
   if ( uKey == VK_TAB )
   {
// do special method in driver for this???
//      m_pZSubtask->m_pZFWnd->SendMessage( WM_COMMAND,
//                                          zPROCESS_ZEIDON_TAB, 0 );
   }

// CEdit::OnKeyDown( uKey, uRepeatCnt, uFlags );
   Default( );             // pass it along to parent
}

void
ZMLE::OnSysKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
// TraceLineI( "WMSysKeyDown - ", Msg.WParam );
// CEdit::OnSysKeyDown( uKey, uRepeatCnt, uFlags );
   Default( );             // pass it along to parent
}

void
ZMLE::OnSysChar( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
// TraceLineI( "WMSysChar - ", Msg.WParam );
// CEdit::OnSysChar( uKey, uRepeatCnt, uFlags );
   Default( );             // pass it along to parent
}

void
ZMLE::OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
// TraceLineI( "WMChar - ", Msg.WParam );
// CEdit::OnChar( uKey, uRepeatCnt, uFlags );
   Default( );             // pass it along to parent
}

void
ZMLE::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZMLE - dtor
ZMLE::~ZMLE( )
{
// TraceLineS( "ZMLE::~ZMLE", "" );

// if ( m_hEditDS )
//    GlobalFree( m_hEditDS );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

zLONG
ZMLE::SelectAll( )
{
   SetSel( 0, -1 );
   return( 1 );
}

zSHORT
ZMLE::SelectText( zSHORT nTextStartPos,
                  zSHORT nTextEndPos )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEdit::SelectText: ", *m_pzsTag );
#endif
   SetSel( nTextStartPos - 1, nTextEndPos - 1 );
   return( 0 );
}

zSHORT
ZMLE::LimitTextLength( zULONG ulTextLth )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZMLE::LimitTextLength: ", *m_pzsTag );
#endif
   if ( ulTextLth >= 0 )
   {
      m_ulLth = ulTextLth;
      if ( mIs_hWnd( m_hWnd ) )
         SetLimitText( m_ulLth );

#ifdef zREMOTE_SERVER
      zCHAR szTextLth[ 32 ];

      _ltoa_s( ulTextLth, szTextLth );
      SetRemoteZCtrlAttribute( this, "MLE", "TextLth", szTextLth );
#endif

      return( 0 );
   }

   return( 1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
MLE( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   if ( zstrcmpi( pCtrlDef->Tag, "_RTE" ) == 0 )
      return( new ZRichEdit( pZSubtask, pWndParent,
                             pzmaComposite, vDialog,
                             nOffsetX, nOffsetY, pCtrlDef ) );

   return( new ZMLE( pZSubtask, pWndParent,
                     pzmaComposite, vDialog,
                     nOffsetX, nOffsetY, pCtrlDef ) );
}
