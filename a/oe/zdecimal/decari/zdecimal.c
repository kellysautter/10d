/*
** This is the special DECARI arithmetic version
*/
/* CHANGE LOG

2002.01.28  HH
   added zDEC_CMD_ROUND.
2002.01.09  DGC
   Changed the way NULLs are compared to fit with Zeidon logic.


04.06.1998   HH
   Check pointer for null.
22.06.1998   HH
   corrected assignment to NULL.
11.08.1998   HH
   improved converting to string (rounding).
12.11.1998   RG
   if a double has a ',' instead of '.' change
   it to '.'
17.03.1999  HH
   use %.15lE instead of %lg converting double to decimal.
   it to '.'
24.10.2000  TMV
   fix bug in conversion routine when conveting DecimalStruct to double
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "kzoeenaa.h"
#include "zdecimal.h"
#include "decari.h"

/*

   Compare two decimal numbers.  Since decari doesn't properly handle
   NULL values the way we want, we have to do extra work if there are
   NULL values.

*/
static zSHORT
fnCompare( tDec *d1, tDec *d2 )
{
   zSHORT nRC;

   nRC = decari_compare( d1, d2 );
   if ( nRC == 2 )
   {
      // At least one of the operands is NULL.
      if ( !decari_isnan( d1 ) )
      {
         // d1 is NOT NULL, so d2 must be NULL.  This means that d1 is
         // greater than d2.
         return( 1 );
      }

      if ( decari_isnan( d2 ) )
         return( 0 ); // Both are NULL, so they compare equal.

      // d1 is NULL and d2 is not, so d1 is less than d2.
      return( -1 );
   }

   return( nRC );
}

zSHORT OPERATION
DecimalOperation( void  *pvReturn,
                  void  *pvDecimal,
                  void  *pvInfo,
                  long  lControl )
{
   char szTemp[ 256 ];
   long lCommand = lControl & zDEC_CMD;
   char *pc;

   switch ( lCommand )
   {
      case zDEC_CMD_ASSIGNDEC:
         // Copy from 1 decimal to another.
         memcpy( pvReturn, pvDecimal, sizeof( zDECIMAL ) );
         return( 0 );

      case zDEC_CMD_ASSIGNNULL:
         // Assign NULL to a decimal.
         memset( pvReturn, 0xff, sizeof( zDECIMAL ) );
         return( 0 );

      case zDEC_CMD_DECTOSTRING:
      {
         short nPrecision = *(short *) pvInfo;
         tDec tempDecimal;

         // Convert zDECIMAL to string.
         //  The area pointed to by pvReturn must be long enough
         //  to hold the string.

         /* normal formatting
            first round to the given nPrecision (digits after comma)
         */
         memcpy( &tempDecimal, pvDecimal, sizeof( tDec ) );
         if( nPrecision >= 0 )
            decari_round( &tempDecimal, nPrecision );
         decari_to_string( (char*) pvReturn, &tempDecimal, nPrecision, 'F' );
         return( 0 );
      }

      case zDEC_CMD_STRINGTODEC:
         // Convert string to decimal.
         decari_from_string( (tDec*) pvReturn, (char *) pvInfo) ;
         return( 0 );

      case zDEC_CMD_DECTOLONG:
         // Convert decimal to long.
         // convert to string with decimal part truncated
         decari_to_string( szTemp, (tDec*) pvDecimal, 0, 'F' );
         {
            char *pc = strchr( szTemp, '.' );
            if( pc )
               *pc = '\0' ;
         }
         * (long *) pvReturn = zatol(szTemp);
         return( 0 );

      case zDEC_CMD_LONGTODEC:
         // Convert long to decimal.
         sprintf_s(szTemp, sizeof( szTemp ), "%ld", * (long *) pvInfo);
         decari_from_string( (tDec*) pvReturn, szTemp) ;
         return( 0 );

      case zDEC_CMD_DECTODOUBLE:
         // Convert decimal to double.
         decari_to_string( szTemp, (tDec*) pvDecimal, -1, 'E' );
         *(double *) pvReturn = atof( szTemp );
         return( 0 );

      case zDEC_CMD_DOUBLETODEC:
         // Convert double to decimal.
         sprintf_s(szTemp, sizeof( szTemp ), "%.15lE", * (double *) pvInfo);
         if ( (pc = strchr (szTemp, ',')) != NULL )
            *pc = '.';
         decari_from_string( (tDec*) pvReturn, szTemp) ;
         return( 0 );

      case zDEC_CMD_COMPARENULL:
         // Test for NULL value (NaN)
         if ( decari_isnan( (tDec*)pvDecimal ) )
            return( 0 );
         else
            return( 1 );

      case zDEC_CMD_COMPAREDOUBLE:
      {
         zDECIMAL dTemp;
         sprintf_s(szTemp, sizeof( szTemp ), "%.15lE", * (double *) pvInfo);
         decari_from_string( (tDec*) &dTemp, szTemp) ;
         return fnCompare( (tDec*) pvDecimal, (tDec*) &dTemp );
      }

      case zDEC_CMD_MULTDEC:
         decari_mult( (tDec*) pvDecimal, (tDec*) pvInfo,
                      (tDec*) pvReturn );
         return( 0 );

      case zDEC_CMD_DIVDEC:
         decari_div( (tDec*) pvDecimal, (tDec*) pvInfo,
                      (tDec*) pvReturn );
         return( 0 );

      case zDEC_CMD_SUBDEC:
         decari_sub( (tDec*) pvDecimal, (tDec*) pvInfo,
                      (tDec*) pvReturn );
         return( 0 );

      case zDEC_CMD_ADDDEC:
         decari_add( (tDec*) pvDecimal, (tDec*) pvInfo,
                      (tDec*) pvReturn );
         return( 0 );

      case zDEC_CMD_COMPAREDEC:
         return fnCompare( (tDec*) pvDecimal, (tDec*) pvInfo );

      case zDEC_CMD_ROUND:
      {
         short nPrecision = * (short *) pvInfo;
         if( nPrecision >= 0 )
            decari_round( (tDec*)pvDecimal, nPrecision );

         return( 0 );
      }
   }

   return( -1 );
}
