/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctltab.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Tab (NoteBook) control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.05.14    DKS
//    SetZCtrlProperty implemented to permit text change of a Tab.
//
// 2002.02.21    FH
//    TAB_SetActiveTab: only change the tab if we're not on the right tab (for LRP)
//
// 2001.09.24    DKS   Bianca
//    Fixed timing of NoteBook Ctrl Mapping to fix bug introduced by
//    previous fix (on 7/24).
//
// 2001.07.24    DKS    Z10
//    Ctrls within NoteBook are AutoDisabled properly.
//
// 2001.06.12    DKS    Z10
//    Fix to painting of NoteBook with large numbers of EditBoxes.
//
// 2001.05.30    BL     RemoteServer
//    Improve handling for set ActivateTab (Web overwrite m_pZNotePage,
//    we must save the first Page).
//
// 2001.05.17    DKS    RemoteServer
//    Honor ActivateTab request.
//
// 2001.05.14    DKS    RemoteServer
//    Validate NoteBook hWnd prior to calling functions requiring a
//    valid hWnd.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2001.02.12    DKS    HPF
//    Retain SelChange event after the Tab has changed, but after
//    automatic mapping.  Add new SelChange event (after the Tab has
//    changed, but prior to autoematic mapping).
//
// 2001.02.06    DKS    QS9999
//    SelChange event changed to use ProcessImmediateEvent.  Prevent
//    recursive call to TabChange.
//
// 2001.01.29    DKS    Enhance
//    Enhancement to move the SelChange event after the Tab has changed,
//    but prior to automatic mapping (with correct defines for Rosi).
//
// 2001.01.16    DKS    HPF
//    Minimize impact of enhancement to permit setting of tab colors by
//    using OWNERDRAW only when colors have been specified.
//
// 2001.01.15    DKS    HPF
//    Enhancement for Helmut to permit setting of tab colors.
//
// 2000.08.23    DKS    Z10
//    Any negative return code from SelChanging event prevents the tab
//    from changing.
//
// 2000.06.12    DKS    Z10    R53065
//    Use NLS text on tab captions.
//
// 2000.03.17    TMV    WEB
//    Validate parent window's handle prior to call Create/CreateEx.
//
// 1999.06.29    DKS    Z10    QS999
//    Added pre-change tab event.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1998.10.29    DKS
//    Error on ctrl within tab now displayed and gets focus properly.
//
// 1998.10.20    DKS
//    Radio buttons on a tab control handle TAB and ARROW keys properly,
//    as well as "quick keys" (XC 278).
//
// 1998.08.31    DKS
//    Stop a SelChange when a mapping error has occurred.

// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlTab.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

#define zTAB_CHECKTAB_PRE_LEAVE     2
#define zTAB_CHANGETAB_PREMAP       3
#define zTAB_CHANGETAB_POSTMAP      1

#define zTAB_IN_CHECKTAB            zMAPACT_CTRL_SPECIFIC1
#define zTAB_IN_CHANGETAB           zMAPACT_CTRL_SPECIFIC2

IMPLEMENT_DYNAMIC( ZNoteBook, CTabCtrl )

BEGIN_MESSAGE_MAP( ZNoteBook, CTabCtrl )
   ON_WM_CREATE( )
   ON_NOTIFY_REFLECT( TCN_SELCHANGE, SelChange )
   ON_NOTIFY_REFLECT( TCN_SELCHANGING, SelChanging )
// ON_NOTIFY_REFLECT( TCN_KEYDOWN, KeyDown )
// ON_WM_KEYDOWN( )
// ON_WM_GETDLGCODE( )
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   ON_WM_MOUSEMOVE( )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

// ZNoteBook - ctor
ZNoteBook::ZNoteBook( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
           CTabCtrl( ),
           ZMapAct( pZSubtask,           // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "NoteBook" )
{
#ifdef DEBUG_ALL
   TraceLineI( "In ctor for ZNoteBook", (zLONG) this );
#endif

// ::MessageBox( 0, "ZNoteBook - ctor", "ZNoteBook", MB_OK );
   if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( !(pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
      Attr.Style |= WS_BORDER;

   Attr.Style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;

   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
// m_pzmaLosingFocus = 0;

   m_pZNotePage = 0;
   m_pZFirstPage = 0;
   m_nPageCnt = 0;
   m_lTabStyle = 0;

   if ( pCtrlDef->CtrlBOI == 0 )    // there is no CtrlBOI
      return;

   zCHAR   szBlob[ 5 * sizeof( zLONG ) ];
   zULONG  ulLth = sizeof( szBlob );
   GetBlobFromAttribute( szBlob, &ulLth, vDialog, "Ctrl", "CtrlBOI" );
   m_nPageCnt  = (zSHORT) *((zPLONG) (szBlob + sizeof( zLONG )));
   m_lTabStyle = *((zPLONG) (szBlob + sizeof( zLONG ) * 2));

   m_clrSelectedText = 0;
   m_clrSelectedTab = 0;
   m_pBrushSelectedTab = 0;

   if ( ulLth == sizeof( szBlob ) )
   {
      m_clrSelectedText = *((COLORREF *) (szBlob + sizeof( zLONG ) * 3));
      m_clrSelectedTab = *((COLORREF *) (szBlob + sizeof( zLONG ) * 4));
      m_clrSelectedText &= 0x00FFFFFF;
      m_clrSelectedTab &= 0x00FFFFFF;

      if ( m_clrSelectedTab )
         m_pBrushSelectedTab = new CBrush( m_clrSelectedTab );
   }

// mDeleteInit( m_pBrush );
   if ( m_pBrush == 0 )
      m_pBrush = new CBrush( ::GetSysColor( COLOR_BTNFACE ) );

   if ( m_lTabStyle & 0x00000001 )
      Attr.Style |= TCS_OWNERDRAWFIXED;

   // Always create a tab control and immediate children (sub-tab controls).
   CreateZ( );

   zVIEW vSubobject;

   CreateViewFromViewForTask( &vSubobject, vDialog, m_pZSubtask->m_vDialog );
   SetViewToSubobject( vSubobject, szlCtrlCtrl );
   CreateZeidonCtrls( vSubobject, pZSubtask,
                      this, this,
                      (zSHORT) pCtrlDef->PSDLG_X,
                      (zSHORT) pCtrlDef->PSDLG_Y, TRUE );
   DropView( vSubobject );

// TraceLineI( "ZNoteBook::ctor m_nPageCnt: ", m_nPageCnt );
// TraceLineI( "ZNoteBook::ctor ItemCount: ", GetItemCount( ) );
   if ( m_pZNotePage )
   {
#ifdef zREMOTE_SERVER
      m_pZNotePage->MapChildren( FALSE, FALSE );
#else
      SetCurSel( 0 );

      while ( m_lTabStyle )  // purist's goto
      {
         ZNotePage *pNotePage;
         CRect rect;
         CRect rectClient;
         zLONG lStyleOff;
         zLONG lStyleOn;

         GetClientRect( rectClient );

         // Values for m_lTabStyle (high order first):
         //   4th digit - 0 ==> TCS_RAGGEDRIGHT
         //               1 ==> turn off TCS_RAGGEDRIGHT
         //   5th digit - 0 ==> TCS_SINGLELINE
         //               1 ==> TCS_MULTILINE
         //   6th digit - 0 ==> TCS_TOP
         //               1 ==> TCS_LEFT
         //               2 ==> TCS_BOTTOM
         //               4 ==> TCS_RIGHT
         //   7th digit - 0 ==> TCS_HORIZONTAL
         //               1 ==> TCS_VERTICAL
         //   8th digit - 1 ==> TCS_OWNERDRAWFIXED
         //               0 ==> turn off TCS_OWNERDRAWFIXED
         //
         if ( m_lTabStyle & 0x00010000 )
         {
            lStyleOff = TCS_RAGGEDRIGHT;
            lStyleOn = 0;
         }
         else
         {
            lStyleOff = 0;
            lStyleOn = 0; // TCS_RIGHTJUSTIFY;
         }

         if ( m_lTabStyle & 0x00001000 )
            lStyleOn |= TCS_MULTILINE;

         if ( m_lTabStyle & 0x00000200 )
            lStyleOn |= TCS_BOTTOM;
         else
         if ( m_lTabStyle & 0x00000400 )
            lStyleOn |= TCS_RIGHT;

         if ( m_lTabStyle & 0x00000010 )
            lStyleOn |= TCS_VERTICAL;

         if ( m_lTabStyle & 0x00000001 )
            lStyleOn |= TCS_OWNERDRAWFIXED;
         else
            lStyleOff |= TCS_OWNERDRAWFIXED;

         m_lTabStyle &= 0x0000FFF0;
         if ( m_lTabStyle == 0x00001410 )
         {
            ModifyStyle( lStyleOff, lStyleOn  );
            GetItemRect( m_pZNotePage->m_nTabIdx, rect );
            rectClient.right = rect.left;
         }
         else
         if ( m_lTabStyle == 0x00001200 )
         {
            ModifyStyle( lStyleOff, lStyleOn );
            pNotePage = m_pZNotePage;
            while ( pNotePage )
            {
               GetItemRect( pNotePage->m_nTabIdx, rect );
               if ( rect.top < rectClient.bottom )
                  rectClient.bottom = rect.top;

               pNotePage = pNotePage->m_pNext;
            }
         }
         else
         if ( m_lTabStyle == 0x00001110 )
         {
            ModifyStyle( lStyleOff, lStyleOn );
            pNotePage = m_pZNotePage;
            while ( pNotePage )
            {
               GetItemRect( pNotePage->m_nTabIdx, rect );
               if ( rect.right > rectClient.left )
                  rectClient.left = rect.right;

               pNotePage = pNotePage->m_pNext;
            }
         }
         else
         if ( m_lTabStyle == 0x00000200 )
         {
            ModifyStyle( lStyleOff, lStyleOn );
            GetItemRect( m_pZNotePage->m_nTabIdx, rect );
            rectClient.bottom = rect.top;
         }
         else
         if ( m_lTabStyle == 0x00001000 )
         {
            ModifyStyle( lStyleOff, lStyleOn );
            pNotePage = m_pZNotePage;
            while ( pNotePage )
            {
               GetItemRect( pNotePage->m_nTabIdx, rect );
               if ( rect.bottom > rectClient.top )
                  rectClient.top = rect.bottom;

               pNotePage = pNotePage->m_pNext;
            }
         }
         else
         {
            ModifyStyle( lStyleOff, lStyleOn );
            break;  // out of purist's goto
         }

         pNotePage = m_pZNotePage;
         while ( pNotePage )
         {
            rect.top = rectClient.top + 4;
            rect.left = rectClient.left + 4;
            rect.bottom = rectClient.bottom - 4;
            rect.right = rectClient.right - 4;
            pNotePage->MoveWindow( rect, FALSE );

            pNotePage = pNotePage->m_pNext;
         }

         break;  // out of purist's goto
      }

   // TraceLineI( "ZNoteBook::GetRowCount: ", GetRowCount( ) );
#endif
   }
}

// The following styles can be applied to a tab control:
//
//  TCS_BUTTONS - Modifies the appearance of the tabs to look like buttons.
//  TCS_FIXEDWIDTH - Makes all tabs the same width.  (By default, the tab
//     control automatically sizes each tab to fit its icon.)  You cannot
//     use this style with the TCS_RIGHTJUSTIFY style.
//  TCS_FOCUSNEVER - Specifies that a tab never receives the input focus.
//  TCS_FOCUSONBUTTONDOWN - Specifies that a tab receives the input focus
//     when clicked.  This style is typically used only with the TCS_BUTTONS
//     style.
//  TCS_FORCEICONLEFT - Forces the icon to the left, but leaves the tab
//     label centered.  (By default, the control centers the icon and label
//     with the icon to the left of the label.)
//  TCS_FORCELABELLEFT - Left-aligns both the icon and label.
//  TCS_MULTILINE - Causes a tab control to display multiple rows of tabs.
//     Thus all tabs can be displayed at once.  (By default, a tab control
//     displays a single row of tabs.)
//  TCS_OWNERDRAWFIXED - Specifies that the parent window draws the tabs
//     in the control.
//  TCS_RIGHTJUSTIFY - Right justifies tabs.  (By default, tabs are
//     left-justified within each row.)
//  TCS_SHAREIMAGELISTS - Specifies that a tab control's image lists are
//     not destroyed when the control is destroyed.
//  TCS_TOOLTIPS - Specifies that the tab control has a tool tip control
//     associated with it.
//  TCS_TABS - Tabs appear as tabs, and a border is drawn around the
//     display area.  This style is the default.
//  TCS_SINGLELINE - Displays only one row of tabs. The user can scroll
//     to see more tabs, if necessary.  This style is the default.
//  TCS_RAGGEDRIGHT - Does not stretch each row of tabs to fill the entire
//     width of the control.  This style is the default.
//
// In addition, you can apply the following window styles to a tab control:
//
//  WS_CHILD - Creates a child window that represents the tab control.
//     Cannot be used with the WS_POPUP style.
//  WS_VISIBLE - Creates a tab control that is initially visible.
//  WS_DISABLED - Creates a window that is initially disabled.
//  WS_GROUP - Specifies the first control of a group of controls in
//     which the user can move from one control to the next with the
//     arrow keys.  All controls defined with the WS_GROUP style after
//     the first control belong to the same group.  The next control
//     with the WS_GROUP style ends the style group and starts the
//     next group (that is, one group ends where the next begins).
//  WS_TABSTOP - Specifies one of any number of controls through which
//     the user can move by using the TAB key.  The TAB key moves the
//     user to the next control specified by the WS_TABSTOP style.

void
ZNoteBook::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook::CreateZ ", *m_pzsTag );
#endif

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         Create( Attr.Style,
                 CRect( Attr.X, Attr.Y,
                        Attr.X + Attr.W, Attr.Y + Attr.H ),
     //          CRect( Attr.X - 2, Attr.Y - 2,
     //                 Attr.X + Attr.W + 4, Attr.Y + Attr.H + 4 ),
                 m_pWndParent, m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

int
ZNoteBook::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook::OnCreate ", *m_pzsTag );
#endif
// return( (int) Default( ) );
   if ( CTabCtrl::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   SetFontOverride( );
   return( 0 );
}

BOOL
ZNoteBook::DestroyWindow( )
{
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CTabCtrl::DestroyWindow( ) );
}

zSHORT
ZNoteBook::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook::MapFromOI: ", *m_pzsTag );
#endif

   if ( (wFlag & zMAP_FIRST_TIME) && m_pZNotePage )
   {
      // Map from OI ... don't create the top level.
      m_pZNotePage->MapChildren( FALSE, FALSE );
      if ( m_pZNotePage->m_nHidden == -1 )
         fnEnableChildCtrls( m_pZNotePage, FALSE, TRUE );

#ifdef zREMOTE_SERVER
      SetRemoteZCtrlAttribute( this, "NoteBook", "ActivePage",
                               *(m_pZNotePage->m_pzsTag) );
#else
      if ( mIs_hWnd( m_hWnd ) )
      {
         SetCurSel( m_pZNotePage->m_nTabIdx );

         m_pZNotePage->ShowWindow( SW_SHOW );
         m_pZNotePage->UpdateWindow( );
      }
#endif
   }

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;

   return( -1 );
}

ZNotePage *
ZNoteBook::GetActivePage( )
{
   ZNotePage *pNotePage = m_pZFirstPage;

#ifdef zREMOTE_SERVER
   if ( m_pZNotePage )
   {
#ifdef DEBUG_ALL
      TraceLineI( "ZNoteBook::GetActivePage: ", m_pZNotePage->m_nTabIdx );
#endif
      while ( pNotePage )
      {
         if ( pNotePage->m_nTabIdx == m_pZNotePage->m_nTabIdx )
            break;

         pNotePage = pNotePage->m_pNext;
      }
   }
#else
   if ( mIs_hWnd( m_hWnd ) )
   {
      zSHORT nCurrIdx = GetCurSel( );
#ifdef DEBUG_ALL
      TraceLineI( "ZNoteBook::GetActivePage: ", nCurrIdx );
#endif

      while ( pNotePage )
      {
         if ( pNotePage->m_nTabIdx == nCurrIdx )
            break;

         pNotePage = pNotePage->m_pNext;
      }
   }
#endif // #endif zREMOTE_SERVER

   return( pNotePage );
}

ZNotePage *
ZNoteBook::GetPage( zSHORT nPageNbr )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZNoteBook::GetPage: ", nPageNbr );
#endif

   ZNotePage *pNotePage = 0;
   if ( nPageNbr > 0 && nPageNbr <= m_nPageCnt )
   {
      pNotePage = m_pZFirstPage;
      while ( pNotePage )
      {
         if ( pNotePage->m_nTabNbr == nPageNbr )
            break;

         pNotePage = pNotePage->m_pNext;
      }
   }

   return( pNotePage );
}

zSHORT
ZNoteBook::ResetActiveTab( zSHORT nTabToActivate )
{
// ::MessageBox( 0, "ResetActiveTab", "ZNoteBook", MB_OK );
   if ( nTabToActivate > 0 && nTabToActivate <= m_nPageCnt )
   {
#ifdef DEBUG_ALL
      TraceLineI( "ResetActiveTab nTabToActivate = ", nTabToActivate );
#endif

      ZNotePage *pNotePage;

      if ( (m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI) == 0 )
      {
         pNotePage = GetPage( nTabToActivate );
         if ( pNotePage )
         {
            m_pZNotePage = pNotePage;
         // m_pZFirstPage = pNotePage;
         }

         return( 0 );
      }

      pNotePage = GetActivePage( );
      if ( pNotePage )
      {
#ifdef DEBUG_ALL
         TraceLineI( "ResetActiveTab found: ", nTabToActivate );
#endif
         // Map to OI ... don't destroy top level.
         if ( pNotePage->MapChildren( TRUE, FALSE ) )
         {
         // TraceLineI( "ResetActiveTab Invalidate: ", nTabToActivate );
         // Invalidate( );
            return( -1 );
         }

         if ( mIs_hWnd( m_hWnd ) )
            pNotePage->ShowWindow( SW_HIDE );
      }
      else
      {
#ifdef DEBUG_ALL
         TraceLineI( "ResetActiveTab could not locate Active Tab",
                     nTabToActivate );
#endif
      }

      pNotePage = GetPage( nTabToActivate );
      if ( pNotePage )
      {
#ifdef DEBUG_ALL
         TraceLineI( "ResetActiveTab found nTabToActivate = ", nTabToActivate );
#endif
         // Map from OI ... don't create top level.
         pNotePage->MapChildren( FALSE, FALSE );
         if ( pNotePage->m_nHidden == -1 )
            fnEnableChildCtrls( pNotePage, FALSE, TRUE );

#ifdef zREMOTE_SERVER
         m_pZNotePage = pNotePage;
         SetRemoteZCtrlAttribute( this, "NoteBook", "ActivePage",
                                  *(pNotePage->m_pzsTag) );
#else
         if ( mIs_hWnd( m_hWnd ) )
         {
            SetCurSel( pNotePage->m_nTabIdx );

            pNotePage->ShowWindow( SW_SHOW );
            pNotePage->UpdateWindow( );
         }
#endif
         return( 0 );
      }
   }

#ifdef DEBUG_ALL
   TraceLineI( "ResetActiveTab did not find nTabToActivate = ",
               nTabToActivate );
#endif

   return( -1 );
}

#if 0
void
ListTabs( ZNoteBook *pNoteBook, zCPCHAR cpcText )
{
   TraceLine( "%s ListTabs for: %s   ActiveTab: %d",
              cpcText, *(pNoteBook->m_pzsTag), pNoteBook->GetCurSel( ) );
   ZNotePage *pNotePage = pNoteBook->m_pZFirstPage;
   while ( pNotePage )
   {
      TraceLine( "   NotePage (%d): %s.%s %s", pNotePage->m_nTabIdx,
                 *(pNotePage->m_pzsTag), *(pNotePage->m_pzsText),
                 pNotePage->m_nHidden < 0 ? "ReadOnly" : pNotePage->m_nHidden > 0 ? "Hidden" : "Visible" );

      pNotePage = pNotePage->m_pNext;
   }
}
#endif

zSHORT
ZNoteBook::HideTab( zSHORT nTabToHide,
                    zSHORT nHide ) //  1 - HIDE the specified notepage
                                   //  0 - SHOW the specified notepage
                                   // -1 - SHOW the specified notepage but
                                   //      disable the child ctrls
{
   if ( (m_ulMapActFlags & (zTAB_IN_CHECKTAB | zTAB_IN_CHANGETAB)) == 0 &&
        nTabToHide > 0 && nTabToHide <= m_nPageCnt )
   {
#ifdef DEBUG_ALL
      TraceLineI( "ZNoteBook::HideTab: ", nTabToHide );
#endif
      ZNotePage *pCurrNotePage = GetPage( nTabToHide );
      if ( pCurrNotePage && pCurrNotePage->m_nHidden == nHide )
         return( 0 );

      if ( pCurrNotePage == 0 )
      {
#ifdef DEBUG_ALL
         TraceLineI( "HideTab did not find nTabToHide = ", nTabToHide );
#endif
         return( -1 );  // we didn't find the one we were looking for
      }

#ifdef DEBUG_ALL
      TraceLineI( "HideTab found nTabToHide = ", nTabToHide );
#endif

      ZNotePage *pNotePage;
      zSHORT    nIdx;

#ifdef zREMOTE_SERVER
      zSHORT    nCurrIdx = m_pZNotePage->m_nTabIdx;
#else
      zSHORT    nCurrIdx = GetCurSel( );  // zero-based index of the selected
                                          // tab if successful or -1 if no
                                          // tab is selected.
#endif

      if ( nHide == 1 )  // HIDE the specified tab
      {
         // The current tab is the one to be hidden.
         if ( nCurrIdx == pCurrNotePage->m_nTabIdx )
         {
            // Map to OI ... don't destroy top level.
            if ( pCurrNotePage->MapChildren( TRUE, FALSE ) )
            {
               return( -1 );  // mapping error prevents hide
            }

            if ( mIs_hWnd( pCurrNotePage->m_hWnd ) )
            {
               TraceLine( "HideTab (Valid window) hiding: (%d) %s",
                          pCurrNotePage->m_nTabIdx, *(pCurrNotePage->m_pzsTag) );
               pCurrNotePage->ShowWindow( SW_HIDE );
            }
            else
            {
               TraceLine( "HideTab (Invalid window) hiding: (%d) %s",
                          pCurrNotePage->m_nTabIdx, *(pCurrNotePage->m_pzsTag) );
            }
         }

         // Maintain proper index for each page.
         nIdx = pCurrNotePage->m_nTabIdx;
         pNotePage = pCurrNotePage->m_pNext;
         while ( pNotePage )
         {
            if ( pNotePage->m_nHidden <= 0 )
               pNotePage->m_nTabIdx = nIdx++;

            pNotePage = pNotePage->m_pNext;
         }

         nIdx = pCurrNotePage->m_nTabIdx;
         pCurrNotePage->m_nHidden = 1;
         pCurrNotePage->m_nTabIdx = -1;
#ifdef zREMOTE_SERVER
         SetRemoteZCtrlAttribute( pCurrNotePage, "NotePage", "Hidden", "Y" );
#else
         if ( mIs_hWnd( m_hWnd ) )
            DeleteItem( nIdx );
#endif

         if ( nCurrIdx == nIdx || nCurrIdx == -1 )  // hiding the active tab
         {
         // ListTabs( this, "Hiding the active tab" );

            // We need to reset which tab is active.
            if ( pCurrNotePage->m_pNext )
               pNotePage = pCurrNotePage->m_pNext;
            else
               pNotePage = m_pZFirstPage;

            while ( pNotePage != pCurrNotePage )
            {
               if ( pNotePage && pNotePage->m_nHidden <= 0 )
               {
                  m_pZNotePage = pNotePage;  // used to determine which page
                                             // to map when this notebook's
                                             // MapFrom/ToOI is called
#ifdef zREMOTE_SERVER
                  SetRemoteZCtrlAttribute( pNotePage, "NotePage", "Hidden", "N" );
                  pNotePage->MapChildren( FALSE, FALSE );
#else
                  if ( mIs_hWnd( pNotePage->m_hWnd ) )
                  {
                     TraceLine( "HideTab (Valid window) setting CurSel to: (%d) %s",
                                pNotePage->m_nTabIdx, *(pNotePage->m_pzsTag) );

                     // Map from OI ... don't create top level.
                     pNotePage->MapChildren( FALSE, FALSE );
                     if ( pNotePage->m_nHidden == -1 )
                        fnEnableChildCtrls( pNotePage, FALSE, TRUE );

                     pNotePage->ShowWindow( SW_SHOW );
                     pNotePage->UpdateWindow( );
                     nIdx = SetCurSel( pNotePage->m_nTabIdx + 1 );
                     nIdx = SetCurSel( pNotePage->m_nTabIdx );
                  }
                  else
                  {
                     TraceLine( "HideTab (Invalid window) setting CurSel to: (%d) %s",
                                pNotePage->m_nTabIdx, *(pNotePage->m_pzsTag) );

                     // Map from OI ... don't create top level.
                     pNotePage->MapChildren( FALSE, FALSE );
                     if ( pNotePage->m_nHidden == -1 )
                        fnEnableChildCtrls( pNotePage, FALSE, TRUE );
                  }
#endif
                  break;
               }

               if ( pNotePage && pNotePage->m_pNext )
               {
                  pNotePage = pNotePage->m_pNext;
               }
               else
               {
                  pNotePage = m_pZFirstPage;
               }
            }
         }
         else
         {
            if ( mIs_hWnd( m_hWnd ) )
            {
               Invalidate( );
               ShowWindow( SW_SHOW );
               UpdateWindow( );
            }
         }
      }
      else
      if ( nHide == -1 ) // disable the specified page's child ctrls
      {
      // ListTabs( this, "Disable the active tab" );

         // Disable all children of this page.
         pCurrNotePage->m_nHidden = -1;
         fnEnableChildCtrls( pCurrNotePage, FALSE, TRUE );
      }
      else     // SHOW the specified tab
      if ( pCurrNotePage->m_nHidden == -1 )
      {
      // ListTabs( this, "Show the active tab" );

         // Enable all children of this page.
         fnEnableChildCtrls( pCurrNotePage, TRUE, TRUE );
         pCurrNotePage->m_nHidden = 0;
      }
      else
      {
         // Locate and map the current active tab.
         pNotePage = GetActivePage( );
         if ( pNotePage )
         {
            // Map to OI ... don't destroy top level.
            if ( pNotePage->MapChildren( TRUE, FALSE ) )
            {
               return( -1 );  // mapping error prevents show of unhidden tab
            }

#ifdef zREMOTE_SERVER
            SetRemoteZCtrlAttribute( pNotePage, "NotePage", "Hidden", "Y" );
#else
            if ( mIs_hWnd( pNotePage->m_hWnd ) )
               pNotePage->ShowWindow( SW_HIDE );
#endif
         }

         // Maintain proper index for each page.
         nIdx = 0;
         pCurrNotePage->m_nHidden = 0;
         pNotePage = m_pZFirstPage;
         while ( pNotePage )
         {
            if ( pNotePage->m_nHidden <= 0 )
               pNotePage->m_nTabIdx = nIdx++;

            pNotePage = pNotePage->m_pNext;
         }

#ifdef zREMOTE_SERVER
         m_pZNotePage = pNotePage;
         SetRemoteZCtrlAttribute( this, "NoteBook", "ActivePage",
                                  *(pNotePage->m_pzsTag) );
         SetRemoteZCtrlAttribute( pCurrNotePage, "NotePage", "Hidden", "N" );
#else
         TC_ITEM TabCtrlItem;
         TabCtrlItem.mask = TCIF_TEXT;
         TabCtrlItem.pszText = pCurrNotePage->m_pchCaption;
#ifdef DEBUG_ALL
         TraceLineS( "Unhiding TabCtrlItem: ", pCurrNotePage->m_pchCaption );
#endif
         InsertItem( pCurrNotePage->m_nTabIdx, &TabCtrlItem );
         SetCurSel( pCurrNotePage->m_nTabIdx );
#endif
         // Map from OI ... don't create top level.
         pCurrNotePage->MapChildren( FALSE, FALSE );
         if ( pCurrNotePage->m_nHidden == -1 )
            fnEnableChildCtrls( pCurrNotePage, FALSE, TRUE );

         pCurrNotePage->ShowWindow( SW_SHOW );
         pCurrNotePage->Invalidate( );
         pCurrNotePage->UpdateWindow( );

#ifdef DEBUG_ALL
         TraceLineI( "END ZNoteBook::HideTab TabCount = ", GetItemCount( ) );
         TraceLineI( "              Current Tab Index = ", GetCurSel( ) );
#endif
      }

      return( 0 );
   }

   return( -1 );
}

zSHORT
ZNoteBook::ActivateTab( zSHORT nTabNbr )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZNoteBook::ActivateTab: ", nTabNbr );
#endif

   if ( (m_ulMapActFlags & (zTAB_IN_CHECKTAB | zTAB_IN_CHANGETAB)) == 0 )
   {
      ZNotePage *pNotePage = GetPage( nTabNbr );
      if ( pNotePage && pNotePage->m_nHidden <= 0 )
      {
         return( ResetActiveTab( nTabNbr ) );
      }
   }

   return( -1 );
}

void
ZNoteBook::RepositionTabDlgs( )
{
#if 0
   zSHORT nLeft = GetClientLeft( );
   zSHORT nTop = GetClientTop( );
   zSHORT nWidth = GetClientWidth( );
   zSHORT nHeight = GetClientHeight( );

   ZNotePage *pNotePage = m_pZFirstPage;
   while ( pNotePage )
   {
      pNotePage->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );
      pNotePage = pNotePage->m_pNext;
   }
#endif
}

void
ZNoteBook::ParentResize( ZMapAct *pParent,
                         CSize   *pCSize )
{
   ZMapAct::ParentResize( pParent, pCSize ); // default processing
   RepositionTabDlgs( );
}

zBOOL
ZNoteBook::InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook InformCompositeOfSetFocus pzma = ",
               *(pzmaReceivingFocus->m_pzsTag) );
#endif
   return( TRUE );
}

zBOOL
ZNoteBook::InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook InformCompositeOfKillFocus pzma = ",
               *(pzmaLosingFocus->m_pzsTag) );
#endif
   return( TRUE );
}

#if 0
UINT
ZNoteBook::OnGetDlgCode( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook::OnGetDlgCode", "WANTARROWS and WANTTAB" );
#endif
   return( DLGC_WANTARROWS | DLGC_WANTTAB );
}
#endif

void
ZNoteBook::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
// TraceLineS( "ZNoteBook::OnMouseMove Tag = ", *m_pzsTag );
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

// ZNoteBook - dtor
ZNoteBook::~ZNoteBook( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook::~ZNoteBook", "" );
#endif

   mDeleteInit( m_pBrushSelectedTab );
   DestroyWindow( );
}

void
ZNoteBook::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook::OnSetFocus", "" );
#endif

#if 0
   ZNotePage *pNotePage = GetActivePage( );
   zBOOL bTabKey = (GetKeyState( VK_TAB ) < 0);
   if ( bTabKey && pNotePage )
   {
      ZMapAct *pzma;
      zBOOL   bCtrlKey = (GetKeyState( VK_CONTROL ) < 0);
      zBOOL   bShiftKey = (GetKeyState( VK_SHIFT ) < 0);

      if ( pNotePage->IsAncestor( m_pZSubtask->m_pzmaWithFocus ) )
      {
         pzma = pNotePage;
      }
      else
      {
         pzma = m_pZSubtask->GetFocusCtrlWithinComposite( pNotePage, 0,
                                                          !bShiftKey );
      }

      if ( pzma && pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) &&
           pNotePage->IsAncestor( pzma ) )
      {
         pzma->m_pCtrl->SetFocus( );
         return;
      }
   }
#endif

// if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   Default( );              // pass it along to parent
   m_pZSubtask->InformDriverOfSetFocus( this );
}

void
ZNoteBook::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNoteBook::OnKillFocus", "" );
#endif
   Default( );              // pass it along to parent
   m_pZSubtask->InformDriverOfKillFocus( this );
}

void
ZNoteBook::SelChanging( NMHDR *pNMHDR, LRESULT *pResult )
{
// TraceLineS( "ZNoteBook::SelChanging ", "============>" );
   zSHORT nCurrIdx = GetCurSel( );
   *pResult = 0;  // default is to let SelChange occur!
   ZNotePage *pNotePage = GetActivePage( );

// while ( pNotePage && pNotePage->m_nHidden > 0 )
//    pNotePage = pNotePage->m_pNext;

   if ( pNotePage == 0 )
      return;

#ifdef DEBUG_ALL
   TraceLine( "ZNoteBook::SelChanging Idx: %d  NotePage Idx: %d  Nbr: %d  IdFrom: %d  Hidden: %s",
              nCurrIdx, pNotePage->m_nTabIdx, pNotePage->m_nTabNbr, pNMHDR->idFrom,
              pNotePage->m_nHidden > 0 ? "TRUE" : "FALSE" );
#endif

   m_ulMapActFlags |= zTAB_IN_CHECKTAB;
   zLONG lRC = ProcessImmediateEvent( pNotePage, zTAB_CHECKTAB_PRE_LEAVE );
   m_ulMapActFlags &= ~zTAB_IN_CHECKTAB;
   if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
   {
      *pResult = 0;
      if ( pNotePage )
      {
         // Map to OI ... don't destroy top level.
         m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr = 0;
         if ( pNotePage->MapChildren( TRUE, FALSE ) )
         {
            if ( m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr )
            {
               ((ZMapAct *) (m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr))->
                                                        m_pCtrl->SetFocus( );
               pNotePage->SendMessage( zZEIDON_POSTED_INVALIDATE );
            }
            else
               pNotePage->PostMessage( zZEIDON_POSTED_INVALIDATE );

            *pResult = 1;  // stop the SelChange from occurring!
            return;
         }

         pNotePage->ShowWindow( SW_HIDE );
      }
   }
   else
      *pResult = 1;  // prevent change!
}

void
ZNoteBook::SelChange( NMHDR *pNMHDR, LRESULT *pResult )
{
   zSHORT nCurrIdx = GetCurSel( );
   ZNotePage *pNotePage = GetActivePage( );

   *pResult = 0;
   if ( pNotePage )
   {
#ifdef DEBUG_ALL
      TraceLine( "ZNoteBook::SelChange Idx: %d  NotePage Idx: %d  Nbr: %d  IdFrom: %d  Hidden: %s",
                 nCurrIdx, pNotePage->m_nTabIdx, pNotePage->m_nTabNbr, pNMHDR->idFrom,
                 pNotePage->m_nHidden > 0 ? "TRUE" : "FALSE" );
#endif
      m_ulMapActFlags |= zTAB_IN_CHANGETAB;
      ProcessImmediateEvent( pNotePage, zTAB_CHANGETAB_PREMAP );
      m_ulMapActFlags &= ~zTAB_IN_CHANGETAB;

      // Map from OI ... don't create top level.
      pNotePage->MapChildren( FALSE, FALSE );
      if ( pNotePage->m_nHidden == -1 )
         fnEnableChildCtrls( pNotePage, FALSE, TRUE );

      ZMapAct *pzma = m_pZSubtask->GetFocusCtrlWithinComposite( pNotePage,
                                                                0, TRUE );
      if ( pzma == 0 || pzma->m_pCtrl == 0 ||
           mIs_hWnd( pzma->m_pCtrl->m_hWnd ) == 0 )
      {
         pzma = this;
      }

      if ( pzma->m_pCtrl && mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
         pzma->m_pCtrl->SetFocus( );

      pNotePage->ShowWindow( SW_SHOW );
      pNotePage->UpdateWindow( );
      ProcessEvent( pNotePage, zTAB_CHANGETAB_POSTMAP );
   }
}

#if 0
// An application should return zero if it processes this message.
void
ZNoteBook::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZNoteBook::OnKeyDown uKey = ", uKey );
#endif
   if ( uKey == VK_ESCAPE )
      return;

   Default( );              // pass it along to parent

   if ( uKey == VK_TAB )
   {
      zBOOL bCtrlKey = (GetKeyState( VK_CONTROL ) < 0);
      zBOOL bShiftKey = (GetKeyState( VK_SHIFT ) < 0);
      ZMapAct *pzmaFocus = 0;

      if ( bCtrlKey )
      {
         if ( GetFocus( ) != this )
         {
            m_pzmaLosingFocus = 0;
            SetFocus( );
         }
         else
         {
            ZNotePage *pNotePage = GetActivePage( );
            if ( pNotePage )
            {
               pzmaFocus = m_pZSubtask->
                 GetFocusCtrlWithinComposite( pNotePage, 0, !bShiftKey );
            }
         }
      }
      else
      {
         pzmaFocus = m_pZSubtask->
            GetFocusCtrlWithinComposite( m_pzmaComposite, this, !bShiftKey );
      }

      if ( pzmaFocus )
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZNoteBook OnKeyDown pzmaFocus ========> ",
                     *(pzmaFocus->m_pzsTag) );
#endif
         m_pzmaLosingFocus = this;
         pzmaFocus->m_pCtrl->SetFocus( );
      }
   }
}
#endif

void
ZNoteBook::OnNcPaint( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( "EdtNcPaint", *m_pzsTag ) == 0 )
   {
      TraceLineS( "ZNoteBook::OnNcPaint: ", *m_pzsTag );
   }
#endif

   CTabCtrl::OnNcPaint( );
}

void
ZNoteBook::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZNoteBook::OnPaint Begin: ", *m_pzsTag );
#endif

   CTabCtrl::OnPaint( );
}

/////////////////////////////////////////////////////////////////////////////
//
// To change the color of text on the selected tab of a property sheet,
// you need to subclass the tab control associated with the property
// sheet and make it owner drawn.
//
// Override the DrawItem method of your class derived from CTabCtrl and
// change the color of text before displaying it.
//
// MORE INFORMATION
//
// Create a new class called CMyTabCtrl deriving from CTabCtrl.  Override
// the DrawItem method for CTabCtrl. Because ClassWizard does not show
// this method, this method needs to be added manually.  The sample code
// below shows how to change the color of the text of the selected tab:
//
/////////////////////////////////////////////////////////////////////////////
void
ZNoteBook::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZNoteBook::DrawItem ", *m_pzsTag );
#endif

   TC_ITEM  tci;
   CDC      *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
   COLORREF clrTxSave = CLR_INVALID;
   COLORREF clrBkSave = CLR_INVALID;
   CRect    rect( lpDrawItemStruct->rcItem );
   zCHAR    szTabText[ 256 ];
   zmemset( szTabText, 0, sizeof( szTabText ) );

   tci.mask = TCIF_TEXT;
   tci.pszText = szTabText;
   tci.cchTextMax = sizeof( szTabText ) - 1;
   GetItem( lpDrawItemStruct->itemID, &tci );

// ZNotePage *pNotePage = GetPage( lpDrawItemStruct->itemID + 1 );

   ZNotePage *pNotePage = m_pZFirstPage;
   zLONG lPage = 0;
   while ( pNotePage )
   {
      if ( pNotePage->m_nHidden <= 0 )
      {
         if ( lPage == (zLONG) lpDrawItemStruct->itemID )
            break;

         lPage++;
      }

      pNotePage = pNotePage->m_pNext;
   }

   if ( pNotePage == 0 )  // if we didn't find the right one ... punt!
      return;

   if ( pNotePage->m_clrText )
   {
   // TraceLineX( "ZNoteBook::Draw Text Color: ", pNotePage->m_clrText );
      clrTxSave = pDC->SetTextColor( pNotePage->m_clrText );
   }

   if ( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
   {
   // TraceLineI( "ZNoteBook::DrawEntire Item #", lpDrawItemStruct->itemID + 1 );
   // pDC->TextOut( rect.left + 5, rect.top + 5, tci.pszText );
   }
// else
   // TraceLineI( "ZNoteBook::Draw Item #", lpDrawItemStruct->itemID + 1 );

// rect.DeflateRect( 4, 4 );
   rect.right -= 2;
   if ( (lpDrawItemStruct->itemState & ODS_SELECTED) &&
        (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)) )
   {
   // TraceLineI( "ZNoteBook::Draw Selected Item #", lpDrawItemStruct->itemID + 1 );
   // CClientDC dc( this );
   // TraceLine( "ZNoteBook::DrawItem drawing Selected SubTab: %s   using color: 0x%08x",
   //            *(pNotePage->m_pzsTag), pNotePage->m_clrText );
      if ( m_pBrushSelectedTab || pNotePage->m_pBrushTab )
      {
         CBrush *pBrush;

         if ( pNotePage->m_pBrushTab )
         {
            pBrush = pNotePage->m_pBrushTab;
            clrBkSave = pDC->SetBkColor( pNotePage->m_clrTab );
         }
         else
         {
            pBrush = m_pBrushSelectedTab;
            clrBkSave = pDC->SetBkColor( m_clrSelectedTab );
         }

         rect.bottom--;
         pDC->FillRect( &rect, pBrush );
         rect.bottom++;
         rect.top = rect.bottom - 2;
         pDC->FillRect( &rect, m_pBrush );
         rect.top = lpDrawItemStruct->rcItem.top;
      }
      else
      {
         clrBkSave = pDC->SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
         pDC->FillRect( &rect, m_pBrush );
      }

      // Set the color of the text of the selected tab.
      if ( pNotePage->m_clrText )
         clrTxSave = pDC->SetTextColor( pNotePage->m_clrText );
      else
      if ( m_clrSelectedText )
         clrTxSave = pDC->SetTextColor( m_clrSelectedText );
   }
   else
   {
   // TraceLineI( "ZNoteBook::Draw UnSel Item #", lpDrawItemStruct->itemID + 1 );
      rect.bottom += 2;
      if ( pNotePage )
      {
      // TraceLineI( "ZNoteBook::Draw Brush Item #", lpDrawItemStruct->itemID + 1 );
      // TraceLine( "ZNoteBook::DrawItem drawing SubTab: %s   using color: 0x%08x",
      //            *(pNotePage->m_pzsTag), pNotePage->m_clrText );
         if ( pNotePage->m_clrText )
         {
         // TraceLineX( "ZNoteBook::Draw Text Color: ", pNotePage->m_clrText );
            clrTxSave = pDC->SetTextColor( pNotePage->m_clrText );
         }

         if ( pNotePage->m_pBrushTab )
         {
         // TraceLineX( "ZNoteBook::Draw BrushColor: ", pNotePage->m_clrTab );
         // CClientDC dc( this );
            clrBkSave = pDC->SetBkColor( pNotePage->m_clrTab );
            pDC->FillRect( &rect, pNotePage->m_pBrushTab );
         }
         else
         {
         // TraceLineS( "ZNoteBook::Draw BrushColor: ", "COLOR_BTNFACE" );
            clrBkSave = pDC->SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
         }
      }
      else
      {
      // TraceLineI( "ZNoteBook::Draw NoFnd Item #", lpDrawItemStruct->itemID + 1 );
      // CClientDC dc( this );
      // rect.bottom -= 4;
      // dc.FillRect( &rect, m_pBrush );
      // rect.bottom += 4;
         clrBkSave = pDC->SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
      }
   }

// TraceLineS( "ZNoteBook::DrawText: ", tci.pszText );
   rect.top += 4;
   pDC->DrawText( tci.pszText, zstrlen( tci.pszText ), rect, DT_CENTER );

   if ( clrTxSave != CLR_INVALID )
      pDC->SetTextColor( clrTxSave );

   if ( clrBkSave != CLR_INVALID )
      pDC->SetBkColor( clrBkSave );
}

#if 0
BOOL
CMyPropertySheet::OnInitDialog( )
{
   BOOL bResult = CPropertySheet::OnInitDialog( );

   /* Subclass the tab control. */
   m_myTabCtrl.SubclassWindow( GetTabControl( )->m_hWnd );

   /* Make it owner drawn. */
   m_myTabCtrl.ModifyStyle( 0, TCS_OWNERDRAWFIXED );

   return( bResult );
}
#endif

BOOL
ZNoteBook::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN )
   {
      if ( fnIsAccelerator( m_pZSubtask, pMsg ) )
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZNoteBook - fnIsAccelerator return ", "TRUE" );
#endif
         return( TRUE );
      }

      ZMapAct *pzma;
      zACCEL_DEF ad;
      ad.uVK = pMsg->wParam;
      ad.fAlt = (unsigned) (GetKeyState( VK_MENU ) < 0);
      ad.fCtrl = (unsigned) (GetKeyState( VK_CONTROL ) < 0);
      ad.fShift = (unsigned) (GetKeyState( VK_SHIFT ) < 0);

      if ( ad.uVK == VK_TAB )
      {
         // NoteBook control TAB order:
         //  a. Page Caption
         //  b. Sub-controls within Page
         // Ctrl-TAB flips to next/prev Page
         //  a. to Page Caption from Page Caption ... or
         //  b. to "first" ctrl within Page from ctrl within Page
         //
#ifdef DEBUG_ALL
         TraceLineS( "ZNoteBook::PreTranslate VK_TAB", "" );
#endif
         ZNotePage *pCurrNotePage = GetActivePage( );
         HWND hWndFocus = ::GetFocus( );
         if ( pCurrNotePage )
         {
            if ( m_pZSubtask->m_pzmaWithFocus &&
                 m_pZSubtask->m_pzmaWithFocus->m_pCtrl &&
                 (hWndFocus == m_pZSubtask->m_pzmaWithFocus->m_pCtrl->m_hWnd ||
                  hWndFocus == m_pZSubtask->m_pzmaWithFocus->m_hWndChild) )
            {
               // Set up pNotePage to be the page that is supposed to
               // have the focus.

               ZNotePage *pNotePage = pCurrNotePage;

               if ( ad.fCtrl )
               {
                  ZNotePage *pNextNotePage = 0;
                  ZNotePage *pPrevNotePage = 0;

                  if ( pCurrNotePage->m_pNext )
                     pNotePage = pCurrNotePage->m_pNext;
                  else
                     pNotePage = m_pZFirstPage;

                  while ( pNotePage != pCurrNotePage )
                  {
                     if ( pNotePage->m_nHidden <= 0 )
                     {
                        if ( pNextNotePage == 0 )
                           pNextNotePage = pNotePage;

                        pPrevNotePage = pNotePage;
                     }

                     if ( pNotePage->m_pNext )
                        pNotePage = pNotePage->m_pNext;
                     else
                        pNotePage = m_pZFirstPage;
                  }

                  if ( ad.fShift )
                     pNotePage = pPrevNotePage;
                  else
                     pNotePage = pNextNotePage;
               }

               ZMapAct *pzmaFirst;
               ZMapAct *pzmaLast;

               pzma = 0;
               if ( pNotePage )  // we are within a page (not on the caption)
               {
                  if ( pNotePage == pCurrNotePage )
                  {
                     if ( m_pZSubtask->m_pZView->CheckSplitterFocus( pMsg ) )
                        return( TRUE );

                     pzma = m_pZSubtask->
                       GetFocusCtrlWithinComposite( pNotePage,
                                                    m_pZSubtask->m_pzmaWithFocus,
                                                    !ad.fShift );
                     if ( m_hWnd != hWndFocus )
                     {
                        pzmaFirst = m_pZSubtask->
                           GetFocusCtrlWithinComposite( pNotePage, 0, TRUE );
                        pzmaLast = m_pZSubtask->
                           GetFocusCtrlWithinComposite( pNotePage, 0, FALSE );
                        if ( (ad.fShift == TRUE && pzma == pzmaLast) ||
                             (ad.fShift == FALSE && pzma == pzmaFirst) )
                        {
                           pzma = 0;  // let normal focus occur
                        }
                     }
                  }
                  else
                  {
                     if ( ResetActiveTab( pNotePage->m_nTabNbr ) == 0 )
                     {
                        if ( m_hWnd != hWndFocus )
                        {
                           pzma = m_pZSubtask->
                                   GetFocusCtrlWithinComposite( pNotePage, 0,
                                                                !ad.fShift );
                           if ( pzma == 0 )
                              pzma = this;
                        }
                        else
                           pzma = this;
                     }
                  }
               }

               if ( pzma && pzma->m_pCtrl &&
                    mIs_hWnd( pzma->m_pCtrl->m_hWnd ) )
               {
                  pzma->m_pCtrl->SetFocus( );
                  return( TRUE );
               }
            }
         }
      }
      else
      if ( ad.uVK == VK_ESCAPE )
         return( TRUE );
      else
      if ( m_pZSubtask->m_pzmaWithFocus &&
           m_pZSubtask->m_pzmaWithFocus->HandleQuickKey( ad ) )
      {
         return( TRUE );
      }
      else
      if ( ad.uVK != VK_MENU && ad.fAlt &&
           (pzma = m_pZSubtask->FindCtrlWithQuickKey( m_pZNotePage,
                                                      (char) ad.uVK )) != 0 &&
           pzma->HandleQuickKey( ad ) )
      {
         return( TRUE );
      }
      else
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZNoteBook - fnIsAccelerator return ", "FALSE" );
#endif
      }
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}

IMPLEMENT_DYNAMIC( ZNotePage, CWnd )

BEGIN_MESSAGE_MAP( ZNotePage, CWnd )
   ON_WM_CREATE( )
// ON_WM_KEYDOWN( )
   ON_WM_MOUSEMOVE( )
   ON_WM_PAINT( )
   ON_MESSAGE( zZEIDON_POSTED_INVALIDATE, OnPostedInvalidate )
END_MESSAGE_MAP( )

// ZNotePage - ctor
ZNotePage::ZNotePage( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
           CWnd( ),
           ZMapAct( pZSubtask,           // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "NotePage" )
{
   // We set the client window as the parent and owner of the dialog. This
   // works fine. From my experience, you can also use HWND_DESKTOP for both.
   // Since I've had luck this way, I'll stay with it.  (from sample code)
   //
   // HWND hwndDlg = WinLoadDlg( hwndClient, hwndClient, pnbp->pfnwpDlg, 0,
   //                            pnbp->idDlg, NULL );

#ifdef DEBUG_ALL
   TraceLineS( "In ctor for ZNotePage", *m_pzsTag );
#endif

   m_pNext = 0;
   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
   m_nHidden = 1;   // just in case this doesn't get off the ground
   m_pZNoteBook = DYNAMIC_DOWNCAST( ZNoteBook, pWndParent );
   if ( m_pZNoteBook == 0 )
   {
      m_pchCaption = 0;
      return;
   }

   m_nHidden = 0;   // tab is initialized to "shown" state

   zLONG lLth = zstrlen(pCtrlDef->Text) + 1;
   m_pchCaption = new char[ lLth ];
   strcpy_s( m_pchCaption, lLth, pCtrlDef->Text );

// m_clrDlg = RGB( 0xC0, 0xC0, 0xC0 );
// m_hBrushDlg = 0;  //? ::CreateSolidBrush( m_clrDlg );

   if ( m_pZNoteBook->m_pZNotePage )
   {
      m_nTabIdx = 1;
      ZNotePage *pNotePage = m_pZNoteBook->m_pZNotePage;
      while ( pNotePage->m_pNext )
      {
         m_nTabIdx++;
         pNotePage = pNotePage->m_pNext;
      }

      pNotePage->m_pNext = this;
   }
   else
   {
      m_nTabIdx = 0;
      m_pZNoteBook->m_pZNotePage = this;
      m_pZNoteBook->m_pZFirstPage = this;
   }

   m_nTabNbr = m_nTabIdx + 1;

   TC_ITEM TabCtrlItem;
   TabCtrlItem.mask = TCIF_TEXT;
   TabCtrlItem.pszText = m_pchCaption;
#ifdef DEBUG_ALL
   TraceLineS( "Adding TabCtrlItem: ", m_pchCaption );
#endif

#ifndef zREMOTE_SERVER
   zCHAR   szBlob[ 2 * sizeof( zLONG ) ];
   zULONG  ulLth = sizeof( szBlob );
   GetBlobFromAttribute( szBlob, &ulLth, vDialog, "Ctrl", "CtrlBOI" );

   m_clrText = 0;
   m_clrTab = 0;
   m_pBrushTab = 0;

   if ( ulLth == sizeof( szBlob ) )
   {
      m_clrText = *((COLORREF *) (szBlob + sizeof( zLONG ) * 0));
      m_clrTab = *((COLORREF *) (szBlob + sizeof( zLONG ) * 1));
      m_clrText &= 0x00FFFFFF;
      m_clrTab &= 0x00FFFFFF;
   // if ( m_clrText || m_clrTab )
   //    m_pZNoteBook->Attr.Style |= TCS_OWNERDRAWFIXED;

      if ( m_clrTab )
         m_pBrushTab = new CBrush( m_clrTab );
   }

// mDeleteInit( m_pBrush );
   if ( m_pBrush == 0 )
      m_pBrush = new CBrush( ::GetSysColor( COLOR_BTNFACE ) );

   m_pZNoteBook->InsertItem( m_nTabIdx, &TabCtrlItem );
   CreateZ( );
#endif

//?SetTitle( m_pchCaption );

   zVIEW vSubobject;

   CreateViewFromViewForTask( &vSubobject, vDialog, m_pZSubtask->m_vDialog );
   SetViewToSubobject( vSubobject, szlCtrlCtrl );
   CreateZeidonCtrls( vSubobject, pZSubtask, this, this,
                      0, 0, TRUE );
//                    0, 2 * g_lDlgUnitCharY, TRUE );
   DropView( vSubobject );
}

void
ZNotePage::CreateZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::CreateZ ", *m_pzsTag );
#endif

#ifdef zREMOTE_SERVER
   m_ulMapActFlags2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) )
   {
      CRect rect;

      m_pWndParent->GetClientRect( rect );
      m_pZNoteBook->AdjustRect( FALSE, &rect );
      if ( CreateEx( 0, "ZeidonCustomCtrl", *m_pzsText, WS_CHILD,
                     rect.left, rect.top,
                     rect.right - rect.left, rect.bottom - rect.top,
                     m_pWndParent->m_hWnd, (HMENU) m_nIdNbr ) )
      {
         // Always create a sub-tab control ... do not create child controls.
         m_ulMapActFlag2 |= zMAPACT_CREATED1;
      }
   }
#endif
}

int
ZNotePage::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::OnCreate ", *m_pzsTag );
#endif

   int nRC = (int) Default( );
   if ( nRC != -1 )
   {
      if ( m_pZSubtask->m_pFontWS )
         SendMessage( WM_SETFONT, (WPARAM)(HFONT) *(m_pZSubtask->m_pFontWS) );
   }

   return( nRC );
}

BOOL
ZNotePage::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::DestroyWindow ", *m_pzsTag );
#endif

   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CWnd::DestroyWindow( ) );
}

zSHORT
ZNotePage::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::MapFromOI ", *m_pzsTag );
#endif

   return( -1 );
}

zSHORT
ZNotePage::PostMapFromOI( WPARAM wFlag )
{
   if ( m_nHidden == -1 )
      fnEnableChildCtrls( this, FALSE, TRUE );

   return( 0 );
}

zLONG
ZNotePage::SetZCtrlProperty( zLONG   lPropertyType,
                             zULONG  ulNumericProperty,
                             zCPCHAR cpcStringProperty )
{
   switch ( lPropertyType )
   {
      case zCONTROL_PROPERTY_STRING_DATA:
      {
         zPCHAR pch;

         ASSERT( cpcStringProperty );
         zLONG lLth = zstrlen(cpcStringProperty) + 1;
         pch = new char[ lLth ];
         strcpy_s( pch, lLth, cpcStringProperty );
         if ( mIs_hWnd( m_pZNoteBook->m_hWnd ) )
         {
            TC_ITEM TabCtrlItem;

            TabCtrlItem.mask = TCIF_TEXT;
            TabCtrlItem.pszText = m_pchCaption;
            m_pZNoteBook->GetItem( m_nTabNbr - 1, &TabCtrlItem );
            mDeleteInitA( m_pchCaption );
            m_pchCaption = pch;
            TabCtrlItem.pszText = m_pchCaption;

            // Set the item in the tab control.
            zBOOL bRC = m_pZNoteBook->SetItem( m_nTabNbr - 1, &TabCtrlItem );
            m_pZNoteBook->Invalidate( );
            return( bRC );
         }

         mDeleteInitA( m_pchCaption );
         m_pchCaption = pch;

         return( -1 );
      }

      case zCONTROL_PROPERTY_INTEGER_DATA:
      {
         m_clrTab = (COLORREF) ulNumericProperty;
         m_clrTab &= 0x00FFFFFF;

         mDeleteInit( m_pBrushTab );
         if ( m_clrTab )
            m_pBrushTab = new CBrush( m_clrTab );

         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            m_pZNoteBook->Invalidate( );
            return( 0 );
         }

         return( -1 );
      }

      case zCONTROL_PROPERTY_COLOR_TEXT:
      {
         m_clrText = (COLORREF) ulNumericProperty;
         m_clrText &= 0x00FFFFFF;

         if ( mIs_hWnd( m_pCtrl->m_hWnd ) )
         {
            m_pZNoteBook->Invalidate( );
            return( 0 );
         }

         return( -1 );
      }
   }

   return( ZMapAct::SetZCtrlProperty( lPropertyType,
                                      ulNumericProperty,
                                      cpcStringProperty ) );
}

void
ZNotePage::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
// TraceLineS( "ZNotePage::OnMouseMove Tag = ", *m_pzsTag );
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
   // TraceLineS( "ZNotePage::OnMouseMove pzma = ", *(pzma->m_pzsTag) );
      m_pZSubtask->InformDriverOfFlyBy( pzma );
   }

   Default( );              // pass it along to parent
}

// ZNotePage - dtor
ZNotePage::~ZNotePage( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::~ZNotePage", "" );
#endif
   mDeleteInitA( m_pchCaption );
   mDeleteInit( m_pBrushTab );
// if ( m_hBrushDlg )
//    ::DeleteObject( m_hBrushDlg );

   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
}

#if 0
HBRUSH
ZNotePage::OnCtlColor( HDC hDC, HWND hWndChild, UINT ctlType )
{
   if ( (ctlType == CTLCOLOR_DLG) || (ctlType == CTLCOLOR_STATIC) )
   {
      ::SetBkColor( hDC, m_clrDlg );
      return( m_hBrushDlg );
   }
   else
      return( TDialog::OnCtlColor( hDC, hWndChild, ctlType ) );
}

void
ZNotePage::OnKeyDown( UINT key, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::OnKeyDown", "" );
#endif
   Default( );
}
#endif

zLONG
ZNotePage::HandleEvent( zLONG lEvent )
{
#ifdef zREMOTE_SERVER
   // The "TabChange" event is the only event we get from the LightDriver.
   // Here we mock the behavior of the Windows Notebook control by permitting
   // the TabChange to be prevented.  Note that the Notebook events are
   // Registered in the PPE so that we get the events whether there is
   // an associated action or not.
   if ( lEvent == 1 )
   {
      if ( m_pZNoteBook->m_pZNotePage )
      {
         // Map to OI ... don't destroy top level.
         m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr = 0;
         if ( m_pZNoteBook->m_pZNotePage->MapChildren( TRUE, FALSE ) )
         {
            if ( m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr )
            {
               ZMapAct *pzma;

               // Set CtrlErrorTag saying the change should not occur.
               pzma = (ZMapAct *) (m_pZSubtask->m_pZMIXCtrl->m_pvNodeErr);
               SetRemoteWndAttribute( m_pZSubtask->m_vDialog,
                                      "CtrlErrorTag", *(pzma->m_pzsTag) );
               return( 1 );
            }
         }
      }

      // Map from OI ... don't create top level.
      m_pZNoteBook->m_pZNotePage = this;  // for Remote Server we are abusing
                                          // the definition of m_pZNotePage!
      MapChildren( FALSE, FALSE );
   }
#endif

   return( 0 );
}

zBOOL
ZNotePage::InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage InformCompositeOfSetFocus pzma = ",
               *(pzmaReceivingFocus->m_pzsTag) );
#endif
#if 0
   zBOOL bCtrlKey = (GetKeyState( VK_CONTROL ) < 0);
   if ( bCtrlKey )
   {
      if ( m_pZNoteBook->m_pzmaLosingFocus )
      {
         m_pZNoteBook->m_pzmaLosingFocus = 0;
      }
      else
      {
         m_pZNoteBook->SetFocus( ); // set focus to the tab and ...
         return( FALSE );      // notify driver that TabDlg doesn't want focus
      }
   }
#endif

   return( TRUE );
}

zBOOL
ZNotePage::InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage InformCompositeOfKillFocus pzma = ",
               *(pzmaLosingFocus->m_pzsTag) );
#endif

#if 0

   fnClearCtrlErr( pzmaLosingFocus, 0, 0 );
   fnPushMsgQ_State( m_pZSubtask->m_vMsgQ, zMSGQ_Tabbing );
   pzmaLosingFocus->MapToOI( zVALIDATE );
   fnPopMsgQ_State( m_pZSubtask->m_vMsgQ );

#endif

   return( TRUE );
}

void
ZNotePage::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::OnSetFocus", "" );
#endif
// if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
// {
      Default( );                // pass it along to parent
//    m_pZNoteBook->m_pzmaLosingFocus = 0;
      m_pZNoteBook->SetFocus( );
// }
}

void
ZNotePage::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::OnKillFocus", *m_pzsTag );
#endif
// m_pZSubtask->InformDriverOfKillFocus( this );
   Default( );                   // pass it along to parent
}

LRESULT
ZNotePage::OnPostedInvalidate( WPARAM wParam, LPARAM lParam )
{
   CRect   rect;

   GetWindowRect( rect );
// TraceRect( "ZNotePage::WindowRect", rect );
   ScreenToClient( rect );

#ifdef DEBUG_ALL
   TraceLineS( "ZNotePage::OnPostedInvalidate", *m_pzsTag );
   TraceRect( "ZNotePage::ClientRect", rect );
#endif

   RedrawWindow( rect, 0,
                 RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN );
// Invalidate( );
   return( 1 );
}

void
ZNotePage::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZNotePage::OnPaint: ", *m_pzsTag );
#endif

   if ( m_ulMapActFlags & zMAPACT_PAINT_IN_PROGRESS )
   {
      CWnd::OnPaint( );
      return;
   }

   m_ulMapActFlags |= zMAPACT_PAINT_IN_PROGRESS;

   CRect rect;
   BOOL  bUpdate = GetUpdateRect( rect, 1 );

#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( "CalendarPage", *m_pzsTag ) == 0 )
   {
      CRect rectWindow;

      TraceLineS( "ZNotePage::OnPaint: ", *m_pzsTag );
      TraceRect( "ZNotePage::OnPaint UpdateRect", rect );
      GetWindowRect( rectWindow );
      TraceRect( "ZNotePage::OnPaint WindowRect", rectWindow );
      GetClientRect( rectWindow );
      TraceRect( "ZNotePage::OnPaint ClientRect", rectWindow );
   }
#endif

   if ( bUpdate )
   {
   // TraceRect( "ZNotePage::OnPaint Update:", rect );
      CClientDC dc( this );
      dc.FillRect( &rect, m_pBrush );
      CRect rectClient( rect );
      ClientToScreen( rect );
      CRect rectScreen;
      GetWindowRect( rectScreen );  // screen coords

   // TraceRect( "ZNotePage::OnPaint Screen:", rectScreen );
   // TraceRect( "ZNotePage::OnPaint Screen2:", rect );

      // If this next line is uncommented, a notebook control will show more
      // sub-controls, but when moved off screen and then back, the repainting
      // does not occur properly ... which is the lesser of two evils?
      if ( rectScreen.EqualRect( rect ) )
         ValidateRect( rectClient ); // conditional validate ... seems to fix
                                     // both problems above.   dks 2005.01.11
      CWnd::OnPaint( );
   // PumpMessages( m_pZSubtask->m_vDialog ); too much!!!
      zLONG lPeekCnt = 0;
      MSG   msg;
      while ( lPeekCnt < 256 &&
              ::PeekMessage( &msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE ) )
      {
         lPeekCnt++;
         ::TranslateMessage( &msg );
         ::DispatchMessage( &msg );
      }

   // TraceLineI( "NotePage PeekCnt: ", lPeekCnt );
   // if ( zstrcmp( "NoteNcPaint", *m_pzsTag ) == 0 )
   //    TraceRect( "ZNotePage::OnPaint ClientToScreenRect", rect );

      if ( lPeekCnt < 256 && m_pZSubtask->m_pZMIXCtrl )
      {
         CRect   rectCtrl;
         CRect   rectTemp;
         ZMapAct *pzma;
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
                  if ( rectTemp.IntersectRect( rectCtrl, rect ) )
                  {
                  //xScreenToClient( &rectCtrl );
                  // rectCtrl.InflateRect( 4, 4 );
                  //xInvalidateRect( rectCtrl, TRUE );

                  // TraceLineS( "ZNotePage::OnPaint invalidating: ",
                  //             *(pzma->m_pzsTag) );
                     pzma->m_pCtrl->Invalidate( TRUE );
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
   // TraceLineS( "ZNotePage::OnPaint: ", *m_pzsTag );
      CWnd::OnPaint( );
   }

   m_ulMapActFlags &= ~zMAPACT_PAINT_IN_PROGRESS;
}

#if 0
BOOL
ZNotePage::PreTranslateMessage( MSG *pMsg )
{
   if ( pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN )
   {
      if ( fnIsAccelerator( m_pZSubtask, pMsg ) )
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZNotePage - fnIsAccelerator return ", "TRUE" );
#endif
         return( TRUE );
      }
      else
      if ( ad.uVK == VK_TAB )
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZNotePage::PreTranslate VK_TAB", "" );
#endif
#if 0
         ZNotePage *pNotePage = m_pZNoteBook->GetActivePage( );
         if ( pNotePage )
         {
            ZMapAct *pzma;
            zBOOL   bCtrlKey = (GetKeyState( VK_CONTROL ) < 0);
            zBOOL   bShiftKey = (GetKeyState( VK_SHIFT ) < 0);

            if ( pNotePage->IsAncestor( m_pZSubtask->m_pzmaWithFocus ) )
            {
               pzma = pNotePage;
            }
            else
            {
               pzma = m_pZSubtask->GetFocusCtrlWithinComposite( pNotePage, 0,
                                                                !bShiftKey );
            }

            if ( pzma && pNotePage->IsAncestor( pzma ) )
            {
               pzma->m_pCtrl->SetFocus( );
               return;
            }
         }
#endif
      }
      else
      if ( ad.uVK == VK_ESCAPE )
         return( TRUE );
      else
      {
#ifdef DEBUG_ALL
         TraceLineS( "ZNotePage - fnIsAccelerator return ", "FALSE" );
#endif
      }
   }

   return( CWnd::PreTranslateMessage( pMsg ) );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
Tab( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
 return( new ZNoteBook( pZSubtask, pWndParent,
                        pzmaComposite, vDialog,
                        nOffsetX, nOffsetY, pCtrlDef ) );
}

AFX_EXT_API
CWnd * OPERATION
TabDlg( ZSubtask *pZSubtask,
        CWnd     *pWndParent,
        ZMapAct  *pzmaComposite,
        zVIEW    vDialog,
        zSHORT   nOffsetX,
        zSHORT   nOffsetY,
        zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZNotePage( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=TAB_GetActiveTab
// Source Module=zdctltab.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TAB_GetActiveTab
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To set the currently active tab (page) within a tab
//             (notebook) control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The tag of the tab control
//
//  RETURNS:    -1  - Error locating window or tab control.
//               0  - Error locating sub-tab.
//             > 0  - The active sub-tab (page) number within the tab
//                    (notebook) control.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
TAB_GetActiveTab( zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZNoteBook *pTab = DYNAMIC_DOWNCAST( ZNoteBook, pzma->m_pCtrl );
      if ( pTab )
      {
         ZNotePage *pNotePage = pTab->GetActivePage( );
         if ( pNotePage )
            return( pNotePage->m_nTabIdx + 1 );
         else
            return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for TAB_GetActiveTab ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TAB_GetActiveSubTab
// Source Module=zdctltab.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TAB_GetActiveSubTab
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To get the currently active tab (page) within a tab
//             (notebook) control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The tag of the tab control
//              pchSubCtrlTag - The sub-tab (page) number within the tab
//                              (notebook) control that is active.
//
//  RETURNS:    -1  - Error locating window or tab control.
//               0  - Error locating sub-tab.
//             > 0  - The active sub-tab (page) number within the tab
//                    (notebook) control.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
TAB_GetActiveSubTab( zVIEW    vSubtask,
                     zCPCHAR  cpcCtrlTag,
                     zPCHAR   pchSubCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZNoteBook *pTab = DYNAMIC_DOWNCAST( ZNoteBook, pzma->m_pCtrl );
      if ( pTab )
      {
         ZNotePage *pNotePage = pTab->GetActivePage( );
         if ( pNotePage )
         {
            strcpy_s( pchSubCtrlTag, zTAG_LTH, *(pNotePage->m_pzsTag) );
            return( pNotePage->m_nTabIdx + 1 );
         }
         else
            return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for TAB_GetActiveSubTab ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TAB_SetActiveTab
// Source Module=zdctltab.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TAB_SetActiveTab
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To set the currently active tab (page) within a tab
//             (notebook) control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The tag of the tab control
//              nTabToActivate - The sub-tab (page) number within the tab
//                               (notebook) control to make active.
//
//  RETURNS:    -1  - Error locating window or tab control or sub-tab
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
TAB_SetActiveTab( zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag,
                  zSHORT   nTabToActivate )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZNoteBook *pTab = DYNAMIC_DOWNCAST( ZNoteBook, pzma->m_pCtrl );
      if ( pTab )
      {
         ZNotePage *pNotePage = pTab->GetActivePage( );
         if ( pNotePage && pNotePage->m_nTabIdx + 1 == nTabToActivate )
            return( 0 );

         return( pTab->ActivateTab( nTabToActivate ) );
      }

      TraceLineS( "drvr - Invalid control type for TAB_SetActiveTab ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TAB_SetActiveSubTab
// Source Module=zdctltab.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TAB_SetActiveSubTab
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To set the currently active tab (page) within a tab
//             (notebook) control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The tag of the tab control
//              cpcTabTag   - The sub-tab (page) tag within the tab
//                            (notebook) control to make active.
//
//  RETURNS:    -1  - Error locating window or tab control or sub-tab
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
TAB_SetActiveSubTab( zVIEW    vSubtask,
                     zCPCHAR  cpcCtrlTag,
                     zCPCHAR  cpcTabToActivate )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZNoteBook *pTab = DYNAMIC_DOWNCAST( ZNoteBook, pzma->m_pCtrl );
      if ( pTab )
      {
         zSHORT nTabToActivate = -1;
         ZNotePage *pNotePage = pTab->m_pZFirstPage;

         while ( pNotePage )
         {
            if ( zstrcmp( (*(pNotePage->m_pzsTag)).GetString(), cpcTabToActivate ) == 0 )
            {
               nTabToActivate = pNotePage->m_nTabNbr;
               break;
            }

            pNotePage = pNotePage->m_pNext;
         }

         pNotePage = pTab->GetActivePage( );
         if ( pNotePage && pNotePage->m_nTabIdx + 1 == nTabToActivate )
            return( 0 );

         return( pTab->ActivateTab( nTabToActivate ) );
      }

      TraceLineS( "drvr - Invalid control type for TAB_SetActiveSubTab ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TAB_SetSubTabColors
// Source Module=zdctltab.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TAB_SetSubTabColors
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To set the currently active tab (page) within a tab
//             (notebook) control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcCtrlTag  - The tag of the tab control
//              cpcTabTag   - The sub-tab (page) tag within the tab
//                            (notebook) control to make active.
//              lClrText    - new notebook tab text color (-1 ==> do not reset)
//              lClrTab     - new notebook tab background color (-1 ==> do not reset)
//
//  RETURNS:    -1  - Error locating window or tab control or sub-tab
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
TAB_SetSubTabColors( zVIEW    vSubtask,
                     zCPCHAR  cpcCtrlTag,
                     zCPCHAR  cpcSubTab,
                     zLONG    lClrText,
                     zLONG    lClrTab )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZNoteBook *pTab = DYNAMIC_DOWNCAST( ZNoteBook, pzma->m_pCtrl );
      if ( pTab )
      {
         zSHORT nTabToActivate = -1;
         ZNotePage *pNotePage = pTab->m_pZFirstPage;

         while ( pNotePage )
         {
            if ( zstrcmp( (*(pNotePage->m_pzsTag)).GetString(), cpcSubTab ) == 0 )
            {
               if ( lClrText != -1 )
                  pNotePage->m_clrText = lClrText;

               if ( lClrTab != -1 )
               {
                  pNotePage->m_clrTab = lClrTab;
                  mDeleteInit( pNotePage->m_pBrushTab );
                  pNotePage->m_pBrushTab = new CBrush( pNotePage->m_clrTab );
                  pTab->Invalidate( );
               }

            // TraceLine( "TAB_SetSubTabColors setting SubTab: %s   text color to: 0x%08x",
            //            *(pNotePage->m_pzsTag), pNotePage->m_clrText );
               break;
            }

            pNotePage = pNotePage->m_pNext;
         }

         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for TAB_SetSubTabColors ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TAB_HideTab
// Source Module=zdctltab.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TAB_HideTab
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To hide or show a sub-tab (page) within a tab
//             (notebook) control.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the control.
//              cpcCtrlTag - The tag of the tab control
//              nTabToHide - The sub-tab (page) number within the tab
//                           (notebook) control to hide or show.
//              nHide      -  1 ==> hide the sub-tab (page).
//                         -  0 ==> show the sub-tab (page).
//                         - -1 ==> set all controls on the sub-tab read-only
//
//  RETURNS:    -1  - Error locating window or tab control or sub-tab
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
TAB_HideTab( zVIEW    vSubtask,
             zCPCHAR  cpcCtrlTag,
             zSHORT   nTabToHide,
             zSHORT   nHide )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZNoteBook *pTab = DYNAMIC_DOWNCAST( ZNoteBook, pzma->m_pCtrl );
      if ( pTab )
         return( pTab->HideTab( nTabToHide, nHide ) );

      TraceLineS( "drvr - Invalid control type for TAB_HideTab ",
                  cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TAB_HideSubTab
// Source Module=zdctltab.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TAB_HideSubTab
//                                              05/01/97 Modified: 05/14/97
//
//  PURPOSE:   To hide or show a sub-tab (page) within a tab
//             (notebook) control.
//
//  PARAMETERS: vSubtask   - The subtask view for the window containing
//                           the control.
//              cpcCtrlTag - The tag of the tab control
//              cpcTabTag  - The sub-tab (page) tag within the tab
//                           (notebook) control to hide or show.
//              nHide      -  1 ==> hide the sub-tab (page).
//                         -  0 ==> show the sub-tab (page).
//                         - -1 ==> set all controls on the sub-tab as readonly
//
//  RETURNS:    -1  - Error locating window or tab control
//               1  - Error locating sub-tab
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
TAB_HideSubTab( zVIEW    vSubtask,
                zCPCHAR  cpcCtrlTag,
                zCPCHAR  cpcTabToHide,
                zSHORT   nHide )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

// nHide /= nHide - nHide;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZNoteBook *pTab = DYNAMIC_DOWNCAST( ZNoteBook, pzma->m_pCtrl );
      if ( pTab )
      {
         ZNotePage *pNotePage = pTab->m_pZFirstPage;
         while ( pNotePage )
         {
            if ( zstrcmp( (*(pNotePage->m_pzsTag)).GetString(), cpcTabToHide ) == 0 )
               return( pTab->HideTab( pNotePage->m_nTabNbr, nHide ) );

            pNotePage = pNotePage->m_pNext;
         }

         return( 1 );
      }

      TraceLineS( "drvr - Invalid control type for TAB_HideSubTab ",
                  cpcCtrlTag );
   }

   return( -1 );
}
