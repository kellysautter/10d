/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlrb.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon RadioButton control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.20    DKS   FH
//    Set focus to correct radio button in radio button group when forced
//    by a call to ResetFocus (the control with focus is being disabled).
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.06.12    DKS    Z10
//    Fix to painting of Radio Group/Buttons as part of NoteBook painting
//    fix with large numbers of EditBoxes.
//
// 2001.05.31    BL     WEB
//    Bugfix GetCtrlState with flag zCONTROL_STATUS_CHECKED
//
// 2001.05.16    DKS
//    Font Override.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2000.03.17    TMV    WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 1999.10.17    DKS    Z10    TB784
//    Radio button group with no default now reset properly on refresh.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1999.01.05    DKS
//    Second Attempt to fix repaint problems (TB 367).
//
// 1998.12.31    DKS
//    Permit application to determine which radio button is checked by
//    querying the radio group control.
//
// 1998.10.30    DKS
//    Corrected processing when the ctrl is invisible/disabled.
//
// 1998.10.20    DKS
//    Radio buttons on a tab control handle TAB and ARROW keys properly.
//
// 1998.10.14    DKS
//    Radio buttons on a tab control stay disabled appropriately (TB 297).
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini and permit the ZoomDiagram
//    to return the current settings so that they may be stored for
//    future use.

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlRb.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZRadioBtnGroup, CButton )

BEGIN_MESSAGE_MAP( ZRadioBtnGroup, CButton )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
   ON_WM_CREATE( )
   ON_WM_ERASEBKGND( )
// ON_WM_CTLCOLOR_REFLECT( )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )

// ZRadioBtnGroup - ctor
ZRadioBtnGroup::ZRadioBtnGroup( ZSubtask *pZSubtask,
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
                         "RadioGroup" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for RadioBtnGroup: ", (zLONG) this );
#endif

   m_pZDfltRB = 0;
   m_pZCurrRB = 0;
   m_pchCurrRB_Value = 0;
   m_nFirstChild = -1;
   m_nLastChild = -1;

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;

// if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
   Attr.Style &= ~WS_TABSTOP;

// mDeleteInit( m_pBrush );
   if ( m_pBrush == 0 )
      m_pBrush = new CBrush( ::GetSysColor( COLOR_BTNFACE ) );

   // used during construction to set the radio buttons in this group to
   // the appropriate style ... used during map from OI as a first time in
   // indicator
   m_nDefault = (pCtrlDef->Subtype & zRADIOBUTTON_DEFAULT) ? 1 : 0;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 || (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }

   nOffsetX = (zSHORT) pCtrlDef->PSDLG_X;
   nOffsetY = (zSHORT) pCtrlDef->PSDLG_Y;

   zVIEW vSubobject;

   CreateViewFromViewForTask( &vSubobject, vDialog, m_pZSubtask->m_vDialog );
   SetViewToSubobject( vSubobject, szlCtrlCtrl );

#if 0 // trying to permit RadioGroup to behave like a GroupBox.

   pCtrlDef->CheckByte = 0xFE;
   pCtrlDef->lStructSize = sizeof( zKZWDLGXO_Ctrl_DEF );
   zSHORT nRC = CheckExistenceOfEntity( vSubobject, szlCtrl );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // get the structure of attributes for this control
      GetStructFromEntityAttrs( (zPCHAR) pCtrlDef, vSubobject, szlCtrl );
         if ( pCtrlDef->CheckByte != 0xFE )
            ::MessageBox( 0, "CtrlDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

      new ZRadioBtn( pZSubtask, pWndParent, this, vSubobject, nOffsetX, nOffsetY, pCtrlDef );

      nRC = SetEntityCursor( vSubobject, szlCtrl, 0, zPOS_NEXT, 0, 0, 0, 0, 0, 0 );
   }
#else

   CreateZeidonCtrls( vSubobject, pZSubtask, this, this, 0, 0, TRUE );

#endif

   DropView( vSubobject );
}

void
ZRadioBtnGroup::CreateZ( )
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
                   BS_GROUPBOX | WS_CLIPCHILDREN | Attr.Style,
                   Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
      if ( mIs_hWnd( m_hWnd ) )
         Invalidate( TRUE );
   }
#endif
}

int
ZRadioBtnGroup::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtnGroup::OnCreate", "" );
#endif

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
ZRadioBtnGroup::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CButton::DestroyWindow( ) );
}

zSHORT
ZRadioBtnGroup::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "RadioGrp1" ) == 0 )
      TraceLineS( "ZRadioBtnGroup::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;

   // Radio button groups which have a default button and NO MAPPING
   // need to get their default button checked.
   if ( m_pZDfltRB && m_pZCurrRB == 0 )    // Default Button
   {
      m_pZCurrRB = m_pZDfltRB;
      if ( m_pZCurrRB && mIs_hWnd( m_pZCurrRB->m_hWnd ) )
         m_pZDfltRB->SetCheck( 1 );
   }
   //else
   //   m_pZCurrRB = 0;

   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zVIEW vApp;

      m_uMapState = GetMappingAutoGray( &vApp, *m_pzsVName, *m_pzsEName, *m_pzsAName );
   }

   return( 0 );
}

zSHORT
ZRadioBtnGroup::MapToOI( zLONG lFlag )
{
   zVIEW   vApp;

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName && (vApp = IsVisibleForMapToOI( *m_pzsVName )) != 0 )
   {
      ZRadioBtn *pZRadioBtn = m_pZCurrRB ? m_pZCurrRB : m_pZDfltRB;
      if ( pZRadioBtn )
      {
         return( SetAttributeFromVariable( vApp, *m_pzsEName, *m_pzsAName,
                                           *(pZRadioBtn->m_pzsValueOn),
                                           zTYPE_STRING, 0, *m_pzsContext,
                                           (zSHORT) lFlag ) );
      }
      else
      {
         return( SetAttributeFromVariable( vApp, *m_pzsEName, *m_pzsAName, "",
                                           zTYPE_STRING, 0, *m_pzsContext, (zSHORT) lFlag ) );
      }
   }

   return( 0 );
}

zLONG
ZRadioBtnGroup::GetZCtrlState( zLONG lStatusType )
{
   if ( lStatusType == zCONTROL_STATUS_CHECKED )
   {
      mDeleteInitA( m_pchCurrRB_Value );
      if ( m_pZCurrRB && m_pZCurrRB->m_pzsValueOn &&
           *(m_pZCurrRB->m_pzsValueOn) )
      {
         zLONG lLth = zstrlen( *(m_pZCurrRB->m_pzsValueOn) ) + 1;
         m_pchCurrRB_Value = new char[ lLth ];
         strcpy_s( m_pchCurrRB_Value, lLth, *(m_pZCurrRB->m_pzsValueOn) );
         return( (zLONG) m_pchCurrRB_Value );
      }
      else
         return( 0 );
   }
   else
      return( ZMapAct::GetZCtrlState( lStatusType ) );
}

zLONG
ZRadioBtnGroup::SetZCtrlState( zLONG lStatusType,
                               zULONG ulValue )
{
   if ( lStatusType == zCONTROL_STATUS_ENABLED ||
        lStatusType == zCONTROL_STATUS_VISIBLE ||
        lStatusType == zCONTROL_STATUS_AUTOGRAY )
   {
      ZMapAct::SetChildCtrlState( lStatusType, ulValue );
   }

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

zCPCHAR ZRadioBtnGroup::g_cpcClassName = 0;

BOOL
ZRadioBtnGroup::PreCreateWindow( CREATESTRUCT& cs )
{
   // TODO: Modify the Window class or styles here by modifying
   // the CREATESTRUCT cs
// #pragma mMSG( Set up a new class with the correct background here )

   if ( CButton::PreCreateWindow( cs ) == FALSE )
      return( FALSE );

   if ( g_cpcClassName == 0 )
   {
      WNDCLASS wc;

      VERIFY( ::GetClassInfo( AfxGetInstanceHandle( ), cs.lpszClass, &wc ) );

      // Change name and style to have background brush
      wc.lpszClassName = "ZeidonRadioBtnGroup";
      wc.hbrBackground = ::GetSysColorBrush( COLOR_BTNFACE );
      VERIFY( AfxRegisterClass( &wc ) );
      g_cpcClassName = wc.lpszClassName;
   }

   cs.lpszClass = g_cpcClassName;
   return( TRUE );
}

BOOL
ZRadioBtnGroup::OnEraseBkgnd( CDC *pDC )
{
// TraceLineS( "ZRadioBtnGroup::OnEraseBkgnd", "" );
// return( (BOOL) Default( ) );
   return( CButton::OnEraseBkgnd( pDC ) );
}

void
ZRadioBtnGroup::OnNcPaint( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtnGroup::OnNcPaint", "" );
#endif

#if 0
   CRect rect;

   if ( GetUpdateRect( rect, 0 ) )
   {
      TraceRect( "ZRadioBtnGroup::OnNcPaint update rect", rect );
      CWindowDC dc( this );
      GetClientRect( rect );
      TraceRect( "ZRadioBtnGroup::OnNcPaint rect", rect );
      dc.FillRect( &rect, m_pBrush );
   }
#endif

   CButton::OnNcPaint( );
}

void
ZRadioBtnGroup::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZRadioBtnGroup::OnPaint: ", *m_pzsTag );
#endif

   if ( m_ulMapActFlags & zMAPACT_PAINT_IN_PROGRESS )
   {
      CButton::OnPaint( );
      return;
   }

   m_ulMapActFlags |= zMAPACT_PAINT_IN_PROGRESS;

   CRect rect;
   BOOL  bUpdate = GetUpdateRect( rect, 1 );

#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( "CalendarGroup", *m_pzsTag ) == 0 )
   {
      CRect rectWindow;

      TraceLineS( "ZRadioBtnGroup::OnPaint: ", *m_pzsTag );
      TraceRect( "ZRadioBtnGroup::OnPaint UpdateRect", rect );
      GetWindowRect( rectWindow );
      TraceRect( "ZRadioBtnGroup::OnPaint WindowRect", rectWindow );
      GetClientRect( rectWindow );
      TraceRect( "ZRadioBtnGroup::OnPaint ClientRect", rectWindow );
   }
#endif

   if ( bUpdate )
   {
      MSG       msg;
      CClientDC dc( this );

   // TraceLineS( "ZRadioBtnGroup::OnPaint Update: ", *m_pzsTag );
      dc.FillRect( &rect, m_pBrush );
   // ValidateRect( rect );
      CButton::OnPaint( );
   // PumpMessages( m_pZSubtask->m_vDialog ); too much!!!
      zLONG lPeekCnt = 0;
      while ( lPeekCnt < 256 &&
              ::PeekMessage( &msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE ) )
      {
         lPeekCnt++;
         ::TranslateMessage( &msg );
         ::DispatchMessage( &msg );
      }

   // TraceLineI( "RadioBtnGroup PeekCnt: ", lPeekCnt );
      ClientToScreen( rect );
   // if ( zstrcmp( "RadioNcPaint", *m_pzsTag ) == 0 )
   //    TraceRect( "ZRadioBtnGroup::OnPaint ClientToScreenRect", rect );

      if ( lPeekCnt < 256 && m_pZSubtask->m_pZMIXCtrl )
      {
         CRect   rectCtrl;
         CRect   rectTemp;
         ZMapAct *pzma;
      // HWND    hWnd = *(m_pZSubtask->m_pZFWnd);
      // MSG     msg;
         zSHORT  k = 1;

         while ( (pzma = (ZMapAct *)
                             m_pZSubtask->m_pZMIXCtrl->GetAt( k++ )) != 0 )
         {
            if ( pzma->m_pzmaComposite == this )
            {
               if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
               {
               // pzma->m_pCtrl->GetClientRect( rectCtrl );  // client coords
                  pzma->m_pCtrl->GetWindowRect( rectCtrl );  // screen coords
                                                             // (including NC)
               // if ( zstrcmp( "GrpNcPaint", *m_pzsTag ) == 0 )
               // {
               //    TraceRect( *(pzma->m_pzsTag), rectCtrl );
               // }

                  if ( rectTemp.IntersectRect( rectCtrl, rect ) )
                  {
                  //xScreenToClient( &rectCtrl );
                  // rectCtrl.InflateRect( 4, 4 );
                  //xInvalidateRect( rectCtrl, TRUE );

                  // TraceLineS( "ZRadioBtnGroup::OnPaint painting: ",
                  //             *(pzma->m_pzsTag) );
                     pzma->m_pCtrl->Invalidate( );
                     pzma->m_pCtrl->UpdateWindow( );
                  //xValidateRect( rectCtrl );
                  }
               }
            }
         }
      }
   }
   else
   {
   // TraceLineS( "ZRadioBtnGroup::OnPaint: ", *m_pzsTag );
      CButton::OnPaint( );
   }

   m_ulMapActFlags &= ~zMAPACT_PAINT_IN_PROGRESS;
}

// The composite (parent) should return FALSE if it does not want the
// specified child ctrl to receive focus.
zBOOL
ZRadioBtnGroup::IsFocusOKForChild( ZMapAct *pzmaReceivingFocus )
{
   if ( (pzmaReceivingFocus == m_pZCurrRB) ||
        (m_pZCurrRB == 0 && pzmaReceivingFocus == m_pZDfltRB) ||
        (m_pZCurrRB == 0 && m_pZDfltRB == 0) )
   {
      return( TRUE );
   }

   return( FALSE );
}

void
ZRadioBtnGroup::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZRadioBtnGroup - dtor
ZRadioBtnGroup::~ZRadioBtnGroup( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtnGroup::~ZRadioBtnGroup", "" );
#endif

   mDeleteInitA( m_pchCurrRB_Value );
   DestroyWindow( );
}

IMPLEMENT_DYNAMIC( ZRadioBtn, CButton )

BEGIN_MESSAGE_MAP( ZRadioBtn, CButton )
   ON_CONTROL_REFLECT( BN_CLICKED, RBClicked )
   ON_CONTROL_REFLECT( BN_DISABLE, RBDisable )
   ON_CONTROL_REFLECT( BN_DOUBLECLICKED, RBDoubleclicked )
   ON_CONTROL_REFLECT( BN_HILITE, RBHilite )
   ON_CONTROL_REFLECT( BN_PAINT, RBPaint )
   ON_CONTROL_REFLECT( BN_UNHILITE, RBUnhilite )
   ON_WM_PAINT( )
   ON_WM_CREATE( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_MOUSEMOVE( )
   ON_WM_CTLCOLOR_REFLECT( )
// ON_WM_CTLCOLOR( )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ZRadioBtn - ctor
ZRadioBtn::ZRadioBtn( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
           CButton( ),
           ZMapAct( pZSubtask,               // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "RadioButton" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for RadioBtn", (zLONG) this );
#endif

   m_pzsValueOn = 0;
   m_pzsValueOff = 0;

   zPCHAR pch = (zPCHAR) pCtrlDef->CtrlBOI;
   if ( pch == 0 )      // there is no CtrlBOI
      return;

   ZRadioBtnGroup *pRadioBtnGroup =
             DYNAMIC_DOWNCAST( ZRadioBtnGroup, m_pzmaComposite->m_pCtrl );

   if ( pRadioBtnGroup )
   {
      if ( pRadioBtnGroup->m_nFirstChild == -1 )
         pRadioBtnGroup->m_nFirstChild = m_nIdNbr;

      pRadioBtnGroup->m_nLastChild = m_nIdNbr;

      if ( *pch )    // default radio button
      {
//       Attr.Style |= WS_TABSTOP;
         pRadioBtnGroup->m_pZDfltRB = this;
      }
      else
      if ( pRadioBtnGroup->m_nDefault <= 0 )
      {
         Attr.Style &= ~BS_AUTORADIOBUTTON;
         Attr.Style |= BS_RADIOBUTTON;

         if ( pRadioBtnGroup->m_nDefault == 0 )
         {
            Attr.Style |= WS_TABSTOP;
            pRadioBtnGroup->m_nDefault = -1;
         }
      }
   }

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( !(pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style |= WS_BORDER;

   Attr.Style &= ~WS_TABSTOP;

   pch++;
   m_pzsValueOn = new CString( pch );
   m_pzsValueOff = new CString( "" );

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

   m_ulMapActFlags |= zMAPACT_ASKPARENT_FOCUS;

   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
   {
      CreateZ( );
   }
}

void
ZRadioBtn::CreateZ( )
{
   if ( (m_ulMapActFlags & zMAPACT_ENABLED) &&
        (m_pzmaComposite->m_ulMapActFlags & zMAPACT_ENABLED) )
      Attr.Style &= ~WS_DISABLED;
   else
      Attr.Style |= WS_DISABLED;

   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "BUTTON", *m_pzsText,
                   BS_RADIOBUTTON | Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
}

int
ZRadioBtn::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
// TraceLineS( "ZRadioBtn::OnCreate", "" );

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
ZRadioBtn::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CButton::DestroyWindow( ) );
}

// get the data from the OI and use it to set the check for the radio button
// appropriately
zSHORT
ZRadioBtn::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
  TraceLineS( "ZRadioBtn::MapFromOI ", *m_pzsTag );
#endif

   // If the control is not Visible, do not do mapping
   if ( (m_ulMapActFlags & zMAPACT_VISIBLE) == 0 || (m_ulMapActFlag2 & zMAPACT_CREATED) == 0 )
   {
      return( 0 );
   }

   if ( m_pzmaComposite->m_pzsVName && m_pzmaComposite->m_pzsEName &&
        m_pzmaComposite->m_pzsAName )
   {
      zBOOL  bCheck = FALSE;
      zPCHAR pch;
      zULONG ulLth;
      zCHAR  szMap[ zMAP_SIZE ];
      zVIEW  vApp;
      ZRadioBtnGroup *pRadioBtnGroup = DYNAMIC_DOWNCAST( ZRadioBtnGroup, m_pzmaComposite->m_pCtrl );

      zUSHORT uMapState = pRadioBtnGroup->m_uMapState;
      GetViewByName( &vApp, *(m_pzmaComposite->m_pzsVName), m_pZSubtask->m_vDialog, zLEVEL_ANY );
//    zUSHORT uMapState = GetMappingAutoGray( &vApp,
//                                            *(m_pzmaComposite->m_pzsVName),
//                                            *(m_pzmaComposite->m_pzsEName),
//                                            *(m_pzmaComposite->m_pzsAName) );
      if ( vApp && (uMapState & zMAPACT_ENTITY_EXISTS) )
      {
         GetAttributeLength( &ulLth, vApp,
                             *(m_pzmaComposite->m_pzsEName),
                             *(m_pzmaComposite->m_pzsAName) );
         if ( ulLth > zsizeof( szMap ) - 1 )
         {
            GetAddrForAttribute( (zPCHAR *) &pch, vApp, *(m_pzmaComposite->m_pzsEName), *(m_pzmaComposite->m_pzsAName) );
         }
         else
         {
            GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                      zsizeof( szMap ) - 1, vApp,
                                      *(m_pzmaComposite->m_pzsEName),
                                      *(m_pzmaComposite->m_pzsAName),
                                      *(m_pzmaComposite->m_pzsContext), 0 );

//                                    pzmaComposite->m_pzsContext ? 0 :
//                                                   zUSE_DEFAULT_CONTEXT );
            pch = szMap;
         }

         if ( pRadioBtnGroup &&
              (zstrcmp( pch, (*m_pzsValueOn).GetString() ) == 0 ||
               (*pch == 0 && pRadioBtnGroup->m_pZDfltRB == this)) )
         {
            bCheck = TRUE;
            pRadioBtnGroup->m_pZCurrRB = this;
         }
         else
         {
            bCheck = FALSE;
         }
      }

      SetCheck( bCheck );
      return( 0 );
   }

   return( -1 );
}

void
ZRadioBtn::RBClicked( )
{
   zBOOL bChecked = GetCheck( );
   zBOOL bEvent = TRUE;

#ifdef DEBUG_ALL
   TraceLineI( "ZRadioBtn::RBClicked check: ", bChecked );
#endif

   ZRadioBtnGroup *pRadioBtnGroup =
           DYNAMIC_DOWNCAST( ZRadioBtnGroup, m_pzmaComposite->m_pCtrl );

   if ( pRadioBtnGroup )
   {
      if ( bChecked )
      {
         if ( pRadioBtnGroup->m_pZCurrRB == this )
         {
            bEvent = FALSE; // do nothing ... correct button is already checked
         }
         else
         {
            if ( pRadioBtnGroup->m_pZCurrRB )
               pRadioBtnGroup->m_pZCurrRB->SetCheck( 0 );

            pRadioBtnGroup->m_pZCurrRB = this;
         }
      }
      else
      {
         if ( pRadioBtnGroup->m_pZCurrRB )
            pRadioBtnGroup->m_pZCurrRB->SetCheck( 0 );

         pRadioBtnGroup->m_pZCurrRB = this;
         SetCheck( 1 );
      }
   }

   if ( mIs_hWnd( m_hWnd ) )
      SetFocus( );

   if ( bEvent )
      ProcessEvent( this, 1 );
}

void
ZRadioBtn::RBDisable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::RBDisable: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

void
ZRadioBtn::RBDoubleclicked( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::RBDoubleclicked: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

void
ZRadioBtn::RBHilite( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::RBHilite: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

void
ZRadioBtn::RBPaint( )
{
//#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::RBPaint: ", *m_pzsTag );
//#endif
   Default( );             // pass it along to parent
}

void
ZRadioBtn::RBUnhilite( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::RBUnhilite: ", *m_pzsTag );
#endif
   Default( );             // pass it along to parent
}

void
ZRadioBtn::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::OnSetFocus: ", *m_pzsTag );
#endif
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
      RBClicked( );

   Default( );             // pass it along to parent
}

void
ZRadioBtn::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::OnKillFocus: ", *m_pzsTag );
#endif

   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );             // pass it along to parent
}

ZMapAct *
ZRadioBtn::GetRealFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::GetRealFocus: ", *m_pzsTag );
#endif
   if ( m_pzmaComposite )
   {
      ZRadioBtnGroup *pRadioBtnGroup =
           DYNAMIC_DOWNCAST( ZRadioBtnGroup, m_pzmaComposite->m_pCtrl );

      if ( pRadioBtnGroup )
      {
         if ( pRadioBtnGroup->m_pZCurrRB )
            return( pRadioBtnGroup->m_pZCurrRB );
         else
         if ( pRadioBtnGroup->m_pZDfltRB )
            return( pRadioBtnGroup->m_pZDfltRB );
      }
   }

   return( this );
}

void
ZRadioBtn::ResetFocus( )
{
   ZMapAct *pzma = GetRealFocus( );
   if ( pzma && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
      pzma->m_pCtrl->SetFocus( );
   else
   if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
      m_pCtrl->SetFocus( );
}

zLONG
ZRadioBtn::GetZCtrlState( zLONG lStatusType )
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
ZRadioBtn::SetZCtrlState( zLONG lStatusType,
                          zULONG ulValue )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "Mr1" ) == 0 || zstrcmp( *m_pzsTag, "Mr2" ) == 0 )
      TraceLineS( "ZRadioBtn::SetZCtrlState ", *m_pzsTag );
#endif

   if ( lStatusType == zCONTROL_STATUS_CHECKED )
   {
      if ( ulValue )
         RBClicked( );
      else
      {
         ZRadioBtnGroup *pRadioBtnGroup =
           DYNAMIC_DOWNCAST( ZRadioBtnGroup, m_pzmaComposite->m_pCtrl );

         SetCheck( 0 );
         if ( pRadioBtnGroup )
         {
            if ( pRadioBtnGroup->m_pZCurrRB == this )
            {
               pRadioBtnGroup->m_pZCurrRB = 0;
               if ( pRadioBtnGroup->m_pZDfltRB )
               {
                  pRadioBtnGroup->m_pZDfltRB->SetCheck( 1 );
                  pRadioBtnGroup->m_pZCurrRB = pRadioBtnGroup->m_pZDfltRB;
               }
            }
         }
      }

      return( 0 );
   }
   else
      return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

zBOOL
ZRadioBtn::HandleQuickKey( zACCEL_DEF ad )
{
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

   if ( ad.uVK == VK_UP || ad.uVK == VK_DOWN ||
        ad.uVK == VK_LEFT || ad.uVK == VK_RIGHT )
   {
      if ( m_pzmaComposite )
      {
         ZMapAct *pzma = this;
         zULONG  ulIdx;
         zSHORT  nIncr;

         nIncr = (ad.uVK == VK_UP || ad.uVK == VK_LEFT) ? -1 : 1;
         ulIdx = m_pZSubtask->m_pZMIXCtrl->GetNodeIndex( pzma );
         ulIdx += nIncr;
         pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( ulIdx );
         while ( pzma && pzma->m_pzmaComposite == m_pzmaComposite )
         {
            if ( (pzma->m_ulMapActFlags & zMAPACT_VISIBLE) &&
                 (pzma->m_ulMapActFlags & zMAPACT_ENABLED) &&
                 (pzma->m_ulMapActFlags & zMAPACT_FOCUS_ENABLED) &&
                 pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
            {
               pzma->m_pCtrl->SetFocus( );
               break;
            }

            ulIdx += nIncr;
            pzma = (ZMapAct *) m_pZSubtask->m_pZMIXCtrl->GetAt( ulIdx );
         }
      }

      return( TRUE );
   }

   return( FALSE );
}

zLONG
ZRadioBtn::HandleEvent( zLONG lEvent )
{
   RBClicked( );
   return( 0 );
}

HBRUSH
ZRadioBtn::CtlColor( CDC *pDC, UINT nCtlColor )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZRadioBtn::CtlColor: ", *m_pzsTag );
#endif
   HBRUSH hBrush = (HBRUSH) Default( );

// if ( m_clrText )
//    pDC->SetTextColor( m_clrText );

   if ( m_pBrush )
   {
      hBrush = (HBRUSH) *m_pBrush;
      pDC->SetBkColor( m_clrBk );
   }

   return( hBrush );
}

void
ZRadioBtn::OnPaint( )
{
// if ( zstrcmp( "Carlson", *m_pzsTag ) == 0 )
//    TraceLineS( "", "" );

#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZRadioBtn::OnPaint: ", *m_pzsTag );
   CRect  rect;

   BOOL bUpdate = GetUpdateRect( rect, 1 );
   TraceRect( "ZRadioBtn::OnPaint rect", rect );
#endif

#if 0
   if ( m_pzmaComposite && m_pzmaComposite->m_pCtrl &&
        (m_pWndParent == 0 ||
         m_pWndParent->m_hWnd != m_pzmaComposite->m_pCtrl->m_hWnd) &&
        mIs_hWnd( m_pzmaComposite->m_pCtrl->m_hWnd ) )
   {
   // TraceLineS( "ZRadioBtn::OnPaint forced composite update - ",
   //             *m_pzsTag );
      m_pzmaComposite->m_pCtrl->UpdateWindow( );
   }
#endif
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtnGroup::OnPaint: ", *m_pzsTag );
#endif

   if ( m_ulMapActFlags & zMAPACT_PAINT_IN_PROGRESS )
   {
      CButton::OnPaint( );
      return;
   }

   m_ulMapActFlags |= zMAPACT_PAINT_IN_PROGRESS;

#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( "Carlson", *m_pzsTag ) == 0 )
   {
      CRect rectWindow;

      TraceLineS( "ZRadioBtnGroup::OnPaint: ", *m_pzsTag );
      TraceRect( "ZRadioBtnGroup::OnPaint UpdateRect", rect );
      GetWindowRect( rectWindow );
      TraceRect( "ZRadioBtnGroup::OnPaint WindowRect", rectWindow );
      GetClientRect( rectWindow );
      TraceRect( "ZRadioBtnGroup::OnPaint ClientRect", rectWindow );
   }
#endif

   CRect rect;
   BOOL  bUpdate = GetUpdateRect( rect, 1 );
   if ( bUpdate )
   {
      CClientDC dc( this );

   // TraceLineS( "ZRadioBtnGroup::OnPaint Update: ", *m_pzsTag );
      dc.FillRect( &rect, m_pBrush );
      CButton::OnPaint( );
      if ( **m_pzsText && (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) &&
                          (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      {
         int   nIndexDC = dc.SaveDC( );
         DWORD dwStyle = 0;

      // if ( zstrcmp( "Message2", *m_pzsTag ) == 0 )
      //    TraceLineS( "ZRadioBtnGroup::OnPaint READONLY: ", *m_pzsTag );

         dc.SetBkMode( TRANSPARENT );
         dc.SelectObject( *m_pFont );

      // if ( m_bLeftText )
      // {
      //    dwStyle = DT_RIGHT;
      // }
      // else
         {
            rect.left += 18;
            dwStyle = DT_LEFT;
         }

         rect.bottom -= 1;  // rect = drawing rectangle inside DC

         dc.DrawText( *m_pzsText, -1, &rect, DT_SINGLELINE | DT_VCENTER | dwStyle );

         dc.RestoreDC( nIndexDC );
      }
   }
   else
   {
      CButton::OnPaint( );
   }

   m_ulMapActFlags &= ~zMAPACT_PAINT_IN_PROGRESS;
}

void
ZRadioBtn::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZRadioBtn - dtor
ZRadioBtn::~ZRadioBtn( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::~ZRadioBtn", "" );
#endif

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );

   mDeleteInit( m_pzsValueOn );
   mDeleteInit( m_pzsValueOff );
}

void
ZRadioBtn::SetCheck( zBOOL bCheck )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::SetCheck: ", *m_pzsTag );
#endif
#ifdef zREMOTE_SERVER
   SetRemoteZCtrlAttribute( this, m_csERemote, "Checked",
                            bCheck ? "Y" : "N" );
#else
   if ( mIs_hWnd( m_hWnd ) )
       CButton::SetCheck( bCheck );
#endif
}

zBOOL
ZRadioBtn::GetCheck( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZRadioBtn::GetCheck: ", *m_pzsTag );
#endif
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
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
RadioBtnGroup( ZSubtask *pZSubtask,
               CWnd     *pWndParent,
               ZMapAct  *pzmaComposite,
               zVIEW    vDialog,
               zSHORT   nOffsetX,
               zSHORT   nOffsetY,
               zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZRadioBtnGroup( pZSubtask, pWndParent,
                               pzmaComposite, vDialog,
                               nOffsetX, nOffsetY, pCtrlDef ) );
}

AFX_EXT_API
CWnd * OPERATION
RadioBtn( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZRadioBtn( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}
