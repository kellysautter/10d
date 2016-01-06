/////////////////////////////////////////////////////////////////////////////
// Project ZdSS
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlssd.cpp
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
// 1999.11.02    DKS    Z10    QS999
//    Fixed paint problem in non-client area.
//
// 1999.09.22    DKS    Z10    QS999
//    Fixed text color problem and started memory leak analyis.
//
// 1999.09.03    DKS   Z10
//    Force text and background colors to be system colors.
//
// 1999.01.18    DKS
//    Changed background color for ComboBox (Helmut's request).
//
// 1998.10.30    DKS
//    Changed background color.  Fixed edit leave/enter cell processing.
//
// 1998.09.16    DKS
//    Fix to limit number of columns to 64.
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

SS_COLORID
SS_AddColor( LPSPREADSHEET lpSS,
             COLORREF      Color )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_AddColor" );
   LPSS_CLRTBLITEM lpColorTable;
   SS_COLORID        ColorId = 0;
   SS_COLORID        k;

   if ( Color == RGBCOLOR_DEFAULT )
      return( 0 );

   if ( g_nColorTableCnt >= g_nColorTableAllocCnt )
      g_hColorTable = SS_ColorAllocItem( g_hColorTable,
                                         &g_nColorTableAllocCnt,
                                         sizeof( SS_CLRTBLITEM ) );

   if ( g_hColorTable == 0 )
      return( -1 );

// lpColorTable = (LPSS_CLRTBLITEM) DrLockTaskMemory( g_hColorTable );
   lpColorTable = (LPSS_CLRTBLITEM) g_hColorTable;

   for ( k = 0; k < g_nColorTableCnt; k++ )
   {
      if ( lpColorTable[ k ].Color == Color )
      {
//       DrUnlockTaskMemory( lpColorTable );
         return( k + 1 );
      }
   }

   // add color to color table
   lpColorTable[ g_nColorTableCnt ].Color = Color;
   lpColorTable[ g_nColorTableCnt ].hBrush = kzCreateSolidBrush( Color );

   g_nColorTableCnt++;

   ColorId = g_nColorTableCnt;

// DrUnlockTaskMemory( lpColorTable );
   return( ColorId );
}

void
SS_BtnDraw( HWND          hWnd,
            HDC           hDC,
            LPRECT        lpRect,
            LPSS_CELLTYPE lpCellType,
            HFONT         hFont,
            zBOOL         fButtonDown,
            zBOOL         fRoundCorners,
            zBOOL         fUseDib,
            LPRECT        lpRectOrig )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_BtnDraw" );
   SUPERBTN      SuperBtn;
   zPCHAR        lpText;
   zBOOL         fSetBtnUp = FALSE;
   zBOOL         fSetBtnDown = FALSE;

   zmemset( &SuperBtn, 0, sizeof( SUPERBTN ) );

   SuperBtn.hText           = lpCellType->Spec.Button.hText;
   SuperBtn.hBrushFocus     = 0;
   SuperBtn.hBitmapDC       = 0;
   SuperBtn.hFont           = hFont;
   SuperBtn.lStyle          = lpCellType->lStyle;
   SuperBtn.nShadowSize     = lpCellType->Spec.Button.nShadowSize;
   SuperBtn.nBtnFrameOffset = 0;
   SuperBtn.nButtonType     = 0;
   SuperBtn.bButtonDown     = fButtonDown;
   SuperBtn.bMouseCaptureOn = FALSE;
   SuperBtn.fStretch        = FALSE;

   if ( (!fButtonDown && lpCellType->Spec.Button.hPictName) ||
        (fButtonDown && lpCellType->Spec.Button.hPictDownName == 0 &&
         lpCellType->Spec.Button.hPictName) )
   {
      if ( lpCellType->Spec.Button.nPictureType & SUPERBTN_PICT_HANDLE )
         SuperBtnSetPict( 0, 0, &SuperBtn.Pict,
                          (WORD) lpCellType->Spec.Button.nPictureType,
                          (zLONG) lpCellType->Spec.Button.hPictName );
      else
      {
         fSetBtnUp = TRUE;
//       lpText = (zPCHAR)
//                    DrLockTaskMemory( lpCellType->Spec.Button.hPictName );
         lpText = (zPCHAR) lpCellType->Spec.Button.hPictName;

         SuperBtnSetPict( 0, (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                          &SuperBtn.Pict,
                          (WORD) lpCellType->Spec.Button.nPictureType,
                          (zLONG) lpText );
//       DrUnlockTaskMemory( lpText );
      }
   }
   else
   if ( fButtonDown && lpCellType->Spec.Button.hPictDownName )
   {
      if ( lpCellType->Spec.Button.nPictureDownType & SUPERBTN_PICT_HANDLE )
         SuperBtnSetPict( 0, 0, &SuperBtn.PictBtnDown,
                          (WORD) lpCellType->Spec.Button.nPictureDownType,
                          (zLONG) lpCellType->Spec.Button.hPictDownName );
      else
      {
         fSetBtnDown = TRUE;
//       lpText = (zPCHAR)
//                 DrLockTaskMemory( lpCellType->Spec.Button.hPictDownName );
         lpText = (zPCHAR) lpCellType->Spec.Button.hPictDownName;

         SuperBtnSetPict( 0, (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                          &SuperBtn.PictBtnDown,
                          (WORD) lpCellType->Spec.Button.nPictureDownType,
                          (zLONG) lpText );
//       DrUnlockTaskMemory( lpText );
      }
   }

   zmemcpy( &SuperBtn.Color, &lpCellType->Spec.Button.Color,
            sizeof( SUPERBTNCOLOR ) );
   SuperBtnPaint( hWnd, &SuperBtn, hDC, lpRect, TRUE, FALSE,
                  fRoundCorners, fUseDib, lpRectOrig );
   if ( fSetBtnUp )
      SuperBtnFreePict( &SuperBtn.Pict );

   if ( fSetBtnDown )
      SuperBtnFreePict( &SuperBtn.PictBtnDown );
}

void
SS_CalcPixelsToColWidth( LPSPREADSHEET lpSS,
                         SS_COORD      ssCol,
                         zLONG         lPixelWidth,
                         zPLONG        pdxColWidth )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_CalcPixelsToColWidth" );
   ssCol;

   if ( lPixelWidth <= 0 )
      *pdxColWidth = 0;
   else
   {
      lPixelWidth -= lpSS->nBorderSize;

      switch ( lpSS->nUnitType )
      {
         case SS_UNITTYPE_NORMAL:
            *pdxColWidth = lPixelWidth / lpSS->nStandardFontWidth;
            break;

         case SS_UNITTYPE_DEFAULT:
            *pdxColWidth = lPixelWidth * XPIXELSPERINCH /
                                 (STANDARDFONTWIDTH * lpSS->nPixelsPerInchX);
            break;

         case SS_UNITTYPE_TWIPS:
            *pdxColWidth = (lPixelWidth * 1440) / lpSS->nPixelsPerInchX;
            break;
      }
   }
}

void
SS_CalcPixelsToRowHeight( LPSPREADSHEET lpSS,
                          SS_COORD      ssRow,
                          zLONG         lPixelHeight,
                          zPLONG        pdxRowHeight )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_CalcPixelsToRowHeight" );
   LPSS_FONT     Font;
   SS_FONT       FontTemp;
   LPSS_ROW      lpRow;
   zLONG         FontCellHeight;
   zSHORT        nFontHeight;

   if ( lPixelHeight <= 0 )
      *pdxRowHeight = 0;

   else
   {
      lPixelHeight -= lpSS->nBorderSize;

      switch ( lpSS->nUnitType )
      {
         case SS_UNITTYPE_NORMAL:
         case SS_UNITTYPE_DEFAULT:
            Font = SS_GetFont( lpSS, &FontTemp, lpSS->DefaultFontId );

            nFontHeight = Font->nFontHeight;
            FontCellHeight = Font->FontCellHeight;

            if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
            {
               Font = SS_GetFont( lpSS, &FontTemp, lpRow->RowMaxFontId );

               if ( Font && Font->nFontHeight != SS_HEIGHT_DEFAULT )
               {
                  nFontHeight = Font->nFontHeight;
                  FontCellHeight = Font->FontCellHeight;
               }

               SS_UnlockRowItem( lpSS, ssRow );
            }

            if ( lpSS->nUnitType == SS_UNITTYPE_NORMAL )
               *pdxRowHeight = (lPixelHeight * FontCellHeight) / nFontHeight;
            else
               *pdxRowHeight = (lPixelHeight * FontCellHeight *
                                      YPIXELSPERINCH) /
                                    (nFontHeight * lpSS->nPixelsPerInchY);

            break;

         case SS_UNITTYPE_TWIPS:
            *pdxRowHeight = (lPixelHeight * 1440) / lpSS->nPixelsPerInchY;
            break;
      }
   }
}

zSHORT
SS_CalcRowHeight( LPSPREADSHEET lpSS,
                  SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_CalcRowHeight" );
   LPSS_ROW lpRow;
// double    RowHeight = SS_HEIGHT_DEFAULT;
   zSHORT   nPixels = -1;

   if ( ssRow == -1 )
      nPixels = SS_RowHeightToPixels( lpSS, ssRow,
                                      lpSS->Row.lCellSizeX100 == -1 ? -1 :
                                      lpSS->Row.lCellSizeX100 / 100 );
   else
   if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
   {
      if ( lpRow->lRowHeightX100 == 0 )
         nPixels = 0;
      else
         nPixels = SS_RowHeightToPixels( lpSS, ssRow, lpRow->lRowHeightX100 ==
                                         SS_HEIGHT_DEFAULT ? -1 :
                                         lpRow->lRowHeightX100 / 100 );

      SS_UnlockRowItem( lpSS, ssRow );
   }

   return( nPixels );
}

void
SS_ClipClientRect( HWND hWnd,
                   HDC hDC,
                   LPRECT  lprectClient )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_ClipClientRect" );
   RECT  rectTemp;

   GetClientRect( hWnd, &rectTemp );

   if ( rectTemp.right > lprectClient->right )
      ExcludeClipRect( hDC, lprectClient->right, 0,
                       rectTemp.right, rectTemp.bottom );

   if ( rectTemp.bottom > lprectClient->bottom )
      ExcludeClipRect( hDC, 0, lprectClient->bottom,
                       rectTemp.right, rectTemp.bottom );
}

zSHORT
SS_ColWidthToPixels( LPSPREADSHEET lpSS,
                     zLONG         lColWidth )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_ColWidthToPixels" );
   zLONG    dxTemp;
   zSHORT   nWidth;

   switch ( lpSS->nUnitType )
   {
      case SS_UNITTYPE_NORMAL:
         if ( lColWidth == SS_WIDTH_DEFAULT )
            nWidth = lpSS->nStandardFontWidth * DEFAULT_CELLWIDTH;
         else
         if ( lColWidth < -1 )
            nWidth = (zSHORT)(((zLONG) lpSS->nStandardFontWidth *
                                      DEFAULT_CELLWIDTH) / -lColWidth);
         else
            nWidth = (zSHORT)((zLONG) lpSS->nStandardFontWidth * lColWidth );

         break;

      case SS_UNITTYPE_DEFAULT:
         if ( lColWidth == SS_WIDTH_DEFAULT )
            lColWidth = DEFAULT_CELLWIDTH;
         else
         if ( lColWidth < -1 )
            lColWidth = DEFAULT_CELLWIDTH / -lColWidth;

         dxTemp = (zLONG) STANDARDFONTWIDTH * lColWidth *
                  (zLONG) lpSS->nPixelsPerInchX / (zLONG) XPIXELSPERINCH;
         nWidth = (zSHORT) dxTemp;
         break;

      case SS_UNITTYPE_TWIPS:
         if ( lColWidth == SS_WIDTH_DEFAULT )
            lColWidth = DEFAULT_CELLWIDTH_TWIPS;
         else
         if ( lColWidth < -1 )
            lColWidth = DEFAULT_CELLWIDTH_TWIPS / (-lColWidth);

         nWidth = (zSHORT) ((zLONG) lColWidth *
                           (zLONG) lpSS->nPixelsPerInchX) / 1440;

         break;
   }

   if ( nWidth > 0 )
   {
      // Make special case for 1024x768 - So much for Microsoft's
      // claim that Windows is device independent.
      if ( lpSS->nPixelsPerInchX == 120 &&
           lpSS->nUnitType != SS_UNITTYPE_TWIPS )
      {
         nWidth++;
      }

      nWidth += lpSS->nBorderSize;
   }

   return( nWidth );
}

zPCHAR
SS_ColorAllocItem( zPCHAR        hList,
                   zPSHORT       lpnItemCnt,
                   zSHORT        nItemSize )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_ColorAllocItem" );
   *lpnItemCnt += 10;

   if ( hList )
   {
      hList = (zPCHAR) DrUnlockTaskMemory( hList );
      DrReAllocTaskMemory( (zCOREMEM) &hList, *lpnItemCnt * nItemSize );
   }
   else
   {
      DrAllocTaskMemory( (zCOREMEM) &hList, *lpnItemCnt * nItemSize, 1013 );
   }

   if ( hList == 0 )
      *lpnItemCnt = 0;
   else
      hList = (zPCHAR) DrLockTaskMemory( hList );

   return( hList );
}

zBOOL
SS_CreateBlockRect( HWND          hWnd,
                    LPSPREADSHEET lpSS,
                    LPRECT        rect,
                    SS_COORD      ssLeft,
                    SS_COORD      ssTop,
                    SS_COORD      ssRight,
                    SS_COORD      ssBottom,
                    zBOOL         fCell )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_CreateBlockRect" );
   RECT          rectClient;
   SS_COORD      ssCol;
   SS_COORD      ssRow;
   zBOOL         fRetRow = TRUE;
   zBOOL         fRetCol = TRUE;
   zBOOL         fRetRow2 = TRUE;
   zBOOL         fRetCol2 = TRUE;
   zSHORT        nOffset;
   zLONG         x;
   zLONG         y;
   SS_COORD      ss;

   SS_GetClientRect( hWnd, &rectClient );

   if ( ssRight == -1 && ssBottom == -1 )
      ;
   else
   if ( ssRight == -1 && ssBottom != -1 )
   {
      for ( ss = 0, nOffset = 0; ss < lpSS->Row.nHeaderCnt; ss++ )
         nOffset += SS_GetRowHeight( lpSS, ss );

      rectClient.top += nOffset;
   }
   else
   if ( ssRight != -1 && ssBottom == -1 )
   {
      for ( ss = 0, nOffset = 0; ss < lpSS->Col.nHeaderCnt; ss++ )
         nOffset += SS_GetColWidth( lpSS, ss );

      rectClient.left += nOffset;
   }
   else
   if ( ssRight != -1 && ssBottom != -1 )
   {
      // Determine Left.
      for ( ss = 0, nOffset = 0; ss < lpSS->Col.nHeaderCnt; ss++ )
         nOffset += SS_GetColWidth( lpSS, ss );

      rectClient.left += nOffset;

      // Determine Top.
      for ( ss = 0, nOffset = 0; ss < lpSS->Row.nHeaderCnt; ss++ )
         nOffset += SS_GetRowHeight( lpSS, ss );

      rectClient.top += nOffset;
   }

   if ( ssRight == -1 )
   {
      rect->left = rectClient.left;
      rect->right = mMin( SS_GetCellPosX( lpSS, lpSS->Col.UL,
                          lpSS->Col.LR ) + SS_GetColWidth( lpSS,
                          lpSS->Col.LR ), rectClient.right );
   }
   else
   {
      if ( lpSS->Col.Frozen &&
           ssLeft < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
      {
         ssCol = ssLeft;
         rect->left = SS_GetCellPosX( lpSS, 0, ssLeft );
      }
      else
      {
         ssCol = mMax( ssLeft, lpSS->Col.UL );
         rect->left = SS_GetCellPosX( lpSS, lpSS->Col.UL, ssCol );
      }

      for ( x = rect->left; ssCol <= ssRight && x < rectClient.right; )
      {
         x += SS_GetColWidth( lpSS, ssCol );

         if ( lpSS->Col.Frozen &&
              ssCol == lpSS->Col.nHeaderCnt + lpSS->Col.Frozen - 1 )
         {
            ssCol = mMax( lpSS->Col.UL, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
         }
         else
         {
            ssCol++;
         }
      }

      if ( rect->left == x )
         fRetCol = FALSE;

      if ( x == rectClient.right )
         fRetCol2 = FALSE;

      rect->right = x + 2;
      rect->left = mMax( rect->left - 1, rectClient.left );

      if ( ssCol == SS_GetColsMax( lpSS ) )
         rect->right -= 2;
      else
      if ( ssLeft <= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen - 1 &&
           ssRight < lpSS->Col.UL )
      {
         rect->right--;
      }
      else
      if ( lpSS->Col.Frozen &&
           ssLeft >= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen &&
           ssLeft <= lpSS->Col.UL )
      {
         rect->left++;
      }

      rect->right = mMin( rect->right, rectClient.right );
   }

   if ( ssBottom == -1 )
   {
      rect->top = rectClient.top;
      rect->bottom = mMin( SS_GetCellPosY( lpSS, lpSS->Row.UL,
                                           lpSS->Row.LR ) +
                 SS_GetRowHeight( lpSS, lpSS->Row.LR ), rectClient.bottom );
   }
   else
   {
      if ( lpSS->Row.Frozen &&
           ssTop < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
      {
         ssRow = ssTop;
         rect->top = SS_GetCellPosY( lpSS, 0, ssTop );
      }
      else
      {
         ssRow = mMax( ssTop, lpSS->Row.UL );
         rect->top = SS_GetCellPosY( lpSS, lpSS->Row.UL, ssRow );
      }

      for ( y = rect->top; ssRow <= ssBottom && y < rectClient.bottom; )
      {
         y += SS_GetRowHeight( lpSS, ssRow );

         if ( lpSS->Row.Frozen &&
              ssRow == lpSS->Row.nHeaderCnt + lpSS->Row.Frozen - 1 )
         {
            ssRow = mMax( lpSS->Row.UL, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
         }
         else
            ssRow++;
      }

      if ( rect->top == y )
         fRetRow = FALSE;

      if ( y == rectClient.bottom )
         fRetRow2 = FALSE;

      if ( SS_USESELBAR( lpSS ) )
      {
         rect->bottom = y + 1;
         rect->top = mMax( rect->top, rectClient.top );

         if ( ssRow == SS_GetRowsMax( lpSS ) )
            rect->bottom -= 1;

         rect->bottom = mMin( rect->bottom, rectClient.bottom );
      }
      else
      {
         rect->bottom = y + 2;
         rect->top = mMax( rect->top - 1, rectClient.top );

         if ( ssRow == SS_GetRowsMax( lpSS ) )
            rect->bottom -= 2;
         else
         if ( ssTop <= lpSS->Row.nHeaderCnt + lpSS->Row.Frozen - 1 &&
              ssBottom < lpSS->Row.UL )
         {
            rect->bottom--;
         }
         else
         if ( lpSS->Row.Frozen &&
              ssTop >= lpSS->Row.nHeaderCnt + lpSS->Row.Frozen &&
              ssTop <= lpSS->Row.UL )
         {
            rect->top++;
         }

         rect->bottom = mMin( rect->bottom, rectClient.bottom );
      }
   }

   if ( fCell )
   {
      if ( fRetRow && !fRetCol && lpSS->Col.ssCurrAt == lpSS->Col.UL - 1 )
      {
         rect->right = rect->left + 1;
         fRetCol = TRUE;
      }

      if ( fRetCol && !fRetRow && lpSS->Row.ssCurrAt == lpSS->Row.UL - 1 )
      {
         rect->bottom = rect->top + 1;
         fRetRow = TRUE;
      }

      if ( fRetRow2 && !fRetCol2 && lpSS->Col.ssCurrAt == lpSS->Col.LR + 1 )
      {
         rect->left = rect->right - 1;
         fRetCol = TRUE;
      }

      if ( fRetCol2 && !fRetRow2 && lpSS->Row.ssCurrAt == lpSS->Row.LR + 1 )
      {
         rect->top = rect->bottom - 1;
         fRetRow = TRUE;
      }
   }

   return( fRetRow && fRetCol );
}

HFONT
SS_CreatePrinterFont( LPSPREADSHEET lpSS,
                      LPSS_FONT     lpFont,
                      SS_FONTID     FontId,
                      LPSSX_PRINTFORMAT lpPFormat )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_CreatePrinterFont" );
   LOGFONT LogFont;
   zSHORT  nHeight;

   zmemcpy( &LogFont, &lpFont->LogFont, sizeof( LOGFONT ) );
   nHeight = SS_SCREEN2PRINTERY( lpPFormat, LogFont.lfHeight );

   if ( lpFont->nFontHeightPrinter != nHeight || lpFont->hFontPrinter == 0 )
   {
      LogFont.lfHeight = nHeight;
      LogFont.lfWidth = 0;

      if ( lpFont->hFontPrinter )
         DeleteFont( lpFont->hFontPrinter );

      lpFont->hFontPrinter = CreateFontIndirect( &LogFont );
      lpFont->nFontHeightPrinter = nHeight;
      SS_SetFont( lpSS, lpFont, FontId );
   }

   return( lpFont->hFontPrinter );
}

// #define DKS_TEST
#ifdef  DKS_TEST
zBOOL
fnListVBuffer( LPSPREADSHEET lpSS, zBOOL bCheck );
#endif

zBOOL
SS_Draw( HWND              hWnd,
         HDC               hDC,
         LPSPREADSHEET     lpSS,
         LPRECT            lpRect,
         LPRECT            lpRectUpdate,
         SS_COORD          CellLeft,
         SS_COORD          CellTop,
         SS_COORD          CellRight,
         SS_COORD          CellBottom,
         SS_COORD          xCellMax,
         SS_COORD          yCellMax,
         LPSSX_PRINTFORMAT lpPFormat )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_Draw" );
   SS_CLRTBLITEM     BackClrTblItem;
   SS_CLRTBLITEM     ForeClrTblItem;
   zPCHAR            hColWidthList;
   zPCHAR            hRowHeightList;
   COLORREF          clrBkOld = CLR_INVALID;
   COLORREF          clrTextOld = CLR_INVALID;
   zPLONG            lpColWidthList = 0;
   zPLONG            lpRowHeightList = 0;
   LPSS_ROW          lpRow;
   LPSS_COL          lpCol;
   WORD              wColWidthListCnt = 0;
   WORD              wColWidthListAllocCnt = 0;
   WORD              wRowHeightListCnt = 0;
   WORD              wRowHeightListAllocCnt = 0;
   RECT              rectTemp;
   RECT              rectTempBorder;
   RECT              rectLast;
   LPRECT            lpCellOverflowRectPrev;
   LPRECT            lpCellOverflowRectNext;
#ifndef SS_NOOVERFLOW
   RECT              CellOverflowRectPrev;
   RECT              CellOverflowRectNext;
#endif
   HPEN              hPen;
   HPEN              hPenOld;
   HPEN              hPenGray = 0;
   zCHAR             LineType;
   zBOOL             fGridLinesVert = FALSE;
   zBOOL             fGridLinesHorz = FALSE;
   zBOOL             fRightColVis;
   zBOOL             fBottomRowVis;
   zBOOL             fColLinesDrawn = FALSE;
   zBOOL             fOverflowToLeft;
   zBOOL             fFirstCellPrintedInRow;
   zSHORT            nRightCol;
   zSHORT            nHeaderRowHeight;
   zSHORT            nHeaderColWidth;
   zLONG             lBottomRowY;
   SS_COORD          ssRowHeaderAt;
   SS_COORD          ssColHeaderAt;
   SS_COORD          ssColHeaderAtSave;
   SS_COORD          TempRowAt;
   SS_COORD          TempColAt;
   SS_COORD          ssRowAt;
   SS_COORD          ssColAt;
   SS_COORD          LastColPrinted;
   SS_COORD          CellOverflowColPrev;
   SS_COORD          CellOverflowColNext;
   zLONG             lCellOverflowPosPrev;
   zLONG             lCellOverflowPosNext;
   zLONG             lRowPosAt;
   zLONG             lColPosAt;
   SS_COORD          ssRowsMax;
   SS_COORD          ssColsMax;
   zLONG             lHeight;
   zLONG             lWidth;
   POINT             pt;
   SS_COORD          ss;

// TraceLineS( "SS_Draw", "" );
   if ( lpPFormat == 0 )
   {
      UnrealizeObject( lpSS->hBrushLines );

      SS_GetColorItem( lpSS, &ForeClrTblItem,
                       lpSS->Color.ForegroundId );
      hPen = kzCreatePen( PS_SOLID, 1, ForeClrTblItem.Color );

#ifdef SS_GRIDGRAY
         lpSS->wGridType |= SS_GRID_SOLID;
         lpSS->GridColor = RGBCOLOR_DARKGRAY;
#endif

      if ( lpSS->wGridType & SS_GRID_SOLID )
      {
         if ( GetDeviceCaps( hDC, NUMCOLORS ) == 2 )
            hPenGray = kzCreatePen( PS_SOLID, 1, RGBCOLOR_BLACK );
         else
            hPenGray = kzCreatePen( PS_SOLID, 1, lpSS->GridColor );
      }
   }
   else
      hPen = kzCreatePen( PS_SOLID, 1, RGBCOLOR_BLACK );

   SS_GetColorItem( lpSS, &BackClrTblItem, lpSS->Color.BackgroundId );

   if ( lpPFormat == 0 || lpPFormat->pf.fDrawColors )
      clrBkOld = SetBkColor( hDC, BackClrTblItem.Color );

   if ( lpPFormat == 0 )
      clrTextOld = SetTxColor( hDC, RGBCOLOR_BLACK ); // correct color???
//    clrTextOld = SetTxColor( hDC, lpSS->GridColor ); // correct color???

   hPenOld = SelectPen( hDC, hPen );
   LineType = SS_LINETYPE_SOLID;
   int nBkModeOld = SetBkMode( hDC, TRANSPARENT );

   if ( lpPFormat == 0 && lpSS->fPrintingInProgress == 0 )
   {
      lpColWidthList = (zPLONG) lpSS->Col.hCellSizeList;
      wColWidthListCnt = lpSS->Col.wCellSizeListCnt;
      wColWidthListAllocCnt = lpSS->Col.wCellSizeListAllocCnt;

      lpRowHeightList = (zPLONG) lpSS->Row.hCellSizeList;
      wRowHeightListCnt = lpSS->Row.wCellSizeListCnt;
      wRowHeightListAllocCnt = lpSS->Row.wCellSizeListAllocCnt;
   }

// if ( hColWidthList )
//    lpColWidthList = (zPLONG) DrLockTaskMemory( hColWidthList );
//
// if ( hRowHeightList )
//    lpRowHeightList = (zPLONG) DrLockTaskMemory( hRowHeightList );

   if ( lpPFormat && lpPFormat->pf.fShowGrid == 0 )
   {
//    fGridLinesHorz = FALSE;
//    fGridLinesVert = FALSE;
   }
   else
   {
      if ( lpSS->wGridType & SS_GRID_HORIZONTAL )
         fGridLinesHorz = TRUE;

      if ( lpSS->wGridType & SS_GRID_VERTICAL )
         fGridLinesVert = TRUE;
   }

   CellTop = mMax( CellTop, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
   CellLeft = mMax( CellLeft, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );

   // Setup row info.
   if ( CellRight == xCellMax - 1 )
      fRightColVis = TRUE;
   else
      fRightColVis = FALSE;

   if ( fRightColVis )
      nRightCol = lpRect->left + SS_GetCellPosX( lpSS, CellLeft, xCellMax - 1 ) +
                                 SS_GetColWidth( lpSS, xCellMax - 1 ) - 1;

   nHeaderColWidth = 0;
   ssRowHeaderAt = 0;

   if ( lpPFormat == 0 || (lpPFormat && lpPFormat->pf.fShowRowHeaders) )
   {
      for ( ss = 0; ss < lpSS->Col.nHeaderCnt; ss++ )
         nHeaderColWidth += SS_GetColWidth( lpSS, ss );
   }

   if ( lpPFormat && lpPFormat->pf.fShowColHeaders == 0 )
      ssRowHeaderAt = lpSS->Row.nHeaderCnt;

   ssRowsMax = yCellMax;
   ssRowAt = CellTop;
   lRowPosAt = -1;
   wRowHeightListCnt = 0;

   // Draw ssRow Lines.
   do
   {
      if ( ssRowHeaderAt < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
         TempRowAt = ssRowHeaderAt;
      else
         TempRowAt = ssRowAt;

      lHeight = SS_GetRowHeight( lpSS, TempRowAt );
      lRowPosAt += lHeight;

      // Create List of row height.
      if ( wRowHeightListCnt >= wRowHeightListAllocCnt )
      {
         if ( wRowHeightListAllocCnt == 0 )
         {
            wRowHeightListAllocCnt += 32;
            DrAllocTaskMemory( (zCOREMEM) &hRowHeightList,
                   (zLONG) (wRowHeightListAllocCnt * sizeof( zLONG )), 1014 );
         }
         else
         {
            hRowHeightList = (zPCHAR) DrUnlockTaskMemory( lpRowHeightList );
            wRowHeightListAllocCnt += 32;
            DrReAllocTaskMemory( (zCOREMEM) &hRowHeightList,
                   (zLONG) (wRowHeightListAllocCnt * sizeof( zLONG )) );
         }

         if ( hRowHeightList )
            lpRowHeightList = (zPLONG) DrLockTaskMemory( hRowHeightList );
         else
            lpRowHeightList = 0;
      }

      if ( lpRowHeightList )
         lpRowHeightList[ wRowHeightListCnt ] = lHeight;

      if ( lpPFormat == 0 )
      {
         lpSS->Row.LR = TempRowAt;

         if ( TempRowAt == CellTop ||
              lpRect->top + lRowPosAt <= lpRect->bottom )
         {
            lpSS->Row.LRAllVis = TempRowAt;
         }
      }

      if ( lpRect->top + lRowPosAt >= lpRectUpdate->top &&
           lpRect->top + lRowPosAt < lpRectUpdate->bottom )
      {
         // Draw ssRow Lines.
         pt.x = lpRect->left;
         pt.y = lpRect->top + lRowPosAt;
         SS_MoveTo( hDC, pt.x, pt.y, lpPFormat );

         if ( nHeaderColWidth )
         {
            LineType = SS_LINETYPE_SOLID;

            if ( lpPFormat )
               pt.x = lpRect->left +
                mMin( nHeaderColWidth - 1, lpRect->right - lpRect->left - 1 );
            else
               pt.x = lpRect->left +
                mMin( nHeaderColWidth - 2, lpRect->right - lpRect->left - 1 );

            if ( pt.x >= lpRectUpdate->left )
               SS_LineTo( lpSS, hDC, lpSS->hBrushLines,
                          lpSS->hBrushLines2, hPenGray, pt.x, pt.y,
                          LineType, fGridLinesHorz, lpPFormat );
            else
               SS_MoveTo( hDC, pt.x + 1, pt.y, lpPFormat ); // RFW 4/25/93
         }

         if ( ssRowHeaderAt < lpSS->Row.nHeaderCnt )
            LineType = SS_LINETYPE_SOLID;
         else
         if ( TempRowAt >= ssRowsMax - 1 )
         {
            if ( lpPFormat || lpSS->fNoBorder )
               LineType = SS_LINETYPE_NONE;
            else
            {
               LineType = SS_LINETYPE_SOLID;
            }
         }
         else
            LineType = SS_LINETYPE_DOT;

         if ( nHeaderColWidth < lpRect->right - lpRect->left )
         {
            pt.x = lpRect->right - 1;

            if ( pt.x >= lpRectUpdate->left )
            {
               if ( pt.x >= lpRectUpdate->right )
                  pt.x = lpRectUpdate->right - 1;

               if ( fRightColVis )
                  pt.x = mMin( pt.x,
                            nRightCol + (TempRowAt >= ssRowsMax - 1 ? 1 : 0) );

               SS_LineTo( lpSS, hDC, lpSS->hBrushLines,
                          lpSS->hBrushLines2, hPenGray, pt.x, pt.y,
                          LineType, fGridLinesHorz, lpPFormat );
            }
         }
      }

      if ( ssRowHeaderAt < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
         ssRowHeaderAt++;
      else
         ssRowAt++;

      wRowHeightListCnt++;

   } while ( lRowPosAt < lpRect->bottom && TempRowAt < ssRowsMax - 1 );

   // Setup col info.
   if ( CellBottom == yCellMax - 1 )
      fBottomRowVis = TRUE;
   else
      fBottomRowVis = FALSE;

   if ( fBottomRowVis )
   {
      lBottomRowY = lpRect->top + SS_GetCellPosY( lpSS, CellTop,
                                                  yCellMax - 1 ) +
                           SS_GetRowHeight( lpSS, yCellMax - 1 ) - 1;
   }

   nHeaderRowHeight = 0;
   ssColHeaderAtSave = 0;
   ssRowHeaderAt = 0;

   if ( lpPFormat == 0 || (lpPFormat && lpPFormat->pf.fShowColHeaders) )
   {
      for ( ss = 0; ss < lpSS->Row.nHeaderCnt; ss++ )
         nHeaderRowHeight += SS_GetRowHeight( lpSS, ss );
   }

   if ( lpPFormat && lpPFormat->pf.fShowColHeaders == 0 )
      ssRowHeaderAt = lpSS->Row.nHeaderCnt;

   if ( lpPFormat && lpPFormat->pf.fShowRowHeaders == 0 )
      ssColHeaderAtSave = lpSS->Col.nHeaderCnt;

   ssColsMax = xCellMax;
   ssColAt = CellLeft;
   lColPosAt = -1;

   ssRowAt = CellTop;
   lRowPosAt = 0;

   wRowHeightListCnt = 0;

   do
   {
      if ( ssRowHeaderAt < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
         TempRowAt = ssRowHeaderAt;
      else
         TempRowAt = ssRowAt;

      if ( lpRowHeightList )
         lHeight = lpRowHeightList[ wRowHeightListCnt ];
      else
         lHeight = SS_GetRowHeight( lpSS, TempRowAt );

      lRowPosAt += lHeight;

      lpRow = SS_LockRowItem( lpSS, TempRowAt );
      if ( lpRow == 0 || lpRow->fDataSet == FALSE )
      {
         SS_VQUERYDATA VQueryData;

#ifdef  DKS_TEST
         TraceLineI( "SS_Draw data not set for row ", TempRowAt );
         TraceLineI( "                       hItem ", (zLONG) lpRow );
         fnListVBuffer( lpSS, FALSE );
#endif

         if ( lpRow )
            SS_UnlockRowItem( lpSS, TempRowAt );

         zmemset( &VQueryData, 0, sizeof( SS_VQUERYDATA ) );
         VQueryData.ssRow = TempRowAt;
         VQueryData.wDirection = SS_VDOWN;
         VQueryData.ssRowsNeeded = 1;
         SS_SendMsg( hWnd, SSM_VQUERYDATA, GetWindowLong( hWnd, GWL_ID ),
                     (zLONG)(zPCHAR) &VQueryData );

         lpRow = SS_LockRowItem( lpSS, TempRowAt );
         if ( lpRow == 0 || lpRow->fDataSet == FALSE )
            TraceLineI( "SS_Draw data NOT set for row ", TempRowAt );
      }

      if ( fColLinesDrawn == 0 ||
           (lpRect->top + lRowPosAt - lHeight - 1 < lpRectUpdate->bottom &&
            lpRect->top + lRowPosAt - 1 >= lpRectUpdate->top) )
      {
         // Draw each column within the row.
         ssColAt = CellLeft;
         lColPosAt = -1;
         ssColHeaderAt = ssColHeaderAtSave;

         wColWidthListCnt = 0;

         CellOverflowColPrev = -1;
         lCellOverflowPosPrev = 0;
         CellOverflowColNext = -1;
         lCellOverflowPosNext = 0;
         fFirstCellPrintedInRow = FALSE;
         LastColPrinted = -1;
         fOverflowToLeft = FALSE;

         do
         {
            if ( ssColHeaderAt < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
               TempColAt = ssColHeaderAt;
            else
               TempColAt = ssColAt;

            if ( fColLinesDrawn && lpColWidthList )
               lWidth = lpColWidthList[ wColWidthListCnt ];
            else
               lWidth = SS_GetColWidth( lpSS, TempColAt );

            if ( fColLinesDrawn == 0 )
            {
               // Create list of column widths.
               if ( wColWidthListCnt >= wColWidthListAllocCnt )
               {
                  if ( wColWidthListAllocCnt == 0 )
                  {
                     wColWidthListAllocCnt = (zUSHORT) ssColsMax;
                     DrAllocTaskMemory( (zCOREMEM) &hColWidthList,
                           (wColWidthListAllocCnt * sizeof( zLONG )), 1016 );
                  }
                  else
                  {
                     hColWidthList =
                               (zPCHAR) DrUnlockTaskMemory( lpColWidthList );
                     wColWidthListAllocCnt = wColWidthListCnt + 16;
                     DrReAllocTaskMemory( (zCOREMEM) &hColWidthList,
                                 (wColWidthListAllocCnt * sizeof( zLONG )) );
                  }

                  if ( hColWidthList )
                     lpColWidthList =
                                  (zPLONG) DrLockTaskMemory( hColWidthList );
                  else
                     lpColWidthList = 0;
               }

               if ( lpColWidthList )
                  lpColWidthList[ wColWidthListCnt ] = lWidth;
            }

            lColPosAt += lWidth;
            if ( lpPFormat == 0 )
            {
               lpSS->Col.LR = TempColAt;

               if ( TempColAt == CellLeft ||
                    lpRect->left + lColPosAt <= lpRect->right )
               {
                  lpSS->Col.LRAllVis = TempColAt;
               }
            }

            if ( lpRect->left + lColPosAt - lWidth < lpRectUpdate->right &&
                 lpRect->left + lColPosAt >= lpRectUpdate->left )
            {
               // Draw ssCol Lines the first time through.
               if ( fColLinesDrawn == 0 )
               {
                  // Draw column lines.
                  pt.x = lpRect->left + lColPosAt;
                  pt.y = lpRect->top;
                  SS_MoveTo( hDC, pt.x, pt.y, lpPFormat );

                  if ( nHeaderRowHeight )
                  {
                     LineType = SS_LINETYPE_SOLID;

                     pt.y = lpRect->top +
                                    mMin( nHeaderRowHeight - 1,
                                          lpRect->bottom - lpRect->top - 1 );

                     if ( pt.y >= lpRectUpdate->top )
                        SS_LineTo( lpSS, hDC, lpSS->hBrushLines,
                                   lpSS->hBrushLines2, hPenGray,
                                   pt.x, pt.y, LineType,
                                   fGridLinesVert, lpPFormat );
                     else                                    // RFW 4/25/93
                        SS_MoveTo( hDC, pt.x, pt.y + 1, lpPFormat );
                  }

                  if ( ssColHeaderAt < lpSS->Col.nHeaderCnt )
                     LineType = SS_LINETYPE_SOLID;
                  else
                  if ( TempColAt >= ssColsMax - 1 )
                  {
                     if ( lpPFormat || lpSS->fNoBorder )
                        LineType = SS_LINETYPE_NONE;
                     else
                     {
                        LineType = SS_LINETYPE_SOLID;
                     }
                  }
                  else
                     LineType = SS_LINETYPE_DOT;

                  pt.y = lpRect->bottom - 1;

                  if ( pt.y > lpRectUpdate->top )
                  {
                     if ( pt.y >= lpRectUpdate->bottom )
                        pt.y = lpRectUpdate->bottom - 1;

                     if ( fBottomRowVis )
                        pt.y = mMin( pt.y, lBottomRowY );

                     SS_LineTo( lpSS, hDC, lpSS->hBrushLines,
                                lpSS->hBrushLines2, hPenGray,
                                pt.x, pt.y, LineType,
                                fGridLinesVert, lpPFormat );
                  }
               }

               lpCol = SS_LockColItem( lpSS, TempColAt );

               // Draw cell.
               rectTemp.left = lpRect->left + lColPosAt - lWidth + 1;
               rectTemp.top = lpRect->top + lRowPosAt - lHeight;
               rectTemp.right = lpRect->left + lColPosAt;
               rectTemp.bottom = lpRect->top + lRowPosAt - 1;
               CopyRect( &rectTempBorder, &rectTemp );

               SS_DrawBorder( hWnd, lpSS, hDC, lpCol, lpRow, &rectTempBorder,
                              TempColAt, TempRowAt, lpPFormat );

               if ( lpRect->top + lRowPosAt - lHeight < lpRectUpdate->bottom &&
                    lpRect->top + lRowPosAt - 1 > lpRectUpdate->top &&
                    lpRect->left + lColPosAt - lWidth + 1 < lpRectUpdate->right &&
                    lpRect->left + lColPosAt > lpRectUpdate->left &&
                    TempRowAt < ssRowsMax && TempColAt < ssColsMax )
               {
                  // Determine next empty cell for text overflow.
                  lpCellOverflowRectPrev = 0;
                  lpCellOverflowRectNext = 0;

#ifndef SS_NOOVERFLOW
                  if ( lpSS->fAllowCellOverflow )
                  {
                     LPSS_CELL lpCellTemp;
                     RECT      rectOverflow;
                     zBOOL     fOverflow;

                     if ( CellOverflowColNext == -1 ||
                         CellOverflowColNext <= TempColAt )
                     {
                        CellOverflowColNext = -1;
                        lCellOverflowPosNext = lpRect->left + lColPosAt;

                        for ( ss = TempColAt + 1; ss <= lpSS->Col.Max; ss++ )
                        {
                           fOverflow = FALSE;

                           if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow,
                                                      ss, TempRowAt )) != 0 )
                           {
                              if ( lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_LEFT )
                                 fOverflow = TRUE;

                              SS_UnlockCellItem( lpSS, ss, TempRowAt );
                           }

                           if ( fOverflow == 0 )
                              break;

                           lCellOverflowPosNext += SS_GetColWidth( lpSS, ss );
                           CellOverflowColNext = ss;
                        }
                     }
                     else
                        CellOverflowColNext = -1;

                     if ( CellOverflowColPrev != -1 )
                     {
                        SetRect( &CellOverflowRectPrev, lCellOverflowPosPrev,
                                 rectTemp.top, rectTemp.right,
                                 rectTemp.bottom );
                        lpCellOverflowRectPrev = &CellOverflowRectPrev;
                     }

                     if ( CellOverflowColNext != -1 )
                     {
                        SetRect( &CellOverflowRectNext, rectTemp.left,
                                 rectTemp.top, lCellOverflowPosNext,
                                 rectTemp.bottom );
                        lpCellOverflowRectNext = &CellOverflowRectNext;
                     }

                     if ( fFirstCellPrintedInRow == 0 &&
                          TempColAt >= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
                     {
                        if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow,
                                               TempColAt, TempRowAt )) != 0 )
                        {
                           if ( lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_LEFT )
                           {
                              if ( CellOverflowColNext == -1 )
                                 CopyRect( &rectOverflow, &rectTemp );
                              else
                                 UnionRect( &rectOverflow, &rectTemp,
                                            lpCellOverflowRectNext );

                              SS_DrawHiddenOverflowCellLeft( hWnd, hDC,
                                                             lpSS, lpRow,
                                                             &rectOverflow,
                                                             TempColAt,
                                                             TempRowAt,
                                                             lpPFormat,
                                                             lpRectUpdate );
                           }

                           SS_UnlockCellItem( lpSS, TempColAt, TempRowAt );
                        }

                        if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow,
                                           TempColAt - 1, TempRowAt )) != 0 )
                        {
                           if ( lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT )
                              fOverflowToLeft = TRUE;

                           SS_UnlockCellItem( lpSS, TempColAt, TempRowAt );
                        }

                        fFirstCellPrintedInRow = TRUE;
                     }

                     LastColPrinted = TempColAt;
                  }
#endif

                  // Draw cell contents.
                  CopyRect( &rectLast, &rectTemp );

                  SS_DrawText( hDC, hWnd, lpSS, lpCol, lpRow, &rectTemp,
                               TempColAt, TempRowAt, FALSE, lpPFormat,
                               lpRectUpdate, lpCellOverflowRectPrev,
                               lpCellOverflowRectNext, fOverflowToLeft );

                  fOverflowToLeft = FALSE;

                  // Determine previous empty cell for text overflow.
#ifndef SS_NOOVERFLOW
                  if ( lpSS->fAllowCellOverflow )
                  {
                     LPSS_CELL lpCellTemp;
                     zBOOL     fOverflow = FALSE;

                     if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow,
                                               TempColAt, TempRowAt )) != 0 )
                     {
                        if ( (lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT) )
                           fOverflow = TRUE;

                        SS_UnlockCellItem( lpSS, TempColAt, TempRowAt );
                     }

                     if ( fOverflow )
                     {
                        if ( CellOverflowColPrev == -1 )
                        {
                           // zBOOL fDone = FALSE;

                           CellOverflowColPrev = TempColAt;
                           lCellOverflowPosPrev = rectLast.left;
                        }

                        fOverflowToLeft = TRUE;
                     }
                     else
                        CellOverflowColPrev = -1;
                  }
#endif
               }

               SS_UnlockColItem( lpSS, TempColAt );
            }

            if ( ssColHeaderAt < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
               ssColHeaderAt++;
            else
               ssColAt++;

            wColWidthListCnt++;

         } while ( lColPosAt < lpRect->right - lpRect->left &&
                   TempColAt < ssColsMax - 1 );

         fColLinesDrawn = TRUE;

#ifndef SS_NOOVERFLOW
         if ( lpSS->fAllowCellOverflow && LastColPrinted != -1 &&
              LastColPrinted < lpSS->Col.Max )
         {
            LPSS_CELL lpCellTemp;
            RECT      rectOverflow;

            if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow,
                                                 LastColPrinted,
                                                 TempRowAt )) != 0 )
            {
               if ( lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT )
               {
                  if ( lpCellOverflowRectPrev == 0 )
                     CopyRect( &rectOverflow, &rectLast );
                  else
                     UnionRect( &rectOverflow, &rectLast,
                                lpCellOverflowRectPrev );

                  SS_DrawHiddenOverflowCellRight( hWnd, hDC, lpSS,
                                                  lpRow, &rectOverflow,
                                                  LastColPrinted, TempRowAt,
                                                  lpPFormat, lpRectUpdate );
               }

               SS_UnlockCellItem( lpSS, LastColPrinted, TempRowAt );
            }

            fFirstCellPrintedInRow = TRUE;
         }
#endif
      }

      SS_UnlockRowItem( lpSS, TempRowAt );

      if ( ssRowHeaderAt < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
         ssRowHeaderAt++;
      else
         ssRowAt++;

      wRowHeightListCnt++;

   } while ( lRowPosAt < lpRect->bottom && TempRowAt < ssRowsMax - 1 );

   if ( lpPFormat == 0 && lColPosAt < lpRect->right - lpRect->left )
   {
      rectTemp.left = mMax( lpRectUpdate->left, lpRect->left + lColPosAt + 1 );
      rectTemp.top = mMax( lpRectUpdate->top, lpRect->top );
      rectTemp.right = mMin( lpRectUpdate->right, lpRect->right );
      rectTemp.bottom = mMin( lpRectUpdate->bottom, lpRect->bottom );

      SS_HatchRect( hDC, lpSS, &rectTemp, lpPFormat );
   }

   lRowPosAt--;

   if ( lpPFormat == 0 && lRowPosAt < lpRect->bottom - lpRect->top )
   {
      rectTemp.left = mMax( lpRectUpdate->left, lpRect->left );
      rectTemp.top = mMax( lpRectUpdate->top, lpRect->top + lRowPosAt + 1 );
      rectTemp.right = mMin( lpRectUpdate->right, lpRect->right );
      rectTemp.bottom = mMin( lpRectUpdate->bottom, lpRect->bottom );

      SS_HatchRect( hDC, lpSS, &rectTemp, lpPFormat );
   }

   if ( lpColWidthList )
   {
      if ( lpPFormat || lpSS->fPrintingInProgress )
      {
         if ( lpSS->Col.hCellSizeList )
         {
            lpSS->Col.hCellSizeList = (zPCHAR)
                              DrUnlockTaskMemory( lpSS->Col.hCellSizeList );
            DrFreeTaskMemory( lpSS->Col.hCellSizeList );
            lpSS->Col.hCellSizeList = 0;
         }

         hColWidthList = (zPCHAR) DrUnlockTaskMemory( lpColWidthList );
         DrFreeTaskMemory( hColWidthList );
         lpSS->Col.wCellSizeListAllocCnt = 0;
         lpSS->Col.wCellSizeListCnt = 0;
      }
      else
      {
         lpSS->Col.hCellSizeList = (zPCHAR) lpColWidthList;
         lpSS->Col.wCellSizeListCnt = wColWidthListCnt;
         lpSS->Col.wCellSizeListAllocCnt = wColWidthListAllocCnt;
      }
   }

   if ( lpRowHeightList )
   {
      if ( lpPFormat || lpSS->fPrintingInProgress )
      {
         if ( lpSS->Row.hCellSizeList )
         {
            lpSS->Row.hCellSizeList = (zPCHAR)
                              DrUnlockTaskMemory( lpSS->Row.hCellSizeList );
            DrFreeTaskMemory( lpSS->Row.hCellSizeList );
            lpSS->Row.hCellSizeList = 0;
         }

         hRowHeightList = (zPCHAR) DrUnlockTaskMemory( lpRowHeightList );
         DrFreeTaskMemory( hRowHeightList );
         lpSS->Row.wCellSizeListAllocCnt = 0;
         lpSS->Row.wCellSizeListCnt = 0;
      }
      else
      {
         lpSS->Row.hCellSizeList = (zPCHAR) lpRowHeightList;
         lpSS->Row.wCellSizeListCnt = wRowHeightListCnt;
         lpSS->Row.wCellSizeListAllocCnt = wRowHeightListAllocCnt;
      }
   }

   SetBkMode( hDC, nBkModeOld );

   if ( clrBkOld != CLR_INVALID )
      SetBkColor( hDC, clrBkOld );

   if ( clrTextOld != CLR_INVALID )
      SetTxColor( hDC, clrTextOld );

   SelectPen( hDC, hPenOld );
   DeletePen( hPen );

   if ( hPenGray )
      DeletePen( hPenGray );

   return( 0 );
}

void
SSx_DrawBorder( LPSPREADSHEET lpSS,
                HDC hDC,
                zLONG lLeft,
                zLONG lTop,
                zLONG lRight,
                zLONG lBottom,
                LPSSX_PRINTFORMAT lpPFormat,
                WORD  wBorderStyle, COLORREF  Color )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SSx_DrawBorder" );
   SS_CLRTBLITEM BackClrTblItem;
   COLORREF        clrBkOld;
   HPEN            hPen;
   HPEN            hPenOld;
   int             nBkModeOld;
   int             nWidth;
   int             nPenStyle;

   if ( wBorderStyle == SS_BORDERSTYLE_NONE )
      return;

   nBkModeOld = SetBkMode( hDC, OPAQUE );
   nWidth = 1;
   nPenStyle = PS_SOLID;

   SS_GetColorItem( lpSS, &BackClrTblItem, lpSS->Color.BackgroundId );
   clrBkOld = SetBkColor( hDC, BackClrTblItem.Color );

   switch ( wBorderStyle )
   {
      case SS_BORDERSTYLE_SOLID:
         nPenStyle = PS_SOLID;
         break;

      case SS_BORDERSTYLE_DASH:
         nPenStyle = PS_DASH;
         break;

      case SS_BORDERSTYLE_DOT:
         nPenStyle = PS_DOT;
         break;

      case SS_BORDERSTYLE_DASHDOT:
         nPenStyle = PS_DASHDOT;
         break;

      case SS_BORDERSTYLE_DASHDOTDOT:
         nPenStyle = PS_DASHDOTDOT;
         break;

      case SS_BORDERSTYLE_BLANK:
         Color = BackClrTblItem.Color;
         nPenStyle = PS_SOLID;
         break;

      case SS_BORDERSTYLE_DOUBLESOLID:
      case SS_BORDERSTYLE_SOLID2:
      case SS_BORDERSTYLE_SOLID3:
         break;
   }

   hPen = kzCreatePen( nPenStyle, nWidth, Color );
   SS_MoveTo( hDC, lLeft, lTop, lpPFormat );
   hPenOld = SelectPen( hDC, hPen );
   SS_LineTo( lpSS, hDC, 0, 0, 0, lRight, lBottom, SS_LINETYPE_SOLID,
              TRUE, lpPFormat );
   SelectPen( hDC, hPenOld );
   DeletePen( hPen );

   if ( clrBkOld != CLR_INVALID )
      SetBkColor( hDC, clrBkOld );

   SetBkMode( hDC, nBkModeOld );
}

zBOOL
SSx_GetBorder( HWND hWnd, LPSPREADSHEET lpSS, LPSS_COL lpCol,
               LPSS_ROW lpRow, LPSS_CELL lpCell, SS_COORD ssCol,
               SS_COORD ssRow, WORD *lpwBorderType, WORD *lpwBorderStyle,
               LPCOLORREF lpColor )
{
   TRACE_DEBUG( "<SS_BORD.CPP>", "SSx_GetBorder" );
   SS_CLRTBLITEM ClrTblItem;
   LPSS_CELL       lpCellOrig = lpCell;
   SS_BORDER       Border;
   zBOOL           fRet = FALSE;

   if ( lpSS->Border.bType )
   {
      fRet = TRUE;
      zmemcpy( &Border, &lpSS->Border, sizeof( SS_BORDER ) );
   }

   if ( ssCol != SS_ALLCOLS && lpCol && lpCol->Border.bType )
   {
      fRet = TRUE;
      zmemcpy( &Border, &lpCol->Border, sizeof( SS_BORDER ) );
   }

   if ( ssRow != SS_ALLROWS && lpRow && lpRow->Border.bType )
   {
      fRet = TRUE;
      zmemcpy( &Border, &lpRow->Border, sizeof( SS_BORDER ) );
   }

   if ( ssRow != SS_ALLROWS && ssCol != SS_ALLCOLS &&
        (lpCell || (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0) &&
        lpCell->m_Border.bType )
   {
      fRet = TRUE;
      zmemcpy( &Border, &lpCell->m_Border, sizeof( SS_BORDER ) );
   }

   if ( lpCell && lpCellOrig == 0 )
      SS_UnlockCellItem( lpSS, ssCol, ssRow );

   if ( fRet )
   {
      if ( Border.ColorId > 0 )
      {
         if ( lpwBorderType )
            *lpwBorderType = (WORD) Border.bType;

         if ( lpwBorderStyle )
            *lpwBorderStyle = (WORD) Border.bStyle;

         if ( lpColor )
         {
            SS_GetColorItem( lpSS, &ClrTblItem, Border.ColorId );
            *lpColor = ClrTblItem.Color;
         }
      }
      else
         fRet = FALSE;
   }

   return( fRet );
}

zBOOL
SS_SetBorder( HWND          hWnd,
              LPSPREADSHEET lpSS,
              SS_COORD      ssCol,
              SS_COORD      ssRow,
              WORD          wBorderType,
              WORD          wBorderStyle,
              COLORREF      Color )
{
   TRACE_DEBUG( "<SS_BORD.CPP>", "SS_SetBorder" );
   LPSS_CELL     lpCell;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   SS_COORD      ss;
   SS_COORD      j;
   zBOOL         fRet = FALSE;

   if ( ssCol != SS_ALLCOLS && ssRow != SS_ALLROWS )
   {
      if ( (lpCell = SS_AllocLockCell( lpSS, 0, ssCol, ssRow )) != 0 )
      {
         SS_SetBorderItem( hWnd, lpSS, &lpCell->m_Border, wBorderType,
                           wBorderStyle, Color );

         SS_UnlockCellItem( lpSS, ssCol, ssRow );
         SS_InvalidateCell( hWnd, lpSS, ssCol, ssRow );
         fRet = TRUE;
      }
   }
   else
   if ( ssCol != SS_ALLCOLS && ssRow == SS_ALLROWS )
   {
      for ( ss = 1; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ss )) != 0 )
         {
            zmemset( &lpCell->m_Border, 0, sizeof( SS_BORDER ) );
            SS_UnlockCellItem( lpSS, ssCol, ss );
         }
      }

      if ( (lpCol = SS_AllocLockCol( lpSS, ssCol )) != 0 )
      {
         SS_SetBorderItem( hWnd, lpSS, &lpCol->Border, wBorderType,
                           wBorderStyle, Color );

         SS_UnlockColItem( lpSS, ssCol );
         SS_InvalidateCol( hWnd, lpSS, ssCol );
         fRet = TRUE;
      }
   }
   else
   if ( ssCol == SS_ALLCOLS && ssRow != SS_ALLROWS )
   {
      for ( ss = 0; ss < lpSS->Col.AllocCnt; ss++ )
      {
         if ( (lpCell = SS_LockCellItem( lpSS, ss, ssRow )) != 0 )
         {
            zmemset( &lpCell->m_Border, 0, sizeof( SS_BORDER ) );
            SS_UnlockCellItem( lpSS, ss, ssRow );
         }
      }

      if ( (lpRow = SS_AllocLockRow( lpSS, ssRow )) != 0 )
      {
         SS_SetBorderItem( hWnd, lpSS, &lpRow->Border, wBorderType,
                           wBorderStyle, Color );

         SS_UnlockRowItem( lpSS, ssRow );
         SS_InvalidateRow( hWnd, lpSS, ssRow );
         fRet = TRUE;
      }
   }
   else
   {
      // Clear All ssCols, ssRows, and Cells.
      for ( ss = 1; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
         {
            zmemset( &lpRow->Border, 0, sizeof( SS_BORDER ) );

            for ( j = 1; j < lpSS->Col.AllocCnt; j++ )
            {
               if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, j, ss )) != 0 )
               {
                  zmemset( &lpCell->m_Border, 0, sizeof( SS_BORDER ) );
                  SS_UnlockCellItem( lpSS, j, ss );
               }
            }
         }
      }

      for ( ss = 1; ss < lpSS->Col.AllocCnt; ss++ )
      {
         if ( (lpCol = SS_LockColItem( lpSS, ss )) != 0 )
         {
            zmemset( &lpCol->Border, 0, sizeof( SS_BORDER ) );
            SS_UnlockColItem( lpSS, ss );
         }
      }

      SS_SetBorderItem( hWnd, lpSS, &lpSS->Border, wBorderType,
                        wBorderStyle, Color );
      SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
      fRet = TRUE;
   }

   return( fRet );
}

zBOOL
SS_SetBorderItem( HWND          hWnd,
                  LPSPREADSHEET lpSS,
                  LPSS_BORDER   lpBorder,
                  WORD          wBorderType,
                  WORD          wBorderStyle,
                  COLORREF      Color )
{
   TRACE_DEBUG( "<SS_BORD.CPP>", "SS_SetBorderItem" );
   if ( wBorderType == SS_BORDERTYPE_NONE ||
        wBorderStyle == SS_BORDERSTYLE_NONE )
   {
      lpBorder->bType = 0;
      lpBorder->bStyle = 0;
   }
   else
   {
      lpBorder->bType |= (BYTE) wBorderType;
      lpBorder->bStyle = (BYTE) wBorderStyle;
   }

   if ( Color != -1 )
      lpBorder->ColorId = SS_AddColor( lpSS, Color );

   return( TRUE );
}

void
SS_DrawBorder( HWND hWnd,
               LPSPREADSHEET lpSS,
               HDC hDC,
               LPSS_COL lpCol,
               LPSS_ROW lpRow,
               LPRECT   lpRect,
               SS_COORD ssCol,
               SS_COORD ssRow,
               LPSSX_PRINTFORMAT lpPFormat )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_DrawBorder" );
   COLORREF  Color;
   LPSS_CELL lpCell = 0;
   WORD      wBorderType;
   WORD      wBorderStyle;
   RECT      rectTemp;

   if ( lpSS->fSetBorder == 0 )
      return;

   lpCell = SS_LockCellItem( lpSS, ssCol, ssRow );

   if ( SSx_GetBorder( hWnd, lpSS, lpCol, lpRow, 0, ssCol, ssRow,
                       &wBorderType, &wBorderStyle, &Color ) )
   {
      SetRect( &rectTemp, lpRect->left - 1, lpRect->top - 1, lpRect->right,
               lpRect->bottom );

      if ( (wBorderType & SS_BORDERTYPE_LEFT) && ssCol != lpSS->Col.UL )
         SSx_DrawBorder( lpSS, hDC, rectTemp.left, rectTemp.top,
                         rectTemp.left, rectTemp.bottom, lpPFormat,
                         wBorderStyle, Color );

      if ( (wBorderType & SS_BORDERTYPE_TOP) && ssRow != lpSS->Row.UL )
         SSx_DrawBorder( lpSS, hDC, rectTemp.left, rectTemp.top,
                         rectTemp.right, rectTemp.top,
                         lpPFormat, wBorderStyle, Color );

      if ( wBorderType & SS_BORDERTYPE_RIGHT )
         SSx_DrawBorder( lpSS, hDC, rectTemp.right, rectTemp.top,
                         rectTemp.right, rectTemp.bottom, lpPFormat,
                         wBorderStyle, Color );

      if ( wBorderType & SS_BORDERTYPE_BOTTOM )
         SSx_DrawBorder( lpSS, hDC, rectTemp.left, rectTemp.bottom,
                         rectTemp.right, rectTemp.bottom, lpPFormat,
                         wBorderStyle, Color );
   }

   if ( lpCell )
      SS_UnlockCellItem( lpSS, ssCol, ssRow );
}

void
SS_DrawCell( HDC hDC, HWND hWnd, LPSPREADSHEET lpSS, LPSS_COL lpCol,
             LPSS_ROW lpRow, LPRECT  lpRect, SS_COORD CellCol,
             SS_COORD CellRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_DrawCell" );
   HDC     hDCMemory;
   HBITMAP hBitmapOld;
   HBITMAP hBitmapDC;
   RECT    rectTemp;

   // Create a memory device context.
   hBitmapDC = kzCreateCompatibleBitmap( hDC, lpRect->right - lpRect->left,
                                         lpRect->bottom - lpRect->top );

   hDCMemory = CreateCompatibleDC( hDC );
   hBitmapOld = SelectBitmap( hDCMemory, hBitmapDC );

   SetRect( &rectTemp, 0, 0, lpRect->right - lpRect->left,
            lpRect->bottom - lpRect->top );

   // Draw Text.
   SS_DrawText( hDCMemory, hWnd, lpSS, lpCol, lpRow, &rectTemp, CellCol,
                CellRow, TRUE, 0, 0, 0, 0, FALSE );

   // Copy the memory device context bitmap to the display.
   BitBlt( hDC, lpRect->left, lpRect->top, lpRect->right - lpRect->left,
           lpRect->bottom - lpRect->top, hDCMemory, 0, 0, SRCCOPY );

   // Clean up.
   SelectBitmap( hDCMemory, hBitmapOld );
   DeleteBitmap( hBitmapDC );
   DeleteDC( hDCMemory );
}

void
SS_DrawComboBtn( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 HDC           hDC,
                 LPRECT        lpRect,
                 zSHORT        nButtonWidth,
                 zBOOL         fButtonDown )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_DrawComboBtn" );
   SS_CELLTYPE   CellType;
   RECT          rectTemp;

   rectTemp.left = mMax( lpRect->left, lpRect->right - nButtonWidth + 1 );
   rectTemp.top = lpRect->top - 1;
   rectTemp.right = lpRect->right + 1;
   rectTemp.bottom = lpRect->bottom + 1;

   CellType.lStyle = 0;
   CellType.Spec.Button.hText = 0;
   CellType.Spec.Button.hPictName = (zPCHAR) lpSS->hBitmapComboBtn;
   CellType.Spec.Button.hPictDownName = 0;
   CellType.Spec.Button.nPictureType = SUPERBTN_PICT_HANDLE |
                                       SUPERBTN_PICT_BITMAP;
   CellType.Spec.Button.nPictureDownType = 0;
   CellType.Spec.Button.nButtonType = SUPERBTN_TYPE_NORMAL;
   CellType.Spec.Button.nShadowSize = 1;
   CellType.Spec.Button.Color.Color = GetSysColor( COLOR_BTNFACE );
   CellType.Spec.Button.Color.ColorBorder = GetSysColor( COLOR_BTNTEXT );
   CellType.Spec.Button.Color.ColorShadow = GetSysColor( COLOR_BTNSHADOW );
   CellType.Spec.Button.Color.ColorHilite = GetSysColor( COLOR_BTNHILIGHT );
   CellType.Spec.Button.Color.ColorText = GetSysColor( COLOR_BTNTEXT );

   SS_BtnDraw( hWnd, hDC, &rectTemp, &CellType, 0, fButtonDown,
               FALSE, FALSE, 0 );
}

void
SS_DrawFormatHeaderLetter( zPCHAR lpBuffer, zLONG lHeaderNumber )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_DrawFormatHeaderLetter" );
   zSHORT k;

   *lpBuffer = 0;
   if ( lHeaderNumber >= 1 )
   {
      do
      {
         for ( k = zstrlen( lpBuffer ) + 1; k > 0; k-- )
            lpBuffer[ k ] = lpBuffer[ k - 1 ];

         lpBuffer[ 0 ] = (char) ('A' + ((lHeaderNumber - 1) % 26));
         lHeaderNumber = (lHeaderNumber - 1) / 26;

      } while ( lHeaderNumber > 0 );
   }
}

void
SS_DrawFormatHeaderNumber( zPCHAR lpBuffer, zLONG lHeaderNumber )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_DrawFormatHeaderNumber" );
   wsprintf( lpBuffer, "%ld", lHeaderNumber );
}

zBOOL
SS_DrawSelBlock( HWND          hWnd,
                 HDC           hDC,
                 LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_DrawSelBlock" );
   zBOOL         fRet;

   if ( lpSS->fMultipleBlocksSelected )
      fRet = SS_InvertMultiSelBlock( hWnd, hDC, lpSS );
   else
      fRet = SS_InvertBlock( hWnd, hDC, lpSS );

   return( fRet );
}

void
SS_DrawShadows( HDC     hDC,
                LPRECT  lpRect,
                zSHORT  nShadowSize,
                HBRUSH  hBrushShadow,
                HBRUSH  hBrushHilite,
                zBOOL   fNoLeft )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_DrawShadows" );
   HBRUSH  hBrushOld;
   zSHORT k;

   // Create left and top.
   hBrushOld = SelectBrush( hDC, hBrushHilite );

   // Left.
   if ( !fNoLeft )
      for ( k = 0; k < nShadowSize; k++ )
         PatBlt( hDC, lpRect->left + k, lpRect->top, 1,
                 lpRect->bottom - lpRect->top - k, PATCOPY );

   // Top.
   for ( k = 0; k < nShadowSize; k++ )
      PatBlt( hDC, lpRect->left, lpRect->top + k,
              lpRect->right - lpRect->left - k, 1, PATCOPY );

   // Create right and bottom.
   SelectBrush( hDC, hBrushShadow );

   // Right.
   for ( k = 0; k < nShadowSize; k++ )
      PatBlt( hDC, lpRect->right - k - 1, lpRect->top + k, 1,
              lpRect->bottom - lpRect->top - k, PATCOPY );

   // Bottom.
   for ( k = 0; k < nShadowSize; k++ )
      PatBlt( hDC, lpRect->left + k, lpRect->bottom - k - 1,
              lpRect->right - lpRect->left - k, 1, PATCOPY );

   SelectBrush( hDC, hBrushOld );
}

zBOOL
SS_DrawText( HDC               hDC,
             HWND              hWnd,
             LPSPREADSHEET     lpSS,
             LPSS_COL          lpCol,
             LPSS_ROW          lpRow,
             LPRECT            rect,
             SS_COORD          CellCol,
             SS_COORD          CellRow,
             zBOOL             fErase,
             LPSSX_PRINTFORMAT lpPFormat,
             LPRECT            lpRectUpdate,
             LPRECT            lpCellOverflowRectPrev,
             LPRECT            lpCellOverflowRectNext,
             zBOOL             fOverflowToLeft )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_DrawText" );
   zPCHAR            hGlobalData = 0;
   HFONT             hFontOld;
   SS_CLRTBLITEM     ClrTblItem;
   SS_CLRTBLITEM     ClrTblItemFore;
   SS_CLRTBLITEM     ClrTblItemLight;
   COLORREF          clrBk;
   COLORREF          clrBkOld = CLR_INVALID;
   COLORREF          clrTextOld = CLR_INVALID;
   HBRUSH            hBrush;
   HBRUSH            hBrush1;
   HBRUSH            hBrush2;
   HFONT             hFont;
   RECT              rectOrig;
   RECT              rectOrigNoScale;
   RECT              rectTemp;
   RECT              rectUpdateTemp;
   RECT              CellOverflowRectPrevTemp;
   RECT              CellOverflowRectNextTemp;
   LPSS_CELL         Cell;
   LPSS_CELLTYPE     CellType;
   SS_CELLTYPE       CellTypeTemp;
   ZSS_Data          SSData;
   LPSS_FONT         Font;
   SS_FONT           FontTemp;
   SS_FONTID         FontId;
   SIZE              size;
   zBOOL             fButtonState;
   zCHAR             Temp[ 10 ];
   zCHAR             szBuffer[ 200 ];
   zPCHAR            pchData = 0;
   HWND              hWndCtrl = 0;
   zDECIMAL          dxValue;
   SS_COORD          Number;
   zSHORT            nShadowSize;
   zLONG             lJust;
   zLONG             x;
   zLONG             y;
   zUSHORT           k;

// TraceLineI( "SS_DrawText Col: ", CellCol );
   if ( rect->left >= rect->right || rect->top >= rect->bottom )
      return( TRUE );

   int nBkModeOld = SetBkMode( hDC, TRANSPARENT );
   CopyRect( &rectOrigNoScale, rect );

   if ( lpPFormat )
   {
      SS_ScaleRect( lpPFormat, rect );
      CopyRect( &rectOrig, rect );

      rectUpdateTemp.left = SS_SCREEN2PRINTERX( lpPFormat, lpRectUpdate->left );
      rectUpdateTemp.top = SS_SCREEN2PRINTERY( lpPFormat, lpRectUpdate->top );
      rectUpdateTemp.right = SS_SCREEN2PRINTERX( lpPFormat, lpRectUpdate->right );
      rectUpdateTemp.bottom = SS_SCREEN2PRINTERY( lpPFormat, lpRectUpdate->bottom );

      lpRectUpdate = &rectUpdateTemp;

      if ( lpCellOverflowRectPrev )
      {
         CopyRect( &CellOverflowRectPrevTemp, lpCellOverflowRectPrev );
         lpCellOverflowRectPrev = &CellOverflowRectPrevTemp;

         SS_ScaleRect( lpPFormat, lpCellOverflowRectPrev );
      }

      if ( lpCellOverflowRectNext )
      {
         CopyRect( &CellOverflowRectNextTemp, lpCellOverflowRectNext );
         lpCellOverflowRectNext = &CellOverflowRectNextTemp;

         SS_ScaleRect( lpPFormat, lpCellOverflowRectNext );
      }
   }
   else
      CopyRect( &rectOrig, rect );

   Cell = SSx_LockCellItem( lpSS, lpRow, CellCol, CellRow );

#ifndef SS_NOOVERFLOW
   if ( lpSS->fAllowCellOverflow )
   {
      if ( Cell && Cell->m_CellData.m_bOverflow )
      {
         SSx_UnlockCellItem( lpSS, lpRow, CellCol, CellRow );
         return( TRUE );
      }
   }
#endif

   CellType = SSx_RetrieveCellType( lpSS, &CellTypeTemp, lpCol, lpRow,
                                    Cell, CellCol, CellRow );

   SSData.m_DataType = 0;
   if ( CellType->chType == SS_TYPE_FLOAT )
   {
      if ( (hGlobalData = SSx_GetData( lpSS, lpCol, lpRow,
                                       Cell, CellType, &SSData,
                                       CellCol, CellRow, TRUE )) != 0 )
      {
//       pchData = (zPCHAR) DrLockTaskMemory( hGlobalData );
         pchData = (zPCHAR) hGlobalData;
      }
      else
      if ( SSData.m_DataType == SS_TYPE_FLOAT )
         pchData = (zPCHAR) 1;
   }
   else
   {
      if ( (hGlobalData = SSx_GetData( lpSS, lpCol, lpRow,
                                       Cell, CellType, 0,
                                       CellCol, CellRow, TRUE )) != 0 )
      {
//       pchData = (zPCHAR) DrLockTaskMemory( hGlobalData );
         pchData = (zPCHAR) hGlobalData;
      }
   }

   // Format headers.
   if ( CellCol >= lpSS->Col.nHeaderCnt && CellRow < lpSS->Row.nHeaderCnt &&
        (!hGlobalData || !Cell || !Cell->m_CellData.m_DataType) )
   {
      if ( CellRow == 0 )
      {
         Number = CellCol - lpSS->Col.nHeaderCnt + lpSS->Col.NumStart;

         if ( lpSS->ColHeaderDisplay == SS_HEADERDISPLAY_LETTERS )
            SS_DrawFormatHeaderLetter( Temp, Number );
         else
         if ( lpSS->ColHeaderDisplay == SS_HEADERDISPLAY_NUMBERS )
            SS_DrawFormatHeaderNumber( Temp, Number );
         else
            Temp[ 0 ] = 0;

         pchData = Temp;
      }
   }
   else
   if ( CellRow >= lpSS->Row.nHeaderCnt &&
        CellCol < lpSS->Col.nHeaderCnt &&
        (!hGlobalData || !Cell || !Cell->m_CellData.m_DataType) )
   {
      if ( CellCol == 0 )
      {
         if ( lpSS->fVirtualMode &&
              (lpSS->Virtual.lVStyle & SSV_NOROWNUMBERS) )
         {
            pchData = "";
         }
         else
         {
            Number = CellRow - lpSS->Row.nHeaderCnt + lpSS->Row.NumStart;

            if ( lpSS->RowHeaderDisplay == SS_HEADERDISPLAY_LETTERS )
               SS_DrawFormatHeaderLetter( Temp, Number );
            else
            if ( lpSS->RowHeaderDisplay == SS_HEADERDISPLAY_NUMBERS )
               SS_DrawFormatHeaderNumber( Temp, Number );
            else
               Temp[ 0 ] = 0;

            pchData = Temp;
         }
      }
   }
   else
   if ( CellCol == 0 && CellRow == 0 &&
        (!Cell || !Cell->m_CellData.m_DataType) )
   {
      pchData = "";
   }

   lpSS->CurVisCell.ssRow = CellRow;
   lpSS->CurVisCell.ssCol = CellCol;

   // If CellRow == 0 ... then CellType->chType should be SS_TYPE_STATICTEXT.

   switch ( CellType->chType )
   {
      case SS_TYPE_STATICTEXT:
      {
         // Paint Cell Background.
         SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.BackgroundId );
         clrBk = ClrTblItem.Color;

         if ( lpSS->LockColor.BackgroundId &&
              SSx_GetLock( hWnd, lpSS, lpCol, lpRow, Cell, CellCol, CellRow ) )
         {
            SS_GetColorItem( lpSS, &ClrTblItem,
                             lpSS->LockColor.BackgroundId );
         }

         hBrush = ClrTblItem.hBrush;
         SSx_GetClrTblItem( hWnd, lpSS, lpCol, lpRow, Cell, CellCol,
                              CellRow, &ClrTblItem, &ClrTblItemFore );

         if ( lpSS->fAllowCellOverflow )
         {
            lJust = SS_RetrieveJust( CellType->lStyle );
            if ( !(lJust & DT_WORDBREAK) )
            {
               if ( SS_CalcOverflowRect( hDC, lpSS, pchData, CellCol,
                                         lJust, rect,
                                         lpCellOverflowRectPrev,
                                         lpCellOverflowRectNext, rect ) )
               {
                  fErase = TRUE;
               }
            }
         }

         // Draw static text.
         if ( CellType->lStyle & SS_TEXT_SHADOWIN )
         {
            SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowColorId );
            FillRect( hDC, rect, ClrTblItem.hBrush );

            SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowDarkId );
            SS_GetColorItem( lpSS, &ClrTblItemLight, lpSS->Color.ShadowLightId );

            HBRUSH hBrushOld = SelectBrush( hDC, ClrTblItem.hBrush );

            // Left.
            PatBlt( hDC, rect->left + 1, rect->top, 1,
                    rect->bottom - rect->top - 1, PATCOPY );

            // Top.
            PatBlt( hDC, rect->left, rect->top + 1,
                    rect->right - rect->left - 1, 1, PATCOPY );

            SelectBrush( hDC, hBrushOld );

            SS_DrawShadows( hDC, rect, 1, ClrTblItem.hBrush,
                            ClrTblItemLight.hBrush, TRUE );
            SS_ShrinkRect( rect, 2, 0 );
         }
         else
         if ( (lpPFormat == 0 || lpPFormat->pf.fDrawShadows) &&
              (CellType->lStyle & (SS_TEXT_SHADOW | SS_TEXT_SHADOWIN)) )
         {
            SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowColorId );
            if ( ClrTblItem.Color != clrBk || fErase )
            {
               hBrush = ClrTblItem.hBrush;
               UnrealizeObject( hBrush );
               FillRect( hDC, rect, ClrTblItem.hBrush );
            }

            SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowDarkId );
            SS_GetColorItem( lpSS, &ClrTblItemLight,
                             lpSS->Color.ShadowLightId );

            if ( CellType->lStyle & SS_TEXT_SHADOW )
            {
               hBrush1 = ClrTblItem.hBrush;
               hBrush2 = ClrTblItemLight.hBrush;
            }
            else
            {
               hBrush1 = ClrTblItemLight.hBrush;
               hBrush2 = ClrTblItem.hBrush;
            }

            if ( CellCol == 0 && CellRow == 0 )
               nShadowSize = 2;
            else
               nShadowSize = 1;

            SS_DrawShadows( hDC, rect, nShadowSize, hBrush1, hBrush2,
                            fOverflowToLeft ? TRUE : FALSE );
            SS_ShrinkRect( rect, nShadowSize, nShadowSize );
            if ( fOverflowToLeft )
               rect->left -= nShadowSize;
         }
         else
         if ( (lpPFormat == 0 || lpPFormat->pf.fDrawColors || fErase) &&
              (ClrTblItem.Color != clrBk || fErase) )
         {
            if ( lpPFormat && lpPFormat->pf.fDrawColors == 0 && fErase )
               hBrush = (HBRUSH) GetStockObject( WHITE_BRUSH );
            else
            {
               hBrush = ClrTblItem.hBrush;
               UnrealizeObject( hBrush );
            }

            FillRect( hDC, rect, hBrush );
         }

         if ( pchData )
         {
            FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow,
                                         Cell, CellCol, CellRow );
            Font = SS_GetFont( lpSS, &FontTemp, FontId );
            if ( Font )
            {
               if ( lpPFormat )
                  hFont = SS_CreatePrinterFont( lpSS, Font, FontId, lpPFormat );
               else
                  hFont = Font->hFont;

               hFontOld = SelectFont( hDC, hFont );
            }
            else
               hFontOld = 0;

            if ( CellType->lStyle & (SS_TEXT_SHADOW | SS_TEXT_SHADOWIN) )
               SS_GetColorItem( lpSS, &ClrTblItemFore, lpSS->Color.ShadowTextId );
            else
            if ( lpSS->LockColor.ForegroundId &&
                 SSx_GetLock( hWnd, lpSS, lpCol, lpRow,
                              Cell, CellCol, CellRow ) )
            {
               SS_GetColorItem( lpSS, &ClrTblItemFore,
                                lpSS->LockColor.ForegroundId );
            }

            if ( lpPFormat == 0 || lpPFormat->pf.fDrawColors )
               clrTextOld = SetTxColor( hDC, ClrTblItemFore.Color );

            lJust = SS_RetrieveJust( CellType->lStyle );
            if ( lJust & DT_WORDBREAK )
            {
               if ( fOverflowToLeft )
                  rectTemp.left = rect->left;
               else
                  rectTemp.left = rect->left + lpSS->nMarginX;

               rectTemp.right = rect->right - lpSS->nMarginX;
               rectTemp.top = rect->top + lpSS->nMarginY;
               rectTemp.bottom = rect->top + lpSS->nMarginY;

               DrawText( hDC, pchData, -1, &rectTemp, lJust | DT_CALCRECT );

               if ( lJust & DT_BOTTOM )
                  rectTemp.top = rect->top +
                               mMax( (rect->bottom - rect->top) -
                                    (rectTemp.bottom - rectTemp.top), 0 );
               else
               if ( lJust & DT_VCENTER )
                  rectTemp.top = rect->top +
                         mMax( (rect->bottom - rect->top) -
                                 (rectTemp.bottom - rectTemp.top), 0 ) / 2;

               rectTemp.bottom = rect->bottom;
               rectTemp.right = rect->right;

               SSx_DrawText( hDC, lpPFormat, pchData, -1,
                             &rectTemp, &rectOrig, lJust );
            }
            else
            {
               CopyRect( &rectTemp, rect );
               SS_ShrinkRect( &rectTemp,
                              fOverflowToLeft ? 0 : lpSS->nMarginX,
                              (lJust & DT_VCENTER) ? 0 : lpSS->nMarginY );

               SSx_DrawText( hDC, lpPFormat, pchData, -1,
                             &rectTemp, &rectOrig, lJust );
            }

            if ( clrTextOld != CLR_INVALID )
               SetTxColor( hDC, clrTextOld );

            if ( hFontOld )
               SelectFont( hDC, hFontOld );
         }

         break;
      }

      case SS_TYPE_PICTURE:
      {
         MessageBox( 0, "SS_TYPE_PICTURE", "SPREAD.CPP", MB_OK );
#if 0
         hWndCtrl = SS_RetrieveControlhWnd( hWnd, lpSS, CellCol, CellRow );

         if ( hWndCtrl )
         {
            CopyRect( &ViewPictPaint.Rect, rect );
            CopyRect( &ViewPictPaint.RectOrig, &rectOrigNoScale );
            ViewPictPaint.fUseDib = (zBOOL) (lpPFormat ? 1 : 0);
            SendMessage( hWndCtrl, VPM_PAINT, (UINT) hDC,
                         (zLONG)(zPCHAR) &ViewPictPaint );
         }
#endif
         break;
      }

#if ( !defined( SS_NOCT_BUTTON ) || !defined( SS_NOCT_CHECK ) )
      case SS_TYPE_BUTTON:
      case SS_TYPE_CHECKBOX:
      {
         rectTemp.left = rect->left - 1;
         rectTemp.top = rect->top - 1;
         rectTemp.right = rect->right + 1;
         rectTemp.bottom = rect->bottom + 1;

         FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow,
                                      Cell, CellCol, CellRow );

         hFont = 0;

         if ( (Font = SS_GetFont( lpSS, &FontTemp, FontId )) != 0 )
         {
            if ( lpPFormat )
               hFont = SS_CreatePrinterFont( lpSS, Font, FontId, lpPFormat );
            else
               hFont = Font->hFont;
         }

         fButtonState = 0;

         if ( CellType->chType == SS_TYPE_BUTTON &&
              CellType->Spec.Button.nButtonType == SUPERBTN_TYPE_2STATE )
         {
            if ( pchData && pchData[ 0 ] == '1' )
               fButtonState = 1;
         }
         else
         if ( CellType->chType == SS_TYPE_CHECKBOX )
         {
            if ( pchData )
            {
               if ( (CellType->lStyle & 0x0F) == BS_3STATE ||
                    (CellType->lStyle & 0x0F) == BS_AUTO3STATE )
               {
                  if ( pchData[ 0 ] == '1' )
                     fButtonState = 1;
                  else
                  if ( pchData[ 0 ] == '2' )
                     fButtonState = 2;
               }
               else
               {
                  if ( pchData[ 0 ] == 0 || (pchData[ 1 ] == 0 && pchData[ 0 ] == '0') )
                     fButtonState = 0;
                  else
                     fButtonState = 1;
               }
            }
         }

#ifndef SS_NOCT_BUTTON
         if ( CellType->chType == SS_TYPE_BUTTON &&
              SS_ShouldButtonBeDrawn( lpSS, CellCol, CellRow ) )
         {
            SS_BtnDraw( hWnd, hDC, &rectTemp, CellType, hFont,
                        fButtonState, TRUE,
                        (zBOOL)(lpPFormat ? 1 : 0), &rectOrigNoScale );
         }
#endif

#ifndef SS_NOCT_CHECK
         if ( CellType->chType == SS_TYPE_CHECKBOX )
         {
            hWndCtrl = SS_RetrieveControlhWnd( hWnd, lpSS, CellCol, CellRow );
            if ( hWndCtrl )
            {
               SS_CheckBoxDraw( hWnd, hWndCtrl, hDC, rect, CellType, hFont,
                                fButtonState, (zBOOL)(lpPFormat ? 1 : 0),
                                &rectOrigNoScale );
            }
         }
#endif

         break;
      }
#endif

#ifndef SS_NOCT_COMBO
      case SS_TYPE_COMBOBOX:
      {
      // TraceLineI( "SS_DrawText ComboBox Row: ", CellRow );

         // Paint Cell Background.
         SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.BackgroundId );
         clrBk = ClrTblItem.Color;
         hBrush = ClrTblItem.hBrush;

         SSx_GetClrTblItem( hWnd, lpSS, lpCol, lpRow, Cell, CellCol,
                            CellRow, &ClrTblItem, &ClrTblItemFore );

         if ( lpSS->LockColor.BackgroundId &&
              SSx_GetLock( hWnd, lpSS, lpCol, lpRow, Cell, CellCol, CellRow ) )
         {
            SS_GetColorItem( lpSS, &ClrTblItem,
                             lpSS->LockColor.BackgroundId );
         }

#if 0
         SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowColorId );
         FillRect( hDC, rect, ClrTblItem.hBrush );
#else
         HBRUSH hWhiteBrush = kzCreateSolidBrush( ::GetSysColor( COLOR_WINDOW ) );
         FillRect( hDC, rect, hWhiteBrush );
         DeleteBrush( hWhiteBrush );
#endif
         SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowDarkId );
         SS_GetColorItem( lpSS, &ClrTblItemLight, lpSS->Color.ShadowLightId );

         SS_DrawShadows( hDC, rect, 1, ClrTblItemLight.hBrush,
                         ClrTblItem.hBrush, FALSE );
         SS_ShrinkRect( rect, 2, 0 );

         if ( pchData )
         {
            FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow,
                                         Cell, CellCol, CellRow );
            Font = SS_GetFont( lpSS, &FontTemp, FontId );

            if ( lpPFormat )
               hFont = SS_CreatePrinterFont( lpSS, Font, FontId, lpPFormat );
            else
               hFont = Font->hFont;

            hFontOld = SelectFont( hDC, hFont );

            if ( lpSS->LockColor.ForegroundId &&
                 SSx_GetLock( hWnd, lpSS, lpCol, lpRow,
                              Cell, CellCol, CellRow ) )
            {
               SS_GetColorItem( lpSS, &ClrTblItemFore,
                                lpSS->LockColor.ForegroundId );
            }

            if ( lpPFormat == 0 || lpPFormat->pf.fDrawColors )
               clrTextOld = SetTxColor( hDC, ClrTblItemFore.Color );

            zmemcpy( &rectTemp, rect, sizeof( RECT ) );

            rectTemp.right -= lpSS->nComboButtonBitmapWidth - 1;
            rectTemp.left += lpSS->nMarginX;
            rectTemp.top += lpSS->nMarginY;

            DrawText( hDC, pchData, -1, &rectTemp,
                      DT_LEFT | DT_SINGLELINE | DT_NOPREFIX );

            if ( clrTextOld != CLR_INVALID )
               SetTxColor( hDC, clrTextOld );

            SelectFont( hDC, hFontOld );
         }

         // Draw Button ... commented next line to always draw button
         if ( lpPFormat == 0 )
            // && SS_ShouldButtonBeDrawn( lpSS, CellCol, CellRow ) )
         {
               SS_DrawComboBtn( hWnd, lpSS, hDC, rect,
                                lpSS->nComboButtonBitmapWidth, FALSE );
         }

         break;
      }
#endif

      case SS_TYPE_EDIT:
      case SS_TYPE_PIC:
      case SS_TYPE_DATE:
      case SS_TYPE_TIME:
      case SS_TYPE_INTEGER:
      case SS_TYPE_FLOAT:
      {
         if ( CellType->chType == SS_TYPE_EDIT &&
              (CellType->lStyle & ES_RIGHT) &&
              !( CellType->lStyle & ES_MULTILINE ) )
         {
            lJust = DT_RIGHT | DT_SINGLELINE;
         }
         else
         if ( CellType->chType == SS_TYPE_EDIT &&
              (CellType->lStyle & ES_MULTILINE) )
         {
            if ( CellType->lStyle & ES_RIGHT )
               lJust = DT_RIGHT | DT_WORDBREAK;
            else
            if ( CellType->lStyle & ES_CENTER )
               lJust = DT_CENTER | DT_WORDBREAK;
            else
               lJust = DT_LEFT | DT_WORDBREAK;
         }
         else
         if ( CellType->lStyle & ES_RIGHT )
            lJust = DT_RIGHT | DT_SINGLELINE;
         else
         if ( CellType->lStyle & ES_CENTER )
            lJust = DT_CENTER | DT_SINGLELINE;
         else
         if ( CellType->lStyle & ES_LEFTALIGN )
            lJust = DT_LEFT | DT_SINGLELINE;
         else
         if ( CellType->chType == SS_TYPE_INTEGER ||
              CellType->chType == SS_TYPE_FLOAT )
         {
            lJust = DT_RIGHT | DT_SINGLELINE;
         }
         else
            lJust = DT_LEFT | DT_SINGLELINE;

         lJust |= DT_NOPREFIX;

         if ( lpSS->fAllowCellOverflow )
         {
            if ( CellType->chType != SS_TYPE_EDIT ||
                 !(CellType->lStyle & ES_MULTILINE) )
            {
               if ( lpSS->fAllowCellOverflow )
               {
                  if ( SS_CalcOverflowRect( hDC, lpSS, pchData, CellCol, lJust,
                                            rect, lpCellOverflowRectPrev,
                                            lpCellOverflowRectNext, rect ) )
                  {
                     fErase = TRUE;
                  }
               }
            }
         }

         // Paint Cell Background.
         SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowColorId );
     //x FillRect( hDC, rect, ClrTblItem.hBrush );

         HBRUSH hWhiteBrush = kzCreateSolidBrush( ::GetSysColor( COLOR_WINDOW ) );
         FillRect( hDC, rect, hWhiteBrush );
         DeleteBrush( hWhiteBrush );

         SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.ShadowDarkId );
         SS_GetColorItem( lpSS, &ClrTblItemLight, lpSS->Color.ShadowLightId );

         SS_DrawShadows( hDC, rect, 1, ClrTblItem.hBrush,
                         ClrTblItemLight.hBrush, FALSE );
         SS_ShrinkRect( rect, 2, 0 );

         if ( pchData )
         {
            FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow,
                                         Cell, CellCol, CellRow );
            Font = SS_GetFont( lpSS, &FontTemp, FontId );

            if ( Font )
            {
               if ( lpPFormat )
                  hFont = SS_CreatePrinterFont( lpSS, Font, FontId, lpPFormat );
               else
                  hFont = Font->hFont;

               hFontOld = SelectFont( hDC, hFont );
            }
            else
               hFontOld = 0;

            if ( lpSS->LockColor.ForegroundId &&
                 SSx_GetLock( hWnd, lpSS, lpCol, lpRow,
                              Cell, CellCol, CellRow ) )
            {
               SS_GetColorItem( lpSS, &ClrTblItemFore,
                                lpSS->LockColor.ForegroundId );

               if ( lpPFormat == 0 || lpPFormat->pf.fDrawColors )
                  clrTextOld = SetTxColor( hDC, ClrTblItemFore.Color );
            }

            if ( CellType->chType == SS_TYPE_EDIT &&
                 (CellType->lStyle & ES_MULTILINE) )
            {
               rect->left += 2;
               rect->top += 1;
               rect->right -= 1;
               rect->bottom -= 1;
            }
            else
            {
               if ( lJust & DT_RIGHT )
                  rect->right -= lpSS->nMarginX;
               else
               if ( !(lJust & DT_CENTER) )
                  rect->left += lpSS->nMarginX;

               rect->top += lpSS->nMarginY;
            }

            if ( SSData.m_DataType == SS_TYPE_FLOAT )
            {
               zmemset( szBuffer, 0, sizeof( szBuffer ) );
               SS_FloatFormat( lpSS, CellType, SSData.m_Data.dxValue,
                               szBuffer, FALSE );
               pchData = szBuffer;
            }
            else
            if ( CellType->chType == SS_TYPE_FLOAT )
            {
               if ( *pchData )
               {
                  zmemset( szBuffer, 0, sizeof( szBuffer ) );
                  dxValue = ZDecimal( pchData );
               // StringToFloat( pchData, &dxValue ),
                  SS_FloatFormat( lpSS, CellType, dxValue, szBuffer, FALSE );
                  pchData = szBuffer;
               }
            }
            else
            if ( CellType->chType == SS_TYPE_EDIT &&
                 (CellType->lStyle & ES_PASSWORD) )
            {
               for ( k = 0; k < zstrlen( pchData ); k++ )
                  szBuffer[ k ] = '*';

               szBuffer[ zstrlen( pchData ) ] = 0;
               pchData = szBuffer;
            }

            if ( lpPFormat )
            {
               if ( lJust & DT_WORDBREAK )
               {
                  CopyRect( &rectTemp, rect );
                  DrawText( hDC, pchData, -1, &rectTemp, lJust | DT_CALCRECT );
                  if ( lJust & DT_BOTTOM )
                     rectTemp.top = rect->top +
                              mMax( (rect->bottom - rect->top) -
                                       (rectTemp.bottom - rectTemp.top), 0 );
                  else
                  if ( lJust & DT_VCENTER )
                     rectTemp.top = rect->top +
                              mMax( (rect->bottom - rect->top) -
                                   (rectTemp.bottom - rectTemp.top), 0 ) / 2;

                  rectTemp.bottom = rect->bottom;
                  rectTemp.right = rect->right;

                  SSx_DrawText( hDC, lpPFormat, pchData, -1,
                                &rectTemp, &rectOrig, lJust );
               }
               else
               {
                  y = rect->top;
                  GetTextExtentPoint( hDC, pchData, zstrlen( pchData ), &size );

                  if ( lJust & DT_VCENTER )
                     y = rect->top +
                                (((rect->bottom - rect->top) - size.cy) / 2);

                  if ( lJust & DT_RIGHT )
                  {
                     x = rect->right - 1;
                     SetTextAlign( hDC, TA_TOP | TA_RIGHT );
                  }
                  else
                  if ( (lJust & DT_CENTER) && !(lJust & DT_WORDBREAK) )
                  {
                     SetTextAlign( hDC, TA_TOP | TA_LEFT );

                     x = rect->left +
                                (((rect->right - rect->left) - size.cx) / 2);
                  }
                  else
                  {
                     x = rect->left;
                     SetTextAlign( hDC, TA_TOP | TA_LEFT );
                  }

                  if ( IntersectRect( rect, lpRectUpdate, rect ) )
                     ExtTextOut( hDC, x, y, ETO_CLIPPED, rect, pchData,
                                 zstrlen( pchData ), 0 );

                  SetTextAlign( hDC, TA_TOP | TA_LEFT );
               }
            }
            else
               SSx_DrawText( hDC, lpPFormat, pchData, -1, rect,
                             &rectOrig, lJust | DT_NOPREFIX );

            if ( clrTextOld != CLR_INVALID )
               SetTxColor( hDC, clrTextOld );

            if ( hFontOld )
               SelectFont( hDC, hFontOld );
         }

         break;
      }

#if 0  // SS_NOCT_OWNERDRAW
      case SS_TYPE_OWNERDRAW:
      {
         SS_DRAWITEMSTRUCT DrawItem;
         HRGN              hRgn;

         SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.BackgroundId );
         clrBk = ClrTblItem.Color;

         SSx_GetClrTblItem( hWnd, lpSS, lpCol, lpRow, Cell, CellCol,
                              CellRow, &ClrTblItem, &ClrTblItemFore );

         if ( ClrTblItem.Color != clrBk || fErase )
         {
            hBrush = ClrTblItem.hBrush;
            UnrealizeObject( hBrush );
            FillRect( hDC, rect, ClrTblItem.hBrush );
         }

         FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow,
                                      Cell, CellCol, CellRow );
         Font = SS_GetFont( lpSS, &FontTemp, FontId );

         if ( lpPFormat )
            hFont = SS_CreatePrinterFont( lpSS, Font, FontId, lpPFormat );
         else
            hFont = Font->hFont;

         hFontOld = SelectFont( hDC, hFont );

         if ( lpSS->LockColor.ForegroundId &&
              SSx_GetLock( hWnd, lpSS, lpCol, lpRow,
                           Cell, CellCol, CellRow ) )
         {
            SS_GetColorItem( lpSS, &ClrTblItemFore,
                             lpSS->LockColor.ForegroundId );
         }

         if ( lpPFormat == 0 || lpPFormat->pf.fDrawColors )
            clrTextOld = SetTxColor( hDC, ClrTblItemFore.Color );

         DrawItem.CtlID = GetWindowLong( hWnd, GWL_ID );
         DrawItem.ssCol = CellCol;
         DrawItem.ssRow = CellRow;
         DrawItem.hWnd = hWnd;
         DrawItem.hDC = hDC;
         DrawItem.lStyle = CellType->lStyle;
         DrawItem.pchData = pchData;
         CopyRect( &DrawItem.Rect, rect );

         if ( lpPFormat == 0 )
         {
            IntersectRect( &rectTemp, rect, lpRectUpdate );
            hRgn = CreateRectRgnIndirect( &rectTemp );
            SelectClipRgn( hDC, hRgn );
            zDeleteObjectRegion( hRgn );
         }

         SS_SendMsg( hWnd, SSM_DRAWITEM, 0, (zLONG)(zPCHAR)&DrawItem );

         if ( lpPFormat == 0 )
            SelectClipRgn( hDC, 0 );

         if ( clrTextOld != CLR_INVALID )
            SetTxColor( hDC, clrTextOld );

         SelectFont( hDC, hFontOld );

   //      if ( lpPFormat )            //???
   //         DeleteFont( hFont );
      }

         break;
#endif

#if 0
      case SS_TYPE_CUSTOM:
         if ( hWndCtrl && pchData )
         {
            FontId = SSx_RetrieveFontId( lpSS, lpCol, lpRow,
                                         Cell, CellCol, CellRow );
            Font = SS_GetFont( lpSS, &FontTemp, FontId );
            SendMessage( hWndCtrl, WM_SETFONT, Font->hFont, 1L );
            SendMessage( hWndCtrl, WM_SETTEXT, 0, (zLONG) pchData );
            TraceLineS( "SSDrawText: ", "MoveWindow" );
            MoveWindow( hWndCtrl, rect->left, rect->top,
                        rect->right - rect->left,
                        rect->bottom - rect->top, TRUE );

            lpSS->NoEraseBkgnd = TRUE;
            ShowWindow( hWndCtrl, SW_SHOWNA );
            UpdateWindow( hWndCtrl );
            lpSS->NoEraseBkgnd = TRUE;
            ShowWindow( hWndCtrl, SW_HIDE );
            ValidateRect( hWnd, 0 );
            lpSS->NoEraseBkgnd = FALSE;
         }
         else
         {
            SS_GetColorItem( lpSS, &ClrTblItem, lpSS->Color.BackgroundId );

            clrBk = ClrTblItem.Color;

            SSx_GetClrTblItem( hWnd, lpSS, lpCol, lpRow, Cell, CellCol,
                                 CellRow, &ClrTblItem, 0 );

            if ( ClrTblItem.Color != clrBk || fErase )
               FillRect( hDC, rect, ClrTblItem.hBrush );
         }

         break;
#endif
   }

// if ( hGlobalData )
//    DrUnlockTaskMemory( pchData );

   SetBkMode( hDC, nBkModeOld );
   SSx_UnlockCellItem( lpSS, lpRow, CellCol, CellRow );

   lpSS->CurVisCell.ssRow = lpSS->Row.ssCurrAt;
   lpSS->CurVisCell.ssCol = lpSS->Col.ssCurrAt;

   return( TRUE );
}

void
SS_EraseBkgnd( HWND hWnd, LPSPREADSHEET lpSS, HDC hDC,
               LPRECT  lprectClient, LPRECT  lpRectUpdate,
               SS_COORD RightVisCell, SS_COORD BottomVisCell )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_EraseBkgnd" );
   SS_CLRTBLITEM BackClrTblItem;
   RECT          rectTemp;
   RECT          rect;

   CopyRect( &rect, lprectClient );

   if ( RightVisCell == SS_GetColsMax( lpSS ) - 1 )
      rect.right = mMin( rect.right,
                         rect.left +
                          SS_GetCellPosX( lpSS, lpSS->Col.UL,
                                          SS_GetColsMax( lpSS ) - 1 ) +
                          SS_GetColWidth( lpSS,
                                          SS_GetColsMax( lpSS ) - 1 ) + 1 );

   if ( BottomVisCell == SS_GetRowsMax( lpSS ) - 1 )
      rect.bottom = mMin( rect.bottom,
                          rect.top +
                            SS_GetCellPosY( lpSS, lpSS->Row.UL,
                                            SS_GetRowsMax( lpSS ) - 1 ) +
                            SS_GetRowHeight( lpSS,
                                             SS_GetRowsMax( lpSS ) - 1 ) + 1 );

// SS_GetColorItem( lpSS, &BackClrTblItem, lpSS->Color.BackgroundId ); DKS
   SS_GetColorItem( lpSS, &BackClrTblItem, lpSS->Color.ShadowColorId );

   IntersectRect( &rectTemp, &rect, lpRectUpdate );
   UnrealizeObject( BackClrTblItem.hBrush );
// TraceRect( "SS_EraseBkgnd", rectTemp );
   FillRect( hDC, &rectTemp, BackClrTblItem.hBrush );
}

SS_COORD
SS_GetBottomCell( HWND          hWnd,
                  LPSPREADSHEET lpSS,
                  SS_COORD      ssRowTop )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetBottomCell" );
   RECT     rect;
   SS_COORD ssRowAt;
   SS_COORD MaxRows;
   zLONG    y;

   SS_GetClientRect( hWnd, &rect );
   MaxRows = SS_GetRowsMax( lpSS );

   y = SS_GetCellPosY( lpSS, ssRowTop, ssRowTop );
   for ( ssRowAt = ssRowTop;
         ssRowAt < MaxRows && y <= rect.bottom - rect.top;
         ssRowAt++ )
   {
      y += SS_GetRowHeight( lpSS, ssRowAt );
   }

   ssRowAt--;

   if ( ssRowAt < ssRowTop )
      ssRowAt = -1;

   return( ssRowAt );
}

SS_COORD
SS_GetBottomVisCell( HWND          hWnd,
                     LPSPREADSHEET lpSS,
                     SS_COORD      ssRowTop )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetBottomVisCell" );
   RECT          rect;
   SS_COORD      ssRowAt;
   SS_COORD      MaxRows;
   zLONG         y;
   zSHORT        cy;

   SS_GetClientRect( hWnd, &rect );
   MaxRows = SS_GetRowsMax( lpSS );

   y = SS_GetCellPosY( lpSS, ssRowTop, ssRowTop );
   for ( cy = 0, ssRowAt = ssRowTop;
         ssRowAt < MaxRows && y + cy <= rect.bottom - rect.top;
         ssRowAt++ )
   {
      y += cy;
      cy = SS_GetRowHeight( lpSS, ssRowAt );
      if ( y + cy > rect.bottom - rect.top )
         break;
   }

   ssRowAt--;

   if ( ssRowAt < ssRowTop )
      ssRowAt = -1;

   return( ssRowAt );
}

zBOOL
SS_GetCellCoord( LPSPREADSHEET lpSS,
                 SS_COORD      ssColLeft,
                 SS_COORD      ssRowTop,
                 SS_COORD      ssCol,
                 SS_COORD      ssRow,
                 zPLONG        x,
                 zPLONG        y,
                 zPLONG        cx,
                 zPLONG        cy )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetCellCoord" );
   *x  = SS_GetCellPosX( lpSS, ssColLeft, ssCol );
   *y  = SS_GetCellPosY( lpSS, ssRowTop, ssRow );
   *cx = SS_GetColWidth( lpSS, ssCol );
   *cy = SS_GetRowHeight( lpSS, ssRow );

   return( TRUE );
}

zSHORT
SS_GetCellPosX( LPSPREADSHEET lpSS,
                SS_COORD      ssColLeft,
                SS_COORD      ssCol )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetCellPosX" );
   SS_COORD  ss;
   zSHORT    k;

   for ( ss = 0, k = -1;
         ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen && ss < ssCol;
         ss++ )
   {
      k += SS_GetColWidth( lpSS, ss );
   }

   for ( ss = mMax( ss, ssColLeft ); ss < ssCol; ss++ )
   {
      k += SS_GetColWidth( lpSS, ss );
   }

   return( k );
}

zSHORT
SS_GetCellPosXExt( LPSPREADSHEET lpSS,
                   SS_COORD ssColLeft, SS_COORD ssCol,
                   LPRECT  lpRect )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetCellPosXExt" );
   SS_COORD  ss;
   zSHORT    k;

   for ( ss = 0, k = -1;
         ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen && ss < ssCol;
         ss++ )
   {
      k += SS_GetColWidth( lpSS, ss );
   }

   for ( ss = mMax( ss, ssColLeft ); ss < ssCol; ss++ )
   {
      k += SS_GetColWidth( lpSS, ss );
      if ( k > lpRect->right - lpRect->left )
         break;
   }

   return( k );
}

zSHORT
SS_GetCellPosY( LPSPREADSHEET lpSS,
                SS_COORD      ssRowTop,
                SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetCellPosY" );
   SS_COORD  ss;
   zSHORT    k;

   for ( ss = 0, k = -1;
         ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen && ss < ssRow;
         ss++ )
   {
      k += SS_GetRowHeight( lpSS, ss );
   }

   for ( ss = mMax( ss, ssRowTop ); ss < ssRow; ss++ )
      k += SS_GetRowHeight( lpSS, ss );

   return( k );
}

zSHORT
SS_GetCellPosYExt( LPSPREADSHEET lpSS,
                   SS_COORD ssRowTop, SS_COORD ssRow,
                   LPRECT  lpRect )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetCellPosYExt" );
   SS_COORD ss;
   zSHORT   k;

   for ( ss = 0, k = -1;
         ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen && ss < ssRow;
         ss++ )
   {
      k += SS_GetRowHeight( lpSS, ss );
   }

   for ( ss = mMax( ss, ssRowTop ); ss < ssRow; ss++ )
   {
      k += SS_GetRowHeight( lpSS, ss );
      if ( k > lpRect->bottom - lpRect->top )
         break;
   }

   return( k );
}

zBOOL
SS_GetClrTblItem( HWND                hWnd,
                    LPSPREADSHEET     lpSS,
                    SS_COORD          ssCol,
                    SS_COORD          ssRow,
                    LPSS_CLRTBLITEM BackClrTblItem,
                    LPSS_CLRTBLITEM ForeClrTblItem )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetClrTblItem" );
   LPSS_ROW   lpRow;
   LPSS_COL   lpCol;
   zBOOL      bRC;

   lpCol = SS_LockColItem( lpSS, ssCol );
   lpRow = SS_LockRowItem( lpSS, ssRow );

   bRC = SSx_GetClrTblItem( hWnd, lpSS, lpCol, lpRow, 0, ssCol, ssRow,
                              BackClrTblItem, ForeClrTblItem );

   SS_UnlockColItem( lpSS, ssCol );
   SS_UnlockRowItem( lpSS, ssRow );

   return( TRUE );
}

zBOOL
SSx_GetClrTblItem( HWND                hWnd,
                     LPSPREADSHEET     lpSS,
                     LPSS_COL          lpCol,
                     LPSS_ROW          lpRow,
                     LPSS_CELL         lpCell,
                     SS_COORD          ssCol,
                     SS_COORD          ssRow,
                     LPSS_CLRTBLITEM BackClrTblItem,
                     LPSS_CLRTBLITEM ForeClrTblItem )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SSx_GetClrTblItem" );
   LPSS_CELL   lpCellOrig;
   SS_COLORID  BackgroundId;
   SS_COLORID  ForegroundId;

   lpCellOrig = lpCell;

   BackgroundId = lpSS->Color.BackgroundId;
   ForegroundId = lpSS->Color.ForegroundId;

   if ( ssCol != -1 && lpCol )
   {
      if ( lpCol->Color.BackgroundId )
         BackgroundId = lpCol->Color.BackgroundId;

      if ( lpCol->Color.ForegroundId )
         ForegroundId = lpCol->Color.ForegroundId;
   }

   if ( ssRow != -1 && lpRow )
   {
      if ( lpRow->Color.BackgroundId )
         BackgroundId = lpRow->Color.BackgroundId;

      if ( lpRow->Color.ForegroundId )
         ForegroundId = lpRow->Color.ForegroundId;
   }

   if ( ssRow != -1 && ssCol != -1 &&
        (lpCell || (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0) )
   {
      if ( lpCell->m_Color.BackgroundId )
         BackgroundId = lpCell->m_Color.BackgroundId;

      if ( lpCell->m_Color.ForegroundId )
         ForegroundId = lpCell->m_Color.ForegroundId;

      if ( lpCellOrig == 0 )
         SS_UnlockCellItem( lpSS, ssCol, ssRow );
   }

   if ( BackClrTblItem )
   {
      zmemset( BackClrTblItem, 0, sizeof( SS_CLRTBLITEM ) );
      SS_GetColorItem( lpSS, BackClrTblItem, BackgroundId );
   }

   if ( ForeClrTblItem )
   {
      zmemset( ForeClrTblItem, 0, sizeof( SS_CLRTBLITEM ) );
      SS_GetColorItem( lpSS, ForeClrTblItem, ForegroundId );
   }

   return( TRUE );
}

zSHORT
SS_GetColWidth( LPSPREADSHEET lpSS,
                SS_COORD ssCol )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetColWidth" );
   LPSS_COL      lpCol;
   zPLONG        plPrintColWidthList;
   zLONG         lColWidth;

   if ( lpSS->hPrintColWidthList && ssCol < lpSS->lPrintColWidthListCnt &&
        lpSS->fPaintInProcess == 0 )
   {
//    plPrintColWidthList = (zPLONG)
//                          DrLockTaskMemory( lpSS->hPrintColWidthList );
      plPrintColWidthList = (zPLONG) lpSS->hPrintColWidthList;
      lColWidth = plPrintColWidthList[ ssCol ];
//    DrUnlockTaskMemory( plPrintColWidthList );
      return( (zSHORT) lColWidth );
   }

   lColWidth = lpSS->Col.nCellSizeInPixels;

   if ( ssCol != -1 && (lpCol = SS_LockColItem( lpSS, ssCol )) != 0 )
   {
      if ( lpCol->nColWidthInPixels != SS_WIDTH_DEFAULT )
         lColWidth = lpCol->nColWidthInPixels;

//    SS_UnlockColItem( lpSS, ssCol );
   }

   return( (zSHORT) lColWidth );
}

LPSS_CLRTBLITEM
SS_GetColorItem( LPSPREADSHEET     lpSS,
                 LPSS_CLRTBLITEM ClrTblItem,
                 SS_COLORID        ColorId )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetColorItem" );
   LPSS_CLRTBLITEM lpColorTable;

   if ( ColorId > 0 && ColorId <= g_nColorTableCnt )
   {
//    lpColorTable = (LPSS_CLRTBLITEM) DrLockTaskMemory( g_hColorTable );
      lpColorTable = (LPSS_CLRTBLITEM) g_hColorTable;
      zmemcpy( ClrTblItem, &lpColorTable[ ColorId - 1 ],
               sizeof( SS_CLRTBLITEM ) );
//    DrUnlockTaskMemory( lpColorTable );
      return( ClrTblItem );
   }

   return( 0 );
}

SS_FONTID
SS_GetFontId( HWND          hWnd,
              LPSPREADSHEET lpSS,
              SS_COORD      ssCol,
              SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SS_GetFontId" );
   LPSS_CELL     lpCell;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   SS_FONTID     FontId = 0;

   if ( ssCol != SS_ALLCOLS && ssRow != SS_ALLROWS )
      if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0 )
      {
         if ( lpCell->m_FontId != SS_FONT_NONE )
            FontId = lpCell->m_FontId;

         SS_UnlockCellItem( lpSS, ssCol, ssRow );
      }

   if ( FontId == 0 && ssRow != SS_ALLROWS && ssRow < lpSS->Row.nHeaderCnt &&
        (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
   {
      if ( lpRow->FontId != SS_FONT_NONE )
         FontId = lpRow->FontId;

      SS_UnlockRowItem( lpSS, ssRow );
   }

   if ( FontId == 0 && ssCol != SS_ALLCOLS &&
        (lpCol = SS_LockColItem( lpSS, ssCol )) != 0 )
   {
      if ( lpCol->FontId != SS_FONT_NONE )
         FontId = lpCol->FontId;

      SS_UnlockColItem( lpSS, ssCol );
   }

   if ( FontId == 0 && ssRow != SS_ALLROWS &&
        (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
   {
      if ( lpRow->FontId != SS_FONT_NONE )
         FontId = lpRow->FontId;

      SS_UnlockRowItem( lpSS, ssRow );
   }

   if ( FontId == 0 )
      FontId = lpSS->DefaultFontId;

   return( FontId );
}

zBOOL
SSx_GetLock( HWND          hWnd,
             LPSPREADSHEET lpSS,
             LPSS_COL      lpCol,
             LPSS_ROW      lpRow,
             LPSS_CELL     lpCell,
             SS_COORD      ssCol,
             SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSx_GetLock" );
   if ( lpSS->DocumentProtected == 0 )
      return( FALSE );

   return( SSxx_GetLock( hWnd, lpSS, lpCol, lpRow, lpCell, ssCol, ssRow ) );
}

SS_COORD
SS_GetRightVisCell( HWND          hWnd,
                    LPSPREADSHEET lpSS,
                    SS_COORD      ssColLeft )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetRightVisCell" );
   RECT          rect;
   SS_COORD      ssColAt;
   SS_COORD      MaxCols;
   zSHORT        x;
   zSHORT        cx;

   SS_GetClientRect( hWnd, &rect );
   MaxCols = SS_GetColsMax( lpSS );

   x = SS_GetCellPosX( lpSS, ssColLeft, ssColLeft );
   for ( cx = 0, ssColAt = ssColLeft;
         ssColAt < MaxCols && x + cx <= rect.right - rect.left;
         ssColAt++ )
   {
      x += cx;
      cx = SS_GetColWidth( lpSS, ssColAt );
      if ( x + cx > rect.right - rect.left )
         break;
   }

   ssColAt--;

   if ( ssColAt < ssColLeft )
      ssColAt = -1;

   return( ssColAt );
}

zSHORT
SS_GetRowHeight( LPSPREADSHEET lpSS,
                 SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_GetRowHeight" );
   LPSS_ROW      lpRow;
   zSHORT        nRowHeight;

   nRowHeight = lpSS->Row.nCellSizeInPixels;
   if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
   {
      if ( lpRow->nRowHeightInPixels != SS_WIDTH_DEFAULT )
         nRowHeight = lpRow->nRowHeightInPixels;

      SS_UnlockRowItem( lpSS, ssRow );
   }

   return( nRowHeight );
}

void
SS_HatchRect( HDC               hDC,
              LPSPREADSHEET     lpSS,
              LPRECT            lpRect,
              LPSSX_PRINTFORMAT lpPFormat )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_HatchRect" );
   HBRUSH  hBrush;

   if ( lpPFormat == 0 )
   {
      hBrush = kzCreateSolidBrush( lpSS->Color.GrayAreaBackground );
      UnrealizeObject( hBrush );
//    SetBrushOrg( hDC, 0, 0 );
      FillRect( hDC, lpRect, hBrush );
      DeleteBrush( hBrush );
   }
}

zBOOL
SS_HiliteCell( HWND   hWnd,
               zBOOL  bHilite )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_HiliteCell" );
   LPSPREADSHEET lpSS;
   HDC           hDC;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->FreezeHilite ||
        lpSS->HiliteOn == (unsigned) bHilite ||
        (bHilite && GetFocus( ) != hWnd && lpSS->fRetainSelBlock == 0) ||
        lpSS->EditModeTurningOff || lpSS->Col.Max == 0 ||
        lpSS->Row.Max == 0 || lpSS->wOpMode == SS_OPMODE_READONLY )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

// DrUnlockTaskMemory( lpSS );

   hDC = GetDC( hWnd );
   SSx_HiliteCell( hWnd, hDC, bHilite );

   if ( mIs_hWnd( hWnd ) == FALSE )
      return( FALSE );

   ReleaseDC( hWnd, hDC );

   return( TRUE );
}

zBOOL
SS_InvertBlock( HWND          hWnd,
                HDC           hDC,
                LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_InvertBlock" );
   LPSS_SELBLOCK lpItemList;
   HDC           hDCOrig;
   RECT          rect;
   RECT          rect2;
   RECT          rectClip;
   HRGN          hRgn;
   HRGN          hRgn2;
   HRGN          hRgnClip;
   zSHORT        k;
   int           iRet;

   hDCOrig = hDC;

   if ( lpSS->fSuspendInvert )
      return( FALSE );

   if ( !SS_IsBlockSelected( lpSS ) )
      return( FALSE );

   if ( lpSS->Row.LR == 0 || lpSS->Col.LR == 0 )
      return( FALSE );

   if ( SS_CreateBlockRect( hWnd, lpSS, &rect, lpSS->BlockCellUL.ssCol,
                            lpSS->BlockCellUL.ssRow,
                            lpSS->BlockCellLR.ssCol,
                            lpSS->BlockCellLR.ssRow, FALSE ) )
   {
      if ( hDCOrig == 0 )
         hDC = GetDC( hWnd );

      hRgn = CreateRectRgnIndirect( &rect );

      if ( lpSS->fNoShowActiveWithSel == 0 &&
           SSIsCellVisible( hWnd, lpSS->Col.ssCurrAt,
                            lpSS->Row.ssCurrAt, SS_VISIBLE_PARTIAL ) )
      {
         if ( SS_CreateBlockRect( hWnd, lpSS, &rect2, lpSS->Col.ssCurrAt,
                                  lpSS->Row.ssCurrAt, lpSS->Col.ssCurrAt,
                                  lpSS->Row.ssCurrAt, TRUE ) )
         {
            hRgn2 = CreateRectRgnIndirect( &rect2 );
            CombineRgn( hRgn, hRgn, hRgn2, RGN_XOR );
            DeleteObject( hRgn2 );
         }
      }

      if ( lpSS->fMultipleBlocksSelected )
      {
//       lpItemList = (LPSS_SELBLOCK)
//                         DrLockTaskMemory( lpSS->MultiSelBlock.hItemList );
         lpItemList = (LPSS_SELBLOCK) lpSS->MultiSelBlock.hItemList;
         for ( k = 0; k < lpSS->MultiSelBlock.lItemCnt; k++ )
         {
            if ( SS_CreateBlockRect( hWnd, lpSS, &rect,
                                     lpItemList[ k ].UL.ssCol,
                                     lpItemList[ k ].UL.ssRow,
                                     lpItemList[ k ].LR.ssCol,
                                     lpItemList[ k ].LR.ssRow, FALSE ) )
            {
               hRgn2 = CreateRectRgnIndirect( &rect );
               CombineRgn( hRgn, hRgn, hRgn2, RGN_DIFF );
               DeleteObject( hRgn2 );
            }
         }

//       DrUnlockTaskMemory( lpItemList );
      }

#if 0 // SS_NODRAGDROP
      else if ( lpSS->fAllowDragDrop )
         SS_AddDragDropRectToBlock( hWnd, lpSS, hRgn );
#endif

      if ( (iRet = GetClipBox( hDC, &rectClip )) != ERROR &&
           iRet != NULLREGION )
      {
         hRgnClip = CreateRectRgnIndirect( &rectClip );
         CombineRgn( hRgn, hRgn, hRgnClip, RGN_AND );
         SelectClipRgn( hDC, hRgn );
         InvertRgn( hDC, hRgn );
         SelectClipRgn( hDC, hRgnClip );
         DeleteObject( hRgnClip );
      }
      else
      {
         SelectClipRgn( hDC, hRgn );
         InvertRgn( hDC, hRgn );
         SetRectEmpty( &rectClip );
         hRgnClip = CreateRectRgnIndirect( &rectClip );
         SelectClipRgn( hDC, hRgnClip );
         DeleteObject( hRgnClip );
      }

      DeleteObject( hRgn );

      if ( hDCOrig == 0 )
         ReleaseDC( hWnd, hDC );
   }

   return( TRUE );
}

void
SS_LineTo( LPSPREADSHEET     lpSS,
           HDC               hDC,
           HBRUSH            hBrush,
           HBRUSH            hBrush2,
           HPEN              hPenGrid,
           zLONG             x,
           zLONG             y,
           zCHAR             LineType,
           zBOOL             fGridLines,
           LPSSX_PRINTFORMAT lpPFormat )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_LineTo" );
   HBRUSH  hBrushOld;
   POINT   pt;
   zLONG   x1;
   zLONG   y1;
   zLONG   lOrigX;
   zLONG   lOrigY;
   zLONG   lx;
   zLONG   ly;
   zSHORT  k;

   if ( lpPFormat )
   {
      x = SS_SCREEN2PRINTERX( lpPFormat, x );
      lx = mMax( SS_SCREEN2PRINTERX( lpPFormat, 1 ), 1 );
      y = SS_SCREEN2PRINTERY( lpPFormat, y );
      ly = mMax( SS_SCREEN2PRINTERY( lpPFormat, 1 ), 1 );
   }

   if ( LineType == SS_LINETYPE_NONE ||
        (!fGridLines && (LineType == SS_LINETYPE_DOT ||
                         (lpPFormat && lpPFormat->pf.fDrawBorder == 0))) )
   {
      return;
   }

   lOrigX = x;
   lOrigY = y;

   GetCurrentPositionEx( hDC, &pt );

   x1 = pt.x;
   y1 = pt.y;

   if ( y == y1 )
      x++;
   else
      y++;

   if ( lpPFormat )
   {
      if ( y == y1 )
      {
         for ( k = 0; k < ly; k++ )
         {
            LineTo( hDC, x, y + k );
            MoveToEx( hDC, x1, y1 + k + 1, 0 );
         }

         MoveToEx( hDC, x, y1, 0 );
      }
      else
      {
         for ( k = 0; k < lx; k++ )
         {
            LineTo( hDC, x + k, y );
            MoveToEx( hDC, x1 + k + 1, y1, 0 );
         }

         MoveToEx( hDC, x1, y, 0 );
      }
   }
   else
   if ( LineType == SS_LINETYPE_SOLID )
      LineTo( hDC, x, y );
   else
   {
      if ( hPenGrid )
      {
         HPEN  hPenOld;

         hPenOld = SelectPen( hDC, hPenGrid );
         LineTo( hDC, x, y );
         SelectPen( hDC, hPenOld );
      }
      else
      {
         RECT  rect;

         rect.left = x1;
         rect.top = y1;
         rect.right = lOrigX + 1;
         rect.bottom = lOrigY + 1;

         hBrushOld = SelectBrush( hDC, hBrush2 );
         UnrealizeObject( hBrush );

#ifndef __WIN32__
         SetBrushOrg( hDC, 0, 0 );

         if ( x == x1 )
            if ( (BYTE) (y1 % 2) == lpSS->bVPixelOffset )
               SetBrushOrg( hDC, (x % 2) ? 0 : 1, 0 );
            else
               SetBrushOrg( hDC, (x % 2) ? 0 : 1, 1 );

         if ( y == y1 )
            if ( (BYTE) (x1 % 2) == lpSS->bHPixelOffset )
               SetBrushOrg( hDC, 0, (y % 2) ? 0 : 1 );
            else
               SetBrushOrg( hDC, 1, (y % 2) ? 0 : 1 );
#endif

         SelectBrush( hDC, hBrush );

         PatBlt( hDC, rect.left, rect.top, rect.right - rect.left,
                 rect.bottom - rect.top, PATCOPY );

         SelectBrush( hDC, hBrushOld );
      }
   }
}

void
SS_MoveTo( HDC               hDC,
           zLONG             x,
           zLONG             y,
           LPSSX_PRINTFORMAT lpPFormat )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_MoveTo" );
   if ( lpPFormat )
   {
      x = SS_SCREEN2PRINTERX( lpPFormat, x );
      y = SS_SCREEN2PRINTERY( lpPFormat, y );
   }

   MoveToEx( hDC, x, y, 0 );
}

zBOOL
SS_Paint( HWND  hWnd,
          HDC   hDC )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_Paint" );
   LPSPREADSHEET lpSS;
   PAINTSTRUCT   ps;
   HDC           hDCOrig;
   RECT          rect;
   RECT          rectTemp;
   RECT          rectClient;
   LPSS_ROW      lpRow;
   SS_COORD      ssRowAt;
   SS_COORD      ssRowPosAt;
   SS_COORD      TempRowAt;
   SS_COORD      ssRowHeaderAt;
   SS_COORD      RightVisCell;
   SS_COORD      BottomVisCell;
   zSHORT        nHeight;
   zBOOL         bKillFocusReceived;
   zBOOL         bHiliteOnOld;
   zBOOL         bDrawSelBlock = FALSE;
   HWND          hWndCtrl = 0;

   hDCOrig = hDC;

   if ( (lpSS = SS_Lock( hWnd )) == 0 )
      return( FALSE );

   if ( lpSS->fPaintInProcess )
   {
//    DrUnlockTaskMemory( lpSS );

      if ( hDCOrig == 0 )
      {
         BeginPaint( hWnd, &ps );
         EndPaint( hWnd, &ps );
      }

      return( FALSE );
   }

   if ( lpSS->fUpdateAutoSize )
      SS_AutoSize( hWnd, FALSE );

   lpSS->fPaintInProcess = TRUE;

   // This spreadsheet is always in virtual mode ... so check to see
   // if more data needs to be requested from the application.

   if ( lpSS->fVirtualMode && lpSS->Virtual.VMax != -1 &&
        lpSS->Row.Max && lpSS->fProcessingVQueryData == 0 )
   {
   // TraceLineS( "SS_Draw SS_Paint VIRTUAL mode", "" ); // we do get in here!!!
      if ( lpSS->Row.UL < lpSS->Virtual.VTop ||
           SS_GetBottomCell( hWnd, lpSS, lpSS->Row.UL ) >=
                               lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize )
      {
         SS_VQueryData( hWnd, lpSS,
                        SS_GetBottomCell( hWnd, lpSS, lpSS->Row.UL ) );
//       InvalidateRect( hWnd, &(ps.rcPaint), FALSE );
      // TraceRect( "SS_Paint in here too?", ps.rcPaint );  yup
      }
   }

   GetClientRect( hWnd, &rectTemp );
   SS_GetClientRect( hWnd, &rectClient );
   CopyRect( &rect, &rectClient );

   if ( hDCOrig )
   {
      CopyRect( &(ps.rcPaint), &rectTemp );
      CopyRect( &rect, &rectTemp );
   }
   else
   {
      hDC = BeginPaint( hWnd, &ps );
   }

   if ( lpSS->NoEraseBkgnd || lpSS->NoPaint || !mIs_hWndVisible( hWnd ) )
   {
      lpSS->NoEraseBkgnd = FALSE;
      lpSS->NoPaint = FALSE;
      lpSS->fPaintInProcess = FALSE;
//    DrUnlockTaskMemory( lpSS );

      if ( hDCOrig == 0 )
         EndPaint( hWnd, &ps );

      return( FALSE );
   }

   if ( lpSS->EditModeOn )
   {
      SS_CELLTYPE   CellTypeTemp;
      LPSS_CELLTYPE CellType;

      CellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, 0,
                                      lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      if ( CellType && CellType->nControlID )
      {
         hWndCtrl = SS_GetControlhWnd( lpSS, CellType->nControlID );
         if ( hWndCtrl )
            HideCaret( hWndCtrl );
      }
   }

   if ( GetFocus( ) == hWnd || lpSS->fRetainSelBlock )
      SS_DrawSelBlock( hWnd, hDC, lpSS );

   if ( SS_USESINGLESELBAR( lpSS ) && lpSS->fKillFocusReceived )
      lpSS->fNoDrawFocusRect = TRUE;

   // Remove cell highlight
   lpSS->fNoDrawFocusRect = FALSE;

   if ( SS_USESINGLESELBAR( lpSS ) && GetFocus( ) != hWnd )
      SSx_HiliteCell( hWnd, hDC, FALSE );
   else
      SS_HiliteCell( hWnd, FALSE );

   lpSS->fNoDrawFocusRect = FALSE;

   RightVisCell = SS_GetRightVisCell( hWnd, lpSS, lpSS->Col.UL );
   BottomVisCell = SS_GetBottomVisCell( hWnd, lpSS, lpSS->Row.UL );

   // Erase Background
   SS_EraseBkgnd( hWnd, lpSS, hDC, &rectClient,
                  &(ps.rcPaint), RightVisCell, BottomVisCell );

   lpSS->fInvalidated = FALSE;
   lpSS->fWindowPainted = TRUE;

   bHiliteOnOld = lpSS->HiliteOn;
   SS_ClipClientRect( hWnd, hDC, &rect );

   // SS_HiliteCell( hWnd, FALSE );

   SS_Draw( hWnd, hDC, lpSS, &rect, &(ps.rcPaint),
            lpSS->Col.UL, lpSS->Row.UL,
            RightVisCell, BottomVisCell,
            SS_GetColsMax( lpSS ), SS_GetRowsMax( lpSS ), 0 );

   if ( hDCOrig == 0 )
   {
      if ( lpSS->EditModeOn == 0 &&
           (!SS_USESINGLESELBAR( lpSS ) || GetFocus( ) == hWnd) )
      {
         SS_HiliteCell( hWnd, TRUE );
      }

      if ( mIs_hWnd( hWnd ) == FALSE )
         return( FALSE );

      if ( lpSS->wOpMode == SS_OPMODE_MULTISEL ||
           lpSS->wOpMode == SS_OPMODE_EXTSEL )
      {
         ssRowPosAt = SS_GetCellPosY( lpSS, lpSS->Row.nHeaderCnt,
                                    lpSS->Row.nHeaderCnt );
         ssRowHeaderAt = lpSS->Row.nHeaderCnt;
         ssRowAt = lpSS->Row.UL;

         do
         {
            if ( ssRowHeaderAt < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
               TempRowAt = ssRowHeaderAt;
            else
               TempRowAt = ssRowAt;

            nHeight = SS_GetRowHeight( lpSS, TempRowAt );

            if ( rect.top + ssRowPosAt < ps.rcPaint.bottom &&
                 rect.top + ssRowPosAt + nHeight > ps.rcPaint.top )
            {
               if ( (lpRow = SS_LockRowItem( lpSS, TempRowAt )) != 0 )
               {
                  if ( lpRow->fRowSelected == TRUE || lpRow->fRowSelected == 3 )
                     SS_InvertRowBlock( hWnd, hDC, lpSS, TempRowAt, TempRowAt );

                  SS_UnlockRowItem( lpSS, TempRowAt );
               }
            }

            if ( ssRowHeaderAt < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
               ssRowHeaderAt++;
            else
               ssRowAt++;

            ssRowPosAt += nHeight;

         } while ( ssRowPosAt < rect.bottom &&
                   TempRowAt < SS_GetRowsMax( lpSS ) - 1 );
      }
      else
      if ( GetFocus( ) == hWnd || lpSS->fRetainSelBlock )
         SS_DrawSelBlock( hWnd, hDC, lpSS );
      else
      if ( lpSS->fSelBlockInvisible == 0 )
         bDrawSelBlock = TRUE;

      if ( SS_USESINGLESELBAR( lpSS ) )
      {
         if ( GetFocus( ) != hWnd )
         {
            if ( lpSS->FreezeHilite == 0 )
            {
               bKillFocusReceived = lpSS->fKillFocusReceived;
               SSx_HiliteCell( hWnd, hDC, TRUE );

               if ( bKillFocusReceived )
               {
                  lpSS->fProcessingKillFocus = TRUE;
                  SSx_HiliteCell( hWnd, hDC, FALSE );
                  lpSS->fProcessingKillFocus = FALSE;
               }
            }
         }
         else
            lpSS->fKillFocusReceived = FALSE;
      }
      else
      if ( lpSS->fEditModePermanent == 0 && bHiliteOnOld &&
           GetFocus( ) != hWnd )
      {
         SSx_HiliteCell( hWnd, hDC, TRUE );
      }

      if ( hWndCtrl )
         ShowCaret( hWndCtrl );

      EndPaint( hWnd, &ps );

      if ( bDrawSelBlock )
         SS_DrawSelBlock( hWnd, 0, lpSS );
   }
   else
   {
#ifndef SS_NOCHILDSCROLL
      if ( lpSS->fVScrollVisible && lpSS->fHScrollVisible )
      {
         HBRUSH hBrush;

         SetRect( &rectTemp, rectClient.right, rectClient.bottom,
                  rectClient.right + lpSS->nVScrollWidth,
                  rectClient.bottom + lpSS->nHScrollHeight );
         hBrush = kzCreateSolidBrush( GetSysColor( COLOR_SCROLLBAR ) );
         FillRect( hDC, &rectTemp, hBrush );
         DeleteBrush( hBrush );
      }
#endif
   }

   lpSS->fPaintInProcess = FALSE;
// DrUnlockTaskMemory( lpSS );
   return( 0 );
}

zLONG
SS_RetrieveJust( zLONG lJustification )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_RetrieveJust" );
   zLONG lJust;

   if ( lJustification & SS_TEXT_WORDWRAP )
      lJust = DT_WORDBREAK;
   else
      lJust = DT_SINGLELINE;

   if ( !(lJustification & SS_TEXT_PREFIX) )
      lJust |= DT_NOPREFIX;

   // Determine Horizontal justification.
   if ( lJustification & SS_TEXT_RIGHT )
      lJust |= DT_RIGHT;
   else
   if ( lJustification & SS_TEXT_CENTER )
      lJust |= DT_CENTER;
   else
      lJust |= DT_LEFT;

   // Determine Vertical justification.
   if ( lJustification & SS_TEXT_BOTTOM )
      lJust |= DT_BOTTOM;
   else
   if ( lJustification & SS_TEXT_VCENTER )
      lJust |= DT_VCENTER;
   else
      lJust |= DT_TOP;

   return( lJust );
}

void
SS_ReverseArea( HDC     hDC,
                zLONG   x,
                zLONG   y,
                zLONG   cx,
                zLONG   cy,
                LPRECT  rectBound )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_ReverseArea" );
   RECT   rect;

   rect.left   = mMax( x, rectBound->left );
   rect.right  = mMin( x + cx, rectBound->right );
   rect.top    = mMax( y, rectBound->top );
   rect.bottom = mMin( y + cy, rectBound->bottom );

   if ( rect.left < rect.right && rect.top < rect.bottom )
      InvertRect( hDC, &rect );
}

zSHORT
SS_RowHeightToPixels( LPSPREADSHEET lpSS,
                      SS_COORD      ssRow,
                      zLONG         lRowHeight )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_RowHeightToPixels" );
   LPSS_FONT     Font;
   SS_FONT       FontTemp;
   LPSS_ROW      lpRow;
   zLONG         FontCellHeight;
   zLONG         Height;
   zLONG         dxTemp;
   zSHORT        nFontHeight = -1;

   if ( ssRow != -1 && (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
   {
      Font = SS_GetFont( lpSS, &FontTemp, lpRow->RowMaxFontId );

      if ( Font && Font->nFontHeight != SS_HEIGHT_DEFAULT )
      {
         nFontHeight = Font->nFontHeight;
         FontCellHeight = Font->FontCellHeight;
      }

      SS_UnlockRowItem( lpSS, ssRow );
   }

   if ( nFontHeight == -1 )
   {
      Font = SS_GetFont( lpSS, &FontTemp, lpSS->DefaultFontId );

      nFontHeight = Font->nFontHeight;
      FontCellHeight = Font->FontCellHeight;
   }

   switch ( lpSS->nUnitType )
   {
      case SS_UNITTYPE_NORMAL:
         if ( lRowHeight == SS_HEIGHT_DEFAULT )
            Height = nFontHeight;
         else
            Height = (lRowHeight * nFontHeight) / FontCellHeight;

         break;

      case SS_UNITTYPE_DEFAULT:
         if ( lRowHeight == SS_HEIGHT_DEFAULT )
            Height = nFontHeight;
         else
         {
            dxTemp = lRowHeight * nFontHeight;
            dxTemp /= FontCellHeight;
            dxTemp *= lpSS->nPixelsPerInchY;
            dxTemp /= YPIXELSPERINCH;
            Height = dxTemp;
         }

         break;

      case SS_UNITTYPE_TWIPS:
         if ( lRowHeight == SS_HEIGHT_DEFAULT )
            Height = nFontHeight;
         else
            Height = (lRowHeight * lpSS->nPixelsPerInchY) / 1440;

         break;
   }

   if ( Height > 0 )
   {
#ifdef SS_GP
      if ( Height % 2 == 0 )
         if ( lpSS->nPixelsPerInchY == 72 )
            Height--;
         else
            Height++;
#endif

      Height += lpSS->nBorderSize;
   }

   return( (zSHORT) Height );
}

zBOOL
SS_ShouldButtonBeDrawn( LPSPREADSHEET lpSS,
                        SS_COORD ssCol,
                        SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_ShouldButtonBeDrawn" );
   zBOOL bRC = FALSE;

   if ( lpSS->wOpMode == SS_OPMODE_ROWMODE )
      return( lpSS->fRowModeEditing && ssRow == lpSS->Row.ssCurrAt ? TRUE : FALSE );

   if ( lpSS->wButtonDrawMode == SS_BDM_ALWAYS )
      bRC = TRUE;

   if ( !bRC && (lpSS->wButtonDrawMode & SS_BDM_CURRENTCELL) &&
        lpSS->Col.ssCurrAt == ssCol && lpSS->Row.ssCurrAt == ssRow )
   {
      bRC = TRUE;
   }

   if ( !bRC && (lpSS->wButtonDrawMode & SS_BDM_CURRENTCOL) &&
        lpSS->Col.ssCurrAt == ssCol )
   {
      bRC = TRUE;
   }

   if ( !bRC && (lpSS->wButtonDrawMode & SS_BDM_CURRENTROW) &&
        lpSS->Row.ssCurrAt == ssRow )
   {
      bRC = TRUE;
   }

   return( bRC );
}

void
SS_ShrinkRect( LPRECT  rect,
               zSHORT  x,
               zSHORT  y )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_ShrinkRect" );
   rect->left += x;
   rect->top += y;
   rect->right -= x;
   rect->bottom -= y;
}

void
SSx_DrawText( HDC hDC, LPSSX_PRINTFORMAT lpPFormat, zPCHAR pchData,
              zSHORT nLth, LPRECT lpRect, LPRECT lpRectOrig, zLONG lJust )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SSx_DrawText" );
   TEXTMETRIC tm;
   SIZE       size = { 0, 0 };
   zLONG      x;
   zLONG      y;


   if ( nLth == -1 )
      nLth = zstrlen( pchData );

   if ( (lJust & (DT_CALCRECT | DT_EXPANDTABS | DT_TABSTOP | DT_WORDBREAK)) ||
        !(lJust & DT_NOPREFIX) )
   {
      DrawText( hDC, pchData, nLth, lpRect, lJust );
   }
   else
   {
      // Calculate the X coordinate.
      if ( lJust & (DT_RIGHT | DT_CENTER) )
      {
         GetTextExtentPoint( hDC, pchData, zstrlen( pchData ), &size );

         if ( lJust & DT_RIGHT )
            x = lpRect->right - size.cx;
         else
            x = lpRectOrig->left -
                    ((size.cx - (lpRectOrig->right - lpRectOrig->left)) / 2);
      }
      else
         x = lpRect->left;

      // Calculate the Y coordinate.
      if ( lJust & (DT_BOTTOM | DT_VCENTER) )
      {
         if ( size.cx == 0 && size.cy == 0 )
         {
            GetTextMetrics( hDC, &tm );
            size.cy = tm.tmHeight;
         }

         if ( lJust & DT_BOTTOM )
            y = lpRect->bottom - size.cy;
         else
            y = lpRect->top -
                            ((size.cy - (lpRect->bottom - lpRect->top)) / 2);
      }
      else
         y = lpRect->top;

      ExtTextOut( hDC, x, y, lJust & DT_NOCLIP ? 0 : ETO_CLIPPED, lpRect,
                  pchData, nLth, 0 );
   }
}

zBOOL
SSx_HiliteCell( HWND   hWnd,
                HDC    hDC,
                zBOOL  bHilite )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SSx_HiliteCell" );
   LPSPREADSHEET lpSS;
   zPLONG        lpColWidthList;
   zPLONG        lpRowHeightList;
   RECT          rectBound;
   RECT          rect;
   zBOOL         fNormal;
   zSHORT        nListAt;
   zLONG         x;
   zLONG         y;
   zLONG         cx;
   zLONG         cy;
   SS_COORD      ss;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->Row.Max == 0 || lpSS->wOpMode == SS_OPMODE_READONLY )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( SS_USESELBAR( lpSS ) )
   {
      if ( !SS_IsRowVisible( hWnd, lpSS, lpSS->Row.ssCurrAt, SS_VISIBLE_PARTIAL ) )
      {
//       DrUnlockTaskMemory( lpSS );
         return( FALSE );
      }
   }

#ifdef SS_GP
   else
   if ( !SS_IsCellVisible( hWnd, lpSS, lpSS->Col.ssCurrAt,
                           lpSS->Row.ssCurrAt, SS_VISIBLE_PARTIAL ) )
#else
   else
   if ( lpSS->fEditModePermanent == 0 &&
        !SS_IsCellVisible( hWnd, lpSS, lpSS->Col.ssCurrAt,
                           lpSS->Row.ssCurrAt, SS_VISIBLE_PARTIAL ) )
#endif
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }
   else
   if ( lpSS->fEditModePermanent )
   {
      if ( !SS_IsCellVisible( hWnd, lpSS, lpSS->Col.ssCurrAt,
                              lpSS->Row.ssCurrAt, SS_VISIBLE_ALL ) &&
           (lpSS->Col.ssCurrAt != lpSS->Col.UL ||
            lpSS->Col.UL != lpSS->Col.LR) &&
           (lpSS->Row.ssCurrAt != lpSS->Row.UL ||
            lpSS->Row.UL != lpSS->Row.LR) )
      {
//       DrUnlockTaskMemory( lpSS );
         return( FALSE );
      }
   }

   if ( lpSS->fEditModePermanent && !SS_USESELBAR( lpSS ) )
   {
      if ( lpSS->fAllowEditModePermSel && SS_IsBlockSelected( lpSS ) )
         lpSS->HiliteOn = FALSE;
      else
      if ( bHilite )
      {
         if ( lpSS->EditModeOn == 0 && lpSS->EditModeTurningOn == 0 &&
              lpSS->wMessageBeingSent == 0 && hWnd == GetFocus( ) )
         {
            lpSS->HiliteOn = bHilite;
            SS_CellEditModeOn( hWnd, lpSS, 0, 0, 0L );
         }
      }
      else
         lpSS->HiliteOn = bHilite;

//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   fNormal = lpSS->wOpMode == SS_OPMODE_NORMAL ||
             (lpSS->wOpMode == SS_OPMODE_ROWMODE && lpSS->fRowModeEditing);

   if ( SS_USESINGLESELBAR( lpSS ) && lpSS->fProcessingKillFocus == 0 )
      SS_InvertRow( hWnd, hDC, lpSS );

   if ( SS_USESELBAR( lpSS ) &&
        lpSS->fFocusHiliteOn == (unsigned) bHilite )
   {
   }
   else
   if ( (SS_USESELBAR( lpSS ) || fNormal) && lpSS->fNoDrawFocusRect == 0 )
   {
      if ( SS_USESELBAR( lpSS ) )
         lpSS->fFocusHiliteOn = bHilite;

      SS_GetClientRect( hWnd, &rect );
      CopyRect( &rectBound, &rect );

      // Calculate ssRow position.
      if ( lpSS->Row.hCellSizeList )
      {
//       lpRowHeightList = (zPLONG)
//                               DrLockTaskMemory( lpSS->Row.hCellSizeList );
         lpRowHeightList = (zPLONG) lpSS->Row.hCellSizeList;
         for ( ss = 0, y = 0, nListAt = 0;
               ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen &&
                  ss < lpSS->Row.ssCurrAt;
               ss++, nListAt++ )
         {
            y += lpRowHeightList[ nListAt ];
         }

         for ( ss = mMax( ss, lpSS->Row.UL );
               ss < lpSS->Row.ssCurrAt; ss++, nListAt++ )
         {
            y += lpRowHeightList[ nListAt ];
         }

         cy = lpRowHeightList[ nListAt ];
//       DrUnlockTaskMemory( lpRowHeightList );
      }
      else
      {
         for ( ss = 0, y = 0;
               ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen &&
                 ss < lpSS->Row.ssCurrAt;
               ss++ )
         {
            y += SS_GetRowHeight( lpSS, ss );
         }

         for ( ss = mMax( ss, lpSS->Row.UL ); ss < lpSS->Row.ssCurrAt; ss++ )
            y += SS_GetRowHeight( lpSS, ss );

         cy = SS_GetRowHeight( lpSS, lpSS->Row.ssCurrAt );
      }

      y -= 1;

      // Calculate ssCol position.
      if ( fNormal )
      {
         if ( lpSS->Col.hCellSizeList )
         {
//          lpColWidthList = (zPLONG)
//                               DrLockTaskMemory( lpSS->Col.hCellSizeList );
            lpColWidthList = (zPLONG) lpSS->Col.hCellSizeList;
            for ( ss = 0, x = 0, nListAt = 0;
                  ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen &&
                     ss < lpSS->Col.ssCurrAt;
                  ss++, nListAt++ )
            {
               x += lpColWidthList[ nListAt ];
            }

            for ( ss = mMax( ss, lpSS->Col.UL );
                  ss < lpSS->Col.ssCurrAt;
                  ss++, nListAt++ )
            {
               x += lpColWidthList[ nListAt ];
            }

            cx = lpColWidthList[ nListAt ];

//          DrUnlockTaskMemory( lpColWidthList );
         }
         else
         {
            for ( ss = 0, x = 0;
                  ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen &&
                     ss < lpSS->Col.ssCurrAt;
                  ss++ )
            {
               x += SS_GetColWidth( lpSS, ss );
            }

            for ( ss = mMax( ss, lpSS->Col.UL ); ss < lpSS->Col.ssCurrAt; ss++ )
               x += SS_GetColWidth( lpSS, ss );

            cx = SS_GetColWidth( lpSS, lpSS->Col.ssCurrAt );
         }

         x -= 1;
      }
      else
      {
         x = SS_GetCellPosX( lpSS, lpSS->Col.nHeaderCnt, lpSS->Col.nHeaderCnt );
         cx = mMin( SS_GetCellPosX( lpSS, lpSS->Col.UL, lpSS->Col.LR ) +
                                        SS_GetColWidth( lpSS, lpSS->Col.LR ),
                    rect.right ) - x;
      }

      x += rect.left;
      y += rect.top;

      if ( (lpSS->Row.ssCurrAt == lpSS->Row.UL && lpSS->Row.Frozen == 0) ||
           lpSS->Row.ssCurrAt == lpSS->Row.nHeaderCnt )
      {
         rectBound.top = mMax( rectBound.top, y + 1 );
      }
      else
         rectBound.top = mMax( rectBound.top, y - 1 );

      if ( !fNormal ||
           ((lpSS->Col.ssCurrAt == lpSS->Col.UL && lpSS->Col.Frozen == 0) ||
             lpSS->Col.ssCurrAt == lpSS->Col.nHeaderCnt) )
      {
         rectBound.left = mMax( rectBound.left, x + 1 );
      }
      else
         rectBound.left = mMax( rectBound.left, x - 1 );

      if ( lpSS->Row.ssCurrAt == SS_GetRowsMax( lpSS ) - 1 )
         rectBound.bottom = mMin( rectBound.bottom, y + cy );
      else
         rectBound.bottom = mMin( rectBound.bottom, y + cy + 2 );

      if ( !fNormal || lpSS->Col.ssCurrAt == SS_GetColsMax( lpSS ) - 1 )
         rectBound.right = mMin( rectBound.right, x + cx );
      else
         rectBound.right = mMin( rectBound.right, x + cx + 2 );

      SS_ReverseArea( hDC, x + 2, y - 1, cx - 3, 3, &rectBound );
      SS_ReverseArea( hDC, x - 1, y - 1, 3, cy + 3, &rectBound );
      SS_ReverseArea( hDC, x + 2, y + cy - 1, cx - 3, 3, &rectBound );
      SS_ReverseArea( hDC, x + cx - 1, y - 1, 3, cy + 3, &rectBound );
   }

   if ( lpSS->fProcessingKillFocus == 0 )
      lpSS->HiliteOn = bHilite;

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

zBOOL
SS_InvertMultiSelBlock( HWND          hWnd,
                        HDC           hDC,
                        LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_InvertMultiSelBlock" );
   LPSS_SELBLOCK lpItemList;
   HDC           hDCOrig;
   RECT          rectActive;
   RECT          rect;
   HRGN          hRgn = 0;
   HRGN          hRgn2;
   HRGN          hRgnActive;
   zSHORT        k;
   zBOOL         fActive = FALSE;

   hDCOrig = hDC;

   if ( lpSS->fSuspendInvert )
      return( FALSE );

   if ( !SS_IsBlockSelected( lpSS ) )
      return( FALSE );

   if ( lpSS->Row.LR == 0 || lpSS->Col.LR == 0 )
      return( FALSE );

   if ( hDCOrig == 0 )
      hDC = GetDC( hWnd );

   if ( SS_CreateBlockRect( hWnd, lpSS, &rectActive, lpSS->Col.ssCurrAt,
                            lpSS->Row.ssCurrAt, lpSS->Col.ssCurrAt,
                            lpSS->Row.ssCurrAt, TRUE ) )
   {
      fActive = TRUE;
   }

// lpItemList = (LPSS_SELBLOCK)
//                         DrLockTaskMemory( lpSS->MultiSelBlock.hItemList );
   lpItemList = (LPSS_SELBLOCK) lpSS->MultiSelBlock.hItemList;
   for ( k = 0; k < lpSS->MultiSelBlock.lItemCnt; k++ )
   {
      if ( SS_CreateBlockRect( hWnd, lpSS, &rect, lpItemList[ k ].UL.ssCol,
                               lpItemList[ k ].UL.ssRow, lpItemList[ k ].LR.ssCol,
                               lpItemList[ k ].LR.ssRow, FALSE ) )
      {
         if ( !hRgn )
         {
            hRgn = CreateRectRgnIndirect( &rect );

            if ( lpSS->ModeBlock == 0 &&
                 k == lpSS->MultiSelBlock.lItemCnt - 1 )
            {
               hRgnActive = CreateRectRgnIndirect( &rectActive );
               CombineRgn( hRgn, hRgn, hRgnActive, RGN_XOR );
               DeleteObject( hRgnActive );
            }
         }
         else
         {
            hRgn2 = CreateRectRgnIndirect( &rect );

            if ( lpSS->ModeBlock == 0 &&
                 k == lpSS->MultiSelBlock.lItemCnt - 1 )
            {
               hRgnActive = CreateRectRgnIndirect( &rectActive );
               CombineRgn( hRgn2, hRgn2, hRgnActive, RGN_XOR );
               DeleteObject( hRgnActive );
            }

            CombineRgn( hRgn, hRgn, hRgn2, RGN_OR );
            DeleteObject( hRgn2 );
         }
      }
   }

   if ( lpSS->ModeBlock )
   {
      if ( SS_CreateBlockRect( hWnd, lpSS, &rect, lpSS->BlockCellUL.ssCol,
                               lpSS->BlockCellUL.ssRow, lpSS->BlockCellLR.ssCol,
                               lpSS->BlockCellLR.ssRow, FALSE ) )
      {
         if ( !hRgn )
         {
            hRgn = CreateRectRgnIndirect( &rect );

            if ( lpSS->ModeBlock == 0 &&
                 k == lpSS->MultiSelBlock.lItemCnt - 1 )
            {
               hRgnActive = CreateRectRgnIndirect( &rectActive );
               CombineRgn( hRgn, hRgn, hRgnActive, RGN_XOR );
               DeleteObject( hRgnActive );
            }
         }
         else
         {
            hRgn2 = CreateRectRgnIndirect( &rect );

            if ( lpSS->ModeBlock == 0 &&
                 k == lpSS->MultiSelBlock.lItemCnt - 1 )
            {
               hRgnActive = CreateRectRgnIndirect( &rectActive );
               CombineRgn( hRgn2, hRgn2, hRgnActive, RGN_XOR );
               DeleteObject( hRgnActive );
            }

            CombineRgn( hRgn, hRgn, hRgn2, RGN_OR );
            DeleteObject( hRgn2 );
         }
      }
   }

   if ( hRgn )
   {
      SelectClipRgn( hDC, hRgn );
      InvertRgn( hDC, hRgn );

      DeleteObject( hRgn );
      SelectClipRgn( hDC, 0 );
   }

   if ( !hDCOrig )
      ReleaseDC( hWnd, hDC );

// DrUnlockTaskMemory( lpItemList );

   return( TRUE );
}

zBOOL
SS_InvertRow( HWND          hWnd,
              HDC           hDC,
              LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_InvertRow" );
   return( SS_InvertRowBlock( hWnd, hDC, lpSS,
                              lpSS->Row.ssCurrAt, lpSS->Row.ssCurrAt ) );
}

zBOOL
SS_InvertRowBlock( HWND          hWnd,
                   HDC           hDC,
                   LPSPREADSHEET lpSS,
                   SS_COORD      ssRow,
                   SS_COORD      ssRow2 )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_InvertRowBlock" );
   RECT  rect;
// HRGN  hRgn;

   if ( lpSS->Row.LR == 0 || lpSS->Col.LR == 0 )
      return( FALSE );

   if ( SS_CreateBlockRect( hWnd, lpSS, &rect, 0, ssRow, -1, ssRow2, FALSE ) )
   {
//??? ::DrawFocusRect( hDC, &rect );

   // hRgn = CreateRectRgnIndirect( &rect );
   // SelectClipRgn( hDC, hRgn );
   // InvertRgn( hDC, hRgn );
   // SelectClipRgn( hDC, 0 );
   // DeleteObject( hRgn );
   }

   return( TRUE );
}

void
SS_ScaleRect( LPSSX_PRINTFORMAT lpPFormat, LPRECT  lpRect )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_ScaleRect" );
   lpRect->left = SS_SCREEN2PRINTERX( lpPFormat, lpRect->left );
   lpRect->top = SS_SCREEN2PRINTERY( lpPFormat, lpRect->top );
   lpRect->right = SS_SCREEN2PRINTERX( lpPFormat, lpRect->right );
   lpRect->bottom = SS_SCREEN2PRINTERY( lpPFormat, lpRect->bottom );
}

#ifndef SS_NOCT_CHECK

void
SS_CheckBoxDraw( HWND          hWnd,
                 HWND          hWndCtrl,
                 HDC           hDC,
                 LPRECT        lpRect,
                 LPSS_CELLTYPE lpCellType,
                 HFONT         hFont,
                 zBOOL         fButtonDown,
                 zBOOL         fUseDib,
                 LPRECT        lpRectOrig )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_CheckBoxDraw" );
   LPCHECKBOX    lpCheckBox;
   CHECKBOX      CheckBox;

   if ( hWndCtrl )
      lpCheckBox = CheckBoxGetPtr( hWndCtrl );
   else
      lpCheckBox = 0;

   if ( lpCheckBox == 0 )
      return;

   zmemset( &CheckBox, 0, sizeof( CHECKBOX ) );

   CheckBox.hText          = lpCellType->Spec.CheckBox.hText;
   CheckBox.hBrushFocus    = lpCheckBox->hBrushFocus;
   CheckBox.hBitmapFocus   = lpCheckBox->hBitmapFocus;
   CheckBox.hBitmapDC      = 0;
   CheckBox.hFont          = hFont;
   CheckBox.chPaint        = CHECKBOX_PAINTALL;
   CheckBox.nCtlColorCode  = CTLCOLOR_TBCHECKBOX;
   CheckBox.lStyle         = lpCellType->lStyle;
   CheckBox.fFocus         = FALSE;

   if ( fButtonDown == 2 && (CheckBox.lStyle & 0x0F) == BS_AUTO3STATE )
   {
      CheckBox.fDisabled = TRUE;
      CheckBox.fChecked = TRUE;
   }
   else
   {
      CheckBox.fDisabled = FALSE;
      CheckBox.fChecked = fButtonDown;
   }

// DrUnlockTaskMemory( lpCheckBox );

   if ( CheckBox.fDisabled && lpCellType->Spec.CheckBox.hPictDisabledDownName )
      SS_CheckBoxDrawSetPict( (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                              &CheckBox.Picts,
                              lpCellType->Spec.CheckBox.hPictDisabledDownName,
                              lpCellType->Spec.CheckBox.bPictDisabledDownType |
                                BT_PICT_DISABLEDDOWN );
   else
   if ( fButtonDown && lpCellType->Spec.CheckBox.hPictDownName )
      SS_CheckBoxDrawSetPict( (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                              &CheckBox.Picts,
                              lpCellType->Spec.CheckBox.hPictDownName,
                              lpCellType->Spec.CheckBox.bPictDownType |
                                BT_PICT_DOWN );
   else
   if ( lpCellType->Spec.CheckBox.hPictUpName )
      SS_CheckBoxDrawSetPict( (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                              &CheckBox.Picts,
                              lpCellType->Spec.CheckBox.hPictUpName,
                              lpCellType->Spec.CheckBox.bPictUpType |
                                BT_PICT_UP );

   CheckBoxPaint( hWndCtrl, &CheckBox, hDC, lpRect, fUseDib, lpRectOrig );

#ifdef SS_W
   CheckBoxFreePict( &CheckBox.Picts.Pict );
   CheckBoxFreePict( &CheckBox.Picts.PictBtnDown );
   CheckBoxFreePict( &CheckBox.Picts.PictFocus );
   CheckBoxFreePict( &CheckBox.Picts.PictFocusDown );
   CheckBoxFreePict( &CheckBox.Picts.PictDisabled );
   CheckBoxFreePict( &CheckBox.Picts.PictDisabledDown );
#endif
}

void
SS_CheckBoxDrawSetPict( HINSTANCE  hInst,
                        LPCHECKBOXPICTS lpPicts,
                        zPCHAR     hPictName,
                        zSHORT     nPictType )
{
   TRACE_DEBUG( "<SS_DRAW.CPP>", "SS_CheckBoxDrawSetPict" );
// zPCHAR lpText;

   if ( nPictType & BT_HANDLE )
      CheckBoxSetPict( 0, 0, lpPicts, (WORD) nPictType, (zLONG) hPictName );
   else
   {
//    lpText = (zPCHAR) DrLockTaskMemory( hPictName );
      CheckBoxSetPict( 0, hInst, lpPicts,
                       (WORD) nPictType, (zLONG) hPictName );
//    DrUnlockTaskMemory( lpText );
   }
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_overf.cpp
zBOOL
SSx_CalcCellOverflow( LPSPREADSHEET lpSS, BYTE bOption,
                      SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SSx_CalcCellOverflow" );
   SS_CELLTYPE CellType;
   LPSS_CELL   lpCell;
   zBOOL       bRC = FALSE;

   if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0 )
   {
      if ( lpCell->m_CellData.m_DataType == 0 &&
           (!SS_RetrieveCellType( lpSS, &CellType, lpCell, ssCol, ssRow ) ||
            (CellType.chType != SS_TYPE_PICTURE &&
             CellType.chType != SS_TYPE_BUTTON &&
             CellType.chType != SS_TYPE_COMBOBOX &&
             CellType.chType != SS_TYPE_CHECKBOX &&
             CellType.chType != SS_TYPE_OWNERDRAW)) )
      {
         lpCell->m_CellData.m_bOverflow |= bOption;
         bRC = TRUE;
      }

      SS_UnlockCellItem( lpSS, ssCol, ssRow );
   }
   else
   {
      if ( (lpCell = SS_AllocLockCell( lpSS, 0, ssCol, ssRow )) != 0 )
      {
         lpCell->m_CellData.m_bOverflow |= bOption;
         SS_UnlockCellItem( lpSS, ssCol, ssRow );
         bRC = TRUE;
      }
   }

   return( bRC );
}

zBOOL
SS_CalcCellOverflow( HWND hWnd, LPSPREADSHEET lpSS, LPSS_CELL lpCell,
                     LPSS_CELLTYPE lpCellType, SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_CalcCellOverflow" );
   zPCHAR         hData = 0;
   SS_CELLTYPE    CellType;
   LPSS_COL       lpCol;
   LPSS_ROW       lpRow;
   RECT           rect;
   HDC            hDC;
   zBOOL          bRC = FALSE;
   zLONG          lSize;
   zSHORT         x;
   zLONG          lWidth;
   zLONG          lJust;
   SS_COORD       ss;

   if ( lpSS->fAllowCellOverflow == 0 )
      return( TRUE );

   if ( ssRow < SS_GetRowsMax( lpSS ) && ssCol < SS_GetColsMax( lpSS ) )
   {
      if ( lpCellType == 0 )
      {
         SS_RetrieveCellType( lpSS, &CellType, lpCell, ssCol, ssRow );
         lpCellType = &CellType;
      }

      if ( (lpCellType->chType == SS_TYPE_STATICTEXT &&
           (lpCellType->lStyle & SS_TEXT_WORDWRAP)) ||
           (lpCellType->chType == SS_TYPE_EDIT &&
           (lpCellType->lStyle & ES_MULTILINE)) )
      {
         return( TRUE );
      }

      SS_OverflowInvalidateData( hWnd, lpSS, ssCol, ssRow );

      hDC = GetDC( hWnd );

      lpRow = SS_LockRowItem( lpSS, ssRow );
      lpCol = SS_LockColItem( lpSS, ssCol );

      rect.left = 0;
      rect.top = 0;
      rect.right = SS_GetColWidth( lpSS, ssCol ) - 1;
      rect.bottom = SS_GetRowHeight( lpSS, ssRow ) - 1;

      lSize = SS_CalcCellMetrics( hDC, hWnd, lpSS, lpCol, lpRow, lpCell,
                                  lpCellType, &rect, ssCol, ssRow, TRUE, TRUE,
                                  &lJust, 0 );

      if ( (zSHORT) LOWORD( lSize ) >= rect.right )
      {
         if ( lJust & DT_RIGHT )
         {
            for ( lWidth = rect.right, ss = ssCol - 1;
                  ss >= lpSS->Row.nHeaderCnt + lpSS->Row.Frozen &&
                                           (zSHORT) LOWORD( lSize ) >= lWidth;
                  ss-- )
            {
               if ( !SSx_CalcCellOverflow( lpSS, SS_OVERFLOW_RIGHT, ss, ssRow ) )
                  break;

               lWidth += SS_GetColWidth( lpSS, ss );
            }
         }
         else
         if ( lJust & DT_CENTER )
         {
            x = rect.right + ((LOWORD( lSize ) - rect.right) / 2);

            for ( lWidth = rect.right, ss = ssCol - 1;
                  ss >= lpSS->Row.nHeaderCnt + lpSS->Row.Frozen && x >= lWidth;
                  ss-- )
            {
               if ( !SSx_CalcCellOverflow( lpSS, SS_OVERFLOW_RIGHT, ss, ssRow ) )
                  break;

               lWidth += SS_GetColWidth( lpSS, ss );
            }

            for ( lWidth = rect.right, ss = ssCol + 1;
                  ss <= lpSS->Col.Max && x >= lWidth;
                  ss++ )
            {
               if ( !SSx_CalcCellOverflow( lpSS, SS_OVERFLOW_LEFT, ss, ssRow ) )
                  break;

               lWidth += SS_GetColWidth( lpSS, ss );
            }
         }
         else                             // DT_LEFT
         {
            for ( lWidth = rect.right, ss = ssCol + 1;
                  ss <= lpSS->Col.Max && (zSHORT) LOWORD( lSize ) >= lWidth;
                  ss++ )
            {
               if ( !SSx_CalcCellOverflow( lpSS, SS_OVERFLOW_LEFT, ss, ssRow ) )
                  break;

               lWidth += SS_GetColWidth( lpSS, ss );
            }
         }
      }

      SS_UnlockColItem( lpSS, ssCol );
      SS_UnlockRowItem( lpSS, ssRow );

      bRC = TRUE;
   }

   ReleaseDC( hWnd, hDC );
   return( bRC );
}

zBOOL
SS_CalcOverflowRect( HDC hDC, LPSPREADSHEET lpSS, zPCHAR Data,
                     SS_COORD ssCol, zLONG lJust, LPRECT lpRect,
                     LPRECT  lpCellOverflowRectPrev,
                     LPRECT  lpCellOverflowRectNext, LPRECT  lpRectNew )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_CalcOverflowRect" );
   zBOOL bRC = FALSE;

   if ( lpSS->fAllowCellOverflow == 0 )
      return( FALSE );

   CopyRect( lpRectNew, lpRect );
   if ( lJust & DT_RIGHT )
   {
      if ( lpCellOverflowRectPrev )
      {
         CopyRect( lpRectNew, lpCellOverflowRectPrev );
         bRC = TRUE;
      }
   }
   else
   if ( lJust & DT_CENTER )
   {
      bRC = TRUE;

      if ( lpCellOverflowRectPrev && lpCellOverflowRectNext )
         UnionRect( lpRectNew, lpCellOverflowRectPrev, lpCellOverflowRectNext );
      else
      if ( lpCellOverflowRectPrev )
         CopyRect( lpRectNew, lpCellOverflowRectPrev );
      else
      if ( lpCellOverflowRectNext )
         CopyRect( lpRectNew, lpCellOverflowRectNext );
      else
         bRC = FALSE;
   }
   else
   {
      if ( lpCellOverflowRectNext )
      {
         CopyRect( lpRectNew, lpCellOverflowRectNext );
         bRC = TRUE;
      }
   }

   return( bRC );
}

void
SS_DrawHiddenOverflowCellLeft( HWND hWnd, HDC hDC, LPSPREADSHEET lpSS,
                               LPSS_ROW lpRow, LPRECT  lpRect, SS_COORD ssCol,
                               SS_COORD ssRow, LPSSX_PRINTFORMAT lpPFormat,
                               LPRECT  lpRectUpdate )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_DrawHiddenOverflowCellLeft" );
   LPSS_CELL lpCellTemp;
   LPSS_COL  lpCol;
   //RECT      rectOverflowPrev;
   RECT      rectOverflow;
   RECT      rectTemp;
   zBOOL     fOverflow;
   zSHORT    nWidth;
   zSHORT    nWidthTotal;
   SS_COORD  ss;

   SaveDC( hDC );

   IntersectClipRect( hDC, lpRect->left, lpRect->top, lpRect->right,
                      lpRect->bottom );

   for ( ss = ssCol - 1, nWidth = 0, nWidthTotal = 0, fOverflow = TRUE;
         fOverflow && ss >= lpSS->Col.nHeaderCnt; ss-- )
   {
      nWidth = SS_GetColWidth( lpSS, ss );
      nWidthTotal += nWidth;
      ssCol--;

      fOverflow = FALSE;

      if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow, ss, ssRow )) != 0 )
      {
         if ( lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_LEFT )
            fOverflow = TRUE;

         SS_UnlockCellItem( lpSS, ss, ssRow );
      }
   }

   CopyRect( &rectOverflow, lpRect );
   rectOverflow.left -= nWidthTotal;
   CopyRect( &rectTemp, &rectOverflow );
   rectTemp.right = rectTemp.left + nWidth - 1;

   lpCol = SS_LockColItem( lpSS, ssCol );
   SS_DrawText( hDC, hWnd, lpSS, lpCol, lpRow, &rectTemp, ssCol, ssRow, FALSE,
                lpPFormat, lpRectUpdate, 0, &rectOverflow, FALSE );
   SS_UnlockColItem( lpSS, ssCol );

   RestoreDC( hDC, -1 );
}

void
SS_DrawHiddenOverflowCellRight( HWND hWnd, HDC hDC, LPSPREADSHEET lpSS,
                                LPSS_ROW lpRow, LPRECT  lpRect,
                                SS_COORD ssCol, SS_COORD ssRow,
                                LPSSX_PRINTFORMAT lpPFormat,
                                LPRECT  lpRectUpdate )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_DrawHiddenOverflowCellRight" );
   LPSS_CELL lpCellTemp;
   LPSS_COL  lpCol;
   RECT      rectOverflow;
   RECT      rectTemp;
   zBOOL     fOverflow;
   zSHORT    nWidth;
   zSHORT    nWidthTotal;
   SS_COORD  ssColOrig = ssCol;
   SS_COORD  ss;

   SaveDC( hDC );

   IntersectClipRect( hDC, lpRect->left, lpRect->top,
                      lpRect->right + 1, lpRect->bottom );

   for ( ss = ssCol + 1, nWidth = 0, nWidthTotal = 0, fOverflow = TRUE;
         fOverflow && ss <= lpSS->Col.Max; ss++ )
   {
      nWidth = SS_GetColWidth( lpSS, ss );
      nWidthTotal += nWidth;
      ssCol++;

      fOverflow = FALSE;

      if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow, ss, ssRow )) != 0 )
      {
         if ( lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT )
            fOverflow = TRUE;

         SS_UnlockCellItem( lpSS, ss, ssRow );
      }
   }

   CopyRect( &rectOverflow, lpRect );
   rectOverflow.right += nWidthTotal;
   CopyRect( &rectTemp, &rectOverflow );
   rectTemp.left = rectTemp.right - nWidth + 1;

   fOverflow = FALSE;

   if ( ssColOrig - 1 >= lpSS->Col.nHeaderCnt )
   {
      if ( (lpCellTemp = SSx_LockCellItem( lpSS, lpRow, ssColOrig - 1, ssRow )) != 0 )
      {
         if ( lpCellTemp->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT )
            fOverflow = TRUE;

         SS_UnlockCellItem( lpSS, ssColOrig - 1, ssRow );
      }
   }

   lpCol = SS_LockColItem( lpSS, ssCol );
   SS_DrawText( hDC, hWnd, lpSS, lpCol, lpRow, &rectTemp, ssCol, ssRow, FALSE,
                lpPFormat, lpRectUpdate, &rectOverflow, 0, fOverflow );
   SS_UnlockColItem( lpSS, ssCol );

   RestoreDC( hDC, -1 );
}

zBOOL
SS_GetOverflowCoords( HWND hWnd, LPSPREADSHEET lpSS,
                      SS_COORD ssCol, SS_COORD ssRow,
                      zPLONG lpnWidthPrev,
                      zPLONG lpnWidthNext )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_GetOverflowCoords" );
   LPSS_CELL lpCell;
   zBOOL     fFound;
   zBOOL     bRC = FALSE;
   SS_COORD  k;

   *lpnWidthPrev = 0;
   *lpnWidthNext = 0;

   if ( ssRow != -1 && ssRow < SS_GetRowsMax( lpSS ) &&
        ssCol != -1 && ssCol < SS_GetColsMax( lpSS ) )
   {
      for ( k = ssCol - 1, fFound = TRUE;
            fFound && k >= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;
            k-- )
      {
         fFound = FALSE;
         if ( (lpCell = SS_LockCellItem( lpSS, k, ssRow )) != 0 )
         {
            if ( lpCell->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT )
            {
               *lpnWidthPrev += SS_GetColWidth( lpSS, k );
               fFound = TRUE;
            }

            SS_UnlockCellItem( lpSS, k, ssRow );
         }
      }

      for ( k = ssCol + 1, fFound = TRUE; fFound && k <= lpSS->Col.Max; k++ )
      {
         fFound = FALSE;
         if ( (lpCell = SS_LockCellItem( lpSS, k, ssRow )) != 0 )
         {
            if ( lpCell->m_CellData.m_bOverflow & SS_OVERFLOW_LEFT )
            {
               *lpnWidthNext += SS_GetColWidth( lpSS, k );
               fFound = TRUE;
            }

            SS_UnlockCellItem( lpSS, k, ssRow );
         }
      }
   }

   if ( *lpnWidthPrev || *lpnWidthNext )
      bRC = TRUE;

   return( bRC );
}

WORD
SS_GetOverflowRects( HWND hWnd, LPSPREADSHEET lpSS, SS_COORD ssCol,
                     SS_COORD ssRow, LPRECT  lprectClient, LPRECT  lpRect,
                     LPRECT  lpRectOverflowPrev,
                     LPRECT  lpRectOverflowNext, zBOOL *lpfOverflowToLeft,
                     LPRECT  lpRectOverflowAll )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_GetOverflowRects" );
   WORD   wRet = 0;
   zLONG lWidthPrev = 0;
   zLONG lWidthNext = 0;

   if ( lpfOverflowToLeft )
      *lpfOverflowToLeft = FALSE;

   if ( SS_GetOverflowCoords( hWnd, lpSS, ssCol, ssRow, &lWidthPrev, &lWidthNext ) )
   {
      if ( lpRectOverflowAll )
         CopyRect( lpRectOverflowAll, lpRect );

      if ( lWidthPrev )
      {
         if ( lpRectOverflowPrev )
            SetRect( lpRectOverflowPrev,
                     mMax( lprectClient->left, lpRect->left - lWidthPrev ),
                     lpRect->top + 1, lpRect->left,
                     mMin( lprectClient->right, lpRect->bottom ) );

         if ( lpfOverflowToLeft )
            if ( lprectClient->left > lpRect->left - lWidthPrev )
               *lpfOverflowToLeft = TRUE;

         if ( lpRectOverflowAll )
            lpRectOverflowAll->left = mMax( lprectClient->left,
                                           lpRect->left - lWidthPrev );
      }

      if ( lWidthNext )
      {
         if ( lpRectOverflowNext )
            SetRect( lpRectOverflowNext, lpRect->right, lpRect->top,
                     mMin( lprectClient->right, lpRect->right + lWidthNext ),
                     lpRect->right );

         if ( lpRectOverflowAll )
            lpRectOverflowAll->right = mMin( lprectClient->right,
                                             lpRect->right + lWidthNext );
      }

      lWidthPrev = lWidthPrev ? 1 : 0;
      lWidthNext = lWidthNext ? 1 : 0;
      wRet = MAKEWORD( (char) lWidthPrev, (char) lWidthNext );
   }

   return( wRet );
}

void
SS_OverflowAdjustNeighbor( HWND hWnd, LPSPREADSHEET lpSS,
                           SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_OverflowAdjustNeighbor" );
   SS_CELLTYPE CellType;
   LPSS_CELL   lpCellCurrent;
   LPSS_CELL   lpCell;
   zBOOL       fFound;
   zBOOL       fDone = FALSE;
   SS_COORD    ss;

   if ( lpSS->fAllowCellOverflow )
   {
      if ( (lpCellCurrent = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0 )
      {
         if ( lpCellCurrent->m_CellData.m_DataType == 0 ||
              (lpCellCurrent->m_CellData.m_bOverflow & SS_OVERFLOW_LEFT) )
         {
            for ( ss = ssCol - 1, fFound = TRUE;
                  fFound && ss >= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;
                  ss-- )
            {
               fFound = FALSE;
               if ( (lpCell = SS_LockCellItem( lpSS, ss, ssRow )) != 0 )
               {
                  if ( lpCell->m_CellData.m_bOverflow & SS_OVERFLOW_LEFT )
                     fFound = TRUE;
                  else
                  if ( lpCell->m_CellData.m_DataType )
                  {
                     if ( SS_RetrieveCellType( lpSS, &CellType,
                                               lpCell, ss, ssRow ) )
                     {
                        SS_CalcCellOverflow( hWnd, lpSS,
                                             lpCell, &CellType, ss, ssRow );
                        SS_OverflowInvalidateCell( hWnd, lpSS, ss, ssRow );
                        fDone = TRUE;
                     }
                  }

                  SS_UnlockCellItem( lpSS, ss, ssRow );
               }
            }
         }

         if ( !fDone && (lpCellCurrent->m_CellData.m_DataType == 0 ||
              (lpCellCurrent->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT)) )
         {
            for ( ss = ssCol + 1, fFound = TRUE;
                  fFound && ss <= lpSS->Col.Max;
                  ss++ )
            {
               fFound = FALSE;

               if ( (lpCell = SS_LockCellItem( lpSS, ss, ssRow )) != 0 )
               {
                  if ( lpCell->m_CellData.m_bOverflow & SS_OVERFLOW_RIGHT )
                     fFound = TRUE;
                  else
                  if ( lpCell->m_CellData.m_DataType )
                  {
                     if ( SS_RetrieveCellType( lpSS, &CellType,
                                               lpCell, ss, ssRow ) )
                     {
                        SS_CalcCellOverflow( hWnd, lpSS, lpCell,
                                             &CellType, ss, ssRow );
                        SS_OverflowInvalidateCell( hWnd, lpSS, ss, ssRow );
                        fDone = TRUE;
                     }
                  }

                  SS_UnlockCellItem( lpSS, ss, ssRow );
               }
            }
         }

         if ( !fDone && lpCellCurrent->m_CellData.m_DataType )
         {
            if ( SS_RetrieveCellType( lpSS, &CellType, lpCellCurrent,
                                      ssCol, ssRow ) )
            {
               SS_CalcCellOverflow( hWnd, lpSS, lpCellCurrent, &CellType,
                                    ssCol, ssRow );
               SS_OverflowInvalidateCell( hWnd, lpSS, ssCol, ssRow );
            }
         }

         SS_UnlockCellItem( lpSS, ssCol, ssRow );
      }
   }
}

zBOOL
SS_OverflowInvalidateCell( HWND hWnd, LPSPREADSHEET lpSS,
                           SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_OverflowInvalidateCell" );
   RECT   rect;
   RECT   rectOverlap;
   zBOOL  bRC = FALSE;
   zLONG lWidthPrev = 0;
   zLONG lWidthNext = 0;
   zLONG x;
   zLONG y;
   zLONG cx;
   zLONG cy;

   if ( SS_GetOverflowCoords( hWnd, lpSS, ssCol, ssRow,
                              &lWidthPrev, &lWidthNext ) )
   {
      SS_GetClientRect( hWnd, &rect );
      SS_GetCellCoord( lpSS, lpSS->Col.UL, lpSS->Row.UL, ssCol, ssRow,
                       &x, &y, &cx, &cy );

      SetRect( &rectOverlap, x, y + 1, x + cx, y + cy );

      if ( lWidthPrev )
         rectOverlap.left = mMax( rect.left, rectOverlap.left - lWidthPrev );

      if ( lWidthNext )
         rectOverlap.right = mMin( rect.right, rectOverlap.right + lWidthNext );

      InvalidateRect( hWnd, &rectOverlap, TRUE );
      bRC = TRUE;
   }

   return( bRC );
}

zBOOL
SS_OverflowInvalidateData( HWND hWnd, LPSPREADSHEET lpSS,
                           SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_OverflowInvalidateData" );
   LPSS_CELL lpCell;
   RECT      rectTemp;
   RECT      rectOverlap;
   zLONG     lWidthPrev;
   zLONG     lWidthNext;
   zLONG     x;
   zLONG     y;
   zLONG     cx;
   zLONG     cy;
   zBOOL     fInvalidateCurrentCell = FALSE;
   zBOOL     bRC = FALSE;

   if ( lpSS->fAllowCellOverflow )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0 )
      {
         if ( lpCell->m_CellData.m_bOverflow )
            fInvalidateCurrentCell = TRUE;

         SS_UnlockCellItem( lpSS, k, ssRow );
      }

      if ( SSx_ResetCellOverflow( hWnd, lpSS, ssCol, ssRow,
                                  &lWidthPrev, &lWidthNext ) )
      {
         if ( lWidthPrev || lWidthNext )
         {
            SS_GetScrollArea( hWnd, lpSS, &rectTemp, SS_SCROLLAREA_CELLS );

            SS_GetCellCoord( lpSS, lpSS->Col.UL, lpSS->Row.UL,
                             ssCol, ssRow, &x, &y, &cx, &cy );

            if ( lWidthPrev )
            {
               SetRect( &rectOverlap, mMax( rectTemp.left, x - lWidthPrev ),
                        y, mMin( rectTemp.right, x ), y + cy );
               InvalidateRect( hWnd, &rectOverlap, TRUE );
            }

            if ( lWidthNext )
            {
               SetRect( &rectOverlap, x + cx - 1, y,
                        mMin( rectTemp.right, x + cx + lWidthNext ), y + cy );
               InvalidateRect( hWnd, &rectOverlap, TRUE );
            }

            if ( fInvalidateCurrentCell )
            {
               SetRect( &rectOverlap, x, y + 1, x + cx, y + cy );
               InvalidateRect( hWnd, &rectOverlap, TRUE );
            }
         }

         bRC = TRUE;
      }
   }

   return( bRC );
}

zBOOL
SSxx_ResetCellOverflow( LPSPREADSHEET lpSS, BYTE bOption,
                        SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SSxx_ResetCellOverflow" );
   LPSS_CELL lpCell;
   zBOOL     bRC = FALSE;

   if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0 )
   {
      if ( lpCell->m_CellData.m_bOverflow & bOption )
      {
         lpCell->m_CellData.m_bOverflow &= ~bOption;
         bRC = TRUE;
      }

      SS_UnlockCellItem( lpSS, ssCol, ssRow );
   }

   return( bRC );
}

zBOOL
SSx_ResetCellOverflow( HWND hWnd, LPSPREADSHEET lpSS,
                       SS_COORD ssCol,
                       SS_COORD ssRow,
                       zPLONG lpWidthPrev,
                       zPLONG lpWidthNext )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SSx_ResetCellOverflow" );
   zBOOL    bRC = FALSE;
   SS_COORD k;

   if ( lpSS->fAllowCellOverflow == 0 )
      return( TRUE );

   if ( lpWidthPrev )
      *lpWidthPrev = 0;

   if ( lpWidthNext )
      *lpWidthNext = 0;

   if ( ssRow != -1 && ssRow < SS_GetRowsMax( lpSS ) && ssCol != -1 &&
        ssCol < SS_GetColsMax( lpSS ) )
   {
      for ( k = ssCol - 1; k >= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen; k-- )
      {
         if ( !SSxx_ResetCellOverflow( lpSS, SS_OVERFLOW_RIGHT, k, ssRow ) )
            break;
         else
         if ( lpWidthPrev )
            *lpWidthPrev += SS_GetColWidth( lpSS, k );
      }

      for ( k = ssCol + 1; k <= lpSS->Col.Max; k++ )
      {
         if ( !SSxx_ResetCellOverflow( lpSS, SS_OVERFLOW_LEFT, k, ssRow ) )
            break;
         else
         if ( lpWidthNext )
            *lpWidthNext += SS_GetColWidth( lpSS, k );
      }

      bRC = TRUE;
   }

   return( bRC );
}

zBOOL
SS_ResetCellOverflow( HWND hWnd, LPSPREADSHEET lpSS,
                      SS_COORD ssCol, SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_OVERF.CPP>", "SS_ResetCellOverflow" );
   return( SS_OverflowInvalidateData( hWnd, lpSS, ssCol, ssRow ) );
}

#endif
