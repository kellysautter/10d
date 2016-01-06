/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadw8aw.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2000.08.04  BL+HH
   Fixed stack overwrite on high asciis (unsigned required).
21.08.1997 DonC
   Corrected data type of selMemory variable in qchange routine.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


// local prototypes
static void
fnLoadDeltaOne( int  nPatternLth,
                unsigned char *szPattern,
                LPLONG lplDelta1Table );
static void
fnLoadDeltaTwo( int  nPatternLth,
                unsigned char *szPattern,
                LPLONG lplDelta2Table );


void FAR PASCAL
qbldelta( int  nPatternLth,         // pattern length
          zPCHAR szPattern,         // pattern to locate
          LPLONG lplDelta1Table,    // Boyer-Moore Delta1 Table
          LPLONG lplDelta2Table )   // Boyer-Moore Delta2 Table
{
   fnLoadDeltaOne( nPatternLth, (unsigned char *) szPattern, lplDelta1Table );
   fnLoadDeltaTwo( nPatternLth, (unsigned char *) szPattern, lplDelta2Table );
}

static void
fnLoadDeltaOne( int  nPatternLth,
                unsigned char *szPattern,
                LPLONG lplDelta1Table )
{
   int  nPatternLthMinus1;
   int  k;

   nPatternLthMinus1 = nPatternLth - 1;

   // Init Delta1 to pattern length for all characters.
   for ( k = 0; k < ASCIISIZE; k++ )
      lplDelta1Table[ k ] = nPatternLth;

   // Set Delta1 for characters in pattern.
   for ( k = 0; k < nPatternLth; k++ )
      lplDelta1Table[ szPattern[ k ] ] = nPatternLthMinus1 - k;

   // Set Delta1 to large value for last zCHAR in pattern.
   lplDelta1Table[ szPattern[ nPatternLthMinus1 ] ] = LARGE;

}   // end of:  fnLoadDeltaOne

static void
fnLoadDeltaTwo( int  nPatternLth,         // using Rytter's correction
                unsigned char *szPattern, // to Knuth's scheme
                LPLONG lplDelta2Table )
{
   int  k,
        q,  j,  t,
        qi, ji, ti,
        nTwicePatternLth;
   int  f[ MAXPATTERN ],
        fi[ MAXPATTERN ];

   nTwicePatternLth = nPatternLth * 2;

   for ( k = 1;  k <= nPatternLth;  k++ )
      lplDelta2Table[ k ] = nTwicePatternLth - k;

   j = nPatternLth;
   t = nPatternLth + 1;

   while ( j > 0 )
   {
      f[ j ] = t;
      while ( t <= nPatternLth  &&
              szPattern[ j - 1 ] != szPattern[ t - 1 ] )
      {
         if ( (zLONG) nPatternLth - j < lplDelta2Table[ t ] )
            lplDelta2Table[ t ] = nPatternLth - j;

         t = f[ t ];

      }   /*  end of:  while ( t <= ... )  */

      t--;
      j--;

   }   /*  end of:  while ( j > 0 )  */

   q = t;
   t = nPatternLth + 1 - q;
   qi = 1;
   ji = 1;
   ti = 0;

   while ( ji <= t )
   {
      fi[ ji ] = ti;
      while ( ti >= 1  &&  szPattern[ ji - 1 ] != szPattern[ ti - 1 ] )
         ti = fi[ ti ];

      ti++;
      ji++;

   } //  end of:   while ( ji <= t )

   while ( q < nPatternLth )
   {
      for ( k = qi;  k <= q; k++ )
         if ( (zLONG) nPatternLth + q - k  <  lplDelta2Table[ k ] )
            lplDelta2Table[ k ] = nPatternLth + q - k;

      qi = q + 1;
      q += t - fi[ t ];
      t = fi[ t ];

   }  //  end of:  while ( q < nPatternLth )

}  //  end of:  fnLoadDeltaTwo

/////////////////////////////////////////////////////////////////////////////
//  KZEDT2AA.C  -  written by Doug Sorensen    - create date 02/10/89
//                (C)  Quinsoft Inc., 1989     - last update 02/12/89
//
//  This program searches a buffer for a string using the Boyer-Moore
//  algorithm.
//
//  Input Parms:
//     1. Pattern to locate
//     2. Pattern length
//     3. Buffer to be searched for pattern
//        - expanded (uncompressed) as necessary
//        - fixed length records
//     4. Length of Buffer to be searched
//     5. Boyer-Moore Delta1 table
//     6. Boyer-Moore Delta2 table
//
//  Returns:
//     1. Location of pattern within buffer(-1 if not found)
//
/////////////////////////////////////////////////////////////////////////////
long FAR PASCAL
qfind( int  nPatternLth,         // pattern length
       zPCHAR szPattern1,        // pattern to locate
       long lBufferLth,          // length of buffer to be searched
       zPCHAR Buffer,            // buffer to be searched for pattern
       LPLONG lplDelta1Table,    // Boyer-Moore Delta1 Table
       LPLONG lplDelta2Table )   // Boyer-Moore Delta2 Table
{
   long lPatternLoc;
   int  nPatternLthMinus1;
   long j, k;
   unsigned char* szPattern = (unsigned char*)szPattern1;


   if ( nPatternLth <= 0 )
      return( -1 );       // signal "not found in buffer"

   lPatternLoc = -1;

   if ( nPatternLth == 1 )
   {
      k = 0;
      while ( k < lBufferLth )
      {
         if ( szPattern[ 0 ] == Buffer[ k ] )
         {
            lPatternLoc = k;
            break;
         }
         else
            k++;
      }

      return( lPatternLoc );
   }

   nPatternLthMinus1 = nPatternLth - 1;
   k = nPatternLthMinus1;

   while ( k < lBufferLth )
   {
      while ( k < lBufferLth )     // FAST Loop
      {
      // printf( "%c - k before = %d  ", Buffer[ k ], k );
         k += lplDelta1Table[(unsigned char) Buffer[ k ] ];
      // printf( " --  k after = %d\n", k );
      }

      if ( k < LARGE )      // really past true end of string ??
         break;             //   yes... terminate search

      // Undo the large skip caused by the match found in the FAST loop.
      // Check for complete match in SLOW loop.
      k -= LARGE;
      k--;
   // printf( "k after FAST loop match = %d\n", k );
      j = nPatternLthMinus1 - 1;
      while ( Buffer[ k ] == szPattern[ j ] )     /* SLOW Loop */
      {
         if ( j > 0 )
         {
            k--;
            j--;
         }
         else
         {
            lPatternLoc = k;  // pattern found ... terminate search
            k = LARGE;        // permit outer while exit
            break;            // exit inner while
         }
      }   // end of:   while ( Buffer[ k ] == szPattern[ j ] )

      if ( lPatternLoc == -1 )   // pattern not yet found
      {
         j = k + lplDelta2Table[ j + 1 ];
         k += lplDelta1Table[ Buffer[ k ] ];
         if ( k < j  ||  k >= LARGE )  // take largest possible jump
            k = j;
      }

   }   // end of outer:  while ( k <= lBufferLth )

   return( lPatternLoc );

}  // end of:  qfind

/////////////////////////////////////////////////////////////////////////////
//  KZEDT3AA.C  -  written by Doug Sorensen    - create date 02/14/89
//                (C)  Quinsoft Inc., 1989     - last update 02/14/89
//
//  This program makes the requested change at the current location
//  and attempts to locate the next occurrence of the string to be
//  changed in the current buffer.
//
//  Input Parms:
//     1. Position at which to replace
//     2. Search Pattern Length
//     3. Search Pattern
//     4. Replace Pattern Length
//     5. Replace Pattern
//     6. Length of buffer to be searched
//     7. Buffer to be searched for pattern
//     8. Boyer-Moore Delta1 Table
//     9. Boyer-Moore Delta2 Table
//    10. Maximum length of resultant string
//
//  Outputs:
//     1. File with line #, line, and file name where string found
//        for each string (named CHANGE.RPT)
//
/////////////////////////////////////////////////////////////////////////////
long FAR PASCAL
qchange( long lCurrentPos,       // current position at which to repl
         int  nPatternLth,       // pattern length
         zPCHAR szPattern1,      // pattern to locate
         int  nReplaceLth,       // replace string length
         zPCHAR szReplace,       // replace string
         LPLONG lBufferLth,      // length of buffer to be searched
         zPCHAR Buffer,          // buffer to be searched for pattern
         LPLONG lplDelta1Table,  // Boyer-Moore Delta1 Table
         LPLONG lplDelta2Table,  // Boyer-Moore Delta2 Table
         long lMaxLth )          // Maximum lth of resultant string
{
   long lRemainingLth,
        lReturnPos;
   int  nTrueDeltaLth;
   long k;
   unsigned char* szPattern = (unsigned char*)szPattern1;

   // If the original and replacement patterns are the same length,
   // the substitution is trivial.
   // Otherwise, preserve original placement of characters within the
   // original buffer as much as possible by checking spacing.
   //
   // e.g.  changing Doug to Jim
   // KZEDT3AA.C  -  written by Doug Sorensen   - create date 02/14/89
   // KZEDT3AA.C  -  written by Jim Sorensen    - create date 02/14/89
   //
   nTrueDeltaLth = nReplaceLth - nPatternLth;
   if ( nTrueDeltaLth == 0 )
   {
      for ( k = 0; k < nReplaceLth; )
         Buffer[ lCurrentPos++ ] = szReplace[ k++ ];

      lReturnPos = lCurrentPos;
   }
   else
   {
      zPCHAR lpBuffer;
      zLONG  selMemory;

      selMemory = SysAllocMemory((zCOREMEM) &lpBuffer, lMaxLth, 0,
                                  zCOREMEM_ALLOC, 0 );
      if ( *lBufferLth + nTrueDeltaLth > lMaxLth )
         return( *lBufferLth + nTrueDeltaLth );     // signal error

      lRemainingLth = *lBufferLth - lCurrentPos - nPatternLth + 1;

      for ( k = 0; k < lRemainingLth; k++ )
         lpBuffer[ k ] = Buffer[ lCurrentPos + nPatternLth + k ];

      for ( k = 0; k < nReplaceLth; )
         Buffer[ lCurrentPos++ ] = szReplace[ k++ ];

      lReturnPos = lCurrentPos;

      k = 0;
      while ( lRemainingLth > 0 )
      {
         if ( lpBuffer[ k ] == '\n' )
         {
            while ( lRemainingLth-- )
               Buffer[ lCurrentPos++ ] = lpBuffer[ k++ ];
         }
         else
         if ( lpBuffer[ k ] == ' ' &&  lpBuffer[ k + 1 ] == ' ' )
         {
            if ( nTrueDeltaLth < 0 )
            {
               while ( nTrueDeltaLth )
               {
                  Buffer[ lCurrentPos++ ] = ' ';
                  nTrueDeltaLth++;
               }

               while ( lRemainingLth-- )
                  Buffer[ lCurrentPos++ ] = lpBuffer[ k++ ];
            }
            else
            if ( nTrueDeltaLth > 0 )
            {
               k++;
               nTrueDeltaLth--;
               lRemainingLth--;
            }
            else
            {
               while ( lRemainingLth-- )
                  Buffer[ lCurrentPos++ ] = lpBuffer[ k++ ];
            }
         }   // end of:  if ( lpBuffer[ i ] == ' ' &&  s...
         else
         {
            Buffer[ lCurrentPos++ ] = lpBuffer[ k++ ];
            lRemainingLth--;
         }

      }   // end of:  while ( lRemainingLth > 0 )

      *lBufferLth += nTrueDeltaLth;

      SysFreeMemory( selMemory );

   }   // end of:  else of:  if ( !nTrueDeltaLth )

   return( lReturnPos );
}

/////////////////////////////////////////////////////////////////////////////
//  KZEDT4AA.C     -  written by Doug Sorensen - create date 02/10/89
//                (C)  Quinsoft Inc., 1989     - last update 02/12/89
//
//  This program searches a buffer for a string using the Boyer-Moore
//  algorithm (reverse direction).
//
//  Input Parms:
//     1. Pattern to locate
//     2. Pattern length
//     3. Buffer to be searched for pattern
//        - expanded (uncompressed) as necessary
//        - fixed length records
//     4. Length of Buffer to be searched
//     5. Boyer-Moore Delta1 table
//     6. Boyer-Moore Delta2 table
//
//  Returns:
//     1. Location of pattern within buffer (-1 if not found)
//
/////////////////////////////////////////////////////////////////////////////
long FAR PASCAL
qfindr( int    nPatternLth,      // pattern length
        zPCHAR szPattern1,       // pattern to locate
        long   lBufferLth,       // length of buffer to be searched
        zPCHAR Buffer,           // buffer to be searched for pattern
        LPLONG lplDelta1Table,   // Boyer-Moore Delta1 Table
        LPLONG lplDelta2Table )  // Boyer-Moore Delta2 Table
{
   long lPatternLoc,
        lHold_k;
   int  negPatternLth;
   long j, k;
   unsigned char* szPattern = (unsigned char *) szPattern1;


   if ( nPatternLth <= 0 )
      return( -1 );       // signal "not found in buffer"

   lPatternLoc = -1;

   if ( nPatternLth == 1 )
   {
      k = lBufferLth;
      while ( k >= 0 )
      {
         if ( szPattern[ 0 ] == Buffer[ k ] )
         {
            lPatternLoc = k;
            break;
         }
         else
            k--;
      }

      return( lPatternLoc );
   }

   negPatternLth = -nPatternLth;

   // Outer loop to:
   // Position pattern within Buffer for next set of comparisons.
   k = lBufferLth;

   while ( k >= 0 )
   {
      while ( k >= 0 )     // FAST Loop
      {
         k -= lplDelta1Table[ Buffer[ k ] ];
      }

      if ( k >= negPatternLth ) // really past true end of string ??
         break;                 //   yes... terminate search

      // Undo the large skip caused by the match found in the FAST loop.
      // Check for complete match in SLOW loop.
      k += LARGE;
      lHold_k = k;  // holds pos of k in case words don't match
      j = nPatternLth - 1;
      while ( Buffer[ k ] == szPattern[ j ] )     // SLOW Loop
      {
         if ( j > 0 )
         {
            k++;
            j = j - 1;
         }
         else
         {
            lPatternLoc = lHold_k; // pattern found terminate search
            k = negPatternLth - 1; // permit outer while exit
            break;                 //  exit inner while
         }
      }   // end of: while ( Buffer[ k ] == szPattern[ j ] )

      if ( lPatternLoc == -1 )   // pattern not yet found
      {
         k = lHold_k - 1; // begins search from first matching chars

      //////////////////////////////////////////////////////////////////
      // I took this code out, because if the word I was searching
      // for had all the same characters ex. "xxxxxxx" then if an 'x'
      // was found in the middle of the word, Buffer and Pattern don't
      // match and when this code gets executed, it positions the
      // cursor before the word, so that the word never gets rechecked.
      //
      // j = k - lplDelta2Table[ j - 1 ];
      // k -= lplDelta1Table[Buffer[ k ]];
      //    take largest possible jump
      // if ( k > j  ||  k < negPatternLth )
      //    k = j;
      //////////////////////////////////////////////////////////////////
      }

   }   // end of outer:  while ( k >= 0 )

   return( lPatternLoc );

}   // end of:  main
