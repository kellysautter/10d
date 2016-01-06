/*
** CHANGE LOG
**
** 2002.01.29  HH
**    implemented zDEC_CMD_ROUND.
** 1998.07.24  RG
**    Several changes from Quinsoft without change log
**
** 1998.12.02  DGC
**    Changed compare to ignore very small differences.
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "kzoeenaa.h"
#include "zdecimal.h"

#ifdef __WIN32__ && defined( _MSC_VER )
   #define zatold       atof
#else
   #define zatold       _atold
#endif

#define LDP long double *

void OPERATION
TraceLineS( zCPCHAR Title, zCPCHAR String );
void OPERATION
TraceLineI( zCPCHAR Title, zLONG l );

long double g_dNull = -99999999999999.9;

#ifdef zDECIMAL_BOUNDARY

zSHORT OPERATION
zDecimalOperation( void  *pvReturn,
                   void  *pvDecimal,
                   void  *pvInfo,
                   long  lControl )
{
   long double dReturn;
   long double dDecimal;
   long double dInfo;
   long        lCommand = lControl & zDEC_CMD;

   switch ( lCommand )
   {
      case zDEC_CMD_ASSIGNDEC:
         // Copy from 1 decimal to another.
         memcpy( pvReturn, pvDecimal, sizeof( long double ) );
         return( 0 )

      case zDEC_CMD_ASSIGNNULL:
         // Assign NULL to a decimal.
         memcpy( pvReturn, &g_dNull, sizeof( long double ) );
         return( 0 )

      case zDEC_CMD_DECTOSTRING:
      {
         short nPrecision = * (short *) pvInfo;

         // Convert double to string.
         memcpy( &dDecimal, pvDecimal, sizeof( long double ) );
         if ( nPrecision > 0 )
            sprintf( (char *) pvReturn, "%.*Lf", nPrecision, dDecimal );
         else
            sprintf( (char *) pvReturn, "%.0Lf", dDecimal );

         // sprintf is defined to return a empty string if dDecimal is 0.0.
         if ( * (char *) pvReturn == 0 )
            strcpy( (char *) pvReturn, "0.0" );

         return( 0 )
      }

      case zDEC_CMD_STRINGTODEC:
         // Convert string to double.
         dReturn = zatold( (char *) pvInfo );
         memcpy( pvReturn, &dReturn, sizeof( long double ) );
         return( 0 )

      case zDEC_CMD_DECTOLONG:
         // Convert double to long.
         memcpy( &dDecimal, pvDecimal, sizeof( long double ) );
         * (long *) pvReturn = (zLONG) dDecimal;
         return( 0 )

      case zDEC_CMD_LONGTODEC:
         dReturn = (long double) * (long *) pvInfo;
         memcpy( pvReturn, &dReturn, sizeof( long double ) );
         return( 0 )

      case zDEC_CMD_DECTODOUBLE:
         // Convert long double to double.
         memcpy( &dDecimal, pvDecimal, sizeof( long double ) );
         * (double *) pvReturn = (double) dDecimal;
         return( 0 )

      case zDEC_CMD_DOUBLETODEC:
         // Convert double to long double.
         dReturn = (long double) * (double *) pvInfo;
         memcpy( pvReturn, &dReturn, sizeof( long double ) );
         return( 0 )

      case zDEC_CMD_COMPARENULL:
         memcpy( &dDecimal, pvDecimal, sizeof( long double ) );
         if ( dDecimal == g_dNull )
            return( 0 )
         else
            return( 1 )

      case zDEC_CMD_COMPAREDOUBLE:
      {
         double d = * (double *) pvInfo;

         memcpy( &dDecimal, pvDecimal, sizeof( long double ) );
         if ( dDecimal < (long double) d )
            return( -1 )
         else
         if ( dDecimal > (long double) d )
            return( 1 )
         else
            return( 0 )
      }

      case zDEC_CMD_MULTDEC:
      case zDEC_CMD_DIVDEC:
      case zDEC_CMD_SUBDEC:
      case zDEC_CMD_ADDDEC:
      case zDEC_CMD_COMPAREDEC:
         memcpy( &dDecimal, pvDecimal, sizeof( long double ) );
         memcpy( &dInfo, pvInfo, sizeof( long double ) );

         switch ( lCommand )
         {
            case zDEC_CMD_MULTDEC:
               dReturn = dDecimal * dInfo;
               break;

            case zDEC_CMD_DIVDEC:
               dReturn = dDecimal / dInfo;
               break;

            case zDEC_CMD_SUBDEC:
               dReturn = dDecimal - dInfo;
               break;

            case zDEC_CMD_ADDDEC:
               dReturn = dDecimal + dInfo;
               break;

            case zDEC_CMD_COMPAREDEC:
               if ( dDecimal < dInfo )
                  return( -1 )
               else
               if ( dDecimal > dInfo )
                  return( 1 )
               else
                  return( 0 )
         }

         memcpy( pvReturn, &dReturn, sizeof( long double ) );
         return( 0 )

      case zDEC_CMD_ROUND:
      {
         zSHORT nPrecision = *(zSHORT*)pvInfo;
         int iSign = 1;
         zSHORT n;
         long double d, d2;

         if ( nPrecision < 0 )
            return( 0 )

         d = *(long double*)pvDecimal;
         if ( d == 0.0 )
            return( 0 )

         if ( d < 0.0 ) {
            d = -d;
            iSign = -1;
         }

         for ( n = 0; n < nPrecision; n++ )
            d = d*10;
         d2 = floor( d );
         if ( d - d2 >= 0.5 )
            d2 += 1.0;
         for ( n = 0; n < nPrecision; n++ )
            d2 = d2/10;
         d2 = d2 * iSign;
         *(long double*)pvDecimal = d2;

         return( 0 )
      }

   }

   return( -1 )
}

#else

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
         * (LDP) pvReturn = * (LDP) pvDecimal;
         return( 0 );

      case zDEC_CMD_ASSIGNNULL:
         // Assign NULL to a decimal.
         * (LDP) pvReturn = g_dNull;
         return( 0 );

      case zDEC_CMD_COMPAREDEC:
      {
         long double ldDifference;

         ldDifference = * (LDP) pvDecimal - * (LDP) pvInfo;

         if ( ldDifference < -0.0000000001 )
            return( -1 );
         else
         if ( ldDifference >  0.0000000001 )
            return( 1 );
         else
            return( 0 );
      }

      case zDEC_CMD_COMPARENULL:
         if ( * (LDP) pvDecimal == g_dNull )
            return( 0 );
         else
            return( 1 );

      case zDEC_CMD_COMPAREDOUBLE:
         if ( * (LDP) pvDecimal < (long double) * (double *) pvInfo )
            return( -1 );
         else
         if ( * (LDP) pvDecimal > (long double) * (double *) pvInfo )
            return( 1 );
         else
            return( 0 );

      case zDEC_CMD_STRINGTODEC:
         // Convert string to double.
         * (LDP) pvReturn = zatold( (char *) pvInfo );
         return( 0 );

      case zDEC_CMD_DECTOSTRING:
      {
         short nPrecision = * (short *) pvInfo;

         // Convert double to string.

         if ( nPrecision > 0 )
            sprintf( (char *) pvReturn, "%.*Lf", nPrecision, * (LDP) pvDecimal );
         else
            sprintf( (char *) pvReturn, "%Lf", * (LDP) pvDecimal );

         // sprintf is defined to return a empty string if dDecimal is 0.0.
         if ( * (char *) pvReturn == 0 )
            strcpy( (char *) pvReturn, "0.0" );

         return( 0 );
      }

      case zDEC_CMD_DECTOLONG:
         // Convert double to long.
         * (long *) pvReturn = (zLONG) * (LDP) pvDecimal;
         return( 0 );

      case zDEC_CMD_LONGTODEC:
         * (LDP) pvReturn = (long double) * (long *) pvInfo;
         return( 0 );

      case zDEC_CMD_DECTODOUBLE:
         // Convert long double to double.
         * (double *) pvReturn = (double) * (LDP) pvDecimal;
         return( 0 );

      case zDEC_CMD_DOUBLETODEC:
         // Convert double to long double.
         * (LDP) pvReturn = (long double) * (double *) pvInfo;
         return( 0 );

      case zDEC_CMD_MULTDEC:
         * (LDP) pvReturn = * (LDP) pvDecimal * * (LDP) pvInfo;
         return( 0 );

      case zDEC_CMD_DIVDEC:
         * (LDP) pvReturn = * (LDP) pvDecimal / * (LDP) pvInfo;
         return( 0 );

      case zDEC_CMD_SUBDEC:
         * (LDP) pvReturn = * (LDP) pvDecimal - * (LDP) pvInfo;
         return( 0 );

      case zDEC_CMD_ADDDEC:
         * (LDP) pvReturn = * (LDP) pvDecimal + * (LDP) pvInfo;
         return( 0 );
   }

   return( -1 );
}

#endif

BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved )
{
   switch( dwReason )
   {
      case DLL_PROCESS_ATTACH:
         TraceLineS( "(ZDecimal) ",
                     "Using 'long double' version of ZDecimal.DLL version 1.0" );
         TraceLineI( "(ZDecimal) Precision (in bits) = ",
                     (zLONG) ( sizeof( long double ) * 8 ) );
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

