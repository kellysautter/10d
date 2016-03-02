//
// MODULE NAME:  KZOEMMAA  -  Object services memory management
// DESCRIPTION:  This is the source file which contains Object services
//               dataspace memory management operations
// OPERATIONS:   All OPERATIONS allocating, initializing and freeing
//               Dataspace memory
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    3/31/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2001.08.22  DGC
   Started adding stuff for local memory version of Core.

2000.12.08  US
   Clear and complete synchronizing for all tasks for all functions
   of the memory management.
   At error conditions some system traces added

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

1999.11.16  US   10a
   Went trough the whole fnAlloc and fnFree stuff and changed it due to
   a reasonable performance hole when freeing memory
   Instead of walking two times through the freespace list each time a
   free is called a kind of 'garbage collection' is implemented.
   The memory is marked as free without being chained into the freespace
   list. Instead each time when the amount of so 'lost' memory is more than
   Available memory / GARBAGE_COLLECT_FACTOR a new function is called
   that goes through all memory spaces in one memory area and join and chain
   all free block one time
   the fnAlloc function is rewritten completely as the old one was awful
   complex and not very logical through all improvements that were made over
   the years.
   Additionally the variable names in the header were changed as their use
   didn't match their names.

24.04.1998 HH
   Emergency fix ### in fnFreeDataspace.
*/

//#define DKS_DBG

#include "kzoephdr.h"


zBOOL g_bTraceDataspace;

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

// GARBAGE_COLLECT_FACTOR defines a fraction of the size of an memory Block
// the fnChainFreespaces( ) function is called when the amount of memory
// that is freed, but not rechained into the freespace list exceeds
// 1/GARBAGE_COLLECT_FACTOR of the size of the memory block
#define GARBAGE_COLLECT_FACTOR 20
#define MIN_MEM_BLOCKSIZE      16  // Minimum allocation size

//./ ADD NAME=fnStoreStringInDataspace
// Source Module=kzoemmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnStoreStringInDataspace
//
//  PURPOSE:    To take a null terminated string, store it in
//              the dataspace whose header is passed and return
//              the address of the stored string.
//
//  PARAMETERS: lpDataHeader - The dataheader into which to store the
//                             string. lpTask->hFirstDataHeader or
//                             AnchorBlock.
//              szName       - The name to be stored
//
//  RETURNS:    zCPCHAR  - The address of the stored name
//                         RETURNED AS A HANDLE!
//              0        - The name is null
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zPCHAR
fnStoreStringInDataspace( LPDATAHEADER lpDataHeader, zCPCHAR cpcName )
{
   zPCHAR lpReturnName;
   zPVOID hDataspace = 0;

   if ( cpcName && cpcName[ 0 ] )
   {
      zULONG ulLth = zstrlen( cpcName ) + 1;
      hDataspace = fnAllocDataspace( lpDataHeader, ulLth, 0, 0, iString );
      if ( hDataspace )
      {
         lpReturnName = zGETPTR( hDataspace );
         strcpy_s( lpReturnName, ulLth, cpcName );
      }
   }

   return( (zPCHAR) hDataspace );
}

// This function walks through all data spaces of a shared memory block.
// It searches for all freespace blocks, joins successive freespaces
// and rechain them all into the freespace chain.
// This function must not be called outside code that is synchronized
// by the memory semaphore.
//
//./ ADD NAME=fnChainFreespaces
// Source Module=kzoemmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnChainFreespaces
//
//  PURPOSE:    This function walks through all data spaces of a shared
//              memory block. It searches for all freespace blocks,
//              joins successive freespaces and rechains them all into
//              the freespace chain.
//              This function must not be called outside code that is
//              synchronized by the memory semaphore.
//
//  PARAMETERS: pDataHeader - The dataheader of the memory block that
//                            has to be re-chained
//
//  RETURNS:    zPVOID   - 0 if successful, the address of an invalid
//                         memory space if an error occurred
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zPVOID
fnChainFreespaces( LPDATAHEADER lpDataHeader)
{
   LPFREESPACE  lpFreespace; // last element of freespace chain
   LPFREESPACE  lpSpace;     // actual space
   LPFREESPACE  lpNextSpace; // next space
   zPVOID       pvEndOfBuffer;
   zULONG       ulCntFreeSpaces = 0;
   zULONG       ulFree = 0;
   zULONG       ulUsed = 0;

   zLOCK_MUTEX( zMUTEX_MEMORY );

   lpSpace = (LPFREESPACE)((zPCHAR) lpDataHeader + sizeof( DataHeaderRecord ));
   pvEndOfBuffer = (zPCHAR) lpDataHeader + sizeof( DataHeaderRecord ) + lpDataHeader->ulUseableSize;
   lpFreespace = &lpDataHeader->Freespace;
   while ( (void *) lpSpace < pvEndOfBuffer )
   {
      lpNextSpace = (LPFREESPACE) ((zPCHAR) lpSpace + lpSpace->ulFreeLth);
      switch ( lpSpace->nTableID )
      {
         case iFreespace:
         {
            if ( ((void *) lpNextSpace < pvEndOfBuffer) &&
                 (lpNextSpace->nTableID == iFreespace) )
            {
               // Next memory space is also a freespace, so join both.
               lpSpace->ulFreeLth += lpNextSpace->ulFreeLth;
            }
            else
            {
               // Next space is used so chain the space into the list.
               if ( lpSpace->ulFreeLth > lpDataHeader->ulMaxEmptyBlockSize )
                  lpDataHeader->ulMaxEmptyBlockSize = lpSpace->ulFreeLth;

               ulCntFreeSpaces++;

               // Chain the space into the freespace list.
               lpSpace->hNextFreespace = 0;
               ulFree += lpSpace->ulFreeLth;
               lpFreespace->hNextFreespace = lpSpace->hFreespace;
               lpFreespace = lpSpace;
               lpSpace = lpNextSpace;
            }

            break;
         }

         case iUsedFreespace:
         case iUsedFreespace1:
         {
            ulUsed += lpSpace->ulFreeLth;
            lpSpace = lpNextSpace;
            break;
         }

         default:
         {
         // SysMessageBox( "Zeidon Error", "Memory Management: "
         //                "Wrong Dataspace Table ID", 0 );
            TraceLineI( "Memory Management Error! Wrong Dataspace Table ID: ",
                        lpSpace->nTableID );
            return( lpSpace );
         }
      }
   }

   if ( (ulUsed + ulFree) != lpDataHeader->ulUseableSize )
   {
      TraceDataHeader( lpDataHeader, TRUE );
      SysMessageBox( 0, "Zeidon Error", "Memory Management: "
                     "Inconsistent size of memory blocks", 0 );
      zUNLOCK_MUTEX( zMUTEX_MEMORY );
      return( lpDataHeader );
   }

   lpDataHeader->ulCntFreeSpaces = ulCntFreeSpaces;

   // Now all freed memory is chained into the list.
   lpDataHeader->ulFreedMemSize = 0;

   zUNLOCK_MUTEX( zMUTEX_MEMORY );
   return( 0 );
}

// Go through one or all dataheaders and trace them to the OI trace window.
void OPERATION
TraceDataHeader( LPDATAHEADER lpDataHeader, zBOOL bAll )
{
   LPFREESPACE  lpSpace;     // actual space
   zPVOID       pvEndOfBuffer;
   zULONG       ulCntFreeSpaces = 0;
   zULONG       ulFree = 0;
   zULONG       ulUsed = 0;

   if ( g_bServerMode )
   {
      TraceLineS( "Running in Server Mode and using local memory", "" );
      return;
   }

   if ( bAll )  // if bAll is set, position to the very first DataHeader
   {
      while ( lpDataHeader->hPrevDataHeader )
         lpDataHeader = zGETPTR( lpDataHeader->hPrevDataHeader );
   }

   // Trace out specified DataHeader(s).
   while ( lpDataHeader )
   {
      fnChainFreespaces( lpDataHeader );  // recover freespace

      TraceLineI( "Data Header Number:    ",
                  (unsigned long) lpDataHeader->hDataHandle >> 24 );
      TraceLineI( "  Total Size         = ", lpDataHeader->ulUseableSize );
      TraceLineI( "  Used Size          = ", lpDataHeader->ulUsedSpace );
      TraceLineI( "  Max free Blocksize = ", lpDataHeader->ulMaxEmptyBlockSize );
      TraceLineI( "  Used Blocks        = ", lpDataHeader->ulCntUsedEntries );
      TraceLineI( "  Free Blocks        = ", lpDataHeader->ulCntFreeSpaces );

      // Trace out the freespace here.
      lpSpace = (LPFREESPACE) ((zPCHAR) lpDataHeader + sizeof( DataHeaderRecord ));
      pvEndOfBuffer = (zPCHAR) lpDataHeader + sizeof( DataHeaderRecord ) +
                                                 lpDataHeader->ulUseableSize;
      while ( (void *) lpSpace < pvEndOfBuffer )
      {
         switch ( lpSpace->nTableID )
         {
            case iFreespace:
            {
            // TraceLine( "Freespace (%d) 0x%08x  =================  Length: %d   Next: 0x%08x",
            //            lpSpace->ulDebugID, lpSpace, lpSpace->ulFreeLth, ((zULONG) lpSpace) + lpSpace->ulFreeLth );
               break;
            }

            case iUsedFreespace:
            case iUsedFreespace1:
            {
            // zCHAR  szMessage[ 256 ];

            // sprintf_s( szMessage, "Used Freespace (%d) 0x%08x  Length: %d ***   Next: 0x%08x",
            //           lpSpace->ulDebugID, lpSpace, lpSpace->ulFreeLth, ((zULONG) lpSpace) + lpSpace->ulFreeLth );
            // TraceBuffer( szMessage, lpSpace, lpSpace->ulFreeLth );
               break;
            }

            default:
            {
            // zCHAR  szMessage[ 256 ];

            // SysMessageBox( "Zeidon Error", "Memory Management: "
            //                "Wrong Dataspace Table ID", 0 );
            // TraceLine( "Memory Management Error! Wrong Dataspace (%d) Table ID: %d",
            //             lpSpace->ulDebugID, lpSpace->nTableID );
               break;
            }
         }

         if ( lpSpace->ulFreeLth == 0 )
         {
            SysMessageBox( 0, "TraceDataHeader", "Zero FreeLth", -1 );
            break;
         }

         lpSpace = (LPFREESPACE) ((zPCHAR) lpSpace + lpSpace->ulFreeLth);
      }

      if ( bAll == FALSE )
         break;

      lpDataHeader = zGETPTR( lpDataHeader->hNextDataHeader );
   }
}

#ifdef ALLOC_FREE_TRACE
zLONG PrimeTable[ 8 ] = { 1009, 5009, 10009, 20011, 50021, 100003, 200003, 500009 };
zULONG SizeTable[ 8 ] = { 0 };
zPVOID AddressTable[ 8 ] = { 0 };
static zLONG lBomb = 0;

zLONG
fnHashPointerAdd( LPANCHOR lpAnchorBlock, zPVOID pv, zLONG lPrime )
{
   zLONG lHash = (zLONG) pv;
   if ( lHash < 0 )
      lHash *= -1;

   lHash += lHash << (sizeof( zLONG ) / 2);
   lHash %= lPrime;
   if ( lpAnchorBlock->pvMemId[ lHash ] )
   {
      if ( lpAnchorBlock->lMemMax == 0 )
         lpAnchorBlock->lMemMax = lPrime;
      else
      if ( lpAnchorBlock->lMemMax < ALLOC_FREE_TRACE )
         (lpAnchorBlock->lMemMax)++;
      else
         return( -1 );

      lHash = lpAnchorBlock->lMemMax;
   }

   return( lHash );
}

zLONG
fnHashPointerRemove( LPANCHOR lpAnchorBlock, zPVOID pv, zLONG lPrime )
{
   zLONG lHash = (zLONG) pv;
   if ( lHash < 0 )
      lHash *= -1;

   lHash += lHash << (sizeof( zLONG ) / 2);
   lHash %= lPrime;

   if ( lpAnchorBlock->pvMemId[ lHash ] != pv )
   {
      lHash = lPrime;
      while ( lpAnchorBlock->pvMemId[ lHash ] )
      {
         if ( lpAnchorBlock->pvMemId[ lHash ] == pv )
            break;

         if ( lHash < lpAnchorBlock->lMemMax )
            lHash++;
         else
            return( -1 );
      }
   }

   return( lHash );
}

void OPERATION
fnListDataspaceLeak( LPANCHOR lpAnchorBlock, zPVOID hTask, zSHORT nFlag )
{
   zULONG ulLeakSize = 0;
   zBOOL  bTraceAll;
   zBOOL  bTrace;
   zLONG k;

   bTraceAll = (nFlag > 0) ? TRUE : FALSE;

   zLOCK_MUTEX( zMUTEX_MEMORY );
   lpAnchorBlock->bDebugLeak = TRUE;

   if ( lpAnchorBlock->lMemMax == 0 )
      lpAnchorBlock->lMemMax = PrimeTable[ 4 ];

   if ( nFlag == -1 )
   {
      for ( k = 0; k < ALLOC_FREE_TRACE; k++ )
         lpAnchorBlock->pvMemId[ k ] = 0;
   }
   else
   for ( k = 0; k < ALLOC_FREE_TRACE; k++ )
   {
      if ( lpAnchorBlock->pvMemId[ k ] )
      {
         if ( lpAnchorBlock->lMemSize[ k ] < 0 ) // already reported as leak
         {
            lpAnchorBlock->lMemSize[ k ] *= -1;
            bTrace = bTraceAll;
         }
         else
            bTrace = TRUE;

         if ( bTrace )
         {
            TraceLine( "(mm) ListDataspaceLeak: 0x%08x for task: 0x%08x leak %d bytes at Id: %d",
                       lpAnchorBlock->pvMemId[ k ], lpAnchorBlock->pvMemTask[ k ],
                       lpAnchorBlock->lMemSize[ k ], k );
         }

         if ( nFlag == 2 &&
              (hTask == 0 || lpAnchorBlock->pvMemTask[ k ] == hTask) )
         {
            zPVOID pvData = zGETPTR( lpAnchorBlock->pvMemId[ k ] );
            if ( pvData )
               TraceBuffer( "  Data: ", pvData, lpAnchorBlock->lMemSize[ k ] );
         }

         ulLeakSize += lpAnchorBlock->lMemSize[ k ];
         lpAnchorBlock->lMemSize[ k ] *= -1;  // mark as reported leak
      }
   }

   lpAnchorBlock->bDebugLeak = FALSE;
   zUNLOCK_MUTEX( zMUTEX_MEMORY );
   TraceLineI( "(mm) ListDataspaceLeak Total: ", ulLeakSize );
}
#else
void OPERATION
fnListDataspaceLeak( LPANCHOR lpAnchorBlock, zPVOID hTask, zSHORT nFlag )
{
}
#endif

//./ ADD NAME=fnAllocDataspace
// Source Module=kzoemmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnAllocDataspace
//
//  PURPOSE:    To allocate some space in a dataspace.
//
//  PARAMETERS: hDataHeader - The dataheader into which to allocate the
//                            space. This may be the lpFirstDataHeader for
//                            a task or the AnchorBlock.
//              ulReqSize   - The number of bytes to allocate
//              nInitialize - Initialize the space to the cInitByte value
//              cInitByte   - The initialization value (usually 0)
//
//  RETURNS:    zPVOID      - The address of the space allocated
//              0           - Couldn't allocate any space
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zPVOID
fnAllocDataspace( LPDATAHEADER hFirstDataHeader,
                  zULONG       ulReqSize,
                  zSHORT       nInitialize,
                  zCHAR        chInitByte,
                  zSHORT       nTableID )
{
   LPDATAHEADER  lpFirstDataHeader;
   LPDATAHEADER  lpDataHeader;
   LPFREESPACE   lpPrevFreespace;
   LPFREESPACE   lpNextFreespace;
   LPFREESPACE   lpNewDataspace;
   LPTASK        lpCurrentTask;
   zPCHAR        lpReturnPtr = 0;
   zPVOID        hReturnPtr;
   zPVOID        hDataHeader;
   zULONG        ulSize;
   zULONG        ulAllocBytes;
   zBOOL         bMutexLocked;
   zSHORT        nAllocCnt;

#if 0 // debugging only
   LPTASK lpMainTask = zGETPTR( AnchorBlock->hMainTask );
   if ( (lpMainTask && hFirstDataHeader == lpMainTask->hFirstDataHeader) ||
        hFirstDataHeader == AnchorBlock->hMainFirstDataHeader ||
        hFirstDataHeader == (LPDATAHEADER) g_hAnchorBlock )
   {
      ulSize = 1;
   }
   else
      ulSize = 0;
#endif

   // Minimum allocation unit is MIN_MEM_BLOCKSIZE.
   if ( ulReqSize < MIN_MEM_BLOCKSIZE )
      ulReqSize = MIN_MEM_BLOCKSIZE;

   // Ensure that the requested space is on the required byte boundary.
   // So we assure that a minimum of at least 4 Bytes is allocated.
   if ( ulReqSize % zALIGN_MEM_SIZE )
      ulReqSize += zALIGN_MEM_SIZE - (ulReqSize % zALIGN_MEM_SIZE);

#if 0
   // If we're running in ServerMode, then we can allocate local memory.
   if ( g_bServerMode )
   {
      lpReturnPtr = SysMalloc( ulReqSize );
      if ( lpReturnPtr )
      {
         if ( nInitialize && chInitByte )
            zmemset( lpReturnPtr, chInitByte, (unsigned int) ulReqSize );

         if ( nTableID && nTableID < iAttributeRecord )
            memcpy( lpReturnPtr, &nTableID, zsizeof( nTableID ) );
      }

      return( lpReturnPtr );
   }
#endif

   // A quick hack to test things.  We're chaining everything off of the main
   // system task.  dks 2006.05.07  change from 10B
// if ( hFirstDataHeader != (LPDATAHEADER) g_hAnchorBlock )
//    hFirstDataHeader = AnchorBlock->hMainFirstDataHeader;

   hDataHeader = hFirstDataHeader;
   lpDataHeader = zGETPTR( hFirstDataHeader );

   // If this is not a valid alloc request, return "no good".
   if ( lpDataHeader == 0 || ulReqSize <= 0 )
   {
      TraceLineI( "(mm) Invalid fnAllocDataspace request ", ulReqSize );
      return( 0 );
   }

   lpFirstDataHeader = lpDataHeader;
   lpCurrentTask = zGETPTR( lpFirstDataHeader->hTask );

   // Add the size of the prefix to the size of the new chunk returned.
   ulSize = ulReqSize + sizeof( FreespaceRecord );
   if ( lpCurrentTask )
      lpCurrentTask->ulTrackTaskMemory += ulSize;

   // Ensure that the total space is on the required byte boundary.
   // The next two lines are commented out, because the FreespaceStruct
   // has to be on the boundary anyway.
// if ( ulSize % zALIGN_MEM_SIZE )
//    ulSize += zALIGN_MEM_SIZE - (ulSize % zALIGN_MEM_SIZE);

// if ( ulSize == 62 || ulSize == 66 )
//    TraceLineI( "AllocDataspace size: ", ulSize );

   // Single thread memory allocation/deallocation for memory allocated out of the
   // AnchorBlock memory segment or for memory allocated for the system task.
   if ( lpFirstDataHeader->hTask == AnchorBlock->hMainTask )
   {
      bMutexLocked = TRUE;
      zLOCK_MUTEX( zMUTEX_MEMORY );
   }
   else
      bMutexLocked = FALSE;

   // Logic completely changed because it was totally confusing (us).
   nAllocCnt = 1;
   do  // search in each shared data block
   {
      if ( lpDataHeader->ulCntFreeSpaces > 0 &&
           lpDataHeader->ulMaxEmptyBlockSize > ulSize )
      {
         // It looks like there is enough space in this memory block.
         lpPrevFreespace = &lpDataHeader->Freespace;
         lpNextFreespace = zGETPTR( lpPrevFreespace->hNextFreespace );
         if ( lpNextFreespace == 0 )
         {
            // Must not occur, because then ulCntFreeSpaces should be 0.
         // SysMessageBox( "Zeidon Error", "Memory Management: "
         //                "Inconsistent Freespace Count", 0 );
            TraceLineS( "Memory Management Error: ",
                        "Inconsistent Freespace Count" );

            TraceDataHeader( lpDataHeader, FALSE ); // trace this header
         // fnChainFreespaces( lpDataHeader );
         // TraceDataHeader( lpDataHeader, FALSE ); // trace this header again after chaining
            continue;  // try it again, now the freespace count should be consistent again

         // if ( bMutexLocked )
         //    zUNLOCK_MUTEX( zMUTEX_MEMORY );

         // return( 0 );
         }

         while ( lpNextFreespace )
         {
            // If a big enough freespace area is found, use it.
            if ( lpNextFreespace->ulFreeLth >= ulSize )
            {
               // If there isn't enough room at the end of the found space to
               // create a new freespace entry, return the whole available chunk.
               if ( lpNextFreespace->ulFreeLth <
                    (ulSize + sizeof( FreespaceRecord ) + MIN_MEM_BLOCKSIZE) )
               {
                  ulSize = lpNextFreespace->ulFreeLth;

                  // Chain last pointer ahead to next pointer.
                  lpPrevFreespace->hNextFreespace = lpNextFreespace->hNextFreespace;
                  lpDataHeader->ulCntFreeSpaces--;
               }
               else
               {
                  // Create a new Freespace chunk after the space being allocated and
                  // point the previous dataspace chunk to the newly created chunk.
                  lpNewDataspace = (LPFREESPACE) ((zPCHAR) lpNextFreespace + ulSize);
                  lpNewDataspace->hNextFreespace = lpNextFreespace->hNextFreespace;
                  lpNewDataspace->nTableID = lpNextFreespace->nTableID;
                  lpNewDataspace->nPrevTableID = 0;
#ifdef ALLOC_FREE_TRACE
                  lpNewDataspace->ulDebugID = ++(AnchorBlock->ulAllocCnt);  // pre-increment intended
#endif
                  lpNewDataspace->ulFreeLth = lpNextFreespace->ulFreeLth - ulSize;
                  lpNewDataspace->hFreespace = fnSysCreateHandle( lpDataHeader, lpNewDataspace );
                  lpPrevFreespace->hNextFreespace = lpNewDataspace->hFreespace;
               // TraceLine( "fnAllocFreespace allocated (%d) Freespace: 0x%08x",
               //            lpNewDataspace->ulDebugID, lpNewDataspace );
               }

               lpReturnPtr = (zPCHAR) lpNextFreespace;
               lpReturnPtr += sizeof( FreespaceRecord );
               hReturnPtr = fnSysCreateHandle( lpDataHeader, lpReturnPtr );

               // Point to the FreespaceStruct in front of the memory.
               lpNewDataspace = (LPFREESPACE) lpReturnPtr - 1;

               // Now initialize the FreespaceStruct in front of the memory.

               // Set the "next freespace pointer" in this used memory chunk
               // to point back to the data header.
               lpNextFreespace->hNextFreespace = (LPFREESPACE) hDataHeader;

               // For some reason ??? hFreespace of a used memory space must
               // point to the beginning of the user memory area.
               lpNextFreespace->hFreespace   = hReturnPtr;
               lpNextFreespace->nTableID     = iUsedFreespace;
               lpNextFreespace->nUsedTableID = nTableID;
               lpNextFreespace->ulFreeLth    = ulSize;

               lpDataHeader->ulCntUsedEntries++;
               lpDataHeader->ulUsedSpace += ulSize;

               // Memory found so return the handle of the found memory space.
               if ( lpReturnPtr )
               {
                  // Turn lpReturnPtr into a handle.
                  if ( nInitialize )
                     zmemset( lpReturnPtr, chInitByte, ulReqSize );

                  if ( nTableID && nTableID < iAttributeRecord )
                     memcpy( lpReturnPtr, &nTableID, sizeof( nTableID ) );
               }

#ifdef ALLOC_FREE_TRACE
               if ( AnchorBlock && nTableID != iAnchor )
               {
                  LPANCHOR lpAnchorBlock = AnchorBlock; // zGETPTR( g_hAnchorBlock );
                  if ( lpAnchorBlock->bTraceMem )
                  {
                     zSHORT k;
                     zLONG lHash = fnHashPointerAdd( lpAnchorBlock, (zPVOID) hReturnPtr, PrimeTable[ 4 ] );
                     if ( lHash >= 0 )
                     {
                        lpAnchorBlock->pvMemId[ lHash ] = (zPVOID) hReturnPtr;
                        lpAnchorBlock->lMemSize[ lHash ] = (zLONG) ulReqSize;
                        lpAnchorBlock->pvMemTask[ lHash ] = zGETHNDL( lpCurrentTask );
#if 1
                        lBomb /= lBomb;
                        lBomb++;
                        for ( k = 0; SizeTable[ k ]; k++ )
                        {
                           if ( ulReqSize == SizeTable[ k ] && (AddressTable[ k ] == 0 || hReturnPtr == AddressTable[ k ]) )
                           {
                              TraceLine( "(mm) AllocDataspace: 0x%08x for task: 0x%08x allocated %d bytes at Id: %d",
                                         lpAnchorBlock->pvMemId[ lHash ], lpAnchorBlock->pvMemTask[ lHash ],
                                         lpAnchorBlock->lMemSize[ lHash ], lHash );
                           }
                        }
#endif
                     }
                     else
                        TraceLine( "(mm) AllocDataspace cannot Add: 0x%08x", hReturnPtr );
                  }
               }
#endif

               if ( bMutexLocked )
                  zUNLOCK_MUTEX( zMUTEX_MEMORY );

            // if ( lpNextFreespace->nUsedTableID == 10100 )
            //    TraceLine( "AllocDataspace: (ID: %d %d) 0x%08x for Task: 0x%08x",
            //               lpNextFreespace->nTableID, lpNextFreespace->nUsedTableID, lpNextFreespace, lpCurrentTask );

               return( hReturnPtr );
            }

            // Move on to next free chunk of space.
            lpPrevFreespace = lpNextFreespace;
            lpNextFreespace = zGETPTR( lpNextFreespace->hNextFreespace );

         }  // while ( lpNextFreespace )
      }

      // In this block there was no freespace that was big enough, so set the
      // lMaxEmptyBlockSize at least 1 smaller than the desired size and try
      // the next block.
      if ( lpDataHeader->ulMaxEmptyBlockSize >= ulSize )
         lpDataHeader->ulMaxEmptyBlockSize = ulSize - 1;

      // Go to the next block, or if it was the last one create a new block.
      if ( lpDataHeader->hNextDataHeader )
      {
         nAllocCnt++;
         lpDataHeader = zGETPTR( lpDataHeader->hNextDataHeader );
      }
      else
      {
         // We need a new shared memory block so create one and chain it in.
         LPDATAHEADER hNewDataHeader;
         LPDATAHEADER lpNewDataHeader;
         void         *pTmp;

      // if ( AnchorBlock->nMemoryCeiling ||
      //      AnchorBlock->nMemoryTraceThreshold )
      // {
      //    TraceLine( "(mm) fnAllocDataspace (%d) allocation for task: 0x%08x ======> Threshold: %d   Ceiling: %d",
      //               nAllocCnt, lpFirstDataHeader->hTask,
      //               AnchorBlock->nMemoryTraceThreshold, AnchorBlock->nMemoryCeiling );
      // }

         if ( AnchorBlock->nMemoryTraceThreshold &&
              nAllocCnt > AnchorBlock->nMemoryTraceThreshold )
         {
            TraceLine( "(mm) fnAllocDataspace (%d) allocation over threshold for task: 0x%08x ======> %d",
                       nAllocCnt, AnchorBlock->nMemoryTraceThreshold );
         }

         if ( AnchorBlock->nMemoryCeiling &&
              nAllocCnt > AnchorBlock->nMemoryCeiling )
         {
            TraceLine( "(mm) fnAllocDataspace (%d) allocation over ceiling for task: 0x%08x ======> %d",
                       nAllocCnt, lpFirstDataHeader->hTask,
                       AnchorBlock->nMemoryCeiling );
            lpCurrentTask->bLoadAllocLimit = TRUE;
         }

         ulAllocBytes = AnchorBlock->lTaskAllocSize;

         if ( ulSize > ulAllocBytes )
            ulAllocBytes = ulSize + sizeof( FreespaceRecord );

         hNewDataHeader = (LPDATAHEADER) fnInitializeDataspace( lpCurrentTask,
                                                                (zCOREMEM) &pTmp,
                                                                lpDataHeader->nTableID,
                                                                0, ulAllocBytes, 0 );
         if ( hNewDataHeader )
         {
            lpNewDataHeader = zGETPTR( hNewDataHeader );

            // Chain next DataHeader to this DataHeader.
            // I really don't know why the handle to the next Dataheader is
            // stored in two variables???? (us)
            lpNewDataHeader->lNextDataHandle = lpDataHeader->lNextDataHandle;
            lpNewDataHeader->hNextDataHeader = lpDataHeader->hNextDataHeader;
            lpNewDataHeader->hPrevDataHeader = (LPDATAHEADER) lpDataHeader->hDataHandle;
            lpNewDataHeader->hTask           = lpFirstDataHeader->hTask;
            lpDataHeader->lNextDataHandle    = (zLONG) hNewDataHeader;
            lpDataHeader->hNextDataHeader    = hNewDataHeader;
            lpDataHeader = lpNewDataHeader;
         }
         else
         {
            TraceLineX( "(mm) fnAllocDataspace for AnchorBlock unsuccessful",
                        (zLONG) AnchorBlock );
            if ( bMutexLocked )
               zUNLOCK_MUTEX( zMUTEX_MEMORY );

            return( 0 );
         }
      }

      hDataHeader = (zPVOID) lpDataHeader->hDataHandle;

   } while ( lpDataHeader );

   if ( bMutexLocked )
      zUNLOCK_MUTEX( zMUTEX_MEMORY );

   TraceLineX( "(mm) fnAllocDataspace unsuccessful for size: ", ulReqSize );
   return( 0 );

} // end of:  fnAllocDataspace

//./ ADD NAME=fnFreeDataspace
// Source Module=kzoemmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnFreeDataspace
//
//  PURPOSE:    To free a previously allocated space in the dataspace.
//
//  PARAMETERS: Address - The Address to be freed
//
//  RETURNS:    0        - The address was freed
//              Address  - The address to be freed is invalid
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zPVOID
fnFreeDataspace( zPVOID Address )
{
   LPTASK        lpCurrentTask;
   zSHORT        nPrevTableID;
   zULONG        ulSize;
// zULONG        ulFreeSize;  // the size of the freed space might be
                              // joined with another freespace
   zPLONG        lpLong;
   LPDATAHEADER  lpDataHeader;
   LPFREESPACE   lpFreespace;
   zPVOID        hTmp;
// LPFREESPACE   lpSearchFreespace;
// LPFREESPACE   lpSearchPrevFreespace;
// zPCHAR        lpSearch;

   if ( Address == 0 )
      return( 0 );

// if ( AnchorBlock->nStatus == 101 ) // DGC debugging hack.
//    return( 0 );

#if 0
   if ( g_bServerMode )
   {
      SysFree( Address );
      return( 0 );
   }
#endif

   lpFreespace = (LPFREESPACE) Address;
   lpFreespace--;  // point to the FreespaceStruct in front of the memory

   // If it is an invalid request, return Address.
   if ( lpFreespace->nTableID != iUsedFreespace &&
        lpFreespace->nTableID != iUsedFreespace1 )
   {
      LPFREESPACE lpFreespace1 = lpFreespace;

      lpFreespace1++;

#ifdef ALLOC_FREE_TRACE
      if ( AnchorBlock )
      {
         LPANCHOR lpAnchorBlock = AnchorBlock;  // zGETPTR( g_hAnchorBlock );
         if ( lpAnchorBlock->bTraceMem )
         {
            zPVOID lHandle = (zPVOID) zGETHNDL( Address );
            zLONG lHash = fnHashPointerRemove( lpAnchorBlock, lHandle, PrimeTable[ 4 ] );

            if ( lHash >= 0 )
               TraceLine( "(mm) FreeDataspace located: 0x%08x for task: 0x%08x freed %d bytes at Id: %d",
                          lpAnchorBlock->pvMemId[ lHash ], lpAnchorBlock->pvMemTask[ lHash ],
                          lpAnchorBlock->lMemSize[ lHash ], lHash );
            else
               TraceLine( "(mm) FreeDataspace not found: 0x%08x", lHandle );
         }
      }
#endif

      TraceLine( "FreeDataspace: Memory to free already freed (ID: %d) 0x%08x   ++ 0x%08x",
                 lpFreespace->nTableID, lpFreespace, lpFreespace1 );
      fnSysMessageBox( 0, "Zeidon Error", "Memory to free already freed.", 0 );
   // fnIssueCoreError( 0, lpView, 16, 14, (zLONG) lpFreespace->nPrevTableID, 0, 0 );
      return( Address );
   }

   // TODO: We only need to lock the mutex when freeing memory from the
   // system task.  However, the current state doesn't allow us to determine
   // what the task is from the freespace struct.  We need to make a change
   // that allows us to find the task and then check to see if the task is
   // the system task before locking the mutex.
   zLOCK_MUTEX( zMUTEX_MEMORY );

#ifdef ALLOC_FREE_TRACE
   if ( AnchorBlock )
   {
      LPANCHOR lpAnchorBlock = AnchorBlock;  // zGETPTR( g_hAnchorBlock );
      if ( lpAnchorBlock->bTraceMem )
      {
         zPVOID lHandle = (zPVOID) zGETHNDL( Address );
         zLONG lHash = fnHashPointerRemove( lpAnchorBlock, lHandle, PrimeTable[ 4 ] );

         if ( lHash >= 0 )
         {
#if 1
            if ( lpAnchorBlock->lMemSize[ lHash ] < 0 )
            {
               lpAnchorBlock->lMemSize[ lHash ] *= -1;
               TraceLine( "(mm) FreeDataspace: 0x%08x for task: 0x%08x freed (previous leak) %d bytes at Id: %d",
                          lpAnchorBlock->pvMemId[ lHash ], lpAnchorBlock->pvMemTask[ lHash ],
                          lpAnchorBlock->lMemSize[ lHash ], lHash );
            }
            else
            if ( lpAnchorBlock->lMemSize[ lHash ] == 1428 )
            {
               TraceLine( "(mm) FreeDataspace: 0x%08x for task: 0x%08x freed %d bytes at Id: %d",
                          lpAnchorBlock->pvMemId[ lHash ], lpAnchorBlock->pvMemTask[ lHash ],
                          lpAnchorBlock->lMemSize[ lHash ], lHash );
            }
#else
            TraceLine( "(mm) FreeDataspace: 0x%08x for task: 0x%08x freed %d bytes at Id: %d",
                       lpAnchorBlock->pvMemId[ lHash ], lpAnchorBlock->pvMemTask[ lHash ],
                       lpAnchorBlock->lMemSize[ lHash ], lHash );
#endif
            if ( lHash >= PrimeTable[ 4 ] && lHash <= lpAnchorBlock->lMemMax )
            {
               zLONG lLth = (lpAnchorBlock->lMemMax - lHash) * sizeof( zLONG );
               if ( lLth )
               {
                  zmemcpy( lpAnchorBlock->pvMemId + lHash, lpAnchorBlock->pvMemId + lHash + 1, lLth );
                  zmemcpy( lpAnchorBlock->lMemSize + lHash, lpAnchorBlock->lMemSize + lHash + 1, lLth );
                  zmemcpy( lpAnchorBlock->pvMemTask + lHash, lpAnchorBlock->pvMemTask + lHash + 1, lLth );
               }

               lpAnchorBlock->pvMemId[ lpAnchorBlock->lMemMax ] = 0;
               lpAnchorBlock->lMemMax--;
            }
            else
            if ( lHash >= 0 )
               lpAnchorBlock->pvMemId[ lHash ] = 0;
         }
         else
            TraceLine( "(mm) FreeDataspace cannot find 0x%08x", lHandle );
      }
   }
#endif

   // The "next freespace pointer" in a used memory chunk points back to
   // the data header.
   // Retrieve Dataspace Header from alloc prefix
   lpDataHeader = (LPDATAHEADER) zGETPTR( lpFreespace->hNextFreespace );
   lpCurrentTask = zGETPTR( lpDataHeader->hTask );

   // If the current task is in the process being deleted we don't need to
   // worry about keeping track of the deallocations.  DeleteTask will delete
   // the memory pages (i.e. DataHeaders) all at once.
   if ( lpCurrentTask && lpCurrentTask->bShutdown )
   {
      zUNLOCK_MUTEX( zMUTEX_MEMORY );
      return( 0 );
   }

   // ### Emergency fix 24.4.1998,HH
   // ### lpDataHeader = 0 in Frank Holzenthal's case.
   // ### We assume that the address has been freed already.
   // ### So, we just return (better than crash).
   if ( lpDataHeader == 0 )
   {
      zUNLOCK_MUTEX( zMUTEX_MEMORY );
      return( 0 );
   }

   // ###
   if ( lpDataHeader->nTableID != iDataHeader )
   {
      TraceLineI( "Memory Management Error! Wrong Dataheader Table ID: ",
                  lpDataHeader->nTableID );
      // fnIssueCoreError( 0, lpView, 16, 15, 0, 0, 0 );
      // return( Address );
   }

   // Why should we only lock the mutex when the "MainTask" is doing a free??
   // We have to lock always to prevent one thread from changing the chains when
   // a second one is doing a alloc or a free too.
   /*if ( lpDataHeader->ulTaskID == AnchorBlock->lMainTaskID )  // TODO: only for main task because
   {                                                            // otherwise the only one to change
      zLOCK_MUTEX( zMUTEX_MEMORY );                             // the chain is the task itself,
   }*/                                                          // which is single-threaded

   if ( lpCurrentTask )
      lpCurrentTask->ulTrackTaskMemory -= lpFreespace->ulFreeLth;

// if ( lpFreespace->nUsedTableID == 10100 )
//    TraceLine( "FreeDataspace: (ID: %d %d) 0x%08x for Task: 0x%08x",
//               lpFreespace->nTableID, lpFreespace->nUsedTableID, lpFreespace, lpCurrentTask );

// TraceLine( "fnFreeDataspace freeing (%d) Freespace: 0x%08x",
//            lpFreespace->ulDebugID, lpFreespace );

   // Get size of area to be freed.
   ulSize = lpFreespace->ulFreeLth;

   // Retain the previous table ID which is the first short in the structure.
   {
      zPSHORT lpnAddress;

      lpnAddress   = (zPSHORT) Address;
      nPrevTableID = *lpnAddress;
   }

   // Clear out first few bytes of area in case an internal table was stored
   // there (so the table id is cleared).
   // Minimum size of an allocated block is 4 Bytes!
   lpLong = (zPLONG) Address;
   *lpLong = 0;

   // Set user-data to all 0.
// zmemset( Address, 0,
//          (unsigned int) (ulSize - (zsizeof( FreespaceRecord ))) );

   // Reset nTableID to Freespace
   lpFreespace->nTableID     = iFreespace;
   lpFreespace->nUsedTableID = 0;
   lpFreespace->nPrevTableID = nPrevTableID;

   // Change the freespace handle to point to the current freespace structure
   // (it used to point to the useable space AFTER the freespace structure).
   hTmp = lpFreespace->hFreespace;
   lpFreespace->hFreespace = fnSysCreateHandle( lpDataHeader, lpFreespace );

   // Bump down used entries
   lpDataHeader->ulCntUsedEntries--;
   lpDataHeader->ulUsedSpace -= lpFreespace->ulFreeLth;
   lpDataHeader->ulFreedMemSize += lpFreespace->ulFreeLth;
   if (lpDataHeader->ulUsedSpace & 0x80000000)
   {
      TraceLineI( "Memory Management! Used memory size becomes negative: ",
                  lpDataHeader->ulUsedSpace );
   }

   // We have now freed up an entry!  If the entry count is 0 and this is
   // not the first dataspace for the task, return the space to the system.
   if ( lpDataHeader->ulCntUsedEntries == 0 && lpDataHeader->hPrevDataHeader )
   {
      zLONG        hDataHandle;
      LPDATAHEADER lpPrevDataHeader;

      hDataHandle = lpDataHeader->hDataHandle;
   // TraceLineI( "Free Block:          ", ((unsigned long) hDataHandle >> 24));

      // Chain Previous Dataspace to next Dataspace
      lpPrevDataHeader = zGETPTR( lpDataHeader->hPrevDataHeader );
      hDataHandle = lpPrevDataHeader->lNextDataHandle;
      lpPrevDataHeader->lNextDataHandle = lpDataHeader->lNextDataHandle;
      lpPrevDataHeader->hNextDataHeader = lpDataHeader->hNextDataHeader;
      if ( lpDataHeader->hNextDataHeader )
      {
         LPDATAHEADER lpNextDataHeader;

         lpNextDataHeader = zGETPTR( lpDataHeader->hNextDataHeader );
         lpNextDataHeader->hPrevDataHeader = lpDataHeader->hPrevDataHeader;
      }

      fnFreeSharedMemory( lpCurrentTask, hDataHandle );

      zUNLOCK_MUTEX( zMUTEX_MEMORY );

      return( 0 );  // return 0 address for space freed
   }

   hTmp = 0;
   if ( lpDataHeader->ulFreedMemSize >
        lpDataHeader->ulUseableSize / GARBAGE_COLLECT_FACTOR )
   {
      // Now clean up all Freed Memory chunks that are not yet chained
      // into the freespace chain.
      hTmp = fnChainFreespaces( lpDataHeader );
   }

   zUNLOCK_MUTEX( zMUTEX_MEMORY );

   return( hTmp );  // return null for space freed

} // end of:  fnFreeDataspace

//./ ADD NAME=fnGetDataspaceLength
// Source Module=kzoemmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnGetDataspaceLength
//
//  PURPOSE:    To return the length of the allocated space in the dataspace
//
//  PARAMETERS: Address - The Address for which to return allocated length
//
//  RETURNS:    0       - The address is invalid
//              Address - The length of the allocated space
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zULONG
fnGetDataspaceLength( zPVOID Address )
{
   LPDATAHEADER  lpDataHeader;
   LPFREESPACE   lpFreespace;

   if ( Address == 0 )
      return( 0 );

#if 0
   if ( g_bServerMode )
      return( SysMemSize( Address ) );
#endif

   lpFreespace = (LPFREESPACE) Address;
   lpFreespace--;

   // If it is an invalid request, return 0.
   if ( lpFreespace->nTableID != iUsedFreespace &&
        lpFreespace->nTableID != iUsedFreespace1 )
   {
      fnSysMessageBox( 0, "Zeidon Error",
                       "Freespace request invalid", 0 );
   // fnIssueCoreError( 0, lpView, 16, 14, (zLONG) lpFreespace->nPrevTableID, 0, 0 );
      return( 0 );
   }

   // Retrieve Dataspace Header from alloc prefix
   lpDataHeader = (LPDATAHEADER) zGETPTR( lpFreespace->hNextFreespace );
   if ( lpDataHeader->nTableID != iDataHeader )
   {
      fnSysMessageBox( 0, "Zeidon Error",
                       "Data header request invalid", 0 );
   // fnIssueCoreError( 0, lpView, 16, 15, 0, 0, 0 );
      return( 0 );
   }

   // Get size of area
   return( lpFreespace->ulFreeLth - sizeof( FreespaceRecord ) );
}

//./ ADD NAME=fnInitializeDataspace
// Source Module=kzoemmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnInitializeDataspace
//
//  PURPOSE:    To Initialize a new dataspace in which to store data
//
//  PARAMETERS: Address  - The return address of the Dataspace allocated
//              nTableID - iAnchor for anchor, iTask for task
//              szTitle  - Identifying string for dataspace
//              lUseableSize - Initial size
//              lIncrementSize - Amount to increment by when needed
//              szShareName - ShareName to pass to fnAllocSharedMemory
//
//  We shouldn't have to put a mutex in this code because it only gets called
//  from two places: InitializeAnchorBlock (which is protected by the INIT
//  mutex) and fnAllocDataspace (which is protected by the MEMORY mutex).
//  Since both places are protected by mutexes we shouldn't need it here.
//
//  RETURNS:    zLONG - The Handle returned from fnAllocSharedMemory
//              Address - the address of the memory
//
//              0    - The memory could not be allocated
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zULONG
fnInitializeDataspace( LPTASK         lpCurrentTask,
                       zCOREMEM       Address,
                       zSHORT         nTableID,
                       zCPCHAR        cpcTitle,
                       zULONG         ulBytes,
                       zCPCHAR        cpcShareName )
{
   zULONG       ulHandle = 0;
   LPDATAHEADER lpDataHeader;
   LPFREESPACE  lpFreespace;
   zULONG       ulTotalSize;

#if 0
   // If we're running in server mode then we will be allocating local memory,
   // so there's nothing to do.
   if ( g_bServerMode )
   {
      *Address = SysMalloc( ulBytes );
      return( (zLONG) *Address );
   }
#endif

   ulTotalSize = ulBytes + sizeof( DataHeaderRecord );
   if ( ulTotalSize % zALIGN_MEM_SIZE )
      ulTotalSize += zALIGN_MEM_SIZE - ulTotalSize % zALIGN_MEM_SIZE;

   // Allocate space for shared memory + data header.
   ulHandle = fnAllocSharedMemory( (zCOREMEM) Address, lpCurrentTask,
                                   ulTotalSize, cpcShareName );
   if ( *Address == 0 )
      return( 0 );

   // Initialize Header To Table.
   lpDataHeader = (LPDATAHEADER) *Address;

   // Clear header to 000000000...
   zmemset( lpDataHeader, 0, sizeof( DataHeaderRecord ) );

   lpDataHeader->nTableID            = iDataHeader;
   lpDataHeader->ulMaxEmptyBlockSize = ulBytes;
   lpDataHeader->hTask               = zGETHNDL( lpCurrentTask );
   lpDataHeader->hDataHandle         = ulHandle;
   lpDataHeader->ulUseableSize       = ulBytes; // + sizeof( DataHeaderRecord );

   if ( g_bTraceDataspace )
   {
      TraceLine( "Allocating new space for Task (0x%08x) [0x%08x] size = %d",
                 lpDataHeader->hTask, lpDataHeader, lpDataHeader->ulUseableSize );
   }

   // Initialize Table.
   lpFreespace = (LPFREESPACE) (lpDataHeader + 1);
   lpFreespace->hFreespace = fnSysCreateHandle( lpDataHeader, lpFreespace );

   // Initialize the root freespace record contained in header record.
// lpDataHeader->Freespace.nTableID      = nTableID;
   lpDataHeader->Freespace.nTableID      = iFreespace;
   lpDataHeader->Freespace.nPrevTableID  = 0;
   lpDataHeader->Freespace.ulFreeLth     = 0;  // this doesn't own any space
   lpDataHeader->ulCntFreeSpaces         = 1;  // there is one big initial freespace
   lpDataHeader->Freespace.hNextFreespace = (LPFREESPACE) lpFreespace->hFreespace;

// lpFreespace->nTableID       = nTableID;
   lpFreespace->nTableID       = iFreespace;
   lpFreespace->nPrevTableID   = 0;
   lpFreespace->ulFreeLth      = ulBytes;
   lpFreespace->hNextFreespace = 0;

   return( ulHandle );   // return the handle to the new dataspace.
}
