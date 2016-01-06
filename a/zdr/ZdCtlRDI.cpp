/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Remote Server helper classes
// FILE:         zdctlrdi.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon Remote Server data items.
//
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.01.28    DKS    WEB
//    Added debugging code.
//
// 2001.09.18    DKS    WEB
//    Back out Delta Data implementation.
//
// 2001.09.12    DKS    WEB
//    Set Deleted flag on deleted list items in delta data.
//
// 2001.09.01    DKS    WEB
//    Handle case of single add/delete in lists.
//
// 2001.05.29    DKS    WEB
//    Implement merge of list data.
//
// 2001.05.13   BL   RemoteServer
//    Modified SelectString for call FindDataRowByString, so than the
//    compare in done in voll length.
//
// 2001.05.12   BL   RemoteServer
//    Modified ZRmtDataRow::ZRmtDataRow for init m_bIsSelected = FALSE
//

// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlGbl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

#ifdef zREMOTE_SERVER

IMPLEMENT_DYNAMIC( ZRmtDataItem, CObject )

IMPLEMENT_DYNAMIC( ZRmtDataRow, CObject )

IMPLEMENT_DYNAMIC( ZRmtDataSet, CWnd )

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZRmtDataItem::ZRmtDataItem( )
{
}

ZRmtDataItem::ZRmtDataItem( CString csItemValue, zLONG lItemPos )
{
   m_csItemValue = csItemValue;
   m_lItemPos    = lItemPos;
}

ZRmtDataItem::ZRmtDataItem( const ZRmtDataItem& rdi ) :
              m_csItemValue( rdi.m_csItemValue )
{
   m_lItemPos = rdi.m_lItemPos;
}

ZRmtDataItem::~ZRmtDataItem( )
{
}

zSHORT
ZRmtDataItem::Compare( const ZRmtDataItem& rdi ) const
{
   return( m_csItemValue.Compare( rdi.m_csItemValue ) );
}

void
ZRmtDataItem::CopyData( const ZRmtDataItem& rdi )
{
   m_csItemValue = rdi.m_csItemValue;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZRmtDataRow::ZRmtDataRow( zCPCHAR cpcValue, zLONG lKey, zLONG lPos )
{
   m_ulState = 0;
   m_lKey = lKey;
   m_lOrderNbr = lPos;
   AddString( cpcValue, 0 );
   m_bIsSelected = FALSE;
}

ZRmtDataRow::ZRmtDataRow( const ZRmtDataRow& rdr )
{
   m_ulState = 0;
   m_lKey = rdr.m_lKey;
   m_lKeyOrig = rdr.m_lKeyOrig;
   m_bIsSelected = rdr.m_bIsSelected;
   m_dwItemData = rdr.m_dwItemData;
   m_lOrderNbr = rdr.m_lOrderNbr;

   ZRmtDataItem *prdi;
   POSITION pos = rdr.m_Cols.GetHeadPosition( );

   while ( pos )
   {
      prdi = (ZRmtDataItem *) rdr.m_Cols.GetNext( pos );
      AddString( prdi->GetString( ), prdi->GetItemPos( ) );
   }
}

zSHORT
ZRmtDataRow::Compare( const ZRmtDataRow& rdr ) const
{
   ZRmtDataItem *prdi1;
   ZRmtDataItem *prdi2;
   POSITION pos1 = m_Cols.GetHeadPosition( );
   POSITION pos2 = rdr.m_Cols.GetHeadPosition( );
   zSHORT   nRC;

   while ( pos1 && pos2 )
   {
      prdi1 = (ZRmtDataItem *) m_Cols.GetNext( pos1 );
      prdi2 = (ZRmtDataItem *) rdr.m_Cols.GetNext( pos2 );
      nRC = prdi1->Compare( *prdi2 );
      if ( nRC )
         return( nRC );
   }

   if ( pos1 || pos2 )
   {
      if ( pos1 )
         return( 1 );
      else
         return( -1 );
   }

   return( 0 );
}

void
ZRmtDataRow::CopyData( const ZRmtDataRow& rdr )
{
   ZRmtDataItem *prdi1;
   ZRmtDataItem *prdi2;
   POSITION pos1 = m_Cols.GetHeadPosition( );
   POSITION pos2 = rdr.m_Cols.GetHeadPosition( );

   while ( pos1 && pos2 )
   {
      prdi1 = (ZRmtDataItem *) m_Cols.GetNext( pos1 );
      prdi2 = (ZRmtDataItem *) rdr.m_Cols.GetNext( pos2 );
      prdi1->CopyData( *prdi2 );
   }

   if ( pos1 || pos2 )
      TraceLineS( "ZRmtDataRow::CopyData: ", "Unexpected extra columns" );
}

ZRmtDataRow::~ZRmtDataRow( )
{
   ResetContent( );
}

void
ZRmtDataRow::ResetContent( )
{
// Disable Delta Data implementation.
   m_dwItemData = 0;
   m_bIsSelected = FALSE;
// Disable Delta Data implementation.

   ZRmtDataItem *pItem;

   while ( m_Cols.IsEmpty( ) == FALSE )
   {
      pItem = (ZRmtDataItem *) m_Cols.RemoveHead( );
      delete( pItem );
   }
}

void
ZRmtDataRow::AddString( zCPCHAR cpcStr, zLONG lPos )
{
   ZRmtDataItem *pItem = new ZRmtDataItem( cpcStr, lPos );
   m_Cols.AddTail( pItem );
}

CString
ZRmtDataRow::GetString( zLONG lPos )
{
   CString csHelp;

   if ( lPos < 0 )
   {
      return( csHelp );
   }

   POSITION pos = m_Cols.FindIndex( lPos );
   if ( pos )
   {
      csHelp = ((ZRmtDataItem *) (m_Cols.GetAt( pos )))->GetString( );
   }

   return( csHelp );
}

zLONG
ZRmtDataRow::UpdateColumn( zLONG lPos, CString csText )
{
   CString csHelp;
   zLONG   lRC = 0;

   if ( lPos < 0 )
   {
      return( LB_ERR );
   }

   POSITION pos = m_Cols.FindIndex( lPos );
   if ( pos )
   {
      ((ZRmtDataItem *) (m_Cols.GetAt( pos )))->SetString( csText );
   }
   else
      lRC = LB_ERR;

   return( lRC );
}

CString
ZRmtDataRow::GetRowDataText( zBOOL bUseSeparator ) const
{
   CString  csHelper;
   CString  csTemp;
   POSITION pos = m_Cols.GetHeadPosition( );

   while ( pos )
   {
      csHelper += ((ZRmtDataItem *) m_Cols.GetNext( pos ))->GetString( );

      // Write a delimiter anytime even if it's the last column.
      if ( bUseSeparator )
         csHelper += zREMOTE_DATA_DELIMITER;
   }

#if 0
   zPCHAR  pchHelper = new char[ (zstrlen( csHelper ) + 1) ];
   zPCHAR  pchBuffer = new char[ (zstrlen( csHelper ) + 1) + 64 ];
   zPCHAR  pchPrev = pchHelper;
   zPCHAR  pchDelim;
   zLONG   lLth = 0;

   strcpy_s( pchHelper, zstrlen( csHelper ) + 1, csHelper );
   strcpy_s( pchBuffer, zstrlen( csHelper ) + 1, csHelper );
   pchDelim = zstrchr( pchHelper, zREMOTE_DATA_DELIMITER );
   while ( pchDelim )
   {
      lLth += pchDelim - pchPrev;
      if ( *(pchDelim + 1) == zREMOTE_DATA_DELIMITER )
      {
         pchBuffer[ lLth++ ] = zREMOTE_DATA_DELIMITER;
         pchBuffer[ lLth++ ] = ' ';
         pchBuffer[ lLth++ ] = zREMOTE_DATA_DELIMITER;
         pchDelim++;
         pchDelim++;
         pchPrev = pchDelim;
         strcpy_s( pchBuffer + lLth, zstrlen( csHelper ) + 1 - nLth, pchDelim );
      }
      else
         pchDelim++;

      pchDelim = zstrchr( pchDelim, zREMOTE_DATA_DELIMITER );
   }

   csHelper = pchBuffer;
   delete [] pchBuffer;
   delete [] pchHelper;
#endif

   return( csHelper );
}

//
// look for delimiters and set column text
//
zLONG
ZRmtDataRow::SetRowDataText( CString csRowText )
{
   zLONG   lIterations = 0;
   CString csText = csRowText;
   zPCHAR  pchText = 0;
   zPCHAR  pchDelimiter = 0;

   csText += " ";
   pchText = csText.GetBuffer( csText.GetLength( ) );

   pchDelimiter = zstrchr( pchText, zREMOTE_DATA_DELIMITER );

   while ( pchDelimiter )
   {
      *pchDelimiter = 0;
      UpdateColumn( lIterations, pchText );
      pchDelimiter++;
      pchText = pchDelimiter;
      pchDelimiter = zstrchr( pchText, zREMOTE_DATA_DELIMITER );
      lIterations++;
   }

   return( lIterations );
}

#define zRMT_REMOVE_STATE   -1
#define zRMT_SET_STATE       0
#define zRMT_ADD_STATE       1

void
ZRmtDataRow::SetState( zULONG ulState, zSHORT nCmd )
{
   if ( nCmd == zRMT_SET_STATE )
      m_ulState = m_ulState;
   else
   if ( nCmd == zRMT_ADD_STATE )
      m_ulState |= ulState;
   else
// if ( nCmd == zRMT_REMOVE_STATE )
      m_ulState &= ~ulState;
}

void
ZRmtDataRow::SetSel( zBOOL bSel )
{
   m_bIsSelected = bSel;
#ifdef DEBUG_ALL
   CString cs = GetRowDataText( );
   TraceLineS( "ZRmtDataRow::SetSel: ", cs );
#endif
}

zBOOL
ZRmtDataRow::GetSel( ) const
{
   return( m_bIsSelected );
}

void
ZRmtDataRow::SetItemData( DWORD dwItemData )
{
   m_dwItemData = dwItemData;
}

void
ZRmtDataRow::SetKey( zLONG lKey )
{
   m_lKey = lKey;
}

zLONG
ZRmtDataRow::GetKey( ) const
{
   return( m_lKey );
}

void
ZRmtDataRow::SetKeyOrig( zLONG lKey )
{
   m_lKeyOrig = lKey;
}

zLONG
ZRmtDataRow::GetKeyOrig( ) const
{
   return( m_lKeyOrig );
}

DWORD
ZRmtDataRow::GetItemData( ) const
{
   return( m_dwItemData );
}

zULONG
ZRmtDataRow::GetState( ) const
{
   return( m_ulState );
}

void
ZRmtDataRow::SetOrder( zLONG lOrderNbr )
{
   m_lOrderNbr = lOrderNbr;
}

zLONG
ZRmtDataRow::GetOrder( ) const
{
   return( m_lOrderNbr );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

extern "C"
{
// Comparison function required to order objects added to the list.  In
// this case it just compares the keys embedded in the ZRmtDataRow type.
zSHORT
fnCompareRowNodes( const ZRmtDataRow *pRow1,
                   const ZRmtDataRow *pRow2 )
{
   zLONG lKey1 = pRow1->GetKey( );
   zLONG lKey2 = pRow2->GetKey( );

   return( (lKey1 > lKey2) ? 1 : (lKey1 < lKey2) ? -1 : 0 );
}

// Comparison function required to seek for an object in the list. In
// this case it just compares the keys embedded in the ZRmtDataRow type.
zSHORT
fnSeekRowNode( const zLONG       lKey1,
               const ZRmtDataRow *pRow )
{
   zLONG lKey2 = pRow->GetKey( );

   return( (lKey1 > lKey2) ? 1 : (lKey1 < lKey2) ? -1 : 0 );
}

// Function to delete the row data.
void
fnDeleteRowObject( ZRmtDataRow *pRow,
                   WPARAM wParam,
                   LPARAM lParam )
{
   mDeleteInit( pRow );
}

void *
fnCopyRowObject( ZRmtDataRow *pRow )
{
   return( new ZRmtDataRow( *pRow ) );
}

} // end: extern "C"

ZRmtDataSet::ZRmtDataSet( )
{
   m_lTopIndex = 0;
   m_pRowsOrig = 0;
   m_pRows = 0;
}

ZRmtDataSet::~ZRmtDataSet( )
{
   if ( m_pRows )
   {
      m_pRows->ResetContent( );
   }

   if ( m_pRowsOrig )
   {
      m_pRowsOrig->ResetContent( );
      mDeleteInit( m_pRowsOrig );
   }
}

void
ZRmtDataSet::ResetContent( )
{
   if ( m_pRows )
   {
      m_pRows->ResetContent( );
      mDeleteInit( m_pRows );
   }

   m_pRows = new ZMultiIdxSet( (fnCOMPARE) fnCompareRowNodes,
                               (fnCOMPARE) fnSeekRowNode, 100,
                               (fnVISIT_INORDER) fnDeleteRowObject,
                               (fnCOPY) fnCopyRowObject );
}

zLONG
ZRmtDataSet::SetSel( zLONG lPos, zBOOL bSet )
{
   zLONG lRC = lPos;

   if ( lPos < 0 )
   {
      ResetSelectionSet( bSet );
      lRC = m_pRows->GetCount( );
   }
   else
   {
      ZRmtDataRow *pRow = (ZRmtDataRow *) m_pRows->GetAt( lPos + 1 );
      if ( pRow )
         pRow->SetSel( bSet );
      else
         lRC = LB_ERR;
   }

   return( lRC );
}

zLONG
ZRmtDataSet::SetCurSel( zLONG lPos )
{
   if ( lPos < 0 )
   {
      ResetSelectionSet( FALSE );
      return( m_pRows->GetCount( ) );
   }

   ZRmtDataRow *pRow = (ZRmtDataRow *) m_pRows->GetAt( lPos + 1 );
   if ( pRow == 0 )
      return( LB_ERR );

   POSITION pos = GetFirstDataRowPosition( );
   while ( pos )
   {
      // reset selection
      pRow = GetNextDataRow( pos );
      pRow->SetSel( FALSE );
   }

   return( SetSel( lPos, TRUE ) );
}

zLONG
ZRmtDataSet::GetCurSel( )
{
   zLONG lReturn = LB_ERR; // -1
   zBOOL bFound = FALSE;
   ZRmtDataRow *pRow = 0;

   // Find the first selected row.
   POSITION pos = GetFirstDataRowPosition( );
   while ( pos && bFound == FALSE )
   {
      pRow = GetNextDataRow( pos );
      bFound = pRow->GetSel( );
      lReturn++;
   }

   if ( bFound )
      return( lReturn );
   else
      return( LB_ERR );
}

zLONG
ZRmtDataSet::SetItemData( zLONG lIdx, DWORD dwItemData )
{
   if ( lIdx < 0 )
      return( LB_ERR );

   ZRmtDataRow *pRow = (ZRmtDataRow *) m_pRows->GetAt( lIdx + 1 );
   if ( pRow == 0 )
      return( LB_ERR );

   // Set Items data
   pRow->SetItemData( dwItemData );
   return( lIdx );
}

DWORD
ZRmtDataSet::GetItemData( zLONG lIdx )
{
   if ( lIdx < 0 )
      return( LB_ERR );

   ZRmtDataRow *pRow = (ZRmtDataRow *) m_pRows->GetAt( lIdx );
   if ( pRow == 0 )
      return( LB_ERR );

   DWORD dwItemData = pRow->GetItemData( );
   return( dwItemData );
}

zLONG
ZRmtDataSet::AddString( zCPCHAR cpcValue, zLONG lKey, zLONG lPos )
{
   zLONG lCnt = 0;
   ZRmtDataRow *pRow = new ZRmtDataRow( cpcValue, lKey, lPos );

// TraceLineX( cpcValue, (zLONG) pRow );
   TRY
   {
      m_pRows->AddTail( pRow );
      lCnt = m_pRows->GetCount( );
   }
   CATCH( CMemoryException, e )
   {
      lCnt = LB_ERRSPACE;
      if ( pRow )
         delete( pRow );
   }
   END_CATCH

   return( lCnt );
}

zLONG
ZRmtDataSet::InsertString( zCPCHAR cpcValue, zLONG lKey, zLONG lPos )
{
   zLONG lCnt = m_pRows->GetCount( );
   ZRmtDataRow *pRow = new ZRmtDataRow( cpcValue, lKey );

   TRY
   {
      if ( lPos == -1 || lCnt == 0 )
      {
         m_pRows->AddTail( pRow );
      }
      else
      {
         if ( lPos == 0 )
         {
            m_pRows->AddHead( pRow );
         }
         else
         {
           ZRmtDataRow *pRowNext = (ZRmtDataRow *) m_pRows->GetAt( lPos );
           m_pRows->InsertNode( pRow, pRowNext, 'B' );
         }
      }

      lCnt++;
   }
   CATCH( CMemoryException, e )
   {
      lCnt = LB_ERRSPACE;
      if ( pRow )
         delete( pRow );
   }
   END_CATCH

// DisplayListData( );
   return( lCnt );
}

zLONG
ZRmtDataSet::GetLBText( zLONG lIdx, LPTSTR lpszText )
{
   zLONG lRC = CB_ERR;

   if ( lIdx >= 0 )
   {
      ZRmtDataRow *pRow = (ZRmtDataRow *) m_pRows->GetAt( lIdx + 1 );
      if ( pRow )
      {
         strcpy_s( lpszText, ???, pRow->GetString( ));
         lRC = pRow->GetString( ).GetLength( );
      }
   }

   return( lRC );
}

void
ZRmtDataSet::GetLBText( zLONG lIdx, CString& rString )
{
   ZRmtDataRow *pRow = 0;

   if ( lIdx < 0 )
   {
      return;
   }

   POSITION pos = m_pRows->FindIndex( lIdx );
   if ( pos )
   {
      pRow = GetNextDataRow( pos );
      if ( pRow )
      {
         rString = pRow->GetString( );
      }
   }
}

zLONG
ZRmtDataSet::GetCount( )
{
   return m_pRows->GetCount();
}

POSITION
ZRmtDataSet::GetFirstDataRowPosition( )
{
   return( m_pRows->GetHeadPosition( ) );
}

ZRmtDataRow *
ZRmtDataSet::GetNextDataRow( POSITION& pos )
{
   return( (ZRmtDataRow *) m_pRows->GetNext( pos ) );
}

zLONG
ZRmtDataSet::SetTopIndex( zLONG lIdx )
{
   zLONG k = lIdx;

   if ( k < 0 )
   {
      k = 0;
   }

   POSITION pos = m_pRows->FindIndex( k );
   if ( pos == 0 )
      return( LB_ERR );

   m_lTopIndex = k;
   return( m_lTopIndex );
}

zLONG
ZRmtDataSet::GetTopIndex( )
{
   POSITION pos = m_pRows->FindIndex( m_lTopIndex );
   if ( pos == 0 )
      return( LB_ERR );

   return( m_lTopIndex );
}

//
// Find a row using a given DWORD value
//
ZRmtDataRow *
ZRmtDataSet::FindDataRowByDataItem( DWORD dw )
{
   POSITION pos =  GetFirstDataRowPosition( );
   ZRmtDataRow *pRow = 0;

   while ( pos )
   {
      pRow = GetNextDataRow( pos );
      if ( (pRow->GetItemData( )) == dw )
         return( pRow );
   }

   return( 0 );
}

zLONG
ZRmtDataSet::SelectString( zLONG lStartAfter, zCPCHAR lpszString )
{
   ZRmtDataRow *pRow = 0;
   POSITION pos = 0;
   zLONG lIdx = LB_ERR;

   pRow = FindDataRowByString( lpszString, &lIdx,  lStartAfter, TRUE );
   if ( pRow )
   {
      // Clear selection flag for all rows.
      ResetSelectionSet( FALSE );

      // Select current row and set top index.
      pRow->SetSel( TRUE );
      SetTopIndex( lIdx );
   }
   else
   {
      lIdx = LB_ERR;
   }

   return( lIdx );
}

ZRmtDataRow *
ZRmtDataSet::FindDataRowByString( zCPCHAR cpcString,
                                  zLONG   *plIdx,
                                  zLONG   lOffset,
                                  zBOOL   bExact )
{
   ZRmtDataRow *pRow = 0;
   POSITION pos = 0;
   zBOOL bFound = FALSE;
   CString csSrc;
   CString csToFind = cpcString;
   zLONG lIdx = -1;
   zLONG lStrLength = csToFind.GetLength( );

   if ( lOffset <= 0 )
   {
      // Start from the beginning
      pos = GetFirstDataRowPosition( );
   }
   else
   {
      // find the row indexed by lOffset
      pos = m_pRows->FindIndex( lOffset );
      if ( pos )
      {
         lIdx = lOffset;
      }
   }

   while ( pos && bFound == FALSE )
   {
      pRow = GetNextDataRow( pos );
      lIdx++;
      csSrc = pRow->GetString( );
      if ( bExact == FALSE )
      {
         csSrc = csSrc.Mid( 0, lStrLength );
      }

      if ( csSrc == csToFind )
      {
         bFound = TRUE;
      }
   }

   if ( bFound == FALSE )
   {
      lIdx = LB_ERR;
      pRow = 0;
   }

   (*plIdx) = lIdx;
   return( pRow );
}

ZRmtDataRow *
ZRmtDataSet::FindDataRowByIndex( zLONG lIdx )
{
   ZRmtDataRow *pRow = 0;
   POSITION pos = 0;

   if ( lIdx >= 0 )
   {
      // find the row indexed by lIdx
      pos = m_pRows->FindIndex( lIdx );
      if ( pos )
      {
         pRow = GetNextDataRow( pos );
      }
   }

   return( pRow );
}

zLONG
ZRmtDataSet::GetSelItems( zLONG lMaxItems, LPINT rgIndex )
{
   zLONG    lItemCnt = 0;
   zLONG    lItemIdx = 0;
   POSITION pos = 0;
   ZRmtDataRow *pRow = 0;

   if ( lMaxItems < 1 )
      return( LB_ERR );

   pos = GetFirstDataRowPosition( );
   while ( pos && lItemCnt < lMaxItems )
   {
      pRow = GetNextDataRow( pos );
      if ( pRow->GetSel( ) == TRUE )
      {
         rgIndex[ lItemCnt ] = lItemIdx;
         lItemCnt++;
      }

      lItemIdx++;
   }

   return( lItemCnt );
}

zLONG
ZRmtDataSet::GetSelCount( )
{
   zLONG    lItemCnt = 0;
   POSITION pos = 0;
   ZRmtDataRow *pRow = 0;

   pos = GetFirstDataRowPosition( );
   while ( pos )
   {
      pRow = GetNextDataRow( pos );
      if ( pRow->GetSel( ) == TRUE )
      {
         lItemCnt++;
      }
   }

   return( lItemCnt );
}

void
ZRmtDataSet::ResetSelectionSet( zBOOL bSet )
{
   POSITION pos = 0;

   pos = GetFirstDataRowPosition( );
   while ( pos )
   {
      (GetNextDataRow( pos ))->SetSel( bSet );
   }
}

// Function to initialize all the rows in a RmtDataSet.
void
fnInitRmtDataSet( ZRmtDataRow *pdr,
                  WPARAM  wParam,
                  LPARAM  lParam )
{
   if ( wParam == 0 )
      pdr->SetState( zRMT_PROCESSED, zRMT_REMOVE_STATE );

#ifdef DEBUG_ALL
   if ( lParam )
   {
      zCHAR szMsg[ 256 ];
      ZRmtDataItem *prdi;
      POSITION pos = pdr->m_Cols.GetHeadPosition( );

      if ( pos )
      {
         prdi = (ZRmtDataItem *) pdr->m_Cols.GetNext( pos );
         sprintf_s( szMsg, sizeof( szMsg ), "InitRmtDataSet: %s %ld %ld %-32s State: 0x%08lx",
                   (zPCHAR) lParam, pdr->GetOrder( ), pdr->GetKey( ),
                   prdi->GetString( ), pdr->GetState( ) );
         TraceLineS( szMsg, "" );
      }
      else
      {
         TraceLineI( "InitRmtDataSet?: ", pdr->GetKey( ) );
      }
   }
#endif
}

// Merge RmtDataSet.
void
ZRmtDataSet::MergeRowData( zBOOL bReset )
{
   if ( bReset )
      mDeleteInit( m_pRowsOrig );

   if ( m_pRowsOrig == 0 )
   {
      m_pRowsOrig = m_pRows;
      m_pRows = new ZMultiIdxSet( *m_pRowsOrig );
   }
   else
   {
      ZRmtDataRow *pdrOrig;
      ZRmtDataRow *pdr;
      zULONG ulState;
      zLONG  lCntOrig;
      zLONG  lCnt;
      zLONG  k;
      zBOOL  bChanged;

#ifdef DEBUG_ALL
      if ( m_pRows )  // this is not needed other than for debugging
      {
         TraceLineI( "m_pRows Init Cnt: ", m_pRows->GetCount( ) );
         m_pRows->VisitInorder( (fnVISIT_INORDER) fnInitRmtDataSet,
                                0, 0, (LPARAM) "m_pRows Init: " );
      }
#endif

      if ( m_pRowsOrig )
      {
         TraceLineI( "m_pRowsOrig Init Cnt: ", m_pRowsOrig->GetCount( ) );
         m_pRowsOrig->VisitInorder( (fnVISIT_INORDER) fnInitRmtDataSet,
                                    0, 0, (LPARAM) "m_pRowsOrig Init: " );
      }

      // Match up rows in the original and new lists.  Delete those
      // from the new list that match those in the original list.

      // First run through the list sequentially with at most a "one entity
      // look-ahead" to resolve differences (this is to take care of the
      // cases where a single delete or a single add has been done and the
      // instance has been re-activated).
      lCntOrig = m_pRowsOrig->GetCount( );
      lCnt = m_pRows->GetCount( );
      if ( (lCnt && lCntOrig) &&     // ensure a diff of at most one!
           lCnt <= lCntOrig + 1 &&
           lCnt >= lCntOrig - 1 )
      {
         ZRmtDataRow *pdrT;
         zLONG lKeyOrig;
         zLONG lKey;
         zLONG lOrderOrig;
         zLONG lOrder;
         zLONG j;
         zLONG n;

         bChanged = FALSE;
         m_pRows->VisitInorder( (fnVISIT_INORDER) fnInitRmtDataSet, 0 );

         for ( k = 0, j = 0; k < lCntOrig || j < lCnt; k++, j++ )
         {
            if ( k < lCntOrig )
               pdrOrig = (ZRmtDataRow *) *(m_pRowsOrig->m_pvNode + k);
            else
               pdrOrig = 0;

            if ( j < lCnt )
               pdr = (ZRmtDataRow *) *(m_pRows->m_pvNode + j);
            else
               pdr = 0;

            if ( pdr == 0 || pdrOrig == 0 || pdrOrig->Compare( *pdr ) )
            {
               if ( bChanged )
               {
                  // Too many changes for this simple-minded analysis ...
                  // try the next approach.
                  m_pRowsOrig->
                     VisitInorder( (fnVISIT_INORDER) fnInitRmtDataSet, 0 );

                  k = -1;  // set flag to skip out of this simple analysis
                  break;
               }

               bChanged = TRUE;
               if ( pdr && pdrOrig )
               {
                  if ( k + 1 < lCntOrig )
                  {
                     pdrT = (ZRmtDataRow *) *(m_pRowsOrig->m_pvNode + k + 1);
                     if ( pdrT->Compare( *pdr ) == 0 )
                     {
                        pdrOrig->SetState( zRMT_DELETED, zRMT_ADD_STATE );
                        pdr = new ZRmtDataRow( "", pdrOrig->GetKey( ), 0 );
                        pdr->SetState( zRMT_DELETED, zRMT_ADD_STATE );
                        m_pRows->InsertNode( pdr );
                        pdrOrig = pdrT;
                        k++;
                        goto DELETED_DATA_label;
                     }
                  }

                  if ( j + 1 < lCnt )
                  {
                     pdrT = (ZRmtDataRow *) *(m_pRows->m_pvNode + j + 1);
                     if ( pdrT->Compare( *pdrOrig ) == 0 )
                     {
                        pdr->SetState( zRMT_ADDED, zRMT_ADD_STATE );
                        pdr = pdrT;
                        if ( j > 0 )
                        {
                           pdrT = (ZRmtDataRow *) *(m_pRows->m_pvNode + j - 1);
                           lOrder = pdrT->GetOrder( );
                        }
                        else
                           lOrder = 0;

                        pdrT = (ZRmtDataRow *) *(m_pRows->m_pvNode + j);
                        pdrT->SetOrder( lOrder + 1 );

                        j++;
                        goto ADDED_DATA_label;
                     }
                  }
               }

               continue;

ADDED_DATA_label:
               for ( n = j; n < lCnt; n++ )
               {
                  pdrT = (ZRmtDataRow *) *(m_pRows->m_pvNode + n);
                  pdrT->SetState( zRMT_CHANGED_ORDER, zRMT_ADD_STATE );
                  lOrder = pdrT->GetOrder( );
                  pdrT->SetOrder( lOrder + 1 );
               }

               goto EQUAL_DATA_label;

DELETED_DATA_label:
               for ( n = j; n < lCnt; n++ )
               {
                  pdrT = (ZRmtDataRow *) *(m_pRows->m_pvNode + n);
                  pdrT->SetState( zRMT_CHANGED_ORDER, zRMT_ADD_STATE );
                  lOrder = pdrT->GetOrder( );
                  pdrT->SetOrder( lOrder - 1 );
               }

               goto EQUAL_DATA_label;
            }

EQUAL_DATA_label:
            lKeyOrig = pdrOrig->GetKey( );
            lKey = pdr->GetKey( );

            pdrOrig->SetState( zRMT_PROCESSED, zRMT_ADD_STATE );
            pdr->SetState( zRMT_PROCESSED, zRMT_ADD_STATE );
            if ( lKeyOrig != lKey )
            {
               pdr->SetState( zRMT_CHANGED_KEY, zRMT_ADD_STATE );
               pdr->SetKeyOrig( lKeyOrig );
               pdrOrig->SetKey( lKey );
            }

            lOrderOrig = pdrOrig->GetOrder( );
            lOrder = pdr->GetOrder( );
            if ( lOrderOrig != lOrder )
            {
               pdr->SetState( zRMT_CHANGED_ORDER, zRMT_ADD_STATE );
               pdr->SetOrder( j );
               pdrOrig->SetOrder( j );
            }
         }

         if ( k >= 0 )
         {
            // Clean out any processed items in the new list (they match
            // an entry in the original list).
            for ( j = 0; j < m_pRows->GetCount( ); j++ )
            {
               pdr = (ZRmtDataRow *) *(m_pRows->m_pvNode + j);
               ulState = pdr->GetState( );
               if ( ulState == zRMT_PROCESSED )
               {
                  m_pRows->RemoveNode( pdr ); // no change ... don't transmit it
                  j = -1;
               }
            }
         }
      }

      lCntOrig = m_pRowsOrig->GetCount( );
      for ( k = 0; k < lCntOrig; k++ )
      {
         pdrOrig = (ZRmtDataRow *) *(m_pRowsOrig->m_pvNode + k);
         ulState = pdrOrig->GetState( );
         if ( (ulState & zRMT_PROCESSED) == zRMT_PROCESSED )
            continue;

         pdr = (ZRmtDataRow *)
              m_pRows->SeekNode( (void *) pdrOrig->GetKey( ) );

         if ( pdr )
         {
            pdrOrig->SetState( zRMT_PROCESSED, zRMT_ADD_STATE );
            bChanged = FALSE;
            if ( pdrOrig->GetOrder( ) != pdr->GetOrder( ) )
            {
               bChanged = TRUE;
               pdrOrig->SetOrder( pdr->GetOrder( ) );
               pdr->SetState( zRMT_CHANGED_ORDER, zRMT_ADD_STATE );
            }

            if ( pdrOrig->Compare( *pdr ) )
            {
               bChanged = TRUE;
               pdrOrig->CopyData( *pdr );
               pdr->SetState( zRMT_CHANGED_DATA, zRMT_ADD_STATE );
            }

            if ( bChanged == FALSE )
               m_pRows->RemoveNode( pdr ); // no change ... don't transmit it
         }
         else
         {
            pdrOrig->SetState( zRMT_DELETED, zRMT_ADD_STATE );
            pdr = new ZRmtDataRow( *pdrOrig );
            pdr->SetState( zRMT_DELETED, zRMT_ADD_STATE );
            m_pRows->InsertNode( pdr );
         }
      }

      // Clean out any deleted items in the original list.
      for ( k = 0; k < m_pRowsOrig->GetCount( ); k++ )
      {
         pdrOrig = (ZRmtDataRow *) *(m_pRowsOrig->m_pvNode + k);
         ulState = pdrOrig->GetState( );
         if ( (ulState & zRMT_DELETED) == zRMT_DELETED )
         {
            m_pRowsOrig->RemoveNode( pdrOrig );
            k = -1;
         }
      }

      // If there are any items in the new list that have not been processed,
      // they are new and need to be added to the original list.
      lCnt = m_pRows->GetCount( );
      for ( k = 0; k < lCnt; k++ )
      {
         pdr = (ZRmtDataRow *) *(m_pRows->m_pvNode + k);
         ulState = pdr->GetState( );
         if ( (ulState & zRMT_PROCESSED) == 0 )
         {
            pdrOrig = (ZRmtDataRow *)
                  m_pRowsOrig->SeekNode( (void *) pdr->GetKey( ) );
            if ( pdrOrig == 0 )
            {
            // pdr->SetState( zRMT_ADDED, zRMT_SET_STATE );
               TraceLineI( "InsertRmtDataSet: ", pdr->GetKey( ) );

               pdrOrig = new ZRmtDataRow( *pdr );
               m_pRowsOrig->InsertNode( pdrOrig );
            }
            else
            {
               TraceLineS( "ZRmtDataSet::MergeRowData: ",
                           "This cannot happen" );
            }
         }
      }

      if ( m_pRows )
      {
         TraceLineI( "m_pRows Term Cnt: ", m_pRows->GetCount( ) );
         m_pRows->VisitInorder( (fnVISIT_INORDER) fnInitRmtDataSet,
                                0, 1, (LPARAM) "m_pRows Term: " );
      }

      if ( m_pRowsOrig )
      {
         TraceLineI( "m_pRowsOrig Term Cnt: ", m_pRowsOrig->GetCount( ) );
         m_pRowsOrig->VisitInorder( (fnVISIT_INORDER) fnInitRmtDataSet,
                                    0, 1, (LPARAM) "m_pRowsOrig Term: " );
      }
   }
}

// Function to initialize all the rows in a RmtDataSet.
void
fnDisplayRmtDataSet( ZRmtDataRow *pdr,
                     WPARAM  wParam,
                     LPARAM  lParam )
{
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, sizeof( szMsg ), "RmtDataSet Row: %s %ld %ld - ",
             pdr->GetRowDataText( ),
             pdr->GetOrder( ), pdr->GetKey( ) );
   TraceLineX( szMsg, (zLONG) pdr );
}

void
ZRmtDataSet::DisplayListData( )
{
   if ( m_pRows )
   {
      TraceLineI( "ZRmtDataSet::DisplayListData (input order) RowCnt: ",
                  m_pRows->GetCount( ) );
      m_pRows->VisitInorder( (fnVISIT_INORDER) fnDisplayRmtDataSet, 0 );

      TraceLineI( "ZRmtDataSet::DisplayListData (sorted order) RowCnt: ",
                  m_pRows->GetCount( ) );
      m_pRows->VisitInorder( (fnVISIT_INORDER) fnDisplayRmtDataSet, 1 );
   }
}

#endif   // #ifdef zREMOTE_SERVER
