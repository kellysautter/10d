#include "kzoephdr.h"
//
// MODULE NAME:  KZOECMAA  -  Object services Cursor management Operations
// DESCRIPTION:  This is the source file which contains Object services
//               cursor manipulation Operations and functions
// OPERATIONS:   All OPERATIONS which do cursor stuff
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    4/19/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

/*
CHANGE LOG

2003.10.25  DKS
   When the entity is not specified for SetAllSelectStates, it causes a
   deselect of all SelectedInstances for the current select set for the view.

2003.10.18  DKS
   SetCursor was not checking for CursorTest prior to calling fnCallListeners.

2001.07.31  DGC
   Fix for bug 55527.  SetCursor was not correctly finding null attributes if
   the lpRecord was null.

2001.04.20  DGC
   Added flag to fnValidViewAttrib( ) to allow hidden attributes.

2001.02.08  DGC
   Changed the changing of cursors.  When we reset a parent, the child
   cursors used to be set to UNSET_CSR but now we set them to the first twin.

   Changed cursor listener stuff to use a linked list.

2001.01.28  HH
   Implemented Cursor Listener interface.

2001.01.11  DGC
   Made a change so that we clean up "dead" entity instances at the end of
   fnSetEntityCursor( ).

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

1999.05.25  DGC  10a
   Added GetEntityNameForHierarchicalCsr( ). Changed SetCursorAbsolutePosition( ).

   Made change to better ignore error #358 from
   SetCursorFirst/NextEntityByString( ) in fnSetEntityCsr( ).

1999.04.06  DGC  10a
   Changed domain interface some.

1999.02.26  DGC
   Made change to get rid of MSVC 6.0 compiler warning.

1999.01.06  DGC
   Fixed bug in SetCursorNextHier( ) that didn't correctly handle multi-root OIs.
*/

// CURSOR PERFORMANCE CODE DOESN'T HELP!!!
// #define __CURSOR_PERFORMANCE__
// CURSOR PERFORMANCE CODE DOESN'T HELP!!!

zVOID fnCallListeners( zVIEW lpView, zCPCHAR cpcEntityName );

//./ ADD NAME=SetCursorNextEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorNextEntity
//
//  PURPOSE: To set the cursor to the next entity instance of the entity
//           type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_UNDEFINED     - Scoping level cursor is undefined
//           zCURSOR_NULL          - Entity cursor is null
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position changed within
//                                   current parent
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorNextEntity( zVIEW    lpView,
                     zCPCHAR  cpcEntityName,
                     zCPCHAR  cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorNextEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            0, (zLONG) zPOS_NEXT,
                            0, 0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorNextEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorPrevEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorPrevEntity
//
//  PURPOSE: To set the cursor to the previous entity instance of the
//           entity type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL          - Entity cursor is NULL
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position changed within
//                                   current parent
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorPrevEntity( zVIEW   lpView,
                     zCPCHAR cpcEntityName,
                     zCPCHAR cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorPrevEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            0, (zLONG) zPOS_PREV,
                            0, 0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorPrevEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorScopeOI
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorScopeOI
//
//  PURPOSE: To set the cursor to the first entity instance of the
//           entity type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL - Entity cursor is NULL
//           zCURSOR_SET  - Entity position established on first entity
//           zCALL_ERROR  - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorScopeOI( zVIEW   lpView,
                  zCPCHAR cpcEntityName,
                  zSHORT  nPos )
{
   zSHORT nRC = SetEntityCursor( lpView, cpcEntityName, 0,
                                 nPos | zQUAL_SCOPE_OI,
                                 0, 0, 0, 0, 0, 0 );
   return( nRC );
}

//./ ADD NAME=SetCursorFirstEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorFirstEntity
//
//  PURPOSE: To set the cursor to the first entity instance of the
//           entity type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL - Entity cursor is NULL
//           zCURSOR_SET  - Entity position established on first entity
//           zCALL_ERROR  - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorFirstEntity( zVIEW   lpView,
                      zCPCHAR cpcEntityName,
                      zCPCHAR cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorFirstEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            0, (zLONG) zPOS_FIRST,
                            0, 0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorFirstEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorLastEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorLastEntity
//
//  PURPOSE: To set the cursor to the last entity instance of the entity
//           type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL - Entity cursor is NULL
//           zCURSOR_SET  - Entity position established
//           zCALL_ERROR  - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorLastEntity( zVIEW   lpView,
                     zCPCHAR cpcEntityName,
                     zCPCHAR cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorLastEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            0, (zLONG) zPOS_LAST,
                            0, 0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorLastEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnQualOperatorMatchesRC
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnQualOperatorMatchesRC
//
//  PURPOSE:   Determines if the Qualification operator specified
//             'matches' the return code for the compare.
//
//  RETURNS:   1 - match
//             0 - do not match
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnQualOperatorMatchesRC( zLONG lOperator, zSHORT nRC )
{
   switch ( nRC )
   {
      case -1:
         if ( (lOperator & zQUAL_LESS_THAN) ||
               lOperator == (zQUAL_NOT | zQUAL_EQUAL) )
         {
            return( 1 );
         }

         break;

      case 0:
         if ( (lOperator & zQUAL_EQUAL) && (lOperator & zQUAL_NOT) == 0 )
         {
            return( 1 );
         }

         break;

      case 1:
         if ( (lOperator & zQUAL_GREATER_THAN) ||
               lOperator == (zQUAL_NOT | zQUAL_EQUAL) )
         {
            return( 1 );
         }

         break;

      case zCALL_ERROR:
         return( zCALL_ERROR );
   }

   return( 0 );
}

//./ ADD NAME=fnStringMatchesAttributeValue
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnStringMatchesAttributeValue
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
int
fnStringMatchesAttributeValue( zCPCHAR             cpcStringValue,
                               zLONG               lOperator,
                               LPENTITYINSTANCE    lpEntityInstance,
                               LPVIEWATTRIB        lpViewAttrib )
{
   zPCHAR   lpRecord;
   zSHORT   nRC;

   lpRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( lpRecord && lpViewAttrib->cType == 'S' )
   {
      lpRecord += lpViewAttrib->ulRecordOffset;
      if ( lpRecord[ 0 ] == (zCHAR) '\xff' )
      {
         zCOREMEM  lpExternalP;

         lpExternalP = (zCOREMEM) (lpRecord + 1);
         lpRecord = zGETPTR( *lpExternalP );
      }

      if ( lpViewAttrib->bCaseSens )
         nRC = zstrcmp( lpRecord, cpcStringValue );
      else
         nRC = zstrcmpi( lpRecord, cpcStringValue );

      if ( nRC < 0 )
         nRC = -1;
      else
      if ( nRC > 0 )
         nRC = 1;

      return( fnQualOperatorMatchesRC( lOperator, nRC ) );
   }

   // If the lpRecord is NULL and the string value is NULL, then they match.
   if ( lpRecord == 0 && (cpcStringValue == 0 || *cpcStringValue == 0) )
      return( 1 );

   return( 0 ); // failure
}

//./ ADD NAME=fnAttrValueMatchesAttrValue
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnAttrValueMatchesAttrValue
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 10
int
fnAttrValueMatchesAttrValue( zVIEW            vAttrView,
                             zLONG            lOperator,
                             LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                             LPENTITYINSTANCE lpSrcEntityInstance,
                             LPVIEWATTRIB     lpSrcViewAttrib,
                             LPVIEWENTITYCSR  lpSearchViewEntityCsr,
                             LPENTITYINSTANCE lpSearchEntityInstance,
                             LPVIEWATTRIB     lpSearchViewAttrib,
                             LPTASK           lpCurrentTask )
{
   zSHORT   nRC;
   zPCHAR   lpSrcRecord;
   zPCHAR   lpSearchRecord;
   zCHAR    szWorkString[ 255 ];

   lpSrcRecord = fnRecordForEntityAttr( lpSrcEntityInstance,
                                        lpSrcViewAttrib );

   lpSearchRecord = fnRecordForEntityAttr( lpSearchEntityInstance,
                                           lpSearchViewAttrib );

   // If both lpRecord are NULL then the attributes are NULL so they match.
   if ( lpSrcRecord == 0 && lpSearchRecord == 0 )
      return( 1 );

   // If only one of them is NULL, then they don't match.
   if ( lpSrcRecord == 0 || lpSearchRecord == 0 )
      return( 0 );

   if ( lpSrcViewAttrib->cType == lpSearchViewAttrib->cType &&
        lpSrcViewAttrib->hDomain == lpSearchViewAttrib->hDomain )
   {
      // If derived attribute for src, invoke operation now.
      // If search attribute is derived, that was done at the
      // beginning of fnSetEntityCursor.
      if ( lpSrcViewAttrib->szDerivedOper[ 0 ] )
      {
         LPVIEWENTITYCSR lpViewEntityCsrDA =
                           fnEstablishViewForInstance( vAttrView, 0,
                                                       lpSrcEntityInstance );

         nRC = fnInvokeDerivedOperation( vAttrView,
                                         zGETPTR( lpViewEntityCsrDA->
                                                            hViewEntity ),
                                         lpSrcViewAttrib,
                                         lpCurrentTask, zDERIVED_GET );
         if ( nRC == zCALL_ERROR )
            return( nRC );
      }

      lpSrcRecord += lpSrcViewAttrib->ulRecordOffset;
      lpSearchRecord += lpSearchViewAttrib->ulRecordOffset;
      switch ( lpSrcViewAttrib->cType )
      {
         case zTYPE_STRING:
            if ( lpSrcRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zCOREMEM  lpExternalP;

               lpExternalP = (zCOREMEM) ( lpSrcRecord + 1 );
               lpSrcRecord = zGETPTR( *lpExternalP );
            }

            if ( lpSearchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zCOREMEM  lpExternalP;

               lpExternalP = (zCOREMEM) ( lpSearchRecord + 1 );
               lpSearchRecord = zGETPTR( *lpExternalP );
            }

            if ( lpSrcViewAttrib->bCaseSens )
               nRC = zstrcmp( lpSrcRecord, lpSearchRecord );
            else
               nRC = zstrcmpi( lpSrcRecord, lpSearchRecord );

         // nRC = (nRC > 0) ? 1 : ((nRC < 0) ? -1 : 0);
            if ( nRC < 0 )
               nRC = -1;
            else
            if ( nRC > 0 )
               nRC = 1;

            return( fnQualOperatorMatchesRC( lOperator, nRC ) );

         case zTYPE_INTEGER:
            if ( *((zPLONG) lpSrcRecord) == *((zPLONG) lpSearchRecord) )
               nRC = 0;
            else
               nRC = (*((zPLONG) lpSrcRecord) <
                                          *((zPLONG) lpSearchRecord) ) ? -1 : 1;

            return( fnQualOperatorMatchesRC( lOperator, nRC ) );

         case zTYPE_DECIMAL:
            nRC = SysCompareDecimalToDecimal( (zPDECIMAL) lpSrcRecord,
                                              (zPDECIMAL) lpSearchRecord );
            return( fnQualOperatorMatchesRC( lOperator, nRC ) );

         case zTYPE_DATETIME:
            nRC = fnCompareDateTimeToDateTime( (LPDTINTERNAL) lpSrcRecord,
                                               (LPDTINTERNAL) lpSearchRecord );
            return( fnQualOperatorMatchesRC( lOperator, nRC ) );

         case zTYPE_BLOB:
            if ( lpSrcRecord[ 0 ] == (zCHAR) '\xff' &&
                 lpSearchRecord[ 0 ] == (zCHAR) '\xff' )
            {
               zPULONG  lpuSrcBlobLth;
               zPULONG  lpuSearchBlobLth;
               zCOREMEM lpExternalP;

               lpExternalP = (zCOREMEM) (lpSrcRecord + 1);
               lpuSrcBlobLth  = (zPULONG) (lpExternalP + 1);
               lpSrcRecord = zGETPTR( *lpExternalP );
               lpExternalP = (zCOREMEM) (lpSearchRecord + 1);
               lpuSearchBlobLth  = (zPULONG) (lpExternalP + 1);
               lpSearchRecord = zGETPTR( *lpExternalP );

               if ( *lpuSrcBlobLth != *lpuSearchBlobLth )
                  break;

               if ( zmemcmp( lpSrcRecord, lpSearchRecord,
                             (int) *lpuSrcBlobLth ) == 0 )
               {
                  return( 1 );
               }

               break;
            }
            else
            if ( lpSrcRecord[ 0 ] != (zCHAR) '\xff' &&
                 lpSearchRecord[ 0 ] != (zCHAR) '\xff' )
            {
               return( 1 );
            }

            break;

      } // end of switch ( lpSrcViewAttrib->cType )
   }
   else
   // Attributes differ in chType and/or Domain
   // Compare the Search to the Src in the context of the Src's Domain
   {
      zULONG          ulTossOut;
      LPVIEWENTITYCSR lpViewEntityCsrForDomain =
                           fnEstablishViewForInstance( vAttrView, 0,
                                                       lpSrcEntityInstance );

      nRC = fnGetVariableFromAttribute( (zPVOID) szWorkString,
                                        &ulTossOut,
                                        zTYPE_STRING,
                                        sizeof( szWorkString ),
                                        vAttrView,
                                        lpViewEntityCsrForDomain,
                                        lpSrcViewAttrib,
                                        0, lpCurrentTask, 0 );
      if ( nRC == 0 )
      {
         zCHAR     szContext[ 33 ];
         LPDOMAIN  lpDomain = zGETPTR( lpSrcViewAttrib->hDomain );
         LPVIEWCSR lpViewCsr = zGETPTR( lpSearchViewEntityCsr->hViewCsr );

         if ( lpDomain )
            strcpy_s( szContext, sizeof( szContext ), lpDomain->szName );
         else
            szContext[ 0 ] = 0;

         nRC = fnCompareAttributeToVariable( zGETPTR( lpViewCsr->hView ),
                                             lpSearchViewEntityCsr,
                                             lpSearchViewAttrib,
                                             (zPVOID) szWorkString,
                                             zTYPE_STRING,
                                             0,
                                             szContext,
                                             lpCurrentTask, 0 );
         return( fnQualOperatorMatchesRC( lOperator, nRC ) );
      }
   }

   // We should never get here.
   return( 0 );
}

//./ ADD NAME=SetCursorFirstEntityByString
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorFirstEntityByString
//
//  PURPOSE: To set the cursor to the first entity instance of the
//           entity type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL      - No entity instances exist
//           zCURSOR_UNCHANGED - Entity cursor unchanged
//           zCURSOR_SET       - Entity position established
//           zCALL_ERROR       - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorFirstEntityByString( zVIEW    lpView,
                              zCPCHAR  cpcEntityName,
                              zCPCHAR  cpcAttributeName,
                              zCPCHAR  cpcStringValue,
                              zCPCHAR  cpcScopingEntityName )
{
   LPTASK                 lpCurrentTask;
   LPVIEWENTITY           lpViewEntity;
   LPVIEWATTRIB           lpViewAttrib;
   LPVIEWENTITY           lpScopingViewEntity;
   LPVIEWENTITYCSR        lpViewEntityCsr;
   LPVIEWENTITYCSR        lpScopingViewEntityCsr;
   zLPCONTEXT             lpDefaultContext;
   LPDOMAIN               lpDomain;
   zSHORT                 nRC;
   zLONG                  lControl = zPOS_FIRST;
   DateTimeInternalRecord stInternalDateTime;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorFirstEntityByString,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorFirstEntityByString, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( cpcStringValue && *cpcStringValue )
   {
      // Check to see if the attribute is really a date/time field.  This is
      // kind of a hack to allow setting cursors by datetimes.
      if ( lpViewAttrib->cType == zTYPE_DATETIME )
      {
         lControl |= zQUAL_DATETIME;
         if ( UfStringToDateTime( cpcStringValue,
                                  (LPDATETIME) &stInternalDateTime ) != 0 )
         {
            zCHAR sz[ 256 ];

            // "KZOEE263 - Input Qualifier cannot be converted to
            //  internal attribute data type"
            strcpy_s( sz, sizeof( sz ), "(S->DT), " );
            strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
            fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz, lpViewAttrib->szName );

            fnOperationReturn( iSetCursorFirstEntityByString, lpCurrentTask );
            return( zCALL_ERROR );
         }

         // Change StringValue to point to internal datetime struct.
         cpcStringValue = (zPCHAR) &stInternalDateTime;
      }
      else
         lControl |= zQUAL_STRING;
   }
   else
      lControl |= zQUAL_ATTR_NULL;

   lpDomain = zGETPTR( lpViewAttrib->hDomain );
   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib, lControl | zIGNORE_ERROR_358,
                            (zPVOID) cpcStringValue,
                            0, 0, 0, lpScopingViewEntityCsr,
                            0, lpCurrentTask );
   if ( nRC < zCURSOR_SET && lpDomain &&
        lpDomain->cDomainType == zDM_TYPE_TABLE )
   {
      zSHORT nRC2;

      nRC2 = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                                lpViewAttrib,
                                lControl | zTEST_CSR_RESULT | zIGNORE_ERROR_358,
                                (zPVOID) cpcStringValue,
                                0, 0, 0, lpScopingViewEntityCsr,
                                (GetDefaultContext( &lpDefaultContext, lpDomain )
                                  ? lpDefaultContext->szName : 0),
                                lpCurrentTask );
      if ( nRC2 >= zCURSOR_SET )
      {
         zCHAR    sz[ 512 ];
         LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

         strcpy_s( sz, sizeof( sz ), "Failure during standard call, but would have been "
                      "successful if the default context was used.\n"
                      "Probable User Error." );
         strcat_s( sz, sizeof( sz ), "\nObject Def: " );
         strcat_s( sz, sizeof( sz ), lpViewOD->szName );
         strcat_s( sz, sizeof( sz ), "\nEntity: " );
         strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
         strcat_s( sz, sizeof( sz ), "\nAttribute: " );
         strcat_s( sz, sizeof( sz ), lpViewAttrib->szName );
         SysMessageBox( lpView, "SetCursorFirstEntityByString", sz, 1 );
      }
   }

   fnOperationReturn( iSetCursorFirstEntityByString, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorNextEntityByString
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorNextEntityByString
//
//  PURPOSE: To set the cursor to the next entity instance of the entity
//           type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL          - No entity instances exist
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position established
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorNextEntityByString( zVIEW    lpView,
                             zCPCHAR  cpcEntityName,
                             zCPCHAR  cpcAttributeName,
                             zCPCHAR  cpcStringValue,
                             zCPCHAR  cpcScopingEntityName )
{
   LPTASK                 lpCurrentTask;
   LPVIEWENTITY           lpViewEntity;
   LPVIEWATTRIB           lpViewAttrib;
   LPVIEWENTITY           lpScopingViewEntity;
   LPVIEWENTITYCSR        lpViewEntityCsr;
   LPVIEWENTITYCSR        lpScopingViewEntityCsr;
   zLPCONTEXT             lpDefaultContext;
   LPDOMAIN               lpDomain;
   zSHORT                 nRC;
   zLONG                  lControl = zPOS_NEXT;
   DateTimeInternalRecord stInternalDateTime;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorNextEntityByString,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorNextEntityByString, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( cpcStringValue && *cpcStringValue )
   {
      // Check to see if the attribute is really a date/time field.  This is
      // kind of a hack to allow setting cursors by datetimes.
      if ( lpViewAttrib->cType == zTYPE_DATETIME )
      {
         lControl |= zQUAL_DATETIME;
         if ( UfStringToDateTime( cpcStringValue,
                                  (LPDATETIME) &stInternalDateTime ) != 0 )
         {
            zCHAR sz[ 256 ];

            // "KZOEE263 - Input Qualifier cannot be converted to
            //  internal attribute data type"
            strcpy_s( sz, sizeof( sz ), "(S->DT), " );
            strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
            fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz, lpViewAttrib->szName );

            fnOperationReturn( iSetCursorFirstEntityByString, lpCurrentTask );
            return( zCALL_ERROR );
         }

         // Change StringValue to point to internal datetime struct.
         cpcStringValue = (zPCHAR) &stInternalDateTime;
      }
      else
         lControl |= zQUAL_STRING;
   }
   else
      lControl |= zQUAL_ATTR_NULL;

   lpDomain = zGETPTR( lpViewAttrib->hDomain );
   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib, lControl | zIGNORE_ERROR_358,
                            (zPVOID) cpcStringValue,
                            0, 0, 0, lpScopingViewEntityCsr,
                            0, lpCurrentTask );
   if ( nRC < zCURSOR_SET && lpDomain &&
        lpDomain->cDomainType == zDM_TYPE_TABLE )
   {
      zSHORT nRC2;

      nRC2 = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                                lpViewAttrib,
                                lControl | zTEST_CSR_RESULT | zIGNORE_ERROR_358,
                                (zPVOID) cpcStringValue,
                                0, 0, 0, lpScopingViewEntityCsr,
                                (GetDefaultContext( &lpDefaultContext, lpDomain )
                                  ? lpDefaultContext->szName : 0),
                                lpCurrentTask );
      if ( nRC2 >= zCURSOR_SET )
      {
         zCHAR    sz[ 512 ];
         LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

         strcpy_s( sz, sizeof( sz ), "Failure during standard call, but would have been "
                      "successful if the default context was used.\n"
                      "Probable User Error." );
         strcat_s( sz, sizeof( sz ), "\nObject Def: " );
         strcat_s( sz, sizeof( sz ), lpViewOD->szName );
         strcat_s( sz, sizeof( sz ), "\nEntity: " );
         strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
         strcat_s( sz, sizeof( sz ), "\nAttribute: " );
         strcat_s( sz, sizeof( sz ), lpViewAttrib->szName );
         SysMessageBox( lpView, "SetCursorNextEntityByString", sz, 1 );
      }
   }

   fnOperationReturn( iSetCursorNextEntityByString, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnIntegerMatchesAttributeValue
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnIntegerMatchesAttributeValue
//
//  PURPOSE:    To determine if an integer value is the same as the
//              the attribute value.  If the Attribute type is not
//              an 'L', a nomatch indicator is return.
//
//  RETURNS:    0 - nomatch
//              1 - match
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
int
fnIntegerMatchesAttributeValue( zLONG               lIntegerValue,
                                zLONG               lOperator,
                                LPENTITYINSTANCE    lpEntityInstance,
                                LPVIEWATTRIB        lpViewAttrib )
{
   zPLONG   lplLong;
   zPCHAR   lpRecord;
   zSHORT   nRC;

   lpRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( lpRecord && lpViewAttrib->cType == 'L' )
   {
      lpRecord += lpViewAttrib->ulRecordOffset;
      lplLong  = (zPLONG) lpRecord;
      if ( *lplLong == lIntegerValue )
         nRC = 0;
      else
         nRC = (*lplLong < lIntegerValue) ? -1 : 1;

      return( fnQualOperatorMatchesRC( lOperator, nRC ) );
   }

   return( 0 );  // failure
}

//./ ADD NAME=fnDecimalMatchesAttributeValue
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDecimalMatchesAttributeValue
//
//  PURPOSE:    To determine if an integer value is the same as the
//              the attribute value.  If the Attribute type is not
//              an 'M', a nomatch indicator is return.
//
//  RETURNS:    0 - nomatch
//              1 - match
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
int
fnDecimalMatchesAttributeValue( zDECIMAL            dDecimalValue,
                                zLONG               lOperator,
                                LPENTITYINSTANCE    lpEntityInstance,
                                LPVIEWATTRIB        lpViewAttrib )
{
   zSHORT    nRC;
   zPDECIMAL lpDecimal;
   zPCHAR    lpRecord;
   LPVIEWOI  lpViewOI = zGETPTR( lpEntityInstance->hViewOI );

   lpRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( lpRecord && lpViewAttrib->cType == zTYPE_DECIMAL )
   {
      lpRecord += lpViewAttrib->ulRecordOffset;
      lpDecimal  = (zPDECIMAL) lpRecord;
      nRC = SysCompareDecimalToDecimal( lpDecimal, &dDecimalValue );
      return( fnQualOperatorMatchesRC( lOperator, nRC ) );
   }

   return( 0 );  // failure
}

//./ ADD NAME=fnDateTimeMatchesAttributeValue
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDateTimeMatchesAttributeValue
//
//  PURPOSE:    To determine if a DateTime value is the same as the
//              the attribute value.  If the Attribute type is not
//              an zTYPE_DATETIME, a nomatch indicator is returned.
//
//  RETURNS:    0 - nomatch
//              1 - match
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
int LOCALOPER
fnDateTimeMatchesAttributeValue( LPDTINTERNAL        lpDateTime,
                                 zLONG               lOperator,
                                 LPENTITYINSTANCE    lpEntityInstance,
                                 LPVIEWATTRIB        lpViewAttrib )
{
   LPDTINTERNAL lpTempDateTime;
   zSHORT       nRC;
   zPCHAR       lpRecord;

   lpRecord = fnRecordForEntityAttr( lpEntityInstance, lpViewAttrib );

   if ( lpRecord && lpViewAttrib->cType == zTYPE_DATETIME )
   {
      lpRecord += lpViewAttrib->ulRecordOffset;
      lpTempDateTime = (LPDTINTERNAL) lpRecord;
      if ( lpTempDateTime->ulDateMinutes == lpDateTime->ulDateMinutes )
      {
         if ( lpTempDateTime->usTSeconds == lpDateTime->usTSeconds )
            nRC = 0;
         else
            nRC = (lpTempDateTime->usTSeconds <
                                       lpDateTime->usTSeconds) ? -1 : 1;
      }
      else
         nRC = (lpTempDateTime->ulDateMinutes <
                                         lpDateTime->ulDateMinutes) ? -1 : 1;

      return( fnQualOperatorMatchesRC( lOperator, nRC ) );
   }

   return( 0 );  // failure
}

//./ ADD NAME=SetCursorFirstEntityByInteger
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorFirstEntityByInteger
//
//  PURPOSE: To set the cursor to the first entity instance of the
//           entity type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL      - No entity instances exist
//           zCURSOR_UNCHANGED - Entity cursor unchanged
//           zCURSOR_SET       - Entity position established
//           zCALL_ERROR       - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorFirstEntityByInteger( zVIEW     lpView,
                               zCPCHAR   cpcEntityName,
                               zCPCHAR   cpcAttributeName,
                               zINTEGER  lIntegerValue,
                               zCPCHAR   cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorFirstEntityByInteger,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorFirstEntityByInteger, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib,
                            (zLONG) (zPOS_FIRST | zQUAL_INTEGER),
                            (zPVOID) &lIntegerValue,
                            0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorFirstEntityByInteger, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorNextEntityByInteger
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorNextEntityByInteger
//
//  PURPOSE: To set the cursor to the next entity instance of the entity
//           type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL          - No entity instances exist
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position established
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorNextEntityByInteger( zVIEW     lpView,
                              zCPCHAR   cpcEntityName,
                              zCPCHAR   cpcAttributeName,
                              zINTEGER  lIntegerValue,
                              zCPCHAR   cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorNextEntityByInteger,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorNextEntityByInteger, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib,
                            (zLONG) (zPOS_NEXT | zQUAL_INTEGER),
                            (zPVOID) &lIntegerValue,
                            0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorNextEntityByInteger, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorFirstEntityByDecimal
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorFirstEntityByDecimal
//
//  PURPOSE: To set the cursor to the first entity instance of the
//           entity type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL      - No entity instances exist
//           zCURSOR_UNCHANGED - Entity cursor unchanged
//           zCURSOR_SET       - Entity position established
//           zCALL_ERROR       - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorFirstEntityByDecimal( zVIEW    lpView,
                               zCPCHAR  cpcEntityName,
                               zCPCHAR  cpcAttributeName,
                               zDECIMAL dDecimalValue,
                               zCPCHAR  cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorFirstEntityByDecimal,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorFirstEntityByDecimal, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib,
                            (zLONG) (zPOS_FIRST | zQUAL_DECIMAL),
                            (zPVOID) &dDecimalValue,
                            0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorFirstEntityByDecimal, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorNextEntityByDecimal
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorNextEntityByDecimal
//
//  PURPOSE: To set the cursor to the next entity instance of the entity
//           type requested withing the scoping entity requested
//
//  RETURNS: zCURSOR_NULL          - No entity instances exist
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position established
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorNextEntityByDecimal( zVIEW     lpView,
                              zCPCHAR   cpcEntityName,
                              zCPCHAR   cpcAttributeName,
                              zDECIMAL  dDecimalValue,
                              zCPCHAR   cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorNextEntityByDecimal,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0  )) == 0 )
   {
      fnOperationReturn( iSetCursorNextEntityByDecimal, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib,
                            (zLONG) (zPOS_NEXT | zQUAL_DECIMAL),
                            (zPVOID) &dDecimalValue,
                            0, 0, 0, lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorNextEntityByDecimal, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorRelativeEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorRelativeEntity
//
//  PURPOSE: To set the cursor to the next/prev entity a specified number
//           of times based on a relative number passed.
//
//  RETURNS: zCURSOR_NULL          - Entity cursor is null
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position changed within
//                                   current parent
//           zCURSOR_SET_NEWPARENT - Entity position changed within a
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetCursorRelativeEntity( zVIEW     lpView,
                         zCPCHAR   cpcEntityName,
                         zLONG     lPositionChange,
                         zCPCHAR   cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorRelativeEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr, 0,
                            (zLONG) zPOS_RELATIVE, 0,
                            0, 0, lPositionChange,
                            lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorRelativeEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetRelativeEntityNumber
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   GetRelativeEntityNumber
//
//  PURPOSE: To get the relative entity number of the entity at the current
//           cursor position.
//
//  PARAMETERS:
//           lpView
//           cpcEntityName
//           cpcScopingEntity
//           nFlag           - 0 count all entities
//                             zQUAL_SELECTED count only selected entites
//                             zQUAL_UNSELECTED count only unselected
//                             entities
//                             zQUAL_SCOPE_OI override scoping entity to
//                             include the entire object instance
//
//  RETURNS: >= 0                  - Relative Entity number of cursor
//           zCURSOR_UNDEFINED     - Entity cursor is undefined
//           zCURSOR_NULL          - Entity cursor is null
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zLONG  OPERATION
GetRelativeEntityNumber( zVIEW     lpView,
                         zCPCHAR   cpcEntityName,
                         zCPCHAR   cpcScopingEntityName,
                         zLONG     lFlag )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   LPENTITYINSTANCE  lpSearchEntityInstance;
   zSHORT            nRC;
   zLONG             lCount;
   zSHORT            nStopLevel;
   zBOOL             bSelectRelevant;
   zSHORT            nDesiredSelectState;
   zLONG             l;

   if ( lFlag & zQUAL_SCOPE_OI )
      cpcScopingEntityName = (zCPCHAR) zSCOPE_OI;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iGetRelativeEntityNumber,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   // If Selected and Unselected entities are requested, then the
   // Select state of the entity is not relevent.
   l = lFlag & (zQUAL_SELECTED | zQUAL_UNSELECTED);
   if ( l && (-l & l) == l )
   {
      bSelectRelevant = TRUE;
      nDesiredSelectState = (l == zQUAL_SELECTED);
   }
   else
      bSelectRelevant = 0;

   // Entity cursor and scoping entity established, now look
   // for a previous entity of the entity type within the
   // scoping entity
   lCount = 0;
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   if ( lpSearchEntityInstance == 0 )
   {
      fnOperationReturn( iGetRelativeEntityNumber, lpCurrentTask );
      return( zCURSOR_NULL );
   }
   else
   if ( lpSearchEntityInstance->u.nInd.bHidden )
   {
      fnOperationReturn( iGetRelativeEntityNumber, lpCurrentTask );
      return( zCURSOR_UNDEFINED );
   }

   if ( cpcScopingEntityName != zSCOPE_OI &&
        (lpScopingViewEntityCsr == 0 ||
         lpScopingViewEntityCsr == zGETPTR( lpViewEntityCsr->hParent )) )
   {
      while ( lpSearchEntityInstance->hPrevTwin )
      {
         lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
         if ( lpSearchEntityInstance->u.nInd.bHidden == FALSE &&
              (bSelectRelevant == FALSE ||
               (fnSelectedInstanceFarbler( lpViewCsr,
                                           lpViewEntity,
                                           zGETHNDL( lpSearchEntityInstance ), 1 )
                  == nDesiredSelectState)) )
         {
            lCount++;
         }
      }
   }
   else
   {
      // We need to count within a scoping view entity csr
      if ( cpcScopingEntityName == zSCOPE_OI )
         nStopLevel = 0;
      else
         nStopLevel = lpScopingViewEntityCsr->nLevel;

      for ( lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
            lpSearchEntityInstance;
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier ) )
      {
         if ( lpSearchEntityInstance->nLevel <= nStopLevel )
            break;

         if ( zGETPTR( lpSearchEntityInstance->hViewEntity ) != lpViewEntity )
            continue;

         if ( lpSearchEntityInstance->nLevel != lpViewEntityCsr->nLevel )
            continue;

         if ( lpSearchEntityInstance->u.nInd.bHidden )
            continue;

         if ( bSelectRelevant == FALSE ||
              fnSelectedInstanceFarbler( lpViewCsr, lpViewEntity,
                                         zGETHNDL( lpSearchEntityInstance ),
                                         1 ) == nDesiredSelectState )
         {
            lCount++;
         }
      }
   }

   fnOperationReturn( iGetRelativeEntityNumber, lpCurrentTask );
   return( lCount );
}

//./ ADD NAME=SetCursorAbsolutePosition
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorAbsolutePosition
//
//  PURPOSE: To set the cursor to an entity based on an absolute number
//           passed.
//
//  PARAMETERS:
//           pchReturnedEntityName - Name of the entity which has
//                                   received position.
//           lpView                - View to object instance.
//           lPosition             - Number of the absolute entity to
//                                   position on.  The first entity is
//                                   position 0.
//
//  RETURNS: zCURSOR_NULL          - Entity cursor is null
//           zCURSOR_SET           - Entity position changed within
//                                   current parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorAbsolutePosition( zPCHAR   pchReturnEntityName,
                           zVIEW    lpView,
                           zLONG    lPosition )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpSearchEntityInstance;
   LPVIEWOI          lpViewOI;
   zSHORT            nRC;

   // initial return entity name
   *pchReturnEntityName = 0;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetCursorAbsolutePosition,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Position must be greater than or equal zero.
   if ( lPosition < 0 )
   {
      fnOperationReturn( iSetCursorAbsolutePosition, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Get the root entity instance
   lpViewCsr              = zGETPTR( lpView->hViewCsr );
   lpViewOI               = zGETPTR( lpViewCsr->hViewOI );
   lpSearchEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );

   if ( lpSearchEntityInstance == 0 )
   {
      fnOperationReturn( iSetCursorAbsolutePosition, lpCurrentTask );
      return( zCURSOR_NULL );
   }

   // count down the number of instances that are not hidden
   while ( lPosition > 0 || lpSearchEntityInstance->u.nInd.bHidden )
   {
      if ( lpSearchEntityInstance->u.nInd.bHidden == FALSE )
         lPosition--;

      lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
      if ( lpSearchEntityInstance == 0 )
         break;
   }

   if ( lPosition == 0 &&
        lpSearchEntityInstance && lpSearchEntityInstance->u.nInd.bHidden == FALSE )
   {
      LPVIEWENTITY lpViewEntity = zGETPTR( lpSearchEntityInstance->hViewEntity );

      // success

      // Check to see if the cursor is a hierarchical cursor.
      if ( lpViewCsr->hHierRootEntityInstance )
      {
         // Look for a View entity cursor which matches the entity instance.
         while ( TRUE )
         {
            for ( lpViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
                  lpViewEntityCsr;
                  lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier ) )
            {
               if ( lpSearchEntityInstance->hViewEntity ==
                                                lpViewEntityCsr->hViewEntity )
               {
                  break;
               }
            }

            if ( lpViewEntityCsr )
               break;

            fnResetViewFromSubobject( lpView );
         }

         while ( lpViewEntityCsr->nLevel > lpSearchEntityInstance->nLevel )
            fnResetViewFromSubobject( lpView );

         lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
         lpViewCsr->hHierEntityInstance = lpViewEntityCsr->hEntityInstance;
      // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "SetCursorAbsolutePosition", "UNSET_CSR", -1 );

         fnResetCursorForViewChildren( lpViewEntityCsr );
         strcpy_s( pchReturnEntityName, 33, lpViewEntity->szName );
         nRC = zCURSOR_SET;
      }
      else
      {
         lpViewEntityCsr = fnEstablishViewForInstance( lpView, 0,
                                                       lpSearchEntityInstance );
         if ( lpViewEntity->bRecursive &&
              SetViewToSubobject( lpView, lpViewEntity->szName ) == 0 )
         {
            LPVIEWENTITY lpSearchViewEntity;

            // Find the recursive parent name.
            for ( lpSearchViewEntity = zGETPTR( lpSearchEntityInstance->hViewEntity );
                  lpSearchViewEntity->bRecursivePar == FALSE;
                  lpSearchViewEntity = zGETPTR( lpSearchViewEntity->hParent ) )
            {
               ; // nothing needs to be done here.
            }

            strcpy_s( pchReturnEntityName, 33, lpSearchViewEntity->szName );
            nRC = zCURSOR_SET_RECURSIVECHILD;
         }
         else
         {
            strcpy_s( pchReturnEntityName, 33, lpViewEntity->szName );
            nRC = zCURSOR_SET;
         }
      }

      fnCallListeners( lpView, pchReturnEntityName );
   }
   else
   {
      // failure, just return zCURSOR_NULL for now...
      nRC = zCURSOR_NULL;
   }

   fnOperationReturn( iSetCursorAbsolutePosition, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetAbsolutePositionForEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   GetAbsolutePositionForEntity
//
//  PURPOSE: To get the absolute entity number of the entity at the current
//           cursor position.
//
//  RETURNS: >= 0                  - Absolute Entity number of returned ok
//           zCURSOR_UNDEFINED     - Entity cursor is undefined
//           zCURSOR_NULL          - Entity cursor is null
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
GetAbsolutePositionForEntity( zPLONG  plReturnPosition,
                              zVIEW   lpView,
                              zCPCHAR cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpSearchEntityInstance;
   zLONG             lCount;
   zSHORT            nRC;

   *plReturnPosition = -1;   // init to -1

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           0,
                                           &lpViewEntityCsr,
                                           0,
                                           iGetAbsolutePositionForEntity,
                                           lpView,
                                           cpcEntityName,
                                           0 )) != 0 )
   {
      return( nRC );
   }

   // Entity cursor and scoping entity established, now look
   // for a previous entity of the entity type within the
   // scoping entity
   lCount = 0;
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpSearchEntityInstance  = zGETPTR( lpViewEntityCsr->hEntityInstance );
   if ( lpSearchEntityInstance == 0 )
   {
      fnOperationReturn( iGetAbsolutePositionForEntity, lpCurrentTask );
      return( zCURSOR_NULL );
   }

   if ( lpSearchEntityInstance == UNSET_CSR ||
        lpSearchEntityInstance->u.nInd.bHidden )
   {
      fnOperationReturn( iGetAbsolutePositionForEntity, lpCurrentTask );
      return( zCURSOR_UNDEFINED );
   }

   // We need to count entities.
   lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
   while ( lpSearchEntityInstance )
   {
      if ( lpSearchEntityInstance->u.nInd.bHidden == FALSE )
         lCount++;

      lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
   }

   *plReturnPosition = lCount;

   fnOperationReturn( iGetAbsolutePositionForEntity, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=fnInstanceMatchesInstance
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:  fnInstanceMatchesInstance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
int
fnInstanceMatchesInstance( LPENTITYINSTANCE    lpCompareInstance,
                           LPENTITYINSTANCE    lpSearchInstance )
{
   LPENTITYINSTANCE lpWrkEntityInstance;

   if ( lpSearchInstance == lpCompareInstance )
      return( 1 );

   while ( lpSearchInstance->hPrevVsn )
      lpSearchInstance = zGETPTR( lpSearchInstance->hPrevVsn );

   while ( lpCompareInstance->hPrevVsn )
      lpCompareInstance = zGETPTR( lpCompareInstance->hPrevVsn );

   while ( lpSearchInstance )
   {
      if ( lpSearchInstance == lpCompareInstance )
         return( 1 );

      if ( lpSearchInstance->hNextLinked )
      {
         for ( lpWrkEntityInstance = zGETPTR( lpSearchInstance->hNextLinked );
               lpWrkEntityInstance != lpSearchInstance;
               lpWrkEntityInstance =
                              zGETPTR( lpWrkEntityInstance->hNextLinked ) )
         {
            if ( lpWrkEntityInstance == lpCompareInstance )
               return( 1 );
         }
      }

      if ( lpCompareInstance->hNextVsn )
         lpCompareInstance = zGETPTR( lpCompareInstance->hNextVsn );
      else
      {
         lpSearchInstance = zGETPTR( lpSearchInstance->hNextVsn );
         while ( lpCompareInstance->hPrevVsn )
            lpCompareInstance = zGETPTR( lpCompareInstance->hPrevVsn );
      }
   }

   return( 0 );  // failure
}

//./ ADD NAME=SetCursorFirstEntityByAttr
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorFirstEntityByAttr
//
//  PURPOSE: To set the cursor to the first entity instance of the
//           entity type requested within the scoping entity requested
//           using the attribute value of a second view, entity
//           as qualification
//
//  RETURNS: zCURSOR_NULL      - No entity instances exist
//           zCURSOR_UNCHANGED - Entity cursor unchanged
//           zCURSOR_SET       - Entity position established
//           zCALL_ERROR       - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
SetCursorFirstEntityByAttr( zVIEW     lpView,
                            zCPCHAR   cpcEntityName,
                            zCPCHAR   cpcAttributeName,
                            zVIEW     lpSrcView,
                            zCPCHAR   cpcSrcEntityName,
                            zCPCHAR   cpcSrcAttributeName,
                            zCPCHAR   cpcScopingEntityName )
{
   LPTASK               lpCurrentTask;
   LPVIEWENTITY         lpViewEntity;
   LPVIEWENTITY         lpSrcViewEntity;
   LPVIEWENTITY         lpScopingViewEntity;
   LPVIEWENTITYCSR      lpViewEntityCsr;
   LPVIEWENTITYCSR      lpSrcViewEntityCsr;
   LPVIEWENTITYCSR      lpScopingViewEntityCsr;
   LPVIEWATTRIB         lpViewAttrib;
   LPVIEWATTRIB         lpSrcViewAttrib;
   zSHORT               nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorFirstEntityByAttr,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpSrcViewEntity,
                                           0, // No scoping view entity
                                           &lpSrcViewEntityCsr,
                                           0,
                                           0, // do not pass oper id
                                           lpSrcView,
                                           cpcSrcEntityName,
                                           0 )) != 0 ) // No Src scoping ent
   {
      fnOperationReturn( iSetCursorFirstEntityByAttr, lpCurrentTask );
      return( nRC );
   }

   // Validate the search attribute
   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorFirstEntityByAttr, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Validate the qualification attribute
   if ( (lpSrcViewAttrib = fnValidViewAttrib( lpSrcView, lpSrcViewEntity,
                                              cpcSrcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorFirstEntityByAttr, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Now, position the cursor
   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib,
                            (zLONG) (zPOS_FIRST | zQUAL_ENTITYATTR),
                            (zPVOID) lpSrcView, lpSrcViewEntityCsr,
                            lpSrcViewAttrib, 0,
                            lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorFirstEntityByAttr, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorNextEntityByAttr
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorNextEntityByAttr
//
//  PURPOSE: To set the cursor to the next entity instance of the
//           entity type requested within the scoping entity requested
//           using the attribute value of a second view, entity
//           as qualification
//
//  RETURNS: zCURSOR_NULL      - No entity instances exist
//           zCURSOR_UNCHANGED - Entity cursor unchanged
//           zCURSOR_SET       - Entity position established
//           zCALL_ERROR       - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zSHORT OPERATION
SetCursorNextEntityByAttr( zVIEW     lpView,
                           zCPCHAR   cpcEntityName,
                           zCPCHAR   cpcAttributeName,
                           zVIEW     lpSrcView,
                           zCPCHAR   cpcSrcEntityName,
                           zCPCHAR   cpcSrcAttributeName,
                           zCPCHAR   cpcScopingEntityName )
{
   LPTASK               lpCurrentTask;
   LPVIEWENTITY         lpViewEntity;
   LPVIEWENTITY         lpSrcViewEntity;
   LPVIEWENTITY         lpScopingViewEntity;
   LPVIEWENTITYCSR      lpViewEntityCsr;
   LPVIEWENTITYCSR      lpSrcViewEntityCsr;
   LPVIEWENTITYCSR      lpScopingViewEntityCsr;
   LPVIEWATTRIB         lpViewAttrib;
   LPVIEWATTRIB         lpSrcViewAttrib;
   zSHORT               nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorNextEntityByAttr,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpSrcViewEntity,
                                           0, // No scoping view entity
                                           &lpSrcViewEntityCsr,
                                           0,
                                           0, // do not pass oper id
                                           lpSrcView,
                                           cpcSrcEntityName,
                                           0 )) != 0 ) //  no scoping entity
   {
      return( nRC );
   }

   // Validate the search attribute
   if ( (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorNextEntityByAttr, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Validate the qualification attribute
   if ( (lpSrcViewAttrib = fnValidViewAttrib( lpSrcView, lpSrcViewEntity,
                                              cpcSrcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iSetCursorNextEntityByAttr, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Now, position the cursor
   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr,
                            lpViewAttrib,
                            (zLONG) (zPOS_NEXT | zQUAL_ENTITYATTR),
                            (zPVOID) lpSrcView, lpSrcViewEntityCsr,
                            lpSrcViewAttrib, 0,
                            lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorNextEntityByAttr, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorFirstEntityByEntityCsr
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorFirstEntityByEntityCsr
//
//  PURPOSE: To set the cursor to the first entity instance of the
//           entity type requested within the scoping entity requested
//           using a view containing a linked instance of the entity
//           type as qualification
//
//  RETURNS: zCURSOR_NULL      - No entity instances exist
//           zCURSOR_UNCHANGED - Entity cursor unchanged
//           zCURSOR_SET       - Entity position established
//           zCALL_ERROR       - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorFirstEntityByEntityCsr( zVIEW     lpView,
                                 zCPCHAR   cpcEntityName,
                                 zVIEW     lpSrcView,
                                 zCPCHAR   cpcSrcEntityName,
                                 zCPCHAR   cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorFirstEntityByEntityCs,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpSrcViewEntity,
                                           0, // No scoping view entity
                                           &lpSrcViewEntityCsr,
                                           0,
                                           0, // do not pass oper id
                                           lpSrcView,
                                           cpcSrcEntityName,
                                           0 )) != 0 ) // no scoping entity
   {
      return( nRC );
   }

   // Make sure the source view entity and the target view entity are
   // the same Entity type
   if ( lpViewEntity->lEREntTok != lpSrcViewEntity->lEREntTok )
   {
      // "KZOEE268 - Source and target entity types do not match"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 268, 0,
                        lpViewEntity->szName,
                        lpSrcViewEntity->szName );
      fnOperationReturn( iSetCursorFirstEntityByEntityCs, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr, 0,
                            (zLONG) (zPOS_FIRST | zQUAL_ENTITYCSR),
                            (zPVOID) lpSrcView, lpSrcViewEntityCsr,
                            0, 0,
                            lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorFirstEntityByEntityCs, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorNextEntityByEntityCsr
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetCursorNextEntityByEntityCsr
//
//  PURPOSE: To set the cursor to the next entity instance of the entity
//           type requested withing the scoping entity requested using
//           a view containing a linked instance of the entity type
//           as qualification
//
//  RETURNS: zCURSOR_NULL          - No entity instances exist
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position established
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetCursorNextEntityByEntityCsr( zVIEW     lpView,
                                zCPCHAR   cpcEntityName,
                                zVIEW     lpSrcView,
                                zCPCHAR   cpcSrcEntityName,
                                zCPCHAR   cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorNextEntityByEntityCsr,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpSrcViewEntity,
                                           0, // No scoping view entity
                                           &lpSrcViewEntityCsr,
                                           0,
                                           0, // do not pass oper id
                                           lpSrcView,
                                           cpcSrcEntityName,
                                           0 )) != 0 ) // no scoping entity
   {
      return( nRC );
   }

   // Make sure the source view entity and the target view entity are
   // the same Entity type
   if ( lpViewEntity->lEREntTok != lpSrcViewEntity->lEREntTok )
   {
      // "KZOEE268 - Source and target entity types do not match"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 268, 0,
                        lpViewEntity->szName,
                        lpSrcViewEntity->szName );
      fnOperationReturn( iSetCursorNextEntityByEntityCsr, lpCurrentTask );
      return( zCALL_ERROR );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr, 0,
                            (zLONG) (zPOS_NEXT | zQUAL_ENTITYCSR),
                            (zPVOID) lpSrcView, lpSrcViewEntityCsr,
                            0, 0,
                            lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorNextEntityByEntityCsr, lpCurrentTask );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  Hierarchical processing operations
//
/////////////////////////////////////////////////////////////////////////////

//./ ADD NAME=DefineHierarchicalCursor
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   DefineHierarchicalCursor
//
//  PURPOSE: To set the mode of processing in a view to hierarchical
//           and define the current entity position for
//           hierarchical processing. Note the root of the view is
//           automatically the scoping position.
//
//  RETURNS:          0 - Hierarchical cursor position set
//          zCALL_ERROR - Error in establishing hierarchical cursor
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
DefineHierarchicalCursor( zVIEW   lpView,
                          zCPCHAR cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPVIEWCSR        lpViewCsr;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITYCSR  lpRootViewEntityCsr;

   if ( fnValidateCursorParameters( &lpCurrentTask,
                                    &lpViewEntity,
                                    0,
                                    &lpViewEntityCsr,
                                    0,
                                    iDefineHierarchicalCursor,
                                    lpView,
                                    cpcEntityName,
                                    0 ) )
   {
      return( zCALL_ERROR );
   }

   // Now that the parameters look OK set the hierarchical cursor in
   // the ViewCsr
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpRootViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
   lpViewCsr->hHierRootEntityInstance = lpRootViewEntityCsr->hEntityInstance;
   if ( lpViewEntityCsr )
      lpViewCsr->hHierEntityInstance = lpViewEntityCsr->hEntityInstance;
   else
      lpViewCsr->hHierEntityInstance = UNSET_CSR;

   fnOperationReturn( iDefineHierarchicalCursor, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=DropHierarchicalCursor
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   DropHierarchicalCursor
//
//  PURPOSE: To drop hierarchical processing
//
//  RETURNS:          0 - Hierarchical cursor position dropped
//          zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
DropHierarchicalCursor( zVIEW   lpView )
{
   LPTASK    lpCurrentTask;
   LPVIEWCSR lpViewCsr;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iDropHierarchicalCursor,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // We drop the cursor regardless of whether is is currently set or not.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewCsr->hHierRootEntityInstance = 0;
   lpViewCsr->hHierEntityInstance = 0;

   fnOperationReturn( iDropHierarchicalCursor, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=SetCursorNextEntityHierarchical
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetCursorNextEntityHierarchical
//
//  PURPOSE:    To position the entity cursor for the next hierarchical
//              position based on the current hierarchical pointer.
//              If a recursive Entity is positioned on, it is the
//              responsibility/option  of the caller to subsequently go
//              down a recursive level with the operation
//              SetViewToSubobject for the recursive entity. Otherwise, the
//              dependent entities to a recursive entity will be skipped in
//              subsequent hierarchical processing. Note: if the caller
//              does go down recursive hierarchical levels, this operation
//              will automatically return hierarchical levels in subsequent
//              calls.
//
//  RETURNS:
//    zCURSOR_UNCHANGED          - Hierarchical cursor already positioned
//                                 on last hierarchical position for view
//                                 (UNCHANGED)
//    zCURSOR_SET                - Hierarchical cursor positioned on
//                                 next hierarchical entity
//    zCURSOR_SET_NEWPARENT      - Hierarchical cursor positioned on
//                                 next hierarchical entity - NEW PARENT
//    zCURSOR_SET_RECURSIVECHILD - Hierarchical cursor positioned on
//                                 next hierarchical entity RECURSIVECHILD
//    zCALL_ERROR                - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorNextEntityHierarchical( zPUSHORT lpnReturnLevel,
                                 zPCHAR   pchReturnEntityName,
                                 zVIEW    lpView )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nStartLevel;
   zSHORT            nRC;

   // Validate parameters.
   if ( fnValidateCursorParameters( &lpCurrentTask,
                                    0, 0, 0, 0,
                                    iSetCursorNextEntityHierarchica,
                                    lpView, 0, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Default RC to zCURSOR_SET
   nRC = zCURSOR_SET;

   // Get View Cursor
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpEntityInstance = zGETPTR( lpViewCsr->hHierEntityInstance );
   if ( lpEntityInstance == UNSET_CSR )
   {
      lpEntityInstance = zGETPTR( lpViewCsr->hHierRootEntityInstance );
      lpViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
   }
   else
   {
      nStartLevel = lpEntityInstance->nLevel;
      lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
      for ( ; ; )
      {
         LPENTITYINSTANCE lpHierRootEntityInstance =
                              zGETPTR( lpViewCsr->hHierRootEntityInstance );

         // Check to see if the EI is out of view.  This happens if the EI has
         // a level <= to the HierRoot EI *unless* the root level is 1.  If the
         // level is 1 then we have a multi-root OI and this is OK.
         if ( lpEntityInstance == 0 ||
              ( lpHierRootEntityInstance->nLevel > 1 &&
                lpEntityInstance->nLevel <= lpHierRootEntityInstance->nLevel ) )
         {
            nRC = zCURSOR_UNCHANGED;
            break;
         }

         // If entity instance is not deleted or excluded, look for
         // a View entity cursor which matches the entity instance.
         if ( lpEntityInstance->u.nInd.bHidden == FALSE )
         {
            while ( TRUE )
            {
               for ( lpViewEntityCsr = zGETPTR( lpViewCsr->hRootViewEntityCsr );
                     lpViewEntityCsr;
                     lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier ) )
               {
                  if ( lpEntityInstance->hViewEntity ==
                                              lpViewEntityCsr->hViewEntity )
                  {
                     break;
                  }
               }

               if ( lpViewEntityCsr )
                  break;

               fnResetViewFromSubobject( lpView );
            }

            while ( lpViewEntityCsr->nLevel > lpEntityInstance->nLevel )
               fnResetViewFromSubobject( lpView );

            if ( lpViewEntityCsr->nLevel == lpEntityInstance->nLevel )
               break;
         }

         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier );
      }
   }

   // If nRC is zCURSOR_SET, set the entity instance in the cursor and reset
   // the cursor's children.
   if ( nRC == zCURSOR_SET )
   {
      LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

      lpViewEntityCsr->hEntityInstance = zGETHNDL( lpEntityInstance );
   // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "SetCursorAbsolutePosition", "UNSET_CSR", -1 );

      lpViewCsr->hHierEntityInstance = lpViewEntityCsr->hEntityInstance;
      fnResetCursorForViewChildren( lpViewEntityCsr );
      *lpnReturnLevel = lpEntityInstance->nLevel;
      strcpy_s( pchReturnEntityName, 33, lpViewEntity->szName );

      lpViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );
      if ( lpViewEntity->bRecursive )
         nRC = zCURSOR_SET_RECURSIVECHILD;
      else
      if ( lpEntityInstance->nLevel < nStartLevel )
         nRC = zCURSOR_SET_NEWPARENT;
   }

   fnOperationReturn( iSetCursorNextEntityHierarchica, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetEntityNameForHierarchicalCsr
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetEntityNameForHierarchicalCsr
//
//  PURPOSE:    Returns the name of the entity that has hierarchical
//              positioning.
//
//  RETURNS:    0 - OK
//              zCURSOR_UNDEFINED     - Entity cursor is undefined
//              zCURSOR_NULL          - Entity cursor is null
//              zCALL_ERROR
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetEntityNameForHierarchicalCsr( zPUSHORT puReturnLevel,
                                 zPCHAR   pchReturnEntityName,
                                 zPULONG  pulAbsolutePosition,
                                 zVIEW    lpView )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nRC;

   *pchReturnEntityName = 0;
   *pulAbsolutePosition = 0;
   *puReturnLevel      = 0;

   // Validate parameters.
   if ( fnValidateCursorParameters( &lpCurrentTask, 0, 0, 0, 0,
                                    iGetEntityNameForHierarchicalCsr,
                                    lpView, 0, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Get View Cursor.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpEntityInstance = zGETPTR( lpViewCsr->hHierEntityInstance );
   if ( lpEntityInstance == UNSET_CSR )
   {
      nRC = zCURSOR_UNDEFINED;
   }
   else
   if ( lpEntityInstance == 0 )
   {
      nRC = zCURSOR_NULL;
   }
   else
   {
      LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

      strcpy_s( pchReturnEntityName, 33, lpViewEntity->szName );
      *puReturnLevel = lpEntityInstance->nLevel;

      // We need to count previous entities.
      for ( ;
            lpEntityInstance->hPrevHier;
            lpEntityInstance = zGETPTR( lpEntityInstance->hPrevHier ) )
      {
         if ( lpEntityInstance->u.nInd.bHidden == FALSE )
            (*pulAbsolutePosition)++;
      }

      nRC = zCURSOR_SET;
   }

   fnOperationReturn( iGetEntityNameForHierarchicalCsr, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetEntityKeyForHierarchicalCsr
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetEntityKeyForHierarchicalCsr
//
//  PURPOSE:    Returns the level, name and key of the entity that has
//              hierarchical positioning.
//
//  RETURNS:    0 - OK
//              zCURSOR_UNDEFINED     - Entity cursor is undefined
//              zCURSOR_NULL          - Entity cursor is null
//              zCALL_ERROR
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetEntityKeyForHierarchicalCsr( zPULONG  pulReturnLevel,
                                zPCHAR   pchReturnEntityName,
                                zPULONG  pulEntityKey,
                                zVIEW    lpView )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nRC;

   *pulReturnLevel      = 0;
   *pchReturnEntityName = 0;
   *pulEntityKey        = 0;

   // Validate parameters.
   if ( fnValidateCursorParameters( &lpCurrentTask, 0, 0, 0, 0,
                                    iGetEntityKeyForHierarchicalCsr,
                                    lpView, 0, 0 ) )
   {
      return( zCALL_ERROR );
   }

   // Get View Cursor.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpEntityInstance = zGETPTR( lpViewCsr->hHierEntityInstance );
   if ( lpEntityInstance == UNSET_CSR )
   {
      nRC = zCURSOR_UNDEFINED;
   }
   else
   if ( lpEntityInstance == 0 )
   {
      nRC = zCURSOR_NULL;
   }
   else
   {
      LPVIEWENTITY lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

      strcpy_s( pchReturnEntityName, 33, lpViewEntity->szName );
      *pulReturnLevel = lpEntityInstance->nLevel;
      *pulEntityKey = lpEntityInstance->ulKey;
      nRC = zCURSOR_SET;
   }

   fnOperationReturn( iGetEntityKeyForHierarchicalCsr, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorFirstSelectedEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetCursorFirstSelectedEntity
//
//  PURPOSE:    Once a subset list of entity instances has been created,
//              you can use this operation to position on the first
//              entity instance.  To position on subsequent instances,
//              use SetCursorNextSelectedEntity.  These operations provide
//              the ability to process a defined subset list of entities.
//
//  PARAMETERS: zView               - View to Object Instance
//              cpcEntityName        - Entity Cursor to set
//              cpcEntityName        - Scoping Entity
//
//  RETURNS: zCURSOR_NULL      - No entity instances exist
//           zCURSOR_UNCHANGED - Entity cursor unchanged
//           zCURSOR_SET       - Entity position established
//           zCALL_ERROR       - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorFirstSelectedEntity( zVIEW   lpView,
                              zCPCHAR cpcEntityName,
                              zCPCHAR cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorFirstSelectedEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr, 0,
                            (zLONG) (zPOS_FIRST | zQUAL_SELECTED),
                            0, 0,
                            0, 0,
                            lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorFirstSelectedEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetCursorNextSelectedEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetCursorNextSelectedEntity
//
//  PURPOSE:    Use this operation to position on subsequent entity
//              instances in a list after you have used the operation
//              SetCursorFirstSelectedEntity to position on the first
//              entity instance of the selected list.  These operations
//              provide the ability to process a defined subset list of
//              entities.
//
//  PARAMETERS: zView               - View to Object Instance
//              cpcEntityName        - Entity Cursor to set
//              cpcEntityName        - Scoping Entity
//
//  RETURNS: zCURSOR_NULL          - No entity instances exist
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position established
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetCursorNextSelectedEntity( zVIEW   lpView,
                             zCPCHAR cpcEntityName,
                             zCPCHAR cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetCursorNextSelectedEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   nRC = fnSetEntityCursor( lpView, lpViewEntity, lpViewEntityCsr, 0,
                            (zLONG) (zPOS_NEXT | zQUAL_SELECTED),
                            0, 0,
                            0, 0,
                            lpScopingViewEntityCsr, 0,
                            lpCurrentTask );

   fnOperationReturn( iSetCursorNextSelectedEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetSelectSetForView
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetSelectSetForView
//
//  PURPOSE: This operation sets the current select set for select
//           processing ( Querying/Setting select states and Selected
//           Cursor navigation ). For each view, there are 16 select
//           sets identified by the number ( id ) 1-16. Select set 1 is
//           the default when a view is initially established.
//
//  PARAMETERS: zVIEW - The view whose current select set is to be
//                      established
//              nSelectSetID  - The Select set id to be set 1-16.
//
//  RETURNS:    1-16 - Success, the previous state of the select set.
//              zCALL_ERROR - invalid view or invalid select set passed
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
SetSelectSetForView( zVIEW   lpView,
                     zSHORT  nSelectSetID )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   zSHORT            nRC, nWk;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetSelectSetForView,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( nSelectSetID < 1 || nSelectSetID > 16 )
   {
      // "KZOEE269 - Invalid Select Set ID, ID="
      fnIssueCoreError( lpCurrentTask, lpView, 8, 269, ( zLONG ) nSelectSetID, 0, 0 );
      fnOperationReturn( iSetSelectSetForView, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Get the view csr
   lpViewCsr = zGETPTR( lpView->hViewCsr );

   // Determine the current select set
   nWk = lpViewCsr->nCurrentSelectSet;
   nRC = 0;
   while ( nWk )
   {
      nWk >>= 1;
      nRC++;
   }

   if ( nRC != nSelectSetID )
   {
      nWk = 0x0001;
      for ( ; ; )
      {
         nSelectSetID--;
         if ( nSelectSetID == 0 )
            break;

         nWk <<= 1;
      }

      lpViewCsr->nCurrentSelectSet = nWk;
   }

   fnOperationReturn( iSetSelectSetForView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=GetSelectStateOfEntityForSet
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   GetSelectStateOfEntityForSet
//
//  PURPOSE: Use this operation to test whether or not an entity instance
//           has been selected for a given select set.
//
//  PARAMETERS: lpView        - View to Object Instance
//              cpcEntityName - Entity to interrogate
//              nSelectSet   - The select Set to check
//
//  RETURNS: 0           - Entity Instance has not been selected
//           1           - Entity Instance has been selected
//           zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
GetSelectStateOfEntityForSet( zVIEW   lpView,
                              zCPCHAR cpcEntityName,
                              zSHORT  nSelectSet )
{
   zSHORT nRC;

   nRC = SetSelectSetForView( lpView, nSelectSet );
   if ( nRC >= 0 )
   {
      nSelectSet = nRC;
      nRC = GetSelectStateOfEntity( lpView, cpcEntityName );
      SetSelectSetForView( lpView, nSelectSet );
   }

   return( nRC );
}

//./ ADD NAME=GetSelectStateOfEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   GetSelectStateOfEntity
//
//  PURPOSE: Use this operation to test whether or not an entity instance
//           has been selected.  Primarily, this is used with the ability
//           to select multiple items in a listbox.  Unlike the operation
//           SetCursorNextSelectedEntity which automatically positions at
//           the next selected entity, GetSelectStateOfEntity allows you
//           to determine the state (selected or not) of an entity
//           instance.
//
//  PARAMETERS: lpView        - View to Object Instance
//              cpcEntityName - Entity to interrogate
//
//  RETURNS: 0           - Entity Instance has not been selected
//           1           - Entity Instance has been selected
//           zCALL_ERROR - Error excluding entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
GetSelectStateOfEntity( zVIEW   lpView,
                        zCPCHAR cpcEntityName )
{
   LPVIEWENTITY       lpViewEntity;
   LPVIEWENTITYCSR    lpViewEntityCsr;
   LPENTITYINSTANCE   lpEntityInstance;
   LPTASK             lpCurrentTask;
   zSHORT             nRC;

   if ( fnValidateCursorParameters( &lpCurrentTask,
                                    &lpViewEntity,
                                    0,
                                    &lpViewEntityCsr,
                                    0,
                                    iGetSelectStateOfEntity,
                                    lpView,
                                    cpcEntityName,
                                    0 ) )
   {
      return( zCALL_ERROR );
   }

   // Scan to see if Entity is in the SelectedInstance chain
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   if ( lpEntityInstance )
   {
      nRC = fnSelectedInstanceFarbler( zGETPTR( lpViewEntityCsr->hViewCsr ),
                                       zGETPTR( lpEntityInstance->hViewEntity ),
                                       lpViewEntityCsr->hEntityInstance,
                                       1 );
   }
   else
      nRC = 0;

   fnOperationReturn( iGetSelectStateOfEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetAllSelStatesForEntityForSet
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetAllSelStatesForEntityForSet
//
//  PURPOSE: Use this operation to select or deselect all the entity
//           instances of the same type under the same parent for a
//           given select set.
//
//  PARAMETERS: lpView               - View to Object Instance
//              cpcEntityName        - Entity to set
//              sState              - 0 deselects, 1 selects
//              nSelectSet          - Select Set ( 0-16 )
//              cpcScopingEntityName - ScopingEntity
//
//  RETURNS: 0           - Entity Instances Selected/Deselected
//           zCALL_ERROR - Error excluding entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
SetAllSelStatesForEntityForSet( zVIEW   lpView,
                                zCPCHAR cpcEntityName,
                                zSHORT  nState,
                                zSHORT  nSelectSet,
                                zCPCHAR cpcScopingEntityName )
{
   zSHORT nRC;

   nRC = SetSelectSetForView( lpView, nSelectSet );
   if ( nRC >= 0 )
   {
      nSelectSet = nRC;
      nRC = SetAllSelectStatesForEntity( lpView, cpcEntityName,
                                         nState, cpcScopingEntityName );
      SetSelectSetForView( lpView, nSelectSet );
   }

   return( nRC );
}

//./ ADD NAME=SetAllSelectStatesForEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetAllSelectStatesForEntity
//
//  PURPOSE: Use this operation to select or deselect all the entity
//           instances of the same type under the same parent.  Once the
//           entity instances have been selected, the operations
//           SetCursorFirstSelectedEntity and SetCursorNextSelectedEntity
//           allow you to advance through the selected instances to be
//           processed.
//
//  PARAMETERS: lpView               - View to Object Instance
//              cpcEntityName        - Entity to set
//              nState               - 0 deselects, 1 selects
//              cpcScopingEntityName - ScopingEntity
//
//  RETURNS: 0           - OK.
//           zCALL_ERROR - Error.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetAllSelectStatesForEntity( zVIEW   lpView,
                             zCPCHAR cpcEntityName,
                             zSHORT  nState,
                             zCPCHAR cpcScopingEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   LPENTITYINSTANCE  lpScopingEntityInstance;
   LPENTITYINSTANCE  lpSearchEntityInstance;
   zSHORT            nRC;

   // Validate parameters
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetAllSelectStatesForEntity,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   if ( nState == 0 && (cpcEntityName == 0 || cpcEntityName[ 0 ] == 0) )
   {
      // This condition causes a deselect of all SelectedInstances for the
      // current select set for a view.
      LPSELECTEDINSTANCE hSelectedInstance = lpViewCsr->hFirstSelectedInstance;
      LPSELECTEDINSTANCE hSelectedInstancePrev = 0;
      LPSELECTEDINSTANCE lpSelectedInstancePrev;
      LPSELECTEDINSTANCE lpSelectedInstance;

      while ( hSelectedInstance )
      {
         lpSelectedInstance = zGETPTR( hSelectedInstance );

         // If the entity is selected for the current select set, drop the
         // selection for the current select set.  If there are no other
         // select bits set for the selected instance, drop the instance.

         lpSelectedInstance->nSelectSet &= ~(lpViewCsr->nCurrentSelectSet);
         if ( lpSelectedInstance->nSelectSet == 0 )
         {
            if ( hSelectedInstancePrev )
            {
               lpSelectedInstancePrev = zGETPTR( hSelectedInstancePrev );
               hSelectedInstance = lpSelectedInstance->hNextSelectedInstance;
               lpSelectedInstancePrev->hNextSelectedInstance = hSelectedInstance;
            }
            else
            {
               hSelectedInstance = lpSelectedInstance->hNextSelectedInstance;
               lpViewCsr->hFirstSelectedInstance = hSelectedInstance;
            }

            fnFreeDataspace( lpSelectedInstance );
         }
         else
         {
            hSelectedInstancePrev = hSelectedInstance;
            hSelectedInstance = lpSelectedInstance->hNextSelectedInstance;
         }
      }

   // LPSELECTEDINSTANCE lpSelectedInstance;

   // while ( lpViewCsr->hFirstSelectedInstance )
   // {
   //    lpSelectedInstance = zGETPTR( lpViewCsr->hFirstSelectedInstance );
   //    lpViewCsr->hFirstSelectedInstance =
   //                      lpSelectedInstance->hNextSelectedInstance;
   //    fnFreeDataspace( lpSelectedInstance );
   // }

      fnOperationReturn( iSetAllSelectStatesForEntity, lpCurrentTask );
      return( 0 );
   }

   // If scoping is zSCOPE_OI then we're changing all occurrences of cpcEntity.
   if ( cpcScopingEntityName == zSCOPE_OI )
   {
      LPVIEWENTITY lpSearchViewEntity;

      for ( lpSearchEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );
            lpSearchEntityInstance;
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier ) )
      {
         lpSearchViewEntity = zGETPTR( lpSearchEntityInstance->hViewEntity );
         if ( lpSearchViewEntity == lpViewEntity )
         {
            // We are not checking for hidden instances here, that
            // is being done in the Farbler routine to allow for
            // dynamic garbage collection.
            fnSelectedInstanceFarbler( lpViewCsr,
                                       lpSearchViewEntity,
                                       zGETHNDL( lpSearchEntityInstance ),
                                       (zSHORT) (nState > 0 ? 2 : 3 ) );
         }
      }

      fnOperationReturn( iSetAllSelectStatesForEntity, lpCurrentTask );
      return( 0 );
   }

   // See if scoping entity is the parent of the target entity.
   if ( lpScopingViewEntityCsr )
      lpScopingEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
   else
      lpScopingViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );

   if ( zGETPTR( lpViewEntityCsr->hParent ) == lpScopingViewEntityCsr )
   {
      LPVIEWENTITY lpSearchViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );

      lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      if ( lpSearchEntityInstance )
      {
         while ( lpSearchEntityInstance->hPrevTwin )
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );

         while ( lpSearchEntityInstance )
         {
            // We are not checking for hidden instances here, that
            // is being done in the Farbler routine to allow for
            // dynamic garbage collection.
            fnSelectedInstanceFarbler( lpViewCsr,
                                       lpSearchViewEntity,
                                       zGETHNDL( lpSearchEntityInstance ),
                                       (zSHORT) (nState > 0 ? 2 : 3 ) );

            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
         }
      }
   }
   else
   {
      int nStopLevel = lpScopingViewEntityCsr->nLevel;

      lpSearchEntityInstance = zGETPTR( lpScopingEntityInstance->hNextHier );
      while ( lpSearchEntityInstance &&
              lpSearchEntityInstance->nLevel > nStopLevel )
      {
         LPVIEWENTITY lpSearchViewEntity = zGETPTR( lpSearchEntityInstance->hViewEntity );

         if ( lpSearchViewEntity == lpViewEntity &&
              lpSearchEntityInstance->nLevel == lpViewEntityCsr->nLevel )
         {
            // We are not checking for hidden instances here, that
            // is being done in the Farbler routine to allow for
            // dynamic garbage collection.
            fnSelectedInstanceFarbler( lpViewCsr, lpSearchViewEntity,
                                       zGETHNDL( lpSearchEntityInstance ),
                                       (zSHORT) (nState > 0 ? 2 : 3 ) );
         }

         lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
      }
   }

   fnOperationReturn( iSetAllSelectStatesForEntity, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=SetSelectStateOfEntityForSet
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetSelectStateOfEntityForSet
//
//  PURPOSE: This operation sets the select state of an entity for
//           a specific select set without changing the current select
//           set of the view.
//
//  PARAMETERS: lpView               - View to Object Instance
//              cpcEntityName        - Entity to set
//              nState              - 0 deselects, 1 selects
//              nSelectSet          - The select set ( 1-16 )
//
//  RETURNS: 0           - Entity Instance has been successfully
//                         selected/deselected
//           1           - Entity already selected/deselected
//           zCALL_ERROR - Error selecting entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
SetSelectStateOfEntityForSet( zVIEW   lpView,
                              zCPCHAR cpcEntityName,
                              zSHORT  nState,
                              zSHORT  nSelectSet )
{
   zSHORT nRC;

   nRC = SetSelectSetForView( lpView, nSelectSet );
   if ( nRC >= 0 )
   {
      nSelectSet = nRC;
      nRC = SetSelectStateOfEntity( lpView, cpcEntityName, nState );
      SetSelectSetForView( lpView, nSelectSet );
   }

   return( nRC );
}

//./ ADD NAME=SetSelectStateOfEntity
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   SetSelectStateOfEntity
//
//  PURPOSE: Use this operation to select or deselect a single entity
//           instance.  This provides the ability to select multiple items
//           in a list of entity instances or, in other words, to create
//           a subset list of entities to be processed as a group for
//           deletion, update, etc.  Once the entity instances have been
//           selected, the operations SetCursorFirstSelectedEntity and
//           SetCursorNextSelectedEntity allow you to advance through the
//           selected instances to be processed.  The operation
//           SetAllSelectStatesForEntity allows you to select all the
//           instances for an entity under its parent or deselect the
//           instances that have been selected.
//
//  PARAMETERS: lpView              - View to Object Instance
//              cpcEntityName       - Entity to set
//              nState              - 0 deselects, 1 selects
//
//  RETURNS: 0           - Entity Instance has been successfully
//                         selected/deselected
//           1           - Entity already selected/deselected
//           zCALL_ERROR - Error excluding entity instance
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SetSelectStateOfEntity( zVIEW   lpView,
                        zCPCHAR cpcEntityName,
                        zSHORT  nState )
{
   LPTASK             lpCurrentTask;
// LPVIEWOI           lpViewOI;
   LPVIEWENTITY       lpViewEntity;
   LPVIEWENTITYCSR    lpViewEntityCsr;
   LPENTITYINSTANCE   lpEntityInstance;
// LPENTITYINSTANCE   lpScanEntityInstance;
   zSHORT             nRC;

   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           0,
                                           &lpViewEntityCsr,
                                           0,
                                           iSetSelectStateOfEntity,
                                           lpView,
                                           cpcEntityName,
                                           0 )) != 0 )
   {
      return( nRC );
   }

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Scan to see if Entity is in the SelectedInstance chain.
   nRC = fnSelectedInstanceFarbler( zGETPTR( lpViewEntityCsr->hViewCsr ),
                                    zGETPTR( lpEntityInstance->hViewEntity ),
                                    lpViewEntityCsr->hEntityInstance, 1 );
   if ( nRC == zCALL_ERROR )
   {
      fnOperationReturn( iSetSelectStateOfEntity, lpCurrentTask );
      return( nRC );
   }

   // DGC 6/2/97
   // It's possible that fnSelectedInstanceFarbler could change the selected
   // entity instance for lpViewEntityCsr.  Just in case get it again.
   // It's possible that this is not needed--I might know if I understood
   // fnSelectedInstanceFarbler more.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   if ( nRC == 1 )
   {
      // Entity found in selected instance chain.

      if ( nState == 0 ) // deselect the instance
      {
         // remove Entity from the SelectedInstance chain
         nRC = fnSelectedInstanceFarbler( zGETPTR( lpViewEntityCsr->hViewCsr ),
                                          zGETPTR( lpEntityInstance->
                                                            hViewEntity ),
                                          lpViewEntityCsr->hEntityInstance, 3 );
         nRC = (nRC > 0) ? (nRC - 1) : zCALL_ERROR; // indicate success,
                                                      // hopefully
      }
#if 0
      else
      //  if we get here the return code is already set to
      //  indicate instance already selected
         ; // null on purpose
#endif
   }
   else
   {
      // Entity not in selected instance chain

      if ( nState ) // select the instance
      {
         // add to the chain
         nRC = fnSelectedInstanceFarbler( zGETPTR( lpViewEntityCsr->hViewCsr ),
                                          zGETPTR( lpEntityInstance->hViewEntity ),
                                          lpViewEntityCsr->hEntityInstance, 2 );
         if ( nRC == zCALL_ERROR )
         {
            fnOperationReturn( iSetSelectStateOfEntity, lpCurrentTask );
            return( nRC );
         }

         nRC = (nRC > 0) ? (nRC - 1) : zCALL_ERROR; // indicate success,
                                                    // hopefully
      }
      else
         nRC = 1; // indicate already deselected
   }

   fnOperationReturn( iSetSelectStateOfEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SetEntityCursor
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetEntityCursor
//
//  PURPOSE:    Use this operation to position on the cursor.
//
//  PARAMETERS: lpView               - Required. View to Object Instance.
//              cpcEntityName        - Required. Entity Name.
//              cpcAttributeName     - Opt. Dependent on lControl qualifiers
//                                     from 'set 1.1' (see below).
//              lControl             - Required, consists of one required
//                                     positioning control indicator, and
//                                     three optional qualifying control
//                                     indicators.
//              pQualValue           - Opt. Dependent on lControl qualifiers
//              cpcSrcEntityName     - Opt. Dependent on lControl qualifiers
//              cpcSrcAttributeName  - Opt. Dependent on lControl qualifiers
//              lRelPosition         - Opt. indicates relative position to
//                                     move to.  zPOS_RELATIVE must be
//                                     specified in lControl.
//              cpcScopingEntityName - scoping entity
//              cpcContextName       - Opt. Context name
//
//
//  Positioning Indicators
//
//    At least one indicator from the following two groups:
//
//       zero or one of the following:
//
//        zPOS_FIRST
//        zPOS_NEXT
//        zPOS_PREV
//        zPOS_LAST
//
//       and/or the following:
//
//        zPOS_RELATIVE    The value specified for lRelPosition is used in
//                         positioning the cursor.
//
//  Qualifying Indicator, Set 1.1
//
//    Zero or one of the following qualifying indicators:
//
//    (Group A)
//        zQUAL_INTEGER    pQualValue should be a pointer to an
//                         integer (zPLONG).
//        zQUAL_DECIMAL    pQualValue should be a pointer to a
//                         decimal (zPDECIMAL).
//        zQUAL_STRING     pQualValue should be a pointer to a
//                         string (zPCHAR).
//        zQUAL_ENTITYKEY  pQualValue should be a pointer to a
//                         a zULONG (zPULONG).
//        zQUAL_ATTR_NULL  Indicates to search for a NULL value in the
//                         Attribute.  Currently only Integer, Decimal and
//                         String types are supported for this qualifier.
//        zQUAL_ATTR_HIDDEN Allow cursor to be set using hidden attrs.
//        zQUAL_ENTITYATTR pQualValue should be a zVIEW value,
//                         cpcSrcEntityName should be the Entity Name,and
//                         cpcSrcAttributeName should be the Attribute Name
//                         for the qualifying View.
//             EXCEPTION,  when zQUAL_ENTITYATTR is a qualifier and the
//             SrcEntityAttribute value is a null then zQUAL_ATTR_NULL
//             must also be specified.
//
//    (Group B)
//        zQUAL_ENTITYCSR  pQualValue should be a zVIEW value,
//                         cpcSrcEntityName should be the Entity Name,and
//                         cpcSrcAttributeName should be the Attribute Name
//                         for the qualifying View.
//
//  Qualifying Indicator, Set 1.2
//
//    When a Qualifying Indicator from Set 1.1, Group A, is specified
//    then any combination of the following may be specified except
//    that only one qualifier from Set 1.2, Group B, may be selected.
//
//    If no Qualifying Indicator is specified from Set 1.2 and a
//    Qualifying Indicator from Set 1.1, Group A, is specified, then
//    a default of zQUAL_EQUAL is taken.
//
//    (Group A)
//        zQUAL_EQUAL
//        zQUAL_NOT
//
//    (Group B)
//        zQUAL_GREATER_THAN
//        zQUAL_LESS_THAN
//
//  Qualifying Indicator, Set 2
//
//    Zero or one of the following qualifying indicators may
//    be specified:
//
//        zQUAL_SELECTED
//        zQUAL_UNSELECTED
//
//  Qualifying Indicator, Set 3
//
//
//     Any of the following may be specified independently of the other
//     indicators.
//
//     zRECURS        - Indicates that recursive entities within scope
//                      will be used in the positioning process.  The
//                      objects that are in 'view' may change as internal
//                      SetViewToSubobject operations may be performed
//                      in order to establish the cursor positioning.
//     zQUAL_SCOPE_OI - override scoping entity to include the entire
//                      object instance
//     zSPARENT_KEEPCHILDREN - Indicates that when the cursor status is
//                      going to be a zCURSOR_SET status and the cursor
//                      will be on the same entity instance as when
//                      SetEntityCursor was invoked, then retain the
//                      cursors for the child instances.  Normally the
//                      cursors for the child instances will be reset
//                      to their first instance.
//
//  The Positioning Indicator, Qualifying Indicator Set 1 and Qualifying
//  Indicator Set 2 are ANDed together to determine the final cursor
//  position.
//
//
//  RETURNS: zCURSOR_NULL          - No entity instances exist
//           zCURSOR_UNCHANGED     - Entity cursor unchanged
//           zCURSOR_SET           - Entity position established
//           zCURSOR_SET_NEWPARENT - Entity position changed within
//                                   new parent
//           zCALL_ERROR           - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 11
zSHORT OPERATION
SetEntityCursor( zVIEW     lpView,
                 zCPCHAR   cpcEntityName,
                 zCPCHAR   cpcAttributeName,
                 zLONG     lControl,
                 zCPVOID   pQualValue,
                 zCPCHAR   cpcSrcEntityName,
                 zCPCHAR   cpcSrcAttributeName,
                 zLONG     lRelPosition,
                 zCPCHAR   cpcScopingEntityName,
                 zCPCHAR   cpcContextName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpSrcViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpSrcViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   LPVIEWATTRIB      lpViewEntityAttrib;
   LPVIEWATTRIB      lpSrcViewEntityAttrib;
   zLONG             lQualRequest;        // type of qualified request
   zBOOL             bScopeOI = FALSE;    // Scope OI
   zSHORT            nRC;

   // First validate the lControl field to ensure that there are no
   // conflicting specifications.
   //
   // lControl must have at least one zPOS_ indicator set...

   if ( (lControl & (zPOS_ABS | zPOS_RELATIVE)) == 0 )
      return( zCALL_ERROR );

   // Check for more than one qualifier request from 'set 1'
   lQualRequest = lControl & (zQUAL_OPERAND | zQUAL_ENTITYCSR);
   if ( lQualRequest )
   {
      if ( (-(lQualRequest) & lQualRequest) != lQualRequest &&
           lQualRequest != (zQUAL_ATTR_NULL | zQUAL_ENTITYATTR) )
      {
         return( zCALL_ERROR );
      }

      if ( (lControl & (zQUAL_GREATER_THAN | zQUAL_LESS_THAN)) ==
                                     (zQUAL_GREATER_THAN | zQUAL_LESS_THAN) )
      {
         return( zCALL_ERROR );
      }
   }

   // Another Doug special ...
   if ( lControl & (zQUAL_SCOPE_OI | zQUAL_ENTITYKEY) )
   {
      if ( (lControl & zQUAL_SCOPE_OI) ||
           cpcScopingEntityName == zSCOPE_OI ||
           cpcScopingEntityName == 0 || *cpcScopingEntityName == 0 )
      {
         cpcScopingEntityName = zSCOPE_OI;
         bScopeOI = TRUE;
      }
   }
   else
   if ( cpcScopingEntityName == zSCOPE_OI )
      bScopeOI = TRUE;

   // Validate parameters.
   if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                           &lpViewEntity,
                                           &lpScopingViewEntity,
                                           &lpViewEntityCsr,
                                           &lpScopingViewEntityCsr,
                                           iSetEntityCursor,
                                           lpView,
                                           cpcEntityName,
                                           cpcScopingEntityName )) != 0 )
   {
      return( nRC );
   }

   // If we turned on SCOPE_OI via zQUAL_ENTITYKEY, but there is no scoping
   // view entity cursor, turn it back off (dks 2004.05.03)!
   if ( (lControl & zQUAL_ENTITYKEY) &&
        (lControl & zQUAL_SCOPE_OI) == 0 && // SCOPE_OI on ... not via zQUAL_ENTITYKEY
        (lControl & zRECURS) == 0 &&
        cpcScopingEntityName == (zCPCHAR) zSCOPE_OI )
   {
      if ( lpScopingViewEntityCsr )
      {
         LPENTITYINSTANCE lpSearchEntityInstance =
                             zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
         if ( lpSearchEntityInstance == 0 || lpSearchEntityInstance->hNextHier == 0 )
         {
            cpcScopingEntityName = 0;
            bScopeOI = FALSE;
         }
      }
      else
      {
         cpcScopingEntityName = 0;
         bScopeOI = FALSE;
      }
   }

   if ( lQualRequest & zQUAL_OPERAND )
   {
      zLONG lAttribControl = 0;

      // Check to see if we should use hidden attributes.
      if ( lControl & zQUAL_ATTR_HIDDEN )
         lAttribControl = zALLOW_HIDDEN;

      if ( (lpViewEntityAttrib =
            fnValidViewAttrib( lpView, lpViewEntity,
                               cpcAttributeName, lAttribControl )) == 0 )
      {
         fnOperationReturn( iSetEntityCursor, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }
   else
      lpViewEntityAttrib = 0;

   if ( lQualRequest & (zQUAL_ENTITYCSR | zQUAL_ENTITYATTR) )
   {
      // Validate parameters
      if ( (nRC = fnValidateCursorParameters( &lpCurrentTask,
                                              &lpSrcViewEntity,
                                              0, // No scoping view entity
                                              &lpSrcViewEntityCsr,
                                              0,
                                              0, // do not pass oper id
                                              (zVIEW) pQualValue,
                                              cpcSrcEntityName,
                                              0 )) != 0 ) // no scoping entity
      {
         fnOperationReturn( iSetEntityCursor, lpCurrentTask );
         return( nRC );
      }

      if ( lQualRequest & zQUAL_ENTITYATTR )
      {
         zLONG lAttribControl = 0;

         // Check to see if we should use hidden attributes.
         if ( lControl & zQUAL_ATTR_HIDDEN )
            lAttribControl = zALLOW_HIDDEN;

         lpSrcViewEntityAttrib =
                   fnValidViewAttrib( (zVIEW) pQualValue, lpSrcViewEntity,
                                      cpcSrcAttributeName, lAttribControl );
         if ( lpSrcViewEntityAttrib == 0 )
         {
            fnOperationReturn( iSetEntityCursor, lpCurrentTask );
            return( zCALL_ERROR );
         }

         // If SrcAttribute value is null, then the zQUAL_ATTR_NULL
         // qualifier must have been specified, else error...
         if ( fnAttributeValueNull( (zVIEW) pQualValue,
                                    zGETPTR( lpSrcViewEntityCsr->hEntityInstance ),
                                    lpSrcViewEntityAttrib ) &&
              (lQualRequest & zQUAL_ATTR_NULL) == 0 )
         {
            fnOperationReturn( iSetEntityCursor, lpCurrentTask );
            return( zCALL_ERROR );
         }
         else
            lControl &= zLBITS - zQUAL_ATTR_NULL; // turn off ...
      }
      else
      {
         lpSrcViewEntityAttrib = 0;
      }

      // Make sure the source view entity and the target view entity are
      // the same Entity type.
      if ( lQualRequest & zQUAL_ENTITYCSR &&
           lpViewEntity->lEREntTok != lpSrcViewEntity->lEREntTok )
      {
         // "KZOEE268 - Source and target entity types do not match"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 268, 0,
                           lpViewEntity->szName,
                           lpSrcViewEntity->szName );
         fnOperationReturn( iSetEntityCursor, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   if ( cpcScopingEntityName == zSCOPE_OI )
   {
      LPVIEWENTITYCSR      lpTempViewEntityCsr;
      LPENTITYINSTANCE     lpTempEntityInstance;
      LPVIEWENTITYCSR      lpNextHier;
      LPVIEWENTITY         lpTempViewEntity;
      LPVIEWCSR            lpTempViewCsr;
      zPVOID               hTempViewEntityCsr;
      zPVOID               hTempEntityInstance;
      zPVOID               hTempViewEntity;

      hTempViewEntityCsr  = fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                              sizeof( ViewEntityCsrRecord ),
                                              1, 0, iViewEntityCsr );
      hTempEntityInstance = fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                              sizeof( EntityInstanceRecord ),
                                              1, 0, iEntityInstance );
      hTempViewEntity     = fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                              sizeof( ViewEntityRecord ),
                                              1, 0, iViewEntity );

      lpTempViewEntityCsr  = zGETPTR( hTempViewEntityCsr );
      lpTempEntityInstance = zGETPTR( hTempEntityInstance );
      lpTempViewEntity     = zGETPTR( hTempViewEntity );

      lpScopingViewEntityCsr = lpTempViewEntityCsr;
      lpScopingViewEntity    = lpTempViewEntity;

      lpTempViewEntityCsr->hViewEntity     = hTempViewEntity;
      lpTempViewEntityCsr->hEntityInstance = hTempEntityInstance;
   // if ( lpTempViewEntityCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "SetEntityCursor", "UNSET_CSR", -1 );

      // Set lpNextHier for ViewEntityCsr
      // Set lpScopingViewEntityCsr->hNextHier
      lpTempViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
      lpTempViewEntityCsr->hNextHier = lpTempViewCsr->hRootViewEntityCsr; // ???

      // Set lpScopingViewEntityCsr->hEntityInstance->hViewEntity.
      lpTempEntityInstance->hViewEntity = lpTempViewEntityCsr->hViewEntity;

      // set lpNextHier for ViewEntity
      // set lpScopingViewEntity->hNextHier
      lpNextHier = zGETPTR( lpTempViewEntityCsr->hNextHier );
      lpTempViewEntity->hNextHier = lpNextHier->hViewEntity;

      // Set lpNextHier for EntityInstance.
      // Set lpScopingViewEntityCsr->hEntityInstance->hNextHier.
      lpTempEntityInstance->hNextHier = lpNextHier->hEntityInstance;

      nRC = fnSetEntityCursor( lpView, lpViewEntity,
                               lpViewEntityCsr, lpViewEntityAttrib,
                               lControl, (zPVOID) pQualValue,
                               lpSrcViewEntityCsr, lpSrcViewEntityAttrib,
                               lRelPosition,
                               lpScopingViewEntityCsr,
                               cpcContextName,
                               lpCurrentTask );

      fnFreeDataspace( lpTempViewEntityCsr );
      fnFreeDataspace( lpTempEntityInstance );
      fnFreeDataspace( lpTempViewEntity );
   }
   else
   {
      nRC = fnSetEntityCursor( lpView, lpViewEntity,
                               lpViewEntityCsr, lpViewEntityAttrib,
                               lControl, (zPVOID) pQualValue,
                               lpSrcViewEntityCsr, lpSrcViewEntityAttrib,
                               lRelPosition,
                               lpScopingViewEntityCsr,
                               cpcContextName,
                               lpCurrentTask );
   }

   fnOperationReturn( iSetEntityCursor, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnSetEntityCursor
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnSetEntityCursor
//
//  PURPOSE: main bulk of the SetEntityCursor processing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 13
zSHORT
fnSetEntityCursor( zVIEW            lpView,
                   LPVIEWENTITY     lpViewEntity,
                   LPVIEWENTITYCSR  lpViewEntityCsr,
                   LPVIEWATTRIB     lpViewEntityAttrib,
                   zLONG            lControl,
                   zPVOID           pQualValue,
                   LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                   LPVIEWATTRIB     lpSrcViewEntityAttrib,
                   zLONG            lRelPosition,
                   LPVIEWENTITYCSR  lpScopingViewEntityCsr,
                   zCPCHAR          cpcContextName,
                   LPTASK           lpCurrentTask )
{
   LPENTITYINSTANCE  lpSrcEntityInstance;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   zLONG    lAbsPosition;        // type of 'absolute' positioning
   zLONG    lQualRequest;        // type of qualified request
   zLONG    lQualOperator;       // qualification operator <,>,=,<=,>=,~...
   zSHORT   nDesiredSelectState; // 0 - unselected, 1 - selected
   zBOOL    bSelectRelevant;     // Set if select state of entity is relevent
   zBOOL    bRelPosition;        // Set if relative positioning to be done
   zBOOL    bSameParentKeepChildren; // This one's for you...
   zBOOL    bRecurse;            // if Set, search recursive subobject
   zBOOL    bCursorTest;         // if Set, only the return code for setting
                                 // the cursor is set, actual cursors are not
                                 // moved.
   zVIEW    vAttrView = 0;       // Used by fnQualifiedInstance, for use
                                 // with domains and derived attributes.
   zCHAR    cDomainType;
   zCHAR    cVariableType;
   union
   {
      zDECIMAL dDecimal;
      zLONG    lInteger;
      zCHAR    szString[ 256 ];
   } unTemp;

   zLONG    lTemp;
   zSHORT   nRC;

   // Check for recursive request
   bRecurse = lControl & zRECURS ? 1 : 0;

   // Check for Test Cursor movement option
   bCursorTest = lControl & zTEST_CSR_RESULT ? 1 : 0;

   // Check for possibility of keeping the child cursors intact.
   bSameParentKeepChildren = lControl & zSPARENT_KEEPCHILDREN ? 1 : 0;

   // Check for "Entity Selected State " request
   lTemp = lControl & (zQUAL_SELECTED | zQUAL_UNSELECTED);

   // If Selected and Unselected entities are requested, then the
   // Select state of the entity is not relevent.
   if ( lTemp && (-lTemp & lTemp) == lTemp )
   {
      nDesiredSelectState = (lTemp == zQUAL_SELECTED);
      bSelectRelevant = TRUE;
   }
   else
      bSelectRelevant = FALSE;

   // Relative and Absolute positioning requests may both be specified.

   // Relative positioning requested.
   if ( lControl & zPOS_RELATIVE )
      bRelPosition = TRUE;
   else
      bRelPosition = FALSE;

   // Set Absolute positioning request.
   lAbsPosition = lControl & zPOS_ABS;

   // Set Qualification request
   lQualRequest = lControl & (zQUAL_INTEGER | zQUAL_DECIMAL | zQUAL_DATETIME |
                              zQUAL_STRING  | zQUAL_ENTITYCSR | zQUAL_ENTITYKEY |
                              zQUAL_ATTR_NULL | zQUAL_ENTITYATTR);

   // Set Qualification operator.
   if ( lQualRequest )
   {
      lQualOperator = lControl & zQUAL_OPERATOR;
      if ( (lQualOperator & (zQUAL_OPERATOR - zQUAL_NOT)) == 0 )
         lQualOperator |= zQUAL_EQUAL;
      else
      {
         if ( (lQualOperator & zQUAL_NOT) == zQUAL_NOT &&
              lQualOperator != (zQUAL_NOT | zQUAL_EQUAL) )
         {
            lQualOperator = -lQualOperator & zQUAL_OPERATOR;
         }
      }
   }

   if ( lQualRequest == zQUAL_ATTR_NULL )
      switch ( lpViewEntityAttrib->cType )
      {
         case zTYPE_INTEGER:
            pQualValue = (zPVOID) &lNullInteger;
            lQualRequest = zQUAL_INTEGER;
            break;

         case zTYPE_DECIMAL:
            SysAssignDecimalFromNull( &unTemp.dDecimal );
            pQualValue = (zPVOID) &unTemp.dDecimal;
            lQualRequest = zQUAL_DECIMAL;
            break;

         case zTYPE_STRING:
            pQualValue = (zPVOID) szNullS;
            lQualRequest = zQUAL_STRING;
            break;

         case zTYPE_DATETIME:
            pQualValue = (zPVOID) &stNullDateTime;
            lQualRequest = zQUAL_DATETIME;
            break;

         case zTYPE_BLOB:
            // Search for null attribute not supported for these attribute types.
            return( zCALL_ERROR );
      }

   // If Qualification request uses a source view, set the pointer to
   // the source entity instance.
   if ( lQualRequest & (zQUAL_ENTITYCSR | zQUAL_ENTITYATTR) )
   {
      lpSrcEntityInstance = zGETPTR( lpSrcViewEntityCsr->hEntityInstance );

      // If the search argument is a derived attribute, set it now.
      if ( lQualRequest == zQUAL_ENTITYATTR )
      {
         if ( lpSrcViewEntityAttrib->szDerivedOper[ 0 ] )
         {
            nRC = fnInvokeDerivedOperation( (zVIEW) pQualValue,
                                            zGETPTR( lpSrcViewEntityCsr->
                                                               hViewEntity ),
                                            lpSrcViewEntityAttrib,
                                            lpCurrentTask, zDERIVED_GET );
            if ( nRC == zCALL_ERROR )
               return( nRC );
         }

         if ( lpSrcViewEntityAttrib->cType != lpViewEntityAttrib->cType ||
              lpSrcViewEntityAttrib->hDomain != lpViewEntityAttrib->hDomain )
         {
            zULONG   ulTossout;

            nRC = fnGetVariableFromAttribute( (zPVOID) &unTemp,
                                              &ulTossout,
                                              zTYPE_STRING,
                                              sizeof( unTemp.szString ),
                                              (zVIEW) pQualValue,
                                              lpSrcViewEntityCsr,
                                              lpSrcViewEntityAttrib,
                                              0, lpCurrentTask, 0 );
            if ( nRC == 0 || nRC == zVAR_NULL )
            {
               zCHAR          szContext[ 33 ];
               LPTASKDOMAIN   lpTaskDomain;
               LPDOMAIN       lpDomain;

               szContext[ 0 ] = 0;
               if ( cpcContextName && *cpcContextName )
                  strcpy_s( szContext, sizeof( szContext ), cpcContextName );
               else
               if ( lpSrcViewEntityAttrib->hDomain )
               {
                  lpDomain = zGETPTR( lpSrcViewEntityAttrib->hDomain );
                  strcpy_s( szContext, sizeof( szContext ), lpDomain->szName );
               }

               cDomainType =
                 fnDomainProcessingRequired( &lpTaskDomain,
                                             lpView,
                                             lpCurrentTask,
                                             zGETPTR( lpViewEntityAttrib->hDomain ) );
               if ( cDomainType && cDomainType != zDM_TYPE_FORMAT )
               {
                  if ( cDomainType == zDM_TYPE_TABLE )
                  {
                     nRC = Table_Handler( zDME_GET_COMPARE_VALUE,
                                          zTYPE_STRING,
                                          (zPVOID) &unTemp,
                                          szContext,
                                          lpView,
                                          lpViewEntity,
                                          lpViewEntityAttrib,
                                          sizeof( unTemp ),
                                          (zPVOID) 0 );
                  }
                  else
                  {
                     nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                             zDME_GET_COMPARE_VALUE,
                                             zTYPE_STRING,
                                             (zPVOID) &unTemp,
                                             szContext,
                                             lpView,
                                             lpViewEntity,
                                             lpViewEntityAttrib,
                                             sizeof( unTemp ) );
                     if ( nRC == zDME_NOT_HANDLED )
                     {
                        zCHAR szMsg[ 500 ];

                        sprintf_s( szMsg, sizeof( szMsg ), "Operation '%s' does not handle an Entry "
                                   "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                                   zDME_GET_COMPARE_VALUE, lpDomain->szName );
                        MessageSend( lpView, "TZDMD101", "System Error",
                                     szMsg, zMSGQ_DOMAIN_ERROR, 0 );
                        return( zCALL_ERROR );
                     }
                  }

                  switch ( nRC )
                  {
                     case -2:
                        // This means we are going to do a lot of extra
                        // processing for domains (down in
                        // fnQualifiedInstance)
                        // Create 'Temporary' view for derived attribute
                        // and also for domain processing.
                        nRC = CreateViewFromViewForTask( &vAttrView, lpView, 0 );
                        if ( nRC )
                           return( nRC );

                        break;

                     case 0:
                     case 1:
                        lQualRequest = zQUAL_STRING;
                        pQualValue = &unTemp.szString;
                        cpcContextName = 0;
                        break;

                     case 2:
                        lQualRequest = zQUAL_INTEGER;
                        pQualValue = &unTemp.lInteger;
                        cpcContextName = 0;
                        break;

                     case 3:
                        lQualRequest = zQUAL_DECIMAL;
                        pQualValue = &unTemp.dDecimal;
                        cpcContextName = 0;
                        break;

                     case -1:
                        // If we get here and we're dealing with a table
                        // domain then the qual value is not a valid domain
                        // value.  Show an error message UNLESS the caller
                        // indicated that this is OK.
                        if ( cDomainType == zDM_TYPE_TABLE && (lControl & zIGNORE_ERROR_358) == 0 )
                        {
                           lpDomain = zGETPTR( lpTaskDomain->hDomain );
                           strcat_s( unTemp.szString, sizeof( unTemp.szString ), ", " );
                           strcat_s( unTemp.szString, sizeof( unTemp.szString ), lpDomain->szName );
                           strcat_s( unTemp.szString, sizeof( unTemp.szString ), ", " );
                           strcat_s( unTemp.szString, sizeof( unTemp.szString ), lpViewEntity->szName );
                           // "KZOEE358 - Value not in Context for Domain "
                           fnIssueCoreError( lpCurrentTask, lpView, 0, 358, 0,
                                             unTemp.szString,
                                             lpViewEntityAttrib->szName );
                        }

                     default:
                        // error message should have been issued by domain.
                        return( zCALL_ERROR );
                  }

               } // if ( cDomainType && cDomainType != zDM_TYPE_FORMAT )...

            } // if ( nRC == 0 || nRC == zVAR_NULL )

         } // if ( lpSrcViewEntityAttrib->cType != lpViewEntityAttrib->cType...

      } // if ( lQualRequest == zQUAL_ENTITYATTR )...

   } // if ( lQualRequest & ( zQUAL_ENTITYCSR | zQUAL_ENTITYATTR ) )...

   // If a context name is specified, see if the domain will give us a
   // simple internal data type to compare for, rather than call the
   // domain for every entity instance looked at.
   cDomainType = 0;
   if ( cpcContextName && *cpcContextName )
   {
      LPTASKDOMAIN   lpTaskDomain;
      zBOOL          bSkipDomain = FALSE;

      switch ( lQualRequest )
      {
         case zQUAL_INTEGER:
            unTemp.lInteger = *((zPLONG) pQualValue);
            cVariableType = zTYPE_INTEGER;
            break;

         case zQUAL_STRING:
            strcpy_s( unTemp.szString, sizeof( unTemp.szString ), (zPCHAR) pQualValue );
            cVariableType = zTYPE_STRING;
            break;

         case zQUAL_DECIMAL:
            unTemp.dDecimal = *((zPDECIMAL) pQualValue);
            cVariableType = zTYPE_DECIMAL;
            break;

         case zQUAL_ENTITYCSR:
         case zQUAL_ENTITYATTR:
            bSkipDomain = TRUE;
            break;

         default:
            TraceLineS( "(cm) Invalid qualification type with context:",
                        "fnSetEntityCursor" );
            return( zCALL_ERROR );
      }

      if ( lpViewEntityAttrib->hDomain && bSkipDomain == FALSE )
      {
         cDomainType = fnDomainProcessingRequired( &lpTaskDomain, lpView,
                                                   lpCurrentTask,
                                                   zGETPTR( lpViewEntityAttrib->hDomain ) );
         if ( cDomainType && cDomainType != zDM_TYPE_FORMAT )
         {
            if ( cDomainType == zDM_TYPE_TABLE )
            {
               nRC = Table_Handler( (zLONG) zDME_GET_COMPARE_VALUE,
                                    cVariableType,
                                    &unTemp,
                                    cpcContextName,
                                    lpView,
                                    lpViewEntity,
                                    lpViewEntityAttrib,
                                    sizeof( unTemp ),
                                    (zPVOID) 0 );
            }
            else
            {
               LPDOMAIN lpDomain = zGETPTR( lpViewEntityAttrib->hDomain );

               nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                       zDME_GET_COMPARE_VALUE,
                                       cVariableType,
                                       &unTemp,
                                       cpcContextName,
                                       lpView,
                                       lpViewEntity,
                                       lpViewEntityAttrib,
                                       sizeof( unTemp ) );
               if ( nRC == zDME_NOT_HANDLED )
               {
                  zCHAR szMsg[ 500 ];

                  sprintf_s( szMsg, sizeof( szMsg ), "Operation '%s' does not handle an Entry "
                             "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                             zDME_GET_COMPARE_VALUE, lpDomain->szName );
                  MessageSend( lpView, "TZDMD101", "System Error",
                               szMsg, zMSGQ_DOMAIN_ERROR, 0 );
                  return( zCALL_ERROR );
               }
            }

//         -2 - lpData ignored, this tells SetEntityCursor that
//              it should invoked domain processing with the
//              zDME_COMPARE_ATTRIBUTE entry type, each time
//              that is determining if the EntityInstance
//              matches the requested qualifications.
//         -1 - lpData invalid
//          0 - lpData unchanged, okay to use as is.
//          1 - lpData returned, string value
//          2 - lpData returned, integer value
//          3 - lpData returned, decimal value

            switch ( nRC )
            {
               case -2:
                  // Create 'Temporary' view for derived attribute
                  // and also for domain processing.
                  nRC = CreateViewFromViewForTask( &vAttrView, lpView, 0 );
                  if ( nRC )
                     return( nRC );

                  break;

               case 0:
                  cpcContextName = 0;
                  break;

               case 1:
                  cpcContextName = 0;
                  lQualRequest = zQUAL_STRING;
                  pQualValue = &unTemp.szString;
                  break;

               case 2:
                  cpcContextName = 0;
                  lQualRequest = zQUAL_INTEGER;
                  pQualValue = &unTemp.lInteger;
                  break;

               case 3:
                  cpcContextName = 0;
                  lQualRequest = zQUAL_DECIMAL;
                  pQualValue = &unTemp.dDecimal;
                  break;

               case -1:
                  if ( cDomainType == zDM_TYPE_TABLE &&
                       (lControl & zIGNORE_ERROR_358) == 0 )
                  {
                     LPDOMAIN lpDomain;

                     switch ( lQualRequest )
                     {
                        case zQUAL_STRING:
                           strcpy_s( unTemp.szString, sizeof( unTemp.szString ), (zPCHAR) pQualValue );
                           break;

                        case zQUAL_INTEGER:
                           zltoa( *((zPLONG) pQualValue), unTemp.szString, sizeof( unTemp.szString ) );
                           break;

                        case zQUAL_DECIMAL:
                           ConvertDecimalToString( unTemp.szString, 0,
                                                   *((zPDECIMAL) pQualValue),
                                                   0 );
                           break;
                     }

                     lpDomain = zGETPTR( lpTaskDomain->hDomain );

                     strcat_s( unTemp.szString, sizeof( unTemp.szString ), ", " );
                     strcat_s( unTemp.szString, sizeof( unTemp.szString ), lpDomain->szName );
                     strcat_s( unTemp.szString, sizeof( unTemp.szString ), ", " );
                     strcat_s( unTemp.szString, sizeof( unTemp.szString ), lpViewEntity->szName );
                     // "KZOEE358 - Value not in Context for Domain "
                     fnIssueCoreError( lpCurrentTask, lpView, 0, 358, 0,
                                       unTemp.szString,
                                       lpViewEntityAttrib->szName );
                  }

               default:
                  // error message should have been issued by domain.
                  return( zCALL_ERROR );
            }
         }
         else
         {
            // When we have a context and the domain is a format type, we
            // want to honor the format!   dks ... 2005.04.27
         // if ( cDomainType == zDM_TYPE_FORMAT &&
         //      cpcContextName && *cpcContextName )
         // {
         // }
         // else
            {
               cpcContextName = 0;
               // if Domain not located or
               //    no operation entry point and domain not a table type domain
               // then we fall through and function as if there is no domain
               // processing to be done...
            }
         }
      }
      else
         if ( bSkipDomain == FALSE )
            cpcContextName = 0;
   }
   else
   {
      // No context specified.  If this is a qualified request for a
      // string, integer or decimal; insure that the attribute data
      // type matches, if not try to convert the qualifying value to
      // match the attribute value.
      switch ( lQualRequest )
      {
         case zQUAL_INTEGER:
            if ( lpViewEntityAttrib->cType == zTYPE_INTEGER )
               break;

            switch ( lpViewEntityAttrib->cType )
            {
               case zTYPE_STRING:
                  // convert integer qualifier to a string
                  zltoa( *((zPLONG) pQualValue), unTemp.szString, sizeof( unTemp.szString ) );
                  lQualRequest = zQUAL_STRING;
                  pQualValue = &unTemp;
                  break;

               case zTYPE_DECIMAL:
                  // convert integer qualifier to decimal
                  SysConvertLongToDecimal( *((zPLONG) pQualValue),
                                           &unTemp.dDecimal );
                  lQualRequest = zQUAL_DECIMAL;
                  pQualValue = &unTemp;
                  break;

               default:
               {
                  zCHAR    sz[ 256 ];
                  zLONG    n;

                  // "KZOEE263 - Input Qualifier cannot be converted to
                  //  internal attribute data type"
                  strcpy_s( sz, sizeof( sz ), "(L->" );
                  n = zstrlen( sz );
                  *( sz + n++ ) = lpViewEntityAttrib->cType;
                  *( sz + n++ ) = 0;
                  strcat_s( sz, sizeof( sz ), "), " );
                  strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
                  fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz,
                                    lpViewEntityAttrib->szName );
                  return( zCALL_ERROR );
               }
            }

            break;

         case zQUAL_DATETIME:
         {
            zCHAR    sz[ 256 ];

            if ( lpViewEntityAttrib->cType == zTYPE_DATETIME )
               break;

            // "KZOEE263 - Input Qualifier cannot be converted to
            //  internal attribute data type"
            strcpy_s( sz, sizeof( sz ), "(?->DT), " );
            strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
            fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz,
                              lpViewEntityAttrib->szName );
            return( zCALL_ERROR );
         }

         case zQUAL_STRING:
            if ( lpViewEntityAttrib->cType == zTYPE_STRING )
               break;

            switch ( lpViewEntityAttrib->cType )
            {
               case zTYPE_INTEGER:
                  // convert string qualifier to integer
                  if ( pQualValue == 0 || *((zPCHAR) pQualValue) == 0 )
                  {
                     lQualRequest = zQUAL_INTEGER;
                     unTemp.lInteger = lNullInteger;
                     pQualValue = &unTemp;
                  }
                  else
                  if ( fnValidNumericString( &unTemp.lInteger,
                                             (zCPCHAR) pQualValue ) )
                  {
                     lQualRequest = zQUAL_INTEGER;
                     pQualValue = &unTemp.lInteger;
                  }
                  else
                  {
                     zCHAR    sz[ 256 ];
                     // "KZOEE263 - Input Qualifier cannot be converted to
                     //  internal attribute data type"
                     strcpy_s( sz, sizeof( sz ), "(S->L), " );
                     strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
                     fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz,
                                       lpViewEntityAttrib->szName );
                     return( zCALL_ERROR );
                  }

                  break;

               case zTYPE_DECIMAL:
                  // convert string qualifier to decimal
                  if ( pQualValue == 0 || *((zPCHAR) pQualValue) == 0 )
                  {
                     lQualRequest = zQUAL_DECIMAL;
                     SysAssignDecimalFromNull( &unTemp.dDecimal );
                     pQualValue = &unTemp;
                  }
                  else
                  if ( fnValidDecimalString( &unTemp.dDecimal,
                                             (zCPCHAR) pQualValue ) )
                  {
                     lQualRequest = zQUAL_DECIMAL;
                     pQualValue = &unTemp;
                  }
                  else
                  {
                     zCHAR    sz[ 256 ];

                     // "KZOEE263 - Input Qualifier cannot be converted to
                     //  internal attribute data type"
                     strcpy_s( sz, sizeof( sz ), "(S->M), " );
                     strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
                     fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz,
                                       lpViewEntityAttrib->szName );
                     return( zCALL_ERROR );
                  }

                  break;

               default:
               {
                  zCHAR    sz[ 256 ];
                  zLONG    n;

                  // "KZOEE263 - Input Qualifier cannot be converted to
                  //  internal attribute data type"
                  strcpy_s( sz, sizeof( sz ), "(S->" );
                  n = zstrlen( sz );
                  *( sz + n++ ) = lpViewEntityAttrib->cType;
                  *( sz + n++ ) = 0;
                  strcat_s( sz, sizeof( sz ), "), " );
                  strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
                  fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz,
                                    lpViewEntityAttrib->szName );
                  return( zCALL_ERROR );
               }
            }

            break;

         case zQUAL_DECIMAL:
            if ( lpViewEntityAttrib->cType == zTYPE_DECIMAL )
               break;

            switch ( lpViewEntityAttrib->cType )
            {
               case zTYPE_INTEGER:
                  // convert decimal qualifier to integer
                  SysConvertDecimalToLong( (zPDECIMAL) pQualValue,
                                           &unTemp.lInteger );
                  lQualRequest = zQUAL_INTEGER;
                  pQualValue = &unTemp;
                  break;

               case zTYPE_STRING:
                  // convert decimal qualifier to string
                  ConvertDecimalToString( unTemp.szString, 0,
                                          *((zPDECIMAL)pQualValue), 0 );
                  lQualRequest = zQUAL_STRING;
                  pQualValue = &unTemp;
                  break;

               default:
               {
                  zCHAR   sz[ 256 ];
                  zLONG   n;

                  // "KZOEE263 - Input Qualifier cannot be converted to
                  //  internal attribute data type"
                  strcpy_s( sz, sizeof( sz ), "(M->" );
                  n = zstrlen( sz );
                  *(sz + n++) = lpViewEntityAttrib->cType;
                  *(sz + n++) = 0;
                  strcat_s( sz, sizeof( sz ), "), " );
                  strcat_s( sz, sizeof( sz ), lpViewEntity->szName );
                  fnIssueCoreError( lpCurrentTask, lpView, 8, 263, 0, sz,
                                    lpViewEntityAttrib->szName );
                  return( zCALL_ERROR );
               }
            }

            unTemp.dDecimal = *((zPDECIMAL) pQualValue);
            break;

         case zQUAL_ENTITYCSR:
         case zQUAL_ENTITYATTR:
         default:
            break;
      }
   }

   // The following is necessary when we are not going to perform domain
   // processing, but the attribute is derived.
// if ( vAttrView == 0 &&
//      (((cpcContextName == 0 || cpcContextName[ 0 ] == 0) &&
//        lpViewEntityAttrib && lpViewEntityAttrib->szDerivedOper[ 0 ]) ||
//       (cDomainType == zDM_TYPE_FORMAT && cpcContextName && *cpcContextName)) )
   if ( vAttrView == 0 &&
        ((cpcContextName == 0 || cpcContextName[ 0 ] == 0) &&
         lpViewEntityAttrib && lpViewEntityAttrib->szDerivedOper[ 0 ]) )
   {
      // Create 'Temporary' view for derived attribute.
      nRC = CreateViewFromViewForTask( &vAttrView, lpView, 0 );
      if ( nRC )
         return( nRC );
   }

   // DGC 10/9/96 - Tried to fix a bug.  The problem occurs when we are
   // setting a cursor from an attribute.  If the target attribute has a
   // domain different from the source attribute, then fnScanEI_NoScoping
   // will try to do something with vAttrView.  The problem is that vAttrView
   // is sometimes 0.  As a hack, we will set vAttrView to something.  This
   // shouldn't have any adverse affects.  Checking for zQUAL_ENTITYATTR
   // means that pQualValue is really a view.
   if ( lQualRequest == zQUAL_ENTITYATTR && vAttrView == 0 && pQualValue &&
        lpSrcViewEntityAttrib->hDomain != lpViewEntityAttrib->hDomain )
   {
      nRC = CreateViewFromViewForTask( &vAttrView, (zVIEW) pQualValue, 0 );
      if ( nRC )
         return( nRC );
   }

   // See if scoping entity is the parent of the target entity.
   if ( lpScopingViewEntityCsr == 0 ||
        (zGETPTR( lpViewEntityCsr->hParent ) == lpScopingViewEntityCsr &&
         bRecurse == FALSE) )
   {
      nRC = fnScanEI_NoScoping( lpView,
                                lpViewEntity,
                                lpViewEntityCsr,
                                lpViewEntityAttrib,
                                pQualValue,
                                lpSrcViewEntityCsr,
                                lpSrcViewEntityAttrib,
                                lRelPosition,
                                lpScopingViewEntityCsr,
                                cpcContextName,
                                lpCurrentTask,
                                lpSrcEntityInstance,
                                vAttrView,
                                lAbsPosition,
                                lQualRequest,
                                lQualOperator,
                                nDesiredSelectState,
                                bSelectRelevant,
                                bRelPosition,
                                bSameParentKeepChildren,
                                bCursorTest );
   }
   else
   {
      nRC = fnScanEI_Scoping( lpView,
                              lpViewEntity,
                              lpViewEntityCsr,
                              lpViewEntityAttrib,
                              pQualValue,
                              lpSrcViewEntityCsr,
                              lpSrcViewEntityAttrib,
                              lRelPosition,
                              lpScopingViewEntityCsr,
                              cpcContextName,
                              lpCurrentTask,
                              lpSrcEntityInstance,
                              vAttrView,
                              lAbsPosition,
                              lQualRequest,
                              lQualOperator,
                              nDesiredSelectState,
                              bSelectRelevant,
                              bRelPosition,
                              bSameParentKeepChildren,
                              bRecurse,
                              bCursorTest );
   }

   // Drop 'Temporary' view for derived attribute and also for
   // domain processing.
   if ( vAttrView )
      DropView( vAttrView );

   // We have set the cursor so it should point to a valid entity or the
   // result is undefined.  Now is a good time to try and clean up "dead"
   // entity instances.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   if ( lpViewOI->nHiddenCount > __RECLAIM_LIMIT__ )
      fnReclaimHiddenInstances( lpViewOI );

   // Maintain the last accessed entity, so that it is possible to determine
   // which entity was accessed by the last successful "SET CURSOR".
   // Note:  we asked for the cursor to be set ...  so if it is unchanged it
   // is set!  zCURSOR_SET  // dks 2004.03.17
   if ( nRC >= zCURSOR_UNCHANGED && bCursorTest == FALSE )
      fnCallListeners( lpView, lpViewEntity->szName );

   return( nRC );
}

zSHORT
fnScanEI_NoScoping( zVIEW            lpView,
                    LPVIEWENTITY     lpViewEntity,
                    LPVIEWENTITYCSR  lpViewEntityCsr,
                    LPVIEWATTRIB     lpViewEntityAttrib,
                    zPVOID           pQualValue,
                    LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                    LPVIEWATTRIB     lpSrcViewEntityAttrib,
                    zLONG            lRelPosition,
                    LPVIEWENTITYCSR  lpScopingViewEntityCsr,
                    zCPCHAR          cpcContextName,
                    LPTASK           lpCurrentTask,
                    LPENTITYINSTANCE lpSrcEntityInstance,
                    zVIEW            vAttrView,
                    zLONG            lAbsPosition,
                    zLONG            lQualRequest,
                    zLONG            lQualOperator,
                    zSHORT           nDesiredSelectState,
                    zBOOL            bSelectRelevant,
                    zBOOL            bRelPosition,
                    zBOOL            bSameParentKeepChildren,
                    zBOOL            bCursorTest )
{
   LPENTITYINSTANCE lpSearchEntityInstance;
   zBOOL    bAbsPosition_Forward; // indicates direction of 'absolute' positioning
   zBOOL    bDoRelPosition = 0;
   zLONG    lCurrRelPosition;
   zSHORT   nRC = zCURSOR_NULL;

   // Relative positioning requested.
   lCurrRelPosition = bRelPosition ? lRelPosition : 0;

   lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   if ( lpSearchEntityInstance )
   {
      switch ( lAbsPosition )
      {
         case zPOS_FIRST:
            // Back up to beginning of chain.
            bAbsPosition_Forward = TRUE;
            while ( lpSearchEntityInstance->hPrevTwin )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );

            break;

         case zPOS_NEXT:
            // Bump to next twin.
            bAbsPosition_Forward = TRUE;
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
            break;

         case zPOS_PREV:
            // Bump to prev twin.
            bAbsPosition_Forward = FALSE;
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
            break;

         case zPOS_LAST:
            // Go to the end of the chain.
            bAbsPosition_Forward = FALSE;
            while ( lpSearchEntityInstance->hNextTwin )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );

            break;

         default:
            // Relative positioning only ...
            lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
            break;
      }

      while ( lpSearchEntityInstance )
      {
         if ( lpSearchEntityInstance->u.nInd.bHidden == FALSE )
         {
            // SelectState
            if ( (bSelectRelevant == FALSE ||
                 ((fnSelectedInstanceFarbler( zGETPTR( lpViewEntityCsr->hViewCsr ),
                                              zGETPTR( lpSearchEntityInstance->hViewEntity ),
                                              zGETHNDL( lpSearchEntityInstance ), 1 )
                              == nDesiredSelectState))) &&
                // Qualification
                // Determine if this lpSearchEntityInstance matches the
                // requested qualifications.
                 (lQualRequest == 0 ||
                  fnQualifiedInstance( vAttrView,
                                       lQualRequest,
                                       lQualOperator,
                                       pQualValue,
                                       lpSearchEntityInstance,
                                       lpViewEntityCsr,
                                       lpViewEntityAttrib,
                                       lpSrcEntityInstance,
                                       lpSrcViewEntityCsr,
                                       lpSrcViewEntityAttrib,
                                       cpcContextName,
                                       lpCurrentTask )) )
            {
               // We have satisfied Select state and Qualification criteria,
               // see if we need to perform any relative positioning ...
               if ( bRelPosition )
                  if ( lCurrRelPosition == 0 )
                     break;
                  else
                  {
                     // If we are here, we are about to perform a
                     // relative positioning.
                     bDoRelPosition = TRUE;
                  }
               else
                  break;
            }
         }

         if ( bDoRelPosition )
         {
            // Time to perform a relative positioning move ...
            if ( lCurrRelPosition > 0 )
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
               lCurrRelPosition--;
            }
            else
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
               lCurrRelPosition++;
            }

            bDoRelPosition = 0;  // reset indicator
         }
         else
         {
            if ( bAbsPosition_Forward )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
            else
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
         }
      } // end of while ( lpSearchEntityInstance )

      // We are now at the requested link in the chain, or we have
      // gone off one of the ends...
      if ( lpSearchEntityInstance )
      {
         if ( lAbsPosition == 0 && lRelPosition == 0 )
         {
            LPVIEWENTITY lpViewEntity = zGETPTR( lpSearchEntityInstance->hViewEntity );

            // This is when only Relative positioning is requested and
            // the relative position specified was zero ...
            return( CheckExistenceOfEntity( lpView, lpViewEntity->szName ) );
         }

         nRC = zCURSOR_SET;
         if ( bCursorTest == FALSE )
         {
            if ( bSameParentKeepChildren == FALSE ||
                 zGETPTR( lpViewEntityCsr->hEntityInstance ) != lpSearchEntityInstance )
            {
               lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
            // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "ScanEI_NoScoping", "UNSET_CSR", -1 );

               fnResetCursorForViewChildren( lpViewEntityCsr );
            }
         }
      }
      else
      {
         LPENTITYINSTANCE lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

         // We didn't find anything yet, so position on the first
         // unhidden entity instance and indicate zCURSOR_UNCHANGED.
         // If no unhidden entity instance exists, return zCURSOR_NULL.

         // If the current entity instance is not hidden, leave the sleeping
         // dogs alone, otherwise scan for a twin that is not hidden.
         if ( lpEntityInstance->u.nInd.bHidden )
         {
            zBOOL bFound;
            zBOOL bForward;   // True, if we have been trying to move forward
                              // in the chain.
            bFound = FALSE;
            bForward = lAbsPosition ? bAbsPosition_Forward : (lRelPosition > 0);

            lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
            while ( (bForward && lpSearchEntityInstance->hPrevTwin) ||
                    (bForward == FALSE && lpSearchEntityInstance->hNextTwin) )
            {
               lpSearchEntityInstance = bForward ?
                               zGETPTR( lpSearchEntityInstance->hPrevTwin ) :
                               zGETPTR( lpSearchEntityInstance->hNextTwin );
               if ( lpSearchEntityInstance->u.nInd.bHidden == FALSE )
               {
                  bFound = TRUE;
                  break;
               }
            }

            if ( bFound )
            {
               if ( bCursorTest == FALSE )
               {
                  lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
               // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
               //    SysMessageBox( 0, "ScanEI_NoScoping2", "UNSET_CSR", -1 );

                  fnResetCursorForViewChildren( lpViewEntityCsr );
               }

               nRC = zCURSOR_UNCHANGED;  // to my way of thinking, this should not
                                         // be UNCHANGED this should be SET, but
                                         // I'm probably wrong ... dks 2004.09.09
            }
// initialize to zCURSOR_NULL at the beginning
//          else
//             nRC = zCURSOR_NULL;
         }
         else
            nRC = zCURSOR_UNCHANGED; // Not NULL and Not Found is UNCHANGED
      }
   }

   return( nRC );
}

zSHORT
fnScanEI_Scoping( zVIEW            lpView,
                  LPVIEWENTITY     lpViewEntity,
                  LPVIEWENTITYCSR  lpViewEntityCsr,
                  LPVIEWATTRIB     lpViewEntityAttrib,
                  zPVOID           pQualValue,
                  LPVIEWENTITYCSR  lpSrcViewEntityCsr,
                  LPVIEWATTRIB     lpSrcViewEntityAttrib,
                  zLONG            lRelPosition,
                  LPVIEWENTITYCSR  lpScopingViewEntityCsr,
                  zCPCHAR          cpcContextName,
                  LPTASK           lpCurrentTask,
                  LPENTITYINSTANCE lpSrcEntityInstance,
                  zVIEW            vAttrView,
                  zLONG            lAbsPosition,
                  zLONG            lQualRequest,
                  zLONG            lQualOperator,
                  zSHORT           nDesiredSelectState,
                  zBOOL            bSelectRelevant,
                  zBOOL            bRelPosition,
                  zBOOL            bSameParentKeepChildren,
                  zBOOL            bRecurse,
                  zBOOL            bCursorTest )
{
   //
   // This processing occurs only when we have a Scoping Entity!
   //
   LPENTITYINSTANCE  lpSearchEntityInstance;
   zBOOL    bAbsPosition_Forward;// indicates direction of 'absolute' positioning
   zBOOL    bDoRelPosition = 0;
   zLONG    lCurrRelPosition;
   zSHORT   nRC = zCURSOR_NULL;

   zSHORT         nStopLevel;
   zBOOL          bNewParent;
   zBOOL          bFound;
   LPVIEWENTITY   lpRecursChild;

#ifdef  __CURSOR_PERFORMANCE__
   zBOOL          bDoTwins;
   zSHORT         nStopHier;
   LPVIEWCSR      lpViewCsr;
   LPVIEWOI       lpViewOI;
   LPVIEWENTITY   lpWkViewEntity;

   bDoTwins = !fnEntityInRecursivePath( lpViewEntity );
   nStopHier = 0;
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   if ( lpViewOI->bMultipleRootLevels == FALSE )
   {
      lpWkViewEntity = zGETPTR( lpViewEntity->hNextHier );
      while ( lpWkViewEntity && lpWkViewEntity->nLevel != 2 )
         lpWkViewEntity = zGETPTR( lpWkViewEntity->hNextHier );
      if ( lpWkViewEntity )
         nStopHier = lpWkViewEntity->nHierNbr;
   }
#endif

   // Relative positioning requested
   lCurrRelPosition = bRelPosition ? lRelPosition : 0;

   nStopLevel = lpScopingViewEntityCsr->nLevel;
   bNewParent = 0;
   bFound = FALSE;

   switch ( lAbsPosition )
   {
      case zPOS_FIRST:
         bAbsPosition_Forward = TRUE;
         lpSearchEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
         if ( lpSearchEntityInstance )
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );

         // go to the start of the chain
         while ( lpSearchEntityInstance &&
                 lpSearchEntityInstance->nLevel > nStopLevel )
         {
            if ( lpSearchEntityInstance->hPrevTwin )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
            else
            {
               LPENTITYINSTANCE lpPrevHier = zGETPTR( lpSearchEntityInstance->hPrevHier );

               if ( lpPrevHier && lpPrevHier->nLevel > nStopLevel )
                  lpSearchEntityInstance = lpPrevHier;
               else
                  break;
            }
         }

         break;

      case zPOS_NEXT:
         bAbsPosition_Forward = TRUE;
         lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
         if ( lpSearchEntityInstance )
         {
            if ( bRecurse )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
            else
            if ( lpSearchEntityInstance->hNextTwin )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
            else
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
               bNewParent = TRUE;
            }
         }

         break;

      case zPOS_PREV:
         bAbsPosition_Forward = 0;
         lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
         if ( lpSearchEntityInstance )
         {
            if ( bRecurse )
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
            }
            else
            if ( lpSearchEntityInstance->hPrevTwin )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
            else
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
               bNewParent = TRUE;
            }
         }

         break;

      case zPOS_LAST:
         bAbsPosition_Forward = 0;
         lpSearchEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );

         if ( lpSearchEntityInstance )
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );

         // Go to the end of the chain.
         while ( lpSearchEntityInstance &&
                 lpSearchEntityInstance->nLevel > nStopLevel )
         {
            if ( lpSearchEntityInstance->hNextTwin )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
            else
            {
               LPENTITYINSTANCE lpNextHier = zGETPTR( lpSearchEntityInstance->hNextHier );

               if ( lpNextHier && lpNextHier->nLevel > nStopLevel )
                  lpSearchEntityInstance = lpNextHier;
               else
                  break;
            }
         }

         break;

      default:
         // Relative positioning only ...
         lpSearchEntityInstance  = zGETPTR( lpViewEntityCsr->hEntityInstance );
         break;
   }

   lpRecursChild = 0;
   if ( bRecurse )
   {
      lpRecursChild = zGETPTR( lpViewEntity->hNextHier );
      while ( lpRecursChild &&
              lpRecursChild->nLevel > lpViewEntity->nLevel &&
              (lpRecursChild->lEREntTok != lpViewEntity->lEREntTok ||
               lpRecursChild->bRecursive == FALSE) )
      {
         lpRecursChild = zGETPTR( lpRecursChild->hNextHier );
      }

      if ( lpRecursChild == 0 || lpRecursChild->nLevel <= lpViewEntity->nLevel )
         lpRecursChild = 0;
   }


   while ( lpSearchEntityInstance &&
           lpSearchEntityInstance->nLevel > nStopLevel )
   {
      LPVIEWENTITY   lpSrchViewEntity;

      lpSrchViewEntity = zGETPTR( lpSearchEntityInstance->hViewEntity );
      if ( lpSearchEntityInstance->u.nInd.bHidden == FALSE &&

           (lpSrchViewEntity == lpViewEntity ||
            lpSrchViewEntity == lpRecursChild) &&

           (bRecurse ||
            lpSearchEntityInstance->nLevel == lpViewEntityCsr->nLevel) &&

           // SelectState
           (bSelectRelevant == FALSE ||
            (fnSelectedInstanceFarbler( zGETPTR( lpViewEntityCsr->hViewCsr ),
                                        zGETPTR( lpSearchEntityInstance->hViewEntity ),
                                        zGETHNDL( lpSearchEntityInstance ), 1 )
                                                   == nDesiredSelectState)) &&

           // Qualification
           (lQualRequest == 0 ||
            fnQualifiedInstance( vAttrView,
                                 lQualRequest,
                                 lQualOperator,
                                 pQualValue,
                                 lpSearchEntityInstance,
                                 lpViewEntityCsr,
                                 lpViewEntityAttrib,
                                 lpSrcEntityInstance,
                                 lpSrcViewEntityCsr,
                                 lpSrcViewEntityAttrib,
                                 cpcContextName,
                                 lpCurrentTask )) )
      {
         // We have satisfied Select state and Qualification criteria, see
         // if we need to perform any relative positioning ...
         if ( bRelPosition )
         {
            if ( lCurrRelPosition == 0 )
            {
               bFound = TRUE;
               break;
            }
            else
            {
               // If we are in here, we are about to perform relative positioning.
               bDoRelPosition = TRUE;
            }
         }
         else
         {
            bFound = TRUE;
            break;
         }
      }

      if ( bDoRelPosition )
      {
         // Time to perform a relative positioning move ...
         if ( lCurrRelPosition > 0 )
         {
            if ( bRecurse )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
            else
            if ( lpSearchEntityInstance->hNextTwin )
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
            else
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
               bNewParent = TRUE;
            }

            lCurrRelPosition--;
         }
         else
         {
            if ( bRecurse )
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
            }
            else
            {
               if ( lpSearchEntityInstance->hPrevTwin )
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
               else
               {
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
                  bNewParent = TRUE;
               }
            }

            lCurrRelPosition++;
         }

         bDoRelPosition = 0;  // reset indicator
      }
      else
      {
         //
         // perform absolute positioning fore or aft...
         //
         if ( bAbsPosition_Forward )
         {
            if ( bRecurse )
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
#ifdef  __CURSOR_PERFORMANCE__
               if ( nStopHier && lpSearchEntityInstance )
               {
                  LPVIEWENTITY lpTempViewEntity = zGETPTR( lpSearchEntityInstance->hViewEntity );

                  if ( lpTempViewEntity->nHierNbr >= nStopHier )
                     lpSearchEntityInstance = 0;
               }
#endif
            }
            else
            {
#ifdef  __CURSOR_PERFORMANCE__
               if ( bDoTwins && lpSrchViewEntity == lpViewEntity )
               {
                  if ( lpSearchEntityInstance->hNextTwin )
                  {
                     lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
                  }
                  else
                  {
                     bNewParent = TRUE;
                     while ( lpSearchEntityInstance &&
                             lpSearchEntityInstance->hNextTwin == 0 )
                     {
                        lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hParent );
                     }

                     if ( lpSearchEntityInstance )
                     {
                        lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
                     }
                  }
               }
               else
#endif
               if ( lpSrchViewEntity == lpViewEntity &&
                    lpSearchEntityInstance->nLevel == lpViewEntityCsr->nLevel &&
                    lpSearchEntityInstance->hNextTwin )
               {
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
               }
               else
               {
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
                  bNewParent = TRUE;
#ifdef  __CURSOR_PERFORMANCE__
                  if ( nStopHier && lpSearchEntityInstance )
                  {
                     LPVIEWENTITY lpTempViewEntity =
                              zGETPTR( lpSearchEntityInstance->hViewEntity );

                     if ( lpTempViewEntity->nHierNbr >= nStopHier )
                        lpSearchEntityInstance = 0;
                  }
#endif
               }
            }
         }
         else
         {
            if ( bRecurse )
            {
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
            }
            else
            {
               if ( lpSrchViewEntity == lpViewEntity &&
                    lpSearchEntityInstance->nLevel == lpViewEntityCsr->nLevel &&
                    lpSearchEntityInstance->hPrevTwin )
               {
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
               }
               else
               {
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevHier );
                  bNewParent = TRUE;
               }
            }
         }
      }
   } // end of while (lpSearchEntityInstance)
   //      for when we have a scoping entity...

   if ( bFound )
   {
      if ( bRecurse )
      {
         LPVIEWENTITYCSR   lpTempEntityCsr;
         LPENTITYINSTANCE  lpFoundEntityInstance;
         zBOOL             bRebuild;

         nRC = zCURSOR_SET;
         bRebuild = FALSE;
         lpFoundEntityInstance = lpSearchEntityInstance;

         // First, try scanning up the to see if parentage and
         // ViewSubobjects are intact, if not chuck it out and then
         // rebuild it.
         if ( lpSearchEntityInstance->nLevel == lpViewEntityCsr->nLevel )
         {
            if ( bCursorTest == FALSE )
            {
               if ( bSameParentKeepChildren == FALSE ||
                    zGETPTR( lpViewEntityCsr->hEntityInstance ) != lpSearchEntityInstance )
               {
                  lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
                  fnResetCursorForViewChildren( lpViewEntityCsr );
               }
            }

            lpTempEntityCsr = lpViewEntityCsr;
            while ( lpTempEntityCsr->hParent &&
                    zGETPTR( lpTempEntityCsr->hParent ) != lpScopingViewEntityCsr )
            {
               lpTempEntityCsr = zGETPTR( lpTempEntityCsr->hParent );
               lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hParent );

               if ( (lpTempEntityCsr->hEntityInstance == UNSET_CSR ) ||
                    (zGETPTR( lpTempEntityCsr->hEntityInstance ) !=
                                                lpSearchEntityInstance ) ||
                    (lpTempEntityCsr->nLevel != lpSearchEntityInstance->nLevel ) )
               {
                  bRebuild = TRUE;
                  break;
               }
            }
         }
         else
            bRebuild = TRUE;

         // if the entity instance level matches the view entity csr level,
         // then we should be okay.
         if ( bRebuild )
         {
         // TraceLineS( "(cm) Levels do NOT match", " leveling..." );
            nRC = zCURSOR_SET_NEWPARENT;
            if ( bCursorTest == FALSE )
               fnEstablishViewForInstance( lpView, lpViewEntityCsr,
                                           lpFoundEntityInstance );
         }
      }
      else
      {
         nRC = zCURSOR_SET;
         if ( lpSearchEntityInstance == zGETPTR( lpViewEntityCsr->hEntityInstance ) )
         {
            if ( bCursorTest == FALSE && bSameParentKeepChildren == FALSE )
               fnResetCursorForViewChildren( lpViewEntityCsr );
         }
         else
         {
            LPENTITYINSTANCE lpResetInstance;
            LPVIEWENTITYCSR  lpParent;
            LPVIEWENTITYCSR  lpResetCsr;

            lpResetCsr      = lpViewEntityCsr;
            lpResetInstance = lpSearchEntityInstance;
            lpParent        = zGETPTR( lpResetCsr->hParent );

            // Need to check for presence of parent, in order to
            // handle multiple roots.
            while ( lpParent &&
                    lpParent->hEntityInstance != lpResetInstance->hParent )
            {
               lpResetCsr      = zGETPTR( lpResetCsr->hParent );
               lpResetInstance = zGETPTR( lpResetInstance->hParent );
               lpParent        = zGETPTR( lpResetCsr->hParent );
            }

            if ( bCursorTest )
            {
               if ( lpResetCsr != lpViewEntityCsr )
                  nRC = zCURSOR_SET_NEWPARENT;
            }
            else
            {
               lpResetCsr->hEntityInstance = zGETHNDL( lpResetInstance );
            // if ( lpResetCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "ScanEI_Scoping", "UNSET_CSR", -1 );

               fnResetCursorForViewChildren( lpResetCsr );
               if ( lpResetCsr != lpViewEntityCsr )
               {
                  nRC = zCURSOR_SET_NEWPARENT;
                  lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
               // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
               //    SysMessageBox( 0, "ScanEI_Scoping2", "UNSET_CSR", -1 );

                  lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );
                  lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hParent );
                  while ( lpViewEntityCsr != lpResetCsr )
                  {
                     lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
                  // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
                  //    SysMessageBox( 0, "ScanEI_Scoping3", "UNSET_CSR", -1 );

                     lpViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );
                     lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hParent );
                  }
               }
            }
         }
      }
   }
   else
   {
      // We didn't find anything yet, so position on the first
      // unhidden entity instance and indicate zCURSOR_UNCHANGED.
      // If no unhidden entity instance exists, return zCURSOR_NULL.

      // If the current entity instance is not hidden, leave the
      // sleeping dogs alone, otherwise scan for a twin that is not hidden.
      lpSearchEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      if ( lpSearchEntityInstance && lpSearchEntityInstance->u.nInd.bHidden )
      {
         zBOOL bFound;
         zBOOL bForward;   // True, if we have been trying to move
                           // forward in the chain

         bFound = FALSE;
         bForward = lAbsPosition ? bAbsPosition_Forward :
                                   (lRelPosition > 0);

         while ( (bForward && lpSearchEntityInstance->hPrevTwin) ||
                 (bForward == FALSE && lpSearchEntityInstance->hNextTwin) )
         {
            lpSearchEntityInstance = bForward ?
                               zGETPTR( lpSearchEntityInstance->hPrevTwin ) :
                               zGETPTR( lpSearchEntityInstance->hNextTwin );
            if ( lpSearchEntityInstance->u.nInd.bHidden == FALSE )
            {
               bFound = TRUE;
               break;
            }
         }

         if ( bFound )
         {
            if ( bCursorTest == FALSE )
            {
               lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
            // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
            //    SysMessageBox( 0, "ScanEI_Scoping4", "UNSET_CSR", -1 );

               fnResetCursorForViewChildren( lpViewEntityCsr );
            }

            nRC = zCURSOR_UNCHANGED;
         }
// set at beginning
//       else
//          nRC = zCURSOR_NULL;
      }
      else
      if ( lpViewEntityCsr->hEntityInstance )
         nRC = zCURSOR_UNCHANGED; // Not NULL and Not found is UNCHANGED

   } // end of if ( bFound )...

   return( nRC );
}

//./ ADD NAME=fnQualifiedInstance
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnQualifiedInstance
//
//  PURPOSE: To determine if an entity instance matches the condition
//           as specified by the Qualification Request type.
//
//  RETURNS: 1 - Match
//           0 - No match
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 13
zSHORT
fnQualifiedInstance( zVIEW             vAttrView,
                     zLONG             lQualRequest,
                     zLONG             lQualOperator,
                     zPVOID            pQualValue,
                     LPENTITYINSTANCE  lpSearchEntityInstance,
                     LPVIEWENTITYCSR   lpViewEntityCsr,
                     LPVIEWATTRIB      lpViewAttrib,
                     LPENTITYINSTANCE  lpSrcEntityInstance,
                     LPVIEWENTITYCSR   lpSrcViewEntityCsr,
                     LPVIEWATTRIB      lpSrcViewAttrib,
                     zCPCHAR           cpcContextName,
                     LPTASK            lpCurrentTask )
{
   zSHORT   nRC;

   if ( lpViewAttrib && lpViewAttrib->szDerivedOper[ 0 ] &&
        lQualRequest != zQUAL_ENTITYCSR &&
        lQualRequest != zQUAL_ENTITYATTR &&
        !(lpViewAttrib->hDomain && cpcContextName && cpcContextName[ 0 ]) )
   {
      LPVIEWENTITYCSR lpViewEntityCsrDA =
                        fnEstablishViewForInstance( vAttrView, 0,
                                                    lpSearchEntityInstance );
      nRC = fnInvokeDerivedOperation( vAttrView,
                                      zGETPTR( lpViewEntityCsrDA->hViewEntity ),
                                      lpViewAttrib,
                                      lpCurrentTask, zDERIVED_GET );
      if ( nRC == zCALL_ERROR )
         return( nRC );
   }

   switch ( lQualRequest )
   {
      case zQUAL_ENTITYKEY:
         return( lpSearchEntityInstance->ulKey == *((zPULONG) pQualValue) );

      case zQUAL_INTEGER:
         if ( lpViewAttrib->hDomain && cpcContextName && *cpcContextName )
         {
            LPVIEWENTITYCSR lpViewEntityCsrForDomain =
                        fnEstablishViewForInstance( vAttrView, 0,
                                                    lpSearchEntityInstance );
            nRC = fnCompareAttributeToVariable( vAttrView,
                                                lpViewEntityCsrForDomain,
                                                lpViewAttrib, pQualValue,
                                                zTYPE_INTEGER, 0,
                                                cpcContextName,
                                                lpCurrentTask, 0 );
            return( fnQualOperatorMatchesRC( lQualOperator, nRC ) );
         }

         return( fnIntegerMatchesAttributeValue( *((zPLONG) pQualValue),
                                                 lQualOperator,
                                                 lpSearchEntityInstance,
                                                 lpViewAttrib ) );

      case zQUAL_DECIMAL:
         if ( lpViewAttrib->hDomain && cpcContextName && *cpcContextName )
         {
            LPVIEWENTITYCSR lpViewEntityCsrForDomain =
                        fnEstablishViewForInstance( vAttrView, 0,
                                                    lpSearchEntityInstance );
            nRC = fnCompareAttributeToVariable( vAttrView,
                                                lpViewEntityCsrForDomain,
                                                lpViewAttrib, pQualValue,
                                                zTYPE_DECIMAL, 0,
                                                cpcContextName, lpCurrentTask, 0 );
            return( fnQualOperatorMatchesRC( lQualOperator, nRC ) );
         }

         return( fnDecimalMatchesAttributeValue( *((zPDECIMAL) pQualValue),
                                                 lQualOperator,
                                                 lpSearchEntityInstance,
                                                 lpViewAttrib ) );

      case zQUAL_DATETIME:
         if ( lpViewAttrib->hDomain && cpcContextName && *cpcContextName )
         {
            LPVIEWENTITYCSR lpViewEntityCsrForDomain =
                        fnEstablishViewForInstance( vAttrView, 0,
                                                    lpSearchEntityInstance );
            nRC = fnCompareAttributeToVariable( vAttrView,
                                                lpViewEntityCsrForDomain,
                                                lpViewAttrib, pQualValue,
                                                zTYPE_DATETIME,
                                                0, cpcContextName,
                                                lpCurrentTask, 0 );
            return( fnQualOperatorMatchesRC( lQualOperator, nRC ) );
         }

         return( fnDateTimeMatchesAttributeValue( (LPDTINTERNAL) pQualValue,
                                                  lQualOperator,
                                                  lpSearchEntityInstance,
                                                  lpViewAttrib ) );

      case zQUAL_STRING:
         if ( lpViewAttrib->hDomain && cpcContextName && *cpcContextName )
         {
            LPVIEWENTITYCSR lpViewEntityCsrForDomain =
                        fnEstablishViewForInstance( vAttrView, 0,
                                                    lpSearchEntityInstance );
            nRC = fnCompareAttributeToVariable( vAttrView,
                                                lpViewEntityCsrForDomain,
                                                lpViewAttrib, pQualValue,
                                                zTYPE_STRING,
                                                0, cpcContextName,
                                                lpCurrentTask, 0 );
            return( fnQualOperatorMatchesRC( lQualOperator, nRC ) );
         }

         return( fnStringMatchesAttributeValue( (zCPCHAR) pQualValue,
                                                lQualOperator,
                                                lpSearchEntityInstance,
                                                lpViewAttrib ) );

      case zQUAL_ENTITYCSR:
         return( fnInstanceMatchesInstance( lpSrcEntityInstance,
                                            lpSearchEntityInstance ) );

      case zQUAL_ENTITYATTR:
         return( fnAttrValueMatchesAttrValue( vAttrView,
                                              lQualOperator,
                                              lpSrcViewEntityCsr,
                                              lpSrcEntityInstance,
                                              lpSrcViewAttrib,
                                              lpViewEntityCsr,
                                              lpSearchEntityInstance,
                                              lpViewAttrib,
                                              lpCurrentTask ) );
   }

   return( 0 );
}

//./ ADD NAME=fnSelectedInstanceFarbler
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: fnSelectedInstanceFarbler
//
//  PURPOSE:  Used to perform functions against the chain of
//            SelectedInstance objects.
//
//  PARAMETERS:
//            lpViewEntityCsr - ViewEntityCsr of the EntityInstance
//            nFunction       - 1 Find SelectedInstance for current select
//                                set
//                              2 Add SelectedInstance for current select
//                                set
//                              3 Delete SelectedInstance for current
//                                select set
//
//  RETURNS:  0 - function could not be performed;
//                SelectedInstance not found
//            1 - function successfully performed;
//                SelectedInstance found
//            2 - function, in essence, successfully performed
//          zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnSelectedInstanceFarbler( LPVIEWCSR        lpViewCsr,
                           LPVIEWENTITY     lpViewEntity,
                           LPENTITYINSTANCE hEntityInstance,
                           zSHORT           nFunction )
{
   LPSELECTEDINSTANCE lpSrchSelectedInstance;
   LPSELECTEDINSTANCE lpPrevSelectedInstance;
   LPSELECTEDINSTANCE lpPickSelectedInstance;
   LPVIEWENTITY       hViewEntity = zGETHNDL( lpViewEntity );
   zSHORT             nRC;

   lpSrchSelectedInstance = zGETPTR( lpViewCsr->hFirstSelectedInstance );
   lpPrevSelectedInstance = 0;
   nRC = 0;    // default to not successful

   // Scan through the SelectedInstance chain.
   while ( lpSrchSelectedInstance )
   {
      if ( hEntityInstance == lpSrchSelectedInstance->xEntityInstance &&
           hViewEntity == lpSrchSelectedInstance->hViewEntity )
      {
         LPENTITYINSTANCE lpEntityInstance = zGETPTR( hEntityInstance );

         // We have found the SelectedInstance, currently in the chain
         switch ( nFunction )
         {
            case 1: // search function
               if ( lpEntityInstance->nTableID != iEntityInstance ||
                    lpEntityInstance->u.nInd.bHidden )
               {
                  // We've found the selected instance and the
                  // instance is either hidden or invalid, delete the
                  // select instance and pretend we never found it
                  if ( lpPrevSelectedInstance )
                  {
                     lpPrevSelectedInstance->hNextSelectedInstance =
                        lpSrchSelectedInstance->hNextSelectedInstance;
                     lpPickSelectedInstance =
                        zGETPTR( lpPrevSelectedInstance->hNextSelectedInstance );
                  }
                  else
                  {
                     lpViewCsr->hFirstSelectedInstance =
                        lpSrchSelectedInstance->hNextSelectedInstance;
                     lpPickSelectedInstance =
                        zGETPTR( lpViewCsr->hFirstSelectedInstance );
                  }

                  fnFreeDataspace( lpSrchSelectedInstance );
                  nRC = -1;

                  // Error message?
               }
               else
               {
                  // find is successful, make sure the instance is selected
                  // for the requested select set
                  if ( lpViewCsr->nCurrentSelectSet &
                           lpSrchSelectedInstance->nSelectSet )
                  {
                     nRC = 1; // find was successful
                  }
                  else
                     nRC = 0; // find was un-successful
               }

               break;

            case 2: // add function
               if ( lpEntityInstance->nTableID != iEntityInstance ||
                    lpEntityInstance->u.nInd.bHidden )
               {
                  // Drop from chain.
                  if ( lpPrevSelectedInstance )
                  {
                     lpPrevSelectedInstance->hNextSelectedInstance=
                        lpSrchSelectedInstance->hNextSelectedInstance;
                     lpPickSelectedInstance =
                        zGETPTR( lpPrevSelectedInstance->hNextSelectedInstance );
                  }
                  else
                  {
                     lpViewCsr->hFirstSelectedInstance =
                        lpSrchSelectedInstance->hNextSelectedInstance;
                     lpPickSelectedInstance =
                        zGETPTR( lpViewCsr->hFirstSelectedInstance );
                  }

                  fnFreeDataspace( lpSrchSelectedInstance );
                  nRC = -1;

                  // Error message, if any, should go in next switch statement.
               }
               else
               {
                  lpSrchSelectedInstance->nSelectSet |=
                                                lpViewCsr->nCurrentSelectSet;
                  nRC = 2;
               }

               break;

            case 3: // delete function
               // Drop from chain.
               lpSrchSelectedInstance->nSelectSet &=
                                     (0xffff - lpViewCsr->nCurrentSelectSet);
               if ( lpSrchSelectedInstance->nSelectSet == 0 )
               {
                  if ( lpPrevSelectedInstance )
                     lpPrevSelectedInstance->hNextSelectedInstance=
                        lpSrchSelectedInstance->hNextSelectedInstance;
                  else
                     lpViewCsr->hFirstSelectedInstance =
                        lpSrchSelectedInstance->hNextSelectedInstance;

                  fnFreeDataspace( lpSrchSelectedInstance );
               }

               nRC = 1; // deselect was successful
               break;
         }
      }

      // -1 occurs on a search/add function and the EntityInstance is
      // hidden.
      if ( nRC == -1 )
      {
         lpSrchSelectedInstance = lpPickSelectedInstance;
         nRC = 0;
      }
      else
      {
         if ( nRC )
            break;

         lpPrevSelectedInstance = lpSrchSelectedInstance;
         lpSrchSelectedInstance =
            zGETPTR( lpSrchSelectedInstance->hNextSelectedInstance );
      }
   }

   // If nRC is 0, then the EntityInstance is not currently in the chain.
   if ( nRC == 0 )
   {
      LPTASK   lpOwningTask;
      LPVIEWOI lpViewOI;
      zPVOID   hInstance;

      switch ( nFunction )
      {
         case 1: // search function
            break;

         case 2: // add function
            //
            // Adding to the SelectedInstance chain...
            //
            lpViewOI     = zGETPTR( lpViewCsr->hViewOI );
            lpOwningTask = zGETPTR( lpViewOI->hTask );

            hInstance = fnAllocDataspace( lpOwningTask->hFirstDataHeader,
                                          sizeof( SelectedInstanceRecord ),
                                          1, 0, iSelectedInstance );
            lpSrchSelectedInstance = zGETPTR( hInstance );
            if ( lpSrchSelectedInstance )
            {
               if ( lpPrevSelectedInstance )
                  lpPrevSelectedInstance->hNextSelectedInstance = hInstance;
               else
                  lpViewCsr->hFirstSelectedInstance = hInstance;

               lpSrchSelectedInstance->hViewEntity     = hViewEntity;
               lpSrchSelectedInstance->xEntityInstance = hEntityInstance;
               lpSrchSelectedInstance->nSelectSet = lpViewCsr->nCurrentSelectSet;

               nRC = 1; // select was successful
            }
            else
            {
               // "KZOEE012 - Error allocating memory"
               zVIEW lpView = zGETPTR( lpViewCsr->hView );
               fnIssueCoreError( zGETPTR( lpView->hTask ), lpView,
                                 20, 12, 0, 0, 0 );
               return( zCALL_ERROR );
            }

            break;

         case 3: // delete function
            nRC = 2;
            break;
      }
   }

   return( nRC );
}

//./ ADD NAME=fnValidateCursorParameters
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnValidateCursorParameters
//
//  PURPOSE: To validate the standard parameters for a cursor
//           manipulation call
//
//  RETURNS: 0 - Attribute parameters valid
//           zCURSOR_UNDEFINED Cursor is undefined
//           -16 - Error validating the attribute parameters
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 10
zSHORT
fnValidateCursorParameters( LPTASK           *plpReturnTask,
                            LPVIEWENTITY     *ppvReturnViewEntity,
                            LPVIEWENTITY     *plpReturnScopingViewEntity,
                            LPVIEWENTITYCSR  *ppvReturnViewEntityCsr,
                            LPVIEWENTITYCSR  *plpReturnScopingViewEntityCsr,
                            zSHORT           nOperationID,
                            zVIEW            lpView,
                            zCPCHAR          cpcEntityName,
                            zCPCHAR          cpcScopingEntity )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpScopingViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITYCSR   lpScopingViewEntityCsr;
   LPVIEWENTITYCSR   lpWorkViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpScopingEntityInstance;
   zBOOL             bOperationIdxSet = FALSE;

   // If task not active or disabled, return zCALL_ERROR.
   if ( nOperationID == 0 )
   {
      lpCurrentTask = *plpReturnTask;
      if ( fnValidViewCsr( lpCurrentTask, lpView ) == 0 )
         return( zCALL_ERROR );
   }
   else
   {
      if ( (lpCurrentTask = fnOperationCall( nOperationID, lpView,
                                             zVALID_VIEW_CSR )) == 0 )
      {
         return( zCALL_ERROR );
      }

      bOperationIdxSet = TRUE;
      *plpReturnTask = lpCurrentTask;
   }

   // Get the view Object Instance for the view.
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   // Validate that the entity exists.
   lpViewEntity    = 0;
   lpViewEntityCsr = 0;
   if ( cpcEntityName &&
        (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      if ( bOperationIdxSet )
         fnOperationReturn( nOperationID, lpCurrentTask );

      return( zCALL_ERROR );
   }

   // Validate that the entity scoping exists and is valid.
   lpScopingViewEntity    = 0;
   lpScopingViewEntityCsr = 0;
   if ( lpViewEntity && (zLONG) cpcScopingEntity != (zLONG) zSCOPE_OI )
   {
#if 0 // DGC 1998.8.13 -- Following no longer needed?
      if ( (zLONG) cpcScopingEntity == (zLONG) zSCOPE_OI )
      {
         if ( lpViewOI->bMultipleRootLevels )
            ; // should be ok, continue
         else
         {
            //  "KZOEE105 - Invalid scoping Entity name for View"
            fnIssueCoreError( lpTask, lpView, 8, 105, 0,
                              lpViewEntity->szName, 0 );
            if ( bOperationIdxSet )
               fnOperationReturn( nOperationID, lpCurrentTask );

            return( zCALL_ERROR );
         }
      }
      else
#endif

      // If the scoping entity is the same as the target entity then we'll
      // just assume that there is no scoping entity.
      if ( cpcScopingEntity && zstrcmp( cpcScopingEntity, cpcEntityName ) == 0 )
         cpcScopingEntity = 0;

      if ( cpcScopingEntity && cpcScopingEntity[ 0 ] )
      {
         if ( (lpScopingViewEntity =
                         fnValidViewEntity( &lpScopingViewEntityCsr,
                                            lpView, cpcScopingEntity, 0 )) == 0 )
         {
            if ( bOperationIdxSet )
               fnOperationReturn( nOperationID, lpCurrentTask );

            return( zCALL_ERROR );
         }

         lpWorkViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );
         while ( lpWorkViewEntityCsr &&
                 lpWorkViewEntityCsr != lpScopingViewEntityCsr )
         {
            lpWorkViewEntityCsr = zGETPTR( lpWorkViewEntityCsr->hParent );
         }

         if ( lpWorkViewEntityCsr == 0 )
         {
            LPTASK lpTask = zGETPTR( lpView->hTask );
            //  "KZOEE105 - Invalid scoping Entity name for View"
            fnIssueCoreError( lpTask, lpView, 8, 105, 0,
                              lpViewEntity->szName, 0 );
            if ( bOperationIdxSet )
               fnOperationReturn( nOperationID, lpCurrentTask );

            return( zCALL_ERROR );
         }
      }
      else
      {
         lpScopingViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );
         lpScopingViewEntity    = zGETPTR( lpViewEntity->hParent );
         while ( lpScopingViewEntityCsr && lpScopingViewEntity->bHidden )
         {
            lpScopingViewEntityCsr = zGETPTR( lpScopingViewEntityCsr->hParent );
            lpScopingViewEntity    = zGETPTR( lpScopingViewEntity->hParent );
         }
      }
   }

   // Validate that operation is consistent with hierarchical mode
   // of cursor.
   if ( lpViewCsr->hHierRootEntityInstance )
   {
      LPENTITYINSTANCE lpHierRootEntityInstance =
                           zGETPTR( lpViewCsr->hHierRootEntityInstance );

      if ( nOperationID < iDefineHierarchicalCursor &&
           nOperationID != iGetAbsolutePositionForEntity )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         // "KZOEE264 - Invalid Operation for Hierarchical processing"
         fnIssueCoreError( lpTask, lpView, 8, 264, 0, 0, 0 );
         if ( bOperationIdxSet )
            fnOperationReturn( nOperationID, lpCurrentTask );

         return( zCALL_ERROR );
      }

      if ( lpHierRootEntityInstance->u.nInd.bHidden )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         // "KZOEE267 - Hierarchical cursor is undefined"
         fnIssueCoreError( lpTask, lpView, 8, 267, 0, 0, 0 );
         if ( bOperationIdxSet )
            fnOperationReturn( nOperationID, lpCurrentTask );

         return( zCALL_ERROR );
      }
   }
   else
   {
      if ( nOperationID > iDefineHierarchicalCursor )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         // "KZOEE265 - Invalid Operation for non-Hierarchical processing"
         fnIssueCoreError( lpTask, lpView, 8, 265, 0, 0, 0 );
         if ( bOperationIdxSet )
            fnOperationReturn( nOperationID, lpCurrentTask );

         return( zCALL_ERROR );
      }
   }

   // Set View Entity and View Entity Cursor for caller
   if ( ppvReturnViewEntity )
   {
      *ppvReturnViewEntity            = lpViewEntity;
      *ppvReturnViewEntityCsr         = lpViewEntityCsr;
   }

   if ( plpReturnScopingViewEntity )
   {
      *plpReturnScopingViewEntity     = lpScopingViewEntity;
      *plpReturnScopingViewEntityCsr  = lpScopingViewEntityCsr;
   }

   // Establish any cursors marked as needing to be established
   // up the parent chain.
   lpEntityInstance = 0;
   if ( lpViewEntityCsr )
   {
      lpWorkViewEntityCsr = lpViewEntityCsr;
      for ( ; ; )
      {
         if ( lpWorkViewEntityCsr->hEntityInstance == UNSET_CSR )
         {
            fnEstablishCursorForView( lpWorkViewEntityCsr );
            break;
         }

         if ( lpWorkViewEntityCsr == lpScopingViewEntityCsr )
         {
            break;
         }

         lpWorkViewEntityCsr = zGETPTR( lpWorkViewEntityCsr->hParent );

         if ( lpWorkViewEntityCsr == 0 )
            break;
      }

      // Get Entity Instance pointer for scoping entity instance
      if ( lpScopingViewEntityCsr ) // set cursor for root of view
         lpScopingEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
      else
         lpScopingEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );

      if ( lpScopingEntityInstance == UNSET_CSR )
         lpScopingEntityInstance = 0;

      if ( lpScopingEntityInstance == 0 )
      {
         // If the scoping cursor looks NULL, then we have an either a NULL
         // cursor error or an undefined cursor error. Look up the parent
         // entity instance chain to see if the parent entity instance to
         // the NULL cursor has been deleted. If so then issue the undefined
         // error because issuing a NULL error in this case would be
         // confusing!!!

         // See if we can find a parent entity instance to the cursor by
         // giong up the scoping entity cursor chain
         while ( lpScopingViewEntityCsr &&
                 lpScopingViewEntityCsr->hEntityInstance == 0 )
         {
            lpScopingViewEntityCsr = zGETPTR( lpScopingViewEntityCsr->hParent );
         }

         // if no cursor was found for the NULL scoping entity instances
         // parent entity type, then use the ViewParentEntityInstance as
         // the scoping entity instances parent.
         if ( lpScopingViewEntityCsr )
         {
            lpScopingEntityInstance = zGETPTR( lpScopingViewEntityCsr->hEntityInstance );
         }
         else
            lpScopingEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );
      }

      if ( lpScopingEntityInstance == UNSET_CSR )
         lpScopingEntityInstance = 0;

      // Validate that the scoping entity instance has not been deleted. Note
      // that lpScopingEntityInstance may in fact be the a NULL
      // Scoping entity instances parent as a result of the above check.
      if ( lpScopingEntityInstance && lpScopingEntityInstance->u.nInd.bHidden )
      {
         LPENTITYINSTANCE  lpParent;

         // A hidden cursor has been found, the cursor is undefined ONLY if
         // one of its twins is NOT hidden at the highest hidden parent
         // level.
         while ( lpScopingEntityInstance->hParent )
         {
            lpParent = zGETPTR( lpScopingEntityInstance->hParent );
            if ( lpParent->u.nInd.bHidden == FALSE )
               break;

            lpScopingEntityInstance = lpParent;
         }

         while ( lpScopingEntityInstance->hPrevTwin )
            lpScopingEntityInstance =
                              zGETPTR( lpScopingEntityInstance->hPrevTwin );

         while ( lpScopingEntityInstance )
         {
            if ( lpScopingEntityInstance->u.nInd.bHidden == FALSE )
               break;

            lpScopingEntityInstance =
                              zGETPTR( lpScopingEntityInstance->hNextTwin );
         }

         if ( lpScopingEntityInstance )
         {
            if ( bOperationIdxSet )
               fnOperationReturn( nOperationID, lpCurrentTask );

            return( zCURSOR_UNDEFINED );
         }
      }

      // Get Entity Instance pointer
      if ( lpScopingViewEntityCsr && lpScopingEntityInstance == 0 )
         lpEntityInstance = 0;
      else
         lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

      // Now make sure a cursor exists for the view entity
      if ( lpEntityInstance == 0 &&
           nOperationID != iSetCursorFirstEntity           &&
           nOperationID != iSetCursorFirstEntityByAttr     &&
           nOperationID != iSetCursorFirstEntityByString   &&
           nOperationID != iSetCursorFirstEntityByInteger  &&
           nOperationID != iSetCursorFirstEntityByDecimal  &&
           nOperationID != iSetCursorFirstEntityByEntityCs &&
           nOperationID != iSetCursorFirstSelectedEntity   &&
           nOperationID != iSetAllSelectStatesForEntity    &&
           nOperationID != iSetEntityCursor                &&
           nOperationID != iSetCursorLastEntity            &&
           nOperationID != iGetRelativeEntityNumber        &&
           nOperationID != iGetSelectStateOfEntity )
      {
         LPTASK lpTask = zGETPTR( lpView->hTask );
         //  "KZOEE253 - Entity cursor is NULL"
         fnIssueCoreError( lpTask, lpView, 8, 253, 0,
                           lpViewEntity->szName, 0 );
         if ( bOperationIdxSet )
            fnOperationReturn( nOperationID, lpCurrentTask );

         return( zCALL_ERROR );
      }
   }

   // Finally, make sure that the target entity instance
   // version indicator matches the scoping entity instance
   // version indicator. If not, then the target entity instance
   // is versioned and the scoping entity is not versioned. The target
   // entity instance must be set to the current version prior to
   // any normal cursor navigation can continue. If the indicators match,
   // then navigation can continue regardless of whether the target
   // and scoping entities are on a current or previous version.
   if ( lpEntityInstance && lpScopingEntityInstance &&
        lpEntityInstance->hNextVsn &&
        lpScopingEntityInstance->hNextVsn == 0 )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      LPVIEWENTITY lpTempViewEntity1 =
                           zGETPTR( lpScopingViewEntityCsr->hViewEntity );
      LPVIEWENTITY lpTempViewEntity2 =
                           zGETPTR( lpViewEntityCsr->hViewEntity );

      // NOTE: This check is NOT BULLETPROOF, It needs some work to
      // Accurately determine a versioning mismatch!!!

      //  "KZOEE257 - Version mismatch between scoping and target "
      //  "           Entities"
      fnIssueCoreError( lpTask, lpView, 8, 257, 0,
                        lpTempViewEntity1->szName,
                        lpTempViewEntity2->szName );
      if ( bOperationIdxSet )
         fnOperationReturn( nOperationID, lpCurrentTask );

      return( zCALL_ERROR );
   }

   // Everything is AOK, return
   return( 0 );
}

void
fnSetChildCursors( LPVIEWENTITYCSR lpViewEntityCsr )
{
   LPVIEWENTITYCSR  lpResetViewEntityCsr;
   LPENTITYINSTANCE lpEntityInstance;
   zSHORT           nRootLevel;

   // We need to keep track of the level of the parent EI.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   nRootLevel = lpEntityInstance->nLevel;

   for ( lpResetViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
         lpResetViewEntityCsr &&
            lpResetViewEntityCsr->nLevel > lpViewEntityCsr->nLevel;
         lpResetViewEntityCsr = zGETPTR( lpResetViewEntityCsr->hNextHier ) )
   {
      // We only care about cursors that are immediate children of the parent.
      if ( lpResetViewEntityCsr->nLevel != lpViewEntityCsr->nLevel + 1 )
         continue;

      // Find the first lpEntityInstance that has the same hViewEntity as
      // the lpResetViewEntityCsr.
      for ( lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
            lpEntityInstance && lpEntityInstance->nLevel > nRootLevel;
            lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier ) )
      {
         if ( lpEntityInstance->hViewEntity == lpResetViewEntityCsr->hViewEntity )
         {
            // We found a match.  Set the cursor.
            lpResetViewEntityCsr->hEntityInstance = zGETHNDL( lpEntityInstance );
         // if ( lpResetViewEntityCsr->hEntityInstance == UNSET_CSR )
         //    SysMessageBox( 0, "fnSetChildCursors", "UNSET_CSR", -1 );

            // Call this function again to set the children of lpResetViewEntityCsr.
            fnSetChildCursors( lpResetViewEntityCsr );

            // We're done with this cursor so break.
            break;
         }

      } // for each lpEntityInstance....

   } // for each csr...
}

//./ ADD NAME=fnResetCursorForViewChildren
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnResetCursorForViewChildren
//
//  PURPOSE:    To reset the entity cursor for children view entities
//              of the passed entity to the first twin.
//
//  RETURNS:    void
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
void
fnResetCursorForViewChildren( LPVIEWENTITYCSR lpViewEntityCsr )
{
   LPVIEWENTITYCSR  lpResetViewEntityCsr;

   // First set all the cursors to UNSET.
   for ( lpResetViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );
         lpResetViewEntityCsr && lpResetViewEntityCsr->nLevel > lpViewEntityCsr->nLevel;
         lpResetViewEntityCsr = zGETPTR( lpResetViewEntityCsr->hNextHier ) )
   {
      lpResetViewEntityCsr->hEntityInstance = UNSET_CSR;
   // if ( lpResetViewEntityCsr->hEntityInstance == UNSET_CSR )  // happens way too much
   //    SysMessageBox( 0, "fnResetCursorForViewChildren", "UNSET_CSR", -1 );
   }

   // If the root entity cursor is unset then we can't set any
   // child cursors to point to EI's because there are no child EI's.
   if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR ||
        lpViewEntityCsr->hEntityInstance == 0 )
   {
      return;
   }

   // We'll have to use a recursive call to set the child entities.  Each call
   // sets the cursors for the immediate children.
   fnSetChildCursors( lpViewEntityCsr );
}

//./ ADD NAME=fnNullCursorForViewChildren
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnNullCursorForViewChildren
//
//  PURPOSE:    To NULL the entity cursor for children view entities
//              of the passed entity.
//
//  RETURNS:    void
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
void
fnNullCursorForViewChildren( LPVIEWENTITYCSR lpViewEntityCsr )
{
   LPVIEWENTITYCSR  lpResetViewEntityCsr;

   lpResetViewEntityCsr = zGETPTR( lpViewEntityCsr->hNextHier );

   while ( lpResetViewEntityCsr &&
           lpResetViewEntityCsr->nLevel > lpViewEntityCsr->nLevel )
   {
      lpResetViewEntityCsr->hEntityInstance = 0;
      lpResetViewEntityCsr = zGETPTR( lpResetViewEntityCsr->hNextHier );
   }
}

//./ ADD NAME=fnEstablishCursorForView
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnEstablishCursorForView
//
//  PURPOSE:    To determine the cursor position for an Entity when
//              the cursor value has been reset to 1.
//
//  RETURNS:    LPENTITYINSTANCE - The entity instance pointer
//              0 - Cursor is NULL
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPENTITYINSTANCE
fnEstablishCursorForView( LPVIEWENTITYCSR lpViewEntityCsr )
{
   LPVIEWCSR        lpViewCsr;
   LPVIEWENTITY     lpSearchViewEntity;
   LPVIEWENTITYCSR  lpSetViewEntityCsr;
   LPVIEWENTITYCSR  lpScanViewEntityCsr;
   LPVIEWSUBOBJECT  lpViewSubobject;
   LPENTITYINSTANCE lpSetEntityInstance;
   LPENTITYINSTANCE lpSearchEntityInstance;
   LPENTITYINSTANCE lpScanEntityInstance;
   int              nSetLevel;
   int              nSearchLevel;
   int              bFound;

   lpViewCsr = zGETPTR( lpViewEntityCsr->hViewCsr );
   lpSetViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );
   if ( lpSetViewEntityCsr &&
        lpSetViewEntityCsr->hEntityInstance == UNSET_CSR )
   {
      fnEstablishCursorForView( lpSetViewEntityCsr );
      if ( lpSetViewEntityCsr->hEntityInstance == NULL_CSR )
      {
         lpViewEntityCsr->hEntityInstance = NULL_CSR;
         return( NULL_CSR );
      }
   }

   // If we do not find a set cursor in the parent chain, then the
   // view MUST be on a subobject and the parent cursor to the view
   // is used as the set entity instance for establishing the cursor.
   if ( lpSetViewEntityCsr )
   {
      lpSetEntityInstance = zGETPTR( lpSetViewEntityCsr->hEntityInstance );
      nSetLevel           = lpSetViewEntityCsr->nLevel;
   }
   else
   {
      lpSetEntityInstance = zGETPTR( lpViewCsr->hViewParentEntityInstance );
      nSetLevel           = lpSetEntityInstance->nLevel;
   }

   nSearchLevel    = lpViewEntityCsr->nLevel;
   lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );

   // Determine the view entity we need to look for, 99% of the time
   // it will be the view entity for the view cursor, otherwise it may
   // be the recursive entity for a folded up cursor when the current
   // view entity has no parent and is the root
   if ( lpViewEntityCsr->hParent || lpViewSubobject == 0 ||
        lpViewSubobject->hRecursViewEntity == 0 ||
        lpViewEntityCsr != lpViewSubobject->hRootCsr )
   {
      lpSearchViewEntity = zGETPTR( lpViewEntityCsr->hViewEntity );
   }
   else
      lpSearchViewEntity = zGETPTR( lpViewSubobject->hRecursViewEntity );

   if ( lpSetEntityInstance )
      lpSearchEntityInstance = zGETPTR( lpSetEntityInstance->hNextHier );
   else
      lpSearchEntityInstance = 0;

   bFound = FALSE;
   while ( lpSearchEntityInstance &&
           lpSearchEntityInstance->nLevel > nSetLevel )
   {
      if ( lpSearchEntityInstance->nLevel == nSearchLevel &&
           zGETPTR( lpSearchEntityInstance->hViewEntity ) == lpSearchViewEntity )
      {
         bFound = TRUE;
         break;
      }

      lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextHier );
   }

   // We're out of the loop, if found, set all cursors up to the set parent.
   if ( bFound )
   {
      // If the found instance is hidden and the parent is not hidden go to
      // the first twin which is not hidden.
      if ( lpSearchEntityInstance->u.nInd.bHidden &&
           lpSetEntityInstance->u.nInd.bHidden == FALSE )
      {
         while ( lpSearchEntityInstance->u.nInd.bHidden &&
                 lpSearchEntityInstance->hNextTwin )
         {
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hNextTwin );
         }

         // If a non-hidden twin was not found, return to the first twin.
         while ( lpSearchEntityInstance->u.nInd.bHidden &&
                 lpSearchEntityInstance->hPrevTwin )
         {
            lpSearchEntityInstance = zGETPTR( lpSearchEntityInstance->hPrevTwin );
         }
      }

      lpViewEntityCsr->hEntityInstance = zGETHNDL( lpSearchEntityInstance );
   // if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
   //    SysMessageBox( 0, "fnEstablishCursorForView", "UNSET_CSR", -1 );

      lpScanViewEntityCsr  = zGETPTR( lpViewEntityCsr->hParent );
      lpScanEntityInstance = zGETPTR( lpSearchEntityInstance->hParent );
      while( lpScanViewEntityCsr != lpSetViewEntityCsr )
      {
         lpScanViewEntityCsr->hEntityInstance = zGETHNDL( lpScanEntityInstance );
      // if ( lpScanViewEntityCsr->hEntityInstance == UNSET_CSR )
      //    SysMessageBox( 0, "fnEstablishCursorForView2", "UNSET_CSR", -1 );

         lpScanViewEntityCsr  = zGETPTR( lpScanViewEntityCsr->hParent );
         lpScanEntityInstance = zGETPTR( lpScanEntityInstance->hParent );
      }
   }
   else
   {
      // If no cursor found, set cursor to null and recursively call ourself
      // to set our parent.
      lpSearchEntityInstance = 0;
      lpViewEntityCsr->hEntityInstance = 0;
      lpScanViewEntityCsr = zGETPTR( lpViewEntityCsr->hParent );
      if ( lpScanViewEntityCsr != lpSetViewEntityCsr )
      {
         zVIEW lpView = zGETPTR( lpViewCsr->hView );
         SysMessageBox( lpView, "Internal Error",
                        "(cm) ERROR Unexpected Recursive "
                        "call to fnEstablishCursorForView", 1 );
         fnEstablishCursorForView( lpScanViewEntityCsr );
      }
   }

   return( lpSearchEntityInstance );  // return instance set
}

//./ ADD NAME=AddCursorListener
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      AddCursorListener
//
//  PURPOSE:    Set Cursor Listener for view. the Cursor Listener is
//              a call back function to be called whenever the cursor
//              is set.
//  RETURNS:    0 = CursorListener was successfully set
//              -1 = error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
AddCursorListener( zVIEW          lpView,
                   CURSORLISTENER lpfnCursorListener,
                   zPVOID         pvInstance )
{
   LPTASK           lpCurrentTask = zGETPTR( lpView->hTask );
   zLONG            lListenerProcess = SysGetProcessID( 0 );
   LPCURSORLISTENER lpCsrListener;
   LPCURSORLISTENER hCsrListener;

// TraceLine( "AddCursorListener for view: 0x%08x", lpView );

   // Check to see if it's already in the list.
   for ( lpCsrListener = zGETPTR( lpView->hFirstCursorListener );
         lpCsrListener;
         lpCsrListener = zGETPTR( lpCsrListener->hNextCursorListener ) )
   {
      if ( lpCsrListener->lpfnCursorListener == lpfnCursorListener &&
           lpCsrListener->pvListenerInstance == pvInstance &&
           lpCsrListener->lListenerProcess == lListenerProcess )
      {
         // The listener is already registered - just return.
         return( 0 );
      }
   }

   hCsrListener = fnAllocDataspace( lpCurrentTask->hFirstDataHeader,
                                    sizeof( CursorListenerRecord ),
                                    1, 0, 0 );
   lpCsrListener = zGETPTR( hCsrListener );
   lpCsrListener->hNextCursorListener = lpView->hFirstCursorListener;
   lpView->hFirstCursorListener = hCsrListener;

   lpCsrListener->lpfnCursorListener = lpfnCursorListener;
   lpCsrListener->pvListenerInstance = pvInstance;
   lpCsrListener->lListenerProcess   = lListenerProcess;
   lpCsrListener->hListenerTask      = lpView->hTask;

   return( 0 );
}

//./ ADD NAME=RemoveCursorListener
// Source Module=kzoecmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      RemoveCursorListener
//
//  PURPOSE:    Remove Cursor Listener for view. The Cursor Listener was
//              previously set by AddCursorListener.

//  RETURNS:    0 = CursorListener was successfully removed.
//              -1 = error
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
RemoveCursorListener( zVIEW          lpView,
                      CURSORLISTENER lpfnCursorListener,
                      zPVOID         pvInstance )
{
   zLONG            lListenerProcess = SysGetProcessID( 0 );
   LPCURSORLISTENER lpCsrListener;
   LPCURSORLISTENER lpPrevCsrListener = 0;

// TraceLine( "RemoveCursorListener for view: 0x%08x", lpView );

   // Look for the cursor listener.
   for ( lpCsrListener = zGETPTR( lpView->hFirstCursorListener );
         lpCsrListener;
         lpCsrListener = zGETPTR( lpCsrListener->hNextCursorListener ) )
   {
      if ( lpCsrListener->lpfnCursorListener == lpfnCursorListener &&
           lpCsrListener->pvListenerInstance == pvInstance &&
           lpCsrListener->lListenerProcess == lListenerProcess )
      {
         // If prev is 0 then we found the very first one.
         if ( lpPrevCsrListener == 0 )
            lpView->hFirstCursorListener = lpCsrListener->hNextCursorListener;
         else
            lpPrevCsrListener->hNextCursorListener = lpCsrListener->hNextCursorListener;

         fnFreeDataspace( lpCsrListener );
         return( 0 );
      }

      lpPrevCsrListener = lpCsrListener;
   }

   return( -1 ); // not found
}

// Call any Cursor Listener which has registered.
zVOID
fnCallListeners( zVIEW lpView, zCPCHAR cpcEntityName )
{
   LPCURSORLISTENER lpCsrListener;
   LPCURSORLISTENER hCsrListener;
   zLONG            lListenerProcess = SysGetProcessID( 0 );

   lpCsrListener = zGETPTR( lpView->hFirstCursorListener );
   while ( lpCsrListener )
   {
      // We cannot issue a callback to another task.
   // if ( lpCsrListener->hListenerTask == hCurrentTask &&
      if ( lpCsrListener->lListenerProcess == lListenerProcess )
      {
         // call back
         (*lpCsrListener->lpfnCursorListener)
                  ( lpView, cpcEntityName, lpCsrListener->pvListenerInstance );
      }

      // There appears to be a bug in Microsoft's compiler.  If the following
      // are done in one line the resulting code crashes.
      hCsrListener = lpCsrListener->hNextCursorListener;
      lpCsrListener = zGETPTR( hCsrListener );
   }
}
