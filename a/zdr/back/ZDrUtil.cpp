/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrutil.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of ZMultiIdxSet, ...
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.01.24    DKS    WEB
//    Fix ordering of list when using InsertNode.
//
// 2001.05.29    DKS    WEB
//    Implement merge of list data.
//
// 2001.04.19    DKS   RemoteServer
//    Added IsRemoteServer operation.
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added Mapping List.
//
// 1999.03.10    DKS
//    Altered the Profile functions to remove name conflict.
//
// 1999.01.21    DKS
//    Protected against null pointer after call to SfGetApplicationForSubtask.
//
// 1998.12.04    DKS
//    ReadWindowPlacement and WriteWindowPlacement's new signature.
//    Moved the processing of user INI files from Zeidon environment variable
//    directory to LOCAL work directory specified in Zeidon.app. (TB zTAG_LTH6)
//
// 1998.08.13    DKS
//    Fixed end-of-array diagnostic processing.
//
// 1998.06.23    DKS
//    Altered message to be a diagnostic message when overwrite detected.
//


#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ZOrderIndex::ZOrderIndex( fnCOMPARE pfnCompare,
                          fnCOMPARE pfnSeek )
{
   ASSERT( pfnCompare );
   ASSERT( pfnSeek );
   m_pfnCompare = pfnCompare;
   m_pfnSeek = pfnSeek;
   m_plIdx = 0;   // new zLONG[ m_lMaxNodes ];
   m_lIdxCnt = 0;        // index array size
   m_pNextOrderIndex = 0;
   m_bDirty = TRUE;
}

ZOrderIndex::~ZOrderIndex( )
{
   mDeleteInitA( m_plIdx );
}

////////////////////////////////////////////////////////////////////////////
//////////////////////////// ZMultiIdxSet ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// The search index is implemented with some methods that depend on the
// structure of the nodes to be placed in the search table.  The following
// is an attempt to document the processing to search for a specified
// node.  Assume the list involves 11 strings:
//
//  input   string      sorted        sorted index
//  -----   ---------   ------        ------------
//    1.    CAPRICORN     (4)          1  -->   2
//    2.    AQUARIUS      (1)          2  -->  10
//    3.    PICES         (8)          3  -->   9
//    4.    GEMINI        (5)          4  -->   1
//    5.    TAURUS       (10)          5  -->   4
//    6.    SCORPIO       (9)          6  -->  11
//    7.    VIRGO        (11)          7  -->   8
//    8.    LIBRA         (7)          8  -->   3
//    9.    CANCER        (3)          9  -->   6
//   10.    ARIES         (2)         10  -->   5
//   11.    LEO           (6)         11  -->   7
//
//
////////////////////////////////////////////////////////////////////////////

// ZMultiIdxSet - ctor
ZMultiIdxSet::ZMultiIdxSet( fnCOMPARE pfnCompare,
                            fnCOMPARE pfnSeek,
                            zLONG     lMaxNodeCnt,
                            fnVISIT_INORDER pfnPurge,
                            fnCOPY    pfnCopy )
{
// TraceLineX( "ZMultiIdxSet::ctor ", (zLONG) this );
// TraceLineI( "ZMultiIdxSet::ctor MaxNodes: ", lMaxNodeCnt );
   ASSERT( lMaxNodeCnt > 0 );

   if ( lMaxNodeCnt )
      m_lMaxNodes = lMaxNodeCnt;
   else
      m_lMaxNodes = 100;

   m_lNodeCnt = 0;
   m_pfnPurge = pfnPurge;
   m_pfnCopy = pfnCopy;
   m_pfnVisit = 0;
   m_nErrorCode = 0;
   m_pHeadIndex = 0;
   m_pCurrIndex = 0;
   m_nIdxCnt = 0;

   m_pvNode = new zPVOID[ m_lMaxNodes + 1 ];
   m_pvNode[ m_lMaxNodes ] = (zPVOID) -1;
   if ( pfnCompare && pfnSeek )
   {
      AddOrderIndex( 1, pfnCompare, pfnSeek );
      UseOrderIndex( 1 );
   }
}

// ZMultiIdxSet - copy ctor
ZMultiIdxSet::ZMultiIdxSet( const ZMultiIdxSet& rmix )
{
   m_lMaxNodes = rmix.m_lMaxNodes;
   m_lNodeCnt = 0;
   if ( rmix.m_pfnCopy )
   {
      m_pfnPurge = rmix.m_pfnPurge;
      m_pfnCopy = rmix.m_pfnCopy;
   }
   else
   {
      m_pfnPurge = 0;
      m_pfnCopy = 0;
   }

   m_pfnVisit = rmix.m_pfnVisit;
   m_nErrorCode = 0;

   m_pHeadIndex = 0;
   m_pCurrIndex = 0;
   m_nIdxCnt = 0;

   ZOrderIndex *pIndex = rmix.m_pHeadIndex;
   while ( m_nIdxCnt < rmix.m_nIdxCnt )
   {
      AddOrderIndex( m_nIdxCnt + 1, pIndex->m_pfnCompare, pIndex->m_pfnSeek );
      if ( pIndex == rmix.m_pCurrIndex )
         UseOrderIndex( m_nIdxCnt );

      pIndex = pIndex->m_pNextOrderIndex;
   }

   m_pvNode = new zPVOID[ m_lMaxNodes + 1 ];
   if ( m_pfnCopy )
   {
      zLONG k;

      for ( k = 0; k < rmix.m_lNodeCnt; k++ )
         m_pvNode[ k ] = (*m_pfnCopy)( (zPVOID) *(rmix.m_pvNode + k ) );

      m_lNodeCnt = rmix.m_lNodeCnt;
      RebuildOrderIndex( m_pCurrIndex );
   }
   else
   {
      m_lNodeCnt = rmix.m_lNodeCnt;
      zmemcpy( m_pvNode, rmix.m_pvNode, m_lMaxNodes * sizeof( zPVOID ) );
   }

   m_pvNode[ m_lMaxNodes ] = (zPVOID) -1;
}

ZMultiIdxSet::~ZMultiIdxSet( )
{
// TraceLineX( "ZMultiIdxSet::dtor ", (zLONG) this );
// TraceLineI( "ZMultiIdxSet::dtor MaxNodes: ", m_lMaxNodes );
   if ( m_pvNode[ m_lMaxNodes ] != (zPVOID) -1 )
   {
      TraceLineI( "ZMultiIdxSet::dtor overwrite detected @", m_lMaxNodes );
   // SysDiagnosticMessage( "Zeidon Driver",
   //                       "ZMultiIdxSet::dtor overwrite detected @", TRUE );
   }

   if ( m_pfnPurge )
   {
      VisitInorder( m_pfnPurge, -1 );  // reverse order ... to remove child
                                       // nodes before parent nodes
   }

   mDeleteInitA( m_pvNode );  // We are at risk of deleting what someone
                              // else allocated (and did not intend for
                              // us to delete) by assuming we can delete
                              // this willy-nilly.  Currently, the driver
                              // does not pre-allocate memory for the
                              // list of nodes.

   ZOrderIndex *pOrderIndex;
   while ( m_pHeadIndex )
   {
      pOrderIndex = m_pHeadIndex;
      m_pHeadIndex = pOrderIndex->m_pNextOrderIndex;
      pOrderIndex->m_pNextOrderIndex = 0;
      delete( pOrderIndex );
   }
}

// The action required when a node is visited can be adjusted during the
// lifetime of the list.
void
ZMultiIdxSet::VisitInorder( fnVISIT_INORDER pfnVisit,
                            zSHORT nSortedOrder,
                            WPARAM wParam,
                            LPARAM lParam )
{
   zLONG k;

 //TraceLineX( "VisitInorder ", (zLONG) this );
 //TraceLineI( "    m_lNodeCnt ", m_lNodeCnt );

   m_pvNodeErr = 0;           // first node in error during visit inorder
   if ( nSortedOrder > 0 && m_pCurrIndex )
   {
      for ( k = m_lMaxNodes - m_lNodeCnt; k < m_lMaxNodes; k++ )
      {
         (*pfnVisit)( (zPVOID) *(m_pvNode + m_pCurrIndex->m_plIdx[ k ]),
                      wParam, lParam );
      }
   }
   else
   {
      // Not traversing in sorted order ... either in reverse or forward
      // order as originally entered in the list.
      if ( nSortedOrder < 0 )
      {
         for ( k = m_lNodeCnt; k > 0; )
         {
            k--;
            (*pfnVisit)( (zPVOID) *(m_pvNode + k), wParam, lParam );
         }
      }
      else
      {
         for ( k = 0; k < m_lNodeCnt; k++ )
         {
          //TraceLineX( "       pvNode ", (zLONG) (zPVOID) *(m_pvNode + k) );
            (*pfnVisit)( (zPVOID) *(m_pvNode + k), wParam, lParam );
         }
      }
   }
}

zPVOID
ZMultiIdxSet::GetAt( zLONG lPos )
{
   if ( lPos > 0 && lPos <= m_lNodeCnt )
      return( m_pvNode[ lPos - 1 ] );
   else
      return( 0 );
}

POSITION
ZMultiIdxSet::FindIndex( zLONG lPos )
{
   if ( GetAt( lPos ) )
      return( (POSITION) lPos );
   else
      return( 0 );
}

// Returns the index to the node in the array.
zLONG
ZMultiIdxSet::GetNodeIndex( zCPVOID pvNode1 )
{
   zLONG k;

   for ( k = 0; k < m_lNodeCnt; k++ )
   {
      if ( pvNode1 == m_pvNode[ k ] )
         return( k + 1 );
   }

   return( 0 );
}

// Returns the pointer to the node.
zPVOID
ZMultiIdxSet::SeekNode( zCPVOID pvData )
{
   ASSERT( m_pCurrIndex );
   zLONG  lLower = m_lMaxNodes - m_lNodeCnt;
   zLONG  lUpper = m_lMaxNodes - 1;
   zLONG  lMid;
   zSHORT nRC;

   if ( m_pCurrIndex->m_bDirty )
      RebuildOrderIndex( m_pCurrIndex );

   while ( lLower <= lUpper )
   {
      lMid = (lUpper + lLower) >> 1;    // ">> 1" ==> "divide by 2"
//    nRC = m_plIdx[ lMid ];
      nRC = (*(m_pCurrIndex->m_pfnSeek))( (zPVOID) pvData,
                                          *(m_pvNode + m_pCurrIndex->m_plIdx[ lMid ]) );
      if ( nRC > 0 )
         lLower = lMid + 1;
      else
      if ( nRC < 0 )
         lUpper = lMid - 1;
      else
         return( *(m_pvNode + m_pCurrIndex->m_plIdx[ lMid ]) );
   }

   m_nErrorCode = zMIXSET_NOTFND;
   return( 0 );
}

// Return the index to the new node (zMIXSET_DUP if duplicate node).
// If cPos is 'A' (After) or 'B' (Before) and pvPosNode is specified,
// reorder the new node as specified.
zLONG
ZMultiIdxSet::InsertNode( zPVOID pvNode, zPVOID pvOrderNode, zCHAR chPos )
{
   if ( pvNode == 0 )
   {
      m_nErrorCode = zMIXSET_INVALID_PTR;
      return( 0 );
   }

   if ( m_lNodeCnt >= m_lMaxNodes )
   {
      zPVOID *pvNodeTemp;

   // TraceLineI( "ZMultiIdxSet::InsertNode Expanding from: ", m_lMaxNodes );
      m_lMaxNodes += zINCR_NODE_CNT;
   // TraceLineI( "ZMultiIdxSet::InsertNode Expanded to: ", m_lMaxNodes );
      pvNodeTemp = new zPVOID[ m_lMaxNodes + 1 ];
      zmemcpy( pvNodeTemp, m_pvNode,
               (m_lMaxNodes - zINCR_NODE_CNT) * sizeof( zPVOID ) );
      delete [] m_pvNode;
      m_pvNode = pvNodeTemp;
      m_pvNode[ m_lMaxNodes ] = (zPVOID) -1;
   }

   if ( m_pCurrIndex && m_lMaxNodes > m_pCurrIndex->m_lIdxCnt )
   {
      zLONG *m_plIdxTemp;

      m_plIdxTemp = new zLONG[ m_lMaxNodes ];

      // Keep data in the upper part of the array.
      if ( m_pCurrIndex->m_plIdx )
      {
         zmemcpy( m_plIdxTemp + m_lMaxNodes - m_lNodeCnt,
                  m_pCurrIndex->m_plIdx,
                  m_pCurrIndex->m_lIdxCnt * sizeof( zLONG ) );
         delete [] m_pCurrIndex->m_plIdx;
      }

      m_pCurrIndex->m_plIdx = m_plIdxTemp;
      m_pCurrIndex->m_lIdxCnt = m_lMaxNodes;
   }

   ZOrderIndex *pOrderIndex = m_pHeadIndex;
   while ( pOrderIndex )
   {
      if ( pOrderIndex != m_pCurrIndex )
         pOrderIndex->m_bDirty = TRUE;

      pOrderIndex = pOrderIndex->m_pNextOrderIndex;
   }

   if ( m_pCurrIndex && m_lNodeCnt == 0 )
      m_pCurrIndex->m_bDirty = FALSE;

   zLONG  lNodeCnt;
   zLONG  lLower;

   if ( m_pCurrIndex && m_pCurrIndex->m_plIdx &&
        m_pCurrIndex->m_bDirty == FALSE )
   {
      zPVOID pvTemp;
      zLONG  lUpper = m_lMaxNodes - 1;
      zLONG  lMid;
      zSHORT nRC;

      lLower = m_lMaxNodes - m_lNodeCnt;
      while ( lLower <= lUpper )
      {
         lMid = (lUpper + lLower) >> 1;    // ">> 1" ==> "divide by 2"
//       nRC = m_plIdx[ lMid ];
         pvTemp = *(m_pvNode + m_pCurrIndex->m_plIdx[ lMid ]);
         nRC = (*(m_pCurrIndex->m_pfnCompare))( pvNode, pvTemp );
         if ( nRC > 0 )
            lLower = lMid + 1;
         else
         if ( nRC < 0 )
            lUpper = lMid - 1;
         else
         {
            m_nErrorCode = zMIXSET_DUP;
            return( 0 );
         }
      }

      if ( lLower > (zLONG) (m_lMaxNodes - m_lNodeCnt) )
      {
         zmemcpy( m_pCurrIndex->m_plIdx + m_lMaxNodes - m_lNodeCnt - 1,
                  m_pCurrIndex->m_plIdx + m_lMaxNodes - m_lNodeCnt,
                  (lLower - m_lMaxNodes + m_lNodeCnt) * sizeof( zLONG ) );
      }

      *(m_pvNode + m_lNodeCnt) = pvNode;
      m_pCurrIndex->m_plIdx[ lLower - 1 ] = m_lNodeCnt;
      lNodeCnt = ++m_lNodeCnt;             // pre-increment intended
   }
   else
   {
      if ( m_pCurrIndex )
         m_pCurrIndex->m_bDirty = TRUE;

      *(m_pvNode + m_lNodeCnt) = pvNode;
      lNodeCnt = m_lNodeCnt++;             // post-increment intended
   }

   zLONG lIdx;

   // Order the node at the appropriate point in the node list.
   if ( pvOrderNode && (lIdx = GetNodeIndex( pvOrderNode )) != 0 )
   {
      // lIdx is the (1-based) index of OrderNode within the m_pvNode array.
      lIdx--;
      zPVOID pvTemp;
      zPVOID pvNext;
      zLONG ulNodeIdx = lIdx;

      if ( chPos == 'B' || chPos == 'b' )
      {
         pvTemp = pvOrderNode;
      }
      else
      {
         lIdx++;
         pvTemp = *(m_pvNode + lIdx);
      }

      // Move everybody up 1 slot.
      *(m_pvNode + lIdx) = pvNode;
      while ( lIdx < m_lNodeCnt )
      {
         lIdx++;
         if ( lIdx < m_lMaxNodes )
            pvNext = *(m_pvNode + lIdx);

         *(m_pvNode + lIdx) = pvTemp;
         pvTemp = pvNext;
      }

      if ( m_pCurrIndex->m_plIdx )
      {
#if 0
         TraceLineI( "ZMultiIdxSet::InsertNode Before: ", m_lNodeCnt );
         for ( lIdx = m_lMaxNodes - m_lNodeCnt; lIdx < m_lMaxNodes; lIdx++ )
         {
            TraceLineX( "   Index: ", m_pCurrIndex->m_plIdx[ lIdx ] );
         }
#endif

         for ( lIdx = m_lMaxNodes - m_lNodeCnt; lIdx < m_lMaxNodes; lIdx++ )
         {
            if ( m_pCurrIndex->m_plIdx[ lIdx ] >= ulNodeIdx )
               m_pCurrIndex->m_plIdx[ lIdx ]++;
         }

         m_pCurrIndex->m_plIdx[ lLower - 1 ] = ulNodeIdx;

#if 0
         TraceLineI( "ZMultiIdxSet::InsertNode After: ", m_lNodeCnt );
         for ( lIdx = m_lMaxNodes - m_lNodeCnt; lIdx < m_lMaxNodes; lIdx++ )
         {
            TraceLineX( "   Index: ", m_pCurrIndex->m_plIdx[ lIdx ] );
         }
#endif
      }
   }

   return( lNodeCnt );
}

zLONG
ZMultiIdxSet::RemoveNode( zCPVOID pvNode )
{
   zLONG k;

   ZOrderIndex *pOrderIndex = m_pHeadIndex;
   while ( pOrderIndex )
   {
      if ( pOrderIndex != m_pCurrIndex )
         pOrderIndex->m_bDirty = TRUE;

      pOrderIndex = pOrderIndex->m_pNextOrderIndex;
   }

   if ( (k = GetNodeIndex( pvNode )) != 0 )
   {
      // k is the index of the Node within the m_pvNode array.
      zLONG ulNodeIdx = k - 1;

      if ( k < m_lNodeCnt )
      {
         zmemcpy( m_pvNode + ulNodeIdx, m_pvNode + k,
                  (m_lMaxNodes - k) * sizeof( zPVOID ) );
      }
      else
         *(m_pvNode + ulNodeIdx) = (zPVOID) 0;

      if ( m_pCurrIndex && m_pCurrIndex->m_plIdx )
      {
         for ( k = m_lMaxNodes - m_lNodeCnt; k < m_lMaxNodes; k++ )
         {
            if ( m_pCurrIndex->m_plIdx[ k ] == ulNodeIdx )
            {
               zLONG j = k;

               while ( j > m_lMaxNodes - m_lNodeCnt )
               {
                  m_pCurrIndex->m_plIdx[ j ] = m_pCurrIndex->m_plIdx[ j - 1 ];
                  j--;
               }

               m_pCurrIndex->m_plIdx[ m_lMaxNodes - m_lNodeCnt ] = 0;
            }
            else
            if ( m_pCurrIndex->m_plIdx[ k ] > ulNodeIdx )
               m_pCurrIndex->m_plIdx[ k ]--;
         }
      }
      else
      if ( m_pCurrIndex )
         m_pCurrIndex->m_bDirty = TRUE;

      m_lNodeCnt--;
      return( ulNodeIdx );
   }

   return( 0 );
}

void
ZMultiIdxSet::SetCopyFunction( fnCOPY pfnCopy )
{
   m_pfnCopy = pfnCopy;
}

zSHORT
ZMultiIdxSet::GetError( )
{
   zSHORT nError = m_nErrorCode;

   m_nErrorCode = 0;
   return( nError );
}

void
ZMultiIdxSet::ResetContent( )

{
   ASSERT( m_pfnPurge );
   VisitInorder( m_pfnPurge, -1 );  // reverse order ... to remove child
                                    // nodes before parent nodes
   m_lNodeCnt = 0;
   ZOrderIndex *pOrderIndex = m_pHeadIndex;
   while ( pOrderIndex )
   {
      pOrderIndex->m_bDirty = TRUE;
      pOrderIndex = pOrderIndex->m_pNextOrderIndex;
   }

// mDeleteInitA( m_pvNode );  may as well keep the memory
}

zBOOL
ZMultiIdxSet::AddOrderIndex( zSHORT    nId,
                             fnCOMPARE pfnCompare,
                             fnCOMPARE pfnSeek )
{
   if ( nId == m_nIdxCnt + 1 )
   {
      m_nIdxCnt = nId;
      ZOrderIndex *pOrderIndex = new ZOrderIndex( pfnCompare, pfnSeek );
      pOrderIndex->m_pNextOrderIndex = m_pHeadIndex;
      m_pHeadIndex = pOrderIndex;
      return( TRUE );
   }
   else
      return( FALSE );
}

zBOOL
ZMultiIdxSet::UseOrderIndex( zSHORT nId )
{
   if ( nId > 0 && nId <= m_nIdxCnt )
   {
      m_pCurrIndex = m_pHeadIndex;
      while ( nId-- > 1 )
         m_pCurrIndex = m_pCurrIndex->m_pNextOrderIndex;

      return( TRUE );
   }
   else
      return( FALSE );
}

zBOOL
ZMultiIdxSet::RebuildOrderIndex( ZOrderIndex *pCurrIndex )
{
   ASSERT( pCurrIndex );

   // Fix up the index.
   if ( m_lMaxNodes != pCurrIndex->m_lIdxCnt ||
        pCurrIndex->m_plIdx == 0 )
   {
      mDeleteInit( pCurrIndex->m_plIdx );
      pCurrIndex->m_plIdx = new zLONG[ m_lMaxNodes ];
      pCurrIndex->m_lIdxCnt = m_lMaxNodes;
   }

   zPVOID pvNode;
   zLONG  lUpper;
   zLONG  lLower;
   zLONG  lMid;
   zLONG  k;
   zSHORT nRC;

   for ( k = 0; k < m_lNodeCnt; k++ )
   {
      pvNode = *(m_pvNode + k);
      lUpper = m_lMaxNodes - 1;
      lLower = m_lMaxNodes - k;
      while ( lLower <= lUpper )
      {
         lMid = (lUpper + lLower) >> 1;    // ">> 1" ==> "divide by 2"
//       nRC = m_plIdx[ lMid ];
         nRC = (*(pCurrIndex->m_pfnCompare))( pvNode,
                                              *(m_pvNode + pCurrIndex->m_plIdx[ lMid ]) );
         if ( nRC > 0 )
            lLower = lMid + 1;
         else
         if ( nRC < 0 )
            lUpper = lMid - 1;
         else
         {
            m_nErrorCode = zMIXSET_DUP;
            break;
         }
      }

      if ( lLower > (zLONG) (m_lMaxNodes - k) )
      {
         zmemcpy( pCurrIndex->m_plIdx + m_lMaxNodes - k - 1,
                  pCurrIndex->m_plIdx + m_lMaxNodes - k,
                  (lLower - m_lMaxNodes + k) * sizeof( zLONG ) );
      }

      m_pCurrIndex->m_plIdx[ lLower - 1 ] = k;
   }

   pCurrIndex->m_bDirty = FALSE;
   return( TRUE );
}

zBOOL
ZMultiIdxSet::RemoveOrderIndex( zSHORT nId )
{
   if ( nId > 0 && nId <= m_nIdxCnt )
   {
      m_pCurrIndex = m_pHeadIndex;
      nId = m_nIdxCnt - nId;
      while ( nId > 0 )
         m_pCurrIndex = m_pCurrIndex->m_pNextOrderIndex;

      return( TRUE );
   }
   else
      return( FALSE );
}

POSITION
ZMultiIdxSet::GetHeadPosition( )
{
   return( m_lNodeCnt ? (POSITION) 1 : (POSITION) 0 );
}

zPVOID
ZMultiIdxSet::GetNext( POSITION& pos )
{
   zPVOID pNode = GetAt( (zLONG) pos );
   if ( (zLONG) pos < m_lNodeCnt )
      pos++;
   else
      pos = 0;

   return( pNode );
}

POSITION
ZMultiIdxSet::AddHead( zPVOID pNode )
{
#if 0
   zLONG k;
   TraceLineX( "ZMultiIdxSet::AddHead: ", (zLONG) pNode );
   for ( k = 0; k < m_lNodeCnt; k++ )
   {
      TraceLineX( "       pvNode ", (zLONG) (zPVOID) *(m_pvNode + k) );
   }
#endif

   zPVOID pHeadNode = GetAt( 1 );
   POSITION pos = (POSITION) InsertNode( pNode, pHeadNode, 'B' );

#if 0
   TraceLineX( "ZMultiIdxSet::AddHead Before: ", (zLONG) pHeadNode );
   for ( k = 0; k < m_lNodeCnt; k++ )
   {
      TraceLineX( "       pvNode ", (zLONG) (zPVOID) *(m_pvNode + k) );
   }
#endif

   return( pos );
}

POSITION
ZMultiIdxSet::AddTail( zPVOID pNode )
{
   return( (POSITION) InsertNode( pNode ) );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(ZDriverException, CException)


ZDriverException::ZDriverException( BOOL bAutoDelete )
                 :CException(bAutoDelete)
{
   m_csErrorMessage.Empty();
}

ZDriverException::ZDriverException( CString csErrorMsg ) :
                  CException( TRUE )
{
   m_csErrorMessage = csErrorMsg;
}

ZDriverException::~ZDriverException()
{
}

BOOL
ZDriverException::GetErrorMessage( LPTSTR lpszError, UINT nMaxError,
                                   PUINT pnHelpContext )
{
   ASSERT( lpszError && AfxIsValidString( lpszError, nMaxError ) );

   if ( pnHelpContext )
      *pnHelpContext = 0;

   lstrcpyn( lpszError, m_csErrorMessage, nMaxError );
   return( TRUE );
}

int
ZDriverException::ReportError( UINT nType, UINT nMessageID )
{
   return( CException::ReportError( nType, nMessageID ) );
}

void
ZDriverException::Delete( )
{
   if ( m_bAutoDelete > 0 )
   {
      delete( this );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// The GetProfileSection function is not case-sensitive; the strings can be
// a combination of uppercase and lowercase letters.
//
// This operation is atomic; no updates to the WIN.INI file are allowed
// while the keys and values for the section are being copied to the buffer.
//
// Windows NT:
//
// Calls to profile functions may be mapped to the registry instead of to
// the initialization files. This mapping occurs when the initialization file
// and section are specified in the registry under the following keys:
//
// HKEY_LOCAL_MACHINE\Software\Microsoft\
// Windows NT\CurrentVersion\IniFileMapping
//
// When the operation has been mapped, the GetProfileSection function
// retrieves information from the registry, not from the initialization file;
// the change in the storage location has no effect on the function's behavior.
//
// The Win32 Profile functions (Get/WriteProfile*, Get/WritePrivateProfile*)
// use the following steps to locate initialization information:
//
// 1. Look in the registry for the name of the initialization file, say
// myfile.ini, under IniFileMapping:
//
// HKEY_LOCAL_MACHINE\Software\Microsoft\
// Windows NT\CurrentVersion\IniFileMapping\myfile.ini
//
// 2. Look for the section name specified by lpAppName. This will be a
// named value under myfile.ini, or a subkey of myfile.ini, or will not exist.
//
// 3. If the section name specified by lpAppName is a named value under
// myfile.ini, then that value specifies where in the registry you will find
// the keys for the section.
//
// 4. If the section name specified by lpAppName is a subkey of myfile.ini,
// then named values under that subkey specify where in the registry you
// will find the keys for the section. If the key you are looking for does
// not exist as a named value, then there will be an unnamed value (shown
// as "<No Name>") that specifies the default location in the registry
// where you will find the key.
//
// 5. If the section name specified by lpAppName does not exist as a named
// value or as a subkey under myfile.ini, then there will be an unnamed
// value (shown as "<No Name>") under myfile.ini that specifies the default
// location in the registry where you will find the keys for the section.
//
// 6. If there is no subkey for myfile.ini, or if there is no entry for the
// section name, then look for the actual myfile.ini on the disk and read
// its contents.
//
// When looking at values in the registry that specify other registry
// locations, there are several prefixes that change the behavior of the
// ini file mapping:
//
// ! - this character forces all writes to go both to the registry and to the
// .INI file on disk.
//
// # - this character causes the registry value to be set to the value in the
// Windows 3.1 .INI file when a new user logs in for the first time after
// setup.
//
// @ - this character prevents any reads from going to the .INI file on disk
// if the requested data is not found in the registry.
//
// USR: - this prefix stands for HKEY_CURRENT_USER, and the text after
// the prefix is relative to that key.
//
// SYS: - this prefix stands for HKEY_LOCAL_MACHINE\SOFTWARE,
// and the text after the prefix is relative to that key.
//
/////////////////////////////////////////////////////////////////////////////
// Helpers for saving/restoring window state

static TCHAR szWindowPos[] = _T( "WindowPos" );
static TCHAR szPlaceFormat[] = _T( "%u,%u,%d,%d,%d,%d,%d,%d,%d,%d" );

zOPER_EXPORT UINT OPERATION
GetTaskbarEdge( CRect rect )
{
   UINT uEdge = -1;

   if ( rect.top == rect.left && rect.bottom > rect.right )
   {
      uEdge = ABE_LEFT;
   }
   else
   if ( rect.top == rect.left && rect.bottom < rect.right )
   {
      uEdge = ABE_TOP;
   }
   else
   if ( rect.top > rect.left )
   {
      uEdge = ABE_BOTTOM;
   }
   else
   {
      uEdge = ABE_RIGHT;
   }

   return( uEdge );
}

void
fnGetLocalPath( zVIEW vSubtask, zPCHAR pchLocalPath, zLONG lMaxLth, zCPCHAR cpcIniID )
{
   LPAPP pApp;
   static int nTrace = 0;

   if ( vSubtask && SfGetApplicationForSubtask( &pApp, vSubtask ) == 0 && pApp )
   {
      strcpy_s( pchLocalPath, lMaxLth, pApp->szLocalDir );
   }
   else
      SysGetEnvVar( pchLocalPath, "ZEIDON", zMAX_FILENAME_LTH );

   SysAppendcDirSep( pchLocalPath );
   wsprintf( pchLocalPath + zstrlen( pchLocalPath ), "%s.ini", cpcIniID );
   if ( nTrace == 0 )
   {
      TraceLineS( "--WindowPlacement configuration file: ", pchLocalPath );
      nTrace++;
   }
}

zOPER_EXPORT zBOOL OPERATION
ReadWindowPlacement( zVIEW    vSubtask,
                     zCPCHAR  cpcIniID,
                     zCPCHAR  cpcDlgTag,
                     zCPCHAR  cpcWndTag,
                     LPWINDOWPLACEMENT pwp )
{
   zCHAR  szZeidonPath[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szSection[ 80 ];
   zCHAR  szBuffer[ 256 ];

   fnGetLocalPath( vSubtask, szZeidonPath, zsizeof( szZeidonPath ), cpcIniID );
   wsprintf( szSection, "USR:%s.%s", cpcDlgTag, cpcWndTag );

   DWORD dw =
   GetPrivateProfileString( szSection,          // section name
                            szWindowPos,        // key name
                            "",                 // default string
                            szBuffer,           // destination buffer
                            zsizeof( szBuffer ), // size of destination buffer
                            szZeidonPath );     // ini filename

#ifdef DEBUG_ALL
   zCHAR szMsg[ 512 ];
   sprintf_s( szMsg, zsizeof( szMsg ), "%s %s: %s", szSection,
                    "ReadWindowPlacement flags,showCmd,minX,minY,maxX,maxY,l,t,r,b: ",
                    szBuffer );
   TraceLineS( szMsg, "" );
#endif

   if ( dw < 2 || szBuffer[ 0 ] == 0 )
      return( FALSE );

   WINDOWPLACEMENT wp;
   int nRead = _stscanf( szBuffer, szPlaceFormat,
                         &wp.flags, &wp.showCmd,
                         &wp.ptMinPosition.x, &wp.ptMinPosition.y,
                         &wp.ptMaxPosition.x, &wp.ptMaxPosition.y,
                         &wp.rcNormalPosition.left,
                         &wp.rcNormalPosition.top,
                         &wp.rcNormalPosition.right,
                         &wp.rcNormalPosition.bottom );

   if ( nRead != 10 || wp.rcNormalPosition.bottom - wp.rcNormalPosition.top < 50 ||
        wp.rcNormalPosition.right - wp.rcNormalPosition.left < 200 )
   {
      return( FALSE );
   }

   wp.length = sizeof( wp );
   *pwp = wp;
   return( TRUE );
}

// write a window placement to settings section of app's ini file
zOPER_EXPORT void OPERATION
WriteWindowPlacement( zVIEW    vSubtask,
                      zCPCHAR  cpcIniID,
                      zCPCHAR  cpcDlgTag,
                      zCPCHAR  cpcWndTag,
                      LPWINDOWPLACEMENT pwp )
{
   zCHAR  szZeidonPath[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szSection[ 80 ];

   fnGetLocalPath( vSubtask, szZeidonPath, zsizeof( szZeidonPath ), cpcIniID );
   wsprintf( szSection, "USR:%s.%s", cpcDlgTag, cpcWndTag );

   TCHAR szBuffer[ zsizeof( "-2147483647," ) * 10 ];

   if ( pwp->showCmd == SW_SHOWMINIMIZED )
      pwp->showCmd = SW_SHOWNORMAL;

   wsprintf( szBuffer, szPlaceFormat,
             pwp->flags, pwp->showCmd,
             pwp->ptMinPosition.x, pwp->ptMinPosition.y,
             pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
             pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
             pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom );

#ifdef DEBUG_ALL
   zCHAR szMsg[ 512 ];
   sprintf_s( szMsg, zsizeof( szMsg ), "%s %s: %s", szSection,
                    "WriteWindowPlacement flags,showCmd,minX,minY,maxX,maxY,l,t,r,b: ",
                    szBuffer );
   TraceLineS( szMsg, "" );
#endif

   WritePrivateProfileString( szSection,        // section name
                              szWindowPos,      // key name
                              szBuffer,         // string to add
                              szZeidonPath );   // ini filename
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static TCHAR szToolBarFormat[] =
      _T( "%u,%d,%d,%d,%d,%d,%d,%u,%d,%u,%d,%d,%d,%d,%lu,%d,%d" );
//         1  2  3  4  5  6x 6y 7  8  9  At Al Ab Ar B   Cx Cy
//
   // 1  UINT m_nBarID;           // ID of this bar
   // 2  BOOL m_bVisible;         // visibility of this bar
   // 3  BOOL m_bFloating;        // whether floating or not
   // 4  BOOL m_bHorz;            // orientation of floating dockbar
   // 5  BOOL m_bDockBar;         // TRUE if a dockbar
   //
   // 6  CPoint m_pointPos;       // topleft point of window
   // 7  UINT m_nMRUWidth;        // MRUWidth for Dynamic Toolbars
   // 8  BOOL m_bDocking;         // TRUE if this bar has a DockContext
   // 9  UINT m_uMRUDockID;       // most recent docked dockbar
   // A  CRect m_rectMRUDockPos;  // most recent docked position
   // B  DWORD m_dwMRUFloatStyle; // most recent floating orientation
   // C  CPoint m_ptMRUFloatPos;  // most recent floating position
   //
   // CPtrArray m_arrBarID;   // bar IDs for bars contained within this one

zOPER_EXPORT zBOOL OPERATION
ReadToolBarPlacement( ZSubtask *pZSubtask,
                      zCPCHAR  cpcIniID,
                      zCPCHAR  cpcBarID,
                      CControlBarInfo *pCB_Info )
{
   zCHAR szBuffer[ 256 ];
   zCHAR szCtrlInfo[ 64 ];

   strcpy_s( szCtrlInfo, zsizeof( szCtrlInfo ), "CtrlInfo:" );
   strcpy_s( szCtrlInfo + 9, zsizeof( szCtrlInfo ) - 9, cpcBarID );

   fnGetProfileStr( pZSubtask, cpcIniID, szCtrlInfo,
                    szBuffer, zsizeof( szBuffer ) );

   int nRead = _stscanf( szBuffer, szToolBarFormat,
                         &(pCB_Info->m_nBarID),
                         &(pCB_Info->m_bVisible),
                         &(pCB_Info->m_bFloating),
                         &(pCB_Info->m_bHorz),
                         &(pCB_Info->m_bDockBar),
                         &(pCB_Info->m_pointPos.x),
                         &(pCB_Info->m_pointPos.y),
                         &(pCB_Info->m_nMRUWidth),
                         &(pCB_Info->m_bDocking),
                         &(pCB_Info->m_uMRUDockID),
                         &(pCB_Info->m_rectMRUDockPos.top),
                         &(pCB_Info->m_rectMRUDockPos.left),
                         &(pCB_Info->m_rectMRUDockPos.bottom),
                         &(pCB_Info->m_rectMRUDockPos.right),
                         &(pCB_Info->m_dwMRUFloatStyle),
                         &(pCB_Info->m_ptMRUFloatPos.x),
                         &(pCB_Info->m_ptMRUFloatPos.y) );


//?pDockState->ScalePoint( m_pointPos );
//?pDockState->ScaleRectPos( m_rectMRUDockPos );
//?pDockState->ScalePoint( m_ptMRUFloatPos );

   if ( nRead != 17 )
      return( FALSE );

   return( TRUE );
}

zOPER_EXPORT void OPERATION
WriteToolBarPlacement( ZSubtask *pZSubtask,
                       zCPCHAR  cpcIniID,
                       zCPCHAR  cpcBarID,
                       CControlBarInfo *pCB_Info )
{
   zCHAR szCtrlInfo[ 64 ];

// if ( m_bDockBar && m_bVisible && !m_bFloating && m_pointPos.x == -1 &&
//      m_pointPos.y == -1 && m_arrBarID.GetSize() <= 1 )
// {
//    return( FALSE );
// }

   strcpy_s( szCtrlInfo, zsizeof( szCtrlInfo ), "CtrlInfo:" );
   strcpy_s( szCtrlInfo + 9, zsizeof( szCtrlInfo ) - 9, cpcBarID );

   TCHAR szBuffer[ zsizeof( "-2147483647," ) * 17 ];

   wsprintf( szBuffer, szToolBarFormat,
             pCB_Info->m_nBarID,
             pCB_Info->m_bVisible,
             pCB_Info->m_bFloating,
             pCB_Info->m_bHorz,
             pCB_Info->m_bDockBar,
             pCB_Info->m_pointPos.x,
             pCB_Info->m_pointPos.y,
             pCB_Info->m_nMRUWidth,
             pCB_Info->m_bDocking,
             pCB_Info->m_uMRUDockID,
             pCB_Info->m_rectMRUDockPos.top,
             pCB_Info->m_rectMRUDockPos.left,
             pCB_Info->m_rectMRUDockPos.bottom,
             pCB_Info->m_rectMRUDockPos.right,
             pCB_Info->m_dwMRUFloatStyle,
             pCB_Info->m_ptMRUFloatPos.x,
             pCB_Info->m_ptMRUFloatPos.y );

   fnSetProfileStr( pZSubtask, cpcIniID, szCtrlInfo, szBuffer );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT void OPERATION
GetProfileStr( zVIEW    vDialog,
               zCPCHAR  cpcIniID,
               zCPCHAR  cpcDlgTag,
               zCPCHAR  cpcWndTag,
               zCPCHAR  cpcStringKey,
               zPCHAR   pchReturnBuffer,
               zLONG    lBufferLth )
{
   zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
   zCHAR  szSection[ 80 ];
   LPAPP pApp;

   if ( vDialog &&
        SfGetApplicationForSubtask( &pApp, vDialog ) == 0 && pApp )
   {
      strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szLocalDir );
   }
   else
      SysGetEnvVar( szZeidonPath, "ZEIDON", zsizeof( szZeidonPath ) );

   SysAppendcDirSep( szZeidonPath );

   wsprintf( szZeidonPath + zstrlen( szZeidonPath ), "%s.ini", cpcIniID );
   wsprintf( szSection, "USR:%s.%s", cpcDlgTag, cpcWndTag );

   GetPrivateProfileString( szSection,        // section name
                            cpcStringKey,     // key name
                            "",               // default string
                            pchReturnBuffer,  // destination buffer
                            lBufferLth,       // size of destination buffer
                            szZeidonPath );   // ini filename
   if ( zstrchr( pchReturnBuffer, '%' ) )
   {
      zLONG lLth = zstrlen( pchReturnBuffer ) + 1;
      zPCHAR pchBuffer = new char[ lLth ];
      strcpy_s( pchBuffer, lLth, pchReturnBuffer );
      SysConvertEnvironmentString( pchReturnBuffer, lBufferLth, pchBuffer );
      delete [] pchBuffer;
   }
}

zOPER_EXPORT void OPERATION
SetProfileStr( zVIEW    vDialog,
               zCPCHAR  cpcIniID,
               zCPCHAR  cpcDlgTag,
               zCPCHAR  cpcWndTag,
               zCPCHAR  cpcStringKey,
               zCPCHAR  cpcString )
{
   zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
   zCHAR  szSection[ 80 ];
   LPAPP pApp;

   if ( vDialog &&
        SfGetApplicationForSubtask( &pApp, vDialog ) == 0 && pApp )
   {
      strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szLocalDir );
   }
   else
      SysGetEnvVar( szZeidonPath, "ZEIDON", zsizeof( szZeidonPath ) );

   SysAppendcDirSep( szZeidonPath );

   wsprintf( szZeidonPath + zstrlen( szZeidonPath ), "%s.ini", cpcIniID );
   wsprintf( szSection, "USR:%s.%s", cpcDlgTag, cpcWndTag );

   WritePrivateProfileString( szSection,        // section name
                              cpcStringKey,     // key name
                              cpcString,        // string to add
                              szZeidonPath );   // ini filename
}

zOPER_EXPORT zLONG OPERATION
GetProfileNbr( zVIEW    vDialog,
               zCPCHAR  cpcIniID,
               zCPCHAR  cpcDlgTag,
               zCPCHAR  cpcWndTag,
               zCPCHAR  cpcIntegerKey )
{
   TCHAR szBuffer[ zsizeof( "-2147483647," ) ];
   zLONG lInteger = 0;

   GetProfileStr( vDialog, cpcIniID, cpcDlgTag, cpcWndTag,
                  cpcIntegerKey, szBuffer, zsizeof( szBuffer ) );
   lInteger = zatol( szBuffer );
   return( lInteger );
}

zOPER_EXPORT void OPERATION
SetProfileNbr( zVIEW    vDialog,
               zCPCHAR  cpcIniID,
               zCPCHAR  cpcDlgTag,
               zCPCHAR  cpcWndTag,
               zCPCHAR  cpcIntegerKey,
               zLONG    lInteger )
{
   TCHAR szBuffer[ zsizeof( "-2147483647," ) ];

   wsprintf( szBuffer, "%ld", lInteger );
   SetProfileStr( vDialog, cpcIniID, cpcDlgTag, cpcWndTag,
                  cpcIntegerKey, szBuffer );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zLONG OPERATION
fnGetProfileNbr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcIntegerKey )
{
   TCHAR szBuffer[ zsizeof( "-2147483647," ) ];
   zLONG lInteger = 0;

   fnGetProfileStr( pZSubtask, cpcIniID, cpcIntegerKey,
                    szBuffer, zsizeof( szBuffer ) );
   lInteger = zatol( szBuffer );
   return( lInteger );
}

zOPER_EXPORT void OPERATION
fnSetProfileNbr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcIntegerKey,
                 zLONG    lInteger )
{
   TCHAR szBuffer[ zsizeof( "-2147483647," ) ];

   wsprintf( szBuffer, "%ld", lInteger );
   fnSetProfileStr( pZSubtask, cpcIniID, cpcIntegerKey, szBuffer );
}

zOPER_EXPORT void OPERATION
fnGetProfileStr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcStringKey,
                 zPCHAR   pchReturnBuffer,
                 zLONG    lBufferLth )
{
   GetProfileStr( pZSubtask->m_vDialog, cpcIniID,
                  *(pZSubtask->m_pzsDlgTag),
                  *(pZSubtask->m_pzsWndTag),
                  cpcStringKey, pchReturnBuffer, lBufferLth );
}

zOPER_EXPORT void OPERATION
fnSetProfileStr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcStringKey,
                 zCPCHAR  cpcString )
{
   SetProfileStr( pZSubtask->m_vDialog, cpcIniID,
                  *(pZSubtask->m_pzsDlgTag),
                  *(pZSubtask->m_pzsWndTag),
                  cpcStringKey, cpcString );
}

//./ ADD NAME=GetWindowPreferenceString
// Source Module=zdrutil.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowPreferenceString
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To get the specified preference (profile) string
//
//  PARAMETERS: vSubtask    - The subtask view for the window for which
//                            the string is requested.
//              cpcStringKey - key name for the desired string
//              pchReturnString - pointer to the return buffer
//              lBufferLth  - maximum length of the return string
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT void OPERATION
GetWindowPreferenceString( zVIEW   vSubtask,
                           zCPCHAR cpcStringKey,
                           zPCHAR  pchReturnString,
                           zLONG   lBufferLth )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
      fnGetProfileStr( pZSubtask, "ZDR", cpcStringKey,
                       pchReturnString, lBufferLth );
   else
      *pchReturnString = 0;
}

//./ ADD NAME=GetWindowPreferenceString
// Source Module=zdrutil.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetWindowPreferenceString
//                                              05/01/98 Modified: 05/01/98
//
//  PURPOSE:   To set the specified preference (profile) string
//
//  PARAMETERS: vSubtask    - The subtask view for the window for which
//                            the string is requested.
//              cpcStringKey - key name for the desired string
//              cpcString   - pointer to buffer containing value to be set
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT void OPERATION
SetWindowPreferenceString( zVIEW   vSubtask,
                           zCPCHAR cpcStringKey,
                           zCPCHAR cpcString )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
      fnSetProfileStr( pZSubtask, "ZDR", cpcStringKey, cpcString );
}

//./ ADD NAME=GetWindowPreferenceInteger
// Source Module=zdrutil.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetWindowPreferenceInteger
//                                              05/01/98 Modified: 05/01/98
//
//  PURPOSE:   To get the specified preference (profile) integer
//
//  PARAMETERS: vSubtask    - The subtask view for the window for which
//                            the integer is requested.
//              cpcIntegerKey - key name for the desired integer
//
//  RETURNS:   value - if found in preferences
//             otherwise - 0
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
GetWindowPreferenceInteger( zVIEW   vSubtask,
                            zCPCHAR cpcIntegerKey )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
      return( fnGetProfileNbr( pZSubtask, "ZDR", cpcIntegerKey ) );
   else
      return( 0 );
}

//./ ADD NAME=SetWindowPreferenceInteger
// Source Module=zdrutil.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetWindowPreferenceInteger
//                                              05/01/98 Modified: 05/01/98
//
//  PURPOSE:   To set the specified preference (profile) integer
//
//  PARAMETERS: vSubtask    - The subtask view for the window for which
//                            the string is requested.
//              cpcStringKey - key name for the desired integer
//              lInteger    - integer containing value to set
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT void OPERATION
SetWindowPreferenceInteger( zVIEW   vSubtask,
                            zCPCHAR cpcIntegerKey,
                            zLONG   lInteger )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
      fnSetProfileNbr( pZSubtask, "ZDR", cpcIntegerKey, lInteger );
}

zOPER_EXPORT zLONG OPERATION
GetRegisteredMessage( zCPCHAR cpcMsg )
{
// if ( zstrcmp( szlPostedClose, cpcMsg ) == 0 )  so far the only one
      return( g_uMsgPostedClose );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// How do I get my MFC app to use the Registry on Win32 platforms?
// Just make a call to SetRegistryKey( "MyCompany" ) in your CWinApp
// class and pass it a string (typically your company name). This
// string will define where in the registry the data goes:
// HKEY_CURRENT_USER\Software\MyCompany\\\
//
// After making this call just use the normal WriteProfilexxx()
// routines and the data will go to the registry instead of to an
// .INI file.  It works well and is simple!
//
/////////////////////////////////////////////////////////////////////////////

#if 0

A number representing one of the VER_NT_xxxx flags defined in Winnt.h,
such as:

   0x0000001 (VER_NT_WORKSTATION)
   0x0000002 (VER_NT_DOMAIN_CONTROLLER)
   0x0000003 (VER_NT_SERVER)

When this information is included, the INF will be used only if the
operating system matches the specified product type. If the INF supports
multiple product types for a single operating system version, multiple
TargetOSVersion entries are required.

  SuiteMask - A number representing a combination of one or more of the
              VER_SUITE_xxxx flags defined in Winnt.H. These flags include:

   0x00000001 (VER_SUITE_SMALLBUSINESS)
   0x00000002 (VER_SUITE_ENTERPRISE)
   0x00000004 (VER_SUITE_BACKOFFICE)
   0x00000008 (VER_SUITE_COMMUNICATIONS)
   0x00000010 (VER_SUITE_TERMINAL)
   0x00000020 (VER_SUITE_SMALLBUSINESS_RESTRICTED)
   0x00000040 (VER_SUITE_EMBEDDEDNT)
   0x00000080 (VER_SUITE_DATACENTER)
   0x00000100 (VER_SUITE_SINGLEUSERTS)
   0x00000200 (VER_SUITE_PERSONAL)
   0x00000400 (VER_SUITE_SERVERAPPLIANCE)

  OSVERSIONINFOEX

  The OSVERSIONINFOEX structure contains operating system version information.
  The information includes major and minor version numbers, a build number, a
  platform identifier, and information about product suites and the latest
  Service Pack installed on the system. This structure is used with the
  GetVersionEx and VerifyVersionInfo functions.

  typedef struct _OSVERSIONINFOEX
  {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    TCHAR szCSDVersion[ 128 ];
    WORD wServicePackMajor;
    WORD wServicePackMinor;
    WORD wSuiteMask;
    BYTE wProductType;
    BYTE wReserved;
  } OSVERSIONINFOEX, *POSVERSIONINFOEX, *LPOSVERSIONINFOEX;

  Members

  dwOSVersionInfoSize
       Specifies the size, in bytes, of this data structure. Set this member
       to zsizeof(OSVERSIONINFOEX) before calling the GetVersionEx or
       VerifyVersionInfo function.
  dwMajorVersion
       Identifies the major version number of the operating system as follows.
        Operating System    Value
        Windows 95            4
        Windows 98            4
        Windows Me            4
        Windows NT 3.51       3
        Windows NT 4.0        4
        Windows 2000          5
        Windows XP            5
        Windows .NET Server   5

  dwMinorVersion
       Identifies the minor version number of the operating system as follows.
        Operating System    Value
        Windows 95            0
        Windows 98           10
        Windows Me           90
        Windows NT 3.51      51
        Windows NT 4.0        0
        Windows 2000          0
        Windows XP            1
        Windows .NET Server   1

  dwBuildNumber
       Identifies the build number of the operating system.
  dwPlatformId
       Identifies the operating system platform. This member can be
       VER_PLATFORM_WIN32_NT.
  szCSDVersion
       Contains a null-terminated string, such as "Service Pack 3", that
       indicates the latest Service Pack installed on the system. If no
       Service Pack has been installed, the string is empty.
  wServicePackMajor
       Identifies the major version number of the latest Service Pack
       installed on the system. For example, for Service Pack 3, the major
       version number is 3. If no Service Pack has been installed, the value
       is zero.
  wServicePackMinor
       Identifies the minor version number of the latest Service Pack
       installed on the system. For example, for Service Pack 3, the minor
       version number is 0.
  wSuiteMask
       A set of bit flags that identify the product suites available on the
       system. This member can be a combination of the following values.

               Value                           Meaning
        VER_SUITE_BACKOFFICE  Microsoft BackOffice components are installed.
        VER_SUITE_BLADE       Windows .NET Web Server is installed.
        VER_SUITE_DATACENTER  Windows 2000 or Windows .NET Datacenter Server
                              is installed.
        VER_SUITE_ENTERPRISE  Windows 2000 Advanced Server or Windows .NET
                              Enterprise Server is installed.
        VER_SUITE_PERSONAL    Windows XP Home Edition is installed.
        VER_SUITE_SMALLBUSINESS Microsoft Small Business Server is installed.
        VER_SUITE_SMALLBUSINESS_RESTRICTED  Microsoft Small Business Server
                              is installed with the restrictive client
                              license in force.
        VER_SUITE_TERMINAL    Terminal Services is installed.

  wProductType
       Indicates additional information about the system. This member can be
       one of the following values.

              Value                            Meaning
        VER_NT_WORKSTATION    The system is running Windows NT 4.0 Workstation,
                              Windows 2000 Professional, Windows XP Home
                              Edition, or Windows XP Professional.
        VER_NT_DOMAIN_CONTROLLER The system is a domain controller.
        VER_NT_SERVER         The system is a server.


  wReserved
       Reserved for future use.

  Remarks

  Relying on version information is not the best way to test for a feature.
  Instead, refer to the documentation for the feature of interest. For more
  information on common techniques for feature detection, see Operating
  System Version.

  If you must require a particular operating system, be sure to use it as a
  minimum supported version, rather than design the test for the one operating
  system. This way, your detection code will continue to work on future
  versions of Windows.

  To determine whether a Win32-based application is running on WOW64, call
  the IsWow64Process function.
#endif

/////////////////////////////////////////////////////////////////////////////
//
// DetermineSystemVersion returns:
//
//   0 - unknown
//  10 - Windows 95
//  15 - Windows 95 (OSR2)
//  20 - Windows 98
//  25 - Windows 98 (SE)
//  30 - Windows ME
//  35 - Windows NT (< 4.0)
//  40 - Windows NT 4
//  50 - Windows 2000
//  60 - Windows XP Home
//  65 - Windows XP Professional
//
// 100 - Server
// 101 - Server
// 120 - DataCenter Server
// 130 - Advanced Server
// 131 - Advanced Server
// 140 - Enterprise Server
// 150 - Web Server
// 165 - Professional Server
// 180 - Windows .NET Server
//
// If plMajorVersion is not 0,
//   Identifies the major version number of the operating system as follows:
//       Operating System    Value
//       Windows 95            4
//       Windows 98            4
//       Windows Me            4
//       Windows NT 3.51       3
//       Windows NT 4.0        4
//       Windows 2000          5
//       Windows XP            5
//       Windows .NET Server   5
//
// If plMinorVersion is not 0,
//   Identifies the minor version number of the operating system as follows:
//       Operating System    Value
//       Windows 95            0
//       Windows 98           10
//       Windows Me           90
//       Windows NT 3.51      51
//       Windows NT 4.0        0
//       Windows 2000          0
//       Windows XP            1
//       Windows .NET Server   1
//
// If pchVersion is not 0, the string version of the current OS will be
// returned.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DetermineSystemVersion( zPLONG plMajorVersion,
                        zPLONG plMinorVersion,
                        zPCHAR pchVersion,
                        zLONG  lMaxLth )
{
   OSVERSIONINFOEX osvi;
   BOOL   bOsVersionInfoEx;
   zCHAR  szVersion[ 256 ];
   zSHORT nRC = 0;

   if ( plMajorVersion )
      *plMajorVersion = 0;

   if ( plMinorVersion )
      *plMinorVersion = 0;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.
   zmemset( &osvi, 0, sizeof( OSVERSIONINFOEX ) );
   osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

   if ( (bOsVersionInfoEx = GetVersionEx( (OSVERSIONINFO *) &osvi )) == 0 )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
      if ( GetVersionEx( (OSVERSIONINFO *) &osvi ) == 0 )
      {
         if ( pchVersion )
            strcpy_s( pchVersion, lMaxLth, "unknown" );

         return( nRC );
      }
   }

   szVersion[ 0 ] = 0;
   if ( plMajorVersion )
      *plMajorVersion = osvi.dwMajorVersion;

   if ( plMinorVersion )
      *plMinorVersion = osvi.dwMinorVersion;

   switch ( osvi.dwPlatformId )
   {
      // Tests for Windows NT product family.
      case VER_PLATFORM_WIN32_NT:
      {
         // Test for the product.
         if ( osvi.dwMajorVersion == 3 )
         {
            strcat_s( szVersion, zsizeof( szVersion ), "Microsoft Windows NT " );
            nRC = 35;
         }

         if ( osvi.dwMajorVersion == 4 )
         {
            strcat_s( szVersion, zsizeof( szVersion ), "Microsoft Windows NT " );
            nRC = 40;
         }

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
         {
            strcat_s( szVersion, zsizeof( szVersion ), "Microsoft Windows 2000 " );
            nRC = 50;
         }

         if ( bOsVersionInfoEx )  // use information from GetVersionEx
         {
            // Test for the workstation type.
            if ( osvi.wProductType == VER_NT_WORKSTATION )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
               {
                  strcat_s( szVersion, zsizeof( szVersion ), "Microsoft Windows XP " );
                  if ( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                  {
                     strcat_s( szVersion, zsizeof( szVersion ), "Home Edition " );
                     nRC = 60;
                  }
                  else
                  {
                     strcat_s( szVersion, zsizeof( szVersion ), "Professional " );
                     nRC = 65;
                  }
               }
            }
            else   // test for the server type
            if ( osvi.wProductType == VER_NT_SERVER )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
               {
                  strcat_s( szVersion, zsizeof( szVersion ), "Microsoft Windows .NET " );
                  nRC = 180;
               }

               if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
               {
                  strcat_s( szVersion, zsizeof( szVersion ), "DataCenter Server " );
                  nRC = 120;
               }
               else
               if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
               {
                  if ( osvi.dwMajorVersion == 4 )
                  {
                     strcat_s( szVersion, zsizeof( szVersion ), "Advanced Server " );
                     nRC = 130;
                  }
                  else
                  {
                     strcat_s( szVersion, zsizeof( szVersion ), "Enterprise Server " );
                     nRC = 140;
                  }
               }
               else
#ifdef VER_SUITE_SERVERAPPLIANCE
               if ( osvi.wSuiteMask == VER_SUITE_SERVERAPPLIANCE )
               {
                  strcat_s( szVersion, zsizeof( szVersion ), "Web Server " );
                  nRC = 150;
               }
               else
#endif
               {
                  strcat_s( szVersion, zsizeof( szVersion ), "Server " );
                  nRC = 100;
               }
            }
         }
         else   // Use the registry on early versions of Windows NT.
         {
            HKEY hKey;
            zCHAR szProductType[ 80 ];
            DWORD dwBufLen;

            RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                          "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
                          0, KEY_QUERY_VALUE, &hKey );
            RegQueryValueEx( hKey, "ProductType", NULL, NULL,
                             (LPBYTE) szProductType, &dwBufLen);
            RegCloseKey( hKey );
            if ( zstricmp( "WINNT", szProductType ) == 0 )
            {
               strcat_s( szVersion, zsizeof( szVersion ), "Professional " );
               nRC = 165;
            }

            if ( zstricmp( "LANMANNT", szProductType ) == 0 )
            {
               strcat_s( szVersion, zsizeof( szVersion ), "Server " );
               nRC = 101;
            }

            if ( zstricmp( "SERVERNT", szProductType ) == 0 )
            {
               strcat_s( szVersion, zsizeof( szVersion ), "Advanced Server " );
               nRC = 131;
            }
         }

         // Display version, service pack (if any), and build number.
         zLONG lLth = zstrlen( szVersion );
         if ( osvi.dwMajorVersion <= 4 )
         {
            sprintf_s( szVersion + lLth, zsizeof( szVersion ) - lLth,
                      "Version %d.%d %s (Build %d)",
                      osvi.dwMajorVersion,
                      osvi.dwMinorVersion,
                      osvi.szCSDVersion,
                      osvi.dwBuildNumber & 0xFFFF );
         }
         else
         {
            sprintf_s( szVersion + lLth, zsizeof( szVersion ) - lLth,
                      "%s (Build %d)",
                      osvi.szCSDVersion,
                      osvi.dwBuildNumber & 0xFFFF );
         }

         break;
      }

      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
      {
         if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0 )
         {
             strcpy_s( szVersion, zsizeof( szVersion ), "Microsoft Windows 95 " );
             if ( osvi.szCSDVersion[ 1 ] == 'C' ||
                  osvi.szCSDVersion[ 1 ] == 'B' )
             {
                nRC = 15;
                strcat_s( szVersion, zsizeof( szVersion ), "OSR2 " );
             }
             else
             {
                nRC = 10;
             }
         }

         if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10 )
         {
             strcpy_s( szVersion, zsizeof( szVersion ), "Microsoft Windows 98 " );
             if ( osvi.szCSDVersion[ 1 ] == 'A' )
             {
                strcat_s( szVersion, zsizeof( szVersion ), "SE " );
                nRC = 25;
             }
             else
             {
                nRC = 20;
             }
         }

         if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90 )
         {
             strcpy_s( szVersion, zsizeof( szVersion ), "Microsoft Windows Millennium Edition " );
             nRC = 30;
         }

         break;
      }
   }

   if ( pchVersion )
   {
      strcpy_s( pchVersion, lMaxLth, szVersion );
   }

// TraceLineS( "DetermineSystemVersion: ", szVersion );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// ActiveDirectoryLoginAuthentication
//
//
// ADsOpenObject Function
//
// The ADsOpenObject function binds to an ADSI object using explicit username and password credentials.ADsOpenObject is a wrapper
// function for IADsOpenDSObject and is equivalent to the IADsOpenDSObject::OpenDsObject method.
//
// Syntax
//
// HRESULT ADsOpenObject( __in   LPWSTR lpszPathName,
//                        __in   LPWSTR lpszUserName,
//                        __in   LPWSTR lpszPassword,
//                        __in   DWORD dwReserved,
//                        __in   REFIID riid,
//                        __out  VOID **ppObject );
//
// Parameters
// lpszPathName [in]
// The null-terminated Unicode string that specifies the ADsPath of the ADSI object. For more information and code examples
// of binding strings for this parameter, see LDAP ADsPath and WinNT ADsPath.
//
// lpszUserName [in]
// The null-terminated Unicode string that specifies the user name to supply to the directory service to use for credentials.
// This string should always be in the format "<domain>\<user name>" to avoid ambiguity. For example, if DomainA and DomainB
// have a trust relationship and both domains have a user with the name "user1", it is not possible to predict which domain
// ADsOpenObject will use to validate "user1".
//
// lpszPassword [in]
// The null-terminated Unicode string that specifies the password to supply to the directory service to use for credentials.
//
// dwReserved [in]
// Provider-specific authentication flags used to define the binding options. For more information, see ADS_AUTHENTICATION_ENUM.
//
// riid [in]
// Interface identifier for the requested interface on this object.
//
// ppObject [out]
// Pointer to a pointer to the requested interface.
//
// Return Value
// This method supports the standard HRESULT return values, including the following.
//
// For more information, see ADSI Error Codes.
//
// Return code Description
// S_OK
//  Binding to the specified object succeeded.
//
//
// Remarks
// This function should not be used just to validate user credentials. For more information about validating user credentials,
// see Microsoft Knowledge Base article 180548 HOWTO: Validate User Credentials on Microsoft Operating Systems.
//
// A C/C++ client calls the ADsOpenObject helper function to bind to an ADSI object, using the user name and password supplied
// as credentials for the appropriate directory service. If lpszUsername and lpszPassword are NULL and ADS_SECURE_AUTHENTICATION
// is set, ADSI binds to the object using the security context of the calling thread, which is either the security context of
// the user account under which the application is running or of the client user account that the calling thread impersonates.
//
// The credentials passed to the ADsOpenObject function are used only with the particular object bound to and do not affect the
// security context of the calling thread. This means that, in the example below, the call to ADsOpenObject will use different
// credentials than the call to ADsGetObject.
//
//
// Copy Code
// HRESULT hr;
// IADs *padsRoot1;
// IADs *padsRoot2;
//
// hr = ADsOpenObject(L"LDAP://rootDSE",
//     pwszUsername,
//     pwszPassword,
//     ADS_SECURE_AUTHENTICATION,
//     IID_IADs,
//     (LPVOID*)&padsRoot1);
//
// hr = ADsGetObject(L"LDAP://rootDSE",
//     IID_IADs,
//     (LPVOID*)&padsRoot2);
//
// The ADsOpenObject function ignores the dwReserved flag when attempting to bind to an NDS or NWCOMPAT object.
//
// To work with the WinNT: provider, you can pass in lpszUsername as one of the following strings:
//
//
// The name of a user account, that is, "jeffsmith".
// The Windows NT 4.0 style user name, that is, "Fabrikam\jeffsmith".
// With the LDAP provider for Active Directory, you may pass in lpszUsername as one of the following strings:
//
//
// The name of a user account, such as "jeffsmith". To use a user name by itself, you must set only the ADS_SECURE_AUTHENTICATION
// flag in the dwReserved parameter.
// The user path from a previous version of Windows NT, such as "Fabrikam\jeffsmith".
// Distinguished Name, such as "CN=Jeff Smith,OU=Sales,DC=Fabrikam,DC=Com". To use a DN, the dwReserved parameter must be zero or
// it must include the ADS_USE_SSL flag.
// User Principal Name (UPN), such as "jeffsmith@Fabrikam.com". To use a UPN, assign the appropriate UPN value for the
// userPrincipalName attribute of the target user object.
// If Kerberos authentication is required for the successful completion of a specific directory request using the LDAP provider,
// the lpszPathName binding string must use either a serverless ADsPath, such as "LDAP://CN=Jeff Smith,CN=admin,DC=Fabrikam,DC=com",
// or it must use an ADsPath with a fully-qualified DNS server name, such as:
//    "LDAP://central3.corp.Fabrikam.com/CN=Jeff Smith,CN=admin,DC=Fabrikam,DC=com".
// Binding to the server using a flat NETBIOS name or a short DNS name, for example, using the short name "central3" instead of
// "central3.corp.Fabrikam.com", may or may not yield Kerberos authentication.
//
// The following code example shows how to bind to a directory service object with the requested user credentials.
//
//
// Copy Code
// IADs *pObject;
// LPWSTR szUsername = NULL;
// LPWSTR szPassword = NULL
// HRESULT hr;
//
// Insert code to securely retrieve the username and password.
//
// hr = ADsOpenObject(L"LDAP://CN=Jeff,DC=Fabrikam,DC=com",
//                    "jeffsmith",
//                    "etercespot",
//                    ADS_SECURE_AUTHENTICATION,
//                    IID_IADs,
//                    (void**) &pObject);

// Requirements
// Minimum supported client Windows 2000 Professional
// Minimum supported server Windows 2000 Server
// Header Adshlp.h
// Library Activeds.lib
// DLL Activeds.dll
//
/////////////////////////////////////////////////////////////////////////////

#include <comdef.h>
//#include <atlbase.h>
#include <activeds.h>
//#include <stdio.h>
//#include <conio.h>

#include <atlbase.h>
#include <stdio.h>
#include <conio.h>

// KJS 12/02/09 - Taking out the following lines of code because for some reason in AddMember on
// the statement QueryInterface, tomcat and application crashes.  When I take these lines out
// I don't get as many warnings and I don't crash.  Go Figure!!
/*
#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

HRESULT ChangeThePassword(LPWSTR strAdsPath);

const IID IID_IADs = {0xFD8256D0, 0xFD15, 0x11CE, {0xAB,0xC4,0x02,0x60,0x8C,0x9E,0x75,0x53}};
const IID IID_IADsContainer = {0xFD8256D0, 0xFD15, 0x11CE, {0xAB,0xC4,0x02,0x60,0x8C,0x9E,0x75,0x53}};
*/


zOPER_EXPORT zLONG  OPERATION
ActiveDirectoryLoginAuthentication( zCPCHAR cpcPathName,
                                    zCPCHAR cpcUserName,
                                    zCPCHAR cpcPassword )
{
   IADs *pObject;
   CComBSTR strPathName = cpcPathName;
   CComBSTR strUserName = cpcUserName;
   CComBSTR strPassword = cpcPassword;
   CComBSTR strPassword2;
   HRESULT hr;
   zLONG   lRC;

   hr = CoInitialize( NULL );
   if ( FAILED( hr ) )
   {
     TraceLineI( "CoInitialize failed. ", zLONG(hr) );
     //goto error;
   }

   hr = ADsOpenObject( strPathName, strUserName, strPassword,
                       ADS_SECURE_AUTHENTICATION,IID_IADs, (zPPVOID) &pObject );

   if ( SUCCEEDED( hr ) )
   {
      // Use the object.
      lRC = 0;  // success ... user authenticated

     /*
     // This is just temp code.
     // We are trying to figure out how to update a property.
     // If we openobject with zencas/enc-ad then we can seemingly
     // update the "userPassword" or "description" (SetInfo return code is 0) but we don't see the
     // change in active directory.  When I try to update "sn" or many of
     // the other properties, we get an error on the SetInfo (-2147016684).
     // Haven't figured out what that means.
      strPassword2 = OLESTR( "ZencasKelly" );
      hr = pObject->Put( L"sn",(_variant_t)strPassword2 );
      if ( FAILED( hr ) )
         TraceLineS("Put Failed again", "");
     else
         TraceLineS("Put successful", "");

      hr = pObject->SetInfo( );
      if ( FAILED( hr ) )
         TraceLineI("SetInfo Failed", zLONG(hr));
     else
         TraceLineS("SetInfo successful", "");
      */

      // Release the object.
      pObject->Release();

      CoUninitialize();

   }
   else
   {
     TraceLineS("ADSOpenObject Failed", "");
     TraceLineI("ADSOpenObject Failed", zLONG(hr));
     lRC = -1;  // user not found/authenticated
   }

   return( lRC );
}

zOPER_EXPORT zLONG  OPERATION
ActiveDirectoryGetProperty( zCPCHAR cpcPathName,
                            zCPCHAR cpcUserName,
                            zCPCHAR cpcPassword,
                            zCPCHAR cpcPropertyName,
                            zPCHAR  pchReturnString)
{
   IADs *pObject;
   CComBSTR strPathName = cpcPathName;
   CComBSTR strUserName = cpcUserName;
   CComBSTR strPassword = cpcPassword;
   CComBSTR strProperty = cpcPropertyName;
   HRESULT hr;
   zLONG   lRC;

   TraceLineS("In ActiveDirectoryGetProperty", "");

  hr = CoInitialize( NULL );
  if ( FAILED( hr ) )
  {
    TraceLineI( "CoInitialize failed. ", zLONG(hr) );
    //goto error;
  }

   hr = ADsOpenObject( strPathName, strUserName, strPassword,
                       ADS_SECURE_AUTHENTICATION,IID_IADs, (zPPVOID) &pObject );

   if ( SUCCEEDED( hr ) )
   {
      // Use the object.
      lRC = 0;  // success ... user authenticated

     VARIANT var;
      VariantInit(&var);

         TraceLineS("AdsOpenObject successful", "");


      //hr = pObject->Get(strProperty, &var );
      hr = pObject->Get(L"cn", &var );
      if ( FAILED( hr ) )
     {
         TraceLineI("Get Failed", zLONG(hr));
         pObject->Release();
         lRC = -1;
     }
     else
     {

         TraceLineS("Get successful", zPCHAR(&var));
     }

      // Release the object.
      pObject->Release();

      CoUninitialize();
   }
   else
   {
     TraceLineI("ADSOpenObject Failed", zLONG(hr));
     lRC = -1;  // user not found/authenticated
   }

   return( lRC );
}

HRESULT
AddMember( OLECHAR *strPathName, OLECHAR *strLoginUserName, OLECHAR *strLoginPassword,
           OLECHAR *strNewUserName, OLECHAR *strPassword )
{
   HRESULT hr = CoInitialize(NULL);

   IADsContainer *pCont = NULL;
   IADsUser      *pUser = NULL;
   //IADs *pADs = NULL;
   CComBSTR      strUserName;

   // Create our connection string
   strUserName = OLESTR( "cn=" );
   strUserName.Append( strNewUserName );
   //strPathName.Append( OLESTR( ":" ) );

   LPWSTR adsPath = L"LDAP://DC=ENC-AD,DC=ENC,DC=EDU";
   LPWSTR usrPass = NULL;
   LPWSTR usrName = NULL;

   // Add code to securely get the username and password or leave
   // as NULL to use the current security context.

   /* Code created for testing.
      hr = ADsOpenObject(strPathName,
                 strLoginUserName,
                 strLoginPassword,
                      0,
                      IID_IADsContainer,
                      (void**)&pCont);

   TraceLineS( "After Adsopen...", "" );
   IDispatch *pDisp;

   hr = pCont->Create(CComBSTR("person"), CComBSTR("cn=Kangaroo"), &pDisp);
   TraceLineI( "After Create.", zLONG(hr) );

   hr = pDisp->QueryInterface(IID_IADs,(void**)&pADs);
    TraceLineS( "After Release1.", "" );
  pCont->Release();
*/
   // KJS Original code on 12/2/09

   hr = ADsOpenObject(strPathName,
                 strLoginUserName,
                 strLoginPassword,
                      ADS_SECURE_AUTHENTICATION,
                      IID_IADsContainer,
                      (void**)&pCont);

   TraceLineS( "After Adsopen...", "" );
   IDispatch *pDisp;

   // KJS 12/02/09 - When I do the following I get a new account but it is under:
   //enc-ad.enc.edu/Kangaroo
   // when I want it to be under:
   //enc-ad.enc.edu/ENC/Users/Students/ENC/Kangaroo
   // not sure how to get that. Our LDAP is LDAP://DC=ENC-AD,DC=ENC,DC=EDU.
   // This shows an example: http://msdn.microsoft.com/en-us/library/aa746483(VS.85).aspx
   // I notice in this example that the ldap is "LDAP://CN=users,DC=fabrikam,DC=com".  Should we
   // have a CN instead of just DCs?  Aadit said we should use OU like OU=ENC,OU=USERS but I can't get that
   // to work.  Now this seemed to work when I stepped thru the code.
   //hr = pCont->Create(CComBSTR("user"), CComBSTR("cn=Kangaroo"), &pDisp);
   hr = pCont->Create(CComBSTR("user"), strUserName, &pDisp);
   TraceLineI( "After Create.", zLONG(hr) );

   pCont->Release();

   hr = pDisp->QueryInterface(IID_IADsUser,(void**)&pUser);

   // Do we need to do the following?
   //hr = pUser->Put(CComBSTR("sn"), CComVariant("JeffSmith"));
   //hr = pUser->Put(CComBSTR("cn"), CComVariant("JeffSmith"));
   //hr = pUser->SetPassword(strPassword);

   // List of AD attributes http://msdn.microsoft.com/en-us/library/cc219752(PROT.13).aspx
   //                       http://msdn.microsoft.com/en-us/library/cc220897(PROT.13).aspx
   // cn This attribute specifies the name that represents an object. It is used to perform searches.
   // distinguishedName This is the same as the distinguished name for an object. Used by Microsoft® Exchange Serve
   // displayName This attribute specifies the display name for an object, usually the combination of the user's first name, middle initial, and last name.
   // givenName Contains the given name (first name) of the user.
   // sn This attribute specifies the family or last name for a user
   // description This attribute specifies the description to display for an object
   // uid This attribute specifies a user ID
   // physicalDeliveryOfficeName
   // ou This attribute specifies the name of the organizational unit. When used as a component of a directory name, it identifies an organizational unit with which the named object is affiliated.


   TraceLineS( "After Query Inferface...", "" );

   pDisp->Release();
   pUser->SetInfo();

   pUser->Release();
   CoUninitialize();
   TraceLineS( "AddMember Completed.", "" );

   return 0;

}

HRESULT
AddMemberDKS( )
{
   HRESULT hr;
   IADsContainer *pCont;
   IDispatch *pDisp=NULL;
   IADs *pUser;

   // Initialize COM before calling any ADSI functions or interfaces.
   CoInitialize( NULL );

   hr = ADsGetObject( L"LDAP://CN=users,DC=fabrikam,DC=com", IID_IADsContainer, (void**) &pCont );

   if ( !SUCCEEDED( hr ) )
   {
      return 0;
   }

   //-----------------
   // Create a user
   //-----------------

   hr = pCont->Create(CComBSTR("user"), CComBSTR("cn=jeffsmith"), &pDisp );

   // Release the container object.
   pCont->Release( );

   if ( !SUCCEEDED( hr ) )
   {
      return 0;
   }

   hr = pDisp->QueryInterface( IID_IADs, (void**) &pUser );

   // Release the dispatch interface.
   pDisp->Release( );

   if ( !SUCCEEDED( hr ) )
   {
      return 0;
   }

   // Commit the object data to the directory.
   pUser->SetInfo( );

   // Release the object.
   pUser->Release( );

   CoUninitialize( );
   return S_OK;
}

HRESULT
AddMemberOrig( OLECHAR *strServerName, OLECHAR *strServerPort, OLECHAR *strOrganization,
               OLECHAR *strMemberName, OLECHAR *strPassword )
{
   HRESULT     hr = NULL;
   IADsContainer *pIADsContainer;
   IADs        *pIADsNewMember;
   IADs     *pObject;
   IADsUser *pUserObject;
   GUID        guid;
   wchar_t     szGUID[39];
   int         cGUIDStrLth;
   CComBSTR    strPathName;
   CComBSTR    strPassword2;
   CComBSTR    strMemberObjectName;
   CComBSTR    strAdminDN;

   // Create our connection string
   strPathName = OLESTR( "LDAP://" );
   strPathName.Append( strServerName );
   strPathName.Append( OLESTR( ":" ) );
   strPathName.Append( strServerPort );
   strPathName.Append( OLESTR( "/o=" ) );
   strPathName.Append( strOrganization );
   strPathName.Append( OLESTR( "/ou=members" ) );

   // Create our Admin DN based on the organiation name
   //strAdminDN = OLESTR( "cn=Administrator, ou=members, o=" );
   //strAdminDN.Append( strOrganization );


   hr = CoInitialize( NULL );
   if ( FAILED( hr ) )
   {
     TraceLineI( "CoInitialize failed. ", zLONG(hr) );
     //goto error;
   }

   // Connect to the LDAP server using an authenticated bind
   strAdminDN = OLESTR( "enc-ad\\zencas" );
   strPathName = OLESTR( "LDAP://CN=Test Z,OU=Infrastructure,OU=Users,OU=ENC,DC=ENC-AD,DC=ENC,DC=EDU" );
   strPassword2 = OLESTR( "Time2Ch@nge" );
   TraceLineS( "Binding to LDAP.", "" );
   //strPathname is name of user to change.
   hr = ADsOpenObject( strPathName, strAdminDN, strPassword2, ADS_SECURE_AUTHENTICATION,IID_IADsContainer,(zPPVOID) &pUserObject  );
  //hr = ADsGetObject(strPathName,IID_IADsUser,(void**) &pUserObject);
   hr = ADsOpenObject( strPathName, strAdminDN, strPassword2, ADS_SECURE_AUTHENTICATION,IID_IADsContainer,(zPPVOID) &pObject  );
   //hr = ADsOpenObject( "LDAP://10.150.0.11", "zencas/enc-ad", L"Time2Ch@nge", 0, IID_IADsContainer,( void** )&pIADsContainer );
   if ( FAILED( hr ) )
   {
      TraceLineI("ADSOpenObject Failed", zLONG(hr));
      return( hr );
   }
   TraceLineS( "ADsOpenObject worked.", "" );

   hr = pUserObject->SetPassword( (BSTR)strPassword );
   if ( FAILED( hr ) )
   {
     TraceLineI( "Put for password failed. ", zLONG(hr) );
     return( hr );
   }
     TraceLineI( "Password changed. ", zLONG(hr) );

/*
   TraceLineS( "Calling SetInfo( ).", "" );
   hr = pObject->SetInfo( );
   if ( FAILED( hr ) )
   {
     TraceLineS( "SetInfo Failed.", "" );
     return( hr );
   }
   else
      TraceLineS( "SetInfo Successful.", "" );
*/
   return( 0 );

      hr = ADsOpenObject( strPathName, strAdminDN, strPassword2, ADS_SECURE_AUTHENTICATION,IID_IADsContainer,( void** )&pIADsContainer  );

   // Generate a GUID and convert it to a text string
   TraceLineS( "Generating a GUID.", "" );
   hr = CoCreateGuid( &guid );
   if ( FAILED( hr ) )
      return( hr );

   cGUIDStrLth = ::StringFromGUID2( guid, szGUID, 39 );
   if ( cGUIDStrLth == 0 )
      return( E_FAIL );

   // Prepend the member name with "cn="
   strMemberObjectName = OLESTR( "cn=" );
   strMemberObjectName.Append( strMemberName );

   // Create the new member
   TraceLineS( "Creating the member ", "" );
   hr = pIADsContainer->Create( L"member", strMemberObjectName, (IDispatch**)&pIADsNewMember );
   if ( FAILED( hr ) )
      return( hr );

   // Complete the transaction
   TraceLineS( "Calling SetInfo( ).", "" );
   hr = pIADsNewMember->SetInfo( );
   if ( FAILED( hr ) )
   {
     TraceLineI( "SetInfo Failed.", zLONG(hr) );
     return( hr );
   }
   else
      TraceLineS( "SetInfo Successful.", "" );

   // Set our required attributes
   TraceLineS( "Saving our attributes.", "" );
   //hr = pIADsNewMember->Put( L"userPassword",(_variant_t)strPassword );
   hr = pIADsNewMember->Put( L"userPassword",(_variant_t)strPassword );
   if ( FAILED( hr ) )
   {
     TraceLineI( "Put for password failed. ", zLONG(hr) );
     return( hr );
   }
   TraceLineS( "Saving our attributes2.", "" );
   hr = pIADsNewMember->Put( L"GUID", (_variant_t)szGUID );


   // Complete the transaction
   TraceLineS( "Calling SetInfo( ).", "" );
   hr = pIADsNewMember->SetInfo( );
   if ( FAILED( hr ) )
   {
     TraceLineS( "SetInfo Failed.", "" );
     return( hr );
   }
   else
      TraceLineS( "SetInfo Successful.", "" );

   // Clean up and return
   pIADsNewMember->Release( );
   pIADsContainer->Release( );

   CoUninitialize();

   return S_OK;
}

zOPER_EXPORT zLONG  OPERATION
ActiveDirectoryAddUser( zCPCHAR cpcPathName, zCPCHAR cpcLoginUserName,
                        zCPCHAR cpcLoginPassword, zCPCHAR cpcNewUserName, zCPCHAR cpcPassword )
{
   OLECHAR strPathName[ 100 ];
   OLECHAR strLoginUserName[ 100 ];
   OLECHAR strLoginPassword[ 100 ];
   OLECHAR strNewUserName[ 100 ];
   OLECHAR strPassword[ 100 ];
   HRESULT hr;
   zLONG   lRC;

   CoInitialize( NULL );

   MultiByteToWideChar( CP_ACP, 0, cpcPathName, -1, strPathName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcLoginUserName, -1, strLoginUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcLoginPassword, -1, strLoginPassword, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcNewUserName, -1, strNewUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcPassword, -1, strPassword, 100 );

   hr = AddMember( strPathName, strLoginUserName, strLoginPassword, strNewUserName, strPassword );
   if ( FAILED( hr ) )
      lRC = -1;
   else
      lRC = 0;

   CoUninitialize( );
   return( lRC );
}

zOPER_EXPORT zLONG  OPERATION
ActiveDirectoryChangePassword( zCPCHAR cpcPathName, zCPCHAR cpcAdminUserName, zCPCHAR cpcAdminPassword,
                               zCPCHAR cpcUserName, zCPCHAR cpcOldPassword, zCPCHAR cpcPassword )
{
   IADsUser *pUser=NULL;
   IDirectorySearch *pSearch;
   OLECHAR strPathName[ 100 ];
   OLECHAR strAdminUserName[ 100 ];
   OLECHAR strAdminPassword[ 100 ];
   OLECHAR strUserName[ 100 ];
   OLECHAR strPassword[ 100 ];
   OLECHAR strOldPassword[ 100 ];
   CComBSTR strSearch;
   LPOLESTR szADsPath = new OLECHAR[MAX_PATH];
   HRESULT hr;
   zLONG   lRC;

   MultiByteToWideChar( CP_ACP, 0, cpcPathName, -1, strPathName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcAdminUserName, -1, strAdminUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcAdminPassword, -1, strAdminPassword, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcUserName, -1, strUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcPassword, -1, strPassword, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcOldPassword, -1, strOldPassword, 100 );

//-2147023570 = 1326  // Invalid domain/username or invalid password in ADSOpenObject
//-2147022651 = 2245  // Password does not meet the complexity requirements (needs number? or already had this password recently)
//-2147024810 = 86    // Current Password Incorrect
//-2147463168 = 20480 // Invalid LDAP
//-2147023569 = ?     // Password cannot be changed at this time (user cannot change within X days after changing it)

   // Error codes for adsi http://www.selfadsi.org/errorcodes.htm

   CoInitialize(NULL);

   hr = S_OK;

   //LDAP://DC=ENC-AD,DC=ENC,DC=EDU
   // Open an ads object using the admin username and password.  We will use this
   // object to get the ads path name for the username for whom we want to change the
   // password.
   // We can not open an ads object to change the password using the user's username.  Because
   // of this we need to get the users AdsPath and use that in the AdsOpenObject method.
    hr = ADsOpenObject(
                  strPathName,
                  strAdminUserName,
                  strAdminPassword,
                  ADS_SECURE_AUTHENTICATION,
                  IID_IDirectorySearch,
                  (void**) &pSearch
               );
    if ( !SUCCEEDED(hr) )
   {
      TraceLineI("ADsOpen for Search object failed. ", zLONG(hr));
       CoUninitialize();
      return zLONG(hr);
   }

    // Search for the user's AdsPath
   ADS_SEARCHPREF_INFO prefInfo[1];
   prefInfo[0].dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
   prefInfo[0].vValue.dwType = ADSTYPE_INTEGER;
   prefInfo[0].vValue.Integer = ADS_SCOPE_SUBTREE;

   hr = pSearch->SetSearchPreference( prefInfo, 1);
   if ( !SUCCEEDED(hr) )
   {
      TraceLineI("SetSearchPreference failed. ", zLONG(hr));
       CoUninitialize();
      return zLONG(hr);
   }

   //Setting the attibutes needs to be retrieved
   LPWSTR pszAttr[] = { L"AdsPath"};

   DWORD dwCount= zsizeof(pszAttr)/sizeof(LPWSTR);
   ADS_SEARCH_HANDLE hSearch;

   // Build a search string based on the passed username.
    strSearch = OLESTR( "(&(objectCategory=Person)(objectclass=User)(sAMAccountName=" );
   strSearch.Append( strUserName);
   strSearch.Append( OLESTR("))"));

   //Executing the search by using the user's username.
   //hr = pSearch->ExecuteSearch(L"(&(objectCategory=Person)(objectclass=User)(sAMAccountName=testz))", pszAttr, dwCount, &hSearch );
   hr = pSearch->ExecuteSearch(strSearch, pszAttr, dwCount, &hSearch );

   if ( !SUCCEEDED(hr) )
   {
      pSearch->Release();
      TraceLineI("Execute Search failed. ", zLONG(hr));
       CoUninitialize();
      return zLONG(hr);
   }
   else
   {
      TraceLineS("ExecuteSearch successful", "");
   }

   ADS_SEARCH_COLUMN col;
   //Searching in the whole collection
   // If no rows where returned then the search was not successful.
   if ( pSearch->GetNextRow(hSearch) == S_ADS_NOMORE_ROWS )
   {
      // Most likely the problem is the username does not exist in AD.
      pSearch->CloseSearchHandle(hSearch);
      pSearch->Release();
       CoUninitialize();
      return (2);
   }
   else
   {
      hr = pSearch->GetColumn( hSearch, pszAttr[0], &col);

      if ( SUCCEEDED(hr) )
      {
         //wsprintf(lpAdsPath  , (LPWSTR)col.pADsValues->CaseIgnoreString);
            wcscpy(szADsPath, col.pADsValues->CaseIgnoreString);
         pSearch->FreeColumn( &col );
      }
      else if ( hr == E_ADS_COLUMN_NOT_SET )
      {
         TraceLineS("Could not do a GetColumn: ", "");
      }
   }
   pSearch->CloseSearchHandle(hSearch);
   pSearch->Release();

   // Open the ads object for the username we want to change the password for.
    hr = ADsOpenObject(
                  szADsPath,
                  strAdminUserName,
                  strAdminPassword,
                  ADS_SECURE_AUTHENTICATION,
                  IID_IADsUser,
                  (void**) &pUser
               );

    if ( !SUCCEEDED(hr) )
   {
      TraceLineI("ADsOpenObject with users AdsPath failed. ", zLONG(hr));
      return zLONG(hr);
   }


   //ChangePassword is the call which will adhere all
   //the Password Policy, if we want to NOT follow the
   //password policies then user SetPassword, however SetPassword
   //is a Admin call

   hr = pUser->ChangePassword(strOldPassword, strPassword);
   //hr = pUser->ChangePassword(L"Kelly1234", L"Aadit1234");

   if(hr == S_OK)
   {
      TraceLineS( "ChangePassword successful.", "" );
       lRC = 0;
   }
   else
   {
      TraceLineI( "ChangePassword Error.. ", zLONG(hr));
       lRC = zLONG(hr);
   }
   pUser->Release();

   CoUninitialize();

   return( lRC );
}

zOPER_EXPORT zLONG  OPERATION
ActiveDirectorySetPassword( zCPCHAR cpcPathName, zCPCHAR cpcAdminUserName,
                            zCPCHAR cpcAdminUserPassword, zCPCHAR cpcUserName, zCPCHAR cpcPassword )
{
   IADsUser *pUser=NULL;
   IDirectorySearch *pSearch;
   OLECHAR strPathName[ 100 ];
   OLECHAR strAdminUserName[ 100 ];
   OLECHAR strAdminPassword[ 100 ];
   OLECHAR strPassword[ 100 ];
   OLECHAR strUserName[ 100 ];
   CComBSTR strSearch;
   LPOLESTR szADsPath = new OLECHAR[MAX_PATH];
   HRESULT hr;
   zLONG   lRC;

   MultiByteToWideChar( CP_ACP, 0, cpcPathName, -1, strPathName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcAdminUserName, -1, strAdminUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcAdminUserPassword, -1, strAdminPassword, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcUserName, -1, strUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcPassword, -1, strPassword, 100 );

   CoInitialize(NULL);

   hr = S_OK;

   //LDAP://DC=ENC-AD,DC=ENC,DC=EDU
   // Open an ads object using the admin username and password.  We will use this
   // object to get the ads path name for the username for whom we want to change the
   // password.
   // We can not open an ads object to change the password using the user's username.  Because
   // of this we need to get the users AdsPath and use that in the AdsOpenObject method.
    hr = ADsOpenObject(
                  strPathName,
                  strAdminUserName,
                  strAdminPassword,
                  ADS_SECURE_AUTHENTICATION,
                  IID_IDirectorySearch,
                  (void**) &pSearch
               );
    if ( !SUCCEEDED(hr) )
   {
      TraceLineI("ADsOpen for Search object failed. ", zLONG(hr));
       CoUninitialize();
      return hr;
   }

    // Search for the user's AdsPath
   ADS_SEARCHPREF_INFO prefInfo[1];
   prefInfo[0].dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
   prefInfo[0].vValue.dwType = ADSTYPE_INTEGER;
   prefInfo[0].vValue.Integer = ADS_SCOPE_SUBTREE;

   hr = pSearch->SetSearchPreference( prefInfo, 1);
   if ( !SUCCEEDED(hr) )
   {
      TraceLineI("SetSearchPreference failed. ", zLONG(hr));
       CoUninitialize();
      return zLONG(hr);
   }

   //Setting the attibutes needs to be retrieved
   LPWSTR pszAttr[] = { L"AdsPath"};

   DWORD dwCount= zsizeof(pszAttr)/sizeof(LPWSTR);
   ADS_SEARCH_HANDLE hSearch;

   // Build a search string based on the passed username.
    strSearch = OLESTR( "(&(objectCategory=Person)(objectclass=User)(sAMAccountName=" );
   strSearch.Append( strUserName);
   strSearch.Append( OLESTR("))"));

   //Executing the search by using the user's username.
   hr = pSearch->ExecuteSearch(strSearch, pszAttr, dwCount, &hSearch );

   if ( !SUCCEEDED(hr) )
   {
      pSearch->Release();
       CoUninitialize();
      TraceLineI("Execute Search failed. ", zLONG(hr));
      return zLONG(hr);
   }
   else
   {
      TraceLineS("ExecuteSearch successful", "");
   }

   ADS_SEARCH_COLUMN col;
   //Searching in the whole collection
   // If no rows where returned then the search was not successful.
   if ( pSearch->GetNextRow(hSearch) == S_ADS_NOMORE_ROWS )
   {
      // Most likely the problem is the username does not exist in AD.
      pSearch->CloseSearchHandle(hSearch);
      pSearch->Release();
       CoUninitialize();
      return (2);
   }
   else
   {
      hr = pSearch->GetColumn( hSearch, pszAttr[0], &col);

      if ( SUCCEEDED(hr) )
      {
         //wsprintf(lpAdsPath  , (LPWSTR)col.pADsValues->CaseIgnoreString);
            wcscpy(szADsPath, col.pADsValues->CaseIgnoreString);
         pSearch->FreeColumn( &col );
      }
      else if ( hr == E_ADS_COLUMN_NOT_SET )
      {
         TraceLineS("Could not do a GetColumn: ", "");
      }
   }
/*
   while( pSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS )
   {
      hr = pSearch->GetColumn( hSearch, pszAttr[0], &col);

      if ( SUCCEEDED(hr) )
      {
         //wsprintf(lpAdsPath  , (LPWSTR)col.pADsValues->CaseIgnoreString);
            wcscpy(szADsPath, col.pADsValues->CaseIgnoreString);
         pSearch->FreeColumn( &col );
      }
      else if ( hr == E_ADS_COLUMN_NOT_SET )
      {
         TraceLineS("Could not do a GetColumn: ", "");
      }
   }
*/
   pSearch->CloseSearchHandle(hSearch);
   pSearch->Release();


   // Open the ads object for the username we want to change the password for.
    hr = ADsOpenObject(
                  szADsPath,
                  strAdminUserName,
                  strAdminPassword,
                  ADS_SECURE_AUTHENTICATION,
                  IID_IADsUser,
                  (void**) &pUser
               );

    if ( !SUCCEEDED(hr) )
   {
      TraceLineI("ADsOpenObject with users AdsPath failed. ", zLONG(hr));
      return zLONG(hr);
   }


   //ChangePassword is the call which will adhere all
   //the Password Policy, if we want to NOT follow the
   //password policies then user SetPassword, however SetPassword
   //is a Admin call

   hr = pUser->SetPassword(strPassword);

   if(hr == S_OK)
   {
      TraceLineS( "SetPassword successful.", "" );
       lRC = 0;
   }
   else
   {
      TraceLineI( "SetPassword Error.. ", zLONG(hr));
       lRC = zLONG(hr);
   }

   pUser->Release();

   CoUninitialize();

   return( lRC );
}

zOPER_EXPORT zLONG  OPERATION
ActiveDirectorySetProperty( zCPCHAR cpcPathName,
                            zCPCHAR cpcAdminUserName,
                            zCPCHAR cpcAdminUserPassword,
                            zCPCHAR cpcUserName,
                            zCPCHAR cpcPropertyName,
                            zCPCHAR cpcPropertyValue )
{
   IADsUser *pUser=NULL;
   IDirectorySearch *pSearch;
   OLECHAR strPathName[ 100 ];
   OLECHAR strAdminUserName[ 100 ];
   OLECHAR strAdminPassword[ 100 ];
   OLECHAR strUserName[ 100 ];
   OLECHAR strPropertyName[ 100 ];
   OLECHAR strPropertyValue[ 100 ];
   CComBSTR strSearch;
   LPOLESTR szADsPath = new OLECHAR[MAX_PATH];
   HRESULT hr;
   zLONG   lRC;

   MultiByteToWideChar( CP_ACP, 0, cpcPathName, -1, strPathName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcAdminUserName, -1, strAdminUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcAdminUserPassword, -1, strAdminPassword, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcUserName, -1, strUserName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcPropertyName, -1, strPropertyName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcPropertyValue, -1, strPropertyValue, 100 );


   // Error codes for adsi http://www.selfadsi.org/errorcodes.htm

   CoInitialize(NULL);

   hr = S_OK;

   //LDAP://DC=ENC-AD,DC=ENC,DC=EDU
   // Open an ads object using the admin username and password.  We will use this
   // object to get the ads path name for the username for whom we want to change the
   // password.
   // We can not open an ads object to change the password using the user's username.  Because
   // of this we need to get the users AdsPath and use that in the AdsOpenObject method.
    hr = ADsOpenObject(
                  strPathName,
                  strAdminUserName,
                  strAdminPassword,
                  ADS_SECURE_AUTHENTICATION,
                  IID_IDirectorySearch,
                  (void**) &pSearch
               );
    if ( !SUCCEEDED(hr) )
   {
      TraceLineI("ADsOpen for Search object failed. ", zLONG(hr));
       CoUninitialize();
      return zLONG(hr);
   }

    // Search for the user's AdsPath
   ADS_SEARCHPREF_INFO prefInfo[1];
   prefInfo[0].dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
   prefInfo[0].vValue.dwType = ADSTYPE_INTEGER;
   prefInfo[0].vValue.Integer = ADS_SCOPE_SUBTREE;

   hr = pSearch->SetSearchPreference( prefInfo, 1);
   if ( !SUCCEEDED(hr) )
   {
      TraceLineI("SetSearchPreference failed. ", zLONG(hr));
       CoUninitialize();
      return zLONG(hr);
   }

   //Setting the attibutes needs to be retrieved
   LPWSTR pszAttr[] = { L"AdsPath"};

   DWORD dwCount= zsizeof(pszAttr)/sizeof(LPWSTR);
   ADS_SEARCH_HANDLE hSearch;

   // Build a search string based on the passed username.
    strSearch = OLESTR( "(&(objectCategory=Person)(objectclass=User)(sAMAccountName=" );
   strSearch.Append( strUserName);
   strSearch.Append( OLESTR("))"));

   //Executing the search by using the user's username.
   //hr = pSearch->ExecuteSearch(L"(&(objectCategory=Person)(objectclass=User)(sAMAccountName=testz))", pszAttr, dwCount, &hSearch );
   hr = pSearch->ExecuteSearch(strSearch, pszAttr, dwCount, &hSearch );

   if ( !SUCCEEDED(hr) )
   {
      pSearch->Release();
      TraceLineI("Execute Search failed. ", zLONG(hr));
       CoUninitialize();
      return zLONG(hr);
   }
   else
   {
      TraceLineS("ExecuteSearch successful", "");
   }

   ADS_SEARCH_COLUMN col;
   //Searching in the whole collection
   // If no rows where returned then the search was not successful.
   if ( pSearch->GetNextRow(hSearch) == S_ADS_NOMORE_ROWS )
   {
      // Most likely the problem is the username does not exist in AD.
      pSearch->CloseSearchHandle(hSearch);
      pSearch->Release();
       CoUninitialize();
      return (2);
   }
   else
   {
      hr = pSearch->GetColumn( hSearch, pszAttr[0], &col);

      if ( SUCCEEDED(hr) )
      {
         //wsprintf(lpAdsPath  , (LPWSTR)col.pADsValues->CaseIgnoreString);
            wcscpy(szADsPath, col.pADsValues->CaseIgnoreString);
         pSearch->FreeColumn( &col );
      }
      else if ( hr == E_ADS_COLUMN_NOT_SET )
      {
         TraceLineS("Could not do a GetColumn: ", "");
      }
   }
   pSearch->CloseSearchHandle(hSearch);
   pSearch->Release();

   // Open the ads object for the username we want to change the password for.
    hr = ADsOpenObject(
                  szADsPath,
                  strAdminUserName,
                  strAdminPassword,
                  ADS_SECURE_AUTHENTICATION,
                  IID_IADsUser,
                  (void**) &pUser
               );

    if ( !SUCCEEDED(hr) )
   {
      TraceLineI("ADsOpenObject with users AdsPath failed. ", zLONG(hr));
      return zLONG(hr);
   }


   //ChangePassword is the call which will adhere all
   //the Password Policy, if we want to NOT follow the
   //password policies then user SetPassword, however SetPassword
   //is a Admin call

   hr = pUser->Put(strPropertyName, (_variant_t)strPropertyValue);

   if(hr == S_OK)
   {
      TraceLineS( "set property successful.", "" );
       lRC = 0;
   }
   else
   {
      TraceLineI( "Set Property Error.. ", zLONG(hr));
       lRC = zLONG(hr);
   }
   pUser->Release();

   CoUninitialize();

   return( lRC );
}

HRESULT
RemoveMember( OLECHAR *strServerName, OLECHAR *strServerPort,
              OLECHAR *strOrganization, OLECHAR *strMemberName )
{
   HRESULT     hr = NULL;
   IADsContainer *pIADsContainer;
   CComBSTR    strPathName;
   CComBSTR    strMemberObjectName;
   CComBSTR    strAdminDN;

   // Create our connection string
   strPathName = OLESTR( "LDAP://" );
   strPathName.Append( strServerName );
   strPathName.Append( OLESTR( ":" ) );
   strPathName.Append( strServerPort );
   strPathName.Append( OLESTR( "/o=" ) );
   strPathName.Append( strOrganization );
   strPathName.Append( OLESTR( "/ou=members" ) );

   // Create our Admin DN based on the organization name
   strAdminDN = OLESTR( "cn=Administrator, ou=members, o=" );
   strAdminDN.Append( strOrganization );

   // Connect to the LDAP server using an authenticated bind
   TraceLineS( "Binding to LDAP.", "" );
   hr = ADsOpenObject( strPathName, strAdminDN, L"password", 0, IID_IADsContainer, (void**)&pIADsContainer );
   if ( FAILED( hr ) )
      return( hr );

   // Prepend the member name with "cn="
   strMemberObjectName = OLESTR( "cn=" );
   strMemberObjectName.Append( strMemberName );

   // Remove the member
   TraceLineS( "Removing the member.", "" );
   hr = pIADsContainer->Delete( L"member", strMemberObjectName );
   if ( FAILED( hr ) )
      return( hr );

   // Clean up and return
   pIADsContainer->Release( );
   return S_OK;
}

zOPER_EXPORT zLONG  OPERATION
ActiveDirectoryRemoveUser( zCPCHAR cpcServerName, zCPCHAR cpcServerPort,
                           zCPCHAR cpcOrganization, zCPCHAR cpcUserName )
{
   OLECHAR strServerName[ 100 ];
   OLECHAR strServerPort[ 100 ];
   OLECHAR strOrganization[ 100 ];
   OLECHAR strMemberName[ 100 ];
   HRESULT hr;
   zLONG   lRC;

   CoInitialize( NULL );

   MultiByteToWideChar( CP_ACP, 0, cpcServerName, -1, strServerName, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcServerPort, -1, strServerPort, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcOrganization, -1, strOrganization, 100 );
   MultiByteToWideChar( CP_ACP, 0, cpcUserName, -1, strMemberName, 100 );

   hr = RemoveMember( strServerName, strServerPort, strOrganization, strMemberName );
   if ( FAILED( hr ) )
      lRC = -1;
   else
      lRC = 0;

   CoUninitialize( );
   return( lRC );
}

#if 0
#define HKEY_CLASSES_ROOT           (( HKEY ) 0x80000000 )
#define HKEY_CURRENT_USER           (( HKEY ) 0x80000001 )
#define HKEY_LOCAL_MACHINE          (( HKEY ) 0x80000002 )
#define HKEY_USERS                  (( HKEY ) 0x80000003 )
#define HKEY_PERFORMANCE_DATA       (( HKEY ) 0x80000004 )
#if(WINVER >= 0x0400)
#define HKEY_CURRENT_CONFIG         (( HKEY ) 0x80000005 )
#define HKEY_DYN_DATA               (( HKEY ) 0x80000006 )
#endif

zCPCHAR cpcSubKey = "mailto\\shell\\open\\command"
RegOpenKeyEx( HKEY_CLASSES_ROOT, cpcSubKey, 0, SECURITY_ACCESS_MASK, &hKey );
RegQueryValueEx( hKey, "", 0, 0, szBuffer, SIZEOF( szBuffer ) );
RegCloseKey( hKey )

<A href="mailto:webmaster@undu.com?subject=This is the Subject Line&body=Here is the message body.">Send Me Some Mail</A>

#endif

zOPER_EXPORT zSHORT OPERATION
DetermineRegKeyValue( zLONG   lKey,
                      zCPCHAR cpcSubKey,
                      zPCHAR  pchReturnValue,
                      zULONG  ulReturnValueLth )
{
   ASSERT( lKey >= 0 && lKey < 7 );
   HKEY hKeyMain = ((HKEY) (lKey | 0x80000000));
   HKEY hKey;

   pchReturnValue[ 0 ] = 0;
   if ( RegOpenKeyEx( hKeyMain, cpcSubKey, 0,
                      KEY_QUERY_VALUE, &hKey ) == ERROR_SUCCESS )
   {
      if ( RegQueryValueEx( hKey, "", 0, 0, (LPBYTE) pchReturnValue,
                            &ulReturnValueLth ) == ERROR_SUCCESS )
      {
         return( 0 );
      }

      RegCloseKey( hKey );
   }

   return( -1 );
}

#if 1

/////////////////////////////////////////////////////////////////////////////
// The MailTo command can do more than enter a single e-mail address in the
// "Send To" field while activating your e-mail program. It can also:
//    Feature       Syntax
// Address message to multiple recipients, (comma separating e-mail addresses)
// Add entry in the "Subject" field  ?subject=Subject Field Text
// Add entry in the "Copy To" or "CC" field  ?&cc=id@internet.node
// Add entry in the "Blind Copy To" or "BCC" field  ?&bcc=id@internet.node
// Add entry in the "Body" field  ?&body=Your message here
//
//Notes:
//
// " " (beginning and ending quotes) are necessary if any spaces are used
//
// Use the ? (question mark) and only one "?", when providing any entry beyond
// e-mail address.
// Some examples, with actual HTML Code included, follow:
//
// Simple MailTo
// <a href="mailto:astark1@unl.edu">
//
// MailTo with Multiple Recipients
// <a href="mailto:astark1@unl.edu,astark@unlserve.unl.edu">
//
// MailTo with Subject
// <a href="mailto:astark1@unl.edu?subject=Comments from MailTo Syntax Page">
//
// MailTo with a Copy
// <a href="mailto:astark1@unl.edu?&cc=astark@unlserve.unl.edu">
//
// MailTo with a Blind Copy
// <a href="mailto:astark1@unl.edu?&bcc=astark@unlserve.unl.edu">
//
// MailTo with message already started in Body
// <a href="mailto:astark1@unl.edu?&body=I am having trouble finding information on ">
//
// MailTo with multiline message in Body
// IMPORTANT NOTE: This feature will NOT work with all e-mail programs, in
// which case the body information appears as a single line.
// <a href="mailto:astark1@unl.edu?&body=message first line. &body=Second line. &body=Third Line. ">
//
// Features may be used in combination
// MailTo with Subject, One Recipient and Two Copies
// <a href="mailto:astark1@unl.edu?subject=MailTo Comments&cc=astark@unlserve.unl.edu,id@internet.node">
//
// Remember to use only one ? (question mark), when providing multiple
// entries beyond e-mail address

// I have seen examples where people say that the attachment works, but none
// of them worked on my machine.  You can try some of these from your browser
// and see if Juno has an attachment.  Make sure that attaching file
// ("C:\ver.txt") exists on your machine.

//  mailto:someone@quinsoft.com?subject=XXXX&body=see attachment&attachment=%22C:\ver.txt%22
//  mailto:someone@quinsoft.com?subject=XXXX&body=see attachment&attach=%22C:\ver.txt%22

// I also saw lots of comments about NOT using "mailto:" because of the
// character limit AND
//   If you want the user to have control over sending and editing the mail,
//   you want a mailto: -- but YOU won't be able to choose what gets attached,
//   they'll have to. You can't choose what's attached anyway -- that would be
//   a huge security hole. And you'll have to live with the limitations of
//   mailto, including the fact that it doesn't work if there's no mail client
//   configured for the browser.
//
// This comment from one discussion thread was interesting:
//   In one of my applications I use a "mailto" URL like the following,
//   to open an e-mail window with a file attached:
//
//   mailto:iudith.m@zim.co.il?subject=my report&body=see attachment&attachment="\\myhost\myfolder\myfile.lis"
//
//   This works ok, except sometimes no attachment
//
//   I don't know whether a solution exists at all for this problem,
//   I also searched the web a lot to find some hints, but it seems
//   that the "attachment" parameter of the "mailto" command is new
//   enough and very fewly discussed at all.
//
//        Space ( ) %20
//        Comma (,) %2C
//        Question Mark   %3F
//        Period (.) %2E
//        Exclamation Point (!) %21
//        Colon (:)  %3A
//        Semicolon (;)  %3B
//
//  see http://www.interlog.com/~tcharron/blat.html for open source solution
//
/////////////////////////////////////////////////////////////////////////////
int
hexEncode( zPCHAR pchTgt, zCHAR c )
{
   if ( c >= 0x80 )
   {
   // *pchTgt = c;
   // *(pchTgt + 1) = 0;
   // TraceLineS( "hexEncode > 0x80: %s", pchTgt );
      *pchTgt = '.';   // I don't know what to do for Mozilla, so punt
#if 0
      // This is what the URN spec says.
      UTF8CodingSystem utf8;
      String<char> bytes( utf8.convertOut( tem ) );
      tem.resize( 0 );
      for ( size_t k = 0; k < bytes.size( ); k++ )
         tem += hexEncode( bytes[ k ] );
#endif
      return( 1 );
   }

// if ( zstrchr( "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$-_.+!*'()%",
   if ( zstrchr( "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$-_.+!*'(),",
                 char( c ) ) )
   {
      *pchTgt = c;  // we have good character
      return( 1 );
   }

   static const zCHAR hexDigits[] = "0123456789ABCDEF";
   pchTgt[ 0 ] = '%';
   pchTgt[ 1 ] = hexDigits[ ((unsigned char) c) >> 4 ];
   pchTgt[ 2 ] = hexDigits[ ((unsigned char) c) & 0xF ];
   pchTgt[ 3 ] = 0;
   return( 3 );
}

zOPER_EXPORT void OPERATION
TranslateEmail( zPCHAR pchTgt, zCPCHAR cpcSrc )
{

   zLONG lSrcIdx = 0;
   zLONG lTgtIdx = 0;

   while ( cpcSrc[ lSrcIdx ] )
   {
#if 1
      lTgtIdx += hexEncode( pchTgt + lTgtIdx, cpcSrc[ lSrcIdx ] );
#else
      switch ( cpcSrc[ lSrcIdx ] )
      {
         case '.':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '2';
            pchTgt[ lTgtIdx++ ] = 'E';
            break;

         case ',':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '2';
            pchTgt[ lTgtIdx++ ] = 'C';
            break;

         case '?':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '3';
            pchTgt[ lTgtIdx++ ] = 'F';
            break;

         case ' ':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '2';
            pchTgt[ lTgtIdx++ ] = '0';
            break;

         case '!':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '2';
            pchTgt[ lTgtIdx++ ] = '1';
            break;

         case ':':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '3';
            pchTgt[ lTgtIdx++ ] = 'A';
            break;

         case ';':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '3';
            pchTgt[ lTgtIdx++ ] = 'B';
            break;

         case '\r':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '0';
            pchTgt[ lTgtIdx++ ] = 'D';
            if ( cpcSrc[ lSrcIdx + 1 ] == '\n' )
               lSrcIdx++;

            break;

         case '\n':
            pchTgt[ lTgtIdx++ ] = '%';
            pchTgt[ lTgtIdx++ ] = '0';
            pchTgt[ lTgtIdx++ ] = 'A';
            if ( cpcSrc[ lSrcIdx + 1 ] == '\r' )
               lSrcIdx++;

            break;

         default:
            pchTgt[ lTgtIdx++ ] = cpcSrc[ lSrcIdx ];
            break;
      }
#endif
      lSrcIdx++;
   }

   pchTgt[ lTgtIdx ] = 0;
}




#if 0

/* ========================================================================== **
 *
 *                                  Escape.c
 *
 * Copyright:
 *  Copyright (C) 2002 by Christopher R. Hertel
 *
 * Email: crh@ubiqx.mn.org
 *
 * $Id: Escape.c,v 0.1 2002/12/06 06:31:01 crh Exp $
 *
 * -------------------------------------------------------------------------- **
 *
 * Description:
 *
 *  This module implements URL escaping/unescaping.
 *
 * -------------------------------------------------------------------------- **
 *
 * License:
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite zTAG_LTH0, Boston, MA 02111-1307 USA
 *
 * -------------------------------------------------------------------------- **
 *
 * Notes:
 *
 * ========================================================================== **
 */
//
//#include "SMB/URL/Escape.h"       /* Module header.                        */
//#include "util/HexOct.h"          /* Support for hex encode/decode.        */


/* -------------------------------------------------------------------------- **
 * Functions:
 */

int smb_urlUnEsc( zPCHAR dst, zCPCHAR src, const int size )
  /* ------------------------------------------------------------------------ **
   * Convert URL escape sequences into single byte values.
   *
   *  Input:  dst   - Target string to which to write the un-escaped result.
   *          src   - Source string possibly containing URL escape sequences.
   *          size  - Number of bytes available in dst[].
   *
   *  Output: If >= 0, the string length of the un-escaped string.  (Eg. same
   *          as the result of strlen(dst) following conversion.)
   *          If < 0, an error code.  (All error codes are negative values.)
   *
   *  Errors: cifs_warnLenExceeded  - The size of the destination string (as
   *                                  provided via the <size> parameter) was
   *                                  too small, and the converted string was
   *                                  truncated.  The result of strlen(dst)
   *                                  will be (size-1).
   *
   *  Notes:  The converted string will always be nul terminated, and the
   *          string length of the converted string will always be less than
   *          or equal to (size - 1).
   *
   *          The two string parameters, <dst> and <src>, may be pointers to
   *          the same memory location without consequence.
   *
   *          We try to be forgiving.  This function converts sequences in
   *          the form %XX to an octet value.  If the second X is not a hex
   *          numeric we convert %X and if the first X is not a hex numeric
   *          we simply copy '%' and continue on.
   *
   *          There are probably many URL management functions written in C
   *          out there in the wide world.  This is a very simple function
   *          which only serves to convert %XX sequences into their correct
   *          octet value.  URLs are more complex than that, however, so
   *          a more powerful suite of functions may be necessary.
   *
   *          Note that this library is under the LGPL.  Do not simply copy
   *          code from a GPL'd URI/URL management package into this library
   *          unless you wish to convert the entire library to the GPL for
   *          your own use.
   *
   * ------------------------------------------------------------------------ **
   */
  {
  int  i;
  int  pos;
  int  val;
  zCHAR scratch[] = "x";

  for ( i = 0; i < size; i++ )
    {
    if( '\0' == *src )
      {
      dst[i] = '\0';
      return( i );
      }

    if( '%' != *src )
      {
      dst[i] = *src;
      src++;
      }
    else
      {
      *scratch = toupper( *(++src) );
      pos = (int)strcspn( util_HexDigits, scratch );
      if( pos < 16 )
        {
        val = pos;
        *scratch = toupper( *(++src) );
        pos = (int)strcspn( util_HexDigits, scratch );
        if( pos < 16 )
          {
          val = (val * 16) + pos;
          src++;
          }
        dst[i] = (char)val;
        }
      else
        dst[i] = '%';
      }
    }

  dst[size-1] = '\0';
  return( cifs_warnLenExceeded );
  } /* smb_urlUnEsc */


/* ========================================================================== */


#endif




#else

/////////////////////////////////////////////////////////////////////////////
//
// URL NOTATION
// Summary of the separator characters and other special notation that can
// appear in a URL.
//
// Special Notation in a URL Notation Meaning
//    ? Appears just after the email address, separating it fromthe first
//      name/value pair.
//    = Separates each name and value, in the form name=value.
//    & Separates name/value pairs, as in name1=value1&name2=value2.
//    % Precedes an ASCII character code in hexadecimal, in the form %xx, as
//      a way of representing that character. For example,%0A represents a
//      newline (line feed) character. See more on hex encoding below.
//    + Another way to represent a space. For example, the value Bjorn Free
//      could appear in the URL as Bjorn+Free.
//
// In JavaScript, you can hex-encode a string - that is, substitute the
// %xx notation for all characters other than letters and numbers (and
// a few special characters) - by using the escape() function, as follows:
//
// var myString = "Odds & ends for tonight!"
// var myEncodedText = escape( myString );
//
// This code puts the following value in the variable myEncodedText:
//
// Odds%20%26%20ends%20for%20tonight%21
//
// Notice that the spaces, ampersand, and exclamation point have been
// converted to their ASCII character code equivalents: %20, %26, and %21,
// respectively. Without hex encoding, this string could not be used as
// a subject value in a URL, because the ampersand would be interpreted
// as a separator between name/value pairs.
//
// Be sure to hex-encode only the subject and message body values in a
// mailto URL. Although the escape() function will not encode letters or
// numbers (or the characters * @ - _ + . /), it will encode the ? and &
// separator characters, which would mess up your URL.
//
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT void OPERATION
TranslateEmail( zPCHAR pchTgt, zCPCHAR cpcSrc )
{
   static zPCHAR Base64Digits =
          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   int nIdx = 0;
   int nLth = strlen( cpcSrc );
   int k;

   for ( k = 0 ; k < nLth ; k += 3 )
   {
      unsigned int uBuffer;
      uBuffer = ((unsigned char *) cpcSrc)[ k ] << 16;
      if ( k + 1 < nLth )
         uBuffer |= ((unsigned char *) cpcSrc)[ k + 1 ] << 8;

      if ( k + 2 < nLth )
         uBuffer |= ((unsigned char *) cpcSrc)[ k + 2 ];

      pchTgt[ nIdx++ ] = Base64Digits[ (uBuffer >> 18) % 64 ];
      pchTgt[ nIdx++ ] = Base64Digits[ (uBuffer >> 12) % 64 ];
      if ( k + 1 < nLth )
         pchTgt[ nIdx++ ] = Base64Digits[ (uBuffer >> 6) % 64 ];
      else
         pchTgt[ nIdx++ ] = '=';

      if ( k + 2 < nLth )
         pchTgt[ nIdx++ ] = Base64Digits[ uBuffer % 64 ];
      else
         pchTgt[ nIdx++ ] = '=';
   }

   pchTgt[ nIdx++ ] = 0;
}

#endif

zPCHAR pchError;
zPCHAR pchShellExecuteErrors[ 100 ] =
{
   "Out of System Resources",
   "OK",
   "(2)File Not Found",            //  SE_ERR_FNF               2
   "(3)Path Not Found",            //  SE_ERR_PNF               3
   "(4)Unknown",
   "(5)Access Denied",             //  SE_ERR_ACCESSDENIED      5
   "(6)Unknown",
   "(7)Unknown",
   "(8)Out of Memory",             //  SE_ERR_OOM               8
   "(11).exe File is Invalid",     //  ERROR_BAD_FORMAT        11
   "(26)Share",                    //  SE_ERR_SHARE            26
   "(27)Association Incomplete",   //  SE_ERR_ASSOCINCOMPLETE  27
   "(28)DDE Timeout",              //  SE_ERR_DDETIMEOUT       28
   "(29)DDE Fail",                 //  SE_ERR_DDEFAIL          29
   "(30)DDE Busy",                 //  SE_ERR_DDEBUSY          30
   "(31)No Association",           //  SE_ERR_NOASSOC          31
   "(32)Dll Not Found",            //  SE_ERR_DLLNOTFOUND      32
};

zSHORT
CopyStringToClipboard( zCPCHAR cpcSource )
{
   int iOK = OpenClipboard( 0 );

   if ( iOK == 0 )
      return( -1 );

   HGLOBAL hClipBuffer;
   zPCHAR  pchBuffer;
   EmptyClipboard( );
   zLONG lLth = zstrlen( cpcSource ) + 1;
   hClipBuffer = GlobalAlloc( GMEM_DDESHARE, lLth );
   pchBuffer = (zPCHAR) GlobalLock( hClipBuffer );
   strcpy_s( pchBuffer, lLth, cpcSource );
   GlobalUnlock( hClipBuffer );
   SetClipboardData( CF_TEXT, hClipBuffer );
   CloseClipboard( );
   return( 0 );
}

zOPER_EXPORT zULONG OPERATION
StartEmailClient( zCPCHAR cpcEmailAddress,  // comma separated list
                  zCPCHAR cpcSubject,
                  zCPCHAR cpcCopyTo,        // comma separated list
                  zCPCHAR cpcBlindCopy,     // comma separated list
                  zCPCHAR cpcBody,
                  zCPCHAR cpcAttachment,
                  zCPCHAR cpcEmailClient,
                  zLONG   lFlags )          // reserved
{
   zLONG lLth = zstrlen( cpcEmailAddress ) + zstrlen( cpcSubject ) +
                zstrlen( cpcCopyTo ) + zstrlen( cpcBlindCopy ) +
                3 * zstrlen( cpcBody ) + zstrlen( cpcAttachment ) +
                zstrlen( cpcEmailClient ) +
                7 + 9 + 4 + 5 + 6 + 8 + 1;
             // mailto: ?subject= &cc= &bcc= &body= &attach=

   zPCHAR  pchCommand = new char[ lLth ];
   zPCHAR  pch;
   zCPCHAR cpcClipBoard = 0;
   zULONG  ulRC = 0;
   zBOOL   bRetry = FALSE;

   while ( TRUE )
   {
      strcpy_s( pchCommand, lLth, "mailto:" );
      strcat_s( pchCommand, lLth, cpcEmailAddress );
      if ( bRetry )
      {
         if ( (pch = zstrchr( pchCommand, ',' )) != 0 )
         {
            *pch = 0;
            cpcClipBoard = cpcEmailAddress;
         }
      }

      strcat_s( pchCommand, lLth, "?subject=" );
      zLONG lCommandLth = zstrlen( pchCommand );
      if ( cpcSubject && *cpcSubject )
         TranslateEmail( pchCommand + lCommandLth, cpcSubject );
      else
         TranslateEmail( pchCommand + lCommandLth, " " );

      if ( cpcCopyTo && *cpcCopyTo )
      {
         if ( bRetry )
            cpcClipBoard = cpcCopyTo;
         else
         {
            strcat_s( pchCommand, lLth, "&cc=" );
            strcat_s( pchCommand, lLth, cpcCopyTo );
         }
      }

      if ( cpcBlindCopy && *cpcBlindCopy )
      {
         if ( bRetry )
            cpcClipBoard = cpcBlindCopy;
         else
         {
            strcat_s( pchCommand, lLth, "&bcc=" );
            strcat_s( pchCommand, lLth, cpcBlindCopy );
         }
      }

      if ( cpcBody && *cpcBody )
      {
         strcat_s( pchCommand, lLth, "&body=" );
         lCommandLth = zstrlen( pchCommand );
         TranslateEmail( pchCommand + lCommandLth, cpcBody );
      // strcat_s( pchCommand, lLth, cpcBody );
      }

   // // Write the merged resulting data in memory out to the file.
   // lCommandLth = zstrlen( pchCommand );
   // zLONG hFileTo = SysOpenFile( g_vRegister, "c:\\temp\\test.html", COREFILE_WRITE );
   // WriteFile( (HANDLE) hFileTo, pchCommand, lCommandLth, &ulRC, 0 );
   // SysCloseFile( g_vRegister, hFileTo, 0 );

      if ( cpcAttachment && *cpcAttachment )
      {
         strcat_s( pchCommand, lLth, "&attach=" );
         strcat_s( pchCommand, lLth, cpcAttachment );
      }

      // 0,"Open","mailto:user@email.com?subject=hello there&body=any text",
      // "","",SW_SHOWNORMAL);
      ulRC = (zULONG) ShellExecute( 0, "Open", pchCommand, "", "", SW_SHOWNORMAL );
      if ( ulRC > 32 )
      {
         pchError = pchShellExecuteErrors[ 1 ];
      }
      else
      {
         if ( ulRC >= 26 )
            pchError = pchShellExecuteErrors[ ulRC - 9 ];
         else
         if ( ulRC >= 2 && ulRC <= 8 )
            pchError = pchShellExecuteErrors[ ulRC ];
         else
         if ( ulRC == 11 )
            pchError = pchShellExecuteErrors[ 9 ];
         else
            pchError = "Unknown";
      }

      TraceLine( "StartEmailClient RC=%d %s   Command(Length:%d)=\"%s\"",
                 ulRC, pchError, zstrlen( pchCommand ), pchCommand );

      if ( bRetry )
      {
         if ( ulRC > 32 && CopyStringToClipboard( cpcClipBoard ) == 0 )
         {
            SysMessageBox( 0, "Error Starting Email Client",
                           "Too many email addresses have been selected.\n"
                             "We've copied the email addresses for you.\n"
                             "To continue, open your email program, create\n"
                             "a New email and in the BCC box, paste the\n"
                             "email addresses via \"Ctrl+V\".", -1 );
         }
         else
         {
            SysMessageBox( 0, "Start Email Client", "Error starting email client", -1 );
         }

         break;
      }
      else
      if ( ulRC <= 32 )
         bRetry = TRUE;
      else
         break;
   }

   delete [] pchCommand;
   return( ulRC );
}

#if 0
zBOOL
IsWindowsNT2000( )
{
   if ( (HIWORD( GetVersion( ) ) & 0x8000) == 0x8000 )
      return( FALSE ); // Under Windows 95/98

   return( TRUE ); // Under Windows NT/2000
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// Determine if we are in NT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zBOOL OPERATION
IsNT( )
{
// SysGetOS_Info( zPLONG plSubVersion )

   OSVERSIONINFO  osvi;

   osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
   GetVersionEx( &osvi );

   if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )
   {
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// Determine if we are operating as a RemoteServer
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zBOOL OPERATION
IsRemoteServer( )
{
#ifdef zREMOTE_SERVER
   return( TRUE );
#else
   return( FALSE );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////// Sorting algorithms from:                     /////////////////
//////////////    Introduction to algorithms                /////////////////
//////////////    by Cormen, Leiserson, Rivest              /////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0

    input   string           sorted      sorted index       heap index
    -----   --------------   ------    ----------------    ------------
      1.    CROCODILE  (4)      4       1  -->   2  (1)     1  --> (16)
      2.    ALLIGATOR  (1)      1       2  -->   4  (2)     2  --> (14)
      3.    CHIMPANZEE (3)      3       3  -->   3  (3)     3  --> (10)
      4.    BEAR       (2)      2       4  -->   1  (4)     4  -->  (8)
      5.    ZEBRA     (16)     10       5  -->  10  (7)     5  -->  (7)
      6.    LION       (9)      7       6  -->   9  (8)     6  -->  (9)
      7.    PEACOCK   (10)      8       7  -->   6  (9)     7  -->  (3)
      8.    TIGER     (14)      9       8  -->   7 (10)     8  -->  (2)
      9.    LAMA       (8)      6       9  -->   8 (14)     9  -->  (4)
     10.    GIRAFFE    (7)      5      10  -->   5 (16)    10  -->  (1)

                                     1 ÚÄÄÄÄÄ¿
                                       ³  4  ³
                                       ÀÄÄÂÄÄÙ
                        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                   2 ÚÄÄÁÄÄ¿                             3 ÚÄÄÁÄÄ¿
                     ³  1  ³                               ³  3  ³
                     ÀÄÄÂÄÄÙ                               ÀÄÄÂÄÄÙ
              ÚÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
         4 ÚÄÄÁÄÄ¿                   5 ÚÄÄÁÄÄ¿  6 ÚÄÄÁÄÄ¿         7 ÚÄÄÁÄÄ¿
           ³  2  ³                     ³ 16  ³    ³  9  ³           ³ 10  ³
           ÀÄÄÂÄÄÙ                     ÀÄÄÂÄÄÙ    ÀÄÄÄÄÄÙ           ÀÄÄÄÄÄÙ
      ÚÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿           ÚÄÄÄÄÄÄÄÙ
 8 ÚÄÄÁÄÄ¿       9 ÚÄÄÁÄÄ¿  10 ÚÄÄÁÄÄ¿
   ³ 14  ³         ³  8  ³     ³  7  ³
   ÀÄÄÄÄÄÙ         ÀÄÄÄÄÄÙ     ÀÄÄÄÄÄÙ

     #1)  1   2   3   4   5   6   7   8   9  10
     #1)  4   1   3   2  16   9  10  14   8   7

                                     1 ÚÄÄÄÄÄ¿
                                       ³  4  ³
                                       ÀÄÄÂÄÄÙ
                        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                   2 ÚÄÄÁÄÄ¿                             3 ÚÄÄÁÄÄ¿
                     ³  1  ³                               ³  3  ³
                     ÀÄÄÂÄÄÙ                               ÀÄÄÂÄÄÙ
              ÚÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
         4 ÚÄÄÁÄÄ¿                   5 ÚÄÄÁÄÄ¿  6 ÚÄÄÁÄÄ¿         7 ÚÄÄÁÄÄ¿
           ³  2  ³                     ³ 16  ³    ³  9  ³           ³ 10  ³
           ÀÄÄÂÄÄÙ                     ÀÄÄÂÄÄÙ    ÀÄÄÄÄÄÙ           ÀÄÄÄÄÄÙ
      ÚÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿           ÚÄÄÄÄÄÄÄÙ
 8 ÚÄÄÁÄÄ¿       9 ÚÄÄÁÄÄ¿  10 ÚÄÄÁÄÄ¿
   ³ 14  ³         ³  8  ³     ³  7  ³
   ÀÄÄÄÄÄÙ         ÀÄÄÄÄÄÙ     ÀÄÄÄÄÄÙ

     #2)  1   2   3   4   5   6   7   8   9  10
     #2)  4   1   3   2  16   9  10  14   8   7

                                     1 ÚÄÄÄÄÄ¿
                                       ³  4  ³
                                       ÀÄÄÂÄÄÙ
                        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                   2 ÚÄÄÁÄÄ¿                             3 ÚÄÄÁÄÄ¿
                     ³  1  ³                               ³  3  ³
                     ÀÄÄÂÄÄÙ                               ÀÄÄÂÄÄÙ
              ÚÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
         4 ÚÄÄÁÄÄ¿                   5 ÚÄÄÁÄÄ¿  6 ÚÄÄÁÄÄ¿         7 ÚÄÄÁÄÄ¿
           ³  14 ³                     ³ 16  ³    ³  9  ³           ³ 10  ³
           ÀÄÄÂÄÄÙ                     ÀÄÄÂÄÄÙ    ÀÄÄÄÄÄÙ           ÀÄÄÄÄÄÙ
      ÚÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿           ÚÄÄÄÄÄÄÄÙ
 8 ÚÄÄÁÄÄ¿       9 ÚÄÄÁÄÄ¿  10 ÚÄÄÁÄÄ¿
   ³  2  ³         ³  8  ³     ³  7  ³
   ÀÄÄÄÄÄÙ         ÀÄÄÄÄÄÙ     ÀÄÄÄÄÄÙ

     #3)  1   2   3   8   5   6   7   4   9  10
     #3)  4   1   3  14  16   9  10   2   8   7

                                     1 ÚÄÄÄÄÄ¿
                                       ³  4  ³
                                       ÀÄÄÂÄÄÙ
                        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                   2 ÚÄÄÁÄÄ¿                             3 ÚÄÄÁÄÄ¿
                     ³  1  ³                               ³  10 ³
                     ÀÄÄÂÄÄÙ                               ÀÄÄÂÄÄÙ
              ÚÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
         4 ÚÄÄÁÄÄ¿                   5 ÚÄÄÁÄÄ¿  6 ÚÄÄÁÄÄ¿         7 ÚÄÄÁÄÄ¿
           ³ 14  ³                     ³ 16  ³    ³  9  ³           ³  3  ³
           ÀÄÄÂÄÄÙ                     ÀÄÄÂÄÄÙ    ÀÄÄÄÄÄÙ           ÀÄÄÄÄÄÙ
      ÚÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿           ÚÄÄÄÄÄÄÄÙ
 8 ÚÄÄÁÄÄ¿       9 ÚÄÄÁÄÄ¿  10 ÚÄÄÁÄÄ¿
   ³  2  ³         ³  8  ³     ³  7  ³
   ÀÄÄÄÄÄÙ         ÀÄÄÄÄÄÙ     ÀÄÄÄÄÄÙ

     #4)  1   2   7   8   5   6   3   4   9  10
     #4)  4   1  10  14  16   9   3   2   8   7

                                     1 ÚÄÄÄÄÄ¿
                                       ³  4  ³
                                       ÀÄÄÂÄÄÙ
                        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                   2 ÚÄÄÁÄÄ¿                             3 ÚÄÄÁÄÄ¿
                     ³ 16  ³                               ³ 10  ³
                     ÀÄÄÂÄÄÙ                               ÀÄÄÂÄÄÙ
              ÚÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
         4 ÚÄÄÁÄÄ¿                   5 ÚÄÄÁÄÄ¿  6 ÚÄÄÁÄÄ¿         7 ÚÄÄÁÄÄ¿
           ³ 14  ³                     ³  7  ³    ³  9  ³           ³  3  ³
           ÀÄÄÂÄÄÙ                     ÀÄÄÂÄÄÙ    ÀÄÄÄÄÄÙ           ÀÄÄÄÄÄÙ
      ÚÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿           ÚÄÄÄÄÄÄÄÙ
 8 ÚÄÄÁÄÄ¿       9 ÚÄÄÁÄÄ¿  10 ÚÄÄÁÄÄ¿
   ³  2  ³         ³  8  ³     ³  1  ³
   ÀÄÄÄÄÄÙ         ÀÄÄÄÄÄÙ     ÀÄÄÄÄÄÙ

     #5)  1   5   7   8  10   6   3   4   9   2
     #5)  4  16  10  14   7   9   3   2   8   1

                                     1 ÚÄÄÄÄÄ¿
                                       ³ 16  ³
                                       ÀÄÄÂÄÄÙ
                        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                   2 ÚÄÄÁÄÄ¿                             3 ÚÄÄÁÄÄ¿
                     ³ 14  ³                               ³ 10  ³
                     ÀÄÄÂÄÄÙ                               ÀÄÄÂÄÄÙ
              ÚÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
         4 ÚÄÄÁÄÄ¿                   5 ÚÄÄÁÄÄ¿  6 ÚÄÄÁÄÄ¿         7 ÚÄÄÁÄÄ¿
           ³  8  ³                     ³  7  ³    ³  9  ³           ³  3  ³
           ÀÄÄÂÄÄÙ                     ÀÄÄÂÄÄÙ    ÀÄÄÄÄÄÙ           ÀÄÄÄÄÄÙ
      ÚÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿           ÚÄÄÄÄÄÄÄÙ
 8 ÚÄÄÁÄÄ¿       9 ÚÄÄÁÄÄ¿  10 ÚÄÄÁÄÄ¿
   ³  2  ³         ³  4  ³     ³  1  ³
   ÀÄÄÄÄÄÙ         ÀÄÄÄÄÄÙ     ÀÄÄÄÄÄÙ

     #6)   5   8   7   9  10   6   3   4   1   2
     #6)  16  14  10   8   7   9   3   2   4   1

char *szTest[ ] = { "CROCODILE", "ALLIGATOR", "CHIMPANZEE",
                    "BEAR", "ZEBRA", "LION", "PEACOCK",
                    "TIGER", "LAMA", "GIRAFFE", ""  };
zLONG plTest[ ] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 0 };


HeapSort:
Heapify is an important routine for manipulating heaps.  When Heapify
is called, it is assumed that the binary trees rooted at Left( k ) and
Right( k ) are heaps, but that A[ k ] may be smaller than its children,
thus violating the heap property (for every node k other than the root,
A[ Parent( k ) ] >= A[ k ] ... that is, the value of a node is at most
the value of its parent).  The Heapify algorithm is:
   Heapify( A, k )
   1. l <- Left( k )
   2. r <- Right( k )
   3. if l <= heap-size[ A ] and A[ l ] > A[ k ]
         then largest <- l
         else largest <- k
   4. if r <= heap-size[ A ] and A[ r ] > A[ largest ]
         then largest <- r
   5. if largest != k
   6.   then exchange A[ k ] <--> A[ largest ]
   7.        Heapify( A, largest )

The BuildHeap procedure produces a heap from an unordered input array:
   BuildHeap( A )
   1. heap-size[ A ] <- length[ A ]
   2. for k <- lowerbound( length[ A ] / 2 ) downto 1
   3.    do Heapify( A, k )

The HeapSort procedure sorts an array in place.  Since the maximum element
of the array is stored at the root A[ 1 ] after BuildHeap, it can be put
into its correct final position by exchanging it with A[ n ]. By discarding
the nth node and restoring the heap property by calling Heapify( A, 1 ),
the procedure may be repeated down to a heap of size 2.  The Heapsort
algorithm is:
   HeapSort( A )
   1. BuildHeap( A )
   2. for k <- length[ A ] downto 2
   3. do exchange A[ 1 ] <--> A[ k ]
   4.    heap-size[ A ] <--> heap-size[ A ] - 1
   5.    Heapify( A, 1 )

QuickSort:
QuickSort is based on a three-step divide-and-conquer process:
   1. Divide: A[ p..r ] is partitioned (rearranged) into two nonempty
      subarrays A[ p..q ] and A[ q + 1..r ] such that each element of
      A[ p..q ] is less than or equal to each element of A[ q + 1..r ].
      The index q is computed as part of this partitioning procedure.
   2. Conquer: The two subarrays A[ p..q ] and A[ q + 1..r ] are sorted
      by recursive calls to quicksort.
   3. Combine: Since the subarrays are sorted in place, no work is
      needed to combine them: the entire array A[ p..r ] is now sorted.

   The Quicksort algorithm is:
   QuickSort( A, p, r )
   1. if p < r
   2. then q <- Partition( A, p, r )
         QuickSort( A, p, q )
         QuickSort( A, q + 1, r )
   To sort an entire array A, call QuickSort( A, 1, length[ A ] )

   The key to the algorithm is the Partition procedure which rearranges
   the subarray A[ p..r ] in place by putting elements smaller than a
   pivot element into the bottom region of the array and elements larger
   than the pivot element into the top region.
   Partition( A, p, r )
   1. x <- A[ p ]
   2. k <- p - 1
   3. j <- r + 1
   4. while TRUE
   5.    do repeat j <- j - 1
               until A[ j ] <= x
   6.       repeat k <- k + 1
               until A[ k ] >= x
   7.       if k < j
               then exchange A[ k ] <--> A[ j ]
               else return j

         ÚÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄ¿
         ³  5  ³  3  ³  2  ³  6  ³  4  ³  1  ³  3  ³  7  ³
         ÀÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÙ
      k                                                     j

         ÚÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄ¿
         ³  5  ³  3  ³  2  ³  6  ³  4  ³  1  ³  3  ³  7* ³
         ÀÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÙ
            k                                   j

         ÚÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄ¿
         ³  3* ³  3  ³  2  ³  6  ³  4  ³  1  ³  5* ³  7* ³
         ÀÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÙ
            k                                   j

         ÚÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄ¿
         ³  3* ³  3* ³  2* ³  6  ³  4  ³  1  ³  5* ³  7* ³
         ÀÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÙ
                              k           j

         ÚÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄ¿
         ³  3* ³  3* ³  2* ³  1* ³  4* ³  6* ³  5* ³  7* ³
         ÀÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÙ
                                    j     k
         ³                             ³                 ³
         ³        A[ p..q ]            ³  A[ q + 1..r ]  ³
         ³                             ³                 ³

         ÚÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄÂÄÄÄÄÄ¿
         ³  3  ³  3  ³  2  ³  1  ³  4  ³  6  ³  5  ³  7  ³
         ÀÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÙ
                                    j     k
#endif

zLONG
Heapify( CArray<zPVOID, zPVOID> *pva,
         zFARPROC_COMPARE lpfnCompare,
         zLONG  lNodeIdx,
         zLONG  lRecordCnt,
         zPVOID pvSortData );
zLONG
QuickSortPartition( CArray<zPVOID, zPVOID> *pva,
                    zFARPROC_COMPARE lpfnCompare,
                    zLONG  lRecord1,
                    zLONG  lRecord2,
                    zPVOID pvSortData );

// #define DEBUG_ALL
#if 0 //def DEBUG_ALL

//////////////////////////// ShowHeap ///////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void
ShowHeap( CArray<zPVOID, zPVOID> *pva,
          zLONG  lRecordCnt,
          zPVOID pvSortData )
{
   void    *pEntity;
   CAttributeData *pAttribute;
   CString cs;
   zPCHAR  pchEntity;
   zPCHAR  pchAttrib;
   zPCHAR  pchAscendDescend;
   zCHAR   szBuffer[ 16 ];
   zLONG   k;

   zPCHAR pchBuffer = new char[ pvSortData.GetLength( ) + 1 ];

   szBuffer[ 0 ] = '\t';
   for ( k = 1; k <= lRecordCnt; k++ )
   {
      ltoa( k, szBuffer + 1, 10 );
      OutputDebugString( szBuffer );
      if ( k < 10 )
         OutputDebugString( ". " );
      else
         OutputDebugString( "." );

      cs = "";
      pEntity = pca->GetAt( k - 1 );
      if ( pEntity )
      {
         strcpy_s( pchBuffer, pvSortData.GetLength( ) + 1, pvSortData );
         pchEntity = pchBuffer;
         while ( *pchEntity && (pchAttrib = zstrchr( pchEntity, '.' )) != 0 )
         {
            cs += " - ";
            *pchAttrib = 0;
            pchAttrib++;
            pchAscendDescend = zstrchr( pchAttrib, ' ' );
            *pchAscendDescend = 0;
            pchAscendDescend++;
            pAttribute = pEntity->FindAttribute( pchAttrib );
            if ( pAttribute )
            {
               cs += pAttribute->GetData( );
            }
         }
      }

      OutputDebugString( cs );
      OutputDebugString( "\n" );
   }

   delete [] pchBuffer;

}  // end of:  ShowHeap

#endif

#if 1
////////////////////////////// BuildHeap ////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void
BuildHeap( CArray<zPVOID, zPVOID> *pva,
           zFARPROC_COMPARE lpfnCompare,
           zLONG  lRecordCnt,
           zPVOID pvSortData )   // non-recursive
{
   zLONG lNodeIdx1;
   zLONG lNodeIdx2;

#if 0 //def DEBUG_ALL
   OutputDebugString( "\nShowing heap prior to build heap ...\n" );
   ShowHeap( pva, lRecordCnt, pvSortData );
#endif

   lNodeIdx1 = lRecordCnt / 2;
   while ( lNodeIdx1 > 0 )
   {
      lNodeIdx2 = lNodeIdx1--;
      do
      {
         lNodeIdx2 = Heapify( pva, lpfnCompare, lNodeIdx2,
                              lRecordCnt, pvSortData );

      } while ( lNodeIdx2 );
   }

#if 0 //def DEBUG_ALL
   OutputDebugString( "\nShowing heap at end of build heap ...\n" );
   ShowHeap( pva, lRecordCnt, pvSortData );
#endif
}  // end of:  BuildHeap

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// When Heapify is called, it is assumed that the binary trees rooted at   //
// Left( k ) and Right( k ) are heaps, but that A[ k ] may be smaller than //
// its children (a violation).  Heapify lets the value of A[ k ] float     //
// down in the heap so that the subtree rooted at index k becomes a heap.  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

////////////////////////////// Heapify //////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
zLONG
Heapify( CArray<zPVOID, zPVOID> *pva,
         zFARPROC_COMPARE lpfnCompare,
         zLONG  lNodeIdx,
         zLONG  lRecordCnt,
         zPVOID pvSortData )
{
   ASSERT( lNodeIdx <= lRecordCnt );
   zLONG lLeft = lNodeIdx << 1;
   zLONG lRight;
   zLONG lLargest;

   if ( lLeft > lRecordCnt )
   {
      return( 0 );
   }

   lLargest = lNodeIdx;
   if ( (*lpfnCompare)( pva, lLeft, lLargest, pvSortData ) > 0 )
   {
      lLargest = lLeft;
   }

   lRight = lLeft + 1;
   if ( lRight <= lRecordCnt )
   {
      if ( (*lpfnCompare)( pva, lRight, lLargest, pvSortData ) > 0 )
      {
         lLargest = lRight;
      }
   }

   if ( lNodeIdx != lLargest )
   {
      zPVOID pv1 = pva->GetAt( lNodeIdx - 1 );
      zPVOID pv2 = pva->GetAt( lLargest - 1 );
      pva->SetAt( lNodeIdx - 1, pv2 );
      pva->SetAt( lLargest - 1, pv1 );

#if 0 //def DEBUG_ALL
      OutputDebugString( "\nShowing heap at end of change to heap ...\n" );
      ShowHeap( pva, lRecordCnt, pvSortData );
#endif
      return( lLargest );
   }

   return( 0 );
}  // end of:  Heapify

#else

void
BuildHeap( CArray<zPVOID, zPVOID> *pva,
           zFARPROC_COMPARE lpfnCompare,
           zLONG  lRecordCnt,   // recursive
           zPVOID pvSortData );
{
   zLONG lNodeIdx;

#if 0 //def DEBUG_ALL
   OutputDebugString( "\nShowing heap prior to build heap ...\n" );
   ShowHeap( pva, lRecordCnt, pvSortData );
#endif

   lNodeIdx = lRecordCnt / 2;
   while ( lNodeIdx > 0 )
   {
      Heapify( lNodeIdx--, pva, lpfnCompare, lRecordCnt, pvSortData );
   }

#ifdef DEBUG_ALL
   OutputDebugString( "\nShowing heap at end of build heap ...\n" );
   ShowHeap( pva, lRecordCnt, pvSortData );
#endif

}  // end of:  BuildHeap

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// When Heapify is called, it is assumed that the binary trees rooted at   //
// Left( k ) and Right( k ) are heaps, but that A[ k ] may be smaller than //
// its children (a violation).  Heapify lets the value of A[ k ] float     //
// down in the heap so that the subtree rooted at index k becomes a heap.  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

////////////////////////////// Heapify //////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void
Heapify( CArray<zPVOID, zPVOID> *pva,
         zFARPROC_COMPARE lpfnCompare,
         zLONG  lNodeIdx,
         zLONG  lRecordCnt,
         zPVOID pvSortData );
{
   ASSERT( lNodeIdx <= lRecordCnt );
   zLONG lLeft = lNodeIdx << 1;
   zLONG lLargest;

   if ( lLeft > lRecordCnt )
   {
      return;
   }

   lLargest = lNodeIdx;
   if ( (*lpfnCompare)( pva, lLeft, lLargest, pvSortData ) > 0 )
   {
      lLargest = lLeft;
   }

   lLeft++;   // lRight (use lLeft to conserve stack space)
   if ( lLeft <= lRecordCnt )
   {
      if ( (*lpfnCompare)( pva, lLeft, lLargest, pvSortData ) > 0 )
      {
         lLargest = lLeft;
      }
   }

   if ( lNodeIdx != lLargest )
   {
      zLONG *p1;
      zLONG *p2;

      p1 = pva->GetAt( lNodeIdx );
      p2 = pva->GetAt( lLargest );
      lLeft = *p1;  // use lLeft to conserve stack space
      *p1 = *p2;
      *p2 = lLeft;

#ifdef DEBUG_ALL
      OutputDebugString( "\nShowing heap at end of change to heap ...\n" );
      ShowHeap( pva, lRecordCnt, pvSortData );
#endif

      Heapify( pva, lLargest, lpfnCompare, lRecordCnt, pvSortData );
   }
}  // end of:  Heapify

#endif

////////////////////////////// HeapSort /////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
HeapSortArray( CArray<zPVOID, zPVOID> *pva,
               zFARPROC_COMPARE lpfnCompare,
               zLONG  lRecordCnt,
               zPVOID pvSortData )
{
   BuildHeap( pva, lpfnCompare, lRecordCnt, pvSortData );

#ifdef DEBUG_ALL
   TraceLineS( "After build heap ...", "" );
   zLONG lHoldCnt = lRecordCnt;
#endif

   zLONG lNodeIdx;

   while ( lRecordCnt > 1 )
   {
      lRecordCnt--;
      zPVOID pv1 = pva->GetAt( lRecordCnt );
      zPVOID pv2 = pva->GetAt( 0 );
      pva->SetAt( lRecordCnt, pv2 );
      pva->SetAt( 0, pv1 );
      lNodeIdx = Heapify( pva, lpfnCompare, 1, lRecordCnt, pvSortData );
      while ( lNodeIdx )
      {
         lNodeIdx = Heapify( pva, lpfnCompare,
                             lNodeIdx, lRecordCnt, pvSortData );
      }
   }

#if 0 //def DEBUG_ALL
   lRecordCnt = lHoldCnt;
   OutputDebugString( "\nAfter heap sort ...\n" );
   ShowHeap( pva, lRecordCnt, pvSortData );
#endif
   return( 0 );
}

/////////////////////////////// QuickSort ///////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
QuickSortArray( CArray<zPVOID, zPVOID> *pva,
                zFARPROC_COMPARE lpfnCompare,
                zLONG  lRecord1,
                zLONG  lRecord2,
                zPVOID pvSortData )    // recursive
{
   static zLONG lRecurseCnt = 0;
   lRecurseCnt++;

#ifdef _DEBUG
   static zLONG lMaxRecurse = 0;
   if ( lMaxRecurse < lRecurseCnt )
   {
      lMaxRecurse = lRecurseCnt;
   }
#endif

   if ( lRecord1 < lRecord2 )
   {
      zLONG k = QuickSortPartition( pva, lpfnCompare,
                                    lRecord1, lRecord2, pvSortData );

      QuickSortArray( pva, lpfnCompare, lRecord1, k, pvSortData );
      QuickSortArray( pva, lpfnCompare, k + 1, lRecord2, pvSortData );
   }

   lRecurseCnt--;

#ifdef DEBUG_ALL
   if ( lRecurseCnt == 0 )
   {
      ASSERT( FALSE );
//    OutputDebugString( "Maximum recurse count = " );
//    ltoa( lMaxRecurse, szBuffer, 10 );
//    OutputDebugString( szBuffer );
//    OutputDebugString( "\n" );
   }
#endif
   return( 0 );
}

/////////////////////////////// QuickSortNR /////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
QuickSortArrayNR( CArray<zPVOID, zPVOID> *pva,
                  zFARPROC_COMPARE lpfnCompare,
                  zLONG  lRecord1,
                  zLONG  lRecord2,
                  zPVOID pvSortData )  // non-recursive
{
   ASSERT( lRecord1 <= lRecord2 );
   zLONG m = lRecord2 - lRecord1;
   zLONG lStackSize = 2;
   while ( m >>= 1 )
   {
      lStackSize++;
   }

   lStackSize <<= 2;     // two entries for each comparison
   zLONG *plStack = (zLONG *) new char[ lStackSize * sizeof( zLONG ) ];
   zLONG lStack = 0;
   zLONG lLeft;
   zLONG lRight;

#ifdef _DEBUG
   zLONG lStackMax = 0;
#endif

   plStack[ lStack++ ] = lRecord1;
   plStack[ lStack++ ] = lRecord2;
   while ( lStack )
   {
      lRight = plStack[ --lStack ];
      lLeft = plStack[ --lStack ];
      m = QuickSortPartition( pva, lpfnCompare, lLeft, lRight, pvSortData );

#ifdef DEBUG_ALL
//??? ShowArray( lLeft, lRight, pvSortData );
#endif

      if ( m + 1 < lRight )
      {
#if 0 //def DEBUG_ALL
         if ( lStack > lStackSize - 2 )
         {
            ASSERT( FALSE );
            delete [] plStack;
            return;
         }
#endif

         plStack[ lStack++ ] = m + 1;
         plStack[ lStack++ ] = lRight;

#if 0 //def DEBUG_ALL
         if ( lStack > lStackMax )
         {
            lStackMax = lStack;
         }
#endif
      }

      if ( lLeft < m )
      {
#if 0 //def _DEBUG
         if ( lStack > lStackSize - 2 )
         {
            ASSERT( FALSE );
            delete [] plStack;
            return( -1 );
         }
#endif

         plStack[ lStack++ ] = lLeft;
         plStack[ lStack++ ] = m;

#if 0 //def _DEBUG
         if ( lStack > lStackMax )
         {
            lStackMax = lStack;
         }
#endif
      }
   }

   delete [] plStack;

#if 0 //def _DEBUG
   ASSERT( FALSE );
#endif
   return( 0 );
}

/////////////////////////////// QuickSortPartition //////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
zLONG
QuickSortPartition( CArray<zPVOID, zPVOID> *pva,
                    zFARPROC_COMPARE lpfnCompare,
                    zLONG  lRecord1,
                    zLONG  lRecord2,
                    zPVOID pvSortData )
{
   zLONG k = lRecord1 - 1;
   zLONG j = lRecord2 + 1;

   while ( TRUE )
   {
      while ( (*lpfnCompare)( pva, --j, lRecord1, pvSortData ) > 0 )
         ;

      while ( (*lpfnCompare)( pva, ++k, lRecord1, pvSortData ) < 0 )
         ;

      if ( k < j )
      {
         zPVOID pv1 = pva->GetAt( k );
         zPVOID pv2 = pva->GetAt( j );
         pva->SetAt( k, pv2 );
         pva->SetAt( j, pv1 );
      }
      else
      {
         return( j );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////// end Sorting algorithms //////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include <tapi.h>

zOPER_EXPORT zLONG OPERATION
DialPhoneNumber( zVIEW   vSubtask,
                 zCPCHAR cpcPhoneNumber,
                 zCPCHAR cpcCalledParty )
{
   LPAPP pApp;

   SfGetApplicationForSubtask( &pApp, vSubtask );
   zLONG lRC = tapiRequestMakeCall( cpcPhoneNumber, pApp->szName,
                                    cpcCalledParty, 0 );
   return( lRC );
}

#ifndef zREMOTE_SERVER  // only available under windows

/////////////////////////////////////////////////////////////////////////////
// WaitRedraw.cpp - class for flicker free drawing
//

int CWaitRedraw::m_nRedrawCount = 0;

CWaitRedraw::CWaitRedraw( )
{
   CWnd *pMainWnd = AfxGetMainWnd( );
   if ( pMainWnd )
   {
      if ( m_nRedrawCount == 0 )
         pMainWnd->SetRedraw( FALSE );

      m_nRedrawCount++;
   }
}

CWaitRedraw::~CWaitRedraw( )
{
   CWnd *pMainWnd = AfxGetMainWnd( );
   if ( pMainWnd )
   {
      if ( m_nRedrawCount > 0 )
         m_nRedrawCount--;

      if ( m_nRedrawCount == 0 )
      {
         pMainWnd->SetRedraw( TRUE );
         pMainWnd->Invalidate( );
      }
   }
}

void
CWaitRedraw::Restore( )
{
   m_nRedrawCount = 0;
   CWnd *pMainWnd = AfxGetMainWnd( );
   if ( pMainWnd )
   {
      pMainWnd->SetRedraw( TRUE );
      pMainWnd->Invalidate( );
   }
}

#endif // #ifndef zREMOTE_SERVER  // only available under windows
