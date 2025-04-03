/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlbtn.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon PushButton control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2003.07.17    DKS   Frank
//    Added operation BTN_SetColor to set button colors.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2000.03.17    TMV    WEB
//    Validate parent window's handle prior to call Create/CreateEx.
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
#include "ZdCtlBtn.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
//  An application sends a BM_SETSTYLE message to change the style of a
//  button.
//
//  BM_SETSTYLE
//  wParam =  (WPARAM) LOWORD(dwStyle); // style
//  lParam =  MAKELPARAM(fRedraw, 0);   // redraw flag
//
//  Parameters:
//
//  dwStyle
//
//  Value of wParam. Specifies the button style. For an explanation of
//  button styles, see the following Remarks section.
//
//  fRedraw
//
//  Value of the low-order word of lParam.  Specifies whether the button
//  is to be redrawn.  A value of TRUE redraws the button; a value of
//  FALSE does not redraw the button.
//
//  Return Values:  This message always returns zero.
//
//  Remarks:  Following are the available button styles:
//
//  BS_3STATE
//  Creates a button that is the same as a check box, except that the
//  box can be grayed as well as checked or unchecked.  Use the grayed
//  state to show that the state of the check box is not determined.
//
//  BS_AUTO3STATE
//  Creates a button that is the same as a three-state check box, except
//  that the box changes its state when the user selects it.  The state
//  cycles through checked, grayed, and unchecked.
//
//  BS_AUTOCHECKBOX
//  Creates a button that is the same as a check box, except that the
//  check state automatically toggles between checked and unchecked each
//  time the user selects the check box.
//
//  BS_AUTORADIOBUTTON
//  Creates a button that is the same as a radio button, except that
//  when the user selects it, Windows automatically sets the button's
//  check state to checked and automatically sets the check state for
//  all other buttons in the same group to unchecked.
//
//  BS_CHECKBOX
//  Creates a small, empty check box with text. By default, the text is
//  displayed to the right of the check box. To display the text to the
//  left of the check box, combine this flag with the BS_LEFTTEXT style
//  (or with the equivalent BS_RIGHTBUTTON style).
//
//  BS_DEFPUSHBUTTON
//  Creates a push button that behaves like a BS_PUSHBUTTON style
//  button, but also has a heavy black border. If the button is in a
//  dialog box, the user can select the button by pressing the ENTER
//  key, even when the button does not have the input focus.  This style
//  is useful for enabling the user to quickly select the most likely
//  (default) option.
//
//  BS_GROUPBOX
//  Creates a rectangle in which other controls can be grouped.  Any
//  text associated with this style is displayed in the rectangle's
//  upper left corner.
//
//  BS_LEFTTEXT
//  Places text on the left side of the radio button or check box when
//  combined with a radio button or check box style.  Same as the
//  BS_RIGHTBUTTON style.
//
//  BS_OWNERDRAW
//  Creates an owner-drawn button.  The owner window receives a
//  WM_MEASUREITEM message when the button is created and a WM_DRAWITEM
//  message when a visual aspect of the button has changed.  Do not
//  combine the BS_OWNERDRAW style with any other button styles.
//
//  BS_PUSHBUTTON
//  Creates a push button that posts a WM_COMMAND message to the owner
//  window when the user selects the button.
//
//  BS_RADIOBUTTON
//  Creates a small circle with text.  By default, the text is displayed
//  to the right of the circle.  To display the text to the left of the
//  circle, combine this flag with the BS_LEFTTEXT style (or with
//  exclusive choices.
//
//  BS_USERBUTTON
//  Obsolete, but provided for compatibility with 16-bit versions of
//  Windows.  Win32-based applications should use BS_OWNERDRAW instead.
//
//  BS_BITMAP
//  Specifies that the button displays a bitmap.
//
//  BS_BOTTOM
//  Places text at the bottom of the button rectangle.
//
//  BS_CENTER
//  Centers text horizontally in the button rectangle.
//
//  BS_ICON
//  Specifies that the button displays an icon.
//
//  BS_LEFT
//  Left-justifies the text in the button rectangle.  However, if the
//  button is a check box or radio button that does not have the
//  BS_RIGHTBUTTON style, the text is left justified on the right side
//  of the check box or radio button.
//
//  BS_MULTILINE
//  Wraps the button text to multiple lines if the text string is too
//  long to fit on a single line in the button rectangle.
//
//  BS_NOTIFY
//  Enables a button to send BN_DBLCLK, BN_KILLFOCUS, and BN_SETFOCUS
//  notification messages to its parent window.  Note that buttons send
//  the BN_CLICKED notification message regardless of whether it has
//  this style.
//
//  BS_PUSHLIKE
//  Makes a button (such as a check box, three-state check box, or radio
//  button) look and act like a push button.  The button looks raised
//  when it isn't pushed or checked, and sunken when it is pushed or
//  checked.
//
//  BS_RIGHT
//  Right-justifies text in the button rectangle.  However, if the
//  button is a check box or radio button that does not have the
//  BS_RIGHTBUTTON style, the text is right justified on the right side
//  of the check box or radio button.
//
//  BS_RIGHTBUTTON
//  Positions a radio button's circle or a check box's square on the
//  right side of the button rectangle.  Same as the BS_LEFTTEXT style.
//
//  BS_TEXT
//  Specifies that the button displays text.
//
//  BS_TOP
//  Places text at the top of the button rectangle.
//
//  BS_VCENTER
//  Places text in the middle (vertically) of the button rectangle.
//
/////////////////////////////////////////////////////////////////////////////

#define zBUTTON_MULTILINE             zCONTROL_SPECIFIC_1

IMPLEMENT_DYNAMIC( ZPushButton, CButton )

BEGIN_MESSAGE_MAP( ZPushButton, CButton )
   ON_CONTROL_REFLECT( BN_CLICKED, BNClicked )
   ON_CONTROL_REFLECT( BN_DISABLE, BNDisable )
   ON_CONTROL_REFLECT( BN_DOUBLECLICKED, BNDoubleClicked )
   ON_CONTROL_REFLECT( BN_HILITE, BNHilite )
   ON_CONTROL_REFLECT( BN_PAINT, BNPaint )
   ON_CONTROL_REFLECT( BN_UNHILITE, BNUnhilite )
   ON_WM_CREATE( )
// ON_WM_CTLCOLOR( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )

// ZPushButton - ctor
ZPushButton::ZPushButton( ZSubtask *pZSubtask,
                          CWnd     *pWndParent,
                          ZMapAct  *pzmaComposite,
                          zVIEW    vDialog,
                          zSHORT   nOffsetX,
                          zSHORT   nOffsetY,
                          zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
             CButton( ),
             ZMapAct( pZSubtask,          // base class ctor
                      pzmaComposite,
                      pWndParent,
                      this,
                      vDialog,
                      nOffsetX,
                      nOffsetY,
                      pCtrlDef,
                      "Button" )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "Bitmap" ) == 0 )
      TraceLineS( "ZPushButton::ctor: ", *m_pzsTag );
#endif

   // Determine if this button is the default pushbutton for this window.
   if ( CompareAttributeToAttribute( pZSubtask->m_vDialog,
                                     szlWnd, szlDfltBtn,
                                     vDialog, szlCtrl, szlTag ) == 0 )
   {
      pZSubtask->m_pDfltBtn = this;            // Zeidon default pushbutton
      pZSubtask->m_pCurrDfltBtn = this;
      Attr.Style |= BS_DEFPUSHBUTTON;
   }
   else
      Attr.Style &= ~BS_DEFPUSHBUTTON;

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (pCtrlDef->Subtype & zCONTROL_BORDEROFF) == 0 )
      Attr.Style |= WS_BORDER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

   if ( pCtrlDef->Subtype & zBUTTON_MULTILINE )
   {
      zPCHAR pchText;
      zPCHAR pch = m_pzsText->GetBuffer( m_pzsText->GetLength( ) );

      Attr.Style |= BS_MULTILINE;

      while ( (pchText = zstrchr( pch, '\\' )) != 0 )
      {
         if ( *(pchText + 1) == '\\' )
         {
            // Convert to one backslash.
            strcpy_s( pchText, zstrlen( pchText ), pchText + 1 );
            pchText--;  // back up so skip only one
         }
         else
         if ( *(pchText + 1) == 'n' )
         {
            *pchText = '\r';
            *(pchText + 1) = '\n';
         }

         pch = pchText + 2;
      }

      m_pzsText->ReleaseBuffer( );
   }

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

   if ( m_clrBk == 0 )
   {
      if ( m_pZSubtask->m_clrBk )
         m_clrBk = m_pZSubtask->m_clrBk;
      else
         m_clrBk = GetSysColor( COLOR_BTNFACE );
   }

// m_clrBkDisabled = ::GetSysColor( COLOR_GRAYTEXT );
   m_clrBkDisabled = m_clrBk;
   m_clrLight = GetSysColor( COLOR_3DLIGHT );
   m_clrHighlight = GetSysColor( COLOR_BTNHIGHLIGHT );
   m_clrShadow = GetSysColor( COLOR_BTNSHADOW );
   m_clrDarkShadow = GetSysColor( COLOR_3DDKSHADOW );

   m_uBevel = 2;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

void
ZPushButton::CreateZ( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( "OrdersUpdate", *m_pzsTag ) == 0 ||
        zstrcmp( "QuotesUpdate", *m_pzsTag ) == 0 )
   {
      TraceLineS( "ZPushButton::CreateZ ", *m_pzsTag );
   }
#endif

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
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZPushButton::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::OnCreate ", *m_pzsTag );
#endif

   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );
   return( nRC );
}

BOOL
ZPushButton::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::DestroyWindow ", *m_pzsTag );
#endif

   if ( m_pZSubtask->m_pDfltBtn == this )
      m_pZSubtask->m_pDfltBtn = 0;

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CButton::DestroyWindow( ) );
}

zSHORT
ZPushButton::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW   vApp;

      if ( GetViewByName( &vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
         {
            zCHAR  szText[ 256 ];
            zSHORT nRC =
               GetVariableFromAttribute( szText, 0, zTYPE_STRING,
                                         zsizeof( szText ),
                                         vApp, *m_pzsEName,
                                         *m_pzsAName, *m_pzsContext,
                                         m_pzsContext->IsEmpty( ) ?
                                zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT :
                                zACCEPT_NULL_ENTITY );
            if ( nRC == zVAR_NULL )
               szText[ 0 ] = 0;

            SetWindowText( szText );
            Invalidate( );
            if ( m_pzsText == 0 || zstrcmp( szText, (*m_pzsText).GetString() ) != 0 )
            {
               mDeleteInit( m_pzsText );
               m_pzsText = new CString( szText );
            }

            return( 0 );
         }
      }
   }

   return( -1 );
}

void
ZPushButton::BNClicked( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::BNClicked ", *m_pzsTag );
#endif

   if ( m_ulMapActFlags & zMAPACT_ENABLED )
      ProcessEvent( this, 1 );

// Default( );
}

void
ZPushButton::BNDisable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::BNDisable ", *m_pzsTag );
#endif

   Default( );   // pass it along to parent
}

void
ZPushButton::BNDoubleClicked( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::BNDoubleClicked ", *m_pzsTag );
#endif

   Default( );   // pass it along to parent
}

void
ZPushButton::BNHilite( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::BNHilite ", *m_pzsTag );
#endif

   Default( );   // pass it along to parent
}

void
ZPushButton::BNPaint( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::BNPaint ", *m_pzsTag );
#endif

   Default( );   // pass it along to parent
}

void
ZPushButton::BNUnhilite( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::BNUnhilite ", *m_pzsTag );
#endif

   Default( );   // pass it along to parent
}

void
ZPushButton::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::OnSetFocus ", *m_pzsTag );
#endif

   if ( m_pZSubtask->InformDriverOfSetFocus( this, TRUE ) )
   {
      Default( );    // pass it along to parent
#if 0
      m_pZSubtask->m_pCurrDfltBtn == this
      {
         UINT uStyle = GetButtonStyle( );
         uStyle = (uStyle & ~BS_PUSHBUTTON) | BS_DEFPUSHBUTTON;
         SetButtonStyle( uStyle );
         SetState( FALSE );
         Invalidate( );
         UpdateWindow( );
      }
#endif
   }
}

void
ZPushButton::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::OnKillFocus ", *m_pzsTag );
#endif

#if 0
   if ( m_pZSubtask->m_pCurrDfltBtn == this &&
        m_pZSubtask->m_pDfltBtn != this )
   {
      m_pZSubtask->m_pCurrDfltBtn = m_pZSubtask->m_pDfltBtn;
      UINT uStyle = GetButtonStyle( );
      if ( uStyle & BS_DEFPUSHBUTTON )
      {
         uStyle = (uStyle & ~BS_DEFPUSHBUTTON) | BS_PUSHBUTTON;
         SetButtonStyle( uStyle );
         Invalidate( );
         UpdateWindow( );
      }

      uStyle = GetButtonStyle( );
      if ( uStyle & BS_DEFPUSHBUTTON )
      {
         uStyle = (uStyle & ~BS_DEFPUSHBUTTON) | BS_PUSHBUTTON;
         SetButtonStyle( uStyle );
         Invalidate( );
         UpdateWindow( );
      }
   }
#endif

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );   // pass it along to parent
}

#if 0  // doesn't get here
HBRUSH
ZPushButton::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlType )
{
   if ( zstrcmp( *m_pzsTag, "Bitmap" ) == 0 )
      TraceLineS( "ZPushButton::CtlColor: ", *m_pzsTag );

   HBRUSH hBrush = (HBRUSH) Default( );

   if ( m_clrText )
      pDC->SetTextColor( m_clrText );

   if ( m_pBrush )
   {
      hBrush = (HBRUSH) *m_pBrush;
      pDC->SetBkColor( m_clrBk );
   }

   return( hBrush );
}
#endif

zBOOL
ZPushButton::HandleQuickKey( zACCEL_DEF ad )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::HandleQuickKey ", *m_pzsTag );
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

void
ZPushButton::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZPushButton - dtor
ZPushButton::~ZPushButton( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPushButton::dtor ", *m_pzsTag );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

#if 0
BOOL
ZPushButton::PreTranslateMessage( MSG *pMsg )
{
#ifdef DEBUG_ALL
// if ( pMsg->message == BM_SETSTYLE )
   if ( pMsg->hwnd == m_hWnd )
   {
      TraceLineI( "ZPushButton::PreTranslateMessage: ", pMsg->message );
   }
#endif

   return( CButton::PreTranslateMessage( pMsg ) );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

const COLORREF CLR_BTN_WHITE  = RGB( 255, 255, 255 );
const COLORREF CLR_BTN_BLACK  = RGB( 0, 0, 0 );
const COLORREF CLR_BTN_DGREY  = RGB( 128, 128, 128 );
const COLORREF CLR_BTN_GREY   = RGB( 192, 192, 192 );
const COLORREF CLR_BTN_LGREY  = RGB( 223, 223, 223 );

const int BUTTON_IN           = 0x01;
const int BUTTON_OUT          = 0x02;
const int BUTTON_BLACK_BORDER = 0x04;

void
ZPushButton::SetColor( COLORREF clrText, COLORREF clrBackground,
                       COLORREF clrBkHigh, COLORREF clrBkDark )
{
   m_clrText = clrText;
   m_clrBk = clrBackground;
   m_clrHighlight = clrBkHigh;
   m_clrShadow = clrBkDark;
   m_clrBkDisabled = GetSysColor( COLOR_BTNFACE );
   ModifyStyle( 0, BS_OWNERDRAW );
   Invalidate( FALSE );
}

void
ZPushButton::ResetColor( )
{
   m_clrText = GetSysColor( COLOR_BTNTEXT );
   m_clrBk = GetSysColor( COLOR_BTNFACE );
   m_clrBkDisabled = m_clrBk;
   m_clrLight = GetSysColor( COLOR_3DLIGHT );
   m_clrHighlight = GetSysColor( COLOR_BTNHIGHLIGHT );
   m_clrShadow = GetSysColor( COLOR_BTNSHADOW );
   m_clrDarkShadow = GetSysColor( COLOR_3DDKSHADOW );
   ModifyStyle( BS_OWNERDRAW, 0 );

   Invalidate( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// ZPushButton message handlers
void
ZPushButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
#if 0 //def DEBUG_ALL lots of messages
   if ( zstrcmp( *m_pzsTag, "PrintPreview" ) == 0 )
      TraceLineS( "ZPushButton::DrawButtonText: ", *m_pzsTag );
#endif
   CDC   *pDC;
   CRect rectFocus;
   CRect rectButton;
   CRect rectText;
   CRect rectTextOffset;
   UINT  uState;

   pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
   uState = lpDrawItemStruct->itemState;

   rectFocus.CopyRect( &lpDrawItemStruct->rcItem );
   rectButton.CopyRect( &lpDrawItemStruct->rcItem );

   rectText = rectButton;
   rectText.OffsetRect( -1, -1 );
   rectTextOffset = rectText;
   rectTextOffset.OffsetRect( 1, 1 );

   // Set the focus rectangle to just past the border decoration.
   rectFocus.left += 4;
   rectFocus.right -= 4;
   rectFocus.top += 4;
   rectFocus.bottom -= 4;

// // Retrieve the button's caption
// const int bufSize = 512;
// TCHAR buffer[ bufSize ];
// GetWindowText( buffer, bufSize );

   if ( uState & ODS_DISABLED )
   {
      DrawFilledRect( pDC, rectButton, m_clrBkDisabled );
   }
   else
   {
      DrawFilledRect( pDC, rectButton, m_clrBk );
   }

   if ( uState & ODS_SELECTED )
   {
      DrawFrame( pDC, rectButton, BUTTON_IN );
   }
   else
   {
      if ( (uState & ODS_DEFAULT) || (uState & ODS_FOCUS) )
      {
         DrawFrame( pDC, rectButton, BUTTON_OUT | BUTTON_BLACK_BORDER );
      }
      else
      {
         DrawFrame( pDC, rectButton, BUTTON_OUT );
      }
   }

   if ( uState & ODS_DISABLED )
   {
      DrawButtonText( pDC, rectText, *m_pzsText, CLR_BTN_DGREY );
      DrawButtonText( pDC, rectTextOffset, *m_pzsText, CLR_BTN_WHITE );
   }
   else
   {
      if ( uState & ODS_SELECTED )
      {
         if ( (Attr.Style & BS_MULTILINE) == BS_MULTILINE )
            rectTextOffset.OffsetRect( 1, 0 );

         DrawButtonText( pDC, rectTextOffset, *m_pzsText, m_clrText );
      }
      else
      {
         DrawButtonText( pDC, rectText, *m_pzsText, m_clrText );
      }
   }

   if ( uState & ODS_FOCUS )
   {
      DrawFocusRect( lpDrawItemStruct->hDC, (LPRECT) &rectFocus );
   }
}

void
ZPushButton::DrawFrame( CDC *pDC, CRect rect, UINT uState )
{
   COLORREF clr;

   if ( uState & BUTTON_BLACK_BORDER )
   {
      clr = CLR_BTN_BLACK;

      DrawLine( pDC, rect.left, rect.top, rect.right, rect.top, clr );   // Across top
      DrawLine( pDC, rect.left, rect.top, rect.left, rect.bottom, clr ); // Down left

      DrawLine( pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, clr ); // Across bottom
      DrawLine( pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, clr );   // Down right

      rect.InflateRect( -1, -1 );
   }

   if ( uState & BUTTON_OUT )
   {
      clr = m_clrHighlight;

      DrawLine( pDC, rect.left, rect.top, rect.right, rect.top, clr ); // Across top
      DrawLine( pDC, rect.left, rect.top, rect.left, rect.bottom, clr ); // Down left

      clr = m_clrDarkShadow;

      DrawLine( pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, clr ); // Across bottom
      DrawLine( pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, clr ); // Down right

      rect.InflateRect( -1, -1 );

      clr = m_clrLight;

      DrawLine( pDC, rect.left, rect.top, rect.right, rect.top, clr ); // Across top
      DrawLine( pDC, rect.left, rect.top, rect.left, rect.bottom, clr ); // Down left

      clr = m_clrShadow;

      DrawLine( pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, clr ); // Across bottom
      DrawLine( pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, clr ); // Down right
   }

   if ( uState & BUTTON_IN )
   {
      clr = m_clrDarkShadow;

      DrawLine( pDC, rect.left, rect.top, rect.right, rect.top, clr ); // Across top
      DrawLine( pDC, rect.left, rect.top, rect.left, rect.bottom, clr ); // Down left
      DrawLine( pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, clr ); // Across bottom
      DrawLine( pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, clr ); // Down right

      rect.InflateRect( -1, -1 );

      clr = m_clrShadow;

      DrawLine( pDC, rect.left, rect.top, rect.right, rect.top, clr ); // Across top
      DrawLine( pDC, rect.left, rect.top, rect.left, rect.bottom, clr ); // Down left
      DrawLine( pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, clr ); // Across bottom
      DrawLine( pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, clr ); // Down right
   }
}

void
ZPushButton::DrawFilledRect( CDC *pDC, CRect rect, COLORREF clr )
{
   CBrush brush;

   brush.CreateSolidBrush( clr );
   pDC->FillRect( rect, &brush );
}

void
ZPushButton::DrawLine( CDC *pDC, zLONG lLeft, zLONG lTop,
                       zLONG lRight, zLONG lBottom, COLORREF clr )
{
   CPen penNew;
   CPen *pPenOld;

   penNew.CreatePen( PS_SOLID, 1, clr );
   pPenOld = pDC->SelectObject( &penNew );
   pDC->MoveTo( lLeft, lTop );
   pDC->LineTo( lRight, lBottom );
   pDC->SelectObject( pPenOld );
   penNew.DeleteObject( );
}

CSize
ZPushButton::GetLargestTextSize( CDC *pDC, CStringArray& arrayText )
{
   CSize sizeMax( 0, 0 );

   for ( zSHORT k = 0; k < arrayText.GetSize( ); k++ )
   {
      CSize   size;

      size = pDC->GetTextExtent( arrayText.GetAt( k ) );
      if ( size.cx == 0 )
         continue;

      if ( sizeMax.cx < size.cx )
         sizeMax.cx = size.cx;

      if ( sizeMax.cy < size.cy )
         sizeMax.cy = size.cy;
   }

   return( sizeMax );
}

void
ZPushButton::FormatMultiLineText( CStringArray& arrayText,
                                  CDC           *pDC,
                                  zCPCHAR       cpcText,
                                  CRect&        rectClient )
{
   // Split text into a list of text lines (for multiline support).
   CSize  size;
   CSize  sizeExtent;
   zLONG  lLineCnt = 0;
   zPCHAR pchNewLine;
   zPCHAR pchSpace;
   zPCHAR pch;
   zCHAR  ch;

   size.cx = rectClient.Width( );
   size.cy = rectClient.Height( );

   zLONG  lLth = (zstrlen( cpcText ) + 1) * 2;
   zPCHAR pchText = new char[ lLth ];
   strcpy_s( pchText, lLth, cpcText );
   pch = pchText;
   while ( pch && *pch )
   {
      if ( (pchNewLine = zstrchr( pch, '\n' )) != 0 )
         *pchNewLine = 0;

      lLth = zstrlen( pch );
      ch = 0;
      while ( lLth > 0 )
      {
         sizeExtent = pDC->GetTextExtent( pch, lLth );
         if ( lLth <= 1 || sizeExtent.cx < size.cx )
         {
            if ( ch && ch != ' ' )
            {
               pchSpace = zstrrchr( pch, ' ' );
               if ( pchSpace )
               {
                  pch[ lLth ] = ch;
                  *pchSpace = 0;
                  lLth = pchSpace - pch;
                  ch = ' ';
               }
            }

            lLineCnt++;
            arrayText.Add( pch );
            pch[ lLth ] = ch;
            pch = pch + lLth;
            while ( *pch == ' ' )
               pch++;

            lLth = zstrlen( pch );
            ch = 0;
         }
         else
         {
            pch[ lLth ] = ch;
            lLth--;
            ch = pch[ lLth ];
            pch[ lLth ] = 0;
         }
      }

      pch[ lLth ] = ch;
      if ( pchNewLine )
         pch = pchNewLine + 1;
   }

   delete( pchText );
}

void
ZPushButton::DrawButtonText( CDC *pDC, CRect rect,
                             zCPCHAR cpcBuffer, COLORREF clrText )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "PrintPreview" ) == 0 )
      TraceRect( "ZPushButton::DrawButtonText PrintPreview: ", rect );
#endif

   COLORREF clrPrev = pDC->SetTextColor( clrText );
   pDC->SetBkMode( TRANSPARENT );
   if ( (Attr.Style & BS_MULTILINE) == BS_MULTILINE )
   {
      CStringArray arrayText;
      CRect rectTemp( rect );
      CSize sizeExtent;
   // UINT uAlign = pDC->GetTextAlign( ); // this doesn't work ==> blank text

   // if ( (uAlign & TA_UPDATECP) == 0 )
   //    pDC->SetTextAlign( uAlign | TA_UPDATECP );

#if 1  // Phil

      FormatMultiLineText( arrayText, pDC, cpcBuffer, rectTemp );
      CSize sizeText = GetLargestTextSize( pDC, arrayText );

      // Draw the list of text.
      zSHORT nSize = arrayText.GetSize( );
      zSHORT nOffsetY = (zSHORT) (rect.Height( ) - (nSize * sizeText.cy)) / 2;
      if ( nOffsetY < -1 )
         nOffsetY = 0;

      for ( zLONG k = 0; k < nSize; k++ )
      {
         rectTemp.left = 0;
         rectTemp.top = rect.top + k * sizeText.cy + nOffsetY;
         rectTemp.right = rect.right;
         rectTemp.bottom = rectTemp.top + sizeText.cy;

         sizeExtent = pDC->GetTextExtent( arrayText.GetAt( k ) );
         sizeExtent.cy = rectTemp.Width( );  // "abuse" cy for a minute
         if ( sizeExtent.cx < sizeExtent.cy )
         {
            sizeExtent.cx = (sizeExtent.cy - sizeExtent.cx) / 2;
            rectTemp.left += sizeExtent.cx;
            rectTemp.right -= sizeExtent.cx;
         }

         if ( (GetStyle( ) & WS_DISABLED) == WS_DISABLED )
         {
            pDC->SetTextColor( ::GetSysColor( COLOR_3DHIGHLIGHT ) );
            rectTemp.left += 1;
            rectTemp.top += 1;
            rectTemp.right += 1;
            rectTemp.bottom += 1;
            pDC->DrawText( arrayText.GetAt( k ), -1, &rectTemp, DT_CENTER | DT_VCENTER );

            pDC->SetTextColor( ::GetSysColor( COLOR_3DSHADOW ) );
         }

         pDC->DrawText( arrayText.GetAt( k ), -1, &rectTemp,  DT_CENTER | DT_VCENTER );
      }

#else
      zLONG lBlank;

      pDC->DrawText( pchText, zstrlen( pchText ),
                     rectTemp, DT_CENTER | DT_VCENTER | DT_CALCRECT );
      lBlank = (rect.top - rectTemp.top) + (rect.bottom - rectTemp.bottom);
      lBlank /= 2;
      rectTemp.top += lBlank;
      rectTemp.bottom += lBlank;
      lBlank = (rect.left - rectTemp.left) + (rect.right - rectTemp.right);
      lBlank /= 2;
      rectTemp.left += lBlank;
      rectTemp.right += lBlank;
      pDC->DrawText( cpcBuffer, zstrlen( cpcBuffer ), rectTemp, DT_CENTER | DT_VCENTER );
   // if ( (uAlign & TA_UPDATECP) == 0 )
   //    pDC->SetTextAlign( uAlign );

#endif
   }
   else
   {
      pDC->DrawText( cpcBuffer, zstrlen( cpcBuffer ), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
   }

   pDC->SetTextColor( clrPrev );
}
void
ZPushButton::DrawLine( CDC *pDC, CRect rectEndPoints, COLORREF clr )
{
   CPen penNew;
   penNew.CreatePen( PS_SOLID, 1, clr );
   CPen *pPenOld = pDC->SelectObject( &penNew );
   pDC->MoveTo( rectEndPoints.left, rectEndPoints.top );
   pDC->LineTo( rectEndPoints.right, rectEndPoints.bottom );
   pDC->SelectObject( pPenOld );
   penNew.DeleteObject( );
}

#if 0
void
ZPushButton::DrawItem( LPDRAWITEMSTRUCT lpDIS )
{
   CDC *pDC = CDC::FromHandle( lpDIS->hDC );

   UINT  uState = lpDIS->itemState;
   CRect rectFocus;
   CRect rectBtn;

   rectFocus.CopyRect( &lpDIS->rcItem );
   rectBtn.CopyRect( &lpDIS->rcItem );
   CRect rectText( rectBtn );
   rectText.OffsetRect( -1, -1 );

   // Set the focus rectangle to just past the border decoration.
   rectFocus.left += 4;
   rectFocus.right -= 4;
   rectFocus.top += 4;
   rectFocus.bottom -= 4;

   // Retrieve the button's caption.
// const zSHORT nLth = 512;
// TCHAR szBuffer[ nLth ];
// GetWindowText( szBuffer, nLth );

   // Draw and label the button using draw methods.
   DrawFilledRect( pDC, rectBtn, GetBkColor( ) );
   DrawFrame( pDC, rectBtn, GetBevel( ) );

   // Now, depending upon the state, redraw the button (down image) if it
   // is selected, place a focus rectangle on it, or redisplay the caption
   // if it is disabled.
   if ( uState & ODS_FOCUS )
   {
      DrawFocusRect( lpDIS->hDC, (LPRECT) &rectFocus );
      if ( uState & ODS_SELECTED )
      {
         DrawFilledRect( pDC, rectBtn, GetBkColor( ) );
         DrawFrame( pDC, rectBtn, -1 );
         DrawButtonText( pDC, rectBtn, *m_pzsText, GetTextColor( ) );
         DrawFocusRect( lpDIS->hDC, (LPRECT) &rectFocus );
      }
   }
   else
   if ( uState & ODS_DISABLED )
   {
   // COLORREF clrDisabled = m_pZSubtask->m_clrBk ^ 0xFFFFFF;  // contrasting color
   // DrawButtonText( pDC, rectBtn, *m_pzsText, clrDisabled ); // szBuffer, GetDisabledColor( ) );

   // DrawButtonText( pDC, rectBtn, *m_pzsText, GetDisabledColor( ) );

   // DrawButtonText( pDC, rectBtn, *m_pzsText, GetTextColor( ) );
   // rectBtn.left++; rectBtn.right++;// rectBtn.top++; rectBtn.bottom++;
   // DrawButtonText( pDC, rectBtn, *m_pzsText, ::GetSysColor( COLOR_3DLIGHT ) );

      DrawButtonText( pDC, rectText, *m_pzsText, CLR_BTN_DGREY );
      DrawButtonText( pDC, rectBtn, *m_pzsText, CLR_BTN_WHITE );
   }
   else
   {
      DrawButtonText( pDC, rectBtn, *m_pzsText, GetTextColor( ) );
   }
}

void
ZPushButton::DrawFrame( CDC *pDC, CRect rect, UINT Inset )
{
   COLORREF clrDark, clrLight, clrTopLeft, clrBottomRight;
   int k, m, width;
   width = (Inset < 0) ? -Inset : Inset;

   for ( k = 0; k < width; k += 1 )
   {
      m = 255 / (k + 2);
      clrDark = PALETTERGB( m, m, m );
      m = 192 + (63 / (k + 1));
      clrLight = PALETTERGB( m, m, m );

      if ( width == 1 )
      {
         clrLight = RGB( 255, 255, 255 );
         clrDark = RGB( 128, 128, 128 );
      }

      if ( Inset < 0 )
      {
         clrTopLeft = clrDark;
         clrBottomRight = clrLight;
      }
      else
      {
         clrTopLeft = clrLight;
         clrBottomRight = clrDark;
      }

      DrawLine( pDC, rect.left, rect.top, rect.right, rect.top, clrTopLeft );      // Across top
      DrawLine( pDC, rect.left, rect.top,  rect.left, rect.bottom, clrTopLeft ); // Down left

      if ( (Inset < 0) && (k == width - 1) && (width > 1) )
      {
         DrawLine( pDC, rect.left + 1, rect.bottom - 1, rect.right, rect.bottom - 1, RGB( 1, 1, 1 ) ); // Across bottom
         DrawLine( pDC, rect.right - 1, rect.top + 1, rect.right - 1, rect.bottom, RGB( 1, 1, 1 ) ); // Down right
      }
      else
      {
         DrawLine( pDC, rect.left + 1, rect.bottom - 1, rect.right, rect.bottom - 1, clrBottomRight );     // Across bottom
         DrawLine( pDC, rect.right - 1, rect.top + 1, rect.right - 1, rect.bottom, clrBottomRight );     // Down right
      }

      InflateRect( rect, -1, -1 );
   }
}

void
ZPushButton::DrawFilledRect( CDC *pDC, CRect rect, COLORREF clr )
{
   CBrush brush;
   brush.CreateSolidBrush( clr );
   pDC->FillRect( rect, &brush );
}

void
ZPushButton::DrawLine( CDC *pDC, zLONG lLeft, zLONG lTop,
                       zLONG lRight, zLONG lBottom, COLORREF clr )
{
   CPen penNew;
   penNew.CreatePen( PS_SOLID, 1, clr );
   CPen *pPenOld = pDC->SelectObject( &penNew );
   pDC->MoveTo( lLeft, lTop );
   pDC->LineTo( lRight, lBottom );
   pDC->SelectObject( pPenOld );
   penNew.DeleteObject( );
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
PushButton( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   if ( pCtrlDef->Tag[ 0 ] == '_' &&
        pCtrlDef->Tag[ 1 ] == '_' )
   {
      if ( pCtrlDef->Tag[ 2 ] == 'F' &&
           pCtrlDef->Tag[ 3 ] == 'o' &&
           pCtrlDef->Tag[ 4 ] == 'n' &&
           pCtrlDef->Tag[ 5 ] == 't' )
      {
         return( new ZFontPicker( pZSubtask, pWndParent,
                                  pzmaComposite, vDialog,
                                  nOffsetX, nOffsetY, pCtrlDef ) );
	  }
   }

   return( new ZPushButton( pZSubtask, pWndParent,
                            pzmaComposite, vDialog,
                            nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=BTN_SetColor
// Source Module=zdctlbtn.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: BTN_SetColor
//                                           2003/06/01 Modified: 2003/06/01
//
//  PURPOSE:    This operation sets the text and background button colors.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the button control.
//              cpcCtrlTag  - The unique name of the control.
//              lClrText    - The RGB color in which text is displayed.
//              lClrBack    - The RGB color in which to display the background.
//              lClrBackHigh - The RGB color in which to highlight the background.
//              lClrBackDark - The RGB color in which to shade the background.
//
//  RETURNS:    0 - Colors set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
BTN_SetColor( zVIEW   vSubtask,
              zCPCHAR cpcCtrlTag,
              zLONG   lClrText,
              zLONG   lClrBack,
              zLONG   lClrBackHigh,
              zLONG   lClrBackDark )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZPushButton *pButton = DYNAMIC_DOWNCAST( ZPushButton, pzma->m_pCtrl );
      if ( pButton )
      {
         pButton->SetColor( lClrText, lClrBack, lClrBackHigh, lClrBackDark );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for BTN_SetColor ", cpcCtrlTag );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Constant and should not be changed unless image IDB_GLYPHS is changed
#define GLYPH_WIDTH 15

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         //
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       //
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZFontPicker

IMPLEMENT_DYNAMIC( ZFontPicker, CComboBox )

BEGIN_MESSAGE_MAP( ZFontPicker, CComboBox )
   //{{AFX_MSG_MAP( ZFontPicker )
   ON_WM_CREATE( )
   ON_CONTROL_REFLECT( CBN_KILLFOCUS, CBNKillFocus )
   ON_CONTROL_REFLECT( CBN_SETFOCUS, CBNSetFocus )
   ON_CONTROL_REFLECT( CBN_CLOSEUP, CBNCloseUp )
   ON_CONTROL_REFLECT( CBN_SELCHANGE, CBNSelchange )
   ON_WM_TIMER()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

// ZFontPicker - ctor
ZFontPicker::ZFontPicker( ZSubtask *pZSubtask,
                          CWnd     *pWndParent,
                          ZMapAct  *pzmaComposite,
                          zVIEW    vDialog,
                          zSHORT   nOffsetX,
                          zSHORT   nOffsetY,
                          zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
             CComboBox( ),
             ZMapAct( pZSubtask,          // base class ctor
                      pzmaComposite,
                      pWndParent,
                      this,
                      vDialog,
                      nOffsetX,
                      nOffsetY,
                      pCtrlDef,
                      "Font" )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZFontPicker::ctor ", *m_pzsTag );
#endif

   // Load up glyphs.
   m_ImageList.Create( IDB_GLYPHS, 15, 1, RGB( 255, 0, 255 ) );

   Attr.Style = CBS_DROPDOWN | /* CBS_OWNERDRAWFIXED | */ CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 || (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
		// KJS 11/22/16 - Right now, this is taking a few seconds to build. On the web we don't use the font field
	    // attribute so I am currently taking this out to speed things up.
        //CreateZ( );
   }
}

void
ZFontPicker::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZFontPicker::CreateZ ", *m_pzsTag );
#endif

   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

   if ( m_ulMapActFlags & zMAPACT_ENABLED )
      Attr.Style &= ~WS_DISABLED;
   else
      Attr.Style |= WS_DISABLED;

   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( Attr.Style,
                 CRect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + 9 * Attr.H ),
                 m_pWndParent, m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
}

/////////////////////////////////////////////////////////////////////////////
// ZFontPicker message handlers
void
ZFontPicker::DeleteItem( LPDELETEITEMSTRUCT lpDIS )
{
}

// Overridables for ownerdraw combos.
void
ZFontPicker::MeasureItem( LPMEASUREITEMSTRUCT lpMIS )
{
}

// Initialize fonts for combo.
int
ZFontPicker::OnCreate( LPCREATESTRUCT lpCS )
{
   if ( CComboBox::OnCreate( lpCS ) == -1 )
      return( -1 );

   SetFontOverride( );
   Initialize( );
   return( 0 );
}

zSHORT
ZFontPicker::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZFontPicker::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW   vApp;

      if ( GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
         {
            zCHAR  szFont[ 256 ];
            zSHORT nRC = GetVariableFromAttribute( szFont, 0, zTYPE_STRING,
                                                   zsizeof( szFont ),
                                                   vApp, *m_pzsEName,
                                                   *m_pzsAName, *m_pzsContext,
                                                   m_pzsContext->IsEmpty( ) ?
                                          zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT : zACCEPT_NULL_ENTITY );
            if ( nRC == zVAR_NULL )
               szFont[ 0 ] = 0;

            SetZCtrlText( this, szFont );
         // SetWindowText( szFont );
            return( 0 );
         }
      }
   }

   return( -1 );
}

// Get the text for the entry control and map it to the data OI.
zSHORT
ZFontPicker::MapToOI( zLONG lFlag )
{
   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineS( "ZFontPicker::MapToOI: ", *m_pzsTag );
#endif

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      zCHAR   szFont[ 256 ];
      zSHORT  nRC;

      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      GetZCtrlText( this, szFont, zsizeof( szFont ) );
      nRC = SetAttributeFromVariable( vApp, *m_pzsEName,
                                      *m_pzsAName, szFont,
                                      zTYPE_STRING, 0,
                                      *m_pzsContext,
                                      m_pzsContext->IsEmpty( ) ?
                                      (zSHORT) (lFlag | zUSE_DEFAULT_CONTEXT) : (zSHORT) lFlag );
      return( nRC );
   }

   return( 0 );
}

// Owner draw to render bitmap and font name.
void
ZFontPicker::DrawItem( LPDRAWITEMSTRUCT lpDIS )
{
   ASSERT( lpDIS->CtlType == ODT_COMBOBOX ); // we've gotta be a combo

   // Lets make a CDC for ease of use.
   CDC *pDC = CDC::FromHandle( lpDIS->hDC );

   ASSERT( pDC ); // Attach failed

   CRect rect( lpDIS->rcItem );

   // Draw focus rectangle.
   if ( lpDIS->itemState & ODS_FOCUS )
      pDC->DrawFocusRect( rect );

   // Save off context attributes.
   int nIndexDC = pDC->SaveDC( );

   CBrush brushFill;

   // Draw selection state.
   if ( lpDIS->itemState & ODS_SELECTED )
   {
      brushFill.CreateSolidBrush( ::GetSysColor( COLOR_HIGHLIGHT ) );
      pDC->SetTextColor( ::GetSysColor( COLOR_HIGHLIGHTTEXT ) );
   }
   else
      brushFill.CreateSolidBrush( pDC->GetBkColor( ) );

   pDC->SetBkMode( TRANSPARENT );
   pDC->FillRect( rect, &brushFill );

   CString csCurrFont;
   CString csNextFont;
   GetLBText( lpDIS->itemID, csCurrFont );

   ZCorrelatedItem *pItem;
   CFontObj *pFontObj;
   pItem = m_FontList.Find( csCurrFont );
   pFontObj = (CFontObj *) pItem->m_lNumber;

   ASSERT( pFontObj );
   DWORD dwData = pFontObj->GetFlags( );

   // Render Bitmaps.
   if ( dwData & TRUETYPE_FONT )
   {
      m_ImageList.Draw( pDC, 1, CPoint( rect.left, rect.top ), ILD_TRANSPARENT );
   }

   if ( dwData & PRINTER_FONT )
   {
      m_ImageList.Draw( pDC, 0, CPoint( rect.left, rect.top ), ILD_TRANSPARENT );
   }

   int nX = rect.left; // save for lines

   rect.left += GLYPH_WIDTH + 2; // Text Position
   pDC->TextOut( rect.left, rect.top, csCurrFont );

   // GetItemData - return font in use.
   if ( GetItemData( lpDIS->itemID ) )
   {
      GetLBText( lpDIS->itemID + 1, csNextFont );

      pItem = m_FontList.Find( csNextFont );
      pFontObj = (CFontObj *) pItem->m_lNumber;

      if ( GetItemData( lpDIS->itemID + 1 ) == 0 )
      {
         // Draw font MRU separator ==============
         TEXTMETRIC tm;
         pDC->GetTextMetrics( &tm );

         pDC->MoveTo( nX, rect.top + tm.tmHeight );
         pDC->LineTo( rect.right, rect.top + tm.tmHeight );

         pDC->MoveTo( nX, rect.top + tm.tmHeight + 2 );
         pDC->LineTo( rect.right, rect.top + tm.tmHeight + 2 );
      }
   }

   // Restore State of context.
   pDC->RestoreDC( nIndexDC );
}

// As it says ... EnumerateFonts on the system.
BOOL
ZFontPicker::EnumerateFonts( )
{
   HDC     hDC;
   LOGFONT lf;
   BOOL    bRC = TRUE;

   // Get screen fonts.
   hDC = ::GetWindowDC( 0 );
   zmemset( &lf, 0, sizeof( lf ) );
   lf.lfCharSet = ANSI_CHARSET;

   if ( EnumFontFamiliesEx( hDC,  // handle to device context
                            &lf,  // pointer to logical font information
                            (FONTENUMPROC) EnumFamScreenCallBackEx, // ptr to callback
                            (LPARAM) this, // application-supplied data
                            (DWORD) 0 ) == 0 )
   {
      bRC = FALSE;
   }

   ::ReleaseDC( 0, hDC );
   if ( bRC == FALSE )
      return( FALSE );

   // Now get printer fonts.
   CPrintDialog dlg( FALSE );
   if ( AfxGetApp( )->GetPrinterDeviceDefaults( &dlg.m_pd ) )
   {
      // GetPrinterDC returns an HDC so attach it.
      hDC = dlg.CreatePrinterDC( );
      ASSERT( hDC );

      zmemset( &lf, 0, sizeof( lf ) );
      lf.lfCharSet = ANSI_CHARSET;

      if ( EnumFontFamiliesEx( hDC,  // handle to device context
                               &lf,  // pointer to logical font information
                               (FONTENUMPROC) EnumFamPrinterCallBackEx,   // ptr to callback
                               (LPARAM) this, // application supplied data
                               (DWORD) 0 ) == 0 )
      {
         bRC = FALSE;
      }

      // Clean up.
      CDC::FromHandle( hDC )->DeleteDC( );
   }

   return( bRC );
}

BOOL
ZFontPicker::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CComboBox::DestroyWindow( ) );
}

// Flags a font that is in use, similar to MS Words font MRU list.
void
ZFontPicker::SetFontInUse( const CString& csFont )
{
   if ( csFont.IsEmpty( ) == FALSE )
   {
      ZCorrelatedItem *pItem;

      // Find the desired font.
      pItem = m_FontList.Find( csFont );
      if ( pItem && pItem->m_lNumber )
      {
         // Check the font is not already in use.
         CFontObj *pFontObj = (CFontObj *) pItem->m_lNumber;
         if ( pFontObj->GetFontInUse( ) == FALSE )
         {
            // Set the font as the top most and selected font.
            pFontObj->SetFontInUse( TRUE );
            InsertString( 0, csFont );
            SetItemData( 0, DWORD( 1 ) );

            // Only hold six MRU'ed fonts, so drop the 7th.
            if ( GetItemData( 6 ) )
            {
               CString  csFontDesc;

               GetLBText( 6, csFontDesc );
               if ( csFontDesc.IsEmpty( ) == FALSE )
               {
                  pItem = m_FontList.Find( csFontDesc );
                  if ( pItem && pItem->m_lNumber )
                  {
                     pFontObj = (CFontObj *) pItem->m_lNumber;
                     pFontObj->SetFontInUse( FALSE );
                     DeleteString( 6 );
                  }
               }
            }
         }
         else
         {
            // If font is aleady MRU'ed position it as the first font.
            int nSel = FindString( -1, csFont );

            if ( nSel != CB_ERR )
            {
               // Remove from original position.
               DeleteString( nSel );

               // Restore to begining of list
               InsertString( 0, csFont );
               SetItemData( 0, TRUE ); // mark as selected
               SetCurSel( 0 );  // bring as current selection
            }
         }
      }
   }
}

void
ZFontPicker::CBNKillFocus( )
{
   SetCurrentFont( );
}

// Save current font.
void
ZFontPicker::CBNSetFocus( )
{
   // Save off original font.
   GetWindowText( m_csFontSave );
}

// Set up the current font.
void
ZFontPicker::SetCurrentFont( )
{
   CString csSelFont;

   // Ensure font in edit box is a known font, otherwise reject the current
   // font and restore orginal.
   int nSel = GetCurSel( );

   if ( nSel == CB_ERR )
   {
      GetWindowText( csSelFont );
      nSel = FindStringExact( -1, csSelFont );

      if ( nSel == CB_ERR )
      {
         SetWindowText( m_csFontSave );
      }
   }
}

// Hide tip window and set font name.
void
ZFontPicker::CBNCloseUp( )
{
   int nSel;
   CString csFont;

   // Set Face Name
   SetCurrentFont( );
   nSel = GetCurSel( );
   if ( nSel != CB_ERR )
      GetLBText( nSel, csFont );

   SetFontInUse( csFont );
   m_wndToolTip.ShowWindow( SW_HIDE );
}

void
ZFontPicker::CBNSelchange( )
{
   ProcessEvent( this, 1 );
}

void
ZFontPicker::Initialize( )
{
   m_wndToolTip.Create( this, *m_pzsText );  // create tip window here
// CString csDefault = "";   // set default font name

   CFontObj *pFontObj;
   CString  csComp;
   zLONG    lMax;
   zLONG    lIdx;
   zBOOL    bInsert;

   AddFont( CString( "" ), 0 );  // add blank font
   EnumerateFonts( );

   ZCorrelatedItem *pItem = m_FontList.m_pHeadItem;
   while ( pItem )
   {
      pFontObj = (CFontObj *) pItem->m_lNumber;

      bInsert = FALSE;
      lMax = GetCount( );
      for ( lIdx = 0; lIdx < lMax; lIdx++ )
      {
         GetLBText( lIdx, csComp );

         if ( csComp.Collate( pItem->m_csValue ) == 1 )
         {
            bInsert = TRUE;
            InsertString( lIdx, pItem->m_csValue );
            break;
         }
      }

      if ( bInsert == FALSE )
         AddString( pItem->m_csValue );

      pItem = pItem->m_pNextItem;
   }

   // We set the timer because it's the only way we know when a selection has changed - use for tip window.
   SetTimer( 1, 500, 0 );
}

void
ZFontPicker::AddFont( CString csFontName, DWORD dwFlags )
{
   // Add fonts not aleady in the array.
   CFontObj *pFontObj;

   ZCorrelatedItem *pItem;
   pItem = m_FontList.Find( csFontName );
   if ( pItem == 0 || pItem->m_lNumber == 0 )
   {
      if ( pItem )
         TraceLineS( "ZFontPicker::AddFont error: ", pItem->m_csValue );

      pFontObj = new CFontObj( dwFlags );
      m_FontList.Add( (zLONG) pFontObj, csFontName );
   }
}

// Positions tip window against selected font.
void
ZFontPicker::OnTimer( UINT uIdEvent )
{
   // Is combo open?
   if ( GetDroppedState( ) )
   {
      int nSel = GetCurSel( );

      // Selected.
      if ( nSel != -1 )
      {
         CString cs;
         CRect rect;

         GetLBText( nSel, cs );
         GetDroppedControlRect( rect );
         zSHORT nHeight = GetItemHeight( 0 ) * ((nSel - GetTopIndex( )) + 1);

         CPoint pt( rect.right + 5, rect.top + nHeight );

         // Show tip in correct position.
         m_wndToolTip.ShowTips( pt, cs );
      }
   }

   CComboBox::OnTimer( uIdEvent );
}

BOOL CALLBACK AFX_EXPORT
ZFontPicker::EnumFamScreenCallBackEx( ENUMLOGFONTEX *pelf,
                                      NEWTEXTMETRICEX *lpntm,
                                      int FontType, LPVOID pThis )
{
   // Don't put in non-printer raster fonts.
   if ( FontType & RASTER_FONTTYPE )
      return( 1 );

   DWORD dwData = (FontType & TRUETYPE_FONTTYPE) ? TRUETYPE_FONT : 0;
   ((ZFontPicker *) pThis)->AddFont( pelf->elfLogFont.lfFaceName, dwData );

   return( 1 ); // Call me back
}

BOOL CALLBACK AFX_EXPORT
ZFontPicker::EnumFamPrinterCallBackEx( ENUMLOGFONTEX *pelf,
                                       NEWTEXTMETRICEX *lpntm,
                                       int nFontType, LPVOID pThis )
{
   if ( (nFontType & DEVICE_FONTTYPE) == 0 )
      return( 1 );

   if ( (nFontType & TRUETYPE_FONTTYPE) )
      return( 1 );

   DWORD dwData = PRINTER_FONT;

   ((ZFontPicker *) pThis)->AddFont( pelf->elfLogFont.lfFaceName, dwData );

   return( 1 ); // Call me back again
}

ZFontPicker::~ZFontPicker()
{
#ifdef DEBUG_ALL
   TraceLineS( "ZFontPicker::dtor ", *m_pzsTag );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      KillTimer( 1 );

   ZCorrelatedItem *pItem = m_FontList.m_pHeadItem;
   ZCorrelatedItem *pNextItem;
   CFontObj *pFontObj;

   m_FontList.m_pHeadItem = 0;
   while ( pItem )
   {
      pNextItem = pItem->m_pNextItem;
      pItem->m_pNextItem = 0;
      pFontObj = (CFontObj *) pItem->m_lNumber;
      delete( pFontObj );
      delete( pItem );
      pItem = pNextItem;
   }

   if ( mIs_hWnd( m_wndToolTip.m_hWnd ) )
      m_wndToolTip.DestroyWindow( );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// TipWnd.cpp : implementation file
//

#define CONTROLTOOLTIP_CLASSNAME  "ZeidonToolTip"
#define FONT_HEIGHT -18

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         //
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       //
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// ZToolTip

BEGIN_MESSAGE_MAP( ZToolTip, CWnd )
   //{{AFX_MSG_MAP( ZToolTip )
   ON_WM_ERASEBKGND( )
   ON_WM_PAINT( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZToolTip::ZToolTip( ) :
          m_sizeExtent( 0, 0 )
{
   m_bResetFont = TRUE;
   m_nFontHeight = FONT_HEIGHT;
}

ZToolTip::~ZToolTip( )
{
}

BOOL
ZToolTip::Create( CWnd *pParent, zCPCHAR cpcText,
                  zCPCHAR cpcFont, int nFontHeight )
{
   if ( cpcText && *cpcText )
      m_csText = cpcText;

   if ( cpcFont && *cpcFont )
   {
      m_csFont = cpcFont;
      m_bResetFont = -1;
      if ( nFontHeight )
         m_nFontHeight = nFontHeight;
   }

   WNDCLASS  WndClass;
   HINSTANCE hInst = AfxGetInstanceHandle( );

   if ( (::GetClassInfo( hInst, CONTROLTOOLTIP_CLASSNAME, &WndClass )) == 0 )
   {
      WndClass.style = CS_SAVEBITS | CS_DBLCLKS;
      WndClass.lpfnWndProc = ::DefWindowProc;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 0;
      WndClass.hInstance = hInst;
      WndClass.hIcon = 0;
      WndClass.hCursor = LoadCursor( hInst, IDC_ARROW );
      WndClass.hbrBackground = (HBRUSH) (COLOR_INFOBK + 1);
      WndClass.lpszMenuName = 0;
      WndClass.lpszClassName = CONTROLTOOLTIP_CLASSNAME;

      if ( AfxRegisterClass( &WndClass ) == 0 )
         AfxThrowResourceException( );
   }

   return( CWnd::CreateEx( 0, CONTROLTOOLTIP_CLASSNAME, 0,
           WS_BORDER | WS_POPUP, 0, 0, 0, 0,
           pParent ? pParent->m_hWnd : 0, (HMENU) 0 ) );
}

/////////////////////////////////////////////////////////////////////////////
// ZToolTip message handlers

BOOL
ZToolTip::OnEraseBkgnd( CDC *pDC )
{
   CBrush br( GetSysColor( COLOR_INFOBK ) );
   CRect rect;
   pDC->GetClipBox( rect );
   CBrush *pOldBrush = pDC->SelectObject( &br );
   pDC->PatBlt( rect.left, rect.top, rect.Width( ), rect.Height( ), PATCOPY );

   pDC->SelectObject( pOldBrush );
   return( TRUE );
}

void
ZToolTip::ShowTips( CPoint pt, const CString& cs )
{
   // Create new font if the selection has changed.
   if ( m_csFont != cs )
   {
      CDC   *pDC = GetDC( );
      CFont *pOldFont = 0;

      if ( m_bResetFont )
      {
         if ( m_bResetFont == -1 )
            m_bResetFont = FALSE;
         else
            m_csFont = cs;

         LOGFONT lf;
         zmemset( &lf, 0, sizeof( lf ) );

         lf.lfHeight = m_nFontHeight;
         strcpy_s( lf.lfFaceName, zsizeof( lf.lfFaceName ), m_csFont );

         // Delete old font.
         m_font.DeleteObject( );
         m_font.CreateFontIndirect( &lf );
         pOldFont = pDC->SelectObject( &m_font );
      }

      // String dimensions of font on screen.
      CString csText;

      if ( *m_csText )
         csText = m_csText;
      else
         csText = m_csFont;

      CSize size = pDC->GetTextExtent( csText );

      // Give some space around the font.
      size.cx += 8;
      size.cy += 8;

      if ( size.cx > m_sizeExtent.cx )
         m_sizeExtent.cx = size.cx;

      if ( size.cy > m_sizeExtent.cy )
         m_sizeExtent.cy = size.cy;

      if ( pOldFont )
         pDC->SelectObject( pOldFont );

      ReleaseDC( pDC );
   // CRect rectWindow;
   // GetWindowRect( rectWindow );
   // CWnd *pParent = GetParent( );
   // pParent->ScreenToClient( rectWindow );
   // pParent->InvalidateRect( rectWindow );
      SetWindowPos( 0, pt.x, pt.y, m_sizeExtent.cx, m_sizeExtent.cy,
                    SWP_SHOWWINDOW | SWP_NOACTIVATE );
      RedrawWindow( ); // force immediate redraw
   }
}

void
ZToolTip::OnPaint( )
{
   CString  cs;
   CPaintDC dc( this ); // device context for painting

   if ( *m_csText )
      cs = m_csText;
   else
      cs = m_csFont;

   dc.SelectObject( &m_font );
   CRect rect;
   GetClientRect( rect );

   dc.DrawText( cs, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
   // Do not call CWnd::OnPaint() for painting messages
}
