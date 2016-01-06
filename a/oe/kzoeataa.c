#include "kzoephdr.h"
//
// MODULE NAME:  KZOEATAA  -  Object services Attribute data Management
// DESCRIPTION:  This is the source file which contains Object services
//               operations for getting and setting attributes
// OPERATIONS:   All OPERATIONS which retrieve and update attributes
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    4/13/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2003.03.11 DC
   Modified DfExecuteXPG operation to cache the vXPG object instance.

2002.11.13  DGC
   Added code to call the interpretor.  It is called via a dynmaic link.

2002.05.21  HH
   changed assignment logic (SetAttributeFromAttribute) for table domains.
   SetAttributeFromAttribute will always use the internal value if a table
   domain attribute is set from another table domain (Don!).

2002.03.29  DGC
   Use SysCreateTempFileName( ).

2002.03.04  HH
   rewrote assignment (SetAttributeFromAttribute) for table domains.

2002.02.11  ASE
   Added an fnOperationReturn( ) in SetAttributeFromAttribute to prevent
   leftovers in the nOper stack, resulting in "Too many nested core calls".

2001.11.16  DGC
   Added code to handle subtraction with decimals in DfExprVML( ).

2001.09.07  DGC
   Fixed typo in fnValidateAttributeParameters( ) that didn't allow work
   attributes to be changed.

2001.09.07  TMV
   changed GetFirstTableEntryForAttribute and GetNextTableEntryForAttribute
      pass zACCEPT_NULL_ENTITY when calling fnValidateAttributeParameters
      because there is no need that an entity instance exists when
      table domain values are retrieved.

2001.09.05  DGC
   Fixed update flag bug reported by Phil.

2001.07.31  HH
   Improved Update Flags for versioned entities.

2001.07.30  DGC  10b
   Added SetBlobFromFile and WriteBlobToFile.

2001.06.29  DGC
   Fixed bug introduced by previous change.

2001.06.08  PAS,BL,HH
   Improve compare attribute to attribute if both are strings.

2001.04.20  DGC
   Added flag to fnValidViewAttrib( ) to allow hidden attributes.

2001.03.29  PAS
   Changed fnStoreBlobInRecord to accept a NULL pointer to the blob value as
   an indicator to allocate the storage for the blob and initialize it to nulls.

2001.02.13  HH
   Improve comparison of integer or decimal to string.

2000.01.09  DGC  10b
   Fixed crash when a child cursor is undefined when getting attribute values.

2000.12.14  DGC  10b
   Change requested by Jeff.  We now allow updates of derived entities even
   if the view is read-only.

2000.12.01  DGC  10b
   Fix for important bug reported by Frank H.  When an entity was being
   excluded it was being flagged as updated.

2000.11.28  HH
   const char change StoreStringInRecord.

2000.09.01  DGC  10b
   Two changes:
      - Made fix to correctly set the OI update flag.
      - Default context wasn't always being used correctly.  Now it does.

2000.09.01  DGC  10b
   Added functionality to SetOI_FromBlob( ).

2000.05.18  DGC  10b
   Derived attrs no longer cause a change in the bUpdate flags.

2000.04.08  DGC  10b
   Changed signature for fnSetUpdateIndicator( ).

2000.05.04  DGC  all
   Added StoreBytesInRecord( ).

2000.04.04  DGC ALL
   Added an error message when retrieving an OI from a blob.

2000.02.02  DGC ALL
   Fixed bug reported by Phil.  When an entity instance is updated we need to
   change the updated flag for all linked entity instances.

2000.02.02  DGC ALL
   Fixed bug reported by Frank Becker--set datetime to null.
   Fixed compare bug...in CompareAttributeToAttribute( ) we were'nt correctly
   checking for null attributes when comparing decimals.

2000.01.26  DGC ALL
   Check to make sure that the domain DLL was loaded.  If not return with
   error.

1999.12.06  DGC
   Small change to SetOI_FromBlob( ).

1999.11.29  DGC
   Added code to fnAllocDataspace to help find leaks.

1999.10.12  HH  10a
   corrections to remove compiler warnings under MVS.

1999.06.01  DGC  10a
   Allow updating work attributes even if entity is not updateable or view
   is read-only.

1999.06.01  DGC  10a
   Added GetAttributeFlags( ).
   Removed references to fnValidRootInstance( ).

1999.04.09  DGC  10a
   Added support for internal derived attribute concat functions.

1999.04.06  DGC  10a
   Changed domain interface some.

1999.02.12  DGC
   Started adding (and removing) stuff for attribute flags.

1999.01.26  DGC
   Fixed attribute display length to return 25 for date.

1999.01.07  DGC
   Changed logic of GetAttributeDisplayLth( ).

*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#include <limits.h>       // for checking overflow
#include <math.h>         // for floor
#include <float.h>        // for DBL_DIG
#include <ctype.h>        // isalpha, isdigit, etc

zSHORT OPERATION
fnActivateOI_FromBuffer( zPVIEW   pvReturnView,
                         zCPCHAR  cpcViewOD_Name,
                         zVIEW    lpAppQualView,
                         zPCHAR   pchBuffer,
                         zULONG   uLth,
                         zLONG    lControl );

// Following struct used by SetBlobFromOI( ) and SetOI_FromBlob( ).
struct OI_AsBlobInfoStruct
{
   zCHAR    cpcViewOD_Name[ zZEIDON_NAME_LTH + 1 ];
   zLONG    lDummy;
   zCHAR    szDummy[ 100 ];
};
typedef struct OI_AsBlobInfoStruct BlobOI_Record;
typedef BlobOI_Record * LPBLOBOI_RECORD;

zPCHAR LOCALOPER
fnEntityNameFromCsr( LPVIEWENTITYCSR lpViewEntityCsr )
{
   LPVIEWENTITY lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );
   return( lpViewEntity->szName );
}

zPCHAR LOCALOPER
fnEntityNameFromInstance( LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
   return( lpViewEntity->szName );
}

//./ ADD NAME=fnGetAttrAddrFromEntityInstance
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnGetAttrAddrFromEntityInstance
//
//  PURPOSE:    To return a pointer to the attribute's value in an entity
//              instance.
//
//  PARAMETERS: lpReturnAddress  - Return pointer to attribute value
//              uReturnLength    - Attribute length
//              lpEntityInstance - Entity instance
//              lpViewAttrib     - Attribute to retrieve
//
//  RETURNS:   -1           - Attrib addr retrieved, but attrib is NULL
//              0           - Attribute address successfully retrieved
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnGetAttrAddrFromEntityInstance( zCOREMEM         lpReturnAddr,
                                 zPULONG          pulReturnLth,
                                 LPENTITYINSTANCE lpEntityInstance,
                                 LPVIEWATTRIB     lpViewAttribute )
{
   zPCHAR   pchRecord;
   LPVIEWOI lpViewOI = zGETPTR( lpEntityInstance->hViewOI );
   zSHORT   nRC;

   // Get record to retrieve from.
   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttribute );

   if ( pchRecord == 0 )
   {
      *lpReturnAddr = 0;
      *pulReturnLth = 0;
      nRC = -1;
   }
   else
   {
      switch ( lpViewAttribute->cType )
      {
         case zTYPE_STRING:
         {
            zPCHAR   pchRecordString;
            zCOREMEM lpExternalP;
            zPULONG  lpuLong;

            pchRecordString = pchRecord + lpViewAttribute->ulRecordOffset;
            if ( pchRecordString[ 0 ] == (zCHAR) '\xff' )
            {
               lpExternalP = (zCOREMEM) (pchRecordString + 1);
               lpuLong = (zPULONG) (lpExternalP + 1);
               *pulReturnLth = *lpuLong;
               pchRecordString = zGETPTR( *lpExternalP );
               *lpReturnAddr = pchRecordString;
            }
            else
            {
               *lpReturnAddr = pchRecordString;
               *pulReturnLth = zstrlen( pchRecordString ) + 1;
            }

            nRC = (*pulReturnLth == 1) ? -1 : 0;
            break;
         }

         case zTYPE_INTEGER:
            *lpReturnAddr = (pchRecord + lpViewAttribute->ulRecordOffset);
            *pulReturnLth = 0;
            if ( *(zPLONG) *(zPLONG *) lpReturnAddr == lNullInteger )
               nRC = -1;
            else
               nRC = 0;

            break;

         case zTYPE_DECIMAL:
            *lpReturnAddr = (pchRecord + lpViewAttribute->ulRecordOffset);
            *pulReturnLth = sizeof( zDECIMAL );
            if ( SysCompareDecimalToNull( *((zPPDECIMAL) lpReturnAddr) ) == 0 )
               nRC = -1;
            else
               nRC = 0;

            break;

         case zTYPE_BLOB:
         {
            zPCHAR   pchBlob;
            zCOREMEM lpExternalP;
            zPULONG  lpuLong;

            pchBlob = pchRecord + lpViewAttribute->ulRecordOffset;
            if ( pchBlob[ 0 ] == (zCHAR) '\xff' )
            {
               lpExternalP = (zCOREMEM) (pchBlob + 1);
               lpuLong = (zPULONG) (lpExternalP + 1);
               *pulReturnLth = *lpuLong;
               pchBlob = zGETPTR( *lpExternalP );
               *lpReturnAddr = pchBlob;
               nRC = 0;
            }
            else
            {
               *pulReturnLth = 0;
               nRC = -1;
            }

            break;
         }

         case zTYPE_DATETIME:
            *lpReturnAddr = (pchRecord + lpViewAttribute->ulRecordOffset);
            *pulReturnLth = sizeof( DateTimeRecord );
            if ( fnCompareDateTimeToDateTime(
                           *((LPDTINTERNAL *) lpReturnAddr),
                           (LPDTINTERNAL) &stNullDateTime ) == 0 )
               nRC = -1;
            else
               nRC = 0;
            break;

         default:
            nRC = zCALL_ERROR;

      } // switch ( lpViewAttribute->cType )...
   } // if ( pchRecord == 0 )...else...

   return( nRC );

} // fnGetAttrAddrFromEntityInstance

LPATTRIBFLAGS LOCALOPER
fnGetAttribFlagsPtr( LPENTITYINSTANCE lpEntityInstance,
                     LPVIEWATTRIB     lpViewAttrib )
{
   zPCHAR   pchRecord;

   // Get record to retrieve from.
   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   // If we didn't find a Record then create one.
   if ( pchRecord == 0 )
   {
      LPVIEWOI     lpViewOI     = zGETPTR( lpEntityInstance->hViewOI );
      LPTASK       lpAllocTask  = zGETPTR( lpViewOI->hAllocTask );
      LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

      // Allocate the data records (data sticks).
      if ( lpViewAttrib->bPersist )
      {
         fnCreateAttributeRecord( lpAllocTask, lpViewEntity,
                                  lpEntityInstance );
      }
      else
      {
         lpEntityInstance->hNonPersistRecord =
                     fnCreateAttributeRecord( lpAllocTask, lpViewEntity, 0 );
      }

      // Try to get the pchRecord again.  It had better be there!
      pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );
      if ( pchRecord == 0 )
         return( 0 );
   }

   return( (LPATTRIBFLAGS) (pchRecord +
               lpViewAttrib->ulRecordOffset - sizeof( AttribFlagsRecord )) );

} // fnGetAttribFlagsPtr

//./ ADD NAME=GetAttributeFlags
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetAttributeFlags
//
//  PURPOSE:    To retrieve a pointer to the attribute flags structure.
//
//  PARAMETERS: plpAttribFlags   - Pointer to return flag pointer.
//              lpView           - View to Object instance
//              lpViewEntity     - Entity containing attribute to retrieve
//              lpViewAttrib     - Attribute to retrieve
//
//  RETURNS:    0           - Attribute flags successfully retrieved
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetAttributeFlags( LPATTRIBFLAGS *plpAttribFlags,
                   zVIEW         lpView,
                   LPVIEWENTITY  lpViewEntity,
                   LPVIEWATTRIB  lpViewAttrib )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   zSHORT            nRC;

   *plpAttribFlags = 0;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iGetAttributeFlags, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   do   // purist's goto
   {
      // Set lpViewCsr.
      lpViewCsr = zGETPTR( lpView->hViewCsr );
      lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

      // Validate that the entity exists.
      if ( fnValidViewEntity( &lpViewEntityCsr, lpView,
                              lpViewEntity->szName, 0 ) == 0 )
      {
         nRC = zCALL_ERROR;
         break;
      }

      *plpAttribFlags =
            fnGetAttribFlagsPtr( zGETPTR( lpViewEntityCsr->hEntityInstance ),
                                 lpViewAttrib );
      nRC = 0;

   } while ( FALSE );

   fnOperationReturn( iGetAttributeFlags, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=AttributeUpdated
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      AttributeUpdated
//
//  PURPOSE:    To retrieve a pointer to the attribute flags structure.
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to retrieve
//
//  RETURNS:    TRUE        - Attribute updated.
//              FALSE       - Attribute not updated.
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
AttributeUpdated( zVIEW   lpView,
                  zPCHAR  pchEntityName,
                  zPCHAR  pchAttribName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   LPATTRIBFLAGS     lpAttribFlags;
   zSHORT            nRC;

   // Validate parameters.
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iAttributeUpdated,
                                        0,   // We do not intend to update
                                        lpView,
                                        pchEntityName,
                                        pchAttribName, 0 );

   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
// if ( nRC == 0 )
   {
      lpAttribFlags =
            fnGetAttribFlagsPtr( zGETPTR( lpViewEntityCsr->hEntityInstance ),
                                 lpViewAttrib );

      if ( lpAttribFlags )
         nRC = lpAttribFlags->u.bFlags.bUpdated;
   }

   fnOperationReturn( iAttributeUpdated, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnStoreValueInEntityInstance
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnStoreValueInEntityInstance
//
//  PURPOSE:    To return a pointer to the attribute's value in an entity
//              instance.  Returns NULL if attribute is NULL.
//
//  PARAMETERS: lpEntityInstance - Entity instance
//              lpViewAttrib     - Attribute to retrieve
//              lpPtr            - Pointer to attribute value.
//              ulLength         - Attribute length (for Blobs only).
//
//  RETURNS:    0           - Attribute address successfully retrieved
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnStoreValueInEntityInstance( LPENTITYINSTANCE lpEntityInstance,
                              LPVIEWENTITY     lpViewEntity,
                              LPVIEWATTRIB     lpViewAttribute,
                              zPVOID           lpValue,
                              zULONG           ulLength )
{
   zBOOL        bUpdated;
   zPCHAR       pchRecord;
   LPVIEWOI     lpViewOI;
   LPTASK       lpAllocTask;
   zULONG       ulLth;
   zPCHAR       pchAttribValue;
   zSHORT       nRC;

   lpViewOI = zGETPTR( lpEntityInstance->hViewOI );

   // Get Task owning the view object instance
   lpAllocTask = zGETPTR( lpViewOI->hAllocTask );

   // Get record to store into
   if ( lpViewAttribute->bPersist ) // is attribute persistent
   {
      pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
      if ( pchRecord == 0 )
      {
         fnCreateAttributeRecord( lpAllocTask,
                                  lpViewEntity, lpEntityInstance );
         pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
         if ( pchRecord == 0 )
            return( zCALL_ERROR );
      }
   }
   else
   {
      pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord );
      if ( pchRecord == 0 )
      {
         lpEntityInstance->hNonPersistRecord =
                     fnCreateAttributeRecord( lpAllocTask, lpViewEntity, 0 );
         pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord );
         if ( pchRecord == 0 )
            return( zCALL_ERROR );
      }
      else
      {
         // dks 2004.09.30 ... not persistent, so should always be updated.
         // dks 2004.09.30 ... not sure this is needed so will comment out for now.
      // *(pchRecord + lpViewAttribute->ulRecordOffset) = 0;
      }
   }

   // Set attrib from string.
   nRC = 0;
   bUpdated = FALSE;

   switch ( lpViewAttribute->cType )
   {
      case zTYPE_STRING:
      {
         zSHORT k;

         if ( (zPCHAR) lpValue )
            pchAttribValue = (zPCHAR) lpValue;
         else
            pchAttribValue = szNullS;

         ulLth = zstrlen( pchAttribValue );

         // truncate string if necessary
         if ( ulLth >= lpViewAttribute->ulLth )
            ulLth = lpViewAttribute->ulLth - 1;      // subtract 1 for null

         k = fnStoreStringInRecord( lpAllocTask, // so allocation can be done
                                    pchRecord,       // Attribute record
                                    lpViewAttribute->ulRecordOffset,   // Offset
                                    pchAttribValue,  // Attribute value to store
                                    ulLth,           // Maximum length to store
                                    1 );             // Null terminate data
         // Check return code
         if ( k )
         {
            if ( k == 1 )
               bUpdated = TRUE;
            else
               nRC = k;
         }

         break;
      }

      case zTYPE_INTEGER:
      {
         zPLONG   lplLong;
         zPLONG   lplLongInput;

         lplLongInput = (zPLONG) lpValue;
         lplLong = (zPLONG) (pchRecord + lpViewAttribute->ulRecordOffset);
         if ( *lplLongInput == lNullInteger )
         {
            if ( *lplLong != lNullInteger )
            {
               *lplLong = lNullInteger;
               bUpdated = TRUE;
            }
         }
         else
         if ( *lplLong != *lplLongInput )
         {
            *lplLong = *lplLongInput;
            bUpdated = TRUE;
         }

         break;
      }

      case zTYPE_DECIMAL:
      {
         zPDECIMAL lpdDecimal;
         zPDECIMAL lpdDecimalInput;

         lpdDecimalInput = (zPDECIMAL) lpValue;
         lpdDecimal = (zPDECIMAL) (pchRecord + lpViewAttribute->ulRecordOffset);
         if ( SysCompareDecimalToNull( lpdDecimalInput ) == 0 )
         {
            if ( SysCompareDecimalToNull( lpdDecimal ) != 0 )
            {
               SysAssignDecimalFromNull( lpdDecimal );
               bUpdated = TRUE;
            }
         }
         else
         if ( SysCompareDecimalToDecimal( lpdDecimal, lpdDecimalInput ) != 0 )
         {
            SysAssignDecimalFromDecimal( lpdDecimal, lpdDecimalInput );
            bUpdated = TRUE;
         }

         break;
      }

      case zTYPE_BLOB:
      {
         zSHORT k;

         k = fnStoreBlobInRecord( lpAllocTask, // so allocation can be done
                                  pchRecord,       // Attribute record
                                  lpViewAttribute->ulRecordOffset, // Offset
                                  lpValue,        // Blob value to store
                                  ulLength );     // Length to store
         // Check return code
         if ( k )
         {
            if ( k == 1 )
               bUpdated = TRUE;
            else
               nRC = k;
         }

         break;
      }

      case zTYPE_DATETIME:
      {
         LPDTINTERNAL  lpDateTime;
         LPDTINTERNAL  lpDateTimeInput;

         lpDateTimeInput = (LPDTINTERNAL) lpValue;
         lpDateTime = (LPDTINTERNAL)
                              (pchRecord + lpViewAttribute->ulRecordOffset);
         if ( lpDateTimeInput->ulDateMinutes == (zULONG) lNullInteger )
         {
            if ( lpDateTime->ulDateMinutes != (zULONG) lNullInteger )
            {
               lpDateTime->ulDateMinutes = (zULONG) lNullInteger;
               lpDateTime->usTSeconds = 0;
               bUpdated = TRUE;
            }
         }
         else
         if ( zmemcmp( lpDateTime, lpDateTimeInput,
                       sizeof( DateTimeRecord ) ) != 0 )
         {
            zmemcpy( lpDateTime, lpDateTimeInput, sizeof( DateTimeRecord ) );
            bUpdated = TRUE;
         }

         break;
      }

      default:
         nRC = zCALL_ERROR;
   }

   // Set update indicator.
   fnSetUpdateIndicator( lpViewOI, lpEntityInstance, lpViewAttribute,
                         bUpdated, TRUE, TRUE );

   return( nRC );
} // fnStoreValueInEntityInstance

//./ ADD NAME=GetStringFromAttributeByContext
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetStringFromAttributeByContext
//
//  PURPOSE:    To retrieve a string formatted through a Context
//
//  PARAMETERS: pchReturnString  - String value returned
//              lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to retrieve
//              cpcContextName   - Context through which data is processed
//              ulLth            - Max length of the return string
//
//  RETURNS:     0          - Attribute successfully retrieved
//              -1          - Returned null string in pchReturnString,
//                            the attribute value was null.
//              -2          - Returned null string in pchReturnString,
//                            the entity was null.
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
GetStringFromAttributeByContext( zPCHAR  pchReturnString,
                                 zVIEW   lpView,
                                 zCPCHAR cpcEntityName,
                                 zCPCHAR cpcAttributeName,
                                 zCPCHAR cpcContextName,
                                 zULONG  ulLth )
{
   zULONG            ulTossOut;
   zSHORT            nRC;

   // If a Context is passed to this routine, use it.
   // If the Context is null or blank, issue the call with default context.
   if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
   {
      nRC = GetVariableFromAttribute( pchReturnString,
                                      &ulTossOut,
                                      zTYPE_STRING,
                                      ulLth,
                                      lpView,
                                      cpcEntityName,
                                      cpcAttributeName,
                                      cpcContextName,
                                      zUSE_DEFAULT_CONTEXT );
   }
   else
   {
      nRC = GetVariableFromAttribute( pchReturnString,
                                      &ulTossOut,
                                      zTYPE_STRING,
                                      ulLth,
                                      lpView,
                                      cpcEntityName,
                                      cpcAttributeName,
                                      cpcContextName,
                                      0 );
   }

   return( nRC );
}

//./ ADD NAME=GetStringFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetStringFromAttribute
//
//  PURPOSE:    To retrieve a string representing an attribute value
//
//  PARAMETERS: pchReturnString  - String value returned
//              lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to retrieve
//
//  RETURNS:    0           - Attribute successfully retrieved
//              -1          - Returned null string in pchReturnString,
//                            the attribute value was null.
//              -2          - Returned null string in pchReturnString,
//                            the entity was null.
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
GetStringFromAttribute( zPCHAR  pchReturnString,
                        zLONG   lMaxLth,
                        zVIEW   lpView,
                        zCPCHAR cpcEntityName,
                        zCPCHAR cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zULONG            ulStringLth;
   zULONG            ulTossOut;
   zSHORT            nRC;

   *pchReturnString = 0;  // initialize return string to empty

   // Validate parameters.
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetStringFromAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );

   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC == -1 )
      nRC = zCALL_ERROR;
   else
   if ( nRC != -2 )   // not a NULL entity.
   {
      fnGetAttributeLength( &ulStringLth, lpView,
                            zGETPTR( lpViewEntityCsr->hEntityInstance ),
                            lpViewAttrib );
      if ( ++ulStringLth > (zULONG) lMaxLth )
      {
         zCHAR szMsg[ 200 ];

         sprintf_s( szMsg, sizeof( szMsg ), "GetStringFromAttribute for Entity: %s  Attribute: %s has length mismatch: %d specified, %d actual",
                    cpcEntityName, cpcAttributeName, ulStringLth, lMaxLth );
         ulStringLth = lMaxLth;
      }

      nRC = fnGetVariableFromAttribute( (zPVOID) pchReturnString, &ulTossOut,
                                        zTYPE_STRING,
                                        ulStringLth, lpView,
                                        lpViewEntityCsr,
                                        lpViewAttrib, 0, lpCurrentTask, 0 );
   }

   fnOperationReturn( iGetStringFromAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetStringFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnGetStringFromAttribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnGetStringFromAttribute( zPCHAR          pchReturnString,
                          zULONG          ulMaxLth,
                          zVIEW           lpView,
                          LPVIEWENTITYCSR lpViewEntityCsr,
                          LPVIEWATTRIB    lpViewAttrib,
                          zCPCHAR         cpcContextName )
{
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchPtr;
   zULONG            ulLth;
   zSHORT            nRC;

   pchReturnString[ 0 ] = 0; // initialize return value
   nRC = 0;

   // Get Entity Instance pointer.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   nRC = fnGetAttrAddrFromEntityInstance( &pchPtr, &ulLth, lpEntityInstance, lpViewAttrib );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   // If a value exists, copy it to the return variable.
   if ( nRC == 0 )
   {
      zCHAR szTemp[ 400 ];

      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         {
            if ( ulMaxLth == 0 )
            {
               SysMessageBox( 0, szlOE_SystemError, "Maximum Length is zero for GetStringFromAttribute", -1 );
               strcpy_s( pchReturnString, ulMaxLth, pchPtr );
            }
            else
            {
               pchReturnString[ 0 ] = 0;

               // DKS 1997.10.14: get string of correct length
               strncat_s( pchReturnString, ulMaxLth, pchPtr, (unsigned int) ulMaxLth - 1 );
//             pchReturnString[ ulMaxLth - 1 ] = 0;
            }

            break;
         }

         case zTYPE_INTEGER:
            if ( ulMaxLth == 0 )
               zltoa( *((zPLONG) pchPtr), pchReturnString, ulMaxLth );
            else
            {
               pchReturnString[ 0 ] = 0;
               zltoa( *((zPLONG) pchPtr), szTemp, sizeof( szTemp ) );
               strncat_s( pchReturnString, ulMaxLth, szTemp, (unsigned int) ulMaxLth - 1 );
            }

            break;

         case zTYPE_DECIMAL:
            if ( ulMaxLth == 0 )
            {
               ConvertDecimalToString( pchReturnString, lpViewAttrib, *((zPDECIMAL) pchPtr), 0 );
            }
            else
            {
               pchReturnString[ 0 ] = 0;
               ConvertDecimalToString( szTemp, lpViewAttrib,
                                       *((zPDECIMAL) pchPtr), cpcContextName );
               strncat_s( pchReturnString, ulMaxLth, szTemp, (unsigned int) ulMaxLth - 1 );
            }

            break;

         case zTYPE_DATETIME:
         {
            zSHORT nTempLth;

            if ( ulMaxLth > 100 )
               nTempLth = 100;
            else
               nTempLth = (zSHORT) ulMaxLth;

            nRC = UfDateTimeToString( (LPDATETIME) pchPtr, pchReturnString, nTempLth );
            break;
         }

         case zTYPE_BLOB:
            // Copy memory from blob to string buffer.
            zmemcpy( pchReturnString, pchPtr, (unsigned int) ulLth );

            // Make sure buffer has null-terminator.
            pchReturnString[ ulLth ] = 0;
            break;

         default:
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }
   else
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_INTEGER:
         case zTYPE_DECIMAL:
         case zTYPE_DATETIME:
         case zTYPE_BLOB:
            nRC = -1;   // indicate null attribute
            break;

         default:
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   return( nRC );

} // fnGetStringFromAttribute

zSHORT OPERATION
GetViewFromAttribute( zPVIEW  pvReturnView,
                      zVIEW   lpView,
                      zCPCHAR cpcEntityName,
                      zCPCHAR cpcAttributeName )
{
   return( GetIntegerFromAttribute( (zPLONG) pvReturnView, lpView,
                                    cpcEntityName, cpcAttributeName ) );
}

//./ ADD NAME=GetIntegerFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetIntegerFromAttribute
//
//  PURPOSE:    To retrieve an integer (zLONG) representing an
//              attribute value
//
//  PARAMETERS: lplReturnInteger - Addr of Integer value to return
//              lpView           - View to Object instance
//              cpcEntityName     - Entity containing attribute to retrieve
//              cpcAttributeName  - Attribute to retrieve
//
//  RETURNS:    0           - Attribute successfully retrieved
//              -1          - Returned 0 in lplReturnInteger,
//                            the attribute value was null.
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetIntegerFromAttribute( zPLONG  lplReturnInteger,
                         zVIEW   lpView,
                         zCPCHAR cpcEntityName,
                         zCPCHAR cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zULONG            ulTossOut;
   zSHORT            nRC;

   *lplReturnInteger = 0;   // initialize return value to zero

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetIntegerFromAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName,
                                        zACCEPT_NULL_ENTITY );

   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC == -1 )
      nRC = zCALL_ERROR;
   else
   if ( nRC != -2 )   // not a NULL entity.
   {
      nRC = fnGetVariableFromAttribute( (zPVOID) lplReturnInteger,
                                        &ulTossOut,
                                        zTYPE_INTEGER,
                                        0, lpView, lpViewEntityCsr,
                                        lpViewAttrib, 0, lpCurrentTask, 0 );
   }

   fnOperationReturn( iGetIntegerFromAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetIntegerFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetIntegerFromAttribute
//
//  RETURNS:   -1           - Attrib addr retrieved, but attrib is NULL
//              0           - Attribute address successfully retrieved
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnGetIntegerFromAttribute( zPLONG          lplReturnInteger,
                           zVIEW           lpView,
                           LPVIEWENTITYCSR lpViewEntityCsr,
                           LPVIEWATTRIB    lpViewAttrib )
{
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchPtr;
   LPVIEWOI          lpViewOI;
   zULONG            ulLth;
   zSHORT            nRC;

   *lplReturnInteger = 0;  // initialize return value to 0

   // Get Entity Instance pointer.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   lpViewOI = zGETPTR( lpEntityInstance->hViewOI );

   nRC = fnGetAttrAddrFromEntityInstance( &pchPtr, &ulLth, lpEntityInstance,
                                          lpViewAttrib );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   // If a value exists, copy it to the return variable.
   if ( nRC == 0 )
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
            *lplReturnInteger = zatol( pchPtr );
            break;

         case zTYPE_INTEGER:
            *lplReturnInteger = *((zPLONG) pchPtr);
            break;

         case zTYPE_DECIMAL:
         {
            zPDECIMAL lpdDecimal;
            zDECIMAL  dLong;

            lpdDecimal = (zPDECIMAL) pchPtr;

            SysConvertLongToDecimal( LONG_MAX, &dLong );
            if ( SysCompareDecimalToDecimal( lpdDecimal, &dLong ) == 1 )
            {
               LPTASK lpTask = zGETPTR( lpView->hTask );
               //  "KZOEE235 - Integer overflow"
               fnIssueCoreError( lpTask, lpView, 16, 235, 0,
                                 fnEntityNameFromCsr( lpViewEntityCsr ),
                                 lpViewAttrib->szName );
               nRC = zCALL_ERROR;
            }
            else
            {
               SysConvertLongToDecimal( LONG_MIN, &dLong );
               if ( SysCompareDecimalToDecimal( lpdDecimal, &dLong ) == -1 )
               {
                  LPTASK lpTask = zGETPTR( lpView->hTask );
                  //  "KZOEE236 - Integer underflow"
                  fnIssueCoreError( lpTask, lpView, 16, 236, 0,
                                    fnEntityNameFromCsr( lpViewEntityCsr ),
                                    lpViewAttrib->szName );
                  nRC = zCALL_ERROR;
               }
               else
               {
                  SysConvertDecimalToLong( lpdDecimal, lplReturnInteger );
               }
            }

            break;
         }

         default:
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   } // if ( nRC == 0 )...
   else
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_INTEGER:
         case zTYPE_DECIMAL:
            nRC = -1;         // indicate null attribute
            break;

         default:
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=GetDecimalFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetDecimalFromAttribute
//
//  PURPOSE:    To retrieve a decimal (double) representing an
//              attribute value
//
//  PARAMETERS: lpdReturnDecimal - Addr of Decimal value to return
//              lpView           - View to Object instance
//              cpcEntityName     - Entity containing attribute to retrieve
//              cpcAttributeName  - Attribute to retrieve
//
//  RETURNS:    0           - Attribute successfully retrieved
//              -1          - Returned 0 in lpdReturnDecimal,
//                            the attribute value was null.
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetDecimalFromAttribute( zPDECIMAL   lpdReturnDecimal,
                         zVIEW       lpView,
                         zCPCHAR     cpcEntityName,
                         zCPCHAR     cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zULONG            ulTossOut;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetDecimalFromAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
   {
      nRC = fnGetVariableFromAttribute( (zPVOID) lpdReturnDecimal,
                                        &ulTossOut,
                                        zTYPE_DECIMAL,
                                        0, lpView, lpViewEntityCsr,
                                        lpViewAttrib, 0, lpCurrentTask, 0 );
   }

   fnOperationReturn( iGetDecimalFromAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetDecimalFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetDecimalFromAttribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnGetDecimalFromAttribute( zPDECIMAL       lpdReturnDecimal,
                           zVIEW           lpView,
                           LPVIEWENTITYCSR lpViewEntityCsr,
                           LPVIEWATTRIB    lpViewAttrib )
{
   LPENTITYINSTANCE  lpEntityInstance;
   zULONG            ulLth;
   zPCHAR            pchPtr;
   LPVIEWOI          lpViewOI;
   zSHORT            nRC;

   // Get Entity Instance pointer.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   lpViewOI = zGETPTR( lpEntityInstance->hViewOI );

   // Initialize return value to 0.
   SysConvertLongToDecimal( 0, lpdReturnDecimal );

   nRC = fnGetAttrAddrFromEntityInstance( &pchPtr, &ulLth, lpEntityInstance,
                                          lpViewAttrib );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   // If a value exists, copy it to the return variable.
   if ( nRC == 0 )
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
            if ( pchPtr[ 0 ] )
               SysConvertStringToDecimal( pchPtr, lpdReturnDecimal );
            else
               nRC = -1;   // indicate attribute null

            break;

         case zTYPE_INTEGER:
            SysConvertLongToDecimal( *((zPLONG) pchPtr), lpdReturnDecimal );
            break;

         case zTYPE_DECIMAL:
            SysAssignDecimalFromDecimal( lpdReturnDecimal, (zPDECIMAL) pchPtr );
            break;

         default:
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }
   else
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_INTEGER:
         case zTYPE_DECIMAL:
            nRC = -1;
            break;

         default:
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=GetBlobFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetBlobFromAttribute
//
//  PURPOSE:    To retrieve a Blob from an attribute value
//
//  PARAMETERS: pReturnBlob      - Blob value returned
//              pulMaxLth        - return length of the blob
//              lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to retrieve
//
//  NOTE:       The Attribute internal type MUST be a Blob attribute.
//              Retrieval of any other attribute type with this operation
//              is an ERROR.
//
//  RETURNS:    0           - Attribute successfully retrieved
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
GetBlobFromAttribute( zPVOID   pReturnBlob,
                      zPULONG  pulMaxLth,
                      zVIEW    lpView,
                      zCPCHAR  cpcEntityName,
                      zCPCHAR  cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zULONG            ulLth;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetBlobFromAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
   {
      if ( pulMaxLth == 0 )
      {
         fnGetAttributeLength( &ulLth, lpView,
                               zGETPTR( lpViewEntityCsr->hEntityInstance ),
                               lpViewAttrib );
         pulMaxLth = &ulLth;
      }

      nRC = fnGetVariableFromAttribute( (zPVOID) pReturnBlob, pulMaxLth,
                                        zTYPE_BLOB,
                                        *pulMaxLth, lpView, lpViewEntityCsr,
                                        lpViewAttrib, 0, lpCurrentTask, 0 );
   }

   fnOperationReturn( iGetBlobFromAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetBlobFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     fnGetBlobFromAttribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnGetBlobFromAttribute( zPVOID          pReturnBlob,
                        zPULONG         pulMaxLth,
                        zVIEW           lpView,
                        LPVIEWENTITYCSR lpViewEntityCsr,
                        LPVIEWATTRIB    lpViewAttrib )
{
   zSHORT            nRC;
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchPtr;
   zULONG            ulLth;

   // Get Entity Instance pointer
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   if ( lpViewAttrib->cType != zTYPE_BLOB )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      // "KZOEE239 - Invalid Operation for attribute type"
      fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                        fnEntityNameFromCsr( lpViewEntityCsr ),
                        lpViewAttrib->szName );
      return( zCALL_ERROR );
   }

   nRC = fnGetAttrAddrFromEntityInstance( &pchPtr, &ulLth, lpEntityInstance,
                                          lpViewAttrib );

   // If a record exists, copy it to the return variable.
   if ( nRC == 0 )
   {
      if ( ulLth > *pulMaxLth )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );

         TraceLine( "Binary large object length: %d   exceeds passed length: %d",
                    ulLth, *pulMaxLth );
         // "KZOEE240 - Binary large object (Blob) exceeds passed "
         // "           length"
         fnIssueCoreError( lpTask, lpView, 8, 240, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ),
                           lpViewAttrib->szName );
         return( zCALL_ERROR );
      }
      else
      {
         *pulMaxLth = ulLth;
         zmemcpy( pReturnBlob, pchPtr, (unsigned int) ulLth );
      }
   }
   else
   {
      *((zPVOID *) pReturnBlob) = 0;
      *pulMaxLth = 0;
   }

   return( nRC );
}

// slows things way down, so don't define unless specifically debugging GetStructFromEntityAttrs
//#define TRACE_STRUCT 1

//./ ADD NAME=GetStructFromEntityAttrs
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetStructFromEntityAttrs
//
//  PURPOSE:    To retrieve a structure representing the persistent attribute list
//
//  PARAMETERS: pchReturnStruct  - Address of structure to return
//              lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute
//                                 structure to retrieve
//
//  RETURNS:    0           - Structure successfully retrieved
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
GetStructFromEntityAttrs( zPCHAR  pchReturnStruct,
                          zVIEW   lpView,
                          zCPCHAR cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchRecord;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetStructFromEntityAttrs,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        0, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
   {
      zBOOL bBlobType;

      // Get Entity Instance pointer.
      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

      zLONG lStructLth = *((zPLONG) pchReturnStruct);
      zPCHAR pchEndStruct = pchReturnStruct + lStructLth;
      pchReturnStruct += sizeof( zLONG );
#ifdef TRACE_STRUCT
      {
         LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );
         TraceLine( "GetStructFromEntityAttrs Def: %s   Entity: %s", cpcEntityName, lpViewOD->szName );
      }
#endif

      // If a record exists, copy it to the return variable.
      for ( nRC = 0;
            lpViewAttrib;
            lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         bBlobType = FALSE;

         // Get record to retrieve from.
         if ( lpViewAttrib->bPersist )
            pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
         else
            pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord );

         // If pchRecord is null, then there are no attribute values.
         if ( pchRecord == 0 )
         {
#ifdef TRACE_STRUCT
            TraceLine( "   Attrib: %s   NULL", lpViewAttrib->szName );
#endif
            if ( lpViewAttrib->cType == zTYPE_STRING )
               pchReturnStruct[ 0 ] = 0;
            else
            {
               zUSHORT nIndex;
               for ( nIndex = 0; nIndex < lpViewAttrib->ulLth; nIndex++ )
                  pchReturnStruct[ nIndex ] = 0;
            }
         }
         else
         {
            switch ( lpViewAttrib->cType )
            {
               case zTYPE_STRING:
               {
                  zPCHAR    pchRecordString;
                  zCOREMEM  lpExternalP;

                  pchRecordString = pchRecord + lpViewAttrib->ulRecordOffset;
                  if ( pchRecordString[ 0 ] == (zCHAR) '\xff' )
                  {
                     lpExternalP = (zCOREMEM) (pchRecordString + 1);
                     pchRecordString = zGETPTR( *lpExternalP );
                  }

                  strcpy_s( pchReturnStruct, lpViewAttrib->ulLth, pchRecordString );
#ifdef TRACE_STRUCT
                  TraceLine( "   Attrib: %s   %s", lpViewAttrib->szName, pchRecordString );
#endif
               }

               break;

               case zTYPE_INTEGER:
               {
                  zPLONG   lplLong;
                  zPLONG   lplLong2;

                  lplLong = (zPLONG) (pchRecord + lpViewAttrib->ulRecordOffset);
                  lplLong2 = (zPLONG) pchReturnStruct;
                  if ( *lplLong != lNullInteger )
                     *lplLong2 = *lplLong;
                  else
                     *lplLong2 = 0;

#ifdef TRACE_STRUCT
                  TraceLine( "   Attrib: %s   %d", lpViewAttrib->szName, *lplLong2 );
#endif
               }

               break;

               case zTYPE_DECIMAL:
               {
                  zPDECIMAL lpdDecimal;
                  zPDECIMAL lpdDecimal2;

                  lpdDecimal = (zPDECIMAL) (pchRecord + lpViewAttrib->ulRecordOffset);
                  lpdDecimal2 = (zPDECIMAL) pchReturnStruct;
                  if ( SysCompareDecimalToNull( lpdDecimal ) == 0 )
                  {
                     SysAssignDecimalFromDecimal( lpdDecimal2, lpdDecimal );
                  }
                  else
                  {
                     SysConvertLongToDecimal( 0, lpdDecimal );
                  }

#ifdef TRACE_STRUCT
                  TraceLine( "   Attrib: %s   DECIMAL", lpViewAttrib->szName );
#endif
               }

               break;

               case zTYPE_BLOB:
               {
                  zPCHAR   pchChar;
                  zCOREMEM lpBlob;
                  zCOREMEM lpReturnBlob;

                  pchChar = pchRecord + lpViewAttrib->ulRecordOffset;
                  lpBlob = (zCOREMEM) (pchRecord + lpViewAttrib->ulRecordOffset + 1);
                  lpReturnBlob = (zCOREMEM) pchReturnStruct;

                  if ( *pchChar == '\xff' )
                     *lpReturnBlob = zGETPTR( *lpBlob );
                  else
                     *lpReturnBlob = 0;

                  bBlobType = TRUE;

#ifdef TRACE_STRUCT
                  TraceLine( "   Attrib: %s   BLOB", lpViewAttrib->szName );
#endif
               }

               break;

               case zTYPE_DATETIME:
                  // a work in progress...
#ifdef TRACE_STRUCT
                  TraceLine( "   Attrib: %s   DATETIME", lpViewAttrib->szName );
#endif
                  break;

            } // switch (...)...

         } // if ( pchRecord == 0 )...

         if ( bBlobType )
            pchReturnStruct += sizeof( zPVOID );
         else
            pchReturnStruct += lpViewAttrib->ulLth;

      } // for ( lpViewAttrib...)...
      if ( pchReturnStruct != pchEndStruct - 1 )  // minus one for the check-byte
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, sizeof( szMsg ), "Error in structure length for entity: %s", cpcEntityName );
         SysMessageBox( lpView, "Struct Size Error", szMsg, 1 );
         nRC = zCALL_ERROR;
      }
   }

   fnOperationReturn( iGetStructFromEntityAttrs, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetAddrForAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetAddrForAttribute
//
//  PURPOSE:    To retrieve the memory address of an attribute
//
//  PARAMETERS: lplReturnAddr    - Address of Address variable to return
//              lpView           - View to Object instance
//              cpcEntityName     - Entity containing attribute to retrieve
//              cpcAttributeName  - Attribute to retrieve
//
//  RETURNS:    0           - Address successfully retrieved
//              zCALL_ERROR - Error retrieving attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetAddrForAttribute( zCOREMEM lpReturnAddr,
                     zVIEW    lpView,
                     zCPCHAR  cpcEntityName,
                     zCPCHAR  cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // initialize returned address to null
   *lpReturnAddr = szGAFANullS;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetAddrForAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
   {
      // If there is an object operation for the sourced attribute,
      // invoke it now...
      if ( lpViewAttrib->szDerivedOper[ 0 ] )
      {
         nRC = fnInvokeDerivedOperation( lpView, lpViewEntity, lpViewAttrib,
                                         lpCurrentTask, zDERIVED_GET );
         if ( nRC == zCALL_ERROR )
         {
            fnOperationReturn( iGetAddrForAttribute, lpCurrentTask );
            return( nRC );
         }
      }

      nRC = fnGetAddrForAttribute( lpReturnAddr, lpView,
                                   lpViewEntityCsr, 0, lpViewAttrib );
   }

   fnOperationReturn( iGetAddrForAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetAddrForAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetAddrForAttribute
//
//  Note:  lpViewEntityCsr and lpEntityInstance are redundant.  Only 1
//         needs to be set.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnGetAddrForAttribute( zCOREMEM         lpReturnAddr,
                       zVIEW            lpView,
                       LPVIEWENTITYCSR  lpViewEntityCsr,
                       LPENTITYINSTANCE lpEntityInstance,
                       LPVIEWATTRIB     lpViewAttrib )
{
   zPCHAR          pchRecord;
   static zDECIMAL dNullDecimal;

   // Get Entity Instance pointer.
   if ( lpEntityInstance == 0 )
      lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Get record to retrieve from.
   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   // If a record exists, copy it to the return variable.
   if ( pchRecord )
   {
      pchRecord += lpViewAttrib->ulRecordOffset;
      if ( lpViewAttrib->cType == zTYPE_BLOB )
      {
         if ( pchRecord[ 0 ] != (zCHAR) '\xff' )
            *lpReturnAddr = 0;
         else
         {
            zCOREMEM  lpExternalP;

            lpExternalP = (zCOREMEM) (pchRecord + 1);
            pchRecord = zGETPTR( *lpExternalP );
            *lpReturnAddr = pchRecord;
         }
      }
      else
      {
         if ( lpViewAttrib->cType == zTYPE_STRING &&
              pchRecord[ 0 ] == (zCHAR) '\xff' )
         {
            zCOREMEM  lpExternalP;

            lpExternalP = (zCOREMEM) (pchRecord + 1);
            pchRecord = zGETPTR( *lpExternalP );
            *lpReturnAddr = pchRecord;
         }
         else
            *lpReturnAddr = pchRecord;
      }
   }
   else
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
            *lpReturnAddr = szNullS;
            break;

         case zTYPE_INTEGER:
            *lpReturnAddr = (zPCHAR) &lNullInteger;
            break;

         case zTYPE_DECIMAL:
            SysAssignDecimalFromNull( &dNullDecimal );
            *lpReturnAddr = (zPCHAR) &dNullDecimal;
            break;

         case zTYPE_DATETIME:
            *lpReturnAddr = (zPCHAR) &stNullDateTime;
            break;

         case zTYPE_BLOB:
            *lpReturnAddr = 0;
            break;

         default:
         {
            LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
            LPTASK lpTask = zGETPTR( lpView->hTask );

            // This error should never happen.
            // "KZOEE246 - Invalid Attribute type "
            fnIssueCoreError( lpTask, lpView, 8, 246, 0,
                              lpViewEntity->szName, lpViewAttrib->szName );
            return( zCALL_ERROR );
         }
      }
   }

   return( 0 );
}

//./ ADD NAME=GetAttributeDisplayLength
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetAttributeDisplayLength
//
//  PURPOSE:    To retrieve the defined length or current length of
//              an attribute based on the attribute type expressed as
//              a string.  Use the specified Context Name in conjunction
//              with them domain of the attribute.  If no Context name is
//              specified then, the default context is used.  If no domain
//              for this attribute, then acts just like the
//              GetAttributeLength operation.
//
//  PARAMETERS: pulReturnLth     - Length value returned
//              lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to retrieve
//              cpcContextName   - Attribute to retrieve
//
//  RETURNS:    0           - Length successfully retrieved
//              zCALL_ERROR - Error retrieving length
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
GetAttributeDisplayLength( zPULONG  pulReturnLth,
                           zVIEW    lpView,
                           zCPCHAR  cpcEntityName,
                           zCPCHAR  cpcAttributeName,
                           zCPCHAR  cpcContextName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   *pulReturnLth = 0; // initialize to 0.

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetAttributeDisplayLength,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName,
                                        zACCEPT_NULL_ENTITY );

   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   // We need to know the length even if the entity instance is null.
   // Therefore when domains are performing GetVariable processing for
   // a chType of zTYPE_PIC, they should not try to retrieve any data
   // from the entity instance.

   if ( nRC && nRC != -2 )
   {
      fnOperationReturn( iGetAttributeDisplayLength, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpViewAttrib->hDomain )
   {
      zLPCONTEXT   lpContext;

      if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
      {
         nRC = GetDefaultContext( &lpContext,
                                  zGETPTR( lpViewAttrib->hDomain ) );
      }
      else
      {
         nRC = GetContext( &lpContext, zGETPTR( lpViewAttrib->hDomain ),
                           cpcContextName );
      }

      if ( nRC )
      {
         nRC = fnGetAttributeDisplayLength( pulReturnLth, lpView,
                                            lpViewEntityCsr, lpViewAttrib,
                                            lpContext, lpCurrentTask );
      }
      else
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         zCHAR  sz[ 102 ];
         LPDOMAIN lpDomain;

         strcpy_s( sz, sizeof( sz ), cpcEntityName );
         strcat_s( sz, sizeof( sz ), ", " );
         strcat_s( sz, sizeof( sz ), cpcAttributeName );
         strcat_s( sz, sizeof( sz ), ", " );
         lpDomain = zGETPTR( lpViewAttrib->hDomain );
         strcat_s( sz, sizeof( sz ), lpDomain->szName );

         // "KZOEE350 - Context Not Valid for Domain"
         fnIssueCoreError( lpTask, lpView, 8, 350, 0, sz, cpcContextName );
         nRC = zCALL_ERROR;
      }
   }
   else
   {
      nRC = fnGetAttributeLength( pulReturnLth, lpView,
                                  zGETPTR( lpViewEntityCsr->hEntityInstance ),
                                  lpViewAttrib );
   }

   fnOperationReturn( iGetAttributeDisplayLength, lpCurrentTask );
   return( nRC );
}

// Returns a pointer to the string representing the value of the table
// entry given the current Language code.
zPCHAR
fnGetTextForTableEntry( LPTABLEENTRY lpTableEntry )
{
   zSHORT          nLanguageCode;
   LPNLSTABLEENTRY lpNLS_TableEntry;

   // Look for an NLS_TableEntry that matches the current country code.
   SysGetLanguageCode( &nLanguageCode );
   for ( lpNLS_TableEntry = zGETPTR( lpTableEntry->hFirstNLS_TableEntry );
         lpNLS_TableEntry && lpNLS_TableEntry->nLanguageCode != nLanguageCode;
         lpNLS_TableEntry = zGETPTR( lpNLS_TableEntry->hNextNLS_TableEntry ) )
   {
      // nothing needs to be done here.
   }

   // If lpNLS_TableEntry is non-null, then a match was found.
   if ( lpNLS_TableEntry )
      return( zGETPTR( lpNLS_TableEntry->pszNLS_Text ) );

   // No match found for the current language code, so return the "normal"
   // external value.
   return( zGETPTR( lpTableEntry->hExternalValue ) );

} // fnGetTextForTableEntry

zULONG
fnGetLongExternalLthForContext( zLPCONTEXT lpContext )
{
   zPCHAR         pch;
   zULONG         n, nMax = 0;
   LPTABLEENTRY   lpTableEntry;

   for ( lpTableEntry = zGETPTR( lpContext->hFirstTableEntry );
         lpTableEntry;
         lpTableEntry = zGETPTR( lpTableEntry->hNextTableEntry ) )
   {
      pch = fnGetTextForTableEntry( lpTableEntry );

      if ( pch && (n = zstrlen( pch )) > nMax )
         nMax = n;
   }

   return( nMax );
}

//./ ADD NAME=fnGetAttributeDisplayLength
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetAttributeDisplayLength
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT
fnGetAttributeDisplayLength( zPULONG         pulReturnLth,
                             zVIEW           lpView,
                             LPVIEWENTITYCSR lpViewEntityCsr,
                             LPVIEWATTRIB    lpViewAttrib,
                             zLPCONTEXT      lpContext,
                             LPTASK          lpCurrentTask )
{
   LPDOMAIN          lpDomain;
   zULONG            ulLth = 0;
   zPCHAR            pchEditString;
   zCHAR             cDomainType;
   LPTASKDOMAIN      lpTaskDomain;
   zSHORT            nRC = 0; // default to okay

   lpDomain = zGETPTR( lpContext->hDomain );
   cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                             lpCurrentTask, lpDomain );

   if ( lpDomain->szDomainOper[ 0 ] )
   {
      if ( lpTaskDomain == 0 )
         return( zCALL_ERROR );

      nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                              zDME_GET_ATTR_DISPLAY_LENGTH,
                              0, (zPCHAR) pulReturnLth, lpContext->szName,
                              lpView, zGETPTR( lpViewAttrib->hViewEntity ),
                              lpViewAttrib, 0 );
      if ( nRC != zDME_NOT_HANDLED )
         return( 0 );
   }

   if ( lpDomain->cDomainType == zDM_TYPE_FORMAT )
   {
      pchEditString = zGETPTR( lpContext->hEditString );
      ulLth = zstrlen( pchEditString );
   }
   else
   if ( lpDomain->cDomainType == zDM_TYPE_TABLE )
   {
      if ( lpContext->nDisplayLength == -1 )
      {
         ulLth = fnGetLongExternalLthForContext( lpContext );
         lpContext->nDisplayLength = (zUSHORT) ulLth;
      }
      else
         ulLth = lpContext->nDisplayLength;
   }

   // If the length is still 0 try another way.
   if ( ulLth == 0 )
   {
      switch ( lpDomain->cType )
      {
         case zTYPE_STRING:
            ulLth = lpViewAttrib->ulLth - 1;
            break;

         case zTYPE_INTEGER:
            ulLth = 16;
            break;

         case zTYPE_DECIMAL:
            ulLth = 24;
            break;

         case zTYPE_DATETIME:
            ulLth = 25;
            break;

         case zTYPE_BLOB:
            ulLth = -1;
            break;

         default:
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   *pulReturnLth = ulLth;

   return( nRC );
}

//./ ADD NAME=GetAttributeLength
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetAttributeLength
//
//  PURPOSE:    To retrieve the defined length or current length of
//              an attribute based on the attribute type expressed as
//              a string.
//
//  PARAMETERS: pulReturnLth     - Length value returned
//              lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to retrieve
//
//  RETURNS:    0           - Length successfully retrieved
//              zCALL_ERROR - Error retrieving length
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetAttributeLength( zPULONG pulReturnLth,
                    zVIEW   lpView,
                    zCPCHAR cpcEntityName,
                    zCPCHAR cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetAttributeLength,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC && nRC != -2 )
      nRC = zCALL_ERROR;
   else
   {
      nRC = fnGetAttributeLength( pulReturnLth,
                                  lpView,
                                  zGETPTR( lpViewEntityCsr->hEntityInstance ),
                                  lpViewAttrib );
   }

   fnOperationReturn( iGetAttributeLength, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetAttributeLength
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetAttributeLength
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnGetAttributeLength( zPULONG          pulReturnLth,
                      zVIEW            lpView,
                      LPENTITYINSTANCE lpEntityInstance,
                      LPVIEWATTRIB     lpViewAttrib )
{
   zPCHAR  pchRecord;
   zSHORT  nRC;

   nRC = 0;
   switch ( lpViewAttrib->cType )
   {
      case zTYPE_STRING:
         *pulReturnLth = lpViewAttrib->ulLth - 1;
         break;

      case zTYPE_INTEGER:
         *pulReturnLth = 16;
         break;

      case zTYPE_DECIMAL:
         *pulReturnLth = 24;
         break;

      case zTYPE_DATETIME:
         *pulReturnLth = 17;
         break;

      case zTYPE_BLOB:
         *pulReturnLth = 0;    // Default return length to 0

         if ( lpEntityInstance == 0 )
            break;

         // Get record to retrieve from.
         pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

         // If a record exists, copy it to the return variable.
         if ( pchRecord )
         {
            pchRecord += lpViewAttrib->ulRecordOffset;
            if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zPULONG   pulLong;
               zCOREMEM  lpExternalP;

               lpExternalP = (zCOREMEM) (pchRecord + 1);
               pulLong = (zPULONG) (lpExternalP + 1);
               *pulReturnLth = *pulLong;
            }
         }

         break;

      default:
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

         // "KZOEE239 - Invalid Operation for attribute type"
         fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                           lpViewEntity->szName, lpViewAttrib->szName );
         nRC = zCALL_ERROR;
      }
   }

   return( nRC );
}

//./ ADD NAME=GetActualAttributeLength
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetActualAttributeLength
//
//  PURPOSE:    To retrieve the current length of an attribute.
//
//  PARAMETERS: pulReturnLth     - Length value returned
//              lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to retrieve
//
//  RETURNS:    0           - Length successfully retrieved
//              zCALL_ERROR - Error retrieving length
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetActualAttributeLength( zPULONG  pulReturnLth,
                          zVIEW    lpView,
                          zCPCHAR  cpcEntityName,
                          zCPCHAR  cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetActualAttributeLength,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
      nRC = fnGetActualAttributeLength( pulReturnLth, lpView,
                                        lpViewEntityCsr, lpViewAttrib );

   fnOperationReturn( iGetActualAttributeLength, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetActualAttributeLength
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetActualAttributeLength
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnGetActualAttributeLength( zPULONG         pulReturnLth,
                            zVIEW           lpView,
                            LPVIEWENTITYCSR lpViewEntityCsr,
                            LPVIEWATTRIB    lpViewAttrib )
{
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchRecord;
   zSHORT            nRC;

   nRC = 0;
   switch ( lpViewAttrib->cType )
   {
      case zTYPE_STRING:
      {
         zCOREMEM lpExternalP;
         zPULONG pulLong;

         // Default return length to 0
         *pulReturnLth = 0;

         // Get Entity Instance pointer
         lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

         // Get record to retrieve from
         pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

         if ( pchRecord )
         {
            pchRecord = pchRecord + lpViewAttrib->ulRecordOffset;
            if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               lpExternalP = (zCOREMEM) (pchRecord + 1);
               pulLong = (zPULONG) (lpExternalP + 1);
               *pulReturnLth = *pulLong;
            }
            else
               *pulReturnLth = zstrlen( pchRecord ) + 1;
         }

         break;
      }

      case zTYPE_INTEGER:
         *pulReturnLth = sizeof( zLONG );
         break;

      case zTYPE_DECIMAL:
         *pulReturnLth = sizeof( zDECIMAL );
         break;

      case zTYPE_DATETIME:
         *pulReturnLth = sizeof( DateTimeInternalRecord );
         break;

      case zTYPE_BLOB:

         // Default return length to 0
         *pulReturnLth = 0;

         // Get Entity Instance pointer.
         lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

         // Get record to retrieve from
         pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

         // if a record exists, copy it to the return variable
         if ( pchRecord )
         {
            pchRecord += lpViewAttrib->ulRecordOffset;
            if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zPULONG    pulLong;
               zCOREMEM   lpExternalP;

               lpExternalP = (zCOREMEM) (pchRecord + 1);
               pulLong = (zPULONG) (lpExternalP + 1);
               *pulReturnLth = *pulLong;
            }
         }

         break;

      default:
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         // "KZOEE239 - Invalid Operation for attribute type"
         fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ),
                           lpViewAttrib->szName );
         nRC = zCALL_ERROR;
      }
   }

   return( nRC );
}

//./ ADD NAME=SetAttributeFromString
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetAttributeFromString
//
//  PURPOSE:    To assign a value to an attribute from a string variable
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to set
//              cpcStringValue   - String to be used to set the Attribute
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetAttributeFromString( zVIEW    lpView,
                        zCPCHAR  cpcEntityName,
                        zCPCHAR  cpcAttributeName,
                        zCPCHAR  cpcStringValue )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetAttributeFromString,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
      nRC = fnSetAttributeFromVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                        (zPVOID) cpcStringValue, 'S', 0, 0,
                                        lpCurrentTask, 0 );

   fnOperationReturn( iSetAttributeFromString, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeFromString
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSetAttributeFromString
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnSetAttributeFromString( zVIEW            lpView,
                          LPVIEWENTITYCSR  lpViewEntityCsr,
                          LPVIEWATTRIB     lpViewAttrib,
                          zCPCHAR          cpcStringValue,
                          zSHORT           bProcessDomain )
{
   LPVIEWENTITY      lpViewEntity;
   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   zPCHAR            pchAttribValue;
   zCHAR             szNull[ 1 ];
   zPCHAR            pchWorkStr;
   zSHORT            nRC;

   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );
   lpViewCsr    = zGETPTR( lpView->hViewCsr );
   lpViewOI     = zGETPTR( lpViewCsr->hViewOI );

   // Get Entity Instance pointer
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   pchWorkStr = (zPCHAR) cpcStringValue;

   // If a null pointer was passed, assume attempt to null out string.
   if ( pchWorkStr == 0 )
   {
      szNull[ 0 ] = 0;
      pchWorkStr = szNull;
   }

   // Set pchAttribValue to string coming in before domain processing
   pchAttribValue = pchWorkStr;

   // set attrib from string
   nRC = 0;
   switch ( lpViewAttrib->cType )
   {
      case zTYPE_STRING:
         nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                             lpViewEntity,
                                             lpViewAttrib,
                                             pchAttribValue, 0 );
         break;

      case zTYPE_INTEGER:
      {
         zLONG lWork;

         if ( pchAttribValue[ 0 ] == 0 )
            lWork = lNullInteger;
         else
            lWork = zatol( pchAttribValue );

         nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                             lpViewEntity,
                                             lpViewAttrib,
                                             (zPVOID) &lWork, 0 );
      }

      break;

      case zTYPE_DECIMAL:
      {
         zDECIMAL   dDecimal;

         if ( pchAttribValue[ 0 ] == 0 )
            SysAssignDecimalFromNull( &dDecimal );
         else
            SysConvertStringToDecimal( pchAttribValue, &dDecimal );

         nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                             lpViewEntity,
                                             lpViewAttrib,
                                             (zPVOID) &dDecimal, 0 );
      }

      break;

      case zTYPE_DATETIME:
      {
         DateTimeInternalRecord  stDTLocal;
         LPDTINTERNAL            lpDTLocal;

         lpDTLocal = &stDTLocal;
         if ( pchAttribValue[ 0 ] == 0 )
         {
            stDTLocal.ulDateMinutes = (zULONG) lNullInteger;
            stDTLocal.usTSeconds = 0;
         }
         else
         {
            nRC = UfStringToDateTime( pchAttribValue, (LPDATETIME) lpDTLocal );
         }

         nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                             lpViewEntity,
                                             lpViewAttrib,
                                             (zPVOID) lpDTLocal, 0 );
      }

      break;

      default:
         nRC = zCALL_ERROR;   // function not supported for other types
   }

   return( nRC );
}

zSHORT OPERATION
SetAttributeFromView( zVIEW   lpView,
                      zCPCHAR cpcEntityName,
                      zCPCHAR cpcAttributeName,
                      zVIEW   vViewValue )
{
   SfLockView( vViewValue );  // we lock this view because it is being stored
                              // in an attribute and we don't want it automatically
                              // cleaned up.
   return( SetAttributeFromInteger( lpView, cpcEntityName,
                                    cpcAttributeName, (zLONG) vViewValue ) );
}

//./ ADD NAME=SetAttributeFromInteger
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetAttributeFromInteger
//
//  PURPOSE:    To assign a value to an attribute from an Integer (zLONG)
//              variable
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to set
//              lIntegerValue    - Integer to be used to set the Attribute
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetAttributeFromInteger( zVIEW   lpView,
                         zCPCHAR cpcEntityName,
                         zCPCHAR cpcAttributeName,
                         zLONG   lIntegerValue )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetAttributeFromInteger,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
      nRC = fnSetAttributeFromVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                        (zPVOID) &lIntegerValue, 'L', 0, 0,
                                        lpCurrentTask, 0 );

   fnOperationReturn( iSetAttributeFromInteger, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeFromInteger
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSetAttributeFromInteger
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnSetAttributeFromInteger( zVIEW            lpView,
                           LPVIEWENTITYCSR  lpViewEntityCsr,
                           LPVIEWATTRIB     lpViewAttrib,
                           zLONG            lIntegerValue )
{
   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWENTITY      lpViewEntity;
   zSHORT            nRC;

   lpViewEntity     = zGETPTR( lpViewEntityCsr->hViewEntity );
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // set attrib from string
   nRC = 0;
   if ( lpViewAttrib->cType == zTYPE_STRING )
   {
      zCHAR szAttribValue[ 32 ];

      szAttribValue[ 0 ] = 0;

      // Convert integer coming in to a string for storing in the record
      if ( lIntegerValue != lNullInteger )
         zltoa( lIntegerValue, szAttribValue, sizeof( szAttribValue ) );

      nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                          lpViewEntity,
                                          lpViewAttrib,
                                          szAttribValue, 0 );
   }
   else
   if ( lpViewAttrib->cType == zTYPE_INTEGER )
   {
      nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                          lpViewEntity,
                                          lpViewAttrib,
                                          &lIntegerValue, 0 );
   }
   else
   if ( lpViewAttrib->cType == zTYPE_DECIMAL ) // Internal Decimal type
   {
      zDECIMAL dDecimal;

      if ( lIntegerValue == lNullInteger )
         SysAssignDecimalFromNull( &dDecimal );
      else
         SysConvertLongToDecimal( lIntegerValue, &dDecimal );

      nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                          lpViewEntity,
                                          lpViewAttrib,
                                          &dDecimal, 0 );
   }
   else
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      // "KZOEE239 - Invalid Operation for attribute type"
      fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                        fnEntityNameFromInstance( lpEntityInstance ),
                        lpViewAttrib->szName );
      nRC = zCALL_ERROR;   // Operation not allowed for other types.
   }

   return( nRC );
}

//./ ADD NAME=AddToAttributeFromInteger
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      AddToAttributeFromInteger
//
//  PURPOSE:    To increment an attribute with an Integer
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to set
//              lIntegerValue    - Integer to be used to increment the
//                                 Attribute
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
AddToAttributeFromInteger( zVIEW   lpView,
                           zCPCHAR cpcEntityName,
                           zCPCHAR cpcAttributeName,
                           zLONG   lIntegerValue )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // If integer coming in is null, short circuit.
   if ( lIntegerValue == lNullInteger )
   {
      return( zCALL_ERROR );
   }

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iAddToAttributeFromInteger,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
      nRC = fnAddToAttributeFromVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                          (zPVOID) &lIntegerValue, zTYPE_INTEGER,
                                          0, 0, lpCurrentTask );

   fnOperationReturn( iAddToAttributeFromInteger, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeFromInteger
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnAddToAttributeFromInteger
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnAddToAttributeFromInteger( zVIEW            lpView,
                             LPVIEWENTITYCSR  lpViewEntityCsr,
                             LPVIEWATTRIB     lpViewAttrib,
                             zLONG            lIntegerValue )
{
   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   zPCHAR            pchRecord;
   zBOOL             bUpdated;
   zSHORT            nRC;

   lpViewCsr        = zGETPTR( lpView->hViewCsr );
   lpViewOI         = zGETPTR( lpViewCsr->hViewOI );
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Get record to store into
   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   // Default RC to 0 and attempt add
   nRC = 0;
   bUpdated = FALSE;
   if ( lpViewAttrib->cType == zTYPE_INTEGER )
   {
      zPLONG   lplLong;

      lplLong = (zPLONG) (pchRecord + lpViewAttrib->ulRecordOffset);
      if ( *lplLong != lNullInteger )
      {
         // Check to make sure that we're not overflowing.  We divide
         // everything by two so we don't have to convert longs to decimals.
         zLONG l1 = *lplLong / 2;
         zLONG l2 = lIntegerValue / 2;

         if ( l1 + l2 > LONG_MAX / 2 )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            //  "KZOEE235 - Integer overflow"
            fnIssueCoreError( lpTask, lpView, 16, 235, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            return( zCALL_ERROR );
         }
         else
         if ( l1 + l2 < LONG_MIN / 2 )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            //  "KZOEE236 - Integer underflow"
            fnIssueCoreError( lpTask, lpView, 16, 236, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            return( zCALL_ERROR );
         }
      }

      // do the addition
      if ( *lplLong == lNullInteger || lIntegerValue )
      {
         bUpdated = TRUE;
         if ( *lplLong == lNullInteger )
            *lplLong = 0;
      }

      *lplLong += lIntegerValue;
   }
   else
   if ( lpViewAttrib->cType == zTYPE_DECIMAL )
   {
      zPDECIMAL lpdDecimal;
      zDECIMAL  dDec;

      lpdDecimal = (zPDECIMAL) ( pchRecord + lpViewAttrib->ulRecordOffset );

      // do the addition
      if ( SysCompareDecimalToNull( lpdDecimal ) == 0 ||
           lIntegerValue )
      {
         bUpdated = TRUE;
         if ( SysCompareDecimalToNull( lpdDecimal ) == 0 )
            SysConvertLongToDecimal( 0, lpdDecimal );
      }

      SysConvertLongToDecimal( lIntegerValue, &dDec );
      SysAddDecimalToDecimal( lpdDecimal, lpdDecimal, &dDec );
   }
   else
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      //  "KZOEE234 - Attempt to add integer or decimal to invalid
      //              attribute type"
      fnIssueCoreError( lpTask, lpView, 8, 234, 0,
                        fnEntityNameFromCsr( lpViewEntityCsr ),
                        lpViewAttrib->szName );
      nRC = zCALL_ERROR;
   }

   // Set update indicator.
   fnSetUpdateIndicator( lpViewOI, lpEntityInstance, lpViewAttrib,
                         bUpdated, TRUE, TRUE );

   return( nRC );
}

//./ ADD NAME=SetAttributeFromDecimal
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetAttributeFromDecimal
//
//  PURPOSE:    To assign a value to an attribute from a Decimal
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to set
//              dDecimalValue    - Decimal to be used to set the Attribute
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetAttributeFromDecimal( zVIEW    lpView,
                         zCPCHAR  cpcEntityName,
                         zCPCHAR  cpcAttributeName,
                         zDECIMAL dDecimalValue )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetAttributeFromDecimal,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
      nRC = fnSetAttributeFromVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                        (zPVOID) &dDecimalValue,
                                        zTYPE_DECIMAL, 0, 0,
                                        lpCurrentTask, 0 );

   fnOperationReturn( iSetAttributeFromDecimal, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeFromDecimal
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSetAttributeFromDecimal
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnSetAttributeFromDecimal( zVIEW            lpView,
                           LPVIEWENTITYCSR  lpViewEntityCsr,
                           LPVIEWATTRIB     lpViewAttrib,
                           zDECIMAL         dDecimalValue )
{
   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWENTITY      lpViewEntity;
   zSHORT            nRC;

   lpViewEntity     = zGETPTR( lpViewEntityCsr->hViewEntity );
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Convert the decimal value to match the domain for the attribute and
   // call fnStoreValueInEntityInstance to store the value.

   nRC = 0;
   if ( lpViewAttrib->cType == zTYPE_STRING )
   {
      zCHAR szAttribValue[ 50 ] = "";

      // Convert decimal coming in to a string for storing in the record
      if ( SysCompareDecimalToNull( &dDecimalValue ) != 0 )
         SysConvertDecimalToString( &dDecimalValue, szAttribValue, 0 );

      nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                          lpViewEntity,
                                          lpViewAttrib,
                                          szAttribValue, 0 );
   }
   else
   if ( lpViewAttrib->cType == zTYPE_INTEGER )
   {
      zLONG lTempLong;

      if ( SysCompareDecimalToNull( &dDecimalValue ) == 0 )
         lTempLong = lNullInteger;
      else
         SysConvertDecimalToLong( &dDecimalValue, &lTempLong );

      nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                          lpViewEntity,
                                          lpViewAttrib,
                                          &lTempLong, 0 );
   }
   else
   if ( lpViewAttrib->cType == zTYPE_DECIMAL ) // Internal Decimal type
   {
      nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                          lpViewEntity,
                                          lpViewAttrib,
                                          &dDecimalValue, 0 );
   }
   else
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      // "KZOEE239 - Invalid Operation for attribute type"
      fnIssueCoreError( lpTask, lpView, 8, 239, 0,
                        fnEntityNameFromInstance( lpEntityInstance ),
                        lpViewAttrib->szName );
      nRC = zCALL_ERROR;   // Operation not allowed for other types.
   }

   return( nRC );
}

//./ ADD NAME=AddToAttributeFromDecimal
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      AddToAttributeFromDecimal
//
//  PURPOSE:    To increment an attribute with a Decimal
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to set
//              dDecimalValue    - Decimal to be used to increment the
//                                 Attribute
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
AddToAttributeFromDecimal( zVIEW     lpView,
                           zCPCHAR   cpcEntityName,
                           zCPCHAR   cpcAttributeName,
                           zDECIMAL  dDecimalValue )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // If decimal coming in is NULL, short circuit.
   if ( SysCompareDecimalToNull( &dDecimalValue ) == 0 )
      return( zCALL_ERROR );

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iAddToAttributeFromDecimal,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
      nRC = fnAddToAttributeFromVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                          (zPVOID) &dDecimalValue, zTYPE_DECIMAL,
                                          0, 0, lpCurrentTask );

   fnOperationReturn( iAddToAttributeFromDecimal, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeFromDecimal
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnAddToAttributeFromDecimal
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnAddToAttributeFromDecimal( zVIEW            lpView,
                             LPVIEWENTITYCSR  lpViewEntityCsr,
                             LPVIEWATTRIB     lpViewAttrib,
                             zDECIMAL         dDecimalValue )
{
   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   zPCHAR            pchRecord;
   zBOOL             bUpdated;
   zSHORT            nRC;

   lpViewCsr        = zGETPTR( lpView->hViewCsr );
   lpViewOI         = zGETPTR( lpViewCsr->hViewOI );
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Get record to store into
   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   // Default nRC to 0
   nRC = 0;
   bUpdated = FALSE;
   if ( lpViewAttrib->cType == zTYPE_INTEGER )
   {
      zPLONG    lplLong;
      zDECIMAL  dNewValue;
      zDECIMAL  dLong;

      lplLong = (zPLONG) (pchRecord + lpViewAttrib->ulRecordOffset);

      // Add lplLong to Decimal unless the integer is null, in which case
      // the integer will simply assume the passed value.
      if ( *lplLong != lNullInteger )
      {
         SysConvertLongToDecimal( *lplLong, &dNewValue );
         SysAddDecimalToDecimal( &dNewValue, &dNewValue, &dDecimalValue );
      }
      else
         SysAssignDecimalFromDecimal( &dNewValue, &dDecimalValue );

      SysConvertLongToDecimal( LONG_MAX, &dLong );
      if ( SysCompareDecimalToDecimal( &dNewValue, &dLong ) > 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE235 - Integer overflow"
         fnIssueCoreError( lpTask, lpView, 16, 235, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ),
                           lpViewAttrib->szName );
         return( zCALL_ERROR );
      }

      SysConvertLongToDecimal( LONG_MIN, &dLong );
      if ( SysCompareDecimalToDecimal( &dNewValue, &dLong ) < 0 )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE236 - Integer underflow"
         fnIssueCoreError( lpTask, lpView, 16, 236, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ),
                           lpViewAttrib->szName );
         return( zCALL_ERROR );
      }

      SysConvertLongToDecimal( *lplLong, &dLong );
      if ( SysCompareDecimalToDecimal( &dNewValue, &dLong ) != 0 )
      {
         SysConvertDecimalToLong( &dNewValue, lplLong );
         bUpdated = TRUE;
      }
   }
   else
   if ( lpViewAttrib->cType == zTYPE_DECIMAL ) // Internal Decimal type
   {
      zPDECIMAL lpdDecimal;
      zDECIMAL  dDec;

      lpdDecimal = (zPDECIMAL) (pchRecord + lpViewAttrib->ulRecordOffset);

      // If either a non-zero addend is coming in or the attribute
      // is currently null, set the update indicator to true
      SysConvertLongToDecimal( 0, &dDec );
      if ( SysCompareDecimalToNull( lpdDecimal ) == 0 ||
           SysCompareDecimalToDecimal( &dDecimalValue, &dDec ) != 0 )
      {
         bUpdated = TRUE;

         // if the current attribute is null, set it to 0 for the add
         if ( SysCompareDecimalToNull( lpdDecimal ) == 0 )
            SysConvertLongToDecimal( 0, lpdDecimal );
      }

      SysAddDecimalToDecimal( lpdDecimal, lpdDecimal, &dDecimalValue );
   }
   else
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      //  "KZOEE234 - Attempt to add integer or decimal to invalid
      //              attribute type"
      fnIssueCoreError( lpTask, lpView, 8, 234, 0,
                        fnEntityNameFromCsr( lpViewEntityCsr ),
                        lpViewAttrib->szName );
      nRC = zCALL_ERROR;
   }

   // Set update indicator.
   fnSetUpdateIndicator( lpViewOI, lpEntityInstance, lpViewAttrib,
                         bUpdated, TRUE, TRUE );

   return( nRC );
}

//./ ADD NAME=SetAttributeFromBlob
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetAttributeFromBlob
//
//  PURPOSE:    To assign a value to an attribute from a Binary large
//              object (Blob)
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to set
//              pBlobValue       - Blob to be used to set the Attribute
//              ulLth            - Size of blob
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetAttributeFromBlob( zVIEW   lpView,
                      zCPCHAR cpcEntityName,
                      zCPCHAR cpcAttributeName,
                      zPVOID  pBlobValue,
                      zULONG  ulLth )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetAttributeFromBlob,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
      nRC = zCALL_ERROR;
   else
   {
      // Call fnSetAttributeFromBlob to do the setting
      nRC = fnSetAttributeFromBlob( lpView, lpViewEntityCsr,
                                    lpViewAttrib, pBlobValue, ulLth );
   }

   fnOperationReturn( iSetAttributeFromBlob, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetBlobFromEntityAttributes
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetBlobFromEntityAttributes
//
//  PURPOSE:    To assign a Blob from the Entity Attributes in another
//              Entity.
//
//  PARAMETERS: lpTgtView        - View to Object instance to be set
//              cpcTgtEntityName - Entity whose attributes are to be set
//              lpSrcView        - View to Object containing blob
//              cpcSrcEntityName - Entity containing blob
//              cpcAttributeName - Blob containing Entity attrs
//
//  RETURNS:    0           - Entity Attributes successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetBlobFromEntityAttributes( zVIEW   lpTgtView,
                             zCPCHAR cpcTgtEntityName,
                             zCPCHAR cpcAttributeName,
                             zVIEW   lpSrcView,
                             zCPCHAR cpcSrcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWATTRIB      lpWrkViewAttrib;
   LPENTITYINSTANCE  lpSrcEntityInstance;
   LPENTITYINSTANCE  lpTgtEntityInstance;
   zULONG            ulLth;
   zPCHAR            pchBlob;
   zPVOID            hBlob;
   zPCHAR            pchWork;
   zCOREMEM          lpExternalP;
   zPULONG           lpuBlobLth;
   zPLONG            lplLong;
   zPLONG            lplLong2;
   zPDECIMAL         lpdDecimal;
   zPDECIMAL         lpdDecimal2;
   zPCHAR            pchRecord;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpTgtViewEntity,
                                        &lpTgtViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetBlobFromEntityAttributes,
                                        0,   // We no not intend to update
                                        lpTgtView,
                                        cpcTgtEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
   {
      fnOperationReturn( iSetBlobFromEntityAttributes, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Validate parameters.
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpSrcViewEntity,
                                        &lpSrcViewEntityCsr,
                                        0,
                                        iSetBlobFromEntityAttributes,
                                        0,   // We do not intend to update
                                        lpSrcView,
                                        cpcSrcEntityName,
                                        0,
                                        zUSE_TASK ); // Oper id already passed
   if ( nRC )
   {
      fnOperationReturn( iSetBlobFromEntityAttributes, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpViewCsr           = zGETPTR( lpTgtView->hViewCsr );
   lpViewOI            = zGETPTR( lpViewCsr->hViewOI );
   lpSrcEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
   lpTgtEntityInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
   pchBlob = 0;
   ulLth   = 0;

   // If there is data, make two passes through the attributes, the first
   // to calculate the length of the blob to be created and the second to
   // actually create the data.
   if ( lpSrcEntityInstance )
   {
      lpWrkViewAttrib = zGETPTR( lpSrcViewEntity->hFirstOD_Attrib );
      while ( lpWrkViewAttrib )
      {
         pchRecord = fnRecordForEntityAttr( lpSrcEntityInstance, lpWrkViewAttrib );
         if ( pchRecord )
         {
            pchRecord += lpWrkViewAttrib->ulRecordOffset;
            if ( pchBlob )
            {
               lplLong = (zPLONG) pchWork;             // we are in the second pass, so pchWork
               *lplLong = lpWrkViewAttrib->lERAttTok;  // points to an area of memory
            }

            switch ( lpWrkViewAttrib->cType )
            {
               case zTYPE_STRING:
                  // if the attribute is not null
                  if ( pchRecord[ 0 ] )
                  {
                     if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
                     {
                        lpExternalP = (zCOREMEM) (pchRecord + 1);
                        pchRecord = zGETPTR( *lpExternalP );
                     }

                     // if this is the blob creation pass, set the data in the blob
                     if ( pchBlob )
                     {
                        zLONG lLth;

                        // Bump work blob pointer past attribute token
                        pchWork += sizeof( zLONG );

                        // Set Attribute type
                        pchWork[ 0 ] = lpWrkViewAttrib->cType;

                        // Bump past attribute type
                        pchWork++;

                        // We allocated "zstrlen( pchRecord ) + 1" bytes in the first pass.
                        lLth = zstrlen( pchRecord ) + 1;
                        strcpy_s( pchWork, lLth, pchRecord );
                        pchWork += lLth;
                     }
                     else
                     {
                        // First pass: Bump the length and blob pointers
                        ulLth += sizeof( zLONG ) + 1;
                        ulLth += zstrlen( pchRecord ) + 1;
                     }
                  }

                  break;

               case zTYPE_INTEGER:
                  lplLong2 = (zPLONG) pchRecord;
                  if ( *lplLong2 != lNullInteger )
                  {
                     if ( pchBlob )
                     {
                        // Bump work blob pointer past attribute token
                        pchWork += sizeof( zLONG );

                        // Set Attribute type
                        pchWork[ 0 ] = lpWrkViewAttrib->cType;

                        // Bump past attribute type
                        pchWork++;

                        // Set long pointer for assignment
                        lplLong = (zPLONG) pchWork;
                        *lplLong = *lplLong2;
                        pchWork += sizeof( zLONG );
                     }
                     else
                     {
                        // First pass: Increment length for attribute token, attribute
                        // type and attribute value
                        ulLth += (2 * sizeof( zLONG )) + 1;
                     }
                  }

                  break;

               case zTYPE_DECIMAL:
                  lpdDecimal2 = (zPDECIMAL) pchRecord;
                  if ( SysCompareDecimalToNull( lpdDecimal2 ) != 0 )
                  {
                     if ( pchBlob )
                     {
                        // Bump work blob pointer past attribute token
                        pchWork += sizeof( zLONG );

                        // Set Attribute type
                        pchWork[ 0 ] = lpWrkViewAttrib->cType;

                        // Bump past attribute type
                        pchWork++;

                        // Set long pointer for assignment
                        lpdDecimal = (zPDECIMAL) pchWork;
                        SysAssignDecimalFromDecimal( lpdDecimal, lpdDecimal2 );
                        pchWork += sizeof( zDECIMAL );
                     }
                     else
                     {
                        // First pass: Increment length for attribute token, attribute
                        // type and attribute value
                        ulLth += sizeof( zDECIMAL ) + sizeof( zLONG ) + 1;
                     }
                  }

                  break;

               case zTYPE_DATETIME:
                  lplLong2 = (zPLONG) pchRecord;
                  if ( *lplLong2 != lNullInteger )
                  {
                     if ( pchBlob )
                     {
                        // Bump work blob pointer past attribute token
                        pchWork += sizeof( zLONG );

                        // Set Attribute type
                        pchWork[ 0 ] = lpWrkViewAttrib->cType;

                        // Bump past attribute type
                        pchWork++;

                        // Set long pointer for assignment
                        strncpy_s( pchWork, sizeof( DateTimeInternalRecord ), pchRecord, sizeof( DateTimeInternalRecord ) - 1 );
                        pchWork[ sizeof( DateTimeInternalRecord ) ] = 0;
                        pchWork += sizeof( DateTimeInternalRecord );
                     }
                     else
                     {
                        // First pass: Increment length for attribute token, attribute
                        // type and attribute value
                        ulLth += sizeof( zLONG ) + 1 + sizeof( DateTimeInternalRecord );
                     }
                  }

                  break;

               case zTYPE_BLOB:   // Blob attribute
                  if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
                  {
                     lpExternalP  = (zCOREMEM) (pchRecord + 1);
                     lpuBlobLth  = (zPULONG) (lpExternalP + 1);
                     pchRecord = zGETPTR( *lpExternalP );
                     if ( pchBlob )
                     {
                        // Bump work blob pointer past attribute token.
                        pchWork += sizeof( zLONG );

                        // Set Attribute type
                        pchWork[ 0 ] = lpWrkViewAttrib->cType;

                        // Bump past attribute type.
                        pchWork++;

                        // Copy blob data to output blob.
                        zmemcpy( pchWork, (zPCHAR) lpuBlobLth, sizeof( zULONG ) );
                        pchWork += sizeof( zULONG );
                        zmemcpy( pchWork, pchRecord, (unsigned int) *lpuBlobLth );
                        pchWork += *lpuBlobLth;
                     }
                     else
                     {
                        // First pass: Increment length for attribute token, attribute
                        // type and attribute value
                        ulLth += sizeof( zLONG ) + 1 + sizeof( zULONG ) + *lpuBlobLth;
                     }
                  }

                  break;

               default:
                  break;
            }
         }

         lpWrkViewAttrib = zGETPTR( lpWrkViewAttrib->hNextOD_Attrib );

         // If we've passed all the attributes and a blob has yet to be
         // created, allocate the blob now and create the blob data
         if ( lpWrkViewAttrib == 0 && pchBlob == 0 && ulLth )
         {
            ulLth += sizeof( zLONG ); // Add 4 to length for entity token

            // We allocate extra bytes for setting attribute tokens
            // in the event the last attribute is null and we've already
            // filled up the blob space calculated
            hBlob = fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                      ulLth + sizeof( zLONG ), 0, 0, 0 );
            pchBlob = zGETPTR( hBlob );
            if ( pchBlob )
            {
               // First store the ER Entity token in the blob
               pchWork = pchBlob;
               lplLong = (zPLONG) pchWork;
               *lplLong = lpSrcViewEntity->lEREntTok;
               pchWork += sizeof( zLONG );
               lpWrkViewAttrib = zGETPTR( lpSrcViewEntity->hFirstOD_Attrib );
            }
            else
            {
               nRC = zCALL_ERROR;
               break;
            }
         }
      }
   }

   // Call fnSetAttributeFromBlob to do the setting.
   if ( ulLth )
   {
      nRC = fnSetAttributeFromBlob( lpTgtView, lpTgtViewEntityCsr,
                                    lpViewAttrib, pchBlob, ulLth );
      fnFreeDataspace( zGETPTR( hBlob ) );
   }
   else
   {
      nRC = fnSetAttributeFromBlob( lpTgtView, lpTgtViewEntityCsr,
                                    lpViewAttrib,
                                    &lpSrcViewEntity->lEREntTok,
                                    sizeof( zLONG ) );
   }

   fnSetUpdateIndicator( 0, lpTgtEntityInstance, lpViewAttrib, 1, TRUE, TRUE );

   // Return the results.
   fnOperationReturn( iSetBlobFromEntityAttributes, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetBlobFromOI
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetBlobFromOI
//
//  PURPOSE:    To assign a Blob from an entire OI.
//
//  PARAMETERS: lpTgtView        - View to Object instance to be set
//              cpcTgtEntityName - Entity whose attributes are to be set
//              cpcAttributeName - Blob containing Entity attrs
//              lpSrcView        - View to Object containing blob
//              lControl         - Controls certain aspects of storing the
//                                 OI; values: zINCREMENTAL,
//                                 zCOMPRESSED, zSAVE_CURSORS.
//
//  Note if zCOMPRESSED is set:  If the OI definition has been changed
//  between storing and retrieval of the OI the resulting OI could have
//  invalid data.
//
//  RETURNS:    0           - Entity Attributes successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetBlobFromOI( zVIEW   lpTgtView,
               zCPCHAR cpcTgtEntityName,
               zCPCHAR cpcAttributeName,
               zVIEW   lpSrcView,
               zLONG   lControl )
{
   LPTASK            lpCurrentTask;
   LPVIEWOD          lpViewOD;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC = zCALL_ERROR;
   zCHAR             szFileName[ zMAX_FILENAME_LTH + 1 ];
   zBOOL             bFileCreated = FALSE;
   zLONG             hFile = -1;
   zULONG            ulLth;
   zLONG             hMem = 0;
   zPCHAR            pchData;
   LPBLOBOI_RECORD   lpBlobOI;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpTgtViewEntity,
                                        &lpTgtViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetBlobFromOI,
                                        1,   // We intend to update
                                        lpTgtView,
                                        cpcTgtEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 || fnValidView( lpCurrentTask, lpSrcView ) == 0 )
   {
      fnOperationReturn( iSetBlobFromOI, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpViewOD = zGETPTR( lpSrcView->hViewOD );

   SysCreateTempFileName( szFileName );

   // Now store the OI as a temporary file.
   if ( CommitOI_ToFile( lpSrcView, szFileName, lControl ) != 0 )
      goto EndOfFunction;

   bFileCreated = TRUE;

   hFile = SysOpenFile( lpSrcView, szFileName, COREFILE_READ );
   if ( hFile == -1 )
   {
      // "KZOEE190 - Error opening temporary file"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 16, 190, 0, szFileName, 0 );
      goto EndOfFunction;
   }

   // Get the file length.
   ulLth = SysGetFileSize( hFile );

   // Now allocate space big enough to hold the entire file.
   hMem = SysAllocMemory( (zCOREMEM) &pchData, ulLth + sizeof( BlobOI_Record ),
                          0, zCOREMEM_ALLOC, 0 );
   if ( hMem == 0 )
      goto EndOfFunction;

   // Set header information.
   lpBlobOI = (LPBLOBOI_RECORD) pchData;
   zmemset( lpBlobOI, '*', sizeof( BlobOI_Record ) );
   strcpy_s( lpBlobOI->cpcViewOD_Name, sizeof( lpBlobOI->cpcViewOD_Name ), lpViewOD->szName );

   if ( SysReadFile( lpSrcView, hFile, pchData + sizeof( BlobOI_Record ), ulLth ) != ulLth )
   {
      TraceLineI( "(at) Couldn't read entire file.  Lth = ", ulLth );
      goto EndOfFunction;
   }

   // Last thing to do--set blob attribute.
   nRC = fnSetAttributeFromBlob( lpTgtView, lpTgtViewEntityCsr,
                                 lpViewAttrib, pchData,
                                 ulLth + sizeof( BlobOI_Record ) );

EndOfFunction:

   if ( hMem )
      SysFreeMemory( hMem );

   if ( hFile != -1 )
      SysCloseFile( lpSrcView, hFile, 0 );

   if ( bFileCreated )
      SysOpenFile( lpSrcView, szFileName, COREFILE_DELETE );

   // Return the results
   fnOperationReturn( iSetBlobFromOI, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetEntityAttributesFromBlob
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetEntityAttributesFromBlob
//
//  PURPOSE:    To assign the Entity attributes from a blob previously
//              created with the operation SetBlobFromEntityAttributes
//
//  PARAMETERS: lpTgtView        - View to Object instance to be set
//              cpcTgtEntityName - Entity whose attributes are to be set
//              lpSrcView        - View to Object containing blob
//              cpcSrcEntityName - Entity containing blob
//              cpcAttributeName - Blob containing Entity attrs
//
//  RETURNS:    0           - Entity Attributes successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetEntityAttributesFromBlob( zVIEW   lpTgtView,
                             zCPCHAR cpcTgtEntityName,
                             zVIEW   lpSrcView,
                             zCPCHAR cpcSrcEntityName,
                             zCPCHAR cpcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpTgtViewOI;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWATTRIB      lpWrkViewAttrib;
   LPENTITYINSTANCE  lpSrcEntityInstance;
   LPENTITYINSTANCE  lpTgtEntityInstance;
   LPENTITYINSTANCE  lpLinkedInstance;
   zULONG            ulLth;
   zPCHAR            pchBlob;
   zCOREMEM          lpExternalP;
   zPULONG           lpulLth;
   zPLONG            lplLong;
   zPCHAR            pchRecord;
   zSHORT            nRC;

   // Validate parameters.
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpSrcViewEntity,
                                        &lpSrcViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetEntityAttributesFromBlob,
                                        0,   // We do not intend to update
                                        lpSrcView,
                                        cpcSrcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn
   else
   if ( nRC == 0 )
   {
      // Validate parameters.  For now we'll ignore checking the keys.
      nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                           &lpTgtViewEntity,
                                           &lpTgtViewEntityCsr,
                                           0,
                                           iSetEntityAttributesFromBlob,
                                           1,   // We intend to update
                                           lpTgtView,
                                           cpcTgtEntityName,
                                           0,
                                           zIGNORE_KEYS |
                                             zUSE_TASK ); // Oper id already passed
   }

   if ( nRC )
   {
      fnOperationReturn( iSetEntityAttributesFromBlob, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Validate that the attribute is a blob.
   if ( lpViewAttrib->cType != zTYPE_BLOB )
   {
      LPTASK lpTask = zGETPTR( lpSrcView->hTask );
      // "KZOEE239 - Invalid Operation for attribute type"
      fnIssueCoreError( lpTask, lpSrcView, 8, 239, 0,
                        lpSrcViewEntity->szName,
                        lpViewAttrib->szName );
      fnOperationReturn( iSetEntityAttributesFromBlob, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpViewCsr           = zGETPTR( lpTgtView->hViewCsr );
   lpTgtViewOI         = zGETPTR( lpViewCsr->hViewOI );
   lpSrcEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );
   lpTgtEntityInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
   pchRecord = 0;
   nRC = 0;

   // This should never be true, but is here for protection.
   if ( lpSrcEntityInstance == 0 || lpTgtEntityInstance == 0 )
   {
      fnOperationReturn( iSetEntityAttributesFromBlob, lpCurrentTask );
      return( 0 );
   }

   // Get the address of the blob and validate that the blob was created
   // from the entity in the first place
   pchRecord = fnRecordForEntityAttr( lpSrcEntityInstance, lpViewAttrib );
   if ( pchRecord )
   {
      pchRecord += lpViewAttrib->ulRecordOffset;
      if ( pchRecord[ 0 ] != (zCHAR) '\xff' )
         pchRecord = 0;
      else
      {
         // Get the address of the blob and ensure that the beginning of
         // the blob is the ER_Entity Token of the target entity
         lpExternalP = (zCOREMEM) (pchRecord + 1);
         lpulLth     = (zPULONG) (lpExternalP + 1);
         ulLth       = *lpulLth;
         pchBlob      = zGETPTR( *lpExternalP );
         lplLong     = (zPLONG) pchBlob;
         if ( ulLth < sizeof ( zLONG ) ||
              *lplLong != lpTgtViewEntity->lEREntTok )
         {
            LPTASK lpTask = zGETPTR( lpSrcView->hTask );
            // "KZOEE247 - Blob Attribute does not match Target Entity Type"
            fnIssueCoreError( lpTask, lpSrcView, 8, 247, 0,
                              lpSrcViewEntity->szName,
                              lpViewAttrib->szName );
            fnOperationReturn( iSetEntityAttributesFromBlob, lpCurrentTask );
            return( zCALL_ERROR );
         }

         ulLth -= sizeof( zLONG );
         pchBlob += sizeof( zLONG );
      }
   }

   // If we get here, then all the edits have passed, delete the target
   // entity information before moving on
   fnDeleteAttributeRecord( lpTgtViewEntity, lpTgtEntityInstance, FALSE );
   fnDeleteAttributeRecord( lpTgtViewEntity, lpTgtEntityInstance, TRUE );
   if ( lpTgtEntityInstance->hNextLinked )
   {
      lpLinkedInstance = zGETPTR( lpTgtEntityInstance->hNextLinked );
      while ( lpLinkedInstance != lpTgtEntityInstance )
      {
         lpLinkedInstance->hPersistRecord = 0;
         lpLinkedInstance = zGETPTR( lpLinkedInstance->hNextLinked );
      }
   }

   // If a source blob exists, go through the attributes stored in the
   // blob, match them up to the attributes in the target entity and reset
   // the target attributes found if the target type has not changed.
   // NOTE: Attributes not found in the target are ignored.
   if ( pchRecord )
   {
      while ( nRC == 0 && ulLth > 0 )
      {
         lplLong = (zPLONG) pchBlob;
         pchBlob += sizeof( zLONG );
         ulLth  -= sizeof( zLONG );
         lpWrkViewAttrib = zGETPTR( lpTgtViewEntity->hFirstOD_Attrib );
         while ( lpWrkViewAttrib &&
                 lpWrkViewAttrib->lERAttTok != *lplLong )
         {
            lpWrkViewAttrib = zGETPTR( lpWrkViewAttrib->hNextOD_Attrib );
         }

         if ( lpWrkViewAttrib && lpWrkViewAttrib->cType == pchBlob[ 0 ] )
         {
            if ( pchBlob[ 0 ] != zTYPE_BLOB )
            {
               // Assign the attribute in the target Entity
               StoreValueInRecord( lpTgtView,
                                   lpTgtViewEntity, lpWrkViewAttrib,
                                   pchBlob + 1, 0 );
            }
            else
            {
               // Get address of length data
               lpulLth = (zPULONG) pchBlob + 1;

               // Assign the attribute in the target Entity
               StoreValueInRecord( lpTgtView,
                                   lpTgtViewEntity, lpWrkViewAttrib,
                                   pchBlob + 1 + sizeof( zULONG ), *lpulLth );
            }

            fnSetUpdateIndicator( lpTgtViewOI, lpTgtEntityInstance,
                                  lpWrkViewAttrib, TRUE, TRUE, TRUE );

         }

         switch ( pchBlob[ 0 ] )
         {
            // If the attribute is a string, we get the length of the
            // string plus the preceding type character 'S' and add 1 for
            // the null terminator at the end of the string; Otherwise,
            // take the attribute length and add 1 for the attribute type
            case zTYPE_STRING:
               ulLth  -= zstrlen( pchBlob ) + 1;
               pchBlob += zstrlen( pchBlob ) + 1;
               break;

            case zTYPE_INTEGER:
               ulLth  -= sizeof( zLONG ) + 1;
               pchBlob += sizeof( zLONG ) + 1;
               break;

            case zTYPE_DECIMAL:
               ulLth  -= sizeof( zDECIMAL ) + 1;
               pchBlob += sizeof( zDECIMAL ) + 1;
               break;

            case zTYPE_DATETIME:
               ulLth  -= sizeof( DateTimeInternalRecord ) + 1;
               pchBlob += sizeof( DateTimeInternalRecord ) + 1;
               break;

            case zTYPE_BLOB:
               ulLth  -= *lpulLth + sizeof( zULONG ) + 1;
               pchBlob += *lpulLth + sizeof( zULONG ) + 1;
               break;

            default:
               nRC = zCALL_ERROR;
               break;
         }
      }
   }

   fnOperationReturn( iSetEntityAttributesFromBlob, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetOI_FromBlob
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetOI_FromBlob
//
//  PURPOSE:    To create an OI from a blob that was set with
//              SetBlobFromOI( ).
//
//  PARAMETERS: plpTgtView      - View to Object instance to be set
//              cpcViewOD_Name  - RETURNS the view object name. May be 0.
//              vSubtask        - Used to qualify the application in which
//                                to look for the object.  If the view
//                                passed is a subtask view, then the
//                                application that is used is the
//                                application tied to the subtask.
//                                Otherwise, the application for the view
//                                object associated with the passed view
//                                is used.
//              lpSrcView       - View to Object containing blob
//              cpcSrcEntityName - Entity containing blob.
//              cpcAttributeName - Blob containing OI.
//              lControl        - Controls certain aspects of retrieving
//                                the OI; values:
//                            zSINGLE   - (default) only one entity at the
//                                        root level.
//                            zMULTIPLE - multiple entities at the root
//                                        level
//                            zLEVEL_APPLICATION - created view is tied
//                                        to the application task,
//                                        rather than the active task.
//                            zLEVEL_SYSTEM   - created view is tied
//                                        with the system task,
//                                        rather than the active task.
//                            zLEVEL_SAME - created view is tied to the
//                                        same task as lpAppQualView.
//                                        Note: specifying this option
//                                        requires that lpAppQualView be
//                                        be specified and that it not be
//                                        a subtask view.
//                            zNOI_OKAY - when set, if the file could not
//                                        be found, no error message is
//                                        issued, just return code set.
//                            zREADONLY - Marks the View and the Instance
//                                        as read only.  Any view created
//                                        from this view will also be
//                                        marked as read only.
//                            zIGNORE_ERRORS - Loads what info it can if
//                                        the file is ASCII
//                            zNEW_OBJECT_NAME - Override the object name
//                                        written in the blob with the one
//                                        specified in pchReturnViewOD_Name.
//
//  Note if zCOMPRESSED is set:  If the OI definition has been changed
//  between storing and retrieval of the OI the resulting OI could have
//  invalid data.
//
//  RETURNS:   -1 - Blob is NULL.  No OI is returned.
//              0 - Object instance activated, single root found
//              1 - Object instance activated, multiple roots found.  If
//                  zSINGLE was specified, only the first was activated.
//    zCALL_ERROR - Error Activating object instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
SetOI_FromBlob( zPVIEW  plpTgtView,
                zPCHAR  pchReturnViewOD_Name,
                zVIEW   vSubtask,
                zVIEW   lpSrcView,
                zCPCHAR cpcSrcEntityName,
                zCPCHAR cpcAttributeName,
                zLONG   lControl )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zCHAR             szFileName[ zMAX_FILENAME_LTH + 1 ];
   zBOOL             bFileCreated = FALSE;
   zLONG             hFile = -1;
   zULONG            ulLth;
   zPCHAR            pchData;
   zPCHAR            pch;
   LPBLOBOI_RECORD   lpOI_Data;
   zSHORT            nRC;

   *plpTgtView = 0;
   if ( pchReturnViewOD_Name && (lControl & zNEW_OBJECT_NAME) == 0 )
      *pchReturnViewOD_Name = 0;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpSrcViewEntity,
                                        &lpSrcViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetOI_FromBlob,
                                        0,   // We do not intend to update
                                        lpSrcView,
                                        cpcSrcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC || fnValidView( lpCurrentTask, vSubtask ) == 0 )
   {
      fnOperationReturn( iSetOI_FromBlob, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = zCALL_ERROR;
   if ( lControl & zNEW_OBJECT_NAME )
   {
      if ( pchReturnViewOD_Name == 0 )
      {
         fnOperationReturn( iSetOI_FromBlob, lpCurrentTask );
         return( zCALL_ERROR );
      }

      if ( ActivateViewObject( vSubtask, pchReturnViewOD_Name, 0 ) == 0 )
      {
         fnOperationReturn( iSetOI_FromBlob, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   SysCreateTempFileName( szFileName );

   fnGetAddrForAttribute( &pchData, lpSrcView, lpSrcViewEntityCsr, 0,
                          lpViewAttrib );

   fnGetAttributeLength( &ulLth, lpSrcView,
                         zGETPTR( lpSrcViewEntityCsr->hEntityInstance ),
                         lpViewAttrib );

   // If there is no blob data then return -1.
   if ( pchData == 0 || ulLth == 0 )
   {
      nRC = -1;
      goto EndOfFunction;
   }

   // Get header information at the beginning of the blob.
   pch = zstrchr( pchData, ' ' );  // ensure OD name is terminated ... dks 2011.08.19
   if ( pch && (pch - pchData) < zZEIDON_NAME_LTH)
      *pch = 0;
   else
      *(pchData + zZEIDON_NAME_LTH) = 0;

   lpOI_Data = (LPBLOBOI_RECORD) pchData;
   pchData += sizeof( BlobOI_Record );
   ulLth  -= sizeof( BlobOI_Record );

   if ( lControl & zNEW_OBJECT_NAME )
      strcpy_s( lpOI_Data->cpcViewOD_Name, sizeof( lpOI_Data->cpcViewOD_Name ), pchReturnViewOD_Name );
   else
   if ( pchReturnViewOD_Name )
      strcpy_s( pchReturnViewOD_Name, zZEIDON_NAME_LTH + 1, lpOI_Data->cpcViewOD_Name );

   // Write the blob to a temp file.
   hFile = SysOpenFile( vSubtask, szFileName, COREFILE_WRITE );
   if ( hFile == -1 )
   {
      // "KZOEE190 - Error opening temporary file"
      fnIssueCoreError( lpCurrentTask, lpSrcView, 16, 190, 0, szFileName, 0 );
      goto EndOfFunction;
   }

   if ( SysWriteFile( vSubtask, hFile, pchData, ulLth ) != (int) ulLth )
      goto EndOfFunction;

   SysCloseFile( vSubtask, hFile, 0 );
   hFile = -1;

   bFileCreated = TRUE;

   // Last thing to do--read the file into an OI.
   nRC = ActivateOI_FromFile( plpTgtView, lpOI_Data->cpcViewOD_Name,
                              vSubtask, szFileName, lControl );
   if ( nRC == -1 && *plpTgtView )
   {
      // No OI was activated but we were returned an empty OI.  Drop the
      // empty OI and reset plpTgtView.
      fnDropView( *plpTgtView );
      *plpTgtView = 0;
   }
   else
   if ( nRC == zCALL_ERROR )
   {
      // "KZOEE096 - Error retrieving OI from blob."
      fnIssueCoreError( lpCurrentTask, lpSrcView, 16, 96, 0, lpSrcViewEntity->szName,
                        lpViewAttrib->szName );
   }

EndOfFunction:

   if ( hFile != -1 )
      SysCloseFile( vSubtask, hFile, 0 );

   if ( bFileCreated )
      SysOpenFile( vSubtask, szFileName, COREFILE_DELETE );

   // Return the results
   fnOperationReturn( iSetOI_FromBlob, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeFromBlob
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSetAttributeFromBlob
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnSetAttributeFromBlob( zVIEW            lpView,
                        LPVIEWENTITYCSR  lpViewEntityCsr,
                        LPVIEWATTRIB     lpViewAttrib,
                        zPVOID           pBlobValue,
                        zULONG           uBlobLth )
{
   LPTASK            lpAllocTask;
   LPVIEWENTITY      lpViewEntity;
   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   zPCHAR            pchRecord;
   zBOOL             bUpdated;
   zSHORT            nRC;

   // Get view Entity from cursor
   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );
   lpViewCsr    = zGETPTR( lpView->hViewCsr );
   lpViewOI     = zGETPTR( lpViewCsr->hViewOI );
   lpAllocTask  = zGETPTR( lpViewOI->hAllocTask );

   // Get Entity Instance pointer
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Make sure target attribute is a Blob
   if ( lpViewAttrib->cType != zTYPE_BLOB )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      // "KZOEE239 - Invalid Operation for attribute type"
      fnIssueCoreError( lpTask, lpView, 8, 239, 0, lpViewEntity->szName,
                        lpViewAttrib->szName );
      return( zCALL_ERROR );
   }

   // Get record to store into
   if ( lpViewAttrib->bPersist ) // is attribute persistent
   {
      pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
      if ( pchRecord == 0 )
      {
         fnCreateAttributeRecord( lpAllocTask,
                                  lpViewEntity, lpEntityInstance );
         pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
         if ( pchRecord == 0 )
            return( zCALL_ERROR );
      }
   }
   else
   {
      pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord );
      if ( pchRecord == 0 )
      {
         lpEntityInstance->hNonPersistRecord =
                     fnCreateAttributeRecord( lpAllocTask, lpViewEntity, 0 );
         pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord );
         if ( pchRecord == 0 )
            return( zCALL_ERROR );
      }
   }

   // Set attrib from string.
   nRC = 0;
   bUpdated = FALSE;

   nRC = fnStoreBlobInRecord( lpAllocTask,      // so allocation can be done
                              pchRecord,        // Attribute record
                              lpViewAttrib->ulRecordOffset,   // Offset
                              pBlobValue,       // Blob value to store
                              uBlobLth );       // Length to store
   // Check return code
   if ( nRC )
   {
      if ( nRC == 1 )
      {
         bUpdated = TRUE;
         nRC = 0;
      }
   }

   // Set update indicator.
   fnSetUpdateIndicator( lpViewOI, lpEntityInstance, lpViewAttrib,
                         bUpdated, TRUE, TRUE );
   return( nRC );
}

//./ ADD NAME=SetAttributeFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetAttributeFromAttribute
//
//  PURPOSE:    To assign a value to an attribute from another attribute
//
//  PARAMETERS: lpTgtView           - View containing target attribute
//              cpcTgtEntityName    - Entity containing target attribute
//              cpcTgtAttributeName - Attribute to be set
//              lpSrcView           - View containing source attribute
//              cpcTgtEntityName    - Entity containing source attribute
//              cpcSrcAttributeName - Attribute used to set target Attr
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
SetAttributeFromAttribute( zVIEW    lpTgtView,
                           zCPCHAR  cpcTgtEntityName,
                           zCPCHAR  cpcTgtAttributeName,
                           zVIEW    lpSrcView,
                           zCPCHAR  cpcSrcEntityName,
                           zCPCHAR  cpcSrcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWATTRIB      lpSrcViewAttrib;
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchRecord;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWATTRIB      lpTgtViewAttrib;
   LPTASKDOMAIN      lpSrcTaskDomain;
   LPTASKDOMAIN      lpTgtTaskDomain;
   LPDOMAIN          lpSrcDomain;
   LPDOMAIN          lpTgtDomain;
   zCHAR             cSrcDomainType;
   zCHAR             cTgtDomainType;
   zSHORT            bMustDoDomains;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpSrcViewEntity,
                                        &lpSrcViewEntityCsr,
                                        &lpSrcViewAttrib,
                                        iSetAttributeFromAttribute,
                                        0,   // We do not intend to update
                                        lpSrcView,
                                        cpcSrcEntityName,
                                        cpcSrcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC == 0 )
   {
      // Validate parameters
      nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                           &lpTgtViewEntity,
                                           &lpTgtViewEntityCsr,
                                           &lpTgtViewAttrib,
                                           iSetAttributeFromAttribute,
                                           1,   // We do intend to update
                                           lpTgtView,
                                           cpcTgtEntityName,
                                           cpcTgtAttributeName,
                                           zUSE_TASK ); // Oper id already passed
   }

   if ( nRC )
   {
      fnOperationReturn( iSetAttributeFromAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpSrcDomain = zGETPTR( lpSrcViewAttrib->hDomain );
   lpTgtDomain = zGETPTR( lpTgtViewAttrib->hDomain );

   cTgtDomainType = fnDomainProcessingRequired( &lpTgtTaskDomain,
                                                lpTgtView,
                                                lpCurrentTask,
                                                lpTgtDomain );
   bMustDoDomains = cTgtDomainType ? 1 : 0;

   // Do this just to get the pointer to TaskDomain for Source
   cSrcDomainType = fnDomainProcessingRequired( &lpSrcTaskDomain,
                                                lpSrcView,
                                                lpCurrentTask,
                                                lpSrcDomain );
   if ( bMustDoDomains &&
        lpTgtDomain == lpSrcDomain &&
        cTgtDomainType != zDM_TYPE_TABLE )
   {
      bMustDoDomains = 0;
   }

   // If there is an object operation for the sourced attribute,
   // invoke it now...
   if ( lpSrcViewAttrib->szDerivedOper[ 0 ] )
   {
      nRC = fnInvokeDerivedOperation( lpSrcView, lpSrcViewEntity,
                                      lpSrcViewAttrib,
                                      lpCurrentTask, zDERIVED_GET );
      if ( nRC == zCALL_ERROR )
      {
         fnOperationReturn( iSetAttributeFromAttribute, lpCurrentTask );
         return( nRC );
      }
   }

   // Try calling the target domain operation to set attribute from attribute.
   if ( lpTgtDomain->szDomainOper[ 0 ] )
   {
      nRC = fnCallDomainOper2( lpCurrentTask, lpTgtTaskDomain,
                               zDME_SET_ATTR_FROM_ATTR,
                               lpTgtView,
                               lpTgtViewEntity,
                               lpTgtViewAttrib,
                               lpSrcView,
                               lpSrcViewEntity,
                               lpSrcViewAttrib );

      // If the domain operation handled this call then we're done.
      if ( nRC != zDME_NOT_HANDLED )
      {
         fnOperationReturn( iSetAttributeFromAttribute, lpCurrentTask );
         return( nRC );
      }
   }

   // Try calling the source domain operation to set attribute from attribute.
   if ( lpSrcDomain->szDomainOper[ 0 ] )
   {
      nRC = fnCallDomainOper2( lpCurrentTask, lpSrcTaskDomain,
                               zDME_SET_ATTR_TO_ATTR,
                               lpTgtView,
                               lpTgtViewEntity,
                               lpTgtViewAttrib,
                               lpSrcView,
                               lpSrcViewEntity,
                               lpSrcViewAttrib );

      // If the domain operation handled this call then we're done.
      if ( nRC != zDME_NOT_HANDLED )
      {
         fnOperationReturn( iSetAttributeFromAttribute, lpCurrentTask );
         return( nRC );
      }
   }

   // If domains are involved we will jump through some hoops,
   // otherwise we should try to use the original code, as to eliminate
   // some unnecessary memory transfers.
   if ( bMustDoDomains )
   {
      zULONG   ulReturnSrcLth;

      fnGetActualAttributeLength( &ulReturnSrcLth, lpSrcView,
                                  lpSrcViewEntityCsr, lpSrcViewAttrib );

      fnGetAddrForAttribute( (zCOREMEM) &pchRecord, lpSrcView,
                             lpSrcViewEntityCsr, 0, lpSrcViewAttrib );


      if ( cTgtDomainType == zDM_TYPE_TABLE &&
           lpSrcViewAttrib->hDomain == lpTgtViewAttrib->hDomain )
      {
         lpEntityInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
         nRC = fnStoreValueInEntityInstance( lpEntityInstance,
                                             lpTgtViewEntity,
                                             lpTgtViewAttrib,
                                             (zPVOID) pchRecord, 0);

         fnOperationReturn( iSetAttributeFromAttribute, lpCurrentTask );
         return( nRC );
      }
      else
      {
         // Pass the Source Attribute Domain Name as the Context for the
         // Target Attribute
         nRC = fnSetAttributeFromVariable( lpTgtView, lpTgtViewEntityCsr,
                                           lpTgtViewAttrib,
                                           (zPVOID) pchRecord,
                                           lpSrcViewAttrib->cType,
                                           ulReturnSrcLth,
                                           0, // no context, internal value
                                           lpCurrentTask, 0 );
      }
   }
   else
   {
      // do the stuff below...

      // Get Entity Instance pointer
      lpEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );

      // Get record to retrieve from
      pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpSrcViewAttrib );

      // if a record exists get the attr pointer.
      if ( pchRecord )
      {
         pchRecord += lpSrcViewAttrib->ulRecordOffset;
         if ( lpSrcViewAttrib->cType == zTYPE_STRING &&
              pchRecord[ 0 ] == (zCHAR) '\xff' )
         {
            zCOREMEM  lpExternalP;

            lpExternalP = (zCOREMEM) (pchRecord + 1);
            pchRecord = zGETPTR( *lpExternalP );
         }
      }

      // Now call the appropriate setting routine with the attribute
      switch ( lpSrcViewAttrib->cType )
      {
         case zTYPE_STRING:
            if ( pchRecord == 0 )
               pchRecord = szNullS;

            nRC = SetAttributeFromString( lpTgtView, cpcTgtEntityName,
                                          cpcTgtAttributeName, pchRecord );
            break;

         case zTYPE_INTEGER:
         {
            zPLONG lplLong;

            if ( pchRecord == 0 )
            {
               nRC = SetAttributeFromString( lpTgtView,
                                             cpcTgtEntityName,
                                             cpcTgtAttributeName,
                                             szNullS );
            }
            else
            {
               lplLong = (zPLONG) pchRecord;
               nRC = SetAttributeFromInteger( lpTgtView,
                                             cpcTgtEntityName,
                                             cpcTgtAttributeName,
                                             *lplLong );
            }

            break;
         }

         case zTYPE_DECIMAL:
         {
            zPDECIMAL lpdDecimal;

            if ( pchRecord == 0 )
            {
               nRC = SetAttributeFromString( lpTgtView,
                                             cpcTgtEntityName,
                                             cpcTgtAttributeName,
                                             szNullS );
            }
            else
            {
               lpdDecimal = (zPDECIMAL) pchRecord;
               nRC = SetAttributeFromDecimal( lpTgtView,
                                              cpcTgtEntityName,
                                              cpcTgtAttributeName,
                                              *lpdDecimal );
            }

            break;
         }

         case zTYPE_BLOB:
         {
            zPVOID  pvBlob;
            zULONG  ulLth;

            // Get the length of the source BLOB
            nRC = GetAttributeLength( &ulLth, lpSrcView, cpcSrcEntityName,
                                      cpcSrcAttributeName );
            if ( nRC == 0 && ulLth )
            {
               nRC = fnGetAddrForAttribute( (zCOREMEM) &pvBlob,
                                            lpSrcView,
                                            lpSrcViewEntityCsr, 0,
                                            lpSrcViewAttrib );
               if ( nRC )
               {
                  TraceLineS( "(at) in SAFA fnGetAddrForAttribute",
                              " failed." );
                  break;
               }
               else
                  nRC = SetAttributeFromBlob( lpTgtView,
                                              cpcTgtEntityName,
                                              cpcTgtAttributeName,
                                              pvBlob,
                                              ulLth );

//             TraceLineI( "(at) in SAFA SetAttrFromBlob nRC-", nRC );
            }

            break;
         }

         case zTYPE_DATETIME:
         {
            if ( pchRecord == 0 )
            {
               nRC = SetAttributeFromString( lpTgtView,
                                             cpcTgtEntityName,
                                             cpcTgtAttributeName,
                                             szNullS );
            }
            else
            {
               zCHAR    szDatetime[ 20 ];

               fnGetStringFromAttribute( szDatetime, sizeof( szDatetime ), lpSrcView,
                                         lpSrcViewEntityCsr,
                                         lpSrcViewAttrib, 0 );

               nRC = SetAttributeFromString( lpTgtView, cpcTgtEntityName,
                                             cpcTgtAttributeName, szDatetime );
            }

            break;
         }

         default:
            nRC = zCALL_ERROR;
      }
   }

   fnOperationReturn( iSetAttributeFromAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetBlobAttributeFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//added 9/2/92 by dgc
//
//
//  ENTRY:      SetBlobAttributeFromAttribute
//
//  PURPOSE:    To assign a value to an blob from another attribute
//              regardless of the source attribute type.
//
//  PARAMETERS: lpTgtView           - View containing target blob
//              cpcTgtEntityName    - Entity containing target blob
//              cpcTgtAttributeName - Attribute to be set
//              lpSrcView           - View containing source attribute
//              cpcSrcEntityName    - Entity containing source attribute
//              cpcSrcAttributeName - Attribute used to set target Attr
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
SetBlobAttributeFromAttribute( zVIEW    lpTgtView,
                               zCPCHAR  cpcTgtEntityName,
                               zCPCHAR  cpcTgtAttributeName,
                               zVIEW    lpSrcView,
                               zCPCHAR  cpcSrcEntityName,
                               zCPCHAR  cpcSrcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zPVOID            pvBlob;
   zULONG            ulLth;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetBlobAttributeFromAttribute,
                                        0,   // We do not intend to update
                                        lpSrcView,
                                        cpcSrcEntityName,
                                        cpcSrcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn
   else
   if ( nRC )
      nRC = zCALL_ERROR;
   else
   {
      // Get the length of the source attribute.
      fnGetActualAttributeLength( &ulLth, lpSrcView, lpViewEntityCsr,
                                  lpViewAttrib );
      if ( ulLth )
      {
         nRC = fnGetAddrForAttribute( (zCOREMEM) &pvBlob, lpSrcView,
                                      lpViewEntityCsr, 0, lpViewAttrib );

         nRC = SetAttributeFromBlob( lpTgtView, cpcTgtEntityName,
                                     cpcTgtAttributeName,
                                     pvBlob, ulLth );
      }
   }

   fnOperationReturn( iSetBlobAttributeFromAttribute, lpCurrentTask );
   return( nRC );

}  /* SetBlobAttributeFromAttribute */

//./ ADD NAME=SetMatchingAttributesByName
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetMatchingAttributesByName
//
//  PURPOSE:    To Set all attributes of one entity instance from
//              another entity instance based on attribute name matching
//
//              NOTE: Does not set derived attributes.
//
//  PARAMETERS: lpTgtView        - View containing target entity instance
//              cpcTgtEntityName - Entity name of the target instance
//              lpSrcView        - View containing source entity instance
//              cpcTgtEntityName - Entity name of the source instance
//              nControl         - Defines condition under which the
//                                 target attribute may be set.
//
//                        zSET_NULL    - Set, if the current value of the
//                                       target attribute is null.
//                                       (except KEYs)
//                        zSET_KEYS    - Set target attributes that
//                                       represent KEY's
//                        zSET_NOTNULL - Set, even if current value of the
//                                       target attribute is not null.
//                                       (except KEYs)
//                        zSET_ALL     - Set all target attributes.
//
//                        zSET_SRCNOTNULL - Only set attribute if the src
//                                       value is not null.
//
//  RETURNS:    0           - Attributes successfully set
//              zCALL_ERROR - Error setting attributes
//
//  NOTE:       BLOB attributes are never set with this operation.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetMatchingAttributesByName( zVIEW     lpTgtView,
                             zCPCHAR   cpcTgtEntityName,
                             zVIEW     lpSrcView,
                             zCPCHAR   cpcSrcEntityName,
                             zSHORT    nControl )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWATTRIB      lpSrcViewAttrib;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWATTRIB      lpTgtViewAttrib;
   LPENTITYINSTANCE  lpTgtEntityInstance;
   zSHORT            nRCa;
   zSHORT            nRC;

   // Validate parameters for source attribute.
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpSrcViewEntity,
                                        &lpSrcViewEntityCsr,
                                        &lpSrcViewAttrib,
                                        iSetMatchingAttributesByName,
                                        0,   // We do not intend to update
                                        lpSrcView,
                                        cpcSrcEntityName,
                                        0, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC == 0 )
   {
      // Validate parameters for target attribute.
      nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                           &lpTgtViewEntity,
                                           &lpTgtViewEntityCsr,
                                           &lpTgtViewAttrib,
                                           iSetMatchingAttributesByName,
                                           1,   // We DO intend to update
                                           lpTgtView,
                                           cpcTgtEntityName,
                                           0,
                                           zIGNORE_KEYS |
                                             zUSE_TASK ); // Oper id already passed
   }

   if ( nRC )
   {
      fnOperationReturn( iSetMatchingAttributesByName, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = 0; // Initialize to no errors

   // For each target view attrib, see if there is a matching
   // entity in the source. NOTE, we attempt to pick up the source view
   // attribute from where we left off to optimize attribute setting when
   // the attribute lists are essentially the same and the order is
   // also preserved.
   for ( lpTgtEntityInstance = zGETPTR( lpTgtViewEntityCsr->hEntityInstance );
         lpTgtViewAttrib;
         lpTgtViewAttrib = zGETPTR( lpTgtViewAttrib->hNextOD_Attrib ) )
   {
      // Don't set the attribute if one of the following occurs:
      //    o  Target attribute is hidden.
      //    o  Target attribute does not allow updates.
      //    o  Target attribute is a derived attribute.
      //    o  Target entity was not just created (this means that the entity
      //       has been committed to the database) AND the attribute cannot
      //       be updated once it has been committed.
      //
      // If any of these conditions occur, just continue the loop.
      if ( lpTgtViewAttrib->bHidden            ||
           lpTgtViewAttrib->szDerivedOper[ 0 ] ||
           lpTgtViewAttrib->bNoUpdate          ||
           (lpTgtEntityInstance->u.nInd.bCreated == FALSE &&
            lpTgtViewAttrib->bNoPersistUpd) )
      {
         continue;
      }

      if ( lpTgtViewAttrib->bKey )
      {
         // Attribute is a 'key', see if user wants to copy keys.  If not,
         // continue looping.
         if ( (nControl & zSET_KEYS ) == 0 )
         {
#if 0
            // Test code dks 2008.12.23
            zCHAR szWorkString[ 16 ];

            // Minimum Length for return string is 9, if the attribute
            // is of type DateTime ... maybe -2,000,000,000 would be 11.
            nRCa = fnGetStringFromAttribute( szWorkString, 16, lpTgtView,
                                             lpTgtViewEntityCsr,
                                             lpTgtViewAttrib, 0 );
            if ( nRCa == -1 )
               szWorkString[ 0 ] = 0;
            // End: Test code dks 2008.12.23
#endif
            continue; // Ignore the key.
         }
      }

      // Check to see if user wants to over-write non-null values.
      if ( (nControl & zSET_NOTNULL) == 0 )
      {
         // The user does NOT want attributes in the target entity that
         // already have values (i.e. they are not null) to be over-written
         // with values from the source entity.  Check the target entity's
         // value.  If it is not null, then continue looping.

         if ( lpTgtViewAttrib->cType == zTYPE_BLOB )
         {
            zULONG  uReturnLth;

            fnGetAttributeLength( &uReturnLth,
                                  lpTgtView,
                                  zGETPTR( lpTgtViewEntityCsr->hEntityInstance ),
                                  lpTgtViewAttrib );
            if ( uReturnLth > 0 )
               // Length of blob is > 0, so attribute is NOT null.
               continue;
         }
         else
         {
            zCHAR szWorkString[ 16 ];

            // Minimum Length for return string is 9, if the attribute
            // is of type DateTime ... maybe -2,000,000,000 would be 11.
            nRCa = fnGetStringFromAttribute( szWorkString, 16, lpTgtView,
                                             lpTgtViewEntityCsr,
                                             lpTgtViewAttrib, 0 );

            // If the return code is -1, the attribute is null, so set szWorkString to null.
            if ( nRCa == -1 )
               szWorkString[ 0 ] = 0;

            // If the first char of szWorkString is NOT null, then the target attribute is
            // NOT null, so continue looping.
            if ( szWorkString[ 0 ] )
               continue;
         }
      }

      if ( lpSrcViewEntity == lpTgtViewEntity )
         lpSrcViewAttrib = lpTgtViewAttrib;
      else
      {
         // Try and pick up search from where we left off
         while ( lpSrcViewAttrib )
         {
            if ( zstrcmp( lpSrcViewAttrib->szName,
                          lpTgtViewAttrib->szName ) == 0 )
            {
               break;
            }

            lpSrcViewAttrib = zGETPTR( lpSrcViewAttrib->hNextOD_Attrib );
         }

         // If search failed, restart search from beginning of attrs
         if ( lpSrcViewAttrib == 0 )
         {
            lpSrcViewAttrib = zGETPTR( lpSrcViewEntity->hFirstOD_Attrib );
            while ( lpSrcViewAttrib )
            {
               if ( zstrcmp( lpSrcViewAttrib->szName,
                             lpTgtViewAttrib->szName ) == 0 )
               {
                  break;
               }

               lpSrcViewAttrib = zGETPTR( lpSrcViewAttrib->hNextOD_Attrib );
            }
         }
      }

      // Source attribute cannot be hidden or derived either.
      if ( lpSrcViewAttrib == 0 ||
           lpSrcViewAttrib->bHidden || lpSrcViewAttrib->szDerivedOper[ 0 ] )
      {
         continue;
      }

      if ( nControl & zSET_SRCNOTNULL )
      {
         // User doesn't want NULL source attributes to be copied.
         if ( lpSrcViewAttrib->cType == zTYPE_BLOB )
         {
            zULONG  uReturnLth;

            fnGetAttributeLength( &uReturnLth,
                                  lpSrcView,
                                  zGETPTR( lpSrcViewEntityCsr->hEntityInstance ),
                                  lpSrcViewAttrib );
            if ( uReturnLth == 0 )
               // Length of blob is == 0, so attribute is null.
               continue;
         }
         else
         {

            zCHAR szWorkString[ 9 ];

            // Minimum Length for return string is 9, if the attribute
            // is of type DateTime.
            nRCa = fnGetStringFromAttribute( szWorkString, 9, lpSrcView,
                                             lpSrcViewEntityCsr,
                                             lpSrcViewAttrib, 0 );

            // If the return code is -1, the attribute is null, so set
            // szWorkString to null.
            if ( nRCa == -1 )
               szWorkString[ 0 ] = 0;

            // If first char of szWorkString is null, then the source
            // attribute is null, so continue looping.
            if ( szWorkString[ 0 ] == 0 )
               continue;
         }
      }

      // If we found a matching attribute, call SetAttributeFromAttribute
      // to copy the attribute
      nRCa = SetAttributeFromAttribute( lpTgtView, cpcTgtEntityName,
                                        lpTgtViewAttrib->szName,
                                        lpSrcView, cpcSrcEntityName,
                                        lpSrcViewAttrib->szName );
      if ( nRC == 0 && nRCa )
         nRC = nRCa;

   } // for ( lpViewAttribute... )...

   fnOperationReturn( iSetMatchingAttributesByName, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=AddToAttributeFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      AddToAttributeFromAttribute
//
//  PURPOSE:    To add 2 attributes
//
//  PARAMETERS: lpTgtView         - View containing target attribute
//              cpcTgtEntityName  - Entity containing target attribute
//              cpcTgtAttributeName - Attribute to be added to
//              lpSrcView         - View containing source attribute
//              cpcSrcEntityName  - Entity containing source attribute
//              cpcSrcAttributeName - Attribute used to chg target Attr
//
//  RETURNS:    0           - Source attribute added to target
//              zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
AddToAttributeFromAttribute( zVIEW     lpTgtView,
                             zCPCHAR   cpcTgtEntityName,
                             zCPCHAR   cpcTgtAttributeName,
                             zVIEW     lpSrcView,
                             zCPCHAR   cpcSrcEntityName,
                             zCPCHAR   cpcSrcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWATTRIB      lpSrcViewAttrib;
   zDECIMAL          dSrcDecimal;
   zSHORT            nRC;

   // Validate parameters for source attribute
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpSrcViewEntity,
                                        &lpSrcViewEntityCsr,
                                        &lpSrcViewAttrib,
                                        iAddToAttributeFromAttribute,
                                        0,   // We won't update source
                                        lpSrcView,
                                        cpcSrcEntityName,
                                        cpcSrcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC )
      nRC = zCALL_ERROR;
   else
   {
      // Get the source attribute as a decimal and add it to the target as
      // a decimal value.
      nRC = fnGetDecimalFromAttribute( &dSrcDecimal,
                                       lpSrcView,
                                       lpSrcViewEntityCsr,
                                       lpSrcViewAttrib );
      // If Source attr value is null, skip the Add, but give ok return code.
      if ( nRC == 0 )
      {
         nRC = AddToAttributeFromDecimal( lpTgtView,
                                          cpcTgtEntityName,
                                          cpcTgtAttributeName,
                                          dSrcDecimal );
      }
      else
      if ( nRC == -1 )
         nRC = 0;
   }

   fnOperationReturn( iAddToAttributeFromAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=CompareAttributeToAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CompareAttributeToAttribute
//
//  PURPOSE:    To compare 2 attributes
//
//  PARAMETERS: lpTgtView           - View containing target attribute
//              cpcTgtEntityName    - Entity containing target attribute
//              cpcTgtAttributeName - Attribute to be compared
//              lpSrcView           - View containing source attribute
//              cpcSrcEntityName    - Entity containing source attribute
//              cpcSrcAttributeName - Attribute to be compared
//
//  RETURNS:    1 - Target attr > source attr
//              0 - Attributes are logically equal
//             -1 - Target attr < source attr
//            -16 - Error in call
//
//  NOTE:  Case sensitivity on string attributes is determined by
//         whether the target (first ) attribute is defined as case
//         sensitive
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
CompareAttributeToAttribute( zVIEW    lpTgtView,
                             zCPCHAR  cpcTgtEntityName,
                             zCPCHAR  cpcTgtAttributeName,
                             zVIEW    lpSrcView,
                             zCPCHAR  cpcSrcEntityName,
                             zCPCHAR  cpcSrcAttributeName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpTgtViewEntity;
   LPVIEWENTITYCSR   lpTgtViewEntityCsr;
   LPVIEWATTRIB      lpTgtViewAttrib;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWATTRIB      lpSrcViewAttrib;
   LPTASKDOMAIN      lpSrcTaskDomain;
   LPTASKDOMAIN      lpTgtTaskDomain;
   zCHAR             cSrcDomainType;
   zCHAR             cTgtDomainType;
   zSHORT            bMustDoDomains;
   zPCHAR            pchRecord;
   zSHORT            nRC;

   // Validate parameters for target attribute.
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpTgtViewEntity,
                                        &lpTgtViewEntityCsr,
                                        &lpTgtViewAttrib,
                                        iCompareAttributeToAttribute,
                                        0,   // We do not intend to update
                                        lpTgtView,
                                        cpcTgtEntityName,
                                        cpcTgtAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC == 0 )
   {
      // Validate parameters for source attribute.
      nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                           &lpSrcViewEntity,
                                           &lpSrcViewEntityCsr,
                                           &lpSrcViewAttrib,
                                           iCompareAttributeToAttribute,
                                           0,   // We do not intend to update
                                           lpSrcView,
                                           cpcSrcEntityName,
                                           cpcSrcAttributeName,
                                           zUSE_TASK ); // Oper id already passed
   }

   if ( nRC )
   {
      fnOperationReturn( iCompareAttributeToAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Invoke derived operation for Src.
   if ( lpSrcViewAttrib->szDerivedOper[ 0 ] )
      fnInvokeDerivedOperation( lpSrcView, lpSrcViewEntity,
                                lpSrcViewAttrib, lpCurrentTask,
                                zDERIVED_GET );

   cTgtDomainType = fnDomainProcessingRequired( &lpTgtTaskDomain,
                                                lpTgtView,
                                                lpCurrentTask,
                                                zGETPTR( lpTgtViewAttrib->hDomain ) );

   bMustDoDomains = cTgtDomainType ? 1 : 0;

   // Just to get the SrcTaskDomain pointer.
   cSrcDomainType = fnDomainProcessingRequired( &lpSrcTaskDomain,
                                                lpSrcView, lpCurrentTask,
                                                zGETPTR( lpSrcViewAttrib->hDomain ) );

   if ( bMustDoDomains &&
        lpTgtViewAttrib->hDomain == lpSrcViewAttrib->hDomain )
   {
      bMustDoDomains = 0;
   }

   if ( bMustDoDomains )
   {
      zPCHAR   pchContextName;
      zULONG   ulReturnSrcLth;
      LPDOMAIN lpSrcDomain;

      // We will not use the Domain Name as a Context Name if the target is a
      // format domain or if only one of the source/target domains is a table.
      if ( (cTgtDomainType == zDM_TYPE_FORMAT) ||
           (cTgtDomainType == zDM_TYPE_TABLE && cSrcDomainType != zDM_TYPE_TABLE) ||
           (cSrcDomainType == zDM_TYPE_TABLE && cTgtDomainType != zDM_TYPE_TABLE) )
      {
         pchContextName = 0;
      }
      else
      if ( lpSrcTaskDomain )
      {
         LPDOMAIN lpDomain = zGETPTR( lpSrcTaskDomain->hDomain );

         pchContextName = lpDomain->szName;
      }
      else
         pchContextName = 0;

      fnGetActualAttributeLength( &ulReturnSrcLth, lpSrcView,
                                  lpSrcViewEntityCsr, lpSrcViewAttrib );

      fnGetAddrForAttribute( (zCOREMEM) &pchRecord, lpSrcView,
                             lpSrcViewEntityCsr, 0, lpSrcViewAttrib );

      // If both the source and target domains are tables, we will get the
      // value of the source as an external value and pass it to the target
      // domain.
      if ( cSrcDomainType == zDM_TYPE_TABLE &&
           cTgtDomainType == zDM_TYPE_TABLE )
      {
         zLONG lTempValue;
         zCHAR szTemp[ 254 ];

         lpSrcDomain = zGETPTR( lpSrcViewAttrib->hDomain );
         if ( lpSrcDomain->cType == zTYPE_DECIMAL )
         {
            SysConvertDecimalToLong( (zPDECIMAL) pchRecord,
                                     &lTempValue );
            TableEntryIntToExt( (zPVOID) szTemp, lpSrcView,
                                lpSrcDomain,
                                pchContextName,
                                &lTempValue );
         }
         else
         {
            TableEntryIntToExt( (zPVOID) szTemp, lpSrcView,
                                lpSrcDomain,
                                pchContextName,
                                pchRecord );
         }

         nRC = fnCompareAttributeToVariable( lpTgtView,
                                             lpTgtViewEntityCsr,
                                             lpTgtViewAttrib,
                                             (zPVOID) szTemp,
                                             zTYPE_STRING,
                                             ulReturnSrcLth,  // not really used
                                             pchContextName,
                                             lpCurrentTask, 0 );
      }
      else
      {
         nRC = fnCompareAttributeToVariable( lpTgtView,
                                             lpTgtViewEntityCsr,
                                             lpTgtViewAttrib,
                                             pchRecord,
                                             lpSrcViewAttrib->cType,
                                             ulReturnSrcLth,
                                             pchContextName,
                                             lpCurrentTask, 0 );
      }
   }
   else
   {
      // do the stuff below...

      /* Determine the type of compare to do based on the internal types
         of the Attributes.  If either type is a Numeric type ( int, zLONG,
         decimal ) then a numeric compare is done. Else if either type is
         a date, then an internal date numeric compare is done. Otherwise
         a string compare is done.                                     */

      //
      // invoke derived operation for Tgt
      //
      if ( lpTgtViewAttrib->szDerivedOper[ 0 ] )
         fnInvokeDerivedOperation( lpTgtView, lpTgtViewEntity,
                                   lpTgtViewAttrib, lpCurrentTask,
                                   zDERIVED_GET );

      nRC = zCALL_ERROR;   // Default nRC to zCALL_ERROR

      if ( lpTgtViewAttrib->cType == zTYPE_INTEGER &&
           lpSrcViewAttrib->cType == zTYPE_INTEGER )
      {
         zPLONG plTarget;
         zPLONG plSource;

         fnGetAddrForAttribute( (zCOREMEM) &plSource, lpSrcView,
                                lpSrcViewEntityCsr, 0, lpSrcViewAttrib );
         fnGetAddrForAttribute( (zCOREMEM) &plTarget, lpTgtView,
                                lpTgtViewEntityCsr, 0, lpTgtViewAttrib );
         if ( *plSource == *plTarget )
            nRC = 0;
         else
         if ( *plTarget == lNullInteger )
            nRC = -1;
         else
         if ( *plSource == lNullInteger || *plSource < *plTarget )
            nRC = 1;
         else
            nRC = -1;
      }
      else
      // If either of the attrs are decimals we will compare them both as
      // decimals
      if ( lpTgtViewAttrib->cType == zTYPE_DECIMAL ||
           lpSrcViewAttrib->cType == zTYPE_DECIMAL )
      {
         zDECIMAL dTgtCompare;
         zDECIMAL dSrcCompare;
         zSHORT   nRC1, nRC2;

         nRC1 = fnGetDecimalFromAttribute( &dTgtCompare,
                                           lpTgtView,
                                           lpTgtViewEntityCsr,
                                           lpTgtViewAttrib );

         nRC2 = fnGetDecimalFromAttribute( &dSrcCompare,
                                           lpSrcView,
                                           lpSrcViewEntityCsr,
                                           lpSrcViewAttrib );
         if ( nRC1 == zCALL_ERROR || nRC2 == zCALL_ERROR )
            nRC = zCALL_ERROR;
         else
         {
            // If we get here then nRCx must be either -1 (attr is null) or
            // 0 (not-null).
            if ( nRC1 == -1 && nRC2 == -1 )
               nRC = 0;    // Both attrs are null so return EQUAL.
            else
            if ( nRC1 == 0 && nRC2 == 0 )
               // Both values are not null so call func to compare 'em.
               nRC = SysCompareDecimalToDecimal( &dTgtCompare,
                                                 &dSrcCompare );
            else
            {
               // Only one of the values is null.  Find which one it is and
               // set the return code accordingly (null < non-null).
               if ( nRC1 == -1 )
                  nRC = -1;  // Target is null so it's "less than".
               else
                  nRC = 1;   // Source is null so it's "less than".
            }
         }
      }
      else
      if ( lpTgtViewAttrib->cType == zTYPE_BLOB &&
           lpSrcViewAttrib->cType == zTYPE_BLOB )
      {
         //compare blobs
         zULONG  ulTgtBlobLength;
         zULONG  ulSrcBlobLength;

         fnGetAttributeLength( &ulTgtBlobLength, lpTgtView,
                               zGETPTR( lpTgtViewEntityCsr->hEntityInstance ),
                               lpTgtViewAttrib );
         fnGetAttributeLength( &ulSrcBlobLength, lpSrcView,
                               zGETPTR( lpSrcViewEntityCsr->hEntityInstance ),
                               lpSrcViewAttrib );
         if ( ulTgtBlobLength != ulSrcBlobLength )
            nRC = ( ulTgtBlobLength < ulSrcBlobLength ) ? -1 : 1;
         else
         {
            if ( ulTgtBlobLength )
            {
               zPVOID TgtBlobAddr;
               zPVOID SrcBlobAddr;

               fnGetAddrForAttribute( (zCOREMEM) &TgtBlobAddr, lpTgtView,
                                      lpTgtViewEntityCsr, 0, lpTgtViewAttrib );
               fnGetAddrForAttribute( (zCOREMEM) &SrcBlobAddr, lpSrcView,
                                      lpSrcViewEntityCsr, 0, lpSrcViewAttrib );
               nRC = zmemcmp( TgtBlobAddr, SrcBlobAddr,
                              (unsigned int) ulTgtBlobLength );
            }
            else
               nRC = 0;
         }
      }
      else   // We need some sort of date compare here
      if ( lpTgtViewAttrib->cType == zTYPE_DATETIME &&
           lpSrcViewAttrib->cType == zTYPE_DATETIME )
      {
         LPDTINTERNAL lpSrcDateTime;
         LPDTINTERNAL lpTgtDateTime;

         fnGetAddrForAttribute( (zCOREMEM) &lpSrcDateTime, lpSrcView,
                                lpSrcViewEntityCsr, 0, lpSrcViewAttrib );
         fnGetAddrForAttribute( (zCOREMEM) &lpTgtDateTime, lpTgtView,
                                lpTgtViewEntityCsr, 0, lpTgtViewAttrib );
         nRC = fnCompareDateTimeToDateTime( lpTgtDateTime, lpSrcDateTime );
      }
      else   // Compare the strings
      if ( lpTgtViewAttrib->cType == zTYPE_STRING &&
           lpSrcViewAttrib->cType == zTYPE_STRING )
      {
         zPCHAR pchSrcString;
         zPCHAR pchTgtString;

         fnGetAddrForAttribute( (zCOREMEM) &pchSrcString, lpSrcView,
                                lpSrcViewEntityCsr, 0, lpSrcViewAttrib );
         fnGetAddrForAttribute( (zCOREMEM) &pchTgtString, lpTgtView,
                                lpTgtViewEntityCsr, 0, lpTgtViewAttrib );
         if ( lpTgtViewAttrib->bCaseSens )
            nRC = zstrcmp( pchTgtString, pchSrcString );
         else
            nRC = zstrcmpi( pchTgtString, pchSrcString );

         if ( nRC < 0 )
            nRC = -1;
         else
         if ( nRC > 0 )
            nRC = 1;
      }
      else
      {
         // all else has failed (mixed internal types),
         // we will compare by 'converting' both type to strings
         // NOTE: with mixed type we assume that the first 64K
         //       will be enough to tell.  This will work for all
         //       cases except a "BLOB to STRING" compare where
         //       one of these would be truncated AND the first
         //       64K are the same.  In the future, we can check
         //       for this special case and do dynamic allocations.
         //
         zCHAR  szTgtCompare[ 65535 ];
         zCHAR  szSrcCompare[ 65535 ];
         nRC = fnGetStringFromAttribute( szTgtCompare, 65535,
                                         lpTgtView,
                                         lpTgtViewEntityCsr,
                                         lpTgtViewAttrib, 0 );
         if ( nRC == 0 || nRC == -1 )
         {
            nRC = fnGetStringFromAttribute( szSrcCompare, 65535,
                                            lpSrcView,
                                            lpSrcViewEntityCsr,
                                            lpSrcViewAttrib, 0 );
            if ( nRC == 0 || nRC == -1 )
            {
               if ( lpTgtViewAttrib->bCaseSens )
                  nRC = zstrcmp( szTgtCompare, szSrcCompare );
               else
                  nRC = zstrcmpi( szTgtCompare, szSrcCompare );

               if ( nRC < 0 )
                  nRC = -1;
               else
               if ( nRC > 0 )
                  nRC = 1;
            }
         }
      }
   }

   fnOperationReturn( iCompareAttributeToAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=CompareAttributeToString
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CompareAttributeToString
//
//  PURPOSE:    To compare an attribute to a string
//
//  PARAMETERS: lpView           - View containing attribute to be compared
//              cpcEntityName    - Entity containing attribute to be compared
//              cpcAttributeName - Attribute to be compared
//              cpcSrcString     - String to compare to
//
//  NOTE:  Case sensitivity on string attributes is determined by
//         whether the Attribute itself is defined as case sensitive
//
//  RETURNS:    1 - Attr > string
//              0 - Attributes are logically equal
//             -1 - Attr < string
//            -16 - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
CompareAttributeToString( zVIEW    lpView,
                          zCPCHAR  cpcEntityName,
                          zCPCHAR  cpcAttributeName,
                          zCPCHAR  cpcSrcString )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters for source attribute
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iCompareAttributeToString,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC )
      nRC = zCALL_ERROR;
   else
      nRC = fnCompareAttributeToVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                          (zPVOID) cpcSrcString, zTYPE_STRING,
                                          0, 0, lpCurrentTask, 0 );

   fnOperationReturn( iCompareAttributeToString, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=CompareAttributeToInteger
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CompareAttributeToInteger
//
//  PURPOSE:    To compare an attribute to an Integer
//
//  PARAMETERS: lpView        - View containing attribute to be compared
//              cpcEntityName  - Entity containing attribute to be compared
//              cpcAttributeName - Attribute to be compared
//              lSrcInteger   - Integer (zLONG) to compare to
//
//  RETURNS:    1 - Attr > integer
//              0 - Attributes are logically equal
//             -1 - Attr < integer
//            -16 - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
CompareAttributeToInteger( zVIEW   lpView,
                           zCPCHAR cpcEntityName,
                           zCPCHAR cpcAttributeName,
                           zLONG   lSrcInteger )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters for source attribute
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iCompareAttributeToInteger,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC )
      nRC = zCALL_ERROR;
   else
      nRC = fnCompareAttributeToVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                          (zPVOID) &lSrcInteger, zTYPE_INTEGER,
                                          0, 0, lpCurrentTask, 0 );

   fnOperationReturn( iCompareAttributeToInteger, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=CompareAttributeToDecimal
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CompareAttributeToDecimal
//
//  PURPOSE:    To compare an attribute to a Decimal
//
//  PARAMETERS: lpView           - View containing attribute to be compared
//              cpcEntityName    - Entity containing attribute to be compared
//              cpcAttributeName - Attribute to be compared
//              dSrcDecimal      - Decimal (double) to compare to
//
//  RETURNS:    1 - Attr > decimal
//              0 - Attributes are logically equal
//             -1 - Attr < decimal
//            -16 - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
CompareAttributeToDecimal( zVIEW     lpView,
                           zCPCHAR   cpcEntityName,
                           zCPCHAR   cpcAttributeName,
                           zDECIMAL  dSrcDecimal )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters for source attribute
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iCompareAttributeToDecimal,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC )
      nRC = zCALL_ERROR;
   else
      nRC = fnCompareAttributeToVariable( lpView, lpViewEntityCsr, lpViewAttrib,
                                          (zPVOID) &dSrcDecimal, zTYPE_DECIMAL,
                                          0, 0, lpCurrentTask, 0 );

   fnOperationReturn( iCompareAttributeToDecimal, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetTaskDomain
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetTaskDomain
//
//  RETURNS:    0 - domain not found
//             otw - domain pointer
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPTASKDOMAIN
fnGetTaskDomain( zVIEW lpView, LPTASK lpCurrentTask, LPDOMAIN lpDomain )
{
   LPTASK         hCurrentTask = zGETHNDL( lpCurrentTask );
   LPLIBRARY      hLibrary = 0;
   LPTASKDOMAIN   lpTaskDomain;
   zPVOID         hTaskDomain;
   zCHAR          cDomainType;
   zLONG          lProcessID = SysGetProcessID( 0 );

   if ( lpDomain == 0 )  // no domain pointer ...
      return( 0 );       //   return not found!

   lpTaskDomain = zGETPTR( lpDomain->hFirstTaskDomain );
   while ( lpTaskDomain )
   {
      if ( lpTaskDomain->hTask == hCurrentTask &&
           lpTaskDomain->lProcessID == lProcessID )
      {
         return( lpTaskDomain );
      }

      lpTaskDomain = zGETPTR( lpTaskDomain->hNextTaskDomainForDomain );
   }

   cDomainType = lpDomain->cDomainType;

   switch ( cDomainType )
   {
      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
         if ( lpDomain->szLibraryName[ 0 ] == 0 )
            return( 0 );

         hLibrary = SysLoadLibrary( lpView, lpDomain->szLibraryName );
         if ( hLibrary == 0 )
            return( 0 );

         // Yes, we should fall through.

      case zDM_TYPE_TABLE:
      case zDM_TYPE_FORMAT:
      {
         // If a table Domain has an operation, make sure the library routine
         // is properly accessed, the same as for Algorithm above.
         if ( cDomainType == zDM_TYPE_TABLE &&
              lpDomain->szDomainOper[ 0 ] && lpDomain->szLibraryName[ 0 ] )
         {
            hLibrary = SysLoadLibrary( lpView, lpDomain->szLibraryName );
         }

         hTaskDomain = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                         sizeof( TaskDomainRecord ), 1, 0,
                                         iTaskDomain );
         if ( hTaskDomain == 0 )
            return( 0 );

         lpTaskDomain = zGETPTR( hTaskDomain );

         lpTaskDomain->hDomain    = zGETHNDL( lpDomain );
         lpTaskDomain->hTask      = hCurrentTask;
         lpTaskDomain->lProcessID = lProcessID;
         lpTaskDomain->hLibrary   = hLibrary;

         // Set pointers for TaskDomain chain off of Task structure.
         // Note: this is a single linked list.
         if ( hCurrentTask == AnchorBlock->hMainTask )
            zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

         lpTaskDomain->hNextTaskDomainForTask = lpCurrentTask->hFirstTaskDomain;
         lpCurrentTask->hFirstTaskDomain = hTaskDomain;

         if ( hCurrentTask == AnchorBlock->hMainTask )
            zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
#if 0
         // debugging only dks 2005.12.01
         if ( lpTaskDomain->hTask == AnchorBlock->hMainTask )
         {
            LPAPP lpApp = zGETPTR( lpDomain->hApp );
            if ( zstricmp( lpApp->szName, "ZENCAS" ) == 0 )
               TraceLineS( "", "" );
         }
#endif
         // Set pointers for TaskDomain chain off of Domain structure.
         // Note: this is a double linked list.
         if ( lpDomain->hLastTaskDomain )
         {
            LPTASKDOMAIN lpLastTaskDomain;

            lpLastTaskDomain = zGETPTR( lpDomain->hLastTaskDomain );
            lpLastTaskDomain->hNextTaskDomainForDomain = hTaskDomain;
            lpTaskDomain->hPrevTaskDomainForDomain =
                                          lpDomain->hLastTaskDomain;
            lpDomain->hLastTaskDomain = hTaskDomain;
         }
         else
         {
            lpDomain->hFirstTaskDomain = hTaskDomain;
            lpDomain->hLastTaskDomain  = hTaskDomain;
         }

         if ( hLibrary )
         {
         // LPLIBRARY lpLibrary = zGETPTR( hLibrary );
         // TraceLine( "fnGetTaskDomain Task: 0x%08x   Domain: 0x%08x   Lib: %s",
         //            lpView->hTask, lpTaskDomain, lpLibrary->szName );
            if ( lpDomain->szDomainOper[ 0 ] )
            {
               lpTaskDomain->pfDomainOper =
                              (zPDMOPER) SysGetProc( hLibrary,
                                                     lpDomain->szDomainOper );
            }
         }
      }
   }

   return( lpTaskDomain );
}

zSHORT LOCALOPER
fnRetrieveAttribName( zPPCHAR        ppchInput,
                      zPCHAR         pchAttribName,
                      zPCHAR         pchContextName )
{
   zSHORT n;
   zPCHAR pchExpr = *ppchInput;

   for ( n = 0; zisalnum( *pchExpr ); pchExpr++ )
      pchAttribName[ n++ ] = *pchExpr;

   pchAttribName[ n ] = 0;

   // Skip whitespaces.
   while ( zisspace( *pchExpr ) )
      pchExpr++;

   // Check to see if the character immediately following the attribute name
   // is a [ to indicate that a context is to be used for the get of the attribute.
   if ( *pchExpr == '[' )
   {
      pchExpr++;

      // Skip whitespaces.
      while ( zisspace( *pchExpr ) )
         pchExpr++;

      // Retrieve the context name.  We allow any non-space characters to
      // be part of the context name.
      for ( n = 0;
            *pchExpr && zisspace( *pchExpr ) == FALSE && *pchExpr != ']';
            pchExpr++ )
      {
         pchContextName[ n++ ] = *pchExpr;
      }

      pchContextName[ n ] = 0;

      // Skip whitespaces.
      while ( zisspace( *pchExpr ) )
         pchExpr++;

      if ( *pchExpr == ']' )
         pchExpr++;
      else
         return( zCALL_ERROR );
   }
   else
     *pchContextName = 0;  // No context.

   *ppchInput = pchExpr;

   return( 0 );
}

// Internal domain operation.  This one concatinates attributes.
zSHORT LOCALOPER
DfExprVML( zVIEW        lpView,
           LPVIEWENTITY lpViewEntity,
           LPVIEWATTRIB lpViewAttrib,
           zSHORT       nFlag )
{
   zCHAR        szConcatBuffer[ 2000 ];
   zCHAR        szContextName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR        szAttribName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR        chSign;
   zLONG        idx;
   zBOOL        bInQuote;
   zLONG        lIntValue;
   zDECIMAL     dDecValue;
   zPCHAR       pchExpr = zGETPTR( lpViewAttrib->hDerivedExpr );
   LPVIEWATTRIB lpSrcViewAttrib;

   // We'll never set this attribute.
   if ( nFlag == zDERIVED_SET )
      return( 0 );

   // Initialize each of the temp storage areas.
   szConcatBuffer[ 0 ] = 0;
   bInQuote            = FALSE;
   szContextName[ 0 ]  = 0;
   lIntValue           = 0;
   SysConvertLongToDecimal( 0, &dDecValue );
   chSign              = 0;

   // Loop through the expression and copy values to the concat buffer.
   // Anything in quotes is copied verbatim.  Anything not in quotes had
   // better be an attribute name or + signs.
   // 'idx' points to the null-terminator in szConcatBuffer.
   for ( idx = 0; *pchExpr; pchExpr++ )
   {
      // Check to see if we are inside a quoted string.  If so we just copy
      // the character from the expression to the buffer.
      if ( bInQuote )
      {
         if ( *pchExpr == '\"' )
         {
            bInQuote = FALSE;

            // We'll terminate the buffer here.
            szConcatBuffer[ idx ] = 0;
         }
         else
            szConcatBuffer[ idx++ ] = *pchExpr;

         continue;
      }

      // Check for plus or minus sign.
      if ( *pchExpr == '+' || *pchExpr == '-' )
      {
         chSign = *pchExpr;
         continue;
      }

      // OK, we're not in a quote or context name.  Check to see if we are
      // starting a quoted section.
      if ( *pchExpr == '\"' )
      {
         bInQuote = TRUE;

         // Increment idx so that it points to the null-terminator.  We do
         // this because it's possible that we've added an attribute value
         // before now and we want idx to point to the end of the string.
         while ( szConcatBuffer[ idx ] )
            idx++;

         // We're starting a new quoted string.  We'll assume that adding
         // attributes will start from here.
         lIntValue = 0;
         SysConvertLongToDecimal( 0, &dDecValue );
         continue;
      }

      // We'll ignore non-alphanumerics.  We can ignore the '+' sign because
      // we'll assume string concatenation/addition.
      if ( zisalnum( *pchExpr ) == FALSE )
         continue;

      // At this point pchExpr had better be pointing to an attribute name.
      // Retrieve the name.
      if ( fnRetrieveAttribName( &pchExpr, szAttribName, szContextName ) != 0 )
         return( zCALL_ERROR );

      pchExpr--;  // Go back one to the space.
      lpSrcViewAttrib = fnValidViewAttrib( lpView, lpViewEntity, szAttribName, 0 );
      if ( lpSrcViewAttrib == 0 )
         return( zCALL_ERROR );

      // The source attribute type determines how we are going to add a value
      // to the buffer.
      switch ( lpSrcViewAttrib->cType )
      {
         case zTYPE_DATETIME:
         case zTYPE_STRING:
            // We'll just concatenate the attribute's value to the buffer.
            if ( szContextName[ 0 ] )
               GetStringFromAttributeByContext( szConcatBuffer + idx, lpView,
                                                lpViewEntity->szName,
                                                lpSrcViewAttrib->szName,
                                                szContextName, 1999 - idx );
            else
               GetStringFromAttribute( szConcatBuffer + idx, sizeof( szConcatBuffer ) - idx, lpView,
                                       lpViewEntity->szName, szAttribName );

            // Increment idx so that it points to the null-terminator.
            idx += zstrlen( &szConcatBuffer[ idx ] );
            break;

         case zTYPE_DECIMAL:
         {
            zDECIMAL dNewValue;

            // Get the attribute's value as a decimal and add it to the
            // "current" value.
            if ( szContextName[ 0 ] )
               szContextName[ 0 ] = 0;  // ignore context on decimals for now

            GetDecimalFromAttribute( &dNewValue, lpView,
                                     lpViewEntity->szName, szAttribName );

            if ( chSign == '-' )
               SysSubtractDecimalFromDecimal( &dDecValue, &dDecValue, &dNewValue );
            else
               SysAddDecimalToDecimal( &dDecValue, &dDecValue, &dNewValue );

            // Add the decimal value to the end of the string.
            SysConvertDecimalToString( &dDecValue, &szConcatBuffer[ idx ], 3 );

            // Convert the value to the temp long variable so that if the next
            // token is a long it will added correctly.
            SysConvertDecimalToLong( &dDecValue, &lIntValue );

            // Note that we *don't* increment idx.  This way if the next token
            // is another decimal attribute we can add the two decimal values
            // together and overwrite the value we just concatenated to the buffer.
            break;
         }

         case zTYPE_INTEGER:
         {
            zLONG lNewValue;

            // Get the attribute's value as an integer and add it to the
            // "current" value.
            if ( szContextName[ 0 ] )
               szContextName[ 0 ] = 0;  // ignore context on integers for now

            GetIntegerFromAttribute( &lNewValue, lpView,
                                     lpViewEntity->szName, szAttribName );
            if ( chSign == '-' )
               lIntValue -= lNewValue;
            else
               lIntValue += lNewValue;

            // Add the integer value to the end of the string.
            zltoa( lIntValue, szConcatBuffer + idx, sizeof( szConcatBuffer ) - idx );

            // Convert the value to the temp decimal variable so that if the next
            // token is a decimal it will added correctly.
            SysConvertLongToDecimal( lIntValue, &dDecValue );

            // Note that we *don't* increment idx.  This way if the next token
            // is another integer attribute we can add the two integer values
            // together and overwrite the value we just concatenated to the buffer.

            break;
         }
      }

      if ( pchExpr[ 0 ] == 0 )
         break;

   } // for ( idx... )

   StoreStringInRecord( lpView, lpViewEntity, lpViewAttrib,
                        szConcatBuffer );
   return( 0 );
}

typedef zLONG (POPERATION PFNINTERP)
            ( zVIEW  lpInterpSubtask,       // Interpretor's subtask
              zVIEW  lpAppSubtask,          // application's subtask
              zPCHAR pchSrcDLLName,         // name of the dialog
              zPCHAR pchOperationName,      // operation to interp
              zPLONG plRetVal );

zSHORT LOCALOPER
DfExecuteXPG( zVIEW        lpView,
              LPVIEWENTITY lpViewEntity,
              LPVIEWATTRIB lpViewAttrib,
              zSHORT       nFlag )
{
   LPAPP  lpApp;
   zVIEW  vSubtask;
   zVIEW  vAppSubtask;
   zVIEW  vXPG;
   zVIEW  vStackObject;
   zVIEW  vOldStackObject;
   LPTASK lpCurrentTask;
   zCHAR  szOperName[] = "DerivedFunction";
   zULONG ulLth;
   zPCHAR pchData;
   zPCHAR pchObjectName;
   zLONG  lReturn;
   zBOOL  bDropSubtask;
   zSHORT nRC;

   static PFNINTERP pfnInterp = 0;

   // We'll never set this attribute.
   if ( nFlag == zDERIVED_SET )
      return( 0 );

   lpCurrentTask = zGETPTR( lpView->hTask );
// lpCurrentTask = fnFindTask( 0 );  // dks 2006.05.07  back to 10B

   // Set up interpreter.
   // Create the appl subtask.
   fnGetApplicationForSubtask( &lpApp, lpView );
   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   vAppSubtask = fnCreateSubtask( lpCurrentTask, lpApp );
   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   // It's possible that we are being called recursively.  This will happen if
   // a derived expression references another attrib with a derived expr.  We
   // need to make sure that when we leave this function the OIs needed for the
   // parent derived expr are still in place.
   GetViewByName( &vSubtask, "ZeidonVML", lpView, zLEVEL_TASK );
   if ( vSubtask == 0 )
   {
      // No parent derived expr.  Do normal init processing.
      bDropSubtask = TRUE;
      SfCreateSubtask( &vSubtask, lpView, "Zeidon_Tools" );

      // name the ZeidonVML subtask at the task level.
      SetNameForView( vSubtask, "ZeidonVML", lpView, zLEVEL_TASK );
   }
   else
   {
      // We must be in the process of being called recursively.  Don't create
      // a new subtask and save the current stack OI.
      bDropSubtask = FALSE;

      // Save the current stack object OI.
      GetViewByName( &vOldStackObject, "StackObject", vSubtask, zLEVEL_SUBTASK );
   }

   // Get a pointer to the begining of the OI data.  For OIs stored as blobs
   // there is some header information that we don't need, so skip over it.
   pchData = (zPCHAR) zGETPTR( lpViewAttrib->hDerivedXPG );
   ulLth  = fnGetDataspaceLength( pchData );
   pchData += sizeof( BlobOI_Record );
   ulLth  -= sizeof( BlobOI_Record );

   // "Activate" the XPG from the binary data.
   if ( GetViewByName( &vXPG, "LastXPG", vSubtask, zLEVEL_TASK ) < 0 )
      nRC = -1;
   else
   {
      LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

      pchObjectName = lpViewOD->szName;
      if ( CompareAttributeToString( vXPG, "VML_XSource",
                                     "MetaName", pchObjectName ) == 0 )
      {
         nRC = 0;
      }
      else
      {
         nRC = -1;
      }
   }

   if ( nRC < 0 )
   {
      nRC = fnActivateOI_FromBuffer( &vXPG, "TZVSPOOO", vSubtask,
                                     pchData, ulLth, zSINGLE );
      if ( nRC < 0 )
      {
         SysMessageBox( lpView, "Internal Core Error", "Error reading OI from buffer", 1 );
         return( zCALL_ERROR );
      }

      SetNameForView( vXPG, "LastXPG", vSubtask, zLEVEL_TASK );
   }

   SetNameForView( vXPG, "XPG", vSubtask, zLEVEL_SUBTASK );

   // Initialize the stack object with arguments for the operation.
   ActivateEmptyObjectInstance( &vStackObject, "TZVSRTOO", vSubtask, zSINGLE );
   CreateEntity( vStackObject, "StackObject", zPOS_AFTER );
   CreateEntity( vStackObject, "OperationCall", zPOS_AFTER );
   SetAttributeFromString( vStackObject, "OperationCall", "Name", szOperName );

   CreateEntity( vStackObject, "Variable", zPOS_AFTER );
   SetAttributeFromInteger( vStackObject, "Variable", "ID", 1 );
   SetAttributeFromString( vStackObject, "Variable", "Name",
                           "vSubtask" );

   CreateEntity( vStackObject, "Variable", zPOS_AFTER );
   SetAttributeFromInteger( vStackObject, "Variable", "ID", 2 );
   SetAttributeFromString( vStackObject, "Variable", "Name",
                           "vView" );
   SetAttributeFromBlob( vStackObject, "Variable", "Value",
                         (zPVOID) &lpView, sizeof( lpView ) );

   // Name the Stack Object
   SetNameForView( vStackObject, "StackObject", vSubtask, zLEVEL_SUBTASK );

   if ( pfnInterp == 0 )
   {
      // Load the library.  We'll let core clean it up for us later.
      LPLIBRARY hLibrary = SysLoadLibrary( vSubtask, "TZVMLOPR" );

      if ( hLibrary == 0 )
      {
         SysMessageBox( lpView, "Zeidon Interpretor Error",
                        "Could not locate TZVMLOPR.DLL", TRUE );
      }
      else
         pfnInterp = (PFNINTERP) SysGetProc( hLibrary, "InvokeInterp" );
   }

   if ( pfnInterp )
      (*pfnInterp)( vSubtask, vAppSubtask, "", szOperName, &lReturn );

   // Drop views.  Note that vXPG has already been dropped by InvokeInterp( ).
   fnDropView( vStackObject );
   SfDropSubtask( vAppSubtask, 0 );
   if ( bDropSubtask )
      SfDropSubtask( vSubtask, 0 );
   else
      SetNameForView( vOldStackObject, "StackObject", vSubtask, zLEVEL_SUBTASK );

   return( 0 );
}

//./ ADD NAME=fnGetAttribOper
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:    fnGetAttribOper
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zPOBJOPER
fnGetAttribOper( zVIEW        lpView,
                 LPVIEWATTRIB lpViewAttrib,
                 LPTASK       lpCurrentTask )
{
   LPTASK      hCurrentTask = zGETHNDL( lpCurrentTask );
   LPVIEWOD    lpViewOD;
   zPCHAR      pchOperLibName;
   LPLIBRARY   hLibrary;
   zPVOID      hTaskOperation;
   LPTASKOPER  lpTaskOperation;
   zBOOL       bMutexLocked;
   zLONG       lProcessID = SysGetProcessID( 0 );

   // If there is no derived operation pointer, return 0 ==> not found.
   if ( lpViewAttrib->szDerivedOper[ 0 ] == 0 )
      return( 0 );

   lpViewOD = zGETPTR( lpView->hViewOD );

   for ( lpTaskOperation = zGETPTR( lpViewAttrib->hFirstTaskOperation );
         lpTaskOperation;
         lpTaskOperation = zGETPTR( lpTaskOperation->hNextTaskOperationForOEA ) )
   {
      if ( lpTaskOperation->hTask == hCurrentTask &&
           lpTaskOperation->lProcessID == lProcessID )
      {
         return( (zPOBJOPER) lpTaskOperation->pfnTaskOper );
      }
   }

   if ( lpViewAttrib->bDerivedIntrnl )
      hLibrary = 0;
   else
   {
      // No task object oper exists yet, create one if possible.
      pchOperLibName = lpViewOD->szOperLibname;
      if ( pchOperLibName == 0 || pchOperLibName[ 0 ] == 0 )
         return( 0 );

      hLibrary = SysLoadLibrary( lpView, pchOperLibName );  // dks 2006.04.06  back to 10b
      if ( hLibrary == 0 )
         return( 0 );
   }

   hTaskOperation = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                      sizeof( TaskOperationRecord ), 1, 0,
                                      iTaskOperation );
   if ( hTaskOperation == 0 )
      return( 0 );

   lpTaskOperation = zGETPTR( hTaskOperation );
   lpTaskOperation->hViewAttrib = zGETHNDL( lpViewAttrib );
   lpTaskOperation->hTask       = hCurrentTask;
   lpTaskOperation->lProcessID  = lProcessID;

   // Set pointers for TaskOperation chain off of Task structure.
   // Note: this is a single linked list.
   if ( hCurrentTask == AnchorBlock->hMainTask )
   {
      bMutexLocked = TRUE;
      zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
   }
   else
      bMutexLocked = FALSE;

   lpTaskOperation->hNextTaskOperationForTask =
                                          lpCurrentTask->hFirstTaskOperation;
   lpCurrentTask->hFirstTaskOperation = hTaskOperation;

   if ( bMutexLocked )
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( lpViewAttrib->bDerivedIntrnl )
   {
      if ( zstrcmp( lpViewAttrib->szDerivedOper, "DfExprVML" ) == 0 )
         lpTaskOperation->pfnTaskOper = (zPVOID) DfExprVML;
      else
      if ( zstrcmp( lpViewAttrib->szDerivedOper, "*xpg*" ) == 0 )
         lpTaskOperation->pfnTaskOper = (zPVOID) DfExecuteXPG;
   }
   else
   {
      lpTaskOperation->pfnTaskOper = SysGetProc( hLibrary,
                                                 lpViewAttrib->szDerivedOper );
   }

   // Set pointers for TaskOperation chain off of ViewAttrib structure
   // Note: this is a double linked list
   if ( lpViewAttrib->hLastTaskOperation )
   {
      LPTASKOPER  lpTempOperation;

      lpTempOperation = zGETPTR( lpViewAttrib->hLastTaskOperation );
      lpTempOperation->hNextTaskOperationForOEA = hTaskOperation;
      lpTaskOperation->hPrevTaskOperationForOEA =
                                            lpViewAttrib->hLastTaskOperation;
      lpViewAttrib->hLastTaskOperation = hTaskOperation;
   }
   else
   {
      lpViewAttrib->hFirstTaskOperation = hTaskOperation;
      lpViewAttrib->hLastTaskOperation  = hTaskOperation;
   }

   return( (zPOBJOPER) lpTaskOperation->pfnTaskOper );
}

/////////////////////////////////////////////////////////////////////////////
//
//   ENTRY:    sFormatIn
//
//   PURPOSE:  Validate an Input string format. This function takes in a
//             formatted character string and validates the format of
//             the string using the format mask provided.
//             The characters 9,A,a, X, and * in an input
//             format string represent numeric, alphabatic,
//             alphabetic upper, alphanumeric and ANY character to be
//             validated in the input string.  All other characters
//             in the format string ( '(', ' ', ')','-', etc ) must
//             match the input String.
//
//             For example the String passed with a format mask of:
//
//                 '( 99AaX )' must contain a '(' in the first byte
//                 followed by a space, a digit 0-9, an uppercase
//                 alphabetic, an upper or lower case alphabetic, an
//                 alphanumeric, a trailing space and a final ')'.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
sFormatIn( zPCHAR pchString, zPCHAR pchFormat, zPCHAR pchEdit )
{
   short i, j, k, l;

   i = 0;
   j = 0;
   while ( pchString[ j ] )
   {
      switch ( pchFormat[ i ] )
      {
         case  0:
            return( 1 );

         case  '9':
            if ( isdigit( pchString[ j ] ) == FALSE )
               return( 1 );

            break;

         case  'A':
            if ( isupper( pchString[ j ] ) == FALSE )
               return( 1 );

            break;

         case  'a':
            if ( isalpha( pchString[ j ] ) == FALSE )
               return( 1 );

            break;

         case  'X':
            if ( zisalnum( pchString[ j ] ) == FALSE )
               return( 1 );

            break;

         case  '*':
            break;

         case  '\\':
            j--;
            break;

         case  ' ':
            if ( pchString[ j ] == ' ' )
            {
               k = j + 1;
               while( pchString[ k ] == ' ' )
                  k++;

               l = j;
               for ( ; ; )
               {
                  pchString[ l++ ] = pchString[ k ];
                  if ( pchString[ k ] == 0 )
                     break;

                  k++;
               }

               break;
            }
            else
               return( 1 );

         default:
            if ( pchFormat[ i ] != pchString[ j ] )
               return( 1 );

            k = j;
            for ( ; ; )
            {
               pchString[ k ] = pchString[ k + 1 ];
               if ( pchString[ k ] == 0 )
                  break;

               k++;
            }

            j--;
        }

        i++;
        j++;
    }

    if ( pchFormat[ i ] )
       return( 1 );

    return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    sFormatOut
//
// PURPOSE:  Format an output string. This function takes in an
//           unformatted character string and formats the string
//           by adding characters as defined in the format string
//           passed.  For example to format a telephone number the
//           string paramater should contain the telephone digits
//           (e.g. 6172718822) and the format string could contain
//           '(999) 999-9999'.  The output string would be
//           '(617) 271-8822'.  The characters 9,A,a,X and * in a
//           format string represent numeric, alphabatic,
//           alphabetic upper, and alphanumeric characters to be
//           mapped as-is from the input string.  All other characters
//           in the format string ( '(', ' ', ')','-', etc ) are
//           inserted into the string at the defined position.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
sFormatOut( zPCHAR pchString, zPCHAR pchFormat )
{
   zSHORT i, j, k;

   i = 0;
   j = 0;

   if ( pchString[ 0 ] == 0 )
      return( 0 );

   while ( pchFormat[ i ] )
   {
      switch ( pchFormat[ i ] )
      {
         case  '9':
         case  'A':
         case  'a':
         case  'X':
         case  '*':
            if ( pchString[ j ] == 0 )
               return( 1 );

            break;

         default:
            if ( pchFormat[ i ] == '\\' && pchFormat[ i + 1 ] )
               i++;

            k = j;
            while ( pchString[ k ] )
               k++;

            while ( k >= j )
            {
               pchString[ k + 1 ] = pchString[ k ];
               k--;
            }

            pchString[ j ] = pchFormat[ i ];
      }

      i++;
      j++;
   }

   return( 0 );
}

//./ ADD NAME=fnValidateAttributeParameters
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnValidateAttributeParameters
//
//  PURPOSE:    To validate the standard parameters for an
//              attribute call
//
//               0 - Attribute parameters valid
//              -1 - Error validating the attribute parameters
//              -2 - EntityInstance is null (only set when the
//                   zACCEPT_NULL_ENTITY flag set in nFlag).
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 12
zSHORT
fnValidateAttributeParameters( LPTASK          *lpReturnTask,
                               LPVIEWENTITY    *pvReturnViewEntity,
                               LPVIEWENTITYCSR *pvReturnViewEntityCsr,
                               LPVIEWATTRIB    *pvReturnViewAttrib,
                               zSHORT          iOperationID,
                               zSHORT          bUpdate,  // Set Operations
                               zVIEW           lpView,
                               zCPCHAR         cpcEntityName,
                               zCPCHAR         cpcAttributeName,
                               zSHORT          nFlag )
{
   LPTASK            lpCurrentTask;
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpVsnInstance;

   if ( nFlag & zUSE_TASK ) // Oper id already passed
   {
      lpCurrentTask = *lpReturnTask;
      if ( fnValidViewCsr( lpCurrentTask, lpView ) == 0 )
         return( -1 );
   }
   else
   {
      // If task not active or disabled, or view csr invalid, return 1.
      if ( (lpCurrentTask = fnOperationCall( iOperationID, lpView,
                                             zVALID_VIEW_CSR )) == 0 )
      {
         return( 1 );  // no need to call fnOperationReturn
      }

      *lpReturnTask = lpCurrentTask;
   }

   // Get the view Object Instance for the view.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      return( -1 );
   }

   // Set View Entity and View Entity Cursor for caller.
   *pvReturnViewEntity     = lpViewEntity;
   *pvReturnViewEntityCsr  = lpViewEntityCsr;

   // Validate that the attribute exists.
   if ( cpcAttributeName == 0 )
   {
      lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
      if ( lpViewAttrib == 0 )
         return( -1 );
   }
   else
   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      return( -1 );
   }

   // If intent is update, fail the call unless the attribute is a work attr.
   if ( bUpdate && lpView->bReadOnly && lpViewAttrib->bPersist &&
        lpViewEntity->bDerivedPath == FALSE )
   {
      // "KZOEE119 - Invalid View, View is Read Only"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 119, 0,
                        lpViewEntity->szName,
                        lpViewAttrib->szName );
   // fnOperationReturn( iOperationID, lpCurrentTask );
      return( -1 );
   }

   // Set return view attrib, NOTE that this parameter is optional
   // because the validation done by this function is also used by other
   // operations such as IncludeSubobjectFromSubobject.
   if ( pvReturnViewAttrib )
   {
      // Set attribute pointer for caller
      *pvReturnViewAttrib = lpViewAttrib;
   }

   // Get Entity Instance pointer.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // This for loop is here so we can retry to establish an undefined
   // cursor when retrieving attribute information!
   for ( ; ; )
   {
      if ( lpEntityInstance == UNSET_CSR )
         lpEntityInstance = fnEstablishCursorForView( lpViewEntityCsr );

      // Now make sure a cursor exists for the view entity.
      if ( lpEntityInstance == 0 )
      {
         // Make sure parent to NULL cursor is defined.
         while ( lpViewEntityCsr && lpViewEntityCsr->hEntityInstance == 0 )
            lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );

         if ( lpViewEntityCsr )
         {
            lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
            if ( lpEntityInstance == UNSET_CSR )
               lpEntityInstance = fnEstablishCursorForView( lpViewEntityCsr );
         }
         else
            lpEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );

         if ( lpEntityInstance == 0 ||
// DGC 12/09/98
// I have no idea why we check for a hidden entity but DON'T CHANGE IT!.
              lpEntityInstance->u.nInd.bHidden == FALSE )
         {
            if ( nFlag & zACCEPT_NULL_ENTITY )
               return( -2 );
            else
            {
               //  "KZOEE253 - Entity cursor is NULL"
               fnIssueCoreError( lpCurrentTask, lpView, 8, 253, 0,
                                 lpViewEntity->szName,
                                 lpViewAttrib->szName );
            // fnOperationReturn( iOperationID, lpCurrentTask );
               return( -1 );
            }
         }
      }

      // If the user wants to update the attribute but the attribute is part of
      // a hanging entity return an error.
      if ( bUpdate && lpEntityInstance->u.nInd.bHangingEntity )
      {
         //  "KZOEE237 - Attempt to update a non-updateable attribute"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 237, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ),
                           lpViewAttrib->szName );
      // fnOperationReturn( iOperationID, lpCurrentTask );
         return( -1 );
      }

      if ( lpEntityInstance->u.nInd.bHidden )
      {
         // If we are attempting to update something with an undefined
         // cursor, or the highest level cursor in the view is undefined,
         // issue an undefined error.
         if ( bUpdate || lpViewEntityCsr == 0 )
         {
            //  "KZOEE254 - Entity cursor is undefined"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 254, 0,
                              lpViewEntity->szName,
                              lpViewAttrib->szName );
         // fnOperationReturn( iOperationID, lpCurrentTask );
            return( -1 );
         }
         else
         {
            // We have found an undefined cursor, try and establish the
            // cursor on the next or previous entity at the highest level.
            while ( lpEntityInstance->hNextTwin )
            {
               lpEntityInstance = zGETPTR( lpEntityInstance->hNextTwin );
               if ( lpEntityInstance->u.nInd.bHidden == FALSE )
                  break;
            }

            if ( lpEntityInstance->u.nInd.bHidden )
            {
               while ( lpEntityInstance->hPrevTwin )
               {
                  lpEntityInstance = zGETPTR( lpEntityInstance->hPrevTwin );
                  if ( lpEntityInstance->u.nInd.bHidden == FALSE )
                     break;
               }
            }

            // If we found a cursor we can establish, try and re-establish
            // it before moving on.
            if ( lpEntityInstance->u.nInd.bHidden == FALSE )
            {
               LPENTITYINSTANCE lpTempEI;

               lpViewEntityCsr->hEntityInstance = zGETHNDL( lpEntityInstance );
            // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "ValidateAttribute", "UNSET_CSR", -1 );

               fnResetCursorForViewChildren( lpViewEntityCsr );
               lpViewEntityCsr = *pvReturnViewEntityCsr;
               lpTempEI = zGETPTR( lpViewEntityCsr->hEntityInstance );
               if ( lpTempEI == 0 || lpTempEI == UNSET_CSR )
               {
                  if ( nFlag & zACCEPT_NULL_ENTITY )
                     return( -2 );
                  else
                  {
                     // "KZOEE254 - Entity cursor is undefined"
                     fnIssueCoreError( lpCurrentTask, lpView, 8, 254, 0,
                                       lpViewEntity->szName,
                                       lpViewAttrib->szName );
                  // fnOperationReturn( iOperationID, lpCurrentTask );
                     return( -1 );
                  }
               }
            }
            else
            {
               if ( nFlag & zACCEPT_NULL_ENTITY )
                  return( -2 );
               else
               {
                  //  "KZOEE254 - Entity cursor is undefined"
                  fnIssueCoreError( lpCurrentTask, lpView, 8, 254, 0,
                                    lpViewEntity->szName,
                                    lpViewAttrib->szName );
               // fnOperationReturn( iOperationID, lpCurrentTask );
                  return( -1 );
               }
            }
         }
      }
      else
         break;
   }

   if ( bUpdate )
   {
      // If the entity doesn't have update authority then fail the check
      // unless the attribute is a work attribute.
      if ( lpViewEntity->bUpdate == FALSE && lpViewAttrib->bPersist )
      {
         // "KZOEE106 - Rules violation"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 106, 0,
                           "Entity Update not allowed",
                           lpViewEntity->szName );
      // fnOperationReturn( iOperationID, lpCurrentTask );
         return( -1 );
      }

      if ( lpEntityInstance->u.nInd.bPrevVersion )
      {
         // We've found an entity instance which may be a previous
         // version. Check that it is a non-updateable record version
         // by finding the record pointer for the newer version
         lpVsnInstance = lpEntityInstance;
         if ( lpEntityInstance->hNextVsn == 0 )
         {
            lpVsnInstance = zGETPTR( lpEntityInstance->hNextLinked );
            if ( lpVsnInstance )
            {
               // Look for a linked instance which is versioned and has
               // a record pointer which is different then the entity's
               // record pointer ( meaning the record has update auth. ).
               for ( ; ; )
               {
                  if ( lpVsnInstance == lpEntityInstance )
                  {
                     lpVsnInstance = 0;
                     break;
                  }

                  if ( lpVsnInstance->hNextVsn )
                  {
                     LPENTITYINSTANCE lpNextVsn =
                                          zGETPTR( lpVsnInstance->hNextVsn );

                     if ( lpNextVsn->hPersistRecord != lpEntityInstance->hPersistRecord )
                        break;
                  }

                  lpVsnInstance = zGETPTR( lpVsnInstance->hNextLinked );
               }
            }
         }

         if ( lpVsnInstance )
         {
            //  "KZOEE255 - Attempt to update a previous entity version"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 255, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
         // fnOperationReturn( iOperationID, lpCurrentTask );
            return( -1 );
         }
      }

      // Check to make sure attributes are updateable. If cpcAttributeName is
      // non-null then we only need to check 1 attribute; otherwise we need
      // to check all the attributes.
      for ( ;
            lpViewAttrib;
            lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         // Check to see if we should ignore key attributes.
         if ( (nFlag & zIGNORE_KEYS) && lpViewAttrib->bKey )
         {
            if ( cpcAttributeName )
               break;  // We only needed to check 1 attr so break 'for' loop.
            else
               continue;
         }

         if ( lpViewAttrib->bNoUpdate )
         {
            //  "KZOEE237 - Attempt to update a non-updateable attribute"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 237, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
         // fnOperationReturn( iOperationID, lpCurrentTask );
            return( -1 );
         }

         if ( lpViewAttrib->bNoPersistUpd &&
              lpEntityInstance->u.nInd.bCreated == FALSE )
         {
            //  "KZOEE238 - Attempt to update a non-updateable persistent "
            //  "           attribute"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 237, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
         // fnOperationReturn( iOperationID, lpCurrentTask );
            return( -1 );
         }

         // We only need to check 1 attribute if cpcAttributeName is non-null.
         if ( cpcAttributeName )
            break;

      } // for...

   } // if ( bUpdate )

   // Everything is AOK, return
   return( 0 );
}

//./ ADD NAME=fnCreateAttributeRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCreateAttributeRecord
//
//  PURPOSE:    To allocate and create an attribute record
//
//  RETURNS:    pchRecord - allocated record.  RETURNED AS A HANDLE!!!
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zPVOID
fnCreateAttributeRecord( LPTASK lpAllocTask,
                         LPVIEWENTITY lpViewEntity,
                         LPENTITYINSTANCE lpEntityInstance )
{
   zPCHAR         pchRecord;
   zPCHAR         pchChar;
   zPVOID         hRecord;
   zPLONG         lplLong;
   zPDECIMAL      lpdDecimal;
   LPDTINTERNAL   lpDateTime;
   LPVIEWATTRIB   lpViewAttrib;
   zULONG         ulBytes;

   if ( lpEntityInstance )
      ulBytes = lpViewEntity->ulRecordSize;
   else
      ulBytes = lpViewEntity->ulNonPersistSize;

   hRecord = fnAllocDataspace( lpAllocTask->hFirstDataHeader, ulBytes, 1, 0,
                               iAttributeRecord );
   if ( hRecord == 0 )
      return( 0 );

   pchRecord = zGETPTR( hRecord );

   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      if ( (lpEntityInstance && lpViewAttrib->bPersist) ||
           (lpEntityInstance == 0 && lpViewAttrib->bPersist == FALSE) )
      {
         if ( lpViewAttrib->hInit )
         {
            // Do the initialization thing
         }
         else
         {
            switch ( lpViewAttrib->cType )
            {
               case 'L':
                  lplLong = (zLONG *) (pchRecord + lpViewAttrib->ulRecordOffset);
                  *lplLong = lNullInteger;
                  break;

               case zTYPE_DECIMAL:
                  lpdDecimal = (zDECIMAL *) (pchRecord + lpViewAttrib->ulRecordOffset);
                  SysAssignDecimalFromNull( lpdDecimal );
                  break;

               case 'T':
                  lpDateTime =
                    (LPDTINTERNAL) (pchRecord + lpViewAttrib->ulRecordOffset);
                  lpDateTime->ulDateMinutes = lNullInteger;
                  lpDateTime->usTSeconds = 0;
                  break;

               case 'B':
                  pchChar = (zCHAR *) (pchRecord + lpViewAttrib->ulRecordOffset);
                  *pchChar = '\x00';
                  break;

               default:
                  break;
            }
         }
      }
   }

   // Now that the record has been created, put the record into the Entity
   // instance created and any linked instances associated with the entity.
   if ( lpEntityInstance )
   {
      LPENTITYINSTANCE lpLinkedEntityInstance;

      // Set the record pointer in the entity instance and copy the record
      // pointer to all linked versions of the entity instance.  If no entity
      // instance is marked as the record owner (should be load situation
      // only!), then mark the current instance as the record owner.
      lpEntityInstance->hPersistRecord = hRecord;
      if ( lpEntityInstance->hNextLinked )
      {
         lpLinkedEntityInstance = zGETPTR( lpEntityInstance->hNextLinked );
         while ( lpLinkedEntityInstance != lpEntityInstance )
         {
            lpLinkedEntityInstance->hPersistRecord = hRecord;
            lpLinkedEntityInstance =
                            zGETPTR( lpLinkedEntityInstance->hNextLinked );
         }
      }
   }

   return( hRecord );
}

//./ ADD NAME=fnStoreBlobInRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnStoreBlobInRecord
//
//  PURPOSE:    To store a blob in a record
//
//  RETURNS:    0 - Attribute value has not changed
//              1 - Attribute value has changed
//             -1 - Error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnStoreBlobInRecord( LPTASK lpAllocTask,
                     zPCHAR pchRecord,
                     zULONG ulOffset,
                     zPVOID pBlob,
                     zULONG ulBlobLth )
{
   zPULONG     pulLth;
   zCOREMEM    lpExternalP;
   zPCHAR      pchExternal;

   lpExternalP = (zCOREMEM) ((zPCHAR) pchRecord + ulOffset + 1);
   pulLth = (zPULONG) (lpExternalP + 1);
   if ( pchRecord[ ulOffset ] == (zCHAR) '\xff' )
   {
      zULONG ulFreespaceLth;

      pchExternal  = zGETPTR( *lpExternalP );
      ulFreespaceLth = fnGetDataspaceLength( pchExternal );

      // See if this space is big enough to hold the new blob.
      if ( ulFreespaceLth < ulBlobLth || ulBlobLth == 0 )
      {
         *pulLth = 0;
         fnFreeDataspace( pchExternal );
      }
      else
      {
         *pulLth = ulBlobLth;
      }
   }
   else
   {
      // A blob does not exist, set *pulLth to 0 so memory allocation
      // takes place below if the Blob length is non-zero.
      *pulLth = 0;
   }

   // Get out if there is no data to store.
   if ( ulBlobLth == 0 )
   {
      pchRecord[ ulOffset ] = 0;
      *lpExternalP = 0;
      return( 1 );
   }

   // Now check lpuShort to see if we must allocate a new space.
   if ( *pulLth == 0 )
   {
      zPVOID hndl;

      hndl = fnAllocDataspace( lpAllocTask->hFirstDataHeader,
                               ulBlobLth, 0, 0, iBlob );
      pchExternal = zGETPTR( hndl );
      if ( pchExternal == 0 )
      {
         pchRecord[ ulOffset ] = 0;
         *lpExternalP = 0;
         return( -1 );
      }

      pchRecord[ ulOffset ] = (zCHAR) '\xff';
      *lpExternalP = hndl;
      *pulLth = ulBlobLth;
   }

   // Copy the Blob data
   if ( pBlob )
      zmemcpy( pchExternal, pBlob, ulBlobLth );
   else
      zmemset( pchExternal, 0, *pulLth );  // null out memory

   return( 1 );   // return that new blob has been stored
}

//./ ADD NAME=fnStoreStringInRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnStoreStringInRecord
//
//  PURPOSE:    To store a string in a record
//
//  RETURNS:    0 - Attribute value has not changed
//              1 - Attribute value has changed
//    zCALL_ERROR - Error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT
fnStoreStringInRecord( LPTASK  lpAllocTask,
                       zPCHAR  pchRecord,
                       zULONG  ulOffset,
                       zCPCHAR cpcString,
                       zULONG  ulLth,
                       zSHORT  bNullTerminated )
{
   zULONG     uCmprLth;
   zPULONG    pulLth;
   zCOREMEM   lpExternalP;
   zPCHAR     pchExternal;

   // If the length is 0, get out fast.
   if ( ulLth == 0 && bNullTerminated == FALSE )
      return( zCALL_ERROR );

   // Initialize pchExternal to 0
   pchExternal = 0;

   // If data is to null terminated, add one for the null.
   uCmprLth = ulLth;
   if ( bNullTerminated )
      ulLth++;

   // Check length to see if it is too big for record.
   if ( cpcString[ 0 ] == (zCHAR) '\xff' ||
        ulLth > g_ulMaxRecordStringLth ||
        bNullTerminated == FALSE )
   {
      zULONG ulFreespaceLth;

      lpExternalP = (zCOREMEM) ((zPCHAR) pchRecord + ulOffset + 1);
      pulLth = (zPULONG) (lpExternalP + 1);
      if ( pchRecord[ ulOffset ] == (zCHAR) '\xff' )
      {
         pchExternal  = zGETPTR( *lpExternalP );
         ulFreespaceLth = fnGetDataspaceLength( pchExternal );

         // See if this space is big enough to hold the new length.
         if ( ulFreespaceLth < ulLth )
         {
            fnFreeDataspace( pchExternal );
            *pulLth = 0;
         }
         else
         {
            if ( *pulLth == ulLth &&
                 zstrcmp( pchExternal, cpcString ) == 0 )
            {
               return( 0 );  // return 0, data has not changed
            }

            *pulLth = ulLth;
         }
      }
      else
         *pulLth = 0;

      // Now check pulLth to see if we must allocate a new space.
      if ( *pulLth == 0 )
      {
         zPVOID hndl;

         hndl = fnAllocDataspace( lpAllocTask->hFirstDataHeader, ulLth, 0, 0,
                                  iAttributeString );
         pchExternal = zGETPTR( hndl );
         if ( pchExternal == 0 )
         {
            pchRecord[ ulOffset ] = 0;
            return( zCALL_ERROR );
         }

         pchRecord[ ulOffset ] = (zCHAR) '\xff';
         *lpExternalP = hndl;
      }

      // Set length and copy data.
      zmemcpy( pchExternal, cpcString, (unsigned int) ulLth );
      *pulLth = ulLth;

      // Add null if requested.
      if ( bNullTerminated )
         pchExternal[ ulLth - 1 ] = 0;
   }
   else
   {
      if ( pchRecord[ ulOffset ] == (zCHAR) '\xff' )
      {
         lpExternalP = (zCOREMEM) ((zPCHAR) pchRecord + ulOffset + 1);
         pchExternal = zGETPTR( *lpExternalP );
         fnFreeDataspace( pchExternal );
      }
      else
      if ( zstrcmp( pchRecord + ulOffset, cpcString ) == 0 )
         return( 0 ); // Return 0 if string has not changed

      zmemcpy( pchRecord + ulOffset, cpcString, (unsigned int) ulLth );

      // Ensure null if data is null terminated.
      if ( bNullTerminated )
         pchRecord[ ulOffset + (ulLth - 1) ] = 0;
   }

   return( 1 ); // return that new string has been stored
}

//./ ADD NAME=fnCopyAttributeRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCopyAttributeRecord
//
//  PURPOSE:    To copy a persistent Attribute record in memory
//
//  RETURNS:    zPCHAR - Address of a new record AS A HANDLE!!!!.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zPCHAR
fnCopyAttributeRecord( LPTASK       lpAllocTask,
                       LPVIEWENTITY lpSrcViewEntity,
                       zCPCHAR      cpcSrcRecord,
                       zBOOL        bPersist )
{
   zULONG         ulBytes;
   zPCHAR         pchNewRecord;
   zPVOID         hNewRecord;
   LPVIEWATTRIB   lpViewAttrib;
   zPCHAR         pchRecordStringS;
   zPCHAR         pchRecordStringT;
   zPCHAR         pchNewExtString;
   zCOREMEM       lpExternalPS;
   zCOREMEM       lpExternalPT;
   zPULONG        pulLth;

   // If record pointer is null, return null.
   if ( cpcSrcRecord == 0 )
      return( 0 );

   // Get size of record to allocate
   if ( bPersist )
      ulBytes = lpSrcViewEntity->ulRecordSize;
   else
      ulBytes = lpSrcViewEntity->ulNonPersistSize;

   hNewRecord = fnAllocDataspace( lpAllocTask->hFirstDataHeader,
                                  ulBytes, 0, 0, iAttributeRecord );
   pchNewRecord = zGETPTR( hNewRecord );
   if ( pchNewRecord == 0 )
      return( 0 );

   // Copy the old record to the new record.  This will cause all non
   // blob/string attributes to be copied.
   zmemcpy( pchNewRecord, cpcSrcRecord, ulBytes );

   // Now that a new record has been created, go through all
   // the attributes and copy any external strings.
   for ( lpViewAttrib = zGETPTR( lpSrcViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      if ( !lpViewAttrib->bPersist == bPersist )
         continue;

      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_BLOB:
            pchRecordStringS = (zPCHAR)
                        (cpcSrcRecord + lpViewAttrib->ulRecordOffset);
            if ( pchRecordStringS[ 0 ] == (zCHAR) '\xff' )
            {
               zPVOID hndl;

               pchRecordStringT = (zPCHAR)
                        (pchNewRecord + lpViewAttrib->ulRecordOffset);
               lpExternalPS = (zCOREMEM) (pchRecordStringS + 1);
               lpExternalPT = (zCOREMEM) (pchRecordStringT + 1);
               pulLth = (zPULONG) (lpExternalPS + 1);
               hndl = fnAllocDataspace( lpAllocTask->hFirstDataHeader,
                                        *pulLth, 0, 0, iAttributeString );
               *lpExternalPT = hndl;
               pchNewExtString = zGETPTR( hndl );
               if ( *lpExternalPT )
               {
                  zmemcpy( pchNewExtString,
                           zGETPTR( *lpExternalPS ), (unsigned int) *pulLth );
               }
            }

            break;

         default:
            break;
      }
   }

   // return the new record created
   return( hNewRecord );
}

//./ ADD NAME=fnDeleteAttributeRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnDeleteAttributeRecord
//
//  PURPOSE:    To delete a record from memory
//
//  RETURNS:    0 - Attribute record was deleted
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnDeleteAttributeRecord( LPVIEWENTITY     lpViewEntity,
                         LPENTITYINSTANCE lpEntityInstance,
                         zBOOL            bPersist )
{
   LPVIEWATTRIB lpViewAttrib;
   zPCHAR       pchRecord;

   if ( bPersist )
   {
      // Drop the record *UNLESS* it has a previous version and the entity
      // is not updatable.  If it's not updatable then we never create
      // another record for temporal entities so skip dropping this one.
      // It will be dropped when the original EI is dropped.
      if ( lpEntityInstance->hPrevVsn && lpViewEntity->bUpdate == FALSE )
         return( 0 );

      pchRecord = zGETPTR( lpEntityInstance->hPersistRecord );  // dks 2006.09.18 error while cleaning up client OE
   }
   else
      pchRecord = zGETPTR( lpEntityInstance->hNonPersistRecord );

   if ( pchRecord == 0 )
      return( 0 ); // skip if record is null

   // Get first attribute.
   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      zPCHAR    pchRecordString;
      zCOREMEM  lpExternalP;

      if ( (lpViewAttrib->cType == 'S' || lpViewAttrib->cType == 'B') &&
           (lpViewAttrib->bPersist == bPersist) )
      {
         pchRecordString = pchRecord + lpViewAttrib->ulRecordOffset;
         if ( pchRecordString[ 0 ] == (zCHAR) '\xff' ) // suballoc'd?
         {
            pchRecordString[ 0 ] = (zCHAR) '\x00'; // clear suballoc flag
            lpExternalP = (zCOREMEM) (pchRecordString + 1);
            pchRecordString = zGETPTR( *lpExternalP );
            fnFreeDataspace( pchRecordString ); // free associated area
            zmemset( lpExternalP, 0, 6 ); // clear ptr and lth
         }
      }
   }

   // Now free the record itself.
   fnFreeDataspace( pchRecord );

   if ( bPersist )
      lpEntityInstance->hPersistRecord = 0;
   else
      lpEntityInstance->hNonPersistRecord = 0;

   return( 0 );  // return success
}

//./ ADD NAME=ConvertDecimalToString
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   ConvertDecimalToString
//
//  PURPOSE: To convert a decimal (double) to a string.  Intended use
//           is primarily for Domain functions.
//
//  PARAMETERS:
//           pchReturnString - Area to return string.
//           lpViewAttrib    - Attribute Struct or 0.  Used for truncation.
//           dDecimal        - Decimal number to convert.
//
//  PURPOSE: ConvertDecimalToString converts a decimal to a null
//           terminated string based on the Attribute definition
//           passed.
//
//           ConvertDecimalToString converts the double
//           into 2 zLONG values and then calls zltoa to convert
//           the zLONG values to ASCII. Then the decimal point is
//           placed at the appropriate position in the string and
//           the decimal portion is truncated based on the Attribute
//           definition ( if supplied ).  If the attribute
//           type is a string or an attribute definition is not
//           supplied, the string is truncated based on the
//           number of digits maintained in the mantissa in a
//           double type.
//
//  RETURNS: The string value for a Decimal
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zVOID OPERATION
ConvertDecimalToString( zPCHAR       pchReturnString,
                        LPVIEWATTRIB lpViewAttrib,
                        zDECIMAL     dDecimal,
                        zCPCHAR      cpcContextName )
{
   zSHORT      nPrecision;
   zLPCONTEXT  lpContext;

   nPrecision = -1;
   if ( lpViewAttrib )
   {
      if ( GetContext( &lpContext, zGETPTR( lpViewAttrib->hDomain ),
                       cpcContextName ) )
         nPrecision = (zSHORT) lpContext->lDecimalFormat;
      else
      if ( lpViewAttrib->cType == zTYPE_DECIMAL )
      {
         zCHAR szDecimal[ 2 ];

         szDecimal[ 0 ] = lpViewAttrib->cDecimal;
         szDecimal[ 1 ] = 0;
         nPrecision = (zSHORT) zatol( szDecimal );
      }
      else
      if ( lpViewAttrib->cType == zTYPE_INTEGER )
         nPrecision = 0;
   }

   SysConvertDecimalToString( &dDecimal, pchReturnString, nPrecision );

}  /* End of fnConvertDecimalToString */

//./ ADD NAME=SetAttributeFromVariable
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SetAttributeFromVariable
//
// PURPOSE:  To assign a value to an attribute using a varying type of
//           variable value.
//
// PARAMETERS:
//           lpView          - View to Object instance
//
//           cpcEntityName    - Entity name containing the attribute to set
//
//           cpcAttributeName - Attribute name to set.
//
//           lpVariable      - pointer to the variable used in setting
//                             the attribute.
//
//           cVariableType   - Zeidon data type, indicates what
//                             lpVariable points to.
//
//           ulVariableLth   - Indicates length of variable that is
//                             pointed to by lpVariable.
//                             Only used when cVariableType is
//                             zTYPE_STRING or zTYPE_BLOB.
//
//           cpcContextName   - Optional context name, used in conjunction
//                             with the domain associated with the
//                             specified attribute.
//
//           nFlag           - zVALIDATE indicates that this operation
//                             will only validate the input (lpVariable)
//                             data and not change any attribute value.
//                             The return code would be as on a normal
//                             SetAttributeFromVariable call.
//
//                             zUSE_DEFAULT_CONTEXT indicates that the
//                             default Context of the Domain associated
//                             with the attribute will be used in process-
//                             ing this request.  Any value specified by
//                             the cpcContextName parameter is iqnored.
//
// RETURNS: 0           - Attribute successfully set
//          zCALL_ERROR - error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zSHORT OPERATION
SetAttributeFromVariable( zVIEW     lpView,
                          zCPCHAR   cpcEntityName,
                          zCPCHAR   cpcAttributeName,
                          zCPVOID   lpVariable,
                          zCHAR     cVariableType,
                          zULONG    ulVariableLth,
                          zCPCHAR   cpcContextName,
                          zSHORT    nFlag )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zLPCONTEXT        lpDefaultContext;
   zSHORT            nRC;

// if ( zstrcmp( cpcEntityName, "QMsg" ) == 0 && cVariableType == 'B' )
//    TraceLineS( "", "" );

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetAttributeFromVariable,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC )
      nRC = zCALL_ERROR;
   else
   {
      if ( nFlag & zUSE_DEFAULT_CONTEXT )
      {
         if ( GetDefaultContext( &lpDefaultContext,
                                 zGETPTR( lpViewAttrib->hDomain ) ) )
         {
            cpcContextName = lpDefaultContext->szName;
         }
         else
         {
            // "KZOEE232 - Operation indicates use default context,
            //  none found"
            fnIssueCoreError( lpCurrentTask, lpView,
                              (zSHORT) (lpViewAttrib->hDomain ? 16 : 8),
                              232, 0L, lpViewEntity->szName,
                              lpViewAttrib->szName );
            fnOperationReturn( iSetAttributeFromVariable, lpCurrentTask );
            return( zCALL_ERROR );
         }
      }

      // Call fnSetAttributeFromVariable to do the setting
      nRC = fnSetAttributeFromVariable( lpView, lpViewEntityCsr,
                                        lpViewAttrib, lpVariable, cVariableType,
                                        ulVariableLth,
                                        cpcContextName,
                                        lpCurrentTask, nFlag );
   }

   fnOperationReturn( iSetAttributeFromVariable, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeFromVariable
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnSetAttributeFromVariable
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 10
zSHORT
fnSetAttributeFromVariable( zVIEW            lpView,
                            LPVIEWENTITYCSR  lpViewEntityCsr,
                            LPVIEWATTRIB     lpViewAttrib,
                            zCPVOID          lpVariable,
                            zCHAR            cVariableType,
                            zULONG           ulVariableLth,
                            zCPCHAR          cpcContextName,
                            LPTASK           lpCurrentTask,
                            zSHORT           nFlag )
{
   LPVIEWENTITY      lpViewEntity;
   LPTASKDOMAIN      lpTaskDomain;
   LPDOMAIN          lpDomain;
   zCHAR             cDomainType;
   zCHAR             szTempStr[ 2 ];
   zPLONG            plInteger;
   zPDOUBLE          pdDecimal;
   zBOOL             bValidationOnly;
   zLONG             lDomainEntryType;
   zSHORT            nRC;

   // Get view Entity from cursor
   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );

   // If the attribute is marked as required and the input variable
   // type is a string, we do not allow a null pointer or a pointer
   // to a null string as input.
   if ( lpViewAttrib->bRequired &&
        cVariableType == zTYPE_STRING &&
        (lpVariable == 0 || *((zPCHAR) lpVariable) == 0) )
   {
      zCHAR  szEntityName[ 255 ];
      zCHAR  szAttribName[ 255 ];

      strcpy_s( szEntityName, sizeof( szEntityName ), "Entity:\t" );
      strcat_s( szEntityName, sizeof( szEntityName ), lpViewEntity->szName );

      strcpy_s( szAttribName, sizeof( szAttribName ), "Attribute:\t" );
      strcat_s( szAttribName, sizeof( szAttribName ), lpViewAttrib->szName );

      // "KZOEE245 - Null string not allowed for a required attribute"
      IssueOE_Error( lpView, 8, 245,
                     zMSGQ_REQUIRED_DATA_ITEM_ERROR,
                     0, szEntityName, szAttribName );
      return( zCALL_ERROR );
   }

   // See if we are only doing validation.
   if ( nFlag & zVALIDATE )
   {
      bValidationOnly = TRUE;
      lDomainEntryType = (zLONG) zDME_VALIDATE_LPDATA;
   }
   else
   {
      bValidationOnly = FALSE;
      lDomainEntryType = (zLONG) zDME_SET_ATTRIBUTE;
   }

   // If the input type is string and if the value is null, then we
   // want to call fnSetAttributeFromString with the null value, which
   // will set the attribute to null, without going through the Domain.
   // Note that we have already gone through the validation to check
   // whether or not the attribute can be set to null, so if we have
   // gotton this far, it is ok.
   // To force the call to fnSetAttributeFromString, we only need to set
   // the variable cDomainType to 0. This causes the CASE structure
   // below to drop through to the default, which will call the operation.

   if ( cVariableType == zTYPE_STRING &&
        (lpVariable == 0 || *((zPCHAR) lpVariable) == 0) )
   {
      cDomainType = 0;
   }
   else
   {
      lpDomain = zGETPTR( lpViewAttrib->hDomain );
      cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                                lpCurrentTask,
                                                lpDomain );

      if ( cDomainType && cDomainType == zDM_TYPE_FORMAT &&
           (cpcContextName == 0 || *cpcContextName == 0) )
      {
         cDomainType = 0;
      }
   }

   switch ( cDomainType )
   {
      case zDM_TYPE_TABLE:
         nRC = Table_Handler( lDomainEntryType,
                              cVariableType,
                              (zPVOID) lpVariable,
                              cpcContextName,
                              lpView,
                              lpViewEntity,
                              lpViewAttrib,
                              ulVariableLth,
                              (zPVOID) 0 );
         break;

      case zDM_TYPE_FORMAT:
         if ( cVariableType != zTYPE_STRING )
         {
            zCHAR sz1[ 100 ];
            zCHAR sz2[ 100 ];

            strcpy_s( sz1, sizeof( sz1 ), "Type: " );
            sz1[ 6 ] = cVariableType;
            sz1[ 7 ] = 0;
            strcat_s( sz1, sizeof( sz1 ), ", Domain: " );
            strcat_s( sz1, sizeof( sz1 ), lpDomain->szName );
            strcpy_s( sz2, sizeof( sz2 ), lpViewEntity->szName );
            strcpy_s( sz2, sizeof( sz2 ), ", " );
            strcpy_s( sz2, sizeof( sz2 ), lpViewAttrib->szName );
            // "KZOEE362 - Variable Type not allowed for this Domain Type "
            fnIssueCoreError( lpCurrentTask, lpView, 8, 362, 0, sz1, sz2 );
            nRC = zCALL_ERROR;
         }
         else
         {
            zLPCONTEXT   lpContext;

            lpContext = fnGetContext( lpDomain, cpcContextName );

            if ( lpContext == 0 )
            {
               // "KZOEE350 - Context Not Valid for Domain"
               fnIssueCoreError( lpCurrentTask, lpView, 8, 350, 0,
                                 lpDomain->szName,
                                 TrueName( cpcContextName, zSHOW_ZKEY ));
               nRC = zCALL_ERROR;
            }
            else
            {
               zCHAR    sz[ 256 ];
               zPCHAR   pch;
               zPVOID   hndl = 0;
               zULONG   ul;

               ul = zstrlen( (zPCHAR) lpVariable ) + 1;
               if ( ul > sizeof( sz ) )
               {
                  hndl = fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                           ul, 0, 0, iAttributeString );
                  pch = zGETPTR( hndl );
               }
               else
                  pch = sz;

               strcpy_s( pch, ul, (zPCHAR) lpVariable );
               nRC = 0;
               if ( UfEditFormatString( pch, zGETPTR( lpContext->hEditString ) ) )
               {
                  zCHAR sz1[ 128 ];

                  strcpy_s( sz1, sizeof( sz1 ), lpViewEntity->szName );
                  strcat_s( sz1, sizeof( sz1 ), "." );
                  strcat_s( sz1, sizeof( sz1 ), lpViewAttrib->szName );
                  strcat_s( sz1, sizeof( sz1 ), "." );
                  strcpy_s( sz1, sizeof( sz1 ), lpDomain->szName );
                  strcat_s( sz1, sizeof( sz1 ), "." );
                  strcat_s( sz1, sizeof( sz1 ), TrueName( cpcContextName, 0 ) );
              // KJS 06/26/09 - We were calling IssueOE_Error but the error sent
              // to the user is too detailed.  We think this error should be more like
              // the domain errors.  So instead we will issue the MessageSend here.
                  TraceLineS( sz1, "" );
                  // "KZOEE361 - Invalid data Format
               // IssueOE_Error( lpView, 4, 361, 0, 0,
               //                zGETPTR( lpContext->hEditString ),
               //                sz1 );
                  strcpy_s( sz1, sizeof( sz1 ), "Invalid data format. Your data should be in the format: " );
                  strcat_s( sz1, sizeof( sz1 ), zGETPTR( lpContext->hEditString ) );

                  //return( MessageSend( zView, szMsgID, pchTitle, szMsg, lMsgType, 0 ) );
                  MessageSend( lpView, "361", szlApplicationLogicError, sz1, 4, 0 );
                  nRC = zCALL_ERROR;
               }

               if ( bValidationOnly )
                  nRC = (nRC == 0) ? 0 : nRC;
               else
               {
                  if ( nRC == 0 )
                     nRC = fnSetAttributeFromString( lpView, lpViewEntityCsr, lpViewAttrib, pch, 0 );
               }

               if ( hndl )
                  fnFreeDataspace( pch );
            }
         }

         break;

      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
         nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                 lDomainEntryType,
                                 cVariableType,
                                 (zPVOID) lpVariable,
                                 cpcContextName,
                                 lpView,
                                 lpViewEntity,
                                 lpViewAttrib,
                                 ulVariableLth );
         if ( nRC == zDME_NOT_HANDLED )
         {
            zCHAR szMsg[ 500 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Operation '%s' does not handle an Entry "
                       "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                       lDomainEntryType, lpDomain->szName );
            MessageSend( lpView, "TZDMD101", "System Error", szMsg, zMSGQ_DOMAIN_ERROR, 0 );
            return( zCALL_ERROR );
         }

         break;

      default:
      {
         if ( cVariableType == zTYPE_STRING && lpViewAttrib->cType == zTYPE_BLOB )
         {
            cVariableType = zTYPE_BLOB;  // dks 2007.01.11  permit blob to be set from string
            ulVariableLth = zstrlen( (zPCHAR) lpVariable ) + 1;
         }

         // do it without a domain ...
         switch ( cVariableType )
         {
            case zTYPE_STRING:
               if ( bValidationOnly )
                  nRC = 0;
               else
                  nRC = fnSetAttributeFromString( lpView, lpViewEntityCsr,
                                                  lpViewAttrib,
                                                  (zPCHAR) lpVariable, 0 );
               break;

            case zTYPE_BLOB:
               if ( bValidationOnly )
                  nRC = 0;
               else
                  nRC = fnSetAttributeFromBlob( lpView, lpViewEntityCsr,
                                                lpViewAttrib,
                                                (zPVOID) lpVariable,
                                                ulVariableLth );
               break;

            case zTYPE_INTEGER:
               if ( bValidationOnly )
                  nRC = 0;
               else
               {
                  plInteger = (zPLONG) lpVariable;
                  nRC = fnSetAttributeFromInteger( lpView,
                                                   lpViewEntityCsr,
                                                   lpViewAttrib,
                                                   *plInteger );
               }

               break;

            case zTYPE_DECIMAL:
               if ( bValidationOnly )
                  nRC = 0;
               else
               {
                  pdDecimal = (zPDECIMAL) lpVariable;
                  nRC = fnSetAttributeFromDecimal( zGETPTR( lpView->hTask ),
                                                   lpViewEntityCsr,
                                                   lpViewAttrib,
                                                   *pdDecimal );
               }

               break;

            default:
               szTempStr[ 0 ] = cVariableType;
               szTempStr[ 1 ] = '\x00';
               // "KZOEE241 - Invalid Variable Type"
               fnIssueCoreError( lpCurrentTask, lpView, 8, 241, 0, szTempStr, 0 );
               return( zCALL_ERROR );
         }
      }
   }

   // If there is an object operation for this attribute,
   // invoke it now...
   if ( nRC == 0 && bValidationOnly == FALSE &&
        lpViewAttrib->szDerivedOper[ 0 ] )
   {
      nRC = fnInvokeDerivedOperation( lpView, lpViewEntity, lpViewAttrib,
                                      lpCurrentTask, zDERIVED_SET );
   }

   return( nRC );
}

//./ ADD NAME=AddToAttributeFromVariable
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AddToAttributeFromVariable
//
// PURPOSE:  To add a value to an attribute using a varying type of
//           variable value.
//
// PARAMETERS:
//           lpView          - View to Object instance
//           cpcEntityName    - Entity name containing the attribute to
//                             add to.
//           cpcAttributeName - Attribute name to add to.
//           lpVariable      - pointer to the variable used in changing
//                             the attribute.
//           cVariableType   - Zeidon data type, indicates what
//                             lpVariable pointer to.
//           ulVariableLth   - Indicates length of variable that is
//                             pointed to by lpVariable.
//                             Only used when cVariableType is
//                             zTYPE_STRING or zTYPE_BLOB.
//           cpcContextName   - Optional context name, used in conjunction
//                             with the domain associated with the
//                             specified attribute.
//
// RETURNS: 0           - Attribute successfully changed
//          zCALL_ERROR - error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
AddToAttributeFromVariable( zVIEW   lpView,
                            zCPCHAR cpcEntityName,
                            zCPCHAR cpcAttributeName,
                            zPVOID  lpVariable,
                            zCHAR   cVariableType,
                            zULONG  ulVariableLth,
                            zCPCHAR cpcContextName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iAddToAttributeFromVariable,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC )
      nRC = zCALL_ERROR;
   else
   {
      // Call fnAddToAttributeFromVariable to do the setting.
      nRC = fnAddToAttributeFromVariable( lpView, lpViewEntityCsr,
                                          lpViewAttrib,
                                          lpVariable, cVariableType,
                                          ulVariableLth, cpcContextName,
                                          lpCurrentTask );
   }

   fnOperationReturn( iAddToAttributeFromVariable, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnAddToAttributeFromVariable
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnAddToAttributeFromVariable
//
//  PURPOSE:    To load set attribute information assuming all parameters
//              are valid for load and dbhandler situations
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zSHORT
fnAddToAttributeFromVariable( zVIEW            lpView,
                              LPVIEWENTITYCSR  lpViewEntityCsr,
                              LPVIEWATTRIB     lpViewAttrib,
                              zPVOID           lpVariable,
                              zCHAR            cVariableType,
                              zULONG           ulVariableLth,
                              zCPCHAR          cpcContextName,
                              LPTASK           lpCurrentTask )
{
   LPVIEWENTITY      lpViewEntity;
   LPTASKDOMAIN      lpTaskDomain;
   LPDOMAIN          lpDomain;
   zCHAR             cDomainType;
   zCHAR             szTempStr[ 2 ];
   zPLONG            plInteger;
   zPDOUBLE          pdDecimal;
   zSHORT            nRC;

   // Get view Entity from cursor
   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );

   lpDomain = zGETPTR( lpViewAttrib->hDomain );
   cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                             lpCurrentTask, lpDomain );
   switch ( cDomainType )
   {
      case zDM_TYPE_TABLE:
         nRC = Table_Handler( (zLONG) zDME_ADD_TO_ATTRIBUTE,
                              cVariableType,
                              lpVariable,
                              cpcContextName,
                              lpView,
                              lpViewEntity,
                              lpViewAttrib,
                              ulVariableLth,
                              (zPVOID) 0 );
         break;

      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
         nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                 zDME_ADD_TO_ATTRIBUTE,
                                 cVariableType,
                                 lpVariable,
                                 cpcContextName,
                                 lpView,
                                 lpViewEntity,
                                 lpViewAttrib,
                                 ulVariableLth );
         if ( nRC == zDME_NOT_HANDLED )
         {
            zCHAR szMsg[ 500 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Operation '%s' does not handle an Entry "
                       "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                       zDME_ADD_TO_ATTRIBUTE, lpDomain->szName );
            MessageSend( lpView, "TZDMD101", "System Error",
                         szMsg, zMSGQ_DOMAIN_ERROR, 0 );
            return( zCALL_ERROR );
         }

         break;

      case zDM_TYPE_FORMAT:
      default:
         // do it without a domain...
         switch ( cVariableType )
         {
          case zTYPE_STRING:
            if ( *((zPCHAR) lpVariable) == 0 )
            {
               // a null string, don't bother adding
               nRC = zCALL_ERROR;
               break;
            }

            if ( lpViewAttrib->cType == zTYPE_DECIMAL )
            {
               zDECIMAL  dDecimalValue;

               SysConvertStringToDecimal( (zPCHAR) lpVariable,
                                          &dDecimalValue );
               nRC = fnAddToAttributeFromDecimal( lpView,
                                                  lpViewEntityCsr,
                                                  lpViewAttrib,
                                                  dDecimalValue );
            }
            else
            if ( lpViewAttrib->cType == zTYPE_INTEGER )
            {
               // convert string to long, then add
               zLONG lIntegerValue;

               lIntegerValue = zatol( (zPCHAR) lpVariable );
               nRC = fnAddToAttributeFromInteger( lpView,
                                                  lpViewEntityCsr,
                                                  lpViewAttrib,
                                                  lIntegerValue );
            }
            else
               nRC = zCALL_ERROR;

            break;

          case zTYPE_BLOB:
            nRC = zCALL_ERROR;
            break;

          case zTYPE_INTEGER:
            plInteger = (zPLONG) lpVariable;
            nRC = fnAddToAttributeFromInteger( lpView,
                                               lpViewEntityCsr,
                                               lpViewAttrib,
                                               *plInteger );
            break;

          case zTYPE_DECIMAL:
            pdDecimal = (zPDECIMAL) lpVariable;
            nRC = fnAddToAttributeFromDecimal( lpView,
                                               lpViewEntityCsr,
                                               lpViewAttrib,
                                               *pdDecimal );
            break;

          default:
            szTempStr[ 0 ] = cVariableType;
            szTempStr[ 1 ] = '\x00';
            // "KZOEE241 - Invalid Variable Type"
            fnIssueCoreError( lpCurrentTask, lpView, 8, 241, 0, szTempStr, 0 );
            return( zCALL_ERROR );
         }
   }

   return( nRC );
}

//./ ADD NAME=fnCompareAttributeToString
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCompareAttributeToString
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnCompareAttributeToString( LPENTITYINSTANCE   lpEntityInstance,
                            LPVIEWENTITYCSR    lpViewEntityCsr,
                            LPVIEWATTRIB       lpViewAttrib,
                            zPCHAR             pchStringValue )
{
   zPCHAR   pchRecord;
   zCHAR    szWorkString[ 25 ];
   zDECIMAL dTemp;
   zLONG    lTemp;
   LPVIEWOI lpViewOI = zGETPTR( lpEntityInstance->hViewOI );
   zSHORT   nRC;

   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( pchRecord )
   {
      pchRecord += lpViewAttrib->ulRecordOffset;
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
            if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zCOREMEM  lpExternalP;

               lpExternalP = (zCOREMEM) (pchRecord + 1);
               pchRecord = zGETPTR( *lpExternalP );
            }

            if ( lpViewAttrib->bCaseSens )
               nRC = zstrcmp( pchRecord, pchStringValue );
            else
               nRC = zstrcmpi( pchRecord, pchStringValue );

            if ( nRC < 0 )
               nRC = -1;
            else
            if ( nRC > 0 )
               nRC = 1;

            break;

         case zTYPE_INTEGER:
            if ( pchStringValue[ 0 ] == 0 &&
                 *((zPLONG) pchRecord) == lNullInteger )
            {
               // empty string matches NULL integer
               nRC = 0;
               break;
            }

            lTemp = zatol( szWorkString );
            if ( *((zPLONG) pchRecord) < lTemp )
               nRC = -1;
            else
            if ( *((zPLONG) pchRecord) > lTemp )
               nRC = 1;
            else
               nRC = 0;

            break;

         case zTYPE_DECIMAL:
            // fnConvertDecimalToString and string comparison
            // is invalid, because the string representation is not unique
            if ( pchStringValue[ 0 ] == 0 &&
                 SysCompareDecimalToNull( (zPDECIMAL) pchRecord) == 0 )
            {
               // empty string matches NULL decimal
               nRC = 0;
               break;
            }

            SysConvertStringToDecimal( pchStringValue, &dTemp );
            nRC = SysCompareDecimalToDecimal( (zPDECIMAL) pchRecord, &dTemp );
            if ( nRC < 0 )
               nRC = -1;
            else
            if ( nRC > 0 )
               nRC = 1;

            break;

         case zTYPE_DATETIME:
            UfDateTimeToString( (LPDATETIME) pchRecord, szWorkString, 18 );
            nRC = zstrcmp( szWorkString, pchStringValue );
            if ( nRC < 0 )
               nRC = -1;
            else
            if ( nRC > 0 )
               nRC = 1;

            break;

         case zTYPE_BLOB:
            // We only allow a comparison of strings to blobs if the comparison
            // string is NULL.
            if ( pchStringValue == 0 || pchStringValue[ 0 ] == 0 )
            {
              if ( pchRecord[ 0 ] != (zCHAR) '\xff' )
                 nRC = 0;
              else
                 nRC = -1;

              break;
            }

            // NOTE: FALL THROUGH...
            // NOTE: FALL THROUGH...
            // NOTE: FALL THROUGH...

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }
   else
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_INTEGER:
         case zTYPE_DECIMAL:
         case zTYPE_DATETIME:
            szWorkString[ 0 ] = 0;
            nRC = zstrcmp( szWorkString, pchStringValue );
            if ( nRC < 0 )
               nRC = -1;
            else
            if ( nRC > 0 )
               nRC = 1;

            break;

         case zTYPE_BLOB:
            // We only allow a comparison of strings to blobs if the comparison
            // string is NULL.
            if ( pchStringValue == 0 || pchStringValue[ 0 ] == 0 )
            {
               nRC = 0;
               break;
            }

            // NOTE: FALL THROUGH...
            // NOTE: FALL THROUGH...
            // NOTE: FALL THROUGH...

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=fnCompareAttributeToInteger
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCompareAttributeToInteger
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnCompareAttributeToInteger( LPENTITYINSTANCE   lpEntityInstance,
                             LPVIEWENTITYCSR    lpViewEntityCsr,
                             LPVIEWATTRIB       lpViewAttrib,
                             zLONG              lIntegerValue )
{
   zPCHAR   pchRecord;
   zLONG    lWork;
   zDECIMAL dWork;
   LPVIEWOI lpViewOI = zGETPTR( lpEntityInstance->hViewOI );
   zSHORT   nRC;

   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( pchRecord )
   {
      pchRecord += lpViewAttrib->ulRecordOffset;
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
            if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zCOREMEM  lpExternalP;

               lpExternalP = (zCOREMEM) (pchRecord + 1);
               pchRecord = zGETPTR( *lpExternalP );
            }

            lWork = zatol( pchRecord );
            lWork -= lIntegerValue;
            nRC = (lWork == 0) ? 0 : (lWork < 0) ? -1 : 1;
            break;

         case zTYPE_INTEGER:
            lWork = *((zPLONG) pchRecord) - lIntegerValue;
            nRC = (lWork == 0) ? 0 : (lWork < 0) ? -1 : 1;
            break;

         case zTYPE_DECIMAL:
            SysConvertLongToDecimal( lIntegerValue, &dWork );
            nRC = SysCompareDecimalToDecimal( (zPDECIMAL) pchRecord, &dWork );
            break;

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }
   else
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_INTEGER:
         case zTYPE_DECIMAL:
            lWork = 0;
            lWork -= lIntegerValue;
            nRC = (lWork == 0) ? 0 : (lWork < 0) ? -1 : 1;
            break;

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=fnCompareAttributeToDecimal
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCompareAttributeToDecimal
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnCompareAttributeToDecimal( LPENTITYINSTANCE   lpEntityInstance,
                             LPVIEWENTITYCSR    lpViewEntityCsr,
                             LPVIEWATTRIB       lpViewAttrib,
                             zDECIMAL           dDecimalValue )
{
   zPCHAR   pchRecord;
   zDECIMAL dWork;
   LPVIEWOI lpViewOI = zGETPTR( lpEntityInstance->hViewOI );
   zSHORT   nRC;

   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( pchRecord )
   {
      pchRecord += lpViewAttrib->ulRecordOffset;
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
            if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zCOREMEM  lpExternalP;

               lpExternalP = (zCOREMEM) (pchRecord + 1);
               pchRecord = zGETPTR( *lpExternalP );
            }

            SysConvertStringToDecimal( pchRecord, &dWork );
            nRC = SysCompareDecimalToDecimal( &dWork, &dDecimalValue );
            break;

         case zTYPE_INTEGER:
            SysConvertLongToDecimal( *((zPLONG) pchRecord), &dWork );
            nRC = SysCompareDecimalToDecimal( &dWork, &dDecimalValue );
            break;

         case zTYPE_DECIMAL:
            nRC = SysCompareDecimalToDecimal( (zPDECIMAL) pchRecord,
                                              &dDecimalValue );
            break;

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }
   else
   {
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_INTEGER:
         case zTYPE_DECIMAL:
            SysConvertLongToDecimal( 0, &dWork );
            nRC = SysCompareDecimalToDecimal( (zPDECIMAL) pchRecord,
                                              &dDecimalValue );
            break;

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=fnCompareAttributeToBlob
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnCompareAttributeToBlob
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnCompareAttributeToBlob( LPENTITYINSTANCE   lpEntityInstance,
                          LPVIEWENTITYCSR    lpViewEntityCsr,
                          LPVIEWATTRIB       lpViewAttrib,
                          zPCHAR             pchBlobValue,
                          zULONG             ulBlobLth )
{
   zPCHAR   pchRecord;
   zULONG   ulAttrBlobLth;
   zSHORT   nRC;

   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( pchRecord )
   {
      pchRecord += lpViewAttrib->ulRecordOffset;
      if ( lpViewAttrib->cType == zTYPE_BLOB )
      {
         if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
         {
            zCOREMEM  lpExternalP;

            lpExternalP = (zCOREMEM) (pchRecord + 1);
            ulAttrBlobLth = *((zPULONG) (lpExternalP + 1));
            pchRecord = zGETPTR( *lpExternalP );
            if ( ulAttrBlobLth != ulBlobLth )
               nRC = (ulAttrBlobLth < ulBlobLth) ? -1 : 1;
            else
               nRC = zmemcmp( pchRecord, pchBlobValue,
                              (unsigned int) ulBlobLth );
         }
         else
         {
            // blob is null
            nRC = ( ulBlobLth == 0 ) ? 0 : -1;
         }
      }
      else
      {
         LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
         zVIEW     lpView = zGETPTR( lpViewCsr->hView );

         // "KZOEE239 - Invalid Operation for attribute type"
         fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ),
                           lpViewAttrib->szName );
         nRC = zCALL_ERROR;
      }
   }
   else
   {
      if ( lpViewAttrib->cType == zTYPE_BLOB )
         nRC = ( ulBlobLth == 0 ) ? 0 : -1;
      else
      {
         LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
         zVIEW     lpView = zGETPTR( lpViewCsr->hView );

         // "KZOEE239 - Invalid Operation for attribute type"
         fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                           fnEntityNameFromCsr( lpViewEntityCsr ),
                           lpViewAttrib->szName );
         nRC = zCALL_ERROR;
      }
   }

   return( nRC );
}

//./ ADD NAME=fnCompareDateTimeToDateTime
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnCompareDateTimeToDateTime
//
// PARAMETERS: lpDateTime1
//             lpDateTime2
//
// RETURNS: -1 - DateTime1 less than DateTime2
//           0 - DateTime1 equal DateTime2
//           1 - DateTime1 greater than DateTime2
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnCompareDateTimeToDateTime( LPDTINTERNAL lpDateTime1,
                             LPDTINTERNAL lpDateTime2 )
{
   zSHORT   nRC;

   if ( lpDateTime1->ulDateMinutes == lpDateTime2->ulDateMinutes )
   {
      if ( (zLONG) lpDateTime1->ulDateMinutes == lNullInteger ||
           lpDateTime1->usTSeconds == lpDateTime2->usTSeconds )
      {
         nRC = 0;
      }
      else
         nRC = (lpDateTime1->usTSeconds < lpDateTime2->usTSeconds) ? -1 : 1;
   }
   else
   {
      if ( (zLONG) lpDateTime1->ulDateMinutes == lNullInteger )
         nRC = -1;
      else
         if ( (zLONG) lpDateTime2->ulDateMinutes == lNullInteger )
            nRC = 1;
         else
            nRC = (lpDateTime1->ulDateMinutes < lpDateTime2->ulDateMinutes) ? -1 : 1;
   }

   return( nRC );
}

//./ ADD NAME=fnCompareAttributeToDateTime
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnCompareAttributeToDateTime
//
// NOTE:     A work in progress...
//
// RETURNS: -1 - Attribute less than DateTime
//           0 - Attribute equal DateTime
//           1 - Attribute greater than DateTime
//    zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnCompareAttributeToDateTime( LPENTITYINSTANCE   lpEntityInstance,
                              LPVIEWENTITYCSR    lpViewEntityCsr,
                              LPVIEWATTRIB       lpViewAttrib,
                              LPDATETIME         lpDateTime )
{
   zPCHAR         pchRecord;
   DateTimeRecord stDateTime;
   LPDTINTERNAL   lpRecdDateTime;
   zSHORT         nRC;

   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( pchRecord )
   {
      pchRecord += lpViewAttrib->ulRecordOffset;
      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
            if ( pchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zCOREMEM  lpExternalP;

               lpExternalP = (zCOREMEM) (pchRecord + 1);
               pchRecord = zGETPTR( *lpExternalP );
            }

            nRC = UfStringToDateTime( pchRecord,
                                      (LPDATETIME) &stDateTime );
            if ( nRC == 0 )
            {
               nRC = fnCompareDateTimeToDateTime( (LPDTINTERNAL) &stDateTime,
                                                  (LPDTINTERNAL) lpDateTime );
            }

            break;

         case zTYPE_DATETIME:
            lpRecdDateTime = (LPDTINTERNAL) pchRecord;
            nRC = fnCompareDateTimeToDateTime( lpRecdDateTime,
                                               (LPDTINTERNAL) lpDateTime );
            break;

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }
   else
   {
      DateTimeInternalRecord stDT;

      switch ( lpViewAttrib->cType )
      {
         case zTYPE_STRING:
         case zTYPE_DATETIME:
            stDT.ulDateMinutes = lNullInteger;
            stDT.usTSeconds = 0;
            nRC = fnCompareDateTimeToDateTime( (LPDTINTERNAL) &stDT,
                                               (LPDTINTERNAL) lpDateTime );
            break;

         default:
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
            zVIEW     lpView = zGETPTR( lpViewCsr->hView );

            // "KZOEE239 - Invalid Operation for attribute type"
            fnIssueCoreError( zGETPTR( lpView->hTask ), lpView, 8, 239, 0,
                              fnEntityNameFromCsr( lpViewEntityCsr ),
                              lpViewAttrib->szName );
            nRC = zCALL_ERROR;
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=CompareAttributeToVariable
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CompareAttributeToVariable
//
// PURPOSE:  To the value of an attribute to a varying type of
//           variable value.
//
// PARAMETERS:
//           lpView          - View to Object instance
//           cpcEntityName    - Entity name containing the attribute to
//                             compare.
//           cpcAttributeName - Attribute name to compare.
//           lpVariable      - pointer to the variable used in comparing
//                             the attribute.
//           cVariableType   - Zeidon data type, indicates what
//                             lpVariable pointer to.
//           ulVariableLth   - Indicates length of variable that is
//                             pointed to by lpVariable.
//                             Only used when cVariableType is
//                             zTYPE_BLOB.
//           cpcContextName   - Optional context name, used in conjunction
//                             with the domain associated with the
//                             specified attribute.
//           nFlag           - Reserved for future use.
//
// RETURNS: -1 - Attribute less than variable
//           0 - Attribute equal variable
//           1 - Attribute greater than variable
//    zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zSHORT OPERATION
CompareAttributeToVariable( zVIEW    lpView,
                            zCPCHAR  cpcEntityName,
                            zCPCHAR  cpcAttributeName,
                            zCPVOID  zVariable,
                            zCHAR    cVariableType,
                            zULONG   ulVariableLth,
                            zCPCHAR  cpcContextName,
                            zSHORT   nFlag )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iCompareAttributeToVariable,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC )
      nRC = zCALL_ERROR;
   else
   {
      // Call fnCompareAttributeToVariable to do the comparing
      nRC = fnCompareAttributeToVariable( lpView, lpViewEntityCsr,
                                          lpViewAttrib,
                                          (zPVOID) zVariable, cVariableType,
                                          ulVariableLth,
                                          cpcContextName, lpCurrentTask,
                                          nFlag );
   }

   fnOperationReturn( iCompareAttributeToVariable, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnCompareAttributeToVariable
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnCompareAttributeToVariable
//
// NOTE:    A work in progress...
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 10
zSHORT
fnCompareAttributeToVariable( zVIEW             lpView,
                              LPVIEWENTITYCSR   lpViewEntityCsr,
                              LPVIEWATTRIB      lpViewAttrib,
                              zPVOID            lpVariable,
                              zCHAR             cVariableType,
                              zULONG            ulVariableLth,
                              zCPCHAR           cpcContextName,
                              LPTASK            lpCurrentTask,
                              zSHORT            nFlag )
{
   LPVIEWENTITY      lpViewEntity;
   LPENTITYINSTANCE  lpEntityInstance;
   LPDOMAIN          lpDomain;
   LPTASKDOMAIN      lpTaskDomain;
   zCHAR             cDomainType;
   zCHAR             szTempStr[ 2 ];
   zPLONG            plInteger;
   zPDECIMAL         pdDecimal;
   zSHORT            nRC;

   // Get view Entity from cursor
   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );

   // If there is an object operation for this attribute,
   // invoke it now...
   if ( lpViewAttrib->szDerivedOper[ 0 ] )
   {
      nRC = fnInvokeDerivedOperation( lpView, lpViewEntity, lpViewAttrib,
                                      lpCurrentTask, zDERIVED_GET );
      if ( nRC == zCALL_ERROR )
         return( nRC );
   }

   lpDomain = zGETPTR( lpViewAttrib->hDomain );
   cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                             lpCurrentTask, lpDomain );
   switch ( cDomainType )
   {
      case zDM_TYPE_TABLE:
         nRC = Table_Handler( (zLONG) zDME_COMPARE_ATTRIBUTE,
                              cVariableType,
                              lpVariable,
                              cpcContextName,
                              lpView,
                              lpViewEntity,
                              lpViewAttrib,
                              (zSHORT) ulVariableLth,
                              (zPVOID) 0 );
         break;

      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
         nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                 zDME_COMPARE_ATTRIBUTE,
                                 cVariableType,
                                 lpVariable,
                                 cpcContextName,
                                 lpView,
                                 lpViewEntity,
                                 lpViewAttrib,
                                 ulVariableLth );

         if ( nRC == zDME_NOT_HANDLED )
         {
            zCHAR szMsg[ 500 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Operation '%s' does not handle an Entry "
                       "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                       zDME_COMPARE_ATTRIBUTE, lpDomain->szName );
            MessageSend( lpView, "TZDMD101", "System Error",
                         szMsg, zMSGQ_DOMAIN_ERROR, 0 );
            return( zCALL_ERROR );
         }

         break;

      case zDM_TYPE_FORMAT:
      default:
         // Get Entity Instance pointer
         lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

         // do it without a domain...
         switch ( cVariableType )
         {
            case zTYPE_STRING:
               nRC = fnCompareAttributeToString( lpEntityInstance,
                                                 lpViewEntityCsr,
                                                 lpViewAttrib,
                                                 (zPCHAR) lpVariable );
               break;

            case zTYPE_BLOB:
               nRC = fnCompareAttributeToBlob( lpEntityInstance,
                                               lpViewEntityCsr,
                                               lpViewAttrib,
                                               (zPCHAR) lpVariable,
                                               ulVariableLth );
               break;

            case zTYPE_INTEGER:
               plInteger = (zPLONG) lpVariable;
               nRC = fnCompareAttributeToInteger( lpEntityInstance,
                                                  lpViewEntityCsr,
                                                  lpViewAttrib,
                                                  *((zPLONG) lpVariable) );
               break;

            case zTYPE_DECIMAL:
               pdDecimal = (zPDECIMAL) lpVariable;
               nRC = fnCompareAttributeToDecimal( lpEntityInstance,
                                                  lpViewEntityCsr,
                                                  lpViewAttrib,
                                                  *((zPDECIMAL) lpVariable) );
               break;

            case zTYPE_DATETIME:
               nRC = fnCompareAttributeToDateTime( lpEntityInstance,
                                                   lpViewEntityCsr,
                                                   lpViewAttrib,
                                                   (LPDATETIME) lpVariable );
               break;

            default:
            {
               LPTASK lpTask = zGETPTR( lpView->hTask );
               szTempStr[ 0 ] = cVariableType;
               szTempStr[ 1 ] = '\x00';
               // "KZOEE241 - Invalid Variable Type"
               fnIssueCoreError( lpTask, lpView, 8, 241, 0, szTempStr, 0 );
               nRC = zCALL_ERROR;
            }
         }
   }

   return( nRC );
}

//./ ADD NAME=GetVariableFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetVariableFromAttribute
//
// PURPOSE:  To retrieve a value from an attribute using a varying type of
//           variable output value.
//
// PARAMETERS:
//           lpVariable      - pointer to the variable to receive the
//                             the attribute value.
//
//           pulReturnLth    - Used to return the length of blobs.
//
//           cVariableType   - Zeidon data type, indicates what
//                             lpVariable pointer to.
//
//           ulVariableLth   - Indicates length of variable that is
//                             pointed to by lpVariable.
//                             Only used when cVariableType is
//                             zTYPE_STRING or zTYPE_BLOB.
//
//           lpView          - View to Object instance
//
//           cpcEntityName    - Entity name containing the attribute to get
//
//           cpcAttributeName - Attribute name to retrieve.
//
//           cpcContextName   - Optional context name, used in conjunction
//                             with the domain associated with the
//                             specified attribute.
//
//           nFlag           - Should be set to zACCEPT_NULL_ENTITY, if
//                             user wants to allow a return code of
//                             zNULL_ENTITY, otherwise should be 0. If this
//                             flag is zero and the entity is null, a
//                             zCALL_ERROR is returned.
//
//                             zUSE_DEFAULT_CONTEXT indicates that the
//                             default Context of the Domain associated
//                             with the attribute will be used in process-
//                             ing this request.  Any value specified by
//                             the cpcContextName parameter is iqnored.
//
// RETURNS: zVAR_SET     - Attribute successfully retrieved
//          zVAR_NULL    - Indicates that the attribute value is a null
//                         value
//          zENTITY_NULL - Indicates that the entity is null, only returned
//                         when zACCEPT_NULL_ENTITY specified in nFlag.
//          zCALL_ERROR  - error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 10
zSHORT OPERATION
GetVariableFromAttribute( zPVOID    lpVariable,
                          zPULONG   pulReturnLth,
                          zCHAR     cVariableType,
                          zULONG    ulVariableLth,
                          zVIEW     lpView,
                          zCPCHAR   cpcEntityName,
                          zCPCHAR   cpcAttributeName,
                          zCPCHAR   cpcContextName,
                          zSHORT    nFlag )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zLPCONTEXT        lpDefaultContext;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetVariableFromAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, nFlag );

   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC && nRC != -2 )
   {
      fnOperationReturn( iGetVariableFromAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( nRC == -2 ) // entity is null
   {
      switch ( cVariableType )
      {
         case zTYPE_STRING:
            *((zPCHAR) lpVariable) = 0;
            break;

         case zTYPE_INTEGER:
            *((zPLONG) lpVariable) = 0;
            break;

         case zTYPE_DECIMAL:
            SysConvertLongToDecimal( 0, (zPDECIMAL) lpVariable );
            break;

         case zTYPE_BLOB:
            *pulReturnLth = 0;
            *((zPCHAR) lpVariable) = 0;
            break;
      }
   }
   else
   {
      if ( nFlag & zUSE_DEFAULT_CONTEXT )
      {
         if ( GetDefaultContext( &lpDefaultContext,
                                 zGETPTR( lpViewAttrib->hDomain ) ) )
         {
            cpcContextName = lpDefaultContext->szName;
         }
         else
         {
            // "KZOEE232 - Operation indicates use default context,
            // none found"
            fnIssueCoreError( lpCurrentTask, lpView,
                              (zSHORT) (lpViewAttrib->hDomain ? 16 : 8),
                              232, 0L, lpViewEntity->szName,
                              lpViewAttrib->szName );
            fnOperationReturn( iGetVariableFromAttribute, lpCurrentTask );
            return( zCALL_ERROR );
         }
      }

      // Call fnGetVariableFromAttribute for the actual retrieval
      nRC = fnGetVariableFromAttribute( lpVariable, pulReturnLth,
                                        cVariableType,
                                        ulVariableLth,
                                        lpView, lpViewEntityCsr,
                                        lpViewAttrib,
                                        cpcContextName,
                                        lpCurrentTask, nFlag );
   }

   fnOperationReturn( iGetVariableFromAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnGetVariableFromAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnGetVariableFromAttribute
//
// NOTE:    A work in progress...
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 11
zSHORT
fnGetVariableFromAttribute( zPVOID           lpVariable,
                            zPULONG          pulReturnLth,
                            zCHAR            cVariableType,
                            zULONG           ulVariableLth,
                            zVIEW            lpView,
                            LPVIEWENTITYCSR  lpViewEntityCsr,
                            LPVIEWATTRIB     lpViewAttrib,
                            zCPCHAR          cpcContextName,
                            LPTASK           lpCurrentTask,
                            zSHORT           nFlag )
{
   LPVIEWENTITY      lpViewEntity;
   LPENTITYINSTANCE  lpEntityInstance;
   LPTASKDOMAIN      lpTaskDomain;
   LPDOMAIN          lpDomain;
   zCHAR             cDomainType;
   zCHAR             szTempStr[ 2 ];
   zPLONG            plInteger;
   zPDECIMAL         pdDecimal;
   zSHORT            nRC;

   // Get view Entity from cursor.
   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );

   // Get Entity Instance pointer.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // If return variable is a string, init to null.
   if ( cVariableType == zTYPE_STRING || cVariableType == zTYPE_PIC )
      *((zPCHAR) lpVariable) = 0;

   // If there is an object operation for this attribute, invoke it now ...
   if ( lpViewAttrib->szDerivedOper[ 0 ] )
   {
      nRC = fnInvokeDerivedOperation( lpView, lpViewEntity, lpViewAttrib,
                                      lpCurrentTask, zDERIVED_GET );
      if ( nRC == zCALL_ERROR )
         return( nRC );
   }

   lpDomain = zGETPTR( lpViewAttrib->hDomain );
   cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                             lpCurrentTask, lpDomain );

   // If cVariableType is zTYPE_STRING or if the cVariableType matches the
   // internal data type, and there is no Context Name specified, just
   // return the internal value.
   if ( cDomainType &&
        (cVariableType == zTYPE_STRING ||
         cVariableType == lpViewAttrib->cType) &&
        (cpcContextName == 0 || *cpcContextName == 0) &&
        (nFlag & zUSE_DEFAULT_CONTEXT) == 0 )
   {
      cDomainType = 0;
   }

   switch ( cDomainType )
   {
      case zDM_TYPE_TABLE:
         nRC = Table_Handler( (zLONG) zDME_GET_VARIABLE,
                              cVariableType,
                              lpVariable,
                              cpcContextName,
                              lpView,
                              lpViewEntity,
                              lpViewAttrib,
                              ulVariableLth,
                              (zPVOID) 0 );
         if ( nRC == 0 && cVariableType == zTYPE_STRING && pulReturnLth )
            *pulReturnLth = zstrlen( (zPCHAR) lpVariable );

         break;

      case zDM_TYPE_FORMAT:
         if ( cVariableType == zTYPE_PIC )
         {
            zLPCONTEXT lpContext = fnGetContext( lpDomain, cpcContextName );
            if ( lpContext )
            {
               if ( lpContext->hEditString )
               {
                  zULONG u = zstrlen( zGETPTR( lpContext->hEditString ) );
                  if ( u > ulVariableLth )
                  {
                     LPTASK lpTask = zGETPTR( lpView->hTask );
                     // "KZOEE364 - Picture length exceeds input length "
                     fnIssueCoreError( lpTask, lpView, 8, 364, 0,
                                       lpDomain->szName,
                                       TrueName( cpcContextName, zSHOW_ZKEY ));
                     nRC = zCALL_ERROR;
                  }
                  else
                  {
                     strcpy_s( (zPCHAR) lpVariable, ulVariableLth, zGETPTR( lpContext->hEditString ) );
                     nRC = 0;
                     if ( pulReturnLth )
                        *pulReturnLth = zstrlen( (zPCHAR) lpVariable );
                  }
               }
               else
               {
                  LPTASK lpTask = zGETPTR( lpView->hTask );
                  // "KZOEE363 - Context Edit string is invalid "
                  fnIssueCoreError( lpTask, lpView, 8, 363, 0,
                                    lpDomain->szName,
                                    TrueName( cpcContextName, zSHOW_ZKEY ));
                  nRC = zCALL_ERROR;
               }
            }
            else
            {
               LPTASK lpTask = zGETPTR( lpView->hTask );
               // "KZOEE350 - Context Not Valid for Domain"
               fnIssueCoreError( lpTask, lpView, 8, 350, 0, lpDomain->szName,
                                 TrueName( cpcContextName, zSHOW_ZKEY ));
               nRC = zCALL_ERROR;
            }
         }
         else
         if ( cVariableType != zTYPE_STRING )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            zCHAR sz1[ 100 ];
            zCHAR sz2[ 100 ];

            strcpy_s( sz1, sizeof( sz1 ), "Type: " );
            sz1[ 6 ] = cVariableType;
            sz1[ 7 ] = 0;
            strcat_s( sz1, sizeof( sz1 ), ", Domain: " );
            strcat_s( sz1, sizeof( sz1 ), lpDomain->szName );
            strcpy_s( sz2, sizeof( sz2 ), lpViewEntity->szName );
            strcpy_s( sz2, sizeof( sz2 ), ", " );
            strcpy_s( sz2, sizeof( sz2 ), lpViewAttrib->szName );
            // "KZOEE362 - Variable Type not allowed for this Domain Type "
            fnIssueCoreError( lpTask, lpView, 8, 362, 0, sz1, sz2 );
            nRC = zCALL_ERROR;
         }
         else
         {
            nRC = fnGetStringFromAttribute( (zPCHAR) lpVariable,
                                            ulVariableLth,
                                            lpView, lpViewEntityCsr,
                                            lpViewAttrib, 0 );
            switch ( nRC )
            {
               case 0:
               {
                  zLPCONTEXT lpContext = fnGetContext( lpDomain, cpcContextName );
                  if ( lpContext )
                  {
                     if ( UfFormatString( (zPCHAR) lpVariable,
                                          zGETPTR( lpContext->hEditString ) ) )
                     {
                        LPTASK lpTask = zGETPTR( lpView->hTask );
                        zCHAR sz1[ 66 ];
                        zCHAR sz2[ 66 ];

                        strcpy_s( sz1, sizeof( sz1 ), lpViewEntity->szName );
                        strcat_s( sz1, sizeof( sz1 ), "/" );
                        strcat_s( sz1, sizeof( sz1 ), lpViewAttrib->szName );
                        strcpy_s( sz2, sizeof( sz2 ), lpDomain->szName );
                        strcat_s( sz2, sizeof( sz2 ), "/" );
                        strcat_s( sz2, sizeof( sz2 ), TrueName( cpcContextName, 0 ) );
                        // "KZOEE361 - Could not process variable
                        //  with Context Format "
                        fnIssueCoreError( lpTask, lpView, 8, 361, 0, sz1, sz2 );
                        nRC = zCALL_ERROR;
                     }
                  }
                  else
                  {
                     LPTASK lpTask = zGETPTR( lpView->hTask );
                     // "KZOEE350 - Context Not Valid for Domain"
                     fnIssueCoreError( lpTask, lpView, 8, 350, 0, lpDomain->szName,
                                       TrueName( cpcContextName, zSHOW_ZKEY ));
                     nRC = zCALL_ERROR;
                  }

                  break;
               }

               case -1:
                  break;

            } // end switch ( nRC )

            if ( pulReturnLth )
            {
               if ( nRC == 0 )
                  *pulReturnLth = zstrlen( (zPCHAR) lpVariable );
               else
                  *pulReturnLth = 0;
            }
         }

         break;

      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
         nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                 zDME_GET_VARIABLE,
                                 cVariableType,
                                 lpVariable,
                                 cpcContextName,
                                 lpView,
                                 lpViewEntity,
                                 lpViewAttrib,
                                 ulVariableLth );

         if ( nRC == 0 )
         {
            if ( cVariableType == zTYPE_STRING && pulReturnLth )
               *pulReturnLth = zstrlen( (zPCHAR) lpVariable );
         }
         else
         if ( nRC == zDME_NOT_HANDLED )
         {
            zCHAR szMsg[ 500 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Operation '%s' does not handle an Entry "
                       "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                       zDME_GET_VARIABLE, lpDomain->szName );
            MessageSend( lpView, "TZDMD101", "System Error",
                         szMsg, zMSGQ_DOMAIN_ERROR, 0 );
            return( zCALL_ERROR );
         }

         break;

      default:
      {
         if ( cVariableType == zTYPE_STRING && lpViewAttrib->cType == zTYPE_BLOB )
         {
            cVariableType = zTYPE_BLOB;  // dks 2007.01.11  permit blob to be set from string
            if ( ulVariableLth == 0 )
               ulVariableLth = zstrlen( (zPCHAR) lpVariable );
               if ( cVariableType == zTYPE_STRING )  // dks 2015.12.15 we need space for the null terminator
                  ulVariableLth++;
         }

         // do it without a domain...
         switch ( cVariableType )
         {
            case zTYPE_STRING:
               nRC = fnGetStringFromAttribute( (zPCHAR) lpVariable, ulVariableLth,
                                               lpView, lpViewEntityCsr,
                                               lpViewAttrib,
                                               cpcContextName );
               if ( pulReturnLth )
               {
                  if ( nRC == 0 )
                     *pulReturnLth = zstrlen( (zPCHAR) lpVariable );
                  else
                     *pulReturnLth = 0;
               }

               break;

            case zTYPE_BLOB:
               *pulReturnLth = ulVariableLth;   // set max size for next call below
               nRC = fnGetBlobFromAttribute( lpVariable, pulReturnLth,
                                             lpView, lpViewEntityCsr,
                                             lpViewAttrib );
               break;

            case zTYPE_INTEGER:
               plInteger = (zPLONG) lpVariable;
               nRC = fnGetIntegerFromAttribute( plInteger,
                                                lpView, lpViewEntityCsr,
                                                lpViewAttrib );
               break;

            case zTYPE_DECIMAL:
               pdDecimal = (zPDECIMAL) lpVariable;
               nRC = fnGetDecimalFromAttribute( pdDecimal,
                                                lpView, lpViewEntityCsr,
                                                lpViewAttrib );
               break;

            case zTYPE_PIC:
               nRC = fnGetAttributeLength( pulReturnLth, lpView,
                                           zGETPTR( lpViewEntityCsr->hEntityInstance ),
                                           lpViewAttrib );
               break;

            default:
            {
               LPTASK lpTask = zGETPTR( lpView->hTask );
               szTempStr[ 0 ] = cVariableType;
               szTempStr[ 1 ] = '\x00';
               // "KZOEE241 - Invalid Variable Type"
               fnIssueCoreError( lpTask, lpView, 8, 241, 0, szTempStr, 0 );
               return( zCALL_ERROR );
            }
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=GetFirstTableEntryForAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetFirstTableEntryForAttribute
//
// PURPOSE:    To return the value of the first table entry for a given
//             attribute and context.
//
// PARAMETERS: pchReturnString  - External Value of table entry returned
//             lpView           - View to object instance
//             cpcEntityName    - Entity name
//             cpcAttributeName - Attribute name
//             cpcContextName   - Context
//             pvTablePosition  - Used for internal positioning
//
// RETURNS: 0           - Information returned
//         -1           - No table entries present
//          zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
GetFirstTableEntryForAttribute( zPCHAR  pchReturnString,
                                zLONG   lMaxLth,
                                zVIEW   lpView,
                                zCPCHAR cpcEntityName,
                                zCPCHAR cpcAttributeName,
                                zCPCHAR cpcContextName,
                                zPLONG  pvTablePosition )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   LPTASKDOMAIN      lpTaskDomain;
   zCHAR             cDomainType;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetFirstTableEntryForAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, zACCEPT_NULL_ENTITY );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC && nRC != -2 )
   {
      fnOperationReturn( iGetFirstTableEntryForAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = zCALL_ERROR;

   cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                             lpCurrentTask,
                                             zGETPTR( lpViewAttrib->hDomain ) );
   switch ( cDomainType )
   {
      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
      case zDM_TYPE_TABLE:
         nRC = Table_Handler( (zLONG) zDME_GET_FIRST_TBL_ENT_FOR_ATTR,
                              0,
                              pchReturnString,
                              cpcContextName,
                              lpView,
                              lpViewEntity,
                              lpViewAttrib,
                              lMaxLth,
                              (zPVOID) pvTablePosition );
         break;

      case zDM_TYPE_FORMAT:
      {
         zCHAR sz[ 2 ];
         LPDOMAIN lpDomain = zGETPTR( lpViewAttrib->hDomain );

         // "KZOEE355 - Table_Handler invalid for this Domain Type "
         sz[ 0 ] = cDomainType;
         sz[ 1 ] = 0;
         fnIssueCoreError( lpCurrentTask, lpView, 8, 355, 0, sz, lpDomain->szName );
         nRC = zCALL_ERROR;
         break;
      }

      default:
         // "KZOEE244 - Attribute has no Domain"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 244, 0, cpcEntityName, cpcAttributeName );
         nRC = zCALL_ERROR;
   }

   fnOperationReturn( iGetFirstTableEntryForAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetNextTableEntryForAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetNextTableEntryForAttribute
//
// PURPOSE:    To return the value of the next table entry for a given
//             attribute and context.  A GetFirstTableEntryForAttribute
//             must have been done prior to this call.
//
// PARAMETERS: pchReturnString  - External Value of table entry returned
//             lpView           - View to object instance
//             cpcEntityName    - Entity name
//             cpcAttributeName - Attribute name
//             cpcContextName   - Context
//             pvTablePosition  - Used for internal positioning
//
// RETURNS: 0           - Information returned
//          -1          - No more entries in table
//          zCALL_ERROR - Error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 11
zSHORT OPERATION
GetNextTableEntryForAttribute( zPCHAR   pchReturnString,
                               zLONG    lMaxLth,
                               zVIEW    lpView,
                               zCPCHAR  cpcEntityName,
                               zCPCHAR  cpcAttributeName,
                               zCPCHAR  cpcContextName,
                               zPLONG   pvTablePosition )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   LPTASKDOMAIN      lpTaskDomain;
   zCHAR             cDomainType;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iGetNextTableEntryForAttribute,
                                        0,   // We do not intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName,
                                        zACCEPT_NULL_ENTITY );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC && nRC != -2 )
   {
      fnOperationReturn( iGetNextTableEntryForAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = zCALL_ERROR;
   cDomainType = fnDomainProcessingRequired( &lpTaskDomain,
                                             lpView,
                                             lpCurrentTask,
                                             zGETPTR( lpViewAttrib->hDomain ) );
   switch ( cDomainType )
   {
      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
      case zDM_TYPE_TABLE:
         nRC = Table_Handler( (zLONG) zDME_GET_NEXT_TBL_ENT_FOR_ATTR,
                              0,
                              pchReturnString,
                              cpcContextName,
                              lpView,
                              lpViewEntity,
                              lpViewAttrib,
                              lMaxLth,
                              (zPVOID) pvTablePosition );
         break;

      case zDM_TYPE_FORMAT:
      {
         zCHAR    sz[ 2 ];
         LPDOMAIN lpDomain = zGETPTR( lpViewAttrib->hDomain );

         // "KZOEE355 - Table_Handler invalid for this Domain Type "
         sz[ 0 ] = cDomainType;
         sz[ 1 ] = 0;
         fnIssueCoreError( lpCurrentTask, lpView, 8, 355, 0, sz, lpDomain->szName );
         nRC = zCALL_ERROR;
         break;
      }

      default:
         // "KZOEE244 - Attribute has no Domain"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 244, 0, cpcEntityName, cpcAttributeName );
         nRC = zCALL_ERROR;
   }

   fnOperationReturn( iGetNextTableEntryForAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetAttributeValueNext
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SetAttributeValueNext
//
// PURPOSE:  To set the value of an attribute to the 'next' value
//           given the current value of the attribute.  For this function
//           to work there must be a domain associated with the attribute
//           that knows how to adjust the value of the attribute.
//           Domains that are of type 'table' will have their value
//           adjusted to the next table entry.  If the current value
//           is the last table entry, then the value is set to the value
//           of the first table entry.
//
// PARAMETERS:
//           lpView          - View to Object instance
//           cpcEntityName    - Entity name containing the attribute to
//                             change.
//           cpcAttributeName - Attribute name to change.
//           cpcContextName   - Optional context name, used in conjunction
//                             with the domain associated with the
//                             specified attribute.
//
// RETURNS:    0 - Attribute value has changed
//            -1 - Attribute value has not changed
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetAttributeValueNext( zVIEW   lpView,
                       zCPCHAR cpcEntityName,
                       zCPCHAR cpcAttributeName,
                       zCPCHAR cpcContextName )
{
   return( fnSetAttributeValue( lpView, cpcEntityName, cpcAttributeName,
                                cpcContextName, iSetAttributeValueNext ) );
}

//./ ADD NAME=SetAttributeValuePrev
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SetAttributeValuePrev
//
// PURPOSE:  To set the value of an attribute to the 'previous' value
//           given the current value of the attribute.  For this function
//           to work there must be a domain associated with the attribute
//           that knows how to adjust the value of the attribute.
//           Domains that are of type 'table' will have their value
//           adjusted to the previous table entry.  If the current value
//           is the first table entry, then the value is set to the value
//           of the last table entry.
//
// PARAMETERS:
//           lpView          - View to Object instance
//           cpcEntityName    - Entity name containing the attribute to
//                             change.
//           cpcAttributeName - Attribute name to change.
//           cpcContextName   - Optional context name, used in conjunction
//                             with the domain associated with the
//                             specified attribute.
//
// RETURNS:    0 - Attribute value has changed
//            -1 - Attribute value has not changed
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetAttributeValuePrev( zVIEW   lpView,
                       zCPCHAR cpcEntityName,
                       zCPCHAR cpcAttributeName,
                       zCPCHAR cpcContextName )
{
   return( fnSetAttributeValue( lpView, cpcEntityName, cpcAttributeName,
                                cpcContextName, iSetAttributeValuePrev ) );
}

//./ ADD NAME=StoreValueInRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   StoreValueInRecord
//
// PURPOSE: Used to store the attribute value into the Zeidon record.
//          Assumption is that lpValue points to the same data type as the
//          attribute, namely, lpViewAttribute->cType.
//          Used by derived attribute, db handler, domain, entity
//          contraint and object constraint operations.
//
// PARAMETERS: lpView
//             lpViewEntity
//             lpViewAttribute
//             lpValue
//             ulLength       - For blobs only, the length of the blob
//
// RETURNS:    0 - Attribute value has been set
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
StoreValueInRecord( zVIEW        lpView,
                    LPVIEWENTITY lpViewEntity,
                    LPVIEWATTRIB lpViewAttrib,
                    zPVOID       lpValue,
                    zULONG       ulLength )
{
   LPTASK            lpCurrentTask;
   LPTASK            lpAllocTask;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITY      lpViewEntityTemp;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchRecord;
   zULONG            ulLth;
   zPCHAR            pchAttribValue;
   zBOOL             bUpdated;
   zSHORT            nRC;

   lpCurrentTask = fnOperationCall( iStoreValueInRecord, lpView, 0 );

   // We have already validated the ViewEntity prior to invoking the
   // Domain function.  We are doing the call again so that we may
   // easily retrieve the lpViewEntityCsr.
   lpViewEntityTemp = fnValidViewEntity( &lpViewEntityCsr, lpView,
                                         lpViewEntity->szName, 0 );

   lpViewCsr    = zGETPTR( lpView->hViewCsr );
   lpViewOI     = zGETPTR( lpViewCsr->hViewOI );
   lpAllocTask  = zGETPTR( lpViewOI->hAllocTask );

   // Get Entity Instance pointer
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // If the entity instance containing the attribute in which the value
   // is to be stored is null, then return an error message.
   if ( lpEntityInstance == 0 )
   {
      // "KZOEE253 - Invalid Operation call "
      fnIssueCoreError( lpCurrentTask, lpView, 8, 253, 0, 0, 0 );
      fnOperationReturn( iStoreValueInRecord, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpEntityInstance == UNSET_CSR )
      lpEntityInstance = fnEstablishCursorForView( lpViewEntityCsr );

   // Get record to store into.
   if ( lpViewAttrib->bPersist ) // is attribute persistent
   {
      pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
      if ( pchRecord == 0 )
      {
         zPVOID hndl = fnCreateAttributeRecord( lpAllocTask,
                                                lpViewEntity,
                                                lpEntityInstance );
         if ( hndl == 0 || hndl != lpEntityInstance->hPersistRecord  )
         {
            fnOperationReturn( iStoreValueInRecord, lpCurrentTask );
            return( zCALL_ERROR );
         }

         pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord );
      }
   }
   else
   {
      pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord );
      if ( pchRecord == 0 )
      {
         lpEntityInstance->hNonPersistRecord =
                     fnCreateAttributeRecord( lpAllocTask, lpViewEntity, 0 );
         pchRecord = (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord );
         if ( pchRecord == 0 )
         {
            fnOperationReturn( iStoreValueInRecord, lpCurrentTask );
            return( zCALL_ERROR );
         }
      }
   }

   // Set attrib from string.
   nRC = 0;
   bUpdated = FALSE;

   switch ( lpViewAttrib->cType )
   {
      case zTYPE_STRING:
      {
         zSHORT k;

         if ( lpValue )
            pchAttribValue = (zPCHAR) lpValue;
         else
            pchAttribValue = szNullS;

         ulLth = zstrlen( pchAttribValue );

         // Truncate string if necessary.
         if ( ulLth >= lpViewAttrib->ulLth )
            ulLth = lpViewAttrib->ulLth - 1;     // subtract 1 for null

         k = fnStoreStringInRecord( lpAllocTask, // so allocation can be done
                                    pchRecord,       // Attribute record
                                    lpViewAttrib->ulRecordOffset,   // Offset
                                    pchAttribValue,  // Attribute value to store
                                    ulLth,           // Maximum length to store
                                    1 );            // Null terminate data
         // Check return code.
         if ( k )
         {
            if ( k == 1 )
               bUpdated = TRUE;
            else
               nRC = k;
         }

         break;
      }

      case zTYPE_INTEGER:
      {
         zPLONG   lplLong;
         zPLONG   lplLongInput;

         lplLongInput = (zPLONG) lpValue;
         lplLong = (zPLONG) (pchRecord + lpViewAttrib->ulRecordOffset);
         if ( *lplLongInput == lNullInteger )
         {
            if ( *lplLong != lNullInteger )
            {
               *lplLong = lNullInteger;
               bUpdated = TRUE;
            }
         }
         else
         if ( *lplLong != *lplLongInput )
         {
            *lplLong = *lplLongInput;
            bUpdated = TRUE;
         }

         break;
      }

      case zTYPE_DECIMAL:
      {
         zPDECIMAL lpdDecimal;
         zPDECIMAL lpdDecimalInput;

         lpdDecimalInput = (zPDECIMAL) lpValue;
         lpdDecimal = (zPDECIMAL) (pchRecord + lpViewAttrib->ulRecordOffset);
         if ( SysCompareDecimalToNull( lpdDecimalInput ) == 0 )
         {
            if ( SysCompareDecimalToNull( lpdDecimal ) != 0 )
            {
               SysAssignDecimalFromNull( lpdDecimal );
               bUpdated = TRUE;
            }
         }
         else
         if ( SysCompareDecimalToDecimal( lpdDecimal,
                                          lpdDecimalInput ) != 0 )
         {
            SysAssignDecimalFromDecimal( lpdDecimal, lpdDecimalInput );
            bUpdated = TRUE;
         }

         break;
      }

      case zTYPE_BLOB:
      {
         zSHORT k;

         k = fnStoreBlobInRecord( lpAllocTask, // so allocation can be done
                                  pchRecord,       // Attribute record
                                  lpViewAttrib->ulRecordOffset, // Offset
                                  lpValue,        // Blob value to store
                                  ulLength );      // Length to store
         // Check return code
         if ( k )
         {
            if ( k == 1 )
               bUpdated = TRUE;
            else
               nRC = k;
         }

         break;
      }

      case zTYPE_DATETIME:
      {
         LPDTINTERNAL  lpDateTime;
         LPDTINTERNAL  lpDateTimeInput;

         lpDateTimeInput = (LPDTINTERNAL) lpValue;
         lpDateTime = (LPDTINTERNAL)
                              (pchRecord + lpViewAttrib->ulRecordOffset);
         if ( lpDateTimeInput->ulDateMinutes == (zULONG) lNullInteger )
         {
            if ( lpDateTime->ulDateMinutes != (zULONG) lNullInteger )
            {
               lpDateTime->ulDateMinutes = (zULONG) lNullInteger;
               lpDateTime->usTSeconds = 0;
               bUpdated = TRUE;
            }
         }
         else
         if ( zmemcmp( lpDateTime, lpDateTimeInput,
                       sizeof( DateTimeInternalRecord ) ) != 0 )
         {
            zmemcpy( lpDateTime, lpDateTimeInput,
                     sizeof( DateTimeInternalRecord ) );
            bUpdated = TRUE;
         }

         break;
      }

      default:
         nRC = zCALL_ERROR;
   }

   // Set update indicator.
   fnSetUpdateIndicator( lpViewOI, lpEntityInstance, lpViewAttrib,
                         bUpdated, TRUE, TRUE );

   fnOperationReturn( iStoreValueInRecord, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=StoreBytesInRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   StoreBytesInRecord
//
// PURPOSE: Used to store bytes into the attribute record.
//          Assumption is that lpValue points to the same data type as the
//          attribute, namely, lpViewAttrib->cType.
//          Does NO VALIDATION.  Used by db handler for quickest setting of
//          attributes.
//
// PARAMETERS: lpView
//             lpViewAttrib
//             lpValue
//             sLength         - For blobs and strings only.
//             pchRecord       - pchRecord for persist attributes.
//
// RETURNS:    0 - Attribute value has been set
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
StoreBytesInRecord( zVIEW        lpView,
                    LPVIEWATTRIB lpViewAttrib,
                    zPVOID       lpValue,
                    zULONG       ulLength,
                    zPCHAR       pchRecord )
{
   LPVIEWCSR lpViewCsr = zGETPTR( lpView->hViewCsr );
   LPVIEWOI  lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   LPTASK    lpAllocTask = zGETPTR( lpViewOI->hAllocTask );

   switch ( lpViewAttrib->cType )
   {
      case zTYPE_STRING:
      {
         zPCHAR pchAttribValue;

         if ( lpValue )
            pchAttribValue = (zPCHAR) lpValue;
         else
            pchAttribValue = szNullS;

         if ( ulLength == 0 )
            ulLength = zstrlen( pchAttribValue );

         // truncate string if necessary
         if ( ulLength >= lpViewAttrib->ulLth )
            ulLength = lpViewAttrib->ulLth - 1;  // subtract 1 for null

         fnStoreStringInRecord( lpAllocTask,  // so allocation can be done
                                pchRecord,       // Attribute record
                                lpViewAttrib->ulRecordOffset,   // Offset
                                pchAttribValue,  // Attribute value to store
                                ulLength,        // Maximum length to store
                                1 );             // Null terminate data
         break;
      }

      case zTYPE_INTEGER:
      {
         zPLONG   lplLong;

         lplLong = (zPLONG) (pchRecord + lpViewAttrib->ulRecordOffset);
         *lplLong = *((zPLONG) lpValue);

         break;
      }

      case zTYPE_DECIMAL:
      {
         zPDECIMAL lpdDecimal;
#if 0
         lpdDecimalInput = (zPDECIMAL) lpValue;
         lpdDecimal = (zPDECIMAL) (pchRecord + lpViewAttrib->ulRecordOffset);
         SysAssignDecimalFromDecimal( lpdDecimal, (zPDECIMAL) lpValue );
#else
         lpdDecimal = (zPDECIMAL) (pchRecord + lpViewAttrib->ulRecordOffset);
         zmemcpy( lpdDecimal, lpValue, sizeof( zDECIMAL ) );
#endif

         break;
      }

      case zTYPE_BLOB:
      {
         fnStoreBlobInRecord( lpAllocTask,  // so allocation can be done
                              pchRecord,       // Attribute record
                              lpViewAttrib->ulRecordOffset, // Offset
                              lpValue,        // Blob value to store
                              ulLength );      // Length to store
         break;
      }

      case zTYPE_DATETIME:
      {
         LPDTINTERNAL  lpDateTime;

         lpDateTime = (LPDTINTERNAL) (pchRecord + lpViewAttrib->ulRecordOffset);
         zmemcpy( lpDateTime, lpValue, sizeof( DateTimeInternalRecord ) );

         break;
      }

   } // switch...

   return( 0 );
}

//./ ADD NAME=StoreValueInDerivedAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   StoreValueInDerivedAttribute
//
// PURPOSE: A wrapper function that allows VML to call StoreValueInRecord
//          using entity and attribute NAMES instead of lpViewEntity and
//          lpViewAttrib.
//
// PARAMETERS: lpView
//             EntityName
//             AttributeName
//             lpValue
//             sLength         - For blobs only, the length of the blob
//
// RETURNS:    0 - Attribute value has been set
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
StoreValueInDerivedAttribute( zVIEW        lpView,
                              zPCHAR       pchEntityName,
                              zPCHAR       pchAttribName,
                              zPVOID       lpValue,
                              zULONG       ulLength )
{
   LPTASK          lpCurrentTask;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWATTRIB    lpViewAttrib;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zSHORT          nRC;

   lpCurrentTask = fnOperationCall( iStoreValueInDerivedAttribute, lpView, 0 );

   lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpView, pchEntityName, 0 );
   if ( lpViewEntity == 0 )
   {
      fnOperationReturn( iStoreValueInDerivedAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity, pchAttribName, 0 );
   if ( lpViewAttrib == 0 )
   {
      fnOperationReturn( iStoreValueInDerivedAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = StoreValueInRecord( lpView, lpViewEntity, lpViewAttrib,
                             lpValue, ulLength );

   fnOperationReturn( iStoreValueInDerivedAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=StoreStringInDerivedAttribute
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   StoreStringInDerivedAttribute
//
// PURPOSE: A wrapper function that allows VML to call StoreStringInRecord
//          using entity and attribute NAMES instead of lpViewEntity and
//          lpViewAttrib.
//
// PARAMETERS: lpView
//             EntityName
//             AttributeName
//             lpValue
//             sLength         - For blobs only, the length of the blob
//
// RETURNS:    0 - Attribute value has been set
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
StoreStringInDerivedAttribute( zVIEW        lpView,
                               zPCHAR       pchEntityName,
                               zPCHAR       pchAttribName,
                               zPCHAR       pchStr )
{
   LPTASK          lpCurrentTask;
   LPVIEWENTITY    lpViewEntity;
   LPVIEWATTRIB    lpViewAttrib;
   LPVIEWENTITYCSR lpViewEntityCsr;
   zSHORT          nRC;

   lpCurrentTask = fnOperationCall( iStoreStringInDerivedAttribute, lpView, 0 );

   lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpView, pchEntityName, 0 );
   if ( lpViewEntity == 0 )
   {
      fnOperationReturn( iStoreStringInDerivedAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity, pchAttribName, 0 );
   if ( lpViewAttrib == 0 )
   {
      fnOperationReturn( iStoreStringInDerivedAttribute, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = StoreStringInRecord( lpView, lpViewEntity, lpViewAttrib, pchStr );

   fnOperationReturn( iStoreStringInDerivedAttribute, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetValueFromRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   GetValueFromRecord
//
// PURPOSE:
//          Used to get a pointer to the attribute  value in the
//          Zeidon record.
//          Used by derived attribute, db handler, domain, entity
//          contraint and object constraint operations.
//
// PARAMETERS: lpView
//             lpViewEntity
//             lpViewAttribute
//             lpReturnValue
//             cReturnType
//             pulReturnLth
//
// RETURNS:    0 - Attribute pointer has been returned
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT OPERATION
GetValueFromRecord( zVIEW        lpView,
                    LPVIEWENTITY lpViewEntity,
                    LPVIEWATTRIB lpViewAttribute,
                    zCOREMEM     lpReturnAddr,
                    zPCHAR       pchReturnType,
                    zPULONG      pulReturnLth )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITY      lpViewEntityTemp;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPENTITYINSTANCE  lpEntityInstance;
   zPCHAR            pchRecord;
   zSHORT            nRC;

   lpCurrentTask = fnOperationCall( iGetValueFromRecord, lpView, 0 );

#if 0
   // This check is done to ensure that the invocation of the operation
   // is from a domain or database handler function.  No outsiders allowed!
   if ( lpCurrentTask->nOperIdx < 1 )
   {
      // "KZOEE020 - Invalid Operation call "
      fnIssueCoreError( lpCurrentTask, lpView, 8, 20, 0, 0, 0 );
      fnOperationReturn( iGetValueFromRecord, lpCurrentTask );
      return( zCALL_ERROR );
   }
#endif

   // We have already validated the ViewEntity prior to invoking the
   // Domain function.  We are doing the call again so that we may
   // easily retrieve the lpViewEntityCsr.
   lpViewEntityTemp = fnValidViewEntity( &lpViewEntityCsr, lpView,
                                         lpViewEntity->szName, 0 );

   lpViewCsr    = zGETPTR( lpView->hViewCsr );
   lpViewOI     = zGETPTR( lpViewCsr->hViewOI );

   // Get Entity Instance pointer.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Get record to retrieve from.
   pchRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttribute );

   nRC = 0;
   *pchReturnType = lpViewAttribute->cType; // return internal type
   if ( pchRecord == 0 )
   {
      *lpReturnAddr = 0;
      *pulReturnLth = 0;
   }
   else
   {
      switch ( lpViewAttribute->cType )
      {
         case zTYPE_STRING:
         {
            zPCHAR   pchRecordString;
            zCOREMEM lpExternalP;
            zPULONG  pulLth;

            pchRecordString = pchRecord + lpViewAttribute->ulRecordOffset;
            if ( pchRecordString[ 0 ] == (zCHAR) '\xff' )
            {
               lpExternalP = (zCOREMEM) (pchRecordString + 1);
               pulLth = (zPULONG) (lpExternalP + 1);
               *pulReturnLth = *pulLth;
               pchRecordString = zGETPTR( *lpExternalP );
               *lpReturnAddr = pchRecordString;
            }
            else
            {
               *lpReturnAddr = pchRecordString;
               *pulReturnLth = zstrlen( pchRecordString ) + 1;
            }

            break;
         }

         case zTYPE_INTEGER:
            *lpReturnAddr = (pchRecord + lpViewAttribute->ulRecordOffset);
            *pulReturnLth = sizeof( zLONG );
            break;

         case zTYPE_DECIMAL:
            *lpReturnAddr = (pchRecord + lpViewAttribute->ulRecordOffset);
            *pulReturnLth = sizeof( zDECIMAL );
            break;

         case zTYPE_BLOB:
         {
            zPCHAR   pchBlob;
            zCOREMEM lpExternalP;
            zPULONG pulLth;

            pchBlob = pchRecord + lpViewAttribute->ulRecordOffset;
            if ( pchBlob[ 0 ] == (zCHAR) '\xff' )
            {
               lpExternalP = (zCOREMEM) (pchBlob + 1);
               pulLth = (zPULONG) (lpExternalP + 1);
               *pulReturnLth = *pulLth;
               pchBlob = zGETPTR( *lpExternalP );
               *lpReturnAddr = pchBlob;
            }
            else
            {
               *lpReturnAddr = 0;
               *pulReturnLth = 0;
            }

            break;
         }

         case zTYPE_DATETIME:
            *lpReturnAddr = (pchRecord + lpViewAttribute->ulRecordOffset);
            *pulReturnLth = sizeof( DateTimeRecord );
            break;

         default:
            nRC = zCALL_ERROR;
      }
   }

   fnOperationReturn( iGetValueFromRecord, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=StoreStringInRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   StoreStringInRecord
//
// PURPOSE: Used to set the attribute value using string input.
//          Used by derived attribute, db handler, domain, entity
//          contraint and object constraint operations.
//
// PARAMETERS: lpView          - View to the object instance.
//             lpViewEntity    - ViewEntity structure
//             lpViewAttribute - ViewAttribute structure
//             cpcValue        - String input used to set the attribute
//
// RETURNS:    0 - Attribute value set
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
StoreStringInRecord( zVIEW        lpView,
                     LPVIEWENTITY lpViewEntity,
                     LPVIEWATTRIB lpViewAttribute,
                     zCPCHAR      cpcValue )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITY      lpViewEntityTemp;
   zSHORT            nRC;

   lpCurrentTask = fnOperationCall( iStoreStringInRecord, lpView, 0 );

   // We have already validated the ViewEntity prior to invoking the
   // Domain function.  We are doing the call again so that we may
   // easily retrieve the lpViewEntityCsr.
   lpViewEntityTemp = fnValidViewEntity( &lpViewEntityCsr, lpView,
                                         lpViewEntity->szName, 0 );
   nRC = fnSetAttributeFromString( lpView, lpViewEntityCsr, lpViewAttribute,
                                   cpcValue, 0 );

   fnOperationReturn( iStoreStringInRecord, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetStringFromRecord
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   GetStringFromRecord
//
// PURPOSE: Used to retrieve the attribute value represented as a string.
//          Used by derived attribute, db handler, domain, entity
//          contraint and object constraint operations.
//
// PARAMETERS: lpView
//             lpViewEntity
//             lpViewAttribute
//             pchReturnValue
//             uMaxLength
//
// RETURNS:    0 - Attribute value retrieved
//            -1 - null string returned, attribute value is NULL
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
GetStringFromRecord( zVIEW        lpView,
                     LPVIEWENTITY lpViewEntity,
                     LPVIEWATTRIB lpViewAttribute,
                     zPCHAR       pchReturnString,
                     zULONG       uMaxLength )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITY      lpViewEntityTemp;
   zSHORT            nRC;

   if ( fnValidateInstanceParameters( &lpCurrentTask,
                                      &lpViewEntityTemp,
                                      &lpViewEntityCsr,
                                      iGetStringFromRecord,
                                      lpView,
                                      lpViewEntity->szName, 0 ) )
   {
      return( zCALL_ERROR );
   }

   nRC = fnGetStringFromAttribute( pchReturnString, uMaxLength, lpView,
                                   lpViewEntityCsr, lpViewAttribute, 0 );

   fnOperationReturn( iGetStringFromRecord, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetAttributeValue
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnSetAttributeValue
//
// PURPOSE: Called by SetAttributeValue Next/Prev
//          value into the Zeidon record.
//
// PARAMETERS: lpView
//             cpcEntityName
//             cpcAttributeName
//             cpcContextName
//             iOperationID
//
// RETURNS:    0 - Attribute value has changed
//            -1 - Attribute value has not changed
//   zCALL_ERROR - Error in call
//
// NOTE:    A work in progress...
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnSetAttributeValue( zVIEW   lpView,
                     zCPCHAR cpcEntityName,
                     zCPCHAR cpcAttributeName,
                     zCPCHAR cpcContextName,
                     zSHORT  iOperationID )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zPVOID            lpAttrValue;
   LPTASKDOMAIN      lpTaskDomain;
   LPDOMAIN          lpDomain;
   zCHAR             cDomainType;
   zLONG             lDomainEntryType;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iOperationID, // next prev
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
   {
      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpViewAttrib->hDomain == 0 )
   {
      // "KZOEE244 - Attribute has no Domain"
//    fnIssueCoreError( lpTask, lpView, 8, 244, 0, cpcEntityName, cpcAttributeName );
      fnOperationReturn( iOperationID, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpDomain = zGETPTR( lpViewAttrib->hDomain );

   fnGetAddrForAttribute( (zCOREMEM) &lpAttrValue, lpView, lpViewEntityCsr, 0,
                          lpViewAttrib );
   if ( lpViewAttrib->cType == zTYPE_STRING )
   {
      if ( *((zPCHAR) lpAttrValue) == 0 )
      {
         // Attribute value is null, it cannot be set to prev/next value
         fnOperationReturn( iOperationID, lpCurrentTask );
         return( -1 );
      }
   }
   else
   {
      // we must be working with an integer type if it isn't a string
      if ( *((zPLONG) lpAttrValue) == lNullInteger )
      {
         // Attribute value is null, it cannot be set to prev/next value
         fnOperationReturn( iOperationID, lpCurrentTask );
         return( -1 );
      }
   }

   // Get view Entity from cursor.
   lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );

   lDomainEntryType = (iOperationID == iSetAttributeValueNext) ?
      zDME_SET_ATTRIBUTE_VALUE_NEXT : zDME_SET_ATTRIBUTE_VALUE_PREV ;

   cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                             lpCurrentTask, lpDomain );
   switch ( cDomainType )
   {
      case zDM_TYPE_TABLE:
         nRC = Table_Handler( lDomainEntryType,
                              0,
                              0,
                              cpcContextName,
                              lpView,
                              lpViewEntity,
                              lpViewAttrib,
                              0,
                              (zPVOID) 0 );
         break;

      case zDM_TYPE_EXPRESSION:
      case zDM_TYPE_ALGORITHM:
         nRC = fnCallDomainOper( lpCurrentTask,
                                 lpTaskDomain,
                                 lDomainEntryType,
                                 0,
                                 0,
                                 cpcContextName,
                                 lpView,
                                 lpViewEntity,
                                 lpViewAttrib,
                                 0 );
         if ( nRC == zDME_NOT_HANDLED )
         {
            zCHAR szMsg[ 500 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Operation '%s' does not handle an Entry "
                       "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                       lDomainEntryType, lpDomain->szName );
            MessageSend( lpView, "TZDMD101", "System Error",
                         szMsg, zMSGQ_DOMAIN_ERROR, 0 );
            fnOperationReturn( iOperationID, lpCurrentTask );
            return( zCALL_ERROR );
         }

         break;

      case zDM_TYPE_FORMAT:
      default:
         nRC = -1;
   }

   fnOperationReturn( iOperationID, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnAttributeValueNull
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnAttributeValueNull
//
// PURPOSE: to determine if an attribute contains a null value
//
// PARAMETERS: lpView
//             lpViewEntityCsr
//             lpViewAttrib
//
// RETURNS:    1 - Attribute value is null
//             0 - Attribute value is not null
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT
fnAttributeValueNull( zVIEW            lpView,
                      LPENTITYINSTANCE lpEntityInstance,
                      LPVIEWATTRIB     lpViewAttrib )
{
   zPVOID   lpAttrValue;

   fnGetAddrForAttribute( (zCOREMEM) &lpAttrValue, lpView, 0, lpEntityInstance,
                          lpViewAttrib );
   switch ( lpViewAttrib->cType )
   {
      case zTYPE_INTEGER:
         return( (*((zPLONG) lpAttrValue) == lNullInteger) ? 1 : 0 );

      case zTYPE_DECIMAL:
         return( SysCompareDecimalToNull( (zPDECIMAL) lpAttrValue ) == 0 ? 1 : 0 );

      case zTYPE_DATETIME:
         return( (fnCompareDateTimeToDateTime( (LPDTINTERNAL) lpAttrValue,
                             (LPDTINTERNAL) &stNullDateTime ) == 0 ) ? 1 : 0 );

      case zTYPE_STRING:
         return( (*((zPCHAR) lpAttrValue) == 0) ? 1 : 0 );

      case zTYPE_BLOB:
         return( (lpAttrValue == 0) ? 1 : 0 );

      default:
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

         // "KZOEE246 - Invalid Attribute type "
         fnIssueCoreError( lpTask, lpView, 8, 246, 0,
                           lpViewEntity->szName, lpViewAttrib->szName );
         return( zCALL_ERROR );
      }
   }
}

#define IDM_GETTRACELOGFILE             216

//./ ADD NAME=fnRecordForEntityAttr
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnRecordForEntityAttr
//
// PURPOSE: Return the address of the Persistent or NonPersistent record
//          depending on the persist flag in the attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zPCHAR
fnRecordForEntityAttr( LPENTITYINSTANCE   lpEntityInstance,
                       LPVIEWATTRIB       lpViewAttribute )
{
   // Get record to retrieve from.
   if ( lpViewAttribute->bPersist ) // is attribute persistent
   {
#if 0
      if ( SysSendOE_Message( IDM_GETTRACELOGFILE, 0 ) != -1 )
      {
         LPVIEWENTITY  lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

         if ( zstrcmp( lpViewEntity->szName, "StudentAccountProfile" ) == 0 &&
              zstrcmp( lpViewAttribute->szName, "wTotalInterestToApply" ) == 0 )
         {
            TraceLine( "RecordForEntityAttr %s.%s: 0x%08x",
                       lpViewEntity->szName, lpViewAttribute->szName,
                       lpEntityInstance->hPersistRecord );
         }
      }
#endif
      return( (zPCHAR) zGETPTR( lpEntityInstance->hPersistRecord ) );
   }
   else
   {
#if 0
      if ( SysSendOE_Message( IDM_GETTRACELOGFILE, 0 ) != -1 )
      {
         LPVIEWENTITY  lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

         if ( zstrcmp( lpViewEntity->szName, "StudentAccountProfile" ) == 0 &&
              zstrcmp( lpViewAttribute->szName, "wTotalInterestToApply" ) == 0 )
         {
            TraceLine( "RecordForEntityAttr (non-persist) %s.%s: 0x%08x",
                       lpViewEntity->szName, lpViewAttribute->szName,
                       lpEntityInstance->hNonPersistRecord );
         }
      }
#endif
      return( (zPCHAR) zGETPTR( lpEntityInstance->hNonPersistRecord ) );
   }
}

//./ ADD NAME=fnInvokeDerivedOperation
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnInvokeDerivedOperation
//
// PURPOSE: Invoke the derived operation for a given attribute
//
// RETURNS:    0 - Attribute value has been established
//   zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnInvokeDerivedOperation( zVIEW        lpView,
                          LPVIEWENTITY lpViewEntity,
                          LPVIEWATTRIB lpViewAttrib,
                          LPTASK       lpCurrentTask,
                          zSHORT       nControl )
{
   zPOBJOPER pfObjectOper = fnGetAttribOper( lpView, lpViewAttrib,
                                             lpCurrentTask );
   if ( pfObjectOper )
      return( (*pfObjectOper)( lpView, lpViewEntity, lpViewAttrib, nControl ) );

   return( 0 );
}

zSHORT LOCALOPER
fnSetBlobFromFile( zVIEW           lpView,
                   LPVIEWENTITYCSR lpViewEntityCsr,
                   LPVIEWATTRIB    lpViewAttrib,
                   zCPCHAR         cpcFileName )
{
   zLONG   hFile = -1;
   zULONG  ulLth;
   zPCHAR  pchData = 0;
   zSHORT  nRC = zCALL_ERROR;

   hFile = SysOpenFile( lpView, cpcFileName, COREFILE_READ );
   if ( hFile == -1 )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      // "KZOEE024190 - Error opening file"
      fnIssueCoreError( lpTask, lpView, 16, 24, 0, cpcFileName, 0 );
      goto EndOfFunction;
   }

   // Get the file length.
   ulLth = SysGetFileSize( hFile );

   // Now allocate space big enough to hold the entire file plus a null-terminator.
   pchData = SysMalloc( ulLth + 1 );
   if ( pchData == 0 )
      goto EndOfFunction;

   if ( SysReadFile( lpView, hFile, pchData, ulLth ) != ulLth )
   {
      TraceLineI( "(at) Couldn't read entire file.  Lth = ", ulLth );
      goto EndOfFunction;
   }

   // Last thing to do--set blob attribute.
   if ( lpViewAttrib->cType == zTYPE_STRING )
   {
      pchData[ ulLth ] = 0;

      nRC = fnSetAttributeFromString( lpView, lpViewEntityCsr,
                                      lpViewAttrib, pchData, (zSHORT) ulLth );
   }
   else
   {
      nRC = fnSetAttributeFromBlob( lpView, lpViewEntityCsr,
                                    lpViewAttrib, pchData, ulLth );
   }

EndOfFunction:

   if ( pchData )
      SysFree( pchData );

   if ( hFile != -1 )
      SysCloseFile( lpView, hFile, 0 );

   return( nRC );
}

//./ ADD NAME=SetBlobFromFile
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetBlobFromFile
//
//  PURPOSE:    To assign a blob or string attribute from a file.
//
//  PARAMETERS: lpView           - View to Object instance
//              cpcEntityName    - Entity containing attribute to retrieve
//              cpcAttributeName - Attribute to set
//              cpcFileName      - Name of file.
//
//  RETURNS:    0           - Attribute successfully set
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetBlobFromFile( zVIEW   lpView,
                 zCPCHAR cpcEntityName,
                 zCPCHAR cpcAttributeName,
                 zCPCHAR cpcFileName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iSetBlobFromFile,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
   {
      fnOperationReturn( iSetBlobFromFile, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpViewAttrib->cType != zTYPE_BLOB &&
        lpViewAttrib->cType != zTYPE_STRING )
   {
      // "KZOEE191 - Attribute must be a Blob or String for this operation."
      fnIssueCoreError( lpCurrentTask, lpView, 16, 191, 0, cpcFileName, 0 );
      fnOperationReturn( iSetBlobFromFile, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Call fnSetBlobFromFile to do the setting
   nRC = fnSetBlobFromFile( lpView, lpViewEntityCsr,
                            lpViewAttrib, cpcFileName );

   fnOperationReturn( iSetBlobFromFile, lpCurrentTask );
   return( nRC );
}

zSHORT LOCALOPER
fnWriteBlobToFile( zVIEW           lpView,
                   LPVIEWENTITYCSR lpViewEntityCsr,
                   LPVIEWATTRIB    lpViewAttrib,
                   zCPCHAR         cpcFileName )
{
   zLONG   hFile = -1;
   zULONG  ulLth;
   zPCHAR  pchData;
   zSHORT  nRC;

   fnGetAddrForAttribute( &pchData, lpView, lpViewEntityCsr, 0,
                          lpViewAttrib );

   fnGetAttributeLength( &ulLth, lpView,
                         zGETPTR( lpViewEntityCsr->hEntityInstance ),
                         lpViewAttrib );

   // If there is no blob data then return -1.
   if ( pchData == 0 || ulLth == 0 )
      return( -1 );

   // Write the blob to a temp file.
   hFile = SysOpenFile( lpView, cpcFileName, COREFILE_WRITE );
   if ( hFile == -1 )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );

      // "KZOEE022 - Error writing file"
      fnIssueCoreError( lpTask, lpView, 16, 22, 0, cpcFileName, 0 );
      return( zCALL_ERROR );
   }

   if ( SysWriteFile( lpView, hFile, pchData, ulLth ) != (int) ulLth )
      nRC = zCALL_ERROR;
   else
      nRC = 0;

   SysCloseFile( lpView, hFile, 0 );
   hFile = -1;

   return( nRC );
}

//./ ADD NAME=WriteBlobToFile
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WriteBlobToFile
//
//  PURPOSE:    Writes a blob to a file.
//
//  PARAMETERS: lpView          - View to Object instance
//              cpcEntityName    - Entity containing blob to write.
//              cpcAttributeName - Blob to write.
//              cpcFileName     - Name of file.
//
//  RETURNS:    0           - Attribute successfully written.
//              -1          - Attribute is null or empty.  File not written.
//              zCALL_ERROR - Error updating attribute
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
WriteBlobToFile( zVIEW   lpView,
                 zCPCHAR cpcEntityName,
                 zCPCHAR cpcAttributeName,
                 zCPCHAR cpcFileName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWATTRIB      lpViewAttrib;
   zSHORT            nRC;

   // Validate parameters
   nRC = fnValidateAttributeParameters( &lpCurrentTask,
                                        &lpViewEntity,
                                        &lpViewEntityCsr,
                                        &lpViewAttrib,
                                        iWriteBlobToFile,
                                        1,   // We intend to update
                                        lpView,
                                        cpcEntityName,
                                        cpcAttributeName, 0 );
   if ( nRC > 0 )
      return( zCALL_ERROR );   // don't call fnOperationReturn

   if ( nRC < 0 )
   {
      fnOperationReturn( iWriteBlobToFile, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lpViewAttrib->cType != zTYPE_BLOB )
   {
      // "KZOEE191 - Attribute must be a Blob for this operation."
      fnIssueCoreError( lpCurrentTask, lpView, 16, 191, 0, cpcFileName, 0 );
      fnOperationReturn( iWriteBlobToFile, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Call fnWriteBlobToFile to do the setting
   nRC = fnWriteBlobToFile( lpView, lpViewEntityCsr,
                            lpViewAttrib, cpcFileName );

   fnOperationReturn( iWriteBlobToFile, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetUpdateIndicator
// Source Module=kzoeataa.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   fnSetUpdateIndicator
//
// PURPOSE: Set update flags as needed in the entity instance.  Will scan
//          through the lpNextLinked chain, if present.
//
// ARGUMENTS:
//          lpViewOI         - OI that has been updated.  May be NULL.
//          lpEntityInstance - EI that has been updated.
//          lpViewAttrib     - Attribute that has been changed.  May be NULL.
//          bUpdated         - If TRUE then turn Updated flags on.  If FALSE
//                             then leave them be.
//          bSetLinked       - Set updated flag for linked EIs, if they exist.
//          bUpdateEntity    - Set updated flag for the entity instance.  If
//                             FALSE then we only set the update flag for the
//                             OI.  Used by Exclude/Include entity functions.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
void
fnSetUpdateIndicator( LPVIEWOI         lpViewOI,
                      LPENTITYINSTANCE lpEntityInstance,
                      LPVIEWATTRIB     lpViewAttrib,
                      zBOOL            bUpdated,
                      zBOOL            bSetLinked,
                      zBOOL            bUpdateEntity )
{
   LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
   zBOOL        bPersist;

   // Set update indicator.
   if ( bUpdated == FALSE )
      return;

   if ( lpViewOI == 0 )
      lpViewOI = zGETPTR( lpEntityInstance->hViewOI );

   if ( lpViewEntity->bDerived || lpViewEntity->bDerivedPath )
      bPersist = FALSE;
   else
      bPersist = TRUE;

   if ( lpViewAttrib )
   {
      LPATTRIBFLAGS lpAttribFlags;

      // If changing a derived attribute then nothing gets changed.
      if ( lpViewAttrib->szDerivedOper[ 0 ] )
         return;

      if ( lpViewAttrib->bPersist == FALSE )
      {
         bPersist = FALSE;

         // If the attribute is not persistent, then we don't want to change
         // the updated flags of linked entities.
         bSetLinked = FALSE;
      }

      lpAttribFlags = fnGetAttribFlagsPtr( lpEntityInstance, lpViewAttrib );
      lpAttribFlags->u.bFlags.bUpdated = TRUE;
   }

   // KJS 08/22/14 - I am confused here because fnInstanceVersioned returns 0 if there is no
   // versioning, so it would seem that the check should be > 0.
   //if ( fnInstanceVersioned( lpEntityInstance ) > 0 )
   // KJS 06/25/15 - Not asking for Save in domains when I create a new domain, so going to set this back to original.
   if ( fnInstanceVersioned( lpEntityInstance ) == 0 )
   {
      lpViewOI->bUpdatedFile = TRUE;

      if ( bPersist )
         lpViewOI->bUpdated = TRUE;
   }
   else
   {
      // The entity instance is versioned, set the version updated flags.
      lpEntityInstance->u.nInd.bVersionUpdFile = TRUE;

      if ( bPersist )
         lpEntityInstance->u.nInd.bVersionUpdated = TRUE;
   }

   if ( bUpdateEntity )
   {
      lpEntityInstance->u.nInd.bUpdated = TRUE;

      //*******************************************************************
      //*******************************************************************
      // We shouldn't go through and set the update flags for linked OIs if
      // the lpEntityInstance is a subobject version.  Accept should do it.
      // This should be changed at some point.
      //*******************************************************************
      //*******************************************************************
      if ( bSetLinked && lpEntityInstance->hNextLinked )
      {
         LPENTITYINSTANCE lpLinked;

         for ( lpLinked = zGETPTR( lpEntityInstance->hNextLinked );
               lpLinked != lpEntityInstance;
               lpLinked = zGETPTR( lpLinked->hNextLinked ) )
         {
            LPVIEWENTITY lpLinkedViewEntity = zGETPTR( lpLinked->hViewEntity );

            lpLinked->u.nInd.bUpdated = TRUE;

            // Now make sure the OI for the linked EI gets flagged as updated.
            if ( lpLinkedViewEntity->bDerived == FALSE &&
                 lpLinkedViewEntity->bDerivedPath == FALSE )
            {
               LPVIEWOI lpLinkedOI = zGETPTR( lpLinked->hViewOI );

               lpLinkedOI->bUpdatedFile = lpLinkedOI->bUpdated = TRUE;
            }
         }
      }
   }

} // fnSetUpdateIndicator

/////////////////////////////////////////////////////////////////////////////
//
// Used by derived expressions for the @sum command.
//
/////////////////////////////////////////////////////////////////////////////
zDECIMAL OPERATION
zdSUM( zVIEW   vView,
       zCPCHAR cpcEntityName,
       zCPCHAR cpcAttrName,
       zCPCHAR cpcScopingEntity )
{
   zDECIMAL d;
   zDECIMAL dTotal;
   zSHORT   nRC;

   SysConvertLongToDecimal( 0L, &dTotal );

   CreateViewFromViewForTask( &vView, vView, 0 );

   for ( nRC = SetCursorFirstEntity( vView, cpcEntityName, cpcScopingEntity );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vView, cpcEntityName, cpcScopingEntity ) )
   {
      GetDecimalFromAttribute( &d, vView, cpcEntityName, cpcAttrName );
      SysAddDecimalToDecimal( &dTotal, &dTotal, &d );
   }

   DropView( vView );
   return( dTotal );
}

zLONG OPERATION
ziSUM( zVIEW   vView,
       zCPCHAR cpcEntityName,
       zCPCHAR cpcAttrName,
       zCPCHAR cpcScopingEntity )
{
   zLONG   l;
   zLONG   lTotal = 0;
   zSHORT  nRC;

   CreateViewFromViewForTask( &vView, vView, 0 );

   for ( nRC = SetCursorFirstEntity( vView, cpcEntityName, cpcScopingEntity );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vView, cpcEntityName, cpcScopingEntity ) )
   {
      GetIntegerFromAttribute( &l, vView, cpcEntityName, cpcAttrName );
      lTotal += l;
   }

   DropView( vView );
   return( lTotal );
}
