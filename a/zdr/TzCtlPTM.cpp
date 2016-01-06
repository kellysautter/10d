/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlptm.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of C 'wrappers' for Zeidon Painter Tools
// painter controls' messages.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 1999.02.08    DKS
//    Fixed rect positioning for reports.
//
// 1998.12.29    DKS
//    Added Shape message for reports.
//
// 1998.08.10    DKS
//    Warn user when a NoteBook control is being created that covers ctrls.
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

/////////////////////////////////////////////////////////////////////////////
//
//  C 'wrappers' for painter controls
//
/////////////////////////////////////////////////////////////////////////////
extern "C" {

zLONG OPERATION
TZTextMsg( zSHORT nMessage,
           TZPainterWindow *pPainterWindow,
           zLONG  lParentChildType,
           CWnd   *pWndParent,
           zVIEW  vCtrl,
           CRect& rect,
           TZPainterCtrl *pCtrl,
           zLONG  lType,
           zBOOL  bPlaceHolder,
           zLONG  lZKey,
           zLONG  lCtrlID,
           zLONG  lTabStopNbr,
           zPCHAR pchTag,
           zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZTextMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left + pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         return( (zLONG) (TZPainterCtrl *)
                 new TZText( pPainterWindow, pCtrl, pWndParent,
                             rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                             lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_pCtrlParent == 0 ||
               pCtrl->m_pCtrlParent->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         break;

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZCalendarMsg( zSHORT nMessage,
               TZPainterWindow *pPainterWindow,
               zLONG  lParentChildType,
               CWnd   *pWndParent,
               zVIEW  vCtrl,
               CRect& rect,
               TZPainterCtrl *pCtrl,
               zLONG  lType,
               zBOOL  bPlaceHolder,
               zLONG  lZKey,
               zLONG  lCtrlID,
               zLONG  lTabStopNbr,
               zPCHAR pchTag,
               zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZCalendarMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZCalendar( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZDayTimerMsg( zSHORT nMessage,
               TZPainterWindow *pPainterWindow,
               zLONG  lParentChildType,
               CWnd   *pWndParent,
               zVIEW  vCtrl,
               CRect& rect,
               TZPainterCtrl *pCtrl,
               zLONG  lType,
               zBOOL  bPlaceHolder,
               zLONG  lZKey,
               zLONG  lCtrlID,
               zLONG  lTabStopNbr,
               zPCHAR pchTag,
               zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZDayTimerMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZDayTimer( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

#define zCONTROLBAR_GRIPPERS          zCONTROL_RESERVED_UNUSABLE
#define zCONTROLBAR_LEFT              zCONTROL_FULL_CLIENT
#define zCONTROLBAR_BOTTOM            zCONTROL_CENTERJUSTIFY
#define zCONTROLBAR_RIGHT             zCONTROL_RIGHTJUSTIFY
#define zCONTROLBAR_TOP               zCONTROL_SPECIFIC_1
#define zCONTROLBAR_XX2               zCONTROL_SPECIFIC_2
#define zCONTROLBAR_XX3               zCONTROL_SPECIFIC_3

zLONG OPERATION
TZControlBarMsg( zSHORT nMessage,
                 TZPainterWindow *pPainterWindow,
                 zLONG  lParentChildType,
                 CWnd   *pWndParent,
                 zVIEW  vCtrl,
                 CRect& rect,
                 TZPainterCtrl *pCtrl,
                 zLONG  lType,
                 zBOOL  bPlaceHolder,
                 zLONG  lZKey,
                 zLONG  lCtrlID,
                 zLONG  lTabStopNbr,
                 zPCHAR pchTag,
                 zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPainterBarMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         zLONG lSubtype;

         if ( vCtrl )
            GetIntegerFromAttribute( &lSubtype, vCtrl,
                                     szlControl, szlSubtype );
         else
            lSubtype = 0;

         if ( lSubtype & zCONTROLBAR_GRIPPERS )
         {
            return( (zLONG) (TZPainterCtrl *)
                    new TZControlBarG( pPainterWindow, pCtrl, pWndParent,
                                       rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                       lCtrlID, pchTag, pchText ) );
         }
         else
         {
            return( (zLONG) (TZPainterCtrl *)
                    new TZControlBarC( pPainterWindow, pCtrl, pWndParent,
                                       rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                       lCtrlID, pchTag, pchText ) );
         }

      case zCTRLMSG_ACCEPT_PARENT:  // Accept no parent
         return( 0 );

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZEditMsg( zSHORT nMessage,
           TZPainterWindow *pPainterWindow,
           zLONG  lParentType,
           CWnd   *pWndParent,
           zVIEW  vCtrl,
           CRect& rect,
           TZPainterCtrl *pCtrl,
           zLONG  lType,
           zBOOL  bPlaceHolder,
           zLONG  lZKey,
           zLONG  lCtrlID,
           zLONG  lTabStopNbr,
           zPCHAR pchTag,
           zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZEditMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZEdit( pPainterWindow, pCtrl, pWndParent,
                             rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                             lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZGraphMsg( zSHORT nMessage,
            TZPainterWindow *pPainterWindow,
            zLONG  lParentType,
            CWnd   *pWndParent,
            zVIEW  vCtrl,
            CRect& rect,
            TZPainterCtrl *pCtrl,
            zLONG  lType,
            zBOOL  bPlaceHolder,
            zLONG  lZKey,
            zLONG  lCtrlID,
            zLONG  lTabStopNbr,
            zPCHAR pchTag,
            zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZGraphMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZGraph( pPainterWindow, pCtrl, pWndParent,
                              rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                              lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZPushMsg( zSHORT nMessage,
           TZPainterWindow *pPainterWindow,
           zLONG  /* lParentType */,
           CWnd   *pWndParent,
           zVIEW  vCtrl,
           CRect& rect,
           TZPainterCtrl *pCtrl,
           zLONG  lType,
           zBOOL  bPlaceHolder,
           zLONG  lZKey,
           zLONG  lCtrlID,
           zLONG  lTabStopNbr,
           zPCHAR pchTag,
           zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZPushMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZPush( pPainterWindow, pCtrl, pWndParent,
                             rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                             lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZBitmapMsg( zSHORT nMessage,
             TZPainterWindow *pPainterWindow,
             zLONG   /* lParentType */,
             CWnd   *pWndParent,
             zVIEW  vCtrl,
             CRect& rect,
             TZPainterCtrl *pCtrl,
             zLONG  lType,
             zBOOL  bPlaceHolder,
             zLONG  lZKey,
             zLONG  lCtrlID,
             zLONG  lTabStopNbr,
             zPCHAR pchTag,
             zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZBitmapMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left + pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         return( (zLONG) (TZPainterCtrl *)
                 new TZBitmap( pPainterWindow, pCtrl, pWndParent,
                               rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                               lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_pCtrlParent == 0 ||
               pCtrl->m_pCtrlParent->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         break;

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;


      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZTimerMsg( zSHORT nMessage,
            TZPainterWindow *pPainterWindow,
            zLONG   /* lParentType */,
            CWnd   *pWndParent,
            zVIEW  vCtrl,
            CRect& rect,
            TZPainterCtrl *pCtrl,
            zLONG  lType,
            zBOOL  bPlaceHolder,
            zLONG  lZKey,
            zLONG  lCtrlID,
            zLONG  lTabStopNbr,
            zPCHAR pchTag,
            zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZTimerMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         rect.bottom = rect.top + TZIcon::m_nIconPixelSizeY;
         rect.right = rect.left + TZIcon::m_nIconPixelSizeX;
         return( (zLONG) (TZPainterCtrl *)
                 new TZTimer( pPainterWindow, pCtrl, pWndParent,
                              rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                              lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
         rect.bottom = rect.top + TZIcon::m_nIconPixelSizeY;
         rect.right = rect.left + TZIcon::m_nIconPixelSizeX;
         break;

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZIconMsg( zSHORT nMessage,
           TZPainterWindow *pPainterWindow,
           zLONG   /* lParentType */,
           CWnd   *pWndParent,
           zVIEW  vCtrl,
           CRect& rect,
           TZPainterCtrl *pCtrl,
           zLONG  lType,
           zBOOL  bPlaceHolder,
           zLONG  lZKey,
           zLONG  lCtrlID,
           zLONG  lTabStopNbr,
           zPCHAR pchTag,
           zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZIconMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         rect.bottom = rect.top + TZIcon::m_nIconPixelSizeY;
         rect.right = rect.left + TZIcon::m_nIconPixelSizeX;

         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZIcon( pPainterWindow, pCtrl, pWndParent,
                             rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                             lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      case zCTRLMSG_SIZE:
         rect.bottom = rect.top + TZIcon::m_nIconPixelSizeY;
         rect.right = rect.left + TZIcon::m_nIconPixelSizeX;
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZRadioMsg( zSHORT nMessage,
            TZPainterWindow *pPainterWindow,
            zLONG  lParentType,
            CWnd   *pWndParent,
            zVIEW  vCtrl,
            CRect& rect,
            TZPainterCtrl *pCtrl,
            zLONG  lType,
            zBOOL  bPlaceHolder,
            zLONG  lZKey,
            zLONG  lCtrlID,
            zLONG  lTabStopNbr,
            zPCHAR pchTag,
            zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZRadioMsg msg: ", nMessage );
#endif

   zCHAR szBlob[ 2 ];

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( lType == zRADIOBUTTON_CONTROL )
         {
            return( (zLONG) (TZPainterCtrl *)
                 new TZRadio( pPainterWindow, pCtrl, pWndParent,
                              rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                              lCtrlID, pchTag, pchText ) );
         }
         else
         {
            return( (zLONG) (TZPainterCtrl *)
                 new TZGroupBox( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );
         }

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         if ( lType == zRADIOBUTTONGROUP_CONTROL )
         {
            if ( lParentType == zRADIOBUTTONGROUP_CONTROL )
            {
               return( zRADIOBUTTON_CONTROL );
            }
            else
            {
               return( lType );
            }
         }
         else
         if ( lParentType == zRADIOBUTTONGROUP_CONTROL )
         {
            return( lType );
         }

         return( 0 );

      case zCTRLMSG_ACCEPT_CHILD:   // if Child requested
         if ( lType == zRADIOBUTTONGROUP_CONTROL &&
                  (lParentType == zRADIOBUTTON_CONTROL ||
                   lParentType == zRADIOBUTTONGROUP_CONTROL) )
         {
            return( zRADIOBUTTON_CONTROL );
         }

         break;

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Blob
         szBlob[ 0 ] = 0;
         szBlob[ 1 ] = 0;
         SetAttributeFromBlob( vCtrl, szlControl, szlCtrlBOI, szBlob, 2 );

         // radio buttons and radio groups will be created with the border off
         pCtrl->m_lSubtype = zCONTROL_BORDEROFF;
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZCheckMsg( zSHORT nMessage,
            TZPainterWindow *pPainterWindow,
            zLONG  /* lParentType */,
            CWnd   *pWndParent,
            zVIEW  vCtrl,
            CRect& rect,
            TZPainterCtrl *pCtrl,
            zLONG  lType,
            zBOOL  bPlaceHolder,
            zLONG  lZKey,
            zLONG  lCtrlID,
            zLONG  lTabStopNbr,
            zPCHAR pchTag,
            zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZCheckMsg msg: ", nMessage );
#endif

   zCHAR szBlob[ 3 ];

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZCheck( pPainterWindow, pCtrl, pWndParent,
                              rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                              lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         szBlob[ 0 ] = '1';
         szBlob[ 1 ] = 0;
         szBlob[ 2 ] = 0;
         SetAttributeFromBlob( vCtrl, szlControl, szlCtrlBOI, szBlob, 3 );
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZComboBoxMsg( zSHORT nMessage,
               TZPainterWindow *pPainterWindow,
               zLONG  /* lParentType */,
               CWnd   *pWndParent,
               zVIEW  vCtrl,
               CRect& rect,
               TZPainterCtrl *pCtrl,
               zLONG  lType,
               zBOOL  bPlaceHolder,
               zLONG  lZKey,
               zLONG  lCtrlID,
               zLONG  lTabStopNbr,
               zPCHAR pchTag,
               zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZComboBoxMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZComboBox( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );

         // Create 4 extra mapping entities for control.
         pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZGroupBoxMsg( zSHORT nMessage,
               TZPainterWindow *pPainterWindow,
               zLONG  lParentType,
               CWnd   *pWndParent,
               zVIEW  vCtrl,
               CRect& rect,
               TZPainterCtrl *pCtrl,
               zLONG  lType,
               zBOOL  bPlaceHolder,
               zLONG  lZKey,
               zLONG  lCtrlID,
               zLONG  lTabStopNbr,
               zPCHAR pchTag,
               zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZGroupBoxMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZGroupBox( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_ACCEPT_CHILD:   // accept all children
         return( lParentType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         break;
   }

   return( 0 );
}

zLONG OPERATION
TZTabMsg( zSHORT nMessage,
          TZPainterWindow *pPainterWindow,
          zLONG  lParentType,
          CWnd   *pWndParent,
          zVIEW  vCtrl,
          CRect& rect,
          TZPainterCtrl *pCtrl,
          zLONG  lType,
          zBOOL  bPlaceHolder,
          zLONG  lZKey,
          zLONG  lCtrlID,
          zLONG  lTabStopNbr,
          zPCHAR pchTag,
          zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZTabMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
//       TraceLineI( "CTRLMSG_PAINT Tab Control id ", lType );
         if ( lType == zTAB_CONTROL )
         {
            TZPainterCtrl *pCurrCtrl;
            TZPainterCtrl *pCurrParent;
            CRect  rectCtrl;
            zSHORT nPos;

            // Determine if the Notebook ctrl is going to completely cover
            // any existing ctrls.  If so, warn the user.
            for ( nPos = 0; nPos < pPainterWindow->m_nCtrlCnt; nPos++ )
            {
               pCurrCtrl = pPainterWindow->m_CtrlList[ nPos ];
               pCurrCtrl->ParentToPainterRect( rectCtrl );
               if ( rect.left < rectCtrl.left &&
                    rect.right > rectCtrl.right &&
                    rect.top < rectCtrl.top &&
                    rect.bottom > rectCtrl.bottom )
               {
                  pCurrParent = pCurrCtrl;
                  while ( pCurrParent )
                  {
                     if ( pCurrParent == pCtrl || pCurrParent->m_lType == 2090 )  // Notebook?
                        break;

                     pCurrParent = pCurrParent->m_pCtrlParent;
                  }

                  if ( pCurrParent == 0 )
                  {
                     OperatorPrompt( pPainterWindow->m_pBar->
                                                       m_pZSubtask->m_vDialog,
                                     pCurrCtrl->m_csTag,
                                     "Tab Control Placement covering control!",
                                     1, zBUTTONS_OK, zRESPONSE_OK, 0 );
                     break;
                  }
               }
               else
               {
                  continue;  // no overlap yet
               }
            }

            return( (zLONG) (TZPainterCtrl *)
                    new TZNoteBook( pPainterWindow, pCtrl, pWndParent,
                                    rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                    lCtrlID, pchTag, pchText ) );
         }
         else
//       if ( lType == zTABCTL_CONTROL )
         {
            if ( pWndParent == 0 )
               return( 0 );

            TZNoteBook *pZNoteBook = DYNAMIC_DOWNCAST( TZNoteBook,
                                                       pWndParent );
            pWndParent->GetClientRect( rect );
            pZNoteBook->AdjustRect( FALSE, &rect );
            rect.bottom -= rect.top;
            rect.right -= rect.left;

            return( (zLONG) (TZPainterCtrl *)
                    new TZNotePage( pPainterWindow, pCtrl, pWndParent,
                                    rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                    lCtrlID, pchTag, pchText ) );
         }

      case zCTRLMSG_ACCEPT_CHILD:
//       TraceLineI( "CTRLMSG_ACCEPT_CHILD Tab Control id ", lType );
//       TraceLineI( "CTRLMSG_ACCEPT_CHILD Tab Child id ", lParentType );
         if ( lType == zTAB_CONTROL && lParentType == zTABCTL_CONTROL )
         {
            return( zTABCTL_CONTROL );
         }
         else
         if ( lType == zTABCTL_CONTROL )   // Accept all children
         {
            return( lParentType );
         }
         else
            return( 0 );

      case zCTRLMSG_ACCEPT_PARENT:
//       TraceLineI( "CTRLMSG_ACCEPT_PARENT Tab Control id ", lType );
//       TraceLineI( "CTRLMSG_ACCEPT_PARENT Tab Child id ", lParentType );

         // accept only a tab control as a parent
         if ( (lType == zTAB_CONTROL && lParentType == zTABCTL_CONTROL) ||
              (lType == zTABCTL_CONTROL && lParentType == zTAB_CONTROL) )
         {
            return( lType );
         }
         else
            return( 0 );

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
//       TraceLineI( "CTRLMSG_CREATE_ZEIDON Tab Control id ", lType );
         if ( lType == zTABCTL_CONTROL )
         {
            SetAttributeFromInteger( vCtrl, szlControl,
                                     szlType, pCtrl->m_lSubtype );
            SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                     zCONTROLX_DISABLE_READONLY );
//          TZNotePage *pZNotePage = DYNAMIC_DOWNCAST( TZNotePage,
//                                                     pCtrl->m_pWndCtrl );
//          SetAttributeFromInteger( vCtrl, szlControl, szlType,
//                                   pZNotePage->m_nTabIdx );
         }
         else
         if ( lType == zTAB_CONTROL )
         {
            SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                     zCONTROLX_DISABLE_READONLY );
            SetAttributeFromInteger( vCtrl, szlControl,
                                     szlSubtype, pCtrl->m_lSubtype );
//
//          // Create 1 extra mapping entity for control.
//          pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         }

         break;

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
      {
//       TraceLineI( "CTRLMSG_MOVE Tab Control id ", lType );
         if ( lType == zTABCTL_CONTROL )
         {
            if ( pCtrl )
               pCtrl->PreMoveCtrl( rect );
         }

         break;
      }

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
//       TraceLineI( "CTRLMSG_??? Message ID = ", nMessage );
         break;
   }

   return( 0 );
}

zLONG OPERATION
TZListBoxMsg( zSHORT nMessage,
              TZPainterWindow *pPainterWindow,
              zLONG  lParentType,
              CWnd   *pWndParent,
              zVIEW  vCtrl,
              CRect& rect,
              TZPainterCtrl *pCtrl,
              zLONG  lType,
              zBOOL  bPlaceHolder,
              zLONG  lZKey,
              zLONG  lCtrlID,
              zLONG  lTabStopNbr,
              zPCHAR pchTag,
              zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZListBoxMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( lType == zLISTBOX_CONTROL )
         {
            pCtrl = (TZPainterCtrl *)
                    new TZListBox( pPainterWindow, pCtrl, pWndParent,
                                   rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                   lCtrlID, pchTag, pchText );

            if ( vCtrl == 0 && pCtrl->m_vCtrl == 0 )
            {
               pCtrl->PositionOnZeidonCtrl( vCtrl );
               if ( pCtrl->m_vCtrl )
               {
                  SetAttributeFromInteger( pCtrl->m_vCtrl, "Control",
                                           "ExtendedStyle",
                                           zCONTROLX_DISABLE_READONLY );
               }
            }

            return( (zLONG) pCtrl );
         }
         else
         {
            rect.top    = 0;
            rect.bottom = 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );

            pCtrl = (TZPainterCtrl *)
                    new TZEdit( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText );
            pCtrl->m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;
            return( (zLONG) pCtrl );
         }

      case zCTRLMSG_MOVE:
      case zCTRLMSG_SIZE:
         if ( lType == zLISTCTL_CONTROL )
         {
            rect.top    = 0;
            rect.bottom = 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
         }

         return( lType );


      // If we get an accept child message for a listbox control and
      // the control type is LISTBOX, LISTCTL, EDIT, or TEXT, return
      // LISTCTL as the accepted child
      case zCTRLMSG_ACCEPT_CHILD:   // if Child requested
         if ( lType == zLISTBOX_CONTROL &&
              (lParentType == zLISTBOX_CONTROL ||
               lParentType == zLISTCTL_CONTROL ||
               lParentType == zEDIT_CONTROL ||
               lParentType == zTEXT_CONTROL) )
         {
            return( zLISTCTL_CONTROL );
         }
         else
            return( 0 );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         if ( lType == zLISTBOX_CONTROL ||
              (lType == zLISTCTL_CONTROL &&
              lParentType == zLISTBOX_CONTROL) )
         {
            return( lType );
         }
         else
            return( 0 );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         if ( lType == zLISTCTL_CONTROL )
         {
            SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
            SetAttributeFromInteger( vCtrl, szlControl,
                                     szlSubtype, pCtrl->m_lSubtype );
            SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                     zCONTROLX_DISABLE_READONLY );
         }
         else
         if ( lType == zLISTBOX_CONTROL )
         {
            SetAttributeFromInteger( vCtrl, szlControl,
                                     szlSubtype, pCtrl->m_lSubtype );
            SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                     zCONTROLX_DISABLE_READONLY );

            // Create 1 extra mapping entity for control.
            pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         }

         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZMLEditMsg( zSHORT nMessage,
             TZPainterWindow *pPainterWindow,
             zLONG  /* lParentType */,
             CWnd   *pWndParent,
             zVIEW  vCtrl,
             CRect& rect,
             TZPainterCtrl *pCtrl,
             zLONG  lType,
             zBOOL  bPlaceHolder,
             zLONG  lZKey,
             zLONG  lCtrlID,
             zLONG  lTabStopNbr,
             zPCHAR pchTag,
             zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZMLEditMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         return( (zLONG) (TZPainterCtrl *)
                 new TZMLEdit( pPainterWindow, pCtrl, pWndParent,
                               rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                               lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_pCtrlParent == 0 ||
               pCtrl->m_pCtrlParent->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         break;

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZDilMsg( zSHORT nMessage,
          TZPainterWindow *pPainterWindow,
          zLONG  lParentType,
          CWnd   *pWndParent,
          zVIEW  vCtrl,
          CRect& rect,
          TZPainterCtrl *pCtrl,
          zLONG  lType,
          zBOOL  bPlaceHolder,
          zLONG  lZKey,
          zLONG  lCtrlID,
          zLONG  lTabStopNbr,
          zPCHAR pchTag,
          zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZDilMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( lType == zDIL_CONTROL )
         {
            if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
            {
               zVIEW vWnd;

               GetViewByName( &vWnd, szlTZWINDOW,
                              pPainterWindow->m_pBar->m_vSubtask, zLEVEL_TASK );
               if ( vWnd )
               {
                  zBOOL bSetUpdatable = FALSE;

                  if ( MiGetUpdateForView( vWnd ) == 0 )
                  {
                     bSetUpdatable = TRUE;
                     SetViewUpdate( vWnd );
                  }

                  SetAttributeFromString( vWnd, szlWindow,
                                          szlDIL_Active, "Y" );

                  if ( bSetUpdatable )
                     SetViewReadOnly( vWnd );
               }
            }

            rect.bottom = rect.top + 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
            rect.bottom += 6;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 2 );

            return( (zLONG) (TZPainterCtrl *)
                    new TZDil( pPainterWindow, pCtrl, pWndParent,
                               rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                               lCtrlID, pchTag, pchText ) );
         }
         else
         {
            rect.top    = 2;
            rect.bottom = 3;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );

            return( (zLONG) (TZPainterCtrl *)
                    new TZDil( pPainterWindow, pCtrl, pWndParent,
                               rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                               lCtrlID, pchTag, pchText ) );
         }

      case zCTRLMSG_MOVE:
      case zCTRLMSG_SIZE:
         if ( lType == zDIL_CONTROL )
         {
            rect.bottom = rect.top + 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
            rect.bottom += 6;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 2 );
         }
         else
         {
            rect.top    = 2;
            rect.bottom = 3;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
         }

         return( lType );


      // If we get an accept child message for a listbox control and
      // the control type is LISTBOX, LISTCTL, EDIT, or TEXT, return
      // LISTCTL as the accepted child.
      case zCTRLMSG_ACCEPT_CHILD:   // if Child requested
         if ( lType == zDIL_CONTROL &&
              (lParentType == zDIL_CONTROL ||
               lParentType == zDILITEM_CONTROL ||
               lParentType == zEDIT_CONTROL ||
               lParentType == zTEXT_CONTROL) )
         {
            return( zDILITEM_CONTROL );
         }
         else
            return( 0 );

      case zCTRLMSG_ACCEPT_PARENT:
         if ( (lType == zDIL_CONTROL && lParentType == 0) ||
              (lType == zDILITEM_CONTROL && lParentType == zDIL_CONTROL) )
         {
            return( lType );
         }
         else
         if ( lType == zDIL_CONTROL && lParentType == zDIL_CONTROL )
            return( zDILITEM_CONTROL );
         else
            return( 0 );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) == 0 )
         {
            zVIEW vWnd;

            GetViewByName( &vWnd, szlTZWINDOW,
                           pPainterWindow->m_pBar->m_vSubtask, zLEVEL_TASK );
            if ( vWnd )
               SetAttributeFromString( vWnd, szlWindow, szlDIL_Active, "Y" );
         }

         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
      default:
         break;
   }

   return( 0 );
}

zLONG OPERATION
TZGridMsg( zSHORT nMessage,
           TZPainterWindow *pPainterWindow,
           zLONG  lParentType,
           CWnd   *pWndParent,
           zVIEW  vCtrl,
           CRect& rect,
           TZPainterCtrl *pCtrl,
           zLONG  lType,
           zBOOL  bPlaceHolder,
           zLONG  lZKey,
           zLONG  lCtrlID,
           zLONG  lTabStopNbr,
           zPCHAR pchTag,
           zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZGridMsg msg: ", nMessage );
#endif

   zVIEW  vTZPESRCO;
   zSHORT nRC;

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( lType == zGRID_CONTROL )
         {
            return( (zLONG) (TZPainterCtrl *)
                    new TZGrid( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText ) );
         }
         else
         if ( lType == zGRID_COMBO_CONTROL )
         {
            rect.top    = 0;
            rect.bottom = 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
            return( (zLONG) (TZPainterCtrl *)
                    new TZComboBox( pPainterWindow, pCtrl, pWndParent,
                                    rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                    lCtrlID, pchTag, pchText ) );
         }
         else
         if ( lType == zGRID_CHECK_CONTROL || lType == zCHECKBOX_CONTROL )
         {
            lType = zGRID_CHECK_CONTROL;
            rect.top    = 0;
            rect.bottom = 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
            return( (zLONG) (TZPainterCtrl *)
                    new TZCheck( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );
         }
         else
         {
            rect.top    = 0;
            if ( lType != zGRID_SUB_CONTROL )
               rect.bottom = 1;

            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
            return( (zLONG) (TZPainterCtrl *)
                    new TZEdit( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText ) );
         }

      case zCTRLMSG_NOTIFY_PARENT:
         if ( GetViewByName( &vTZPESRCO, "TZPESRCO",
                             pPainterWindow->m_pBar->m_vSubtask, zLEVEL_TASK ) > 0 )
         {
            nRC = zCURSOR_UNCHANGED;

            if ( lParentType == zGRID_SUB_CONTROL )
            {
               if ( lType == zGRID_COMBO_CONTROL || lType == zCOMBOBOX_CONTROL )
                  nRC = SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key",
                                                       zCOMBOBOX_CONTROL, 0 );
               else
               if ( lType == zGRID_EDIT_CONTROL || lType == zEDIT_CONTROL )
                  nRC = SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key",
                                                       zEDIT_CONTROL, 0 );
               else
               if ( lType == zGRID_CHECK_CONTROL || lType == zCHECKBOX_CONTROL )
                  nRC = SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key",
                                                       zCHECKBOX_CONTROL, 0 );
            }
            else
            {
               if ( lType == zCOMBOBOX_CONTROL )
                  nRC = SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key",
                                                       zGRID_COMBO_CONTROL, 0 );
               else
               if ( lType == zEDIT_CONTROL )
                  nRC = SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key",
                                                       zGRID_EDIT_CONTROL, 0 );
               else
               if ( lType == zCHECKBOX_CONTROL )
                  nRC = SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key",
                                                       zGRID_CHECK_CONTROL, 0 );
               else
               if ( lType == zGRID_SUB_CONTROL )
                  nRC = SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key",
                                                       zGRID_SUB_CONTROL, 0 );
            }

            if ( nRC >= zCURSOR_SET )
            {
               ExcludeEntity( vCtrl, "ControlDef", zREPOS_PREV );
               IncludeSubobjectFromSubobject( vCtrl, "ControlDef",
                                              vTZPESRCO, "ControlDef",
                                              zPOS_AFTER );
            }
         }

         break;

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );

         // Create 1 extra mapping entity for control.
         pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( lType != zGRID_CONTROL )
         {
            rect.bottom -= rect.top;
            rect.top = 0;
            if ( lType == zGRID_SUB_CONTROL )
            {
               TZPainterCtrl *pCtrlChild = pPainterWindow->GetChildCtrl( pCtrl, 0 );
               if ( pCtrlChild )
               {
                  pCtrlChild->m_rectCtrl.bottom = rect.bottom;
                  pCtrlChild->m_rectCtrl.top = 0;
                  pCtrlChild->m_rectCtrl.right = rect.right - rect.left;
                  pCtrlChild->m_rectCtrl.left = 0;

               // pPainterWindow->SendCtrlMsg( zCTRLMSG_MOVE, 0,
               //                              pCtrlChild->m_lType, lType,
               //                              pCtrlChild->m_bPlaceHolder,
               //                              &rect, 0, pCtrlChild );
               }
            }
         }

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZToolBarMsg( zSHORT nMessage,
              TZPainterWindow *pPainterWindow,
              zLONG  lParentType,
              CWnd   *pWndParent,
              zVIEW  vCtrl,
              CRect& rect,
              TZPainterCtrl *pCtrl,
              zLONG  lType,
              zBOOL  bPlaceHolder,
              zLONG  lZKey,
              zLONG  lCtrlID,
              zLONG  lTabStopNbr,
              zPCHAR pchTag,
              zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZToolBarMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         return( (zLONG) (TZPainterCtrl *)
                 new TZBitmap( pPainterWindow, pCtrl, pWndParent,
                               rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                               lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_pCtrlParent == 0 ||
               pCtrl->m_pCtrlParent->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         break;

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;


      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZOutlinerMsg( zSHORT nMessage,
               TZPainterWindow *pPainterWindow,
               zLONG  lParentType,
               CWnd   *pWndParent,
               zVIEW  vCtrl,
               CRect& rect,
               TZPainterCtrl *pCtrl,
               zLONG  lType,
               zBOOL  bPlaceHolder,
               zLONG  lZKey,
               zLONG  lCtrlID,
               zLONG  lTabStopNbr,
               zPCHAR pchTag,
               zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZOutlinerMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZTree( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );

         // Create 1 extra mapping entity for control.
         pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZERD_Msg( zSHORT nMessage,
           TZPainterWindow *pPainterWindow,
           zLONG  /* lParentType */,
           CWnd   *pWndParent,
           zVIEW  vCtrl,
           CRect& rect,
           TZPainterCtrl *pCtrl,
           zLONG  lType,
           zBOOL  bPlaceHolder,
           zLONG  lZKey,
           zLONG  lCtrlID,
           zLONG  lTabStopNbr,
           zPCHAR pchTag,
           zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZERD_Msg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZERD( pPainterWindow, pCtrl, pWndParent,
                            rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                            lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // Accept NO parents
//       return( lType );
         break;

      case zCTRLMSG_ACCEPT_CHILD:  // Accept NO children
//       return( lParentType );
         break;

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                  zCONTROLX_DISABLE_READONLY );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zLONG OPERATION
TZGroupSetRptMsg( zSHORT nMessage,
                  TZPainterWindow *pPainterWindow,
                  zLONG  lParentType,
                  CWnd   *pWndParent,
                  zVIEW  vCtrl,
                  CRect& rect,
                  TZPainterCtrl *pCtrl,
                  zLONG  lType,
                  zBOOL  bPlaceHolder,
                  zLONG  lZKey,
                  zLONG  lCtrlID,
                  zLONG  lTabStopNbr,
                  zPCHAR pchTag,
                  zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZGroupSetRptMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         if ( rect.top < pPainterWindow->m_lTopMargin )
            rect.top += pPainterWindow->m_lTopMargin;

         rect.left = 4;
         rect.right = pPainterWindow->m_lLeftMargin - 1 +
                    mConvert256thInchToPixelX( pPainterWindow->m_lRptSizeX );

         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZGroupSetRpt( pPainterWindow, pCtrl, pWndParent,
                                    rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                    lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
      {
         if ( rect.top < pPainterWindow->m_lTopMargin )
         {
            // temporarily use rect.left
            rect.left = pPainterWindow->m_lTopMargin - rect.top;
            rect.top += rect.left;
            rect.bottom += rect.left;
         }

         rect.left = 4;
         rect.right = pPainterWindow->m_lLeftMargin - 1 +
                    mConvert256thInchToPixelX( pPainterWindow->m_lRptSizeX );

         break;
      }

      case zCTRLMSG_CREATE_ZEIDON:  // Create Zeidon Control, empty Text
      {
         TZGroupSetRpt *pGroupSet = (TZGroupSetRpt *) pCtrl;
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl,
                                  "Flags", pGroupSet->m_lGrpSetFlags );
         break;
      }

      default:
         break;
   }

   return( 0 );
}

zLONG OPERATION
TZGroupRptMsg( zSHORT nMessage,
               TZPainterWindow *pPainterWindow,
               zLONG  lParentType,
               CWnd   *pWndParent,
               zVIEW  vCtrl,
               CRect& rect,
               TZPainterCtrl *pCtrl,
               zLONG  lType,
               zBOOL  bPlaceHolder,
               zLONG  lZKey,
               zLONG  lCtrlID,
               zLONG  lTabStopNbr,
               zPCHAR pchTag,
               zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZGroupRptMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:

         rect.left = 4;
         rect.right = pPainterWindow->m_lLeftMargin - 5 +
                    mConvert256thInchToPixelX( pPainterWindow->m_lRptSizeX );

         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZGroupRpt( pPainterWindow, pCtrl, pWndParent,
                                 rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                 lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
      {
         rect.left = 4;
         rect.right = pPainterWindow->m_lLeftMargin - 5 +
                    mConvert256thInchToPixelX( pPainterWindow->m_lRptSizeX );

         break;
      }

//    case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
//       return( lType );
//
//    case zCTRLMSG_ACCEPT_CHILD:   // accept all children
//       return( lParentType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
      {
         TZGroupRpt *pGroup = (TZGroupRpt *) pCtrl;
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         SetAttributeFromInteger( vCtrl, szlControl,
                                  "Flags", pGroup->m_lGrpFlags );
         break;
      }

      default:
         break;
   }

   return( 0 );
}

#if 0
zLONG OPERATION
TZTextRptMsg( zSHORT nMessage,
              TZPainterWindow *pPainterWindow,
              zLONG  lParentType,
              CWnd   *pWndParent,
              zVIEW  vCtrl,
              CRect& rect,
              TZPainterCtrl *pCtrl,
              zLONG  lType,
              zBOOL  bPlaceHolder,
              zLONG  lZKey,
              zLONG  lCtrlID,
              zLONG  lTabStopNbr,
              zPCHAR pchTag,
              zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZTextRptMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZTextRpt( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText ) );

//    case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
//       return( lType );
//
//    case zCTRLMSG_ACCEPT_CHILD:   // accept all children
//       return( lParentType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         break;

      default:
         break;
   }

   return( 0 );
}

zLONG OPERATION
TZMLE_RptMsg( zSHORT nMessage,
              TZPainterWindow *pPainterWindow,
              zLONG  lParentType,
              CWnd   *pWndParent,
              zVIEW  vCtrl,
              CRect& rect,
              TZPainterCtrl *pCtrl,
              zLONG  lType,
              zBOOL  bPlaceHolder,
              zLONG  lZKey,
              zLONG  lCtrlID,
              zLONG  lTabStopNbr,
              zPCHAR pchTag,
              zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZMLE_RptMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZMLE_Rpt( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText ) );

//    case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
//       return( lType );
//
//    case zCTRLMSG_ACCEPT_CHILD:   // accept all children
//       return( lParentType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         break;

      default:
         break;
   }

   return( 0 );
}

zLONG OPERATION
TZBMP_RptMsg( zSHORT nMessage,
              TZPainterWindow *pPainterWindow,
              zLONG  lParentType,
              CWnd   *pWndParent,
              zVIEW  vCtrl,
              CRect& rect,
              TZPainterCtrl *pCtrl,
              zLONG  lType,
              zBOOL  bPlaceHolder,
              zLONG  lZKey,
              zLONG  lCtrlID,
              zLONG  lTabStopNbr,
              zPCHAR pchTag,
              zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZBMP_RptMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         return( (zLONG) (TZPainterCtrl *)
                 new TZBMP_Rpt( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText ) );

//    case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
//       return( lType );
//
//    case zCTRLMSG_ACCEPT_CHILD:   // accept all children
//       return( lParentType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         break;

      default:
         break;
   }

   return( 0 );
}
#endif

zLONG OPERATION
TZShapeMsg( zSHORT nMessage,
           TZPainterWindow *pPainterWindow,
           zLONG  lParentChildType,
           CWnd   *pWndParent,
           zVIEW  vCtrl,
           CRect& rect,
           TZPainterCtrl *pCtrl,
           zLONG  lType,
           zBOOL  bPlaceHolder,
           zLONG  lZKey,
           zLONG  lCtrlID,
           zLONG  lTabStopNbr,
           zPCHAR pchTag,
           zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZShapeMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         return( (zLONG) (TZPainterCtrl *)
                 new TZShape( pPainterWindow, pCtrl, pWndParent,
                              rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                              lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_pCtrlParent == 0 ||
               pCtrl->m_pCtrlParent->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         break;

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZReportListMsg( zSHORT nMessage,
                 TZPainterWindow *pPainterWindow,
                 zLONG  lParentType,
                 CWnd   *pWndParent,
                 zVIEW  vCtrl,
                 CRect& rect,
                 TZPainterCtrl *pCtrl,
                 zLONG  lType,
                 zBOOL  bPlaceHolder,
                 zLONG  lZKey,
                 zLONG  lCtrlID,
                 zLONG  lTabStopNbr,
                 zPCHAR pchTag,
                 zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZReportListMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( lType == zLISTBOX_CONTROL )
         {
            pCtrl = (TZPainterCtrl *)
                    new TZListBox( pPainterWindow, pCtrl, pWndParent,
                                   rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                   lCtrlID, pchTag, pchText );

            if ( vCtrl == 0 && pCtrl->m_vCtrl == 0 )
            {
               pCtrl->PositionOnZeidonCtrl( vCtrl );
               if ( pCtrl->m_vCtrl )
               {
                  SetAttributeFromInteger( pCtrl->m_vCtrl, "Control",
                                           "ExtendedStyle",
                                           zCONTROLX_DISABLE_READONLY );
               }
            }

            return( (zLONG) pCtrl );
         }
         else
         {
            rect.top    = 0;
            rect.bottom = 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );

            pCtrl = (TZPainterCtrl *)
                    new TZEdit( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText );
            pCtrl->m_lSubtype = zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP;
            return( (zLONG) pCtrl );
         }

      case zCTRLMSG_MOVE:
      case zCTRLMSG_SIZE:
         if ( lType == zLISTCTL_CONTROL )
         {
            rect.top    = 0;
            rect.bottom = 1;
            PainterSnapRect( pPainterWindow->m_pBar, &rect, 0, 0, 1 );
         }

         return( lType );


      // If we get an accept child message for a listbox control and
      // the control type is LISTBOX, LISTCTL, EDIT, or TEXT, return
      // LISTCTL as the accepted child
      case zCTRLMSG_ACCEPT_CHILD:   // if Child requested
         if ( lType == zLISTBOX_CONTROL &&
              (lParentType == zLISTBOX_CONTROL ||
               lParentType == zLISTCTL_CONTROL ||
               lParentType == zEDIT_CONTROL ||
               lParentType == zTEXT_CONTROL) )
         {
            return( zLISTCTL_CONTROL );
         }
         else
            return( 0 );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         if ( lType == zLISTBOX_CONTROL ||
              (lType == zLISTCTL_CONTROL &&
              lParentType == zLISTBOX_CONTROL) )
         {
            return( lType );
         }
         else
            return( 0 );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         if ( lType == zLISTCTL_CONTROL )
         {
            SetAttributeFromString( vCtrl, szlControl, szlText, 0 );
            SetAttributeFromInteger( vCtrl, szlControl,
                                     szlSubtype, pCtrl->m_lSubtype );
            SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                     zCONTROLX_DISABLE_READONLY );
         }
         else
         if ( lType == zLISTBOX_CONTROL )
         {
            SetAttributeFromInteger( vCtrl, szlControl,
                                     szlSubtype, pCtrl->m_lSubtype );
            SetAttributeFromInteger( vCtrl, szlControl, "ExtendedStyle",
                                     zCONTROLX_DISABLE_READONLY );

            // Create 1 extra mapping entity for control.
            pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap, zPOS_AFTER );
         }

         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZReportListCtlMsg( zSHORT nMessage,
                    TZPainterWindow *pPainterWindow,
                    zLONG  lParentType,
                    CWnd   *pWndParent,
                    zVIEW  vCtrl,
                    CRect& rect,
                    TZPainterCtrl *pCtrl,
                    zLONG  lType,
                    zBOOL  bPlaceHolder,
                    zLONG  lZKey,
                    zLONG  lCtrlID,
                    zLONG  lTabStopNbr,
                    zPCHAR pchTag,
                    zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZReportListCtlMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         return( (zLONG) (TZPainterCtrl *)
                 new TZShape( pPainterWindow, pCtrl, pWndParent,
                              rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                              lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_pCtrlParent == 0 ||
               pCtrl->m_pCtrlParent->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         break;

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZBlockMsg( zSHORT nMessage,
            TZPainterWindow *pPainterWindow,
            zLONG  lParentChildType,
            CWnd   *pWndParent,
            zVIEW  vCtrl,
            CRect& rect,
            TZPainterCtrl *pCtrl,
            zLONG  lType,
            zBOOL  bPlaceHolder,
            zLONG  lZKey,
            zLONG  lCtrlID,
            zLONG  lTabStopNbr,
            zPCHAR pchTag,
            zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZBlockMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         return( (zLONG) (TZPainterCtrl *)
                 new TZShape( pPainterWindow, pCtrl, pWndParent,
                              rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                              lCtrlID, pchTag, pchText ) );
      // return( (zLONG) (TZPainterCtrl *)
      //         new TZGroupBox( pPainterWindow, pCtrl, pWndParent,
      //                         rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
      //                         lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_SIZE:
      case zCTRLMSG_MOVE:
         if ( (pPainterWindow->m_pBar->m_fState & (zTZCB_REPORT | zTZCB_XSLT)) &&
              (pCtrl == 0 || pCtrl->m_pCtrlParent == 0 ||
               pCtrl->m_pCtrlParent->m_chEntity != 'C') )
         {
            if ( rect.left < pPainterWindow->m_lLeftMargin - 8 )
            {
               rect.right = rect.right - rect.left +
                                           pPainterWindow->m_lLeftMargin - 8;
               rect.left = pPainterWindow->m_lLeftMargin - 8;
            }
         }

         break;

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         break;

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

zLONG OPERATION
TZActiveXMsg( zSHORT nMessage,
              TZPainterWindow *pPainterWindow,
              zLONG  lParentChildType,
              CWnd   *pWndParent,
              zVIEW  vCtrl,
              CRect& rect,
              TZPainterCtrl *pCtrl,
              zLONG  lType,
              zBOOL  bPlaceHolder,
              zLONG  lZKey,
              zLONG  lCtrlID,
              zLONG  lTabStopNbr,
              zPCHAR pchTag,
              zPCHAR pchText )
{
#ifdef DEBUG_ALL
   TraceLineI( "TZActiveXMsg msg: ", nMessage );
#endif

   switch ( nMessage )
   {
      case zCTRLMSG_CREATE_GUI:
         // Note that for zCTRLMSG_CREATE_GUI, pCtrl is the parent ctrl!!!
      // TraceLineS( "TZActiveXMsg zCTRLMSG_CREATE_GUI: ", pchTag );
         return( (zLONG) (TZPainterCtrl *)
                 new TZActiveX( pPainterWindow, pCtrl, pWndParent,
                                rect, vCtrl, lType, bPlaceHolder, lZKey, lTabStopNbr,
                                lCtrlID, pchTag, pchText ) );

      case zCTRLMSG_ACCEPT_PARENT:  // accept parents if they accept me
      // TraceLineS( "TZActiveXMsg zCTRLMSG_ACCEPT_PARENT: ",
      //             pchTag ? pchTag : pCtrl->m_csTag );
         return( lType );

      case zCTRLMSG_CREATE_ZEIDON:  // create Zeidon Control, empty Text
      {
         zVIEW   vTZPESRCO;

      // TraceLineS( "TZActiveXMsg zCTRLMSG_CREATE_ZEIDON: ",
      //             pchTag ? pchTag : pCtrl->m_csTag );
         SetAttributeFromInteger( vCtrl, szlControl,
                                  szlSubtype, pCtrl->m_lSubtype );
         if ( GetViewByName( &vTZPESRCO, szlTZPESRCO,
                             pPainterWindow->m_pBar->m_vSubtask, zLEVEL_TASK ) > 0 &&
              SetCursorFirstEntityByInteger( vTZPESRCO, szlControlDef,
                                             szlKey, lType,
                                             0 ) >= zCURSOR_SET )
         {
            zPCHAR  pchGUID;
            zSHORT  nRC;
            zSHORT  nAttribCnt;
            zSHORT  nCnt = 0;

            TZActiveX *pAX = DYNAMIC_DOWNCAST( TZActiveX, pCtrl->m_pWndCtrl );
            nAttribCnt = pAX->GetAttributeCount( );

            GetAddrForAttribute( &pchGUID, vTZPESRCO, szlControlDef, "GUID" );
            SetAttributeFromString( vCtrl, szlControl, "GUID", pchGUID );

            nRC = SetCursorFirstEntity( vCtrl, szlCtrlMap, 0 );
            while ( nRC >= zCURSOR_SET )
               nRC = DeleteEntity( vCtrl, szlCtrlMap, zREPOS_FIRST );

            nRC = SetCursorFirstEntity( vTZPESRCO, "CtrlMapDef", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               nCnt++;
               pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap,
                                                 zPOS_AFTER );
               SetMatchingAttributesByName( vCtrl, szlCtrlMap,
                                            vTZPESRCO, "CtrlMapDef",
                                            zSET_NULL );

               nRC = SetCursorNextEntity( vTZPESRCO, "CtrlMapDef", 0 );
            }

            while ( nAttribCnt > nCnt )
            {
               nCnt++;
               pPainterWindow->CreateMetaEntity( vCtrl, szlCtrlMap,
                                                 zPOS_AFTER );
               CString strHelp;
               strHelp.Format( "New%d", nCnt );
               SetAttributeFromString( vCtrl, szlCtrlMap, szlTag, strHelp );
            }
         }

         break;
      }

      case zCTRLMSG_CHANGE_TYPE:    // Type change
         break;

      default:
         return( 0 );
   }

   return( 0 );
}

}  // END of C 'wrapper' message stubs
