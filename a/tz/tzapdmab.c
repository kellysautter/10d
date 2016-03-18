/*
CHANGE LOG

2003.01.16  DGC
   Fixed bug reported by Jeff.  Datetime context ignored ulaxStringLth when
   it copied date/time values to the target string.  It now uses the length.

2002.05.14  HH
   prevent crash for freeform without format string.

2001.04.10  HH
   Correct strcmp bug (DEUKA), see tzapdmaa.

2001.02.02  HH
2001.01.17  HH
   FreeForm date & time.

2000.06.25  HH
   fix bug with SetAttibuteFromAttribute in fnDateTime.

1999/05/20  DC   Z10
   Eliminated support for 2-digit years and forced them to be treated as
   4-digit years.

1999/04/22  DC   Z10
   Modified fnGetDateTimeAttribute to force the time portion of Date
   attributes to zeros and the date portion of Time attributes to
   "19000101".

12.07.1997  DonC
   Modified fnCompareDateTimeAttribute to properly handle the compare
   against a null string.

30.07.1997  DonC
   Modified fnGetDateTimeAttribute to correctly handle WeekOfYear processing
   when returning a string.  It was correct when returning an integer.

15.10.1997  HH
   include kzoeinco.h for 9I.

1997.10.31  DGC
   Removed kzoeinco.h for 9I.

21.11.1997  DonC
   Modified fnDateTime routine to allow attribute data type to be anything if
   the function is only for validation and the input data is of type string.

27.01.1998  DonC
   Modified setting and comparing date-to-date attributes to not consider
   ContextName values. This means, any date can be set from any other date
   and any date can be compared to any date within this algorithm.

29.05.1998    RG!!
   Bugfixes erhalten am 29.05.1998 von DonC, Doug ohne Aenderungsvermerk
   sollte nachgeliefert werden

*/

#define  KZMETA_INCL
#define  KZSYSSVC_INCL
#include "kzoengaa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#ifdef __WIN32__
#include <windows.h>
#include <winnls.h>
#endif
#include "tzapdmaa.hg"

enum dm_name
{
   eDM_Date = 0,
   eDM_Time,
   eDM_DateTime
};

enum spec_context
{
   eNotFound = -1,
   eDAY = 0,
   eDAYOFMONTH,
   eDAYOFWEEK,
   eDAYOFYEAR,
   eHOUR,
   eMINUTE,
   eMONTH,
   eMONTHOFYEAR,
   eMSWINDOWSSETTING,
   eMSWINDOWSLONGSETTING,
   eMSWINDOWSSHORTSETTING,
   eSECOND,
   eWEEKOFYEAR,
   eYEAR,
   eYY,
   eYYYY,
   eFREEFORM
};

struct context_table_tag
{
   zPCHAR   pszContextName;
   zPCHAR   pszAllowed_cTypes;
};

typedef struct context_table_tag context_table;

context_table DT_Context[] =
{
   { "Day", "L" },
   { "DayOfMonth", "LS" },
   { "DayOfWeek", "LS" },
   { "DayOfYear", "LS" },
   { "Hour", "LSM" },
   { "Minute", "LSM" },
   { "Month", "L" },
   { "MonthOfYear", "LS" },
   { "MSWindowsSetting", "PS" },
   { "MSWindowsLongSetting", "PS" },
   { "MSWindowsShortSetting", "PS" },
   { "Second", "LSM" },
   { "WeekOfYear", "LS" },
   { "Year", "L" },
   { "YY", "LS" },
   { "YYYY", "LS" },
   { "FreeForm", "S" }
};

zSHORT nNbrContextEntries = sizeof( DT_Context ) / sizeof( context_table );

static zUCHAR bDateEditStringChecked = 0;
static zUCHAR bTimeEditStringChecked = 0;

// date edit string format codes
//    0   for dd.mm.yyyy
//    1   for yyyy.mm.dd
//    2   for mm.dd.yyyy (US)
//    3   for yyyy.dd.mm (US)
//   -1   otherwise
static zSHORT kDateFormat = 0;
static zCHAR cDateSeparator = 0;

static zSHORT kTimeAM = 0; // indicate time foramt is "am"
static zCHAR cTimeSeparator[ 3 ] = {0};

zSHORT
fnSetDateTimeAttribute( zSHORT       nDomain,
                        LPDOMAINDATA lpDomainData,
                        zBOOL        bValidationOnly );

zSHORT LOCALOPER
fnGetDateTimeAttribute( zSHORT       nDomain,
                        LPDOMAINDATA lpDomainData );

zSHORT LOCALOPER
fnCompareDateTimeAttribute( zSHORT       nDomain,
                            LPDOMAINDATA lpDomainData );

zSHORT
fnGetDateTimeCompareValue( zSHORT       nDomain,
                           LPDOMAINDATA lpDomainData );

zSHORT
fnAddToDateTimeAttribute( zSHORT       nDomain,
                          LPDOMAINDATA lpDomainData );

zSHORT
fnSpecialContext( zPCHAR   szContext, zCHAR  cType );

zLONG LOCALOPER
fnDateTime( zSHORT       nDomain,
            zLONG        lEntryType,
            LPDOMAINDATA lpDomainData );

static zSHORT NormalizeFreeFormDate( zPCHAR sz );
static zSHORT NormalizeFreeFormTime( zPCHAR sz );
static zLONG ExtractNumber( zPCHAR pc, zCHAR cSep );
static zSHORT
CheckDate( zLONG lYear, zLONG lMonth, zLONG lDay );
static zSHORT
CheckTime( zLONG *pnHour, zLONG lMinute, zLONG lSecond, zLONG lMilliSecond,
           zSHORT nAPM );
static zLONG
LongYearFromShort( zLONG lYear );
static zSHORT
FormatFreeFormDate( zPCHAR szTarget,
                    zLONG  lMaxLth,
                    zCPCHAR szZeidon,
                    zCPCHAR szEditString );
static zSHORT
FormatFreeFormTime( zPCHAR szTarget,
                    zLONG  lMaxLth,
                    zCPCHAR szZeidon,
                    zCPCHAR szEditString );
static void
CheckEditString( zCPCHAR szEditString,
                 LPDOMAINDATA lpDomainData, zLPCONTEXT lpContext);

// Month table
static const
char *pchMon[ 12 ] = { "January", "February", "March", "April",
                       "May", "June", "July", "August",
                       "September", "October", "November",
                       "December" };

//
// ///////////////////////////////////////////////////////////////////
// * Copyright (c) 1993 - 2016 QuinSoft, Inc. All rights reserved.     *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// ///////////////////////////////////////////////////////////////////
//  AUTHOR:  John Price
//  DATE:    7/8/93
//  API:     TBA
//  ENVIRONMENT: ANY
//
// MODULE NAME:  TZAPDMAB  -  Application Domain library DateTime
//                            routines
//
// DESCRIPTION:  This is the source file which contains DateTime domains
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
//./ END + 2
zSHORT
fnSpecialContext( zPCHAR pchContext, zCHAR  cType )
{
   zSHORT k;

   for ( k = 0; k < nNbrContextEntries; k++ )
   {
      if ( zstrcmp( pchContext, DT_Context[ k ].pszContextName ) == 0 )
      {
         // if cType is null, we only care if the Context name is in the special table...
         if ( cType == 0 )
            return( k );

         if ( zstrchr( DT_Context[ k ].pszAllowed_cTypes, cType ) )
            return( k );
         else
            return( -1 );
      }
   }

   return( -1 );
}

//./ ADD NAME=zdmDateTime
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmDateTime
//
//  PURPOSE:    DateTime attribute handling
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
//./ END + 9
zOPER_EXPORT zLONG OPERATION
zdmDateTime( zLONG        lEntryType,
             LPDOMAINDATA lpDomainData )
{
   return( fnDateTime( eDM_DateTime, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmDate
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmDate
//
//  PURPOSE:    DateTime attribute handling, for just the Date portion.
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
zdmDate( zLONG        lEntryType,
         LPDOMAINDATA lpDomainData )
{
   if ( lpDomainData && lpDomainData->pszContextName &&
        (zstrcmp( lpDomainData->pszContextName, "REVMMDDYY" ) == 0 || zstrcmp( lpDomainData->pszContextName, "ESLMMDDYY" ) == 0) )
   {
      zLONG  lRC;

      lpDomainData->pszContextName += 3;
      lRC = fnDateTime( eDM_Date, lEntryType, lpDomainData );
      lpDomainData->pszContextName -= 3;
      if ( lRC >= 0 )
      {
         zCHAR szDate[ 32 ];

         szDate[ 0 ] = lpDomainData->pszContextName[ 0 ];
         szDate[ 1 ] = lpDomainData->pszContextName[ 1 ];
         szDate[ 2 ] = lpDomainData->pszContextName[ 2 ];
         strcpy_s( szDate + 3, zsizeof( szDate ) - 3, (zPCHAR) (lpDomainData->lpData) );
         strcpy_s( (zPCHAR) (lpDomainData->lpData), lpDomainData->uMaxStringLength, szDate );
      }

      return( lRC );
   }
   else
      return( fnDateTime( eDM_Date, lEntryType, lpDomainData ) );
}

//./ ADD NAME=zdmTime
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      zdmTime
//
//  PURPOSE:    DateTime attribute handling, using just the Time portion
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
//./ END + 9
zOPER_EXPORT zLONG OPERATION
zdmTime( zLONG        lEntryType,
         LPDOMAINDATA lpDomainData )
{
   return( fnDateTime( eDM_Time, lEntryType, lpDomainData ) );
}

//./ ADD NAME=fnDateTime
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
//  ENTRY:      fnDateTime
//
//  PURPOSE:    DateTime attribute handling
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
//./ END + 10
zLONG LOCALOPER
fnDateTime( zSHORT       nDomain,
            zLONG        lEntryType,
            LPDOMAINDATA lpDomainData )
{
   zCHAR          sz[ 100 ];
   LPDOMAIN       lpDomain;
   zSHORT         nRC;

   // befor issuing doofus error messages, we have to check,
   //  whether we would handle the case at all.
   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
      case zDME_GET_VARIABLE:
      case zDME_COMPARE_ATTRIBUTE:
      case zDME_GET_COMPARE_VALUE:
      case zDME_ADD_TO_ATTRIBUTE:
      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         break;

      default:
         return( zDME_NOT_HANDLED );
   }

   nRC = zCALL_ERROR;         // Default to in error status...
   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   // If the lEntryType is validation and the input data type is string,
   // we won't bother to check the attribute lpDomainData->cType.  This means a Domain
   // can use this code for validation regardless of the data type of the
   // Domain.
   // If the above is not true, then the data type must be zTYPE_DATETIME.
   if ( lpDomainData->lpViewAttribute->cType != zTYPE_DATETIME && lEntryType != zDME_VALIDATE_LPDATA && lpDomainData->cType != zTYPE_STRING )
   {
      sz[ 0 ] = lpDomainData->lpViewAttribute->cType;
      sz[ 1 ] = 0;
      strcat_s( sz, zsizeof( sz ), ", " );
      strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );

      // "TZDME003 - Attribute Type invalid for this Domain"
      SendDomainError( lpDomainData->zView, lpDomain, 8, 3, 0, sz, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

#if 0
   TraceLineS( "(apdm) ", "zdmDateTime" );
   TraceLineI( "(apdm) Entry type is ", lEntryType );
   sz[ 0 ] = lpDomainData->cType;
   sz[ 1 ] = 0;
   TraceLineS( "(apdm) lpDomainData->cType is ", sz );
   TraceLineS( "(apdm) lpDomainData->pszContextName is ", lpDomainData->pszContextName );
   TraceLineS( "(apdm) EntityName is ", lpDomainData->lpViewEntity->szName );
   TraceLineS( "(apdm) AttributeName is ", lpDomainData->lpViewAttribute->szName );
#endif
   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
         nRC = fnSetDateTimeAttribute( nDomain, lpDomainData, 1 );
         break;

      case zDME_SET_ATTRIBUTE:
         nRC = fnSetDateTimeAttribute( nDomain, lpDomainData, 0 );
         break;

      case zDME_GET_VARIABLE:
         nRC = fnGetDateTimeAttribute( nDomain, lpDomainData );
         break;

      case zDME_COMPARE_ATTRIBUTE:
         nRC = fnCompareDateTimeAttribute( nDomain, lpDomainData );
         break;

      case zDME_GET_COMPARE_VALUE:
         nRC = fnGetDateTimeCompareValue( nDomain, lpDomainData );
         break;

      case zDME_ADD_TO_ATTRIBUTE:
         nRC = fnAddToDateTimeAttribute( nDomain, lpDomainData );
         break;

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
         // "TZDME004 - Invalid Domain Entry Type "
         SendDomainError( lpDomainData->zView, lpDomain, 8, 4, lEntryType,
                          lpDomainData->lpViewEntity->szName,
                          lpDomainData->lpViewAttribute->szName );
         return( zCALL_ERROR );

      default:
         // We're not handling this case so send it back.
         nRC = zDME_NOT_HANDLED;
   }

   return( nRC );
}

//./ ADD NAME=fnSetDateTimeAttribute
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
// ENTRY:  fnSetDateTimeAttribute
//
// PARAMETERS:
//
// 1. nDomain -
//       Identifier as to which DateTime domain is being invoked.
// 2. lpDomainData->cType -
//       The Source/Target Data Type, identifies the type of data (of valid
//       Zeidon data types) that is the source in a Set call and the target
//       in a Get call.
// 3. lpDomainData->lpData -
//       The Source/Target Data Pointer, points to the data that is the
//       source in a Set call and the target in a Get call.
// 4. lpDomainData->pszContextName    -
//       The Context Name to be applied to this Domain function.
// 5. lpDomainData->zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
// 6. lpDomainData->lpViewEntity -
//       Points to the entity that is the target in a Set call and the
//       source in a Get call.
// 7. lpDomainData->lpViewAttribute -
//       Points to the attribute that is the target in a Set call and the
//       source in a Get call.
// 8. nMaxStringLength -
//       For a Get call with data type 'string', it identifies the maximum
//       length of the string variable (should be used only with a
//       GetVariableFromAttribute operation).
// 9. bValidationOnly -
//       If true, then every thing is done except storing the value in
//       the record.
//
// RETURNS:
//              0 - Attribute value set
//    zCALL_ERROR - error on call
//
////////////////////////////////////////////////
//./ END + 4
zSHORT
fnSetDateTimeAttribute( zSHORT       nDomain,
                        LPDOMAINDATA lpDomainData,
                        zBOOL        bValidationOnly )
{
   LPDOMAIN       lpDomain;
   zLPCONTEXT     lpContext;
   DateTimeRecord stDT;
   zPCHAR         psz;
   zPCHAR         pszEditString;
   zCHAR          szDT1[ 20 ];
   zCHAR          sz[ 256 ];
   zCHAR          szDateFmt[ 256 ];
   zBOOL          bWasNull = 0;
   zSHORT         nSeverity;
   zSHORT         nContextIndex;
   zSHORT         k;
   zSHORT         nRC;

   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   // determine input data type
   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         // string to decimal
         psz = (zPCHAR) lpDomainData->lpData;
         if ( psz == 0 || *psz == 0 )
            sz[ 0 ] = 0;
         else
            strcpy_s( sz, zsizeof( sz ), (zPCHAR) lpDomainData->lpData );

         psz = sz;

         if ( lpDomainData->pszContextName && lpDomainData->pszContextName[ 0 ] )
         {
            nRC = GetStringFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, szDT1, zsizeof( szDT1 ) - 1 );
            switch ( nRC )
            {
               case 0:
                  // string retrieved
                  break;

               case -1:
                  // attribute value was null
                  bWasNull = 1;
                  strcpy_s( szDT1, zsizeof( szDT1 ), "00000000000000000" );
                  break;

               default:
                  return( zCALL_ERROR );
            }

            if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) )
            {
               if ( (nContextIndex = fnSpecialContext( lpContext->szName, 0 ) ) >= 0 )
               {
                  switch ( nContextIndex )
                  {
                     case eDAYOFMONTH:
                        if ( isdigit( *sz ) && isdigit( *(sz + 1) ) )
                        {
                           *(szDT1 + 6) = *sz;
                           *(szDT1 + 7) = *(sz + 1);
                           zmemcpy( sz, szDT1, 18 );
                        }
                        else
                        {
                           if ( bValidationOnly )
                              nSeverity = -1;
                           else
                              nSeverity = 0;

                           // "TZDME016 - Input invalid for context "
                           SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 16, 0, sz, lpContext->szName );
                           return( zCALL_ERROR );
                        }

                        break;

                     case eDAYOFYEAR:
                        for ( k = 0; k < 3; k++ )
                        {
                           if ( !isdigit( *(sz + k) ) )
                              break;
                        }

                        if ( k == 3 )
                        {
                           // use year from original DateTime
                           zmemcpy( sz + 50, szDT1, 4 );
                           // ddd from input
                           zmemcpy( sz + 54, sz, 3 );
                           *(sz + 57) = 0;
                           // convert
                           UfJulianToGregorian( sz + 50, sz );
                           // copy Time of original DateTime
                           zmemcpy( sz + 8, szDT1 + 8, 10 );
                        }
                        else
                        {
                           if ( bValidationOnly )
                              nSeverity = -1;
                           else
                              nSeverity = 0;

                           // "TZDME016 - Input invalid for context "
                           SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 16, 0, sz, lpContext->szName );
                           return( zCALL_ERROR );
                        }

                        break;

                     case eMONTHOFYEAR:
                        for ( k = 0; k < 12; k++ )
                        {
                           if ( ztoupper( *sz ) == *pchMon[ k ] )
                           {
                              if ( zstrcmpi( sz, pchMon[ k ] ) == 0 )
                              {
                                 break;
                              }
                           }
                        }

                        if ( k == 12 )
                        {
                           if ( bValidationOnly )
                              nSeverity = -1;
                           else
                              nSeverity = 0;

                           // "TZDME016 - Input invalid for context "
                           SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 16, 0, sz, lpContext->szName );
                           return( zCALL_ERROR );
                        }
                        else
                        {
                           zltoa( (zLONG) k + 101L, sz, zsizeof( sz ) );
                           *(szDT1 + 4) = *(sz + 1);
                           *(szDT1 + 5) = *(sz + 2);
                           zmemcpy( sz, szDT1, 18 );
                        }

                        break;

                     case eYY:
                        // Eliminated support of 2-digit years and forced them to be treated as 4-digit years.
#if 0
                        if ( isdigit( *sz ) && isdigit( *(sz + 1) ) )
                        {
                           *szDT1 = '1';
                           *(szDT1 + 1) = '9';
                           *(szDT1 + 2) = *sz;
                           *(szDT1 + 3) = *(sz + 1);
                           zmemcpy( sz, szDT1, 18 );
                        }
                        else
                        {
                           if ( bValidationOnly )
                              nSeverity = -1;
                           else
                              nSeverity = 0;

                           // "TZDME016 - Input invalid for context "
                           SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 16, 0, sz, lpContext->szName );
                           return( zCALL_ERROR );
                        }

                        break;
#endif
                     case eYYYY:
                        for ( k = 0; k < 4; k++ )
                        {
                           if ( !isdigit( *(sz + k) ) )
                              break;
                        }

                        if ( k == 4 )
                        {
                           zmemcpy( sz + 4, szDT1 + 4, 14 );
                        }
                        else
                        {
                           if ( bValidationOnly )
                              nSeverity = -1;
                           else
                              nSeverity = 0;

                           // "TZDME016 - Input invalid for context "
                           SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 16, 0, sz, lpContext->szName );
                           return( zCALL_ERROR );
                        }

                        break;

                     case eWEEKOFYEAR:
                     case eDAYOFWEEK:
                        if ( bValidationOnly )
                           nSeverity = -1;
                        else
                           nSeverity = 8;

                        // "TZDME014 - Context is for retrieval only "
                        SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 14, 0, lpContext->szName, 0 );
                        return( zCALL_ERROR );

                     case eDAY:
                     case eMONTH:
                     case eYEAR:
                        if ( bValidationOnly )
                           nSeverity = -1;
                        else
                           nSeverity = 8;

                        // "TZDME015 - Context only used for arithmetic operations "
                        SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 15, 0, lpContext->szName, 0 );
                        return( zCALL_ERROR );

                     case eFREEFORM:
                        if ( lpContext->hEditString )
                        {
                           pszEditString = SysGetPointerFromHandle( lpContext->hEditString );
                           CheckEditString( pszEditString, lpDomainData, lpContext );
                        }

                        if ( zstrcmp( "Date", lpDomain->szName) == 0 )
                           nRC = NormalizeFreeFormDate( sz );
                        else
                           nRC = NormalizeFreeFormTime( sz );

                        if ( nRC == -1 )
                        {
                           if ( bValidationOnly )
                              nSeverity = -1;
                           else
                              nSeverity = 4;

                           // "TZDME010 - DateTime input string invalid "
                            SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 10, 0, lpContext->szName, sz );
                            return( zCALL_ERROR );
                        }

                        break;

                     case eMSWINDOWSSETTING:
                     case eMSWINDOWSLONGSETTING:
                     case eMSWINDOWSSHORTSETTING:
                     default:
                        if ( lpContext->hEditString )
                           pszEditString = SysGetPointerFromHandle( lpContext->hEditString );
                        else
                        {
                           if ( nContextIndex == eMSWINDOWSLONGSETTING )
                           {
                              SysGetLongDateFormat( szDateFmt );
                              pszEditString = szDateFmt;
                           }
                           else
                           if ( nContextIndex == eMSWINDOWSSHORTSETTING )
                           {
                              SysGetShortDateFormat( szDateFmt );
                              pszEditString = szDateFmt;
                           }
                           else
                           if ( nContextIndex == eMSWINDOWSSETTING )
                           {
                              SysGetTimeFormat( szDateFmt );
                              pszEditString = szDateFmt;
                           }
                        }

                        // At last, we finally edit the silly thing.
                        nRC = UfEditFormatDateTime( sz, zsizeof( sz ), pszEditString );

                        if ( nRC )
                        {
                           if ( nRC == -1 )
                           {
                              if ( bValidationOnly )
                                 nSeverity = -1;
                              else
                                 nSeverity = 4;

                              // "TZDME010 - DateTime input string invalid "
                              SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 10, 0, lpContext->szName, sz );
                           }
                           else
                           {
                              if ( bValidationOnly )
                                 nSeverity = -1;
                              else
                                 nSeverity = 8;

                              // "TZDME009 - Context edit string is invalid for Domain "
                              SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 9, 0, lpContext->szName, pszEditString );
                           }

                           return( zCALL_ERROR );
                        }
                  }
               }
               else
               {
                  if ( lpContext->hEditString == 0 )
                  {
                     if ( bValidationOnly )
                        nSeverity = -1;
                     else
                        nSeverity = 8;

                     strcpy_s( sz, zsizeof( sz ), lpContext->szName );
                     k = (zSHORT) zstrlen( sz );
                     sz[ k ] = '/';
                     sz[ k + 1 ] = lpDomainData->cType;
                     sz[ k + 2 ] = 0;

                     // "TZDME013 - Context/cType Combination is invalid "
                     SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 13, 0, sz, 0 );
                     return( zCALL_ERROR );
                  }

                  nRC = UfEditFormatDateTime( sz, zsizeof( sz ), SysGetPointerFromHandle( lpContext->hEditString ) );
                  if ( nRC )
                  {
                     if ( nRC == -1 )
                     {
                        if ( bValidationOnly )
                           nSeverity = -1;
                        else
                           nSeverity = 4;

                        // "TZDME010 - DateTime input string invalid "
                        SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 10, 0, lpContext->szName, sz );
                     }
                     else
                     {
                        if ( bValidationOnly )
                           nSeverity = -1;
                        else
                           nSeverity = 8;

                        pszEditString = SysGetPointerFromHandle( lpContext->hEditString );

                        // "TZDME009 - Context edit string is invalid for Domain "
                        SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 9, 0, lpContext->szName, pszEditString );
                     }

                     return( zCALL_ERROR );
                  }

                  if ( nDomain != eDM_Time && zstrncmp( sz, "XX", 2 ) == 0 )
                  {
                     *sz = '1';
                     *(sz + 1) = '9';
                  }
               }
            }
            else
            {
               if ( bValidationOnly )
                  nSeverity = -1;
               else
                  nSeverity = 8;

               // "TZDME008 - Could not find context for Domain "
               SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
               return( zCALL_ERROR );
            }

#if 0
//* This code was eliminated 1999/04/22 by DC because DM_Date should always have Time portion set to zeros and DM_Time should always have Date portion set to "19000101".
            // If domain is Date or Time, retrieve the portion that will not be changing and place into string
            if ( nDomain != eDM_DateTime )
            {
               switch ( nDomain )
               {
                  case eDM_Date:
                     // When working with the Date domain leave the time portion as is.
                     zmemcpy( psz + 8, szDT1 + 8, 10 );
                     break;

                  case eDM_Time:
                     // When working with the Time domain leave the date portion as is.
                     zmemcpy( psz, szDT1, 8 );
                     break;
               }
            }
#endif

         } // end of if ( pchContext...

         // Added 1999/04/22 by DC so that Dates would have time portion set to zeros and Times would have date portion set to "19000101".
         if ( nDomain != eDM_DateTime )
         {
            switch ( nDomain )
            {
               case eDM_Date:
                  // When working with the Date domain, force the time portion to zeros.
                  zmemcpy( psz + 8, "000000000", 10 );
                  break;

               case eDM_Time:
                  // When working with the Time domain, force the date portion to "19000101".
                  zmemcpy( psz, "19000101", 8 );
                  break;
            }
         }

         // Convert to internal format
         nRC = UfStringToDateTime( psz, &stDT );

         if ( nRC )
         {
            if ( bValidationOnly )
               nSeverity = -1;
            else
               nSeverity = 0;

            strcpy_s( sz, zsizeof( sz ), (zPCHAR) lpDomainData->lpData );
            strcat_s( sz, zsizeof( sz ), ", " );
            strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
            // "TZDME010 - DateTime input string invalid "
            SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 10, 0, sz, lpDomainData->lpViewAttribute->szName );
         }

         if ( nRC == 0 && bValidationOnly == FALSE )
         {
            nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, &stDT, 0 );
            if ( nRC )
            {
               if ( bValidationOnly )
                  nSeverity = -1;
               else
                  nSeverity = 16;

               strcpy_s( sz, zsizeof( sz ), (zPCHAR) lpDomainData->lpData );
               strcat_s( sz, zsizeof( sz ), ", " );
               strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
               // "TZDME011 - Error storing value in record "
               SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 11, 0, sz, lpDomainData->lpViewAttribute->szName );
            }
         }

         break;

      case zTYPE_INTEGER:
         if ( lpDomainData->pszContextName == 0 || *lpDomainData->pszContextName == 0 )
         {
            if ( bValidationOnly )
               nSeverity = -1;
            else
               nSeverity = 8;

            // "TZDME012 - Context Required when lpDomainData->cType is zTYPE_INTEGER "
            SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 12, 0, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         nRC = GetStringFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, szDT1, zsizeof( szDT1 ) - 1 );
         switch ( nRC )
         {
            case 0:
               // string retrieved
               break;

            case -1:
               // attribute value was null
               bWasNull = 1;
               strcpy_s( szDT1, zsizeof( szDT1 ), "00000000000000000" );
               break;

            default:
               return( zCALL_ERROR );
         }

         psz = szDT1;

         if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) )
         {
            if ( (nContextIndex = fnSpecialContext( lpContext->szName, lpDomainData->cType ) ) >= 0 )
            {
               switch ( nContextIndex )
               {
                  case eDAYOFMONTH:
                     zltoa( *((zPLONG) lpDomainData->lpData) + 100, sz, zsizeof( sz ) );
                     *(szDT1 + 6) = *(sz + 1);
                     *(szDT1 + 7) = *(sz + 2);
                     break;

                  case eMONTHOFYEAR:
                     zltoa( *((zPLONG) lpDomainData->lpData) + 100, sz, zsizeof( sz ) );
                     *(szDT1 + 4) = *(sz + 1);
                     *(szDT1 + 5) = *(sz + 2);
                     break;

                  case eYY:
                     // Eliminated support of 2-digit years and forced them to be treated as 4-digit years.
#if 0
                     zltoa( *((zPLONG) lpDomainData->lpData) + 1900, sz, zsizeof( sz ) );
                     zmemcpy( szDT1, sz, 4 );
                     break;
#endif
                  case eYYYY:
                     zltoa( *((zPLONG) lpDomainData->lpData) + 10000, sz, zsizeof( sz ) );
                     zmemcpy( szDT1, sz + 1, 4 );
                     break;

                  case eDAYOFYEAR:
                  {
                     zCHAR sz2[ 20 ];
                     zltoa( *((zPLONG) lpDomainData->lpData) + 1000, sz, zsizeof( sz ) );
                     zmemcpy( sz2, szDT1, 4 ); // copy yyyy
                     strcpy_s( sz2 + 4, zsizeof( sz2 ) - 4, sz + 1 ); // copy jjj
                     if ( UfJulianToGregorian( sz2, sz ) == zCALL_ERROR )
                     {
                        return( zCALL_ERROR );
                     }
                     else
                     {
                        zmemcpy( szDT1, sz, 8 );
                        break;
                     }
                  }

                  case eWEEKOFYEAR:
                  case eDAYOFWEEK:
                     if ( bValidationOnly )
                        nSeverity = -1;
                     else
                        nSeverity = 8;

                     // "TZDME014 - Context is for retrieval only "
                     SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 14, 0, lpContext->szName, 0 );
                     return( zCALL_ERROR );

                  case eDAY:
                  case eMONTH:
                  case eYEAR:
                     if ( bValidationOnly )
                        nSeverity = -1;
                     else
                        nSeverity = 8;

                     // "TZDME015 - Context only used for arithmetic operations "
                     SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 15, 0, lpContext->szName, 0 );
                     return( zCALL_ERROR );
               }

               // Convert to internal format
               nRC = UfStringToDateTime( psz, &stDT );

               if ( nRC )
               {
                  if ( bValidationOnly )
                     nSeverity = -1;
                  else
                     nSeverity = 0;

                  strcpy_s( sz, zsizeof( sz ), psz );
                  strcat_s( sz, zsizeof( sz ), ", " );
                  strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
                  // "TZDME010 - DateTime input string invalid "
                  SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 10, 0, sz, lpDomainData->lpViewAttribute->szName );
               }

               if ( nRC == 0 && bValidationOnly == FALSE )
               {
                  nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, &stDT, 0 );
                  if ( nRC )
                  {
                     strcpy_s( sz, zsizeof( sz ), psz );
                     strcat_s( sz, zsizeof( sz ), ", " );
                     strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
                     // "TZDME011 - Error storing value in record "
                     SendDomainError( lpDomainData->zView, lpDomain, 16, 11, 0, sz, lpDomainData->lpViewAttribute->szName );
                  }
               }
            }
            else
            {
               if ( bValidationOnly )
                  nSeverity = -1;
               else
                  nSeverity = 8;

               strcpy_s( sz, zsizeof( sz ), lpContext->szName );
               k = (zSHORT) zstrlen( sz );
               sz[ k ] = '/';
               sz[ k + 1 ] = lpDomainData->cType;
               sz[ k + 2 ] = 0;
               // "TZDME013 - Context/cType Combination is invalid "
               SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 13, 0, sz, 0 );
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
            SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
            return( zCALL_ERROR );
         }

         break;

      case zTYPE_DATETIME:
#if 0
  //* Eliminated 27.01.1998 by DonC.
      Date to Date operations are OK without considering ContextName.
         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName && zstrcmp( lpDomainData->pszContextName, "DateTime" ) != 0 )
         {
            strcpy_s( sz, zsizeof( sz ), lpDomainData->pszContextName );
            strcat_s( sz, zsizeof( sz ), "/" );
            k = zstrlen( sz );
            *(sz + k) = lpDomainData->cType;
            *(sz + k + 1) = 0;
            strcat_s( sz, zsizeof( sz ), ", " );
            strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
            // "TZDME013 - Context/cType Combination is invalid "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }
#endif

         if ( bValidationOnly )
            nRC = 0;
         else
            nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, lpDomainData->lpData, 0 );

         if ( nRC )
         {
            // "TZDME011 - Error storing value in record "
            SendDomainError( lpDomainData->zView, lpDomain, 16, 11, 0, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
         }

         break;

      default:
         if ( bValidationOnly )
            nSeverity = -1;
         else
            nSeverity = 8;

         sz[ 0 ] = lpDomainData->cType;
         sz[ 1 ] = 0;
         strcat_s( sz, zsizeof( sz ), ", " );
         strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
         // "TZDME001 - Invalid Input Data Type"
         SendDomainError( lpDomainData->zView, lpDomain, nSeverity, 1, 0, sz, lpDomainData->lpViewAttribute->szName );
         return( zCALL_ERROR );
   }

   return( nRC );
}

//./ ADD NAME=fnGetDateTimeAttribute
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
// ENTRY:  fnGetDateTimeAttribute
//
// PARAMETERS:
//
// 1. nDomain -
//       Identifier as to which DateTime domain is being invoked.
// 2. lpDomainData->cType -
//       The Source/Target Data Type, identifies the type of data (of valid
//       Zeidon data types) that is the source in a Set call and the target
//       in a Get call.
// 3. lpDomainData->lpData -
//       The Source/Target Data Pointer, points to the data that is the
//       source in a Set call and the target in a Get call.
// 4. lpDomainData->pszContextName    -
//       The Context Name to be applied to this Domain function.
// 5. lpDomainData->zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
// 6. lpDomainData->lpViewEntity -
//       Points to the entity that is the target in a Set call and the
//       source in a Get call.
// 7. lpDomainData->lpViewAttribute -
//       Points to the attribute that is the target in a Set call and the
//       source in a Get call.
// 8. nMaxStringLength -
//       For a Get call with data type 'string', it identifies the maximum
//       length of the string variable (should be used only with a
//       GetVariableFromAttribute operation).
//
// RETURNS:
//              0 - Attribute value returned
//    zCALL_ERROR - error on call
//
////////////////////////////////////////////////
//./ END + 3
zSHORT LOCALOPER
fnGetDateTimeAttribute( zSHORT       nDomain,
                        LPDOMAINDATA lpDomainData )
{
   LPDOMAIN    lpDomain;
   zLPCONTEXT  lpContext;
   zPCHAR      pszEditString;
   zCHAR       sz1[ 256 ];
   zCHAR       sz2[ 256 ];
   zCHAR       szDT1[ 20 ] = { "1234567890123456789" };
   zBOOL       bWasNull = 0;
   zSHORT      nContextIndex;
   zSHORT      k;
   zSHORT      nRC;

   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   // When retrieving the pic value for an attribute, we cannot assume that there is an entity instance present.
   if ( lpDomainData->cType != zTYPE_PIC )
   {
      nRC = GetStringFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, szDT1, zsizeof( szDT1 ) - 1 );
      switch ( nRC )
      {
         case 0:
            // string retrieved
            break;

         case -1:
            // attribute value was null
            bWasNull = 1;
            break;

         default:
            return( zCALL_ERROR );
      }
   }

   // determine output data type requested.
   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         if ( bWasNull )
         {
            // if no record, 0 the output string
            *((zPCHAR) lpDomainData->lpData) = 0;
            return( -1 );
         }

         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
         {
            if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) )
            {
               if ( (nContextIndex = fnSpecialContext( lpContext->szName, lpDomainData->cType ) ) >= 0 )
               {
                  if ( lpContext->hEditString )
                     pszEditString = SysGetPointerFromHandle( lpContext->hEditString );
                  else
                  {
                     if ( nContextIndex == eMSWINDOWSLONGSETTING )
                     {
                        SysGetLongDateFormat( sz1 );
                        pszEditString = sz1;
                     }
                     else
                     if ( nContextIndex == eMSWINDOWSSHORTSETTING )
                     {
                        SysGetShortDateFormat( sz1 );
                        pszEditString = sz1;
                     }
                     else
                     if ( nContextIndex == eMSWINDOWSSETTING )
                     {
                        SysGetTimeFormat( sz1 );
                        pszEditString = sz1;
                     }
                     else
                     if ( nContextIndex == eWEEKOFYEAR )
                        ; // just skip out... we'll handle below...
                     else
                     {
                        pszEditString = 0;
                        // "TZDME018 - Context edit string is null "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 18, 0, lpContext->szName, 0 );
                        if ( nContextIndex == eFREEFORM )
                           return( zCALL_ERROR );

                     // continue (?)
                     }
                  }

                  if ( nContextIndex == eWEEKOFYEAR )
                     zltoa( UfGetWeekOfYear( szDT1 ), sz2, zsizeof( sz2 ) );
                  else
                  if ( nContextIndex == eFREEFORM )
                  {
                     CheckEditString( pszEditString, lpDomainData, lpContext );
                     if ( zstrcmp( "Date", lpDomain->szName) == 0 )
                        nRC = FormatFreeFormDate( sz2, zsizeof( sz2 ), szDT1, pszEditString );
                     else
                        nRC = FormatFreeFormTime( sz2, zsizeof( sz2 ), szDT1, pszEditString );
                  }
                  else
                     nRC = UfFormatDateTime( sz2, zsizeof( sz2 ), szDT1, pszEditString );

                  if ( nRC == zCALL_ERROR )
                  {
                     // "TZDME009 - Context edit string is invalid for Domain "
                     SendDomainError( lpDomainData->zView, lpDomain, 8, 9, 0, lpContext->szName, pszEditString );
                     return( nRC );
                  }
               }
               else
               {
                  if ( lpContext->hEditString == 0 )
                  {
                     strcpy_s( sz1, zsizeof( sz1 ), lpContext->szName );
                     k = (zSHORT) zstrlen( sz1 );
                     sz1[ k ] = '/';
                     sz1[ k + 1 ] = lpDomainData->cType;
                     sz1[ k + 2 ] = 0;

                     // "TZDME013 - Context/cType Combination is invalid "
                     SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz1, 0 );
                     return( zCALL_ERROR );
                  }

                  nRC = UfFormatDateTime( sz2, zsizeof( sz2 ), szDT1, SysGetPointerFromHandle( lpContext->hEditString ) );
                  if ( nRC == zCALL_ERROR )
                  {
                     // "TZDME009 - Context edit string is invalid for Domain "
                     SendDomainError( lpDomainData->zView, lpDomain, 8, 9, 0, lpContext->szName, SysGetPointerFromHandle( lpContext->hEditString ) );
                     return( nRC );
                  }
               }

               // Copy value to target string.  Since strncpy doesn't automatically add the null-terminator, we have to do it ourselves ... not anymore dks 2016.02.14
               strncpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, sz2, lpDomainData->uMaxStringLength - 1 );
            // ((zPCHAR) lpDomainData->lpData )[ lpDomainData->uMaxStringLength ] = 0;
            }
            else
            {
               // "TZDME008 - Could not find context for Domain "
               SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
               return( zCALL_ERROR );
            }
         }
         else
         {
            // no context, just copy our DateTime string as is.  Since strncpy doesn't automatically add the null-terminator, we have to do it ourselves ... not anymore dks 2016.02.14
            strncpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, szDT1, lpDomainData->uMaxStringLength - 1 );
         // ((zPCHAR) lpDomainData->lpData)[ lpDomainData->uMaxStringLength ] = 0;
         }

         break;

      case zTYPE_PIC:
         // return the picture string for the given context
         if ( GetContext( &lpContext, lpDomain, lpDomainData->pszContextName ) )
         {
            zPCHAR lpEditString = SysGetPointerFromHandle( lpContext->hEditString );

            if ( lpEditString && *lpEditString )
            {
               // Copy value to target string.  Since strncpy doesn't automatically add the null-terminator, we have to do it ourselves ... not anymore dks 2016.02.14
               strncpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, lpEditString, lpDomainData->uMaxStringLength - 1 );
            // ((zPCHAR) lpDomainData->lpData )[ lpDomainData->uMaxStringLength ] = 0;
            }
            else
            {
               if ( (nContextIndex = fnSpecialContext( lpContext->szName, lpDomainData->cType ) ) >= 0 )
               {
                  zCHAR szTemp[ 100 ];

                  if ( nContextIndex == eMSWINDOWSLONGSETTING )
                     SysGetLongDateFormat( szTemp );
                  else
                  if ( nContextIndex == eMSWINDOWSSHORTSETTING )
                     SysGetShortDateFormat( szTemp );
                  else
                  if ( nContextIndex == eMSWINDOWSSETTING )
                     SysGetTimeFormat( szTemp );
                  else
                     szTemp[ 0 ] = 0;

                  // Copy value to target string.  Since strncpy doesn't automatically add the null-terminator, we have to do it ourselves ... not anymore dks 2016.02.14
                  strncpy_s( (zPCHAR) lpDomainData->lpData, lpDomainData->uMaxStringLength, szTemp, lpDomainData->uMaxStringLength - 1 );
               // ((zPCHAR) lpDomainData->lpData)[ lpDomainData->uMaxStringLength ] = 0;
               }
               else
                  *((zPCHAR) lpDomainData->lpData) = 0;
            }

            nRC = 0;
         }
         else
         {
            // "TZDME008 - Could not find context for Domain "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, TrueName( lpDomainData->pszContextName, zSHOW_ZKEY ), lpDomainData->lpViewAttribute->szName );
            nRC = zCALL_ERROR;
         }
         break;

      case zTYPE_INTEGER:
         if ( bWasNull )
         {
            // if no record, 0 the output string
            *((zPLONG) lpDomainData->lpData) =0;
            return( -1 );
         }

         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
         {
            if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) )
            {
               if ( (nContextIndex = fnSpecialContext( lpContext->szName, lpDomainData->cType ) ) >= 0 )
               {
                  zLONG    l;
                  zLONG    lFrac;
                  zPCHAR   psz;

                  switch ( nContextIndex )
                  {
                     case eDAY:
                     case eDAYOFMONTH:
                        *(szDT1 + 8) = 0;
                        l = atol( szDT1 + 6 );
                        break;

                     case eDAYOFWEEK:
                        l = UfGetDayOfWeek( szDT1 );
                        break;

                     case eDAYOFYEAR:
                        l = UfGetDayOfYear( szDT1 );
                        break;

                     case eMONTH:
                     case eMONTHOFYEAR:
                        *(szDT1 + 6) = 0;
                        l = atol( szDT1 + 4 );
                        break;

                     case eWEEKOFYEAR:
                        l = UfGetWeekOfYear( szDT1 );
                        break;

                     case eYEAR:
                     case eYY:
                     case eYYYY:
                        psz = szDT1;
                        *(szDT1 + 4) = 0;

                     // Eliminated support of 2-digit years and forced them to be treated as 4-digit years.
#if 0
                        if ( nContextIndex == eYY )
                           psz += 2;
#endif
                        l = atol( psz );
                        break;

                     case eHOUR:
                        *(szDT1 + 12) = 0;
                        lFrac = atol( szDT1 + 10 );
                        *(szDT1 + 10) = 0;
                        l = atol( szDT1 + 8 );
                        if ( lFrac >= 30L )
                           l++;

                        if ( l == 24L )
                           l = 0;

                        break;

                     case eMINUTE:
                        *(szDT1 + 14) = 0;
                        lFrac = atol( szDT1 + 12 );
                        *(szDT1 + 12) = 0;
                        l = atol( szDT1 + 10 );
                        if ( lFrac >= 30L )
                           l++;

                        if ( l == 60L )
                           l = 0;

                        break;

                     case eSECOND:
                        lFrac = atol( szDT1 + 14 );
                        *(szDT1 + 14) = 0;
                        l = atol( szDT1 + 12 );
                        if ( lFrac >= 500L )
                           l++;

                        if ( l == 60L )
                           l = 0;

                        break;

                     default:
                        // "TZDME099 - Forgot this one! "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 99, nContextIndex, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
                  }

                  *((zPLONG) lpDomainData->lpData) = l;
               }
               else
               {
                  strcpy_s( sz2, zsizeof( sz2 ), lpContext->szName );
                  k = (zSHORT) zstrlen( sz2 );
                  sz2[ k ] = '/';
                  sz2[ k + 1 ] = zTYPE_INTEGER;
                  sz2[ k + 2 ] = 0;

                  // "TZDME013 - Context/cType Combination is invalid "
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz2, 0 );
                  return( zCALL_ERROR );
               }
            }
            else
            {
               // "TZDME008 - Could not find context for Domain "
               SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
               return( zCALL_ERROR );
            }
         }
         else
         {
            // "TZDME012 - Context Required when lpDomainData->cType is zTYPE_INTEGER "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 12, 0, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         break;

      case zTYPE_DECIMAL:
         if ( bWasNull )
         {
            // if no record, 0 the output string
            SysConvertLongToDecimal( 0, (zPDECIMAL) lpDomainData->lpData );
            return( -1 );
         }

         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName && nRC == 0 )
         {
            if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) )
            {
               if ( (nContextIndex = fnSpecialContext( lpContext->szName, lpDomainData->cType )) >= 0 )
               {
                  double d;
                  double dFrac;

                  switch ( nContextIndex )
                  {
                     case eHOUR:
                        d = atof( szDT1 + 14 );        // thousands
                        dFrac = d / 1000.0;            // frac of sec
                        *(szDT1 + 14) = 0;
                        d = atof( szDT1 + 12 );        // seconds
                        dFrac = ( d + dFrac) / 60.0;   // frac of minute
                        *(szDT1 + 12) = 0;
                        d = atof( szDT1 + 10 );        // min
                        dFrac = ( d + dFrac) / 60.0;   // frac of hour
                        *(szDT1 + 10) = 0;
                        d = atof( szDT1 + 8 );         // hour
                        d += dFrac;                    // + frac of hour
                        break;

                     case eMINUTE:
                        d = atof( szDT1 + 14 );        // thousands
                        dFrac = d / 1000.0;            // frac of sec
                        *(szDT1 + 14) = 0;
                        d = atof( szDT1 + 12 );        // seconds
                        dFrac = ( d + dFrac) / 60.0;   // frac of minute
                        *(szDT1 + 12) = 0;
                        d = atof( szDT1 + 10 );        // min
                        d += dFrac;                    // + frac of min
                        break;

                     case eSECOND:
                        d = atof( szDT1 + 14 );        // thousands
                        dFrac = d / 1000.0;            // frac of sec
                        *(szDT1 + 14) = 0;
                        d = atof( szDT1 + 12 );        // seconds
                        d += dFrac;                    // + frac of sec
                        break;

                     default:
                        // "TZDME099 - Forgot this one! "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 99, nContextIndex, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
                  }

                  SysConvertDoubleToDecimal( d, (zPDECIMAL) lpDomainData->lpData );
               }
               else
               {
                  strcpy_s( sz2, zsizeof( sz2 ), lpContext->szName );
                  k = (zSHORT) zstrlen( sz2 );
                  sz2[ k ] = '/';
                  sz2[ k + 1 ] = lpDomainData->cType;
                  sz2[ k + 2 ] = 0;
                  // "TZDME013 - Context/cType Combination is invalid "
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz2, 0 );
                  return( zCALL_ERROR );
               }
            }
            else
            {
               // "TZDME008 - Could not find context for Domain "
               SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
               return( zCALL_ERROR );
            }
         }
         else
         {
            // "TZDME012 - Context Required when lpDomainData->cType is zTYPE_INTEGER "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 12, 0, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }
         break;

      default:
         sz1[ 0 ] = lpDomainData->cType;
         sz1[ 1 ] = 0;
         strcat_s( sz1, zsizeof( sz1 ), ", " );
         strcat_s( sz1, zsizeof( sz1 ), lpDomainData->lpViewEntity->szName );
         // "TZDME001 - Invalid Input Data Type"
         SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, sz1, lpDomainData->lpViewAttribute->szName );
         return( zCALL_ERROR );
   }

   return( nRC );
}

//./ ADD NAME=fnCompareDateTimeAttribute
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
// ENTRY:  fnDateTimeCompareAttribute
//
// PARAMETERS:
//
// 1. nDomain -
//       Identifier as to which DateTime domain is being invoked.
// 2. lpDomainData->cType -
//       The Source/Target Data Type, identifies the type of data (of valid
//       Zeidon data types) that is the source in a Set call and the target
//       in a Get call.
// 3. lpDomainData->lpData -
//       The Source/Target Data Pointer, points to the data that is the
//       source in a Set call and the target in a Get call.
// 4. lpDomainData->pszContextName    -
//       The Context Name to be applied to this Domain function.
// 5. lpDomainData->zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
// 6. lpDomainData->lpViewEntity -
//       Points to the entity that is the target in a Set call and the
//       source in a Get call.
// 7. lpDomainData->lpViewAttribute -
//       Points to the attribute that is the target in a Set call and the
//       source in a Get call.
// 8. nMaxStringLength -
//       For a Get call with data type 'string', it identifies the maximum
//       length of the string variable (should be used only with a
//       GetVariableFromAttribute operation).
//
// RETURNS:
//
//             -1 - Attribute value < lpDomainData->lpData value
//              0 - Attribute value = lpDomainData->lpData value
//              1 - Attribute value > lpDomainData->lpData value
//    zCALL_ERROR - error on call
//
////////////////////////////////////////////////
//./ END + 9
zSHORT LOCALOPER
fnCompareDateTimeAttribute( zSHORT       nDomain,
                            LPDOMAINDATA lpDomainData )
{
   LPDOMAIN    lpDomain;
   zLPCONTEXT  lpContext;
   zCHAR       szDT1[ 20 ];
   zCHAR       szDT2[ 20 ];
   zPCHAR      pszEditString;
   zPCHAR      psz1;
   zPCHAR      psz2;
   zCHAR       sz1[ 128 ];
   zCHAR       sz2[ 128 ];
   zBOOL       bWasNull;
   zSHORT      nContextIndex;
   zSHORT      k;
   zSHORT      nRC;

   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );
   nRC = GetStringFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, szDT1, zsizeof( szDT1 ) - 1 );

   switch ( nRC )
   {
      case 0:
         // string retrieved
         bWasNull = 0;
         break;

      case -1:
         // attribute value was null
         bWasNull = 1;
         break;

      default:
         return( zCALL_ERROR );
   }

   // determine input data type (lpDomainData->lpData)
   switch ( lpDomainData->cType )
   {
      case zTYPE_STRING:
         // DateTime to string.  Skip processing if lpDomainData->lpData ptr is 0.
         if ( lpDomainData->lpData )
         {
            // Before checking Context, we will process the cases where
            // either the attr value is null or the compare value is null.

            // Case 1: Both are null so return equal.
            if ( bWasNull && *((zPCHAR) lpDomainData->lpData) == 0 )
               return( 0 );

            // Case 2: Attribute value is null but compare value is not.  Return -1.
            if ( bWasNull && *((zPCHAR) lpDomainData->lpData) != 0 )
               return( -1 );

            // Case 3: Compare value is null but attribute value is not.  Return +1.
            if ( !bWasNull && *((zPCHAR) lpDomainData->lpData) == 0 )
               return( 1 );

            // if no context specified, assumption is that the input string looks like a DateTime data type that has been converted to a string.
            if ( lpDomainData->pszContextName == 0 || *lpDomainData->pszContextName == 0 )
            {
               switch ( nDomain )
               {
                  case eDM_Date:
                     // only compare the date portion of the string.
                     nRC = zstrncmp( szDT1, (zPCHAR) lpDomainData->lpData, 8 );
                     break;

                  case eDM_Time:
                     if ( zstrlen( (zPCHAR) lpDomainData->lpData ) < 9 )
                     {
                        // no 'time' in the date time input string ... indicate that attr > string value
                        nRC = 1;
                        break;
                     }
                     nRC = zstrcmp( szDT1 + 8, ((zPCHAR) lpDomainData->lpData + 8) );
                     break;

                  default: // DateTime
                     nRC = zstrcmp( szDT1, (zPCHAR) lpDomainData->lpData );
               }

               // avoid domain handler return code conflict
               if ( nRC > 0 )
                  nRC = 1;
               else
               if ( nRC < 0 )
                  nRC = -1;

            }
            else
            {
               // we have a context specified.
               if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) )
               {
                  if ( (nContextIndex = fnSpecialContext( lpContext->szName, lpDomainData->cType ) ) >= 0 )
                  {
                     // We have a special context...
                     if ( lpContext->hEditString )
                        pszEditString = SysGetPointerFromHandle( lpContext->hEditString );
                     else
                     {
                        if ( nContextIndex == eMSWINDOWSLONGSETTING )
                        {
                           SysGetLongDateFormat( sz1 );
                           pszEditString = sz1;
                        }
                        else
                        if ( nContextIndex == eMSWINDOWSSHORTSETTING )
                        {
                           SysGetShortDateFormat( sz1 );
                           pszEditString = sz1;
                        }
                        else
                        if ( nContextIndex == eMSWINDOWSSETTING )
                        {
                           SysGetTimeFormat( sz1 );
                           pszEditString = sz1;
                        }
                        else
                        if ( nContextIndex == eWEEKOFYEAR )
                           ; // just skip out... we'll handle below...
                        else
                        {
                           pszEditString = 0;
                           // "TZDME018 - Context edit string is null "
                           SendDomainError( lpDomainData->zView, lpDomain, 8, 18, 0, lpContext->szName, 0 );
                           // continue anyway...
                        }
                     }

                     // put string into sz2 for comparing

                     if ( nContextIndex == eWEEKOFYEAR )
                        zltoa( UfGetWeekOfYear( sz1 ), sz2, zsizeof( sz2 ) );
                     else
                        nRC = UfFormatDateTime( sz2, zsizeof( sz2 ), szDT1, pszEditString );

                     if ( nRC == zCALL_ERROR )
                     {
                        // "TZDME009 - Context edit string is invalid for Domain "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 9, 0, lpContext->szName, pszEditString );
                        return( nRC );
                     }
                  }
                  else
                  {
                     // We have a context, but nothing 'special'
                     if ( lpContext->hEditString == 0 )
                     {
                        strcpy_s( sz1, zsizeof( sz1 ), lpContext->szName );
                        k = (zSHORT) zstrlen( sz1 );
                        sz1[ k ] = '/';
                        sz1[ k + 1 ] = lpDomainData->cType;
                        sz1[ k + 2 ] = 0;
                        // "TZDME013 - Context/cType Combination is invalid "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz1, 0 );
                        return( zCALL_ERROR );
                     }
                     nRC = UfFormatDateTime( sz2, zsizeof( sz2 ), szDT1, SysGetPointerFromHandle( lpContext->hEditString ) );
                     if ( nRC == zCALL_ERROR )
                     {
                        // "TZDME009 - Context edit string is invalid for Domain "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 9, 0, lpContext->szName, SysGetPointerFromHandle( lpContext->hEditString ) );
                        return( nRC );
                     }
                  }

                  // now for the actual compare
                  nRC = zstrcmp( sz2, (zPCHAR) lpDomainData->lpData );

                  // avoid domain handler return code conflict
                  if ( nRC > 0 )
                     nRC = 1;
                  else
                  if ( nRC < 0 )
                     nRC = -1;

               }
               else
               {
                  // "TZDME008 - Could not find context for Domain "
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
                  return( zCALL_ERROR );
               }
            }
         }
         else
         {
            // input data is null pointer or points to a null
            if ( bWasNull )
               nRC = 0; // indicate attr == string input ( both null )
            else
               nRC = 1; // indicate attr > string input
         }
         break;

      case zTYPE_DATETIME:
#if 0
  //* Eliminated 27.01.1998 by DonC.
      Date to Date operations are OK without considering ContextName.
         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName )
         {
            strcpy_s( sz1, zsizeof( sz1 ), lpDomainData->pszContextName );
            strcat_s( sz1, zsizeof( sz1 ), "/" );
            k = zstrlen( sz1 );
            *(sz1 + i) = lpDomainData->cType;
            *(sz1 + k + 1) = 0;
            strcat_s( sz1, zsizeof( sz1 ), ", " );
            strcat_s( sz1, zsizeof( sz1 ), lpDomainData->lpViewEntity->szName );
            // "TZDME013 - Context/cType Combination is invalid "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz1, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }
#endif

         UfDateTimeToString( (LPDATETIME) lpDomainData->lpData, szDT2, zsizeof( szDT2 ) );

         psz1 = szDT1;
         psz2 = szDT2;
         switch ( nDomain )
         {
            case eDM_Date:
               nRC = zstrncmp( psz1, psz2, 8 );
               break;

            case eDM_Time:
               if ( *psz1 )
                  psz1 += 8; // bump to time portion
               if ( *psz2 )
                  psz2 += 8;
               nRC = zstrcmp( psz1, psz2 );
               break;

            default:
               // eDM_DateTime
               nRC = zstrcmp( psz1, psz2 );
         }

         // avoid domain handler return code conflict
         if ( nRC > 0 )
            nRC = 1;
         else
         if ( nRC < 0 )
            nRC = -1;

         break;

      case zTYPE_INTEGER:
         if ( lpDomainData->pszContextName && *lpDomainData->pszContextName && nRC == 0 )
         {
            if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) )
            {
               if ( (nContextIndex = fnSpecialContext( lpContext->szName, lpDomainData->cType ) ) >= 0 )
               {
                  zLONG    l;
                  zLONG    lFrac;
                  zPCHAR   psz;

                  // if attribute is null, just say that the
                  // attribute is less than the specified variable
                  if ( bWasNull )
                     return( -1 );

                  switch ( nContextIndex )
                  {
                     case eDAY:
                     case eDAYOFMONTH:
                        *(szDT1 + 8) = 0;
                        l = atol( szDT1 + 6 );
                        nRC = (l == *((zPLONG) lpDomainData->lpData) ? 0 : (l > *((zPLONG) lpDomainData->lpData) ? 1 : -1));
                        break;

                     case eDAYOFWEEK:
                        l = UfGetDayOfWeek( szDT1 );
                        break;

                     case eDAYOFYEAR:
                        l = UfGetDayOfYear( szDT1 );
                        break;

                     case eMONTH:
                     case eMONTHOFYEAR:
                        *(szDT1 + 6) = 0;
                        l = atol( szDT1 + 4 );
                        break;

                     case eWEEKOFYEAR:
                        l = UfGetWeekOfYear( szDT1 );
                        break;

                     case eYEAR:
                     case eYY:
                     case eYYYY:
                        psz = szDT1;
                        *(szDT1 + 4) = 0;

                     // Eliminated support of 2-digit years and forced them to be treated as 4-digit years.
#if 0
                        if ( nContextIndex == eYY )
                           psz += 2;
#endif
                        l = atol( psz );
                        break;

                     case eHOUR:
                        *(szDT1 + 12) = 0;
                        lFrac = atol( szDT1 + 10 );
                        *(szDT1 + 10) = 0;
                        l = atol( szDT1 + 8 );
                        if ( lFrac >= 30L )
                           l++;
                        if ( l == 24L )
                           l = 0;
                        break;

                     case eMINUTE:
                        *(szDT1 + 14) = 0;
                        lFrac = atol( szDT1 + 12 );
                        *(szDT1 + 12) = 0;
                        l = atol( szDT1 + 10 );
                        if ( lFrac >= 30L )
                           l++;
                        if ( l == 60L )
                           l = 0;
                        break;

                     case eSECOND:
                        lFrac = atol( szDT1 + 14 );
                        *(szDT1 + 14) = 0;
                        l = atol( szDT1 + 12 );
                        if ( lFrac >= 500L )
                           l++;
                        if ( l == 60L )
                           l = 0;
                        break;

                     default:
                        // "TZDME099 - Forgot this one! "
                        SendDomainError( lpDomainData->zView, lpDomain, 8, 99, nContextIndex, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
                        l = 0;
                  }

                  nRC = (l == *((zPLONG) lpDomainData->lpData) ? 0 : (l > *((zPLONG) lpDomainData->lpData) ? 1 : -1));
               }
               else
               {
                  strcpy_s( sz1, zsizeof( sz1 ), lpContext->szName );
                  k = (zSHORT) zstrlen( sz1 );
                  sz1[ k ] = '/';
                  sz1[ k + 1 ] = zTYPE_INTEGER;
                  sz1[ k + 2 ] = 0;
                  // "TZDME013 - Context/cType Combination is invalid "
                  SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz1, 0 );
                  return( zCALL_ERROR );
               }
            }
            else
            {
               // "TZDME008 - Could not find context for Domain "
               SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
               return( zCALL_ERROR );
            }
         }
         else
         {
            // "TZDME012 - Context Required when lpDomainData->cType is zTYPE_INTEGER "
            SendDomainError( lpDomainData->zView, lpDomain, 8, 12, 0, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
            return( zCALL_ERROR );
         }

         break;

      default:
         *sz1 = lpDomainData->cType;
         *(sz1 + 1) = 0;
         strcat_s( sz1, zsizeof( sz1 ), ", " );
         strcat_s( sz1, zsizeof( sz1 ), lpDomainData->lpViewEntity->szName );
         // "TZDME001 - Invalid Input Data Type"
         SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, sz1, lpDomainData->lpViewAttribute->szName );
         return( zCALL_ERROR );
   }

   return( nRC );
}


//./ ADD NAME=fnGetDateTimeCompareValue
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
// ENTRY:  fnGetDateTimeCompareValue
//
// PARAMETERS:
//
// 1. nDomain -
//       Identifier as to which DateTime domain is being invoked.
// 2. lpDomainData->cType -
//       The Source/Target Data Type, identifies the type of data (of valid
//       Zeidon data types) that is the source in a Set call and the target
//       in a Get call.
// 3. lpDomainData->lpData -
//       The Source/Target Data Pointer, points to the data that is the
//       source in a Set call and the target in a Get call.
// 4. lpDomainData->pszContextName    -
//       The Context Name to be applied to this Domain function.
// 5. lpDomainData->zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
// 6. lpDomainData->lpViewEntity -
//       Points to the entity that is the target in a Set call and the
//       source in a Get call.
// 7. lpDomainData->lpViewAttribute -
//       Points to the attribute that is the target in a Set call and the
//       source in a Get call.
// 8. nMaxStringLength -
//       For a Get call with data type 'string', it identifies the maximum
//       length of the string variable (should be used only with a
//       GetVariableFromAttribute operation).
//
// RETURNS:
//             -2 - lpDomainData->lpData ignored, this tells SetEntityCursor that
//                  it should invoked domain processing with the
//                  zDME_COMPARE_ATTRIBUTE entry type, each time
//                  that is determining if the EntityInstance
//                  matches the requested qualifications.
//             -1 - lpDomainData->lpData invalid
//              0 - lpDomainData->lpData unchanged, okay to use as is.
//              1 - lpDomainData->lpData returned, string value
//              2 - lpDomainData->lpData returned, integer value
//              3 - lpDomainData->lpData returned, decimal value
//
//    zCALL_ERROR - error on call
//
////////////////////////////////////////////////
//./ END + 9
zSHORT
fnGetDateTimeCompareValue( zSHORT       nDomain,
                           LPDOMAINDATA lpDomainData )
{
   zSHORT      nRC;

   if ( lpDomainData->cType == zTYPE_DATETIME && (lpDomainData->pszContextName == 0 || *lpDomainData->pszContextName == 0) )
      nRC = 0;
   else
      nRC = -2;

   return( nRC );
}

//./ ADD NAME=fnAddToDateTimeAttribute
// Source Module=tzapdmaa.c
////////////////////////////////////////////////
//
// ENTRY:  fnAddToDateTimeAttribute
//
// PARAMETERS:
//
// 1. nDomain -
//       Identifier as to which DateTime domain is being invoked.
// 2. lpDomainData->cType -
//       The Source/Target Data Type, identifies the type of data (of valid
//       Zeidon data types) that is the source in a Set call and the target
//       in a Get call.
// 3. lpDomainData->lpData -
//       The Source/Target Data Pointer, points to the data that is the
//       source in a Set call and the target in a Get call.
// 4. lpDomainData->pszContextName    -
//       The Context Name to be applied to this Domain function.
// 5. lpDomainData->zView -
//       Identifies the View that is the target in a Set call and the
//       source in a Get call.
// 6. lpDomainData->lpViewEntity -
//       Points to the entity that is the target in a Set call and the
//       source in a Get call.
// 7. lpDomainData->lpViewAttribute -
//       Points to the attribute that is the target in a Set call and the
//       source in a Get call.
// 8. nMaxStringLength -
//       For a Get call with data type 'string', it identifies the maximum
//       length of the string variable (should be used only with a
//       GetVariableFromAttribute operation).
//
// RETURNS:
//              0 - Attribute value has been modified
//    zCALL_ERROR - error on call
//
////////////////////////////////////////////////
//./ END + 9
zSHORT
fnAddToDateTimeAttribute( zSHORT       nDomain,
                          LPDOMAINDATA lpDomainData )
{
   LPDOMAIN       lpDomain;
   zLPCONTEXT     lpContext;
   zSHORT         nRC;
   zSHORT         nDateType;
   zSHORT         n;
   DateTimeRecord stDT;
   zCHAR          sz[ 256 ];
   zCHAR          szDT1[ 20 ];
   zCHAR          szDT2[ 20 ];

   lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );

   if ( lpDomainData->cType != zTYPE_INTEGER )
   {
      sz[ 0 ] = lpDomainData->cType;
      sz[ 1 ] = 0;
      strcat_s( sz, zsizeof( sz ), ", " );
      strcat_s( sz, zsizeof( sz ), lpDomainData->lpViewEntity->szName );
      // "TZDME001 - Invalid Input Data Type"
      SendDomainError( lpDomainData->zView, lpDomain, 8, 1, 0, sz, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   if ( lpDomainData->pszContextName == 0 || *lpDomainData->pszContextName == 0 )
   {
      // "TZDME012 - Context Required when lpDomainData->cType is zTYPE_INTEGER "
      SendDomainError( lpDomainData->zView, lpDomain, 8, 12, 0, lpDomainData->lpViewEntity->szName, lpDomainData->lpViewAttribute->szName );
      return( zCALL_ERROR );
   }

   if ( GetContext( &lpContext, lpDomain, (zCPCHAR) lpDomainData->pszContextName ) == 0 )
   {
      // "TZDME008 - Could not find context for Domain "
      SendDomainError( lpDomainData->zView, lpDomain, 8, 8, 0, (zCPCHAR) lpDomainData->pszContextName, 0 );
      return( zCALL_ERROR );
   }

   nRC = GetStringFromRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, szDT2, zsizeof( szDT2 ) - 1 );
   switch ( nRC )
   {
      case 0:
         // string retrieved
         break;

      case -1:
         // attribute value was null
         strcpy_s( szDT2, zsizeof( szDT2 ), "00000000000000000" );
         break;

      default:
         return( zCALL_ERROR );
   }

   // Case: Year
   if ( zstrcmp( lpDomainData->pszContextName, "Year" ) == 0 )
      nDateType = zDT_YEAR;
   else

   // Case: Month
   if ( zstrcmp( lpDomainData->pszContextName, "Month" ) == 0 )
      nDateType = zDT_MONTH;
   else

   // Case: Day
   if ( zstrcmp( lpDomainData->pszContextName, "Day" ) == 0 )
      nDateType = zDT_DAY;
   else

   // Case: Hour
   if ( zstrcmp( lpDomainData->pszContextName, "Hour" ) == 0 )
      nDateType = zDT_HOUR;
   else

   // Case: Minute
   if ( zstrcmp( lpDomainData->pszContextName, "Minute" ) == 0 )
      nDateType = zDT_MINUTE;
   else

   // Case: Second
   if ( zstrcmp( lpDomainData->pszContextName, "Second" ) == 0 )
      nDateType = zDT_SECOND;

   else
   {
      // Invalid Context for Add operation.

      strcpy_s( sz, zsizeof( sz ), lpDomainData->pszContextName );
      n = (zSHORT) zstrlen( sz );
      sz[ n ] = '/';
      sz[ n + 1 ] = zTYPE_INTEGER;
      sz[ n + 2 ] = 0;
      // "TZDME013 - Context/cType Combination is invalid "
      SendDomainError( lpDomainData->zView, lpDomain, 8, 13, 0, sz, 0 );
      return( zCALL_ERROR );
   }

   // save original value for later usage...
   strcpy_s( szDT1, zsizeof( szDT1 ), szDT2 );

   UfStringToDateTime( szDT2, &stDT );

   nRC = UfAddToDateTime( &stDT, *((zPLONG) lpDomainData->lpData), nDateType );

   // If domain is Date or Time, retrieve the portion that will not be changing and place into string
   if ( nDomain != eDM_DateTime && nRC == 0 )
   {
      UfDateTimeToString( &stDT, szDT2, zsizeof( szDT2 ) );
      switch ( nDomain )
      {
         case eDM_Date:
            // When working with the Date domain leave the time portion as is.
            zmemcpy( szDT2 + 8, szDT1 + 8, 10 );
            break;

         case eDM_Time:
            // When working with the Time domain leave the date portion as is.
            zmemcpy( szDT2, szDT1, 8 );
            break;
      }

      UfStringToDateTime( szDT2, &stDT );
   }

   if ( nRC == 0 )
      nRC = StoreValueInRecord( lpDomainData->zView, lpDomainData->lpViewEntity, lpDomainData->lpViewAttribute, &stDT, 0 );

   return( nRC );
}

// analyze date string and transform to Zeidon standard
// allowed  is
// 22.10.1999 22-10-1999 22/10/1999
// 1.3.98
// 1999/10/22 1999-10-22 1999/10/22
// 22101999
// 221099 010101
static zSHORT NormalizeFreeFormDate( zPCHAR sz )
{
   zCHAR szTemp[ 256 ];
   zCHAR cSeparator = 0;
   zPCHAR pc, pc2;
   zLONG l1, l2, l3, l4;

   strcpy_s( szTemp, zsizeof( szTemp ), sz );
   if ( (pc = strpbrk( szTemp, "./-" )) != NULL )
   {
      cSeparator = *pc;
      // the separator must occur twice
      if ( (pc2 = strchr( pc+1, cSeparator )) == NULL )
         return( -1 );

      l1 = ExtractNumber( szTemp, cSeparator );
      l2 = ExtractNumber( pc+1, cSeparator );
      l3 = ExtractNumber( pc2+1, cSeparator );
      // If we got a separator, then the year can be l1 or l3.
      // If l1 or l3 is bigger than 1000, it is clear...???  don't understand this comment
      // If both are less than 100, then it must be l3.
      if ( l1 >= 100 )
      {
         // exchange, so that l3=year and l1=day
         l4 = l1;
         l1 = l3;
         l3 = l4;
      }
      else
      if ( l3 >= 100 )
         ; // nothing to do
      else
         l3 = LongYearFromShort( l3 );

      if ( kDateFormat == 2 || kDateFormat == 3 )
      {
         // American format, first month, then day
         l4 = l2;
         l2 = l1;
         l1 = l4;
      }
   }
   else
   {
      // short form without separator
      zCHAR  szTemp[ 3 ] = { 0 };
      zSHORT k;
      zLONG  lLength = zstrlen( sz );

      switch ( lLength )
      {
         case 6:
         case 8:
            break; // those are the only valid numbers
         default:
            return( -1 ); // error
      }

      for ( k = 0; k < lLength; k++ )
      {
         if ( !isdigit( sz[ k ] ) )
            return( -1 ); // error, nothing but digits allowed
      }

      // date without separator
      //  ddmmyy or ddmmyyyy for german format ( kDateFormat=0,1 )
      //  mmddyy or mmddyyyy for american format ( kDateFormat=2,3 )
      memcpy( szTemp, sz, 2 );
      l1 = zatol( szTemp ); // day
      memcpy( szTemp, sz +2, 2 );
      l2 = zatol( szTemp ); // month
      if ( kDateFormat >= 2 )
      {
         l4 = l1;
         l1 = l2;
         l2 = l4;
      }

      l3 = zatol( sz + 4 ); // year
      if ( lLength == 6 )
      {
         l3 = LongYearFromShort( l3 );
      }
   }

   if ( CheckDate( l3, l2, l1 ) )
      return( -1 ); // error

   sprintf_s( sz, zsizeof( sz ), "%04ld", l3 ); // year
   sprintf_s( sz + 4, zsizeof( sz ) - 4, "%02ld", l2 ); // month
   sprintf_s( sz + 6, zsizeof( sz ) - 6, "%02ld", l1 ); // day

   return( 0 );
}

// analyze time string and transform to Zeidon standard
// allowed  is
// 12:33.44.11 12:33.44 12:33 12
// 8 am 8:22 am 8:22:44 am 8.22.33.444 am
static zSHORT
NormalizeFreeFormTime( zPCHAR sz )
{
   zCHAR  szTemp[ 256 ];
   zCHAR  cSeparator = 0;
   zPCHAR pc, pc2;
   zLONG  l1, l2, l3, l4;
   zULONG ulLth = zstrlen( sz );
   zULONG k;
   zSHORT nAPM = 0;

   strcpy_s( szTemp, zsizeof( szTemp ), sz + strspn( sz, " " ) ); // copy without leading blanks

   // test for illegal characters
   if ( strspn( szTemp, "0123456789aAmMpP:., " ) < ulLth )
      return( -1 ); // error no other zCHAR allowed

   // strip trailing blanks
   pc = szTemp + ulLth -1;
   while ( pc >= szTemp && *pc == ' ' )
   {
      *pc = 0;
      --pc;
   }

   // check for " am" or " pm"
   if ( (pc2 = strchr( szTemp, ' ' )) != NULL )
   {
      pc = pc2 +strspn( pc2, " " ); // eliminate blanks
      if ( *pc == 'a' || *pc == 'A' )
         nAPM = 1;  // am
      else
      if ( *pc == 'p' || *pc == 'P' )
         nAPM = 2; // pm
      else
        return( -1 ); // erroneous format

      if ( *(pc + 1) != 'm' && *(pc + 1) != 'M' )
        return( -1 ); // erroneous format

      *pc2 = 0; // cut off
   }

   ulLth = zstrlen( szTemp );

   if ( (pc = strpbrk( szTemp, ":.," )) == NULL )
   {
      zCHAR szx[ 3 ] = { 0 };

      // error if non digit found
      for ( k = 0; k < ulLth; k++ )
         if ( !isdigit( szTemp[ k ] ) )
            return( -1 );

      // no separator
      //  valid strings are
      // 8 12 1230 123044 083040999
      switch ( ulLth )
      {
      case 1: // h
      case 2: // hh
         l1 = zatol( szTemp ); // hour
         l2 = l3 = l4 = 0;
         break;
      case 3: // hmm - first digit is the hour
         l1 = (int) szTemp[ 0 ] - (int)'0';
         l2 = zatol( szTemp +1 ); //digits 2,3 are minute
         l3 = l4 = 0;
         break;
      case 4: // hhmm
         memcpy( szx, szTemp, 2 ); // hh
         l1 = zatol( szx );
         l2 = zatol( szTemp +2 ); // mm
         l3 = l4 = 0;
         break;
      case 5: // hmmss - first digit is the hour
         l1 = (int) szTemp[ 0 ] - (int)'0';
         memcpy( szx, szTemp+1, 2 ); // mm
         l2 = zatol( szx );
         l3 = zatol( szTemp +3 ); // ss
         l4 = 0;
         break;
      case 6: // hhmmss
         memcpy( szx, szTemp, 2 ); // hh
         l1 = zatol( szx );
         memcpy( szx, szTemp +2, 2 ); // mm
         l2 = zatol( szx );
         l3 = zatol( szTemp +4 ); // ss
         l4 = 0;
         break;
      case 8: // hmmssxxx
         memcpy( szx, szTemp, 1 ); // hh
         l1 = zatol( szx );
         memcpy( szx, szTemp +1, 2 ); // mm
         l2 = zatol( szx );
         memcpy( szx, szTemp +3, 2 ); // ss
         l3 = zatol( szx );
         l4 = zatol( szTemp +5);
         break;
      case 9: // hhmmssxxx
         memcpy( szx, szTemp, 2 ); // hh
         l1 = zatol( szx );
         memcpy( szx, szTemp +2, 2 ); // mm
         l2 = zatol( szx );
         memcpy( szx, szTemp +4, 2 ); // ss
         l3 = zatol( szx );
         l4 = zatol( szTemp +6);
         break;
      default:
         return( -1 );
      }
   }
   else
   {
      // separators in time format
      l1 = ExtractNumber( szTemp, *pc );
      if ( (pc2 = strpbrk( pc + 1, ":.," )) == NULL )
      {
         // 12:30
         l2 = zatol( pc + 1 );
         l3 = l4 = 0;
      }
      else
      {
         l2 = ExtractNumber( pc + 1, *pc2 );
         if ( (pc = strpbrk( pc2 + 1, ":.," )) == NULL )
         {
            // 12:30:44
            l3 = zatol( pc2 + 1 );
            l4 = 0;
         }
         else
         {
            // 12:30:44.999
            l3 = ExtractNumber( pc2 +1, *pc );
            l4 = zatol( pc +1 );
         }
      }
   }

   if ( CheckTime( &l1, l2, l3, l4, nAPM ) )
      return( -1 ); // error

   sprintf_s( sz + 8, zsizeof( sz ) - 8, "%02ld", l1 ); // hour
   sprintf_s( sz + 10, zsizeof( sz ) - 10, "%02ld", l2 ); // minute
   sprintf_s( sz + 12, zsizeof( sz ) - 12, "%02ld", l3 ); // second
   sprintf_s( sz + 14, zsizeof( sz ) - 14, "%03ld", l4 ); // millisecond

   return( 0 );
}


static zLONG
ExtractNumber( zPCHAR pc, zCHAR cSep )
{
   zLONG k;
   zLONG l = 0;

   for ( k = 0; pc[ k ] && pc[ k ] != cSep; k++ )
   {
      if ( isdigit( pc[ k ] ) )
         l = l * 10 + (int) pc[ k ] - (int) '0';
      else
         return( -1 );
   }

   return( l );
}

// test for valid date
//  return( 0 ) ==> OK
//         otherwise Error
static zSHORT
CheckDate( zLONG lYear, zLONG lMonth, zLONG lDay )
{
   if ( lYear <= 0 || lYear > 10000 )
      return( 1 );

   if ( lMonth <=0 || lMonth > 12 )
      return( 1 );

   if ( lDay <=0 || lDay > 31 )
      return( 1 );

   switch ( lMonth )
   {
      case 4:
      case 6:
      case 9:
      case 11:
         // April,June,September,November
         if ( lDay > 30 )
            return( 1 );

         break;

      case 2:
         // February
         if ( lDay > 29 )
            return( 1 );

         if ( lDay == 29 )
         {
            // Leap Year test
            if ( lYear % 4 != 0 )
               return( 1 );

            if ( lYear % 400 == 0 ) // exception from exception
               return( 0 );

            if ( lYear % 100 == 0 ) // exception
               return( 1 );
         }
   }

   return( 0 );
}

// test for valid date
//  return( 0 ) ==> OK
//         otherwise Error
static zSHORT
CheckTime( zLONG *plHour, zLONG lMinute, zLONG lSecond,
           zLONG lMilliSecond, zSHORT nAPM )
{
   if ( (*plHour > 13 && nAPM == 1 ) || *plHour >= 24 )
      return( 1 ); // if am, then hour must be < 13

   if ( *plHour <= 12 && nAPM == 2 )
      *plHour = (*plHour + 12) % 24;

   if ( lMinute >= 60 )
      return( 1 );

   if ( lSecond >= 60 )
      return( 1 );

   if ( lMilliSecond >= 1000 )
      return( 1 );

   return( 0 );
}

// get long year from short using time window
static zLONG
LongYearFromShort( zLONG lYear )
{
   static zLONG lCurrent = 0;
   zCHAR szCurrentDate[ 32 ];
   zLONG l1, l2;

   if ( lCurrent == 0 )
   {
      SysGetDateTime( szCurrentDate, zsizeof( szCurrentDate ) );
      szCurrentDate[ 4 ] = 0;
      lCurrent = atol( szCurrentDate );
   }

   // time window is 70 years back and 29 years in advance starting from the decade.
   l1 = lCurrent / 10;
   l1 = l1 * 10;
   l2 = lCurrent / 100;
   l2 = l2 * 100;
   if ( l2 + lYear - l1 > 29 )
      return( l2 + lYear - 100 );
   else
      return( l2 + lYear );
}

// Format date string according to edit string
//  valid edit strings are:
//  dd.mm.yyyy dd/mm/yyyy dd-mm-yyyy
//  yyyy.mm.dd yyyy/mm/dd yyyy-mm-dd
static zSHORT
FormatFreeFormDate( zPCHAR  pchTarget,
                    zLONG   lMaxLth,
                    zCPCHAR pchZeidon,
                    zCPCHAR pchEditString )
{
   // date
   if ( !cDateSeparator )
      return( -1 );

   switch ( kDateFormat )
   {
      case 0:  // dd.mm.yyyy
         strncpy_s( pchTarget, lMaxLth, pchZeidon + 6, 2 ); // dd
         pchTarget[ 2 ] = 0;
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon + 4, 2 ); // mm
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon, 4 ); // yyyy
         break;

      case 1:  // yyyy.mm.dd
         strncpy_s( pchTarget, lMaxLth, pchZeidon, 4 ); // yyyy
         pchTarget[ 4 ] = 0;
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon + 4, 2 ); // mm
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon + 6, 2 ); // dd
         break;

      case 2:  // mm.dd.yyyy (US)
         strncpy_s( pchTarget, lMaxLth, pchZeidon + 4, 2 ); // mm
         pchTarget[ 2] = 0;
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon + 6, 2 ); // dd
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon, 4 ); // yyyy
         break;

      case 3:  // yyyy.dd.mm (US)
         strncpy_s( pchTarget, lMaxLth, pchZeidon, 4 ); // yyyy
         pchTarget[ 4 ] = 0;
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon + 4, 2 ); // mm
         strncat_s( pchTarget, lMaxLth, &cDateSeparator, 1 );
         strncat_s( pchTarget, lMaxLth, pchZeidon + 6, 2 ); // dd
         break;

      default:
         return( -1 ); // error, does not occur
   }

   return( 0 );
}

// format time string
// hh:mm:ss.sss [pm]
static zSHORT
FormatFreeFormTime( zPCHAR  pchTarget,
                    zLONG   lMaxLth,
                    zCPCHAR pchZeidon,
                    zCPCHAR pchEditString )
{
   int iPM = 0;
   zCPCHAR pc;

   strncpy_s( pchTarget, lMaxLth, pchZeidon + 8, 2 ); // hh
   pchTarget[ 2 ] = 0;
   if ( kTimeAM )
   {
      int k = atoi( pchTarget );
      if ( k > 12 )
      {
         k = k - 12;
         iPM = 1;
         sprintf_s( pchTarget, lMaxLth, "%02d", k );
      }
   }

   strncat_s( pchTarget, lMaxLth, cTimeSeparator, 1 );
   strncat_s( pchTarget, lMaxLth, pchZeidon + 10, 2 ); // mm

   // write seconds only if > 0
   pc = pchZeidon + 12;
   while ( *pc == '0' )
     ++pc;

   if ( *pc )
   {
      strncat_s( pchTarget, lMaxLth, cTimeSeparator + 1, 1 );
      strncat_s( pchTarget, lMaxLth, pchZeidon + 12, 2 ); // ss
   }

   // write milliseconds only if > 0
   pc = pchZeidon + 14;
   while ( *pc == '0' )
     ++pc;

   if ( *pc )
   {
      strncat_s( pchTarget, lMaxLth, cTimeSeparator + 2, 1 );
      strncat_s( pchTarget, lMaxLth, pchZeidon + 14, 3 ); // sss
   }

   if ( iPM )
      strcat_s( pchTarget, lMaxLth, " PM" );
   else
   if ( kTimeAM )
      strcat_s( pchTarget, lMaxLth, " AM" );

   return( 0 );
}

// analyze edit string for date or time
static void
CheckEditString( zCPCHAR pchEditString,
                 LPDOMAINDATA lpDomainData, zLPCONTEXT lpContext)
{
   LPDOMAIN lpDomain = SysGetPointerFromHandle( lpDomainData->lpViewAttribute->hDomain );
   zCPCHAR pc, pc2;
   int iDate = 0;

   if ( zstrcmp( "Date", lpDomain->szName) == 0 )
   {
      // date
      // set kDateFormat
      //     0   for dd.mm.yyyy
      //     1   for yyyy.mm.dd
      //     2   for mm.dd.yyyy (US)
      //     3   for yyyy.dd.mm (US)
      //    -1   otherwise

      // for performance execute only one time
      if ( bDateEditStringChecked )
         return;
      bDateEditStringChecked = 1;

#ifdef __WIN32__
      // if "*" is the format, we get the windows standard
      if ( pchEditString[ 0 ] == '*' )
      {
         zCHAR szBuffer[ 16 ];
         GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_IDATE, szBuffer, zsizeof( szBuffer ) );
         switch ( szBuffer[ 0 ] )
         {
         case '0': // Month-Day-Year
            kDateFormat = 2;  // mm.dd.yyyy
            break;
         case '1': // Day-Month-Year
            kDateFormat = 0;  // dd.mm.yyyy
            break;
         case '2': // Year-Month-Day
            kDateFormat = 1;  // yyyy.mm.dd
            break;
         default: // should never occur
            kDateFormat = -1;
            break;
         }
         // LOCALE_SDATE = date separator
         GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDATE, szBuffer, zsizeof( szBuffer ) );
         cDateSeparator = szBuffer[ 0 ];

//       GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, pchEditString, zsizeof( szBuffer ) );
         return;
      }
#endif

      if ( (pc = strpbrk( pchEditString, "./-" )) != NULL )
      {
         cDateSeparator = *pc;
         // separator must occur twice
         if ( (pc2 = strchr( pc+1, cDateSeparator )) == NULL )
             cDateSeparator = 0;
      }

      if ( cDateSeparator == 0 )
      {
         // error, invalid edit string
         // "TZDME009 - Context edit string is  invalid for Domain "
         SendDomainError( lpDomainData->zView, lpDomain, 8, 9, 0, lpContext->szName, pchEditString );

         cDateSeparator = '.';
      }

      if ( pchEditString[ 0 ] == 'Y' || pchEditString[ 0 ] == 'y' )
      {
         pc = strchr( pchEditString, cDateSeparator );
         if ( !pc )
            kDateFormat = -1;
         if ( pc[ 1 ] == 'M' || pc[ 1 ] == 'm' )
            kDateFormat = 1;  // yyyy.mm.dd
         else
         if ( pc[ 1 ] == 'D' || pc[ 1 ] == 'd' )
            kDateFormat = 3;  // yyyy.dd.mm
         else
            kDateFormat = -1;
      }
      else
      if ( pchEditString[ 0 ] == 'D' || pchEditString[ 0 ] == 'd' )
         kDateFormat = 0;  // dd.mm.yyyy
      else
      if ( pchEditString[ 0 ] == 'M' || pchEditString[ 0 ] == 'm' )
         kDateFormat = 2;  // mm.dd.yyyy
      else
         kDateFormat = -1;
   }
   else
   {
      int iErr = 0;

      // Time
      if ( bTimeEditStringChecked )
         return;
      bTimeEditStringChecked = 1;

#ifdef __WIN32__
      // if "*" is the format, we get the windows standard
      if ( pchEditString[ 0 ] == '*' )
      {
         zCHAR szBuffer[ 16 ];
         GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_STIME, szBuffer, zsizeof( szBuffer ) );
         cTimeSeparator[ 0 ] = cTimeSeparator[ 1 ] = szBuffer[ 0 ];
         cTimeSeparator[ 2 ] = '.';
         GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, zsizeof( szBuffer ) );
         switch( szBuffer[ 0 ] )
         {
         case '0' : // AM/PM
            kTimeAM = 1;
            break;
         case '1' : // 24 hour
         default:
            kTimeAM = 0;
            break;
         }
//       GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, pchEditString, zsizeof( pchEditString ) );
         return;
      }
#endif

      // edit string is hh:mm:ss[.s] [am]
      pc = pchEditString;
      while ( *pc == 'h' || *pc == 'H' )
        ++pc;
      if ( pc - pchEditString != 2 ) // exactly 2 h's expected
         ++iErr;

      // valid separators are :(colon) .(dot),(comma)
      if ( strchr( ":.,", *pc ) != NULL )
         cTimeSeparator[ 0 ] = *pc;
      else
      {
         ++iErr;
         cTimeSeparator[ 0 ] = ':';
      }
      ++pc;

      while ( *pc == 'm' || *pc == 'M' )
        ++pc;
      if ( pc - pchEditString != 5 ) // exactly 2 m's expected
         ++iErr;

      if ( strchr( ":.,", *pc ) != NULL )
         cTimeSeparator[ 1 ] = *pc;
      else
      {
         ++iErr;
         cTimeSeparator[ 1 ] = ':';
      }
      ++pc;

      while ( *pc == 's' || *pc == 'S' )
        ++pc;
      if ( pc - pchEditString != 8 ) // exactly 2 s's expected
         ++iErr;

      if ( *pc == ' ' || *pc == 0 )
      {
         cTimeSeparator[ 2 ] = '.';
      }
      else
      if ( strchr( ":.,", *pc ) != NULL )
         cTimeSeparator[ 2 ] = *pc;
      else
      {
         ++iErr;
         cTimeSeparator[ 2 ] = '.';
      }

      pc = strpbrk( pc, " " ); // advance to next blank
      if ( pc )
      {
         // skip over blanks and check for "am"
         pc += strspn( pc, " " );
         if ( (*pc == 'a' || *pc == 'A' || *pc == 'p' || *pc == 'P') && (*(pc + 1) == 'm' || *(pc + 1) == 'M') )
         {
            kTimeAM = 1;
         }
      }

      if ( iErr > 0 )
      {
         // error, invalid edit string
         // "TZDME009 - Context edit string is invalid for Domain "
         SendDomainError( lpDomainData->zView, lpDomain, 8, 9, 0, lpContext->szName, pchEditString );
      }
   }
}
