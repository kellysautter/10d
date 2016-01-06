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

zCHAR *szTest[ ] = { "CROCODILE", "ALLIGATOR", "CHIMPANZEE",
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

//#define DEBUG_ALL
#ifdef DEBUG_ALL


#endif

#if 0
void
BuildHeap( LPSORTDATA       lpData,
           LPENTITYINSTANCE lpEI[],
           zULONG           ulRecordCnt ) // recursive
{
   zULONG k;

   k = ulRecordCnt / 2;
   while( k > 0 )
   {
      Heapify( lpData, lpEI, ulRecordCnt, k-- );
   }
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
Heapify( LPSORTDATA       lpData,
         LPENTITYINSTANCE lpEI[],
         zULONG           ulRecordCnt,
         zULONG           k )
{
   ASSERT_TRACE( "Heapify error", 0, k <= ulRecordCnt );
   zULONG ulLeft = k << 1;
   zULONG ulLargest;

   if ( ulLeft > ulRecordCnt )
   {
      return;
   }

   ulLargest = k;
   if ( CompareRecords( ulLeft, ulLargest ) > 0 )
   {
      ulLargest = ulLeft;
   }

   ulLeft++;   // ulRight (use ulLeft to conserve stack space)
   if ( ulLeft <= ulRecordCnt )
   {
      if ( CompareRecords( ulLeft, ulLargest ) > 0 )
      {
         ulLargest = ulLeft;
      }
   }

   if ( k != ulLargest )
   {
      zULONG *p1;
      zULONG *p2;

      p1 = CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx );
      p2 = CSIMP_GETSORTIDX_PTR( ulLargest, m_pUseIdx->m_pxIdx );
      ulLeft = *p1;  // use ulLeft to conserve stack space
      *p1 = *p2;
      *p2 = ulLeft;

      Heapify( lpData, lpEI, ulRecordCnt, ulLargest );
   }
}  // end of:  Heapify

#else

////////////////////////////// BuildHeap ////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void
BuildHeap( LPSORTDATA       lpData,
           LPENTITYINSTANCE lpEI[],
           zULONG           ulRecordCnt ) // non-recursive
{
   zULONG k;

#ifdef DEBUG_ALL
   OutputDebugString( "\nShowing heap prior to build heap ...\n" );
   ShowHeap( this, m_pUseIdx->m_pxIdx );
#endif

   k = ulRecordCnt / 2;
   while( k > 0 )
   {
      zULONG ul = Heapify( lpData, lpEI, k-- );
      while ( ul )
      {
         ul = Heapify( lpData, lpEI, ul );
      }
   }
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
zULONG
Heapify( LPSORTDATA       lpData,
         LPENTITYINSTANCE lpEI[],
         zULONG           ulRecordCnt,
         zULONG           k )
{
   ASSERT_TRACE( "Heapify error", 0, k <= ulRecordCnt );
   zULONG ulLeft = k << 1;
   zULONG ulLargest;

   if ( ulLeft > ulRecordCnt )
   {
      return( 0 );
   }

   ulLargest = k;
   if ( CompareRecords( ulLeft, ulLargest ) > 0 )
   {
      ulLargest = ulLeft;
   }

   ulLeft++;   // ulRight (use ulLeft to conserve stack space)
   if ( ulLeft <= ulRecordCnt )
   {
      if ( CompareRecords( ulLeft, ulLargest ) > 0 )
      {
         ulLargest = ulLeft;
      }
   }

   if ( k != ulLargest )
   {
      zULONG *p1;
      zULONG *p2;

      // Use uLeft to conserve stack.
      p1 = CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx );
      p2 = CSIMP_GETSORTIDX_PTR( ulLargest, m_pUseIdx->m_pxIdx );
      ulLeft = *p1;
      *p1 = *p2;
      *p2 = ulLeft;
      return( ulLargest );
   }

   return( 0 );
}  // end of:  Heapify

#endif

////////////////////////////// HeapSort /////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void
HeapSort( LPSORTDATA       lpData,
          LPENTITYINSTANCE lpEI[],
          zULONG           ulRecordCnt )
{
   BuildHeap( lpData, lpEI, ulRecordCnt );

   zULONG ulHoldCnt = ulRecordCnt;
   zULONG ulHoldMax = ulMaxRecords;
   zULONG *p1;
   zULONG ul;
   while ( ulRecordCnt > 1 )
   {
      ulRecordCnt--;
      m_lMaxRecords--;

      p1 = CSIMP_GETSORTIDX_PTR( ulRecordCnt + 1, m_pUseIdx->m_pxIdx );
      ul = *p1;
      *p1 = *(m_pUseIdx->m_pxIdx);
      *(m_pUseIdx->m_pxIdx) = ul;

      ul = Heapify( 1 );
      while ( ul )
      {
         ul = Heapify( ul );
      }
   }

   ulRecordCnt = ulHoldCnt;
   m_lMaxRecords = ulHoldMax;
}

/////////////////////////////// QuickSort ///////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void
QuickSort( zULONG ulRecord1,    // recursive
           zULONG ulRecord2 )
{
   static WORD wRecurseCnt = 0;
   wRecurseCnt++;
#ifdef DEBUG_ALL
   static WORD wMaxRecurse = 0;
   if ( wMaxRecurse < wRecurseCnt )
   {
      wMaxRecurse = wRecurseCnt;
   }
#endif

   if ( ulRecord1 < ulRecord2 )
   {
      zULONG ul = QuickSortPartition( ulRecord1, ulRecord2 );

      QuickSort( ulRecord1, ul );
      QuickSort( ul + 1, ulRecord2 );
   }

   wRecurseCnt--;

#ifdef DEBUG_ALL
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

/////////////////////////////// QuickSortNR /////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void
QuickSortNR( zULONG ulRecord1,    // non-recursive
             zULONG ulRecord2 )
{
   ASSERT_TRACE( "QuickSort argument error", 0, ulRecord1 <= ulRecord2 );
   zULONG m = ulRecord2 - ulRecord1;
   zULONG ulStackSize = 2;
   while ( m >>= 1 )
   {
      ulStackSize++;
   }

   ulStackSize <<= 2;     // two entries for each comparison
   zPULONG pulStack = (zPULONG)
                       m_pPoolVarData->Alloc( ulStackSize * sizeof( zULONG ) );
   zULONG ulStack = 0;
   zULONG uLeft;
   zULONG uRight;

#ifdef DEBUG_ALL
   zULONG ulStackMax = 0;
#endif

   pulStack[ ulStack++ ] = ulRecord1;
   pulStack[ ulStack++ ] = ulRecord2;
   while ( ulStack )
   {
      uRight = pulStack[ --ulStack ];
      uLeft = pulStack[ --ulStack ];
      m = QuickSortPartition( uLeft, uRight );

#ifdef DEBUG_ALL
      ShowArray( uLeft, uRight );
#endif

      if ( m + 1 < uRight )
      {
#ifdef DEBUG_ALL
         if ( ulStack > ulStackSize - 2 )
         {
            ASSERT_TRACE( "QuickSort stack overflow - %d", ulStackSize, FALSE );
            m_pPoolVarData->Free( pulStack );
            return;
         }
#endif

         pulStack[ ulStack++ ] = m + 1;
         pulStack[ ulStack++ ] = uRight;

#ifdef DEBUG_ALL
         if ( ulStack > ulStackMax )
         {
            ulStackMax = ulStack;
         }
#endif
      }

      if ( uLeft < m )
      {
#ifdef DEBUG_ALL
         if ( ulStack > ulStackSize - 2 )
         {
            ASSERT_TRACE( "QuickSort stack overflow - %d", ulStackSize, FALSE );
            m_pPoolVarData->Free( pulStack );
            return;
         }
#endif

         pulStack[ ulStack++ ] = uLeft;
         pulStack[ ulStack++ ] = m;

#ifdef DEBUG_ALL
         if ( ulStack > ulStackMax )
         {
            ulStackMax = ulStack;
         }
#endif
      }
   }

   m_pPoolVarData->Free( pulStack );

#ifdef DEBUG_ALL
   ASSERT_TRACE( "Max stack utilization = %d\tAllocated stacksize = %d",
                 (ulStackMax, ulStackSize), FALSE );
#endif
}

/////////////////////////////// QuickSortPartition //////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
zULONG
QuickSortPartition( zULONG ulRecord1,
                           zULONG ulRecord2 )
{
   zULONG k = ulRecord1 - 1;
   zULONG j = ulRecord2 + 1;
   zULONG *p1;
   zULONG *p2;
   zULONG ul;

   while ( TRUE )
   {
      while ( CompareRecords( --j, ulRecord1 ) > 0 )
         ;

      while ( CompareRecords( ++k, ulRecord1 ) < 0 )
         ;

      if ( k < j )
      {
         p1 = CSIMP_GETSORTIDX_PTR( k, m_pUseIdx->m_pxIdx );
         p2 = CSIMP_GETSORTIDX_PTR( j, m_pUseIdx->m_pxIdx );
         ul = *p1;
         *p1 = *p2;
         *p2 = ul;
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


