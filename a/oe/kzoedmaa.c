#include "kzoephdr.h"
//
// MODULE NAME:  KZOEDMAA  -  Object services Domain Routines.
// DESCRIPTION:  This is the source file which contains Object services
//               operations for accessing Domains and their subobjects.
// OPERATIONS:
// **********************************************************************
// * Copyright (c) 1993-2016 QuinSoft, Inc. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  John Price
//  DATE:    9/22/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2002.09.10  HH
   Fix R56925, assign integer to table domain based on string.

2000.12.27  HH
   Fix bug comparing table domain value against NULL string.
   NULL attribute always compares against NULL string.
   For compatibility, NULL attribute compares against 0, too.

2000.08.03  DGC  10a
   Fixed bug reported by Phil.  Table_Handler was not correctly checking
   the max string length when copying table values.

2000.01.19  RG   Zeidon2000
   Fix in functions: fnCallDomainOper and fnCallDomainOper2
   return zDME_NOT_HANDLED when pfDomainOper == NULL

1999.05.12  DGC  10a
   Fixed bug in fnGetContext( ) reported by LW.

1999.04.06  DGC  10a
   Changed domain interface some to handle set-attribute-from-attribute.

1999.02.26  DGC
   Made change to get rid of MSVC 6.0 compiler warning.
*/

#include <stdlib.h>

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

//./ ADD NAME=fnCallDomainOper
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnCallDomainOper
//
//  PURPOSE:
//
//  RETURNS: non 0 - pointer to Context object
//           0     - context not found
//
/////////////////////////////////////////////////////////////////////////////
zSHORT LOCALOPER
fnCallDomainOper( LPTASK       lpTask,
                  LPTASKDOMAIN lpTaskDomain,
                  zLONG        lEntryType,
                  zCHAR        chType,
                  zPVOID       lpData,
                  zCPCHAR      cpcContextName,
                  zVIEW        lpView,
                  LPVIEWENTITY lpViewEntity,
                  LPVIEWATTRIB lpViewAttribute,
                  zULONG       ulMaxStringLth )
{
   zDomainOperData DomainData;

   zmemset( &DomainData, 0, sizeof( DomainData ) );

   DomainData.lID              = 0;
   DomainData.cType            = chType;
   DomainData.lpData           = lpData;
   DomainData.pszContextName   = (zPCHAR) cpcContextName;
   DomainData.zView            = lpView;
   DomainData.lpViewEntity     = lpViewEntity;
   DomainData.lpViewAttribute  = lpViewAttribute;
   DomainData.uMaxStringLength = ulMaxStringLth;

   // pfDomainOper is only set for DomainTypes:
   // zDM_TYPE_EXPRESSION
   // zDM_TYPE_ALGORITHM
   // zDM_TYPE_TABLE
   // and the domain type needs a Library name.
   // Otherwise the Function pointer is NULL.
   // 2000.01.19, RG
   if ( *lpTaskDomain->pfDomainOper == 0 )
      return( zDME_NOT_HANDLED );

   return( (zSHORT) (*lpTaskDomain->pfDomainOper)( lEntryType, &DomainData ) );
}

//./ ADD NAME=fnCallDomainOper2
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnCallDomainOper2
//
//  PURPOSE: To call the domain operations that have a source and target
//           attributes.
//
//  RETURNS: non 0 - pointer to Context object
//           0     - context not found
//
/////////////////////////////////////////////////////////////////////////////
zSHORT LOCALOPER
fnCallDomainOper2( LPTASK       lpTask,
                   LPTASKDOMAIN lpTaskDomain,
                   zLONG        lEntryType,
                   zVIEW        lpView,
                   LPVIEWENTITY lpViewEntity,
                   LPVIEWATTRIB lpViewAttribute,
                   zVIEW        lpSrcView,
                   LPVIEWENTITY lpSrcViewEntity,
                   LPVIEWATTRIB lpSrcViewAttribute )
{
   zDomainOperData DomainData;

   zmemset( &DomainData, 0, sizeof( DomainData ) );

   DomainData.zView              = lpView;
   DomainData.lpViewEntity       = lpViewEntity;
   DomainData.lpViewAttribute    = lpViewAttribute;
   DomainData.zSrcView           = lpSrcView;
   DomainData.lpSrcViewEntity    = lpSrcViewEntity;
   DomainData.lpSrcViewAttribute = lpSrcViewAttribute;

   // pfDomainOper is only set for DomainTypes:
   // zDM_TYPE_EXPRESSION
   // zDM_TYPE_ALGORITHM
   // zDM_TYPE_TABLE
   // and the domaintype need a Libraryname.
   // Otherwise the Functionpointer is NULL
   // 2000.01.19, RG
   if ( *lpTaskDomain->pfDomainOper == 0 )
      return( zDME_NOT_HANDLED );

   return( (zSHORT) (*lpTaskDomain->pfDomainOper)( lEntryType, &DomainData ) );
}

//./ ADD NAME=fnGetContext
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnGetContext
//
//  PURPOSE: Find the requested Context Subobject with in the Domain.
//
//  RETURNS: non 0 - pointer to Context object
//           0     - context not found
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLPCONTEXT
fnGetContext( LPDOMAIN lpDomain, zCPCHAR cpcContextName )
{
   zLPCONTEXT  lpSrchContext;

   lpSrchContext = zGETPTR( lpDomain->hFirstContext );

   // Check for presence of ZKey selection
   if ( *cpcContextName == '\xff' )
   {
      LPZKEY lpZKey = (LPZKEY) cpcContextName;

      while ( lpSrchContext )
      {
         if ( lpSrchContext->lContextTok == lpZKey->lZKey )
            break;

         lpSrchContext = zGETPTR( lpSrchContext->hNextContext );
      }
   }
   else
   {
      while ( lpSrchContext )
      {
         zPCHAR lpName = lpSrchContext->szName;

         if ( lpName == 0 || lpName[ 0 ] == 0 )
         {
            // "KZOEE356 - Domain has an unnamed Context"
            fnSysMessageBox( 0, "Zeidon Error",
                             "Domain has an unnamed Context", 0 );
         // fnIssueCoreError( 0, lpView, 16, 356, 0, lpDomain->szName, 0 );
         }
         else
         if ( zstrcmpi( lpName, cpcContextName ) == 0 )
               break;

         lpSrchContext = zGETPTR( lpSrchContext->hNextContext );
      }
   }

   return( lpSrchContext );
}

//./ ADD NAME=GetContext
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetContext
//
//  PURPOSE:    To return pointer to a context struct for a given domain.
//              For use in domains and dbhandlers.
//
//  PARAMETERS: lpReturnContext - return pointer of the context struct
//              lpDomain        - domain struct
//              cpcContextName  - Context Name
//
//  RETURNS:    0 - Context not found
//              1 - Pointer to the context struct has been returned
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
GetContext( zLPCONTEXT *lpReturnContext,
            LPDOMAIN lpDomain,
            zCPCHAR cpcContextName )
{
   // if no domain or no context name, return 0.
   if ( lpDomain == 0 || cpcContextName == 0 || *cpcContextName == 0 )
   {
      *lpReturnContext = (zLPCONTEXT) 0;
      return( 0 );
   }

   *lpReturnContext = fnGetContext( lpDomain, cpcContextName );

   return( (*lpReturnContext) ? 1 : 0 );
}

//./ ADD NAME=GetDefaultContext
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetDefaultContext
//
//  PURPOSE:    To return pointer to the default context struct for a
//              given domain.
//              For use in domains and dbhandlers.
//
//  PARAMETERS: lpReturnContext - return pointer of the context struct
//              lpDomain        - domain struct
//
//  RETURNS:    0 - Context not found
//              1 - Pointer to the context struct has been returned
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
GetDefaultContext( zLPCONTEXT *lpReturnContext,
                   LPDOMAIN lpDomain )
{
   ZKeyRecord stZKeyTok;

   // If no domain, return 0.
   if ( lpDomain == 0 )
   {
      *lpReturnContext = (zLPCONTEXT) 0;
      return( 0 );
   }

   if ( lpDomain->lDefaultContextZKey )
   {
      stZKeyTok.ch1 = '\xff';
      stZKeyTok.ch2 = '\x00';
      stZKeyTok.lZKey = lpDomain->lDefaultContextZKey;
      *lpReturnContext = fnGetContext( lpDomain, (zPCHAR) &stZKeyTok );
   }
   else
      *lpReturnContext = fnGetContext( lpDomain, lpDomain->szName );

   return( (*lpReturnContext) ? 1 : 0 );
}

//./ ADD NAME=SfGetDefaultContextForDomain
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetDefaultContextForDomain
//
//  PURPOSE:    To retrieve the default context pointer associated with a
//              given domain.
//
//  PARAMETERS: lpApp - a void far * to return the pointer associated
//                      with the subtask.
//
//              lpView - A view which identifies the subtask pointing
//                       to the application from which the application
//                       pointer is to be retrieved. // changed to ANY view
//
//  RETURNS:    0           - Pointer returned in lpApp
//              zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
SfGetDefaultContextForDomain( zLPCONTEXT *lpReturnContext,
                              zVIEW      lpView,
                              zCPCHAR    cpcDomainName )
{
   LPTASK   lpCurrentTask;
   LPAPP    lpApp;
   LPDOMAIN lpDomain;
   zSHORT   nRC = 0;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfGetDefaultContextForDomain,
                                          lpView, zVALID_VIEW )) == 0 )
   {
      *lpReturnContext = 0;
      return( zCALL_ERROR );
   }

   SfGetApplicationForSubtask( &lpApp, lpView );
   lpDomain = zGETPTR( lpApp->hFirstDomain );
   while ( lpDomain )
   {
      if ( zstrcmp( lpDomain->szName, cpcDomainName ) == 0 )
      {
         nRC = GetDefaultContext( lpReturnContext, lpDomain );
      // TraceLine( "SfGetDefaultContextForDomain Name: %s   Context: %s",
      //            cpcDomainName, (*lpReturnContext)->szName );
         break;
      }

      lpDomain = zGETPTR( lpDomain->hNextDomain );
   }

   fnOperationReturn( iSfGetDefaultContextForDomain, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnDomainProcessingRequired
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   fnDomainProcessingRequired
//
//  PURPOSE: Determine if domain processing on an attribute
//
//  RETURNS: non-zero - DomainType (zDM_TYPE_...)
//                  0 - no domain processing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zCHAR
fnDomainProcessingRequired( LPTASKDOMAIN  *lpReturnTaskDomain,
                            zVIEW         lpView,
                            LPTASK        lpCurrentTask,
                            LPDOMAIN      lpDomain )
{
   LPTASKDOMAIN   lpTDTemp;
   LPDOMAIN       lpTempDomain;

   *lpReturnTaskDomain = fnGetTaskDomain( lpView, lpCurrentTask, lpDomain );
   lpTDTemp = *lpReturnTaskDomain;
   if ( lpTDTemp )
   {
      lpTempDomain = zGETPTR( lpTDTemp->hDomain );

      if ( lpTDTemp->pfDomainOper ||
           lpTempDomain->cDomainType == zDM_TYPE_TABLE ||
           lpTempDomain->cDomainType == zDM_TYPE_FORMAT )
      {
         return( lpTempDomain->cDomainType );
      }
   }

   return( 0 );
}

//./ ADD NAME=TableEntryForInternalValue
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   TableEntryForInternalValue
//
//  PURPOSE: To perform a table lookup on an internal value
//
//  PARAMETERS: lpView          - the view to the object
//              lpDomain        - domain struct
//              cpcContext      - the name of the Context currently being processed
//              lpInputValue    - a pointer to the value passed to this routine,
//                                which can be either string or integer. This
//                                is the value which is to be looked up
//                                against the internal table value.
//                                Note that if the attribute value being
//                                processed is Decimal, then the caller must
//                                have converted the Decimal value to an
//                                Integer value and passed a pointer to the
//                                Integer value to this routine. This is
//                                because a table domain of type decimal
//                                stores the internal value as an integer.
//
//  RETURNS: 0          - Value present
//          -1          - Value not found in table
//          zCALL_ERROR - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
TableEntryForInternalValue( zVIEW      lpView,
                            LPDOMAIN   lpDomain,
                            zCPCHAR    cpcContextName,
                            zCPVOID    lpInputValue )
{
   LPTASK         lpCurrentTask;
   zLPCONTEXT     lpContext;
   LPTABLEENTRY   lpSrchTableEntry;
   zCHAR          chType;
   zSHORT         nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iTableEntryForInternalValue,
                                          lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
      GetDefaultContext( &lpContext, lpDomain );
   else
      lpContext = fnGetContext( lpDomain, cpcContextName );

   if ( lpContext == 0 )
   {
      // "KZOEE350 - Context Not Valid for Domain"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 350, 0, lpDomain->szName,
                        TrueName( cpcContextName, zSHOW_ZKEY ) );
      fnOperationReturn( iTableEntryForInternalValue, lpCurrentTask );
      return( -1 );
   }

   chType = lpDomain->cType;
   lpSrchTableEntry = zGETPTR( lpContext->hFirstTableEntry );

   while ( lpSrchTableEntry )
   {
      if ( chType == zTYPE_INTEGER || chType == zTYPE_DECIMAL )
      {
         if ( lpSrchTableEntry->unInternal.lInteger ==
                                                  *((zPLONG) lpInputValue) )
         {
            break;
         }
      }
      else
      {
         if ( ((lpInputValue == 0 || *((zPCHAR) lpInputValue) == 0) &&
               lpSrchTableEntry->unInternal.hStringValue == 0) ||
              (lpSrchTableEntry->unInternal.hStringValue != 0 &&
               zstrcmpi( zGETPTR( lpSrchTableEntry->unInternal.hStringValue ),
                         (zPCHAR) lpInputValue ) == 0) )
         {
            break;
         }
      }

      lpSrchTableEntry = zGETPTR( lpSrchTableEntry->hNextTableEntry );
   }

   if ( lpSrchTableEntry )
      nRC = 0;
   else
      nRC = -1;

   fnOperationReturn( iTableEntryForInternalValue, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=TableEntryIntToExt
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   TableEntryIntToExt
//
//  PURPOSE: To perform a table lookup on an internal value and return
//           the external value.
//
//  PARAMETERS: pchReturnValue  - the external value returned, which will
//                                always be a string.
//              lpView          - the view to the object
//              lpDomain        - domain struct
//              cpcContext      - the name of the Context currently being processed
//              lpInternalValue - a pointer to the value passed into this
//                                routine which is to be looked up in the
//                                table as an internal value. This value MUST
//                                BE either an integer or a string. Note that
//                                if the data type of the original value
//                                being processed is Decimal, then the caller
//                                must have converted the Decimal value to an
//                                Integer value and passed a pointer to the
//                                Integer value to this routine. This is
//                                because a table domain of type decimal
//                                stores the internal value as an integer.
//
//  RETURNS: 0          - Value returned
//          -1          - External value not found in table
//          zCALL_ERROR - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
TableEntryIntToExt( zPCHAR     pchReturnValue,
                    zVIEW      lpView,
                    LPDOMAIN   lpDomain,
                    zCPCHAR    cpcContext,
                    zCPVOID    lpInternalValue )
{
   LPTASK         lpCurrentTask;
   zLPCONTEXT     lpContext;
   LPTABLEENTRY   lpSrchTableEntry;
   zCHAR          chType;
   zSHORT         nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iTableEntryIntToExt, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( cpcContext == 0 || cpcContext[ 0 ] == 0 )
      GetDefaultContext( &lpContext, lpDomain );
   else
      lpContext = fnGetContext( lpDomain, cpcContext );

   if ( lpContext == 0 )
   {
      // "KZOEE350 - Context Not Valid for Domain"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 350, 0, lpDomain->szName,
                        TrueName( cpcContext, zSHOW_ZKEY ) );
      fnOperationReturn( iTableEntryIntToExt, lpCurrentTask );
      return( -1 );
   }

   chType = lpDomain->cType;
   lpSrchTableEntry = zGETPTR( lpContext->hFirstTableEntry );

   while ( lpSrchTableEntry )
   {
      if ( chType == zTYPE_INTEGER || chType == zTYPE_DECIMAL )
      {
         if ( lpSrchTableEntry->unInternal.lInteger ==
                                                *((zPLONG) lpInternalValue) )
         {
            break;
         }
      }
      else
      {
         if ( ((lpInternalValue == 0 || *((zPCHAR) lpInternalValue) == 0) &&
               lpSrchTableEntry->unInternal.hStringValue == 0) ||
              (lpSrchTableEntry->unInternal.hStringValue != 0 &&
               zstrcmpi( zGETPTR( lpSrchTableEntry->unInternal.hStringValue ),
                         (zPCHAR) lpInternalValue ) == 0) )
         {
            break;
         }
      }

      lpSrchTableEntry = zGETPTR( lpSrchTableEntry->hNextTableEntry );
   }

   if ( lpSrchTableEntry )
   {
      zPCHAR pch;

      pch = fnGetTextForTableEntry( lpSrchTableEntry );
      if ( pch )
         strcpy_s( pchReturnValue, 254, pch );
      else
         pchReturnValue[ 0 ] = 0;

      nRC = 0;
   }
   else
   {
      pchReturnValue[ 0 ] = 0;
      nRC = -1;
   }

   fnOperationReturn( iTableEntryIntToExt, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=TableEntryExtToInt
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   TableEntryExtToInt
//
//  PURPOSE: To perform a table lookup on an external value and return
//           the internal value.
//
//  PARAMETERS: lpReturnValue   - a pointer to the internal value returned,
//                                which is currently a string, integer or
//                                decimal.
//                                Beeing cosistent with TableEntryIntToExt
//                                and TableEntryForInternalValue, which only
//                                handle pointers to strings and integers,
//                                we return for type Decimal a long integer,
//                                too. Internal table values can only be
//                                integer or string. Decimal attributes are
//                                logically integer and so they are handled.
//              lpView          - the view to the object
//              lpDomain        - domain struct
//              cpcContext       - the name of the Context currently being processed
//              cpcExternalValue - the external value passed to this routine,
//                                which is always a string.
//              lpInternalValue - a pointer to the value passed into this
//                                routine which is to be looked up in the
//                                table as an external value.  It must always
//                                be a string.
//
//  RETURNS: 0          - Value returned
//          -1          - External value not found in table
//          zCALL_ERROR - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT OPERATION
TableEntryExtToInt( zPVOID     lpReturnValue,
                    zVIEW      lpView,
                    LPDOMAIN   lpDomain,
                    zCPCHAR    cpcContext,
                    zCPCHAR    cpcExternalValue )
{
   LPTASK         lpCurrentTask;
   zLPCONTEXT     lpContext;
   LPTABLEENTRY   lpSrchTableEntry;
   zCHAR          chType;
   zSHORT         nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iTableEntryExtToInt, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( cpcContext == 0 || cpcContext[ 0 ] == 0 )
      GetDefaultContext( &lpContext, lpDomain );
   else
      lpContext = fnGetContext( lpDomain, cpcContext );

   if ( lpContext == 0 )
   {
      // "KZOEE350 - Context Not Valid for Domain"
      fnIssueCoreError( lpCurrentTask, lpView, 8, 350, 0, lpDomain->szName,
                        TrueName( cpcContext, zSHOW_ZKEY ) );
      fnOperationReturn( iTableEntryExtToInt, lpCurrentTask );
      return( zCALL_ERROR );
   }

   chType = lpDomain->cType;
   lpSrchTableEntry = zGETPTR( lpContext->hFirstTableEntry );

   while ( lpSrchTableEntry )
   {
      zPCHAR pch;

      pch = fnGetTextForTableEntry( lpSrchTableEntry );
      if ( pch )
      {
         if ( cpcExternalValue && cpcExternalValue[ 0 ] &&
              zstrcmpi( pch, cpcExternalValue ) == 0 )
         {
            break;
         }
      }
      else
      {
         if ( cpcExternalValue == 0 || cpcExternalValue[ 0 ] == 0 )
            break;
      }

      lpSrchTableEntry = zGETPTR( lpSrchTableEntry->hNextTableEntry );
   }

   if ( lpSrchTableEntry )
   {
      if ( chType == zTYPE_INTEGER || chType == zTYPE_DECIMAL )
      {
         // Integer or Decimal, internal table value to return is long
         *((zPLONG) lpReturnValue) = lpSrchTableEntry->unInternal.lInteger;
      }
      else
      {
         // internal table value to return is string
         if ( lpSrchTableEntry->unInternal.hStringValue )
         {
            strcpy_s( (zPCHAR) lpReturnValue, 257,
                      zGETPTR( lpSrchTableEntry->unInternal.hStringValue ) );
         }
         else
            *((zPCHAR) lpReturnValue) = 0;
      }

      nRC = 0;
   }
   else
   {
      if ( chType == zTYPE_INTEGER || chType == zTYPE_DECIMAL )
         *((zPLONG) lpReturnValue) = 0;
      else
         *((zPCHAR) lpReturnValue) = 0;

      nRC = -1;
   }

   fnOperationReturn( iTableEntryExtToInt, lpCurrentTask );
   return( nRC );
}

zBOOL
IsDataNull( zPVOID pvData, zCHAR chType )
{
   // If the pvData input is null or points to a null char.
   if ( pvData == 0 ||
        (chType == zTYPE_STRING && *((zPCHAR) pvData) == 0) ||
        (chType == zTYPE_INTEGER && *((zPLONG) pvData) == lNullInteger) ||
        (chType == zTYPE_DECIMAL &&
         SysCompareDecimalToNull( (zPDECIMAL) pvData ) == 0) )
   {
      return( TRUE );
   }

   return( FALSE );
}

zSHORT OPERATION
fnGetTableEntryForDomain( zVIEW lpView, LPVIEWENTITY lpViewEntity,
                          LPVIEWATTRIB lpViewAttribute, LPDOMAIN lpDomain,
                          zCPCHAR cpcContextName,
                          zPVOID pvData, zULONG ulMaxStringLth,
                          zPVOID pvTablePosition,
                          zLONG lFlag )  // 1-required   2-next
{
#if 0
   LPTABLEENTRY lpTempTableEntry;  // used to check TableEntry list integrity
#endif
   LPTABLEENTRY lpTableEntry;
   zLPCONTEXT   lpSrchContext;
   zPCHAR       pch;
   zSHORT       nRC;

   // If the table Domain has an operation, call the Domain Operation, instead
   // of continuing processing here.
   if ( lpDomain->szDomainOper[ 0 ] )
   {
      zCHAR        szWorkString[ 256 ];
      LPTASK       lpCurrentTask = zGETPTR( lpView->hTask );
      LPTASKDOMAIN lpTaskDomain = fnGetTaskDomain( lpView, lpCurrentTask, lpDomain );
      if ( lpTaskDomain->pfDomainOper )
      {
         zLONG lEntryType = (lFlag & 2) ? zDME_GET_NEXT_TBL_ENT_FOR_ATTR :
                                          zDME_GET_FIRST_TBL_ENT_FOR_ATTR;
         nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                 lEntryType,
                                 lpDomain->cType, pvData,
                                 cpcContextName, lpView,
                                 lpViewEntity, lpViewAttribute, 256 );
         if ( nRC == zDME_NOT_HANDLED )
         {
            sprintf_s( szWorkString, zsizeof( szWorkString ), "Operation '%s' does not handle an Entry "
                       "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                       lEntryType, lpDomain->szName );
            MessageSend( lpView, "TZDMD101", "System Error",
                         szWorkString, zMSGQ_DOMAIN_ERROR, 0 );
         }
      }
      else
      {
         strcpy_s( szWorkString, zsizeof( szWorkString ), "Operation, " );
         strcat_s( szWorkString, zsizeof( szWorkString ), lpDomain->szDomainOper );
         strcat_s( szWorkString, zsizeof( szWorkString ), ", not found for Domain: " );
         strcat_s( szWorkString, zsizeof( szWorkString ), lpDomain->szName );
         MessageSend( lpView, "TZDMD101", "System Error",
                      szWorkString, zMSGQ_DOMAIN_ERROR, 0 );
         nRC = -1;
      }

      return( nRC );
   }

   if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
      GetDefaultContext( &lpSrchContext, lpDomain );
   else
      lpSrchContext = fnGetContext( lpDomain, cpcContextName );

#if 0
   // check TableEntry list integrity
   lpTempTableEntry = zGETPTR(lpSrchContext->hFirstTableEntry);
   while (lpTempTableEntry)
   {
      lpTempTableEntry = zGETPTR(lpTempTableEntry->hNextTableEntry);
   }
#endif

   if ( lFlag & 2 )  // next
   {
      zLPCONTEXT   lpContext;

      lpTableEntry = *((LPTABLEENTRY *) pvTablePosition );
      lpTableEntry = zGETPTR( lpTableEntry->hNextTableEntry );
      if ( lpTableEntry == 0 )
         return( -1 );

      // Verify that the pvTablePosition is valid (i.e.  pointing to a
      // TableEntry struct, context names match, pointing to the same domain).
      lpContext = zGETPTR( lpTableEntry->hContext );
      if ( lpTableEntry->nTableID != iTableEntry ||
           lpContext != lpSrchContext ||
           zGETPTR( lpContext->hDomain ) != lpDomain )
      {
         return( zCALL_ERROR );
      }
   }
   else
   {
      if ( lpSrchContext == 0 )
      {
         return( zCALL_ERROR );
      }

      // The context for this domain has been located. Save position
      // of the table entry (for GetNextTableEntryForAttribute)
      // in the pvTablePosition parameter.  Let the 'external'
      // routine return the external value for the first table entry.
      lpTableEntry = zGETPTR( lpSrchContext->hFirstTableEntry );
   }

   // 1/5/93 jap
   // For attributes that are marked as required (may not be null),
   // do not return any table entry whose internal value is null.
   if ( lFlag & 1 )  // required
   {
      while ( lpTableEntry )
      {
         if ( (lpDomain->cType == zTYPE_INTEGER &&
               lpTableEntry->unInternal.lInteger == lNullInteger) ||
              (lpDomain->cType == zTYPE_DECIMAL &&
               lpTableEntry->unInternal.lInteger == lNullInteger) ||
              (lpDomain->cType == zTYPE_STRING &&
               lpTableEntry->unInternal.hStringValue == 0) )
         {
            lpTableEntry = zGETPTR( lpTableEntry->hNextTableEntry );
         }
         else
            break;
      }
   }

   // Return the external value in pvData.
   if ( lpTableEntry )
   {
      pch = fnGetTextForTableEntry( lpTableEntry );
      if ( pch )
         strcpy_s( (zPCHAR) pvData, ulMaxStringLth, pch );
      else
         *((zPCHAR) pvData ) = 0;
   }

   // Return the table position in pvTablePosition.
   zmemcpy( pvTablePosition, (zPVOID) &lpTableEntry, sizeof( zPVOID ) );

   nRC = *((LPTABLEENTRY *) pvTablePosition) ? 0 : -1 ; // -1 is end of table
   return( nRC );
}

//./ ADD NAME=Table_Handler
// Source Module=kzoedmaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      Table_Handler
//
//  PURPOSE:    To serve as a general purpose handler for Domains that
//              are of type Table.
//
//  PARAMETERS: standard Domain Entry parameter list
//
//  RETURNS:
//
//  For zDME_SET_ATTRIBUTE:
//              0 - Attribute value has been set
//
//  For zDME_GET_VARIABLE:
//              0 - Attribute value returned
//             -1 - Attribute value is NULL
//
//  For zDME_COMPARE_ATTRIBUTE:
//             -1 - Attribute value < Variable value
//              0 - Attribute value = Variable value
//              1 - Attribute value > Variable value
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
//  For zDME_SET_ATTRIBUTE_VALUE_PREV:
//             Same as zDME_SET_ATTRIBUTE_VALUE_NEXT.
//
//  For zDME_ADD_TO_ATTRIBUTE:
//    zCALL_ERROR - not supported for tables
//
//  For zDME_GET_COMPARE_VALUE:
//             -1 - lpData invalid, i.e. not found in table ...
//              0 - lpData unchanged, OK to use as is.
//              1 - lpData returned, string value
//              2 - lpData returned, integer value
//
//  For all of the above:
//    zCALL_ERROR - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zSHORT OPERATION
Table_Handler( zLONG        lEntryType,
               zCHAR        chType,
               zPVOID       pvData,
               zCPCHAR      cpcContextName,
               zVIEW        lpView,
               LPVIEWENTITY lpViewEntity,
               LPVIEWATTRIB lpViewAttribute,
               zULONG       ulMaxStringLth,
               zPVOID       pvTablePosition )
{
   zCHAR             szWorkString[ 257 ];
   zCHAR             szWorkString2[ 257 ];
   zLONG             lWork = 0;
   zPVOID            lpInternalValue;
   zPLONG            lplInteger;
   LPDOMAIN          lpDomain;
   LPVIEWENTITY      lpViewEntityTemp;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPTASKDOMAIN      lpTaskDomain;
   LPTASK            lpCurrentTask;
   zDECIMAL          dTempValue;
   zLONG             lTempValue;
   zLONG             lTempValue2;
   zSHORT            nSeverity;
   zSHORT            nRC = zCALL_ERROR;  // default to in error status ...

   if ( lEntryType < 1 || lEntryType > 10 )
   {
      szWorkString[ 0 ] = chType;
      szWorkString[ 1 ] = 0;
      TraceLine( "(tblh) Entry type: %d   chType: %s   Context Name: %s   "
                   "Entity Name: %s   Attribute Name: %s",
                 lEntryType, szWorkString, cpcContextName,
                 lpViewEntity->szName, lpViewAttribute->szName );
   }

   lpDomain = zGETPTR( lpViewAttribute->hDomain );
   lpCurrentTask = zGETPTR( lpView->hTask );

   // If the table Domain has an operation, call the Domain Operation, instead
   // of continuing processing here.
   if ( lpDomain->szDomainOper[ 0 ] )
   {
      lpTaskDomain = fnGetTaskDomain( lpView, lpCurrentTask, lpDomain );
      if ( lpTaskDomain->pfDomainOper )
      {
         nRC = fnCallDomainOper( lpCurrentTask, lpTaskDomain,
                                 lEntryType, chType, pvData,
                                 cpcContextName, lpView,
                                 lpViewEntity, lpViewAttribute,
                                 ulMaxStringLth );
         if ( nRC == zDME_NOT_HANDLED )
         {
            sprintf_s( szWorkString, zsizeof( szWorkString ), "Operation '%s' does not handle an Entry "
                       "Type of %ld for domain '%s'", lpDomain->szDomainOper,
                       lEntryType, lpDomain->szName );
            MessageSend( lpView, "TZDMD101", "System Error",
                         szWorkString, zMSGQ_DOMAIN_ERROR, 0 );
         }
      }
      else
      {
         strcpy_s( szWorkString, zsizeof( szWorkString ), "Operation, " );
         strcat_s( szWorkString, zsizeof( szWorkString ), lpDomain->szDomainOper );
         strcat_s( szWorkString, zsizeof( szWorkString ), ", not found for Domain: " );
         strcat_s( szWorkString, zsizeof( szWorkString ), lpDomain->szName );
         MessageSend( lpView, "TZDMD101", "System Error",
                      szWorkString, zMSGQ_DOMAIN_ERROR, 0 );
         nRC = -1;
      }

      return( nRC );
   }

   // Determine the function request.
   switch ( lEntryType )
   {
      case zDME_VALIDATE_LPDATA:
      case zDME_SET_ATTRIBUTE:
         // If the pvData input is null or points to a null char, skip the
         // conversions, just null the attribute.
         if ( IsDataNull( pvData, chType ) )
         {
            if ( lpViewAttribute->cType == zTYPE_INTEGER )
               lpInternalValue = (zPVOID) &lNullInteger;
            else
            {
               if ( lpViewAttribute->cType == zTYPE_DECIMAL )
               {
                  SysAssignDecimalFromNull( &dTempValue );
                  lpInternalValue = (zPVOID) &dTempValue;
               }
               else
                  lpInternalValue = (zPVOID) szNullS;
            }

            if ( lEntryType == zDME_SET_ATTRIBUTE )
               nRC = StoreValueInRecord( lpView, lpViewEntity,
                                         lpViewAttribute,
                                         lpInternalValue, 0 );
            else
               nRC = 0;

            return( nRC );
         }

         switch ( chType )
         {
            case zTYPE_STRING:
               switch ( lpViewAttribute->cType )
               {
                  case zTYPE_STRING:
                     if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
                     {
                        if ( TableEntryForInternalValue( lpView, lpDomain, 0,
                                                         pvData ) == 0 )
                        {
                           lpInternalValue = pvData;
                           break;
                        }

                        if ( lEntryType == zDME_VALIDATE_LPDATA )
                           nSeverity = -1;
                        else
                           nSeverity = 0;

                        // "TZDME025 - Invalid table value "
                        TraceLineS( "TZDME025 - Invalid table value: ",
                                    (zPCHAR) pvData );
                        SendDomainError( lpView, lpDomain, nSeverity, 25, 0,
                                         lpViewEntity->szName,
                                         lpViewAttribute->szName );

                        return( zCALL_ERROR );
                     }
                     else
                     {
                        // convert from external to internal
                        nRC = TableEntryExtToInt( (zPVOID) szWorkString2, lpView,
                                                  lpDomain, cpcContextName,
                                                  (zPCHAR) pvData );
                        if ( nRC == 0 )
                        {
                           lpInternalValue = (zPVOID) szWorkString2;
                           break;
                        }

                        if ( lEntryType == zDME_VALIDATE_LPDATA )
                           nSeverity = -1;
                        else
                           nSeverity = 0;

                        // "TZDME025 - Invalid table value "
                        TraceLineS( "TZDME025 - Invalid table value: ",
                                    (zPCHAR) pvData );
                        SendDomainError( lpView, lpDomain, nSeverity, 25, 0,
                                         lpViewEntity->szName,
                                         lpViewAttribute->szName );

                        return( zCALL_ERROR ); // TableEntry not found, error
                     }

                  case zTYPE_INTEGER:
                  case zTYPE_DECIMAL:
                     if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
                     {
                        // The value coming in (pvData) is string.  Since the
                        // attribute type is integer or decimal, the table type
                        // is integer. (Internal table types are only stored as
                        // strings and integers.)
                        lWork = zatol( (zPCHAR) pvData );
                        if ( TableEntryForInternalValue( lpView, lpDomain,
                                                         lpDomain->szName,
                                                         (zPVOID) &lWork ) == 0 )
                        {
                           if ( lpViewAttribute->cType == zTYPE_DECIMAL )
                           {
                              SysConvertLongToDecimal( lWork, &dTempValue );
                              lpInternalValue = (zPVOID) &dTempValue;
                           }
                           else
                              lpInternalValue = (zPVOID) &lWork;

                           break;
                        }

                        if ( lEntryType == zDME_VALIDATE_LPDATA )
                           nSeverity = -1;
                        else
                           nSeverity = 0;

                        // "TZDME025 - Invalid table value "
                        SendDomainError( lpView, lpDomain, nSeverity, 25, 0,
                                         lpViewEntity->szName,
                                         lpViewAttribute->szName );
                        return( zCALL_ERROR );
                     }
                     else
                     {
                        // convert from external to internal
                        nRC = TableEntryExtToInt( (zPVOID) &lWork, lpView,
                                                  lpDomain, cpcContextName,
                                                  (zPCHAR) pvData );
                        if ( nRC == 0 )
                        {
                           if ( lpViewAttribute->cType == zTYPE_DECIMAL )
                           {
                              SysConvertLongToDecimal( lWork, &dTempValue );
                              lpInternalValue = (zPVOID) &dTempValue;
                           }
                           else
                              lpInternalValue = (zPVOID) &lWork;

                           break;
                        }

                        if ( lEntryType == zDME_VALIDATE_LPDATA )
                           nSeverity = -1;
                        else
                           nSeverity = 0;

                        // "TZDME025 - Invalid table value "
                        SendDomainError( lpView, lpDomain, nSeverity, 25, 0,
                                         lpViewEntity->szName,
                                         lpViewAttribute->szName );

                        return( zCALL_ERROR ); // TableEntry not found, error
                     }

               default:
                  if ( lEntryType == zDME_VALIDATE_LPDATA )
                     nSeverity = -1;
                  else
                     nSeverity = 0;

                  // "TZDME003 - Attribute type invalid for this Domain "
                  SendDomainError( lpView, lpDomain, nSeverity, 3, 0,
                                   lpViewEntity->szName,
                                   lpViewAttribute->szName );
                  return( zCALL_ERROR );
               }

               break;

            case zTYPE_INTEGER:
            case zTYPE_DECIMAL:
               if ( lpViewAttribute->cType != zTYPE_INTEGER &&
                    lpViewAttribute->cType != zTYPE_DECIMAL &&
                    lpViewAttribute->cType != zTYPE_STRING )
               {
                  // "TZDME003 - Attribute type invalid for this Domain "
                  SendDomainError( lpView, lpDomain, 0, 3, 0,
                                   lpViewEntity->szName,
                                   lpViewAttribute->szName );
                  return( zCALL_ERROR );
               }

               if ( lpViewAttribute->cType == zTYPE_STRING )
               {
                  // Allow assignment of a number to string.
                  // We want the same behaviour,
                  //  as we get assigning a number to a normal string attribute.
                  // As the source is a number, there cannot be any context.

                  zCHAR szAttribValue[ 128 ] = { 0 };

                  if ( chType == zTYPE_INTEGER )
                  {
                     // Convert integer coming in to a string
                     if ( *((zPLONG) pvData) != lNullInteger )
                        zltoa( *((zPLONG) pvData), szAttribValue, zsizeof( szAttribValue ) );
                  }
                  else
                  {
                     // Convert decimal coming in to a string
                     if ( SysCompareDecimalToNull( (zDECIMAL*) pvData ) != 0 )
                        SysConvertDecimalToString( (zDECIMAL*) pvData, szAttribValue, -1 );
                  }

                  if ( TableEntryForInternalValue( lpView, lpDomain, 0,
                                                   szAttribValue ) == 0 )
                  {
                     lpInternalValue = szAttribValue;
                     break;
                  }

                  // "TZDME025 - Invalid table value "
                  SendDomainError( lpView, lpDomain, 0, 25, 0,
                                   lpViewEntity->szName,
                                   lpViewAttribute->szName );

                  return( zCALL_ERROR );
               }

               // Decimal and integer values are treated the same in table lookup.
               if ( lpViewAttribute->cType == zTYPE_INTEGER )
                  lTempValue = *((zPLONG) pvData);
               else
                  SysConvertDecimalToLong( (zPDECIMAL) pvData, &lTempValue );

               // We will always treat the setting of a table attribute value from
               // an integer or decimal as setting it from an internal value,
               // whether or not a Context Name is passed in.
               if ( TableEntryForInternalValue( lpView, lpDomain,
                                                lpDomain->szName,
                                                (zPVOID) &lTempValue ) == 0 )
               {
                  lpInternalValue = pvData;
                  break;
               }
               else
               {
                  // "TZDME025 - Invalid table value "
                  SendDomainError( lpView, lpDomain, 0, 25, 0,
                                   lpViewEntity->szName,
                                   lpViewAttribute->szName );
                  return( zCALL_ERROR );
               }

            default:
               // "TZDME001 - Invalid input data type "
               SendDomainError( lpView, lpDomain, 0, 1, 0,
                                lpViewEntity->szName,
                                lpViewAttribute->szName );
               return( zCALL_ERROR );
         }

         if ( lEntryType == zDME_SET_ATTRIBUTE )
         {
            nRC = StoreValueInRecord( lpView, lpViewEntity,
                                      lpViewAttribute, lpInternalValue, 0 );
         }
         else
            nRC = 0;

         return( nRC );

      case zDME_GET_VARIABLE:
         // We have already validated the ViewEntity prior to invoking the
         // Domain function.  We are doing the call again so that we may
         // easily retrieve the lpViewEntityCsr.
         lpViewEntityTemp = fnValidViewEntity( &lpViewEntityCsr, lpView,
                                               lpViewEntity->szName, 0 );
         if ( chType != zTYPE_STRING )
         {
            switch ( chType )
            {
               case zTYPE_INTEGER:
                  nRC = fnGetIntegerFromAttribute( (zPLONG) pvData, lpView,
                                                   lpViewEntityCsr,
                                                   lpViewAttribute );
                  break;

               case zTYPE_DECIMAL:
                  nRC = fnGetDecimalFromAttribute( (zPDECIMAL) pvData, lpView,
                                                   lpViewEntityCsr,
                                                   lpViewAttribute );
                  break;

               case zTYPE_PIC:
                  // return the picture string for the given context
                  {
                     zLPCONTEXT  lpContext;

                     lpContext = fnGetContext( zGETPTR( lpViewAttribute->hDomain ),
                                               cpcContextName );
                     if ( lpContext )
                     {
                        if ( lpContext->hEditString &&
                             *((zPCHAR) zGETPTR( lpContext->hEditString )) )
                        {
                           strcpy_s( (zPCHAR) pvData, ulMaxStringLth,
                                     zGETPTR( lpContext->hEditString ) );
                        }
                        else
                           *((zPCHAR) pvData) = '\x00';

                        nRC = 0;
                     }
                     else
                     {
                        // "TZDME008 - Could not find context for Domain "
                        SendDomainError( lpView, lpDomain, 0, 8, 0,
                                         lpViewEntity->szName,
                                         lpViewAttribute->szName );
                        nRC = zCALL_ERROR;
                     }
                  }

                  break;

               default:
                  // "TZDME001 - Invalid input data type "
                  SendDomainError( lpView, lpDomain, 0, 1, 0,
                                   lpViewEntity->szName,
                                   lpViewAttribute->szName );

                  return( zCALL_ERROR );
            }

            return( nRC );
         }

         nRC = fnGetAddrForAttribute( (zCOREMEM) &lpInternalValue, lpView,
                                      lpViewEntityCsr, 0, lpViewAttribute );

         switch ( nRC )
         {
            case -1:
               // If the value of the attribute is a 'null' value,
               // set the output to null
               *((zPCHAR) pvData) = 0;
               return( -1 );

            case 0:
               break;

            default:
               // error message issued from fnGetAddrForAttribute...
               return( zCALL_ERROR );
         }

         // Perform a table lookup and conversion to external string value
         if ( lpViewAttribute->cType == zTYPE_DECIMAL )
         {
            SysConvertDecimalToLong( (zPDECIMAL) lpInternalValue, &lTempValue );
            nRC = TableEntryIntToExt( szWorkString2, lpView,
                                      lpDomain,
                                      cpcContextName, (zPVOID) &lTempValue );
         }
         else
         {
            nRC = TableEntryIntToExt( szWorkString2, lpView,
                                      lpDomain,
                                      cpcContextName, lpInternalValue );
         }

         if ( nRC == 0 )
         {
            zULONG nLth = zstrlen( szWorkString2 );

            if ( nLth < ulMaxStringLth )
               strcpy_s( (zPCHAR) pvData, ulMaxStringLth, szWorkString2 );
            else
               strncpy_s( (zPCHAR) pvData, ulMaxStringLth, szWorkString2, ulMaxStringLth - 1 );
         }

         return( nRC );

      case zDME_COMPARE_ATTRIBUTE:
      {
         // If the pvData input is null or points to a null char, skip the
         // conversions, just null the attribute.
         zBOOL bNull = IsDataNull( pvData, chType );

         // We have already validated the ViewEntity prior to invoking the
         // Domain function.  We are doing the call again so that we may
         // easily retrieve the lpViewEntityCsr.
         lpViewEntityTemp = fnValidViewEntity( &lpViewEntityCsr, lpView,
                                               lpViewEntity->szName, 0 );
         switch ( chType )
         {
            case zTYPE_STRING:
               switch ( lpViewAttribute->cType )
               {
                  case zTYPE_STRING:
                     // String to string with no context, just compare the
                     // attribute to the variable.
                     if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
                     {
                        zPCHAR  pchAttribute;

                        if ( bNull )
                           pvData = (zPVOID) "";

                        fnGetAddrForAttribute( (zCOREMEM) &pchAttribute,
                                               lpView, lpViewEntityCsr, 0,
                                               lpViewAttribute );
                        if ( lpViewAttribute->bCaseSens )
                           nRC = zstrcmp( pchAttribute, (zPCHAR) pvData );
                        else
                           nRC = zstrcmpi( pchAttribute, (zPCHAR) pvData );

                        if ( nRC < 0 )
                           nRC = -1;
                        else
                        if ( nRC > 0 )
                           nRC = 1;

                        return( nRC );
                     }
                     else
                     {
                        // String to string with context, convert input
                        // pvData from external to internal, not found
                        // returns -1, else compare the attribute to the
                        // returned internal value.
                        nRC = TableEntryExtToInt( (zPVOID) szWorkString,
                                                  lpView, lpDomain,
                                                  cpcContextName,
                                                  (zPCHAR) pvData );
                        if ( nRC < 0 && bNull )
                        {
                           szWorkString[ 0 ] = 0; // null is always acceptable
                           nRC = 0;
                        }

                        if ( nRC == 0 )
                        {
                           zPCHAR   pchAttribute;

                           fnGetAddrForAttribute( (zCOREMEM) &pchAttribute,
                                                  lpView, lpViewEntityCsr, 0,
                                                  lpViewAttribute );
                           if ( lpViewAttribute->bCaseSens )
                              nRC = zstrcmp( pchAttribute, szWorkString );
                           else
                              nRC = zstrcmpi( pchAttribute, szWorkString );

                           if ( nRC < 0 )
                              nRC = -1;
                           else
                           if ( nRC > 0 )
                              nRC = 1;

                           return( nRC );
                        }
                        else
                        {
                           // "TZDME025 - Invalid table value "
                           SendDomainError( lpView, lpDomain, 0, 25, 0,
                                            lpViewEntity->szName,
                                            lpViewAttribute->szName );
                           return( zCALL_ERROR );
                        }
                     }

                  case zTYPE_INTEGER:
                  case zTYPE_DECIMAL:
                     // If there is no ContextName passed, use the Domain
                     // name as ContextName.  In any event, use
                     // TableEntryExtToInt to determine if the external value
                     // passed is found under the correct Context.
                     //
                     // Note that in this case, we are NOT differentiating
                     // between < or >, but only between not equal and equal.

                     {
                        zPCHAR  pchContextName;

                        if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
                           pchContextName = lpDomain->szName;
                        else
                           pchContextName = (zPCHAR) cpcContextName;

                        if ( lpViewAttribute->cType == zTYPE_DECIMAL )
                        {
                           nRC = TableEntryExtToInt( (zPVOID) &dTempValue, lpView, lpDomain,
                                                     pchContextName, (zPCHAR) pvData );
                           if ( nRC < 0 && bNull )
                              lWork = lNullInteger; // null is always acceptable
                           else
                           if ( *((zPLONG) &dTempValue) == lNullInteger )
                              lWork = lNullInteger;
                           else
                              SysConvertDecimalToLong( &dTempValue, &lWork );

                           nRC = 0;
                        }
                        else
                        {
                           nRC = TableEntryExtToInt( (zPVOID) &lWork, lpView, lpDomain,
                                                     pchContextName, (zPCHAR) pvData );
                           if ( nRC < 0 && bNull )
                           {
                              lWork = lNullInteger; // null is always acceptable
                              nRC = 0;
                           }
                        }

                        if ( nRC == 0 )
                        {
                           zLONG lZero = 0;

                           fnGetAddrForAttribute( (zCOREMEM) &lplInteger, lpView,
                                                  lpViewEntityCsr, 0, lpViewAttribute );

                           if ( lpViewAttribute->cType == zTYPE_DECIMAL )
                           {
                              if ( SysCompareDecimalToNull( (zPDECIMAL) lplInteger ) == 0 )
                                 lTempValue = lNullInteger;
                              else
                                 SysConvertDecimalToLong( (zPDECIMAL) lplInteger,
                                                          &lTempValue );
                           }
                           else
                           {
                              lTempValue = *lplInteger;
                           }

                           // for compatibility reasons:
                           //  NULL attribute compared to 0 matches
                           if ( lTempValue == lWork ||
                                (lTempValue == lNullInteger && lWork == 0) )
                           {
                              return( 0 );
                           }
                           else
                              return( lTempValue < lWork ? -1 : 1 );
                        }
                        else
                        {
                           if ( nRC == -1 )
                           {
                              // "TZDME025 - Invalid table value "
                              SendDomainError( lpView, lpDomain, 0, 25, 0,
                                               lpViewEntity->szName,
                                               lpViewAttribute->szName );
                           }

                           return( zCALL_ERROR );
                        }
                     }

                  default:
                     // "TZDME001 - Invalid input data type "
                     SendDomainError( lpView, lpDomain, 0, 1, 0,
                                      lpViewEntity->szName,
                                      lpViewAttribute->szName );
                     return( zCALL_ERROR );
               }

            case zTYPE_INTEGER:
            case zTYPE_DECIMAL:

               // When both the variable coming in and the target Domain are
               // integer/decimal, we will do an internal value compare and
               // forget the Context. In all cases we will convert the values
               // to integer before making the compare.

               if ( lpViewAttribute->cType == zTYPE_INTEGER ||
                    lpViewAttribute->cType == zTYPE_DECIMAL )
               {
                  if ( chType == zTYPE_DECIMAL )
                     SysConvertDecimalToLong( (zPDECIMAL) pvData, &lTempValue );
                  else
                     lTempValue = (zLONG) *((zPLONG) pvData);

                  fnGetAddrForAttribute( (zCOREMEM) &lplInteger, lpView,
                                          lpViewEntityCsr, 0, lpViewAttribute );

                  if ( lpViewAttribute->cType == zTYPE_DECIMAL )
                  {
                     if ( SysCompareDecimalToNull( (zPDECIMAL) lplInteger ) == 0 )
                        lTempValue2 = 0;
                     else
                     {
                        SysConvertDecimalToLong( (zPDECIMAL) lplInteger,
                                                 &lTempValue2 );
                     }
                  }
                  else
                  {
                     if ( *lplInteger == lNullInteger )
                        lTempValue2 = 0;
                     else
                        lTempValue2 = (zLONG) *((zPLONG) lplInteger);
                  }

                  // Now do the compare.
                  if ( lTempValue == lTempValue2 )
                     return( 0 );
                  else
                     return( (lTempValue2 < lTempValue) ? -1 : 1 );
               }
               else
                  ; // fall into default code, for error processing

            default:
               // "TZDME001 - Invalid input data type "
               SendDomainError( lpView, lpDomain, 0, 1, 0,
                                lpViewEntity->szName,
                                lpViewAttribute->szName );
               return( zCALL_ERROR );
         }

         // FALL THROUGH!!!
         // FALL THROUGH!!!
         // FALL THROUGH!!!
      }

      case zDME_GET_FIRST_TBL_ENT_FOR_ATTR:
      case zDME_GET_NEXT_TBL_ENT_FOR_ATTR:
      {
         zLONG lFlag;

         if ( lpViewAttribute->bRequired )
            lFlag = 1;
         else
            lFlag = 0;

         if ( lEntryType == zDME_GET_NEXT_TBL_ENT_FOR_ATTR )
           lFlag += 2;  // asking for next

         nRC = fnGetTableEntryForDomain( lpView, lpViewEntity, lpViewAttribute,
                                         lpDomain, cpcContextName,
                                         pvData, ulMaxStringLth,
                                         pvTablePosition, lFlag );
         if ( nRC == zCALL_ERROR )
         {
            // "TZDME008 - Could not find context for Domain "
            SendDomainError( lpView, lpDomain, 0, 8, 0,
                             lpViewEntity->szName,
                             lpViewAttribute->szName );
         }

         break;
      }

      case zDME_SET_ATTRIBUTE_VALUE_NEXT:
      case zDME_SET_ATTRIBUTE_VALUE_PREV:
      {
         zCHAR             c;
         zLPCONTEXT        lpSrchContext;
         LPTABLEENTRY      lpTableEntry;
         zPVOID            lpAttrValue;
         zCHAR             cAttrType;
         zULONG            uAttrLth;

         nRC = GetValueFromRecord( lpView, lpViewEntity, lpViewAttribute,
                                   (zCOREMEM) &lpAttrValue,
                                   &cAttrType, &uAttrLth );
         if ( nRC )
         {
            // error message issued by lower level routine...
            return( zCALL_ERROR );
         }

         if ( lpAttrValue == 0 )
         {
            // Attribute value is null, it cannot be set to prev/next value
            nRC = 1;
            break;
         }

         if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
            GetDefaultContext( &lpSrchContext, lpDomain );
         else
            lpSrchContext = fnGetContext( lpDomain, cpcContextName );

         c = lpDomain->cType;

         if ( lpSrchContext )
         {
            // The context for this domain has been located. Scan the table
            // for the current value of the attribute.
            lpTableEntry = zGETPTR( lpSrchContext->hFirstTableEntry );
            while ( lpTableEntry )
            {
               if ( c == zTYPE_INTEGER )
               {
                  if ( lpTableEntry->unInternal.lInteger ==
                       *((zPLONG)lpAttrValue) )
                     break;
               }
               else
               {
                  if ( lpTableEntry->unInternal.hStringValue )
                  {
                     if ( zstrcmp( zGETPTR( lpTableEntry->
                                                  unInternal.hStringValue ),
                                   (zPCHAR) lpAttrValue ) == 0 )
                     {
                        break;
                     }
                  }
                  else
                  {
                     if ( *((zPCHAR) lpAttrValue) == 0 )
                        break;
                  }
               }

               lpTableEntry = zGETPTR( lpTableEntry->hNextTableEntry );
            }

            if ( lpTableEntry )
            {
               zPVOID         pvDataToStore;
               LPTABLEENTRY   lpOrigTableEntry;

               // We have found the current table entry.  Now go up or down
               // depending on the input operation id. Also note that we
               // perform wrap around processing.

               lpOrigTableEntry = lpTableEntry; // used to prevent looping

               if ( lEntryType == zDME_SET_ATTRIBUTE_VALUE_NEXT )
               {
                  lpTableEntry = zGETPTR( lpTableEntry->hNextTableEntry );
                  if ( lpTableEntry == 0 )
                     lpTableEntry = zGETPTR( lpSrchContext->hFirstTableEntry );

                  if ( lpViewAttribute->bRequired &&
                       ((c == zTYPE_INTEGER &&
                         lpTableEntry->unInternal.lInteger == lNullInteger) ||
                        (c == zTYPE_STRING &&
                         lpTableEntry->unInternal.hStringValue == 0)) )
                  {
                     while ( TRUE )
                     {
                        if ( (c == zTYPE_INTEGER &&
                              lpTableEntry->unInternal.lInteger == lNullInteger) ||
                             (c == zTYPE_STRING &&
                              lpTableEntry->unInternal.hStringValue == 0) )
                        {
                           lpTableEntry =
                                    zGETPTR( lpTableEntry->hNextTableEntry );
                           if ( lpTableEntry == 0 )
                              lpTableEntry =
                                  zGETPTR( lpSrchContext->hFirstTableEntry );

                           if ( lpTableEntry == lpOrigTableEntry )
                              return( 1 ); // indicate cannot set value
                        }
                        else
                           break;
                     }
                  }
               }
               else
               {
                  // ( lEntryType == zDME_SET_ATTRIBUTE_VALUE_PREV )
                  lpTableEntry = zGETPTR( lpTableEntry->hPrevTableEntry );
                  if ( lpTableEntry == 0 )
                     lpTableEntry = zGETPTR( lpSrchContext->hLastTableEntry );

                  if ( lpViewAttribute->bRequired &&
                       ((c == zTYPE_INTEGER &&
                         lpTableEntry->unInternal.lInteger == lNullInteger) ||
                        (c == zTYPE_STRING &&
                         lpTableEntry->unInternal.hStringValue == 0)) )
                  {
                     while ( TRUE )
                     {
                        if ( (c == zTYPE_INTEGER &&
                              lpTableEntry->unInternal.lInteger == lNullInteger) ||
                             (c == zTYPE_STRING &&
                              lpTableEntry->unInternal.hStringValue == 0) )
                        {
                           lpTableEntry =
                                 zGETPTR( lpTableEntry->hPrevTableEntry );
                           if ( lpTableEntry == 0 )
                              lpTableEntry =
                                 zGETPTR( lpSrchContext->hLastTableEntry );

                           if ( lpTableEntry == lpOrigTableEntry )
                              return( 1 ); // indicate cannot set value
                        }
                        else
                           break;
                     }
                  }
               }

               if ( c == zTYPE_INTEGER )
               {
                  pvDataToStore = (zPVOID)
                                  &(lpTableEntry->unInternal.lInteger);
               }
               else
               {
                  if ( lpTableEntry->unInternal.hStringValue )
                     pvDataToStore = (zPVOID)
                            zGETPTR( lpTableEntry->unInternal.hStringValue );
                  else
                     pvDataToStore = (zPVOID) szNullS;
               }

               nRC = StoreValueInRecord( lpView, lpViewEntity,
                                         lpViewAttribute,
                                         pvDataToStore, 0 );
            }
            else
            {
               // "TZDME025 - Invalid table value "
               SendDomainError( lpView, lpDomain, 0, 25, 0,
                                lpViewEntity->szName,
                                lpViewAttribute->szName );
               nRC = zCALL_ERROR;
            }
         }
         else
         {
            // "TZDME008 - Could not find context for Domain "
            SendDomainError( lpView, lpDomain, 0, 8, 0,
                             lpViewEntity->szName,
                             lpViewAttribute->szName );
            nRC = zCALL_ERROR;
         }

         break;
      }

      case zDME_ADD_TO_ATTRIBUTE:
         break;

      case zDME_GET_COMPARE_VALUE:
         //  -1 - pvData invalid, that is not found in table...
         //   0 - pvData unchanged, okay to use as is.
         //   1 - pvData returned, string value
         //   2 - pvData returned, integer value
         switch ( chType )
         {
            case zTYPE_INTEGER:
               // Integer type data accepted only if the attribute data
               // type is integer.  If not zCALL_ERROR returned.
               if ( lpViewAttribute->cType != zTYPE_INTEGER )
                  break;

               // If both the variable and attribute are of type integer and
               // no context is specified, the variable is returned as is.
               if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
                  nRC = 0;
               else
               {
               // If both the variable and attribute are of type integer
               // and a context is specified, convert the integer to a
               // a string, then locate the resultant string as
               // as an external value for the specified context,
               // and return the corresponding internal value.
                  zCHAR szIntString[ 16 ];
                  zLONG lInternalValue;

                  // convert to string then look up using context
                  zltoa( *((zPLONG) pvData), szIntString, zsizeof( szIntString ) );
                  nRC = TableEntryExtToInt( (zPVOID) &lInternalValue,
                                            lpView,
                                            lpDomain,
                                            cpcContextName,
                                            (zPCHAR) szIntString );
                  // If TableEntryExtToInt returns a not found (-1)
                  // just return that along to the caller of the
                  // Table_Handler. (zCALL_ERROR gets passed along too.)
                  if ( nRC == 0 )
                  {
                     nRC = 2;
                     *((zPLONG) pvData) = lInternalValue;
                  }
               }

               break;

            case zTYPE_STRING:
               switch ( lpViewAttribute->cType )
               {
                  case zTYPE_INTEGER:
                     {
                        zLONG    lIntValue;
                        zPCHAR   pchContext;

                        if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
                           pchContext = lpDomain->szName;
                        else
                           pchContext = (zPCHAR) cpcContextName;

                        // If the variable is of type string and the
                        // attribute is of type integer, locate an external
                        // value, within the specified context (or default
                        // context if non specified), that matches the
                        // the variable and return the corresponding internal
                        // table value.
                        nRC = TableEntryExtToInt( (zPVOID) &lIntValue, lpView,
                                                  lpDomain,
                                                  pchContext,
                                                  (zPCHAR) pvData );
                        // If TableEntryExtToInt returns a not found (-1)
                        // just return that along to the caller of the
                        // Table_Handler. (zCALL_ERROR gets passed along too.)
                        if ( nRC == 0 )
                        {
                           nRC = 2;
                           *((zPLONG) pvData) = lIntValue;
                        }
                     }

                     break;

                  case zTYPE_STRING:
                     // If both the variable and attribute are of type string
                     // and no context is specified, return value as is.

                     /* DGC 1999.01.21 Used to be but don't know why:

                     if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 ||
                          zstrcmp( cpcContextName, lpDomain->szName ) == 0 )

                     I changed it because if the domain and context are the same
                     we shouldn't ignore it.
                     */
                     if ( cpcContextName == 0 || cpcContextName[ 0 ] == 0 )
                        nRC = 0;
                     else
                     {
                        // If both the variable and the attribute are of type
                        // string and a context different from the Domain is
                        // specified, locate the external table value, with
                        // specified context, and return the corresponding
                        // internal value.
                        nRC = TableEntryExtToInt( (zPVOID) szWorkString, lpView,
                                                  lpDomain,
                                                  cpcContextName,
                                                  (zPCHAR) pvData );
                        // If TableEntryExtToInt returns a not found (-1)
                        // just return that along to the caller of the
                        // Table_Handler. (zCALL_ERROR gets passed along too.)
                        if ( nRC == 0 )
                        {
                           nRC = 1;
                           strcpy_s( (zPCHAR) pvData, ulMaxStringLth, szWorkString );
                        }
                     }

                     break;

                  default:
                     if ( lEntryType == zDME_VALIDATE_LPDATA )
                        nSeverity = -1;
                     else
                        nSeverity = 0;

                     // "TZDME003 - Attribute type invalid for this Domain "
                     SendDomainError( lpView, lpDomain, nSeverity, 3, 0,
                                      lpViewEntity->szName,
                                      lpViewAttribute->szName );
                     break;
               }

               break;

            default:
               if ( lEntryType == zDME_VALIDATE_LPDATA )
                  nSeverity = -1;
               else
                  nSeverity = 0;

               // "TZDME001 - Invalid input data type "
               SendDomainError( lpView, lpDomain, nSeverity, 1, 0,
                                lpViewEntity->szName,
                                lpViewAttribute->szName );
               break;
         }

         break;

      default:
         if ( lEntryType == zDME_VALIDATE_LPDATA )
            nSeverity = -1;
         else
            nSeverity = 0;

         // "TZDME001 - Invalid input data type "
         SendDomainError( lpView, lpDomain, nSeverity, 1, 0,
                          lpViewEntity->szName,
                          lpViewAttribute->szName );
         return( zCALL_ERROR );
   }

   return( nRC );
}
