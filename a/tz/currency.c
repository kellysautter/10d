/*
//----------------------------------------------------------------------
//
// .Name:         DomainC.c
//
// .Version:      1.0
//
// .Last change:  04-03-1999
//
// .Description:  Zeidon operations
//
//----------------------------------------------------------------------
// .Change notes
//
//  1.0  = New
//        (04-03-1999) Zeidon
//
//----------------------------------------------------------------------
*/

#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <ZDRVROPR.H>

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZEIDONOP.H"
//#include "tzlodopr.h"
#include "tzapdmaa.hg"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define zMAX_DECIMAL_STRING     256   // 128 is too short
// const ZDecimal dInvalidDecimalValue( "99999999999999999999999999999" );
#define zPLACE_ONES             0
#define zPLACE_THOUSANDS        1
#define zPLACE_MILLIONS         2
#define zPLACE_BILLIONS         3
#define zPLACE_TRILLIONS        4

#define IDS_ONES_0              100
#define IDS_ONES_1              101
#define IDS_ONES_2              102
#define IDS_ONES_3              103
#define IDS_ONES_4              104
#define IDS_ONES_5              105
#define IDS_ONES_6              106
#define IDS_ONES_7              107
#define IDS_ONES_8              108
#define IDS_ONES_9              109
#define IDS_TEENS_10            110
#define IDS_TEENS_11            111
#define IDS_TEENS_12            112
#define IDS_TEENS_13            113
#define IDS_TEENS_14            114
#define IDS_TEENS_15            115
#define IDS_TEENS_16            116
#define IDS_TEENS_17            117
#define IDS_TEENS_18            118
#define IDS_TEENS_19            119
#define IDS_TENS_0              120
#define IDS_TENS_10             121
#define IDS_TENS_20             122
#define IDS_TENS_30             123
#define IDS_TENS_40             124
#define IDS_TENS_50             125
#define IDS_TENS_60             126
#define IDS_TENS_70             127
#define IDS_TENS_80             128
#define IDS_TENS_90             129

#define IDS_DECIMAL             150
#define IDS_COMMA               151

#define IDS_THOUSAND            190
#define IDS_MILLION             191
#define IDS_BILLION             192
#define IDS_TRILLION            193

#define IDS_AND                 196
#define IDS_HUNDRETHS_DOLLARS   197
#define IDS_HUNDRED             198

#define IDS_UNKNOWN             199

//./ ADD NAME=fnFormatDecimalString
// Source Module=tzapdmac.c
////////////////////////////////////////////////
//
//  ENTRY:      fnFormatDecimalString
//
//  PURPOSE:    Decorate a decimal string with 'commas' and 'periods'
//
//  PARAMETERS:
//              pchDecString - in/out decimal string
//              pchThou     - points to the thousands character separator
//              pchDecPt    - points to the decimal point character
//              lPrecision - desired number of characters to the right
//                           of the decimal point.  Note that -1 indicates
//                           variable precision.
//
//  RETURNS:
//
//              0 - Always this for now...
//
////////////////////////////////////////////////
//./ END + 5
zSHORT
fnFormatDecimalString( zPCHAR pchDecString,
                       zPCHAR pchThou,
                       zPCHAR pchDecPt,
                       zLONG  lPrecision )
{
   zPCHAR psz;
   zPCHAR psz2;
   zLONG  l;

   // Determine size of input string.
   l = zstrlen( pchDecString );

   // Locate the decimal point.
   psz = zstrchr( pchDecString, '.' );
   if ( psz )
   {
      *psz = *pchDecPt;     // decimal point, as the user likes it

      // A negative lPrecision value means variable decimal placement.
      if ( lPrecision >= 0 )
      {
         zLONG los; // desired length of string for precision

         los = (zLONG) (psz - pchDecString) +
                                          (lPrecision ? 1 : 0) + lPrecision;
         if ( los > l )
         {
            psz2 = (zPCHAR) (pchDecString + l);
            while ( los > l )
            {
               *psz2++ = '0';    // pad with trailing zeros
               l++;              // adjust string length
            }

            *psz2 = 0;           // null terminator for string
         }
         else
         {
            // This is for rounding ...
            if ( l > los)
            {
               psz2 = pchDecString;

               // Does the string need to be rounded?
               if ( *(psz2 + los + (lPrecision == 0 ? 1 : 0)) > '4' )
               {
                  l = los - 1;
                  while ( l != 0 &&
                          *(psz2 + l) == '9' || *(psz2 + l) == *pchDecPt )
                  {
                     l--;
                  }

                  if ( *(psz2 + l) == '-' )
                     l++;

                  // If the string is all 9's, add a '1' at the beginning of
                  // the string and propagate the rest of the string with 0's.
                  if ( *(psz2 + l) == '9' )
                  {
                     *(psz2 + l++) = '1';
                     while ( *(psz2 + l) != *pchDecPt && *(psz2 + l) )
                        *(psz2 + l++) = '0';

                     *(psz2 + l++) = '0';
                     // If we are at the decimal point and the attribute
                     // is not defined as whole number, add a ".0".
                     if ( lPrecision )
                     {
                        psz = psz2; // position of decimal point
                        *(psz2 + l++) = *pchDecPt;
                        *(psz2 + l++) = '0';
                        while ( l <= los )
                           *(psz2 + l++) = '0';
                     }
                     los = l;
                  }
                  else
                  {
                     // The string has a digit which requires rounding up,
                     // Round up the digit and change subsequent digits
                     // within the precision (if any) from '9' to '0'.
                     (*(psz2 + l++))++;
                     while ( l < los )
                     {
                        if ( *(psz2 + l) == *pchDecPt )
                        {
                           // if we are at the decimal point and the attribute
                           // is defined as whole number, quit
                           if ( lPrecision == 0 )
                              break;
                        }
                        else
                           *(psz2 + l) = '0';

                        l++;
                     }

                     los = l;
                  }
               }
            }

            *(psz + (lPrecision ? 1 : 0) + lPrecision) = 0; // truncate string
            l = los;             // adjust string length
         }
      }
   }
   else
   {
      // There is no decimal point in string.
      if ( lPrecision > 0 )
      {
         zLONG los; // desired length of string for precision

         /* but we want decimal fraction to show ... */
         psz = (pchDecString + l++); // point to terminator, bump str length
         *psz = *pchDecPt;        // append decimal pt
         los = (zLONG) (psz - pchDecString) + 1 + lPrecision;
         psz2 = (zPCHAR) (pchDecString + l);
         while ( los > l )
         {
            *psz2++ = '0';    // pad with trailing zeros
            l++;              // adjust string length
         }

         *psz2 = 0;           // null terminator for string.
      }
      else
         psz = pchDecString + l;  // point to string end
   }

   // psz is now pointing to decimal point or string end.

   // If there is a thousands character, add it.
   if ( pchThou != 0 )
   {
      while ( psz - pchDecString > 3 )
      {
         zPCHAR psz3;
         psz -= 3;
         if ( isdigit( *(psz - 1) ) )
         {
            /* shuffle down ... point at terminator ... bump string length */
            psz2 = (zPCHAR) (pchDecString + l++);
            psz3 = psz2 + 1;
            while ( psz3 > psz )
            {
               *psz3-- = *psz2--; // move to the right
            }

            *psz = *pchThou;       // insert thousands separator
         }
         else
            break;
      }
   }

   return( 0 );
}

//./ ADD NAME=fnGetDecimalFmtStrings
// Source Module=tzapdmac.c
////////////////////////////////////////////////
//
//  ENTRY:      fnGetDecimalFmtStrings
//
//  PURPOSE:    Determine values necessary for decorating decimal
//              strings.
//
//  PARAMETERS:
//              lpContext     - Context control structure.
//              szReturnThou  - return for a string representing the
//                              thousands character separator.
//              szReturnDecPt - return for a string representing the
//                              decimal point character.
//              lplPrecision  - desired number of characters to the right
//                              of the decimal point.  A -1 indicates
//                              variable precision.
//
//  RETURNS:
//              0 - Information has been returned.
//    zCALL_ERROR - Only returned for context "MSWindowsSetting", when we
//                  could not retrieve the International information from
//                  windows.  In that case we return 'US' defaults.
//
////////////////////////////////////////////////
//./ END + 5
zSHORT
fnGetDecimalFmtStrings( zLPCONTEXT  lpContext,
                        zPCHAR      szReturnThou,
                        zPCHAR      szReturnDecPt,
                        zPLONG      lplPrecision )
{
   zPCHAR   pszName;    // context name

   // First check for Windows setting.  If so, use all international
   // settings, including precision.
   if ( zstrcmp( pszName, "MSWindowsSetting" ) == 0 )
      SysGetNumberFormat( szReturnThou, szReturnDecPt, lplPrecision );
   else
   if ( zstrcmp( pszName, "CurrencyText" ) == 0 )
   {
      SysGetNumberFormat( szReturnThou, szReturnDecPt, lplPrecision );
      *lplPrecision = 2;
   }
   else
   if ( zstrncmp( pszName, "DECIMAL", 7 ) == 0 )  // check for DECIMALx
   {
      // Use the international settings for szReturnThou and szReturnDecPt,
      // but use the x value, if it exists, for lplPrecision.
      SysGetNumberFormat( szReturnThou, szReturnDecPt, lplPrecision );
      if ( pszName[ 7 ] == '4' || pszName[ 7 ] == '3' ||
           pszName[ 7 ] == '2' || pszName[ 7 ] == '1' ||
           pszName[ 7 ] == '0' )
      {
         *lplPrecision = atol( pszName + 7 );
      }
      else
         *lplPrecision = lpContext->lDecimalFormat;
   }
   else  // check for DecimalCommax or DecimalPointx
   if ( zstrncmp( pszName, "DecimalPoint", 12 ) == 0 )
   {
      // Use the comma or point as appropriate.  Use the x value, if it
      // exists, for lplPrecision.
      strcpy_s( szReturnThou, sizeof( szReturnThou ), "," );
      strcpy_s( szReturnDecPt, sizeof( szReturnDecPt ), "." );
      if ( pszName[ 12 ] == '4' || pszName[ 12 ] == '3' ||
           pszName[ 12 ] == '2' || pszName[ 12 ] == '1' ||
           pszName[ 12 ] == '0' )
      {
         *lplPrecision = atol( pszName + 12 );
      }
      else
         *lplPrecision = lpContext->lDecimalFormat;
   }
   else
   if ( zstrncmp( pszName, "DecimalComma", 12 ) == 0 )
   {
      strcpy_s( szReturnThou, sizeof( szReturnThou ), "." );
      strcpy_s( szReturnDecPt, sizeof( szReturnDecPt ), "," );
      if ( pszName[ 12 ] == '4' || pszName[ 12 ] == '3' ||
           pszName[ 12 ] == '2' || pszName[ 12 ] == '1' ||
           pszName[ 12 ] == '0' )
      {
         *lplPrecision = atol( pszName + 12 );
      }
      else
         *lplPrecision = lpContext->lDecimalFormat;
   }
   else  // if it is anything else
   {
      // Use the international settings for szReturnThou and szReturnDecPt,
      // but use the context value for lplPrecision.
      SysGetNumberFormat( szReturnThou, szReturnDecPt, lplPrecision );
      *lplPrecision = lpContext->lDecimalFormat;
   }

   return( 0 );
}

zSHORT
fnLoadString( zVIEW   vSubtask,
              zCPCHAR cpcModuleName,
              zULONG  ulID,
              zPCHAR  pchReturnString,
              zSHORT  nBufferMax )
{
   LPLIBRARY hLibrary;

   if ( cpcModuleName && *cpcModuleName )   // DLL_Name
   {
      hLibrary = SysLoadLibraryWithErrFlag( vSubtask, cpcModuleName,
                                            zLOADLIB_RESOURCES );
      if ( hLibrary )
      {
         zSHORT nLanguageCode;

         HINSTANCE hInstDLL = (HINSTANCE) SysGetInstance( hLibrary );
         SysGetLanguageCode( &nLanguageCode );
         return( LoadString( hInstDLL, ulID + (nLanguageCode * 100) - 100,
                             pchReturnString, nBufferMax ) );
      }
   }

   return( -1 );
}

zSHORT
TensOnes( zVIEW    vSubtask,
          zPCHAR   pchReturnTensOnes,
          zCPCHAR  cpcTensOnes,
          zSHORT   nPower )
{
   zSHORT nLth = 0;

   if ( nPower == 2 )
   {
      if ( *cpcTensOnes > '1' )
      {
         nPower--;
         nLth += fnLoadString( vSubtask,
                               "domainc",
                               IDS_TENS_0 + *cpcTensOnes - '0',
                               pchReturnTensOnes + nLth,
                               zMAX_DECIMAL_STRING - nLth );
         cpcTensOnes++;
         if ( *cpcTensOnes > '0' )
         {
            pchReturnTensOnes[ nLth++ ] = '-';
            nLth += fnLoadString( vSubtask,
                                  "domainc",
                                  IDS_ONES_0 + *cpcTensOnes - '0',
                                  pchReturnTensOnes + nLth,
                                  zMAX_DECIMAL_STRING - nLth );
         }

         nPower--;
         cpcTensOnes++;
      }
      else
      if ( *cpcTensOnes == '1' )
      {
         nPower--;
         cpcTensOnes++;
         nLth += fnLoadString( vSubtask,
                               "domainc",
                               IDS_TEENS_10 + *cpcTensOnes - '0',
                               pchReturnTensOnes + nLth,
                               zMAX_DECIMAL_STRING - nLth );

         nPower--;
         cpcTensOnes++;
      }
      else
      if ( *cpcTensOnes == '0' )
      {
         nPower--;
         cpcTensOnes++;
         nLth += fnLoadString( vSubtask,
                               "domainc",
                               IDS_ONES_0 + *cpcTensOnes - '0',
                               pchReturnTensOnes + nLth,
                               zMAX_DECIMAL_STRING - nLth );

         nPower--;
         cpcTensOnes++;
      }
   }
   else
   if ( nPower == 1 )
   {
      nLth += fnLoadString( vSubtask,
                            "domainc",
                            IDS_ONES_0 + *cpcTensOnes - '0',
                            pchReturnTensOnes + nLth,
                            zMAX_DECIMAL_STRING - nLth );
   }

   if ( nLth )
      pchReturnTensOnes[ nLth++ ] = ' ';

   return( nLth );
}

zSHORT
HundredsTensOnes( zVIEW    vSubtask,
                  zPCHAR   pchReturnHundredsTensOnes,
                  zCPCHAR  cpcHundredsTensOnes,
                  zULONG   lResourceId,
                  zSHORT   nPower )
{
   zSHORT nLth = 0;

   if ( nPower == 3 )
   {
      nLth += fnLoadString( vSubtask,
                            "domainc",
                            IDS_ONES_0 + *cpcHundredsTensOnes - '0',
                            pchReturnHundredsTensOnes + nLth,
                            zMAX_DECIMAL_STRING - nLth );
      pchReturnHundredsTensOnes[ nLth++ ] = ' ';
      nLth += fnLoadString( vSubtask,
                            "domainc", IDS_HUNDRED,
                            pchReturnHundredsTensOnes + nLth,
                            zMAX_DECIMAL_STRING - nLth );
      pchReturnHundredsTensOnes[ nLth++ ] = ' ';
      cpcHundredsTensOnes++;
      nPower--;
      while ( nPower && *cpcHundredsTensOnes == '0' )
      {
         nPower--;
         cpcHundredsTensOnes++;
      }

   }

   if ( nPower )
      nLth += TensOnes( vSubtask, pchReturnHundredsTensOnes + nLth,
                        cpcHundredsTensOnes, nPower );

   if ( nLth && lResourceId )
   {
      nLth += fnLoadString( vSubtask,
                            "domainc", lResourceId,
                            pchReturnHundredsTensOnes + nLth,
                            zMAX_DECIMAL_STRING - nLth );
      pchReturnHundredsTensOnes[ nLth++ ] = ' ';
   }

   return( nLth );
}

zSHORT
fnFormatCurrencyText( zVIEW  vSubtask,
                      zPCHAR pchDecString,
                      zPCHAR pchThou,
                      zPCHAR pchDecPt,
                      zLONG  lPrecision )
{
   char   szCurrencyText[ zMAX_DECIMAL_STRING ];
   zPCHAR pcDecimal;
   zPCHAR pch;
   zSHORT nLth;
   zSHORT nCurrencyLth;
   zSHORT nPlace;
   zSHORT nPower;

   // Check if there is a decimal point
   pcDecimal = zstrchr( pchDecString, '.' );
   if ( pcDecimal )
   {
      *pcDecimal = 0;
      pcDecimal++;
   }
   else
   {
      pcDecimal = pchDecString + zstrlen( pchDecString );
   }

   // Remove leading blanks or zeros.
   pch = pchDecString;
   while ( *pch == ' ' || *pch == '0' )
      pch++;

   // Handle case of no nonzero value.
   nLth = zstrlen( pch );
   if ( nLth == 0 )
   {
      strcpy_s( szCurrencyText, sizeof( szCurrencyText ), pcDecimal );
      zstrcpy( pchDecString, "0" );
      zstrcpy( pchDecString + 2, szCurrencyText );
      pch = pchDecString;
      pcDecimal = pchDecString + 2;
      nLth = 1;
   }

   // Leading zeros have been eliminated except for the essential one in
   // the one's place.

   // Process whole number part of decimal.
   szCurrencyText[ 0 ] = 0;
   nCurrencyLth = 0;
   while ( nLth )
   {
      nPlace = (nLth - 1) / 3;
      nPower = nLth % 3;    // Hundreds = 3, Tens = 2, Ones = 1
      if ( nPower == 0 )
         nPower = 3;

      switch ( nPlace )
      {
         case zPLACE_TRILLIONS:
         {
            nCurrencyLth += HundredsTensOnes( vSubtask,
                                              szCurrencyText + nCurrencyLth,
                                              pch, IDS_TRILLION, nPower );
            break;
         }

         case zPLACE_BILLIONS:
         {
            nCurrencyLth += HundredsTensOnes( vSubtask,
                                              szCurrencyText + nCurrencyLth,
                                              pch, IDS_BILLION, nPower );
            break;
         }

         case zPLACE_MILLIONS:
         {
            nCurrencyLth += HundredsTensOnes( vSubtask,
                                              szCurrencyText + nCurrencyLth,
                                              pch, IDS_MILLION, nPower );
            break;
         }

         case zPLACE_THOUSANDS:
         {
            nCurrencyLth += HundredsTensOnes( vSubtask,
                                              szCurrencyText + nCurrencyLth,
                                              pch, IDS_THOUSAND, nPower );
            break;
         }

         case zPLACE_ONES:
         {
            nCurrencyLth += HundredsTensOnes( vSubtask,
                                              szCurrencyText + nCurrencyLth,
                                              pch, 0, nPower );
            break;
         }

         default:
         {
            fnFormatDecimalString( pchDecString, pchThou,
                                   pchDecPt, lPrecision );
            return( 1 );
         }
      }

      pch += nPower;
      nLth -= nPower;
      while ( *pch == '0' )
      {
         pch++;
         nLth--;
      }
   }

   // Add "AND" between whole number & decimal part.
   nCurrencyLth += fnLoadString( vSubtask,
                                 "domainc", IDS_AND,
                                 szCurrencyText + nCurrencyLth,
                                 zMAX_DECIMAL_STRING - nCurrencyLth );
   szCurrencyText[ nCurrencyLth++ ] = ' ';


   // Process decimal part of number.

   // If the decimal portion is not filled with digits, fill it with zeros.
   if ( *pcDecimal == 0 )
   {
      *pcDecimal = '0';
      *(pcDecimal + 1) = '0';
   }
   else
   if ( *(pcDecimal + 1) == 0 )
   {
      *(pcDecimal + 1) = '0';
   }

   *(pcDecimal + 2) = 0;
   nCurrencyLth += TensOnes( vSubtask, szCurrencyText + nCurrencyLth,
                             pcDecimal, 2 );
   nCurrencyLth += fnLoadString( vSubtask,
                                 "domainc", IDS_HUNDRETHS_DOLLARS,
                                 szCurrencyText + nCurrencyLth,
                                 zMAX_DECIMAL_STRING - nCurrencyLth );

   zstrcpy( pchDecString, szCurrencyText );
   return( 0 );
}


zSHORT
fnGetCurrencyText( LPDOMAINDATA lpDomainData )
{
   LPDOMAIN    lpDomain;
   zLPCONTEXT  lpContext;
   zSHORT      nRC;
   zCHAR       sz[ 200 ];
   zPCHAR      lpAttrValue;
   zCHAR       cAttrType;
   zULONG      uAttrLength;
   zBOOL       bWasNull = FALSE;
   zCHAR       szThou[ 10 ];
   zCHAR       szDecPt[ 10 ];
   zLONG       lPrecision;
   zDECIMAL    dDecimal;
   zDECIMAL    dNullDecimal;

   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );
   SysAssignDecimalFromNull( &dNullDecimal );

   if ( lpDomainData->lpViewAttribute->cType != zTYPE_DECIMAL )
   {
      *sz = lpDomainData->lpViewAttribute->cType;
      *(sz + 1) = 0;
      strcat_s( sz, sizeof( sz ), ", " );
      strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
      MessageSend( lpDomainData->zView, "", "Domain System Error",
                   sz, zMSGQ_DOMAIN_ERROR, 0 );
      return( zCALL_ERROR );
   }

   // determine output data type
   switch ( lpDomainData->cType )
   {
    case zTYPE_STRING:

      // Get a pointer to the attribute value.
      nRC = GetValueFromRecord( lpDomainData->zView,
                                lpDomainData->lpViewEntity,
                                lpDomainData->lpViewAttribute,
                                &lpAttrValue, &cAttrType, &uAttrLength );
      if ( nRC )
         return( zCALL_ERROR );

      // if there is no lpRecord, lpAttrValue returns a null...
      if ( lpAttrValue == 0 )
         lpAttrValue = (zPCHAR) &dNullDecimal;

      // Get the decimal characteristics.
      if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
      {
         if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) )
         {
            // Get the formatting characters.
            nRC = fnGetDecimalFmtStrings( lpContext, szThou, szDecPt,
                                          &lPrecision );


         }
         else
         {
            MessageSend( lpDomainData->zView, "", "Domain System Error",
                         "Invalid Context for Currency Text",
                         zMSGQ_DOMAIN_ERROR, 0 );
            return( zCALL_ERROR );
         }
      }
      else
      {
         GetDefaultContext( &lpContext, lpDomain );
         fnGetDecimalFmtStrings( lpContext, szThou, szDecPt, &lPrecision );
      }

      // Convert the decimal to a basic string.
      if ( SysCompareDecimalToNull( (zPDECIMAL) lpAttrValue ) == 0 )
      {
         *((zPCHAR) lpDomainData->lpData) = 0;
         strcpy_s( sz, sizeof( sz ), "0.0" );
         bWasNull = TRUE;
      }
      else
      {
         SysAssignDecimalFromDecimal( &dDecimal, (zPDECIMAL) lpAttrValue );
         SysConvertDecimalToString( &dDecimal, sz, (zSHORT) lPrecision );
      }

      // Convert the decimal string to correct precision and international
      // formatting characters.
      if ( szThou[ 0 ] == 0 )
      {
         if ( szDecPt[ 0 ] == 0 )
            fnFormatDecimalString( sz, 0, 0, lpDomain->lDecimalFormat );
         else
            fnFormatDecimalString( sz, 0, szDecPt, lPrecision );
      }
      // Convert the string decimal to the character string.
      fnFormatCurrencyText( lpDomainData->zView, sz,
                            szThou, szDecPt, lPrecision );
      // Return the formatted decimal, unless the return area is not
      // large enough, in that case fill it with '*'.
      if ( lpDomainData->uMaxStringLength && zstrlen( sz ) > lpDomainData->uMaxStringLength )
      {
         zmemset( (zPCHAR) lpDomainData->lpData, '*', lpDomainData->uMaxStringLength );
         *((zPCHAR) lpDomainData->lpData + lpDomainData->uMaxStringLength) = 0;
         nRC = zCALL_ERROR;
      }
      else
      {
         strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, sz );
         nRC = bWasNull ? -1 : 0;
      }

      break;


    default:
      *sz = lpDomainData->cType;
      *(sz + 1) = 0;
      strcat_s( sz, sizeof( sz ), ", " );
      strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
      MessageSend( lpDomainData->zView, "", "Domain System Error",
                   sz, zMSGQ_DOMAIN_ERROR, 0 );
      return( zCALL_ERROR );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: RevenueText
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
RevenueText( zLONG        lEntryType,
            LPDOMAINDATA lpDomainData )
{
   zSHORT      nRC;
   LPDOMAIN    lpDomain;

   nRC = zCALL_ERROR;         // Default to in error status...
   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   switch ( lEntryType )
   {
      case zDME_GET_VARIABLE:
         nRC = fnGetCurrencyText( lpDomainData );
         break;

      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
      case zDME_ADD_TO_ATTRIBUTE:
      case zDME_COMPARE_ATTRIBUTE:
      case zDME_GET_COMPARE_VALUE:
      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
      default:
         // Entry not handled
         return( zDME_NOT_HANDLED );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: AlphaNumericUpperStrip
//
// Strip all non-alphanumerics and upper case all characters
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
AlphaNumericUpperStrip( zLONG lEntryType,
            LPDOMAINDATA lpDomainData )
{
   zSHORT         nRC;
   zSHORT         nWChar;
   zCHAR          szWorkString[ 512 ];
   zPCHAR         pszStringToStore;
   zPLONG         lplInteger;
   zLONG          lWork;
   LPDOMAIN       lpDomain;
   zCHAR          szNullS[ 2 ];
   zLONG          lNullInteger = -2147483647 - 1;

   nRC = zDME_NOT_HANDLED;         // Default to not-handled.
   szNullS[ 0 ] = 0;

   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            szWorkString[ 0 ] = 0;

            // determine input data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  // string to string
                  pszStringToStore = (zPCHAR) lpDomainData->lpData;
                  if ( lpDomainData->lpData == 0 )
                     pszStringToStore = szWorkString;

                  break;

               case zTYPE_INTEGER:
                  // convert long to a string
                  lplInteger = (zPLONG) lpDomainData->lpData;
                  if ( *lplInteger != lNullInteger )
                     zltoa( *lplInteger, szWorkString );

                  pszStringToStore = szWorkString;
                  break;

               case zTYPE_DATETIME:
                  // convert DateTime to String
                  nRC = UfDateTimeToString( (LPDATETIME) lpDomainData->lpData,
                                            szWorkString, 18 );
                  if ( nRC == zCALL_ERROR )
                     return( nRC );

                  pszStringToStore = szWorkString;
                  break;

               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, sizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, sizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0,
                                   szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            // transfer all alphanumeric characters
            zPCHAR  psz;
            nWChar = 0;

            for ( psz = (zPCHAR) lpDomainData->lpData; *psz; psz++ )
            {
               if ( zisalnum( *psz ) )
               {
                  szWorkString[ nWChar++ ] = ztoupper( *psz );
               }
            }

            szWorkString[ nWChar ] = 0;

            // finally store it
            if ( lEntryType == zDME_SET_ATTRIBUTE )
            {
               nRC = StoreValueInRecord( lpDomainData->zView,
                                         lpDomainData->lpViewEntity,
                                         lpDomainData->lpViewAttribute,
                                         szWorkString, 0 );
            }
            else
               nRC = 0;
         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, sizeof( szWorkString ), ", " );
            strcat_s( szWorkString, sizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString,
                             lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_VARIABLE:

         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            if ( lpDomainData->cType != zTYPE_PIC )
            {
               nRC = GetValueFromRecord( lpDomainData->zView,
                                         lpDomainData->lpViewEntity,
                                         lpDomainData->lpViewAttribute,
                                         &lpAttrAddr, &cAttrType,
                                         &uAttrLength );
               if ( nRC )
                  return( zCALL_ERROR );
            }

            // determine input data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  if ( (zPCHAR) lpAttrAddr == 0 ) //null string
                  {
                     *((zPCHAR) lpDomainData->lpData) = 0;
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     *((zPCHAR) lpDomainData->lpData ) = 0;
                     zstrncat( (zPCHAR) lpDomainData->lpData,
                               (zPCHAR) lpAttrAddr,
                               lpDomainData->uMaxStringLength );
                     nRC = 0;
                  }

                  break;

               case zTYPE_INTEGER:
                  // convert string to long
                  if ( (zPCHAR) lpAttrAddr == 0 ) //null string
                  {
                     *((zPLONG) lpDomainData->lpData) = 0;
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     strcpy_s( szWorkString, sizeof( szWorkString ), (zPCHAR) lpAttrAddr );
                     *((zPLONG) lpDomainData->lpData) = atol( szWorkString );
                     nRC = 0;
                  }

                  break;

               case zTYPE_PIC:
                  // return the picture string for the given context
                  {
                     zLPCONTEXT   lpContext;

                     if ( GetContext( &lpContext, lpDomain,
                                      lpDomainData->pszContextName ) )
                     {
                        zPCHAR lpEditString = (zPCHAR) SysGetPointerFromHandle( lpContext->hEditString );

                        if ( lpEditString && *lpEditString )
                           strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength,
                                    lpEditString );
                        else
                           *((zPCHAR) lpDomainData->lpData) = 0;

                        nRC = 0;
                     }
                     else
                     {
                        // "TZDME008 - Could not find context for Domain "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0,
                                         TrueName( lpDomainData->pszContextName, zSHOW_ZKEY ),
                                         lpDomainData->lpViewAttribute->szName );
                        nRC = zCALL_ERROR;
                     }
                  }

                  break;

               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, sizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, sizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                   0, szWorkString, lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            nRC = 0;
         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, sizeof( szWorkString ), ", " );
            strcat_s( szWorkString, sizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

      case zDME_COMPARE_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            if ( lpAttrAddr == 0 )
               lpAttrAddr = szNullS;

            // determine input data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  if ( lpDomainData->lpViewAttribute->bCaseSens )
                     nRC = zstrcmp( lpAttrAddr, (zPCHAR) lpDomainData->lpData );
                  else
                     nRC = zstrcmpi( lpAttrAddr, (zPCHAR) lpDomainData->lpData );

                  break;

               case zTYPE_INTEGER:
                  lWork = zatol( lpAttrAddr );
                  lWork -= *((zPLONG) lpDomainData->lpData);
                  nRC = (lWork == 0) ? 0 : (lWork < 0) ? -1 : 1;
                  break;

               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, sizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, sizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }
         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, sizeof( szWorkString ), ", " );
            strcat_s( szWorkString, sizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_COMPARE_VALUE:
         switch ( lpDomainData->cType )
         {
            case zTYPE_STRING:
               nRC = 0;
               break;

            case zTYPE_INTEGER:
               zltoa( *((zPLONG) lpDomainData->lpData), szWorkString );
               strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, szWorkString );
               nRC = 1; // indicate returning a string value
               break;

            default:
               szWorkString[ 0 ] = lpDomainData->cType;
               szWorkString[ 1 ] = 0;
               strcat_s( szWorkString, sizeof( szWorkString ), ", " );
               strcat_s( szWorkString, sizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

               // "TZDME001 - Invalid Input Data Type"
               SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0,
                                szWorkString, lpDomainData->lpViewAttribute->szName );
               nRC = -2;
         }

         break;

      case zDME_ADD_TO_ATTRIBUTE:
      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
      default:
         // Entry not handled
         return( zDME_NOT_HANDLED );
   }

   return( nRC );

} // AlphaNumericUpperStrip

#ifdef __cplusplus
}
#endif
