/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlpte.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Painter ctrl cover window
// (permits common move, size and selection processing for all ctrl types).

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.06.26    DKS
//    Initial phase of trying to fix TabStop settings in Win2000 (not done).
//
// 1999.03.10    DKS
//    Altered the Profile functions to remove name conflict.
//
// 1999.02.05    DKS
//    Added class to support the implementation of Abut ctrls.
//
// 1999.02.01    DKS
//    Create of ctrl when scrolled appears in correct position.
//
// 1999.01.27    DKS
//    Added class to support the implementation of Undo/Redo.
//
// 1998.10.30    DKS
//    Improved selection algorithim.
//
// 1998.08.10    DKS
//    Fixed a positioning problem when moving a ctrl from one parent to
//    another.
//

#include "zstdafx.h"

#define TZCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "TzCtl.h"
#include "TzCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL
// #define zDEBUG_CTRL

IMPLEMENT_DYNAMIC( TZCtrlCover, CWnd )

BEGIN_MESSAGE_MAP( TZCtrlCover, CWnd )
   ON_WM_CREATE( )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
   ON_WM_WINDOWPOSCHANGED( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_MOUSEMOVE( )
   ON_WM_KEYDOWN( )
END_MESSAGE_MAP( )

zSHORT TZCtrlCover::m_nFrameSizeX = ::GetSystemMetrics( SM_CXSIZEFRAME );
zSHORT TZCtrlCover::m_nFrameSizeY = ::GetSystemMetrics( SM_CYSIZEFRAME );
zSHORT TZCtrlCover::m_nMinWindowX = ::GetSystemMetrics( SM_CXMIN );
zSHORT TZCtrlCover::m_nMinWindowY = ::GetSystemMetrics( SM_CYMIN );
zSHORT TZCtrlCover::m_nDragX = ::GetSystemMetrics( SM_CXDRAG );
zSHORT TZCtrlCover::m_nDragY = ::GetSystemMetrics( SM_CYDRAG );

#ifdef zDEBUG_CTRL

void
TraceCtrl( zCPCHAR pchText,
           TZPainterCtrl *pCtrl )
{
   zCHAR szMessage[ 512 ];
   short nLth = 0;

   strcpy_s( szMessage, zsizeof( szMessage ), pchText );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " Tag:" );
   nLth += 5;
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, pCtrl->m_csTag );
   if ( pCtrl->m_pCtrlParent )
   {
      strcat_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " Parent:" );
      strcat_s( szMessage + nLth, zsizeof( szMessage ) - nLth, pCtrl->m_pCtrlParent->m_csTag );
   }

   strcat_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " Type:" );
   nLth = (zSHORT) zstrlen( szMessage );
   _ltoa_s( pCtrl->m_lType, szMessage + nLth, zsizeof( szMessage ) - nLth, 10 );

   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " X:" );
   _ltoa_s( pCtrl->m_rectPainter.left, szMessage + nLth + 4, zsizeof( szMessage ) - nLth - 4, 10 );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " Y:" );
   _ltoa_s( pCtrl->m_rectPainter.top, szMessage + nLth + 3, zsizeof( szMessage ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " W:" );
   _ltoa_s( pCtrl->m_rectPainter.right - pCtrl->m_rectPainter.left, szMessage + nLth + 3, zsizeof( szMessage ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " H:" );
   _ltoa_s( pCtrl->m_rectPainter.bottom - pCtrl->m_rectPainter.top, szMessage + nLth + 3, zsizeof( szMessage ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " Ctrl X:" );
   _ltoa_s( pCtrl->m_rectCtrl.left, szMessage + nLth + 9, zsizeof( szMessage ) - nLth - 9, 10 );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " Y:" );
   _ltoa_s( pCtrl->m_rectCtrl.top, szMessage + nLth + 3, zsizeof( szMessage ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " W:" );
   _ltoa_s( pCtrl->m_rectCtrl.right - pCtrl->m_rectCtrl.left, szMessage + nLth + 3, zsizeof( szMessage ) - nLth - 3, 10 );
   nLth = (zSHORT) zstrlen( szMessage );
   strcpy_s( szMessage + nLth, zsizeof( szMessage ) - nLth, " H:" );
// _ltoa_s( pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top,
//        szMessage + nLth + 3 );
   TraceLineI( szMessage, pCtrl->m_rectCtrl.bottom - pCtrl->m_rectCtrl.top );
}

#else

#define TraceCtrl

#endif

TZCtrlCover::TZCtrlCover( TZPainterWindow *pPainterWindow,
                          TZPainterCtrl   *pCtrl,
                          zCPCHAR         cpcTag )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( pCtrl->m_csTag, "Tree2" ) == 0 )
      TraceLineS( "TZCtrlCover::ctor: ", pCtrl->m_csTag );
#endif

   ASSERT( pCtrl );

   m_pCtrl = pCtrl;
   m_pCtrl->m_pCtrlCover = this;
   m_pPainterWindow = pPainterWindow;
   m_pSelectCover = 0;
   m_bMoving = FALSE;

   // Keep this as low as possible without getting into code that will
   // cause messages to the CtrlCover!!!
   m_pCtrl->SetupCtrlCover( );  // first chance the control has to know
                                // its corresponding pCtrlCover

   Attr.X = m_pCtrl->m_rectPainter.left;
   Attr.Y = m_pCtrl->m_rectPainter.top;
   Attr.W = m_pCtrl->m_rectPainter.right - m_pCtrl->m_rectPainter.left;
   Attr.H = m_pCtrl->m_rectPainter.bottom - m_pCtrl->m_rectPainter.top;
   Attr.Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;

   CString cs = "~";  cs += cpcTag;

   TZPainterCtrl *pCtrlParent = pCtrl;
   CWnd          *pWndParent;

   while ( pCtrlParent->IsModeValid( zCTRL_MODE_CLIENT ) == 0 &&
           pCtrlParent->m_pCtrlParent )
   {
      pCtrlParent = pCtrlParent->m_pCtrlParent;
   }

   if ( pCtrlParent->IsModeValid( zCTRL_MODE_CLIENT ) )
      pWndParent = pCtrlParent->m_pWndCtrl;
   else
      pWndParent = pPainterWindow->m_pPainterClient;

   CreateEx( WS_EX_TOPMOST | WS_EX_TRANSPARENT, 0, cs,
             WS_CHILD | WS_VISIBLE,
             m_pCtrl->m_rectPainter, pWndParent, -1 );

   // Place our CtrlCover window on top of the control in the ZOrder.
   SetWindowPos( &wndTop, 0, 0, 0, 0,
                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW );
}

TZCtrlCover::~TZCtrlCover( )
{
#ifdef DEBUG_ALL
   if ( m_pCtrl )
      TraceLineS( "TZCtrlCover::dtor ", m_pCtrl->m_csTag );
   else
      TraceLineS( "TZCtrlCover::dtor", "" );
#endif

   mDeleteInit( m_pSelectCover );
   m_pPainterWindow = 0;
   DestroyWindow( );
}

int
TZCtrlCover::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCtrlCover::OnCreate: ", m_pCtrl->m_csTag );
#endif

   if ( (int) Default( ) == -1 )
      return( -1 );

   ModifyStyleEx( 0, WS_EX_TOPMOST | WS_EX_TRANSPARENT );
   return( 0 );
}

BOOL
TZCtrlCover::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCtrlCover::DestroyWindow: ", m_pCtrl->m_csTag );
#endif

   m_pCtrl = 0;

   return( CWnd::DestroyWindow( ) );
}

//
//  Determine if the current click is part of a double click.
//  Return: TRUE  - current click is part of a double-click
//          FALSE - current click is not part of a double-click ...
//                  OR the specified window was not located
zBOOL
TZCtrlCover::IsDoubleClick( zUSHORT uMessage, CPoint ptStart )
{
   MSG     msg;
   zULONG  ulInterval = 0;
   zLONG   lMessage = GetMessageTime( );
   zULONG  ulDblclkTime = GetDoubleClickTime( );

#ifdef DEBUG_ALL
   TraceLineI( "TZCtrlCover::IsDoubleClick Time: ", ulDblclkTime );
   TraceLineI( "                    MessageTime: ", lMessage );
#endif

   while ( ulInterval < ulDblclkTime )
   {
      ulInterval = (zULONG) (GetCurrentTime( ) - lMessage);
      if ( ::PeekMessage( &msg, m_hWnd, uMessage, uMessage,
                          PM_NOREMOVE | PM_NOYIELD ) )
      {
      // TraceLineI( "TZCtrlCover::IsDoubleClick TRUE: ", ulInterval );
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
         // TracePoint( "TZCtrlCover::IsDoubleClick FALSE: ", pt );
         // ::TranslateMessage( &msg );
         // ::DispatchMessage( &msg );
            return( FALSE );
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

//
// There are several states that need to be addressed during button down processing:
//  - If a ctrl type has been selected for painting, let the PainterClient handle the message.
//  - If the Shift Key is pressed, the user is drawing a selection rect ... all selected ctrls are cloned.
//  - Otherwise, if this ctrl is selected and mouse movement is detected,
//    it can be moved or sized (or cloned if the Ctrl Key is pressed).
//  - Otherwise, if the Ctrl Key is pressed, this ctrl's selection state is toggled.
//  - Otherwise, all other ctrl's are deselected and this ctrl is selected.
//
void
TZCtrlCover::OnLButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCtrlCover::OnLButtonDown: ", m_pCtrl->m_csTag );
   TracePoint( "======================= pt:", pt );
#endif

   if ( (m_pPainterWindow->m_fWndState & zTZPW_UPDATEABLE) == 0 ||
        mIs_hWnd( m_hWnd ) == FALSE )
   {
      return;
   }

// if ( m_pCtrl == 0 || m_pCtrl->m_pWndCtrl != this )
// {
//    MessageSend( m_pPainterWindow->m_pBar->m_vSubtask, 0,
//                 "Painter Error", "Unexpected Cover",
//                 zMSGQ_MODAL_ERROR, FALSE );
//    return;  // we don't know how this happened
// }

   TZPainterWindow *pPainterWindow = m_pPainterWindow;
   TZPainterCtrl *pCtrl;
   TZPainterCtrl *pCtrlLast;
   TZPainterCtrl *pCtrlParent;
   CRect rect;
   zBOOL bClient;

   // Adjust the point into PainterClient coords.
   rect.left = pt.x;
   rect.top = pt.y;

   pCtrl = m_pCtrl;
   while ( pCtrl )
   {
      pCtrlLast = pCtrl;
      pCtrl = pCtrl->AdjustCtrlRect( rect, TRUE );
   }

   if ( pCtrlLast->IsModeValid( zCTRL_MODE_CLIENT ) )
      bClient = TRUE;
   else
      bClient = FALSE;

   // Hold on to the real mouse position with respect to PainterClient coords.
   m_pCtrl->m_ptMouseSelect.x = rect.left;
   m_pCtrl->m_ptMouseSelect.y = rect.top;
   m_pCtrl->m_ptLastMouseMove = m_pCtrl->m_ptMouseSelect;

#if 0 // this does not work for report text ctrls when scrolling or
      // even for painting text ctrls in a group (not limited to text
      // ctrls) ... At this point I am not sure when this code should
      // be in effect.

   // Adjust for scroll position.
   CPoint ptScrollPos =
              pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
   pt.x = rect.left - ptScrollPos.x;
   pt.y = rect.top - ptScrollPos.y;
#else
   pt = m_pCtrl->m_ptMouseSelect;
#endif

   // Determine if the Shift or Ctrl Keys are pressed.
   m_pCtrl->m_bShiftKey = (uModKeys & MK_SHIFT);
   m_pCtrl->m_bCtrlKey = (uModKeys & MK_CONTROL);

   // If a ctrl type is selected for painting, convert the point
   // to the appropriate offset and send the message along to the
   // PainterClient, then get out of here.
   if ( pPainterWindow->m_pBar->m_uSelectedPaletteItem )
   {
//    TracePoint( "TZCtrlCover::OnLButtonDown before adjust", pt );
      // Adjust for scroll position.
      CPoint ptScrollPos =
              pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
      m_pCtrl->m_ptMouseSelect.x -= ptScrollPos.x;
      m_pCtrl->m_ptMouseSelect.y -= ptScrollPos.y;
      m_pCtrl->m_ptLastMouseMove.x -= ptScrollPos.x;
      m_pCtrl->m_ptLastMouseMove.y -= ptScrollPos.y;
      pt = m_pCtrl->m_ptMouseSelect;
//    TracePoint( "TZCtrlCover::OnLButtonDown after adjust", pt );
//    pPainterWindow->m_fWndState |= zTZPW_SCROLLADJUSTED;

//    TraceLineS( "TZCtrlCover::LButtonDown", "SelectedPaletteItem" );
      pPainterWindow->m_pPaintParent = m_pCtrl;
      pPainterWindow->m_pPainterClient->OnLButtonDown( uModKeys, pt );
//    pPainterWindow->m_fWndState &= ~zTZPW_SCROLLADJUSTED;
      return;     // out of here!!!
   }

   // If the Shift Key is pressed, the user is trying to draw a selection
   // rectangle.
   if ( m_pCtrl->m_bShiftKey )
   {
//    TraceLineS( "TZCtrlCover::LButtonDown", "Drawing selection rect" );
      pPainterWindow->m_pPainterClient->OnLButtonDown( uModKeys,
                                                m_pCtrl->m_ptLastMouseMove );
      return;     // out of here!!!
   }

// if ( IsDoubleClick( WM_LBUTTONDBLCLK, pt ) )
// {
//    TraceLineS( "TZCtrlCover::OnLButtonDown", "DBLCLK" );
//    return;     // out of here!!!
// }

   zLONG lSelectPos = -1;
   zLONG lSelectOne = 64000;  // we are going to select the one whose top
                              // is closest to the selection point.
   zSHORT nPos;

// TracePoint( "Locating ptMouseSelect =======>", m_pCtrl->m_ptMouseSelect );
// TracePoint( "Locating best CtrlCover ======>", pt );

   for ( nPos = 0; nPos < pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->GetVisibility( ) )    // not visible or not selectable
         continue;

      if ( bClient )
      {
         pCtrlParent = pCtrl;
         while ( pCtrlParent )
         {
            if ( pCtrlParent == pCtrlLast )
               break;

            pCtrlParent = pCtrlParent->m_pCtrlParent;
         }

         if ( pCtrlParent == 0 )
            continue;       // NOT within "Client"
      }

      pCtrl->ParentToPainterRect( rect );
   // TraceRect( pCtrl->m_csTag, rect );
      if ( (pt.x >= rect.left && pt.x <= rect.right &&
            pt.y >= rect.top && pt.y <= rect.bottom) )
      {
         if ( pt.y - rect.top <= lSelectOne )
         {
            lSelectOne = pt.y - rect.top;
         // TraceLineI( "Current Selection value ===> ", lSelectOne );
            lSelectPos = nPos;
         }
      }
   }

// TraceLineI( "Final Selection ======> ", lSelectOne );
   if ( lSelectOne >= 0 && lSelectPos >= 0 )
      pCtrl = pPainterWindow->m_CtrlList[ lSelectPos ];
   else
      pCtrl = m_pCtrl;

// TraceLineS( "Selected ctrl: ", pCtrl->m_csTag );
   if ( m_pCtrl->m_bCtrlKey )
   {
      // If the Ctrl Key is pressed, toggle this ctrl's selection state.
//    TraceLineS( "TZCtrlCover::LButtonDown", "Toggling selection state" );

      // Find the childmost ctrl without regard to its select state.
//x   pCtrl = FindChildmostUnselected( m_pCtrl, pt, TRUE );
//x   if ( pCtrl == 0 )
//x      pCtrl = m_pCtrl;

      if ( pCtrl->IsSelected( ) )
         pCtrl->Deselect( );
      else
         pCtrl->Select( );

      pPainterWindow->SetDIL_Text( );
   }
   else
   {
      // Find the childmost ctrl without regard to its select state.
//x   pCtrl = FindChildmostUnselected( m_pCtrl, pt, TRUE );
//x   if ( pCtrl == 0 )
//x      pCtrl = m_pCtrl;
//x
//x   // If this ctrl is not visible or not selectable, attempt to find
//x   // a parent that is.
//x   while ( pCtrl && pCtrl->GetVisibility( ) )
//x      pCtrl = pCtrl->m_pCtrlParent;

      if ( pCtrl )
      {
         TZPainterCtrl *pCtrlSelect;

         // Select this ctrl and clear selection for all others.
         pCtrl->m_bCtrlCoverSelected = TRUE;
         pPainterWindow->Select( pCtrl );
         pCtrlSelect = pCtrl;
      // m_pCtrl = pCtrl;
         if ( pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
         {
            pCtrl->m_pWndCtrl->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                             SWP_NOSIZE | SWP_NOMOVE );
            pCtrl->m_pWndCtrl->Invalidate( );
            pCtrl->m_pWndCtrl->UpdateWindow( );
         }

#ifdef DEBUG_ALL
   zCHAR szMsg[ 256 ];

   TraceLineI( "TZCtrlCover::OnLButtonDown cnt: ",
               pPainterWindow->m_nCtrlCnt );
   for ( nPos = 0; nPos < pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = pPainterWindow->m_CtrlList[ nPos ];
   // while ( pCtrl )
      {
      // if ( pCtrl->m_pCtrlParent &&
      //      zstrcmp( pCtrl->m_pCtrlParent->m_csTag, "Candidates" ) == 0 )
      //      zstrcmp( pCtrl->m_csTag, "ClientActivity" ) == 0 )
         {
      //    pCtrl = pPainterWindow->m_CtrlList[ nPos ];
            sprintf_s( szMsg, zsizeof( szMsg ), " %3d.%s%-32s %-32s%s0x%04x:", nPos + 1,
                      pCtrl->IsSelected( ) ? "*" : " ",
                      pCtrl->m_pCtrlParent ?
                                    pCtrl->m_pCtrlParent->m_csTag : "<null>",
                      pCtrl->m_csTag,
                      (pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd )) ? " " : "~",
                      pCtrl->m_uVisible );
            TraceRect( szMsg, pCtrl->m_rectCtrl );
      //    break;
         }

      // pCtrl = pCtrl->m_pCtrlParent;
      }
   }

   TraceCtrl( "PTE Selecting Ctrl", pCtrlSelect );

#endif

#if 0
         // try to permit DBLCLK to occur when first selected
         if ( pCtrl->m_pCtrlCover->m_pSelectCover &&
              mIs_hWnd( pCtrl->m_pCtrlCover->m_pSelectCover->m_hWnd ) )
         {
            pCtrl->m_pCtrlCover->m_pSelectCover->OnLButtonDown( uModKeys, pt );
         }
#endif
         pPainterWindow->SetDIL_Text( );
      }
   }

   pPainterWindow->MoveAllSelectedFramesToTop( );
}

void
TZCtrlCover::OnMouseMove( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
// TracePoint( "TZCtrlCover::OnMouseMove", pt );
#endif

   if ( m_pCtrl->IsSelected( ) )
   {
//    TraceLineS( "TZCtrlCover::OnMouseMove ================>", "SELECTED" );
//    m_pCtrl->m_ptLastMouseMove = pt;
   }
}

void
TZCtrlCover::OnLButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZCtrlCover::OnLButtonUp", pt );
#endif

   // not needed ... code moved to SelectCover TrackSelection processing
}

void
TZCtrlCover::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZCtrlCover::OnLButtonDblClk", pt );
#endif

   zVIEW vTZPESRCO;

   m_pPainterWindow->m_pCurrCtrl = m_pCtrl;
   if ( GetViewByName( &vTZPESRCO, szlTZPESRCO, m_pPainterWindow->m_pBar->m_vSubtask, zLEVEL_TASK ) <= 0 )
      return;

   // If the mouse button has been double clicked:
   //  - If more than one ctrl is selected, go to the "common" detail update
   //    for attributes common to all selected ctrls.
   //  - Otherwise, go to the appropriate control update window.
   m_pPainterWindow->m_ulCommonFlags = 0;
   while ( m_pPainterWindow->m_nSelectCnt > 1 )  // purist's goto
   {
      TZPainterCtrl *pCtrl;
      zLONG         lType = 0;
      zSHORT        nPos;

      m_pPainterWindow->m_ulCommonFlags = 0xFFFFFFFF;
      m_pPainterWindow->m_pBar->m_pActivePainterWindow = m_pPainterWindow;
      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->IsSelected( ) )
         {
            if ( lType == 0 || lType == pCtrl->m_lType )
               lType = pCtrl->m_lType;
            else
               lType = -1;

            pCtrl->PositionOnZeidonCtrl( pCtrl->m_vCtrl );
            m_pPainterWindow->m_ulCommonFlags =
                       pCtrl->SetCommon( m_pPainterWindow->m_ulCommonFlags );
         }
      }

      if ( lType > 0 )
      {
         if ( SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef,
                                             szlKey, m_pCtrl->m_lType,
                                             0 ) >= zCURSOR_SET )
         {
            zBOOL  bProperties = FALSE;
            zPCHAR pch;
            zSHORT nRC;

            nRC = SetCursorFirstEntity( vTZPESRCO, "PropertyDef", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               GetAddrForAttribute( &pch, vTZPESRCO,
                                    "PropertyDef", "Update" );
               if ( *pch )
               {
                  bProperties = TRUE;
               }
               else
               {
                  DeleteEntity( vTZPESRCO, "PropertyDef", zREPOS_NONE );
               }

               nRC = SetCursorNextEntity( vTZPESRCO, "PropertyDef", 0 );
            }

            if ( bProperties )
            {
               m_pPainterWindow->m_ulCommonFlags = 0xFFFFFFFF;
               break;
            }
         }
      }

      if ( m_pPainterWindow->m_ulCommonFlags == 0 )
      {
         OperatorPrompt( m_pPainterWindow->m_pBar->m_pZSubtask->m_vDialog,
                         "Set Common Attributes",
                         "The selected controls have no common attributes",
                         1, zBUTTONS_OK, zRESPONSE_OK, 0 );
      }
      else
      {
         m_pPainterWindow->UpdateZeidonWindow( );
         SetNameForView( m_pCtrl->m_vCtrl, szlTZCONTROL, 0, zLEVEL_TASK );
         m_pPainterWindow->m_pBar->EnablePainterWindows( 0 );
         m_pPainterWindow->m_pBar->ResetSelectedMouseReleaseSwap( FALSE );
         SetWindowActionBehavior( m_pPainterWindow->m_pBar->m_vSubtask,
                                  zWAB_StartModalSubwindow,
                                  "TZPNCTAD", "CommonSelectedDetail" );
         ProcessActionLoop( m_pPainterWindow->m_pBar->m_pZSubtask,
                            0, 0, 0, 0, FALSE, "(pntr wnd)" );
      }

      return;
   }

   if ( SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef,
                                       szlKey, m_pCtrl->m_lType,
                                       0 ) >= zCURSOR_SET )
   {
      zPCHAR pchDialog;
      zPCHAR pchWindow;
      zLONG  lTypeHold;

      GetAddrForAttribute( &pchDialog, vTZPESRCO,
                           szlControlDef, szlMaintenanceDialog );
      GetAddrForAttribute( &pchWindow, vTZPESRCO,
                           szlControlDef, szlMaintenanceWindow );
      if ( pchDialog[ 0 ] && pchWindow[ 0 ] )
      {
         zVIEW vDialogCtrl;

      // ::MessageBox( 0, "Before UpdateZeidonWindow", "TZWINDOW", MB_OK );
         lTypeHold = m_pCtrl->m_lType; // save type before updating the window
         m_pPainterWindow->UpdateZeidonWindow( );
      // ::MessageBox( 0, "Before AfterZeidonWindow", "TZWINDOW", MB_OK );
         if ( GetViewByName( &vDialogCtrl, szlTZCONTROL,
                             m_pPainterWindow->m_pBar->m_vSubtask, zLEVEL_TASK ) > 0 &&
              vDialogCtrl != m_pPainterWindow->m_vDialog )
         {
            SetViewFromView( vDialogCtrl, m_pPainterWindow->m_vDialog );
         }
         else
         {
            CreateViewFromViewForTask( &vDialogCtrl,
                                       m_pPainterWindow->m_vDialog,
                                       m_pPainterWindow->m_vDialog );
            SetNameForView( vDialogCtrl, szlTZCONTROL,
                            m_pPainterWindow->m_vDialog, zLEVEL_TASK );
         }

         if ( m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
         {
            SetNameForView( vDialogCtrl, "PAGECTRL",
                            m_pPainterWindow->m_vDialog, zLEVEL_TASK );
         }

         // Position on the appropriate control in the dialog.
         m_pCtrl->PositionOnZeidonCtrl( vDialogCtrl );

         // Now that we are prepared to transfer to the update dialog for
         // the control, check to see if the control type has been changed
         // as a result of creating the Zeidon information for the control.
         // If so, re-do the positioning on the control in the PE object.
         if ( m_pCtrl->m_lType != lTypeHold )
         {
            pchDialog = 0;
            if ( SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef,
                                                szlKey, m_pCtrl->m_lType,
                                                0 ) >= zCURSOR_SET )
            {
               GetAddrForAttribute( &pchDialog, vTZPESRCO,
                                    szlControlDef, szlMaintenanceDialog );
               GetAddrForAttribute( &pchWindow, vTZPESRCO,
                                    szlControlDef, szlMaintenanceWindow );
            }
         }

         if ( pchDialog && pchDialog[ 0 ] && pchWindow[ 0 ] )
         {
            CWaitCursor wait;

            // Build the event/action object relating to the control.
            m_pPainterWindow->BuildTZPNEVWO( );

            WINDOWPLACEMENT wp;
            wp.length = sizeof( wp );
            if ( m_pPainterWindow->GetWindowPlacement( &wp ) )
            {
            // TraceRect( "TZPainterWindow::OnClose wp.rect", wp.rcNormalPosition );
               wp.flags = 0;
               if ( IsZoomed( ) )
                  wp.flags |= WPF_RESTORETOMAXIMIZED;

               // and write it to the .INI file
               WriteWindowPlacement( m_pPainterWindow->m_vDialog, "Design",
                                     m_pPainterWindow->m_csDlgTag,
                                     m_pPainterWindow->m_csWndTag, &wp );
            }

            CPoint ptScrollPos =
               m_pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
            SetProfileNbr( m_pPainterWindow->m_vDialog, "Design",
                           m_pPainterWindow->m_csDlgTag,
                           m_pPainterWindow->m_csWndTag,
                           "ScrollX", ptScrollPos.x );
            SetProfileNbr( m_pPainterWindow->m_vDialog, "Design",
                           m_pPainterWindow->m_csDlgTag,
                           m_pPainterWindow->m_csWndTag,
                           "ScrollY", ptScrollPos.y );

            // Name the view for the subdialog.
            SetNameForView( m_pPainterWindow->m_vDialog, szlTZWINDOW,
                            0, zLEVEL_TASK );
            if ( m_pPainterWindow->m_ulCommonFlags == 0xFFFFFFFF )
            {
               m_pPainterWindow->m_pBar->
                                 ResetSelectedMouseReleaseSwap( FALSE );
            }
            else
            {
               m_pPainterWindow->m_pBar->
                                 ResetSelectedMouseReleaseSwap( TRUE );
            }

            SetWindowActionBehavior( m_pPainterWindow->m_pBar->m_vSubtask,
                                     zWAB_StartModalSubwindow,
                                     pchDialog, pchWindow );
            m_pPainterWindow->m_pBar->EnablePainterWindows( 0 );
            ProcessActionLoop( m_pPainterWindow->m_pBar->m_pZSubtask,
                               0, 0, 0, 0, FALSE, "(pntr dlg)" );
         }
      }
   }
}

void
TZCtrlCover::OnRButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZCtrlCover::OnRButtonDown", pt );
#endif

   if ( (uModKeys & MK_CONTROL) == 0 )
   {
      TZActiveX *pAX = DYNAMIC_DOWNCAST( TZActiveX, m_pCtrl->m_pWndCtrl );
      if ( pAX )
      {
         pAX->OnRButtonDown( uModKeys, pt );
         return;
      }
   }

   CPoint ptScrollPos =
              m_pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
   pt.x -= ptScrollPos.x;
   pt.y -= ptScrollPos.y;

   // If we get an RBUTTONDOWN, ship it to the painter client window to
   // handle setting of Tab Stop order via rubberband selection or click
   // with the Shift or Ctrl Keys pressed.

   TZPainterCtrl *pCtrl = m_pCtrl;
   while ( pCtrl )
   {
      pt.x += pCtrl->m_rectCtrl.left;
      pt.y += pCtrl->m_rectCtrl.top;
      pCtrl = pCtrl->m_pCtrlParent;
   }

   if ( (uModKeys & MK_CONTROL) == 0 && (uModKeys & MK_SHIFT) == 0 )
   {
      if ( m_pPainterWindow->m_nSelectCnt == 0 )
      {
         m_pPainterWindow->m_pt = pt;
         m_pPainterWindow->m_pCurrCtrl = m_pCtrl;
         CreateTrackingPopupMenu( m_pPainterWindow->m_pBar->m_vSubtask,
                                  "UpdateCtrl", -1, -1, FALSE, FALSE );
      }
   }

   m_pPainterWindow->m_pPainterClient->OnRButtonDown( uModKeys, pt );
}

void
TZCtrlCover::OnRButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZCtrlCover::OnRButtonUp", pt );
#endif
}

// This method is implemented to show a different border for the last
// selected ctrl.
void
TZCtrlCover::OnNcPaint( )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZCtrlCover::OnNcPaint: ", m_pCtrl->m_csTag );
#endif

   Default( );
}

void
TZCtrlCover::OnPaint( )
{
   CPaintDC dc( this );   // a paint DC calls Begin and End Paint
   CRect& rect = *(CRect *) &dc.m_ps.rcPaint;

#ifdef DEBUG_ALL // lots of messages

   if ( m_pCtrl )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, zsizeof( szMsg ), "TZCtrlCover::OnPaint: %s ====>", m_pCtrl->m_csTag );
      TraceRect( szMsg, m_pCtrl->m_rectPainter );
   }
   else
      TraceRect( "TZCtrlCover::OnPaint: ???? ====>", rect );

#endif

#if 0
   if ( (m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
        m_pCtrl && m_pCtrl->m_chEntity != 'C' )
   {
      int nOldBkMode = dc.SetBkMode( TRANSPARENT );

      if ( rect.left < m_pPainterWindow->m_lLeftMargin )
      {
         CRect rectOrig = rect;
         CBrush brush( ::GetSysColor( COLOR_BTNFACE ) );

         if ( rect.right > m_pPainterWindow->m_lLeftMargin )
            rect.right = m_pPainterWindow->m_lLeftMargin;

         dc.FillRect( rect, &brush );
         rect = rectOrig;
      }

//    dc.SetBkMode( nOldBkMode );
   }
#endif

   if ( m_pPainterWindow->m_pBar->m_fState & zTZCB_SHOWTABS )
   {
      CRect     rect;
      zCHAR     szTab[ 6 ];
      CClientDC dcClient( this );

      CFont *pFont =
            CFont::FromHandle( m_pPainterWindow->m_pBar->m_hFontTab );
      GetClientRect( rect );
      CFont *pFontOld = (CFont *) dcClient.SelectObject( pFont );
      COLORREF clrBk = ::GetSysColor( COLOR_ACTIVECAPTION );
      COLORREF clrTx = ::GetSysColor( COLOR_CAPTIONTEXT );
      COLORREF clrTxSave = dcClient.SetTextColor( clrTx );
      COLORREF clrBkSave = dcClient.SetBkColor( clrBk );

      if ( m_pCtrl->m_nTabStopNbr > 0 && m_pCtrl->m_nTabStopNbr < 9999 )
         _ltoa_s( m_pCtrl->m_nTabStopNbr, szTab, zsizeof( szTab ), 10 );
      else
         szTab[ 0 ] = '0', szTab[ 1 ] = 0;

      strcat_s( szTab, " " );
      dcClient.TextOut( rect.left + 2, rect.top + 2, szTab, zstrlen( szTab ) );

      dcClient.SetTextColor( clrTxSave );
      dcClient.SetBkColor( clrBkSave );
      dcClient.SelectObject( pFontOld );
   }
}

TZPainterCtrl *
TZCtrlCover::FindChildmostUnselected( TZPainterCtrl *pCtrlParent,
                                      CPoint& pt,
                                      zBOOL   bIgnoreSelected )
{
#ifdef DEBUG_ALL
   TracePoint( "TZCtrlCover::FindChildmostUnselected", pt );
#endif

   TZPainterCtrl *pCtrl;
   TZPainterCtrl *pChildCtrl;
   CRect rect;

   zSHORT nPos = m_pPainterWindow->m_nCtrlCnt;
   while ( nPos-- )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_pCtrlParent == pCtrlParent )
      {
         pCtrl->ParentToPainterRect( rect );
         if ( pt.x >= rect.left    &&
              pt.x <= rect.right   &&
              pt.y >= rect.top     &&
              pt.y <= rect.bottom )
         {
            if ( pCtrl->IsSelected( ) && bIgnoreSelected == FALSE )
               return( 0 );

            pChildCtrl = FindChildmostUnselected( pCtrl, pt,
                                                  bIgnoreSelected );
            if ( pChildCtrl )
               return( pChildCtrl );
            else
            if ( pCtrl->GetVisibility( ) <= 1 ) // visible
               return( pCtrl );
            else
               return( 0 );
         }
      }
   }

   return( 0 );
}

// Get the CtrlCover window's rect (with or without the border).
void
TZCtrlCover::GetAdjustedWindowRect( CRect& rect,
                                    zBOOL bIncludeSelect )
{
   TraceRect( "AdjustedWindowRect should not be here???", rect );
   return;

#if 0

   CPoint ptScrollPos =
     m_pCtrl->m_pPainterWindow->m_pPainterClient->GetScrollDevicePosition( );

   // Get the screen coordinates
   m_pCtrl->ParentToPainterRect( rect );
   TraceRect( "AdjustedWindowRect", rect );

   if ( bIncludeSelect )
   {
      // Not selected, but we want to include the selection border
//    rect.InflateRect( TZCtrlCover::m_nFrameSizeX,
//                      TZCtrlCover::m_nFrameSizeY );
      TraceRect( "AdjustedWindowRect (includes border)", rect );
   }
   else
   {
      TraceRect( "AdjustedWindowRect (without border)", rect );
   }

   pt.x = rect.left + ptScrollPos.x;
   pt.y = rect.top + ptScrollPos.y;

//   // Convert the screen top and left position to the offset within the
//   // PainterClient's client area.
//   GetParent( )->ScreenToClient( &pt );

//   // Now change rect to be a client area position rather than
//   // a screen position
//   rect.right -= (rect.left - pt.x);
//   rect.left = pt.x;
//   rect.bottom -= (rect.top - pt.y);
//   rect.top = pt.y;
   TraceRect( "AdjustedWindowRect final", rect );

#endif
}

void
TZCtrlCover::MoveCtrl( CRect& rectNew,  // in painter coordinates
                       TZPainterCtrl *pNewParent,
                       zSHORT nIdNbr, zCPCHAR cpcLabel )
{
   ASSERT( m_pCtrl );
   ASSERT( m_pCtrl->m_pWndCtrl );

#ifdef DEBUG_ALL
   TraceCtrl( "TZCtrlCover::MoveCtrl: ", m_pCtrl );
   TraceRect( "TZCtrlCover::MoveCtrl rectNew:", rectNew );
#endif

   if ( mIs_hWnd( m_pCtrl->m_pWndCtrl->m_hWnd ) == 0 )
   {
      TraceCtrl( "TZCtrlCover::MoveCtrl Invalid Window: ", m_pCtrl );
      return;
   }

   m_pPainterWindow->m_pPainterClient->m_nMultiMoveSize++;
   TZPainterCtrl *pLastSelected = m_pPainterWindow->m_pLastSelected;
   zSHORT nSelectCnt = m_pPainterWindow->m_nSelectCnt;
   zBOOL bSelected = m_pCtrl->IsSelected( );

   // Zero m_pLastSelected so DIL is not cleared.
   m_pPainterWindow->m_pLastSelected = 0;
   if ( bSelected )
   {
      if ( m_pCtrl == pLastSelected )
         bSelected = -1;

      m_pCtrl->Deselect( );
   }

   if ( pNewParent && pNewParent != (TZPainterCtrl *) -1 )
   {
      // Make sure that we know who the real parent is.
      pNewParent = pNewParent->GetRealCtrl( );
   }

   if ( pNewParent != (TZPainterCtrl *) -1 )
   {
      if ( m_pCtrl->m_pCtrlParent &&
           pNewParent == m_pCtrl->m_pCtrlParent->GetRealCtrl( ) )
      {
         // The ctrl's parent is not really changing.
         pNewParent = (TZPainterCtrl *) -1;
      }
   }

// if ( pNewParent && pNewParent != (TZPainterCtrl *) -1 )
//    TraceLineS( "TZCtrlCover::MoveCtrl ParentReal: ", pNewParent->m_csTag );

   if ( nIdNbr )
   {
      zCHAR szLabel[ 64 ];

      if ( rectNew.top == m_pCtrl->m_rectPainter.top &&
           rectNew.left == m_pCtrl->m_rectPainter.left )
      {
         if ( cpcLabel )
            strcpy_s( szLabel, zsizeof( szLabel ), cpcLabel );
         else
            sprintf_s( szLabel, zsizeof( szLabel ), "&xxdo Size %d item%s\tCtrl+x",
                      nSelectCnt, nSelectCnt == 1 ? "" : "s" );

         m_pPainterWindow->AddToUndo( m_pCtrl, nIdNbr, -1, bSelected,
                                      TZUndoRedo::eSize, szLabel );
      }
      else
      {
         if ( cpcLabel )
            strcpy_s( szLabel, zsizeof( szLabel ), cpcLabel );
         else
            sprintf_s( szLabel, zsizeof( szLabel ), "&xxdo Move %d item%s\tCtrl+x",
                      nSelectCnt, nSelectCnt == 1 ? "" : "s" );

         m_pPainterWindow->AddToUndo( m_pCtrl, nIdNbr, -1, bSelected,
                                      TZUndoRedo::eMove, szLabel );
      }
   }

   TZPainterCtrl *pOldParent = m_pCtrl->m_pCtrlParent;

   if ( pNewParent != (TZPainterCtrl *) -1 && pNewParent != pOldParent )
   {
      CRect rectPainter = rectNew;

//    m_pCtrl->ParentToPainterRect( rectPainter );
      m_pCtrl->MoveZeidonCtrl( pNewParent );
      m_pCtrl->m_pCtrlParent = pNewParent;
      m_pCtrl->PainterToParentRect( rectPainter );
      if ( pNewParent )
      {
         // If a parent CtrlCover was found, then position the control
         // so it is completely contained within the found parent.

         // Now that we've gotten the parent rectangle for the control,
         // make sure that the control is completely within the parent.
         if ( m_pCtrl->m_rectCtrl.left < 0 )
         {
            m_pCtrl->m_rectCtrl.right -= m_pCtrl->m_rectCtrl.left;
            m_pCtrl->m_rectCtrl.left = 0;
         }
         else
         if ( m_pCtrl->m_rectCtrl.right >
              (pNewParent->m_rectCtrl.right - pNewParent->m_rectCtrl.left) )
         {
            m_pCtrl->m_rectCtrl.left -= (m_pCtrl->m_rectCtrl.right -
               (pNewParent->m_rectCtrl.right - pNewParent->m_rectCtrl.left));
            m_pCtrl->m_rectCtrl.right -= (m_pCtrl->m_rectCtrl.right -
               (pNewParent->m_rectCtrl.right - pNewParent->m_rectCtrl.left));
         }

         if ( m_pCtrl->m_rectCtrl.top < 0 )
         {
            m_pCtrl->m_rectCtrl.bottom -= m_pCtrl->m_rectCtrl.top;
            m_pCtrl->m_rectCtrl.top = 0;
         }
         else
         if ( m_pCtrl->m_rectCtrl.bottom > (pNewParent->m_rectCtrl.bottom -
                                                pNewParent->m_rectCtrl.top) )
         {
            m_pCtrl->m_rectCtrl.top -= (m_pCtrl->m_rectCtrl.bottom -
               (pNewParent->m_rectCtrl.bottom - pNewParent->m_rectCtrl.top));
            m_pCtrl->m_rectCtrl.bottom -= (m_pCtrl->m_rectCtrl.bottom -
               (pNewParent->m_rectCtrl.bottom - pNewParent->m_rectCtrl.top));
         }
      }
      else
      {
         m_pCtrl->m_rectPainter = m_pCtrl->m_rectCtrl; // no parent control
      }
   }
   else
   {
      // We are in the same parent.
      pNewParent = m_pCtrl->m_pCtrlParent;
      if ( pNewParent && pNewParent->m_bPlaceHolder )
      {
         zLONG lHeight = rectNew.Height( );
         zLONG lWidth = rectNew.Width( );

         if ( pNewParent->m_rectCtrl.Width( ) != lWidth ||
              pNewParent->m_rectCtrl.Height( ) != lHeight )
         {
            // We must be trying to resize the ctrl.  Since the parent is a
            // place holder ctrl, resize the parent to that of the ctrl.
            if ( pNewParent->m_rectCtrl.Width( ) != lWidth )
               pNewParent->m_rectCtrl.right = pNewParent->m_rectCtrl.left + lWidth;

            if ( pNewParent->m_rectCtrl.Height( ) != lHeight )
               pNewParent->m_rectCtrl.bottom = pNewParent->m_rectCtrl.top + lHeight;

            pNewParent->m_pWndCtrl->MoveWindow( pNewParent->m_rectCtrl.left,
                                                pNewParent->m_rectCtrl.top,
                                                pNewParent->m_rectCtrl.right -
                                                                pNewParent->m_rectCtrl.left,
                                                pNewParent->m_rectCtrl.bottom -
                                                                pNewParent->m_rectCtrl.top,
                                                TRUE );
         }
         else
         {
            CRect rectPainter = pNewParent->m_rectCtrl;

         // pNewParent->PainterToParentRect( rectPainter );
            pNewParent->ParentToPainterRect( rectPainter );
            if ( rectPainter != rectNew )
            {
               // We must be trying to move the ctrl, so we need to adjust the
               // parent place holder ctrl and this ctrl.
               pNewParent->m_rectCtrl.bottom = pNewParent->m_rectCtrl.top + lHeight;
               pNewParent->m_rectCtrl.left = rectNew.left;
               pNewParent->m_rectCtrl.right = rectNew.right;
               rectNew = pNewParent->m_rectCtrl;
            }
         }
      }

      m_pCtrl->PainterToParentRect( rectNew );
   }

// TraceRect( "TZCtrlCover::MoveCtrl before SendCtrlMsg rectNew", rectNew );

   // Convert from rectNew (in client coords) to m_rectCtrl (in parent
   // coords).  N.B.  rectNew MUST be initialized in client coords!!!
// m_pCtrl->PainterToParentRect( rectNew );

   // Give the ctrl a chance to set the rectangle.
   m_pPainterWindow->SendCtrlMsg( zCTRLMSG_MOVE, 0, m_pCtrl->m_lType,
                                  m_pCtrl->m_pCtrlParent ? m_pCtrl->m_pCtrlParent->m_lType : 0,
                                  m_pCtrl->m_bPlaceHolder,
                                  &(m_pCtrl->m_rectCtrl), 0, m_pCtrl );
// m_pCtrl->PreMoveCtrl( rectNew );

   // Now get rectNew in case the ctrl modified its m_rectCtrl.
   // Convert from m_rectCtrl (in parent coords) to rectNew (in client
   // coords).  N.B.  rectNew does NOT need to be initialized ... in
   // fact, initial values are destroyed.
   m_pCtrl->ParentToPainterRect( rectNew );
   m_pCtrl->m_rectPainter = rectNew;

// TraceRect( "TZCtrlCover::MoveCtrl after SendCtrlMsg rectNew", rectNew );

   CPoint ptScrollPos =
             m_pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );

#if 0
   if ( m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
   {
      if ( m_pCtrl->m_chEntity == 'C' )
      {
         if ( rectNew.left + ptScrollPos.x <
                                   m_pCtrl->m_pPainterWindow->m_lLeftMargin )
         {
            rectNew.right = rectNew.right - ptScrollPos.x +
                 m_pCtrl->m_pPainterWindow->m_lLeftMargin - rectNew.left;
            rectNew.left = m_pCtrl->m_pPainterWindow->m_lLeftMargin -
                                                               ptScrollPos.x;
         }
      }
      else
      {
//       zCHAR szEntity[ 2 ];
//       szEntity[ 0 ] = m_pCtrl->m_chEntity;
//       szEntity[ 1 ] = 0;
//       TraceLineS( "==CtrlCover PosChanged chEntity: ", szEntity );

         if ( m_pCtrl->m_chEntity == 'G' )
            rectNew.left = 8;
         else
            rectNew.left = 4;

         rectNew.right = m_pCtrl->m_pPainterWindow->m_lLeftMargin - 1 +
            mConvert256thInchToPixelX( m_pCtrl->m_pPainterWindow->
                                                               m_lRptSizeX );
//       if ( m_pCtrl->m_chEntity == 'G' )
//          rectNew.right -= 4;
      }
   }
#endif

   m_pCtrl->m_rectPainter = rectNew;

   // This hide window prevents the SetParent call from redrawing and
   // repainting the control before its time.
   if ( mIs_hWnd( m_hWnd ) )
      ShowWindow( SW_HIDE );

   m_pCtrl->m_pWndCtrl->ShowWindow( SW_HIDE );

   // Now that the position and parent has been established, change
   // the parentage of the control if necessary.
   if ( pNewParent != pOldParent )
   {
      if ( pOldParent )
         pOldParent->m_nChildren--;

      m_pCtrl->m_pWndCtrl->SetParent( m_pPainterWindow->m_pPainterClient );
      if ( pNewParent )
      {
         m_pCtrl->m_pWndCtrl->SetParent( pNewParent->m_pWndCtrl );
         pNewParent->m_nChildren++;
      }
   }

// TraceRect( "TZCtrlCover::MoveCtrl moving Ctrl", m_pCtrl->m_rectCtrl );

   // If the SWP_SHOWWINDOW or SWP_HIDEWINDOW flag is set, the window cannot
   // be moved or sized.
   if ( m_pCtrl->m_pCtrlParent )
   {
      m_pCtrl->m_pWndCtrl->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                         SWP_NOSIZE | SWP_NOMOVE );
      m_pCtrl->m_pWndCtrl->MoveWindow( m_pCtrl->m_rectCtrl.left,
                                       m_pCtrl->m_rectCtrl.top,
                                       m_pCtrl->m_rectCtrl.right -
                                                    m_pCtrl->m_rectCtrl.left,
                                       m_pCtrl->m_rectCtrl.bottom -
                                                    m_pCtrl->m_rectCtrl.top,
                                       TRUE );
   }
   else
   {
      m_pCtrl->m_pWndCtrl->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                         SWP_NOSIZE | SWP_NOMOVE );
      m_pCtrl->m_pWndCtrl->MoveWindow( m_pCtrl->m_rectCtrl.left - ptScrollPos.x,
                                       m_pCtrl->m_rectCtrl.top - ptScrollPos.y,
                                       m_pCtrl->m_rectCtrl.right -
                                                      m_pCtrl->m_rectCtrl.left,
                                       m_pCtrl->m_rectCtrl.bottom -
                                                      m_pCtrl->m_rectCtrl.top,
                                       TRUE );
   }

   // m_pCtrl->m_pWndCtrl->ShowWindow( SW_SHOW ); // crashes with ActiveX
   if ( mIs_hWnd( m_hWnd ) )
      ::ShowWindow( m_pCtrl->m_pWndCtrl->m_hWnd, SW_SHOW );

   m_pCtrl->m_pWndCtrl->UpdateWindow( );
// TraceRect( "TZCtrlCover::MoveCtrl moving CtrlCover", rectNew );

   m_pCtrl->m_rectPainter = rectNew;

   if ( mIs_hWnd( m_hWnd ) )
   {
      MoveWindow( rectNew.left - ptScrollPos.x,
                  rectNew.top - ptScrollPos.y,
                  rectNew.right - rectNew.left,
                  rectNew.bottom - rectNew.top, TRUE );
      SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
      UpdateWindow( );
   }

   m_pCtrl->MoveDescendentCoveringWindows( );

   if ( bSelected )
      m_pCtrl->Select( );

//?m_pPainterWindow->m_pPainterClient->InvalidateRect( rectPainter, TRUE );

// TraceLineI( "TZCtrlCover::MoveCtrl nSelectCnt: ", nSelectCnt );
   m_pPainterWindow->SetDIL_Text( m_pCtrl, nSelectCnt == 1 ? 0 : 1 );
   m_pCtrl->PostMoveCtrl( );

#if 0
   TraceRect( "TZCtrlCover::MoveCtrl moving CtrlCover", rectNew );

   TZPainterCtrl *pCtrl;
   zSHORT nPos;

   for ( nPos = 0; nPos < m_pCtrl->m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pCtrl->m_pPainterWindow->m_CtrlList[ nPos ];
      TraceCtrl( "Post MoveCtrl: ", pCtrl );
   }
#endif

   if ( m_pCtrl && m_pCtrl->m_pPainterWindow &&
        m_pCtrl->m_pPainterWindow->m_pBar &&
        (m_pCtrl->m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
        m_pCtrl->m_pPainterWindow->m_pPainterClient &&
        m_pCtrl->m_pPainterWindow->m_pPainterClient->m_nRecurse == 0 )
   {
      m_pCtrl->m_pPainterWindow->m_pPainterClient->AdjustGroupSets( );
   }

   m_pPainterWindow->m_pLastSelected = pLastSelected;

// TraceCtrl( "TZCtrlCover::MoveCtrl (post): ", m_pCtrl );
   m_pPainterWindow->m_pPainterClient->m_nMultiMoveSize--;
}

//
// typedef struct _WINDOWPOS
// {
//    HWND hwnd;
//    HWND hwndInsertAfter;
//    int  x;
//    int  y;
//    int  cx;
//    int  cy;
//    UINT uFlags;
//
// } WINDOWPOS;
//
//
// hwndInsertAfter may be one of the following special values:
//
//    HWND_BOTTOM - Places the window at the bottom of the Z order.  If the
//       hWnd parameter identifies a topmost window, the window loses its
//       topmost status and is placed at the bottom of all other windows.
//
//    HWND_NOTOPMOST - Places the window above all non-topmost windows (that
//       is, behind all topmost windows). This flag has no effect if the
//       window is already a non-topmost window.
//
//    HWND_TOP - Places the window at the top of the Z order.
//
//    HWND_TOPMOST - Places the window above all non-topmost windows. The
//       window maintains its topmost position even when it is deactivated.
//
void
TZCtrlCover::OnWindowPosChanged( WINDOWPOS *pWindowPos )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZCtrlCover::OnWindowPosChanged: ", m_pCtrl->m_csTag );
#endif

#if 1
// Default( );
#if 0
   if ( ((pWindowPos->flags & SWP_NOMOVE) &&
         (pWindowPos->flags & SWP_NOSIZE)) &&
        m_pCtrl && m_pCtrl->IsSelected( ) )
   {
      zSHORT nAdjust = m_pCtrl->m_pCtrlCover->
                                    m_pSelectCover->m_nTrackerHandleSize;
      ::SetWindowPos( m_pCtrl->m_pCtrlCover->m_pSelectCover->m_hWnd,
                      pWindowPos->hwndInsertAfter,
                      pWindowPos->x - nAdjust,
                      pWindowPos->y - nAdjust,
                      pWindowPos->cx + 2 * nAdjust,
                      pWindowPos->cy + 2 * nAdjust,
                      pWindowPos->flags );
   }
#endif
#else
   CRect  rectNew;

   rectNew.top = pWindowPos->y;
   rectNew.left = pWindowPos->x;
   rectNew.right = rectNew.left + pWindowPos->cx;
   rectNew.bottom = rectNew.top + pWindowPos->cy;
// TraceRect( "TZCtrlCover::OnWindowPosChanged new Pos", rectNew );

   if ( m_pCtrl == 0 || m_pCtrl->m_pPainterWindow == 0 ||
        m_pCtrl->m_pPainterWindow->m_pBar == 0 ||
        ((pWindowPos->flags & SWP_NOMOVE) &&
         (pWindowPos->flags & SWP_NOSIZE)) )
   {
      // WM_SIZE and WM_MOVE messages are not sent if an application handles
      // the WM_WINDOWPOSCHANGED message without calling DefWindowProc.  It
      // is more efficient to handle move or size change processing during
      // the WM_WINDOWPOSCHANGED message without calling DefWindowProc.
   // Default( );
      return;
   }

   if ( rectNew == m_pCtrl->m_rectPainter )
   {
      return;
   }

   if ( pWindowPos->cx < m_nMinWindowX || pWindowPos->cy < m_nMinWindowY )
   {
      if ( pWindowPos->cx < m_nMinWindowX )
      {
         if ( rectNew.left == m_pCtrl->m_rectPainter.left )
            rectNew.right = rectNew.left + m_nMinWindowX;
         else
            rectNew.left = rectNew.right - m_nMinWindowX;
      }

      if ( pWindowPos->cy < m_nMinWindowY )
      {
         if ( rectNew.top == m_pCtrl->m_rectPainter.top )
            rectNew.bottom = rectNew.top + m_nMinWindowY;
         else
            rectNew.top = rectNew.bottom - m_nMinWindowY;
      }
   }

   m_pCtrl->m_rectPainter = rectNew;
   CRect  rectPainter = m_pCtrl->m_rectPainter;

// TraceRect( "TZCtrlCover::OnWindowPosChanged rectPainter", rectPainter );
// TraceRect( "                                    rectNew", rectNew );
// TraceLineI( "                                   Selected ",
//             m_pCtrl->IsSelected( ) );

   // We are resizing the CtrlCover, so we must resize the associated ctrl.
   MoveCtrl( rectPainter, rectNew );
#endif
}

void
TZCtrlCover::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZCtrlCover::OnKeyDown: ", m_pCtrl->m_csTag );
#endif

   Default( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TZOrderCtrl::TZOrderCtrl( TZPainterCtrl *pCtrl,
                          zSHORT nArrayPos,
                          CRect& rectCtrl )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZOrderCtrl::TZOrderCtrl: ", pCtrl->m_csTag );
#endif

   m_pCtrl = pCtrl;
   m_nArrayPos = nArrayPos;
   m_rectCtrl = rectCtrl;
   m_pNext = 0;
   m_pPrev = 0;
}

TZOrderCtrl::~TZOrderCtrl( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZOrderCtrl::dtor: ", m_pCtrl->m_csTag );
#endif

   mDeleteInit( m_pNext );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TZUndoRedo::TZUndoRedo( TZPainterCtrl *pCtrl, zSHORT nIdNbr,
                        SHORT nArrayPos, zBOOL bSelected,
                        eUNDO_REDO_TYPE eType, zCPCHAR cpcLabel )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZUndoRedo::ctor: ", pCtrl->m_csTag );
#endif

   m_pCtrl = pCtrl;
   m_nIdNbr = nIdNbr;
   m_nArrayPos = nArrayPos;
   m_eType = eType;
   if ( cpcLabel )
      strcpy_s( m_szLabel, zsizeof( m_szLabel ), cpcLabel );
   else
      m_szLabel[ 0 ] = 0;

   m_pNext = 0;
   m_pPrev = 0;
   m_uVisible = m_pCtrl->m_uVisible;   // Retain Visibility value
   m_bSelected = bSelected;            // Retain selected state
   m_rect = m_pCtrl->m_rectCtrl;       // Retain Pos/Size for Move/Resize
   m_rectPainter = m_pCtrl->m_rectPainter;
   m_pParent = m_pCtrl->m_pCtrlParent; // Retain Parent for Move/Resize
   m_pCtrl->HideForUndo( eType == eDelete ? TRUE : FALSE );
}

TZUndoRedo::~TZUndoRedo( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZUndoRedo::dtor: ", m_pCtrl->m_csTag );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zSHORT
fnCompareWebCtrls( const TZWebCtrl *pwc1,
                   const TZWebCtrl *pwc2 )
{
   return( pwc1->m_csTag.Compare( pwc2->m_csTag ) );
}

zSHORT
fnSeekWebCtrls( const CString *pzsTag,
                const TZWebCtrl *pwc )
{
   return( pzsTag->Compare( pwc->m_csTag ) );
}

TZWebCtrl::TZWebCtrl( CRect&   rect,
                      zCPCHAR  cpcTag,
                      zLONG    lType ) :
           m_csTag( cpcTag ),
           m_rect( rect )
{
   m_lType = lType;  // Ctrl type (PPE)
   m_pTop = 0;
   m_pLeft = 0;
}

TZWebCtrl::~TZWebCtrl( )
{
   mDeleteInit( m_pTop );
   mDeleteInit( m_pLeft );
}

zLONG
TZPainterBar::CreateFlatCtrl( zVIEW vDialog, CRect& rectParent,
                              zLONG lMultX, zLONG lDivX,
                              zLONG lMultY, zLONG lDivY )
{
   TZWebCtrl *pWebCtrl;
   zCHAR  szTag[ zTAG_LTH ];
   CRect  rect;
   zLONG  lPosX;
   zLONG  lPosY;
   zLONG  lSizeX;
   zLONG  lSizeY;
   zLONG  lType;
   zLONG  lListCnt = 0;
   zSHORT nRC;

   OrderEntityForView( vDialog, szlControl, "PSDLG_Y A PSDLG_X A" );

   nRC = SetCursorFirstEntity( vDialog, szlControl, 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lType, vDialog, szlControlDef, szlKey );
      GetStringFromAttribute( szTag, zsizeof( szTag ), vDialog, szlControl, "Tag" );
      GetIntegerFromAttribute( &lPosX, vDialog, szlControl, szlPSDLG_X );
      GetIntegerFromAttribute( &lPosY, vDialog, szlControl, szlPSDLG_Y );
      GetIntegerFromAttribute( &lSizeX, vDialog, szlControl, szlSZDLG_X );
      GetIntegerFromAttribute( &lSizeY, vDialog, szlControl, szlSZDLG_Y );
      lPosX = lPosX * lMultX / lDivX;
      lPosY = lPosY * lMultY / lDivY;
      lSizeX = lSizeX * lMultX / lDivX;
      lSizeY = lSizeY * lMultY / lDivY;
      rect = rectParent;
      rect.left += lPosX;
      rect.right += lPosX + lSizeX;
      rect.top += lPosY;
      rect.bottom += lPosY + lSizeY;
      if ( lType != zGROUPBOX_CONTROL )
      {
         pWebCtrl = new TZWebCtrl( rect, szTag, lType );
         m_pZMIXWebCtrl->InsertNode( pWebCtrl );
      }

      if ( lType == zGRID_CONTROL )
         lListCnt++;
      else
      if ( CheckExistenceOfEntity( vDialog, szlCtrlCtrl ) == 0 )
      {
         SetViewToSubobject( vDialog, szlCtrlCtrl );
         lListCnt += CreateFlatCtrl( vDialog, rect,
                                     lMultX, lDivX, lMultY, lDivY );
         ResetViewFromSubobject( vDialog );
      }

      nRC = SetCursorNextEntity( vDialog, szlControl, 0 );
   }

   return( lListCnt );
}

zOPER_EXPORT zLONG OPERATION
CreateFlatCtrlList( zVIEW vSubtask, zCPCHAR cpcWndTag,
                    zLONG lMultX, zLONG lDivX,
                    zLONG lMultY, zLONG lDivY )
{
   TZPainterBar    *pPainterBar;
   zVIEW           vTZPNTROO;
   zULONG          ulBlobMax;
   zLONG           lListCnt = 0;

   GetViewByName( &vTZPNTROO, szlTZPNTROO, vSubtask, zLEVEL_TASK );
   if ( vTZPNTROO )
   {
      ulBlobMax = sizeof( zPVOID );
      GetBlobFromAttribute( &pPainterBar, &ulBlobMax, vTZPNTROO,
                            szlPalette, szlPointer );
      if ( pPainterBar )
      {
         pPainterBar->DeleteWebCtrls( );
         if ( pPainterBar->m_pActivePainterWindow )
         {
            zVIEW vDialog;

            CreateViewFromView( &vDialog, pPainterBar->m_pActivePainterWindow->m_vDialog );
            if ( SetCursorFirstEntityByString( vDialog, szlWindow, szlTag,
                                               cpcWndTag, 0 ) >= zCURSOR_SET &&
                 CheckExistenceOfEntity( vDialog, szlControl ) == 0 )

            {
               CRect rect( 0, 0, 0, 0 );
               zBOOL bFloatV = FALSE;
               zBOOL bFloatH = FALSE;

               pPainterBar->m_pZMIXWebCtrl =
                              new ZMultiIdxSet( (fnCOMPARE) fnCompareWebCtrls,
                                                (fnCOMPARE) fnSeekWebCtrls,
                                                100 );

               lListCnt = pPainterBar->CreateFlatCtrl( vDialog, rect,
                                                       lMultX, lDivX,
                                                       lMultY, lDivY );
               if ( lListCnt )
               {
                  TZWebCtrl *pWebList;
                  TZWebCtrl *pWebCtrl;
                  ZNameItem *pName;
                  zLONG     lCnt = pPainterBar->m_pZMIXWebCtrl->GetCount( );
                  zLONG     k;
                  zLONG     j;

                  for ( k = 0; k < lCnt; k++ )
                  {
                     if ( *(pPainterBar->m_pZMIXWebCtrl->m_pvNode + k) )
                     {
                        pWebList = (TZWebCtrl *) *(pPainterBar->m_pZMIXWebCtrl->m_pvNode + k);
                        if ( pWebList->m_lType == zGRID_CONTROL )
                        {
                           for ( j = 0; j < lCnt; j++ )
                           {
                              pWebCtrl = (TZWebCtrl *) *(pPainterBar->m_pZMIXWebCtrl->m_pvNode + j);
                              if ( pWebCtrl != pWebList )
                              {
                              // if ( pWebCtrl->m_rect.left > pWebList->m_rect.right &&
                              //      ((pWebCtrl->m_rect.top > pWebList->m_rect.top &&
                              //        pWebCtrl->m_rect.top < pWebList->m_rect.bottom) ||
                              //       (pWebCtrl->m_rect.bottom > pWebList->m_rect.top &&
                              //        pWebCtrl->m_rect.bottom < pWebList->m_rect.bottom)) )
                                 if ( pWebCtrl->m_rect.left < pWebList->m_rect.right ||
                                      pWebCtrl->m_rect.top > pWebList->m_rect.bottom ||
                                      pWebCtrl->m_rect.bottom < pWebList->m_rect.top )
                                 {
                                 }
                                 else
                                 {
                                    pName = new ZNameItem( pWebList->m_csTag );
                                    pName->m_pNextName = pWebCtrl->m_pLeft;
                                    pWebCtrl->m_pLeft = pName;
                                    bFloatH = TRUE;
                                 }

                              // if ( pWebCtrl->m_rect.top > pWebList->m_rect.bottom &&
                              //      ((pWebCtrl->m_rect.left >= pWebList->m_rect.left &&
                              //        pWebCtrl->m_rect.left < pWebList->m_rect.right) ||
                              //       (pWebCtrl->m_rect.right > pWebList->m_rect.left &&
                              //        pWebCtrl->m_rect.right <= pWebList->m_rect.right)) )
                                 if ( pWebCtrl->m_rect.top < pWebList->m_rect.bottom ||
                                      pWebCtrl->m_rect.left > pWebList->m_rect.right ||
                                      pWebCtrl->m_rect.right < pWebList->m_rect.left )
                                 {
                                 }
                                 else
                                 {
                                    pName = new ZNameItem( pWebList->m_csTag );
                                    pName->m_pNextName = pWebCtrl->m_pTop;
                                    pWebCtrl->m_pTop = pName;
                                    bFloatV = TRUE;
                                 }
                              }
                           }
                        }
                     }
                  }
               }

               if ( bFloatV == FALSE && bFloatH == FALSE )
               {
                  lListCnt = 0;
                  pPainterBar->DeleteWebCtrls( );
               }
               else
               {
                  if ( bFloatV && bFloatH )
                     lListCnt = 3;
                  else
                  if ( bFloatV )
                     lListCnt = 1;
                  else
                  if ( bFloatH )
                     lListCnt = 2;
               }
            }

            DropView( vDialog );
         }
      }
   }

   return( lListCnt );
}

zOPER_EXPORT zLONG OPERATION
GetNextWebListCtrl( zVIEW vSubtask, zPCHAR pchListCtrlTag,
                    zPLONG plHeight, zPLONG plWidth )
{
   TZPainterBar    *pPainterBar;
   zVIEW           vTZPNTROO;
   zULONG          ulBlobMax;

   GetViewByName( &vTZPNTROO, szlTZPNTROO, vSubtask, zLEVEL_TASK );
   if ( vTZPNTROO )
   {
      ulBlobMax = sizeof( zPVOID );
      GetBlobFromAttribute( &pPainterBar, &ulBlobMax, vTZPNTROO,
                            szlPalette, szlPointer );
      if ( pPainterBar && pPainterBar->m_pZMIXWebCtrl )
      {
         TZWebCtrl *pWebCtrl;
         zLONG     lCnt = pPainterBar->m_pZMIXWebCtrl->GetCount( );
         zLONG     k;

         for ( k = 0; k < lCnt; k++ )
         {
            if ( *(pPainterBar->m_pZMIXWebCtrl->m_pvNode + k) )
            {
               pWebCtrl = (TZWebCtrl *) *(pPainterBar->m_pZMIXWebCtrl->m_pvNode + k);
               if ( pWebCtrl->m_lType == zGRID_CONTROL )
               {
                  if ( pchListCtrlTag[ 0 ] == 0 )
                  {
                     strcpy_s( pchListCtrlTag, zTAG_LTH, pWebCtrl->m_csTag );
                     *plHeight = pWebCtrl->m_rect.Height( );
                     *plWidth = pWebCtrl->m_rect.Width( );
                     return( 1 );
                  }

                  if ( zstrcmp( pchListCtrlTag, pWebCtrl->m_csTag.GetString() ) == 0 )
                     pchListCtrlTag[ 0 ] = 0;
               }
            }
         }
      }
   }

   pchListCtrlTag[ 0 ] = 0;
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
GetNextWebCtrlOffset( zVIEW vSubtask, zCPCHAR cpcListCtrlTag,
                      zPCHAR pchFloatCtrlTag, zSHORT bVertical )
{
   TZPainterBar    *pPainterBar;
   zVIEW           vTZPNTROO;
   zULONG          ulBlobMax;

   GetViewByName( &vTZPNTROO, szlTZPNTROO, vSubtask, zLEVEL_TASK );
   if ( vTZPNTROO )
   {
      ZNameItem *m_pName;

      ulBlobMax = sizeof( zPVOID );
      GetBlobFromAttribute( &pPainterBar, &ulBlobMax, vTZPNTROO,
                            szlPalette, szlPointer );
      if ( pPainterBar && pPainterBar->m_pZMIXWebCtrl )
      {
         TZWebCtrl *pWebCtrl;
         zLONG     lCnt = pPainterBar->m_pZMIXWebCtrl->GetCount( );
         zLONG     k;

         for ( k = 0; k < lCnt; k++ )
         {
            if ( *(pPainterBar->m_pZMIXWebCtrl->m_pvNode + k) )
            {
               pWebCtrl = (TZWebCtrl *) *(pPainterBar->m_pZMIXWebCtrl->m_pvNode + k);
               if ( pchFloatCtrlTag[ 0 ] )
               {
                  if ( zstrcmp( pchFloatCtrlTag, pWebCtrl->m_csTag.GetString() ) == 0 )
                     pchFloatCtrlTag[ 0 ] = 0;
               }
               else
               {
                  if ( bVertical )
                     m_pName = pWebCtrl->m_pTop;
                  else
                     m_pName = pWebCtrl->m_pLeft;
#if 1
                  if ( m_pName && zstrcmp( cpcListCtrlTag, m_pName->m_csName.GetString() ) == 0 )
                  {
                     strcpy_s( pchFloatCtrlTag, zTAG_LTH, pWebCtrl->m_csTag );
                     if ( bVertical )
                        return( pWebCtrl->m_rect.top );
                     else
                        return( pWebCtrl->m_rect.left );
                  }
#else
                  while ( m_pName )
                  {
                     if ( zstrcmp( cpcListCtrlTag, m_pName->m_csName ) == 0 )
                     {
                        strcpy_s( pchFloatCtrlTag, zTAG_LTH, pWebCtrl->m_csTag );
                        if ( bVertical )
                           return( pWebCtrl->m_rect.top );
                        else
                           return( pWebCtrl->m_rect.left );
                     }

                     m_pName = m_pName->m_pNextName;
                  }
#endif
               }
            }
         }
      }
   }

   pchFloatCtrlTag[ 0 ] = 0;
   return( 0 );
}
