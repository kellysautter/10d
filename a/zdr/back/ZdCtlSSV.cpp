/////////////////////////////////////////////////////////////////////////////
// Project ZdSS
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlssv.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon SpreadSheet control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.10.15    DKS    Z10    QS999
//    Disabled horizontal AutoSize (always off).
//
// 1999.09.22    DKS    Z10    QS999
//    Fixed text color problem and started memory leak analyis.
//
// 1999.09.03    DKS   Z10
//    Force text and background colors to be system colors.
//
// 1999.02.03    DKS
//    Fix to THUMBTRACK message when one or two lines in SS (Helmut bug).
//
// 1998.10.30    DKS
//    Changed background color.  Fixed edit leave/enter cell processing.
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlSS.h"
#include "ZdCtlGbl.h"

#ifndef _NOANCHOR

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SpreadSheet globals
//
mGLOBAL_DECLARE( zPCHAR, szlSpinClassName, VA_SPINBTN_CLASS_NAME );
mGLOBAL_DECLARE( zPCHAR, szlCalendarClassName, VA_CALENDAR_CLASS_NAME );

zEXTERN ZListFile clfHead
#ifdef zGLOBAL_DATA
   ( "", "" );
#else
   ;
#endif

zEXTERN SS_CLASSNAME SSClassNames[ 1 ]
#ifdef zGLOBAL_DATA
 = {
     VA_GENERAL_CLASS_NAME,
     "SSwEdit",
     "SSwEditHScroll",
     "SSwEditComboBox",
     VA_PIC_CLASS_NAME,
     "SSwPic",
     VA_DATE_CLASS_NAME,
     "SSwDate",
     VA_TIME_CLASS_NAME,
     "SSwTime",
     VA_INTEGER_CLASS_NAME,
     "SSwInteger",
     VA_FLOAT_CLASS_NAME,
     "SSwFloat",
     "SSwtbSuperBtn",
     "SSwSuperBtn",
     "SSwtbComboBox",
     "SSwComboBox",
     "SSwListBox",
     VA_VIEWPICT_CLASS_NAME,
     "SSwtbCheckBox",
     "SSwCheckBox",
     "tbScrlBar",
   };
#else
   ;
#endif

mGLOBAL_DECLARE( zPCHAR, g_hColorTable, 0 );
mGLOBAL_DECLARE( zSHORT, g_nColorTableCnt, 0 );
mGLOBAL_DECLARE( zSHORT, g_nColorTableAllocCnt, 0 );
mGLOBAL_DECLARE( zPCHAR, g_hFontTable, 0 );
mGLOBAL_DECLARE( zSHORT, g_nFontTableCnt, 0 );
mGLOBAL_DECLARE( zLONG,  g_lFontTableAllocCnt, 0 );
mGLOBAL_DECLARE( zSHORT, g_nSpreadInstCnt, 0 );
mGLOBAL_DECLARE( zPCHAR, g_hCheckBoxDefPicts, 0 );
mGLOBAL_DECLARE( zSHORT, g_nCheckBoxDefPictCnt, 0 );
mGLOBAL_DECLARE( zSHORT, g_nCheckBoxDefPictLoadCnt, 0 );

//zEXTERN SSX_CALCFUNCTION SS_CalcFunctions[];

mGLOBAL_DECLARE( WNDPROC, g_lpfnDeskTopProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnSSDeskTopProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnEditProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnEditHScrollProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnPicProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnDateProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnTimeProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnIntegerProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnFloatProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnViewPictProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnButtonProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnCheckBoxProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnComboBoxProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnEditComboBoxProc, 0 );
mGLOBAL_DECLARE( WNDPROC, g_lpfnListBoxProc, 0 );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define OEMRESOURCE                    //???
#define DEFAULT_CELLWIDTH         8    //???
#define RESIZE_OFFSET             4    //???

void
SS_DeleteColorTable( )
{
   TRACE_DEBUG( "<SPREAD.CPP>", "SS_DeleteColorTable" );
   LPSS_CLRTBLITEM lpColorTable;
   zUSHORT           k;

   if ( g_hColorTable && g_nColorTableCnt )
   {
//    lpColorTable = (LPSS_CLRTBLITEM) DrLockTaskMemory( g_hColorTable );
      lpColorTable = (LPSS_CLRTBLITEM) g_hColorTable;

      for ( k = 0; k < g_nColorTableCnt; k++ )
      {
         if ( lpColorTable[ k ].hBrush )
            DeleteBrush( lpColorTable[ k ].hBrush );

         if ( lpColorTable[ k ].hPen )
            DeletePen( lpColorTable[ k ].hPen );
      }

      g_hColorTable = (zPCHAR) DrUnlockTaskMemory( lpColorTable );
      DrFreeTaskMemory( g_hColorTable );
   }

   g_hColorTable = 0;
   g_nColorTableCnt = 0;
   g_nColorTableAllocCnt = 0;
}

void
SS_DeleteFontTable( )
{
   TRACE_DEBUG( "<SPREAD.CPP>", "SS_DeleteFontTable" );
   LPSS_FONT lpFontTable;
   zUSHORT   k;

   if ( g_hFontTable && g_nFontTableCnt )
   {
//    lpFontTable = (LPSS_FONT) DrLockTaskMemory( g_hFontTable );
      lpFontTable = (LPSS_FONT) g_hFontTable;

      for ( k = 0; k < g_nFontTableCnt; k++ )
      {
         if ( lpFontTable[ k ].DeleteFont && lpFontTable[ k ].hFont )
         {
            DeleteFont( lpFontTable[ k ].hFont );
            lpFontTable[ k ].hFont = 0;
         }
      }

      g_hFontTable = (zPCHAR) DrUnlockTaskMemory( lpFontTable );
      DrFreeTaskMemory( g_hFontTable );
   }

   g_hFontTable = 0;
   g_nFontTableCnt = 0;
   g_lFontTableAllocCnt = 0;
}

// ss_main.cpp

zBOOL
SS_AutoSize( HWND hWnd, zBOOL fWM_Size )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_AutoSize" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   POINT         pt;
   RECT          rectClient;
   RECT          rectParentClient;
   RECT          rect;
   SS_COORD      ssRows;
   SS_COORD      ssCols;
   zLONG         lWidth;
   zLONG         lHeight;
   zLONG         x;
   zLONG         y;
   zBOOL         fVScrollVisibleOld;
   zBOOL         fHScrollVisibleOld;
   zBOOL         bRC = FALSE;
   HWND          hWndParent;

   hWndParent = GetParent( hWnd );
   if ( lpSS->Redraw &&
        (GetWindowLong( hWndParent, GWL_STYLE ) & WS_MINIMIZE) == 0 )
   {
      lpSS->fUpdateAutoSize = FALSE;

      fHScrollVisibleOld = lpSS->fHScrollVisible;
      fVScrollVisibleOld = lpSS->fVScrollVisible;

      SS_SetVScrollBar( hWnd, lpSS );
      SS_SetHScrollBar( hWnd, lpSS );

      if ( fHScrollVisibleOld != (zBOOL) lpSS->fHScrollVisible )
         SS_SetVScrollBar( hWnd, lpSS );

      zSHORT nExtra;

      GetClientRect( hWnd, &rectClient );

      if ( lpSS->fHScrollVisible )
         nExtra = lpSS->nHScrollHeight - 1;
      else
         nExtra = 0;

   // TraceLineS( "SS_AutoSize: ", "MoveWindow" );
      MoveWindow( lpSS->hWndVScroll, rectClient.right -
                  lpSS->nVScrollWidth + 1, -1, lpSS->nVScrollWidth,
                  rectClient.bottom - nExtra + 2, TRUE );

      if ( lpSS->fVScrollVisible )
         nExtra = lpSS->nVScrollWidth - 1;
      else
         nExtra = 0;

      MoveWindow( lpSS->hWndHScroll, -1, rectClient.bottom -
                  lpSS->nHScrollHeight + 1, rectClient.right - nExtra + 2,
                  lpSS->nHScrollHeight, TRUE );

      if ( lpSS->fVScrollVisible == 0 && lpSS->fHScrollVisible == 0 &&
           fVScrollVisibleOld && fHScrollVisibleOld )
      {
         SetRect( &rect, rectClient.right - lpSS->nVScrollWidth,
                  rectClient.bottom - lpSS->nHScrollHeight,
                  rectClient.right, rectClient.bottom );
         InvalidateRect( hWnd, &rect, TRUE );
      }

      SS_GetClientRect( hWnd, &rectClient );
      zmemcpy( &lpSS->rectClient, &rectClient, zsizeof( RECT ) );

      if ( lpSS->fAutoSize )
      {
         // Compute the maximum size of the control as the size of the parent.
         GetWindowRect( hWnd, &rect );
         pt.x = rect.left;
         pt.y = rect.top;
         ScreenToClient( hWndParent, &pt );
         GetClientRect( hWndParent, &rectParentClient );
   //    GetWindowRect( hWnd, &rect );  we just did this?

   //DKS rectClient.right = rectParentClient.right - pt.x -
   //DKS                    (rect.right - rect.left - rectClient.right);
   //DKS rectClient.bottom = rectParentClient.bottom - pt.y -
   //DKS                     (rect.bottom - rect.top - rectClient.bottom);
         rectClient.bottom = rect.bottom - pt.y -
                             (rect.bottom - rect.top - rectClient.bottom);

         if ( lpSS->Col.AutoSizeVisibleCnt )
            ssCols = mMin( lpSS->Col.AutoSizeVisibleCnt,
                           SS_GetColsMax( lpSS ) - 1 );
         else
            ssCols = SS_GetColsMax( lpSS ) - 1;

         x = SS_GetCellPosXExt( lpSS, lpSS->Col.nHeaderCnt,
                                ssCols + 1, &rectClient );
// DKS   lWidth = mMin( x, rectClient.right );
         lWidth = rectClient.right; // disable horizontal AutoSize

         if ( lpSS->Row.AutoSizeVisibleCnt )
         {
            ssRows = mMin( lpSS->Row.AutoSizeVisibleCnt,
                           SS_GetRowsMax( lpSS ) - 1 );
//          TraceLineI( "Rows AutoSize ", ssRows );
         }
         else
         {
            ssRows = SS_GetRowsMax( lpSS ) - 1;
//          TraceLineI( "Rows Max ", ssRows );
         }

         ssRows = mMax( lpSS->Virtual.VMinVisibleCnt, ssRows );    // DKS
         lpSS->Virtual.VMinVisibleCnt = ssRows;

         y = SS_GetCellPosYExt( lpSS, lpSS->Row.nHeaderCnt,
                                ssRows + 1, &rectClient );
         lHeight = mMin( y, rectClient.bottom );

         if ( lWidth != lpSS->rectClient.right ||
              lHeight != lpSS->rectClient.bottom )
         {
            pt.x = rect.left;
            pt.y = rect.top;
            ScreenToClient( GetParent( hWnd ), &pt );
            MoveWindow( hWnd, pt.x, pt.y,
                lWidth + (rect.right - rect.left - lpSS->rectClient.right),
                lHeight + (rect.bottom - rect.top - lpSS->rectClient.bottom),
                TRUE );
            bRC = TRUE;
         }
      }
   }
   else
      lpSS->fUpdateAutoSize = TRUE;

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

void
SS_Beep( LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_Beep" );
   if ( lpSS->fNoBeep == FALSE )
      ::MessageBeep( MB_ICONQUESTION );
}

zBOOL
SS_DeleteItem( LPSPREADSHEET lpSS, LPSS_ALLOCITEM lpAllocItem,
               SS_COORD wItemNum, SS_COORD wMaxItems )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_DeleteItem" );
   LPSS_ALLOCITEM   lpItems;
   zPCHAR *         lpItems2;
   DWORD            dwItemAt;
   WORD             wItem2At;
   zBOOL            bRC = 0;

   if ( wItemNum + 1 > wMaxItems )
      return( FALSE );

   dwItemAt = CALCALLOCITEM( wItemNum );

   if ( dwItemAt < lpAllocItem->dwItemCnt )
   {
//    lpItems = (LPSS_ALLOCITEM) DrLockTaskMemory( lpAllocItem->hItems );
      lpItems = (LPSS_ALLOCITEM) lpAllocItem->hItems;
      wItem2At = CALCALLOCITEMREM( wItemNum );

      if ( wItem2At < lpItems[ dwItemAt ].dwItemCnt )
      {
         if ( lpItems[ dwItemAt ].hItems )
         {
//          lpItems2 = (zPCHAR *) DrLockTaskMemory( lpItems[ dwItemAt ].hItems );
            lpItems2 = (zPCHAR *) lpItems[ dwItemAt ].hItems;

            if ( lpItems2[ wItem2At ] )
            {
               lpItems2[ wItem2At ] = lpItems2[ wItem2At ];
               DrFreeTaskMemory( lpItems2[ wItem2At ] );
               lpItems2[ wItem2At ] = 0;
            }

//          DrUnlockTaskMemory( lpItems2 );
            bRC = TRUE;
         }
      }

//    DrUnlockTaskMemory( lpItems );
   }

   return( bRC );
}

void
SS_ExtSelBegin( HWND hWnd, LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_ExtSelBegin" );
   LPSS_ROW lpRow;
   SS_COORD ssRow;
   SS_COORD ssRow1;
   SS_COORD ssRow2;
   SS_COORD ss;
   zBOOL    RedrawOld;

   lpSS->fExtSelHilite = TRUE;

   if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
   {
      if ( lpSS->ExtSelAnchorRow >= 1 )
         ssRow = lpSS->ExtSelAnchorRow;
      else
         ssRow = 1;

      if ( ssRow < lpSS->Row.ssCurrAt )
      {
         ssRow1 = ssRow;
         ssRow2 = lpSS->Row.ssCurrAt;
         lpSS->BlockCellUL.ssRow = ssRow;

         if ( lpSS->BlockCellLR.ssRow != -1 && lpSS->BlockCellLR.ssRow > ssRow )
            lpSS->BlockCellLR.ssRow = mMin( lpSS->BlockCellLR.ssRow, ssRow2 );
         else
            lpSS->BlockCellLR.ssRow = ssRow;

         lpSS->Row.BlockCellCurrentPos = lpSS->BlockCellLR.ssRow;
      }
      else
      {
         ssRow1 = lpSS->Row.ssCurrAt;
         ssRow2 = ssRow;
         lpSS->BlockCellLR.ssRow = ssRow;

         if ( lpSS->BlockCellUL.ssRow != -1 && lpSS->BlockCellUL.ssRow < ssRow )
            lpSS->BlockCellUL.ssRow = mMax( lpSS->BlockCellUL.ssRow, ssRow1 );
         else
            lpSS->BlockCellUL.ssRow = ssRow;

         lpSS->Row.BlockCellCurrentPos = lpSS->BlockCellUL.ssRow;
      }

      RedrawOld = lpSS->Redraw;
      lpSS->Redraw = FALSE;

      for ( ss = lpSS->Row.nHeaderCnt; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( ss < ssRow1 || ss > ssRow2 )
         {
            if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
            {
               if ( lpRow->fRowSelected )
               {
                  lpRow->fRowSelected = FALSE;
                  SS_InvalidateRow( hWnd, lpSS, ss );
                  lpSS->MultiSelCnt--;
               }

               SS_UnlockRowItem( lpSS, ss );
            }
         }
      }

      SSSetBool( hWnd, SSB_REDRAW, RedrawOld );
   }
   else
   {
      if ( !HIBYTE( GetKeyState( VK_CONTROL) ) )
         SSSelModeSendMessage( hWnd, SS_SELMODE_CLEARSEL, 0L, 0L, 0L );
      else
      {
         if ( (lpRow = SS_LockRowItem( lpSS, lpSS->Row.ssCurrAt )) != 0 )
         {
            if ( lpRow->fRowSelected )
               lpSS->fExtSelHilite = !lpRow->fRowSelected;

            SS_UnlockRowItem( lpSS, lpSS->Row.ssCurrAt );
         }
      }

      lpSS->BlockCellUL.ssRow = -1;
      lpSS->BlockCellLR.ssRow = -1;
      lpSS->Row.BlockCellCurrentPos = lpSS->Row.ssCurrAt;
      lpSS->ExtSelAnchorRow = lpSS->Row.ssCurrAt;
   }

   SS_SelectRow( hWnd, lpSS );
}

zBOOL
SS_GetCellFromPixel( HWND          hWnd,
                     LPSPREADSHEET lpSS,
                     LPSS_COORD    lpCol,
                     LPSS_COORD    lpRow,
                     zPLONG        lpX,
                     zPLONG        lpY,
                     zLONG         lMouseX,
                     zLONG         lMouseY )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_GetCellFromPixel" );
   RECT      rectClient;
   zSHORT    nWidth;
   zSHORT    nHeight;
   SS_COORD  ssColAt;
   SS_COORD  ssCol = -1;
   SS_COORD  ssRowAt;
   SS_COORD  ssRow = -1;
   zLONG     x;
   zLONG     y;
   zBOOL     bRC = TRUE;

   SS_GetClientRect( hWnd, &rectClient );

   if ( lpCol )
   {
      x = SS_GetCellPosX( lpSS, 0, lpSS->Col.nHeaderCnt );

      if ( lpSS->Col.Frozen )
         ssColAt = lpSS->Col.nHeaderCnt;
      else
         ssColAt = lpSS->Col.UL;

      for ( ; ssColAt < SS_GetColsMax( lpSS ) && x < rectClient.right; )
      {
         nWidth = SS_GetColWidth( lpSS, ssColAt );

         if ( lMouseX >= rectClient.left + x )
         {
            ssCol = -2;
            if ( lMouseX < rectClient.left + x + nWidth )
            {
               ssCol = ssColAt;
               break;
            }
         }

         x += nWidth;

         if ( lpSS->Col.Frozen && ssColAt == lpSS->Col.nHeaderCnt +
             lpSS->Col.Frozen - 1 )
            ssColAt = mMax( lpSS->Col.UL, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
         else
            ssColAt++;
      }

      *lpCol = ssCol;

      if ( lpX )
         *lpX = x;

      if ( ssCol == -2 )
         bRC = FALSE;
   }

   if ( lpRow )
   {
      y = SS_GetCellPosY( lpSS, 0, lpSS->Row.nHeaderCnt );

      if ( lpSS->Row.Frozen )
         ssRowAt = lpSS->Row.nHeaderCnt;
      else
         ssRowAt = lpSS->Row.UL;

      for ( ; ssRowAt < SS_GetRowsMax( lpSS ) && y < rectClient.bottom; )
      {
         nHeight = SS_GetRowHeight( lpSS, ssRowAt );

         if ( lMouseY >= rectClient.top + y )
         {
            ssRow = -2;
            if ( lMouseY < rectClient.top + y + nHeight )
            {
               ssRow = ssRowAt;
               break;
            }
         }

         y += nHeight;

         if ( lpSS->Row.Frozen && ssRowAt == lpSS->Row.nHeaderCnt +
             lpSS->Row.Frozen - 1 )
            ssRowAt = mMax( lpSS->Row.UL, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
         else
            ssRowAt++;
      }

      *lpRow = ssRow;

      if ( lpY )
         *lpY = y;

      if ( ssRow == -2 )
         bRC = FALSE;
   }

   return( bRC );
}

void
SS_GetClientRect( HWND    hWnd,
                  LPRECT  lpRect )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_GetClientRect" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );

   GetClientRect( hWnd, lpRect );

   if ( lpSS->fVScrollVisible )
      lpRect->right -= lpSS->nVScrollWidth - 1;

   if ( lpSS->fHScrollVisible )
      lpRect->bottom -= lpSS->nHScrollHeight - 1;

   if ( lpSS->fUseScrollArrows )
   {
      if ( lpSS->fRightScrollArrowVis )
         lpRect->right -= lpSS->nScrollArrowWidth;

      if ( lpSS->fBottomScrollArrowVis )
         lpRect->bottom -= lpSS->nScrollArrowHeight;
   }

// DrUnlockTaskMemory( lpSS );
}

zBOOL
SS_InitSpread( HWND hWnd, LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_InitSpread" );
   SS_CELLTYPE CellType;
   zLONG       lStyle;

   lpSS->RestrictRows = 0;
   lpSS->RestrictCols = 0;
   lpSS->DocumentProtected = 0;
   lpSS->DocumentLocked = 0;
#ifndef SS_NOCALC
   lpSS->AllowUserFormulas = 0;
#endif
   lpSS->wUserResize = 0;
   lpSS->wSelBlockOption = 0;
   lpSS->wGridType = 0;
   lpSS->EditModeOn = 0;
   lpSS->EditModeTurningOn = 0;
   lpSS->EditModeTurningOff = 0;
   lpSS->HiliteOn = 0;
   lpSS->FreezeHilite = 0;
   lpSS->NoEraseBkgnd = 0;
   lpSS->NoPaint = 0;

   lpSS->GridLines = 0;
   lpSS->EscapeInProgress = 0;
   lpSS->fFormulaMode = 0;
   lpSS->fEditModePermanent = 0;
   lpSS->fAutoSize = 0;
   lpSS->fProcessTab = 0;
   lpSS->wMessageBeingSent = 0;
   lpSS->fKillFocusReceived = 0;
   lpSS->fWindowPainted = 0;
   lpSS->fEditModeDataChange = 0;
   lpSS->fNoProcessButtonDown = 0;
   lpSS->fArrowsExitEditMode = 0;
   lpSS->fMoveActiveOnFocus = 0;
   lpSS->fFocusOn = 0;
   lpSS->fRetainSelBlock = 0;
   lpSS->fEditModeReplace = 0;
   lpSS->fNoBorder = 0;
   lpSS->fNoBeep = 0;
   lpSS->fInvalidated = 0;
   lpSS->fSetFocusWhileMsgBeingSent = 0;
   lpSS->fProcessingKillFocus = 0;
   lpSS->fProcessingLButtonDown = 0;
   lpSS->fCalcInProgress = 0;
   lpSS->fRowModeEditing = 0;
   lpSS->fRowModeChangeMade = 0;
   lpSS->fSuspendInvert = 0;
   lpSS->fAutoClipboard = 0;
   lpSS->fSingleSelRowFound = 0;
   lpSS->fProcessSetFocus = 0;
   lpSS->fProcessKillFocus = 0;
   lpSS->fInvertBlock = 0;
   lpSS->fSetBorder = 0;
   lpSS->fAllowMultipleSelBlocks = 0;
   lpSS->fSetFocusReceived = 0;
   lpSS->fMultipleBlocksSelected = 0;
   lpSS->fNoDrawFocusRect = 0;
   lpSS->fNoSetCapture = 0;
   lpSS->fAllowCellOverflow = 0;
   lpSS->fNoInvalidate = 0;
   lpSS->fSetActiveCellCalled = 0;
   lpSS->fSelBlockInvisible = 0;
   lpSS->fAllowDragDrop = 0;
   lpSS->fModeDragDrop = 0;
   lpSS->fDragDropBoxOn = 0;
   lpSS->fChangeMade = 0;
   lpSS->fAdjustUpdateRgn = 0;
   lpSS->fFocusHiliteOn = 0;
   lpSS->fRButtonDown = 0;
   lpSS->wButtonDrawMode = 0;
   lpSS->wEnterAction = 0;
   lpSS->wOpMode = SS_OPMODE_SINGLESEL;
   lpSS->wGridType = 0;
   lpSS->wUserResize = 0;
   lpSS->wSelBlockOption = 0;
   lpSS->fScrollBarExtMode = 0;
   lpSS->fUseScrollArrows = 0;
   lpSS->fNoSetScrollBars = 0;
   lpSS->fVScrollVisible = 0;
   lpSS->fHScrollVisible = 0;
   lpSS->fUseSpecialVScroll = 0;
   lpSS->wSpecialVScrollOptions = 0;
   lpSS->wScrollDirection = 0;

   lpSS->ModeResize = 0;
   lpSS->ModeSelect = 0;
   lpSS->ModeBlock = 0;
   lpSS->fIsBlockSelected = 0;

   if ( DrAllocTaskMemory( (zCOREMEM) &lpSS->hBuffer, 50, 1080 ) == 0 )
   {
   // lpSS->hBuffer = (zPCHAR) DrLockTaskMemory( lpSS->hBuffer );
   }

   lStyle = GetWindowLong( hWnd, GWL_STYLE );

   if ( lStyle & SSS_RESTRICTROWS )
      lpSS->RestrictRows = TRUE;

   if ( lStyle & SSS_RESTRICTCOLS )
      lpSS->RestrictCols = TRUE;

   lpSS->DocumentProtected = TRUE;

   if ( lStyle & SSS_LOCKALL )
      lpSS->DocumentLocked = TRUE;

#ifndef SS_NOCALC
   if ( lStyle & SSS_ALLOWUSERFORMULAS )
      lpSS->AllowUserFormulas = TRUE;
#endif

   if ( (lStyle & SSS_NORESIZE) == 0 )
      lpSS->wUserResize |= SS_USERRESIZE_COL | SS_USERRESIZE_ROW;

   if ( (lStyle & SSS_NOSELECTBLOCK) == 0 )
      lpSS->wSelBlockOption = SS_SELBLOCK_COLS | SS_SELBLOCK_ROWS |
                              SS_SELBLOCK_BLOCKS | SS_SELBLOCK_ALL;

   if ( (lStyle & SSS_NOGRIDLINES) == 0 )
      lpSS->wGridType |= SS_GRID_HORIZONTAL | SS_GRID_VERTICAL;

// if ( lStyle & SSS_ALLOWCELLOVERFLOW )
      lpSS->fAllowCellOverflow = TRUE;

   if ( lStyle & SSS_SCROLLBAREXTMODE )
      lpSS->fScrollBarExtMode = TRUE;

   lpSS->wGridType |= SS_GRID_SOLID;
   lpSS->GridColor = RGBCOLOR_PALEGRAY;

   lpSS->LastCell.Coord.ssRow = -1;
   lpSS->LastCell.Coord.ssCol = -1;
   lpSS->Row.LastRefCoord.Coord.ssRow = -1;
   lpSS->Col.LastRefCoord.Coord.ssCol = -1;

   lpSS->fIsBlockSelected = FALSE;
   lpSS->BlockCellUL.ssCol = -1;
   lpSS->BlockCellUL.ssRow = -1;

   lpSS->Row.Max = 0;
   lpSS->Col.Max = 0;
   lpSS->Row.nHeaderCnt = 1;
   lpSS->Col.nHeaderCnt = 1;
   lpSS->Row.Frozen = 0;
   lpSS->Col.Frozen = 0;
   lpSS->Row.MaxBeforeSort = -1;
   lpSS->Col.MaxBeforeSort = -1;

   lpSS->Row.UL = lpSS->Row.nHeaderCnt + lpSS->Row.Frozen;
   lpSS->Col.UL = lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;

   lpSS->Row.ssCurrAt = lpSS->Row.nHeaderCnt;
   lpSS->Col.ssCurrAt = lpSS->Col.nHeaderCnt;

   lpSS->Row.AllocCnt = 1;
   lpSS->Col.AllocCnt = 1;

   lpSS->Row.DataCnt = 1;
   lpSS->Col.DataCnt = 1;

   lpSS->Col.lCellSizeX100 = SS_WIDTH_DEFAULT;
   lpSS->Col.nCellSizeInPixels = SS_ColWidthToPixels( lpSS, -1 );
   lpSS->Row.lCellSizeX100 = SS_HEIGHT_DEFAULT;
   lpSS->Row.nCellSizeInPixels = SS_RowHeightToPixels( lpSS, -1, -1 );

   //lpSS->Redraw = TRUE;
   lpSS->CalcAuto = TRUE;
   lpSS->VertScrollBar = TRUE;
   lpSS->HorzScrollBar = TRUE;

   lpSS->fScrollBarShowMax = TRUE;
   lpSS->fScrollBarMaxAlign = TRUE;

   lpSS->fMoveActiveOnFocus = TRUE;
   lpSS->fAutoClipboard = TRUE;

   lpSS->wEnterAction = SS_ENTERACTION_NONE;
   lpSS->nUnitType = SS_UNITTYPE_DEFAULT;

   lpSS->fVirtualMode = 1;       // Zeidon usage requires virtual mode
   lpSS->Virtual.VOverlap = 0;
   lpSS->Virtual.VSize = VBUFFER_SIZE;
   lpSS->Virtual.VStartRow = -1;
   lpSS->Virtual.VStartIdx = -1;
   lpSS->Virtual.VHighWaterIdx = -1;
   lpSS->Virtual.VMax = -1;
   lpSS->Virtual.VRows = 0;

   // Allocate rolling row buffer (array of row alloc item pointers).  We
   // are using alloc items in an attempt to guarantee that the row pointer
   // corresponds to the correct row.  To do this, we are using dwItemCnt
   // from the SS_ALLOCITEM structure to keep track of the row number for
   // which the row buffer is allocated.  Note that the zeroth row buffer
   // is reserved for header row.
   while ( lpSS->Virtual.VSize > VBUFFER_MIN &&
           DrAllocTaskMemory( (zCOREMEM) &lpSS->Virtual.VRows, (zLONG)
           ((lpSS->Virtual.VSize + 1) * zsizeof( SS_ALLOCITEM )), 1081 ) != 0 )
   {
      lpSS->Virtual.VSize -= VBUFFER_MIN;
   }

   if ( lpSS->Virtual.VSize < VBUFFER_SIZE )
   {
      TraceLineI( "Virtual spreadsheet size row buffers reduced to ",
                  lpSS->Virtual.VSize );
   }

   if ( lpSS->Virtual.VSize <= VBUFFER_MIN )
   {
      lpSS->Virtual.VRows = 0;
      lpSS->Virtual.VSize = 0;

      // this should go thru message handler???
      MessageBox( 0, "Insufficient memory for spreadsheet",
                  "Zeidon Error", MB_OK );
   }
   else
   {
   // lpSS->Virtual.VRows =
   //          (LPSS_ALLOCITEM) DrLockTaskMemory( lpSS->Virtual.VRows );
      zmemset( (zPCHAR) lpSS->Virtual.VRows, 0,
               lpSS->Virtual.VSize * zsizeof( SS_ALLOCITEM ) );
//    DrUnlockTaskMemory( lpRows );
   }

   lpSS->fCalcBuildDependencies = TRUE;
   lpSS->nBorderSize = 1;
   lpSS->nMarginX = XMARGIN;
   lpSS->nMarginY = YMARGIN;

   lpSS->hWndOwner = GetParent( hWnd );
// TraceLineX( "SS_InitSpread hWndOwner: ", (zLONG) lpSS->hWndOwner );

   // Setup colors

#if 0
   HDC hDC = GetDC( hWnd );
   lpSS->Color.BackgroundId = SS_AddColor( lpSS, GetBkColor( hDC ) );
   lpSS->Color.ForegroundId = SS_AddColor( lpSS, GetTextColor( hDC ) );
   ReleaseDC( hWnd, hDC );
#else
   lpSS->Color.BackgroundId = SS_AddColor( lpSS, GetSysColor( COLOR_BACKGROUND ) );
   lpSS->Color.ForegroundId = SS_AddColor( lpSS, GetSysColor( COLOR_WINDOWTEXT ) );
#endif

   lpSS->Color.ShadowColorId = SS_AddColor( lpSS, GetSysColor( COLOR_BTNFACE ) );
   lpSS->Color.ShadowTextId  = SS_AddColor( lpSS, GetSysColor( COLOR_BTNTEXT ) );
   lpSS->Color.ShadowDarkId  = SS_AddColor( lpSS, GetSysColor( COLOR_BTNSHADOW ) );
   lpSS->Color.ShadowLightId = SS_AddColor( lpSS, GetSysColor( COLOR_BTNHILIGHT ) );

// lpSS->Color.GrayAreaBackground = RGBCOLOR_PALEGRAY;
   lpSS->Color.GrayAreaBackground = GetSysColor( COLOR_APPWORKSPACE );

   lpSS->ScrollBarColorId = SS_AddColor( lpSS, GetSysColor( COLOR_SCROLLBAR ) );

   // set up default cell types
   SSSetTypeEdit( hWnd, &lpSS->DefaultCellType, ES_LEFT |
                  ES_AUTOHSCROLL, 60, SS_CHRSET_CHR, SS_CASE_NOCASE );

   // set headers
   SSSetTypeStaticText( hWnd, &CellType, SS_TEXT_SHADOW | SS_TEXT_CENTER |
                        SS_TEXT_VCENTER | SS_TEXT_WORDWRAP );

   SSSetCellType( hWnd, SS_HEADER, SS_ALLROWS, &CellType );
   SSSetCellType( hWnd, SS_ALLCOLS, SS_HEADER, &CellType );

   lpSS->hCursorDefault = SS_CURSOR_DEFAULT;
   lpSS->hCursorColResize = (HICON) SS_CURSOR_DEFCOLRESIZE;
   lpSS->hCursorRowResize = (HICON) SS_CURSOR_DEFROWRESIZE;
   lpSS->hCursorButton = (HICON) SS_CURSOR_ARROW;
   lpSS->hCursorGrayArea = (HICON) SS_CURSOR_ARROW;
   lpSS->hCursorLockedCell = (HICON) SS_CURSOR_DEFAULT;
   lpSS->hCursorColHeader = (HICON) SS_CURSOR_DEFAULT;
   lpSS->hCursorRowHeader = (HICON) SS_CURSOR_DEFAULT;

   lpSS->Col.NumStart = 1;
   lpSS->Row.NumStart = 1;

   lpSS->ColHeaderDisplay = SS_HEADERDISPLAY_LETTERS;
   lpSS->RowHeaderDisplay = SS_HEADERDISPLAY_NUMBERS;

   {
      LPSS_COL lpCol;
      LPSS_ROW lpRow;

      if ( (lpCol = SS_AllocLockCol( lpSS, 0 )) != 0 )
      {
         // Set ssRow Header Width.
         lpCol->lColWidthX100 = (zLONG) (-2 * 100.0);
         lpCol->nColWidthInPixels = SS_ColWidthToPixels( lpSS, -2 );

         lpCol->bUserResize = (BYTE) SS_RESIZE_OFF;
         SS_UnlockColItem( lpSS, 0 );
      }

      if ( (lpRow = SS_AllocLockRow( lpSS, 0 )) != 0 )
      {
         lpRow->bUserResize = (BYTE) SS_RESIZE_OFF;
         SS_UnlockRowItem( lpSS, 0 );
      }
   }

   if ( lStyle & SSS_HIDECOLHEADERS )
      SSSetBool( hWnd, SSB_SHOWCOLHEADERS, FALSE );

   if ( lStyle & SSS_HIDEROWHEADERS )
      SSSetBool( hWnd, SSB_SHOWROWHEADERS, FALSE );

   lpSS->nScrollArrowWidth = SS_SCROLLARROW_WIDTH;
   lpSS->nScrollArrowHeight = SS_SCROLLARROW_HEIGHT;

   return( 0 );
}

zBOOL
SS_IntersectBlock( LPSS_SELBLOCK lpBlock,
                   LPSS_SELBLOCK lpSrc1,
                   LPSS_SELBLOCK lpSrc2 )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IntersectBlock" );
   zBOOL bRC = FALSE;

   if ( lpSrc1->LR.ssRow >= lpSrc2->UL.ssRow && lpSrc1->UL.ssRow <= lpSrc2->LR.ssRow &&
        lpSrc1->LR.ssCol >= lpSrc2->UL.ssCol && lpSrc1->UL.ssCol <= lpSrc2->LR.ssCol )
   {
      lpBlock->UL.ssCol = mMax( lpSrc1->UL.ssCol, lpSrc2->UL.ssCol );
      lpBlock->UL.ssRow = mMax( lpSrc1->UL.ssRow, lpSrc2->UL.ssRow );
      lpBlock->LR.ssCol = mMin( lpSrc1->LR.ssCol, lpSrc2->LR.ssCol );
      lpBlock->LR.ssRow = mMin( lpSrc1->LR.ssRow, lpSrc2->LR.ssRow );
      bRC = TRUE;
   }

   else
      SS_SetBlock( lpBlock, 0, 0, 0, 0 );

   return( bRC );
}

zBOOL
SS_IsBlockSelected( LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IsBlockSelected" );
   if ( SS_USESELBAR( lpSS ) )
      return( FALSE );

   if ( lpSS->fMultipleBlocksSelected == FALSE &&
        ((lpSS->BlockCellUL.ssCol == -1 &&
         lpSS->BlockCellUL.ssRow == -1 ) ||
        (lpSS->BlockCellUL.ssCol == lpSS->BlockCellLR.ssCol &&
         lpSS->BlockCellUL.ssRow == lpSS->BlockCellLR.ssRow &&
         lpSS->BlockCellUL.ssRow != -1 && lpSS->BlockCellUL.ssCol != -1)) )
   {
      return( FALSE );
   }

   return( TRUE );
}

zBOOL
SS_IsColsSelected( LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IsColsSelected" );
   if ( SS_IsBlockSelected( lpSS ) &&
        lpSS->BlockCellUL.ssRow == 0 && lpSS->BlockCellLR.ssRow == -1 &&
        lpSS->BlockCellUL.ssCol > 0 )
   {
      return( TRUE );
   }

   return( FALSE );
}

zBOOL
SS_IsMouseInButton( LPSPREADSHEET lpSS,
                    LPRECT        rectClient,
                    zLONG         lMouseX,
                    zLONG         lMouseY )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IsMouseInButton" );
   SS_CELLTYPE   CellTypeTemp;
   SS_COORD      ssCol;
   SS_COORD      ssRow;
   zBOOL         bRC = FALSE;
   zLONG         x;
   zLONG         y;
   SS_COORD      ssColAt;
   SS_COORD      ssRowAt;
   zSHORT        nWidth;
   zSHORT        nHeight;

   ssCol = -1;
   ssRow = -1;

   x = SS_GetCellPosX( lpSS, 0, lpSS->Col.nHeaderCnt );

   if ( lpSS->Col.Frozen )
      ssColAt = lpSS->Col.nHeaderCnt;
   else
      ssColAt = lpSS->Col.UL;

   for ( ; ssColAt < SS_GetColsMax( lpSS ) && x < rectClient->right; )
   {
      nWidth = SS_GetColWidth( lpSS, ssColAt );

      if ( lMouseX >= rectClient->left + x + 1 )
      {
         ssCol = -2;
         if ( lMouseX < rectClient->left + x + nWidth - 1 )
         {
            ssCol = ssColAt;
            break;
         }
      }

      x += nWidth;

      if ( lpSS->Col.Frozen && ssColAt == lpSS->Col.nHeaderCnt + lpSS->Col.Frozen - 1 )
         ssColAt = mMax( lpSS->Col.UL, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
      else
         ssColAt++;
   }

   y = SS_GetCellPosY( lpSS, 0, lpSS->Row.nHeaderCnt );

   if ( lpSS->Row.Frozen )
      ssRowAt = lpSS->Row.nHeaderCnt;
   else
      ssRowAt = lpSS->Row.UL;

   for ( ; ssRowAt < SS_GetRowsMax( lpSS ) && y < rectClient->bottom; )
   {
      nHeight = SS_GetRowHeight( lpSS, ssRowAt );

      if ( lMouseY >= rectClient->top + y )
      {
         ssRow = -2;
         if ( lMouseY < rectClient->top + y + nHeight - 1 )
         {
            ssRow = ssRowAt;
            break;
         }
      }

      y += nHeight;

      if ( lpSS->Row.Frozen && ssRowAt == lpSS->Row.nHeaderCnt + lpSS->Row.Frozen - 1 )
         ssRowAt = mMax( lpSS->Row.UL, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
      else
         ssRowAt++;
   }

   if ( ssCol >= 0 && ssRow >= 0 )
   {
      if ( SS_RetrieveCellType( lpSS, &CellTypeTemp, 0, ssCol, ssRow) )
      {
         if ( CellTypeTemp.chType == SS_TYPE_BUTTON ||
              CellTypeTemp.chType == SS_TYPE_CHECKBOX ||
              (CellTypeTemp.chType == SS_TYPE_COMBOBOX &&
               lMouseX >= rectClient->left + x + nWidth -
                                             lpSS->nComboButtonBitmapWidth) )
         {
            bRC = TRUE;
         }
      }
   }

   return( bRC );
}

zBOOL
SS_IsMouseInGrayArea( LPSPREADSHEET lpSS,
                      LPRECT        rectClient,
                      zLONG         lMouseX,
                      zLONG         lMouseY )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IsMouseInGrayArea" );
   zLONG x;
   zLONG y;

   if ( lpSS->Col.LR == lpSS->Col.Max )
   {
      x = SS_GetCellPosX( lpSS, lpSS->Col.UL, lpSS->Col.LR + 1 );
      if ( x < rectClient->right && lMouseX >= x )
         return( TRUE );
   }

   if ( lpSS->Row.LR == lpSS->Row.Max )
   {
      y = SS_GetCellPosY( lpSS, lpSS->Row.UL, lpSS->Row.LR + 1 );
      if ( y < rectClient->bottom && lMouseY >= y )
         return( TRUE );
   }

   return( FALSE );
}

zSHORT
SS_IsMouseInHorizResize( LPSPREADSHEET lpSS,
                         LPRECT        rect,
                         LPRECT        rectClient,
                         zLONG         lMouseX,
                         zLONG         lMouseY,
                         LPSS_COORD    ssRow )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IsMouseInHorizResize" );
   LPSS_ROW      lpRow;
   zSHORT        lResize;
   zLONG         x;
   zLONG         y;
   SS_COORD      ssRowAt;
   zSHORT        nHeight;
   zLONG         lPosY = -1;

   x = SS_GetCellPosX( lpSS, 0, lpSS->Col.nHeaderCnt );

   if ( lMouseX < rectClient->left + x )
   {
      y = -1;
      ssRowAt = 0;

      for ( ; ssRowAt < SS_GetRowsMax( lpSS ) &&
            y < rectClient->bottom + RESIZE_OFFSET; )
      {
         nHeight = SS_GetRowHeight( lpSS, ssRowAt );

         if ( y + nHeight >= rectClient->bottom + RESIZE_OFFSET )
            break;

         if ( lMouseY >= rectClient->top + y + nHeight - RESIZE_OFFSET &&
              lMouseY <= rectClient->top + y + nHeight + RESIZE_OFFSET )
         {
            lResize = SS_RESIZE_DEFAULT;

            if ( (lpRow = SS_LockRowItem( lpSS, ssRowAt) ) != 0 )
            {
               lResize = lpRow->bUserResize;
               SS_UnlockRowItem( lpSS, ssRowAt );
            }

            if ( (!(lpSS->wUserResize & SS_USERRESIZE_ROW) &&
                  lResize == SS_RESIZE_DEFAULT) || lResize == SS_RESIZE_OFF )
               ;
            else
            {
               if ( ssRow )
                  *ssRow = ssRowAt;

               lPosY = y + nHeight;
               break;
            }
         }

         y += nHeight;

         if ( ssRowAt == lpSS->Row.nHeaderCnt + lpSS->Row.Frozen - 1 )
            ssRowAt = mMax( lpSS->Row.UL, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
         else
            ssRowAt++;
      }
   }

   return( (zSHORT) lPosY );
}

zSHORT
SS_IsMouseInVertResize( LPSPREADSHEET lpSS,
                        LPRECT        rect,
                        LPRECT        rectClient,
                        zLONG         lMouseX,
                        zLONG         lMouseY,
                        LPSS_COORD    ssCol )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IsMouseInVertResize" );
   LPSS_COL      lpCol;
   zSHORT        lResize;
   zLONG         x;
   zLONG         y;
   SS_COORD      ssColAt;
   zSHORT        nWidth;
   zSHORT        nPosX = -1;

   y = SS_GetCellPosY( lpSS, 0, lpSS->Row.nHeaderCnt );

   if ( lMouseY < rectClient->top + y )
   {
      x = -1;
      ssColAt = 0;

      for ( ; ssColAt < SS_GetColsMax( lpSS ) &&
            x < rectClient->right + RESIZE_OFFSET; )
      {
         nWidth = SS_GetColWidth( lpSS, ssColAt );

         if ( x + nWidth >= rectClient->right + RESIZE_OFFSET )
            break;

         if ( lMouseX >= rectClient->left + x + nWidth - RESIZE_OFFSET &&
              lMouseX <= rectClient->left + x + nWidth + RESIZE_OFFSET )
         {
            lResize = SS_RESIZE_DEFAULT;

            if ( (lpCol = SS_LockColItem( lpSS, ssColAt )) != 0 )
            {
               lResize = lpCol->bUserResize;
               SS_UnlockColItem( lpSS, ssColAt );
            }

            if ( (!(lpSS->wUserResize & SS_USERRESIZE_COL) &&
                  lResize == SS_RESIZE_DEFAULT) ||
                 lResize == SS_RESIZE_OFF )
            {
               ;
            }
            else
            {
               if ( ssCol )
                  *ssCol = ssColAt;

               nPosX = x + nWidth;
               break;
            }
         }

         x += nWidth;

         if ( ssColAt == lpSS->Col.nHeaderCnt + lpSS->Col.Frozen - 1 )
            ssColAt = mMax( lpSS->Col.UL, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
         else
            ssColAt++;
      }
   }

   return( nPosX );
}

zBOOL
SS_IsRowsSelected( LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_IsRowsSelected" );
   if ( SS_IsBlockSelected( lpSS ) &&
        lpSS->BlockCellUL.ssCol == 0 && lpSS->BlockCellLR.ssCol == -1 &&
        lpSS->BlockCellUL.ssRow > 0 )
   {
      return( TRUE );
   }

   return( FALSE );
}

zLONG
SS_ProcessLButton( HWND hWnd, WORD  wMsg,
                   WPARAM  wParam, LPARAM  lParam )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_ProcessLButton" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   LPSS_CELLTYPE lpCellType;
   SS_CELLCOORD  BlockCellUL;
   SS_CELLCOORD  BlockCellLR;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   SS_COORD      ssColDblClk;
   SS_COORD      ssRowDblClk;
   SS_COORD      ssRow;
   SS_COORD      ssRowAtOld;
   SS_COORD      ssRowAtTemp;
   SS_COORD      ssCol;
   SS_COORD      ssColAtOld;
   SS_COORD      ssColAtTemp;
   SS_COORD      ssRowSave;
   SS_COORD      ssRowAt;
   SS_COORD      ssColAt;
   SS_COORD      ssColSave;
   WORD          wMsgOrig;
   RECT          rect;
   RECT          rectClient;
   HWND          hWndFocus;
   zBOOL         fDone = FALSE;
   zBOOL         fNoSetCapture = FALSE;
   zBOOL         fComboBtn = FALSE;
   zBOOL         bRC;
   zBOOL         bInvert;
   zBOOL         fControlDown;
   zLONG         lMouseX;
   zLONG         lMouseY;
   zLONG         lPos;
   zLONG         x;
   zLONG         y;
   WORD          fFlag = 0;

   if ( (lpSS = SS_Lock( hWnd )) != 0 )
   {
      GetWindowRect( hWnd, &rect );

      lMouseX = wParam;
      lMouseY = LOWORD( lParam );

      if ( wMsg == SSM_PROCESSLBUTTONDOWN )
         wMsg = WM_LBUTTONDOWN;
      else
         wMsg = WM_LBUTTONDBLCLK;

      wMsgOrig = wMsg;

      lpSS->fKillFocusReceived = FALSE;
      lpSS->fProcessingLButtonDown = FALSE;
      lpSS->fSingleSelRowFound = FALSE;

      SS_GetClientRect( hWnd, &rectClient );

      if ( lMouseX >= rectClient.right || lMouseY >= rectClient.bottom )
      {
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }

      lpSS->wMsgLast = WM_LBUTTONDOWN;

      if ( wMsg == WM_LBUTTONDOWN &&
           (lPos = SS_IsMouseInHorizResize( lpSS, &rect, &rectClient,
                                            lMouseX, lMouseY, &ssRow)) != -1 )
      {
         ssRowAt = ssRow;
         ssRowAtTemp = ssRow;
         while ( (ssRowAtTemp + 1 <= SS_GetRowsMax( lpSS ) - 1 ) &&
                 (SS_GetRowHeight( lpSS, ssRowAtTemp + 1 ) == 0) )
         {
            ssRowAtTemp++;
            if ( SSGetUserResizeRow( hWnd, ssRowAtTemp ) != SS_RESIZE_OFF )
               ssRowAt = ssRowAtTemp;
         }

         if ( ssRowAt <= SS_GetRowsMax( lpSS ) - 1 )
            ssRow = ssRowAt;

         if ( (ssRow < lpSS->BlockCellUL.ssRow || ssRow > lpSS->BlockCellLR.ssRow ) &&
              lpSS->wOpMode != SS_OPMODE_EXTSEL )
         {
            SS_ResetBlock( hWnd, TRUE );
         }

         SS_CellEditModeOff( hWnd, lpSS, 0 );
         SetCapture( hWnd );
         lpSS->ModeResize = SS_MODERESIZE_HORIZ;
         lpSS->lResizeOffset = lMouseY - lPos;
         lpSS->lResizeStartPos = lPos;
         lpSS->lResizeMinPos = lPos - SS_GetRowHeight( lpSS, ssRow );
         lpSS->ssResizeCoord = ssRow;
         SS_Resize( hWnd, lpSS, SS_RESIZEMODE_START,
                    MAKELONG( lMouseX, lMouseY ) );
      }
      else
      if ( wMsg == WM_LBUTTONDOWN &&
           (lPos = SS_IsMouseInVertResize( lpSS, &rect,
                                           &rectClient,
                                           lMouseX, lMouseY,
                                           &ssCol) ) != -1 )
      {
         ssColAt = ssCol;
         ssColAtTemp = ssCol;
         while ( (ssColAtTemp + 1 <= SS_GetColsMax( lpSS ) - 1) &&
                 (SS_GetColWidth( lpSS, ssColAtTemp + 1 ) == 0) )
         {
            ssColAtTemp++;
            if ( SSGetUserResizeCol( hWnd, ssColAtTemp ) != SS_RESIZE_OFF )
               ssColAt = ssColAtTemp;
         }

         if ( ssColAt <= SS_GetColsMax( lpSS ) - 1 )
            ssCol = ssColAt;

         if ( (ssCol < lpSS->BlockCellUL.ssCol || ssCol > lpSS->BlockCellLR.ssCol) &&
              lpSS->wOpMode != SS_OPMODE_EXTSEL )
         {
            SS_ResetBlock( hWnd, TRUE );
         }

         SS_CellEditModeOff( hWnd, lpSS, 0 );
         SetCapture( hWnd );
         lpSS->ModeResize = SS_MODERESIZE_VERT;
         lpSS->lResizeOffset = lMouseX - lPos;
         lpSS->lResizeStartPos = lPos;
         lpSS->lResizeMinPos = lPos - SS_GetColWidth( lpSS, ssCol );
         lpSS->ssResizeCoord = ssCol;
         SS_Resize( hWnd, lpSS, SS_RESIZEMODE_START,
                    MAKELONG( lMouseX, lMouseY ) );
      }
#ifndef SS_NODRAGDROP
      else
      if ( wMsg == WM_LBUTTONDOWN &&
           SS_IsMouseInDragDrop( hWnd, lpSS, &rectClient,
                                 lMouseX, lMouseY, &Col, &Row ) )
      {
         SetCapture( hWnd );
         lpSS->fModeDragDrop = TRUE;
         lpSS->DragDropCurrentCol = ssCol;
         lpSS->DragDropCurrentRow = ssRow;

         if ( SS_IsBlockSelected( lpSS ) )
         {
            lpSS->DragDropBlockUL.ssCol = lpSS->BlockCellUL.ssCol;
            lpSS->DragDropBlockUL.ssRow = lpSS->BlockCellUL.ssRow;
         }
         else
         {
            lpSS->DragDropBlockUL.ssCol = lpSS->Col.ssCurrAt;
            lpSS->DragDropBlockUL.ssRow = lpSS->Row.ssCurrAt;
         }
      }
#endif
      else
      {
         ssColSave = lpSS->Col.ssCurrAt;
         ssRowSave = lpSS->Row.ssCurrAt;

         SS_GetCellFromPixel( hWnd, lpSS, &ssCol, &ssRow, &x, &y,
                              lMouseX, lMouseY );

         // Check if column has been selected.
         if ( ssRow == -1 )
           fFlag |= zLEAVE_CELL_COLSELECTED;
         else
         if ( ssCol == -2 || ssRow == -2 )
            return( 0 );

         if ( HIBYTE( GetKeyState( VK_CONTROL) ) &&
              lpSS->fAllowMultipleSelBlocks && !SS_USESELBAR( lpSS ) )
         {
            fControlDown = TRUE;
         }
         else
            fControlDown = FALSE;

         if ( HIBYTE( GetKeyState( VK_SHIFT) ) &&
              wMsg == WM_LBUTTONDOWN &&
              !SS_USESELBAR( lpSS ) &&
              (((lpSS->wSelBlockOption & SS_SELBLOCK_BLOCKS ) && ssCol != -1 &&
                 ssRow != -1) ||
               ((lpSS->wSelBlockOption & SS_SELBLOCK_COLS ) && ssCol != -1 &&
                 ssRow == -1) ||
               ((lpSS->wSelBlockOption & SS_SELBLOCK_ROWS ) && ssCol == -1 &&
                 ssRow != -1)) )
         {
            lpSS->ModeBlock = TRUE;
            lpSS->fIsBlockSelected = TRUE;

            if ( !SS_IsBlockSelected( lpSS ) )
            {
               lpSS->BlockCellUL.ssRow = lpSS->Row.ssCurrAt;
               lpSS->BlockCellUL.ssCol = lpSS->Col.ssCurrAt;

               lpSS->BlockCellLR.ssRow = lpSS->Row.ssCurrAt;
               lpSS->BlockCellLR.ssCol = lpSS->Col.ssCurrAt;

               lpSS->Row.BlockCellCurrentPos = lpSS->Row.ssCurrAt;
               lpSS->Col.BlockCellCurrentPos = lpSS->Col.ssCurrAt;
            }

            if ( lpSS->fMultipleBlocksSelected )
               lpSS->MultiSelBlock.lItemCnt--;

            SetCapture( hWnd );
            SS_SelectBlock( hWnd, lpSS, lMouseX, lMouseY );
            SS_HiliteCell( hWnd, TRUE );
//          DrUnlockTaskMemory( lpSS );
            return( 0 );
         }

         // Select Block of Cells.
         if ( lpSS->fAllowMultipleSelBlocks && fControlDown &&
              ((lpSS->wSelBlockOption & SS_SELBLOCK_BLOCKS) ||
              ((lpSS->wSelBlockOption & SS_SELBLOCK_ROWS) && ssCol == -1) ||
              ((lpSS->wSelBlockOption & SS_SELBLOCK_COLS) && ssRow == -1)) )
         {
            SS_BeginMultipleSelection( hWnd, lpSS );
         }
         else
         if ( lpSS->wOpMode != SS_OPMODE_EXTSEL )
            SS_ResetBlock( hWnd, TRUE );

         if ( wMsg == WM_LBUTTONDBLCLK &&
              ssRow == lpSS->Row.ssCurrAt && ssCol == lpSS->Col.ssCurrAt )
         {
         }
         else
         {
            if ( SS_CellEditModeOff( hWnd, lpSS, 0 ) == FALSE )
               return( 0 );

            if ( SS_ALLOWSELBLOCK( lpSS ) &&
//  can't get here otherwise  ssRow != -2 && ssCol != -2 &&
                 lpSS->Col.Max > 0 && lpSS->Row.Max > 0 )
            {
               lpSS->ModeBlock = TRUE;

               BlockCellUL.ssRow = ssRow;
               BlockCellUL.ssCol = ssCol;

               BlockCellLR.ssRow = ssRow;
               BlockCellLR.ssCol = ssCol;
            }

            bInvert = FALSE;

// can't be here if this is true  if ( ssCol == -2 || ssRow == -2 )
//                                   ;
//                                else
            if ( ssCol != -1 && ssRow != -1 )
            {
               SS_HiliteCell( hWnd, FALSE );
               ssRowAtOld = lpSS->Row.ssCurrAt;
               ssColAtOld = lpSS->Col.ssCurrAt;
               lpSS->Row.ssCurrAt = ssRow;
               lpSS->Col.ssCurrAt = ssCol;
               ssColSave = lpSS->Col.ssCurrAt;
               ssRowSave = lpSS->Row.ssCurrAt;
//             TraceLineI( "Calling SS_LeaveCell Row: ", ssRowAtOld );
//             TraceLineI( "                     Col: ", ssColAtOld );
               bRC = SS_LeaveCell( hWnd, lpSS, ssColAtOld, -1,
                                    ssRowAtOld, -1, fFlag );

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( FALSE );

               lpSS->EditModeTurningOff = TRUE;
               UpdateWindow( hWnd );
               lpSS->EditModeTurningOff = FALSE;

               if ( bRC )
               {
                  lpCellType = SS_RetrieveCellType( lpSS, &CellType,
                                                    0, ssCol, ssRow );

                  if ( SS_USESELBAR( lpSS ) )
                  {
                     SS_HiliteCell( hWnd, TRUE );
                     lpSS->fSingleSelRowFound = TRUE;

#ifndef SS_NOLISTBOX
                     if ( lpSS->wOpMode == SS_OPMODE_MULTISEL )
                        SS_SelectRow( hWnd, lpSS );
                     else
                     if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
                        SS_ExtSelBegin( hWnd, lpSS );
#endif
                  }

                  else
                  if ( lpCellType &&
                       CellType.chType == SS_TYPE_COMBOBOX &&
                       (lpSS->fEditModePermanent ||
                        SS_IsMouseInButton( lpSS, &rectClient,
                                            lMouseX, lMouseY )) )
                  {
                     fComboBtn = TRUE;
                     fNoSetCapture = TRUE;
                     wMsg = WM_LBUTTONDBLCLK;
                  }
                  else
                  if ( lpCellType &&
                       ((CellType.chType == SS_TYPE_BUTTON ||
                         CellType.chType == SS_TYPE_CHECKBOX) &&
                         (lpSS->wOpMode != SS_OPMODE_ROWMODE ||
                          lpSS->fRowModeEditing) &&
                          !SS_GetLock( hWnd, lpSS, ssCol, ssRow )) )
                  {
                  }
                  else
                  {
#ifndef SS_GP
                     if ( lpSS->fEditModePermanent )
                        SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
#endif
                     SS_HiliteCell( hWnd, TRUE );
                     lpSS->fSingleSelRowFound = TRUE;
                  }

                  if ( (lpSS->wSelBlockOption & SS_SELBLOCK_BLOCKS) == 0 )
                     lpSS->ModeBlock = FALSE;
               }
               else
               {
                  SS_SendMsgCoords( hWnd, SSM_CLICK, GetDlgCtrlID( hWnd ),
                                    ssCol == -1 ?
                                     SS_HEADER : ssCol - lpSS->Col.nHeaderCnt + 1,
                                    ssRow == -1 ?
                                     SS_HEADER : ssRow - lpSS->Row.nHeaderCnt + 1 );

                  if ( mIs_hWnd( hWnd ) )
                  {
                     UpdateWindow( hWnd );
                     lpSS->ModeBlock = FALSE;
                  }

//                DrUnlockTaskMemory( lpSS );
                  return( 0 );
               }
            }
            else
            if ( (lpSS->wOpMode == SS_OPMODE_SINGLESEL ||
                  lpSS->wOpMode == SS_OPMODE_ROWMODE ||
                  (lpSS->wOpMode == SS_OPMODE_ROWMODE &&
                   lpSS->fRowModeEditing == 0) ||
                  lpSS->wOpMode == SS_OPMODE_MULTISEL ||
                  lpSS->wOpMode == SS_OPMODE_EXTSEL) &&
                 ssCol == -1 && ssRow != -1 )
            {
               SS_HiliteCell( hWnd, FALSE );
               ssRowAtOld = lpSS->Row.ssCurrAt;
               lpSS->Row.ssCurrAt = ssRow;

               if ( !SS_LeaveCell( hWnd, lpSS, -1, -1, ssRowAtOld, -1, fFlag) )
               {
               }

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( FALSE );

#ifndef SS_NOLISTBOX
               SS_HiliteCell( hWnd, TRUE );
               lpSS->fSingleSelRowFound = TRUE;

               if ( lpSS->wOpMode == SS_OPMODE_MULTISEL )
                  SS_SelectRow( hWnd, lpSS );

               else
               if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
                  SS_ExtSelBegin( hWnd, lpSS );

               else
#endif
               {
   //               SS_HiliteCell( hWnd, TRUE );
   //               lpSS->fSingleSelRowFound = TRUE;
               }

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( FALSE );

               if ( SS_SendMsgCoords( hWnd, SSM_CLICK,
                                      GetDlgCtrlID( hWnd ), SS_HEADER,
                                      ssRow - lpSS->Row.nHeaderCnt + 1) )
               {
//                DrUnlockTaskMemory( lpSS );
                  return( FALSE );
               }

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( FALSE );
               else
                  UpdateWindow( hWnd );
            }
            else
            if ( SS_ALLOWSELBLOCK( lpSS ) )
            {
               ssRowAtOld = lpSS->Row.ssCurrAt;
               ssColAtOld = lpSS->Col.ssCurrAt;

               if ( ssCol == -1 && ssRow != -1 )
               {
                  if ( (lpSS->wSelBlockOption & SS_SELBLOCK_ROWS) &&
                       ((lpRow = SS_LockRowItem( lpSS, ssRow )) == 0 ||
                        lpRow->fNonSelectable == 0) )
                  {
                     BlockCellUL.ssCol = 0;

                     if ( SS_GetFirstValidCol( hWnd, lpSS, ssRow, &ssColAtTemp ) &&
                          (lpSS->Col.ssCurrAt != ssColAtTemp ||
                           lpSS->Row.ssCurrAt != ssRow) )
                     {
                        SS_HiliteCell( hWnd, FALSE );
                        lpSS->Row.ssCurrAt = ssRow;
                        lpSS->Col.ssCurrAt = ssColAtTemp;
                        ssColSave = lpSS->Col.ssCurrAt;
                        ssRowSave = lpSS->Row.ssCurrAt;
                        if ( !SS_LeaveCell( hWnd, lpSS, ssColAtOld, -1,
                                            ssRowAtOld, -1, fFlag ) )
                        {
                           lpSS->ModeBlock = FALSE;
//                         DrUnlockTaskMemory( lpSS );
                           return( 0 );
                        }

                        if ( lpSS->fAllowEditModePermSel == 0 )
                           SS_HiliteCell( hWnd, TRUE );
                     }
                  }
                  else
                     lpSS->ModeBlock = FALSE;
               }
               else
               if ( ssCol != -1 && ssRow == -1 )
               {
                  if ( (lpSS->wSelBlockOption & SS_SELBLOCK_COLS) &&
                       ((lpCol = SS_LockColItem( lpSS, ssCol )) == 0 ||
                        lpCol->fNonSelectable == 0) )
                  {
                     BlockCellUL.ssRow = 0;

                     if ( SS_GetFirstValidRow( hWnd, lpSS, ssCol, &ssRowAtTemp ) &&
                          (lpSS->Row.ssCurrAt != ssRowAtTemp ||
                           lpSS->Col.ssCurrAt != ssCol) )
                     {
                        SS_HiliteCell( hWnd, FALSE );
                        lpSS->Row.ssCurrAt = ssRowAtTemp;
                        lpSS->Col.ssCurrAt = ssCol;
                        ssColSave = lpSS->Col.ssCurrAt;
                        ssRowSave = lpSS->Row.ssCurrAt;
                        if ( !SS_LeaveCell( hWnd, lpSS, ssColAtOld, -1,
                                            ssRowAtOld, -1, fFlag ) )
                        {
                           lpSS->ModeBlock = FALSE;
//                         DrUnlockTaskMemory( lpSS );
                           return( 0 );
                        }

                        if ( lpSS->fAllowEditModePermSel == 0 )
                           SS_HiliteCell( hWnd, TRUE );
                     }
                  }
                  else
                     lpSS->ModeBlock = FALSE;
               }
               else
               if ( ssCol == -1 && ssRow == -1 )
               {
                  if ( lpSS->wSelBlockOption & SS_SELBLOCK_ALL )
                  {
                     BlockCellUL.ssCol = 0;
                     BlockCellUL.ssRow = 0;

                     if ( lpSS->Col.Frozen )
                        ssColAtTemp = lpSS->Col.nHeaderCnt;
                     else
                        ssColAtTemp = lpSS->Col.UL;

                     if ( lpSS->Row.Frozen )
                        ssRowAtTemp = lpSS->Row.nHeaderCnt;
                     else
                        ssRowAtTemp = lpSS->Row.UL;

                     if ( lpSS->Col.ssCurrAt != ssColAtTemp ||
                          lpSS->Row.ssCurrAt != ssRowAtTemp )
                     {
                        SS_HiliteCell( hWnd, FALSE );
                        lpSS->Col.ssCurrAt = ssColAtTemp;
                        lpSS->Row.ssCurrAt = ssRowAtTemp;
                        ssColSave = lpSS->Col.ssCurrAt;
                        ssRowSave = lpSS->Row.ssCurrAt;
                        if ( !SS_LeaveCell( hWnd, lpSS, ssColAtOld, -1,
                                            ssRowAtOld, -1, fFlag) )
                        {
                           lpSS->ModeBlock = FALSE;
//                         DrUnlockTaskMemory( lpSS );
                           return( 0 );
                        }

                        if ( lpSS->fAllowEditModePermSel == 0 )
                           SS_HiliteCell( hWnd, TRUE );
                     }
                  }
                  else
                     lpSS->ModeBlock = FALSE;
               }

               if ( lpSS->ModeBlock )
                  bInvert = TRUE;
            }

            if ( ssColSave != lpSS->Col.ssCurrAt || ssRowSave != lpSS->Row.ssCurrAt )
               fDone = TRUE;
            else
            if ( SS_ALLOWSELBLOCK( lpSS ) && ssCol != -2 && ssRow != -2 )
            {
               if ( lpSS->ModeBlock )
               {
                  lpSS->BlockCellUL.ssRow = BlockCellUL.ssRow;
                  lpSS->BlockCellUL.ssCol = BlockCellUL.ssCol;

                  lpSS->BlockCellLR.ssRow = BlockCellLR.ssRow;
                  lpSS->BlockCellLR.ssCol = BlockCellLR.ssCol;
               }

               lpSS->Row.BlockCellCurrentPos = ssRow;
               lpSS->Col.BlockCellCurrentPos = ssCol;

               if ( bInvert )
               {
                  lpSS->fIsBlockSelected = TRUE;
                  SS_InvertBlock( hWnd, 0, lpSS );
               }
            }
         }

         if ( fDone )
         {
         }
         else
         if ( wMsg == WM_LBUTTONDBLCLK && ssCol >= 0 && ssRow >= 0 )
         {
            if ( lpSS->RestrictCols && ssCol > lpSS->Col.DataCnt &&
                 lpSS->RestrictRows && ssRow > lpSS->Row.DataCnt )
            {
               if ( !SS_SendMsgCoords( hWnd, SSM_COLROWRESTRICTED,
                                       GetDlgCtrlID( hWnd ),
                                       ssCol - lpSS->Col.nHeaderCnt + 1,
                                       ssRow - lpSS->Row.nHeaderCnt + 1 ) )
               {
                  SS_Beep( lpSS );
               }
            }
            else
            if ( lpSS->RestrictCols && ssCol > lpSS->Col.DataCnt )
            {
               if ( !SS_SendMsg( hWnd, SSM_COLRESTRICTED,
                                 GetDlgCtrlID( hWnd ),
                                 (zLONG) (ssCol - lpSS->Col.nHeaderCnt + 1) ) )
               {
                  SS_Beep( lpSS );
               }
            }
            else
            if ( lpSS->RestrictRows && ssRow > lpSS->Row.DataCnt )
            {
               if ( !SS_SendMsg( hWnd, SSM_ROWRESTRICTED,
                                 GetDlgCtrlID( hWnd ),
                                 (zLONG) (ssRow - lpSS->Row.nHeaderCnt + 1) ) )
               {
                  SS_Beep( lpSS );
               }
            }
            else
            {
               if ( fComboBtn == FALSE )
               {
                  if ( SS_SendMsgCoords( hWnd, SSM_DBLCLK,
                                         GetDlgCtrlID( hWnd ),
                                         ssCol - lpSS->Col.nHeaderCnt + 1,
                                         ssRow - lpSS->Row.nHeaderCnt + 1 ) )
                  {
//                   DrUnlockTaskMemory( lpSS );
                     return( FALSE );
                  }

                  if ( mIs_hWnd( hWnd ) == FALSE )
                     return( FALSE );

                  if ( GetSystemMetrics( SM_SWAPBUTTON ) )
                  {
                     if ( (GetAsyncKeyState( VK_RBUTTON ) & 0x8000) == 0 )
                        lpSS->fNoSetCapture = TRUE;
                  }
                  else
                     if ( (GetAsyncKeyState( VK_LBUTTON ) & 0x8000) == 0 )
                        lpSS->fNoSetCapture = TRUE;
               }

               if ( ssRow == lpSS->Row.ssCurrAt && ssCol == lpSS->Col.ssCurrAt )
                  SS_CellEditModeOn( hWnd, lpSS,
                                     fComboBtn ? wMsgOrig : wMsg, 0, 0L );
            }
         }
         else
         {
            if ( wMsg == WM_LBUTTONDBLCLK )
            {
               if ( ssCol == -1 )
                  ssColDblClk = SS_HEADER;
               else
                  ssColDblClk = ssCol - lpSS->Col.nHeaderCnt + 1;

               if ( ssRow == -1 )
                  ssRowDblClk = SS_HEADER;
               else
                  ssRowDblClk = ssRow - lpSS->Row.nHeaderCnt + 1;

               if ( SS_SendMsgCoords( hWnd, SSM_DBLCLK,
                                      GetDlgCtrlID( hWnd ),
                                      ssColDblClk, ssRowDblClk ) )
               {
//                DrUnlockTaskMemory( lpSS );
                  return( FALSE );
               }

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( FALSE );
            }
            else
            if ( ssCol != -2 && ssRow != -2 )
            {
               hWndFocus = 0;

               if ( lpSS->EditModeOn )
                  hWndFocus = GetFocus( );

               if ( SS_SendMsgCoords( hWnd, SSM_CLICK,
                                      GetDlgCtrlID( hWnd ),
                                      ssCol == -1 ? SS_HEADER :
                                             ssCol - lpSS->Col.nHeaderCnt + 1,
                                      ssRow == -1 ? SS_HEADER :
                                             ssRow - lpSS->Row.nHeaderCnt + 1 ) )
               {
//                DrUnlockTaskMemory( lpSS );
                  return( FALSE );
               }

               if ( mIs_hWnd( hWnd ) == FALSE )
                  return( FALSE );
               else
                  UpdateWindow( hWnd );

               // This handles the case where the application changes
               // the active cell within the SSM_CLICK message

               if ( (ssCol != -1 && lpSS->Col.ssCurrAt != ssCol) ||
                    (ssRow != -1 && lpSS->Row.ssCurrAt != ssRow) )
                  lpSS->ModeBlock = FALSE;

               if ( lpSS->EditModeOn && hWndFocus && GetFocus( ) == hWnd )
                  SetFocus( hWndFocus );
            }

            if ( GetSystemMetrics( SM_SWAPBUTTON) )
            {
               if ( (GetAsyncKeyState( VK_RBUTTON ) & 0x8000) == 0 )
                  lpSS->fNoSetCapture = TRUE;
            }
            else
               if ( (GetAsyncKeyState( VK_LBUTTON ) & 0x8000) == 0 )
                  lpSS->fNoSetCapture = TRUE;

            if ( ssCol != -2 && ssRow != -2 &&
                 SS_RetrieveCellType( lpSS, &CellType, 0, ssCol, ssRow ) &&
                 (CellType.chType == SS_TYPE_BUTTON ||
                  CellType.chType == SS_TYPE_CHECKBOX) &&
                 (lpSS->wOpMode != SS_OPMODE_ROWMODE ||
                  lpSS->fRowModeEditing) &&
                 ssCol >= 0 && ssRow >= 0 &&
                 !SS_GetLock( hWnd, lpSS, ssCol, ssRow ) )
            {
               lpSS->Col.ssCurrAt = ssCol;
               lpSS->Row.ssCurrAt = ssRow;

               lpSS->ModeBlock = FALSE;

               SS_CellEditModeOn( hWnd, lpSS, WM_LBUTTONDOWN, 0,
                                  MAKELONG( lMouseX - x, lMouseY - y ) );
            }
            else
            if ( lpSS->EditModeOn == 0 && lpSS->HiliteOn == 0 )
               SS_HiliteCell( hWnd, TRUE );
         }

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( 0 );

         if ( fDone || lpSS->fNoSetCapture )
            lpSS->fNoSetCapture = FALSE;
         else
         if ( SS_USESELBAR( lpSS ) )
         {
            lpSS->ModeSelect = TRUE;
            SetCapture( hWnd );
         }
         else
         if ( fNoSetCapture )
            lpSS->ModeBlock = FALSE;
         else
         if ( lpSS->ModeBlock && lpSS->fKillFocusReceived == 0 )
            SetCapture( hWnd );

         lpSS->Col.PrevAt = lpSS->Col.ssCurrAt;
         lpSS->Row.PrevAt = lpSS->Row.ssCurrAt;
         lpSS->Col.PrevUL = lpSS->Col.UL;
         lpSS->Row.PrevUL = lpSS->Row.UL;
      }

      if ( lpSS->fInvertBlock )
      {
         lpSS->fSuspendInvert = FALSE;
         SS_HiliteCell( hWnd, TRUE );
         SS_InvertBlock( hWnd, 0, lpSS );
      }

//    DrUnlockTaskMemory( lpSS );
   }

   return( 0 );
}

zBOOL
SS_ResetBlock( HWND   hWnd,
               zBOOL  fDraw )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_ResetBlock" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   zBOOL         fBlockSelected = FALSE;
   zBOOL         bRC = FALSE;

   if ( SS_IsBlockSelected( lpSS ) )
      fBlockSelected = TRUE;

   if ( fDraw )
   {
      if ( GetFocus( ) == hWnd || lpSS->fRetainSelBlock )
         bRC = SS_DrawSelBlock( hWnd, 0, lpSS );
      else
         bRC = !lpSS->fSuspendInvert;
   }
   else
      bRC = TRUE;

   lpSS->fSelBlockInvisible = FALSE;

   if ( lpSS->fMultipleBlocksSelected )
   {
      if ( lpSS->MultiSelBlock.hItemList )
      {
         lpSS->MultiSelBlock.hItemList = (zPCHAR)
                        DrUnlockTaskMemory( lpSS->MultiSelBlock.hItemList );
         DrFreeTaskMemory( lpSS->MultiSelBlock.hItemList );
      }

      lpSS->MultiSelBlock.hItemList = 0;
      lpSS->MultiSelBlock.lItemCnt = 0;
      lpSS->MultiSelBlock.lItemAllocCnt = 0;
      lpSS->fMultipleBlocksSelected = FALSE;
   }

   if ( bRC )
   {
      lpSS->BlockCellUL.ssRow = -1;
      lpSS->BlockCellUL.ssCol = -1;

      lpSS->BlockCellLR.ssRow = -1;
      lpSS->BlockCellLR.ssCol = -1;
   }

   if ( fBlockSelected )
      SS_SendMsgCommand( hWnd, 0, SSN_BLOCKDESELECTED, FALSE );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

void
SS_Resize( HWND          hWnd,
           LPSPREADSHEET lpSS,
           zSHORT        nMode,
           zLONG         lPos )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_Resize" );
   HDC           hDC;
   RECT          rectClient;
   zLONG         lMaxX;
   zLONG         lMaxY;
   zLONG         lNewPos;
   zLONG         lCurPos;

   SS_GetClientRect( hWnd, &rectClient );
   hDC = GetDC( hWnd );
   if ( lpSS->ModeResize == SS_MODERESIZE_HORIZ )
   {
      lNewPos = HIWORD( lPos ) - lpSS->lResizeOffset;
   // y = SS_GetCellPosY( lpSS, 0, lpSS->Row.nHeaderCnt );
   // lNewPos = mMax( lNewPos, y );
      lNewPos = mMin( lNewPos, rectClient.bottom - 1 );
   }
   else
   {
      lNewPos = LOWORD( lPos ) - lpSS->lResizeOffset;
   // x = SS_GetCellPosX( lpSS, 0, lpSS->Col.nHeaderCnt );
   // lNewPos = mMax( lNewPos, x );
      lNewPos = mMin( lNewPos, rectClient.right - 1 );
   }

   lNewPos = mMax( lNewPos, lpSS->lResizeMinPos );

   if ( lpSS->ModeResize == SS_MODERESIZE_HORIZ )
      lMaxX = mMin( SS_GetCellPosX( lpSS, lpSS->Col.UL, lpSS->Col.LR ) +
                    SS_GetColWidth( lpSS, lpSS->Col.LR ), rectClient.right );
   else
      lMaxY = mMin( SS_GetCellPosY( lpSS, lpSS->Row.UL, lpSS->Row.LR ) +
                    SS_GetRowHeight( lpSS, lpSS->Row.LR ), rectClient.bottom );

   if ( nMode != SS_RESIZEMODE_START )
   {
      lCurPos = lpSS->lResizeCurrentPos;
      if ( lpSS->ModeResize == SS_MODERESIZE_HORIZ )
         PatBlt( hDC, 0, lCurPos, lMaxX, 1, DSTINVERT );
      else
         PatBlt( hDC, lCurPos, 0, 1, lMaxY, DSTINVERT );
   }

   if ( nMode != SS_RESIZEMODE_END )
   {
      if ( lpSS->ModeResize == SS_MODERESIZE_HORIZ )
         PatBlt( hDC, 0, lNewPos, lMaxX, 1, DSTINVERT );
      else
         PatBlt( hDC, lNewPos, 0, 1, lMaxY, DSTINVERT );
   }

   ReleaseDC( hWnd, hDC );
   lpSS->lResizeCurrentPos = lNewPos;
}

zBOOL
SS_ScrollWithMouse( HWND           hWnd,
                    LPSPREADSHEET  lpSS,
                    LPSS_CELLCOORD lpBlockCellLR,
                    LPSS_CELLCOORD lpCurrentPos,
                    LPSS_CELLCOORD lpNewPos,
                    zLONG          lMouseX,
                    zLONG          lMouseY )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_ScrollWithMouse" );
   RECT           rectClient;
   SS_COORD       ssRowAt;
   SS_COORD       ssColAt;
   SS_COORD       ssRow = -1;
   SS_COORD       ssCol = -1;
   zSHORT         nWidth;
   zSHORT         nHeight;
   zLONG          lLeft;
   zLONG          lTop;
   zLONG          x;
   zLONG          y;

   SS_GetClientRect( hWnd, &rectClient );
   x = SS_GetCellPosX( lpSS, 0, lpSS->Col.nHeaderCnt );
   lLeft = x;

   if ( lpBlockCellLR->ssCol != -1 && lMouseX >= rectClient.left &&
        lMouseX < rectClient.right )
   {
      if ( lpSS->Col.Frozen )
         ssColAt = lpSS->Col.nHeaderCnt;
      else
         ssColAt = lpSS->Col.UL;

      for ( ; ssColAt < SS_GetColsMax( lpSS ) && x < rectClient.right; )
      {
         nWidth = SS_GetColWidth( lpSS, ssColAt );

         if ( lMouseX >= rectClient.left + x &&
              lMouseX < rectClient.left + x + nWidth )
         {
            ssCol = ssColAt;
            break;
         }

         x += nWidth;

         if ( lpSS->Col.Frozen && ssColAt == lpSS->Col.nHeaderCnt +
             lpSS->Col.Frozen - 1 )
            ssColAt = mMax( lpSS->Col.UL, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
         else
            ssColAt++;
      }
   }

   y = SS_GetCellPosY( lpSS, 0, lpSS->Row.nHeaderCnt );
   lTop = y;

   if ( lpBlockCellLR->ssRow != -1 && lMouseY >= rectClient.top &&
       lMouseY < rectClient.bottom )
   {
      if ( lpSS->Row.Frozen )
         ssRowAt = lpSS->Row.nHeaderCnt;
      else
         ssRowAt = lpSS->Row.UL;

      for ( ; ssRowAt < SS_GetRowsMax( lpSS ) && y < rectClient.bottom; )
      {
         nHeight = SS_GetRowHeight( lpSS, ssRowAt );
         if ( lMouseY >= rectClient.top + y &&
              lMouseY < rectClient.top + y + nHeight )
         {
            ssRow = ssRowAt;
            break;
         }

         y += nHeight;

         if ( lpSS->Row.Frozen && ssRowAt == lpSS->Row.nHeaderCnt +
                                                       lpSS->Row.Frozen - 1 )
            ssRowAt = mMax( lpSS->Row.UL, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
         else
            ssRowAt++;
      }
   }

   if ( lpSS->fModeDragDrop )
   {
#ifndef SS_NODRAGDROP
      if ( ssCol == lpSS->DragDropCurrentCol &&
           ssRow == lpSS->DragDropCurrentRow )
      {
         return( FALSE );
      }
      else
         SS_DragDropDrawBox( hWnd, lpSS, FALSE );
#endif
   }
   else
   if ( ssCol == lpCurrentPos->ssCol && ssRow == lpCurrentPos->ssRow )
      return( FALSE );

   if ( ssCol == -1 && lpBlockCellLR->ssCol != -1 )
   {
      if ( lMouseX <= lLeft )
      {
         SS_ScrollLineUL( hWnd, lpSS, SS_F_SCROLL_LEFT );

         if ( lpSS->Col.Frozen )
            ssCol = lpSS->Col.nHeaderCnt;
         else
            ssCol = lpSS->Col.UL;
      }
      else
      {
         SS_ScrollLineLR( hWnd, lpSS, SS_F_SCROLL_RIGHT );
         ssCol = lpSS->Col.LR;
      }
   }
   else
   if ( ssCol == -1 && lpBlockCellLR->ssCol == -1 )
      ssCol = 0;

   if ( ssRow == -1 && lpBlockCellLR->ssRow != -1 )
   {
      if ( lMouseY <= lTop )
      {
         SS_ScrollLineUL( hWnd, lpSS, SS_F_SCROLL_UP );

         if ( lpSS->Row.Frozen )
            ssRow = lpSS->Row.nHeaderCnt;
         else
            ssRow = lpSS->Row.UL;
      }
      else
      {
         SS_ScrollLineLR( hWnd, lpSS, SS_F_SCROLL_DOWN );
         ssRow = lpSS->Row.LR;
      }
   }
   else
   if ( ssRow == -1 && lpBlockCellLR->ssRow == -1 )
      ssRow = 0;

   lpNewPos->ssCol = ssCol;
   lpNewPos->ssRow = ssRow;

   return( TRUE );
}

void
SS_SelectBlock( HWND          hWnd,
                LPSPREADSHEET lpSS,
                zLONG         lMouseX,
                zLONG         lMouseY )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_SelectBlock" );
   SS_CELLCOORD NewPos;
   SS_CELLCOORD CurrentPos;

   CurrentPos.ssCol = lpSS->Col.BlockCellCurrentPos;
   CurrentPos.ssRow = lpSS->Row.BlockCellCurrentPos;

   if ( SS_ScrollWithMouse( hWnd, lpSS, &lpSS->BlockCellLR, &CurrentPos,
                            &NewPos, lMouseX, lMouseY ) )
   {
      SSx_SelectBlock( hWnd, lpSS, NewPos.ssCol, NewPos.ssRow );
   }
}

void
SSx_SelectBlock( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 SS_COORD      ssCol,
                 SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SSx_SelectBlock" );
   HDC           hDC;
   SS_SELBLOCK   BlockRectNew;
   SS_SELBLOCK   BlockRectOld;
   SS_SELBLOCK   BlockRectUnion;
   RECT          rectOld;
   RECT          rect;
   HRGN          hRgn;
   HRGN          hRgn2;

   if ( (ssCol == lpSS->Col.BlockCellCurrentPos &&
         ssRow == lpSS->Row.BlockCellCurrentPos) ||
        lpSS->wOpMode == SS_OPMODE_EXTSEL )
   {
      return;
   }

   SS_SetBlock( &BlockRectOld, lpSS->BlockCellUL.ssCol,
                lpSS->BlockCellUL.ssRow, lpSS->BlockCellLR.ssCol,
                lpSS->BlockCellLR.ssRow );

   if ( SS_CreateBlockRect( hWnd, lpSS, &rectOld,
                            lpSS->BlockCellUL.ssCol,
                            lpSS->BlockCellUL.ssRow,
                            lpSS->BlockCellLR.ssCol,
                            lpSS->BlockCellLR.ssRow, FALSE ) == FALSE )
   {
      SetRectEmpty( &rectOld );
   }

   hRgn = CreateRectRgnIndirect( &rectOld );

#ifndef SS_NODRAGDROP
   if ( lpSS->BlockCellUL.ssCol != lpSS->Col.ssCurrAt ||
        lpSS->BlockCellUL.ssRow != lpSS->Row.ssCurrAt ||
        lpSS->BlockCellLR.ssCol != lpSS->Col.ssCurrAt ||
        lpSS->BlockCellLR.ssRow != lpSS->Row.ssCurrAt )
   {
      if ( lpSS->fAllowDragDrop && lpSS->fMultipleBlocksSelected == 0 )
         SS_AddDragDropRectToBlock( hWnd, lpSS, hRgn );
   }
#endif

   lpSS->BlockCellUL.ssCol = mMin( ssCol, lpSS->Col.ssCurrAt );
   lpSS->BlockCellUL.ssRow = mMin( ssRow, lpSS->Row.ssCurrAt );

   if ( lpSS->BlockCellLR.ssCol != -1 )
      lpSS->BlockCellLR.ssCol = mMax( ssCol, lpSS->Col.ssCurrAt );

   if ( lpSS->BlockCellLR.ssRow != -1 )
      lpSS->BlockCellLR.ssRow = mMax( ssRow, lpSS->Row.ssCurrAt );

   hDC = GetDC( hWnd );

   if ( SS_CreateBlockRect( hWnd, lpSS, &rect, lpSS->BlockCellUL.ssCol,
                          lpSS->BlockCellUL.ssRow, lpSS->BlockCellLR.ssCol,
                          lpSS->BlockCellLR.ssRow, FALSE) )
   {
      hRgn2 = CreateRectRgnIndirect( &rect );

#ifndef SS_NODRAGDROP
      if ( lpSS->fAllowDragDrop && lpSS->fMultipleBlocksSelected == 0 )
         SS_AddDragDropRectToBlock( hWnd, lpSS, hRgn2 );
#endif

      CombineRgn( hRgn, hRgn, hRgn2, RGN_XOR );
      DeleteObject( hRgn2 );
   }

   if ( lpSS->fMultipleBlocksSelected )
   {
      SS_SetBlock( &BlockRectNew, lpSS->BlockCellUL.ssCol,
                  lpSS->BlockCellUL.ssRow, lpSS->BlockCellLR.ssCol,
                  lpSS->BlockCellLR.ssRow );
      BlockRectUnion.UL.ssCol = mMin( BlockRectNew.UL.ssCol, BlockRectOld.UL.ssCol );
      BlockRectUnion.UL.ssRow = mMin( BlockRectNew.UL.ssRow, BlockRectOld.UL.ssRow );
      BlockRectUnion.LR.ssCol = mMax( BlockRectNew.LR.ssCol, BlockRectOld.LR.ssCol );
      BlockRectUnion.LR.ssRow = mMax( BlockRectNew.LR.ssRow, BlockRectOld.LR.ssRow );

      if ( SS_CreateBlockRect( hWnd, lpSS, &rect, BlockRectUnion.UL.ssCol,
                               BlockRectUnion.UL.ssRow, BlockRectUnion.LR.ssCol,
                               BlockRectUnion.LR.ssRow, FALSE) )
      {
         hRgn = SSxx_SelectBlock( hWnd, lpSS, hRgn, &rect,
                                  lpSS->MultiSelBlock.lItemCnt );
      }
   }

   SelectClipRgn( hDC, hRgn );
   InvertRgn( hDC, hRgn );
   DeleteObject( hRgn );
   SelectClipRgn( hDC, 0 );

   ReleaseDC( hWnd, hDC );

   if ( lpSS->Row.BlockCellCurrentPos != ssRow ||
        lpSS->Col.BlockCellCurrentPos != ssCol )
   {
      lpSS->Row.BlockCellCurrentPos = ssRow;
      lpSS->Col.BlockCellCurrentPos = ssCol;

      SS_SendMsgCommand( hWnd, 0, SSN_BLOCKCHANGED, FALSE );
   }
}

HRGN
SSxx_SelectBlock( HWND          hWnd,
                  LPSPREADSHEET lpSS,
                  HRGN          hRgnMain,
                  LPRECT        lpRectMain,
                  zLONG         lItemCnt )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SSxx_SelectBlock" );
   LPSS_SELBLOCK lpItemList;
   RECT          rect;
   HRGN          hRgn2;
   HRGN          hRgnPrev = 0;
   zLONG         k;

   if ( lpSS->fMultipleBlocksSelected )
   {
//    lpItemList = (LPSS_SELBLOCK) DrLockTaskMemory( lpSS->MultiSelBlock.hItemList );
      lpItemList = (LPSS_SELBLOCK) lpSS->MultiSelBlock.hItemList;
      for ( k = 0; k < lItemCnt; k++ )
      {
         if ( SS_CreateBlockRect( hWnd, lpSS, &rect, lpItemList[ k ].UL.ssCol,
                                  lpItemList[ k ].UL.ssRow, lpItemList[ k ].LR.ssCol,
                                  lpItemList[ k ].LR.ssRow, FALSE) )
         {
            if ( !hRgnPrev )
            {
               hRgnPrev = CreateRectRgnIndirect( &rect );
            }
            else
            {
               hRgn2 = CreateRectRgnIndirect( &rect );
               CombineRgn( hRgnPrev, hRgnPrev, hRgn2, RGN_OR );
               DeleteObject( hRgn2 );
            }
         }
      }

//    DrUnlockTaskMemory( lpItemList );

      if ( hRgnPrev )
      {
         HRGN hRgnDest;

         hRgnDest = CreateRectRgn( 0, 0, 0, 0 );
         CombineRgn( hRgnDest, hRgnMain, hRgnPrev, RGN_DIFF );
         DeleteObject( hRgnMain );
         DeleteObject( hRgnPrev );
         hRgnMain = hRgnDest;
      }
   }

   return( hRgnMain );
}


void
SS_SelectRow( HWND hWnd,
              LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_SelectRow" );
   SSx_SelectRow( hWnd, lpSS, lpSS->Row.ssCurrAt );
}

void
SSx_SelectRow( HWND     hWnd,
               LPSPREADSHEET lpSS,
               SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SSx_SelectRow" );
   LPSS_ROW lpRow;
   LPSS_ROW lpRowTemp;
   SS_COORD ssRow1 = 0;
   SS_COORD ssRow2 = 0;
   SS_COORD ss;
   HDC      hDC;
   zBOOL    fInvert = FALSE;

   if ( (lpRow = SS_AllocLockRow( lpSS, ssRow)) != 0 )
   {
      // Extended Selection.
      if ( lpSS->wOpMode == SS_OPMODE_EXTSEL )
      {
         hDC = GetDC( hWnd );

         if ( lpSS->BlockCellUL.ssRow == -1 ||
              ssRow < lpSS->BlockCellUL.ssRow ||
              ssRow > lpSS->BlockCellLR.ssRow )
         {
            if ( lpSS->BlockCellUL.ssRow == -1 )
            {
               ssRow1 = ssRow;
               ssRow2 = ssRow;
               lpSS->BlockCellUL.ssRow = ssRow;
               lpSS->BlockCellLR.ssRow = ssRow;
            }
            else
            if ( ssRow < lpSS->BlockCellUL.ssRow )
            {
               ssRow1 = ssRow;
               ssRow2 = lpSS->BlockCellUL.ssRow - 1;
               lpSS->BlockCellUL.ssRow = ssRow;
            }
            else
            {
               ssRow1 = lpSS->BlockCellLR.ssRow + 1;
               ssRow2 = ssRow;
               lpSS->BlockCellLR.ssRow = ssRow;
            }

            for ( ss = ssRow1; ss <= ssRow2; ss++ )
               if ( (lpRowTemp = SS_AllocLockRow( lpSS, ss )) != 0 )
               {
                  if ( lpSS->fExtSelHilite )
                  {
                     if ( lpRowTemp->fRowSelected == 0 )
                     {
                        lpSS->MultiSelCnt++;
                        lpRowTemp->fRowSelected = TRUE;
                        SS_InvertRowBlock( hWnd, hDC, lpSS, ss, ss );
                     }
                     else
                         lpRowTemp->fRowSelected = 3;
                  }
                  else
                  {
                     if ( lpRowTemp->fRowSelected )
                     {
                        lpSS->MultiSelCnt--;
                        lpRowTemp->fRowSelected = FALSE;
                        SS_InvertRowBlock( hWnd, hDC, lpSS, ss, ss );
                     }
                     else
                         lpRowTemp->fRowSelected = 2;
                   }

                   SS_UnlockRowItem( lpSS, ss );
               }

         }
         else
         {
            if ( lpSS->Row.BlockCellCurrentPos == lpSS->BlockCellUL.ssRow )
            {
               ssRow1 = lpSS->BlockCellUL.ssRow;
               ssRow2 = ssRow - 1;
               lpSS->BlockCellUL.ssRow = ssRow;
            }

            if ( lpSS->Row.BlockCellCurrentPos == lpSS->BlockCellLR.ssRow )
            {
               ssRow1 = ssRow + 1;
               ssRow2 = lpSS->BlockCellLR.ssRow;
               lpSS->BlockCellLR.ssRow = ssRow;
            }

            for ( ss = ssRow1; ss <= ssRow2; ss++ )
            {
               if ( (lpRowTemp = SS_LockRowItem( lpSS, ss )) != 0 )
               {
                  if ( lpSS->fExtSelHilite )
                  {
                     if ( lpRowTemp->fRowSelected == 1 )
                     {
                        lpSS->MultiSelCnt--;
                        lpRowTemp->fRowSelected = FALSE;
                        SS_InvertRowBlock( hWnd, hDC, lpSS, ss, ss );
                     }
                     else
                     if ( lpRowTemp->fRowSelected == 3 )
                        lpRowTemp->fRowSelected = TRUE;
                  }
                  else
                  {
                     if ( lpRowTemp->fRowSelected == 0 )
                     {
                        lpSS->MultiSelCnt++;
                        lpRowTemp->fRowSelected = TRUE;
                        SS_InvertRowBlock( hWnd, hDC, lpSS, ss, ss );
                     }
                     else
                     if ( lpRowTemp->fRowSelected == 2 )
                        lpRowTemp->fRowSelected = FALSE;
                  }

                  SS_UnlockRowItem( lpSS, ss );
               }
            }
         }

         ReleaseDC( hWnd, hDC );
         fInvert = FALSE;

         lpSS->Row.BlockCellCurrentPos = ssRow;
      }
      else     // multiple selection
      {
         if ( lpRow->fRowSelected )
         {
            lpSS->MultiSelCnt--;
            lpRow->fRowSelected = FALSE;
         }
         else
         {
            lpSS->MultiSelCnt++;
            lpRow->fRowSelected = TRUE;
         }

         hDC = GetDC( hWnd );
         SS_InvertRowBlock( hWnd, hDC, lpSS, ssRow, ssRow );
         ReleaseDC( hWnd, hDC );
      }

      SS_UnlockRowItem( lpSS, ssRow );
   }
}

void OPERATION
SSScrollEvent( UINT scrollCode, UINT thumbPos,
               HWND hWndCtrl, zBOOL bVertical )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SSScrollEvent" );
   LPSPREADSHEET lpSS = SS_Lock( hWndCtrl );

// TraceLineI( "SSScrollEvent bVertical: ", bVertical );
   if ( bVertical )
   {
      lpSS->fEditModeDataChange = FALSE;
      lpSS->wScrollDirection = 0;
      lpSS->Row.ULPrev = lpSS->Row.UL;

// #ifndef ZEIDON
      UpdateWindow( hWndCtrl );
// #endif

      if ( SS_USESINGLESELBAR( lpSS ) )
         if ( GetFocus( ) != hWndCtrl )
            SetFocus( hWndCtrl );

      switch ( scrollCode )
      {
         // line up
         case SB_LINEUP:
            SS_ScrollLineUL( hWndCtrl, lpSS,
                             SS_F_SCROLL_UP | SS_F_SKIP_UNHILITE );
            break;

         // line down
         case SB_LINEDOWN:
            SS_ScrollLineLR( hWndCtrl, lpSS,
                             SS_F_SCROLL_DOWN | SS_F_SKIP_UNHILITE );
            break;

         // page down
         case SB_PAGEDOWN:
            SS_ScrollPageLR( hWndCtrl, lpSS, SS_F_SCROLL_DOWN );
            break;

         // page up
         case SB_PAGEUP:
            SS_ScrollPageUL( hWndCtrl, lpSS, SS_F_SCROLL_UP );
            break;

         // top
         case SB_TOP:
            SS_ScrollHome( hWndCtrl, lpSS, SS_F_SCROLL_COL );
            break;

         // bottom
         case SB_BOTTOM:
            SS_ScrollEnd( hWndCtrl, lpSS, SS_F_SCROLL_COL | SS_F_SCROLLMAX );
            break;

         // slider position has changed
         case SB_THUMBPOSITION:
         case SB_THUMBTRACK:
         {
            SS_COORD ssRow;
            SS_COORD yPos;

            if ( lpSS->fScrollBarMaxAlign )
               ssRow = SS_GetLastPageTopRow( hWndCtrl, lpSS );
            else
               ssRow = SS_GetRowsMax( lpSS ) - 1;

            if ( lpSS->fVirtualMode &&
                 (lpSS->Virtual.lVStyle & SSV_SCROLLBARSHOWBUFFER) )
            {
               yPos = thumbPos + lpSS->Virtual.VTop - 1;
            }
            else
            if ( ssRow <= SS_SBMAX )
               yPos = thumbPos;
            else
               yPos = (ssRow * (thumbPos / SS_SBMAX));

            // If in virtual mode and the ScrollBarShowBuffer style
            // is not being used, then do not do anything.
            if ( lpSS->fVirtualMode == 0 ||
                 (lpSS->Virtual.lVStyle & SSV_SCROLLBARSHOWBUFFER) ||
                 (lpSS->Virtual.VMax != -1 && lpSS->Row.Max) )
            {
               // 1999.12.03 fix check for yPos being non-zero
               if ( yPos && yPos <= ssRow && yPos != lpSS->Row.UL )
                  SS_VPos( hWndCtrl, yPos );
            }

            break;
         }
      }
   }
   else
   {
      lpSS->fEditModeDataChange = FALSE;
      lpSS->Col.ULPrev = lpSS->Col.UL;
      UpdateWindow( hWndCtrl );

      if ( SS_USESINGLESELBAR( lpSS ) )
         if ( GetFocus( ) != hWndCtrl )
            SetFocus( hWndCtrl );

      switch ( scrollCode )
      {
         // scroll one line to the left
         case SB_LINEUP:
            SS_ScrollLineUL( hWndCtrl, lpSS, SS_F_SCROLL_LEFT );
            break;

         // scroll one line to the right
         case SB_LINEDOWN:
            SS_ScrollLineLR( hWndCtrl, lpSS, SS_F_SCROLL_RIGHT );
            break;

         // scroll one page right
         case SB_PAGEDOWN:
            SS_ScrollPageLR( hWndCtrl, lpSS, SS_F_SCROLL_RIGHT );
            break;

         // scroll one page left
         case SB_PAGEUP:
            SS_ScrollPageUL( hWndCtrl, lpSS, SS_F_SCROLL_LEFT );
            break;

         // slider position has changed
         case SB_THUMBPOSITION:
         {
            SS_COORD ssCol;

            if ( lpSS->fScrollBarMaxAlign )
               ssCol = SS_GetLastPageLeftCol( hWndCtrl, lpSS );
            else
               ssCol = SS_GetColsMax( lpSS ) - 1;

            if ( thumbPos <= (UINT) ssCol && thumbPos != (UINT) lpSS->Col.UL )
            {
               SS_HPos( hWndCtrl, thumbPos );
            }

            break;
         }
      }
   }

// DrUnlockTaskMemory( lpSS );
}


// ss_virt.cpp

zBOOL
SS_VClear( HWND hWnd,
           SS_COORD ssRow1,
           SS_COORD ssRow2 )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SS_VClear" );
   SS_VCLEARDATA VClearData;

   VClearData.ssRow = ssRow1;
   VClearData.ssRowsBeingCleared = ssRow2 - ssRow1 + 1;

   if ( VClearData.ssRowsBeingCleared > 0 )
   {
      if ( !SS_SendMsg( hWnd, SSM_VCLEARDATA,
                        GetWindowLong( hWnd, GWL_ID ),
                        (LPARAM) &VClearData ) )
      {
         return( SSClearRange( hWnd, SS_ALLCOLS, ssRow1, SS_ALLCOLS, ssRow2 ) );
      }
   }

   return( TRUE );
}

void
SS_VRefreshBuffer( HWND hWnd,
                   LPSPREADSHEET lpSS,
                   zBOOL fTop )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SS_VRefreshBuffer" );
   SS_VQUERYDATA VQueryData;
   SS_COORD      BottomVisCell;
   SS_COORD      ssRowsPerScreen;
   SS_COORD      ssRowOld;
   SS_COORD      Size;
   zBOOL         fRedraw;

   fRedraw = lpSS->Redraw;
   lpSS->Redraw = FALSE;

   zmemset( &VQueryData, 0, zsizeof( SS_VQUERYDATA ) );

   SS_VClear( hWnd, lpSS->Virtual.VTop,
              lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 );

   if ( fTop )
      VQueryData.ssRow = 1;
   else
      VQueryData.ssRow = mMax( lpSS->Virtual.VTop, 1 );

   VQueryData.wDirection = SS_VREFRESH;

   BottomVisCell = SS_GetBottomCell( hWnd, lpSS, lpSS->Row.UL );

   ssRowsPerScreen = BottomVisCell - lpSS->Row.UL + 1;
   Size = mMax( lpSS->Virtual.VSize, ssRowsPerScreen );

   if ( VQueryData.ssRow + Size > lpSS->Row.Max )
      Size = lpSS->Row.Max - VQueryData.ssRow;

   if ( fTop )
      VQueryData.ssRowsNeeded = mMax( lpSS->Virtual.VSize, ssRowsPerScreen );
   else
      VQueryData.ssRowsNeeded = mMax( mMax( lpSS->Virtual.VSize, ssRowsPerScreen ),
                                    lpSS->Virtual.VPhysSize );

   if ( VQueryData.ssRowsNeeded && Size > 0 )
   {
      VQueryData.ssRowsNeeded = mMin( VQueryData.ssRowsNeeded, Size );

      ssRowOld = VQueryData.ssRow;
      SS_SendMsg( hWnd, SSM_VQUERYDATA, GetWindowLong( hWnd, GWL_ID ),
                  (LPARAM) &VQueryData );

      lpSS->Virtual.VTop = VQueryData.ssRow;
      lpSS->Virtual.VPhysSize = VQueryData.ssRowsLoaded;

      if ( VQueryData.fAtBottom &&
           lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 < lpSS->Row.Max &&
           lpSS->Virtual.VMax == -1 )
      {
         SS_SetMaxRows( hWnd,
                        lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 );
      }

      if ( lpSS->fVirtualMode &&
           (lpSS->Virtual.lVStyle & SSV_SCROLLBARSHOWBUFFER) )
      {
         SS_SetVScrollBar( hWnd, lpSS );
      }
   }

   if ( VQueryData.fAtTop && lpSS->Virtual.VTop > lpSS->Row.nHeaderCnt )
   {
      ::MessageBox( 0, "SSMoveRangeCall", "VQueryData 2693", MB_OK );
#ifdef DKS_COPY
      SSMoveRange( hWnd, SS_ALLCOLS, lpSS->Virtual.VTop, SS_ALLCOLS,
                   lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1,
                   SS_ALLCOLS, lpSS->Row.nHeaderCnt );

      lpSS->Row.UL = mMax( lpSS->Row.nHeaderCnt,
                          lpSS->Row.UL -
                                (lpSS->Virtual.VTop - lpSS->Row.nHeaderCnt) );
      lpSS->Row.ssCurrAt = mMax( lpSS->Row.nHeaderCnt,
                             lpSS->Row.ssCurrAt -
                                (lpSS->Virtual.VTop - lpSS->Row.nHeaderCnt) );

      lpSS->Virtual.VTop = lpSS->Row.nHeaderCnt;
      InvalidateRect( hWnd, 0, TRUE );
#endif
   }

   if ( lpSS->Virtual.VTop > lpSS->Row.UL ||
        lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 < BottomVisCell )
   {
      SS_ShowCell( hWnd, SS_SHOW_TOPLEFT, lpSS->Col.UL, lpSS->Virtual.VTop );

      if ( ssRowOld != VQueryData.ssRow )
         lpSS->Row.ssCurrAt = lpSS->Row.UL;
   }

   lpSS->Redraw = fRedraw;
}

zBOOL OPERATION
SSVRefreshBuffer( HWND hWnd )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVRefreshBuffer" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   SS_VRefreshBuffer( hWnd, lpSS, FALSE );
// DrUnlockTaskMemory( lpSS );

   return( TRUE );
}

#ifndef SS_NOSCBAR

zBOOL OPERATION
SSVScrollGetSpecial( HWND  hWnd,
                     WORD  *lpwOptions )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVScrollGetSpecial" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   zBOOL         bRC = lpSS->fUseSpecialVScroll;

   if ( lpwOptions )
      *lpwOptions = lpSS->wSpecialVScrollOptions;

// DrUnlockTaskMemory( lpSS );

   return( bRC );
}

zBOOL OPERATION
SSVScrollSetSpecial( HWND hWnd,
                     zBOOL fUseSpecialVScroll,
                     WORD  wOptions )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVScrollSetSpecial" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   zLONG         lStyle;
   zBOOL         bRC;

   bRC = lpSS->fUseSpecialVScroll;
   lpSS->fUseSpecialVScroll = fUseSpecialVScroll;
   lpSS->wSpecialVScrollOptions = wOptions;

   if ( fUseSpecialVScroll )
   {
      lStyle = SBRS_HOMEEND | SBRS_UPDN | SBRS_PAGEUPDN;

      if ( wOptions & SS_VSCROLL_NOHOMEEND )
         lStyle &= ~SBRS_HOMEEND;

      if ( wOptions & SS_VSCROLL_NOUPDN )
         lStyle &= ~SBRS_UPDN;

      if ( wOptions & SS_VSCROLL_NOPAGEUPDN )
         lStyle &= ~SBRS_PAGEUPDN;

      if ( !bRC )
      {
         if ( lpSS->hWndVScroll )
            DestroyWindow( lpSS->hWndVScroll );

         lpSS->hWndVScroll =
            CreateWindow( SSClassNames[ 0 ].TBScrlBar, 0,
                          WS_CHILD | WS_VISIBLE | WS_BORDER | lStyle,
                          0, 0, 0, 0, hWnd, 0, g_hInstanceDLL, 0 );

         SS_AutoSize( hWnd, FALSE );
      }
      else
      {
         SetWindowLong( lpSS->hWndVScroll, GWL_STYLE,
                        (GetWindowLong( lpSS->hWndVScroll, GWL_STYLE) &
                        ~(SBRS_HOMEEND | SBRS_UPDN | SBRS_PAGEUPDN)) | lStyle );
         InvalidateRect( lpSS->hWndVScroll, 0, TRUE );
         UpdateWindow( lpSS->hWndVScroll );
      }
   }
   else
   {
      if ( bRC )
      {
         if ( lpSS->hWndVScroll )
            DestroyWindow( lpSS->hWndVScroll );

         lpSS->hWndVScroll = CreateWindow( "ScrollBar", 0, WS_CHILD |
                                           WS_VISIBLE | SBS_VERT, 0, 0, 0, 0,
                                           hWnd, 0, g_hInstanceDLL, 0 );
         SS_AutoSize( hWnd, FALSE );
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

#endif

zLONG OPERATION
SSVGetStyle( HWND hWnd )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVGetStyle" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   zLONG         lRet;

   lRet = lpSS->Virtual.lVStyle;
// DrUnlockTaskMemory( lpSS );

   return( lRet );
}

zLONG OPERATION
SSVSetStyle( HWND  hWnd,
             zLONG lStyle )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVSetStyle" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   zLONG         lRet;

   lRet = lpSS->Virtual.lVStyle;
   lpSS->Virtual.lVStyle = lStyle;
// DrUnlockTaskMemory( lpSS );

   return( lRet );
}

void OPERATION
SSVGetBufferSize( HWND hWnd,
                  LPSS_COORD lpBufferSize,
                  LPSS_COORD lpOverlap )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVGetBufferSize" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );

   if ( lpBufferSize )
      *lpBufferSize = lpSS->Virtual.VSize;

   if ( lpOverlap )
      *lpOverlap = lpSS->Virtual.VOverlap;

// DrUnlockTaskMemory( lpSS );
}


SS_COORD OPERATION
SSVGetPhysBufferTop( HWND hWnd )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVGetPhysBufferTop" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   SS_COORD      ssRet;

   ssRet = lpSS->Virtual.VTop;
// DrUnlockTaskMemory( lpSS );

   return( ssRet );
}

SS_COORD OPERATION
SSVGetPhysBufferSize( HWND hWnd )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVGetPhysBufferSize" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   SS_COORD      ssRet;

   ssRet = lpSS->Virtual.VPhysSize;
// DrUnlockTaskMemory( lpSS );

   return( ssRet );
}

SS_COORD OPERATION
SSVGetMax( HWND hWnd )
{
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   SS_COORD      ssRet;

   ssRet = lpSS->Virtual.VMax;
// DrUnlockTaskMemory( lpSS );

   return( ssRet );
}

void OPERATION
SSVSetBufferSize( HWND hWnd,
                  SS_COORD BufferSize,
                  SS_COORD Overlap )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVSetBufferSize" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );

   lpSS->Virtual.VSize = BufferSize;
   lpSS->Virtual.VOverlap = Overlap;
// DrUnlockTaskMemory( lpSS );
}

SS_COORD OPERATION
SSVSetMax( HWND hWnd,
           SS_COORD Max )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SSVSetMax" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   SS_COORD      ssRet;

   ssRet = lpSS->Virtual.VMax;
   lpSS->Virtual.VMax = Max;

   if ( lpSS->fVirtualMode )
   {
      if ( Max == -1 )
         SS_SetMaxRows( hWnd, SS_VMAXROWS );
      else
      {
         SS_SetMaxRows( hWnd, Max );

         if ( Max < lpSS->Virtual.VTop )
         {
            lpSS->Virtual.VTop = 1;
            lpSS->Virtual.VPhysSize = 0;
         }
         else
         if ( Max < lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 )
            lpSS->Virtual.VPhysSize = Max - lpSS->Virtual.VTop + 1;
      }
   }

// DrUnlockTaskMemory( lpSS );

   return( ssRet );
}

zBOOL
SS_VQueryData( HWND hWnd,
               LPSPREADSHEET lpSS,
               SS_COORD BottomVisCell )
{
   TRACE_DEBUG( "<SS_VIRT.CPP>", "SS_VQueryData" );
// TraceLineS( "<SS_VIRT.CPP>", "SS_VQueryData" );
   return( 0 );

#if 0  // don't know why this was taken out a long time ago
   SS_VQUERYDATA VQueryData;
   SS_COORD      ssRowsPerScreen;
   SS_COORD      VSize;
   SS_COORD      ssRowNew;
   SS_COORD      VNewTop;
   SS_COORD      VPhysOverlap;
   SS_COORD      VOverlap;
   SS_COORD      ssRowsNeeded;
   SS_COORD      VQueryDataRowOffset = 0;
   zBOOL         fRedraw;
   zBOOL         bRC = TRUE;

   fRedraw = lpSS->Redraw;
   lpSS->Redraw = FALSE;

   lpSS->fProcessingVQueryData = TRUE;

   zmemset( &VQueryData, 0, zsizeof( SS_VQUERYDATA ) );

   ssRowsPerScreen = BottomVisCell - lpSS->Row.UL + 1;

   VSize = mMax( lpSS->Virtual.VSize, ssRowsPerScreen );
   VPhysOverlap = mMax( lpSS->Virtual.VOverlap, ssRowsPerScreen );

   // Check to see if requested row is above buffer.
   if ( lpSS->Row.UL < lpSS->Virtual.VTop )
   {
      VOverlap = 0;
      ssRowNew = lpSS->Row.UL;
      ssRowsNeeded = VSize;

      if ( lpSS->Virtual.VPhysSize )
      {
         // Set up overlap portion of buffer
         if ( lpSS->Virtual.VTop - lpSS->Row.UL <= VSize )
         {
            ssRowsNeeded = mMin( VSize, lpSS->Virtual.VTop - 1 );
            VOverlap = mMin( VPhysOverlap, lpSS->Virtual.VPhysSize );
            ssRowNew = lpSS->Virtual.VTop - ssRowsNeeded;
         }

         // Clear any un-needed rows
         if ( VOverlap <= lpSS->Virtual.VPhysSize - 1 )
            SS_VClear( hWnd, lpSS->Virtual.VTop + VOverlap,
                       lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 );
      }

      if ( bRC && ssRowsNeeded )
      {
         VQueryData.ssRowsNeeded = ssRowsNeeded;

         if ( lpSS->wScrollDirection == SS_VSCROLL_HOME )
         {
            VQueryData.wDirection = SS_VTOP;
            VQueryData.ssRow = lpSS->Row.nHeaderCnt;
         }
         else
         {
            VQueryData.wDirection = SS_VUP;
            VQueryData.ssRow = ssRowNew;
         }

         VQueryDataRowOffset = VQueryData.ssRow;
         ssRowNew = VQueryData.ssRow;       // 6/5/93
         lpSS->Virtual.VTop = ssRowNew;

         SS_SendMsg( hWnd, SSM_VQUERYDATA,
                     GetWindowLong( hWnd, GWL_ID ),
                     (LPARAM) &VQueryData );

         VQueryDataRowOffset = VQueryData.ssRow - VQueryDataRowOffset;

         VQueryData.ssRowsLoaded = mMin( VQueryData.ssRowsLoaded, ssRowsNeeded );
         lpSS->Virtual.VPhysSize = VQueryData.ssRowsLoaded + VOverlap;

         if ( VOverlap != 0 && VQueryData.ssRowsLoaded < ssRowsNeeded )
         {
            ::MessageBox( 0, "SSMoveRange Call", "VOverlap 3035", MB_OK );
#ifdef DKS_COPY
            SSMoveRange( hWnd, SS_ALLCOLS, lpSS->Virtual.VTop,
                         SS_ALLCOLS, lpSS->Virtual.VTop +
                         VQueryData.ssRowsLoaded - 1, SS_ALLCOLS,
                         lpSS->Virtual.VTop + (RowsNeeded -
                                                   VQueryData.ssRowsLoaded) );

            lpSS->Virtual.VTop += ssRowsNeeded - VQueryData.ssRowsLoaded;
#endif
         }

         if ( lpSS->fVirtualMode &&
              (lpSS->Virtual.lVStyle & SSV_SCROLLBARSHOWBUFFER) )
         {
            SS_SetVScrollBar( hWnd, lpSS );
         }
      }
   }
   else  // Check to see if requested row is below buffer
   if ( BottomVisCell >= lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize )
   {
      VOverlap = 0;
      ssRowNew = lpSS->Row.UL;
      VNewTop = lpSS->Row.UL;

      if ( lpSS->Virtual.VPhysSize )
      {
         if ( lpSS->Row.UL - lpSS->Virtual.VTop > lpSS->Virtual.VPhysSize )
         {
            if ( lpSS->Virtual.VTop <= lpSS->Virtual.VTop +
                 lpSS->Virtual.VPhysSize - 1 )
            {
               SS_VClear( hWnd, lpSS->Virtual.VTop,
                          lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 );
            }
         }
         else
         {
            VOverlap = mMin( VPhysOverlap, lpSS->Virtual.VPhysSize );
            ssRowNew = lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize;
            VNewTop = lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - VOverlap;

            if ( lpSS->Virtual.VTop <= VNewTop - 1 )
               bRC = SS_VClear( hWnd, lpSS->Virtual.VTop, VNewTop - 1 );
         }
      }

      if ( bRC )
      {
         VQueryData.ssRow = ssRowNew;

         if ( lpSS->wScrollDirection == SS_VSCROLL_END )
         {
            VQueryData.wDirection = SS_VBOTTOM;
            if ( VOverlap == 0 )
            {
               VQueryData.ssRow = lpSS->Row.Max - VSize + 1;
               VNewTop = VQueryData.ssRow;
            }
         }
         else
            VQueryData.wDirection = SS_VDOWN;

         VQueryData.ssRowsNeeded = mMin( VSize,
                                       lpSS->Row.Max - VQueryData.ssRow + 1 );

         if ( VQueryData.ssRowsNeeded )
         {
            VQueryDataRowOffset = VQueryData.ssRow;
            lpSS->Virtual.VTop = VNewTop;

            SS_SendMsg( hWnd, SSM_VQUERYDATA,
                        GetWindowLong( hWnd, GWL_ID ),
                        (LPARAM) &VQueryData );

            VQueryDataRowOffset = VQueryData.ssRow - VQueryDataRowOffset;

            lpSS->Virtual.VPhysSize = VQueryData.ssRowsLoaded + VOverlap;

            if ( VQueryData.fAtBottom &&
                 lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 <
                                                            lpSS->Row.Max &&
                 lpSS->Virtual.VMax == -1 )
            {
               SS_SetMaxRows( hWnd, lpSS->Virtual.VTop +
                                               lpSS->Virtual.VPhysSize - 1 );
            }

            if ( lpSS->fVirtualMode &&
                 (lpSS->Virtual.lVStyle & SSV_SCROLLBARSHOWBUFFER) )
            {
               SS_SetVScrollBar( hWnd, lpSS );
            }
         }
      }
   }

   if ( VQueryData.fAtTop && lpSS->Virtual.VTop > lpSS->Row.nHeaderCnt )
   {
      ::MessageBox( 0, "SSMoveRange Call", "VQueryData 3136", MB_OK );
#ifdef DKS_COPY
      SSMoveRange( hWnd, SS_ALLCOLS, lpSS->Virtual.VTop, SS_ALLCOLS,
                   lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1,
                   SS_ALLCOLS, lpSS->Row.nHeaderCnt );

      lpSS->Row.UL = mMax( lpSS->Row.nHeaderCnt,
                          lpSS->Row.UL -
                               (lpSS->Virtual.VTop - lpSS->Row.nHeaderCnt) );
      lpSS->Row.ssCurrAt = mMax( lpSS->Row.nHeaderCnt,
                             lpSS->Row.ssCurrAt -
                               (lpSS->Virtual.VTop - lpSS->Row.nHeaderCnt) );

      lpSS->Virtual.VTop = lpSS->Row.nHeaderCnt;
      lpSS->Row.DataCnt = lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize;
      InvalidateRect( hWnd, 0, TRUE );
#endif
   }

   if ( VQueryDataRowOffset )
   {
      VNewTop = mMax( lpSS->Row.nHeaderCnt,
                     lpSS->Virtual.VTop + VQueryDataRowOffset );

      if ( VNewTop != lpSS->Virtual.VTop )
      {
         ::MessageBox( 0, "SSMoveRange Call", "Virtual VTop 3162", MB_OK );
#ifdef DKS_COPY
         SSMoveRange( hWnd, SS_ALLCOLS, lpSS->Virtual.VTop, SS_ALLCOLS,
                      lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1,
                      SS_ALLCOLS, VNewTop );

         lpSS->Row.UL = mMax( lpSS->Row.nHeaderCnt,
                             lpSS->Row.UL - (lpSS->Virtual.VTop - VNewTop) );
         lpSS->Row.ssCurrAt = mMax( lpSS->Row.nHeaderCnt,
                                lpSS->Row.ssCurrAt -
                                            (lpSS->Virtual.VTop - VNewTop) );

         lpSS->Virtual.VTop = VNewTop;
         lpSS->Row.DataCnt = lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize;
         InvalidateRect( hWnd, 0, TRUE );
#endif
      }
   }

   lpSS->Row.AllocCnt = lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize;
   lpSS->Redraw = fRedraw;
   lpSS->fProcessingVQueryData = FALSE;

   return( VQueryData.fAtTop || VQueryData.fAtBottom );
#endif
}


// superbtn.cpp

zBOOL
SuperBtnFocusRect( HDC     hDC,
                   LPRECT  lpRect,
                   HBRUSH  hBrush )
{
   TRACE_DEBUG( "<SUPERBTN.CPP>", "SuperBtnFocusRect" );
   int      nOldROP;
   RECT     rect;
   HBRUSH   hOldBrush;
   HPEN     hOldPen;
   COLORREF clrTextOld;

   clrTextOld = SetTxColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );

   nOldROP = SetROP2( hDC, R2_XORPEN );
   hOldBrush = SelectBrush( hDC, hBrush );
   UnrealizeObject( hBrush );
   hOldPen = SelectPen( hDC, (HPEN) GetStockObject( NULL_PEN ) );

   // Draw the dotted lines.
   // Draw the top line.
   rect.bottom = rect.top = lpRect->top;
   rect.bottom += 2;
   rect.left = lpRect->left;
   rect.right = lpRect->right;
   Rectangle( hDC, rect.left, rect.top, rect.right, rect.bottom );

   // Draw the bottom line.
   rect.bottom = rect.top = lpRect->bottom;
   rect.top -= 2;
   Rectangle( hDC, rect.left, rect.top, rect.right, rect.bottom );

   // Draw do the left vertical line.
   rect.top = lpRect->top + 1;
   rect.bottom = lpRect->bottom;
   rect.right = rect.left = lpRect->left;
   rect.right += 2;
   Rectangle( hDC, rect.left, rect.top, rect.right, rect.bottom );

   // Draw the right vertical line.
   rect.right = rect.left = lpRect->right;
   rect.left -= 2;
   Rectangle( hDC, rect.left, rect.top, rect.right, rect.bottom );

   // Clean up.
   if ( clrTextOld != CLR_INVALID )
      SetTxColor( hDC, clrTextOld );

   SetROP2( hDC, nOldROP );
   SelectPen( hDC, hOldPen );
   SelectBrush( hDC, hOldBrush );
   return( TRUE );
}

void
SuperBtnFreePict( LPBTNPICT Pict )
{
   TRACE_DEBUG( "<SUPERBTN.CPP>", "SuperBtnFreePict" );
   if ( Pict->hPict && !Pict->HandleProvided )
   {
      if ( Pict->chPictType == SUPERBTN_PICT_BITMAP )
         DeleteObject( Pict->hPict );
      else
      if ( Pict->chPictType == SUPERBTN_PICT_ICON )
         DestroyIcon( (HICON) Pict->hPict );
   }

   if ( Pict->hPictStretch )
      DeleteObject( Pict->hPictStretch );
}

void
SuperBtnGetCurrColor( LPSUPERBTNCOLOR lpColor,
                      LPSUPERBTN lpSuperBtn )
{
   TRACE_DEBUG( "<SUPERBTN.CPP>", "SuperBtnGetCurrColor" );
   zmemcpy( lpColor, &lpSuperBtn->Color, zsizeof( SUPERBTNCOLOR ) );

   if ( lpColor->Color == RGBCOLOR_DEFAULT )
      lpColor->Color = GetSysColor( COLOR_BTNFACE );

   if ( lpColor->ColorBorder == RGBCOLOR_DEFAULT )
      lpColor->ColorBorder = GetSysColor( COLOR_BTNTEXT );

   if ( lpColor->ColorShadow == RGBCOLOR_DEFAULT )
      lpColor->ColorShadow = GetSysColor( COLOR_BTNSHADOW );

   if ( lpColor->ColorText == RGBCOLOR_DEFAULT )
      lpColor->ColorText = GetSysColor( COLOR_BTNTEXT );

#if ( WINVER >= 0x030a )
   if ( lpColor->ColorHilite == RGBCOLOR_DEFAULT )
      lpColor->ColorHilite = GetSysColor( COLOR_BTNHIGHLIGHT );
#endif
}

void
SuperBtnPaint( HWND          hWnd,
               LPSUPERBTN    lpSuperBtn,
               HDC           hDC,
               LPRECT        lpRect,
               zBOOL         fIsEnabled,
               zBOOL         fFocus,
               zBOOL         fRoundCorners,
               zBOOL         fUseDib,
               LPRECT        lpRectOrig )
{
   TRACE_DEBUG( "<SUPERBTN.CPP>", "SuperBtnPaint" );
   SUPERBTNCOLOR Color;
   zPCHAR        hTextTemp;
   TEXTMETRIC    fm;
   COLORREF      colorOldText;
   COLORREF      colorOldBk;
   BITMAP        bm;
   HBRUSH        hBrush;
   HBRUSH        hBrushOld;
   HBRUSH        hBrushBackground;
   HPEN          hPenBorder;
   HPEN          hPenOld;
   HFONT         hFontOld = 0;
   HANDLE        hDIB;
   HPALETTE      hPalette;
   HBITMAP       hPict;
   LPBTNPICT     lpPict;
   RECT          rect;
   RECT          rectTemp;
   RECT          rectPict;
   RECT          rectText;
   zPCHAR        Text;
   zPCHAR        TextTemp;
   zLONG         lStyle;
   zBOOL         ButtonIn;
   zBOOL         fDeletePict = FALSE;
   zLONG         dxScaleX;
   zLONG         dxScaleY;
   zSHORT        nBorderSize;
   zLONG         lTextWidth;
   zLONG         lTextHeight;
   zLONG         lPictWidth;
   zLONG         lPictHeight;
   zLONG         lPictWidthNew;
   zLONG         lPictHeightNew;
   zLONG         lPictWidthOrig;
   zLONG         lPictHeightOrig;
   zLONG         lLth;
   zLONG         lAt;
   zLONG         x;
   zLONG         y;
   zLONG         lOrigX;
   zLONG         lOrigY;
   zULONG        k;
   int           nMapModeOld;

   CopyRect( &rect, lpRect );

   lStyle = lpSuperBtn->lStyle;

   // Fill the background with the current background of the display.
   SuperBtnGetCurrColor( &Color, lpSuperBtn );

   hBrushBackground = kzCreateSolidBrush( Color.Color );
   FillRect( hDC, &rect, hBrushBackground );
   DeleteBrush( hBrushBackground );

   colorOldText = SetTxColor( hDC, Color.ColorText );
   colorOldBk = SetBkColor( hDC, Color.Color );

   if ( lpSuperBtn->hFont )
      hFontOld = SelectFont( hDC, lpSuperBtn->hFont );

   // Paint Outside border.
   if ( lStyle & SBS_DEFPUSHBUTTON )
      nBorderSize = 2;
   else
      nBorderSize = 1;

   if ( nBorderSize )
   {
      hBrushOld = SelectBrush( hDC, (HBRUSH) GetStockObject( NULL_BRUSH ) );
      hPenBorder = kzCreatePen( PS_SOLID, 1, Color.ColorBorder );
      hPenOld = SelectPen( hDC, hPenBorder );

      for ( k = 0; k < (zULONG) nBorderSize; k++ )
      {
         if ( fRoundCorners )
            RoundRect( hDC, rect.left, rect.top,
                       rect.right, rect.bottom, 2, 2 );
         else
            Rectangle( hDC, rect.left, rect.top, rect.right, rect.bottom );

         kzShrinkRect( &rect, 1, 1 );
      }

      SelectBrush( hDC, hBrushOld );
      SelectPen( hDC, hPenOld );
      DeletePen( hPenBorder );
   }

   // Draw Shadow.
   kzDrawShadows( hDC, &rect, lpSuperBtn->nShadowSize,
                  Color.ColorShadow, Color.ColorHilite,
                  (zBOOL)(lpSuperBtn->bButtonDown ? 1 : 0), TRUE );

   kzShrinkRect( &rect, lpSuperBtn->nShadowSize, lpSuperBtn->nShadowSize );

   // Draw Frame.
   if ( (lStyle & SBS_BTNFRAMEIN) || (lStyle & SBS_BTNFRAMEOUT) )
   {
      kzShrinkRect( &rect, lpSuperBtn->nBtnFrameOffset,
                    lpSuperBtn->nBtnFrameOffset );

      // Draw Shadow for Frame.
      ButtonIn = (zBOOL) ((lStyle & SBS_BTNFRAMEOUT) ? 0 : 1);
      if ( !(lStyle & SBS_NOINVERTFRAME) && lpSuperBtn->bButtonDown )
         ButtonIn = !ButtonIn;

      kzDrawShadows( hDC, &rect, lpSuperBtn->nShadowSize,
                    Color.ColorShadow,
                    Color.ColorHilite, ButtonIn, FALSE );

      kzShrinkRect( &rect, lpSuperBtn->nShadowSize,
                    lpSuperBtn->nShadowSize );
   }

   // Determine Picture and Text size.
   if ( lpSuperBtn->hText )
   {
//    Text = (zPCHAR) DrLockTaskMemory( lpSuperBtn->hText );
      Text = (zPCHAR) lpSuperBtn->hText;
      lLth = zstrlen( Text );
   }
   else
      lLth = 0;

   zmemcpy( &rectPict, &rect, zsizeof( RECT ) );
   zmemcpy( &rectText, &rect, zsizeof( RECT ) );

   lpPict = &lpSuperBtn->Pict;

   if ( fIsEnabled == 0 && lpSuperBtn->PictDisabled.chPictType )
      lpPict = &lpSuperBtn->PictDisabled;
   else
   if ( lpSuperBtn->bButtonDown && lpSuperBtn->PictBtnDown.chPictType )
      lpPict = &lpSuperBtn->PictBtnDown;

   GetTextMetrics( hDC, &fm );

   if ( fUseDib )
   {
      dxScaleX = (lpRect->right - lpRect->left) /
                                 (lpRectOrig->right - lpRectOrig->left);

      dxScaleY = (lpRect->bottom - lpRect->top) /
                                 (lpRectOrig->bottom - lpRectOrig->top);

      lPictWidthNew = (lpPict->lPictWidth * dxScaleX);
      lPictHeightNew = (lpPict->lPictHeight * dxScaleY);
   }
   else
   {
      lPictWidthNew = lpPict->lPictWidth;
      lPictHeightNew = lpPict->lPictHeight;
   }

   if ( lLth && lpPict->chPictType )
   {
      if ( (lStyle & SBS_TEXTLEFT_PICTRIGHT) ||
           (lStyle & SBS_TEXTRIGHT_PICTLEFT) )
      {
         SIZE size;

         GetTextExtentPoint( hDC, Text, zstrlen( Text ), &size );
         lTextWidth = size.cx;
         if ( (lStyle & SBS_TEXTFRAMEIN) || (lStyle & SBS_TEXTFRAMEOUT) )
            lTextWidth += 8;

         lPictWidth = lPictWidthNew;
         if ( (lStyle & SBS_PICTFRAMEIN) || (lStyle & SBS_PICTFRAMEOUT) )
            lPictWidth += 8;

         x = (rect.right - rect.left - lTextWidth - lPictWidth) / 3;

         if ( lStyle & SBS_TEXTLEFT_PICTRIGHT )
         {
            rectText.right = rect.left + x + lTextWidth + x;
            rectPict.left = rectText.right - x;
         }
         else
         {
            rectPict.right = rect.left + x + lPictWidth + x;
            rectText.left = rectPict.right - x;
         }
      }
      else
      {
         lTextHeight = fm.tmHeight;
         if ( (lStyle & SBS_TEXTFRAMEIN) || (lStyle & SBS_TEXTFRAMEOUT) )
            lTextHeight += 8;

         lPictHeight = lpPict->lPictHeight;
         if ( (lStyle & SBS_PICTFRAMEIN) || (lStyle & SBS_PICTFRAMEOUT) )
            lPictHeight += 8;

         y = (rect.bottom - rect.top - lTextHeight - lPictHeight) / 3;

         if ( lStyle & SBS_TEXTTOP_PICTBOT )
         {
            rectText.bottom = rect.top + y + lTextHeight + y;
            rectPict.top = rectText.bottom - y;
         }
         else
         {
            rectPict.bottom = rect.top + y + lPictHeight + y;
            rectText.top = rectPict.bottom - y;
         }
      }
   }

   // Draw Picture.
   if ( lpPict->chPictType )
   {
      if ( lpSuperBtn->fStretch )
      {
         if ( lpPict->hPictStretch == 0 )
         {
            if ( lpPict->chPictType == SUPERBTN_PICT_ICON )
               lpPict->hPictStretch =
                       kzCreateStretchedIcon( hDC, &rectPict,
                                              Color.Color,
                                              (HICON) lpPict->hPict );
            else
               lpPict->hPictStretch =
                       kzCreateStretchedBitmap( hDC,
                                                (HBITMAP) lpPict->hPict,
                                                &rectPict );
         }

         GetObject( lpPict->hPictStretch, zsizeof( BITMAP ), (zPCHAR) &bm );

         if ( fUseDib )
         {
            bm.bmWidth = (bm.bmWidth * dxScaleX);
            bm.bmHeight = (bm.bmHeight * dxScaleY);
         }

         x = rectPict.left + (rectPict.right - rectPict.left - bm.bmWidth) / 2;
         y = rectPict.top + (rectPict.bottom - rectPict.top - bm.bmHeight) / 2;
      }
      else
      {
         x = rectPict.left +
                          (rectPict.right - rectPict.left - lPictWidthNew) / 2;
         y = rectPict.top +
                          (rectPict.bottom - rectPict.top - lPictHeightNew) / 2;

         // Draw Frame around picture.
         if ( (lStyle & SBS_PICTFRAMEIN) || (lStyle & SBS_PICTFRAMEOUT) )
         {
            rectTemp.left = mMax( x - 4, rectPict.left );
            rectTemp.right = mMin( x + lPictWidthNew + 4, rectPict.right );
            rectTemp.top = mMax( y - 4, rectPict.top );
            rectTemp.bottom = mMin( y + lPictHeightNew + 4, rectPict.bottom );

            ButtonIn = (zBOOL) ((lStyle & SBS_PICTFRAMEOUT) ? 0 : 1);
            if ( !(lStyle & SBS_NOINVERTFRAME) && lpSuperBtn->bButtonDown )
               ButtonIn = !ButtonIn;

            kzDrawShadows( hDC, &rectTemp, lpSuperBtn->nShadowSize,
                           Color.ColorShadow,
                           Color.ColorHilite, ButtonIn, FALSE );
         }
      }

      if ( lpSuperBtn->bButtonDown &&
           lpSuperBtn->PictBtnDown.chPictType == 0 )
      {
         x++;
         y++;
      }

      if ( fUseDib )
      {
         hPict = 0;

         if ( lpPict->hPictStretch )
            hPict = (HBITMAP) lpPict->hPictStretch;
         else
         if ( lpPict->hPict )
         {
            switch ( lpPict->chPictType )
            {
               case SUPERBTN_PICT_ICON:
                  SetRect( &rectTemp, 0, 0, GetSystemMetrics( SM_CXICON ),
                           GetSystemMetrics( SM_CYICON ) );
                  hPict = kzCreateStretchedIcon( hDC, &rectTemp,
                                                Color.Color,
                                                (HICON) lpPict->hPict );
                  fDeletePict = TRUE;
                  break;

               case SUPERBTN_PICT_BITMAP:
                  hPict = (HBITMAP) lpPict->hPict;
                  break;
            }
         }

         hPalette = GetSystemPalette( );
         hDIB = BitmapToDIB( hPict, hPalette );
         DeleteObject( hPalette );

         SetStretchBltMode( hDC, COLORONCOLOR );
         nMapModeOld = SetMapMode( hDC, MM_TEXT );

         x = mMax( rectPict.left, x );
         y = mMax( rectPict.top, y );

         lPictWidthOrig = mMin( lpPict->lPictWidth,
                                ((rectPict.right - x) / dxScaleX) );
         lPictHeightOrig = mMin( lpPict->lPictHeight,
                                 ((rectPict.bottom - y) / dxScaleY) );

         lOrigX = mMax( (lpPict->lPictWidth - ((rectPict.right - x) /
                                                      dxScaleX)) / 2, 0 );
         lOrigY = mMax( (lpPict->lPictHeight - ((rectPict.bottom - y) /
                                                      dxScaleY)) / 2, 0 );

         StretchDibBlt( hDC, x, y, mMin( rectPict.right - x, lPictWidthNew ),
                        mMin( rectPict.bottom - y, lPictHeightNew ), hDIB,
                        lOrigX, lOrigY, lPictWidthOrig, lPictHeightOrig,
                        SRCCOPY );

         if ( fDeletePict )
            DeleteObject( hPict );

         SetMapMode( hDC, nMapModeOld );
      }
      else
      {
         if ( lpPict->hPictStretch )
            kzDrawBitmap( hDC, (HBITMAP) lpPict->hPictStretch,
                          &rectPict, x, y );
         else
         {
            switch ( lpPict->chPictType )
            {
               case SUPERBTN_PICT_ICON:
                  if ( lpPict->hPict )
                     kzDrawIcon( hDC, &rectPict, Color.Color, x, y,
                                 (HICON) lpPict->hPict );

                  break;

               case SUPERBTN_PICT_BITMAP:
                  if ( lpPict->hPict )
                     kzDrawBitmap( hDC, (HBITMAP) lpPict->hPict,
                                   &rectPict, x, y );

                  break;
            }
         }
      }
   }

   // Draw Text.
   if ( lLth )
   {
      zmemcpy( &rectTemp, &rectText, zsizeof( RECT ) );
      lTextHeight = DrawText( hDC, Text, -1, &rectTemp, DT_CALCRECT | DT_TOP |
                              DT_LEFT | DT_SINGLELINE );
      lTextWidth = rectTemp.right - rectTemp.left;
      lTextHeight = fm.tmHeight;

      x = rectText.left +
                  mMax( (rectText.right - rectText.left - lTextWidth) / 2, 0 );
      y = rectText.top +
                  mMax( (rectText.bottom - rectText.top - lTextHeight) / 2, 0 );

      // Draw Frame around text.
      if ( (lStyle & SBS_TEXTFRAMEIN) || (lStyle & SBS_TEXTFRAMEOUT) )
      {
         rectTemp.left = mMax( x - 4, rectText.left );
         rectTemp.right = mMin( x + lTextWidth + 4, rectText.right );
         rectTemp.top = mMax( y - 4, rectText.top );
         rectTemp.bottom = mMin( y + lTextHeight + 4, rectText.bottom );

         ButtonIn = (zBOOL) ((lStyle & SBS_TEXTFRAMEOUT) ? 0 : 1);
         if ( !(lStyle & SBS_NOINVERTFRAME) && lpSuperBtn->bButtonDown )
            ButtonIn = !ButtonIn;

         kzDrawShadows( hDC, &rectTemp, lpSuperBtn->nShadowSize,
                        Color.ColorShadow,
                        Color.ColorHilite, ButtonIn, FALSE );
      }

      rectText.left = x;
      rectText.top = y;
      rectText.right = mMin( x + lTextWidth, rectText.right );
      rectText.bottom = mMin( y + lTextHeight, rectText.bottom );

      SetTxColor( hDC, Color.ColorText );
      SetBkColor( hDC, Color.Color );

      if ( lpSuperBtn->bButtonDown )
         OffsetRect( &rectText, 1, 1 );

      if ( fIsEnabled )
         DrawText( hDC, Text, -1, &rectText, DT_TOP | DT_LEFT | DT_SINGLELINE );
      else
      {
         if ( DrAllocTaskMemory( (zCOREMEM) &hTextTemp,
                                 zstrlen( Text ) + 1, 1082 ) == 0 )
         {
            TextTemp = (zPCHAR) DrLockTaskMemory( hTextTemp );
            for ( k = 0, lAt = 0; k < zstrlen( Text ); k++ )
            {
               if ( Text[ k ] != '&' )
                  TextTemp[ lAt++ ] = Text[ k ];
            }

            hBrush = kzCreateSolidBrush( Color.ColorText );
            fnGrayString( hDC, hBrush, 0, (DWORD) TextTemp, 0, x, y, 0, 0 );
            DeleteBrush( hBrush );

            DrUnlockTaskMemory( TextTemp );
            DrFreeTaskMemory( hTextTemp );
         }
      }
   }

// if ( lpSuperBtn->hText )
//    DrUnlockTaskMemory( Text );

   if ( fFocus )
   {
      if ( lLth )
      {
         rectText.bottom += 1;
         rectText.left -= 1;
         rectText.right += 1;

         SuperBtnFocusRect( hDC, &rectText, lpSuperBtn->hBrushFocus );
      }
      else
      if ( rect.right - rect.left > 12 && rect.bottom - rect.top > 12 )
      {
         if ( lpSuperBtn->bButtonDown )
            OffsetRect( &rect, 1, 1 );

         kzShrinkRect( &rect, 1, 1 );
         SuperBtnFocusRect( hDC, &rect, lpSuperBtn->hBrushFocus );
      }
   }

   if ( colorOldBk != CLR_INVALID )
      SetBkColor( hDC, colorOldBk );

   if ( colorOldText != CLR_INVALID )
      SetTxColor( hDC, colorOldText );

   if ( hFontOld )
      SelectFont( hDC, hFontOld );
}

zBOOL
SuperBtnSetPict( HWND        hWnd,
                 HINSTANCE   hInst,
                 LPBTNPICT   Pict,
                 WORD        wPictType,
                 zLONG       lPict )
{
   TRACE_DEBUG( "<SUPERBTN.CPP>", "SuperBtnSetPict" );
   BITMAP      Bitmap;
   zBOOL       bRC = FALSE;

   if ( hWnd && !hInst )
      hInst = (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE );

   if ( Pict->hPict )
   {
      SuperBtnFreePict( Pict );
      Pict->hPict = 0;
   }

   Pict->chPictType = SUPERBTN_PICT_NONE;

   if ( wPictType & SUPERBTN_PICT_HANDLE )
      Pict->HandleProvided = TRUE;
   else
      Pict->HandleProvided = FALSE;

   if ( wPictType & SUPERBTN_PICT_ICON )
   {
      bRC = TRUE;

      if ( wPictType & SUPERBTN_PICT_HANDLE )
         Pict->hPict = (HANDLE) lPict;
      else
      if ( (Pict->hPict = LoadIcon( hInst, (zPCHAR) lPict) ) == 0 )
         bRC = FALSE;

      if ( Pict->hPict )
      {
         Pict->chPictType = SUPERBTN_PICT_ICON;
         Pict->lPictWidth = GetSystemMetrics( SM_CXICON );
         Pict->lPictHeight = GetSystemMetrics( SM_CYICON );
      }
   }
   else
   if ( wPictType & SUPERBTN_PICT_BITMAP )
   {
      bRC = TRUE;

      if ( wPictType & SUPERBTN_PICT_HANDLE )
         Pict->hPict = (HANDLE) lPict;
      else
      if ( (Pict->hPict = LoadBitmap( hInst, (zPCHAR) lPict )) == 0 )
         bRC = FALSE;

      if ( Pict->hPict )
      {
         Pict->chPictType = SUPERBTN_PICT_BITMAP;

         GetObject( Pict->hPict, zsizeof( BITMAP ), (zPCHAR) &Bitmap );
         Pict->lPictWidth = Bitmap.bmWidth;
         Pict->lPictHeight = Bitmap.bmHeight;
      }
   }

   if ( hWnd )
   {
      InvalidateRect( hWnd, 0, TRUE );
      UpdateWindow( hWnd );
   }

   return( bRC );
}

LPSUPERBTN
SuperBtnGetPtr( HWND    hWnd,
                zPCHAR  *hGlobal )
{
   TRACE_DEBUG( "<SUPERBTN.CPP>", "SuperBtnGetPtr" );
   *hGlobal = (zPCHAR) GetWindowLong( hWnd, 8 );
   return( (LPSUPERBTN) *hGlobal );
// return( (LPSUPERBTN) DrLockTaskMemory( *hGlobal ) );
}

// userfloa.cpp

#define  LockFloatHeader( lpInfo,hInfo, hWnd )  \
   HANDLE          (hInfo);\
   LPEDITFLOATINFO   (lpInfo);\
   if (((hInfo) = (HANDLE) GetWindowLong((hWnd), 4)) != 0)\
      (lpInfo) = (LPEDITFLOATINFO)GlobalLock((hInfo))\

#define  UnlockFloatHeader(hInfo)     GlobalUnlock((hInfo))


zBOOL OPERATION
FloatGetFormat( HWND hWnd, LPFLOATFORMAT ff )
{
   TRACE_DEBUG( "<USERFLOA.CPP>", "FloatGetFormat " );
   zCHAR szText[ 5 ];

   if ( hWnd == 0 )
   {
      GetProfileString( WININTL, WIN_THOUSAND, WIN_DEFTHOUSAND, szText, 2 );
      ff->chSeparator = szText[ 0 ];
      GetProfileString( WININTL, WIN_DECIMAL, WIN_DEFDECIMAL, szText, 2 );
      ff->chDecimalSign = szText[ 0 ];
      GetProfileString( WININTL, WIN_CURRENCY, WIN_DEFCURRENCY, szText, 2 );
      ff->chCurrencySign = szText[ 0 ];
   }
   else
      SendMessage( hWnd, EM_GETFORMAT, 0, (zLONG) ff );

   return( TRUE );
}

zBOOL  OPERATION
FloatGetValue( HWND hWnd, zDECIMAL *pdxDouble )
{
   TRACE_DEBUG( "<USERFLOA.CPP>", "FloatGetValue" );
   zCHAR  szVal[ 40 ];
   zPCHAR szPtr = szVal;

   LockFloatHeader( lpInfo, hInfo, hWnd );
   if ( lpInfo )
   {
      szPtr[ 0 ] = 0;
      SendMessage( hWnd, WM_GETTEXT, 39, (zLONG) (zPCHAR) szVal );
      if ( InSet( lpInfo->lStyle, FS_MONEY ) )
         StrDeleteCh( szVal, lpInfo->chCurrencySign );

      StrDeleteCh( szVal, lpInfo->chSeparator );
      StrReplaceCh( szVal, MAKEWORD( '.', lpInfo->chDecimalSign ) );
      UnlockFloatHeader( hInfo );
      *pdxDouble = ZDecimal( szVal );
   // StringToFloat( szVal, pdxDouble );
      return( TRUE );
   }

   return( FALSE );
}

zBOOL OPERATION
FloatSetFormat( HWND hWnd, LPFLOATFORMAT ff )
{
   TRACE_DEBUG( "<USERFLOA.CPP>", "FloatSetFormat " );
   SendMessage( hWnd, EM_SETFORMAT, 0, (zLONG)ff );
   return( TRUE );
}

zBOOL OPERATION
FloatSetMask( HWND hWnd, zPCHAR szMask )
{
   TRACE_DEBUG( "<USERFLOA.CPP>", "FloatSetMask" );
   zDECIMAL dxVal;
   zBOOL    bRC;

   LockFloatHeader( lpInfo, hInfo, hWnd );
   FloatGetValue( hWnd, &dxVal );
   bRC = GetFloatPicture( lpInfo, szMask );
   FloatSetValue( hWnd, dxVal );

   UnlockFloatHeader( hInfo );
   return( bRC );
}

zBOOL OPERATION
FloatSetRange( HWND hWnd, zDECIMAL dxMin, zDECIMAL dxMax )
{
   TRACE_DEBUG( "<USERFLOA.CPP>", "FloatSetRange" );
   zCHAR szString[ 800 + 1 ];

   if ( dxMin > dxMax )
      return( FALSE );

   StrPrintf( szString, "%f %f", dxMin, dxMax );

   return( (zBOOL) SendMessage( hWnd, EM_SETRANGE, 0,
                                (zLONG) (zPCHAR) szString ) );
}

zBOOL OPERATION
FloatSetValue( HWND hWnd, zDECIMAL dxDouble )
{
   TRACE_DEBUG( "<USERFLOA.CPP>", "FloatSetValue" );
   zCHAR  szVal[ 40 ];
   zPCHAR szPtr = szVal;
   BYTE   bFractionalWidth;

   LockFloatHeader( lpInfo, hInfo, hWnd );

   if ( lpInfo )
   {
      if ( lpInfo->bFractionalWidth == 0 )
         bFractionalWidth = 1;
      else
         bFractionalWidth = lpInfo->bFractionalWidth;

      StrPrintf( szVal, "%.*f", (zSHORT) bFractionalWidth, dxDouble );
      if ( lpInfo->chDecimalSign != '.' )
         StrReplaceCh( szVal, MAKEWORD( lpInfo->chDecimalSign, '.' ) );

      if ( lpInfo->bFractionalWidth == 0 )
         szVal[ zstrlen( szVal ) - 2 ] = 0;

      UnlockFloatHeader( hInfo );
      SendMessage( hWnd, WM_SETTEXT, 0, (LPARAM)(zPCHAR) szVal );
      return( TRUE );
   }

   return( FALSE );
}

zBOOL
GetFloatPicture( LPEDITFLOATINFO lpInfo, zPCHAR szRange )
{
   TRACE_DEBUG( "<FLOATMGR.CPP>", "GetFloatPicture" );
   zPCHAR lpDecimal;
   int   nLth;
   zBOOL bRC = FALSE;

   if ( szRange )
   {
      lpDecimal = zstrchr( szRange, lpInfo->chDecimalSign );
      nLth = zstrlen( szRange );

      if ( lpDecimal == 0 )
         lpDecimal = zstrchr( szRange, '.' );     // look for the default

      if ( (nLth > 0) && (nLth < 70) ) // The total width must be greater than 0
      {
         if ( lpDecimal )
         {
            lpInfo->bIntegerWidth = (BYTE) (lpDecimal ?
                                     ((DWORD) lpDecimal - (DWORD) szRange ) :
                                     nLth + 1 );
            lpInfo->bTotalWidth = (BYTE) nLth;
            lpInfo->bFractionalWidth =
                   (BYTE) (lpInfo->bTotalWidth - lpInfo->bIntegerWidth - 1 );
            bRC = TRUE;
         }
         else
         {
            lpInfo->bFractionalWidth = 0;
            lpInfo->bTotalWidth = (BYTE) nLth;
            lpInfo->bMinWidth = (BYTE) nLth;
            lpInfo->bMaxWidth = (BYTE) nLth;
            lpInfo->bIntegerWidth = (BYTE) nLth;
         }
      }
   }

   return( bRC );
}

// wintools.cpp

WORD
DibNumColors( VOID  *pv )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "DibNumColors" );
   int                bits;
   LPBITMAPINFOHEADER lpbi;
   LPBITMAPCOREHEADER lpbc;

   lpbi = ((LPBITMAPINFOHEADER) pv);
   lpbc = ((LPBITMAPCOREHEADER) pv);

   // With the BITMAPINFO format headers, the size of the palette
   // is in biClrUsed, whereas in the BITMAPCORE - style headers, it
   // is dependent on the bits per pixel ( = 2 raised to the power of
   // bits/pixel).
   if ( lpbi->biSize != zsizeof( BITMAPCOREHEADER ) )
   {
      if ( lpbi->biClrUsed != 0 )
         return( (WORD) lpbi->biClrUsed );

      bits = lpbi->biBitCount;
   }

   else
      bits = lpbc->bcBitCount;

   switch ( bits )
   {
      case 1:
         return( 2 );
      case 4:
         return( 16 );
      case 8:
         return( 256 );
      default:
         return( 0 );   // a 24 bitcount DIB has no color table
   }
}

zBOOL
StretchDibBlt( HDC     hDC,
               int     x,
               int     y,
               int     dx,
               int     dy,
               HANDLE  hDib,
               int     x0,
               int     y0,
               int     dx0,
               int     dy0,
               zLONG   Rop )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "StretchDibBlt" );
   LPBITMAPINFOHEADER lpbi;
   zPCHAR             pBuf;
   zBOOL              bRC;

   lpbi = (LPBITMAPINFOHEADER) GlobalLock( hDib );

   pBuf = (zPCHAR) lpbi + *(DWORD *) lpbi + ::PaletteSize( lpbi );

   bRC = StretchDIBits( hDC, x, y, dx, dy, x0, y0, dx0, dy0,
                        pBuf, (LPBITMAPINFO) lpbi, DIB_RGB_COLORS, Rop );

   GlobalUnlock( hDib );
   GlobalFree( hDib );

   return( bRC );
}

HBITMAP
kzCreateStretchedIcon( HDC        hDC,
                       LPRECT     lpRect,
                       COLORREF   Color,
                       HICON      hIcon )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "kzCreateStretchedIcon" );
   HBITMAP    hBitmap;
   HBITMAP    hBitmapNew;
   HBITMAP    hBitmapOld;
   HDC        hDCMemory;
   HBRUSH     hBrush;
   RECT       rect;
   zSHORT     nWidth;
   zSHORT     nHeight;

   nWidth = GetSystemMetrics( SM_CXICON );
   nHeight = GetSystemMetrics( SM_CYICON );

   // Create Bitmap to select to the memory device context.
   hBitmap = kzCreateCompatibleBitmap( hDC, nWidth, nHeight );

   // Create a memory device context.
   hDCMemory = CreateCompatibleDC( hDC );
   hBitmapOld = SelectBitmap( hDCMemory, hBitmap );
   int nMapModeOld = SetMapMode( hDCMemory, MM_TEXT );

   // Fill the hDCMemory background with the current background of the
   // display.
   hBrush = kzCreateSolidBrush( Color );
   SetRect( &rect, 0, 0, nWidth, nHeight );
   FillRect( hDCMemory, &rect, hBrush );
   DeleteBrush( hBrush );

   // Draw the icon on the memory device context.
   DrawIcon( hDCMemory, 0, 0, hIcon );

   SetMapMode( hDCMemory, nMapModeOld );
   SelectBitmap( hDCMemory, hBitmapOld );
   DeleteDC( hDCMemory );

   if ( nWidth == lpRect->right - lpRect->left &&
        nHeight == lpRect->bottom - lpRect->top )
   {
      hBitmapNew = hBitmap;
   }
   else
   {
      // Stretch the bitmap.
      hBitmapNew = kzCreateStretchedBitmap( hDC, hBitmap, lpRect );

      // Clean up.
      DeleteBitmap( hBitmap );
   }

   return( hBitmapNew );
}

HBITMAP
kzCreateStretchedBitmap( HDC     hDC,
                         HBITMAP hBitmap,
                         LPRECT  lpRect )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "kzCreateStretchedBitmap" );
   BITMAP  bm;
   HBITMAP hBitmapSrcOld;
   HBITMAP hBitmapDestOld;
   HBITMAP hBitmapDest;
   HDC     hDCMemSrc;
   HDC     hDCMemDest;
   zLONG   lWidth;
   zLONG   lHeight;

   GetObject( hBitmap, zsizeof( BITMAP ), (zPCHAR) &bm );

   lHeight = (zLONG) (((lpRect->right - lpRect->left) * bm.bmHeight) / bm.bmWidth);

   if ( lHeight <= lpRect->bottom - lpRect->top )
      lWidth = lpRect->right - lpRect->left;
   else
   {
      lWidth = ((lpRect->bottom - lpRect->top) * bm.bmWidth) / bm.bmHeight;
      lHeight = lpRect->bottom - lpRect->top;
   }

   if ( lWidth == bm.bmWidth * 2 && lHeight == bm.bmHeight * 2 )
   {
      lWidth--;
      lHeight--;
   }

   hDCMemDest = CreateCompatibleDC( hDC );
   hBitmapDest = kzCreateCompatibleBitmap( hDC, lWidth, lHeight );
   hBitmapDestOld = SelectBitmap( hDCMemDest, hBitmapDest );
   int nMapModeOld = SetMapMode( hDCMemDest, GetMapMode( hDC ) );

   hDCMemSrc = CreateCompatibleDC( hDC );
   hBitmapSrcOld = SelectBitmap( hDCMemSrc, hBitmap );
   SetMapMode( hDCMemSrc, GetMapMode( hDC ) );
   SetStretchBltMode( hDCMemDest, COLORONCOLOR );

   StretchBlt( hDCMemDest, 0, 0, lWidth, lHeight, hDCMemSrc, 0, 0, bm.bmWidth,
               bm.bmHeight, SRCCOPY );

   SetMapMode( hDCMemDest, nMapModeOld );
   SelectBitmap( hDCMemDest, hBitmapDestOld );
   SelectBitmap( hDCMemSrc, hBitmapSrcOld );
   DeleteDC( hDCMemDest );
   DeleteDC( hDCMemSrc );

   return( hBitmapDest );
}

void
kzDrawBitmap( HDC     hDC,
              HBITMAP hBitmap,
              LPRECT  lpRect,
              zLONG   x,
              zLONG   y )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "kzDrawBitmap" );
   BITMAP  bm;
   HBITMAP hBitmapOld;
   HDC     hDCMem;
   POINT   ptSize;
   POINT   ptOrg;

   hDCMem = CreateCompatibleDC( hDC );
   hBitmapOld = SelectBitmap( hDCMem, hBitmap );
   int nMapModeOld = SetMapMode( hDCMem, GetMapMode( hDC ) );

   GetObject( hBitmap, zsizeof( BITMAP ), (zPCHAR)&bm );

   ptSize.x = mMin( bm.bmWidth, lpRect->right - lpRect->left );
   ptSize.y = mMin( bm.bmHeight, lpRect->bottom - lpRect->top );

   if ( x < 0 )
      ptOrg.x = -x;
   else
      ptOrg.x = 0;

   if ( y < 0 )
      ptOrg.y = -y;
   else
      ptOrg.y = 0;

   BitBlt( hDC, mMax( x, lpRect->left ), mMax( y, lpRect->top ),
           ptSize.x, ptSize.y, hDCMem, ptOrg.x, ptOrg.y, SRCCOPY );

   SetMapMode( hDCMem, nMapModeOld );
   SelectBitmap( hDCMem, hBitmapOld );
   DeleteDC( hDCMem );
}

void
kzDrawIcon( HDC        hDC,
            LPRECT     lpRect,
            COLORREF   Color,
            zLONG      x,
            zLONG      y,
            HICON      hIcon )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "kzDrawIcon" );
   HBITMAP    hBitmap;
   HBITMAP    hBitmapOld;
   HDC        hDCMemory;
   HBRUSH     hBrush;
   RECT       rect;
   zLONG      lWidth;
   zLONG      lHeight;
   zLONG      lOrgX;
   zLONG      lOrgY;

   lWidth = GetSystemMetrics( SM_CXICON );
   lHeight = GetSystemMetrics( SM_CYICON );

   // Create Bitmap to select to the memory device context.
   hBitmap = kzCreateCompatibleBitmap( hDC, lWidth, lHeight );

   // Create a memory device context.
   hDCMemory = CreateCompatibleDC( hDC );
   hBitmapOld = SelectBitmap( hDCMemory, hBitmap );
   int nMapModeOld = SetMapMode( hDCMemory, MM_TEXT );

   // Fill the hDCMemory background with the current background of the
   // display.
   hBrush = kzCreateSolidBrush( Color );
   SetRect( &rect, 0, 0, lWidth, lHeight );
   FillRect( hDCMemory, &rect, hBrush );
   DeleteBrush( hBrush );

   // Draw the icon on the memory device context.
   DrawIcon( hDCMemory, 0, 0, hIcon );

   if ( x == LONG_MIN )
      x = lpRect->left + (((lpRect->right - lpRect->left) - lWidth) / 2);

   if ( y == LONG_MIN )
      y = lpRect->top + (((lpRect->bottom - lpRect->top) - lHeight) / 2);

   lWidth = mMin( lWidth, lpRect->right - lpRect->left );
   lHeight = mMin( lHeight, lpRect->bottom - lpRect->top );

   if ( x < lpRect->left )
      lOrgX = lpRect->left - x;
   else
      lOrgX = 0;

   if ( y < lpRect->top )
      lOrgY = lpRect->top - y;
   else
      lOrgY = 0;

   // Copy the memory device context bitmap to the display.
   BitBlt( hDC, mMax( x, lpRect->left ), mMax( y, lpRect->top ),
           lWidth, lHeight, hDCMemory, lOrgX, lOrgY, SRCCOPY );

   // Clean up.
   SetMapMode( hDCMemory, nMapModeOld );
   SelectBitmap( hDCMemory, hBitmapOld );
   DeleteDC( hDCMemory );
   DeleteBitmap( hBitmap );
}

void
kzDrawShadows( HDC       hDC,
               LPRECT    lpRect,
               zSHORT    nShadowSize,
               COLORREF  ColorShadow,
               COLORREF  ColorHilite,
               zBOOL     fShadowIn,
               zBOOL     fShadowButton )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "kzDrawShadows" );
   HBRUSH   hBrush;
   HBRUSH   hBrushOld;
   zUSHORT  k;

   // Create left and top.
   hBrush = kzCreateSolidBrush( fShadowIn ? ColorShadow : ColorHilite );
   hBrushOld = SelectBrush( hDC, hBrush );

   // Left.
   for ( k = 0; k < nShadowSize; k++ )
      PatBlt( hDC, lpRect->left + k, lpRect->top, 1,
              lpRect->bottom - lpRect->top - k, PATCOPY );

   // Top.
   for ( k = 0; k < nShadowSize; k++ )
      PatBlt( hDC, lpRect->left, lpRect->top + k,
              lpRect->right - lpRect->left - k, 1, PATCOPY );

   SelectBrush( hDC, hBrushOld );
   DeleteBrush( hBrush );

   // Create right and bottom.
   if ( !fShadowButton || !fShadowIn )
   {
      hBrush = kzCreateSolidBrush( fShadowIn ? ColorHilite : ColorShadow );
      hBrushOld = SelectBrush( hDC, hBrush );

      // Right.
      for ( k = 0; k < nShadowSize; k++ )
         PatBlt( hDC, lpRect->right - k - 1, lpRect->top + k, 1,
                 lpRect->bottom - lpRect->top - k, PATCOPY );

      // Bottom.
      for ( k = 0; k < nShadowSize; k++ )
         PatBlt( hDC, lpRect->left + k, lpRect->bottom - k - 1,
                 lpRect->right - lpRect->left - k, 1, PATCOPY );

      SelectBrush( hDC, hBrushOld );
      DeleteBrush( hBrush );
   }
}

void
kzShrinkRect( LPRECT  lpRect,
              zLONG   x,
              zLONG   y )
{
   TRACE_DEBUG( "<WINTOOLS.CPP>", "kzShrinkRect" );
   lpRect->left += x;
   lpRect->top += y;
   lpRect->right -= x;
   lpRect->bottom -= y;
}

#endif
