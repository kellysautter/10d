#include "kzoephdr.h"
//
// MODULE NAME:  KZOEAPAA  -  Object services Application Management
// DESCRIPTION:  This is the source file which contains Object services
//               application management operations.
// OPERATIONS:   All OPERATIONS managing Applications
// **********************************************************************
// * Copyright (c) 1993-2016 QuinSoft, Inc. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    4/08/92
//  API:     NONE
//  ENVIRONMENT: ANY
//

/*
CHANGE LOG

2002.09.05  DGC
   Removed reference to KZOMDMOI.XDM

2002.01.18  HH
   added SetDatabaseUser.

2001.08.06  DGC
   Fixed bug 55515--We now allow domains that consist of multiple blanks.

2000.11.06  DGC
   Removed code so that tables can have a NULL external value.

2000.04.18  DGC
   Added bKeepAlive in App struct to allow us to keep an application loaded
   in core even if there are no tasks referencing it.

1999.11.29  DGC
   Added code to fnAllocDataspace to help find leaks.
*/

#if defined( _WINDOWS ) && defined( DEBUG )
#pragma optimize( "", off )
#endif

#include <stdio.h>
#include <stdlib.h>

//./ ADD NAME=GetApplDirectoryFromView
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      GetApplDirectoryFromView
//
//  PURPOSE:    To retrieve one of the directory names defined for
//              an application.
//
//  PARAMETERS: pchReturnDirectory - return variable for the directory
//
//              lpView - A view which identifies the subtask pointing
//                       to the application from which the directory
//                       is to be retrieved.
//
//              nDirectory - zAPPL_DIR_LIB=1 - DLL Directory
//                           zAPPL_DIR_OBJECT=2 - Exe Meta Directory
//                           zAPPL_DIR_LOCAL=3 - Local directory
//                           zAPPL_DIR_SHARED=4 - Shared directory
//                           zAPPL_DIR_QLPLR=5 - Query LPLR directory
//                           zAPPL_DIR_SOURCE=6 - LPLR Source directory
//
//  RETURNS:    0           - Directory returned in pchReturnDirectory
//              zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
GetApplDirectoryFromView( zPCHAR pchReturnDirectory,
                          zVIEW  lpView,
                          zSHORT nDirectory,
                          zSHORT nMaxLth )
{
   LPTASK lpCurrentTask;
   LPAPP  lpApp;
   zPCHAR pchDirectory;
   zSHORT nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iGetApplDirectoryFromView, lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( lpView->hSubtask )
   {
      LPSUBTASK lpSubtask = zGETPTR( lpView->hSubtask );

      lpApp = lpSubtask->hApp;
   }
   else
   if ( lpView->hViewOD )
   {
      LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

      lpApp = lpViewOD->hApp;
   }
   else
      lpApp = lpCurrentTask->hApp;

   lpApp = zGETPTR( lpApp );

   pchReturnDirectory[ 0 ] = 0;
   nRC = 0;
   switch ( nDirectory )
   {
      case zAPPL_DIR_LIB:
         pchDirectory = lpApp->szLibraryDir;
         break;

      case zAPPL_DIR_OBJECT:
         pchDirectory = lpApp->szObjectDir;
         break;

      case zAPPL_DIR_LOCAL:
         pchDirectory = lpApp->szLocalDir;
         break;

      case zAPPL_DIR_SHARED:
         pchDirectory = lpApp->szSharedDir;
         break;

      case zAPPL_DIR_QLPLR:
         pchDirectory = lpApp->szQLPLR_Dir;
         break;

      case zAPPL_DIR_SOURCE:
         pchDirectory = lpApp->szSourceDir;
         break;

      case zAPPL_NAME:
         pchDirectory = lpApp->szName;
         break;

      default:
         nRC = zCALL_ERROR;
         pchDirectory = 0;
   }

   if ( pchDirectory )
   {
      if ( (zSHORT) zstrlen( pchDirectory ) > (nMaxLth - 1) )
         nRC = zCALL_ERROR;
      else
         strcpy_s( pchReturnDirectory, nMaxLth, pchDirectory );
   }

   fnOperationReturn( iGetApplDirectoryFromView, lpCurrentTask );
   return( nRC );
}

// DGC 9/6/96  Created following local operation from SfGetApplicationForSubtask to be used internally by Core.
zSHORT LOCALOPER
fnGetApplicationForSubtask( LPAPP *lpApp,
                            zVIEW lpView )
{
   *lpApp = 0;

   if ( lpView == 0 )
      return( 0 );

   // If this is a subtask view, which it should be, pass the pointer to the application.
   if ( lpView->hSubtask )
   {
      LPSUBTASK lpSubtask = zGETPTR( lpView->hSubtask );
      *lpApp = zGETPTR( lpSubtask->hApp );
   }
   else
   if ( lpView->hViewCsr )  // there is a time when this should be done
   {                        // after the lpView->hTask, but I need to
      LPVIEWCSR lpViewCsr;  // find it and correct it since this needs to
      LPVIEWOD  lpViewOD;   // be ahead of it ... dks 2005.09.26
      LPVIEWOI  lpViewOI;   // fixed in fnFindMQInterface ... dks 2005.11.01

      lpViewCsr = zGETPTR( lpView->hViewCsr );
      lpViewOI  = zGETPTR( lpViewCsr->hViewOI );
      lpViewOD  = zGETPTR( lpViewOI->hViewOD );
      *lpApp    = zGETPTR( lpViewOD->hApp );
   }

// Change by DGC 5/27/96 -- Instead of just a subtask view we will get the application from any kind of view.

// Change by DGC 10/15/96 -- Look at lpView->hViewOD before lpView->hTask.
   else
   if ( lpView->hViewOD )
   {
      LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );
      *lpApp = zGETPTR( lpViewOD->hApp );
   }
   else
   if ( lpView->hTask )
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );
      *lpApp = zGETPTR( lpTask->hApp );
   }
   else
   {
      LPTASK lpTask = zGETPTR( lpView->hTask );

      // We don't have a subtask view, zero the returned application pointer, issue an error message, and give an error return code.
      *lpApp = 0;

      // "KZOEE107 - Invalid Subtask View"
      fnIssueCoreError( lpTask, lpView, 8, 107, 0, 0, 0 );
      return( zCALL_ERROR );
   }

   return( 0 );
}

//./ ADD NAME=SfGetApplicationForSubtask
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfGetApplicationForSubtask
//
//  PURPOSE:    To retrieve the application pointer associated with a given
//              subtask.
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
//./ END + 3
zSHORT OPERATION
SfGetApplicationForSubtask( LPAPP *lpApp,
                            zVIEW lpView )
{
   LPTASK lpCurrentTask;
   zSHORT nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfGetApplicationForSubtask,
                                          lpView, zVALID_VIEW )) == 0 )
   {
      *lpApp = 0;
      return( zCALL_ERROR );
   }

   nRC = fnGetApplicationForSubtask( lpApp, lpView );

   fnOperationReturn( iSfGetApplicationForSubtask, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=SfSetApplicationForSubtask
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SfSetApplicationForSubtask
//
//  PURPOSE:    To set the application pointer associated with a given
//              subtask.
//
//  PARAMETERS: lpView - subtask view which will be set to point to the
//                       specified application
//              lpApp - pointer to the application
//
//  RETURNS:    0           - Application set for subtask
//              zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zSHORT OPERATION
SfSetApplicationForSubtask( zVIEW  lpView,
                            LPAPP  lpApp )
{
   LPTASK    lpCurrentTask;
   LPSUBTASK lpSubtask;
   zSHORT    nRC;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSfSetApplicationForSubtask,
                                          lpView, zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( lpView->hSubtask )
   {
      lpSubtask = zGETPTR( lpView->hSubtask );
      lpSubtask->hApp = zGETHNDL( lpApp );
      nRC = zCALL_ERROR;
   }
   else
      nRC = 0;

   fnOperationReturn( iSfSetApplicationForSubtask, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=fnParseAppString
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnParseAppString
//
//  PURPOSE:    To parse the application startup string passed
//              in the RegisterZeidonApplication
//
//  PARAMETERS: pchReturnApp      - Application name returned
//              pchReturnDialog   - Dialog name returned
//              pchReturnUserID   - UserID name returned
//              pchReturnPassword - Password returned
//              pchReturnCommand  - Command returned
//              cpcAppString      - Application string to parse
//
//  RETURNS:    0 - String parsed successfully
//             -1 - Invalid string format
//
//             see parameters above
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zSHORT
fnParseAppString( zPCHAR  pchReturnApp,
                  zPCHAR  pchReturnDialog,
                  zPCHAR  pchReturnUserID,
                  zPCHAR  pchReturnPassword,
                  zPCHAR  pchReturnCommand,
                  zCPCHAR cpcAppString )
{
   zPCHAR  pchParsePtr;
   zPCHAR  pchApplPtr;
   zPCHAR  pchWdodPtr;
   zPCHAR  pchUserPtr;
   zPCHAR  pchPassPtr;
   zPCHAR  pchCmdPtr;
   zCHAR   szWorkAppString[ 256 ];
   zCHAR   chSeparator;

   // Null first byte in all return strings
   pchReturnApp[ 0 ]      = 0;
   pchReturnDialog[ 0 ]   = 0;
   pchReturnUserID[ 0 ]   = 0;
   pchReturnPassword[ 0 ] = 0;
   pchReturnCommand[ 0 ]  = 0;

   // return if pointer is null
   if ( cpcAppString == 0 || *cpcAppString == 0 )
      return( -1 );

   // STEP2: Parse the initialization String
   pchApplPtr  = 0;
   pchWdodPtr  = 0;
   pchUserPtr  = 0;
   pchPassPtr  = 0;
   pchCmdPtr   = 0;

   // Relocate the application string so we can edit it
   strcpy_s( szWorkAppString, zsizeof( szWorkAppString ), cpcAppString );

   // Get Pointers to Application Name and the Dialog Name
   pchParsePtr = szWorkAppString;
   while ( pchParsePtr[ 0 ] == ' ' )
      pchParsePtr++;

   chSeparator = *pchParsePtr;
   if ( pchParsePtr[ 0 ] == chSeparator && pchParsePtr[ 1 ] == chSeparator )
   {
      pchParsePtr += 2;
      pchApplPtr = pchParsePtr;
      while ( pchParsePtr[ 0 ] && pchParsePtr[ 0 ] != chSeparator )
         pchParsePtr++;

      if ( pchParsePtr[ 0 ] )
      {
         pchParsePtr[ 0 ] = 0;
         pchParsePtr++;
         if ( pchParsePtr[ 0 ] )
         {
            if ( pchParsePtr[ 0 ] != ' ' )
            {
               pchWdodPtr = pchParsePtr;
            }
            else
            {
               pchCmdPtr = pchParsePtr;
               while ( pchCmdPtr[ 0 ] == ' ' )
                  pchCmdPtr++;
            }
         }
      }
   }
   else
   {
      return( -1 );  // since the separator is now assumed to be the first
                     // two characters of the application parameter, the
                     // LPLR name must be specified (and therefore preceded
                     // by two separator characters).
//    pchWdodPtr = pchParsePtr;
   }

   // Parse pointer is currently at the first byte of the Dialog
   // Name.  Look for trailing null, separator, or blank which
   // ends the Dialog Name.

   //  Get the UserID if supplied. First position past the Wdod pointer.
   if ( pchCmdPtr == 0 )  // if Command found, we are done.
   {
      while ( pchParsePtr[ 0 ] &&
              pchParsePtr[ 0 ] != chSeparator &&
              pchParsePtr[ 0 ] != ' ' )
      {
         pchParsePtr++;
      }

      // if there is a token, see if it is a UserID (denoted by a separator
      // followed by a non-blank).
      if ( pchParsePtr[ 0 ] )
      {
         if ( pchParsePtr[ 0 ] == chSeparator && pchParsePtr[ 1 ] &&
              pchParsePtr[ 1 ] != ' ' )
         {
            pchUserPtr = pchParsePtr + 1;
         }
         else
         if ( pchParsePtr[ 0 ] == ' ' ||
              (pchParsePtr[ 0 ] == chSeparator && pchParsePtr[ 1 ] == ' ') )
         {
            pchCmdPtr = pchParsePtr;
            if ( pchCmdPtr[ 0 ] == chSeparator )
               pchCmdPtr++;

            while ( pchCmdPtr[ 0 ] == ' ' )
               pchCmdPtr++;
         }

         pchParsePtr[ 0 ] = 0;
         pchParsePtr++;
      }
   }

   // Get the Password if supplied.  First position past
   // the UserID pointer.
   if ( pchCmdPtr == 0 )  // if Command found, we are done
   {
      if ( pchUserPtr ) // if UserID found
      {
         while ( pchParsePtr[ 0 ] && pchParsePtr[ 0 ] != chSeparator &&
                 pchParsePtr[ 0 ] != ' ' )
         {
            pchParsePtr++;
         }
      }

      // if there is a token, see if it is a Password (denoted by a separator
      // followed by a non-blank).
      if ( pchParsePtr[ 0 ] )
      {
         if ( pchParsePtr[ 0 ] == chSeparator &&
              pchParsePtr[ 1 ] && pchParsePtr[ 1 ] != ' ' )
         {
            pchPassPtr = pchParsePtr + 1;
         }

         pchParsePtr[ 0 ] = 0;
         pchParsePtr++;

         // finally, find the command, if any
         if ( pchPassPtr )  // if Password found
         {
            while ( pchParsePtr[ 0 ] && pchParsePtr[ 0 ] != ' ' &&
                    pchParsePtr[ 0 ] != chSeparator )
            {
               pchParsePtr++;
            }

            if ( pchParsePtr[ 0 ] )
            {
               pchParsePtr[ 0 ] = 0;
               pchParsePtr++;
            }
         }

         if ( pchParsePtr[ 0 ] )
         {
            while ( pchParsePtr[ 0 ] == ' ' )
               pchParsePtr++;

            if ( pchParsePtr[ 0 ] )
               pchCmdPtr = pchParsePtr;
         }
      }
   }

#if 0
   TraceLineS( "Application parse Input String->", cpcAppString );
   // Now trace out what we have found
   if ( szApplPtr )
      TraceLineS( "Application:", szApplPtr );
   else
      TraceLineS( "Application pointer is ", "NULL" );

   if ( szWdodPtr )
      TraceLineS( "Dialog:", szWdodPtr );
   else
      TraceLineS( "Dialog pointer is ", "NULL" );

   if ( szUserPtr )
      TraceLineS( "User:", pchUserPtr );
   else
      TraceLineS( "User pointer is ", "NULL" );

   if ( pchPassPtr )
      TraceLineS( "Password:", pchPassPtr );
   else
      TraceLineS( "Password pointer is ", "NULL" );

   if ( pchCmdPtr )
      TraceLineS( "Command:", pchCmdPtr );
   else
      TraceLineS( "Command pointer is", "NULL" );
#endif

   // Copy the parameters found back for the caller
   if ( pchApplPtr )
      strcpy_s( pchReturnApp, 64, pchApplPtr );

   if ( pchWdodPtr )
      strcpy_s( pchReturnDialog, 64, pchWdodPtr );

   if ( pchUserPtr )
      strcpy_s( pchReturnUserID, 64, pchUserPtr );

   if ( pchPassPtr )
      strcpy_s( pchReturnPassword, 64, pchPassPtr );

   if ( pchCmdPtr )
      strcpy_s( pchReturnCommand, 256, pchCmdPtr );

   return( 0 );
}

//./ ADD NAME=fnCreateApp
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnCreateApp
//
//  PURPOSE:    To create application information for subsequent
//              processing by the task.
//
//  PARAMETERS: lpView       - A required view passed to error handling
//              szAppName    - Application name to load
//
//  We don't use a mutex here because every function that calls fnCreateApp
//  locks the ANCHORBLOCK mutex.  The functions that call fnCreateApp are:
//          InitializeAnchorBlock
//          RegisterZeidonApplication
//          SfCreateSubtask
//          SfCreateSystemSubtask
//
//  RETURNS:    LPAPP - Application pointer
//              0     - Application creation unsuccessful
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
LPAPP
fnCreateApp( LPTASK lpTask, zCPCHAR cpcAppName )
{
   LPAPP             lpApp;
   zPVOID            hApp;
   zPVOID            hTemp;
   zPVOID            pvFile;
   LPDOMAIN          lpDomain;
   LPDOMAIN          lpCurrDomain;
   zLONG             hFile;
   zCHAR             szWorkString[ zMAX_FILESPEC_LTH ];  // prevent overflow in
   zCHAR             szWorkString2[ zMAX_FILESPEC_LTH ]; // fnConvertEnvironnentString
   zPCHAR            pchLine;
   zSHORT            nCurrState;
   zSHORT            nCurrEntity;
   zSHORT            nLineNbr;
   zCHAR             szEntityName[ 33 ];
   zCHAR             szAttrName[ 33 ];
   zLPCONTEXT        lpContext;
   LPTABLEENTRY      lpTableEntry;
   LPNLSTABLEENTRY   lpNLS_TableEntry;
   LPREGEXPRESSION   lpRegExpression;
   LPLNAME           lpLName;
   LPDOMAIN          lpNextDomain;
   zBOOL             bFound;
   zBOOL             bLName;
   int               nEOF;
// zCHAR             chMainTask;  // debugging only dks 2005.12.01

   if ( AnchorBlock == 0 || lpTask == 0 )
      return( 0 );

   // debugging only dks 2005.12.01
// if ( zGETHNDL( lpTask ) == AnchorBlock->hMainTask )
//    chMainTask = 'Y';
// else
//    chMainTask = 0;

   // If no application name was passed and the system application
   // has been established, return the system application.
   if ( (cpcAppName == 0 || cpcAppName[ 0 ] == 0) && AnchorBlock->hSystemApp )
      return( zGETPTR( AnchorBlock->hSystemApp ) );

   if ( cpcAppName ) // if not a call to load system app
   {
      // First see if application is already running
      for ( lpApp = zGETPTR( AnchorBlock->hFirstApp );
            lpApp;
            lpApp = zGETPTR( lpApp->hNextApp ) )
      {
         if ( zstrcmpi( lpApp->szName, cpcAppName ) == 0 )
            return( lpApp );
      }

      // Next see if application is defined to system.
      strcpy_s( szWorkString, zsizeof( szWorkString ), AnchorBlock->szZeidonLocal );
      strcat_s( szWorkString, zsizeof( szWorkString ), szlAppFile );

      // Open Application definition file.
      bFound = FALSE;
      hFile = fnSysOpenFile( lpTask, szWorkString, COREFILE_READ );
      if ( hFile == -1 )
      {
         // "KZOEE009 - Error opening application definition file"
         fnIssueCoreError( lpTask, 0, 16, 9, 0, szWorkString, 0 );
         return( 0 );
      }

      // Read in application definition information.
      pvFile = 0;
      nEOF = fnSysReadLine( lpTask, &pchLine, hFile, &pvFile );
      while ( nEOF > 0 )
      {
         if ( zstrncmp( pchLine + 1, szlAPPLICATION, 11 ) == 0 )
         {
            while ( (nEOF = fnSysReadLine( lpTask, &pchLine, hFile, &pvFile )) > 0 )
            {
               if ( zstrncmp( pchLine + 1, szlAPP_NAME, 8 ) == 0 )
               {
                  SysParseLine( szWorkString, zsizeof( szWorkString ), &pchLine, pchLine + 1 );
                  if ( zstrcmpi( pchLine, cpcAppName ) == 0  )
                     bFound = TRUE;  //  Set found flag

                  break;          //  Break to Application search
               }
            }

            if ( bFound )
               break;   // Break to main line
         }

         nEOF = fnSysReadLine( lpTask, &pchLine, hFile, &pvFile );
      }

      // If the application was not found, issue an error and exit.
      if ( bFound == FALSE )
      {
         // "KZOEE010 - Requested application not found in "
         fnSysCloseFile( lpTask, hFile, 0 );  // Close the Appl Defn file
         fnIssueCoreError( lpTask, 0, 16, 10, 0, szlAppFile, cpcAppName );
         return( 0 );
      }
   }

   // Otherwise, create the named application (under ANCHORBLOCK mutex).
   hApp = (LPAPP) fnAllocDataspace( (LPDATAHEADER) g_hAnchorBlock,
                                    sizeof( AppRecord ), 1, 0, iApp );
   if ( hApp == 0 )
      return( 0 );

   lpApp = zGETPTR( hApp );
   bLName = FALSE;
   if ( cpcAppName )   // look for the user application
   {
      if ( AnchorBlock->TraceFlags.bTaskMsgs )
         TraceLine( "(APP) About to create app (0x%08x): %s (%s)  for Task: 0x%08x   Process: %d",
                    hApp, cpcAppName, pchLine, zGETHNDL( lpTask ), SysGetProcessID( 0 ) );

      strcpy_s( lpApp->szName, zsizeof( lpApp->szName ), pchLine );

      // Load up libraries for application.
      nEOF = fnSysReadLine( lpTask, &pchLine, hFile, &pvFile );
      while ( nEOF > 0 )
      {
         if ( pchLine[ 0 ] == 'e' )
         {
            if ( zstrncmp( pchLine + 1, szlLNAME, 5 ) != 0 )
               break;

            bLName = TRUE;

            // Otherwise, create the named application (under ANCHORBLOCK mutex).
            hTemp = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                      sizeof( LNameRecord ), 1, 0, iLName );
            if ( hTemp )
            {
               lpLName = zGETPTR( hTemp );

               lpApp->hFirstLName = hTemp;
               lpLName->hNextLName = lpApp->hFirstLName;
            }
         }

         if ( pchLine[ 0 ] == 'a' )
         {
            SysParseLine( szWorkString, zsizeof( szWorkString ), &pchLine, pchLine + 1 );

            // Get rid of leading spaces in pchLine.
            while ( *pchLine && *pchLine == ' ' )
               pchLine++;

            if ( *pchLine == 0 )
            {
               // Do nothing.
            }
            else
            if ( bLName )
            {
               if ( lpLName )
               {
                  if ( zstrcmp( szWorkString, szlTYPE ) == 0 )
                     lpLName->nType = (zSHORT) zatol( pchLine );
                  else
                  if ( zstrcmp( szWorkString, szlLNAME ) == 0 )
                     strcpy_s( lpLName->szLName, zsizeof( lpLName->szLName ), pchLine );
                  else
                  if ( zstrcmp( szWorkString, szlPNAME ) == 0 )
                     strcpy_s( lpLName->szPName, zsizeof( lpLName->szPName ), pchLine );
               }
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_DFLT_Dialog ) == 0 &&
                 lpApp->szDefaultDialog[ 0 ] == 0 )
            {
               strcpy_s( lpApp->szDefaultDialog, zsizeof( lpApp->szDefaultDialog ), pchLine );
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_DLL ) == 0 &&
                 lpApp->szLibraryDir[ 0 ] == 0 )
            {
               SysAppendcDirSep( pchLine );
               fnConvertEnvironmentString( szWorkString2, zsizeof( szWorkString2 ), pchLine );
               strcpy_s( lpApp->szLibraryDir, zsizeof( lpApp->szLibraryDir ), szWorkString2 );
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_ADOBIN ) == 0 &&
                 lpApp->szObjectDir[ 0 ] == 0 )
            {
               SysAppendcDirSep( pchLine );
               fnConvertEnvironmentString( szWorkString2, zsizeof( szWorkString2 ), pchLine );
               strcpy_s( lpApp->szObjectDir, zsizeof( lpApp->szObjectDir ), szWorkString2 );
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_LOCAL ) == 0 &&
                 lpApp->szLocalDir[ 0 ] == 0 )
            {
               SysAppendcDirSep( pchLine );
               fnConvertEnvironmentString( szWorkString2, zsizeof( szWorkString2 ), pchLine );
               strcpy_s( lpApp->szLocalDir, zsizeof( lpApp->szLocalDir ), szWorkString2 );
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_SHARED ) == 0 &&
                 lpApp->szSharedDir[ 0 ] == 0 )
            {
               SysAppendcDirSep( pchLine );
               fnConvertEnvironmentString( szWorkString2, zsizeof( szWorkString2 ), pchLine );
               strcpy_s( lpApp->szSharedDir, zsizeof( lpApp->szSharedDir ), szWorkString2 );
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_QLPLR ) == 0 &&
                 lpApp->szQLPLR_Dir[ 0 ] == 0 )
            {
               SysAppendcDirSep( pchLine );
               fnConvertEnvironmentString( szWorkString2, zsizeof( szWorkString2 ), pchLine );
               strcpy_s( lpApp->szQLPLR_Dir, zsizeof( lpApp->szQLPLR_Dir ), szWorkString2 );
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_QXODS ) == 0 &&
                 lpApp->szQXODS_Dir[ 0 ] == 0 )
            {
               SysAppendcDirSep( pchLine );
               fnConvertEnvironmentString( szWorkString2, zsizeof( szWorkString2 ), pchLine );
               strcpy_s( lpApp->szQXODS_Dir, zsizeof( lpApp->szQXODS_Dir ), szWorkString2 );
            }
            else
            if ( zstrcmp( szWorkString, szlAPP_SOURCE ) == 0 &&
                 lpApp->szSourceDir[ 0 ] == 0 )
            {
               SysAppendcDirSep( pchLine );
               fnConvertEnvironmentString( szWorkString2, zsizeof( szWorkString2 ), pchLine );
               strcpy_s( lpApp->szSourceDir, zsizeof( lpApp->szSourceDir ), szWorkString2 );
            }
         }

         nEOF = fnSysReadLine( lpTask, &pchLine, hFile, &pvFile );
      }
   }
   else       // set up the system application
   {
      if ( AnchorBlock->TraceFlags.bTaskMsgs )
         TraceLineS( "(APP) About to create app = ", "Zeidon System" );

      strcpy_s( lpApp->szName, zsizeof( lpApp->szName ), "Zeidon System" );
      strcpy_s( lpApp->szObjectDir, zsizeof( lpApp->szObjectDir ), AnchorBlock->szZeidonSys );
      strcpy_s( lpApp->szLibraryDir, zsizeof( lpApp->szLibraryDir ), AnchorBlock->szZeidonBin );
      strcpy_s( lpApp->szLocalDir, zsizeof( lpApp->szLocalDir ), AnchorBlock->szZeidonLoc );
      strcpy_s( lpApp->szSharedDir, zsizeof( lpApp->szSharedDir ), AnchorBlock->szZeidonShr );
   }

   if ( cpcAppName )              // if not creating the system app
      fnSysCloseFile( lpTask, hFile, 0 );  // Close the file

   // Now that the application has been loaded, load the domains
   // for the application
   strcpy_s( szWorkString, zsizeof( szWorkString ), "[App." );
   strcpy_s( szWorkString + 5, zsizeof( szWorkString ) - 5, lpApp->szName );
   strcat_s( szWorkString, zsizeof( szWorkString ), "]" );
   szWorkString2[ 0 ] = 0;

   // Check to see if we should keep the app running at all times.
   SysReadZeidonIni( -1, szWorkString, "KeepAlive", szWorkString2, zsizeof( szWorkString2 ) );
   if ( szWorkString2[ 0 ] == 'Y' )
   {
      lpApp->bKeepAlive = TRUE;
      TraceLineS( "CreateApp KeepAlive: ", lpApp->szName );
   }

   // Look for the application group in the .ini file, and the
   // Domains parameter.
   if ( zstrchr( szWorkString, ' ' ) )
   {
      zLONG k = zstrlen( szWorkString );
      while( k-- )
      {
         if ( szWorkString[ k ] == ' ' )
            szWorkString[ k ] = '_';
      }
   }

   SysReadZeidonIni( -1, szWorkString, "Domains", szWorkString2, zsizeof( szWorkString2 ) );

   // If there is directory separator then assume a path name is specified,
   // otherwise, it hopefully is just a filename.ext.
   if ( szWorkString2[ 0 ] )
   {
      if ( zstrchr( szWorkString2, cDirSep ) )
         strcpy_s( szWorkString, zsizeof( szWorkString ), szWorkString2 );
      else
      {
         strcpy_s( szWorkString, zsizeof( szWorkString ), lpApp->szObjectDir );
         SysAppendcDirSep( szWorkString );
         strcat_s( szWorkString, zsizeof( szWorkString ), szWorkString2 );
      }
   }
   else
   {
      // Build the file name for the application domains.
      strcpy_s( szWorkString, zsizeof( szWorkString ), lpApp->szObjectDir );
      SysAppendcDirSep( szWorkString );
      strcat_s( szWorkString, zsizeof( szWorkString ), szlDomainFile );
#if 0 // DGC Removed 09/05/2002
      if ( cpcAppName )
         strcat_s( szWorkString, zsizeof( szWorkString ), szlDomainFile );
      else
         strcat_s( szWorkString, zsizeof( szWorkString ), szlSystemDomainFile );
#endif
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(ap) looking for domains with: ", szWorkString );

   // Open Domains definition file.
   bFound = FALSE;
   pvFile = 0;
   hFile = fnSysOpenFile( lpTask, szWorkString, COREFILE_READ );
   if ( hFile == -1 )
   {
      if ( cpcAppName == 0 )    // the system app will run w/o domains
         return( lpApp );

      // "KZOEE011 - Error opening application domains file"
      fnIssueCoreError( lpTask, 0, 20, 11, 0, szWorkString, cpcAppName );
      fnDeleteApp( lpApp );
      return( 0 );
   }

   // Process the domain file.
   nEOF = fnSysReadLine( lpTask, &pchLine, hFile, &pvFile );

   lpDomain = 0;
   nCurrEntity = 0;  // no Current entity
   nCurrState = 0;   // Indicate no object in focus

   //
   // nCurrState codes
   //    1  we are looking at a entity object line item
   //    2  we are looking at a attribute object line item
   //   10  invalid entity type
   //   99  blank line / invalid line item.
   //
   // nCurrEntity code
   //    1  Domain
   //    2  Context
   //    3  TableEntry
   //    4  RegularExpression
   //
   nLineNbr = 0;
   while ( nEOF > 0 )
   {
      zPVOID hDomain;
      zPVOID hContext;

      nLineNbr++;
      if ( pchLine[ 0 ] == 'e' )
      {
         SysParseLine( szEntityName, zsizeof( szEntityName ), &pchLine, pchLine + 1 );
         nCurrState = 1;      // entity line item
      }
      else
      if ( pchLine[ 0 ] == 'a' )
      {
         zSHORT k;

         // We've got an attribute value.  Some attribute values may have
         // leading spaces, so we need to do some special processing and we
         // can't use SysParseLine.
         for ( k = 0; !zisspace( pchLine[ k + 1 ] ) && pchLine[ k + 1 ]; k++ )
            szAttrName[ k ] = pchLine[ k + 1 ];

         // Set the null term for the attr name.
         szAttrName[ k ] = 0;

         // Check for a null attribute value.
         if ( pchLine[ k + 1 ] == 0 )
            pchLine = pchLine + k + 1;
         else
         {
            // We have an attribute value.  First make sure we've skipped the
            // appropriate number of spaces.  Example lines:
            //    aZKey      89226532
            //    aExternalValue Income
            if ( k < 10 )
               pchLine = pchLine + 11;
            else
               pchLine = pchLine + k + 2;
         }

         nCurrState = 2;   // attribute line item
      }
      else
         nCurrState = 99;  // undefined or blank line

      switch ( nCurrState )
      {
         case 1:
            // We have the beginning of an Entity object.
            if ( zstrcmpi( szEntityName, szlDOMAIN ) == 0 )
            {
               // Create the domain (under ANCHORBLOCK mutex).
               hDomain = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                           sizeof( DomainRecord ), 1, 0, iDomain );

               if ( hDomain )
               {
                  lpDomain = zGETPTR( hDomain );

                  lpDomain->hApp = hApp;
                  lpDomain->hNextDomain = lpApp->hFirstDomain;
                  lpApp->hFirstDomain = hDomain;
                  nCurrEntity = 1;  // indicate working with Domain Entity
                  lpContext = 0;
                  lpTableEntry = 0;
                  lpNLS_TableEntry = 0;
                  lpRegExpression = 0;
                  nCurrState = 1;   // indicate we are working on a Domain object
                  break;
               }

               // "KZOEE012 - Error allocating memory"
               fnIssueCoreError( lpTask, 0, 20, 12, 0, 0, 0 );
               fnDeleteApp( lpApp );
               return( 0 );
            }
            else
            if ( zstrcmpi( szEntityName, szlCONTEXT ) == 0 )
            {
               // Create the context (under ANCHORBLOCK mutex).
               hContext = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                            sizeof( ContextRecord ), 1, 0,
                                            iContext );
               if ( hContext )
               {
                  lpContext = zGETPTR( hContext );
                  lpContext->hDomain = hDomain;
                  lpContext->hNextContext = lpDomain->hFirstContext;
                  lpContext->nDisplayLength = -1;
                  lpDomain->hFirstContext = hContext;
                  lpTableEntry = 0;
                  lpNLS_TableEntry = 0;
                  lpRegExpression = 0;
                  nCurrEntity = 2;  // indicate working with Context Entity
                  break;
               }

               // "KZOEE012 - Error allocating memory"
               fnIssueCoreError( lpTask, 0, 20, 12, 0, 0, 0 );
               fnDeleteApp( lpApp );
               return( 0 );
            }
            else
            if ( zstrcmpi( szEntityName, szlTABLEENTRY ) == 0 )
            {
               // Create the table entry (under ANCHORBLOCK mutex).
               hTemp = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                         sizeof( TableEntryRecord ), 1, 0,
                                         iTableEntry );
               if ( hTemp )
               {
                  lpTableEntry = zGETPTR( hTemp );
                  lpTableEntry->hContext = hContext;

                  if ( lpDomain->cType == zTYPE_INTEGER ||
                       lpDomain->cType == zTYPE_DECIMAL )
                  {
                     lpTableEntry->unInternal.lInteger = lNullInteger;
                  }

                  if ( lpContext->hLastTableEntry )
                  {
                     LPTABLEENTRY lpTempTable;

                     lpTempTable = zGETPTR( lpContext->hLastTableEntry );
                     lpTempTable->hNextTableEntry = hTemp;
                  }

                  lpTableEntry->hPrevTableEntry = lpContext->hLastTableEntry;
                  lpContext->hLastTableEntry = hTemp;

                  if ( lpContext->hFirstTableEntry == 0 )
                     lpContext->hFirstTableEntry = hTemp;

                  lpRegExpression = 0;
                  nCurrEntity = 3;  // indicate working with TableEntry Entity
                  break;
               }

               // "KZOEE012 - Error allocating memory"
               fnIssueCoreError( lpTask, 0, 20, 12, 0, 0, 0 );
               fnDeleteApp( lpApp );
               return( 0 );
            }
            else
            if ( zstrcmpi( szEntityName, szlNLS_TABLEENTRY ) == 0 )
            {
               // Don't do anything if there isn't a current table entry.
               if ( lpTableEntry )
               {
                  // Create the table entry (under ANCHORBLOCK mutex).
                  hTemp = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                            sizeof( NLS_TableEntryRecord ), 1, 0,
                                            iNLS_TableEntry );
                  if ( hTemp )
                  {
                     lpNLS_TableEntry = zGETPTR( hTemp );

                     // Stick lpNLS_TableEntry in the chain.
                     lpNLS_TableEntry->hNextNLS_TableEntry =
                                            lpTableEntry->hFirstNLS_TableEntry;
                     lpTableEntry->hFirstNLS_TableEntry = hTemp;

                     // indicate working with NLS_TableEntry Entity
                     nCurrEntity = 5;

                     break;
                  }

                  // "KZOEE012 - Error allocating memory"
                  fnIssueCoreError( lpTask, 0, 20, 12, 0, 0, 0 );
                  fnDeleteApp( lpApp );
                  return( 0 );
               }
            }
            else
            if ( zstrcmpi( szEntityName, szlREGULAREXPRESSION ) == 0 )
            {
               if ( lpDomain && lpContext )
               {
                  // Create the regular expression entry (under ANCHORBLOCK mutex).
                  hTemp = fnAllocDataspace( AnchorBlock->hMainFirstDataHeader,
                                            sizeof( RegExpressionRecord ), 1, 0,
                                            iRegularExpression );
                  if ( hTemp )
                  {
                     lpRegExpression = zGETPTR( hTemp );
                     lpRegExpression->hContext = hContext;
                     lpRegExpression->hNextRegExpression = lpContext->hFirstRegExpression;
                     lpContext->hFirstRegExpression = hTemp;

                     lpTableEntry = 0;
                     lpNLS_TableEntry = 0;
                     nCurrEntity = 4;  // indicate working with RegularExpression Entity
                     break;
                  }

                  // "KZOEE012 - Error allocating memory"
                  fnIssueCoreError( lpTask, 0, 20, 12, 0, 0, 0 );
                  fnDeleteApp( lpApp );
                  return( 0 );
               }
            }
            else
            if ( zstrcmpi( szEntityName, "ROOT" ) == 0 )
            {
               break;
            }
            else
            if ( zstrcmpi( szEntityName, "OBJECT" ) == 0 )
            {
               break;
            }
            else
            if ( zstrcmpi( szEntityName, "DomainGroup" ) == 0 )
            {
               break;
            }
            else
            {
               // Invalid Entity Name in Domain object
               // "KZOEE018 - Error in Domain file "
               fnIssueCoreError( lpTask, 0, 16, 18, nLineNbr,
                                 szWorkString, 0 );
               lpDomain = 0;
               lpContext = 0;
               lpTableEntry = 0;
               lpNLS_TableEntry = 0;
               lpRegExpression = 0;
               nCurrEntity = 0;
               nCurrState = 0;
               break;
            }

         case 2:
            // we have a attribute
            switch ( nCurrEntity )
            {
               case 1:
                  // Current Entity is Domain
                  if ( lpDomain == 0 )
                     break;   // no domain is 'active' skip this attribute

                  switch ( szAttrName[ 0 ] )
                  {
                     case 'C':
                        if ( zstrcmpi( szAttrName, szlCONTEXTRESTRICTION ) == 0 )
                           lpDomain->cContextRestriction = pchLine[ 0 ];

                        break;

                     case 'D':
                        if ( zstrcmpi( szAttrName, szlDOMAINTYPE ) == 0 )
                           lpDomain->cDomainType = pchLine[ 0 ];
                        else
                        if ( zstrcmpi( szAttrName, szlDATATYPE ) == 0 )
                           lpDomain->cType = pchLine[ 0 ];
                        else
                        if ( zstrcmpi( szAttrName, szlDECIMALFORMAT ) == 0 )
                        {
                           lpDomain->lDecimalFormat = zatol( pchLine );
                           lpDomain->cDecimal = pchLine[ 0 ];
                        }
                        else
                        if ( zstrcmpi( szAttrName, szlDLL_NAME ) == 0 )
                           strcpy_s( lpDomain->szLibraryName, zsizeof( lpDomain->szLibraryName ), pchLine );

                        break;

                     case 'L':
#if 0
                        if ( zstrcmpi( szAttrName, szlLTH ) == 0 )
                        {
                           lpDomain->ulLth = (zULONG) zatol( pchLine );
                           if ( lpDomain->cType == 'S' && lpDomain->ulLth )
                              lpDomain->ulLth++;
                        }
#endif
                        break;

                     case 'M':
                        if ( zstrcmpi( szAttrName, szlMAXSTRINGLTH ) == 0 )
                           lpDomain->ulMaxStringLth = (zULONG) zatol( pchLine ) + 1;

                        break;

                     case 'N':
                        if ( zstrcmpi( szAttrName, szlNAME ) == 0 )
                           strcpy_s( lpDomain->szName, zsizeof( lpDomain->szName ), pchLine );

                        break;

                     case 'O':
                        if ( zstrcmpi( szAttrName, "OPERNAME" ) == 0 )
                           strcpy_s( lpDomain->szDomainOper, zsizeof( lpDomain->szDomainOper ), pchLine );
                        else
                        if ( zstrcmpi( szAttrName, "OPERNUMBER" ) == 0 )
                           lpDomain->uOperNumber = (zUSHORT) zatol( pchLine );

                        break;

                     case 'R':
                        if ( zstrcmpi( szAttrName, szlRESTRICTED ) == 0 &&
                             (*pchLine == 'Y' || *pchLine == 'y') )
                        {
                           lpDomain->bRestricted = TRUE;
                        }

                        break;

                     case 'Z':
                        if ( zstrcmpi( szAttrName, szlZKEY ) == 0 )
                           lpDomain->lApDmTok = zatol( pchLine );

                        break;
                  }

                  break;

               case 2:
                  // Current Entity is Context
                  if ( lpContext == 0 )
                     break;   // no context is 'active' skip this attribute

                  switch ( szAttrName[ 0 ] )
                  {
                     case 'D':
                        if ( zstrcmpi( szAttrName, szlDECIMALFORMAT ) == 0 )
                           lpContext->lDecimalFormat = zatol( pchLine );

                        break;

                     case 'E':
                        if ( zstrcmpi( szAttrName, szlEDITSTRING ) == 0 )
                        {
                           lpContext->hEditString =
                             fnStoreStringInDataspace( AnchorBlock->hMainFirstDataHeader, pchLine );
                        }

                        break;

                     case 'I':
                        if ( zstrcmpi( szAttrName, szlISDEFAULT ) == 0 &&
                             (*pchLine == 'Y' || *pchLine == 'y') )
                        {
                           if ( lpDomain->lDefaultContextZKey )
                              TraceLineS( "(ap) More than one default Context for Domain: ",
                                          lpDomain->szName );
                           else
                              lpDomain->lDefaultContextZKey = lpContext->lContextTok;
                        }

                        break;

                     case 'N':
                        if ( zstrcmpi( szAttrName, szlNAME ) == 0 )
                             strcpy_s( lpContext->szName, zsizeof( lpContext->szName ), pchLine );

                        break;

                     case 'Z':
                        if ( zstrcmpi( szAttrName, szlZKEY ) == 0 )
                             lpContext->lContextTok = zatol( pchLine );

                        break;
                     }

                  break;

               case 3:
                  // Current Entity is TableEntry
                  if ( lpTableEntry == 0 )
                     break;   // no tableentry is 'active' skip this attribute

                  switch ( szAttrName[ 0 ] )
                  {
                     case 'E':
                        if ( zstrcmpi( szAttrName, szlEXTERNALVALUE ) == 0 )
                        {
                           lpTableEntry->hExternalValue =
                             fnStoreStringInDataspace( AnchorBlock->hMainFirstDataHeader,
                                                       pchLine );
                        }

                        break;

                     case 'I':
                        if ( zstrcmpi( szAttrName, szlINTERNALVALUE ) == 0 )
                        {
                           if ( lpDomain->cType == zTYPE_INTEGER ||
                                lpDomain->cType == zTYPE_DECIMAL )
                           {
                              lpTableEntry->unInternal.lInteger = zatol( pchLine );
                           }
                           else
                           {
                              lpTableEntry->unInternal.hStringValue  =
                               fnStoreStringInDataspace( AnchorBlock->hMainFirstDataHeader,
                                                         pchLine );
                           }
                        }

                        break;

                     case 'Z':
                        if ( zstrcmpi( szAttrName, szlZKEY ) == 0 )
                           lpTableEntry->lTableEntryTok = zatol( pchLine );

                        break;
                  }

                  break;

               case 4:
                  // Current Entity is RegularExpression
                  if ( lpRegExpression == 0 )
                     break;   // no regular expression is 'active' skip this attr

                  switch ( szAttrName[ 0 ] )
                  {
                     case 'Z':
                        if ( zstrcmpi( szAttrName, szlZKEY ) == 0 )
                           lpTableEntry->lTableEntryTok = zatol( pchLine );

                     break;
                  }

                  break;

               case 5:
                  // Current Entity is NLS_TableEntry
                  if ( lpNLS_TableEntry == 0 )
                     break;   // no NLS table entry is 'active' skip this attr

                  switch ( szAttrName[ 0 ] )
                  {
                     case 'L':
                        if ( zstrcmpi( szAttrName, szlLanguageIndex ) == 0 )
                           lpNLS_TableEntry->nLanguageCode = (zSHORT) zatol( pchLine );

                        break;

                     case 'T':
                        if ( zstrcmpi( szAttrName, szlText ) == 0 )
                        {
                           lpNLS_TableEntry->pszNLS_Text =
                             fnStoreStringInDataspace( AnchorBlock->hMainFirstDataHeader,
                                                       pchLine );
                        }

                        break;
                  }

                  break;
            }

            break;

         case 99:
            // invalid or blank lines... ignore
            break;

      } // end of switch( nCurrState )...

      nEOF = fnSysReadLine( lpTask, &pchLine, hFile, &pvFile );
   }

   // Scan through domains.  Drop Domains and Contexts
   // that are unnamed.  For table entrys with an internal value, but no
   // external value have the external value point to the internal value.
   // Table entrys that do not have an internal value are dropped.

   lpCurrDomain = 0; // last valid domain, 0 means at head of queue
   for ( lpDomain = zGETPTR( lpApp->hFirstDomain );
         lpDomain;
         lpDomain = zGETPTR( lpNextDomain ) )
   {
      zLPCONTEXT   lpCurrContext;
      zLPCONTEXT   lpNextContext;

      lpNextDomain = lpDomain->hNextDomain;

      if ( lpDomain->szName[ 0 ] == 0 )
      {
         fnDeleteContext( lpDomain );  // delete all contexts
         lpDomain = zGETPTR( lpApp->hFirstDomain );
         fnDeleteContext( lpDomain );
         lpApp->hFirstDomain = lpDomain->hNextDomain;
         fnFreeDataspace( lpDomain );
         if ( lpCurrDomain )
            lpCurrDomain->hNextDomain = lpNextDomain;
         else
            lpApp->hFirstDomain = lpNextDomain;

         continue;
      }

      lpCurrDomain = lpDomain;   // indicate curr domain as valid
      lpCurrContext = 0;   // last valid context, 0 means at head of queue
      for ( lpContext = zGETPTR( lpDomain->hFirstContext );
            lpContext;
            lpContext = zGETPTR( lpNextContext ) )
      {
         lpNextContext = lpContext->hNextContext;

         if ( lpContext->szName[ 0 ] == 0 )  // if no name, drop it!
         {
            // delete all table entries for this context
            while ( lpContext->hFirstTableEntry )
            {
               hTemp = lpContext->hFirstTableEntry;
               lpTableEntry = zGETPTR( lpContext->hFirstTableEntry );
               lpContext->hFirstTableEntry =
                                    zGETPTR( lpTableEntry->hNextTableEntry );
               fnDropTableEntry( lpContext, hTemp, 0 );
            }

            // Delete all regular expressions for this context.
            while ( lpContext->hFirstRegExpression )
            {
               lpRegExpression = zGETPTR( lpContext->hFirstRegExpression );
               lpContext->hFirstRegExpression =
                              zGETPTR( lpRegExpression->hNextRegExpression );
               fnFreeDataspace( lpRegExpression );
            }

            lpDomain->hFirstContext = lpContext->hNextContext;
            fnFreeDataspace( zGETPTR( lpContext->hEditString ) );
            fnFreeDataspace( lpContext );

            // Fix up the context chain.
            if ( lpCurrContext )
               lpCurrContext->hNextContext = lpNextContext;
            else
               lpDomain->hFirstContext = lpNextContext;

            continue;
         }

         lpCurrContext = lpContext; // indicate curr context as valid

// DGC 2000.11.06
// I removed following code to allow external values to be null.
#if 0
         // Context has a name.  Go check each table entry.
         for ( lpTableEntry = zGETPTR( lpContext->hFirstTableEntry );
               lpTableEntry;
               lpTableEntry = zGETPTR( lpNextTableEntry ) )
         {
            lpNextTableEntry = lpTableEntry->hNextTableEntry;

            if ( lpDomain->cType == zTYPE_INTEGER ||
                 lpDomain->cType == zTYPE_DECIMAL )
            {
               if ( lpTableEntry->hExternalValue == 0 &&
                    lpTableEntry->unInternal.lInteger != lNullInteger )
               {
                  zCHAR szWork[ 16 ];

                  zltoa( lpTableEntry->unInternal.lInteger, szWork );
                  lpTableEntry->hExternalValue =
                   fnStoreStringInDataspace( AnchorBlock->hMainFirstDataHeader,
                                             szWork );
               }

               continue;
            }

            if ( lpTableEntry->hExternalValue == 0 )
            {
               if ( lpTableEntry->unInternal.hStringValue )
               {
                  lpTableEntry->hExternalValue =
                                       lpTableEntry->unInternal.hStringValue;
               }
            }
         }
#endif
      }

   } // while ( lpDomain )...

   fnSysCloseFile( lpTask, hFile, 0 ); // Close the domain file

   // Add created app to anchor block list (under the ANCHORBLOCK mutex).
   lpApp->hNextApp = AnchorBlock->hFirstApp;
   AnchorBlock->hFirstApp = hApp;

   return( lpApp );         // return the application
}

//./ ADD NAME=fnDropTableEntry
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDropTableEntry
//
//  PURPOSE:    To delete a table entry struct and optionally maintain
//              the linkage in the table chain
//
//  PARAMETERS: lpContext      - The Context from which the table entry is
//                               being deleted.
//              lpTableEntry   - the table entry to delete
//              nMaintainLinks - 1 maintain linkage
//                               0 just free up stuff
//
//  RETURNS:    nothing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
void
fnDropTableEntry( zLPCONTEXT   lpContext,
                  LPTABLEENTRY lpTableEntry,
                  zSHORT       nMaintainLinks )
{
   LPNLSTABLEENTRY lpNLS_TableEntry;
   LPNLSTABLEENTRY lpNextNLS_TableEntry;
   LPDOMAIN        lpDomain;

   // Drop the NLS stuff.
   for ( lpNLS_TableEntry = zGETPTR( lpTableEntry->hFirstNLS_TableEntry );
         lpNLS_TableEntry;
         lpNLS_TableEntry = zGETPTR( lpNextNLS_TableEntry ) )
   {
      lpNextNLS_TableEntry = lpNLS_TableEntry->hNextNLS_TableEntry;

      if ( lpNLS_TableEntry->pszNLS_Text )
         fnFreeDataspace( zGETPTR( lpNLS_TableEntry->pszNLS_Text ) );

      fnFreeDataspace( lpNLS_TableEntry );
   }

   lpDomain = zGETPTR( lpContext->hDomain );
   if ( lpDomain->cType != zTYPE_INTEGER &&
        lpDomain->cType != zTYPE_DECIMAL )
   {
      if ( lpTableEntry->hExternalValue ==
                          lpTableEntry->unInternal.hStringValue )
      {
         fnFreeDataspace( zGETPTR( lpTableEntry->unInternal.hStringValue ) );
      }
      else
      {
         fnFreeDataspace( zGETPTR( lpTableEntry->hExternalValue ) );
         fnFreeDataspace( zGETPTR( lpTableEntry->unInternal.hStringValue ) );
      }
   }
   else
      fnFreeDataspace( zGETPTR( lpTableEntry->hExternalValue ) );

   if ( nMaintainLinks )
   {
      LPTABLEENTRY lpTempTable;

      if ( lpTableEntry->hNextTableEntry )
      {
         // not the tail of the chain

         lpTempTable = zGETPTR( lpTableEntry->hNextTableEntry );
         lpTempTable->hPrevTableEntry = lpTableEntry->hPrevTableEntry;

         if ( lpTableEntry->hPrevTableEntry )
         {
            // not the head of the chain
            lpTempTable = zGETPTR( lpTableEntry->hPrevTableEntry );
            lpTempTable->hNextTableEntry = lpTableEntry->hNextTableEntry;
         }
         else
            // head of the chain
            lpContext->hFirstTableEntry = lpTableEntry->hNextTableEntry;
      }
      else
      {
         // deleting the tail of the chain
         lpContext->hLastTableEntry = lpTableEntry->hPrevTableEntry;
         if ( lpTableEntry->hPrevTableEntry )
         {
            // not the only entry in the chain
            lpTempTable = zGETPTR( lpTableEntry->hPrevTableEntry );
            lpTempTable->hNextTableEntry = 0;
         }
         else
         {
            // only entry in the chain
            lpContext->hFirstTableEntry = 0;
         }
      }
   }

   fnFreeDataspace( lpTableEntry );
}

//./ ADD NAME=fnDeleteContext
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDeleteContext
//
//  PURPOSE:    To delete all contexts and their subobjects, for a given
//              domain.
//
//  PARAMETERS: lpDomain  - The Domain from which the contexts are being
//                          deleted.
//
//  RETURNS:    0 - Context deleted
//             -1 - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnDeleteContext( LPDOMAIN lpDomain )
{
   while ( lpDomain->hFirstContext )
   {
      zLPCONTEXT      lpContext;
      LPTABLEENTRY    lpTableEntry;
      LPREGEXPRESSION lpRegExpression;

      // Delete all table entries for this context.
      lpContext = zGETPTR( lpDomain->hFirstContext );
      while ( lpContext->hFirstTableEntry )
      {
         lpTableEntry = zGETPTR( lpContext->hFirstTableEntry );
         lpContext->hFirstTableEntry = lpTableEntry->hNextTableEntry;
         fnDropTableEntry( lpContext, lpTableEntry, 0 );
      }

      // Delete all regular expressions for this context.
      while ( lpContext->hFirstRegExpression )
      {
         lpRegExpression = zGETPTR( lpContext->hFirstRegExpression );
         lpContext->hFirstRegExpression = lpRegExpression->hNextRegExpression;
         fnFreeDataspace( lpRegExpression );
      }

      lpDomain->hFirstContext = lpContext->hNextContext;
      fnFreeDataspace( zGETPTR( lpContext->hEditString ) );
      fnFreeDataspace( lpContext );
   }

   return( 0 );
}

zSHORT
fnDropTaskDomain( LPTASK lpTask, LPTASKDOMAIN lpTaskDomain );
zSHORT
fnDropSubtask( LPTASK lpTask, LPSUBTASK lpSubtask, zSHORT nCleanup );

//./ ADD NAME=fnDeleteApp
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   fnDeleteApp
//
//  PURPOSE:    To delete application information when no longer
//              in use by any application tasks or subtasks.
//
//  PARAMETERS: lpApp            - The application to delete
//
//  We don't need a mutex here because this function is called only
//  from DeleteTask and fnDropSubtask.  These two functions both set
//  the ANCHORBLOCK mutex.
//
//  RETURNS:    0 - App deleted
//              1 - Tasks still using application
//             -1 - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnDeleteApp( LPAPP lpApp )
{
   LPAPP          lpPrevApp;
   LPAPP          hApp;
   LPTASK         hTask;
   LPTASK         lpTask;
   LPDBHANDLER    lpDBHandler;
   LPGKHANDLER    lpGKHandler;
   zVIEW          lpView;
   zVIEW          vTemp;
   LPVIEWNAME     lpViewName;
   LPSUBTASK      lpSubtask;
   LPLNAME        lpLName;
   LPTASKDOMAIN   lpTaskDomain;
   LPTASKDOMAIN   lpPrevTaskDomain;
   LPDOMAIN       lpDomain;
   LPVIEWOI       lpViewOI;
   zPCHAR         pchAppName;

   if ( AnchorBlock == 0 )
      return( -1 );

   if ( lpApp == 0 || lpApp->nTableID != iApp )
      return( -1 );

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
   {
      TraceLine( "DeleteApp for App ===================>>>> %s %s",
                 lpApp->szName, lpApp->bKeepAlive ? "(KeepAlive)" : "" );
   }

   // Some apps never get dropped, even if no tasks reference them.  Check
   // to see if this is one of them and just return if it is.
   if ( lpApp->bKeepAlive )
      return( 1 );

   hApp = zGETHNDL( lpApp );
   pchAppName = lpApp->szName;

   // Loop through the tasks.  Note that we don't need to worry about a
   // task being deleted while we are here because we are protected by
   // the ANCHORBLOCK mutex.
   hTask = AnchorBlock->hFirstTask;
   while ( (lpTask = zGETPTR( hTask )) != 0 )
   {
      // Kick out if task is connected to app.
      if ( lpTask->hApp == hApp )
      {
         if ( AnchorBlock->TraceFlags.bTaskMsgs )
            TraceLineS( "fnDeleteApp found Tasks linked to App: ", lpApp->szName );

         return( 1 );
      }

      // Make sure that no task is using a View OI for one of the object's
      // view instances.
      lpView = zGETPTR( lpTask->hFirstView );
      while ( lpView )
      {
         if ( lpView->hViewOD )
         {
            LPVIEWOD lpViewOD = zGETPTR( lpView->hViewOD );

            // Kick out if ViewOD is connected to app.
            if ( lpViewOD->hApp == hApp )
            {
               if ( AnchorBlock->TraceFlags.bTaskMsgs )
               {
                  TraceLine( "fnDeleteApp found ViewOD (%s) for task: 0x%08x  linked to App: %s",
                             lpViewOD->szName, zGETHNDL( lpTask ), lpApp->szName );
               }

               return( 1 );
            }
         }

         lpView = zGETPTR( lpView->hNextView );
      }

      // Make sure none of the active subtasks is using the application to
      // be deleted.
      lpSubtask = zGETPTR( lpTask->hFirstSubtask );
      while ( lpSubtask )
      {
         // Kick out if subtask is connected to app.
         if ( lpSubtask->hApp == hApp )
         {
            if ( hTask == AnchorBlock->hMainTask )
            {
               // Clean up any subtasks for this app in the Main task.
               fnDropSubtask( lpTask, lpSubtask, -1 );
               lpSubtask = zGETPTR( lpTask->hFirstSubtask );  // restart
               continue;
            }
            else
            {
               if ( AnchorBlock->TraceFlags.bTaskMsgs )
                  TraceLineS( "fnDeleteApp found Subtasks linked to App: ", lpApp->szName );

               return( 1 );
            }
         }

         lpSubtask = zGETPTR( lpSubtask->hNextSubtask );
      }

      if ( lpView || lpSubtask )
         return( 1 );

      // Check this for leak ... dks 2005.12.01
      lpPrevTaskDomain = 0;
      lpTaskDomain = zGETPTR( lpTask->hFirstTaskDomain );
      while( lpTaskDomain )
      {
         lpDomain = zGETPTR( lpTaskDomain->hDomain );
         if ( lpDomain->hApp == hApp )
         {
            if ( hTask == AnchorBlock->hMainTask )
            {
               // Clean up any domains for this app in the Main task.
               fnDropTaskDomain( lpTask, lpTaskDomain );
               lpTaskDomain = zGETPTR( lpTask->hFirstTaskDomain );  // restart
               continue;
            }
            else
            {
               if ( AnchorBlock->TraceFlags.bTaskMsgs )
                  TraceLineS( "fnDeleteApp found Domains linked to App: ", lpApp->szName );

               return( 1 );
            }
         }

         lpPrevTaskDomain = lpTaskDomain;
         lpTaskDomain = zGETPTR( lpTaskDomain->hNextTaskDomainForTask );
      }

      hTask = lpTask->hNextTask;
   }

   // If lpTask is set, then the application is still in use either by
   // a view in a task chain OR a subtask still active in a task chain.
   // Return that the application is still in use!
   if ( lpTask )    // i don't think it is possible to get here ... but who
   {                // am i to question such things ... dks  2005.05.04
      SysMessageBox( 0, "DKS", "IMPOSSIBLE!!!", -1 );
      return( 1 );
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) About to delete App: ", pchAppName );

   // Even though the application is not in use, there may yet be instances
   // tied to the application. Go through all ViewOI's in all tasks and
   // delete any instances which are for this application.
   for ( lpTask = zGETPTR( AnchorBlock->hFirstTask );
         lpTask;
         lpTask = zGETPTR( lpTask->hNextTask ) )
   {
      // Make sure that no task is using a View OI for one of the object's
      // view instances.
      lpViewOI = zGETPTR( lpTask->hFirstViewOI );
      while ( lpViewOI )
      {
         LPVIEWOD lpViewOD = zGETPTR( lpViewOI->hViewOD );

         if ( lpViewOD->hApp == hApp )
         {
            LPVIEWCSR lpViewCsr = zGETPTR( lpViewOI->hFirstViewCsr );

            lpView   = zGETPTR( lpViewCsr->hView );
            lpViewOI = zGETPTR( lpViewOI->hNextViewOI );
            fnDropObjectInstance( lpView );
         }
         else
            lpViewOI = zGETPTR( lpViewOI->hNextViewOI );
      }
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping Views for App: ", pchAppName );

   // Drop all the view names chained to the application.
   while ( lpApp->hFirstViewName )
   {
      lpViewName = zGETPTR( lpApp->hFirstViewName );

      // Unchain the current view name.
      lpApp->hFirstViewName = lpViewName->hNextViewName;

      // Decrement the name count in the View referenced by the ViewName.
      vTemp = zGETPTR( lpViewName->hView );
      vTemp->nViewNames--;

      // Free up storage for name string and the ViewName itself.
      fnFreeDataspace( lpViewName );
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping View Names for App: ", pchAppName );

   // Drop all the views chained to the application.
   while ( lpApp->hFirstView )
   {
      fnDropView( zGETPTR( lpApp->hFirstView ) );
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping App Views for App: ", pchAppName );

   // Clean up application domains.
   while ( lpApp->hFirstDomain )
   {
      lpDomain = zGETPTR( lpApp->hFirstDomain );
      fnDeleteContext( lpDomain );
      lpApp->hFirstDomain = lpDomain->hNextDomain;
      fnFreeDataspace( lpDomain );
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping Domains for App: ", pchAppName );

   // Clean up logical names.
   while ( lpApp->hFirstLName )
   {
      lpLName = zGETPTR( lpApp->hFirstLName );
      lpApp->hFirstLName = lpLName->hNextLName;
      fnFreeDataspace( lpLName );
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping Logical Names for App: ", pchAppName );

   // Clean up view Objects.
   while ( lpApp->hFirstViewOD )
      fnDeleteViewObject( zGETPTR( lpApp->hFirstViewOD ) );

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping ViewODs for App: ", pchAppName );

   // Clean up application DB Handlers.
   while ( lpApp->hFirstDBHandler )
   {
      lpDBHandler = zGETPTR( lpApp->hFirstDBHandler );
      lpApp->hFirstDBHandler = lpDBHandler->hNextDBHandler;
      fnFreeDataspace( lpDBHandler );
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping DB Handlers for App: ", pchAppName );

   // Clean up application GK Handlers.
   while ( lpApp->hFirstGKHandler )
   {
      lpGKHandler = zGETPTR( lpApp->hFirstGKHandler );
      lpApp->hFirstGKHandler = lpGKHandler->hNextGKHandler;
      fnFreeDataspace( lpGKHandler );
   }

   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) Finished dropping GK Handlers for App: ", pchAppName );

   // Unchain the application from the Anchor Block.
   if ( AnchorBlock->hFirstApp == hApp )
      AnchorBlock->hFirstApp = lpApp->hNextApp;
   else
   {
      LPAPP hApp2;

      // It is possible that lpApp is non-zero and also not in the chain
      // beginning at AnchorBlock->hFirstApp.  We had that situation
      // when zeidon.xdm was missing.  We must handle that, otherwise
      // the engine crashes.
      lpPrevApp = zGETPTR( AnchorBlock->hFirstApp );
      while ( (hApp2 = lpPrevApp->hNextApp) != 0 )
      {
         if ( hApp2 == hApp )
         {
            lpPrevApp->hNextApp = lpApp->hNextApp;
            break;
         }

         lpPrevApp = zGETPTR( lpPrevApp->hNextApp );
      }

//    while ( lpPrevApp && lpPrevApp->hNextApp != hApp )
//       lpPrevApp = zGETPTR( lpPrevApp->hNextApp );

      // It's possible the app wasn't added to the chain so be careful ...
//    if ( lpPrevApp )
//       lpPrevApp->hNextApp = lpApp->hNextApp;
   }

   // Clean up application.
   if ( AnchorBlock->TraceFlags.bTaskMsgs )
      TraceLineS( "(APP) DeleteApp SUCCESSFUL for App: ", lpApp->szName );

   fnFreeDataspace( lpApp );
   return( 0 );
}

//./ ADD NAME=SetDatabaseUser
// Source Module=kzoeapaa.c
/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION:   SetDatabaseUser
//
//  PURPOSE:    To assign user/password to a database. The database is
//              identified by a view passes as first parameter.
//              The appropriate DB Handler is called. Rather than
//              SysSetDB_UserID this method is able to maintain
//              different databases/users at the same time.
//
//  PARAMETERS: lpView      - the view, XOD saved with the database TE
//              cpcUser     - user ID
//              cpcPassword - password
//
//  RETURNS:    0 - ok
//              1 - not processed because of running transaction
//              zCALL_ERROR - Error in call
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT OPERATION
SetDatabaseUser( zVIEW lpView, zCPCHAR cpcUser, zCPCHAR cpcPassword )
{
   LPTASKDBHANDLER lpTaskDBHandler;
   LPTASK          lpCurrentTask;
   LPVIEWOD        lpViewOD;
   zPDBHOPER       pOper;
   zVIEW           lpQualObj = 0;
   zPVOID          pvWork;
   zSHORT          nRC;

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iSetDatabaseUser, lpView, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   nRC = SfActivateSysEmptyOI( &lpQualObj, "KZDBHQUA", lpView, zMULTIPLE );
   if ( nRC < 0 )
   {
      nRC = zCALL_ERROR;
      goto EndOfFunction;
   }

   if ( cpcUser == 0 || zstrlen( cpcUser ) > 32 ||
        cpcPassword == 0 || zstrlen( cpcPassword ) > 32 ||
        (lpTaskDBHandler = fnGetDBHandlerOper( lpCurrentTask, lpView, &pOper )) == 0 )
   {
      nRC = zCALL_ERROR;
      goto EndOfFunction;
   }

   TraceLine( "SetDatabaseUser: %s for task: 0x%08x", cpcUser, lpCurrentTask );

   CreateEntity( lpQualObj, "EntitySpec", zPOS_FIRST );

   // Store user in EntityName.
   SetAttributeFromString( lpQualObj, "EntitySpec", "EntityName", cpcUser );

   // Store password in Options.
   SetAttributeFromString( lpQualObj, "EntitySpec", "Options", cpcPassword );

   lpViewOD = zGETPTR( lpView->hViewOD );
   nRC = (*pOper)( (zLONG) lpView->hTask,
                   DBH_Database | DBH_SetUser, 0, lpViewOD,
                   0, lpView, lpQualObj, &pvWork );

   // nRC = 0 ==> success
   // nRC = 1 ==> unsuccessful, can't close DB because of transaction running
   if ( nRC < 0 )
   {
      nRC = zCALL_ERROR;
      goto EndOfFunction;
   }

EndOfFunction:
   if ( lpQualObj )
      DropView( lpQualObj );

   fnOperationReturn( iSetDatabaseUser, lpCurrentTask );
   return( nRC );
}
