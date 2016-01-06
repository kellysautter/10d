#include "kzoephdr.h"
//
// MODULE NAME:  KZOEODAA  -  Object services View OD Activation
// DESCRIPTION:  This is the source file which contains Object services
//               operations for Activating and dropping object definitions
// OPERATIONS:   All OPERATIONS which activate and drop object definitions
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    3/31/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2001.04.19  DGC  10b
   Added ORDER BY stuff.

2001.04.18  DGC  10b
   Added code for DropViewObject( ) logic.

2001.02.26  DGC  10b
   Added "JOIN" field to DataRecord.

2000.06.05  DGC  10b
   Added quick check to make sure a child entity does not have bDupRel flag
   set if one of its parents has bDupRel set.

2000.01.07  DGC  10b
   Added flags in ViewEntityStruct for dealing with abstract entities.

   Removed nMultiRootLimit.

1999.11.29  DGC
   Added code to fnAllocDataspace( ) to help find leaks.

1999.10.14  DGC  10a
   Added lpPrev/NextSibling pointers to ViewEntityStruct.

1999.06.03  DGC  10a
   Added tracing stuff.

1999.05.07  DGC  10a
   Added ER_DATE attribute to the XOD object so we can check for LODs that
   are out of sync.

1999.04.09  DGC  10a
   Added support for internal derived attribute concat functions.

1999.02.12  DGC
   Started adding (and removing) stuff for attribute flags.

*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

zSHORT LOCALOPER
fnDropTaskOperation( LPTASK lpTask, LPTASKOPER lpTaskOperation );

#define __LOAD_CONSTR__

// Following structure used by fnProcessXxx functions.
typedef struct ProcessInfoStruct ProcessInfoRecord;
typedef ProcessInfoRecord *LPPROCESSINFO;
struct ProcessInfoStruct
{
   zLONG          hFile;
   zPCHAR         szLine;
   zCHAR          szAttrName[ 33 ];
   zCHAR          szFileName[ zMAX_FILENAME_LTH + 1 ];
   zLONG          lLine;
   LPAPP          lpSystemApp;
   LPAPP          lpApp;
   zPVOID         hApp;
   LPTASK         lpTask;
   zSHORT         nCurrState;
   zSHORT         nKeyCnt;
   zVIEW          lpView;
   zBOOL          bLth;
   zBOOL          bType;
   zBOOL          bDomain;
   zBOOL          bPersist;
   zULONG         ulRecordOffset;
   zULONG         ulNonPersistOffset;
};

//./ ADD NAME=fnProcessObjectEntity
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnProcessObjectEntity
//
//  PURPOSE:    To process Object entity information for an Object Def.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnProcessObjectEntity( zVIEW         lpView,
                       LPVIEWOD      lpViewOD,
                       LPPROCESSINFO lpProcessInfo )
{
   zPCHAR  pchAttrName;
   zPVOID  hndl;

   pchAttrName = lpProcessInfo->szAttrName;

   switch ( *pchAttrName )
   {
      // Object Attributes beginning with C
      case 'C':
         if ( zstrcmp( pchAttrName, "CACHENUM" ) == 0 )
            lpViewOD->cCacheNbr = (zCHAR) zatol( lpProcessInfo->szLine );

         break;

      // Object Attributes beginning with D
      case 'D':
         if ( zstrcmp( pchAttrName, szlDFT_DBNAME ) == 0 )
         {
            LPLNAME lpLName;

            lpLName = zGETPTR( lpProcessInfo->lpApp->hFirstLName );
            while ( lpLName )
            {
               if ( lpLName->nType == 2 &&
                    zstrcmp( lpLName->szLName,
                             lpProcessInfo->szLine ) == 0 )
               {
                  break;
               }

               lpLName = zGETPTR( lpLName->hNextLName );
            }

            if ( lpLName )
            {
               lpViewOD->bLogicalDBName = TRUE;
               strcpy_s( lpViewOD->szDfltDBName, sizeof( lpViewOD->szDfltDBName ), lpLName->szPName );
            }
            else
               strcpy_s( lpViewOD->szDfltDBName, sizeof( lpViewOD->szDfltDBName ), lpProcessInfo->szLine );
         }
         else
         if ( zstrcmp( pchAttrName, szlDFT_SERVER ) == 0 )
         {
            LPLNAME lpLName;

            lpLName = zGETPTR( lpProcessInfo->lpApp->hFirstLName );
            while ( lpLName )
            {
               if ( lpLName->nType == 1 &&
                    zstrcmp( lpLName->szLName, lpProcessInfo->szLine ) == 0 )
               {
                  break;
               }

               lpLName = zGETPTR( lpLName->hNextLName );
            }

            if ( lpLName )
            {
               lpViewOD->bLogicalServerName = TRUE;
               strcpy_s( lpViewOD->szDfltServer, sizeof( lpViewOD->szDfltServer ), lpLName->szPName );
            }
            else
               strcpy_s( lpViewOD->szDfltServer, sizeof( lpViewOD->szDfltServer ), lpProcessInfo->szLine );
         }
         else
         if ( zstrcmp( pchAttrName, "DBH_Data" ) == 0 )
         {
            zULONG ulLth;
            zPVOID ptr;
            zPCHAR pch;

            ulLth = zatol( lpProcessInfo->szLine );
            lpViewOD->hDBH_Data = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                                    ulLth, 1, 0, 0 );
            ptr = zGETPTR( lpViewOD->hDBH_Data );

            SysReadLineLth( lpView, &pch, lpProcessInfo->hFile, ulLth );
            zmemcpy( ptr, pch, ulLth );
         }

         break;

      case 'E':
         if ( zstrcmp( pchAttrName, "ER_DATE" ) == 0 )
            strcpy_s( lpViewOD->szER_Date, sizeof( lpViewOD->szER_Date ), lpProcessInfo->szLine );

         break;

      // Object Attributes beginning with G
      case 'G':
         if ( zstrcmp( pchAttrName, szlGKHANDLER ) == 0 )
         {
            // Search Application and then system application
            // handlers and link lpViewOD->hGKHandler to the
            // appropriate handler found.
            // Start searches from lpApp->hFirstGKHandler.
            LPGKHANDLER lpGKHandler;

            // Look for GKHandler under current application.
            lpGKHandler = zGETPTR( lpProcessInfo->lpApp->hFirstGKHandler );
            while ( lpGKHandler &&
                    zstrcmp( lpGKHandler->szName, lpProcessInfo->szLine ) != 0 )
            {
               lpGKHandler = zGETPTR( lpGKHandler->hNextGKHandler );
            }

            // If GKhandler wasn't found under current application, look
            // for it in the system application.
            if ( lpGKHandler == 0 )
            {
               LPAPP lpSystemApp = lpProcessInfo->lpSystemApp;

               for ( lpGKHandler = zGETPTR( lpSystemApp->hFirstGKHandler );
                     lpGKHandler;
                     lpGKHandler = zGETPTR( lpGKHandler->hNextGKHandler ) )
               {
                  if ( zstrcmp( lpGKHandler->szName, lpProcessInfo->szLine ) == 0 )
                     break;
               }
            }

            // If GKHandler wasn't found, create the GKHandler.
            if ( lpGKHandler == 0 )
            {
               hndl = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                        sizeof( GKHandlerRecord ), 1, 0,
                                        iGKHandler );

               // Set info in GKHandler object.
               lpGKHandler = zGETPTR( hndl );
               if ( lpGKHandler )
               {
                  lpGKHandler->hNextGKHandler =
                                       lpProcessInfo->lpApp->hFirstGKHandler;
                  lpProcessInfo->lpApp->hFirstGKHandler = hndl;

                  strcpy_s( lpGKHandler->szName, sizeof( lpGKHandler->szName ), lpProcessInfo->szLine );
                  lpGKHandler->hApp = lpProcessInfo->lpApp;
               }
            }

            // If lpGKHandler is still not set, then set error.
            // Otherwise link lpViewOD to the handler.
            if ( lpGKHandler == 0 )
            {
               //  "KZOEE0xx - xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
               // fnIssueCoreError( lpTask, lpView, 16, 54, lLine, szFileName, 0 );
               lpProcessInfo->nCurrState = 10;  // set error
            }
            else
               lpViewOD->hGKHandler = zGETHNDL( lpGKHandler );

         }  // if ( zstrcmp( pchAttrName, szlGKHANDLER ) == 0 )...

         break;

      // Object Attributes beginning with H
      case 'H':
         if ( zstrcmp( pchAttrName, szlHANDLER ) == 0 )
         {
            // Search Application and then system application
            // handlers and link lpViewOD->hDBHandler to the
            // appropriate handler found.
            // Start searches from lpApp->hFirstDBHandler.
            LPDBHANDLER lpDBHandler;

            // Look for DBHandler under current application.
            lpDBHandler = zGETPTR( lpProcessInfo->lpApp->hFirstDBHandler );
            while ( lpDBHandler && zstrcmp( lpDBHandler->szName, lpProcessInfo->szLine ) != 0 )
            {
               lpDBHandler = zGETPTR( lpDBHandler->hNextDBHandler );
            }

            // If DBHandler wasn't found under current application, look
            // for it in the system application.
            if ( lpDBHandler == 0 )
            {
               lpDBHandler =
                     zGETPTR( lpProcessInfo->lpSystemApp->hFirstDBHandler );
               while ( lpDBHandler && zstrcmp( lpDBHandler->szName, lpProcessInfo->szLine ) != 0 )
               {
                  lpDBHandler = zGETPTR( lpDBHandler->hNextDBHandler );
               }
            }

            // If DBHandler wasn't found, create the DBHandler.
            if ( lpDBHandler == 0 )
            {
               hndl = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                        sizeof( DBHandlerRecord ), 1, 0,
                                        iDBHandler );

               // Set info in DBHandler object.
               lpDBHandler = zGETPTR( hndl );
               if ( lpDBHandler )
               {
                  lpDBHandler->hNextDBHandler = lpProcessInfo->lpApp->hFirstDBHandler;
                  lpProcessInfo->lpApp->hFirstDBHandler = hndl;

                  strcpy_s( lpDBHandler->szName, sizeof( lpDBHandler->szName ), lpProcessInfo->szLine );
                  lpDBHandler->hApp = lpProcessInfo->lpApp;
               }
            }

            // If lpDBHandler is still not set, then set error.
            // Otherwise link lpViewOD to the handler.
            if ( lpDBHandler == 0 )
            {
               //  "KZOEE0xx - xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
               // fnIssueCoreError( lpTask, lpView, 16, 54, lLine, szFileName, 0 );
               lpProcessInfo->nCurrState = 10;  // set error
            }
            else
               lpViewOD->hDBHandler = zGETHNDL( lpDBHandler );

         }  // if ( zstrcmp( pchAttrName, "HANDLER" ) == 0 )...

         break;

      // Object Attributes beginning with L
      case 'L':
         if ( zstrcmp( pchAttrName, szlLOCK ) == 0 )
            lpViewOD->nLock = (zBYTE) zatol( lpProcessInfo->szLine );

         break;

      // Object Attributes beginning with M
      case 'M':
         break;

      // Object Attributes beginning with N
      case 'N':
         if ( zstrcmp( pchAttrName, szlNAME ) == 0 )
            strcpy_s( lpViewOD->szName, sizeof( lpViewOD->szName ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlNETWORK ) == 0 )
            strcpy_s( lpViewOD->szNetwork, sizeof( lpViewOD->szNetwork ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlNETCOMPRESS) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewOD->bNetCompress = TRUE;
         }

         break;

      // Object Attributes beginning with O
      case 'O':
         if ( zstrcmp( pchAttrName, "ORDERBY" ) == 0 )
            lpViewOD->bUseOrderBy = (lpProcessInfo->szLine[ 0 ] == 'Y');
#ifdef __LOAD_CONSTR__
         else
         if ( zstrcmp( pchAttrName, szlOPER_LIBNM ) == 0 )
            strcpy_s( lpViewOD->szOperLibname, sizeof( lpViewOD->szOperLibname ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlOCEOPER ) == 0 )
            strcpy_s( lpViewOD->szOCEOpername, sizeof( lpViewOD->szOCEOpername ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, "OCEOPERN" ) == 0 )
         {
            // OCEOPERN is no longer a valid attribute, but we keep it here to
            // avoid error messages.
         }
         else
         if ( zstrcmp( pchAttrName, szlOCACT ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewOD->bActivateConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlOCACTE ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewOD->bActivateEmptyConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlOCCOM ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewOD->bCommitConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlOCDROP ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewOD->bDropOIConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlOFBUFSZ ) == 0 )
         {
            lpViewOD->nOfBufSz = (zSHORT) zatol( lpProcessInfo->szLine );
         }
         else
         if ( zstrcmp( pchAttrName, szlOFRECID ) == 0 )
         {
            lpViewOD->nOfRecID = (zSHORT) zatol( lpProcessInfo->szLine );
         }
#endif
         break;

      // Object Attributes beginning with S
      case 'S':
         if ( zstrcmp( pchAttrName, "SironDB_Type" ) == 0 )
            lpViewOD->cSironDB_Type = lpProcessInfo->szLine[ 0 ];

         break;

   } // switch ( *pchAttrName )...

   return( 0 );

} // fnProcessObjectEntity

//./ ADD NAME=fnProcessEntityEntity
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnProcessEntityEntity
//
//  PURPOSE:    To process Entity information for an Object Def.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT
fnProcessEntityEntity( LPVIEWOD      lpViewOD,
                       LPVIEWENTITY  lpViewEntity,
                       LPPROCESSINFO lpProcessInfo )
{
   zPCHAR pchAttrName;

   pchAttrName = lpProcessInfo->szAttrName;
   switch ( pchAttrName[ 0 ] )
   {
      // Entity attributes beginning with 'A'
      case 'A':
         if ( zstrcmp( pchAttrName, "ABSTRACT" ) == 0 )
            lpViewEntity->bAbstract = TRUE;
         else
         if ( zstrcmp( pchAttrName, "ABSTRACTCHILD" ) == 0 )
            lpViewEntity->bAbstractChild = TRUE;
         else
         if ( zstrcmp( pchAttrName, "ACT_LIMIT" ) == 0 )
            lpViewEntity->lActivateLimit = zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlAUTOCREATE ) == 0 )
            lpViewEntity->bAutoCreate = TRUE;
         else
         if ( zstrcmp( pchAttrName, szlAUTOLOADFROMPARENT ) == 0 )
            lpViewEntity->bAutoLoadFromParent = TRUE;

         break;

      // Entity attributes beginning with 'C'
      case 'C':
         if ( zstrcmp( pchAttrName, szlCARDMAX ) == 0 )
            lpViewEntity->uCardMax =(zUSHORT) zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlCARDMIN ) == 0 )
         {
            lpViewEntity->uCardMin = (zUSHORT) zatol( lpProcessInfo->szLine );
            // If min cardinality is non-zero then mark
            // the parent as having a required child.
            if ( lpViewEntity->uCardMin && lpViewEntity->hParent )
            {
               LPVIEWENTITY lpParent = zGETPTR( lpViewEntity->hParent );

               lpParent->bRequiredChild = TRUE;
            }
         }
         else
         if ( zstrcmp( pchAttrName, szlCREATE ) == 0 )
            lpViewEntity->bCreate = TRUE;

         break;

      // Entity attributes beginning with 'D'
      case 'D':
         if ( zstrcmp( pchAttrName, szlDBNAME ) == 0 )
         {
            if ( zstrcmp( lpViewOD->szDfltDBName, lpProcessInfo->szLine ) == 0 )
               strcpy_s( lpViewEntity->szDBName, sizeof( lpViewEntity->szDBName ), lpViewOD->szDfltDBName );
            else
               strcpy_s( lpViewEntity->szDBName, sizeof( lpViewEntity->szDBName ), lpProcessInfo->szLine );
         }
         else
         if ( zstrcmp( pchAttrName, szlDELETE ) == 0 )
            lpViewEntity->bDelete = TRUE;
         else
         if ( zstrcmp( pchAttrName, szlDERIVED ) == 0 )
         {
            lpViewEntity->bDerived = TRUE;
            lpViewEntity->bDerivedPath = TRUE;
            lpViewOD->bHasDerivedPath = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlDUPENTIN ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
            {
               lpViewEntity->bDupInstance = TRUE;
               lpViewOD->bHasDupInstance = TRUE;
            }
         }
         else
         if ( zstrcmp( pchAttrName, szlDUPRELIN ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
            {
               lpViewEntity->bDupRel     = TRUE;
               lpViewEntity->bDupRelPath = TRUE;
               lpViewOD->bHasDupRel      = TRUE;
            }
         }

         break;

      // Entity attributes beginning with 'E'
      case 'E':

#ifdef __LOAD_CONSTR__
         if ( zstrcmp( pchAttrName, szlECACC ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewEntity->bAcceptConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlECCAN ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewEntity->bCancelConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlECCR ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewEntity->bCreateConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlECDEL ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewEntity->bDeleteConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlECEOPER ) == 0 )
            strcpy_s( lpViewEntity->szECEOpername, sizeof( lpViewEntity->szECEOpername ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, "ECEOPERN" ) == 0 )
         {
            // ECEOPERN is no longer a valid attribute, but we keep it here to
            // avoid error messages.
         }
         else
         if ( zstrcmp( pchAttrName, szlECEXC ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewEntity->bExcludeConstraint = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlECINC ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewEntity->bIncludeConstraint = TRUE;
         }
         else
#endif
         if ( zstrcmp( pchAttrName, szlERENT_TOK ) == 0 )
         {
            LPVIEWENTITY lpParentViewEntity;

            lpViewEntity->lEREntTok = zatol( lpProcessInfo->szLine );

            // See if the entity is structurally recursive
            for ( lpParentViewEntity = zGETPTR( lpViewEntity->hParent );
                  lpParentViewEntity;
                  lpParentViewEntity = zGETPTR( lpParentViewEntity->hParent ) )
            {
               if ( lpParentViewEntity->lEREntTok == lpViewEntity->lEREntTok )
               {
                  lpParentViewEntity->bRecursivePar = TRUE;
                  lpViewEntity->bRecursiveSt = TRUE;
                  break;
               }
            }
         }
         else
         if ( zstrcmp( pchAttrName, szlERREL_TOK ) == 0 )
            lpViewEntity->lERRelTok = zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlERREL_LINK ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == '1' )
               lpViewEntity->bRelLink1 = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlEXCLUDE ) == 0 )
            lpViewEntity->bExclude = TRUE;

         break;

      // Entity attributes beginning with 'F'
      case 'F':
         if ( zstrcmp( pchAttrName, szlFULLPERSIST ) == 0 )
            lpViewEntity->bFullPersist = TRUE;

         break;

      // Entity attributes beginning with 'H'
      case 'H':
         if ( zstrcmp( pchAttrName, szlHIDDEN ) == 0 )
            lpViewEntity->bHidden = TRUE;
         else
         if ( zstrcmp( pchAttrName, "HANG_FK" ) == 0 )
         {
            if ( toupper( lpProcessInfo->szLine[ 0 ] ) == 'Y' )
               lpViewEntity->bHangingFKey = TRUE;
         }

         break;

      // Entity attributes beginning with 'I'
      case 'I':
         if ( zstrcmp( pchAttrName, szlINCLSRC ) == 0 )
            lpViewEntity->bInclSrc = TRUE;
         else
         if ( zstrcmp( pchAttrName, szlINCLUDE ) == 0 )
            lpViewEntity->bInclude = TRUE;
         else
         if ( zstrcmp( pchAttrName, szlINCRLOAD ) == 0 )
            lpViewEntity->bIncrLoad = TRUE;

         break;

      case 'L':
         if ( zstrcmp( pchAttrName, szlLOCK ) == 0 )
         {
            lpViewEntity->nLock = (zBYTE) zatol( lpProcessInfo->szLine );

            // Keep track of highest entity locking level.
            if ( lpViewEntity->nLock > lpViewOD->nEntityLock )
               lpViewOD->nEntityLock = lpViewEntity->nLock;
         }

      // Entity attributes beginning with 'M'
      case 'M':
         if ( zstrcmp( pchAttrName, "MAX_LTH" ) == 0 )
            lpViewEntity->ulMaxLth = (zULONG) zatol( lpProcessInfo->szLine );

         break;

      // Entity attributes beginning with 'N'
      case 'N':
         if ( zstrcmp( pchAttrName, szlNAME ) == 0 )
            strcpy_s( lpViewEntity->szName, sizeof( lpViewEntity->szName ), lpProcessInfo->szLine );

         break;

      // Entity attributes beginning with 'P'
      case 'P':
         if ( zstrcmp( pchAttrName, szlPDELETE ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'D' )
               lpViewEntity->bPDelete = TRUE;
            else
            if ( lpProcessInfo->szLine[ 0 ] == 'R' )
            {
               LPVIEWENTITY lpParent = zGETPTR( lpViewEntity->hParent );

               lpViewEntity->bPRestrict = TRUE;
               lpParent->bCheckRestrict = TRUE;
            }
         }

         break;

      // Entity attributes beginning with 'R'
      case 'R':
         if ( zstrcmp( pchAttrName, szlRECURSIVE ) == 0 )
         {
            if ( lpViewEntity->bRecursiveSt )
               lpViewEntity->bRecursive = TRUE;
            else
            {
               LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
// "KZOEE063 - Entity attrib RECURSIVE Y, but not structurally recursive "
               fnIssueCoreError( lpTask, lpProcessInfo->lpView, 16, 63,
                                 lpProcessInfo->lLine,
                                 lpProcessInfo->szFileName,
                                 lpViewEntity->szName );
               lpProcessInfo->nCurrState = 10;  // set error
            }
         }

         break;

      // Entity attributes beginning with 'S'
      case 'S':
         if ( zstrcmp( pchAttrName, szlSERVER ) == 0 )
         {
            if ( zstrcmp( lpViewOD->szDfltServer, lpProcessInfo->szLine ) == 0 )
               strcpy_s( lpViewEntity->szServer, sizeof( lpViewEntity->szServer ), lpViewOD->szDfltServer );
            else
               strcpy_s( lpViewEntity->szServer, sizeof( lpViewEntity->szServer ), lpProcessInfo->szLine );
         }

         break;

      // Entity attributes beginning with 'U'
      case 'U':
         if ( zstrcmp( pchAttrName, szlUPDATE ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] != 'N' )
               lpViewEntity->bUpdate = TRUE;
         }

         break;

      // Entity attributes beginning with 'X'
      case 'X':
         if ( zstrcmp( pchAttrName, "XML_NAME" ) == 0 )
            strcpy_s( lpViewEntity->szXML_Name, sizeof( lpViewEntity->szXML_Name ), lpProcessInfo->szLine );

         break;

      // Skip other Entity attributes
      default:
         break;
   }

   return( 0 );
} // fnProcessEntityEntity

//./ ADD NAME=fnProcessAttribEntity
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnProcessAttribEntity
//
//  PURPOSE:    To process Entity information for an Object Def.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnProcessAttribEntity( zVIEW         lpView,
                       LPVIEWOD      lpViewOD,
                       LPVIEWENTITY  lpViewEntity,
                       LPVIEWATTRIB  lpViewAttrib,
                       LPPROCESSINFO lpProcessInfo )
{
   zPCHAR pchAttrName;

   pchAttrName = lpProcessInfo->szAttrName;
   switch ( pchAttrName[ 0 ] )
   {
      // Attrib attributes beginning with 'A'
      case 'A':
         if ( zstrcmp( pchAttrName, szlAPDM_TOK ) == 0 )
         {
            zLONG    lSearchTok;
            LPDOMAIN lpDomain;

            if ( lpProcessInfo->bLth || lpProcessInfo->bType )
            {
               LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
               //  "KZOEE055 - Invalid file, LTH or TYPE before DOMAIN on line"
               fnIssueCoreError( lpTask, lpView, 16, 55,
                                 lpProcessInfo->lLine,
                                 lpProcessInfo->szFileName, 0 );
               lpProcessInfo->nCurrState = 10;  // set error
               break;
            }

            lpProcessInfo->bDomain = TRUE;
            lSearchTok = zatol( lpProcessInfo->szLine );

            for ( lpDomain = zGETPTR( lpProcessInfo->lpApp->hFirstDomain );
                  lpDomain;
                  lpDomain = zGETPTR( lpDomain->hNextDomain ) )
            {
               if ( lpDomain->lApDmTok == lSearchTok )
                  break;
            }

            if ( lpDomain )
            {
               zBOOL bErr = 0;

               lpViewAttrib->hDomain = zGETHNDL( lpDomain );
               lpViewAttrib->cDecimal = lpDomain->cDecimal;
               lpViewAttrib->cType = lpDomain->cType;
               switch ( lpDomain->cType )
               {
                  case zTYPE_STRING:
                     if ( lpDomain->ulMaxStringLth )
                        lpViewAttrib->ulLth = lpDomain->ulMaxStringLth;
                     else
                        lpViewAttrib->ulLth = 254;

                     break;

                  case zTYPE_INTEGER:
                     lpViewAttrib->ulLth = sizeof( zLONG );
                     break;

                  case zTYPE_DECIMAL:
                     lpViewAttrib->ulLth = sizeof( zDECIMAL );
                     break;

                  case zTYPE_DATETIME:
                     lpViewAttrib->ulLth = sizeof( zLONG ) + sizeof( zSHORT );
                     break;

                  case zTYPE_BLOB:
                     lpViewEntity->bContainsBlob = TRUE;

                     lpViewAttrib->ulLth = sizeof( zPCHAR ) + sizeof( zSHORT ) + sizeof( zCHAR );
                     break;

                  default:
                  {
                     LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
                     // "KZOEE246 - Invalid Attribute type "
                     fnIssueCoreError( lpTask, lpView, 16, 246, 0,
                                       lpViewEntity->szName,
                                       lpViewAttrib->szName );
                     bErr = TRUE;
                     lpProcessInfo->nCurrState = 10;  // set error
                     break;
                  }
               }

               if ( bErr )
                  break;
            }
            else
            {
               // DGC 3-7-96
               // The following 'if' was put in so that when testing DBHandlers
               // we can tell core to ignore domain errors if the trace level
               // is 2 or greater.  The 'if' statement can be taken out if it
               // proves to cause problems.
               if ( GetTaskDBHandlerTraceLevel( lpProcessInfo->lpView ) < 2 )
               {
                  LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
                  //  "KZOEE056 - Domain not found on line "
                  fnIssueCoreError( lpTask, lpView, 16, 56,
                                    lpProcessInfo->lLine,
                                    lpProcessInfo->szLine,
                                    lpProcessInfo->szFileName );
               }
            }
         }
         else
         if ( zstrcmp( pchAttrName, szlAUTO_SEQ ) == 0 )
         {
            lpViewOD->bAutoSeq = TRUE;
            lpViewEntity->bAutoSeq = TRUE;
            lpViewAttrib->bAutoSeq = TRUE;
         }

         break;

      // Attrib attributes beginning with 'C'
      case 'C':
         if ( zstrcmp( pchAttrName, szlCASESENS ) == 0 )
            lpViewAttrib->bCaseSens = TRUE;
         else
         if ( zstrcmp( pchAttrName, szlCR_DATE ) == 0 )
            lpViewAttrib->bCrDate = TRUE;

         break;

      // Attrib attributes beginning with 'D'
      case 'D':
         if ( zstrcmp( pchAttrName, "DB_OPER" ) == 0 )
         {
            lpViewEntity->bHasDB_Oper = TRUE;
            lpViewAttrib->cDB_Oper    = lpProcessInfo->szLine[ 0 ];

            // If entity contains a db operation, then that entity is read-only.
            lpViewEntity->bCreate     =
               lpViewEntity->bDelete  =
               lpViewEntity->bUpdate  =
               lpViewEntity->bInclude =
               lpViewEntity->bExclude =
               lpViewEntity->bInclSrc = 0;
         }
         else
         if ( zstrcmp( pchAttrName, "DERIVEDE" ) == 0 )
         {
            lpViewAttrib->hDerivedExpr =
               fnStoreStringInDataspace( AnchorBlock->hMainFirstDataHeader,
                                         lpProcessInfo->szLine );
         }
         else
         if ( zstrcmp( pchAttrName, szlDERIVEDF ) == 0 )
            strcpy_s( lpViewAttrib->szDerivedOper, sizeof( lpViewAttrib->szDerivedOper ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, "DERIVEDOPERXPG" ) == 0 )
         {
            zULONG ulLth;
            zPCHAR ptr;
            zPCHAR pch;

            ulLth = zatol( lpProcessInfo->szLine );
            lpViewAttrib->hDerivedXPG = (zPCHAR)
                            fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                              ulLth, 1, 0, 0 );
            ptr = (zPCHAR) zGETPTR( lpViewAttrib->hDerivedXPG );
            SysReadLineLth( lpView, &pch, lpProcessInfo->hFile, ulLth );
            zmemcpy( ptr, pch, ulLth );
         }
         else
         if ( zstrcmp( pchAttrName, "DERIVEDZ" ) == 0 )
            lpViewAttrib->bDerivedIntrnl = TRUE;
         else
         if ( lpViewAttrib->hDomain == 0 &&
              zstrcmp( pchAttrName, szlDECIMAL ) == 0 )
         {
            lpViewAttrib->cDecimal = (zCHAR) zatol( lpProcessInfo->szLine );
         }
         else
         if ( zstrcmp( pchAttrName, "DEBUGCHG" ) == 0 )
         {
            lpViewEntity->bDebugChange = TRUE;
            lpViewAttrib->bDebugChange = TRUE;
         }

         break;

      // Attrib attributes beginning with 'E'
      case 'E':
         if ( zstrcmp( pchAttrName, szlERATT_TOK ) == 0 )
            lpViewAttrib->lERAttTok  = zatol( lpProcessInfo->szLine );

         break;

      // Attrib attributes beginning with 'F'
      case 'F':
         if ( zstrcmp( pchAttrName, "FORKEY" ) == 0 )
            lpViewAttrib->bForeignKey = TRUE;

         break;

      // Attrib attributes beginning with 'G'
      case 'G':
         if ( zstrcmp( pchAttrName, szlGENKEY ) == 0 )
         {
            lpViewAttrib->bGenkey = TRUE;
            lpViewEntity->bGenkey = TRUE;
            lpViewOD->bGenkey     = TRUE;
         }

         break;

      // Attrib attributes beginning with 'H'
      case 'H':
         if ( zstrcmp( pchAttrName, szlHIDDEN ) == 0 )
         {
            lpViewAttrib->bHidden = TRUE;
         }

         break;

      // Attrib attributes beginning with 'I'
      case 'I':
         if ( zstrcmp( pchAttrName, szlINIT ) == 0 )
         {
            lpViewAttrib->hInit = fnStoreStringInDataspace
                  ( AnchorBlock->hMainFirstDataHeader, lpProcessInfo->szLine );
            lpViewEntity->bAttrInit = TRUE;
         }

         break;

      // Attrib attributes beginning with 'K'
      case 'K':
         if ( zstrcmp( pchAttrName, szlKEY ) == 0 )
         {
            lpProcessInfo->nKeyCnt++;   // dks ... 2006.01.25
            lpViewAttrib->bKey = TRUE;
            lpViewAttrib->bNoPersistUpd = TRUE;
         }

         break;

      // Attrib attributes beginning with 'L'
      case 'L':
         if ( zstrcmp( pchAttrName, szlLTH ) == 0 )
         {
            LPVIEWATTRIB lpSearchAttrib;
            LPVIEWATTRIB lpHoldAttrib;
            zLONG        lSearchTok;
            zULONG       ulOffsetIncrement;

            if ( lpProcessInfo->bType == FALSE &&
                 lpProcessInfo->bDomain == FALSE )
            {
               LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
               //  "KZOEE057 - Invalid file, LTH before type
               //              and DOMAIN on line "
               fnIssueCoreError( lpTask, lpView, 16, 57,
                                 lpProcessInfo->lLine,
                                 lpProcessInfo->szFileName, 0 );
               lpProcessInfo->nCurrState = 10;  // set error
               break;
            }

            lpProcessInfo->bLth = TRUE;
            lpViewAttrib->ulLth = zatol( lpProcessInfo->szLine );

            // We're going to explicitly change the length in some cases.
            switch ( lpViewAttrib->cType )
            {
               case zTYPE_INTEGER:
                  lpViewAttrib->ulLth = sizeof( zLONG );
                  ulOffsetIncrement = lpViewAttrib->ulLth;
                  break;

               case zTYPE_DECIMAL:
                  lpViewAttrib->ulLth = sizeof( zDECIMAL );
                  ulOffsetIncrement = lpViewAttrib->ulLth;
                  break;

               case zTYPE_BLOB:
               case zTYPE_STRING:
                  if ( lpViewAttrib->cType == zTYPE_BLOB )
                     lpViewAttrib->ulLth = 1 + sizeof( zPVOID ) + sizeof( zULONG );
                  else
                     lpViewAttrib->ulLth++;

                  // Determine offset increment
                  if ( lpViewAttrib->ulLth > g_ulMaxRecordStringLth )
                     ulOffsetIncrement = g_ulMaxRecordStringLth;
                  else
                     ulOffsetIncrement = lpViewAttrib->ulLth;

                  break;

               default:
                  ulOffsetIncrement = lpViewAttrib->ulLth;
                  break;
            }

            // Add room for attribute flags.
            ulOffsetIncrement += sizeof( AttribFlagsRecord );

            if ( lpViewAttrib->bPersist )
            {
               lpViewAttrib->ulRecordOffset = lpProcessInfo->ulRecordOffset;

               // Now that we have determined a length, set the
               // offset of the attribute in token order, First
               // look for the first attribute with a token
               // greater than ours. Record offsets are kept
               // in token order so entity linking can take
               // place from multiple objects and attributes
               // can be correctly interpreted by all objects.

               lpHoldAttrib   = 0;
               lSearchTok     = 0;
               for ( lpSearchAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
                     lpSearchAttrib;
                     lpSearchAttrib = zGETPTR( lpSearchAttrib->hNextOD_Attrib ) )
               {
                  if ( lpSearchAttrib->bPersist &&
                       lpSearchAttrib->lERAttTok > lpViewAttrib->lERAttTok &&
                       (lSearchTok == 0 ||
                        lSearchTok > lpSearchAttrib->lERAttTok) )
                  {
                     lSearchTok = lpSearchAttrib->lERAttTok;
                     lpHoldAttrib = lpSearchAttrib;
                  }
               }

               // if an attribute was found, go through the
               // attributes again and add the offset
               // increment to all attributes with tokens
               // greater than the current attributes token
               if ( lpHoldAttrib )
               {
                  lpViewAttrib->ulRecordOffset = lpHoldAttrib->ulRecordOffset;
                  lpSearchAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
                  while ( lpSearchAttrib )
                  {
                     if ( lpSearchAttrib->bPersist &&
                          lpSearchAttrib->lERAttTok > lpViewAttrib->lERAttTok )
                     {
                        lpSearchAttrib->ulRecordOffset += ulOffsetIncrement;
                     }

                     lpSearchAttrib = zGETPTR( lpSearchAttrib->hNextOD_Attrib );
                  }
               }

               // Finally, increment the offset increment value
               lpProcessInfo->ulRecordOffset += ulOffsetIncrement;
               lpViewEntity->ulRecordSize = lpProcessInfo->ulRecordOffset;

            }  // end of persistent attribute stuff
            else
            {
               lpViewAttrib->ulRecordOffset = lpProcessInfo->ulNonPersistOffset;
               lpProcessInfo->ulNonPersistOffset += ulOffsetIncrement;
               lpViewEntity->ulNonPersistSize = lpProcessInfo->ulNonPersistOffset;
            }
         }

         break;

      // Attrib attributes beginning with 'N'
      case 'N':
         if ( zstrcmp( pchAttrName, szlNAME ) == 0 )
            strcpy_s( lpViewAttrib->szName, sizeof( lpViewAttrib->szName ), lpProcessInfo->szLine );

         break;

      // Attrib attributes beginning with 'P'
      case 'P':
         if ( zstrcmp( pchAttrName, szlPERSIST ) == 0 )
         {
            if ( lpProcessInfo->bType || lpProcessInfo->bLth || lpProcessInfo->bDomain )
            {
               LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
               //  "KZOEE058 - Invalid file, PERSIST after
               //              DOMAIN, TYPE or LTH on line "
               fnIssueCoreError( lpTask, lpView, 16, 58,
                                 lpProcessInfo->lLine,
                                 lpProcessInfo->szFileName, 0 );
               lpProcessInfo->nCurrState = 10;  // set error
               break;
            }

            lpProcessInfo->bPersist = TRUE;
            lpViewAttrib->bPersist = TRUE;
         }

         break;

      // Attrib attributes beginning with 'R'
      case 'R':
         if ( zstrcmp( pchAttrName, szlREQUIRED ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'Y' )
               lpViewAttrib->bRequired = TRUE;
         }

         break;

      // Attrib attributes beginning with 'S'
      case 'S':
         if ( zstrcmp( pchAttrName, szlSEQUENCING ) == 0 )
         {
            lpViewAttrib->cSequencing = (zUCHAR) zatol( lpProcessInfo->szLine );

            // Set flag to indicate that the OD has ordering.
            lpViewOD->bAutoSeq = TRUE;

            // Indicate that the owning entity has at least 1
            // attribute that is used for ordering.
            if ( lpViewEntity->hParent == 0 || lpViewEntity->uCardMax > 1 )
               lpViewEntity->bAttrOrder = TRUE;
            else
            {
               LPVIEWENTITY lpSearchViewEntity;

               // The current entity's relationship with its parent is 1-1.
               // This means that the ordering must be for a parent entity--
               // we don't order 1 entity.  Find the parent entity.
               for ( lpSearchViewEntity = zGETPTR( lpViewEntity->hParent );
                     lpSearchViewEntity->uCardMax <= 1;
                     lpSearchViewEntity = zGETPTR( lpSearchViewEntity->hParent ) )
               {
                  // If there is no parent then we're done.
                  if ( lpSearchViewEntity->hParent == 0 )
                     break;
               }

               lpSearchViewEntity->bAttrOrder =
                  lpSearchViewEntity->bAttrOrderChild = TRUE;
            }
         }
         else
         if ( zstrcmp( pchAttrName, szlSEQ_AD ) == 0 )
         {
            if ( lpProcessInfo->szLine[ 0 ] == 'D' )
               lpViewAttrib->bSequencingD = TRUE;
         }
         break;

      // Attrib attributes beginning with 'T'
      case 'T':
         if ( zstrcmp( pchAttrName, szlTYPE ) == 0 )
         {
            if ( lpViewAttrib->hDomain == 0 )
            {
               lpProcessInfo->bType = TRUE;
               lpViewAttrib->cType = lpProcessInfo->szLine[ 0 ];
            }
            else
            {
               LPDOMAIN lpDomain = zGETPTR( lpViewAttrib->hDomain );

               if ( lpDomain->cType != lpProcessInfo->szLine[ 0 ] )
               {
                  LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
                  zCHAR szTempMsg[ 40 ];

                  // "KZOEE062 - Conflict between E/R Attribute type and TE Field Type: "
                  strcpy_s( szTempMsg, sizeof( szTempMsg ), lpViewOD->szName );
                  strcat_s( szTempMsg, sizeof( szTempMsg ), ", " );
                  strcat_s( szTempMsg, sizeof( szTempMsg ), lpViewEntity->szName );
                  fnIssueCoreError( lpTask, lpView, 16, 62,
                                    0, szTempMsg,
                                    lpViewAttrib->szName );
                  lpProcessInfo->nCurrState = 10;  // set error
                  break;
               }
               else
                  lpProcessInfo->bType = TRUE;
            }
         }

         break;

      // Attrib attributes beginning with 'U'
      case 'U':
         if ( zstrcmp( pchAttrName, szlUP_DATE ) == 0 )
         {
            lpViewAttrib->bUpDate = TRUE;
         }
         else
         if ( zstrcmp( pchAttrName, szlUSERID ) == 0 )
         {
            lpViewAttrib->bUserID = TRUE;
         }

         break;

      case 'X':
         if ( zstrcmp( pchAttrName, "XML_SIMPLE_NAME" ) == 0 )
            strcpy_s( lpViewAttrib->szXML_SimpleName, sizeof( lpViewAttrib->szXML_SimpleName ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, "XML_NAME_EXTERNAL" ) == 0 )
            strcpy_s( lpViewAttrib->szXML_ExternalName, sizeof( lpViewAttrib->szXML_ExternalName ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlXVAATT_TOK ) == 0 )
            lpViewAttrib->lXVAAttTok = zatol( lpProcessInfo->szLine );

         break;

      default:
         break;
   }   // End Attribute Attribute switch

   return( 0 );
} // fnProcessAttribEntity

//./ ADD NAME=fnProcessDataRecordEntity
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnProcessDataRecordEntity
//
//  PURPOSE:    To process DataRecord information for an Object Def.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT
fnProcessDataRecordEntity( LPVIEWOD      lpViewOD,
                           LPVIEWENTITY  lpViewEntity,
                           LPDATARECORD  lpDataRecord,
                           LPPROCESSINFO lpProcessInfo )
{
   zPCHAR pchAttrName;

   pchAttrName = lpProcessInfo->szAttrName;
   switch ( pchAttrName[ 0 ] )
   {
      // Data Record attributes beginning with 'J'
      case 'J':
         if ( zstrcmp( pchAttrName, "JOIN" ) == 0 )
            lpDataRecord->cJoin = lpProcessInfo->szLine[ 0 ];

         break;

      // Data Record attributes beginning with 'R'
      case 'R':
         if ( zstrcmp( pchAttrName, szlRECID ) == 0 )
            lpDataRecord->lRecordID = zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlRECNAME ) == 0 )
            strcpy_s( lpDataRecord->szRecordName, sizeof( lpDataRecord->szRecordName ), lpProcessInfo->szLine );

         break;

      // Data Record attributes beginning with 'T'
      case 'T':
         if ( zstrcmp( pchAttrName, szlTYPE ) == 0 )
         {
            // Type is 'P' primary, 'S' - Secondary
            lpDataRecord->cRecordType = lpProcessInfo->szLine[ 0 ];
         }

         break;

      default:
         break;

   }   // End DataRecord Attribute switch

   return( 0 );
} // fnProcessDataRecordEntity

//./ ADD NAME=fnProcessDataFieldEntity
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnProcessDataFieldEntity
//
//  PURPOSE:    To process DataField information for an Object Def.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnProcessDataFieldEntity( LPVIEWOD      lpViewOD,
                          LPVIEWENTITY  lpViewEntity,
                          LPDATARECORD  lpDataRecord,
                          LPDATAFIELD   lpDataField,
                          LPPROCESSINFO lpProcessInfo )
{
   zPCHAR pchAttrName;

   pchAttrName = lpProcessInfo->szAttrName;
   switch ( pchAttrName[ 0 ] )
   {
      // Data Field attributes beginning with 'F'
      case 'F':
         if ( zstrcmp( pchAttrName, szlFLDID ) == 0 )
            lpDataField->lFldID = zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlFLDLTH ) == 0 )
            lpDataField->uFldLth = (zUSHORT) zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlFLDNAME ) == 0 )
            strcpy_s( lpDataField->szFldName, sizeof( lpDataField->szFldName ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlFLDOFFSET ) == 0 )
            lpDataField->uFldOffset = (zUSHORT) zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlFLDTYPE ) == 0 )
            lpDataField->cFldType = lpProcessInfo->szLine[ 0 ];

         break;

      // Data Field attributes beginning with 'X'
      case 'X':
         if ( zstrcmp( pchAttrName, szlXVAATT_TOK ) == 0 )
         {
            zLONG         lSearchTok;
            LPVIEWATTRIB  lpSearchViewAttrib;

            // Get Token value to search for
            lSearchTok = zatol( lpProcessInfo->szLine );

            // Search through the Current Entity to find
            // the attribute owning this relationship field
            for ( lpSearchViewAttrib =
                              zGETPTR( lpViewEntity->hFirstOD_Attrib );
                  lpSearchViewAttrib;
                  lpSearchViewAttrib =
                              zGETPTR( lpSearchViewAttrib->hNextOD_Attrib ) )
            {
               if ( lpSearchViewAttrib->lXVAAttTok == lSearchTok )
                  break;
            }

            if ( lpSearchViewAttrib )
               lpDataField->hViewAttrib = zGETHNDL( lpSearchViewAttrib );
            else
            {
               LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
               // If the attribute was not found, issue Error
               // "KZOEE059 - Invalid file, Invalid Attribute
               //             Token located on line "
               fnIssueCoreError( lpTask, lpProcessInfo->lpView, 16, 59,
                                 lpProcessInfo->lLine,
                                 lpProcessInfo->szFileName, 0 );
            }
         }
         break;

      default:
         break;

   }   // End Data Field Attribute switch

   return( 0 );
} // fnProcessDataFieldEntity

//./ ADD NAME=fnProcessRelRecordEntity
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnProcessRelRecordEntity
//
//  PURPOSE:    To process RelRecord information for an Object Def.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zSHORT
fnProcessRelRecordEntity( LPVIEWOD      lpViewOD,
                          LPVIEWENTITY  lpViewEntity,
                          LPDATARECORD  lpDataRecord,
                          LPRELRECORD   lpRelRecord,
                          LPPROCESSINFO lpProcessInfo )
{
   zPCHAR pchAttrName;

   pchAttrName = lpProcessInfo->szAttrName;
   switch ( pchAttrName[ 0 ] )
   {
      // Rel Record attributes beginning with 'O'
      case 'O':
         if ( zstrcmp( pchAttrName, szlOWNER_MEMB ) == 0 )
            lpRelRecord->cOwnerMember = lpProcessInfo->szLine[ 0 ];

         break;

      // Rel Record attributes beginning with 'R'
      case 'R':
         if ( zstrcmp( pchAttrName, szlRECID ) == 0 )
            lpRelRecord->lRecordID = zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlRECNAME ) == 0 )
            strcpy_s( lpRelRecord->szRecordName, sizeof( lpRelRecord->szRecordName ), lpProcessInfo->szLine );

         break;

      // Rel Record attributes beginning with 'S'
      case 'S':
         if ( zstrcmp( pchAttrName, szlSETID1 ) == 0 )
            lpRelRecord->lSetID1 = zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlSETID2 ) == 0 )
            lpRelRecord->lSetID2 = zatol( lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlSETNAME1 ) == 0 )
            strcpy_s( lpRelRecord->szSetName1, sizeof( lpRelRecord->szSetName1 ), lpProcessInfo->szLine );
         else
         if ( zstrcmp( pchAttrName, szlSETNAME2 ) == 0 )
            strcpy_s( lpRelRecord->szSetName2, sizeof( lpRelRecord->szSetName2 ), lpProcessInfo->szLine );
         break;

      default:
         break;

   }   // End Rel Record Attribute switch

   return( 0 );
} // fnProcessRelRecordEntity

//./ ADD NAME=fnProcessRelFieldEntity
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnProcessRelFieldEntity
//
//  PURPOSE:    To process RelField information for an Object Def.
//
//  RETURNS:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT
fnProcessRelFieldEntity( LPVIEWOD      lpViewOD,
                         LPVIEWENTITY  lpViewEntity,
                         LPDATARECORD  lpDataRecord,
                         LPRELRECORD   lpRelRecord,
                         LPRELFIELD    lpRelField,
                         LPPROCESSINFO lpProcessInfo )
{
   zPCHAR pchAttrName;

   pchAttrName = lpProcessInfo->szAttrName;
   switch ( pchAttrName[ 0 ] )
   {
      // Rel Field attributes beginning with 'F'
      case 'F':
         if ( zstrcmp( pchAttrName, szlFLDNAME ) == 0 )
            strcpy_s( lpRelField->szFldName, sizeof( lpRelField->szFldName ), lpProcessInfo->szLine );

         break;

      // Rel Field attributes beginning with 'R' or 'S'
      case 'R':
      case 'S':
         if ( zstrcmp( pchAttrName, szlRELXVAATT_TOK ) == 0 ||
              zstrcmp( pchAttrName, szlSRCXVAATT_TOK ) == 0 )
         {
            LPVIEWENTITY  lpSearchViewEntity;
            LPDATAFIELD   lpSearchDataField;
            LPDATARECORD  lpFirstDataRecord;
            LPVIEWATTRIB  lpViewAttrib;
            zLONG         lSearchTok;

            // Get Token value to search for.
            lSearchTok = zatol( lpProcessInfo->szLine );

            // Search through the Current Entity and Entities up the
            // parent chain to find the data field corresponding to the
            // view attribute matching the xva token.
            lpSearchViewEntity = lpViewEntity;
            while ( lpSearchViewEntity )
            {
               lpFirstDataRecord = zGETPTR( lpSearchViewEntity->hFirstDataRecord );
               for ( lpSearchDataField =
                           zGETPTR( lpFirstDataRecord->hFirstDataField );
                     lpSearchDataField;
                     lpSearchDataField =
                           zGETPTR( lpSearchDataField->hNextDataField ) )
               {
                  lpViewAttrib = zGETPTR( lpSearchDataField->hViewAttrib );
                  if ( lpViewAttrib->lXVAAttTok == lSearchTok )
                     break;
               }

               if ( lpSearchDataField )
                  break;

               lpSearchViewEntity = zGETPTR( lpSearchViewEntity->hParent );
            }

            if ( lpSearchDataField )
            {
               if ( zstrcmp( pchAttrName, szlRELXVAATT_TOK ) == 0 )
                  lpRelField->hRelDataField = zGETHNDL( lpSearchDataField );
               else
                  lpRelField->hSrcDataField = zGETHNDL( lpSearchDataField );
            }
            else
            {
               LPTASK lpTask = zGETPTR( lpProcessInfo->lpView->hTask );
               // If the attribute was not found, issue Error
               // "KZOEE059 - Invalid file, Invalid Attribute
               //             Token located on line "
               fnIssueCoreError( lpTask, lpProcessInfo->lpView, 16, 59,
                                 lpProcessInfo->lLine,
                                 lpProcessInfo->szFileName, 0 );
            }
         }

         break;

      default:
         break;

   }   // End Rel Field Attribute switch

   return( 0 );
} // fnProcessRelFieldEntity

/*

   Make sure that we can reload the ViewOD; i.e. make sure that no views
   currently reference the view OD.

   Returns: 0 : ViewOD not in use.
            zCALL_ERROR : in use.

*/
zSHORT LOCALOPER
fnCheckIfViewOD_IsUsed( zVIEW zView, LPVIEWOD lpViewOD )
{
   LPTASK   lpTask;
   zVIEW    lpView;
   LPAPP    lpApp   = zGETPTR( lpViewOD->hApp );
   LPVIEWOD hViewOD = zGETHNDL( lpViewOD );
   zBOOL    bBrowseStarted = FALSE;
   zSHORT   nRC = zCALL_ERROR;

   // Go through the app and make sure there are no views referencing lpViewOD.
   for ( lpView = zGETPTR( lpApp->hFirstView );
         lpView;
         lpView = zGETPTR( lpView->hNextView ) )
   {
      if ( lpView->hViewOD == hViewOD )
         goto EndOfFunction;
   }

   fnStartBrowseOfTaskList( zView->hTask, FALSE );
   bBrowseStarted = TRUE;

   for ( lpTask = zGETPTR( AnchorBlock->hFirstTask );
         lpTask;
         lpTask = zGETPTR( lpTask->hNextTask ) )
   {
      for ( lpView = zGETPTR( lpTask->hFirstView );
            lpView;
            lpView = zGETPTR( lpView->hNextView ) )
      {
         if ( lpView->hViewOD == hViewOD )
            goto EndOfFunction;
      }
   }

   // If we get here then everything's OK.
   nRC = 0;

EndOfFunction:
   if ( bBrowseStarted )
      fnEndBrowseOfTaskList( FALSE );

   return( nRC );
}

void
DisplayObjectDefinition( LPVIEWOD lpViewOD )
{
   LPVIEWENTITY   lpViewEntity;
   LPVIEWATTRIB   lpViewAttrib;
   LPVIEWATTRIB   lpViewAttrib1;
   LPVIEWATTRIB   lpViewAttrib2;
   LPDATARECORD   lpDataRecord;
   LPRELRECORD    lpRelRecord;
   LPDATAFIELD    lpDataField;
   LPDATAFIELD    lpDataField2;
   LPRELFIELD     lpRelField;
   LPAPP  lpApp = zGETPTR( lpViewOD->hApp );
   zCHAR  szIndent[ 256 ];
   zSHORT nLth = 0;

   zmemset( szIndent, ' ', sizeof( szIndent ) - 1 );
   TraceLineS( "", "" );
   TraceLine( "Object Definition: %s  for App: %s", lpViewOD->szName, lpApp->szName );

   lpViewEntity = zGETPTR( lpViewOD->hFirstOD_Entity );
   while ( lpViewEntity )
   {
      // Set up 4 character indentation based on Entity Level.
      szIndent[ nLth ] = ' ';
      nLth = (lpViewEntity->nLevel - 1) * 4;
      szIndent[ nLth ] = 0;

      TraceLine( "%se%s  PDelete: %s  EREntTok: 0x%08x  ERRelTok: 0x%08x",
                 szIndent, lpViewEntity->szName,
                 lpViewEntity->bPDelete ? "Y" : "N",
                 lpViewEntity->lEREntTok,
                 lpViewEntity->lERRelTok );

      lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
      while ( lpViewAttrib )
      {
         if ( zGETPTR( lpViewAttrib->hViewEntity ) != lpViewEntity )
            TraceLineS( "Attribute for entity NOT parented by Entity: ", lpViewEntity->szName );

         TraceLine( "%s  a%s  Key: %s%s  ERAttTok: 0x%08x  XVAAttTok: 0x%08x",
                    szIndent, lpViewAttrib->szName,
                    lpViewAttrib->bKey ? "Y" : "N",
                    lpViewAttrib->bForeignKey ? "  ForeignKey" : "  ",
                    lpViewAttrib->lERAttTok,
                    lpViewAttrib->lXVAAttTok );

         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
      }

      lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );
      while ( lpDataRecord )
      {
         TraceLine( "%s  r%s  RecordID: 0x%08x  PrimarySecondary: %c  Join: %s",
                    szIndent, lpDataRecord->szRecordName,
                    lpDataRecord->lRecordID, lpDataRecord->cRecordType,
                    lpDataRecord->cJoin ? "Y" : "N" );

         lpRelRecord = zGETPTR( lpDataRecord->hFirstRelRecord );
         while ( lpRelRecord )
         {
            TraceLine( "%s    rr%s  RecordID: 0x%08x  OwnerMember: %c   Set1: %s 0x%08x   Set2: %s 0x%08x",
                       szIndent, lpRelRecord->szRecordName,
                       lpRelRecord->lRecordID,
                       lpRelRecord->cOwnerMember,
                       lpRelRecord->szSetName1,
                       lpRelRecord->lSetID1,
                       lpRelRecord->szSetName2,
                       lpRelRecord->lSetID2 );

            lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
            while ( lpRelField )
            {
               lpViewAttrib = zGETPTR( lpRelField->hViewAttrib );
               lpDataField = zGETPTR( lpRelField->hSrcDataField );
               lpDataField2 = zGETPTR( lpRelField->hRelDataField );
               lpViewAttrib1 = zGETPTR( lpDataField->hViewAttrib );
               lpViewAttrib2 = zGETPTR( lpDataField2->hViewAttrib );

               TraceLine( "%s      rf%s  [Attrib Name: %s  ERAttTok: 0x%08x  XVAAttTok: 0x%08x]"
                            "  SrcDataField: %s 0x%08x [Attrib: %s  ERAttTok: 0x%08x  XVAAttTok: 0x%08x]"
                            "  RelDataField: %s 0x%08x [Attrib: %s  ERAttTok: 0x%08x  XVAAttTok: 0x%08x]",
                          szIndent, lpRelField->szFldName,
                          lpViewAttrib ? lpViewAttrib->szName : "<null>",
                          lpViewAttrib ? lpViewAttrib->lERAttTok : 0,
                          lpViewAttrib ? lpViewAttrib->lXVAAttTok : 0,
                          lpDataField->szFldName,
                          lpDataField->lFldID,
                          lpViewAttrib1 ? lpViewAttrib1->szName : "<null>",
                          lpViewAttrib1 ? lpViewAttrib1->lERAttTok : 0,
                          lpViewAttrib1 ? lpViewAttrib1->lXVAAttTok : 0,
                          lpDataField2->szFldName,
                          lpDataField2->lFldID,
                          lpViewAttrib2 ? lpViewAttrib2->szName : "<null>",
                          lpViewAttrib2 ? lpViewAttrib2->lERAttTok : 0,
                          lpViewAttrib2 ? lpViewAttrib2->lXVAAttTok : 0 );

               lpRelField = zGETPTR( lpRelField->hNextRelField );
            }

            lpRelRecord = zGETPTR( lpRelRecord->hNextRelRecord );
         }

         lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
         while ( lpDataField )
         {
            lpViewAttrib = zGETPTR( lpDataField->hViewAttrib );
            TraceLine( "%s    df%s  FldID: 0x%08x  [Attrib Name: %s  ERAttTok: 0x%08x  XVAAttTok: 0x%08x]",
                       szIndent, lpDataField->szFldName,
                       lpDataField->lFldID,
                       lpViewAttrib->szName,
                       lpViewAttrib->lERAttTok,
                       lpViewAttrib->lXVAAttTok );

            lpDataField = zGETPTR( lpDataField->hNextDataField );
         }

         lpDataRecord = zGETPTR( lpDataRecord->hNextDataRecord );
      }

      lpViewEntity = zGETPTR( lpViewEntity->hNextHier );
   }

   TraceLineS( "", "" );
}

//./ ADD NAME=ActivateViewObject
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ActivateViewObject
//
//  PURPOSE:    To activate a View Object for an application
//
//  RETURNS:    LPVIEWOD - pointer to object actived
//              0 - Error Activating object definition
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
LPVIEWOD OPERATION
ActivateViewObject( zVIEW    lpView,
                    zCPCHAR  cpcViewOD_Name,
                    zBOOL    bSystemObject )
{
   ProcessInfoRecord ProcessInfo;
   LPVIEWENTITY   lpViewEntity;
   LPVIEWATTRIB   lpViewAttrib;
   LPDATARECORD   lpDataRecord;
   LPDATAFIELD    lpDataField;
   LPRELRECORD    lpRelRecord;
   LPRELFIELD     lpRelField;
   LPTASK         lpTask;
   LPAPP          lpApp;
   LPVIEWOD       lpViewOD;
   zPVOID         hApp;
   zPVOID         hViewOD;
   zPVOID         hViewEntity;
   zPVOID         hndl;
   zLONG          hFile;
   zSHORT         nEOF;
   zSHORT         nHierNbr;

// if ( zstrcmpi( cpcViewOD_Name, "TZ__PRFO" ) == 0 )
// {
//    nEOF = 0;
//    nEOF /= nEOF;
// }

   // First, make sure Anchor Block exists, if not return 0.
   if ( AnchorBlock == 0 )
      return( 0 );

   // Now, check that task is active
   lpTask = zGETPTR( lpView->hTask );
   if ( lpTask == 0 )
      return( 0 ); // if not, error

   if ( bSystemObject )
      hApp = AnchorBlock->hSystemApp;
   else
   if ( lpView->hSubtask )
   {
      LPSUBTASK lpSubtask = zGETPTR( lpView->hSubtask );
      hApp = lpSubtask->hApp;
   }
   else
   if ( lpView->hViewOD )
   {
      lpViewOD = zGETPTR( lpView->hViewOD );
      hApp = lpViewOD->hApp;
   }
   else
      hApp = lpTask->hApp;

   if ( hApp == 0 )
      return( 0 );    // if not, error

   lpApp = zGETPTR( hApp );
// TraceLine( "ActivateViewObject App (0x%08x): %s   ViewOD: %s", hApp, lpApp->szName, cpcViewOD_Name );

   // Next, see if View Object is already active in App.  Note that we haven't
   // locked the mutex yet because 99% of the time the OD has already been
   // activated and we don't want the overhead of locking the mutex.  If the
   // OD hasn't been loaded we'll lock the mutex and then check again.
   for ( lpViewOD = zGETPTR( lpApp->hFirstViewOD );
         lpViewOD;
         lpViewOD = zGETPTR( lpViewOD->hNextViewOD ) )
   {
   // TraceLine( "ActivateViewObject App (0x%08x): %s   checking ViewOD (0x%08x): %s",
   //            hApp, lpApp->szName, zGETHNDL( lpViewOD ), lpViewOD->szName );

      // See if this is the ViewOD we're looking for.
      if ( zstrcmpi( lpViewOD->szName, cpcViewOD_Name ) == 0 )
      {
         // This is the right ViewOD.  See if we're supposed to reload it.
         if ( lpViewOD->bDeprecated )
            break; // we need to reload this guy so break loop

         // Everything OK so return.
         return( lpViewOD );
      }
   }

// if ( zstrcmp( cpcViewOD_Name, "mGLBudgt" ) == 0 )
//  {
//    nEOF = 1;
//    nEOF /= nEOF - nEOF;
// }

   // Set ProcessInfo record.
   zmemset( (zPVOID) &ProcessInfo, 0, sizeof( ProcessInfo ) );
   ProcessInfo.hApp        = hApp;
   ProcessInfo.lpApp       = lpApp;
   ProcessInfo.lpSystemApp = zGETPTR( AnchorBlock->hSystemApp );
   ProcessInfo.lpTask      = lpTask;
   ProcessInfo.lpView      = lpView;

   // Set up the directory of where to find the object file.
   strcpy_s( ProcessInfo.szFileName, sizeof( ProcessInfo.szFileName ), lpApp->szObjectDir );
   strcat_s( ProcessInfo.szFileName, sizeof( ProcessInfo.szFileName ), cpcViewOD_Name );
   strcat_s( ProcessInfo.szFileName, sizeof( ProcessInfo.szFileName ), szObjectSuffix );

   // DGC 7/11/96 -- The following is a hack for compatability with Unix.
   // Unix file names are case-sensitive.  Since DOS/Windows files (when
   // copied to a unix system via a network) are assigned lower-case letters
   // in Unix, we will (for now) make all Zeidon System file names lowercase.
   _strlwr_s( ProcessInfo.szFileName, sizeof( ProcessInfo.szFileName ) );  // zstrlower

   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   // Check again to see if the ViewOD is in the list.  It's possible that
   // the OD was loaded by another process after we checked for the OD but
   // before we set the mutex.
   // This is all done to save the overhead of locking the mutex when it
   // doesn't need to be done (see above).
   for ( lpViewOD = zGETPTR( lpApp->hFirstViewOD );
         lpViewOD;
         lpViewOD = zGETPTR( lpViewOD->hNextViewOD ) )
   {
      // See if this is the ViewOD we're looking for.
      if ( zstrcmpi( lpViewOD->szName, cpcViewOD_Name ) == 0 )
      {
         // This is the right ViewOD.  See if we're supposed to reload it.
         if ( lpViewOD->bDeprecated )
            break; // We need to reload this guy so break loop.

         // Everything OK so return.
         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
         return( lpViewOD );
      }
   }

   if ( (hFile = SysOpenFile( lpView, ProcessInfo.szFileName,
                              COREFILE_READ )) == -1 )
   {
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      //  "KZOEE050 - Error opening View Object file "
      fnIssueCoreError( lpTask, lpView, 8, 50, 0, ProcessInfo.szFileName, 0 );
      return( 0 );
   }

   // Display the name and date time stamp of file just opened.
   if ( AnchorBlock->TraceFlags.bOpenFile )
      TraceLineS( "(od) opening Object Def file Name: ", ProcessInfo.szFileName );

   ProcessInfo.hFile = hFile;

   // Object file opened, load the object definition.
   ProcessInfo.nCurrState = 0;    // 0 ==> Looking for Object Entity
   nEOF = SysReadLine( lpView, &ProcessInfo.szLine, hFile );

   // Invalid file, fall out.
   if ( ProcessInfo.szLine[ 0 ] == 'z' )
   {
      // New style.
      if ( zstrncmp( &ProcessInfo.szLine[ 6 ], "Zeidon", 6 ) != 0 )
      {
         zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

         //  "KZOEE051 - Invalid View Object file header in "
         fnIssueCoreError( lpTask, lpView, 16, 51, 0, ProcessInfo.szFileName, 0 );
         SysCloseFile( lpView, hFile, 0 );
         return( 0 );
      }
   }
   else
   if ( zstrncmp( ProcessInfo.szLine, "pZeidon", 7 ) != 0 )
   {
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

      //  "KZOEE051 - Invalid View Object file header in "
      fnIssueCoreError( lpTask, lpView, 16, 51, 0, ProcessInfo.szFileName, 0 );
      SysCloseFile( lpView, hFile, 0 );
      return( 0 );
   }

   nEOF = SysReadLine( lpView, &ProcessInfo.szLine, hFile );  // get second line

   ProcessInfo.lLine = 2;
   while ( nEOF > 0 )
   {
      if ( ProcessInfo.szLine[ 0 ] == 'e' )
      {
         // If currently processing an attribute, make sure length is specified.
         if ( ProcessInfo.nCurrState == 3 &&  // processing ATTRIB
              ProcessInfo.bLth == FALSE )
         {
            if ( lpViewAttrib->cType == 'B' )
            {
               lpViewAttrib->ulLth = 7;
               ProcessInfo.bLth = TRUE;
            }
            else
            {
               //  "KZOEE052 - Invalid Object file, Attrib w/o lth on line "
               fnIssueCoreError( lpTask, lpView, 16, 52, ProcessInfo.lLine,
                                 ProcessInfo.szFileName, 0 );
               ProcessInfo.nCurrState = 10;  // set error
               break;
            }
         }

         // An Entity type has been found, validate the Entity name and that
         // it has been found when it was expected.
         if ( ProcessInfo.nCurrState == 0 &&  // looking for OBJECT
              zstrncmp( &ProcessInfo.szLine[ 1 ], szlOBJECT, 6 ) == 0 )
         {
            // We were looking for an OBJECT and it was found
            ProcessInfo.nCurrState = 1;  // working on OBJECT
            hViewOD = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                        sizeof( ViewOD_Record ), 1, 0,
                                        iViewOD );
            lpViewOD = zGETPTR( hViewOD );
         // TraceLine( "ActivateViewObject: %s  ViewOD: (0x%08x) 0x%08x",
         //            cpcViewOD_Name, lpViewOD, hViewOD );
            if ( lpViewOD == 0 )
               ProcessInfo.nCurrState = 10;  // set error
            else
            {
               lpViewOD->hApp = hApp;  // set application for object
               lpViewEntity = 0;
               nHierNbr     = 0;
            }
         }
         else
         if ( ProcessInfo.nCurrState &&
              (zstrncmp( &ProcessInfo.szLine[ 1 ], szlCHILDENTITY, 11 ) == 0 ||
               zstrncmp( &ProcessInfo.szLine[ 1 ], szlENTITY, 6 ) == 0) )
         {
            // Processing OBJECT, ENTITY, OR ATTRIB and ENTITY appears.
            LPVIEWENTITY lpPrevViewEntity;

            if ( lpViewEntity )        // dks ... 2006.01.25
            {
               if ( ProcessInfo.nKeyCnt == 1 )
                  lpViewEntity->bSingleKey = TRUE;
            }

            ProcessInfo.nKeyCnt = 0;   // dks ... 2006.01.25

            // Attribute matching when task debug is on.
            lpViewAttrib = 0;
            lpDataRecord = 0;
            ProcessInfo.ulRecordOffset = 0; // offset of attribute in record
            ProcessInfo.ulNonPersistOffset = 0;
            ProcessInfo.nCurrState = 2;    // processing ENTITY

            lpPrevViewEntity = lpViewEntity;
            hViewEntity = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                            sizeof( ViewEntityRecord ), 1, 0,
                                            iViewEntity );
            lpViewEntity = zGETPTR( hViewEntity );
            if ( lpViewEntity == 0 )
               ProcessInfo.nCurrState = 10;  // set error
            else
            {
               // Parse the Attribute line received.
               SysParseLine( ProcessInfo.szAttrName, sizeof( ProcessInfo.szAttrName ),
                             &ProcessInfo.szLine, &ProcessInfo.szLine[ 1 ] );

               // Set the Level for the Entity.
               lpViewEntity->nLevel = (zSHORT) (zatol( ProcessInfo.szLine ) - 1);

               // Set hierarchical number for the entity.
               nHierNbr++;
               lpViewEntity->nHierNbr = nHierNbr;
               lpViewOD->uEntities = nHierNbr;

               // Chain new Entity to the previous entity.
               if ( lpPrevViewEntity )
               {
                  LPVIEWENTITY lpParent;
                  LPVIEWENTITY lpSearchEntity;

                  lpPrevViewEntity->hNextHier = hViewEntity;
                  lpViewEntity->hPrevHier     = zGETHNDL( lpPrevViewEntity );

                  // Now set the parent of the entity.
                  while ( lpPrevViewEntity &&
                          lpPrevViewEntity->nLevel !=
                                          (lpViewEntity->nLevel - 1) )
                  {
                     lpPrevViewEntity = zGETPTR( lpPrevViewEntity->hParent );
                  }

                  if ( lpPrevViewEntity == 0 )
                  {
                     //  "KZOEE053 - Invalid Entity level found on line "
                     fnIssueCoreError( lpTask, lpView, 16, 53, ProcessInfo.lLine,
                                       ProcessInfo.szFileName, 0 );
                     ProcessInfo.nCurrState = 10;  // set error
                  }

                  lpViewEntity->hParent = zGETHNDL( lpPrevViewEntity );
               // if ( lpViewEntity->hParent == (LPVIEWENTITY) 1 )
               //    SysMessageBox( 0, "ActivateViewObject", "1", -1 );

                  // Set persistent indicator based on persistent
                  // indicator of parent chain.
                  lpParent = zGETPTR( lpViewEntity->hParent );
                  if ( lpParent->bDerivedPath )
                     lpViewEntity->bDerivedPath = TRUE;

                  if ( lpParent->bDupRelPath )
                  {
                     lpViewEntity->bDupRelPath = TRUE;
                     lpViewEntity->bDupRel     = FALSE;
                  }

                  // Set the sibling pointers.
                  for ( lpSearchEntity = zGETPTR( lpViewEntity->hPrevHier );
                        lpSearchEntity;
                        lpSearchEntity = zGETPTR( lpSearchEntity->hPrevHier ) )
                  {
                     // If lpSearchEntity is the lpParent then we're done
                     // searching.
                     if ( lpSearchEntity == lpParent )
                        break;

                     // If we find an entity with the same level as the
                     // current entity then we have found the prev sibling.
                     if ( lpSearchEntity->nLevel == lpViewEntity->nLevel )
                     {
                        lpSearchEntity->hNextSibling = zGETHNDL( lpViewEntity );
                        lpViewEntity->hPrevSibling = zGETHNDL( lpSearchEntity );
                        break;
                     }
                  }
               }
               else
                  lpViewOD->hFirstOD_Entity = hViewEntity;
            }
         }
         else
         if ( (ProcessInfo.nCurrState == 2 ||   // processing ENTITY
               ProcessInfo.nCurrState == 3) &&  // processing ATTRIB
              zstrncmp( &ProcessInfo.szLine[ 1 ], szlATTRIB, 6 ) == 0 )
         {
            // Processing ENTITY, OR ATTRIB and ATTRIB appears.
            LPVIEWATTRIB lpPrevViewAttrib;

            ProcessInfo.nCurrState = 3;  // processing ATTRIB
            ProcessInfo.bPersist = FALSE;
            ProcessInfo.bLth     = FALSE;
            ProcessInfo.bDomain  = FALSE;
            ProcessInfo.bType    = FALSE;
            lpPrevViewAttrib = lpViewAttrib;
            hndl = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                     sizeof( ViewAttribRecord ), 1, 0,
                                     iViewAttrib );
            lpViewAttrib = zGETPTR( hndl );
            if ( lpViewAttrib == 0 )
               ProcessInfo.nCurrState = 10;  // set error
            else
            {
               lpViewAttrib->hViewEntity = hViewEntity;

               // Chain new Attrib to Entity or prev Attrib.
               if ( lpPrevViewAttrib )
                  lpPrevViewAttrib->hNextOD_Attrib = hndl;
               else
                  lpViewEntity->hFirstOD_Attrib = hndl;
            }
         }
         else
         if ( ProcessInfo.nCurrState == 3 &&
              zstrncmp( &ProcessInfo.szLine[ 1 ], szlDATARECORD, 10 ) == 0 )
         {
            LPDATARECORD lpPrevDataRecord;

            ProcessInfo.nCurrState = 4;  // processing DATARECORD
            lpDataField  = 0;
            lpRelRecord  = 0;
            lpPrevDataRecord = lpDataRecord;
            hndl = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                     sizeof( DataRecordRecord ), 1, 0,
                                     iDataRecord );
            lpDataRecord = zGETPTR( hndl );
            if ( lpDataRecord == 0 )
               ProcessInfo.nCurrState = 10;  // set error
            else
            {
               if ( lpPrevDataRecord )
                  lpPrevDataRecord->hNextDataRecord = hndl;
               else
                  lpViewEntity->hFirstDataRecord = hndl;
            }
         }
         else
         if ( (ProcessInfo.nCurrState == 4 ||   // processing DATARECORD
               ProcessInfo.nCurrState == 5) &&  // processing DATAFIELD
              zstrncmp( &ProcessInfo.szLine[ 1 ], szlDATAFIELD, 9 ) == 0 )
         {
            LPDATAFIELD  lpPrevDataField;

            ProcessInfo.nCurrState = 5;  // processing DATAFIELD
            lpPrevDataField = lpDataField;
            hndl = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                     sizeof( DataFieldRecord ), 1, 0,
                                     iDataField );
            lpDataField = zGETPTR( hndl );
            if ( lpDataField == 0 )
               ProcessInfo.nCurrState = 10;  // set error
            else
            {
               if ( lpPrevDataField )
                  lpPrevDataField->hNextDataField = hndl;
               else
                  lpDataRecord->hFirstDataField = hndl;
            }
         }
         else
         if ( (ProcessInfo.nCurrState == 5 ||   // processing DATAFIELD
               ProcessInfo.nCurrState == 6 ||   // processing RELRECORD
               ProcessInfo.nCurrState == 7) &&  // processing RELFIELD
              zstrncmp( &ProcessInfo.szLine[ 1 ], szlRELRECORD, 9 ) == 0 )
         {
            LPRELRECORD  lpPrevRelRecord;

            ProcessInfo.nCurrState = 6;  // processing RELRECORD
            lpRelField = 0;
            lpPrevRelRecord = lpRelRecord;
            hndl = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                     sizeof( RelRecordRecord ), 1, 0,
                                     iRelRecord );
            lpRelRecord = zGETPTR( hndl );
            if ( lpRelRecord == 0 )
               ProcessInfo.nCurrState = 10;  // set error
            else
            {
               if ( lpPrevRelRecord )
                  lpPrevRelRecord->hNextRelRecord = hndl;
               else
                  lpDataRecord->hFirstRelRecord = hndl;
            }
         }
         else
         if ( (ProcessInfo.nCurrState == 5 ||   // processing DATAFIELD
               ProcessInfo.nCurrState == 6 ||   // processing RELRECORD
               ProcessInfo.nCurrState == 7) &&  // processing RELFIELD
              zstrncmp( &ProcessInfo.szLine[ 1 ], szlRELFIELD, 8 ) == 0 )
         {
            LPRELFIELD  lpPrevRelField;

            ProcessInfo.nCurrState = 7;  // processing RELFIELD
            lpPrevRelField = lpRelField;
            hndl = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                     sizeof( RelFieldRecord ), 1, 0,
                                     iRelField );
            lpRelField = zGETPTR( hndl );
            if ( lpRelField == 0 )
               ProcessInfo.nCurrState = 10;  // set error
            else
            {
               if ( lpPrevRelField )
                  lpPrevRelField->hNextRelField = hndl;
               else
                  lpRelRecord->hFirstRelField = hndl;
            }
         }
         else
         {
            //  "KZOEE054 - Invalid record found on line "
            fnIssueCoreError( lpTask, lpView, 16, 54, ProcessInfo.lLine,
                              ProcessInfo.szFileName, 0 );
            ProcessInfo.nCurrState = 10;  // set error
         }
      }
      else
      if ( ProcessInfo.szLine[ 0 ] == 'a' )
      {
         // Parse the Attribute line received.
         SysParseLine( ProcessInfo.szAttrName, sizeof( ProcessInfo.szAttrName ),
                       &ProcessInfo.szLine, &ProcessInfo.szLine[ 1 ] );

         switch ( ProcessInfo.nCurrState )
         {
            //
            //  ATTRIBUTES FOR OBJECT ENTITY
            //
            case 1:  // Processing Attributes for Object Entity
               fnProcessObjectEntity( lpView, lpViewOD, &ProcessInfo );
               break;

            //
            //  ATTRIBUTES FOR ENTITY ENTITY
            //
            case 2:  // Processing Attributes for Entity Entity
               fnProcessEntityEntity( lpViewOD, lpViewEntity, &ProcessInfo );
               break;

            //
            //  ATTRIBUTES FOR ATTRIB ENTITY
            //
            case 3:  // Processing Attributes for Attribute Entity
               fnProcessAttribEntity( lpView, lpViewOD, lpViewEntity,
                                      lpViewAttrib, &ProcessInfo );
               break;

            //
            //  ATTRIBUTES FOR DATARECORD ENTITY
            //
            case 4:  // Processing Attributes for Data Record Entity
               fnProcessDataRecordEntity( lpViewOD, lpViewEntity,
                                          lpDataRecord, &ProcessInfo );
               break;

            //
            //  ATTRIBUTES FOR DATAFIELD ENTITY
            //
            case 5:  // Processing Attributes for Data Field Entity
               fnProcessDataFieldEntity( lpViewOD, lpViewEntity, lpDataRecord,
                                         lpDataField, &ProcessInfo );
               break;

            //
            //  ATTRIBUTES FOR RELRECORD ENTITY
            //
            case 6:  // Processing Attributes for Rel Record Entity
               fnProcessRelRecordEntity( lpViewOD, lpViewEntity, lpDataRecord,
                                         lpRelRecord, &ProcessInfo );
               break;

            //
            //  ATTRIBUTES FOR RELFIELD ENTITY
            //
            case 7:  // Processing Attributes for Rel Field Entity
               fnProcessRelFieldEntity( lpViewOD, lpViewEntity, lpDataRecord,
                                        lpRelRecord, lpRelField, &ProcessInfo );
               break;

            default:
               break;

         }   // End switch ( nCurrState )
      }      // End if for 'e' or 'a' records
      else
      {
         //  "KZOEE054 - Invalid record found on line "
         fnIssueCoreError( lpTask, lpView, 16, 54, ProcessInfo.lLine,
                           ProcessInfo.szFileName, 0 );
         ProcessInfo.nCurrState = 10;  // set error
      }

      // If an error occurred, force exit from loop.
      if ( ProcessInfo.nCurrState == 10 )
         nEOF = 0;    // Error occurred force exit from loop
      else
      {
         // Read away until non-blank line or end-of-file reached.
         while ( (nEOF = SysReadLine( lpView, &ProcessInfo.szLine, hFile )) > 0 )
         {
            ProcessInfo.lLine++;
            if ( ProcessInfo.lLine == 400 )
               TraceLineS( ProcessInfo.szFileName, "   400" );
            if ( ProcessInfo.szLine[ 0 ] == ' ' ||
                 ProcessInfo.szLine[ 0 ] == ';' ||
                 ProcessInfo.szLine[ 0 ] == 0 )
            {
               // Line contains a char that we should ignore, so read next line.
               continue;
            }

            // If we get here then we found a valid line so break loop.
            break;
         }
      }
   } // while ( nEOF > 0 )...

   // Close the Object file.
   SysCloseFile( lpView, hFile, 0 );

   // If currently processing an attribute, make sure length is specified.
   if ( ProcessInfo.nCurrState == 3 &&  // processing ATTRIB
        ProcessInfo.bLth == FALSE )
   {
      if ( lpViewAttrib->cType == 'B' )
      {
         lpViewAttrib->ulLth = 7;
         ProcessInfo.bLth = TRUE;
      }
      else
      {
         //  "KZOEE052 - Invalid Object file, Attrib w/o lth on line "
         fnIssueCoreError( lpTask, lpView, 16, 52, ProcessInfo.lLine,
                           ProcessInfo.szFileName, 0 );
         ProcessInfo.nCurrState = 10;  // set error
      }
   }

   // If an error did NOT occur, add this object to the application.
   if ( lpViewOD && ProcessInfo.nCurrState != 10 )
   {
   // LPVIEWOD  lpViewODT;

      lpViewOD->hNextViewOD = lpApp->hFirstViewOD;
      lpApp->hFirstViewOD = hViewOD;
   // TraceLine( "ActivateViewObject App (0x%08x): %s   loaded ViewOD (0x%08x): %s",
   //            zGETHNDL( lpApp ), lpApp->szName, zGETHNDL( lpViewOD ), lpViewOD->szName );
   //
   // for ( lpViewODT = zGETPTR( lpApp->hFirstViewOD );
   //       lpViewODT;
   //       lpViewODT = zGETPTR( lpViewODT->hNextViewOD ) )
   // {
   //    TraceLine( "ActivateViewObject (after add) App (0x%08x): %s  ViewOD (0x%08x): %s",
   //               zGETHNDL( lpApp ), lpApp->szName, zGETHNDL( lpViewODT ), lpViewODT->szName );
   // }
   }
   else
   {
      fnDeleteViewObject( lpViewOD );
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      return( 0 ); // return null Object pointer
   }

   // Now loop through all the entities/attributes and reset the offset
   // values to skip over the attribute flag structure.
   for ( lpViewEntity = zGETPTR( lpViewOD->hFirstOD_Entity );
         lpViewEntity;
         lpViewEntity = zGETPTR( lpViewEntity->hNextHier ) )
   {
      for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
            lpViewAttrib;
            lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         lpViewAttrib->ulRecordOffset += sizeof( AttribFlagsRecord );
      }
   }

// TraceLine( "ActivateViewObject App (0x%08x): %s   checking ViewOD (0x%08x): %s  LibName: %s  DBHandler: 0x%08x  From: %s",
//            zGETHNDL( lpApp ), lpApp->szName, zGETHNDL( lpViewOD ),
//            cpcViewOD_Name, lpViewOD->szOperLibname, lpViewOD->hDBHandler,
//            ProcessInfo.szFileName );

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

// if ( zstrcmp( cpcViewOD_Name, "mGLBudgt" ) == 0 )
//    DisplayObjectDefinition( lpViewOD );

   return( lpViewOD );  // return Object Definition pointer
}

//./ ADD NAME=fnDeleteViewObject
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnDeleteViewObject
//
//  PURPOSE:    To delete View Object from an application
//
//  RETURNS:    0           - Delete worked
//              zCALL_ERROR - invalid view object
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT   OPERATION
fnDeleteViewObject( LPVIEWOD lpViewOD )
{
   LPAPP    lpApp;
   LPVIEWOD lpPrevViewOD;

   if ( AnchorBlock == 0 || lpViewOD == 0 )
      return( zCALL_ERROR );

   if ( lpViewOD->nTableID != iViewOD )
      return( zCALL_ERROR );

   // Get Application for this ViewOD.
   lpApp = zGETPTR( lpViewOD->hApp );

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLine( "Deleting Object Definition (0x%08x): %s for App: %s",
                 zGETHNDL( lpViewOD ), lpViewOD->szName, lpApp->szName );

   lpPrevViewOD = 0;

   // Unchain this view OD from the application.
   if ( zGETPTR( lpApp->hFirstViewOD ) != lpViewOD )
   {
      lpPrevViewOD = zGETPTR( lpApp->hFirstViewOD );
      while ( lpPrevViewOD &&
              zGETPTR( lpPrevViewOD->hNextViewOD ) != lpViewOD )
      {
         lpPrevViewOD = zGETPTR( lpPrevViewOD->hNextViewOD );
      }

      if ( lpPrevViewOD )
         lpPrevViewOD->hNextViewOD = lpViewOD->hNextViewOD;
   }
   else
      lpApp->hFirstViewOD = lpViewOD->hNextViewOD;

   // Free TaskOperations for this OD.
   while ( lpViewOD->hFirstTaskOperation )
      fnDropTaskOperation( 0, zGETPTR( lpViewOD->hFirstTaskOperation ) );

   // Free up view Entity information.
   while ( lpViewOD->hFirstOD_Entity )
   {
      LPVIEWENTITY lpViewEntity;

      lpViewEntity = zGETPTR( lpViewOD->hFirstOD_Entity );

      // Free TaskOperations for this OD.
      while ( lpViewEntity->hFirstTaskOperation )
         fnDropTaskOperation( 0, zGETPTR( lpViewEntity->hFirstTaskOperation ) );

      // Free up Data Record information for view entity.
      while ( lpViewEntity->hFirstDataRecord )
      {
         LPDATARECORD lpDataRecord;

         lpDataRecord = zGETPTR( lpViewEntity->hFirstDataRecord );

         // Free up Data Field information for Data Record.
         while ( lpDataRecord->hFirstDataField )
         {
            LPDATAFIELD lpDataField;

            lpDataField = zGETPTR( lpDataRecord->hFirstDataField );
            lpDataRecord->hFirstDataField = lpDataField->hNextDataField;
            fnFreeDataspace( lpDataField );
         }

         // Free up Rel Record information for Data Record.
         while ( lpDataRecord->hFirstRelRecord )
         {
            LPRELRECORD lpRelRecord;

            lpRelRecord = zGETPTR( lpDataRecord->hFirstRelRecord );

            // Free up Rel Field information for Rel Record.
            while ( lpRelRecord->hFirstRelField )
            {
               LPRELFIELD lpRelField;

               lpRelField = zGETPTR( lpRelRecord->hFirstRelField );
               lpRelRecord->hFirstRelField = lpRelField->hNextRelField;
               fnFreeDataspace( lpRelField );
            }

            lpDataRecord->hFirstRelRecord = lpRelRecord->hNextRelRecord;
            fnFreeDataspace( lpRelRecord );
         }

         lpViewEntity->hFirstDataRecord = lpDataRecord->hNextDataRecord;
         fnFreeDataspace( lpDataRecord );
      }

      // Free up View Attrib information for View Entity.
      while ( lpViewEntity->hFirstOD_Attrib )
      {
         LPVIEWATTRIB lpViewAttrib;

         lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );

         // Free TaskOperations for this OD.
         while ( lpViewAttrib->hFirstTaskOperation )
            fnDropTaskOperation( 0, zGETPTR( lpViewAttrib->hFirstTaskOperation ) );

         fnFreeDataspace( zGETPTR( lpViewAttrib->hDerivedExpr ) );
         fnFreeDataspace( zGETPTR( lpViewAttrib->hDerivedXPG ) );
         fnFreeDataspace( zGETPTR( lpViewAttrib->hInit ) );
         lpViewEntity->hFirstOD_Attrib = lpViewAttrib->hNextOD_Attrib;
         fnFreeDataspace( lpViewAttrib );
      }

      lpViewOD->hFirstOD_Entity = lpViewEntity->hNextHier;
      fnFreeDataspace( lpViewEntity );
   }

   if ( lpViewOD->hDBH_Data )
      fnFreeDataspace( zGETPTR( lpViewOD->hDBH_Data ) );

   fnFreeDataspace( lpViewOD );

   return( 0 );
}

//./ ADD NAME=DropViewObject
// Source Module=kzoeodaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DropViewObject
//
//  PURPOSE:    To drop a View Object for an application.  If the View OD is
//              still in use, it becomes deprecated and all following views
//              will use the new OD.
//
//  ARGUMENTS:  lpView - Subtask view for application.
//              pchViewOD_Name - Name of View OD to drop.  If View OD is NULL,
//                   then the ViewOD of lpView is dropped.
//              bSystemObject - If TRUE then pchViewOD_Name is a system OD.
//
//  RETURNS:    1 - View OD dropped.
//              0 - View OD is still in use but has been deprecated.
//              zCALL_ERROR
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zSHORT OPERATION
DropViewObject( zVIEW   lpView,
                zCPCHAR pchViewOD_Name,
                zBOOL   bSystemObject )
{
   LPTASK         lpCurrentTask;
   LPAPP          hApp, lpApp;
   LPVIEWOD       lpViewOD;
   zSHORT         nRC = 1;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iDropViewObject, lpView,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( bSystemObject )
      hApp = AnchorBlock->hSystemApp;
   else
   if ( lpView->hSubtask )
   {
      LPSUBTASK lpSubtask = zGETPTR( lpView->hSubtask );
      hApp = lpSubtask->hApp;
   }
   else
   if ( lpView->hViewOD )
   {
      lpViewOD = zGETPTR( lpView->hViewOD );
      hApp = lpViewOD->hApp;
   }
   else
      hApp = lpCurrentTask->hApp;

   lpApp = zGETPTR( hApp );

   zLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   if ( pchViewOD_Name )
   {
      // Find the first occurrence of the view OD.
      for ( lpViewOD = zGETPTR( lpApp->hFirstViewOD );
            lpViewOD;
            lpViewOD = zGETPTR( lpViewOD->hNextViewOD ) )
      {
         // See if this is the ViewOD we're looking for.
         if ( zstrcmpi( lpViewOD->szName, pchViewOD_Name ) == 0 )
            break;
      }
   }
   else
      lpViewOD = zGETPTR( lpView->hViewOD );

   if ( lpViewOD == 0 || lpViewOD->bDeprecated )
   {
      // Couldn't find it.  Nothing to do.
      zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );
      fnOperationReturn( iDropViewObject, lpCurrentTask );
      return( nRC );
   }

   // Set flag to indicate this OD is deprecated.
   lpViewOD->bDeprecated = TRUE;

   // Check to see if any views still reference the view.  If so, then
   // we can't drop it.  We'll do it later.
   if ( fnCheckIfViewOD_IsUsed( lpView, lpViewOD ) == zCALL_ERROR )
   {
      zCHAR szMsg[ 200 ];

      sprintf_s( szMsg, sizeof( szMsg ), "View referencing '%s' = ", lpViewOD->szName );
      TraceLineX( szMsg, (zLONG) lpView );
      nRC = 1;
   }
   else
   {
      // ViewOD is not in use so remove it.
      fnDeleteViewObject( lpViewOD );
   }

   zUNLOCK_MUTEX( zMUTEX_ANCHORBLOCK );

   fnOperationReturn( iDropViewObject, lpCurrentTask );
   return( nRC );
}
