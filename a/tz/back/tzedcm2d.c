/////////////////////////////////////////////////////////////////////////////
//
// These operations are written in C for easier access to global operations.
// At some point they should be re-written in VML.
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2001.10.23 DKS
   Added code to put out makefile with target of 80 character line.
   Format generated date as YYYY.MM.DD.

2001.07.13 RG
   - Moved fnVerifyDir from this file to tzlooprs.c because it is
     needed in this file too. Rename to VerifyDir in tzlooprs.c
   - Moved fnDirectoryIsRequired fron this file to tzlooprs.c
     because it is only needed in VerifyDir.
2001.07.06 RG
   Build relative pathes for Target Executable Directory,
   Obj/Lib Directory, Make-Directory and Resource-Directory

2001.06.30  BL
   Maintained Compiler Specification

2001.03.04 RG   SironReports
   generate BASELINKDLLS for zenv.hnm with kzsirrep.lib

2001.03.13 BL   GRID
   generate zenv.hnm with truegrid.lib

2000.12.12 DKS
   Corrected Hungarian notation

2000.12.01 BL/RG Z2000
   if dialog not checked out view is readonly
   change view to updateble for parsing

2000.11.27  DKS Z2000
   Workstation generation of .C files now reports if there is a mismatch
   between a Dialog (.PWD) and its generated resource file.

2000.10.30  RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.10.24  SR  Z2000 Size of path variables
   Modified size of of all filename and path variables in whole module
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
   Adjust the size of messagetext variables.

2000.09.27  BL  Z10
   Modified fnParseGenTarget: do not change Entity Cursor in Meta View

2000.09.24  BL  Z10
   Modified RefreshTargetList: Check exists CurrentCompilerSpec

2000.09.19  HH
   try to load the "active" meta before parsing.

2000.08.08  BL  Z10
   Modified RefreshTargetList: if Compiler Spec does not exists, disable
   all Compile Options in Target Specification
   Modified fnVerifyDir and added fnDirectoryIsRequiredD: if Directory required,
   send an other Error Message

2000.03.02  TMV Z10
   User should not compile and link against debug libs if not necessary

1999.11.27  BL  Z10  new menu structure
   Modified Operation MakeTarget: to not drop the TaskLPLR view when an error occurred.
   Modified Operation RefreshTargetList for Refresh Button Make Target
   Modified Operation Close_CompilerSpecification: if valid Current Compiler
   Spec, then not close Window
   Added Operation DeleteCompilerSpec for Delete Compiler Spec and refresh
   Current Compiler Spec

1999.11.15  RG  Z2000
    Changed msvcrtd to msvcrt because we don't want to use MSVC Debug lib's
    and DLL's

1999.10.29  DGC 10a
   Bumped up a buffer size...we were over-running the buffer when creating
   ZCOMPILE.HNM when the user had a long path.

1999.09.17  BL  ALL
   insert tzvmlip.lib for new Zeidon Operations

 from here old version

   10.10.1996 Guenter Trageser: Add start of OS2 make

   20.03.1997 GT
      Check, if external targets for compiler spec

   21.05.1997 FrankH/DonC
      external targets: - the test of valid extensions didn't work (szExtension needed length 5)
                        - the test of valid extensions is executed direct after
                          creation of the OI
                        - better UserInterface using the WindowsStandardDialog OperatorPromptForFile
                        - the ext. Obj-& Lib-Files are stored with Drive and Directory
                          --> Change in TZLOOPRS.C; execute the splitpath there

16.05.1997  DKS
   Modified OperatorPromptForFile signature for SaveAs option

27.06.1997  DonC
   Modified MakeTarget to handle selected entries in the listbox rather than
   a single entry.

03.07.1997  DonC
   Corrected MakeTarget to not drop the TaskLPLR view when an error occurred.

19.07.1997  DonC
   Corrected OperatorPromptForFile calls so that files would be opened
   read-only.

25.08.1997  DonC
   Modified fnParseGenTarget to create a work object of type LOD from vMeta
   for GlobalOperationGroup to pass to the VML Parser.  It is necessary to
   parse a VML global operation from work station.

13.11.1997  DonC
   Modified fnParseGenTarget to Delete OI _GenFromXPG that was left hanging
   from the C generation.

12.05.1998  DonC
   Modified fnParseGenTarget to use ActivateMetaOI_ByName instead of
   ActivateMetaOI_ByZKey

1998.05.21  DGC
   Removed '$(LINKDLLS)' from target dependencies in the makefile.

1998.06.12  DKS
   Forced compiler to MSC 1.5 because FindCompilerType in tzlooprs.c
   requires it for Microsoft compiles ... which is the only one we
   are supporting for 9j so far.

1998.06.24  DGC
   Added kzrpsrco.lib to makefile generation.
   Changed CMD.EXE to COMMAND.COM for Win95
   Added .RES file to ZCOMPILE.HNM.

1998.07.23  DGC
   Made a couple of changes for Frank H:
   o  LIB files are now put in the OBJ directory.
   o  Added PATH to the Compiler spec and we now use it.

1998.07.29  DGC
   Added '/PDB:...' to link step to avoid creating vc50.pdb.

1998.08.05  DGC
   Added second /IMPLIB to zcompile.hnm.

1998.10.14  DGC
   We can now handle NT/95 questions at runtime instead of compile time.

1998.12.07  DGC
   Fixed bug reported by Frank--if user doesn't want debug we shouldn't
   compile with debug.

1999.01.13  DGC
   Made change to avoid creating vc50.pdb.

1999.02.23  DGC
   Made fix so that TZZOLODO is activated as a system object.

*/

#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "TZ__OPRS.H"
#include "TZLODOPR.H"
#include "ZDRVROPR.H"
#include "ZEIDONOP.H"

#include <dos.h>  // needed for PIF file processing
#include <direct.h>

#define SWL( v, str ) SysWriteLine( v, hFile, str )


zOPER_EXPORT zSHORT OPERATION
CreateMakeIncludeFile( zVIEW vSubtask,
                       zVIEW vTaskLPLR,
                       zBOOL bOverwrite );

zOPER_EXPORT zSHORT OPERATION
CopyCompilerSpec( zVIEW vSubtask );


zSHORT LOCALOPER
CreatePIF_File( zVIEW vTaskLPLR, zCHAR cOverwrite );
static BOOL
fnVerifyExternalTargetDir( zVIEW vSubtask, zVIEW vTaskLPLR );
static BOOL
fnVerifyIncludeLibTargetDir( zVIEW vSubtask, zVIEW vTaskLPLR );
static zSHORT
fnVerifyExternalAllDir( zVIEW vSubtask, zVIEW vTaskLPLR );
static zSHORT
fnRemoveTargetsWithoutLibsObjs( zVIEW vTaskLPLR );
static zSHORT
fnSetNameForCurrentCompilerSpec( zVIEW vSubtask, zVIEW vTaskLPLR );
static zSHORT
fnCheckCompilerSpec( zVIEW vSubtask, zVIEW vTaskLPLR );
static BOOL
fnVerifyExternalObjFile( zVIEW vSubtask, zVIEW vTaskLPLR, zPCHAR szFileName );
static BOOL
fnVerifyExternalLibFile( zVIEW vSubtask, zVIEW vTaskLPLR, zPCHAR szFileName );
static void
fnRemoveSpace( zPCHAR szFileName );
static void
fnExternalToLibTargetDir( zVIEW vSubtask, zVIEW vTaskLPLR );
static void
fnExternalLibObjToLibTarget( zVIEW vTask2, zPCHAR pchEntity );

void
fnGetFileDateTime( zVIEW lpTaskView, zPCHAR pchFile, zPCHAR pchTimeStamp, zLONG lMaxLth )
{
   zLONG hFile;

   hFile = SysOpenFile( lpTaskView, pchFile, COREFILE_READ );

   if ( hFile == -1 )
   {
      *pchTimeStamp = 0;
      return;
   }

   SysGetFileDateTime( hFile, pchTimeStamp, lMaxLth );
   SysCloseFile( lpTaskView, hFile, 0 );

} // fnGetFileDateTime

// Parse and/or Gen the current target if it is needed.
//
// Returns:  TRUE  - everything OK.
//           FALSE - Error.
//
zSHORT LOCALOPER
fnParseGenTarget( zVIEW vSubtask, zVIEW vTaskLPLR, zCPCHAR cpcGenLang )
{
   zVIEW  vMeta;
   zVIEW  vMetaCopy;
   zVIEW  vTemp;
   zVIEW  vT;
   zLONG  lType;
   zSHORT nExecDirLth;
   zSHORT nSourceDirLth;
   zSHORT nTargetDirLth;
   zCHAR  szXPG[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szSourceFile[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szTargetFile[ zMAX_FILESPEC_LTH + 1 ];
   zBOOL  bParseOnly = FALSE;
   zBOOL  bAlwaysParse = FALSE;
   zBOOL  bAlwaysGen = FALSE;
   zLONG  lMakeOption;
   zCHAR  szMetaName[ 33 ];
   zCHAR  szMetaName2[ 33 ];
   zCHAR  szMsg[ 200 ];
   zSHORT nRC;

   GetStringFromAttribute( szTargetFile, zsizeof( szTargetFile ), vTaskLPLR, "LPLR", "ExecDir" );
   SysConvertEnvironmentString( szXPG, zsizeof( szXPG ), szTargetFile );
   nExecDirLth = SysAppendcDirSep( szXPG );
   GetStringFromAttribute( szTargetFile, zsizeof( szTargetFile ), vTaskLPLR, "LPLR", "PgmSrcDir" );
   SysConvertEnvironmentString( szSourceFile, zsizeof( szSourceFile ), szTargetFile );
   nSourceDirLth = SysAppendcDirSep( szSourceFile );
   strcpy_s( szTargetFile, zsizeof( szTargetFile ), szSourceFile );
   nTargetDirLth = nSourceDirLth;

   if ( cpcGenLang[ 0 ] == 'J' )
   {
      strcat_s( szTargetFile + nTargetDirLth, zsizeof( szTargetFile ) - nTargetDirLth, "JavaProject\\src\\" );
      nTargetDirLth += 16;
      GetStringFromAttribute( szTargetFile + nTargetDirLth, zsizeof( szTargetFile ) - nTargetDirLth, vTaskLPLR, "LPLR", "JavaPackageName" );
      nTargetDirLth = SysAppendcDirSep( szTargetFile );
   }

   // Retrieve the make option.  Current values are:
   //    1  Parse only--parse only if needed.
   //    2  Parse only--force parse.
   //    3  Build -- parse/gen/compile if needed.
   //    4  Force Parse--implicitly forces gen/compile.
   //    5  Force Gen--parse if needed;implicitly forces compile.
   //    6  Force Compile---parse/gen if needed.
   //    7  Parse/Gen if needed -- no compile!
   GetIntegerFromAttribute( &lMakeOption, vTaskLPLR, "Compiler", "MakeOption" );

   bParseOnly   = (lMakeOption <= 2);
   bAlwaysParse = (lMakeOption == 2 || lMakeOption == 4);
   bAlwaysGen   = (lMakeOption == 5 || lMakeOption == 4);

   // First check to see if any source needs to be parsed/generated.
   // Go through each of the metas that make up the current target and
   // check the timestamps of all source files and their .XPG files.
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Meta", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "Meta", 0 ) )
   {
      zPCHAR pchPtr;
      zPCHAR pchSourceFileEntity;
      zSHORT nMetaRC = 0;

      GetIntegerFromAttribute( &lType, vTaskLPLR, "Meta", "MetaType" ); // 2007
      GetStringFromAttribute( szMetaName, zsizeof( szMetaName ), vTaskLPLR, "Meta", "Name" ); // TZRPSRCO

      // If we are called from a Zeidon Tool, the we first try to open
      // the meta, which might already be updated from inside the tool.
      vMeta = 0;
      switch ( lType )
      {
         case zREFER_GOPGRP_META:  // Global Op
         case zSOURCE_GOPGRP_META: // Global Op
            nMetaRC = GetViewByName( &vMeta, "TZOGSRCO", vSubtask, zLEVEL_TASK );

            // Determine if the open meta is the one to be parsed.
            if ( nMetaRC <= 0 )
               break;

            GetStringFromAttribute( szMetaName2, zsizeof( szMetaName2 ), vMeta, "GlobalOperationGroup", "Name" );
            if ( zstrcmp( szMetaName, szMetaName2 ) != 0 )
               nMetaRC = 0; // no, not the right one

            break;

         case zREFER_LOD_META:  // LOD
         case zSOURCE_LOD_META: // LOD
            nMetaRC = GetViewByName( &vMeta, "TZZOLODO", vSubtask, zLEVEL_TASK );

            // Determine if the open meta is the one to be parsed.
            if ( nMetaRC <= 0 )
               break;

            GetStringFromAttribute( szMetaName2, zsizeof( szMetaName2 ), vMeta, "LOD", "Name" );
            if ( zstrcmp( szMetaName, szMetaName2 ) != 0 )
               nMetaRC = 0; // no, not the right one

            break;

         case zREFER_DIALOG_META:  // Dialog
         case zSOURCE_DIALOG_META: //Dialog
            nMetaRC = GetViewByName( &vMeta, "TZWINDOWL", vSubtask, zLEVEL_TASK );
            if ( nMetaRC <= 0 )
               break;

            // Determine if the open meta is the one to be parsed.
            GetStringFromAttribute( szMetaName2, zsizeof( szMetaName2 ), vMeta, "Dialog", "Tag" );
            if ( zstrcmp( szMetaName, szMetaName2 ) != 0 )
               nMetaRC = 0;  // no, not the right one

            break;
      }

      if ( nMetaRC <= 0 &&
           ActivateMetaOI_ByName( vSubtask, &vMeta, 0, (zSHORT) lType, 0, szMetaName, 0 ) < 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Internal error - ActivateMeta failed for " );
         strcat_s( szMsg , zsizeof( szMsg ), szMetaName );
         strcat_s( szMsg , zsizeof( szMsg ), "." );

         MessageSend( vSubtask, "ED00999", "Zeidon Compiler",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( FALSE );
      }

      switch ( lType )
      {
         case zREFER_DOMAINGRP_META:
            pchSourceFileEntity = "DomainGroup";
            break;

         case zREFER_GOPGRP_META:
            pchSourceFileEntity = "GlobalOperationGroup";

            // *** TEMPORARY HACK ***
            // This should be replaced when/if VML is re-written.
            //
            // Copy the SourceFile subobject information to a new work object of type TZZOLODO.
            // This is necessary because the Parser requires an object with a subobject starting
            // with entity named "SourceFile".  It is easier to set the source information as a
            // LOD than it is to modify the Parser to handle the different Global Operation
            // structure.
            // This is basically the same hack that exists in tzopupdd.c.

            CreateViewFromViewForTask( &vT, vMeta, 0 );
            ActivateEmptyObjectInstance( &vTemp, "TZZOLODO", vTaskLPLR, zSINGLE );
            SetNameForView( vTemp, "GOP_SourceFile", vSubtask, zLEVEL_TASK );
            CreateMetaEntity( vSubtask, vTemp, "LOD", zPOS_LAST );
            SetAttributeFromAttribute( vTemp, "LOD", "Name", vT, "GlobalOperationGroup", "Name" );
            CreateMetaEntity( vSubtask, vTemp, "SourceFile", zPOS_LAST );
            SetMatchingAttributesByName( vTemp, "SourceFile", vT, "GlobalOperationGroup", zSET_NULL );
            nRC = SetCursorFirstEntity( vT, "Operation", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               CreateMetaEntity( vSubtask, vTemp, "Operation", zPOS_LAST );
               SetMatchingAttributesByName( vTemp, "Operation", vT, "Operation", zSET_NULL );
               nRC = SetCursorFirstEntity( vT, "Parameter", 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  CreateMetaEntity( vSubtask, vTemp, "Parameter", zPOS_LAST );
                  SetMatchingAttributesByName( vTemp, "Parameter", vT, "Parameter", zSET_NULL );
                  nRC = SetCursorNextEntity( vT, "Parameter", 0 );
               }

               nRC = SetCursorNextEntity( vT, "Operation", 0 );
            }

            DropView( vT );
            // *** TEMPORARY HACK ***

            break;

         default:
            pchSourceFileEntity = "SourceFile";
            break;
      }

      // Loop through each of the source files and check to see if they
      // need to be parsed/generated.
      CreateViewFromViewForTask( &vMetaCopy, vMeta, 0 );

      for ( nRC = SetCursorFirstEntity( vMetaCopy, pchSourceFileEntity, 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vMetaCopy, pchSourceFileEntity, 0 ) )
      {
         zCHAR szXPG_DateTime[ 20 ];
         zCHAR szSourceDateTime[ 20 ];
         zCHAR szSourceBase[ 20 ];
         zCHAR szMsg[ 200 ];
         zBOOL bParsed = FALSE;

         // If the language type is .C, then we don't need to parse/generate so skip it.
         if ( CompareAttributeToString( vMetaCopy, pchSourceFileEntity,
                                        "LanguageType", "V" ) != 0 )
         {
            continue;  // Continue with next SourceFile.
         }

         // We have dialogs, that contain only resources and no source code.
         // skip them, too.
         if ( SetCursorFirstEntity( vMetaCopy, "Operation", NULL ) != zCURSOR_SET )
            continue;  // Continue with next SourceFile.

         // Reset Source, Target and XPG name to be just the directory.
         szTargetFile[ nTargetDirLth ] = 0;
         szSourceFile[ nSourceDirLth ] = 0;
         szXPG[ nExecDirLth ] = 0;

         // Retrieve the source file name and XPG name.
         GetAddrForAttribute( &pchPtr, vMetaCopy, pchSourceFileEntity, "Subdirectory" );
         strcat_s( szSourceFile, zsizeof( szSourceFile ), pchPtr );
         SysAppendcDirSep( szSourceFile );
         GetAddrForAttribute( &pchPtr, vMetaCopy, pchSourceFileEntity, "Name" );
         strcat_s( szSourceFile, zsizeof( szSourceFile ), pchPtr );
         strcat_s( szTargetFile, zsizeof( szTargetFile ), pchPtr );
         strcat_s( szSourceFile, zsizeof( szSourceFile ), ".VML" );
         strcat_s( szXPG, zsizeof( szXPG ), pchPtr );
         if ( cpcGenLang[ 0 ] == 'J' )
            strcat_s( szXPG, zsizeof( szXPG ), ".XPJ" );
         else
            strcat_s( szXPG, zsizeof( szXPG ), ".XPG" );

         strcpy_s( szSourceBase, zsizeof( szSourceBase ), pchPtr );

         sprintf_s( szMsg, zsizeof( szMsg ), "Checking %s for parse ... ", szSourceFile );
         MB_SetMessage( vSubtask, 0, szMsg );

         fnGetFileDateTime( vSubtask, szSourceFile, szSourceDateTime, zsizeof( szSourceDateTime ) );
         fnGetFileDateTime( vSubtask, szXPG, szXPG_DateTime, zsizeof( szXPG_DateTime ) );

         // Check to see if we need to parse the file.
         if ( szXPG_DateTime[ 0 ] == 0 || bAlwaysParse ||
              zstrcmpi( szSourceDateTime, szXPG_DateTime ) > 0 )
         {
            sprintf_s( szMsg, zsizeof( szMsg ), "Parsing %s...", szSourceFile );
            MB_SetMessage( vSubtask, 0, szMsg );
            TraceLineS( "fnParseGenTarget: ", szMsg );

            // If the dialog is not checked out, the view is readonly.
            // Change the view to updatable for parsing. We don't have to
            // set it back to readonly because the view will be dropped.
            if ( !MiGetUpdateForView( vMetaCopy ) )
               SetViewUpdate( vMetaCopy );

            bParsed = FALSE;  // if parsed and not in "parse only mode", generate it! (dks 2009.10.14)
            if ( ParseSource( vSubtask, vMetaCopy, zPARSE_ACTION_FULL, zPARSE_LIMIT_SOURCE,
                              zPARSE_SOURCE_FILE, szSourceBase, "", lType, 0, cpcGenLang ) < 0 )
            {
               char szContinueParseGen[ 2 ];

               sprintf_s( szMsg, zsizeof( szMsg ), "Error parsing %s.\nSee error file for a list of the errors.",
                         szSourceFile );

               SysReadZeidonIni( -1, "[Workstation]", "ContinueParseGen", szContinueParseGen, zsizeof( szContinueParseGen ) );

               if ( szContinueParseGen[ 0 ] != 'Y' )
               {
                  MessageSend( vSubtask, "ED00101", "Zeidon Compiler", szMsg,
                               zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               }
               else
                  TraceLineS( szMsg, "" );

               if ( nMetaRC <= 0 )
                  DropMetaOI( vSubtask, vMeta );

               DropView( vMetaCopy );
               return( FALSE );
            }
            else
               bParsed = TRUE;

            sprintf_s( szMsg, zsizeof( szMsg ), "Completed parse of %s...", szSourceFile );
            MB_SetMessage( vSubtask, 0, szMsg );
         }

         // If we're only parsing the VML, continue with the next source file.
         if ( bParseOnly )
            continue;

         // Change the ".VML" in szSourceFile to ".C" or whatever is required for java.
         if ( cpcGenLang[ 0 ] == 'J' )
         {
            if ( lType == zREFER_LOD_META ) // 2007
               strcpy_s( szTargetFile + zstrlen( szTargetFile ), zsizeof( szTargetFile ) - zstrlen( szTargetFile ), "_Object.java" );
            else
            if ( lType == zREFER_DIALOG_META ) // 2011
               strcpy_s( szTargetFile + zstrlen( szTargetFile ), zsizeof( szTargetFile ) - zstrlen( szTargetFile ), "_Dialog.java" );
            else
            if ( lType == zREFER_GOPGRP_META ) // 2014
               strcpy_s( szTargetFile + zstrlen( szTargetFile ), zsizeof( szTargetFile ) - zstrlen( szTargetFile ), "_Operation.java" );
            else
            {
               TraceLineI( "Make Target Unexpected Meta Type: ", lType );
               MessageSend( vSubtask, "ED00101", "Zeidon Compiler", "unexpected meta type",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            }
         }
         else
            strcpy_s( szTargetFile + zstrlen( szTargetFile ), zsizeof( szTargetFile ) - zstrlen( szTargetFile ), ".c" );

         // Now retrieve the date/time of the .c/.java file and the .XPG.  We retrieve the date/time of the .XPG again because it might
         // have been changed by a parse.
         fnGetFileDateTime( vSubtask, szTargetFile, szSourceDateTime, zsizeof( szSourceDateTime ) );
         fnGetFileDateTime( vSubtask, szXPG, szXPG_DateTime, zsizeof( szXPG_DateTime ) );

         // Check to see if we need to generate the file.
         if ( szTargetFile[ 0 ] == 0 || bAlwaysGen || bParsed || zstrcmpi( szXPG_DateTime, szSourceDateTime ) > 0 )
         {
            zCHAR szMsg[ 200 ];

            sprintf_s( szMsg, zsizeof( szMsg ), "Generating %s...", szTargetFile );
            TraceLineS( szMsg, "" );
            MB_SetMessage( vSubtask, 0, szMsg );

            if ( Generate3GL( vSubtask, vSubtask, vMetaCopy, szSourceBase, cpcGenLang, lType ) < 0 )
            {
               char szContinueParseGen[ 2 ];

               SysReadZeidonIni( -1, "[Workstation]", "ContinueParseGen", szContinueParseGen, zsizeof( szContinueParseGen ) );
               sprintf_s( szMsg, zsizeof( szMsg ), "Error generating %s.", szTargetFile );
               if ( szContinueParseGen[ 0 ] != 'Y' )
               {
                  MessageSend( vSubtask, "ED00102", "Zeidon Compiler", szMsg,
                               zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

                  if ( nMetaRC <= 0 )
                     DropMetaOI( vSubtask, vMeta );

                  DropView( vMetaCopy );
                  return( FALSE );
               }
               else
                  TraceLineS( szMsg, "" );
            }

            sprintf_s( szMsg, zsizeof( szMsg ), "Completed generation of %s ...", szTargetFile );
            TraceLineS( szMsg, "" );
            MB_SetMessage( vSubtask, 0, szMsg );

            // The gen function creates a named view _GenFromXPG, which we want to get rid of here because they accrue in core.
            if ( GetViewByName( &vT, "_GenFromXPG", vSubtask, zLEVEL_SUBTASK ) > 0 )
            {
            // MessageSend( vSubtask, "ED00102", "Zeidon Compiler",
            //              "Before Drop _GenFromXPG",
            //              zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               DropObjectInstance( vT );
            }
         }

      } // for each vMeta.SourceFile...

      if ( nMetaRC <= 0 )
         DropMetaOI( vSubtask, vMeta );

      DropView( vMetaCopy );
   }  // For each vTaskLPLR.Meta...

   return( TRUE );

} // fnParseGenTarget

#ifndef __WIN32__

zOPER_EXPORT BOOL OPERATION
fnEnumFindWindow( HWND hWnd, LPARAM pchWindowTitle )
{
   zCHAR szWindowText[ 100 ];

   GetWindowText( hWnd, szWindowText, 99 );
   if ( zstrcmp( szWindowText, pchWindowTitle ) == 0 )
      return( 0 );

   return( 1 );
}

void
fnLocalSysWait( zULONG ulMilliseconds )
{
   zLONG    lStart, lMill;

   lStart = GetCurrentTime( );

   do
   {
      Yield( );
      lMill = GetCurrentTime( ) - lStart;

   } while ( lMill < (zLONG) ulMilliseconds );
}

#endif

# pragma warning(disable: 4996)

// To execute a make with an existing batch file/make file.
zSHORT LOCALOPER
fnExecMake( zVIEW   vSubtask,
            zVIEW   vTaskLPLR,
            zCPCHAR pchTargetExecutableName,
            zCPCHAR szCommand )
{
   UINT      hInst;
// OSVERSIONINFO osvi;
   DWORD     WinVersion;
   zSHORT    nVersion;
   zSHORT    nCompilerType;
   zBOOL     bStarted;
   zCHAR     szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zCHAR     szWinOS2[ 30 ];
   zSHORT    nRC = 0;

#ifdef __WIN32__

   STARTUPINFO         StartInfo;
   PROCESS_INFORMATION ProcInfo;

   // Write the .HNM files if they don't already exist.
   CreateMakeIncludeFile( vSubtask, vTaskLPLR, FALSE );

#else

   // Write a PIF file if it doesn't already exist.
   CreatePIF_File( vTaskLPLR, 'N' );

#endif

   nCompilerType = FindCompilerType( vTaskLPLR );

   sprintf_s( szMsg, zsizeof( szMsg ), "Compiling %s...", pchTargetExecutableName );
   MB_SetMessage( vSubtask, 0, szMsg );

   TraceLineS( "(tzedcm2d) Command for Make = ", szCommand );

   bStarted = FALSE;
   hInst = 0;

   WinVersion = GetVersion();
   nVersion = LOBYTE(LOWORD(WinVersion))*100 + HIBYTE( LOWORD( WinVersion ) );

// osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
// GetVersionEx( &osvi );
// nVersion = (zSHORT) (osvi.dwMajorVersion * 100 + osvi.dwMinorVersion);

   TraceLineI( "(tzedcm2d) Compiling for Win Version = ", (zLONG) nVersion );
   switch ( nCompilerType )
   {
      case zCOMP_TYPE_MSVC15:
      {
         zCHAR szCommandNT[ zMAX_FILESPEC_LTH + 1 ];
         zCHAR szDefaultPIF[ zMAX_FILESPEC_LTH + 1 ];
         zCHAR cPIF[ 4 ] = { 0 };
         zLONG hFile;

         // Windows NT 3.51 show us always Windows version 3.10, but there is a different format in the PIF, which we use to detect NT.
         GetWindowsDirectory( szDefaultPIF, 300 );
         strcat_s( szDefaultPIF, zsizeof( szDefaultPIF ), "\\_DEFAULT.PIF");
         hFile = SysOpenFile( vSubtask, szDefaultPIF, COREFILE_READ );
         if ( hFile >= 0 )
         {
            SysReadFile( vSubtask, hFile, (zPCHAR) cPIF, 2 );
            SysCloseFile( vSubtask, hFile, 0 );
         }

         TraceLineX( "(tzedcm2d) PIF file ID = ", *(zPLONG) cPIF );

         // Compiling under NT starts a shell (COMMAND.COM) without a PIF we need no PIF, because under NT the COMMAND.COM is a real windows application.
         if ( nVersion > 311 || cPIF[ 1 ] == '\xC9' )
         {
            // Build new Command "COMMAND.COM /C Path\\ZMAKE.BAT"
            if ( SysGetOS_Info( 0 ) == zOS_WINDOWSNT )
               strcpy_s( szCommandNT, zsizeof( szCommandNT ), "CMD.EXE ");
            else
               strcpy_s( szCommandNT, zsizeof( szCommandNT ), "COMMAND.COM ");

            if ( CompareAttributeToString( vTaskLPLR, "Compiler", "CloseWindowAfterCompile", "Y" ) == 0 )
            {
               strcat_s( szCommandNT, zsizeof( szCommandNT ), "/C ");
            }
            else
               strcat_s( szCommandNT, zsizeof( szCommandNT ), "/K ");

            strcat_s( szCommandNT, zsizeof( szCommandNT ), szCommand );
            szCommandNT[  strlen( szCommandNT ) - 12  ] = 0; // remove PIF file
            strcat_s( szCommandNT, zsizeof( szCommandNT ), "ZMAKE.BAT" );

            TraceLineS( "(tzedcm2d) New Command for Make = ", szCommandNT );

#ifdef __WIN32__
            zmemset( &StartInfo, 0, sizeof( StartInfo ) );
            StartInfo.cb = sizeof( StartInfo );
            StartInfo.lpTitle = szMsg;

            if ( CreateProcess( 0, szCommandNT, 0, 0, FALSE, CREATE_NEW_CONSOLE,
                                0, 0, &StartInfo, &ProcInfo ) == 0 )
            {
               TraceLineI( "(tzedcmpd) CreateProcess error # = ", GetLastError( ) );
               MB_SetMessage( vSubtask, 0, "Error on compile" );
               MessageSend( vSubtask, "ED00102", "Zeidon Compiler",
                            "Error starting compile",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

               return( zCALL_ERROR );
            }
#else
            if ( WinExec( szCommandNT, SW_SHOW ) <= 32 )
            {
               MB_SetMessage( vSubtask, 0, "Error on compile" );
               MessageSend( vSubtask, "ED00102", "Zeidon Compiler",
                            "Error starting compile",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

               return( zCALL_ERROR );
            }
#endif
         }
         else
         {
            // Compiling for windows 3.1 runs with PIF for enhanced mode
            hInst = WinExec( szCommand, SW_SHOW );
            if ( hInst <= 32 )
            {
               switch ( hInst )
               {
                  case 0:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. System out of memory" );
                     break;

                  case 2:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Command shell not found." );
                     break;

                  case 3:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Path was not found. Check the specification of the Compiler directory." );
                     break;

                  case 5:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Sharing or network-protection error." );
                     break;

                  case 8:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Insufficient memory. Close apps and try again. " );
                     break;

                  default:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Reason code = %d", hInst );
                     break;
               }

               MB_SetMessage( vSubtask, 0, "Error on compile" );
               MessageSend( vSubtask, "ED00102", "Zeidon Compiler",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

               return( zCALL_ERROR );
            }
         }

         break;
      }

      default:
         // Compiling for OS/2 under WinOS2 runs with PIF for standard mode but WinOS2 could be still in enhanced mode???

         // we set WinOS2=Y in ZEIDON.INI, if we are under WinOS2 otherwise, we just create a make file but don't compile
         SysReadZeidonIni( -1, "[Workstation]", "WinOS2", szWinOS2, zsizeof( szWinOS2 ) );
         if ( szWinOS2[ 0 ] == 'Y' || szWinOS2[ 0 ] == 'y')
         {
            hInst = WinExec( szCommand, SW_SHOW );
            if ( hInst <= 32 )
            {
               switch ( hInst )
               {
                  case 0:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. System out of memory" );
                     break;

                  case 2:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Command shell not found." );
                     break;

                  case 3:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Path was not found. Check the specification of the Compiler directory." );
                     break;

                  case 5:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Sharing or network-protection error. " );
                     break;

                  case 8:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Insufficient memory. Close apps and try again. " );
                     break;

                  default:
                     sprintf_s( szMsg, zsizeof( szMsg ), "Error running compile. Reason code = %d", hInst );
                     break;
               }

               MB_SetMessage( vSubtask, 0, "Error on compile" );
               MessageSend( vSubtask, "ED00102", "Zeidon Compiler",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

               return( zCALL_ERROR );
            }
         }
         else
         {
            sprintf_s( szMsg, zsizeof( szMsg ), "Makefile for %s has been generated.", pchTargetExecutableName );
            MB_SetMessage( vSubtask, 0, szMsg );
            return( 0 );
         }

   } // switch ( nCompilerType )...

#ifdef __WIN32__

   WaitForSingleObject( ProcInfo.hProcess, INFINITE );

#else
   // Re-use szMsg as a buffer.  You probably shouldn't change the following
   // string because it's used in other places.
   strcpy_s( szMsg, zsizeof( szMsg ), "Compiling Zeidon Target..." );

   // Give the compile task 10 seconds to appear
   for ( nRC = 0; nRC < 5; nRC++ )
   {
      fnLocalSysWait( 2000 );
      if ( EnumWindows( fnEnumFindWindow, (LPARAM) szMsg ) == 0 )
         break;
   }

   // Give the compile task 5 minutes seconds to disappear
   for ( nRC = 0; nRC < ( 60 * 5 ); nRC ++ )
   {
      fnLocalSysWait( 1000 );  // Wait a second.
      if ( EnumWindows( fnEnumFindWindow, (LPARAM) szMsg ) != 0 )
         break;
   }
#endif

   sprintf_s( szMsg, zsizeof( szMsg ), "Make of %s completed", pchTargetExecutableName );
   MB_SetMessage( vSubtask, 0, szMsg );
   return( 0 );

} // fnExecMake

// To make the currently selected C target.
zOPER_EXPORT zSHORT OPERATION
MakeTarget( zVIEW vSubtask, zCPCHAR cpcGenLang )
{
   zVIEW     vTaskLPLR;
   zCHAR     szCommand[ zMAX_FILENAME_LTH + 1 ];
   zBOOL     bCompileAllTargets;
// zLONG     lKeepAlive;
   zLONG     lMakeOption;
   zSHORT    nRC;

   SysUpdateZeidonIni( "[Workstation]", "GenLang", cpcGenLang );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vTaskLPLR, "Compiler" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "ED00100", "Zeidon Compiler",
                   "No Compiler Specification currently specified.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( SetCursorFirstSelectedEntity( vTaskLPLR, "TargetExecutable", 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "ED00104", "Zeidon Compiler",
                   "No Target currently selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

// lKeepAlive = MiGetSetViewTaskAppFlags( vTaskLPLR, 1, 0, 3 ); // set KeepAlive ON

   // Parse and gen any source files that might need it and are selected.
   for ( nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "TargetExecutable", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTaskLPLR, "TargetExecutable", 0 ) )
   {
   // MessageBox( 0, "Parse TaskLPLR", "Check it out", MB_OK );
      if ( !fnParseGenTarget( vSubtask, vTaskLPLR, cpcGenLang ) )  // dks???
      {
         char szContinueParseGen[ 2 ];

         SysReadZeidonIni( -1, "[Workstation]", "ContinueParseGen", szContinueParseGen, zsizeof( szContinueParseGen ) );

         if ( szContinueParseGen[ 0 ] != 'Y' )
         {
            // Parse/Gen failed.  Determine if user wants to continue.
            if ( MessagePrompt( vSubtask, "ED00200", "Zeidon Compiler",
                                "Parse/Gen failed.  Continue with 'Make Target'?",
                                1, zBUTTONS_YESNO, zRESPONSE_YES,
                                zICON_STOP ) == zRESPONSE_NO )
            {
               MB_SetMessage( vSubtask, 0, "'Make All' failed" );
               return( zCALL_ERROR );
            }
         }
      }
   }

// if ( lKeepAlive == 0 ) // it was originally off
//    MiGetSetViewTaskAppFlags( vTaskLPLR, 1, 0, 1 ); // reset KeepAlive to OFF

   MB_SetMessage( vSubtask, 0, "Completed parse/generation phase ..." );

   // Retrieve the make option.  Current values are:
   //    1  Parse only--parse only if needed.
   //    2  Parse only--force parce.
   //    3  Build -- parse/gen/compile if needed.
   //    4  Force Parse--implicitly forces gen/compile.
   //    5  Force Gen--parse if needed;implicitly forces compile.
   //    6  Force Compile---parse/gen if needed.
   //
   // DGC 2/11/97
   // Added following option for use from ApplA code.  This will parse AND
   // generate VML if needed.
   //    7  Parse/Gen if needed -- no compile!
   GetIntegerFromAttribute( &lMakeOption, vTaskLPLR, "Compiler", "MakeOption" );

   // If we're only supposed to parse the VML, then return.
   if ( cpcGenLang[ 0 ] == 'J' || lMakeOption <= 2 || lMakeOption == 7 )
   {
      MB_SetMessage( vSubtask, 0, "Make completed" );
      return( 0 );
   }

   bCompileAllTargets = (lMakeOption == 6);
   nRC = CreateMakefileForAllTargets( vSubtask, vTaskLPLR, 0, bCompileAllTargets, szCommand, zsizeof( szCommand ), cpcGenLang );
   if ( nRC == 0 )
   {
      MB_SetMessage( vSubtask, 0, "Build of Makefile failed" );
      return( zCALL_ERROR );
   }
   else
   if ( nRC > 1 )
   {
      if ( cpcGenLang[ 0 ] == 'J' )
         MessageSend( vSubtask, "ED00999", "Zeidon Compiler",
                      "Java generation completed.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      else
         MessageSend( vSubtask, "ED00999", "Zeidon Compiler",
                      "Dialog needs to be saved to synchronize\nresource specification.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   // start the make
   fnExecMake( vSubtask, vTaskLPLR, "all executables", szCommand );

   return( 0 );

} // MakeTarget

// To make the currently selected Java target.
zOPER_EXPORT zSHORT OPERATION
MakeC_Target( zVIEW vSubtask )
{
   return( MakeTarget( vSubtask, "C" ) );
}

// To make the currently selected Java target.
zOPER_EXPORT zSHORT OPERATION
MakeJavaTarget( zVIEW vSubtask )
{
   return( MakeTarget( vSubtask, "Java" ) );
}

#if 0 // this does not seem to be used from anywhere??? DKS 2000.11.27
zOPER_EXPORT zSHORT OPERATION
MakeAllTargets( zVIEW vSubtask, zVIEW vTaskLPLR )
{
   zVIEW     vTempTaskLPLR;
   zCHAR     szCommand[ zMAX_FILENAME_LTH + 1 ];
   zSHORT    nRC;
   zBOOL     bCompileAllTargets;
   zLONG     lMakeOption;

   if ( vTaskLPLR == 0 )
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTaskLPLR, "Compiler" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "ED00100", "Zeidon Compiler",
                   "No Compiler Specification currently specified.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( zCALL_ERROR );
   }

   // Create a new LPLR view so that we can change the cursors without messing anyone up.
   CreateViewFromViewForTask( &vTempTaskLPLR, vTaskLPLR, 0 );

   // Parse and gen any source files that might need it.
   for ( nRC = SetCursorFirstEntity( vTempTaskLPLR, "TargetExecutable", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vTempTaskLPLR, "TargetExecutable", 0 ) )
   {
      if ( !fnParseGenTarget( vSubtask, vTempTaskLPLR, cpcGenLang ) )
      {
         // Parse/Gen failed.  Ask the user if we are to continue.
         if ( MessagePrompt( vSubtask, "ED00200", "Zeidon Compiler",
                             "Parse/Gen failed.  Continue with 'Make All'?",
                             1, zBUTTONS_YESNO, zRESPONSE_YES,
                             zICON_STOP ) == zRESPONSE_NO )
         {
            MB_SetMessage( vSubtask, 0, "'Make All' failed" );
            DropView( vTempTaskLPLR );
            return( zCALL_ERROR );
         }
      }
   }

   // Retrieve the make option.  Current values are:
   //    1  Parse only--parse only if needed.
   //    2  Parse only--force parce.
   //    3  Build -- parse/gen/compile if needed.
   //    4  Force Parse--implicitly forces gen/compile.
   //    5  Force Gen--parse if needed;implicitly forces compile.
   //    6  Force Compile---parse/gen if needed.
   //
   // DGC 2/11/97
   // Added following option for use from ApplA code.  This will parse AND
   // generate VML if needed.
   //    7  Parse/Gen if needed -- no compile!
   GetIntegerFromAttribute( &lMakeOption, vTempTaskLPLR, "Compiler", "MakeOption" );

   // If we're only supposed to parse the VML, then return.
   if ( lMakeOption <= 2 || lMakeOption == 7 )
   {
      MB_SetMessage( vSubtask, 0, "Make completed" );
      return( 0 );
   }

   // Now we need to set the selection of all the Targets so that they get
   // included into the makefile.
   for ( nRC = SetCursorFirstEntity( vTempTaskLPLR, "TargetExecutable", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTempTaskLPLR, "TargetExecutable", 0 ) )
   {
      SetSelectStateOfEntity( vTempTaskLPLR, "TargetExecutable", 1 );
   }

   bCompileAllTargets = (lMakeOption == 6);
   if ( !CreateMakefileForAllTargets( vSubtask, vTempTaskLPLR, 0, bCompileAllTargets, szCommand, zsizeof( szCommand ), cpcGenLang ) )
   {
      MB_SetMessage( vSubtask, 0, "Build of Makefile failed" );
      DropView( vTempTaskLPLR );
      return( zCALL_ERROR );
   }

   DropView( vTempTaskLPLR );

   // start the make
   fnExecMake( vSubtask, vTaskLPLR, "all executables", szCommand );

   return( 0 );

} // MakeAllTargets.
#endif

#ifdef __WIN32__

zSHORT LOCALOPER
fnWriteInclude_MSVC50( zVIEW  vTaskLPLR,
                       zPCHAR pchLine,
                       zBOOL  bOverwrite )
{
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szFileName2[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szDT[ 21 ];
   zCHAR  szDTFormatted[ 21 ];
   zPCHAR pch;
   zPCHAR pch2;
   zPCHAR pchAppName;
   zPCHAR pchMakefileDir;
   zCHAR  szMakefileDir[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pchExecDir;
   zCHAR  szExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szEnvDir[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR pchPgmSrcDir;
   zPCHAR pchDebugOption;
   zPCHAR pchObj;
   zCHAR  szObj[ zMAX_FILESPEC_LTH + 1 ];
   zBOOL  bPath;
   zLONG  hFile;
   zCHAR  szBaseDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

   // Base directory for relative paths.
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szBaseDir, zsizeof( szBaseDir ), szFileName );

   // get dirs
   GetAddrForAttribute( &pchAppName, vTaskLPLR, "LPLR", "Name" );
   GetAddrForAttribute( &pchDebugOption, vTaskLPLR, "Compiler", "CompileWithDebug" );
   GetAddrForAttribute( &pchExecDir, vTaskLPLR, "LPLR", "ExecDir" );
   GetAddrForAttribute( &pchPgmSrcDir, vTaskLPLR, "LPLR", "PgmSrcDir" );
   GetAddrForAttribute( &pchMakefileDir, vTaskLPLR, "Compiler", "MakefileDir" );
   if ( pchMakefileDir == 0 || *pchMakefileDir == 0 )
      pchMakefileDir = pchPgmSrcDir;

   // Build relative path if necessary; Base directory is Component Source Dir
   SysConvertEnvironmentString( szMakefileDir, zsizeof( szMakefileDir ), pchMakefileDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szMakefileDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Makefile Directory" ) == -1 )
   {
      return( 0 );
   }

   pchMakefileDir = szMakefileDir;

   //=======================================================================
   // Create zEnv.HNM
   //=======================================================================

   // Check to see if the file already exists.  If it does then skip it.
   if ( bOverwrite == FALSE &&
        SysOpenFile( vTaskLPLR, szFileName, COREFILE_EXISTS ) != -1 )
   {
      return( FALSE );
   }

   strcpy_s( szFileName, zsizeof( szFileName ), pchMakefileDir );
   SysAppendcDirSep( szFileName );
   strcat_s( szFileName, zsizeof( szFileName ), "ZENV.HNM" );
   hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
   if ( hFile == -1 )
      return( FALSE );

   SWL( vTaskLPLR, "#" );
   SWL( vTaskLPLR, "#  zEnv.HNM" );
   SWL( vTaskLPLR, "#  Make Include File (MSVC 5.0)." );
   SysGetDateTime( szDT, zsizeof( szDT ) );
   UfFormatDateTime( szDTFormatted, zsizeof( szDTFormatted ), szDT, "YYYY.MM.DD HH:MI:SS" );
   sprintf_s( pchLine, 10000, "#  Generated at %s", szDTFormatted);
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, "#" );

   sprintf_s( pchLine, 10000, "APPNAME    = %s", pchAppName );
   SWL( vTaskLPLR, pchLine );

   sprintf_s( pchLine, 10000, "MODULE_DIR = %s", "" );   // source sub-dir.
   SWL( vTaskLPLR, pchLine );

   GetAddrForAttribute( &pch, vTaskLPLR, "Compiler", "TargetExecutableDir" );
   if ( pch == 0 || *pch == 0 )
      pch = pchExecDir;

   // Build relative path if necessary; Base directory is Component Source Dir
   strcpy_s( szExecDir, zsizeof( szExecDir ), pch );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szExecDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Target Executable Directory" ) == -1 )
   {
      return( 0 );
   }

   SysConvertEnvironmentString( szFileName2, zsizeof( szFileName2 ), szExecDir );
   pch = szFileName2;

   sprintf_s( pchLine, 10000, "TARGET_DIR = %s", pch );
   SWL( vTaskLPLR, pchLine );

   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), pchPgmSrcDir );
   sprintf_s( pchLine, 10000, "SRC_DIR    = %s$(MODULE_DIR)", szFileName );
   SWL( vTaskLPLR, pchLine );

   GetAddrForAttribute( &pchObj, vTaskLPLR, "Compiler", "TargetObjectDir" );
   if ( pchObj == 0 || *pchObj == 0 )
      pchObj = pchPgmSrcDir;

   // Build relative path if necessary; Base directory is Component Source Dir.
   strcpy_s( szObj, zsizeof( szObj ), pchObj );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szObj, szBaseDir, zMAX_FILENAME_LTH - 2, "Obj/Lib directory" ) == -1 )
   {
      return( 0 );
   }

   SysConvertEnvironmentString( szFileName2, zsizeof( szFileName2 ), szObj );
   pchObj = szFileName2;

   sprintf_s( pchLine, 10000, "OBJ_DIR    = %s", pchObj );
   SWL( vTaskLPLR, pchLine );

   GetAddrForAttribute( &pch, vTaskLPLR, "Compiler", "EnvironmentDir" );
   if ( pch == 0 || *pch == 0 )
      pch = pchPgmSrcDir;

   // Build relative path if necessary; Base directory is Component Source Dir.
   strcpy_s( szEnvDir, zsizeof( szEnvDir ), pch );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szEnvDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Resource Directory" ) == -1 )
   {
      return( 0 );
   }

   SysConvertEnvironmentString( szFileName2, zsizeof( szFileName2 ), szEnvDir );
   pch = szFileName2;

   sprintf_s( pchLine, 10000, "ENV_DIR    = %s", pch );
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, " " );

   SWL( vTaskLPLR, "# Windows and Zeidon Core libraries" );
   SWL( vTaskLPLR, "BASELINKDLLS = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \\" );
   SWL( vTaskLPLR, "               advapi32.lib shell32.lib ole32.lib kzoengaa.lib msvcrt.lib \\" );
   SWL( vTaskLPLR, "               zdrapp.lib zdctl.lib tzctl.lib kzrpsrco.lib tzvmlip.lib \\" );
   SWL( vTaskLPLR, "               truegrid.lib zsemail.lib z2mp.lib" );
   SWL( vTaskLPLR, "" );

   // TMV 2000.03.02 User should not link against debug libs if not necessary
   // SWL( vTaskLPLR, "CL_OPT =  /Gz /MDd /W3 /GmX3f /Zip /c /nologo /TP \\" );
   SWL( vTaskLPLR, "CL_OPT =  /Gz /W3 /GX3f /Zp /c /nologo /TP \\" );
   SWL( vTaskLPLR, "          /D ZEIDON /D __WIN32__ /D _WINDOWS \\" );
   SWL( vTaskLPLR, "          /Fd$(TARGET_DIR)\\$(MODULE).pdb $(CL_OPT)" );
   SWL( vTaskLPLR, "LINK_OPT = /subsystem:windows /dll /machine:I386 /NOD" );
   SWL( vTaskLPLR, "" );

   if ( *pchDebugOption == 'A' )
      SWL( vTaskLPLR, "DEBUG = 1" );
   else
   if ( *pchDebugOption == 'N' )
      SWL( vTaskLPLR, "DEBUG =" );

   SWL( vTaskLPLR, "!IF \"$(DEBUG)\" == \"1\"" );
   SWL( vTaskLPLR, "CL_OPT = -DDEBUG -D_DEBUG -Od /Gm /Zi /MDd $(CL_OPT)" );
   SWL( vTaskLPLR, "LINK_OPT = /debug:full $(LINK_OPT)" );
   // TMV 2000.03.02 User should not link against debug libs if not necessary
   SWL( vTaskLPLR, "!ELSE" );
   SWL( vTaskLPLR, "CL_OPT = /MD $(CL_OPT)" );
   SWL( vTaskLPLR, "!ENDIF" );

   SysCloseFile( vTaskLPLR, hFile, 0 );

   //=======================================================================
   // Create zCompile.HNM
   //=======================================================================

   strcpy_s( szFileName, zsizeof( szFileName ), pchMakefileDir );
   SysAppendcDirSep( szFileName );
   strcat_s( szFileName, zsizeof( szFileName ), "ZCOMPILE.HNM" );
   hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
   if ( hFile == -1 )
      return( FALSE );

   SWL( vTaskLPLR, "#" );
   sprintf_s( pchLine, 10000, "#  zCompile.HNM" );
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, "#  Make Include File (MSVC 5.0)." );
   SysGetDateTime( szDT, zsizeof( szDT ) );
   UfFormatDateTime( szDTFormatted, zsizeof( szDTFormatted ), szDT, "YYYY.MM.DD HH:MI:SS" );
   sprintf_s( pchLine, 10000, "#  Generated at %s", szDTFormatted );
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, "#" );

   // Delete all the ZeidonLib entities.
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ZeidonLib", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "ZeidonLib", 0 ) )
   {
      DeleteEntity( vTaskLPLR, "ZeidonLib", zREPOS_NONE );
   }

   SWL( vTaskLPLR, "" );
   SWL( vTaskLPLR, "ALL: _zSetEnv $(TARGET_DIR)\\$(MODULE).DLL" );
   SWL( vTaskLPLR, "" );

   // Set the DOS environment.
   GetAddrForAttribute( &pch,  vTaskLPLR, "Compiler", "CompilerDir" );
   GetAddrForAttribute( &pch2, vTaskLPLR, "Compiler", "Path" );
   SWL( vTaskLPLR, "# Set the DOS environment" );
   SWL( vTaskLPLR, "_zSetEnv:" );
   sprintf_s( pchLine, 10000, "   @set PATH=%s;%s;%s", pchExecDir, pch, pch2 );
   SWL( vTaskLPLR, pchLine );

   // Write Environment-Variable LIB in Makefile
   strcpy_s( pchLine, 10000, "   @set LIB=" );
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Lib", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "Lib", 0 ) )
   {
      GetStringFromAttribute( pchLine + zstrlen( pchLine ), 10000 - zstrlen( pchLine ), vTaskLPLR, "Lib", "LibDir" );
      strcat_s( pchLine, 10000, ";" );
   }

   strcat_s( pchLine, 10000, pchObj );
   SWL( vTaskLPLR, pchLine );

   // Write Environment-Variable INCLUDE in Makefile
   strcpy_s( pchLine, 10000, "   @set INCLUDE=" );
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Include", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "Include", 0 ) )
   {
      GetStringFromAttribute( pchLine + zstrlen( pchLine ), 10000 - zstrlen( pchLine ), vTaskLPLR, "Include", "IncludeDir" );
      strcat_s( pchLine, 10000, ";" );
   }

   strcat_s( pchLine, 10000, pchPgmSrcDir );
   SWL( vTaskLPLR, pchLine );
   SWL( vTaskLPLR, "" );

   // If there is an ExternalTarget entity with name "<ALL>", there are
   // some external modules that need to be linked to all targets.
   if ( SetCursorFirstEntityByString( vTaskLPLR, "ExternalTarget",
                                      "Name", "<ALL>", 0 ) >= zCURSOR_SET )
   {
      SWL( vTaskLPLR, "# Common external libraries/objects for all targets" );
      strcpy_s( pchLine, 10000, "BASELINKDLLS = $(BASELINKDLLS) " );

      // Add each ExternalLibFile to the LINKDLLS line.
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalLibFile", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalLibFile", 0 ) )
      {
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "ExternalLibFile", "Name" );

         // extract the Filename without Dirs...
         _splitpath( szFileName2, 0, 0, szFileName2, 0 );

         strcat_s( pchLine, 10000, szFileName2 );
         strcat_s( pchLine, 10000, " " );
      }

      // The following is a little trick.  The last zCHAR in pchLine is either a comma or a space.
      // Since we can't have a comma at the end, we change that last zCHAR to a null term.  Since
      // the only other possibility is a space, it doesn't hurt to change it to a null-term.
      pchLine[ zstrlen( pchLine ) - 1 ] = 0;

      SWL( vTaskLPLR, pchLine );

      strcpy_s( pchLine, 10000, "LINKOBJS = $(LINKOBJS) " );

      // Add each ExternalObjFile to the LINKOBJS line.
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalObjFile", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalObjFile", 0 ) )
      {
         GetAddrForAttribute( &pchObj, vTaskLPLR, "ExternalObjFile", "Name" );

         // extract the Filename without Dirs...
         _splitpath( pchObj, 0, 0, pchObj, 0 );

         // if no path given, use OBJ_DIR from LPLR
         for ( pch = pchObj, bPath = FALSE;
               !bPath && *pch;
               pch++ )
         {
           if ( *pch == '\\' || *pch == ':' )
             bPath = TRUE;
         }

         if ( bPath == FALSE )
           strcat_s( pchLine, 10000, "$(OBJ_DIR)\\" );

         strcat_s( pchLine, 10000, pchObj );
         strcat_s( pchLine, 10000, ".OBJ" );

         strcat_s( pchLine, 10000, " " );
      }

      // The following is a little trick.  The last zCHAR in pchLine is either a comma or a space.
      // Since we can't have a comma at the end, we change that last zCHAR to a null term.  Since
      // the only other possibility is a space, it doesn't hurt to change it to a null-term.
      pchLine[ zstrlen( pchLine ) - 1 ] = 0;

      SWL( vTaskLPLR, pchLine );
   }
   else
   {
      // The user didn't specify any special DLLs, so write and empty DLL line.
      SWL( vTaskLPLR, "# No common external libraries/objects for all targets defined." );
   }

   SWL( vTaskLPLR, "" );

   //
   //=======================================================================
   //
   SWL( vTaskLPLR, "!if \"$(OBJS)\" != \"\"" );
   SWL( vTaskLPLR, "" );
   SWL( vTaskLPLR, "OBJS_ALL = $(OBJS_ALL) $(OBJS)" );
   SWL( vTaskLPLR, "$(OBJS): $(SRC_DIR)\\$(@B).c" );
   SWL( vTaskLPLR, "   qspawn $(SRC_DIR)\\$(@B).ERR $(CC) @<<" );
   SWL( vTaskLPLR, "$(CL_OPT) -Fo$(OBJ_DIR)\\$(@B).obj $(SRC_DIR)\\$(@B).C" );
   SWL( vTaskLPLR, "<<" );
   SWL( vTaskLPLR, "" );
   SWL( vTaskLPLR, "!endif" );
   SWL( vTaskLPLR, "" );

   SWL(  vTaskLPLR, "!if \"$(CPP_OBJS)\" != \"\"" );
   SWL(  vTaskLPLR, "" );
   SWL(  vTaskLPLR, "OBJS_ALL = $(OBJS_ALL) $(CPP_OBJS)" );
   SWL(  vTaskLPLR, "TP = -TP" );
   SWL(  vTaskLPLR, "LINKDLLS = $(LINKDLLS) MFCS42.LIB" );
   SWL(  vTaskLPLR, "" );
   SWL(  vTaskLPLR, "$(CPP_OBJS): $(SRC_DIR)\\$(@B).c" );
   SWL(  vTaskLPLR, "   qspawn $(SRC_DIR)\\$(@B).ERR cl @<<" );
   SWL(  vTaskLPLR, "$(CL_OPT) $(TP)" );
   SWL(  vTaskLPLR, "$(SRC_DIR)\\$(@B).c" );
   SWL(  vTaskLPLR, "-Fo$(OBJ_DIR)\\$(@B).obj" );
   SWL(  vTaskLPLR, "<<" );
   SWL(  vTaskLPLR, "" );
   SWL(  vTaskLPLR, "!endif" );
   SWL(  vTaskLPLR, "" );

   SWL( vTaskLPLR, "$(ENV_DIR)\\$(MODULE).res:  $(ENV_DIR)\\$(MODULE).rc" );
   SWL( vTaskLPLR, "    rc -d__WIN32__  /fo$(ENV_DIR)\\$(MODULE).res $(ENV_DIR)\\$(MODULE).rc" );
   SWL( vTaskLPLR, "" );
   SWL( vTaskLPLR, "$(TARGET_DIR)\\$(MODULE).DLL : $(OBJS_ALL) $(LINKOBJS) $(ENV_DIR)\\$(MODULE).res" );
   SWL( vTaskLPLR, "   qspawn $(SRC_DIR)\\$(MODULE).LER link @<<" );
   SWL( vTaskLPLR, "$(LINK_OPT) $(LINKDLLS) $(BASELINKDLLS) $(OBJS_ALL) $(LINKOBJS) zdllinst.obj" );
   SWL( vTaskLPLR, "$(ENV_DIR)\\$(MODULE).res" );
   SWL( vTaskLPLR, "/OUT:$(TARGET_DIR)\\$(MODULE).DLL" );
   SWL( vTaskLPLR, "/IMPLIB:$(OBJ_DIR)\\$(MODULE).LIB" );
   SWL( vTaskLPLR, "/PDB:$(TARGET_DIR)\\$(MODULE).PDB" );
   SWL( vTaskLPLR, "/DEF:$(ENV_DIR)\\$(MODULE).dfb" );
   SWL( vTaskLPLR, "<<" );
   SWL( vTaskLPLR, "!if \"$(SKIP_MERGEDEF)\" != \"Y\"" );
   SWL( vTaskLPLR, "   dumpbin /exports $(TARGET_DIR)\\$(MODULE).DLL > $(ENV_DIR)\\$(MODULE).def" );
   SWL( vTaskLPLR, "   mergedef $(ENV_DIR)\\$(MODULE).dfb $(ENV_DIR)\\$(MODULE).def" );
   SWL( vTaskLPLR, "   qspawn +$(SRC_DIR)\\$(MODULE).LER link @<<" );
   SWL( vTaskLPLR, "$(LINK_OPT) $(LINKDLLS) $(BASELINKDLLS) $(OBJS) $(LINKOBJS) zdllinst.obj" );
   SWL( vTaskLPLR, "$(ENV_DIR)\\$(MODULE).res" );
   SWL( vTaskLPLR, "/OUT:$(TARGET_DIR)\\$(MODULE).DLL" );
   SWL( vTaskLPLR, "/IMPLIB:$(OBJ_DIR)\\$(MODULE).LIB" );
   SWL( vTaskLPLR, "/PDB:$(TARGET_DIR)\\$(MODULE).PDB" );
   SWL( vTaskLPLR, "/MAP:$(OBJ_DIR)\\$(MODULE).MAP" );
   SWL( vTaskLPLR, "/DEF:$(ENV_DIR)\\$(MODULE).tmp" );
   SWL( vTaskLPLR, "<<" );
   SWL( vTaskLPLR, "!endif" );

   SysCloseFile( vTaskLPLR, hFile, 0 );

   return( TRUE );

} // fnWriteInclude_MSVC50

zOPER_EXPORT zSHORT OPERATION
CreateMakeIncludeFile( zVIEW vSubtask,
                       zVIEW vTaskLPLR,
                       zBOOL bOverwrite )
{
   zPCHAR pchLine;
   zLONG  hLine;
   zSHORT nCompilerType;
   zSHORT nRC;

   if ( vTaskLPLR == 0 )
   {
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vTaskLPLR == 0 )
         return( FALSE );
   }

   // Check to make sure CompilerSpec is set.
   nRC = SetCursorFirstEntityByAttr( vTaskLPLR, "Compiler", "Name", vTaskLPLR, "LPLR",
                                     "CurrentCompilerSpec", 0 );
   if ( nRC < zCURSOR_SET )
   {
      zCHAR szMsg[ 200 ];

      strcpy_s( szMsg, zsizeof( szMsg ), "Current Compiler Specification is invalid:\n'" );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ), vTaskLPLR, "LPLR", "CurrentCompilerSpec" );
      strcat_s( szMsg, zsizeof( szMsg ), "'" );
      MessageSend( vSubtask, "LO00105", "Zeidon Compiler",
                   szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );
      return( FALSE );
   }

   // what compiler do we use?
   nCompilerType = FindCompilerType( vTaskLPLR );

   hLine = SysAllocMemory( (zCOREMEM) &pchLine, 10000, 0, zCOREMEM_ALLOC, 0 );
   nRC = fnWriteInclude_MSVC50( vTaskLPLR, pchLine, bOverwrite );
   SysFreeMemory( hLine );

   return( nRC );
}

#else

struct PIF_Struct386
{
   zCHAR     szDummy0     [   2 ];
   zCHAR     szWindowTitle[  31 ];
   zCHAR     szDummy1     [   3 ];
   zCHAR     szCommand    [  63 ];
   zCHAR     szDummy2     [   2 ];
   zCHAR     szDirectory  [  64 ];
   zCHAR     szDummy3     [  65 ];
   zCHAR     szDummy4     [   8 ];
   zCHAR     szDummy5     [ 131 ];
   zCHAR     szDummy6     [  93 ];
   zCHAR     szDummy7     [  19 ];
   zCHAR     szParameters [  64 ];
};

struct PIF_Struct286
{
   zCHAR     szDummy0     [   2 ];
   zCHAR     szWindowTitle[  30 ];
   zCHAR     szDummy1     [   4 ];
   zCHAR     szCommand    [  63 ];
   zCHAR     szDummy2     [   2 ];
   zCHAR     szDirectory  [  64 ];
   zCHAR     szParameters [  64 ];
   zCHAR     szDummy3     [   9 ];
   zCHAR     szDummy4     [ 129 ]; // 0
   zCHAR     szDummy5     [  67 ];
   zCHAR     szDummy6     [  21 ]; // 0
   zCHAR     szDummy7     [  62 ]; // ' '
   zCHAR     szDummy8     [  21 ];
   zCHAR     szDummy9     [   7 ]; // 0
};

// Creates a PIF file in the Environment directory specified in the
// Compiler Specification.
//
// Arguments: vTaskLPLR  - View to LPLR.  May be 0.
//            cOverwrite - Indicates whether the PIF file should be
//                         overwritten if it exists. Values are:
//                            'Y' - Always overwrite file.
//                            'N' - Never overwrite.
//                            'A' - Ask user if she wants it overwritten.
zSHORT LOCALOPER
CreatePIF_File( zVIEW vTaskLPLR, zCHAR cOverwrite )
{
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zLONG    hFile;
   zPCHAR   pchMakefileDir;
   zCHAR    szMakefileDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szBaseDir[ zMAX_FILESPEC_LTH + 1 ];
   zPCHAR   pchEnvDir;
   zCHAR    szEnvDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nCompilerType;
   unsigned int unFileAttr = 0;
   zCHAR    szMsg[ zLONG_MESSAGE_LTH + 1 ];
   zSHORT   nRC;

   TraceLineS( "(tzedcm2d) Trying to create PIF file", 0 );
   if ( vTaskLPLR == 0 )
   {
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vTaskLPLR == 0 )
         return( FALSE );
   }

   // Check to make sure CompilerSpec is set.
   nRC = SetCursorFirstEntityByAttr( vTaskLPLR, "Compiler", "Name", vTaskLPLR, "LPLR",
                                     "CurrentCompilerSpec", 0 );
   if ( nRC < zCURSOR_SET )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Current Compiler Specification is invalid:\n'" );
      GetStringFromAttribute( szMsg + zstrlen( szMsg ), zsizeof( szMsg ) - zstrlen( szMsg ),
                              vTaskLPLR, "LPLR", "CurrentCompilerSpec" );
      strcat_s( szMsg, zsizeof( szMsg ), "'" );
      MessageSend( vTaskLPLR, "LO00105", "Zeidon Compiler",
                   szMsg, zMSGQ_SYSTEM_ERROR, zBEEP );

      return( FALSE );
   }

   // what compiler do we use?
   nCompilerType = FindCompilerType( vTaskLPLR);

   // Base directory for relative paths.
   GetStringFromAttribute( szBaseDir, zsizeof( szBaseDir ), vTaskLPLR, "LPLR", "MetaSrcDir");

   // get dirs
   GetAddrForAttribute( &pchMakefileDir, vTaskLPLR, "Compiler", "MakefileDir" );
   if ( pchMakefileDir == 0 || *pchMakefileDir == 0 )
      GetAddrForAttribute( &pchMakefileDir, vTaskLPLR, "LPLR", "PgmSrcDir" );

   // Build relative path; base is Component Source directory
   strcpy_s( szMakefileDir, zsizeof( szMakefileDir ), pchMakefileDir );
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szMakefileDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Makefile Directory" ) == -1 )
   {
      return( FALSE );
   }

   pchMakefileDir = szMakefileDir;

   GetAddrForAttribute( &pchEnvDir, vTaskLPLR, "Compiler", "EnvironmentDir" );
   if ( pchEnvDir == 0 || *pchEnvDir == 0 )
      pchEnvDir = pchMakefileDir;

   // Build relative path; base is Component Source directory
   strcpy_s( szEnvDir, zsizeof( szEnvDir ), pchEnvDir);
   if ( IntGenFullQualPathFromRelPath( vTaskLPLR, szEnvDir, szBaseDir, zMAX_FILENAME_LTH - 2, "Resource Directory" ) == -1 )
   {
      return( FALSE );
   }

   pchEnvDir = szEnvDir;

   strcpy_s( szFileName, zsizeof( szFileName ), pchEnvDir );
   SysAppendcDirSep( szFileName );

   // we have different PIFs for Windows and OS/2 compiling
   if ( nCompilerType == zCOMP_TYPE_MSVC15 )
     strcat_s( szFileName, zsizeof( szFileName ), "ZCOMPILE.PIF" );
   else
     strcat_s( szFileName, zsizeof( szFileName ), "ZCOMPOS2.PIF" );

   if ( cOverwrite == 'N' || cOverwrite == 'A' )
   {
      if ( SysOpenFile( vTaskLPLR, szFileName, COREFILE_EXIST ) != -1 )
      {
         if ( cOverwrite == 'N' )
            return( FALSE );

         sprintf_s( szMsg, zsizeof( szMsg ), "Overwrite %s?", szFileName );
         if ( MessagePrompt( vTaskLPLR, "LO00106", "Zeidon Compiler", szMsg, 1,
                             zBUTTONS_YESNO, zRESPONSE_YES,
                             zICON_QUESTION ) == zRESPONSE_NO )
         {
            return( FALSE );
         }
      }
   }

   TraceLineS( "(tzedcm2d) PIF file = ", szFileName );

   // We have different PIF types for Windows and OS/2 compiling.
   if ( nCompilerType == zCOMP_TYPE_MSVC15 )
   {
     struct PIF_Struct386 sPIF_Info;

     // When working with Win 3.1, we are using and enhanced PIF struct
     zmemset( &sPIF_Info, 0, zsizeof( struct PIF_Struct386 ) );
     zmemcpy( sPIF_Info.szDummy0, "\0\x90", 2 );
     strcpy_s( sPIF_Info.szWindowTitle, zsizeof( sPIF_Info.szWindowTitle ), "Compiling Zeidon Target..." );
     zmemcpy( sPIF_Info.szDummy1, "\x02\x80\0", 3 );
     strcpy_s( sPIF_Info.szCommand, zsizeof( sPIF_Info.szCommand ), "COMMAND.COM" );
     strcpy_s( sPIF_Info.szDirectory, zsizeof( sPIF_Info.szDirectory ), pchMakefileDir );
     zmemcpy( sPIF_Info.szDummy2, "\x10\0", 2 );
     zmemcpy( sPIF_Info.szDummy4, "\x01\0\xff\x19\x50\0\0\x07", 8 );
     zmemcpy( sPIF_Info.szDummy6, "MICROSOFT PIFEX\0\x87\x01\0\0\x71\x01WINDOWS "
              "286 3.0\0\xa3\x01\x9d\x01\x06\0\0\0\0\0\0\0WINDOWS 386 3.0\0"
              "\xff\xff\xb9\x01\x68\0\xff\xff\xff\xff\x64\0\x32\0\0\x04\0\0\0"
              "\0\0\0\x04\x10\x02\0\x1f", 93 );

     // WIN: Look for close flag
     if ( CompareAttributeToString( vTaskLPLR, "Compiler", "CloseWindowAfterCompile", "Y" ) == 0 )
        strcpy_s( sPIF_Info.szParameters, zsizeof( sPIF_Info.szParameters ), "/e:2048 /c " );
     else
        strcpy_s( sPIF_Info.szParameters, zsizeof( sPIF_Info.szParameters ), "/e:2048 /k " );

     strcat_s( sPIF_Info.szParameters, zsizeof( sPIF_Info.szParameters ), pchEnvDir );
     SysAppendcDirSep( sPIF_Info.szParameters );
     strcat_s( sPIF_Info.szParameters, zsizeof( sPIF_Info.szParameters ), "ZMAKE.BAT" );

     // There appears to be a bug in Windows caching that sometimes obliterates changes we make
     // to the PIF file.  To get around this we set the read-only attribute on after creating the
     // PIF file.  Therefore we need to turn the read-only attribute OFF before we change the PIF
     // file.  When we're done, we'll reset the read-only attribute.  This code can be removed
     // for other OS's.
     _dos_getfileattr( szFileName, &unFileAttr );
     unFileAttr &= ~_A_RDONLY;
     _dos_setfileattr( szFileName, unFileAttr );

     hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
     SysWriteFile( vTaskLPLR, hFile,
                   (zPCHAR) &sPIF_Info, zsizeof( struct PIF_Struct386 ) );
     SysCloseFile( vTaskLPLR, hFile, 0 );

     // See comment above about setting the read-only attribute.
     unFileAttr |= _A_RDONLY;
     _dos_setfileattr( szFileName, unFileAttr );

   }
   else
   {
     struct PIF_Struct286 sPIF_Info;

     // WinOS2 can't run in 386 mode of windows, we are using the old PIF struct.
     zmemset( &sPIF_Info, 0, zsizeof( struct PIF_Struct286 ) );
     zmemcpy( sPIF_Info.szDummy0, "\0\x8a", 2 );
     zmemcpy( sPIF_Info.szDummy1, "\x80\0\x80\0", 4 );
     zmemcpy( sPIF_Info.szDummy2, "\x12\0", 2 );
     zmemcpy( sPIF_Info.szDummy3, "\x7f\x01\0\xff\x19\x50\0\0\x17", 9 );
     zmemcpy( sPIF_Info.szDummy3, "\x7f\x01\0\xff\x19\x50\0\0\x17", 9 );
     zmemcpy( sPIF_Info.szDummy5, "\xe0\x60MICROSOFT PIFEX\0\x87\x01\0\0\x71\x01"
                                   "WINDOWS 386 3.0\0\x05\x02\x9d\x01\x68\0"
                                   "\x80\x02\x80\0\x64\0\x32\0\0\x04\0\0\0"
                                   "\x04\0\0\x08\x10\x02\0\x1f", 67 );
     zmemset( sPIF_Info.szDummy7, ' ', 62);
     zmemcpy( sPIF_Info.szDummy8, "WINDOWS 286 3.0\0\xff\xff\x1b\x02\x06", 21);

     strcpy_s( sPIF_Info.szWindowTitle, zsizeof( sPIF_Info.szWindowTitle ), "Compiling Zeidon Target..." );
     strcpy_s( sPIF_Info.szCommand, zsizeof( sPIF_Info.szCommand ), "ZMAKEOS2.BAT" );
     strncpy_s( sPIF_Info.szDirectory, zsizeof( sPIF_Info.szDirectory ), pchMakefileDir, zsizeof( sPIF_Info.szDirectory ) - 1 );
     sPIF_Info.szDirectory[63] = 0;
     strcpy_s( sPIF_Info.szParameters, zsizeof( sPIF_Info.szParameters ), "" );

     _dos_getfileattr( szFileName, &unFileAttr );
     unFileAttr &= ~_A_RDONLY;
     _dos_setfileattr( szFileName, unFileAttr );

     hFile = SysOpenFile( vTaskLPLR, szFileName, COREFILE_WRITE );
     SysWriteFile( vTaskLPLR, hFile, (zPCHAR) &sPIF_Info, zsizeof( struct PIF_Struct286 ) );
     SysCloseFile( vTaskLPLR, hFile, 0 );

     // See comment above about setting the read-only attribute.
     unFileAttr |= _A_RDONLY;
     _dos_setfileattr( szFileName, unFileAttr );

   }

   return( TRUE );

} // CreatePIF_File

#endif   // __WIN32__


zOPER_EXPORT zSHORT OPERATION
SetCurrentCompilerSpec( zVIEW vSubtask )
{
   zVIEW  vCompilerSpec;
   zCHAR  szName[ 34 ];

   GetViewByName( &vCompilerSpec, "CompilerSpec", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "cbCurrentCompSpec", szName, 33 );
   SetAttributeFromString( vCompilerSpec, "LPLR", "CurrentCompilerSpec", szName );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
CloseCompilerSpecification( zVIEW vSubtask )
{
   zVIEW   vTaskLPLR;
   zVIEW   vCompilerSpec;
   zCHAR   szCompilerSpec[ 33 ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompilerSpec, "CompilerSpec", vSubtask, zLEVEL_TASK );

   // If there are no compiler specs, then don't perform any validation.
   if ( CheckExistenceOfEntity( vTaskLPLR, "Compiler" ) == zCURSOR_NULL )
   {
      DropView( vCompilerSpec );
      return( 0 );
   }

   // Make sure that the current compiler is valid.
   GetStringFromAttribute( szCompilerSpec, zsizeof( szCompilerSpec ), vCompilerSpec, "LPLR", "CurrentCompilerSpec" );
   if ( SetCursorFirstEntityByString( vCompilerSpec, "Compiler", "Name", szCompilerSpec, 0 ) != zCURSOR_SET )
   {
      zCHAR szMsg[ 200 ];

      if ( zstrcmp( szCompilerSpec, "" ) == 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Current Compiler Specification is required." );
         strcat_s( szMsg, zsizeof( szMsg ), "\n\nDo you want to set Current Compiler Specification?" );
      }
      else
      {
         sprintf_s( szMsg, zsizeof( szMsg ), "Current Compiler Specification '%s' is invalid.", szCompilerSpec );
         strcat_s( szMsg, zsizeof( szMsg ), "\n\nDo you want to set Current Compiler Specification?" );
      }

      if ( MessagePrompt( vSubtask, "LO00105", "Zeidon Compiler",
                          szMsg, zBEEP, zBUTTONS_YESNO,
                          zRESPONSE_NO,  0 ) == zRESPONSE_YES )
      {
         SetFocusToCtrl( vSubtask, "cbCurrentCompSpec" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      }
      else
      {
         DropView( vCompilerSpec );
      }

      return( 0 );
   }

   // If the OI has been changed, then we need to recreate the PIF file.
   if ( ObjectInstanceUpdatedFromFile( vTaskLPLR ) )
   {
#ifdef __WIN32__
      CreateMakeIncludeFile( vSubtask, vTaskLPLR, TRUE );
#else
      CreatePIF_File( vTaskLPLR, 'N' );
#endif
   }

   DropView( vCompilerSpec );

   return( 0 );
}

zBOOL LOCALOPER
fnVerifyExternalTargetFile( zVIEW  vSubtask,
                            zPCHAR pchName,
                            zPCHAR pchValidExtension,
                            zPCHAR pchControlName)
{

   zCHAR     szMsg[ zLONG_MESSAGE_LTH + 1 ];
   zCHAR     szPath[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR     szDrive[ 3 ];
   zCHAR     szDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR     szFilename[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR     szExtension[ zMAX_FILENAME_LTH + 1 ];

   // Check the extension
   // file should have no extension, but it is allowed to have valid extension
   _splitpath( pchName, szDrive, szDir, szFilename, szExtension);
   if ( szExtension[ 0 ] != 0 && zstrcmpi( szExtension, pchValidExtension ) != 0 )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), "%s '%s' has invalid extension: %s", pchControlName, pchName, szExtension );
      MessageSend( vSubtask, "ED1003", "Zeidon Compiler", szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( FALSE ); // We can't ignore the error, makefile would not work
   }

   // build filename without extension
   _makepath( szPath, szDrive, szDir, szFilename, "" );

   // build filename with extension
   _makepath( szPath, szDrive, szDir, szFilename, pchValidExtension );

   // do not check libs, cause it is too much effort to check the LIB variable
   if ( zstrcmpi( pchValidExtension, "LIB" ) == 0 )
      return( TRUE );

   // Check to see if file exists
   if ( SysValidDirOrFile( szPath, FALSE, FALSE, (zSHORT) (zstrlen( szPath ) + 1) ) == FALSE )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), "%s '%s' does not exist.\nIgnore error?", pchControlName, szPath );
      if ( MessagePrompt( vSubtask, "ED1003", "Zeidon Compiler",
                          szMsg, TRUE, zBUTTONS_YESNO, zRESPONSE_NO,
                          zICON_QUESTION ) == zRESPONSE_YES )
      {
         return( TRUE );  // User wants to ignore the error.
      }

      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( FALSE );
   }

   return( TRUE );

} // fnVerifyExternalTargetFile

static BOOL
fnVerifyExternalObjFile( zVIEW vSubtask, zVIEW vTaskLPLR, zPCHAR szFileName )
{
   // Remove leading spaces.
   fnRemoveSpace( szFileName );

   if ( zstrcmp( szFileName, "" ) == 0 )
   {
      DeleteEntity( vTaskLPLR, "ExternalObjFile", zREPOS_NONE );
      return( TRUE );
   }

   if ( !fnVerifyExternalTargetFile( vSubtask, szFileName, ".OBJ", "External Object File" ) )
   {
       TAB_SetActiveTab( vSubtask, "Tab", 3 );
       TG_SetActiveCell( vSubtask, "tgObjectFiles", -1, 0 );
       return( FALSE );
   }

   // set the filename always without extension
   SetAttributeFromString( vTaskLPLR, "ExternalObjFile", "Name", szFileName );

   return( TRUE );
}

static void
fnRemoveSpace( zPCHAR szFileName )
{
   zPCHAR pch;

   // Remove leading spaces.
   for ( pch = szFileName; zisspace( *pch ); pch++ )
   {
      // Nothing needs to be done here ... we're just moving pch to the right spot
   }
   // KJS 11/20/2019 - zsizof() of a pointer only returns 4. Using zsizeof seems incorrect!
   strcpy_s(szFileName, zstrlen(szFileName) + 1, pch);
   //strcpy_s(szFileName, zsizeof(szFileName), pch);
}

static BOOL
fnVerifyExternalLibFile( zVIEW vSubtask, zVIEW vTaskLPLR, zPCHAR pchFileName )
{
   zCHAR szFileName[ zMAX_FILESPEC_LTH + 1 ];
   size_t lLth;

   lLth = zstrlen(pchFileName);
   lLth = zsizeof(pchFileName);
   lLth = zsizeof(szFileName);
   // Remove leading spaces.
   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), pchFileName );
   lLth = zsizeof(szFileName);

   fnRemoveSpace( szFileName );
   if ( zstrcmp( szFileName, "" ) == 0 )
   {
      DeleteEntity( vTaskLPLR, "ExternalLibFile", zREPOS_NONE );
      return( TRUE );
   }

   if ( !fnVerifyExternalTargetFile( vSubtask, szFileName, ".LIB", "External Lib File" ) )
   {
      TAB_SetActiveTab( vSubtask, "Tab", 3 );
      TG_SetActiveCell( vSubtask, "tgLibFiles", -1, 0 );
      return( FALSE );
   }

   // set the filename always without extension
   SetAttributeFromString( vTaskLPLR, "ExternalLibFile", "Name", szFileName );

   return( TRUE );
}


static BOOL
fnVerifyExternalTargetDir( zVIEW vSubtask, zVIEW vTaskLPLR )
{
   zCHAR   szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT  nRC;

   // Now check the external libs and objs
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalTarget", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "ExternalTarget", 0 ) )
   {
      // check objs
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalObjFile", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalObjFile", 0 ) )
      {
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "ExternalObjFile", "Name" );

         if ( fnVerifyExternalObjFile( vSubtask, vTaskLPLR, szFileName ) == FALSE )
            return( FALSE );
      }

      // check libs
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalLibFile", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalLibFile", 0 ) )
      {
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "ExternalLibFile", "Name" );

         if ( fnVerifyExternalLibFile( vSubtask, vTaskLPLR, szFileName ) == FALSE )
            return( FALSE );
      }
   }

   return( TRUE );
}

static BOOL
fnVerifyIncludeLibTargetDir( zVIEW vSubtask, zVIEW vTaskLPLR )
{
   zCHAR   szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT  nRC;

   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Include", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "Include", 0 ) )
   {
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "Include", "IncludeDir" );

      if ( !VerifyDir( vSubtask, szFileName, "Include Directory", FALSE ) )
      {
         TAB_SetActiveTab( vSubtask, "Tab", 2 );
         TG_SetActiveCell( vSubtask, "tgInclude", -1, 0 );
         return( FALSE );
      }

      if ( zstrcmp( szFileName, "" ) == 0 )
         DeleteEntity( vTaskLPLR, "Include", zREPOS_NONE );
   }

   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "Lib", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "Lib", 0 ) )
   {
      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "Lib", "LibDir" );

      if ( !VerifyDir( vSubtask, szFileName, "Lib Directory", FALSE ) )
      {
         TAB_SetActiveTab( vSubtask, "Tab", 2 );
         TG_SetActiveCell( vSubtask, "tgLib", -1, 0 );
         return( FALSE );
      }

      if ( zstrcmp( szFileName, "" ) == 0 )
         DeleteEntity( vTaskLPLR, "Lib", zREPOS_NONE );
   }

   return( TRUE );
}

static void
fnExternalToLibTargetDir( zVIEW vSubtask, zVIEW vTaskLPLR )
{
   zVIEW  vTask2;
   zSHORT nRC;
   zPCHAR pchTarget;

   CreateViewFromViewForTask( &vTask2, vTaskLPLR, 0 );

   for ( nRC = SetCursorFirstEntity( vTask2, "ExternalTarget", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vTask2, "ExternalTarget", 0 ) )
   {
      // skip target "<ALL>"
      GetAddrForAttribute( &pchTarget, vTask2, "ExternalTarget", "Name" );
      if ( zstrcmp( pchTarget, "<ALL>" ) == 0 )
         continue;

      // Set the external Lib/Obj directories into the LibDirectories
      fnExternalLibObjToLibTarget (vTask2, "ExternalLibFile");
      fnExternalLibObjToLibTarget (vTask2, "ExternalObjFile");
   }

   DropView (vTask2);
}

static void
fnExternalLibObjToLibTarget (zVIEW vTask2, zPCHAR pchEntity)
{

   zSHORT nRC;
   zCHAR  szFullFileName[ zMAX_FILESPEC_LTH + zMAX_FILENAME_LTH + zMAX_FILENAME_LTH + 1 ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szExtension[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szDir[ zMAX_FILESPEC_LTH + 1 ];
   //zCHAR  szDrive[10];
   zCHAR  szDrive[ zMAX_FILESPEC_LTH + 1 ]; // KJS 11/20/2019

   for (nRC = SetCursorFirstEntity( vTask2, pchEntity, 0);
        nRC == zCURSOR_SET;
        nRC = SetCursorNextEntity (vTask2, pchEntity, 0) )
   {
      // Get the full qualified Lib-name
      GetStringFromAttribute( szFullFileName, zsizeof( szFullFileName ), vTask2, pchEntity, "Name" );
      // Split into Filename and directory name
      _splitpath( szFullFileName, szDrive, szDir, szFileName, szExtension );
      // Remove \ at last position.
      szDir[ strlen(szDir) - 1 ] = 0;
	  // KJS 11/20/2019 - I don't understand... but when szDir=\10C\W\LIBZ (which looks to me to be length of 11)
	  // zsizeof returns 10, strlen returns 11. I'm going to change the following to be zstrlen...
	  // And it should be the size of szDir, not szDrive
	  //strcat_s(szDrive, zsizeof(szDrive), szDir);
	  strcat_s(szDrive, zsizeof(szDir), szDir);
	  //strcat_s(szDrive, strlen(szDrive), szDir);

      // put the directoryname into the Library list. From this library list
      // Variable LIB will be build.
      nRC = SetCursorFirstEntityByString( vTask2, "Lib", "LibDir", szDrive, "" );
      if ( nRC < 0 )
      {
         CreateEntity( vTask2, "Lib", zPOS_AFTER );
         SetAttributeFromString( vTask2, "Lib", "LibDir", szDrive );
      }
   }
}

static zSHORT
fnVerifyExternalAllDir( zVIEW vSubtask, zVIEW vTaskLPLR )
{
   zVIEW   vTemp;
   zSHORT  nRC;
   zCHAR   szMsg[ zLONG_MESSAGE_LTH + 1 ];
   zPCHAR  pchName;
   zPCHAR  pchTarget;

   // check for duplicate libs (defined for a single target and "<ALL>" targets
   CreateViewFromViewForTask( &vTemp, vTaskLPLR, 0 );
   SetCursorFirstEntityByString( vTemp, "ExternalTarget", "Name", "<ALL>", "" );

   // go through all ExternalTarges
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalTarget", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "ExternalTarget", 0 ) )
   {
      // skip target "<ALL>"
      GetAddrForAttribute( &pchTarget, vTaskLPLR, "ExternalTarget", "Name" );
      if ( zstrcmp( pchTarget, "<ALL>" ) == 0 )
         continue;

      // check libs
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalLibFile", 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalLibFile", 0 ) )
      {
         GetAddrForAttribute( &pchName, vTaskLPLR, "ExternalLibFile", "Name" );

         // Do we have the name for our target in "<ALL>" targets, too?
         nRC = SetCursorFirstEntityByString( vTemp, "ExternalTarget", "Name",
                                             pchName, "" );
         if ( nRC >= 0 )
         {
            // Show a warning, then delete the item for our target
            sprintf_s( szMsg, zsizeof( szMsg ), "Duplicate lib \"%s\" in target \"%s\" and in \"<ALL>\"",
                       pchName, pchTarget );
            MessageSend( vSubtask, "ED00999", "Zeidon Compiler",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            DeleteEntity( vTaskLPLR, "ExternalLibFile", zREPOS_PREV );
            continue;
         }
      }

      // check objs
      for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalObjFile", 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vTaskLPLR, "ExternalObjFile", 0 ) )
      {
         GetAddrForAttribute( &pchName, vTaskLPLR, "ExternalObjFile", "Name" );

         // Do we have the name for our target in "<ALL>" targets, too?
         nRC = SetCursorFirstEntityByString( vTemp, "ExternalTarget", "Name",  pchName, "" );
         if ( nRC >= 0 )
         {
            // Show a warning, then delete the item for our target
            sprintf_s( szMsg, zsizeof( szMsg ), "Duplicate object \"%s\" in target \"%s\" and in \"<ALL>\"",
                       pchName, pchTarget );
            MessageSend( vSubtask, "ED00999", "Zeidon Compiler",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            DeleteEntity( vTaskLPLR, "ExternalObjFile", zREPOS_PREV );
            continue;
         }
      }
   }

   DropView( vTemp );
   return( 0 );
}


static zSHORT
fnRemoveTargetsWithoutLibsObjs( zVIEW vTaskLPLR )
{
   zSHORT  nRC;

   // remove targets without libs or objs

   // go through all ExternalTarges
   for ( nRC = SetCursorFirstEntity( vTaskLPLR, "ExternalTarget", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vTaskLPLR, "ExternalTarget", 0 ) )
   {
      // if we have no external file
      if ( SetCursorFirstEntity( vTaskLPLR, "ExternalObjFile", 0 ) != zCURSOR_SET &&
           SetCursorFirstEntity( vTaskLPLR, "ExternalLibFile", 0 ) != zCURSOR_SET )
      {
         // Delete it
         DeleteEntity( vTaskLPLR, "ExternalTarget", zREPOS_PREV );
      }
   }

   return( 0 );
}


static zSHORT
fnSetNameForCurrentCompilerSpec( zVIEW vSubtask, zVIEW vTaskLPLR )
{
   zVIEW vCompilerSpec;

   // set new Name for Current Compiler Spec
   GetViewByName( &vCompilerSpec, "CompilerSpec", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vTaskLPLR, "Compiler", "CurrentCompilerSpec", "Y" ) == 0 )
   {
      SetAttributeFromAttribute( vCompilerSpec, "LPLR", "CurrentCompilerSpec",
                                 vTaskLPLR, "Compiler", "Name" );
   }

   return( 0 );
}


static zSHORT
fnCheckCompilerSpec( zVIEW vSubtask, zVIEW vTaskLPLR )
{
   zCHAR  szName[ 33 ];
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zVIEW  vCopy;
   zSHORT nRC;

   if ( TAB_GetActiveTab( vSubtask, "Tab" ) != 1 )
      TAB_SetActiveTab( vSubtask, "Tab", 1 );

   GetCtrlText( vSubtask, "edName", szName, 33 );
   UfCompressName( szName, szName, 32, "", "", "", "", 0 );

   // if new name required.
   if ( zstrcmp( szName, "" ) == 0 )
   {
      MessageSend( vSubtask, "CM00283", "Zeidon Compiler",
                   "Specification Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ERROR, TRUE );
      SetFocusToCtrl( vSubtask, "edName" );
      return( -1 );
   }

   SetAttributeFromString( vTaskLPLR, "Compiler", "Name", szName );

   // if new name not unique.
   CreateViewFromViewForTask( &vCopy, vTaskLPLR, 0 );

   SetCursorFirstEntityByString( vCopy, "Compiler", "Name", szName, "" );
   nRC = SetCursorNextEntityByString( vCopy, "Compiler", "Name", szName, "" );

   DropView( vCopy );

   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "CM00283", "Zeidon Compiler",
                   "Specification Name already exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ERROR, TRUE );
      SetFocusToCtrl( vSubtask, "edName" );
      return( -1 );
   }

   GetCtrlText( vSubtask, "Compiler Directory", szFileName, zMAX_FILESPEC_LTH + 1 );
   UfCompressName( szFileName, szFileName, zMAX_FILESPEC_LTH, "", "", "", "", 0 );

   // if Compiler Directory required.
   if ( zstrcmp( szFileName, "" ) == 0 )
   {
      MessageSend( vSubtask, "CM00283", "Zeidon Compiler",
                   "Compiler Directory is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetCtrlState( vSubtask, "Compiler Directory", zCONTROL_STATUS_ERROR, TRUE );
      SetFocusToCtrl( vSubtask, "Compiler Directory" );
      return( -1 );
   }

   MapWindow( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VerifyCompilerSpecOptions( zVIEW vSubtask )
{
   zVIEW     vTaskLPLR;
   zCHAR     szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR     szBaseDir[zMAX_FILESPEC_LTH + 1];
   zSHORT    nRC = -1;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( fnCheckCompilerSpec( vSubtask, vTaskLPLR ) < 0 )
      return( nRC );

   // Force compiler to MSC 1.5 because FindCompilerType in tzlooprs.c requires it
   // for Microsoft compiles.
   SetAttributeFromString( vTaskLPLR, "Compiler", "CompilerName", "MSC 1.5" );

   // Basedirectory for relative pathes is the component source directory.
   GetStringFromAttribute( szBaseDir, zsizeof( szBaseDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "Compiler", "TargetExecutableDir" );
   if ( IntGenFullQualPathFromRelPath( vSubtask, szFileName, szBaseDir, zMAX_FILENAME_LTH - 2, "Target Executable Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   if ( !VerifyDir( vSubtask, szFileName, "Target Executable Directory", TRUE ) )
      goto EndOfFunction;

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "Compiler", "TargetObjectDir" );
   if ( IntGenFullQualPathFromRelPath( vSubtask, szFileName, szBaseDir, zMAX_FILENAME_LTH - 2, "OBJ Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   if ( !VerifyDir( vSubtask, szFileName, "OBJ Directory", TRUE ) )
      goto EndOfFunction;

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "Compiler", "MakefileDir" );
   if ( IntGenFullQualPathFromRelPath( vSubtask, szFileName, szBaseDir, zMAX_FILENAME_LTH - 2, "Makefile Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   if ( !VerifyDir( vSubtask, szFileName, "Makefile Directory", TRUE ) )
      goto EndOfFunction;

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "Compiler", "EnvironmentDir" );
   if ( IntGenFullQualPathFromRelPath( vSubtask, szFileName, szBaseDir, zMAX_FILENAME_LTH - 2, "Resource Directory" ) == -1 )
   {
      goto EndOfFunction;
   }

   if ( !VerifyDir( vSubtask, szFileName, "Resource Directory", TRUE ) )
      goto EndOfFunction;

   GetStringFromAttribute( szBaseDir, zsizeof( szBaseDir ), vTaskLPLR, "Compiler", "CompilerDir" );
   SysConvertEnvironmentString( szFileName, zsizeof( szFileName ), szBaseDir );
   SysAppendcDirSep( szFileName );
   strcat_s( szFileName, zsizeof( szFileName ), "NMAKE.EXE" );
   if ( SysOpenFile( vSubtask, szFileName, COREFILE_EXIST ) == -1 )
   {
      if ( MessagePrompt( vSubtask, "ED1000", "Zeidon Compiler",
                          "Could not find 'NMAKE.EXE' in specified compiler "
                          "directory.\nIgnore this error?",
                          TRUE, zBUTTONS_YESNO,
                          zRESPONSE_NO, zICON_STOP ) == zRESPONSE_NO )
      {
         SetCtrlState( vSubtask, "Compiler Directory", zCONTROL_STATUS_ERROR, TRUE );
         SetFocusToCtrl( vSubtask, "Compiler Directory" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         goto EndOfFunction;
      }
   }

   // Now check the lib and include directories.
   if ( !fnVerifyIncludeLibTargetDir( vSubtask, vTaskLPLR ) )
      goto EndOfFunction;

   // Now check the external libs and objs
   if ( !fnVerifyExternalTargetDir( vSubtask, vTaskLPLR ) )
      goto EndOfFunction;

   // check for duplicate libs (defined for a single target and "<ALL>" targets
   fnVerifyExternalAllDir( vSubtask, vTaskLPLR );

   // remove targets without libs or objs
   fnRemoveTargetsWithoutLibsObjs( vTaskLPLR );

   // Set the external lib/obj directories into the lib directories for LIB definition
   fnExternalToLibTargetDir( vSubtask, vTaskLPLR );

   if ( AcceptSubobject( vTaskLPLR, "Compiler" ) == 0 )
   {
      // set new Name for Current Compiler Spec
      fnSetNameForCurrentCompilerSpec( vSubtask, vTaskLPLR );
      nRC = 0;
   }

EndOfFunction:

   return( nRC );

} // VerifyCompilerSpecOptions

zOPER_EXPORT zSHORT OPERATION
ExternalTargetLibFile_Browse( zVIEW vSubtask )
{
   zVIEW   vTaskLPLR;
   zCHAR   szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR   szMetaSrcDir[ zMAX_FILESPEC_LTH * 2 + 1 ];
   zSHORT  nRC;

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vTaskLPLR, "ExternalLibFile" ) >= zCURSOR_SET )
      {
         SetAllSelectStatesForEntity( vTaskLPLR, "ExternalLibFile", FALSE, "" );
         SetSelectStateOfEntity( vTaskLPLR, "ExternalLibFile", TRUE );
         RefreshCtrl( vSubtask, "tgLibFiles" );
      }

      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "ExternalLibFile", "Name" );
      fnRemoveSpace( szFileName );

      if ( zstrcmp( szFileName, "" ) == 0 ||
           CompareAttributeToString( vTaskLPLR, "Compiler", "BrowseInfo", "Y" ) != 0 )
      {
         GetStringFromAttribute( szMetaSrcDir, zsizeof( szMetaSrcDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );
         _chdir( szMetaSrcDir );
      }

      // do a loop for input of filename until the filename is valid or the user pressed cancel...
      do
      {
         nRC = OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ),
                                      "Library Files (*.LIB)|*.lib|", "LIB",
                                      zOFN_HIDEREADONLY | zOFN_FILEMUSTEXIST );
      } while ( nRC == 1 &&
                !fnVerifyExternalTargetFile( vSubtask, szFileName, ".LIB", "External Library File" ) );

      // user pressed OK, so the filename must be valid:
      if ( nRC == 1 )
      {
         SetAttributeFromString( vTaskLPLR, "ExternalLibFile", "Name", szFileName );

         // save Flag for last Browse Directory
         SetAttributeFromString( vTaskLPLR, "Compiler", "BrowseInfo", "Y" );

         RefreshCtrl( vSubtask, "tgLibFiles" );
      }
   }

   TG_SetActiveCell( vSubtask, "tgLibFiles", -1, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ExternalTargetObjFile_Browse( zVIEW vSubtask )
{
   zVIEW   vTaskLPLR;
   zCHAR   szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR   szMetaSrcDir[ zMAX_FILESPEC_LTH * 2 + 1 ];
   zSHORT  nRC;

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vTaskLPLR, "ExternalObjFile" ) >= zCURSOR_SET )
      {
         SetAllSelectStatesForEntity( vTaskLPLR, "ExternalObjFile", FALSE, "" );
         SetSelectStateOfEntity( vTaskLPLR, "ExternalObjFile", TRUE );
         RefreshCtrl( vSubtask, "tgObjectFiles" );
      }

      GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "ExternalObjFile", "Name");
      fnRemoveSpace( szFileName );

      if ( zstrcmp( szFileName, "" ) == 0 ||
           CompareAttributeToString( vTaskLPLR, "Compiler", "BrowseInfo", "Y" ) != 0 )
      {
         GetStringFromAttribute( szMetaSrcDir, zsizeof( szMetaSrcDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );
         _chdir( szMetaSrcDir );
      }

      // do a loop for input of filename until the filename is valid or the user pressed cancel...
      do
      {
         nRC = OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ),
                                      "Object Files (*.OBJ)|*.obj|", "OBJ",
                                      zOFN_HIDEREADONLY | zOFN_FILEMUSTEXIST );

      } while ( nRC == 1 &&
                !fnVerifyExternalTargetFile( vSubtask, szFileName, ".OBJ", "External Object File" ) );

      // user pressed OK, so the filename must be valid:
      if ( nRC == 1 )
      {
         SetAttributeFromString( vTaskLPLR, "ExternalObjFile", "Name", szFileName );

         // save Flag for last Browse Directory
         SetAttributeFromString( vTaskLPLR, "Compiler", "BrowseInfo", "Y" );

         RefreshCtrl( vSubtask, "tgObjectFiles" );
      }
   }

   TG_SetActiveCell( vSubtask, "tgObjectFiles", -1, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FindComponent( zVIEW vSubtask )
{
   zCHAR  szComponentName[ 256 ];
   zVIEW  vTaskLPLR;
   zVIEW  vLPLR;
   zSHORT nRC;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetCtrlText( vSubtask, "FindComponentName", szComponentName, 255 );
   if ( szComponentName[ 0 ] )
   {
      CreateViewFromViewForTask( &vLPLR, vTaskLPLR, 0 );
      nRC = SetCursorFirstEntity( vLPLR, "TargetExecutable", "" );
      while ( nRC >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByString( vLPLR, "Meta", "Name", szComponentName, "" ) >= zCURSOR_SET )
         {
            SetViewFromView( vTaskLPLR, vLPLR );
            SetAllSelectStatesForEntity( vTaskLPLR, "TargetExecutable", FALSE, "" );
            SetSelectStateOfEntity( vTaskLPLR, "TargetExecutable", TRUE );
            SetAllSelectStatesForEntity( vTaskLPLR, "Meta", FALSE, "" );
            SetSelectStateOfEntity( vTaskLPLR, "Meta", TRUE );
            OrderEntityForView( vTaskLPLR, "Meta", "Name A" );
            RefreshCtrl( vSubtask, "TargetCompList" );
            RefreshCtrl( vSubtask, "TargetList" );  // TZEDCMPD.TargetSpecification.TargetList
            break;
         }

         nRC = SetCursorNextEntity( vLPLR, "TargetExecutable", "" );
      }

      DropView( vLPLR );
   }
   else
   {
      MessageSend( vSubtask, "ED00???", "Find Component",
                   "Find Component requires a Component Name",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
RefreshTargetList( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zVIEW  vLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vLPLR, vTaskLPLR, 0 );
   if ( SetCursorFirstSelectedEntity( vLPLR, "TargetExecutable", "" ) >= zCURSOR_SET )
      SetCtrlState( vSubtask, "MakeTarget", zCONTROL_STATUS_ENABLED, TRUE );
   else
      SetCtrlState( vSubtask, "MakeTarget", zCONTROL_STATUS_ENABLED, FALSE );

   DropView( vLPLR );

   OrderEntityForView( vTaskLPLR, "Meta", "Name A" );
   SetAllSelectStatesForEntity( vTaskLPLR, "Meta", FALSE, "" );
   RefreshCtrl( vSubtask, "TargetList" );  // TZEDCMPD.TargetSpecification.TargetList

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: PostbuildTargetList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
PostbuildTargetList( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zBOOL  bEnable = FALSE;
   zCHAR  szCurrentCompilerSpec[ 33 ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   // if Compiler Spec does not exists, Zeidon create it in PreBuild
   // if Compiler Spec from Zeidon created, Compiler Dir is empty
   GetStringFromAttribute( szCurrentCompilerSpec, zsizeof( szCurrentCompilerSpec ), vTaskLPLR, "LPLR",
                           "CurrentCompilerSpec" );
   if ( CheckExistenceOfEntity( vTaskLPLR, "Compiler" ) >= zCURSOR_SET &&
        zstrcmp( szCurrentCompilerSpec, "" ) != 0 &&
        SetCursorFirstEntityByString( vTaskLPLR, "Compiler", "Name",
                                      szCurrentCompilerSpec, "" ) >= zCURSOR_SET &&
        CompareAttributeToString( vTaskLPLR, "Compiler", "CompilerDir", "" ) != 0 )
   {
      bEnable = TRUE;
   }

   SetCtrlState( vSubtask, "rbParseGenerateCompileAsNeeded", zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "rbParseGenerateCompile", zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "rbGenerateCompile", zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "rbCompile", zCONTROL_STATUS_ENABLED, bEnable );

   RefreshTargetList( vSubtask );

   return( 0 );
} // PostbuildTargetList

zOPER_EXPORT zSHORT OPERATION
DeleteCompilerSpec( zVIEW vSubtask )
{
   zVIEW     vTaskLPLR;
   zCHAR     szMsg[ zSHORT_MESSAGE_LTH + 1 ];
   zCHAR     szCompilerName[ 33 ];
   zSHORT    nRC = -1;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szCompilerName, zsizeof( szCompilerName ), vTaskLPLR, "Compiler", "Name" );

   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete Compiler Specification '" );
   strcat_s( szMsg, zsizeof( szMsg ), szCompilerName );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );

   if ( MessagePrompt( vSubtask, "PE002", "Zeidon Compiler",
                       szMsg, zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,  0 ) == zRESPONSE_YES )
   {
      nRC = 0;
      DeleteEntity( vTaskLPLR, "Compiler", zREPOS_PREV );

      //Check Current Compiler Spec
      if ( CompareAttributeToString( vTaskLPLR, "LPLR", "CurrentCompilerSpec",
                                     szCompilerName ) == 0 )
      {
         SetAttributeFromString( vTaskLPLR, "LPLR", "CurrentCompilerSpec", "" );
      }
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: EditCompilerSpecification
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
EditCompilerSpecification( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zVIEW  vCompilerSpec;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCompilerSpec, "CompilerSpec", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToAttribute( vCompilerSpec, "LPLR", "CurrentCompilerSpec",
                                     vTaskLPLR, "Compiler", "Name" ) == 0 )
   {
      SetAttributeFromString( vTaskLPLR, "Compiler", "CurrentCompilerSpec", "Y" );
   }
   else
   {
      SetAttributeFromString( vTaskLPLR, "Compiler", "CurrentCompilerSpec", "N" );
   }

   // save Flag for last Browse Directory
   SetAttributeFromString( vTaskLPLR, "Compiler", "BrowseInfo", "N" );
   SetAttributeFromInteger( vTaskLPLR, "Compiler", "CreateExternalList", 0 );

   CreateTemporalSubobjectVersion( vTaskLPLR, "Compiler" );

   return( 0 );

} // EditCompilerSpecification

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: NextCompilerSpecification
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
NextCompilerSpecification( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zVIEW  vParentWindow;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( VerifyCompilerSpecOptions( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 0 );
   SetCursorNextEntity( vTaskLPLR, "Compiler", "" );

   EditCompilerSpecification( vSubtask );
   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 1 );

   SetFocusToCtrl( vSubtask, "edName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );

} // NextCompilerSpecification

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: PrevCompilerSpecification
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
PrevCompilerSpecification( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zVIEW  vParentWindow;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( VerifyCompilerSpecOptions( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 0 );
   SetCursorPrevEntity( vTaskLPLR, "Compiler", "" );

   EditCompilerSpecification( vSubtask );
   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 1 );

   SetFocusToCtrl( vSubtask, "edName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );

} // PrevCompilerSpecification

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: NewCompilerSpecification
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
NewCompilerSpecification( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zVIEW  vParentWindow;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( VerifyCompilerSpecOptions( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 0 );

   CreateTemporalEntity( vTaskLPLR, "Compiler", zPOS_AFTER );
   SetAttributeFromString( vTaskLPLR, "Compiler", "CurrentCompilerSpec", "N" );
   SetAttributeFromInteger( vTaskLPLR, "Compiler", "CreateExternalList", 0 );
   SetAttributeFromString( vTaskLPLR, "Compiler", "BrowseInfo", "N" );

   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 1 );
   SetFocusToCtrl( vSubtask, "edName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );

} // NewCompilerSpecification

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DeleteCompilerSpecification
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DeleteCompilerSpecification( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zVIEW  vParentWindow;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( DeleteCompilerSpec( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vTaskLPLR, "Compiler" ) == zCURSOR_NULL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
   }
   else
   {
      if ( TAB_GetActiveTab( vSubtask, "Tab" ) != 1 )
         TAB_SetActiveTab( vSubtask, "Tab", 1 );

      EditCompilerSpecification( vSubtask );
      SetSelectStateOfEntity( vTaskLPLR, "Compiler", 1 );
      SetFocusToCtrl( vSubtask, "edName" );
   }

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );

} // DeleteCompilerSpecification

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CopyCompilerSpecification
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CopyCompilerSpecification( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zVIEW  vParentWindow;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( VerifyCompilerSpecOptions( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 0 );

   CopyCompilerSpec( vSubtask );
   SetAttributeFromInteger( vTaskLPLR, "Compiler", "CreateExternalList", 0 );
   SetAttributeFromString( vTaskLPLR, "Compiler", "BrowseInfo", "N" );

   SetSelectStateOfEntity( vTaskLPLR, "Compiler", 1 );

   SetFocusToCtrl( vSubtask, "edName" );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );

} // CopyCompilerSpecification

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: IncludeFile_AddNew
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
IncludeFile_AddNew( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   CreateEntity( vTaskLPLR, "Include", zPOS_LAST );

   RefreshWindowExceptForCtrl( vSubtask, "tgInclude" );

   return( 0 );

} // IncludeFile_AddNew

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: IncludeFile_New
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
IncludeFile_New( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTaskLPLR, "Include" ) >= zCURSOR_SET )
      SetAllSelectStatesForEntity( vTaskLPLR, "Include", FALSE, "" );

   CreateEntity( vTaskLPLR, "Include", zPOS_LAST );

   RefreshWindowExceptForCtrl( vSubtask, "tgLib" );
   TG_SetActiveCell( vSubtask, "tgInclude", -1, 0 );

   return( 0 );

} // IncludeFile_New

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LibFile_AddNew
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
LibFile_AddNew( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   CreateEntity( vTaskLPLR, "Lib", zPOS_LAST );

   RefreshWindowExceptForCtrl( vSubtask, "tgLib" );

   return( 0 );

} // LibFile_AddNew

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LibFile_New
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
LibFile_New( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTaskLPLR, "Lib" ) >= zCURSOR_SET )
      SetAllSelectStatesForEntity( vTaskLPLR, "Lib", FALSE, "" );

   CreateEntity( vTaskLPLR, "Lib", zPOS_LAST );

   RefreshWindowExceptForCtrl( vSubtask, "tgInclude" );
   TG_SetActiveCell( vSubtask, "tgLib", -1, 0 );

   return( 0 );

} // LibFile_New

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: IncludeFile_Delete
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
IncludeFile_Delete( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zSHORT nRC;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTaskLPLR, "Include" ) >= zCURSOR_SET )
   {
      SetSelectStateOfEntity( vTaskLPLR, "Include", TRUE );
      RefreshCtrl( vSubtask, "tgInclude" );

      if ( MessagePrompt( vSubtask, "ED1004", "Zeidon Compiler",
                          "OK to delete selected Include Files?",
                          0, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_YES )
      {
         for ( nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "Include", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextSelectedEntity( vTaskLPLR, "Include", 0 ) )
         {
            DeleteEntity( vTaskLPLR, "Include", zREPOS_NONE );
         }

         TG_SetActiveCell( vSubtask, "tgInclude", -1, 0 );
      }
   }

   return( 0 );

} // IncludeFile_Delete

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LibFile_Delete
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
LibFile_Delete( zVIEW vSubtask )
{
   zVIEW  vTaskLPLR;
   zSHORT nRC;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTaskLPLR, "Lib" ) >= zCURSOR_SET )
   {
      SetSelectStateOfEntity( vTaskLPLR, "Lib", TRUE );
      RefreshCtrl( vSubtask, "tgLib" );

      if ( MessagePrompt( vSubtask, "ED1004", "Zeidon Compiler",
                          "OK to delete selected Lib Files?", 0,
                          zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_YES )
      {
         for ( nRC = SetCursorFirstSelectedEntity( vTaskLPLR, "Lib", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextSelectedEntity( vTaskLPLR, "Lib", 0 ) )
         {
            DeleteEntity( vTaskLPLR, "Lib", zREPOS_NONE );
         }

         TG_SetActiveCell( vSubtask, "tgLib", -1, 0 );
      }
   }

   return( 0 );

} // LibFile_Delete
