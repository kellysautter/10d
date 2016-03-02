/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlchk.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon CheckBox control.
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
// 2001.05.31   BL      WEB
//    Bugfix GetCtrlState with flag zCONTROL_STATUS_CHECKED
//
// 2000.03.17    TMV    WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1998.10.30    DKS
//    Corrected processing when the ctrl is invisible/disabled.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlChk.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( ZCheckBox, CButton )

BEGIN_MESSAGE_MAP( ZCheckBox, CButton )
   //{{AFX_MSG_MAP( ZCheckBox )
   ON_WM_PAINT( )
   ON_WM_ERASEBKGND( )
   ON_WM_ENABLE( )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_MOUSEMOVE( )
   ON_WM_LBUTTONUP( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   //}}AFX_MSG_MAP
   ON_MESSAGE( BM_GETCHECK, OnGetCheck )
   ON_MESSAGE( BM_SETCHECK, OnSetCheck )

   ON_CONTROL_REFLECT( BN_CLICKED, CHKClicked )
// ON_CONTROL_REFLECT( BN_DISABLE, CHKDisable )
// ON_CONTROL_REFLECT( BN_DOUBLECLICKED, CHKDoubleclicked )
// ON_CONTROL_REFLECT( BN_HILITE, CHKHilite )
// ON_CONTROL_REFLECT( BN_PAINT, CHKPaint )
// ON_CONTROL_REFLECT( BN_UNHILITE, CHKUnhilite )
   ON_WM_CREATE( )
END_MESSAGE_MAP( )

#define zCHECKBOX_XCHECK              zCONTROL_SPECIFIC_1
#define zCHECKBOX_LED                 zCONTROL_SPECIFIC_2

// ZCheckBox - ctor
ZCheckBox::ZCheckBox( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
           CButton( ),
           ZMapAct( pZSubtask,            // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "CheckBox" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for CheckBox", (zLONG) this );
#endif

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( !(pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style |= WS_BORDER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

   m_pzsValueOn = 0;
   m_pzsValueOff = 0;

   zPCHAR pch = (zPCHAR) pCtrlDef->CtrlBOI;
   if ( pch == 0 )      // there is no CtrlBOI
      return;

   zPCHAR pchAmpersand = zstrchr( *m_pzsText, '&' );
   while ( pchAmpersand )
   {
      pchAmpersand++;
      if ( *pchAmpersand == '&' )
         pchAmpersand++;
      else
      {
         m_chQuickKey = toupper( *pchAmpersand );
         break;
      }

      pchAmpersand = zstrchr( pchAmpersand, '&' );
   }

   m_pzsValueOn = new CString( pch );
   pch += zstrlen( pch ) + 1;
   m_pzsValueOff = new CString( pch );

   BOX_SIZE = 13;  // ASSERT( BOX_SIZE > 1 );  ASSERT( BOX_SIZE < 30 );
   if ( pCtrlDef->Subtype & zCHECKBOX_XCHECK )
      m_resID = IDB_CB_XCHECK;
   else
   if ( pCtrlDef->Subtype & zCHECKBOX_LED )
   {
      m_resID = IDB_CB_LED;
      BOX_SIZE = 11;
   }
   else
      m_resID = IDB_CB_CHECKMARK;

   // load state bitmaps
   m_bmpSrc = LoadBitmap( AfxGetResourceHandle( ), MAKEINTRESOURCE( m_resID ) );
   ASSERT( m_bmpSrc ); // bitmap resource ID valid?

   m_bLeftText = FALSE;
   m_bDisabled = FALSE;
   m_bChecked  = FALSE;
   m_lState    = BOX_OFF;

   m_bInRect    = FALSE;
   m_bSpaceDown = FALSE;
   m_bMouseDown = FALSE;

   m_brush = 0;
   m_clr = CLR_INVALID;
   m_bHasFocus = FALSE;
   m_bPrepareBitmaps = TRUE;

   for ( int k = 0; k < 6; k++ )
      m_bitmaps[ k ] = 0;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

// ZCheckBox - dtor
ZCheckBox::~ZCheckBox( )
{
// TraceLineS( "ZCheckBox::~ZCheckBox", "" );
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );

   mDeleteInit( m_pzsValueOn );
   mDeleteInit( m_pzsValueOff );
   m_clr = CLR_INVALID;
   for ( int k = 0; k < 6; k++ )
   {
      if ( m_bitmaps[ k ] )
      {
         DeleteObject( m_bitmaps[ k ] );  // it was subscript 0???
         m_bitmaps[ k ] = 0;
      }
   }

// DeleteObject( m_pFont );
}

void
ZCheckBox::CreateZ( )
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
         CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "BUTTON", *m_pzsText,
                   BS_CHECKBOX | BS_OWNERDRAW | Attr.Style,
                   Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZCheckBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
// TraceLineS( "ZCheckBox::OnCreate", "" );

   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   LOGFONT lf;

   mDeleteInit( m_pFont );
   m_pFont = new CFont;
   ::GetObject( (HFONT) GetStockObject( DEFAULT_GUI_FONT ), sizeof( lf ), &lf );
   m_pFont->CreateFontIndirect( &lf );
   SetFontOverride( );
   return( nRC );
}

BOOL
ZCheckBox::DestroyWindow( )
{
// TraceLineS( "ZCheckBox::DestroyWindow", "" );
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CButton::DestroyWindow( ) );
}

// Get the text for the entry control and map it to the data OI.
zSHORT
ZCheckBox::MapToOI( zLONG lFlag )
{
// if ( zstrcmp( *m_pzsTag, "CheckAutoCreateActOnCatChg" ) == 0 )
//    TraceLineS( "ZCheckBox::MapToOI ", *m_pzsTag );

   zVIEW vApp;

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 &&
        CheckExistenceOfEntity( vApp, *m_pzsEName ) == 0 )
   {
      zBOOL bCheck = (GetCheck( ) == 1);

      return( SetAttributeFromVariable( vApp, *m_pzsEName, *m_pzsAName,
                                        bCheck ? *m_pzsValueOn : *m_pzsValueOff,
                                        zTYPE_STRING, 0, *m_pzsContext,
                                        (zSHORT) lFlag ) );
   }

   return( 0 );
}

// Get the data from the OI and use it to set the check for the check button
// appropriately.
zSHORT
ZCheckBox::MapFromOI( WPARAM wFlag )
{
// if ( zstrcmp( *m_pzsTag, "CheckAutoCreateActOnCatChg" ) == 0 )
//    TraceLineS( "ZCheckBox::MapFromOI ", *m_pzsTag );

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zPCHAR pch;
      zBOOL  bCheck = FALSE;
      zULONG ulLth;
      zCHAR  szMap[ 256 ];
      zVIEW  vApp;

      zUSHORT uMapState = GetMappingAutoGray( &vApp, *m_pzsVName,
                                              *m_pzsEName,
                                              *m_pzsAName );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) )
      {
         GetAttributeLength( &ulLth, vApp, *m_pzsEName, *m_pzsAName );
         if ( ulLth >= zsizeof( szMap ) )
         {
            GetAddrForAttribute( (zPCHAR *) &pch, vApp,
                                 *m_pzsEName, *m_pzsAName );
         }
         else
         {
            GetVariableFromAttribute( szMap, 0, zTYPE_STRING, 256,
                                      vApp, *m_pzsEName,
                                      *m_pzsAName, *m_pzsContext, 0 );
            pch = szMap;
         }

         // determine check or uncheck
         bCheck = (zstrcmp( pch, (*m_pzsValueOn).GetString() ) ? 0 : 1);
      }

      SetCheck( bCheck );
      return( 0 );
   }

   return( -1 );
}

void
ZCheckBox::CHKClicked( )
{
//?SetCheck( GetCheck( ) ? 0 : 1 );
   ProcessEvent( this, 1 );
}

#if 0
void
ZCheckBox::CHKDisable( )
{
   Default( );             // pass it along to parent
}

void
ZCheckBox::CHKDoubleclicked( )
{
   Default( );             // pass it along to parent
}

void
ZCheckBox::CHKHilite( )
{
   Default( );             // pass it along to parent
}

void
ZCheckBox::CHKPaint( )
{
   Default( );             // pass it along to parent
}

void
ZCheckBox::CHKUnhilite( )
{
   Default( );             // pass it along to parent
}
#endif

zLONG
ZCheckBox::GetZCtrlState( zLONG lStatusType )
{
   if ( lStatusType == zCONTROL_STATUS_CHECKED )
   {
#ifdef zREMOTE_SERVER
      return( (GetCheck( ) == 0) ? FALSE : TRUE );
#else
      if ( mIs_hWnd( m_hWnd ) )
         return( (GetCheck( ) == 0) ? FALSE : TRUE );
      else
         return( FALSE );
#endif
   }
   else
      return( ZMapAct::GetZCtrlState( lStatusType ) );
}

zLONG
ZCheckBox::SetZCtrlState( zLONG lStatusType,
                          zULONG ulValue )
{
   if ( lStatusType == zCONTROL_STATUS_CHECKED )
   {
#ifdef zREMOTE_SERVER
      SetRemoteZCtrlAttribute( this, "CheckBox",
                               "Checked", ulValue ? "Y" : "N" );
#else
      if ( mIs_hWnd( m_hWnd ) )
         SetCheck( ulValue ? 1 : 0 );
#endif

      return( 0 );
   }
   else
      return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

zBOOL
ZCheckBox::HandleQuickKey( zACCEL_DEF ad )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZCheckBox::HandleQuickKey ", *m_pzsTag );
#endif

   if ( ad.fAlt && ad.uVK == m_chQuickKey )
   {
      if ( (m_ulMapActFlags & zMAPACT_VISIBLE) &&
           (m_ulMapActFlags & zMAPACT_ENABLED) &&
           (m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) &&
           (mIs_hWnd( m_hWnd )) )
      {
         SetFocus( );
         SendMessage( WM_LBUTTONDOWN );
         SendMessage( WM_LBUTTONUP );
      }

      return( TRUE );
   }

   return( FALSE );
}

zLONG
ZCheckBox::HandleEvent( zLONG lEvent )
{
   CHKClicked( );
   return( 0 );
}

void
ZCheckBox::SetCheck( zBOOL bCheck )
{
#ifdef zREMOTE_SERVER
   SetRemoteZCtrlAttribute( this, m_csERemote, "Checked",
                            bCheck ? "Y" : "N" );
#else
   if ( mIs_hWnd( m_hWnd ) )
      CButton::SetCheck( bCheck );
#endif
}

zBOOL
ZCheckBox::GetCheck( )
{
   zBOOL bRC = FALSE;

#ifdef zREMOTE_SERVER
   zCHAR  szYN[ 2 ];

   if ( GetRemoteZCtrlAttribute( this, m_csERemote,
                                 "Checked", szYN, zsizeof( szYN ) ) >= 0 )
   {
      if ( *szYN == 'Y' || *szYN == 'y' )
         bRC = TRUE;
   }
#else
   if ( mIs_hWnd( m_hWnd ) )
      bRC = (zBOOL) CButton::GetCheck( );
#endif

   return( bRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Initialization of static members

zBOOL ZCheckBox::m_bDrawFocus = TRUE;

/////////////////////////////////////////////////////////////////////////////
// Name:    ZCheckBox
// Description:   Constructor
//
// Entry:
//          int rcID    - resource ID with state bitmaps
//
//          int dimension  == width == height of a state bitmap
/////////////////////////////////////////////////////////////////////////////
//
// ZCheckBox overrides

/////////////////////////////////////////////////////////////////////////////
// Name:    PreSubclassWindow
// Description:   Make some initializations before the actual
//          subclassing occurs
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::PreSubclassWindow( )
{
   // text on left side?
   //
   DWORD dwStyle = GetStyle( );
   DWORD dwStyleEx = GetExStyle( );

   if ( (dwStyle & BS_LEFTTEXT) || (dwStyle & BS_RIGHTBUTTON) ||
        (dwStyleEx & WS_EX_RIGHT) )
   {
      m_bLeftText = TRUE;
   }

// GetWindowText( *m_pzsText );                              // get text
// m_pFont = (HFONT) GetParent( )->SendMessage( WM_GETFONT, 0, 0 );  // get font

   if ( IsWindowEnabled( ) == FALSE )
   {
      m_bDisabled = TRUE;
      m_lState = BOX_DISABLED_2;
   }

   // prevent any drawing by the control itself
   ModifyStyle( 0, BS_OWNERDRAW, 0 );

   CButton::PreSubclassWindow( );
}

/////////////////////////////////////////////////////////////////////////////
// Name:    DrawItem
// Description:   We have to override this, because CButton::DrawItem
//          only executes an ASSERT
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
   return;
}

/////////////////////////////////////////////////////////////////////////////
// Name:    PreTranslateMessage
// Description:   WM_LBUTTONDBLCLK => WM_LBUTTONDOWN
//             - to ensure proper state repainting of the control
//             - to prevent BN_DBLCLK notifications to the parent
//              (BS_OWNERDRAW buttons send them, but checkboxes don't)
/////////////////////////////////////////////////////////////////////////////
BOOL
ZCheckBox::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_LBUTTONDBLCLK )
      pMsg->message = WM_LBUTTONDOWN;

   return( CButton::PreTranslateMessage( pMsg ) );
}

/////////////////////////////////////////////////////////////////////////////
// Name:    OnPaint
// Description:   Blits the prepared bitmaps to the screen
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnPaint( )
{
   DWORD clr1 = ::GetSysColor( COLOR_3DFACE );
   if ( m_clr != clr1 )  // sysColor changed??
   {
      for ( int k = 0; k < 6; k++ )  // => prepare bitmaps again
      {
         if ( m_bitmaps[ k ] )
         {
            DeleteObject( m_bitmaps[ k ] );  // it was subscript 0???
            m_bitmaps[ k ] = 0;
         }
      }

      m_clr = clr1;

      if ( m_brush )
         DeleteObject( m_brush );

      m_brush = ::CreateSolidBrush( m_clr );
      m_bPrepareBitmaps = TRUE;
   }

   if ( m_bHasFocus )
      DrawFocus( TRUE );

   CPaintDC dc( this );

   if ( m_bPrepareBitmaps )  // called first time or sysColor changed?
   {
      PrepareBitmaps( dc.m_hDC ); // => prepare bitmaps
      m_bPrepareBitmaps = FALSE;
   }

   BlitToScreen( dc.m_hDC, m_lState );

   if ( m_bHasFocus )
      DrawFocus( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// Prevent any background erasing
/////////////////////////////////////////////////////////////////////////////
BOOL
ZCheckBox::OnEraseBkgnd( CDC *pDC )
{
   return( 1 );
}

/////////////////////////////////////////////////////////////////////////////
// Enable or disable CheckBox
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnEnable( BOOL bEnable )
{
   if ( bEnable )
   {
      m_bDisabled = FALSE;

      if ( m_lState == BOX_DISABLED_1 )
          SetState( BOX_ON );
      else
         SetState( BOX_OFF );
   }
   else
   {
      m_bDisabled = TRUE;

      if ( m_bChecked )
         SetState( BOX_DISABLED_1 );
      else
         SetState( BOX_DISABLED_2 );
   }

   CButton::OnEnable( bEnable );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnKeyDown( UINT uChar, UINT uRepCnt, UINT nFlags )
{
   if ( uChar == VK_SPACE && !m_bSpaceDown )
   {
      m_bSpaceDown = TRUE;

      if ( !m_bMouseDown )
      {
         if ( m_bChecked )
            SetState( BOX_LDOWN_1 );
         else
            SetState( BOX_LDOWN_2 );
      }
   }
   else
   if ( uChar != VK_SPACE && m_bSpaceDown )
   {
      // if any other key pressed while space down
      m_bSpaceDown = FALSE;

      if ( !m_bMouseDown )
      {
         if ( m_bChecked )
            SetState( BOX_ON );
         else
            SetState( BOX_OFF );
      }
   }

   CButton::OnKeyDown( uChar, uRepCnt, nFlags );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnKeyUp( UINT uChar, UINT uRepCnt, UINT nFlags )
{
   if ( uChar == VK_SPACE && m_bSpaceDown )
   {
      m_bSpaceDown = FALSE;

      if ( m_bMouseDown == FALSE )
      {
         m_bChecked = (m_bChecked == TRUE ? FALSE : TRUE);

         if ( m_bChecked )
            SetState( BOX_ON );
         else
            SetState( BOX_OFF );
      }
   }

   CButton::OnKeyUp( uChar, uRepCnt, nFlags );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnLButtonDown( UINT nFlags, CPoint pt )
{
// TracePoint( "ZCheckBox::OnLButtonDown: ", pt );
   m_bMouseDown = TRUE;
   m_bInRect = TRUE;

   if ( m_bChecked )
      SetState( BOX_LDOWN_1 );
   else
      SetState( BOX_LDOWN_2 );

   CButton::OnLButtonDown( nFlags, pt );  // also calls "SetCapture" & "SetFocus"
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnMouseMove( UINT uModKeys, CPoint pt )
{
   CRect rect;
   GetClientRect( &rect );

   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   if ( m_bMouseDown )
   {
      if ( PtInRect( &rect, pt ) == FALSE )
      {
         if ( m_bChecked )
            SetState( BOX_ON );
         else
            SetState( BOX_OFF );

         m_bInRect = FALSE;
      }
      else
      if ( m_bInRect == FALSE )  // again over control
      {
         m_bInRect = TRUE;

         if ( m_bChecked )
            SetState( BOX_LDOWN_1 );
         else
            SetState( BOX_LDOWN_2 );
      }
   }

   CButton::OnMouseMove( uModKeys, pt );
// Default( );              // pass it along to parent
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnLButtonUp( UINT uModKeys, CPoint pt )
{
// TracePoint( "ZCheckBox::OnLButtonUp: ", pt );
   if ( m_bMouseDown )
      m_bMouseDown = FALSE;
   else
      return;

   CRect rect;
   GetClientRect( &rect );

   if ( PtInRect( &rect, pt ) )
   {
      m_bChecked = (m_bChecked == TRUE ? FALSE : TRUE);

      if ( m_bSpaceDown == FALSE )
      {
         if ( m_bChecked )
            SetState( BOX_ON );
         else
            SetState( BOX_OFF );
      }
      else
      {
         if ( m_bChecked )
            SetState( BOX_LDOWN_1 );
         else
            SetState( BOX_LDOWN_2 );
      }
   }
   else
   if ( m_bSpaceDown )  // mouse released outside of control, but "space" still pressed
   {
      if ( m_bChecked )
         SetState( BOX_LDOWN_1 );
      else
         SetState( BOX_LDOWN_2 );
   }

   CButton::OnLButtonUp( uModKeys, pt );   // also calls "ReleaseCapture"
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnSetFocus( CWnd *pWndLostFocus )
{
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
      m_bHasFocus = m_bDrawFocus;
      DrawFocus( );

   // Default( );             // pass it along to parent
      CButton::OnSetFocus( pWndLostFocus );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::OnKillFocus( CWnd *pNewWnd )
{
   m_pZSubtask->InformDriverOfKillFocus( this );
   if ( m_bSpaceDown )
   {
      // mouse pressed in another window, while m_bSpaceDown
      m_bSpaceDown = FALSE;
      if ( m_bChecked )
         SetState( BOX_ON );
      else
         SetState( BOX_OFF );
   }

   m_bHasFocus = FALSE;
   DrawFocus( );

   CButton::OnKillFocus( pNewWnd );
// Default( );             // pass it along to parent
}

/////////////////////////////////////////////////////////////////////////////
// Received BM_GETCHECK - either directly or through:
//    - CWnd::IsDlgButtonChecked or
//    - CButton::GetCheck wrapper
/////////////////////////////////////////////////////////////////////////////
LRESULT
ZCheckBox::OnGetCheck( WPARAM wParam, LPARAM lParam )
{
   if ( m_bChecked )
      return( BST_CHECKED );
   else
      return( BST_UNCHECKED );
}

/////////////////////////////////////////////////////////////////////////////
// Received BM_SETCHECK - either directly or through
//    - CButton::SetCheck wrapper
/////////////////////////////////////////////////////////////////////////////
LRESULT
ZCheckBox::OnSetCheck( WPARAM wParam, LPARAM lParam )
{
   if ( wParam == BST_CHECKED )
   {
      m_bChecked = TRUE;

      if ( m_bDisabled )
      {
         // the state of a disabled CB can also be changed
         SetState( BOX_DISABLED_1 );
         return 0;
      }

      if ( m_bMouseDown )     // if mouse down => space doesn't matter
      {
         if ( m_bInRect )
            SetState( BOX_LDOWN_1 );
         else
            SetState( BOX_ON );
      }
      else
      if ( m_bSpaceDown )
          SetState( BOX_LDOWN_1 );
      else
         SetState( BOX_ON );
   }
   else
   {
      m_bChecked = FALSE;

      if ( m_bDisabled )
      {
         SetState( BOX_DISABLED_2 );
         return( 0 );
      }

      if ( m_bMouseDown )
      {
         if ( m_bInRect )
            SetState( BOX_LDOWN_2 );
         else
            SetState( BOX_OFF );
      }
      else
      if ( m_bSpaceDown )
          SetState( BOX_LDOWN_2 );
      else
         SetState( BOX_OFF );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// Draws a focus rectangle
//
//   zBOOL forceDraw - this parameter can be one of the following:
//         FALSE : draw focus rect only if "m_bHasFocus" has changed
//         TRUE  : draw focus rect regardles of previous value
//                 of "m_bHasFocus"
//
//   m_bHasFocus - has the following meaning:
//         TRUE  : check box has the keyboard focus at the moment
//         FALSE : check box is not focused at the moment
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::DrawFocus( zBOOL bForceDraw )
{
   static zBOOL bPrev = FALSE;

   if ( m_bDrawFocus )   // draw focus rectangle?
   {
      if ( bPrev != m_bHasFocus || bForceDraw )
      {
         bPrev = m_bHasFocus;

         CDC *pDC = GetDC( );
         DrawFocusRect( pDC->m_hDC, &m_rectFocus );
         ReleaseDC( pDC );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// Sets the new state and then blits the CB to the screen
//
//   int lState - specifies the state of the CB and can be:
//         BOX_ON         == checked state
//         BOX_OFF        == unchecked state
//         BOX_LDOWN_1    == mouse button down in checked state
//         BOX_LDOWN_2    == mouse button down in unchecked state
//         BOX_DISABLED_1 == CB disabled in checked state
//         BOX_DISABLED_2 == CB disabled in unchecked state
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::SetState( int lState )
{
   if ( lState != m_lState )
   {
      m_lState = lState;

      CDC *pDC = GetDC( );
      BlitToScreen( pDC->m_hDC, lState );
      ReleaseDC( pDC );

      if ( m_bHasFocus )
         DrawFocus( TRUE );
   }
}

/////////////////////////////////////////////////////////////////////////////
// Blits( =draw, copy,.. ) "m_bitmaps[nState]" to the screen
//
//   HDC hDC - DC to blit to
//   int nState - specifies the state of the CB and can be:
//         BOX_ON
//         BOX_OFF
//         BOX_LDOWN_1
//         BOX_LDOWN_2
//         BOX_DISABLED_1
//         BOX_DISABLED_2
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::BlitToScreen( HDC hDC, int nState )
{
   HDC     destDC = ::CreateCompatibleDC( hDC );
   HBITMAP bmpDestDC = (HBITMAP) ::SelectObject( destDC, m_bitmaps[ nState ] );

   CRect rect;
   GetClientRect( &rect );

   int nWidth  = rect.right  - rect.left;
   int nHeight = rect.bottom - rect.top;

   if ( nHeight >= m_nHeight )
   {
      rect.top += (nHeight - m_nHeight) / 2;
      rect.bottom -= (nHeight - m_nHeight) / 2;
      nHeight = rect.bottom - rect.top;
   }

   // Blit prepared bitmap to screen.
   BitBlt( hDC, rect.left, rect.top, nWidth, nHeight, destDC, 0, 0, SRCCOPY );

   // Clean up.
   SelectObject( destDC, bmpDestDC );
   DeleteDC( destDC );
}

/////////////////////////////////////////////////////////////////////////////
// Calculates minimal height of CB based on m_pFont
//   HDC hDC - DC into which m_pFont is selected
/////////////////////////////////////////////////////////////////////////////
int
ZCheckBox::GetMinHeight( HDC hDC )
{
   HFONT oFont = (HFONT) SelectObject( hDC, *m_pFont );
   SIZE sz; GetTextExtentPoint32( hDC, "Dummy", 5, &sz );
   SelectObject( hDC, oFont );

   if ( sz.cy < Attr.H )
      return( Attr.H );
   else
      return( sz.cy + 1 );
}

/////////////////////////////////////////////////////////////////////////////
// Prepare "m_bitmaps[???]" for all possible states
//
//   HDC     hDC - DC used to create memory DCs for bitmap preparation
//
//          HBITMAP &bmpDest - this parameter is one of the following:
//                m_bitmaps[ 0 ] : BOX_ON  bitmap
//                m_bitmaps[ 1 ] : BOX_OFF bitmap
//                ...
//                m_bitmaps[ 5 ] : BOX_DISABLED_2 bitmap
//
//          int nState - specifies the state of the CB and can be:
//             BOX_ON
//             BOX_OFF
//             BOX_LDOWN_1
//             BOX_LDOWN_2
//             BOX_DISABLED_1
//             BOX_DISABLED_2
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::PrepareBitmaps( HDC hDC )
{
   m_nHeight = GetMinHeight( hDC );
   if ( m_nHeight < BOX_SIZE )
      m_nHeight = BOX_SIZE;

   PrepareState( hDC, m_bitmaps[ 0 ], BOX_ON );
   PrepareState( hDC, m_bitmaps[ 1 ], BOX_OFF );
   PrepareState( hDC, m_bitmaps[ 2 ], BOX_LDOWN_1 );
   PrepareState( hDC, m_bitmaps[ 3 ], BOX_LDOWN_2 );
   PrepareState( hDC, m_bitmaps[ 4 ], BOX_DISABLED_1 );
   PrepareState( hDC, m_bitmaps[ 5 ], BOX_DISABLED_2 );
}

void
ZCheckBox::PrepareState( HDC hDC, HBITMAP& bmpDest, int nState )
{
   CRect rect;
   GetClientRect( &rect );

   int nWidth  = rect.right  - rect.left;
   int nHeight = rect.bottom - rect.top;

   if ( nHeight >= m_nHeight )
   {
      rect.top += (nHeight - m_nHeight) / 2;
      rect.bottom -= (nHeight - m_nHeight) / 2;
      nHeight = rect.bottom - rect.top;
   }

   int j = (m_nHeight - BOX_SIZE) / 2;

   // Create memory DCs and bitmaps to prepare the image.
   HDC srcDC  = ::CreateCompatibleDC( hDC );
   HDC maskDC = ::CreateCompatibleDC( hDC );
   HDC destDC = ::CreateCompatibleDC( hDC );

   HBITMAP bmpMask = ::CreateBitmap( BOX_SIZE,BOX_SIZE, 1, 1, NULL );
   bmpDest = ::CreateCompatibleBitmap( hDC, nWidth, m_nHeight );

   HBITMAP bmpSrcDC  = (HBITMAP) ::SelectObject( srcDC, m_bmpSrc );
   HBITMAP bmpMaskDC = (HBITMAP) ::SelectObject( maskDC, bmpMask );
   HBITMAP bmpDestDC = (HBITMAP) ::SelectObject( destDC, bmpDest );

   // Create mask - mask color: RGB( 255, 0, 255 ).
   COLORREF clr = SetBkColor( srcDC, RGB( 255, 0, 255 ) );

   // Drawing rectangle inside client area of CB.
   CRect rectDraw( 0, 0, nWidth, nHeight );

   // Copy the display surface where the CheckBox will be to destDC.
#if TRANSPARENT_CB
   BitBlt( destDC, 0, 0, nWidth, nHeight, hDC, rect.left, rect.top, SRCCOPY );
#else
   FillRect( destDC, &rectDraw, m_brush );
#endif

   int k = 0;
   if ( m_bLeftText )
      k = nWidth - BOX_SIZE;

   int x = BOX_SIZE * nState;
   BitBlt( maskDC, 0, 0, BOX_SIZE, BOX_SIZE, srcDC, x, 0, SRCCOPY );
   BitBlt( destDC, k, j, BOX_SIZE, BOX_SIZE, srcDC, x, 0, SRCINVERT );
   BitBlt( destDC, k, j, BOX_SIZE, BOX_SIZE, maskDC, 0, 0, SRCAND );
   BitBlt( destDC, k, j, BOX_SIZE, BOX_SIZE, srcDC, x, 0, SRCINVERT );

   // Draw control text.
   DrawTextToDestDC( destDC, rectDraw, nState );
   m_rectFocus.top += rect.top;
   m_rectFocus.bottom += rect.top;
   m_rectFocus.left -= 1;
   m_rectFocus.right += 1;

   // Clean up.
   SetBkColor( srcDC, clr );
   SelectObject( srcDC, bmpSrcDC );
   SelectObject( maskDC, bmpMaskDC );
   SelectObject( destDC, bmpDestDC );

   DeleteDC( srcDC );
   DeleteDC( maskDC );
   DeleteDC( destDC );

   DeleteObject( bmpMask );
}

/////////////////////////////////////////////////////////////////////////////
// Name:    DrawTextToDestDC
// Description:   Draws text for the CB to hDC
//         (into which one of the "m_bitmaps[???]" is selected)
//
// Entry:
//          HDC hDC - DC to which text is drawn
//                   ("m_bitmaps[???]" is selected into it)
//
//          CRect rect  - part of client area (of the CB) to which
//                    text is output
//
/////////////////////////////////////////////////////////////////////////////
void
ZCheckBox::DrawTextToDestDC( HDC hDC, CRect& rect, int nState )
{
   if ( **m_pzsText == 0 )
      return;

   int   nIndexDC = SaveDC( hDC );
   DWORD dwStyle = 0;

   SetBkMode( hDC, TRANSPARENT );
   SelectObject( hDC, *m_pFont );

   if ( m_bLeftText )
   {
      rect.right -= BOX_SIZE + 6;
      dwStyle = DT_RIGHT;
   }
   else
   {
      rect.left += BOX_SIZE + 5;
      dwStyle = DT_LEFT;
   }

   rect.bottom -= 1;            // rect = drawing rectangle inside hDC

   if ( nState == BOX_DISABLED_1 || nState == BOX_DISABLED_2 )
   {
      if ( (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) == 0 )
      {
         SetTextColor( hDC, GetSysColor( COLOR_3DHILIGHT ) );

         rect.OffsetRect( 1, 1 );
         DrawText( hDC, *m_pzsText, -1, &rect,
                   DT_SINGLELINE | DT_VCENTER | dwStyle );

         SetTextColor( hDC, GetSysColor( COLOR_3DSHADOW ) );
         rect.OffsetRect( -1, -1 );
      }

      DrawText( hDC, *m_pzsText, -1, &rect,
                DT_SINGLELINE | DT_VCENTER | dwStyle );
   }
   else
   {
      DrawText( hDC, *m_pzsText, -1, &rect,
                DT_SINGLELINE | DT_VCENTER | dwStyle );
   }

   // calculate m_rectFocus
   int r_t = rect.right;           // right side of text rectangle

   DrawText( hDC, *m_pzsText, -1, &rect,
             DT_SINGLELINE | DT_VCENTER | dwStyle | DT_CALCRECT );

   CDC *pDC = CDC::FromHandle( hDC );
   CRect rectClip;
   pDC->GetClipBox( &rectClip );
   if ( rect.Width( ) > rectClip.Width( ) - BOX_SIZE -5 )
      rect.right = rect.left + rectClip.Width( ) - BOX_SIZE - 5;
   else
   if ( m_bLeftText )
   {
      int d = rect.right - rect.left; // text width
      d = r_t - d;
      rect.left  += d;
      rect.right += d;
   }

   m_rectFocus = rect;
   RestoreDC( hDC, nIndexDC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION                 // WINAPI
CheckBox( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZCheckBox( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}
