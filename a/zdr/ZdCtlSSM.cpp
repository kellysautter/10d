/////////////////////////////////////////////////////////////////////////////
// Project ZdSS
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlssm.cpp
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
// 1999.09.22    DKS    Z10    QS999
//    Fixed text color problem and started memory leak analyis.
//
// 1998.12.22    DKS
//    Hi-bit not set fix for Bianca.
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

void
TraceThisRect( zPCHAR  pchMessage,
               LPRECT  lpRect )
{
   zCHAR szBuffer[ 256 ];
   zSHORT nLth;

   if ( lpRect == 0 )
      return;

   nLth = zstrlen( pchMessage );
   if ( nLth < zsizeof( szBuffer ) )
      strcpy_s( szBuffer, zsizeof( szBuffer ), pchMessage );
   else
      nLth = 0;

   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " X=" );
   _ltoa_s( lpRect->left, szBuffer + nLth + 3 );
   nLth = zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " Y=" );
   _ltoa_s( lpRect->top, szBuffer + nLth + 3 );
   nLth = zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " W=" );
   _ltoa_s( lpRect->right - lpRect->left, szBuffer + nLth + 3 );
   nLth = zstrlen( szBuffer );
   strcpy_s( szBuffer + nLth, zsizeof( szBuffer ) - nLth, " H=" );
// _ltoa_s( lpRect->bottom - lpRect->top, szBuffer + nLth + 3 );
   TraceLineI( szBuffer, lpRect->bottom - lpRect->top );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_multi.cpp

zBOOL
SS_BeginMultipleSelection( HWND hWnd,
                           LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_BeginMultipleSelection" );
   SS_CELLCOORD BlockCellUL;
   SS_CELLCOORD BlockCellLR;

   if ( lpSS->fMultipleBlocksSelected == 0 )
   {
      if ( SS_IsBlockSelected( lpSS ) )
      {
         if ( !SS_MultiSelBlockAddItem( lpSS, &lpSS->BlockCellUL,
                                        &lpSS->BlockCellLR ) )
         {
            return( FALSE );
         }
      }
      else
      {
         BlockCellUL.ssCol = lpSS->Col.ssCurrAt;
         BlockCellUL.ssRow = lpSS->Row.ssCurrAt;
         BlockCellLR.ssCol = lpSS->Col.ssCurrAt;
         BlockCellLR.ssRow = lpSS->Row.ssCurrAt;

         if ( !SS_MultiSelBlockAddItem( lpSS, &BlockCellUL, &BlockCellLR ) )
            return( FALSE );
      }

      SS_MultiSelCompleteBlock( hWnd, lpSS );
      lpSS->fMultipleBlocksSelected = TRUE;
   }
   else
      SS_MultiSelCompleteBlock( hWnd, lpSS );

   return( TRUE );
}

zBOOL
SS_IntersectSelBlock( LPSS_SELBLOCK lpSelBlock1,
                      LPSS_SELBLOCK lpSelBlock2 )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_IntersectSelBlock" );
   if ( lpSelBlock1->LR.ssRow >= lpSelBlock2->UL.ssRow &&
        lpSelBlock1->UL.ssRow <= lpSelBlock2->LR.ssRow &&
        lpSelBlock1->LR.ssCol >= lpSelBlock2->UL.ssCol &&
        lpSelBlock1->UL.ssCol <= lpSelBlock2->LR.ssCol )
   {
      return( TRUE );
   }

   return( FALSE );
}

zPCHAR
SS_ListAddItem( zPCHAR  hList,
                zPLONG lpnItemCnt,
                zPLONG lpnItemAllocCnt,
                zPVOID  lpItem,
                zSHORT  nItemLth,
                zSHORT  nAllocInc )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_ListAddItem" );
   zPCHAR lpList;

   if ( *lpnItemCnt + 1 > *lpnItemAllocCnt )
   {
      if ( *lpnItemAllocCnt )
         DrReAllocTaskMemory( &hList,
                  (zLONG) (nItemLth * (*lpnItemAllocCnt + nAllocInc)) );
      else
         DrAllocTaskMemory( &hList,
                  (zLONG) (nItemLth * (*lpnItemAllocCnt + nAllocInc)), 1030 );

      if ( hList == 0 )
         return( 0 );

      *lpnItemAllocCnt += SS_MULTISEL_ALLOC_CNT;
   }

   lpList = (zPCHAR) DrLockTaskMemory( hList );

   zmemcpy( &lpList[ *lpnItemCnt * nItemLth ], lpItem, nItemLth );
   (*lpnItemCnt)++;

// DrUnlockTaskMemory( lpList );
   return( lpList );
}

void
SS_ListDelItem( zPCHAR  hList,
                zLONG   lItemNbr,
                zPLONG  lpnItemCnt,
                zSHORT  nItemLth )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_ListDelItem" );
   zPCHAR lpList;

   if ( hList && *lpnItemCnt )
   {
//    lpList = (zPCHAR) DrLockTaskMemory( hList );
      lpList = (zPCHAR) hList;

      zmemcpy( &lpList[ lItemNbr * nItemLth ],
               &lpList[ (lItemNbr + 1) * nItemLth ],
               nItemLth *(*lpnItemCnt - (lItemNbr + 1)) );
      (*lpnItemCnt)--;
//    DrUnlockTaskMemory( lpList );
   }
}

zPCHAR
SS_ListDup( zPCHAR  hList,
            zLONG   lItemCnt,
            zSHORT  nItemSize )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_ListDup" );
   zPCHAR        hListNew = 0;
// LPVOID        lpList;
   LPVOID        lpListNew;

   if ( hList )
   {
      if ( DrAllocTaskMemory( &hListNew,
                              (zLONG) (nItemSize * lItemCnt), 1031 ) == 0 )
      {
//       lpList = (LPVOID) DrLockTaskMemory( hList );
         lpListNew = (LPVOID) DrLockTaskMemory( hListNew );

         zmemcpy( lpListNew, hList, nItemSize * lItemCnt );

//       DrUnlockTaskMemory( lpList );
//       DrUnlockTaskMemory( lpListNew );
      }
   }

   return( (zPCHAR) lpListNew );
}

zBOOL
SS_MultiSelBlockAddItem( LPSPREADSHEET lpSS,
                         LPSS_CELLCOORD lpUL,
                         LPSS_CELLCOORD lpLR )
{
#if 0

   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_MultiSelBlockAddItem" );
   LPSS_SELBLOCK lpItemList;

   if ( lpSS->MultiSelBlock.nItemCnt + 1 >
       lpSS->MultiSelBlock.nItemAllocCnt )
   {
      if ( lpSS->MultiSelBlock.nItemAllocCnt == 0 )
         lpSS->MultiSelBlock.hItemList = GlobalAlloc( GMEM_MOVEABLE |
                                                      GMEM_ZEROINIT,
                                          (zLONG) (zsizeof( SS_SELBLOCK ) *
                                           (lpSS->MultiSelBlock.nItemAllocCnt +
                                            SS_MULTISEL_ALLOC_CNT)) );
      else
         lpSS->MultiSelBlock.hItemList = GlobalReAlloc(
                                          lpSS->MultiSelBlock.hItemList,
                                          (zLONG) (zsizeof( SS_SELBLOCK ) *
                                           (lpSS->MultiSelBlock.nItemAllocCnt +
                                            SS_MULTISEL_ALLOC_CNT)),
                                          GMEM_MOVEABLE | GMEM_ZEROINIT );

      if ( lpSS->MultiSelBlock.hItemList == 0 )
         return( FALSE );

      lpSS->MultiSelBlock.nItemAllocCnt += SS_MULTISEL_ALLOC_CNT;
   }

   lpItemList = (LPSS_SELBLOCK) GlobalLock( lpSS->MultiSelBlock.hItemList );

   zmemcpy( &lpItemList[ lpSS->MultiSelBlock.nItemCnt ].UL, lpUL,
            zsizeof( SS_CELLCOORD ) );
   zmemcpy( &lpItemList[ lpSS->MultiSelBlock.nItemCnt ].LR, lpLR,
            zsizeof( SS_CELLCOORD ) );
   lpSS->MultiSelBlock.nItemCnt++;

   GlobalUnlock( lpSS->MultiSelBlock.hItemList );
   return( TRUE );

#else

   MESSAGE_DEBUG( "<SS_MULTI.CPP>", "SS_MultiSelBlockAddItem" );
   return( FALSE );

#endif
}

void
SS_MultiSelCompleteBlock( HWND          hWnd,
                          LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_MultiSelCompleteBlock" );
   HDC           hDC;
   RECT          rect;
   HRGN          hRgn;

   if ( !SS_CreateBlockRect( hWnd, lpSS, &rect, lpSS->Col.ssCurrAt,
                             lpSS->Row.ssCurrAt, lpSS->Col.ssCurrAt,
                             lpSS->Row.ssCurrAt, FALSE ) )
   {
      SetRectEmpty( &rect );
   }

   hRgn = CreateRectRgnIndirect( &rect );

#ifndef SS_NODRAGDROP
{
   HRGN hRgn2;

   if ( lpSS->fAllowDragDrop && lpSS->fMultipleBlocksSelected == 0 )
   {
      if ( (hRgn2 = SS_CreateDragDropRect( hWnd, lpSS )) != 0 )
      {
         CombineRgn( hRgn, hRgn, hRgn2, zRGN_XOR );
         zDeleteObjectRegion( hRgn2 );
      }
   }
}
#endif

   hRgn = SSxx_SelectBlock( hWnd, lpSS, hRgn, &rect,
                            lpSS->MultiSelBlock.lItemCnt - 1 );

   hDC = GetDC( hWnd );
   SelectClipRgn( hDC, hRgn );
   InvertRgn( hDC, hRgn );
   DeleteObject( hRgn );
   SelectClipRgn( hDC, 0 );

   ReleaseDC( hWnd, hDC );
}

zLONG OPERATION
SSSelModeSendMessage( HWND hWnd, WORD  wMsg,
                      zLONG lParam1, zLONG lParam2, zLONG lParam3 )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSSelModeSendMessage" );
   LPSPREADSHEET lpSS;
   LPSS_COORD    lpBuffer;
   LPSS_ROW      lpRow;
   zLONG         lRet = -1;
   SS_COORD      Sel1;
   SS_COORD      Sel2;
   SS_COORD      dCnt;
   SS_COORD      ss;
   zBOOL         RedrawOld;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->wOpMode == SS_OPMODE_SINGLESEL ||
        lpSS->wOpMode == SS_OPMODE_MULTISEL ||
        lpSS->wOpMode == SS_OPMODE_EXTSEL )
   {
      switch ( wMsg )
      {
         case SS_SELMODE_CLEARSEL:
            if ( lpSS->wOpMode != SS_OPMODE_SINGLESEL )
            {
               RedrawOld = lpSS->Redraw;
               lpSS->Redraw = FALSE;

               for ( ss = lpSS->Row.nHeaderCnt; ss < lpSS->Row.AllocCnt; ss++ )
               {
                  if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
                  {
                     if ( lpRow->fRowSelected )
                     {
                        lpRow->fRowSelected = FALSE;
                        SS_InvalidateRow( hWnd, lpSS, ss );
                     }

                     SS_UnlockRowItem( lpSS, ss );
                  }
               }

               lpSS->MultiSelCnt = 0;
               SSSetBool( hWnd, SSB_REDRAW, RedrawOld );
               lRet = TRUE;
            }

            break;

         case SS_SELMODE_GETCURSEL:
            if ( lpSS->wOpMode == SS_OPMODE_SINGLESEL )
               lRet = lpSS->Row.ssCurrAt;

            break;

         case SS_SELMODE_GETSEL:
            if ( lpSS->wOpMode == SS_OPMODE_SINGLESEL )
            {
               if ( lpSS->Row.ssCurrAt == lParam1 )
                  lRet = TRUE;
               else
                  lRet = FALSE;
            }
            else
            {
               lRet = FALSE;

               if ( (lpRow = SS_LockRowItem( lpSS, (SS_COORD) lParam1 )) != 0 )
                  lRet = lpRow->fRowSelected;
            }

            break;

         case SS_SELMODE_GETSELCOUNT:
            if ( lpSS->wOpMode != SS_OPMODE_SINGLESEL )
               lRet = lpSS->MultiSelCnt;

            break;

         case SS_SELMODE_GETSELITEMS:
            if ( lpSS->wOpMode != SS_OPMODE_SINGLESEL )
            {
               lpBuffer = (LPSS_COORD) lParam2;

               for ( ss = 1, dCnt = 0;
                     ss < lpSS->Row.AllocCnt && dCnt < lParam1;
                     ss++ )
               {
                  if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
                  {
                     if ( lpRow->fRowSelected )
                        lpBuffer[ dCnt++ ] = ss;

                     SS_UnlockRowItem( lpSS, ss );
                  }
               }

               lRet = (zLONG) dCnt;
            }

            break;

         case SS_SELMODE_GETSELITEM:
            if ( lpSS->wOpMode != SS_OPMODE_SINGLESEL )
            {
               for ( ss = mMax( (SS_COORD) lParam1 + 1, 1 ); lRet == -1 &&
                     ss < lpSS->Row.AllocCnt; ss++ )
               {
                  if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
                  {
                     if ( lpRow->fRowSelected )
                        lRet = ss;

                     SS_UnlockRowItem( lpSS, ss );
                  }
               }
            }

            break;

         case SS_SELMODE_SELITEMRANGE:
            if ( lpSS->wOpMode != SS_OPMODE_SINGLESEL )
            {
               Sel1 = (SS_COORD) lParam2;
               Sel2 = (SS_COORD) lParam3;

               RedrawOld = lpSS->Redraw;
               lpSS->Redraw = FALSE;

               for ( ss = Sel1; ss <= Sel2; ss++ )
               {
                  if ( !lParam1 )
                  {
                     if ( ss >= lpSS->Row.AllocCnt )
                        break;

                     if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
                     {
                        if ( lpRow->fRowSelected )
                        {
                           lpSS->MultiSelCnt--;
                           lpRow->fRowSelected = FALSE;
                           SS_InvalidateRow( hWnd, lpSS, ss );
                        }
                     }
                  }
                  else
                  {
                     if ( (lpRow = SS_AllocLockRow( lpSS, ss )) != 0 )
                     {
                        if ( lpRow->fRowSelected == 0 )
                        {
                           lpSS->MultiSelCnt++;
                           lpRow->fRowSelected = TRUE;
                           SS_InvalidateRow( hWnd, lpSS, ss );
                        }

                        SS_UnlockRowItem( lpSS, ss );
                     }
                  }
               }

               SSSetBool( hWnd, SSB_REDRAW, RedrawOld );
            }

            break;

         case SS_SELMODE_SETCURSEL:
            if ( lpSS->wOpMode == SS_OPMODE_SINGLESEL )
            {
               SSSetActiveCell( hWnd, 1, (SS_COORD) lParam1 );
               lRet = 0;
            }

            break;

         case SS_SELMODE_SETSEL:
            if ( lpSS->wOpMode != SS_OPMODE_SINGLESEL )
            {
               if ( lParam2 == SS_ALLROWS || lParam2 == 0 )
                  SSSelModeSendMessage( hWnd, SS_SELMODE_SELITEMRANGE,
                                        lParam1, 1, lpSS->Row.Max );
               else
                  SSSelModeSendMessage( hWnd, SS_SELMODE_SELITEMRANGE,\
                                        lParam1, lParam2, lParam2 );

               lRet = 0;
            }

            break;
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( lRet );
}

WORD  OPERATION
SSSetSelBlockOptions( HWND hWnd,
                      WORD  wOption )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSSetSelBlockOptions" );
   LPSPREADSHEET lpSS;
   WORD          wSelBlockOptionOld;

   lpSS = SS_Lock( hWnd );
   wSelBlockOptionOld = lpSS->wSelBlockOption;

   if ( lpSS->fEditModePermanent == 0 )
      lpSS->wSelBlockOption = wOption;

// DrUnlockTaskMemory( lpSS );

   return( wSelBlockOptionOld );
}

zPCHAR
SS_SelBlockSplitItem( zPCHAR  hList,
                      zPLONG lpnItemCnt,
                      zPLONG lpnItemAllocCnt,
                      LPSS_SELBLOCK lpSelBlockOrig,
                      LPSS_SELBLOCK lpSelBlockSplit )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SS_SelBlockSplitItem" );
   SS_SELBLOCK SelBlockTemp;
   SS_COORD    ULCol;
   SS_COORD    ULRow;
   SS_COORD    LRCol;
   SS_COORD    LRRow;

   ULCol = lpSelBlockSplit->UL.ssCol;
   ULRow = lpSelBlockSplit->UL.ssRow;
   LRCol = lpSelBlockSplit->LR.ssCol;
   LRRow = lpSelBlockSplit->LR.ssRow;

   if ( ULRow < lpSelBlockOrig->UL.ssRow )
   {
      SSx_SetSelBlockStruct( &SelBlockTemp, ULCol, ULRow, LRCol,
                             mMin( LRRow, lpSelBlockOrig->UL.ssRow - 1 ) );

      hList = SS_ListAddItem( hList, lpnItemCnt, lpnItemAllocCnt,
                              &SelBlockTemp, zsizeof( SS_SELBLOCK ), 50 );

      ULRow = lpSelBlockOrig->UL.ssRow;
   }

   if ( ULCol < lpSelBlockOrig->UL.ssCol )
   {
      SSx_SetSelBlockStruct( &SelBlockTemp, ULCol, ULRow,
                             mMin( LRCol, lpSelBlockOrig->UL.ssCol - 1 ), LRRow );

      hList = SS_ListAddItem( hList, lpnItemCnt, lpnItemAllocCnt,
                              &SelBlockTemp, zsizeof( SS_SELBLOCK ), 50 );

      ULCol = lpSelBlockOrig->UL.ssCol;
   }

   if ( LRCol > lpSelBlockOrig->LR.ssCol )
   {
      SSx_SetSelBlockStruct( &SelBlockTemp, mMax( ULCol, lpSelBlockOrig->LR.ssCol + 1 ),
                             ULRow, LRCol, LRRow );

      hList = SS_ListAddItem( hList, lpnItemCnt, lpnItemAllocCnt,
                              &SelBlockTemp, zsizeof( SS_SELBLOCK ), 50 );

      LRCol = lpSelBlockOrig->LR.ssCol;
   }

   if ( LRRow > lpSelBlockOrig->LR.ssRow )
   {
      SSx_SetSelBlockStruct( &SelBlockTemp, ULCol, mMax( ULRow,
                             lpSelBlockOrig->LR.ssRow + 1 ), LRCol, LRRow );

      hList = SS_ListAddItem( hList, lpnItemCnt, lpnItemAllocCnt,
                              &SelBlockTemp, zsizeof( SS_SELBLOCK ), 50 );

      LRCol = lpSelBlockOrig->LR.ssCol;
   }

   return( hList );
}

void
SSx_SetSelBlockStruct( LPSS_SELBLOCK lpSelBlockDest,
                       SS_COORD ULCol,
                       SS_COORD ULRow,
                       SS_COORD LRCol,
                       SS_COORD LRRow )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSx_SetSelBlockStruct" );
   lpSelBlockDest->UL.ssCol = ULCol;
   lpSelBlockDest->UL.ssRow = ULRow;
   lpSelBlockDest->LR.ssCol = LRCol;
   lpSelBlockDest->LR.ssRow = LRRow;
}


// ss_row.cpp

zBOOL OPERATION
SSGetRowHeight( HWND      hWnd,
                SS_COORD  ssRow,
                zPLONG    pdxRowHeight )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSGetRowHeight" );
   LPSPREADSHEET lpSS;
   zBOOL         fRet = FALSE;

   lpSS = SS_Lock( hWnd );
   SSx_AdjustCellCoords( lpSS, 0, &ssRow );

   if ( ssRow < SS_GetRowsMax( lpSS ) )
   {
      SS_CalcPixelsToRowHeight( lpSS, ssRow,
                                SS_GetRowHeight( lpSS, ssRow ),
                                pdxRowHeight );
      fRet = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

zBOOL OPERATION
SSGetRowHeightInPixels( HWND     hWnd,
                        SS_COORD ssRow,
                        zPLONG lpnHeightPixels )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSGetRowHeightInPixels" );
   LPSPREADSHEET lpSS;
   zBOOL         fRet = FALSE;

   lpSS = SS_Lock( hWnd );
   SSx_AdjustCellCoords( lpSS, 0, &ssRow );

   if ( ssRow < SS_GetRowsMax( lpSS ) )
   {
      *lpnHeightPixels = SS_GetRowHeight( lpSS, ssRow );
      fRet = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

zBOOL OPERATION
SSInsColRange( HWND     hWnd,
               SS_COORD ssCol,
               SS_COORD ssCol2 )
{
#if 0
   TRACE_DEBUG( "<SS_COL.CPP>", "SSInsColRange" );
   LPSPREADSHEET    lpSS;
   zzPCHAR *        CalcTable;
   LPSS_ROW         lpRow;
   LPSS_CALC        lpCalc;
   SS_COORD         ss;
   zBOOL            fVisible = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( ssCol == SS_FIRST )
      ssCol = lpSS->Col.nHeaderCnt;
   else
   if ( ssCol == SS_LAST )
      ssCol = lpSS->Col.AllocCnt;
   else
   if ( ssCol <= 0 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }
   else
      ssCol += lpSS->Col.nHeaderCnt - 1;

   if ( ssCol >= lpSS->Col.AllocCnt || ssCol >= SS_GetColsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( ssCol2 == SS_FIRST )
      ssCol2 = lpSS->Col.nHeaderCnt;
   else
   if ( ssCol2 == SS_LAST )
      ssCol2 = lpSS->Col.AllocCnt;
   else
   if ( ssCol2 <= 0 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }
   else
      ssCol2 += lpSS->Col.nHeaderCnt - 1;

   if ( ssCol > ssCol2 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   SS_CellEditModeOff( hWnd, lpSS, 0 );

   for ( ss = SS_GetColsMax( lpSS ) - 1 - (Col2 - ssCol);
         ss <= SS_GetColsMax( lpSS ) - 1;
         ss++ )
   {
      SS_DeAllocCol( hWnd, ss, FALSE );
   }

   if ( SS_MoveAllocItems( lpSS,
                           &lpSS->Col.Items, ssCol, lpSS->Col.AllocCnt - 1,
                           ssCol2 + 1, SS_GetColsMax( lpSS ) ) )
   {
      for ( ss = 0; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
         {
            if ( !SS_MoveAllocItems( lpSS,
                                 &lpRow->Cells, ssCol, lpSS->Col.AllocCnt - 1,
                                 ssCol2 + 1, SS_GetColsMax( lpSS ) ) )
            {
               SS_UnlockRowItem( lpSS, ss );
//             DrUnlockTaskMemory( lpSS );
               return( FALSE );
            }

            SS_UnlockRowItem( lpSS, ss );
         }
      }

      if ( ssCol < lpSS->Col.DataCnt )
         SS_SetDataColCnt( lpSS, mMin( SS_GetColsMax( lpSS ),
                                    lpSS->Col.DataCnt + (Col2 - ssCol + 1) ) );

      if ( ssCol < lpSS->Col.AllocCnt )
         lpSS->Col.AllocCnt = mMin( SS_GetColsMax( lpSS ),
                                lpSS->Col.AllocCnt + ( ssCol2 - ssCol + 1 ) );

      lpSS->Col.LastRefCoord.Coord.ssCol = -1;
      lpSS->LastCell.Coord.ssCol = -1;
      lpSS->LastCell.Coord.ssRow = -1;

      if ( lpSS->hCalcTable )
      {
//       CalcTable = (zPCHAR) DrLockTaskMemory( lpSS->hCalcTable );
         CalcTable = (zPCHAR) lpSS->hCalcTable;
         for ( ss = 0; ss < lpSS->CalcTableCnt; ss++ )
         {
//          lpCalc = (LPSS_CALC) DrLockTaskMemory( CalcTable[ ss ] );
            lpCalc = (LPSS_CALC) CalcTable[ ss ];

            if ( lpCalc->CellCoord.ssCol >= ssCol2 )
               lpCalc->CellCoord.ssCol += ssCol2 - ssCol + 1;

//          DrUnlockTaskMemory( lpCalc );
         }

//       DrUnlockTaskMemory( CalcTable );
      }

      for ( ss = ssCol, fVisible = FALSE; ss <= ssCol2; ss++ )
         if ( SS_IsColVisible( hWnd, lpSS, ss, SS_VISIBLE_PARTIAL ) )
            fVisible = TRUE;

      if ( fVisible )
         SS_InvalidateColRange( hWnd, lpSS, ssCol, -1 );
      else
      if ( ssCol < lpSS->Col.UL )
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_COLS );
   }

   SSBuildDependencies( hWnd );
   SS_HiliteCell( hWnd, TRUE );
// DrUnlockTaskMemory( lpSS );
   return( TRUE );
#else
   MESSAGE_DEBUG( "<SS_COL.CPP>", "SSInsColRange" );
   return( FALSE );
#endif
}

zBOOL OPERATION
SSInsRowRange( HWND             hWnd,
               SS_COORD         ssRow,
               SS_COORD         ssRow2 )
{
#if 0
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSInsRowRange" );
   LPSPREADSHEET    lpSS;
   zzPCHAR *        CalcTable;
   LPSS_CALC        lpCalc;
   zBOOL            fRet;
   SS_COORD         i;
   zBOOL            fVisible = FALSE;

   lpSS = SS_Lock( hWnd );
   if ( ssRow == SS_FIRST )
      ssRow = lpSS->Row.nHeaderCnt;
   else
   if ( ssRow == SS_LAST )
      ssRow = lpSS->Row.AllocCnt;
   else
   if ( ssRow <= 0 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }
   else
      ssRow += lpSS->Row.nHeaderCnt - 1;

   if ( ssRow >= lpSS->Row.AllocCnt || ssRow >= SS_GetRowsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( TRUE );
   }

   if ( ssRow2 == SS_FIRST )
      ssRow2 = lpSS->Row.nHeaderCnt;
   else
   if ( ssRow2 == SS_LAST )
      ssRow2 = lpSS->Row.AllocCnt;
   else
   if ( ssRow2 <= 0 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }
   else
      ssRow2 += lpSS->Row.nHeaderCnt - 1;

   if ( ssRow > ssRow2 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   SS_CellEditModeOff( hWnd, lpSS, 0 );

   for ( ss = SS_GetRowsMax( lpSS ) - 1 - (Row2 - ssRow);
         ss <= SS_GetRowsMax( lpSS ) - 1;
         ss++ )
   {
      SS_DeAllocRow( hWnd, ss, FALSE );
   }

   if ( (fRet = SS_MoveAllocItems( lpSS,
                              &lpSS->Row.Items, ssRow, lpSS->Row.AllocCnt - 1,
                              ssRow2 + 1, SS_GetRowsMax( lpSS ) )) != 0 )
   {
      if ( ssRow < lpSS->Row.DataCnt )
         SS_SetDataRowCnt( lpSS, mMin( SS_GetRowsMax( lpSS ),
                           lpSS->Row.DataCnt + (Row2 - ssRow + 1) ) );

      if ( ssRow < lpSS->Row.AllocCnt )
         lpSS->Row.AllocCnt = mMin( SS_GetRowsMax( lpSS ),
                                   lpSS->Row.AllocCnt + (Row2 - ssRow + 1) );

      lpSS->Row.LastRefCoord.Coord.ssRow = -1;
      lpSS->LastCell.Coord.ssCol = -1;
      lpSS->LastCell.Coord.ssRow = -1;

      if ( lpSS->hCalcTable )
      {
//       CalcTable = (zPCHAR) DrLockTaskMemory( lpSS->hCalcTable );
         CalcTable = (zPCHAR) lpSS->hCalcTable;
         for ( ss = 0; ss < lpSS->CalcTableCnt; ss++ )
         {
//          lpCalc = (LPSS_CALC) DrLockTaskMemory( CalcTable[ ss ] );
            lpCalc = (LPSS_CALC) CalcTable[ ss ];

            if ( lpCalc->CellCoord.ssRow >= ssRow2 )
               lpCalc->CellCoord.ssRow += ssRow2 - ssRow + 1;

//          DrUnlockTaskMemory( lpCalc );
         }

//       DrUnlockTaskMemory( CalcTable );
      }

      if ( lpSS->fVirtualMode )
      {
         if ( ssRow <= lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 &&
             ssRow2 >= lpSS->Virtual.VTop )
            lpSS->Virtual.VPhysSize +=
               mMin( ssRow2, lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 ) -
               mMax( ssRow, lpSS->Virtual.VTop ) + 1;
      }

      for ( ss = ssRow, fVisible = FALSE; ss <= ssRow2 && !fVisible; ss++ )
         if ( SS_IsRowVisible( hWnd, lpSS, ss, SS_VISIBLE_PARTIAL ) )
            fVisible = TRUE;

      if ( fVisible )
         SS_InvalidateRowRange( hWnd, lpSS, ssRow, -1 );
      else
      if ( ssRow < lpSS->Row.UL )
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ROWS );
   }

   SSBuildDependencies( hWnd );
   SS_HiliteCell( hWnd, TRUE );

// DrUnlockTaskMemory( lpSS );
   return( fRet );
#else
   MESSAGE_DEBUG( "<SS_ROW.CPP>", "SSInsRowRange" );
   return( FALSE );
#endif
}

zBOOL OPERATION
SSSetRowCellType( HWND hWnd, LPSPREADSHEET lpSS,
                  SS_COORD ssRow, LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSSetRowCellType" );
   LPSS_CELLTYPE CellTypeTemp;
   LPSS_ROW      lpRow;
   LPSS_CELL     lpCell;
   SS_COORD      ss;

   for ( ss = 0; ss < lpSS->Col.AllocCnt; ss++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ss, ssRow )) != 0 )
      {
         SS_DeAllocCellType( hWnd, lpSS, lpCell->m_hCellType );
         lpCell->m_hCellType = 0;
      }
   }

   if ( (lpRow = SS_AllocLockRow( lpSS, ssRow )) != 0 )
   {
      SS_DeAllocCellType( hWnd, lpSS, lpRow->hCellType );
      lpRow->hCellType = 0;

      if ( CellType )
      {
         if ( DrAllocTaskMemory( (zCOREMEM) &lpRow->hCellType,
                                 (zLONG) zsizeof( SS_CELLTYPE ), 1032 ) != 0 )
         {
//          SS_UnlockRowItem( lpSS, ssRow );
            return( FALSE );
         }

         CellTypeTemp = (LPSS_CELLTYPE) DrLockTaskMemory( lpRow->hCellType );
         lpRow->hCellType = (zPCHAR) CellTypeTemp;
         zmemcpy( CellTypeTemp, CellType, zsizeof( SS_CELLTYPE ) );
//       DrUnlockTaskMemory( CellTypeTemp );
      }

      SS_UnlockRowItem( lpSS, ssRow );
      SS_InvalidateRow( hWnd, lpSS, ssRow );
   }

   return( TRUE );
}

zBOOL
SS_SetRowColor( HWND          hWnd,
                LPSPREADSHEET lpSS,
                SS_COORD      ssRow,
                COLORREF      Background,
                COLORREF      Foreground )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SS_SetRowColor" );
   LPSS_ROW      lpRow;
   LPSS_CELL     lpCell;
   SS_COORD      ss;

   for ( ss = 0; ss < lpSS->Col.AllocCnt; ss++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ss, ssRow )) != 0 )
         SS_ClearColorItem( &lpCell->m_Color, Background, Foreground );
   }

   if ( (lpRow = SS_AllocLockRow( lpSS, ssRow )) != 0 )
   {
      if ( Background != RGBCOLOR_DEFAULT )
         lpRow->Color.BackgroundId = SS_AddColor( lpSS, Background );

      if ( Foreground != RGBCOLOR_DEFAULT )
         lpRow->Color.ForegroundId = SS_AddColor( lpSS, Foreground );

      SS_UnlockRowItem( lpSS, ssRow );
   }

   SS_InvalidateRow( hWnd, lpSS, ssRow );
   return( TRUE );
}

zBOOL OPERATION
SSSetRowFont( HWND          hWnd,
              LPSPREADSHEET lpSS,
              SS_COORD      ssRow,
              HFONT         hFont,
              zBOOL         DeleteFont,
              zBOOL         *lpfDeleteFontObject )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSSetRowFont" );
   LPSS_ROW      lpRow;
   LPSS_CELL     lpCell;
   SS_COORD      ss;

   for ( ss = 0; ss < lpSS->Col.AllocCnt; ss++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ss, ssRow )) != 0 )
         lpCell->m_FontId = SS_FONT_NONE;
   }

   if ( (lpRow = SS_AllocLockRow( lpSS, ssRow )) != 0 )
   {
      lpRow->FontId = SS_InitFont( hWnd, hFont, DeleteFont, TRUE,
                                   lpfDeleteFontObject );
      SS_SetRowMaxFont( lpSS, lpRow, ssRow );
      SS_UnlockRowItem( lpSS, ssRow );

      if ( lpSS->fAllowCellOverflow )
         for ( ss = lpSS->Col.nHeaderCnt; ss < lpSS->Col.DataCnt; ss++ )
            SS_OverflowAdjustNeighbor( hWnd, lpSS, ss, ssRow );
   }

   if ( SS_IsRowVisible( hWnd, lpSS, ssRow, SS_VISIBLE_PARTIAL ) )
      SS_InvalidateRowRange( hWnd, lpSS, ssRow, -1 );

   return( TRUE );
}

zBOOL OPERATION
SSSetRowHeight( HWND     hWnd,
                SS_COORD ssRow,
                zLONG    dxHeight )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSSetRowHeight" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   LPSS_ROW      lpRow;
   SS_COORD      ss;
   zLONG         lHeightOld = -2;
   zLONG         lRowHeightX100;

   SS_CellEditModeOff( hWnd, lpSS, 0 );

   if ( dxHeight == -1 )
      lRowHeightX100 = -1;
   else
      lRowHeightX100 = (zLONG)(dxHeight * 100.0);

   if ( ssRow == -1 )
   {
      for ( ss = lpSS->Row.nHeaderCnt; ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
         {
            lpRow->lRowHeightX100 = SS_WIDTH_DEFAULT;
            lpRow->nRowHeightInPixels = SS_WIDTH_DEFAULT;

            if ( lpRow->FontId != SS_FONT_NONE )
               lpRow->nRowHeightInPixels = SS_CalcRowHeight( lpSS, ss );

            SS_UnlockRowItem( lpSS, ss );
         }
      }

      lHeightOld = lpSS->Row.lCellSizeX100;
      lpSS->Row.lCellSizeX100 = lRowHeightX100;
      lpSS->Row.nCellSizeInPixels = SS_RowHeightToPixels( lpSS, ssRow, dxHeight );
   }
   else
   {
      SSx_AdjustCellCoords( lpSS, 0, &ssRow );

      if ( ssRow >= SS_GetRowsMax( lpSS ) )
      {
//       DrUnlockTaskMemory( lpSS );
         return( FALSE );
      }

      if ( (lpRow = SS_AllocLockRow( lpSS, ssRow )) != 0 )
      {
         lHeightOld = lpRow->lRowHeightX100;
         if ( lpRow->lRowHeightX100 != lRowHeightX100 )
         {
            lpRow->lRowHeightX100 = lRowHeightX100;

            if ( lpRow->lRowHeightX100 == -1 && lpRow->FontId == SS_FONT_NONE )
               lpRow->nRowHeightInPixels = SS_WIDTH_DEFAULT;
            else
               lpRow->nRowHeightInPixels = SS_CalcRowHeight( lpSS, ssRow );
         }

         SS_UnlockRowItem( lpSS, ssRow );
      }
   }

   if ( (lHeightOld != -2 && lHeightOld != dxHeight) || ssRow == -1 )
   {
      if ( !SS_AutoSize( hWnd, FALSE ) )
      {
         if ( ssRow == -1 )
            SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
         else
         if ( SS_IsRowVisible( hWnd, lpSS, ssRow, SS_VISIBLE_PARTIAL ) )
            SS_InvalidateRowRange( hWnd, lpSS, ssRow, -1 );
      }
   }
   else
   if ( ssRow > 0 && dxHeight == 0.0 && lpSS->Row.ssCurrAt == ssRow )
   {
      SS_ScrollLineLR( hWnd, lpSS, SS_F_SCROLL_DOWN | SS_F_MOVEACTIVECELL );
      if ( lpSS->Row.ssCurrAt == ssRow )
         SS_ScrollLineUL( hWnd, lpSS, SS_F_SCROLL_UP | SS_F_MOVEACTIVECELL );
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

zBOOL
SS_SetRowLock( HWND          hWnd,
               LPSPREADSHEET lpSS,
               SS_COORD      ssRow,
               zBOOL         Lock )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SS_SetRowLock" );
   LPSS_ROW      lpRow;
   LPSS_CELL     lpCell;
   SS_COORD      ss;

   for ( ss = 0; ss < lpSS->Col.AllocCnt; ss++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ss, ssRow )) != 0 )
      {
         lpCell->m_CellLocked = SS_LOCKED_DEF;
         SS_UnlockCellItem( lpSS, ss, ssRow );
      }
   }

   if ( (lpRow = SS_AllocLockRow( lpSS, ssRow )) != 0 )
   {
      lpRow->fRowLocked = Lock;
      SS_UnlockRowItem( lpSS, ssRow );

      if ( lpSS->LockColor.ForegroundId || lpSS->LockColor.BackgroundId )
         SS_InvalidateRow( hWnd, lpSS, ssRow );
   }

   return( TRUE );
}

void
SS_AdjustDataRowCnt( LPSPREADSHEET lpSS,
                     SS_COORD ssRow,
                     SS_COORD ssRow2 )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SS_AdjustDataRowCnt" );
   LPSS_ROW  lpRow;
   LPSS_CELL lpCell;
   SS_COORD  Top;
   SS_COORD  ss;
   SS_COORD  j;
   zBOOL     fHasData = FALSE;

   if ( ssRow == SS_ALLROWS ||
        (ssRow < lpSS->Row.DataCnt && lpSS->Row.DataCnt <= ssRow2 + 1) )
   {
      if ( lpSS->fVirtualMode )
         Top = lpSS->Virtual.VTop;
      else
         Top = lpSS->Row.nHeaderCnt;

      for ( ss = lpSS->Row.DataCnt - 1; ss >= Top && !fHasData; ss-- )
      {
         if ( (lpRow = SS_LockRowItem( lpSS, ss )) != 0 )
         {
            for ( j = lpSS->Col.nHeaderCnt; j < lpSS->Col.DataCnt && !fHasData; j++ )
            {
               if ( (lpCell = SS_LockCellItem( lpSS, j, ss )) != 0 )
               {
                  if ( lpCell->m_CellData.m_DataType )
                     fHasData = TRUE;

                  SS_UnlockCellItem( lpSS, j, ss );
               }
            }

            SS_UnlockRowItem( lpSS, ss );
         }

         if ( !fHasData )
            lpSS->Row.DataCnt--;
      }

      if ( lpSS->fVirtualMode && !fHasData )
         lpSS->Row.DataCnt = lpSS->Row.nHeaderCnt;
   }
}

zBOOL
SS_ClearRow( HWND  hWnd,
             SS_COORD ssRow,
             zBOOL Paint,
             zBOOL fSendChangeMsg )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SS_ClearRow" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( ssRow >= lpSS->Row.AllocCnt || ssRow >= SS_GetRowsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   SS_DeAllocRow( hWnd, ssRow, fSendChangeMsg );

   if ( Paint && SS_IsRowVisible( hWnd, lpSS, ssRow, SS_VISIBLE_PARTIAL ) )
      SS_InvalidateRowRange( hWnd, lpSS, ssRow, -1 );

// DrUnlockTaskMemory( lpSS );

   return( TRUE );
}

zBOOL
SS_IsRowVisible( HWND hWnd,
                 LPSPREADSHEET lpSS,
                 SS_COORD ssRow,
                 zSHORT nVisible )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SS_IsRowVisible" );
   zBOOL fRet = FALSE;

   switch ( nVisible )
   {
      case SS_VISIBLE_ALL:
         if ( ssRow < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen ||
              (ssRow >= lpSS->Row.UL &&
               ssRow <= SS_GetBottomVisCell( hWnd, lpSS, lpSS->Row.UL )) )
         {
            fRet = TRUE;
         }

         break;

      case SS_VISIBLE_PARTIAL:
         if ( lpSS->fVirtualMode )
         {
            if ( ssRow < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen ||
                 (ssRow >= lpSS->Row.UL &&
                  ssRow <= SS_GetBottomCell( hWnd, lpSS, lpSS->Row.UL )) )
            {
               fRet = TRUE;
            }
         }
         else
         {
            if ( ssRow < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen ||
                 (ssRow >= lpSS->Row.UL && ssRow <= lpSS->Row.LR) )
            {
               fRet = TRUE;
            }
         }

         break;
   }

   return( fRet );
}

void
SS_SetBlock( LPSS_SELBLOCK lpBlock,
             SS_COORD ssCol1, SS_COORD ssRow1,
             SS_COORD ssCol2, SS_COORD ssRow2 )
{
   TRACE_DEBUG( "<SS_MAIN.CPP>", "SS_SetBlock" );
   lpBlock->UL.ssCol = ssCol1;
   lpBlock->UL.ssRow = ssRow1;
   lpBlock->LR.ssCol = ssCol2;
   lpBlock->LR.ssRow = ssRow2;
}

void
SS_SetDataRowCnt( LPSPREADSHEET lpSS,
                  SS_COORD ssRowCnt )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SS_SetDataRowCnt" );
   if ( lpSS->Row.MaxBeforeSort == -1 || ssRowCnt <= lpSS->Row.MaxBeforeSort )
      lpSS->Row.DataCnt = ssRowCnt;
}


// ss_scrol.cpp

zBOOL
SS_ActivateCell( HWND      hWnd,
                 SS_COORD  ssCol,
                 SS_COORD  ssRow,
                 zBOOL     Show )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ActivateCell" );
   LPSPREADSHEET lpSS;
   zBOOL         fKillFocusReceived;
   zBOOL         fRet = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return( FALSE );

   if ( (lpSS->wOpMode == SS_OPMODE_SINGLESEL ||
         lpSS->wOpMode == SS_OPMODE_MULTISEL ||
         lpSS->wOpMode == SS_OPMODE_EXTSEL ||
         lpSS->wOpMode == SS_OPMODE_NORMAL) && GetFocus( ) != hWnd )
   {
      if ( SS_USESINGLESELBAR( lpSS ) && lpSS->fKillFocusReceived )
         lpSS->fNoDrawFocusRect = TRUE;

      SS_HiliteCell( hWnd, FALSE );
      lpSS->fNoDrawFocusRect = FALSE;
   }
   else
      SS_HiliteCell( hWnd, FALSE );

   if ( SS_IsCellValid( hWnd, lpSS, ssCol, ssRow ) )
   {
      lpSS->Row.ssCurrAt = ssRow;
      lpSS->Col.ssCurrAt = ssCol;

      if ( Show )
      {
         if ( SS_USESINGLESELBAR( lpSS ) && GetFocus( ) != hWnd )
            lpSS->FreezeHilite = TRUE;

         SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
      }
      else
         SS_HiliteCell( hWnd, TRUE );

      fRet = TRUE;
   }
   else
      SS_HiliteCell( hWnd, TRUE );

   if ( SS_USESINGLESELBAR( lpSS ) )
   {
      if ( GetFocus( ) != hWnd )
      {
         lpSS->FreezeHilite = FALSE;
         fKillFocusReceived = lpSS->fKillFocusReceived;
         SS_HiliteCell( hWnd, TRUE );

         if ( fKillFocusReceived )
         {
            lpSS->fProcessingKillFocus = TRUE;
            SS_HiliteCell( hWnd, FALSE );
            lpSS->fProcessingKillFocus = FALSE;
         }
      }
      else
         lpSS->fKillFocusReceived = FALSE;
   }

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

zBOOL
SS_AllCellsHaveData( HWND          hWnd,
                     LPSPREADSHEET lpSS,
                     SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_AllCellsHaveData" );
   SS_CELLTYPE   CellType;
   LPSS_CELL     lpCell;
   SS_COORD      ss;
   zBOOL         fRet = TRUE;

   for ( ss = lpSS->Col.nHeaderCnt; ss <= lpSS->Col.Max; ss++ )
   {
      SS_RetrieveCellType( lpSS, &CellType, 0, ss, ssRow );

      if ( CellType.chType != SS_TYPE_STATICTEXT &&
           CellType.chType != SS_TYPE_OWNERDRAW &&
           CellType.chType != SS_TYPE_PICTURE &&
           CellType.chType != SS_TYPE_BUTTON &&
           CellType.chType != SS_TYPE_CHECKBOX )
      {
         if ( ( lpCell = SS_LockCellItem( lpSS, ss, ssRow ) ) != 0 )
         {
            if ( lpCell->m_CellData.m_DataType == 0 )
               fRet = FALSE;

            SS_UnlockCellItem( lpSS, ss, ssRow );
         }
      }
      else
         fRet = FALSE;
   }

   return( fRet );
}

zSHORT
SS_GetCellPosXInRect( LPSPREADSHEET lpSS,
                      SS_COORD ssColLeft,
                      SS_COORD ssCol,
                      LPRECT   lpRect )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_GetCellPosXInRect" );
   SS_COORD ss;
   zSHORT   k;

   for ( ss = 0, k = -1;
         ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen && ss < ssCol;
         ss++ )
   {
      k += SS_GetColWidth( lpSS, ss );
   }

   for ( ss = mMax( ss, ssColLeft ); ss < ssCol && k < lpRect->right; ss++ )
      k += SS_GetColWidth( lpSS, ss );

   return( k );
}

zSHORT
SS_GetCellPosYInRect( LPSPREADSHEET lpSS,
                      SS_COORD ssRowTop,
                      SS_COORD ssRow,
                      LPRECT   lpRect )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_GetCellPosYInRect" );
   SS_COORD ss;
   zSHORT   k;

   for ( ss = 0, k = -1;
         ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen && ss < ssRow;
         ss++ )
   {
      k += SS_GetRowHeight( lpSS, ss );
   }

   for ( ss = mMax( ss, ssRowTop ); ss < ssRow && k < lpRect->bottom; ss++ )
      k += SS_GetRowHeight( lpSS, ss );

   return( k );
}

zBOOL
SS_GetFirstValidCol( HWND hWnd,
                     LPSPREADSHEET lpSS,
                     SS_COORD ssRow,
                     LPSS_COORD lpCol )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_GetFirstValidCol" );
   SS_CELLTYPE   CellType;
   SS_COORD      dColAtTemp = 0;
   zBOOL         fFound = FALSE;

   while ( fFound == 0 && dColAtTemp < SS_GetColsMax( lpSS ) - 1 &&
           (lpSS->RestrictCols == 0 || dColAtTemp <= lpSS->Col.DataCnt) )
   {
     if ( dColAtTemp == lpSS->Col.Frozen )
         dColAtTemp = mMax( lpSS->Col.UL,
                           lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
      else
         dColAtTemp++;

      SS_RetrieveCellType( lpSS, &CellType, 0, dColAtTemp, ssRow );

      if ( SS_GetColWidth( lpSS, dColAtTemp ) > 0 &&
           (lpSS->fEditModePermanent == 0 ||
            (CellType.chType != SS_TYPE_STATICTEXT &&
             CellType.chType != SS_TYPE_OWNERDRAW &&
             CellType.chType != SS_TYPE_PICTURE &&
             !SS_GetLock( hWnd, lpSS, dColAtTemp, ssRow ))) )
      {
         fFound = TRUE;
      }
   }

   if ( fFound )
      *lpCol = dColAtTemp;

   return( fFound );
}

zBOOL
SS_GetFirstValidRow( HWND hWnd,
                     LPSPREADSHEET lpSS,
                     SS_COORD ssCol,
                     LPSS_COORD lpRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_GetFirstValidRow" );
   SS_CELLTYPE   CellType;
   SS_COORD      dRowAtTemp = 0;
   zBOOL         fFound = FALSE;

   while ( fFound == 0 && dRowAtTemp < SS_GetRowsMax( lpSS ) - 1 &&
           (lpSS->RestrictRows == 0 || dRowAtTemp <= lpSS->Row.DataCnt) )
   {
     if ( dRowAtTemp == lpSS->Row.Frozen )
         dRowAtTemp = mMax( lpSS->Row.UL,
                           lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
      else
         dRowAtTemp++;

      SS_RetrieveCellType( lpSS, &CellType, 0, ssCol, dRowAtTemp );

      if ( SS_GetRowHeight( lpSS, dRowAtTemp ) > 0 &&
           (lpSS->fEditModePermanent == 0 ||
           (CellType.chType != SS_TYPE_STATICTEXT &&
            CellType.chType != SS_TYPE_OWNERDRAW &&
            CellType.chType != SS_TYPE_PICTURE &&
            !SS_GetLock( hWnd, lpSS, ssCol, dRowAtTemp ))) )
      {
         fFound = TRUE;
      }
   }

   if ( fFound )
      *lpRow = dRowAtTemp;

   return( fFound );
}

SS_COORD
SS_GetLastPageLeftCol( HWND          hWnd,
                       LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_GetLastPageLeftCol" );
   RECT          rect;
   SS_COORD      MaxCols;
   SS_COORD      ssColAt;
   SS_COORD      ssColAtTemp;
   zSHORT        nWidth;
   zSHORT        k;

   if ( lpSS->fVirtualMode && lpSS->Col.Max == 0 )
      return( 0 );

   SS_GetClientRect( hWnd, &rect );
   k = SS_GetCellPosX( lpSS, 0, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
   MaxCols = SS_GetColsMax( lpSS );
   ssColAt = MaxCols - 1;

   for ( ssColAtTemp = ssColAt;
         ssColAtTemp >= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;
         ssColAtTemp-- )
   {
      nWidth = SS_GetColWidth( lpSS, ssColAtTemp );
      k += nWidth;

      if ( k > rect.right - rect.left )
         break;

      if ( nWidth > 0 )
         ssColAt = ssColAtTemp;
   }

   return( mMin( MaxCols - 1, ssColAt ) );
}

SS_COORD
SS_GetLastPageTopRow( HWND    hWnd,
                      LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_GetLastPageTopRow" );
   return( SSx_GetLastPageTopRow( hWnd, lpSS, SS_GetRowsMax( lpSS ) ) );
}

void
SS_GetScrollArea( HWND    hWnd,
                  LPSPREADSHEET lpSS,
                  LPRECT  lpRect,
                  zSHORT  lScrollArea )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_GetScrollArea" );
   SS_COORD ss;

   SS_GetClientRect( hWnd, lpRect );

   switch ( lScrollArea )
   {
      case SS_SCROLLAREA_ALL:
         break;

      case SS_SCROLLAREA_COLS:
         for ( ss = 0; ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen; ss++ )
            lpRect->left += SS_GetColWidth( lpSS, ss );

         break;

      case SS_SCROLLAREA_ROWS:
         for ( ss = 0; ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen; ss++ )
            lpRect->top += SS_GetRowHeight( lpSS, ss );

         break;

      case SS_SCROLLAREA_CELLS:
         // Determine Left.
         for ( ss = 0; ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen; ss++ )
            lpRect->left += SS_GetColWidth( lpSS, ss );

         // Determine Top.
         for ( ss = 0; ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen; ss++ )
            lpRect->top += SS_GetRowHeight( lpSS, ss );

         break;
   }
}

void
SS_HPos( HWND          hWnd,
         SS_COORD      ssPos )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_HPos" );
   LPSPREADSHEET lpSS;
   zSHORT        lScrollInc;

   lpSS = SS_Lock( hWnd );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return;

   SS_HiliteCell( hWnd, FALSE );

   lScrollInc = SS_GetCellPosX( lpSS,
                                lpSS->Col.nHeaderCnt + lpSS->Col.Frozen,
                                ssPos ) -
                SS_GetCellPosX( lpSS,
                                lpSS->Col.nHeaderCnt + lpSS->Col.Frozen,
                                lpSS->Col.UL );
   lpSS->Col.UL = ssPos;

   SS_HScroll( hWnd, lScrollInc );
// DrUnlockTaskMemory( lpSS );
}

void
SS_HScroll( HWND   hWnd,
            zLONG lScrollInc )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_HScroll" );
   LPSPREADSHEET lpSS;
   RECT          rectTemp;
   RECT          rect;

   if ( lScrollInc != 0 )
   {
      lpSS = SS_Lock( hWnd );

      SS_GetScrollArea( hWnd, lpSS, &rect, SS_SCROLLAREA_COLS );

      SS_Scroll( hWnd, -lScrollInc, 0, &rect );

      if ( lScrollInc < 0 )
      {
         if ( SS_IsBlockSelected( lpSS ) )
         {
            SetRect( &rectTemp, rect.left + -lScrollInc, rect.top,
                     rect.left + -lScrollInc + 1, rect.bottom );
            InvalidateRect( hWnd, &rectTemp, TRUE );
         }
      }

      // this "if" is required for certain repaint operations, for example,
      // when the user clicks on a right-most column that causes an auto-
      // scroll to the right, this code is required to paint appropriately
      if ( lpSS->fUseScrollArrows )
         lpSS->Redraw = FALSE;
      else
         UpdateWindow( hWnd );

      SS_SetHScrollBar( hWnd, lpSS );
      SS_LeftColChange( hWnd );

      // this "if" is required for certain repaint operations, for example,
      // when the user clicks on a right-most column that causes an auto-
      // scroll to the right, this code is required to paint appropriately
      if ( lpSS->fUseScrollArrows )
      {
         lpSS->Redraw = TRUE;
         SS_SetHScrollBar( hWnd, lpSS );
         UpdateWindow( hWnd );
      }

//    DrUnlockTaskMemory( lpSS );
   }
}

void
SSx_InvalidateButtons( HWND          hWnd,
                       LPSPREADSHEET lpSS,
                       SS_COORD      ssColLeft,
                       SS_COORD      ssRowTop,
                       SS_COORD      ssCol1,
                       SS_COORD      ssRow1,
                       SS_COORD      ssCol2,
                       SS_COORD      ssRow2 )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSx_InvalidateButtons" );
   SS_CELLTYPE   CellType;
   RECT          rect;
   zLONG         x;
   zLONG         y;
   zSHORT        cx;
   zSHORT        cy;
   SS_COORD      ss;
   SS_COORD      k;

   x = SS_GetCellPosX( lpSS, ssColLeft, ssCol1 );

   for ( ss = ssCol1; ss <= ssCol2; ss++ )
   {
      cx = SS_GetColWidth( lpSS, ss );
      y = SS_GetCellPosY( lpSS, ssRowTop, ssRow1 );

      for ( k = ssRow1; k <= ssRow2; k++ )
      {
         cy = SS_GetRowHeight( lpSS, k );

         SS_RetrieveCellType( lpSS, &CellType, 0, ss, k );

         if ( CellType.chType == SS_TYPE_BUTTON ||
              CellType.chType == SS_TYPE_COMBOBOX )
         {
            rect.left = x;
            rect.top = y;
            rect.right = x + cx + 1;
            rect.bottom = y + cy + 1;

            InvalidateRect( hWnd, &rect, TRUE );
         }

         y += cy;
      }

      x += cx;
   }
}

void
SS_InvalidateButtons( HWND          hWnd,
                      LPSPREADSHEET lpSS,
                      SS_COORD      ssColOld,
                      SS_COORD      ssColLeftOld,
                      SS_COORD      ssRowOld,
                      SS_COORD      ssRowTopOld )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateButtons" );
   if ( ssColOld == -1 )
      ssColOld = lpSS->Col.ssCurrAt;

   if ( ssColLeftOld == -1 )
      ssColLeftOld = lpSS->Col.UL;

   if ( ssRowOld == -1 )
      ssRowOld = lpSS->Row.ssCurrAt;

   if ( ssRowTopOld == -1 )
      ssRowTopOld = lpSS->Row.UL;

   if ( lpSS->wButtonDrawMode & SS_BDM_CURRENTCELL )
   {
      SSx_InvalidateButtons( hWnd, lpSS, ssColLeftOld, ssRowTopOld,
                             ssColOld, ssRowOld, ssColOld, ssRowOld );
      SSx_InvalidateButtons( hWnd, lpSS, ssColLeftOld, ssRowTopOld,
                             lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt,
                             lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
   }

   if ( lpSS->wButtonDrawMode & SS_BDM_CURRENTCOL &&
        ssColOld != lpSS->Col.ssCurrAt )
   {
      SSx_InvalidateButtons( hWnd, lpSS, ssColLeftOld, ssRowTopOld,
                             ssColOld, ssRowTopOld, ssColOld, lpSS->Row.LR );
      SSx_InvalidateButtons( hWnd, lpSS, ssColLeftOld, ssRowTopOld,
                             lpSS->Col.ssCurrAt, lpSS->Row.UL,
                             lpSS->Col.ssCurrAt, lpSS->Row.LR );
   }

   if ( lpSS->wButtonDrawMode & SS_BDM_CURRENTROW &&
        ssRowOld != lpSS->Row.ssCurrAt )
   {
      SSx_InvalidateButtons( hWnd, lpSS, ssColLeftOld, ssRowTopOld,
                             ssColLeftOld, ssRowOld, lpSS->Col.LR, ssRowOld );
      SSx_InvalidateButtons( hWnd, lpSS, ssColLeftOld, ssRowTopOld,
                             lpSS->Col.UL, lpSS->Row.ssCurrAt,
                             lpSS->Col.LR, lpSS->Row.ssCurrAt );
   }
}

void
SS_InvalidateCell( HWND          hWnd,
                   LPSPREADSHEET lpSS,
                   SS_COORD      CellCol,
                   SS_COORD      CellRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateCell" );
   RECT          rect;
   RECT          rectTemp;
   RECT          rectClient;
   HDC           hDC;
   HRGN          hRgn;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   WORD          wOverflow = 0;
   zBOOL         fDrawSelBlock = FALSE;
   zBOOL         fTurnEditModeOn = FALSE;
   zLONG         x;
   zLONG         y;
   zLONG         cx;
   zLONG         cy;

#ifndef SS_NOOVERFLOW
   RECT  rectOverflowAll;
#endif

   if ( lpSS->fNoInvalidate )
      return;

   if ( mIs_hWndVisible( hWnd ) &&
        SS_IsCellVisible( hWnd, lpSS, CellCol, CellRow, SS_VISIBLE_PARTIAL ) )
   {
      SS_GetCellCoord( lpSS, lpSS->Col.UL, lpSS->Row.UL,
                       CellCol, CellRow, &x, &y, &cx, &cy );

      SetRect( &rect, x + 1, y + 1, x + cx, y + cy );
      SS_GetClientRect( hWnd, &rectClient );

#ifndef SS_NOOVERFLOW
      if ( lpSS->fAllowCellOverflow )
         wOverflow = SS_GetOverflowRects( hWnd, lpSS, CellCol, CellRow,
                                          &rectClient, &rect, 0, 0,
                                          0, &rectOverflowAll );
#endif

      if ( wOverflow )
      {
#ifndef SS_NOOVERFLOW
         InflateRect( &rectOverflowAll, 1, 1 );
         InvalidateRect( hWnd, &rectOverflowAll, TRUE );

   //x   if (lpSS->Redraw)
   //x      UpdateWindow( hWnd );
   //x   else
            lpSS->fInvalidated = TRUE;
#endif
      }
#ifndef SS_NOLISTBOX
      else
      if ( SS_USESELBAR( lpSS ) &&
           SSSelModeSendMessage( hWnd, SS_SELMODE_GETSEL, CellRow, 0L, 0L ) )
      {
         InflateRect( &rect, 1, 1 );
         InvalidateRect( hWnd, &rect, TRUE );
         lpSS->fInvalidated = TRUE;
      }
#endif
      else
      if ( lpSS->Redraw )
      {
         if ( CellCol >= lpSS->Col.ssCurrAt - 1 && CellCol <= lpSS->Col.ssCurrAt + 1 &&
              CellRow >= lpSS->Row.ssCurrAt - 1 && CellRow <= lpSS->Row.ssCurrAt + 1 )
         {
            SS_HiliteCell( hWnd, FALSE );
         }

         if ( lpSS->EditModeOn && lpSS->EditModeTurningOff == 0 &&
              lpSS->Row.ssCurrAt == CellRow && lpSS->Col.ssCurrAt == CellCol )
         {
            WORD wMessageBeingSentOld = lpSS->wMessageBeingSent;
            lpSS->wMessageBeingSent = FALSE;
            SS_CellEditModeOff( hWnd, lpSS, 0 );
            lpSS->wMessageBeingSent = wMessageBeingSentOld;
            fTurnEditModeOn = TRUE;
         }

         hDC = GetDC( hWnd );

         lpCol = SS_LockColItem( lpSS, CellCol );
         lpRow = SS_LockRowItem( lpSS, CellRow );

         IntersectRect( &rectTemp, &rect, &rectClient );
         hRgn = CreateRectRgnIndirect( &rectTemp );
         SelectClipRgn( hDC, hRgn );
         DeleteObject( hRgn );

         if ( GetFocus( ) == hWnd || lpSS->fRetainSelBlock ||
             SS_IsBlockSelected( lpSS ) )
         {
            SS_DrawSelBlock( hWnd, hDC, lpSS );
            fDrawSelBlock = TRUE;
         }

         SS_DrawCell( hDC, hWnd, lpSS, lpCol, lpRow, &rect, CellCol, CellRow );

         if ( fDrawSelBlock )
            SS_DrawSelBlock( hWnd, hDC, lpSS );

         SelectClipRgn( hDC, 0 );

         SS_UnlockColItem( lpSS, CellCol );
         SS_UnlockRowItem( lpSS, CellRow );

         ReleaseDC( hWnd, hDC );

         if ( fTurnEditModeOn )
            PostMessage( hWnd, SSM_SETEDITMODE, TRUE, 0L );

         else
         if ( CellCol >= lpSS->Col.ssCurrAt - 1 &&
              CellCol <= lpSS->Col.ssCurrAt + 1 &&
              CellRow >= lpSS->Row.ssCurrAt - 1 &&
              CellRow <= lpSS->Row.ssCurrAt + 1 )
         {
            SS_HiliteCell( hWnd, TRUE );
         }
      }
      else
      {
         InflateRect( &rect, 1, 1 );
         InvalidateRect( hWnd, &rect, TRUE );
         lpSS->fInvalidated = TRUE;
      }
   }
}

void
SS_InvalidateCellRange( HWND          hWnd,
                        LPSPREADSHEET lpSS,
                        SS_COORD      ssCol1,
                        SS_COORD      ssRow1,
                        SS_COORD      ssCol2,
                        SS_COORD      ssRow2 )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateCellRange" );
   RECT          rect;
   zLONG         x;
   zLONG         y;
   zSHORT        cx;
   zSHORT        cy;

   if ( mIs_hWndVisible( hWnd ) &&
        (ssCol1 < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen ||
         ((ssCol1 == -1 || ssCol1 <= lpSS->Col.LR) &&
          (ssCol2 == -1 || ssCol2 >= lpSS->Col.UL))) &&
        (ssRow1 < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen ||
         ((ssRow1 == -1 || ssRow1 <= lpSS->Row.LR) &&
          (ssRow2 == -1 || ssRow2 >= lpSS->Row.UL))) )
   {
      SS_GetClientRect( hWnd, &rect );

      if ( ssRow1 != -1 && ssRow1 >= lpSS->Row.UL )
      {
         y = SS_GetCellPosYInRect( lpSS, lpSS->Row.UL, ssRow1, &rect );

         rect.top = y + 1;

         if ( ssRow1 == ssRow2 )
         {
            cy = SS_GetRowHeight( lpSS, ssRow1 );
            rect.bottom = y + cy;
         }
      }

      if ( ssRow2 != -1 && ssRow1 != ssRow2 && ssRow2 <= lpSS->Row.LR )
      {
         y = SS_GetCellPosYInRect( lpSS, lpSS->Row.UL, ssRow2 + 1, &rect );
         rect.bottom = y;
      }

      if ( ssCol1 != -1 && ssCol1 >= lpSS->Col.UL )
      {
         x = SS_GetCellPosXInRect( lpSS, lpSS->Col.UL, ssCol1, &rect );

         rect.left = x + 1;

         if ( ssCol1 == ssCol2 )
         {
            cx = SS_GetColWidth( lpSS, ssCol1 );
            rect.right = x + cx;
         }
      }

      if ( ssCol2 != -1 && ssCol1 != ssCol2 && ssCol2 <= lpSS->Col.LR )
      {
         x = SS_GetCellPosXInRect( lpSS, lpSS->Col.UL, ssCol2 + 1, &rect );
         rect.right = x;
      }

      InvalidateRect( hWnd, &rect, TRUE );

   //xif (lpSS->Redraw)
   //x   UpdateWindow( hWnd );
   //xelse
         lpSS->fInvalidated = TRUE;
   }
}

void
SS_InvalidateCol( HWND          hWnd,
                  LPSPREADSHEET lpSS,
                  SS_COORD      ssCol )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateCol" );
   RECT          rect;
   zLONG         x;
   zSHORT        cx;

   if ( lpSS->fNoInvalidate )
      return;

   if ( mIs_hWndVisible( hWnd ) &&
        SS_IsColVisible( hWnd, lpSS, ssCol, SS_VISIBLE_PARTIAL ) )
   {
      SS_GetClientRect( hWnd, &rect );

      x = SS_GetCellPosX( lpSS, lpSS->Col.UL, ssCol );
      cx = SS_GetColWidth( lpSS, ssCol );

      rect.left = x + 1;
      rect.right = x + cx;

      InflateRect( &rect, 1, 0 );

      InvalidateRect( hWnd, &rect, TRUE );

   //xif (lpSS->Redraw)
   //x   UpdateWindow( hWnd );
   //xelse
         lpSS->fInvalidated = TRUE;
   }
}

void
SS_InvalidateColRange( HWND hWnd,
                       LPSPREADSHEET lpSS,
                       SS_COORD ssCol1,
                       SS_COORD ssCol2 )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateColRange" );
   RECT          rect;
   zLONG         x;

   if ( mIs_hWndVisible( hWnd ) &&
        (ssCol1 < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen ||
         ((ssCol1 == -1 || ssCol1 <= lpSS->Col.LR) &&
          (ssCol2 == -1 || ssCol2 >= lpSS->Col.UL))) )
   {
      SS_GetClientRect( hWnd, &rect );

      if ( ssCol1 != -1 && ssCol1 >= lpSS->Col.UL )
      {
         x = SS_GetCellPosXInRect( lpSS, lpSS->Col.UL, ssCol1, &rect );
         rect.left = x;

         if ( ssCol1 == ssCol2 )
            rect.right = x + SS_GetColWidth( lpSS, ssCol1 ) + 1;
      }
      else
      if ( ssCol1 != -1 )
         rect.left =
            SS_GetCellPosXInRect( lpSS, lpSS->Col.UL,
                mMin( ssCol1, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen ), &rect );

      if ( ssCol2 != -1 && ssCol1 != ssCol2 && ssCol2 <= lpSS->Col.LR )
      {
         x = SS_GetCellPosXInRect( lpSS, lpSS->Col.UL, ssCol2 + 1, &rect );
         rect.right = x + 1;
      }

      InvalidateRect( hWnd, &rect, TRUE );

   //xif (lpSS->Redraw)
   //x   UpdateWindow( hWnd );
   //xelse
         lpSS->fInvalidated = TRUE;
   }
}

void
SS_InvalidateRange( HWND          hWnd,
                    LPSPREADSHEET lpSS,
                    SS_COORD      ssCol1,
                    SS_COORD      ssRow1,
                    SS_COORD      ssCol2,
                    SS_COORD      ssRow2 )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateRange" );
   if ( ssCol1 != SS_ALLCOLS && ssRow1 != SS_ALLROWS )
      SS_InvalidateCellRange( hWnd, lpSS, ssCol1, ssRow1, ssCol2, ssRow2 );
   else
   if ( ssCol1 != SS_ALLCOLS && ssRow1 == SS_ALLROWS )
      SS_InvalidateColRange( hWnd, lpSS, ssCol1, ssCol2 );
   else
   if ( ssCol1 == SS_ALLCOLS && ssRow1 != SS_ALLROWS )
      SS_InvalidateRowRange( hWnd, lpSS, ssRow1, ssRow2 );
}

void
SS_InvalidateRow( HWND          hWnd,
                  LPSPREADSHEET lpSS,
                  SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateRow" );
   RECT          rect;
   zLONG         y;
   zSHORT        cy;

   if ( lpSS->fNoInvalidate )
      return;

   if ( mIs_hWndVisible( hWnd ) &&
        SS_IsRowVisible( hWnd, lpSS, ssRow, SS_VISIBLE_PARTIAL ) )
   {
      SS_GetClientRect( hWnd, &rect );

      y = SS_GetCellPosY( lpSS, lpSS->Row.UL, ssRow );
      cy = SS_GetRowHeight( lpSS, ssRow );

      rect.top = y + 1;
      rect.bottom = y + cy;

      InflateRect( &rect, 0, 1 );

      InvalidateRect( hWnd, &rect, TRUE );

   //xif (lpSS->Redraw)
   //x   UpdateWindow( hWnd );
   //xelse
         lpSS->fInvalidated = TRUE;
   }
}

void
SSx_InvalidateRowArea( HWND hWnd,
                       LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSx_InvalidateRowArea" );
   if ( lpSS->fUseScrollArrows )
      lpSS->Redraw = FALSE;

   SS_InvalidateRowRange( hWnd, lpSS,
                          lpSS->Row.nHeaderCnt + lpSS->Row.Frozen, -1 );
   SS_SetVScrollBar( hWnd, lpSS );
   SS_TopRowChange( hWnd );

   if ( lpSS->fUseScrollArrows )
   {
      lpSS->Redraw = TRUE;
      SS_SetVScrollBar( hWnd, lpSS );
   //xUpdateWindow( hWnd );
   }
}

void
SS_InvalidateRowModeButtons( HWND          hWnd,
                             LPSPREADSHEET lpSS,
                             SS_COORD      ssColOld,
                             SS_COORD      ssColLeftOld,
                             SS_COORD      ssRowOld,
                             SS_COORD      ssRowTopOld )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateRowModeButtons" );
   if ( ssColOld == -1 )
      ssColOld = lpSS->Col.ssCurrAt;

   if ( ssColLeftOld == -1 )
      ssColLeftOld = lpSS->Col.UL;

   if ( ssRowOld == -1 )
      ssRowOld = lpSS->Row.ssCurrAt;

   if ( ssRowTopOld == -1 )
      ssRowTopOld = lpSS->Row.UL;

#ifndef SS_NOBUTTONDRAWMODE
   if ( ssRowOld != lpSS->Row.ssCurrAt )
      SSx_InvalidateButtons( hWnd, lpSS, ssColLeftOld, ssRowTopOld,
                             ssColLeftOld, ssRowOld, lpSS->Col.LR, ssRowOld );
#endif
}

void
SS_InvalidateRowRange( HWND hWnd,
                       LPSPREADSHEET lpSS,
                       SS_COORD ssRow1,
                       SS_COORD ssRow2 )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateRowRange" );
   RECT   rect;
   zLONG y;

   if ( mIs_hWndVisible( hWnd ) &&
        (ssRow1 < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen ||
         ((ssRow1 == -1 || ssRow1 <= lpSS->Row.LR) &&
          (ssRow2 == -1 || ssRow2 >= lpSS->Row.UL))) )
   {
      SS_GetClientRect( hWnd, &rect );

      if ( ssRow1 != -1 && ssRow1 >= lpSS->Row.UL )
      {
         y = SS_GetCellPosYInRect( lpSS, lpSS->Row.UL, ssRow1, &rect );
         rect.top = y;

         if ( ssRow1 == ssRow2 )
            rect.bottom = y + SS_GetRowHeight( lpSS, ssRow1 ) + 1;
      }
      else
      if ( ssRow1 != -1 )
         rect.top =
            SS_GetCellPosYInRect( lpSS, lpSS->Row.UL,
                mMin( ssRow1, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen ), &rect );

      if ( ssRow2 != -1 && ssRow1 != ssRow2 && ssRow2 <= lpSS->Row.LR )
      {
         y = SS_GetCellPosYInRect( lpSS, lpSS->Row.UL, ssRow2 + 1, &rect );
         rect.bottom = y + 1;
      }

      InvalidateRect( hWnd, &rect, TRUE );

   //xif (lpSS->Redraw)
   //x   UpdateWindow( hWnd );
   //xelse
         lpSS->fInvalidated = TRUE;
   }
}

void
SS_InvalidateScrollArea( HWND hWnd,
                         LPSPREADSHEET lpSS,
                         zSHORT lScrollArea )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_InvalidateScrollArea" );
   RECT  rect;

   if ( mIs_hWndVisible( hWnd ) )
   {
      SS_GetScrollArea( hWnd, lpSS, &rect, lScrollArea );
      InvalidateRect( hWnd, &rect, TRUE );

   //xif (lpSS->Redraw)
   //x   UpdateWindow( hWnd );
   //xelse
         lpSS->fInvalidated = TRUE;
   }
}

zBOOL
SS_IsCellValid( HWND          hWnd,
                LPSPREADSHEET lpSS,
                SS_COORD      ssColAt,
                SS_COORD      ssRowAt )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_IsCellValid" );
   zBOOL         Cancel = FALSE;
   zBOOL         Ret;

   if ( lpSS->fEditModePermanent )
   {
      if ( SS_GetLock( hWnd, lpSS, ssColAt, ssRowAt ) )
      {
         Ret = (zBOOL) SS_SendMsgCoords( hWnd, SSM_CELLLOCKED,
                                         GetDlgCtrlID( hWnd ),
                                         ssColAt - lpSS->Col.nHeaderCnt + 1,
                                         ssRowAt - lpSS->Row.nHeaderCnt + 1 );

         // Determine if the cell is still locked
         if ( SS_GetLock( hWnd, lpSS, ssColAt, ssRowAt ) )
         {
            if ( !Ret )
               SS_Beep( lpSS );

            Cancel = TRUE;
         }
      }
      else
      if ( lpSS->RestrictCols && ssColAt > lpSS->Col.DataCnt &&
           lpSS->RestrictRows && ssRowAt > lpSS->Row.DataCnt )
      {
         if ( !SS_SendMsgCoords( hWnd, SSM_COLROWRESTRICTED,
                                 GetDlgCtrlID( hWnd ),
                                 ssColAt - lpSS->Col.nHeaderCnt + 1,
                                 ssRowAt - lpSS->Row.nHeaderCnt + 1 ) )
         {
            SS_Beep( lpSS );
         }

         Cancel = TRUE;
      }
      else
      if ( lpSS->RestrictCols && ssColAt > lpSS->Col.DataCnt )
      {
         if ( !SS_SendMsg( hWnd, SSM_COLRESTRICTED, GetDlgCtrlID( hWnd ),
                           (zLONG)(ssColAt - lpSS->Col.nHeaderCnt + 1) ) )
         {
            SS_Beep( lpSS );
         }

         Cancel = TRUE;
      }
      else
      if ( lpSS->RestrictRows && ssRowAt > lpSS->Row.DataCnt )
      {
         if ( !SS_SendMsg( hWnd, SSM_ROWRESTRICTED, GetDlgCtrlID( hWnd ),
                           (zLONG)(ssRowAt - lpSS->Row.nHeaderCnt + 1) ) )
         {
            SS_Beep( lpSS );
         }

         Cancel = TRUE;
      }
   }

   return( !Cancel );
}

zBOOL
SS_LeaveCell( HWND          hWnd,
              LPSPREADSHEET lpSS,
              SS_COORD      ssColOld,
              SS_COORD      ssColLeftOld,
              SS_COORD      ssRowOld,
              SS_COORD      ssRowTopOld,
              WORD          fFlag )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_LeaveCell" );
   SS_LEAVECELL  LeaveCell;
   zBOOL         bCancel = FALSE;
   zBOOL         fRedraw;
   zBOOL         Ret;
   SS_COORD      ssColTemp;
   SS_COORD      ssColLeftTemp;
   SS_COORD      ssRowTemp;
   SS_COORD      ssRowTopTemp;
   SS_COORD      ssColAtSave;
   SS_COORD      ssColLeftSave;
   SS_COORD      ssRowAtSave;
   SS_COORD      ssRowTopSave;

   if ( lpSS->wOpMode == SS_OPMODE_READONLY )
      return( TRUE );

   if ( ssColOld == -1 )
      LeaveCell.ssColCurrent = lpSS->Col.ssCurrAt;
   else
      LeaveCell.ssColCurrent = ssColOld;

   if ( ssRowOld == -1 )
      LeaveCell.ssRowCurrent = lpSS->Row.ssCurrAt;
   else
      LeaveCell.ssRowCurrent = ssRowOld;

   LeaveCell.ssColCurrent = LeaveCell.ssColCurrent - lpSS->Col.nHeaderCnt + 1;
   LeaveCell.ssRowCurrent = LeaveCell.ssRowCurrent - lpSS->Row.nHeaderCnt + 1;

   if ( fFlag & zLEAVE_CELL_KILLFOCUS )
   {
      LeaveCell.ssColNew = -1;
      LeaveCell.ssRowNew = -1;
   }
   else
   {
      LeaveCell.ssColNew = lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1;
      LeaveCell.ssRowNew = lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1;
   }

   if ( LeaveCell.ssColCurrent == LeaveCell.ssColNew &&
        LeaveCell.ssRowCurrent == LeaveCell.ssRowNew )
   {
      return( TRUE );
   }

   ssColTemp = lpSS->Col.ssCurrAt;
   ssColLeftTemp = lpSS->Col.UL;
   ssRowTemp = lpSS->Row.ssCurrAt;
   ssRowTopTemp = lpSS->Row.UL;

// if ( LeaveCell.ssColCurrent == LeaveCell.ssColNew &&  redundant code!!!
//      LeaveCell.ssRowCurrent == LeaveCell.ssRowNew )   redundant code!!!
// {                                                     redundant code!!!
//    return( TRUE );                                    redundant code!!!
// }                                                     redundant code!!!

   lpSS->fWindowPainted = FALSE;

   if ( lpSS->fEditModePermanent )
   {
      if ( SS_GetLock( hWnd, lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt ) )
      {
         Ret = (zBOOL)
               SS_SendMsgCoords( hWnd, SSM_CELLLOCKED,
                                 GetDlgCtrlID( hWnd ),
                                 lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1,
                                 lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1 );

         // Check to see if the cell is still locked.
         if ( SS_GetLock( hWnd, lpSS, lpSS->Col.ssCurrAt,
                          lpSS->Row.ssCurrAt ) )
         {
            if ( !Ret )
               SS_Beep( lpSS );

            bCancel = TRUE;
         }
      }
      else
      if ( lpSS->RestrictCols &&
           lpSS->Col.ssCurrAt > lpSS->Col.DataCnt &&
           lpSS->RestrictRows &&
           lpSS->Row.ssCurrAt > lpSS->Row.DataCnt )
      {
         if ( !SS_SendMsgCoords( hWnd, SSM_COLROWRESTRICTED,
                                 GetDlgCtrlID( hWnd ),
                                 lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1,
                                 lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1 ) )
         {
            SS_Beep( lpSS );
         }

         bCancel = TRUE;
      }
      else
      if ( lpSS->RestrictCols && lpSS->Col.ssCurrAt > lpSS->Col.DataCnt )
      {
         if ( !SS_SendMsg( hWnd, SSM_COLRESTRICTED,
                           GetDlgCtrlID( hWnd ),
                           (zLONG)(lpSS->Col.ssCurrAt - lpSS->Col.nHeaderCnt + 1) ) )
         {
            SS_Beep( lpSS );
         }

         bCancel = TRUE;
      }
      else
      if ( lpSS->RestrictRows && lpSS->Row.ssCurrAt > lpSS->Row.DataCnt )
      {
         if ( !SS_SendMsg( hWnd, SSM_ROWRESTRICTED, GetDlgCtrlID( hWnd ),
                           (zLONG)(lpSS->Row.ssCurrAt - lpSS->Row.nHeaderCnt + 1) ) )
         {
            SS_Beep( lpSS );
         }

         bCancel = TRUE;
      }
   }

   if ( bCancel == FALSE )
   {
      lpSS->wMessageBeingSent++;

      // We don't care if we are in ROWMODE, if we are leaving the
      // cell, let's notify the appropriate authorities.
#if 0
      if ( lpSS->wOpMode == SS_OPMODE_ROWMODE &&
           ssRowOld != -1 && ssRowOld != lpSS->Row.ssCurrAt )
      {
         SS_LEAVEROW  LeaveRow;

         zmemset( &LeaveRow, 0, zsizeof( SS_LEAVEROW ) );
         LeaveRow.ssRowCurrent = LeaveCell.ssRowCurrent;
         LeaveRow.ssRowNew = LeaveCell.ssRowNew;
         LeaveRow.fCancel = 0;

         if ( lpSS->fRowModeEditing )
         {
            LeaveRow.fRowEdited = TRUE;
            LeaveRow.fRowChangeMade = lpSS->fRowModeChangeMade;
            LeaveRow.fAllCellsHaveData = SS_AllCellsHaveData( hWnd, lpSS,
                                                              ssRowOld );

            if ( lpSS->DataRowCntOld < lpSS->Row.DataCnt )
               LeaveRow.fAddRow = TRUE;
         }

         if ( lpSS->Row.ssCurrAt > lpSS->Row.DataCnt )
            LeaveRow.fRowNewBeyondLastDataRow = TRUE;

         ssRowAtSave = lpSS->Row.ssCurrAt;
         ssRowTopSave = lpSS->Row.UL;

         if ( ssRowOld != -1 )
            lpSS->Row.ssCurrAt = ssRowOld;

         if ( ssRowTopOld != -1 )
            lpSS->Row.UL = ssRowTopOld;

         fRedraw = lpSS->Redraw;
         lpSS->Redraw = FALSE;

         lpSS->fSetActiveCellCalled = FALSE;

         bCancel = (zBOOL) SS_SendMsg( hWnd, SSM_LEAVEROW,
                                       GetDlgCtrlID( hWnd ),
                                       (zLONG)(LPSS_LEAVEROW) &LeaveRow );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( FALSE );

         lpSS->Redraw = fRedraw;

         if ( lpSS->fSetActiveCellCalled == 0 )
         {
            lpSS->Row.ssCurrAt = ssRowAtSave;
            lpSS->Row.UL = ssRowTopSave;
         }
         else
            lpSS->fSetActiveCellCalled = FALSE;

         if ( LeaveRow.fCancel )
            bCancel = TRUE;

         if ( bCancel == FALSE && lpSS->fRowModeEditing )
         {
            lpSS->fRowModeEditing = FALSE;

#ifndef SS_NOBUTTONDRAWMODE
            SS_InvalidateRowModeButtons( hWnd, lpSS, ssColOld, ssColLeftOld,
                                         ssRowOld, ssRowTopOld );
#endif
         }
      }
      else
#endif
      {
         ssColAtSave = lpSS->Col.ssCurrAt;
         ssColLeftSave = lpSS->Col.UL;
         ssRowAtSave = lpSS->Row.ssCurrAt;
         ssRowTopSave = lpSS->Row.UL;

         if ( ssColOld != -1 )
            lpSS->Col.ssCurrAt = ssColOld;

         if ( ssColLeftOld != -1 )
            lpSS->Col.UL = ssColLeftOld;

         if ( ssRowOld != -1 )
            lpSS->Row.ssCurrAt = ssRowOld;

         if ( ssRowTopOld != -1 )
            lpSS->Row.UL = ssRowTopOld;

         fRedraw = lpSS->Redraw;
         lpSS->Redraw = FALSE;

         lpSS->fSetActiveCellCalled = FALSE;

// #ifdef ZEIDON
         // inform the parent that we are about to leave the cell
         SS_LeaveCellNotify( hWnd );
// #endif
      // bCancel = (zBOOL) SS_SendMsg( hWnd, SSM_LEAVECELL,
      //                               GetDlgCtrlID( hWnd ),
      //                               (zLONG)(LPSS_LEAVECELL) &LeaveCell );

         if ( mIs_hWnd( hWnd ) == FALSE )
            return( FALSE );

         lpSS->Redraw = fRedraw;

         if ( lpSS->fSetActiveCellCalled == 0 )
         {
            lpSS->Col.ssCurrAt = ssColAtSave;
            lpSS->Col.UL = ssColLeftSave;
            lpSS->Row.ssCurrAt = ssRowAtSave;
            lpSS->Row.UL = ssRowTopSave;
         }
         else
            lpSS->fSetActiveCellCalled = FALSE;
      }

      lpSS->wMessageBeingSent--;
   }

#ifndef SS_NOBUTTONDRAWMODE
   if ( bCancel == FALSE && lpSS->wButtonDrawMode )
      if ( lpSS->wOpMode != SS_OPMODE_ROWMODE )
         SS_InvalidateButtons( hWnd, lpSS, ssColOld, ssColLeftOld,
                               ssRowOld, ssRowTopOld );
#endif

   if ( ssColLeftTemp != lpSS->Col.UL || ssRowTopTemp != lpSS->Row.UL )
   {
      SS_HiliteCell( hWnd, TRUE );
      return( FALSE );
   }

   if ( ssColTemp != lpSS->Col.ssCurrAt || ssRowTemp != lpSS->Row.ssCurrAt )
   {
      if ( bCancel )
         SS_HiliteCell( hWnd, TRUE );

      return( !bCancel );
   }

   if ( bCancel )
   {
      if ( ssColOld != -1 )
         lpSS->Col.ssCurrAt = ssColOld;

      if ( ssColLeftOld != -1 )
         lpSS->Col.UL = ssColLeftOld;

      if ( ssRowOld != -1 )
         lpSS->Row.ssCurrAt = ssRowOld;

      if ( ssRowTopOld != -1 )
         lpSS->Row.UL = ssRowTopOld;

      SS_HiliteCell( hWnd, TRUE );
   }
// #ifdef ZEIDON
   // inform the parent that we have entered the new cell
   if ( (fFlag & zLEAVE_CELL_COLSELECTED) == 0 )
   {
      SS_EnterCellNotify( hWnd );
   }
// #endif

   return( !bCancel );
}

void
SS_LeftColChange( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_LeftColChange" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_SendMsgCommand( hWnd, 0, SSN_LEFTCOLCHANGE, FALSE );
// DrUnlockTaskMemory( lpSS );
}

void
SS_Scroll( HWND    hWnd,
           zLONG   lScrollX,
           zLONG   lScrollY,
           LPRECT  rect )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_Scroll" );
   LPSPREADSHEET   lpSS;
   SS_CLRTBLITEM BackClrTblItem;
   HRGN            hRgn;
   HRGN            hRgn2;
   RECT            rectUpd;
   HDC             hDC;

   hDC = GetDC( hWnd );
   if ( (lScrollX == 0 && abs( lScrollY ) >= rect->bottom - rect->top) ||
        (lScrollY == 0 && abs( lScrollX ) >= rect->right - rect->left) )
   {
      lpSS = SS_Lock( hWnd );
      SS_GetColorItem( lpSS, &BackClrTblItem, lpSS->Color.BackgroundId );
//    DrUnlockTaskMemory( lpSS );
      InvalidateRect( hWnd, rect, TRUE );
   }
   else
   {
      if ( (hRgn = CreateRectRgn( 0, 0, 0, 0 )) != 0 )
      {
         if ( GetUpdateRgn( hWnd, hRgn, FALSE ) != NULLREGION )
         {
            if ( (hRgn2 = CreateRectRgn( rect->left, rect->top,
                                         rect->right, rect->bottom )) != 0 )
            {
               if ( CombineRgn( hRgn, hRgn, hRgn2, RGN_AND ) != NULLREGION )
               {
                  RECT  rectTemp;

                  GetRgnBox( hRgn, &rectTemp );
                  ValidateRect( hWnd, &rectTemp );

                  OffsetRgn( hRgn, lScrollX, lScrollY );
                  InvalidateRgn( hWnd, hRgn, TRUE );
               }

               DeleteObject( hRgn2 );
            }
         }

         DeleteObject( hRgn );
      }

      lpSS = SS_Lock( hWnd );

      if ( lScrollX )
         lpSS->bHPixelOffset = (abs( lScrollX ) + lpSS->bHPixelOffset) % 2;

      if ( lScrollY )
         lpSS->bVPixelOffset = (abs( lScrollY ) + lpSS->bVPixelOffset) % 2;

//    DrUnlockTaskMemory( lpSS );
//    TraceThisRect( "SS_Scroll", rect );
//    TraceLineI( "  xScroll ", xScroll );
//    TraceLineI( "  yScroll ", yScroll );
      ScrollDC( hDC, lScrollX, lScrollY, rect, rect, 0, &rectUpd );
      InvalidateRect( hWnd, &rectUpd, TRUE );
//    TraceThisRect( "SS_Scroll Upd", &rectUpd );
   }

   ReleaseDC( hWnd, hDC );
}

zBOOL
SS_ScrollEnd( HWND          hWnd,
              LPSPREADSHEET lpSS,
              WORD          fFlags )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollEnd" );
   zSHORT        nSSSize;
   zLONG         lCellPos;
   zLONG         lRectSize;
   RECT          rect;
   zBOOL         bRow;
   zBOOL         bMoveActiveCell;
   SS_COORD      ssCurrAt;
   SS_COORD      ssCurrAtOld;
   SS_COORD      ULOld;
   SS_COORD      Cnt;
   SS_COORD      BlockCellCurrentPos;
   LPSS_ROWCOL   lpRC;
   zSHORT        (*lpfnGetSize)( LPSPREADSHEET, SS_COORD );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return( FALSE );

   bRow = ((fFlags & SS_F_SCROLL_ROW) == SS_F_SCROLL_ROW);
   bMoveActiveCell = ((fFlags & SS_F_MOVEACTIVECELL) == SS_F_MOVEACTIVECELL);

   if ( bRow )
   {
      lpRC = &(lpSS->Col);
      lpfnGetSize = SS_GetColWidth;
   }
   else // end of col
   {
      lpRC = &(lpSS->Row);
      lpfnGetSize = SS_GetRowHeight;
   }

   ssCurrAtOld = lpRC->ssCurrAt;
   ULOld = lpRC->UL;

   BlockCellCurrentPos = lpRC->BlockCellCurrentPos;

   if ( fFlags & SS_F_SCROLLMAX )
   {
      if ( bRow )
         Cnt = SS_GetColsMax( lpSS ) - 1;
      else
         Cnt = SS_GetRowsMax( lpSS ) - 1;
   }
   else
      Cnt = lpRC->DataCnt - 1;

   if ( Cnt == lpRC->nHeaderCnt - 1 && Cnt < lpRC->Max )
   {
      Cnt++;

      while ( Cnt < lpRC->Max &&
              (lpfnGetSize( lpSS, Cnt ) == 0 ||
               (lpSS->fEditModePermanent &&
                SS_GetLock( hWnd, lpSS, bRow ? Cnt : lpSS->Col.ssCurrAt,
                            bRow ? lpSS->Row.ssCurrAt : Cnt ))) )
      {
         Cnt++;
      }
   }

   while ( Cnt > lpRC->nHeaderCnt &&
           (lpfnGetSize( lpSS, Cnt ) == 0 ||
            (lpSS->fEditModePermanent &&
             SS_GetLock( hWnd, lpSS, bRow ? Cnt : lpSS->Col.ssCurrAt,
                         bRow ? lpSS->Row.ssCurrAt : Cnt ))) )
   {
      Cnt--;
   }

   if ( Cnt >= lpRC->UL && Cnt <= lpRC->LRAllVis )
   {
      if ( bMoveActiveCell )
      {
         if ( lpSS->ModeBlock )
         {
            if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bRow )
               lpSS->Row.ssCurrAt = Cnt;

            SSx_SelectBlock( hWnd, lpSS,
                             bRow ? Cnt : lpSS->Col.BlockCellCurrentPos,
                             bRow ? lpSS->Row.BlockCellCurrentPos : Cnt );
         }
         else
         {
            SS_HiliteCell( hWnd, FALSE );
            lpRC->ssCurrAt = Cnt;

            if ( SS_LeaveCell( hWnd, lpSS, bRow ? ssCurrAtOld : -1, -1,
                               bRow ? -1 : ssCurrAtOld, -1, 0 ) )
            {
               SS_HiliteCell( hWnd, TRUE );
            }
         }
      }
   }
   else
   {
      SS_GetClientRect( hWnd, &rect );

      if ( bRow )
      {
         lCellPos = SS_GetCellPosX( lpSS, lpRC->UL, lpRC->UL );
         lRectSize = rect.right - rect.left;
      }
      else
      {
         lCellPos = SS_GetCellPosY( lpSS, lpRC->UL, lpRC->UL );
         lRectSize = rect.bottom - rect.top;
      }

      for ( ssCurrAt = Cnt + 1; ssCurrAt > lpRC->nHeaderCnt +
            lpRC->Frozen && lCellPos <= lRectSize; )
      {
         if ( --ssCurrAt < lpRC->nHeaderCnt + lpRC->Frozen )
            break;

         nSSSize = lpfnGetSize( lpSS, ssCurrAt );

         if ( lCellPos + nSSSize > lRectSize )
         {
            ssCurrAt++;
            break;
         }

         lCellPos += nSSSize;
      }

      SS_HiliteCell( hWnd, FALSE );
      lpRC->UL = mMax( lpRC->nHeaderCnt + lpRC->Frozen, ssCurrAt );

      if ( bMoveActiveCell )
      {
         if ( lpSS->ModeBlock )
         {
            BlockCellCurrentPos = Cnt;
            if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bRow )
               lpSS->Row.ssCurrAt = Cnt;
         }
         else
         {
#if 0
            if ( bRow )
               lpRC->ssCurrAt = Cnt;
            else
               lpRC->ssCurrAt = lpRC->UL;
#endif

            lpRC->ssCurrAt = Cnt;

            if ( !SS_LeaveCell( hWnd, lpSS, bRow ? ssCurrAtOld : -1,
                                bRow ? ULOld : -1,
                                bRow ? -1 : ssCurrAtOld,
                                bRow ? -1 : ULOld, 0 ) )
               return( TRUE );
         }
      }

      if ( lpRC->UL != ULOld )
      {
         if ( bRow )
            SSx_InvalidateColArea( hWnd, lpSS );
         else
         {
            lpSS->wScrollDirection = SS_VSCROLL_END;
            SSx_InvalidateRowArea( hWnd, lpSS );
         }

         if ( lpSS->ModeBlock && bMoveActiveCell )
            SSx_SelectBlock( hWnd, lpSS,
                bRow ? BlockCellCurrentPos : lpSS->Col.BlockCellCurrentPos,
                bRow ? lpSS->Row.BlockCellCurrentPos : BlockCellCurrentPos );
      }

      if ( !bRow )
      {
         if ( bMoveActiveCell && lpSS->ModeBlock == 0 )
            SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );

         if ( lpSS->Row.UL != ULOld )
            lpSS->wScrollDirection = SS_VSCROLL_END;
      }
   }

   return( TRUE );
}

zBOOL
SS_ScrollHome( HWND          hWnd,
               LPSPREADSHEET lpSS,
               WORD          fFlags )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollHome" );
   zBOOL         bRow;
   zBOOL         bMoveActiveCell;
   SS_COORD      PosAtOld;
   SS_COORD      ssCurrAtTemp;
   SS_COORD      ULTemp;
   SS_COORD      BlockCellCurrentPos;
   SS_COORD      MaxCnt;
   LPSS_ROWCOL   lpRC;
   zSHORT        (*lpfnGetSize)( LPSPREADSHEET, SS_COORD );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return( FALSE );

   bRow = ((fFlags & SS_F_SCROLL_ROW) == SS_F_SCROLL_ROW);
   bMoveActiveCell = ((fFlags & SS_F_MOVEACTIVECELL) == SS_F_MOVEACTIVECELL);

   if ( bRow )
   {
      lpRC = &(lpSS->Col );
      MaxCnt = SS_GetColsMax( lpSS );
      lpfnGetSize = SS_GetColWidth;
   }
   else // home of col
   {
      lpRC = &(lpSS->Row);
      MaxCnt = SS_GetRowsMax( lpSS );
      lpfnGetSize = SS_GetRowHeight;
   }

   PosAtOld = lpRC->ssCurrAt;
   ULTemp = lpRC->UL;

   BlockCellCurrentPos = lpRC->BlockCellCurrentPos;

   if ( bMoveActiveCell )
   {
      ssCurrAtTemp = lpRC->nHeaderCnt;
      while ( ssCurrAtTemp < MaxCnt - 1 && (lpfnGetSize( lpSS, ssCurrAtTemp ) == 0 ||
              (lpSS->fEditModePermanent &&
               SS_GetLock( hWnd, lpSS, bRow ? ssCurrAtTemp : lpSS->Col.ssCurrAt,
                           bRow ? lpSS->Row.ssCurrAt : ssCurrAtTemp ))) )
      {
         ssCurrAtTemp++;
      }
   }

   if ( bMoveActiveCell &&
        lpRC->UL == lpRC->nHeaderCnt + lpRC->Frozen &&
        lpRC->ssCurrAt <= lpRC->LR )
   {
      if ( lpSS->ModeBlock )
      {
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bRow )
            lpSS->Row.ssCurrAt = ssCurrAtTemp;

         SSx_SelectBlock( hWnd, lpSS,
                          bRow ? ssCurrAtTemp : lpSS->Col.BlockCellCurrentPos,
                          bRow ? lpSS->Row.BlockCellCurrentPos : ssCurrAtTemp );
      }
      else
      {
         SS_HiliteCell( hWnd, FALSE );
         lpRC->ssCurrAt = ssCurrAtTemp;

         if ( SS_LeaveCell( hWnd, lpSS, bRow ? PosAtOld : -1, -1,
                            bRow ? -1 : PosAtOld, -1, 0 ) )
            SS_HiliteCell( hWnd, TRUE );
      }
   }
   else
   if ( lpRC->UL > lpRC->nHeaderCnt + lpRC->Frozen )
   {
      SS_HiliteCell( hWnd, FALSE );

      lpRC->UL = lpRC->nHeaderCnt + lpRC->Frozen;
      while ( lpSS->Col.UL < MaxCnt - 1 && lpfnGetSize( lpSS, lpRC->UL ) == 0 )
         lpRC->UL++;

      if ( bMoveActiveCell )
      {
         if ( lpSS->ModeBlock )
         {
            BlockCellCurrentPos = ssCurrAtTemp;

            if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bRow )
               lpSS->Row.ssCurrAt = BlockCellCurrentPos;
         }
         else
         {
            lpRC->ssCurrAt = ssCurrAtTemp;

            if ( !SS_LeaveCell( hWnd, lpSS, bRow ? PosAtOld : -1,
                                bRow ? ULTemp : -1,
                                bRow ? -1 : PosAtOld,
                                bRow ? -1 : ULTemp, 0 ) )
            {
               return( TRUE );
            }
         }
      }

      if ( bRow )
         SSx_InvalidateColArea( hWnd, lpSS );
      else
      {
         lpSS->wScrollDirection = SS_VSCROLL_HOME;
         SSx_InvalidateRowArea( hWnd, lpSS );
      }

      if ( lpSS->ModeBlock && bMoveActiveCell )
         SSx_SelectBlock( hWnd, lpSS,
                bRow ? BlockCellCurrentPos : lpSS->Col.BlockCellCurrentPos,
                bRow ? lpSS->Row.BlockCellCurrentPos : BlockCellCurrentPos );
   }
   else
      SS_HiliteCell( hWnd, TRUE );

   return( TRUE );
}

void
SS_ScrollLineLR( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 WORD          fFlags )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollLineLR" );
   zLONG         lScrollInc;
   zBOOL         bRight;
   zBOOL         bMoveActiveCell;
   zBOOL         Found;
   SS_COORD      BlockCellCurrentPos;
   SS_COORD      PosAtOld;
   SS_COORD      ULTemp;
   SS_COORD      ULOld;
   SS_COORD      PosLast;
   SS_COORD      Pos1AtTemp;
   SS_COORD      Pos2AtTemp;
   SS_COORD      MaxCnt;
   LPSS_ROWCOL   lpRC1;        //either lpRow or lpCol structure
   LPSS_ROWCOL   lpRC2;        //inverse of lpRC1
   zSHORT        (*lpfnGetSize)( LPSPREADSHEET, SS_COORD );
   zBOOL         (*lpfnIsVisible)( HWND, LPSPREADSHEET, SS_COORD, zSHORT );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return;

   bRight = ((fFlags & SS_F_SCROLL_RIGHT) == SS_F_SCROLL_RIGHT);
   bMoveActiveCell = ((fFlags & SS_F_MOVEACTIVECELL) == SS_F_MOVEACTIVECELL);

   if ( bRight )
   {
      MaxCnt = SS_GetColsMax( lpSS );
      lpRC1 = &( lpSS->Col );
      lpRC2 = &( lpSS->Row );
      lpfnGetSize = SS_GetColWidth;
      lpfnIsVisible = SS_IsColVisible;
   }
   else // scroll down
   {
      MaxCnt = SS_GetRowsMax( lpSS );
      lpRC1 = &(lpSS->Row);
      lpRC2 = &(lpSS->Col);
      lpfnGetSize = SS_GetRowHeight;
      lpfnIsVisible = SS_IsRowVisible;
   }

   if ( lpSS->ModeBlock )
   {
      BlockCellCurrentPos = lpRC1->BlockCellCurrentPos;
      Pos1AtTemp = lpRC1->BlockCellCurrentPos;
      Pos2AtTemp = lpRC2->BlockCellCurrentPos;
   }
   else
   {
      Pos1AtTemp = lpRC1->ssCurrAt;
      Pos2AtTemp = lpRC2->ssCurrAt;

      if ( bMoveActiveCell )
        SS_ResetBlock( hWnd, TRUE );
   }


   PosAtOld = lpRC1->ssCurrAt;
   ULTemp = lpRC1->UL;
   ULOld = lpRC1->UL;
   lScrollInc = 0;

   if ( bMoveActiveCell )
   {
      if ( !SS_IsCellVisible( hWnd, lpSS,
                     bRight ? Pos1AtTemp : Pos2AtTemp,
                     bRight ? Pos2AtTemp : Pos1AtTemp, SS_VISIBLE_PARTIAL ) )
      {
         SS_HiliteCell( hWnd, FALSE );

         while ( Pos1AtTemp < MaxCnt - 1 )
         {
            Pos1AtTemp++;
            if ( (lpfnGetSize( lpSS, Pos1AtTemp ) > 0) &&
                 (lpSS->fEditModePermanent == 0 ||
                  !SS_GetLock( hWnd, lpSS,
                               bRight ? Pos1AtTemp : Pos2AtTemp,
                               bRight ? Pos2AtTemp : Pos1AtTemp )) )
            {
               if ( lpSS->ModeBlock )
                  BlockCellCurrentPos = Pos1AtTemp;
               else
                  lpRC1->ssCurrAt = Pos1AtTemp;

               break;
            }
         }

         if ( lpSS->ModeBlock )
            SS_ShowCell( hWnd, SS_SHOW_NEAREST,
              bRight ? BlockCellCurrentPos : lpSS->Col.BlockCellCurrentPos,
              bRight ? lpSS->Row.BlockCellCurrentPos : BlockCellCurrentPos );
         else
         if ( SS_LeaveCell( hWnd, lpSS,
                            bRight ? PosAtOld : -1, -1,
                            bRight ? -1 : PosAtOld, -1, 0 ) )
         {
            SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
         }

         if ( bRight )
            return;
      }
      else
      {
         Found = FALSE;
         while ( Pos1AtTemp < MaxCnt - 1 )
         {
            if ( Pos1AtTemp == lpRC1->nHeaderCnt + lpRC1->Frozen - 1 )
               Pos1AtTemp = mMax( lpRC1->UL, lpRC1->nHeaderCnt + lpRC1->Frozen );
            else
               Pos1AtTemp++;

            while ( !lpfnIsVisible( hWnd, lpSS, Pos1AtTemp, SS_VISIBLE_ALL ) &&
                    Pos1AtTemp > lpRC1->UL )
            {
               SS_HiliteCell( hWnd, FALSE );
               lScrollInc += lpfnGetSize( lpSS, lpRC1->UL );
               lpRC1->UL++;
            }

            if ( lpfnGetSize( lpSS, Pos1AtTemp ) > 0 &&
                 (lpSS->fEditModePermanent == 0 ||
                  !SS_GetLock( hWnd, lpSS, bRight ? Pos1AtTemp : Pos2AtTemp,
                               bRight ? Pos2AtTemp : Pos1AtTemp )) )
            {
               if ( lpSS->ModeBlock )
                  BlockCellCurrentPos = Pos1AtTemp;
               else
               {
                  SS_HiliteCell( hWnd, FALSE );
                  lpRC1->ssCurrAt = Pos1AtTemp;
               }

               Found = TRUE;
               break;
            }
         }

         if ( Found == 0 )
         {
            lpRC1->UL = ULTemp;
            if ( !bRight )
               lScrollInc = 0;
         }
      }
   }
   else
   {
      if ( lpSS->fScrollBarMaxAlign )
      {
         if ( bRight )
            PosLast = SS_GetLastPageLeftCol( hWnd, lpSS );
         else
            PosLast = SS_GetLastPageTopRow( hWnd, lpSS );
      }
      else
         PosLast = MaxCnt - 1;

      while ( ULTemp < PosLast )
      {
         ULTemp++;
         if ( lpfnGetSize( lpSS, ULTemp ) > 0 )
         {
            SS_HiliteCell( hWnd, FALSE );
            lScrollInc += lpfnGetSize( lpSS, lpRC1->UL );
            lpRC1->UL = ULTemp;
            break;
         }
      }
   }

   if ( lpSS->ModeBlock || !bMoveActiveCell ||
        SS_LeaveCell( hWnd, lpSS,
                      bRight ? PosAtOld : -1,
                      bRight ? ULOld : -1,
                      bRight ? -1 : PosAtOld,
                      bRight ? -1 : ULOld, 0 ) )
   {
      if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bRight && lpSS->ModeBlock )
         lpSS->Row.ssCurrAt = BlockCellCurrentPos;

      if ( lScrollInc )
      {
         if ( bRight )
            SS_HScroll( hWnd, lScrollInc );
         else
         {
// #ifdef ZEIDON
            if ( (fFlags & SS_F_SKIP_UNHILITE) == FALSE )
            {
               SS_HiliteCell( hWnd, FALSE );
            }
// #endif
            SS_VScroll( hWnd, lScrollInc );
         }
      }
      else
         SS_HiliteCell( hWnd, TRUE );

      if ( lpSS->ModeBlock && bMoveActiveCell )
         SSx_SelectBlock( hWnd, lpSS,
              bRight ? BlockCellCurrentPos : lpSS->Col.BlockCellCurrentPos,
              bRight ? lpSS->Row.BlockCellCurrentPos : BlockCellCurrentPos );
      else
      if ( bRight == 0 && lpSS->ModeBlock == 0 && bMoveActiveCell &&
           PosAtOld == lpRC1->ssCurrAt )
      {
         SS_SendMsgCommand( hWnd, 0, SSN_EXITNEXT, FALSE );
      }
   }

   if ( !bRight && lpRC1->UL != ULOld )
      lpSS->wScrollDirection = SS_VSCROLL_DOWN;
}

void
SS_ScrollLineUL( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 WORD          fFlags )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollLineUL" );
   zSHORT        k;
   zLONG         lScrollInc;
   zSHORT        nSizeTemp;
   zBOOL         bLeft;
   zBOOL         bMoveActiveCell;
   SS_COORD      BlockCellCurrentPos;
   SS_COORD      ULTemp;
   SS_COORD      ULOld;
   SS_COORD      ULNew;
   SS_COORD      PosAtOld;
   SS_COORD      Pos1AtTemp;
   SS_COORD      Pos2AtTemp;
   LPSS_ROWCOL   lpRC1;        //either lpRow or lpCol structure
   LPSS_ROWCOL   lpRC2;        //opposite of lpRC1
   zSHORT        (*lpfnGetSize)( LPSPREADSHEET, SS_COORD );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return;

   bLeft = ((fFlags & SS_F_SCROLL_LEFT) == SS_F_SCROLL_LEFT);
   bMoveActiveCell = ((fFlags & SS_F_MOVEACTIVECELL) == SS_F_MOVEACTIVECELL);

   if ( bLeft )
   {
      lpRC1 = &(lpSS->Col);
      lpRC2 = &(lpSS->Row);
      lpfnGetSize = SS_GetColWidth;
   }
   else // scroll up
   {
      lpRC1 = &(lpSS->Row);
      lpRC2 = &(lpSS->Col);
      lpfnGetSize = SS_GetRowHeight;
   }

   if ( lpSS->ModeBlock )
   {
      BlockCellCurrentPos = lpRC1->BlockCellCurrentPos;
      Pos2AtTemp = lpRC2->BlockCellCurrentPos;
      Pos1AtTemp = lpRC1->BlockCellCurrentPos;
   }
   else
   {
      PosAtOld = lpRC1->ssCurrAt;
      Pos2AtTemp = lpRC2->ssCurrAt;
      Pos1AtTemp = lpRC1->ssCurrAt;

      if ( bMoveActiveCell )
         SS_ResetBlock( hWnd, TRUE );
   }

   if ( bMoveActiveCell &&
        !SS_IsCellVisible( hWnd, lpSS,
                      bLeft ? Pos1AtTemp : Pos2AtTemp,
                      bLeft ? Pos2AtTemp : Pos1AtTemp, SS_VISIBLE_PARTIAL ) )
   {
      SS_HiliteCell( hWnd, FALSE );

      if ( lpSS->ModeBlock )
         Pos1AtTemp = lpRC1->BlockCellCurrentPos;
      else
         Pos1AtTemp = lpRC1->ssCurrAt;

      while ( Pos1AtTemp > lpRC1->nHeaderCnt )
      {
         Pos1AtTemp--;

         if ( bLeft )
            k = (SS_GetColWidth( lpSS, Pos1AtTemp ) > 0);
         else
            k = (SS_GetRowHeight( lpSS, Pos1AtTemp ) > 0 &&
                (lpSS->fEditModePermanent == 0 ||
                 !SS_GetLock( hWnd, lpSS, lpRC2->ssCurrAt, Pos1AtTemp )));

         if ( k )
         {
            if ( lpSS->ModeBlock )
               BlockCellCurrentPos = Pos1AtTemp;
            else
               lpRC1->ssCurrAt = Pos1AtTemp;

            break;
         }
      }

      if ( lpSS->ModeBlock )
         SS_ShowCell( hWnd, SS_SHOW_NEAREST,
               bLeft ? BlockCellCurrentPos : lpSS->Col.BlockCellCurrentPos,
               bLeft ? lpSS->Row.BlockCellCurrentPos : BlockCellCurrentPos );
      else
      if ( SS_LeaveCell( hWnd, lpSS, bLeft ? PosAtOld : -1, -1,
                         bLeft ? -1 : PosAtOld, -1, 0 ) )
      {
         SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
      }
   }
   else
   {
      if ( lpSS->ModeBlock )
         Pos1AtTemp = lpRC1->BlockCellCurrentPos;
      else
         Pos1AtTemp = lpRC1->ssCurrAt;

      ULTemp = lpRC1->UL;
      ULNew = lpRC1->UL;
      ULOld = ULTemp;
      lScrollInc = 0;

      if ( bMoveActiveCell )
      {
         while ( Pos1AtTemp > lpRC1->nHeaderCnt )
         {
            Pos1AtTemp--;
            if ( Pos1AtTemp < lpRC1->UL &&
                 ULTemp > lpRC1->nHeaderCnt + lpRC1->Frozen )
            {
               ULTemp--;
               if ( (nSizeTemp = lpfnGetSize( lpSS, ULTemp )) != 0 )
               {
                  lScrollInc -= nSizeTemp;
                  ULNew = ULTemp;
               }
            }

            if ( lpfnGetSize( lpSS, Pos1AtTemp ) > 0 )
            {
               if ( lpSS->fEditModePermanent == 0 ||
                    !SS_GetLock( hWnd, lpSS,
                                 bLeft ? Pos1AtTemp : lpRC2->ssCurrAt,
                                 bLeft ? lpRC2->ssCurrAt : Pos1AtTemp ) )
               {
                  if ( lpSS->ModeBlock )
                     BlockCellCurrentPos = Pos1AtTemp;
                  else
                  {
                     SS_HiliteCell( hWnd, FALSE );
                     lpRC1->ssCurrAt = Pos1AtTemp;
                  }

                  lpRC1->UL = ULNew;
                  break;
               }
               else
                  lpRC1->UL = ULNew;
            }
         }
      }
      else
      {
         while ( ULTemp > lpRC1->nHeaderCnt + lpRC1->Frozen )
         {
            ULTemp--;
            if ( lpfnGetSize( lpSS, ULTemp ) > 0 )
            {
               SS_HiliteCell( hWnd, FALSE );
               lpRC1->UL = ULTemp;
               lScrollInc -= lpfnGetSize( lpSS, lpRC1->UL );
               break;
            }
         }
      }

      if ( lpSS->ModeBlock || !bMoveActiveCell ||
           SS_LeaveCell( hWnd, lpSS,
                         bLeft ? PosAtOld : -1,
                         bLeft ? ULOld : -1,
                         bLeft ? -1 : PosAtOld,
                         bLeft ? -1 : ULOld, 0 ) )
      {
         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bLeft && lpSS->ModeBlock )
            lpSS->Row.ssCurrAt = BlockCellCurrentPos;

         if ( lScrollInc )
         {
            if ( bLeft )
               SS_HScroll( hWnd, lScrollInc );
            else
            {
// #ifdef ZEIDON
               SS_HiliteCell( hWnd, FALSE );
// #endif
               SS_VScroll( hWnd, lScrollInc );
            }
         }
         else
            SS_HiliteCell( hWnd, TRUE );

         if ( bLeft == 0 && lpSS->ModeBlock == 0 && bMoveActiveCell &&
              PosAtOld == lpSS->Row.ssCurrAt )
         {
            SS_SendMsgCommand( hWnd, 0, SSN_EXITPREV, FALSE );
         }
         else
         if ( lpSS->ModeBlock && bMoveActiveCell )
            SSx_SelectBlock( hWnd, lpSS,
               bLeft ? BlockCellCurrentPos : lpSS->Col.BlockCellCurrentPos,
               bLeft ? lpSS->Row.BlockCellCurrentPos : BlockCellCurrentPos );
      }
   }

   if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bLeft && lpSS->ModeBlock )
      lpSS->Row.ssCurrAt = BlockCellCurrentPos;

   if ( !bLeft && lpSS->Row.UL != ULOld )
      lpSS->wScrollDirection = SS_VSCROLL_UP;
}

void
SS_ScrollPageLR( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 WORD          fFlags )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollPageLR" );
   zBOOL         bMoveActiveCell;
   zBOOL         bRight;
   RECT          rect;
   SS_COORD      PosAt;
   SS_COORD      ULNew;
   SS_COORD      PosAtOld;
   SS_COORD      ULOld;
   SS_COORD      LRPos;
   SS_COORD      BlockCellCurrentPos;
   SS_COORD      MaxCnt;
   LPSS_ROWCOL   lpRC1;        //either lpRow or lpCol structure
   LPSS_ROWCOL   lpRC2;        //opposite of lpRC1
   zBOOL         ( *lpfnIsSelected )( LPSPREADSHEET );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return;

   bMoveActiveCell = ((fFlags & SS_F_MOVEACTIVECELL) == SS_F_MOVEACTIVECELL);
   bRight = ((fFlags & SS_F_SCROLL_RIGHT) == SS_F_SCROLL_RIGHT);

   if ( bRight )
   {
      lpRC1 = &(lpSS->Col);
      lpRC2 = &(lpSS->Row);
      lpfnIsSelected = SS_IsColsSelected;
      MaxCnt = SS_GetColsMax( lpSS );
   }
   else // scroll down
   {
      lpRC1 = &(lpSS->Row);
      lpRC2 = &(lpSS->Col);
      lpfnIsSelected = SS_IsRowsSelected;
      MaxCnt = SS_GetRowsMax( lpSS );
   }

   PosAtOld = lpRC1->ssCurrAt;
   ULOld = lpRC1->UL;

   BlockCellCurrentPos = lpRC1->BlockCellCurrentPos;
   LRPos = lpRC1->LRAllVis;

   ULNew = LRPos + 1;

   if ( LRPos < lpRC1->UL )
      LRPos = lpRC1->UL;

   if ( LRPos < MaxCnt - 1 )
   {
      if ( bRight )
         PosAt = SS_GetRightVisCell( hWnd, lpSS, ULNew );
      else
         PosAt = SS_GetBottomVisCell( hWnd, lpSS, ULNew );

      if ( PosAt == MaxCnt - 1 )
         SS_ScrollEnd( hWnd, lpSS, ( bRight ? SS_F_SCROLL_ROW : SS_F_SCROLL_COL )
        | SS_F_SCROLLMAX | ( bMoveActiveCell ? SS_F_MOVEACTIVECELL : 0 ) );

      else
      {
         SS_HiliteCell( hWnd, FALSE );
         lpRC1->UL = ULNew;

         if ( lpSS->ModeBlock )
       BlockCellCurrentPos = lpRC1->UL;
         else if ( bMoveActiveCell )
    {
       lpRC1->ssCurrAt = lpRC1->UL;
    }

         // Scroll All.
         SS_GetClientRect( hWnd, &rect );

         if ( bRight )
         {
            if ( lpSS->ModeBlock || !bMoveActiveCell ||
                SS_LeaveCell( hWnd, lpSS, PosAtOld, ULOld, -1, -1, 0 ) )
               SS_HScroll( hWnd, rect.right - rect.left );
         }
         else
         {
            if ( lpSS->ModeBlock || !bMoveActiveCell ||
                SS_LeaveCell( hWnd, lpSS, -1, -1, PosAtOld, ULOld, 0 ) )
               SS_VScroll( hWnd, rect.bottom - rect.top );
         }

         if ( lpSS->ModeBlock && bMoveActiveCell )
         {
            SSx_SelectBlock( hWnd, lpSS,
              bRight ? BlockCellCurrentPos : lpSS->Col.BlockCellCurrentPos,
              bRight ? lpSS->Row.BlockCellCurrentPos : BlockCellCurrentPos );
         }

         if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bRight && lpSS->ModeBlock )
            lpSS->Row.ssCurrAt = BlockCellCurrentPos;
      }
   }
   else
      SS_ScrollEnd( hWnd, lpSS, (bRight ? SS_F_SCROLL_ROW : SS_F_SCROLL_COL) |
         SS_F_SCROLLMAX | (bMoveActiveCell ? SS_F_MOVEACTIVECELL : 0) );
}

void
SS_ScrollPageUL( HWND          hWnd,
                 LPSPREADSHEET lpSS,
                 WORD          fFlags )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollPageUL" );
   zBOOL         bMoveActiveCell;
   zBOOL         bLeft;
   RECT          rect;
   SS_COORD      PosAt;
   SS_COORD      PosAtOld;
   SS_COORD      ULOld;
   SS_COORD      BlockCellCurrentPos;
   zLONG         lCellPos;
   zLONG         lRectSize;
   LPSS_ROWCOL   lpRC1;        //either lpRow or lpCol structure
   LPSS_ROWCOL   lpRC2;        //opposite of lpRC1
   zSHORT        (*lpfnGetSize)( LPSPREADSHEET, SS_COORD );
   zBOOL         (*lpfnIsSelected)( LPSPREADSHEET );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return;

   bMoveActiveCell = ((fFlags & SS_F_MOVEACTIVECELL) == SS_F_MOVEACTIVECELL);
   bLeft = ((fFlags & SS_F_SCROLL_LEFT) == SS_F_SCROLL_LEFT);

   if ( bLeft )
   {
      lpRC1 = &(lpSS->Col);
      lpRC2 = &(lpSS->Row);
      lpfnGetSize = SS_GetColWidth;
      lpfnIsSelected = SS_IsColsSelected;
   }
   else // scroll up
   {
      lpRC1 = &(lpSS->Row);
      lpRC2 = &(lpSS->Col);
      lpfnGetSize = SS_GetRowHeight;
      lpfnIsSelected = SS_IsRowsSelected;
   }

   PosAtOld = lpRC1->ssCurrAt;
   ULOld = lpRC1->UL;

   BlockCellCurrentPos = lpRC1->BlockCellCurrentPos;

   if ( lpRC1->UL > lpRC1->nHeaderCnt + lpRC1->Frozen )
   {
      SS_GetClientRect( hWnd, &rect );

      if ( bLeft )
      {
         lCellPos = SS_GetCellPosX( lpSS, lpRC1->UL, lpRC1->UL );
         lRectSize = rect.right - rect.left;
      }
      else
      {
         lCellPos = SS_GetCellPosY( lpSS, lpRC1->UL, lpRC1->UL );
         lRectSize = rect.bottom - rect.top;
      }

      PosAt = lpRC1->UL;

      for ( ; PosAt > lpRC1->nHeaderCnt && lCellPos <= lRectSize; )
      {
         if ( --PosAt < lpRC1->nHeaderCnt )
            break;

         lCellPos += lpfnGetSize( lpSS, PosAt );
      }

      if ( lCellPos > lRectSize )
      {
         SS_HiliteCell( hWnd, FALSE );

         if ( PosAt + 1 == lpRC1->UL )
            lpRC1->UL = mMax( lpRC1->nHeaderCnt + lpRC1->Frozen, PosAt );
         else
            lpRC1->UL = mMax( lpRC1->nHeaderCnt + lpRC1->Frozen, PosAt + 1 );

         if ( bMoveActiveCell )
         {
            if ( lpRC1->UL == lpRC1->nHeaderCnt + lpRC1->Frozen )
               PosAt = lpRC1->nHeaderCnt;
            else
               PosAt = lpRC1->UL;

            if ( lpSS->ModeBlock )
            {
               BlockCellCurrentPos = PosAt;

               if ( lpSS->wOpMode == SS_OPMODE_EXTSEL && !bLeft )
                  lpSS->Row.ssCurrAt = BlockCellCurrentPos;
            }
            else
               lpRC1->ssCurrAt = PosAt;

         }

         // Scroll All.
         if ( bLeft )
         {
            if ( lpSS->ModeBlock || !bMoveActiveCell ||
                 SS_LeaveCell( hWnd, lpSS, PosAtOld, ULOld, -1, -1, 0 ) )
            {
               SS_HScroll( hWnd, -lRectSize );
            }
         }
         else
         {
            if ( lpSS->ModeBlock || !bMoveActiveCell ||
                 SS_LeaveCell( hWnd, lpSS, -1, -1, PosAtOld, ULOld, 0 ) )
            {
               SS_VScroll( hWnd, -lRectSize );
            }
         }

         if ( lpSS->ModeBlock && bMoveActiveCell )
            SSx_SelectBlock( hWnd, lpSS, bLeft ? BlockCellCurrentPos :
                           lpRC2->BlockCellCurrentPos, bLeft ?
                           lpRC2->BlockCellCurrentPos : BlockCellCurrentPos );
      }
      else
         SS_ScrollHome( hWnd, lpSS,
                        (bLeft ? SS_F_SCROLL_ROW : SS_F_SCROLL_COL) |
                        (bMoveActiveCell ? SS_F_MOVEACTIVECELL : 0) );
   }
   else
   {
   // SS_HiliteCell(hWnd, TRUE);
      SS_ScrollHome( hWnd, lpSS,
                     (bLeft ? SS_F_SCROLL_ROW : SS_F_SCROLL_COL) |
                     (bMoveActiveCell ? SS_F_MOVEACTIVECELL : 0) );
   }

   if ( !bLeft && lpRC1->UL != ULOld )
      lpSS->wScrollDirection = SS_VSCROLL_PAGEUP;
}

void
SS_ScrollNextCell( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollNextCell" );
   SSx_ScrollNextCell( hWnd, FALSE, FALSE );
}

zBOOL
SSx_ScrollNextCell( HWND  hWnd,
                    zBOOL fIsNext,
                    zBOOL fNextRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSx_ScrollNextCell" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   SS_COORD      ssColLeftOld;
   SS_COORD      ssColAtOld;
   SS_COORD      ssColAtTemp;
   SS_COORD      ssRowAtOld;
   SS_COORD      ssRowAtTemp;
   SS_COORD      DataColCnt;
   SS_COORD      DataRowCnt;
   zBOOL         Found = FALSE;
   zBOOL         fRet = TRUE;
   zBOOL         fRedraw;

   lpSS = SS_Lock( hWnd );

   ssColLeftOld = lpSS->Col.UL;
   ssColAtOld = lpSS->Col.ssCurrAt;
   ssColAtTemp = lpSS->Col.ssCurrAt;
   ssRowAtOld = lpSS->Row.ssCurrAt;
   ssRowAtTemp = lpSS->Row.ssCurrAt;

   if ( !fIsNext )
   {
      if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
         return( FALSE );
   }

   DataColCnt = lpSS->Col.DataCnt;
   DataRowCnt = lpSS->Row.DataCnt;

   if ( fIsNext )
      SS_UpdateRestrictCnt( hWnd, lpSS, &DataColCnt, &DataRowCnt );

   if ( fNextRow )
   {
      if ( ssRowAtTemp == lpSS->Row.nHeaderCnt + lpSS->Row.Frozen - 1 )
         ssRowAtTemp = mMax( lpSS->Row.UL, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
      else
         ssRowAtTemp++;

      ssColAtTemp = lpSS->Col.nHeaderCnt - 1;
   }

   while ( Found == 0 && ssRowAtTemp < SS_GetRowsMax( lpSS ) &&
           (lpSS->RestrictRows == 0 ||
           (lpSS->RestrictRows && ssRowAtTemp <= DataRowCnt)) )
   {
      if ( SS_GetRowHeight( lpSS, ssRowAtTemp ) > 0 )
      {
         while ( Found == 0 && ssColAtTemp < SS_GetColsMax( lpSS ) - 1 &&
                 (lpSS->RestrictCols == 0 ||
                 (lpSS->RestrictCols && ssColAtTemp <= DataColCnt)) )
         {
            if ( ssColAtTemp == lpSS->Col.nHeaderCnt + lpSS->Col.Frozen - 1 &&
                 ssRowAtTemp == ssRowAtOld )
            {
               ssColAtTemp = mMax( lpSS->Col.UL,
                                lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );
            }
            else
               ssColAtTemp++;

            SS_RetrieveCellType( lpSS, &CellType, 0,
                                 ssColAtTemp, ssRowAtTemp );

            if ( SS_GetColWidth( lpSS, ssColAtTemp ) > 0 &&
                 (lpSS->fEditModePermanent == 0 ||
                 (CellType.chType != SS_TYPE_STATICTEXT &&
                  CellType.chType != SS_TYPE_OWNERDRAW &&
                  CellType.chType != SS_TYPE_PICTURE &&
                  !SS_GetLock( hWnd, lpSS, ssColAtTemp, ssRowAtTemp ))) )
            {
               Found = TRUE;
            }
         }
      }

      if ( Found == 0 )
      {
         if ( ssRowAtTemp == lpSS->Row.nHeaderCnt + lpSS->Row.Frozen - 1 )
            ssRowAtTemp = mMax( lpSS->Row.UL,
                             lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
         else
            ssRowAtTemp++;

         ssColAtTemp = lpSS->Col.nHeaderCnt - 1;
      }
   }

   if ( fIsNext )
      fRet = Found == 0;
   else
   if ( Found )
   {
      SS_HiliteCell( hWnd, FALSE );
      lpSS->Col.ssCurrAt = ssColAtTemp;
      lpSS->Row.ssCurrAt = ssRowAtTemp;

      if ( SS_LeaveCell( hWnd, lpSS, ssColAtOld, ssColLeftOld, ssRowAtOld, -1, 0 ) )
      {
         if ( lpSS->Col.ssCurrAt < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen &&
              lpSS->Col.UL != lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
         {
            lpSS->Col.UL = lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;
            fRedraw = lpSS->Redraw;
            lpSS->Redraw = FALSE;

            lpSS->fAdjustUpdateRgn = TRUE;
            SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
            lpSS->fAdjustUpdateRgn = FALSE;

            lpSS->Redraw = fRedraw;
            SSx_InvalidateColArea( hWnd, lpSS );
         }
         else
         {
            lpSS->fAdjustUpdateRgn = TRUE;
            SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
            lpSS->fAdjustUpdateRgn = FALSE;
         }
      }
   }
   else
   {
      SS_HiliteCell( hWnd, TRUE );
      SS_SendMsgCommand( hWnd, 0, SSN_EXITNEXT, FALSE );
   }

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

void
SS_ScrollPrevCell( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ScrollPrevCell" );
   SSx_ScrollPrevCell( hWnd, FALSE );
}

zBOOL
SSx_ScrollPrevCell( HWND  hWnd,
                    zBOOL fIsNext )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSx_ScrollPrevCell" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   SS_COORD      ssColAtOld;
   SS_COORD      ssColAtTemp;
   SS_COORD      ssRowAtOld;
   SS_COORD      ssRowAtTemp;
   zBOOL         Found = FALSE;
   zBOOL         fRet = TRUE;

   lpSS = SS_Lock( hWnd );

   ssColAtOld = lpSS->Col.ssCurrAt;
   ssColAtTemp = lpSS->Col.ssCurrAt;
   ssRowAtOld = lpSS->Row.ssCurrAt;
   ssRowAtTemp = lpSS->Row.ssCurrAt;

   while ( Found == 0 && ssRowAtTemp >= lpSS->Row.nHeaderCnt )
   {
      if ( SS_GetRowHeight( lpSS, ssRowAtTemp ) > 0 )
      {
         while ( Found == 0 && ssColAtTemp > lpSS->Col.nHeaderCnt )
         {
            ssColAtTemp--;

            SS_RetrieveCellType( lpSS, &CellType, 0, ssColAtTemp, ssRowAtTemp );

            if ( SS_GetColWidth( lpSS, ssColAtTemp ) > 0 &&
                 (lpSS->fEditModePermanent == 0 ||
                  (CellType.chType != SS_TYPE_STATICTEXT &&
                   CellType.chType != SS_TYPE_OWNERDRAW &&
                   CellType.chType != SS_TYPE_PICTURE &&
                   !SS_GetLock( hWnd, lpSS, ssColAtTemp, ssRowAtTemp ))) )
            {
               Found = TRUE;
            }
         }
      }

      if ( Found == 0 )
      {
         ssRowAtTemp--;
         ssColAtTemp = SS_GetColsMax( lpSS );
      }
   }

   if ( fIsNext )
      fRet = Found == 0;
   else
   if ( Found )
   {
      if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
         return( FALSE );

      SS_HiliteCell( hWnd, FALSE );
      lpSS->Col.ssCurrAt = ssColAtTemp;
      lpSS->Row.ssCurrAt = ssRowAtTemp;

      if ( SS_LeaveCell( hWnd, lpSS, ssColAtOld, -1, ssRowAtOld, -1, 0 ) )
      {
         lpSS->fAdjustUpdateRgn = TRUE;
         SS_ShowActiveCell( hWnd, SS_SHOW_NEAREST );
         lpSS->fAdjustUpdateRgn = FALSE;
      }
   }
   else
      SS_SendMsgCommand( hWnd, 0, SSN_EXITPREV, FALSE );

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

void
SS_SetHScrollBar( HWND hWnd,
                  LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_SetHScrollBar" );
   SS_COORD ScrollLast;
   RECT     rect;
   SS_COORD ssCols;
   SS_COORD ss;
   zLONG    lPos;
   zLONG    x;
   zBOOL    SetScrollBar = TRUE;
   zBOOL    fNoScrollBar = FALSE;
   zBOOL    fDisableScrollBar = FALSE;

   if ( lpSS->fNoSetScrollBars || lpSS->Redraw == 0 )
      return;

   lpSS->fHScrollVisible = TRUE;

   if ( lpSS->HorzScrollBar )
   {
      if ( lpSS->fScrollBarShowMax == 0 )
      {
         ScrollLast = mMax( lpSS->Col.DataCnt - 1,
                           SS_DEFAULT_HSCROLL + lpSS->Col.nHeaderCnt - 1 );
         ScrollLast = mMin( lpSS->Col.Max, ScrollLast );
      }
      else
         ScrollLast = lpSS->Col.Max;

      if ( lpSS->fScrollBarMaxAlign )
         ScrollLast = mMin( SS_GetLastPageLeftCol( hWnd, lpSS ), ScrollLast );

      if ( lpSS->fScrollBarExtMode )
      {
         ssCols = SS_GetColsMax( lpSS );

         if ( lpSS->fAutoSize && lpSS->Col.AutoSizeVisibleCnt &&
              lpSS->Col.AutoSizeVisibleCnt < ssCols - 1 )
         {
            SetScrollBar = TRUE;
            fNoScrollBar = FALSE;
         }
         else
         {
            SS_GetClientRect( hWnd, &rect );

            SetScrollBar = FALSE;
            fNoScrollBar = TRUE;

            for ( ss = 0, x = -1;
                  ss < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen && ss < ssCols;
                  ss++ )
            {
               x += SS_GetColWidth( lpSS, ss );
            }

            for ( ss = mMax( ss, lpSS->Col.nHeaderCnt ); ss < ssCols; ss++ )
            {
               x += SS_GetColWidth( lpSS, ss );
               if ( x > rect.right + 1 )
               {
                  SetScrollBar = TRUE;
                  fNoScrollBar = FALSE;
                  break;
               }
            }
         }
      }

      if ( SetScrollBar )
      {
         if ( lpSS->Col.nHeaderCnt + lpSS->Col.Frozen < ScrollLast )
         {
            if ( ScrollLast <= SS_SBMAX )
               lPos = (zSHORT) lpSS->Col.UL;
            else
               lPos = (zSHORT)((zLONG) lpSS->Col.UL *
                                ((zLONG) SS_SBMAX / (zLONG) ScrollLast ) );

#ifndef SS_NOCHILDSCROLL
            ShowScrollBar( lpSS->hWndHScroll, SB_CTL, TRUE );
            SetScrollRange( lpSS->hWndHScroll, SB_CTL,
                            mMin( mMin( lpSS->Col.nHeaderCnt + lpSS->Col.Frozen,
                                        ScrollLast ), SS_SBMAX ),
                            mMin( ScrollLast, SS_SBMAX ), FALSE );
            SetScrollPos( lpSS->hWndHScroll, SB_CTL, lPos, TRUE );
#else
            SetScrollRange( hWnd, SB_HORZ,
                            mMin( mMin( lpSS->Col.nHeaderCnt +
                                lpSS->Col.Frozen, ScrollLast ), SS_SBMAX ),
                            mMin( ScrollLast, SS_SBMAX ), FALSE );
            SetScrollPos( hWnd, SB_HORZ, lPos, TRUE );
#endif
         }
         else
            fDisableScrollBar = TRUE;
      }
   }
   else
   if ( lpSS->fHScrollVisible )
#ifndef SS_NOCHILDSCROLL
   {
      lpSS->fHScrollVisible = FALSE;
      ShowScrollBar( lpSS->hWndHScroll, SB_CTL, FALSE );
      SetScrollRange( lpSS->hWndHScroll, SB_CTL, 0, 0, TRUE );
   }
#else
   SetScrollRange( hWnd, SB_HORZ, 0, 0, TRUE );
#endif

   if ( fNoScrollBar || fDisableScrollBar )
   {
#ifndef SS_NOCHILDSCROLL
      if ( fNoScrollBar )
      {
         lpSS->fHScrollVisible = FALSE;
         ShowScrollBar( lpSS->hWndHScroll, SB_CTL, FALSE );
         SetScrollRange( lpSS->hWndHScroll, SB_CTL, 0, 0, TRUE );
      }
      else
      {
         ShowScrollBar( lpSS->hWndHScroll, SB_CTL, TRUE );
         SetScrollRange( lpSS->hWndHScroll, SB_CTL, 0, -1, TRUE );
      }
#else
      if ( fNoScrollBar )
         SetScrollRange( hWnd, SB_HORZ, 0, 0, TRUE );
      else
         SetScrollRange( hWnd, SB_HORZ, 0, -1, TRUE );
#endif

      if ( lpSS->Col.UL > lpSS->Col.nHeaderCnt + lpSS->Col.Frozen )
      {
         lpSS->Col.UL = lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;
         InvalidateRect( hWnd, 0, TRUE );
      }
   }
}

void
SS_SetVScrollBar( HWND hWnd,
                  LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_SetVScrollBar" );
   SS_COORD ScrollLast;
   RECT     rect;
   SS_COORD ssRows;
   SS_COORD ssRowTop;
   SS_COORD ss;
   zLONG    lPos;
   zLONG    y;
   zBOOL    SetScrollBar = TRUE;
   zBOOL    fNoScrollBar = FALSE;
   zBOOL    fShowBuffer = FALSE;
   zBOOL    fDisableScrollBar = FALSE;

   if ( lpSS->fNoSetScrollBars || lpSS->Redraw == 0 )
      return;

   lpSS->fVScrollVisible = TRUE;

   if ( lpSS->fVirtualMode && ( lpSS->Virtual.lVStyle & SSV_SCROLLBARSHOWBUFFER ) )
      fShowBuffer = TRUE;

   if ( lpSS->VertScrollBar )
   {
      if ( fShowBuffer )
         ScrollLast = lpSS->Virtual.VPhysSize;
      else
      if ( lpSS->fScrollBarShowMax == 0 )
      {
         ScrollLast = mMax( lpSS->Row.DataCnt - 1,
                           SS_DEFAULT_VSCROLL + lpSS->Row.nHeaderCnt - 1 );

         ScrollLast = mMin( lpSS->Row.Max, ScrollLast );
      }
      else
         ScrollLast = lpSS->Row.Max;

      if ( lpSS->fScrollBarMaxAlign )
      {
         if ( fShowBuffer )
            ScrollLast = mMin( SSx_GetLastPageTopRow( hWnd, lpSS,
                                           lpSS->Virtual.VTop +
                                           lpSS->Virtual.VPhysSize ) -
                                           lpSS->Virtual.VTop, ScrollLast );
         else
            ScrollLast = mMin( SS_GetLastPageTopRow( hWnd, lpSS ), ScrollLast );
      }

      if ( lpSS->fScrollBarExtMode )
      {
         ssRows = SS_GetRowsMax( lpSS );

         if ( lpSS->fAutoSize && lpSS->Row.AutoSizeVisibleCnt &&
              lpSS->Row.AutoSizeVisibleCnt < ssRows - 1 )
         {
            SetScrollBar = TRUE;
            fNoScrollBar = FALSE;
         }
         else
         {
            SS_GetClientRect( hWnd, &rect );

            SetScrollBar = FALSE;
            fNoScrollBar = TRUE;

            for ( ss = 0, y = -1;
                  ss < lpSS->Row.nHeaderCnt + lpSS->Row.Frozen && ss < ssRows;
                  ss++ )
            {
               y += SS_GetRowHeight( lpSS, ss );
            }

            for ( ss = mMax( ss, lpSS->Row.nHeaderCnt ); ss < ssRows; ss++ )
            {
               y += SS_GetRowHeight( lpSS, ss );
               if ( y > rect.bottom + 1 )
               {
                  SetScrollBar = TRUE;
                  fNoScrollBar = FALSE;
                  break;
               }
            }
         }
      }

      if ( SetScrollBar )
         if ( lpSS->Row.nHeaderCnt + lpSS->Row.Frozen < ScrollLast )
         {
            if ( ScrollLast <= SS_SBMAX )
               lPos = fShowBuffer ?
                     lpSS->Row.UL - lpSS->Virtual.VTop + 1 : lpSS->Row.UL;
            else
            {
               if ( fShowBuffer )
                  ssRowTop = lpSS->Row.UL - lpSS->Virtual.VTop + 1;
               else
                  ssRowTop = lpSS->Row.UL;

               lPos = (ssRowTop * ((zLONG) SS_SBMAX / ScrollLast));
            }

#ifndef SS_NOCHILDSCROLL
            ShowScrollBar( lpSS->hWndVScroll, SB_CTL, TRUE );
            SetScrollRange( lpSS->hWndVScroll, SB_CTL,
                            mMin( mMin( lpSS->Row.nHeaderCnt +
                                  lpSS->Row.Frozen, ScrollLast ), SS_SBMAX ),
                            mMin( ScrollLast, SS_SBMAX ), FALSE );
            SetScrollPos( lpSS->hWndVScroll, SB_CTL, lPos, TRUE );
#else
            SetScrollRange( hWnd, SB_VERT,
                            mMin( mMin( lpSS->Row.nHeaderCnt +
                                  lpSS->Row.Frozen, ScrollLast ), SS_SBMAX ),
                            mMin( ScrollLast, SS_SBMAX ), FALSE );
            SetScrollPos( hWnd, SB_VERT, lPos, TRUE );
#endif
         }
         else
            fDisableScrollBar = TRUE;
   }
   else
   if ( lpSS->fVScrollVisible )
   {
#ifndef SS_NOCHILDSCROLL
      lpSS->fVScrollVisible = FALSE;
      ShowScrollBar( lpSS->hWndVScroll, SB_CTL, FALSE );
      SetScrollRange( lpSS->hWndVScroll, SB_CTL, 0, 0, TRUE );
#else
      SetScrollRange( hWnd, SB_VERT, 0, 0, TRUE );
#endif
   }

   if ( fNoScrollBar || fDisableScrollBar )
   {
#ifndef SS_NOCHILDSCROLL
      if ( fNoScrollBar )
      {
         lpSS->fVScrollVisible = FALSE;
         ShowScrollBar( lpSS->hWndVScroll, SB_CTL, FALSE );
         SetScrollRange( lpSS->hWndVScroll, SB_CTL, 0, 0, TRUE );
      }
      else
      {
         ShowScrollBar( lpSS->hWndVScroll, SB_CTL, TRUE );
         SetScrollRange( lpSS->hWndVScroll, SB_CTL, 0, -1, TRUE );
      }
#else
      if ( fNoScrollBar )
         SetScrollRange( hWnd, SB_VERT, 0, 0, TRUE );
      else
         SetScrollRange( hWnd, SB_VERT, 0, -1, TRUE );
#endif

      if ( !fShowBuffer &&
           lpSS->Row.UL > lpSS->Row.nHeaderCnt + lpSS->Row.Frozen )
      {
         lpSS->Row.UL = lpSS->Row.nHeaderCnt + lpSS->Row.Frozen;
         InvalidateRect( hWnd, 0, TRUE );
      }
   }
}

zBOOL
SS_ShowActiveCell( HWND   hWnd,
                   zLONG lShowWhere )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ShowActiveCell" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC;

   lpSS = SS_Lock( hWnd );

   bRC = SS_ShowCell( hWnd, lShowWhere, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL
SS_ShowCell( HWND      hWnd,
             zLONG     lShowWhere,
             SS_COORD  CellCol,
             SS_COORD  CellRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_ShowCell" );
   LPSPREADSHEET lpSS;
   RECT          rect;
   SS_COORD      ssRowAt;
   SS_COORD      ssColAt;
   SS_COORD      ssRowTopOld;
   SS_COORD      ssColLeftOld;
   SS_COORD      ssRowTopTemp;
   SS_COORD      ssColLeftTemp;
   zBOOL         ScrollCol = FALSE;
   zBOOL         ScrollRow = FALSE;
   zLONG         y;
   zLONG         x;
   zLONG         cy;
   zLONG         cx;

   lpSS = SS_Lock( hWnd );

   lpSS->Col.ULPrev = lpSS->Col.UL;
   lpSS->Row.ULPrev = lpSS->Row.UL;

   ssRowTopOld = lpSS->Row.UL;
   ssColLeftOld = lpSS->Col.UL;

   // Set ssCol.
   if ( CellCol != -1 )
   {
      switch ( lShowWhere )
      {
         case SS_SHOW_TOPLEFT:
         case SS_SHOW_CENTERLEFT:
         case SS_SHOW_BOTTOMLEFT:
            if ( CellCol != lpSS->Col.UL )
            {
               ssColAt = mMax( lpSS->Col.nHeaderCnt + lpSS->Col.Frozen, CellCol );
               if ( ssColAt != lpSS->Col.UL )
               {
                  lpSS->Col.UL = ssColAt;
                  ScrollCol = TRUE;
               }
            }

            break;

         case SS_SHOW_TOPRIGHT:
         case SS_SHOW_CENTERRIGHT:
         case SS_SHOW_BOTTOMRIGHT:
            SS_GetClientRect( hWnd, &rect );

            x = SS_GetCellPosX( lpSS, CellCol, CellCol );
            cx = SS_GetColWidth( lpSS, CellCol );

            for ( ssColAt = CellCol;
                  ssColAt > lpSS->Col.nHeaderCnt &&
                     x + cx <= rect.right - rect.left; )
            {
               if ( --ssColAt < lpSS->Col.nHeaderCnt )
                  break;

               x += SS_GetColWidth( lpSS, ssColAt );
            }

            if ( ssColAt > lpSS->Col.nHeaderCnt )
               ssColAt++;

            ssColAt = mMax( ssColAt, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );

            if ( ssColAt != lpSS->Col.UL )
            {
               lpSS->Col.UL = ssColAt;
               ScrollCol = TRUE;
            }

            break;

         case SS_SHOW_TOPCENTER:
         case SS_SHOW_CENTER:
         case SS_SHOW_BOTTOMCENTER:
            SS_GetClientRect( hWnd, &rect );

            x = SS_GetCellPosX( lpSS, CellCol, CellCol );
            cx = SS_GetColWidth( lpSS, CellCol );

            for ( ssColAt = CellCol;
                  ssColAt > lpSS->Col.nHeaderCnt && x +
                           cx < (rect.right - rect.left) / 2; )
            {
               if ( --ssColAt < lpSS->Col.nHeaderCnt )
                  break;

               x += SS_GetColWidth( lpSS, ssColAt );
            }

            if ( ssColAt > lpSS->Col.nHeaderCnt )
               ssColAt++;

            ssColAt = mMax( ssColAt, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen );

            if ( ssColAt != lpSS->Col.UL )
            {
               lpSS->Col.UL = ssColAt;
               ScrollCol = TRUE;
            }

            break;

         case SS_SHOW_NEAREST:
            if ( CellCol >= lpSS->Col.nHeaderCnt + lpSS->Col.Frozen &&
                 CellCol < lpSS->Col.UL )
            {
               lpSS->Col.UL = mMax( lpSS->Col.nHeaderCnt +
                                   lpSS->Col.Frozen, CellCol );
               ScrollCol = TRUE;
            }
            else
            if ( CellCol > SS_GetRightVisCell( hWnd, lpSS, lpSS->Col.UL ) &&
                 CellCol != lpSS->Col.UL )
            {
               SS_GetClientRect( hWnd, &rect );
               x = SS_GetCellPosX( lpSS, CellCol, CellCol ) +
                                            SS_GetColWidth( lpSS, CellCol );

               for ( ssColAt = CellCol; ssColAt > lpSS->Col.nHeaderCnt; ssColAt-- )
               {
                  x += SS_GetColWidth( lpSS, ssColAt - 1 );

                  if ( x > rect.right - rect.left )
                     break;
               }

               ssColAt = mMax( lpSS->Col.nHeaderCnt + lpSS->Col.Frozen, ssColAt );

               if ( ssColAt != lpSS->Col.UL )
               {
                  lpSS->Col.UL = ssColAt;
                  ScrollCol = TRUE;
               }
            }

            break;
      }
   }

   // Set ssRow.
   if ( CellRow != -1 )
   {
      switch ( lShowWhere )
      {
         case SS_SHOW_TOPLEFT:
         case SS_SHOW_TOPCENTER:
         case SS_SHOW_TOPRIGHT:
            if ( CellRow != lpSS->Row.UL )
            {
               ssRowAt = mMax( lpSS->Row.nHeaderCnt + lpSS->Row.Frozen, CellRow );

               if ( ssRowAt != lpSS->Row.UL )
               {
                  lpSS->Row.UL = ssRowAt;
                  ScrollRow = TRUE;
               }
            }

            break;

         case SS_SHOW_BOTTOMLEFT:
         case SS_SHOW_BOTTOMCENTER:
         case SS_SHOW_BOTTOMRIGHT:
            SS_GetClientRect( hWnd, &rect );

            y = SS_GetCellPosY( lpSS, CellRow, CellRow );
            cy = SS_GetRowHeight( lpSS, CellRow );

            for ( ssRowAt = CellRow;
                  ssRowAt > lpSS->Row.nHeaderCnt &&
                                          y + cy <= rect.bottom - rect.top; )
            {
               if ( --ssRowAt < lpSS->Row.nHeaderCnt )
                  break;

               y += SS_GetRowHeight( lpSS, ssRowAt );
            }

            if ( ssRowAt > lpSS->Row.nHeaderCnt )
               ssRowAt++;

            ssRowAt = mMax( ssRowAt, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );

            if ( lpSS->Row.UL != ssRowAt )
            {
               lpSS->Row.UL = ssRowAt;
               ScrollRow = TRUE;
            }

            break;

         case SS_SHOW_CENTERLEFT:
         case SS_SHOW_CENTER:
         case SS_SHOW_CENTERRIGHT:
            SS_GetClientRect( hWnd, &rect );

            y = SS_GetCellPosY( lpSS, CellRow, CellRow );
            cy = SS_GetRowHeight( lpSS, CellRow );

            for ( ssRowAt = CellRow;
                  ssRowAt > lpSS->Row.nHeaderCnt &&
                                  y + cy < ( rect.bottom - rect.top ) / 2; )
            {
               if ( --ssRowAt < lpSS->Row.nHeaderCnt )
                  break;

               y += SS_GetRowHeight( lpSS, ssRowAt );
            }

            if ( ssRowAt > lpSS->Row.nHeaderCnt )
               ssRowAt++;

            ssRowAt = mMax( ssRowAt, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );

            if ( lpSS->Row.UL != ssRowAt )
            {
               lpSS->Row.UL = ssRowAt;
               ScrollRow = TRUE;
            }

            break;

         case SS_SHOW_NEAREST:
            if ( CellRow >= lpSS->Row.nHeaderCnt + lpSS->Row.Frozen &&
                 CellRow < lpSS->Row.UL )
            {
               lpSS->Row.UL = mMax( lpSS->Row.nHeaderCnt + lpSS->Row.Frozen,
                                   CellRow );
               ScrollRow = TRUE;
            }
            else
            if ( CellRow > SS_GetBottomVisCell( hWnd, lpSS, lpSS->Row.UL ) &&
                 CellRow != lpSS->Row.UL )
            {
               SS_GetClientRect( hWnd, &rect );

               y = SS_GetCellPosY( lpSS, CellRow, CellRow ) +
               SS_GetRowHeight( lpSS, CellRow );

               for ( ssRowAt = CellRow; ssRowAt > lpSS->Row.nHeaderCnt; ssRowAt-- )
               {
                  y += SS_GetRowHeight( lpSS, ssRowAt - 1 );

                  if ( y > rect.bottom - rect.top )
                     break;
               }

               lpSS->Row.UL = mMax( lpSS->Row.nHeaderCnt + lpSS->Row.Frozen,
                                   ssRowAt );
               ScrollRow = TRUE;
            }

            break;

      }
   }

   // Show Cell.
   {
      ssRowTopTemp = lpSS->Row.UL;
      ssColLeftTemp = lpSS->Col.UL;
      lpSS->Row.UL = ssRowTopOld;
      lpSS->Col.UL = ssColLeftOld;

      SS_HiliteCell( hWnd, FALSE );

      if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
         return( FALSE );

      lpSS->Row.UL = ssRowTopTemp;
      lpSS->Col.UL = ssColLeftTemp;
   }

   SS_GetClientRect( hWnd, &rect );

   if ( ScrollCol )
   {
      if ( lpSS->Col.UL < ssColLeftOld )
         x = -(SS_GetCellPosXExt( lpSS, lpSS->Col.UL, ssColLeftOld, &rect ) -
               SS_GetCellPosX( lpSS, ssColLeftOld, ssColLeftOld ));
      else
         x = SS_GetCellPosXExt( lpSS, ssColLeftOld, lpSS->Col.UL, &rect ) -
                SS_GetCellPosX( lpSS, ssColLeftOld, ssColLeftOld );
   }

   if ( ScrollRow )
   {
      if ( lpSS->Row.UL < ssRowTopOld )
         y = -(SS_GetCellPosYExt( lpSS, lpSS->Row.UL, ssRowTopOld, &rect ) -
                SS_GetCellPosY( lpSS, ssRowTopOld, ssRowTopOld ) );
      else
         y = SS_GetCellPosYExt( lpSS, ssRowTopOld, lpSS->Row.UL, &rect ) -
                SS_GetCellPosY( lpSS, ssRowTopOld, ssRowTopOld );
   }

   if ( ScrollCol && ScrollRow )
   {
      if ( lpSS->wMessageBeingSent )
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
      else
      {
         SS_GetScrollArea( hWnd, lpSS, &rect, SS_SCROLLAREA_ALL );
         SS_Scroll( hWnd, x, y, &rect );
   //x   UpdateWindow( hWnd );
      }

      SS_SetVScrollBar( hWnd, lpSS );
      SS_SetHScrollBar( hWnd, lpSS );
      SS_TopLeftChange( hWnd );
   }
   else
   if ( ScrollRow )
   {
      if ( abs( y ) < rect.bottom && lpSS->wMessageBeingSent == 0 &&
           (lpSS->fUseScrollArrows == 0 || lpSS->Redraw) )
      {
         SS_VScroll( hWnd, y );
      }
      else
      {
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ROWS );
         SS_SetVScrollBar( hWnd, lpSS );
         SS_TopRowChange( hWnd );
      }
   }
   else
   if ( ScrollCol )
   {
      if ( abs( x ) < rect.right && lpSS->wMessageBeingSent == 0 &&
           (lpSS->fUseScrollArrows == 0 || lpSS->Redraw) )
      {
         SS_HScroll( hWnd, x );
      }
      else
      {
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_COLS );
         SS_SetHScrollBar( hWnd, lpSS );
         SS_LeftColChange( hWnd );
      }
   }
   else
      SS_HiliteCell( hWnd, TRUE );

// DrUnlockTaskMemory( lpSS );

   return( TRUE );
}

SS_COORD
SSx_GetLastPageTopRow( HWND hWnd,
                       LPSPREADSHEET lpSS,
                       SS_COORD MaxRows )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSx_GetLastPageTopRow" );
   RECT     rect;
   SS_COORD ssRowAt;
   SS_COORD ssRowAtTemp;
   zSHORT   nHeight;
   zLONG    y;

   if ( lpSS->fVirtualMode && lpSS->Row.Max == 0 )
      return( 0 );

   SS_GetClientRect( hWnd, &rect );
   y = SS_GetCellPosY( lpSS, 0, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen );
   ssRowAt = MaxRows - 1;

   for ( ssRowAtTemp = ssRowAt;
         ssRowAtTemp >= lpSS->Row.nHeaderCnt + lpSS->Row.Frozen;
         ssRowAtTemp-- )
   {
      nHeight = SS_GetRowHeight( lpSS, ssRowAtTemp );
      y += nHeight;

      if ( y > rect.bottom - rect.top )
         break;

      if ( nHeight > 0 )
         ssRowAt = ssRowAtTemp;
   }

   return( mMin( MaxRows - 1, ssRowAt ) );
}

void
SSx_InvalidateColArea( HWND hWnd,
                       LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSx_InvalidateColArea" );
   if ( lpSS->fUseScrollArrows )
      lpSS->Redraw = FALSE;

   SS_InvalidateColRange( hWnd, lpSS,
                          lpSS->Col.nHeaderCnt + lpSS->Col.Frozen, -1 );
   SS_SetHScrollBar( hWnd, lpSS );
   SS_LeftColChange( hWnd );

   if ( lpSS->fUseScrollArrows )
   {
      lpSS->Redraw = TRUE;
      SS_SetHScrollBar( hWnd, lpSS );
   //xUpdateWindow( hWnd );
   }
}

void
SS_TopLeftChange( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_TopLeftChange" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_SendMsgCommand( hWnd, 0, SSN_TOPLEFTCHANGE, FALSE );
// DrUnlockTaskMemory( lpSS );
}

void
SS_TopRowChange( HWND hWnd )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_TopRowChange" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_SendMsgCommand( hWnd, 0, SSN_TOPROWCHANGE, FALSE );
// DrUnlockTaskMemory( lpSS );
}

void
SS_UpdateRestrictCnt( HWND hWnd,
                      LPSPREADSHEET lpSS,
                      LPSS_COORD lpDataColCnt,
                      LPSS_COORD lpDataRowCnt )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_UpdateRestrictCnt" );
   SS_CELLTYPE   CellTypeTemp;
   LPSS_CELLTYPE lpCellType;
   LPSS_CELL     lpCell;
   HWND          hWndCtrl = 0;

   if ( lpSS->EditModeOn &&
        ((lpSS->RestrictRows && lpSS->Row.ssCurrAt >= lpSS->Row.DataCnt) ||
         (lpSS->RestrictCols && lpSS->Col.ssCurrAt >= lpSS->Col.DataCnt)) )
   {
      lpCell = SS_LockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
      lpCellType = SS_RetrieveCellType( lpSS, &CellTypeTemp, lpCell,
                                        lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );
      if ( lpCell )
         SS_UnlockCellItem( lpSS, lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt );

      if ( lpCellType->nControlID )
         hWndCtrl = SS_GetControlhWnd( lpSS, lpCellType->nControlID );

      if ( hWndCtrl && lpCellType->chType == SS_TYPE_EDIT ||
           lpCellType->chType == SS_TYPE_DATE ||
           lpCellType->chType == SS_TYPE_TIME ||
           lpCellType->chType == SS_TYPE_INTEGER ||
           lpCellType->chType == SS_TYPE_FLOAT ||
           lpCellType->chType == SS_TYPE_PIC )
      {
         if ( SendMessage( hWndCtrl, EM_GETMODIFY, 0, 0L ) )
         {
            if ( lpSS->RestrictCols && lpSS->Col.ssCurrAt >= lpSS->Col.DataCnt )
               (*lpDataColCnt)++;

            if ( lpSS->RestrictRows && lpSS->Row.ssCurrAt >= lpSS->Row.DataCnt )
               (*lpDataRowCnt)++;
         }
      }
   }
}

void
SS_VScroll( HWND    hWnd,
            zLONG   lScrollInc )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_VScroll" );

   LPSPREADSHEET lpSS;
   RECT          rectTemp;
   RECT          rect;

   if ( lScrollInc != 0 )
   {
      lpSS = SS_Lock( hWnd );

      SS_GetScrollArea( hWnd, lpSS, &rect, SS_SCROLLAREA_ROWS );
      SS_Scroll( hWnd, 0, -lScrollInc, &rect );

      if ( lScrollInc < 0 )
      {
         if ( SS_IsBlockSelected( lpSS ) )
         {
            SetRect( &rectTemp, rect.left, rect.top + -lScrollInc,
                     rect.right, rect.top + -lScrollInc + 1 );
            InvalidateRect( hWnd, &rectTemp, TRUE );
         }
      }

   //xif (lpSS->fUseScrollArrows)  //x???
   //x   lpSS->Redraw = FALSE;
   //xelse
   //x   UpdateWindow( hWnd );

      SS_SetVScrollBar( hWnd, lpSS );
      SS_TopRowChange( hWnd );

      if ( lpSS->fUseScrollArrows )
      {
         lpSS->Redraw = TRUE;
         SS_SetVScrollBar( hWnd, lpSS );
   //x   UpdateWindow( hWnd );                //x???
      }

//    DrUnlockTaskMemory( lpSS );
   }
}

void
SS_VPos( HWND      hWnd,
         SS_COORD  Pos )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SS_VPos" );
   LPSPREADSHEET lpSS;
   zLONG         lScrollInc;

   lpSS = SS_Lock( hWnd );

   if ( !SS_CellEditModeOff( hWnd, lpSS, 0 ) )
      return;

   SS_HiliteCell( hWnd, FALSE );

   lScrollInc = SS_GetCellPosY( lpSS, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen,
                               Pos ) -
               SS_GetCellPosY( lpSS, lpSS->Row.nHeaderCnt + lpSS->Row.Frozen,
                               lpSS->Row.UL );

   lpSS->Row.UL = Pos;

   SS_VScroll( hWnd, lScrollInc );
// DrUnlockTaskMemory( lpSS );
}

// ss_type.cpp
zPCHAR
StrInsertChar( zCHAR cChar, zPCHAR pchString, int iInsertPos )
{
   TRACE_DEBUG( "<STR.CPP>", "StrInsertChar " );
   int   k;
   zPCHAR lptmp = pchString;

   if ( lptmp )
   {
      // The new character is inserted into the string at index iInsertPos.
      // Loop backwards from the end of the string to the insert position
      // moving each character (including the terminator) RIGHT one place.
      for ( k = zstrlen( pchString ); k >= iInsertPos; k-- )
         pchString[ k + 1 ] = pchString[ k ];

      // Finally insert the new character
      pchString[ ++k ] = cChar;
   }

   return( lptmp );
}

int cdecl
StrPrintf( zPCHAR buff, zPCHAR format,... )
{
   MESSAGE_DEBUG( "<FSSTUB.CPP>", "StrPrintf" );
   return( 0 );
}

zBOOL
SS_FloatStringStrip( LPSPREADSHEET lpSS,
                     LPSS_CELLTYPE lpCellType,
                     zPCHAR pchText )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_FloatStringStrip" );
   zPCHAR lpTextTemp;
   zPCHAR lpTextNew;
   zCHAR  chSeparator;
   zCHAR  chDecimalSign;
   zCHAR  chCurrencySign;
   zBOOL fRet = TRUE;

   lpTextTemp = pchText;
   lpTextNew = pchText;

   if ( lpCellType->Spec.Float.fSetFormat &&
        lpCellType->Spec.Float.Format.chSeparator )
   {
      chSeparator = lpCellType->Spec.Float.Format.chSeparator;
   }
   else
      chSeparator = lpSS->DefaultFloatFormat.chSeparator;

   if ( lpCellType->Spec.Float.fSetFormat &&
        lpCellType->Spec.Float.Format.chDecimalSign )
   {
      chDecimalSign = lpCellType->Spec.Float.Format.chDecimalSign;
   }
   else
      chDecimalSign = lpSS->DefaultFloatFormat.chDecimalSign;

   if ( lpCellType->Spec.Float.fSetFormat &&
        lpCellType->Spec.Float.Format.chCurrencySign )
   {
      chCurrencySign = lpCellType->Spec.Float.Format.chCurrencySign;
   }
   else
      chCurrencySign = lpSS->DefaultFloatFormat.chCurrencySign;

   do
   {
      if ( *lpTextTemp == chSeparator || *lpTextTemp == chCurrencySign )
         ;
      else
      if ( *lpTextTemp == chDecimalSign )
      {
         *lpTextNew = '.';
         lpTextNew++;
      }
      else
      if ( isdigit( *lpTextTemp ) || *lpTextTemp == '.' ||
           *lpTextTemp == 0 || *lpTextTemp == '-' || *lpTextTemp == 'd' ||
           *lpTextTemp == 'D' || *lpTextTemp == 'e' || *lpTextTemp == 'E' )
      {
         *lpTextNew = *lpTextTemp;
         lpTextNew++;
      }
      else
         fRet = FALSE;

   } while ( *(lpTextTemp++) );

   return( fRet );
}

zBOOL
SS_FormatDataFloat( LPSPREADSHEET lpSS,
                    LPSS_CELLTYPE lpCellType,
                    zPCHAR        pchText,
                    zPCHAR        pchTextFmt,
                    zDECIMAL      *pdxValue,
                    zBOOL         fValue )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_FormatDataFloat" );
   zSHORT   nLth;
   zBOOL    fRet = FALSE;

   if ( *pchText )
   {
      nLth = mMin( (zLONG) zstrlen( pchText ), SS_MAXDATALTH - 1 );
      zmemcpy( pchTextFmt, pchText, nLth );
      pchTextFmt[ nLth ] = 0;

      fRet = SS_FloatStringStrip( lpSS, lpCellType, pchTextFmt );
      *pdxValue = ZDecimal( pchTextFmt );
   // StringToFloat( pchTextFmt, pdxValue );

      fRet = TRUE;
   }

   return( fRet );
}

zPCHAR
SS_CreateTextHandle( zPCHAR pchText,
                     zSHORT nLth )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_CreateTextHandle" );
   zPCHAR        hText;
   zPCHAR        pchTextTemp;

   if ( pchText == 0 )
      return( 0 );

   if ( nLth == 0 )
      nLth = zstrlen( pchText ) + 1;

   if ( DrAllocTaskMemory( (zCOREMEM) &hText, nLth, 1038 ) != 0 )
      return( 0 );

   pchTextTemp = (zPCHAR) DrLockTaskMemory( hText );
   strcpy_s( pchTextTemp, nLth, pchText );
// DrUnlockTaskMemory( pchTextTemp );

   return( pchTextTemp );
}

void
SS_FloatFormat( LPSPREADSHEET lpSS,
                LPSS_CELLTYPE lpCellType,
                zDECIMAL      dxVal,
                zPCHAR        pchText,
                zBOOL         fValue )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_FloatFormat" );
   BYTE  bFractionalWidth;
   zCHAR  chSeparator;
   zCHAR  chDecimalSign;
   zCHAR  chCurrencySign;

   if ( lpCellType->chType == SS_TYPE_FLOAT )
   {
#ifndef SS_USEAWARE
      if ( lpCellType->Spec.Float.nRight == 0 )
         bFractionalWidth = 1;
      else
#endif
         bFractionalWidth = (BYTE) lpCellType->Spec.Float.nRight;
   }
   else
   if ( lpCellType->chType == SS_TYPE_INTEGER )
      bFractionalWidth = 0;
   else
      bFractionalWidth = 2;

   if ( lpCellType->Spec.Float.fSetFormat &&
        lpCellType->Spec.Float.Format.chSeparator )
   {
      chSeparator = lpCellType->Spec.Float.Format.chSeparator;
   }
   else
      chSeparator = lpSS->DefaultFloatFormat.chSeparator;

   if ( lpCellType->Spec.Float.fSetFormat &&
        lpCellType->Spec.Float.Format.chDecimalSign )
   {
      chDecimalSign = lpCellType->Spec.Float.Format.chDecimalSign;
   }
   else
      chDecimalSign = lpSS->DefaultFloatFormat.chDecimalSign;

   if ( lpCellType->Spec.Float.fSetFormat &&
        lpCellType->Spec.Float.Format.chCurrencySign )
   {
      chCurrencySign = lpCellType->Spec.Float.Format.chCurrencySign;
   }
   else
      chCurrencySign = lpSS->DefaultFloatFormat.chCurrencySign;

#ifdef SS_USEAWARE

   if ( fValue )
      fpDoubleToString( pchText, dxVal, bFractionalWidth, '.', 0, FALSE,
                        (lpCellType->lStyle & FS_MONEY) ? TRUE : FALSE,
                        TRUE, FALSE, 0, 0 );
   else
   {
      zCHAR szCurrencySign[ 1 + 1 ];

      szCurrencySign[ 0 ] = chCurrencySign;
      szCurrencySign[ 1 ] = 0;

      fpDoubleToString( pchText, dxVal, bFractionalWidth, chDecimalSign,
                        chSeparator, FALSE,
                        (lpCellType->lStyle & FS_MONEY) ? TRUE : FALSE,
                        TRUE, FALSE, 0,
                        (lpCellType->lStyle & FS_MONEY) ? szCurrencySign : 0 );
   }

#else

   if ( !fValue && (lpCellType->lStyle & FS_SEPARATOR) )
      StrPrintf( pchText, "%.*Sm", bFractionalWidth,
                 chSeparator, chDecimalSign, dxVal );
   else
   {
      StrPrintf( pchText, "%.*f", bFractionalWidth, dxVal );

      if ( !fValue )
         SS_StrReplaceCh( pchText, MAKEWORD( chDecimalSign, '.' ) );
   }

   if ( lpCellType->chType == SS_TYPE_FLOAT )
   {
      zPCHAR pchPtr;

      if ( (pchPtr = zstrchr( pchText, fValue ? '.' : chDecimalSign )) != 0 )
      {
         if ( lpCellType->Spec.Float.nRight == 0 )
            *pchPtr = 0;
         else
            pchPtr[ lpCellType->Spec.Float.nRight + 1 ] = 0;
      }

      if ( !fValue && (lpCellType->lStyle & FS_MONEY) )
      {
         StrInsertChar( chCurrencySign, pchText, dxVal < 0.0 ? 1 : 0 );
      }
   }

#endif
}

HWND
SS_GetControlhWnd( LPSPREADSHEET lpSS,
                   zSHORT nCtrlID )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_GetControlhWnd" );
   LPSS_CONTROL lpControls;
   HWND         hWnd;

// lpControls = (LPSS_CONTROL) DrLockTaskMemory( lpSS->hControls );
   lpControls = (LPSS_CONTROL) lpSS->hControls;
   hWnd = lpControls[ nCtrlID - SS_CONTROL_BASEID ].hWnd;
// DrUnlockTaskMemory( lpControls );

   return( hWnd );
}

void
SS_PicDeref( HWND hWnd, HANDLE hPic )
{
   TRACE_DEBUG( "<SS_ALLOC.CPP>", "SS_PicDeref" );
   LPSPREADSHEET lpSS;
   LPSS_PICLIST  lpPicList;
   zBOOL         fFound = FALSE;
   zSHORT        k;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->hPicList && lpSS->nPicListCnt )
   {
//    lpPicList = (LPSS_PICLIST) DrLockTaskMemory( lpSS->hPicList );
      lpPicList = (LPSS_PICLIST) lpSS->hPicList;
      for ( k = 0; k < lpSS->nPicListCnt && !fFound; k++ )
      {
         if ( lpPicList[ k ].hPic == hPic )
         {
            lpPicList[ k ].nRefCnt--;
            if ( lpPicList[ k ].nRefCnt == 0 )
            {
               DeleteObject( lpPicList[ k ].hPic );
               lpPicList[ k ].hPic = 0;
            }

            fFound = TRUE;
         }
      }

//    DrUnlockTaskMemory( lpPicList );
   }

// DrUnlockTaskMemory( lpSS );
}

LPSS_CELLTYPE OPERATION
SSSetTypeComboBox( HWND          hWnd,
                   LPSS_CELLTYPE CellType,
                   zLONG         lStyle,
                   zPCHAR        lpItems )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeComboBox" );
   zPCHAR  hItems = 0;
   zPCHAR  lpItemsTemp = 0;
   zPCHAR  pch;
   zSHORT  nItemCnt;

   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_COMBOBOX;
   CellType->lStyle = lStyle;

   if ( lpItems )
   {
      zLONG lItemLth = zstrlen( lpItems ) + 1;
      if ( DrAllocTaskMemory( (zCOREMEM) &hItems, lItemLth, 1034 ) != 0 )
      {
         return( 0 );
      }

      lpItemsTemp = (zPCHAR) DrLockTaskMemory( hItems );
      strcpy_s( lpItemsTemp, lItemLth, lpItems );

      nItemCnt = 1;

      pch = lpItemsTemp;
      while ( (pch = zstrchr( pch, '\t' )) != 0 )
      {
         *pch = 0;
         pch++;

         if ( *pch )
            nItemCnt++;
      }

      CellType->Spec.ComboBox.nItemCnt = nItemCnt;
//    DrUnlockTaskMemory( lpItemsTemp );
   }

   if ( CellType->Spec.ComboBox.hItems )
      DrFreeTaskMemory( CellType->Spec.ComboBox.hItems );

   CellType->Spec.ComboBox.hItems = lpItemsTemp;
   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypeEdit( HWND          hWnd,
               LPSS_CELLTYPE CellType,
               zLONG         lStyle,
               zSHORT        nLth,
               zSHORT        nChrSet,
               zUSHORT       uCase )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeEdit" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   if ( lStyle & ES_MULTILINE )
      lStyle |= ES_AUTOVSCROLL;

   CellType->chType              = SS_TYPE_EDIT;
   CellType->lStyle              = lStyle;
   CellType->Spec.Edit.nLth      = nLth;
   CellType->Spec.Edit.nChrSet   = nChrSet;
   CellType->Spec.Edit.uCase     = uCase;

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypePicture( HWND          hWnd,
                  LPSS_CELLTYPE CellType,
                  zLONG         lStyle,
                  zPCHAR        PictName )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypePicture" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_PICTURE;
   CellType->lStyle = lStyle;

   if ( lStyle & VPS_HANDLE )
      CellType->Spec.ViewPict.hPictName = PictName;
   else
   if ( PictName &&
        (CellType->Spec.ViewPict.hPictName =
                                SS_CreateTextHandle( PictName, 0 )) == 0 )
   {
      return( 0 );
   }

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypePictureHandle( HWND          hWnd,
                        LPSS_CELLTYPE CellType,
                        zLONG         lStyle,
                        zPCHAR        hPict,
                        HPALETTE      hPal,
                        zBOOL         fDeleteHandle )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypePictureHandle" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_PICTURE;
   CellType->lStyle = lStyle | VPS_HANDLE;

   CellType->Spec.ViewPict.hPictName = hPict;
   CellType->Spec.ViewPict.hPal = hPal;
   CellType->Spec.ViewPict.fDeleteHandle = fDeleteHandle;

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypeStaticText( HWND          hWnd,
                     LPSS_CELLTYPE CellType,
                     zSHORT        nTextStyle )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeStaticText" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_STATICTEXT;
   CellType->lStyle = (zLONG) nTextStyle;

#ifdef SS_DEFPREFIX
   CellType->lStyle |= SS_TEXT_PREFIX;
#endif

   return( CellType );
}

zSHORT
SS_CreateControl( HWND          hWnd,
                  LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_CreateControl" );
   LPSPREADSHEET lpSS;
   LPSS_CONTROL  Controls;
   HWND          hWndCtrl;
   HWND          hWndParent;
   zPCHAR        PictName;
   zPCHAR        lpText;
   zLONG         lStyle;
   zCHAR         Buffer[ 40 ];
   zSHORT        nCtrlID = 0;
   zSHORT        k;
   zBOOL         fRet;
   zBOOL         fCreateAlways = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( !fCreateAlways && CellType->chType != SS_TYPE_PICTURE )
   {
      if ( lpSS->hControls )
      {
//       Controls = (LPSS_CONTROL) DrLockTaskMemory( lpSS->hControls );
         Controls = (LPSS_CONTROL) lpSS->hControls;
         for ( k = 0; k < lpSS->lControlsCnt; k++ )
         {
            if ( Controls[ k ].chType == CellType->chType )
            {
               if ( (CellType->chType == SS_TYPE_EDIT ||
                     CellType->chType == SS_TYPE_PIC ||
                     CellType->chType == SS_TYPE_FLOAT ||
                     CellType->chType == SS_TYPE_INTEGER ||
                     CellType->chType == SS_TYPE_BUTTON ||
                     CellType->chType == SS_TYPE_CHECKBOX) &&
                     Controls[ k ].lStyle != CellType->lStyle )
               {
                  continue;
               }

               nCtrlID = Controls[ k ].nCtrlID;
               break;
            }
         }

//       DrUnlockTaskMemory( Controls );

         if ( nCtrlID )
         {
            CellType->nControlID = nCtrlID;
//          DrUnlockTaskMemory( lpSS );
            return( nCtrlID );
         }
      }
   }

   nCtrlID = (zSHORT) (SS_CONTROL_BASEID + lpSS->lControlsCnt);

   if ( fCreateAlways || CellType->chType == SS_TYPE_PICTURE )
   {
//    Controls = (LPSS_CONTROL) DrLockTaskMemory( lpSS->hControls );
      Controls = (LPSS_CONTROL) lpSS->hControls;
      for ( k = 0; k < lpSS->lControlsCnt; k++ )
      {
         if ( !Controls[ k ].hWnd )
         {
            nCtrlID = SS_CONTROL_BASEID + k;
            break;
         }
      }

//    DrUnlockTaskMemory( Controls );
   }

   CellType->lStyle &= ~WS_VISIBLE;
   hWndParent = hWnd;
   switch ( CellType->chType )
   {
      case SS_TYPE_EDIT:
         if ( (CellType->lStyle & ES_AUTOHSCROLL) ||
              (CellType->lStyle & ES_MULTILINE) )
            hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSEditHScroll,
                                     0, WS_CHILD | CellType->lStyle, 0, 0, 0, 0,
                                     hWndParent, (HMENU) nCtrlID, g_hInstanceDLL, 0 );
         else
            hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSEdit, "",
                                     WS_CHILD | CellType->lStyle, 0, 0, 10, 10,
                                     hWndParent, (HMENU) nCtrlID, g_hInstanceDLL, 0 );

#ifdef SS_USEAWARE
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, ESX_AUTOADVANCE );
#endif
         break;

#ifndef SS_NOCT_PIC
      case SS_TYPE_PIC:
         lStyle = CellType->lStyle & ~(ES_CENTER | ES_RIGHT);
         hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSPic, "",
                                  WS_CHILD | lStyle, 0, 0, 10, 10,
                                  hWndParent, (HMENU) CtrlID, hDynamicInst, 0 );

         break;
#endif

#ifndef SS_NOCT_INT
      case SS_TYPE_INTEGER:
         wsprintf( Buffer, "%ld", CellType->Spec.Integer.Max );

         for ( k = 0; k < zstrlen( Buffer ); k++ )
            Buffer[ k ] = '9';

         lStyle = CellType->lStyle & ~( ES_CENTER | ES_RIGHT );
         hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSInteger, Buffer,
                                  WS_CHILD | lStyle, 0, 0, 65, 10,
                                  hWndParent, (HMENU) CtrlID, hDynamicInst, 0 );

         IntSetRange( hWndCtrl, CellType->Spec.Integer.Min,
                      CellType->Spec.Integer.Max );
         break;
#endif

      case SS_TYPE_FLOAT:
         zmemset( Buffer, 0, zsizeof( Buffer ) );

         for ( k = 0; k < CellType->Spec.Float.nLeft; k++ )
            strcat_s( Buffer, "9" );

         strcat_s( Buffer, "." );

         for ( k = 0; k < CellType->Spec.Float.nRight; k++ )
            strcat_s( Buffer, "9" );

         lStyle = CellType->lStyle & ~(ES_CENTER | ES_RIGHT);
         hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSFloat, Buffer,
                                  WS_CHILD | lStyle, 0, 0, 65, 10,
                                  hWndParent, (HMENU) nCtrlID,
                                  g_hInstanceDLL, 0 );

         FloatSetRange( hWndCtrl, CellType->Spec.Float.dxMin,
                        CellType->Spec.Float.dxMax );

#ifdef SS_USEAWARE
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, ESX_NOFORMATTEDEDIT );
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, ESX_AUTOADVANCE );
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, ESX_NULLSUPPORT );
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, DS_SEPARATOR );
#endif

#if 0
#ifdef SS_USEAWARE
         if ( lStyle & FS_MONEY )
         {
            SendMessage( hWndCtrl, DM_SETFRACTWIDTH, 2, 2L );
            SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, DS_FIXEDPOINT );
         }
         else
            SendMessage( hWndCtrl, DM_SETFRACTWIDTH,
                         CellType->Spec.Float.Right, 0L );
#endif
#endif

         break;

#ifndef SS_NOCT_COMBO
      case SS_TYPE_COMBOBOX:
         hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSComboBox, 0,
                                  WS_CHILD, 0, 0, 10, 10, hWndParent,
                                  (HMENU) nCtrlID, g_hInstanceDLL, 0 );

         break;
#endif

#ifndef SS_NOCT_TIME
      case SS_TYPE_TIME:
         lStyle = CellType->lStyle & ~(ES_CENTER | ES_RIGHT);
         hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSTime, "",
                                  WS_CHILD | lStyle, 0, 0, 10, 10,
                                  hWndParent, (HMENU) nCtrlID,
                                  g_hInstanceDLL, 0 );

         TimeGetFormat( hWndCtrl, &lpSS->DefaultTimeFormat );
         break;
#endif

#ifndef SS_NOCT_DATE
      case SS_TYPE_DATE:
         lStyle = CellType->lStyle & ~(ES_CENTER | ES_RIGHT);
         hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSDate, "",
                                  WS_CHILD | lStyle, 0, 0, 10, 10,
                                  hWndParent, (HMENU) nCtrlID,
                                  g_hInstanceDLL, 0 );

         DateGetFormat( hWndCtrl, &lpSS->DefaultDateFormat );
#ifdef SS_USEAWARE
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, ESX_NOFORMATTEDEDIT );
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, ESX_AUTOADVANCE );
         SendMessage( hWndCtrl, EM_SETEXTSTYLE, TRUE, ESX_NULLSUPPORT );
#endif
         break;
#endif

      case SS_TYPE_PICTURE:
         if ( CellType->lStyle & VPS_HANDLE )
         {
            hWndCtrl = CreateWindow( SSClassNames[ 0 ].TBViewPict,
                                     0, WS_CHILD | CellType->lStyle, 0, 0, 10,
                                     10, hWndParent, (HMENU) nCtrlID,
                                     (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                                     0 );

            SendMessage( hWndCtrl, VPM_SETPICTURE, (WORD) CellType->lStyle,
                         (zLONG)CellType->Spec.ViewPict.hPictName );

            if ( CellType->Spec.ViewPict.hPal )
               SendMessage( hWndCtrl, VPM_SETPALETTE, 0,
                            (zLONG)CellType->Spec.ViewPict.hPal );
         }
         else
         {
//          PictName = (zPCHAR) DrLockTaskMemory( CellType->Spec.ViewPict.hPictName );
            PictName = (zPCHAR) CellType->Spec.ViewPict.hPictName;

            hWndCtrl = CreateWindow( SSClassNames[ 0 ].TBViewPict,
                                     PictName, WS_CHILD | CellType->lStyle, 0, 0,
                                     10, 10, hWndParent, (HMENU) nCtrlID,
                                     (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE ),
                                     0 );

//          DrUnlockTaskMemory( PictName );
         }

         break;

#ifndef SS_NOCT_BUTTON
      case SS_TYPE_BUTTON:
         lpText = 0;

         if ( CellType->Spec.Button.hText )
         {
//          lpText = (zPCHAR) DrLockTaskMemory( CellType->Spec.Button.hText );
            lpText = (zPCHAR) CellType->Spec.Button.hText;
            hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSSuperBtn, lpText,
                                     WS_CHILD | CellType->lStyle, 0, 0, 0, 0,
                                     hWndParent, (HMENU) nCtrlID,
                                     g_hInstanceDLL, 0 );

//          DrUnlockTaskMemory( lpText );
         }

         break;
#endif

#ifndef SS_NOCT_CHECKBOX
      case SS_TYPE_CHECKBOX:
         lpText = 0;

         if ( CellType->Spec.CheckBox.hText )
         {
//          lpText = (zPCHAR) DrLockTaskMemory( CellType->Spec.CheckBox.hText );
            lpText = (zPCHAR) CellType->Spec.CheckBox.hText;

            hWndCtrl = CreateWindow( SSClassNames[ 0 ].SSCheckBox, lpText,
                                     WS_CHILD | CellType->lStyle, 0, 0, 0, 0,
                                     hWndParent, (HMENU) nCtrlID,
                                     g_hInstanceDLL, 0 );

//          DrUnlockTaskMemory( lpText );
         }

         break;
#endif
   }

   if ( hWndCtrl == 0 )
      nCtrlID = 0;
   else
   {
      fRet = SS_TypeControlAlloc( &lpSS->hControls,
                                  &lpSS->lControlsAllocCnt,
                                  &lpSS->lControlsCnt, hWndCtrl,
                                  CellType->chType, nCtrlID,
                                  CellType->lStyle );

      if ( !fRet )
      {
//       DrUnlockTaskMemory( lpSS );
         return( 0 );
      }
   }

   CellType->nControlID = nCtrlID;

// DrUnlockTaskMemory( lpSS );
   return( nCtrlID );
}

zPCHAR
SS_FormatComboBox( LPSS_CELLTYPE lpCellType,
                   zPCHAR        pchText,
                   zPCHAR        pchTextFmt,
                   zBOOL         fValue )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_FormatComboBox" );
   zPCHAR        lpItems;
   zBOOL         fRet = TRUE;
   zBOOL         fFound = FALSE;
   zLONG         lIndex;
   zSHORT        nLth;
   zUSHORT       k;

   zmemset( pchTextFmt, 0, 2 );
   nLth = 2;

   if ( fValue )
      lIndex = zatol( pchText );

   if ( lpCellType->Spec.ComboBox.hItems )
   {
//    lpItems = (zPCHAR) DrLockTaskMemory( lpCellType->Spec.ComboBox.hItems );
      lpItems = (zPCHAR) lpCellType->Spec.ComboBox.hItems;

      for ( k = 0; k < lpCellType->Spec.ComboBox.nItemCnt; k++ )
      {
         if ( (fValue && k == lIndex) || (!fValue && zstricmp( pchText, lpItems ) == 0) )
         {
            strcpy_s( pchTextFmt, nLth, lpItems );  // don't know about nLth (=2) here ... dks 2015.11.18
            nLth = zstrlen( pchTextFmt ) + 1;
            wsprintf( &pchTextFmt[ nLth ], "%d", k );
            nLth += zstrlen( &pchTextFmt[ nLth ] ) + 1;
            fFound = TRUE;
            break;
         }

         lpItems += zstrlen( lpItems ) + 1;
      }

//    DrUnlockTaskMemory( lpItems );
   }

   if ( !fFound && (lpCellType->lStyle & SS_CB_DROPDOWN) && pchText &&  zstrlen( pchText ) )
   {
      strcpy_s( pchTextFmt, nLth, pchText );
      nLth = zstrlen( pchTextFmt ) + 1;
      wsprintf( &pchTextFmt[ nLth ], "%d", -1 );
      nLth += zstrlen( &pchTextFmt[ nLth ] ) + 1;
   }

   zPCHAR hText;
   if ( DrAllocTaskMemory( (zCOREMEM) &hText, nLth, 1035 ) != 0 )
      return( 0 );

   zPCHAR pchTextTemp = (zPCHAR) DrLockTaskMemory( hText );
   zmemcpy( pchTextTemp, pchTextFmt, nLth );
// DrUnlockTaskMemory( pchTextTemp );

   return( hText );
}

zBOOL
SS_FormatData( LPSPREADSHEET  lpSS,
               LPSS_DATA      lpData,
               LPSS_CELLTYPE  lpCellType,
               zPCHAR         pchText,
               zBOOL          fValue )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_FormatData" );
   zPCHAR        hText;
   zPCHAR        pchTextTemp;
   zCHAR         szTextFmtBuffer[ SS_MAXDATALTH ];
   zPCHAR        pchTextFmt;
   zBOOL         bRC = FALSE;
   zDECIMAL      dxValue;

   pchTextFmt = (zPCHAR) szTextFmtBuffer;
   *pchTextFmt =  0;

   SSx_FreeData( lpData );
   switch ( lpCellType->chType )
   {
      case SS_TYPE_PIC:
#ifndef SS_NOCT_PIC
         bRC = SS_FormatDataPic( lpCellType, pchText, pchTextFmt, fValue );
#endif
         break;

      case SS_TYPE_INTEGER:
#ifndef SS_NOCT_INT
      {
         zLONG lValue;

         if ( fValue == SS_VALUE_INT )
         {
            lpData->bDataType = SS_TYPE_INTEGER;
            lpData->Data.lValue = *(zPLONG) pchText;
         }
         else
         if ( SS_FormatDataInteger( lpCellType, pchText,
                                    pchTextFmt, &lValue, fValue ) )
         {
            lpData->bDataType = SS_TYPE_INTEGER;
            lpData->Data.lValue = lValue;
         }
         else
         {
            lpData->bDataType = 0;
            lpData->Data.hszData = 0;
         }
      }
#endif

         return( TRUE );

      case SS_TYPE_FLOAT:
         if ( fValue == SS_VALUE_FLOAT )
         {
            lpData->m_DataType = SS_TYPE_FLOAT;
            lpData->m_Data.dxValue = *(zDECIMAL *) pchText;
         }
         else
         if ( SS_FormatDataFloat( lpSS, lpCellType, pchText,
                                  pchTextFmt, &dxValue, fValue ) )
         {
            lpData->m_DataType = SS_TYPE_FLOAT;
            lpData->m_Data.dxValue = dxValue;
         }
         else
         {
            lpData->m_DataType = 0;
            lpData->m_Data.hszData = 0;
         }

         return( TRUE );

      case SS_TYPE_EDIT:
         if ( zstrlen( pchText ) == 0 )
            return( TRUE );
         else
         if ( (hText = SS_FormatDataEdit( lpCellType, pchText,
                                          pchTextFmt, fValue )) != 0 )
         {
            lpData->m_DataType = SS_TYPE_EDIT;
            lpData->m_Data.hszData = hText;
            return( TRUE );
         }

         break;

      case SS_TYPE_TIME:
#ifndef SS_NOCT_TIME
         bRC = SS_FormatDataTime( lpCellType, pchText, pchTextFmt, fValue );
#endif
         break;

      case SS_TYPE_DATE:
#ifndef SS_NOCT_DATE
         bRC = SS_FormatDataDate( lpCellType, pchText, pchTextFmt, fValue );
#endif
         break;

      case SS_TYPE_COMBOBOX:
#ifndef SS_NOCT_COMBO
         if ( (hText = SS_FormatComboBox( lpCellType, pchText,
                                          pchTextFmt, fValue )) != 0 )
         {
            lpData->m_DataType = SS_TYPE_EDIT;
            lpData->m_Data.hszData = hText;
            return( TRUE );
         }
#endif
         break;

      case SS_TYPE_STATICTEXT:
      case SS_TYPE_OWNERDRAW:
      case SS_TYPE_CUSTOM:
      case SS_TYPE_PICTURE:
      case SS_TYPE_BUTTON:
      case SS_TYPE_CHECKBOX:
      default:
         pchTextFmt = pchText;
         bRC = TRUE;
         break;
   }

   if ( bRC )
   {
      if ( *pchTextFmt == 0 )
      {
         lpData->m_DataType = 0;
#ifndef SS_NOOVERFLOW
         lpData->m_bOverflow = 0;
#endif
         lpData->m_Data.hszData = 0;
      }
      else
      {
         zLONG lFmtLth = zstrlen( pchTextFmt ) + 1;
         if ( DrAllocTaskMemory( (zCOREMEM) &hText, lFmtLth, 1036 ) != 0 )
         {
            return( FALSE );
         }

         pchTextTemp = (zPCHAR) DrLockTaskMemory( hText );
         strcpy_s( pchTextTemp, lFmtLth, pchTextFmt );
         SSx_FreeData( lpData );
         lpData->m_DataType = SS_TYPE_EDIT;

#ifndef SS_NOOVERFLOW
         lpData->m_bOverflow = 0;
#endif
         lpData->m_Data.hszData = pchTextTemp;
      }

      return( TRUE );
   }

   return( FALSE );
}

zPCHAR
SS_FormatDataEdit( LPSS_CELLTYPE lpCellType,
                   zPCHAR        pchText,
                   zPCHAR        pchTextFmt,
                   zBOOL         fValue )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_FormatDataEdit" );
   zPCHAR        hText;
   zBOOL         bRC = TRUE;
   zULONG        lLth;
   zUSHORT       k;

   lLth = mMin( (zULONG) lpCellType->Spec.Edit.nLth, zstrlen( pchText ) );

   if ( lLth == 0 )
      return( 0 );

   if ( DrAllocTaskMemory( (zCOREMEM) &hText, lLth + 1, 1037 ) != 0 )
      return( 0 );

   pchTextFmt = (zPCHAR) DrLockTaskMemory( hText );
   pchTextFmt[ lLth ] =  0;

   if ( lpCellType->Spec.Edit.nChrSet == SS_CHRSET_CHR &&
        lpCellType->Spec.Edit.uCase == SS_CASE_NOCASE )
   {
      zmemcpy( pchTextFmt, pchText, lLth );
   }
   else
   {
      for ( k = 0; bRC && k < lLth; k++ )
      {
         if ( SS_EditIsValidKey( lpCellType, pchText[ k ] ) )
         {
            if ( lpCellType->Spec.Edit.uCase == SS_CASE_UCASE )
            {
               pchTextFmt[ k ] = (char)(zLONG)
                        AnsiUpper( (zPCHAR) MAKELONG( pchText[ k ], 0 ) );
            }
            else
            if ( lpCellType->Spec.Edit.uCase == SS_CASE_LCASE )
            {
               pchTextFmt[ k ] = (char)(zLONG)
                        AnsiLower( (zPCHAR) MAKELONG( pchText[ k ], 0 ) );
            }
            else
               pchTextFmt[ k ] = pchText[ k ];
         }
         else
            bRC = FALSE;
      }
   }

   if ( bRC == FALSE )
   {
      DrUnlockTaskMemory( pchTextFmt );
      DrFreeTaskMemory( hText );
      hText = 0;
   }

   return( hText );
}

LPSS_CELLTYPE
SS_RetrieveCellType( LPSPREADSHEET lpSS,
                     LPSS_CELLTYPE CellType,
                     LPSS_CELL     Cell,
                     SS_COORD      CellCol,
                     SS_COORD      CellRow )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_RetrieveCellType" );
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;

   lpCol = SS_LockColItem( lpSS, CellCol );
   lpRow = SS_LockRowItem( lpSS, CellRow );

   SSx_RetrieveCellType( lpSS, CellType, lpCol, lpRow,
                         Cell, CellCol, CellRow );

   SS_UnlockColItem( lpSS, CellCol );
   SS_UnlockRowItem( lpSS, CellRow );

   return( CellType );
}

HWND
SS_RetrieveControlhWnd( HWND hWnd,
                        LPSPREADSHEET lpSS,
                        SS_COORD ssCol,
                        SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_RetrieveControlhWnd" );
   zPCHAR        hCellType = 0;
   LPSS_CELLTYPE lpCellType = 0;
   LPSS_CELL     lpCell;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   HWND          hWndCtrl = 0;

   lpCol = SS_LockColItem( lpSS, ssCol );
   lpRow = SS_LockRowItem( lpSS, ssRow );
   lpCell = SSx_LockCellItem( lpSS, lpRow, ssCol, ssRow );

   if ( lpCell && lpCell->m_hCellType && ssCol != SS_ALLCOLS &&
        ssRow != SS_ALLROWS )
   {
      hCellType = lpCell->m_hCellType;
   }
   else
   if ( ssRow != SS_ALLROWS && lpRow && ssRow < lpSS->Row.nHeaderCnt &&
        lpRow->hCellType )
   {
      hCellType = lpRow->hCellType;
   }
   else
   if ( ssCol != SS_ALLCOLS && lpCol && lpCol->hCellType )
      hCellType = lpCol->hCellType;
   else
   if ( ssRow != SS_ALLROWS && lpRow && lpRow->hCellType )
      hCellType = lpRow->hCellType;
   else
      lpCellType = &lpSS->DefaultCellType;

   SS_UnlockCellItem( lpSS, ssCol, ssRow );
   SS_UnlockColItem( lpSS, ssCol );
   SS_UnlockRowItem( lpSS, ssRow );

   if ( hCellType || lpCellType )
   {
      if ( hCellType )
      {
//       lpCellType = (LPSS_CELLTYPE) DrLockTaskMemory( hCellType );
         lpCellType = (LPSS_CELLTYPE) hCellType;

         if ( lpCellType->nControlID == 0 )
         {
            if ( SS_CreateControl( hWnd, lpCellType ) == 0 )
               return( 0 );
         }

         hWndCtrl = SS_GetControlhWnd( lpSS, lpCellType->nControlID );

//       DrUnlockTaskMemory( lpCellType );
      }
   }

   return( hWndCtrl );
}

int
SS_StrReplaceCh( zPCHAR   szStr,
                 register WORD  ch )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_StrReplaceCh" );
   zPCHAR szpch = szStr;

   while ( *szStr )
   {
      if ( *szStr == (char) HIBYTE( ch ) )
         *szStr = (char) LOBYTE( ch );

      ++szStr;
   }

   return( (int) (szStr - szpch) );
}

zBOOL
SS_TypeControlAlloc( zPCHAR  *lphControls,
                     zPLONG lpControlsAllocCnt,
                     zPLONG lpControlsCnt,
                     HWND    hWndCtrl,
                     zSHORT  nType,
                     zSHORT  nCtrlID,
                     zLONG   lStyle )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_TypeControlAlloc" );
   zSHORT       nIndex;

   nIndex = nCtrlID - SS_CONTROL_BASEID;

   if ( nIndex + 1 > *lpControlsAllocCnt )
   {
      if ( *lpControlsAllocCnt )
      {
         *lphControls = (zPCHAR) DrUnlockTaskMemory( *lphControls );
         DrFreeTaskMemory( *lphControls );
      }

      DrAllocTaskMemory( (zCOREMEM) lphControls,
                         (zLONG)(zsizeof( SS_CONTROL ) *
                          (*lpControlsAllocCnt + SS_TYPE_ALLOC_CNT)), 1039 );
      if ( *lphControls == 0 )
         return( FALSE );

      *lpControlsAllocCnt += SS_TYPE_ALLOC_CNT;
      *lphControls = (zPCHAR) DrLockTaskMemory( *lphControls );
   }

   LPSS_CONTROL lpControls = (LPSS_CONTROL) *lphControls;

   lpControls[ nIndex ].hWnd = hWndCtrl;
   lpControls[ nIndex ].chType = (char) nType;
   lpControls[ nIndex ].nCtrlID = nCtrlID;
   lpControls[ nIndex ].lStyle = lStyle;

   if ( nIndex + 1 > *lpControlsCnt )
      *lpControlsCnt = nIndex + 1;

// DrUnlockTaskMemory( lpControls );
   return( TRUE );
}

zSHORT
SS_TypeGetControlType( LPSPREADSHEET lpSS,
                       zSHORT nCtrlID )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_TypeGetControlType" );
   LPSS_CONTROL lpControls;
   zSHORT       nType = -1;
   zUSHORT       k;

// lpControls = (LPSS_CONTROL) DrLockTaskMemory( lpSS->hControls );
   lpControls = (LPSS_CONTROL) lpSS->hControls;
   for ( k = 0; k < lpSS->lControlsCnt; k++ )
   {
      if ( nCtrlID == lpControls[ k ].nCtrlID )
      {
         nType = lpControls[ k ].chType;
         break;
      }
   }

// DrUnlockTaskMemory( lpControls );

   return( nType );
}

LPSS_CELLTYPE
SSx_RetrieveCellType( LPSPREADSHEET lpSS,
                      LPSS_CELLTYPE CellType,
                      LPSS_COL      lpCol,
                      LPSS_ROW      lpRow,
                      LPSS_CELL     Cell,
                      SS_COORD      CellCol,
                      SS_COORD      CellRow )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSx_RetrieveCellType" );
   LPSS_CELLTYPE CellTypeTemp;
   zBOOL         fCellLock = FALSE;

   if ( !Cell && CellCol != SS_ALLCOLS && CellRow != SS_ALLROWS )
   {
      if ( (Cell = SSx_LockCellItem( lpSS, lpRow, CellCol, CellRow )) != 0 )
         fCellLock = TRUE;
   }

   if ( Cell && Cell->m_hCellType &&
        CellCol != SS_ALLCOLS && CellRow != SS_ALLROWS )
   {
//    CellTypeTemp = (LPSS_CELLTYPE) DrLockTaskMemory( Cell->m_hCellType );
      CellTypeTemp = (LPSS_CELLTYPE) Cell->m_hCellType;
      zmemcpy( CellType, CellTypeTemp, zsizeof( SS_CELLTYPE ) );
//    DrUnlockTaskMemory( CellTypeTemp );
   }
   else
   if ( CellRow != SS_ALLROWS && lpRow &&
        CellRow < lpSS->Row.nHeaderCnt && lpRow->hCellType )
   {
//    CellTypeTemp = (LPSS_CELLTYPE) DrLockTaskMemory( lpRow->hCellType );
      CellTypeTemp = (LPSS_CELLTYPE) lpRow->hCellType;
      zmemcpy( CellType, CellTypeTemp, zsizeof( SS_CELLTYPE ) );
//    DrUnlockTaskMemory( CellTypeTemp );
   }
   else
   if ( CellCol != SS_ALLCOLS && lpCol && lpCol->hCellType )
   {
//    CellTypeTemp = (LPSS_CELLTYPE) DrLockTaskMemory( lpCol->hCellType );
      CellTypeTemp = (LPSS_CELLTYPE) lpCol->hCellType;
      zmemcpy( CellType, CellTypeTemp, zsizeof( SS_CELLTYPE ) );
//    DrUnlockTaskMemory( CellTypeTemp );
   }
   else
   if ( CellRow != SS_ALLROWS && lpRow && lpRow->hCellType )
   {
//    CellTypeTemp = (LPSS_CELLTYPE) DrLockTaskMemory( lpRow->hCellType );
      CellTypeTemp = (LPSS_CELLTYPE) lpRow->hCellType;
      zmemcpy( CellType, CellTypeTemp, zsizeof( SS_CELLTYPE ) );
//    DrUnlockTaskMemory( CellTypeTemp );
   }
   else
      zmemcpy( CellType, &lpSS->DefaultCellType, zsizeof( SS_CELLTYPE ) );

   if ( fCellLock )
      SSx_UnlockCellItem( lpSS, lpRow, CellCol, CellRow );

#if 0
   if ( CellRow == 0 )
   {
      TraceLineI( "RetrieveCellType Cell ", (zLONG) Cell );
      TraceLineI( "                 CellCol ", CellCol );
      TraceLineI( "                 lpCol ", (zLONG) lpCol );
      TraceLineI( "                 lpRow ", (zLONG) lpRow );
      TraceLineI( "                 Type ", CellType->chType );
      TraceLineI( "                lStyle ", CellType->lStyle );
      TraceLineI( "================== Id ", CellType->nControlID );
   }
#endif

   return( CellType );
}

zPCHAR
SS_UnFormatData( LPSPREADSHEET lpSS,
                 LPSS_CELLTYPE lpCellType,
                 zPCHAR        pchText )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_UnFormatData" );
   zPCHAR         hText;
   zPCHAR         pchTextTemp;
   zCHAR          szTextValueBuffer[ SS_MAXDATALTH ];
   zPCHAR         pchTextValue;
   zBOOL          bRC = FALSE;

   pchTextValue = szTextValueBuffer;
   zmemset( szTextValueBuffer, 0, zsizeof( szTextValueBuffer ) );

   switch ( lpCellType->chType )
   {
      case SS_TYPE_PIC:
#ifndef SS_NOCT_PIC
         bRC = SS_UnFormatDataPic( lpCellType, pchText, pchTextValue );
#endif
         break;

      case SS_TYPE_DATE:
#ifndef SS_NOCT_DATE
         bRC = SS_UnFormatDataDate( lpCellType, pchText, pchTextValue );
#endif
         break;

      case SS_TYPE_TIME:
#ifndef SS_NOCT_TIME
         bRC = SS_UnFormatDataTime( lpCellType, pchText, pchTextValue );
#endif
         break;

      case SS_TYPE_EDIT:
      case SS_TYPE_FLOAT:
      case SS_TYPE_INTEGER:
      {
         zLONG lTextLth = zstrlen( pchText ) + 1;
         if ( DrAllocTaskMemory( (zCOREMEM) &hText, lTextLth, 1040 ) != 0 )
         {
            return( 0 );
         }

         pchTextTemp = (zPCHAR) DrLockTaskMemory( hText );
         strcpy_s( pchTextTemp, lTextLth, pchText );
//       DrUnlockTaskMemory( pchTextTemp );

         return( pchTextTemp );
      }

      case SS_TYPE_COMBOBOX:
         pchTextValue = pchText + zstrlen( pchText ) + 1;
         bRC = TRUE;
         break;

      case SS_TYPE_STATICTEXT:
      case SS_TYPE_OWNERDRAW:
      case SS_TYPE_CUSTOM:
      case SS_TYPE_PICTURE:
      case SS_TYPE_BUTTON:
      case SS_TYPE_CHECKBOX:
      default:
         pchTextValue = pchText;
         bRC = TRUE;
         break;
   }

   if ( bRC )
   {
      zLONG lTextLth = mMin( zstrlen( pchTextValue ), SS_MAXDATALTH ) + 1;
      if ( DrAllocTaskMemory( (zCOREMEM) &hText, lTextLth, 1041 ) != 0 )
      {
         return( 0 );
      }

      pchTextTemp = (zPCHAR) DrLockTaskMemory( hText );
      strncpy_s( pchTextTemp, lTextLth, pchTextValue, lTextLth - 1 );
//    DrUnlockTaskMemory( pchTextTemp );

      return( pchTextTemp );
   }

   return( 0 );
}


// ss_user.cpp

zBOOL OPERATION
SSClear( HWND          hWnd,
         SS_COORD      ssCol,
         SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSClear" );
   return( SSClearRange( hWnd, ssCol, ssRow, ssCol, ssRow ) );
}

zBOOL OPERATION
SSClearRange( HWND          hWnd,
              SS_COORD      ssCol,
              SS_COORD      ssRow,
              SS_COORD      ssCol2,
              SS_COORD      ssRow2 )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSClearRange" );
   if ( !hWnd )
      return( FALSE );

   return( SS_ClearRange( hWnd, ssCol, ssRow, ssCol2, ssRow2, FALSE ) );
}

void OPERATION
SSGetClientRect( HWND hWnd,
                 LPRECT  lpRect )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetClientRect" );
   SS_GetClientRect( hWnd, lpRect );
}

#ifdef SS_USE16BITCOORDS
zLONG OPERATION
SSGetTopLeftCell( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetTopLeftCell" );
   LPSPREADSHEET lpSS;
   zLONG         lLocation;

   lpSS = SS_Lock( hWnd );
   Location = MAKELONG( lpSS->Col.UL, lpSS->Row.UL );
// DrUnlockTaskMemory( lpSS );

   return( Location );
}

zLONG OPERATION
SSGetBottomRightCell( HWND hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetBottomRightCell" );
   LPSPREADSHEET lpSS;
   zLONG         lLocation;

   lpSS = SS_Lock( hWnd );
   Location = MAKELONG( lpSS->Col.LR, lpSS->Row.LR );
// DrUnlockTaskMemory( lpSS );

   return( Location );
}

#else

zBOOL OPERATION
SSGetTopLeftCell( HWND hWnd,
                  LPSS_COORD lpCol,
                  LPSS_COORD lpRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetTopLeftCell" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = TRUE;

   lpSS = SS_Lock( hWnd );

   if ( lpCol )
      *lpCol = lpSS->Col.UL;

   if ( lpRow )
      *lpRow = lpSS->Row.UL;

// DrUnlockTaskMemory( lpSS );

   return( bRC );
}

zBOOL OPERATION
SSGetBottomRightCell( HWND hWnd,
                      LPSS_COORD lpCol,
                      LPSS_COORD lpRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetBottomRightCell" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = TRUE;

   lpSS = SS_Lock( hWnd );

   if ( lpCol )
      *lpCol = lpSS->Col.LR;

   if ( lpRow )
      *lpRow = lpSS->Row.LR;

// DrUnlockTaskMemory( lpSS );

   return( bRC );
}
#endif

void OPERATION
SSGetTopLeftPrev( HWND hWnd,
                  LPSS_COORD lpCol,
                  LPSS_COORD lpRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetTopLeftPrev" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   *lpCol = lpSS->Col.ULPrev;
   *lpRow = lpSS->Row.ULPrev;

// DrUnlockTaskMemory( lpSS );
}

zSHORT OPERATION
SSGetUserResizeCol( HWND hWnd, SS_COORD ssCol )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetUserResizeCol" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   zSHORT        nRC = SS_RESIZE_DEFAULT;

   if ( (lpSS = SS_Lock( hWnd )) != 0 )
   {
      if ( ssCol == SS_ALLCOLS )
      {
         if ( lpSS->wUserResize & SS_USERRESIZE_COL )
            nRC = SS_RESIZE_ON;
         else
            nRC = SS_RESIZE_OFF;
      }
      else
      if ( ssCol >= SS_GetColsMax( lpSS ) )
         nRC = -1;
      else
      if ( (lpCol = SS_LockColItem( lpSS, ssCol )) != 0 )
      {
         nRC = lpCol->bUserResize;
         SS_UnlockColItem( lpSS, ssCol );
      }

//    DrUnlockTaskMemory( lpSS );
   }

   return( nRC );
}

zSHORT OPERATION
SSSetUserResizeCol( HWND hWnd,
                    SS_COORD ssCol,
                    zSHORT nOption )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetUserResizeCol" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   zSHORT        nRC = SS_RESIZE_DEFAULT;
   zUSHORT       k;

   if ( (lpSS = SS_Lock( hWnd )) != 0 )
   {
      if ( ssCol == SS_ALLCOLS )
      {
         if ( lpSS->wUserResize & SS_USERRESIZE_COL )
            nRC = SS_RESIZE_ON;
         else
            nRC = SS_RESIZE_OFF;

         if ( nOption == SS_RESIZE_ON )
            lpSS->wUserResize |= SS_USERRESIZE_COL;
         else
            lpSS->wUserResize &= ~SS_USERRESIZE_COL;

         for ( k = 0; k < lpSS->Col.AllocCnt; k++ )
         {
            if ( ( lpCol = SS_LockColItem( lpSS, k ) ) != 0 )
            {
               lpCol->bUserResize = SS_RESIZE_DEFAULT;
               SS_UnlockColItem( lpSS, k );
            }
         }
      }

      else
      {
         SSx_AdjustCellCoords( lpSS, &ssCol, 0 );

         if ( ssCol >= SS_GetColsMax( lpSS ) )
         {
//          DrUnlockTaskMemory( lpSS );
            return( -1 );
         }

         if ( ( lpCol = SS_AllocLockCol( lpSS, ssCol ) ) != 0 )
         {
            nRC = lpCol->bUserResize;
            lpCol->bUserResize = (BYTE) nOption;
            SS_UnlockColItem( lpSS, ssCol );
         }
      }

//    DrUnlockTaskMemory( lpSS );
   }

   return( nRC );
}

zSHORT OPERATION
SSGetUserResizeRow( HWND     hWnd,
                    SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetUserResizeRow" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   zSHORT        nRC = SS_RESIZE_DEFAULT;

   if ( (lpSS = SS_Lock( hWnd )) != 0 )
   {
      if ( ssRow == SS_ALLROWS )
      {
         if ( lpSS->wUserResize & SS_USERRESIZE_ROW )
            nRC = SS_RESIZE_ON;
         else
            nRC = SS_RESIZE_OFF;
      }
      else
      if ( ssRow >= SS_GetRowsMax( lpSS ) )
         nRC = -1;

      if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
      {
         nRC = lpRow->bUserResize;
         SS_UnlockRowItem( lpSS, ssRow );
      }

//    DrUnlockTaskMemory( lpSS );
   }

   return( nRC );
}

void OPERATION
SSSetAutoSizeVisible( HWND hWnd,
                      SS_COORD VisibleCols,
                      SS_COORD VisibleRows )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetAutoSizeVisible" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   lpSS->Col.AutoSizeVisibleCnt = VisibleCols;
   lpSS->Row.AutoSizeVisibleCnt = VisibleRows;

   if ( lpSS->fAutoSize )
      SS_AutoSize( hWnd, FALSE );

// DrUnlockTaskMemory( lpSS );
}

WNDPROC OPERATION
SSSetCallBack( HWND hWnd,
               WNDPROC lpfnCallBack )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetCallBack" );
   LPSPREADSHEET lpSS;
   WNDPROC       lpfnCallBackOld;

   lpSS = SS_Lock( hWnd );
   lpfnCallBackOld = lpSS->lpfnCallBack;
   lpSS->lpfnCallBack = lpfnCallBack;
// DrUnlockTaskMemory( lpSS );

   return( lpfnCallBackOld );
}

zBOOL OPERATION
SSSetCellType( HWND          hWnd,
               SS_COORD      ssCol,
               SS_COORD      ssRow,
               LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetCellType" );
   return( SSSetCellTypeRange( hWnd, ssCol, ssRow, ssCol, ssRow, CellType ) );
}

zBOOL OPERATION
SSSetCellTypeRange( HWND          hWnd,
                    SS_COORD      ssCol,
                    SS_COORD      ssRow,
                    SS_COORD      ssCol2,
                    SS_COORD      ssRow2,
                    LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetCellTypeRange" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   SS_COORD      x;
   SS_COORD      y;
   zBOOL         RedrawOld;
   zBOOL         bRC = FALSE;

   SS_AdjustCellRangeCoords( lpSS, &ssCol, &ssRow, &ssCol2, &ssRow2 );
   if ( ssRow != ssRow2 || ssCol != ssCol2 )
   {
      RedrawOld = lpSS->Redraw;
      lpSS->Redraw = FALSE;
   }

   for ( y = ssRow; y <= ssRow2; y++ )
   {
      for ( x = ssCol; x <= ssCol2; x++ )
      {
         if ( x != SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SSSetCellCellType( hWnd, lpSS, x, y, CellType );
         else
         if ( x != SS_ALLCOLS && y == SS_ALLROWS )
            bRC = SSSetColCellType( hWnd, lpSS, x, CellType );
         else
         if ( x == SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SSSetRowCellType( hWnd, lpSS, y, CellType );
         else
            bRC = SSSetDocDefaultCellType( hWnd, lpSS, CellType );

         if ( bRC == FALSE )
            break;
      }
   }

   if ( ssRow != ssRow2 || ssCol != ssCol2 )
      SSSetBool( hWnd, SSB_REDRAW, RedrawOld );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

WORD  OPERATION
SSSetColHeaderDisplay( HWND hWnd,
                       WORD  wDisplay )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetColHeaderDisplay" );
   LPSPREADSHEET lpSS;
   WORD          wDisplayOld;

   lpSS = SS_Lock( hWnd );

   wDisplayOld = lpSS->ColHeaderDisplay;
   lpSS->ColHeaderDisplay = wDisplay;

   SS_InvalidateRow( hWnd, lpSS, 0 );

// DrUnlockTaskMemory( lpSS );
   return( wDisplayOld );
}


WORD  OPERATION
SSSetOperationMode( HWND hWnd,
                    WORD  wMode )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetOperationMode" );
   LPSPREADSHEET lpSS;
   WORD          wModeOld;

   lpSS = SS_Lock( hWnd );
   wModeOld = lpSS->wOpMode;

   if ( wMode <= SS_OPMODE_EXTSEL )
   {
      if ( wMode == SS_OPMODE_READONLY )
         SS_HiliteCell( hWnd, FALSE );

      lpSS->wOpMode = wMode;

      if ( wMode == SS_OPMODE_ROWMODE || wMode == SS_OPMODE_SINGLESEL ||
           wMode == SS_OPMODE_MULTISEL || wMode == SS_OPMODE_EXTSEL )
      {
         lpSS->wSelBlockOption = 0;
      }
      else
      if ( wMode == SS_OPMODE_NORMAL )
         lpSS->wSelBlockOption |= SS_SELBLOCK_COLS | SS_SELBLOCK_ROWS |
                                  SS_SELBLOCK_BLOCKS | SS_SELBLOCK_ALL;

      if ( lpSS->fEditModePermanent && lpSS->fAllowEditModePermSel == 0 )
         lpSS->wSelBlockOption = 0;

      if ( wMode == SS_OPMODE_MULTISEL || wModeOld == SS_OPMODE_MULTISEL ||
           wMode == SS_OPMODE_EXTSEL || wModeOld == SS_OPMODE_EXTSEL )
      {
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
      }
      else
      if ( wMode == SS_OPMODE_SINGLESEL || wModeOld == SS_OPMODE_SINGLESEL ||
           wMode == SS_OPMODE_ROWMODE || wModeOld == SS_OPMODE_ROWMODE )
      {
         SS_InvalidateRow( hWnd, lpSS, lpSS->Row.ssCurrAt );
      }

      if ( wModeOld == SS_OPMODE_READONLY )
         SS_HiliteCell( hWnd, TRUE );
   }

// DrUnlockTaskMemory( lpSS );
   return( wModeOld );
}

void OPERATION
SSSetStartingNumbers( HWND hWnd,
                      SS_COORD StartingColNumber,
                      SS_COORD StartingRowNumber )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetStartingNumbers" );
   LPSPREADSHEET lpSS;
   SS_COORD      StartingColNumberOld;
   SS_COORD      StartingRowNumberOld;

   lpSS = SS_Lock( hWnd );

   StartingColNumberOld = lpSS->Col.NumStart;
   StartingRowNumberOld = lpSS->Row.NumStart;

   lpSS->Col.NumStart = StartingColNumber;
   lpSS->Row.NumStart = StartingRowNumber;

   if ( StartingColNumberOld != lpSS->Col.NumStart )
      SS_InvalidateRow( hWnd, lpSS, 0 );

   if ( StartingRowNumberOld != lpSS->Row.NumStart )
      SS_InvalidateCol( hWnd, lpSS, 0 );

// DrUnlockTaskMemory( lpSS );
}

zSHORT OPERATION
SSSetUserResizeRow( HWND hWnd,
                    SS_COORD ssRow,
                    zSHORT nOption )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetUserResizeRow" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   zSHORT        nRC = SS_RESIZE_DEFAULT;
   zUSHORT        k;

   if ( (lpSS = SS_Lock( hWnd )) != 0 )
   {
      if ( ssRow == SS_ALLROWS )
      {
         if ( lpSS->wUserResize & SS_USERRESIZE_ROW )
            nRC = SS_RESIZE_ON;
         else
            nRC = SS_RESIZE_OFF;

         if ( nOption == SS_RESIZE_ON )
            lpSS->wUserResize |= SS_USERRESIZE_ROW;
         else
            lpSS->wUserResize &= ~SS_USERRESIZE_ROW;

         for ( k = 0; k < lpSS->Row.AllocCnt; k++ )
         {
            if ( (lpRow = SS_LockRowItem( lpSS, k )) != 0 )
            {
               lpRow->bUserResize = SS_RESIZE_DEFAULT;
               SS_UnlockRowItem( lpSS, k );
            }
         }
      }
      else
      {
         SSx_AdjustCellCoords( lpSS, 0, &ssRow );
         if ( ssRow >= SS_GetRowsMax( lpSS ) )
         {
//          DrUnlockTaskMemory( lpSS );
            return( -1 );
         }

         if ( (lpRow = SS_AllocLockRow( lpSS, ssRow )) != 0 )
         {
            nRC = lpRow->bUserResize;
            lpRow->bUserResize = (BYTE) nOption;
            SS_UnlockRowItem( lpSS, ssRow );
         }
      }

//    DrUnlockTaskMemory( lpSS );
   }

   return( nRC );
}

zBOOL
SS_ClearRange( HWND          hWnd,
               SS_COORD      ssCol,
               SS_COORD      ssRow,
               SS_COORD      ssCol2,
               SS_COORD      ssRow2,
               zBOOL         fSendChangeMsg )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SS_ClearRange" );
   LPSPREADSHEET lpSS;
   zBOOL         RedrawOld;
   SS_COORD      x;
   SS_COORD      y;
   zBOOL         fTurnEditModeOn = FALSE;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( ssCol >= SS_GetColsMax( lpSS ) || ssRow >= SS_GetRowsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   SS_AdjustCellRangeCoords( lpSS, &ssCol, &ssRow, &ssCol2, &ssRow2 );

   if ( lpSS->EditModeOn )
   {
      SS_CellEditModeOff( hWnd, lpSS, 0 );
      fTurnEditModeOn = TRUE;
   }

   if ( lpSS->fVirtualMode )
   {
      ssRow = mMax( ssRow, lpSS->Virtual.VTop );

      if ( ssRow2 == -1 )
         ssRow2 = lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1;
      else
         ssRow2 = mMin( ssRow2,
                        lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 );
   }

   RedrawOld = lpSS->Redraw;
   lpSS->Redraw = FALSE;

   for ( y = ssRow; y <= ssRow2; y++ )
   {
      for ( x = ssCol; x <= ssCol2; x++ )
      {
         if ( x != SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SS_ClearCell( hWnd, x, y, TRUE, fSendChangeMsg );
         else
         if ( x != SS_ALLCOLS && y == SS_ALLROWS )
            bRC = SS_ClearCol( hWnd, x, TRUE, fSendChangeMsg );
         else
         if ( x == SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SS_ClearRow( hWnd, y, TRUE, fSendChangeMsg );
         else
            bRC = SSClearDoc( hWnd );

         if ( !bRC )
            break;
      }
   }

   SS_AdjustDataRowCnt( lpSS, ssRow, ssRow2 );
   SS_AdjustDataColCnt( lpSS, ssCol, ssCol2 );
   SSSetBool( hWnd, SSB_REDRAW, RedrawOld );

   if ( fTurnEditModeOn )
      SS_CellEditModeOn( hWnd, lpSS, 0, 0, 0L );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL
SS_GetLock( HWND          hWnd,
            LPSPREADSHEET lpSS,
            SS_COORD      ssCol,
            SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SS_GetLock" );
   LPSS_ROW      lpRow;
   LPSS_COL      lpCol;
   zBOOL         fLocked = FALSE;

   lpCol = SS_LockColItem( lpSS, ssCol );
   lpRow = SS_LockRowItem( lpSS, ssRow );

   if ( lpSS->DocumentProtected )
      fLocked = SSxx_GetLock( hWnd, lpSS, lpCol, lpRow, 0, ssCol, ssRow );

   SS_UnlockColItem( lpSS, ssCol );
   SS_UnlockRowItem( lpSS, ssRow );

   return( fLocked );
}

zBOOL
SSxx_GetLock( HWND          hWnd,
              LPSPREADSHEET lpSS,
              LPSS_COL      lpCol,
              LPSS_ROW      lpRow,
              LPSS_CELL     lpCell,
              SS_COORD      ssCol,
              SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSxx_GetLock" );
   LPSS_CELL     lpCellOrig = lpCell;
   zBOOL         fLocked;

   if ( ssRow != SS_ALLROWS && ssCol != SS_ALLCOLS &&
        (lpCell || (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0) &&
         lpCell->m_CellLocked )
   {
      fLocked = lpCell->m_CellLocked;
   }
   else
   if ( ssRow != SS_ALLROWS && lpRow && lpRow->fRowLocked )
      fLocked = lpRow->fRowLocked;
   else
   if ( ssCol != SS_ALLCOLS && lpCol && lpCol->chColLocked )
      fLocked = lpCol->chColLocked;
   else
      fLocked = lpSS->DocumentLocked;

   if ( lpCell && lpCellOrig == 0 )
      SS_UnlockCellItem( lpSS, ssCol, ssRow );

   if ( fLocked == SS_LOCKED_ON )
      return( TRUE );

   return( FALSE );
}

#endif
