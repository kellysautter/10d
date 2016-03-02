/////////////////////////////////////////////////////////////////////////////
// Project ZdSS
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlssx.cpp
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
// 1998.12.15    DKS
//    Fix to support LockColumn operations.
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

zBOOL OPERATION
SSClearData( HWND      hWnd,
             SS_COORD  ssCol,
             SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSClearData" );
   return( SS_ClearDataRange( hWnd, ssCol, ssRow, ssCol, ssRow, FALSE, FALSE ) );
}

zBOOL OPERATION
SSClearDataRange( HWND      hWnd,
                  SS_COORD  ssCol,
                  SS_COORD  ssRow,
                  SS_COORD  ssCol2,
                  SS_COORD  ssRow2 )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSClearDataRange" );
   return( SS_ClearDataRange( hWnd, ssCol, ssRow, ssCol2, ssRow2, FALSE, FALSE ) );
}

zBOOL OPERATION
SSClipIn( HWND      hWnd,
          SS_COORD  ssCol,
          SS_COORD  ssRow,
          SS_COORD  ssCol2,
          SS_COORD  ssRow2,
          HPSTR     Buffer,
          zLONG     lBufferLth )
{
#if 0
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSClipIn" );
   return( SS_ClipIn( hWnd, ssCol, ssRow, ssCol2, ssRow2, Buffer, lBufferLth,
                      FALSE, FALSE ) );
#else
   MESSAGE_DEBUG( "<SS_DATA.CPP>", "SSClipIn" );
   return( 0 );
#endif
}

zBOOL OPERATION
SSClipValueIn( HWND      hWnd,
               SS_COORD  ssCol,
               SS_COORD  ssRow,
               SS_COORD  ssCol2,
               SS_COORD  ssRow2,
               HPSTR     Buffer,
               zLONG     lBufferLth )
{
#if 0
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSClipValueIn" );
   return( SS_ClipIn( hWnd, ssCol, ssRow, ssCol2, ssRow2, Buffer, BufferLth, TRUE, FALSE ) );
#else
   MESSAGE_DEBUG( "<SS_DATA.CPP>", "SSClipValueIn" );
   return( 0 );
#endif
}

zPVOID  OPERATION
SSClipValueOut( HWND      hWnd,
                SS_COORD  ssCol,
                SS_COORD  ssRow,
                SS_COORD  ssCol2,
                SS_COORD  ssRow2 )
{
#if 0
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSClipValueOut" );
   return( SS_ClipOut( hWnd, ssCol, ssRow, ssCol2, ssRow2, TRUE ) );
#else
   MESSAGE_DEBUG( "<SS_DATA.CPP>", "SSClipValueOut" );
   return( 0 );
#endif
}

zSHORT OPERATION
SSColWidthToLogUnits( HWND   hWnd,
                      zLONG dxColWidth )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSColWidthToLogUnits" );
   LPSPREADSHEET lpSS;
   zSHORT        nUnits;

   lpSS = SS_Lock( hWnd );
   nUnits = SS_ColWidthToPixels( lpSS, dxColWidth );
// DrUnlockTaskMemory( lpSS );
   return( nUnits );
}

zLONG OPERATION
SSComboBoxSendMessage( HWND      hWnd,
                       SS_COORD  ssCol,
                       SS_COORD  ssRow,
                       WORD      wMsg,
                       WPARAM    wParam,
                       LPARAM    lParam )
{
#if 0
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSComboBoxSendMessage" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   LPSS_CELL     lpCell = 0;
   zPCHAR        lpItems;
   zLONG         lRet = 0L;
   zUSHORT       k;

   if ( ssCol != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &Col, 0 );

   if ( ssRow != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &Row );

   lpSS = SS_Lock( hWnd );

   if ( ssCol != SS_ALLCOLS && ssRow != SS_ALLROWS )
      lpCell = SS_LockCellItem( lpSS, ssCol, ssRow );

   if ( SS_RetrieveCellType( lpSS, &CellType, lpCell, ssCol, ssRow ) &&
        CellType.Type == SS_TYPE_COMBOBOX )
   {
      switch ( wMsg )
      {
         case CB_ADDSTRING:
            lRet = (zLONG) SS_ComboBoxInsert( &CellType, (zPCHAR) lParam, -1 );
            break;

         case CB_DELETESTRING:
            lRet = (zLONG) SS_ComboBoxDelete( &CellType, wParam );
            break;

         case CB_GETLBTEXT:
            if ( lParam && wParam < CellType.Spec.ComboBox.nItemCnt &&
                 CellType.Spec.ComboBox.nItemCnt &&
                 CellType.Spec.ComboBox.hItems )
            {
//             lpItems = (zPCHAR) DrLockTaskMemory( CellType.Spec.ComboBox.hItems );
               lpItems = (zPCHAR) CellType.Spec.ComboBox.hItems;
               for ( ss = 0; ss < (zSHORT) wParam; ss++ )
                  lpItems += zstrlen( lpItems ) + 1;

               lRet = zstrlen( lpItems );
               strcpy_s( (zPCHAR) lParam, commented out, lpItems );

//             DrUnlockTaskMemory( lpItems );
            }

            break;

         case CB_GETLBTEXTLEN:
            if ( (zSHORT) wParam < CellType.Spec.ComboBox.nItemCnt &&
                 CellType.Spec.ComboBox.nItemCnt &&
                 CellType.Spec.ComboBox.hItems )
            {
//             lpItems = (zPCHAR) DrLockTaskMemory( CellType.Spec.ComboBox.hItems );
               lpItems = (zPCHAR) CellType.Spec.ComboBox.hItems;
               for ( ss = 0; ss < wParam; ss++ )
                  lpItems += zstrlen( lpItems ) + 1;

               lRet = zstrlen( lpItems );

//             DrUnlockTaskMemory( lpItems );
            }

            break;

         case CB_INSERTSTRING:
            lRet = (zLONG) SS_ComboBoxInsert( &CellType,
                                             (zPCHAR) lParam, wParam );
            break;

         case CB_RESETCONTENT:
            CellType.Spec.ComboBox.nItemCnt = 0;

            if ( CellType.Spec.ComboBox.hItems )
            {
               CellType.Spec.ComboBox.hItems =
                        DrUnlockTaskMemory( CellType.Spec.ComboBox.hItems );
               DrFreeTaskMemory( CellType.Spec.ComboBox.hItems );
               CellType.Spec.ComboBox.hItems = 0;
            }

            lRet = 0;
            break;

         case CB_GETCOUNT:
            lRet = CellType.Spec.ComboBox.nItemCnt;
            break;

         case CB_GETCURSEL:
         {
            zCHAR Buffer[ 10 ];

            if ( SSGetValue( hWnd, ssCol, ssRow, Buffer ) )
               lRet = zatol( Buffer );

            break;
         }

         case CB_SETCURSEL:
         {
            zCHAR Buffer[10];

            _ltoa_s( (zLONG) wParam, Buffer );
            if ( !SSSetValue( hWnd, ssCol, ssRow, Buffer ) )
               lRet = -1;

            break;
         }

      }

      SS_SaveCellType( lpSS, &CellType, lpCell, ssCol, ssRow );
   }

   if ( lpCell )
      SS_UnlockCellItem( lpSS, ssCol, ssRow );

// DrUnlockTaskMemory( lpSS );
   return( lRet );
#else
   MESSAGE_DEBUG( "<SS_TYPE.CPP>", "SSComboBoxSendMessage" );
   return( 0 );
#endif
}

#ifdef DKS_COPY

zBOOL OPERATION
SSCopyRange( HWND hWnd, SS_COORD ssCol, SS_COORD ssRow,
             SS_COORD ssCol2, SS_COORD ssRow2, SS_COORD ssColDest,
             SS_COORD ssRowDest )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SSCopyRange" );
   return( SS_CopySwapRange( hWnd, ssCol, ssRow, ssCol2, ssRow2, ssColDest, ssRowDest,
                             SS_CMD_COPY, FALSE ) );
}

#endif

zBOOL OPERATION
SSDelCol( HWND      hWnd,
          SS_COORD  ssCol )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSDelCol" );
   return( SSDelColRange( hWnd, ssCol, ssCol ) );
}

zBOOL OPERATION
SSDelColRange( HWND             hWnd,
               SS_COORD         ssCol,
               SS_COORD         ssCol2 )
{
#if 0
   TRACE_DEBUG( "<SS_COL.CPP>", "SSDelColRange" );
   LPSPREADSHEET    lpSS;
   zzPCHAR *        CalcTable;
   zzPCHAR *        lpCellList;
   LPSS_UNDOCOL     lpList;
   LPSS_ROW         lpRow;
   LPSS_CALC        lpCalc;
   SS_COORD         Index;
   SS_COORD         k;
   SS_COORD         j;
   zBOOL            fVisible = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( ssCol == SS_FIRST )
      ssCol = lpSS->Col.nHeaderCnt;
   else
   if ( ssCol == SS_LAST )
      ssCol = lpSS->Col.AllocCnt - 1;
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
      ssCol2 = lpSS->Col.AllocCnt - 1;
   else
   if ( ssCol2 <= 0 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }
   else
   {
      ssCol2 += lpSS->Col.nHeaderCnt - 1;
      ssCol2 = mMin( ssCol2, lpSS->Col.AllocCnt - 1 );
   }

   if ( ssCol > ssCol2 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   SS_CellEditModeOff( hWnd, lpSS, 0 );

   if ( lpSS->fAllowUndo )
   {
      SS_ClearUndoBuffer( hWnd, lpSS );

      if ( DrAllocTaskMemory( (zCOREMEM) &lpSS->UndoBuffer.hData,
                              (zLONG) zsizeof( SS_UNDOCOL ) *
                                (Col2 - ssCol + 1), 1090 ) == 0 )
      {
         lpList = (LPSS_UNDOCOL) DrLockTaskMemory( lpSS->UndoBuffer.hData );
      // lpSS->UndoBuffer.hData = lpList;

         for ( Index = 0, k = ssCol; k <= ssCol2; k++, Index++ )
         {
            lpList[ Index ].hCol = SS_GrabItemHandle( lpSS, &lpSS->Col.Items, k,
                                                      SS_GetColsMax( lpSS ) );

            if ( DrAllocTaskMemory( (zCOREMEM) &lpList[Index].hCellList,
                                    (zLONG) sizeof( zPCHAR ) *
                                            lpSS->Row.AllocCnt, 1091 ) == 0 )
            {
               lpCellList = (zPCHAR) DrLockTaskMemory( lpList[ Index ].hCellList );
            // lpList[ Index ].hCellList ) = lpCellList;
            }

            for ( j = 0; j < lpSS->Row.AllocCnt; j++ )
            {
               if ( (lpRow = SS_LockRowItem( lpSS, j )) != 0 )
               {
                  lpCellList[ j ] = SS_GrabItemHandle( lpSS, &lpRow->Cells, k,
                                                       SS_GetColsMax( lpSS ) );
                  SS_UnlockRowItem( lpSS, j );
               }
            }

//          DrUnlockTaskMemory( lpList[ Index ].hCellList );
         }

//       DrUnlockTaskMemory( lpList );
      }

      lpSS->UndoBuffer.bUndoType = SS_UNDOTYPE_DELCOL;
      lpSS->UndoBuffer.ItemAllocCnt = lpSS->Row.AllocCnt;
      lpSS->UndoBuffer.ssCol = ssCol;
      lpSS->UndoBuffer.ssCol2 = ssCol2;
   }
   else
   {
      for ( k = ssCol; k <= ssCol2; k++ )
         SS_DeAllocCol( hWnd, k, FALSE );
   }

   SS_MoveAllocItems( lpSS, &lpSS->Col.Items, ssCol2 + 1,
                     lpSS->Col.AllocCnt - 1, ssCol, SS_GetColsMax( lpSS ) );

   for ( k = 0; k < lpSS->Row.AllocCnt; k++ )
   {
      if ( ( lpRow = SS_LockRowItem( lpSS, k ) ) != 0 )
      {
         if ( !SS_MoveAllocItems( lpSS, &lpRow->Cells, ssCol2 + 1,
                                  lpSS->Col.AllocCnt - 1, ssCol,
                                  SS_GetColsMax( lpSS ) ) )
         {
            SS_UnlockRowItem( lpSS, k );
//          DrUnlockTaskMemory( lpSS );
            return( FALSE );
         }

         SS_UnlockRowItem( lpSS, k );
      }
   }

   lpSS->Col.LastRefCoord.Coord.ssCol = -1;
   lpSS->LastCell.Coord.ssCol = -1;
   lpSS->LastCell.Coord.ssRow = -1;

   if ( lpSS->hCalcTable )
   {
//    CalcTable = (zPCHAR) DrLockTaskMemory( lpSS->hCalcTable );
      CalcTable = (zPCHAR) lpSS->hCalcTable;
      for ( k = 0; k < lpSS->CalcTableCnt; k++ )
      {
//       lpCalc = (LPSS_CALC) DrLockTaskMemory( CalcTable[ k ] );
         lpCalc = (LPSS_CALC) CalcTable[ k ];

         if ( lpCalc->CellCoord.ssCol > ssCol2 )
            lpCalc->CellCoord.ssCol -= ssCol2 - ssCol + 1;

//       DrUnlockTaskMemory( CalcTable[ k ] );
      }

//    DrUnlockTaskMemory( CalcTable );
   }

   for ( k = ssCol, fVisible = FALSE; k <= ssCol2; k++ )
   {
      if ( SS_IsColVisible( hWnd, lpSS, k, SS_VISIBLE_PARTIAL ) )
         fVisible = TRUE;
   }

   if ( fVisible )
      SS_InvalidateColRange( hWnd, lpSS, ssCol, -1 );
   else
   if ( ssCol < lpSS->Col.UL )
      SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_COLS );

   if ( ssCol < lpSS->Col.DataCnt - 1 && ssCol2 < lpSS->Col.DataCnt - 1 )
      SS_SetDataColCnt( lpSS, mMax( lpSS->Col.DataCnt -
                        mMin( ssCol2 - ssCol + 1, lpSS->Col.DataCnt - ssCol ), 1 ) );

   if ( ssCol < lpSS->Col.AllocCnt )
      lpSS->Col.AllocCnt = mMax( lpSS->Col.AllocCnt - mMin( ssCol2 - ssCol + 1,
                                 lpSS->Col.AllocCnt - ssCol ), 1 );

   SS_AdjustDataColCnt( lpSS, ssCol, ssCol2 );
   SS_AdjustDataRowCnt( lpSS, -1, -1 );
   SSBuildDependencies( hWnd );
   SS_HiliteCell( hWnd, TRUE );

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
#else
   MESSAGE_DEBUG( "<SS_COL.CPP>", "SSDelColRange" );
   return( TRUE );
#endif
}

zBOOL OPERATION
SSDelRow( HWND      hWnd,
          SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSDelRow" );
   return( SSDelRowRange( hWnd, ssRow, ssRow ) );
}

zBOOL OPERATION
SSDelRowRange( HWND      hWnd,
               SS_COORD  ssRow,
               SS_COORD  ssRow2 )
{
#if 0
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSDelRowRange" );
   LPSPREADSHEET    lpSS;
   zzPCHAR *        CalcTable;
   zzPCHAR *        lpList;
   LPSS_CALC        lpCalc;
   zBOOL            fRet = TRUE;
   SS_COORD         ss;
   zBOOL            fVisible = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( ssRow == SS_FIRST )
      ssRow = lpSS->Row.nHeaderCnt;
   else
   if ( ssRow == SS_LAST )
      ssRow = lpSS->Row.AllocCnt - 1;
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
      return( FALSE );
   }

   if ( ssRow2 == SS_FIRST )
      ssRow2 = lpSS->Row.nHeaderCnt;
   else
   if ( ssRow2 == SS_LAST )
      ssRow2 = lpSS->Row.AllocCnt - 1;
   else
   if ( ssRow2 <= 0 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }
   else
   {
      ssRow2 += lpSS->Row.nHeaderCnt - 1;
      ssRow2 = mMin( ssRow2, lpSS->Row.AllocCnt - 1 );
   }

   if ( ssRow > ssRow2 )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   SS_CellEditModeOff( hWnd, lpSS, 0 );

   if ( lpSS->fAllowUndo )
   {
      SS_ClearUndoBuffer( hWnd, lpSS );

      if ( DrAllocTaskMemory( (zCOREMEM) &lpSS->UndoBuffer.hData,
                              (zLONG) sizeof( zPCHAR ) *
                                      (Row2 - ssRow + 1), 1092 ) == 0 )
      {
         lpList = (zPCHAR) DrLockTaskMemory( lpSS->UndoBuffer.hData );
      // lpSS->UndoBuffer.hData = lpList;

         for ( ss = ssRow; ss <= ssRow2; ss++ )
            lpList[ ss - ssRow ] = SS_GrabItemHandle( lpSS,
                                                      &lpSS->Row.Items, ss,
                                                      SS_GetRowsMax( lpSS ) );

//       DrUnlockTaskMemory( lpList );
      }

      lpSS->UndoBuffer.bUndoType = SS_UNDOTYPE_DELROW;
      lpSS->UndoBuffer.ItemAllocCnt = lpSS->Col.AllocCnt;
      lpSS->UndoBuffer.ssRow = ssRow;
      lpSS->UndoBuffer.ssRow2 = ssRow2;
   }
   else
   {
      for ( ss = ssRow; ss <= ssRow2; ss++ )
         SS_DeAllocRow( hWnd, ss, FALSE );
   }

   SS_MoveAllocItems( lpSS, &lpSS->Row.Items, ssRow2 + 1,
                      lpSS->Row.AllocCnt - 1, ssRow, SS_GetRowsMax( lpSS ) );

   lpSS->Row.LastRefCoord.Coord.ssRow = -1;
   lpSS->LastCell.Coord.ssCol = -1;
   lpSS->LastCell.Coord.ssRow = -1;

   if ( lpSS->hCalcTable )
   {
//    CalcTable = (zPCHAR) DrLockTaskMemory( lpSS->hCalcTable );
      CalcTable = (zPCHAR) lpSS->hCalcTable;
      for ( ss = 0; ss < lpSS->CalcTableCnt; ss++ )
      {
//       lpCalc = (LPSS_CALC) DrLockTaskMemory( CalcTable[ ss ] );
         lpCalc = (LPSS_CALC) CalcTable[ ss ];

         if ( lpCalc->CellCoord.ssRow > ssRow )
            lpCalc->CellCoord.ssRow -= ssRow2 - ssRow + 1;

//       DrUnlockTaskMemory( lpCalc );
      }

//    DrUnlockTaskMemory( CalcTable );
   }

   if ( lpSS->fVirtualMode )
   {
      if ( ssRow <= lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 &&
           ssRow2 >= lpSS->Virtual.VTop )
      {
         lpSS->Virtual.VPhysSize -=
            mMin( ssRow2, lpSS->Virtual.VTop + lpSS->Virtual.VPhysSize - 1 ) -
            mMax( ssRow, lpSS->Virtual.VTop ) + 1;
      }
   }

   for ( ss = ssRow, fVisible = FALSE; ss <= ssRow2; ss++ )
      if ( SS_IsRowVisible( hWnd, lpSS, ss, SS_VISIBLE_PARTIAL ) )
         fVisible = TRUE;

   if ( ssRow < lpSS->Row.DataCnt - 1 && ssRow2 < lpSS->Row.DataCnt - 1 )
      SS_SetDataRowCnt( lpSS, mMax( lpSS->Row.DataCnt -
                        mMin( ssRow2 - ssRow + 1, lpSS->Row.DataCnt - ssRow ), 1 ) );

   if ( ssRow < lpSS->Row.AllocCnt )
      lpSS->Row.AllocCnt = mMax( lpSS->Row.AllocCnt - mMin( ssRow2 - ssRow + 1,
                                 lpSS->Row.AllocCnt - ssRow ), 1 );

   SS_AdjustDataColCnt( lpSS, -1, -1 );
   SS_AdjustDataRowCnt( lpSS, ssRow, ssRow2 );

   if ( fVisible )
      SS_InvalidateRowRange( hWnd, lpSS, ssRow, -1 );
   else
   if ( ssRow < lpSS->Row.UL )
      SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ROWS );

   SSBuildDependencies( hWnd );
   SS_HiliteCell( hWnd, TRUE );

// DrUnlockTaskMemory( lpSS );
   return( fRet );
#else
   MESSAGE_DEBUG( "<SS_ROW.CPP>", "SSDelRowRange" );
   return( FALSE );
#endif
}

zBOOL OPERATION
SSDeSelectBlock( HWND hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSDeSelectBlock" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( (GetFocus( ) == hWnd || lpSS->fRetainSelBlock) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( SS_ResetBlock( hWnd, TRUE ) );
   }
   else
   {
//    DrUnlockTaskMemory( lpSS );
      return( SS_ResetBlock( hWnd, FALSE ) );
   }
}

void OPERATION
SSFloatFormat( HWND          hWnd,
               LPSS_CELLTYPE lpCellType,
               zDECIMAL      dxVal,
               zPCHAR        lpszText,
               zBOOL         fValue )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSFloatFormat" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_FloatFormat( lpSS, lpCellType, dxVal, lpszText, fValue );
// DrUnlockTaskMemory( lpSS );
}

void OPERATION
SSGetAutoSizeVisible( HWND       hWnd,
                      LPSS_COORD lpVisibleCols,
                      LPSS_COORD lpVisibleRows )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetAutoSizeVisible" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   *lpVisibleCols = lpSS->Col.AutoSizeVisibleCnt;
   *lpVisibleRows = lpSS->Row.AutoSizeVisibleCnt;

// DrUnlockTaskMemory( lpSS );
}

zBOOL
SS_GetBorder( HWND          hWnd,
              LPSPREADSHEET lpSS,
              SS_COORD      ssCol,
              SS_COORD      ssRow,
              WORD          *lpwBorderType,
              WORD          *lpwBorderStyle,
              LPCOLORREF    lpColor )
{
   TRACE_DEBUG( "<SS_BORD.CPP>", "SS_GetBorder" );
   LPSS_ROW      lpRow;
   LPSS_COL      lpCol;
   zBOOL         fRet;

   lpCol = SS_LockColItem( lpSS, ssCol );
   lpRow = SS_LockRowItem( lpSS, ssRow );

   fRet = SSx_GetBorder( hWnd, lpSS, lpCol, lpRow, 0, ssCol, ssRow,
                         lpwBorderType, lpwBorderStyle, lpColor );

   SS_UnlockColItem( lpSS, ssCol );
   SS_UnlockRowItem( lpSS, ssRow );

   return( fRet );
}

zBOOL OPERATION
SSGetBorder( HWND        hWnd,
             SS_COORD    ssCol,
             SS_COORD    ssRow,
             WORD        *lpwBorderType,
             WORD        *lpwBorderStyle,
             LPCOLORREF  lpColor )
{
   TRACE_DEBUG( "<SS_BORD.CPP>", "SSGetBorder" );
   LPSPREADSHEET lpSS;
   zBOOL         fRet;

   if ( ssCol != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &ssCol, 0 );

   if ( ssRow != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &ssRow );

   lpSS = SS_Lock( hWnd );

   fRet = SS_GetBorder( hWnd, lpSS, ssCol, ssRow, lpwBorderType,
                        lpwBorderStyle, lpColor );

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

WORD  OPERATION
SSGetButtonDrawMode( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetButtonDrawMode" );
   LPSPREADSHEET lpSS;
   WORD          wModeOld;

   lpSS = SS_Lock( hWnd );
   wModeOld = lpSS->wButtonDrawMode;
// DrUnlockTaskMemory( lpSS );

   return( wModeOld );
}

zBOOL OPERATION
SSGetButtonText( HWND      hWnd,
                 SS_COORD  ssCol,
                 SS_COORD  ssRow,
                 zPCHAR    lpText )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSGetButtonText" );
   SS_CELLTYPE CellType;
   zPCHAR      lpTextTemp;
   zBOOL       bRC = FALSE;

   *lpText = 0;

   if ( SSGetCellType( hWnd, ssCol, ssRow, &CellType ) )
   {
      if ( CellType.chType == SS_TYPE_BUTTON )
      {
         if ( CellType.Spec.Button.hText )
         {
//          lpTextTemp = (zPCHAR) DrLockTaskMemory( CellType.Spec.Button.hText );
            lpTextTemp = (zPCHAR) CellType.Spec.Button.hText;
            strcpy_s( lpText, ?, lpTextTemp );
//          DrUnlockTaskMemory( lpTextTemp );
            bRC = TRUE;
         }
      }
   }

   return( bRC );
}

WNDPROC OPERATION
SSGetCallBack( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetCallBack" );
   LPSPREADSHEET lpSS;
   WNDPROC       lpfnCallBackOld;

   lpSS = SS_Lock( hWnd );
   lpfnCallBackOld = lpSS->lpfnCallBack;
// DrUnlockTaskMemory( lpSS );

   return( lpfnCallBackOld );
}

zBOOL OPERATION
SSGetCellFromPixel( HWND        hWnd,
                    LPSS_COORD  lpCol,
                    LPSS_COORD  lpRow,
                    zLONG       lMouseX,
                    zLONG       lMouseY )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetCellFromPixel" );
   LPSPREADSHEET lpSS;
   zLONG         x;
   zLONG         y;
   zBOOL         bRC;

   lpSS = SS_Lock( hWnd );

   if ( (bRC = SS_GetCellFromPixel( hWnd, lpSS, lpCol, lpRow, &x, &y,
                                    lMouseX, lMouseY )) != 0 )
   {
      if ( lpCol )
         if ( *lpCol == -1 )
            *lpCol = SS_HEADER;
         else
            *lpCol = *lpCol - lpSS->Col.nHeaderCnt + 1;

      if ( lpRow )
      {
         if ( *lpRow == -1 )
            *lpRow = SS_HEADER;
         else
            *lpRow = *lpRow - lpSS->Row.nHeaderCnt + 1;
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSGetCellRect( HWND     hWnd,
               SS_COORD ssCol,
               SS_COORD ssRow,
               LPRECT   lpRect )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetCellRect" );
   LPSPREADSHEET lpSS;
   zLONG         x;
   zLONG         y;
   zLONG         cx;
   zLONG         cy;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );
   if ( SS_IsCellVisible( hWnd, lpSS, ssCol, ssRow, SS_VISIBLE_PARTIAL ) )
   {
      SS_GetCellCoord( lpSS, lpSS->Col.UL, lpSS->Row.UL, ssCol, ssRow,
                       &x, &y, &cx, &cy );

      lpRect->left = mMax( x, 0 );
      lpRect->top = mMax( y, 0 );
      lpRect->right = x + cx;
      lpRect->bottom = y + cy;

      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );

   return( bRC );
}

void OPERATION
SSGetCellSendingMsg( HWND       hWnd,
                     LPSS_COORD lpCol,
                     LPSS_COORD lpRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetCellSendingMsg" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   *lpCol = lpSS->CurCellSendingMsg.ssCol;
   *lpRow = lpSS->CurCellSendingMsg.ssRow;
// DrUnlockTaskMemory( lpSS );
}

zBOOL OPERATION
SSGetCellType( HWND          hWnd,
               SS_COORD      ssCol,
               SS_COORD      ssRow,
               LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetCellType" );
   LPSPREADSHEET lpSS;
   zBOOL         Ret = TRUE;

   lpSS = SS_Lock( hWnd );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   if ( !SS_RetrieveCellType( lpSS, CellType, 0, ssCol, ssRow ) )
      Ret = FALSE;

   lpSS->CurVisCell.ssCol = ssCol;
   lpSS->CurVisCell.ssRow = ssRow;

// DrUnlockTaskMemory( lpSS );
   return( Ret );
}

zBOOL OPERATION
SSGetColPageBreak( HWND     hWnd,
                   SS_COORD ssCol )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSGetColPageBreak" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &ssCol, 0 );

   if ( ssCol < SS_GetColsMax( lpSS ) &&
        (lpCol = SS_LockColItem( lpSS, ssCol )) != 0 )
   {
      bRC = lpCol->fPageBreak;
      SS_UnlockColItem( lpSS, ssCol );
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSGetColor( HWND        hWnd,
            SS_COORD    ssCol,
            SS_COORD    ssRow,
            LPCOLORREF  lpBackground,
            LPCOLORREF  lpForeground )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetColor" );
   LPSPREADSHEET   lpSS;
   SS_CLRTBLITEM BackClrTblItem;
   SS_CLRTBLITEM ForeClrTblItem;
   zBOOL           Ret = FALSE;

   lpSS = SS_Lock( hWnd );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   if ( (Ret = SS_GetClrTblItem( hWnd, lpSS, ssCol, ssRow, &BackClrTblItem,
                                   &ForeClrTblItem ) ) != 0 )
   {
      if ( lpBackground )
         *lpBackground = BackClrTblItem.Color;

      if ( lpForeground )
         *lpForeground = ForeClrTblItem.Color;
   }

// DrUnlockTaskMemory( lpSS );
   return( Ret );
}

#ifndef SS_NOUSERDATA
zBOOL OPERATION
SSGetColUserData( HWND      hWnd,
                  SS_COORD  ssCol,
                  zPLONG    lplUserData )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSGetColUserData" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &Col, 0 );

   *lplUserData = 0;

   if ( ssCol < SS_GetColsMax( lpSS ) &&
        (lpCol = SS_LockColItem( lpSS, ssCol )) != 0 )
   {
      *lplUserData = lpCol->lUserData;
      SS_UnlockColItem( lpSS, ssCol );

      if ( *lplUserData )
         bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}
#endif

/////////////////////////////////////////////////////////////////////////////

zBOOL OPERATION
SSAddCustomFunction( HWND     hWnd,
                     zPCHAR   FunctionName,
                     zSHORT   nParameterCnt,
                     FARPROC  lpfnFunctionProc )
{
   TRACE_DEBUG( "<SS_CALC.CPP>", "SSAddCustomFunction" );
   return( 1 );
}

zBOOL OPERATION
SSAddMultiSelBlocks( HWND          hWnd,
                     LPSS_SELBLOCK lpSelBlock )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSAddMultiSelBlocks" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = TRUE;

   lpSS = SS_Lock( hWnd );

#if 0
   if ( lpSelBlock )
   {
      fMultipleBlocksSelectedPrev = lpSS->fMultipleBlocksSelected;

      if ( !SS_MultiSelBlockAddItem( lpSS, &lpSelBlock->UL, &lpSelBlock->LR ) )
         bRC = FALSE;

      else
      {
         /*
         lpSS->BlockCellUL.ssCol = lpSelBlock->UL.ssCol;
         lpSS->BlockCellUL.ssRow = lpSelBlock->UL.ssRow;

         lpSS->BlockCellLR.ssCol = lpSelBlock->LR.ssCol;
         lpSS->BlockCellLR.ssRow = lpSelBlock->LR.ssRow;

         lpSS->Col.ssCurrAt = mMax(lpSS->BlockCellUL.ssCol, 1);
         lpSS->Row.ssCurrAt = mMax(lpSS->BlockCellUL.ssRow, 1);
         */

         if ( fMultipleBlocksSelectedPrev )
            SS_BeginMultipleSelection( hWnd, lpSS );

         lpSS->BlockCellUL.ssCol = lpSelBlock->UL.ssCol;
         lpSS->BlockCellUL.ssRow = lpSelBlock->UL.ssRow;

         lpSS->BlockCellLR.ssCol = lpSelBlock->UL.ssCol;
         lpSS->BlockCellLR.ssRow = lpSelBlock->UL.ssRow;

         lpSS->Col.ssCurrAt = mMax( lpSS->BlockCellUL.ssCol, 1 );
         lpSS->Row.ssCurrAt = mMax( lpSS->BlockCellUL.ssRow, 1 );

         lpSS->Col.BlockCellCurrentPos = lpSelBlock->UL.ssCol;
         lpSS->Row.BlockCellCurrentPos = lpSelBlock->UL.ssRow;

         SSx_SelectBlock( hWnd, lpSS, lpSS->Col.BlockCellCurrentPos,
                         lpSS->Row.BlockCellCurrentPos );
      }
   }
#endif

   if ( lpSelBlock )
   {
      if ( lpSS->fMultipleBlocksSelected )
         SS_BeginMultipleSelection( hWnd, lpSS );

      lpSS->BlockCellUL.ssCol = lpSelBlock->UL.ssCol;
      lpSS->BlockCellUL.ssRow = lpSelBlock->UL.ssRow;

      lpSS->BlockCellLR.ssCol = lpSelBlock->UL.ssCol;
      lpSS->BlockCellLR.ssRow = lpSelBlock->UL.ssRow;

      SS_HiliteCell( hWnd, FALSE );

      lpSS->Col.ssCurrAt = mMax( lpSS->BlockCellUL.ssCol, 1 );
      lpSS->Row.ssCurrAt = mMax( lpSS->BlockCellUL.ssRow, 1 );

      SS_HiliteCell( hWnd, TRUE );

      lpSS->Col.BlockCellCurrentPos = lpSelBlock->UL.ssCol;
      lpSS->Row.BlockCellCurrentPos = lpSelBlock->UL.ssRow;

      SSx_SelectBlock( hWnd, lpSS, lpSelBlock->LR.ssCol, lpSelBlock->LR.ssRow );

      lpSS->fMultipleBlocksSelected = TRUE;
      bRC = SS_MultiSelBlockAddItem( lpSS, &lpSelBlock->UL,
                                     &lpSelBlock->LR );
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSGetColWidthInPixels( HWND      hWnd,
                       SS_COORD  ssCol,
                       zPLONG    lpnWidthPixels )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSGetColWidthInPixels" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &ssCol, 0 );

   if ( ssCol < SS_GetColsMax( lpSS ) )
   {
      *lpnWidthPixels = SS_GetColWidth( lpSS, ssCol );
      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSGetCurrSelBlockPos( HWND        hWnd,
                      LPSS_COORD  lpCurrentCol,
                      LPSS_COORD  lpCurrentRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetCurrSelBlockPos" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( SS_IsBlockSelected( lpSS ) )
   {
      if ( lpCurrentCol )
         *lpCurrentCol = lpSS->Col.BlockCellCurrentPos - lpSS->Col.nHeaderCnt + 1;

      if ( lpCurrentRow )
         *lpCurrentRow = lpSS->Row.BlockCellCurrentPos - lpSS->Row.nHeaderCnt + 1;

      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

HCURSOR OPERATION
SSGetCursor( HWND  hWnd,
             WORD  wType )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetCursor" );
   LPSPREADSHEET lpSS;
   HCURSOR       hCursor = 0;

   if ( ( lpSS = SS_Lock( hWnd ) ) != 0 )
   {
      switch ( wType )
      {
         case SS_CURSORTYPE_DEFAULT:
            hCursor = lpSS->hCursorDefault;
            break;

         case SS_CURSORTYPE_COLRESIZE:
            hCursor = lpSS->hCursorColResize;
            break;

         case SS_CURSORTYPE_ROWRESIZE:
            hCursor = lpSS->hCursorRowResize;
            break;

         case SS_CURSORTYPE_BUTTON:
            hCursor = lpSS->hCursorButton;
            break;

         case SS_CURSORTYPE_GRAYAREA:
            hCursor = lpSS->hCursorGrayArea;
            break;

         case SS_CURSORTYPE_LOCKEDCELL:
            hCursor = lpSS->hCursorLockedCell;
            break;

         case SS_CURSORTYPE_COLHEADER:
            hCursor = lpSS->hCursorColHeader;
            break;

         case SS_CURSORTYPE_ROWHEADER:
            hCursor = lpSS->hCursorRowHeader;
            break;
      }

//    DrUnlockTaskMemory( lpSS );
   }

   return( hCursor );
}

zBOOL OPERATION
SSGetDataCnt( HWND       hWnd,
              LPSS_COORD lpColCnt,
              LPSS_COORD lpRowCnt )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetDataCnt" );
   LPSPREADSHEET lpSS;

   if ( ( lpSS = SS_Lock( hWnd ) ) != 0 )
   {
      if ( lpColCnt )
         *lpColCnt = mMax( 0, lpSS->Col.DataCnt - 1 );

      if ( lpRowCnt )
         *lpRowCnt = mMax( 0, lpSS->Row.DataCnt - 1 );

//    DrUnlockTaskMemory( lpSS );
      return( TRUE );
   }

   return( FALSE );
}

zBOOL OPERATION
SSGetDefFloatFormat( HWND          hWnd,
                     LPFLOATFORMAT lpFloatFormat )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetDefFloatFormat" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = TRUE;

   lpSS = SS_Lock( hWnd );
   zmemcpy( lpFloatFormat, &lpSS->DefaultFloatFormat, zsizeof( LPFLOATFORMAT ) );
// DrUnlockTaskMemory( lpSS );

   return( bRC );
}

WORD  OPERATION
SSGetEditEnterAction( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetEditEnterAction" );
   LPSPREADSHEET lpSS;
   WORD          wRet;

   lpSS = SS_Lock( hWnd );
   wRet = lpSS->wEnterAction;
// DrUnlockTaskMemory( lpSS );

   return( wRet );
}

zBOOL OPERATION
SSGetEditMode( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetEditMode" );
   LPSPREADSHEET lpSS;
   zBOOL         EditModeOn;

   lpSS = SS_Lock( hWnd );
   EditModeOn = lpSS->EditModeOn;
// DrUnlockTaskMemory( lpSS );
   return( EditModeOn );
}

zBOOL OPERATION
SSGetFirstValidCell( HWND       hWnd,
                     LPSS_COORD lpCol,
                     LPSS_COORD lpRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSGetFirstValidCell" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   SS_COORD      ssRowAtTemp = 1;
   SS_COORD      ssColAtTemp = 0;
   zBOOL         fFound = FALSE;

   lpSS = SS_Lock( hWnd );

   while ( fFound == 0 && ssRowAtTemp < SS_GetRowsMax( lpSS ) &&
           (lpSS->RestrictRows == 0 ||
           (lpSS->RestrictRows && ssRowAtTemp <= lpSS->Row.DataCnt)) )
   {
      if ( SS_GetRowHeight( lpSS, ssRowAtTemp ) > 0 )
      {
         while ( fFound == 0 && ssColAtTemp < SS_GetColsMax( lpSS ) - 1 &&
                 (lpSS->RestrictCols == 0 ||
                  (lpSS->RestrictCols && ssColAtTemp <= lpSS->Col.DataCnt)) )
         {
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
               fFound = TRUE;
            }
         }
      }

      if ( !fFound )
      {
         ssRowAtTemp++;
         ssColAtTemp = lpSS->Col.nHeaderCnt - 1;
      }
   }

   if ( fFound )
   {
      *lpCol = ssColAtTemp;
      *lpRow = ssRowAtTemp;
   }

// DrUnlockTaskMemory( lpSS );
   return( fFound );
}

zBOOL OPERATION
SSGetFloat( HWND      hWnd,
            SS_COORD  ssCol,
            SS_COORD  ssRow,
            zDECIMAL  *pdxValue )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSGetFloat" );
   zPCHAR         hValue;
   zPCHAR         hData;
   LPSPREADSHEET  lpSS;
   SS_CELLTYPE    CellType;
   LPSS_COL       lpCol;
   LPSS_ROW       lpRow;
   ZSS_Data       SSData;
   zPCHAR         pchData;
   zPCHAR         pchValue;
   zBOOL          fDelete = FALSE;
   zBOOL          bRC = FALSE;

   *pdxValue = (ZDecimal) 0;

   lpSS = SS_Lock( hWnd );

   SS_RetrieveCellType( lpSS, &CellType, 0, ssCol, ssRow );

   if ( CellType.chType == SS_TYPE_FLOAT )
   {
      if ( lpSS->EditModeOn && lpSS->Col.ssCurrAt == ssCol &&
          lpSS->Row.ssCurrAt == ssRow )
      {
         if ( (hData = SS_GetEditModeOnData( hWnd, lpSS, TRUE )) != 0 )
         {
            fDelete = TRUE;
            SSData.m_DataType = SS_TYPE_EDIT;
            SSData.m_Data.hszData = hData;
         }
         else
            SSData.m_DataType = 0;
      }
      else
      {
         lpCol = SS_LockColItem( lpSS, ssCol );
         lpRow = SS_LockRowItem( lpSS, ssRow );

         SS_GetDataStruct( lpSS, lpCol, lpRow, 0, &SSData, ssCol, ssRow );
      }

      bRC = TRUE;

      if ( SSData.m_DataType == 0 )
         bRC = FALSE;
      else
      if ( SSData.m_DataType == SS_TYPE_INTEGER )
         *pdxValue = (ZDecimal) SSData.m_Data.lValue;
      else
      if ( SSData.m_DataType == SS_TYPE_FLOAT )
         *pdxValue = SSData.m_Data.dxValue;
      else
      {
//       pchData = (zPCHAR) DrLockTaskMemory( SSData.m_Data.hszData );
         pchData = (zPCHAR) SSData.m_Data.hszData;

         hValue = SS_UnFormatData( lpSS, &CellType, pchData );
//       pchValue = (zPCHAR) DrLockTaskMemory( hValue );
         pchValue = (zPCHAR) hValue;
         *pdxValue = ZDecimal( pchValue );
      // StringToFloat( pchValue, pdxValue );
         hValue = (zPCHAR) DrUnlockTaskMemory( pchValue );
         DrFreeTaskMemory( hValue );

//       DrUnlockTaskMemory( pchData );

         if ( fDelete )
         {
            SSData.m_Data.hszData = (zPCHAR)
                              DrUnlockTaskMemory( SSData.m_Data.hszData );
            DrFreeTaskMemory( SSData.m_Data.hszData );
            SSData.m_Data.hszData = 0;
         }
      }

      if ( lpCol )
         SS_UnlockColItem( lpSS, ssCol );

      if ( lpRow )
         SS_UnlockRowItem( lpSS, ssRow );
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

HFONT OPERATION
SSGetFont( HWND      hWnd,
           SS_COORD  ssCol,
           SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetFont" );
   LPSPREADSHEET lpSS;
   SS_FONT       SSFont;
   SS_FONTID     FontId;

   if ( mIs_hWnd( hWnd ) == FALSE || (lpSS = SS_Lock( hWnd )) == 0 )
      return( 0 );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );
   FontId = SS_GetFontId( hWnd, lpSS, ssCol, ssRow );
// DrUnlockTaskMemory( lpSS );

   if ( FontId )
   {
      if ( SS_GetFont( lpSS, &SSFont, FontId ) )
         return( SSFont.hFont );
   }

   return( 0 );
}

zSHORT OPERATION
SSGetFormula( HWND      hWnd,
              SS_COORD  ssCol,
              SS_COORD  ssRow,
              zPCHAR    lpFormula )
{
   TRACE_DEBUG( "<SS_FORMU.CPP>", "SSGetFormula" );
   return( TRUE );
}

zSHORT OPERATION
SSGetFormulaLth( HWND      hWnd,
                 SS_COORD  ssCol,
                 SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_FORMU.CPP>", "SSGetFormulaLth" );
   return( TRUE );
}

zBOOL OPERATION
SSGetFreeze( HWND       hWnd,
             LPSS_COORD lpColsFrozen,
             LPSS_COORD lpRowsFrozen )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetFreeze" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   *lpRowsFrozen = lpSS->Row.Frozen;
   *lpColsFrozen = lpSS->Col.Frozen;

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

void OPERATION
SSGetGrayAreaColor( HWND        hWnd,
                    LPCOLORREF  lpBackground,
                    LPCOLORREF  lpForeground )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetGrayAreaColor" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( lpBackground )
      *lpBackground = lpSS->Color.GrayAreaBackground;

   if ( lpForeground )
      *lpForeground = lpSS->Color.GrayAreaForeground;

// DrUnlockTaskMemory( lpSS );
}

COLORREF  OPERATION
SSGetGridColor( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetGridColor" );
   LPSPREADSHEET lpSS;
   COLORREF      GridColorOld;

   lpSS = SS_Lock( hWnd );
   GridColorOld = lpSS->GridColor;
// DrUnlockTaskMemory( lpSS );

   return( GridColorOld );
}

WORD  OPERATION
SSGetGridType( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetGridType" );
   LPSPREADSHEET lpSS;
   WORD          wGridTypeOld;

   lpSS = SS_Lock( hWnd );
   wGridTypeOld = lpSS->wGridType;
// DrUnlockTaskMemory( lpSS );

   return( wGridTypeOld );
}

zBOOL OPERATION
SSGetInteger( HWND      hWnd,
              SS_COORD  ssCol,
              SS_COORD  ssRow,
              zPLONG    lplValue )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSGetInteger" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   zCHAR         Buffer[30];
   zBOOL         bRC = FALSE;

   *lplValue = 0;

   lpSS = SS_Lock( hWnd );

   SS_RetrieveCellType( lpSS, &CellType, 0, ssCol, ssRow );

   if ( CellType.chType == SS_TYPE_INTEGER )
   {
      if ( SSGetData( hWnd, ssCol, ssRow, Buffer ) )
      {
         *lplValue = zatol( Buffer );

         if ( *Buffer )
            bRC = TRUE;
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSGetLastValidCell( HWND       hWnd,
                    LPSS_COORD lpCol,
                    LPSS_COORD lpRow )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSGetLastValidCell" );
   LPSPREADSHEET lpSS;
   SS_CELLTYPE   CellType;
   SS_COORD      ssColAtTemp;
   SS_COORD      ssRowAtTemp;
   zBOOL         fFound = FALSE;

   lpSS = SS_Lock( hWnd );

   ssColAtTemp = SS_GetColsMax( lpSS );
   ssRowAtTemp = SS_GetRowsMax( lpSS ) - 1;

   while ( !fFound && ssRowAtTemp >= lpSS->Row.nHeaderCnt )
   {
      if ( SS_GetRowHeight( lpSS, ssRowAtTemp ) > 0 )
      {
         while ( !fFound && ssColAtTemp > lpSS->Col.nHeaderCnt )
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
               fFound = TRUE;
            }
         }
      }

      if ( !fFound )
      {
         ssRowAtTemp--;
         ssColAtTemp = SS_GetColsMax( lpSS );
      }
   }

   if ( fFound )
   {
      *lpCol = ssColAtTemp;
      *lpRow = ssRowAtTemp;
   }

// DrUnlockTaskMemory( lpSS );
   return( fFound );
}

zBOOL OPERATION
SSGetLock( HWND      hWnd,
           SS_COORD  ssCol,
           SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetLock" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   LPSS_COL      lpCol;
   zBOOL         fLocked;

   lpSS = SS_Lock( hWnd );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   lpCol = SS_LockColItem( lpSS, ssCol );
   lpRow = SS_LockRowItem( lpSS, ssRow );

   fLocked = SSxx_GetLock( hWnd, lpSS, lpCol, lpRow, 0, ssCol, ssRow );

   SS_UnlockColItem( lpSS, ssCol );
   SS_UnlockRowItem( lpSS, ssRow );

// DrUnlockTaskMemory( lpSS );
   return( fLocked );
}

void OPERATION
SSGetLockColor( HWND        hWnd,
                LPCOLORREF  lpBackground,
                LPCOLORREF  lpForeground )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetLockColor" );
   LPSPREADSHEET   lpSS;
   SS_CLRTBLITEM ClrTblItem;

   lpSS = SS_Lock( hWnd );

   if ( lpBackground )
   {
      if ( lpSS->LockColor.BackgroundId )
      {
         SS_GetColorItem( lpSS, &ClrTblItem,
                          lpSS->LockColor.BackgroundId );
         *lpBackground = ClrTblItem.Color;
      }
      else
         *lpBackground = (COLORREF) RGBCOLOR_DEFAULT;
   }

   if ( lpForeground )
   {
      if ( lpSS->LockColor.ForegroundId )
      {
         SS_GetColorItem( lpSS, &ClrTblItem,
                          lpSS->LockColor.ForegroundId );
         *lpForeground = ClrTblItem.Color;
      }
      else
         *lpForeground = (COLORREF) RGBCOLOR_DEFAULT;
   }

// DrUnlockTaskMemory( lpSS );
}

SS_COORD OPERATION
SSGetMaxCols( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetMaxCols" );
   LPSPREADSHEET lpSS;
   SS_COORD      ssColsMax;

   lpSS = SS_Lock( hWnd );
   ssColsMax = lpSS->Col.Max;
// DrUnlockTaskMemory( lpSS );
   return( ssColsMax );
}

SS_COORD OPERATION
SSGetMaxRows( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetMaxRows" );
   LPSPREADSHEET lpSS;
   SS_COORD      ssRowsMax;

   lpSS = SS_Lock( hWnd );
   ssRowsMax = lpSS->Row.Max;
// DrUnlockTaskMemory( lpSS );
   return( ssRowsMax );
}

zBOOL OPERATION
SSGetMaxTextCellSize( HWND      hWnd,
                      SS_COORD  ssCol,
                      SS_COORD  ssRow,
                      zPLONG    lpnfWidth,
                      zPLONG    lpnfHeight )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSGetMaxTextCellSize" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   HDC           hDC;
   zLONG         lSize;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &ssCol, &ssRow );

   if ( lpnfWidth )
      *lpnfWidth = 0;

   if ( lpnfHeight )
      *lpnfHeight = 0;

   hDC = GetDC( hWnd );

   if ( ssRow < SS_GetRowsMax( lpSS ) && ssCol < SS_GetColsMax( lpSS ) )
   {
      lpRow = SS_LockRowItem( lpSS, ssRow );
      lpCol = SS_LockColItem( lpSS, ssCol );

      lSize = SS_CalcCellMetrics( hDC, hWnd, lpSS, lpCol, lpRow, 0, 0,
                                 0, ssCol, ssRow, TRUE, TRUE, 0, 0 );

      SS_UnlockColItem( lpSS, ssCol );
      SS_UnlockRowItem( lpSS, ssRow );

      if ( lpnfWidth && LOWORD( lSize ) )
         SS_CalcPixelsToColWidth( lpSS, ssCol, LOWORD( lSize ), lpnfWidth );

      if ( lpnfHeight && HIWORD( lSize ) )
         SS_CalcPixelsToRowHeight( lpSS, ssRow, HIWORD( lSize ), lpnfHeight );

      bRC = TRUE;
   }

   ReleaseDC( hWnd, hDC );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSGetMaxTextColWidth( HWND      hWnd,
                      SS_COORD  ssCol,
                      zPLONG    lpnfColWidth )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSGetMaxTextColWidth" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   LPSS_ROW      lpRow;
   HDC           hDC;
   zSHORT        nWidth = 0;
   SS_COORD      Top;
   SS_COORD      ss;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &ssCol, 0 );

   *lpnfColWidth = 0;

   hDC = GetDC( hWnd );

   if ( ssCol < SS_GetColsMax( lpSS ) )
   {
      lpCol = SS_LockColItem( lpSS, ssCol );

      if ( lpSS->fVirtualMode )
         Top = lpSS->Virtual.VTop;
      else
         Top = lpSS->Row.nHeaderCnt;

      for ( ss = 0; ss < lpSS->Row.DataCnt; )
      {
         lpRow = SS_LockRowItem( lpSS, ss );

         nWidth = mMax( nWidth, (zSHORT) LOWORD( SS_CalcCellMetrics( hDC, hWnd,
                       lpSS, lpCol, lpRow, 0, 0, 0, ssCol, ss, TRUE,
                       FALSE, 0, 0 ) ) );

         if ( lpRow )
            SS_UnlockRowItem( lpSS, ss );

         if ( ss == 0 )
            ss = Top;
         else
            ss++;
      }

      if ( nWidth )
         SS_CalcPixelsToColWidth( lpSS, ssCol, nWidth, lpnfColWidth );

      if ( lpCol )
         SS_UnlockColItem( lpSS, ssCol );

      bRC = TRUE;
   }

   ReleaseDC( hWnd, hDC );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSGetMaxTextRowHeight( HWND      hWnd,
                       SS_COORD  ssRow,
                       zPLONG    lpnfRowHeight )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSGetMaxTextRowHeight" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   HDC           hDC;
   zLONG         lHeight = 0;
   SS_COORD      ss;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, 0, &ssRow );

   *lpnfRowHeight = 0;

   hDC = GetDC( hWnd );

   if ( ssRow < SS_GetRowsMax( lpSS ) )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, ssRow )) != 0 )
      {
         for ( ss = 0; ss < lpSS->Col.DataCnt; ss++ )
            lHeight = mMax( lHeight, HIWORD( SS_CalcCellMetrics( hDC, hWnd,
                            lpSS, 0, lpRow, 0, 0, 0, ss, ssRow, FALSE,
                            TRUE, 0, 0 ) ) );

         if ( lHeight )
            SS_CalcPixelsToRowHeight( lpSS, ssRow, lHeight, lpnfRowHeight );

         SS_UnlockRowItem( lpSS, ssRow );
      }

      bRC = TRUE;
   }

   ReleaseDC( hWnd, hDC );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zPCHAR  OPERATION
SSGetMultiSelBlocks( HWND    hWnd,
                     zPLONG lpnSelBlockCnt )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSGetMultiSelBlocks" );
   LPSPREADSHEET lpSS;
   LPSS_SELBLOCK lpItemList;
   SS_SELBLOCK   SelBlockTemp;
   SS_SELBLOCK   SelBlockSplit;
   zPCHAR        hList;
   zPCHAR        hListNew = 0;
   zLONG         lTemp;
   zLONG         lMaxCells;
   zLONG         lItemCntOld;
   zLONG         lItemAllocCnt;
   zLONG         lItemCnt;
   zLONG         lItemAt;
   zLONG         lMaxItemNbr;
   zLONG         lItemCntNew = 0;
   zLONG         lItemAllocCntNew = 0;
   zUSHORT       k;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->MultiSelBlock.lItemCnt > 0 )
   {
      if ( (hList = SS_ListDup( lpSS->MultiSelBlock.hItemList,
                                lpSS->MultiSelBlock.lItemCnt,
                                zsizeof( SS_SELBLOCK ) )) != 0 )
      {
         lItemCnt = lpSS->MultiSelBlock.lItemCnt;
         lItemAllocCnt = lpSS->MultiSelBlock.lItemCnt;

         while ( lItemCnt > 0 )
         {
            lMaxCells = 0;
            lMaxItemNbr = -1;

//          lpItemList = (LPSS_SELBLOCK) DrLockTaskMemory( hList );
            lpItemList = (LPSS_SELBLOCK) hList;

            for ( k = 0; k < lItemCnt; k++ )
            {
               if ( lpItemList[ k ].LR.ssCol == -1 )
                  lTemp = lpSS->Col.Max - lpItemList[ k ].UL.ssCol + 1;
               else
                  lTemp = lpItemList[ k ].LR.ssCol - lpItemList[ k ].UL.ssCol + 1;

               if ( lpItemList[k].LR.ssRow == -1 )
                  lTemp = lTemp * (lpSS->Row.Max - lpItemList[ k ].UL.ssRow + 1);
               else
                  lTemp = lTemp * (lpItemList[ k ].LR.ssRow -
                          lpItemList[ k ].UL.ssRow + 1);

               if ( lTemp > lMaxCells )
               {
                  lMaxItemNbr = k;
                  lMaxCells = lTemp;
               }
            }

//          DrUnlockTaskMemory( lpItemList );

            zmemcpy( &SelBlockTemp, &lpItemList[ lMaxItemNbr ],
                     zsizeof( SS_SELBLOCK ) );

            hListNew = SS_ListAddItem( hListNew, &lItemCntNew, &lItemAllocCntNew,
                                       &SelBlockTemp, zsizeof( SS_SELBLOCK ), 50 );

            SS_ListDelItem( hList, lMaxItemNbr, &lItemCnt, zsizeof( SS_SELBLOCK ) );

            // Split rest of list.
            for ( lItemAt = 0, lItemCntOld = lItemCnt; lItemAt < lItemCntOld; )
            {
//             lpItemList = (LPSS_SELBLOCK) DrLockTaskMemory( hList );
               lpItemList = (LPSS_SELBLOCK) hList;

               if ( SS_IntersectSelBlock( &SelBlockTemp, &lpItemList[ lItemAt ] ) )
               {
                  zmemcpy( &SelBlockSplit, &lpItemList[ lItemAt ],
                           zsizeof( SS_SELBLOCK ) );

                  SS_ListDelItem( hList, lItemAt, &lItemCnt, zsizeof( SS_SELBLOCK ) );

                  hList = SS_SelBlockSplitItem( hList, &lItemCnt, &lItemAllocCnt,
                                                &SelBlockTemp, &SelBlockSplit );

                  lItemCntOld--;
               }
               else
                  lItemAt++;

//             DrUnlockTaskMemory( lpItemList );
            }
         }

         hList = (zPCHAR) DrUnlockTaskMemory( hList );
         DrFreeTaskMemory( hList );
      }
   }
   else
   if ( SS_IsBlockSelected( lpSS ) )
   {
      zmemcpy( &SelBlockTemp.UL, &lpSS->BlockCellUL, zsizeof( SS_CELLCOORD ) );
      zmemcpy( &SelBlockTemp.LR, &lpSS->BlockCellLR, zsizeof( SS_CELLCOORD ) );

      hListNew = SS_ListAddItem( hListNew, &lItemCntNew, &lItemAllocCntNew,
                                 &SelBlockTemp, zsizeof( SS_SELBLOCK ), 1 );
   }

// DrUnlockTaskMemory( lpSS );
   *lpnSelBlockCnt = lItemCntNew;

   return( hListNew );
}

WORD  OPERATION
SSGetOperationMode( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetOperationMode" );
   LPSPREADSHEET lpSS;
   WORD          wOpMode;

   lpSS = SS_Lock( hWnd );
   wOpMode = lpSS->wOpMode;
// DrUnlockTaskMemory( lpSS );

   return( wOpMode );
}

HWND OPERATION
SSGetOwner( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetOwner" );
   LPSPREADSHEET lpSS;
   HWND          hWndOwner;

   lpSS = SS_Lock( hWnd );
   hWndOwner = lpSS->hWndOwner;
   TraceLineX( "SSGetOwner hWndOwner: ", (zLONG) lpSS->hWndOwner );
// DrUnlockTaskMemory( lpSS );

   return( hWndOwner );
}

zBOOL OPERATION
SSGetRowPageBreak( HWND     hWnd,
                   SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSGetRowPageBreak" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, 0, &ssRow );

   if ( ssRow < SS_GetRowsMax( lpSS ) &&
       ( lpRow = SS_LockRowItem( lpSS, ssRow ) ) != 0 )
   {
      bRC = lpRow->fPageBreak;
      SS_UnlockRowItem( lpSS, ssRow );
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

#ifndef SS_NOUSERDATA
zBOOL OPERATION
SSGetRowUserData( HWND      hWnd,
                  SS_COORD  ssRow,
                  zPLONG    lplUserData )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSGetRowUserData" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   zBOOL         bRC = FALSE;

   *lplUserData = 0;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, 0, &Row );

   if ( ssRow < SS_GetRowsMax( lpSS ) &&
        (lpRow = SS_LockRowItem( lpSS, ssRow ))!= 0 )
   {
      *lplUserData = lpRow->lUserData;
      SS_UnlockRowItem( lpSS, ssRow );

      if ( *lplUserData )
         bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}
#endif

zBOOL OPERATION
SSGetSel( HWND    hWnd,
          zPLONG plSelStart,
          zPLONG plSelEnd )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSGetSel" );
   LPSPREADSHEET   lpSS;
   SS_CELLTYPE     CellType;
   zLONG           lSel;
   zBOOL           bRC = FALSE;

   *plSelStart = 0;
   *plSelEnd = 0;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->EditModeOn )
   {
      if ( SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt,
                                lpSS->Row.ssCurrAt ) && CellType.nControlID )
      {
         lSel = SendMessage( SS_GetControlhWnd( lpSS, CellType.nControlID ),
                             EM_GETSEL, 0, 0 );
         *plSelStart = LOWORD( lSel );
         *plSelEnd = HIWORD( lSel );

         bRC = TRUE;
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

WORD  OPERATION
SSGetSelBlockOptions( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSGetSelBlockOptions" );
   LPSPREADSHEET lpSS;
   WORD          wSelBlockOptionOld;

   lpSS = SS_Lock( hWnd );
   wSelBlockOptionOld = lpSS->wSelBlockOption;
// DrUnlockTaskMemory( lpSS );

   return( wSelBlockOptionOld );
}

zPCHAR  OPERATION
SSGetSelText( HWND  hWnd )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSGetSelText" );
   LPSPREADSHEET   lpSS;
   SS_CELLTYPE     CellType;
   zLONG           lSel;
   zPCHAR          hData = 0;
   zPCHAR          Data;
   zSHORT          nTextLth;
   zUSHORT         nStart;
   zUSHORT         nEnd;
   zUSHORT         k;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->EditModeOn )
   {
      if ( SS_RetrieveCellType( lpSS, &CellType, 0,
                                lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt ) &&
                                CellType.nControlID )
      {
         lSel = SendMessage( SS_GetControlhWnd( lpSS, CellType.nControlID ),
                             EM_GETSEL, 0, 0 );

         nStart = LOWORD( lSel );
         nEnd = HIWORD( lSel );

         if ( nStart < nEnd )
         {
            nTextLth = (zSHORT) SendMessage(
                              SS_GetControlhWnd( lpSS, CellType.nControlID ),
                              WM_GETTEXTLENGTH, 0, 0L );

            if ( nTextLth )
            {
               if ( DrAllocTaskMemory( &hData, nTextLth + 1, 1093 ) == 0 )
               {
               // Data = (zPCHAR) DrLockTaskMemory( hData );
                  Data = hData;
                  SendMessage( SS_GetControlhWnd( lpSS, CellType.nControlID ),
                               WM_GETTEXT, nTextLth + 1, (zLONG) Data );

                  for ( k = 0; k < mMin( zstrlen( Data ), nEnd ) - nStart; k++ )
                     Data[ k ] = Data[ nStart + k ];

                  Data[ k ] = 0;
//                DrUnlockTaskMemory( Data );
               }
            }
         }
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( hData );
}

void OPERATION
SSGetShadowColor( HWND        hWnd,
                  LPCOLORREF  ShadowColor,
                  LPCOLORREF  ShadowText,
                  LPCOLORREF  ShadowDark,
                  LPCOLORREF  ShadowLight )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSGetShadowColor" );
   LPSPREADSHEET   lpSS;
   SS_CLRTBLITEM ClrTblItem;

   lpSS = SS_Lock( hWnd );

   if ( ShadowColor )
   {
      SS_GetColorItem( lpSS, &ClrTblItem,
                       lpSS->Color.ShadowColorId );
      *ShadowColor = ClrTblItem.Color;
   }

   if ( ShadowText )
   {
      SS_GetColorItem( lpSS, &ClrTblItem,
                       lpSS->Color.ShadowTextId );
      *ShadowText = ClrTblItem.Color;
   }

   if ( ShadowDark )
   {
      SS_GetColorItem( lpSS, &ClrTblItem,
                       lpSS->Color.ShadowDarkId );
      *ShadowDark = ClrTblItem.Color;
   }

   if ( ShadowLight )
   {
      SS_GetColorItem( lpSS, &ClrTblItem,
                       lpSS->Color.ShadowLightId );
      *ShadowLight = ClrTblItem.Color;
   }

// DrUnlockTaskMemory( lpSS );
}

WORD  OPERATION
SSGetUnitType( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetUnitType" );
   LPSPREADSHEET lpSS;
   WORD          wRet;

   lpSS = SS_Lock( hWnd );
   wRet = lpSS->nUnitType;
// DrUnlockTaskMemory( lpSS );

   return( wRet );
}

#ifndef SS_NOUSERDATA
zLONG OPERATION
SSGetUserData( HWND  hWnd )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSGetUserData" );
   LPSPREADSHEET lpSS;
   zLONG         lRet;

   lpSS = SS_Lock( hWnd );
   lRet = lpSS->lUserData;
// DrUnlockTaskMemory( lpSS );

   return( lRet );
}
#endif

WORD  OPERATION
SSGetUserResize( HWND hWnd )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetUserResize" );
   LPSPREADSHEET lpSS;
   WORD          wUserResizeOld;

   lpSS = SS_Lock( hWnd );
   wUserResizeOld = lpSS->wUserResize;
// DrUnlockTaskMemory( lpSS );

   return( wUserResizeOld );
}

zSHORT OPERATION
SSGetValue( HWND      hWnd,
            SS_COORD  ssCol,
            SS_COORD  ssRow,
            zPCHAR    lpBuffer )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSGetValue" );
   LPSPREADSHEET  lpSS;
   zPCHAR         hData;
   zPCHAR         pchData;
   zBOOL          bRC = FALSE;

   if ( lpBuffer == 0 )
      return( 0 );

   lpBuffer[ 0 ] = 0;

   lpSS = SS_Lock( hWnd );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   if ( (hData = SS_GetValue( hWnd, lpSS, ssCol, ssRow )) != 0 )
   {
//    pchData = (zPCHAR) DrLockTaskMemory( hData );
      pchData = (zPCHAR) hData;
      strcpy_s( lpBuffer, ?, pchData );
      hData = (zPCHAR) DrUnlockTaskMemory( pchData );
      DrFreeTaskMemory( hData );
      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( zstrlen( lpBuffer ) );
}

zSHORT OPERATION
SSGetValueLth( HWND      hWnd,
               SS_COORD  ssCol,
               SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSGetValueLth" );
   LPSPREADSHEET  lpSS;
   zPCHAR         hData;
   zPCHAR         pchData;
   zSHORT         nLth = 0;

   lpSS = SS_Lock( hWnd );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   if ( (hData = SS_GetValue( hWnd, lpSS, ssCol, ssRow )) != 0 )
   {
//    pchData = (zPCHAR) DrLockTaskMemory( hData );
      pchData = (zPCHAR) hData;
      nLth = zstrlen( pchData );
      hData = (zPCHAR) DrUnlockTaskMemory( pchData );
      DrFreeTaskMemory( hData );
   }

// DrUnlockTaskMemory( lpSS );
   return( nLth );
}

zBOOL OPERATION
SSGetVisible( HWND      hWnd,
              SS_COORD  ssCol,
              SS_COORD  ssRow,
              zSHORT    nVisible )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSGetVisible" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   SSx_AdjustCellCoords( lpSS, &ssCol, &ssRow );

   if ( ssCol != SS_ALLCOLS && ssRow != SS_ALLROWS )
   {
      bRC = SS_IsCellVisible( hWnd, lpSS, ssCol, ssRow, nVisible );

      if ( bRC &&
           (!SS_GetColWidth( lpSS, ssCol ) || !SS_GetRowHeight( lpSS, ssRow )) )
      {
         bRC = FALSE;
      }
   }
   else
   if ( ssCol != SS_ALLCOLS && ssRow == SS_ALLROWS )
   {
      bRC = SS_IsColVisible( hWnd, lpSS, ssCol, nVisible );

      if ( bRC && !SS_GetColWidth( lpSS, ssCol ) )
         bRC = FALSE;
   }
   else
   if ( ssCol == SS_ALLCOLS && ssRow != SS_ALLROWS )
   {
      bRC = SS_IsRowVisible( hWnd, lpSS, ssRow, nVisible );
      if ( bRC && !SS_GetRowHeight( lpSS, ssRow ) )
         bRC = FALSE;
   }
   else
      bRC = SSGetDocVisible( hWnd );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSInsCol( HWND      hWnd,
          SS_COORD  ssCol )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSInsCol" );
   return( SSInsColRange( hWnd, ssCol, ssCol ) );
}

zBOOL OPERATION
SSInsRow( HWND      hWnd,
          SS_COORD  ssRow )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSInsRow" );
   return( SSInsRowRange( hWnd, ssRow, ssRow ) );
}

zBOOL OPERATION
SSIsCellInSelection( HWND     hWnd,
                     SS_COORD ssCol,
                     SS_COORD ssRow )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSIsCellInSelection" );
   LPSPREADSHEET lpSS;
   LPSS_SELBLOCK lpItemList;
   SS_COORD      ss;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->MultiSelBlock.lItemCnt > 0 )
   {
//    lpItemList = (LPSS_SELBLOCK) DrLockTaskMemory( lpSS->MultiSelBlock.hItemList );
      lpItemList = (LPSS_SELBLOCK) lpSS->MultiSelBlock.hItemList;

      for ( ss = 0; !bRC && ss < lpSS->MultiSelBlock.lItemCnt; ss++ )
      {
         if ( ssCol >= lpItemList[ ss ].UL.ssCol && ssCol <= lpItemList[ ss ].LR.ssCol &&
             ssRow >= lpItemList[ ss ].UL.ssRow && ssRow <= lpItemList[ ss ].LR.ssRow )
         {
            bRC = TRUE;
         }
      }

//    DrUnlockTaskMemory( lpItemList );
   }
   else
   if ( SS_IsBlockSelected( lpSS ) )
   {
      if ( (lpSS->BlockCellUL.ssCol == -1 || lpSS->BlockCellLR.ssCol == -1 ||
            (ssCol >= lpSS->BlockCellUL.ssCol &&
             ssCol <= lpSS->BlockCellLR.ssCol)) &&
           (lpSS->BlockCellUL.ssRow == -1 || lpSS->BlockCellLR.ssRow == -1 ||
            (ssRow >= lpSS->BlockCellUL.ssRow &&
             ssRow <= lpSS->BlockCellLR.ssRow)) )
      {
         bRC = TRUE;
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSLoadFromBuffer( HWND    hWnd,
                  zPCHAR  hBuffer,
                  zLONG   lBufferLth )
{
   TRACE_DEBUG( "<SS_SAVE.CPP>", "SSLoadFromBuffer" );
   return( 1 );
}

zBOOL OPERATION
SSLoadFromFile( HWND    hWnd,
                zPCHAR  lpFileName )
{
#if 0
   TRACE_DEBUG( "<SS_SAVE.CPP>", "SSLoadFromFile" );
   HANDLE     hBuffer = 0;
   OFSTRUCT   of;
   zCHAR huge *lpBuffer;
   zLONG      lBufferLth = 0;
   int        hFile;
   WORD       Bytes;
   zBOOL      bRC;

   if ( (hFile = OpenFile( lpFileName, &of, OF_READ | OF_SHARE_EXCLUSIVE )) == -1 )
      return( FALSE );

   for ( ;; )
   {
      if ( !hBuffer )
      {
         if ( (hBuffer = GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT,
                                      SS_FILEREADSIZE )) == 0 )
         {
            _lclose( hFile );
            return( FALSE );
         }
      }

      else
      {
         GlobalUnlock( hBuffer );
         if ( (hBuffer = GlobalReAlloc( hBuffer, lBufferLth + SS_FILEREADSIZE,
                                        GMEM_MOVEABLE | GMEM_ZEROINIT )) == 0 )
         {
            _lclose( hFile );
            return( FALSE );
         }
      }

      lpBuffer = (char huge *) GlobalLock( hBuffer );

      Bytes = _lread( hFile, &lpBuffer[ lBufferLth ], SS_FILEREADSIZE );

      if ( Bytes == -1 )
         break;

      lBufferLth += Bytes;

      if ( Bytes < SS_FILEREADSIZE )
         break;
   }

   _lclose( hFile );
   GlobalUnlock( hBuffer );

   bRC = SSLoadFromBuffer( hWnd, hBuffer, lBufferLth );

   GlobalFree( hBuffer );
   return( bRC );
#else
   MESSAGE_DEBUG( "<SS_SAVE.CPP>", "SSLoadFromFile" );
   return( FALSE );
#endif
}

void OPERATION
SSLogUnitsToColWidth( HWND    hWnd,
                      zSHORT  nUnits,
                      zPLONG lpnfColWidth )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSLogUnitsToColWidth" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SS_CalcPixelsToColWidth( lpSS, 0, nUnits, lpnfColWidth );
// DrUnlockTaskMemory( lpSS );
}

void OPERATION
SSLogUnitsToRowHeight( HWND      hWnd,
                       SS_COORD  ssRow,
                       zSHORT    nUnits,
                       zPLONG    lpnfRowHeight )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSLogUnitsToRowHeight" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SSx_AdjustCellCoords( lpSS, 0, &ssRow );

   SS_CalcPixelsToRowHeight( lpSS, ssRow, nUnits, lpnfRowHeight );
// DrUnlockTaskMemory( lpSS );
}

zBOOL OPERATION
SSPrint( HWND             hWnd,
         zPCHAR           lpszPrintJobName,
         LPSS_PRINTFORMAT lpPrintFormat,
         FARPROC          lpfnPrintProc,
         zLONG            lAppData,
         FARPROC          lpfnAbortProc )
{
   TRACE_DEBUG( "<SS_PRINT.CPP>", "SSPrint" );
   return( TRUE );
}

zBOOL OPERATION
SSReplaceSel( HWND    hWnd,
              zPCHAR  lpText )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSReplaceSel" );
   LPSPREADSHEET   lpSS;
   SS_CELLTYPE     CellType;
   zBOOL           bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->EditModeOn )
   {
      if ( SS_RetrieveCellType( lpSS, &CellType, 0,
                                lpSS->Col.ssCurrAt, lpSS->Row.ssCurrAt ) &&
                                CellType.nControlID )
      {
         SendMessage( SS_GetControlhWnd( lpSS, CellType.nControlID ),
                      EM_REPLACESEL, 0, (zLONG) lpText );

         bRC = TRUE;
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zPCHAR        OPERATION
SSSaveToBuffer( HWND   hWnd,
                zPLONG lpBufferLth,
                zBOOL  fDataOnly )
{
   TRACE_DEBUG( "<SS_SAVE.CPP>", "SSSaveToBuffer" );
   return( 0 );
}

zBOOL OPERATION
SSSaveToFile( HWND    hWnd,
              zPCHAR  lpFileName,
              zBOOL   fDataOnly )
{
   TRACE_DEBUG( "<SS_SAVE.CPP>", "SSSaveToFile" );
   return( 1 );
}

zBOOL OPERATION
SSSetBorder( HWND      hWnd,
             SS_COORD  ssCol,
             SS_COORD  ssRow,
             WORD      wBorderType,
             WORD      wBorderStyle,
             COLORREF  Color )
{
   TRACE_DEBUG( "<SS_BORD.CPP>", "SSSetBorder" );
   return( SSSetBorderRange( hWnd, ssCol, ssRow, ssCol, ssRow,
                             wBorderType, wBorderStyle, Color ) );
}

zBOOL OPERATION
SSSetBorderRange( HWND      hWnd,
                  SS_COORD  ssCol,
                  SS_COORD  ssRow,
                  SS_COORD  ssCol2,
                  SS_COORD  ssRow2,
                  WORD      wBorderType,
                  WORD      wBorderStyle,
                  COLORREF  Color )
{
   TRACE_DEBUG( "<SS_BORD.CPP>", "SSSetBorderRange" );
   LPSPREADSHEET lpSS;
   SS_COORD      x;
   SS_COORD      y;
   zBOOL         RedrawOld;
   zBOOL         bRC = TRUE;

   if ( ssCol == SS_ALLCOLS || ssCol2 == SS_ALLCOLS )
   {
      ssCol = SS_ALLCOLS;
      ssCol2 = SS_ALLCOLS;
   }

   if ( ssRow == SS_ALLROWS || ssRow2 == SS_ALLROWS )
   {
      ssRow = SS_ALLROWS;
      ssRow2 = SS_ALLROWS;
   }

   if ( ssCol != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &ssCol, 0 );

   if ( ssRow != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &ssRow );

   if ( ssCol2 != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &ssCol2, 0 );

   if ( ssRow2 != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &ssRow2 );

   lpSS = SS_Lock( hWnd );

   lpSS->fSetBorder = TRUE;

   RedrawOld = lpSS->Redraw;
   lpSS->Redraw = FALSE;

   if ( wBorderType & SS_BORDERTYPE_OUTLINE )
   {
      for ( x = ssCol; x <= ssCol2; x++ )
      {
         bRC = SS_SetBorder( hWnd, lpSS, x, ssRow, SS_BORDERTYPE_TOP,
                              wBorderStyle, Color );
         bRC = SS_SetBorder( hWnd, lpSS, x, ssRow2, SS_BORDERTYPE_BOTTOM,
                              wBorderStyle, Color );
      }

      for ( y = ssRow; y <= ssRow2; y++ )
      {
         bRC = SS_SetBorder( hWnd, lpSS, ssCol, y, SS_BORDERTYPE_LEFT,
                              wBorderStyle, Color );
         bRC = SS_SetBorder( hWnd, lpSS, ssCol2, y, SS_BORDERTYPE_RIGHT,
                              wBorderStyle, Color );
      }
   }
   else
      for ( y = ssRow; y <= ssRow2 && bRC; y++ )
         for ( x = ssCol; x <= ssCol2 && bRC; x++ )
            bRC = SS_SetBorder( hWnd, lpSS, x, y,
                                 wBorderType, wBorderStyle, Color );

   SSSetBool( hWnd, SSB_REDRAW, RedrawOld );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

WORD  OPERATION
SSSetButtonDrawMode( HWND  hWnd,
                     WORD  wMode )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetButtonDrawMode" );
   LPSPREADSHEET lpSS;
   WORD          wModeOld;

   lpSS = SS_Lock( hWnd );
   wModeOld = lpSS->wButtonDrawMode;
   lpSS->wButtonDrawMode = wMode;
// DrUnlockTaskMemory( lpSS );

   return( wModeOld );
}

zBOOL OPERATION
SSSetButtonText( HWND     hWnd,
                 SS_COORD ssCol,
                 SS_COORD ssRow,
                 zPCHAR   lpText )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetButtonText" );
   SS_CELLTYPE   CellType;
   zPCHAR        lpPict;
   zPCHAR        hPict;
   zBOOL         bRC = FALSE;

   if ( SSGetCellType( hWnd, ssCol, ssRow, &CellType ) )
   {
      if ( CellType.chType == SS_TYPE_BUTTON )
      {
         if ( CellType.Spec.Button.hPictName &&
              !(CellType.Spec.Button.nPictureType & SUPERBTN_PICT_HANDLE) )
         {
//          lpPict = (zPCHAR) DrLockTaskMemory( CellType.Spec.Button.hPictName );
            lpPict = (zPCHAR) CellType.Spec.Button.hPictName;
            hPict = SS_CreateTextHandle( lpPict, 0 );
//          DrUnlockTaskMemory( lpPict );
            DrFreeTaskMemory( CellType.Spec.Button.hPictName );
            CellType.Spec.Button.hPictName = hPict;
         }

         if ( CellType.Spec.Button.hPictDownName &&
              !(CellType.Spec.Button.nPictureDownType & SUPERBTN_PICT_HANDLE) )
         {
//          lpPict = (zPCHAR) DrLockTaskMemory( CellType.Spec.Button.hPictDownName );
            lpPict = (zPCHAR) CellType.Spec.Button.hPictDownName;
            hPict = SS_CreateTextHandle( lpPict, 0 );
//          DrUnlockTaskMemory( lpPict );
            DrFreeTaskMemory( CellType.Spec.Button.hPictDownName );
            CellType.Spec.Button.hPictDownName = hPict;
         }

         if ( CellType.Spec.Button.hText )
         {
            CellType.Spec.Button.hText = (zPCHAR)
                         DrUnlockTaskMemory( CellType.Spec.Button.hText );
            DrFreeTaskMemory( CellType.Spec.Button.hText );
            CellType.Spec.Button.hText = 0;
         }

         if ( lpText && zstrlen( lpText ) )
            CellType.Spec.Button.hText = SS_CreateTextHandle( lpText, 0 );

         bRC = SSSetCellType( hWnd, ssCol, ssRow, &CellType );
      }
   }

   return( bRC );
}


zBOOL OPERATION
SSSetColPageBreak( HWND      hWnd,
                   SS_COORD  ssCol,
                   zBOOL     fPageBreak )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSSetColPageBreak" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &ssCol, 0 );

   if ( ssCol < SS_GetColsMax( lpSS ) &&
        (lpCol = SS_AllocLockCol( lpSS, ssCol )) != 0 )
   {
      lpCol->fPageBreak = fPageBreak;
      SS_UnlockColItem( lpSS, ssCol );
      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

#ifndef SS_NOUSERDATA
zBOOL OPERATION
SSSetColUserData( HWND      hWnd,
                  SS_COORD  ssCol,
                  zLONG     lUserData )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSSetColUserData" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &Col, 0 );

   if ( ssCol < SS_GetColsMax( lpSS ) &&
        (lpCol = SS_AllocLockCol( lpSS, ssCol )) != 0 )
   {
      lpCol->lUserData = lUserData;
      SS_UnlockColItem( lpSS, ssCol );
      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}
#endif

zBOOL OPERATION
SSSetColWidthInPixels( HWND hWnd, SS_COORD ssCol, zLONG lWidth )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSSetColWidthInPixels" );
   zLONG   dxWidth;

   SSLogUnitsToColWidth( hWnd, (zSHORT) lWidth, &dxWidth );
   return( SSSetColWidth( hWnd, ssCol, dxWidth ) );
}

zBOOL OPERATION
SSSetColor( HWND      hWnd,
            SS_COORD  ssCol,
            SS_COORD  ssRow,
            COLORREF  Background,
            COLORREF  Foreground )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetColor" );
   return( SSSetColorRange( hWnd, ssCol, ssRow, ssCol, ssRow, Background, Foreground ) );
}

zBOOL OPERATION
SSSetColorRange( HWND      hWnd,
                 SS_COORD  ssCol,
                 SS_COORD  ssRow,
                 SS_COORD  ssCol2,
                 SS_COORD  ssRow2,
                 COLORREF  Background,
                 COLORREF  Foreground )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetColorRange" );
   LPSPREADSHEET lpSS;
   SS_COORD      x;
   SS_COORD      y;
   zBOOL         RedrawOld;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   SS_AdjustCellRangeCoords( lpSS, &ssCol, &ssRow, &ssCol2, &ssRow2 );

   if ( ssRow != ssRow2 || ssCol != ssCol2 )
   {
      RedrawOld = lpSS->Redraw;
      lpSS->Redraw = FALSE;
      lpSS->fNoInvalidate = TRUE;
   }

   for ( y = ssRow; y <= ssRow2; y++ )
   {
      for ( x = ssCol; x <= ssCol2; x++ )
      {
         if ( x != SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SS_SetCellColor( hWnd, lpSS, x, y, Background, Foreground );
         else
         if ( x != SS_ALLCOLS && y == SS_ALLROWS )
            bRC = SS_SetColColor( hWnd, lpSS, x, Background, Foreground );
         else
         if ( x == SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SS_SetRowColor( hWnd, lpSS, y, Background, Foreground );
         else
         {
            SS_SetDocColor( hWnd, lpSS, Background, Foreground );
            bRC = TRUE;
         }

         if ( !bRC )
            break;
      }
   }

   if ( lpSS->fNoInvalidate )
   {
      lpSS->fNoInvalidate = FALSE;
      SS_InvalidateRange( hWnd, lpSS, ssCol, ssRow, ssCol2, ssRow2 );
      SSSetBool( hWnd, SSB_REDRAW, RedrawOld );
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSSetCursor( HWND    hWnd,
             WORD    wType,
             HCURSOR hCursor )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetCursor" );
   LPSPREADSHEET lpSS;

   if ( (lpSS = SS_Lock( hWnd )) != 0 )
   {
      if ( wType & SS_CURSORTYPE_DEFAULT )
         lpSS->hCursorDefault = hCursor;

      if ( wType & SS_CURSORTYPE_COLRESIZE )
         lpSS->hCursorColResize = hCursor;

      if ( wType & SS_CURSORTYPE_ROWRESIZE )
         lpSS->hCursorRowResize = hCursor;

      if ( wType & SS_CURSORTYPE_BUTTON )
         lpSS->hCursorButton = hCursor;

      if ( wType & SS_CURSORTYPE_GRAYAREA )
         lpSS->hCursorGrayArea = hCursor;

      if ( wType & SS_CURSORTYPE_LOCKEDCELL )
         lpSS->hCursorLockedCell = hCursor;

      if ( wType & SS_CURSORTYPE_COLHEADER )
         lpSS->hCursorColHeader = hCursor;

      if ( wType & SS_CURSORTYPE_ROWHEADER )
         lpSS->hCursorRowHeader = hCursor;

//    DrUnlockTaskMemory( lpSS );
      return( TRUE );
   }

   return( FALSE );
}

zBOOL OPERATION
SSSetDefFloatFormat( HWND hWnd,
                     LPFLOATFORMAT lpFloatFormat )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetDefFloatFormat" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = TRUE;

   lpSS = SS_Lock( hWnd );
   zmemcpy( &lpSS->DefaultFloatFormat,
            lpFloatFormat, zsizeof( LPFLOATFORMAT ) );
// DrUnlockTaskMemory( lpSS );

   return( bRC );
}

WORD  OPERATION
SSSetEditEnterAction( HWND          hWnd,
                      WORD          wAction )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetEditEnterAction" );
   LPSPREADSHEET lpSS;
   WORD          wRet;

   lpSS = SS_Lock( hWnd );

   wRet = lpSS->wEnterAction;
   lpSS->wEnterAction = wAction;

// DrUnlockTaskMemory( lpSS );
   return( wRet );
}

zBOOL OPERATION
SSSetEditMode( HWND  hWnd,
               zBOOL fEditModeOn )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetEditMode" );
   LPSPREADSHEET lpSS;
   zBOOL         fEditModeOnOld;

   lpSS = SS_Lock( hWnd );
   fEditModeOnOld = lpSS->EditModeOn;

   if ( mIs_hWndVisible( hWnd ) )
#ifdef SS_GP
      SendMessage( hWnd, SSM_SETEDITMODE, fEditModeOn, 0L );
#else
      PostMessage( hWnd, SSM_SETEDITMODE, fEditModeOn, 0L );
#endif

// DrUnlockTaskMemory( lpSS );
   return( fEditModeOnOld );
}

zBOOL OPERATION
SSSetFloat( HWND     hWnd,
            SS_COORD ssCol,
            SS_COORD ssRow,
            zDECIMAL dxValue )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetFloat" );
   return( SSSetFloatRange( hWnd, ssCol, ssRow, ssCol, ssRow, dxValue ) );
}

zBOOL OPERATION
SSSetFloatRange( HWND      hWnd,
                 SS_COORD  ssCol,
                 SS_COORD  ssRow,
                 SS_COORD  ssCol2,
                 SS_COORD  ssRow2,
                 zDECIMAL  dxValue )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetFloatRange" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC;

   lpSS = SS_Lock( hWnd );
   bRC = SS_SetDataRange( hWnd, lpSS, ssCol, ssRow, ssCol2, ssRow2, (zPCHAR)&dxValue,
                           SS_VALUE_FLOAT, FALSE );
// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSSetFont( HWND          hWnd,
           SS_COORD      ssCol,
           SS_COORD      ssRow,
           HFONT         hFont,
           zBOOL         fDeleteFont )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetFont" );
   return( SSSetFontRange( hWnd, ssCol, ssRow, ssCol, ssRow,
                           hFont, fDeleteFont ) );
}

zBOOL OPERATION
SSSetFontRange( HWND      hWnd,
                SS_COORD  ssCol,
                SS_COORD  ssRow,
                SS_COORD  ssCol2,
                SS_COORD  ssRow2,
                HFONT     hFont,
                zBOOL     fDeleteFont )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetFontRange" );
   LPSPREADSHEET lpSS;
   SS_COORD      x;
   SS_COORD      y;
   HFONT         hFontOld = hFont;
   zBOOL         RedrawOld;
   zBOOL         fDeleteFontObject;
   zBOOL         bRC = FALSE;

   if ( hFont == 0 )
      return( FALSE );

   if ( (lpSS = SS_Lock( hWnd )) == 0 )
      return( FALSE );

   SS_AdjustCellRangeCoords( lpSS, &ssCol, &ssRow, &ssCol2, &ssRow2 );

   if ( ssRow != ssRow2 || ssCol != ssCol2 )
   {
      RedrawOld = lpSS->Redraw;
      lpSS->Redraw = FALSE;
   }

   fDeleteFontObject = FALSE;

   for ( y = ssRow; y <= ssRow2; y++ )
   {
      for ( x = ssCol; x <= ssCol2; x++ )
      {
         if ( x != SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SSSetCellFont( hWnd, lpSS, x, y, hFont, -1, 0 );
         else
         if ( x != SS_ALLCOLS && y == SS_ALLROWS )
            bRC = SSSetColFont( hWnd, lpSS, x, hFont, -1, 0 );
         else
         if ( x == SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SSSetRowFont( hWnd, lpSS, y, hFont, -1, 0 );
         else
            bRC = SSSetDocDefaultFont( hWnd, lpSS, hFont, -1, 0 );

         if ( !bRC )
            break;
      }
   }

   if ( fDeleteFont )
      DeleteFont( hFontOld );

   if ( ssRow != ssRow2 || ssCol != ssCol2 )
      SSSetBool( hWnd, SSB_REDRAW, RedrawOld );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSSetFormula( HWND      hWnd,
              SS_COORD  ssCol,
              SS_COORD  ssRow,
              zPCHAR    Formula,
              zBOOL     BuildDependencies )
{
   TRACE_DEBUG( "<SS_FORMU.CPP>", "SSSetFormula" );
   return( SSSetFormulaRange( hWnd, ssCol, ssRow, ssCol, ssRow, Formula,
                             BuildDependencies ) );
}

zBOOL OPERATION
SSSetFormulaRange( HWND      hWnd,
                   SS_COORD  ssCol,
                   SS_COORD  ssRow,
                   SS_COORD  ssCol2,
                   SS_COORD  ssRow2,
                   zPCHAR    Formula,
                   zBOOL     BuildDependencies )
{
#if 0
   TRACE_DEBUG( "<SS_FORMU.CPP>", "SSSetFormulaRange" );
   LPSPREADSHEET lpSS;
   SS_CELLCOORD CellCoord;
   zBOOL         bRC = FALSE;
   zBOOL         RedrawOld;
   SS_COORD      x;
   SS_COORD      x1;
   SS_COORD      x2;
   SS_COORD      y;
   SS_COORD      y1;
   SS_COORD      y2;
   SS_COORD      i;
   SS_COORD      j;

   if ( ssCol == SS_ALLCOLS && ssRow == SS_ALLROWS )
      return( FALSE );

   if ( ssCol == SS_ALLCOLS || ssCol2 == SS_ALLCOLS )
   {
      ssCol = SS_ALLCOLS;
      ssCol2 = SS_ALLCOLS;
   }

   if ( ssRow == SS_ALLROWS || ssRow2 == SS_ALLROWS )
   {
      ssRow = SS_ALLROWS;
      ssRow2 = SS_ALLROWS;
   }

   if ( ssCol != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &Col, 0 );

   if ( ssRow != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &Row );

   if ( ssCol2 != SS_ALLCOLS )
      SS_AdjustCellCoords( hWnd, &Col2, 0 );

   if ( ssRow2 != SS_ALLROWS )
      SS_AdjustCellCoords( hWnd, 0, &Row2 );

   lpSS = SS_Lock( hWnd );

   if ( ssRow != ssRow2 || ssCol != ssCol2 )
   {
      RedrawOld = lpSS->Redraw;
      lpSS->Redraw = FALSE;
   }

   for ( y = ssRow; y <= ssRow2; y++ )
      for ( x = ssCol; x <= ssCol2; x++ )
         if ( !(bRC = SS_SetFormula( hWnd, lpSS, x, y, Formula, FALSE )) )
            break;

   if ( bRC && BuildDependencies )
   {
      SSBuildDependencies( hWnd );

      if ( lpSS->CalcAuto )
      {
         for ( y = ssRow; y <= ssRow2; y++ )
         {
            for ( x = ssCol; x <= ssCol2; x++ )
            {
               if ( x == -1 )
               {
                  x1 = lpSS->Col.nHeaderCnt;
                  x2 = lpSS->Col.DataCnt - 1;
               }
               else
               {
                  x1 = x;
                  x2 = x;
               }

               if ( y == -1 )
               {
                  y1 = lpSS->Row.nHeaderCnt;
                  y2 = lpSS->Row.DataCnt - 1;
               }
               else
               {
                  y1 = y;
                  y2 = y;
               }

               for ( k = x1; k <= x2; k++ )
               {
                  for ( j = y1; j <= y2; j++ )
                  {
                     CellCoord.ssCol = k;
                     CellCoord.ssRow = j;
                     SSx_CalcCell( hWnd, lpSS, &CellCoord );
                     SS_CalcDependencies( hWnd, lpSS, k, j );
                  }
               }
            }
         }
      }
   }

   if ( ssRow != ssRow2 || ssCol != ssCol2 )
      SSSetBool( hWnd, SSB_REDRAW, RedrawOld );

// DrUnlockTaskMemory( lpSS );
   return( bRC );
#else
   MESSAGE_DEBUG( "<SS_FORMU.CPP>", "SSSetFormulaRange" );
   return( 0 );
#endif
}

zBOOL OPERATION
SSSetFreeze( HWND      hWnd,
             SS_COORD  ssColsFrozen,
             SS_COORD  ssRowsFrozen )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetFreeze" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( ssColsFrozen < 0 )
      ssColsFrozen = 0;

   if ( ssColsFrozen + lpSS->Col.nHeaderCnt > SS_GetColsMax( lpSS ) )
      ssColsFrozen = SS_GetColsMax( lpSS ) - lpSS->Col.nHeaderCnt;

   if ( ssRowsFrozen < 0 )
      ssRowsFrozen = 0;

   if ( ssRowsFrozen + lpSS->Row.nHeaderCnt > SS_GetRowsMax( lpSS ) )
      ssRowsFrozen = SS_GetRowsMax( lpSS ) - lpSS->Row.nHeaderCnt;

   if ( lpSS->fUseScrollArrows && lpSS->Col.Frozen > 0 &&
       ssColsFrozen > lpSS->Col.Frozen )
      SSInsColRange( hWnd, lpSS->Col.nHeaderCnt + lpSS->Col.Frozen - 1,
                    ssColsFrozen - 1 );

   lpSS->Row.Frozen = ssRowsFrozen;
   lpSS->Col.Frozen = ssColsFrozen;

   lpSS->Row.UL = lpSS->Row.nHeaderCnt + lpSS->Row.Frozen;
   lpSS->Col.UL = lpSS->Col.nHeaderCnt + lpSS->Col.Frozen;

   SS_GetFirstValidRow( hWnd, lpSS, lpSS->Col.ssCurrAt, &lpSS->Row.ssCurrAt );
   SS_GetFirstValidCol( hWnd, lpSS, lpSS->Row.ssCurrAt, &lpSS->Col.ssCurrAt );

   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

void OPERATION
SSSetGrayAreaColor( HWND      hWnd,
                    COLORREF  Background,
                    COLORREF  Foreground )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetGrayAreaColor" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   lpSS->Color.GrayAreaBackground = Background;
   lpSS->Color.GrayAreaForeground = Foreground;

   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

// DrUnlockTaskMemory( lpSS );
}

COLORREF  OPERATION
SSSetGridColor( HWND hWnd,
                COLORREF  GridColor )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetGridColor" );
   LPSPREADSHEET lpSS;
   COLORREF      GridColorOld;

   lpSS = SS_Lock( hWnd );
   GridColorOld = lpSS->GridColor;
   if ( GridColorOld != GridColor )
   {
      lpSS->GridColor = GridColor;
      SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
   }

// DrUnlockTaskMemory( lpSS );

   return( GridColorOld );
}

WORD  OPERATION
SSSetGridType( HWND hWnd,
               WORD  wGridType )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetGridType" );
   LPSPREADSHEET lpSS;
   WORD          wGridTypeOld;

   lpSS = SS_Lock( hWnd );
   wGridTypeOld = lpSS->wGridType;
   if ( wGridTypeOld != wGridType )
   {
      lpSS->wGridType = wGridType;
      SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
   }

// DrUnlockTaskMemory( lpSS );

   return( wGridTypeOld );
}

zBOOL OPERATION
SSSetInteger( HWND     hWnd,
              SS_COORD ssCol,
              SS_COORD ssRow,
              zLONG    lValue )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetInteger" );
   return( SSSetIntegerRange( hWnd, ssCol, ssRow, ssCol, ssRow, lValue ) );
}

zBOOL OPERATION
SSSetIntegerRange( HWND          hWnd,
                   SS_COORD      ssCol,
                   SS_COORD      ssRow,
                   SS_COORD      ssCol2,
                   SS_COORD      ssRow2,
                   zLONG         lValue )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetIntegerRange" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC;

   lpSS = SS_Lock( hWnd );
   bRC = SS_SetDataRange( hWnd, lpSS, ssCol, ssRow, ssCol2, ssRow2, (zPCHAR)&lValue,
                          SS_VALUE_INT, FALSE );
// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSSetLock( HWND          hWnd,
           SS_COORD      ssCol,
           SS_COORD      ssRow,
           zBOOL         Lock )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetLock" );
   return( SSSetLockRange( hWnd, ssCol, ssRow, ssCol, ssRow, Lock ) );
}

void OPERATION
SSSetLockColor( HWND          hWnd,
                COLORREF      Background,
                COLORREF      Foreground )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetLockColor" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( Background != RGBCOLOR_DEFAULT )
      lpSS->LockColor.BackgroundId = SS_AddColor( lpSS, Background );
   else
      lpSS->LockColor.BackgroundId = 0;

   if ( Foreground != RGBCOLOR_DEFAULT )
      lpSS->LockColor.ForegroundId = SS_AddColor( lpSS, Foreground );
   else
      lpSS->LockColor.ForegroundId = 0;

// DrUnlockTaskMemory( lpSS );

   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
}

zBOOL OPERATION
SSSetLockRange( HWND          hWnd,
                SS_COORD      ssCol,
                SS_COORD      ssRow,
                SS_COORD      ssCol2,
                SS_COORD      ssRow2,
                zBOOL         bLock )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetLockRange" );
   LPSPREADSHEET lpSS;
   SS_COORD      x;
   SS_COORD      y;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   SS_AdjustCellRangeCoords( lpSS, &ssCol, &ssRow, &ssCol2, &ssRow2 );

   if ( bLock )
      bLock = SS_LOCKED_ON;
   else
      bLock = SS_LOCKED_OFF;

   for ( y = ssRow; y <= ssRow2; y++ )
      for ( x = ssCol; x <= ssCol2; x++ )
      {
         if ( x != SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SS_SetCellLock( hWnd, lpSS, x, y, bLock );
         else
         if ( x != SS_ALLCOLS && y == SS_ALLROWS )
            bRC = SS_SetColLock( hWnd, lpSS, x, bLock );
         else
         if ( x == SS_ALLCOLS && y != SS_ALLROWS )
            bRC = SS_SetRowLock( hWnd, lpSS, y, bLock );
         else
         {
            SSSetDocLock( hWnd, bLock );
            bRC = TRUE;
         }

         if ( bRC == 0 )
            break;
      }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSSetMultiSelBlocks( HWND hWnd,
                     LPSS_SELBLOCK lpSelBlockList,
                     zLONG lSelBlockCnt )
{
   TRACE_DEBUG( "<SS_MULTI.CPP>", "SSSetMultiSelBlocks" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC = TRUE;
   zSHORT        k;

   lpSS = SS_Lock( hWnd );

   SSDeSelectBlock( hWnd );

   /*
   if (lpSS->MultiSelBlock.hItemList)
      GlobalFree(lpSS->MultiSelBlock.hItemList);

   lpSS->MultiSelBlock.nItemCnt = 0;
   lpSS->MultiSelBlock.nItemAllocCnt = 0;
   lpSS->fMultipleBlocksSelected = FALSE;
   */

   if ( lpSelBlockList && lSelBlockCnt > 0 )
   {
      lpSS->fMultipleBlocksSelected = TRUE;

      for ( k = 0; k < lSelBlockCnt && bRC; k++ )
         if ( !SS_MultiSelBlockAddItem( lpSS, &lpSelBlockList[ k ].UL,
                                      &lpSelBlockList[ k ].LR ) )
            bRC = FALSE;

      if ( bRC )
      {
         lpSS->BlockCellUL.ssCol = lpSelBlockList[ lSelBlockCnt - 1 ].UL.ssCol;
         lpSS->BlockCellUL.ssRow = lpSelBlockList[ lSelBlockCnt - 1 ].UL.ssRow;

         lpSS->BlockCellLR.ssCol = lpSelBlockList[ lSelBlockCnt - 1 ].LR.ssCol;
         lpSS->BlockCellLR.ssRow = lpSelBlockList[ lSelBlockCnt - 1 ].LR.ssRow;

         lpSS->Col.ssCurrAt = mMax( lpSS->BlockCellUL.ssCol, 1 );
         lpSS->Row.ssCurrAt = mMax( lpSS->BlockCellUL.ssRow, 1 );

         SS_InvertMultiSelBlock( hWnd, 0, lpSS );
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

HWND OPERATION
SSSetOwner( HWND hWnd, HWND hWndOwner )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetOwner" );
   LPSPREADSHEET lpSS;
   HWND          hWndOwnerOld;

   TraceLineX( "SSSetOwner hWndOwner: ", (zLONG) hWndOwner );
   lpSS = SS_Lock( hWnd );
   hWndOwnerOld = lpSS->hWndOwner;
   lpSS->hWndOwner = hWndOwner;
// DrUnlockTaskMemory( lpSS );

   return( hWndOwnerOld );
}

zBOOL OPERATION
SSSetRowHeightInPixels( HWND     hWnd,
                        SS_COORD ssRow,
                        zLONG    lHeight )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSSetRowHeightInPixels" );
   zLONG dxHeight;

   SSLogUnitsToRowHeight( hWnd, ssRow, (zSHORT) lHeight, &dxHeight );
   return( SSSetRowHeight( hWnd, ssRow, dxHeight ) );
}

#ifndef SS_NOUSERDATA
zBOOL OPERATION
SSSetRowUserData( HWND      hWnd,
                  SS_COORD  ssRow,
                  zLONG     lUserData )
{
   TRACE_DEBUG( "<SS_ROW.CPP>", "SSSetRowUserData" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   zBOOL         bRC = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, 0, &Row );

   if ( ssRow < SS_GetRowsMax( lpSS ) && ( lpRow = SS_AllocLockRow( lpSS, ssRow ) )!= 0 )
   {
      lpRow->lUserData = lUserData;
      SS_UnlockRowItem( lpSS, ssRow );
      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}
#endif

zBOOL OPERATION
SSSetSel( HWND    hWnd,
          zLONG   lSelStart,
          zLONG   lSelEnd )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSSetSel" );
   LPSPREADSHEET   lpSS;
   SS_CELLTYPE     CellType;
   zBOOL           bRC = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->EditModeOn )
   {
      if ( SS_RetrieveCellType( lpSS, &CellType, 0, lpSS->Col.ssCurrAt,
                                lpSS->Row.ssCurrAt ) && CellType.nControlID )
      {
         SendMessage( SS_GetControlhWnd( lpSS, CellType.nControlID ),
                      EM_SETSEL, 0, MAKELONG( lSelStart, lSelEnd ) );

         bRC = TRUE;
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

void OPERATION
SSSetShadowColor( HWND          hWnd,
                  COLORREF      ShadowColor,
                  COLORREF      ShadowText,
                  COLORREF      ShadowDark,
                  COLORREF      ShadowLight )
{
   TRACE_DEBUG( "<SS_DOC.CPP>", "SSSetShadowColor" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );

   if ( ShadowColor != RGBCOLOR_DEFAULT )
      lpSS->Color.ShadowColorId = SS_AddColor( lpSS, ShadowColor );
   if ( ShadowText != RGBCOLOR_DEFAULT )
      lpSS->Color.ShadowTextId  = SS_AddColor( lpSS, ShadowText );
   if ( ShadowDark != RGBCOLOR_DEFAULT )
      lpSS->Color.ShadowDarkId  = SS_AddColor( lpSS, ShadowDark );
   if ( ShadowLight != RGBCOLOR_DEFAULT )
      lpSS->Color.ShadowLightId = SS_AddColor( lpSS, ShadowLight );

   SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );

// DrUnlockTaskMemory( lpSS );
}

LPSS_CELLTYPE OPERATION
SSSetTypeButton( HWND            hWnd,
                 LPSS_CELLTYPE   CellType,
                 zLONG           lStyle,
                 zPCHAR          Text,
                 zPCHAR          Picture,
                 zSHORT          nPictureType,
                 zPCHAR          PictureDown,
                 zSHORT          nPictureDownType,
                 zSHORT          nButtonType,
                 zSHORT          nShadowSize,
                 LPSUPERBTNCOLOR lpColor )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeButton" );

   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_BUTTON;
   CellType->lStyle = lStyle | SBS_NOPOINTERFOCUS;

   CellType->Spec.Button.nPictureType = nPictureType;
   CellType->Spec.Button.nPictureDownType = nPictureDownType;

   CellType->Spec.Button.nButtonType = (char) nButtonType;
   CellType->Spec.Button.nShadowSize = (char) nShadowSize;

   if ( lpColor )
   {
      zmemcpy( &CellType->Spec.Button.Color, lpColor, zsizeof( SUPERBTNCOLOR ) );

      if ( CellType->Spec.Button.Color.Color == RGBCOLOR_DEFAULT )
         CellType->Spec.Button.Color.Color = GetSysColor( COLOR_BTNFACE );

      if ( CellType->Spec.Button.Color.ColorBorder == RGBCOLOR_DEFAULT )
         CellType->Spec.Button.Color.ColorBorder = GetSysColor( COLOR_BTNTEXT );

      if ( CellType->Spec.Button.Color.ColorShadow == RGBCOLOR_DEFAULT )
         CellType->Spec.Button.Color.ColorShadow = GetSysColor( COLOR_BTNSHADOW );

      if ( CellType->Spec.Button.Color.ColorHilite == RGBCOLOR_DEFAULT )
         CellType->Spec.Button.Color.ColorHilite = RGBCOLOR_WHITE;

      if ( CellType->Spec.Button.Color.ColorText == RGBCOLOR_DEFAULT )
         CellType->Spec.Button.Color.ColorText = GetSysColor( COLOR_BTNTEXT );
   }

   if ( Text )
   {
      if ( CellType->Spec.Button.hText )
      {
         CellType->Spec.Button.hText = (zPCHAR)
                      DrUnlockTaskMemory( CellType->Spec.Button.hText );
         DrFreeTaskMemory( CellType->Spec.Button.hText );
         CellType->Spec.Button.hText = 0;
      }

      if ( (CellType->Spec.Button.hText =
                                   SS_CreateTextHandle( Text, 0 )) == 0 )
      {
         return( 0 );
      }
   }

   if ( Picture )
   {
      if ( nPictureType & SUPERBTN_PICT_HANDLE )
         CellType->Spec.Button.hPictName = *(zPCHAR *) Picture;
      else
      {
         if ( CellType->Spec.Button.hPictName )
         {
            CellType->Spec.Button.hPictName = (zPCHAR)
                       DrUnlockTaskMemory( CellType->Spec.Button.hPictName );
            DrFreeTaskMemory( CellType->Spec.Button.hPictName );
            CellType->Spec.Button.hPictName = 0;
         }

         if ( (CellType->Spec.Button.hPictName =
                                    SS_CreateTextHandle( Picture, 0 )) == 0 )
         {
            if ( CellType->Spec.Button.hText )
            {
               CellType->Spec.Button.hText = (zPCHAR)
                            DrUnlockTaskMemory( CellType->Spec.Button.hText );
               DrFreeTaskMemory( CellType->Spec.Button.hText );
               CellType->Spec.Button.hText = 0;
            }

            return( 0 );
         }
      }
   }

   if ( PictureDown )
   {
      if ( nPictureDownType & SUPERBTN_PICT_HANDLE )
         CellType->Spec.Button.hPictDownName = *(zPCHAR *) PictureDown;
      else
      if ( (CellType->Spec.Button.hPictDownName =
                             SS_CreateTextHandle( PictureDown, 0 )) == 0 )
      {
         if ( !(nPictureType & SUPERBTN_PICT_HANDLE) &&
              CellType->Spec.Button.hPictName )
         {
            CellType->Spec.Button.hPictName = (zPCHAR)
                     DrUnlockTaskMemory( CellType->Spec.Button.hPictName );
            DrFreeTaskMemory( CellType->Spec.Button.hPictName );
            CellType->Spec.Button.hPictName = 0;
         }

         if ( CellType->Spec.Button.hText )
         {
            CellType->Spec.Button.hText = (zPCHAR)
                        DrUnlockTaskMemory( CellType->Spec.Button.hText );
            DrFreeTaskMemory( CellType->Spec.Button.hText );
            CellType->Spec.Button.hText = 0;
         }

         return( 0 );
      }
   }

   if ( lpColor == 0 )
   {
      CellType->Spec.Button.Color.Color          = RGBCOLOR_DEFAULT;
      CellType->Spec.Button.Color.ColorBorder = RGBCOLOR_DEFAULT;
      CellType->Spec.Button.Color.ColorShadow = RGBCOLOR_DEFAULT;
#if ( WINVER >= 0x030a )
      CellType->Spec.Button.Color.ColorHilite = RGBCOLOR_DEFAULT;
#else
      CellType->Spec.Button.Color.ColorHilite = RGBCOLOR_WHITE;
#endif
      CellType->Spec.Button.Color.ColorText      = RGBCOLOR_DEFAULT;
   }

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypeCheckBox( HWND          hWnd,
                   LPSS_CELLTYPE lpCellType,
                   zLONG         lStyle,
                   zPCHAR        lpText,
                   zPCHAR        lpPictUp,
                   WORD          wPictUpType,
                   zPCHAR        lpPictDown,
                   WORD          wPictDownType,
                   zPCHAR        lpPictFocusUp,
                   WORD          wPictFocusUpType,
                   zPCHAR        lpPictFocusDown,
                   WORD          wPictFocusDownType,
                   zPCHAR        lpPictDisabledUp,
                   WORD          wPictDisabledUpType,
                   zPCHAR        lpPictDisabledDown,
                   WORD          wPictDisabledDownType )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeCheckBox" );
   LPSPREADSHEET lpSS;
   CHECKBOXPICTS Picts;
   zBOOL         bRC = TRUE;

   zmemset( lpCellType, 0, zsizeof( SS_CELLTYPE ) );

   lpCellType->chType = SS_TYPE_CHECKBOX;

   if ( (lStyle & 0x0F) == BS_3STATE || (lStyle & 0x0F) == BS_AUTO3STATE )
      lpCellType->lStyle = lStyle | BS_AUTO3STATE;
   else
      lpCellType->lStyle = lStyle | BS_AUTOCHECKBOX;

   lpCellType->Spec.CheckBox.bPictUpType = (BYTE) wPictUpType;
   lpCellType->Spec.CheckBox.bPictDownType = (BYTE) wPictDownType;
   lpCellType->Spec.CheckBox.bPictFocusUpType = (BYTE) wPictFocusUpType;
   lpCellType->Spec.CheckBox.bPictFocusDownType = (BYTE) wPictFocusDownType;
   lpCellType->Spec.CheckBox.bPictDisabledUpType = (BYTE) wPictDisabledUpType;
   lpCellType->Spec.CheckBox.bPictDisabledDownType = (BYTE) wPictDisabledDownType;

   if ( lpText )
   {
      if ( lpCellType->Spec.CheckBox.hText )
      {
         lpCellType->Spec.CheckBox.hText = (zPCHAR)
                      DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hText );
         DrFreeTaskMemory( lpCellType->Spec.CheckBox.hText );
         lpCellType->Spec.CheckBox.hText = 0;
      }

      if ( (lpCellType->Spec.CheckBox.hText =
                                     SS_CreateTextHandle( lpText, 0 )) == 0 )
      {
         return( 0 );
      }
   }

   if ( lpPictUp )
   {
      if ( wPictUpType & BT_HANDLE )
         lpCellType->Spec.CheckBox.hPictUpName = lpPictUp;
      else
      {
         if ( lpCellType->Spec.CheckBox.hPictUpName )
         {
            lpCellType->Spec.CheckBox.hPictUpName = (zPCHAR)
                 DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictUpName );
            DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictUpName );
            lpCellType->Spec.CheckBox.hPictUpName = 0;
         }

         if ( (lpCellType->Spec.CheckBox.hPictUpName =
                                  SS_CreateTextHandle( lpPictUp, 0 )) == 0 )
         {
            bRC = FALSE;
         }
      }
   }

   if ( bRC && lpPictDown )
   {
      if ( wPictDownType & BT_HANDLE )
         lpCellType->Spec.CheckBox.hPictDownName = lpPictDown;
      else
      {
         if ( lpCellType->Spec.CheckBox.hPictDownName )
         {
            lpCellType->Spec.CheckBox.hPictDownName = (zPCHAR)
               DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictDownName );
            DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictDownName );
            lpCellType->Spec.CheckBox.hPictDownName = 0;
         }

         if ( (lpCellType->Spec.CheckBox.hPictDownName =
                                SS_CreateTextHandle( lpPictDown, 0 )) == 0 )
         {
            bRC = FALSE;
         }
      }
   }

   if ( bRC && lpPictFocusUp )
   {
      if ( wPictFocusUpType & BT_HANDLE )
         lpCellType->Spec.CheckBox.hPictFocusUpName = lpPictFocusUp;
      else
      {
         if ( lpCellType->Spec.CheckBox.hPictFocusUpName )
         {
            lpCellType->Spec.CheckBox.hPictFocusUpName = (zPCHAR)
             DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictFocusUpName );
            DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictFocusUpName );
            lpCellType->Spec.CheckBox.hPictFocusUpName = 0;
         }

         if ( (lpCellType->Spec.CheckBox.hPictFocusUpName =
                             SS_CreateTextHandle( lpPictFocusUp, 0 )) == 0 )
         {
            bRC = FALSE;
         }
      }
   }

   if ( bRC && lpPictFocusDown )
   {
      if ( wPictFocusDownType & BT_HANDLE )
         lpCellType->Spec.CheckBox.hPictFocusDownName = lpPictFocusDown;
      else
      {
         if ( lpCellType->Spec.CheckBox.hPictFocusDownName )
         {
            lpCellType->Spec.CheckBox.hPictFocusDownName = (zPCHAR)
             DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictFocusDownName );
            DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictFocusDownName );
            lpCellType->Spec.CheckBox.hPictFocusDownName = 0;
         }

         if ( (lpCellType->Spec.CheckBox.hPictFocusDownName =
                          SS_CreateTextHandle( lpPictFocusDown, 0 )) == 0 )
         {
            bRC = FALSE;
         }
      }
   }

   if ( bRC && lpPictDisabledUp )
   {
      if ( wPictDisabledUpType & BT_HANDLE )
      {
         lpCellType->Spec.CheckBox.hPictDisabledUpName = lpPictDisabledUp;
      }
      else
      {
         if ( lpCellType->Spec.CheckBox.hPictDisabledUpName )
         {
            lpCellType->Spec.CheckBox.hPictDisabledUpName = (zPCHAR)
             DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledUpName );
            DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledUpName );
            lpCellType->Spec.CheckBox.hPictDisabledUpName = 0;
         }

         if ( (lpCellType->Spec.CheckBox.hPictDisabledUpName =
                           SS_CreateTextHandle( lpPictDisabledUp, 0 )) == 0 )
         {
            bRC = FALSE;
         }
      }
   }

   if ( bRC && lpPictDisabledDown )
   {
      if ( wPictDisabledDownType & BT_HANDLE )
      {
         lpCellType->Spec.CheckBox.hPictDisabledDownName = lpPictDisabledDown;
      }
      else
      {
         if ( lpCellType->Spec.CheckBox.hPictDisabledDownName )
         {
            lpCellType->Spec.CheckBox.hPictDisabledDownName = (zPCHAR)
             DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledDownName );
            DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledDownName );
            lpCellType->Spec.CheckBox.hPictDisabledDownName = 0;
         }

         if ( (lpCellType->Spec.CheckBox.hPictDisabledDownName =
                         SS_CreateTextHandle( lpPictDisabledDown, 0 )) == 0 )
         {
            bRC = FALSE;
         }
      }
   }

   if ( bRC == FALSE )
   {
      if ( !(wPictUpType & BT_HANDLE) &&
           lpCellType->Spec.CheckBox.hPictUpName )
      {
         lpCellType->Spec.CheckBox.hPictUpName = (zPCHAR)
                DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictUpName );
         DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictUpName );
         lpCellType->Spec.CheckBox.hPictUpName = 0;
      }

      if ( !(wPictDownType & BT_HANDLE) &&
           lpCellType->Spec.CheckBox.hPictDownName )
      {
         lpCellType->Spec.CheckBox.hPictDownName = (zPCHAR)
            DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictDownName );
         DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictDownName );
         lpCellType->Spec.CheckBox.hPictDownName = 0;
      }

      if ( !(wPictFocusUpType & BT_HANDLE) &&
           lpCellType->Spec.CheckBox.hPictFocusUpName )
      {
         lpCellType->Spec.CheckBox.hPictFocusUpName = (zPCHAR)
            DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictFocusUpName );
         DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictFocusUpName );
         lpCellType->Spec.CheckBox.hPictFocusUpName = 0;
      }

      if ( !(wPictFocusDownType & BT_HANDLE ) &&
           lpCellType->Spec.CheckBox.hPictFocusDownName)
      {
         lpCellType->Spec.CheckBox.hPictFocusDownName = (zPCHAR)
            DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictFocusDownName );
         DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictFocusDownName );
         lpCellType->Spec.CheckBox.hPictFocusDownName = 0;
      }

      if ( !(wPictDisabledUpType & BT_HANDLE) &&
           lpCellType->Spec.CheckBox.hPictDisabledUpName )
      {
         lpCellType->Spec.CheckBox.hPictDisabledUpName = (zPCHAR)
            DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledUpName );
         DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledUpName );
         lpCellType->Spec.CheckBox.hPictDisabledUpName = 0;
      }

      if ( !(wPictDisabledDownType & BT_HANDLE) &&
           lpCellType->Spec.CheckBox.hPictDisabledDownName )
      {
         lpCellType->Spec.CheckBox.hPictDisabledDownName = (zPCHAR)
            DrUnlockTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledDownName );
         DrFreeTaskMemory( lpCellType->Spec.CheckBox.hPictDisabledDownName );
         lpCellType->Spec.CheckBox.hPictDisabledDownName = 0;
      }

      return( 0 );
   }

   if ( lpPictUp == 0 && lpPictDown == 0 &&
        lpPictFocusUp == 0 && lpPictFocusDown == 0 &&
        lpPictDisabledUp == 0 && lpPictDisabledDown == 0 )
   {
      lpSS = SS_Lock( hWnd );

      if ( lpSS->fCheckBoxDefBitmapsLoaded == 0 )
      {
         lpSS->fCheckBoxDefBitmapsLoaded = TRUE;
         CheckBoxLoadDefBitmaps( g_hInstanceDLL );
      }

//    DrUnlockTaskMemory( lpSS );

      kzCheckBoxGetPict( g_hInstanceDLL, &Picts );
      lpCellType->Spec.CheckBox.hPictUpName =
                               (zPCHAR) Picts.Pict.hPict;
      lpCellType->Spec.CheckBox.bPictUpType |=
                               BT_HANDLE | Picts.Pict.chPictType;
      lpCellType->Spec.CheckBox.hPictDownName =
                               (zPCHAR) Picts.PictBtnDown.hPict;
      lpCellType->Spec.CheckBox.bPictDownType |=
                               BT_HANDLE | Picts.PictBtnDown.chPictType;
      lpCellType->Spec.CheckBox.hPictFocusUpName =
                               (zPCHAR) Picts.PictFocus.hPict;
      lpCellType->Spec.CheckBox.bPictFocusUpType |=
                               BT_HANDLE | Picts.PictFocus.chPictType;
      lpCellType->Spec.CheckBox.hPictFocusDownName =
                               (zPCHAR) Picts.PictFocusDown.hPict;
      lpCellType->Spec.CheckBox.bPictFocusDownType |=
                               BT_HANDLE | Picts.PictFocusDown.chPictType;
      lpCellType->Spec.CheckBox.hPictDisabledUpName =
                               (zPCHAR) Picts.PictDisabled.hPict;
      lpCellType->Spec.CheckBox.bPictDisabledUpType |=
                               BT_HANDLE | Picts.PictDisabled.chPictType;
      lpCellType->Spec.CheckBox.hPictDisabledDownName =
                              (zPCHAR) Picts.PictDisabledDown.hPict;
      lpCellType->Spec.CheckBox.bPictDisabledDownType |=
                               BT_HANDLE | Picts.PictDisabledDown.chPictType;
   }

   return( lpCellType );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// local functions
void
DateGetDefFormat( LPDATEFORMAT lpDateFormat )
{
   TRACE_DEBUG( "<USERDATE.CPP>", "DateGetDefFormat" );
   zCHAR        szTemp[ 20 ];
   zPCHAR       Ptr;

   switch ( (zBOOL) GetProfileInt( "intl", "iDate", -1 ) )
   {
      case -1:
      case 0:
         lpDateFormat->nFormat = IDF_MMDDYY;
         break;

      case 1:
         lpDateFormat->nFormat = IDF_DDMMYY;
         break;

      case 2:
         lpDateFormat->nFormat = IDF_YYMMDD;
         break;
   }

   GetProfileString ( "intl", "sDate", "/", szTemp, 2 );
   if ( szTemp[ 0 ] )
      lpDateFormat->cSeparator = szTemp[ 0 ];
   else                                   // Windows seems to return 0
      lpDateFormat->cSeparator = ' ';     //    if you specified a space.

   GetProfileString( "intl", "sShortDate", "M/d/yy", szTemp, 11 );

   lpDateFormat->bCentury = 0;

   if ( (Ptr = zstrchr( szTemp, 'y' )) != 0 ||
        (Ptr = zstrchr( szTemp, 'Y' )) != 0 )
   {
      if ( zstrstr( Ptr, "yyyy" ) || zstrstr( Ptr, "YYYY" ) )
         lpDateFormat->bCentury = 1;
   }

   lpDateFormat->bSpin = 0;
}

zBOOL OPERATION
DateGetFormat( HWND hWnd, LPDATEFORMAT dx )
{
   TRACE_DEBUG( "<USERDATE.CPP>", "DateGetFormat " );
   if ( hWnd )
      SendMessage( hWnd, EM_GETFORMAT, 0, (zLONG) dx );
   else
      DateGetDefFormat( dx );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LPSS_CELLTYPE OPERATION
SSSetTypeDate( HWND          hWnd,
               LPSS_CELLTYPE CellType,
               zLONG         lStyle,
               LPDATEFORMAT  lpFormat,
               LPDATE        lpMin,
               LPDATE        lpMax )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeDate" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_DATE;
   CellType->lStyle = lStyle;
   CellType->Spec.Date.SetFormat = TRUE;

   if ( lpFormat )
   {
      zmemcpy( &CellType->Spec.Date.Format, lpFormat, zsizeof( DATEFORMAT ) );

      if ( CellType->Spec.Date.Format.bSpin )
         CellType->lStyle |= DS_SPIN;
   }
   else
   {
      DateGetFormat( 0, &CellType->Spec.Date.Format );

      if ( lStyle & DS_SPIN )
         CellType->Spec.Date.Format.bSpin = TRUE;

      if ( lStyle & DS_DDMONYY )
         CellType->Spec.Date.Format.nFormat = IDF_DDMONYY;

      if ( lStyle & DS_DDMMYY )
         CellType->Spec.Date.Format.nFormat = IDF_DDMMYY;

      if ( lStyle & DS_MMDDYY )
         CellType->Spec.Date.Format.nFormat = IDF_MMDDYY;

      if ( lStyle & DS_YYMMDD )
         CellType->Spec.Date.Format.nFormat = IDF_YYMMDD;

      if ( lStyle & DS_CENTURY )
         CellType->Spec.Date.Format.bCentury = TRUE;

      if ( !(lStyle & DS_DEFCENTURY) )
         CellType->Spec.Date.Format.bCentury = FALSE;
   }

   if ( lpMin && lpMax )
   {
      CellType->Spec.Date.SetRange = TRUE;
      zmemcpy( &CellType->Spec.Date.Min, lpMin, zsizeof( TB_DATE ) );
      zmemcpy( &CellType->Spec.Date.Max, lpMax, zsizeof( TB_DATE ) );
   }

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypeFloat( HWND          hWnd,
                LPSS_CELLTYPE CellType,
                zLONG         lStyle,
                zSHORT        nLeft,
                zSHORT        nRight,
                zDECIMAL      dxMin,
                zDECIMAL      dxMax )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeFloat" );
   return( SSSetTypeFloatExt( hWnd, CellType, lStyle,
                              nLeft, nRight, dxMin, dxMax, 0 ) );
}

LPSS_CELLTYPE OPERATION
SSSetTypeFloatExt( HWND          hWnd,
                   LPSS_CELLTYPE CellType,
                   zLONG         lStyle,
                   zSHORT        nLeft,
                   zSHORT        nRight,
                   zDECIMAL      dxMin,
                   zDECIMAL      dxMax,
                   LPFLOATFORMAT lpFormat )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeFloatExt" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType            = SS_TYPE_FLOAT;
   CellType->lStyle            = lStyle;
   CellType->Spec.Float.nLeft  = nLeft;
   CellType->Spec.Float.nRight = nRight;
   CellType->Spec.Float.dxMin  = dxMin;
   CellType->Spec.Float.dxMax  = dxMax;

   if ( lpFormat )
   {
      zmemcpy( &CellType->Spec.Float.Format, lpFormat, zsizeof( FLOATFORMAT ) );
      CellType->Spec.Float.fSetFormat = TRUE;
   }

   /*
   if ( !SS_CreateControl( hWnd, CellType ) )
      return( 0 );
   */

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypeInteger( HWND          hWnd,
                  LPSS_CELLTYPE CellType,
                  zLONG         lMin,
                  zLONG         lMax )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeInteger" );
   return( SSSetTypeIntegerExt( hWnd, CellType, 0L, lMin, lMax, FALSE, 0L ) );
}

LPSS_CELLTYPE OPERATION
SSSetTypeIntegerExt( HWND          hWnd,
                     LPSS_CELLTYPE CellType,
                     zLONG         lStyle,
                     zLONG         lMin,
                     zLONG         lMax,
                     zBOOL         fSpinWrap,
                     zLONG         lSpinInc )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeIntegerExt" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType                 = SS_TYPE_INTEGER;
   CellType->lStyle                 = lStyle;
   CellType->Spec.Integer.lMin      = lMin;
   CellType->Spec.Integer.lMax      = lMax;
   CellType->Spec.Integer.fSpinWrap = fSpinWrap;
   CellType->Spec.Integer.lSpinInc  = lSpinInc;

   /*
   if (!SS_CreateControl(hWnd, CellType))
      return( 0 );
   */

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypeOwnerDraw( HWND          hWnd,
                    LPSS_CELLTYPE CellType,
                    zLONG         lStyle )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeOwnerDraw" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_OWNERDRAW;
   CellType->lStyle = lStyle;

   return( CellType );
}

LPSS_CELLTYPE OPERATION
SSSetTypePic( HWND          hWnd,
              LPSS_CELLTYPE CellType,
              zLONG         lStyle,
              zPCHAR        pchMask )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypePic" );
   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   if ( pchMask == 0 )
      return( 0 );

   CellType->chType = SS_TYPE_PIC;
   CellType->lStyle = lStyle;

   if ( CellType->Spec.Pic.hMask )
   {
      CellType->Spec.Pic.hMask = (zPCHAR)
                   DrUnlockTaskMemory( CellType->Spec.Pic.hMask );
      DrFreeTaskMemory( CellType->Spec.Pic.hMask );
      CellType->Spec.Pic.hMask = 0;
   }

   if ( (CellType->Spec.Pic.hMask = SS_CreateTextHandle( pchMask, 0 )) == 0 )
      return( 0 );

   return( CellType );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// local function
void
SS_TimeGetDefault( LPTIMEFORMAT lpTimeFormat )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SS_TimeGetDefault" );
   zCHAR szTemp[ 2 ];

   lpTimeFormat->b24Hour = (zBOOL) GetProfileInt( "intl", "iTime", 0 );
   lpTimeFormat->bSeconds = FALSE;

   GetProfileString( "intl", "sTime", ":", szTemp, 2 );
   lpTimeFormat->cSeparator = szTemp[ 0 ];
   lpTimeFormat->bSpin = 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LPSS_CELLTYPE OPERATION
SSSetTypeTime( HWND          hWnd,
               LPSS_CELLTYPE CellType,
               zLONG         lStyle,
               LPTIMEFORMAT  lpFormat,
               LPTIME        lpMin,
               LPTIME        lpMax )
{
   TRACE_DEBUG( "<SS_TYPE.CPP>", "SSSetTypeTime" );
   LPSPREADSHEET   lpSS;

   zmemset( CellType, 0, zsizeof( SS_CELLTYPE ) );

   CellType->chType = SS_TYPE_TIME;
   CellType->lStyle = lStyle;

   if ( lpFormat )
   {
      CellType->Spec.Time.SetFormat = TRUE;
      zmemcpy( &CellType->Spec.Time.Format, lpFormat, zsizeof( TIMEFORMAT ) );

      if ( CellType->Spec.Time.Format.bSpin )
         CellType->lStyle |= TS_SPIN;
   }

   if ( lpMin && lpMax )
   {
      CellType->Spec.Time.SetRange = TRUE;
      zmemcpy( &CellType->Spec.Time.Min, lpMin, zsizeof( TIME ) );
      zmemcpy( &CellType->Spec.Time.Max, lpMax, zsizeof( TIME ) );
   }

   if ( lpFormat == 0 )
   {
      CellType->Spec.Time.SetFormat = TRUE;
      lpSS = SS_Lock( hWnd );
      SS_TimeGetDefault( &CellType->Spec.Time.Format );
//    DrUnlockTaskMemory( lpSS );
   }

   return( CellType );
}

WORD  OPERATION
SSSetUnitType( HWND  hWnd,
               WORD  wUnitType )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetUnitType" );
   LPSPREADSHEET lpSS;
   WORD          wRet;

   lpSS = SS_Lock( hWnd );

   wRet = lpSS->nUnitType;
   lpSS->nUnitType = wUnitType;

// DrUnlockTaskMemory( lpSS );

   return( wRet );
}

#ifndef SS_NOUSERDATA
zLONG OPERATION
SSSetUserData( HWND  hWnd,
               zLONG lUserData )
{
   TRACE_DEBUG( "<SS_DATA.CPP>", "SSSetUserData" );
   LPSPREADSHEET lpSS;
   zLONG         lRet;

   lpSS = SS_Lock( hWnd );

   lRet = lpSS->lUserData;
   lpSS->lUserData = lUserData;

// DrUnlockTaskMemory( lpSS );

   return( lRet );
}
#endif

WORD  OPERATION
SSSetUserResize( HWND  hWnd,
                 WORD  wUserResize )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSSetUserResize" );
   LPSPREADSHEET lpSS;
   WORD          wUserResizeOld;

   lpSS = SS_Lock( hWnd );
   wUserResizeOld = lpSS->wUserResize;
   lpSS->wUserResize = wUserResize;
// DrUnlockTaskMemory( lpSS );

   return( wUserResizeOld );
}

zBOOL OPERATION
SSShowActiveCell( HWND   hWnd,
                  zLONG lPosition )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSShowActiveCell" );
   return( SS_ShowActiveCell( hWnd, lPosition ) );
}

zBOOL OPERATION
SSShowCell( HWND      hWnd,
            SS_COORD  CellCol,
            SS_COORD  CellRow,
            zLONG     lPosition )
{
   TRACE_DEBUG( "<SS_SCROL.CPP>", "SSShowCell" );
   LPSPREADSHEET lpSS;
   zBOOL         bRC;
   zBOOL         fEditModeOn = FALSE;

   lpSS = SS_Lock( hWnd );

   if ( CellCol != -1 )
      SS_AdjustCellCoords( hWnd, &CellCol, 0 );

   if ( CellRow != -1 )
      SS_AdjustCellCoords( hWnd, 0, &CellRow );

   if ( CellCol >= SS_GetColsMax( lpSS ) ||
       CellRow >= SS_GetRowsMax( lpSS ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   if ( lpSS->EditModeOn )
   {
      WORD wMessageBeingSentOld = lpSS->wMessageBeingSent;
      lpSS->wMessageBeingSent = FALSE;
      SS_CellEditModeOff( hWnd, lpSS, 0 );
      lpSS->wMessageBeingSent = wMessageBeingSentOld;
      SS_HiliteCell( hWnd, FALSE );
      fEditModeOn = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   bRC = SS_ShowCell( hWnd, lPosition, CellCol, CellRow );

   if ( fEditModeOn && SS_IsCellVisible( hWnd, lpSS, lpSS->Col.ssCurrAt,
                                         lpSS->Row.ssCurrAt, SS_VISIBLE_ALL ) )
      PostMessage( hWnd, SSM_SETEDITMODE, TRUE, 0L );
   else
      SS_HiliteCell( hWnd, TRUE );

   return( bRC );
}

zBOOL OPERATION
SSShowCol( HWND      hWnd,
           SS_COORD  ssCol,
           zBOOL     fShow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSShowCol" );
   LPSPREADSHEET lpSS;
   LPSS_COL      lpCol;
   BYTE          bUserResize = 0;

   lpSS = SS_Lock( hWnd );

   if ( ( lpCol = SS_LockColItem( lpSS, 0 ) ) != 0 )
   {
      bUserResize = lpCol->bUserResize;
      SS_UnlockColItem( lpSS, 0 );
   }

   if ( fShow )
   {
      if ( SS_GetColWidth( lpSS, ssCol ) == 0 )
      {
         SSSetColWidth( hWnd, ssCol, SS_WIDTH_DEFAULT );
         SSSetUserResizeCol( hWnd, ssCol, SS_RESIZE_ON );
      }
   }
   else
   {
      SSSetColWidth( hWnd, ssCol, 0 );
      SSSetUserResizeCol( hWnd, ssCol, SS_RESIZE_OFF );
   }

   if ( ( lpCol = SS_LockColItem( lpSS, 0 ) ) != 0 )
   {
      lpCol->bUserResize = bUserResize;
      SS_UnlockColItem( lpSS, 0 );
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

zBOOL OPERATION
SSShowRow( HWND      hWnd,
           SS_COORD  ssRow,
           zBOOL     fShow )
{
   TRACE_DEBUG( "<SS_USER.CPP>", "SSShowRow" );
   LPSPREADSHEET lpSS;
   LPSS_ROW      lpRow;
   BYTE          bUserResize = 0;

   lpSS = SS_Lock( hWnd );

   if ( ( lpRow = SS_LockRowItem( lpSS, 0 ) ) != 0 )
   {
      bUserResize = lpRow->bUserResize;
      SS_UnlockRowItem( lpSS, 0 );
   }

   if ( fShow )
   {
      if ( SS_GetRowHeight( lpSS, ssRow ) == 0 )
      {
         SSSetRowHeight( hWnd, ssRow, SS_HEIGHT_DEFAULT );
         SSSetUserResizeRow( hWnd, ssRow, SS_RESIZE_ON );
      }
   }
   else
   {
      SSSetRowHeight( hWnd, ssRow, 0 );
      SSSetUserResizeRow( hWnd, ssRow, SS_RESIZE_OFF );
   }

   if ( ( lpRow = SS_LockRowItem( lpSS, 0 ) ) != 0 )
   {
      lpRow->bUserResize = bUserResize;
      SS_UnlockRowItem( lpSS, 0 );
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

#ifdef DKS_COPY

zBOOL OPERATION
SSSwapRange( HWND hWnd, SS_COORD ssCol, SS_COORD ssRow,
             SS_COORD ssCol2, SS_COORD ssRow2, SS_COORD ssColDest,
             SS_COORD ssRowDest )
{
   TRACE_DEBUG( "<SS_COPY.CPP>", "SSSwapRange" );
   return( SS_CopySwapRange( hWnd, ssCol, ssRow, ssCol2, ssRow2, ssColDest, ssRowDest,
                             SS_CMD_SWAP, FALSE ) );
}

#endif

/////////////////////////////////////////////////////////////////////////////
//
// double atof(nptr) - convert string to floating point number
//
// Purpose:
//    atof recognizes an optional string of tabs and spaces, then
//    an optional sign, then a string of digits optionally
//    containing a decimal point, then an optional e or E followed
//    by an optionally signed integer, and converts all this to
//    to a floating point number.  The first unrecognized
//    character ends the string.
//
// Entry:
//    nptr - pointer to string to convert
//
// Exit:
//    returns floating point value of character representation
//
// Exceptions:
//
/////////////////////////////////////////////////////////////////////////////
#define CVTBUFSIZE (309 + 40) // # of digits in max. dp value + slop
char g_cvtb[ CVTBUFSIZE ];

void
StringToFloat( zPCHAR pch, DECIMAL *pdxVal )
{
   TRACE_DEBUG( "<ATOF.CPP>", "StringToFloat" );
   strcpy_s( g_cvtb, zsizeof( g_cvtb ), pch ); do we know if we can take the zsizeof g_cvtb?
   ::MessageBox( 0, "Should not be here!!!", "StringToFloag", MB_OK );
// *pdxVal = ZDecimal( g_cvtb );
// *pdxVal = atof( g_cvtb );
}

FARPROC OPERATION
fnMakeProcInstance( FARPROC    fp,
                    HINSTANCE  hInst )
{
   return( MakeProcInstance( fp, hInst ) );
}

void OPERATION
fnFreeProcInstance( FARPROC fp )
{
   FreeProcInstance( fp );
}

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

// #define DKS_TEST

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <math.h>

zBOOL OPERATION
SSBuildDependencies( HWND  hWnd )
{
#if 0
   TRACE_DEBUG( "<SS_CALC.CPP>", "SSBuildDependencies" );

   zzPCHAR *        CalcTable;
   LPSPREADSHEET    lpSS;
   LPSS_ROW         lpRow;
   LPSS_COL         lpCol;
   LPSS_CELL        lpCell;
   SS_CELLCOORD     CellCoord;
   LPSS_CALC        Calc;
   SS_COORD         k;
   SS_COORD         j;

   lpSS = SS_Lock( hWnd );

   if ( lpSS->fCalcBuildDependencies == 0 )
      return( TRUE );

   if ( lpSS->CalcTableCnt == 0 )
      return( TRUE );

   // Delete all dependency lists.
   SS_FreeCalcList( &lpSS->CalcAllDependents );
   for ( k = 0; k < lpSS->Row.AllocCnt; k++ )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, k )) != 0 )
      {
         if ( lpRow->Dependents.hItems )
            SS_FreeCalcList( &lpRow->Dependents );

         for ( j = 0; j < lpSS->Col.AllocCnt; j++ )
         {
            if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, j, k )) != 0 )
            {
               if ( lpCell->m_Dependents.hItems )
                  SS_FreeCalcList( &lpCell->m_Dependents );

               SS_UnlockCellItem( lpSS, j, k );
            }
         }

         SS_UnlockRowItem( lpSS, k );
      }
   }

   for ( k = 0; k < lpSS->Col.AllocCnt; k++ )
   {
      if ( (lpCol = SS_LockColItem( lpSS, k )) != 0 )
      {
         if ( lpCol->Dependents.hItems )
            SS_FreeCalcList( &lpCol->Dependents );

         SS_UnlockColItem( lpSS, k );
      }
   }

   if ( lpSS->hCalcTable )
   {
//    CalcTable = (zzPCHAR *) DrLockTaskMemory( lpSS->hCalcTable );
      CalcTable = (zzPCHAR *) lpSS->hCalcTable;
      for ( k = 0; k < lpSS->CalcTableCnt; k++ )
      {
//       Calc = (LPSS_CALC) DrLockTaskMemory( CalcTable[ k ] );
         Calc = (LPSS_CALC) CalcTable[ k ];

         if ( !SS_CalcBuildDependencies1( hWnd, lpSS, Calc ) )
         {
//          DrUnlockTaskMemory( Calc );
//          DrUnlockTaskMemory( CalcTable );
//          DrUnlockTaskMemory( lpSS );
            return( FALSE );
         }

//       DrUnlockTaskMemory( Calc );
      }

//    DrUnlockTaskMemory( CalcTable );
   }

   CellCoord.ssRow = -1;
   CellCoord.ssCol = -1;

   if ( !SS_CalcBuildDependencies2( hWnd, lpSS,
                                    &lpSS->CalcAllDependents, &CellCoord ) )
   {
//    DrUnlockTaskMemory( lpSS );
      return( FALSE );
   }

   for ( k = 0; k < lpSS->Row.AllocCnt; k++ )
   {
      if ( (lpRow = SS_LockRowItem( lpSS, k )) != 0 )
      {
         if ( lpRow->Dependents.hItems )
         {
            CellCoord.ssRow = k;
            CellCoord.ssCol = -1;

            if ( !SS_CalcBuildDependencies2( hWnd, lpSS, &lpRow->Dependents,
                                             &CellCoord ) )
            {
               SS_UnlockRowItem( lpSS, k );
//             DrUnlockTaskMemory( lpSS );
               return( FALSE );
            }
         }

         for ( j = 0; j < lpSS->Col.AllocCnt; j++ )
         {
            if ( (lpCell = SSx_LockCellItem( lpSS, lpRow, j, k )) != 0 )
            {
               if ( lpCell->m_Dependents.hItems )
               {
                  CellCoord.ssRow = k;
                  CellCoord.ssCol = j;

                  if ( !SS_CalcBuildDependencies2( hWnd, lpSS,
                                                   &lpCell->m_Dependents,
                                                   &CellCoord ) )
                  {
                     SS_UnlockCellItem( lpSS, j, k );
                     SS_UnlockRowItem( lpSS, k );
//                   DrUnlockTaskMemory( lpSS );
                     return( FALSE );
                  }
               }

               SS_UnlockCellItem( lpSS, j, k );
            }
         }

         SS_UnlockRowItem( lpSS, k );
      }
   }

   for ( k = 0; k < lpSS->Col.AllocCnt; k++ )
   {
      if ( (lpCol = SS_LockColItem( lpSS, k )) != 0 )
      {
         if ( lpCol->Dependents.hItems )
         {
            CellCoord.ssRow = -1;
            CellCoord.ssCol = k;
            if ( !SS_CalcBuildDependencies2( hWnd, lpSS, &lpCol->Dependents,
                                             &CellCoord ) )
            {
               SS_UnlockColItem( lpSS, k );
//             DrUnlockTaskMemory( lpSS );
               return( FALSE );
            }
         }

         SS_UnlockColItem( lpSS, k );
      }
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
#else
   TRACE_DEBUG( "<SS_CALC.CPP>", "SSBuildDependencies???" );
   return( TRUE );
#endif
}

zBOOL
SS_CalcDependencies( HWND          hWnd,
                     LPSPREADSHEET lpSS,
                     SS_COORD      ssCol,
                     SS_COORD      ssRow )
{
   TRACE_DEBUG( "<SS_CALC.CPP>", "SS_CalcDependencies" );
   return( 1 );
}

void
SS_FreeCalcFunctions( LPSPREADSHEET lpSS )
{
   TRACE_DEBUG( "<SS_CALC.CPP>", "SS_FreeCalcFunctions" );
}

double
SS_Round( double Val,
          zLONG    lPrecision )
{
   TRACE_DEBUG( "<SS_CALC.CPP>", "SS_Round" );
   double Power;

   Power = pow( 10.0, lPrecision );
   Val = ((zLONG) ((Val + (0.5 / Power)) * Power )) / Power;

   return( Val );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_cell.cpp

zPCHAR
SS_GetData( LPSPREADSHEET lpSS,
            LPSS_CELLTYPE lpCellType,
            SS_COORD      ssCol,
            SS_COORD      ssRow,
            zBOOL         fValue )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_GetData" );
   LPSS_ROW      lpRow;
   zPCHAR        hData;

// if ( ssRow == 1 )
//    TraceLineI( "SS_GetData ", ssRow );

   lpRow = SS_LockRowItem( lpSS, ssRow );
   hData = SSx_GetData( lpSS, 0, lpRow, 0, lpCellType, 0,
                        ssCol, ssRow, fValue );
   SS_UnlockRowItem( lpSS, ssRow );

   return( hData );
}

zBOOL
SS_GetDataStruct( LPSPREADSHEET  lpSS,
                  LPSS_COL       lpCol,
                  LPSS_ROW       lpRow,
                  LPSS_CELL      lpCell,
                  LPSS_DATA      lpData,
                  SS_COORD       ssCol,
                  SS_COORD       ssRow )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_GetDataStruct" );
   LPSS_CELL      lpCellOrig;

   lpCellOrig = lpCell;

   if ( ssRow != SS_ALLROWS && ssCol != SS_ALLCOLS &&
        (lpCell || (lpCell = SS_LockCellItem( lpSS, ssCol, ssRow )) != 0 ) &&
        lpCell->m_CellData.m_DataType )
   {
      zmemcpy( lpData, &lpCell->m_CellData, zsizeof( ZSS_Data ) );
   }
   else
   if ( ssRow != SS_ALLROWS && lpRow && lpRow->Data.m_DataType )
      zmemcpy( lpData, &lpRow->Data, zsizeof( ZSS_Data ) );
   else
   if ( ssCol != SS_ALLCOLS && lpCol && lpCol->Data.m_DataType )
      zmemcpy( lpData, &lpCol->Data, zsizeof( ZSS_Data ) );
   else
      zmemcpy( lpData, &lpSS->DefaultData, zsizeof( ZSS_Data ) );

   if ( lpCell && lpCellOrig == 0 )
      SS_UnlockCellItem( lpSS, ssCol, ssRow );

   return( lpData->m_DataType );
}

zBOOL
SSIsCellVisible( HWND      hWnd,
                 SS_COORD  CellCol,
                 SS_COORD  CellRow,
                 zSHORT    nVisible )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSIsCellVisible" );
   LPSPREADSHEET lpSS;
   zBOOL         fRet = FALSE;

   lpSS = SS_Lock( hWnd );
   SS_AdjustCellCoords( hWnd, &CellCol, &CellRow );
   fRet = SS_IsCellVisible( hWnd, lpSS, CellCol, CellRow, nVisible );

// DrUnlockTaskMemory( lpSS );
   return( fRet );
}

zBOOL
SSSetCellCellType( HWND hWnd, LPSPREADSHEET lpSS,
                   SS_COORD CellCol, SS_COORD CellRow,
                   LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSSetCellCellType" );
   LPSS_CELL     lpCell;
   LPSS_CELLTYPE CellTypeTemp;
   SS_CELLTYPE   CellTypeOld;
   zPCHAR        hData;
   zPCHAR        lpData;
   zBOOL         fSetData = FALSE;
   zBOOL         fTurnEditModeOn = FALSE;

   if ( (lpCell = SS_AllocLockCell( lpSS, 0, CellCol, CellRow )) != 0 )
   {
      if ( lpSS->EditModeOn && CellCol == lpSS->Col.ssCurrAt &&
           CellRow == lpSS->Row.ssCurrAt )
      {
         SS_CellEditModeOff( hWnd, lpSS, 0 );
         fTurnEditModeOn = TRUE;
      }

      SS_RetrieveCellType( lpSS, &CellTypeOld, lpCell, CellCol, CellRow );
      SS_DeAllocCellType( hWnd, lpSS, lpCell->m_hCellType );
      lpCell->m_hCellType = 0;

      if ( CellType )
      {
         SSx_FreeData( &lpCell->m_CellData );
         if ( DrAllocTaskMemory( (zCOREMEM) &lpCell->m_hCellType,
                                 zsizeof( SS_CELLTYPE ), 1070 ) != 0 )
         {
            return( FALSE );
         }

         if ( lpCell->m_CellData.m_DataType == SS_TYPE_FLOAT &&
              CellType->chType != SS_TYPE_FLOAT )
         {
            fSetData = TRUE;
         }

         CellTypeTemp = (LPSS_CELLTYPE)
                                    DrLockTaskMemory( lpCell->m_hCellType );
         lpCell->m_hCellType = (zPCHAR) CellTypeTemp;
         zmemcpy( CellTypeTemp, CellType, zsizeof( SS_CELLTYPE ) );
//       DrUnlockTaskMemory( CellTypeTemp );
      }

      if ( fSetData )
      {
         if ( (hData = SS_GetData( lpSS, &CellTypeOld,
                                   CellCol, CellRow, FALSE )) != 0 )
         {
//          lpData = (zPCHAR) DrLockTaskMemory( hData );
            lpData = (zPCHAR) hData;
            SSSetData( hWnd, CellCol, CellRow, lpData );
//          DrUnlockTaskMemory( lpData );
         }
         else
            lpCell->m_CellData.m_DataType= SS_TYPE_EDIT;
      }

      SS_InvalidateCell( hWnd, lpSS, CellCol, CellRow );

      if ( fTurnEditModeOn )
      {
         UpdateWindow( hWnd );
         SS_CellEditModeOn( hWnd, lpSS, 0, 0, 0L );
      }

      SS_UnlockCellItem( lpSS, CellCol, CellRow );
   }

   return( TRUE );
}

zBOOL
SS_SetColLock( HWND          hWnd,
               LPSPREADSHEET lpSS,
               SS_COORD      ssCol,
               zBOOL         bLock )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SS_SetColLock" );
   LPSS_COL      lpCol;
   LPSS_CELL     lpCell;
   SS_COORD      ss;

   for ( ss = 1; ss < lpSS->Row.AllocCnt; ss++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ss )) != 0 )
         lpCell->m_CellLocked = SS_LOCKED_DEF;
   }

   if ( (lpCol = SS_AllocLockCol( lpSS, ssCol )) != 0 )
   {
      lpCol->chColLocked = bLock;
      SS_UnlockColItem( lpSS, ssCol );

      if ( lpSS->LockColor.ForegroundId || lpSS->LockColor.BackgroundId )
         SS_InvalidateCol( hWnd, lpSS, ssCol );
   }

   return( TRUE );
}

void
SS_AdjustCellCoords( HWND          hWnd,
                     LPSS_COORD    CellCol,
                     LPSS_COORD    CellRow )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_AdjustCellCoords" );
   LPSPREADSHEET lpSS;

   lpSS = SS_Lock( hWnd );
   SSx_AdjustCellCoords( lpSS, CellCol, CellRow );
// DrUnlockTaskMemory( lpSS );
}

void
SS_AdjustCellRangeCoords( LPSPREADSHEET lpSS, LPSS_COORD lpCellCol,
                          LPSS_COORD lpCellRow, LPSS_COORD lpCellCol2,
                          LPSS_COORD lpCellRow2 )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_AdjustCellRangeCoords" );
   if ( lpCellRow )
   {
      if ( *lpCellRow == -1 )
      {
         if ( lpCellRow2 )
            *lpCellRow2 = -1;
      }
      else
      {
         if ( *lpCellRow < 0 )
            *lpCellRow += -SS_HEADER;

         *lpCellRow = mMin( *lpCellRow, SS_GetRowsMax( lpSS ) - 1 );
         if ( lpCellRow2 )
         {
            if ( *lpCellRow2 != -1 )
            {
               if ( *lpCellRow2 < 0 )
                  *lpCellRow2 += -SS_HEADER;

               *lpCellRow2 = mMin( *lpCellRow2, SS_GetRowsMax( lpSS ) - 1 );
            }
            else
               *lpCellRow = -1;
         }
      }
   }

   if ( lpCellCol )
   {
      if ( *lpCellCol == -1 )
      {
         if ( lpCellCol2 )
            *lpCellCol2 = -1;
      }
      else
      {
         if ( *lpCellCol < 0 )
            *lpCellCol += -SS_HEADER;

         *lpCellCol = mMin( *lpCellCol, SS_GetColsMax( lpSS ) - 1 );

         if ( lpCellCol2 )
         {
            if ( *lpCellCol2 != -1 )
            {
               if ( *lpCellCol2 < 0 )
                  *lpCellCol2 += -SS_HEADER;

               *lpCellCol2 = mMin( *lpCellCol2, SS_GetColsMax( lpSS ) - 1 );
            }
            else
               *lpCellCol = -1;
         }
      }
   }
}

zBOOL
SS_IsCellVisible( HWND hWnd, LPSPREADSHEET lpSS,
                  SS_COORD CellCol, SS_COORD CellRow,
                  zSHORT nVisible )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SS_IsCellVisible" );
   zBOOL fRet = FALSE;

   if ( SS_IsRowVisible( hWnd, lpSS, CellRow, nVisible ) &&
        SS_IsColVisible( hWnd, lpSS, CellCol, nVisible ) )
   {
      fRet = TRUE;
   }

   return( fRet );
}

zPCHAR
SSx_GetData( LPSPREADSHEET  lpSS,
             LPSS_COL       lpCol,
             LPSS_ROW       lpRow,
             LPSS_CELL      lpCell,
             LPSS_CELLTYPE  lpCellType,
             LPSS_DATA      lpData,
             SS_COORD       ssCol,
             SS_COORD       ssRow,
             zBOOL          fValue )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSx_GetData" );
   SS_CELLTYPE    CellType;
   ZSS_Data       SSData;
   zPCHAR         hData = 0;
   LPSS_CELL      lpCellOrig;
   LPSS_COL       lpColOrig;
   zPCHAR         lpszBuffer;

   lpCellOrig = lpCell;
   lpColOrig = lpCol;

   if ( ssRow != SS_ALLROWS && ssCol != SS_ALLCOLS &&
        (lpCell || (lpCell = SSx_LockCellItem( lpSS, lpRow, ssCol, ssRow )) != 0) &&
        lpCell->m_CellData.m_DataType )
   {
      zmemcpy( &SSData, &lpCell->m_CellData, zsizeof( ZSS_Data ) );
   }
   else
   if ( ssRow != SS_ALLROWS && lpRow && lpRow->Data.m_DataType )
   {
      zmemcpy( &SSData, &lpRow->Data, zsizeof( ZSS_Data ) );
   }
   else
   if ( ssCol != SS_ALLCOLS &&
        (lpCol || (lpCol = SS_LockColItem( lpSS, ssCol )) != 0) &&
        lpCol->Data.m_DataType )
   {
      zmemcpy( &SSData, &lpCol->Data, zsizeof( ZSS_Data ) );
   }
   else
      zmemcpy( &SSData, &lpSS->DefaultData, zsizeof( ZSS_Data ) );

   if ( lpCellOrig == 0 && lpCell )
      SSx_UnlockCellItem( lpSS, lpRow, ssCol, ssRow );

   if ( lpColOrig == 0 && lpCol )
      SS_UnlockColItem( lpSS, ssCol );

   if ( lpData && SSData.m_DataType == SS_TYPE_FLOAT )
      zmemcpy( lpData, &SSData, zsizeof( ZSS_Data ) );
   else
   if ( SSData.m_DataType )
   {
      switch ( SSData.m_DataType )
      {
         case SS_TYPE_EDIT:
            hData = (zPCHAR) SSData.m_Data.hszData;
//          TraceLineI( "GetData lpRow ", (zLONG) lpRow );
//          TraceLineI( "          ssRow ", ssRow );
//          TraceLineI( "          ssCol ", ssCol );
//          TraceLineS( "=======> Data ", hData );
            break;

         case SS_TYPE_FLOAT:
//          lpszBuffer = (zPCHAR) DrLockTaskMemory( lpSS->hBuffer );
            lpszBuffer = (zPCHAR) lpSS->hBuffer;

            if ( lpCellType == 0 )
            {
               SS_RetrieveCellType( lpSS, &CellType, lpCell, ssCol, ssRow );
               lpCellType = &CellType;
            }

            SS_FloatFormat( lpSS, lpCellType, SSData.m_Data.dxValue,
                            lpszBuffer, fValue );
//          DrUnlockTaskMemory( lpszBuffer );
            hData = lpSS->hBuffer;
            break;

         case SS_TYPE_INTEGER:
//          lpszBuffer = (zPCHAR) DrLockTaskMemory( lpSS->hBuffer );
            lpszBuffer = (zPCHAR) lpSS->hBuffer;
            wsprintf( lpszBuffer, "%ld", SSData.m_Data.lValue );
//          DrUnlockTaskMemory( lpszBuffer );
            hData = lpSS->hBuffer;
            break;
      }
   }

   return( hData );
}

void
SSx_AdjustCellCoords( LPSPREADSHEET lpSS,
                      LPSS_COORD    lpCellCol,
                      LPSS_COORD    lpCellRow )
{
   TRACE_DEBUG( "<ZSS_Cell.CPP>", "SSx_AdjustCellCoords" );
   if ( lpCellRow && *lpCellRow != -1 )
   {
      if ( *lpCellRow < 0 )
         *lpCellRow += -SS_HEADER;
      else
         *lpCellRow += lpSS->Row.nHeaderCnt - 1;

      *lpCellRow = mMin( *lpCellRow, SS_GetRowsMax( lpSS ) - 1 );
   }

   if ( lpCellCol && *lpCellCol != -1 )
   {
      if ( *lpCellCol < 0 )
         *lpCellCol += -SS_HEADER;
      else
         *lpCellCol += lpSS->Col.nHeaderCnt - 1;

      *lpCellCol = mMin( *lpCellCol, SS_GetColsMax( lpSS ) - 1 );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_col.cpp

zBOOL OPERATION
SSGetColWidth( HWND      hWnd,
               SS_COORD  ssCol,
               zLONG     *pdxColWidth )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSGetColWidth" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   zBOOL         bRC = FALSE;

   SS_AdjustCellCoords( hWnd, &ssCol, 0 );

   if ( ssCol < SS_GetColsMax( lpSS ) )
   {
      SS_CalcPixelsToColWidth( lpSS, ssCol,
                               SS_GetColWidth( lpSS, ssCol ), pdxColWidth );
      bRC = TRUE;
   }

// DrUnlockTaskMemory( lpSS );
   return( bRC );
}

zBOOL OPERATION
SSSetColCellType( HWND hWnd, LPSPREADSHEET lpSS,
                  SS_COORD ssCol, LPSS_CELLTYPE CellType )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSSetColCellType" );
   zPCHAR         hData;
   LPSS_CELLTYPE CellTypeTemp;
   LPSS_COL       lpCol;
   LPSS_CELL      lpCell;
   zPCHAR         lpData;
   SS_COORD       ss;
   zBOOL          fSetData = FALSE;

   for ( ss = (lpSS->fVirtualMode ? lpSS->Virtual.VTop : 1);
         ss < lpSS->Row.AllocCnt; ss++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ss )) != 0 )
      {
         if ( lpCell->m_hCellType )
         {
            SS_DeAllocCellType( hWnd, lpSS, lpCell->m_hCellType );
            lpCell->m_hCellType = 0;
         }

         SS_UnlockCellItem( lpSS, ssCol, ss );
      }
   }

   if ( (lpCol = SS_AllocLockCol( lpSS, ssCol )) != 0 )
   {
      SS_DeAllocCellType( hWnd, lpSS, lpCol->hCellType );
      lpCol->hCellType = 0;

      if ( CellType )
      {
         if ( DrAllocTaskMemory( (zCOREMEM) &lpCol->hCellType,
                                 zsizeof( SS_CELLTYPE ), 1072 ) != 0 )
         {
            SS_UnlockColItem( lpSS, ssCol );
            return( FALSE );
         }

         if ( lpCol->Data.m_DataType == SS_TYPE_FLOAT &&
              CellType->chType != SS_TYPE_FLOAT )
         {
            fSetData = TRUE;
         }

         CellTypeTemp = (LPSS_CELLTYPE) DrLockTaskMemory( lpCol->hCellType );
         zmemcpy( CellTypeTemp, CellType, zsizeof( SS_CELLTYPE ) );
//       DrUnlockTaskMemory( CellTypeTemp );
      }
      else
      if ( lpCol->Data.m_DataType == SS_TYPE_FLOAT )
         fSetData = TRUE;

      if ( fSetData )
      {
         if ( (hData = SS_GetData( lpSS, CellType, ssCol, -1, FALSE )) != 0 )
         {
//          lpData = (zPCHAR) DrLockTaskMemory( hData );
            lpData = (zPCHAR) hData;
            SSSetData( hWnd, ssCol, -1, lpData );
//          DrUnlockTaskMemory( lpData );
         }
         else
            lpCol->Data.m_DataType = SS_TYPE_EDIT;
      }

      for ( ss = (lpSS->fVirtualMode ? lpSS->Virtual.VTop : 1);
            ss < lpSS->Row.AllocCnt; ss++ )
      {
         if ( (hData = SS_GetData( lpSS, CellType, ssCol, ss, FALSE )) != 0 )
         {
//          lpData = (zPCHAR) DrLockTaskMemory( hData );
            lpData = (zPCHAR) hData;
            SSSetData( hWnd, ssCol, ss, lpData );
//          DrUnlockTaskMemory( lpData );
         }
         else
         if ( (lpCell = SS_LockCellItem( lpSS, ssCol, ss )) != 0 )
         {
            lpCell->m_CellData.m_DataType= SS_TYPE_EDIT;
            SS_UnlockCellItem( lpSS, ssCol, ss );
         }
      }

      SS_InvalidateCol( hWnd, lpSS, ssCol );
      SS_UnlockColItem( lpSS, ssCol );
   }

   return( TRUE );
}

zBOOL
SS_SetColColor( HWND          hWnd,
                LPSPREADSHEET lpSS,
                SS_COORD      ssCol,
                COLORREF      Background,
                COLORREF      Foreground )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SS_SetColColor" );
   LPSS_COL      lpCol;
   LPSS_CELL     lpCell;
   SS_COORD      k;

   for ( k = 1; k < lpSS->Row.AllocCnt; k++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ssCol, k )) != 0 )
         SS_ClearColorItem( &lpCell->m_Color, Background, Foreground );
   }

   if ( (lpCol = SS_AllocLockCol( lpSS, ssCol )) != 0 )
   {
      if ( Background != RGBCOLOR_DEFAULT )
         lpCol->Color.BackgroundId = SS_AddColor( lpSS, Background );

      if ( Foreground != RGBCOLOR_DEFAULT )
         lpCol->Color.ForegroundId = SS_AddColor( lpSS, Foreground );

      SS_UnlockColItem( lpSS, ssCol );
   }

   SS_InvalidateCol( hWnd, lpSS, ssCol );
   return( TRUE );
}

zBOOL OPERATION
SSSetColFont( HWND          hWnd,
              LPSPREADSHEET lpSS,
              SS_COORD      ssCol,
              HFONT         hFont,
              zBOOL         DeleteFont,
              zBOOL         *lpfDeleteFontObject )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSSetColFont" );
   LPSS_COL      lpCol;
   LPSS_CELL     lpCell;
   SS_COORD      k;

   for ( k = 1; k < lpSS->Row.AllocCnt; k++ )
   {
      if ( (lpCell = SS_LockCellItem( lpSS, ssCol, k )) != 0 )
         lpCell->m_FontId = SS_FONT_NONE;
   }

   if ( ( lpCol = SS_AllocLockCol( lpSS, ssCol ) ) != 0 )
   {
      lpCol->FontId = SS_InitFont( hWnd, hFont, DeleteFont, TRUE,
                                   lpfDeleteFontObject );
      SS_UnlockColItem( lpSS, ssCol );

      if ( lpSS->fAllowCellOverflow )
         for ( k = lpSS->Row.nHeaderCnt; k < lpSS->Row.DataCnt; k++ )
            SS_OverflowAdjustNeighbor( hWnd, lpSS, ssCol, k );
   }

   SS_InvalidateCol( hWnd, lpSS, ssCol );
   return( TRUE );
}

zBOOL OPERATION
SSSetColWidth( HWND     hWnd,
               SS_COORD ssCol,
               zLONG    dxWidth )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SSSetColWidth" );
   LPSPREADSHEET lpSS = SS_Lock( hWnd );
   LPSS_COL      lpCol;
   SS_COORD      ss;
   zLONG         lColWidthX100;


   if ( dxWidth == -1 )
      lColWidthX100 = -1;
   else
      lColWidthX100 = (zLONG)(dxWidth * 100.0);

   if ( ssCol == -1 )
   {
      for ( ss = lpSS->Col.nHeaderCnt; ss < lpSS->Col.AllocCnt; ss++ )
      {
         if ( (lpCol = SS_LockColItem( lpSS, ss )) != 0 )
         {
            lpCol->lColWidthX100 = SS_WIDTH_DEFAULT;
            lpCol->nColWidthInPixels = SS_WIDTH_DEFAULT;
            SS_UnlockColItem( lpSS, ss );
         }
      }

      lpSS->Col.lCellSizeX100 = lColWidthX100;
      lpSS->Col.nCellSizeInPixels = SS_ColWidthToPixels( lpSS, dxWidth );
   }
   else
   {
      if ( ssCol >= SS_GetColsMax( lpSS ) )
      {
//       DrUnlockTaskMemory( lpSS );
         return( FALSE );
      }

      SS_AdjustCellCoords( hWnd, &ssCol, 0 );

      if ( (lpCol = SS_AllocLockCol( lpSS, ssCol )) != 0 )
      {
         if ( lpCol->lColWidthX100 == lColWidthX100 )
         {
            SS_UnlockColItem( lpSS, ssCol );
//          DrUnlockTaskMemory( lpSS );
            return( TRUE );
         }

         lpCol->lColWidthX100 = lColWidthX100;

         if ( lpCol->lColWidthX100 == -1 )
            lpCol->nColWidthInPixels = SS_WIDTH_DEFAULT;
         else
            lpCol->nColWidthInPixels = SS_ColWidthToPixels( lpSS, dxWidth );

         SS_UnlockColItem( lpSS, ssCol );
      }

      if ( lpSS->fAllowCellOverflow )
      {
         for ( ss = lpSS->Row.nHeaderCnt; ss < lpSS->Row.DataCnt; ss++ )
            SS_OverflowAdjustNeighbor( hWnd, lpSS, ssCol, ss );
      }
   }

   SS_CellEditModeOff( hWnd, lpSS, 0 );
   if ( SS_AutoSize( hWnd, FALSE ) == FALSE )
   {
      SS_SetHScrollBar( hWnd, lpSS );

      if ( ssCol == -1 )
         SS_InvalidateScrollArea( hWnd, lpSS, SS_SCROLLAREA_ALL );
      else
      if ( SS_IsColVisible( hWnd, lpSS, ssCol, SS_VISIBLE_PARTIAL ) )
         SS_InvalidateColRange( hWnd, lpSS, ssCol, -1 );
   }

   if ( ssCol > 0 && dxWidth == 0.0 && lpSS->Col.ssCurrAt == ssCol )
   {
      SS_ScrollLineLR( hWnd, lpSS, SS_F_SCROLL_RIGHT | SS_F_MOVEACTIVECELL );
      if ( lpSS->Col.ssCurrAt == ssCol )
         SS_ScrollLineUL( hWnd, lpSS, SS_F_SCROLL_LEFT | SS_F_MOVEACTIVECELL );
   }

// DrUnlockTaskMemory( lpSS );
   return( TRUE );
}

void
SS_AdjustDataColCnt( LPSPREADSHEET lpSS,
                     SS_COORD ssCol, SS_COORD ssCol2 )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SS_AdjustDataColCnt" );
   LPSS_CELL lpCell;
   SS_COORD  Top;
   SS_COORD  k;
   SS_COORD  j;
   zBOOL     fHasData = FALSE;

   if ( ssCol == SS_ALLCOLS || ( ssCol < lpSS->Col.DataCnt &&
        lpSS->Col.DataCnt <= ssCol2 + 1 ) )
   {
      if ( lpSS->fVirtualMode )
         Top = lpSS->Virtual.VTop;
      else
         Top = lpSS->Row.nHeaderCnt;

      for ( k = lpSS->Col.DataCnt - 1; k >= lpSS->Col.nHeaderCnt && !fHasData; k-- )
      {
         for ( j = Top; j < lpSS->Row.DataCnt && !fHasData; j++ )
         {
            if ( (lpCell = SS_LockCellItem( lpSS, k, j )) != 0 )
            {
               if ( lpCell->m_CellData.m_DataType )
                  fHasData = TRUE;

               SS_UnlockCellItem( lpSS, k, j );
            }
         }

         if ( fHasData == FALSE )
            lpSS->Col.DataCnt--;
      }
   }
}

zBOOL
SS_IsColVisible( HWND hWnd, LPSPREADSHEET lpSS,
                 SS_COORD ssCol, zSHORT nVisible )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SS_IsColVisible" );
   zBOOL fRet = FALSE;

   switch ( nVisible )
   {
      case SS_VISIBLE_ALL:
         if ( ssCol < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen ||
              (ssCol >= lpSS->Col.UL && ssCol <=
                          SS_GetRightVisCell( hWnd, lpSS, lpSS->Col.UL )) )
         {
            fRet = TRUE;
         }

         break;

      case SS_VISIBLE_PARTIAL:
         if ( ssCol < lpSS->Col.nHeaderCnt + lpSS->Col.Frozen ||
              (ssCol >= lpSS->Col.UL && ssCol <= lpSS->Col.LR) )
         {
            fRet = TRUE;
         }

         break;
   }

   return( fRet );
}

void
SS_SetDataColCnt( LPSPREADSHEET lpSS, SS_COORD ssColCnt )
{
   TRACE_DEBUG( "<SS_COL.CPP>", "SS_SetDataColCnt" );
   if ( lpSS->Col.MaxBeforeSort == -1 || ssColCnt <= lpSS->Col.MaxBeforeSort )
      lpSS->Col.DataCnt = ssColCnt;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ss_curs.cpp

static BYTE CursorPointerXOR[] =
{
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x7B, 0xBC, 0x00,
   0x00, 0x7D, 0x7C, 0x00,
   0x00, 0x7E, 0xFC, 0x00,
   0x00, 0x7D, 0x7C, 0x00,
   0x00, 0x7B, 0xBC, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x07, 0xC0, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
};

static BYTE CursorPointerAND[] =
{
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xF8, 0x0F, 0xFF,
   0xFF, 0xF0, 0x0F, 0xFF,
   0xFF, 0xF0, 0x0F, 0xFF,
   0xFF, 0xF0, 0x0F, 0xFF,
   0xFF, 0xF0, 0x00, 0xFF,
   0xFF, 0x00, 0x00, 0xFF,
   0xFF, 0x00, 0x00, 0xFF,
   0xFF, 0x00, 0x00, 0xFF,
   0xFF, 0x00, 0x00, 0xFF,
   0xFF, 0x00, 0x00, 0xFF,
   0xFF, 0x00, 0x00, 0xFF,
   0xFF, 0x00, 0x01, 0xFF,
   0xFF, 0xF0, 0x0F, 0xFF,
   0xFF, 0xF0, 0x0F, 0xFF,
   0xFF, 0xF0, 0x0F, 0xFF,
   0xFF, 0xF0, 0x1F, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
};

static BYTE CursorResizehXOR[] =
{
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x06, 0xC0, 0x00,
   0x00, 0x04, 0x40, 0x00,
   0x00, 0x02, 0x80, 0x00,
   0x01, 0xFE, 0xFF, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x01, 0xFF, 0xFF, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x01, 0xFE, 0xFF, 0x00,
   0x00, 0x02, 0x80, 0x00,
   0x00, 0x04, 0x40, 0x00,
   0x00, 0x06, 0xC0, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
};

static BYTE CursorResizehAND[] =
{
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFE, 0x00, 0x00, 0xFF,
   0xFE, 0x00, 0x00, 0xFF,
   0xFE, 0x00, 0x00, 0xFF,
   0xFE, 0x00, 0x00, 0xFF,
   0xFE, 0x00, 0x00, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0XFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
};

static BYTE CursorResizevXOR[] =
{
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x35, 0x58, 0x00,
   0x00, 0x2D, 0x68, 0x00,
   0x00, 0x01, 0x00, 0x00,
   0x00, 0x2D, 0x68, 0x00,
   0x00, 0x35, 0x58, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x05, 0x40, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
};

static BYTE CursorResizevAND[] =
{
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xC0, 0x07, 0xFF,
   0xFF, 0xC0, 0x07, 0xFF,
   0xFF, 0xC0, 0x07, 0xFF,
   0xFF, 0xC0, 0x07, 0xFF,
   0xFF, 0xC0, 0x07, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xF8, 0x3F, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF,
};

HCURSOR
SS_CreateCursorPointer( HINSTANCE  hInstance )
{
   TRACE_DEBUG( "<SS_CURS.CPP>", "SS_CreateCursorPointer" );
   HCURSOR hCursor;

   hCursor = CreateCursor( hInstance, 15, 15, 32, 32,
                           CursorPointerAND, CursorPointerXOR );

   return( hCursor );
}

HCURSOR
SS_CreateCursorResizeh( HINSTANCE  hInstance )
{
   TRACE_DEBUG( "<SS_CURS.CPP>", "SS_CreateCursorResizeh" );
   HCURSOR hCursor;

   hCursor = CreateCursor( hInstance, 15, 15, 32, 32,
                           CursorResizehAND, CursorResizehXOR );

   return( hCursor );
}

HCURSOR
SS_CreateCursorResizev( HINSTANCE  hInstance )
{
   TRACE_DEBUG( "<SS_CURS.CPP>", "SS_CreateCursorResizev" );
   HCURSOR hCursor;

   hCursor = CreateCursor( hInstance, 15, 15, 32, 32,
                           CursorResizevAND, CursorResizevXOR );
   return( hCursor );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZSS_Data::ZSS_Data( )
{
   m_DataType = 0;
   m_Data.hszData = 0;
}

ZSS_Data::~ZSS_Data( )
{
   if ( m_Data.hszData )
   {
      if ( m_DataType == SS_TYPE_EDIT )
      {
      // m_Data.hszData = (zPCHAR) DrUnlockTaskMemory( m_Data.hszData );
      // DrFreeTaskMemory( m_Data.hszData );
      }

      m_Data.hszData = 0;
   }
}

ZSS_Cell::ZSS_Cell( )
{
   m_hCellType = 0;
}

ZSS_Cell::~ZSS_Cell( )
{
// if ( m_hCellType )
// {
//    DrFreeTaskMemory( m_hCellType );
//    m_hCellType = 0;
// }
}

COLORREF
SetTxColor( HDC hDC, COLORREF colorText )
{
// TraceLineX( "SetTextColor: ", colorText );
   return( SetTextColor( hDC, colorText ) );
// return( CLR_INVALID );
}

void
TraceDebug( zPCHAR pchSrcFileName,
            zPCHAR pchFuncName )
{
// TraceLineS( pchSrcFileName, pchFuncName );
   ZListFile *clf = &clfHead;
   zSHORT nFileRC;
   zSHORT nFuncRC;
   while ( clf->m_pNext )
   {
      nFileRC = zstrcmp( pchSrcFileName, clf->m_pNext->m_pchFileName );
      nFuncRC = zstrcmp( pchFuncName, clf->m_pNext->m_pchFunctionName );
      if ( nFileRC == 0 && nFuncRC == 0 )
      {
         clf->m_pNext->m_lCnt++;
         return;
      }

      if ( nFileRC > 0 || (nFileRC == 0 && nFuncRC > 0) )
      {
         clf = clf->m_pNext;
      }
      else
      {
         break;
      }
   }

   // add a new one in (alphabetical order)
   ZListFile *clfCurr = new ZListFile( pchSrcFileName, pchFuncName );
   clfCurr->m_pNext = clf->m_pNext;
   clf->m_pNext = clfCurr;
}

void OPERATION
SS_TraceDone( )
{
   ZListFile *clf = &clfHead;
   zCHAR szBuffer[ 256 ];

   while ( clf->m_pNext )
   {
      if ( zstrcmp( clf->m_pchFileName, clf->m_pNext->m_pchFileName ) != 0 )
      {
         TraceLineS( "Source File Name ========> ",
                     clf->m_pNext->m_pchFileName );
      }

      strcpy_s( szBuffer, zsizeof( szBuffer ), " Function: " );
      strcat_s( szBuffer, zsizeof( szBuffer ), clf->m_pNext->m_pchFunctionName );
      strcat_s( szBuffer, zsizeof( szBuffer ), " access count: " );
      TraceLineI( szBuffer, clf->m_pNext->m_lCnt );
      clf = clf->m_pNext;
   }
}

#if 0
SS_COORD
mMin( SS_COORD a, SS_COORD b )
{
   return( a < b ? a : b );
}

SS_COORD
mMax( SS_COORD a, SS_COORD b )
{
   return( a > b ? a : b );
}
#endif

// a simpleton single-link list for file and function names
ZListFile::ZListFile( zCHAR const *pchFileName,
                      zCHAR const *pchFunctionName )
{
   m_nLth = strlen( pchFunctionName );
   m_pchFunctionName = new char[ m_nLth + 1 ];
   strcpy_s( m_pchFunctionName, m_nLth + 1, pchFunctionName );
   m_nLth = strlen( pchFileName );
   m_pchFileName = new char[ m_nLth + 1 ];
   strcpy_s( m_pchFileName, m_nLth + 1, pchFileName );
   m_pNext = 0;
   m_lCnt = 1;
}

ZListFile::~ZListFile( )
{
   delete [] m_pchFileName;
   delete [] m_pchFunctionName;
   if ( m_pNext )
      delete( m_pNext );
}

#ifdef  DKS_TEST
zBOOL
fnListVBuffer( LPSPREADSHEET lpSS, zBOOL bCheck )
{
   zPCHAR    hRowItem;
   LPSS_ROW  lpRowItem;

   if ( lpSS->Virtual.VStartIdx <= 0 )
      return( FALSE );

   if ( bCheck == FALSE )
   {
      TraceLineI( "Virtual Buffer Start Idx ", lpSS->Virtual.VStartIdx );
      TraceLineI( "               Start ssRow ", lpSS->Virtual.VStartRow );
      TraceLineI( "             HiWater Idx ", lpSS->Virtual.VHighWaterIdx );
   }

   zLONG k = lpSS->Virtual.VStartIdx;
   zLONG j = lpSS->Virtual.VStartRow;

   // guarantee check index (dwItemCnt) agrees with expected row ... also
   // ensure sequential entries
   while ( TRUE )
   {
      if ( k > lpSS->Virtual.VHighWaterIdx )
      {
         k = 0;
      }
      else
      if ( k )
      {
         if ( bCheck )
         {
            if ( lpSS->Virtual.VRows[ k ].hItems &&
                 j != lpSS->Virtual.VRows[ k ].dwItemCnt )
            {
               TraceLineI( "Check found Virtual Buffer error for row = ", j );
               fnListVBuffer( lpSS, FALSE );
               return( TRUE );
            }
         }
         else
         {
            TraceLineI( "Virtual VRows idx = ", k );
            TraceLineI( "              row = ", j );
            hRowItem = lpSS->Virtual.VRows[ k ].hItems;
            TraceLineI( "  check row hItem = ", (zLONG) hRowItem );
            if ( hRowItem )
            {
//             lpRowItem = (LPSS_ROW) DrLockTaskMemory( hRowItem );
               lpRowItem = (LPSS_ROW) hRowItem;
               TraceLineI( "           fDataSet ",
                           (lpRowItem->fDataSet) ? 1L : 0L );
//             DrUnlockTaskMemory( lpRowItem );
            }

            TraceLineI( "___________ value = ",
                        lpSS->Virtual.VRows[ k ].dwItemCnt );
         }

         j++;
      }

      k++;
      if ( k == lpSS->Virtual.VStartIdx )
         break;
   }

   return( FALSE );
}
#endif

BOOL
fnGrayString( HDC hDC, HBRUSH hBrush, GRAYSTRINGPROC gsProc,
              LPARAM lParam, int cch, int x, int y, int cx, int cy )
{
   return( GrayString( hDC, hBrush, gsProc,
                       lParam, cch, x, y, cx, cy ) );
}

HFONT
SelectFont( HDC hDC, HFONT hFont )
{
// zCHAR szMsg[ 128 ];
//
   HFONT hFontOld = (HFONT) SelectObject( hDC, hFont );
// sprintf_s( szMsg, zsizeof( szMsg ), "SelectFont new: %d old: %d",
//           (zLONG) hFont, (zLONG) hFontOld );
// TraceLineS( szMsg, "" );
   return( hFontOld );
}

HPEN
SelectPen( HDC hDC, HPEN hPen )
{
// zCHAR szMsg[ 128 ];
//
   HPEN hPenOld = (HPEN) SelectObject( hDC, hPen );
// sprintf_s( szMsg, zsizeof( szMsg ), "SelectPen new: %d old: %d",
//           (zLONG) hPen, (zLONG) hPenOld );
// TraceLineS( szMsg, "" );
   return( hPenOld );
}

HBRUSH
SelectBrush( HDC hDC, HBRUSH hBrush )
{
// zCHAR szMsg[ 128 ];
//
   HBRUSH hBrushOld = (HBRUSH) SelectObject( hDC, hBrush );
// sprintf_s( szMsg, zsizeof( szMsg ), "SelectBrush new: %d old: %d",
//           (zLONG) hBrush, (zLONG) hBrushOld );
// TraceLineS( szMsg, "" );
   return( hBrushOld );
}

HBITMAP
SelectBitmap( HDC hDC, HBITMAP hBitmap )
{
// zCHAR szMsg[ 128 ];
//
   HBITMAP hBitmapOld = (HBITMAP) SelectObject( hDC, hBitmap );
// sprintf_s( szMsg, zsizeof( szMsg ), "SelectBitmap new: %d old: %d",
//           (zLONG) hBitmap, (zLONG) hBitmapOld );
// TraceLineS( szMsg, "" );
   return( hBitmapOld );
}

void
DeleteFont( HFONT hFont )
{
   if ( hFont )
   {
//    TraceLineI( "Deleting Font: ", (zLONG) hFont );
      DeleteObject( hFont );
   }
}

void
DeletePen( HPEN hPen )
{
   if ( hPen )
   {
//    TraceLineI( "Deleting Pen: ", (zLONG) hPen );
      DeleteObject( hPen );
   }
}

void
DeleteBrush( HBRUSH hBrush )
{
   if ( hBrush )
   {
//    TraceLineI( "Deleting Brush: ", (zLONG) hBrush );
      DeleteObject( hBrush );
   }
}

void
DeleteBitmap( HBITMAP hBitmap )
{
   if ( hBitmap )
   {
//    TraceLineI( "Deleting Bitmap: ", (zLONG) hBitmap );
      DeleteObject( hBitmap );
   }
}

HFONT
GetStockFont( int iFont )
{
   HFONT hFont = (HFONT) GetStockObject( iFont );
// TraceLineI( "GetStockFont: ", (zLONG) hFont );
   return( hFont );
}

HPEN
GetStockPen( int iPen )
{
   HPEN hPen = (HPEN) GetStockObject( iPen );
// TraceLineI( "GetStockPen: ", (zLONG) hPen );
   return( hPen );
}

HBRUSH
GetStockBrush( int iBrush )
{
   HBRUSH hBrush = (HBRUSH) GetStockObject( iBrush );
// TraceLineI( "GetStockBrush: ", (zLONG) hBrush );
   return( hBrush );
}

HPEN
kzCreatePen( int nPenStyle,     // pen style
             int nWidth,        // pen width
             COLORREF color )   // pen color
{
   HPEN hPen = CreatePen( nPenStyle, nWidth, color );
// TraceLineI( "kzCreatePen: ", (zLONG) hPen );
   return( hPen );
}

HBITMAP
kzCreateBitmap( int nWidth,              // bitmap width, in pixels
                int nHeight,             // bitmap height, in pixels
                UINT cPlanes,            // number of color planes used by device
                UINT cBitsPerPel,        // number of bits required to identify a color
                CONST void *lpvBits )    // pointer to array containing color data
{
   HBITMAP hBitmap = CreateBitmap( nWidth, nHeight, cPlanes,
                                   cBitsPerPel, lpvBits );
// TraceLineI( "kzCreateBitmap: ", (zLONG) hBitmap );
   return( hBitmap );
}

HBITMAP
kzCreateBitmapIndirect( BITMAP *pBitmap )
{
   HBITMAP hBitmap = CreateBitmapIndirect( pBitmap );
// TraceLineI( "kzCreateBitmapIndirect: ", (zLONG) hBitmap );
   return( hBitmap );
}

HBITMAP
kzCreateCompatibleBitmap( HDC hdc,        // handle to device context
                          int nWidth,     // width of bitmap, in pixels
                          int nHeight )   // height of bitmap, in pixels
{
   HBITMAP hBitmap = CreateCompatibleBitmap( hdc, nWidth, nHeight );
// TraceLineI( "kzCreateCompatibleBitmap: ", (zLONG) hBitmap );
   return( hBitmap );
}

HBRUSH
kzCreateSolidBrush( COLORREF color )
{
   HBRUSH hBrush = CreateSolidBrush( color );
// TraceLineI( "kzCreateSolidBrush: ", (zLONG) hBrush );
   return( hBrush );
}

HBRUSH
kzCreatePatternBrush( HBITMAP hbmp )
{
   HBRUSH hBrush = CreatePatternBrush( hbmp );
// TraceLineI( "kzCreatePatternBrush: ", (zLONG) hBrush );
   return( hBrush );
}

#endif
