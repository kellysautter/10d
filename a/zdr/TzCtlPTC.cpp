/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlptc.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of common painter control (TZPainterCtrl)
// class.  All painter controls must be derived from this class.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.05.21    DKS   Z10
//    Cloning of a control that has sub-controls that are not "painted"
//    (e.g.  on a Grid) now works.
//
// 2001.06.28    DKS   Z10
//    Fixed painting of ruler.
//
// 2001.06.20    DKS   Z10
//    Added sizable ruler margin.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2000.03.15    DKS   Z10    Enhance
//    Use last selected control as "best" parent when moving control(s).
//
// 1999.07.23    DKS   Z10    QS999
//    Refresh Painter window after dropping a page (tab) from the notebook.
//
// 1999.03.10    DKS
//    Altered the TranslateColor function to remove name conflict.
//
// 1999.02.09    DKS
//    Altered positioning code to account for reports.
//
// 1999.01.27    DKS
//    Consolidated code for deletion of a ctrl in preparation for the
//    implementation of Undo/Redo.
//
// 1999.01.26    DKS
//    Added one more protection against an unexpected invalid window.
//
// 1999.01.11    DKS
//    Fixed Ruler drawing due to rounding correction in dialog unit
//    conversion routines.
//
// 1998.12.29    DKS
//    Removed processing when doing reports.
//
// 1998.12.15    DKS
//    Right mouse button select on a selected control behaves properly.
//
// 1998.12.14    DKS
//    Can now move GroupSets successfully while scrolled.
//
// 1998.10.30    DKS
//    Improved selection algorithim.
//
// 1998.09.22    DKS
//    Right mouse rubber band select within selected control (TB 193)
//
// 1998.08.13    DKS
//    Fix to Tab control acceptance by GroupBox (now warns)
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
#undef CX_BORDER
#undef CY_BORDER
#define CX_BORDER 1
#define CY_BORDER 1

void
PainterCursorListener( zVIEW   pView,
                       zCPCHAR cpcEntityName,
                       zPVOID  pvInstance )
{
   TZPainterCtrl *pCtrl = ((TZPainterCtrl *) pvInstance);
   if ( pCtrl && cpcEntityName == 0 )
      TraceLineS( "Dropping Ctrl View: ", pCtrl->m_csTag );
}

TZPainterCtrl::TZPainterCtrl( TZPainterWindow *pPainterWindow,
                              TZPainterCtrl   *pCtrlParent,
                              CWnd     *pWndParent,
                              CRect&   rect,
                              zVIEW    vCtrl,
                              CWnd     *pWndCtrl,
                              zCPCHAR  cpcTag,
                              zCPCHAR  cpcText,
                              zLONG    lType,
                              zBOOL    bPlaceHolder,
                              zLONG    lZKey,
                              zLONG    lTabStopNbr,
                              zLONG    lCtrlID,
                              zCHAR    chEntity )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::ctor: ", cpcTag );
#endif

   ASSERT( pWndCtrl );

   m_lZKey = lZKey;
   m_lType = lType;
   m_bPlaceHolder = bPlaceHolder;
   if ( cpcTag && *cpcTag )
      m_csTag = cpcTag;
   else
      m_csTag = "?";

   if ( cpcText && *cpcText )
      m_csText = cpcText;  // save the text

   m_pPainterWindow = pPainterWindow;
   m_ulZOrder = m_pPainterWindow->m_ulZOrder++;
   m_pCtrlCover = 0;
   if ( vCtrl )
   {
      CreateViewFromViewForTask( &m_vCtrl, vCtrl,
                                 m_pPainterWindow->m_vDialog );
      SfLockView( m_vCtrl );  // we lock this view because it is
                              // unnamed ... to prevent the OE from
                              // automatically cleaning it up during
                              // the deletion of an OE subtask.
   // AddCursorListener( m_vCtrl, PainterCursorListener, this );

#ifdef DEBUG_ALL
      CString cs = "__";
      cs += pPainterWindow->m_csDlgTag;
      cs += ".";
      cs += pPainterWindow->m_csWndTag;
      cs += ".";
      cs += m_csTag;
      if ( zstrlen( cs ) > 64 )
      {
         cs = "__";
         cs += pPainterWindow->m_csWndTag;
         cs += ".";
         cs += m_csTag;
         if ( zstrlen( cs ) > 64 )
         {
            cs = "__";
            cs += m_csTag;
         }
      }

      SetNameForView( m_vCtrl, cs,
                      m_pPainterWindow->m_pBar->m_vSubtask,
                      zLEVEL_TASK );
#endif
   }
   else
      m_vCtrl = 0;

   m_pWndCtrl = pWndCtrl;
   m_wp.length = 0;

   m_lCtrlID = lCtrlID;
   m_chEntity = chEntity;
// m_csLabel = "";      done by CString's default ctor
   m_chType = 0;

   m_pWndParent = pWndParent;
   m_pCtrlParent = pCtrlParent;
   m_pOrigParent = pCtrlParent;
   if ( pCtrlParent )
   {
      m_uVisible = pCtrlParent->m_uVisible;
      pCtrlParent->m_nChildren++;
   }
   else
   {
      m_uVisible = 0;   // visible by default
   }

   m_nChildren = 0;
   m_rectCtrl = rect;

#ifdef DEBUG_ALL
   TraceRect( "TZPainterCtrl ctor m_rectCtrl:", m_rectCtrl );
#endif

   // Our covering window needs to be a child of the painter client so
   // that it can be used to reposition and resize the control!!!
   ParentToPainterRect( m_rectPainter );

   Attr.X = rect.left;
   Attr.Y = rect.top;
   Attr.W = rect.right - rect.left;
   Attr.H = rect.bottom - rect.top;
   Attr.Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;

   m_nTabStopNbr = (zSHORT) lTabStopNbr;
   m_bCapture = FALSE;

// m_ptMouseSelect      initialized by default ctor
// m_ptLastMouseMove    initialized by default ctor
   m_bShiftKey = FALSE;
   m_bCtrlKey = FALSE;
   m_bBuildOnDemand = FALSE;     // Build sub-controls during initialization
   m_bCtrlCoverSelected = FALSE; // Selected via CtrlCover

   zLONG lColorText = 0;

   if ( m_vCtrl )
      GetIntegerFromAttribute( &m_lSubtype, m_vCtrl, szlControl, szlSubtype );
   else
      m_lSubtype = 0;

#if 0
   zPCHAR pch;

   zSHORT nRC = SetCursorFirstEntity( vCtrl, "CtrlColorDef", 0 );
   if ( nRC > zCURSOR_UNCHANGED )
      nRC = SetCursorFirstEntity( vCtrl, "CtrlColor", 0 );

   while ( nRC > zCURSOR_UNCHANGED )
   {
      GetAddrForAttribute( (zPCHAR *) &pch, vCtrl, "CtrlColorDef", "Tag" );
      if ( zstrcmp( pch, "Text" ) == 0 )
      {
         GetIntegerFromAttribute( &lColorText, vCtrl,
                                  "CtrlColor", "RGB_Color" );
         break;
      }

      nRC = SetCursorNextEntity( vCtrl, "CtrlColorDef", 0 );
      if ( nRC > zCURSOR_UNCHANGED )
         nRC = SetCursorNextEntity( vCtrl, "CtrlColor", 0 );
   }
#endif

   m_clrText = fnTranslateColor( lColorText );
   m_pFont = 0;

   // Add ourselves to the list maintained by the painter.
   m_pPainterWindow->m_CtrlList.
                           SetAtGrow( m_pPainterWindow->m_nCtrlCnt++, this );
// TraceLineI( "TZPainterCtrl::ctor: ", m_pPainterWindow->m_nCtrlCnt );

#if 0
   nRC = SetCursorFirstEntity( vCtrl, "CtrlFontDef", 0 );
   if ( nRC > zCURSOR_UNCHANGED )
      nRC = SetCursorFirstEntity( vCtrl, "CtrlFont", 0 );

   while ( nRC > zCURSOR_UNCHANGED )
   {
      GetAddrForAttribute( (zPCHAR *) &pch, vCtrl, "CtrlFontDef", "Tag" );
      if ( zstrcmp( pch, "Text" ) == 0 )
      {
         zULONG  ulLth;
         GetAttributeLength( &ulLth, vCtrl, "CtrlFont", "WindowsStructure" );
         if ( ulLth <= sizeof( LOGFONT ) )
         {
            LOGFONT  lf;

            GetBlobFromAttribute( &lf, &ulLth, vCtrl,
                                  "CtrlFont", "WindowsStructure" );
            GetIntegerFromAttribute( &lColorText, vCtrl,
                                     "CtrlFont", "RGB_Color" );
            m_pFont = new CFont( &lf );
            m_pWndCtrl->SetWindowFont( m_pFont, FALSE );

            if ( m_clrText == 0 )
            {
               m_clrText = lColorText;
            }
         }

         break;
      }

      nRC = SetCursorNextEntity( vCtrl, "CtrlFontDef", 0 );
      if ( nRC > zCURSOR_UNCHANGED )
         nRC = SetCursorNextEntity( vCtrl, "CtrlFont", 0 );
   }
#endif
}

TZPainterCtrl::~TZPainterCtrl( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl dtor: ", m_csTag );
#endif

   // this should be done ... not necessarily easy to do
   if ( MiValidView( m_vCtrl ) )
   {
      DropView( m_vCtrl );
      m_vCtrl = 0;
   }

   if ( m_pPainterWindow && m_pPainterWindow->m_pCurrCtrl == this )
      m_pPainterWindow->m_pCurrCtrl = 0;

   mDeleteInit( m_pFont );

// m_pWndCtrl->DestroyWindow( );
   Deselect( );
   mDeleteInit( m_pCtrlCover );

   // Remove ourselves from the painter window's list.
   zSHORT nPos = m_pPainterWindow->m_nCtrlCnt;
   while ( nPos-- )
   {
      if ( m_pPainterWindow->m_CtrlList[ nPos ] == this )
      {
//       TraceLineI( "TZPainterCtrl::dtor: ", nPos + 1 );
         m_pPainterWindow->m_nCtrlCnt--;
         m_pPainterWindow->m_CtrlList.RemoveAt( nPos );
         m_pPainterWindow = 0;
         return;
      }
   }

// TraceLineX( "TZPainterCtrl dtor NO detach ", (zLONG) this );
   m_pPainterWindow = 0;
}

//
// METHOD : TZPainterCtrl::Bigger
//
//  PURPOSE: to determine if we are bigger than the control object passed
//
zSHORT
TZPainterCtrl::Bigger( TZPainterCtrl *pCmprCtrl, CRect *rectCmpr )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::Bigger: ", m_csTag );
#endif

   zLONG  lSize1, lSize2;

   lSize1 = m_rectCtrl.right - m_rectCtrl.left;
   lSize1 *= (m_rectCtrl.bottom - m_rectCtrl.top);
   if ( rectCmpr )
   {
      lSize2 = rectCmpr->right - rectCmpr->left;
      lSize2 *= (rectCmpr->bottom - rectCmpr->top);
   }
   else
   {
      lSize2 = pCmprCtrl->m_rectCtrl.right - pCmprCtrl->m_rectCtrl.left;
      lSize2 *= (pCmprCtrl->m_rectCtrl.bottom - pCmprCtrl->m_rectCtrl.top);
   }

   if ( lSize1 > lSize2 )
      return( 1 );
   else
   if ( lSize1 < lSize2 )
      return( -1 );
   else
      return( 0 );
}

//
// METHOD : TZPainterCtrl::FindParentObject
//
//
// PURPOSE : To determine the parent control for a control when
//           a control has been moved
//
TZPainterCtrl *
TZPainterCtrl::FindParentObject( CRect&        rectPainter,
                                 TZPainterCtrl *pLastParent,
                                 zBOOL         bNewObject )
{
#ifdef DEBUG_ALL
   if ( pLastParent )
      TraceLineS( "TZPainterCtrl::FindParentObject for: ", m_csTag );
   else
      TraceLineS( "TZPainterCtrl::FindParentObject", "" );
#endif

   CPoint         ptMiddle;
   CRect          rectSrchPainter;
   zSHORT         nPos;
   TZPainterCtrl  *pSearchCtrl;
   TZPainterCtrl  *pFoundParent;
   zBOOL          bLastFound;

   // Determine the middle point of the controls in painter
   // rectangle coordinates.
   ptMiddle.x = (rectPainter.left + rectPainter.right) / 2;
   ptMiddle.y = (rectPainter.top  + rectPainter.bottom) / 2;

   pFoundParent = 0;
   if ( pLastParent )
      bLastFound = 0;
   else
      bLastFound = 1;

   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pSearchCtrl = m_pPainterWindow->m_CtrlList[ nPos ];

      // Skip any controls that are not visible (or not selectable).
      if ( pSearchCtrl->GetVisibility( ) )
         continue;

      if ( pSearchCtrl == this && bNewObject == FALSE )
         continue;

      if ( pSearchCtrl->Bigger( this, &rectPainter ) > 0 )
      {
         pSearchCtrl->ParentToPainterRect( rectSrchPainter );
         if ( ptMiddle.x > rectSrchPainter.left &&
              ptMiddle.x < rectSrchPainter.right &&
              ptMiddle.y > rectSrchPainter.top   &&
              ptMiddle.y < rectSrchPainter.bottom )
         {
            if ( pSearchCtrl == pLastParent )
               bLastFound = 1;
            else
            if ( (pLastParent == 0 ||
                  pSearchCtrl->Bigger( pLastParent ) > 0 ||
                  (bLastFound &&
                   pSearchCtrl->Bigger( pLastParent ) == 0)) &&
                 (pFoundParent == 0 ||
                  (pLastParent == 0 &&
                   (pSearchCtrl->m_ulZOrder > pFoundParent->m_ulZOrder ||
                    pSearchCtrl->m_pCtrlParent == pFoundParent)) ||
                  (pLastParent && pFoundParent->Bigger( pSearchCtrl ) > 0)) )
            {
               pFoundParent = pSearchCtrl;
            }
         }
      }
   }

   // All done, return the found parent
#ifdef DEBUG_ALL
   if ( pFoundParent )
      TraceLineS( "TZPainterCtrl::FindParentObject found: ",
                  pFoundParent->m_csTag );
#endif
   return( pFoundParent );
}

//
// This method converts the rect coordinates for a CtrlCover from the
// local coordinates within its parent to the client coordinates for
// the painter window. To do this, the parent CtrlCover's (if any)
// coordinates are added to the local coordinates
//
// N.B.  rectReturn does NOT need to be initialized ... in fact,
// initial values are destroyed!!!
void
TZPainterCtrl::ParentToPainterRect( CRect& rectReturn )
{
#ifdef DEBUG_ALL
// TraceLineI( "TZPainterCtrl::ParentToPainterRect m_rectCtrl.top ",
//             m_rectCtrl.top );
#endif

   TZPainterCtrl *pCtrlParent;

   rectReturn = m_rectCtrl;
   pCtrlParent = m_pCtrlParent;
   while ( pCtrlParent )
      pCtrlParent = pCtrlParent->AdjustCtrlRect( rectReturn, TRUE );

// TraceLineI( "============================ rectReturn.top ",
//             rectReturn.top );
}

//
// This method assigns the control rect position based on a rect position
// passed in painter coordinates.
//
// N.B.  rectPainter MUST be initialized to a rectangle in client
// coordinates!!!
void
TZPainterCtrl::PainterToParentRect( CRect& rectPainter )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::PainterToParentRect: ", m_csTag );
#endif

   TZPainterCtrl *pCtrlParent;

   m_rectCtrl = rectPainter;
   pCtrlParent = m_pCtrlParent;
   while ( pCtrlParent )
      pCtrlParent = pCtrlParent->AdjustCtrlRect( m_rectCtrl, FALSE );

// TraceLineI( "############################ m_rectCtrl.top ",
//             m_rectCtrl.top );
}

void
TZPainterCtrl::MoveCoveringWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::MoveCoveringWindow: ", m_csTag );
#endif

   CRect rectPainter;
   zLONG lScrollX;
   zLONG lScrollY;

   ASSERT( m_pCtrlCover );

   // Convert from m_rectCtrl (in parent coords) to rectPainter (in client
   // coords).  N.B.  rectPainter does NOT need to be initialized ... in
   // fact, initial values are destroyed.
   ParentToPainterRect( rectPainter );

   // Convert from rectPainter (in client coords) to m_rectCtrl (in parent
   // coords).  N.B.  rectPainter MUST be initialized in client coords!!!
   PainterToParentRect( rectPainter );

// TraceRect( "MoveCoveringWindow rectCtrl", m_rectCtrl );

   // This gives the control a chance to alter the "move to" rect (in
   // parent coords).
   m_pPainterWindow->SendCtrlMsg( zCTRLMSG_MOVE, 0, m_lType,
                                  m_pCtrlParent ? m_pCtrlParent->m_lType : 0,
                                  m_bPlaceHolder, &m_rectCtrl, 0, this );

   // These are unadulterated coords (relative to parent).  Everyone has
   // had a shot at changing them.

   // Attempt to adjust for scrolling.
   if ( m_pCtrlParent == 0 ) // no parent ==> set pos relative to client
   {
      CPoint ptScrollPos =
              m_pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );

      lScrollX = ptScrollPos.x;
      lScrollY = ptScrollPos.y;
   }
   else
   {
      lScrollX = 0;
      lScrollY = 0;
   }

   // Now we want to move the "real" // control (as opposed to the
   // CtrlCover) ... HOLD ON A MINUTE ... The move of the CtrlCover
   // will cause the Ctrl to move ... so let's defer this until then.
// m_pWndCtrl->SetWindowPos( &CWnd::wndNoTopMost, m_rectCtrl.left - lScrollX,
//                           m_rectCtrl.top - lScrollY,
//                           m_rectCtrl.right - m_rectCtrl.left,
//                           m_rectCtrl.bottom - m_rectCtrl.top,
//                           SWP_SHOWWINDOW );

   // Now set the rectPainter coords to client coords and move the CtrlCover
   // to the appropriate spot.
   ParentToPainterRect( rectPainter );
// TraceRect( "MoveCoveringWindow rectPainter", rectPainter );
#if 0
   m_pCtrlCover->SetWindowPos( &CWnd::wndTop,
                               rectPainter.left - lScrollX,
                               rectPainter.top - lScrollY,
                               rectPainter.right - rectPainter.left,
                               rectPainter.bottom - rectPainter.top,
                               SWP_SHOWWINDOW );
#else
   if ( (m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
   {
      rectPainter.left -= lScrollX;
      rectPainter.top -= lScrollY;
   }

   m_pCtrlCover->MoveCtrl( rectPainter );
#endif
}

// Move the hidden covering window (CtrlCover) for child controls when
// the parent control is moved.  This is a recursive routine so that
// all descendents of a control are moved.
void
TZPainterCtrl::MoveDescendentCoveringWindows( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::MoveDescendentCoveringWindows: ", m_csTag );
#endif

   zSHORT        nPos;
   CRect         rectPainter;
   TZPainterCtrl *pCtrl;

   if ( m_nChildren == 0 )
      return;

   for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
      if ( pCtrl->m_pCtrlParent == this )
      {
         pCtrl->ParentToPainterRect( rectPainter );
         if ( pCtrl->m_pCtrlCover )
         {
#if 1
            pCtrl->m_pCtrlCover->MoveCtrl( rectPainter );
#else
            pCtrl->m_pCtrlCover->SetWindowPos( &CWnd::wndTop,
                                               rectPainter.left,
                                               rectPainter.top,
                                               rectPainter.right - rectPainter.left,
                                               rectPainter.bottom - rectPainter.top,
                                               SWP_SHOWWINDOW );
#endif
         }

         pCtrl->MoveDescendentCoveringWindows( );
      }
   }
}

//
// METHOD : TZPainterCtrl::PositionOnParentCtrl
//
//    This method positions on a parent control in the Zeidon Object.
//    If the parent control does not exist in the Zeidon object, then
//    it is created!
//
//    RETURNS TRUE  - Positioning successful
//            FALSE - Positioning unsuccessful
//
//
zBOOL
TZPainterCtrl::PositionOnParentCtrl( zVIEW vDialog )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::PositionOnParentCtrl: ", m_csTag );
#endif

   zLONG  lType;
   zCHAR  szText[ 128 ];
   zVIEW  vTZPESRCO;
   zPCHAR pchEntity;

   if ( m_pCtrlParent == 0 )
      return( TRUE );

   if ( vDialog == 0 )
      vDialog = m_pPainterWindow->m_vDialog;

   if ( m_pCtrlParent->PositionOnParentCtrl( vDialog ) == FALSE )
      return( FALSE );

   if ( m_pCtrlParent->m_chEntity == 'C' )
   {
      pchEntity = szlControl;
      if ( m_pCtrlParent->m_pCtrlParent &&
           m_pCtrlParent->m_pCtrlParent->m_chEntity == 'C' )
      {
         SetViewToSubobject( vDialog, szlCtrlCtrl );
      }
   }
   else
   if ( m_pCtrlParent->m_chEntity == 'G' )
      pchEntity = szlGroup;
   else
// if ( m_pCtrlParent->m_chEntity == 'S' )
      pchEntity = szlGroupSet;

   if ( m_pCtrlParent->m_lZKey )
   {
      if ( SetCursorFirstEntityByInteger( vDialog, pchEntity, szlZKey,
                                          m_pCtrlParent->m_lZKey,
                                          0 ) != zCURSOR_SET )
      {
         return( FALSE );
      }
   }
   else
   {
      SetCursorLastEntity( vDialog, pchEntity, 0 );
      if ( m_pPainterWindow->CreateMetaEntity( vDialog, pchEntity,
                                               zPOS_AFTER ) < 0 )
      {
         return( FALSE );
      }

      GetIntegerFromAttribute( &m_pCtrlParent->m_lZKey, vDialog,
                               pchEntity, szlZKey );
      lType = m_pCtrlParent->m_lType;
      szText[ 0 ] = 0;
      m_pCtrlParent->m_pWndCtrl->GetWindowText( szText,
                                                sizeof( szText ) - 1 );
      SetAttributeFromString( vDialog, pchEntity, szlTag,
                              m_pCtrlParent->m_csTag );
      SetAttributeFromString( vDialog, pchEntity, szlText, szText );

      if ( m_pCtrlParent->m_chEntity == 'C' )
      {
         zCHAR  szPlaceHolder[ 2 ];
         zBOOL  bPlaceHolder;

         GetViewByName( &vTZPESRCO, szlTZPESRCO,
                        m_pPainterWindow->m_vDialog, zLEVEL_TASK );
         SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef, szlKey,
                                        lType, 0 );
         IncludeSubobjectFromSubobject( vDialog, szlControlDef,
                                        vTZPESRCO, szlControlDef,
                                        zPOS_BEFORE );
         GetStringFromAttribute( szPlaceHolder, sizeof( szPlaceHolder ), vTZPESRCO, szlControlDef, "PlaceHolder" );
         bPlaceHolder = (szPlaceHolder[ 0 ] == 'Y') ? TRUE : FALSE;
         m_pPainterWindow->CreateMetaEntity( vDialog, szlCtrlMap,
                                             zPOS_AFTER );
         m_pPainterWindow->SendCtrlMsg( zCTRLMSG_CREATE_ZEIDON, vDialog, lType,
                                        m_pCtrlParent ? m_pCtrlParent->m_lType : 0,
                                        bPlaceHolder, 0, 0, this );
      }
      else
      {
         SetAttributeFromInteger( vDialog, pchEntity, "PPE_Type", lType );
      }

      // We've told the control to create its Zeidon information
      // and informed the parent (if any) of the new child, now we
      // extract the type (Key) of control in the event the control or its
      // parent changed the base type
//?   GetIntegerFromAttribute( &lType, vDialog, szlControlDef, szlKey );
   }

   return( TRUE );
}

//
// METHOD : TZPainterCtrl::PositionOnZeidonCtrl
//
//    This method positions on a control in the Zeidon Object.
//    If the control does not exist in the Zeidon object, then
//    it is created!.
//
//    RETURNS TRUE  - Positioning successful
//            FALSE - Positioning unsuccessful
//
//
zBOOL
TZPainterCtrl::PositionOnZeidonCtrl( zVIEW vDialog )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::PositionOnZeidonCtrl: ", m_csTag );
#endif

   zCHAR  szText[ 128 ];
   zVIEW  vTZPESRCO;
   zPCHAR pchEntity;
   zPCHAR pchParent;

   // If vDialog was not passed, get the dialog view from the owning painter.
   if ( vDialog == 0 )
      vDialog = m_pPainterWindow->m_vDialog;

   if ( MiValidView( vDialog ) == 0 )
      return( FALSE );

   if ( MiValidView( m_vCtrl ) == 0 )
      m_vCtrl = 0;

   if ( m_chEntity == 'C' )
   {
      pchEntity = szlControl;
      pchParent = szlWindow;
   }
   else
   {
      pchParent = szlReport;
      if ( m_chEntity == 'G' )
        pchEntity = szlGroup;
      else
   // if ( m_chEntity == 'S' )
        pchEntity = szlGroupSet;
   }

   if ( m_vCtrl )
   {
      zLONG lZKey;

      GetIntegerFromAttribute( &lZKey, m_vCtrl, pchEntity, szlZKey );
      if ( lZKey == m_lZKey )
      {
      // SetViewFromView( vDialog, m_vCtrl );  can't do this since it
      // return( TRUE );                       recreates deleted entities
      }
      else
      {
         while ( ResetViewFromSubobjectWithoutError( m_vCtrl ) == 0 )
         {
         }

         if ( CheckEntityInView( m_vCtrl, pchParent ) != 0 ||
              SetEntityCursor( m_vCtrl, pchEntity, szlZKey, zPOS_FIRST | zQUAL_INTEGER,
                               &m_lZKey, 0, 0, 0, pchParent, 0 ) != zCURSOR_SET )
         {
         // DisplayObjectInstance( m_vCtrl, pchParent, 0 );
            DropView( m_vCtrl );
            m_vCtrl = 0;
         }
      }
   }

   while ( ResetViewFromSubobjectWithoutError( vDialog ) == 0 )
   {
   }

   if ( PositionOnParentCtrl( vDialog ) == 0 )
      return( FALSE );

   if ( m_pCtrlParent && m_pCtrlParent->m_chEntity == 'C' )
      SetViewToSubobject( vDialog, szlCtrlCtrl );

   if ( m_lZKey )
   {
      if ( SetCursorFirstEntityByInteger( vDialog, pchEntity, szlZKey,
                                          m_lZKey, 0 ) != zCURSOR_SET )
      {
         return( FALSE );
      }
   }
   else
   {
      SetCursorLastEntity( vDialog, pchEntity, 0 );
      if ( m_pPainterWindow->CreateMetaEntity( vDialog, pchEntity,
                                               zPOS_AFTER ) < 0 )
      {
         return( FALSE );
      }

      GetIntegerFromAttribute( &m_lZKey, vDialog, pchEntity, szlZKey );
//    TraceLineS( "Initializing view for Ctrl: ", m_csTag );
      if ( m_vCtrl == 0 )
      {
         CreateViewFromViewForTask( &m_vCtrl, vDialog,
                                    m_pPainterWindow->m_vDialog );
         SfLockView( m_vCtrl );  // we lock this view because it is
                                 // unnamed ... to prevent the OE from
                                 // automatically cleaning it up during
                                 // the deletion of an OE subtask.
      // AddCursorListener( m_vCtrl, PainterCursorListener, this );
      }

      if ( m_chEntity == 'C' )
      {
         zCHAR  szPlaceHolder[ 2 ];
         zBOOL  bPlaceHolder;

         szText[ 0 ] = 0;
         if ( m_pWndCtrl && mIs_hWnd( m_pWndCtrl->m_hWnd ) )
         {
            m_pWndCtrl->GetWindowText( szText, sizeof( szText ) - 1 );
            SetAttributeFromString( vDialog, pchEntity, szlText, szText );
         }

         SetAttributeFromString( vDialog, pchEntity, szlTag, m_csTag );
         GetViewByName( &vTZPESRCO, szlTZPESRCO,
                        m_pPainterWindow->m_vDialog, zLEVEL_TASK );
         SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef, szlKey,
                                        m_lType, 0 );
         IncludeSubobjectFromSubobject( vDialog, szlControlDef,
                                        vTZPESRCO, szlControlDef,
                                        zPOS_BEFORE );
         GetStringFromAttribute( szPlaceHolder, sizeof( szPlaceHolder ), vTZPESRCO, szlControlDef, "PlaceHolder" );
         bPlaceHolder = (szPlaceHolder[ 0 ] == 'Y') ? TRUE : FALSE;
         m_pPainterWindow->CreateMetaEntity( vDialog, szlCtrlMap,
                                             zPOS_AFTER );
         m_pPainterWindow->SendCtrlMsg( zCTRLMSG_CREATE_ZEIDON, vDialog, m_lType,
                                        m_pCtrlParent ? m_pCtrlParent->m_lType : 0,
                                        bPlaceHolder, 0, 0, this );
      }
      else
      {
         SetAttributeFromInteger( vDialog, pchEntity, "PPE_Type", m_lType );
      }

      // We've told the control to create its Zeidon information
      // and informed the parent (if any) of the new child, now we
      // extract the type (Key) of control in the event the control or its
      // parent changed the base type.
//?   GetIntegerFromAttribute( &m_lType, vDialog, szlControlDef, szlKey );
   }

// zPCHAR pch;
// GetAddrForAttribute( &pch, vDialog, pchEntity, szlTag );
// TraceLineS( "PositionOnZeidonCtrl: ", pch );

   return( TRUE );
}

void
TZPainterCtrl::CopySubordinateEntities( zVIEW vOrigCtrl, zVIEW vNewCtrl )
{
   zSHORT nRC = SetCursorFirstEntity( vOrigCtrl, szlCtrlMap, 0 );
   while ( nRC == zCURSOR_SET )
   {
      m_pPainterWindow->CreateMetaEntity( vNewCtrl, szlCtrlMap, zPOS_AFTER );
      SetMatchingAttributesByName( vNewCtrl, szlCtrlMap,
                                   vOrigCtrl, szlCtrlMap,
                                   zSET_NULL | zSET_NOTNULL );
      if ( CheckExistenceOfEntity( vOrigCtrl, szlCtrlMapLOD_Attribute ) ==
                                                            zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vNewCtrl, szlCtrlMapLOD_Attribute,
                                        vOrigCtrl, szlCtrlMapLOD_Attribute,
                                        zPOS_AFTER );
      }

      if ( CheckExistenceOfEntity( vOrigCtrl, szlCtrlMapLOD_Entity ) ==
                                                            zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vNewCtrl, szlCtrlMapLOD_Entity,
                                        vOrigCtrl, szlCtrlMapLOD_Entity,
                                        zPOS_AFTER );
      }

      if ( CheckExistenceOfEntity( vOrigCtrl, szlCtrlMapContext ) ==
                                                            zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vNewCtrl, szlCtrlMapContext,
                                        vOrigCtrl, szlCtrlMapContext,
                                        zPOS_AFTER );
      }

      if ( CheckExistenceOfEntity( vOrigCtrl, szlCtrlMapView ) ==
                                                            zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vNewCtrl, szlCtrlMapView,
                                        vOrigCtrl, szlCtrlMapView,
                                        zPOS_AFTER );
      }

      nRC = SetCursorNextEntity( vOrigCtrl, szlCtrlMap, 0 );
   }

   if ( CheckExistenceOfEntity( vOrigCtrl, "Font" ) == 0 )
   {
      m_pPainterWindow->CreateMetaEntity( vNewCtrl, "Font", zPOS_AFTER );
      SetMatchingAttributesByName( vNewCtrl, "Font",
                                   vOrigCtrl, "Font", zSET_NULL );
   }

   if ( CheckExistenceOfEntity( vOrigCtrl, "ControlNLS_Text" ) == 0 )
   {
      m_pPainterWindow->CreateMetaEntity( vNewCtrl, "ControlNLS_Text", zPOS_AFTER );
      SetMatchingAttributesByName( vNewCtrl, "ControlNLS_Text",
                                   vOrigCtrl, "ControlNLS_Text", zSET_NULL );
   }

   if ( CheckExistenceOfEntity( vOrigCtrl, "ControlNLS_DIL_Text" ) == 0 )
   {
      m_pPainterWindow->CreateMetaEntity( vNewCtrl, "ControlNLS_DIL_Text", zPOS_AFTER );
      SetMatchingAttributesByName( vNewCtrl, "ControlNLS_DIL_Text",
                                   vOrigCtrl, "ControlNLS_DIL_Text", zSET_NULL );
   }

   nRC = SetCursorFirstEntity( vOrigCtrl, "WebControlProperty", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      m_pPainterWindow->CreateMetaEntity( vNewCtrl, "WebControlProperty", zPOS_AFTER );
      SetMatchingAttributesByName( vNewCtrl, "WebControlProperty",
                                   vOrigCtrl, "WebControlProperty", zSET_NULL );
      nRC = SetCursorNextEntity( vOrigCtrl, "WebControlProperty", 0 );
   }

   if ( (m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
   {
#if 0
      nRC = SetCursorFirstEntity( vOrigCtrl, "CtrlFontOverride", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         m_pPainterWindow->CreateMetaEntity( vNewCtrl, "CtrlFontOverride",
                                             zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewCtrl, "CtrlFontDef",
                                        vOrigCtrl, "CtrlFontDef", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewCtrl, "CtrlFont",
                                        vOrigCtrl, "CtrlFont", zPOS_AFTER );

         nRC = SetCursorNextEntity( vOrigCtrl, "CtrlFontOverride", 0 );
      }

      nRC = SetCursorFirstEntity( vOrigCtrl, "CtrlClrOverride", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         m_pPainterWindow->CreateMetaEntity( vNewCtrl, "CtrlClrOverride",
                                             zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewCtrl, "CtrlColorDef",
                                        vOrigCtrl, "CtrlColorDef", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewCtrl, "CtrlColor",
                                        vOrigCtrl, "CtrlColor", zPOS_AFTER );

         nRC = SetCursorNextEntity( vOrigCtrl, "CtrlClrOverride", 0 );
      }
#endif

      nRC = SetCursorFirstEntity( vOrigCtrl, szlEvent, 0 );
      while ( nRC == zCURSOR_SET )
      {
         m_pPainterWindow->CreateMetaEntity( vNewCtrl, szlEvent, zPOS_AFTER );
         SetMatchingAttributesByName( vNewCtrl, szlEvent,
                                      vOrigCtrl, szlEvent,
                                      zSET_NULL | zSET_NOTNULL );
         if ( CheckExistenceOfEntity( vOrigCtrl, szlEventAct ) == zCURSOR_SET )
         {
            IncludeSubobjectFromSubobject( vNewCtrl, szlEventAct,
                                           vOrigCtrl, szlEventAct,
                                           zPOS_AFTER );
//          m_pPainterWindow->CreateMetaEntity( vNewCtrl, szlEventAct,
//                                              zPOS_AFTER );
//          SetMatchingAttributesByName( vNewCtrl, szlEventAct,
//                                       vOrigCtrl, szlEventAct,
//                                       zSET_NULL | zSET_NOTNULL );
         }

         nRC = SetCursorNextEntity( vOrigCtrl, szlEvent, 0 );
      }
   }
}

//
// METHOD : TZPainterCtrl::CopyZeidonCtrl
//
//    This method copies a Zeidon control under the given parent
//
//
TZPainterCtrl *
TZPainterCtrl::CopyZeidonCtrl( TZPainterCtrl *pParent, zSHORT nIdNbr,
                               zCPCHAR cpcLabel, CRect *rectCtrl )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::CopyZeidonCtrl: ", m_csTag );
#endif

   zVIEW         vNewCtrl;
   zVIEW         vOrigCtrl;
   zLONG         lSubZKey;
   zLONG         lNewZKey;
   zLONG         lType;
   zLONG         lPosX, lPosY;
   zLONG         lSizeX, lSizeY;
   zCHAR         szNewTag[ 40 ];
   TZPainterCtrl *pNewCtrl = 0;
   TZPainterCtrl *pCtrl;
   zSHORT        nPos;
// zBOOL         bSelected;

   // If the control is not yet in the Zeidon object, return.
   if ( m_lZKey == 0 )
      return( 0 );

// // If control being copied is selected, then the new control will
// // be selected
// bSelected = IsSelected( );

   // Create two views from the existing dialog view.  vOrigCtrl is used
   // to position on the Control to be cloned, while vNewCtrl is used to
   // position on the new Control.
   CreateViewFromViewForTask( &vOrigCtrl, m_pPainterWindow->m_vDialog,
                              m_pPainterWindow->m_vDialog );
   CreateViewFromViewForTask( &vNewCtrl, vOrigCtrl,
                              m_pPainterWindow->m_vDialog );

   // Position on the control to be copied.
   PositionOnZeidonCtrl( vOrigCtrl );
// SetNameForView( vOrigCtrl, "__OrigCtrl", 0, zLEVEL_TASK );

   // Position on the parent on which the new control is to be placed.
   if ( pParent )
   {
      pParent->PositionOnZeidonCtrl( vNewCtrl );

      if ( (m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 ||
           ((m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
            pParent->m_chEntity == 'C') )
      {
         SetViewToSubobject( vNewCtrl, szlCtrlCtrl );
      }
   }
   else
   {
      while ( ResetViewFromSubobject( vNewCtrl ) == 0 )
      {
      }
   }

   // Position to the end of the list and then create the new control.
   SetCursorLastEntity( vNewCtrl, szlControl, 0 );
   m_pPainterWindow->CreateMetaEntity( vNewCtrl, szlControl, zPOS_AFTER );

   // Now copy the information from the source control to the target control.
   SetMatchingAttributesByName( vNewCtrl, szlControl,
                                vOrigCtrl, szlControl,
                                zSET_NULL | zSET_NOTNULL );
// SetNameForView( vNewCtrl, "__NewCtrl", 0, zLEVEL_TASK );
// ::MessageBox( 0, "Orig and New", "CopyZeidonCtrl", MB_OK );
   if ( CheckExistenceOfEntity( vOrigCtrl, szlControlDef ) == zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vNewCtrl, szlControlDef,
                                     vOrigCtrl, szlControlDef,
                                     zPOS_AFTER );
   }

   // If a position has been passed, set the positioning attributes
   // from the position passed.
   if ( rectCtrl )
   {
      if ( m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
      {
         lPosX = rectCtrl->left;
         lPosY = rectCtrl->top;

         if ( pParent->m_chEntity == 'G' )
         {
            // 8 for GroupSet + Group indent
            lPosX -= m_pPainterWindow->m_lLeftMargin - 8;
         }

         lPosX = mConvertPixelTo256thInchX( lPosX );
         lPosY = mConvertPixelTo256thInchY( lPosY );
         lSizeX = mConvertPixelTo256thInchX( rectCtrl->right - rectCtrl->left );
         lSizeY = mConvertPixelTo256thInchY( rectCtrl->bottom - rectCtrl->top );
      }
      else
      {
         zLONG lStyleEx;

         GetIntegerFromAttribute( &lStyleEx, vNewCtrl,
                                  szlControl, "ExtendedStyle" );

         if ( lStyleEx & zCONTROLX_ABSOLUTE_PIXEL_POS )
         {
            lPosX = rectCtrl->left;
            lPosY = rectCtrl->top;
         }
         else
         {
            if ( m_pPainterWindow->m_pBar->m_bUseMapDlgUnits )
            {
               lPosX = mConvertPixelToMapDlgX( rectCtrl->left );
               lPosY = mConvertPixelToMapDlgY( rectCtrl->top );
            }
            else
            {
               lPosX = mConvertPixelToDlgUnitX( rectCtrl->left );
               lPosY = mConvertPixelToDlgUnitY( rectCtrl->top );
            }
         }

         if ( lStyleEx & zCONTROLX_ABSOLUTE_PIXEL_SIZE )
         {
            lSizeX = rectCtrl->right - rectCtrl->left;
            lSizeY = rectCtrl->bottom - rectCtrl->top;
         }
         else
         {
            if ( m_pPainterWindow->m_pBar->m_bUseMapDlgUnits )
            {
               lSizeX =
                 mConvertPixelToMapDlgX( rectCtrl->right - rectCtrl->left );
               lSizeY =
                 mConvertPixelToMapDlgY( rectCtrl->bottom - rectCtrl->top );
            }
            else
            {
               lSizeX =
                 mConvertPixelToDlgUnitX( rectCtrl->right - rectCtrl->left );
               lSizeY =
                 mConvertPixelToDlgUnitY( rectCtrl->bottom - rectCtrl->top );
            }
         }
      }

      SetAttributeFromInteger( vNewCtrl, szlControl, szlPSDLG_X, lPosX );
      SetAttributeFromInteger( vNewCtrl, szlControl, szlPSDLG_Y, lPosY );
      SetAttributeFromInteger( vNewCtrl, szlControl, szlSZDLG_X, lSizeX );
      SetAttributeFromInteger( vNewCtrl, szlControl, szlSZDLG_Y, lSizeY );
   }

   SetAttributeFromInteger( vNewCtrl, szlControl, szlSyncKey, 9999 );

   // Copy subordinate entity types
   CopySubordinateEntities( vOrigCtrl, vNewCtrl );

   // Get the control Id and generate a new tag for the control
   if ( SetCursorFirstEntity( vNewCtrl, szlControlDef, 0 ) == zCURSOR_SET )
      GetIntegerFromAttribute( &lType, vNewCtrl, szlControlDef, szlKey );
   else
      GetIntegerFromAttribute( &lType, vNewCtrl, szlControl, szlType );

   szNewTag[ 0 ] = 0;
   m_pPainterWindow->GenerateTag( szNewTag, lType );
   SetAttributeFromString( vNewCtrl, szlControl, szlTag, szNewTag );
   GetIntegerFromAttribute( &lNewZKey, vNewCtrl, szlControl, szlZKey );
// TraceLineI( "CtrlCnt Before PaintCtrl: ", m_pPainterWindow->m_nCtrlCnt );
   pNewCtrl = m_pPainterWindow->PaintCtrl( pParent, szlControl,
                                           vNewCtrl, 0, 0, 0, 0,
                                           lNewZKey, FALSE );
// TraceLineI( "CtrlCnt After PaintCtrl: ", m_pPainterWindow->m_nCtrlCnt );
// ::MessageBox( 0, "PaintCtrl", "After", MB_OK );

   // If the control has sub-controls, then recursively call ourself for
   // each sub-control.
   if ( pNewCtrl &&
        SetCursorFirstEntity( vOrigCtrl, szlCtrlCtrl, 0 ) == zCURSOR_SET )
   {
      do
      {
         // Get the sync key from the sub-control to be copied.
         GetIntegerFromAttribute( &lSubZKey, vOrigCtrl, szlCtrlCtrl, szlZKey );

         // Find the sync key in the ctrl cover list.
         pCtrl = 0;
         for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
         {
            pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            if ( pCtrl->m_lZKey == lSubZKey )
               break;
         }

         // If the ctrl cover was found, recursively call ourself.
         if ( pCtrl && nPos < m_pPainterWindow->m_nCtrlCnt )
            pCtrl->CopyZeidonCtrl( pNewCtrl, nIdNbr, cpcLabel );
         else
         {
            // The sub-control is not a "painted" control, so we will just
            // clone it in place (without adding it to the list of controls).
            zULONG ulKey;

            ulKey = GetEntityKey( vOrigCtrl, szlCtrlCtrl );
            SetViewToSubobject( vOrigCtrl, szlCtrlCtrl );
            SetViewToSubobject( vNewCtrl, szlCtrlCtrl );
            m_pPainterWindow->CreateMetaEntity( vNewCtrl, szlControl,
                                                zPOS_AFTER );

            // Now copy information from source control to target control.
            SetMatchingAttributesByName( vNewCtrl, szlControl,
                                         vOrigCtrl, szlControl,
                                         zSET_NULL | zSET_NOTNULL );
            if ( CheckExistenceOfEntity( vOrigCtrl,
                                         szlControlDef ) == zCURSOR_SET )
            {
               IncludeSubobjectFromSubobject( vNewCtrl, szlControlDef,
                                              vOrigCtrl, szlControlDef,
                                              zPOS_AFTER );
            }

            // Copy subordinate entity types
            CopySubordinateEntities( vOrigCtrl, vNewCtrl );

            ResetViewFromSubobject( vOrigCtrl );
            ResetViewFromSubobject( vNewCtrl );
            if ( ulKey )
            {
               SetEntityCursor( vOrigCtrl, szlCtrlCtrl, 0,
                                zQUAL_ENTITYKEY | zPOS_FIRST,
                                (zCPVOID) &ulKey, 0, 0, 0, 0, 0 );
            }
         }

      }  while ( SetCursorNextEntity( vOrigCtrl,
                                      szlCtrlCtrl, 0 ) == zCURSOR_SET );
   }

   DropView( vNewCtrl );     // this ctrl view is no longer needed

   if ( pNewCtrl )
   {
      zSHORT nPos = m_pPainterWindow->m_nCtrlCnt;

      while ( nPos > 0 )
      {
         nPos--;
         if ( pNewCtrl == m_pPainterWindow->m_CtrlList[ nPos ] )
         {
            m_pPainterWindow->AddToUndo( pNewCtrl, nIdNbr, nPos, FALSE,
                                         TZUndoRedo::eClone, cpcLabel );
            break;
         }
      }
   }

   // Drop the temporary dialog view.
   DropView( vOrigCtrl );
   return( pNewCtrl );
}

//
// This method positions on a Control in the Zeidon Object and moves it
// to a new parent.
//
void
TZPainterCtrl::MoveZeidonCtrl( TZPainterCtrl *pNewParent )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::MoveZeidonCtrl: ", m_csTag );
#endif

   zPCHAR pchEntity = szlControl;
   zVIEW vNewParent;

   // If the control is not yet in the Zeidon object, or the current
   // parent is the new parent, return
   if ( m_lZKey == 0 || m_pCtrlParent == pNewParent )
      return;

   // Create a view from the existing dialog view
   CreateViewFromViewForTask( &vNewParent, m_pPainterWindow->m_vDialog,
                              m_pPainterWindow->m_vDialog );

   // Position on the control to be moved
   PositionOnZeidonCtrl( );

   // Position on the new parent
// SetNameForView( vNewParent, "__NewParent", 0, zLEVEL_TASK );
   if ( pNewParent )
   {
      pNewParent->PositionOnZeidonCtrl( vNewParent );

      if ( pNewParent->m_chEntity == 'C' )
      {
         SetViewToSubobject( vNewParent, szlCtrlCtrl );
      }
      else
      if ( pNewParent->m_chEntity == 'S' )
      {
         pchEntity = szlGroup;
      }
   }

   // Move the Control.  Since the subobject is being moved, the normal
   // include processing rules are not used.  The Source entity MUST be
   // excludable and the Target entity MUST be creatable.

   SetCursorLastEntity( vNewParent, pchEntity, 0 );
   if ( MoveSubobject( vNewParent, pchEntity,
                       m_pPainterWindow->m_vDialog, pchEntity,
                       zPOS_AFTER, zREPOS_NEXT ) == zCALL_ERROR )
   {
//    TraceLineX( "MoveSubobject view 1 ", (zLONG) vNewParent );
//    TraceLineX( "MoveSubobject view 2 ", (zLONG) m_pPainterWindow->m_vDialog );
//    TraceLineS( "MoveSubobject entity ", pchEntity );
      if ( m_pCtrlCover )
         m_pCtrlCover->MessageBox( "MoveSubobject error", pchEntity );
   }

   // Reset the painter dialog so Window is in view
   while ( ResetViewFromSubobject( m_pPainterWindow->m_vDialog ) == 0 )
   {
   }

   DropView( vNewParent );
}

// Clone a control in a new position, if the control is connected to a
// Zeidon object, copy the Zeidon control and create a new synckey and
// tag for the control.
TZPainterCtrl *
TZPainterCtrl::Clone( TZPainterCtrl *pParent, CRect& rectNew,
                      zSHORT nIdNbr, zCPCHAR cpcLabel )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::Clone: ", m_csTag );
#endif

   zCHAR   szText[ 128 ];
   zLONG   lNewZKey;
   zCHAR   szNewTag[ 40 ];
   TZPainterCtrl *pNewCtrl;

   if ( (m_pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
        m_chEntity != 'C' )
   {
      return( 0 ); // we do not currently permit cloning of GroupSets or Groups
   }

   lNewZKey = 0;

   // If the control is connected to a Zeidon object, go to that object
   // and create a new control with matching attributes.
   if ( m_lZKey )
   {
      pNewCtrl = CopyZeidonCtrl( pParent, nIdNbr, cpcLabel, &rectNew );
   }
   else
   {
      szNewTag[ 0 ] = 0;
      m_pPainterWindow->GenerateTag( szNewTag, m_lType );

      m_pWndCtrl->GetWindowText( szText, sizeof( szText ) - 1 );
      if ( szText[ 0 ] == 0 )
         strcpy_s( szText, sizeof( szText ), szNewTag );

      pNewCtrl = m_pPainterWindow->PaintCtrl( pParent, szlControl,
                                              0, m_lType, &rectNew,
                                              szNewTag, szText,
                                              lNewZKey, FALSE );

      // If the control has children, clone them as well.
      if ( pNewCtrl )
      {
         zSHORT   nPos;

         if ( m_nChildren )
         {
            TZPainterCtrl *pChildCtrl;
            for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
            {
               pChildCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
               if ( pChildCtrl->m_pCtrlParent == this )
               {
                  pChildCtrl->Clone( pNewCtrl, pChildCtrl->m_rectCtrl,
                                     nIdNbr, cpcLabel );
               }
            }
         }

         nPos = m_pPainterWindow->m_nCtrlCnt;

         while ( nPos > 0 )
         {
            nPos--;
            if ( pNewCtrl == m_pPainterWindow->m_CtrlList[ nPos ] )
            {
               m_pPainterWindow->AddToUndo( pNewCtrl, nIdNbr, nPos, FALSE,
                                            TZUndoRedo::eClone, cpcLabel );
               break;
            }
         }
      }
   }

   return( pNewCtrl );
}

zBOOL
TZPainterCtrl::ShowContextMenu( CPoint& pt )
{
   return( FALSE );  // there is no default context menu for a control (yet)
}

zBOOL
TZPainterCtrl::Select( )
{
#ifdef DEBUG_ALL
   TraceCtrl( "TZPainterCtrl::Select", this );
#endif

   if ( m_pPainterWindow && m_pPainterWindow->m_pDirectModeCtrl )
   {
      TZPainterCtrl *pCtrl = m_pPainterWindow->m_pDirectModeCtrl;
      if ( pCtrl )
      {
         CRect rect;

         pCtrl->m_pCtrlCover->GetClientRect( &rect );
         pCtrl->m_pCtrlCover->ClientToScreen( &rect );
         m_pPainterWindow->m_pPainterClient->ScreenToClient( &rect );
      // pCtrl->m_pCtrlCover->ShowWindow( SW_SHOW );
         pCtrl->m_pCtrlCover->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                            SWP_NOSIZE | SWP_NOMOVE |
                                            SWP_NOREDRAW | SWP_SHOWWINDOW );

         rect.InflateRect( 4, 4 );
         m_pPainterWindow->m_pPainterClient->InvalidateRect( rect );
         m_pPainterWindow->m_pDirectModeCtrl = 0;
      }
   }

   // Skip any ctrls that are not visible or not selectable.
   if ( GetVisibility( ) || m_pPainterWindow == 0 ||
        (m_pPainterWindow->m_fWndState & zTZPW_UPDATEABLE) == 0 )
   {
      return( FALSE );
   }

   m_pPainterWindow->m_pLastSelected = this;

   // Let the Windows control know ... for example, the NoteBook control
   // needs this in order to select the appropriate page.
   SelectPoint( m_ptLastMouseMove );
   if ( IsSelected( ) == FALSE )
   {
      if ( m_pCtrlCover )
      {
#if 0
         TZPainterCtrl *pCtrl = m_pCtrlParent;
         while ( pCtrl )
         {
            if ( pCtrl->m_pWndCtrl )
               pCtrl->m_pWndCtrl->UpdateWindow( );

            pCtrl = pCtrl->m_pCtrlParent;
         }
#else
         TZPainterCtrl *pCtrl;
         for ( zSHORT nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
         {
            pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            if ( pCtrl->m_pWndCtrl && mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
               pCtrl->m_pWndCtrl->UpdateWindow( );
         }
#endif

//       m_pCtrlCover->SetRedraw( FALSE );
         m_pCtrlCover->m_pSelectCover = new TZSelectCover( this );
         m_pPainterWindow->m_nSelectCnt++;
//       TraceLineI( "TzPainterCtrl::Select cnt: ",
//                   m_pPainterWindow->m_nSelectCnt );
//       m_pCtrlCover->SetRedraw( TRUE );

         // This is to force the currently selected control to the top.
         m_pCtrlCover->m_pSelectCover->Invalidate( FALSE );
#if 0
         m_pCtrlCover->m_pSelectCover->
                   SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                 SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
//       m_pCtrlCover->
//                 SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
//                               SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );

         m_pWndCtrl->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                   SWP_NOSIZE | SWP_NOMOVE |
                                   SWP_NOREDRAW | SWP_SHOWWINDOW );
         m_pWndCtrl->ValidateRect( 0 );
#endif
         m_pCtrlCover->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                     SWP_NOSIZE | SWP_NOMOVE |
                                     SWP_NOREDRAW | SWP_SHOWWINDOW );
         m_pCtrlCover->ValidateRect( 0 );
//       m_pCtrlCover->m_pSelectCover->
//                 SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
//                               SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
//x      m_pCtrlCover->m_pSelectCover->
//x                SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
//x                              SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW );
//x                              SWP_DRAWFRAME | SWP_FRAMECHANGED );
//       m_pCtrlCover->m_pSelectCover->UpdateWindow( );
      }

      return( FALSE );     // was not already selected
   }

   return( TRUE );         // was already selected
}

// The control is being deselected, delete the sizing frame around it.
zBOOL
TZPainterCtrl::Deselect( )
{
#ifdef DEBUG_ALL
// TraceCtrl( "TZPainterCtrl::Deselect", this );
#endif

   if ( m_pPainterWindow == 0 ||
        (m_pPainterWindow->m_fWndState & zTZPW_UPDATEABLE) == 0 )
   {
      return( TRUE );
   }

   if ( m_pPainterWindow->m_pLastSelected == this )
   {
      m_pPainterWindow->m_pLastSelected = 0;
      m_pPainterWindow->SetDIL_Text( );
   }

   if ( IsSelected( ) )
   {
      mDeleteInit( m_pCtrlCover->m_pSelectCover );

      m_pPainterWindow->m_nSelectCnt--;
      if ( m_nChildren )
      {
         TZPainterCtrl *pCtrl;
         zSHORT        nPos;

         for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
         {
            pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
            if ( pCtrl->m_pCtrlParent == this && pCtrl->m_pCtrlCover )
            {
               pCtrl->m_pCtrlCover->
                   SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,
                                 SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
            }
         }
      }

      return( FALSE );  // deselected
   }

   return( TRUE );      // was not selected (did not need to deselect)
}

zBOOL
TZPainterCtrl::IsSelected( )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::IsSelected: ", m_csTag );
#endif

   if ( m_pCtrlCover && m_pCtrlCover->m_pSelectCover )
      return( TRUE );

   return( FALSE );
}

void
TZPainterCtrl::InvalidateParent( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::InvalidateParent: ", m_csTag );
#endif

#if 0
   CRect rect = m_rectCtrl;
   if ( IsSelected( ) )
   {
      rect.InflateRect( TZCtrlCover::m_nFrameSizeX,
                        TZCtrlCover::m_nFrameSizeY );
   }

   if ( m_pWndParent )
      m_pWndParent->InvalidateRect( rect, TRUE );
   else
      m_pPainterWindow->m_pPainterClient->InvalidateRect( rect, TRUE );
#endif
}

// Note:  this code is very similar to the code in
// TZToolBox::PaintToolBoxCtrl (and should thereby be kept in sync).
//
TZPainterCtrl *
TZPainterCtrl::ValidateParentObject( CRect& rect,
                                     zLONG& lNewType,
                                     zSHORT nValidate )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::ValidateParentObject: ", m_csTag );
#endif

   zVIEW  vCtrlDef;
   zVIEW  vParentDef;
   zSHORT nRC;

   TZPainterCtrl *pNewParent = FindParentObject( rect, 0, 0 );

   // If the parent control found is selected, then it is being moved
   // along with this control, assume the parent is not changed.
   if ( pNewParent && pNewParent->IsSelected( ) )
      pNewParent = m_pCtrlParent;

   zLONG lType = lNewType = m_lType;

   GetViewByName( &vCtrlDef, szlTZPESRCO, m_pPainterWindow->m_vDialog, zLEVEL_TASK );
   vParentDef = 0;
   if ( pNewParent )
   {
      // If the parent wants someone else to be the parent, give
      // it the chance to let us know now!
      pNewParent = pNewParent->GetRealCtrl( );

      CreateViewFromViewForTask( &vParentDef, vCtrlDef,
                                 m_pPainterWindow->m_vDialog );
      SetCursorFirstEntityByInteger( vParentDef, szlControlDef, szlKey,
                                     pNewParent->m_lType, 0 );
   }

   SetCursorFirstEntityByInteger( vCtrlDef, szlControlDef,
                                  szlKey, lType, 0 );

   // CtrlValidate will return:
   //   -1 - Create or move is invalid
   //    0 - Create or move is valid and vParent is a valid parent.
   //    1 - Create or move is valid but vParent is not a valid parent
   //        so create or move vCtrl to the client area (if the
   //        client is a valid parent).
   nRC = CtrlValidate( vCtrlDef, vParentDef, 0, nValidate );

   while ( pNewParent && nRC == 1 )
   {
      pNewParent = FindParentObject( rect, pNewParent, 0 );
      if ( pNewParent )
      {
         SetCursorFirstEntityByInteger( vParentDef, szlControlDef,
                                        szlKey, pNewParent->m_lType, 0 );
         nRC = CtrlValidate( vCtrlDef, vParentDef, 0, nValidate );
      }
   }

   if ( nRC < 0 )
      lNewType = 0;
   else
   {
      // Get the ID of the control to be painted
      GetIntegerFromAttribute( &lNewType, vCtrlDef, szlControlDef, szlKey );

      // If the parent of the control is to be the client area, then
      // zero out the candidate parent and update the paint position
      // of the control.
      if ( vParentDef && nRC == 1 )
      {
         pNewParent = 0;
      }
   }

   // Drop the view to the parent control if one was created
   if ( vParentDef )
      DropView( vParentDef );

   if ( pNewParent && lNewType )
   {
      if ( pNewParent != m_pCtrlParent &&
           pNewParent->IsChildValid( lNewType ) == FALSE )
      {
         pNewParent = 0;
         lNewType = 0;
      }
   }

   return( pNewParent );
}

HBRUSH
TZPainterCtrl::CtrlColor( CDC *pDC, HBRUSH hBrushDflt )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::CtrlColor: ", m_csTag );
#endif

   if ( m_clrText )
   {
      pDC->SetTextColor( m_clrText );
   }

   return( hBrushDflt );
}

/////////////////////////////////////////////////////////////////////////////
//
// The return code has three values:
//   0 - painter can select and display
//   1 - painter can display but not select ???
//   2 - painter can select but not display ???
//   3 - painter cannot select or display
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
TZPainterCtrl::GetVisibility( )
{
   if ( (m_uVisible & 0xFF7F) == 0 )
      return( 0 );

   if ( (m_uVisible & 0xFF7E) == 0 )
      return( 1 );

// if ( (m_uVisible & 0xFF7E) == 0 )
//    return( 2 );

   return( 2 );
}

/////////////////////////////////////////////////////////////////////////////
//
// uVisible has four states:
// 0x0000 - painter can select and display
// 0x0001 - painter can select ... not display
// 0x0010 - ctrl is a congruent sibling of a ctrl with the 0x0080 bit set,
//          it is not visible to the user (or to the painter) ...
//          painter cannot select or display.  It can be made selectable
//          and displayable if the sibling ctrl on top of it (with the
//          same size and position) is selected again ... in which case,
//          the sibling is made non-visible and non-selectable and is made
//          lower than this ctrl in z-order.
// 0x0020 - ctrl is a child of a ctrl with the 0x0010 bit set.
// 0x0080 - ctrl is a congruent sibling (same parent, position, and size)
//          causing other ctrl(s) to have their 0x0010 bits set.
// 0x0100 - ctrl is not visible to the user (or to the painter) ...
//          painter cannot select or display
//
// nRecurse has three states:
//    1  - children of a ctrl are set to the same visibility as the ctrl
//    0  - only the current ctrl is set to the visible state
//   -1  - only the children of a ctrl are set to visible state, the
//         ctrl itself is left alone
//   -2  - only the grand-children of a ctrl are set to visible state,
//         the ctrl itself and its children are left alone
//   -3  - and so on ...
//
// This routine is thereby recursive in nature (when nRecurse is non-zero).
//
// nFlag has three settings:
//   < 0 - remove bits from flag
//   = 0 - set flag from bits
//   > 0 - add bits to flag
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
TZPainterCtrl::SetVisibility( zUSHORT uVisible,
                              zSHORT  nRecurse,
                              zSHORT  nFlag )
{
   if ( nRecurse >= 0 )
   {
      if ( nFlag == 0 )
         m_uVisible = uVisible;
      else
      if ( nFlag > 0 )
         m_uVisible |= uVisible;
      else
   // if ( nFlag < 0 )
         m_uVisible &= ~uVisible;

#ifdef DEBUG_ALL
//    if ( zstrcmp( m_csTag, "General" ) == 0 ||
//         (m_pCtrlParent && zstrcmp( m_pCtrlParent->m_csTag, "General" ) == 0) )
      if ( m_lType == 2090 || m_lType == 2091 ) // 0x082A || 0x082B
      {
         TraceLineX( "TZPainterCtrl::SetVisibility ", m_uVisible );
         TraceLineS( "  for control: ", m_csTag );
      }
#endif

      if ( GetVisibility( ) > 1 )
      {
         Deselect( );
         mDeleteInit( m_pCtrlCover );
         RealizeCtrl( FALSE );
      }
      else
      {
         RealizeCtrl( TRUE );
         if ( m_pCtrlCover == 0 )
         {
            // m_pCtrlCover is set by the TZCtrlCover ctor.
            new TZCtrlCover( m_pPainterWindow, this, m_csTag );
         }

         m_pCtrlCover->ShowWindow( GetVisibility( ) > 1 ? SW_HIDE : SW_SHOW );
      }

//    TraceLineI( "PainterCtrl SetVisibility IdNbr = ", m_lCtrlID );
//    TraceLineI( "===================>>> uVisible = ", uVisible );
   }
   else
   if ( nRecurse < -1 )
   {
      // Don't do this control or children of this control.
      nRecurse++;
   }
   else
   {
      // We didn't do this control, but all descendents of this control.
      nRecurse = 1;
   }

   if ( nRecurse )
   {
      TZPainterCtrl *pCtrl;
      zSHORT nPos;
      for ( nPos = 0; nPos < m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl->m_pCtrlParent == this )
         {
            pCtrl->SetVisibility( uVisible, nRecurse, nFlag );
         }
      }
   }

   return( FALSE );
}

void
TZPainterCtrl::RealizeCtrl( zBOOL bCreate )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::RealizeCtrl: ", m_csTag );
#endif

   if ( bCreate )
   {
      if ( mIs_hWnd( m_pWndCtrl->m_hWnd ) == FALSE )
      {
         if ( m_pCtrlParent &&
              mIs_hWnd( m_pCtrlParent->m_pWndCtrl->m_hWnd ) == FALSE )
         {
            m_pCtrlParent->RealizeCtrl( TRUE );
         }

         CreateZ( );
         if ( m_wp.length == sizeof( WINDOWPLACEMENT ) &&
              mIs_hWnd( m_pWndCtrl->m_hWnd ))
         {
            m_pWndCtrl->SetWindowPlacement( &m_wp );
         }
      }

      m_wp.length = 0;
   }
   else
   {
      if ( m_pWndCtrl && mIs_hWnd( m_pWndCtrl->m_hWnd ) )
      {
         m_wp.length = sizeof( WINDOWPLACEMENT );
         m_pWndCtrl->GetWindowPlacement( &m_wp );
         DestroyZ( );
      }
   }
}

void
TZPainterCtrl::UndoRedo( zBOOL bToRedo )
{
}

void
TZPainterCtrl::SelectPoint( CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZPainterCtrl::SelectPoint", pt );
#endif
}

TZPainterCtrl *
TZPainterCtrl::IsDeletable( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::IsDeletable: ", m_csTag );
#endif

   // If this ctrl is visible and selected, it can be deleted.
   if ( GetVisibility( ) == 0 && IsSelected( ) )
      return( this );

   return( 0 );
}

void
TZPainterCtrl::SetupCtrlCover( )   // first chance the control has to know
{                                  // its corresponding pCtrlCover
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::SetupCtrlCover: ", m_csTag );
#endif
}

void
TZPainterCtrl::EndPaint( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::EndPaint: ", m_csTag );
#endif
}

TZPainterCtrl *
TZPainterCtrl::GetRealCtrl( )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterCtrl::GetRealCtrl IdNbr = ", m_lCtrlID );
#endif

   return( this );
}

#if 0
void
TZPainterCtrl::GetMoveRects( CPoint& pt, CRect& rectPainter,
                             CRect& rectOld, CRect& rectNew )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::GetMoveRects: ", m_csTag );
#endif

   CPoint ptOld;
   CPoint ptNew;
   CPoint ptScrollPos;

   ptScrollPos = m_pPainterWindow->m_pPainterClient->
                                                  GetDeviceScrollPosition( );
   pt.x -= ptScrollPos.x;
   pt.y -= ptScrollPos.y;

   ParentToPainterRect( rectPainter );

   // Erase the last rectangle drawn to represent where a control
   // would have been positioned on the last move
   ptOld = m_ptMouseSelect;
   ptNew = m_ptLastMouseMove;

   // Calculate the previous rectangle
   rectOld.left = ptNew.x - (ptOld.x - rectPainter.left) - ptScrollPos.x;
   rectOld.top = ptNew.y - (ptOld.y - rectPainter.top) - ptScrollPos.y;
   rectOld.right = rectOld.left + (rectPainter.right - rectPainter.left);
   rectOld.bottom = rectOld.top + (rectPainter.bottom - rectPainter.top);

   // Calculate the new rectangle to represent where the control will be
   // positioned if the user releases the mouse button at the given point.
   ptOld = m_ptMouseSelect;
   ptNew = pt;

   rectNew.left = ptNew.x - (ptOld.x - rectPainter.left) - ptScrollPos.x;
   rectNew.top = ptNew.y - (ptOld.y - rectPainter.top) - ptScrollPos.y;
   rectNew.right = rectNew.left + (rectPainter.right - rectPainter.left);
   rectNew.bottom = rectNew.top + (rectPainter.bottom - rectPainter.top);

// TraceRect( "TZPainterCtrl::GetMoveRects m_rectPainter", m_rectPainter );
// TraceRect( "TZPainterCtrl::GetMoveRects rectPainter", rectPainter );
// TraceRect( "TZPainterCtrl::GetMoveRects rectOld", rectOld );
// TraceRect( "TZPainterCtrl::GetMoveRects rectNew", rectNew );
}
#endif

zBOOL
TZPainterCtrl::IsChildValid( zLONG lChildId )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::IsChildValid: ", m_csTag );
#endif

   return( TRUE );
}

void
TZPainterCtrl::ReturnFromMaintenanceWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::ReturnFromMaintenanceWindow: ", m_csTag );
#endif
}

void
TZPainterCtrl::PreMoveCtrl( CRect& rect )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::PreMoveCtrl: ", m_csTag );
#endif
}

void
TZPainterCtrl::PostMoveCtrl( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::PostMoveCtrl: ", m_csTag );
#endif
}

TZPainterCtrl *
TZPainterCtrl::AdjustCtrlRect( CRect& rect,
                               zBOOL  bToPainterRect )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::AdjustCtrlRect: ", m_csTag );
#endif

   if ( IsModeValid( zCTRL_MODE_CLIENT ) )
   {
      m_pWndCtrl->GetClientRect( m_rectCtrl );
      m_rectPainter = m_rectCtrl; // No parent control
   }

   if ( bToPainterRect )
   {
      rect.left += m_rectCtrl.left;
      rect.top += m_rectCtrl.top;
      rect.right += m_rectCtrl.left;
      rect.bottom += m_rectCtrl.top;
   }
   else
   {
      rect.left -= m_rectCtrl.left;
      rect.top -= m_rectCtrl.top;
      rect.right -= m_rectCtrl.left;
      rect.bottom -= m_rectCtrl.top;
   }

   return( m_pCtrlParent );
}

zBOOL
TZPainterCtrl::IsModeValid( zULONG ulMode )
{
#ifdef DEBUG_ALL
// TraceLineS( "TZPainterCtrl::IsModeValid: ", m_csTag );
#endif

   return( FALSE );
}

void
TZPainterCtrl::DestroyZ( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZPainterCtrl::DestroyZ: ", m_csTag );
#endif

   m_pWndCtrl->DestroyWindow( );
}

zULONG
TZPainterCtrl::SetCommon( zULONG ulCommonFlags )
{
   ulCommonFlags &= (zCTRL_COMMON_INVISIBLE | zCTRL_COMMON_DISABLED |
                     zCTRL_COMMON_DISABLED_READONLY | zCTRL_COMMON_BORDER |
                     zCTRL_COMMON_SIZE_BORDER | zCTRL_COMMON_LEFT_JUSTIFY |
                     zCTRL_COMMON_RIGHT_JUSTIFY |
                     zCTRL_COMMON_CENTER_JUSTIFY |
                     zCTRL_COMMON_ABSOLUTE_PIXEL_SIZE |
                     zCTRL_COMMON_ABSOLUTE_PIXEL_POS |
                     zCTRL_COMMON_NO_TAB);
   return( ulCommonFlags );
}

void
TZPainterCtrl::WritePersistProperties( )
{
}

zBOOL
TZPainterCtrl::SetFontOverride( )
{
   if ( m_pPainterWindow->m_pBar->m_pFontCtrl )
   {
   // m_pWndCtrl->SetFont( m_pPainterWindow->m_pBar->m_pFontCtrl );
      m_pWndCtrl->SendMessage( WM_SETFONT,
          (WPARAM) (HFONT) *(m_pPainterWindow->m_pBar->m_pFontCtrl) );
      return( TRUE );
   }
   else
   if ( m_pPainterWindow->m_pBar->m_pFontWS )
   {
      m_pWndCtrl->SendMessage( WM_SETFONT,
             (WPARAM)(HFONT) *(m_pPainterWindow->m_pBar->m_pFontWS) );
      return( TRUE );
   }

   return( FALSE );
}

void
TZPainterCtrl::HideForUndo( zBOOL bHide )
{
   if ( m_pWndCtrl &&
        mIs_hWnd( m_pWndCtrl->m_hWnd ) )
   {
      m_pWndCtrl->ShowWindow( bHide ? SW_HIDE : SW_SHOW );
   }

   if ( m_pCtrlCover &&
        mIs_hWnd( m_pCtrlCover->m_hWnd ) )
   {
      m_pCtrlCover->ShowWindow( bHide ? SW_HIDE : SW_SHOW );
   }
}

/////////////////////////////////////////////////////////////////////////////
// TZTracker
//
// The control is being selected, create a tracking frame around it
//
// CRectTracker( LPCRECT lpSrcRect, UINT nStyle );
// Parameters:
//    lpSrcRect - The coordinates of the rectangle object.
//    nStyle - Specifies the style of the CRectTracker object.  The
//    following styles are supported:
//       CRectTracker::solidLine - solid line for rectangle border.
//       CRectTracker::dottedLine - dotted line for rectangle border.
//       CRectTracker::hatchedBorder - hatched pattern for rectangle border.
//       CRectTracker::resizeInside - resize handles inside rectangle.
//       CRectTracker::resizeOutside - resize handles outside rectangle.
//       CRectTracker::hatchInside - hatched pattern covers entire rectangle.
//
// BOOL Track( CWnd *pWnd, CPoint point,
//             BOOL bAllowInvert = FALSE,
//             CWnd *pWndClipTo = 0 );
//
// Return Value:
//    If the ESC key is pressed, the tracking process is halted, the
//    rectangle stored in the tracker is not altered, and 0 is returned.
//    If the change is committed, by moving the mouse and releasing the
//    left mouse button, the new position and/or size is recorded in the
//    tracker's rectangle and nonzero is returned.
//
// Parameters:
//    pWnd - The window object that contains the rectangle.
//    point - Device coordinates of the current mouse position relative
//            to the client area.
//    bAllowInvert - If TRUE, the rectangle can be inverted along the
//            x-axis or y-axis; otherwise FALSE.
//    pWndClipTo - The window that drawing operations will be clipped to.
//            If NULL, pWnd is used as the clipping rectangle.
//
// Remarks:
//    Call this function to display the user interface for resizing the
//    rectangle. This is usually called from inside the function of your
//    application that handles the WM_LBUTTONDOWN message (typically
//    OnLButtonDown).  This function will capture the mouse until the user
//    releases the left mouse button, presses the ESC key, or presses the
//    right mouse button. As the user moves the mouse cursor, the feedback
//    is updated by calling DrawTrackerRect and OnChangedRect.  If
//    bAllowInvert is TRUE, the tracking rectangle can be inverted on
//    either the x-axis or y-axis.
//
// BOOL TrackRubberBand( CWnd* pWnd, CPoint point,
//                       BOOL bAllowInvert = TRUE );
// Return Value:
//    Nonzero if the mouse has moved and the rectangle is not empty;
//    otherwise 0.
// Parameters:
//    pWnd - The window object that contains the rectangle.
//    point - Device coordinates of the current mouse position relative
//           to the client area.
//    bAllowInvert - If TRUE, the rectangle can be inverted along the
//            x-axis or y-axis; otherwise FALSE.
// Remarks:
//    Call this function to do rubber-band selection. It is usually
//    called from inside the function of your application that handles
//    the WM_LBUTTONDOWN message (typically OnLButtonDown).
//    This function will capture the mouse until the user releases the
//    left mouse button, presses the ESC key, or presses the right mouse
//    button.  As the user moves the mouse cursor, the feedback is
//    updated by calling DrawTrackerRect and OnChangedRect.
//    Tracking is performed with a rubber-band-type selection from the
//    lower-right handle.  If inverting is allowed, the rectangle can be
//    sized by dragging either up and to the left or down and to the right.
//
// void DrawTrackerRect( LPCRECT lpRect, CWnd *pWndClipTo,
//                       CDC *pDC, CWnd *pWnd );
// Parameters:
//    lpRect - Pointer to the RECT that contains the rectangle to draw.
//    pWndClipTo - Pointer to the window to use in clipping the rectangle.
//    pDC - Pointer to the device context on which to draw.
//    pWnd - Pointer to the window on which the drawing will occur.
//
// Remarks:
//    Called by the framework whenever the position of the tracker has
//    changed while inside the Track or TrackRubberBand member function.
//    The default implementation makes a call to CDC::DrawFocusRect, which
//    draws a dotted rectangle.
//
// Style displayed                     State of OLE item
// ------------------------------      ---------------------------------
// Dotted border                       Item is linked
// Solid border                        Item is embedded in your document
// Resize handles                      Item is currently selected
// Hatched border                      Item is currently in-place active
// Hatching pattern overlays item      Item's server is open
//
/////////////////////////////////////////////////////////////////////////////
TZTracker::TZTracker( LPCRECT pSrcRect, UINT uStyle,
                      CPoint& ptSrc, TZPainterCtrl *pCtrl ) :
           CRectTracker( pSrcRect, uStyle )
{
#ifdef DEBUG_ALL
   TracePoint( "TZTracker::ctor ptSrc", ptSrc );
#endif

   ASSERT( pCtrl );
   m_rectSrc = *pSrcRect;
   m_uStyle = uStyle;
   m_ptSrc = ptSrc;
   m_pCtrl = pCtrl;

   m_ptPrev.x = m_ptPrev.y = 0;
   m_pDC = 0;
   m_nHit = -1;
}

TZTracker::~TZTracker( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTracker::dtor", "" );
#endif

   if ( m_pCtrl && m_pCtrl->m_pCtrlCover && m_pCtrl->m_pCtrlCover->m_pSelectCover )
      m_pCtrl->m_pCtrlCover->m_pSelectCover->m_pTracker = 0;

   if ( m_pDC )
   {
      TraceLineS( "TZTracker::dtor unexpected DC", "" );
      mDeleteInit( m_pDC );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// OnChangedRect
//
// Parameters:
//    rectOld - Contains the old device coordinates of the CRectTracker
//              object.
//
// Remarks:
//    Called by the framework whenever the tracker rectangle has changed
//    during a call to Track.  At the time this function is called, all
//    feedback drawn with DrawTrackerRect has been removed.  The default
//    implementation of this function does nothing.  Override this function
//    when you want to perform any actions after the rectangle has been
//    resized.
//
/////////////////////////////////////////////////////////////////////////////
void
TZTracker::OnChangedRect( const CRect& rectOld )
{
#ifdef DEBUG_ALL
   TraceRect( "TZTracker::OnChangedRect", (RECT) rectOld );
#endif

   if ( m_rect == m_rectSrc && m_pDC == 0 )
      return;

   if ( m_nHit == 8 && m_pCtrl->m_pPainterWindow->m_nSelectCnt > 1 )
   {
//    TraceRect( "OnChangedRect", m_rect );
      CSize  size( TZCtrlCover::m_nFrameSizeX, TZCtrlCover::m_nFrameSizeY );
      TZPainterCtrl *pCtrl;
      CRect  rectNew;
      CRect  rectPrev;
      CPoint pt;

      if ( m_pDC == 0 )
         m_pDC = new CClientDC( m_pCtrl->m_pPainterWindow->m_pPainterClient );

      ASSERT( m_pDC );

      pt.x = m_rect.left - m_rectSrc.left;
      pt.y = m_rect.top - m_rectSrc.top;

      zSHORT nPos;
      for ( nPos = 0; nPos < m_pCtrl->m_pPainterWindow->m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_pCtrl->m_pPainterWindow->m_CtrlList[ nPos ];
         if ( pCtrl != m_pCtrl &&
              pCtrl->IsSelected( ) &&
              pCtrl->m_pCtrlParent == m_pCtrl->m_pCtrlParent )   //???
         {
            rectNew = pCtrl->m_rectPainter;
            // Adjust rect for Scroll Position.
            rectNew.OffsetRect( pt );
            rectNew.InflateRect( TZCtrlCover::m_nFrameSizeX,
                                 TZCtrlCover::m_nFrameSizeY );
//          TraceRect( "Track other selected", rectNew );

            CPoint ptScrollPos =
               pCtrl->m_pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
            rectNew.left -= ptScrollPos.x;
            rectNew.right -= ptScrollPos.x;
            rectNew.top -= ptScrollPos.y;
            rectNew.bottom -= ptScrollPos.y;

            rectPrev = pCtrl->m_rectPainter;
            rectPrev.OffsetRect( m_ptPrev );
            rectPrev.InflateRect( TZCtrlCover::m_nFrameSizeX,
                                  TZCtrlCover::m_nFrameSizeY );
            rectPrev.left -= ptScrollPos.x;
            rectPrev.right -= ptScrollPos.x;
            rectPrev.top -= ptScrollPos.y;
            rectPrev.bottom -= ptScrollPos.y;

            m_pDC->DrawDragRect( rectNew, size, rectPrev, size );
         }
      }

      m_ptPrev = pt;
   }
}

#if 1
void
TZTracker::DrawTrackerRect( LPCRECT lpRect, CWnd *pWndClipTo, CDC *pDC, CWnd *pWnd )
{
   CRect rect = *lpRect;
#ifdef DEBUG_ALL
   TraceLine( "TZTracker::DrawTrackerRect %s  WndClipTo: 0x%x  Wnd: 0x%x   DC: 0x%x",
              m_pCtrl->m_csTag, pWndClipTo, pWnd, pDC );
   TraceRect( "TZTracker::DrawTrackerRect: ", rect );
#endif

   CWnd *pWndDC;
   if ( pWndClipTo )
      pWndDC = pWndClipTo;
   else
      pWndDC = m_pCtrl->m_pPainterWindow;

   CWindowDC dc( pWndDC );
   pWndClipTo = pWndDC;
   pDC = &dc;

   // First, normalize the rectangle for drawing.
   rect.NormalizeRect( );

   // Convert to client coordinates ...
   if ( pWndClipTo )
   {
      pWnd->ClientToScreen( &rect );
      pWndClipTo->ScreenToClient( &rect );
   }

   CSize size( 0, 0 );
   if ( m_bFinalErase == FALSE )
   {
      // otherwise, size depends on the style
      if ( m_nStyle & hatchedBorder )
      {
         size.cx = size.cy = max( 1, GetHandleSize( rect ) - 1 );
         rect.InflateRect( size );
      }
      else
      {
         size.cx = CX_BORDER;
         size.cy = CY_BORDER;
      }
   }

   // and draw it.
   if ( m_bFinalErase || m_bErase == FALSE )
      pDC->DrawDragRect( rect, size, m_rectLast, m_sizeLast );

   // Remember last rectangles.
   m_rectLast = rect;
   m_sizeLast = size;
}

#else

void
TZTracker::DrawTrackerRect( LPCRECT pRect, CWnd *pWndClipTo,
                            CDC *pDC, CWnd *pWnd )
{
#ifdef DEBUG_ALL
   CRect rect = pRect;
   TraceLine( "TZTracker::DrawTrackerRect WndClipTo: 0x%x  Wnd: 0x%x   DC: 0x%x",
              pWndClipTo, pWnd, pDC );
   TraceRect( "TZTracker::DrawTrackerRect: ", rect );
#endif

   CRectTracker::DrawTrackerRect( pRect, pWndClipTo, pDC, pWnd );
}
#endif

#if 0
UINT
TZTracker::GetHandleMask( ) const
{
#ifdef DEBUG_ALL
   TraceLineS( "TZTracker::GetHandleMask: ", m_csTag );
#endif

   if ( m_bHide == TRUE )
      return( 0 );
   else
      return( CRectTracker::GetHandleMask( ) );
}
#endif
/////////////////////////////////////////////////////////////////////////////
// TZSelectCover class implementation
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC( TZSelectCover, CWnd )

BEGIN_MESSAGE_MAP( TZSelectCover, CWnd )
   ON_WM_CREATE( )
   ON_WM_PAINT( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_MOUSEMOVE( )
   ON_WM_SETCURSOR( )
   ON_WM_KEYDOWN( )
END_MESSAGE_MAP( )

TZSelectCover::TZSelectCover( TZPainterCtrl *pCtrl )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZSelectCover::ctor: ", pCtrl->m_csTag );
#endif

   m_pCtrl = pCtrl;
   m_pCtrl->m_ulZOrder = m_pCtrl->m_pPainterWindow->m_ulZOrder++;
   m_pTracker = 0;
   m_nTrackerHandleSize = 6;
   m_bFromMouseMove = FALSE;
   m_bFirstPaint = TRUE;

   CRect  rect;

   rect = m_pCtrl->m_rectPainter;

   // Adjust rect for Scroll Position.
   CPoint ptScrollPos =
     m_pCtrl->m_pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
// TracePoint( "TZSelectCover::OnLButtonDown DeviceScrollPos", ptScrollPos );
   rect.left -= ptScrollPos.x;
   rect.right -= ptScrollPos.x;
   rect.top -= ptScrollPos.y;
   rect.bottom -= ptScrollPos.y;

// TraceRect( "TZSelectCover::ctor rect", rect );
   rect.InflateRect( m_nTrackerHandleSize, m_nTrackerHandleSize );
// TraceRect( "TZSelectCover::ctor Inflated rect", rect );

   CString cs = "$";  cs += m_pCtrl->m_csTag;

   TZPainterCtrl *pCtrlParent = m_pCtrl;
   CWnd  *pWndClient;
   zBOOL bClient = FALSE;

   while ( pCtrlParent )
   {
      if ( pCtrlParent->IsModeValid( zCTRL_MODE_CLIENT ) )
      {
         bClient = TRUE;
         break;
      }

      pCtrlParent = pCtrlParent->m_pCtrlParent;
   }

   if ( bClient )
      pWndClient = pCtrlParent->m_pWndCtrl;
   else
      pWndClient = m_pCtrl->m_pPainterWindow->m_pPainterClient;

   Create( 0, cs, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
           rect, pWndClient, -1 );

   // Place our SelectCover window on top of everything in the ZOrder
   SetWindowPos( &wndTop, 0, 0, 0, 0,  // dks Most?  2007.05.03
                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW );
}

TZSelectCover::~TZSelectCover( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZSelectCover::dtor: ", m_pCtrl->m_csTag );
#endif

   mDeleteInit( m_pTracker );
}

int
TZSelectCover::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZSelectCover::OnCreate: ", m_pCtrl->m_csTag );
#endif

   if ( (int) Default( ) == -1 )
      return( -1 );

   ModifyStyleEx( 0, WS_EX_TRANSPARENT | WS_EX_TOPMOST );

   CRect  rect;

   rect = m_pCtrl->m_rectPainter;
   rect.right -= (rect.left - m_nTrackerHandleSize);
   rect.bottom -= (rect.top - m_nTrackerHandleSize);
   rect.top = rect.left = m_nTrackerHandleSize;
// TraceRect( "TZSelectCover::OnCreate Track rect", rect );
// TracePoint( "TZSelectCover::OnCreate MouseSelect",
//             m_pCtrl->m_ptMouseSelect );
   m_pTracker = new TZTracker( rect,
                               CRectTracker::resizeOutside |
                               CRectTracker::hatchedBorder,
                               m_pCtrl->m_ptMouseSelect, m_pCtrl );
   m_pTracker->m_nHandleSize = m_nTrackerHandleSize;

   Invalidate( TRUE );
// TraceCtrl( "Selecting ctrl", m_pCtrl );

   return( 0 );
}

//
//  Determine if the current click is part of a double-click.
//  Return: TRUE  - current click is part of a double-click
//          FALSE - current click is not part of a double-click ...
//                  OR the specified window was not located
zBOOL
TZSelectCover::IsDoubleClick( zUSHORT uMessage, CPoint ptStart )
{
   MSG     msg;
   zULONG  ulInterval = 0;
   zLONG   lMessage = GetMessageTime( );
   zULONG  ulDblclkTime = GetDoubleClickTime( );

#ifdef DEBUG_ALL
   TraceLineI( "TZSelectCover::IsDoubleClick Time: ", ulDblclkTime );
   TraceLineI( "                      MessageTime: ", lMessage );
#endif

   while ( ulInterval < ulDblclkTime )
   {
      ulInterval = (zULONG) (GetCurrentTime( ) - lMessage);
      if ( ::PeekMessage( &msg, m_hWnd, uMessage, uMessage,
                          PM_NOREMOVE | PM_NOYIELD ) )
      {
      // TraceLineI( "TZSelectCover::IsDoubleClick TRUE: ", ulInterval );
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
         // TracePoint( "TZSelectCover::IsDoubleClick FALSE: ", pt );
         // ::TranslateMessage( &msg );
         // ::DispatchMessage( &msg );
            return( FALSE );
         }
      }
   }

   return( FALSE ); // Double-click has NOT occurred
}

void
TZSelectCover::TrackSelection( UINT uModKeys, CPoint pt,
                               zBOOL bFromMouseMove )
{
#ifdef DEBUG_ALL
   TracePoint( "TZSelectCover::TrackSelection", pt );
#endif

   // In case the ctrl is deselected, hold these values.
   TZPainterCtrl   *pCtrlSelect = m_pCtrl;
   TZCtrlCover     *pCtrlCover = m_pCtrl->m_pCtrlCover;
   TZPainterWindow *pPainterWindow = m_pCtrl->m_pPainterWindow;
   TZPainterClient *pPainterClient = pPainterWindow->m_pPainterClient;
   TZPainterCtrl   *pCtrl;
   zSHORT          nPos;

   pCtrlSelect->m_ptLastMouseMove.x = pt.x - m_nTrackerHandleSize;
   pCtrlSelect->m_ptLastMouseMove.y = pt.y - m_nTrackerHandleSize;
// TracePoint( "TZSelectCover::TrackSelection ptLastMouseMove",
//             pCtrlSelect->m_ptLastMouseMove );

   // Track the mouse when a ctrl has been selected by a left button click.
   if ( pCtrlSelect->IsSelected( ) )
   {
      CRect rect;

//    TraceCtrl( "Handling selected Ctrl", pCtrlSelect );

      if ( m_pTracker == 0 )
         return;

      m_bFromMouseMove = bFromMouseMove;
      m_pTracker->m_nHit = m_pTracker->HitTest( pt );
//    TraceRect( "RectTracker initial rect", m_pTracker->m_rect );
      // TrackerRect:  Node Numbers    GetHandleMask
      //
      //               0 -- 4 -- 1     1 -- 16 -- 2
      //               |         |     |          |
      //               7    8    5    128        32
      //               |         |     |          |
      //               3 -- 6 -- 2     8 -- 64 -- 4
      //
      // Note:  the following call to Track does not change the value of pt.
      zBOOL bTrack = FALSE;
      if ( m_pTracker->m_nHit >= 0 && m_pTracker->m_nHit <= 8 )
         bTrack = m_pTracker->Track( this, pt, FALSE, pPainterClient );

      if ( m_pTracker->m_pDC )
      {
         rect = m_pTracker->m_rect;    // hold the Tracker rect
         m_pTracker->m_rect = m_pTracker->m_rectSrc;
         m_pTracker->OnChangedRect( rect );
         m_pTracker->m_rect = rect;    // restore the Tracker rect
         mDeleteInit( m_pTracker->m_pDC );
         bTrack = TRUE;
      }
      else
      if ( bTrack == FALSE )
      {
         if ( (GetKeyState( VK_ESCAPE ) < 0) )
         {
            bFromMouseMove = TRUE;  // prevent selection processing
         }
         else
         {
            // Pass the click through to the real control
         }
      }

      CPoint ptOffset;

      ptOffset.x = m_pTracker->m_rect.left - m_nTrackerHandleSize;
      ptOffset.y = m_pTracker->m_rect.top - m_nTrackerHandleSize;

      if ( bTrack && m_pTracker->m_nHit == 8 &&
           pCtrlSelect->m_bCtrlCoverSelected )
      {
         // Mouse sensitivity checking, if the mouse has moved during the
         // click, make sure it has moved enough to do a "control move"
         // instead of interpreting it as a click
         if ( ptOffset.x < TZCtrlCover::m_nDragX &&
              ptOffset.x > -TZCtrlCover::m_nDragX &&
              ptOffset.y < TZCtrlCover::m_nDragY &&
              ptOffset.y > -TZCtrlCover::m_nDragY )
         {
            m_pTracker->m_rect.left = m_nTrackerHandleSize;
            m_pTracker->m_rect.top = m_nTrackerHandleSize;
            bTrack = FALSE;         // prevent move/size processing
            bFromMouseMove = TRUE;  // prevent selection processing
         }
      }

      zBOOL bCtrlKey = (GetKeyState( VK_CONTROL ) < 0);
      zBOOL bShiftKey = (GetKeyState( VK_SHIFT ) < 0);

      if ( bTrack )
      {
//       TraceRect( "RectTracker final", m_pTracker->m_rect );
         rect = pCtrlSelect->m_rectPainter;

         rect.left += ptOffset.x; // m_pTracker->m_rect.left - m_nTrackerHandleSize;
         rect.top += ptOffset.y;  // m_pTracker->m_rect.top - m_nTrackerHandleSize;
         rect.right = rect.left +
                  (m_pTracker->m_rect.right - m_pTracker->m_rect.left);
         rect.bottom = rect.top +
                  (m_pTracker->m_rect.bottom - m_pTracker->m_rect.top);

         pt.x += ptOffset.x; // m_pTracker->m_rect.left - m_nTrackerHandleSize; //??? needed?
         pt.y += ptOffset.y; // m_pTracker->m_rect.top - m_nTrackerHandleSize;  //???

         zSHORT nIdNbr;

         if ( pPainterWindow->m_pUndoCurr )
            nIdNbr = pPainterWindow->m_pUndoCurr->m_nIdNbr + 1;
         else
            nIdNbr = 1;

         // If MOVING or CLONING ... determine if new parent.
         if ( m_pTracker->m_nHit == 8 )
         {
            TZPainterCtrl *pNewParent;
            zLONG lNewType;
            zLONG lType;
            zBOOL bContinue = TRUE;

            if ( bCtrlKey )
            {
               // If we are trying to clone and there has been minimal
               // mouse movement, make sure the user intended for the
               // clone to occur.
               if ( ptOffset.x < TZCtrlCover::m_nDragX << 3 &&
                    -ptOffset.x < TZCtrlCover::m_nDragX << 3 &&
                    ptOffset.y < TZCtrlCover::m_nDragY << 3 &&
                    -ptOffset.y < TZCtrlCover::m_nDragY << 3 )
               {
                  if ( (ptOffset.x < 4 && -ptOffset.x < 4 &&
                        ptOffset.y < 4 && -ptOffset.y < 4) ||
                       OperatorPrompt( pPainterWindow->m_pBar->
                                                      m_pZSubtask->m_vDialog,
                                       "Copy selected controls",
                                       "OK to copy selected controls?", 1,
                                       zBUTTONS_YESNO,
                                       zRESPONSE_YES, 0 ) != zRESPONSE_YES )
                  {
                     bContinue = FALSE;
                  }
               }
            }

            if ( bContinue )
            {
               pPainterClient->MoveOtherSelected( pCtrlSelect, ptOffset,
                                                  nIdNbr );

            // TraceCtrl( "MOVING or CLONING", pCtrlSelect );

               lNewType = lType = pCtrlSelect->m_lType;
            // TraceLine( "1. Continue: %s  NewType: %d  Type: %d",
            //             bContinue ? "TRUE" : "FALSE", lNewType, lType );
               pNewParent = pCtrlSelect->ValidateParentObject( rect, lNewType,
                           bCtrlKey ? zCTRL_VALID_PAINT : zCTRL_VALID_MOVE );
            }

         // TraceLine( "2. Continue: %s  NewType: %d  Type: %d",
         //            bContinue ? "TRUE" : "FALSE", lNewType, lType );
            if ( bContinue == FALSE || lNewType == 0 || lNewType != lType )
            {
               ::MessageBeep( MB_ICONQUESTION );
            }
            else
            {
               // If the control key is down, Clone the controls rather than
               // moving them.
               if ( bCtrlKey )
               {
                  // CLONE the control!
                  if ( pNewParent )
                  {
                     TZPainterCtrl *pParent = pNewParent;

                     while ( pParent )
                     {
                        rect.top -= pParent->m_rectCtrl.top;
                        rect.left -= pParent->m_rectCtrl.left;
                        rect.bottom -= pParent->m_rectCtrl.top;
                        rect.right -= pParent->m_rectCtrl.left;
                        pParent = pParent->m_pCtrlParent;
                     }
                  }

                  zCHAR szLabel[ 64 ];
                  TZPainterCtrl *pNewCtrl;

                  sprintf_s( szLabel, sizeof( szLabel ), "&xxdo Clone %d item%s\tCtrl+x",
                            pPainterWindow->m_nSelectCnt,
                            pPainterWindow->m_nSelectCnt == 1 ? "" : "s" );
                  CPoint ptScrollPos = pPainterWindow->
                                m_pPainterClient->GetDeviceScrollPosition( );
                  pPainterWindow->m_pPainterClient->
                                          ScrollToPosition( CPoint( 0, 0 ) );
                  pNewCtrl = pCtrlSelect->Clone( pNewParent, rect,
                                                 nIdNbr, szLabel );
                  if ( pNewCtrl )
                     pNewCtrl->m_uVisible = 0;  // visible by default

                  pPainterWindow->m_pPainterClient->
                                            ScrollToPosition( ptScrollPos  );
               }
               else
               {
                  // MOVE the control!
                  pCtrlSelect->m_pCtrlCover->MoveCtrl( rect, pNewParent, nIdNbr );
                  pCtrlSelect->m_uVisible = 0;  // visible by default
               }

               if ( pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT) )
               {
                  pPainterClient->AdjustGroupSets( );
               }
            }
         }
         else  // SIZING ... not moving or cloning
         {
//          TraceRect( "RectTracker for SIZING", m_pTracker->m_rect );
//          TraceCtrl( "SIZING", pCtrlSelect );
            pCtrlSelect->m_pCtrlCover->MoveCtrl( rect, (TZPainterCtrl *) -1, nIdNbr );
         }

      // pPainterWindow->SetOverlapVisibility( );  needs more work
      }
      else
      if ( bFromMouseMove == FALSE )
      {
#if 1
         CPoint ptOrig = pt;

         // Adjust the point into PainterClient coords.
         rect.left = pt.x;
         rect.top = pt.y;

         pCtrl = m_pCtrl;
         while ( pCtrl )
            pCtrl = pCtrl->AdjustCtrlRect( rect, TRUE );

         // Adjust for scroll position.
         // CPoint ptScrollPos =
         //     pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
         // ptScrollPos.x = rect.left - ptScrollPos.x;
         // ptScrollPos.y = rect.top - ptScrollPos.y;

         // No adjustment is necessary since we now have "painter" coords.
         ptOrig.x = rect.left;
         ptOrig.y = rect.top;
//       TracePoint( "TZCtrlCover::TrackSelection ptOrig:", ptOrig );
#endif
         if ( bCtrlKey )
         {
            // If the Ctrl Key is pressed, toggle the selection state for
            // the ctrl whose top is closest to the click point.
//          TracePoint( "TZCtrlCover::TrackSelection", ptScrollPos );

            zLONG lSelectPos = -1;
            zLONG lSelectOne = 64000;  // we are going to select the one whose top
                                       // is closest to the selection point.
            for ( nPos = 0; nPos < pPainterWindow->m_nCtrlCnt; nPos++ )
            {
               pCtrl = pPainterWindow->m_CtrlList[ nPos ];
               if ( pCtrl->GetVisibility( ) ) // not visible/selectable
                  continue;

               pCtrl->ParentToPainterRect( rect );
//             TraceRect( pCtrl->m_csTag, rect );
               if ( (ptOrig.x >= rect.left &&
                     ptOrig.x <= rect.right &&
                     ptOrig.y >= rect.top &&
                     ptOrig.y <= rect.bottom) )
               {
                  if ( ptOrig.y - rect.top <= lSelectOne )
                  {
                     lSelectOne = ptOrig.y - rect.top;
                     lSelectPos = nPos;
                  }
               }
            }

//          TraceLineI( "lSelectOne = ", lSelectOne );
            if ( lSelectOne >= 0 && lSelectPos >= 0 )
               pCtrl = pPainterWindow->m_CtrlList[ lSelectPos ];
            else
               pCtrl = pCtrlSelect;

//          TraceLineS( "Selected ctrl: ", pCtrl->m_csTag );

            // Find the childmost ctrl without regard to its select state.
//x         pCtrl = pCtrlSelect->m_pCtrlCover->
//x                     FindChildmostUnselected( pCtrlSelect, ptOrig, TRUE );
//x         if ( pCtrl == 0 )
//x            pCtrl = pCtrlSelect;

            if ( pCtrl->IsSelected( ) )
               pCtrl->Deselect( );
            else
               pCtrl->Select( );

            pPainterWindow->SetDIL_Text( );
         }
         else
         {
            zBOOL bAlreadySelected = pCtrlSelect->IsSelected( );

            // This is where we locate the next control to be selected!!!
            if ( bAlreadySelected == FALSE )
            {
               // Let's see if we can find a child of this control that
               // is not selected that could be selected.
               pCtrl = pCtrlSelect->m_pCtrlCover->
                       FindChildmostUnselected( pCtrlSelect, ptOrig, FALSE );
               if ( pCtrl )
               {
                  bAlreadySelected = FALSE;
                  pPainterWindow->Select( pCtrl );
               }
               else
               {
                  bAlreadySelected = pPainterWindow->Select( pCtrlSelect );
                  pCtrl = pCtrlSelect;
               }
            }
            else
            {
               bAlreadySelected = pPainterWindow->Select( pCtrlSelect );
               pCtrl = pCtrlSelect;
            }

//          TraceCtrl( "Current selected control", pCtrl );

            // There is one check we want to do first.  If we are on a
            // ctrl that is a congruent sibling (same parent, position,
            // and size of another ctrl), we want to move to the next
            // congruent sibling.  We have to enable/disable appropriately.
            if ( pCtrlSelect->m_uVisible & 0x0080 ) // if congruent sibling
            {
               pCtrl = pCtrlSelect->m_pCtrlCover->
                    FindChildmostUnselected( pCtrlSelect, ptOrig, FALSE );
               if ( pCtrl == 0 || pCtrl == pCtrlSelect )  // not on child
               {
                  for ( nPos = 0; nPos < pPainterWindow->m_nCtrlCnt; nPos++ )
                  {
                     pCtrl = pPainterWindow->m_CtrlList[ nPos ];
                     if ( pCtrl == pCtrlSelect )
                        break;
                  }

                  if ( nPos < pPainterWindow->m_nCtrlCnt )
                  {
                     zSHORT k = nPos;

                     while ( TRUE )
                     {
                        k++;
                        if ( k >= pPainterWindow->m_nCtrlCnt )
                           k = 0;

                        if ( k == nPos )
                        {
                           // Punt ... something 'interesting' has occurred.
                           TraceLineS( "TZSelectCover::TrackSelection PUNT: ",
                                        pCtrlSelect->m_csTag );
                           pPainterWindow->SetOverlapVisibility( );
                           pCtrl = pCtrlSelect;
                       //  m_pCtrl = pCtrl;
                           break;      // process normally
                        }

                        pCtrl = pPainterWindow->m_CtrlList[ k ];
                        if ( (pCtrl->m_uVisible & 0x0010) &&
                             pCtrl->m_pCtrlParent == pCtrlSelect->m_pCtrlParent &&
                             pCtrl->m_rectCtrl == pCtrlSelect->m_rectCtrl )
                        {
                           pCtrlSelect->m_uVisible &= ~0x0080;
                        // pCtrlSelect->m_uVisible |= 0x0010;
                           pCtrlSelect->SetVisibility( 0x0010, 0, 1 );
                           pPainterWindow->SetChildOverlapVisibility( pCtrlSelect,
                                                                      FALSE );
                           pCtrl->m_uVisible &= ~0x0010;
                        // pCtrl->m_uVisible |= 0x0080;
                           pCtrl->SetVisibility( 0x0080, 0, 1 );
                           pPainterWindow->SetChildOverlapVisibility( pCtrl,
                                                                      TRUE );
                           pCtrlSelect = pCtrl;
                        // m_pCtrl = pCtrl;
                        // pPainterWindow->UpdateZeidonWindow( );
                        // pPainterWindow->PaintZeidonWindow( FALSE );

                           for ( nPos = 0; nPos < pPainterWindow->m_nCtrlCnt; nPos++ )
                           {
                              pCtrl = pPainterWindow->m_CtrlList[ nPos ];
                              if ( pCtrl->m_lType == 2090 )
                              {
                                 pCtrl->EndPaint( );
#if 0
                                 TZNoteBook *pNoteBook = DYNAMIC_DOWNCAST( TZNoteBook, pCtrl->m_pWndCtrl );
                                 if ( pNoteBook && pCtrl->GetVisibility( ) == 0 )
                                 {
                                    TZNotePage *pNotePage = pNoteBook->GetActivePage( );
                                    if ( pNotePage )
                                       pNoteBook->ActivateTab( pNotePage->m_nTabNbr );
                                    else
                                    if ( pNoteBook->m_nTabNbrMax > 1 )
                                       pNoteBook->ActivateTab( 1 );
                                 }
#endif
                              }
                           }

                           pPainterWindow->Select( pCtrlSelect );
                           if ( pCtrl->m_pWndCtrl &&
                                mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
                           {
                              pCtrl->m_pWndCtrl->SetWindowPos( &CWnd::wndTop,
                                                               0, 0, 0, 0,
                                                               SWP_NOSIZE | SWP_NOMOVE );
                              pCtrl->m_pWndCtrl->Invalidate( );
                              pCtrl->m_pWndCtrl->UpdateWindow( );
                           }

                           pPainterWindow->SetDIL_Text( );
                           goto TRACK_SELECTION_END_label;
                        }
                     }
                  }
               }
            }

            // If the control was already selected, then see if there is
            // another control underneath this control which the user is
            // attempting to get to.  To do this, we find the current
            // control in the control list and search forward from there.
            // If not found, we loop back on the beginning of the list.
            if ( bAlreadySelected )
            {
               // Find this control in the list and proceed from there.
               zSHORT nIncr = 1;
               nPos = pPainterWindow->m_nCtrlCnt;

               // ptOrig has been set to "painter" coords.
#if 0
               CPoint ptOrig = pt;
               CPoint ptScrollPos = pPainterClient->GetDeviceScrollPosition( );

               // Get back to original values for checking location.
               ptOrig.x += ptScrollPos.x;
               ptOrig.y += ptScrollPos.y;

               // Now get the true offset of this point within the frame.
               pCtrl = pCtrlSelect;
               while ( pCtrl )
               {
                  ptOrig.x += pCtrl->m_rectCtrl.left;
                  ptOrig.y += pCtrl->m_rectCtrl.top;
                  pCtrl = pCtrl->m_pCtrlParent;
               }
#endif
               while ( nPos-- )
               {
                  if ( pPainterWindow->m_CtrlList[ nPos ] == pCtrlSelect )
                  {
                     // If the point is still within this control, start
                     // from this control and go backwards.  Otherwise,
                     // start at the end of the list and go backwards.
                     pCtrlSelect->ParentToPainterRect( rect );
                     if ( ptOrig.x < rect.left    ||
                          ptOrig.x > rect.right   ||
                          ptOrig.y < rect.top     ||
                          ptOrig.y > rect.bottom )
                     {
                        nPos = pPainterWindow->m_nCtrlCnt - 1;
                     }

                     nIncr = -1;
                     break;
                  }
               }

               if ( nPos >= 0 )
               {
                  TZPainterCtrl *pParentSelect;
                  TZPainterCtrl *pParent;
                  CRect rectCurrent;

                  pParentSelect = pCtrlSelect;
                  while ( pParentSelect )
                  {
                     if ( pParentSelect->IsModeValid( zCTRL_MODE_CLIENT ) )
                        break;

                     pParentSelect = pParentSelect->m_pCtrlParent;
                  }

                  while ( TRUE )
                  {
                     // Increment/decrement the list iterator
                     nPos += nIncr;
                     if ( nPos >= pPainterWindow->m_nCtrlCnt )
                        nPos = 0;
                     else
                     if ( nPos < 0 )
                        nPos = pPainterWindow->m_nCtrlCnt - 1;

                     pCtrl = pPainterWindow->m_CtrlList[ nPos ];

                     // Break out when we've wrapped back around to this ctrl.
                     if ( pCtrl == pCtrlSelect )
                     {
                     // TraceCtrl( "WrapBack Checking ctrl", pCtrl );
                        break;
                     }

                     // Skip any controls that are not visible (or not
                     // selectable).
                     if ( pCtrl->GetVisibility( ) )
                        continue;

                     // If the currently selected control and this control do
                     // not have the same greatest ancestor, skip it.
                     pParent = pCtrl;
                     while ( pParent )
                     {
                        if ( pParent->IsModeValid( zCTRL_MODE_CLIENT ) )
                           break;

                        pParent = pParent->m_pCtrlParent;
                     }

                     if ( pParent != pParentSelect )
                        continue;

//                   TraceCtrl( "??? Checking control", pCtrl );
//                   TraceLineI( "   Checking control at pos: ", nPos );
//                   TraceLineX( "========> Visibility = ",
//                               pCtrl->m_uVisible );

                     {
                        pCtrl->ParentToPainterRect( rect );
                        if ( ptOrig.x >= rect.left &&
                             ptOrig.x <= rect.right &&
                             ptOrig.y >= rect.top &&
                             ptOrig.y <= rect.bottom )
                        {
//                         TraceCtrl( "~~~~~~~~~> Located control", pCtrl );
//                         TraceRect( "=========> Located control rect",
//                                    rect );

                           pPainterWindow->Select( pCtrl );
                           pCtrlSelect = pCtrl;
                           if ( pCtrl->m_pWndCtrl &&
                                mIs_hWnd( pCtrl->m_pWndCtrl->m_hWnd ) )
                           {
                              pCtrl->m_pWndCtrl->SetWindowPos( &CWnd::wndTop,
                                                               0, 0, 0, 0,
                                                               SWP_NOSIZE | SWP_NOMOVE );
                              pCtrl->m_pWndCtrl->Invalidate( );
                              pCtrl->m_pWndCtrl->UpdateWindow( );
                           }

                           pPainterWindow->SetDIL_Text( );
                           break;
                        }
                     }
                  }  // end of:  while searching after current control
               }  // end of:  if Find this control in list for search
            }  // end of:  if this control was already selected
         }
      }
   }
   else
   {
      TracePoint( "TZSelectCover::TrackSelection can't believe we are here",
                  pt );
   }

TRACK_SELECTION_END_label:

#ifdef DEBUG_ALL
   zCHAR szMsg[ 256 ];

   TraceLineI( "TZSelectCover::TrackSelection (after) cnt: ",
               pPainterWindow->m_nCtrlCnt );
   for ( nPos = 0; nPos < pPainterWindow->m_nCtrlCnt; nPos++ )
   {
      pCtrl = pPainterWindow->m_CtrlList[ nPos ];
   // while ( pCtrl )
      {
      // if ( pCtrl->m_lType == 2090 || pCtrl->m_lType == 2091 )
      // if ( pCtrl->m_pCtrlParent &&
      //      zstrcmp( pCtrl->m_pCtrlParent->m_csTag, "Candidates" ) == 0 )
      //      zstrcmp( pCtrl->m_csTag, "ClientActivity" ) == 0 )
         {
      //    pCtrl = pPainterWindow->m_CtrlList[ nPos ];
            sprintf_s( szMsg, sizeof( szMsg ), " %3d.%s%-32s %-32s%s0x%04x:", nPos + 1,
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

   TraceCtrl( "PTC Selecting Ctrl", pCtrlSelect );

#endif

   pPainterWindow->MoveAllSelectedFramesToTop( );
   pCtrlSelect->m_bCtrlCoverSelected = FALSE;
}

void
TZSelectCover::OnLButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZSelectCover::OnLButtonDown", pt );
#endif

   if ( IsDoubleClick( WM_LBUTTONDBLCLK, pt ) == FALSE )
   {
      TZCtrlCover     *pCtrlCover = m_pCtrl->m_pCtrlCover;
      TZPainterWindow *pPainterWindow = m_pCtrl->m_pPainterWindow;

      if ( pPainterWindow->m_pBar->m_uSelectedPaletteItem == 0 ||
           pPainterWindow->m_pBar->
            m_PaletteBarItems[ pPainterWindow->m_pBar->
                                        m_uSelectedPaletteItem ].lKey == 0 )
      {
      // TracePoint( "TZSelectCover::OnLButtonDown Track", pt );
         TrackSelection( uModKeys, pt, FALSE );
      }
      else
      {
         // Deselect any controls that were selected.
      // TracePoint( "TZSelectCover::OnLButtonDown Pass to CtrlCover", pt );
         pPainterWindow->Select( 0 );
         pCtrlCover->OnLButtonDown( uModKeys, pt );
      }
   }
}

void
TZSelectCover::OnRButtonDown( UINT uModKeys, CPoint pt )
{
// The SwapMouseButton function reverses or restores the meaning of the
// left and right mouse buttons.
//    Parameters:
//       fSwap - Specifies whether the mouse button meanings are reversed
//               or restored.  If this parameter is TRUE, the left button
//               generates right-button messages and the right button
//               generates left-button messages.  If this parameter is FALSE,
//               the buttons are restored to their original meanings.
//
//    Return Values:
//       If the meaning of the mouse buttons was reversed previously,
//       before the function was called, the return value is nonzero.
//       If the meaning of the mouse buttons was not reversed, the
//       return value is zero.

#ifdef DEBUG_ALL
   TracePoint( "TZSelectCover::OnRButtonDown", pt );
#endif

   if ( (uModKeys & MK_CONTROL) == 0 && (uModKeys & MK_SHIFT) == 0 )
   {
   // ShowContextMenuForSelectedCtrls( pt );
      TraceLineS( "TZPainterClient::ShowContextMenu for ForSelectedCtrls: ",
                  m_pCtrl->m_csTag );
      CreateTrackingPopupMenu( m_pCtrl->m_pPainterWindow->m_pBar->m_vSubtask,
                               "UpdateSelectedCtrl", -1, -1, FALSE, FALSE );
   }
   else
   {
      TZPainterCtrl   *pCtrl = m_pCtrl;
      TZCtrlCover     *pCtrlCover = m_pCtrl->m_pCtrlCover;
      TZPainterWindow *pPainterWindow = m_pCtrl->m_pPainterWindow;
      zBOOL bModeValid = m_pCtrl->IsModeValid( zCTRL_MODE_DIRECT_EDIT );

      pPainterWindow->m_pPainterClient->m_bRButtonUp = TRUE;
      SwapMouseButton( !pPainterWindow->m_pBar->m_bMouseSwapped );

      ClientToScreen( &pt );
      pPainterWindow->m_pPainterClient->ScreenToClient( &pt );
      pPainterWindow->m_pPainterClient->OnLButtonDown( uModKeys, pt );

      if ( bModeValid )
      {
         CClientDC dc( pPainterWindow->m_pPainterClient );
         CRect     rect;

         pPainterWindow->Select( 0 );
         pCtrlCover->GetClientRect( &rect );
         pCtrlCover->ClientToScreen( &rect );
         pPainterWindow->m_pPainterClient->ScreenToClient( &rect );
         pCtrlCover->ShowWindow( SW_HIDE );
         pPainterWindow->m_pDirectModeCtrl = pCtrl;
         rect.InflateRect( 4, 4 );

         CPen *pPen = new CPen( PS_COSMETIC, 4, RGB( 0, 0, 0 ) );
         CPen *pPenOld = dc.SelectObject( pPen );
         CBrush *pBrushOld = (CBrush *) dc.SelectStockObject( HOLLOW_BRUSH );

         dc.Rectangle( rect );
         dc.SelectObject( pPenOld );
         dc.SelectObject( pBrushOld );
         delete( pPen );
      }
   }
}

void
TZSelectCover::OnRButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZSelectCover::OnRButtonUp", pt );
#endif
}

void
TZSelectCover::OnMouseMove( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
// TracePoint( "TZSelectCover::OnMouseMove", pt );
#endif

   if ( (uModKeys & MK_LBUTTON) && m_pCtrl->IsSelected( ) )
   {
      m_bFromMouseMove = TRUE;
   // TracePoint( "TZSelectCover::OnMouseMove", pt );
      TrackSelection( uModKeys, pt, TRUE );
   }
}

void
TZSelectCover::OnLButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZSelectCover::OnLButtonDblClk", pt );
#endif

   m_pCtrl->m_pCtrlCover->OnLButtonDblClk( uModKeys, pt );
}

void
TZSelectCover::OnLButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TracePoint( "TZSelectCover::OnLButtonUp", pt );
#endif
}

BOOL
TZSelectCover::OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "TZSelectCover::OnSetCursor: ", pCtrl->m_csTag );
#endif

   // forward to tracker
   if ( pWnd == this && m_pTracker && m_pTracker->SetCursor( this, uHitTest ) )
      return( TRUE );

   return( CWnd::OnSetCursor( pWnd, uHitTest, uMessage ) );
}

void
TZSelectCover::OnPaint( )
{
#ifdef DEBUG_ALL // lots of messages
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "TZSelectCover::OnPaint: %s ====>", m_pCtrl->m_csTag );
   TraceRect( szMsg, m_pCtrl->m_rectPainter );
#endif

   if ( IsWindow( m_hWnd ) == FALSE )
      return;

   SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0,  // NoTopMost dks 2007.05.01
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW );

   CPaintDC dc( this );   // a paint DC calls Begin and End Paint
   if ( m_pTracker )
      m_pTracker->Draw( &dc );
}

void
TZSelectCover::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZSelectCover::OnKeyDown: ", m_pCtrl->m_csTag );
#endif

   Default( );
}

/////////////////////////////////////////////////////////////////////////////
// TZRuler class implementation
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( TZRuler, CWnd )
BEGIN_MESSAGE_MAP( TZRuler, CWnd )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
   ON_WM_MOUSEMOVE( )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

// ctor
TZRuler::TZRuler( TZPainterWindow *pPainterWindow )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZRuler::ctor", "" );
#endif

   m_pPainterWindow = pPainterWindow;
   m_hCursor = 0;
   m_bTrack = FALSE;
   m_bCapture = FALSE;
   m_lLastX = -64000;

   CRect rect;

   rect.left = 0; // m_pPainterWindow->m_lLeftMargin;
   rect.top = 0;
   rect.right = (m_pPainterWindow->m_lLeftMargin * 2) +
                  mConvert256thInchToPixelX( m_pPainterWindow->m_lRptSizeX );
   rect.bottom = m_pPainterWindow->m_lTopMargin;

   Attr.X = 0;
   Attr.Y = 0;
   Attr.W = rect.right;
   Attr.H = rect.bottom;

   CreateEx( WS_EX_TOPMOST, 0, "Ruler", WS_CHILD | WS_VISIBLE, //| WS_BORDER,
             rect, pPainterWindow->m_pPainterClient, 0 );
   ShowWindow( SW_SHOW );
}

void
Draw3D_Border( CDC&   dc,
               CRect& rect,
               zSHORT nShadowSize,
               COLORREF colorShadow,
               COLORREF colorHilite,
               BOOL   bShadowIn,
               BOOL   bShadowButton )
{
#ifdef DEBUG_ALL
   TraceLineS( "Draw3D_Border", "" );
#endif

   CBrush *pBrushOld = 0;
   zSHORT k;

   // Paint top/left.
   {
      CBrush brush( bShadowIn ? colorShadow : colorHilite );
      pBrushOld = dc.SelectObject( &brush );

      for ( k = 0; k < nShadowSize; k++ )          // left edge
         dc.PatBlt( rect.left + k, rect.top, 1,
                    rect.bottom - rect.top - k, PATCOPY );

      for ( k = 0; k < nShadowSize; k++ )          // top edge
         dc.PatBlt( rect.left, rect.top + k,
                    rect.right - rect.left - k, 1, PATCOPY );
   }

   // Paint bottom/right.
   if ( bShadowButton == FALSE || bShadowIn == FALSE )
   {
      CBrush brush( bShadowIn ? colorHilite : colorShadow );
      dc.SelectObject( &brush );

      for ( k = 0; k < nShadowSize; k++ )          // right edge
         dc.PatBlt( rect.right - k - 1, rect.top + k, 1,
                    rect.bottom - rect.top - k, PATCOPY );

      for ( k = 0; k < nShadowSize; k++ )          // bottom edge
         dc.PatBlt( rect.left + k, rect.bottom - k - 1,
                    rect.right - rect.left - k, 1, PATCOPY );
   }

   dc.SelectObject( pBrushOld );
}

void
TZRuler::OnMouseMove( UINT uModKeys, CPoint pt )
{
   if ( m_bTrack )
   {
      CRect rect;

      GetClientRect( rect );
      CClientDC dc( this );
      int nOldROP2 = dc.SetROP2( R2_NOT );

      if ( m_lLastX != -64000 &&
           m_lLastX != pt.x )
      {
         dc.MoveTo( m_lLastX, 0 );
         dc.LineTo( m_lLastX, rect.bottom );
      }

      if ( m_lLastX != pt.x )
      {
         dc.MoveTo( pt.x, 0 );
         dc.LineTo( pt.x, rect.bottom );
         m_lLastX = pt.x;
      }

      dc.SetROP2( nOldROP2 );
      return;
   }

   CWnd *pWndChild = ChildWindowFromPoint( pt, CWP_SKIPINVISIBLE );
   if ( pWndChild == this &&
        pt.x <= m_pPainterWindow->m_lLeftMargin &&
        pt.x >= m_pPainterWindow->m_lLeftMargin - 3 )
   {
   // TracePoint( "TZRuler::OnMouseMove", pt );
      if ( m_bCapture == FALSE )
      {
         m_bCapture = TRUE;
         SetCapture( );
         m_hCursor = GetCursor( );
         HCURSOR hCursor = ::LoadCursor( 0, IDC_SIZEWE );
         SetCursor( hCursor );
      }
   }
   else
   if ( m_bCapture )
   {
      if ( m_hCursor )
      {
         SetCursor( m_hCursor );
         m_hCursor = 0;
      }
      else
      {
         HCURSOR hCursor = ::LoadCursor( 0, IDC_ARROW );
         SetCursor( hCursor );
      }

      ReleaseCapture( );
      m_bCapture = FALSE;
   }
}

void
TZRuler::OnLButtonDown( UINT uModKeys, CPoint pt )
{
   if ( m_bCapture )
   {
      m_bTrack = TRUE;
      ShowCursor( FALSE );
   }
}

void
TZRuler::OnLButtonUp( UINT uModKeys, CPoint pt )
{
   if ( m_bCapture )
   {
      m_bCapture = FALSE;
      if ( m_hCursor )
      {
         SetCursor( m_hCursor );
         m_hCursor = 0;
      }
      else
      {
         HCURSOR hCursor = ::LoadCursor( 0, IDC_ARROW );
         SetCursor( hCursor );
      }

      ReleaseCapture( );
   }

   if ( m_bTrack )
   {
      zLONG lLastX = m_lLastX;

      m_bTrack = FALSE;
      CRect rect;

      GetClientRect( rect );
      CClientDC dc( this );
      int nOldROP2 = dc.SetROP2( R2_NOT );

      if ( m_lLastX != -64000 )
      {
         dc.MoveTo( m_lLastX, 0 );
         dc.LineTo( m_lLastX, rect.bottom );
         m_lLastX = -64000;
      }

      dc.SetROP2( nOldROP2 );

      ShowCursor( TRUE );

      if ( lLastX != -64000 )
      {
         if ( lLastX > mConvert256thInchToPixelX( 1024 ) )  // four inches
            lLastX = mConvert256thInchToPixelX( 1024 );

         if ( lLastX < mConvert256thInchToPixelX( 128 ) )  // one-half inch
            lLastX = mConvert256thInchToPixelX( 128 );

         if ( m_pPainterWindow->m_lLeftMargin != lLastX )
         {
         // zCHAR szMsg1[ 256 ];
         // zCHAR szMsg2[ 256 ];

         // sprintf_s( szMsg1, "Old Margin: %ld", m_pPainterWindow->m_lLeftMargin );
            m_pPainterWindow->m_lLeftMargin = lLastX;
            m_pPainterWindow->PaintZeidonWindow( FALSE );

         // sprintf_s( szMsg2, "New Margin: %ld", lLastX );
         // ::MessageBox( 0, szMsg1, szMsg2, MB_OK );
         }
      }
   }
}

void
TZRuler::OnPaint( )
{
   CPaintDC dc( this );   // a paint DC calls Begin and End Paint
   CRect& rect = *(CRect *) &dc.m_ps.rcPaint;
   CPoint ptScrollPos =
              m_pPainterWindow->m_pPainterClient->GetDeviceScrollPosition( );
#if 0 //def DEBUG_ALL lots of messages
   TraceRect( "TZRuler::OnPaint", rect );
   TracePoint( "TZRuler::OnPaint Scroll:", ptScrollPos );
#endif

   if ( ptScrollPos.y > rect.bottom )
      return;

   int nOldBkMode = dc.SetBkMode( TRANSPARENT );
   CRect rectOrig = rect;
   CBrush brush( ::GetSysColor( COLOR_3DFACE ) );
   COLORREF colorShadow( ::GetSysColor( COLOR_3DSHADOW ) );
   COLORREF colorHilite( ::GetSysColor( COLOR_3DHIGHLIGHT ) );

   rect.top = 0;
   rect.left = 0;
   rect.bottom = Attr.H;
   rect.right = Attr.W;

   // Create 3D look.
   dc.Rectangle( rect );
   dc.FillRect( rect, &brush );
   Draw3D_Border( dc, rect, 2, colorShadow, colorHilite, FALSE, TRUE );

   zSHORT k;
   zLONG  lSixteenthX = mConvert256thInchToPixelX( 16 );
   zLONG  lSixteenthY = mConvert256thInchToPixelY( 16 );
   zLONG  lHeight;
   CPen   penGray( PS_SOLID, 1, colorHilite );
   CPen   penWideBlack( PS_INSIDEFRAME, 3, RGB( 0, 0, 0 ) );
   CPen   penBlack( PS_SOLID, 1, RGB( 0, 0, 0 ) );
   CPen   *pPenOld = dc.SelectObject( &penWideBlack );
   COLORREF colorOld = dc.SetTextColor( RGB( 0, 0, 0 ) );
   zSHORT nDigits;
   zCHAR  szText[ 3 ];

   szText[ 2 ] = 0;

   rect.left = m_pPainterWindow->m_lLeftMargin;
   rect.right = rect.left +
                  mConvert256thInchToPixelX( m_pPainterWindow->m_lRptSizeX );
   dc.MoveTo( rect.left - 3, rect.top );
   dc.LineTo( rect.left - 3, rect.bottom );
   dc.SelectObject( &penGray );
   for ( k = 0; rect.left <= rect.right; k++ )
   {
      if ( k % 16 == 0 )
         lHeight = lSixteenthY * 3;              // 16;
      else
      if ( k % 8 == 0 )
         lHeight = lSixteenthY * 2;              // 8;
      else
      if ( k % 4 == 0 )
         lHeight = lSixteenthY * 1;              // 4;
      else
      if ( k % 2 == 0 )
         lHeight = lSixteenthY / 2;              // 2;
      else
         lHeight = 0;

      if ( lHeight )
      {
         lHeight += 1 * lSixteenthY;
         dc.SelectObject( &penGray );
         dc.MoveTo( rect.left + 1, rect.bottom - (1 * lSixteenthY) );
         dc.LineTo( rect.left + 1, rect.bottom - lHeight );

         dc.SelectObject( &penBlack );
         dc.MoveTo( rect.left, rect.bottom - (1 * lSixteenthY) );
         dc.LineTo( rect.left, rect.bottom - lHeight );

         if ( k && k % 16 == 0 )
         {
            nDigits = k / 16;
            if ( nDigits < 10 )
            {
               szText[ 0 ] = '0' + (char) nDigits;
               nDigits = 1;
            }
            else
            {
               szText[ 0 ] = '0' + (char) (nDigits / 10);
               szText[ 1 ] = '0' + (char) (nDigits % 10);
               nDigits = 2;
            }

            szText[ nDigits ] = 0;
            dc.SetTextColor( colorHilite );
            dc.TextOut( rect.left + 5, rect.bottom - lHeight, szText, nDigits );
            dc.SetTextColor( RGB( 0, 0, 0 ) );
            dc.TextOut( rect.left + 4, rect.bottom - lHeight, szText, nDigits );
         }
      }

      rect.left = m_pPainterWindow->m_lLeftMargin +
                            mConvert256thInchToPixelX( (k + 1) * 16 );
   }

   rect.left = m_pPainterWindow->m_lLeftMargin;

   dc.SelectObject( &penGray );
   dc.MoveTo( rect.left + lSixteenthX, rect.bottom - (1 * lSixteenthY) + 4 );
   dc.LineTo( rect.right - lSixteenthX, rect.bottom - (1 * lSixteenthY) + 4 );
   dc.SelectObject( &penBlack );
   dc.MoveTo( rect.left + lSixteenthX, rect.bottom - (1 * lSixteenthY) + 5 );
   dc.LineTo( rect.right - lSixteenthX, rect.bottom - (1 * lSixteenthY) + 5 );

   dc.SetTextColor( colorOld );
   if ( pPenOld )
      dc.SelectObject( pPenOld );

   rect = rectOrig;
   dc.SetBkMode( nOldBkMode );
}

// dtor
TZRuler::~TZRuler( )
{
#ifdef DEBUG_ALL
   TraceLineS( "TZRuler::dtor", "" );
#endif

   DestroyWindow( );
}
