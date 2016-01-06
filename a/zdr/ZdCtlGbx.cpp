/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlgbx.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon GroupBox control.
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
// 2001.07.27    DKS    Z10
//    Fix to painting of GroupBox with large numbers of EditBoxes.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2000.03.17    TMV    WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 2000.03.02    DKS    Z10    ActiveX
//    ActiveX controls now show through a GroupBox.  This change is quite
//    pervasive and will need to be tested quite thoroughly to determine
//    if it is a valid fix.
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
#include "ZdCtlGbx.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

IMPLEMENT_DYNAMIC( ZGroupBox, CButton )

BEGIN_MESSAGE_MAP( ZGroupBox, CButton )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
   ON_WM_CREATE( )
   ON_WM_ERASEBKGND( )
// ON_WM_CTLCOLOR_REFLECT( )
   ON_WM_ENABLE( )
   ON_CONTROL_REFLECT( BN_CLICKED, GNClicked )
   ON_CONTROL_REFLECT( BN_DISABLE, GNDisable )
   ON_CONTROL_REFLECT( BN_DOUBLECLICKED, GNDoubleclicked )
   ON_CONTROL_REFLECT( BN_HILITE, GNHilite )
   ON_CONTROL_REFLECT( BN_PAINT, GNPaint )
   ON_CONTROL_REFLECT( BN_UNHILITE, GNUnhilite )
   ON_WM_MOUSEMOVE( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
END_MESSAGE_MAP( )


// ZGroupBox - ctor
ZGroupBox::ZGroupBox( ZSubtask *pZSubtask,
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
                    "GroupBox" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for ZGroupBox", (zLONG) this );
#endif

   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( !(pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style |= WS_BORDER;

// if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;

   // Before DKS  ActiveX change 2000/03/02
//xAttr.Style &= ~WS_TABSTOP;
   Attr.Style |= WS_TABSTOP;

// mDeleteInit( m_pBrush );
   if ( m_pBrush == 0 )
      m_pBrush = new CBrush( ::GetSysColor( COLOR_BTNFACE ) );

// Attr.Style |= WS_CLIPSIBLINGS;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }

   zVIEW vSubobject;

   CreateViewFromViewForTask( &vSubobject, vDialog, m_pZSubtask->m_vDialog );
   SetViewToSubobject( vSubobject, szlCtrlCtrl );
   CreateZeidonCtrls( vSubobject, pZSubtask,
                      this, this, 0, 0, TRUE );
//                    pWndParent, pzmaComposite,
//x  before  DKS      pWndParent, this,
//x  ActiveX 2000/    (zSHORT) pCtrlDef->PSDLG_X,
//x  change  03/02    (zSHORT) pCtrlDef->PSDLG_Y, TRUE );
   DropView( vSubobject );
}

void
ZGroupBox::CreateZ( )
{
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( 0 /*WS_EX_CLIENTEDGE*/, "BUTTON", *m_pzsText,
                   BS_GROUPBOX | Attr.Style,
                   // | WS_CLIPCHILDREN causes non-client paint problems
                   Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
      if ( mIs_hWnd( m_hWnd ) )
         Invalidate( TRUE );
   }
#endif
}

#if 0
HBRUSH
ZGroupBox::CtlColor( CDC *pDC, UINT nCtlColor )
{
// TraceLineS( "ZGroupBox::CtlColor", "" );
   return( (HBRUSH) Default( ) );
}
#endif

LPCSTR ZGroupBox::g_cpcClassName = 0;

BOOL
ZGroupBox::PreCreateWindow( CREATESTRUCT& cs )
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
      wc.lpszClassName = "ZeidonGroupBox";
      wc.hbrBackground = ::GetSysColorBrush( COLOR_BTNFACE );
      VERIFY( AfxRegisterClass( &wc ) );
      g_cpcClassName = wc.lpszClassName;
   }

   cs.lpszClass = g_cpcClassName;
   return( TRUE );
}

int
ZGroupBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
// TraceLineS( "ZGroupBox::OnCreate: ", *m_pzsTag );
   int nRC = CButton::OnCreate( lpCreateStruct );

   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );
   return( nRC );
}

BOOL
ZGroupBox::DestroyWindow( )
{
// TraceLineS( "ZGroupBox::DestroyWindow: ", *m_pzsTag );
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CButton::DestroyWindow( ) );
}

zSHORT
ZGroupBox::MapFromOI( WPARAM wFlag )
{
// TraceLineS( "ZGroupBox::MapFromOI ", *m_pzsTag );

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   return( 0 );
}

zLONG
ZGroupBox::SetZCtrlState( zLONG lStatusType,
                          zULONG ulValue )
{
   if ( lStatusType == zCONTROL_STATUS_ENABLED ||
        lStatusType == zCONTROL_STATUS_VISIBLE )
   {
      ZMapAct::SetChildCtrlState( lStatusType, ulValue );
   }

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

void
ZGroupBox::GNClicked( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGroupBox::GNClicked", "" );
#endif
   Default( );             // pass it along to parent
}

void
ZGroupBox::GNDisable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGroupBox::GNDisable", "" );
#endif
   Default( );             // pass it along to parent
}

void
ZGroupBox::GNDoubleclicked( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGroupBox::GNDoubleclicked", "" );
#endif
   Default( );             // pass it along to parent
}

void
ZGroupBox::GNHilite( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGroupBox::GNHilite", "" );
#endif
   Default( );             // pass it along to parent
}

void
ZGroupBox::GNPaint( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGroupBox::GNPaint", "" );
#endif
   Default( );             // pass it along to parent
}

void
ZGroupBox::GNUnhilite( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGroupBox::GNUnhilite", "" );
#endif
   Default( );             // pass it along to parent
}

void
ZGroupBox::OnEnable( BOOL bEnable )
{
   if ( bEnable == FALSE )
      EnableWindow( );  // never disable a groupbox ... 2005.03.31
}

BOOL
ZGroupBox::OnEraseBkgnd( CDC *pDC )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "GrpNcPaint", *m_pzsTag ) == 0 )
   {
      TraceLineS( "ZGroupBox::OnEraseBkgnd: ", *m_pzsTag );
   }
#endif

   return( (BOOL) Default( ) );
}

void
ZGroupBox::OnNcPaint( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "GrpNcPaint", *m_pzsTag ) == 0 )
   {
      TraceLineS( "ZGroupBox::OnNcPaint: ", *m_pzsTag );
   }
#endif

#if 0
   CRect rect;

   CWindowDC dc( this );
   GetClientRect( rect );
   TraceRect( "ZGroupBox::OnNcPaint rect", rect );
   dc.FillRect( &rect, m_pBrush );
#endif

// TraceLineS( "ZGroupBox::OnNcPaint: ", *m_pzsTag );
// if ( m_pzmaComposite && m_pzmaComposite->m_pCtrl &&
//      mIs_hWnd( m_pzmaComposite->m_pCtrl->m_hWnd ) )
// {
//    m_pzmaComposite->m_pCtrl->UpdateWindow( );
// }

   CButton::OnNcPaint( );
}

void
ZGroupBox::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZGroupBox::OnPaint: ", *m_pzsTag );
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

      TraceLineS( "ZGroupBox::OnPaint: ", *m_pzsTag );
      TraceRect( "ZGroupBox::OnPaint UpdateRect", rect );
      GetWindowRect( rectWindow );
      TraceRect( "ZGroupBox::OnPaint WindowRect", rectWindow );
      GetClientRect( rectWindow );
      TraceRect( "ZGroupBox::OnPaint ClientRect", rectWindow );
   }
#endif

   if ( bUpdate )
   {
      MSG       msg;
      CClientDC dc( this );

   // TraceLineS( "ZGroupBox::OnPaint Update: ", *m_pzsTag );
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

   // TraceLineI( "GroupBox PeekCnt: ", lPeekCnt );
      ClientToScreen( rect );
   // if ( zstrcmp( "GrpNcPaint", *m_pzsTag ) == 0 )
   //    TraceRect( "ZGroupBox::OnPaint ClientToScreenRect", rect );

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

                  // TraceLineS( "ZGroupBox::OnPaint painting: ",
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
   // TraceLineS( "ZGroupBox::OnPaint: ", *m_pzsTag );
      CButton::OnPaint( );
   }

   m_ulMapActFlags &= ~zMAPACT_PAINT_IN_PROGRESS;
}

void
ZGroupBox::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
// TracePoint( "ZGroupBox::OnMouseMove", pt );
   CWnd    *pWnd = ChildWindowFromPoint( pt, CWP_SKIPINVISIBLE );
   CWnd    *pWndChild;
   ZMapAct *pzma = 0;

   if ( pWnd )
   {
      zSHORT  k = 1;

      while ( (pzma = (ZMapAct *)
                          m_pZSubtask->m_pZMIXCtrl->GetAt( k++ )) != 0 )
      {
         if ( pzma->m_pCtrl && pzma->m_pCtrl->m_hWnd == pWnd->m_hWnd )
         {
            pWndChild = pWnd->ChildWindowFromPoint( pt, CWP_SKIPINVISIBLE );
            if ( pWndChild && pWndChild != pWnd )
            {
               pWnd = pWndChild;
               k = 1;
            }
            else
               break;
         }
      }
   }

   if ( pzma == 0 )
   {
      m_pZSubtask->InformDriverOfFlyBy( this );
   }
   else
   {
   // TraceLineS( "ZGroupBox::OnMouseMove pzma = ", *(pzma->m_pzsTag) );
      m_pZSubtask->InformDriverOfFlyBy( pzma );
   }

   Default( );              // pass it along to parent
}

// DKS  ActiveX change 2000/03/02

zBOOL
ZGroupBox::InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus )
{
// TraceLineS( "ZGroupBox InformCompositeOfSetFocus pzma = ",
//             *(pzmaReceivingFocus->m_pzsTag) );
   return( TRUE );
}

zBOOL
ZGroupBox::InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus )
{
// TraceLineS( "ZGroupBox InformCompositeOfKillFocus pzma = ",
//             *(pzmaLosingFocus->m_pzsTag) );

   // Look for the next/prev sub-control that can take focus.  If there is
   // one, set focus to it, otherwise look for a control at the same level
   // as this composite and set focus to it.
   //
   zBOOL bTab = (GetKeyState( VK_TAB ) < 0);
   if ( bTab == FALSE )
      return( TRUE );

   zBOOL bShift = (GetKeyState( VK_SHIFT ) < 0);
   ZMapAct *pzmaFocus = m_pZSubtask->
            GetFocusCtrlWithinComposite( this, pzmaLosingFocus, !bShift );
   if ( pzmaFocus )
   {
      if ( mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }
   else
   {
      pzmaFocus = m_pZSubtask->
                          GetFocusCtrlWithinComposite( 0, this, !bShift );
      if ( pzmaFocus && mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }

   return( TRUE );
}

void
ZGroupBox::OnSetFocus( CWnd *pWndLostFocus )
{
// TraceLineS( "ZGroupBox OnSetFocus", "" );
   m_pZSubtask->InformDriverOfSetFocus( this, FALSE );
// UpdatePalette( TRUE );
   Default( );              // pass it along to parent

   // Look for a sub-control that can take focus.  If there is one, set
   // focus to it, otherwise look for a control at the same level as this
   // composite and set focus to it.
   zBOOL bShift = (GetKeyState( VK_SHIFT ) < 0);
   ZMapAct *pzma = 0;

   if ( bShift && pWndLostFocus )
   {
      zSHORT k = 1;

      while ( (pzma = (ZMapAct *)
                               m_pZSubtask->m_pZMIXCtrl->GetAt( k )) != 0 )
      {
         if ( pzma && pzma->m_pCtrl &&
              pzma->m_pCtrl->m_hWnd == pWndLostFocus->m_hWnd )
         {
            if ( pzma->m_pzmaComposite != this )
               pzma = 0;

            break;
         }

         k++;
      }
   }

   ZMapAct *pzmaFocus = m_pZSubtask->
                          GetFocusCtrlWithinComposite( this, pzma, !bShift );
   if ( pzmaFocus )
   {
      if ( mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }
   else
   {
      pzmaFocus = m_pZSubtask->
                          GetFocusCtrlWithinComposite( 0, this, !bShift );
      if ( pzmaFocus && mIs_hWnd( pzmaFocus->m_pCtrl->m_hWnd ) )
         pzmaFocus->m_pCtrl->SetFocus( );
   }
}

void
ZGroupBox::OnKillFocus( CWnd *pWndGetFocus )
{
// TraceLineS( "ZGroupBox OnKillFocus", "" );
   m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );              // pass it along to parent
}

// ZGroupBox - dtor
ZGroupBox::~ZGroupBox( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZGroupBox::~ZGroupBox", "" );
#endif
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
OutlookBar( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

AFX_EXT_API
CWnd * OPERATION
GroupBox( ZSubtask *pZSubtask,
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
      if ( pCtrlDef->Tag[ 2 ] == 'O' &&
           pCtrlDef->Tag[ 3 ] == 'L' &&
           pCtrlDef->Tag[ 4 ] == 'B' )
      {
         return( OutlookBar( pZSubtask, pWndParent, pzmaComposite,
                             vDialog, nOffsetX, nOffsetY, pCtrlDef ) );
      }
   }

   return( new ZGroupBox( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}
