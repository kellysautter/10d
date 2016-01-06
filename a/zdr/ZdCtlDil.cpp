/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctldil.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon StatusBar (DIL) control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.10.02    DKS    Z10
//    Honor right and center justify flags.
//
// 2001.05.23    DKS    Z10
//    Disable MessageBar from Conversational dialogs.
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
#include "ZdCtlDIL.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( ZStatus, CStatusBar )

BEGIN_MESSAGE_MAP( ZStatus, CStatusBar )
   ON_WM_CREATE( )
   ON_MESSAGE( WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////

#if 1
static UINT indicators[] =
{
   ID_SEPARATOR,           // status line indicator
   ID_INDICATOR_CAPS,
   ID_INDICATOR_NUM,
   ID_INDICATOR_SCRL,
};
#endif

// ZStatus - ctor
ZStatus::ZStatus( ZSubtask *pZSubtask,
                  CWnd     *pWndParent,
                  ZMapAct  *pzmaComposite,
                  zVIEW    vDialog,
                  zSHORT   nOffsetX,
                  zSHORT   nOffsetY,
                  zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
         CStatusBar( ),
         ZMapAct( pZSubtask,                       // base class ctor
                  pzmaComposite,
                  pWndParent,
                  this,
                  vDialog,
                  nOffsetX,
                  nOffsetY,
                  pCtrlDef,
                  "DIL" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ZStatus::ctor", (zLONG) this );
#endif

   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
   zPCHAR pch = (zPCHAR) pCtrlDef->CtrlBOI;
   if ( pch == 0 || // there is no CtrlBOI
        (m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_SUBWND_CONVERSATIONAL) )
   {
      m_plSubtype = 0;
      return;
   }

   m_pZSubtask->m_nAutoDIL = (zSHORT) *((zPLONG) pch);
   m_pZSubtask->m_nDIL_Items = (zSHORT) *((zPLONG) (pch + sizeof( zLONG )));
   m_plSubtype = (zLONG *) new char[ m_pZSubtask->m_nDIL_Items * sizeof( zLONG ) ];

   // A status bar will never have a parent other than the client window ...
   // so always create.

#ifdef zREMOTE_SERVER

   return;

#else

   CreateZ( );
   if ( m_hWnd == 0 )
   {
   // TraceLineS( "ZStatus Failed to create status bar", "" );
      return;      // fail to create
   }

   STATUSBAR_ITEM *pStatusItem;
   zLONG  k;
   int    *pint = (int *) new char[ m_pZSubtask->m_nDIL_Items * sizeof( int ) ];

   // Initialize with all ID's set to zero.
   for ( k = 0; k < m_pZSubtask->m_nDIL_Items; k++ )
      pint[ k ] = 0;

   SetIndicators( (UINT *) pint, m_pZSubtask->m_nDIL_Items );

   // Set the information for each pane using"
   // void SetPaneInfo( int nIndex, UINT nID, UINT nStyle, int cxWidth );
   // Parameters:
   //    nIndex   Index of the indicator pane whose style is to be set.
   //    nID      New ID for the indicator pane.
   //    nStyle   New style for the indicator pane.
   //    cxWidth  New width for the indicator pane.
   //
   // Remarks:
   //    Sets the specified indicator pane to a new ID, style, and width.
   //    The following indicator styles are supported:
   //        SBPS_NOBORDERS  No 3-D border around the pane.
   //        SBPS_POPOUT     Reverse border so that text "pops out."
   //        SBPS_DISABLED   Do not draw text.
   //        SBPS_STRETCH    Stretch pane to fill unused space. Only one pane
   //                         per status bar can have this style.
   //        SBPS_NORMAL     No stretch, borders, or pop-out.

   int  pBase = 0;
   UINT uStyle;
   pStatusItem = (STATUSBAR_ITEM *) (pch + 2 * sizeof( zLONG ));
   for ( k = 0; k < m_pZSubtask->m_nDIL_Items; k++ )
   {
      m_plSubtype[ k ] = pStatusItem->lMode;

//    pBase += (pStatusItem->lWidth << 1) / g_lDlgUnitCharX;
      pBase = pStatusItem->lWidth << 1;
      if ( pBase < 0 )
      {
         TraceLine( "ZStatus::ctol resetting status bar item width from: %d ... "
                    "Please resave Dialog: %s", pBase, *(m_pZSubtask->m_pzsDlgTag) );
         pBase = 30;
      }

      if ( k == 0 && m_pZSubtask->m_nAutoDIL == 0 ||
           (m_pZSubtask->m_nAutoDIL && k == m_pZSubtask->m_nAutoDIL - 1) )
      {
         uStyle = SBPS_OWNERDRAW | SBPS_STRETCH | SBPS_POPOUT;
      }
      else
      {
         uStyle = SBPS_OWNERDRAW | SBPS_NORMAL;
      }

      SetPaneInfo( k, k + 1, uStyle, pBase );

//    pint[ k ] = pBase;
//    TraceLineI( "pBase = ", pBase );
      pStatusItem++;
   }

// TraceLineI( "StatusBar part count = ", m_pZSubtask->m_nDIL_Items );
   CStatusBarCtrl& sb = GetStatusBarCtrl( );
#if 0
   sb.SetParts( m_pZSubtask->m_nDIL_Items, pint );
   TraceLineI( "StatusBar part count = ",
               sb.GetParts( m_pZSubtask->m_nDIL_Items, pint ) );
   for ( k = 0; k < m_pZSubtask->m_nDIL_Items; k++ )
   {
      TraceLineI( "pint = ", pint[ k ] );
   }
#endif

   pStatusItem = (STATUSBAR_ITEM *) (pch + 2 * sizeof( zLONG ));
   for ( k = 0; k < m_pZSubtask->m_nDIL_Items; k++ )
   {
      sb.SetText( pch + (zLONG) pStatusItem->pchText, k, 0 );
//    TraceLineS( "Text: ", pch + (zLONG) pStatusItem->pchText );
      pStatusItem++;
   }

   mDeleteInitA( pint );

//
// for ( k = 1; k <= nItems; k++ )
//    m_pZSubtask->SetStatusBarText(
//       "01234567891123456789212345678931234567894123456789"    //  50
//       "51234567896123456789712345678981234567899123456789"    // 100
//       "01234567891123456789212345678931234567894123456789"    // 150
//       "51234567896123456789712345678981234567899123456789",   // 200
//       k );
//

   m_pZSubtask->m_pDIL = this;

#endif   // #ifdef zREMOTE_SERVER
}

void
ZStatus::CreateZ( )
{
// Create( CCS_BOTTOM | Attr.Style,
//         CRect( Attr.X, Attr.Y, Attr.X + Attr.W, Attr.Y + Attr.H ),
//         m_pZSubtask->m_pZFWnd, m_nIdNbr );

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( m_pZSubtask->m_pZFWnd &&
        mIs_hWnd( m_pZSubtask->m_pZFWnd->m_hWnd ) &&
        Create( m_pZSubtask->m_pZFWnd ) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZStatus::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CStatusBar::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

#if 0
// The StatusBar has its own font ... we don't want to change it
// SetFontOverride( );
   Of course, a normal frame window has WS_THICKFRAME and is sizable.
   So the only way to get rid of the pesky size gripper is to derive
   a new class from CStatusBar and override PreCreateWindow to turn
   off the gripper style.
 BOOL CMyStatusBar::PreCreateWindow(
     CREATESTRUCT& cs)
 {
     BOOL bRet = CStatusBar::PreCreateWindow(cs);
     cs.style &= ~SBARS_SIZEGRIP;
     // no gripper!
     return bRet;
 }
#endif

   return( nRC );
}

BOOL
ZStatus::DestroyWindow( )
{
   if ( m_pZSubtask->m_pDIL == this )
   {
      m_pZSubtask->m_pDIL = 0;
      m_pZSubtask->m_nAutoDIL = 0;
   }

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CStatusBar::DestroyWindow( ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// CDlgStatusBar::OnIdleUpdateCmdUI
//    OnIdleUpdateCmdUI handles the WM_IDLEUPDATECMDUI message, which is
//    used to update the status of user-interface elements within the MFC
//    framework.
//
/////////////////////////////////////////////////////////////////////////////
LRESULT
ZStatus::OnIdleUpdateCmdUI( WPARAM wParam, LPARAM lParam )
{
//xif ( mIs_hWndVisible( ) )
//x{
//x   CStatusBarCtrl& sb = GetStatusBarCtrl( );
//x   sb.SetText( "OnIdleUpdateCmdUI", 0, 0 );
//    TraceLineI( "ZStatus::OnIdleUpdateCmdUI ", wParam );
//    OnUpdateCmdUI( m_pZSubtask->m_pZFWnd, (BOOL) wParam );
//x}

   return( 1L );
}

void
ZStatus::DrawItem( LPDRAWITEMSTRUCT pDrawItemStruct )
{
   CDC     *pDC = CDC::FromHandle( pDrawItemStruct->hDC );
   UINT    uFormat;
   CString cs;
   CRect   rect;
   zCHAR   szText[ 512 ];
   zPCHAR  pchText;
   zLONG   lOffset;
   zSHORT  nLth;
   zSHORT  k;

   // Same as label offsets in CHeaderCtrl
   zLONG OFFSET_FIRST = pDC->GetTextExtent( _T(" "), 1 ).cx * 2;
   zLONG OFFSET_OTHER = OFFSET_FIRST;

   int nOldBkMode = pDC->SetBkMode( TRANSPARENT );
   for ( k = 0; k < m_pZSubtask->m_nDIL_Items; k++ )
   {
      GetPaneText( k, cs );
      GetItemRect( k, rect );
      uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER;
      if ( m_plSubtype[ k ] & zCONTROL_RIGHTJUSTIFY )
      {
         lOffset = OFFSET_OTHER;
         rect.right -= lOffset;
         uFormat |= DT_RIGHT;
      }
      else
      if ( m_plSubtype[ k ] & zCONTROL_CENTERJUSTIFY )
      {
         lOffset = 0;
         uFormat |= DT_CENTER;
      }
      else
   // if ( m_plSubtype[ k ] & zCONTROL_LEFTJUSTIFY )
      {
         lOffset = OFFSET_OTHER;
         rect.left += lOffset;
      }

   // TraceLineI( "ZStatus::DrawItem: ", k );
   // TraceRect( cs, rect );
      nLth = cs.GetLength( ) + 1;
      if ( nLth <= sizeof( szText ) )
         pchText = szText;
      else
         pchText = new char[ nLth ];

      MakeShortString( pDC, pchText, nLth, cs, rect.right - rect.left, OFFSET_OTHER );

      pDC->DrawText( pchText, -1, rect, uFormat );

      if ( nLth >= sizeof( szText ) )
         delete [] pchText;
   }

   pDC->SetBkMode( nOldBkMode );
}

void
ZStatus::ParentResize( UINT uSizeType,
                       CSize *pCSize )
{
   BringWindowToTop( );
}

// ZStatus - dtor
ZStatus::~ZStatus( )
{
// TraceLineS( "ZStatus::~ZStatus", "" );
   mDeleteInitA( m_plSubtype );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
DIL( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZStatus( pZSubtask, pWndParent,
                        pzmaComposite, vDialog,
                        nOffsetX, nOffsetY, pCtrlDef ) );
}
