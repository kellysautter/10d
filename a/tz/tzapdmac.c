/*
CHANGE LOG

2002.05.28  DGC
   Changed fnGetDecimalAttribute() to use the attributes precision when
   retrieving an attribute into an external value.

2001.10.09  HH
   corrected fnAddToDecimalAttribute (decimal + decimal NEVER worked).

2001.07.19  DGC
   Changed str-to-dec conversion to ignore trailing zeros after the decimal
   point.

2000.09.26  HH
   Improved NULL comparison.

2000.06.13  DGC  Z10
   Fixed bug in fnSetDecimalAttribute() to caused the source decimal to be
   overwritten.

1999.11.30  US   Z10
   Corrected the change from 1999.11.09 as DG used a non ANSI construct

1999.11.09  DC   Z10
   Modified fnSetDecimalAttribute for setting the attribute from a decimal
   value so that the precision defined in the Domain would be used to round
   the decimal value to the correct number of digits.

1999.06.24  DC   Z10
   Removed "CurrencyText" processing and include to tzlodopr.h.

1999.04.06  DGC  Z10
   Changes for new attribute domain interface.

1999.03.10  DKS
   Added Format Currency routines to decimal domain.

97/04/16    DonC
   Added code to handle Contexts whose names start with DecimalKey so
   that they do not return string values with thousands character
   delimitors.  (ie., 1234567.12 is returned instead of 1,234,567.12 and
   1234567,12 is returned instead of 1.234.567,12)

97/04/19    DonC
   Corrected range edit so that no blank was necessary between operator
   and value.

97/07/15    HH
   Corrected inserting thousand-point ine fnFormatDecimalString

15.10.1997  HH
   include kzoeinco.h for 9I.

1997.10.31  DGC
   Removed kzoeinco.h for 9I.
   Changed fnStrToDec to try the default context if the supplied context
   doesn't work.

14.11.1997  DonC
   Modified fnEditDecimalFormatString to give error if thousand zCHAR or
   decimal point appear after the first decimal point.
   Modified fnStrToDec to give error if the number of characters after the
   decimal point is greater than that specified in the Context or Domain.

19.11.1997  DonC
   Modified fnGetDecimalAttribute to use default Context when Context name is
   null.

1998.06.16  RG
   Changes from ??? without change log:
   in function 'fnSetDecimalAttribute' set NULL-Byte in array 'sz'

12.08.1998  DonC
   Modified fnEditFormatDecimalString to eliminate leading blanks before
   a decimal string being passed to SetAttribute.
*/

#define  KZMETA_INCL
#define  KZSYSSVC_INCL
#define  KZOESYS_INCL
#include "kzoengaa.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "tzapdmaa.hg"

zSHORT
fnSetDecimalAttribute( LPDOMAINDATA lpDomainData,
                       zBOOL        bValidationOnly );
zSHORT
fnGetDecimalAttribute( LPDOMAINDATA lpDomainData );
zSHORT
fnAddToDecimalAttribute( LPDOMAINDATA lpDomainData );
zSHORT
fnCompareDecimalAttribute( LPDOMAINDATA lpDomainData );
zSHORT
fnGetCompValDecimalAttribute( LPDOMAINDATA lpDomainData );
zSHORT
fnDecIntContext( zLPCONTEXT lpContext );
zSHORT
fnFormatDecimalString( zPCHAR pchDecString,
                       zPCHAR pchThou,
                       zPCHAR pchDecPt,
                       zLONG  lPrecision );
zSHORT
fnEditFormatDecimalString( zPCHAR pchDecString,
                           zPCHAR pchThou,
                           zPCHAR pchDecPt );
zSHORT
fnGetDecimalFmtStrings( zLPCONTEXT  lpContext,
                        zPCHAR      szReturnThou,
                        zPCHAR      szReturnDecPt,
                        zPLONG      lplPrecision );
zSHORT
fnStrToDec( zPCHAR         sz,
            zVIEW          zView,
            LPVIEWENTITY   lpViewEntity,
            LPVIEWATTRIB   lpViewAttribute,
            LPDOMAIN       lpDomain,
            zPCHAR         szContextName,
            zPDECIMAL      lpDecimal,
            zBOOL          bValidationOnly );

//
// ///////////////////////////////////////////////////////////////////
// * Copyright (c) 1993 QuinSoft Corporation. All rights reserved.   *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// ///////////////////////////////////////////////////////////////////
//  AUTHOR:  John Price
//  DATE:    7/28/93
//  API:     TBA
//  ENVIRONMENT: ANY
//
// MODULE NAME:  TZAPDMAC  -  Application Domain library Decimal
//                            routines
//
// DESCRIPTION:  This is the source file which contains Decimal domains
//               that are supplied for the Zeidon product.
//
//

//./ ADD NAME=StandardDomainInterface
// Source Module=tzapdmac.c
////////////////////////////////////////////////
//
// ENTRY:  StandardDomainInterface
//
//         This is a description of the Domain Interface supplied with
//         the Zeidon product.
//
//         A Domain is used for four purposes:
//         1. Validation - It validates any dat enterd through a window
//         or through a SetAttribute operation.
//         2. Presentation - It formats data when displaying it
//         on windows and reports and it accepts formatted information
//         from data entry on a window.
//         3. Data Conversion - It provides for the controlled
//         conversion of data from one valid Domain to another valid
//         Domain.
//         4. Data Comparison - It provides for the controlled
//         comparison of data from one valid Domain to another valid
//         Domain.
//
// OPERATIONS:   Each Domain entry should handle each of the possible
//               entry types.
// PARAMETERS:
//
// 1. lEntryType -
//       Indicates how to handle the other parameters.  Valid values are:
//       -  zDME_SET_ATTRIBUTE
//             Set Attribute, this entry is always used for setting an
//             attribute.  If the data type is string, it is probably the
//             normal data entry interface for validating string input.
//       -  zDME_GET_VARIABLE -
//             Get Variable, this entry is always used for setting a
//             variable from an attribute.  If the data type is string it
//             is probably the normal data presentation interface for
//             formatting data to a string.
//       -  zDME_COMPARE_ATTRIBUTE -
//             Compare Attribute, this entry will be called whenever an
//             attribute is compared to another attribute or variable.  The
//             Domain for the must include the code to handle the
//             comparison to source with any context defined for it.
//       -  zDME_GET_FIRST_TBL_ENT_FOR_ATTR
//       -  zDME_GET_NEXT_TBL_ENT_FOR_ATTR
//             These two entries are necessary to create the list of
//             entries for a Combo Box.  It could also be used by any
//             interface that wanted to list all the Domain values for a
//             given Context.
//       -  zDME_SET_ATTRIBUTE_VALUE_NEXT
//       -  zDME_SET_ATTRIBUTE_VALUE_PREV
//             These entries are necessary to handle a Spin Button.  It
//             would increment/decrement a numeric value or retrieve the
//             next or previous table entry for a given Context.
//       -  zDME_ADD_TO_ATTRIBUTE
//
//       -  zDME_GET_COMPARE_VALUE
//             Used by SetEntityCursor for performance.  SetEntityCursor
//             will ask for the internal attribute value for its input
//             qualifying value, and use the returned value in determining
//             if the entity instance matches the qualification request.
//             See Return Codes below.
//
//       -  zDME_VALIDATE_LPDATA
//             Go through validation procedures as if this was a
//             zDME_SET_ATTRIBUTE call, but do NOT set any attribute
//             values, just set the return code.
//
// 2. cType -
//       The Source/Target Data Type, identifies the type of data (of valid
//       Zeidon data types) that is the source in a Set call and the target
//       in a Get call.
// 3. lpData -
//       The Source/Target Data Pointer, points to the data that is the
//       source in a Set call and the target in a Get call.
// 4. szContextName    -
//       The Context Name to be applied to this Domain function.
// 5. zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
// 6. lpViewEntity -
//       Points to the entity that is the target in a Set call and the
//       source in a Get call.
// 7. lpViewAttribute -
//       Points to the attribute that is the target in a Set call and the
//       source in a Get call.
// 8. nMaxStringLength -
//       For a Get call with data type 'string', it identifies the maximum
//       length of the string variable (should be used only with a
//       GetVariableFromAttribute operation).
//
// RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has been set
//
//  For zDME_GET_VARIABLE:
//              0 - Attribute value returned
//
//  For zDME_COMPARE_ATTRIBUTE:
//             -1 - Attribute value < lpData value
//              0 - Attribute value = lpData value
//              1 - Attribute value > lpData value
//
//  For zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
//              0 - Value returned
//             -1 - No value returned, Context has no table entries
//
//  For zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
//              0 - Value returned
//             -1 - No value returned, end of table has been reached
//
//  For zDME_SET_ATTRIBUTE_VALUE_NEXT:
//              0 - Attribute value has been set
//              1 - Attribute value is null
//
//
//  For zDME_SET_ATTRIBUTE_VALUE_PREV:
//             Same as zDME_SET_ATTRIBUTE_VALUE_NEXT.
//
//  For zDME_ADD_TO_ATTRIBUTE:
//
//  For zDME_GET_COMPARE_VALUE:
//             -2 - lpData ignored, this tells SetEntityCursor that
//                  it should invoked domain processing with the
//                  zDME_COMPARE_ATTRIBUTE entry type, each time
//                  that is determining if the EntityInstance
//                  matches the requested qualifications.
//             -1 - lpData invalid
//              0 - lpData unchanged, okay to use as is.
//              1 - lpData returned, string value
//              2 - lpData returned, integer value
//              3 - lpData returned, decimal value
//
//  For zDME_VALIDATE_LPDATA:
//              0 - Data is okay.
//
//  For all of the above:
//    zCALL_ERROR - error on call
//
////////////////////////////////////////////////
//./ END + 0

#define zMAX_DECIMAL_STRING     256   // 128 is too short

//./ ADD NAME=zdmDecimal
// Source Module=tzapdmac.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmDecimal
//
//  PURPOSE:    To retrieve an decimal representing an attribute value
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT zLONG OPERATION
zdmDecimal( zLONG        lEntryType,
            LPDOMAINDATA lpDomainData )
{
   zSHORT      nRC;
   LPDOMAIN    lpDomain;

   nRC = zCALL_ERROR;         // Default to in error status...
   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

#if 0
   TraceLineS( "(apdm) ", "zdmDecimal" );
   TraceLineI( "(apdm) Entry type is ", lEntryType );
   szWorkString[ 0 ] = lpDomainData->cType;
   szWorkString[ 1 ] = 0;
   TraceLineS( "(apdm) lpDomainData->cType is ", szWorkString );
   TraceLineS( "(apdm) lpDomainData->pszContextName is ",
               lpDomainData->pszContextName );
   TraceLineS( "(apdm) EntityName is ", lpDomainData->lpViewEntity->szName );
   TraceLineS( "(apdm) AttributeName is ", lpDomainData->lpViewAttribute->szName );
#endif

   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
         nRC = fnSetDecimalAttribute( lpDomainData, 1 );
         break;

      case zDME_SET_ATTRIBUTE:
         nRC = fnSetDecimalAttribute( lpDomainData, 0 );
         break;

      case zDME_GET_VARIABLE:
         nRC = fnGetDecimalAttribute( lpDomainData );
         break;

      case zDME_ADD_TO_ATTRIBUTE:
         nRC = fnAddToDecimalAttribute( lpDomainData );
         break;

      case zDME_COMPARE_ATTRIBUTE:
         nRC = fnCompareDecimalAttribute( lpDomainData );
         break;

      case zDME_GET_COMPARE_VALUE:
         nRC = fnGetCompValDecimalAttribute( lpDomainData );
         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         // "TZDME004 - Invalid Domain Entry Type "
         SendDomainError( lpDomainData->zView, lpDomain, 8, 4, lEntryType,
                          lpDomainData->lpViewEntity->szName,
                          lpDomainData->lpViewAttribute->szName );
         nRC = zCALL_ERROR;

      default:
         // We're not handling this case so send it back.
         nRC = zDME_NOT_HANDLED;
   }

   return( nRC );
}

//./ ADD NAME=fnSetDecimalAttribute
//./ END + 9
zSHORT
fnSetDecimalAttribute( LPDOMAINDATA lpDomainData,
                       zBOOL        bValidationOnly )
{
   LPDOMAIN    lpDomain;
   zSHORT      nRC;
   zCHAR       sz[ zMAX_DECIMAL_STRING ];
   zPCHAR      psz;
   zSHORT      k, j, nStart;
   zDECIMAL    d;
   zDECIMAL    range;
   zPDECIMAL   lpdDecimalToStore;
   zLPCONTEXT  lpContext;
   zBOOL       bCompare;
   zCHAR       szThou[ 10 ];
   zCHAR       szDecPt[ 10 ];
   zLONG       lPrecision;

   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   if ( lpDomainData->lpViewAttribute->cType != zTYPE_DECIMAL )
   {
      *sz = lpDomainData->lpViewAttribute->cType;
      *(sz + 1) = 0;
      strcat_s( sz, sizeof( sz ), ", " );
      strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
      // "TZDME003 - Attribute Type invalid for this Domain"
      SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0, sz, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   // Unless we're told otherwise the decimal we want stored will be in 'd'.
   lpdDecimalToStore = &d;

   // determine input data type
   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         // string to decimal
         psz = (zPCHAR) lpDomainData->lpData;
         if ( psz == 0 || *psz == 0 )
            SysAssignDecimalFromNull( &d );
         else
         {
            strncpy_s( sz, sizeof( sz ), psz, sizeof( sz ) - 1 );
            sz[ sizeof( sz ) - 1 ] = 0;
            nRC = fnStrToDec( sz, lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                              lpDomain, lpDomainData->pszContextName, &d, bValidationOnly );
            if ( nRC == zCALL_ERROR )
               return( nRC );
         }

         break;

      case zTYPE_INTEGER:
         // long to decimal
         if ( *((zPLONG) lpDomainData->lpData) == lNullInteger )
            SysAssignDecimalFromNull( &d );
         else
            SysConvertLongToDecimal( *(zPLONG) lpDomainData->lpData, &d );

         break;

      case zTYPE_DECIMAL:
         {
            zCHAR  szDecimal[ 100 ];
            zSHORT nPrecision;

            // convert decimal to decimal
            szDecimal[ 0 ] = lpDomain->cDecimal;
            szDecimal[ 1 ] = 0;
            nPrecision = (zSHORT) zatol( szDecimal );
            SysConvertDecimalToString( (zPDECIMAL) lpDomainData->lpData, szDecimal, nPrecision );
            SysConvertStringToDecimal( szDecimal, &d );
         }

         break;

      default:
         *sz = lpDomainData->cType;
         *(sz + 1) = 0;
         strcat_s( sz, sizeof( sz ), ", " );
         strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
         // "TZDME001 - Invalid Input Data Type"
         SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, sz, lpDomainData->lpViewAttribute->szName );
         return( zCALL_ERROR );
   }

   // Now perform any range checking to make sure the decimal value is
   // within range.  The range value is in the EditString variable and
   // needs to be parsed.

   if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) && lpContext->hEditString )
   {
      zCHAR  szEditString[ zMAX_DECIMAL_STRING ];

      strcpy_s( szEditString, sizeof( szEditString ), (zPCHAR) SysGetPointerFromHandle( lpContext->hEditString ) );

      // Loop for each range check in EditString.
      // At each pass of loop, copy the range check to sz.
      nStart = 0;
      for ( k = 0; szEditString[ k ] != ';' && szEditString[ k ] != 0; k++ )
      {
         ;
      }

      for ( ; ; ) // loop until colon is found
      {
         strncpy_s( sz, sizeof( sz ), szEditString + nStart, k - nStart );
         sz[ k - nStart ] = 0;

         if ( sz[ 0 ] == '>' || sz[ 0 ] == '<' || sz[ 0 ] == '=' )
         {
            zSHORT nCompare;

            // First convert the range value to a decimal.

            // Stop at first nonblank after compare symbol.
            if ( sz[ 1 ] == '=' )
            {
               for ( j = 0; sz[ j + 2 ] == ' '; j++ );
               psz = sz + 2 + j;
            }
            else
            {
               for ( j = 0; sz[ j + 1 ] == ' '; j++ );
               psz = sz + 1 + j;
            }

            // Determine what is the szThou, szDecPt, and lPrecision from the
            // range string.
            for ( j = (zSHORT) zstrlen( psz ) - 1; j >= 0; j-- )
            {
               if ( psz[ j ] == ',' || psz[ j ] == '.' )
                  break;
            }

            // If j > -1 then psz[ j ] points to the char
            // representing the decimal point.
            if ( j > -1 )
            {
               lPrecision = (zSHORT) zstrlen( psz ) - j;
               szDecPt[ 0 ] = psz[ j ];
               szDecPt[ 1 ] = 0;

               if ( szDecPt[ 0 ] == ',' )
                  szThou[ 0 ] = '.';
               else
                  szThou[ 0 ] = ',';

               szThou[ 1 ] = 0;
            }
            else
            {
               // Precision is 0 and it doesn't matter what szThou etc. is.
               lPrecision = 0;
               strcpy_s( szDecPt, sizeof( szDecPt ), "," );
               strcpy_s( szThou, sizeof( szThou ), "." );
            }

            fnEditFormatDecimalString( psz, szThou, szDecPt );
            SysConvertStringToDecimal( psz, &range );

            // Next perform the correct compare.
            nCompare = SysCompareDecimalToDecimal( lpdDecimalToStore, &range );
            switch ( sz[ 0 ] )
            {
               case '=':
                  bCompare = ( nRC == 0 );
                  break;

               case '>':
                  if ( sz[ 1 ] == '=' )
                     bCompare = ( nRC >= 0 );
                  else
                     bCompare = ( nRC > 0 );
                  break;

               case '<':
                  if ( sz[ 1 ] == '=' )
                     bCompare = ( nRC <= 0 );
                  else
                     bCompare = ( nRC < 0 );
                  break;
            }

            if ( bCompare == 0 )
            {
               *sz = lpDomainData->cType;
               *(sz + 1) = 0;
               strcat_s( sz, sizeof( sz ), ", " );
               strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
               // "TZDME023 - Value out of valid range"
               SendDomainError( lpDomainData->zView, lpDomain, 8, 23, 0, sz, lpDomainData->lpViewAttribute->szName );
               return( zCALL_ERROR );
            }
         }

         if ( szEditString[ k ] == ';' )
         {
            // Find first nonblank character after the colon.
            for ( k++; szEditString[ k ] == ' '; k++ );
            nStart = k;
            // Find end of next range value.
            for ( k = nStart;
                  szEditString[ k ] != ';' && szEditString[ k ] != 0;
                  k++ )
            {
               ;
            }
         }
         else
            break;

      } // End of EditString loop
   }

   if ( bValidationOnly )
      nRC = 0;
   else
      nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                lpDomainData->lpViewAttribute, lpdDecimalToStore, 0 );
   return( nRC );
}

//./ ADD NAME=fnGetDecimalAttribute
//./ END + 8
zSHORT
fnGetDecimalAttribute( LPDOMAINDATA lpDomainData )
{
   LPDOMAIN    lpDomain;
   zLPCONTEXT  lpContext;
   zSHORT      nRC;
   zCHAR       sz[ zMAX_DECIMAL_STRING ];
   zCHAR       szWorkName[ 33 ];
   zPCHAR      lpAttrValue;
   zCHAR       cAttrType;
   zULONG      uAttrLength;
   zBOOL       bWasNull = FALSE;
   zCHAR       szThou[ 10 ];
   zCHAR       szDecPt[ 10 ];
   zLONG       lPrecision;
   zDECIMAL    dDecimal;
   zDECIMAL    dNullDecimal;
   LPTASK      lpTask = SfGetCurrentTask( lpDomainData->zView );

   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );
   SysAssignDecimalFromNull( &dNullDecimal );

   if ( lpDomainData->lpViewAttribute->cType != zTYPE_DECIMAL )
   {
      *sz = lpDomainData->lpViewAttribute->cType;
      *(sz + 1) = 0;
      strcat_s( sz, sizeof( sz ), ", " );
      strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
      // "TZDME003 - Attribute Type invalid for this Domain"
      SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0, sz, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   if ( lpDomainData->cType != zTYPE_PIC )
   {
      nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                &lpAttrValue, &cAttrType, &uAttrLength );
      if ( nRC )
         return( zCALL_ERROR );

      // If there is no lpRecord, lpAttrValue returns a null...
      if ( lpAttrValue == 0 )
         lpAttrValue = (zPCHAR) &dNullDecimal;
   }

   // determine output data type
   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         // First get the formatting information from the Context or use
         // default.
         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
         {
            if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName) )
            {
               // Get the formatting characters.
               nRC = fnGetDecimalFmtStrings( lpContext, szThou, szDecPt, &lPrecision );
               if ( nRC )
               {
                  // "TZDME019 - International number formatting is
                  //  not available "
                  // We continue with the US default...
                  SendDomainError( lpDomainData->zView, lpDomain, 16, 19, 0,
                                   lpDomainData->lpViewEntity->szName,
                                   lpDomainData->lpViewAttribute->szName );
               }

               // Check if the Context is one of the DecimalKeyx values.
               // If so, there will be no thousands value.
               strcpy_s( szWorkName, sizeof( szWorkName ), lpDomainData->pszContextName );
               szWorkName[ 10 ] = 0;
               if ( zstrcmp( szWorkName, "DecimalKey" ) == 0 )
               {
                  szThou[ 0 ] = 0;
               }
               else
               {
                  // Make sure it's not one of the arithmetic types.
                  if ( fnDecIntContext( lpContext ) )
                  {
                     zCHAR  sz2[ zMAX_DECIMAL_STRING ];

                     strcpy_s( sz, sizeof( sz ), lpContext->szName );
                     strcat_s( sz, sizeof( sz ), "/S" );
                     strcpy_s( sz2, sizeof( sz2 ), lpDomainData->lpViewEntity->szName );
                     strcat_s( sz2, sizeof( sz2 ), ", " );
                     strcat_s( sz2, sizeof( sz2 ), lpDomainData->lpViewAttribute->szName );
                     // "TZDME013 - Context/cType Combination is invalid "
                     SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz, sz2 );
                     return( zCALL_ERROR );
                  }
               }
            }
            else
            {
               // "TZDME008 - Could not find context for Domain "
               SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, lpDomainData->pszContextName, 0 );
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
         else
         {
            fnFormatDecimalString( sz, szThou, szDecPt, lPrecision );
         }

         // Return the formatted decimal, unless the return area is not
         // large enough, in that case fill it with '*'.
         if ( lpDomainData->uMaxStringLength && zstrlen( sz ) > lpDomainData->uMaxStringLength )
         {
            zmemset( lpDomainData->lpData, '*', lpDomainData->uMaxStringLength - 1 );
            *((zPCHAR) lpDomainData->lpData + lpDomainData->uMaxStringLength - 1) = 0;

            // "TZDME021 - Return area not large enough for formatted string "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 21, 0,  lpDomainData->lpViewEntity->szName,
                             lpDomainData->lpViewAttribute->szName );
            nRC = zCALL_ERROR;
         }
         else
         {
            strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, sz );
            nRC = bWasNull ? -1 : 0;
         }

         break;

      case zTYPE_INTEGER:
         // decimal to long
         if ( SysCompareDecimalToNull( (zPDECIMAL) lpAttrValue ) == 0 )
         {
            *((zPLONG) lpDomainData->lpData) = 0;
            nRC = -1;   // indicate attribute is null
         }
         else
         {
            if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
            {
               if ( zstrcmp( lpDomainData->pszContextName, szlDecimalIntegerRounded ) == 0 )
               {
                  zDECIMAL dTemp;
                  zDECIMAL dZero;

                  SysConvertLongToDecimal( 0, &dZero );
                  SysAssignDecimalFromDecimal( &dTemp, (zPDECIMAL) lpAttrValue );
                  if ( SysCompareDecimalToDecimal( &dTemp, &dZero ) < 0 )
                     SysConvertDoubleToDecimal( -0.5, &dZero );
                  else
                     SysConvertDoubleToDecimal( 0.5, &dZero );

                  SysAddDecimalToDecimal( &dTemp, &dTemp, &dZero );
                  SysConvertDecimalToLong( &dTemp, (zPLONG) lpDomainData->lpData );
               }
               else
               if ( zstrcmp( lpDomainData->pszContextName,
                             szlDecimalFraction ) == 0 )
               {
                  zCHAR    sz2[ zMAX_DECIMAL_STRING ];

                  strcpy_s( sz, sizeof( sz ), lpDomainData->pszContextName );
                  strcat_s( sz, sizeof( sz ), "/L" );
                  strcpy_s( sz2, sizeof( sz2 ), lpDomainData->lpViewEntity->szName );
                  strcat_s( sz2, sizeof( sz2 ), ", " );
                  strcat_s( sz2, sizeof( sz2 ), lpDomainData->lpViewAttribute->szName );
                  // "TZDME013 - Context/cType Combination is invalid "
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz, sz2 );
                  return( zCALL_ERROR );
               }
               else
                  SysConvertDecimalToLong( (zPDECIMAL) lpAttrValue, (zPLONG) lpDomainData->lpData );
            }
            else
               SysConvertDecimalToLong( (zPDECIMAL) lpAttrValue, (zPLONG) lpDomainData->lpData );

            nRC = 0;
         }

         break;

      case zTYPE_DECIMAL:
         // decimal to decimal
         if ( SysCompareDecimalToNull( (zPDECIMAL) lpAttrValue ) == 0 )
         {
            SysConvertLongToDecimal( 0, (zPDECIMAL) lpDomainData->lpData );
            nRC = -1;   // indicate attribute is null
         }
         else
         {
            if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
            {
               if ( zstrcmp( lpDomainData->pszContextName, szlDecimalInteger ) == 0 )
               {
                  zLONG lTemp;

                  SysConvertDecimalToLong( (zPDECIMAL) lpAttrValue, &lTemp );
                  SysConvertLongToDecimal( lTemp, (zPDECIMAL) lpDomainData->lpData );
               }
               else
               if ( zstrcmp( lpDomainData->pszContextName,
                             szlDecimalFraction ) == 0 )
               {
                  zLONG    l1;
                  zDECIMAL d1;

                  SysAssignDecimalFromDecimal( &d1, (zPDECIMAL) lpAttrValue );
                  SysConvertDecimalToLong( &d1, &l1 );
                  SysConvertLongToDecimal( l1, &d1 );
                  SysSubtractDecimalFromDecimal( (zPDECIMAL) lpDomainData->lpData, (zPDECIMAL) lpAttrValue, &d1 );
               }
               else
               if ( zstrcmp( lpDomainData->pszContextName,
                             szlDecimalIntegerRounded ) == 0 )
               {
                  zLONG    l;
                  zDECIMAL d1;
                  zDECIMAL d2;

                  SysAssignDecimalFromDecimal( &d1, (zPDECIMAL) lpAttrValue );
                  SysConvertLongToDecimal( 0, &d2 );
                  if ( SysCompareDecimalToDecimal( &d1, &d2 ) >= 0 )
                     SysConvertDoubleToDecimal( 0.5, &d2 );
                  else
                     SysConvertDoubleToDecimal( -0.5, &d2 );

                  SysAddDecimalToDecimal( &d1, &d1, &d2 );
                  SysConvertDecimalToLong( &d1, &l );
                  SysConvertLongToDecimal( l, (zPDECIMAL) lpDomainData->lpData );
               }
               else
                  SysAssignDecimalFromDecimal( (zPDECIMAL) lpDomainData->lpData, (zPDECIMAL) lpAttrValue );
            }
            else
            {
               zLONG lPrecision;
               zCHAR szThou[ 50 ];
               zCHAR sz[ 100 ];

               // We want to copy the source decimal to the target decimal but
               // we want to use the precision for the default domain.  The only
               // way to do this is to convert the decimal to a string.
               GetDefaultContext( &lpContext, lpDomain );
               fnGetDecimalFmtStrings( lpContext, szThou, szDecPt, &lPrecision );
               SysConvertDecimalToString( &dDecimal, sz, (zSHORT) lPrecision );
               SysConvertStringToDecimal( sz, (zPDECIMAL) lpAttrValue );
            }

            nRC = 0;
         }

         break;

      case zTYPE_PIC:
         // return the picture string for the given context
         {
            zLPCONTEXT   lpContext;

            if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
            {
               GetContext( &lpContext, lpDomain, lpDomainData->pszContextName);
            }
            else
               GetDefaultContext( &lpContext, lpDomain );

            if ( lpContext )
            {
               zPCHAR lpEditString = (zPCHAR) SysGetPointerFromHandle( lpContext->hEditString );

               if ( lpEditString && *lpEditString )
                  strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, lpEditString );
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
         *sz = lpDomainData->cType;
         *(sz + 1) = 0;
         strcat_s( sz, sizeof( sz ), ", " );
         strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
         // "TZDME001 - Invalid Input Data Type"
         SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, sz, lpDomainData->lpViewAttribute->szName );
         return( zCALL_ERROR );
   }

   return( nRC );
}

//./ ADD NAME=fnAddToDecimalAttribute
//./ END + 8
zSHORT
fnAddToDecimalAttribute( LPDOMAINDATA lpDomainData )
{
   LPDOMAIN    lpDomain;
   zCHAR       sz[ zMAX_DECIMAL_STRING ];
   zPCHAR      lpAttrValue;
   zCHAR       cAttrType;
   zULONG      uAttrLength;
   zDECIMAL    dWork;
   zDECIMAL    d;
   zPDECIMAL   lpdDecimalToStore;
   LPTASK      lpTask = SfGetCurrentTask( lpDomainData->zView );
   zSHORT      nRC;

   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   if ( lpDomainData->lpViewAttribute->cType != zTYPE_DECIMAL )
   {
      *sz = lpDomainData->lpViewAttribute->cType;
      *(sz + 1) = 0;
      strcat_s( sz, sizeof( sz ), ", " );
      strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
      // "TZDME003 - Attribute Type invalid for this Domain"
      SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0, sz, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   // Only allow string, integer or decimal types to be added.
   // If input is null, return immediately.
   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         if ( lpDomainData->lpData == 0 ||
              *((zPCHAR) lpDomainData->lpData) == 0 )
         {
            return( 0 );
         }

         break;

      case zTYPE_INTEGER:
         if ( *((zPLONG) lpDomainData->lpData) == lNullInteger || *((zPLONG) lpDomainData->lpData) == 0L )
         {
            return( 0 );
         }

      break;

      case zTYPE_DECIMAL:
         SysConvertLongToDecimal( 0, &dWork );
         if ( SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) == 0 ||
              SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData, &dWork ) == 0 )
         {
            return( 0 );
         }

         break;

      default:
         return( zCALL_ERROR );
   }

   nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                             lpDomainData->lpViewAttribute,
                             &lpAttrValue, &cAttrType, &uAttrLength );
   if ( nRC )
      return( zCALL_ERROR );

   // if there is no lpRecord, lpAttrValue returns a null...
   if ( lpAttrValue == 0 ||
        SysCompareDecimalToNull( (zPDECIMAL) lpAttrValue ) == 0 )
   {
      SysConvertLongToDecimal( 0, &dWork );
   }
   else
      SysAssignDecimalFromDecimal( &dWork, (zPDECIMAL) lpAttrValue );

   // determine input data type
   switch ( lpDomainData->cType )
   {
    case zTYPE_STRING:
      // string to decimal
      strcpy_s( sz, sizeof( sz ), (zPCHAR) lpDomainData->lpData );
      nRC = fnStrToDec( sz, lpDomainData->zView, lpDomainData->lpViewEntity,
                        lpDomainData->lpViewAttribute,
                        lpDomain, lpDomainData->pszContextName, &d, FALSE );
      if ( nRC == zCALL_ERROR )
         return( nRC );

      SysAddDecimalToDecimal( &dWork, &dWork, &d );
      break;

    case zTYPE_INTEGER:
      // long to decimal
      if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
      {
         if ( zstrcmp( lpDomainData->pszContextName, szlDecimalFraction ) == 0 )
         {
            zCHAR sz2[ zMAX_DECIMAL_STRING ];

            strcpy_s( sz, sizeof( sz ), lpDomainData->pszContextName );
            strcat_s( sz, sizeof( sz ), "/L" );
            strcpy_s( sz2, sizeof( sz2 ), lpDomainData->lpViewEntity->szName );
            strcat_s( sz2, sizeof( sz2 ), ", " );
            strcat_s( sz2, sizeof( sz2 ), lpDomainData->lpViewAttribute->szName );
            // "TZDME013 - Context/cType Combination is invalid "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz, sz2 );
            return( zCALL_ERROR );
         }
         else
         if ( zstrcmp( lpDomainData->pszContextName,
                       szlDecimalIntegerRounded ) == 0 )
         {
            strcpy_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
            strcat_s( sz, sizeof( sz ), ", " );
            strcat_s( sz, sizeof( sz ), lpDomainData->lpViewAttribute->szName );
            // "TZDME014 - Context is for retrieval only "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 14, 0,
                             lpDomainData->pszContextName, sz );
            return( zCALL_ERROR );
         }
         else
            SysConvertLongToDecimal( *(zPLONG) lpDomainData->lpData, &d );
      }
      else
         SysConvertLongToDecimal( *(zPLONG) lpDomainData->lpData, &d );

      nRC = 0;
      SysAddDecimalToDecimal( &dWork, &dWork, &d );
      break;

    case zTYPE_DECIMAL:
      // decimal to decimal
      if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
      {
         if ( zstrcmp( lpDomainData->pszContextName, szlDecimalInteger ) == 0 )
         {
            zLONG l;

            SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData, &l );
            SysConvertLongToDecimal( l, &d );
         }
         else
         if ( zstrcmp( lpDomainData->pszContextName,
                       szlDecimalFraction ) == 0 )
         {
            zLONG    l1;
            zDECIMAL d2;

            SysAssignDecimalFromDecimal( &d, (zPDECIMAL) lpDomainData->lpData );
            SysConvertDecimalToLong( &d, &l1 );
            SysConvertLongToDecimal( l1, &d2 );
            SysSubtractDecimalFromDecimal( &d, &d, &d2 );
         }
         else
         if ( zstrcmp( lpDomainData->pszContextName,
                       szlDecimalIntegerRounded ) == 0 )
         {
            strcpy_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
            strcat_s( sz, sizeof( sz ), ", " );
            strcat_s( sz, sizeof( sz ), lpDomainData->lpViewAttribute->szName );
            // "TZDME014 - Context is for retrieval only "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 14, 0, lpDomainData->pszContextName, sz );
            return( zCALL_ERROR );
         }
         else
            d = *((zPDECIMAL) lpDomainData->lpData);
      }
      else
         d = *((zPDECIMAL) lpDomainData->lpData);

      nRC = 0;

      SysAddDecimalToDecimal( &dWork, &dWork, &d );
      break;

    default:
      *sz = lpDomainData->cType;
      *(sz + 1) = 0;
      strcat_s( sz, sizeof( sz ), ", " );
      strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
      // "TZDME001 - Invalid Input Data Type"
      SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, sz, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   lpdDecimalToStore = &dWork;
   nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                             lpDomainData->lpViewAttribute, lpdDecimalToStore, 0 );
   return( nRC );
}

//./ ADD NAME=fnCompareDecimalAttribute
//./ END + 8
zSHORT
fnCompareDecimalAttribute( LPDOMAINDATA lpDomainData )
{
   LPDOMAIN    lpDomain;
   zCHAR       sz[ zMAX_DECIMAL_STRING ];
   zPCHAR      lpAttrValue;
   zCHAR       cAttrType;
   zULONG      uAttrLength;
   zDECIMAL    dAttrWork;
   zDECIMAL    d;
   LPTASK      lpTask = SfGetCurrentTask( lpDomainData->zView );
   zSHORT      nNULL = 0;
   zSHORT      nRC;

   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   if ( lpDomainData->lpViewAttribute->cType != zTYPE_DECIMAL )
   {
      *sz = lpDomainData->lpViewAttribute->cType;
      *(sz + 1) = 0;
      strcat_s( sz, sizeof( sz ), ", " );
      strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
      // "TZDME003 - Attribute Type invalid for this Domain"
      SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0, sz, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                             lpDomainData->lpViewAttribute, &lpAttrValue, &cAttrType, &uAttrLength );
   if ( nRC )
      return( zCALL_ERROR );

   // if there is no lpRecord, lpAttrValue returns a null...
   if ( lpAttrValue == 0 ||
        SysCompareDecimalToNull( (zPDECIMAL) lpAttrValue ) == 0 )
   {
      SysAssignDecimalFromNull( &dAttrWork );
      nNULL = 1;
   }
   else
      SysAssignDecimalFromDecimal( &dAttrWork, (zPDECIMAL) lpAttrValue );

   if ( lpDomainData->pszContextName && *lpDomainData->pszContextName &&
        nNULL == 0 )
   {
      if ( zstrcmp( lpDomainData->pszContextName, szlDecimalInteger ) == 0 )
      {
         zLONG l;

         SysConvertDecimalToLong( &dAttrWork, &l );
         SysConvertLongToDecimal( l, &dAttrWork );
      }
      else
      if ( zstrcmp( lpDomainData->pszContextName, szlDecimalIntegerRounded ) == 0 )
      {
         zDECIMAL d;
         zLONG    l;

         if ( SysCompareDecimalToDouble( &dAttrWork, 0.0 ) < 0 )
            SysConvertDoubleToDecimal( -0.5, &d );
         else
            SysConvertDoubleToDecimal( 0.5, &d );

         SysAddDecimalToDecimal( &dAttrWork, &dAttrWork, &d );
         SysConvertDecimalToLong( &dAttrWork, &l );
         SysConvertLongToDecimal( l, &dAttrWork );
      }
      else
      if ( zstrcmp( lpDomainData->pszContextName, szlDecimalFraction ) == 0 )
      {
         zLONG    l1;
         zDECIMAL d;

         SysConvertDecimalToLong( &dAttrWork, &l1 );
         SysConvertLongToDecimal( l1, &d );
         SysSubtractDecimalFromDecimal( &dAttrWork, &dAttrWork, &d );
      }

      // If none of the special contexts above, just use as is.
   }

   // Set in case lpDomainData->lpData input is null.
   SysConvertLongToDecimal( 0, &d );

   // determine input data type
   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         // string to decimal
         if ( lpDomainData->lpData && *((zPCHAR) lpDomainData->lpData) )
         {
            strcpy_s( sz, sizeof( sz ), (zPCHAR) lpDomainData->lpData );
            nRC = fnStrToDec( sz, lpDomainData->zView, lpDomainData->lpViewEntity,
                              lpDomainData->lpViewAttribute, lpDomain, lpDomainData->pszContextName, &d, FALSE );
            if ( nRC == zCALL_ERROR )
               return( nRC );
         }
         else
         {
            // we compare against an empty String
            //  this is only TRUE, if the attribute is NULL
            if ( nNULL )
               return( 0 );
            else
               return( 1 );
         }

         break;

      case zTYPE_INTEGER:
         // long to decimal
         if ( *((zPLONG) lpDomainData->lpData) == lNullInteger )
         {
            // we compare against a NULL integer
            //  this is only TRUE, if the decimal attribute is NULL too
            if ( nNULL )
               return( 0 );
            else
               return( 1 );
         }

         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName &&
              zstrcmp( lpDomainData->pszContextName, szlDecimalFraction ) == 0 )
         {
            zCHAR    sz2[ zMAX_DECIMAL_STRING ];

            strcpy_s( sz, sizeof( sz ), lpDomainData->pszContextName );
            strcat_s( sz, sizeof( sz ), "/L" );
            strcpy_s( sz2, sizeof( sz2 ), lpDomainData->lpViewEntity->szName );
            strcat_s( sz2, sizeof( sz2 ), ", " );
            strcat_s( sz2, sizeof( sz2 ), lpDomainData->lpViewAttribute->szName );

            // "TZDME013 - Context/cType Combination is invalid "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz, sz2 );
            return( zCALL_ERROR );
         }
         else
            SysConvertLongToDecimal( *(zPLONG) lpDomainData->lpData, &d );

         break;

      case zTYPE_DECIMAL:
         // convert decimal to decimal
         //  NULL and NULL compare ( mathematically not correct but ... )
         if ( SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) == 0 )
         {
            if ( nNULL )
               return( 0 );
            else
               return( 1 );
         }

         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
         {
            if ( zstrcmp( lpDomainData->pszContextName, szlDecimalInteger ) == 0 )
            {
               zLONG l;

               SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData, &l );
               SysConvertLongToDecimal( l, &d );
            }
            else
            if ( zstrcmp( lpDomainData->pszContextName,
                          szlDecimalIntegerRounded ) == 0 )
            {
               zDECIMAL d;
               zLONG    l;

               if ( SysCompareDecimalToDouble( (zPDECIMAL) lpDomainData->lpData, 0.0 ) < 0 )
                  SysConvertDoubleToDecimal( -0.5, &d );
               else
                  SysConvertDoubleToDecimal( 0.5, &d );

               SysAddDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData, (zPDECIMAL) lpDomainData->lpData, &d );
               SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData, &l );
               SysConvertLongToDecimal( l, &d );
            }
            else
            if ( zstrcmp( lpDomainData->pszContextName,
                          szlDecimalFraction ) == 0 )
            {
               zLONG    l1;
               zDECIMAL d;

               SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData, &l1 );
               SysConvertLongToDecimal( l1, &d );
               SysSubtractDecimalFromDecimal( &d, (zPDECIMAL) lpDomainData->lpData, &d );
            }
            else
               SysAssignDecimalFromDecimal( &d, (zPDECIMAL) lpDomainData->lpData );
         }
         else
            SysAssignDecimalFromDecimal( &d, (zPDECIMAL) lpDomainData->lpData );

         break;

      default:
         *sz = lpDomainData->cType;
         *(sz + 1) = 0;
         strcat_s( sz, sizeof( sz ), ", " );
         strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
         // "TZDME001 - Invalid Input Data Type"
         SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, sz, lpDomainData->lpViewAttribute->szName );
         return( zCALL_ERROR );
   }

   // For compatibility:
   //  if a NULL decimal is compared against 0.0, then we say TRUE.
   //  This is for old applications, where comparison to zero was used
   //  to check for a NULL value

   if ( nNULL )
   {
      if ( SysCompareDecimalToDouble( &d, 0.0 ) == 0 )
         return( 0 ); // Compatibility
      else
         return( -1 );
   }

   SysSubtractDecimalFromDecimal( &dAttrWork, &dAttrWork, &d );
   nRC = SysCompareDecimalToDouble( &dAttrWork, 0.0 );
   return( nRC );
}

//./ ADD NAME=fnGetCompValDecimalAttribute
//./ END + 8
zSHORT
fnGetCompValDecimalAttribute( LPDOMAINDATA lpDomainData )
{
   LPDOMAIN lpDomain;
   zDECIMAL d;
   zCHAR    sz[ 50 ];
   zSHORT   nRC;

   lpDomain = (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         if ( lpDomainData->lpData && *((zPCHAR) lpDomainData->lpData) )
         {
            strcpy_s( sz, sizeof( sz ), (zPCHAR) lpDomainData->lpData );
            nRC = fnStrToDec( sz, lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                              lpDomain, lpDomainData->pszContextName, &d, FALSE );
            if ( nRC == zCALL_ERROR )
               return( nRC );
         }

         *((zPDECIMAL) lpDomainData->lpData) = d;
         nRC = 3;
         break;

      case zTYPE_INTEGER:
         d = *((zPDECIMAL) lpDomainData->lpData);
         *((zPDECIMAL) lpDomainData->lpData) = d;
         nRC = 3;
         break;

      case zTYPE_DECIMAL:
         nRC = 0; // indicate okay to use as is
         break;

      default:
         sz[ 0 ] = lpDomainData->cType;
         sz[ 1 ] = 0;
         strcat_s( sz, sizeof( sz ), ", " );
         strcat_s( sz, sizeof( sz ), lpDomainData->lpViewEntity->szName );
         // "TZDME001 - Invalid Input Data Type"
         SendDomainError( lpDomainData->zView, (LPDOMAIN) SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain ),
                          8, 1, 0, sz, lpDomainData->lpViewAttribute->szName );
         nRC = -2;
   }

   return( nRC );
}

//./ ADD NAME=fnDecIntContext
// Source Module=tzapdmac.c
////////////////////////////////////////////////
//
//  ENTRY:      fnDecIntContext
//
//  PURPOSE:    Determine if the context is one of our special ones.
//
//  PARAMETERS:
//              lpContext - Context control structure
//
//  RETURNS:
//              0 - Nothing special
//              1 - Isn't that special
//
////////////////////////////////////////////////
//./ END + 2
zSHORT
fnDecIntContext( zLPCONTEXT lpContext )
{
   zPCHAR   psz = lpContext->szName;

   if ( zstrcmp( psz, szlDecimalFraction ) == 0 )
      return( 1 );
   else
   if ( zstrcmp( psz, szlDecimalInteger ) == 0 )
      return( 1 );
   else
   if ( zstrcmp( psz, szlDecimalIntegerRounded ) == 0 )
      return( 1 );
   else
      return( 0 );
}

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

         los = (zLONG) (psz - pchDecString) + (lPrecision ? 1 : 0) + lPrecision;
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
                  while ( l != 0 && *(psz2 + l) == '9' || *(psz2 + l) == *pchDecPt )
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

//./ ADD NAME=fnEditFormatDecimalString
// Source Module=tzapdmac.c
////////////////////////////////////////////////
//
//  ENTRY:      fnEditFormatDecimalString
//
//  PURPOSE:    Undecorate a decimal string with 'commas' and a 'decimal
//              point'.  That is remove the 'commas' and replace the
//              'decimal point' with the decimal point that math
//              conversion routines can use.
//
//  PARAMETERS:
//              pchDecString - in/out decimal string
//              pchThou     - points to the thousands character separator
//              pchDecPt    - points to the decimal point character
//
//  RETURNS:
//              0 - string has been stripped.
//    zCALL_ERROR - 'invalid' character encountered in the pchDecString.
//
////////////////////////////////////////////////
//./ END + 4
zSHORT
fnEditFormatDecimalString( zPCHAR pchDecString,
                           zPCHAR pchThou,
                           zPCHAR pchDecPt )
{
   zPCHAR   psz;
   zPCHAR   psz2;
   zBOOL    bDecimalPointProcessed;

   bDecimalPointProcessed = FALSE;
   psz = pchDecString;

   // Skip leading blanks.
   while ( *psz == ' ' )
      psz++;

   while ( *psz )
   {
      if ( *psz == '+' || *psz == '-' )
      {
         if ( psz != pchDecString )
            return( zCALL_ERROR );
         else
            psz++;
      }
      else
      if ( *psz == *pchThou )
      {
         // Make sure that three digits follow the pchThou character,
         // followed by either another pchThou, an pchDecPt or a null.
         // Else it is an error.
         if ( (isdigit( psz[ 1 ] ) == 0) ||
              (isdigit( psz[ 2 ] ) == 0) ||
              (isdigit( psz[ 3 ] ) == 0) ||
              ((psz[ 4 ] != *pchThou) && (psz[ 4 ] != *pchDecPt) && (psz[ 4 ] != 0)) )
         {
            return( zCALL_ERROR );
         }

         // A thousands character after the Decimal Point is an error.
         if ( bDecimalPointProcessed )
         {
            return( zCALL_ERROR );
         }

         // drop out the thousands separator
         psz2 = psz;
         while ( *psz2 )
         {
            *psz2 = *(psz2 + 1);
            psz2++;
         }
      }
      else
      if ( *psz == *pchDecPt )
      {
         // A second Decimal Point is an error.
         if ( bDecimalPointProcessed )
         {
            return( zCALL_ERROR );
         }

         *psz++ = '.';  // put in a real decimal point
         bDecimalPointProcessed = TRUE;
      }
      else
      if ( isdigit( *psz ) )
         psz++;
      else
      {
         // who knows what, or is it, who's on first.
         // Does what play third?
         return( zCALL_ERROR );
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

   pszName = lpContext->szName;

   // First check for Windows setting.  If so, use all international settings, including precision.
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
      // Use the comma or point as appropriate.  Use the x value, if it exists, for lplPrecision.
      strcpy_s( szReturnThou, sizeof( szReturnThou ), "," );
      strcpy_s( szReturnDecPt, sizeof( szReturnDecPt ), "." );
      if ( pszName[ 12 ] == '4' || pszName[ 12 ] == '3' ||
           pszName[ 12 ] == '2' || pszName[ 12 ] == '1' || pszName[ 12 ] == '0' )
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
           pszName[ 12 ] == '2' || pszName[ 12 ] == '1' || pszName[ 12 ] == '0' )
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

//./ ADD NAME=fnStrToDec
// Source Module=tzapdmac.c
////////////////////////////////////////////////
//
//  ENTRY:      fnStrToDec
//
//  PURPOSE:    Convert a string to a decimal.
//
//  PARAMETERS:
//              sz            - input decimal string
//              lpViewEntity  -
//              lpViewAttrib  -
//              lpDomain      - Domain structure
//              szContextName - context name from entry zdmDecimal
//              lpDecimal     - resultant decimal value.
//
//  RETURNS:
//              0 - Conversion successful.
//    zCALL_ERROR - Error during conversion.
//
////////////////////////////////////////////////
//./ END + 8
zSHORT
fnStrToDec( zPCHAR         sz,
            zVIEW          zView,
            LPVIEWENTITY   lpViewEntity,
            LPVIEWATTRIB   lpViewAttribute,
            LPDOMAIN       lpDomain,
            zPCHAR         szContextName,
            zPDECIMAL      lpDecimal,
            zBOOL          bValidationOnly )
{
   zLPCONTEXT  lpContext;
   zCHAR       sz1[ 40 ];
   zCHAR       szDec[ 100 ];
   zCHAR       szThou[ 10 ];
   zCHAR       szDecPt[ 10 ];
   zLONG       lPrecision;
   zSHORT      k;
   zSHORT      l;
   zSHORT      nSeverity;
   zSHORT      nRC;

   SysConvertDoubleToDecimal( 0.0, lpDecimal );
   if ( szContextName && *szContextName )
   {
      if ( !GetContext( &lpContext, lpDomain, szContextName) )
         GetDefaultContext( &lpContext, lpDomain );
   }
   else
      GetDefaultContext( &lpContext, lpDomain );

   // We should now have a valid Context, either for the ContextName passed in
   // or for the default Context.

   if ( lpContext )
   {
      // Make sure it's not one of the arithmetic types.
      if ( fnDecIntContext( lpContext ) )
      {
         zCHAR    sz2[ 70 ];

         strcpy_s( sz1, sizeof( sz1 ), lpContext->szName );
         strcat_s( sz1, sizeof( sz1 ), "/S" );
         strcpy_s( sz2, sizeof( sz2 ), lpViewEntity->szName );
         strcat_s( sz2, sizeof( sz2 ), ", " );
         strcat_s( sz2, sizeof( sz2 ), lpViewAttribute->szName );

         // "TZDME013 - Context/cType Combination is invalid "
         SendDomainError( zView, lpDomain, 8, 13, 0, sz, sz2 );
         return( zCALL_ERROR );
      }

      nRC = fnGetDecimalFmtStrings( lpContext, szThou, szDecPt, &lPrecision );
      if ( nRC )
      {
         // "TZDME019 - International number formatting is not available "
         // We continue with the US default...
         SendDomainError( zView, lpDomain, 16, 19, 0, lpViewEntity->szName, lpViewAttribute->szName );
      }

      nRC = fnEditFormatDecimalString( sz, szThou, szDecPt );
      if ( nRC )
      {
         zCHAR sz2[ 70 ];

         if ( bValidationOnly )
            nSeverity = -1;
         else
            nSeverity = 0;

         strcpy_s( sz2, sizeof( sz2 ), lpViewEntity->szName );
         strcat_s( sz2, sizeof( sz2 ), ", " );
         strcat_s( sz2, sizeof( sz2 ), lpViewAttribute->szName );
         // "TZDME020 - Invalid decimal string "
         SendDomainError( zView, lpDomain, nSeverity, 20, 0, sz, sz2 );
         return( zCALL_ERROR );
      }
   }
   else
   {
      if ( bValidationOnly )
         nSeverity = -1;
      else
         nSeverity = 8;

      // "TZDME008 - Could not find context for Domain "
      SendDomainError( zView, lpDomain, nSeverity, 8, 0, szContextName, 0 );
      return( zCALL_ERROR );
   }

   strcpy_s( szDec, sizeof( szDec ), sz );

   // Locate the decimal point in the string and then the number of digits after the decimal point.
   for ( k = 0; szDec[ k ] && szDec[ k ] != '.'; k++ )
   {
      ;
   }
   for ( l = k; szDec[ l ]; l++ )
   {
      ;
   }

   // If there is at least one digit after the decimal point, subtract out the decimal point.
   if ( l >= k + 1 )
   {
      // Truncate any trailing 0s.
      while ( szDec[ l - 1 ] == '0' )
         szDec[ --l ] = 0;  // Turn trailing '0' into null-term.

      l--;
   }

   l = l - k;

   // The number of precision places after the decimal point must be <= the precision for the Context/Domain.

   if ( l > lPrecision )
   {
      zCHAR sz2[ 70 ];

      strcpy_s( sz2, sizeof( sz2 ), lpViewEntity->szName );
      strcat_s( sz2, sizeof( sz2 ), ", " );
      strcat_s( sz2, sizeof( sz2 ), lpViewAttribute->szName );
      // "TZDME020 - Invalid decimal string "
      SendDomainError( zView, lpDomain, 0, 20, 0, szDec, sz2 );
      return( zCALL_ERROR );
   }

   SysConvertStringToDecimal( szDec, lpDecimal );
   return( 0 );
}

#ifdef __cplusplus
}
#endif
