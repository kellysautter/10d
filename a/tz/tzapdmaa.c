/*
CHANGE LOG
2002.04.23  HH
   fix bug zdmDynamicTable,
   handle GetAttributeDisplayLength for dynamic table domains

2002.03.05  HH
   fix bug zdmDynamicTable, integer value from string

2001.10.05  DGC
   Fixed a bug reported by Don--zdmDynamicTable() had an error when being
   called with SET_ATTR_FROM_ATTR flag.

2001.06.07  BL    WEB
   The Web Domain XRA_Text has a Length from 65534 characters
   and the Operation fnText works with a Length 4096.
   Change the Length in fnText from 4096 to 65534

2001.04.04  HH/BL
   Improve handling of returncode from strcmp. this code is
   NOT NECESSARILY -1, 0 or +1 but <0, 0 OR >0.
   Under Debug and Windows 2000 there was a erroneous return of
   zDME_NOT_HANDLED. The Error occured at DEUKA in Dueseldorf.

2001.01.26  DC
   Modified zdmDynamicTable to activate and name the OI at the
   APPLICATON level instead of the TASK level. Also added mutex to
   avoid simultaneous table creation from multiple tasks.

2000.09.06  DC
   Modified error message in zdmDynamicTable operation to display the
   table name that could not be loaded.

2000.08.09  HH
   Return -1 if converting a string to decimal yields NULL, bug 53440.
2000.06.30  HH
   fixed bug handling NULL values with integer.
   Concept:
   - compare NULL Integer attribute to NULL string : EQUAL
   - compare NULL Integer attribute to integer 0 : EQUAL
     this is wrong, but needed for compatibility. Applications
     made use of the bug and compared to integer 0 to check for NULL.
     The right way is to compare against NULL string.
   - compare NULL Integer attribute to decimal NULL : EQUAL
   - compare NULL Integer attribute anything else : LESS
   - compare NOT NULL Integer attribute to any NULL variable : GREATER

2000.01.24  DKS  Z10
   Return Error when calling TABLE cases for Integer and Password.

2000.01.24  HH   Z10
2000.01.19  DGC  Z10
   Added zDME_GET_COMPARE_VALUE to zdmDynamicTable().

1999.12.29  DKS  Z10
   Added domain operations for removing & from text and tag fields in the XRA.

1999.04.06  DGC  Z10
   Started changes for new attribute domain interface.

1999.03.10  DKS
   Added zdmExpandEnvVar as a domain to expand embedded environment strings.

17.04.1998  DonC
   Modified zdmPassword routine, case zDME_COMPARE_ATTRIBUTE, to correct
   the compare of a Password value.

10.02.1998  DonC
   Modified zdmInteger so that " " would be treated the same as "". A single
   blank was causing 0 to be mapped to the attribute.

14.01.1998  DonC
   Changed domain error message id TZDMD101 TO TZDME022.

19.12.1997  DonC
   Modified zdmDynamicDecimal to properly handle the case where
   szContextName is null.

15.10.1997  HH
   include kzoeinco.h for 9I.

14.07.1997  DonC
   Eliminated error message when setting one string from another of a longer
   length since the normal handling of such a situation is to truncate the
   string.

28.05.1997  DonC
   Moved operation SendDomainError to KZOEERAA.C

19.04.1997  DonC
   Corrected range edit so that no blank was necessary between operator
   and value.

16.04.1997  FH
   Corrected the formatting of hex values.
*/

#define  KZSYSSVC_INCL
#define  KZOESYS_INCL
#define zGLOBAL_DATA
#include "kzoengaa.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "tzapdmaa.hg"

#define  eDM_TEXT                1
#define  eDM_QSNAME              2
#define  eDM_ALPHANUMERIC        3
#define  eDM_EXPAND_ENVIRONMENT  4
#define  eDM_XRA_TAG             5
#define  eDM_XRA_TEXT            6
#define  eDM_FILENAME            7
#define  eDM_HTML                8
#define  __DO_MSGSEND__

#define  zMAX_TEXTDOMAIN_LEN     65534   // Length from Domain XRA_Text


int
fnHexToLong( zPCHAR  szHexString,
             zPLONG  lIn );

zLONG
fnText( zSHORT       nID,
        zLONG        lEntryType,
        LPDOMAINDATA lpDomainData );

static zSHORT ValidInteger( zPCHAR szInteger );

//
// ///////////////////////////////////////////////////////////////////
// * Copyright (c) 1993 - 2016 QuinSoft, Inc. All rights reserved.     *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// ///////////////////////////////////////////////////////////////////
//  AUTHOR:  John Price
//  DATE:    9/4/92
//  API:     TBA
//  ENVIRONMENT: ANY
//
// MODULE NAME:  TZAPDMAA  -  Application Domain library
//
// DESCRIPTION:  This is the source file which contains standard domains
//               that are supplied for the Zeidon product.
//
//
//./ ADD NAME=StandardDomainInterface
// Source Module=tzapdmaa.c
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

//./ ADD NAME=zdmQSName
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmQSName
//
//  PURPOSE:    To retrieve a string representing a logical value in
//              Zeidon.  A QSName can contain only 'a'-'z', '0' - '9',
//              and '_'.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmQSName( zLONG        lEntryType,
           LPDOMAINDATA lpDomainData )
{
   return( fnText( eDM_QSNAME, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmAlphaNumeric
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmAlphaNumeric
//
//  PURPOSE:    To retrieve a string made up only of alphanumeric characters.
//              It is the same as zdmQSName above.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmAlphaNumeric( zLONG        lEntryType,
                 LPDOMAINDATA lpDomainData )
{
   return( fnText( eDM_ALPHANUMERIC, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmExpandEnvVar
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmExpandEnvVar
//
//  PURPOSE:    To retrieve a string representing an attribute value and
//              to expand embedded environment variables to their values.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmExpandEnvVar( zLONG        lEntryType,
                 LPDOMAINDATA lpDomainData )
{
   if ( lpDomainData->pszContextName &&
        zstrcmp( lpDomainData->pszContextName, "TEXT" ) == 0 )
   {
      return( fnText( eDM_TEXT, lEntryType, lpDomainData ) );
   }
   else
      return( fnText( eDM_EXPAND_ENVIRONMENT, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmText
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      Text
//
//  PURPOSE:    To retrieve a string representing an attribute value
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmText( zLONG        lEntryType,
         LPDOMAINDATA lpDomainData )
{
   return( fnText( eDM_TEXT, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmHTML
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      HTML
//
//  PURPOSE:    To retrieve a string representing an attribute value
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmHTML( zLONG        lEntryType,
         LPDOMAINDATA lpDomainData )
{
   return( fnText( eDM_HTML, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmXRA_Tag
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      XRA_Tag
//
//  PURPOSE:    To retrieve a string representing an attribute value
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmXRA_Tag( zLONG        lEntryType,
            LPDOMAINDATA lpDomainData )
{
   return( fnText( eDM_XRA_TAG, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmXRA_Text
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      XRA_Text
//
//  PURPOSE:    To retrieve a string representing an attribute value
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmXRA_Text( zLONG        lEntryType,
             LPDOMAINDATA lpDomainData )
{
   return( fnText( eDM_XRA_TEXT, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmFileName
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmFileName
//
//  PURPOSE:    To retrieve a string representing a logical value in
//              Zeidon.  A FileName can contain only 'a'-'z', '0' - '9',
//              and '_'.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmFileName( zLONG        lEntryType,
           LPDOMAINDATA lpDomainData )
{
   return( fnText( eDM_FILENAME, lEntryType, lpDomainData ) );
}

//./ ADD NAME=fnText
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      fnText
//
//  PURPOSE:    To retrieve a string representing an attribute value
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//             -1 - Nothing done.
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 4
zLONG
fnText( zSHORT       nID,
        zLONG        lEntryType,
        LPDOMAINDATA lpDomainData )
{
   zCHAR          szWorkString[ zMAX_TEXTDOMAIN_LEN + 1 ];
   zPCHAR         pchStringToStore;
   zPLONG         lplInteger;
   zLONG          lWork;
   zPDECIMAL      lpdDecimal;
   zDECIMAL       dWork;
   LPDOMAIN       lpDomain;
   zSHORT         nSeverity;
   zSHORT         nRC;

   nRC = zDME_NOT_HANDLED;         // Default to not-handled.

   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

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

                  // If this is for the QSName domain, check for embedded
                  // spaces and special characters as they are not allowed.
                  if ( nID == eDM_QSNAME )
                  {
                     zPCHAR   pch;

                     for ( pch = (zPCHAR) lpDomainData->lpData; *pch; pch++ )
                     {
                        if ( !zisalnum( *pch ) && *pch != '_' )
                        {
                           if ( lEntryType != zDME_VALIDATE_LPDATA )
                           {
                              // "TZDME101 - AlphaNumeric chars and '_' only are allowed"
                              MessageSend( lpDomainData->zView, "TZDME022",
                                           "Data Validation",
                                           "Only AlphaNumeric and '_' chars are allowed.",
                                           zMSGQ_DOMAIN_ERROR, 0 );
                           }

                           return( zCALL_ERROR );
                        }
                     }

                  } // end if ( nID == eDM_QSNAME )...
                  else
                  if ( nID == eDM_FILENAME )
                  {
                     zPCHAR   pch;

                     for ( pch = (zPCHAR) lpDomainData->lpData; *pch; pch++ )
                     {
                        if ( !zisalnum( *pch ) && *pch != '_' && *pch != ' ' )
                        {
                           if ( lEntryType != zDME_VALIDATE_LPDATA )
                           {
                              // "TZDME101 - AlphaNumeric chars, space and '_' only are allowed"
                              MessageSend( lpDomainData->zView, "TZDME022",
                                           "Data Validation",
                                           "Only AlphaNumeric, space and  '_' chars are allowed.",
                                           zMSGQ_DOMAIN_ERROR, 0 );
                           }

                           return( zCALL_ERROR );
                        }
                     }

                  } // end if ( nID == eDM_FILENAME )
                  else
                  if ( nID == eDM_XRA_TAG || nID == eDM_XRA_TEXT )
                  {
                  // zCHAR    szTempString[ 256 ];
                     zULONG   ulMaxLth;
                     zULONG   ulLth;
                  // zPCHAR   pch;

                     if ( nID == eDM_XRA_TAG )
                        ulMaxLth = 32;
                     else
                        ulMaxLth = 254;

                     if ( lpDomainData->uMaxStringLength < ulMaxLth )
                        ulMaxLth = lpDomainData->uMaxStringLength;

                     ulLth = zstrlen( (zPCHAR) lpDomainData->lpData );
                     if ( ulLth >= zsizeof( szWorkString ) )
                        ulLth = zsizeof( szWorkString ) - 1;

                     szWorkString[ 0 ] = 0;
                     strncat_s( szWorkString, zsizeof( szWorkString ), (zPCHAR) lpDomainData->lpData, ulLth );

#if 0    // don't need this part of the domain processing anymore
                     pch = zstrchr( szWorkString, '^' );
                     while ( pch && ulLth < ulMaxLth )
                     {
                        strcpy_s( szTempString, zsizeof( szTempString ), pch );
                        strcpy_s( pch + 1, commented out, szTempString );
                        ulLth++;
                        pch = zstrchr( pch + 2, '^' );
                     }

                     if ( pch )
                     {
                        if ( lEntryType != zDME_VALIDATE_LPDATA )
                        {
                           // "TZDME??? - Maximum length exceeded"
                           MessageSend( lpDomainData->zView, "TZDME???",
                                        "Data Validation",
                                        "Maximum length exceeded for XRA.",
                                        zMSGQ_DOMAIN_ERROR, 0 );
                        }

                        return( zCALL_ERROR );
                     }

                     pch = zstrchr( szWorkString, '&' );
                     while ( pch )
                     {
                        *pch = '^';
                        pch = zstrchr( pch + 1, '&' );
                     }
#endif
                     pchStringToStore = szWorkString;
                     break;
                  }
                  else
                  if ( nID == eDM_ALPHANUMERIC )
                  {
                     // If this is for the AlphaNumeric domain, check for
                     // embedded spaces and special characters since they
                     // are not allowed.
                     zPCHAR  pch;

                     for ( pch = (zPCHAR) lpDomainData->lpData; *pch; pch++ )
                     {
                        if ( !zisalnum( *pch ) )
                        {
                           if ( lEntryType == zDME_VALIDATE_LPDATA )
                              nSeverity = -1;
                           else
                              nSeverity = 0;

                           // "TZDME022 - AlphaNumeric chars only are allowed"
                           SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 22, 0,
                                            lpDomainData->lpViewEntity->szName,
                                            lpDomainData->lpViewAttribute->szName );

                           return( zCALL_ERROR );
                        }
                     }

                  } // end if ( nID == eDM_AlphNumeric )...

                  pchStringToStore = (zPCHAR) lpDomainData->lpData;
                  if ( lpDomainData->lpData == 0 )
                     pchStringToStore = szWorkString;

                  break;

               case zTYPE_INTEGER:
                  // convert long to a string
                  lplInteger = (zPLONG) lpDomainData->lpData;
                  if ( *lplInteger != lNullInteger )
                     zltoa( *lplInteger, szWorkString, zsizeof( szWorkString ) );

                  pchStringToStore = szWorkString;
                  break;

               case zTYPE_DECIMAL:
                  // convert decimal to string
                  lpdDecimal = (zPDECIMAL) lpDomainData->lpData;
                  if ( SysCompareDecimalToNull( lpdDecimal ) != 0 )
                  {
                     ConvertDecimalToString( szWorkString,
                                             lpDomainData->lpViewAttribute,
                                             *lpdDecimal,
                                             lpDomainData->pszContextName );
                  }

                  pchStringToStore = szWorkString;
                  break;

               case zTYPE_DATETIME:
                  // convert DateTime to String
                  nRC = UfDateTimeToString( (LPDATETIME) lpDomainData->lpData,
                                            szWorkString, 18 );
                  if ( nRC == zCALL_ERROR )
                     return( nRC );

                  pchStringToStore = szWorkString;
                  break;

               default:

                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 8;

                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain,
                                   nSeverity, 1, 0, szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            if ( (zUSHORT) zstrlen( pchStringToStore ) <=
                                    lpDomainData->lpViewAttribute->ulLth - 1 )
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView,
                                            lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            pchStringToStore, 0 );
               else
                  nRC = 0;
            }
            else
            {
            // if ( lEntryType == zDME_VALIDATE_LPDATA )
                  nSeverity = -1;  // always permit set dks/jsb 006.04.06
            // else
            //    nSeverity = 8;

               // "TZDME002 - Text String exceeds attribute length "
               SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 2,
                                lpDomainData->lpViewAttribute->ulLth - 1,
                                lpDomainData->lpViewEntity->szName,
                                lpDomainData->lpViewAttribute->szName );

               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView,
                                            lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            pchStringToStore, 0 );
               else
                  nRC = zCALL_ERROR;
            }
         }
         else
         {
            if ( lEntryType == zDME_VALIDATE_LPDATA )
               nSeverity = -1;
            else
               nSeverity = 0;

            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain,
                             nSeverity, 3, 0, szWorkString,
                             lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

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

            nRC = 0;

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
                     // string to string
   //    Gig, strncpy_s NULLS for complete length!!!!
   //                strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, (zPCHAR) lpAttrAddr );
   //                strncpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, (zPCHAR) lpAttrAddr, lpDomainData->uMaxStringLength - 1 );
   //                *((zPCHAR) lpDomainData->lpData + lpDomainData->uMaxStringLength - 1 ) = 0;  no longer needed

                     *((zPCHAR) lpDomainData->lpData) = 0;
                     if ( nID == eDM_EXPAND_ENVIRONMENT )
                     {
                        szWorkString[ 0 ] = 0;
                        SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), (zPCHAR) lpAttrAddr );
                        strncat_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength,
                                   szWorkString, lpDomainData->uMaxStringLength - 1 );
                     }
                     else
                     {
                        if ( uAttrLength > lpDomainData->uMaxStringLength )
                        {
                           TraceLine( "Truncating Entity.Attribute %s.%s from %d to %d",
                                      lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName, uAttrLength, lpDomainData->uMaxStringLength );
                        // SysMessageBox( 0, "Domain Error", "Truncating attribute length - see Trace Log", -1 );
                           uAttrLength = lpDomainData->uMaxStringLength;
                        }
                        strncat_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, (zPCHAR) lpAttrAddr, lpDomainData->uMaxStringLength - 1 );
                     }

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
                     strcpy_s( szWorkString, zsizeof( szWorkString ), (zPCHAR) lpAttrAddr );
                     *((zPLONG) lpDomainData->lpData) = atol( szWorkString );
                     nRC = 0;
                  }

                  break;

               case zTYPE_DECIMAL:
                  // convert string to decimal
                  if ( (zPCHAR) lpAttrAddr == 0 ) //null string
                  {
                     SysConvertLongToDecimal( 0, (zPDECIMAL) lpDomainData->lpData );
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     SysConvertStringToDecimal( (zPCHAR) lpAttrAddr, (zPDECIMAL) lpDomainData->lpData );
                     if ( SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) == 0 )
                     {
                        nRC = -1;   // indicate attribute is null
                     }
                     else
                     {
                        nRC = 0;
                     }
                  }

                  break;

               case zTYPE_PIC:
                  // return the picture string for the given context
                  {
                     zLPCONTEXT   lpContext;

                     if ( GetContext( &lpContext, lpDomain,
                                      lpDomainData->pszContextName ) )
                     {
                        zPCHAR lpEditString = SysGetPointerFromHandle( lpContext->hEditString );

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
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                   0, szWorkString, lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

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

                  if ( nRC > 0 )
                     nRC = 1;
                  if ( nRC < 0 )
                     nRC = -1;

                  break;

               case zTYPE_INTEGER:
                  lWork = zatol( lpAttrAddr );
                  lWork -= *((zPLONG) lpDomainData->lpData);
                  nRC = (lWork == 0) ? 0 : (lWork < 0) ? -1 : 1;
                  break;

               case zTYPE_DECIMAL:
                  SysConvertStringToDecimal( lpAttrAddr, &dWork );
                  nRC = SysCompareDecimalToDecimal( &dWork,
                                                    (zPDECIMAL) lpDomainData->lpData );
                  break;

               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
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
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
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
               zltoa( *((zPLONG) lpDomainData->lpData), szWorkString, zsizeof( szWorkString ) );
               strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, szWorkString );
               nRC = 1; // indicate returning a string value
               break;

            case zTYPE_DECIMAL:
               ConvertDecimalToString( szWorkString, lpDomainData->lpViewAttribute,
                                       *((zPDECIMAL) lpDomainData->lpData), lpDomainData->pszContextName );
               strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, szWorkString );
               nRC = 1; // indicate returning a string value
               break;

            default:
               szWorkString[ 0 ] = lpDomainData->cType;
               szWorkString[ 1 ] = 0;
               strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
               strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
               // "TZDME001 - Invalid Input Data Type"
               SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0,
                                szWorkString, lpDomainData->lpViewAttribute->szName );
               nRC = -2;
         }

         break;
   }

   return( nRC );
}

//./ ADD NAME=Integer
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      Integer
//
//  PURPOSE:    To retrieve an integer representing an attribute value
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has been set
//    zCALL_ERROR - Error in call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmInteger( zLONG        lEntryType,
            LPDOMAINDATA lpDomainData )
{
   zPLONG     lplLongToStore;
   zLONG      lLongWork;
   zCHAR      sz[ 256 ];
   LPDOMAIN   lpDomain;
   zPCHAR     pch;
   zLONG      range;
   zBOOL      bCompare;
   zLPCONTEXT lpContext;
   zSHORT     nPos, nPos2;
   zCHAR      cThouComma;
   zCHAR      szThou[ 10 ];
   zCHAR      szDecPt[ 10 ];
   zLONG      lPrecision;
   zSHORT     i, j, nStart;
   zSHORT     nSeverity;
   zSHORT     nRC;

   nRC = 0;      // Default to ok status.
   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_INTEGER )
         {
            // determine input data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:

                  // Skip past any leading blanks.
                  while ( *(zPCHAR) lpDomainData->lpData == ' ' )
                     lpDomainData->lpData = (zPVOID) ((zPCHAR) lpDomainData->lpData + 1);

                  if ( lpDomainData->lpData == 0 || *((zPCHAR) lpDomainData->lpData) == 0 )
                     lplLongToStore = &lNullInteger;
                  else
                  {
                     // Convert string to long.  If the string has a minus
                     // sign or a plus sign at the beginning, we will copy it
                     // as is.  If the string has neither at the beginning, we
                     // will copy it with a plus sign.  This simplifies the
                     // validations below.
                     if ( *((zPCHAR) lpDomainData->lpData) == '-' ||
                          *((zPCHAR) lpDomainData->lpData) == '+' )
                     {
                        strcpy_s( sz, zsizeof( sz ), (zPCHAR) lpDomainData->lpData );
                     }
                     else
                     {
                        sz[ 0 ] = '+';
                        strcpy_s( sz + 1, zsizeof( sz ) - 1, (zPCHAR) lpDomainData->lpData );
                     }
                     if ( lpDomainData->pszContextName && zstrcmpi( lpDomainData->pszContextName, "Hex" ) == 0 )
                     {
                        fnHexToLong( sz + 1, &lLongWork );
                     }
                     else
                     {
                        if ( lpDomainData->pszContextName && zstrcmpi( lpDomainData->pszContextName, "hexAddress" ) == 0 )
                        {
                           fnHexToLong( sz + 1, &lLongWork );
                        }

                        else
                        {
                           // We will first validate any commas that might be a
                           // part of the integer and remove them.  The first
                           // comma must be one of the first four characters.
                           // Each additional comma must be four positions from
                           // the last.
                           nRC = 0;
                           nPos = 1;
                           SysGetNumberFormat( szThou, szDecPt, &lPrecision );
                           cThouComma = szThou[ 0 ];
                           // Loop until end of string or until an error occurs.
                           while ( sz[ nPos ] != 0 && nRC == 0 )
                           {
                              if ( sz[ nPos ] == cThouComma )
                              {
                                 if ( nPos > 4 )
                                    nRC = -1;
                                 else
                                 {
                                    // The first comma was ok, remove it and
                                    // check the rest.
                                    nPos2 = nPos;
                                    // Loop through each set of 4 characters, starting with
                                    // the first comma, and check that they are of the form,
                                    // ",XXX", where the X's are not null.  Note that we will
                                    // check that the X's are valid digits below.  Then shift
                                    // the digits but not the comma.
                                    while ( sz[ nPos ] != 0 && nRC == 0 )
                                    {
                                       if ( sz[ nPos ] != cThouComma ||
                                          sz[ nPos+1 ] == 0    ||
                                          sz[ nPos+2 ] == 0    ||
                                          sz[ nPos+3 ] == 0 )
                                       {
                                          nRC = -1;
                                       }
                                       else
                                       {
                                          sz[ nPos2 ] = sz[ nPos + 1 ];
                                          sz[ nPos2 + 1 ] = sz[ nPos + 2 ];
                                          sz[ nPos2 + 2 ] = sz[ nPos + 3 ];
                                          sz[ nPos2 + 3 ] = 0;
                                          nPos  = nPos  + 4;
                                          nPos2 = nPos2 + 3;
                                       }
                                    }
                                 }
                              }
                              else
                                 nPos++;
                           }

                           // Validate each character as a numeric digit, if things are ok to this
                           // point.
                           if ( nRC == 0 )
                           {
                              for ( nPos = 1; sz[ nPos ]; nPos++ )
                              {
                                 if ( isdigit( sz[ nPos ] ) == 0 )
                                    nRC = -1;
                              }
                           }

                           // Validate that the number entered is not greater
                           // than 2,147,483,647 (max value of an integer).
                           if ( nPos > 11 )
                              nRC = -1;
                           else
                              if ( nPos == 11 )
                              {
                                 if ( sz[ 0 ] > '2' )
                                    nRC = -1;
                                 else
                                    if ( sz[ 1 ] == '2' )
                                       if ( sz[ 2 ] > '1' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 2 ] == '1' )
                                       if ( sz[ 3 ] > '4' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 3 ] == '4' )
                                       if ( sz[ 4 ] > '7' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 4 ] == '7' )
                                       if ( sz[ 5 ] > '4' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 5 ] == '4' )
                                       if ( sz[ 6 ] > '8' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 6 ] == '8' )
                                       if ( sz[ 7 ] > '3' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 7 ] == '3' )
                                       if ( sz[ 8 ] > '6' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 8 ] == '6' )
                                       if ( sz[ 9 ] > '4' )
                                          nRC = -1;
                                 else
                                    if ( sz[ 9 ] == '4' )
                                       if ( sz[ 10 ] > '7' )
                                          nRC = -1;
                              }

                           // If an error has occurred, give error message and return.
                           if ( nRC )
                           {
                              /*zCHAR sz2[ 70 ];
                              strcpy_s( sz2, zsizeof( sz2 ), lpDomainData->lpViewEntity->szName );
                              strcat_s( sz2, zsizeof( sz2 ), ", " );
                              strcat_s( sz2, zsizeof( sz2 ), lpDomainData->lpViewAttribute->szName );
                              */

                              if ( lEntryType == zDME_VALIDATE_LPDATA )
                                 nSeverity = -1;
                              else
                                 nSeverity = 0;

                              // "TZDME024 - Invalid integer string "
                              SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 24, 0,
                                               lpDomainData->lpViewEntity->szName,
                                               lpDomainData->lpViewAttribute->szName );

                              return( zCALL_ERROR );
                           }

                           lLongWork = atol( sz );
                        }
                     }

                     lplLongToStore = (zPLONG) &lLongWork;
                  }

                  break;

               case zTYPE_INTEGER:
                  // long to long
                  lplLongToStore = (zPLONG) lpDomainData->lpData;
                  break;

               case zTYPE_DECIMAL:
                  // convert decimal to long
                  if ( SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) == 0 )
                     lplLongToStore = &lNullInteger;
                  else
                  {
                     zDECIMAL dLong;

                     SysConvertLongToDecimal( LONG_MAX, &dLong );
                     if ( SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData,
                                                      &dLong ) > 0 )
                     {
                        if ( lEntryType == zDME_VALIDATE_LPDATA )
                           nSeverity = -1;
                        else
                           nSeverity = 8;

                        //  "TZDME006 - Integer overflow"
                        SendDomainError( lpDomainData->zView, lpDomain,
                                         nSeverity, 6, 0,
                                         lpDomainData->lpViewEntity->szName,
                                         lpDomainData->lpViewAttribute->szName );
                        return( zCALL_ERROR );
                     }

                     SysConvertLongToDecimal( LONG_MIN, &dLong );
                     if ( SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData,
                                                      &dLong ) < 0 )
                     {
                        if ( lEntryType == zDME_VALIDATE_LPDATA )
                           nSeverity = -1;
                        else
                           nSeverity = 8;

                        //  "TZDME007 - Integer underflow"
                        SendDomainError( lpDomainData->zView, lpDomain,
                                         nSeverity, 7, 0,
                                         lpDomainData->lpViewEntity->szName,
                                         lpDomainData->lpViewAttribute->szName );
                        return( zCALL_ERROR );
                     }

                     SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData, &lLongWork );
                     lplLongToStore = (zPLONG) &lLongWork;
                  }

                  break;

               default:
                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 8;

                  sz[ 0 ] = lpDomainData->cType;
                  sz[ 1 ] = 0;
                  strcat_s( sz, zsizeof( sz ), ", " );
                  strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 1,
                                   0, sz, lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            // Now perform any range checking to make sure the integer value is
            // within range.  The range value is in the EditString variable and
            // needs to be parsed.

            if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) &&
                 SysGetPointerFromHandle( lpContext->hEditString ) )
            {
               zCHAR  szEditString[ 100 ];
               zPCHAR lpEditString = SysGetPointerFromHandle( lpContext->hEditString );

               strcpy_s( szEditString, zsizeof( szEditString ), lpEditString );

               // Loop for each range check in EditString.
               // At each pass of loop, copy the range check to sz.
               nStart = 0;
               for ( i = 0; szEditString[ i ] != ';' && szEditString[ i ] != 0; i++ )
               {
                  ;
               }

               for ( ; ; ) // loop until colon is found
               {
                  strncpy_s( sz, zsizeof( sz ), szEditString + nStart, i - nStart );
                  sz[ i - nStart ] = 0;

                  if ( sz[ 0 ] == '>' || sz[ 0 ] == '<' || sz[ 0 ] == '=' )
                  {
                     // First convert the range value to an integer.

                     // Stop at first nonblank after compare symbol.
                     if ( sz[ 1 ] == '=' )
                     {
                        for ( j = 0; sz[ j + 2 ] == ' '; j++ );
                        pch = sz + 2 + j;
                     }
                     else
                     {
                        for ( j = 0; sz[ j + 1 ] == ' '; j++ );
                        pch = sz + 1 + j;
                     }

                     range = zatol( pch );

                     // Next perform the correct compare.

                     if ( sz[ 0 ] == '=' )
                        bCompare = (*lplLongToStore == range);
                     else

                     if ( sz[ 0 ] == '>' && sz[ 1 ] == '=' )
                        bCompare = (*lplLongToStore >=  range);
                     else

                     if ( sz[ 0 ] == '>' )
                        bCompare = (*lplLongToStore > range);
                     else

                     if ( sz[ 0 ] == '<' && sz[ 1 ] == '=' )
                        bCompare = (*lplLongToStore <= range);
                     else

                     if ( sz[ 0 ] == '<' )
                        bCompare = (*lplLongToStore <  range);

                     if ( bCompare == 0 )
                     {
                        if ( lEntryType == zDME_VALIDATE_LPDATA )
                           nSeverity = -1;
                        else
                           nSeverity = 0;

                        *sz = lpDomainData->cType;
                        *(sz + 1) = 0;
                        strcat_s( sz, zsizeof( sz ), ", " );
                        strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );

                        // "TZDME023 - Value out of valid range"
                        SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 23,
                                         0, sz, lpDomainData->lpViewAttribute->szName );
                        return( zCALL_ERROR );
                     }
                  }

                  if ( szEditString[ i ] == ';' )
                  {
                     // Find first nonblank character after the colon.
                     for ( i++; szEditString[ i ] == ' '; i++ );
                     nStart = i;
                     // Find end of next range value.
                     for ( i = nStart; szEditString[ i ] != ';' && szEditString[ i ] != 0; i++ )
                     {
                        ;
                     }
                  }
                  else
                     break;

               } // End of EditString loop
            }

            if ( lEntryType == zDME_SET_ATTRIBUTE )
               nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                         lpDomainData->lpViewAttribute,
                                         lplLongToStore, 0 );
         }
         else
         {
            if ( lEntryType == zDME_VALIDATE_LPDATA )
               nSeverity = -1;
            else
               nSeverity = 8;

            sz[ 0 ] = lpDomainData->lpViewAttribute->cType;
            sz[ 1 ] = 0;
            strcat_s( sz, zsizeof( sz ), ", " );
            strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 3, 0,
                             sz, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_GET_VARIABLE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_INTEGER )
         {
            zPCHAR   lpAttrValue;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            if ( lpDomainData->cType != zTYPE_PIC )
            {
               nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                         &lpAttrValue, &cAttrType,
                                         &uAttrLength );
               if ( nRC )
                  return( zCALL_ERROR );

               // if there is no lpRecord, lpAttrValue returns a null...
               if ( lpAttrValue == 0 )
                  lpAttrValue = (zPCHAR) &lNullInteger;
            }

            // determine output data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  // long to string
                  if ( *((zPLONG) lpAttrValue) == lNullInteger )
                  {
                     *((zPCHAR) lpDomainData->lpData) = 0;
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     zLONG lLocal;
                     lLocal = *((zPLONG) lpAttrValue);
                     if ( lpDomainData->pszContextName && zstrcmpi( lpDomainData->pszContextName, "Hex" ) == 0 )
                     {
                        zltox( lLocal, sz, zsizeof( sz ) );
                     }
                     else
                     {
                        if ( lpDomainData->pszContextName && zstrcmpi( lpDomainData->pszContextName, "hexAddress" ) == 0 )
                        {
                           zCHAR      sz3[ 256 ];
                           unsigned char c;

                           zltox( lLocal, sz, zsizeof( sz ) );


                           // fill up with "0" at the left to the length of 8 (+ termination)
                           while ( strlen( sz ) < 8 )
                           {
                              strcpy_s( sz3, zsizeof( sz3 ), "0" );
                              strcat_s( sz3, zsizeof( sz3 ), sz );
                              strcpy_s( sz, zsizeof( sz ), sz3 );
                           }

                           // insert a ":" in the middle
                           for (i = 9; i > 4; i--)
                              sz[ i ] = sz[ i - 1 ];

                           sz[ 4 ] = ':';

                           // now, replace all lower case character with upper case...
                           for ( i = 0; i < 9; i++ )
                           {
                              c = sz[ i ];
                              if ( c >= 97 &&  // lower case 'a'
                                   c <= 102 )  // lower case 'f'
                              {
                                 c = c - 32;   // upper case 'A'
                                 sz[ i ] = c;
                              }
                           }


                        }
                        else
                        {
                           zltoa( lLocal, sz, zsizeof( sz ) );
                        }
                     }

                     // If the Context requests the inclusion of the Thousand
                     // character, add it.
                     if ( lpDomainData->pszContextName && zstrcmpi( lpDomainData->pszContextName, "INTEGER_THOUSAND" ) == 0 )
                     {
                        SysGetNumberFormat( szThou, szDecPt, &lPrecision );
                        cThouComma = szThou[ 0 ];
                        // Set pch to beginning of first digit that is not
                        // a minus sign.
                        if ( sz[ 0 ] == '-' )
                           pch = sz + 1;
                        else
                           pch = sz;
                        nPos = (zSHORT) zstrlen( pch );
                        if ( nPos > 9 )
                           j = 3;
                        else
                        if ( nPos > 6 )
                           j = 2;
                        else
                        if ( nPos > 3 )
                           j = 1;
                        else
                           j = 0;

                        *(pch + nPos + j) = *(pch + nPos);
                        while ( j > 0 )
                        {
                           // sz[ nPos + j - 1 ] = sz[ nPos - 1 ]; etc.
                           *(pch + nPos + j - 1) = *(pch + nPos - 1);
                           *(pch + nPos + j - 2) = *(pch + nPos - 2);
                           *(pch + nPos + j - 3) = *(pch + nPos - 3);
                           *(pch + nPos + j - 4) = cThouComma;
                           nPos = nPos - 3;
                           j--;
                        }
                     }

                     // Store the result and return.
                     strcpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, sz );
                     nRC = 0;
                  }

                  break;

               case zTYPE_INTEGER:
                  // long to long
                  if ( *((zPLONG) lpAttrValue) == lNullInteger )
                  {
                     *((zPLONG) lpDomainData->lpData) = 0;
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     *((zPLONG) lpDomainData->lpData) = *((zPLONG) lpAttrValue);
                     nRC = 0;
                  }

                  break;

               case zTYPE_DECIMAL:
                  // long to decimal
                  if ( *((zPLONG) lpAttrValue) == lNullInteger )
                  {
                     SysConvertLongToDecimal( 0, (zPDECIMAL) lpDomainData->lpData );
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     SysConvertLongToDecimal( *(zPLONG) lpAttrValue,
                                              (zPDECIMAL) lpDomainData->lpData );
                     nRC = 0;
                  }

                  break;

               case zTYPE_PIC:
                  // return the picture string for the given context
                  {
                     zLPCONTEXT   lpContext;

                     if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) )
                     {
                        zPCHAR lpEditString = SysGetPointerFromHandle( lpContext->hEditString );

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
                  sz[ 0 ] = lpDomainData->cType;
                  sz[ 1 ] = 0;
                  strcat_s( sz, zsizeof( sz ), ", " );
                  strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                   0, sz, lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }
         }
         else
         {
            sz[ 0 ] = lpDomainData->lpViewAttribute->cType;
            sz[ 1 ] = 0;
            strcat_s( sz, zsizeof( sz ), ", " );
            strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             sz, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_ADD_TO_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_INTEGER )
         {
            zPCHAR   lpAttrValue;
            zCHAR    cAttrType;
            zULONG   uAttrLength;
            zLONG    lWork;
            zDECIMAL  dWork1;
            zDECIMAL  dWork2;

            // Only allow string, integer or decimal types to be added.
            // If input is null, return immediately.
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  if ( lpDomainData->lpData == 0 || *((zPCHAR) lpDomainData->lpData) == 0 )
                     return( 0 );

                  break;

               case zTYPE_INTEGER:
                  if ( *((zPLONG) lpDomainData->lpData) == lNullInteger ||
                       *((zPLONG) lpDomainData->lpData) == 0L )
                  {
                     return( 0 );
                  }

                  break;

               case zTYPE_DECIMAL:
                  if ( SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) == 0 )
                  {
                     return( 0 );
                  }

                  SysConvertLongToDecimal( 0, &dWork1 );
                  if ( SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData,
                                                   &dWork1 ) == 0 )
                  {
                     return( 0 );
                  }

                  break;

               default:
                  return( zCALL_ERROR );
            }

            nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                      &lpAttrValue, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            // if there is no lpRecord, lpAttrValue returns a null...
            if ( lpAttrValue == 0 || *((zPLONG) lpAttrValue) == lNullInteger )
               SysConvertLongToDecimal( 0, &dWork1 );
            else
               SysConvertLongToDecimal( *(zPLONG) lpAttrValue, &dWork1 );

            // determine input data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  // string to long
                  SysConvertStringToDecimal( (zPCHAR) lpDomainData->lpData,
                                             &dWork2 );
                  SysAddDecimalToDecimal( &dWork1, &dWork1, &dWork2 );
                  break;

               case zTYPE_INTEGER:
                  // long to long
                  SysConvertLongToDecimal( *(zPLONG) lpDomainData->lpData,
                                           &dWork2 );
                  SysAddDecimalToDecimal( &dWork1, &dWork1, &dWork2 );
                  break;

               case zTYPE_DECIMAL:
                  // decimal to long
                  // convert decimal to long
                  SysAddDecimalToDecimal( &dWork1, &dWork1,
                                          (zPDECIMAL) lpDomainData->lpData );

                  SysConvertLongToDecimal( LONG_MAX, &dWork2 );
                  if ( SysCompareDecimalToDecimal( &dWork1, &dWork2 ) > 0 )
                  {
                     //  "TZDME006 - Integer overflow"
                     SendDomainError( lpDomainData->zView, lpDomain,
                                      8, 6, 0,
                                      lpDomainData->lpViewEntity->szName,
                                      lpDomainData->lpViewAttribute->szName );
                     return( zCALL_ERROR );
                  }

                  SysConvertLongToDecimal( LONG_MIN, &dWork2 );
                  if ( SysCompareDecimalToDecimal( &dWork1, &dWork2 ) < 0 )
                  {
                     //  "TZDME007 - Integer underflow"
                     SendDomainError( lpDomainData->zView, lpDomain,
                                      8, 7, 0,
                                      lpDomainData->lpViewEntity->szName,
                                      lpDomainData->lpViewAttribute->szName );
                     return( zCALL_ERROR );
                  }

                  break;

               default:
                  sz[ 0 ] = lpDomainData->cType;
                  sz[ 1 ] = 0;
                  strcat_s( sz, zsizeof( sz ), ", " );
                  strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                   0, sz, lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            SysConvertDecimalToLong( &dWork1, &lWork );
            lplLongToStore = &lWork;
            nRC = StoreValueInRecord( lpDomainData->zView,
                                      lpDomainData->lpViewEntity,
                                      lpDomainData->lpViewAttribute,
                                      lplLongToStore, 0 );
         }
         else
         {
            sz[ 0 ] = lpDomainData->lpViewAttribute->cType;
            sz[ 1 ] = 0;
            strcat_s( sz, zsizeof( sz ), ", " );
            strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             sz, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_COMPARE_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_INTEGER )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;
            zLONG    lAttrWork;

            nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            if ( lpAttrAddr == 0 )
               lAttrWork = lNullInteger;
            else
               lAttrWork = *((zPLONG) lpAttrAddr );

            lLongWork = 0;    // set in case lpDomainData->lpData input is null
            // determine input data type
            switch ( lpDomainData->cType )
            {
             case zTYPE_STRING:
             {
                zSHORT n = ValidInteger( (zPCHAR)lpDomainData->lpData );
                if ( lpDomainData->lpData && n == 1 )
                   lLongWork = zatol( (zPCHAR) lpDomainData->lpData );
                else
                if ( n == 0 )
                {
                   // NULL (empty) string
                   lLongWork = lNullInteger;
                }
                else
                {
                   // not empty string, but invalid integer
                   //  This comparison must NEVER be EQUAL
                   if ( lAttrWork == lNullInteger )
                      lLongWork = 0;
                   else
                      lLongWork = lNullInteger;
                }

                break;
             }

             case zTYPE_INTEGER:
               // long to long
               // The following is awful but necessary for compatibility.
               // As application programmers compared an integer attribute
               // to 0 (number) to check for NULL, we must support that.
               // The right way is to compare against 0 string, however,
               lLongWork = *((zPLONG) lpDomainData->lpData );
               if ( lLongWork != lNullInteger && lAttrWork == lNullInteger )
                   lAttrWork = 0;
               break;

             case zTYPE_DECIMAL:
               // convert decimal to long
               if ( SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) != 0 )
               {
                  zDECIMAL dWork2;

                  SysConvertLongToDecimal( LONG_MAX, &dWork2 );
                  if ( SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData,
                                                   &dWork2 ) > 0 )
                  {
                     //  "TZDME006 - Integer overflow"
                     SendDomainError( lpDomainData->zView, lpDomain,
                                      8, 6, 0,
                                      lpDomainData->lpViewEntity->szName,
                                      lpDomainData->lpViewAttribute->szName );
                     return( zCALL_ERROR );
                  }

                  SysConvertLongToDecimal( LONG_MIN, &dWork2 );
                  if ( SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData,
                                                   &dWork2 ) < 0 )
                  {
                     //  "TZDME007 - Integer underflow"
                     SendDomainError( lpDomainData->zView, lpDomain,
                                      8, 7, 0,
                                      lpDomainData->lpViewEntity->szName,
                                      lpDomainData->lpViewAttribute->szName );
                     return( zCALL_ERROR );
                  }

                  SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData, &lLongWork );
               }
               else
                  lLongWork = lNullInteger;

               break;

             default:
               sz[ 0 ] = lpDomainData->cType;
               sz[ 1 ] = 0;
               strcat_s( sz, zsizeof( sz ), ", " );
               strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
               // "TZDME001 - Invalid Input Data Type"
               SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                0, sz, lpDomainData->lpViewAttribute->szName );
               return( zCALL_ERROR );
            }

            if ( lAttrWork == lLongWork )
               nRC = 0; // this is also ok if both are NULL
            else
            if ( lAttrWork == lNullInteger )
               nRC = -1; // attribute is null
            else
            if ( lLongWork == lNullInteger )
               nRC = 1; // Comparison value is null
            else
               nRC = (lAttrWork > lLongWork) ? 1 : -1;
         }
         else
         {
            sz[ 0 ] = lpDomainData->lpViewAttribute->cType;
            sz[ 1 ] = 0;
            strcat_s( sz, zsizeof( sz ), ", " );
            strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             sz, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         return( zCALL_ERROR );

      case zDME_GET_COMPARE_VALUE:
         // determine input data type
         switch ( lpDomainData->cType )
         {
            case zTYPE_STRING:
               if ( lpDomainData->lpData && *((zPCHAR) lpDomainData->lpData ) )
               {
                  lLongWork = zatol( (zPCHAR) lpDomainData->lpData );
                  *((zPLONG) lpDomainData->lpData) = lLongWork;
                  nRC = 2; // indicate we're returning an integer
               }

               break;

            case zTYPE_INTEGER:
               nRC = 0;
               break;

            case zTYPE_DECIMAL:
               if ( SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) != 0 )
               {
                  zDECIMAL dWork2;

                  SysConvertLongToDecimal( LONG_MAX, &dWork2 );
                  if ( SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData,
                                                   &dWork2 ) > 0 )
                  {
                     //  "TZDME006 - Integer overflow"
                     SendDomainError( lpDomainData->zView, lpDomain,
                                      8, 6, 0,
                                      lpDomainData->lpViewEntity->szName,
                                      lpDomainData->lpViewAttribute->szName );
                     return( zCALL_ERROR );
                  }

                  SysConvertLongToDecimal( LONG_MIN, &dWork2 );
                  if ( SysCompareDecimalToDecimal( (zPDECIMAL) lpDomainData->lpData,
                                                   &dWork2 ) < 0 )
                  {
                     //  "TZDME007 - Integer underflow"
                     SendDomainError( lpDomainData->zView, lpDomain,
                                      8, 7, 0,
                                      lpDomainData->lpViewEntity->szName,
                                      lpDomainData->lpViewAttribute->szName );
                     return( zCALL_ERROR );
                  }

                  SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData,
                                           &lLongWork );
                  SysConvertLongToDecimal( lLongWork,
                                           (zPDECIMAL) lpDomainData->lpData );
                  nRC = 2;
               }

               break;

            default:
               sz[ 0 ] = lpDomainData->cType;
               sz[ 1 ] = 0;
               strcat_s( sz, zsizeof( sz ), ", " );
               strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
               // "TZDME001 - Invalid Input Data Type"
               SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                0, sz, lpDomainData->lpViewAttribute->szName );
               nRC = -2;
         }

         break;

      default:
         // We're not handling this case so send it back.
         nRC = zDME_NOT_HANDLED;
   }

   return( nRC );
}


// Added by Phil
int
fnHexToLong( zPCHAR  szHexString, zPLONG  lIn )
{
   zLONG  lWk;
   zCHAR  szWk[9];
   zPCHAR p;
   unsigned char c;
   int i, ii;

   lWk = 0;
   strcpy_s( szWk, zsizeof( szWk ), "00000000" );
   ii = 8 - zstrlen( szHexString );
   if ( ii < 0 ) ii = 0;
   strcpy_s( szWk + ii, zsizeof( szWk ) - ii, szHexString );


   p = (zPCHAR) &lWk;
   p = p + 3;
   for ( i = 0; i < 7; )
   {
      c = szWk[ i++ ];
      if ( c >= 97 )  // lower case 'a'
         c = c -87;
      else
      if ( c >= 65 )  // upper case 'A'
         c = c -55;
      else
         c = c -48;  // character '0'

      if ( c > 15 )
         c = 0;

      *p = c << 4;      // shift c because p is a 'signed' char
      c = szWk[ i++ ];
      if ( c >= 97 )  // lower case 'a'
         c = c -87;
      else
      if ( c >= 65 )  // upper case 'A'
         c = c -55;
      else
         c = c -48;  // character '0'

      if ( c > 15 )
         c = 0;

      *p = c | *p;
      p--;
   }

   *lIn = lWk;
   return( 0 );
}

//./ ADD NAME=zdmPassword
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmPassword
//
//  PURPOSE:    To set/retrieve and encrypted string.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmPassword( zLONG        lEntryType,
             LPDOMAINDATA lpDomainData )
{
   zCHAR          szWorkString[ 40 ];
   zPCHAR         pchStringToStore;
   LPDOMAIN       lpDomain;
   zSHORT         nSeverity;
   zSHORT         nRC;

// TraceLineS( "zdmPassword Domain Call", "" );
   nRC = zCALL_ERROR;         // Default to in error status...
   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

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
                  pchStringToStore = (zPCHAR) szWorkString;
                  UfEncryptString( szWorkString, (zPCHAR) lpDomainData->lpData,
                                   (zUSHORT) (lpDomainData->lpViewAttribute->ulLth - 1) );
                  break;

               default:
                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 8;

                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 1,
                                   0, szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            if ( (zUSHORT) zstrlen( pchStringToStore ) <=
                                                lpDomainData->lpViewAttribute->ulLth - 1 )
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            pchStringToStore, 0 );
               else
                  nRC = 0;
            }
            else
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
               {
                  nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            pchStringToStore, 0 );
               }

               if ( lEntryType == zDME_VALIDATE_LPDATA )
                  nSeverity = -1;
               else
                  nSeverity = 8;

               // "TZDME002 - Text String exceeds attribute length "
               SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 2,
                                lpDomainData->lpViewAttribute->ulLth - 1,
                                lpDomainData->lpViewEntity->szName,
                                lpDomainData->lpViewAttribute->szName );
               return( zCALL_ERROR );
            }
         }
         else
         {
            if ( lEntryType == zDME_VALIDATE_LPDATA )
               nSeverity = -1;
            else
               nSeverity = 8;

            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_GET_VARIABLE:

         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            if ( lpDomainData->cType != zTYPE_PIC )
            {
               nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                         &lpAttrAddr, &cAttrType, &uAttrLength );
               if ( nRC )
                  return( zCALL_ERROR );
            }

            // Determine input data type.
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  if ( lpAttrAddr == 0 || *lpAttrAddr == 0 )  // null string
                  {
                     *((zPCHAR) lpDomainData->lpData) = 0;
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     // string to string
                     UfDecryptString( (zPCHAR) lpDomainData->lpData,
                                      (zPCHAR) lpAttrAddr,
                                      (zUSHORT) (lpDomainData->lpViewAttribute->ulLth - 1) );
                     nRC = 0;
                  }

                  break;

               case zTYPE_PIC:
               {
                  // Return the picture string for the given context.
                  zLPCONTEXT   lpContext;

                  if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) )
                  {
                     zPCHAR lpEditString = SysGetPointerFromHandle( lpContext->hEditString );

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

                  break;
               }

               case zTYPE_INTEGER:
               case zTYPE_DECIMAL:
               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                   0, szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            nRC = 0;
         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_ADD_TO_ATTRIBUTE:
         break;

      case zDME_COMPARE_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zPCHAR   lpOrigValue;   // Pointer to original unconverted data
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            lpOrigValue = (zPCHAR) lpDomainData->lpData;
            nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            UfEncryptString( szWorkString, (zPCHAR) lpDomainData->lpData,
                             (zUSHORT) (lpDomainData->lpViewAttribute->ulLth - 1) );
            if ( lpAttrAddr == 0 )
               lpAttrAddr = szNullS;

            // determine input data type
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:

                  // We will first make the compare on the original, unencrypted
                  // lpDomainData->lpData value that was passed in. If we don't get a match, we
                  // will encrypt the value and redo the compare. This allows the
                  // value passed in to be either before or after encryption.
                  nRC = zstrcmp( lpAttrAddr, (zPCHAR) lpOrigValue );
                  if ( nRC )
                     nRC = zstrcmp( lpAttrAddr, szWorkString );

                  if ( nRC > 0 )
                     nRC = 1;
                  if ( nRC < 0 )
                     nRC = -1;

                  break;

               case zTYPE_INTEGER:
               case zTYPE_DECIMAL:
               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0,
                                   szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }
         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         return( zCALL_ERROR );

      case zDME_GET_COMPARE_VALUE:
         switch ( lpDomainData->cType )
         {
            case zTYPE_STRING:
               nRC = -2;
               break;

            case zTYPE_INTEGER:
            case zTYPE_DECIMAL:
            default:
               szWorkString[ 0 ] = lpDomainData->cType;
               szWorkString[ 1 ] = 0;
               strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
               strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
               // "TZDME001 - Invalid Input Data Type"
               SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0,
                                szWorkString,
                                lpDomainData->lpViewAttribute->szName );
               nRC = -2;
         }

         break;

      default:
         // We're not handling this case so send it back.
         nRC = zDME_NOT_HANDLED;
   }

   return( nRC );
}

//./ ADD NAME=zdmPasswordUL
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmPasswordUL
//
//  PURPOSE:    To set/retrieve and encrypted string.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmPasswordUL( zLONG        lEntryType,
               LPDOMAINDATA lpDomainData )
{
   zCHAR          szWorkString[ zMAXEL + 1 ];
   zPCHAR         pchStringToStore;
   LPDOMAIN       lpDomain;
   zSHORT         nSeverity;
   zSHORT         nRC;

// TraceLineS( "zdmPasswordUL Domain Call", "" );
   nRC = zCALL_ERROR;         // Default to in error status...
   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

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
                  pchStringToStore = (zPCHAR) szWorkString;
                  UfEncryptStringUL( szWorkString, (zPCHAR) lpDomainData->lpData,
                                     0, zMAXEL );
                  break;

               default:
                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 8;

                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 1,
                                   0, szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            if ( (zUSHORT) zstrlen( pchStringToStore ) <=
                                                lpDomainData->lpViewAttribute->ulLth - 1 )
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
                  nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            pchStringToStore, 0 );
               else
                  nRC = 0;
            }
            else
            {
               if ( lEntryType == zDME_VALIDATE_LPDATA )
                  nSeverity = -1;
               else
                  nSeverity = 8;

               if ( lEntryType == zDME_SET_ATTRIBUTE )
               {
                  nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                            lpDomainData->lpViewAttribute,
                                            pchStringToStore, 0 );
               }

               // "TZDME002 - Text String exceeds attribute length "
               SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 2,
                                lpDomainData->lpViewAttribute->ulLth - 1,
                                lpDomainData->lpViewEntity->szName,
                                lpDomainData->lpViewAttribute->szName );
               return( zCALL_ERROR );
            }
         }
         else
         {
            if ( lEntryType == zDME_VALIDATE_LPDATA )
               nSeverity = -1;
            else
               nSeverity = 8;

            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_GET_VARIABLE:

         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            if ( lpDomainData->cType != zTYPE_PIC )
            {
               nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                         &lpAttrAddr, &cAttrType, &uAttrLength );
               if ( nRC )
                  return( zCALL_ERROR );
            }

            // Determine input data type.
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:
                  if ( lpAttrAddr == 0 || *lpAttrAddr == 0 )  // null string
                  {
                     *((zPCHAR) lpDomainData->lpData) = 0;
                     nRC = -1;   // indicate attribute is null
                  }
                  else
                  {
                     // string to string
                     UfDecryptStringUL( (zPCHAR) lpDomainData->lpData,
                                        (zPCHAR) lpAttrAddr, 0,
                                        (zUSHORT) (lpDomainData->lpViewAttribute->ulLth - 1) );
                     nRC = 0;
                  }

                  break;

               case zTYPE_PIC:
               {
                  // Return the picture string for the given context.
                  zLPCONTEXT   lpContext;

                  if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) )
                  {
                     zPCHAR lpEditString = SysGetPointerFromHandle( lpContext->hEditString );

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

                  break;
               }

               case zTYPE_INTEGER:
               case zTYPE_DECIMAL:
               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1,
                                   0, szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }

            nRC = 0;
         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         return( nRC );

      case zDME_ADD_TO_ATTRIBUTE:
         break;

      case zDME_COMPARE_ATTRIBUTE:
         if ( lpDomainData->lpViewAttribute->cType == zTYPE_STRING )
         {
            zPCHAR   lpAttrAddr;
            zPCHAR   lpOrigValue;   // pointer to original unconverted data
            zCHAR    cAttrType;
            zULONG   uAttrLength;

            lpOrigValue = (zPCHAR) lpDomainData->lpData;
            nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                      &lpAttrAddr, &cAttrType, &uAttrLength );
            if ( nRC )
               return( zCALL_ERROR );

            UfEncryptStringUL( szWorkString, (zPCHAR) lpDomainData->lpData,
                               0, zMAXEL );
            if ( lpAttrAddr == 0 )
               lpAttrAddr = szNullS;

            // Determine input data type.
            switch ( lpDomainData->cType )
            {
               case zTYPE_STRING:

                  // We will first make the compare on the original, unencrypted
                  // lpDomainData->lpData value that was passed in. If we don't get
                  // a match, we will encrypt the value and redo the compare. This
                  // allows the value passed in to be either pre or post encryption.
                  nRC = zstrcmp( lpAttrAddr, (zPCHAR) lpOrigValue );
                  if ( nRC )
                     nRC = zstrcmp( lpAttrAddr, szWorkString );

                  if ( nRC > 0 )
                     nRC = 1;
                  else
                  if ( nRC < 0 )
                     nRC = -1;

                  break;

               case zTYPE_INTEGER:
               case zTYPE_DECIMAL:
               default:
                  szWorkString[ 0 ] = lpDomainData->cType;
                  szWorkString[ 1 ] = 0;
                  strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
                  strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
                  // "TZDME001 - Invalid Input Data Type"
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0,
                                   szWorkString,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
            }
         }
         else
         {
            szWorkString[ 0 ] = lpDomainData->lpViewAttribute->cType;
            szWorkString[ 1 ] = 0;
            strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
            strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );

            // "TZDME003 - Attribute Type invalid for this Domain"
            SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0,
                             szWorkString, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         return( zCALL_ERROR );

      case zDME_GET_COMPARE_VALUE:
         switch ( lpDomainData->cType )
         {
            case zTYPE_STRING:
               nRC = -2;
               break;

            case zTYPE_INTEGER:
            case zTYPE_DECIMAL:
            default:
               szWorkString[ 0 ] = lpDomainData->cType;
               szWorkString[ 1 ] = 0;
               strcat_s( szWorkString, zsizeof( szWorkString ), ", " );
               strcat_s( szWorkString, zsizeof( szWorkString ), lpDomainData->lpViewEntity->szName );
               // "TZDME001 - Invalid Input Data Type"
               SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0,
                                szWorkString,
                                lpDomainData->lpViewAttribute->szName );
               nRC = -2;
         }

         break;

      default:
         // We're not handling this case so send it back.
         nRC = zDME_NOT_HANDLED;
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
fnDyamicT_fnLocalBuildQual_0( zVIEW     vSubtask,
                              zPVIEW    vQualObject,
                              zPCHAR    pchContextName );

//./ ADD NAME=DynamicTable
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      DynamicTable
//
//  PURPOSE:    To process a Dynamic Table Domain
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zLONG OPERATION
zdmDynamicTable( zLONG        lEntryType,
                 LPDOMAINDATA lpDomainData )
{
   zVIEW       vDomainTable;
   zCHAR       szSavedTableName[ 36 ];
   zCHAR       szStringAttributeValue[ 65 ];
   zCHAR       szMsg[ 100 ];
   zLONG       lIntegerAttributeValue;
   zLONG       lWork = 0;
   zVIEW       vTempViewVar_0;
   LPDOMAIN    lpDomain;
   zLPCONTEXT  lpContext;
   zPCHAR      lpContextName;
   zPVOID      lpInternalValue;
   zCHAR       cAttrType;
   zULONG      uAttrLength;
   zDECIMAL    dTempValue;
   zLONG       lTempValue;
   zSHORT      nSeverity;
   zSHORT      nRC;

   if ( lEntryType == zDME_SET_ATTR_TO_ATTR )
      return( zDME_NOT_HANDLED );

   // Activate the Dynamic Table and name it, if it does not currently
   // exist in memory with a name of "X_" + Domain Name.
   strcpy_s( szSavedTableName, zsizeof( szSavedTableName ), "X_" );

   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );
   if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
   {
      strcat_s( szSavedTableName, zsizeof( szSavedTableName ), lpDomainData->pszContextName );
      lpContextName = lpDomainData->pszContextName;
   }
   else
   {
      GetDefaultContext( &lpContext, lpDomain );
      strcat_s( szSavedTableName, zsizeof( szSavedTableName ), lpContext->szName );
      lpContextName = lpContext->szName;
   }

   nRC = GetViewByName( &vDomainTable, szSavedTableName, lpDomainData->zView, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zCHAR szMutexName[ 500 ];

      // Make sure we are the only ones accessing the application OI.
      sprintf_s( szMutexName, zsizeof( szMutexName ), "DomainMutex_%s", szSavedTableName );
      SysMutexLock( lpDomainData->zView, szMutexName, 0, 0 );

      nRC = GetViewByName( &vDomainTable, szSavedTableName, lpDomainData->zView, zLEVEL_APPLICATION );

      if ( nRC < 0 )
      {
         fnDyamicT_fnLocalBuildQual_0( lpDomainData->zView, &vTempViewVar_0, lpContextName );
         nRC = ActivateObjectInstance( &vDomainTable, "DOMAINT", lpDomainData->zView, vTempViewVar_0, zSINGLE | zLEVEL_APPLICATION );
         if ( nRC >= 0 )
         {
            SetNameForView( vDomainTable, szSavedTableName, 0, zLEVEL_APPLICATION );
         }
         else
         {
            SysMutexUnlock( lpDomainData->zView, szMutexName, 0 );

            strcpy_s( szMsg, zsizeof( szMsg ), "Validation table, " );
            strcat_s( szMsg, zsizeof( szMsg ), szSavedTableName );
            strcat_s( szMsg, zsizeof( szMsg ), ", cannot be loaded." );
            MessageSend( lpDomainData->zView, "TZDMD002", "Data Validation", szMsg, zMSGQ_DOMAIN_ERROR, 0 );
            return( zCALL_ERROR );
         }
      }

      CreateViewFromViewForTask( &vDomainTable, vDomainTable, 0 );
      SetNameForView( vDomainTable, szSavedTableName, lpDomainData->zView, zLEVEL_TASK );

      SysMutexUnlock( lpDomainData->zView, szMutexName, 0 );
   }

   nRC = 0;
   switch( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
         //
         // if the lpDomainData->lpData input is null or points to a null char, skip the
         // conversions, just null the attribute
         //
         if ( lpDomainData->lpData == 0 ||
              (lpDomainData->cType == zTYPE_STRING && *((zPCHAR) lpDomainData->lpData) == 0) ||
              (lpDomainData->cType == zTYPE_INTEGER && *((zPLONG) lpDomainData->lpData) == lNullInteger) ||
              (lpDomainData->cType == zTYPE_DECIMAL &&
              SysCompareDecimalToNull( (zPDECIMAL) lpDomainData->lpData ) == 0) )
         {
            if ( lpDomainData->lpViewAttribute->cType == zTYPE_INTEGER )
               lpInternalValue = &lNullInteger;
            else
            {
               if ( lpDomainData->lpViewAttribute->cType == zTYPE_DECIMAL )
               {
                  lpInternalValue = &lNullInteger;
                  SysAssignDecimalFromNull( (zPDECIMAL) lpInternalValue );
               }
               else
                  lpInternalValue = szNullS;
            }

            if ( lEntryType == zDME_SET_ATTRIBUTE )
               nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                         lpDomainData->lpViewAttribute,
                                         lpInternalValue, 0 );
            else
               nRC = 0;

            return( nRC );
         }

         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
         {

            // A Context name was passed, so use data value passed in as an external value.

            nRC = SetCursorFirstEntityByString( vDomainTable, "DomainValue", "ExternalDescription", (zPCHAR) lpDomainData->lpData, "" );
            if ( nRC >= zCURSOR_SET )
            {
               if ( lEntryType == zDME_SET_ATTRIBUTE )
               {
                  if ( lpDomain->cType == zTYPE_STRING )
                  {
                     GetStringFromAttribute( szStringAttributeValue, zsizeof( szStringAttributeValue ), vDomainTable, "DomainValue", "InternalStringValue" );
                     StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                         lpDomainData->lpViewAttribute, szStringAttributeValue, 0 );
                  }

                  if ( lpDomain->cType == zTYPE_INTEGER )
                  {
                     GetIntegerFromAttribute( &lIntegerAttributeValue, vDomainTable, "DomainValue", "InternalIntegerValue" );
                     StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                         lpDomainData->lpViewAttribute, &lIntegerAttributeValue, 0 );
                  }
               }
            }
            else
            {
               if ( lEntryType != zDME_VALIDATE_LPDATA )
               {
                  nRC = MessageSend( lpDomainData->zView, "TZDMD003", "Data Validation", "Invalid input value", zMSGQ_DOMAIN_ERROR, 0 );
               }

               return( zCALL_ERROR );
            }
         }
         else
         {
            // No Context name was passed, treat the value passed as an internal value.

            switch ( lpDomainData->cType )
            {
            case zTYPE_STRING:
               switch ( lpDomainData->lpViewAttribute->cType )
               {
               case zTYPE_STRING:

                  nRC = SetCursorFirstEntityByString( vDomainTable, "DomainValue", "InternalStringValue", (zPCHAR) lpDomainData->lpData, "" );
                  if ( nRC >= zCURSOR_SET )
                  {
                     lpInternalValue = lpDomainData->lpData;
                     break;
                  }

                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 0;

                  // "TZDME025 - Invalid table value "
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 25, 0,
                                   lpDomainData->lpViewEntity->szName,
                                   lpDomainData->lpViewAttribute->szName );
                  TraceLineS( "TZDME025 attempting to set value: ", (zPCHAR) lpDomainData->lpData );
                  return( zCALL_ERROR );

               case zTYPE_INTEGER:
               case zTYPE_DECIMAL:
                  // The value coming in (lpDomainData->lpData) is string.
                  // Since the attribute type is integer or decimal, the
                  // table type is integer (internal table types are only
                  // stored as strings and integers).
                  lWork = zatol( (zPCHAR) lpDomainData->lpData );
                  nRC = SetCursorFirstEntityByInteger( vDomainTable, "DomainValue", "InternalIntegerValue", lWork, "" );
                  if ( nRC >= zCURSOR_SET )
                  {
                     if ( lpDomainData->lpViewAttribute->cType == zTYPE_DECIMAL )
                     {
                        LPTASK lpTask = SfGetCurrentTask( lpDomainData->zView );
                        lpInternalValue = 0;  // initialize to something DKS 2000.03.09?
                        SysConvertLongToDecimal( lWork, &dTempValue );
                        SysAssignDecimalFromDecimal( (zPDECIMAL) lpInternalValue, &dTempValue );
                     }
                     else
                        lpInternalValue = &lWork;

                     break;
                  }

                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 0;

                  // "TZDME025 - Invalid table value "
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 25, 0,
                                   lpDomainData->lpViewEntity->szName,
                                   lpDomainData->lpViewAttribute->szName );
                  TraceLineS( "TZDME025 attempting to set invalid decimal value", "" );
                  return( zCALL_ERROR );

               default:
                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 0;

                  // "TZDME003 - Attribute type invalid for this Domain "
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 3, 0,
                                   lpDomainData->lpViewEntity->szName,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
               }

               break;

            case zTYPE_INTEGER:
            case zTYPE_DECIMAL:
               if ( lpDomainData->lpViewAttribute->cType != zTYPE_INTEGER && lpDomainData->lpViewAttribute->cType != zTYPE_DECIMAL )
               {
                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 0;

                  // "TZDME003 - Attribute type invalid for this Domain "
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 3, 0,
                                   lpDomainData->lpViewEntity->szName,
                                   lpDomainData->lpViewAttribute->szName );
                  return( zCALL_ERROR );
               }

               // Decimal and integer values are treated the same in table lookup.
               if ( lpDomainData->lpViewAttribute->cType == zTYPE_INTEGER )
                  lTempValue = (zLONG) *((zPLONG)lpDomainData->lpData);
               else
                  SysConvertDecimalToLong( (zPDECIMAL) lpDomainData->lpData, &lTempValue );

               // We will always treat the setting of a table attribute value from
               // an integer or decimal as setting it from an internal value,
               // whether or not a Context Name is passed in.

               nRC = SetCursorFirstEntityByInteger( vDomainTable, "DomainValue", "InternalStringValue", lTempValue, "" );
               if ( nRC >= zCURSOR_SET )
               {
                  lpInternalValue = lpDomainData->lpData;
                  break;
               }
               else
               {
                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 0;

                  // "TZDME025 - Invalid table value "
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 25, 0,
                                   lpDomainData->lpViewEntity->szName,
                                   lpDomainData->lpViewAttribute->szName );
                  TraceLineI( "TZDME025 attempting to set invalid integer value: ",
                              lTempValue );
                  return( zCALL_ERROR );
               }

            default:
               if ( lEntryType == zDME_VALIDATE_LPDATA )
                  nSeverity = -1;
               else
                  nSeverity = 0;

               // "TZDME001 - Invalid input data type "
               SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 1, 0,
                                lpDomainData->lpViewEntity->szName,
                                lpDomainData->lpViewAttribute->szName );
               return( zCALL_ERROR );
            }

            // Now store the value.
            if ( lEntryType == zDME_SET_ATTRIBUTE )
               nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                         lpDomainData->lpViewAttribute, lpInternalValue, 0 );
            else
               nRC = 0;

         }

         break;

      case zDME_GET_VARIABLE:
         if ( lpDomain->cType == zTYPE_STRING )
         {
            nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                      (zPVOID) &lpInternalValue, &cAttrType, &uAttrLength );
            if ( nRC || (zPCHAR) lpInternalValue == 0 || *(zPCHAR)lpInternalValue == 0 )
               return( zCALL_ERROR );

            nRC = SetCursorFirstEntityByString( vDomainTable, "DomainValue", "InternalStringValue", (zPCHAR) lpInternalValue, "" );
         }
         else
         if ( lpDomain->cType == zTYPE_INTEGER )
         {
            nRC = GetValueFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute,
                                      (zPVOID) &lpInternalValue, &cAttrType, &uAttrLength );
            if ( nRC || lpInternalValue == 0 || *(zPCHAR)lpInternalValue == 0 )
               return( zCALL_ERROR );

            lIntegerAttributeValue = *(zPLONG)lpInternalValue;
            nRC = SetCursorFirstEntityByInteger( vDomainTable, "DomainValue", "InternalIntegerValue", lIntegerAttributeValue, "" );
         }
         else
            nRC = -1;
         if ( nRC >= zCURSOR_SET )
         {
            nRC = GetStringFromAttribute( lpDomainData->lpData, lpDomainData->uMaxStringLength, vDomainTable, "DomainValue", "ExternalDescription" );
         }

         break;

      case zDME_GET_COMPARE_VALUE:
         if ( lpDomain->cType == zTYPE_STRING )
         {
            // Check to see if it is already an internal value
            nRC = SetCursorFirstEntityByString( vDomainTable, "DomainValue", "InternalStringValue", (zPCHAR) lpDomainData->lpData, "" );
            if ( nRC >= zCURSOR_SET )
               return( 0 );

            // No, assume it is an external value
            nRC = SetCursorFirstEntityByString( vDomainTable, "DomainValue", "ExternalDescription", (zPCHAR) lpDomainData->lpData, "" );
            if ( nRC < zCURSOR_SET )
            {
               // neither internal nor external value, this is an error
               TraceLineS( "Domain name = ", lpDomain->szName );
               TraceLineS( "External value = ", (zPCHAR) lpDomainData->lpData );
               MessageSend( lpDomainData->zView, "TZDMD002", "Data Validation",
                            "Invalid external value for dynamic domain.",
                            zMSGQ_DOMAIN_ERROR, 0 );
               return( zCALL_ERROR );
            }

            GetStringFromAttribute( lpDomainData->lpData, lpDomainData->uMaxStringLength, vDomainTable, "DomainValue", "InternalStringValue" );

            nRC = 1;  // Returning a string.
         }
         else
         if ( lpDomain->cType == zTYPE_INTEGER )
         {
            // We get here if we have an int based table domain and
            // the search field is string. In this case the lpData
            // points to a string which hast to be interpreted as
            // an external value. It cannot point to anything,
            // that might be interpreted as external value.
            //
            nRC = SetCursorFirstEntityByString( vDomainTable, "DomainValue", "ExternalDescription", (zPCHAR) lpDomainData->lpData, "" );
            if ( nRC < zCURSOR_SET )
            {
               lIntegerAttributeValue = 0;  // initialize to something DKS 2000.03.09?
               TraceLineS( "Domain name = ", lpDomain->szName );
               TraceLineI( "External value = ", lIntegerAttributeValue );
               MessageSend( lpDomainData->zView, "TZDMD002", "Data Validation",
                            "Invalid external value for dynamic domain.",
                            zMSGQ_DOMAIN_ERROR, 0 );
               return( zCALL_ERROR );
            }

            GetIntegerFromAttribute( (zPLONG) lpDomainData->lpData,
                                     vDomainTable,
                                     "DomainValue",
                                     "InternalIntegerValue" );

            nRC = 2;  // Returning an integer.
         }
         else
            return( zCALL_ERROR );

         break;

      case zDME_COMPARE_ATTRIBUTE:
         // Get internal value of attribute and simply compare to the string
         // or integer passed in.
         if ( lpDomain->cType == zTYPE_STRING )
         {
            GetStringFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                 lpDomainData->lpViewAttribute,
                                 szStringAttributeValue, 64 );
            nRC = zstrcmp( (zPCHAR) lpDomainData->lpData, szStringAttributeValue );

            if ( nRC > 0 )
               nRC = 1;

            if ( nRC < 0 )
               nRC = -1;
         }
         else
         if ( lpDomain->cType == zTYPE_INTEGER )
         {
            GetValueFromRecord( lpDomainData->zView,
                                lpDomainData->lpViewEntity,
                                lpDomainData->lpViewAttribute,
                                (zPVOID) &lpInternalValue,
                                &cAttrType, &uAttrLength );
            if ( *((zPLONG) lpDomainData->lpData) == *((zPLONG) lpInternalValue) )
               nRC = 1;
            else
               nRC = 0;
         }

         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
         nRC = SetCursorFirstEntity( vDomainTable, "DomainValue", "" );
         if ( nRC >= zCURSOR_SET )
         {
            nRC = GetStringFromAttribute( lpDomainData->lpData, lpDomainData->uMaxStringLength, vDomainTable, "DomainValue", "ExternalDescription" );
            nRC = 0;
         }
         else
         {
            nRC = nRC;
         }

         break;

      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
         nRC = SetCursorNextEntity( vDomainTable, "DomainValue", "" );
         if ( nRC >= zCURSOR_SET )
         {
            nRC = GetStringFromAttribute( lpDomainData->lpData, lpDomainData->uMaxStringLength, vDomainTable, "DomainValue", "ExternalDescription" );
            nRC = 0;
         }
         else
         {
            nRC = nRC;
         }

         break;

      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
         nRC = SetCursorNextEntity( vDomainTable, "DomainValue", "" );
         break;

      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         nRC = SetCursorPrevEntity( vDomainTable, "DomainValue", "" );
         break;

      case zDME_GET_ATTR_DISPLAY_LENGTH:
      {
         // get the max length of the external value out of the View OD
         LPVIEWENTITY lpViewEntity;
         LPVIEWATTRIB lpViewAttrib;
         LPVIEWOD lpViewOD =
            (LPVIEWOD) MiGetViewEntityForView( vDomainTable, 0 );
         if ( lpViewOD == 0 )
         {
            nRC = zCALL_ERROR; // should not get here
            break;
         }

         // go to DomainValue.ExternalDescription in OD
         lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );
         while ( zstrcmp( lpViewEntity->szName, "DomainValue" ) != 0 )
            lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );

         if ( lpViewEntity == 0 )
         {
            nRC = zCALL_ERROR; // should not get here
            break;
         }

         lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewEntity->hFirstOD_Attrib );
         while ( zstrcmp( lpViewAttrib->szName, "ExternalDescription" ) != 0 )
            lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );

         if ( lpViewAttrib == 0 )
         {
            nRC = zCALL_ERROR; // should not get here
            break;
         }

         // return the OD length of DomainValue.ExternalDescription
         *(zULONG *)(lpDomainData->lpData) = lpViewAttrib->ulLth;
         nRC = 0;
         break;
      }

      default:
         // We're not handling this case so send it back.
         nRC = zDME_NOT_HANDLED;
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
fnDyamicT_fnLocalBuildQual_0( zVIEW     vSubtask,
                              zPVIEW    vQualObject,
                              zPCHAR    szContextName )
{
   SfActivateSysEmptyOI( vQualObject, "KZDBHQUA", vSubtask, zMULTIPLE );
   CreateEntity( *vQualObject, "EntitySpec", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "EntitySpec", "EntityName", "Domain" );
   CreateEntity( *vQualObject, "QualAttrib", zPOS_AFTER );
   SetAttributeFromString( *vQualObject, "QualAttrib", "EntityName", "Domain" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "AttributeName", "Name" );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Value", szContextName );
   SetAttributeFromString( *vQualObject, "QualAttrib", "Oper", "=" );
   return( 0 );
}

//./ ADD NAME=DynamicTableM
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      DynamicTableM
//
//  PURPOSE:    To process a Dynamic Table Domain with multiple Domain
//              tables.
//
//              In this routine, the EditString of the Domain Context defines
//              the two Attribute names which will hold the internal table
//              value and the external table value.  The two names are separated
//              by a slash ("/"). The Context name is used for the name of the
//              Object to be activated.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has set
//    zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT zLONG OPERATION
zdmDynamicTableM( zLONG        lEntryType,
                  LPDOMAINDATA lpDomainData )

{
   zVIEW      vDomainTable;
   zSHORT     nRC;
   zSHORT     ndx;
   zCHAR      szInternalAttributeName[ 33 ];
   zCHAR      szExternalAttributeName[ 33 ];
   zPCHAR     lpWorkString;
   LPDOMAIN   lpDomain;
   zLPCONTEXT lpContext;
   zPCHAR     lpTempReturn;
   zCHAR      szSavedTableName[ 36 ];
   zCHAR      szStringAttributeValue[ 65 ];
   zLONG      lIntegerAttributeValue;
   zPCHAR     lpInternalValue;
   zCHAR      cAttrType;
   zULONG     uAttrLength;

   /////////
   // Get the Attribute names for the Internal and External table value
   // from the EditString in the Context.
   /////////

   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );
   if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) )
   {
      lpWorkString = SysGetPointerFromHandle( lpContext->hEditString );
   }
   else
   {
      // "TZDME008 - Could not find context for Domain "
      SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0,
                       TrueName( lpDomainData->pszContextName, zSHOW_ZKEY ),
                       lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   // Parse the lpWorkString to get Internal and External table value names.
   szInternalAttributeName[ 0 ] = 0;
   szExternalAttributeName[ 0 ] = 0;
   ndx = 0;
   if ( lpWorkString != 0 )
   {
      while ( *lpWorkString != 0 && *lpWorkString != '/' )
      {
         szInternalAttributeName[ ndx++ ] = *(lpWorkString++);
      }

      szInternalAttributeName[ ndx ] = 0;

      if ( *lpWorkString == '/' )
      {
         ndx = 0;
         lpWorkString++;
         while ( *lpWorkString != 0 )
         {
            szExternalAttributeName[ ndx++ ] = *(lpWorkString++);
         }
         szExternalAttributeName[ ndx ] = 0;
      }
   }

   /////////
   // Activate the Dynamic Table and name it, if it does not currently
   // exist in memory with a name of "X_" + Domain Name.
   /////////
   strcpy_s( szSavedTableName, zsizeof( szSavedTableName ), "X_" );
   strcat_s( szSavedTableName, zsizeof( szSavedTableName ), lpDomainData->pszContextName );
   nRC = GetViewByName( &vDomainTable, szSavedTableName, lpDomainData->zView, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      nRC = ActivateObjectInstance( &vDomainTable, lpDomainData->pszContextName, lpDomainData->zView,
                                    0, zACTIVATE_ROOTONLY_MULTIPLE );
      if ( nRC >= 0 )
      {
         SetNameForView( vDomainTable, szSavedTableName, 0, zLEVEL_TASK );
      }
      else
      {
         MessageSend( lpDomainData->zView, "TZDMD002", "Data Validation",
                      "Validation table cannot be loaded.", zMSGQ_DOMAIN_ERROR, 0 );
         return( zCALL_ERROR );
      }

   }

   // Validate that the entity name and the two attribute names exist.
   // We will do this by simply issueing a GetValue for each attribute.
   nRC = GetAddrForAttribute( &lpTempReturn,
                              vDomainTable,
                              lpDomainData->pszContextName,
                              szInternalAttributeName );
   if ( nRC < -1 )
   {
      MessageSend( lpDomainData->zView, "TZDMD004", "Data Validation",
                   "Internal Attribute definition is in error.", zMSGQ_DOMAIN_ERROR, 0 );
      return( zCALL_ERROR );
   }

   nRC = GetAddrForAttribute( &lpTempReturn,
                              vDomainTable,
                              lpDomainData->pszContextName,
                              szExternalAttributeName );
   if ( nRC < -1 )
   {
      MessageSend( lpDomainData->zView, "TZDMD005", "Data Validation",
                   "External Attribute definition is in error.", zMSGQ_DOMAIN_ERROR, 0 );
      return( zCALL_ERROR );
   }

   // Now process the actual domain request.
   nRC = 0;
   switch( lEntryType )
   {
      case zDME_SET_ATTRIBUTE:

         // For String Values.
         nRC = SetCursorFirstEntityByString( vDomainTable,
                                             lpDomainData->pszContextName,
                                             szExternalAttributeName,
                                             (zPCHAR) lpDomainData->lpData, "" );
         if ( nRC >= zCURSOR_SET )
         {
            if ( lpDomain->cType == zTYPE_STRING )
            {
               GetStringFromAttribute( szStringAttributeValue, zsizeof( szStringAttributeValue ),
                                       vDomainTable, lpDomainData->pszContextName, szInternalAttributeName );
               StoreValueInRecord( lpDomainData->zView,
                                   lpDomainData->lpViewEntity,
                                   lpDomainData->lpViewAttribute,
                                   szStringAttributeValue, 0 );
            }

            if ( lpDomain->cType == zTYPE_INTEGER )
            {
               GetIntegerFromAttribute( &lIntegerAttributeValue,
                                       vDomainTable, lpDomainData->pszContextName, szInternalAttributeName );
               StoreValueInRecord( lpDomainData->zView,
                                   lpDomainData->lpViewEntity,
                                   lpDomainData->lpViewAttribute,
                                   &lIntegerAttributeValue, 0 );
            }
         }
         else
         {
            nRC = MessageSend( lpDomainData->zView, "TZDMD003", "Data Validation",
                               "Invalid input value", zMSGQ_DOMAIN_ERROR, 0 );
            return( zCALL_ERROR );
         }

         break;

      case zDME_GET_VARIABLE:

         if ( lpDomain->cType == zTYPE_STRING )
         {
            GetValueFromRecord( lpDomainData->zView,
                                lpDomainData->lpViewEntity,
                                lpDomainData->lpViewAttribute,
                                &lpInternalValue,
                                &cAttrType,
                                &uAttrLength );

            nRC = SetCursorFirstEntityByString( vDomainTable,
                                                lpDomainData->pszContextName,
                                                szInternalAttributeName,
                                                (zPCHAR) lpInternalValue, "" );
         }
         else
         if ( lpDomain->cType == zTYPE_INTEGER )
         {
            GetValueFromRecord( lpDomainData->zView,
                                lpDomainData->lpViewEntity,
                                lpDomainData->lpViewAttribute,
                                &lpInternalValue,
                                &cAttrType,
                                &uAttrLength );

            lIntegerAttributeValue = (zLONG)(*lpInternalValue);
            nRC = SetCursorFirstEntityByInteger( vDomainTable,
                                                 lpDomainData->pszContextName,
                                                 szInternalAttributeName,
                                                 lIntegerAttributeValue, "" );
         }
         else
            nRC = -1;

         if ( nRC >= zCURSOR_SET )
         {
            nRC = GetStringFromAttribute( lpDomainData->lpData, lpDomainData->uMaxStringLength,
                                          vDomainTable, lpDomainData->pszContextName, szExternalAttributeName );
         }

         break;
      case zDME_COMPARE_ATTRIBUTE:

         // Get internal value of attribute and simply compare to the string
         // or integer passed in.
         if ( lpDomain->cType == zTYPE_STRING )
         {
            GetStringFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity,
                                 lpDomainData->lpViewAttribute,
                                 szStringAttributeValue, 64 );
            nRC = zstrcmp( (zPCHAR) lpDomainData->lpData, szStringAttributeValue );
            if ( nRC > 0 )
               nRC = 1;
            if ( nRC < 0 )
               nRC = -1;
         }

         if ( lpDomain->cType == zTYPE_INTEGER )
         {
            GetValueFromRecord( lpDomainData->zView,
                                lpDomainData->lpViewEntity,
                                lpDomainData->lpViewAttribute,
                                &lpInternalValue,
                                &cAttrType,
                                &uAttrLength );
            if ( *((zPLONG) lpDomainData->lpData) == *((zPLONG) lpInternalValue) )
               nRC = 1;
            else
               nRC = 0;
         }

         break;
      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
         nRC = SetCursorFirstEntity( vDomainTable, lpDomainData->pszContextName, "" );
         if ( nRC >= zCURSOR_SET )
         {
            nRC = GetStringFromAttribute( lpDomainData->lpData, lpDomainData->uMaxStringLength, vDomainTable,
                                          lpDomainData->pszContextName, szExternalAttributeName );
            nRC = 0;
         }
         else
         {
            nRC = nRC;
         }

         break;
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
         nRC = SetCursorNextEntity( vDomainTable, lpDomainData->pszContextName, "" );
         if ( nRC >= zCURSOR_SET )
         {
            nRC = GetStringFromAttribute( lpDomainData->lpData, lpDomainData->uMaxStringLength, vDomainTable,
                                          lpDomainData->pszContextName, szExternalAttributeName );
            nRC = 0;
         }
         else
         {
            nRC = nRC;
         }

         break;

      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
         nRC = SetCursorNextEntity( vDomainTable, lpDomainData->pszContextName, "" );
         break;

      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         nRC = SetCursorPrevEntity( vDomainTable, lpDomainData->pszContextName, "" );
         break;

      default:
         // We're not handling this case so send it back.
         nRC = zDME_NOT_HANDLED;
   }

   return( nRC );
}

// Determine, whether a string represents a valid integer
//  return( 1 ) : valid integer
//         0 : empty or blank string
//        -1 : not valid integer string
static zSHORT ValidInteger( zPCHAR szInteger )
{
   #define WHITESPACE " \t"
   zCHAR *pc = szInteger;
   pc = pc + strspn( pc, WHITESPACE );  /* eliminate blanks */
   if ( *pc == 0 )
      return( 0 ); // empty string

   if ( *pc == '-' || *pc == '+' )
      ++pc;

   pc = pc + strspn( pc, WHITESPACE );  /* eliminate blanks */
   if ( isdigit( *pc ) )
      return( 1 );
   else
      return( -1 );
}
