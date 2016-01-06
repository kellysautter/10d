#include "kzoephdr.h"
//
// MODULE NAME:  KZOEMIAA  -  Object services Meta Inquiry routines
// DESCRIPTION:  This is the source file which contains Object services
//               operations for querying Meta information
// OPERATIONS:   OPERATIONS which retrieve meta information
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  John Price
//  DATE:    12/1/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2003.10.23  DKS
   Added operation MiSetInstanceUpdateFlag.

2003.05.12  DGC
   Added the ability of MiGetObjectNameForView to be able to handle subtask
   views.  It returns an empty string for the LOD name.

2001.04.20  DGC
   Added flag to fnValidViewAttrib( ) to allow hidden attributes.

2000.05.02  DGC  10b
   Added MiGetActivateFlags( ).

1999.11.16  US   10a
   Variable names in FreespaceStruct and DataHeaderStruct changed
   according to chanches in Modul kzoemmaa.c

97.09.26    DKS
   Incorporated memory checking for OE allocated memory.  Added
   MiListOE_Memory which lists allocated memory.  MiListOE_Memory
   may be used by an application to analyze OE's memory usage.
   Dataspace alloc/free tracing may be toggled on or off by calling
   MiListOE_Memory with nLevel set to -1.

97.06.16    DGC
   Added operation MiValidView to check if a view was valid.

*/

extern zBOOL g_bTraceDataspace;

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

//./ ADD NAME=MiGetParentEntityNameForView
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetParentEntityNameForView
//
//  PURPOSE: Find entity name of the parent of the specified entity.
//
//  RETURNS: 0     - parent name in return string, will be null if
//                   the input entity name is the root.
//     zCALL_ERROR - error in call, return string set to null
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
MiGetParentEntityNameForView( zPCHAR   pchReturnString,
                              zVIEW    lpView,
                              zCPCHAR  cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPVIEWCSR        lpViewCsr;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITY     lpViewEntity;
   zSHORT           nRC;

   // First initialize return string to null.
   pchReturnString[ 0 ] = 0;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetParentEntityNameForView,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Set lpViewCsr
   lpViewCsr = zGETPTR( lpView->hViewCsr );

   // Start off assuming we'll get a hit
   nRC = 0;

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // If view entity csr at level 1 (root), parent is null.

   if ( lpViewEntityCsr->nLevel == 1 )
      ;  // done...
   else

   // If view entity csr has a parent pass back its name.
   if ( lpViewEntityCsr->hParent )
   {
      LPVIEWENTITYCSR lpParent;
      LPVIEWENTITY    lpParentViewEntity;

      lpParent           = zGETPTR( lpViewEntityCsr->hParent );
      lpParentViewEntity = zGETPTR( lpParent->hViewEntity );

      strcpy_s( pchReturnString, 33, lpParentViewEntity->szName );
   }
   else

   // if view entity csr level matches view entity level,
   // return the view entity parent name
   if ( lpViewEntityCsr->nLevel == lpViewEntity->nLevel )
   {
      LPVIEWENTITY lpParent;

      lpParent = zGETPTR( lpViewEntity->hParent );
      strcpy_s( pchReturnString, 33, lpParent->szName );
   }
   else

   // Look for the recursive child in the subobject view
   if ( lpViewCsr->hFirstViewSubobject )
   {
      LPVIEWSUBOBJECT   lpViewSubobject;
      LPVIEWENTITY      lpRecursViewEntity;
      LPVIEWENTITY      lpParent;

      lpViewSubobject = zGETPTR( lpViewCsr->hFirstViewSubobject );
      if ( lpViewSubobject->hRecursViewEntity )
      {
         lpRecursViewEntity = zGETPTR( lpViewSubobject->hRecursViewEntity );
         lpParent           = zGETPTR( lpRecursViewEntity->hParent );
         strcpy_s( pchReturnString, 33, lpParent->szName );
      }
      else
      {
         // "KZOEE300 - Problems finding a parent "
         fnIssueCoreError( lpCurrentTask, lpView, 16, 300, 0, cpcEntityName, 0 );
         nRC = zCALL_ERROR;
      }
   }
   else
   {
      // "KZOEE300 - Problems finding a parent "
      fnIssueCoreError( lpCurrentTask, lpView, 16, 300, 0, cpcEntityName, 0 );
      nRC = zCALL_ERROR;
   }

   fnOperationReturn( iMiGetParentEntityNameForView, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=MiGetInstanceID_ForView
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetInstanceID_ForView
//
//  PURPOSE: Get the unique instance identifier for a view.
//
//  RETURNS: zCALL_ERROR  - ERROR in call
//           !zCALL_ERROR - an instance ID successfully returned
//                          RETURNED AS A HANDLE!
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG  OPERATION
MiGetInstanceID_ForView( zVIEW    lpView )
{
   LPTASK      lpCurrentTask;
   LPVIEWCSR   lpViewCsr;
   zLONG       lRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetInstanceID_ForView, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lRC = zCALL_ERROR;
   if ( lpView->hViewCsr )
   {
      lpViewCsr = zGETPTR( lpView->hViewCsr );
      lRC = (zLONG) lpViewCsr->hViewOI;
   }

   fnOperationReturn( iMiGetInstanceID_ForView, lpCurrentTask );
   return( lRC );
}

//./ ADD NAME=MiGetUpdateForView
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetUpdateForView
//
//  PURPOSE: Get the update indicator for a view.
//
//  RETURNS: 0     - View is NOT Updateable
//           1     - View IS Updateable
//           zCALL_ERROR
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
MiGetUpdateForView( zVIEW    lpView )
{
   LPTASK     lpCurrentTask;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetUpdateForView, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   fnOperationReturn( iMiGetUpdateForView, lpCurrentTask );
   if ( lpView->bReadOnly )
      return( 0 );       // NOT Updateable
   else
      return( 1 );       // Updateable
}

//./ ADD NAME=MiSetInstanceUpdateFlag
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiSetInstanceUpdateFlag
//
//  PURPOSE: Set the instance update indicator for a view, returning
//           the original update indicator setting.
//
//  OPTION:  lFlag  0 - Set instance updated indicator off.
//                  1 - Set instance updated indicator on.
//
//  RETURNS: 0     - Instance was not updated.
//           1     - Instance was previously updated.
//           zCALL_ERROR
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG  OPERATION
MiSetInstanceUpdateFlag( zVIEW lpView, zLONG lFlag )
{
   LPTASK     lpCurrentTask;
   LPVIEWCSR  lpViewCsr;
   LPVIEWOI   lpViewOI;
   zBOOL      bUpdated = FALSE;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiSetInstanceUpdateFlag, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   bUpdated = lpViewOI->bUpdated || lpViewOI->bUpdatedFile;
   lpViewOI->bUpdatedFile = lpViewOI->bUpdated = (lFlag & 0x00000001) ? TRUE : FALSE;

   fnOperationReturn( iMiSetInstanceUpdateFlag, lpCurrentTask );
   return( bUpdated );
}

//./ ADD NAME=MiGetTypeForViewEntityAttr
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetTypeForViewEntityAttr
//
//  PURPOSE: Get the type indicator for a view entity attribute
//
//  RETURNS: Type - if View/Entity/Attribute valid
//              zTYPE_STRING    'S'
//              zTYPE_INTEGER   'L'
//              zTYPE_DECIMAL   'M'
//              zTYPE_BLOB      'B'
//              zTYPE_DATETIME  'T'
//              zTYPE_PIC       'P'
//              zTYPE_FIXEDCHAR 'F'
//              zTYPE_DATE      'D'
//              zTYPE_TIME      'I'
//           Otherwise - zCALL_ERROR
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
MiGetTypeForViewEntityAttr( zVIEW    lpView,
                            zCPCHAR  cpcEntityName,
                            zCPCHAR  cpcAttribName )
{
   LPTASK        lpCurrentTask;
   LPVIEWOD      lpViewOD;
   LPVIEWENTITY  lpViewEntity;
   LPVIEWATTRIB  lpViewAttrib = 0;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetTypeForViewEntityAttr,
                                          lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Get the view object definition for the view.
   lpViewOD = zGETPTR( lpView->hViewOD );

   // Now make sure that the entity is valid for the view.
   for ( lpViewEntity = zGETPTR( lpViewOD->hFirstOD_Entity );
         lpViewEntity;
         lpViewEntity = zGETPTR( lpViewEntity->hNextHier ) )
   {
      if ( zstrcmp( lpViewEntity->szName, cpcEntityName ) == 0 )
      {
         // Get first attribute for entity.
         lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );

         // Look for the attribute.
         while ( lpViewAttrib )
         {
            if ( zstrcmp( lpViewAttrib->szName, cpcAttribName ) == 0 )
            {
               break;
            }

            lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
         }

         break;  // we found the entity
      }
   }

   fnOperationReturn( iMiGetTypeForViewEntityAttr, lpCurrentTask );
   if ( lpViewAttrib )
      return( lpViewAttrib->cType );
   else
      return( zCALL_ERROR );
}

//./ ADD NAME=MiGetUpdateForViewEntityAttr
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetUpdateForViewEntityAttr
//
//  PURPOSE: Get the update indicator for a view entity
//
//  OPTION:  zUPD_INCLUDE      Check if entity is includable only
//           zUPD_NONPERSIST   Check if attribute is not persistent
//
//  RETURNS: 0     - Entity/Attr is NOT Updateable
//           1     - Entity/Attr IS Updateable
//           zCALL_ERROR
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
MiGetUpdateForViewEntityAttr( zVIEW    lpView,
                              zCPCHAR  cpcEntityName,
                              zCPCHAR  cpcAttributeName,
                              zSHORT   nOption )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWATTRIB      lpViewAttrib;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetUpdateForViewEntityAttr,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists.
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iMiGetUpdateForViewEntityAttr, lpCurrentTask );
      return( zCALL_ERROR );
   }

   // Validate that the Attrib exists.
   lpViewAttrib = 0;
   if ( cpcAttributeName && cpcAttributeName[ 0 ] &&
        (lpViewAttrib = fnValidViewAttrib( lpView, lpViewEntity,
                                           cpcAttributeName, 0 )) == 0 )
   {
      fnOperationReturn( iMiGetUpdateForViewEntityAttr, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( nOption & zUPD_INCLUDE )
   {
      fnOperationReturn( iMiGetUpdateForViewEntityAttr, lpCurrentTask );
      if ( lpView->bReadOnly || lpViewEntity->bInclude == FALSE )
         return( 0 );
      else
         return( 1 );
   }

   if ( lpView->bReadOnly || lpViewEntity->bUpdate == FALSE )
   {
      fnOperationReturn( iMiGetUpdateForViewEntityAttr, lpCurrentTask );
      if ( (nOption & zUPD_NONPERSIST) != 0 && lpViewAttrib && lpViewAttrib->bPersist == FALSE )
         return( 1 );
      else
         return( 0 );
   }

   if ( lpViewAttrib == 0 || lpViewAttrib->bKey == FALSE )
   {
      fnOperationReturn( iMiGetUpdateForViewEntityAttr, lpCurrentTask );
      return( 1 );
   }

   // If we get here then the attribute is a key.  If the cursor is
   // not established, get a cursor position on an entity instance.
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
   if ( lpEntityInstance == UNSET_CSR )
      lpEntityInstance = fnEstablishCursorForView( lpViewEntityCsr );

   // If there is an entity instance and it has not been created, then
   // we will say that the attribute cannot be updated because the
   // attribute is a key and keys cannot be changed once they have
   // been stored to a database, otherwise, we will return that the
   // attribute can be updated.
   fnOperationReturn( iMiGetUpdateForViewEntityAttr, lpCurrentTask );
   if ( lpEntityInstance && lpEntityInstance->u.nInd.bCreated == FALSE )
      return( 0 );
   else
      return( 1 );
}

//./ ADD NAME=MiGetTemporalStateOfEntity
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetTemporalStateOfEntity
//
//  PURPOSE: Get an indicator of the temporal state of a view entity
//
//  RETURNS: zCURSOR_NULL          - Entity cursor is null
//           zCURSOR_UNDEFINED     - Entity cursor is undefined
//           zTES_NONE             - Entity has no Temporal instances
//           zTES_SUBOBJECT_CHILD  - Entity is a child of a Temporal
//                                   Subobject Root
//           zTES_TEMPORAL         - Entity is a Temporal Entity
//           zTES_SUBOBJECT_ROOT   - Entity is a Temporal Subobject Root
//           zCALL_ERROR           - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
MiGetTemporalStateOfEntity( zVIEW    lpView,
                            zCPCHAR  cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetTemporalStateOfEntity, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iMiGetTemporalStateOfEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Establish cursor if necessary.
   if ( lpEntityInstance == UNSET_CSR )
      lpEntityInstance = fnEstablishCursorForView( lpViewEntityCsr );

   //
   // 1. Check Cursor for null or undefined.
   //
   if ( lpEntityInstance == 0L )
      nRC = zCURSOR_NULL;
   else
   if ( lpEntityInstance->u.nInd.bHidden )
      nRC = zCURSOR_UNDEFINED;
   else
   {
   //
   // 2. Check the EntityInstance for being marked as temporal.
   //
      if ( lpEntityInstance->u.nInd.bTemporal )
         // Indicate Entity instance is a Temporal Entity.
         nRC = zTES_TEMPORAL;
      else
   //
   // 3. Does Instance have a previous version
   //
      if ( lpEntityInstance->hPrevVsn )
      {
         LPENTITYINSTANCE lpPrevVsn = zGETPTR( lpEntityInstance->hPrevVsn );

         if ( lpPrevVsn->u.nInd.bPrevVsnRoot )
         {
            // Indicate Entity instance is a Temporal Subobject Root.
            nRC = zTES_SUBOBJECT_ROOT;
         }
         else
         {
            // Indicate Entity instance is a child of a Temporal
            // Subobject Root.
            nRC = zTES_SUBOBJECT_CHILD;
         }
      }
      else
      {
   //
   // 4. Is any parent Instance marked as a previous version
   //
         // Default to entity with no Temporal instances.
         nRC = zTES_NONE;

         // Go up the parent chain looking for an instance marked as
         // a previous version.
         for ( lpEntityInstance = zGETPTR( lpEntityInstance->hParent );
               lpEntityInstance;
               lpEntityInstance = zGETPTR( lpEntityInstance->hParent ) )
         {
            if ( lpEntityInstance->u.nInd.bPrevVersion || lpEntityInstance->hPrevVsn )
            {
               // Indicate entity instance is child of Temporal
               // Subobject Root.
               nRC = zTES_SUBOBJECT_CHILD;
               break;
            }
         }
      }
   }

   fnOperationReturn( iMiGetTemporalStateOfEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=MiGetDateTimeForOI
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetDateTimeForOI
//
//  PURPOSE: For the specified view, return the DateTimeRecord of when
//           the ObjectInstance was built within core.
//
//  RETURNS: 0     - DateTimeRecord of when the OI was created.
//           zCALL_ERROR   - error in call, return string set to zeros.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
MiGetDateTimeForOI( zPCHAR pchDateTime,
                    zVIEW  lpView )
{
   LPTASK     lpCurrentTask;
   LPVIEWCSR  lpViewCsr;
   LPVIEWOI   lpViewOI;

   // First set return area to zeros
   zmemset( pchDateTime, 0, sizeof( DateTimeRecord ) );

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetDateTimeForOI, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Set lpViewCsr
   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI = zGETPTR( lpViewCsr->hViewOI );

   // copy the DateTime of when the OI was created, into the return area.
   zmemcpy( pchDateTime, (zPCHAR) &(lpViewOI->DateTime),
            sizeof( DateTimeRecord ) );

   fnOperationReturn( iMiGetDateTimeForOI, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=MiGetERTokenForEntity
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetERTokenForEntity
//
//  PURPOSE: For the specified view, return the DateTimeRecord of when
//           the ObjectInstance was built within core.
//
//  RETURNS: > 0  - value of the token
//           zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zLONG OPERATION
MiGetERTokenForEntity( zVIEW     lpView,
                       zCPCHAR   cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITY     lpViewEntity;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetERTokenForEntity, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView,
                                           cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iMiGetERTokenForEntity, lpCurrentTask );
      return( zCALL_ERROR );
   }

   fnOperationReturn( iMiGetERTokenForEntity, lpCurrentTask );
   return( lpViewEntity->lEREntTok );
}

//./ ADD NAME=MiGetKeyFromInstance
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetKeyFromInstance
//
//  PURPOSE: For the specified view/entity return a list of concatenated
//           keys (seperated by ".").
//
//  RETURNS: > 0 - length of returned string
//             0 - no keys found...
//            -1 - key had a null value...
//   zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
MiGetKeyFromInstance( zPCHAR  pchReturnString,
                      zSHORT  nLth,
                      zVIEW   lpView,
                      zCPCHAR cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWATTRIB     lpViewAttrib;
   zPCHAR           pch;
   zSHORT           nBytesLeft;

   pchReturnString[ 0 ] = 0;   // init return string

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetKeyFromInstance, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView,
                                           cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iMiGetKeyFromInstance, lpCurrentTask );
      return( zCALL_ERROR );
   }

   pch = pchReturnString; // set pointer
   nBytesLeft = nLth;
   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      zSHORT   nRC;

      if ( lpViewAttrib->bKey == FALSE )
         continue;

      if ( nBytesLeft < nLth )
      {
         strcat_s( pch, nLth - (pch - pchReturnString), "." );
         pch++;
         nBytesLeft--;
      }

      if ( nBytesLeft < 1 )
      {
         // we ran out of buffer space...
         // "KZOEE301 - Return buffer not large enough to
         //  contain concatenated keys"
         fnIssueCoreError( lpCurrentTask, lpView, 8, 301, nLth,
                           lpViewEntity->szName, 0 );
         fnOperationReturn( iMiGetKeyFromInstance, lpCurrentTask );
         return( zCALL_ERROR );
      }

      nRC = fnGetStringFromAttribute( pch, nBytesLeft,
                                      lpView, lpViewEntityCsr,
                                      lpViewAttrib, 0 );

      // -1 return means attribute was null.
      // zCALL_ERROR for blobs...
      if ( nRC )
      {
         fnOperationReturn( iMiGetKeyFromInstance, lpCurrentTask );
         return( nRC );
      }

      while ( *pch && nBytesLeft )
      {
         pch++;
         nBytesLeft--;
      }

   } // for...

   fnOperationReturn( iMiGetKeyFromInstance, lpCurrentTask );
   return( nLth - nBytesLeft );
}

//./ ADD NAME=MiGetObjectNameForView
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetObjectNameForView
//
//  PURPOSE: For the specified view, return the object name associated
//           with it.
//
//  RETURNS:   0 - View Object Name
//             zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
MiGetObjectNameForView( zPCHAR pchReturnName, zVIEW lpView )
{
   LPTASK     lpCurrentTask;
   LPVIEWOD   lpViewOD;

   pchReturnName[ 0 ] = 0;  // init return name

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetObjectNameForView, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( lpView->hViewOD == 0 )
   {
      *pchReturnName = 0;
      fnOperationReturn( iMiGetObjectNameForView, lpCurrentTask );
      return( 0 );
   }

   lpViewOD = zGETPTR( lpView->hViewOD );
   strcpy_s( pchReturnName, 33, lpViewOD->szName );

   fnOperationReturn( iMiGetObjectNameForView, lpCurrentTask );
   return( 0 );
}

//./ ADD NAME=MiGetViewEntityForView
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetViewEntityForView
//
//  PURPOSE: For the specified view and entity name, return the
//           ViewEntity Struct.  Intended for use by DB Handlers.  If
//           cpcEntityName is 0, then the lpViewOD is returned.
//
//  RETURNS:   0 - error...
//            >0 - address of the ViewEntity struct
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPVIEWENTITY OPERATION
MiGetViewEntityForView( zVIEW lpView, zCPCHAR cpcEntityName )
{
   LPTASK           lpCurrentTask;
   LPVIEWENTITYCSR  lpViewEntityCsr;
   LPVIEWENTITY     lpViewEntity;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetViewEntityForView, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( 0 );
   }

   if ( cpcEntityName == 0 )
   {
      fnOperationReturn( iMiGetViewEntityForView, lpCurrentTask );
      return( (LPVIEWENTITY) zGETPTR( lpView->hViewOD ) );
   }

   // Validate that the entity exists.
   lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpView, cpcEntityName, 0 );

   fnOperationReturn( iMiGetViewEntityForView, lpCurrentTask );
   return( lpViewEntity );
}

//./ ADD NAME=MiGetOI_ReleaseForView
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetOI_ReleaseForView
//
//  PURPOSE: For the specified view, return the OI Release.  Intended
//           for use by version checking routines.  The Release string
//           is returned in the caller provided string which should
//           hold the eight byte release string plus the null terminator
//           (for a total of 9 bytes).
//
//  RETURNS:    0 - error
//              otherwise - aok
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT  OPERATION
MiGetOI_ReleaseForView( zVIEW    lpView,
                        zPCHAR   pchReturnRelease )
{
   LPTASK     lpCurrentTask;
   LPVIEWCSR  lpViewCsr;
   LPVIEWOI   lpViewOI;

   *pchReturnRelease = 0;   // ensure no junk in return value

   // If task not active or disabled, or view csr invalid, return 0.
   if ( (lpCurrentTask = fnOperationCall( iMiGetOI_ReleaseForView, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( 0 );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   strcpy_s( pchReturnRelease, sizeof( lpViewOI->szRelease ), lpViewOI->szRelease );
   fnOperationReturn( iMiGetOI_ReleaseForView, lpCurrentTask );
   return( 1 );
}

//./ ADD NAME=MiSetOI_ReleaseForView
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiSetOI_ReleaseForView
//
//  PURPOSE: For the specified view, set the OI Release.  Intended
//           for use by version checking routines.  The Release string
//           is set in the caller provided string which should be a
//           maximum of eight bytes (plus 1 for the null terminator for
//           a total of 9).  The release strings are of the format:
//           9999.9x9  where the 9's represent digits and the x represents
//           a character, a-z (e.g.  10.1b2).
//
//  RETURNS:    0 - error
//              otherwise - new release has been set for the OI
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT  OPERATION
MiSetOI_ReleaseForView( zVIEW   lpView,
                        zCPCHAR cpcNewRelease )
{
   LPTASK     lpCurrentTask;
   LPVIEWCSR  lpViewCsr;
   LPVIEWOI   lpViewOI;

   // If task not active or disabled, or view csr invalid, return 0.
   if ( (lpCurrentTask = fnOperationCall( iMiSetOI_ReleaseForView, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( 0 );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
   strncpy_s( lpViewOI->szRelease, sizeof( lpViewOI->szRelease ), cpcNewRelease, 8 );
   lpViewOI->szRelease[ 8 ] = 0;

   fnOperationReturn( iMiSetOI_ReleaseForView, lpCurrentTask );
   return( 1 );
}


//./ ADD NAME=MiCompareOI_ReleaseToRelease
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiCompareOI_ReleaseToRelease
//
//  PURPOSE: Compare the view's release to the specified Release.  Intended
//           for use by version checking routines.  The Release string
//           is set in the caller provided string which should be a
//           maximum of eight byte (plus 1 for the null terminator for
//           a total of 9).  The release strings are of the format:
//           9999.9x9  where the 9's represent digits and the x represents
//           a character, a-z (e.g.  10.1b2).
//
//           Note that the CompareRelease buffer will not be altered by
//           this routine, but processing within the routine is simplified
//           if it is not declared const.
//
//  RETURNS:    < 0 - OI Release < CompareRelease
//              = 0 - OI Release == CompareRelease
//              > 0 - OI Release > CompareRelease
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT  OPERATION
MiCompareOI_ToRelease( zVIEW   lpView,
                       zPCHAR  pchCompareRelease )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   zSHORT            nReleaseOI;
   zSHORT            nReleaseCmpr;
   zPCHAR            pchOI;
   zPCHAR            pchCmpr;
   zPCHAR            pchTemp;

   // If task not active or disabled, or view csr invalid, return 0.
   if ( (lpCurrentTask = fnOperationCall( iMiCompareOI_ReleaseToRelease,
                                          lpView, zVALID_VIEW_CSR )) == 0 )
   {
      return( 0 );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   pchOI   = lpViewOI->szRelease;
   pchCmpr = pchCompareRelease;

   // if the number of digits preceding the '.' is larger in one release
   // value than the other, that release value is larger
   pchTemp = zstrchr( pchOI, '.' );
   if ( pchTemp )
      nReleaseOI = pchTemp - pchOI;
   else
      nReleaseOI = 0;

   pchTemp = zstrchr( pchCmpr, '.' );
   if ( pchTemp )
      nReleaseCmpr = pchTemp - pchCmpr;
   else
      nReleaseCmpr = 0;

   if ( nReleaseOI == nReleaseCmpr )
   {
      if ( pchOI && pchCmpr )
      {
         nReleaseCmpr = zstrcmp( pchOI, pchCmpr );
         if ( nReleaseCmpr < 0 )
            nReleaseCmpr = -1;
         else
         if ( nReleaseCmpr > 0 )
            nReleaseCmpr = 1;
      }
      else
      if ( pchOI )
         nReleaseCmpr = 1;
      else
      if ( pchCmpr )
         nReleaseCmpr = -1;
      else
         nReleaseCmpr = 0;
   }
   else
   if ( nReleaseOI > nReleaseCmpr )
      nReleaseCmpr = 1;
   else
      nReleaseCmpr = -1;

   fnOperationReturn( iMiCompareOI_ReleaseToRelease, lpCurrentTask );
   return( nReleaseCmpr );
}

//./ ADD NAME=MiGetCardinalityForEntity
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetCardinalityForEntity
//
//  PURPOSE: For the specified view and entity name, return the
//           minimum and maximum cardinality values for the specified
//           entity.  CardMin = 0 indicates no minimum, CardMax = 0
//           indicates no maximum.
//
//  RETURNS:
//            0 - min and max cardinality values returned.
//  zCALL_ERROR - error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
MiGetCardinalityForEntity( zPUSHORT puCardMin,
                           zPUSHORT puCardMax,
                           zVIEW    lpView,
                           zCPCHAR  cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPVIEWENTITY      lpViewEntity;
   zSHORT            nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetCardinalityForEntity, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists
   lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpView, cpcEntityName, 0 );
   if ( lpViewEntity )
   {
      *puCardMin = lpViewEntity->uCardMin;
      *puCardMax = lpViewEntity->uCardMax;
      nRC = 0;
   }
   else
      nRC = zCALL_ERROR;

   fnOperationReturn( iMiGetCardinalityForEntity, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=MiHasPersistentChanges
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiHasPersistentChanges
//
//  PURPOSE: Checks to see if OI has persistent changes (changes that will
//           written to the database).
//
//  RETURNS: TRUE  if OI has persistent changes.
//           FALSE if not.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
MiHasPersistentChanges( zVIEW lpView )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nRC = FALSE;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiHasPersistentChanges, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   for ( lpEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );
         lpEntityInstance;
         lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier ) )
   {
      LPVIEWENTITY lpViewEntity;

      // First check and make sure there are changes to the EI.
      if ( lpEntityInstance->u.nInd.bCreated == FALSE  &&
           lpEntityInstance->u.nInd.bDeleted == FALSE  &&
           lpEntityInstance->u.nInd.bIncluded == FALSE &&
           lpEntityInstance->u.nInd.bExcluded == FALSE &&
           lpEntityInstance->u.nInd.bUpdated == FALSE  &&
           lpEntityInstance->u.nInd.bDeleted == FALSE )
      {
         continue;
      }

      if ( fnEntityInstanceIsDead( lpEntityInstance ) )
         continue;

      lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
      if ( lpViewEntity->bDerived ||
           lpViewEntity->bDerivedPath ||
           lpViewEntity->bHasDB_Oper ) // added Phil/Doug 2005.10.17
      {
         continue;
      }

      nRC = TRUE;
      break;
   }

   fnOperationReturn( iMiHasPersistentChanges, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=MiGetActivateFlags
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetActivateFlags
//
//  PURPOSE: Returns the flags used to activate the object instance.
//
//  RETURNS: The activate flags.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG OPERATION
MiGetActivateFlags( zVIEW lpView )
{
   LPTASK            lpCurrentTask;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOI          lpViewOI;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiGetActivateFlags, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpViewCsr = zGETPTR( lpView->hViewCsr );
   lpViewOI  = zGETPTR( lpViewCsr->hViewOI );

   fnOperationReturn( iMiGetActivateFlags, lpCurrentTask );
   return( lpViewOI->lActivateControl );
}

zSHORT OPERATION
MiValidView( zVIEW lpView )
{
   if ( lpView == 0 )
      return( FALSE );

   if ( SysValidAddress( (zPVOID) lpView,
                         (zLONG) sizeof( ViewRecord ), 1 ) )
   {
      if ( lpView->nTableID == iView )
         return( TRUE );
   }

   return( FALSE );
}

//./ ADD NAME=MiGetSetViewTaskAppFlags
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiGetSetViewTaskAppFlags
//
//  PURPOSE: For the specified view, get/set the specified view/task/app flag.
//
//           lOption - 0x00000001 - Application KeepAlive
//                     0x00000002 - Task Title
//           lFlag   - 1 - Set flag (otw just get flag)
//                   - 2 - Set flag on (otw set flag off)
//
//  RETURNS: Previous state of flag
//              0 - flag previously not set
//              1 - flag previously set
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zLONG   OPERATION
MiGetSetViewTaskAppFlags( zVIEW   lpView,
                          zLONG   lOption,
                          zPCHAR  pchTaskInfo,
                          zLONG   lFlag )
{
   LPTASK     lpTask;
   LPAPP      lpApp = 0;
   zLONG      lRC = 0;

   // If task not active or disabled, return 0.
   if ( (lpTask = fnOperationCall( iMiGetSetViewTaskAppFlags,
                                   lpView, zVALID_VIEW )) == 0 )
   {
      return( 0 );
   }

   if ( lOption & 0x00000001 ) // Application KeepAlive
   {
   // lRC /= lRC;
      lpApp = zGETPTR( lpTask->hApp );
      if ( lpApp && lpApp->bKeepAlive )
         lRC = 1;
   }

   if ( lFlag & 0x00000001 ) // set flag
   {
      if ( lOption & 0x00000001 ) // Application KeepAlive
      {
         zBOOL bSetTo = (lFlag & 0x00000002) ? 1 : 0;

         if ( lpApp )
         {
            TraceLine( "MiGetSetViewTaskAppFlags setting KeepAlive for Application: %s to %s",
                       lpApp->szName, bSetTo == TRUE ? "ON" : "OFF" );
            lpApp->bKeepAlive = bSetTo;
         }
      }

      if ( lOption & 0x00000002 ) // Task Title
      {
         strncpy_s( lpTask->szTaskTitle, sizeof( lpTask->szTaskTitle ), pchTaskInfo, 127 );
         lpTask->szTaskTitle[ 127 ] = 0;
      }
   }
   else
   if ( lOption & 0x00000002 ) // Task Title
   {
      strcpy_s( pchTaskInfo, sizeof( lpTask->szTaskTitle ), lpTask->szTaskTitle );
   }

   fnOperationReturn( iMiGetSetViewTaskAppFlags, lpTask );
   return( lRC );
}

//./ ADD NAME=MiListOE_Memory
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiListOE_Memory
//
//  PURPOSE: List Object Engine allocated memory.  This may be
//           used by an application to analyze OE's memory usage.
//
//           nLevel  0 - list OE Memory utilization
//                  -1 - toggle allocation trace (and then list)
//
//  RETURNS: Total memory currently allocated by the Object Engine.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zLONG  OPERATION
MiListOE_Memory( zVIEW lpView, zSHORT nLevel )
{
   zCHAR        szPageTable[ zSHR_MAX_PAGES + 1 ];
   zCHAR        szMsg[ 256 + zSHR_MAX_PAGES + 1 ];
   zULONG       ulFreeBytes;
   zULONG       ulTotalAlloc = 0;
   LPTASK       hTask = 0;
   LPTASK       lpTask;
   LPAPP        lpApp;
   LPDATAHEADER hDataHeader;
   LPDATAHEADER lpDataHeader;
   LPFREESPACE  lpPrevFreespace;
   LPFREESPACE  lpNextFreespace;
   zBYTE        by;
   zSHORT       k;

   if ( nLevel == -1 )
   {
      g_bTraceDataspace = !g_bTraceDataspace;
      TraceLineS( "Trace dataspace ", g_bTraceDataspace ? "On" : "Off" );
   }

   if ( lpView )
      hTask = lpView->hTask;

   if ( hTask == 0 && AnchorBlock)
      hTask = AnchorBlock->hFirstTask;

   if ( hTask == 0 || (lpTask = zGETPTR( hTask )) == 0 )
      return( 0 );

   TraceLine( "(mi) List OE Memory ###### BEGIN ###### Task: 0x%08x", hTask );

   zLOCK_MUTEX( zMUTEX_SHAREDMEM );
   fnStartBrowseOfTaskList( lpTask, FALSE );

   // Make sure the task is a valid task
   for ( lpTask = zGETPTR( AnchorBlock->hFirstTask );
         lpTask;
         lpTask = zGETPTR( lpTask->hNextTask ) )
   {
      hTask = zGETHNDL( lpTask );
      by = (zBYTE) ((zULONG) hTask >> 24);
      lpApp = zGETPTR( lpTask->hApp );
      sprintf_s( szMsg, sizeof( szMsg ), " Memory [%x] for Task 0x%08x App: %s Size: %d Status: ",
                 (zLONG) by, (zULONG) hTask, lpApp ? lpApp->szName : "???", lpTask->ulTrackTaskMemory );
      if ( lpTask->bShutdown )
      {
         TraceLineS( szMsg, "Shutdown" );
         continue;
      }
      else
         TraceLineS( szMsg, "Active" );

      k = sizeof( zCHAR ) * zSHR_MAX_PAGES;
      zmemset( szPageTable, '.', k );
      szPageTable[ sizeof( zCHAR ) * zSHR_MAX_PAGES ] = 0;
      hDataHeader = lpTask->hFirstDataHeader;
      while ( hDataHeader )
      {
         by = (zBYTE) ((zULONG) hDataHeader >> 24);
         szPageTable[ by ] = 'Y';

         lpDataHeader = zGETPTR( hDataHeader );
         ulTotalAlloc += lpDataHeader->ulUseableSize;

#if 1 //def WE_NEED_A_MUTEX_JUST_FOR_THIS???
         lpPrevFreespace = &lpDataHeader->Freespace;
         lpNextFreespace = zGETPTR( lpPrevFreespace->hNextFreespace );

         // If lpNextFreespace is null, set it to lpPrevFreespace (the zero-
         // length Freespace link in the table header).
         if ( lpNextFreespace == 0 )
            lpNextFreespace = lpPrevFreespace;

         // lpNextFreespace points to the first free chunk of space
         ulFreeBytes = 0;
         while ( lpNextFreespace )
         {
            ulFreeBytes += lpNextFreespace->ulFreeLth;

            // Move on to next free chunk of space
            lpNextFreespace = zGETPTR( lpNextFreespace->hNextFreespace );
         }

         k = sizeof( zCHAR ) * zSHR_MAX_PAGES - 1;
         while ( szPageTable[ k ] == '.' )
            szPageTable[ k-- ] = 0;

         sprintf_s( szMsg, sizeof( szMsg ), " [%lx] size: %ld; fs ceiling: %ld; total fs: %ld ==> %s",
                    (zLONG) lpDataHeader, lpDataHeader->ulUseableSize,
                    lpDataHeader->ulMaxEmptyBlockSize, ulFreeBytes, szPageTable );
#else
         sprintf_s( szMsg, sizeof( szMsg ), " [%lx] size: %ld; fs ceiling: %ld ==> %s",
                    (zLONG) lpDataHeader, lpDataHeader->lNumberOfEntries,
                    lpDataHeader->ulMaxEmptyBlockSize, szPageTable );
#endif
         TraceLineS( szMsg, "" );

         hDataHeader = lpDataHeader->hNextDataHeader;
      }
   }

   fnEndBrowseOfTaskList( FALSE );
   zUNLOCK_MUTEX( zMUTEX_SHAREDMEM );

   TraceLineI( "(mi) List OE Memory ###### END ###### - total = ",
               ulTotalAlloc );
   return( ulTotalAlloc );
}

//./ ADD NAME=MiEntityVersioned
// Source Module=kzoemiaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:   MiEntityVersioned
//
//  PURPOSE: To determine if an entity is part of a versioned sub-object
//
//  RETURNS: 0 - instance not versioned
//           1 - instance versioned
//           zCALL_ERROR           - error on call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
MiEntityVersioned( zVIEW    lpView,
                   zCPCHAR  cpcEntityName )
{
   LPTASK            lpCurrentTask;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   LPENTITYINSTANCE  lpEntityInstance;
   zSHORT            nRC;

   // If task not active or disabled, or view csr invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iMiEntityVersioned, lpView,
                                          zVALID_VIEW_CSR )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // Validate that the entity exists
   if ( (lpViewEntity = fnValidViewEntity( &lpViewEntityCsr,
                                           lpView, cpcEntityName, 0 )) == 0 )
   {
      fnOperationReturn( iMiEntityVersioned, lpCurrentTask );
      return( zCALL_ERROR );
   }

   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   nRC = fnInstanceVersioned( lpEntityInstance );

   fnOperationReturn( iMiEntityVersioned, lpCurrentTask );
   return( nRC );
}
