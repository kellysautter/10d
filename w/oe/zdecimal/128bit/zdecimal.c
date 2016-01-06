#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "kzoeenaa.h"
#include "zdecimal.h"
#include "ehead.h"

#define USP unsigned short *

void OPERATION
TraceLineS( zCPCHAR Title, zCPCHAR String );

zDECIMAL g_dNull;

void
fnDoubleToDecimal( double d, USP dDec )
{
   char sz[ 200 ];

   // First convert double to a string.
   sprintf( sz, "%f", d );

   // Convert the string to the extended decimal.
   asctoe( sz, dDec );
}

void
fnDecimalToDouble( USP dDec, double * pd )
{
   char sz[ 25 ];

   // First convert decimal to string.
   etoasc( dDec, sz, (int) 18 );

   // Convert the string to double.
   *pd = strtod( sz, 0 );
}

// currently hardwired to OE's KZOEDECA.CPP
zSHORT OPERATION
zDecimalOperation( void  *pvReturn,
                   void  *pvDecimal,
                   void  *pvInfo,
                   long  lControl )
{
   long  lCommand = lControl & zDEC_CMD;

   switch ( lCommand )
   {
      case zDEC_CMD_ASSIGNDEC:
         // Copy from 1 decimal to another.
         emov( (USP) pvDecimal, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_ASSIGNNULL:
         // Assign NULL to a decimal.
         emov( (USP) &g_dNull, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_COMPAREDEC:
         return ecmp( (USP) pvDecimal, (USP) pvInfo );

      case zDEC_CMD_COMPARENULL:
         return ecmp( (USP) pvDecimal, (USP) &g_dNull ) ? TRUE : FALSE;

      case zDEC_CMD_COMPAREDOUBLE:
      {
         zDECIMAL dDec;

         fnDoubleToDecimal( * (double *) pvInfo, (USP) &dDec );
         return ecmp( (USP) pvDecimal, (USP) &dDec );
      }

      case zDEC_CMD_STRINGTODEC:
         // Convert string to double.
         asctoe( (char *) pvInfo, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_DECTOSTRING:
      {
         short  nPrecision = * (short *) pvInfo;
         char * psz;
         char   szTempDec[ 200 ];
         char * pszReturn = szTempDec;
         short  n;

         if ( nPrecision < 0 )
            etoasc( (USP) pvDecimal, pszReturn, (int) 100 );
         else
            etoasc( (USP) pvDecimal, pszReturn, (int) nPrecision );

         // Remove any leading spaces.
         psz = pszReturn;
         while ( *psz == ' ' )
            psz++;

         // Shift over the original string to remove the spaces.
         if ( psz != pszReturn )
            memcpy( pszReturn, psz, strlen( psz ) + 1 );

         // Now convert the string from scientic notation to "normal".

         // Find the 'E'.
         psz = strchr( pszReturn, 'E' );
         if ( psz )
         {
            short nExponent = (short) atoi( psz + 1 );

            if ( nExponent > 150 || nExponent < -150 )
            {
               zstrcpy( (char *) pvReturn, pszReturn );
               return( 0 );
            }

            // Null out the exponent.
            *psz = 0;

            if ( nExponent > 0 )
            {
               // Find the '.'.
               psz = strchr( pszReturn, '.' );

               if ( psz )
               {
                  short i;

                  // Shift the period over.
                  for ( i = 0; i < nExponent; i++ )
                     psz[ i ] = psz[ i + 1 ];

                  psz[ i ] = '.';
               }
            }
            else
            if ( nExponent < 0 )
            {
               // If we get here then we need to add 0's after the decimal.

               short sNegative = 0;  // 1 if number is negative.

               // Add 1 to n because we're going to add a '0' before the
               // decimal point.
               n = nExponent * -1 + 1;

               // psz points to the null char.  Shift everything over 'n'
               // chars until we get to the beginning of the string.
               while ( psz >= pszReturn )
               {
                  // If we're about to copy a '.' don't do it.
                  if ( *psz == '.' )
                     n++;  // Incr n so that next numbers are copied OK.
                  else
                     psz[ n ] = *psz;

                  psz--;

                  // If we find a minus sign then stop copying digits.
                  if ( *psz == '-' )
                  {
                     sNegative = 1;
                     break;
                  }
               }

               // Set psz back to the beginning of the string.
               psz = pszReturn + sNegative;

               *psz++ = '0';
               *psz++ = '.';

               // Now insert the 0's.
               n -= 2;   // Subtact 2 because we've already added 2 chars.
               while ( n > 0 )
                  psz[ --n ] = '0';

               // Lastly make sure we don't have too much precision.
               if ( nPrecision >= 0 &&
                    (short) strlen( pszReturn ) > nPrecision + 2 + sNegative )
                  pszReturn[ nPrecision + 2 + sNegative ] = 0;

            } // if ( nExponent < 0 )...

         } // if ( psz )...

         if ( nPrecision < 0 )
         {
            // Go to the end of the string and back up until we find
            // a non-zero character.

            psz = &pszReturn[ strlen( pszReturn ) - 1 ];

            while ( psz > pszReturn && *psz == '0' )
               psz--;

            if ( *psz == '.' )
               *++psz = '0';

            psz[ 1 ] = 0;
         }

         zstrcpy( (char *) pvReturn, pszReturn );

         return( 0 );
      }

      case zDEC_CMD_DECTOLONG:
      {
         unsigned short sFrac;

         // Convert double to long.
         eifrac( (USP) pvDecimal, (long *) pvReturn, &sFrac );
         return( 0 );
      }

      case zDEC_CMD_LONGTODEC:
         ltoe( (long *) pvInfo, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_DECTODOUBLE:
         // Convert long double to double.
         fnDecimalToDouble( (USP) pvDecimal, (double *) pvReturn );
         return( 0 );

      case zDEC_CMD_DOUBLETODEC:
         // Convert double to long double.
         fnDoubleToDecimal( * (double *) pvInfo, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_MULTDEC:
         emul( (USP) pvInfo, (USP) pvDecimal, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_DIVDEC:
         ediv( (USP) pvInfo, (USP) pvDecimal, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_SUBDEC:
         esub( (USP) pvInfo, (USP) pvDecimal, (USP) pvReturn );
         return( 0 );

      case zDEC_CMD_ADDDEC:
         eadd( (USP) pvInfo, (USP) pvDecimal, (USP) pvReturn );
         return( 0 );
   }

   return( -1 );
}

BOOL
WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved )
{
   switch( dwReason )
   {
      case DLL_PROCESS_ATTACH:
         asctoe( "-99999999999999.9", (USP) &g_dNull );
         TraceLineS( "(ZDecimal) ",
                     "Using 128-bit version of ZDecimal.DLL version 1.0" );
         break;

      case DLL_THREAD_ATTACH:
         break;

      case DLL_THREAD_DETACH:
         break;

      case DLL_PROCESS_DETACH:
         break;
   }

   return( TRUE );
}
