/****************************************************************************
*
* $Workfile:   fmdbcsmp.cpp  $
*   $Author:   DougS  $
*     $Date:   09 Aug 1995 14:59:34  $
* $Revision:   3.4.2.2  $
*
*  DESCRIPTION:
*     Class implementation for the SIMP and auxiliary classes:
*
*        CSIMP           - Sequential Indexed Memory Pool
*        CDataDescriptor - Descriptor of a data item in a record
*        CSortIndex      - Sort index list
*        CSortItem       - Sort item
*
*     These classes are for internal use by the Logical Record View class
*     and are not called directly by an application.  The public interface
*     is more succintly documented.
*
****************************************************************************/

#include <fmpubpch.h>
#include "fmdbc.h"
#include <ctype.h>


RCDX L_MAX_BYTES         = (RCDX) 65000;    // leave room for overhead
RCDX L_NEXT_BLOCK_OFFSET = (RCDX) L_MAX_BYTES - sizeof( VOID * );
RCDX L_MAX_RECORD_BLOCK  = (RCDX) L_NEXT_BLOCK_OFFSET / sizeof( VOID * );
RCDX L_MAX_SORT_BLOCK    = (RCDX) L_NEXT_BLOCK_OFFSET / sizeof( RCDX );

#if 0
#define CSIMP_GETRECORD_PTR( xRecord, pvRecords )  (pvRecords + xRecord - 1)
#else
VOID ** NEAR PASCAL
CSIMP_GETRECORD_PTR( RCDX xRecord, VOID **pvRecords )
{
   while ( xRecord > L_MAX_RECORD_BLOCK )
   {
      pvRecords = *(VOID ***)((BYTE *) pvRecords + L_NEXT_BLOCK_OFFSET);
      xRecord -= L_MAX_RECORD_BLOCK;
   }

   return( pvRecords + xRecord - 1 );
}
#endif

#if 0
#define CSIMP_GETRECORD( xRecord, pvRecords )   *(pvRecords + xRecord - 1)
#else
VOID * NEAR PASCAL
CSIMP_GETRECORD( RCDX xRecord, VOID **pvRecords )
{
   while ( xRecord > L_MAX_RECORD_BLOCK )
   {
      pvRecords = *(VOID ***)((BYTE *) pvRecords + L_NEXT_BLOCK_OFFSET);
      xRecord -= L_MAX_RECORD_BLOCK;
   }

   return( *(pvRecords + xRecord - 1) );
}
#endif

#if 0
#define CSIMP_GETSORTIDX_PTR( xRecord, pxIdx )  (pxIdx[ xRecord - 1 ])
#else
RCDX * NEAR PASCAL
CSIMP_GETSORTIDX_PTR( RCDX xRecord, RCDX *pxIdx )
{
   while ( xRecord > L_MAX_SORT_BLOCK )
   {
      pxIdx = *(RCDX **)((BYTE *) pxIdx + L_NEXT_BLOCK_OFFSET);
      xRecord -= L_MAX_SORT_BLOCK;
   }

   return( pxIdx + xRecord - 1 );
}
#endif

#if 0
#define CSIMP_GETSORTREC( xRecord, pvRecords, pxIdx ) \
                       *(pvRecords + pxIdx[ xRecord - 1 ])
#else
VOID * NEAR PASCAL
CSIMP_GETSORTREC( RCDX xRecord, VOID **pvRecords, RCDX *pxIdx )
{
   while ( xRecord > L_MAX_SORT_BLOCK )
   {
      pxIdx = *(RCDX **)((BYTE *) pxIdx + L_NEXT_BLOCK_OFFSET);
      xRecord -= L_MAX_SORT_BLOCK;
   }

   xRecord = pxIdx[ xRecord - 1 ] + 1;
   return( CSIMP_GETRECORD( xRecord, pvRecords ) );
}
#endif

/* the binary search is implemented with some methods that depend on the
   structure of the records to be placed in the search table.  The following
   is an attempt to document the processing to search for a specified
   record.  Assume the list involves 11 strings:

    input   string       sorted        sorted index    heap index
    -----   ----------   ------        ------------    ----------
      1.    CROCODILE      (4)          1  -->   2      1  --> 10
      2.    ALLIGATOR      (1)          2  -->   4      2  -->  9
      3.    CHIMPANZEE     (3)          3  -->   3      3  -->  8
      4.    BEAR           (2)          4  -->   1      4  -->  6
      5.    ZEBRA         (10)          5  -->  10      5  -->  5
      6.    LION           (7)          6  -->   9      6  -->  7
      7.    PEACOCK        (8)          7  -->   6      7  -->  3
      8.    TIGER          (9)          8  -->   7      8  -->  2
      9.    LAMA           (6)          9  -->   8      9  -->  4
     10.    GIRAFFE        (5)         10  -->   5     10  -->  1
*/


/* initial variable length strings will point here to avoid unnecessary
   1-byte allocations from the variable length data pool */
RCDX pxNul = 0;


/************************** CSIMP::CSIMP ************************************
*
*HelpKey:         Public
*
*Description:
*     CSIMP - constructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSIMP::CSIMP( WORD wReserveIncrement,
              DWORD dwRecordLimit,
              WORD wMaxFixedPoolFieldLth,
              WORD wReservedBytes,
              eAUTOTYPE autotype )
{
   /* record limit is 64K (65536) minus enough space for 4 void pointers ...
      this may quite easily be changed if it is necessary to handle more
      than 64K records by changing the RCDX typedef to a DWORD (from its
      current WORD setting); relaxing the restrictions in the CSIMP and
      CFMDBC_LV constructors; and making any required changes to the
      CSIMP_GETRECORD_PTR, CSIMP_GETRECORD, and CSIMP_GETSORTREC macros
      (at least that's the plan) */
   ASSERT_MSGBOX( "Invalid record limit specified for CSIMP constructor", 0,
                  dwRecordLimit > 0 &&
                  dwRecordLimit < 65536L - 4 * sizeof( VOID * ) );

   m_wMaxFixedPoolFieldLth =
      (wMaxFixedPoolFieldLth > sizeof( LONG double )) ?
                              wMaxFixedPoolFieldLth : sizeof( LONG double );
   m_wReservedBytes = wReservedBytes;  // reserved bytes at start of record
   m_wApplBytes = 0;                   // reserved bytes at end of record
   m_dwRecordLimit = dwRecordLimit;

   if ( m_wReserveIncrement > m_dwRecordLimit )
   {
      m_wReserveIncrement = m_dwRecordLimit;
   }

   m_wReserveIncrement = wReserveIncrement;
   m_lMaxRecords = m_wReserveIncrement > L_MAX_RECORD_BLOCK ?
                                    L_MAX_RECORD_BLOCK : m_wReserveIncrement;

   m_bPurge = TRUE;                    // this is the OWNER of memory pools

   m_pPoolFixedData = 0;
   m_pPoolVarData = 0;
   m_pPoolVariable = new CFMMEM_VarPool( );
   VERIFY( m_pPoolVariable );

   m_wFlags = 0;           // varchar, ...
   if ( autotype == eAUTO_VERSION )
   {
      m_wFlags |= CSIMP_VERSIONING;
      m_wReservedBytes += sizeof( VOID * );
   }
   else
   if ( autotype == eAUTO_UNDO_REDO )
   {
      m_wFlags |= CSIMP_UNDO_REDO;
      m_wReservedBytes += 2 * sizeof( VOID * );
   }

   // the DWORD is for the dirty flag, select bits, etc.
   m_wRecordLth = m_wReservedBytes + sizeof( DWORD );

   m_wDescriptorCnt = 0;
   m_wDescriptorMax = 10;
   m_pDD = (CDataDescriptor **)
   m_pPoolVariable->Alloc( m_wDescriptorMax * sizeof( CDataDescriptor * ) );

   m_dwRecordCnt = 0;
   m_pvRecords = 0;

   m_eSortDir = CSortItem::eASCENDING;
   m_dwDirtySelect = CSIMP_RESERVED; // dirty and select set indicators

   m_pUseIdx = 0;
   m_bInsertSort = FALSE; // as records are inserted, do NOT keep indexes sorted

   m_pfnEnum = 0;
   m_pvRecordErr = 0;
   m_pHead = new CSortIndex( );
   m_pTail = m_pHead;
   m_Error = eCSIMP_RC_SUCCESS;
}

/************************** CSIMP::~CSIMP ***********************************
*
*HelpKey:         Public
*
*Description:
*     CSIMP - destructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSIMP::~CSIMP( VOID )
{
   if ( m_bPurge )                        // owner of memory pools
   {
      WORD k = m_wDescriptorCnt;
      while ( k )
      {
         delete( GetDataDescriptor( k-- ) );
      }

      delete( m_pHead );
      delete( m_pPoolFixedData );
      delete( m_pPoolVarData );
      delete( m_pPoolVariable );
   }
}


/************************** CSIMP::CSIMP ************************************
*
*HelpKey:         Public
*
*Description:
* CSIMP - copy constructor - still incomplete???
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSIMP::CSIMP( const CSIMP& csimp )
{
   m_bPurge = FALSE;                   // NOT owner of memory pools
   m_lMaxRecords = csimp.m_lMaxRecords;
   m_wReserveIncrement = csimp.m_wReserveIncrement;
   m_dwRecordLimit = csimp.m_dwRecordLimit;
   m_wMaxFixedPoolFieldLth = csimp.m_wMaxFixedPoolFieldLth;

   m_dwRecordCnt = csimp.m_dwRecordCnt;

   m_pPoolFixedData = csimp.m_pPoolFixedData;
   m_pPoolVarData = csimp.m_pPoolVarData;
   m_pPoolVariable = csimp.m_pPoolVariable;
   m_pvRecords = csimp.m_pvRecords;

   m_pDD = csimp.m_pDD;
   m_wDescriptorCnt = csimp.m_wDescriptorCnt;
   m_wDescriptorMax = csimp.m_wDescriptorMax;
   m_wReservedBytes = csimp.m_wReservedBytes;
   m_wRecordLth = csimp.m_wRecordLth;
   m_dwDirtySelect = csimp.m_dwDirtySelect;

   m_pHead = csimp.m_pHead;
   m_pTail = csimp.m_pTail;

   m_pUseIdx = csimp.m_pUseIdx;
   m_bInsertSort = csimp.m_bInsertSort;
   m_eSortDir = csimp.m_eSortDir;

   m_pvRecordErr = csimp.m_pvRecordErr;
   m_pfnEnum = csimp.m_pfnEnum;
   m_wFlags = csimp.m_wFlags;
   m_Error = csimp.m_Error;
}


/********************** CSIMP::AcceptVersion ********************************
*
*HelpKey:         Public
*
*Description:
*  Accept a version (retain the work copy) and delete the immediate previous
*  version (either a previous work copy or the original data).
*
*Arguments:
*  RCDX xRecord - absolute 1-based index into the record list
*  BOOL bAllVersions - TRUE  - Accept all versions (record no longer versioned)
*                      FALSE - Accept current version only
*
*Return Value:
*  pointer to record after version is accepted
*  0 - error (record was not versioned)
*
****************************************************************************/
VOID *
CSIMP::AcceptVersion( RCDX xRecord,
                      BOOL bAllVersions )
{
   ASSERT_MSGBOX_RC( "Versioning not enabled for AcceptVersion", 0,
                      m_wFlags & CSIMP_VERSIONING, 0 );

   VOID **ppvVer;
   VOID *pvRecord;
   VOID *pvVersion = GetRecord( xRecord );
   if ( IsDirtySelectBitSet( pvVersion, CSIMP_REDO ) )
   {
      SetDirtySelectBit( pvVersion, CSIMP_REDO, FALSE );
      pvRecord = *((VOID **)((BYTE *) pvVersion +
                             m_wReservedBytes - 2 * sizeof( VOID * )));

      VOID *pv;
      while ( pvRecord )
      {
         pv = pvRecord;
         if ( IsDirtySelectBitSet( pvRecord, CSIMP_REDO ) )
         {
            pvRecord = *((VOID **)((BYTE *) pvRecord +
                                   m_wReservedBytes - 2 * sizeof( VOID * )));
         }
         else
         {
            pvRecord = 0;
         }

         DeleteRecord( pv );
      }
   }

   if ( IsDirtySelectBitSet( pvVersion, CSIMP_VERSIONED ) == FALSE )
   {
      return( 0 );
   }

   ppvVer = (VOID **)((BYTE *) pvVersion + m_wReservedBytes - sizeof( VOID * ));
   do
   {
      /* all we want to do is delete the previous version(s) */
      pvRecord = *ppvVer;  // link back to previous version

      if ( IsDirtySelectBitSet( pvRecord, CSIMP_VERSIONED ) )
      {
         /* re-link to previous previous version */
         *ppvVer = *((VOID **)((BYTE *) pvRecord +
                              m_wReservedBytes - sizeof( VOID * )));
      }
      else
      {
         SetDirtySelectBit( pvVersion, CSIMP_VERSIONED, FALSE );
         *ppvVer = 0;   // no longer versioned
         bAllVersions = FALSE;
      }

      DeleteRecord( pvRecord );

   } while ( bAllVersions );

   return( pvVersion );
}


/********************** CSIMP::AddIndex *************************************
*
*HelpKey:         Public
*
*Description:
*  Add a sort index to a record set
*
*Arguments:
*  WORD wSortIndex - 1-based index
*
*Return Value:
*  pointer to CSortIndex class
*
****************************************************************************/
CSortIndex *
CSIMP::AddIndex( WORD wSortIdx )      // uses 1-based index
{
   ASSERT_MSGBOX_RC( "Invalid index specified for AddIndex", 0,
                     wSortIdx > 0, 0 );

   CSortIndex *pSortIndex = GetSortIndex( wSortIdx );
   if ( pSortIndex )
   {
      pSortIndex->Empty( );
      return( pSortIndex );
   }

   if ( m_pHead == 0 )
   {
      m_pHead = new CSortIndex( );
      m_pTail = m_pHead;
   }

   m_pTail->m_pNext = new CSortIndex( this, wSortIdx, m_lMaxRecords );
   m_pTail = m_pTail->m_pNext;
   return( m_pTail );
}


/********************** CSIMP::AllocBindTimestamps **************************
*
*HelpKey:         Public
*
*Description:
*  Add variable portion of timestamp columns to bind record.
*
*Arguments:
*  VOID *pvBindRecord - pointer to bind record
*
*Return Value:
*  None
*
****************************************************************************/
VOID
CSIMP::AllocBindTimestamps( VOID *pvBindRecord )
{
   if ( m_wFlags & CSIMP_FMTIMESTAMPS_USED )
   {
      CDataDescriptor *pDD;
      WORD k = m_wDescriptorCnt;
      while ( k )
      {
         pDD = GetDataDescriptor( k );    // 1-based index
         if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_FMTIMESTAMP )
         {
            VOID **ppv = (VOID **) ((BYTE *) pvBindRecord + pDD->GetOffset( ));
            *ppv = m_pPoolVarData->Alloc( FM_MAX_SIZE_DATETIME + 2 );
            VERIFY( *ppv );
            memset( *ppv, 0, FM_MAX_SIZE_DATETIME + 2 );  // zeroinit
         }

#if 0
         else
         if ( pDD->m_DataType & CSIMP_UNDERFLOW )
         {
            if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_SDATETIME )
            {
               uAdjLth = FM_MAX_SIZE_SMALLDATETIME + 2;
            }
            else
            if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_TIMESTAMP )
            {
               uAdjLth = FM_MAX_SIZE_DATETIME + 2;
            }
         }

         if ( uAdjLth )
         {
            VOID **ppv = (VOID **) ((BYTE *) pvBindRecord + pDD->GetOffset( ));
            *ppv = m_pPoolVarData->Alloc( uAdjLth );
            VERIFY( *ppv );
            memset( *ppv, 0, uAdjLth );  // zeroinit
         }
#endif

         k--;
      }
   }
}


/********************** CSIMP::CompareRecords *******************************
*
*HelpKey:         Public
*
*Description:
*  Compare two records
*
*Arguments:
*  RCDX xRecord1 - index to first record to compare
*  RCDX xRecord2 - index second record to compare
*
*Return Value:
*  0 - records were equal
*  1 - first record was greater than second record
* -1 - first record was less than second record
*
****************************************************************************/
short
CSIMP::CompareRecords( RCDX xRecord1,
                       RCDX xRecord2 )
{
   VOID *pvRecord1 = CSIMP_GETSORTREC( xRecord1, m_pvRecords,
                                       m_pUseIdx->m_pxIdx );
   VOID *pvRecord2 = CSIMP_GETSORTREC( xRecord2, m_pvRecords,
                                       m_pUseIdx->m_pxIdx );
   return( CompareRecords( pvRecord1, pvRecord2 ) );
}

/********************** CSIMP::CompareRecords *******************************
*
*HelpKey:         Public
*
*Description:
*  Compare two records
*
*Arguments:
*  VOID *pvRecord1 - first record to compare
*  VOID *pvRecord2 - second record to compare
*
*Return Value:
*  0 - records were equal
*  1 - first record was greater than second record
* -1 - first record was less than second record
*
****************************************************************************/
short
CSIMP::CompareRecords( const VOID *pvRecord1,
                       const VOID *pvRecord2 )
{
   ASSERT_MSGBOX_RC( "Must UseSortIndex prior to CompareRecords", 0,
                     m_pUseIdx, 0 );
   short nRC;
   CSortItem *pSortItem = m_pUseIdx->m_pHead->m_pNext;
   ASSERT_MSGBOX_RC( "Compare function required for CompareRecords", 0,
                     pSortItem->m_cmprfn, 0 );
   VOID *pvItem1;
   VOID *pvItem2;
   WORD wOffset;

   while ( pSortItem )
   {
      ASSERT_MSGBOX_RC( "Compare function required for CompareRecords", 0,
                        pSortItem->m_cmprfn, 0 );

      wOffset = pSortItem->m_pDD->GetOffset( );
      pvItem1 = pSortItem->m_pDD->GetDataAddress( pvRecord1 );
      pvItem2 = pSortItem->m_pDD->GetDataAddress( pvRecord2 );

      nRC = (*(pSortItem->m_cmprfn))( pvItem1, pvItem2 );
      if ( nRC )
      {
         return( pSortItem->m_sortdir == CSortItem::eASCENDING ? nRC : -nRC );
      }

      pSortItem = pSortItem->m_pNext;
   }

   m_Error = eCSIMP_RC_DUP;
   return( 0 );
}


/********************** CSIMP::CreateRecord *********************************
*
*HelpKey:         Public
*
*Description:
*  Create a fixed length record.
*  If there is no sort index in use while records are being inserted into
*  the list, the sort indices are left in an INVALID state.  If the user
*  wants to keep an index sorted during inserts, it must be selected and
*  the user is responsible for calling InsertPendingRecord when the data
*  to support the sort index has been set in the record.
*
*
*Arguments:
*  WORD wFlags - CSIMP_NO_INSERT - create the record but do not insert it
*                into the list (e.g. bind and pend records)
*                CSIMP_FULL_RECORD - create a record with all buffers
*                (fixed and variable) allocated to maximum extent
*
*Return Value:
*  0         - record could not be created (insufficient memory or the
*              record limit has been reached)
*  otherwise - record pointer
*
****************************************************************************/
VOID *
CSIMP::CreateRecord( WORD wFlags )
{
   if ( m_pPoolFixedData == 0 )
   {
      Enable( TRUE );
   }

   CSortIndex::eSORTVALID eSortState = CSortIndex::eUNSORTED;

   if ( !(wFlags & CSIMP_NO_INSERT) && m_dwRecordCnt >= m_lMaxRecords )
   {
      RCDX xPrevMax = m_lMaxRecords;

      /* reallocate the buffers (MemRealloc could possibly be used for the
         record buffer ... but the other buffers need to be duplicated while
         they are copied) */
      if ( m_wReserveIncrement < 2000 )
      {
         m_wReserveIncrement <<= 1;   // exponentially increment up to 2K
      }

      if ( m_lMaxRecords + m_wReserveIncrement < m_dwRecordLimit )
      {
         m_lMaxRecords += m_wReserveIncrement;
      }
      else
      {
         if ( m_lMaxRecords < m_dwRecordLimit )
         {
            m_lMaxRecords = m_dwRecordLimit;
         }
         else
         {
            return( 0 ); // no more room in the inn
         }
      }

      {  // just for stack reuse below

      VOID ***pvOldPtr = &m_pvRecords;
      VOID **pvNewPtr = 0;
      VOID **pvOld = m_pvRecords;
      VOID **pvNew = 0;
      RCDX xOldMax = xPrevMax;
      RCDX xNewMax = m_lMaxRecords;

      while ( xOldMax > L_MAX_RECORD_BLOCK )
      {
         pvOldPtr = (VOID ***)((BYTE *) pvOld + L_NEXT_BLOCK_OFFSET);
         pvOld = *pvOldPtr;
         xOldMax -= L_MAX_RECORD_BLOCK;
         xNewMax -= L_MAX_RECORD_BLOCK;
      }

      /* if there are more records than can fit in a block, allocate a
         second block and have the first block point to the second block.
         If we are just increasing the size of the old block, allocate
         the new block and copy the old to the new.  If the new block
         requires breaking a block boundary, allocate one complete block
         of data, copy the old data to the new complete block, and then
         allocate another block of data, having the complete block point
         to this other block of data */
      RCDX xNewRemain = xNewMax % (RCDX) L_MAX_RECORD_BLOCK;
      if ( (xNewMax >= L_MAX_RECORD_BLOCK) && xNewRemain )
      {
         pvNew = (VOID **)
            m_pPoolVarData->Alloc( (LONG) sizeof( VOID * ) * xNewRemain );
         VERIFY( pvNew );

         memset( pvNew, 0, sizeof( VOID * ) * xNewRemain );
         xNewMax = L_MAX_RECORD_BLOCK;
      }

      *pvOldPtr = (VOID **)
               m_pPoolVarData->Alloc( (LONG) sizeof( VOID * ) * xNewMax );
      VERIFY( *pvOldPtr );

      memcpy( *pvOldPtr, pvOld, xOldMax * sizeof( VOID * ) );
      m_pPoolVarData->Free( pvOld );
      if ( pvNew )  // only true when block boundary has been broken
      {
         pvOldPtr = (VOID ***)((BYTE *) *pvOldPtr + L_NEXT_BLOCK_OFFSET);
         *pvOldPtr = pvNew;
      }

      } // end of braces just for the pupose of permitting stack reuse

      if ( m_pUseIdx )
      {
         RCDX **pxOldPtr = &(m_pUseIdx->m_pxIdx);
         RCDX *pxNewPtr = 0;
         RCDX *pxOld = m_pUseIdx->m_pxIdx;
         RCDX *pxNew = 0;
         RCDX xOldMax = xPrevMax;
         RCDX xNewMax = m_lMaxRecords;
         while ( xOldMax > L_MAX_SORT_BLOCK )
         {
            pxOldPtr = (RCDX **)((BYTE *) pxOld + L_NEXT_BLOCK_OFFSET);
            pxOld = *pxOldPtr;
            xOldMax -= L_MAX_SORT_BLOCK;
            xNewMax -= L_MAX_SORT_BLOCK;
         }

         /* if there are more sort indices than can fit in a block, allocate
            a second block and have the first block point to the second block.
            If we are just increasing the size of the old block, allocate
            the new block and copy the old to the new.  If the new block
            requires breaking a block boundary, allocate one complete block
            of data, copy the old data to the new complete block, and then
            allocate another block of data, having the complete block point
            to this other block of data */
         RCDX xNewRemain = xNewMax % (RCDX) L_MAX_RECORD_BLOCK;
         if ( (xNewMax >= L_MAX_SORT_BLOCK) && xNewRemain )
         {
            pxNew = (RCDX *)
               m_pPoolVarData->Alloc( (LONG) sizeof( RCDX ) * xNewRemain );
            VERIFY( pxNew );

            memset( pxNew, 0, sizeof( RCDX ) * xNewRemain );
            xNewMax = L_MAX_SORT_BLOCK;
         }

         *pxOldPtr = (RCDX *)
                  m_pPoolVarData->Alloc( (LONG) sizeof( RCDX ) * xNewMax );
         VERIFY( *pxOldPtr );

         memcpy( *pxOldPtr, pxOld, xOldMax * sizeof( RCDX ) );
         m_pPoolVarData->Free( pxOld );
         if ( pxNew )  // only true when block boundary has been broken
         {
            pxOldPtr = (RCDX **)((BYTE *) *pxOldPtr + L_NEXT_BLOCK_OFFSET);
            *pxOldPtr = pxNew;
         }
      }

      CSortIndex *pSortIndex = m_pHead->m_pNext;
      while ( pSortIndex )
      {
         if ( m_pUseIdx != pSortIndex )
         {
            pSortIndex->m_eSortState = CSortIndex::eINDEX_INVALID;
            if ( pSortIndex->m_pxIdx )
            {
               FreeSortIndex( pSortIndex );
            }
         }

         pSortIndex = pSortIndex->m_pNext;
      }
   }

   VOID *pv;
   VOID *pvRec = m_pPoolFixedData->Alloc( );
   if ( pvRec )
   {
      memset( pvRec, 0, m_wRecordLth );
      if ( wFlags & CSIMP_FULL_RECORD )
      {
         SetDirtySelectBit( pvRec, CSIMP_FULL, TRUE );
      }
      else
      if ( wFlags & CSIMP_NO_INSERT )
      {
         return( pvRec );
      }

      if ( wFlags & CSIMP_FULL_RECORD )
      {
         CDataDescriptor *pDD;
         WORD k = m_wDescriptorCnt;
         while ( k )
         {
            pDD = GetDataDescriptor( k );    // 1-based index
            if ( pDD->IsVarSize( ) )
            {
               VOID **ppv = (VOID **) ((BYTE *) pvRec + pDD->GetOffset( ));
               WORD wLth = pDD->GetRawLth( ) + sizeof( WORD );
               pv = m_pPoolVarData->Alloc( wLth );
               memset( pv, 0, wLth );  // zeroinit
               VERIFY( pv );

               WORD *pw = (WORD *) pv;
               *pw = 0;
               *ppv = pv;
            }

            k--;
         }
      }

      if ( !(wFlags & CSIMP_NO_INSERT) )
      {
         m_dwRecordCnt++;
         *(CSIMP_GETRECORD_PTR( m_dwRecordCnt, m_pvRecords )) = pvRec;

         if ( m_pUseIdx )
         {
            if ( m_pUseIdx->m_eSortState >= CSortIndex::eINDEX_PENDING )
            {
               *(CSIMP_GETSORTIDX_PTR( m_dwRecordCnt, m_pUseIdx->m_pxIdx )) =
                                                           m_dwRecordCnt - 1;
               m_pUseIdx->m_eSortState = CSortIndex::eINDEX_PENDING;
            }
         }
      }
   }

   return( pvRec );
}


/********************** CSIMP::CreateVersion ********************************
*
*HelpKey:         Public
*
*Description:
*  Version a record.
*
*Arguments:
*  RCDX xRecord - absolute 1-based index into the record list
*
*Return Value:
*  pointer to new version of record
*  0 - record could not be versioned (insufficient memory)
*
****************************************************************************/
VOID *
CSIMP::CreateVersion( RCDX xRecord )
{
   ASSERT_MSGBOX_RC( "Versioning not enabled for CreateVersion", 0,
                     m_wFlags & CSIMP_VERSIONING, 0 );

   VOID *pvRecord = GetRecord( xRecord );
   VOID *pvVersion = CreateRecord( CSIMP_NO_INSERT );

   /* copy flags et. al. */
   memcpy( pvVersion, pvRecord, m_wReservedBytes + sizeof( DWORD ) );
   SetRecordFromRecord( pvVersion, pvRecord );
   SetDirtySelectBit( pvVersion, CSIMP_VERSIONED, TRUE );
   *(CSIMP_GETRECORD_PTR( xRecord, m_pvRecords )) = pvVersion;
   VOID **pv = (VOID **)((BYTE *) pvVersion +
                         m_wReservedBytes - sizeof( VOID * ));
   *pv = pvRecord;   // link back to previous version

   return( pvVersion );
}


/********************** CSIMP::DeleteRecord *********************************
*
*HelpKey:         Public
*
*Description:
*  Delete a fixed length record from the pool (and all its variable portions)
*
*Arguments:
*  VOID *pvRecord - record to delete from the pool
*
*Return Value:
*  eCSIMP_RC_SUCCESS - Record successfully deleted
*  eCSIMP_RC_FAILURE - Record NOT deleted
*
****************************************************************************/
eCSIMP_RC
CSIMP::DeleteRecord( VOID *pvRecord )
{
   ASSERT_MSGBOX_RC( "Attempting to delete Null record", 0,
                     pvRecord, eCSIMP_RC_FAILURE );

   CDataDescriptor *pDD;
   WORD k = m_wDescriptorCnt;
   while ( k && (m_wFlags & CSIMP_VARSIZE_USED) )
   {
      pDD = GetDataDescriptor( k );    // 1-based index
      if ( pDD->IsVarSize( ) )
      {
         VOID *pv = pDD->GetDataAddress( pvRecord );
         if ( pv )
         {
            m_pPoolVarData->Free( pv );
         }
      }

      k--;
   }

   m_pPoolFixedData->Free( pvRecord );
   return( eCSIMP_RC_SUCCESS );
}


/********************** CSIMP::DeleteVersion ********************************
*
*HelpKey:         Public
*
*Description:
*  Delete a version (reset to the previous version).
*
*Arguments:
*  RCDX xRecord - absolute 1-based index into the record list
*  BOOL bAllVersions - TRUE  - Accept all versions (record no longer versioned)
*                      FALSE - Accept current version only
*
*Return Value:
*  pointer to record after version is deleted
*  0 - error (record was not versioned)
*
****************************************************************************/
VOID *
CSIMP::DeleteVersion( RCDX xRecord,
                      BOOL bAllVersions )
{
   ASSERT_MSGBOX_RC( "Versioning not enabled for DeleteVersion", 0,
                     m_wFlags & CSIMP_VERSIONING, 0 );

   VOID *pvRecord;
   VOID *pvVersion = GetRecord( xRecord );
   if ( IsDirtySelectBitSet( pvVersion, CSIMP_REDO ) )
   {
      SetDirtySelectBit( pvVersion, CSIMP_REDO, FALSE );
      pvRecord = *((VOID **)((BYTE *) pvVersion +
                             m_wReservedBytes - 2 * sizeof( VOID * )));

      VOID *pv;
      while ( pvRecord )
      {
         pv = pvRecord;
         if ( IsDirtySelectBitSet( pvRecord, CSIMP_REDO ) )
         {
            pvRecord = *((VOID **)((BYTE *) pvRecord +
                                   m_wReservedBytes - 2 * sizeof( VOID * )));
         }
         else
         {
            pvRecord = 0;
         }

         DeleteRecord( pv );
      }
   }


   if ( IsDirtySelectBitSet( pvVersion, CSIMP_VERSIONED ) == FALSE )
   {
      return( 0 );
   }

   do
   {
      pvRecord = *((VOID **)((BYTE *) pvVersion +
                             m_wReservedBytes - sizeof( VOID * )));
      DeleteRecord( pvVersion );
      pvVersion = pvRecord;  // link back to previous version

      if ( IsDirtySelectBitSet( pvVersion, CSIMP_VERSIONED ) == FALSE )
      {
         bAllVersions = FALSE;
      }

   } while ( bAllVersions );

   *(CSIMP_GETRECORD_PTR( xRecord, m_pvRecords )) = pvVersion;
   return( pvVersion );
}

/********************** CSIMP::FreeSortIndex ********************************
*
*HelpKey:         Private
*
*Description:
*  Free the sort index (all segments)
*
*Arguments:
*  CSortIndex * pSortIndex - sort index to free
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::FreeSortIndex( CSortIndex *pSortIndex )
{
   RCDX *pxOld = pSortIndex->m_pxIdx;
   RCDX **pxOldPtr = &(pSortIndex->m_pxIdx);
   RCDX xCnt = m_dwRecordCnt;

   while ( xCnt > L_MAX_SORT_BLOCK )
   {
      pxOld = *pxOldPtr;
      pxOldPtr = (RCDX **)((BYTE *) pxOld + L_NEXT_BLOCK_OFFSET);
      m_pPoolVarData->Free( pxOld );

      xCnt -= L_MAX_SORT_BLOCK;
   }

   m_pPoolVarData->Free( pxOld );
   pSortIndex->m_pxIdx = 0;
}

/********************** CSIMP::Enable ***************************************
*
*HelpKey:         Public
*
*Description:
*  Called when all fields have been defined for the record.  Fixed and
*  variable memory pools are created/destroyed.
*  If this is not the owner of the memory pools, the pools are not freed
*
*Arguments:
*  WORD wEnable - CSIMP_DISABLE - free memory pools
*                 CSIMP_ENABLE  - allocate memory pools
*                 CSIMP_RESET   - reestablish fixed lth and allocate memory
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::Enable( WORD wEnable )
{
   ASSERT_MSGBOX( "Non-owner view freeing memory", 0, m_bPurge );
   if ( m_bPurge == FALSE )            // NOT owner of memory pools
   {
      return;
   }

   m_dwRecordCnt = 0;

   delete( m_pPoolFixedData );
   m_pPoolFixedData = 0;
   delete( m_pPoolVarData );
   m_pPoolVarData = 0;

   CSortIndex *pSortIndex = m_pHead->m_pNext;
   while ( pSortIndex )
   {
      pSortIndex->m_eSortState = CSortIndex::eINDEX_INVALID;
      pSortIndex->m_pxIdx = 0;
      pSortIndex = pSortIndex->m_pNext;
   }

   if ( wEnable )   // not disable
   {
      m_pPoolVarData = new CFMMEM_VarPool( );
      VERIFY( m_pPoolVarData );

      if ( m_lMaxRecords > L_MAX_RECORD_BLOCK )
      {
         m_lMaxRecords = L_MAX_RECORD_BLOCK;    //  <= L_MAX_SORT_BLOCK
      }

      m_pvRecords = (VOID **)
                 m_pPoolVarData->Alloc( m_lMaxRecords * sizeof( VOID * ) );
      VERIFY( m_pvRecords );


      /* future ... */
//    if ( wEnable == CSIMP_RESET )
//    {
//    }

      m_pPoolFixedData = new CFMMEM_FixPool( m_wRecordLth + m_wApplBytes,
                                             (DWORD) m_wReserveIncrement );
      VERIFY( m_pPoolFixedData );
   }
   else
   {
      m_pvRecords = 0;
   }
}


/********************** CSIMP::EnableInsertSort *****************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::EnableInsertSort( BOOL bEnable )
{
   m_bInsertSort = bEnable;
}


/********************** CSIMP::GetApplBytes *********************************
*
*HelpKey:         Public
*
*Description:
*  Get the number of application reserved bytes in each record
*
*Arguments:
*  None
*
*Return Value:
*  WORD - number of bytes reserved for the application.
*
****************************************************************************/
WORD
CSIMP::GetApplBytes( VOID )
{
   return( m_wApplBytes );
}


/********************** CSIMP::GetDataDescriptor ****************************
*
*HelpKey:         Public
*
*Description:
*  Get the data descriptor corresponding to a particular field in the
*  record using a 1-based index
*
*Arguments:
*  WORD wIdx - 1-based index to field in the record
*
*Return Value:
*  pointer to a CDataDescriptor class for the field
*  0 - data descriptor was not located
*
****************************************************************************/
CDataDescriptor *
CSIMP::GetDataDescriptor( WORD wIdx )     // 1-based index
{
   ASSERT_MSGBOX_RC( "Invalid index for GetDataDescriptor", 0,
                     wIdx > 0 && wIdx <= m_wDescriptorCnt, 0 );
   return( m_pDD[ wIdx - 1 ] );
}

/********************** CSIMP::GetDataDescriptor ****************************
*
*HelpKey:         Public
*
*Description:
*  Get the data descriptor for a field in the record using the field's
*  id (TES).
*
*Arguments:
*  FMDBM_FLDID FldId - TES field id for desired field's data descriptor
*
*Return Value:
*  pointer to a CDataDescriptor class for the field
*  0 - data descriptor was not located
*
****************************************************************************/
CDataDescriptor *
CSIMP::GetDataDescriptor( FMDBM_FLDID FldId )
{
   for ( WORD k = 0; k < m_wDescriptorCnt; k++ )
   {
      if ( m_pDD[ k ]->m_FldId == FldId )
      {
         return( m_pDD[ k ] );
      }
   }

   return( 0 );
}


/********************** CSIMP::GetDataDescriptorIndex ***********************
*
*HelpKey:         Public
*
*Description:
*  Get the index to the data descriptor in the record using a pointer to
*  the CDataDescriptor class for a field
*
*Arguments:
*  CDateDescriptor *pDD - pointer to the data descriptor class for a field
*
*Return Value:
*  0         - field corresponding to the data descriptor did not exist
*              for the record
*  otherwise - 1-based index to the data descriptor for the field
*
****************************************************************************/
WORD
CSIMP::GetDataDescriptorIndex( CDataDescriptor *pDD )
{
   for ( WORD k = 0; k < m_wDescriptorCnt; k++ )
   {
      if ( m_pDD[ k ] == pDD )
      {
         return( k + 1 );
      }
   }

   return( 0 );
}


/********************** CSIMP::GetDataFromRecord ****************************
*
*HelpKey:         Public
*
*Description:
*  Get the data for a field in the record.
*
*Arguments:
*  CDataDescriptor pDD - descriptor of field in the record
*  VOID *pvData - buffer in which to retrieve the raw data
*  WORD wDataLth - length of buffer
*  RCDX xRecord - absolute 1-based index into the record list for the
*                 source record
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::GetDataFromRecord( CDataDescriptor *pDD,
                          VOID *pvData,
                          WORD wDataLth,
                          RCDX xRecord )  // absolute 1-based index into
                                          // the input order list
{
   ASSERT_MSGBOX( "Invalid index specified for GetDataFromRecord", 0,
                  xRecord > 0 && xRecord <= m_dwRecordCnt );
   GetDataFromRecord( pDD, pvData, wDataLth,
                      CSIMP_GETRECORD( xRecord, m_pvRecords ) );
}


/********************** CSIMP::GetDataFromRecord ****************************
*
*HelpKey:         Public
*
*Description:
*  Get the data for a field in the record.
*
*Arguments:
*  CDataDescriptor pDD - descriptor of field in the record
*  VOID *pvData - buffer in which to retrieve the raw data
*  WORD wDataLth - length of buffer
*  VOID *pvSrcRecord - source record from which to get the field data
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::GetDataFromRecord( CDataDescriptor *pDD,
                          VOID *pvData,
                          WORD wDataLth,
                          VOID *pvSrcRecord )
{
   ASSERT_MSGBOX( "Invalid data descriptor specified for GetDataFromRecord",
                  0, pDD );
   ASSERT_MSGBOX( "Null return buffer specified for GetDataFromRecord",
                  0, pvData );

   WORD wRawLth = pDD->GetDataLength( pvSrcRecord );
   VOID *pv = pDD->GetDataAddress( pvSrcRecord );
   if ( wDataLth > wRawLth )
   {
      wDataLth = wRawLth;
   }

   memcpy( pvData, pv, wDataLth );
}


/********************** CSIMP::GetError *************************************
*
*HelpKey:         Public
*
*Description:
*  Get the last error that occurred while processing the list.  The last
*  error is reset to eCSIMP_RC_SUCCESS after this function is called.
*
*Arguments:
*  none
*
*Return Value:
*  last error - eCSIMP_RC_SUCCESS ==> no error
*
*
****************************************************************************/
eCSIMP_RC
CSIMP::GetError( VOID )
{
   eCSIMP_RC Error = m_Error;

   m_Error = eCSIMP_RC_SUCCESS;
   return( Error );
}


/********************** CSIMP::GetRecord ************************************
*
*HelpKey:         Public
*
*Description:
*  Get the pointer to a record based on the specified 1-based index
*
*Arguments:
*  RCDX xRecord - 1-based record index
*
*Return Value:
*  pointer to record at specified index
*
****************************************************************************/
VOID *
CSIMP::GetRecord( RCDX xRecord )
{
   ASSERT_MSGBOX_RC( "Invalid index specified for GetRecord", 0,
                     xRecord > 0 && xRecord <= m_dwRecordCnt, 0 );

   VOID *pvRecord;

   if ( m_eSortDir == CSortItem::eDESCENDING )
   {
      xRecord = m_dwRecordCnt - xRecord + 1;
   }

   if ( m_pUseIdx )
   {
      pvRecord = CSIMP_GETSORTREC( xRecord, m_pvRecords, m_pUseIdx->m_pxIdx );
   }
   else
   {
      pvRecord = CSIMP_GETRECORD( xRecord, m_pvRecords );
   }

   return( pvRecord );
}


/********************** CSIMP::GetRecordIndex *******************************
*
*HelpKey:         Public
*
*Description:
*  Get the 1-based index for a record based on its pointer
*
*Arguments:
*  VOID *pvRecord - record for which to retrieve the 1-based index
*
*Return Value:
*  0         - record was not located
*  otherwise - 1-based index to the record in the list
*
****************************************************************************/
RCDX
CSIMP::GetRecordIndex( const VOID *pvRecord )
{
   RCDX k;

   if ( m_pUseIdx )
   {
      for ( k = 1; k <= m_dwRecordCnt; k++ )
      {
         if ( pvRecord == CSIMP_GETSORTREC( k, m_pvRecords,
                                            m_pUseIdx->m_pxIdx ) )
         {
            return( k );
         }
      }
   }
   else
   {
      for ( k = 1; k <= m_dwRecordCnt; k++ )
      {
         if ( pvRecord == CSIMP_GETRECORD( k, m_pvRecords ) )
         {
            return( k );
         }
      }
   }

   return( 0 );
}


/********************** CSIMP::GetRecordApplBytesPtr ************************
*
*HelpKey:         Public
*
*Description:
*  Get the pointer to a the application reserved bytes in the record
*  based on the record pointer
*
*Arguments:
*  VOID *pvRecord - pointer to record for which to get application
*                   reserved bytes
*
*Return Value:
*  pointer to record application bytes at specified index
*
****************************************************************************/
VOID *
CSIMP::GetRecordApplBytesPtr( VOID *pvRecord )
{
   ASSERT_MSGBOX_RC( "Application bytes not reserved", 0, m_wApplBytes, 0 );
   return( (char *) pvRecord + m_wRecordLth );
}


/********************** CSIMP::GetRecordApplBytesPtr ************************
*
*HelpKey:         Public
*
*Description:
*  Get the pointer to a the application reserved bytes in the record
*  based on the specified 1-based index
*
*Arguments:
*  RCDX xRecord - 1-based record index
*
*Return Value:
*  pointer to record application bytes at specified index
*
****************************************************************************/
VOID *
CSIMP::GetRecordApplBytesPtr( RCDX xRecord )
{
   ASSERT_MSGBOX_RC( "Invalid index specified for GetRecord", 0,
                     xRecord > 0 && xRecord <= m_dwRecordCnt, 0 );
   ASSERT_MSGBOX_RC( "Application bytes not reserved", 0, m_wApplBytes, 0 );

   VOID *pvRecord;

   if ( m_eSortDir == CSortItem::eDESCENDING )
   {
      xRecord = m_dwRecordCnt - xRecord + 1;
   }

   if ( m_pUseIdx )
   {
      pvRecord = CSIMP_GETSORTREC( xRecord, m_pvRecords, m_pUseIdx->m_pxIdx );
   }
   else
   {
      pvRecord = CSIMP_GETRECORD( xRecord, m_pvRecords );
   }

   return( (char *) pvRecord + m_wRecordLth );
}


/********************** CSIMP::SetApplBytes *********************************
*
*HelpKey:         Public
*
*Description:
*  Set the number of bytes to reserve for the application in each record
*  This method would more appropriately have been named ReserveApplBytes.
*
*Arguments:
*  WORD wBytes - number of bytes to reserve for the application
*
*Return Value:
*  WORD - previous number of bytes reserved for the application.
*
****************************************************************************/
WORD
CSIMP::SetApplBytes( WORD wBytes )
{
   ASSERT_MSGBOX_RC( "Must reserve application bytes prior to Enable", 0,
                     m_pPoolFixedData == 0, 0 );
   WORD wPrevBytes = m_wApplBytes;
   m_wApplBytes = wBytes;
   return( wPrevBytes );
}


/********************** CSIMP::GetSortIndex *********************************
*
*HelpKey:         Public
*
*Description:
*  Get the pointer to the CSortIndex using a 1-based index
*
*Arguments:
*  WORD wSortIndex - Index by which CSortIndex was created (> 0)
*
*Return Value:
*  0         - no index corresponding to wSortIndex exists
*  otherwise - pointer to CSortIndex
*
****************************************************************************/
CSortIndex *
CSIMP::GetSortIndex( WORD wSortIdx )
{
   ASSERT_MSGBOX_RC( "Invalid search index in GetSortIndex", 0,
                     wSortIdx > 0, 0 );

   CSortIndex *pSortIndex = m_pHead->m_pNext;
   while ( pSortIndex )
   {
      if ( pSortIndex->m_wIndex == wSortIdx )
      {
         return( pSortIndex );
      }

      pSortIndex = pSortIndex->m_pNext;
   }

   return( 0 );
}


/********************** CSIMP::GetVersion ***********************************
*
*HelpKey:         Public
*
*Description:
*  Gets the current version number (0 ==> not versioned; 1 ==> first version,
*  etc.) for a record.
*
*Arguments:
*  RCDX xRecord - absolute 1-based index into the record list
*
*Return Value:
*  Version number
*  0 - record was not versioned
*
****************************************************************************/
WORD
CSIMP::GetVersion( RCDX xRecord )
{
   ASSERT_MSGBOX_RC( "Versioning not enabled for GetVersion", 0,
                     m_wFlags & CSIMP_VERSIONING, 0 );

   VOID *pvRecord = GetRecord( xRecord );
   WORD wVersion = 0;

   do
   {
      if ( IsDirtySelectBitSet( pvRecord, CSIMP_VERSIONED ) == FALSE )
      {
         return( wVersion );
      }

      pvRecord = *((VOID **)((BYTE *) pvRecord +  // link back to prev version
                             m_wReservedBytes - sizeof( VOID * )));
      wVersion++;

   } while ( TRUE );

   return( 0 );
}


/********************** CSIMP::InsertDataDescriptor *************************
*
*HelpKey:         Public
*
*Description:
*  Add a data descriptor for a field in the record
*
*Arguments:
*  FMDBM_FLDID - field id for the field being added to the record
*  CSIMP_DATATYPE - data type of the field being added to the record
*  WORD wRawLth - raw length of the field being added to the record
*
*Return Value:
*  pointer to the data descriptor for the field added to the record
*
****************************************************************************/
CDataDescriptor *
CSIMP::InsertDataDescriptor( FMDBM_FLDID FldId,
                             CSIMP_DATATYPE DataType,
                             WORD wRawLth )
{
   if ( m_wDescriptorCnt >= m_wDescriptorMax - 1 )
   {
      CDataDescriptor **pDDNew;

      m_wDescriptorMax += 10;
      pDDNew = (CDataDescriptor **) m_pPoolVariable->
                    Alloc( m_wDescriptorMax * sizeof( CDataDescriptor * ) );
      VERIFY( pDDNew );
      memcpy( pDDNew, m_pDD,
              sizeof( CDataDescriptor * ) * m_wDescriptorCnt );
      m_pPoolVariable->Free( m_pDD );
      m_pDD = pDDNew;
   }

   CDataDescriptor *pDD = new CDataDescriptor( );
   pDD->m_FldId = FldId;
   pDD->m_wOffset = m_wRecordLth;

   if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_VARCHAR )
   {
      wRawLth += sizeof( WORD ) + 1;  // null terminator + lth bytes
      m_wFlags |= CSIMP_VARCHAR_USED;
   }
   else
   if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_NTSTRING )
   {
      wRawLth++;       // allow for null terminator
   }
   else
   if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_DESENCRYPT )
   {
      wRawLth = DBM_SIZE_DESENCRYPT( wRawLth );
   }
   else
   if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_FMTIMESTAMP )
   {
      m_wFlags |= CSIMP_FMTIMESTAMPS_USED;
   }

   /* report correct raw length (prior to rounding up to even byte
      boundary) */
   pDD->m_wRawLth = wRawLth;

   if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_CHAR )
   {
      wRawLth++;  // allow for null terminator but do not report extra byte
   }

   if ( wRawLth > m_wMaxFixedPoolFieldLth )
   {
      DataType |= CSIMP_ADDRESS;
      m_wRecordLth += sizeof( VOID * );
   }
   else
   {
      /* keep even byte boundaries (future for 32-bit) */
      m_wRecordLth += wRawLth + (wRawLth % 2);
   }

   pDD->m_DataType = DataType;
   m_pDD[ m_wDescriptorCnt++ ] = pDD;
   return( pDD );
}


/********************** CSIMP::InsertPendingRecord **************************
*
*HelpKey:         Public
*
*Description:
*  Insert a record that was created but not placed in the list at its
*  "correct" location.
*  If the user is using a sort index but wants new insertions to be left
*  at the end (unsorted), the user will call InsertPendingRecord with the
*  deferred flag set to TRUE
*
*
*Arguments:
*  VOID *pvNewRecord - the record to be positioned in the list
*  BOOL bDeferred - TRUE  - defer sorting (keep record at end of list)
*                   FALSE - sort immediately (if pending)
*  BOOL bRemoveDuplicates - remove any duplicate record being inserted
*
*Return Value:
*  CSIMP_SUCCESS  - record positioned correctly
*  otherwise - an error (e.g.  CSIMP_DUP)
*
****************************************************************************/
eCSIMP_RC
CSIMP::InsertPendingRecord( VOID *pvNewRecord,
                            BOOL bDeferred,
                            BOOL bRemoveDuplicates )
{
   ASSERT_MSGBOX_RC( "A sort index must be in use to Remove Duplicates", 0,
                     bRemoveDuplicates == FALSE ||
                     (bRemoveDuplicates && m_pUseIdx),
                     eCSIMP_RC_FAILURE );

   if ( !pvNewRecord )
   {
      m_Error = eCSIMP_RC_INVALID_PTR;
      return( m_Error );
   }

   m_Error = eCSIMP_RC_SUCCESS;
   if ( m_pUseIdx && m_pUseIdx->m_eSortState == CSortIndex::eINDEX_PENDING )
   {
      ASSERT_MSGBOX_RC( "Timing error for InsertPendingRecord", 0,
                        pvNewRecord == CSIMP_GETSORTREC( m_dwRecordCnt,
                                                         m_pvRecords,
                                                         m_pUseIdx->m_pxIdx ),
                        eCSIMP_RC_FAILURE );

      m_dwRecordCnt--; // to simplify the logic (will reset at end)
      RCDX xUpper = m_dwRecordCnt;
      RCDX xLower;
      RCDX xMid;
      short nRC;

      m_pUseIdx->m_eSortState = CSortIndex::eSORTED;
      if ( bDeferred )
      {
         xLower = m_dwRecordCnt + 1;
      }
      else
      {
         xLower = 0;
      }

      while ( xLower <= xUpper )
      {
         xMid = (xUpper + xLower) >> 1;    // ">> 1" == "divide by 2"

         nRC = CompareRecords( pvNewRecord,
                               CSIMP_GETSORTREC( xMid + 1, m_pvRecords,
                                                 m_pUseIdx->m_pxIdx ) );
         if ( nRC > 0 )
            xLower = xMid + 1;
         else
         if ( nRC < 0 )
            xUpper = xMid - 1;
         else
         {
            ASSERT_TRACE( "Duplicate encountered on InsertRecord", 0, FALSE );
            m_Error = eCSIMP_RC_DUP;
            if ( bRemoveDuplicates )
            {
               /* free duplicate record leaving record count decremented */
               m_pPoolFixedData->Free( pvNewRecord );
               return( m_Error );    // leave record count decremented
            }

            xLower = xMid + 1;
            xUpper = xMid;
            m_pUseIdx->m_eSortState = CSortIndex::eSORTED_DUPLICATES;
         }
      }

      if ( xLower < m_dwRecordCnt )
      {
         RCDX xRecordCnt = m_dwRecordCnt;
         RCDX *pxIdx = m_pUseIdx->m_pxIdx;
         VOID *pv = m_pPoolVarData->Alloc( L_MAX_BYTES );
         while ( xLower > L_MAX_SORT_BLOCK )
         {
            pxIdx = *(RCDX **)((BYTE *) pxIdx + L_NEXT_BLOCK_OFFSET);
            xLower -= L_MAX_SORT_BLOCK;
            xRecordCnt -= L_MAX_SORT_BLOCK;
         }

         /* now move every reference that needs to be moved up by one */
         RCDX xSave = m_dwRecordCnt;
         RCDX xTemp;
         while ( xRecordCnt > L_MAX_SORT_BLOCK )
         {
            xTemp = pxIdx[ L_MAX_SORT_BLOCK - 1 ];
            memcpy( (char *) pv, (char *) pxIdx + xLower,
                    (L_MAX_SORT_BLOCK - xLower) * sizeof( RCDX ) );
            memcpy( (char *) pxIdx + xLower + 1, (char *) pv,
                    (L_MAX_SORT_BLOCK - xLower - 1) * sizeof( RCDX ) );
            pxIdx[ xLower ] = xSave;
            xSave = xTemp;
            xLower = 0;
            xRecordCnt -= L_MAX_SORT_BLOCK;
            pxIdx = *(RCDX **)((BYTE *) pxIdx + L_NEXT_BLOCK_OFFSET);
         }

         /* boundary condition??? check when record cnt right at buffer end */
         if ( xRecordCnt )
         {
            memcpy( (char *) pv, (char *) pxIdx + xLower,
                    (xRecordCnt - xLower) * sizeof( RCDX ) );
            memcpy( (char *) pxIdx + xLower + 1, (char *) pv,
                    (xRecordCnt - xLower - 1) * sizeof( RCDX ) );
            pxIdx[ xLower ] = xSave;
         }

         m_pPoolVarData->Free( pv );
         m_dwRecordCnt++; // reset count
      }
   }

   return( m_Error );
}


/********************** CSIMP::IsDirtySelectBitSet **************************
*
*HelpKey:         Public
*
*Description:
*  Is a dirty (record changed) or select (record selected) or other (user
*  defined) bit set for the record.
*
*Arguments:
*  VOID *pvRecord - record about which information is being queried
*  DWORD dwBitPattern - bit pattern being queried (e.g.  dirty flag or
*                       select set)
*
*Return Value:
*  TRUE  - record was dirty/selected
*  FALSE - record was NOT dirty/selected
*
****************************************************************************/
BOOL
CSIMP::IsDirtySelectBitSet( const VOID *pvRecord,
                            DWORD dwBitPattern )
{
   DWORD dw = *((DWORD *)((BYTE *) pvRecord + m_wReservedBytes));
   return( (dw & dwBitPattern) ? TRUE : FALSE );
}


/********************** CSIMP::IsEnabled ************************************
*
*HelpKey:         Public
*
*Description:
*  Has the CSIMP been enabled?
*
*Arguments:
*  none
*
*Return Value:
*  BOOL bEnabled - TRUE  - CSIMP has been enabled
*                  FALSE - CSIMP has not been enabled
*
****************************************************************************/
BOOL
CSIMP::IsEnabled( VOID )
{
   return( m_pPoolFixedData == 0 ? FALSE : TRUE );
}


/********************** CSIMP::IsUndone *************************************
*
*HelpKey:         Public
*
*Description:
*  Determine if the record is versioned and has been undone (redo
*  is permitted)

*
*Arguments:
*  VOID *pvRecord - record pointer
*
*Return Value:
*  TRUE  - record is undone (redo permitted)
*  FALSE - record is NOT undone (redo is invalid)
*
****************************************************************************/
BOOL
CSIMP::IsUndone( VOID *pvRecord )
{
   DWORD dw = *((DWORD *)((BYTE *) pvRecord + m_wReservedBytes));
   return( (dw & CSIMP_REDO) ? TRUE : FALSE );
}


/********************** CSIMP::IsVersioned **********************************
*
*HelpKey:         Public
*
*Description:
*  Determine if the record is versioned.
*
*Arguments:
*  VOID *pvRecord - record pointer
*
*Return Value:
*  TRUE  - record is versioned
*  FALSE - record is NOT versioned
*
****************************************************************************/
BOOL
CSIMP::IsVersioned( VOID *pvRecord )
{
   DWORD dw = *((DWORD *)((BYTE *) pvRecord + m_wReservedBytes));
   return( (dw & CSIMP_VERSIONED) ? TRUE : FALSE );
}


/********************** CSIMP::RedoVersion **********************************
*
*HelpKey:         Public
*
*Description:
*  Redo a version (set the current version available for undo).
*
*Arguments:
*  RCDX xRecord - absolute 1-based index into the record list
*  BOOL bAllVersions - TRUE  - Redo all undone versions
*                      FALSE - Redo one undone version only
*
*Return Value:
*  pointer to record after version is undone
*  0 - error (record was not versioned)
*
****************************************************************************/
VOID *
CSIMP::RedoVersion( RCDX xRecord,
                    BOOL bAllVersions )
{
   ASSERT_MSGBOX_RC( "Undo/Redo not enabled for RedoVersion", 0,
                     m_wFlags & CSIMP_UNDO_REDO, 0 );

   VOID *pvRecord = GetRecord( xRecord );
   if ( IsDirtySelectBitSet( pvRecord, CSIMP_REDO ) == FALSE )
   {
      return( 0 );
   }

   VOID **ppvVer;
   VOID **ppvRedo;
   VOID *pvVersion;

   do
   {
      SetDirtySelectBit( pvRecord, CSIMP_REDO, FALSE );
      ppvRedo = (VOID **)((BYTE *) pvRecord +
                          m_wReservedBytes - 2 * sizeof( VOID * ));
      pvVersion = *ppvRedo;   // link forward to redo version
      SetDirtySelectBit( pvVersion, CSIMP_VERSIONED, TRUE );
      ppvVer = (VOID **)((BYTE *) pvVersion +
                         m_wReservedBytes - sizeof( VOID * ));
      *ppvVer = pvRecord;    // link back to previous version
      pvRecord = pvVersion;
      if ( IsDirtySelectBitSet( pvRecord, CSIMP_REDO ) == FALSE )
      {
         bAllVersions = FALSE;
      }

   } while ( bAllVersions );

   *(CSIMP_GETRECORD_PTR( xRecord, m_pvRecords )) = pvVersion;
   return( pvVersion );
}


/********************** CSIMP::RemoveRecord *********************************
*
*HelpKey:         Public
*
*Description:
*  Remove a record from the list.
*
*Arguments:
*  const VOID *pvRemoveRecord - record to remove from the list
*
*Return Value:
*  TRUE  - record was removed from the list (and deleted)
*  FALSE - record was not located in the list (not removed)
*
****************************************************************************/
BOOL
CSIMP::RemoveRecord( const VOID *pvRemoveRecord )
{
   ASSERT_MSGBOX_RC( "Attempting to remove Null record", 0,
                     pvRemoveRecord, FALSE );
   RCDX k;

   for ( k = 1; k <= m_dwRecordCnt; k++ )
   {
      if ( pvRemoveRecord == CSIMP_GETRECORD( k, m_pvRecords ) )
      {
         if ( m_eSortDir == CSortItem::eDESCENDING )
         {
            k = m_dwRecordCnt - k + 1;
         }

         CSortIndex * pUseIdx = m_pUseIdx;   // hold current index in use
         m_pUseIdx = 0;
         BOOL bRC = RemoveRecord( k );
         if ( pUseIdx )
         {
            m_pUseIdx = pUseIdx;             // restore current index
         }

         return( bRC );
      }
   }

   return( FALSE );
}

/********************** CSIMP::RemoveRecord *********************************
*
*HelpKey:         Public
*
*Description:
*  Remove a record from the list.
*
*Arguments:
*  RCDX xRecord - 1-based absolute index of record to be removed
*
*Return Value:
*  TRUE  - record was removed from the list (and deleted)
*  FALSE - record was not located in the list (not removed)
*
****************************************************************************/
BOOL
CSIMP::RemoveRecord( RCDX xRecord )
{
   ASSERT_MSGBOX_RC( "Invalid index specified for RemoveRecord", 0,
                     xRecord > 0 && xRecord <= m_dwRecordCnt, FALSE );
   RCDX k;

   if ( m_pUseIdx )
   {
      if ( m_eSortDir == CSortItem::eDESCENDING )
      {
         xRecord = *(CSIMP_GETSORTIDX_PTR( m_dwRecordCnt - xRecord + 1,
                                           m_pUseIdx->m_pxIdx ));
      }
      else
      {
         xRecord = *(CSIMP_GETSORTIDX_PTR( xRecord, m_pUseIdx->m_pxIdx ));
      }

      xRecord++;  // 1-based index
   }

   VOID *pvRecord = CSIMP_GETRECORD( xRecord, m_pvRecords );
   if ( IsUndone( pvRecord ) )
   {
      pvRecord = RedoVersion( xRecord, TRUE );
   }

   if ( IsVersioned( pvRecord ) )
   {
      pvRecord = DeleteVersion( xRecord, TRUE );
   }

   for ( k = xRecord; k <= m_dwRecordCnt; k++ )
   {
      *(CSIMP_GETRECORD_PTR( k, m_pvRecords )) =
                               *(CSIMP_GETRECORD_PTR( k + 1, m_pvRecords ));
   }

   xRecord--;  // 0-based index
   CSortIndex *pSortIndex = m_pHead->m_pNext;
   while ( pSortIndex )
   {
      if ( pSortIndex->m_eSortState >= CSortIndex::eUNSORTED )
      {
         for ( k = 0; k < m_dwRecordCnt; k++ )
         {
            if ( *(CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx )) == xRecord )
            {
               for ( RCDX j = k; j < m_dwRecordCnt; j++ )
               {
                  *(CSIMP_GETSORTIDX_PTR( j, m_pUseIdx->m_pxIdx )) =
                     *(CSIMP_GETSORTIDX_PTR( j + 1, m_pUseIdx->m_pxIdx ));
               }
            }

            if ( *(CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx )) > xRecord )
            {
               (*(CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx )))--;
            }
         }
      }

      pSortIndex = pSortIndex->m_pNext;

   }  // end of:  while ( pSortIndex )

   m_dwRecordCnt--;

   DeleteRecord( pvRecord );
   return( TRUE );
}


/********************** CSIMP::RemoveSortIndex ******************************
*
*HelpKey:         Public
*
*Description:
*  Remove a sort index from a record set
*
*Arguments:
*  WORD wSortIndex - 1-based index
*
*Return Value:
*  TRUE  - index was located an removed
*  FALSE - index could not be located (was not removed)
*
****************************************************************************/
BOOL
CSIMP::RemoveSortIndex( WORD wSortIdx )      // uses 1-based index
{
   ASSERT_MSGBOX_RC( "Invalid index specified for RemoveSortIndex", 0,
                     wSortIdx > 0, FALSE );

   CSortIndex *pSortIndex = m_pHead;
   CSortIndex *pTempIndex;
   while ( pSortIndex->m_pNext )
   {
      if ( pSortIndex->m_pNext->m_wIndex == wSortIdx )
      {
         pTempIndex = pSortIndex->m_pNext;
         pSortIndex->m_pNext = pSortIndex->m_pNext->m_pNext;
         if ( m_pTail == pTempIndex )
         {
            m_pTail = pSortIndex;
         }

         delete( pTempIndex );
         return( TRUE );
      }

      pSortIndex = pSortIndex->m_pNext;
   }

   return( FALSE );
}


/********************** CSIMP::RestructureRecord ****************************
*
*HelpKey:         Public
*
*Description:
*  Restructure a record based on a redefined data descriptor.  This may
*  be used to change a data descriptor in the record BEFORE the record
*  is complete (CSIMP::Enable has not been called)
*
*Arguments:
*  CDateDescriptor *pDD - pointer to the data descriptor class for a field
*  CSIMP_DATATYPE - data type of the field being revised in the record
*  WORD wRawLth - raw length of the field being revised in the record (if
*                 zero, length will not be reset)
*
*Return Value:
*  eCSIMP_RC_SUCCESS - Record successfully restructured
*  eCSIMP_RC_FAILURE - Record NOT restructured
*
****************************************************************************/
eCSIMP_RC
CSIMP::RestructureRecord( CDataDescriptor *pDD,
                          CSIMP_DATATYPE DataType,
                          WORD wRawLth )
{
   WORD k = GetDataDescriptorIndex( pDD );
   BOOL bRemove = FALSE;
   ASSERT_MSGBOX_RC( "Cannot locate data descriptor in RestructureRecord",
                     0, k, eCSIMP_RC_FAILURE );

#if 0   // this code may be valuable if there ever needs to be difference
        // between the size of the bound buffer the the stored buffer ...
        // as there is for the TIMESTAMP stuff (which is already handled)
   if ( pDD->m_DataType == DataType && wRawLth &&
        wRawLth < pDD->m_wRawLth &&
        ((DataType & ~CSIMP_HIGHBITS) == CSIMP_SDATETIME ||
         (DataType & ~CSIMP_HIGHBITS) == CSIMP_TIMESTAMP) )
   {
      pDD->m_DataType |= CSIMP_UNDERFLOW;
      m_wFlags |= CSIMP_FMTIMESTAMPS_USED;
   }
   else
   {
      pDD->m_DataType = DataType;
   }

#else

   pDD->m_DataType = DataType;

#endif

   if ( k == 0 || wRawLth == 0 )       // ==> not changing length
   {
      return( eCSIMP_RC_SUCCESS );
   }

   ASSERT_MSGBOX_RC( "Altering record structure after data retrieval", 0,
                     m_dwRecordCnt == 0, eCSIMP_RC_FAILURE );

   LONG lDiff;
   if ( k < m_wDescriptorCnt )   // we know k > 0 if we got here
   {
      // k is 1 based ... m_pDD is zero based
      lDiff = m_pDD[ k ]->m_wOffset - pDD->m_wOffset;
   }
   else
   {
      lDiff = m_wRecordLth - pDD->m_wOffset;
   }

   if ( wRawLth == -1 )    // ==> remove data descriptor from record
   {
      // k has been 1-based ... now need 0-based ==> leave it alone
      while ( k < m_wDescriptorCnt )
      {
         m_pDD[ k - 1 ] = m_pDD[ k ];
         m_pDD[ k - 1 ]->m_wOffset -= lDiff;
         k++;
      }

      m_wRecordLth -= lDiff;
      m_wDescriptorCnt--;
      delete( pDD );
   }
   else         // shift by change is length
   {
      if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_VARCHAR )
      {
         wRawLth += sizeof( WORD ) + 1;  // null terminator + lth bytes
         m_wFlags |= CSIMP_VARCHAR_USED;
      }
      else
      if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_NTSTRING )
      {
         wRawLth++;       // allow for null terminator
      }
      else
      if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_DESENCRYPT )
      {
         wRawLth = DBM_SIZE_DESENCRYPT( wRawLth );
      }

      /* report correct raw length (prior to rounding up to even byte
         boundary) */
      pDD->m_wRawLth = wRawLth;

      if ( (DataType & ~CSIMP_HIGHBITS) == CSIMP_CHAR )
      {
         wRawLth++;  // allow for null terminator but do not report extra byte
      }

      if ( wRawLth > m_wMaxFixedPoolFieldLth )
      {
         pDD->m_DataType |= CSIMP_ADDRESS;
         wRawLth = sizeof( VOID * );
      }
      else
      {
         /* keep even byte boundaries (future for 32-bit) */
         wRawLth += (wRawLth % 2);
      }

      if ( (DWORD) lDiff == wRawLth )
      {
         return( eCSIMP_RC_SUCCESS );
      }

      lDiff -= wRawLth;
      while ( k < m_wDescriptorCnt )
      {
         m_pDD[ k++ ]->m_wOffset -= lDiff;
      }

      m_wRecordLth -= lDiff;
   }

   return( eCSIMP_RC_SUCCESS );
}


/********************** CSIMP::SeekRecord ***********************************
*
*HelpKey:         Public
*
*Description:
*  Search for a record using data in another (seek) record via a binary
*  search algorithm.
*
*Arguments:
*  VOID *pvSeekRecord - record whose data is used to search for a record
*                       in the list
*
*Return Value:
*  0         - could not locate the record based on the search criteria in
*              the seek record
*  otherwise - index to the record pointer
*
****************************************************************************/
/* returns the record pointer index */
RCDX
CSIMP::SeekRecord( const VOID *pvSeekRecord )
{
   if ( m_dwRecordCnt == 0 )
   {
      return( 0 );
   }

   ASSERT_MSGBOX_RC( "SortIndex required for SeekRecord", 0,
                     m_pUseIdx, 0 );
   if ( m_pUseIdx->m_eSortState != CSortIndex::eSORTED )
   {
      m_Error = eCSIMP_RC_INVALID_IDX;
      return( 0 );
   }

   RCDX xLower = 0;
   RCDX xUpper = m_dwRecordCnt - 1;
   RCDX xMid;
   short k;

   while ( xLower <= xUpper )
   {
      xMid = (xUpper + xLower) >> 1;      // ">> 1" == "divide by 2"

      k = CompareRecords( pvSeekRecord,
                          CSIMP_GETSORTREC( xMid + 1, m_pvRecords,
                                            m_pUseIdx->m_pxIdx ) );
      if ( k > 0 )
         xLower = xMid + 1;
      else
      if ( k < 0 )
         xUpper = xMid - 1;
      else
      {
//       return( m_pUseIdx->m_pxIdx[ xMid ] + 1 );
         return( xMid + 1 );
      }
   }

   m_Error = eCSIMP_RC_NOTFND;
   return( 0 );
}


extern DATEMASK l_Datemask;  // = { '/', CMN_DATE_MMDDYYYY };
extern TIMEMASK l_Timemask;  // = { ':', CMN_TIME_HHMMSSMMMAM };

/********************** CSIMP::SetBindRecord ********************************
*
*HelpKey:         Public
*
*Description:
*  Set a record from another record where the first record is the bind
*  record.  The bind record may contain variable length (pascal type)
*  strings and strings that have trailing spaces.
*
*  At some opportune moment, this method should be broken into two methods,
*  one for the "from bind" and another for the "to bind" case.  Originally
*  there was significant common code for the two cases, but no longer.
*
*  Sybase returns date strings in the format:  Apr 6 1995 8:47:25:356AM
*  DB Master changes this format using the ConvertToCmnDate function into
*  the "common format":  04/06/1995 08:47:25:356AM
*
*Arguments:
*  VOID *pvBindRecord - Bind record
*  VOID *pvRecord - Target or source record depending on bFromBindRecord
*  BOOL bFromBindRecord - TRUE  - set (target) record from bind record
*                         FALSE - set bind record from (source) record
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::SetBindRecord( VOID *pvBindRecord,
                      VOID *pvRecord,
                      BOOL bFromBindRecord )
{
   /* we are after performance here because this is called a lot of
      times.  So we are optimizing for speed (in-lining function calls
      where possible) */
   CDataDescriptor *pDD;
   LPSTR pchSrcData;
   WORD k = m_wDescriptorCnt;
   if ( bFromBindRecord )
   {
      DWORD *pdw = (DWORD *)((BYTE *) pvBindRecord + m_wReservedBytes);
      BOOL bFix = (m_wFlags & CSIMP_VARCHAR_USED) || !(*pdw & CSIMP_FIXED_UP);
      *pdw |= CSIMP_FIXED_UP;
      while ( k )
      {
         pDD = GetDataDescriptor( k );    // 1-based index
         pchSrcData = (char *) pDD->GetDataAddress( pvBindRecord );
         if ( bFix )
         {
            if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_VARCHAR )
            {
               /* strip the leading length bytes */
               WORD *pw = (WORD *) pchSrcData;
               pchSrcData[ *pw + sizeof( WORD ) ] = 0;
               memcpy( pchSrcData, pchSrcData + sizeof( WORD ), *pw + 1 );
            }
            else
            if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_CHAR )
            {
               /* strip the trailing length bytes */
               WORD wLth = pDD->GetRawLth( );   // note an extra byte is
                                                // allocated but not reported
               while ( pchSrcData[ wLth - 1 ] == ' ' )
               {
                  wLth--;
               }

               pchSrcData[ wLth ] = 0;
            }
         }

         if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_FMTIMESTAMP )
         {
            /* the value at the src data is a pointer to the real string
               timestamp data ... dereference pointer to pointer */
            LPSTR lpTimestamp = *(LPSTR *)(pchSrcData);
            WORD wOffset = pDD->GetOffset( );
            DWORD *pdw = (DWORD *) ((BYTE *) pvRecord + wOffset);
            Cmn_StringToDateTime( lpTimestamp,
                                  pdw, &l_Datemask,
                                  pdw + 1, &l_Timemask );
         }
         else
#if 0
         if ( pDD->m_DataType & CSIMP_UNDERFLOW )
         {
            WORD wLth = pDD->GetRawLth( ) - 1;

            /* the value at the src data is a pointer to the real string
               datetime data ... dereference pointer to pointer */
            LPSTR lpDatetime = *(LPSTR *)(pchSrcData);
            WORD wOffset = pDD->GetOffset( );
            LPSTR lpRealStorage = (LPSTR) ((BYTE *) pvRecord + wOffset);
            strncpy( lpRealStorage, lpDatetime, wLth );
            lpRealStorage[ wLth ] = 0;
         }
         else
#endif
         {
            SetDataInRecord( pDD, pchSrcData, -1, pvRecord );
         }

         k--;
      }
   }
   else
   {
      /* going to the bind record from standard record */
      if ( (m_wFlags & CSIMP_VARCHAR_USED) ||
           (m_wFlags & CSIMP_FMTIMESTAMPS_USED) )
      {
         while ( k )
         {
            pDD = GetDataDescriptor( k );    // 1-based index
            pchSrcData = (LPSTR) pDD->GetDataAddress( pvRecord );
            if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_VARCHAR )
            {
               /* strip the leading length bytes */
               WORD wLth = strlen( pchSrcData );
               VOID *pv = pDD->GetDataAddress( pvBindRecord );
               WORD *pw = (WORD *) pv;
               *pw = wLth;
               memcpy( (BYTE *) pv + sizeof( WORD ),
                       pchSrcData + sizeof( WORD ), wLth - 2 );
            }
            else
            if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_FMTIMESTAMP )
            {
               /* the value in the src buffer is two DWORDs representing
                  date and time.  The bind buffer contains a pointer to
                  the string representation of the datetime value.  Decode
                  and set the string datetime in the bind buffer's
                  dereferenced pointer to pointer */
               WORD wOffset = pDD->GetOffset( );
               LPSTR lpTimeStamp = *(LPSTR *) ((BYTE *) pvBindRecord + wOffset);
               DWORD *pdw = (DWORD *) ((BYTE *) pvRecord + wOffset);
               Cmn_DateToString( lpTimeStamp, *pdw, &l_Datemask );
               lpTimeStamp += strlen( lpTimeStamp );
               *lpTimeStamp = ' ';
               Cmn_TimeToString( lpTimeStamp + 1, *(pdw + 1), &l_Timemask );
            }
            else
#if 0
            if ( pDD->m_DataType & CSIMP_UNDERFLOW )
            {
               WORD wOffset = pDD->GetOffset( );
               LPSTR lpTimeStamp = *(LPSTR *) ((BYTE *) pvBindRecord + wOffset);
               LPSTR lpRealData = (LPSTR) ((BYTE *) pvRecord + wOffset);
               strcpy( lpTimeStamp, lpRealData );
            }
            else
#endif
            {
               SetDataInRecord( pDD, pchSrcData, -1, pvBindRecord );
            }

            k--;
         }
      }
      else
      {
         SetRecordFromRecord( pvBindRecord, pvRecord );
      }
   }
}


/********************** CSIMP::SetDataInRecord ******************************
*
*HelpKey:         Public
*
*Description:
*  Set the data for a field in a record.
*
*Arguments:
*  CDataDescriptor pDD - descriptor of field in the record
*  VOID *pvData - raw data to set in the field
*  WORD wDataLth - may be specified as follows for variable length data:
*                   -1 - use raw data length
*                    0 - indicates nul (nothing done with this yet)
*                    otherwise - use the specified length (must include
*                                null terminator)
*  RCDX xRecord - absolute 1-based index into the record list for the
*                 target record
*Return Value:
*  eCSIMP_RC_SUCCESS - Record successfully updated with data
*  eCSIMP_RC_FAILURE - Record not updated with data
*
****************************************************************************/
eCSIMP_RC
CSIMP::SetDataInRecord( CDataDescriptor *pDD,
                        VOID *pvData,
                        WORD wDataLth,
                        RCDX xRecord )    // absolute 1-based index into
                                          // the input order list
{
   ASSERT_MSGBOX_RC( "Invalid index specified for SetDataInRecord", 0,
                     xRecord > 0, eCSIMP_RC_FAILURE );
   return( SetDataInRecord( pDD, pvData, wDataLth,
                            CSIMP_GETRECORD( xRecord, m_pvRecords ) ) );
}

/********************** CSIMP::SetDataInRecord ******************************
*
*HelpKey:         Public
*
*Description:
*  Set the data for a field in a record.
*
*Arguments:
*  CDataDescriptor pDD - descriptor of field in the record
*  VOID *pvData - raw data to set in the field
*  WORD wDataLth - may be specified as follows for variable length data:
*                   -1 - use raw data length
*                    0 - indicates nul (nothing done with this yet)
*                    otherwise - use the specified length (must include
*                                null terminator)
*  VOID *pvTgtRecord - target record for which to set the field data
*
*Return Value:
*  eCSIMP_RC_SUCCESS - Record successfully updated with data
*  eCSIMP_RC_FAILURE - Record not updated with data
*
****************************************************************************/
eCSIMP_RC
CSIMP::SetDataInRecord( CDataDescriptor *pDD,
                        VOID *pvData,
                        WORD wDataLth,
                        VOID *pvTgtRecord )
{
   ASSERT_MSGBOX_RC( "Invalid data descriptor specified for SetDataInRecord",
                     0, pDD, eCSIMP_RC_FAILURE );
   ASSERT_MSGBOX_RC( "Null data specified for SetDataInRecord", 0,
                     pvData, eCSIMP_RC_FAILURE );
   ASSERT_MSGBOX_RC( "Invalid data length specified for SetDataInRecord", 0,
                     wDataLth, eCSIMP_RC_FAILURE ); // doing nothing with nul

   WORD wOffset = pDD->GetOffset( );
   WORD wRawLth = pDD->GetRawLth( );

   VOID *pv;
   if ( (m_wFlags & CSIMP_VARSIZE_USED) && pDD->IsVarSize( ) )
   {
      /* if we are in a variable situation, use the true length of the
         data for the variable allocation (unless nDataLth has been set
         to something greater than the true length, in which case use
         wDataLth) */
      RCDX *px;
      if ( (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_VARCHAR ||
           (pDD->m_DataType & ~CSIMP_HIGHBITS) == CSIMP_NTSTRING )
      {
         if ( wDataLth == -1 )
         {
            ASSERT_MSGBOX( "Null terminator not located in varchar", 0,
                 memchr( pvData, 0, wRawLth ) ); // ensure null terminator
            wRawLth = strlen( (char *) pvData ) + 1;
         }
         else
         {
            ASSERT_MSGBOX( "Maximum record length exceeded", 0,
                           wDataLth <= wRawLth );
            ASSERT_MSGBOX( "Null terminator not located in varchar", 0,
                 memchr( pvData, 0, wDataLth ) ); // ensure null terminator
            wRawLth = wDataLth;
         }

#ifdef _DEBUG
         char *pch = (char *) pvData;
         pch[ wRawLth - 1 ] = 0;
#endif
      }

      /* - allocate the buffer for the pascal type variable length data
           (includes its length at the beginning of the allocated buffer)
         - put variable-length buffer's address into the fixed pool buffer
           at the correct offset
         - put the allocated buffer's length into the allocated buffer
         - set the pointer just past the length bytes in the allocated buffer
      */
      VOID **pvRec = (VOID **) ((BYTE *) pvTgtRecord + wOffset);
      pv = 0;
      if ( *pvRec )
      {
         pv = *pvRec;      // would point to pxNul using GetDataAddress
         if ( pv == 0 )
         {
            SetDirtySelectBit( pvTgtRecord, CSIMP_DIRTY, TRUE );
         }
         else
         {
            px = (RCDX *) pv;
            pv = (VOID *) ((BYTE *) pv + sizeof( RCDX ));
            if ( *px == wRawLth &&
                 memcmp( pv, pvData, wRawLth ) == 0 )
            {
               return( eCSIMP_RC_SUCCESS );
            }

            SetDirtySelectBit( pvTgtRecord, CSIMP_DIRTY, TRUE );

            /* if possible, retain old allocation */
            if ( *px >= wRawLth ||
                 IsDirtySelectBitSet( pvTgtRecord, CSIMP_FULL ) )
            {
               pv = *pvRec;
               *px = wRawLth;
            }
            else
            {
               m_pPoolVarData->Free( *pvRec );  // wash away old variable data
               pv = 0;
            }
         }
      }

      if ( pv == 0 )
      {
         pv = m_pPoolVarData->Alloc( wRawLth + sizeof( RCDX ) );
         VERIFY( pv );
         memset( pv, 0, wRawLth + sizeof( RCDX ) );  // zeroinit
         *pvRec = pv;
         px = (RCDX *) pv;
         *px = wRawLth;
      }

      pv = (VOID *) ((BYTE *) pv + sizeof( RCDX ));
   }
   else
   {
      pv = (VOID *) ((BYTE *) pvTgtRecord + wOffset);
      if ( memcmp( pv, pvData, wRawLth ) == 0 )
      {
         return( eCSIMP_RC_SUCCESS );
      }

      SetDirtySelectBit( pvTgtRecord, CSIMP_DIRTY, TRUE );
   }

   memcpy( pv, pvData, wRawLth );
   return( eCSIMP_RC_SUCCESS );
}


/********************** CSIMP::SetDirtySelectBit ****************************
*
*HelpKey:         Public
*
*Description:
*  Sets a dirty (record changed) or select (record selected) or other (user
*  defined) bit set for the record.
*
*Arguments:
*  VOID *pvRecord - record for which information is being set
*  DWORD dwBitPattern - bit pattern being set (e.g.  dirty flag or
*                       select set)
*  BOOL bSetOn - TRUE  - set dirty/select bit ON
*                FALSE - set dirty/select bit OFF
*
*Return Value (previous state of bit pattern):
*  TRUE  - record was dirty/selected
*  FALSE - record was NOT dirty/selected
*
****************************************************************************/
BOOL
CSIMP::SetDirtySelectBit( VOID *pvRecord,
                          DWORD dwBitPattern,
                          BOOL bSetOn )
{
   DWORD *pdw;
   pdw = (DWORD *)((BYTE *) pvRecord + m_wReservedBytes);
   BOOL bRC = (*pdw & dwBitPattern) ? TRUE : FALSE;
   *pdw = bSetOn ? (*pdw | dwBitPattern) : (*pdw & ~dwBitPattern);
   return( bRC );
}


/********************** CSIMP::SetRecordFromRecord **************************
*
*HelpKey:         Public
*
*Description:
*  Set a record from another record field by field (raw data).
*
*Arguments:
*  VOID *pvTgtRecord - Target record
*  VOID *pvSrcRecord - Source record
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::SetRecordFromRecord( VOID *pvTgtRecord,
                            VOID *pvSrcRecord )
{
   CDataDescriptor *pDD;
   VOID *pvSrcData;
   WORD k = m_wDescriptorCnt;
   while ( k )
   {
      pDD = GetDataDescriptor( k );    // 1-based index
      pvSrcData = pDD->GetDataAddress( pvSrcRecord );
      SetDataInRecord( pDD, pvSrcData, -1, pvTgtRecord );
      k--;
   }
}


/********************** CSIMP::ShareData ************************************
*
*HelpKey:         Public
*
*Description:
*  Share record structures and memory pools
*
*Arguments:
*  CSIMP *pSrcCSIMP - CSIMP with which to share structure
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::ShareData( CSIMP *pSrcCSIMP )
{
   m_wFlags = pSrcCSIMP->m_wFlags;
   m_wDescriptorCnt = pSrcCSIMP->m_wDescriptorCnt;
   m_wDescriptorMax = pSrcCSIMP->m_wDescriptorMax;
   m_dwDirtySelect = pSrcCSIMP->m_dwDirtySelect;
   if ( m_pDD )
   {
      m_pPoolVariable->Free( m_pDD );
   }

   m_pDD = (CDataDescriptor **)
   m_pPoolVariable->Alloc( m_wDescriptorMax * sizeof( CDataDescriptor * ) );
   WORD k = m_wDescriptorCnt;
   while ( k )
   {
      k--;
      m_pDD[ k ] = new CDataDescriptor( *(pSrcCSIMP->m_pDD[ k ]) );
   }

   m_wReservedBytes = pSrcCSIMP->m_wReservedBytes;
   m_wRecordLth = pSrcCSIMP->m_wRecordLth;
   m_wApplBytes = pSrcCSIMP->m_wApplBytes;
}


/********************** CSIMP::UndoVersion **********************************
*
*HelpKey:         Public
*
*Description:
*  Undo a version (set the previous version available for redo).
*
*Arguments:
*  RCDX xRecord - absolute 1-based index into the record list
*  BOOL bAllVersions - TRUE  - Undo all versions (record no longer versioned)
*                      FALSE - Undo current version only
*
*Return Value:
*  pointer to record after version is undone
*  0 - error (record was not versioned)
*
****************************************************************************/
VOID *
CSIMP::UndoVersion( RCDX xRecord,
                    BOOL bAllVersions )
{
   ASSERT_MSGBOX_RC( "Undo/Redo not enabled for UndoVersion", 0,
                     m_wFlags & CSIMP_UNDO_REDO, 0 );

   VOID *pvRecord = GetRecord( xRecord );
   if ( IsDirtySelectBitSet( pvRecord, CSIMP_VERSIONED ) == FALSE )
   {
      return( 0 );
   }

   VOID **ppvRedo;
   VOID *pvVersion;

   do
   {
      pvVersion = *((VOID **)((BYTE *) pvRecord +  // link back to prev version
                              m_wReservedBytes - sizeof( VOID * )));
      ppvRedo = (VOID **)((BYTE *) pvVersion +
                          m_wReservedBytes - 2 * sizeof( VOID * ));
      *ppvRedo = pvRecord;    // link forward to redo version
      SetDirtySelectBit( pvVersion, CSIMP_REDO, TRUE );

      if ( IsDirtySelectBitSet( pvVersion, CSIMP_VERSIONED ) == FALSE )
      {
         bAllVersions = FALSE;
      }

   } while ( bAllVersions );

   *(CSIMP_GETRECORD_PTR( xRecord, m_pvRecords )) = pvVersion;
   return( pvVersion );
}


/********************** CSIMP::UseSortType **********************************
*
*HelpKey:         Public
*
*Description:
*  Set the sort algorithm to be used when sorting is required.
*
*Arguments:
*  eSORTTYPE eSortType - eHEAPSORT or eQUICKSORT
*
*Return Value:
*  none
*
****************************************************************************/
VOID
CSIMP::UseSortType( eSORTTYPE eSortType )
{
   if ( eSortType == eHEAPSORT )
   {
      m_wFlags &= ~CSIMP_QUICKSORT;
   }
   else
   {
      m_wFlags |= CSIMP_QUICKSORT;
   }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Heap sort or quick sort algorithm implementations for resorting a       //
// CSIMP index, using an array of pointers as the index.                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Utility PrivateDisplaySortTime function for timing sorts                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#include <sys\timeb.h>
#include <time.h>
#include <stdio.h>
#endif

/********************** PrivateDisplaySortTime ******************************
*
*HelpKey:         Private
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
PrivateDisplaySortTime( LPCSTR szText = "",
                        WORD wFlag = 0 )     // 1 - show elapsed time
                                             // 2 - reset start time
{
#if 0
   char szBuffer[ 128 ];
   struct _timeb timebuffer;
   char *timeline;

   _ftime( &timebuffer );
   timeline = ctime( &(timebuffer.time) );
   sprintf( szBuffer, "%%s Time: %.19s.%hu %s",
            timeline, timebuffer.millitm, timeline + 20 );
   ASSERT_TRACE( szBuffer, szText, FALSE );
#endif
#ifdef _DEBUG
   static struct _timeb TimeStart;
   struct _timeb TimeNow;
   char szBuffer[ 128 ];
   char *timeline;

   _ftime( &TimeNow );
   timeline = ctime( &(TimeNow.time) );
   sprintf( szBuffer, "%%s Time: %.19s.%hu %s",
            timeline, TimeNow.millitm, timeline + 20 );
   VERIFY_TRACE( szBuffer, szText, FALSE );

   if ( wFlag & 1 )     // show elapsed time
   {
      DWORD dwSeconds = TimeNow.time - TimeStart.time;
      WORD wMilliSeconds;
      if ( TimeNow.millitm < TimeStart.millitm )
      {
         dwSeconds--;
         wMilliSeconds = 1000 + TimeNow.millitm - TimeStart.millitm;
      }
      else
      {
         wMilliSeconds = TimeNow.millitm - TimeStart.millitm;
      }

      sprintf( szBuffer, "Elapsed Time: %lu seconds %u ms",
               dwSeconds, wMilliSeconds );
      VERIFY_TRACE( szBuffer, 0, FALSE );
   }

   if ( wFlag & 2 )     // reset start time
   {
      memcpy( &TimeStart, &TimeNow, sizeof( struct _timeb ) );
   }
#endif
}


/********************** CSIMP::ResortSortIndex ******************************
*
*HelpKey:         Public
*
*Description:
*  Resorts the data for a specified sort index.
*
*Arguments:
*  WORD wSortIndex - 1-based index
*
*Return Value:
*  eCSIMP_RC_DUP - duplicate records were encountered ... sort was successful
*  eCSIMP_RC_SUCCESS - sort was successful
*
****************************************************************************/
eCSIMP_RC
CSIMP::ResortSortIndex( WORD wSortIdx )     // uses 1-based index
{
   ASSERT_MSGBOX_RC( "Invalid search index for ResortSortIndex", 0,
                     wSortIdx > 0, eCSIMP_RC_FAILURE );

   CSortIndex *pHoldIndex = m_pUseIdx;
   m_pUseIdx = GetSortIndex( wSortIdx );
   ASSERT_MSGBOX_RC( "Undefined search index for ResortSortIndex", 0,
                     m_pUseIdx, eCSIMP_RC_FAILURE );
   ASSERT_MSGBOX_RC( "Incomplete search index specification for "
                     "ResortSortIndex", 0,
                     m_pUseIdx->m_pHead->m_pNext, eCSIMP_RC_FAILURE );

   m_Error = eCSIMP_RC_SUCCESS;
   m_pUseIdx->m_eSortState = CSortIndex::eSORTED;

   if ( m_dwRecordCnt > 1 )
   {
      if ( m_wFlags & CSIMP_QUICKSORT )
      {
#ifdef _DEBUG
         PrivateDisplaySortTime( "Begin QuickSort", 2 );
#endif

         QuickSortNR( 1, m_dwRecordCnt );

#ifdef _DEBUG
         PrivateDisplaySortTime( "End QuickSort", 3 );
#endif

#ifdef _DEBUGG
         PrivateDisplaySortTime( "Begin Quicksort Recursive", 2 );
         QuickSort( 1, m_dwRecordCnt );
         PrivateDisplaySortTime( "EndQuicksort Recursive", 3 );
         PrivateDisplaySortTime( "Begin Heapsort", 2 );
         HeapSort( );
         PrivateDisplaySortTime( "EndHeapsort", 3 );
#endif
      }
      else
      {
#ifdef _DEBUG
         PrivateDisplaySortTime( "Begin HeapSort", 2 );
#endif

         HeapSort( );

#ifdef _DEBUG
         PrivateDisplaySortTime( "End HeapSort", 3 );
#endif

#ifdef _DEBUGG
         PrivateDisplaySortTime( "Begin Quicksort Recursive", 2 );
         QuickSort( 1, m_dwRecordCnt );
         PrivateDisplaySortTime( "EndQuicksort Recursive", 3 );
         PrivateDisplaySortTime( "Begin QuicksortNR", 2 );
         QuickSortNR( 1, m_dwRecordCnt );
         PrivateDisplaySortTime( "End QuicksortNR", 3 );
#endif
      }
   }

   m_pUseIdx = pHoldIndex;
   return( m_Error );
}


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
LONG plTest[ ] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 0 };


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
into its correct final position by exchariging it with A[ n ]. By discarding
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

//#define _DEBUGG
#ifdef _DEBUGG

/*************************** ShowHeap **************************************
*
*HelpKey:         Private
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
ShowHeap( CSIMP *pcs,
          RCDX *pxBase )
{
   char szBuffer[ 16 ];
   RCDX k;
   LONG l;

   szBuffer[ 0 ] = '\t';
   for ( k = 1; k <= pcs->m_dwRecordCnt; k++ )
   {
      ltoa( k, szBuffer + 1, 10 );
      OutputDebugString( szBuffer );
      if ( k < 10 )
         OutputDebugString( ".  - " );
      else
         OutputDebugString( ". - " );

      ltoa( pxBase[ k - 1 ] + 1, szBuffer + 1, 10 );
      OutputDebugString( szBuffer );
      if ( k < 10 )
         OutputDebugString( "  - " );
      else
         OutputDebugString( " - " );

      pcs->GetDataFromRecord( pcs->GetDataDescriptor( 1 ),
                              &l, sizeof( l ), pxBase[ k - 1 ] + 1 );
      ltoa( l, szBuffer + 1, 10 );
      OutputDebugString( szBuffer + 1 );
      OutputDebugString( "\n" );
   }
}  // end of:  ShowHeap

#endif


/*************************** CSIMP::HeapSort ********************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
CSIMP::HeapSort( VOID )
{
   BuildHeap( );

#ifdef _DEBUGG
   OutputDebugString( "\nAfter build heap ...\n" );
#endif

   RCDX xHoldCnt = m_dwRecordCnt;
   UINT uHoldMax = m_lMaxRecords;
   UINT *p1;
   UINT u;
   while ( m_dwRecordCnt > 1 )
   {
      m_dwRecordCnt--;
      m_lMaxRecords--;

      p1 = CSIMP_GETSORTIDX_PTR( m_dwRecordCnt + 1, m_pUseIdx->m_pxIdx );
      u = *p1;
      *p1 = *(m_pUseIdx->m_pxIdx);
      *(m_pUseIdx->m_pxIdx) = u;

      u = Heapify( 1 );
      while ( u )
      {
         u = Heapify( u );
      }
   }

   m_dwRecordCnt = xHoldCnt;
   m_lMaxRecords = uHoldMax;


#ifdef _DEBUGG
   OutputDebugString( "\nAfter heap sort ...\n" );
   ShowHeap( this, m_pUseIdx->m_pxIdx );
#endif
}

#if 0
VOID
CSIMP::BuildHeap( VOID )   // recursive
{
   UINT k;

#ifdef _DEBUGG
   OutputDebugString( "\nShowing heap prior to build heap ...\n" );
   ShowHeap( this, m_pUseIdx->m_pxIdx );
#endif

   k = m_dwRecordCnt / 2;
   while( k > 0 )
   {
      Heapify( k-- );
   }

#ifdef _DEBUGG
   OutputDebugString( "\nShowing heap at end of build heap ...\n" );
   ShowHeap( this, m_pUseIdx->m_pxIdx );
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
/*************************** CSIMP::Heapify *********************************
*
*HelpKey:         Private
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
CSIMP::Heapify( UINT k )
{
   ASSERT_TRACE( "Heapify error", 0, k <= m_dwRecordCnt );
   UINT uLeft = k << 1;
   UINT uLargest;

   if ( uLeft > m_dwRecordCnt )
   {
      return;
   }

   uLargest = k;
   if ( CompareRecords( uLeft, uLargest ) > 0 )
   {
      uLargest = uLeft;
   }

   uLeft++;      // uRight (use uLeft to conserve stack space)
   if ( uLeft <= m_dwRecordCnt )
   {
      if ( CompareRecords( uLeft, uLargest ) > 0 )
      {
         uLargest = uLeft;
      }
   }

   if ( k != uLargest )
   {
      UINT *p1;
      UINT *p2;

      /* use uLeft to conserve stack */
      p1 = CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx );
      p2 = CSIMP_GETSORTIDX_PTR( uLargest, m_pUseIdx->m_pxIdx );
      uLeft = *p1;
      *p1 = *p2;
      *p2 = uLeft;

#ifdef _DEBUGG
      OutputDebugString( "\nShowing heap at end of change to heap ...\n" );
      ShowHeap( this, m_pUseIdx->m_pxIdx );
#endif

      Heapify( uLargest );
   }
}  // end of:  Heapify

#else

/*************************** CSIMP::BuildHeap *******************************
*
*HelpKey:         Private
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
CSIMP::BuildHeap( VOID )   // non-recursive
{
   UINT k;

#ifdef _DEBUGG
   OutputDebugString( "\nShowing heap prior to build heap ...\n" );
   ShowHeap( this, m_pUseIdx->m_pxIdx );
#endif

   k = m_dwRecordCnt / 2;
   while( k > 0 )
   {
      UINT u = Heapify( k-- );
      while ( u )
      {
         u = Heapify( u );
      }
   }

#ifdef _DEBUGG
   OutputDebugString( "\nShowing heap at end of build heap ...\n" );
   ShowHeap( this, m_pUseIdx->m_pxIdx );
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
/*************************** CSIMP::Heapify *********************************
*
*HelpKey:         Private
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
UINT
CSIMP::Heapify( UINT k )
{
   ASSERT_TRACE( "Heapify error", 0, k <= m_dwRecordCnt );
   UINT uLeft = k << 1;
   UINT uLargest;

   if ( uLeft > m_dwRecordCnt )
   {
      return( 0 );
   }

   uLargest = k;
   if ( CompareRecords( uLeft, uLargest ) > 0 )
   {
      uLargest = uLeft;
   }

   uLeft++;      // uRight (use uLeft to conserve stack space)
   if ( uLeft <= m_dwRecordCnt )
   {
      if ( CompareRecords( uLeft, uLargest ) > 0 )
      {
         uLargest = uLeft;
      }
   }

   if ( k != uLargest )
   {
      UINT *p1;
      UINT *p2;

      /* use uLeft to conserve stack */
      p1 = CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx );
      p2 = CSIMP_GETSORTIDX_PTR( uLargest, m_pUseIdx->m_pxIdx );
      uLeft = *p1;
      *p1 = *p2;
      *p2 = uLeft;

#ifdef _DEBUGG
      OutputDebugString( "\nShowing heap at end of change to heap ...\n" );
      ShowHeap( this, m_pUseIdx->m_pxIdx );
#endif
      return( uLargest );
   }

   return( 0 );
}  // end of:  Heapify

#endif

/*************************** CSIMP::QuickSort *******************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
CSIMP::QuickSort( RCDX xRecord1,    // recursive
                  RCDX xRecord2 )
{
   static WORD wRecurseCnt = 0;
   wRecurseCnt++;
#ifdef _DEBUG
   static WORD wMaxRecurse = 0;
   if ( wMaxRecurse < wRecurseCnt )
   {
      wMaxRecurse = wRecurseCnt;
   }
#endif

   if ( xRecord1 < xRecord2 )
   {
      UINT u = QuickSortPartition( xRecord1, xRecord2 );

      QuickSort( xRecord1, u );
      QuickSort( u + 1, xRecord2 );
   }

   wRecurseCnt--;

#ifdef _DEBUG
   if ( wRecurseCnt == 0 )
   {
      ASSERT_TRACE( "Maximum quicksort recurse count = %d", wMaxRecurse, FALSE );
//    OutputDebugString( "Maximum recurse count = " );
//    ltoa( wMaxRecurse, szBuffer, 10 );
//    OutputDebugString( szBuffer );
//    OutputDebugString( "\n" );
   }
#endif
}


VOID
/*************************** CSIMP::QuickSortNR *****************************
*
*HelpKey:         Public
*
*Description:
*  Non recursive quicksort
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSIMP::QuickSortNR( RCDX xRecord1,    // non-recursive
                    RCDX xRecord2 )
{
   ASSERT_TRACE( "QuickSort argument error", 0, xRecord1 <= xRecord2 );
   UINT m = xRecord2 - xRecord1;
   UINT uStackSize = 2;
   while ( m >>= 1 )
   {
      uStackSize++;
   }

   uStackSize <<= 2;     // two entries for each comparison
   UINT *puStack = (UINT *)
                       m_pPoolVarData->Alloc( uStackSize * sizeof( UINT ) );
   UINT uStack = 0;
   UINT uLeft;
   UINT uRight;

#ifdef _DEBUG
   UINT uStackMax = 0;
#endif

   puStack[ uStack++ ] = xRecord1;
   puStack[ uStack++ ] = xRecord2;
   while ( uStack )
   {
      uRight = puStack[ --uStack ];
      uLeft = puStack[ --uStack ];
      m = QuickSortPartition( uLeft, uRight );

#ifdef _DEBUGG
      ShowArray( uLeft, uRight );
#endif

      if ( m + 1 < uRight )
      {
#ifdef _DEBUG
         if ( uStack > uStackSize - 2 )
         {
            ASSERT_TRACE( "QuickSort stack overflow - %d", uStackSize, FALSE );
            m_pPoolVarData->Free( puStack );
            return;
         }
#endif

         puStack[ uStack++ ] = m + 1;
         puStack[ uStack++ ] = uRight;

#ifdef _DEBUG
         if ( uStack > uStackMax )
         {
            uStackMax = uStack;
         }
#endif
      }

      if ( uLeft < m )
      {
#ifdef _DEBUG
         if ( uStack > uStackSize - 2 )
         {
            ASSERT_TRACE( "QuickSort stack overflow - %d", uStackSize, FALSE );
            m_pPoolVarData->Free( puStack );
            return;
         }
#endif

         puStack[ uStack++ ] = uLeft;
         puStack[ uStack++ ] = m;

#ifdef _DEBUG
         if ( uStack > uStackMax )
         {
            uStackMax = uStack;
         }
#endif
      }
   }

   m_pPoolVarData->Free( puStack );

#ifdef _DEBUG
   ASSERT_TRACE( "Max stack utilization = %d\tAllocated stacksize = %d",
                 (uStackMax, uStackSize), FALSE );
#endif
}


/*********************** CSIMP::QuickSortPartition **************************
*
*HelpKey:         Private
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
UINT
CSIMP::QuickSortPartition( RCDX xRecord1,
                           RCDX xRecord2 )
{
   UINT k = xRecord1 - 1;
   UINT j = xRecord2 + 1;
   UINT *p1;
   UINT *p2;
   UINT u;

   while ( TRUE )
   {
      while ( CompareRecords( --j, xRecord1 ) > 0 )
         ;

      while ( CompareRecords( ++k, xRecord1 ) < 0 )
         ;

      if ( k < j )
      {
         p1 = CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx );
         p2 = CSIMP_GETSORTIDX_PTR( j, m_pUseIdx->m_pxIdx );
         u = *p1;
         *p1 = *p2;
         *p2 = u;
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
/////////////////////////////////////////////////////////////////////////////


/********************** CSIMP::UseSortIndex *********************************
*
*HelpKey:         Public
*
*Description:
*  Use a previously defined sort index.
*  If there is no index in use while records are being inserted into the
*  list, the sort indices are left in an invalid state.  Note that position
*  is no longer "established".
*
*
*Arguments:
*  WORD wSortIndex - 1-based index
*  CSortItem::eSORTDIRECTION eSortDir - eASCENDING or eDESCENDING
*  BOOL bForceResort - TRUE  - resort even if previously sorted
*                      FALSE - only resort if not previously sorted
*
*Return Value:
*  pointer to CSortIndex class
*
****************************************************************************/
CSortIndex *
CSIMP::UseSortIndex( WORD wSortIdx,
                     CSortItem::eSORTDIRECTION eSortDir,
                     BOOL bForceResort )
{
   m_eSortDir = eSortDir;  // CSortItem::eASCENDING || eDESCENDING
   if ( m_pUseIdx )
   {
      if ( m_pUseIdx->m_pxIdx == 0 ||
           m_pUseIdx->m_eSortState < CSortIndex::eUNSORTED )
      {
         m_pUseIdx->m_eSortState = CSortIndex::eINDEX_INVALID;
         if ( m_pUseIdx->m_pxIdx )
         {
            FreeSortIndex( m_pUseIdx );
         }
      }
   }

   if ( wSortIdx == 0 )
   {
      m_pUseIdx = 0;
      return( 0 );
   }

   m_pUseIdx = GetSortIndex( wSortIdx );
   ASSERT_MSGBOX_RC( "Undefined search index for UseSortIndex",
                     0, m_pUseIdx, 0 );

#ifdef _DEBUG
   CSortItem *pSortItem = m_pUseIdx->m_pHead->m_pNext;
   while ( pSortItem )
   {
      ASSERT_MSGBOX( "Compare function required for CompareRecords",
                     0, pSortItem->m_cmprfn );
      ASSERT_MSGBOX( "Compare function required for CompareRecords",
                     0, GetDataDescriptorIndex( pSortItem->m_pDD ) );

      if ( GetDataDescriptorIndex( pSortItem->m_pDD ) == 0 ||
           pSortItem->m_cmprfn == 0 )
      {
         m_pUseIdx = 0;
         return( 0 );
      }

      pSortItem = pSortItem->m_pNext;
   }
#endif

   /* if the index is invalid (has not been maintained to correct length)
      reallocate and initialize it */
   if ( m_pUseIdx->m_pxIdx == 0 ||
        m_pUseIdx->m_eSortState == CSortIndex::eINDEX_INVALID )
   {
      if ( m_pUseIdx->m_pxIdx )
      {
         FreeSortIndex( m_pUseIdx );
      }

      /* allocate clean index segments (initialized sequentially) */
      RCDX *px;
      RCDX **pxOldPtr = 0;
      m_pUseIdx->m_pxIdx = 0;
      RCDX xMax = m_lMaxRecords;
      while ( xMax )
      {
         if ( xMax > L_MAX_SORT_BLOCK )
         {
            px = (RCDX *) m_pPoolVarData->Alloc( L_MAX_BYTES );
            xMax -= L_MAX_SORT_BLOCK;
         }
         else
         {
            px = (RCDX *) m_pPoolVarData->Alloc( (LONG) sizeof( RCDX ) * xMax );
            xMax = 0;
         }

         VERIFY( px );
         if ( m_pUseIdx->m_pxIdx == 0 )
         {
            m_pUseIdx->m_pxIdx = px;
         }
         else
         {
            *pxOldPtr = px;
         }

         /* this will be valid if xMax > 0 */
         if ( xMax > 0 )
         {
            pxOldPtr = (RCDX **)((BYTE *) px + L_NEXT_BLOCK_OFFSET);
         }
      }


      px = m_pUseIdx->m_pxIdx;
      for ( RCDX k = 0, j = 0; k < m_dwRecordCnt; k++, j++ )
      {
         if ( j == L_MAX_SORT_BLOCK )
         {
            px = *(RCDX **)((BYTE *) px + L_NEXT_BLOCK_OFFSET);
//          px = (RCDX *)((BYTE *) px + L_NEXT_BLOCK_OFFSET);
            j = 0;
         }

         px[ j ] = k;
      }

      m_pUseIdx->m_eSortState = CSortIndex::eUNSORTED;
   }

   /* even if it is sorted with duplicates, the index can still be used
      to traverse the list in order */
   if ( m_pUseIdx->m_eSortState <= CSortIndex::eUNSORTED || bForceResort )
   {
      ResortSortIndex( wSortIdx );
   }

   return( m_pUseIdx );
}


/********************** CSIMP::SetEnumFunction ******************************
*
*HelpKey:         Public
*
*Description:
*  Set the function to be invoked when a record is enumerated
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
CSIMP::SetEnumFunction( CSIMP_ENUM pfnEnumRecord )
{
   ASSERT_MSGBOX( "Null function specified for SetEnumFunction", 0,
                  pfnEnumRecord );
   m_pfnEnum = pfnEnumRecord;
}

/********************** CSIMP::Enumerate ************************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
CSIMP::Enumerate( VOID *pDataPassToEnum )
{
#ifdef _DEBUG
   if ( m_pfnEnum == 0 )
   {
      ASSERT_MSGBOX( "No function has been specified for Enumerate",
                     0, FALSE );
      return;
   }
#endif

   RCDX k;

   m_pvRecordErr = 0;  // initialize first record in error during enumeration
   if ( m_pUseIdx )
   {
      for ( k = 1; k <= m_lMaxRecords; k++ )
      {
         (*m_pfnEnum)( this,
                       CSIMP_GETSORTREC( k, m_pvRecords, m_pUseIdx->m_pxIdx ),
                       pDataPassToEnum );
      }
   }
   else
   {
      for ( k = 1; k <= m_dwRecordCnt; k++ )
      {
         (*m_pfnEnum)( this, CSIMP_GETRECORD( k, m_pvRecords ),
                       pDataPassToEnum );
      }
   }
}




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/********************** CSortItem::CSortItem ********************************
*
*HelpKey:         Private
*
*Description:
* Constructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortItem::CSortItem( VOID )
{
   m_cmprfn = 0;
   m_sortdir = eASCENDING;
   m_pDD = 0;
   m_pNext = 0;
}

/********************** CSortItem::~CSortItem *******************************
*
*HelpKey:         Private
*
*Description:
*  Destructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortItem::~CSortItem( VOID )
{
   delete( m_pNext );
}


/********************** CSortIndex::CSortIndex ******************************
*
*HelpKey:         Private
*
*Description:
*  Constructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortIndex::CSortIndex( VOID )   // used for "head" pointer only
{
   m_pNext = 0;
   m_pHead = 0;

   /* never used or referenced ... but just for safety's sake */
   m_pCSIMP = 0;
   m_pTail = 0;
   m_pxIdx = 0;
   m_wIndex = 0;
   m_eSortState = eINDEX_INVALID;
}

/********************** CSortIndex::CSortIndex ******************************
*
*HelpKey:         Private
*
*Description:
*   Constructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortIndex::CSortIndex( CSIMP *pCSIMP,
                        WORD wIndex,
                        RCDX xEntries )
{
   ASSERT_MSGBOX( "Sort Index requires CSIMP pointer", 0, pCSIMP );

   /* create memory for and initialize (unsorted) index */
   m_pCSIMP = pCSIMP;
   m_pxIdx = 0;
   m_pNext = 0;
   m_wIndex = wIndex;
   m_pHead = new CSortItem;
   m_pTail = m_pHead;
   m_eSortState = eINDEX_INVALID;
}

/********************** CSortIndex::~CSortIndex *****************************
*
*HelpKey:         Private
*
*Description:
*  Destructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortIndex::~CSortIndex( VOID )
{
   if ( m_pxIdx )
   {
      m_pCSIMP->m_pPoolVarData->Free( m_pxIdx );
   }

   delete( m_pHead );
}


/********************** CSortIndex::Empty ***********************************
*
*HelpKey:         Public
*
*Description:
*  Clear out a sort index so that it can be re-specified.
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID
CSortIndex::Empty( VOID )
{
   delete( m_pHead );
   m_pHead = new CSortItem;
   m_pTail = m_pHead;
   m_eSortState = eINDEX_INVALID;
}

/********************** CSortIndex::operator << *****************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortIndex&
CSortIndex::operator << ( FMDBM_FLDID FldId )
{
   CDataDescriptor *pDD = m_pCSIMP->GetDataDescriptor( FldId );
   ASSERT_MSGBOX( "Invalid field id for sort index", 0, pDD );

   m_pTail->m_pNext = new CSortItem;
   m_pTail = m_pTail->m_pNext;
   m_pTail->m_pDD = pDD;
   return( *this );
}

/********************** CSortIndex::operator << *****************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortIndex&
CSortIndex::operator << ( CSIMP_CMPR cmprfn )
{
   ASSERT_MSGBOX( "Invalid compare function for sort index", 0, cmprfn );
   ASSERT_MSGBOX( "Compare function specified for Null sort index", 0,
                  m_pHead != m_pTail );

   m_pTail->m_cmprfn = cmprfn;
   return( *this );
}

/********************** CSortIndex::operator << *****************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSortIndex&
CSortIndex::operator << ( CSortItem::eSORTDIRECTION sortdir )
{
   ASSERT_MSGBOX( "Sort direction specified for Null sort index", 0,
                  m_pHead != m_pTail );
   m_pTail->m_sortdir = sortdir;
   return( *this );
}


/********************** CDataDescriptor::CDataDescriptor ********************
*
*HelpKey:         Public
*
*Description:
*  Constructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CDataDescriptor::CDataDescriptor( VOID )
{
   /* local data will be set by CSIMP */
}


/********************** CDataDescriptor::CDataDescriptor ********************
*
*HelpKey:         Public
*
*Description:
*  Copy constructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
/* CDataDescriptor - copy constructor */
CDataDescriptor::CDataDescriptor( const CDataDescriptor& whereSrcDD )
{
   m_FldId = whereSrcDD.m_FldId;
   m_DataType = whereSrcDD.m_DataType;
   m_wRawLth = whereSrcDD.m_wRawLth;
   m_wOffset = whereSrcDD.m_wOffset;
}


/********************** CDataDescriptor::CDataDescriptor ********************
*
*HelpKey:         Public
*
*Description:
*  Destructor
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CDataDescriptor::~CDataDescriptor( VOID )
{
}


/********************** CDataDescriptor::GetOffset **************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
WORD
CDataDescriptor::GetOffset( VOID )
{
   return( m_wOffset );
}

/********************** CDataDescriptor::GetRawLth **************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
WORD
CDataDescriptor::GetRawLth( VOID )
{
   return( m_wRawLth );
}

/********************** CDataDescriptor::IsVarSize **************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
BOOL
CDataDescriptor::IsVarSize( VOID )
{
   return( (m_DataType & CSIMP_ADDRESS) ? TRUE : FALSE );
}

/********************** CDataDescriptor::GetDataLength **********************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
WORD
CDataDescriptor::GetDataLength( const VOID *pvRecord )
{
   WORD wDataLth;

// if ( IsVarSize( ) )
   if ( m_DataType & CSIMP_ADDRESS ) // just for performance
   {
      VOID *pv = (VOID *) ((BYTE *) pvRecord + GetOffset( ) );
      pv = *((VOID **) pv);            // dereference pointer to pointer
      if ( pv == 0 )
      {
         wDataLth = 1;
      }
      else
      {
         wDataLth = *((WORD *) pv);  // get length from leading bytes
      }
   }
   else
   {
      wDataLth = m_wRawLth;
   }

   return( wDataLth );
}

/********************** CDataDescriptor::GetDataAddress *********************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
VOID *
CDataDescriptor::GetDataAddress( const VOID *pvRecord )
{
   VOID *pvData = (VOID *) ((BYTE *) pvRecord + GetOffset( ) );
// if ( IsVarSize( ) )
   if ( m_DataType & CSIMP_ADDRESS ) // just for performance
   {
      if ( pvData == 0 )
      {
         pvData = &pxNul;
      }
      else
      {
         pvData = *((VOID **) pvData);    // dereference pointer to pointer

         /* skip length bytes */
         pvData = (VOID *) ((BYTE *) pvData + sizeof( WORD ));
      }
   }

   return( pvData );    // now pointing at data item in record
}

/********************** CDataDescriptor::GetDataType ************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSIMP_DATATYPE
CDataDescriptor::GetDataType( VOID )
{
   /* strip out high order 4 bits (reserved) */
   return( m_DataType & ~CSIMP_HIGHBITS );
}

/********************** CDataDescriptor::GetHighbits ************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
CSIMP_DATATYPE
CDataDescriptor::GetHighbits( VOID )
{
   /* return complete datatype (including reserved bits) */
   return( m_DataType );
}

/********************** CDataDescriptor::IsUserType *************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
BOOL
CDataDescriptor::IsUserType( VOID )
{
   return( FMDBC_IS_USERTYPE( m_FldId ) );
}


/********************** CDataDescriptor::IsUnderflow ************************
*
*HelpKey:         Public
*
*Description:
*
*Arguments:
*
*Return Value:
*
****************************************************************************/
BOOL
CDataDescriptor::IsUnderflow( VOID )
{
   return( (m_DataType & CSIMP_UNDERFLOW) ? TRUE : FALSE );
}


/****************************  Revision History *****************************
* $Log:   S:\csdg\prj\firm\src\fmdbc\fmdbcsmp.cpv  $

      Rev 3.4.2.2   09 Aug 1995 14:59:34   DougS
   Source documentation effort

      Rev 3.4.2.1   07 Aug 1995 13:53:46   DougS
   Documentation pass

      Rev 3.4.2.0   02 Aug 1995 11:22:14   DougS
   Beginning of PROD3.4_DEV branch

      Rev 3.4   01 Aug 1995 11:14:46   JeffD
   Release_3.4

      Rev 3.3.2.22   14 Jun 1995 10:12:16   DougS
   No change.

      Rev 3.3.2.21   06 Jun 1995 16:14:40   DougS

      Rev 3.3.2.20   31 May 1995 10:24:06   BUILD
   Non-debug version of PrivateDisplayTime eliminated

      Rev 3.3.2.19   30 May 1995 15:43:38   DougS
   Changed MESSAGE_ASSERT to ASSERT_MSGBOX

      Rev 3.3.2.18   24 May 1995 16:44:28   DougS

      Rev 3.3.2.17   23 May 1995 15:51:06   DougS
   Eliminated overflow message from dblib

      Rev 3.3.2.16   22 May 1995 19:42:44   DougS
   Fix to eliminate column truncation warning by Sybase

      Rev 3.3.2.15   22 May 1995 17:12:48   DougS
   Attempted to remove warning from Sybase when the receiving field is too
   short to handle the full length of data for datetime (and smalldatetime)
   columns

      Rev 3.3.2.14   20 May 1995 12:52:56   JeffD

      Rev 3.3.2.13   17 May 1995 19:59:40   DougS

      Rev 3.3.2.12   16 May 1995 16:03:10   DougS

      Rev 3.3.2.11   12 May 1995 14:12:54   DougS
   Fixed sort index reset for Empty case

      Rev 3.3.2.10   10 May 1995 16:30:20   DougS
   Fixed length of CSIMP_CHAR

      Rev 3.3.2.9   08 May 1995 17:32:46   DougS
   All code removed from dbc public header

      Rev 3.3.2.8   08 May 1995 16:13:02   DougS
   Fixed shared views to completely mirror structures (including flags) and
   to retain data from first query

      Rev 3.3.2.7   05 May 1995 14:07:04   DougS
   Fixed Restructure record and SharedViews

      Rev 3.3.2.6   04 May 1995 20:19:52   DougS
   Changed USERTYPE TO FMDBC_USERTYPE.  Added Bind field count and corrected
   StructureMatch test in AddRecord

      Rev 3.3.2.5   04 May 1995 08:01:52   DougS
   Comments

      Rev 3.3.2.4   03 May 1995 22:16:34   DougS
   Fixed smalldatetime interface and shared views

      Rev 3.3.2.3   01 May 1995 17:39:18   DougS
   Corrected sorting indexing

      Rev 3.3.2.2   01 May 1995 12:03:32   DougS
   Fix to sorting caused by prior fix

      Rev 3.3.2.1   01 May 1995 09:45:50   DougS
   Added timestamp conversion routines to support optimistic concurrency.
   Added SyncViews to permit structure matched views to share data.

      Rev 1.0.2.7   26 Apr 1995 17:10:52   DougS
   Cosmetic

      Rev 1.0.2.6   25 Apr 1995 10:41:36   DougS
   cosmetic

      Rev 3.2.2.12  24 Apr 1995 18:42:22   DougS
   Added support for optimistic concurrency, database aggregrate functions,
   database query expressions, datetime conversions.

      Rev 3.2.2.11  06 Apr 1995 13:25:20   DougS
   GENERAL: Made Private function available as a stub function for Non-Debug

      Rev 3.2.2.10  28 Mar 1995 15:41:52   DougS
   Additional methods for to support hierarchical processing and Position
   notification

      Rev 3.2.2.9   16 Mar 1995 08:12:16   BUILD
   Changed name of DisplayTime to PrivateDisplaySortTime

      Rev 3.2.2.8   14 Mar 1995 17:04:28   DougS
   Implemented initial pass at support for hierarchical views

      Rev 3.2.2.7   08 Feb 1995 12:52:06   DougS
   added comment for return value from GetAddress when the address is zero.

      Rev 3.2.2.6   08 Feb 1995 12:04:42   DougS
   A bit more clean-up (memory leak) in the accept/delete version methods.

      Rev 3.2.2.5   07 Feb 1995 16:43:26   DougS
   Added versioning methods to CSIMP class.

      Rev 3.2.2.3   30 Jan 1995 10:59:20   DougS
   Seek record correction

      Rev 3.2.2.2   19 Jan 1995 13:30:42   DougS
   Added standard sort comparison routines

      Rev 3.2.2.1   17 Jan 1995 08:48:34   JeffD
   updated with HelpKeys

      Rev 1.0.2.0   05 Jan 1995 15:08:08   DougS
   Beginning of PROD3.1_DEV branch

      Rev 1.0   05 Jan 1995 15:04:40   DougS
   Initial revision.
*
****************************************************************************/

/////////////////////////////// end of file /////////////////////////////////
