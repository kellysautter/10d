/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmpmaa.c - VML SubSystem Parser Main Module
// DESCRIPTION:   Setup/ Initialization/ Parse Call
//                This is the source file which contains the main functions
//                for the parser. It contains the functions to do all of
//                the setup as well as start the parse.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/07/13
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.9A   1995/08/30
//
// LAST MESSAGE ID: VM01008
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2002.08.29  DGC
   Added an argument to ParseSource().
2002.05.08 HH
   reset scanner after fatal error exit.
2002.03.15 HH
   support fatal error exit.
2002.03.06  HH
   improve recognition of Global Operations.
2002.02.28  DKS
   Remove warnings.
2002.02.04  DGC
   When XPG is created, it is given the same timestamp as the VML file so that
   we can better determine when a VML file needs to be parsed.
2001.11.15  HH
   R55920, set g_szSourceMetaObjectName.
2001.09.09  HH
   initially delete generated operations from source meta.
2001.08.28 HH
   added PrintXPG.
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.25  SR  Z2000  Length of path variables
   Modified the size of file specification variables in function ParseVMLSource
   and ParseSource because size of attributes ExecDir, MetaSrcDir and PgmSrcDir
   has been changed to 254 in datamodel.
   Adjust the size of messagetext variables.

2000.09.21  HH
   fix bug with empty source file.

2000.08.31  HH
   added ParserVersion attribute to TZVSPOOO (xpg).

OLD CHANGE LOG
13.06.1997  DonC
   Eliminated reuse of existing XPG as it sometimes created problems and
   seemed not to help performance.
21.11.1997  HH
   Remove Tabs

14.03.1998  DonC
   Initialized variable g_bActivate;

04.11.1998  HH
   Error Message, if temp file cannot be written.

30.12.1998  HH
   Restructured German Umlaut Hack.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.H"


#define LENWORK 256

zPCHAR szMessageListParse[ ] =
      { "1 -Profile Object could not be accessed, Parse Terminating",
        "2 -Expression Object could not be accessed, Parse Terminating",
        "3 -Application's Global Operation List could not be accessed from"
            "Configuration Management, Parse Terminating",
        "4 -TaskLPLR could not be accessed, Parse Terminating",
        "5 -TZOPSIGK.XGO could not be accessed, Parse Terminating",
        "6 -Could not get a list of LODs, Parse Terminating",
        "7 -Could not Load Error Object, Parse Terminating",
        "8 -Source file Meta could not be loaded, parse terminating",
        "9 -Could not get a list of Registered Views, Parse Terminating",
        "10 -The expression is too big for the LocalCode work area",
        "11 -Could not write to temp file, Parse Terminating"
      };

static zPCHAR RemoveTabs( zPCHAR szWork, zLONG lMaxLth );
static zVOID
fnDeleteGenerated( zVIEW vSourceMeta );
static zVOID
fnPrintOperation( zVIEW vXPG );
static zVOID
fnPrintVariable( zVIEW vXPG );
static zVOID
fnPrintStatement( zVIEW vXPG, zLONG lLevel );
static zVOID
fnPrintExpression( sQExprStruct *pWorkExpr, zLONG lLevel );
static zVOID
fnPrintOperator( sQExprStruct *pWorkExpr, zCPCHAR szIndent );


/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS: ParseVMLSource
//             ParseSource
//             PositionAtOperationInSource
//             DeleteAllOpersForSource
//             DeleteOneOperForSource
//             InitReadBuffer
//             InitGlobalDataForParse
//             InitErrorObject
//             InitExpressionObject
//             GetTextLine
//             SaveTextLine
//             GetProfileData
//             ReInitOperation
//             CompareFileToXPG
//             SkipRemainingFile
//             IncludeCoreHeader
//
/////////////////////////////////////////////////////////////////////////////

//
// OPERATION: ParseVMLSource
//
// DESCRIPTION: ParseVMLSource is the main driving function for the parser.
//    It is passed a Subtask View and the name of the file to be parsed. This
//    function takes this name and does several things. First it activates
//    an empty object instance for the parsed vml object (TZVSPOOO). All
//    of the time saving mechanisms must be done prior to this. That is,
//    if the fast parsed object (or XPG) already exists, then it should
//    have been loaded prior to this. Once this function is called the
//    VML source will be parsed. Once the empty object instance is activated,
//    the parsing begins to build the instance. If the parsing is successful,
//    then the parsed object is stored as a fast file. If errors exist, the
//    instance is deleted. ParseVMLSoiurce initializes the global variables
//    necessary to parse then calls qqparse (output of yacc.).
//
//    The file name that has been passed in can be either the name of the
//    source member to be parsed or, it could be the pointer to the
//    string already. (this allows for faster parsing if, say, the string
//    of text is already in memory; perhaps in the editor). Whether or not
//    this is the file name or a pointer to the string depends on the
//    nOpenMode parameter. If it is 0 then it is a file name, otherwise
//    it is a 1.
//
zOPER_EXPORT zLONG OPERATION
ParseVMLSource( zVIEW  vVML_Subtask,      /* pointer to the vml task */
                zVIEW  vSubtask,          /* pointer to the appl task */
                zPCHAR szSrcDirectory,    /* name of the text file to parse */
                zPCHAR szFileName,        /* name of the text file to parse */
                zCPCHAR cpcGenLang )
{
   zCHAR  szSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szSourceFileName[ 33 ];
   zCHAR  szXPGFileName[ 33 ];
   zCHAR  szOperationName[ 33 ];
   zLONG  lRC; /* a return code to test the returns of operation calls */

   GetProfileData( vSubtask, szSrcDir, szSourceFileName,
                   szXPGFileName, szOperationName );

   lRC = ParseSource( vVML_Subtask, 0,     /* pointer to the vml task */
                      g_lParseAction, g_lParseLimit,
                      g_lParseSource, szSourceFileName,
                      szOperationName, 0, 0, cpcGenLang );

   return( lRC );
}

// lControl - Allows caller to specify what to do with the XPG:
// zPARSE_DONOTWRITEXPG - Doesn't drop the XPG object AND doesn't write it to a file.
//
zOPER_EXPORT zLONG OPERATION
ParseSource( zVIEW  vVML_Subtask,
             zVIEW  vInSourceMeta,
             zLONG  lIlParseAction,
             zLONG  lIlParseLimit,
             zLONG  lIlParseSource,
             zPCHAR szSourceFileName,
             zPCHAR szInOperationName,
             zLONG  lMetaType,
             zLONG  lControl,
             zCPCHAR cpcGenLang )
{
   zVIEW  vLPLR;
   zCHAR  szSrcDirectory[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szSrcDirectory2[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szFullXPGPathName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szErrorFile[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szVML_DateTime[ 30 ];
   zCHAR  szXGODirectory[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szOperationName[ 33 ];
   zCHAR  szXPGFileName[ 33 ];
   zCHAR  szOpGenType[ 2 ];
   zVIEW  vError;
   zVIEW  vViewObject;
   zLONG  lCompare;
   zLONG  lZKey;
   zLONG  lOrigFile;
   zLONG  lNewFile;
   zLONG  lLineCount = 0;
   zLONG  lRC;  // a return code to test the returns of operation calls

   g_lfHandle = 0;

   g_szGenLang[ 0 ] = cpcGenLang[ 0 ];
   g_szGenLang[ 1 ] = 0;

   //------------------------------------ Step One --------------------------------------------//
   // Initialize a global subtask so that every module in the parse has access to the parser subtask.
   //----------------- Step One ------------------------------//
   lpGlobalSubtask = vVML_Subtask;

   //------------------------------------ Step Two --------------------------------------------//
   // Initialize all of the global data that the parser needs to run.
   InitGlobalDataForParse( );

   // Set the globals that need to be set from parameters.  This is redundant if from the
   // ParseVMLSource call.
   g_lParseAction = lIlParseAction;
   g_lParseLimit = lIlParseLimit;
   g_lParseSource = lIlParseSource;

   //----------------------------------- Step Three -------------------------------------------//
   // Get All of the foreign information that is needed to parse ... file names directories etc.

   // DonC comment, 08/24/95.  We are currently assuming that the view of the source meta (which
   // would be the LOD or the Dialog) is always passed in.  We need to re-evaluate later.

   // Activate the source meta to get the other necessary information.  vSource meta could be sent
   // in from the Editor (or another calling tool) This means that they have it checked out
   // therefore the parser cannot check it out, so use the view sent in.

   // If it was not sent in then it's an error.
   if ( vInSourceMeta == 0 )
   {
      MessageSend( lpGlobalSubtask, "VM01001", "VML Parser",
                   szMessageListParse[ 7 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
   {
      g_vSourceMeta = vInSourceMeta;  // else use the one passed
   }

   SetNameForView( g_vSourceMeta, "vParseMeta", lpGlobalSubtask, zLEVEL_TASK );

   // Get the Directory for the actual source file text.  The directory information is found
   // in the root of the CM LPLR object.  The LPLR object exists by name, "TaskLPLR".
   // DM - NOTE: szSrcDirectory will carry the full source file name. Bad naming - should get fixed sometime.
   GetViewByName( &vLPLR, "TaskLPLR", vVML_Subtask, zLEVEL_TASK );
   GetStringFromAttribute( szSrcDirectory2, sizeof( szSrcDirectory2 ), vLPLR, "LPLR", "PgmSrcDir" );
   SysConvertEnvironmentString( szSrcDirectory, sizeof( szSrcDirectory ), szSrcDirectory2 );
   SysAppendcDirSep( szSrcDirectory );
   strcpy_s( szSrcDirectory2, sizeof( szSrcDirectory2 ), szSrcDirectory );

   // DM - Is this doing what it is supposed to?  Save the operation name.
   strcpy_s( szOperationName, sizeof( szOperationName ), szInOperationName );

   // Fill out directory for error file.
   strcpy_s( szErrorFile, sizeof( szErrorFile ), szSrcDirectory );

   // Stick in the file name.  DM - I am assuming the file name was properly passed in!!
   strcat_s( szSrcDirectory, sizeof( szSrcDirectory ), szSourceFileName );
   strcat_s( szErrorFile, sizeof( szErrorFile ), szSourceFileName );

   // Tack on extensions.
   strcat_s( szSrcDirectory, sizeof (szSrcDirectory ), ".VML" );
   strcat_s( szErrorFile, sizeof( szErrorFile ), ".ERR" );

   // Get the dir name of the Zeidon tools so that we get the XGO directory.
   zgGetZeidonToolsDir( vVML_Subtask, szXGODirectory, zAPPL_DIR_OBJECT );

   // The name of the system core operation list.
   strcat_s( szXGODirectory, sizeof( szXGODirectory ), "TZOPSIGK.XGO" );

   // Get a view to the TaskLPLR to get the executable dir off of the TaskLPLR Entity. Never
   // Never Drop this View.
   lRC = GetViewByName( &vViewObject, "TaskLPLR", vVML_Subtask, zLEVEL_TASK );

   // If no LPLR exists get out now.
   if ( lRC != zLEVEL_TASK )
   {
      MessageSend( lpGlobalSubtask, "VM01002", "VML Parser",
                   szMessageListParse[ 3 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Get the ExecDir off of the LPLR Entity.  This will be the directory where the xpg is now or will be saved to.
   GetStringFromAttribute( szFullXPGPathName, sizeof( szFullXPGPathName ), vViewObject, "LPLR", "ExecDir" );

   // Be sure the dir name is terminated with the delimiter.
   SysAppendcDirSep( szFullXPGPathName );

   // For Global Operations and Domain Operations, a work object (type LOD) for the source meta was created and is to be used here
   // instead of the vInSourceMeta passed to the Parser.
   g_szSourceMetaObjectName[ 0 ] = 0;
   if ( lMetaType == zSOURCE_DOMAINGRP_META || lMetaType == zREFER_DOMAINGRP_META )
   {
      // Should never come here, domain operations are not VML.
      GetViewByName( &g_vSourceMeta, "DGP_SourceFile", vVML_Subtask, zLEVEL_TASK );
      strcpy_s( g_szSourceMetaObjectName, sizeof( g_szSourceMetaObjectName ), "TZDGSRCO" );
   }
   else
   if ( lMetaType == zSOURCE_GOPGRP_META || lMetaType == zREFER_GOPGRP_META )
   {
      GetViewByName( &g_vSourceMeta, "GOP_SourceFile", vVML_Subtask, zLEVEL_TASK );
      strcpy_s( g_szSourceMetaObjectName, sizeof( g_szSourceMetaObjectName ), "TZOGSRCO" );
   }
   else
   if ( lMetaType == zSOURCE_SRC_META || lMetaType == zREFER_SRC_META )
   {
      // Special code to recognize a Global Operation, if called from "parsgen".
      // We must examine the operation type, which is "G" for a Global Operation.
      zCHAR szOpType[ 2 ] = { 0 };
      GetStringFromAttribute( szOpType, sizeof( szOpType ), g_vSourceMeta, "Operation", "Type" );
      if ( szOpType[ 0 ] == 'G' )
      {
         // We recognized the fake.
         strcpy_s( g_szSourceMetaObjectName, sizeof( g_szSourceMetaObjectName ), "TZOGSRCO" );
      }
   }

   if ( g_szSourceMetaObjectName[ 0 ] == 0 )
   {
      // Store the type of the meta (TZZOLODO, TZWDLGSO, ...).
      MiGetObjectNameForView( g_szSourceMetaObjectName, g_vSourceMeta );
   }

   strcpy_s( szXPGFileName, sizeof( szXPGFileName ), szSourceFileName );
   strcat_s( szFullXPGPathName, sizeof( szFullXPGPathName ), szXPGFileName );

   // Concat the Extension onto the directory.
   if ( g_szGenLang[ 0 ] == 'J' )
      strcat_s( szFullXPGPathName, sizeof( szFullXPGPathName ), ".XPJ" );
   else
      strcat_s( szFullXPGPathName, sizeof( szFullXPGPathName ), ".XPG" );

   //-------------------------------------- Step Four -------------------------------------------//
   // Delete "generated" operations from source meta.  Set up support instance needed to parse
   // like error object expression object etc.
   fnDeleteGenerated( g_vSourceMeta );
   lRC = InitErrorObject( vVML_Subtask, szErrorFile, szSrcDirectory );

   // If we have an error, get out now.
   if ( lRC == -1 )
      return( -1 );

   // Activate a global expression to manipulate.
   lRC = InitExpressionObject( );

   // If we have an error, get out now.
   if ( lRC == -1 )
      return( -1 );

   //-------------------------------------- Step Five -------------------------------------------//
   // Get or create other needed views from the system such as Get the View to the Global
   // operation list, LOD list etc. from CM GLOBAL OPERATION LIST.
   lRC = RetrieveViewForMetaList( vVML_Subtask, &g_lpGOListView, zREFER_GO_META );

   // Failed, get out.
   if ( lRC < 0 )
   {
      // Drop all views already retrieved so they don't hang around.
      DropObjectInstance( g_lpExprView );
      g_lpGOListView = 0;
      MessageSend( lpGlobalSubtask, "VM01003", "VML Parser",
                   szMessageListParse[ 2 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // If there are no global operations, this is okay but drop the view and zero it out here
   // so we don't access it later.
   if ( CheckExistenceOfEntity( g_lpGOListView, "W_MetaDef" ) != zCURSOR_SET )
   {
      DropView( g_lpGOListView );
      g_lpGOListView = 0;
   }

   // Now that we have the name, try to load the operation list.
   lRC = ActivateOI_FromFile( &g_lpZOListView, "TZOPGRPO", vVML_Subtask, szXGODirectory, zSINGLE );

   if ( lRC < 0 ) //  failed
   {
      // Drop all views already retrieved so they dont hang around.
      g_lpZOListView = 0;
      if ( g_lpGOListView != 0 )
         DropView( g_lpGOListView );

      MessageSend( lpGlobalSubtask, "VM01004", "VML Parser",
                   szMessageListParse[ 4 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // KJS 04/23/13 - I am testing whether it is feasible to have two different zeidon operations one for c generation and one for java.
   // Loop through operations and delete the ones that are not for the language we are currently generating.
   lRC = SetCursorFirstEntity( g_lpZOListView, "Operation", "" );
   while ( lRC >= zCURSOR_SET )
   {
      szOpGenType[ 0 ] = 0;
      GetStringFromAttribute( szOpGenType, sizeof( szOpGenType ), g_lpZOListView, "Operation", "GenerationType" );
      if ( szOpGenType[ 0 ] == 'C' && g_szGenLang[ 0 ] == 'J' )
      {
         DropEntity( g_lpZOListView, "Operation", zREPOS_NONE );
      }
      else
      if ( szOpGenType[ 0 ] == 'J' && g_szGenLang[ 0 ] == 'C' )
      {
         DropEntity( g_lpZOListView, "Operation", zREPOS_NONE );
      }
       lRC = SetCursorNextEntity( g_lpZOListView, "Operation", "" );
   }

   // Get access to the meta list for all LODs LOD LIST.
   lRC = RetrieveViewForMetaList( vVML_Subtask, &g_vLODList, zREFER_LOD_META );
   if ( lRC < 0 ) //  failed
   {
      // Drop all views already retrieved so they don't hang around.
      DropObjectInstance( g_lpExprView );
      DropMetaOI( vVML_Subtask, g_lpGOListView );
      DropMetaOI( vVML_Subtask, g_lpZOListView );

      MessageSend( lpGlobalSubtask, "VM01005", "VML Parser",
                   szMessageListParse[ 5 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      return( -1 );
   }

   // Get access to the meta list for all VORs VOR LIST.
   lRC = RetrieveViewForMetaList( vVML_Subtask, &g_vVORList, zREFER_VOR_META );
   if ( lRC < 0 ) //  failed
   {
      // Drop all views already retrieved so they don't hang around.
      DropObjectInstance( g_lpExprView );
      DropMetaOI( vVML_Subtask, g_vLODList );
      DropMetaOI( vVML_Subtask, g_lpGOListView );
      DropMetaOI( vVML_Subtask, g_lpZOListView );

      MessageSend( lpGlobalSubtask, "VM01006", "VML Parser",
                   szMessageListParse[ 8 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      return( -1 );
   }

   // Initialize variables.
   g_bActivate = FALSE;

   ////////////////////////////////////////////////////////////////////////
   //                                                                    //
   // At this point we have init'ed the globals, created an expression   //
   // instance, gotten all of the profile data, loaded both Operation    //
   // lists and accessed the lod reference list and have created an      //
   // error object now we are ready to parse code                        //
   //                                                                    //
   ////////////////////////////////////////////////////////////////////////

   //-------------------------------------- Step Six --------------------------------------------//
   // Try to activate the pi (parsed instance) use the NOI_OKAY parameter to NOT get an error
   // message on the open if it does not exist.

   // *** Eliminate reuse of old XPG at this time because an error in an old XPG can crash the system. 6/13/97 DonC
   ActivateEmptyObjectInstance( &g_lpPIView, "TZVSPOOO", vVML_Subtask, zSINGLE );

   // Since we had no preexisting XPG, then we cannot parse incrementally therefore, what we need
   // to do is be sure the flags are set to parse full from a file.
   g_lParseLimit = zPARSE_LIMIT_SOURCE;
   g_lParseAction = zPARSE_ACTION_FULL;

   // Create the PI entities needed the root.
   CreateEntity( g_lpPIView, "VML_XSource", zPOS_AFTER );

   // Set the name.
   SetAttributeFromString( g_lpPIView, "VML_XSource", "Name", szXPGFileName );

#define PARSER_VERSION 1

   // Set The Parser version Attribute.  We start with version, 2000.08.31, Version 10d.  This is for
   // compatibility checking, so that interpreter and parser can be synchronized on parser version.
   SetAttributeFromInteger( g_lpPIView, "VML_XSource", "ParserVersion", PARSER_VERSION );
   {
      // The TimeStamp is not actually used, but might be in future
      zCHAR szTimestamp[ 22 ];
      SysGetDateTime( szTimestamp, sizeof( szTimestamp ) );
      SetAttributeFromString( g_lpPIView, "VML_XSource", "ParseTimeStamp", szTimestamp );
   }

   // Include the sourcemeta into the XPG object.
   IncludeSubobjectFromSubobject( g_lpPIView, "SourceFile", g_vSourceMeta, "SourceFile", zPOS_AFTER );

   // *** Eliminate reuse of old XPG at this time because an error in an old XPG can crash the system. 6/13/97 DonC

   // If the meta is a LOD, we want to put the LOD Name into the VML_XSource entity for use later
   // in generating the prefix for Operation Names.
   if ( zstrcmp( g_szSourceMetaObjectName, "TZZOLODO" ) == 0 )
   {
      SetAttributeFromAttribute( g_lpPIView, "VML_XSource", "MetaName", g_vSourceMeta, "LOD", "Name" );
   }

   // Now create the search global view to the PI.
   CreateViewFromViewForTask( &g_lpSGView,     // view we search with
                              g_lpPIView, 0 ); // view we create and delete with

   // *** TEMPORARY HACK ***
   ////////////////////////////////////////////////////////////////////////
   //                                                                    //
   // Copy the source file to a work file, changing German umlaut chars to
   // a | + the equivalent regular char.  Also get rid of any other chars
   // whose hex value is greater than 80, since the parser crashes if
   // such characters somehow got into the source.
   //                                                                    //
   ////////////////////////////////////////////////////////////////////////

   // Build the temporary file name. The directory structure was built above.
   strcat_s( szSrcDirectory2, sizeof( szSrcDirectory2 ), "z__temp.vml" );
   lOrigFile = SysOpenFile( vVML_Subtask, szSrcDirectory, COREFILE_READ );

   // Convert lines from original vml file to temp vml file.
   if ( lOrigFile >= 0 )
   {
      zPCHAR pchLineIn;
      zCHAR  szLineOut[ 256 ];

      lNewFile = SysOpenFile( vVML_Subtask, szSrcDirectory2, COREFILE_WRITE );
      if ( lNewFile < 0 )
      {
         // Error opening temp file for write
         MessageSend( lpGlobalSubtask, "VM01011", "VML Parser",
                      szMessageListParse[ 10 ],
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
      else
      while ( SysReadLine( vVML_Subtask, &pchLineIn, lOrigFile ) == 1 )
      {
         // Copy the input line to the output line, converting German umlaut characters to |x and eliminating other characters with hex value > 80.
         GermanUmlautHack( pchLineIn, szLineOut, sizeof( szLineOut ) - 1, FALSE );
         SysWriteLine( vVML_Subtask, lNewFile, szLineOut );
         lLineCount++;
      }

      SysCloseFile( vVML_Subtask, lNewFile, 0 );
      SysCloseFile( vVML_Subtask, lOrigFile, 0 );
   }
   else
   {
      // Right now we had better just return if there is an open error.
      return( 0 );
   }

   // If the file is empty - just return.
   if ( lLineCount <= 0 )
      return( 0 );

   //------------------------------------- Step Seven -------------------------------------------//
   // Set up the read buffer to begin reading.  Be sure the Source file can be checked out as SOURCE not REFER.
   lRC = InitReadBuffer( vVML_Subtask, szSrcDirectory2,  // fully qualified source name
                         g_lParseSource );   // parse amount flag
   if ( lRC == -1 )
   {
      // On error drop all views already retrieved so they dont hang around.
      DropView( g_lpSGView );
      DropObjectInstance( g_lpPIView );
      DropView( g_vGlobalView );
      DropObjectInstance( g_lpExprView );

      if ( g_lpGOListView )
         DropMetaOI( vVML_Subtask, g_lpGOListView );

      DropMetaOI( vVML_Subtask, g_lpZOListView );
      SysCloseFile( vVML_Subtask, g_lfErrorHandle, 0 );
      return( -1 );
   }

   // Verify that the core header file was included at some point during the parse. It is needed for all source to minimally define some
   // data types even if it was never referenced in the code.  Never being referenced is rare but could possibly happen.
   lRC = IncludeCoreHeader( g_lpPIView, "KZOENGAA" );

   ////////////////////////////////////////////////////////////////////////
   //                                                                    //
   // Ready to do the actual parsing.                                    //
   //                                                                    //
   ////////////////////////////////////////////////////////////////////////

   // TRY to parse ==========>

   if ( setjmp( jbFatalError ) == 0 )
   {
   //------------------------------------- Step Eight -------------------------------------------//
   // Finally we actually begin parsing.  Begin parsing based on what flags are set.  Switch based on how much code we are going to deal with.
   switch( g_lParseLimit )
   {

      case zPARSE_LIMIT_SIGNATURES:  // only signatures
      {
         // switch based on what type of parse we are going to deal with
         switch( g_lParseAction )
         {
            // if only incremental, update sig that have changed
            case zPARSE_ACTION_INCREMENTAL:
               // update the operation entities only (i.e. update line numbers)
               g_lOperationFlag = 2;  // update
               GetTextLine( );
               SaveTextLine( );
               // one Pass only will create/update signatures
               qqparse( vVML_Subtask );
               break;

            // if full, delete all opers and recreate only signatures
            case zPARSE_ACTION_FULL:
               // delete all of the operations that exist for this source
               lRC = DeleteAllOpersForSource( g_lpPIView );

               // create all new signatures
               g_lOperationFlag = 1;  // create
               GetTextLine( );
               SaveTextLine( );

               // one Pass only will create/update signatures
               qqparse( vVML_Subtask );
               break;
         }

         break;
      }


      case zPARSE_LIMIT_OPERATION:  // do the operation, whose name was sent in
      {
         switch( g_lParseAction )
         {
            // check it first to see if the text changed
            case zPARSE_ACTION_INCREMENTAL:
            {
               // do the text comparison first.
               lRC = GetIntegerFromAttribute( &lZKey, g_lpPIView,
                                              "OperationSource", "ZKey" );
               lRC = PositionAtOperationInSource( lZKey );

               lCompare = CompareFileToXPG( lZKey );

               // if these was a change, then we need to reparse it.
               if ( lCompare != 0 )
               {
                  // first we need to lode the file,, reinit the  globals
                  TraceLineS( "Must Reparse", szOperationName );
                  SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
                  InitGlobalDataForParse( );

                  // reread the file/open it
                  InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );

                  // get position in the XPG and the source
                  lRC = PositionAtOperationInSource( lZKey );

                  // delete the old one, actually this deletes all of
                  // the statements, variables, and sets the operation
                  // attributes to null so that the ZKeyt does not change
                  // icase it was called locally
                  lRC = DeleteOneOperForSource( g_lpPIView );

                  // we are going to recreate the operation entity
                  g_lOperationFlag = 1;  // create
                  // see if we can get rid of these
                  g_lParseAction = zPARSE_ACTION_FULL;
                  g_lParseLimit = zPARSE_LIMIT_SIGNATURES;

                  SaveTextLine( );

                  // this should create the OperationTextEntity ???
                  qqparse( vVML_Subtask );

                  SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
                  InitGlobalDataForParse( );
                  g_lParseLimit = zPARSE_LIMIT_OPERATION;
                  g_lParseAction = zPARSE_ACTION_FULL;

                  InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );
                  lRC = PositionAtOperationInSource( lZKey );

                  g_lOperationFlag = 3;  // position
                  qqparse( vVML_Subtask );
                  break;
               }

               break;
            }

            // full parse of the operation
            case zPARSE_ACTION_FULL:
            {
               lZKey = 0;  // initialize to something DKS 2000.03.09?
               g_lOperationFlag = 1;  // create
               lRC = PositionAtOperationInSource( lZKey );
               lRC = DeleteOneOperForSource( g_lpPIView );

               g_lOldParseLimit = g_lParseLimit = zPARSE_LIMIT_SIGNATURES;

               qqparse( vVML_Subtask );

               SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
               InitGlobalDataForParse( );
               g_lParseLimit = zPARSE_LIMIT_OPERATION;
               g_lParseAction = zPARSE_ACTION_FULL;

               InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );
               lRC = PositionAtOperationInSource( lZKey );

               g_lOldParseLimit = zPARSE_LIMIT_OPERATION;
               g_lParseLimit = zPARSE_LIMIT_OPERATION;

               g_lOperationFlag = 3;  // position
               qqparse( vVML_Subtask );
               break;
            }
         }
      }


      case zPARSE_LIMIT_SOURCE:  // parse the entire source code
      {
         switch( g_lParseAction )
         {
            // Loop through all of the operations, delete the ones that have changed and fall through.
            case zPARSE_ACTION_INCREMENTAL :
            {
               // Position at the first operation.
               lRC = PositionAtOperationInSource( 0 );
               do
               {
                  lRC = GetIntegerFromAttribute( &lZKey, g_lpPIView, "OperationSource", "ZKey" );
                  lCompare = CompareFileToXPG( lZKey ); // zALL

                  if ( lCompare != 0 )
                  {
                     SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
                     InitGlobalDataForParse( );
                     InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );
                     lRC = PositionAtOperationInSource( lZKey );
                     lRC = DeleteOneOperForSource( g_lpPIView );

                     g_lOldParseLimit = zPARSE_LIMIT_OPERATION;
                     g_lParseLimit = zPARSE_LIMIT_OPERATION;
                     g_lParseAction = zPARSE_ACTION_FULL;

                     g_lOperationFlag = 1;  // create
                     SaveTextLine( );

                     qqparse( vVML_Subtask );

                     SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
                     InitGlobalDataForParse( );
                     g_lParseLimit = zPARSE_LIMIT_OPERATION;
                     g_lParseAction = zPARSE_ACTION_FULL;

                     InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );
                     lRC = PositionAtOperationInSource( lZKey );

                     g_lOperationFlag = 3;  // position

                     qqparse( vVML_Subtask );

                     SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
                     InitGlobalDataForParse( );
                     InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );
                  }
                  else
                  {
                     SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
                     InitGlobalDataForParse( );
                     InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );
                  }

                  lRC = PositionAtOperationInSource( 1 );

               } while ( lRC == zCURSOR_SET );

               break;
            }

            case zPARSE_ACTION_FULL:
            {
               // Delete all of the operations that exist for this source.
               lRC = DeleteAllOpersForSource( g_lpPIView );

               // Do operations only first ... then the full.
               g_lParseLimit = zPARSE_LIMIT_SIGNATURES;
               g_lOldParseLimit = g_lParseLimit;

               g_lOperationFlag = 1;  // create
               GetTextLine( );
               SaveTextLine( );

               qqparse( vVML_Subtask );

               g_lParseLimit = zPARSE_LIMIT_SOURCE;
               g_lOldParseLimit = g_lParseLimit;
               SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
               InitGlobalDataForParse( );
               InitReadBuffer( vVML_Subtask, szSrcDirectory2, g_lParseSource );
               lGlobalReadRetCode = GetTextLine( );
               lRC = ResetView( g_lpPIView );
               InitGlobalDataForParse( );
               g_lOperationFlag = 3;  // position
               qqlineno = 1;

               qqparse( vVML_Subtask );
               break;
            }
         }
      }
   }

   //-------------------------------------- Step Nine -------------------------------------------//
   // Clean up. Commit what is necessary and drop instances no longer needed reset the View to the top.
   lRC = ResetView( g_lpPIView );

// if ( 0 )
// {
//    SetNameForView( g_lpPIView, "AfterParse", vVML_Subtask, zLEVEL_TASK );
//    IssueError( vVML_Subtask, 0, 0, "AfterParse" );
// }

   // Don't write the XPG if the user doesn't want it.
   if ( (lControl & zPARSE_DONOTWRITEXPG) == 0 )
   {
      // store the XPG File
      lRC = CommitOI_ToFile( g_lpPIView, szFullXPGPathName, zASCII );

      // Give the XPG file the same timestamp as the VML file.  This will give us a more accurate guide to when the VML file needs to be parsed.
      lOrigFile = SysOpenFile( vVML_Subtask, szSrcDirectory,  COREFILE_READ );
      SysGetFileDateTime( lOrigFile, szVML_DateTime, sizeof( szVML_DateTime ) );
      SysCloseFile( vVML_Subtask, lOrigFile, 0 );
      SysSetFileTime( szFullXPGPathName, szVML_DateTime, 0 );
   }

   // print xpg if triggered by zeidon.ini  // dks
   {
      zCHAR szIni[ 16 ] = { 0 };
      SysReadZeidonIni( -1, "[Zeidon]", "PrintXPG", szIni, sizeof( szIni ) );

      if ( szIni[ 0 ] == 'Y' || szIni[ 0 ] == 'y' || szIni[ 0 ] == '1' )
         PrintXPG( g_lpPIView );
   }

   }
   else  // <========== CATCH fatal error exception
   {
      // Return from longjmp ... parse abborted due to fatal error.
      if ( g_lfHandle > 0 )
         SysCloseFile( vVML_Subtask, g_lfHandle, 0 );

      qq_reset(); // necessary for re-parse
      SysMessageBox( vVML_Subtask, "Parse aborted due to fatal error", g_szFatalErrorMsg, zBEEP );
   }

   // Drop the views ... drop the XPG Views and instance.
// DropMetaOI( vVML_Subtask, g_vSourceMeta );
   DropView( g_lpSGView );

   // If the user doesn't want the XPG written to a file, we'll name the view so that the
   // XPG OI can be retrieved by the caller.
   if ( lControl & zPARSE_DONOTWRITEXPG )
      SetNameForView( g_lpPIView, "XPG", vVML_Subtask, zLEVEL_TASK );
   else
      DropObjectInstance( g_lpPIView );

   // Drop the Global Expression view.
   DropView( g_vGlobalView );

   // Drop the Expression view.
   DropObjectInstance( g_lpExprView );

   // Drop the Global operation list.
   if ( g_lpGOListView )
      DropView( g_lpGOListView );

   // Drop the LOD list view.
   DropView( g_vLODList );
   DropView( g_vVORList );

   // Drop the core operation list view.
   DropView( g_lpZOListView );

   // Close the error file and source file.
   SysCloseFile( vVML_Subtask, g_lfHandle, 0 );
   SysCloseFile( vVML_Subtask, g_lfErrorHandle, 0 );

   // Return a return code based on the error object.
   lRC = GetViewByName( &vError, "TZVMEROO", vVML_Subtask, zLEVEL_TASK );

   // If the error object exists, which it better.
   if ( lRC == zLEVEL_TASK )
   {
      zLONG lCountErrors;

      // Count the number of entities. If there are any, then there were errors.
      lCountErrors = CountEntitiesForView( vError, "Error" );

      // If none, say so.
      if ( lCountErrors == 0 )
      {
         CreateEntity( vError, "Error", zPOS_AFTER );
         SetAttributeFromString( vError, "Error", "String", "No errors" );
         return( 0 );
      }
      else
         return( -1 );  // more than one means there were errors
   }

   // If no view, just get out.
   return( 0 );
}

//
// OPERATION: PositionAtOperationInSource
//
// DESCRIPTION: This function is passed a ZKey, 1, or 0. Its function
//    is to position the PI View on an operation of that ZKey. To position
//    an operation in the PI view means to  position both the operation
//    entity as well as the operation source entity. If a 0 is sent in for
//    the ZKey then position is set to the first operation. If a 1 is sent
//    then position is sent to the NEXT operation. This is okay since 0 and
//    1 can never be ZKeys.
//
zOPER_EXPORT zLONG OPERATION
PositionAtOperationInSource( zLONG lZKey )
{
   zLONG lLineNumber;
   zLONG lRC;

   if ( lZKey == 0 )  // position at first operation
   {
      lRC = SetCursorFirstEntity( g_lpPIView, "OperationSource", "" );
   }
   else
   if ( lZKey == 1 )  // position at next operation
   {
      lRC = SetCursorNextEntity( g_lpPIView, "OperationSource", "" );
   }
   else  // position at specific operation via zkey
   {
      lRC = SetCursorFirstEntityByInteger( g_lpPIView, "OperationSource", "ZKey", lZKey, "" );
   }

   // if it is not there return an error
   if ( lRC != zCURSOR_SET )
      return( lRC );

   // now position on the operation entity
   SetCursorFirstEntityByAttr( g_lpPIView, "Operation", "ZKey", g_lpPIView, "OperationSource", "ZKey", "" );

   // position on the first text line
   SetCursorFirstEntity( g_lpPIView, "OperationText", "" );

   // get the line number
   GetIntegerFromAttribute( &lLineNumber, g_lpPIView, "OperationSource", "LineNumber" );

   // this section is to position on the correct line in the text file it re-reads
   // through the file to the correct line number
   lGlobalReadRetCode = GetTextLine( );

   // fast forward in the file
   while( qqlineno < lLineNumber )
   {
      lGlobalReadRetCode = GetTextLine( );
   }

   return( 0 );  // success
}

//
// OPERATION: DeleteAllOpersForSource
//
// DESCRIPTION: Since an XPG file can have multiple source members per, we need to delete
//    the operations by position from under the source entity.
//
zOPER_EXPORT zLONG OPERATION
DeleteAllOpersForSource( zVIEW vXPGView )
{
   zLONG lRC;

   // Start with the first source file.
   lRC = SetCursorFirstEntity( vXPGView, "OperationSource", "SourceFile" );

   // While source entities exist, position on the correct operation entity as well as operation source.
   while ( lRC >= 0 )
   {
      // Now the operation
      lRC = SetCursorFirstEntityByAttr( vXPGView, "Operation", "ZKey",
                                        vXPGView, "OperationSource", "ZKey", "" );
      if ( lRC >= 0 )  // if one is there, do excludes (they were included)
      {
         // exclude operation
         ExcludeEntity( vXPGView, "Operation", zREPOS_NONE );

         // exclude operation source positioning at the next
         lRC = ExcludeEntity( vXPGView, "OperationSource", zREPOS_NEXT );
      }
      else
      {
         // if not there go on.
         lRC = SetCursorNextEntity( vXPGView, "OperationSource", "SourceFile" );
      }
   }

   // now exclude from the XPG
   ExcludeEntity( vXPGView, "SourceFile", zREPOS_NONE );

   // now re-include it for the parse.
   IncludeSubobjectFromSubobject( vXPGView, "SourceFile", g_vSourceMeta, "SourceFile", zPOS_AFTER );
   return( 0 );
}

//
// OPERATION: DeleteOneOperForSource
//
// DESCRIPTION: to delete one operation and any reference to it from the xpg file.  What this
//    really does is EXCLUDE it from the XPG then re-include it. This is so the ZKey of the
//    operation does not change.  If we deleted it and recreated it the ZKey would be no good,
//    thereby making local calls to this operation no good.
//
zOPER_EXPORT zLONG OPERATION
DeleteOneOperForSource( zVIEW vXPGView )
{
   // We are already positioned on the correct operation in the XPG object so
   // key off of it to position in the source.
   SetCursorFirstEntityByAttr( g_vSourceMeta, "Operation", "ZKey", vXPGView, "Operation", "ZKey", "" );

   // Now the source entity.
   SetCursorFirstEntityByAttr( vXPGView, "OperationSource", "ZKey", vXPGView, "Operation", "ZKey", "" );

   // Exclude the operation.  This is easier and quicker that deleting all of the statements and
   // variables, but it essentially does the same thing.
   ExcludeEntity( vXPGView, "Operation", zREPOS_NONE );

   // Ditto here, deletes all of the text lines.
   ExcludeEntity( vXPGView, "OperationSource", zREPOS_NONE );

   // Re-include it back from the Source meta.
   IncludeSubobjectFromSubobject( vXPGView, "OperationSource", g_vSourceMeta, "Operation", zPOS_AFTER );
   return( 0 );
}

//
// OPERATION: InitReadBuffer
//
// DESCRIPTION: This function sets up the text buffer for the parser.  yylex (actually qqlex)
//    requires that the string of text be named qqin, and the file handle be g_lfHandle (see qqlex).
//    Parsing can occur from either a file handle or a buffer. If the buffer parse is requested
//    then this function simply sets qqin to point to lpBuffer. If a file parse is requested,
//    then the lpBuffer contains the name (fully qualified) of the file. In that case, this
//    function opens the file for parsing.
//
zOPER_EXPORT zLONG OPERATION
InitReadBuffer( zVIEW  vSubtask,
                zPCHAR lpBuffer,      // buffer or file name.
                zLONG  lParseSource ) // PARSE_FROM_FILE or PARSE_FROM_BUFFER
{
// zVIEW lpTaskView = GetDefaultViewForActiveTask( );

   // View to error object just in case.
   zVIEW vError;

   // Init the line number to 0.
   qqlineno = 0;

   // Get the error object
   GetViewByName( &vError, "TZVMEROO", vSubtask, zLEVEL_TASK );

   // Do different things depending on what mode of parsing will be needed.
   switch ( lParseSource )
   {
      case 1: // file
         // Open the file for reading.
         g_lfHandle = SysOpenFile( vSubtask, lpBuffer, COREFILE_READ );

         // Generateerror if open failed.
         if ( g_lfHandle == -1 )
         {
            if ( vError )
            {
               CreateEntity( vError, "Error", zPOS_AFTER );
               SetAttributeFromString( vError, "Error", "String", "File Open Error!" );
            }

            return( -1 );
         }

         break;

      case 2: //  buffer
         qqin = lpBuffer;
         break;

      case 3: // xpg not implemented
         break;

      default:
         break;
   }

   return( 0 );
}

//
// OPERATION: InitGlobalDataForParse
//
// DESCRIPTION: Just Zero's out global stuff for parsing.
//
zOPER_EXPORT zLONG OPERATION
InitGlobalDataForParse( void )
{
   g_lLastTypeDeclared = 0;
   g_lDeclaredStringLength = 0;
   g_lEntityNameLength = 18;
   g_lAttributeNameLength = 32;
   g_lNumber = 0;
   g_lSetFNPL = 0;
   g_lPriorityFudgeFactor = 0;

   g_nStackPtr = 0;
   g_nOperStack[ 0 ].lOffset = 0;
   g_nOperStack[ 0 ].lLevel = 0;
   g_szViewName[ 0 ] = 0;
   g_szEntityName[ 0 ] = 0;
   g_szAttributeName[ 0 ] = 0;
   g_szScopeEntityName[ 0 ] = 0;
   g_lViewID = 0;
   g_lEntityID = 0;
   g_lScopeEntityID = 0;
   g_lAttributeID = 0;
   g_lAddNextStatementBefore = 0;
   g_lLoopControlID = -1;
   g_lNeedExtraEnd = 0;
   g_lParmListIndex = 0;
   //g_lParmListIndex = 1;
   g_lLeftDataType = 0;
   g_lLeftDataClass = 0;
   g_lRelativeLineNumber = 1; // test
   g_lFlag = 0;
   g_lParseAction = 0;
   g_lSaveText = 0;
   g_lCount = 0;

   return( 0 );
}

//
// OPERATION: InitErrorObject
//
// DESCRIPTION: Sets up the error object so that SyntaxError
//    can set up messages.
//
zOPER_EXPORT zLONG OPERATION
InitErrorObject( zVIEW  vSubtask,
                 zPCHAR szErrorFile, // fully qualified file name
                 zPCHAR szSourceFile ) // name of source file
{
// zVIEW  lpTaskView = GetDefaultViewForActiveTask( );
   zVIEW  vError;
   zSHORT nRC;

   // See if an error object already exists.
   nRC = GetViewByName( &vError, "TZVMEROO", vSubtask, zLEVEL_TASK );

   // If there is, drop it. from last parse.
   if ( nRC == zLEVEL_TASK )
      DropObjectInstance( vError );

   // Activate an empty instance of the error object.
   nRC = ActivateEmptyObjectInstance( &vError, "TZVMEROO", lpGlobalSubtask, zSINGLE );

   // If this could not be done, then give an error stating this.  Notice we do not use the global error object since this is
   // what we are trying to load.
   if ( nRC == -1 )
   {
      MessageSend( lpGlobalSubtask, "VM01007", "VML Parser",
                   szMessageListParse[ 6 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Create the root entity.
   nRC = CreateEntity( vError, "Root", zPOS_AFTER );

   // Name the view.
   nRC = SetNameForView( vError, "TZVMEROO", vSubtask, zLEVEL_TASK );

   // Open the error file to write errors too.
   g_lfErrorHandle = SysOpenFile( vSubtask, szErrorFile, COREFILE_CREATE );

   // Put the name of the source file in the error list as a title.
   SysWriteLine( vSubtask, g_lfErrorHandle, szSourceFile );
   nRC = SetAttributeFromString( vError, "Root", "SourceFileName", szSourceFile );
   return( 0 );
}

//
// OPERATION: InitExpressionObject
//
// DESCRIPTION: This function initializes the expression object so that
//    it may be manipulated
//
zOPER_EXPORT zLONG OPERATION
InitExpressionObject( zVOID )
{
   zSHORT nRC;

   // Activate it empty.
   nRC = ActivateEmptyObjectInstance( &g_lpExprView, "TZVSEXPO", lpGlobalSubtask, zSINGLE );

   // If it could not be activated empty, give an error why.
   if ( nRC == -1 )
   {
      MessageSend( lpGlobalSubtask, "VM01008", "VML Parser",
                   szMessageListParse[ 1 ],
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Create a root expression and a globally accessable view to it.
   CreateEntity( g_lpExprView, "Root", zPOS_AFTER );
   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &g_vGlobalView, g_lpExprView, 0 );

   return( 0 );
}

//
// OPERATION: GetTextLine
//
// DESCRIPTION: Reads the next line from the file into qqin and increments the line
//    number for the next line.
//
zOPER_EXPORT zLONG OPERATION
GetTextLine( )
{
   lGlobalReadRetCode = SysReadLine( g_lpPIView, &qqin, g_lfHandle );
   qqlineno++;

   if ( lGlobalReadRetCode < 0 )
      lGlobalReadRetCode = 0;

   return( lGlobalReadRetCode );
}

//
// OPERATION: SaveTextLine
//
// DESCRIPTION: creates an operation text entity and stores the actual line
//
//
zOPER_EXPORT zLONG OPERATION
SaveTextLine( void )
{
   zCHAR szWork[ LENWORK ];
   zLONG lRC;

   // If we have already stored the entire text git out.
   if ( g_lOperationFlag > 1 )  // update or position
      return( 0 );

   // This situation exists where we have read in some lines but.
   if ( g_lSaveText != 0 &&
        (CheckExistenceOfEntity( g_lpPIView, "OperationSource" ) == zCURSOR_SET) )
   {
      zLONG k;

      lRC = SetCursorLastEntity( g_lpPIView, "PreParseText", "" );
      k = qqlineno - g_lRelativeLineNumber;
      while ( lRC == zCURSOR_SET && k > 0 )
      {
         CreateEntity( g_lpPIView, "OperationText", zPOS_BEFORE );
         SetMatchingAttributesByName( g_lpPIView, "OperationText", g_lpPIView, "PreParseText", zSET_ALL );
         SetAttributeFromInteger( g_lpPIView, "OperationText", "RelativeLineNumber", k - 1 );
         lRC = DeleteEntity( g_lpPIView, "PreParseText", zREPOS_PREV );
         k--;
      }

      while ( lRC == zCURSOR_SET )
         lRC = DeleteEntity( g_lpPIView, "PreParseText", zREPOS_PREV );

      SetCursorLastEntity( g_lpPIView, "OperationText", "" );
      CreateEntity( g_lpPIView, "OperationText", zPOS_AFTER );
      SetAttributeFromInteger( g_lpPIView, "OperationText", "RelativeLineNumber",
                               qqlineno - g_lRelativeLineNumber );
      SetAttributeFromString( g_lpPIView, "OperationText", "Text", RemoveTabs( szWork, sizeof( szWork ) ) );
   }
   else
   {
      // Save line.
      CreateEntity( g_lpPIView, "PreParseText", zPOS_AFTER );
      SetAttributeFromInteger( g_lpPIView, "PreParseText", "RelativeLineNumber", qqlineno - g_lRelativeLineNumber );
      SetAttributeFromString( g_lpPIView, "PreParseText", "Text", RemoveTabs( szWork, sizeof( szWork ) ) );
   }

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
GetProfileData( zVIEW  vSubtask,
                zPCHAR szSrcDirectory,
                zPCHAR szSourceFileName,
                zPCHAR szXPGFileName,
                zPCHAR szOperationName )
{
   zVIEW  vProfileView;
   zLONG lLth;
   zLONG lRC;

   lRC = oTZ__PRFO_GetViewToProfile( &vProfileView, "VML", vSubtask, zCURRENT_OI );
   if ( lRC == 0 )
   {
      GetIntegerFromAttribute( &g_lParseAction, vProfileView, "VML", "ParseAction" );
      GetIntegerFromAttribute( &g_lParseLimit, vProfileView, "VML", "ParseLimit" );
      GetIntegerFromAttribute( &g_lParseSource, vProfileView, "VML", "ParseSource" );
      GetStringFromAttribute( szSrcDirectory, sizeof( szSrcDirectory ), vProfileView, "VML", "Dir" );
      lLth = zstrlen( szSrcDirectory );
      if ( szSrcDirectory[ lLth ] != '\\' )
      {
         szSrcDirectory[ lLth ] = '\\';
         szSrcDirectory[ lLth + 1 ] = 0;
      }

      lLth = 0;
      GetStringFromAttribute( szSourceFileName, sizeof( szSourceFileName ), vProfileView, "VML", "SourceFileName" );
      GetStringFromAttribute( szXPGFileName, sizeof( szXPGFileName ), vProfileView, "VML", "XPGFileName" );

      // If the XPG file name is NULL then copy in the SourceFileName.
      if ( szXPGFileName[ 0 ] == 0 )
      {
         strcpy_s( szXPGFileName, sizeof( szXPGFileName ), szSourceFileName );
      }

      GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vProfileView, "OP", "PassedName1" );
      return( 0 );
   }
   else
      return( -1 );
}

zOPER_EXPORT zLONG OPERATION
ReInitOperation( void )
{
   // Set the LINENUMBER attribute.
   SetAttributeFromInteger( g_lpPIView, "OperationSource", "LineNumber", -1 );
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
CompareFileToXPG( zLONG lZKey )
{
   zLONG lCompare;
   zLONG lRC;

   SetCursorFirstEntityByInteger( g_lpPIView, "OperationSource", "ZKey", lZKey, "");

   // Compare the current line to what was stored.
   lCompare = CompareAttributeToString( g_lpPIView, "OperationText", "Text", qqin );

   // As long as the lines are equal and there are more text attributes, loop.
   lRC = zCURSOR_SET;
   while ( lCompare == 0 && lRC == zCURSOR_SET )
   {
      // Last was good, get the next from the file.
      lRC = GetTextLine( );

      if ( lRC < 0 )
         return( -1 );

      // Get the next from the XPG.
      lRC = SetCursorNextEntity( g_lpPIView, "OperationText", "" );

      // If these was one there, compare.
      if ( lRC == zCURSOR_SET )
         lCompare = CompareAttributeToString( g_lpPIView, "OperationText", "Text", qqin );
   }

   return( lCompare );
}

zOPER_EXPORT zLONG OPERATION
SkipRemainingFile( )
{
   lGlobalReadRetCode = SysReadLine( g_lpPIView, &qqin, g_lfHandle );

   while ( lGlobalReadRetCode > 0 )
   {
      lGlobalReadRetCode = SysReadLine( g_lpPIView, &qqin, g_lfHandle );
      qqlineno++;
   }

   return( lGlobalReadRetCode );
}

zOPER_EXPORT zLONG OPERATION
IncludeCoreHeader( zVIEW lpView, zPCHAR szHeaderName )
{
   zLONG lZKey;
   zLONG lRC;

   lRC = SetCursorFirstEntityByString( g_lpZOListView, "HeaderFile", "Name", szHeaderName, "" );
   if ( lRC > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &lZKey, g_lpZOListView, "HeaderFile", "ZKey" );

      lRC = SetCursorFirstEntityByInteger( lpView, "ExternalHeaderFile", "ZKey", lZKey, "" );
      if ( lRC < zCURSOR_SET )
      {
         SetCursorLastEntity( lpView, "ExternalHeaderFile", "" );
         IncludeSubobjectFromSubobject( lpView, "ExternalHeaderFile", g_lpZOListView, "HeaderFile", zPOS_AFTER );
      }
   }

   return( 0 );
}

static zPCHAR
RemoveTabs( zPCHAR pchWork, zLONG lMaxLth )
{
   zPCHAR pch;
   zSHORT nTab = 0;
   zSHORT nLth = (zSHORT) zstrlen( qqin );

   // Count the tabs.
   pch = qqin;
   while ( (pch = zstrchr( pch,'\t' )) != 0 )
   {
      nTab++;
      pch++;
   }

   if ( nTab == 0 )
     return( qqin ); // nothing to do

   // For each tab we need 2 more bytes in the workarea.
   if ( nLth + 2 * nTab >= LENWORK - 1 )
   {
      // The workstring is not large enough, so we replace each tab in qqin by a blank and return qqin without using the workstring.
      pch = qqin;
      while ( (pch = zstrchr( pch, '\t' )) != 0 )
      {
         *pch = ' ';
         pch++;
      }

      return( qqin );
   }
   else
   {
      // There is enouch space in pchWork we substitute each tab by 3 blanks.
      zPCHAR pchSource = qqin;
      zPCHAR pchTarget = pchWork;
      zLONG k, n;
      while ( (pch = zstrchr( pchSource, '\t' ) ) != 0 )
      {
        k = pch - pchSource;
        if ( k > 0 )
           zmemcpy( pchTarget, pchSource, k );

        pchTarget += k;
        for ( n = 0; n < 3; n++ )
           *(pchTarget++) = ' ';

        pchSource = ++pch;
      }

      // Lastly, move the rest of the string or add a zero-Byte to target.
      if ( *pchSource )
        strcpy_s( pchTarget, lMaxLth, pchSource );
      else
        *pchTarget = 0;

      return( pchWork );
   }
}

////////////////////////////////////////////////
//
//  >>>PrintXPG
//
// PURPOSE:     Print XPG object to Engine Trace Window
//
// DESCRIPTION:
// go Operations, Variables, Statements
// resolve exprexions and print everything to Trace Window
//
// PARAMETER:
//    zVIEW vXPG  = the object (based on LOD TZVSPOOO) to be printed
//
// RETURNS:     zLONG
//              0         ==> ok
//              otherwise ==> Error
//
////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
PrintXPG( zVIEW vXPG )
{
   zSHORT nRC;

   // For Each Operation ...
   nRC = SetCursorFirstEntity( vXPG, "Operation", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      zSHORT nRC2;

      // print operation detail
      fnPrintOperation( vXPG );

      // loop over variables
      nRC2 = SetCursorFirstEntity( vXPG, "Variable", 0 );
      while ( nRC2 >= zCURSOR_SET )
      {
         // print variable detail
         fnPrintVariable( vXPG );
         nRC2 = SetCursorNextEntity( vXPG, "Variable", 0 );
      }

      // loop over statements
      nRC2 = SetCursorFirstEntity( vXPG, "Statement", 0 );
      while ( nRC2 >= zCURSOR_SET )
      {
         // print statement detail
         fnPrintStatement( vXPG, 1 );
         nRC2 = SetCursorNextEntity( vXPG, "Statement", 0 );
      }

      nRC = SetCursorNextEntity( vXPG, "Operation", 0 );
   }
   return( 0 );
}

////////////////////////////////////////////////
//
//  >>>fnPrintOperation
//
//  Print XPG "Operation" to Engine Trace Window
//
////////////////////////////////////////////////
static zVOID
fnPrintOperation( zVIEW vXPG )
{
   zCHAR szLine[ 256 ];
   zCHAR szOperation[ 33 ] = { 0 };
   zLONG lZKey = 0;
   zCHAR szType[ 2 ] = { 0 };
   zCHAR szReturnDataType[ 2 ] = { 0 };

   GetStringFromAttribute( szOperation, sizeof( szOperation ), vXPG, "Operation", "Name" );
   GetIntegerFromAttribute( &lZKey, vXPG, "Operation", "ZKey" );
   // Description is skipped
   GetStringFromAttribute( szType, sizeof( szType ), vXPG, "Operation", "Type" );
   // ReturnDesc is skipped
   GetStringFromAttribute( szReturnDataType, sizeof( szReturnDataType ), vXPG, "Operation", "ReturnDataType" );
   // OrdinalValue is skipped
   // CurrentDebugFlag is skipped
   // C_GeneratedOperationName is skipped

   sprintf_s( szLine, sizeof( szLine ), "Operation=%s ZKey=%ld Type=%s ReturnType=%s",
              szOperation, lZKey, szType, szReturnDataType );
   TraceLineS( "", "" );
   TraceLineS( szLine, "" );
}

////////////////////////////////////////////////
//
//  >>>fnPrintVariable
//
//  Print XPG "Variable" to Engine Trace Window
//
////////////////////////////////////////////////
static zVOID
fnPrintVariable( zVIEW vXPG )
{
   zCHAR szLine[ 256 ];
   zLONG lID = 0;
   zLONG lParameterFlag = 0;
   zLONG lVMLType = 0;
   zLONG lDataType = 0;
   zLONG lNElements = 0;
   zLONG lLineNumber = 0;
   zCHAR szName[ 33 ] = { 0 };
   zCHAR szPFlag[ 2 ] = { 0 };
   zCHAR szUFlag[ 2 ] = { 0 };

   GetIntegerFromAttribute( &lID, vXPG, "Variable", "ID" );
   GetIntegerFromAttribute( &lParameterFlag, vXPG, "Variable", "ParameterFlag" );
   GetStringFromAttribute( szName, sizeof( szName ), vXPG, "Variable", "Name" );
   GetIntegerFromAttribute( &lVMLType, vXPG, "Variable", "VMLType" );
   GetIntegerFromAttribute( &lDataType, vXPG, "Variable", "DataType" );
   GetIntegerFromAttribute( &lNElements, vXPG, "Variable", "NElements" );
   // Modify is skipped
   // Value is skipped
   GetIntegerFromAttribute( &lLineNumber, vXPG, "Variable", "LineNumber" );
   GetStringFromAttribute( szPFlag, sizeof( szPFlag ), vXPG, "Variable", "PFlag" );
   GetStringFromAttribute( szUFlag, sizeof( szUFlag ), vXPG, "Variable", "UFlag" );

   sprintf_s( szLine, sizeof( szLine ), "   Variable=%s ID=%ld Parameter=%ld VMLType=%ld DataType=%ld NElements=%ld LineNumber=%ld PFlag=%s UFlag=%s",
              szName, lID, lParameterFlag, lVMLType, lDataType, lNElements, lLineNumber, szPFlag, szUFlag );
   TraceLineS( szLine, "" );
}

////////////////////////////////////////////////
//
//  >>>fnPrintStatement
//
//  Print XPG "Statement" to Engine Trace Window
//    Print Substatements recursively
//
////////////////////////////////////////////////
static zVOID
fnPrintStatement( zVIEW vXPG, zLONG lLevel )
{
   zCHAR szIndent[ 91 ] = { 0 };
   zCHAR szClass[ 33 ] = { 0 };
   zCHAR szLine[ 256 ];
   zLONG lLineNumber = 0;
   zLONG lClass = 0;
   zCHAR szExprCode[ EXPRCODE_MAXSTRING ];
   zCHAR szExprText[ EXPRTEXT_MAXSTRING ];
   sQExprStruct sWorkExpr = { 0 };

   zLONG lParameterFlag = 0;
   zLONG lVMLType = 0;
   zLONG lDataType = 0;
   zLONG lNElements = 0;
   zULONG uCodeLth;
   zULONG uTextLth;
   zULONG nExprIndex;
   int k;

   // indentation
   for ( k = 0; k < lLevel; k++ )
      strcat_s( szIndent, sizeof( szIndent ), "   " );

   GetIntegerFromAttribute( &lLineNumber, vXPG, "Statement", "LineNumber" );
   GetIntegerFromAttribute( &lClass, vXPG, "Statement", "Class" );

   switch ( lClass )
   {
      case qEXPRESSION:
         strcpy_s( szClass, sizeof( szClass ), "qEXPRESSION" );
         break;
      case qWHILE:
         strcpy_s( szClass, sizeof( szClass ), "qWHILE" );
         break;
      case qEND:
         strcpy_s( szClass, sizeof( szClass ), "qEND" );
         break;
      case qRETURN:
      case qRETURNEND:
         strcpy_s( szClass, sizeof( szClass ), "qRETURN" );
         break;
      case qIF:
         strcpy_s( szClass, sizeof( szClass ), "qIF" );
         break;
      case qFOR:
         strcpy_s( szClass, sizeof( szClass ), "qFOR" );
         break;
   }

   sprintf_s( szLine, sizeof( szLine ), "*STATEMENT* LineNumber=%ld Class=%s(%ld)", lLineNumber, szClass, lClass );
   TraceLineS( "", "" );
   TraceLineS( szIndent, szLine );

   uCodeLth = EXPRCODE_MAXSTRING;
   GetBlobFromAttribute( szExprCode, &uCodeLth, vXPG, "Statement", "Expression" );

   uTextLth = EXPRTEXT_MAXSTRING;
   GetBlobFromAttribute( szExprText, &uTextLth, vXPG, "Statement", "ExprText" );

   // loop over expressions
   nExprIndex = 0;
   while ( nExprIndex < uCodeLth )
   {
      zmemcpy( (zPCHAR) &sWorkExpr, (zPCHAR) &szExprCode[ nExprIndex ], sizeof( sQExprStruct ) - MAXSTRING );
      fnPrintExpression( &sWorkExpr, lLevel );
      nExprIndex += sizeof( sQExprStruct ) - MAXSTRING;
   }

   if ( uTextLth )
   {
      for ( k = 0; k < (int) uTextLth - 1; k++ )
      {
         if ( szExprText[ k ] == 0 )
            szExprText[ k ] = ' ';
      }

      sprintf_s( szLine, sizeof( szLine ), "%s ExpressionText=", szIndent );
      TraceLineS( szLine, szExprText );
   }

   // recursion to SubStatement
   if ( CheckExistenceOfEntity( vXPG, "Substatement" ) >= zCURSOR_SET )
   {
      zSHORT nRC2;

      nRC2 = SetCursorFirstEntity( vXPG, "Substatement", 0 );
      while ( nRC2 >= zCURSOR_SET )
      {
         // recurse down
         SetViewToSubobject( vXPG, "Substatement" );
         fnPrintStatement( vXPG, lLevel + 1 );
         ResetViewFromSubobject( vXPG );
         nRC2 = SetCursorNextEntity( vXPG, "Substatement", 0 );
      }
   }
}

////////////////////////////////////////////////
//
//  >>>fnPrintExpression
//
//  Print XPG Expressionto Engine Trace Window
//
////////////////////////////////////////////////
static zVOID
fnPrintExpression( sQExprStruct *pWorkExpr, zLONG lLevel )
{
   zCHAR szIndent[ 91 ] = { 0 };
   zCHAR szElementClass[ 33 ] = { 0 };
   zCHAR szDataClass[ 33 ] = { 0 };
   zCHAR szDataType[ 33 ] = { 0 };
   zCHAR szLine[ 256 ] = { 0 };
   int k;

   // indentation
   for ( k = 0; k < lLevel; k++ )
      strcat_s( szIndent, sizeof( szIndent ), "   " );

   switch( pWorkExpr->lElementClass )
   {
      case qVARIABLE:
         strcpy_s( szElementClass, sizeof( szElementClass ), "qVARIABLE" );
         break;
      case qDEFINE:
         strcpy_s( szElementClass, sizeof( szElementClass ), "qDEFINE" );
         break;
      case qCONSTANT:
         strcpy_s( szElementClass, sizeof( szElementClass ), "qCONSTANT" );
         break;
      case qATTRIBUTEQUALIFIER:
         strcpy_s( szElementClass, sizeof( szElementClass ), "qATTRIBUTEQUALIFIER" );
         break;
      case qOPERATIONCALL:
         strcpy_s( szElementClass, sizeof( szElementClass ), "qOPERATIONCALL" );
         break;
      case qOPERATOR:
         fnPrintOperator( pWorkExpr, szIndent );
         return;
   }

   switch( pWorkExpr->lDataClass )
   {
      case qTEXT:
         strcpy_s( szDataClass, sizeof( szDataClass ), "qTEXT" );
         break;
      case qNUMERIC:
         strcpy_s( szDataClass, sizeof( szDataClass ), "qNUMERIC" );
         break;
   }

   switch( pWorkExpr->lDataType )
   {
      case qTINTEGER:
         strcpy_s( szDataType, sizeof( szDataType ), "qTINTEGER" );
         break;
      case qTDECIMAL:
         strcpy_s( szDataType, sizeof( szDataType ), "qTDECIMAL" );
         break;
      case qTSHORT:
         strcpy_s( szDataType, sizeof( szDataType ), "qTSHORT" );
         break;
      case qTSTRING:
         strcpy_s( szDataType, sizeof( szDataType ), "qTSTRING" );
         break;
      case qTCHARACTER:
         strcpy_s( szDataType, sizeof( szDataType ), "qTCHARACTER" );
         break;
      case qTUNKNOWN:
         strcpy_s( szDataType, sizeof( szDataType ), "qTUNKNOWN" );
         break;
   }

   sprintf_s( szLine, sizeof( szLine ), "%s DataClass=%s(%ld) DataType=%s(%ld) Offset=%ld Level=%ld Flags=%ld",
            szElementClass, szDataClass, pWorkExpr->lDataClass, szDataType, pWorkExpr->lDataType,
            pWorkExpr->lOffset, pWorkExpr->lLevel, pWorkExpr->lFlags );
   TraceLineS( szIndent, szLine );
}

////////////////////////////////////////////////
//
//  >>>fnPrintOperator
//
//  Print XPG Operator to Engine Trace Window
//
////////////////////////////////////////////////
static zVOID
fnPrintOperator( sQExprStruct *pWorkExpr, zCPCHAR szIndent )
{
   zCHAR szOffset[ 33 ] = { 0 };
   zCHAR szLine[ 256 ] = { 0 };

   switch ( pWorkExpr->lOffset )
   {
      case qLPAREN:
         strcpy_s( szOffset, sizeof( szOffset ), "qLPAREN" );
         break;
      case qRPAREN:
         strcpy_s( szOffset, sizeof( szOffset ), "qRPAREN" );
         break;
      case qOPENOPER:
         strcpy_s( szOffset, sizeof( szOffset ), "qOPENOPER" );
         break;
      case qCLOSEOPER:
         strcpy_s( szOffset, sizeof( szOffset ), "qCLOSEOPER" );
         break;
      case qCOMMA:
         strcpy_s( szOffset, sizeof(szOffset), "qCOMMA" );
         break;
      case qPERIOD:
         strcpy_s( szOffset, sizeof( szOffset ), "qPERIOD" );
         break;
      case qEQUAL:
         strcpy_s( szOffset, sizeof( szOffset ), "qEQUAL" );
         break;
      case qMINUS:
         strcpy_s( szOffset, sizeof( szOffset ), "qMINUS" );
         break;
      case qADDITION:
         strcpy_s( szOffset, sizeof( szOffset ), "qADDITION" );
         break;
      case qMULTIPLICATION:
         strcpy_s( szOffset, sizeof( szOffset ), "qMULTIPLICATION" );
         break;
      case qDIVISION:
         strcpy_s( szOffset, sizeof( szOffset ), "qDIVISION" );
         break;
      case qCONDEQUAL:
         strcpy_s( szOffset, sizeof( szOffset ), "qCONDEQUAL" );
         break;
      case qLESSTHANEQUAL:
         strcpy_s( szOffset, sizeof( szOffset ), "qLESSTHANEQUAL" );
         break;
      case qLESSTHAN:
         strcpy_s( szOffset, sizeof( szOffset ), "qLESSTHAN" );
         break;
      case qGREATERTHANEQUAL:
         strcpy_s( szOffset, sizeof( szOffset ), "qGREATERTHANEQUAL" );
         break;
      case qGREATERTHAN:
         strcpy_s( szOffset, sizeof( szOffset ), "qGREATERTHAN" );
         break;
      case qNOTEQUAL:
         strcpy_s( szOffset, sizeof( szOffset ), "qNOTEQUAL" );
         break;
      case qNOT:
         strcpy_s( szOffset, sizeof( szOffset ), "qNOT" );
         break;
      case qAND:
         strcpy_s( szOffset, sizeof( szOffset ), "qAND" );
         break;
      case qOR:
         strcpy_s( szOffset, sizeof( szOffset ), "qOR" );
         break;
   }

   sprintf_s( szLine, sizeof( szLine ), "qOPERATOR Offset=%s(%ld)", szOffset, pWorkExpr->lOffset );
   TraceLineS( szIndent, szLine );
}

// >>>fnDeleteGenerated
//  delete generated operations from source meta
//
static zVOID
fnDeleteGenerated( zVIEW vSourceMeta )
{
   zSHORT nRC;
   zVIEW vSearchView;

   CreateViewFromViewForTask( &vSearchView, vSourceMeta, 0 );
   nRC = SetCursorFirstEntity( vSearchView, "Operation", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      zCHAR szOperName[ 33 ];
      zLONG lClass = 0;

      GetStringFromAttribute( szOperName, sizeof( szOperName ), vSearchView, "Operation", "Name" );
      GetIntegerFromAttribute( &lClass, vSearchView, "Operation", "Class" );
      if ( zstrlen( szOperName ) > 7 && memcmp( szOperName, "fnLocal", 7 ) == 0 &&
           (lClass & 0x00000001) != 0 )
      {
         DeleteEntity( vSearchView, "Operation", zREPOS_NONE );
      }

      nRC = SetCursorNextEntity( vSearchView, "Operation", 0 );
   }

   DropView( vSearchView );
}
